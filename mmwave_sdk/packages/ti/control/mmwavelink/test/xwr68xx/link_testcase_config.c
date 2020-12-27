/* ======================================================================
 *   Copyright (C) 2015 Texas Instruments Incorporated
 *
 *   All rights reserved. Property of Texas Instruments Incorporated.
 *   Restricted rights to use, duplicate or disclose this code are
 *   granted through contract.
 *
 *   The program may not be used without the written permission
 *   of Texas Instruments Incorporated or against the terms and conditions
 *   stipulated in the agreement under which this program has been
 *   supplied.
 * ==================================================================== */

/**
 *  \file link_testcase_config.c
 *
 *  \brief This file defines the device configurations like driver config etc...
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
 
/* Standard Include Files */
#include <stdio.h>
#include <stdbool.h>

/* mmWave SK Include File */
#include <ti/control/mmwavelink/mmwavelink.h>

/* Link Test Include File */
#include <ti/control/mmwavelink/test/common/link_testcase_setting.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
 
/* These variables are device variant. So don't change */

/* This varaible affects TX2 monitoring */
bool gLinkEnableTx2 = true;
bool gLinkDig1TempSensor = false;

/**************************************************************************
 *************************** Global Structures*****************************
 **************************************************************************/
 
const rlChanCfg_t chCfg  =
{
    .rxChannelEn = RX_CHANNEL_1_2_3_4_ENABLE,
    .txChannelEn = TX_CHANNEL_1_2_3_ENABLE,
    .cascading   = 0x0,
    .cascadingPinoutCfg   = 0x0,
};

const rlAdcOutCfg_t adcOutCfgArgs =
{
    .fmt.b2AdcBits = ADC_BITS_16,
    .fmt.b2AdcOutFmt = ADC_FORMAT_CPMLEX_WITH_IMG_BAND,
    .fmt.b8FullScaleReducFctr = 0U,
    .reserved0  = 0x0,
    .reserved1  = 0x0,
};

const rlLowPowerModeCfg_t lowPowerModeCfg =
{
    .reserved  = 0x0,
    .lpAdcMode = LP_ADC_MODE_LOW_POWER,
};

const rlDevHsiClk_t deviceHsiClk =
{
    .hsiClk = 9,
    .reserved = 0x0,
};


const rlProfileCfg_t  profileCfg[4U] =
{
    {
        .profileId = CHIRP_PROFILE_ID,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL_60GHz,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL_60GHz,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+1,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL_60GHz,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL_60GHz,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+2,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL_60GHz,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL_60GHz,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+3,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL_60GHz,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL_60GHz,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    }
};

const rlChirpCfg_t  chirpCfg[2] =
{
    {
        .chirpStartIdx = CHIRP_START_INDEX,
        .chirpEndIdx = CHIRP_END_INDEX + 255U,
        .profileId = CHIRP_PROFILE_ID,
        .reserved = 0x0,
        .startFreqVar = CHIRP_START_FREQ_VAL,
        .freqSlopeVar= CHIRP_FREQ_SLOPE_VAL,
        .idleTimeVar = CHIRP_IDLE_TIME_VAL,
        .adcStartTimeVar = CHIRP_ADC_START_TIME_VAL,
        .txEnable = CHIRP_TX_1_CHANNEL_ENABLE
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX + 256U,
        .chirpEndIdx = CHIRP_END_INDEX + 511U,
        .profileId = CHIRP_PROFILE_ID + 1U,
        .reserved = 0x0,
        .startFreqVar = CHIRP_START_FREQ_VAL,
        .freqSlopeVar= CHIRP_FREQ_SLOPE_VAL,
        .idleTimeVar = CHIRP_IDLE_TIME_VAL,
        .adcStartTimeVar = CHIRP_ADC_START_TIME_VAL,
        .txEnable = CHIRP_TX_2_CHANNEL_ENABLE
    }
};

