/*
 *   @file  main_dss.c
 *
 *   @brief
 *      Unit Test code for the mmWave on the XWR16xx DSS
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
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/

/* Global Variable: The variable detects if the configuration has been
 * detected or not. */
uint32_t        gConfigDetected = 0U;

/* Global Variable: The variable detects if the start has been
 * detected or not. */
uint32_t        gStartDetected = 0U;

/* Global Variable: The variable detects if the start has been
 * detected or not. */
uint32_t        gStopDetected = 0U;

/* Global Variable: The variable detects if the open has been
 * detected or not. */
uint32_t         gOpenDetected = 0U;

/* Global Variable: The variable detects if the close has been
 * detected or not. */
uint32_t         gCloseDetected = 0U;

/* Global Variable: This is the handle to the mmWave module */
MMWave_Handle   gMMWaveHandle;

/* Global Variable: The variable detects the test selection as entered on the MSS */
int32_t         gMCPIEnable = 0U;
int32_t         gTestSelection;
char            gSaveRestoreChoice;

/* Global Handle which tracks the SOC Handle */
SOC_Handle      socHandle;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**************************************************************************
 ************************** Extern Definitions ****************************
 **************************************************************************/
extern void Mmwave_populateDefaultOpenCfg (MMWave_OpenCfg* ptrOpenCfg);
extern void Mmwave_populateDefaultChirpControlCfg (MMWave_CtrlCfg* ptrCtrlCfg);
extern void Mmwave_populateDefaultAdvancedControlCfg (MMWave_CtrlCfg* ptrCtrlCfg);
extern void Mmwave_populateDefaultCalibrationCfg (MMWave_CalibrationCfg* ptrCalibrationCfg, MMWave_DFEDataOutputMode dfeOutputMode);
extern int32_t Mmwave_eventFxn (uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload);
extern void Mmwave_cfgFxn (MMWave_CtrlCfg* ptrCtrlCfg);
extern void Mmwave_advancedFrameCfgFxn (MMWave_CtrlCfg* ptrCtrlCfg);
extern void Mmwave_startFxn (MMWave_CalibrationCfg* ptrCalibrationCfg);
extern void Mmwave_stopFxn (void);
extern void Mmwave_openFxn (MMWave_OpenCfg* ptrOpenCfg);
extern void Mmwave_closeFxn (void);
extern void Mmwave_stopFxn (void);
extern void Mmwave_ctrlTask(UArg arg0, UArg arg1);
extern int32_t Test_mmWaveFullAPI (void);

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/
static void Test1 (void);
static void Test2 (void);
static void Test3 (void);
static void Test4 (void);
static void Test5 (void);
static void Test6 (void);
static void Test7 (void);
static void Test8 (void);
static void Test9 (void);

/**************************************************************************
 *********************** mmWave Unit Test Functions ***********************
 **************************************************************************/

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
 *      The function implements the following Test
 *
 *      [Isolation Mode]   MSS executes the mmWave link: Chirp Configuration Mode
 *
 *  @retval
 *      Not Applicable.
 */
static void Test1 (void)
{
    /***********************************************************************
     * [Isolation Mode]: MSS executes the mmWave link
     * - There is nothing to be done here since we are operating in
     *   isolation mode. In theory the DSS should not be even loaded and
     *   executed but if someone has done the hard work; we can just execute
     *   the DSS in Idle mode
     ***********************************************************************/
    System_printf ("Debug: Placing the DSS in Idle Mode!\n");
    while (1)
        Task_sleep(1);

}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      [Isolation Mode]   DSS executes the mmWave link: Chirp Configuration Mode
 *
 *  @retval
 *      Not Applicable.
 */
