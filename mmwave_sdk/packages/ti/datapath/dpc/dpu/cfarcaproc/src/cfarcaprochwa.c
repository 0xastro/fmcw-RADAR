/**
 *   @file  cfarcaprochwa.c
 *
 *   @brief
 *      Implements Data path processing Unit using HWA.
 *
 *  \par
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

/* mmWave SDK drivers/common Include Files */
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>

/* Data Path Include Files */
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpc/dpu/cfarcaproc/include/cfarcaprochwainternal.h>
#include <ti/datapath/dpc/dpu/cfarcaproc/cfarcaprochwa.h>

//#define DBG_CFAR_HWA_OBJ_DPU

#ifdef DBG_CFAR_HWA_OBJ_DPU
CFARHwaObj         *cfarHwaObj;
volatile CFARHwaObj *gCfarHwaObj[RL_MAX_SUBFRAMES] = {NULL};
volatile uint32_t gCfarHwaObjInd = 0;
#endif

/**
 *  @b Description
 *  @n
 *      HWA processing completion call back function as per HWA API.
 *      Depending on the programmed transfer completion codes,
 *      posts HWA done semaphore.
 *
 *  @param[in]  arg                 Argument to the callback function
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static void CFARCAHWADoneIsrCallback(void * arg)
{
    if (arg != NULL) 
    {
        SemaphoreP_post((SemaphoreP_Handle)arg);
    }
}

/** @brief Configures ParameterSet for CFAR detection (The CFAR detection runs along range bins).
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *   @param[in] handle              HWA driver handle
 *
 *   @param[in] paramSetStartIdx    HWA parameter set start index
 *
 *   @param[in] numRangeBins        Number of range bins
 *
 *   @param[in] numDopplerBins      NUmber of Doppler bins
 *
 *   @param[in] cfarCfg             Pointer to CFAR HWA configuration
 *
 *   @param[in] detObjectListSize   Maximum size of the list of detected objects
 *
 *   @param[in] dmaTriggerSource    DMA trigger source channel
 *
 *   @param[in] hwaSourceBufOffset  HWA memory offset with input data:
 *                                  log magnitude 2nd D FFT matrix
 *
 *   @param[in] hwaDestBufOffset    HWA memory offset with CFAR output results:
 *                                  List of detected objects as array of structures
 *                                  cfarDetOutput_t
 *   @param[in] cfarDomain          0: CFAR in range domain, 1: CFAR in Doppler domain
 */