const rlDynChirpCfg_t dynChirpCfg[10] =
{
    {
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 0,
        .programMode    = 0,
        .chirpRow { {0x00000003, 0x00700000, 0x00000000},
                    {0x00000103, 0x00700001, 0x00000001},
                    {0x00000203, 0x00700002, 0x00000002},
                    {0x00000303, 0x00700003, 0x00000003},
                    {0x00000403, 0x00700004, 0x00000004},
                    {0x00000503, 0x00700005, 0x00000005},
                    {0x00000603, 0x00700006, 0x00000006},
                    {0x00000703, 0x00700007, 0x00000007},
                    {0x00000803, 0x00700008, 0x00000008},
                    {0x00000903, 0x00700009, 0x00000009},
                    {0x00000A03, 0x0070000A, 0x0000000A},
                    {0x00000B03, 0x0070000B, 0x0000000B},
                    {0x00000C03, 0x0070000C, 0x0000000C},
                    {0x00000D03, 0x0070000D, 0x0000000D},
                    {0x00000E03, 0x0070000E, 0x0000000E},
                    {0x00000F03, 0x0070000F, 0x0000000F} }
    },
    {
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 1,
        .programMode    = 0,
        .chirpRow { {0x00001003, 0x00700010, 0x00000010},
                    {0x00001103, 0x00700011, 0x00000011},
                    {0x00001203, 0x00700012, 0x00000012},
                    {0x00001303, 0x00700013, 0x00000013},
                    {0x00001403, 0x00700014, 0x00000014},
                    {0x00001503, 0x00700015, 0x00000015},
                    {0x00001603, 0x00700016, 0x00000016},
                    {0x00001703, 0x00700017, 0x00000017},
                    {0x00001803, 0x00700018, 0x00000018},
                    {0x00001903, 0x00700019, 0x00000019},
                    {0x00001A03, 0x0070001A, 0x0000001A},
                    {0x00001B03, 0x0070001B, 0x0000001B},
                    {0x00001C03, 0x0070001C, 0x0000001C},
                    {0x00001D03, 0x0070001D, 0x0000001D},
                    {0x00001E03, 0x0070001E, 0x0000001E},
                    {0x00001F03, 0x0070001F, 0x0000001F} }
    },
    {
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 2,
        .programMode    = 0,
        .chirpRow { {0x00002003, 0x00700020, 0x00000020},
                    {0x00002103, 0x00700021, 0x00000021},
                    {0x00002203, 0x00700022, 0x00000022},
                    {0x00002303, 0x00700023, 0x00000023},
                    {0x00002403, 0x00700024, 0x00000024},
                    {0x00002503, 0x00700025, 0x00000025},
                    {0x00002603, 0x00700026, 0x00000026},
                    {0x00002703, 0x00700027, 0x00000027},
                    {0x00002803, 0x00700028, 0x00000028},
                    {0x00002903, 0x00700029, 0x00000029},
                    {0x00002A03, 0x0070002A, 0x0000002A},
                    {0x00002B03, 0x0070002B, 0x0000002B},
                    {0x00002C03, 0x0070002C, 0x0000002C},
                    {0x00002D03, 0x0070002D, 0x0000002D},
                    {0x00002E03, 0x0070002E, 0x0000002E},
                    {0x00002F03, 0x0070002F, 0x0000002F} }
    },
    {
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 3,
        .programMode    = 0,
        .chirpRow { {0x00003003, 0x00700030, 0x00000030},
                    {0x00003103, 0x00700031, 0x00000031},
                    {0x00003203, 0x00700032, 0x00000032},
                    {0x00003303, 0x00700033, 0x00000033},
                    {0x00003403, 0x00700034, 0x00000034},
                    {0x00003503, 0x00700035, 0x00000035},
                    {0x00003603, 0x00700036, 0x00000036},
                    {0x00003703, 0x00700037, 0x00000037},
                    {0x00003803, 0x00700038, 0x00000038},
                    {0x00003903, 0x00700039, 0x00000039},
                    {0x00003A03, 0x0070003A, 0x0000003A},
                    {0x00003B03, 0x0070003B, 0x0000003B},
                    {0x00003C03, 0x0070003C, 0x0000003C},
                    {0x00003003, 0x0070003D, 0x0000003D},
                    {0x00003003, 0x0070003E, 0x0000003E},
                    {0x00003003, 0x0070003F, 0x0000003F} }
    },
    {
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 4,
        .programMode    = 0,
        .chirpRow { {0x00000003, 0x00700040, 0x00000040},
                    {0x00000103, 0x00700041, 0x00000041},
                    {0x00000203, 0x00700042, 0x00000042},
                    {0x00000303, 0x00700043, 0x00000043},
                    {0x00000403, 0x00700044, 0x00000044},
                    {0x00000503, 0x00700045, 0x00000045},
                    {0x00000603, 0x00700046, 0x00000046},
                    {0x00000703, 0x00700047, 0x00000047},
                    {0x00000803, 0x00700048, 0x00000048},
                    {0x00000903, 0x00700049, 0x00000049},
                    {0x00000A03, 0x0070004A, 0x0000004A},
                    {0x00000B03, 0x0070004B, 0x0000004B},
                    {0x00000C03, 0x0070004C, 0x0000004C},
                    {0x00000D03, 0x0070004D, 0x0000004D},
                    {0x00000E03, 0x0070004E, 0x0000004E},
                    {0x00000F03, 0x0070004F, 0x0000004F} }
    },
    {
                    
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 5,
        .programMode    = 0,
        .chirpRow { {0x00001003, 0x00700050, 0x00000050},
                    {0x00001103, 0x00700051, 0x00000051},
                    {0x00001203, 0x00700052, 0x00000052},
                    {0x00001303, 0x00700053, 0x00000053},
                    {0x00001403, 0x00700054, 0x00000054},
                    {0x00001503, 0x00700055, 0x00000055},
                    {0x00001603, 0x00700056, 0x00000056},
                    {0x00001703, 0x00700057, 0x00000057},
                    {0x00001803, 0x00700058, 0x00000058},
                    {0x00001903, 0x00700059, 0x00000059},
                    {0x00001A03, 0x0070005A, 0x0000005A},
                    {0x00001B03, 0x0070005B, 0x0000005B},
                    {0x00001C03, 0x0070005C, 0x0000005C},
                    {0x00001D03, 0x0070005D, 0x0000005D},
                    {0x00001E03, 0x0070005E, 0x0000005E},
                    {0x00001F03, 0x0070005F, 0x0000005F} }
    },
    {
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 6,
        .programMode    = 0,
        .chirpRow { {0x00002003, 0x00700060, 0x00000060},
                    {0x00002103, 0x00700061, 0x00000061},
                    {0x00002203, 0x00700062, 0x00000062},
                    {0x00002303, 0x00700063, 0x00000063},
                    {0x00002403, 0x00700064, 0x00000064},
                    {0x00002503, 0x00700065, 0x00000065},
                    {0x00002603, 0x00700066, 0x00000066},
                    {0x00002703, 0x00700067, 0x00000067},
                    {0x00002803, 0x00700068, 0x00000068},
                    {0x00002903, 0x00700069, 0x00000069},
                    {0x00002A03, 0x0070006A, 0x0000006A},
                    {0x00002B03, 0x0070006B, 0x0000006B},
                    {0x00002C03, 0x0070006C, 0x0000006C},
                    {0x00002D03, 0x0070006D, 0x0000006D},
                    {0x00002E03, 0x0070006E, 0x0000006E},
                    {0x00002F03, 0x0070006F, 0x0000006F} }
    },
    {        
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 7,
        .programMode    = 0,
        .chirpRow { {0x00003003, 0x00700070, 0x00000070},
                    {0x00003103, 0x00700071, 0x00000071},
                    {0x00003203, 0x00700072, 0x00000072},
                    {0x00003303, 0x00700073, 0x00000073},
                    {0x00003403, 0x00700074, 0x00000074},
                    {0x00003503, 0x00700075, 0x00000075},
                    {0x00003603, 0x00700076, 0x00000076},
                    {0x00003703, 0x00700077, 0x00000077},
                    {0x00003803, 0x00700078, 0x00000078},
                    {0x00003903, 0x00700079, 0x00000079},
                    {0x00003A03, 0x0070007A, 0x0000007A},
                    {0x00003B03, 0x0070007B, 0x0000007B},
                    {0x00003C03, 0x0070007C, 0x0000007C},
                    {0x00003003, 0x0070007D, 0x0000007D},
                    {0x00003003, 0x0070007E, 0x0000007E},
                    {0x00003003, 0x0070007F, 0x0000007F} }
    },
    {
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 8,
        .programMode    = 0,
        .chirpRow { {0x00000003, 0x00700080, 0x00000080},
                    {0x00000103, 0x00700081, 0x00000081},
                    {0x00000203, 0x00700082, 0x00000082},
                    {0x00000303, 0x00700083, 0x00000083},
                    {0x00000403, 0x00700084, 0x00000084},
                    {0x00000503, 0x00700085, 0x00000085},
                    {0x00000603, 0x00700086, 0x00000086},
                    {0x00000703, 0x00700087, 0x00000087},
                    {0x00000803, 0x00700088, 0x00000088},
                    {0x00000903, 0x00700089, 0x00000089},
                    {0x00000A03, 0x0070008A, 0x0000008A},
                    {0x00000B03, 0x0070008B, 0x0000008B},
                    {0x00000C03, 0x0070008C, 0x0000008C},
                    {0x00000D03, 0x0070008D, 0x0000008D},
                    {0x00000E03, 0x0070008E, 0x0000008E},
                    {0x00000F03, 0x0070008F, 0x0000008F} }
    },
    {
                    
        .chirpRowSelect = 0x00,
        .chirpSegSel    = 9,
        .programMode    = 0,
        .chirpRow { {0x00001003, 0x00700090, 0x00000090},
                    {0x00001103, 0x00700091, 0x00000091},
                    {0x00001203, 0x00700092, 0x00000092},
                    {0x00001303, 0x00700093, 0x00000093},
                    {0x00001403, 0x00700094, 0x00000094},
                    {0x00001503, 0x00700095, 0x00000095},
                    {0x00001603, 0x00700096, 0x00000096},
                    {0x00001703, 0x00700097, 0x00000097},
                    {0x00001803, 0x00700098, 0x00000098},
                    {0x00001903, 0x00700099, 0x00000099},
                    {0x00001A03, 0x0070009A, 0x0000009A},
                    {0x00001B03, 0x0070009B, 0x0000009B},
                    {0x00001C03, 0x0070009C, 0x0000009C},
                    {0x00001D03, 0x0070009D, 0x0000009D},
                    {0x00001E03, 0x0070009E, 0x0000009E},
                    {0x00001F03, 0x0070009F, 0x0000009F} }
    },
};


