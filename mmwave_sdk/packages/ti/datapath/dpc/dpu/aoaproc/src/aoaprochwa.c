/**
 *   @file  aoaprochwa.c
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

/* Data Path Include Files */
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpc/dpu/aoaproc/include/aoaprochwa_internal.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>
#include <ti/utils/mathutils/mathutils.h>

/*! @brief Converts Doppler index to signed variable. Value greater than or equal
 *         half the Doppler FFT size will become negative.
 *         Needed for extended maximum velocity.
 */
#define DOPPLER_IDX_TO_SIGNED(_idx, _fftSize) ((_idx) < (_fftSize)/2 ? \
        ((int16_t) (_idx)) : ((int16_t) (_idx) - (int16_t) (_fftSize)))

//#define DBG_AOA_HWA_OBJ_DPU

#ifdef DBG_AOA_HWA_OBJ_DPU
volatile AOAHwaObj *gAoaHwaObj[RL_MAX_SUBFRAMES] = {NULL};
volatile uint32_t gAoaHwaObjInd = 0;
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
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *  @retval     None
 */
static void AOAProcHWADoneIsrCallback(void * arg)
{
    if (arg != NULL) 
    {
        SemaphoreP_post((SemaphoreP_Handle)arg);
    }
}
static uint32_t AoAProcHWA_angleEstimationAzimElev
(
    uint32_t        objInStartIdx,
    uint32_t        numObjIn,
    uint16_t        *azimFFTAbsPtr,
    uint16_t        *azimFFTPtr,
    uint16_t        *elevFFTPtr,
    AOAHwaObj       *aoaHwaObj,
    uint32_t        objOutIdx
);

static int32_t HWAutil_configDopplerFFTSingleRangeBin
(
    HWA_Handle      hwaHandle,
    uint8_t        paramSetStartIdx,
    uint32_t        numDopplerChirps,
    uint32_t         numDopplerBins,
    uint32_t        numOutSamplesPerFft,
    uint8_t         numRxAnt,
    uint8_t         numTxAntIn,
    uint8_t         numTxAntOut,
    uint32_t        windowOffset,
    uint8_t         winSym,
    uint8_t         dmaTriggerSource,      
    uint8_t         dmaTriggerDest,
    uint16_t        hwaMemAzimSource,
    uint16_t        hwaMemAzimDest
);

static int32_t HWAutil_configAngleEstAzimuthElevation
(
    HWA_Handle      hwaHandle,
    uint8_t        paramSetStartIdx,
    uint32_t        numVirtualAntAzim,
    uint32_t        numVirtualAntElev,
    uint32_t        fftOutSize,
    uint32_t        numIter,
    uint16_t        hwaSourceBufOffset,
    uint16_t        hwaDestAzimAbsBufOffset,
    uint16_t        hwaDestAzimCplxBufOffset,
    uint16_t        hwaDestElevCplxBufOffset
);

static int32_t HWAutil_configAngleEstAzimuth
(
    HWA_Handle      hwaHandle,
    uint8_t        paramSetStartIdx,
    uint32_t        numVirtualAnt,
    uint32_t        fftOutSize,
    uint32_t        numIter,
    uint16_t        hwaSourceBufOffset,
    uint16_t        hwaDestBufOffset
);

/**
 *   @b Description
 *   @n
 *      AoAProcHWA Rx channel phase/gain compensation. The function performs phase/gain
 *      compensation on the Rx virtual antenna symbols of the detected object. The
 *      function is also used to perform the phase/gain compensation on the two dimensional
 *      array of only azimuth antennas which is used for the azimuth heat map display
 *      on the host. In this case the two dimensional array is laid out as
 *      X[number of range bins][number of azimuth antennas].
 *
 * @param[in]      rxChComp Rx channel phase/gain compensation coefficients
 *
 * @param[in]      numObj Number of detected points
 *
 * @param[in]      numAnt number of antennas
 *
 * @param[in]      symbolsIn Pointer to array of input symbols
 *
 * @param[out]     symbolsOut Pointer to array of output symbols
 *
 * @return         void
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static void AoAProcHWA_rxChanPhaseBiasCompensation(cmplx16ImRe_t *rxChComp,
                                                   uint32_t numObj,
                                                   uint32_t numAnt,
                                                   cmplx16ImRe_t *symbolsIn,
                                                   cmplx16ImRe_t *symbolsOut)
{
    int32_t Re, Im;
    uint32_t i, j;
    uint32_t antIndx;
    uint32_t objIdx;

    for (objIdx = 0; objIdx < numObj; objIdx++)
    {
        j = 0;
        /* Compensation of azimuth antennas */
        for (antIndx = 0; antIndx < numAnt; antIndx++)
        {
            i = objIdx*numAnt + antIndx;
            Re = (int32_t) symbolsIn[i].real * (int32_t) rxChComp[j].real -
                 (int32_t) symbolsIn[i].imag * (int32_t) rxChComp[j].imag;

            Im = (int32_t) symbolsIn[i].real * (int32_t) rxChComp[j].imag +
                 (int32_t) symbolsIn[i].imag * (int32_t) rxChComp[j].real;
            MATHUTILS_ROUND_AND_SATURATE_Q15(Re);
            MATHUTILS_ROUND_AND_SATURATE_Q15(Im);

            symbolsOut[i].real = (int16_t) Re;
            symbolsOut[i].imag = (int16_t) Im;
            j++;
        }
    }
}



/**
 *  @b Description
 *  @n
 *      The function configures input EDMA for single bin 2D-FFT computation using HWA.
 *      It configures EDMA to copy data from 1D-FFT radar cube matrix to HWA memory for
 *      single bin 2D-FFT calculation. The radar cube matrix format must be defined as @ref DPIF_RADARCUBE_FORMAT_1.
 *      The EDMA is configured to copy 1D-FFT antenna symbols of desired range bin
 *      into HWA memory and trigger HWA to execute 2D-FFT computation. The data copy is done per trigger event.
 *      The EDMA is configured to increment source address to the next range bin.
 *      The function configures EDMA to run in one of the two modes:
 *      a) to compute 2D-FFT zero Doppler symbols for azimuth heatmap display,
 *      where the chain of input EDMA+HWA FFT+output EDMA is triggered with a
 *      single event to run in the loop and compute zero Doppler 2D FFT symbols
 *      for all range bins, or
 *      b) to compute 2D-FFT symbols for all detected objects where the EDMA
 *      source address is reconfigured by R4F for each detected object according to its range index.
 *
 *  @param[in]  hwaHandle     Handle to HWA
 *  @param[in]  res           Pointer to hardware resources structure
 *  @param[in]  DPParams      Pointer to profile parameters
 *  @param[in]  srcBuffAddr   Address of the 1D-FFT radar cube matrix
 *  @param[in]  dstBuffAddr   Address of HWA memory
 *  @param[in]  numIterations NUmber of iterations
 *  @param[in]  forAzimuthHeatMap true: use for azimuth heatmap, false: use for detected points
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t AoAProcHWA_config2DSingleBinInp_EDMA
(
    HWA_Handle  hwaHandle,
    DPU_AoAProcHWA_HW_Resources        *res,
    DPU_AoAProcHWA_StaticConfig  *DPParams,
    uint32_t           srcBuffAddr,
    uint32_t           dstBuffAddr,
    uint32_t           numIterations,
    bool               forAzimuthHeatMap
)
{
    int32_t     retVal = 0;
    volatile bool       edmaTransComplete = false ;
    uint16_t numBlocks;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncABCfg    syncABCfg;


    if (forAzimuthHeatMap)
    {
        /* Load to HWA only azimuth antennas */
        numBlocks = DPParams->numVirtualAntAzim * DPParams->numDopplerChirps;
    }
    else
    {
        /* Load to HWA all antennas, azimuth and elevation */
        numBlocks = (DPParams->numVirtualAntAzim + DPParams->numVirtualAntElev) * DPParams->numDopplerChirps;
    }
    /* Copy one Range bin to HWA memory */
    chainingCfg.chainingChan                  = res->edmaHwaInSignature.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = true;

    syncABCfg.srcAddress  = srcBuffAddr;
    syncABCfg.destAddress = dstBuffAddr;
    syncABCfg.aCount      = sizeof(uint32_t);
    syncABCfg.bCount      = numBlocks;
    syncABCfg.cCount      = numIterations;
    syncABCfg.srcBIdx     = DPParams->numRangeBins * sizeof(uint32_t);
    syncABCfg.dstBIdx     = sizeof(uint32_t);
    syncABCfg.srcCIdx     = sizeof(uint32_t);
    syncABCfg.dstCIdx     = 0;

    retVal = DPEDMA_configSyncAB(res->edmaHandle,
                                 &res->edmaHwaIn,
                                 &chainingCfg,
                                 &syncABCfg,
                                 false,//isEventTriggered
                                 false, //isIntermediateTransferCompletionEnabled
                                 false,//isTransferCompletionEnabled
                                 NULL, //transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg
    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Copy one hot signature to trigger HWA 2D FFT */
    retVal = DPEDMAHWA_configOneHotSignature(res->edmaHandle,
                                             &res->edmaHwaInSignature,
                                             hwaHandle,
                                             res->hwaCfg.paramSetStartIdx,
                                             false);

    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }
exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      The function configures output EDMA for single bin 2D-FFT computation using HWA.
 *      It configures EDMA to copy 2D-FFT antenna symbols at desired Doppler bin
 *      from the HWA memory to the output buffer in local R4F memory.
 *
 *  @param[in]  res            Pointer to hardware resources
 *  @param[in]  DPParams       Pointer to static configuration parameters
 *  @param[in]  srcBuffPtr     Source buffer address, points to zero bin of 2D FFT output in HWA memory. THe source address is incremented
 *  @param[in]  dstBuffPtr     Destination address, local memory of CPU
 *  @param[in]  numIterations  Number of iterations, (number of HWA loops)
 *  @param[in]  chainEnable    true: chaining enabled to the input EDMA. This mode is used for azimuth heatmap, false: chaining disabled.
 *
 *  @retval EDMA error code, see EDMA API.
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 */
static int32_t AoAProcHWA_config2DSingleBinOut_EDMA
(
    DPU_AoAProcHWA_HW_Resources        *res,
    DPU_AoAProcHWA_StaticConfig  *DPParams,
    uint32_t            srcBuffPtr,
    uint32_t            dstBuffPtr,
    uint32_t            numIterations,
    bool                chainEnable
)
{
    int32_t     retVal;
    int16_t     dstIterationIncrBytes;
    bool isFinalTransferCompletionEnabled;
    bool isIntermediateTransferCompletionEnabled;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncABCfg    syncABCfg;
    uint16_t aCount;

    if (chainEnable)
    {
        /* This mode is used for azimuth heatmap */
        dstIterationIncrBytes = DPParams->numVirtualAntAzim * sizeof(uint32_t);
        isIntermediateTransferCompletionEnabled = false;
        isFinalTransferCompletionEnabled = true;
        aCount = DPParams->numVirtualAntAzim * sizeof(uint32_t);
    }
    else
    {
        dstIterationIncrBytes = 0;
        isIntermediateTransferCompletionEnabled = true;
        isFinalTransferCompletionEnabled = true;
        aCount = (DPParams->numVirtualAntAzim + DPParams->numVirtualAntElev) * sizeof(uint32_t);
    }

    /* Copy one Range bin to HWA memory */
    chainingCfg.chainingChan                  = res->edmaHwaIn.channel;
    chainingCfg.isIntermediateChainingEnabled = chainEnable;
    chainingCfg.isFinalChainingEnabled        = chainEnable;
   
    syncABCfg.srcAddress  = srcBuffPtr;
    syncABCfg.destAddress = dstBuffPtr;
    syncABCfg.aCount      = aCount;
    syncABCfg.bCount      = 1;
    syncABCfg.cCount      = numIterations;
    syncABCfg.srcBIdx     = 0; 
    syncABCfg.dstBIdx     = 0;
    syncABCfg.srcCIdx     = 0;
    syncABCfg.dstCIdx     = dstIterationIncrBytes;
         
    retVal = DPEDMA_configSyncAB(res->edmaHandle,
                                 &res->edmaHwaOut,
                                 &chainingCfg,
                                 &syncABCfg,
                                 true,//isEventTriggered
                                 isIntermediateTransferCompletionEnabled,
                                 isFinalTransferCompletionEnabled,
                                 NULL, //transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg
   
    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      The function is called during the single bin 2D-FFT calculation in the
 *      loop for every detected point to set the EDMA source address pointing
 *      to 1D-FFT symbols in the radar cube matrix at the object range bin.
 *
 *  @param[in]  res   Pointer to hardware resources structure
 *  @param[in]  srcBuffAddr   Address of the 1D-FFT radar cube matrix pointing to
 *              desired range bin of 1D-FFT antenna symbols

 *  @retval
 *      Success     - 0
 *  @retval
 *      EDMA Error code       - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 */
static int32_t AoAProcHWA_setSource2DSingleBinInp_EDMA
(
        DPU_AoAProcHWA_HW_Resources        *res,
        uint32_t            srcBuffAddr
)
{
    int32_t     retVal;

    retVal = EDMA_setSourceAddress(res->edmaHandle,
          res->edmaHwaIn.channel,
          SOC_translateAddress(srcBuffAddr, SOC_TranslateAddr_Dir_TO_EDMA, NULL));

    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      The function is called during the single bin 2D-FFT calculation in the
 *      loop for every detected point to set the EDMA source address pointing
 *      to 2D-FFT symbols in the HWA FFT output memory at the object Doppler bin.
 *
 *  @param[in]  res   Pointer to hardware resources structure
 *  @param[in]  srcBuffAddr   Address in the HWA memory pointing to desired Doppler
 *              bin of 2D FFT antenna symbols to read out from HWA

 *  @retval
 *      Success     - 0
 *  @retval
 *      EDMA Error code       - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 */
static int32_t AoAProcHWA_setSource2DSingleBinOut_EDMA
(
        DPU_AoAProcHWA_HW_Resources *res,
        uint32_t srcBuffAddr
)
{
    int32_t     retVal;

    retVal = EDMA_setSourceAddress(res->edmaHandle,
          res->edmaHwaOut.channel,
          SOC_translateAddress(srcBuffAddr, SOC_TranslateAddr_Dir_TO_EDMA, NULL));

    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      The funciton configures HWA for angle (azimuth/elevation) estimation.
 *      It configures one HWA Param set when only azimuth antennas are used,
 *      or three Param sets when azimuth+elevation antennas are used.
 *      It also configures the HWA common register.
 *
 *  @param[in]  hwaHandle       HWA handle
 *  @param[in]  res             Pointer to hardware resources structure
 *  @param[in]  DPParams        Data path parameters
 *  @param[in]  numObjOut       Number of detected objects
 *  @param[in]  hwaMemBankAddr  Addresses of HWA memory banks
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
int32_t AoAProcHWA_dataPathConfigHwaAoa
(
    HWA_Handle  hwaHandle,
    DPU_AoAProcHWA_HW_Resources *res,
    DPU_AoAProcHWA_StaticConfig  *DPParams,
    uint32_t            numObjOut,
    uint32_t    hwaMemBankAddr[4]
)
{
    int32_t             retVal = 0;
    HWA_CommonConfig    hwaCommonConfig;
    int32_t             numHwaParamSets3D;

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    if(DPParams->numVirtualAntElev == 0)
    {
        retVal =
            HWAutil_configAngleEstAzimuth(hwaHandle,
            res->hwaCfg.paramSetStartIdx,
            (DPParams->numVirtualAntAzim + DPParams->numVirtualAntElev),
            DPU_AOAPROCHWA_NUM_ANGLE_BINS,
            numObjOut,
            ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[AOAHWA_ANGLE_INP_HWA_MEM_BANK]),
            ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[AOAHWA_ANGLE_AZIM_ABS_OUT_HWA_MEM_BANK]));

        if (retVal != 0)
        {
            goto exit;
        }
        numHwaParamSets3D = 1;
    }
    else
    {
        retVal =
        HWAutil_configAngleEstAzimuthElevation(hwaHandle,
            res->hwaCfg.paramSetStartIdx,
            DPParams->numVirtualAntAzim,
            DPParams->numVirtualAntElev,
            DPU_AOAPROCHWA_NUM_ANGLE_BINS,
            numObjOut,
            ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[AOAHWA_ANGLE_INP_HWA_MEM_BANK]),
            ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[AOAHWA_ANGLE_AZIM_ABS_OUT_HWA_MEM_BANK]),
            ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[AOAHWA_ANGLE_AZIM_CPLX_OUT_HWA_MEM_BANK]),
            ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[AOAHWA_ANGLE_ELEV_CPLX_OUT_HWA_MEM_BANK]));

        if (retVal != 0)
        {
            goto exit;
        }
        numHwaParamSets3D = 3;
    }

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_NUMLOOPS |
        HWA_COMMONCONFIG_MASK_PARAMSTARTIDX |
        HWA_COMMONCONFIG_MASK_PARAMSTOPIDX |
        HWA_COMMONCONFIG_MASK_FFT1DENABLE |
        HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD;

    hwaCommonConfig.numLoops = 1;
    hwaCommonConfig.paramStartIdx = res->hwaCfg.paramSetStartIdx;
    hwaCommonConfig.paramStopIdx = res->hwaCfg.paramSetStartIdx + (numHwaParamSets3D - 1);
    hwaCommonConfig.fftConfig.fft1DEnable = HWA_FEATURE_BIT_DISABLE;
    hwaCommonConfig.fftConfig.interferenceThreshold = 0xFFFFFF;

    retVal = HWA_configCommon(hwaHandle, &hwaCommonConfig);
    if (retVal != 0)
    {
        goto exit;
    }
exit:
    return(retVal);
}

