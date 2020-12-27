/*
 *   @file  mss.c
 *
 *   @brief
 *      MSS Implementation of the CSI Stream
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

/** @mainpage MSS CSI Stream Application for XWR14xx
 *
 * ## Introduction #
 *
 * The CSI Stream application on the XWR14xx is responsible for following:-
 *  - Setup of the control path
 *      This is done via the MMWave Module.
 *  - Setup of the data path
 *      This is done via the Test Framework. The MSS is responsible for
 *      the execution of the data profile so the application loads the
 *      CSI Stream Profile.
 *
 * The application initializes the modules for the control & data path.
 * This would imply the creation of the following tasks:-
 *
 * 1. mmWave Task: Responsible for the execution of the communication
 *    between the MSS & BSS. This would include the configuration of the
 *    BSS and handling any control notifications from the BSS. For more
 *    information please refer to the mmWave documentation.
 *
 * 2. Framework Task: This is the task which is responsible for the
 *    execution and configuration of the CSI Stream Profile.
 *
 * At this time control & data tasks have been initialized and are
 * operational.
 *
 * ## Configuration #
 *
 * The application uses a hardcoded configuration to open the mmWave
 * control module. The same configuration is passed to the Test Framework
 * through the \ref TestFmk_open API. At this stage the application has
 * the data & control paths opened with the same configuration.
 *
 * The application uses hardcoded configuration for the chirp, profile
 * and frame. This configuration is then passed to the BSS via the mmWave
 * control module.
 *
 * Once the mmWave link has been configured it is time to now configure
 * the Test Framework and profile with the configuration. Please refer to
 * the CSI Stream configuration commands \ref CSI_STREAM_PROFILE_COMMAND
 *
 * The application uses the Test Framework IOCTL API \ref TestFmk_ioctl
 * to configure the CSI Stream Profile. Since there are no more
 * configuration commands the Test Framework will finalize the configuration
 * via the \ref TEST_FMK_CMD_SET_CONFIG.
 *
 * ## Start #
 *
 * The data & control paths have now been configured. Before we can start
 * the mmWave sensor the Test Framework needs to notify the profile that
 * the configuration needs to be applied. Once the profile has been
 * successfully started the application can now start the mmWave sensor.
 * This will now cause the ADC Data to be received and the profile is
 * notified via the chirp ISR.
 *
 * ## Stop #
 *
 * Once all the configured frames have been sent the BSS will inform the
 * application via the event RL_RF_AE_FRAME_END_SB. At this time we can
 * stop the data path using the \ref TestFmk_stop.
 */

/** @defgroup MSS_XWR14_CSI_STREAM_APP      MSS CSI Stream Application for XWR14xx
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
#include <ti/sysbios/family/arm/v7r/vim/Hwi.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/common/mmwave_error.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/edma/edma.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/test/common/framework.h>
#include <ti/drivers/test/csi_stream/csi_stream.h>

/** @addtogroup MSS_XWR14_CSI_STREAM_APP
 @{ */

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/**
 * @brief   Task Priority for the mmWave control module:
 */
#define MSS_APP_MMWAVE_CTRL_TASK_PRIORITY       5U

/**
 * @brief   Task Priority for the Test Framework module:
 *  This should always have a lower priority in the system than the control
 *  task priority.
 */
#define MSS_APP_TEST_FRAMEWORK_TASK_PRIORITY    (MSS_APP_MMWAVE_CTRL_TASK_PRIORITY - 1U)

/**************************************************************************
 *************************** Local Functions ******************************
 **************************************************************************/