const rlRxSatMonConf_t rxSatMonCfg =
{
    .profileIndx   = 0U,
    .satMonSel     = 3U,
    .reserved0     = 0U,
    .primarySliceDuration = 5U,
    .numSlices     = CQ_NUM_SLICES,
    .rxChannelMask = 0U,
    .reserved1     = 0U,
    .reserved2     = 0U,
    .reserved3     = 0U,
    .reserved4     = 0U
};

const rlSigImgMonConf_t sigImgMonCfg =
{
    .profileIndx = 0U,
    .numSlices   = CQ_NUM_SLICES,
    .timeSliceNumSamples = 8U,
    .reserved0   = 0U,
    .reserved1   = 0U
};

rlFrameCfg_t  frameCfg =
{
    .reserved0 = 0x0,
    .chirpStartIdx = FRAME_CHIRP_START_IDX,
    .chirpEndIdx = FRAME_CHIRP_END_IDX,
    .numLoops = FRAME_LOOP_COUNT,
    .numFrames = FRAME_COUNT_VAL,
    .numAdcSamples = FRAME_ADC_SAMPLE_NUM,
    .framePeriodicity = FRAME_PRIODICITY_VAL,
    .triggerSelect = RL_FRAMESTRT_API_TRIGGER,
    .reserved1 = 0x0,
    .frameTriggerDelay = FRAME_TRIGGER_DELAY_VAL,
};


