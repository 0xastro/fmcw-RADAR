/*
 *   @file  main_dss.c
 *
 *   @brief
 *      Unit Test code for the mmWave on the XWR68xx DSS
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
int32_t         gTestSelection;

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
extern int32_t Mmwave_testLinkAPI (void);
extern int32_t Mmwave_eventFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload);
extern void Mmwave_cfgFxn(MMWave_CtrlCfg* ptrCtrlCfg);
extern void Mmwave_startFxn(MMWave_CalibrationCfg* ptrCalibrationCfg);
extern void Mmwave_stopFxn(void);
extern void Mmwave_openFxn (MMWave_OpenCfg* ptrOpenCfg);
extern void Mmwave_closeFxn (void);
extern void Mmwave_ctrlTask(UArg arg0, UArg arg1);

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
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwaveLink_initTask(UArg arg0, UArg arg1)
{
    MMWave_InitCfg          initCfg;
    Task_Params             taskParams;
    int32_t                 errCode;
    MMWave_CalibrationCfg   calibrationCfg;
    MMWave_OpenCfg          openCfg;
    int32_t                 retVal;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;

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
        if ((gTestSelection >= 1) && (gTestSelection <= 4))
            break;
        Task_sleep(1);
    }

    /* Initialize the configuration: */
    memset ((void *)&initCfg, 0, sizeof(MMWave_InitCfg));
    memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));

    /******************************************************************************
     * TEST: Initialization of the mmWave module
     * - Populate the configuration on the basis of the test selection:
     ******************************************************************************/
    switch (gTestSelection)
    {
        case 1:
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
        case 2:
        {
            /***********************************************************************
             * [Isolation Mode]: DSS executes the mmWave link
             ***********************************************************************/
            initCfg.domain                  = MMWave_Domain_DSS;
            initCfg.socHandle               = socHandle;
            initCfg.eventFxn                = Mmwave_eventFxn;
            initCfg.cfgMode                 = MMWave_ConfigurationMode_MINIMAL;
            initCfg.executionMode           = MMWave_ExecutionMode_ISOLATION;
            initCfg.linkCRCCfg.useCRCDriver = 1U;
            initCfg.linkCRCCfg.crcChannel   = CRC_Channel_CH1;

            /* Initialize and setup the mmWave Control module */
            gMMWaveHandle = MMWave_init (&initCfg, &errCode);
            if (gMMWaveHandle == NULL)
            {
                /* Error: Unable to initialize the mmWave control module */
                System_printf ("Error: mmWave Control Initialization failed [Error code %d]\n", errCode);

                /* Log into the MCPI Test Logger: */
                MCPI_setFeatureTestResult ("MMWave DSS [Isolation Mode] Initialization", MCPI_TestResult_FAIL);
                return;
            }
            MCPI_setFeatureTestResult ("MMWave DSS [Isolation Mode] Initialization", MCPI_TestResult_PASS);
            break;
        }
        case 3:
        {
            /***********************************************************************
             * [Cooperative Mode] MSS executes the mmWave link
             ************************************************************************/
            initCfg.domain                      = MMWave_Domain_DSS;
            initCfg.socHandle                   = socHandle;
            initCfg.eventFxn                    = Mmwave_eventFxn;
            initCfg.cfgMode                     = MMWave_ConfigurationMode_MINIMAL;
            initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
            initCfg.linkCRCCfg.useCRCDriver     = 1U;
            initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
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
                System_printf ("Error: mmWave Control Initialization failed [Error code %d]\n", errCode);

                /* Log into the MCPI Test Logger: */
                MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
                return;
            }
            MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);
            break;
        }
        case 4:
        {
            /***********************************************************************
             * [Cooperative Mode] DSS executes the mmWave link
             ************************************************************************/
            initCfg.domain                      = MMWave_Domain_DSS;
            initCfg.socHandle                   = socHandle;
            initCfg.eventFxn                    = Mmwave_eventFxn;
            initCfg.cfgMode                     = MMWave_ConfigurationMode_MINIMAL;
            initCfg.executionMode               = MMWave_ExecutionMode_COOPERATIVE;
            initCfg.linkCRCCfg.useCRCDriver     = 1U;
            initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
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
                System_printf ("Error: mmWave Control Initialization failed [Error code %d]\n", errCode);

                /* Log into the MCPI Test Logger: */
                MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_FAIL);
                return;
            }
            MCPI_setFeatureTestResult ("MMWave DSS [Cooperative Mode] Initialization", MCPI_TestResult_PASS);
            break;
        }
        default:
        {
            System_printf ("Error: Invalid test selection [%d]\n", gTestSelection);
            return;
        }
    }

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
    taskParams.stackSize = 4*1024;
    Task_create(Mmwave_ctrlTask, &taskParams, NULL);

    /******************************************************************************
     * TEST: Configuration
     * - This only needs to be done if the mmWave execution domain which has been
     *   provisioned to configure the mmWave link
     ******************************************************************************/
    switch (gTestSelection)
    {
        case 1:
        {
            /***********************************************************************
             * [Isolation Mode]: MSS executes the mmWave link
             *  - We should never have reached here. We should be looping above!!!
             ***********************************************************************/
            System_printf ("Error: We should never have reached here! The DSS is in Idle state\n");
            return;
        }
        case 2:
        {
            /***********************************************************************
             * [Isolation Mode] DSS executes the mmWave
             ************************************************************************/
            Mmwave_populateDefaultOpenCfg (&openCfg);

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
             * Minimal Mode: We can now call the mmWave Link API to perform the
             * profile/chirp/frame configuration. This is just an illustration
             ************************************************************************/
            if (Mmwave_testLinkAPI () < 0)
                return;

            /* Populate the calibration configuration: */
            calibrationCfg.dfeDataOutputMode                          = MMWave_DFEDataOutputMode_FRAME;
            calibrationCfg.u.chirpCalibrationCfg.enableCalibration    = true;
            calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity    = true;
            calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames = 10U;

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
                               (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Info",
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
            break;
        }
        case 3:
        {
            /***********************************************************************
             * [Cooperative Mode] MSS executes the mmWave link
             * - Even in the MINIMAL configuration mode the peer should be notified
             *   that the mmWave has been started and is operational.
             ************************************************************************/
            while (1)
            {
                if (gStartDetected == 1U)
                    break;
                Task_sleep(1);
            }
            MCPI_setFeatureTestResult ("MMWave DSS Start Callback", MCPI_TestResult_PASS);

            /***********************************************************************
             * Minimal Configuration: The configuration callback is the responsibility
             * of the application and not mmWave.
             ************************************************************************/
            if (gConfigDetected == 1U)
            {
                MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_FAIL);
                return;
            }
            MCPI_setFeatureTestResult ("MMWave DSS Configuration Callback", MCPI_TestResult_PASS);

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
            break;
        }
        case 4:
        {
            /***********************************************************************
             * [Cooperative Mode] DSS executes the mmWave link
             ************************************************************************/
            Mmwave_populateDefaultOpenCfg (&openCfg);

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

            /************************************************************************
             * Minimal Mode: We can now call the mmWave Link API to perform the
             * profile/chirp/frame configuration. This is just an illustration
             ************************************************************************/
            if (Mmwave_testLinkAPI () < 0)
                return;

            /* Populate the calibration configuration: */
            calibrationCfg.dfeDataOutputMode                          = MMWave_DFEDataOutputMode_FRAME;
            calibrationCfg.u.chirpCalibrationCfg.enableCalibration    = true;
            calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity    = true;
            calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames = 10U;

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
                               (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Info",
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
                /* Error: Unable to close the mmWave control module */
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
            if (gStopDetected == 1)
            {
                /* Error: Our callback function was invoked */
                MCPI_setFeatureTestResult ("MMWave DSS Close Callback", MCPI_TestResult_FAIL);
            }
            else
            {
                /* Error: Our callback function was NOT invoked */
                MCPI_setFeatureTestResult ("MMWave DSS Close Callback", MCPI_TestResult_PASS);
            }
            break;
        }
        default:
        {
            System_printf ("Error: We should never reach here [Test Selection: %d]\n", gTestSelection);
            return;
        }
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

    /* All the tests completed successfully. */
    System_printf ("Debug: mmWave Unit Tests completed successfully\n");

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

    /* Populate the SOC configuration: We are bypassing the clock initialization
     * in the unit test here since the MSS unit test is doing this. */
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
    System_printf ("Debug: Launching the mmwaveLink\n");
    System_printf ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(MmwaveLink_initTask, &taskParams, NULL);

    /* Debug Message: */
    System_printf ("Debug: Launching BIOS\n");

    /* Start BIOS */
    BIOS_start();
    return 0;
}