int32_t  HWAutil_configCFAR(HWA_Handle handle,
                             uint8_t  paramSetStartIdx,
                             uint32_t numRangeBins,
                             uint32_t numDopplerBins,
                             DPU_CFARCAProc_CfarCfg     *cfarCfg,
                             uint16_t detObjectListSize,
                             uint8_t dmaTriggerSource,
                             uint16_t hwaSourceBufOffset,
                             uint16_t hwaDestBufOffset,
                             uint8_t cfarDomain)
{
    HWA_ParamConfig hwaParamCfg;
    int32_t retVal = 0;

    retVal = HWA_enable(handle, 0); // Disable HWA
    if (retVal != 0)
    {
        goto exit;
    }


    memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
    hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
    hwaParamCfg.dmaTriggerSrc = dmaTriggerSource;

    hwaParamCfg.accelModeArgs.cfarMode.peakGroupEn = cfarCfg->peakGroupingEn;
    hwaParamCfg.accelMode = HWA_ACCELMODE_CFAR;

    //cfarInpMode = 1, cfarLogMode = 1, cfarAbsMode = 00b
    hwaParamCfg.accelModeArgs.cfarMode.operMode = HWA_CFAR_OPER_MODE_LOG_INPUT_REAL;

    if(cfarDomain == DPU_CFAR_RANGE_DOMAIN)
    {
        hwaParamCfg.source.srcAcnt = numRangeBins-1;
        hwaParamCfg.source.srcAIdx = numDopplerBins*sizeof(uint16_t);
        hwaParamCfg.source.srcBIdx = sizeof(uint16_t);
        hwaParamCfg.source.srcBcnt = numDopplerBins-1;
    }
    else
    {
        if (cfarCfg->cyclicMode==1)
        {
            hwaParamCfg.source.srcAcnt = (numDopplerBins-1) + (cfarCfg->guardLen + cfarCfg->winLen) + (cfarCfg->guardLen + cfarCfg->winLen); //numDopplerBins-1
            hwaParamCfg.source.srcShift = numDopplerBins - (cfarCfg->guardLen + cfarCfg->winLen);
            hwaParamCfg.source.srcCircShiftWrap = mathUtils_floorLog2(numDopplerBins); //fix this
        }
        else
        {
            hwaParamCfg.source.srcAcnt = numDopplerBins-1;
            hwaParamCfg.source.srcShift = 0;
            hwaParamCfg.source.srcCircShiftWrap = 0;
        }

        hwaParamCfg.source.srcAIdx = sizeof(uint16_t);
        hwaParamCfg.source.srcBIdx = numDopplerBins*sizeof(uint16_t);
        hwaParamCfg.source.srcBcnt = numRangeBins-1;
    }
    hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL;
    hwaParamCfg.source.srcAddr = hwaSourceBufOffset;
    hwaParamCfg.source.srcScale = 8;

    hwaParamCfg.dest.dstAddr = hwaDestBufOffset;
    hwaParamCfg.dest.dstAcnt = detObjectListSize - 1;
    hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
    hwaParamCfg.dest.dstAIdx = sizeof(DPU_CFARCAProcHWA_CfarDetOutput);
    hwaParamCfg.dest.dstBIdx = 0; //don't care
    hwaParamCfg.dest.dstScale = 8;

    hwaParamCfg.accelModeArgs.cfarMode.numGuardCells = cfarCfg->guardLen;
    hwaParamCfg.accelModeArgs.cfarMode.nAvgDivFactor = cfarCfg->noiseDivShift;
    hwaParamCfg.accelModeArgs.cfarMode.cyclicModeEn = cfarCfg->cyclicMode;
    hwaParamCfg.accelModeArgs.cfarMode.nAvgMode = cfarCfg->averageMode;
    hwaParamCfg.accelModeArgs.cfarMode.numNoiseSamplesRight = cfarCfg->winLen >> 1;
    hwaParamCfg.accelModeArgs.cfarMode.numNoiseSamplesLeft =  cfarCfg->winLen >> 1;
    hwaParamCfg.accelModeArgs.cfarMode.outputMode = HWA_CFAR_OUTPUT_MODE_I_PEAK_IDX_Q_NEIGHBOR_NOISE_VAL;

    retVal = HWA_configParamSet(handle, paramSetStartIdx, &hwaParamCfg, NULL);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Disable trigger DMA/interrupt for this param set */
    retVal = HWA_disableParamSetInterrupt(handle,
                                          paramSetStartIdx,
                                          HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);


exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Configures all CFAR processing related EDMA configuration.
 *
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *  @param[in] hwaHandle HWA handle
 *
 *  @param[in] pRes Pointer to hardware resources
 *
 *  @param[in] staticCfg Pointer to static configuration
 *
 *  @retval EDMA error code, see EDMA API.
 */
int32_t CFARCAHWA_config_EDMA
(
    HWA_Handle hwaHandle,
    DPU_CFARCAProcHWA_HW_Resources *pRes,
    DPU_CFARCAProcHWA_StaticConfig  *staticCfg
)
{
    int32_t errorCode = EDMA_NO_ERROR;
    EDMA_Handle edmaHandle = pRes->edmaHandle;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncACfg     syncACfg;

    chainingCfg.chainingChan                  = pRes->edmaHwaInSignature.channel; 
    chainingCfg.isIntermediateChainingEnabled = false;
    chainingCfg.isFinalChainingEnabled        = true;

    syncACfg.srcAddress  = (uint32_t)pRes->detMatrix.data;
    syncACfg.destAddress = (uint32_t)pRes->hwaMemInp;
    syncACfg.aCount      = staticCfg->numRangeBins * staticCfg->numDopplerBins * sizeof(uint16_t);
    syncACfg.bCount      = 1;
    syncACfg.srcBIdx     = 0;
    syncACfg.dstBIdx     = 0;
         
    errorCode = DPEDMA_configSyncA_singleFrame(edmaHandle,
                                   &pRes->edmaHwaIn,
                                   &chainingCfg,
                                   &syncACfg,
                                   false, //isEventTriggered
                                   false,//isIntermediateTransferInterruptEnabled
                                   false,//isTransferCompletionEnabled
                                   NULL, //transferCompletionCallbackFxn
                                   NULL);//transferCompletionCallbackFxnArg
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    errorCode = DPEDMAHWA_configOneHotSignature(edmaHandle,
                                                &pRes->edmaHwaInSignature,
                                                hwaHandle,
                                                pRes->hwaCfg.paramSetStartIdx,
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
 *      Prepares HWA during run time. Configures common registers. If the HWA
 *      param sets are not reused, they can be programmed one time, and only
 *      this function has to be called during run time.
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *
 *  @param[in] cfarHwaObj Pointer to CFAR DPU instance
 *
 *  @param[in] cfarDomain 0: prepare CFAR in range domain, 1: prepare CFAR in Doppler domain
 *
 *  @retval EDMA error code, see EDMA API.
 */
int32_t CFARHWA_prepareHwaRunTime
(
    CFARHwaObj *cfarHwaObj,
    uint32_t cfarDomain
)
{
    int32_t             retVal = 0;
    HWA_CommonConfig    hwaCommonConfig;
    DPU_CFARCAProc_CfarCfg     *pCfarCfg;
    uint8_t paramSetStartIdx;

    DPU_CFARCAProcHWA_HW_Resources *pRes = &cfarHwaObj->res;

    if (cfarDomain == DPU_CFAR_DOPPLER_DOMAIN)
    {
        pCfarCfg = &cfarHwaObj->cfarCfgDoppler;
        paramSetStartIdx = pRes->hwaCfg.paramSetStartIdx;
    }
    else
    {
        pCfarCfg = &cfarHwaObj->cfarCfgRange;
        paramSetStartIdx = pRes->hwaCfg.paramSetStartIdx + 1;
    }

    /* Disable the HWA */
    retVal = HWA_enable(cfarHwaObj->hwaHandle, 0); // set 1 to enable
    if (retVal != 0)
    {
        goto exit;
    }

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_NUMLOOPS |
                               HWA_COMMONCONFIG_MASK_PARAMSTARTIDX |
                               HWA_COMMONCONFIG_MASK_PARAMSTOPIDX |
                               HWA_COMMONCONFIG_MASK_FFT1DENABLE |
                               HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD |
                               HWA_COMMONCONFIG_MASK_CFARTHRESHOLDSCALE;

    hwaCommonConfig.numLoops = 1;
    hwaCommonConfig.paramStartIdx = paramSetStartIdx;
    hwaCommonConfig.paramStopIdx =  paramSetStartIdx;
    hwaCommonConfig.fftConfig.fft1DEnable = HWA_FEATURE_BIT_DISABLE;
    hwaCommonConfig.fftConfig.interferenceThreshold = 0xFFFFFF;
    hwaCommonConfig.cfarConfig.cfarThresholdScale = pCfarCfg->thresholdScale;

    retVal = HWA_configCommon(cfarHwaObj->hwaHandle,&hwaCommonConfig);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n  Convert range field of view meters to indices
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *
 * @param[in]  fovRangeCfg Field of view configuration for range (units in meters)
 *
 * @param[in]  rangeStep range resolution
 *
 * @param[out]  fovRangeIdxCfg Field of view configuration for range (range indices)
 *
 */
void CFARHWA_fovConvertRange(DPU_CFARCAProc_FovCfg *fovRangeCfg,
                                float rangeStep,
                                DPU_CFARCAProc_FovIdxCfg *fovRangeIdxCfg)
{
    fovRangeIdxCfg->minIdx = (int16_t) (fovRangeCfg->min / rangeStep + 0.5);
    fovRangeIdxCfg->maxIdx = (int16_t) (fovRangeCfg->max / rangeStep + 0.5);
}


/**
 *  @b Description
 *  @n  Convert Doppler field of view meters/sec to indices
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *
 * @param[in]  fovDopplerCfg fov in meters/sec
 *
 * @param[in]  dopplerStep Doppler resolution
 *
 * @param[out]  fovDopplerIdxCfg fov indices
 *
 *
 */
void CFARHWA_fovConvertDoppler(DPU_CFARCAProc_FovCfg *fovDopplerCfg,
                                float dopplerStep,
                                DPU_CFARCAProc_FovIdxCfg *fovDopplerIdxCfg)
{
    float doppler;

    doppler = (fovDopplerCfg->min / dopplerStep);
    if (doppler < 0.)
    {
        doppler -= .5;
    }
    else
    {
        doppler += .5;

    }
    fovDopplerIdxCfg->minIdx = (int16_t) doppler;

    doppler = (fovDopplerCfg->max / dopplerStep);
    if (doppler < 0.)
    {
        doppler -= .5;
    }
    else
    {
        doppler += .5;

    }
    fovDopplerIdxCfg->maxIdx = (int16_t) doppler;

}

/**
 *  @b Description
 *  @n  Saves configuration parameters to CFAR instance
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *
 * @param[in]  cfarHwaObj Pointer to CFAR instance
 *
 * @param[in]  cfarHwaCfg Pointer to configuration
 *
 *
 */
void CFARHWA_saveConfiguration(CFARHwaObj * cfarHwaObj,
                               DPU_CFARCAProcHWA_Config *cfarHwaCfg)
{
    cfarHwaObj->staticCfg = cfarHwaCfg->staticCfg;
    cfarHwaObj->res = cfarHwaCfg->res;
    cfarHwaObj->cfarCfgDoppler = *cfarHwaCfg->dynCfg.cfarCfgDoppler;
    cfarHwaObj->cfarCfgRange = *cfarHwaCfg->dynCfg.cfarCfgRange;
    cfarHwaObj->staticCfg = cfarHwaCfg->staticCfg;

    CFARHWA_fovConvertDoppler(cfarHwaCfg->dynCfg.fovDoppler,
                              cfarHwaObj->staticCfg.dopplerStep,
                              &cfarHwaObj->fovDoppler);

    CFARHWA_fovConvertRange(cfarHwaCfg->dynCfg.fovRange,
                            cfarHwaObj->staticCfg.rangeStep,
                            &cfarHwaObj->fovRange);
}

/**
 *  @b Description
 *  @n  Triggers CFAR execution.
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *
 * @param[in]  cfarHwaObj Pointer to CFAR instance
 *
 * @param[in]  loadDataToHWA loadDataToHWA=1: triggers EDMA to copy data from L3 memory
 *             to internal HWA memory. EDMA then triggers HWA CFAR execution.
 *             loadDataToHWA=0: triggers HWA CFAR execution only since the input data
 *             is already in internal HWA memory.
 *
 *
 */int32_t CFARHWA_triggerCFAR(CFARHwaObj *cfarHwaObj, bool loadDataToHWA)
{
    int32_t retVal = 0;
    DPU_CFARCAProcHWA_HW_Resources *pRes = &cfarHwaObj->res;

    /* Enable the HWA */
    retVal = HWA_enable(cfarHwaObj->hwaHandle, 1);
    if (retVal != 0)
    {
        goto exit;
    }
    if (loadDataToHWA)
    {
        /* Trigger EDMA  + CFAR Doppler */
        retVal  = EDMA_startTransfer(pRes->edmaHandle, pRes->edmaHwaIn.channel, EDMA3_CHANNEL_TYPE_DMA);
        if (retVal != EDMA_NO_ERROR)
        {
            goto exit;
        }
    }
    else
    {
        /* Trigger CFAR Range only */
        retVal = HWA_setDMA2ACCManualTrig(cfarHwaObj->hwaHandle,
                                          pRes->hwaCfg.paramSetStartIdx);
        if (retVal != 0)
        {
            goto exit;
        }
    }
exit:
        return retVal;
}

/**
 *  @b Description
 *  @n
 *      Converts HWA CFAR Detection list format into bit mask format:
 *      word = (rngInd*dopFftsize+dopInd) >> 5
 *      bit = (rngInd*dopFftsize+dopInd) & 31
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 *
 * @param[in]  hwaCfarDetList Pointer to HWA CFAR detection list
 *
 * @param[in]  numDopBins Number of Doppler bins (must be power of 2)
 *
 * @param[in]  numCfarDetections Number of detected points reported by CFAR
 *
 * @param[in]  cfarDomain 0: CFAR in range domain, 1: CFAR in Doppler domain
 *
 * @param[out]  cfarDetOutBitMask Pointer to output bit mask (compressed list)
 *
 */
void CFARHWA_convHwaCfarDetListToDetMask(DPU_CFARCAProcHWA_CfarDetOutput *hwaCfarDetList,
                                             uint32_t numDopBins,
                                             uint32_t numCfarDetections,
                                             uint32_t cfarDomain,
                                             uint32_t *cfarDetOutBitMask)
{
    uint32_t i;
    uint32_t bit, word;
    uint32_t temp;
    uint32_t cellOffset;
    uint32_t iterOffset;

    if (cfarDomain == DPU_CFAR_RANGE_DOMAIN)
    {
       /*CFAR in Range domain, cellIdx is range index, iterNum is Doppler index*/
       cellOffset = numDopBins;
       iterOffset = 1;
    }
    else
    {
       /*CFAR in Doppler domain, cellIdx is Doppler index, iterNum is Range index*/
       cellOffset = 1;
       iterOffset = numDopBins;
    }

    for (i=0; i < numCfarDetections; i++)
    {
       temp = hwaCfarDetList[i].cellIdx * cellOffset + hwaCfarDetList[i].iterNum * iterOffset;
       word = temp >> 5;
       bit = temp & 31;
       cfarDetOutBitMask[word] |= 1 << bit;
    }
}

/**
 *  @b Description
 *  @n
 *     Checks if an object is detected at range/doppler index parir.
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 * @param[in]  rangeInd range index
 *
 * @param[in]  dopplerInd Doppler index
 *
 * @param[in]  numDopBins Number of Doppler bins (must be power of 2)
 *
 * @param[in]  cfarDetOutBitMask Pointer to output bit mask (compressed list)
 *
 *  @retval
 *      true     - Object found in CFAR detection bit mask
 *  @retval
 *      false    - Object not found in CFAR detection bit mask
 *
 */
bool CFARHWA_isHwaCfarDetected(uint32_t rangeInd,
                               uint32_t dopplerInd,
                               uint32_t numDopBins,
                               uint32_t *cfarDetOutBitMask)
{
    uint32_t bit, word;
    uint32_t temp;

    temp = rangeInd * numDopBins + dopplerInd;
    word = temp >> 5;
    bit = temp & 31;
    if (cfarDetOutBitMask[word] & (1 << bit))
    {
       return true;
    }
    else
    {
       return false;
    }
}

/**
 *  @b Description
 *  @n
 *     Logical AND operation on the two input lists, output is a list of cloud points
 *     detected in both domains.
 *
 *  \ingroup    DPU_CFARCAPROC_INTERNAL_FUNCTION
 *
 * @param[in]  cfarRangeDetOutList CFAR Range List of detected points
 *
 * @param[in]  numCfarRangeDet Number CFAR Range detected points
 *
 * @param[in]  cfarDopplerDetOutBitMask CFAR Doppler bit mask of detected points
 *
 * @param[in]  cfarDetectionList intermediate detection list
 *
 * @param[in]  cfarDetectionListSize intermediate detection list size
 *
 * @param[in]  detMat Detection matrix
 *
 * @param[in]  numDopplerBins Number of Doppler bins (must be power of 2)
 *
 * @param[in]  selRangeMin Minimum range index value to be shipped out
 *
 * @param[in]  selRangeMax Maximum range index value to be shipped out
 *
 * @param[in]  selDopplerMin Minimum Doppler index value to be shipped out
 *
 * @param[in]  selDopplerMax Maximum Doppler index value to be shipped out
 *
 * @param[in]  andWithCfarDopplerOut 1: Send out CFAR range point if it is CFAR detected in Doppler Domain
 *                                   0: Ignore CFAR Doppler detections
 *
 */
uint32_t CFARHWA_cfarRange_AND_cfarDoppler(
                                    DPU_CFARCAProcHWA_CfarDetOutput *cfarRangeDetOutList,
                                    uint32_t numCfarRangeDet,
                                    uint32_t *cfarDopplerDetOutBitMask,
                                    DPIF_CFARDetList *cfarDetectionList,
                                    uint32_t cfarDetectionListSize,
                                    uint16_t *detMat,
                                    uint16_t numDopplerBins,
                                    int16_t selRangeMin,
                                    int16_t selRangeMax,
                                    int16_t selDopplerMin,
                                    int16_t selDopplerMax,
                                    bool andWithCfarDopplerOut
                                    )
{
    uint32_t numCfarDet;
    uint32_t rangeIdx;
    uint32_t dopplerIdx;
    int32_t dopplerSgnIdx;
    uint32_t i;
    uint32_t cellLog2NoiseQ9;
    bool addToList;
    float snrdB, noisedB;

    numCfarDet = 0;
    for (i=0; i < numCfarRangeDet; i++)
    {
       rangeIdx = cfarRangeDetOutList[i].cellIdx;
       dopplerIdx = cfarRangeDetOutList[i].iterNum;
       dopplerSgnIdx = (int32_t ) dopplerIdx;
       if (dopplerSgnIdx >= (int32_t)(numDopplerBins>>1))
       {
           dopplerSgnIdx = dopplerSgnIdx - (int32_t)numDopplerBins;
       }
       cellLog2NoiseQ9 =  cfarRangeDetOutList[i].noise;
       if ((rangeIdx >= selRangeMin) &&
           (rangeIdx <= selRangeMax) &&
           (dopplerSgnIdx >= selDopplerMin) &&
           (dopplerSgnIdx <= selDopplerMax))
       {
           if (andWithCfarDopplerOut)
           {
               addToList = CFARHWA_isHwaCfarDetected(rangeIdx, dopplerIdx, numDopplerBins, cfarDopplerDetOutBitMask);
           }
           else
           {
               addToList = true;
           }
           if (addToList)
           {
               cfarDetectionList[numCfarDet].rangeIdx = rangeIdx;
               cfarDetectionList[numCfarDet].dopplerIdx = dopplerIdx;
               snrdB = ((float) detMat[rangeIdx * numDopplerBins + dopplerIdx] - (float) cellLog2NoiseQ9) * 6. /512.;
               noisedB = ((float) cellLog2NoiseQ9) * 6. /512.;
               cfarDetectionList[numCfarDet].noise = noisedB * 10.;
               cfarDetectionList[numCfarDet].snr = snrdB * 10.;
               numCfarDet++;
               if (numCfarDet >= cfarDetectionListSize)
               {
                   break;
               }
           }
       }
    }
    return numCfarDet;
}

DPU_CFARCAProcHWA_Handle DPU_CFARCAProcHWA_init
(
    DPU_CFARCAProcHWA_InitParams *initCfg,
    int32_t* errCode
)
{
    CFARHwaObj         *cfarHwaObj = NULL;
    SemaphoreP_Params  semParams;
    *errCode = 0;

    if ((initCfg == NULL) || (initCfg->hwaHandle == NULL))
    {
        *errCode = DPU_CFARCAPROCHWA_EINVAL;
        goto exit;
    }

    cfarHwaObj = MemoryP_ctrlAlloc(sizeof(CFARHwaObj), 0);
    if (cfarHwaObj == NULL)
    {
        *errCode = DPU_CFARCAPROCHWA_ENOMEM;
        goto exit;
    }

    /* Save for debugging */
#ifdef DBG_CFAR_HWA_OBJ_DPU
    if (gCfarHwaObjInd < RL_MAX_SUBFRAMES)
    {
        gCfarHwaObj[gCfarHwaObjInd++] = cfarHwaObj;
    }
#endif

    /* Initialize memory */
    memset((void *)cfarHwaObj, 0, sizeof(CFARHwaObj));

    /* Save init config params */
    cfarHwaObj->hwaHandle   = initCfg->hwaHandle;

    /* Create semaphore for HWA done */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    cfarHwaObj->hwaDone_semaHandle = SemaphoreP_create(0, &semParams);
    if(cfarHwaObj->hwaDone_semaHandle == NULL)
    {
        *errCode = DPU_CFARCAPROCHWA_ESEMA;
        goto exit;
    }

exit:

    if(*errCode < 0)
    {
        /* Free the object if it was allocated */
        if(cfarHwaObj != NULL)
        {
            MemoryP_ctrlFree(cfarHwaObj, sizeof(CFARHwaObj));
            cfarHwaObj = NULL;
        }
    }

    return ((DPU_CFARCAProcHWA_Handle)cfarHwaObj);
}

int32_t DPU_CFARCAProcHWA_config
(
   DPU_CFARCAProcHWA_Handle       handle,
   DPU_CFARCAProcHWA_Config       *cfarHwaCfg
)
{
   int32_t  retVal = 0;

   CFARHwaObj *cfarHwaObj = (CFARHwaObj *)handle;
   DPU_CFARCAProcHWA_HW_Resources *pRes = &cfarHwaCfg->res;
   DPU_CFARCAProcHWA_StaticConfig *staticCfg = &cfarHwaCfg->staticCfg;

   if(cfarHwaObj == NULL)
   {
       retVal = DPU_CFARCAPROCHWA_EINVAL;
       goto exit;
   }


   if (pRes->hwaCfg.numParamSet != DPU_CFARCAPROCHWA_NUM_HWA_PARAM_SETS)
   {
       retVal = DPU_CFARCAPROCHWA_EINVAL__NUM_PARAM_SETS;
       goto exit;
   }

   /* Check if detection matrix format is supported. */
   if(pRes->detMatrix.datafmt != DPIF_DETMATRIX_FORMAT_1)
   {
       retVal = DPU_CFARCAPROCHWA_EINVAL__DET_MATRIX_FORMAT;
       goto exit;
   }

   if MEM_IS_NOT_ALIGN(pRes->detMatrix.data,
                       DPU_CFARCAPROCHWA_DET_MATRIX_BYTE_ALIGNMENT)
   {
       retVal = DPU_CFARCAPROCHWA_ENOMEMALIGN_DET_MATRIX;
       goto exit;
   }

   if MEM_IS_NOT_ALIGN(pRes->cfarRngDopSnrList,
                       DPU_CFARCAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT)
   {
       retVal = DPU_CFARCAPROCHWA_ENOMEMALIGN_CFAR_DET_LIST;
       goto exit;
   }

   if MEM_IS_NOT_ALIGN(pRes->hwaMemOutDoppler,
                       DPU_CFARCAPROCHWA_HWA_MEM_OUT_DOPPLER_BYTE_ALIGNMENT)
   {
       retVal = DPU_CFARCAPROCHWA_ENOMEMALIGN_HWA_MEM_OUT_DOPPLER;
       goto exit;
   }

   if MEM_IS_NOT_ALIGN(pRes->hwaMemOutRange,
                       DPU_CFARCAPROCHWA_HWA_MEM_OUT_RANGE_BYTE_ALIGNMENT)
   {
       retVal = DPU_CFARCAPROCHWA_ENOMEMALIGN_HWA_MEM_OUT_RANGE;
       goto exit;
   }

   if MEM_IS_NOT_ALIGN(pRes->cfarDopplerDetOutBitMask,
                       DPU_CFARCAPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT)
   {
       retVal = DPU_CFARCAPROCHWA_ENOMEMALIGN_DOPPLER_DET_OUT_BIT_MASK;
       goto exit;
   }

   /* check if input M scratch for detection matrix has
    * entire detection matrix fitting in it */
   if ((pRes->hwaMemInpSize * sizeof(uint16_t)) < pRes->detMatrix.dataSize)
   {
       retVal = DPU_CFARCAPROCHWA_ENOMEM__DET_MATRIX_EXCEEDS_HWA_INP_MEM;
       goto exit;
   }

   /* check if input scratch memory for bit mask is sufficient */
   if ((pRes->cfarDopplerDetOutBitMaskSize * 32U) <
       (staticCfg->numDopplerBins * staticCfg->numRangeBins))
   {
       retVal = DPU_CFARCAPROCHWA_ENOMEM__INSUFFICIENT_DOP_DET_OUT_BIT_MASK;
       goto exit;
   }

   CFARHWA_saveConfiguration(cfarHwaObj, cfarHwaCfg);

   if (cfarHwaObj->cfarCfgDoppler.thresholdScale > 0)
   {
       /**************************************/
       /* CFAR DOPPLER DOMAIN                */
       /**************************************/
       retVal = HWAutil_configCFAR(cfarHwaObj->hwaHandle,
                                   pRes->hwaCfg.paramSetStartIdx,
                                   staticCfg->numRangeBins,
                                   staticCfg->numDopplerBins,
                                   &cfarHwaObj->cfarCfgDoppler,
                                   pRes->hwaMemOutDopplerSize,
                                   pRes->hwaCfg.paramSetStartIdx, /* dmaTriggerSrc */
                                   ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemInp),
                                   ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemOutDoppler),
                                   DPU_CFAR_DOPPLER_DOMAIN);
       if (retVal != 0)
       {
         goto exit;
       }
   }

   /**************************************/
   /* CFAR RANGE DOMAIN                  */
   /**************************************/
   retVal = HWAutil_configCFAR(cfarHwaObj->hwaHandle,
                               pRes->hwaCfg.paramSetStartIdx + 1,
                               staticCfg->numRangeBins,
                               staticCfg->numDopplerBins,
                               &cfarHwaObj->cfarCfgRange,
                               pRes->hwaMemOutRangeSize,
                               pRes->hwaCfg.paramSetStartIdx, /* dmaTriggerSrc */
                               ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemInp),
                               ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemOutRange),
                               DPU_CFAR_RANGE_DOMAIN);
   if (retVal != 0)
   {
     goto exit;
   }

   /* Configure EDMA */
   retVal = CFARCAHWA_config_EDMA(cfarHwaObj->hwaHandle, pRes, staticCfg);
   if (retVal != 0)
   {
       goto exit;
   }

exit:
   return retVal;
}