const rlRfCalMonTimeUntConf_t calMonTimeUnitConf =
{
    .calibMonTimeUnit = 1,
    .numOfCascadeDev = 1,
    .devId = 1,
    .reserved = 0
};

const rlRfCalMonFreqLimitConf_t freqLimit =
{
    .freqLimitLow = 760,
    .freqLimitHigh = 810,
    .reserved0 = 0,
    .reserved1 = 0
};

const rlRfInitCalConf_t rfInitCalib =
{
    .calibEnMask = 0x010,
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0
};

const rlRunTimeCalibConf_t runTimeCalib =
{
    .oneTimeCalibEnMask = 0x010,
    .periodicCalibEnMask = 0x010,
    .calibPeriodicity = 1,
    .reportEn = 1,
    .reserved0 = 0,
    .reserved1 = 0
};

rlAdvFrameCfg_t advFrameCfg =
{
        .frameSeq.numOfSubFrames = 4,
        .frameSeq.forceProfile = 0,
        .frameSeq.loopBackCfg = 0,
        .frameSeq.subFrameTrigger = 0,

        .frameSeq.subFrameCfg[0].forceProfileIdx = 0,
        .frameSeq.subFrameCfg[0].chirpStartIdx = 0,
        .frameSeq.subFrameCfg[0].numOfChirps = 64,
        .frameSeq.subFrameCfg[0].numLoops = 1,
        .frameSeq.subFrameCfg[0].burstPeriodicity = 8000000,
        .frameSeq.subFrameCfg[0].chirpStartIdxOffset = 0,
        .frameSeq.subFrameCfg[0].numOfBurst = 1,
        .frameSeq.subFrameCfg[0].numOfBurstLoops = 1,
        .frameSeq.subFrameCfg[0].reserved0 = 0,
        .frameSeq.subFrameCfg[0].subFramePeriodicity = 8000000,
        .frameSeq.subFrameCfg[0].reserved1 = 0,
        .frameSeq.subFrameCfg[0].reserved2 = 0,
        .frameSeq.subFrameCfg[1].forceProfileIdx = 0,
        .frameSeq.subFrameCfg[1].chirpStartIdx = 0,
        .frameSeq.subFrameCfg[1].numOfChirps = 64,
        .frameSeq.subFrameCfg[1].numLoops = 1,
        .frameSeq.subFrameCfg[1].burstPeriodicity = 8000000,
        .frameSeq.subFrameCfg[1].chirpStartIdxOffset = 0,
        .frameSeq.subFrameCfg[1].numOfBurst = 1,
        .frameSeq.subFrameCfg[1].numOfBurstLoops = 1,
        .frameSeq.subFrameCfg[1].reserved0 = 0,
        .frameSeq.subFrameCfg[1].subFramePeriodicity = 8000000,
        .frameSeq.subFrameCfg[1].reserved1 = 0,
        .frameSeq.subFrameCfg[1].reserved2 = 0,
        .frameSeq.subFrameCfg[2].forceProfileIdx = 0,
        .frameSeq.subFrameCfg[2].chirpStartIdx = 0,
        .frameSeq.subFrameCfg[2].numOfChirps = 64,
        .frameSeq.subFrameCfg[2].numLoops = 1,
        .frameSeq.subFrameCfg[2].burstPeriodicity = 8000000,
        .frameSeq.subFrameCfg[2].chirpStartIdxOffset = 0,
        .frameSeq.subFrameCfg[2].numOfBurst = 1,
        .frameSeq.subFrameCfg[2].numOfBurstLoops = 1,
        .frameSeq.subFrameCfg[2].reserved0 = 0,
        .frameSeq.subFrameCfg[2].subFramePeriodicity = 8000000,
        .frameSeq.subFrameCfg[2].reserved1 = 0,
        .frameSeq.subFrameCfg[2].reserved2 = 0,
        .frameSeq.subFrameCfg[3].forceProfileIdx = 0,
        .frameSeq.subFrameCfg[3].chirpStartIdx = 0,
        .frameSeq.subFrameCfg[3].numOfChirps = 64,
        .frameSeq.subFrameCfg[3].numLoops = 1,
        .frameSeq.subFrameCfg[3].burstPeriodicity = 8000000,
        .frameSeq.subFrameCfg[3].chirpStartIdxOffset = 0,
        .frameSeq.subFrameCfg[3].numOfBurst = 1,
        .frameSeq.subFrameCfg[3].numOfBurstLoops = 1,
        .frameSeq.subFrameCfg[3].reserved0 = 0,
        .frameSeq.subFrameCfg[3].subFramePeriodicity = 8000000,
        .frameSeq.subFrameCfg[3].reserved1 = 0,
        .frameSeq.subFrameCfg[3].reserved2 = 0,

        .frameSeq.numFrames = 10,
        .frameSeq.triggerSelect = 1,
        .frameSeq.frameTrigDelay = 0,
        .frameSeq.reserved0 = 0,
        .frameSeq.reserved1 = 0,

};

