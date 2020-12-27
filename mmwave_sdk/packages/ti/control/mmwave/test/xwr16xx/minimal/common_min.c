/*
 *   @file  common.c
 *
 *   @brief
 *      The file contains functions which are reused among the DSS and
 *      MSS code base
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

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern uint32_t         gConfigDetected;
extern uint32_t         gStartDetected;
extern uint32_t         gStopDetected;
extern uint32_t         gOpenDetected;
extern uint32_t         gCloseDetected;
extern MMWave_Handle    gMMWaveHandle;

/**************************************************************************
 ************************* Common Test Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default open configuration.
 *
 *  @param[out]  ptrOpenCfg
 *      Pointer to the open configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultOpenCfg (MMWave_OpenCfg* ptrOpenCfg)
{
    /* Initialize the open configuration: */
    memset ((void*)ptrOpenCfg, 0, sizeof(MMWave_OpenCfg));

    /* Setup the frequency for calibrations. */
    ptrOpenCfg->freqLimitLow  = 760U;
    ptrOpenCfg->freqLimitHigh = 810U;

    /* Enable start/stop async events */
    ptrOpenCfg->disableFrameStartAsyncEvent = false;
    ptrOpenCfg->disableFrameStopAsyncEvent  = false;

    /* Initialize the channel configuration: */
    ptrOpenCfg->chCfg.rxChannelEn = 0xF;
    ptrOpenCfg->chCfg.txChannelEn = 1;
    ptrOpenCfg->chCfg.cascading   = 0;

    /* Initialize the low power mode configuration: */
    ptrOpenCfg->lowPowerMode.lpAdcMode     = 1;

    /* Initialize the ADCOut configuration: */
    ptrOpenCfg->adcOutCfg.fmt.b2AdcBits   = 2;
    ptrOpenCfg->adcOutCfg.fmt.b2AdcOutFmt = 2;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the usage of the mmWave Link API
 *      since we are testing minimal mode.
 *
 *  @retval
 *      Not applicable
 */
