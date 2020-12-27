/*
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
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
/**
 *   @file  rangeprochwa.c
 *
 *   @brief
 *      Implements Range FFT data processing Unit using HWA.
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

/* mmWave SDK common/driver Include files */
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/hwa/hwa.h>

/* Data Path Include files */
#include <ti/datapath/dpu/rangeproc/rangeprochwa.h>

/* MATH utils library Include files */
#include <ti/utils/mathutils/mathutils.h>

/* Internal include Files */
#include <ti/datapath/dpu/rangeproc/include/rangeprochwa_internal.h>

/* Flag to check input parameters */
#define DEBUG_CHECK_PARAMS   1

/**************************************************************************
 ************************ Internal Functions Prototype       **********************
 **************************************************************************/
static void rangeProcHWADoneIsrCallback(void * arg);
static void rangeProcHWA_EDMA_transferCompletionCallbackFxn(uintptr_t arg,
    uint8_t transferCompletionCode);

static int32_t rangeProcHWA_ConfigEDMATranspose
(
    rangeProc_dpParams      *dpParams,
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    uint32_t                srcAddress,
    uint32_t                destAddress,
    bool                    isTransferCompletionEnabled,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t               transferCompletionCallbackFxnArg
);

static int32_t rangeProcHWA_ConfigHWA
(
    rangeProcHWAObj     *rangeProcObj,
    uint8_t     destChanPing,
    uint8_t     destChanPong,
    uint16_t    hwaMemSrcPingOffset,
    uint16_t    hwaMemSrcPongOffset,
    uint16_t    hwaMemDestPingOffset,
    uint16_t    hwaMemDestPongOffset
);

static int32_t rangeProcHWA_TriggerHWA
(
    rangeProcHWAObj     *rangeProcObj
);
static int32_t rangeProcHWA_ConfigEDMA_DataOut_interleave
(
    rangeProcHWAObj     *rangeProcObj,
    rangeProc_dpParams  *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t            hwaOutPingOffset,
    uint32_t            hwaOutPongOffset
);
static int32_t rangeProcHWA_ConfigEDMA_DataOut_nonInterleave
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t                hwaOutPingOffset,
    uint32_t                hwaOutPongOffset
);
static int32_t rangeProcHWA_ConfigEDMA_DataIn
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
);
static int32_t rangeProcHWA_ConifgInterleaveMode
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
);
static int32_t rangeProcHWA_ConifgNonInterleaveMode
(
    rangeProcHWAObj          *rangeProcObj,
    rangeProc_dpParams       *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
);
static int32_t rangeProcHWA_dcRangeSignatureCompensation_init
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcRangeSigCfg,
    uint8_t             resetMeanBuffer
);

static void rangeProcHWA_dcRangeSignatureCompensation
(
    rangeProcHWAObj     *obj
);
static int32_t rangeProcHWA_ConfigEDMADummyThreeLinks
(
    EDMA_Handle             handle,
    DPEDMA_3LinkChanCfg     *chanCfg,
    uint8_t                 chainChId0,
    uint8_t                 chainChId1,
    uint8_t                 chainChId2,
    uint16_t                numIter,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t               transferCompletionCallbackFxnArg
);

/**************************************************************************
 ************************RangeProcHWA Internal Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      HWA processing completion call back function as per HWA API.
 *      Depending on the programmed transfer completion codes,
 *      posts HWA done semaphore.
 *
 *  @param[in]  arg                 Argument to the callback function
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static void rangeProcHWADoneIsrCallback(void * arg)
{
    if (arg != NULL) 
    {
        SemaphoreP_post((SemaphoreP_Handle)arg);
    }
}

/**
 *  @b Description
 *  @n
 *      EDMA processing completion call back function as per EDMA API.
 *
 *  @param[in]  arg                     Argument to the callback function
 *  @param[in]  transferCompletionCode  EDMA transfer complete code
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static void rangeProcHWA_EDMA_transferCompletionCallbackFxn(uintptr_t arg,
    uint8_t transferCompletionCode)
{
    rangeProcHWAObj     *rangeProcObj;

    /* Get rangeProc object */
    rangeProcObj = (rangeProcHWAObj *)arg;

    if (transferCompletionCode == rangeProcObj->dataOutSignatureChan)
    {
        rangeProcObj->numEdmaDataOutCnt++;
        SemaphoreP_post(rangeProcObj->edmaDoneSemaHandle);
    }
}