/**
 *   @b Description
 *   @n
 *      The function configures HWA Param sets for 2D-FFT single range bin
 *      calculation on 1D-FFT data from radarCube. The 1D FFT symbols are
 *      arranged in HWA memory as three dimensional array as
 *      X[Number of Tx antennas][Number of virtual chirps][Number of Rx antennas]
 *      THe output symbols are arrange as three dimensional array as
 *      Y[Number of Doppler bins][Number of Tx antnnas][Number of Rx antennas]
 *
 *   @param[in] hwaHandle               HWA driver handle
 *   @param[in] paramSetStartIdx        HWA parameter set start index
 *   @param[in] numDopplerChirps        Number of Doppler Chirps
 *   @param[in] numDopplerBins          Number of Doppler bins
 *   @param[in] numOutSamplesPerFft     Number of output samples to generate from Doppler FFT size
 *   @param[in] numRxAnt                Number of Rx antennas
 *   @param[in] numTxAntIn              Number of Tx antennas
 *   @param[in] numTxAntOut             Number of Rx antennas sets pulled output
 *   @param[in] windowOffset            Starting address of the FFT window in HWA RAM
 *   @param[in] winSym                  Flag to indicate if HWA windowing is symmetric
                                        see HWA_WINDOW_SYMM definitions in HWA driver's
                                        doxygen documentation
 *   @param[in] dmaTriggerSource        DMA trigger source channel
 *   @param[in] dmaTriggerDest          DMA channel to trigger output EDMA to copy data out of HWA
 *   @param[in] hwaMemAzimSource        HWA memory offset of the set of input symbols corresponding to one Tx antenna
 *   @param[in] hwaMemAzimDest          HWA memory offset of the set of output symbols corresponding to one Tx antnna
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *   \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t HWAutil_configDopplerFFTSingleRangeBin
(
    HWA_Handle      hwaHandle,
    uint8_t         paramSetStartIdx,
    uint32_t        numDopplerChirps,
    uint32_t        numDopplerBins,
    uint32_t        numOutSamplesPerFft,
    uint8_t         numRxAnt,
    uint8_t         numTxAntIn,
    uint8_t         numTxAntOut,
    uint32_t        windowOffset,
    uint8_t         winSym,
    uint8_t         dmaTriggerSource,
    uint8_t         dmaTriggerDest,
    uint16_t        hwaMemAzimSource,
    uint16_t        hwaMemAzimDest
)
{
    HWA_ParamConfig hwaParamCfg;
    HWA_InterruptConfig     paramISRConfig;
    int32_t retVal = 0;
    uint8_t         numVirtualAntOut = numTxAntOut * numRxAnt;
    int32_t i;

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    memset( (void*) &hwaParamCfg, 0, sizeof(HWA_ParamConfig));


    //Doppler FFT
    hwaParamCfg.dmaTriggerSrc = dmaTriggerSource;
    hwaParamCfg.accelMode = HWA_ACCELMODE_FFT; //do FFT

    hwaParamCfg.source.srcAcnt = numDopplerChirps - 1; //size in samples - 1

    hwaParamCfg.source.srcAIdx = numRxAnt * sizeof(uint32_t); //
    hwaParamCfg.source.srcBcnt = numRxAnt - 1;
    hwaParamCfg.source.srcBIdx = sizeof(uint32_t); //should be dont care
    hwaParamCfg.source.srcShift = 0; //no shift
    hwaParamCfg.source.srcCircShiftWrap = 0; //no shift
    hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED; //signed
    hwaParamCfg.source.srcConjugate = 0; //no conjugate
    hwaParamCfg.source.srcScale = 0;
    hwaParamCfg.source.bpmEnable = 0; //bpm removal not enabled
    hwaParamCfg.source.bpmPhase = 0; //dont care

    hwaParamCfg.dest.dstAcnt = numOutSamplesPerFft -1;
    hwaParamCfg.dest.dstAIdx = numVirtualAntOut * sizeof(uint32_t);
    hwaParamCfg.dest.dstBIdx = sizeof(uint32_t) ; //should be dont care
    hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    hwaParamCfg.dest.dstConjugate = 0; //no conjugate
    hwaParamCfg.dest.dstScale = 8 ;
    hwaParamCfg.dest.dstSkipInit = 0; // no skipping

    hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_floorLog2(numDopplerBins);
    hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0x3FF; //LSB fftSize bits are relevant
    hwaParamCfg.accelModeArgs.fftMode.interfZeroOutEn = 0; //disabled
    hwaParamCfg.accelModeArgs.fftMode.windowEn = 1; //enabled
    hwaParamCfg.accelModeArgs.fftMode.windowStart = windowOffset; //start of window RAM
    hwaParamCfg.accelModeArgs.fftMode.winSymm = winSym;
    hwaParamCfg.accelModeArgs.fftMode.winInterpolateMode = 0;
    hwaParamCfg.accelModeArgs.fftMode.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
    hwaParamCfg.accelModeArgs.fftMode.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

    hwaParamCfg.complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

    for (i = 0; i < numTxAntOut; i++)
    {
        if (i == 0)
        {
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
        }
        else
        {
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        }
        hwaParamCfg.source.srcAddr = hwaMemAzimSource + i * numDopplerChirps * numRxAnt * sizeof(uint32_t); // address is relative to start of MEM0
        hwaParamCfg.dest.dstAddr = hwaMemAzimDest + i * numRxAnt * sizeof(uint32_t); // address is relative to start of MEM0
        retVal = HWA_configParamSet(hwaHandle, (uint8_t) (paramSetStartIdx + i), &hwaParamCfg, NULL);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Disable DMA/interrupt hookup to all except the last one */
        retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                              paramSetStartIdx + i,
                                              HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
        /* Output interrupt to DMA only by last param set */
        if (i == (numTxAntOut-1))
        {
            /* Last param set: enable the DMA hookup to this paramset so that data gets copied out */
            paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
            paramISRConfig.dma.dstChannel = dmaTriggerDest;  /* EDMA channel to trigger to copy the data out */
            paramISRConfig.cpu.callbackArg = NULL;
            retVal = HWA_enableParamSetInterrupt(hwaHandle,
                                                 paramSetStartIdx + i,
                                                 &paramISRConfig);
        }
        if (retVal != 0)
        {
             goto exit;
        }
    }
exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Configures HWA common register. It sets the Params start index and
 *      number of iteration loops.
 *
 *   @param[in] hwaHandle               HWA driver handle
 *   @param[in] numLoops                Number of HWA loops
 *   @param[in] paramStartIdx           Param start index
 *   @param[in] numParams               Number of params
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t AoAProc_HWAutil_configCommon (HWA_Handle hwaHandle,
                                                uint16_t numLoops,
                                                uint16_t paramStartIdx,
                                                uint16_t numParams)
{
    int32_t errCode = 0;
    HWA_CommonConfig    hwaCommonConfig;

    /* Disable the HWA */
     errCode = HWA_enable(hwaHandle, 0);
     if (errCode != 0)
     {
         goto exit;
     }

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    hwaCommonConfig.configMask =
        HWA_COMMONCONFIG_MASK_NUMLOOPS |
        HWA_COMMONCONFIG_MASK_PARAMSTARTIDX |
        HWA_COMMONCONFIG_MASK_PARAMSTOPIDX |
        HWA_COMMONCONFIG_MASK_FFT1DENABLE |
        HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD;

    hwaCommonConfig.numLoops = numLoops;
    hwaCommonConfig.paramStartIdx = paramStartIdx;
    hwaCommonConfig.paramStopIdx = paramStartIdx + (numParams-1);

    hwaCommonConfig.fftConfig.fft1DEnable = HWA_FEATURE_BIT_DISABLE;
    hwaCommonConfig.fftConfig.interferenceThreshold = 0xFFFFFF;
    errCode = HWA_configCommon(hwaHandle, &hwaCommonConfig);
exit:
    return (errCode);
}