int32_t DPU_CFARCAProcHWA_process
(
    DPU_CFARCAProcHWA_Handle   handle,
    DPU_CFARCAProcHWA_OutParams  *outParams
)
{
    volatile uint32_t   startTime;
    volatile uint32_t   startTime1;
    uint16_t            numObjs = 0;
    uint32_t cfarDomain;
    bool loadDataToHWA;
    bool andWithCfarDopplerOut;
    int32_t             retVal = 0;
    uint32_t            waitTimeLocal = 0;

    CFARHwaObj *cfarHwaObj;
    DPU_CFARCAProcHWA_HW_Resources *pRes;
    DPU_CFARCAProcHWA_StaticConfig *staticCfg;

    if (handle == NULL)
    {
        retVal = DPU_CFARCAPROCHWA_EINVAL;
        goto exit;
    }

    cfarHwaObj = (CFARHwaObj *)handle;
    pRes = &cfarHwaObj->res;
    staticCfg = &cfarHwaObj->staticCfg;

    startTime = Cycleprofiler_getTimeStamp();


    /**********************************************/
    /* ENABLE NUMLOOPS DONE INTERRUPT FROM HWA */
    /**********************************************/
    retVal = HWA_enableDoneInterrupt(cfarHwaObj->hwaHandle,
                                        CFARCAHWADoneIsrCallback,
                                        cfarHwaObj->hwaDone_semaHandle);
    if (retVal != 0)
    {
        goto exit;
    }
    if (cfarHwaObj->cfarCfgDoppler.thresholdScale > 0)
    {
        cfarDomain = DPU_CFAR_DOPPLER_DOMAIN;
        retVal = CFARHWA_prepareHwaRunTime (cfarHwaObj, cfarDomain);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Trigger CFAR */
        loadDataToHWA = true;
        retVal = CFARHWA_triggerCFAR(cfarHwaObj, loadDataToHWA);
        if (retVal != 0)
        {
            goto exit;
        }

        /* In parallel with HWA zero cfarDetOutBitMask while HWA doing CFAR */
        memset (pRes->cfarDopplerDetOutBitMask, 0x0, pRes->cfarDopplerDetOutBitMaskSize * sizeof(uint32_t));

        /* wait for the paramSets done interrupt */
        startTime1 = Cycleprofiler_getTimeStamp();
        SemaphoreP_pend(cfarHwaObj->hwaDone_semaHandle, SemaphoreP_WAIT_FOREVER);
        waitTimeLocal += Cycleprofiler_getTimeStamp() - startTime1;

        HWA_readCFARPeakCountReg(cfarHwaObj->hwaHandle, (uint8_t *) &cfarHwaObj->numHwaCfarObjs, sizeof(uint16_t));
        if (cfarHwaObj->numHwaCfarObjs > cfarHwaObj->res.hwaMemOutDopplerSize)
        {
            cfarHwaObj->numHwaCfarObjs = cfarHwaObj->res.hwaMemOutDopplerSize;
        }
    }

    /* Do CFAR Range */
    /* Prepare Common Reg and CFAR threshold */
    cfarDomain = DPU_CFAR_RANGE_DOMAIN;
    retVal = CFARHWA_prepareHwaRunTime (cfarHwaObj, cfarDomain);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Trigger CFAR Range */
    if (cfarHwaObj->cfarCfgDoppler.thresholdScale > 0)
    {
        loadDataToHWA = false;
    }
    else
    {
        loadDataToHWA = true;
    }
    retVal = CFARHWA_triggerCFAR(cfarHwaObj, loadDataToHWA);
    if (retVal != 0)
    {
        goto exit;
    }

    if (cfarHwaObj->cfarCfgDoppler.thresholdScale > 0)
    {
        /* In parallel with HWA convert Doppler CFAR output list into bit mask */
        CFARHWA_convHwaCfarDetListToDetMask((DPU_CFARCAProcHWA_CfarDetOutput * ) pRes->hwaMemOutDoppler,
                                           staticCfg->numDopplerBins,
                                           cfarHwaObj->numHwaCfarObjs,
                                           DPU_CFAR_DOPPLER_DOMAIN,
                                           pRes->cfarDopplerDetOutBitMask);
    }

    /* wait for the paramSets done interrupt */
    startTime1 = Cycleprofiler_getTimeStamp();
    SemaphoreP_pend(cfarHwaObj->hwaDone_semaHandle, SemaphoreP_WAIT_FOREVER);
    waitTimeLocal += Cycleprofiler_getTimeStamp() - startTime1;

    HWA_readCFARPeakCountReg(cfarHwaObj->hwaHandle,
                             (uint8_t *) &cfarHwaObj->numHwaCfarObjs,
                             sizeof(uint16_t));
    if (cfarHwaObj->numHwaCfarObjs > cfarHwaObj->res.hwaMemOutRangeSize)
    {
        cfarHwaObj->numHwaCfarObjs = cfarHwaObj->res.hwaMemOutRangeSize;
    }

    if (cfarHwaObj->cfarCfgDoppler.thresholdScale > 0)
    {
        andWithCfarDopplerOut = true;
    }
    else
    {
        andWithCfarDopplerOut = false;
    }

    cfarHwaObj->numHwaCfarObjs  =  CFARHWA_cfarRange_AND_cfarDoppler(
                                            (DPU_CFARCAProcHWA_CfarDetOutput * ) pRes->hwaMemOutRange,
                                            cfarHwaObj->numHwaCfarObjs,
                                            pRes->cfarDopplerDetOutBitMask,
                                            pRes->cfarRngDopSnrList,
                                            pRes->cfarRngDopSnrListSize,
                                            pRes->detMatrix.data,
                                            staticCfg->numDopplerBins,
                                            cfarHwaObj->fovRange.minIdx,
                                            cfarHwaObj->fovRange.maxIdx,
                                            cfarHwaObj->fovDoppler.minIdx,
                                            cfarHwaObj->fovDoppler.maxIdx,
                                            andWithCfarDopplerOut);

    numObjs = cfarHwaObj->numHwaCfarObjs;
    cfarHwaObj->numProcess++;

    /* Disable the HWA */
    retVal = HWA_enable(cfarHwaObj->hwaHandle, 0);
    if (retVal != 0)
    {
        numObjs = 0U;
        goto exit;
    }


    outParams->numCfarDetectedPoints = numObjs;
    HWA_disableDoneInterrupt(cfarHwaObj->hwaHandle);
    outParams->stats.waitTime = waitTimeLocal;
    outParams->stats.processingTime = Cycleprofiler_getTimeStamp() - startTime - waitTimeLocal;
    outParams->stats.numProcess = cfarHwaObj->numProcess;
exit:
    return (retVal);
}

