/*
 *   @file  test_common.c
 *
 *   @brief
 *      Unit Test common code for the MibSpi Driver
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

/* MCPI logger include files */
#include <ti/utils/testlogger/logger.h>

/* SPI test include files */
#include <ti/drivers/spi/test/common/test_common.h>

/**************************************************************************
 *************************** Local Definitions *********************************
 **************************************************************************/

/**************************************************************************
 *************************** Global Definitions ********************************
 **************************************************************************/
/* System DMA handle, created in init Task */
DMA_Handle          gDmaHandle = NULL;

/* Test case global variables */
bool gXWR1xxxLoopbackTest         = true;
bool gXWR1xxxSlaveReady = false;

/* Test with MSP43x */
bool gXWR1xxxMasterWithMSPTest    = false;
bool gXWR1xxxSlaveWithMSPTest     = false;

/* Test with PC through FTDI */
bool gXWR1xxxSlaveWithFTDITest    = true;

/* Test with XWR1xxx  */
bool gXWR1xxxMasterWithXWR1xxx = false;
bool gXWR1xxxSlaveWithXWR1xxx  = false;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *   @param[in] arg0               Task Arg0
 *   @param[in] arg1               Task Arg1
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    bool           masterMode = true;
    DMA_Params      dmaParams;
    int32_t         retVal = 0;
    SOC_Handle      socHandle;
    int32_t         errCode;

    /* Get SOC driver Handle */
    socHandle = (SOC_Handle) arg0;

    /**************************************************************************
     * Test: MCPI_Initialize
     **************************************************************************/
    /* Initialize MCPI logger framework */
    MCPI_Initialize ();

    /* Init SYSDMA params */
    DMA_Params_init(&dmaParams);

    /* Open DMA driver instance 0 for SPI test */
    gDmaHandle = DMA_open(0, &dmaParams, &retVal);

    if(gDmaHandle == NULL)
    {
        printf("Open DMA driver failed with error=%d\n", retVal);
        return;
    }

    /* Initialize the SPI */
    SPI_init();

    /**************************************************************************
     * Test: One instace API test - SPIA
     **************************************************************************/
    Test_spiAPI_oneInstance(0);
    System_printf("Debug: Finished API Test for SPIA!\n");

    /**************************************************************************
     * Test: LoopBack Test
     **************************************************************************/
    if(gXWR1xxxLoopbackTest)
    {
        System_printf("Debug: SPI loopback Test for SPIA!\n");

        /* MibSPIA loopback test , MibSPIA only supports one slave */
        Test_loopback_oneInstance(0U, 0U);

        System_printf("Debug: Loopback test finished!\n");
        }

    if (gXWR1xxxMasterWithMSPTest)
        {
        /* Enable output control for SPIA 3-pin Mode */
        if(SOC_SPIOutputCtrl(socHandle, 0U, 1U, &errCode) < 0)
        {
            /* Debug Message: */
            System_printf ("Debug: SOC_SPIOutputCtrl failed with Error [%d]\n", errCode);
            return;
        }

        masterMode = true;

        /**************************************************************************
         * Test: XWR14 Master mode test with MSP43x
         **************************************************************************/
        Test_spiWithMSP43x(masterMode, 1000000U);

        /**************************************************************************
         * Test: XWR14 Master mode for throughput test with MSP43x
         **************************************************************************/
        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 1000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 2000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 6000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 8000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 10000000U);
    }
    else if (gXWR1xxxSlaveWithMSPTest)
    {
        /* Enable output control for SPIA 3-pin Mode */
        if(SOC_SPIOutputCtrl(socHandle, 0U, 1U, &errCode) < 0)
        {
            /* Debug Message: */
            System_printf ("Debug: SOC_SPIOutputCtrl failed with Error [%d]\n", errCode);
            return;
        }

        masterMode = false;
        /**************************************************************************
         * Test: XWR14 Slave mode test with MSP43x
         **************************************************************************/
        Test_spiWithMSP43x(masterMode, 0);

        /**************************************************************************
         * Test: XWR14 Master mode for throughput test with MSP43x
         **************************************************************************/
        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 1000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 2000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 6000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 8000000U);

        Task_sleep(10);
        Test_spiMSP432xThroughput(masterMode, 10000000U);
    }

    if(gXWR1xxxSlaveWithFTDITest)
    {
        /* Enable output control for SPIA in 4Pin mode */
        if(SOC_SPIOutputCtrl(socHandle, 0U, 0U, &errCode) < 0)
        {
            /* Debug Message: */
            System_printf ("Debug: SOC_SPIOutputCtrl failed with Error [%d]\n", errCode);
            return;
        }

        Test_spiSlaveWithPC();
    }

    /* AR back to back test */
    if(gXWR1xxxMasterWithXWR1xxx || gXWR1xxxSlaveWithXWR1xxx)
    {
        Test_XWR1xxxWithXWR1xxx(socHandle);
    }
    System_printf("Debug: SPI Test is done!\n");

    MCPI_setTestResult ();

    /* After test all done, terminate DSP by calling BIOS_exit().
        This is required by MCPI test framework script.
        MCPI test framework script waits for DSP halt,
        if DSP doesn't halt, script will wait until timeout then claim test fail.
     */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params     taskParams;
    SOC_Handle      socHandle;
    int32_t         errCode;
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
    if(socHandle == NULL)
    {
        /* Debug Message: */
        System_printf ("Debug: Soc_init failed with Error [%d]\n", errCode);
        goto Exit;
    }

    /*=======================================
     * Setup the PINMUX to bring out the MibSpiA
     *=======================================*/
    /* NOTE: Please change the following pin configuration according
            to EVM used for the test */

    /* SPIA_MOSI */
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINR8_PADAD, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINR8_PADAD, SOC_XWR14XX_PINR8_PADAD_SPIA_MOSI);

    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINP5_PADAE, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINP5_PADAE, SOC_XWR14XX_PINP5_PADAE_SPIA_MISO);

    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINR9_PADAF, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINR9_PADAF, SOC_XWR14XX_PINR9_PADAF_SPIA_CLK);

    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINR7_PADAG, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINR7_PADAG, SOC_XWR14XX_PINR7_PADAG_SPIA_CS);

    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINP6_PADAA, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINP6_PADAA, SOC_XWR14XX_PINP6_PADAA_SPI_HOST1_INTR);

    /* Debug Message: */
    System_printf ("******************************************\n");
    System_printf ("Debug: MibSPI Driver Test Application Start \n");
    System_printf ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    taskParams.arg0 = (UArg)socHandle;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();

Exit:
    return 0;
}