/**
 *  @b Description
 *  @n
 *      Function to config a dummy channel with 3 linked paramset. Each paramset is linked
 *   to a EDMA data copy channel
 *
 *  @param[in]  handle                  EDMA handle
 *  @param[in]  chanCfg                 EDMA channel configuraton
 *  @param[in]  chainChId0              linked EDMA channel 1
 *  @param[in]  chainChId1              linked EDMA channel 2
 *  @param[in]  chainChId2              linked EDMA channel 3
 *  @param[in]  numIter                 Number of iterations the dummy channel will be excuted.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static int32_t rangeProcHWA_ConfigEDMADummyThreeLinks
(
    EDMA_Handle             handle,
    DPEDMA_3LinkChanCfg     *chanCfg,
    uint8_t                 chainChId0,
    uint8_t                 chainChId1,
    uint8_t                 chainChId2,
    uint16_t                numIter,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t               transferCompletionCallbackFxnArg
)
{
    EDMA_channelConfig_t config;
    int32_t             errorCode = EDMA_NO_ERROR;
    uint16_t            linkChId0;
    uint16_t            linkChId1;
    uint16_t            linkChId2;

    /* Get LinkChan from configuraiton */
    linkChId0 = chanCfg->channelShadow[0];
    linkChId1 = chanCfg->channelShadow[1];
    linkChId2 = chanCfg->channelShadow[2];

    config.channelId = chanCfg->channel;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = chanCfg->channel;
    config.eventQueueId = chanCfg->eventQueue;

    config.paramSetConfig.sourceAddress = (uint32_t) NULL;
    config.paramSetConfig.destinationAddress = (uint32_t) NULL;
    config.paramSetConfig.bCountReload = 0;

    /* Hardware triggered dummy channel that runs for numIter loops */
    config.paramSetConfig.aCount = 0;
    config.paramSetConfig.bCount = numIter;
    config.paramSetConfig.cCount = 0;

    config.paramSetConfig.sourceBindex = 0;
    config.paramSetConfig.destinationBindex = 0;

    config.paramSetConfig.sourceCindex = 0;
    config.paramSetConfig.destinationCindex = 0;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.transferCompletionCode = chainChId0;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;
    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled = false;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = false;
    config.paramSetConfig.isFinalChainingEnabled =true;
    config.paramSetConfig.isIntermediateChainingEnabled =true;
    config.transferCompletionCallbackFxn = NULL;

    if ((errorCode = EDMA_configChannel(handle, &config, true)) != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Change the parameter set to use different transferCompletionCode */
    {
        EDMA_paramConfig_t paramConfig;

        memcpy((void *)&paramConfig.paramSetConfig, (void *)&config.paramSetConfig, sizeof(EDMA_paramSetConfig_t));

        paramConfig.transferCompletionCallbackFxn = NULL;
        paramConfig.transferCompletionCallbackFxnArg = NULL;

        /* to TX1 EDMA channel */
        paramConfig.paramSetConfig.transferCompletionCode = chainChId2;
        if ((errorCode = EDMA_configParamSet(handle, linkChId2, &paramConfig)) != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* to TX2 EDMA channel */
        paramConfig.paramSetConfig.transferCompletionCode = chainChId1;
        if ((errorCode = EDMA_configParamSet(handle, linkChId1, &paramConfig)) != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* to TX0 EDMA channel */
        paramConfig.paramSetConfig.transferCompletionCode = chainChId0;
        if ((errorCode = EDMA_configParamSet(handle, linkChId0, &paramConfig)) != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* Link 3 shadow links */
        if ((errorCode = EDMA_linkParamSets(handle, linkChId0, linkChId1)) != EDMA_NO_ERROR)
        {
            goto exit;
        }

        if ((errorCode = EDMA_linkParamSets(handle, linkChId1, linkChId2)) != EDMA_NO_ERROR)
        {
            goto exit;
        }

        if ((errorCode = EDMA_linkParamSets(handle, linkChId2, linkChId0)) != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* Link dummy channel to the 2nd link */
        if ((errorCode = EDMA_linkParamSets(handle, chanCfg->channel, linkChId1)) != EDMA_NO_ERROR)
        {
            goto exit;
        }
    }
exit:
    return(errorCode);
}


/**
 *  @b Description
 *  @n
 *      Function to config a dummy channel with 3 linked paramset. Each paramset is linked
 *   to a EDMA data copy channel
 *
 *  @param[in]  dpParams                Pointer to data path parameters
 *  @param[in]  handle                  EDMA handle
 *  @param[in]  chanCfg                 EDMA channel configuraton
 *  @param[in]  chainingCfg             EDMA chaining configuration
 *  @param[in]  srcAddress              EDMA copy source address
 *  @param[in]  destAddress             EDMA copy destination address
 *  @param[in]  isTransferCompletionEnabled Number of iterations the dummy channel will be excuted.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static int32_t rangeProcHWA_ConfigEDMATranspose
(
    rangeProc_dpParams      *dpParams,
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    uint32_t                srcAddress,
    uint32_t                destAddress,
    bool                    isTransferCompletionEnabled,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t               transferCompletionCallbackFxnArg
)
{
    DPEDMA_syncABCfg        syncABCfg;
    int32_t                 retVal;

    /* dpedma configuration */
    syncABCfg.aCount = dpParams->numRxAntennas * sizeof(cmplx16ImRe_t);
    syncABCfg.bCount = dpParams->numRangeBins;
    syncABCfg.cCount = dpParams->numChirpsPerFrame/2U;
    syncABCfg.srcBIdx = dpParams->numRxAntennas * sizeof(cmplx16ImRe_t);
    syncABCfg.srcCIdx = 0U;
    syncABCfg.dstBIdx = dpParams->numRxAntennas *dpParams->numChirpsPerFrame * sizeof(cmplx16ImRe_t);
    syncABCfg.dstCIdx = dpParams->numRxAntennas * 2U * sizeof(cmplx16ImRe_t);

    syncABCfg.srcAddress = srcAddress;
    syncABCfg.destAddress= destAddress;

    retVal = DPEDMA_configSyncAB(handle,
            chanCfg,
            chainingCfg,
            &syncABCfg,
            true,    /* isEventTriggered */
            false,   /* isIntermediateTransferCompletionEnabled */
            isTransferCompletionEnabled,   /* isTransferCompletionEnabled */
            transferCompletionCallbackFxn,
            transferCompletionCallbackFxnArg);

    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Internal function to config HWA to perform range FFT
 *
 *  @param[in]  rangeProcObj                  Pointer to rangeProc object
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigHWACommon
(
    rangeProcHWAObj     *rangeProcObj
)
{
    HWA_CommonConfig    hwaCommonConfig;
    rangeProc_dpParams  *DPParams;
    int32_t             retVal;

    DPParams = &rangeProcObj->params;

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_NUMLOOPS |
                               HWA_COMMONCONFIG_MASK_PARAMSTARTIDX |
                               HWA_COMMONCONFIG_MASK_PARAMSTOPIDX |
                               HWA_COMMONCONFIG_MASK_FFT1DENABLE |
                               HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD |
                               HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE |
                               HWA_COMMONCONFIG_MASK_LFSRSEED;

    hwaCommonConfig.fftConfig.twidDitherEnable = HWA_FEATURE_BIT_ENABLE;
    hwaCommonConfig.fftConfig.lfsrSeed = 0x1234567; /*Some non-zero value*/
    hwaCommonConfig.numLoops = DPParams->numChirpsPerFrame/2U;
    hwaCommonConfig.paramStartIdx = rangeProcObj->hwaCfg.paramSetStartIdx;
    hwaCommonConfig.paramStopIdx = rangeProcObj->hwaCfg.paramSetStartIdx + rangeProcObj->hwaCfg.numParamSet - 1U;

    if (rangeProcObj->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_ISOLATED)
    {
        /* HWA will input data from M0 memory*/
        hwaCommonConfig.fftConfig.fft1DEnable = HWA_FEATURE_BIT_DISABLE;
    }
    else
    {
        /* HWA will input data from ADC buffer memory*/
        hwaCommonConfig.fftConfig.fft1DEnable = HWA_FEATURE_BIT_ENABLE;
    }
    hwaCommonConfig.fftConfig.interferenceThreshold = 0xFFFFFF;
    retVal = HWA_configCommon(rangeProcObj->initParms.hwaHandle, &hwaCommonConfig);
    if (retVal != 0)
    {
        goto exit;
    }

    /**********************************************/
    /* ENABLE NUMLOOPS DONE INTERRUPT FROM HWA */
    /**********************************************/
    retVal = HWA_enableDoneInterrupt(rangeProcObj->initParms.hwaHandle,
                                        rangeProcHWADoneIsrCallback,
                                        rangeProcObj->hwaDoneSemaHandle);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return(retVal);
}
/**
 *  @b Description
 *  @n
 *      Internal function to config HWA to perform range FFT
 *
 *  @param[in]  rangeProcObj                  Pointer to rangeProc object
 *  @param[in]  destChanPing                  Destination channel id for PING
 *  @param[in]  destChanPong                  Destination channel id for PONG
 *  @param[in]  hwaMemSrcPingOffset           Source Address offset for Ping input
 *  @param[in]  hwaMemSrcPongOffset           Source Address offset for Pong input
 *  @param[in]  hwaMemDestPingOffset          Destination address offset for Ping output
 *  @param[in]  hwaMemDestPongOffset          Destination address offset for Pong output
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigHWA
(
    rangeProcHWAObj     *rangeProcObj,
    uint8_t     destChanPing,
    uint8_t     destChanPong,
    uint16_t    hwaMemSrcPingOffset,
    uint16_t    hwaMemSrcPongOffset,
    uint16_t    hwaMemDestPingOffset,
    uint16_t    hwaMemDestPongOffset
)
{
    HWA_InterruptConfig     paramISRConfig;
    int32_t                 errCode = 0;
    uint32_t                paramsetIdx = 0;
    uint32_t                hwParamsetIdx;
    uint32_t                pingParamSetIdx = 0;
    HWA_ParamConfig         hwaParamCfg[DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS];
    HWA_Handle                      hwaHandle;
    rangeProc_dpParams             *pDPParams;
    uint8_t                         index;

    hwaHandle = rangeProcObj->initParms.hwaHandle;
    pDPParams = &rangeProcObj->params;

    memset(hwaParamCfg,0,sizeof(hwaParamCfg));

    hwParamsetIdx = rangeProcObj->hwaCfg.paramSetStartIdx;
    for(index = 0; index < DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS; index++)
    {
        errCode = HWA_disableParamSetInterrupt(hwaHandle, index + rangeProcObj->hwaCfg.paramSetStartIdx, 
                HWA_PARAMDONE_INTERRUPT_TYPE_CPU |HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /***********************/
    /* PING DUMMY PARAMSET */
    /***********************/
    hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA;
    hwaParamCfg[paramsetIdx].dmaTriggerSrc = hwParamsetIdx;
    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_NONE;
    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /***********************/
    /* PING PROCESS PARAMSET */
    /***********************/
    paramsetIdx++;
    hwParamsetIdx++;
    pingParamSetIdx = paramsetIdx;

    /* adcbuf mapped */
    if(rangeProcObj->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_MAPPED)
    {
        hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DFE;
    }
    else
    {
        /*adcbuf not mapped, should trigger after edma copy is done */
        hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA;
        hwaParamCfg[paramsetIdx].dmaTriggerSrc = hwParamsetIdx;
    }

    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    hwaParamCfg[paramsetIdx].source.srcAddr = hwaMemSrcPingOffset; 

    hwaParamCfg[paramsetIdx].source.srcShift = 0;
    hwaParamCfg[paramsetIdx].source.srcCircShiftWrap = 0;
    hwaParamCfg[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    hwaParamCfg[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
    hwaParamCfg[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg[paramsetIdx].source.srcConjugate = 0;
    hwaParamCfg[paramsetIdx].source.srcScale = 8;
    hwaParamCfg[paramsetIdx].source.bpmEnable = 0;
    hwaParamCfg[paramsetIdx].source.bpmPhase = 0;
    hwaParamCfg[paramsetIdx].dest.dstAddr = hwaMemDestPingOffset; 

    hwaParamCfg[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    hwaParamCfg[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
    hwaParamCfg[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; 
    hwaParamCfg[paramsetIdx].dest.dstConjugate = 0; 
    hwaParamCfg[paramsetIdx].dest.dstScale = 0;
    hwaParamCfg[paramsetIdx].dest.dstSkipInit = 0; 

    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(pDPParams->numRangeBins);
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x3;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.interfZeroOutEn = 0;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowEn = 1;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowStart = rangeProcObj->hwaCfg.hwaWinRamOffset; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.winSymm = rangeProcObj->hwaCfg.hwaWinSym; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.winInterpolateMode = 0; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
    hwaParamCfg[paramsetIdx].complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

    /* HWA range FFT src/dst configuration*/
    if(rangeProcObj->interleave == DPIF_RXCHAN_INTERLEAVE_MODE)
    {
        if(rangeProcObj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
        {
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPParams->numAdcSamples - 1; /* this is samples - 1 */
            hwaParamCfg[paramsetIdx].source.srcAIdx = pDPParams->numRxAntennas* sizeof(uint32_t);
            hwaParamCfg[paramsetIdx].source.srcBcnt = pDPParams->numRxAntennas-1;
            hwaParamCfg[paramsetIdx].source.srcBIdx = sizeof(uint32_t);

            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPParams->numRangeBins-1;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = pDPParams->numRxAntennas * sizeof(uint32_t);
            hwaParamCfg[paramsetIdx].dest.dstBIdx = sizeof(uint32_t);
        }
        else
        {
            /* Other radarCube layout format is not supported */
            errCode = DPU_RANGEPROCHWA_ENOTIMPL;
            goto exit;
        }
    }
    else
    {
        if(rangeProcObj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
        {
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPParams->numAdcSamples - 1;
            hwaParamCfg[paramsetIdx].source.srcAIdx = sizeof(uint32_t);
            hwaParamCfg[paramsetIdx].source.srcBcnt = pDPParams->numRxAntennas-1;
            hwaParamCfg[paramsetIdx].source.srcBIdx = rangeProcObj->rxChanOffset;

            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPParams->numRangeBins-1;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = sizeof(uint32_t) * pDPParams->numRxAntennas; 
            hwaParamCfg[paramsetIdx].dest.dstBIdx = sizeof(uint32_t);
        }
        else
        {
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPParams->numAdcSamples - 1;
            hwaParamCfg[paramsetIdx].source.srcAIdx = sizeof(uint32_t); 
            hwaParamCfg[paramsetIdx].source.srcBcnt = pDPParams->numRxAntennas-1; 
            hwaParamCfg[paramsetIdx].source.srcBIdx = rangeProcObj->rxChanOffset; 
            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPParams->numRangeBins-1;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = sizeof(uint32_t); 
            hwaParamCfg[paramsetIdx].dest.dstBIdx = pDPParams->numRangeBins * sizeof(uint32_t); 
        }
    }

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /* enable the DMA hookup to this paramset so that data gets copied out */
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChanPing;

    errCode = HWA_enableParamSetInterrupt(hwaHandle,hwParamsetIdx,&paramISRConfig);
    if (errCode != 0)
    {
        goto exit;
    }

    /***********************/
    /* PONG DUMMY PARAMSET */
    /***********************/
    paramsetIdx++;
    hwParamsetIdx++;

    hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA;
    hwaParamCfg[paramsetIdx].dmaTriggerSrc = hwParamsetIdx;
    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_NONE;
    errCode = HWA_configParamSet(hwaHandle, 
                                  hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /***********************/
    /* PONG PROCESS PARAMSET */
    /***********************/
    paramsetIdx++;
    hwParamsetIdx++;
    hwaParamCfg[paramsetIdx] = hwaParamCfg[pingParamSetIdx];
    hwaParamCfg[paramsetIdx].source.srcAddr = hwaMemSrcPongOffset; 
    hwaParamCfg[paramsetIdx].dest.dstAddr = hwaMemDestPongOffset;

    hwaParamCfg[paramsetIdx].dmaTriggerSrc = hwParamsetIdx;

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /* Enable the DMA hookup to this paramset so that data gets copied out */
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChanPong;
    errCode = HWA_enableParamSetInterrupt(hwaHandle, 
                                           hwParamsetIdx,
                                           &paramISRConfig);
    if (errCode != 0)
    {
        goto exit;
    }
exit:
    return(errCode);
}

/**
 *  @b Description
 *  @n
 *      Trigger HWA for range processing.
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_TriggerHWA
(
    rangeProcHWAObj     *rangeProcObj
)
{
    int32_t             retVal = 0;
    HWA_Handle          hwaHandle;

    /* Get HWA driver handle */
    hwaHandle = rangeProcObj->initParms.hwaHandle;

    /* Configure HWA common parameters */
    retVal = rangeProcHWA_ConfigHWACommon(rangeProcObj);
    if(retVal < 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(hwaHandle, 1);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Trigger the HWA paramset for Ping */
    retVal = HWA_setDMA2ACCManualTrig(hwaHandle, rangeProcObj->dataOutTrigger[0]);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Trigger the HWA paramset for Pong */
    retVal = HWA_setDMA2ACCManualTrig(hwaHandle, rangeProcObj->dataOutTrigger[1]);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      EDMA configuration for rangeProc data output in interleave mode
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  DPParams                  Pointer to datapath parameter
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *  @param[in]  hwaOutPingOffset          Ping HWA memory address offset
 *  @param[in]  hwaOutPongOffset          Pong HWA memory address offset
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_DataOut_interleave
(
    rangeProcHWAObj     *rangeProcObj,
    rangeProc_dpParams  *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t            hwaOutPingOffset,
    uint32_t            hwaOutPongOffset
)
{
    int32_t             errorCode = EDMA_NO_ERROR;
    EDMA_Handle         handle ;
    DPEDMA_ChainingCfg       chainingCfg;

    /* Get rangeProc hardware resources pointer */
    handle = rangeProcObj->edmaHandle;

    /* Setup Chaining configuration */
    chainingCfg.chainingChan = pHwConfig->edmaOutCfg.dataOutSignature.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled = true;

    errorCode = rangeProcHWA_ConfigEDMATranspose(DPParams,
                                        handle,
                                        &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                                        &chainingCfg,
                                        hwaOutPingOffset,
                                        (uint32_t)rangeProcObj->radarCubebuf,
                                        false,  /* isTransferCompletionEnabled */
                                        NULL,   /* transferCompletionCallbackFxn */
                                        NULL);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }
    errorCode = rangeProcHWA_ConfigEDMATranspose(DPParams,
                                        handle,
                                        &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                                        &chainingCfg,
                                        hwaOutPongOffset,
                                        (uint32_t)(rangeProcObj->radarCubebuf + DPParams->numRxAntennas),
                                        true,
                                        rangeProcHWA_EDMA_transferCompletionCallbackFxn,  
                                        (uintptr_t)rangeProcObj);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

     /**************************************************************************
      *  HWA hot signature EDMA, chained to the transpose EDMA channels
      *************************************************************************/
    errorCode = DPEDMAHWA_configTwoHotSignature(handle, 
                                                  &pHwConfig->edmaOutCfg.dataOutSignature,
                                                  rangeProcObj->initParms.hwaHandle,
                                                  rangeProcObj->dataOutTrigger[0],
                                                  rangeProcObj->dataOutTrigger[1],
                                                  false);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      EDMA configuration for rangeProc data output in non-interleave mode
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  DPParams                  Pointer to datapath parameter
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *  @param[in]  hwaOutPingOffset          Ping HWA memory address offset
 *  @param[in]  hwaOutPongOffset          Pong HWA memory address offset
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_DataOut_nonInterleave
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t                hwaOutPingOffset,
    uint32_t                hwaOutPongOffset
)
{
    int32_t                 errorCode = EDMA_NO_ERROR;
    EDMA_Handle             handle;
    DPEDMA_syncABCfg        syncABCfg;
    DPEDMA_ChainingCfg      chainingCfg;

    /* Get rangeProc Configuration */
    handle = rangeProcObj->edmaHandle;

    /* Chaining configuration for all cases -> chaining to the data out signature channel */
    chainingCfg.chainingChan = pHwConfig->edmaOutCfg.dataOutSignature.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled = true;

     /**************************************************************************
      *  Configure EDMA to copy from HWA memory to radar cube 
      *************************************************************************/
    if(rangeProcObj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
    {
        /* Ping/Pong common configuration */
        syncABCfg.aCount = DPParams->numRxAntennas * sizeof(uint32_t);
        syncABCfg.bCount = DPParams->numRangeBins;
        syncABCfg.cCount = DPParams->numChirpsPerFrame/2U;
        syncABCfg.srcBIdx = DPParams->numRxAntennas * sizeof(uint32_t);
        syncABCfg.srcCIdx = 0U;
        syncABCfg.dstBIdx = DPParams->numRxAntennas * sizeof(uint32_t) *DPParams->numChirpsPerFrame;
        syncABCfg.dstCIdx = DPParams->numRxAntennas * 2U * sizeof(uint32_t);

        /* Ping specific config */
        syncABCfg.srcAddress = hwaOutPingOffset;
        syncABCfg.destAddress= (uint32_t)rangeProcObj->radarCubebuf;

        errorCode = DPEDMA_configSyncAB(handle,
                &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                &chainingCfg,
                &syncABCfg,
                true,    /* isEventTriggered */
                false,   /* isIntermediateTransferCompletionEnabled */
                false,   /* isTransferCompletionEnabled */
                NULL,
                NULL);

        if (errorCode != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* Pong specific configuration */
        syncABCfg.srcAddress = hwaOutPongOffset;
        syncABCfg.destAddress= (uint32_t)(rangeProcObj->radarCubebuf + DPParams->numRxAntennas);

        errorCode = DPEDMA_configSyncAB(handle,
                &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                &chainingCfg,
                &syncABCfg,
                true,   /* isEventTriggered */
                false,  /* isIntermediateTransferCompletionEnabled */
                true,   /* isTransferCompletionEnabled */
                rangeProcHWA_EDMA_transferCompletionCallbackFxn,
                (uintptr_t)rangeProcObj);
        if (errorCode != EDMA_NO_ERROR)
        {
            goto exit;
        }
    }
    else
    {
        uint32_t    numSamplePerTx;

        numSamplePerTx = DPParams->numRangeBins  *DPParams->numRxAntennas   * DPParams->numDopplerChirps;

        if (DPParams->numTxAntennas == 3U)
        {

            /**************************************************************************
            *  Configure EDMA to copy HWA results to radar cube 
            *  For cases with 3 TX Antenna
            *************************************************************************/
            DPEDMA_syncACfg        syncACfg;
            uint32_t    numSamplePerChirp;
            uint32_t    numBytesPerChirp;
            uint32_t    destAddr[2][3];
            uint8_t     index;
            bool        lastChan = false;

            numSamplePerChirp = DPParams->numRangeBins  *DPParams->numRxAntennas;
            numBytesPerChirp = numSamplePerChirp * sizeof(uint32_t);

            destAddr[0][0] = (uint32_t)rangeProcObj->radarCubebuf;
            destAddr[0][1] = (uint32_t)(rangeProcObj->radarCubebuf + 2 * numSamplePerTx);
            destAddr[0][2] = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerTx + numSamplePerChirp);
            destAddr[1][0] = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerTx);
            destAddr[1][1] = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerChirp);
            destAddr[1][2] = (uint32_t)(rangeProcObj->radarCubebuf + 2U * numSamplePerTx + numSamplePerChirp);

            /* Desitnation EDMA */
            /* Ping configuration to transfer 1D FFT output from HWA to L3 RAM transposed */
            errorCode = rangeProcHWA_ConfigEDMADummyThreeLinks(handle,
                &pHwConfig->edmaOutCfg.u.fmt2.dataOutPing,
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[0].channel,
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[1].channel,
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[2].channel,
                DPParams->numChirpsPerFrame/2U,
                NULL,
                NULL);
            if (errorCode != EDMA_NO_ERROR)
            {
                goto exit;
            }

            /* Desitnation EDMA */
            /* Pong configuration to transfer 1D FFT output from HWA to L3 RAM transposed */
            errorCode = rangeProcHWA_ConfigEDMADummyThreeLinks(handle,
                &pHwConfig->edmaOutCfg.u.fmt2.dataOutPong,
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[0].channel,
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[1].channel,
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[2].channel,
                DPParams->numChirpsPerFrame/2U,
                NULL,
                NULL);
            if (errorCode != EDMA_NO_ERROR)
            {
                goto exit;
            }

            /* Ping/Pong common configuration */
            syncACfg.aCount = numBytesPerChirp;
            syncACfg.bCount = DPParams->numChirpsPerFrame/6U;
            syncACfg.srcBIdx = 0U;
            syncACfg.dstBIdx = numBytesPerChirp * 2U;

            for(index=0;index < 3; index++)
            {
                if(index == 2)
                {
                    /* Set last channel flag to enable completion flag */
                    lastChan = true;
                }

                /* Configure 3 EDMA channels to copy data from M2 to one of TX antenna radar Cube */
                /* PING specific config 
                   M2 - >Txi (i=0,2, 1) */
                syncACfg.srcAddress = hwaOutPingOffset;
                syncACfg.destAddress= destAddr[0][index];

                errorCode = DPEDMA_configSyncA_singleFrame(handle,
                            &pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index],
                            &chainingCfg,
                            &syncACfg,
                            false,      /* isEventTriggered */
                            false,      /* isIntermediateTransferInterruptEnabled */
                            lastChan,   /* isTransferCompletionEnabled */
                            NULL,
                            NULL);

                /* PONG specific config 
                   M3 - >Txi (i=1,0, 2) */
                syncACfg.srcAddress = hwaOutPongOffset;
                syncACfg.destAddress= destAddr[1][index];

                errorCode = DPEDMA_configSyncA_singleFrame(handle,
                            &pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index],
                            &chainingCfg,
                            &syncACfg,
                            false,      /* isEventTriggered */
                            false,      /* isIntermediateTransferInterruptEnabled */
                            lastChan,   /* isTransferCompletionEnabled */
                            (lastChan ==true)? rangeProcHWA_EDMA_transferCompletionCallbackFxn: NULL,
                            (lastChan ==true)? (uintptr_t)rangeProcObj:(uintptr_t)NULL);

            }
        }
        else if(DPParams->numTxAntennas == 2U)
        {
            /**************************************************************************
            *  Configure EDMA to copy HWA results to radar cube 
            *  For cases with 2 TX Antenna
            *************************************************************************/
            /* Ping/Pong common configuration */
            syncABCfg.aCount = DPParams->numRangeBins * sizeof(uint32_t);
            syncABCfg.bCount = DPParams->numRxAntennas;
            syncABCfg.cCount = DPParams->numChirpsPerFrame/2U;
            syncABCfg.srcBIdx = DPParams->numRangeBins * sizeof(uint32_t);
            syncABCfg.srcCIdx = 0U;
            syncABCfg.dstBIdx = DPParams->numRangeBins * sizeof(uint32_t);
            syncABCfg.dstCIdx = DPParams->numRxAntennas * DPParams->numRangeBins * sizeof(uint32_t);

            /* Ping specific config */
            syncABCfg.srcAddress = hwaOutPingOffset;
            syncABCfg.destAddress= (uint32_t)rangeProcObj->radarCubebuf;

            errorCode = DPEDMA_configSyncAB(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                    &chainingCfg,
                    &syncABCfg,
                    true,    /* isEventTriggered */
                    false,   /* isIntermediateTransferCompletionEnabled */
                    false,   /* isTransferCompletionEnabled */
                    NULL,
                    NULL);

            if (errorCode != EDMA_NO_ERROR)
            {
                goto exit;
            }

            /* Pong specific configuration */
            syncABCfg.srcAddress = hwaOutPongOffset;
            syncABCfg.destAddress= (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerTx);

            errorCode = DPEDMA_configSyncAB(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                    &chainingCfg,
                    &syncABCfg,
                    true,   /* isEventTriggered */
                    false,  /* isIntermediateTransferCompletionEnabled */
                    true,   /*isTransferCompletionEnabled */
                    rangeProcHWA_EDMA_transferCompletionCallbackFxn,  
                    (uintptr_t)rangeProcObj);
            if (errorCode != EDMA_NO_ERROR)
            {
                goto exit;
            }
        }
        else if(DPParams->numTxAntennas == 1U)
        {
            uint32_t    numSamplePerChirp;

            numSamplePerChirp = DPParams->numRangeBins  *DPParams->numRxAntennas  ;

            /**************************************************************************
            *  Configure EDMA to copy HWA results to radar cube 
            *  For cases with 1 TX Antenna
            *************************************************************************/
            DPEDMA_syncACfg        syncACfg;

            /* Ping/Pong common configuration */
            syncACfg.aCount = numSamplePerChirp * sizeof(cmplx16ImRe_t);
            syncACfg.bCount = DPParams->numChirpsPerFrame/2U;
            syncACfg.srcBIdx = 0;
            syncACfg.dstBIdx = numSamplePerChirp * sizeof(cmplx16ImRe_t) * 2U;

            /* Ping specific config */
            syncACfg.srcAddress = hwaOutPingOffset;
            syncACfg.destAddress= (uint32_t)rangeProcObj->radarCubebuf;

            errorCode = DPEDMA_configSyncA_singleFrame(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                    &chainingCfg,
                    &syncACfg,
                    true,    /*isEventTriggered */
                    false,   /*isIntermediateTransferCompletionEnabled */
                    false,   /*isTransferCompletionEnabled */
                    NULL,
                    NULL);

            if (errorCode != EDMA_NO_ERROR)
            {
                goto exit;
            }

            /* Pong specific configuration */
            syncACfg.srcAddress = hwaOutPongOffset;
            syncACfg.destAddress= (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerChirp);

            errorCode = DPEDMA_configSyncA_singleFrame(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                    &chainingCfg,
                    &syncACfg,
                    true,   /*isEventTriggered */
                    false,  /* isIntermediateTransferCompletionEnabled */
                    true,   /*isTransferCompletionEnabled */
                    rangeProcHWA_EDMA_transferCompletionCallbackFxn,  
                    (uintptr_t)rangeProcObj);
            if (errorCode != EDMA_NO_ERROR)
            {
                goto exit;
            }
        }
        else
        {
            /* Fall through */
        }
    }

    /**************************************************************************
    *  HWA hot signature EDMA, chained to the transpose EDMA channels
    *************************************************************************/
    errorCode = DPEDMAHWA_configTwoHotSignature(handle, 
                                                  &pHwConfig->edmaOutCfg.dataOutSignature,
                                                  rangeProcObj->initParms.hwaHandle,
                                                  rangeProcObj->dataOutTrigger[0],
                                                  rangeProcObj->dataOutTrigger[1],
                                                  false);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      EDMA configuration for rangeProc data in when EDMA is used to copy data from 
 *  ADCBuf to HWA memory
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object handle
 *  @param[in]  DPParams                  Pointer to datapath parameter
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_DataIn
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    int32_t             errorCode = EDMA_NO_ERROR;
    EDMA_Handle         handle ;
    uint16_t            bytePerRxChan;
    DPEDMA_ChainingCfg  chainingCfg;

    /* Get rangeProc Configuration */
    handle = rangeProcObj->edmaHandle;

    bytePerRxChan = DPParams->numAdcSamples * sizeof(cmplx16ImRe_t);

    /**********************************************/
    /* ADCBuf -> Ping/Pong Buffer(M0 and M1)           */
    /**********************************************/
    chainingCfg.chainingChan = pHwConfig->edmaInCfg.dataInSignature.channel;
    chainingCfg.isFinalChainingEnabled = true;
    chainingCfg.isIntermediateChainingEnabled = true;

    if (rangeProcObj->interleave == DPIF_RXCHAN_NON_INTERLEAVE_MODE)
    {
        DPEDMA_syncABCfg    syncABCfg;

        syncABCfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf;
        syncABCfg.destAddress = rangeProcObj->hwaMemBankAddr[0];

        syncABCfg.aCount = bytePerRxChan;
        syncABCfg.bCount = DPParams->numRxAntennas;
        syncABCfg.cCount =2U; /* ping and pong */

        syncABCfg.srcBIdx=rangeProcObj->rxChanOffset;
        syncABCfg.dstBIdx=rangeProcObj->rxChanOffset;
        syncABCfg.srcCIdx=0U;
        syncABCfg.dstCIdx=((uint32_t)rangeProcObj->hwaMemBankAddr[1] - (uint32_t)rangeProcObj->hwaMemBankAddr[0]);

        errorCode = DPEDMA_configSyncAB(handle,
                                        &pHwConfig->edmaInCfg.dataIn,
                                        &chainingCfg,
                                        &syncABCfg,
                                        true,    /* isEventTriggered */
                                        false,   /* isIntermediateTransferInterruptEnabled */
                                        false,   /*isFinalTransferInterruptEnabled */
                                        NULL,
                                        (uintptr_t)NULL);
    }
    else
    {
        DPEDMA_syncACfg    syncACfg;

        syncACfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf;
        syncACfg.destAddress = rangeProcObj->hwaMemBankAddr[0];
        syncACfg.aCount = bytePerRxChan * DPParams->numRxAntennas;
        syncACfg.bCount =2U; /* ping and pong */
        syncACfg.srcBIdx=0U;
        syncACfg.dstBIdx=((uint32_t)rangeProcObj->hwaMemBankAddr[1] - (uint32_t)rangeProcObj->hwaMemBankAddr[0]);

        errorCode = DPEDMA_configSyncA_singleFrame(handle,
                                        &pHwConfig->edmaInCfg.dataIn,
                                        &chainingCfg,
                                        &syncACfg,
                                        true,    /* isEventTriggered */
                                        false,   /* isIntermediateTransferInterruptEnabled */
                                        false,   /* isFinalTransferInterruptEnabled */
                                        NULL,
                                        (uintptr_t)NULL);
    }

    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /*************************************************/
    /* Generate Hot Signature to trigger Ping/Pong paramset   */
    /*************************************************/

    errorCode = DPEDMAHWA_configTwoHotSignature(handle, 
                                                  &pHwConfig->edmaInCfg.dataInSignature,
                                                  rangeProcObj->initParms.hwaHandle,
                                                  rangeProcObj->dataInTrigger[0],
                                                  rangeProcObj->dataInTrigger[1],
                                                  false);

    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }
exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      rangeProc configuraiton in interleaved mode
 *
 *  @param[in]  rangeProcObj                 Pointer to rangeProc object
 *  @param[in]  DPParams                     Pointer to data path common params
 *  @param[in]  pHwConfig                    Pointer to rangeProc hardware resources
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConifgInterleaveMode
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    int32_t             retVal = 0;
    uint8_t             destChanPing;
    uint8_t             destChanPong;
    HWA_Handle          hwaHandle;

    hwaHandle = rangeProcObj->initParms.hwaHandle;

    /* In interleave mode, only edmaOutCfgFmt is supported */
    retVal = HWA_getDMAChanIndex(hwaHandle, pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel, &destChanPing);
    if (retVal != 0)
    {
        goto exit;
    }

    /* In interleave mode, only edmaOutCfgFmt is supported */
    retVal = HWA_getDMAChanIndex(hwaHandle, pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel, &destChanPong);
    if (retVal != 0)
    {
        goto exit;
    }

    if(pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_ISOLATED)
    {
        /* Copy data from ADC buffer to HWA buffer */
        rangeProcHWA_ConfigEDMA_DataIn(rangeProcObj,    DPParams, pHwConfig);

        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            destChanPing,
            destChanPong,
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[0]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[1]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[2]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[3])
        );
        if(retVal < 0)
        {
            goto exit;
        }
    }
    else
    {
        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
                destChanPing,
                destChanPong,
                ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[0]),
                ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[0]),
                ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[2]),
                ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[3])
        );
        if(retVal < 0)
        {
            goto exit;
        }
    }

    /* EDMA configuration */
    retVal = rangeProcHWA_ConfigEDMA_DataOut_interleave(rangeProcObj,
                                                  DPParams,
                                                  pHwConfig,
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[2],
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[3]);
exit:
    return (retVal);
}



/**
 *  @b Description
 *  @n
 *      rangeProc configuraiton in non-interleaved mode
 *
 *  @param[in]  rangeProcObj                 Pointer to rangeProc object
 *  @param[in]  DPParams                     Pointer to data path common params
 *  @param[in]  pHwConfig                    Pointer to rangeProc hardware resources
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConifgNonInterleaveMode
(
    rangeProcHWAObj          *rangeProcObj,
    rangeProc_dpParams       *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    HWA_Handle          hwaHandle;
    int32_t             retVal = 0;
    uint8_t             destChanPing;
    uint8_t             destChanPong;
    uint8_t             edmaChanPing;
    uint8_t             edmaChanPong;

    hwaHandle = rangeProcObj->initParms.hwaHandle;


    if((rangeProcObj->radarCubeLayout == rangeProc_dataLayout_TxAnt_DOPPLER_RxAnt_RANGE) &&
        (DPParams->numTxAntennas == 3U) )
    {
        edmaChanPing = pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.channel;
        edmaChanPong = pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.channel;
    }
    else
    {
        edmaChanPing = pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel;
        edmaChanPong = pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel;
    }

    /* Get HWA destination channel id */
    retVal = HWA_getDMAChanIndex(hwaHandle, edmaChanPing, &destChanPing);
    if (retVal != 0)
    {
        goto exit;
    }
    /* In interleave mode, only edmaOutCfgFmt is supported */
    retVal = HWA_getDMAChanIndex(hwaHandle, edmaChanPong, &destChanPong);
    if (retVal != 0)
    {
        goto exit;
    }

    /* In ADCBuf and HWA memory isolated mode, 
       - copy data from ADCBuf to HWA memory by EDMA 
       - trigger HWA */
    if(pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_ISOLATED)
    {
        /* Copy data from ADC buffer to HWA buffer */
        rangeProcHWA_ConfigEDMA_DataIn(rangeProcObj,    DPParams, pHwConfig);

        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            destChanPing,
            destChanPong,
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[0]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[1]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[2]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[3])
        );
        if(retVal < 0)
        {
            goto exit;
        }
    }
    else
    {
        /* EDMA copy is not needed */

        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            destChanPing,
            destChanPong,
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[0]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[0]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[2]),
            ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[3])
        );
        if(retVal < 0)
        {
            goto exit;
        }
    }

    /* Data output EDMA configuration */
    retVal = rangeProcHWA_ConfigEDMA_DataOut_nonInterleave(rangeProcObj,
                                             DPParams,
                                             pHwConfig,
                                             (uint32_t)rangeProcObj->hwaMemBankAddr[2],
                                             (uint32_t)rangeProcObj->hwaMemBankAddr[3]);
    if (retVal < 0)
    {
        goto exit;
    }

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Compensation of DC range antenna signature Init function
 *
 *  @param[in]  rangeProcObj                 Pointer to rangeProc object
 *  @param[in]  calibDcRangeSigCfg           Pointer DC range compensation configuration
 *  @param[in]  resetMeanBuffer              Flag to indicate if buffer need to be reset
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_dcRangeSignatureCompensation_init
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcRangeSigCfg,
    uint8_t             resetMeanBuffer
)
{
    int32_t                 retVal = 0;
    uint32_t                meanbufSize;

    meanbufSize = DPU_RANGEPROC_SIGNATURE_COMP_MAX_BIN_SIZE * rangeProcObj->params.numVirtualAntennas
                 * sizeof(cmplx32ImRe_t);

    /* Validate DC revomal configuraiton */
    if(calibDcRangeSigCfg->enabled)
    {
        if(rangeProcObj->dcRangeSigMean == (cmplx32ImRe_t*)NULL)
        {
            /* Check DC range average buffer pointer */
            retVal = DPU_RANGEPROCHWA_EDCREMOVAL;
            goto exit;
        }
        else if(meanbufSize > rangeProcObj->dcRangeSigMeanSize)
        {
            /* Check DC range average buffer pointer */
            retVal = DPU_RANGEPROCHWA_EDCREMOVAL;
            goto exit;
        }
        else
        {
            /* Initialize memory */
            if (resetMeanBuffer == 1U)
            {
                memset((void *)rangeProcObj->dcRangeSigMean, 0, meanbufSize);
                rangeProcObj->dcRangeSigCalibCntr = 0;
            }
            rangeProcObj->calibDcNumLog2AvgChirps = mathUtils_floorLog2(calibDcRangeSigCfg->numAvgChirps);
        }
    }
    else
    {
        /* Feature is disabled , nothing needs to done here */
    }

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Compensation of DC range antenna signature
 *
 *  @param[in]  obj                 Pointer to rangeProc object
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
static void rangeProcHWA_dcRangeSignatureCompensation
(
    rangeProcHWAObj     *obj
)
{
    uint32_t    antIdx, rngBinIdx;
    uint32_t    ind;
    int32_t     dcRangeSigMeanSize;
    uint32_t    doppIdx;
    cmplx16ImRe_t *fftOut1D;
    DPU_RangeProc_CalibDcRangeSigCfg *calibDc = &obj->calibDcRangeSigCfg;
    int32_t     negativeBinIdx = calibDc->negativeBinIdx;
    int32_t     positiveBinIdx = calibDc->positiveBinIdx;
    cmplx32ImRe_t *dcRangeSigMean = obj->dcRangeSigMean;
    rangeProc_dpParams   *dpParams;

    dpParams    = &obj->params;
    fftOut1D    = (cmplx16ImRe_t *) obj->radarCubebuf;

    dcRangeSigMeanSize = dpParams->numVirtualAntennas * (positiveBinIdx - negativeBinIdx + 1);
    if (obj->dcRangeSigCalibCntr == 0)
    {
        memset(dcRangeSigMean, 0, dcRangeSigMeanSize * sizeof(cmplx32ImRe_t));
    }

    /* RadarCube FMT2 */
    if(obj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
    {
        int32_t     rngOffset;
        int32_t     numVirtAnt;

        numVirtAnt = dpParams->numVirtualAntennas;

        rngOffset = dpParams->numVirtualAntennas * dpParams->numDopplerChirps;
        /* Calibration */
        if (obj->dcRangeSigCalibCntr < calibDc->numAvgChirps)
        {
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                /* Accumulate */
                ind = 0;
                for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                {
                    for (antIdx  = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        int32_t binIdx = rngBinIdx * rngOffset + doppIdx * numVirtAnt + antIdx;
                        dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                        dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                        ind++;
                    }
                }
                for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                {
                    for (antIdx  = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        int32_t binIdx = (dpParams->numRangeBins + negativeBinIdx + rngBinIdx) * rngOffset +
                                          doppIdx * numVirtAnt + antIdx;
                        dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                        dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                        ind++;
                    }
                }
                obj->dcRangeSigCalibCntr++;
            }

            if (obj->dcRangeSigCalibCntr == calibDc->numAvgChirps)
            {
                /* Divide */
                for (ind  = 0; ind < dcRangeSigMeanSize; ind++)
                {
                    dcRangeSigMean[ind].real = dcRangeSigMean[ind].real >> obj->calibDcNumLog2AvgChirps;
                    dcRangeSigMean[ind].imag = dcRangeSigMean[ind].imag >> obj->calibDcNumLog2AvgChirps;
                }
            }
        }
        else
        {
            /* fftOut1D -= dcRangeSigMean */
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                ind = 0;
                for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                {
                    for (antIdx = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        int32_t binIdx = rngBinIdx * rngOffset + doppIdx * numVirtAnt + antIdx;

                        fftOut1D[binIdx].real -= dcRangeSigMean[ind].real;
                        fftOut1D[binIdx].imag -= dcRangeSigMean[ind].imag;
                        ind++;
                    }
                }
                for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                {
                    for (antIdx = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        int32_t binIdx = (dpParams->numRangeBins + negativeBinIdx + rngBinIdx) * rngOffset +
                                         doppIdx * numVirtAnt + antIdx;
                        fftOut1D[binIdx].real -= dcRangeSigMean[ind].real;
                        fftOut1D[binIdx].imag -= dcRangeSigMean[ind].imag;
                        ind++;
                    }
                }
            }
         }
    }
    else
    {
        /* RadarCube FMT1 */
        uint32_t    txAntIdx;
        uint32_t    rxAntIdx;
        int32_t     txAntOffset;
        int32_t     dopOffset;

        /* FMT1: cmplx16ImRe_t x[numTXPatterns][numDopplerChirps][numRX][numRangeBins] |1D Range FFT output*/
        txAntOffset = dpParams->numRxAntennas * dpParams->numDopplerChirps * dpParams->numRangeBins;
        dopOffset  = dpParams->numRxAntennas * dpParams->numRangeBins;

        /* Calibration */
        if (obj->dcRangeSigCalibCntr < calibDc->numAvgChirps)
        {
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                /* Accumulate for all doppler bins */
                ind = 0;

                for(txAntIdx = 0; txAntIdx< dpParams->numTxAntennas; txAntIdx++)
                {
                    for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            int32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins *rxAntIdx +
                                          rngBinIdx;
                            dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                            dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                            ind++;
                        }
                    }
                    for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            int32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins *rxAntIdx +
                                          (dpParams->numRangeBins + negativeBinIdx + rngBinIdx);

                            dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                            dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                            ind++;
                        }
                    }
                }
                obj->dcRangeSigCalibCntr++;
            }

            if (obj->dcRangeSigCalibCntr == calibDc->numAvgChirps)
            {
                /* Divide */
                for (ind = 0; ind < dcRangeSigMeanSize; ind++)
                {
                    dcRangeSigMean[ind].real = dcRangeSigMean[ind].real >> obj->calibDcNumLog2AvgChirps;
                    dcRangeSigMean[ind].imag = dcRangeSigMean[ind].imag >> obj->calibDcNumLog2AvgChirps;
                }
            }
        }
        else
        {
            /* fftOut1D -= dcRangeSigMean */
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                ind = 0;
                for(txAntIdx = 0; txAntIdx< dpParams->numTxAntennas; txAntIdx++)
                {
                    for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            int32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins *rxAntIdx +
                                          rngBinIdx;
                            fftOut1D[binIdx].real -= dcRangeSigMean[ind].real;
                            fftOut1D[binIdx].imag -= dcRangeSigMean[ind].imag;
                            ind++;
                        }
                    }
                    for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            int32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins *rxAntIdx +
                                          (dpParams->numRangeBins + negativeBinIdx + rngBinIdx);
                            fftOut1D[binIdx].real -= dcRangeSigMean[ind].real;
                            fftOut1D[binIdx].imag -= dcRangeSigMean[ind].imag;
                            ind++;
                        }
                    }
                }
            }
         }
    }
}
/**
 *  @b Description
 *  @n
 *      Internal function to parse rangeProc configuration and save in internal rangeProc object
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  pConfigIn                 Pointer to rangeProcHWA configuration structure
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ParseConfig
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config  *pConfigIn
)
{
    int32_t                 retVal = 0;
    rangeProc_dpParams      *params;
    DPU_RangeProcHWA_StaticConfig   *pStaticCfg;

    /* Get configuration pointers */
    pStaticCfg = &pConfigIn->staticCfg;
    params    = &rangeProcObj->params;

    /* Save datapath parameters */
    params->numTxAntennas = pStaticCfg->numTxAntennas;
    params->numRxAntennas = pStaticCfg->ADCBufData.dataProperty.numRxAntennas;
    params->numVirtualAntennas = pStaticCfg->numVirtualAntennas;
    params->numChirpsPerChirpEvent = pStaticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent;
    params->numAdcSamples = pStaticCfg->ADCBufData.dataProperty.numAdcSamples;
    params->numRangeBins = pStaticCfg->numRangeBins;
    params->numChirpsPerFrame = pStaticCfg->numChirpsPerFrame;
    params->numDopplerChirps = pStaticCfg->numChirpsPerFrame/pStaticCfg->numTxAntennas;

    /* Save buffers */
    rangeProcObj->ADCdataBuf        = (cmplx16ImRe_t *)pStaticCfg->ADCBufData.data;
    rangeProcObj->radarCubebuf      = (cmplx16ImRe_t *)pConfigIn->hwRes.radarCube.data;

    /* Save interleave mode from ADCBuf configuraiton */
    rangeProcObj->interleave = pStaticCfg->ADCBufData.dataProperty.interleave;

    if((rangeProcObj->interleave ==DPIF_RXCHAN_NON_INTERLEAVE_MODE) &&
        (rangeProcObj->params.numRxAntennas > 1) )
    {
        /* For rangeProcDPU needs rx channel has same offset from one channel to the next channel
           Use first two channel offset to calculate the BIdx for EDMA
         */
        rangeProcObj->rxChanOffset = pStaticCfg->ADCBufData.dataProperty.rxChanOffset[1] - 
                                    pStaticCfg->ADCBufData.dataProperty.rxChanOffset[0];

        /* rxChanOffset should be 16 bytes aligned and should be big enough to hold numAdcSamples */
        if ((rangeProcObj->rxChanOffset < (rangeProcObj->params.numAdcSamples * sizeof(cmplx16ImRe_t))) ||
          ((rangeProcObj->rxChanOffset & 0xF) != 0))
        {
            retVal = DPU_RANGEPROCHWA_EADCBUF_INTF;
            goto exit;
        }
    }

    /* Save RadarCube format */
    if (pConfigIn->hwRes.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_2)
    {
        rangeProcObj->radarCubeLayout = rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt;
    }
    else if(pConfigIn->hwRes.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_1)
    {
        rangeProcObj->radarCubeLayout = rangeProc_dataLayout_TxAnt_DOPPLER_RxAnt_RANGE;
    }
    else
    {
        retVal = DPU_RANGEPROCHWA_EINTERNAL;
        goto exit;
    }

    /* The following case can not be handled with the current 1TX EDMA scheme, reason is the Bindex exceeds what EDMA can handle. */
    if( (params->numRangeBins == 1024U) &&
       (params->numTxAntennas == 1U) &&
       (params->numRxAntennas == 4U) &&
       (rangeProcObj->radarCubeLayout == rangeProc_dataLayout_TxAnt_DOPPLER_RxAnt_RANGE) )
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }

    /* The following case can not be handled with the current 3TX EDMA scheme, reason is the Bindex exceeds what EDMA(jump index<32768) can handle. */
    if( (params->numRangeBins == 1024U) &&
       (params->numTxAntennas == 3U) &&
       (params->numRxAntennas == 4U) &&
       (rangeProcObj->radarCubeLayout == rangeProc_dataLayout_TxAnt_DOPPLER_RxAnt_RANGE) )
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }

    /* Prepare internal hardware resouces = trigger source matchs its  paramset index */
    rangeProcObj->dataInTrigger[0]      = 1U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataInTrigger[1]      = 3U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataOutTrigger[0]     = 0U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataOutTrigger[1]     = 2U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;

    /* Save hardware resources that will be used at runtime */
    rangeProcObj->edmaHandle= pConfigIn->hwRes.edmaHandle;
    rangeProcObj->dataOutSignatureChan = pConfigIn->hwRes.edmaOutCfg.dataOutSignature.channel;
    rangeProcObj->dcRangeSigMean = pConfigIn->hwRes.dcRangeSigMean;
    rangeProcObj->dcRangeSigMeanSize = pConfigIn->hwRes.dcRangeSigMeanSize;
    memcpy((void *)&rangeProcObj->hwaCfg, (void *)&pConfigIn->hwRes.hwaCfg, sizeof(DPU_RangeProcHWA_HwaConfig));

exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Internal function to config HWA/EDMA to perform range FFT
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_HardwareConfig
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    int32_t                 retVal = 0;
    rangeProc_dpParams      *DPParams;
    DPParams    = &rangeProcObj->params;

    if (rangeProcObj->interleave == DPIF_RXCHAN_INTERLEAVE_MODE)
    {
        retVal = rangeProcHWA_ConifgInterleaveMode(rangeProcObj, DPParams, pHwConfig);
        if (retVal != 0)
        {
            goto exit;
        }
    }
    else
    {
        retVal =rangeProcHWA_ConifgNonInterleaveMode(rangeProcObj, DPParams, pHwConfig);
        if (retVal != 0)
        {
            goto exit;
        }
    }
exit:
    return(retVal);
}

/**************************************************************************
 ************************RangeProcHWA External APIs **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU init function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]  initParams              Pointer to DPU init parameters
 *  @param[in]  errCode                 Pointer to errCode generates from the API
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid rangeProc handle
 *  @retval
 *      Error       - NULL
 */
DPU_RangeProcHWA_Handle DPU_RangeProcHWA_init
(
    DPU_RangeProcHWA_InitParams     *initParams,
    int32_t*                        errCode
)
{
    rangeProcHWAObj     *rangeProcObj = NULL;
    SemaphoreP_Params   semParams;
    HWA_MemInfo         hwaMemInfo;
    uint8_t             index;

    *errCode = 0;

    if( (initParams == NULL) ||
       (initParams->hwaHandle == NULL) )
    {
        *errCode = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Allocate Memory for rangeProc */
    rangeProcObj = MemoryP_ctrlAlloc(sizeof(rangeProcHWAObj), 0);
    if(rangeProcObj == NULL)
    {
        *errCode = DPU_RANGEPROCHWA_ENOMEM;
        goto exit;
    }

    /* Initialize memory */
    memset((void *)rangeProcObj, 0, sizeof(rangeProcHWAObj));

    memcpy((void *)&rangeProcObj->initParms, initParams, sizeof(DPU_RangeProcHWA_InitParams));

    /* Set HWA bank memory address */
    *errCode =  HWA_getHWAMemInfo(initParams->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    for (index = 0; index < hwaMemInfo.numBanks; index++)
    {
        rangeProcObj->hwaMemBankAddr[index] = hwaMemInfo.baseAddress + index * hwaMemInfo.bankSize;
    }

    /* Create semaphore for EDMA done */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    rangeProcObj->edmaDoneSemaHandle = SemaphoreP_create(0, &semParams);
    if(rangeProcObj->edmaDoneSemaHandle == NULL)
    {
        *errCode = DPU_RANGEPROCHWA_ESEMA;
        goto exit;
    }

    /* Create semaphore for HWA done */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    rangeProcObj->hwaDoneSemaHandle = SemaphoreP_create(0, &semParams);
    if(rangeProcObj->hwaDoneSemaHandle == NULL)
    {
        *errCode = DPU_RANGEPROCHWA_ESEMA;
        goto exit;
    }

exit:
    if(*errCode < 0)
    {
        if(rangeProcObj != NULL)
        {
            MemoryP_ctrlFree(rangeProcObj, sizeof(rangeProcHWAObj));
        }

        rangeProcObj = (DPU_RangeProcHWA_Handle)NULL;
    }
    else
    {
        /* Fall through */
    }
    return ((DPU_RangeProcHWA_Handle)rangeProcObj);

}


/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU config function. It saves buffer pointer and configurations 
 *  including system resources and configures HWA and EDMA for runtime range processing.
 *  
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle                  rangeProc DPU handle
 *  @param[in]  pConfigIn               Pointer to rangeProc configuration data structure
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_config
(
    DPU_RangeProcHWA_Handle  handle,
    DPU_RangeProcHWA_Config  *pConfigIn
)
{
    rangeProcHWAObj                 *rangeProcObj;
    DPU_RangeProcHWA_StaticConfig   *pStaticCfg;
    HWA_Handle                      hwaHandle;
    int32_t                         retVal = 0;

    rangeProcObj = (rangeProcHWAObj *)handle;
    if(rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Get configuration pointers */
    pStaticCfg = &pConfigIn->staticCfg;
    hwaHandle = rangeProcObj->initParms.hwaHandle;

#if DEBUG_CHECK_PARAMS
    /* Validate params */
    if(!pConfigIn ||
      !(pConfigIn->hwRes.edmaHandle) ||
       (pConfigIn->hwRes.hwaCfg.numParamSet != DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS)
      )
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Parameter check: validate Adc data interface configuration
        Support:
            - 1 chirp per chirpEvent
            - Complex 16bit ADC data in IMRE format
     */
    if( (pStaticCfg->ADCBufData.dataProperty.dataFmt != DPIF_DATAFORMAT_COMPLEX16_IMRE) ||
       (pStaticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent != 1U) )
    {
        retVal = DPU_RANGEPROCHWA_EADCBUF_INTF;
        goto exit;
    }

    /* Parameter check: windowing Size */
    {
        uint16_t expectedWinSize;

        if( pConfigIn->hwRes.hwaCfg.hwaWinSym == HWA_FFT_WINDOW_SYMMETRIC)
        {
            /* Only half of the windowing factor is needed for symmetric window */
            expectedWinSize = ((pStaticCfg->ADCBufData.dataProperty.numAdcSamples + 1U) / 2U ) * sizeof(uint32_t);
        }
        else
        {
            expectedWinSize = pStaticCfg->ADCBufData.dataProperty.numAdcSamples * sizeof(uint32_t);
        }

        if(pStaticCfg->windowSize != expectedWinSize)
        {
            retVal = DPU_RANGEPROCHWA_EWINDOW;
            goto exit;
        }
    }

    /* Refer to radar cube definition for FORMAT_x , the following are the only supported formats
        Following assumption is made upon radar cube FORMAT_x definition 
           1. data type is complex in cmplx16ImRe_t format only
           2. It is always 1D range output.
     */
    if( (pConfigIn->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_1) &&
       (pConfigIn->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_2) )
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

    /* Not supported input & output format combination */
    if ((pStaticCfg->ADCBufData.dataProperty.interleave == DPIF_RXCHAN_INTERLEAVE_MODE) &&
         (pConfigIn->hwRes.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_1) )
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }
    if (pStaticCfg->ADCBufData.dataProperty.numRxAntennas == 3U)
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }

    /* Parameter check: radarcube buffer Size */
    if(pConfigIn->hwRes.radarCube.dataSize != (pStaticCfg->numRangeBins* sizeof(cmplx16ImRe_t) *
                                      pStaticCfg->numChirpsPerFrame *
                                      pStaticCfg->ADCBufData.dataProperty.numRxAntennas) )
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }
#endif

    /* Save hardware resources */
    memcpy((void *)&rangeProcObj->calibDcRangeSigCfg, (void *)pConfigIn->dynCfg.calibDcRangeSigCfg, sizeof(DPU_RangeProc_CalibDcRangeSigCfg));

    retVal = rangeProcHWA_ParseConfig(rangeProcObj, pConfigIn);
    if (retVal < 0)
    {
        goto exit;
    }

    /* DC calibration and compensation init */
    retVal = rangeProcHWA_dcRangeSignatureCompensation_init(rangeProcObj, pConfigIn->dynCfg.calibDcRangeSigCfg, pStaticCfg->resetDcRangeSigMeanBuffer);
    if (retVal < 0)
    {
        goto exit;
    }

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Reset the internal state of the HWA */
    retVal = HWA_reset(hwaHandle);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Windowing configuraiton in HWA */
    retVal = HWA_configRam(hwaHandle,
                            HWA_RAM_TYPE_WINDOW_RAM,
                            (uint8_t *)pStaticCfg->window,
                            pStaticCfg->windowSize,   /* size in bytes */
                            pConfigIn->hwRes.hwaCfg.hwaWinRamOffset * sizeof(uint32_t));
    if (retVal != 0)
    {
        goto exit;
    }

    /* Clear stats */
    rangeProcObj->numProcess = 0U;

    /* Initial configuration of rangeProc */
    retVal = rangeProcHWA_HardwareConfig(rangeProcObj, &pConfigIn->hwRes);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU process function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle                  rangeProc DPU handle
 *  @param[in]  outParams               DPU output parameters
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_process
(
    DPU_RangeProcHWA_Handle     handle,
    DPU_RangeProcHWA_OutParams  *outParams
)
{
    rangeProcHWAObj     *rangeProcObj;
    int32_t             retVal = 0;

    rangeProcObj = (rangeProcHWAObj *)handle;
    if ((rangeProcObj == NULL) ||
        (outParams == NULL))
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Set inProgress state */
    rangeProcObj->inProgress = true;
    outParams->endOfChirp = false;

    /**********************************************/
    /* WAIT FOR HWA NUMLOOPS INTERRUPT            */
    /**********************************************/
    /* wait for the all paramSets done interrupt */
    SemaphoreP_pend(rangeProcObj->hwaDoneSemaHandle, SemaphoreP_WAIT_FOREVER);

    /**********************************************/
    /* WAIT FOR EDMA INTERRUPT                    */
    /**********************************************/
    SemaphoreP_pend(rangeProcObj->edmaDoneSemaHandle, SemaphoreP_WAIT_FOREVER);

    /* Range FFT is done, disable Done interrupt */
    HWA_disableDoneInterrupt(rangeProcObj->initParms.hwaHandle);

    /* Disable the HWA */
    retVal = HWA_enable(rangeProcObj->initParms.hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    /* calib DC processing if enabled */
    if(rangeProcObj->calibDcRangeSigCfg.enabled)
    {
         rangeProcHWA_dcRangeSignatureCompensation(rangeProcObj);
    }
    /* Update stats and output parameters */
    rangeProcObj->numProcess++;

    /* Following stats is not available for rangeProcHWA */
    outParams->stats.processingTime = 0;
    outParams->stats.waitTime= 0;

    outParams->endOfChirp = true;

    /* Clear inProgress state */
    rangeProcObj->inProgress = false;

exit:

    return retVal;
}


/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU control function. 
 *
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle           rangeProc DPU handle
 *  @param[in]  cmd              rangeProc DPU control command
 *  @param[in]  arg              rangeProc DPU control argument pointer
 *  @param[in]  argSize          rangeProc DPU control argument size
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_control
(
    DPU_RangeProcHWA_Handle handle,
    DPU_RangeProcHWA_Cmd    cmd,
    void*                   arg,
    uint32_t                argSize
)
{
    int32_t             retVal = 0;
    rangeProcHWAObj     *rangeProcObj;

    /* Get rangeProc data object */
    rangeProcObj = (rangeProcHWAObj *)handle;

    /* Sanity check */
    if (rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Check if control() is called during processing time */
    if(rangeProcObj->inProgress == true)
    {
        retVal = DPU_RANGEPROCHWA_EINPROGRESS;
        goto exit;
    }

    /* Control command handling */
    switch(cmd)
    {
        case DPU_RangeProcHWA_Cmd_dcRangeCfg:
        {
            DPU_RangeProc_CalibDcRangeSigCfg   *calibDc;
            
            if((argSize != sizeof(DPU_RangeProc_CalibDcRangeSigCfg)) ||
               (arg == NULL))
            {
                retVal = DPU_RANGEPROCHWA_EDCREMOVAL;
                goto exit;
            }
            else
            {
                calibDc = (DPU_RangeProc_CalibDcRangeSigCfg *)arg;

                if(memcmp((void *)&rangeProcObj->calibDcRangeSigCfg,
                           (void *)calibDc, sizeof(DPU_RangeProc_CalibDcRangeSigCfg)) == 0) 
                {
                    /* NO configuration change, nothing needs to be done here */
                }
                else
                {
                    /* If dcRangeSignature configuration is changed , always reset buffer and counter */
                    retVal = rangeProcHWA_dcRangeSignatureCompensation_init(rangeProcObj, calibDc, 1U);
                    if(retVal < 0)
                    {
                        goto exit;
                    }
                    else
                    {
                        /* Save configuration */
                        memcpy((void *)&rangeProcObj->calibDcRangeSigCfg, (void *)arg, argSize);
                    }
                }
            }
        }
        break;

        case DPU_RangeProcHWA_Cmd_triggerProc:
            /* Trigger rangeProc in HWA */
            retVal = rangeProcHWA_TriggerHWA( rangeProcObj);
            if(retVal != 0)
            {
                goto exit;
            }
        break;

        default:
            retVal = DPU_RANGEPROCHWA_ECMD;
            break;
    }
exit:
    return (retVal);
}


/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU deinit function. It frees the resources used for the DPU.
 *
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle           rangeProc DPU handle
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_deinit
(
    DPU_RangeProcHWA_Handle     handle
)
{
    rangeProcHWAObj     *rangeProcObj;
    int32_t             retVal = 0;

    /* Sanity Check */
    rangeProcObj = (rangeProcHWAObj *)handle;
    if(rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Delete Semaphores */
    SemaphoreP_delete(rangeProcObj->edmaDoneSemaHandle);
    SemaphoreP_delete(rangeProcObj->hwaDoneSemaHandle);

    /* Free memory */
    MemoryP_ctrlFree(handle, sizeof(rangeProcHWAObj));
exit:

    return (retVal);
}
