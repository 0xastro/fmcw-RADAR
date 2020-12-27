/*
 *   @file  main_mss.c
 *
 *   @brief
 *      Unit Test code for MSS Watchdog driver
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
#include <math.h>

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
#include <ti/drivers/watchdog/Watchdog.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/esm/esm.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/drivers/osal/HwiP.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
/** \brief Number of messages sent */
#define WATCHDOG_APP_TEST_RESET          1
#define WATCHDOG_APP_TEST_INTERRUPT      2
#define WATCHDOG_APP_TEST_DSP_INTERRUPT  3

/* Global Variables */
volatile uint32_t       testSelection = 0;
volatile uint32_t       gWatchdogInt = 0;
volatile uint32_t       gWatchdogDspInt = 0;
Watchdog_Handle         watchdogHandle;
SOC_Handle              socHandle;
ESM_Handle              esmHandle;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle watchdog interrupt.
 *
 *   @param[in] handle
 *      Handle to the Watchdog driver
 *  @retval
 *      Not Applicable.
 */
static void watchdogCallback(Watchdog_Handle handle)
{
    gWatchdogInt++;
    if (gWatchdogInt < 10)
    {
        System_printf ("Debug: Watchdog Driver NMI received\n");
        Watchdog_clear(watchdogHandle);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle DSS ESM group errors. When the watchdog timeout causes
 *      a NMI to occur on DSS, that event inturn triggers a group1 ESM error on the MSS. The application can register
 *      a callback using the ESM notifier to handle these errors.
 *
 *   @param[in] arg
 *      Argument which is registered with the ESM module while registering the notify function.
 *  @retval
 *      Not Applicable.
 */
static void DSPNotifyMSSCallback(void* arg)
{
    gWatchdogDspInt++;
    return;
}

static int32_t watchdogNotifyTest()
{
    ESM_NotifyParams        notifyParams;
    int32_t                 errCode = 0;
    int32_t                 retVal = 0;

    notifyParams.groupNumber = 1;
    notifyParams.errorNumber = SOC_XWR18XX_MSS_DSS_GP2_ERR_ESM;
    notifyParams.arg = NULL;
    notifyParams.notify = DSPNotifyMSSCallback;
    retVal = ESM_registerNotifier (esmHandle, &notifyParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: ESM Register Notifier failed\n");
        return -1;
    }
    else
    {
        System_printf ("Debug: ESM Register Notifier registered\n");
    }

    while (gWatchdogDspInt == 0);

    System_printf ("Debug: DSP Watchdog Timeout received......Exiting\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      Configuration and functional watchdog driver test.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t watchdogTest()
{
    Watchdog_Params     watchdogParams;
    int32_t             errCode = 0;
    Watchdog_ResetMode  resetMode;
    Watchdog_WindowSize windowSize;

    /* Initializa the Watchdog driver */
    Watchdog_init();

    /* Initialize the Watchdog driver default parameters */
    Watchdog_Params_init(&watchdogParams);

    if (testSelection == WATCHDOG_APP_TEST_RESET)
    {
        watchdogParams.resetMode = Watchdog_RESET_ON;
    }
    if (testSelection == WATCHDOG_APP_TEST_INTERRUPT)
    {
        watchdogParams.resetMode = Watchdog_RESET_OFF;
        watchdogParams.callbackFxn = watchdogCallback;
    }

    watchdogParams.debugStallMode = Watchdog_DEBUG_STALL_ON;
    watchdogParams.windowSize = Watchdog_WINDOW_100_PERCENT;
    watchdogParams.preloadValue = 20;
    watchdogParams.socHandle = socHandle;
    watchdogParams.esmHandle = esmHandle;


    /* Open the Watchdog driver */
    watchdogHandle = Watchdog_open(0, &watchdogParams);

    if (watchdogHandle == NULL)
    {
        System_printf ("Error: Watchdog Driver Open failed\n");
        return -1;
    }
    if (testSelection == WATCHDOG_APP_TEST_RESET)
    {
        /* Wait for the watchdog to expire and trigger a warm reset */
        while (1);
    }
    else if (testSelection == WATCHDOG_APP_TEST_INTERRUPT)
    {
        while (gWatchdogInt == 0);
    }

    /* Configure the Watchdog reset mode ON */
    resetMode = Watchdog_RESET_ON;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_RESETMODE, (void* )&resetMode);
    if (errCode < 0)
    {
        System_printf ("Error: Watchdog control Set Reset Mode failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog reset mode OFF */
    resetMode = Watchdog_RESET_OFF;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_RESETMODE, (void* )&resetMode);
    if (errCode < 0)
    {
        System_printf ("Error: Watchdog control Set Reset Mode failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 50% */
    windowSize = Watchdog_WINDOW_50_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        System_printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 25% */
    windowSize = Watchdog_WINDOW_25_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        System_printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 12.5% */
    windowSize = Watchdog_WINDOW_12_5_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        System_printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 6.25% */
    windowSize = Watchdog_WINDOW_6_25_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        System_printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configure the Watchdog Window size 3.125% */
    windowSize = Watchdog_WINDOW_3_125_PERCENT;
    errCode = Watchdog_control (watchdogHandle, WATCHDOG_CMD_WINDOWSIZE, (void* )&windowSize);
    if (errCode < 0)
    {
        System_printf ("Error: Watchdog control Set Window size failed [Error code %d]\n", errCode);
        return -1;
    }

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
    int32_t         retVal = 0;

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    while (1)
    {
        System_printf ("********************************************************\n");
        System_printf ("Watchdog Unit Test                                      \n");
        System_printf ("Please select the type of test to execute:  \n");
        System_printf ("1. Watchdog reset test                      \n");
        System_printf ("2. Watchdog interrupt CPU test              \n");
        System_printf ("3. Watchdog DSP interrupt CPU test          \n");
        System_printf ("*******************************************************\n");
        System_printf ("> Enter your selection: ");

        while (testSelection == 0);

        /* Validate the selection: */
        if ((testSelection >= WATCHDOG_APP_TEST_RESET) && (testSelection <= WATCHDOG_APP_TEST_DSP_INTERRUPT))
            break;
    }
    if (testSelection == WATCHDOG_APP_TEST_RESET)
    {
        System_printf("Debug: Testing watchdog reset mode\n");
        retVal = watchdogTest();
    }
    else if (testSelection == WATCHDOG_APP_TEST_INTERRUPT)
    {
        System_printf("Debug: Testing watchdog interrupt mode\n");
        retVal = watchdogTest();
    }
    else if (testSelection == WATCHDOG_APP_TEST_DSP_INTERRUPT)
    {
        System_printf("Debug: Waiting for watchdog interrupt from DSP\n");
        retVal = watchdogNotifyTest();
    }

    if (retVal == -1)
        MCPI_setFeatureTestResult("Watchdog testing", MCPI_TestResult_FAIL);
    else
        MCPI_setFeatureTestResult("Watchdog testing", MCPI_TestResult_PASS);

    if (retVal < 0)
    {
        System_printf("Debug: Watchdog testing failed\n");
    }

    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t main (void)
{
	Task_Params     taskParams;
    SOC_Cfg         socCfg;
    int32_t         errCode;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */

    esmHandle = ESM_init(0U);
    if (esmHandle == NULL)
    {
        System_printf ("Error: ESM Module Initialization failed\n");
        return -1;
    }

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

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
    return 0;
}

