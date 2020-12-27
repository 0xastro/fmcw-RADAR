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
 *   @file  rangeprocdsp.c
 *
 *   @brief
 *      Implements Data path range FFT processing functionality on DSP.
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

/* mmwave SDK include files */
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>

#include <ti/alg/mmwavelib/mmwavelib.h>

/* C64P dsplib (fixed point part for C674X) */
#include "DSP_fft16x16_imre.h"
#include "gen_twiddle_fft16x16_imre.h"

/* Internal include Files */
#include <ti/datapath/dpu/rangeproc/include/rangeprocdsp_internal.h>

/* MATH utils library Include files */
#include <ti/utils/mathutils/mathutils.h>

#define  DEBUG_CHECK_PARAMS 1

/* Macros to determine pingpong index */
#define pingPongId(x) ((x) & 0x1U)
#define isPong(x) (pingPongId(x) == 1U)
#define BYTES_PER_SAMP_1D   sizeof(cmplx16ImRe_t)

/******************************************************************
 *                      Internal Function prototype
 ******************************************************************/
static void rangeProcDSP_WaitEDMAComplete
(
    EDMA_Handle         edmaHandle,
    uint8_t             chId
);

static int32_t rangeProcDSP_ConfigDataInEDMA
(
    rangeProcDSPObj          *rangeProcObj,
    DPU_RangeProcDSP_HW_Resources  *hwRes
);

static int32_t rangeProcDSP_ConfigDataOutEDMA
(
    rangeProcDSPObj          *rangeProcObj,
    DPU_RangeProcDSP_HW_Resources  *hwRes
);

static int32_t rangeProcDSP_dcRangeSignatureCompensation_init
(
    rangeProcDSPObj     *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcRangeSigCfg,
    uint8_t             resetBuffer
);

static void rangeProcDSP_dcRangeSignatureCompensation
(
    rangeProcDSPObj         *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg  *calibDcCfg,
    uint8_t                 chirpPingPongId,
    uint8_t                 txIdx
);

/**
 *  @b Description
 *  @n
 *      Polling function to wait for EDMA transper complete.
 *
 *  @param[in]  edmaHandle              EDMA Handle
 *  @param[in]  chId                    EDMA channel id
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 * 
 */