const rlBpmCommonCfg_t bpmCommonCfg =
{
    .mode.b2SrcSel = 0,
    .mode.b1Reserved0 = 0,
    .mode.b13Reserved1 = 0,
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
    .reserved4 = 0
};

const rlBpmChirpCfg_t bpmChirpCfg[3] =
{
    {
        .chirpStartIdx = CHIRP_START_INDEX,
        .chirpEndIdx = CHIRP_END_INDEX,
        .constBpmVal = 0,
        .reserved = 0x0
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX+1,
        .chirpEndIdx = CHIRP_END_INDEX+1,
        .constBpmVal = 21,
        .reserved = 0x0
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX+2,
        .chirpEndIdx = CHIRP_END_INDEX+2,
        .constBpmVal = 21,
        .reserved = 0x0
    }
};

const rlContModeCfg_t contModeCfg =
{
    .startFreqConst = 1497521874,
    .txOutPowerBackoffCode = 0,
    .txPhaseShifter = 0,
    .digOutSampleRate = 10000,
    .hpfCornerFreq1 = 0,
    .hpfCornerFreq2 = 0,
    .rxGain = 30,
    .vcoSelect = 0x0,
    .reserved  = 0x0
};

const rlContModeEn_t contModeEn =
{
    .contModeEn = 1,
    .reserved   = 0
};

const rlDynPwrSave_t dynPwrSave =
{
    .blkCfg   = 0x7,
    .reserved = 0
};

const rlGpAdcCfg_t gpAdcCfg =
{
    .enable = 0x3F,
    .bufferEnable = 0x3F,
    .numOfSamples[0].sampleCnt = 20,
    .numOfSamples[0].settlingTime = 3,
    .numOfSamples[1].sampleCnt = 14,
    .numOfSamples[1].settlingTime = 3,
    .numOfSamples[2].sampleCnt = 14,
    .numOfSamples[2].settlingTime = 3,
    .numOfSamples[3].sampleCnt = 14,
    .numOfSamples[3].settlingTime = 3,
    .numOfSamples[4].sampleCnt = 14,
    .numOfSamples[4].settlingTime = 3,
    .numOfSamples[5].sampleCnt = 14,
    .numOfSamples[5].settlingTime = 3,
    .numOfSamples[6].sampleCnt = 14,
    .numOfSamples[6].settlingTime = 3,
    .numOfSamples[7].sampleCnt = 14,
    .numOfSamples[7].settlingTime = 3,
    .numOfSamples[8].sampleCnt = 14,
    .numOfSamples[8].settlingTime = 3,
    .numOfSamples[9].sampleCnt = 14,
    .numOfSamples[9].settlingTime = 3,
    .numOfSamples[10].sampleCnt = 14,
    .numOfSamples[10].settlingTime = 3,
    .numOfSamples[11].sampleCnt = 14,
    .reserved = 0
};

const rlRfLdoBypassCfg_t rfLdoBypassCfg =
{
    .ldoBypassEnable   = 1,
    .supplyMonIrDrop   = 0,
    .ioSupplyIndicator = 0,
};