int32_t Mmwave_testLinkAPI (void)
{
    rlProfileCfg_t      profileCfg;
    rlChirpCfg_t        chirpCfg;
    rlFrameCfg_t        frameCfg;
    int32_t             retVal;

    /* Initialize the chirp configuration: */
    memset ((void*)&profileCfg, 0, sizeof(rlProfileCfg_t));

    /* Populate the profile configuration: */
    profileCfg.profileId             = 0;
    profileCfg.startFreqConst        = (uint32_t) ((float)77 * (1U << 26) / 3.6);
    profileCfg.idleTimeConst         = 20 * 1000 / 10;
    profileCfg.adcStartTimeConst     = 5 * 1000 / 10;
    profileCfg.rampEndTime           = 50 * 1000 /10;
    profileCfg.txOutPowerBackoffCode = 0;
    profileCfg.txPhaseShifter        = 0;
    profileCfg.freqSlopeConst        = 20 * 1000 / 48;
    profileCfg.txStartTime           = 1 * 1000 / 10;
    profileCfg.numAdcSamples         = 256;
    profileCfg.digOutSampleRate      = 6000;
    profileCfg.hpfCornerFreq1        = 0;
    profileCfg.hpfCornerFreq2        = 0;
    profileCfg.rxGain                = 30;

    /* Setup the profile configuration: */
    retVal = rlSetProfileConfig (RL_DEVICE_MAP_INTERNAL_BSS, 1U, &profileCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to configure the profile module */
        System_printf ("Error: mmWave link profile configuration failed [Error code %d]\n", retVal);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave Link Profile API Usage", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("MMWave Link Profile API Usage", MCPI_TestResult_PASS);

    /* Initialize the chirp configuration: */
    memset ((void*)&chirpCfg, 0, sizeof(rlChirpCfg_t));

    /* Setup the chirp configuration: */
    chirpCfg.chirpStartIdx   = 0;
    chirpCfg.chirpEndIdx     = 0;
    chirpCfg.profileId       = 0;
    chirpCfg.startFreqVar    = 0;
    chirpCfg.freqSlopeVar    = 0;
    chirpCfg.idleTimeVar     = 0;
    chirpCfg.adcStartTimeVar = 0;
    chirpCfg.txEnable        = 1;

    /* Setup the chirp configuration: Using the mmWave Link API directly */
    retVal = rlSetChirpConfig (RL_DEVICE_MAP_INTERNAL_BSS, 1U, &chirpCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to configure the chirp module */
        System_printf ("Error: mmWave link chirp configuration failed [Error code %d]\n", retVal);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave Link Chirp API Usage", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("MMWave Link Chirp API Usage", MCPI_TestResult_PASS);

    /* Initialize the frame configuration: */
    memset ((void*)&frameCfg, 0, sizeof(rlFrameCfg_t));

    /* Populate the frame configuration: */
    frameCfg.chirpStartIdx      = 0;
    frameCfg.chirpEndIdx        = 0;
    frameCfg.numLoops           = 128;
    frameCfg.numFrames          = 1;
    frameCfg.numAdcSamples      = 256;
    frameCfg.framePeriodicity   = 20 * 1000000 / 5;
    frameCfg.triggerSelect      = 1;
    frameCfg.frameTriggerDelay  = 0;

    /* Setup the frame configuration: Using the mmWave Link API directly */
    retVal = rlSetFrameConfig (RL_DEVICE_MAP_INTERNAL_BSS, &frameCfg);
    if (retVal != RL_RET_CODE_OK)
    {
        /* Error: Unable to configure the chirp module */
        System_printf ("Error: mmWave link frame configuration failed [Error code %d]\n", retVal);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave Link Frame API Usage", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("MMWave Link Frame API Usage", MCPI_TestResult_PASS);
    return 0;
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
 *      Always return 0 to pass the event to the peer domain
 */
int32_t Mmwave_eventFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload)
{
    System_printf ("Debug: BSS Event MsgId: %d [Sub Block Id: %d Sub Block Length: %d]\n",
                    msgId, sbId, sbLen);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Application registered callback function which is invoked after the configuration
 *      has been used to configure the mmWave link and the BSS. This is applicable only for
 *      the XWR16xx. The BSS can be configured only by the MSS *or* DSS. The callback API is
 *      triggered on the remote execution domain (which did not configure the BSS).
 *
 *  @param[in]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_cfgFxn(MMWave_CtrlCfg* ptrCtrlCfg)
{
    /* Error: We are executing the test in MINIMAL mode and so the callback function
     * should never be invoked because in this mode the application is responsible
     * for passing the configuration and not the mmWave module. */
    MCPI_setFeatureTestResult ("MMWave Configuration Callback", MCPI_TestResult_FAIL);

    /* Set the global flag to indicate that the function was invoked. */
    gConfigDetected = 1;

    /* Assertion: In MINIMAL mode this should never ever occur. */
    DebugP_assert (0);
    return;
}

/**
 *  @b Description
 *  @n
 *      Application registered callback function which is invoked the mmWave link on BSS
 *      has been started. This is applicable only for the XWR16xx. The BSS can be configured
 *      only by the MSS *or* DSS. The callback API is triggered on the remote execution
 *      domain (which did not configure the BSS)
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_startFxn(MMWave_CalibrationCfg* ptrCalibrationCfg)
{
    /* Set the flag to indicate the start has been detected */
    gStartDetected = 1U;
}

/**
 *  @b Description
 *  @n
 *      Application registered callback function which is invoked the mmWave link on BSS
 *      has been stopped. This is applicable only for the XWR16xx. The BSS can be configured
 *      only by the MSS *or* DSS. The callback API is triggered on the remote execution
 *      domain (which did not configure the BSS)
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_stopFxn(void)
{
    /* Set the flag to indicate the stop has been detected */
    gStopDetected = 1U;
}

/**
 *  @b Description
 *  @n
 *      Application registered callback function which is invoked after the mmWave
 *      module has been opened.
 *
 *  @param[in]  ptrOpenCfg
 *      Pointer to the open configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_openFxn (MMWave_OpenCfg* ptrOpenCfg)
{
    MMWave_OpenCfg      defaultOpenCfg;

    /* Get the default open configuration */
    Mmwave_populateDefaultOpenCfg (&defaultOpenCfg);

    /* Compare the default with the received configuration: */
    if (memcmp ((void *)&defaultOpenCfg, (void *)ptrOpenCfg, sizeof(MMWave_OpenCfg)) != 0)
    {
        /* Error: There is a mismatch in the open configuration */
        System_printf ("Error: Mismatch in the open configuration\n");
        MCPI_setFeatureTestResult ("MMWave Open Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
    }
    MCPI_setFeatureTestResult ("MMWave Open Configuration Validation", MCPI_TestResult_PASS);

    /* Set the flag to indicate that the open has been detected */
    gOpenDetected = 1U;
    return;
}

/**
 *  @b Description
 *  @n
 *      Application registered callback function which is invoked after the mmWave
 *      module has been closed.
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_closeFxn (void)
{
    /* Set the flag to indicate that the close has been detected */
    gCloseDetected = 1U;
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
void Mmwave_ctrlTask(UArg arg0, UArg arg1)
{
    int32_t errCode;

    while (1)
    {
        /* Execute the mmWave control module: */
        if (MMWave_execute (gMMWaveHandle, &errCode) < 0)
            System_printf ("Error: mmWave control execution failed [Error code %d]\n", errCode);
    }
}