static void rangeProcDSP_WaitEDMAComplete
(
    EDMA_Handle         edmaHandle,
    uint8_t             chId
)
{
    volatile bool isTransferDone = false;
    do {
        if (EDMA_isTransferComplete(edmaHandle,
                                    (uint8_t) chId,
                                    (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
        }
    } while (isTransferDone == false);
}

/**
 *  @b Description
 *  @n
 *      Helper function to configuration data in EDMA
 *
 *  @param[in]  rangeProcObj             Pointer to rangeProc object
 *  @param[in]  hwRes                    Pointer to hard resource configuration
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcDSP_ConfigDataInEDMA
(
    rangeProcDSPObj          *rangeProcObj,
    DPU_RangeProcDSP_HW_Resources  *hwRes
)
{
    int32_t retVal;
    rangeProc_dpParams   *dpParams;
    DPEDMA_syncACfg         syncACfg;

    dpParams = &rangeProcObj->DPParams;

    /* Copy data from ADCbuffer to internal adcbufIn scratch buffer 
      Assumes the input data is in non-interleaved mode
     */
    syncACfg.aCount = dpParams->numAdcSamples * BYTES_PER_SAMP_1D;
    syncACfg.bCount =MAX(dpParams->numRxAntennas / 2U, 1U) * dpParams->numChirpsPerChirpEvent;
    syncACfg.srcBIdx = rangeProcObj->rxChanOffset * 2U ; 
    syncACfg.dstBIdx = 0U;

    /* PING configuration */
    syncACfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf;
    syncACfg.destAddress = (uint32_t)rangeProcObj->adcDataIn;
    
    retVal = DPEDMA_configSyncA_singleFrame(hwRes->edmaCfg.edmaHandle,
                         &hwRes->edmaCfg.dataInPing,
                         NULL,  /* no Chaining */
                         &syncACfg,
                         false,
                         true,
                         true,
                         NULL,
                         NULL);
    if (retVal < 0)
    {
        goto exit;
    }

    /* PONG src/dest address */
    syncACfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf + rangeProcObj->rxChanOffset;
    syncACfg.destAddress = (uint32_t)&rangeProcObj->adcDataIn[dpParams->numRangeBins];

    retVal = DPEDMA_configSyncA_singleFrame(hwRes->edmaCfg.edmaHandle,
                         &hwRes->edmaCfg.dataInPong,
                         NULL,  /* no Chaining */
                         &syncACfg,
                         false,
                         true,
                         true,
                         NULL,
                         NULL);
    if (retVal < 0)
    {
        goto exit;
    }
exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Helper function to configuration data out EDMA
 *
 *  @param[in]  rangeProcObj             Pointer to rangeProc object
 *  @param[in]  hwRes                    Pointer to hard resource configuration
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcDSP_ConfigDataOutEDMA
(
    rangeProcDSPObj          *rangeProcObj,
    DPU_RangeProcDSP_HW_Resources  *hwRes
)
{
    int32_t     retVal;
    uint16_t    samplesPerChirp, aCount;
    uint16_t     oneD_destinationCindex;
    uint8_t     *oneD_destinationPongAddress;
    DPEDMA_syncABCfg         syncABCfg;
    rangeProc_dpParams      *dpParams;
    DPU_RangeProcDSP_EDMAConfig *edmaCfg;

    edmaCfg = &hwRes->edmaCfg;
    dpParams = &rangeProcObj->DPParams;

    /*****************************************************
     * EDMA configuration for storing 1d fft output to L3.
     * It copies all Rx antennas of the chirp per trigger event.
     *****************************************************/
    samplesPerChirp = dpParams->numRangeBins * dpParams->numRxAntennas;
    aCount = samplesPerChirp * sizeof(cmplx16ImRe_t);

    if ((dpParams->numTxAntennas == 2U) || (dpParams->numTxAntennas == 3U))
    {
        oneD_destinationCindex = (int16_t)aCount;

        /* Calculation address for 2 TxAnt. 3 txAnt , the address is calculated at runtime */
        oneD_destinationPongAddress = (uint8_t *)(&rangeProcObj->radarCubebuf[samplesPerChirp * dpParams->numDopplerChirps]);
    }
    else if (dpParams->numTxAntennas == 1U)
    {
        oneD_destinationCindex = (int16_t)(aCount * 2U);
        oneD_destinationPongAddress = (uint8_t *)(&rangeProcObj->radarCubebuf[samplesPerChirp]);
    }
    else
    {
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }

    /* Ping/Pong common configuration */
    syncABCfg.aCount = dpParams->numRangeBins * sizeof(cmplx16ImRe_t);
    syncABCfg.bCount = dpParams->numRxAntennas;
    syncABCfg.cCount = dpParams->numChirpsPerFrame / 2U; /*bCount */
    syncABCfg.srcBIdx = dpParams->numRangeBins * sizeof(cmplx16ImRe_t);
    syncABCfg.srcCIdx = 0;
    syncABCfg.dstBIdx = dpParams->numRangeBins * sizeof(cmplx16ImRe_t);
    syncABCfg.dstCIdx = oneD_destinationCindex;

    syncABCfg.srcAddress = (uint32_t)rangeProcObj->fftOut1D;
    syncABCfg.destAddress= (uint32_t)rangeProcObj->radarCubebuf;

    /* Ping - Copies from ping FFT output (even chirp indices)  to L3 */
    retVal = DPEDMA_configSyncAB (edmaCfg->edmaHandle,
                                 &edmaCfg->dataOutPing,
                                 NULL,
                                 &syncABCfg,
                                 false,
                                 true,
                                 true,
                                 NULL,
                                 NULL
                                 );
    if (retVal < 0)
    {
        goto exit;
    }

    /* Pong - copies from pong FFT output (odd chirp indices)  to L3 */
    syncABCfg.srcAddress = (uint32_t)&rangeProcObj->fftOut1D[samplesPerChirp];
    syncABCfg.destAddress= (uint32_t)oneD_destinationPongAddress;

    retVal = DPEDMA_configSyncAB (edmaCfg->edmaHandle,
                                 &edmaCfg->dataOutPong,
                                 NULL,
                                 &syncABCfg,
                                 false,
                                 true,
                                 true,
                                 NULL,
                                 NULL
                                 );

exit:
    return(retVal);
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
static int32_t rangeProcDSP_dcRangeSignatureCompensation_init
(
    rangeProcDSPObj     *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcRangeSigCfg,
    uint8_t             resetMeanBuffer
)
{
    int32_t                 retVal = 0;
    uint32_t                meanbufSize;

    meanbufSize = DPU_RANGEPROC_SIGNATURE_COMP_MAX_BIN_SIZE * rangeProcObj->DPParams.numVirtualAntennas
                 * sizeof(cmplx32ImRe_t);

    /* Validate DC revomal configuraiton */
    if(calibDcRangeSigCfg->enabled)
    {
        if( (rangeProcObj->dcRangeSigMean == (cmplx32ImRe_t*)NULL) ||
            (rangeProcObj->dcRangeSigMeanBufSize < meanbufSize))
        {
            /* Check DC range average buffer pointer */
            retVal = DPU_RANGEPROCDSP_EINVAL;
            goto exit;
        }
        else
        {
            if(resetMeanBuffer == 1U)
            {
                /* Initialize memory */
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
 *    Compensation of DC range antenna signature
 *
 *
 *  @retval
 *      Not Applicable.
 */
/**
 *  @b Description
 *  @n
 *      Compensation of DC range antenna signature
 *
 *  @param[in]  rangeProcObj                 Pointer to rangeProc object
 *  @param[in]  calibDcCfg                   Pointer DC range compensation configuration
 *  @param[in]  chirpPingPongId              Ping/Pong id to calculate offset
 *  @param[in]  txIdx                        Tx index
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static void rangeProcDSP_dcRangeSignatureCompensation
(
    rangeProcDSPObj         *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcCfg,
    uint8_t                 chirpPingPongId,
    uint8_t                 txIdx
)
{
    rangeProc_dpParams      *params;
    uint32_t rxAntIdx, binIdx;
    uint32_t ind;
    int32_t chirpPingPongOffs;
    int32_t chirpPingPongSize;

    params = &rangeProcObj->DPParams;
    chirpPingPongSize = params->numRxAntennas * (calibDcCfg->positiveBinIdx - calibDcCfg->negativeBinIdx + 1);

    chirpPingPongOffs = txIdx * chirpPingPongSize;

    /* Calibration */
    if (rangeProcObj->dcRangeSigCalibCntr < (calibDcCfg->numAvgChirps * params->numTxAntennas))
    {
        /* Accumulate */
        ind = 0;
        for (rxAntIdx = 0; rxAntIdx < params->numRxAntennas; rxAntIdx++)
        {
            uint32_t chirpInOffs = chirpPingPongId * (params->numRxAntennas * params->numRangeBins) +
                                  (params->numRangeBins * rxAntIdx);
            int64_t *meanPtr = (int64_t *) &rangeProcObj->dcRangeSigMean[chirpPingPongOffs];
            uint32_t *fftPtr = (uint32_t *) &rangeProcObj->fftOut1D[chirpInOffs];
            int64_t meanBin;
            uint32_t fftBin;
            int32_t Re, Im;
            for (binIdx = 0; binIdx <= calibDcCfg->positiveBinIdx; binIdx++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                fftBin = _amem4(&fftPtr[binIdx]);
                Im = _loll(meanBin) + _ext(fftBin, 0, 16);
                Re = _hill(meanBin) + _ext(fftBin, 16, 16);
                _amem8(&meanPtr[ind]) = _itoll(Re, Im);
                ind++;
            }

            chirpInOffs = chirpPingPongId * (params->numRxAntennas * params->numRangeBins) +
                (params->numRangeBins * rxAntIdx) + params->numRangeBins + calibDcCfg->negativeBinIdx;
            fftPtr =  (uint32_t *) &rangeProcObj->fftOut1D[chirpInOffs];
            for (binIdx = 0; binIdx < -calibDcCfg->negativeBinIdx; binIdx++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                fftBin = _amem4(&fftPtr[binIdx]);
                Im = _loll(meanBin) + _ext(fftBin, 0, 16);
                Re = _hill(meanBin) + _ext(fftBin, 16, 16);
                _amem8(&meanPtr[ind]) = _itoll(Re, Im);
                ind++;
            }
        }

        if (rangeProcObj->dcRangeSigCalibCntr == (calibDcCfg->numAvgChirps * params->numTxAntennas - 1))
        {
            /* Divide */
            int64_t *meanPtr = (int64_t *) rangeProcObj->dcRangeSigMean;
            int32_t Re, Im;
            int64_t meanBin;
            int32_t divShift = mathUtils_floorLog2(calibDcCfg->numAvgChirps);
            for (ind  = 0; ind < (params->numTxAntennas * chirpPingPongSize); ind++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                Im = _sshvr(_loll(meanBin), divShift);
                Re = _sshvr(_hill(meanBin), divShift);
                _amem8(&meanPtr[ind]) = _itoll(Re, Im);
            }
        }
    }
    else
    {
       /* fftOut1D -= dcRangeSigMean */
        ind = 0;
        for (rxAntIdx  = 0; rxAntIdx < params->numRxAntennas; rxAntIdx++)
        {
            uint32_t chirpInOffs = chirpPingPongId * (params->numRxAntennas * params->numRangeBins) +
                                   (params->numRangeBins * rxAntIdx);
            int64_t *meanPtr = (int64_t *) &rangeProcObj->dcRangeSigMean[chirpPingPongOffs];
            uint32_t *fftPtr =  (uint32_t *) &rangeProcObj->fftOut1D[chirpInOffs];
            int64_t meanBin;
            uint32_t fftBin;
            int32_t Re, Im;
            for (binIdx = 0; binIdx <= calibDcCfg->positiveBinIdx; binIdx++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                fftBin = _amem4(&fftPtr[binIdx]);
                Im = _ext(fftBin, 0, 16) - _loll(meanBin);
                Re = _ext(fftBin, 16, 16) - _hill(meanBin);
                _amem4(&fftPtr[binIdx]) = _pack2(Im, Re);
                ind++;
            }

            chirpInOffs = chirpPingPongId * (params->numRxAntennas * params->numRangeBins) +
                (params->numRangeBins * rxAntIdx) + params->numRangeBins + calibDcCfg->negativeBinIdx;
            fftPtr =  (uint32_t *) &rangeProcObj->fftOut1D[chirpInOffs];
            for (binIdx = 0; binIdx < -calibDcCfg->negativeBinIdx; binIdx++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                fftBin = _amem4(&fftPtr[binIdx]);
                Im = _ext(fftBin, 0, 16) - _loll(meanBin);
                Re = _ext(fftBin, 16, 16) - _hill(meanBin);
                _amem4(&fftPtr[binIdx]) = _pack2(Im, Re);
                ind++;
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
 *  @param[in]  pConfigIn                 Pointer to rangeProcDSP configuration structure
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcDSP_ParseConfig
(
    rangeProcDSPObj          *rangeProcObj,
    DPU_RangeProcDSP_Config  *pConfigIn
)
{
    int32_t                 retVal = 0;
    rangeProc_dpParams  *params;
    DPU_RangeProcDSP_StaticConfig      *pStaticCfg;
    DPU_RangeProcDSP_HW_Resources      *pHwRes;

    /* Get configuration pointers */
    pStaticCfg = &pConfigIn->staticCfg;
    pHwRes = &pConfigIn->hwRes;
    params    = &rangeProcObj->DPParams;

    /* Save datapath parameters */
    params->numTxAntennas = pStaticCfg->numTxAntennas;
    params->numRxAntennas = pStaticCfg->ADCBufData.dataProperty.numRxAntennas;
    params->numVirtualAntennas = pStaticCfg->numVirtualAntennas;
    params->numChirpsPerChirpEvent = pStaticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent;
    params->numAdcSamples = pStaticCfg->ADCBufData.dataProperty.numAdcSamples;
    params->numRangeBins = pStaticCfg->numRangeBins;
    params->numChirpsPerFrame = pStaticCfg->numChirpsPerFrame;
    params->numDopplerChirps = pStaticCfg->numChirpsPerFrame/pStaticCfg->numTxAntennas;

    /* Save EDMA Handle */
    rangeProcObj->edmaHandle = pHwRes->edmaCfg.edmaHandle;

    /* Save interface buffers */
    rangeProcObj->ADCdataBuf        = (cmplx16ImRe_t *)pStaticCfg->ADCBufData.data;
    rangeProcObj->radarCubebuf      = (cmplx16ImRe_t *)pHwRes->radarCube.data;

    /* Save Scratch buffers */
    rangeProcObj->fftOut1D          = pHwRes->fftOut1D;
    rangeProcObj->adcDataIn         = pHwRes->adcDataIn;
    rangeProcObj->dcRangeSigMean    = pHwRes->dcRangeSigMean;
    rangeProcObj->dcRangeSigMeanBufSize    = pHwRes->dcRangeSigMeanSize;

    /* Scratch windowing & twiddle buffers */
    rangeProcObj->twiddle16x16      = pHwRes->twiddle16x16;
    rangeProcObj->window            = pHwRes->window;

    if(params->numRxAntennas > 1)
    {
        /* For rangeProc DPU needs rx channel has same offset from one channel to the next channel
           Use first two channel offset to calculate the BIdx for EDMA
         */
        rangeProcObj->rxChanOffset = pStaticCfg->ADCBufData.dataProperty.rxChanOffset[1] -
                                     pStaticCfg->ADCBufData.dataProperty.rxChanOffset[0];

        /* rxChanOffset should be 16 bytes aligned and should be big enough to hold numAdcSamples */
        if (rangeProcObj->rxChanOffset < rangeProcObj->DPParams.numAdcSamples * sizeof(cmplx16ImRe_t) ||
          (rangeProcObj->rxChanOffset & 0xF != 0))
        {
            retVal = DPU_RANGEPROCDSP_EINVAL;
            goto exit;
        }
    }

    /* Prepare Ping/Pong EDMA data in/out channels */
    rangeProcObj->dataInChan[0] = pHwRes->edmaCfg.dataInPing.channel;
    rangeProcObj->dataInChan[1] = pHwRes->edmaCfg.dataInPong.channel;
    rangeProcObj->dataOutChan[0] = pHwRes->edmaCfg.dataOutPing.channel;
    rangeProcObj->dataOutChan[1] = pHwRes->edmaCfg.dataOutPong.channel;

    /* Calculation used at runtime */
    rangeProcObj->numSamplePerChirp = params->numRangeBins * params->numRxAntennas ;
    rangeProcObj->numSamplePerTx = params->numDopplerChirps *rangeProcObj->numSamplePerChirp;

exit:
    return(retVal);
}



/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU init function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]  errCode                 Pointer to errCode generates from the API
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid rangeProc handle
 *  @retval
 *      Error       - NULL
 */
DPU_RangeProcDSP_Handle DPU_RangeProcDSP_init
(
    int32_t*    errCode
)
{
    rangeProcDSPObj *rangeProcObj;

    rangeProcObj = MemoryP_ctrlAlloc(sizeof(rangeProcDSPObj), 0);
    if(rangeProcObj == NULL)
    {
        *errCode = DPU_RANGEPROCDSP_ENOMEM;
        return NULL;
    }

    /* Initialize memory */
    memset((void *)rangeProcObj, 0, sizeof(rangeProcDSPObj));

    return ((DPU_RangeProcDSP_Handle)rangeProcObj);
}

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU config function. It saves buffer pointer and configurations 
 *  including system resources and configures EDMA for runtime range processing.
 *  
 *  @pre    DPU_RangeProcDSP_init() has been called
 *
 *  @param[in]  handle                  rangeProc DPU handle
 *  @param[in]  pConfig                 Pointer to rangeProc configuration data structure
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcDSP_config
(
    DPU_RangeProcDSP_Handle     handle,
    DPU_RangeProcDSP_Config*    pConfig
)
{
    rangeProcDSPObj          *rangeProcObj;
    DPU_RangeProcDSP_StaticConfig *pStaticCfg;
    DPU_RangeProcDSP_HW_Resources      *pHwRes;
    int32_t                  retVal = 0;

    rangeProcObj = (rangeProcDSPObj *)handle;
    if(rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }

    pStaticCfg = &pConfig->staticCfg;
    pHwRes = &pConfig->hwRes;

#if DEBUG_CHECK_PARAMS
    /* Validate params */
    if(!pConfig->hwRes.edmaCfg.edmaHandle ||
      !pHwRes->radarCube.data ||
      !pStaticCfg->ADCBufData.data||
      !pHwRes->adcDataIn ||
      !pHwRes->fftOut1D ||
      !pHwRes->window ||
      !pHwRes->twiddle16x16 ||
      !pHwRes->dcRangeSigMean
      )
    {
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }

    if(rangeProcObj->inProgress == true)
    {
        retVal = DPU_RANGEPROCDSP_EINPROGRESS;
        goto exit;
    }

    /* Parameter check: validate Adc data interface configuration
        Support:
            - Complex 16bit ADC data in IMRE format
            - Non-interleaved mode
     */
    if( (pStaticCfg->ADCBufData.dataProperty.dataFmt != DPIF_DATAFORMAT_COMPLEX16_IMRE) ||
       (pStaticCfg->ADCBufData.dataProperty.interleave != DPIF_RXCHAN_NON_INTERLEAVE_MODE) )
    {
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }

    /* Validate dp radarCube interface */
    if (pConfig->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_1)
    {
        /* Only one format is supported */
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }

    /* Validate windowing configuraiton */
    if(pHwRes->windowSize != (pStaticCfg->ADCBufData.dataProperty.numAdcSamples *sizeof(int16_t) / 2U))
    {
        /* Only one format is supported */
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }
#endif
    /* Save hardware resources */
    memcpy((void *)&rangeProcObj->calibDcRangeSigCfg, (void *)pConfig->dynCfg.calibDcRangeSigCfg, sizeof(DPU_RangeProc_CalibDcRangeSigCfg));

    retVal = rangeProcDSP_ParseConfig(rangeProcObj, pConfig);
    if(retVal < 0)
    {
        goto exit;
    }

    /* Generate twiddle factors for 1D FFT. This is one time */
    mmwavelib_gen_twiddle_fft16x16_imre_sa((short *)rangeProcObj->twiddle16x16, pStaticCfg->numRangeBins);

    /* Configure EDMA */
    retVal = rangeProcDSP_ConfigDataInEDMA(rangeProcObj, &pConfig->hwRes);
    if(retVal < 0)
    {
        goto exit;
    }
    retVal = rangeProcDSP_ConfigDataOutEDMA(rangeProcObj, &pConfig->hwRes);
    if(retVal < 0)
    {
        goto exit;
    }

    /* DC calibration and compensation init */
    retVal = rangeProcDSP_dcRangeSignatureCompensation_init(rangeProcObj,
                                                             pConfig->dynCfg.calibDcRangeSigCfg,
                                                             pStaticCfg->resetDcRangeSigMeanBuffer);
    if (retVal < 0)
    {
        goto exit;
    }

    rangeProcObj->chirpCount = 0;
    rangeProcObj->inProgress = false;
exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU process function. It executes FFT operation. 
 *  It can be called multiple times in a frame until all chirps are handled in the frame.
 *
 *  @pre    DPU_RangeProcDSP_init() has been called
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
int32_t DPU_RangeProcDSP_process
(
    DPU_RangeProcDSP_Handle     handle,
    DPU_RangeProc_OutParams     *outParams
)
{
    rangeProc_dpParams  *DPParams;
    rangeProcDSPObj     *rangeProcObj;
    EDMA_Handle         edmaHandle;
    uint32_t            rxChanId;
    volatile uint32_t   startTime;
    volatile uint32_t   startTime1;
    uint32_t            waitingTime;
    uint32_t            outChannel;
    uint16_t            chirpIndex;
    uint8_t             chirpPingPongId;
    int32_t             retVal = 0;
    uint32_t            totalChirpIndex;

    rangeProcObj = (rangeProcDSPObj *)handle;
    if(rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }

    if(rangeProcObj->inProgress == true)
    {
        retVal = DPU_RANGEPROCDSP_EINPROGRESS;
        goto exit;
    }
    else
    {
        rangeProcObj->inProgress = true;
    }

    DPParams = &rangeProcObj->DPParams;
    edmaHandle = rangeProcObj->edmaHandle;

    startTime = Cycleprofiler_getTimeStamp();
    waitingTime = 0;

    outParams->endOfChirp = false;

    /* 
     Process chirp data per loop, loops for numChirpsPerChirpEvent
     */
    for (chirpIndex = 0; chirpIndex < DPParams->numChirpsPerChirpEvent; chirpIndex++)
    {
        uint32_t    dataInAddr[2];
        uint32_t    numAdcSampleAligned;
        cmplx16ImRe_t *adcBufPongOffset;

        /*********************************
         * Prepare for the FFT
         *********************************/
        numAdcSampleAligned = (DPParams->numAdcSamples + 3U)/4U * 4U;
        adcBufPongOffset = (cmplx16ImRe_t *)((uint32_t)rangeProcObj->ADCdataBuf + rangeProcObj->rxChanOffset);

        dataInAddr[0] = (uint32_t)&rangeProcObj->ADCdataBuf[chirpIndex * numAdcSampleAligned];
        dataInAddr[1] = (uint32_t)&adcBufPongOffset[chirpIndex * numAdcSampleAligned];

        /* Set Ping source Address */
        EDMA_setSourceAddress(edmaHandle,  rangeProcObj->dataInChan[0],
            (uint32_t) SOC_translateAddress(dataInAddr[0], SOC_TranslateAddr_Dir_TO_EDMA, NULL));

        /* Set Pong source Address */
        EDMA_setSourceAddress(edmaHandle,  rangeProcObj->dataInChan[1],
            (uint32_t) SOC_translateAddress(dataInAddr[1], SOC_TranslateAddr_Dir_TO_EDMA, NULL));

        /* Kick off DMA to fetch data from ADC buffer for first channel */
        EDMA_startDmaTransfer(edmaHandle, rangeProcObj->dataInChan[0]);

        chirpPingPongId = pingPongId(rangeProcObj->chirpCount);
        /* 1d fft for first antenna, followed by kicking off the DMA of fft output */
        for (rxChanId = 0; rxChanId < DPParams->numRxAntennas; rxChanId++)
        {
            int16_t     *fftSrcAddr;
            int16_t     *fftDestAddr;
            uint8_t     inChannel;

            /*********************************
             * Data Input
             *********************************/
            inChannel = rangeProcObj->dataInChan[pingPongId(rxChanId)];

             /* Get the src/dest Address for FFT operation */
            fftSrcAddr = (int16_t*)&rangeProcObj->adcDataIn[pingPongId(rxChanId) * DPParams->numRangeBins];
            fftDestAddr = (int16_t*)&rangeProcObj->fftOut1D[chirpPingPongId * rangeProcObj->numSamplePerChirp +
                                                       (DPParams->numRangeBins * rxChanId)];

            if(rxChanId<DPParams->numRxAntennas -1U)
            {
                /* Kick off DMA to fetch data from ADC buffer for the next channel */
                EDMA_startDmaTransfer(edmaHandle, rangeProcObj->dataInChan[pingPongId(rxChanId + 1)]);
            }

            /* Verify if DMA has completed for current antenna */
            startTime1 = Cycleprofiler_getTimeStamp();
            rangeProcDSP_WaitEDMAComplete (  edmaHandle, inChannel);
            waitingTime += (Cycleprofiler_getTimeStamp() - startTime1);

            /*********************************
             * Data Processing
             *********************************/

            /* Only support even length of windowing */
            mmwavelib_windowing16x16_evenlen(
                    (int16_t *) fftSrcAddr,
                    (int16_t *) rangeProcObj->window,
                    DPParams->numAdcSamples);

            /* Zero out padding region */
            memset((void *)&rangeProcObj->adcDataIn[pingPongId(rxChanId) * DPParams->numRangeBins + DPParams->numAdcSamples],
                0 , (DPParams->numRangeBins - DPParams->numAdcSamples) * sizeof(cmplx16ImRe_t));

            /* 16bit FFT in imre format */
            DSP_fft16x16_imre(
                    (int16_t *) rangeProcObj->twiddle16x16,
                    DPParams->numRangeBins,
                    (int16_t *)fftSrcAddr,
                    (int16_t *) fftDestAddr);

        }

        /*********************************
         * DC removal processing
         *********************************/

        if(rangeProcObj->calibDcRangeSigCfg.enabled)
        {
            int32_t chirpPingPongSize;

            chirpPingPongSize = DPParams->numRxAntennas * (rangeProcObj->calibDcRangeSigCfg.positiveBinIdx -
                                                      rangeProcObj->calibDcRangeSigCfg.negativeBinIdx + 1);

            if (rangeProcObj->dcRangeSigCalibCntr == 0)
            {
                memset(rangeProcObj->dcRangeSigMean, 0, DPParams->numTxAntennas * chirpPingPongSize * sizeof(cmplx32ImRe_t));
            }

            rangeProcDSP_dcRangeSignatureCompensation(rangeProcObj, 
                                                         &rangeProcObj->calibDcRangeSigCfg,
                                                         chirpPingPongId,
                                                         rangeProcObj->chirpCount%DPParams->numTxAntennas);

            rangeProcObj->dcRangeSigCalibCntr++;
        }

        /*********************************
         * Data Output
         *********************************/
        outChannel = rangeProcObj->dataOutChan[chirpPingPongId];

        /* For non TDM case, when chirpBytes is >= 16384, the destinationBindex in
           EDMA will be twice of this which is negative jump, so need to set the
           destination address in this situation.
           e.g if numRangeBins = 1024, numRxAntennas = 4 then destinationBindex becomes -32768 */
        if ((DPParams->numTxAntennas == 1U) && (rangeProcObj->numSamplePerChirp * sizeof(cmplx16ImRe_t) >= (uint32_t)16384U) )
        {
            uint32_t    radarCubeAddr;

            radarCubeAddr = (uint32_t)(rangeProcObj->radarCubebuf + rangeProcObj->chirpCount * rangeProcObj->numSamplePerChirp);
            EDMA_setDestinationAddress(edmaHandle, outChannel,
                (uint32_t)SOC_translateAddress((radarCubeAddr), SOC_TranslateAddr_Dir_TO_EDMA, NULL));
        }
        /* Limitation of EDMA index settings, manually set the destination Address */
        else if ((DPParams->numTxAntennas == 2U) && (rangeProcObj->numSamplePerChirp * sizeof(cmplx16ImRe_t) >= (uint32_t)32768) ||
               (DPParams->numTxAntennas == 3U))
        {
            uint32_t    radarCubeAddr;
            uint8_t     numTxAnt;

            totalChirpIndex = rangeProcObj->chirpCount;
            numTxAnt = DPParams->numTxAntennas;

            radarCubeAddr = (uint32_t)(rangeProcObj->radarCubebuf + rangeProcObj->numSamplePerTx *(totalChirpIndex % numTxAnt) +
                                    (totalChirpIndex/numTxAnt)*rangeProcObj->numSamplePerChirp);
            EDMA_setDestinationAddress(edmaHandle, outChannel,
                (uint32_t)SOC_translateAddress((radarCubeAddr), SOC_TranslateAddr_Dir_TO_EDMA, NULL));
        }
        else
        {
            /* Other cases does not need manually set the destination address */
        }

        if(rangeProcObj->chirpCount > 1U)
        {
            startTime1 = Cycleprofiler_getTimeStamp();
            rangeProcDSP_WaitEDMAComplete (  edmaHandle, outChannel);
            waitingTime += (Cycleprofiler_getTimeStamp() - startTime1);
        }

        EDMA_startDmaTransfer(edmaHandle, outChannel);

        /* Increment chirp count */
        rangeProcObj->chirpCount++;

        /* Last chirp , wait until EDMA is completed */
        if(rangeProcObj->chirpCount == DPParams->numChirpsPerFrame)
        {
            /* Wait until last tansfer is done */
            rangeProcDSP_WaitEDMAComplete (  edmaHandle, outChannel);
            rangeProcObj->chirpCount = 0;
            outParams->endOfChirp = true;
        }

        rangeProcObj->numProcess++;
    }

    /* Update outParams */
    outParams->stats.processingTime = Cycleprofiler_getTimeStamp() - startTime - waitingTime;
    outParams->stats.waitTime = waitingTime;

    rangeProcObj->inProgress = false;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU control function. 
 *
 *  @pre    DPU_RangeProcDSP_init() has been called
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
int32_t DPU_RangeProcDSP_control
(
    DPU_RangeProcDSP_Handle     handle,
    DPU_RangeProcDSP_Cmd        cmd,
    void*                       arg,
    uint32_t                    argSize
)
{
    int32_t             retVal = 0;
    rangeProcDSPObj     *rangeProcObj;

    /* Get rangeProc data object */
    rangeProcObj = (rangeProcDSPObj *)handle;

    /* Sanity check */
    if (rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }

    /* Check if control() is called during processing time */
    if(rangeProcObj->inProgress == true)
    {
        retVal = DPU_RANGEPROCDSP_EINPROGRESS;
        goto exit;
    }

    /* Control command handling */
    switch(cmd)
    {
        case DPU_RangeProcDSP_Cmd_dcRangeCfg:
        {
            DPU_RangeProc_CalibDcRangeSigCfg   *calibDc;
            
            if((argSize != sizeof(DPU_RangeProc_CalibDcRangeSigCfg)) ||
               (arg == NULL))
            {
                retVal = DPU_RANGEPROCDSP_EINVAL;
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
                    retVal = rangeProcDSP_dcRangeSignatureCompensation_init(rangeProcObj, calibDc, 1U);
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

        default:
            retVal = DPU_RANGEPROCDSP_ECMD;
            break;
    }
exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU deinitl function. It frees up the 
 *   resources allocated during init.
 *
 *  @pre    DPU_RangeProcDSP_init() has been called
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
int32_t DPU_RangeProcDSP_deinit(DPU_RangeProcDSP_Handle handle)
{
    rangeProcDSPObj     *rangeProcObj;
    int32_t             retVal = 0;

    /* Sanity Check */
    rangeProcObj = (rangeProcDSPObj *)handle;
    if(rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCDSP_EINVAL;
        goto exit;
    }
    else
    {
        /* Free memory */
        MemoryP_ctrlFree(handle, sizeof(rangeProcDSPObj));
    }
exit:
    return (retVal);
}
