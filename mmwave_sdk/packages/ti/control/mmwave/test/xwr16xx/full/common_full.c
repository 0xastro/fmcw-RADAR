/*
 *   @file  common_full.c
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
#include <xdc/runtime/Memory.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/
#define TEST_MAX_PROFILE        2
#define TEST_MAX_CHIRP          4
#define TEST_MAX_BPM_CHIRP      8

/* Tx Channel Configuration */
#define TX_CHANNEL_1_ENABLE                 (1U << 0U)
#define TX_CHANNEL_2_ENABLE                 (1U << 1U)
#define TX_CHANNEL_3_ENABLE                 (1U << 2U)
#define TX_CHANNEL_1_2_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE)
#define TX_CHANNEL_2_3_ENABLE               (TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_3_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_2_3_ENABLE             (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)

/* Rx Channel Configuration */
#define RX_CHANNEL_1_ENABLE                 (1U << 0U)
#define RX_CHANNEL_2_ENABLE                 (1U << 1U)
#define RX_CHANNEL_3_ENABLE                 (1U << 2U)
#define RX_CHANNEL_4_ENABLE                 (1U << 3U)
#define RX_CHANNEL_1_2_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE)
#define RX_CHANNEL_1_3_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_1_4_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_2_3_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_4_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_3_4_ENABLE               (RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_3_4_ENABLE             (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_3_4_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_4_ENABLE           (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)

/* ADC Config Settings */
#define ADC_BITS_12                         (0U)
#define ADC_BITS_14                         (1U)
#define ADC_BITS_16                         (2U)

#define ADC_FORMAT_REAL                     (0U)
#define ADC_FORMAT_COMPLEX                  (1U)
#define ADC_FORMAT_CPMLEX_WITH_IMG_BAND     (2U)

#define ADC_I_FIRST                         (0U)
#define ADC_Q_FIRST                         (1U)

#define ADC_INTERLEAVED_MODE                (0U)
#define ADC_NON_INTERLEAVED_MODE            (1U)

/* Data Path Configuration */
#define DATA_PATH_CSI2                      (0U)
#define DATA_PATH_LVDS                      (1U)


#define DATA_PATH_FMT1_SUPRESS              (0U)
#define DATA_PATH_FMT1_CP_CQ                (1U)
#define DATA_PATH_FMT1_CQ_CP                (2U)

#define DATA_PATH_FMT0_ADC_DATA_ONLY        (0U)
#define DATA_PATH_FMT0_CP_ADC_DATA          (1U)
#define DATA_PATH_FMT0_ADC_CP_DATA          (2U)
#define DATA_PATH_FMT0_CP_ADC_CQ_DATA       (3U)

#define DATA_PATH_CQ_FMT_BITS_12            (0U)
#define DATA_PATH_CQ_FMT_BITS_14            (1U)
#define DATA_PATH_CQ_FMT_BITS_16            (2U)

/* LVDS Clock Configuration */
#define LVDS_LANE_CLOCK_SDR                 (0U)
#define LVDS_LANE_CLOCK_DDR                 (1U)

#define LVDS_ALL_LANE_EN                    (0xFU)

#define LVDS_DATA_RATE_900                  (0U)
#define LVDS_DATA_RATE_600                  (1U)
#define LVDS_DATA_RATE_450                  (2U)
#define LVDS_DATA_RATE_400                  (3U)
#define LVDS_DATA_RATE_300                  (4U)
#define LVDS_DATA_RATE_225                  (5U)
#define LVDS_DATA_RATE_150                  (6U)

/* LCDS Lane Configuration */
#define LVDS_LANE1_DISABLE                  (0U)
#define LVDS_LANE1_FORMAT_0                 (1U)
#define LVDS_LANE1_FORMAT_1                 (2U)

#define LVDS_LANE2_DISABLE                  (0U)
#define LVDS_LANE2_FORMAT_0                 (1U)
#define LVDS_LANE2_FORMAT_1                 (2U)

#define LVDS_LANE3_DISABLE                  (0U)
#define LVDS_LANE3_FORMAT_0                 (1U)
#define LVDS_LANE3_FORMAT_1                 (2U)

#define LVDS_LANE4_DISABLE                  (0U)
#define LVDS_LANE4_FORMAT_0                 (1U)
#define LVDS_LANE4_FORMAT_1                 (2U)

#define LVDS_LANE_MSB_FIRST_ENABLE          (1U)
#define LVDS_LANE_MSB_FIRST_DISABLE         (0U)
#define LVDS_LANE_PACKET_END_PULSE_ENABLE   (1U)
#define LVDS_LANE_PACKET_END_PULSE_DISABLE  (0U)
#define LVDS_LANE_CRC_ENABLE                (1U)
#define LVDS_LANE_CRC_DISABLE               (0U)
#define LVDS_LANE_TI_MODE_ENABLE            (1U)
#define LVDS_LANE_TI_MODE_DISABLE           (0U)

/* Advanced configuration */
#define ANA_CHANNEL_COMPLEX_CHAIN           (0U)
#define ANA_CHANNEL_REAL_CHAIN              (1U)

#define LP_ADC_MODE_REGULAR                 (0U)
#define LP_ADC_MODE_LOW_POWER               (1U)

#define NOISE_FIGURE_LOW                    (0U)
#define NOISE_FIGURE_HIGH                   (1U)


/* CHIRP tab settings */
/* CHIRP Sub-tab */
#define CHIRP_0_PROFILE_ID                (0U)
#define CHIRP_0_START_INDEX               (0U)
#define CHIRP_0_END_INDEX                 (0U)
#define CHIRP_0_START_FREQ_VAL            (0U)
#define CHIRP_0_FREQ_SLOPE_VAL            (0U)
#define CHIRP_0_IDLE_TIME_VAL             (0U)
#define CHIRP_0_ADC_START_TIME_VAL        (0U)
#define CHIRP_0_TX_CHANNEL                CHIRP_TX_1_CHANNEL_ENABLE

#define CHIRP_1_PROFILE_ID                (1U)
#define CHIRP_1_START_INDEX               (1U)
#define CHIRP_1_END_INDEX                 (1U)
#define CHIRP_1_START_FREQ_VAL            (0U)
#define CHIRP_1_FREQ_SLOPE_VAL            (0U)
#define CHIRP_1_IDLE_TIME_VAL             (0U)
#define CHIRP_1_ADC_START_TIME_VAL        (0U)
#define CHIRP_1_TX_CHANNEL                CHIRP_TX_1_CHANNEL_ENABLE

/* PROFILE Sub-tab */
#define PROFILE_0_PROFILE_ID              (0U)
#define PROFILE_0_HPFCORNER_FREQ1_VAL     RL_RX_HPF1_175_KHz
#define PROFILE_0_HPFCORNER_FREQ2_VAL     RL_RX_HPF2_350_KHz
#define PROFILE_0_RX_GAIN_VAL             (30U)
#define PROFILE_0_DIGOUT_SAMPLERATE_VAL   (10000U)
#define PROFILE_0_ADC_SAMPLE_VAL          (224U)
#define PROFILE_0_IDLE_TIME_VAL           (1500U)
#define PROFILE_0_RAMP_END_TIME_VAL       (3000U)
#define PROFILE_0_START_FREQ_VAL          (1454025386U)
#define PROFILE_0_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_0_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_0_FREQ_SLOPE_VAL          (1657U)
#define PROFILE_0_TX_START_TIME_VAL       (100U)  // 1us
#define PROFILE_0_ADC_START_TIME_VAL      (600U)  // 6us