/**
 *  @b Description
 *  @n
 *      This function configures one HWA Param set to calculate azimuth FFTs
 *      on a set of detected objects. It is used when only azimuth antennas are enabled.
 *
 *  @param[in] hwaHandle          HWA driver handle
 *  @param[in] paramSetStartIdx   Param start index
 *  @param[in] numVirtualAnt      Number of virtual antennas
 *  @param[in] fftOutSize         Azimuth FFT size
 *  @param[in] numIter            Number of iterations (number of detected objects)
 *  @param[in] hwaSourceBufOffset Input data address in HWA
 *  @param[in] hwaDestBufOffset   Output data address in HWA
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t HWAutil_configAngleEstAzimuth
(
    HWA_Handle      hwaHandle,
    uint8_t         paramSetStartIdx,
    uint32_t        numVirtualAnt,
    uint32_t        fftOutSize,
    uint32_t        numIter,
    uint16_t        hwaSourceBufOffset,
    uint16_t        hwaDestBufOffset
)
{
    HWA_ParamConfig hwaParamCfg;
    int32_t retVal = 0;

    memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
    hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

    hwaParamCfg.source.srcAddr = hwaSourceBufOffset;

    hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
    hwaParamCfg.source.srcAcnt = numVirtualAnt-1;
    hwaParamCfg.source.srcAIdx = sizeof(uint32_t);
    hwaParamCfg.source.srcBIdx = numVirtualAnt * sizeof(uint32_t);
    hwaParamCfg.source.srcBcnt = numIter-1;
    hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg.source.srcScale = 8;

    hwaParamCfg.complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg.accelModeArgs.fftMode.magLogEn = HWA_FFT_MODE_MAGNITUDE_ONLY_ENABLED;
    hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftOutSize);//assumes power of 2;
    hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
    hwaParamCfg.accelModeArgs.fftMode.winSymm = 1;
    hwaParamCfg.accelModeArgs.fftMode.windowStart = 0; //do not care
    hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

    hwaParamCfg.dest.dstAddr =  hwaDestBufOffset;
    hwaParamCfg.dest.dstAcnt = fftOutSize-1;
    hwaParamCfg.dest.dstAIdx = sizeof(uint16_t);//abs
    hwaParamCfg.dest.dstBIdx = fftOutSize * sizeof(uint16_t);
    hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;
    hwaParamCfg.dest.dstScale = 3;
    hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;

    retVal = HWA_configParamSet(hwaHandle, paramSetStartIdx, &hwaParamCfg, NULL);
    if (retVal != 0)
    {
         goto exit;
    }

    /* Disable DMA/interrupt hookup to all except the last one */
    retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                          paramSetStartIdx,
                                          HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      This function configures three HWA Param sets to calculate azimuth FFTs
 *      on a set of detected objects. It is used when both azimuth and elevation
 *      antennas are enabled. The param sets are programmed to compute:
 *      a) FFT on azimuth antennas, log2 magnitude outputs,
 *      b) FFT on azimuth antennas, complex symbol outputs,
 *      c) FFT on elevation antennas, complex symbol outputs.
 *
 *  @param[in]  hwaHandle                 HWA handle
 *  @param[in]  paramSetStartIdx          HWA parameters starting index
 *  @param[in]  numVirtualAntAzim         Number of azimuth antennas
 *  @param[in]  numVirtualAntElev         Number of elevation antennas
 *  @param[in]  fftOutSize                Azimuth FFT size
 *  @param[in]  numIter                   Number of iterations (number of detected objects)
 *  @param[in]  hwaSourceBufOffset    Input starting address of azimuth antennas
 *  @param[out] hwaDestAzimAbsBufOffset   Output starting address of azimuth antennas, log2 magnitudes
 *  @param[out] hwaDestAzimCplxBufOffset  Output starting address of azimuth antennas, complex values
 *  @param[out] hwaDestElevCplxBufOffset  Output starting address of elevation antennas, complex values
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t HWAutil_configAngleEstAzimuthElevation
(
    HWA_Handle      hwaHandle,
    uint8_t         paramSetStartIdx,
    uint32_t        numVirtualAntAzim,
    uint32_t        numVirtualAntElev,
    uint32_t        fftOutSize,
    uint32_t        numIter,
    uint16_t        hwaSourceBufOffset,
    uint16_t        hwaDestAzimAbsBufOffset,
    uint16_t        hwaDestAzimCplxBufOffset,
    uint16_t        hwaDestElevCplxBufOffset
)
{
    HWA_ParamConfig hwaParamCfg;
    int32_t retVal = 0;
    int32_t i;

    /**************First Param set computes the complex values of the elevation-FFT **********/
    memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
    hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

    hwaParamCfg.source.srcAddr = (uint16_t) (hwaSourceBufOffset + (numVirtualAntAzim * sizeof(cmplx16ImRe_t)));
    hwaParamCfg.source.srcAcnt = numVirtualAntElev-1;
    hwaParamCfg.source.srcAIdx = sizeof(uint32_t);
    hwaParamCfg.source.srcBIdx = (numVirtualAntAzim + numVirtualAntElev) * sizeof(uint32_t);
    hwaParamCfg.source.srcBcnt = numIter-1;
    hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg.source.srcScale = 8;


    hwaParamCfg.complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftOutSize);//assumes power of 2;
    hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
    hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

    hwaParamCfg.dest.dstAddr =  (uint16_t) hwaDestElevCplxBufOffset;
    hwaParamCfg.dest.dstAcnt = fftOutSize-1;
    hwaParamCfg.dest.dstAIdx = sizeof(uint32_t);
    hwaParamCfg.dest.dstBIdx = fftOutSize * sizeof(uint32_t);
    hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg.dest.dstScale = 3;

    retVal = HWA_configParamSet(hwaHandle, paramSetStartIdx, &hwaParamCfg, NULL);
    if (retVal != 0)
    {
        goto exit;
    }

    /**************Second Param set computes the absolute values of the azimuth-FFT **********/
    memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
    hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

    hwaParamCfg.source.srcAddr = (uint16_t) hwaSourceBufOffset;
    hwaParamCfg.source.srcAcnt = numVirtualAntAzim-1;
    hwaParamCfg.source.srcAIdx = sizeof(uint32_t);
    hwaParamCfg.source.srcBIdx = (numVirtualAntAzim + numVirtualAntElev) * sizeof(uint32_t);
    hwaParamCfg.source.srcBcnt = numIter-1;
    hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg.source.srcScale = 8;

    hwaParamCfg.complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg.accelModeArgs.fftMode.magLogEn = HWA_FFT_MODE_MAGNITUDE_ONLY_ENABLED;
    hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftOutSize);//assumes power of 2;
    hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
    hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

    hwaParamCfg.dest.dstAddr =  (uint16_t) hwaDestAzimAbsBufOffset;
    hwaParamCfg.dest.dstAcnt = fftOutSize-1;
    hwaParamCfg.dest.dstAIdx = sizeof(uint16_t);//abs
    hwaParamCfg.dest.dstBIdx = fftOutSize * sizeof(uint16_t);
    hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;
    hwaParamCfg.dest.dstScale = 3;
    hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;

    retVal = HWA_configParamSet(hwaHandle, (uint8_t) (paramSetStartIdx + 1), &hwaParamCfg, NULL);
    if (retVal != 0)
    {
        goto exit;
    }

   /**************Third Param set computes the complex values of the azimuth-FFT **********/
    memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
    hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

    hwaParamCfg.source.srcAddr = (uint16_t) hwaSourceBufOffset;
    hwaParamCfg.source.srcAcnt = numVirtualAntAzim-1;
    hwaParamCfg.source.srcAIdx = sizeof(uint32_t);
    hwaParamCfg.source.srcBIdx = (numVirtualAntAzim + numVirtualAntElev) * sizeof(uint32_t);
    hwaParamCfg.source.srcBcnt = numIter-1;
    hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg.source.srcScale = 8;

    hwaParamCfg.complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(fftOutSize);//assumes power of 2;
    hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
    hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

    hwaParamCfg.dest.dstAddr =  (uint16_t) hwaDestAzimCplxBufOffset;
    hwaParamCfg.dest.dstAcnt = fftOutSize-1;
    hwaParamCfg.dest.dstAIdx = sizeof(uint32_t);
    hwaParamCfg.dest.dstBIdx = fftOutSize * sizeof(uint32_t);
    hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg.dest.dstScale = 3;

    retVal = HWA_configParamSet(hwaHandle, (uint8_t) (paramSetStartIdx + 2), &hwaParamCfg, NULL);
    if (retVal != 0)
    {
         goto exit;
    }

    /* Disable DMA/interrupt hookup to all param sets */
    for (i=0; i<3; i++)
    {
        retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                              paramSetStartIdx + i,
                                              HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
        if (retVal != 0)
        {
             goto exit;
        }
    }
exit:
    return(retVal);
}


/**
 *  @b Description
 *  @n
 *      Trigger Single range bin 2D FFT processing.
 *
 *  @param[in] edmaHandle   EDMA handle
 *  @param[in] hwaHandle    HWA handle
 *  @param[in] edmaChanId   EDMA channel number to be triggered
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
int32_t AoAProcHWA_dataPathTrigger2DSingleBin
(
    EDMA_Handle         edmaHandle,
    HWA_Handle          hwaHandle,
    uint8_t             edmaChanId
)
{
    int32_t retVal = 0U;

    /* Trigger EDMA */
    retVal = EDMA_startDmaTransfer(edmaHandle, edmaChanId);
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
 *      Wait for angle estimation to complete. This is a blocking function.
 *
 *  @param[in] semaHandle  Semaphore handle
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *
 */
int32_t AoAProcHWA_dataPathWait2DFFTDetObj(SemaphoreP_Handle semaHandle)
{
    return((int32_t) SemaphoreP_pend(semaHandle, SemaphoreP_WAIT_FOREVER));
}


/**
 *  @b Description
 *  @n
 *      This function is called per object and it calculates its x/y/z coordinates
 *      based on Azimuth FFT output. It stores the coordinates
 *      to the output list of type @ref DPIF_PointCloudCartesian_t and it also
 *      calculates object's SNR and the noise level and writes into the side
 *      information list of type @ref DPIF_PointCloudSideInfo_t
 *
 *  @param[in] azimFFTPtr Pointer to the FFT complex output of azimuth antennas
 *
 *  @param[in] elevFFTPtr Pointer to the FFT complex output of elevation antennas
 *
 *  @param[in] aoaHwaObj Pointer to AoA DPU internal object
 *
 *  @param[in] objOutIdx Index of the current object in the output lists @ref DPIF_PointCloudCartesian_t and @ref DPIF_PointCloudSideInfo_t
 *
 *  @param[in] objInIdx Index of the current object in CFAR output list @ref DPIF_CFARDetList_t
 *
 *  @param[in] objInHwaIdx Index of the current object angle FFT data in HWA memory
 *
 *  @param[in] maxIdx Peak position in the FFT output of azimuth antennas
 *
 *  @param[in] peakVal Value of the FFT output of azimuth antennas
 *
 *  @retval objOutIdx Index for the next object in the output list
 *
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
uint32_t AoAProcHWA_XYZestimation
(
    uint16_t    *azimFFTPtr,
    uint16_t    *elevFFTPtr,
    AOAHwaObj   *aoaHwaObj,
    uint32_t    objOutIdx,
    uint32_t    objInIdx,
    uint32_t    objInHwaIdx,
    uint32_t    maxIdx,
    uint32_t    peakVal
)
{
    int32_t         sMaxIdx;
    float           temp;
    float           Wx, Wz;
    float           range;
    float           limitScale;
    float           x, y, z;
    float           peakAzimRe, peakAzimIm, peakElevRe, peakElevIm;
    uint32_t numAngleBins;
    DPU_AoAProcHWA_HW_Resources            *res = &aoaHwaObj->res;
    DPIF_CFARDetList        *objIn = res->cfarRngDopSnrList;
    DPIF_PointCloudCartesian *objOut = res->detObjOut;
    DPIF_PointCloudSideInfo *objOutSideInfo = res->detObjOutSideInfo;

    DPU_AoAProcHWA_StaticConfig *params = &aoaHwaObj->params;
    uint16_t numDopplerBins = params->numDopplerBins;

    numAngleBins = DPU_AOAPROCHWA_NUM_ANGLE_BINS;

    if (params->numVirtualAntElev > 0)
    {
        peakAzimIm = (float) ((int16_t) (*(azimFFTPtr + objInHwaIdx*2*numAngleBins + maxIdx*2)));
        peakAzimRe = (float) ((int16_t) (*(azimFFTPtr + objInHwaIdx*2*numAngleBins + maxIdx*2+1)));
        peakElevIm = (float) ((int16_t) (*(elevFFTPtr + objInHwaIdx*2*numAngleBins + maxIdx*2)));
        peakElevRe = (float) ((int16_t) (*(elevFFTPtr + objInHwaIdx*2*numAngleBins + maxIdx*2+1)));
    }

    range = objIn[objInIdx].rangeIdx * params->rangeStep;

    /* Compensate for range bias */
    range -= aoaHwaObj->dynLocalCfg.compRxChanCfg.rangeBias;
    if (range < 0)
    {
        range = 0;
    }

    if(maxIdx > (numAngleBins/2 -1))
    {
        sMaxIdx = maxIdx - numAngleBins;
    }
    else
    {
        sMaxIdx = maxIdx;
    }

    Wx = 2 * (float) sMaxIdx / numAngleBins;
    /* Check if it is within configured field of view */

    x = range * Wx;

    if (params->numVirtualAntElev > 0)
    {
        Wz = atan2(peakAzimIm * peakElevRe - peakAzimRe * peakElevIm,
                   peakAzimRe * peakElevRe + peakAzimIm * peakElevIm)/PI_ + (2 * Wx);

        if (Wz > 1)
        {
            Wz = Wz - 2;
        }
        else if (Wz < -1)
        {
            Wz = Wz + 2;
        }
        /* Check if it is within configured field of view */
        if((Wz < aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minElevationSineVal) || (Wz > aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxElevationSineVal))
        {
            goto exit;
        }

        z = range * Wz;
        /*record wz for debugging/testing*/
        res->detObjElevationAngle[objOutIdx] = Wz;
        limitScale = sqrt(1 - Wz*Wz);
    }
    else
    {
        z = 0;
        limitScale = 1;
    }

    if((Wx < (limitScale * aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minAzimuthSineVal)) ||
       (Wx > (limitScale * aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxAzimuthSineVal)))
    {
        goto exit;
    }

    temp = range*range -x*x -z*z;
    if (temp > 0)
    {
        y = sqrt(temp);
    }
    else
    {
        goto exit;
    }

    objOut[objOutIdx].x = x;
    objOut[objOutIdx].y = y;
    objOut[objOutIdx].z = z;

    objOut[objOutIdx].velocity = params->dopplerStep *
        DOPPLER_IDX_TO_SIGNED(objIn[objInIdx].dopplerIdx, numDopplerBins);
    objOutSideInfo[objOutIdx].noise = objIn[objInIdx].noise;
    objOutSideInfo[objOutIdx].snr = objIn[objInIdx].snr;
    res->detObj2dAzimIdx[objOutIdx] = maxIdx;

    objOutIdx++;

