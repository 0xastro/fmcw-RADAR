/*
 *   @file  main_dss.c
 *
 *   @brief
 *      Unit Test code for the UART on the XWR18xx DSS
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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
#include <c6x.h>

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
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#include <ti/sysbios/family/c64p/Cache.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 ************************** External Definitions **************************
 **************************************************************************/

/* Common Test Functions: */
extern int32_t Test_profileTransmit(uint32_t uartInstanceNum);

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/* Global Variable which holds the UART Clock Frequency */
uint32_t        gCPUClockFrequency = (200 * 1000000);

/* Global Variable which holds the timestamp */
uint32_t        gBenchmarkStart;

/* Global Variable which holds the EDMA Handle */
EDMA_Handle     gEDMAHandle;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**************************************************************************
 *************************** UART DSS Unit Test ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Registered callback function with the EDMA to be invoked on an error
 *
 *  @retval
 *      Not Applicable.
 */
void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      Registered callback function with the EDMA to be invoked on an error
 *
 *  @retval
 *      Not Applicable.
 */
void Test_edmaTransferControllerErrorCallbackFxn
(
    EDMA_Handle                         handle,
    EDMA_transferControllerErrorInfo_t* errorInfo
)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the event combiner.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_setupEventCombiner (void)
{
    Hwi_Params  params;
    uint32_t    i;

    Hwi_Params_init(&params);
    params.enableInt = TRUE;
    for (i = 0; i < 4; i++)
    {
        params.arg      = i;
        params.eventId  = i;
        if (Hwi_create(4 + i, &EventCombiner_dispatch, &params, NULL) == NULL)
        {
            System_printf("failed to create Hwi interrupt %d\n",4 + i);
        }
    }
    return;
}

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
    gBenchmarkStart = TSCL;
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
    return (TSCL - gBenchmarkStart);
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
    uint32_t        baudRate = 115200;
    uint8_t         txData = 'A';
    uint8_t*        ptrTestBanner = "DSS Test Banner\n";

    /* Debug Message: */
    System_printf("*************************************************************\n");
    System_printf("Debug: Testing HW Capabilities @ Baud Rate: %d\n", baudRate);
    System_printf("*************************************************************\n");

    /* Setup the default UART Parameters */
    UART_Params_init(&params);
    params.baudRate       = baudRate;
    params.isPinMuxDone   = 1;

    /* Open the UART-1 Instance: */
    handle = UART_open(0, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the UART-1\n");
        return -1;
    }

    /* We cannot read from the UART-1 Instance: We should fail always */
    status = UART_read (handle, &rxData, 1);
    if (status != UART_EINVAL)
    {
        System_printf("Error: Read from the UART-1 was successful [Status = %d]\n", status);
        return -1;
    }
    status = UART_readPolling (handle, &rxData, 1);
    if (status != UART_EINVAL)
    {
        System_printf("Error: Polling Read from the UART-1 was successful [Status = %d]\n", status);
        return -1;
    }
    System_printf("Debug: UART-1 Read capability test passed\n");

    /****************************************************************************
     * Sanity Check: UART-1 is capable of only transmitting data
     ****************************************************************************/
    UART_writePolling(handle, ptrTestBanner, strlen((const char*)ptrTestBanner));
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
    System_printf ("Debug: Write to the UART-1 was successful. Please verify the Console application.\n");
    UART_close (handle);

    /**************************************************************************
     * Test: Profile DMA vs. Non-DMA UART Profiling for UART-1
     **************************************************************************/
    if (Test_profileTransmit (0U) < 0)
    {
        MCPI_setFeatureTestResult ("UART1 DMA/Normal Profile", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("UART1 DMA/Normal Profile", MCPI_TestResult_PASS);
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
    EDMA_errorConfig_t  errorConfig;
    EDMA_instanceInfo_t instanceInfo;
    int32_t             errCode;

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize & setup the Event combiner: */
    Test_setupEventCombiner();

    /* Initialize the UART */
    UART_init();

    /* Debug Message: */
    System_printf ("Debug: UART has been initialized\n");

    /* Initialize the EDMA: */
    errCode = EDMA_init(0);
    if (errCode != EDMA_NO_ERROR)
    {
        System_printf("Error: Unable to initialize EDMA, errorCode = %d\n", errCode);
        return;
    }
    System_printf ("Debug: EDMA instance 0 has been initialized\n");

    /* Open the first edma Instance */
    gEDMAHandle = EDMA_open(0, &errCode, &instanceInfo);
    if (gEDMAHandle == NULL)
    {
        System_printf("Error: Unable to open the edma Instance, erorCode = %d\n", errCode);
        return;
    }

    /* Setup the EDMA Error Monitoring: */
    errorConfig.isConfigAllEventQueues              = true;
    errorConfig.isConfigAllTransferControllers      = true;
    errorConfig.isEventQueueThresholdingEnabled     = true;
    errorConfig.eventQueueThreshold                 = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    errorConfig.callbackFxn                         = Test_edmaErrorCallbackFxn;
    errorConfig.transferControllerCallbackFxn       = Test_edmaTransferControllerErrorCallbackFxn;
    errCode = EDMA_configErrorMonitoring(gEDMAHandle, &errorConfig);
    if (errCode != EDMA_NO_ERROR)
    {
        System_printf("Debug: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errCode);
        return;
    }

    /**************************************************************************
     * Test: SOC Specific Hardware Capabilities
     **************************************************************************/
    if (Test_HwCapabilities() != 0)
    {
        MCPI_setFeatureTestResult ("UART SOC Hw Capabilties", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("UART SOC Hw Capabilties", MCPI_TestResult_PASS);

    /* There are no additional tests which we want to execute. */
    MCPI_setTestResult ();
    BIOS_exit(0);
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
    SOC_Cfg         socCfg;
    int32_t         errCode;
    SOC_Handle      socHandle;

    /* Start the Timestamp counter */
    TSCL = 0;

    /* Enable the caches. */
    Cache_enable(Cache_Type_L1P);
    Cache_enable(Cache_Type_L1D);

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: We are bypassing the clock initialization
     * in the unit test here. */
    socCfg.clockCfg = SOC_SysClock_BYPASS_INIT;

    /* Initialize the SOC Module: */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Debug Message: */
    System_printf ("******************************************\n");
    System_printf ("Debug: Launching the UART Test Application\n");
    System_printf ("UART Clock Frequency: %d\n", gCPUClockFrequency);
    System_printf ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Debug Message: */
    System_printf ("Debug: Launching BIOS\n");

    /* Start BIOS */
	BIOS_start();
    return 0;
}