static void CSIStream_reportFxn
(
    TestFmk_Report  reportType,
    int32_t         errCode,
    uint32_t        arg0,
    uint32_t        arg1
);
static int32_t CSIStream_eventCallbackFxn
(
    uint16_t    msgId,
    uint16_t    sbId,
    uint16_t    sbLen,
    uint8_t*    payload
);
static void CSIStream_initLinkProfileCfgParams (rlProfileCfg_t* ptrProfileCfg);
static void CSIStream_initLinkChirpCfgParams (rlChirpCfg_t* ptrChirpCfg);
static void CSIStream_initLinkFrameParams (rlFrameCfg_t* ptrFrameCfg);
static void CSIStream_initMMWaveOpenParams (MMWave_OpenCfg* ptrOpenCfg);
static void CSIStream_initTestFrameworkParams
(
    uint8_t         rxChannelEnable,
    uint32_t        numADCSamples,
    TestFmk_Cfg*    ptrTestFmkCfg
);
static int32_t CSIStream_run (void);
static void CSIStream_mmWaveCtrlTask(UArg arg0, UArg arg1);
static void CSIStream_fmkTask(UArg arg0, UArg arg1);
static void CSIStream_initTask(UArg arg0, UArg arg1);

/**************************************************************************
 ************************* Application Functions **************************
 **************************************************************************/

/**
 * @brief
 *  CSI Stream Configuration
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  which is used to execute the CSI Stream test
 */
typedef struct CSIStream_Cfg_t
{
    /**
     * @brief   mmWave control configuration:
     */
    MMWave_CtrlCfg          ctrlCfg;

    /**
     * @brief   mmWave open configuration:
     */
    MMWave_OpenCfg          openCfg;
}CSIStream_Cfg;

/**
 * @brief
 *  CSI Stream statistics
 *
 * @details
 *  The structure is used to hold the statistics information for the
 *  CSI Stream Test
 */
typedef struct CSIStream_Stats_t
{
    /**
     * @brief   Counter which tracks the number of frame trigger from BSS
     */
    uint32_t     frameTrigEvt;

    /**
     * @brief   Counter which tracks the number of Monitoring Report received from BSS
     */
    uint32_t     monRepEvt;

    /**
     * @brief   Counter which tracks the number of Calibration Report received from BSS
     */
    uint32_t     calibRepEvt;
}CSIStream_Stats;

/**
 * @brief
 *  Stream Demo Master Control Block
 *
 * @details
 *  The structure is used to hold all the relevant information for the
 *  Stream Demo.
 */
typedef struct CSIStream_MCB_t
{
    /**
     * @brief   Configuration which is used to execute the demo
     */
    CSIStream_Cfg      cfg;

    /**
     * @brief   SOC Handle:
     */
    SOC_Handle          socHandle;

    /**
     * @brief   This is the mmWave control handle which is used to configure the BSS.
     */
    MMWave_Handle       ctrlHandle;

    /**
     * @brief   Status flag which indicates that the mmWave module has been opened
     */
    bool                isMMWaveControlOpened;

    /**
     * @brief   Test Framework Handle:
     */
    TestFmk_Handle      fmkHandle;

    /**
     * @brief   Statistics associated with the Stream Demo
     */
    CSIStream_Stats    stats;
}CSIStream_MCB;

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Global Variable for tracking information required by the CSI Streaming Demo
 */
CSIStream_MCB    gCSIStreamMCB;

/**************************************************************************
 ************************** CSI Stream Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to intialize the mmWave Link Profile configuration
 *
 *  @param[out]  ptrProfileCfg
 *      Populated link profile configuration
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_initLinkProfileCfgParams (rlProfileCfg_t* ptrProfileCfg)
{
    /* Initialize the mmWave Link Profile configuration: */
    memset ((void *)ptrProfileCfg, 0, sizeof(rlProfileCfg_t));

    /* Populate the profile configuration: */
    ptrProfileCfg->profileId             = 0U;
    ptrProfileCfg->startFreqConst        = (uint32_t)(77.0 * (1U << 26) / 3.6);
    ptrProfileCfg->idleTimeConst         = (uint32_t)(20U * 1000 / 10);
    ptrProfileCfg->adcStartTimeConst     = (uint32_t)(5 * 1000 / 10);
    ptrProfileCfg->rampEndTime           = (uint32_t)(80 * 1000 / 10);
    ptrProfileCfg->txOutPowerBackoffCode = 0U;
    ptrProfileCfg->txPhaseShifter        = 0U;
    ptrProfileCfg->freqSlopeConst        = (int16_t)(40U * (1U << 26) / (3.6*1e3*900));
    ptrProfileCfg->txStartTime           = (int32_t)(1U * 1000 / 10);
    ptrProfileCfg->numAdcSamples         = 256;
    ptrProfileCfg->digOutSampleRate      = 6000;
    ptrProfileCfg->hpfCornerFreq1        = 0;
    ptrProfileCfg->hpfCornerFreq2        = 0;
    ptrProfileCfg->rxGain                = 30;
}