exit:
    return (objOutIdx);
}

/**
 *   @b Description
 *   @n
 *      this function calculates doppler compensation index for a given Doppler
 *      index of the object. It first converts Doppler index to the signed value
 *      and then divides by number of Tx antennas (number TDM MIMO Tx antennas).
 *      The type of return value is float.
 *
 *  @param[in]  dopplerIdx           Doppler index 
 *
 *  @param[in]  numDopplerBins       Number of Doppler bins
 *
 *  @param[in]  numTxAnt             Number of Tx Antennas
 *
 *  @return Doppler compensation index
 *
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static float aoaHwa_calcCompIdx
(
    uint16_t dopplerIdx,
    uint32_t numDopplerBins,
    uint32_t numTxAnt
)
{
    float      dopplerCompensationIdx;

    /* Doppler compensation index calculation */
    if (dopplerIdx >= numDopplerBins/2)
    {
        dopplerCompensationIdx =  ((int32_t) dopplerIdx - (int32_t) numDopplerBins);
    }
    else
    {
        dopplerCompensationIdx =  dopplerIdx;
    }
    
    /* Doppler phase correction is 1/2 or (1/3 in elevation case) of the phase between two chirps of the same antenna */
    dopplerCompensationIdx = dopplerCompensationIdx / (float) numTxAnt;
    if (dopplerCompensationIdx < 0)
    {
        dopplerCompensationIdx +=  (float) numDopplerBins;
    }

    return dopplerCompensationIdx;
}

/*!
 *  @b Description
 *  @n
 *      This function performs Doppler compensation on a single antenna symbol.
 *
 *  @param[in]  in           Pointer to the Input Symbol 
 *
 *  @param[in]  out          Pointer to the Output Symbol
 *
 *  @param[in]  Cos          Cosine value depending on doppler index
 *
 *  @param[in]  Sin          Sine value depending on doppler index
 *
 *  @retval None
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static void aoaHwa_dopplerComp
(
    cmplx16ImRe_t *in,
    cmplx16ImRe_t *out,
    float  Cos,
    float  Sin
)
{
    float           yRe, yIm;

    /* Rotate symbol (correct the phase) */
    yRe = in->real * Cos + in->imag * Sin;
    yIm = in->imag * Cos - in->real * Sin;
    out->real = (int16_t) yRe;
    out->imag = (int16_t) yIm;
}

/**
 *   @b Description
 *   @n
 *      Function performs Doppler compensation on antenna symbols.
 *
 *
 *  @param[in]  numDetectedObjects  Number of detected objects
 *
 *  @param[in]  srcPtr              Input pointer to antenna symbols
 *
 *  @param[in]  cfarOutList         CFAR detection list
 *
 *  @param[in]  dstPtr              Output pointer to antenna symbols
 *
 *  @param[in]  numTxAnt            Number of Tx antennas
 *
 *  @param[in]  numRxAnt            Number of physical Rx antennas
 *
 *  @param[in]  numVirtualAntAzim   Number of virtual azimuth Rx antennas
 *
 *  @param[in]  numVirtualAntElev   Number of virtual elevation Rx antennas
 *
 *  @param[in]  numDopplerBins      Number of Doppler bins
 *
 *  @return None
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static void aoaHwa_dopplerCompensation
(
    uint32_t numDetectedObjects,
    uint32_t *srcPtr,
    DPIF_CFARDetList *cfarOutList,
    uint32_t *dstPtr,
    uint32_t numTxAnt,
    uint32_t numRxAnt,
    uint32_t numVirtualAntAzim,
    uint32_t numVirtualAntElev,
    uint32_t numDopplerBins
)
{
    uint32_t   index;
    uint32_t   j;
    uint16_t   dopplerIdx;
    float      dopplerCompensationIdx;
    
    for(index = 0; index < numDetectedObjects; index ++)
    {
        dopplerIdx = cfarOutList->dopplerIdx;

        /* transfer data corresponding to azimuth virtual antennas (corresponding to chirp of antenna Tx0) */
        for(j = 0; j < numRxAnt; j++)
        {
            *dstPtr++ = *srcPtr++;
        }
        
        /* transfer data corresponding to azimuth virtual antennas (corresponding to chirp of antenna Tx1) */
        if (numVirtualAntAzim > numRxAnt)
        {
            float Cos,Sin;

            dopplerCompensationIdx = aoaHwa_calcCompIdx(dopplerIdx, numDopplerBins, numTxAnt);

            Cos = cos(2*PI_*dopplerCompensationIdx/numDopplerBins);
            Sin = sin(2*PI_*dopplerCompensationIdx/numDopplerBins);

            for(j = numRxAnt; j < numVirtualAntAzim; j++)
            {
                aoaHwa_dopplerComp((cmplx16ImRe_t *)srcPtr++, (cmplx16ImRe_t *)dstPtr++, Cos, Sin);
            }

            if (numVirtualAntElev > 0)
            {
                /* transfer data corresponding to elevation virtual antennas, (corresponding to chirp of antenna Tx2) */
                float Cos2, Sin2;
                /* Doppler phase shift is 2/3 */
                Cos2 = Cos * Cos - Sin * Sin;
                Sin2 = 2 * Cos * Sin;
                for(j = 0; j < numVirtualAntElev; j++)
                {
                    aoaHwa_dopplerComp((cmplx16ImRe_t *)srcPtr++, (cmplx16ImRe_t *)dstPtr++, Cos2, Sin2);
                }
            }
        }
        cfarOutList++;
    }
}            

/**
 *  @b  Description
 *  @n
 *      Function calculates x/y/z coordinates of the objects detected by CFAR DPU.
 *      The number of output objects may be larger than input if multiobject beam
 *      forming is enabled, and more than one objects are detected at the
 *      same range/doppler bin.
 *
 *  @param[in]  objInstartIdx      Starting index in the input object detection list
 *
 *  @param[in]  numObjIn           Number of objects detected by CFAR DPU
 *
 *  @param[in]  azimFFTAbsPtr      Pointer to azimuth FFT log2 magnitude outputs of detected objects.
 *                                 The data are laid out as uint16_t type array x[numObjIn][azimuthFFTSize]
 *
 *  @param[in]  azimFFTPtr         Pointer to azimuth antennas FFT complex outputs of detected objects.
 *                                 The data are laid out as cmplx16ImRe_t type array x[numObjIn][azimuthFFTSize]
 *
 *  @param[in]  elevFFTPtr         Pointer  to elevation antennas FFT complex outputs of detected objects
 *                                 The data are laid out as cmplx16ImRe_t type array x[numObjIn][azimuthFFTSize]
 *
 *  @param[in]  aoaHwaObj          Pointer to AoAProc DPU internal data Object
 *
 *  @param[in]  objOutIdx          Index to the next object in the output list
 *
 *  @retval    objOutIdx           Number of detected objects in the output list
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static uint32_t AoAProcHWA_angleEstimationAzimElev
(
    uint32_t        objInstartIdx,
    uint32_t        numObjIn,
    uint16_t        *azimFFTAbsPtr,
    uint16_t        *azimFFTPtr,
    uint16_t        *elevFFTPtr,
    AOAHwaObj       *aoaHwaObj,
    uint32_t        objOutIdx
)
{
    uint32_t objInIdx, j, maxVal = 0,maxIdx = 0, tempVal;
    uint32_t objInHwaIdx;
    uint16_t *origAzimFFTAbsPtr;
    DPU_AoAProcHWA_HW_Resources *res = &aoaHwaObj->res;
    uint32_t numAngleBins = DPU_AOAPROCHWA_NUM_ANGLE_BINS;
    uint16_t maxNumObj = res->detObjOutMaxSize;

    for(objInIdx = objInstartIdx, objInHwaIdx = 0; objInIdx < (objInstartIdx + numObjIn); objInIdx++, objInHwaIdx++)
    {
        if (objOutIdx >= maxNumObj)
        {
            break;
        }
        maxVal = 0;
        /* For this detected object, save starting point of "FFT absolute array",
           to be used later for the detection of the second azimuth on the same
           range and doppler*/
        origAzimFFTAbsPtr = azimFFTAbsPtr;
        for(j=0; j < numAngleBins; j++)
        {
            tempVal = *azimFFTAbsPtr++;
            if(tempVal > maxVal)
            {
                maxVal = tempVal;
                maxIdx = j;
            }
        }

        /* Estimate x,y,z */
        objOutIdx = AoAProcHWA_XYZestimation(azimFFTPtr,
                                             elevFFTPtr,
                                             aoaHwaObj,
                                             objOutIdx,
                                             objInIdx,
                                             objInHwaIdx,
                                             maxIdx,
                                             maxVal);
        if (objOutIdx >= maxNumObj)
        {
            break;
        }
        /* Multi peak azimuzth search?*/
        if (aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg.enabled)
        {
            uint32_t leftSearchIdx;
            uint32_t rightSearchIdx;
            uint32_t secondSearchLen;
            uint32_t iModAzimLen;
            uint32_t maxVal2;
            int32_t k;
            uint32_t t;
            uint16_t azimIdx = maxIdx;
            uint16_t* azimuthMag = origAzimFFTAbsPtr;

            /* Find right edge of the first peak */
            t = azimIdx;
            leftSearchIdx = (t + 1) & (numAngleBins-1);
            k = numAngleBins;
            while ((azimuthMag[t] >= azimuthMag[leftSearchIdx]) && (k > 0))
            {
                t = (t + 1) & (numAngleBins-1);
                leftSearchIdx = (leftSearchIdx + 1) & (numAngleBins-1);
                k--;
            }

            /* Find left edge of the first peak */
            t = azimIdx;
            rightSearchIdx = (t - 1) & (numAngleBins-1);
            k = numAngleBins;
            while ((azimuthMag[t] >= azimuthMag[rightSearchIdx]) && (k > 0))
            {
                t = (t - 1) & (numAngleBins-1);
                rightSearchIdx = (rightSearchIdx - 1) & (numAngleBins-1);
                k--;
            }

            secondSearchLen = ((rightSearchIdx - leftSearchIdx) & (numAngleBins-1)) + 1;
            /* Find second peak */
            maxVal2 = azimuthMag[leftSearchIdx];
            azimIdx = leftSearchIdx;
            for (t = leftSearchIdx; t < (leftSearchIdx + secondSearchLen); t++)
            {
                iModAzimLen = t & (numAngleBins-1);
                if (azimuthMag[iModAzimLen] > maxVal2)
                {
                    azimIdx = iModAzimLen;
                    maxVal2 = azimuthMag[iModAzimLen];
                }
            }
            /* Is second peak greater than threshold? */
            if ( (maxVal2 >( ((uint32_t)(maxVal * aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg.multiPeakThrsScal)))) && (objOutIdx < maxNumObj) )
            {

                /* Estimate x,y,z for second peak */
                objOutIdx = AoAProcHWA_XYZestimation(azimFFTPtr,
                                                     elevFFTPtr,
                                                     aoaHwaObj,
                                                     objOutIdx,
                                                     objInIdx,
                                                     objInHwaIdx,
                                                     azimIdx,
                                                     maxVal2);
            }
        }
    }
    return(objOutIdx);
}