/* PROFILE Sub-tab */
#define PROFILE_1_PROFILE_ID              (1U)
#define PROFILE_1_HPFCORNER_FREQ1_VAL     RL_RX_HPF1_175_KHz
#define PROFILE_1_HPFCORNER_FREQ2_VAL     RL_RX_HPF2_350_KHz
#define PROFILE_1_RX_GAIN_VAL             (30U)
#define PROFILE_1_DIGOUT_SAMPLERATE_VAL   (10000U)
#define PROFILE_1_ADC_SAMPLE_VAL          (512U)
#define PROFILE_1_IDLE_TIME_VAL           (1500U)
#define PROFILE_1_RAMP_END_TIME_VAL       (6000U)
#define PROFILE_1_START_FREQ_VAL          (1454025386U)  //78Ghz
#define PROFILE_1_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_1_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_1_FREQ_SLOPE_VAL          (828U)
#define PROFILE_1_TX_START_TIME_VAL       (100U)  // 1us
#define PROFILE_1_ADC_START_TIME_VAL      (600U)  // 6us

/* FRAME Sub-tab */
#define FRAME_CHIRP_START_IDX           (0U)
#define FRAME_CHIRP_END_IDX             (0U)
#define FRAME_COUNT_VAL                 (0)
#define FRAME_LOOP_COUNT                (64)
#define FRAME_PRIODICITY_VAL            (16000000)
#define FRAME_TRIGGER_DELAY_VAL         (0U)
#define FRAME_ADC_SAMPLE_NUM            (512U)


/* CHIRP Tx Channel Settings */
#define CHIRP_TX_1_CHANNEL_ENABLE           (1U << 0U)
#define CHIRP_TX_2_CHANNEL_ENABLE           (1U << 1U)
#define CHIRP_TX_3_CHANNEL_ENABLE           (1U << 2U)
#define CHIRP_TX_1_2_CHANNEL_ENABLE         (CHIRP_TX_1_CHANNEL_ENABLE | CHIRP_TX_2_CHANNEL_ENABLE)
#define CHIRP_TX_1_3_CHANNEL_ENABLE         (CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)
#define CHIRP_TX_2_3_CHANNEL_ENABLE         (CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)
#define CHIRP_TX_1_2_3_CHANNEL_ENABLE       (CHIRP_TX_1_CHANNEL_ENABLE | CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)

/* CHIRP Profile Settings */
#define CHIRP_HPF1_CORNER_FREQ_175K         (0U)
#define CHIRP_HPF1_CORNER_FREQ_235K         (1U)
#define CHIRP_HPF1_CORNER_FREQ_350K         (2U)
#define CHIRP_HPF1_CORNER_FREQ_700K         (3u)

#define CHIRP_HPF2_CORNER_FREQ_350K         (0U)
#define CHIRP_HPF2_CORNER_FREQ_700K         (1U)
#define CHIRP_HPF2_CORNER_FREQ_1_4M         (2U)
#define CHIRP_HPF2_CORNER_FREQ_2_8M         (3U)
#define CHIRP_HPF2_CORNER_FREQ_5M           (4U)
#define CHIRP_HPF2_CORNER_FREQ_7_5M         (5U)
#define CHIRP_HPF2_CORNER_FREQ_10M          (6U)
#define CHIRP_HPF2_CORNER_FREQ_15M          (7U)

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern uint32_t         gConfigDetected;
extern uint32_t         gStartDetected;
extern uint32_t         gStopDetected;
extern uint32_t         gOpenDetected;
extern uint32_t         gCloseDetected;
extern int32_t          gTestSelection;
extern MMWave_Handle    gMMWaveHandle;

/**************************************************************************
 ************************* Common Test Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Utility function which populates the profile configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrProfileCfg
 *      Pointer to the populated profile configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultProfileCfg (rlProfileCfg_t* ptrProfileCfg)
{
    /* Initialize the profile configuration: */
    memset ((void*)ptrProfileCfg, 0, sizeof(rlProfileCfg_t));

    /* Populate the *default* profile configuration: */
    ptrProfileCfg->profileId             = 0;
    ptrProfileCfg->startFreqConst        = (uint32_t) ((float)77 * (1U << 26) / 3.6);
    ptrProfileCfg->idleTimeConst         = 20 * 1000 / 10;
    ptrProfileCfg->adcStartTimeConst     = 5 * 1000 / 10;
    ptrProfileCfg->rampEndTime           = 50 * 1000 /10;
    ptrProfileCfg->txOutPowerBackoffCode = 0;
    ptrProfileCfg->txPhaseShifter        = 0;
    ptrProfileCfg->freqSlopeConst        = 20 * 1000 / 48;
    ptrProfileCfg->txStartTime           = 1 * 1000 / 10;
    ptrProfileCfg->numAdcSamples         = 256;
    ptrProfileCfg->digOutSampleRate      = 6000;
    ptrProfileCfg->hpfCornerFreq1        = 0;
    ptrProfileCfg->hpfCornerFreq2        = 0;
    ptrProfileCfg->rxGain                = 30;
}

/**
 *  @b Description
 *  @n
 *      Utility function which populates the chirp configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrChirpCfg
 *      Pointer to the populated chirp configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultChirpCfg (rlChirpCfg_t* ptrChirpCfg)
{
    /* Initialize the chirp configuration: */
    memset ((void*)ptrChirpCfg, 0, sizeof(rlChirpCfg_t));

    /* Populate the chirp configuration: */
    ptrChirpCfg->chirpStartIdx   = 0;
    ptrChirpCfg->chirpEndIdx     = 0;
    ptrChirpCfg->profileId       = 0;
    ptrChirpCfg->startFreqVar    = 0;
    ptrChirpCfg->freqSlopeVar    = 0;
    ptrChirpCfg->idleTimeVar     = 0;
    ptrChirpCfg->adcStartTimeVar = 0;
    ptrChirpCfg->txEnable        = 1;
}

/**
 *  @b Description
 *  @n
 *      Utility function which populates the BPM chirp configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrBpmChirpCfg
 *      Pointer to the populated BPM chirp configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultBpmChirpCfg (rlBpmChirpCfg_t* ptrBpmChirpCfg)
{
    /* Initialize the configuration: */
    memset ((void*)ptrBpmChirpCfg, 0, sizeof(rlBpmChirpCfg_t));

    ptrBpmChirpCfg->chirpStartIdx   = 0;
    ptrBpmChirpCfg->chirpEndIdx     = 1;
    ptrBpmChirpCfg->constBpmVal     = 2;
}