const rlTempMonConf_t tempMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .anaTempThreshMin = -10,
    .anaTempThreshMax = 60,
    .digTempThreshMin = -10,
    .digTempThreshMax = 60,
    .tempDiffThresh = 20,
    .reserved1 = 0,
    .reserved2 = 0,
};

const rlRxGainPhaseMonConf_t rxGainPhaseMonCfg =
{
    .profileIndx = 0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .reserved0 = 0,
    .txSel = 1,
    .rxGainAbsThresh = 40,
    .rxGainMismatchErrThresh = 40,
    .rxGainFlatnessErrThresh = 40,
    .rxGainPhaseMismatchErrThresh = (30 * (1U << 16))/360U,
    .rxGainMismatchOffsetVal[0][0] = 0x1A,
    .rxGainMismatchOffsetVal[0][1] = 0x1A,
    .rxGainMismatchOffsetVal[0][2] = 0x1A,
    .rxGainMismatchOffsetVal[1][0] = 0x1A,
    .rxGainMismatchOffsetVal[1][1] = 0x1A,
    .rxGainMismatchOffsetVal[1][2] = 0x1A,
    .rxGainMismatchOffsetVal[2][0] = 0x1A,
    .rxGainMismatchOffsetVal[2][1] = 0x1A,
    .rxGainMismatchOffsetVal[2][2] = 0x1A,
    .rxGainMismatchOffsetVal[3][0] = 0x1A,
    .rxGainMismatchOffsetVal[3][1] = 0x1A,
    .rxGainMismatchOffsetVal[3][2] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[0][0] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[0][1] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[0][2] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[1][0] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[1][1] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[1][2] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[2][0] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[2][1] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[2][2] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[3][0] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[3][1] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[3][2] = 0x1A,
    .reserved1 = 0,
};

const rlRxNoiseMonConf_t rxNoiseMonCfg =
{
    .profileIndx = 0,//profileCfg.profileId,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .reserved0 = 0x0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved1 = 0x0,
    .noiseThresh = 350,
    .reserved2 = 0x0,
};

const rlRxIfStageMonConf_t monRxIfStageCfg =
{
    .profileIndx = 0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .reserved1 = 0,
    .hpfCutoffErrThresh = 15,
    .lpfCutoffErrThresh = 15,
    .ifaGainErrThresh = 100,
    .reserved2 = 0,
};

rlTxPowMonConf_t tx0PowMonCfg =
{
    .profileIndx = 0x0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .reserved0 = 0x0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved1 = 0x0,
    .txPowAbsErrThresh = 30,
    .txPowFlatnessErrThresh = 30,
    .reserved2 = 0x0,
    .reserved3 = 0x0,
};

rlTxPowMonConf_t tx1PowMonCfg =
{
    .profileIndx = 0x0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .reserved0 = 0x0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved1 = 0x0,
    .txPowAbsErrThresh = 30,
    .txPowFlatnessErrThresh = 30,
    .reserved2 = 0x0,
    .reserved3 = 0x0,
};

rlTxPowMonConf_t tx2PowMonCfg =
{
    .profileIndx = 0x0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .reserved0 = 0x0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved1 = 0x0,
    .txPowAbsErrThresh = 30,
    .txPowFlatnessErrThresh = 30,
    .reserved2 = 0x0,
    .reserved3 = 0x0,
};

const rlAllTxPowMonConf_t allTxPowMonCfg =
{
    .tx0PowrMonCfg = &tx0PowMonCfg,
    .tx1PowrMonCfg = &tx1PowMonCfg,
    .tx2PowrMonCfg = &tx2PowMonCfg,
};

rlTxBallbreakMonConf_t tx0BallBreakMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0x0,
    .txReflCoeffMagThresh = -90,
    .reserved1 = 0x0,
    .reserved2 = 0x0,
};

rlTxBallbreakMonConf_t tx1BallBreakMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0x0,
    .txReflCoeffMagThresh = -90,
    .reserved1 = 0x0,
    .reserved2 = 0x0,
};

rlTxBallbreakMonConf_t tx2BallBreakMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0x0,
    .txReflCoeffMagThresh = -90,
    .reserved1 = 0x0,
    .reserved2 = 0x0,
};

const rlAllTxBallBreakMonCfg_t allTxBallbreakMonCfg =
{
    .tx0BallBrkMonCfg = &tx0BallBreakMonCfg,
    .tx1BallBrkMonCfg = &tx1BallBreakMonCfg,
    .tx2BallBrkMonCfg = &tx2BallBreakMonCfg,
};

const rlTxGainPhaseMismatchMonConf_t txGainPhMisMatchMonCfg =
{
    .profileIndx = 0x0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .txEn = 0x3,
    .rxEn = 0x7,
    .reserved0 = 0x0,
    .reserved1 = 0x0,
    .txGainMismatchThresh = 50,
    .txPhaseMismatchThresh = (30 * (1 << 16))/360 ,
    .txGainMismatchOffsetVal = 0x0,
    .reserved1 = 0x0,
    .reserved2 = 0x0,
};