/**
 *  @b Description
 *  @n
 *      The function waits for 2D FFT single bin data to be transfered to the output buffer.
 *      This is a blocking function. The function polls for EDMA transfer completion.
 *
 *  @param[in]  aoaHwaObj    Pointer to internal AoAProcHWA data object
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t  AOAHWA_waitSinglePointFftEdmaOut(AOAHwaObj * aoaHwaObj)
{
    DPU_AoAProcHWA_HW_Resources *res = &aoaHwaObj->res;
    int32_t errCode = EDMA_NO_ERROR;

    /* wait until transfer done */
    volatile bool isTransferDone;
    do {
        if ((errCode = EDMA_isTransferComplete(res->edmaHandle,
                                    res->edmaHwaIn.channel,
                                    (bool *)&isTransferDone)) != EDMA_NO_ERROR)
        {
            break;
        }
    } while (isTransferDone == false);

    return errCode;
}

/**
 *  @b Description
 *  @n
 *      The function prepares data for range-azimuth heat-map. 
 *      It calculates 2D-FFT for all virtual antennas and saves 
 *      zero Doppler values for all range bins.
 *
 *  @param[in]    aoaHwaObj     Pointer to internal AoAProcHWA data object
 *
 *  @param[inout] waitTimeLocal CPU waiting time
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t  AoAProcHWA_PrepareRangeAzimuthHeatmap(AOAHwaObj *aoaHwaObj,
                                               volatile uint32_t   *waitTimeLocal)
{
    volatile uint32_t   startTime1;
    int32_t retVal = 0;
    uint16_t numParams;
    HWA_Handle hwaHandle;
    cmplx16ImRe_t     *radarCubeBase;
    DPU_AoAProcHWA_HW_Resources *res;
    DPU_AoAProcHWA_StaticConfig *DPParams;

    res = &aoaHwaObj->res;
    DPParams = &aoaHwaObj->params;
    radarCubeBase = (cmplx16ImRe_t *)res->radarCube.data;
    hwaHandle = aoaHwaObj->hwaHandle;

    retVal =
            HWAutil_configDopplerFFTSingleRangeBin(hwaHandle,
            res->hwaCfg.paramSetStartIdx,
            DPParams->numDopplerChirps,
            DPParams->numDopplerBins,
            1,
            DPParams->numRxAntennas,
            DPParams->numTxAntennas,
            DPParams->numVirtualAntAzim/DPParams->numRxAntennas,
            res->hwaCfg.winRamOffset,
            res->hwaCfg.winSym,
            res->hwaCfg.paramSetStartIdx /* dmaTriggerSrc */,
            aoaHwaObj->dmaDestChannel,
            ADDR_TRANSLATE_CPU_TO_HWA(aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_INP_HWA_MEM_BANK]),
            ADDR_TRANSLATE_CPU_TO_HWA(aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_OUT_HWA_MEM_BANK]));
    if (retVal != 0)
    {
        goto exit;
    }

    numParams = DPParams->numVirtualAntAzim/DPParams->numRxAntennas; //DPParams->numTxAntennas;
    retVal =  AoAProc_HWAutil_configCommon (hwaHandle,
                                              DPParams->numRangeBins, //numLoops,
                                              res->hwaCfg.paramSetStartIdx, //paramStartIdx,
                                              numParams);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = AoAProcHWA_config2DSingleBinInp_EDMA(hwaHandle, res,
                                  DPParams,
                                  (uint32_t) radarCubeBase,
                                  (uint32_t)aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_INP_HWA_MEM_BANK],
                                  DPParams->numRangeBins,
                                  true);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = AoAProcHWA_config2DSingleBinOut_EDMA( res,
                                   DPParams,
                                   ((uint32_t)aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_OUT_HWA_MEM_BANK]),
                                   (uint32_t) res->azimuthStaticHeatMap,
                                   DPParams->numRangeBins,
                                   true);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(hwaHandle, 1);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Trigger single 2D FFT */
    retVal = AoAProcHWA_dataPathTrigger2DSingleBin(res->edmaHandle,
                                    hwaHandle,
                                    res->edmaHwaIn.channel); //MMW_EDMA_2DFFT_SINGLERBIN_CH_ID
    if (retVal != 0)
    {
        goto exit;
    }

    /* Wait until HWA done with FFT */
    startTime1 = Cycleprofiler_getTimeStamp();
    AoAProcHWA_dataPathWait2DFFTDetObj(aoaHwaObj->hwaDone_semaHandle);
    *waitTimeLocal += Cycleprofiler_getTimeStamp() - startTime1;

    /* Wait until EDMA output is done */
    startTime1 = Cycleprofiler_getTimeStamp();
    retVal = AOAHWA_waitSinglePointFftEdmaOut(aoaHwaObj);
    *waitTimeLocal += Cycleprofiler_getTimeStamp() - startTime1;
    if (retVal != 0)
    {
          goto exit;
    }

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Rx channel gain/phase offset compensation */
    AoAProcHWA_rxChanPhaseBiasCompensation(&aoaHwaObj->dynLocalCfg.compRxChanCfg.rxChPhaseComp[0],
                                        DPParams->numRangeBins,
                                        DPParams->numVirtualAntAzim,
                                        res->azimuthStaticHeatMap,
                                        res->azimuthStaticHeatMap);
exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function converts angle of arrival field of view values specified
 *      in degrees to the values appropriate for internal DPU comparison.
 *
 *  @param[in]  aoaHwaObj    Pointer to internal AoAProcHWA data object
 *
 *  @param[in]  fovAoaCfg    Pointer to field of view configuration
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *  @retval  None
 *
 */
void AoAProcHWA_ConvertFov(AOAHwaObj *aoaHwaObj,
                           DPU_AoAProc_FovAoaCfg *fovAoaCfg)
{
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minAzimuthSineVal = sin(fovAoaCfg->minAzimuthDeg / 180. * PI_);
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxAzimuthSineVal = sin(fovAoaCfg->maxAzimuthDeg / 180. * PI_);
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minElevationSineVal = sin(fovAoaCfg->minElevationDeg / 180. * PI_);
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxElevationSineVal = sin(fovAoaCfg->maxElevationDeg  / 180. * PI_);
}