/**
 *  @b Description
 *  @n
 *      The function is used to intialize the mmWave Link chirp configuration
 *
 *  @param[out]  ptrChirpCfg
 *      Populated link chirp configuration
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_initLinkChirpCfgParams (rlChirpCfg_t* ptrChirpCfg)
{
    /* Initialize the chirp configuration: */
    memset ((void *)ptrChirpCfg, 0, sizeof(rlChirpCfg_t));

    /* Populate the chirp configuration: */
    ptrChirpCfg->chirpStartIdx   = 0U;
    ptrChirpCfg->chirpEndIdx     = 0U;
    ptrChirpCfg->profileId       = 0U;
    ptrChirpCfg->startFreqVar    = (uint32_t)(0U * (1U << 26) / 3.6e9);
    ptrChirpCfg->freqSlopeVar    = (uint16_t)(0U * (1U << 26) / (3.6e6 * 900.0));
    ptrChirpCfg->idleTimeVar     = (uint32_t)(0U * 1000 / 10);
    ptrChirpCfg->adcStartTimeVar = (uint32_t)(0U * 1000 / 10);
    ptrChirpCfg->txEnable        = 1U;
}

/**
 *  @b Description
 *  @n
 *      The function is used to intialize the mmWave Link frame configuration
 *
 *  @param[out]  ptrFrameCfg
 *      Populated link frame configuration
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_initLinkFrameParams (rlFrameCfg_t* ptrFrameCfg)
{
    /* Populate the frame configuration: */
    ptrFrameCfg->chirpStartIdx      = 0U;
    ptrFrameCfg->chirpEndIdx        = 0U;
    ptrFrameCfg->numLoops           = 128;
    ptrFrameCfg->numFrames          = 1;
    ptrFrameCfg->framePeriodicity   = (uint32_t)(26 * 1000000 / 5);
    ptrFrameCfg->triggerSelect      = 1U;
    ptrFrameCfg->frameTriggerDelay  = (uint32_t)(0 * 1000000 / 5);
}

/**
 *  @b Description
 *  @n
 *      The function is used to intialize the mmWave open configuration
 *
 *  @param[out]  ptrOpenCfg
 *      Populated open configuration
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_initMMWaveOpenParams (MMWave_OpenCfg* ptrOpenCfg)
{
    /* Initialize the open configuration: */
    memset ((void*)ptrOpenCfg, 0, sizeof(MMWave_OpenCfg));

    /* Populate the open configuration: */
    ptrOpenCfg->freqLimitLow                        = 760U;
    ptrOpenCfg->freqLimitHigh                       = 810U;
    ptrOpenCfg->chCfg.rxChannelEn                   = 2U;
    ptrOpenCfg->chCfg.txChannelEn                   = 1U;
    ptrOpenCfg->chCfg.cascading                     = 0U;
    ptrOpenCfg->lowPowerMode.lpAdcMode              = 1U;
    ptrOpenCfg->adcOutCfg.fmt.b2AdcBits             = 2U;
    ptrOpenCfg->adcOutCfg.fmt.b2AdcOutFmt           = 2U;
    ptrOpenCfg->disableFrameStartAsyncEvent         = false;
    ptrOpenCfg->disableFrameStopAsyncEvent          = false;
    ptrOpenCfg->useCustomCalibration                = false;
    ptrOpenCfg->customCalibrationEnableMask         = 0x0U;
}

