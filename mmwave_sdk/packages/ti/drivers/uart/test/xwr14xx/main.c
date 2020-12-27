/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the UART
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/arm/v7a/Pmu.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/esm/esm.h>
#include <ti/utils/testlogger/logger.h>

/* MCPI Global Variable which is used to select run
 * the original unit test or MCPI uart echo test
 */
uint8_t         gMCPIUARTEchoTestEnable = 0;

/**************************************************************************
 ************************** External Definitions **************************
 **************************************************************************/

/* Common Test Functions: */
extern int32_t Test_uartAPI (void);
extern int32_t Test_profileTransmit(uint32_t uartInstanceNum);
extern int32_t Test_pollingData(void);
extern int32_t Test_blockingData(void);
extern int32_t Test_DMAReadWrite(void);
extern void    Test_echo(UArg a0, UArg a1);

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/* Global Variable which holds the CPU Clock Frequency */
uint32_t        gCPUClockFrequency = (200 * 1000000);

/* Global Variable which holds the DMA Instance */
DMA_Handle   gDMAHandle;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**************************************************************************
 *************************** SOC UART Startup code *************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This function starts the PMU counter.
 *
 *   @param[in] counter
 *      Counter id used for benchmarking
 *
 *  @retval
 *      Not Applicable.
 */
void Test_benchmarkStart(uint32_t counter)
{
    /* Initialize counter to count cycles */
    Pmu_configureCounter(counter, 0x11, FALSE);

    /* Reset PMU counter */
    Pmu_resetCount(counter);

    /* Start PMU counter */
    Pmu_startCounter(counter);
}

/**
 *  @b Description
 *  @n
 *      This function stops a PMU counter and returns the current
 *      counter value.
 *
 *   @param[in] counter
 *      Counter id used for benchmarking
 *
 *  @retval
 *      Current PMU counter value.
 */