DPU_AoAProcHWA_Handle DPU_AoAProcHWA_init
(
    DPU_AoAProcHWA_InitParams *initParams,
    int32_t*            errCode
)
{
    AOAHwaObj     *aoaHwaObj = NULL;
    HWA_MemInfo   hwaMemInfo;
    uint8_t       index;
    SemaphoreP_Params  semParams;

    if ((initParams == NULL) || (initParams->hwaHandle == NULL))
    {
        *errCode = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }

    aoaHwaObj = MemoryP_ctrlAlloc(sizeof(AOAHwaObj), 0);
    if(aoaHwaObj == NULL)
    {
        *errCode = DPU_AOAPROCHWA_ENOMEM;
        goto exit;
    }

    /* Save for debugging */
#ifdef DBG_AOA_HWA_OBJ_DPU
    if (gAoaHwaObjInd < RL_MAX_SUBFRAMES)
    {
        gAoaHwaObj[gAoaHwaObjInd++] = aoaHwaObj;
    }
#endif

    /* Set HWA bank memory address */
    *errCode =  HWA_getHWAMemInfo(initParams->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    /* Initialize memory */
    memset((void *)aoaHwaObj, 0, sizeof(AOAHwaObj));

    /* Save init config params */
    aoaHwaObj->hwaHandle   = initParams->hwaHandle;

    for (index = 0; index < hwaMemInfo.numBanks; index++)
    {
        aoaHwaObj->hwaMemBankAddr[index] = hwaMemInfo.baseAddress + index * hwaMemInfo.bankSize;
    }

    /* Create semaphore for HWA done */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    aoaHwaObj->hwaDone_semaHandle = SemaphoreP_create(0, &semParams);
    if(aoaHwaObj->hwaDone_semaHandle == NULL)
    {
        *errCode = DPU_AOAPROCHWA_ESEMA;
        goto exit;
    }

exit:
    return ((DPU_AoAProcHWA_Handle)aoaHwaObj);
}

int32_t DPU_AoAProcHWA_config
(
    DPU_AoAProcHWA_Handle    handle,
    DPU_AoAProcHWA_Config    *aoaHwaCfg
)
{
    int32_t   retVal = 0;
    AOAHwaObj *aoaHwaObj = (AOAHwaObj *)handle;

    if(aoaHwaObj == NULL)
    {
        retVal = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }

    /* Check if radar cube formats are supported. */
    if (!(aoaHwaCfg->res.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_1))
    {
        retVal = DPU_AOAPROCHWA_EINVAL__RADARCUBE_DATAFORMAT;
        goto exit;
    }


    if MEM_IS_NOT_ALIGN(aoaHwaCfg->res.cfarRngDopSnrList,
                        DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT)
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_CFAR_DET_LIST;
        goto exit;
    }

    if MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjOut,
                        DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT)
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_POINT_CLOUD_CARTESIAN;
        goto exit;
    }

    if MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjOutSideInfo,
                        DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT)
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_POINT_CLOUD_SIDE_INFO;
        goto exit;
    }

    if MEM_IS_NOT_ALIGN(aoaHwaCfg->res.azimuthStaticHeatMap,
                        DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT)
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_AZIMUTH_STATIC_HEAT_MAP;
        goto exit;
    }

    /* Check if radar cube range column fits into one HWA memory bank */
    if((aoaHwaCfg->staticCfg.numTxAntennas * aoaHwaCfg->staticCfg.numRxAntennas *
            aoaHwaCfg->staticCfg.numDopplerChirps * sizeof(cmplx16ImRe_t)) > (SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS))
    {
        retVal = DPU_AOAPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check if complex values of 2D FFT fit in two HWA memory banks */
    if((aoaHwaCfg->staticCfg.numTxAntennas * aoaHwaCfg->staticCfg.numRxAntennas *
            aoaHwaCfg->staticCfg.numDopplerBins * sizeof(cmplx16ImRe_t)) > (2 * SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS))
    {
        retVal = DPU_AOAPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check Heapmap configuration */
    if (aoaHwaCfg->dynCfg.prepareRangeAzimuthHeatMap)
    {
        if (aoaHwaCfg->res.azimuthStaticHeatMapSize !=
          (aoaHwaCfg->staticCfg.numRangeBins * aoaHwaCfg->staticCfg.numVirtualAntAzim))
        {
            retVal = DPU_AOAPROCHWA_ENOMEM__AZIMUTH_STATIC_HEAT_MAP;
            goto exit;
        }

        if (aoaHwaCfg->res.azimuthStaticHeatMap == NULL)
        {
            retVal = DPU_AOAPROCHWA_EINVAL;
            goto exit;
        }
    }

    if MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjElevationAngle,
                        DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT)
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_DET_OBJ_ELEVATION_ANGLE;
        goto exit;
    }

    if (aoaHwaCfg->dynCfg.prepareRangeAzimuthHeatMap && (aoaHwaCfg->staticCfg.numVirtualAntAzim == 1))
    {
        retVal = DPU_AOAPROCHWA_EINVALID_NUM_VIRT_ANT_AND_AZIMUTH_STATIC_HEAT_MAP;
        goto exit;
    }

#if DEBUG_CHECK_PARAMS
    /* Validate params */
    if(!aoaHwaCfg->edmaHandle ||
      !aoaHwaCfg->hwaHandle ||
      !aoaHwaCfg->params ||
      !aoaHwaCfg->radarCube ||
      !aoaHwaCfg->detObjOut ||
      (aoaHwaCfg->numParamSet != 1)
      )
    {
        retVal= DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }
#endif
    aoaHwaObj->res = aoaHwaCfg->res;
    aoaHwaObj->params = aoaHwaCfg->staticCfg;

    aoaHwaObj->dynLocalCfg.compRxChanCfg  = *aoaHwaCfg->dynCfg.compRxChanCfg;
    aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg = *aoaHwaCfg->dynCfg.multiObjBeamFormingCfg;
    aoaHwaObj->dynLocalCfg.prepareRangeAzimuthHeatMap  = aoaHwaCfg->dynCfg.prepareRangeAzimuthHeatMap;
    AoAProcHWA_ConvertFov(aoaHwaObj, aoaHwaCfg->dynCfg.fovAoaCfg);

    /* In interleave mode, only edmaOutCfgFmt is supported */
    retVal = HWA_getDMAChanIndex(aoaHwaObj->hwaHandle,
                                 aoaHwaCfg->res.edmaHwaOut.channel,
                                 &aoaHwaObj->dmaDestChannel);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Windowing configuraiton in HWA */
    retVal = HWA_configRam(aoaHwaObj->hwaHandle,
                           HWA_RAM_TYPE_WINDOW_RAM,
                           (uint8_t *)aoaHwaCfg->res.hwaCfg.window,
                           aoaHwaCfg->res.hwaCfg.windowSize, /* size in bytes */
                           aoaHwaCfg->res.hwaCfg.winRamOffset * sizeof(uint32_t));
    if (retVal != 0)
    {
        goto exit;
    }
exit:
    return retVal;
}