/**
 *  @b Description
 *  @n
 *      The function is used to intialize the Test Framework configuration
 *
 *  @param[in]  rxChannelEnable
 *      Receive Channel Enable Mask
 *  @param[in]  numADCSamples
 *      Number of ADC samples
 *  @param[out]  ptrTestFmkCfg
 *      Populated framework configuration
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_initTestFrameworkParams
(
    uint8_t         rxChannelEnable,
    uint32_t        numADCSamples,
    TestFmk_Cfg*    ptrTestFmkCfg
)
{
    /* Initialize the framework configuration: */
    memset ((void*)ptrTestFmkCfg, 0, sizeof(TestFmk_Cfg));

    /* Populate the framework configuration: */
    ptrTestFmkCfg->dfeDataOutputMode        = MMWave_DFEDataOutputMode_FRAME;
    ptrTestFmkCfg->adcFmt                   = 0U;
    ptrTestFmkCfg->iqSwapSel                = 0U;
    ptrTestFmkCfg->chInterleave             = 0U;
    ptrTestFmkCfg->chirpThreshold           = 1U;
    ptrTestFmkCfg->rxChannelEn              = rxChannelEnable;
    ptrTestFmkCfg->u.frameCfg.numADCSamples = numADCSamples;
}

/**
 *  @b Description
 *  @n
 *      Registered event function which is invoked when an event from the
 *      BSS is received.
 *
 *  @param[in]  msgId
 *      Message Identifier
 *  @param[in]  sbId
 *      Subblock identifier
 *  @param[in]  sbLen
 *      Length of the subblock
 *  @param[in]  payload
 *      Pointer to the payload buffer
 *
 *  @retval
 *      Always return 0 to pass the event message to the remote domain
 */