int32_t DPU_CFARCAProcHWA_control
(
    DPU_CFARCAProcHWA_Handle handle,
    DPU_CFARCAProcHWA_Cmd cmd,
    void *arg,
    uint32_t argSize
)
{
    int32_t    retVal = 0;
    CFARHwaObj *cfarHwaObj = (CFARHwaObj *)handle;
    DPU_CFARCAProcHWA_StaticConfig  *staticCfg = &cfarHwaObj->staticCfg;
    DPU_CFARCAProcHWA_HW_Resources *pRes = &cfarHwaObj->res;

    /* Get rangeProc data object */
    if (cfarHwaObj == NULL)
    {
        retVal = DPU_CFARCAPROCHWA_EINVAL;
        goto exit;
    }

    switch(cmd)
    {
        case DPU_CFARCAProcHWA_Cmd_CfarRangeCfg:
        {
            if((argSize != sizeof(DPU_CFARCAProc_CfarCfg)) ||
               (arg == NULL))
            {
                retVal = DPU_CFARCAPROCHWA_EINVAL;
                goto exit;
            }
            else
            {
                /* Save configuration */
                memcpy((void *)&cfarHwaObj->cfarCfgRange, arg, argSize);

                /**************************************/
                /* CFAR RANGE DOMAIN                */
                /**************************************/
                retVal = HWAutil_configCFAR(cfarHwaObj->hwaHandle,
                                    pRes->hwaCfg.paramSetStartIdx + 1,  //Range position
                                    staticCfg->numRangeBins,
                                    staticCfg->numDopplerBins,
                                    &cfarHwaObj->cfarCfgRange,
                                    pRes->hwaMemOutRangeSize,
                                    pRes->hwaCfg.paramSetStartIdx, /* dmaTriggerSrc */
                                    ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemInp),
                                    ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemOutRange),
                                    DPU_CFAR_RANGE_DOMAIN);
                if (retVal != 0)
                {
                  goto exit;
                }
            }
        }
        break;
        case DPU_CFARCAProcHWA_Cmd_CfarDopplerCfg:
        {
            if((argSize != sizeof(DPU_CFARCAProc_CfarCfg)) ||
               (arg == NULL))
            {
                retVal = DPU_CFARCAPROCHWA_EINVAL;
                goto exit;
            }
            else
            {
                /* Save configuration */
                memcpy((void *)&cfarHwaObj->cfarCfgDoppler, arg, argSize);

                /**************************************/
                /* CFAR DOPPLER DOMAIN                */
                /**************************************/
                retVal = HWAutil_configCFAR(cfarHwaObj->hwaHandle,
                                    pRes->hwaCfg.paramSetStartIdx, //Doppler position
                                    staticCfg->numRangeBins,
                                    staticCfg->numDopplerBins,
                                    &cfarHwaObj->cfarCfgDoppler,
                                    pRes->hwaMemOutDopplerSize,
                                    pRes->hwaCfg.paramSetStartIdx, /* dmaTriggerSrc */
                                    ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemInp),
                                    ADDR_TRANSLATE_CPU_TO_HWA(pRes->hwaMemOutDoppler),
                                    DPU_CFAR_DOPPLER_DOMAIN);
                if (retVal != 0)
                {
                  goto exit;
                }
            }
        }
        break;
        case DPU_CFARCAProcHWA_Cmd_FovRangeCfg:
        {
            if((argSize != sizeof(DPU_CFARCAProc_FovCfg)) ||
               (arg == NULL))
            {
                retVal = DPU_CFARCAPROCHWA_EINVAL;
                goto exit;
            }
            else
            {
                /* Save configuration */
                CFARHWA_fovConvertRange(arg,
                                        cfarHwaObj->staticCfg.rangeStep,
                                        &cfarHwaObj->fovRange);
            }
        }
        break;
        case DPU_CFARCAProcHWA_Cmd_FovDopplerCfg:
        {
            if((argSize != sizeof(DPU_CFARCAProc_FovCfg)) ||
               (arg == NULL))
            {
                retVal = DPU_CFARCAPROCHWA_EINVAL;
                goto exit;
            }
            else
            {
                /* Save configuration */
                CFARHWA_fovConvertDoppler(arg,
                                          cfarHwaObj->staticCfg.dopplerStep,
                                          &cfarHwaObj->fovDoppler);
            }
        }
        break;
        default:
            retVal = DPU_CFARCAPROCHWA_EINVAL;
            break;
    }
exit:
    return (retVal);
}

int32_t DPU_CFARCAProcHWA_deinit(DPU_CFARCAProcHWA_Handle handle)
{
    int32_t retVal = 0;
    if (handle == NULL)
    {
        retVal = DPU_CFARCAPROCHWA_EINVAL;
        goto exit;
    }
    MemoryP_ctrlFree(handle, sizeof(CFARHwaObj));
exit:
    return (retVal);
}