/**
 *  @b Description
 *  @n
 *      Utility function which populates the default advanced frame configuration
 *
 *  @param[out]  ptrAdvFrameCfg
 *      Pointer to the populated advanced frame configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultAdvancedFrameCfg (rlAdvFrameCfg_t* ptrAdvFrameCfg)
{
    uint8_t numOfSubFrame = 0U;

    /* Initialize the advanced frame configuration: */
    memset ((void*)ptrAdvFrameCfg, 0, sizeof(rlAdvFrameCfg_t));

    ptrAdvFrameCfg->frameSeq.forceProfile  = 0; // 1: force Profile,  0: Don't force profile
    ptrAdvFrameCfg->frameSeq.numFrames     = 0; //infinite
    ptrAdvFrameCfg->frameSeq.triggerSelect = 1; //SW Trigger
    ptrAdvFrameCfg->frameSeq.frameTrigDelay= 0;

    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].forceProfileIdx    = 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numLoops           = 64;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfBurst         = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfBurstLoops    = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].chirpStartIdxOffset= 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfChirps        = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].chirpStartIdx      = 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].subFramePeriodicity= 20200000; // 101 msec
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].burstPeriodicity   = 20000000; // 100 msec

    ptrAdvFrameCfg->frameData.subframeDataCfg[0].numAdcSamples = PROFILE_0_ADC_SAMPLE_VAL*2;
    ptrAdvFrameCfg->frameData.subframeDataCfg[0].totalChirps   = 64;
    ptrAdvFrameCfg->frameData.subframeDataCfg[0].numChirpsInDataPacket = 1;
    numOfSubFrame++;

    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].forceProfileIdx    = 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numLoops           = 32;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numOfBurst         = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numOfBurstLoops    = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].chirpStartIdxOffset= 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numOfChirps        = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].chirpStartIdx      = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].subFramePeriodicity= 20200000; //101 msec
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].burstPeriodicity   = 20000000; //100 msec

    ptrAdvFrameCfg->frameData.subframeDataCfg[1].numAdcSamples = PROFILE_1_ADC_SAMPLE_VAL*2;
    ptrAdvFrameCfg->frameData.subframeDataCfg[1].totalChirps   = 32;
    ptrAdvFrameCfg->frameData.subframeDataCfg[1].numChirpsInDataPacket = 1;
    numOfSubFrame++;

    ptrAdvFrameCfg->frameSeq.numOfSubFrames = numOfSubFrame;
    ptrAdvFrameCfg->frameData.numSubFrames  = numOfSubFrame;

}

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

    /* No custom calibration: */
    ptrOpenCfg->useCustomCalibration        = false;
    ptrOpenCfg->customCalibrationEnableMask = 0x0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default control configuration
 *      in chirp configuration mode
 *
 *  @param[out]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultChirpControlCfg (MMWave_CtrlCfg* ptrCtrlCfg)
{
    rlProfileCfg_t      profileCfg;
    rlChirpCfg_t        chirpCfg;
    int32_t             errCode;
    MMWave_ChirpHandle  chirpHandle;

    /* Initialize the control configuration: */
    memset ((void*)ptrCtrlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* This is frame mode configuration */
    ptrCtrlCfg->dfeDataOutputMode = MMWave_DFEDataOutputMode_FRAME;

    /* Populate the profile configuration: */
    Mmwave_populateDefaultProfileCfg (&profileCfg);

    /* Create the profile: */
    ptrCtrlCfg->u.frameCfg.profileHandle[0] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
    if (ptrCtrlCfg->u.frameCfg.profileHandle[0] == NULL)
    {
        System_printf ("Error: Unable to add the profile [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_PASS);

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration Profile API
     **************************************************************************************************/
    {
        rlProfileCfg_t          profileCfgTmp;
        uint32_t                numProfiles;
        MMWave_ProfileHandle    tmpProfileHandle;

        /* Verify the number of profiles */
        if (MMWave_getNumProfiles (gMMWaveHandle, &numProfiles, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of profiles [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_FAIL);
            return;
        }
        if (numProfiles != 1U)
        {
            System_printf ("Error: Invalid number of profiles detected [%d]\n", numProfiles);
            MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_FAIL);
        }
        MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_PASS);

        /* Get the profile handle: */
        if (MMWave_getProfileHandle (gMMWaveHandle, 0U, &tmpProfileHandle, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_FAIL);
        }
        if (tmpProfileHandle != ptrCtrlCfg->u.frameCfg.profileHandle[0])
        {
            System_printf ("Error: Invalid profile handle detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_FAIL);
        }
        MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_PASS);

        /* Get the profile configuration */
        if (MMWave_getProfileCfg (ptrCtrlCfg->u.frameCfg.profileHandle[0], &profileCfgTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_FAIL);
            return;
        }
        if (memcmp ((void*)&profileCfg, (void*)&profileCfgTmp, sizeof(rlProfileCfg_t)) != 0)
        {
            System_printf ("Error: Invalid profile configuration detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_PASS);
    }

    /* Populate the default chirp configuration */
    Mmwave_populateDefaultChirpCfg (&chirpCfg);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->u.frameCfg.profileHandle[0], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        System_printf ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_PASS);

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration Chirp API
     **************************************************************************************************/
    {
        rlChirpCfg_t        chirpCfgTmp;
        uint32_t            numChirps;
        MMWave_ChirpHandle  chirpHandleTmp;

        /* Get the number of chirps attached to the profile */
        if (MMWave_getNumChirps (ptrCtrlCfg->u.frameCfg.profileHandle[0], &numChirps, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of chirps [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_FAIL);
            return;
        }
        if (numChirps != 1U)
        {
            System_printf ("Error: Invalid number of chirps detected [%d]\n", numChirps);
            MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_PASS);

        /* Get the Chirp Handle */
        if (MMWave_getChirpHandle (ptrCtrlCfg->u.frameCfg.profileHandle[0], 1U, &chirpHandleTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the chirp handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_FAIL);
            return;
        }
        if (chirpHandleTmp != chirpHandle)
        {
            System_printf ("Error: Chirp handle validation failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_PASS);

        /* Get the chirp configuration */
        if (MMWave_getChirpCfg (chirpHandle, &chirpCfgTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp", MCPI_TestResult_FAIL);
            return;
        }
        if (memcmp ((void*)&chirpCfg, (void*)&chirpCfgTmp, sizeof(rlChirpCfg_t)) != 0)
        {
            System_printf ("Error: Invalid chirp configuration detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Chirp Configuration", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Chirp Configuration", MCPI_TestResult_PASS);
    }

    /* Populate the frame configuration: */
    ptrCtrlCfg->u.frameCfg.frameCfg.chirpStartIdx      = 0;
    ptrCtrlCfg->u.frameCfg.frameCfg.chirpEndIdx        = 0;
    ptrCtrlCfg->u.frameCfg.frameCfg.numLoops           = 128;
    ptrCtrlCfg->u.frameCfg.frameCfg.numFrames          = 1;
    ptrCtrlCfg->u.frameCfg.frameCfg.numAdcSamples      = 256;
    ptrCtrlCfg->u.frameCfg.frameCfg.framePeriodicity   = 20 * 1000000 / 5;
    ptrCtrlCfg->u.frameCfg.frameCfg.triggerSelect      = 1;
    ptrCtrlCfg->u.frameCfg.frameCfg.frameTriggerDelay  = 0;

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration BPM Chirp API
     **************************************************************************************************/
    {
        rlBpmChirpCfg_t        bpmChirpCfg;
        rlBpmChirpCfg_t        bpmChirpCfgTmp;
        uint32_t               numBpmChirps;
        MMWave_BpmChirpHandle  bpmChirpHandle;
        MMWave_BpmChirpHandle  bpmChirpHandleTmp;

        /* Populate the default chirp configuration */
        Mmwave_populateDefaultBpmChirpCfg (&bpmChirpCfg);

        /* Add config to the list: */
        bpmChirpHandle = MMWave_addBpmChirp (gMMWaveHandle, &bpmChirpCfg, &errCode);
        if (bpmChirpHandle == NULL)
        {
            System_printf ("Error: Unable to add BPM cfg [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Add BPM cfg", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Add BPM cfg", MCPI_TestResult_PASS);

        /* Get the number of BPM configurations */
        if (MMWave_getNumBpmChirp (gMMWaveHandle, &numBpmChirps, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of BPM cfgs [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Number of BPM cfg", MCPI_TestResult_FAIL);
            return;
        }
        if (numBpmChirps != 1U)
        {
            System_printf ("Error: Invalid number of BPM cfgs detected [%d]\n", numBpmChirps);
            MCPI_setFeatureTestResult ("MMWave Get Number of BPM cfg", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get Number of BPM cfg", MCPI_TestResult_PASS);

        /* Get BPM cfg Handle */
        if (MMWave_getBpmChirpHandle (gMMWaveHandle, 1U, &bpmChirpHandleTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the BPM chirp handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get BPM  Chirp Handle", MCPI_TestResult_FAIL);
            return;
        }
        if (bpmChirpHandleTmp != bpmChirpHandle)
        {
            System_printf ("Error: BPM Chirp handle validation failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Handle", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Handle", MCPI_TestResult_PASS);

        /* Get the chirp configuration */
        if (MMWave_getBpmChirpCfg (bpmChirpHandle, &bpmChirpCfgTmp, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the BPM configuration [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get BPM configuration", MCPI_TestResult_FAIL);
            return;
        }
        if (memcmp ((void*)&bpmChirpCfg, (void*)&bpmChirpCfgTmp, sizeof(rlBpmChirpCfg_t)) != 0)
        {
            System_printf ("Error: Invalid BPM chirp configuration detected\n");
            MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Configuration", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("MMWave Get BPM Chirp Configuration", MCPI_TestResult_PASS);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      Application registered callback function which is invoked after the configuration
 *      has been used to configure the mmWave link and the BSS. This is applicable only for
 *      the XWR16xx. The BSS can be configured only by the MSS *or* DSS. The callback API is
 *      triggered on the remote execution domain (which did not configure the BSS)
 *
 *      This is registered function only while executing in frame mode
 *
 *  @param[in]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_cfgFxn(MMWave_CtrlCfg* ptrCtrlCfg)
{
    rlProfileCfg_t         defaultProfileCfg;
    rlProfileCfg_t         profileCfg;
    rlChirpCfg_t           defaultChirpCfg;
    rlChirpCfg_t           chirpCfg;
    rlBpmChirpCfg_t        defaultBpmChirpCfg;
    rlBpmChirpCfg_t        bpmChirpCfg;
    MMWave_CtrlCfg         defaultCfg;
    uint32_t               numChirps;
    uint32_t               numBpmChirps;
    uint8_t                index;
    int32_t                errCode;
    MMWave_ChirpHandle     chirpHandle;
    MMWave_BpmChirpHandle  bpmChirpHandle;
    uint8_t                failedMatch = 0U;
    bool                   isProfileDetected = false;

    /* Initialize the default configuration: */
    memset ((void*)&defaultCfg, 0, sizeof(MMWave_CtrlCfg));

    /* Get the default configuration(s): This is to ensure that we can compare and verify that we
     * received the correct configurations. */
    defaultCfg.dfeDataOutputMode = MMWave_DFEDataOutputMode_FRAME;
    Mmwave_populateDefaultProfileCfg   (&defaultProfileCfg);
    Mmwave_populateDefaultChirpCfg     (&defaultChirpCfg);
    Mmwave_populateDefaultBpmChirpCfg  (&defaultBpmChirpCfg);

    /* Compare the DFE Output Mode: */
    if (defaultCfg.dfeDataOutputMode != ptrCtrlCfg->dfeDataOutputMode)
    {
        failedMatch = 1;
        System_printf ("Error: Invalid DFE Output mode detected\n");
    }

    /************************************************************************************************
     * Profile & Chirp configurations:
     ************************************************************************************************/
    for (index = 0U; index < MMWAVE_MAX_PROFILE; index++)
    {
        /* Did we get a valid profile? */
        if (ptrCtrlCfg->u.frameCfg.profileHandle[index] == NULL)
        {
            continue;
        }

        /* Get the profile configuration: */
        if (MMWave_getProfileCfg (ptrCtrlCfg->u.frameCfg.profileHandle[index],
                                  &profileCfg, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            return;
        }

        /* Validate the configuration: */
        if (memcmp ((void*)&profileCfg, (void*)&defaultProfileCfg, sizeof(rlProfileCfg_t)) != 0)
        {
            System_printf ("Error: Invalid profile configuration detected\n");
            return;
        }

        /* Get the number of chirps attached to the profile */
        if (MMWave_getNumChirps (ptrCtrlCfg->u.frameCfg.profileHandle[index],
                                 &numChirps, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of chirps [Error code %d]\n", errCode);
            return;
        }
        if (numChirps != 1U)
        {
            System_printf ("Error: Invalid number of chirps detected [%d]\n", numChirps);
            return;
        }

        /* Get the Chirp Handle */
        if (MMWave_getChirpHandle (ptrCtrlCfg->u.frameCfg.profileHandle[index],
                                   1U, &chirpHandle, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the chirp handle [Error code %d]\n", errCode);
            return;
        }

        /* Get the chirp configuration */
        if (MMWave_getChirpCfg (chirpHandle, &chirpCfg, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            return;
        }

        /* Validate the configuration: */
        if (memcmp ((void*)&chirpCfg, (void*)&defaultChirpCfg, sizeof(rlChirpCfg_t)) != 0)
        {
            System_printf ("Error: Invalid chirp configuration detected\n");
            return;
        }

        /* Profile was detected & validated */
        isProfileDetected = true;
    }

    /* We should have detected at least 1 profile. */
    if (isProfileDetected == false)
    {
        System_printf ("Error: No profile/chirp detected\n");
        DebugP_assert (0);
        return;
    }

    /* Check BPM configuration */
    if (MMWave_getNumBpmChirp (gMMWaveHandle, &numBpmChirps, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the number of BPM chirps [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
        return;
    }
    if (numBpmChirps != 1U)
    {
        System_printf ("Error: Invalid number of BPM chirps detected [%d]\n", numBpmChirps);
        MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
        return;
    }

    /* Get the Handle */
    if (MMWave_getBpmChirpHandle (gMMWaveHandle, 1U, &bpmChirpHandle, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the BPM chirp handle [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
        return;
    }

    /* Get the configuration */
    if (MMWave_getBpmChirpCfg (bpmChirpHandle, &bpmChirpCfg, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the BPM configuration [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
        return;
    }

    /* Validate the configuration: */
    if (memcmp ((void*)&bpmChirpCfg, (void*)&defaultBpmChirpCfg, sizeof(rlBpmChirpCfg_t)) != 0)
    {
        System_printf ("Error: Invalid BPM chirp configuration detected\n");
        MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
        return;
    }

    /* Were there any mismatch detected? */
    if (failedMatch == 1)
    {
        MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_PASS);

    /* Configuration validated: Set the global flag */
    gConfigDetected = 1;
    return;
}

/**
 *  @b Description
 *  @n
 *      The function initializes the profile configuration with the default
 *      parameters.
 *
 *  @param[in] profileNum
 *      Profile number to be initialized
 *  @param[out] ptrProfileCfg
 *      Pointer to the profile configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultAdvancedFrameProfileCfg (uint8_t profileNum, rlProfileCfg_t* ptrProfileCfg)
{
    /* Initialize the configuration: */
    memset ((void*)ptrProfileCfg, 0, sizeof(rlProfileCfg_t));

    if (profileNum == 0U)
    {
        /* Populate the default configuration for profile0  */
        ptrProfileCfg->profileId             = PROFILE_0_PROFILE_ID;
        ptrProfileCfg->startFreqConst        = PROFILE_0_START_FREQ_VAL;
        ptrProfileCfg->idleTimeConst         = PROFILE_0_IDLE_TIME_VAL;
        ptrProfileCfg->adcStartTimeConst     = PROFILE_0_ADC_START_TIME_VAL;
        ptrProfileCfg->rampEndTime           = PROFILE_0_RAMP_END_TIME_VAL;
        ptrProfileCfg->txOutPowerBackoffCode = PROFILE_0_TXOUT_POWER_BACKOFF;
        ptrProfileCfg->txPhaseShifter        = PROFILE_0_TXPHASESHIFTER_VAL;
        ptrProfileCfg->freqSlopeConst        = PROFILE_0_FREQ_SLOPE_VAL;
        ptrProfileCfg->txStartTime           = PROFILE_0_TX_START_TIME_VAL;
        ptrProfileCfg->numAdcSamples         = PROFILE_0_ADC_SAMPLE_VAL;
        ptrProfileCfg->digOutSampleRate      = PROFILE_0_DIGOUT_SAMPLERATE_VAL;
        ptrProfileCfg->hpfCornerFreq1        = PROFILE_0_HPFCORNER_FREQ1_VAL;
        ptrProfileCfg->hpfCornerFreq2        = PROFILE_0_HPFCORNER_FREQ2_VAL;
        ptrProfileCfg->rxGain                = PROFILE_0_RX_GAIN_VAL;
    }
    else
    {
        /* Populate the default configuration for profile0  */
        ptrProfileCfg->profileId             = PROFILE_1_PROFILE_ID;
        ptrProfileCfg->startFreqConst        = PROFILE_1_START_FREQ_VAL;
        ptrProfileCfg->idleTimeConst         = PROFILE_1_IDLE_TIME_VAL;
        ptrProfileCfg->adcStartTimeConst     = PROFILE_1_ADC_START_TIME_VAL;
        ptrProfileCfg->rampEndTime           = PROFILE_1_RAMP_END_TIME_VAL;
        ptrProfileCfg->txOutPowerBackoffCode = PROFILE_1_TXOUT_POWER_BACKOFF;
        ptrProfileCfg->txPhaseShifter        = PROFILE_1_TXPHASESHIFTER_VAL;
        ptrProfileCfg->freqSlopeConst        = PROFILE_1_FREQ_SLOPE_VAL;
        ptrProfileCfg->txStartTime           = PROFILE_1_TX_START_TIME_VAL;
        ptrProfileCfg->numAdcSamples         = PROFILE_1_ADC_SAMPLE_VAL;
        ptrProfileCfg->digOutSampleRate      = PROFILE_1_DIGOUT_SAMPLERATE_VAL;
        ptrProfileCfg->hpfCornerFreq1        = PROFILE_1_HPFCORNER_FREQ1_VAL;
        ptrProfileCfg->hpfCornerFreq2        = PROFILE_1_HPFCORNER_FREQ2_VAL;
        ptrProfileCfg->rxGain                = PROFILE_1_RX_GAIN_VAL;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function initializes the chirp configuration with the default
 *      parameters.
 *
 *  @param[out] chirpNum
 *      Chirp Number to be configured
 *  @param[out] ptrChirpCfg
 *      Pointer to the chirp configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultAdvancedFrameChirpCfg (uint8_t chirpNum, rlChirpCfg_t* ptrChirpCfg)
{
    /* Initialize the configuration: */
    memset ((void*)ptrChirpCfg, 0, sizeof(rlChirpCfg_t));

    if (chirpNum == 0U)
    {
        /* Populate the default configuration for chirp 0 */
        ptrChirpCfg->profileId       = CHIRP_0_PROFILE_ID;
        ptrChirpCfg->adcStartTimeVar = CHIRP_0_ADC_START_TIME_VAL;
        ptrChirpCfg->chirpEndIdx     = CHIRP_0_START_INDEX;
        ptrChirpCfg->chirpStartIdx   = CHIRP_0_END_INDEX;
        ptrChirpCfg->idleTimeVar     = CHIRP_0_IDLE_TIME_VAL;
        ptrChirpCfg->txEnable        = CHIRP_1_TX_CHANNEL;
        ptrChirpCfg->startFreqVar    = CHIRP_0_START_FREQ_VAL;
        ptrChirpCfg->freqSlopeVar    = CHIRP_0_FREQ_SLOPE_VAL;
    }
    else
    {
        /* Populate the default configuration for chirp 1 */
        ptrChirpCfg->profileId       = CHIRP_1_PROFILE_ID;
        ptrChirpCfg->adcStartTimeVar = CHIRP_1_ADC_START_TIME_VAL;
        ptrChirpCfg->chirpEndIdx     = CHIRP_1_START_INDEX;
        ptrChirpCfg->chirpStartIdx   = CHIRP_1_END_INDEX;
        ptrChirpCfg->idleTimeVar     = CHIRP_1_IDLE_TIME_VAL;
        ptrChirpCfg->txEnable        = CHIRP_1_TX_CHANNEL;
        ptrChirpCfg->startFreqVar    = CHIRP_1_START_FREQ_VAL;
        ptrChirpCfg->freqSlopeVar    = CHIRP_1_FREQ_SLOPE_VAL;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default control configuration
 *      in chirp configuration mode
 *
 *  @param[out]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultAdvancedControlCfg (MMWave_CtrlCfg* ptrCtrlCfg)
{
    rlProfileCfg_t         profileCfg;
    rlChirpCfg_t           chirpCfg;
    rlBpmChirpCfg_t        bpmChirpCfg;
    int32_t                errCode;
    MMWave_ChirpHandle     chirpHandle;
    MMWave_BpmChirpHandle  bpmChirpHandle;

    /* Initialize the control configuration: */
    memset ((void*)ptrCtrlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* This is frame mode configuration */
    ptrCtrlCfg->dfeDataOutputMode = MMWave_DFEDataOutputMode_ADVANCED_FRAME;

    /* Populate the profile configuration: */
    Mmwave_populateDefaultAdvancedFrameProfileCfg (0U, &profileCfg);

    /* Create the profile: */
    ptrCtrlCfg->u.advancedFrameCfg.profileHandle[0] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
    if (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[0] == NULL)
    {
        System_printf ("Error: Unable to add the profile [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_PASS);

    /* Populate the profile configuration: */
    Mmwave_populateDefaultAdvancedFrameProfileCfg (1U, &profileCfg);

    /* Create the profile: */
    ptrCtrlCfg->u.advancedFrameCfg.profileHandle[1] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
    if (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[1] == NULL)
    {
        System_printf ("Error: Unable to add the profile [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_PASS);

    /* Populate the default chirp configuration */
    Mmwave_populateDefaultAdvancedFrameChirpCfg (0U, &chirpCfg);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[0], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        System_printf ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_PASS);

    /* Populate the default chirp configuration */
    Mmwave_populateDefaultAdvancedFrameChirpCfg (1U, &chirpCfg);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[1], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        System_printf ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_PASS);

    /* Populate the BPM configuration. */
    Mmwave_populateDefaultBpmChirpCfg (&bpmChirpCfg);
    bpmChirpHandle = MMWave_addBpmChirp (gMMWaveHandle, &bpmChirpCfg, &errCode);
    if (bpmChirpHandle == NULL)
    {
        System_printf ("Error: Unable to add BPM chirp [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add BPM Chirp", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Add BPM Chirp", MCPI_TestResult_PASS);

    /* Populate the advanced frame configuration: */
    Mmwave_populateDefaultAdvancedFrameCfg(&ptrCtrlCfg->u.advancedFrameCfg.frameCfg);
    return;
}

/**
 *  @b Description
 *  @n
 *      Application registered callback function which is invoked after the configuration
 *      has been used to configure the mmWave link and the BSS. This is applicable only for
 *      the XWR16xx. The BSS can be configured only by the MSS *or* DSS. The callback API is
 *      triggered on the remote execution domain (which did not configure the BSS)
 *
 *      This is registered function only while executing in advanced frame configuration mode
 *
 *  @param[in]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_advancedFrameCfgFxn (MMWave_CtrlCfg* ptrCtrlCfg)
{
    rlProfileCfg_t         defaultProfileCfg[2];
    rlChirpCfg_t           defaultChirpCfg[2];
    rlBpmChirpCfg_t        defaultBpmChirpCfg;
    rlProfileCfg_t         profileCfg;
    rlChirpCfg_t           chirpCfg;
    rlBpmChirpCfg_t        bpmChirpCfg;
    rlAdvFrameCfg_t        defaultAdvancedFrameCfg;
    uint32_t               numChirps;
    uint32_t               numBpmChirps;
    uint8_t                index;
    int32_t                errCode;
    MMWave_ChirpHandle     chirpHandle;
    MMWave_BpmChirpHandle  bpmChirpHandle;
    uint8_t                failedMatch = 0U;
    bool                   isProfileDetected = false;

    /* Get the default configuration(s): This is to ensure that we can compare and verify that we
     * received the correct configurations. */
    Mmwave_populateDefaultAdvancedFrameProfileCfg (0U, &defaultProfileCfg[0]);
    Mmwave_populateDefaultAdvancedFrameProfileCfg (1U, &defaultProfileCfg[1]);
    Mmwave_populateDefaultAdvancedFrameChirpCfg   (0U, &defaultChirpCfg[0]);
    Mmwave_populateDefaultAdvancedFrameChirpCfg   (1U, &defaultChirpCfg[1]);
    Mmwave_populateDefaultBpmChirpCfg (&defaultBpmChirpCfg);
    Mmwave_populateDefaultAdvancedFrameCfg (&defaultAdvancedFrameCfg);

    /* Compare the DFE Output Mode: */
    if (ptrCtrlCfg->dfeDataOutputMode != MMWave_DFEDataOutputMode_ADVANCED_FRAME)
    {
        failedMatch = 1;
        System_printf ("Error: Invalid DFE Output mode detected\n");
    }

    /************************************************************************************************
     * Profile & Chirp configurations:
     ************************************************************************************************/
    for (index = 0U; index < 2U; index++)
    {
        /* Did we get a valid profile? */
        if (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[index] == NULL)
        {
            System_printf ("Error: No registered profile @ Index %d\n", index);
            return;
        }

        /* Get the profile configuration: */
        if (MMWave_getProfileCfg (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[index],
                                  &profileCfg, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            return;
        }

        /* Validate the configuration: */
        if (memcmp ((void*)&profileCfg, (void*)&defaultProfileCfg[index], sizeof(rlProfileCfg_t)) != 0)
        {
            System_printf ("Error: Invalid profile configuration detected\n");
            return;
        }

        /* Get the number of chirps attached to the profile */
        if (MMWave_getNumChirps (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[index],
                                 &numChirps, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of chirps [Error code %d]\n", errCode);
            return;
        }
        if (numChirps != 1U)
        {
            System_printf ("Error: Invalid number of chirps detected [%d]\n", numChirps);
            return;
        }

        /* Get the Chirp Handle */
        if (MMWave_getChirpHandle (ptrCtrlCfg->u.advancedFrameCfg.profileHandle[index],
                                   1U, &chirpHandle, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the chirp handle [Error code %d]\n", errCode);
            return;
        }

        /* Get the chirp configuration */
        if (MMWave_getChirpCfg (chirpHandle, &chirpCfg, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            return;
        }

        /* Validate the configuration: */
        if (memcmp ((void*)&chirpCfg, (void*)&defaultChirpCfg[index], sizeof(rlChirpCfg_t)) != 0)
        {
            System_printf ("Error: Invalid chirp configuration detected\n");
            return;
        }

        /* Profile was detected & validated */
        isProfileDetected = true;
    }

    /* We should have detected at least 1 profile. */
    if (isProfileDetected == false)
    {
        System_printf ("Error: No profile/chirp detected\n");
        DebugP_assert (0);
        return;
    }

    /* Check BPM configuration */
    /* Get the number of BPM cfgs */
    if (MMWave_getNumBpmChirp (gMMWaveHandle, &numBpmChirps, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the number of BPM chirps [Error code %d]\n", errCode);
        DebugP_assert (0);
        return;
    }
    if (numBpmChirps != 1U)
    {
        System_printf ("Error: Invalid number of BPM chirps detected [%d]\n", numBpmChirps);
        DebugP_assert (0);
        return;
    }

    /* Get the Handle */
    if (MMWave_getBpmChirpHandle (gMMWaveHandle, 1U, &bpmChirpHandle, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the BPM chirp handle [Error code %d]\n", errCode);
        DebugP_assert (0);
        return;
    }

    /* Get the BPM configuration */
    if (MMWave_getBpmChirpCfg (bpmChirpHandle, &bpmChirpCfg, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the BPM configuration [Error code %d]\n", errCode);
        DebugP_assert (0);
        return;
    }

    /* Validate the configuration: */
    if (memcmp ((void*)&bpmChirpCfg, (void*)&defaultBpmChirpCfg, sizeof(rlBpmChirpCfg_t)) != 0)
    {
        System_printf ("Error: Invalid BPM chirp configuration detected\n");
        DebugP_assert (0);
        return;
    }

    /* Sanity Check: Validate the advanced frame configuration */
    if (memcmp ((void*)&ptrCtrlCfg->u.advancedFrameCfg.frameCfg, (void*)&defaultAdvancedFrameCfg, sizeof(rlAdvFrameCfg_t) != 0))
    {
        System_printf ("Error: Invalid advanced frame configuration detected\n");
        DebugP_assert (0);
        return;
    }

    /* Were there any mismatch detected? */
    if (failedMatch == 1)
    {
        MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave Configuration Validation", MCPI_TestResult_PASS);

    /* Configuration validated: Set the global flag */
    gConfigDetected = 1;
    return;
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

    /* Dont do the validations for the Save/Restore Calibration Tests? This is
     * because in these tests we dont use the well known default open configuration
     * so the validations cannot be done: */
    if ((gTestSelection != 8U) && (gTestSelection != 9U))
    {
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
    }
    else
    {
        System_printf ("Debug: Skipping open configuration validation for Save/Restore Calibration Tests\n");
    }

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
 *      The function is used to populate the default calibration
 *      configuration which is passed to start the mmWave module
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultCalibrationCfg (MMWave_CalibrationCfg* ptrCalibrationCfg, MMWave_DFEDataOutputMode dfeOutputMode)
{
    /* Populate the calibration configuration: */
    ptrCalibrationCfg->dfeDataOutputMode                          = dfeOutputMode;
    ptrCalibrationCfg->u.chirpCalibrationCfg.enableCalibration    = true;
    ptrCalibrationCfg->u.chirpCalibrationCfg.enablePeriodicity    = true;
    ptrCalibrationCfg->u.chirpCalibrationCfg.periodicTimeInFrames = 10U;
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
void Mmwave_startFxn (MMWave_CalibrationCfg* ptrCalibrationCfg)
{
    MMWave_CalibrationCfg   defaultCalibrationCfg;

    /* Populate the default configuration: */
    Mmwave_populateDefaultCalibrationCfg(&defaultCalibrationCfg, ptrCalibrationCfg->dfeDataOutputMode);

    /* Compare the default with the received configuration: */
    if (memcmp ((void *)&defaultCalibrationCfg, (void *)ptrCalibrationCfg, sizeof(MMWave_CalibrationCfg)) != 0)
    {
        /* Error: There is a mismatch in the open configuration */
        System_printf ("Error: Mismatch in the calibration configuration\n");
        MCPI_setFeatureTestResult ("MMWave Calibration Configuration Validation", MCPI_TestResult_FAIL);
        DebugP_assert (0);
    }
    MCPI_setFeatureTestResult ("MMWave Calibration Configuration Validation", MCPI_TestResult_PASS);

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


/**
 *  @b Description
 *  @n
 *      The function is used to test the mmWave Full configuration API
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t Test_mmWaveFullAPI (void)
{
    rlProfileCfg_t          profileCfg;
    rlChirpCfg_t            chirpCfg;
    rlBpmChirpCfg_t         bpmChirpCfg;
    int32_t                 errCode;
    uint32_t                profileIndex;
    uint32_t                chirpIndex;
    uint32_t                bpmChirpIndex;
    uint32_t                numChirps;
    uint32_t                numBpmChirps;
    MMWave_ProfileHandle    tmpProfileHandle;
    MMWave_ChirpHandle      tmpChirpHandle;
    MMWave_BpmChirpHandle   tmpBpmChirpHandle;
    MMWave_ProfileHandle    profileHandle[TEST_MAX_PROFILE];
    MMWave_ChirpHandle      chirpHandle[TEST_MAX_PROFILE][TEST_MAX_CHIRP];
    MMWave_BpmChirpHandle   bpmChirpHandle[TEST_MAX_BPM_CHIRP];
    Memory_Stats            startMemoryStats;
    Memory_Stats            endMemoryStats;

    /* Get the heap statistics at the beginning of the tests */
    HeapMem_getStats (heap0, &startMemoryStats);
    System_printf ("Starting Total Heap Size = %d bytes\n", startMemoryStats.totalSize);
    System_printf ("Starting Total Free Size = %d bytes\n", startMemoryStats.totalFreeSize);

    /**********************************************************************************
     * Unit Test: Profile Addition
     **********************************************************************************/
    for (profileIndex = 0U; profileIndex < TEST_MAX_PROFILE; profileIndex++)
    {
        /* Initialize the profile configuration: */
        memset ((void*)&profileCfg, 0, sizeof(rlProfileCfg_t));

        /* Populate the *default* profile configuration: Override the profile identifier */
        Mmwave_populateDefaultProfileCfg (&profileCfg);
        profileCfg.profileId = profileIndex;

        /* Add the profile: */
        profileHandle[profileIndex] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
        if (profileHandle [profileIndex] == NULL)
        {
            System_printf ("Error: Unable to add the profile index %d [Error code %d]\n", profileIndex, errCode);
            MCPI_setFeatureTestResult ("Profile Addition", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    MCPI_setFeatureTestResult ("Profile Addition", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Get the number of profiles
     **********************************************************************************/
    profileIndex = 0U;
    if (MMWave_getNumProfiles(gMMWaveHandle, &profileIndex, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the number of profiles [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("Number of Profiles", MCPI_TestResult_FAIL);
        return -1;
    }
    if (profileIndex != TEST_MAX_PROFILE)
    {
        System_printf ("Error: Invalid number of profiles detected [%d]\n", profileIndex);
        MCPI_setFeatureTestResult ("Number of Profiles", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("Number of Profiles", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Get the profile handle
     **********************************************************************************/
    for (profileIndex = 0U; profileIndex < TEST_MAX_PROFILE; profileIndex++)
    {
        /* Get the profile handle: */
        if (MMWave_getProfileHandle (gMMWaveHandle, profileIndex, &tmpProfileHandle, &errCode) < 0)
        {
            System_printf ("Error: Unable to get profile handle for index %d [Error code %d]\n", profileIndex, errCode);
            MCPI_setFeatureTestResult ("Profile Handle", MCPI_TestResult_FAIL);
            return -1;
        }
        if (tmpProfileHandle != profileHandle[profileIndex])
        {
            System_printf ("Error: Invalid profile handle detected [%x]\n", tmpProfileHandle);
            MCPI_setFeatureTestResult ("Profile Handle", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    MCPI_setFeatureTestResult ("Profile Handle", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Chirp Addition
     **********************************************************************************/
    numChirps = 0U;
    for (profileIndex = 0U; profileIndex < TEST_MAX_PROFILE; profileIndex++)
    {
        /* For each profile create the chirps */
        for (chirpIndex = 0U; chirpIndex < TEST_MAX_CHIRP; chirpIndex++)
        {
            /* Initialize the chirp configuration: */
            memset ((void*)&chirpCfg, 0, sizeof(rlChirpCfg_t));

            /* Populate the *default* chirp configuration: */
            Mmwave_populateDefaultChirpCfg(&chirpCfg);

            /* Override the chirp configuration: */
            chirpCfg.chirpStartIdx   = numChirps;
            chirpCfg.chirpEndIdx     = numChirps;
            chirpCfg.profileId       = profileIndex;

            /* Add the chirp: */
            chirpHandle[profileIndex][chirpIndex] = MMWave_addChirp (profileHandle[profileIndex], &chirpCfg, &errCode);
            if (chirpHandle[profileIndex][chirpIndex] == NULL)
            {
                System_printf ("Error: Unable to add the chirp index %d profile index %d [Error code %d]\n", chirpIndex, profileIndex, errCode);
                MCPI_setFeatureTestResult ("Chirp Addition", MCPI_TestResult_FAIL);
                return -1;
            }

            /* Increment the number of chirps which have been created */
            numChirps = numChirps + 1U;
        }
    }
    MCPI_setFeatureTestResult ("Chirp Addition", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Number of chirps
     **********************************************************************************/
    for (profileIndex = 0U; profileIndex < TEST_MAX_PROFILE; profileIndex++)
    {
        /* Initialize the number of chirps: */
        numChirps = 0U;

        /* Get the number of chirps: */
        if (MMWave_getNumChirps (profileHandle[profileIndex], &numChirps, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the number of chirps [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("Number of Chirps", MCPI_TestResult_FAIL);
            return -1;
        }
        if (numChirps != TEST_MAX_CHIRP)
        {
            System_printf ("Error: Invalid number of chirps detected [%d]\n", numChirps);
            MCPI_setFeatureTestResult ("Number of Chirps", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    MCPI_setFeatureTestResult ("Number of Chirps", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Get the chirp handle
     **********************************************************************************/
    numChirps = 0U;
    for (profileIndex = 0U; profileIndex < TEST_MAX_PROFILE; profileIndex++)
    {
        for (chirpIndex = 0U; chirpIndex < TEST_MAX_CHIRP; chirpIndex++)
        {
            /* Get the chirp handle: Chirp Index is set to 1 for the first chirp */
            if (MMWave_getChirpHandle (profileHandle[profileIndex], (chirpIndex+1), &tmpChirpHandle, &errCode) < 0)
            {
                System_printf ("Error: Unable to get the chirp handle [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult ("Chirp Handle", MCPI_TestResult_FAIL);
                return -1;
            }
            if (tmpChirpHandle != chirpHandle[profileIndex][chirpIndex])
            {
                System_printf ("Error: Invalid chirp handle detected [%x]\n", tmpChirpHandle);
                MCPI_setFeatureTestResult ("Chirp Handle", MCPI_TestResult_FAIL);
                return -1;
            }
            numChirps++;
        }
    }
    MCPI_setFeatureTestResult ("Chirp Handle", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: BPM Chirp Addition
     **********************************************************************************/
    numBpmChirps = 0U;
    /* Add BPM configurations */
    for (bpmChirpIndex = 0U; bpmChirpIndex < TEST_MAX_BPM_CHIRP; bpmChirpIndex++)
    {
        /* Initialize the configuration: */
        memset ((void*)&bpmChirpCfg, 0, sizeof(rlBpmChirpCfg_t));

        /* Populate the *default* configuration: */
        Mmwave_populateDefaultBpmChirpCfg(&bpmChirpCfg);

        /* Override the configuration: */
        bpmChirpCfg.chirpStartIdx   = numBpmChirps;
        bpmChirpCfg.chirpEndIdx     = numBpmChirps;
        bpmChirpCfg.constBpmVal     = numBpmChirps%64;

        /* Add BPM configuration: */
        bpmChirpHandle[bpmChirpIndex] = MMWave_addBpmChirp (gMMWaveHandle, &bpmChirpCfg, &errCode);
        if (bpmChirpHandle[bpmChirpIndex] == NULL)
        {
            System_printf ("Error: Unable to add the BPM config index %d [Error code %d]\n", bpmChirpIndex, errCode);
            MCPI_setFeatureTestResult ("BPM chirp Addition", MCPI_TestResult_FAIL);
            return -1;
        }

        /* Increment the number of chirps which have been created */
        numBpmChirps = numBpmChirps + 1U;
    }
    MCPI_setFeatureTestResult ("BPM Chirp Addition", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Number of BPM chirps
     **********************************************************************************/
    /* Initialize the number of chirps: */
    numBpmChirps = 0U;

    /* Get the number of BPM chirps: */
    if (MMWave_getNumBpmChirp (gMMWaveHandle, &numBpmChirps, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the number of BPM chirps [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("Number of BPM Chirps", MCPI_TestResult_FAIL);
        return -1;
    }
    if (numBpmChirps != TEST_MAX_BPM_CHIRP)
    {
        System_printf ("Error: Invalid number of BPM chirps detected [%d]\n", numBpmChirps);
        MCPI_setFeatureTestResult ("Number of BPM Chirps", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("Number of BPM Chirps", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Get the BPM chirp handle
     **********************************************************************************/
    numBpmChirps = 0U;
    for (bpmChirpIndex = 0U; bpmChirpIndex < TEST_MAX_BPM_CHIRP; bpmChirpIndex++)
    {
        /* Get the BPM chirp handle: Index is set to 1 for the first BPM configuration */
        if (MMWave_getBpmChirpHandle (gMMWaveHandle, (bpmChirpIndex+1), &tmpBpmChirpHandle, &errCode) < 0)
        {
            System_printf ("Error: Unable to get the BPM chirp handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("BPM Chirp Handle", MCPI_TestResult_FAIL);
            return -1;
        }
        if (tmpBpmChirpHandle != bpmChirpHandle[bpmChirpIndex])
        {
            System_printf ("Error: Invalid BPM chirp handle detected [%x]\n", tmpBpmChirpHandle);
            MCPI_setFeatureTestResult ("BPM Chirp Handle", MCPI_TestResult_FAIL);
            return -1;
        }

        numBpmChirps++;
    }
    MCPI_setFeatureTestResult ("BPM Chirp Handle", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Display the memory consumption once all the chirps and profiles have
     * been configured and added.
     **********************************************************************************/
    {
        Memory_Stats    stats;

        /* Get the memory statistics: */
        HeapMem_getStats (heap0, &stats);
        System_printf ("Debug: %d Profile with %d chirps per profile. %d BPM chirp configurations. Total Free Size = %d bytes Memory Consumed is = %d bytes\n",
                       TEST_MAX_PROFILE, TEST_MAX_CHIRP, TEST_MAX_BPM_CHIRP,
                       stats.totalFreeSize, (startMemoryStats.totalFreeSize - stats.totalFreeSize));
    }

    /**********************************************************************************
     * Unit Test: Deleting the first chirp of each profile
     **********************************************************************************/
    for (profileIndex = 0U; profileIndex < TEST_MAX_PROFILE; profileIndex++)
    {
        if (MMWave_delChirp (profileHandle[profileIndex], chirpHandle[profileIndex][0], &errCode) < 0)
        {
            System_printf ("Error: Unable to delete the chirp [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("Delete Chirp", MCPI_TestResult_FAIL);
            return -1;
        }
    }
    MCPI_setFeatureTestResult ("Delete Chirp", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Deleting the first profile
     **********************************************************************************/
    if (MMWave_delProfile (gMMWaveHandle, profileHandle[0], &errCode) < 0)
    {
        System_printf ("Error: Unable to delete the profile [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("Delete Profile", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("Delete Profile", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Deleting the first BPM configuration
     **********************************************************************************/
    if (MMWave_delBpmChirp (gMMWaveHandle, bpmChirpHandle[0], &errCode) < 0)
    {
        System_printf ("Error: Unable to delete BPM configuration [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("Delete BPM configuration", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("Delete BPM configuration", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Flush configuration
     *
     * *NOTE*: Once the configuration has been flushed all the profile/chirp handles
     * are no longer valid.
     **********************************************************************************/
    if (MMWave_flushCfg (gMMWaveHandle, &errCode) < 0)
    {
        System_printf ("Error: Unable to flush the configuration [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("Flush configuration", MCPI_TestResult_FAIL);
        return -1;
    }

    /* Get the number of profiles: */
    profileIndex = TEST_MAX_PROFILE;
    if (MMWave_getNumProfiles(gMMWaveHandle, &profileIndex, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the number of profiles [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("Flush configuration", MCPI_TestResult_FAIL);
        return -1;
    }
    if (profileIndex != 0U)
    {
        System_printf ("Error: Invalid number of profiles detected [%d]\n", profileIndex);
        MCPI_setFeatureTestResult ("Flush configuration", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("Flush configuration", MCPI_TestResult_PASS);

    /* Get the number of BPM configurations: */
    bpmChirpIndex = TEST_MAX_BPM_CHIRP;
    if (MMWave_getNumBpmChirp(gMMWaveHandle, &bpmChirpIndex, &errCode) < 0)
    {
        System_printf ("Error: Unable to get the number of BPM chirp configurations [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("Flush configuration", MCPI_TestResult_FAIL);
        return -1;
    }
    if (bpmChirpIndex != 0U)
    {
        System_printf ("Error: Invalid number of BPM chirp configuration detected [%d]\n", bpmChirpIndex);
        MCPI_setFeatureTestResult ("Flush configuration", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("Flush configuration", MCPI_TestResult_PASS);

    /**********************************************************************************
     * Unit Test: Memory Leak Testing
     **********************************************************************************/
    HeapMem_getStats (heap0, &endMemoryStats);
    System_printf ("Ending Total Heap Size = %d bytes\n", endMemoryStats.totalSize);
    System_printf ("Ending Total Free Size = %d bytes\n", endMemoryStats.totalFreeSize);
    if (startMemoryStats.totalSize != endMemoryStats.totalSize)
    {
        System_printf ("Error: Mismatch in total size detected [%d %d]\n", startMemoryStats.totalSize, endMemoryStats.totalSize);
        MCPI_setFeatureTestResult ("Memory Leak Tests", MCPI_TestResult_FAIL);
        return -1;
    }
    if (startMemoryStats.totalFreeSize != endMemoryStats.totalFreeSize)
    {
        System_printf ("Error: Mismatch in total free size detected [%d %d]\n", startMemoryStats.totalFreeSize, endMemoryStats.totalFreeSize);
        MCPI_setFeatureTestResult ("Memory Leak Tests", MCPI_TestResult_FAIL);
        return -1;
    }
    MCPI_setFeatureTestResult ("Memory Leak Tests", MCPI_TestResult_PASS);

    return 0;
}
