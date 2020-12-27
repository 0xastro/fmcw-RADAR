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
 *  \file link_testcase_config.h
 *
 *  \brief This file defines the common configurations like driver config etc...
 */

#ifndef LINK_TESTCASE_CONFIG_H_
#define LINK_TESTCASE_CONFIG_H_

/* mmwave SDK include files */
#include <ti/control/mmwavelink/mmwavelink.h>

extern const rlChanCfg_t chCfg;

extern const rlAdcOutCfg_t adcOutCfgArgs;

extern const rlLowPowerModeCfg_t lowPowerModeCfg;

extern const rlDevHsiClk_t deviceHsiClk;

extern const rlProfileCfg_t  profileCfg[4U];

extern const rlChirpCfg_t  chirpCfg[2];

extern const rlDynChirpCfg_t dynChirpCfg[10];

extern const rlRxSatMonConf_t rxSatMonCfg;

extern const rlSigImgMonConf_t sigImgMonCfg;

extern rlFrameCfg_t  frameCfg;

extern const rlRfCalMonTimeUntConf_t calMonTimeUnitConf;

extern const rlRfCalMonFreqLimitConf_t freqLimit;

extern const rlRfInitCalConf_t rfInitCalib;

extern const rlRunTimeCalibConf_t runTimeCalib;

extern rlAdvFrameCfg_t advFrameCfg;

extern const rlBpmCommonCfg_t bpmCommonCfg;

extern const rlBpmChirpCfg_t bpmChirpCfg[3];

extern const rlContModeCfg_t contModeCfg;

extern const rlContModeEn_t contModeEn;

extern const rlDynPwrSave_t dynPwrSave;

extern const rlGpAdcCfg_t gpAdcCfg;

extern const rlRfLdoBypassCfg_t rfLdoBypassCfg;

extern const rlTempMonConf_t tempMonCfg;

extern const rlRxGainPhaseMonConf_t rxGainPhaseMonCfg;

extern const rlRxNoiseMonConf_t rxNoiseMonCfg;

extern const rlRxIfStageMonConf_t monRxIfStageCfg;

extern const rlAllTxPowMonConf_t allTxPowMonCfg;

extern const rlAllTxBallBreakMonCfg_t allTxBallbreakMonCfg;

extern const rlTxGainPhaseMismatchMonConf_t txGainPhMisMatchMonCfg;

extern const rlAllTxBpmMonConf_t allTxBpmMonCfg;

extern const rlSynthFreqMonConf_t synthFreqMonCfg;

extern const rlExtAnaSignalsMonConf_t extAnaSigMonCfg;

extern const rlAllTxIntAnaSignalsMonConf_t allTxanaSigMonCfg;

extern const rlRxIntAnaSignalsMonConf_t rxIntAnaSigMonCfg;

extern const rlPmClkLoIntAnaSignalsMonConf_t pmClkLoIntAnaSigMonCfg;

extern const rlGpadcIntAnaSignalsMonConf_t gpadcIntAnaSigMonCfg;

extern const rlPllContrVoltMonConf_t pllConVoltMonCfg;

extern const rlDualClkCompMonConf_t dualClkCompMonCfg;

extern const rlRxMixInPwrMonConf_t rxMixInpwrMonCfg;

extern const rlRfProgFiltConf_t progFiltCfg;

extern const rlRfProgFiltCoeff_t progFiltCoef;

extern const rlSubFrameStartCfg_t subFrameTrigCfg;

extern const rlRfMiscConf_t miscCfg;

extern const rlRfPhaseShiftCfg_t phaseShiftCfg[2];

extern const rlDynPerChirpPhShftCfg_t dynPhaseShiftCfg[10];

extern const rlInterRxGainPhConf_t interRxGainPhCfg;

extern const rlTestSource_t testSourceCfg;

extern const rlTestSourceEnable_t testSourceEn;

extern const rlRxGainTempLutReadReq_t rxGainTempLutReadCfg;

extern const rlTxGainTempLutReadReq_t txGainTempLutReadCfg;

#endif /* #ifndef LINK_TESTCASE_CONFIG_H_ */