static void Test2 (void)
{
    MMWave_InitCfg          initCfg;
    MMWave_CtrlCfg          ctrlCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_CalibrationCfg   calibrationCfg;
    MMWave_OpenCfg          openCfg;
    int32_t                 retVal;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

    /* Initialize the configuration: */
    memset ((void *)&initCfg, 0, sizeof(MMWave_InitCfg));

    /***********************************************************************
     * [Isolation Mode]: DSS executes the mmWave link
     ***********************************************************************/
    initCfg.domain                  = MMWave_Domain_DSS;
    initCfg.socHandle               = socHandle;
    initCfg.eventFxn                = Mmwave_eventFxn;
    initCfg.executionMode           = MMWave_ExecutionMode_ISOLATION;
    initCfg.linkCRCCfg.useCRCDriver = 1U;
    initCfg.linkCRCCfg.crcChannel   = CRC_Channel_CH1;
    initCfg.cfgMode                 = MMWave_ConfigurationMode_FULL;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                       (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                       mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Isolation Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Isolation Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /***********************************************************************
     * [Isolation Mode] DSS executes the mmWave
     ************************************************************************/
    Mmwave_populateDefaultOpenCfg (&openCfg);
    Mmwave_populateDefaultChirpControlCfg (&ctrlCfg);

    /* Override the default configuration and ensure that the DSS is the default
     * asynchronous event handler */
    openCfg.defaultAsyncEventHandler = MMWave_DefaultAsyncEventHandler_DSS;

    /************************************************************************
     * Open the mmWave:
     ************************************************************************/
    if (MMWave_open (gMMWaveHandle, &openCfg, NULL, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave open failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_PASS);

    /************************************************************************
     * Configure the mmWave:
     ************************************************************************/
    if (MMWave_config (gMMWaveHandle, &ctrlCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave configuration failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Configuration", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Configuration", MCPI_TestResult_PASS);

    /* Populate the calibration configuration: */
    memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));
    Mmwave_populateDefaultCalibrationCfg (&calibrationCfg, MMWave_DFEDataOutputMode_FRAME);

    /************************************************************************
     * Start the mmWave:
     ************************************************************************/
    if (MMWave_start (gMMWaveHandle, &calibrationCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave start failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Start", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Start", MCPI_TestResult_PASS);

    /************************************************************************
     * Stop the mmWave:
     ************************************************************************/
    retVal = MMWave_stop (gMMWaveHandle, &errCode);
    if (retVal < 0)
    {
        /* Error: Stopping the sensor failed. Decode the error code. */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                       (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                       mmWaveErrorCode, subsysErrorCode);

        /* Did we fail because of an error? */
        if (errorLevel == MMWave_ErrorLevel_ERROR)
        {
            /* Error Level: The test has failed. */
            MCPI_setFeatureTestResult ("MMWave DSS Stop", MCPI_TestResult_FAIL);
            return;
        }
        else
        {
            /* Informational Level: The test has passed. Fall through...*/
        }
    }
    MCPI_setFeatureTestResult ("MMWave DSS Stop", MCPI_TestResult_PASS);

    /************************************************************************
     * Close the mmWave:
     ************************************************************************/
    if (MMWave_close (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to close the mmWave control module */
        System_printf ("Error: mmWave close failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_PASS);

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      [Cooperative Mode] MSS executes the mmWave link: Chirp Configuration Mode
 *
 *  @retval
 *      Not Applicable.
 */
static void Test3 (void)
{
    MMWave_InitCfg          initCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

    /***********************************************************************
     * [Cooperative Mode] MSS executes the mmWave link
     ************************************************************************/
     initCfg.domain                      = MMWave_Domain_DSS;
     initCfg.socHandle                   = socHandle;
     initCfg.eventFxn                    = Mmwave_eventFxn;
     initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
     initCfg.linkCRCCfg.useCRCDriver     = 1U;
     initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
     initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;
     initCfg.cooperativeModeCfg.cfgFxn   = Mmwave_cfgFxn;
     initCfg.cooperativeModeCfg.openFxn  = Mmwave_openFxn;
     initCfg.cooperativeModeCfg.closeFxn = Mmwave_closeFxn;
     initCfg.cooperativeModeCfg.startFxn = Mmwave_startFxn;
     initCfg.cooperativeModeCfg.stopFxn  = Mmwave_stopFxn;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                        (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                        mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /***********************************************************************
     * [Cooperative Mode] MSS executes the mmWave link
     * - Open Callback Function
     ************************************************************************/
    while (1)
    {
        if (gOpenDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Configuration Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gConfigDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Start Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gStartDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Start Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Stop Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gStopDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Stop Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Close Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gCloseDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close Callback", MCPI_TestResult_PASS);

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      [Cooperative Mode] DSS executes the mmWave link: Chirp Configuration Mode
 *
 *  @retval
 *      Not Applicable.
 */
static void Test4 (void)
{
    MMWave_InitCfg          initCfg;
    MMWave_CtrlCfg          ctrlCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_CalibrationCfg   calibrationCfg;
    MMWave_OpenCfg          openCfg;
    int32_t                 retVal;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

    /***********************************************************************
     * [Cooperative Mode] DSS executes the mmWave link
     ************************************************************************/
    initCfg.domain                      = MMWave_Domain_DSS;
    initCfg.socHandle                   = socHandle;
    initCfg.eventFxn                    = Mmwave_eventFxn;
    initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
    initCfg.linkCRCCfg.useCRCDriver     = 1U;
    initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
    initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;
    initCfg.cooperativeModeCfg.cfgFxn   = Mmwave_cfgFxn;
    initCfg.cooperativeModeCfg.openFxn  = Mmwave_openFxn;
    initCfg.cooperativeModeCfg.closeFxn = Mmwave_closeFxn;
    initCfg.cooperativeModeCfg.startFxn = Mmwave_startFxn;
    initCfg.cooperativeModeCfg.stopFxn  = Mmwave_stopFxn;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                        (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                        mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /***********************************************************************
     * [Cooperative Mode] DSS executes the mmWave link
     ************************************************************************/
    Mmwave_populateDefaultOpenCfg (&openCfg);
    Mmwave_populateDefaultChirpControlCfg (&ctrlCfg);

    /************************************************************************
     * Open the mmWave:
     ************************************************************************/
    if (MMWave_open (gMMWaveHandle, &openCfg, NULL, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave open failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we initiated the open
     * the open callback on the remote execution domain should be invoked. */
    if (gOpenDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_PASS);
    }

    /************************************************************************
     * Configure the mmWave:
     ************************************************************************/
    if (MMWave_config (gMMWaveHandle, &ctrlCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave configuration failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Configuration", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Configuration", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we initiated the configuration
     * the configuration callback on the remote execution domain should be invoked. */
    if (gConfigDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_PASS);
    }

    /* Populate the calibration configuration: */
    Mmwave_populateDefaultCalibrationCfg (&calibrationCfg, MMWave_DFEDataOutputMode_FRAME);

    /************************************************************************
     * Start the mmWave:
     ************************************************************************/
    if (MMWave_start (gMMWaveHandle, &calibrationCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave start failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Start", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Start", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we started the configuration
     * the configuration callback on the remote execution domain should be invoked. */
    if (gStartDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Start Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Start Callback", MCPI_TestResult_PASS);
    }

    /************************************************************************
     * Stop the mmWave:
     ************************************************************************/
    retVal = MMWave_stop (gMMWaveHandle, &errCode);
    if (retVal < 0)
    {
        /* Error: Stopping the sensor failed. */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                       (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                       mmWaveErrorCode, subsysErrorCode);

        /* Did we fail because of an error? */
        if (errorLevel == MMWave_ErrorLevel_ERROR)
        {
            /* Error Level: The test has failed. */
            MCPI_setFeatureTestResult ("MMWave DSS Stop", MCPI_TestResult_FAIL);
            return;
        }
        else
        {
            /* Informational Level: The test has passed. Fall through...*/
        }
    }
    MCPI_setFeatureTestResult ("MMWave DSS Stop", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we stopped the configuration
     * the configuration callback on the remote execution domain should be invoked. */
    if (gStopDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Stop Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Stop Callback", MCPI_TestResult_PASS);
    }

    /************************************************************************
     * Close the mmWave:
     ************************************************************************/
    if (MMWave_close (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave close failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_PASS);

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      Full Configuration API Testing only
 *
 *  @retval
 *      Not Applicable.
 */
static void Test5 (void)
{
    MMWave_InitCfg          initCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

    /***********************************************************************
     * [Cooperative Mode] DSS executes the mmWave link
     ************************************************************************/
    initCfg.domain                      = MMWave_Domain_DSS;
    initCfg.socHandle                   = socHandle;
    initCfg.eventFxn                    = Mmwave_eventFxn;
    initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
    initCfg.linkCRCCfg.useCRCDriver     = 1U;
    initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
    initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;
    initCfg.cooperativeModeCfg.cfgFxn   = Mmwave_cfgFxn;
    initCfg.cooperativeModeCfg.openFxn  = Mmwave_openFxn;
    initCfg.cooperativeModeCfg.closeFxn = Mmwave_closeFxn;
    initCfg.cooperativeModeCfg.startFxn = Mmwave_startFxn;
    initCfg.cooperativeModeCfg.stopFxn  = Mmwave_stopFxn;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                        (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                        mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /* Verify the Full configuration API functionality */
    Test_mmWaveFullAPI ();

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      [Cooperative Mode] MSS executes mmWave link: Advanced Frame Configuration Mode
 *
 *  @retval
 *      Not Applicable.
 */
static void Test6 (void)
{
    MMWave_InitCfg          initCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

    /***********************************************************************
     * [Cooperative Mode] MSS executes the mmWave link
     * - Advanced Frame Configuration
     ************************************************************************/
    initCfg.domain                      = MMWave_Domain_DSS;
    initCfg.socHandle                   = socHandle;
    initCfg.eventFxn                    = Mmwave_eventFxn;
    initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
    initCfg.linkCRCCfg.useCRCDriver     = 1U;
    initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
    initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;
    initCfg.cooperativeModeCfg.cfgFxn   = Mmwave_advancedFrameCfgFxn;
    initCfg.cooperativeModeCfg.openFxn  = Mmwave_openFxn;
    initCfg.cooperativeModeCfg.closeFxn = Mmwave_closeFxn;
    initCfg.cooperativeModeCfg.startFxn = Mmwave_startFxn;
    initCfg.cooperativeModeCfg.stopFxn  = Mmwave_stopFxn;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                        (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                        mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /***********************************************************************
     * [Cooperative Mode] MSS executes the mmWave link
     * - Open Callback Function
     ************************************************************************/
    while (1)
    {
        if (gOpenDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Configuration Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gConfigDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Start Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gStartDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Start Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Stop Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gStopDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Stop Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Close Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gCloseDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close Callback", MCPI_TestResult_PASS);

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      [Cooperative Mode] DSS executes mmWave link: Advanced Frame Configuration Mode
 *
 *  @retval
 *      Not Applicable.
 */
static void Test7 (void)
{
    MMWave_InitCfg          initCfg;
    MMWave_CtrlCfg          ctrlCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_CalibrationCfg   calibrationCfg;
    MMWave_OpenCfg          openCfg;
    int32_t                 retVal;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

    /***********************************************************************
     * [Cooperative Mode] DSS executes the mmWave link
     * - Advanced Frame Configuration
     ************************************************************************/
     initCfg.domain                      = MMWave_Domain_DSS;
     initCfg.socHandle                   = socHandle;
     initCfg.eventFxn                    = Mmwave_eventFxn;
     initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
     initCfg.linkCRCCfg.useCRCDriver     = 1U;
     initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
     initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;
     initCfg.cooperativeModeCfg.cfgFxn   = Mmwave_advancedFrameCfgFxn;
     initCfg.cooperativeModeCfg.openFxn  = Mmwave_openFxn;
     initCfg.cooperativeModeCfg.closeFxn = Mmwave_closeFxn;
     initCfg.cooperativeModeCfg.startFxn = Mmwave_startFxn;
     initCfg.cooperativeModeCfg.stopFxn  = Mmwave_stopFxn;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                        (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                        mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /***********************************************************************
     * [Cooperative Mode] DSS executes the mmWave link
     * - Advanced Frame configuration
     ************************************************************************/
    Mmwave_populateDefaultOpenCfg (&openCfg);
    Mmwave_populateDefaultAdvancedControlCfg (&ctrlCfg);

    /************************************************************************
     * Open the mmWave:
     ************************************************************************/
    if (MMWave_open (gMMWaveHandle, &openCfg, NULL, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave open failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we initiated the open
     * the open callback on the remote execution domain should be invoked. */
    if (gOpenDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_PASS);
    }

    /************************************************************************
     * Configure the mmWave:
     ************************************************************************/
    if (MMWave_config (gMMWaveHandle, &ctrlCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave configuration failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Configuration", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Configuration", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we initiated the configuration
     * the configuration callback on the remote execution domain should be invoked. */
    if (gConfigDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_PASS);
    }

    /* Populate the calibration configuration: */
    Mmwave_populateDefaultCalibrationCfg (&calibrationCfg, MMWave_DFEDataOutputMode_ADVANCED_FRAME);

    /************************************************************************
     * Start the mmWave:
     ************************************************************************/
    if (MMWave_start (gMMWaveHandle, &calibrationCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave start failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Start", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Start", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we started the configuration
     * the configuration callback on the remote execution domain should be invoked. */
    if (gStartDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Start Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Start Callback", MCPI_TestResult_PASS);
    }

    /************************************************************************
     * Stop the mmWave:
     ************************************************************************/
    retVal = MMWave_stop (gMMWaveHandle, &errCode);
    if (retVal < 0)
    {
        /* Error: Stopping the sensor failed. Decode the error code. */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                       (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                       mmWaveErrorCode, subsysErrorCode);

        /* Did we fail because of an error? */
        if (errorLevel == MMWave_ErrorLevel_ERROR)
        {
            /* Error Level: The test has failed. */
            MCPI_setFeatureTestResult ("MMWave DSS Stop", MCPI_TestResult_FAIL);
            return;
        }
        else
        {
            /* Informational Level: The test has passed. Fall through...*/
        }
    }
    MCPI_setFeatureTestResult ("MMWave DSS Stop", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we stopped the configuration
     * the configuration callback on the remote execution domain should be invoked. */
    if (gStopDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Stop Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Stop Callback", MCPI_TestResult_PASS);
    }

    /************************************************************************
     * Close the mmWave:
     ************************************************************************/
    if (MMWave_close (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave close failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_PASS);

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we stopped the configuration
     * the configuration callback on the remote execution domain should be invoked. */
    if (gCloseDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Close Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Close Callback", MCPI_TestResult_PASS);
    }

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      [Cooperative Mode] MSS Calibration Data Save/Restore
 *
 *  @retval
 *      Not Applicable.
 */
static void Test8 (void)
{
    MMWave_InitCfg          initCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

    /***********************************************************************
     * [Cooperative Mode] MSS executes the mmWave link
     * - Calibration Data Save & Restore
     ************************************************************************/
    initCfg.domain                      = MMWave_Domain_DSS;
    initCfg.socHandle                   = socHandle;
    initCfg.eventFxn                    = Mmwave_eventFxn;
    initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
    initCfg.linkCRCCfg.useCRCDriver     = 1U;
    initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
    initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;
    initCfg.cooperativeModeCfg.cfgFxn   = Mmwave_cfgFxn;
    initCfg.cooperativeModeCfg.openFxn  = Mmwave_openFxn;
    initCfg.cooperativeModeCfg.closeFxn = Mmwave_closeFxn;
    initCfg.cooperativeModeCfg.startFxn = Mmwave_startFxn;
    initCfg.cooperativeModeCfg.stopFxn  = Mmwave_stopFxn;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                        (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                        mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /***********************************************************************
     * [Cooperative Mode] MSS executes the mmWave link
     * - Calibration Data Save/Restore
     ************************************************************************/
    while (1)
    {
        if (gOpenDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_PASS);

    /************************************************************************
     * Close Callback Function:
     ************************************************************************/
    while (1)
    {
        if (gCloseDetected == 1U)
            break;
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close Callback", MCPI_TestResult_PASS);

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function implements the following Test
 *
 *      [Cooperative Mode] DSS Calibration Data Save/Restore
 *
 *  @retval
 *      Not Applicable.
 */
static void Test9 (void)
{
    MMWave_InitCfg          initCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_OpenCfg          openCfg;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;
    FILE*                   fCalibDataFile;
    uint8_t*                ptrCalibData;
    rlReturnVal_t           linkRetVal;
    uint32_t                sizeCalibData;
    char                    inputString[3U];
    rlCalibrationData_t     calibrationData;

    /***********************************************************************
     * [Cooperative Mode] DSS executes the mmWave link
     * - Calibration Data Save & Restore
     ************************************************************************/
    initCfg.domain                      = MMWave_Domain_DSS;
    initCfg.socHandle                   = socHandle;
    initCfg.eventFxn                    = Mmwave_eventFxn;
    initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
    initCfg.linkCRCCfg.useCRCDriver     = 1U;
    initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
    initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;
    initCfg.cooperativeModeCfg.cfgFxn   = Mmwave_cfgFxn;
    initCfg.cooperativeModeCfg.openFxn  = Mmwave_openFxn;
    initCfg.cooperativeModeCfg.closeFxn = Mmwave_closeFxn;
    initCfg.cooperativeModeCfg.startFxn = Mmwave_startFxn;
    initCfg.cooperativeModeCfg.stopFxn  = Mmwave_stopFxn;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        System_printf ("Error Level: %s mmWave: %d Subsys: %d\n",
                        (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                        mmWaveErrorCode, subsysErrorCode);
        MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);

    /******************************************************************************
     * TEST: Synchronization
     * - The synchronization API always needs to be invoked [Irrespective of the
     *   mode]
     ******************************************************************************/
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gMMWaveHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);

            /* Log into the MCPI Test Logger: */
            MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        Task_sleep(1);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = 6;
    taskParams.stackSize = 6*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /***********************************************************************
     * [Cooperative Mode] DSS executes the mmWave link
     * - Calibration Data Save/Restore
     ************************************************************************/
    Mmwave_populateDefaultOpenCfg (&openCfg);
    
    /* User Input: */
    System_printf ("*****************************************************\n");
    System_printf ("Are we Saving(S) or Restoring(R) the Calibration Data\n");
    System_printf ("*****************************************************\n");
    System_printf ("Enter your choice: ");
    if (gMCPIEnable == 0) /* Run test from CCS */
    {
        fgets (inputString, 3U, stdin);
        gSaveRestoreChoice = inputString[0];
    }

    /* Processing is done on the user choice: */
    if (gSaveRestoreChoice == 'R')
    {
        /********************************************************************
         * Restoring the Calibration Data:
         * - Read the calibration data from the file
         ********************************************************************/
        fCalibDataFile = fopen ("XWR16xxDSSCalibrationData.bin", "rb");
        if (fCalibDataFile == NULL)
        {
            System_printf ("Error: Unable to open the file to restore the calibration data\n");
            MCPI_setFeatureTestResult ("MMWave Restore Calibration Data", MCPI_TestResult_FAIL);
            return;
        }

        /* Cycle through and write all the data into the file: */
        sizeCalibData = sizeof(calibrationData);
        ptrCalibData  = (uint8_t*)&calibrationData;
        while (sizeCalibData > 0U)
        {
            /* Read each byte at a time: */
            if (fread (ptrCalibData, 1U, sizeof(uint8_t), fCalibDataFile) != 1U)
            {
                System_printf ("Error: Unable to write to the file to save the calibration data\n");
                MCPI_setFeatureTestResult ("MMWave Restore Calibration Data", MCPI_TestResult_FAIL);
                fclose (fCalibDataFile);
                return;
            }

            /* Skip to the next set of data: */
            ptrCalibData++;
            sizeCalibData--;
        }
        MCPI_setFeatureTestResult ("MMWave Restore Calibration Data", MCPI_TestResult_PASS);

        /* Update the "default" open configuration:
         * - We are using custom calibration with a custom enable mask
         * - Pass the pointer to the restore calibration data */
        openCfg.useCustomCalibration            = true;
        openCfg.customCalibrationEnableMask     = 0x0010;

        /* Close the file: */
        fclose (fCalibDataFile);

        /* Open the mmWave: Use the calibration data which has been read from the file */
        if (MMWave_open (gMMWaveHandle, &openCfg, &calibrationData, &errCode) < 0)
        {
            /* Error: Unable to configure the mmWave control module */
            System_printf ("Error: mmWave open failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_PASS);
    }
    else
    {
        /********************************************************************
         * Saving the Calibration data:
         * - Use the default configuration and let the mmWave module perform
         *   all the calibrations.
         ********************************************************************/
        openCfg.useCustomCalibration            = false;
        openCfg.customCalibrationEnableMask     = 0U;

        /* Open the mmWave Module: */
        if (MMWave_open (gMMWaveHandle, &openCfg, NULL, &errCode) < 0)
        {
            /* Error: Unable to configure the mmWave control module */
            System_printf ("Error: mmWave open failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_PASS);
    }

    /* Sleep for some time: */
    Task_sleep(5);

    /* The callback function should NOT have been invoked: Since we initiated the open
     * the open callback on the remote execution domain should be invoked. */
    if (gOpenDetected == 1)
    {
        /* Error: Our callback function was invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_FAIL);
    }
    else
    {
        /* Error: Our callback function was NOT invoked */
        MCPI_setFeatureTestResult ("MMWave DSS Open Callback", MCPI_TestResult_PASS);
    }

    /* Processing is done on the user choice: */
    if (gSaveRestoreChoice == 'R')
    {
        /********************************************************************
         * Restore the Calibration data:
         * - There is nothing to be done here
         * - The calibration data has already been passed to the MMWave
         *   module.
         ********************************************************************/
    }
    else
    {
        /********************************************************************
         * Saving the Calibration data:
         * - The MMWave module has been opened succesfully.
         * - Use the Link API to get the calibration data
         * - Save the calibration data into a file.
         ********************************************************************/
        linkRetVal = rlRfCalibDataStore(RL_DEVICE_MAP_INTERNAL_BSS, &calibrationData);
        if (linkRetVal != RL_RET_CODE_OK)
        {
            System_printf ("Error: Unable to save the calibration data [Error: %d]\n", linkRetVal);
            MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_FAIL);
            return;
        }

        /* Open the file in which the calibration data is to be written: */
        fCalibDataFile = fopen ("XWR16xxDSSCalibrationData.bin", "wb");
        if (fCalibDataFile == NULL)
        {
            System_printf ("Error: Unable to open the file to save the calibration data\n");
            MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_FAIL);
            return;
        }

        /* Cycle through and write all the data into the file: */
        sizeCalibData = sizeof(calibrationData);
        ptrCalibData  = (uint8_t*)&calibrationData;
        while (sizeCalibData > 0U)
        {
            /* Write each byte at a time: */
            if (fwrite (ptrCalibData, 1U, sizeof(uint8_t), fCalibDataFile) != 1U)
            {
                System_printf ("Error: Unable to write to the file to save the calibration data\n");
                MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_FAIL);
                fclose (fCalibDataFile);
                break;
            }

            /* Skip to the next set of data: */
            ptrCalibData++;
            sizeCalibData--;
        }
        MCPI_setFeatureTestResult ("MMWave Save Calibration Data", MCPI_TestResult_PASS);
        fclose (fCalibDataFile);
    }

    /************************************************************************
     * Close the mmWave:
     ************************************************************************/
    if (MMWave_close (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        System_printf ("Error: mmWave close failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_PASS);

    /* Deinitialize the mmWave module: */
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        System_printf ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    return;
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
static void MmwaveLink_initTask(UArg arg0, UArg arg1)
{
    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize & setup the Event combiner: */
    Test_setupEventCombiner();

    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_DSS);

    /* We are waiting for the test selection to be entered by the MSS. The DSS and MSS need
     * to work in cojunction with each other. */
    System_printf ("***************************************************\n");
    System_printf ("Waiting for the test selection to be entered by MSS\n");
    System_printf ("In CCS: Set the Global Variable 'gTestSelection'\n");
    System_printf ("***************************************************\n");
    while (1)
    {
        if ((gTestSelection >= 1) && (gTestSelection <= 9))
            break;
        Task_sleep(1);
    }

    /*********************************************************************
     * Execute the Test on the basis of the test selection:
     *********************************************************************/
    switch (gTestSelection)
    {
        case 1:
        {
            Test1();
            break;
        }
        case 2:
        {
            Test2();
            break;
        }
        case 3:
        {
            Test3();
            break;
        }
        case 4:
        {
            Test4();
            break;
        }
        case 5:
        {
            Test5();
            break;
        }
        case 6:
        {
            Test6();
            break;
        }
        case 7:
        {
            Test7();
            break;
        }
        case 8:
        {
            Test8();
            break;
        }
        case 9:
        {
            Test9();
            break;
        }
        default:
        {
            System_printf ("Error: Invalid MENU selection; try again\n");
            return;
        }
    }

    /* MCPI Logger Test Completion */
    MCPI_setTestResult ();
    BIOS_exit (0);
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

    /* Start the Timestamp counter */
    TSCL = 0;

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
    System_printf ("******************************************\n");
    System_printf ("Debug: Launching the mmwaveLink\n");
    System_printf ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 8*1024;
    Task_create(MmwaveLink_initTask, &taskParams, NULL);

    /* Debug Message: */
    System_printf ("Debug: Launching BIOS\n");

    /* Start BIOS */
    BIOS_start();
    return 0;
}