uint32_t Test_benchmarkStop(uint32_t counter)
{
    /* Stop PMU counter */
    Pmu_stopCounter(counter);

    /* Read PMU counter */
    return (Pmu_getCount(counter));
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the UART Driver HW Capabilities.
 *      This is device specific testing.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t Test_HwCapabilities(void)
{
    UART_Params     params;
    UART_Handle     handle;
    int32_t         status;
    uint8_t         rxData;
    uint32_t        index;
    uint8_t         txData = 'A';

    /* Debug Message: */
    System_printf("*************************************************************\n");
    System_printf("Debug: Testing HW Capabilities\n");
    System_printf("*************************************************************\n");

    /* Setup the default UART Parameters */
    UART_Params_init(&params);
    params.clockFrequency = gCPUClockFrequency;
    params.isPinMuxDone   = 0;

    /* UART-1 requires PIN MUX but the application does not acknowledge it. The UART
     * Driver should fail */
    handle = UART_open(0, &params);
    if (handle != NULL)
    {
        System_printf("Error: Opened the UART-1 with Invalid PIN MUX capabilties\n");
        return -1;
    }
    System_printf("Debug: UART-1 PIN Mux capability test passed\n");

    /* Setup the default UART Parameters */
    UART_Params_init(&params);
    params.clockFrequency = gCPUClockFrequency;
    params.isPinMuxDone   = 0;

    /* Open the UART-3 Instance: */
    handle = UART_open(1, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the UART-3\n");
        return -1;
    }

    /* We cannot read from the UART-3 Instance: We should fail always */
    status = UART_read (handle, &rxData, 1);
    if (status != UART_EINVAL)
    {
        System_printf("Error: Read from the UART-3 was successful [Status = %d]\n", status);
        return -1;
    }
    status = UART_readPolling (handle, &rxData, 1);
    if (status != UART_EINVAL)
    {
        System_printf("Error: Polling Read from the UART-3 was successful [Status = %d]\n", status);
        return -1;
    }
    System_printf("Debug: UART-3 Read capability test passed\n");

    /****************************************************************************
     * Sanity Check: UART-3 is capable of only transmitting data
     ****************************************************************************/
    for (index = 0; index < 64; index++)
    {
        /* Write out the character: */
        status = UART_writePolling(handle, &txData, 1);
        if (status < 0)
        {
            System_printf("Error: Unable to send out the data on UART-3 [Error %d]\n", status);
            return -1;
        }

        /* Next iteration; next character */
        if (txData == 'Z')
        {
            txData = 'A';
        }
        else
        {
            txData++;
        }
    }
    System_printf ("Debug: Write to the UART-3 was successful. Please verify the Console application.\n");

    /* Close the driver: */
    UART_close (handle);

    /**************************************************************************
     * The commented out tests need to be enabled only if the UART Driver
     * has been configured to support the DMA mode. The current default driver
     * build for xwr14xx does not support DMA.
     **************************************************************************/
#if 0
    /**************************************************************************
     * Test: Profile DMA vs. Non-DMA UART Profiling for UART-3
     **************************************************************************/
    if (Test_profileTransmit (1U) < 0)
    {
        MCPI_setFeatureTestResult ("UART3 DMA/Normal Profile", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("UART3 DMA/Normal Profile", MCPI_TestResult_PASS);
#endif
    return 0;
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    Task_Params     taskParams;
    DMA_Params      dmaParams;
    int32_t         errCode;

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize the UART */
    UART_init();

    /* Initialize the DMA */
    DMA_init ();

    /* Open the DMA Instance */
    DMA_Params_init(&dmaParams);
    gDMAHandle = DMA_open(0, &dmaParams, &errCode);
    if (gDMAHandle == NULL)
    {
        printf ("Error: Unable to open the DMA Instance [Error code %d]\n", errCode);
        return;
    }

    /* Setup the PINMUX to bring out the UART-1 */
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINN6_PADBE, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINN6_PADBE, SOC_XWR14XX_PINN6_PADBE_MSS_UARTA_TX);
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINN5_PADBD, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINN5_PADBD, SOC_XWR14XX_PINN5_PADBD_MSS_UARTA_RX);

    /* Debug Message: */
    if (gMCPIUARTEchoTestEnable == 0)
    {
        System_printf ("Debug: UART has been initialized\n");
    }

    if (gMCPIUARTEchoTestEnable == 1)
    {
        /* Initialize the Task Parameters. */
        Task_Params_init(&taskParams);
        taskParams.stackSize = 4*1024;
        Task_create(Test_echo, &taskParams, NULL);
        return;
    }

    /**************************************************************************
     * Test: UART Exported API
     **************************************************************************/
    if (Test_uartAPI () < 0)
    {
        MCPI_setFeatureTestResult ("Basic UART API", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("Basic UART API", MCPI_TestResult_PASS);

    /**************************************************************************
     * The commented out tests need to be enabled only if the UART Driver
     * has been configured to support the DMA mode. The current default driver
     * build for xwr14xx does not support DMA.
     **************************************************************************/
#if 0
    /**************************************************************************
     * Test: DMA Read and Write operations
     **************************************************************************/
    if (Test_DMAReadWrite () < 0)
    {
        MCPI_setFeatureTestResult ("DMA UART", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DMA UART", MCPI_TestResult_PASS);

    /**************************************************************************
     * Test: Profile DMA vs. Non-DMA UART Profiling
     **************************************************************************/
    if (Test_profileTransmit (0U) < 0)
    {
        MCPI_setFeatureTestResult ("DMA/Normal Profile", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DMA/Normal Profile", MCPI_TestResult_PASS);
#endif

    /**************************************************************************
     * Test: SOC Specific Hardware Capabilities
     **************************************************************************/
    if (Test_HwCapabilities() != 0)
    {
        MCPI_setFeatureTestResult ("UART SOC Hw Capabilties", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("UART SOC Hw Capabilties", MCPI_TestResult_PASS);

    /********************************************************************
     * Test: Polled Data operations on the UART Driver
     ********************************************************************/
    if (Test_pollingData() != 0)
    {
        MCPI_setFeatureTestResult ("UART Polling Data", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("UART Polling Data", MCPI_TestResult_PASS);

    /********************************************************************
     * Test: Blocking Data operations on the UART Driver
     * - MCPI Logging is done within the test
     ********************************************************************/
    if (Test_blockingData() != 0)
        return;

	return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
 *
 *  @retval
 *      Not Applicable.
 */
int32_t main (void)
{
	Task_Params     taskParams;
    int32_t         errCode;
    SOC_Handle      socHandle;
    SOC_Cfg         socCfg;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Debug Message: */
    if (gMCPIUARTEchoTestEnable == 0)
    {
        System_printf ("******************************************\n");
        System_printf ("Debug: Launching the UART Test Application\n");
        System_printf ("******************************************\n");
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Debug Message: */
    if (gMCPIUARTEchoTestEnable == 0)
    {
        System_printf ("Debug: Launching BIOS [Reset the FIQ]\n");
    }

    /* Start BIOS */
	BIOS_start();
    return 0;
}