rlTxBpmMonConf_t tx0BpmMonCfg =
{
    .profileIndx = 0x0,
    .phaseShifterMonCnfg = 0xC2,
    .phaseShifterMon1 = 0x06,
    .phaseShifterMon2 = 0x06,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .rxEn = 0x1,
    .txBpmPhaseErrThresh = 0x1555,
    .txBpmAmplErrThresh = 30,
    .phaseShifterThreshMax = 40,
    .phaseShifterThreshMin = 0,
    .reserved = 0x0,
};

rlTxBpmMonConf_t tx1BpmMonCfg =
{
    .profileIndx = 0x0,
    .phaseShifterMonCnfg = 0xC2,
    .phaseShifterMon1 = 0x06,
    .phaseShifterMon2 = 0x06,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .rxEn = 0x1,
    .txBpmPhaseErrThresh = 0x1555,
    .txBpmAmplErrThresh = 30,
    .phaseShifterThreshMax = 40,
    .phaseShifterThreshMin = 0,
    .reserved = 0x0,
};

rlTxBpmMonConf_t tx2BpmMonCfg =
{
    .profileIndx = 0x0,
    .phaseShifterMonCnfg = 0xC2,
    .phaseShifterMon1 = 0x06,
    .phaseShifterMon2 = 0x06,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .rxEn = 0x1,
    .txBpmPhaseErrThresh = 0x1555,
    .txBpmAmplErrThresh = 30,
    .phaseShifterThreshMax = 40,
    .phaseShifterThreshMin = 0,
    .reserved = 0x0,
};

const rlAllTxBpmMonConf_t allTxBpmMonCfg =
{
    .tx0BpmMonCfg = &tx0BpmMonCfg,
    .tx1BpmMonCfg = &tx1BpmMonCfg,
    .tx2BpmMonCfg = &tx2BpmMonCfg,
};

const rlSynthFreqMonConf_t synthFreqMonCfg =
{
    .profileIndx = 0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .freqErrThresh = 4000,
    .monStartTime = 10,
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
};

const rlExtAnaSignalsMonConf_t extAnaSigMonCfg =
{
    .reportMode = 2,
    .reserved0 = 0,
    .signalInpEnables = 0x3F,
    .signalBuffEnables = 0x1F,
    .signalSettlingTime[0] = 10,
    .signalSettlingTime[1] = 10,
    .signalSettlingTime[2] = 10,
    .signalSettlingTime[3] = 10,
    .signalSettlingTime[4] = 10,
    .signalSettlingTime[5] = 10,
    .signalThresh[0] = 0,
    .signalThresh[1] = 0,
    .signalThresh[2] = 0,
    .signalThresh[3] = 0,
    .signalThresh[4] = 0,
    .signalThresh[5] = 0,
    .signalThresh[6] = 200,
    .signalThresh[7] = 200,
    .signalThresh[8] = 200,
    .signalThresh[9] = 200,
    .signalThresh[10] = 200,
    .signalThresh[11] = 200,
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
};

rlTxIntAnaSignalsMonConf_t tx0IntAnaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = 2,
    .reserved0 = 0,
    .reserved1 = 0,
};

rlTxIntAnaSignalsMonConf_t tx1IntAnaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = 2,
    .reserved0 = 0,
    .reserved1 = 0,
};

rlTxIntAnaSignalsMonConf_t tx2IntAnaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = 2,
    .reserved0 = 0,
    .reserved1 = 0,
};

const rlAllTxIntAnaSignalsMonConf_t allTxanaSigMonCfg =
{
    .tx0IntAnaSgnlMonCfg = &tx0IntAnaSigMonCfg,
    .tx1IntAnaSgnlMonCfg = &tx1IntAnaSigMonCfg,
    .tx2IntAnaSgnlMonCfg = &tx2IntAnaSigMonCfg,
};

const rlRxIntAnaSignalsMonConf_t rxIntAnaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = 2,
    .reserved0 = 0,
    .reserved1 = 0,
};

const rlPmClkLoIntAnaSignalsMonConf_t pmClkLoIntAnaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .reserved1 = 0,
};

const rlGpadcIntAnaSignalsMonConf_t gpadcIntAnaSigMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
};

const rlPllContrVoltMonConf_t pllConVoltMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .signalEnables = 7,
    .reserved1 = 0,
};

const rlDualClkCompMonConf_t dualClkCompMonCfg =
{
    .reportMode = 0,
    .reserved0 = 10,
    .dccPairEnables = 63,
    .reserved1 = 0,
};