static int32_t CSIStream_eventCallbackFxn
(
    uint16_t    msgId,
    uint16_t    sbId,
    uint16_t    sbLen,
    uint8_t*    payload
)
{
    int32_t  errCode;
    uint16_t asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);

    /* Process the received message: */
    switch (msgId)
    {
        case RL_RF_ASYNC_EVENT_MSG:
        {
            /* Received Asychronous Message: */
            switch (asyncSB)
            {
                case RL_RF_AE_CPUFAULT_SB:
                case RL_RF_AE_ESMFAULT_SB:
                case RL_RF_AE_ANALOG_FAULT_SB:
                {
                    /* Fault has been reported by the BSS */
                    DebugP_assert (0);
                    break;
                }
                case RL_RF_AE_INITCALIBSTATUS_SB:
                {
                    rlRfInitComplete_t*  ptrRFInitCompleteMessage;
                    uint32_t            calibrationStatus;

                    /* Get the RF-Init completion message: */
                    ptrRFInitCompleteMessage = (rlRfInitComplete_t*)payload;
                    calibrationStatus = ptrRFInitCompleteMessage->calibStatus & 0xFFFU;

                    /* Display the calibration status: */
                    System_printf ("Debug: Init Calibration Status = 0x%x\n", calibrationStatus);
                    break;
                }
                case RL_RF_AE_FRAME_TRIGGER_RDY_SB:
                {
                    gCSIStreamMCB.stats.frameTrigEvt++;
                    break;
                }
                case RL_RF_AE_MON_TIMING_FAIL_REPORT_SB:
                {
                    gCSIStreamMCB.stats.monRepEvt++;
                    break;
                }
                case RL_RF_AE_RUN_TIME_CALIB_REPORT_SB:
                {
                    gCSIStreamMCB.stats.calibRepEvt++;
                    break;
                }
                case RL_RF_AE_FRAME_END_SB:
                {
                    /******************************************************************
                     * The ASYNC Event is generation when the configuration has caused
                     * all the frames to be sent out. Stop the Profile.
                     ******************************************************************/
                    if (TestFmk_stop (gCSIStreamMCB.fmkHandle, &errCode) < 0)
                    {
                        System_printf ("Error: Unable to stop the profile [Error: %d]\n", errCode);
                        return -1;
                    }
                    break;
                }
                default:
                {
                    DebugP_assert (0);
                    break;
                }
            }
            break;
        }
        default:
        {
            DebugP_assert (0);
            break;
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered Test Framework Report function. This is invoked by the
 *      Test Framework to report the status
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  errCode
 *      Error code associated with the status event.
 *  @param[in]  arg0
 *      Optional argument. This is status specific so please refer to the documentation
 *      about the status
 *  @param[in]  arg1
 *      Optional argument. This is status specific so please refer to the documentation
 *      about the status
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_reportFxn
(
    TestFmk_Report  reportType,
    int32_t         errCode,
    uint32_t        arg0,
    uint32_t        arg1
)
{
    MMWave_CalibrationCfg       calibrationCfg;

    /* Process the Report Status:*/
    switch (reportType)
    {
        case TestFmk_Report_PROFILE_LOADED:
        {
            /* Was there an error reported? */
            if (errCode == 0)
            {
                /* Success: The profile has been loaded successfully. */
                System_printf ("Debug: Profile has been loaded successfully\n");
            }
            else
            {
                /* Error: Unable to load the profile. */
                System_printf ("Error: Unable to load the profile [Error code %d]\n", errCode);
            }
            break;
        }
        case TestFmk_Report_PROFILE_OPENED:
        {
            /* Was there an error reported? */
            if (errCode == 0)
            {
                /* Success: The profile has been opened successfully. */
                System_printf ("Debug: Profile has been opened successfully\n");
            }
            else
            {
                /* Error: Unable to open the profile. */
                System_printf ("Error: Unable to open the profile [Error code %d]\n", errCode);
            }
            break;
        }
        case TestFmk_Report_PROFILE_CLOSED:
        {
            /* Was there an error reported? */
            if (errCode == 0)
            {
                /* Success: The profile has been closed successfully. */
                System_printf ("Debug: Profile has been closed successfully\n");
            }
            else
            {
                /* Error: Unable to close the profile. */
                System_printf ("Error: Unable to close the profile [Error code %d]\n", errCode);
            }
            break;
        }
        case TestFmk_Report_SET_CFG:
        {
            break;
        }
        case TestFmk_Report_IOCTL:
        {
            break;
        }
        case TestFmk_Report_PROFILE_STARTED:
        {
            /* Was there an error reported? */
            if (errCode == 0)
            {
                /* Success: The profile has been configured successfully. */
                System_printf ("Debug: Profile has been started successfully\n");

                /* Initialize the calibration configuration: */
                memset ((void*)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));

                /* Populate the calibration configuration: */
                calibrationCfg.dfeDataOutputMode                          = MMWave_DFEDataOutputMode_FRAME;
                calibrationCfg.u.chirpCalibrationCfg.enableCalibration    = true;
                calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity    = true;
                calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames = 10U;

                /* Start the mmWave: This will start the sensor. */
                if (MMWave_start (gCSIStreamMCB.ctrlHandle, &calibrationCfg, &errCode) < 0)
                {
                    System_printf ("Error: Unable to start the mmWave module [Error: %d]\n", errCode);
                    return;
                }
            }
            else
            {
                /* Error: Unable to start the profile. */
                System_printf ("Error: Unable to start the profile [Error code %d]\n", errCode);
            }
            break;
        }
        case TestFmk_Report_PROFILE_STOPPED:
        {
            /* Was there an error reported? */
            if (errCode == 0)
            {
                /* Success: The profile has been closed successfully. */
                System_printf ("Debug: Profile has been stopped successfully\n");
            }
            else
            {
                /* Error: Unable to close the profile. */
                System_printf ("Error: Unable to stop the profile [Error code %d]\n", errCode);
            }
            break;
        }
        case TestFmk_Report_PROFILE_UNLOADED:
        {
            /* Was there an error reported? */
            if (errCode == 0)
            {
                /* Success: The profile has been unloaded successfully. */
                System_printf ("Debug: Profile has been unloaded successfully\n");
            }
            else
            {
                /* Error: Unable to unload the profile. */
                System_printf ("Error: Unable to unload the profile [Error code %d]\n", errCode);
            }
            break;
        }
        default:
        {
            DebugP_assert (0);
            break;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The task is used to provide an execution context for the mmWave
 *      control task
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_mmWaveCtrlTask(UArg arg0, UArg arg1)
{
    int32_t errCode;

    while (1)
    {
        /* Execute the mmWave control module: */
        if (MMWave_execute (gCSIStreamMCB.ctrlHandle, &errCode) < 0)
            System_printf ("Error: mmWave control execution failed [Error code %d]\n", errCode);
    }
}

/**
 *  @b Description
 *  @n
 *      The task is used to provide an execution context for the Test Framework
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_fmkTask(UArg arg0, UArg arg1)
{
    int32_t             errCode;
    TestFmk_Result      result;

    while (1)
    {
        /* Execute the Test Framework module: */
        if (TestFmk_execute (gCSIStreamMCB.fmkHandle, &result, &errCode) < 0)
            System_printf ("Error: Test Framework execution failed [Error code %d]\n", errCode);
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to run the CSI Stream application. The
 *      CSI Stream uses a hardcoded configuration which configures
 *      the system.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CSIStream_run (void)
{
    int32_t                 errCode;
    rlProfileCfg_t          profileCfg;
    rlChirpCfg_t            chirpCfg;
    MMWave_ProfileHandle    profileHandle;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveError;
    int16_t                 subSysError;
    TestFmk_Cfg             fmkCfg;
    rlDevHsiClk_t           hsiClkgs;
    int32_t                 retVal;
    uint32_t                numChirpsPerFrame;

    /******************************************************************
     * Open the mmWave module:
     ******************************************************************/
    CSIStream_initMMWaveOpenParams (&gCSIStreamMCB.cfg.openCfg);
    if (MMWave_open (gCSIStreamMCB.ctrlHandle, &gCSIStreamMCB.cfg.openCfg, NULL, &errCode) < 0)
    {
        MMWave_decodeError (errCode, &errorLevel, &mmWaveError, &subSysError);
        System_printf ("Error: Unable to open the mmWave module [Error: %d mmWave: %d SubSys: %d]\n",
                        errCode, mmWaveError, subSysError);
        return -1;
    }
    System_printf ("Debug: mmWave has been opened\n");

    /******************************************************************
     * Open the Test Framework profile:
     ******************************************************************/
    if (TestFmk_open (gCSIStreamMCB.fmkHandle, &gCSIStreamMCB.cfg.openCfg, &errCode) < 0)
    {
        System_printf ("Error: Unable to open the test framework profile [Error: %d]\n", errCode);
        return -1;
    }

    /******************************************************************
     * Configure the mmWave module:
     * - Create the Profile
     ******************************************************************/
    CSIStream_initLinkProfileCfgParams (&profileCfg);

    /* Add the profile to the mmWave module: */
    profileHandle = MMWave_addProfile (gCSIStreamMCB.ctrlHandle, &profileCfg, &errCode);
    if (profileHandle == NULL)
    {
        MMWave_decodeError (errCode, &errorLevel, &mmWaveError, &subSysError);
        System_printf ("Error: Unable to add the profile [Error: %d mmWave: %d SubSys: %d]\n",
                        errCode, mmWaveError, subSysError);
        return -1;
    }

    /******************************************************************
     * Configure the mmWave module:
     * - Create a chirp & associate it with the profile above
     ******************************************************************/
    CSIStream_initLinkChirpCfgParams (&chirpCfg);

    /* Add the chirp to the profile */
    if (MMWave_addChirp (profileHandle, &chirpCfg, &errCode) == NULL)
    {
        MMWave_decodeError (errCode, &errorLevel, &mmWaveError, &subSysError);
        System_printf ("Error: Unable to add the chirp [Error: %d mmWave: %d SubSys: %d]\n",
                        errCode, mmWaveError, subSysError);
        return -1;
    }

    /******************************************************************
     * Configure the mmWave module:
     * - Setup the frame configuration
     ******************************************************************/
    memset ((void *)&gCSIStreamMCB.cfg.ctrlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* We are operating in frame mode: */
    gCSIStreamMCB.cfg.ctrlCfg.dfeDataOutputMode = MMWave_DFEDataOutputMode_FRAME;
    gCSIStreamMCB.cfg.ctrlCfg.u.frameCfg.profileHandle[0] = profileHandle;
    CSIStream_initLinkFrameParams (&gCSIStreamMCB.cfg.ctrlCfg.u.frameCfg.frameCfg);

    /* Configure the mmWave module: */
    if (MMWave_config (gCSIStreamMCB.ctrlHandle, &gCSIStreamMCB.cfg.ctrlCfg, &errCode) < 0)
    {
        MMWave_decodeError (errCode, &errorLevel, &mmWaveError, &subSysError);
        System_printf ("Error: Unable to configure the mmWave module [Error: %d mmWave: %d SubSys: %d]\n",
                        errCode, mmWaveError, subSysError);
        return -1;
    }
    System_printf ("Debug: mmWave has been configured\n");

    /*************************************************************************************
     * Setup the HSI Clock through the mmWave Link:
     *************************************************************************************/
    memset ((void*)&hsiClkgs, 0, sizeof(rlDevHsiClk_t));

    /* Setup the HSI Clock as per the Radar Interface Document:
     * - The CSI Stream Profile is configured to operate @450Mhtz */
    hsiClkgs.hsiClk = 0x6;

    /* Setup the HSI in the radar link: */
    retVal = rlDeviceSetHsiClk(RL_DEVICE_MAP_CASCADED_1, &hsiClkgs);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to set the HSI clock */
        System_printf ("Error: Setting up the HSI Clock in BSS Failed [Error %d]\n", retVal);
        return -1;
    }

    /******************************************************************
     * Profile specific configuration:
     * - Setup the number of chirps per frame
     ******************************************************************/
    numChirpsPerFrame = (gCSIStreamMCB.cfg.ctrlCfg.u.frameCfg.frameCfg.chirpEndIdx -
                         gCSIStreamMCB.cfg.ctrlCfg.u.frameCfg.frameCfg.chirpStartIdx + 1) *
                         gCSIStreamMCB.cfg.ctrlCfg.u.frameCfg.frameCfg.numLoops;

    if (TestFmk_ioctl (gCSIStreamMCB.fmkHandle,
                       CSI_STREAM_PROFILE_SET_NUM_CHIRPS_PER_FRAME,
                       &numChirpsPerFrame,
                       sizeof(numChirpsPerFrame),
                       &errCode) < 0)
    {
        System_printf ("Error: Unable to configure the number of chirps [Error: %d]\n", errCode);
        return -1;
    }

    /******************************************************************
     * Finalize the configuration:
     ******************************************************************/
    CSIStream_initTestFrameworkParams (gCSIStreamMCB.cfg.openCfg.chCfg.rxChannelEn,
                                       profileCfg.numAdcSamples,
                                       &fmkCfg);

    if (TestFmk_ioctl (gCSIStreamMCB.fmkHandle,
                       TEST_FMK_CMD_SET_CONFIG,
                       &fmkCfg,
                       sizeof(fmkCfg),
                       &errCode) < 0)
    {
        System_printf ("Error: Unable to finalize the configuration [Error: %d]\n", errCode);
        return -1;
    }

    /******************************************************************
     * Start the Profile:
     ******************************************************************/
    if (TestFmk_start (gCSIStreamMCB.fmkHandle, &errCode) < 0)
    {
        System_printf ("Error: Unable to start the profile [Error: %d]\n", errCode);
        return -1;
    }

    /********************************************************************************
     * The profile is sent a message and it will be started on the reception of the
     * report. Do not start the sensor till we know that the profile is ready.
     ********************************************************************************/
    return 0;
}

/**
 *  @b Description
 *  @n
 *      MSS SubSystem Initialization Task which initializes the various
 *      components in the MSS subsystem.
 *
 *  @retval
 *      Not Applicable.
 */
static void CSIStream_initTask(UArg arg0, UArg arg1)
{
    Task_Params         taskParams;
    int32_t             errCode;
    MMWave_InitCfg      initCfg;
    TestFmk_InitCfg     fmkInitCfg;

    /* Debug Message: */
    System_printf("Debug: Launched the Demo Initialization Task\n");

    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/

    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_MSS);

    /*****************************************************************************
     * Initialization of the mmWave control module
     *****************************************************************************/
    memset ((void*)&initCfg, 0 , sizeof(MMWave_InitCfg));

    /* Populate the init configuration for mmwave library: */
    initCfg.domain                      = MMWave_Domain_MSS;
    initCfg.socHandle                   = gCSIStreamMCB.socHandle;
    initCfg.eventFxn                    = CSIStream_eventCallbackFxn;
    initCfg.linkCRCCfg.useCRCDriver     = 1U;
    initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
    initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;

    /* Initialize and setup the mmWave Control module */
    gCSIStreamMCB.ctrlHandle = MMWave_init (&initCfg, &errCode);
    if (gCSIStreamMCB.ctrlHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        System_printf("Error: mmWave Control Initialization failed [Error code %d]\n", errCode);
        return;
    }

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequiste and always needs to be invoked. */
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = MMWave_sync (gCSIStreamMCB.ctrlHandle , &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the mmWave control module */
            System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);
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
    System_printf("Debug: mmWave Control Sync was successful\n");

    /* Initialize and Launch the mmWave Link Management Task: */
    Task_Params_init(&taskParams);
    taskParams.priority  = MSS_APP_MMWAVE_CTRL_TASK_PRIORITY;
    taskParams.stackSize = 4*1024;
    Task_create(CSIStream_mmWaveCtrlTask, &taskParams, NULL);

    /*****************************************************************************
     * Initialization of the Test Framework:
     *****************************************************************************/
    memset ((void *)&fmkInitCfg, 0, sizeof(TestFmk_InitCfg));

    /* Setup the configuration: */
    fmkInitCfg.socHandle  = gCSIStreamMCB.socHandle;
    fmkInitCfg.ctrlHandle = gCSIStreamMCB.ctrlHandle;
    fmkInitCfg.reportFxn  = CSIStream_reportFxn;

    /* Initialize the Test Framework: */
    gCSIStreamMCB.fmkHandle = TestFmk_init (&fmkInitCfg, &errCode);
    if (gCSIStreamMCB.fmkHandle == NULL)
    {
        System_printf ("Error: Unable to initialize the Test Framework [Error: %d]\n", errCode);
        return;
    }
    System_printf ("Debug: Test Framework Initialized\n");

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequiste and always needs to be invoked. */
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = TestFmk_synch (gCSIStreamMCB.fmkHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the framework */
            System_printf ("Error: Framework Synchronization failed [Error code %d]\n", errCode);
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
    System_printf("Debug: Framework Sync was successful\n");

    /* Launch the Test Framework Task:
     * - This is always a lower priority than the mmWave control task because
     *   control events should always be higher priority. */
    Task_Params_init(&taskParams);
    taskParams.priority  = MSS_APP_TEST_FRAMEWORK_TASK_PRIORITY;
    taskParams.stackSize = 4*1024;
    Task_create(CSIStream_fmkTask, &taskParams, NULL);

    /*************************************************************************
     * The Framework is operational and we can now load the profile
     *************************************************************************/
    if (TestFmk_loadProfile (gCSIStreamMCB.fmkHandle, &gCSIStreamProfileCfg, &errCode) < 0)
    {
        System_printf ("Error: Unable to load the profile [Error: %d]\n", errCode);
        return;
    }

    /*************************************************************************
     * The CSI Stream runs with a hardcoded configuration:
     *************************************************************************/
    DebugP_assert (CSIStream_run () == 0);
    return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the CSI Stream Unit Test
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params	    taskParams;
    int32_t         errCode;
    SOC_Cfg         socCfg;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);

    /* Initialize the Streaming Demo MCB */
    memset ((void*)&gCSIStreamMCB, 0, sizeof(CSIStream_MCB));

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    gCSIStreamMCB.socHandle = SOC_init (&socCfg, &errCode);
    if (gCSIStreamMCB.socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return 0;
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    Task_create(CSIStream_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}

/**
@}
*/