uint32_t DPU_AoAProcHWA_process
(
    DPU_AoAProcHWA_Handle    handle,
    uint32_t        numObjsIn,
    DPU_AoAProcHWA_OutParams  *outParams
)
{
    volatile uint32_t   startTime;
    volatile uint32_t   startTime1;
    volatile uint32_t   waitTimeLocal = 0;
    int32_t             retVal = 0;
    uint16_t            idx;
    float               range;
    uint32_t            numObjsOut = 0;
    uint16_t numParams;
    HWA_Handle hwaHandle;
    cmplx16ImRe_t     *radarCubeBase;

    AOAHwaObj *aoaHwaObj;
    DPU_AoAProcHWA_HW_Resources *res;
    DPU_AoAProcHWA_StaticConfig *DPParams;

    if (handle == NULL)
    {
        retVal = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }
    aoaHwaObj = (AOAHwaObj *)handle;
    res = &aoaHwaObj->res;
    DPParams = &aoaHwaObj->params;
    radarCubeBase = (cmplx16ImRe_t *)res->radarCube.data;
    hwaHandle = aoaHwaObj->hwaHandle;

    startTime = Cycleprofiler_getTimeStamp();

    if (numObjsIn == 0U)
    {
        outParams->numAoADetectedPoints = numObjsIn;
        outParams->stats.waitTime = 0;
        outParams->stats.processingTime = 0;
        retVal = 0;
        goto exit;
    }

    /**********************************************/
    /* ENABLE NUMLOOPS DONE INTERRUPT FROM HWA */
    /**********************************************/
    retVal = HWA_enableDoneInterrupt(hwaHandle,
                                     AOAProcHWADoneIsrCallback,
                                     aoaHwaObj->hwaDone_semaHandle);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Azimuth heap doppler compensation */
    if (aoaHwaObj->dynLocalCfg.prepareRangeAzimuthHeatMap)
    {
        /* Perform 2D FFT for all range bins and save zero Doppler bin */
        retVal = AoAProcHWA_PrepareRangeAzimuthHeatmap(aoaHwaObj,
                                                       &waitTimeLocal);
        if (retVal != 0)
        {
            goto exit;
        }
    }

    /* Angle estimation */
    if(DPParams->numVirtualAntAzim == 1)
    {
       /*If there is only one virtual antenna, there is no
         need of azimuth FFT as azimuth can not be estimated.*/

        /* Limit number of input samples */
        if (numObjsIn > res->detObjOutMaxSize)
        {
          numObjsIn = res->detObjOutMaxSize;
        }

        /* Fill the output list */
        for(idx=0; idx < numObjsIn; idx++)
        {
            range = res->cfarRngDopSnrList[idx].rangeIdx * DPParams->rangeStep;
            res->detObjOut[idx].y = range;
            res->detObjOut[idx].x = 0.;
            res->detObjOut[idx].z = 0.;

            res->detObjOut[idx].velocity = DPParams->dopplerStep *
                 DOPPLER_IDX_TO_SIGNED(res->cfarRngDopSnrList[idx].dopplerIdx,
                                       DPParams->numDopplerBins);
            res->detObjOutSideInfo[idx].snr = res->cfarRngDopSnrList[idx].snr;
            res->detObjOutSideInfo[idx].noise = res->cfarRngDopSnrList[idx].noise;
        }
        numObjsOut = numObjsIn;
    }
    else
    {
        uint32_t numObjsRemain;
        uint32_t objInStartIdx = 0;
        uint32_t numObjsPerChunk;
        uint32_t maxNumObjsPerChunk;

        uint32_t    dstAddr;

        uint32_t RangeBinSize;
        uint32_t dopplerBinSize = sizeof(uint32_t) * (DPParams->numVirtualAntAzim + DPParams->numVirtualAntElev);

        /* Limit number of input samples */
        if(DPParams->numVirtualAntElev > 0)
        {
            maxNumObjsPerChunk = DPU_AOAPROCHWA_MAX_OBJ_WITH_3_TX_ANT;
        }
        else
        {
            maxNumObjsPerChunk = DPU_AOAPROCHWA_MAX_OBJ_WITH_2_TX_ANT;
        }

        /* Limit number of input samples */
        if (numObjsIn > res->detObjOutMaxSize)
        {
          numObjsIn = res->detObjOutMaxSize;
        }

        numObjsRemain = numObjsIn;
        while (objInStartIdx < numObjsIn)
        {
            numObjsPerChunk = MIN(numObjsRemain, maxNumObjsPerChunk);

            /* Perform 2D FFT for detected Objects */
            retVal = HWAutil_configDopplerFFTSingleRangeBin(hwaHandle,
                    res->hwaCfg.paramSetStartIdx,
                    DPParams->numDopplerChirps,
                    DPParams->numDopplerBins,
                    DPParams->numDopplerBins,
                    DPParams->numRxAntennas,
                    DPParams->numTxAntennas,
                    DPParams->numTxAntennas,
                    res->hwaCfg.winRamOffset,
                    res->hwaCfg.winSym,
                    res->hwaCfg.paramSetStartIdx /* dmaTriggerSrc */,
                    aoaHwaObj->dmaDestChannel,
                    ADDR_TRANSLATE_CPU_TO_HWA(aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_INP_HWA_MEM_BANK]),
                    ADDR_TRANSLATE_CPU_TO_HWA(aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_OUT_HWA_MEM_BANK]));

            if (retVal != 0)
            {
                goto exit;
            }

            numParams = DPParams->numTxAntennas;
            retVal =  AoAProc_HWAutil_configCommon (hwaHandle,
                                                    numObjsPerChunk, //numLoops,
                                                    res->hwaCfg.paramSetStartIdx, //paramStartIdx,
                                                    numParams);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Configure input EDMA */
            retVal = AoAProcHWA_config2DSingleBinInp_EDMA(hwaHandle, res,
                                          DPParams,
                                          (uint32_t) radarCubeBase,
                                          (uint32_t)aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_INP_HWA_MEM_BANK],
                                          numObjsPerChunk,
                                          false);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Configure output EDMA */
            retVal = AoAProcHWA_config2DSingleBinOut_EDMA( res,
                                           DPParams,
                                           ((uint32_t)aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_OUT_HWA_MEM_BANK]),
                                           (uint32_t) aoaHwaObj->azimElevLocalBuf,
                                           numObjsPerChunk,
                                           false);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Enable the HWA */
            retVal = HWA_enable(hwaHandle, 1);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Virtual antenna buffer in HWA as input to angle FFT */
            dstAddr = (uint32_t)aoaHwaObj->hwaMemBankAddr[AOAHWA_ANGLE_INP_HWA_MEM_BANK];

            for(idx = objInStartIdx; idx < (objInStartIdx + numObjsPerChunk); idx++)
            {
                RangeBinSize = 1;
                retVal= AoAProcHWA_setSource2DSingleBinInp_EDMA(res,
                                                (uint32_t)(&radarCubeBase[RangeBinSize * res->cfarRngDopSnrList[idx].rangeIdx]));
                if (retVal != 0)
                {
                    goto exit;
                }

                retVal = AoAProcHWA_setSource2DSingleBinOut_EDMA(res,
                                                 ((uint32_t)(aoaHwaObj->hwaMemBankAddr[AOAHWA_2DFFT_SINGLE_BIN_OUT_HWA_MEM_BANK])) +
                                                 res->cfarRngDopSnrList[idx].dopplerIdx *  dopplerBinSize);
                if (retVal != 0)
                {
                    goto exit;
                }

                /* Trigger single 2D FFT */
                retVal = AoAProcHWA_dataPathTrigger2DSingleBin(res->edmaHandle,
                                                hwaHandle,
                                                res->edmaHwaIn.channel); //MMW_EDMA_2DFFT_SINGLERBIN_CH_ID

                if (retVal != 0)
                {
                    goto exit;
                }

                /* Wait until EDMA output is done */
                startTime1 = Cycleprofiler_getTimeStamp();
                retVal = AOAHWA_waitSinglePointFftEdmaOut(aoaHwaObj);
                waitTimeLocal += Cycleprofiler_getTimeStamp() - startTime1;
                if (retVal != 0)
                {
                      goto exit;
                }

                /* Rx channel gain/phase offset compensation */
                AoAProcHWA_rxChanPhaseBiasCompensation(&aoaHwaObj->dynLocalCfg.compRxChanCfg.rxChPhaseComp[0],
                                                    1,
                                                    DPParams->numVirtualAntAzim + DPParams->numVirtualAntElev,
                                                    (cmplx16ImRe_t *) aoaHwaObj->azimElevLocalBuf,
                                                    (cmplx16ImRe_t *) aoaHwaObj->azimElevLocalBuf);

                /* Doppler Compensation */
                aoaHwa_dopplerCompensation(1,
                                           (uint32_t *) aoaHwaObj->azimElevLocalBuf,
                                           &res->cfarRngDopSnrList[idx],
                                           (uint32_t *) dstAddr,
                                           DPParams->numTxAntennas,
                                           DPParams->numRxAntennas,
                                           DPParams->numVirtualAntAzim,
                                           DPParams->numVirtualAntElev,
                                           DPParams->numDopplerBins);

                dstAddr += (DPParams->numVirtualAntAzim + DPParams->numVirtualAntElev) * sizeof(uint32_t);
            } /* numObjsPerChunk loop */

            /* Wait until HWA done with FFT. This code is redundant because HWA will be done
             * by this time (output EDMA triggers on each iteration in above loop
             * only after HWA is done for that iteration) */
            startTime1 = Cycleprofiler_getTimeStamp();
            retVal = AoAProcHWA_dataPathWait2DFFTDetObj(aoaHwaObj->hwaDone_semaHandle);
            waitTimeLocal += Cycleprofiler_getTimeStamp() - startTime1;

            /* Angle estimation - Azimuth FFTs */
            retVal = AoAProcHWA_dataPathConfigHwaAoa(hwaHandle, res, DPParams,
                                                     numObjsPerChunk, &aoaHwaObj->hwaMemBankAddr[0]);
            if (retVal < 0)
            {
                goto exit;
            }

#ifdef SUBSYS_MSS
            /* Ensure R4F writes to HWA memory is flushed out */
            MEM_BARRIER();
#endif

            /* Enable the HWA */
            retVal = HWA_enable(hwaHandle,1);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Wait for HWA finish */
            startTime1 = Cycleprofiler_getTimeStamp();
            AoAProcHWA_dataPathWait2DFFTDetObj(aoaHwaObj->hwaDone_semaHandle);
            waitTimeLocal += Cycleprofiler_getTimeStamp() - startTime1;

            /* Disable the HWA */
            retVal = HWA_enable(hwaHandle, 0);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Azimuth FFT done!*/
            numObjsOut = AoAProcHWA_angleEstimationAzimElev(objInStartIdx,
                                                            numObjsPerChunk,
                                    (uint16_t *)aoaHwaObj->hwaMemBankAddr[AOAHWA_ANGLE_AZIM_ABS_OUT_HWA_MEM_BANK],
                                    (uint16_t *)aoaHwaObj->hwaMemBankAddr[AOAHWA_ANGLE_AZIM_CPLX_OUT_HWA_MEM_BANK],
                                    (uint16_t *)aoaHwaObj->hwaMemBankAddr[AOAHWA_ANGLE_ELEV_CPLX_OUT_HWA_MEM_BANK],
                                    aoaHwaObj,
                                    numObjsOut);

            objInStartIdx += numObjsPerChunk;
            numObjsRemain -= numObjsPerChunk;
        }//End of while loop (processing per chunk)
    }

    outParams->numAoADetectedPoints = numObjsOut;
    outParams->stats.waitTime = waitTimeLocal;
    outParams->stats.processingTime = Cycleprofiler_getTimeStamp() - startTime - waitTimeLocal;
    /* Process is done, disable Done interrupt */
    HWA_disableDoneInterrupt(hwaHandle);
exit:
    return retVal;
}

int32_t DPU_AoAProcHWA_control
(
   DPU_AoAProcHWA_Handle handle,
   DPU_AoAProcHWA_Cmd cmd,
   void *arg,
   uint32_t argSize
)
{
   int32_t retVal = 0;
   AOAHwaObj *aoaHwaObj = (AOAHwaObj *)handle;

   /* Get rangeProc data object */
   if (aoaHwaObj == NULL)
   {
       retVal = DPU_AOAPROCHWA_EINVAL;
       goto exit;
   }

   switch(cmd)
   {
       case DPU_AoAProcHWA_Cmd_FovAoACfg:
       {
           if((argSize != sizeof(DPU_AoAProc_FovAoaCfg)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
               /* Save configuration */
               AoAProcHWA_ConvertFov(aoaHwaObj, (DPU_AoAProc_FovAoaCfg *) arg);

           }
       }
       break;
       case DPU_AoAProcHWA_Cmd_MultiObjBeamFormingCfg:
       {
           if((argSize != sizeof(DPU_AoAProc_MultiObjBeamFormingCfg)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
               /* Save configuration */
               memcpy((void *)&aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg, arg, argSize);
           }
       }
       break;
       case DPU_AoAProcHWA_Cmd_CompRxChannelBiasCfg:
       {
           if((argSize != sizeof(DPU_AoAProc_compRxChannelBiasCfg)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
               /* Save configuration */
               memcpy((void *)&aoaHwaObj->dynLocalCfg.compRxChanCfg, arg, argSize);
           }
       }
       break;
       case DPU_AoAProcHWA_Cmd_PrepareRangeAzimuthHeatMap:
       {
           if((argSize != sizeof(bool)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
                /* Sanity check on saved heapmap ptr and size */
                if ((aoaHwaObj->res.azimuthStaticHeatMapSize !=
                    (aoaHwaObj->params.numRangeBins * aoaHwaObj->params.numVirtualAntAzim)) ||
                    (aoaHwaObj->res.azimuthStaticHeatMap == NULL))
                {
                   retVal = DPU_AOAPROCHWA_EINVAL;
                   goto exit;
                }

                /* Save configuration */
                memcpy((void *)&aoaHwaObj->dynLocalCfg.prepareRangeAzimuthHeatMap, arg, argSize);
           }
       }
       break;
       default:
           retVal = DPU_AOAPROCHWA_EINVAL;
           break;
   }
exit:
   return (retVal);
}

int32_t DPU_AoAProcHWA_deinit(DPU_AoAProcHWA_Handle handle)
{
    int32_t retVal = 0;

    if (handle == NULL)
    {
        retVal = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }
    MemoryP_ctrlFree(handle, sizeof(AOAHwaObj));
exit:
    return (retVal);
}