const rlRxMixInPwrMonConf_t rxMixInpwrMonCfg =
{
    .profileIndx = 0,
    .reportMode = 2,
    .txEnable = 3,
    .reserved0 = 0,
    .thresholds = 0xFA00,
    .reserved1 = 0,
    .reserved1 = 0,

};

const rlRfProgFiltConf_t progFiltCfg =
{
    .profileId = 0,
    .coeffStartIdx = 0,
    .progFiltLen = 14,
    .progFiltFreqShift = 100,
};

const rlRfProgFiltCoeff_t progFiltCoef =
{
    .coeffArray[0] = -876,
    .coeffArray[1] = -272,
    .coeffArray[2] = 1826,
    .coeffArray[3] = -395,
    .coeffArray[4] = -3672,
    .coeffArray[5] = 3336,
    .coeffArray[6] = 15976,
    .coeffArray[7] = 15976,
    .coeffArray[8] = 3336,
    .coeffArray[9] = -3672,
    .coeffArray[10] = -395,
    .coeffArray[11] = 1826,
    .coeffArray[12] = -272,
    .coeffArray[13] = -876,
};

const rlSubFrameStartCfg_t subFrameTrigCfg =
{
    .startCmd = 1,
    .reserved = 0,
};

const rlRfMiscConf_t miscCfg = 
{
    .miscCtl  = 1,
    .reserved = 0,
};

const rlRfPhaseShiftCfg_t phaseShiftCfg[2] = 
{
    {
        .chirpStartIdx = CHIRP_START_INDEX,
        .chirpEndIdx   = CHIRP_END_INDEX + 255,
        .tx0PhaseShift = 42,
        .tx1PhaseShift = 62,
        .tx2PhaseShift = 15,
        .reserved      = 0
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX + 256,
        .chirpEndIdx   = CHIRP_END_INDEX + 511,
        .tx0PhaseShift = 63,
        .tx1PhaseShift = 50,
        .tx2PhaseShift = 25,
        .reserved      = 0
    },
};

const rlDynPerChirpPhShftCfg_t dynPhaseShiftCfg[10] = 
{
    {
        .reserved     = 0,
        .chirpSegSel  = 0,
        .phShiftPerTx = {{47, 1,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 49},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 48, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {51, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {50, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 1,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  56},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 54, 20}, {21, 53, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {57, 31, 32}, {33, 34, 35},
                         {52, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 2,
        .phShiftPerTx = {{0, 61,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 59}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {58, 28, 29},
                         {60, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 62, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 3,
        .phShiftPerTx = {{50, 1,  2},  {3,  4,  5},
                         {6,  7,  8},  {49, 10, 11},
                         {12, 13, 14}, {15, 16, 48},
                         {47, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 63, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 4,
        .phShiftPerTx = {{0, 51,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 52}, {53, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 5,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  55},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 54}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 6,
        .phShiftPerTx = {{0,  1,  2}, {3,  4,  5},
                         {6,  7,  8}, {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 7,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {56, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {57, 58, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 8,
        .phShiftPerTx = {{47, 48, 49}, {50, 51, 52},
                         {53, 54, 55}, {56, 57, 58},
                         {59, 60, 61}, {62, 63, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 9,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
};

const rlInterRxGainPhConf_t interRxGainPhCfg = 
{
    .profileIndx     = 0,
    .reserved0       = 0,
    .reserved1       = 1,
    .digRxGain[0]    = 100,
    .digRxGain[1]    = 20,
    .digRxGain[2]    = 75,
    .digRxGain[3]    = 105,
    .digRxPhShift[0] = 25102,
    .digRxPhShift[1] = 102,
    .digRxPhShift[2] = 65535,
    .digRxPhShift[3] = 55201,
    .reserved2       = 0,
    .reserved3       = 0,
};

const rlTestSource_t testSourceCfg = 
{
    .testObj[0]      = {102, 2, 5698, 5, 8, 0, 100, 1256, 2589, 225, 2489, 12256, 112},
    .testObj[1]      = {10, 5, 0, 5, 5, 5, 255, 0, 216, 2251, 341, 556, 5112},
    .rxAntPos[0]     = {15, 15},
    .rxAntPos[1]     = {15, 15},
    .rxAntPos[2]     = {15, 15},
    .rxAntPos[3]     = {15, 15},
    .txAntPos[0]     = {15, 15},
    .txAntPos[1]     = {15, 15},
    .txAntPos[2]     = {15, 15},
    .reserved        = 0,
};

const rlTestSourceEnable_t testSourceEn = 
{
    .tsEnable  = 1,
    .reserved  = 0,
};

const rlRxGainTempLutReadReq_t rxGainTempLutReadCfg = 
{
    .profileIndx   = 0,
    .reserved0     = 0,
    .reserved1     = 0,
};

const rlTxGainTempLutReadReq_t txGainTempLutReadCfg = 
{
    .profileIndx   = 0,
    .reserved0     = 0,
    .reserved1     = 0,
};

