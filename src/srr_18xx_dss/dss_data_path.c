/**
*   @file  dss_data_path.c
*
*   @brief
*      Implements Data path processing functionality.
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
#if defined (SUBSYS_DSS)
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#endif
#define DebugP_ASSERT_ENABLED 1
#include <ti/drivers/osal/DebugP.h>
#include <assert.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>

#include <ti/alg/mmwavelib/mmwavelib.h>
/* C64P dsplib (fixed point part for C674X) */
#include "gen_twiddle_fft32x32.h"
#include "gen_twiddle_fft16x16.h"
#include "DSP_fft32x32.h"
#include "DSP_fft16x16.h"

/* C674x mathlib */
/* Suppress the mathlib.h warnings
 *  #48-D: incompatible redefinition of macro "TRUE"
 *  #48-D: incompatible redefinition of macro "FALSE" 
 */
#pragma diag_push
#pragma diag_suppress 48
#include <ti/mathlib/mathlib.h>
#pragma diag_pop

#include <common/app_cfg.h>
#include "dss_data_path.h"
#include "dss_config_edma_util.h"

/* Clustering library */
#include "clusteringDBscan.h"

/* Tracking library */
#include "EKF_XYZ_Interface.h"
#include "common/mmWave_XSS.h"

#define ROUND(x) ((x) < 0 ? ((x) - 0.5) : ((x) + 0.5) )

#define SOC_XWR18XX_DSS_L3RAM_SIZE                  0x100000

#define MMW_ADCBUF_SIZE     0x4000U

/*! @brief L2 heap used for allocating buffers in L2 SRAM,
mostly scratch buffers */
#define MMW_L2_HEAP_SIZE    0x19000U

/*! @brief L1 heap used for allocating buffers in L1D SRAM,
mostly scratch buffers */
#define MMW_L1_HEAP_SIZE    0x4000U

/*! L3 RAM buffer */
#pragma DATA_SECTION(gMmwL3, ".l3data");
#pragma DATA_ALIGN(gMmwL3, 8);
uint8_t gMmwL3[SOC_XWR18XX_DSS_L3RAM_SIZE];

/*! L2 Heap */
#pragma DATA_SECTION(gMmwL2, ".l2data");
#pragma DATA_ALIGN(gMmwL2, 8);
uint8_t gMmwL2[MMW_L2_HEAP_SIZE];

/*! L1 Heap */
#pragma DATA_SECTION(gMmwL1, ".l1data");
#pragma DATA_ALIGN(gMmwL1, 8);
uint8_t gMmwL1[MMW_L1_HEAP_SIZE];

/*! Types of FFT window */
/*! FFT window 16 - samples format is int16_t */
#define FFT_WINDOW_INT16 0
/*! FFT window 32 - samples format is int32_t */
#define FFT_WINDOW_INT32 1

/* FFT Window */
/*! Hamming window */
#define MMW_WIN_HAMMING  0
/*! Blackman window */
#define MMW_WIN_BLACKMAN 1
/*! Rectangular window */
#define MMW_WIN_RECT     2
/*! rectangularized Hanning window */
#define MMW_WIN_HANNING_RECT  3
/*! simple rounding function for float */
#define ROUNDF(x)((x) < 0 ? ((x) - 0.5f) : ((x) + 0.5f) )
/* Main control structure. */
extern MCB gMCB;
/* Local defines. */
#define pingPongId(x) ((x) & 0x1U)
#define isPong(x) (pingPongId(x))

void MmwDemo_genWindow(void *win,
    uint32_t windowDatumType,
    uint32_t winLen,
    uint32_t winGenLen,
    int32_t oneQformat,
    uint32_t winType);


uint32_t findKLargestPeaks(uint16_t * restrict cfarDetObjIndexBuf,
                           uint16_t * restrict cfarDetObjSNR, 
                           uint32_t numDetObjPerCfar, 
                           uint16_t * restrict sumAbs, 
                           uint16_t numBins, 
                           uint16_t K);

uint32_t pruneToPeaks(uint16_t* restrict cfarDetObjIndexBuf, 
                      uint16_t* restrict cfarDetObjSNR, 
                      uint32_t numDetObjPerCfar, 
                      uint16_t* restrict sumAbs, 
                      uint16_t numBins);

uint32_t pruneToPeaksOrNeighbourOfPeaks(uint16_t* restrict cfarDetObjIndexBuf, 
                      uint16_t* restrict cfarDetObjSNR, 
                      uint32_t numDetObjPerCfar, 
                      uint16_t* restrict sumAbs, 
                      uint16_t numBins);

uint32_t findandPopulateIntersectionOfDetectedObjects(
    DSS_DataPathObj * restrict obj,
    uint32_t numDetObjPerCfar,
    uint16_t dopplerLine,
    uint32_t numDetObj2D,
    uint16_t * restrict sumAbsRange);
    
uint32_t findandPopulateDetectedObjects(DSS_DataPathObj * restrict obj,    uint32_t numDetObjPerCfar,
                                        uint16_t dopplerLine, uint32_t numDetObj2D, uint16_t * restrict sumAbsRange);

uint32_t MmwDemo_cfarPeakGrouping(MmwDemo_detectedObjActual*  objOut, uint32_t numDetectedObjects,
                                uint16_t* detMatrix, uint32_t numRangeBins, uint32_t numDopplerBins,
                                uint32_t groupInDopplerDirection, uint32_t groupInRangeDirection);
                                
uint32_t secondDimFFTandLog2Computation(DSS_DataPathObj *obj, uint16_t * sumAbs, 
                                        uint16_t checkDetMatrixTx, uint16_t rangeIdx, 
                                        uint32_t * pingPongIdxPtr);


uint32_t cfarCa_SO_dBwrap_withSNR(const uint16_t inp[restrict],
                                uint16_t out[restrict], 
                                uint16_t outSNR[restrict], 
                                uint32_t len,
                                uint32_t const1, uint32_t const2,
                                uint32_t guardLen, uint32_t noiseLen);
uint32_t cfarCadB_SO_withSNR(const uint16_t inp[restrict],
                            uint16_t out[restrict], 
                            uint16_t outSNR[restrict], uint32_t len,
                            uint32_t const1, uint32_t const2,
                            uint32_t guardLen, uint32_t noiseLen, 
                            uint32_t minIndxToIgnoreHPF);

uint32_t aziEleProcessing(DSS_DataPathObj *obj, uint32_t subframeIndx);

uint16_t computeSinAzimSNR(float * azimuthMagSqr, uint16_t azimIdx, uint16_t numVirtualAntAzim, uint16_t numAngleBins, uint16_t xyzOutputQFormat);

float antilog2(int32_t inputActual, uint16_t fracBitIn);

void associateClustering(clusteringDBscanOutput_t * restrict output,
                         clusteringDBscanReport_t * restrict state, 
                         uint16_t maxNumTrackers, 
                         int32_t  epsilon2, 
                         int32_t vFactor);

uint32_t cfarPeakGroupingAlongDoppler(
                                MmwDemo_objRaw2D_t * restrict objOut,
                                uint32_t numDetectedObjects,
                                uint16_t* detMatrix,
                                uint32_t numRangeBins,
                                uint32_t numDopplerBins);
 
void populateOutputs(DSS_DataPathObj *obj);
uint32_t pruneTrackingInput(trackingInputReport_t * trackingInput, uint32_t numCluster);
float quadraticInterpFltPeakLoc(float * restrict y, int32_t len, int32_t indx);
float quadraticInterpLog2ShortPeakLoc(uint16_t * restrict y, int32_t len, int32_t indx, uint16_t fracBitIn);
void MmwDemo_XYcalc (DSS_DataPathObj *obj, uint32_t objIndex, uint16_t azimIdx, float * azimuthMagSqr);
void MmwDemo_addDopplerCompensation(int32_t dopplerIdx,
                                    int32_t numDopplerBins,
                                    uint32_t *azimuthModCoefs,
									uint32_t *azimuthModCoefsThirdBin,
									uint32_t *azimuthModCoefsTwoThirdBin,
                                    int64_t *azimuthIn,
                                    uint32_t numAnt,
                                    uint32_t numTxAnt,
                                    uint16_t txAntIdx);

static inline void MmwDemo_rxChanPhaseBiasCompensation(uint32_t *rxChComp,
                                         int64_t *input,
                                         uint32_t numAnt);

extern volatile cycleLog_t gCycleLog;

/**
*  @b Description
*  @n
*      selects one of four channels based on the subframe and the 'ping pong' ID
*
*/
uint8_t select_channel(uint8_t subframeIndx,
    uint8_t pingPongId,
    uint8_t option0ping,
    uint8_t option0pong)
{
    uint8_t chId;
    if (pingPongId == 0)
    {
        if (subframeIndx == 0)
        {
            chId = option0ping;
        } /*
        else
        {
            chId = option1ping;
        } */
    }
    else
    {
        if (subframeIndx == 0)
        {
            chId = option0pong;
        }
        /* else
        {
            chId = option1pong;
        } */
    }
    return chId;
}

/**
*  @b Description
*  @n
*      Starts a DMA transfer on a specifed channel corresponding to a given subframe.
*
*/
void MmwDemo_startDmaTransfer(EDMA_Handle handle, uint8_t channelId0, uint8_t subframeIndx)
{
    if (subframeIndx == 0)
    {
        EDMA_startDmaTransfer(handle, channelId0);
    }

}

/**
*  @b Description
*  @n
*      Resets the Doppler line bit mask. Doppler line bit mask indicates Doppler
*      lines (bins) on wich the CFAR in Doppler direction detected objects.
*      After the CFAR in Doppler direction is completed for all range bins, the
*      CFAR in range direction is performed on indicated Doppler lines.
*      The array dopplerLineMask is uint32_t array. The LSB bit of the first word
*      corresponds to Doppler line (bin) zero.
*
*/
void MmwDemo_resetDopplerLines(DSS_1D_DopplerLines_t * ths)
{
    memset((void *)ths->dopplerLineMask, 0, ths->dopplerLineMaskLen * sizeof(uint32_t));
    ths->currentIndex = 0;
}

/**
*  @b Description
*  @n
*      Sets the bit in the Doppler line bit mask dopplerLineMask corresponding to Doppler
*      line on which CFAR in Doppler direction detected object. Indicating the Doppler
*      line being active in observed frame. @sa MmwDemo_resetDopplerLines
*/
void MmwDemo_setDopplerLine(DSS_1D_DopplerLines_t * ths, uint16_t dopplerIndex)
{
    uint32_t word = dopplerIndex >> 5;
    uint32_t bit = dopplerIndex & 31;

    ths->dopplerLineMask[word] |= (0x1 << bit);
}

/**
*  @b Description
*  @n
*      Checks whether Doppler line is active in the observed frame. It checks whether the bit
*      is set in the Doppler line bit mask corresponding to Doppler
*      line on which CFAR in Doppler direction detected object.
*      @sa MmwDemo_resetDopplerLines
*/
uint32_t MmwDemo_isSetDopplerLine(DSS_1D_DopplerLines_t * ths, uint16_t index)
{
    uint32_t dopplerLineStat;
    uint32_t word = index >> 5;
    uint32_t bit = index & 31;

    if (ths->dopplerLineMask[word] & (0x1 << bit))
    {
        dopplerLineStat = 1;
    }
    else
    {
        dopplerLineStat = 0;
    }
    return dopplerLineStat;
}

/**
*  @b Description
*  @n
*      Gets the Doppler index from the Doppler line bit mask, starting from the
*      smallest active Doppler lin (bin). Subsequent calls return the next
*      active Doppler line. @sa MmwDemo_resetDopplerLines
*
*/
int32_t MmwDemo_getDopplerLine(DSS_1D_DopplerLines_t * ths)
{
    uint32_t index = ths->currentIndex;
    uint32_t word = index >> 5;
    uint32_t bit = index & 31;

    while (((ths->dopplerLineMask[word] >> bit) & 0x1) == 0)
    {
        index++;
        bit++;
        if (bit == 32)
        {
            word++;
            bit = 0;
            
        }
    }
    ths->currentIndex = index + 1;
    return index;
}


/**
*  @b Description
*  @n
*      Power of 2 round up function.
*/
uint32_t MmwDemo_pow2roundup(uint32_t x)
{
    uint32_t result = 1;
    while (x > result)
    {
        result <<= 1;
    }
    return result;
}

/**
*  @b Description
*  @n
*      Calculates X/Y coordinates in meters based on the maximum position in
*      the magnitude square of the azimuth FFT output. The function is called
*      per detected object. The detected object structure already has populated
*      range and doppler indices. This function finds maximum index in the
*      azimuth FFT, calculates X and Y and coordinates and stores them into
*      object fields along with the peak height. Also it populates the azimuth
*      index in azimuthMagSqr array.
*
*  @param[in] obj                Pointer to data path object
*  @param[in] objIndex           Detected object index
*
*  @retval
*      NONE
*/
void MmwDemo_XYestimation(DSS_DataPathObj *obj,    uint32_t objIndex)
{
    uint32_t i;
    float *azimuthMagSqr = obj->azimuthMagSqr;
    uint32_t xyzOutputQFormat = obj->xyzOutputQFormat;
    uint32_t oneQFormat = (1 << xyzOutputQFormat);
    uint32_t numAngleBins = obj->numAngleBins;
    uint32_t numSearchBins;
    uint16_t azimIdx = 0;
    float maxVal = 0;
    
    if(obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        numSearchBins = numAngleBins*2;
    }
    else
    {
        numSearchBins = numAngleBins;
    }
    
    /* Find peak position - search in original and flipped output */
    for (i = 0; i < numSearchBins ; i++)
    {
        if (azimuthMagSqr[i] > maxVal)
        {
            azimIdx = i;
            maxVal = azimuthMagSqr[i];
        }
    }
    
    if(obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        if(azimIdx >= numAngleBins)
        {
            /* Velocity aliased: |velocity| > Vmax */
            /* Correct peak index: */
            azimIdx -= numAngleBins;
            
            /* Use the second azimuthMagSqr array for further computation. */ 
            azimuthMagSqr += numAngleBins;
            
            /* Correct velocity */
            if (obj->detObj2D[objIndex].speed < 0)
            {
                obj->detObj2D[objIndex].speed += (int16_t) (2  * obj->maxUnambiguousVel * (float)oneQFormat);
            }
            else
            {
                obj->detObj2D[objIndex].speed -= (int16_t) (2  * obj->maxUnambiguousVel * (float)oneQFormat);            
            }
        }
    }
    
    MmwDemo_XYcalc (obj, objIndex, azimIdx, azimuthMagSqr);

    /* Check for second peak. */
    if (obj->multiObjBeamFormingCfg.enabled)
    {
        uint32_t leftSearchIdx;
        uint32_t rightSearchIdx;
        uint32_t secondSearchLen;
        uint32_t iModAzimLen;
        float maxVal2;
        int32_t k;

        /* Find right edge of the first peak. */
        i = azimIdx;
        leftSearchIdx = (i + 1) & (numAngleBins - 1);
        k = numAngleBins;
        while ((azimuthMagSqr[i] >= azimuthMagSqr[leftSearchIdx]) && (k > 0))
        {
            i = (i + 1) & (numAngleBins - 1);
            leftSearchIdx = (leftSearchIdx + 1) & (numAngleBins - 1);
            k--;
        }

        /* Find left edge of the first peak. */
        i = azimIdx;
        rightSearchIdx = (i - 1) & (numAngleBins - 1);
        k = numAngleBins;
        while ((azimuthMagSqr[i] >= azimuthMagSqr[rightSearchIdx]) && (k > 0))
        {
            i = (i - 1) & (numAngleBins - 1);
            rightSearchIdx = (rightSearchIdx - 1) & (numAngleBins - 1);
            k--;
        }

        secondSearchLen = ((rightSearchIdx - leftSearchIdx) & (numAngleBins - 1)) + 1;
        /* Find second peak. */
        maxVal2 = azimuthMagSqr[leftSearchIdx];
        azimIdx = leftSearchIdx;
        for (i = leftSearchIdx; i < (leftSearchIdx + secondSearchLen); i++)
        {
            iModAzimLen = i & (numAngleBins - 1);
            if (azimuthMagSqr[iModAzimLen] > maxVal2)
            {
                azimIdx = iModAzimLen;
                maxVal2 = azimuthMagSqr[iModAzimLen];
            }
        }

        /* Is second peak greater than threshold? */
        if (maxVal2 > (maxVal * obj->multiObjBeamFormingCfg.multiPeakThrsScal) && (obj->numDetObj < MRR_MAX_OBJ_OUT))
        {
            /* Second peak detected! Add it to the end of the list. */
            obj->detObj2D[obj->numDetObj] = obj->detObj2D[objIndex];
            objIndex = obj->numDetObj;
            obj->numDetObj++;
            
            MmwDemo_XYcalc (obj, objIndex, azimIdx, azimuthMagSqr);
        }
    }
}


/**
*  @b Description
*  @n
*      Calculates X/Y coordinates in meters based on the maximum position in
*      the magnitude square of the azimuth FFT output. The function is called
*      per detected object. The detected object structure already has populated
*      range and doppler indices. This function finds maximum index in the
*      azimuth FFT, calculates X and Y and coordinates and stores them into
*      object fields along with the peak height. Also it populates the azimuth
*      index in azimuthMagSqr array.
*
*  @param[in] obj                Pointer to data path object
*  @param[in] objIndex           Detected object index
*
*  @retval
*      NONE
*/
void MmwDemo_XYZestimation(DSS_DataPathObj *obj,    uint32_t objIndex)
{
    uint32_t i;
    float *azimuthMagSqr = obj->azimuthMagSqr;
   uint32_t xyzOutputQFormat = obj->xyzOutputQFormat; 
    uint32_t oneQFormat = (1 << xyzOutputQFormat); 
    uint32_t numAngleBins = obj->numAngleBins;
    uint32_t numSearchBins;
    uint16_t azimIdx = 0;
    float maxVal = 0;
    
    if (MAX_VEL_POINT_CLOUD_PROCESSING_IS_ENABLED)
    {
        if(obj->processingPath == POINT_CLOUD_PROCESSING)
        {
            numSearchBins = numAngleBins*2;
        }
        else
        {
            numSearchBins = numAngleBins;
        }
    }
    else
    {
        numSearchBins = numAngleBins;
    }
    /* Find peak position - search in original and flipped output */
    for (i = 0; i < numSearchBins ; i++)
    {
        if (azimuthMagSqr[i] > maxVal)
        {
            azimIdx = i;
            maxVal = azimuthMagSqr[i];
        }
    }
    
    if(obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        if (MAX_VEL_POINT_CLOUD_PROCESSING_IS_ENABLED)
        {
            
            if(azimIdx >= numAngleBins)
            {
                /* Velocity aliased: |velocity| > Vmax */
                /* Correct peak index: */
                azimIdx -= numAngleBins;
                
                /* Use the second azimuthMagSqr array for further computation. */ 
                azimuthMagSqr += numAngleBins;
                
                /* Correct velocity */
                if (obj->detObj2D[objIndex].speed < 0)
                {
                    obj->detObj2D[objIndex].speed += (int16_t) (2  * obj->maxUnambiguousVel * (float)oneQFormat);
                }
                else
                {
                    obj->detObj2D[objIndex].speed -= (int16_t) (2  * obj->maxUnambiguousVel * (float)oneQFormat);            
                }
            }
        }
    }
    
    
    MmwDemo_XYZcalc (obj, objIndex, azimIdx, azimuthMagSqr);

    /* Check for second peak. */
    
    if (obj->multiObjBeamFormingCfg.enabled)
    {
        uint32_t leftSearchIdx;
        uint32_t rightSearchIdx;
        uint32_t secondSearchLen;
        uint32_t iModAzimLen;
        float maxVal2;
        int32_t k;

        /* Find right edge of the first peak. */
        i = azimIdx;
        leftSearchIdx = (i + 1) & (numAngleBins - 1);
        k = numAngleBins;
        while ((azimuthMagSqr[i] >= azimuthMagSqr[leftSearchIdx]) && (k > 0))
        {
            i = (i + 1) & (numAngleBins - 1);
            leftSearchIdx = (leftSearchIdx + 1) & (numAngleBins - 1);
            k--;
        }

        /* Find left edge of the first peak. */
        i = azimIdx;
        rightSearchIdx = (i - 1) & (numAngleBins - 1);
        k = numAngleBins;
        while ((azimuthMagSqr[i] >= azimuthMagSqr[rightSearchIdx]) && (k > 0))
        {
            i = (i - 1) & (numAngleBins - 1);
            rightSearchIdx = (rightSearchIdx - 1) & (numAngleBins - 1);
            k--;
        }

        secondSearchLen = ((rightSearchIdx - leftSearchIdx) & (numAngleBins - 1)) + 1;
        /* Find second peak. */
        maxVal2 = azimuthMagSqr[leftSearchIdx];
        azimIdx = leftSearchIdx;
        for (i = leftSearchIdx; i < (leftSearchIdx + secondSearchLen); i++)
        {
            iModAzimLen = i & (numAngleBins - 1);
            if (azimuthMagSqr[iModAzimLen] > maxVal2)
            {
                azimIdx = iModAzimLen;
                maxVal2 = azimuthMagSqr[iModAzimLen];
            }
        }

        /* Is second peak greater than threshold? */
        if (maxVal2 > (maxVal * obj->multiObjBeamFormingCfg.multiPeakThrsScal) && (obj->numDetObj < MRR_MAX_OBJ_OUT))
        {
            /* Second peak detected! Add it to the end of the list. */
            obj->detObj2D[obj->numDetObj] = obj->detObj2D[objIndex];
            objIndex = obj->numDetObj;
            obj->numDetObj++;
            
            MmwDemo_XYZcalc (obj, objIndex, azimIdx, azimuthMagSqr);
        }
    }
    
}

/**
*  @b Description
*  @n
*      Waits for 1D FFT data to be transferrd to input buffer.
*      This is a blocking function.
*
*  @param[in] obj  Pointer to data path object
*  @param[in] pingPongId ping-pong id (ping is 0 and pong is 1)
*  @param[in] subframeIndx
*
*  @retval
*      NONE
*/
void MmwDemo_dataPathWait1DInputData(DSS_DataPathObj *obj, uint32_t pingPongId, uint32_t subframeIndx)
{
    /* wait until transfer done */
    volatile bool isTransferDone;
    uint8_t chId;
    if (pingPongId == 0)
    {
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_IN_PING;
        } /*
        else
        {
            chId = MRR_SF1_EDMA_CH_1D_IN_PING;
        }*/
    }
    else
    {

        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_IN_PONG;
        }
        /* else
        {
            chId = MRR_SF1_EDMA_CH_1D_IN_PONG;
        } */

    }
    do {
        if (EDMA_isTransferComplete(obj->edmaHandle[EDMA_INSTANCE_DSS],
            chId,
            (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
            dssAssert(0);
        }
    } while (isTransferDone == false);
}

/**
*  @b Description
*  @n
*      Waits for 1D FFT data to be transferred to output buffer.
*      This is a blocking function.
*
*  @param[in] obj  Pointer to data path object
*  @param[in] pingPongId ping-pong id (ping is 0 and pong is 1)
*  @param[in] subframeIndx
*
*  @retval
*      NONE
*/
void MmwDemo_dataPathWait1DOutputData(DSS_DataPathObj *obj, uint32_t pingPongId, uint32_t subframeIndx)
{
    volatile bool isTransferDone;
    /* select the right EDMA channel based on the subframeIndx, and the pinPongId. */
    uint8_t chId = select_channel(subframeIndx, pingPongId, \
        MRR_SF0_EDMA_CH_1D_OUT_PING, MRR_SF0_EDMA_CH_1D_OUT_PONG);

    /* wait until transfer done */
    do {
        if (EDMA_isTransferComplete(obj->edmaHandle[EDMA_INSTANCE_DSS],
            chId,
            (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
            dssAssert(0);
        }
    } while (isTransferDone == false);
}

/**
*  @b Description
*  @n
*      Waits for 1D FFT data to be transferred to input buffer for 2D-FFT caclulation.
*      This is a blocking function.
*
*  @param[in] obj  Pointer to data path object
*  @param[in] pingPongId ping-pong id (ping is 0 and pong is 1)
*  @param[in] subframe Index 
*
*  @retval
*      NONE
*/
void MmwDemo_dataPathWait2DInputData(DSS_DataPathObj *obj, uint32_t pingPongId, uint32_t subframeIndx)
{
    volatile bool isTransferDone;

    /* select the right EDMA channel based on the subframeIndx, and the pinPongId. */
    uint8_t chId = select_channel(subframeIndx, pingPongId, \
        MRR_SF0_EDMA_CH_2D_IN_PING, MRR_SF0_EDMA_CH_2D_IN_PONG );

    /* wait until transfer done */
    do {
        if (EDMA_isTransferComplete(obj->edmaHandle[EDMA_INSTANCE_DSS],
            chId,
            (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
            dssAssert(0);
        }
    } while (isTransferDone == false);
}

/**
*  @b Description
*  @n
*      Waits for 1D FFT data to be transferred to input buffer for 3D-FFT calculation.
*      This is a blocking function.
*
*  @param[in] obj  Pointer to data path object
*  @param[in] pingPongId ping-pong id (ping is 0 and pong is 1)
*  @param[in] subframeIndx
*
*  @retval
*      NONE
*/
void MmwDemo_dataPathWait3DInputData(DSS_DataPathObj *obj, uint32_t pingPongId, uint32_t subframeIndx)
{
    /* wait until transfer done */
    volatile bool isTransferDone;
    uint8_t chId = select_channel(subframeIndx, pingPongId, \
        MRR_SF0_EDMA_CH_3D_IN_PING, MRR_SF0_EDMA_CH_3D_IN_PONG);
    do 
    {
        if (EDMA_isTransferComplete(obj->edmaHandle[EDMA_INSTANCE_DSS],
            chId,
            (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
            dssAssert(0);
        }
    } while (isTransferDone == false);
}

/**
*  @b Description
*  @n
*      Waits for 2D FFT calculated data to be transferred out from L2 memory
*      to detection matrix located in L3 memory.
*      This is a blocking function.
*
*  @param[in] obj  Pointer to data path object
*  @param[in] subframeIndx
*
*  @retval
*      NONE
*/
void MmwDemo_dataPathWaitTransDetMatrix(DSS_DataPathObj *obj, uint8_t subframeIndx)
{
    volatile bool isTransferDone;
    uint8_t chId;
    if (subframeIndx == 0)
    {
        chId = MRR_SF0_EDMA_CH_DET_MATRIX;
    }
    /* else
    {
        chId = MRR_SF1_EDMA_CH_DET_MATRIX;
    } */

    do 
    {
        if (EDMA_isTransferComplete(obj->edmaHandle[EDMA_INSTANCE_DSS],
            (uint8_t)chId,
            (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
            dssAssert(0);
        }
    } while (isTransferDone == false);
}

/**
*  @b Description
*  @n
*      Waits for 2D FFT data to be transferred from detection matrix in L3
*      memory to L2 memory for CFAR detection in range direction.
*      This is a blocking function.
*
*  @param[in] obj  Pointer to data path object
*  @param[in] subframeIndx
*
*  @retval
*      NONE
*/
void MmwDemo_dataPathWaitTransDetMatrix2(DSS_DataPathObj *obj, uint32_t subframeIndx)
{

    /* wait until transfer done */
    volatile bool isTransferDone;
    uint8_t chId;
    if (subframeIndx == 0)
    {
        chId = MRR_SF0_EDMA_CH_DET_MATRIX2;
    }

    /*else
    {
        chId = MRR_SF1_EDMA_CH_DET_MATRIX2;
    } */

    do 
    {
        if (EDMA_isTransferComplete(obj->edmaHandle[EDMA_INSTANCE_DSS],
            (uint8_t)chId,
            (bool *)&isTransferDone) != EDMA_NO_ERROR)
        {
            dssAssert(0);
        }
    } while (isTransferDone == false);
}

/**
*  @b Description
*  @n
*      Configures all EDMA channels and param sets used in data path processing
*
*  This function is very similar to the dataPathConfigEDMA from the OOB demo, but
*  with the difference that we have two subframes, and one subframe can support the
*  maximum velocity enhancement modification. In this method , the 2nd dimension has 
*  two kinds of chirps and each chirp is repeated 'numDopplerBins' times, and each 
*  chirp has the same  number of adc samples. 
* 
*  We would also like to ensure that when the data is transferred to
*  L3 RAM, a range gate (i.e. doppler bins corresponding to a range bin) of each
*  'chirptype' is contiguous, so that a single EDMA can pull them both out in
*  the 2nd dimension processing.
* 
*  Hence the EDMAs corresponding to the transfer of 1D data to L3 and the transfer of data 
*  from L3 to L2 are modified. 
*
*  @param[in] obj  Pointer to data path object array.
* 
*  @retval
*      -1 if error, 0 for no error
*/
int32_t DSS_dataPathConfigEdma(DSS_DataPathObj *obj)
{
    uint32_t eventQueue;
    uint16_t shadowParam = EDMA_NUM_DMA_CHANNELS;
    int32_t retVal = 0;
    uint8_t chId;
    uint8_t subframeIndx;
    uint32_t numChirpTypes = 1;
    uint32_t ADCBufferoffset = (32 * 1024)/4;

    for (subframeIndx = 0; subframeIndx < NUM_SUBFRAMES; subframeIndx++, obj++)
    {
        
        
        numChirpTypes = 1; 
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            numChirpTypes = 2;
        }
        
        /*****************************************************
        * EDMA configuration for getting ADC data from ADC buffer
        * to L2 (prior to 1D FFT)
        * For ADC Buffer to L2 use EDMA-A TPTC =1
        *****************************************************/
        eventQueue = 0U;

        /* Ping - copies chirp samples from even antenna numbers (e.g. RxAnt0 and RxAnt2) */

        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_IN_PING;
        }
       /* else
        {
            chId = MRR_SF1_EDMA_CH_1D_IN_PING;
        }
        */

        retVal =
            EDMAutil_configType1(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)(&obj->ADCdataBuf[0]),
                (uint8_t *)(SOC_translateAddress((uint32_t)&obj->adcDataIn[0], SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                chId,
                false,
                shadowParam++,
                obj->numAdcSamples * BYTES_PER_SAMP_1D,
                MAX(obj->numRxAntennas / 2, 1),
                ADCBufferoffset * 2,
                0,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_IN_PONG;
        }
       /* else
        {
            chId = MRR_SF1_EDMA_CH_1D_IN_PONG;
        } */

        /* Pong - copies chirp samples from odd antenna numbers (e.g. RxAnt1 and RxAnt3) 
         * Note that ADCBufferoffset is in bytes, but ADCdataBuf is in cmplx16ReIm_t. 
         * There are four bytes in one cmplx16ReIm_t*/
        retVal =
            EDMAutil_configType1(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)(&obj->ADCdataBuf[(ADCBufferoffset>>2)]), 
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->adcDataIn[obj->numRangeBins]), SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                chId,
                false,
                shadowParam++,
                obj->numAdcSamples * BYTES_PER_SAMP_1D,
                MAX(obj->numRxAntennas / 2, 1),
                ADCBufferoffset * 2,
                0,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        /* using different event queue between input and output to parallelize better */
        eventQueue = 1U;
        /*
        * EDMA configuration for storing 1d fft output in transposed manner to L3.
        * It copies all Rx antennas of the chirp per trigger event.
        */


        /* Ping - Copies from ping FFT output (even chirp indices)  to L3 */
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_OUT_PING;
        }
        /*else
        {
            chId = MRR_SF1_EDMA_CH_1D_OUT_PING;
        }*/


        retVal =
            EDMAutil_configType2a(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->fftOut1D[0]), SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                (uint8_t *)(&obj->radarCube[0]),
                chId,
                false,
                shadowParam++,
                BYTES_PER_SAMP_1D,
                obj->numRangeBins,
                obj->numTxAntennas * numChirpTypes,
                obj->numRxAntennas,
                obj->numDopplerBins,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        /* Ping - Copies from pong FFT output (odd chirp indices)  to L3 */
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_OUT_PONG;
        }
        /*else
        {
            chId = MRR_SF1_EDMA_CH_1D_OUT_PONG;
        }*/

        retVal =
            EDMAutil_configType2a(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->fftOut1D[obj->numRxAntennas * obj->numRangeBins]),
                    SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                (uint8_t *)(&obj->radarCube[0]),
                chId,
                false,
                shadowParam++,
                BYTES_PER_SAMP_1D,
                obj->numRangeBins,
                obj->numTxAntennas * numChirpTypes,
                obj->numRxAntennas,
                obj->numDopplerBins,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }


        /*****************************************
        * Interframe processing related EDMA configuration
        *****************************************/
        eventQueue = 0U;

        /* For the max-vel enh implementation, we pull out twice as much range-gates per range bin.
         * Hence  EDMA BCNT is multiplied by 2. */
        
        /* Ping: This DMA channel is programmed to fetch the 1D FFT data from radarCube
        * matrix in L3 mem of even antenna rows into the Ping Buffer in L2 mem*/
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_2D_IN_PING;
        }
        /*else
        {
            chId = MRR_SF1_EDMA_CH_2D_IN_PING;
        }*/

        retVal =
            EDMAutil_configType1(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)(&obj->radarCube[0]),
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->dstPingPong[0]), SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                chId,
                false,
                shadowParam++,
                obj->numDopplerBins * BYTES_PER_SAMP_1D,
                (obj->numRangeBins * obj->numRxAntennas * obj->numTxAntennas * numChirpTypes) / 2,
                obj->numDopplerBins * BYTES_PER_SAMP_1D * 2,
                0,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        /* Pong: This DMA channel is programmed to fetch the 1D FFT data from radarCube
        * matrix in L3 mem of odd antenna rows into thePong Buffer in L2 mem*/
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_2D_IN_PONG;
        }
/*        else
        {
            chId = MRR_SF1_EDMA_CH_2D_IN_PONG;
        } */

        retVal =
            EDMAutil_configType1(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)(&obj->radarCube[obj->numDopplerBins]),
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->dstPingPong[obj->numDopplerBins]),
                    SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                chId,
                false,
                shadowParam++,
                obj->numDopplerBins * BYTES_PER_SAMP_1D,
                (obj->numRangeBins * obj->numRxAntennas * obj->numTxAntennas * numChirpTypes) / 2,
                obj->numDopplerBins * BYTES_PER_SAMP_1D * 2,
                0,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        eventQueue = 1U;
        /* This EDMA channel copies the sum (across virtual antennas) of log2
        * magnitude squared of Doppler FFT bins from L2 mem to detection
        * matrix in L3 mem. */
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_DET_MATRIX;
        }
        /*else
        {
            chId = MRR_SF1_EDMA_CH_DET_MATRIX;
        }*/

        retVal =
            EDMAutil_configType1(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->sumAbs[0U]), SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->detMatrix[0U]), SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
               /* (uint8_t *)(obj->detMatrix),*/
                chId,
                false,
                shadowParam++,
                obj->numDopplerBins * BYTES_PER_SAMP_DET,
                obj->numRangeBins,
                0,
                obj->numDopplerBins * BYTES_PER_SAMP_DET,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        /* This EDMA Channel brings selected range bins  from detection matrix in
        * L3 mem (reading in transposed manner) into L2 mem for CFAR detection (in
        * range direction). */
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_DET_MATRIX2;
        }
/*        else
        {
            chId = MRR_SF1_EDMA_CH_DET_MATRIX2;
        } */

        retVal =
            EDMAutil_configType3(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)0,
                (uint8_t *)0,
                chId,
                false,
                shadowParam++,
                BYTES_PER_SAMP_DET, \
                obj->numRangeBins,
                (obj->numDopplerBins * BYTES_PER_SAMP_DET),
                BYTES_PER_SAMP_DET,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        /*********************************************************
        * These EDMA Channels are for Azimuth calculation. They bring
        * 1D FFT data for 2D DFT and Azimuth FFT calculation.
        ********************************************************/
        /* Ping: This DMA channel is programmed to fetch the 1D FFT data from radarCube
        * matrix in L3 mem of even antenna rows into the Ping Buffer in L2 mem.
        */
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_3D_IN_PING;
        }
/*        else
        {
            chId = MRR_SF1_EDMA_CH_3D_IN_PING;
        } */

        retVal =
            EDMAutil_configType1(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)NULL,
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->dstPingPong[0]), SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                chId,
                false,
                shadowParam++,
                obj->numDopplerBins * BYTES_PER_SAMP_1D,
                MAX((obj->numRxAntennas * obj->numTxAntennas) / 2, 1),
                (obj->numDopplerBins * BYTES_PER_SAMP_1D * 2),
                0,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

        /* Pong: This DMA channel is programmed to fetch the 1D FFT data from radarCube
        * matrix in L3 mem of odd antenna rows into the Pong Buffer in L2 mem*/
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_3D_IN_PONG;
        }
        /* else
        {
            chId = MRR_SF1_EDMA_CH_3D_IN_PONG;
        } */

        retVal =
            EDMAutil_configType1(obj->edmaHandle[EDMA_INSTANCE_DSS],
                (uint8_t *)NULL,
                (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->dstPingPong[obj->numDopplerBins]), SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                chId,
                false,
                shadowParam++,
                obj->numDopplerBins * BYTES_PER_SAMP_1D,
                MAX((obj->numRxAntennas * obj->numTxAntennas) / 2, 1),
                obj->numDopplerBins * BYTES_PER_SAMP_1D * 2,
                0,
                eventQueue,
                NULL,
                (uintptr_t)obj);
        if (retVal < 0)
        {
            return -1;
        }

    }

    return(0);
}

/**
*  @b Description
*  @n
*    This function populates the ObjOut based on the objRaw. It includes one more layer
*    of pruning which prevent objects beyond the maximum range or minimum range from 
*    being populated. 
*    Additionally we change the SNR requirement as a function of the range, requiring 
*    larger SNR for objects closer to the car, and lower SNR for objects farther from 
*    the car. 
*  @param[out]   objOut             Output array of  detected objects after peak grouping
*  @param[in]    objRaw             Array of detected objects after CFAR detection
*  @param[in]    SNRThresh          A list of SNR thresholds for a list of ranges. 
*  @param[in]    SNRThresh          A list of peakVal thresholds for a list of ranges. 
*  @param[in]    numDetectedObjects Number of detected objects by CFAR
*  @param[in]    numDopplerBins     Number of Doppler bins
*  @param[in]    maxRange           Maximum range (in ONE_QFORMAT)
*  @param[in]    minRange           Minimum range (in ONE_QFORMAT)
*
*  @retval
*      Number of detected objects after grouping
*/
uint32_t rangeBasedPruning(
    MmwDemo_detectedObjActual*  restrict objOut,
    MmwDemo_objRaw2D_t * restrict objRaw,
    RangeDependantThresh_t * restrict SNRThresh, 
    RangeDependantThresh_t * restrict peakValThresh, 
    uint32_t numDetectedObjects,
    uint32_t numDopplerBins,
    uint32_t maxRange,
    uint32_t minRange)
{
    int32_t i, j, k;
    uint32_t numObjOut = 0;
    uint32_t searchSNRThresh = 0;
    uint32_t searchpeakValThresh = 0;
    j = 0;
    k = 0;         
    /* No grouping, copy all detected objects to the output matrix within specified min max range
     * with the necessary SNR. */
    for (i = 0; i < numDetectedObjects; i++)
    {
        if ((objRaw[i].range <= maxRange) && ((objRaw[i].range >= minRange)))
        {
            /* We change the SNR requirement as a function of the range, requiring larger 
             * SNR for objects closer to the car, and lower SNR for objects farther from 
             * the car. */
            searchSNRThresh = 0;
            
            /* Check if  the range (of the target) lies between SNRThresh[j].rangelim and
             * SNRThresh[j-1].rangelim. If it doesn't search for a new SNR threshold. */
            if (objRaw[i].range > SNRThresh[j].rangelim)
            {
                searchSNRThresh = 1;
            }
            else if (j > 0) 
            {
                if (objRaw[i].range < SNRThresh[j-1].rangelim)
                {
                    searchSNRThresh = 1;
                }
            }
            
            if (searchSNRThresh == 1)
            {
                /* MAX_NUM_SNR_THRESH_LIM is typically 3; A linear search should be fine */
                for (j = 0; j < MAX_NUM_RANGE_DEPENDANT_SNR_THRESHOLDS - 1; j++) 
                {
                    if (objRaw[i].range < SNRThresh[j].rangelim)
                    {
                        break;
                    }
                }    
            }
            
            /* Ditto for the peakValThresh. */
            searchpeakValThresh = 0;
            
            if (objRaw[i].range > peakValThresh[k].rangelim)
            {
                searchpeakValThresh = 1;
            }
            else if (k > 0) 
            {
                if (objRaw[i].range < peakValThresh[k-1].rangelim)
                {
                    searchpeakValThresh = 1;
                }
            }
            
            if (searchpeakValThresh == 1)
            {
                for (k = 0; k < MAX_NUM_RANGE_DEPENDANT_SNR_THRESHOLDS - 1; k++) 
                {
                    if (objRaw[i].range < peakValThresh[k].rangelim)
                    {
                        break;
                    }
                }    
            }
            
                        
            if ( (objRaw[i].rangeSNRdB > SNRThresh[j].threshold) && 
                 (objRaw[i].peakVal > peakValThresh[k].threshold) )
            {
                objOut[numObjOut].rangeIdx      = objRaw[i].rangeIdx;
                objOut[numObjOut].dopplerIdx    = objRaw[i].dopplerIdx;
                objOut[numObjOut].range         = objRaw[i].range;
                objOut[numObjOut].speed         = objRaw[i].speed;
                objOut[numObjOut].peakVal       = objRaw[i].peakVal;
                objOut[numObjOut].rangeSNRdB    = objRaw[i].rangeSNRdB;
                objOut[numObjOut].dopplerSNRdB  = objRaw[i].dopplerSNRdB;        
                numObjOut++;
                
                if (numObjOut == MRR_MAX_OBJ_OUT)
                {
                    break;
                }
            }
           
        }
    }
    return numObjOut;
}

/**
*  @b Description
*  @n
*    Outputs magnitude squared float array of input complex32 array
*
*  @retval
*      Not Applicable.
*/
void MmwDemo_magnitudeSquared(cmplx32ReIm_t * restrict inpBuff, float * restrict magSqrdBuff, uint32_t numSamples)
{
    uint32_t i;
    for (i = 0; i < numSamples; i++)
    {
        magSqrdBuff[i] = (float)inpBuff[i].real * (float)inpBuff[i].real +
            (float)inpBuff[i].imag * (float)inpBuff[i].imag;
    }
}

/**
*  @b Description
*  @n
*    Compensation of DC range antenna signature (unused currently)
*
*
*  @retval
*      Not Applicable.
*/
void MmwDemo_dcRangeSignatureCompensation(DSS_DataPathObj *obj, uint8_t chirpPingPongId)
{
    uint32_t rxAntIdx, binIdx;
    uint32_t ind;
    int32_t chirpPingPongOffs;
    int32_t chirpPingPongSize;

    chirpPingPongSize = obj->numRxAntennas * (obj->calibDcRangeSigCfg.positiveBinIdx - obj->calibDcRangeSigCfg.negativeBinIdx + 1);
    if (obj->dcRangeSigCalibCntr == 0)
    {
        memset(obj->dcRangeSigMean, 0, obj->numTxAntennas * chirpPingPongSize * sizeof(cmplx32ImRe_t));
    }

    chirpPingPongOffs = chirpPingPongId * chirpPingPongSize;

    /* Calibration */
    if (obj->dcRangeSigCalibCntr < (obj->calibDcRangeSigCfg.numAvgChirps * obj->numTxAntennas))
    {
        /* Accumulate */
        ind = 0;
        for (rxAntIdx = 0; rxAntIdx < obj->numRxAntennas; rxAntIdx++)
        {
            uint32_t chirpInOffs = chirpPingPongId * (obj->numRxAntennas * obj->numRangeBins) + (obj->numRangeBins * rxAntIdx);
            int64_t *meanPtr = (int64_t *)&obj->dcRangeSigMean[chirpPingPongOffs];
            uint32_t *fftPtr = (uint32_t *)&obj->fftOut1D[chirpInOffs];
            int64_t meanBin;
            uint32_t fftBin;
            int32_t Re, Im;
            
            for (binIdx = 0; binIdx <= obj->calibDcRangeSigCfg.positiveBinIdx; binIdx++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                fftBin = _amem4(&fftPtr[binIdx]);
                Im = _loll(meanBin) + _ext(fftBin, 0, 16);
                Re = _hill(meanBin) + _ext(fftBin, 16, 16);
                _amem8(&meanPtr[ind]) = _itoll(Re, Im);
                ind++;
            }

            chirpInOffs = chirpPingPongId * (obj->numRxAntennas * obj->numRangeBins) + (obj->numRangeBins * rxAntIdx) + obj->numRangeBins + obj->calibDcRangeSigCfg.negativeBinIdx;
            fftPtr = (uint32_t *)&obj->fftOut1D[chirpInOffs];
            for (binIdx = 0; binIdx < -obj->calibDcRangeSigCfg.negativeBinIdx; binIdx++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                fftBin = _amem4(&fftPtr[binIdx]);
                Im = _loll(meanBin) + _ext(fftBin, 0, 16);
                Re = _hill(meanBin) + _ext(fftBin, 16, 16);
                _amem8(&meanPtr[ind]) = _itoll(Re, Im);
                ind++;
            }
        }
        obj->dcRangeSigCalibCntr++;

        if (obj->dcRangeSigCalibCntr == (obj->calibDcRangeSigCfg.numAvgChirps * obj->numTxAntennas))
        {
            /* Divide */
            int64_t *meanPtr = (int64_t *)obj->dcRangeSigMean;
            int32_t Re, Im;
            int64_t meanBin;
            int32_t divShift = obj->log2NumAvgChirps;
            for (ind = 0; ind < (obj->numTxAntennas * chirpPingPongSize); ind++)
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
        for (rxAntIdx = 0; rxAntIdx < obj->numRxAntennas; rxAntIdx++)
        {
            uint32_t chirpInOffs = chirpPingPongId * (obj->numRxAntennas * obj->numRangeBins) + (obj->numRangeBins * rxAntIdx);
            int64_t *meanPtr = (int64_t *)&obj->dcRangeSigMean[chirpPingPongOffs];
            uint32_t *fftPtr = (uint32_t *)&obj->fftOut1D[chirpInOffs];
            int64_t meanBin;
            uint32_t fftBin;
            int32_t Re, Im;
            for (binIdx = 0; binIdx <= obj->calibDcRangeSigCfg.positiveBinIdx; binIdx++)
            {
                meanBin = _amem8(&meanPtr[ind]);
                fftBin = _amem4(&fftPtr[binIdx]);
                Im = _ext(fftBin, 0, 16) - _loll(meanBin);
                Re = _ext(fftBin, 16, 16) - _hill(meanBin);
                _amem4(&fftPtr[binIdx]) = _pack2(Im, Re);
                ind++;
            }

            chirpInOffs = chirpPingPongId * (obj->numRxAntennas * obj->numRangeBins) + (obj->numRangeBins * rxAntIdx) + obj->numRangeBins + obj->calibDcRangeSigCfg.negativeBinIdx;
            fftPtr = (uint32_t *)&obj->fftOut1D[chirpInOffs];
            for (binIdx = 0; binIdx < -obj->calibDcRangeSigCfg.negativeBinIdx; binIdx++)
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
*    Interchirp processing. It is executed per chirp event, after ADC
*    buffer is filled with chirp samples.
*
*  @retval
*      Not Applicable.
*/
void MmwDemo_interChirpProcessing(DSS_DataPathObj *obj, uint32_t chirpPingPongId, uint8_t subframeIndx)
{
    uint32_t antIndx, waitingTime;
    volatile uint32_t startTime;
    volatile uint32_t startTime1;

    waitingTime = 0;
    startTime = Cycleprofiler_getTimeStamp();

    /* Kick off DMA to fetch data from ADC buffer for first channel */
    MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
        MRR_SF0_EDMA_CH_1D_IN_PING,
        subframeIndx);

    /* 1d fft for first antenna, followed by kicking off the DMA of fft output */
    for (antIndx = 0; antIndx < obj->numRxAntennas; antIndx++)
    {
        /* kick off DMA to fetch data for next antenna */
        if (antIndx < (obj->numRxAntennas - 1))
        {
            if (isPong(antIndx))
            {
                MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
                    MRR_SF0_EDMA_CH_1D_IN_PING,
                    subframeIndx);
            }
            else
            {
                MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
                    MRR_SF0_EDMA_CH_1D_IN_PONG,
                    subframeIndx);
            }
        }

        /* verify if DMA has completed for current antenna */
        startTime1 = Cycleprofiler_getTimeStamp();
        MmwDemo_dataPathWait1DInputData(obj, pingPongId(antIndx), subframeIndx);
        waitingTime += Cycleprofiler_getTimeStamp() - startTime1;
        
    
        mmwavelib_windowing16x16(
            (int16_t *)&obj->adcDataIn[pingPongId(antIndx) * obj->numRangeBins],
            (int16_t *)obj->window1D,
            obj->numAdcSamples);
        memset((void *)&obj->adcDataIn[pingPongId(antIndx) * obj->numRangeBins + obj->numAdcSamples],
            0, (obj->numRangeBins - obj->numAdcSamples) * sizeof(cmplx16ReIm_t));
        
        
        
        DSP_fft16x16(
            (int16_t *)obj->twiddle16x16_1D,
            obj->numRangeBins,
            (int16_t *)&obj->adcDataIn[pingPongId(antIndx) * obj->numRangeBins],
            (int16_t *)&obj->fftOut1D[chirpPingPongId * (obj->numRxAntennas * obj->numRangeBins) +
            (obj->numRangeBins * antIndx)]);
    }

    gCycleLog.interChirpProcessingTime += Cycleprofiler_getTimeStamp() - startTime - waitingTime;
    gCycleLog.interChirpWaitTime += waitingTime;
}

/**
*  @b Description
*  @n
*    Interframe processing. It is called from MmwDemo_dssDataPathProcessEvents
*    after all chirps of the frame have been received and 1D FFT processing on them
*    has been completed.
*
*  @retval
*      Not Applicable.
*/
void MmwDemo_interFrameProcessing(DSS_DataPathObj *obj, uint8_t subframeIndx)
{
    uint32_t rangeIdx, detIdx1, numDetObjPerCfar, numDetDopplerLine1D, numDetObj1D, numDetObj2D;
    volatile uint32_t startTime;
    volatile uint32_t startTimeWait;
    uint32_t waitingTime = 0;
    uint32_t pingPongIdx = 0;
    uint32_t dopplerLine, dopplerLineNext;
    startTime = Cycleprofiler_getTimeStamp();
       
    /* Trigger first DMA (Ping) to bring the 1DFFT data out of L3 to dstPingPong buffer  */
    MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
        MRR_SF0_EDMA_CH_2D_IN_PING,
        subframeIndx);

    /* Initialize the  variable that keeps track of the number of objects detected */
    numDetDopplerLine1D = 0;
    numDetObj1D = 0;
    MmwDemo_resetDopplerLines(&obj->detDopplerLines);
    
    for (rangeIdx = 0; rangeIdx < obj->numRangeBins; rangeIdx++)
    {
        
       
        /* Perform the 2nd dimension  FFT (doppler-FFT), compute the log2Abs, and provide the 
         * noncoherently added sum of the log2Abs across antennas as the output (sumAbs). */
        waitingTime += secondDimFFTandLog2Computation(obj, obj->sumAbs, CHECK_FOR_DET_MATRIX_TX, rangeIdx, &pingPongIdx);

        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            /* In the subframe for maximum velocity enancement, the second half of the chirps of the 
            * subframe consists of 'slow chirps', i.e., chirps with larger idle times compared to 
            * the first set. These have a different (lower) max-unambiguous-velocities, and its 
            * 2D-FFT output can be used (along with the 2D-FFT output of the 'fast-chirps' of the 
            * first half of the frame ) with the chinese remainder theorem (or CRT) to increase 
            * the max-unambiguous velocity. */
            waitingTime += secondDimFFTandLog2Computation(obj, obj->sumAbsSlowChirp, DO_NOT_CHECK_FOR_DET_MATRIX_TX, rangeIdx, &pingPongIdx);
        }

        /* doppler-CFAR-detecton on the current range gate.*/
        numDetObjPerCfar = cfarCa_SO_dBwrap_withSNR(
            obj->sumAbs,
            obj->cfarDetObjIndexBuf,
            obj->cfarDetObjSNR,
            obj->numDopplerBins,
            obj->cfarCfgDoppler.thresholdScale,
            obj->cfarCfgDoppler.noiseDivShift,
            obj->cfarCfgDoppler.guardLen,
            obj->cfarCfgDoppler.winLen);

        /* Reduce the detected objects to peaks. */
        numDetObjPerCfar = pruneToPeaks(obj->cfarDetObjIndexBuf, obj->cfarDetObjSNR, 
                                        numDetObjPerCfar, obj->sumAbs, obj->numDopplerBins);

        /* If the chirp design allows, perform the 'maximum velocity enhancement using dissimilar chirps and the chinese remainder theorem'. */
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            uint32_t detObj1DRawIdx;
            float interpOffset;
            float fastChirpVelIdxFlt, fastChirpVel;
            int16_t fastChirpVelIdx;
            
            /* Prune the list of detected objects to at most MAX_NUM_DET_PER_RANGE_GATE largest peaks. */
            numDetObjPerCfar = findKLargestPeaks(obj->cfarDetObjIndexBuf, obj->cfarDetObjSNR, 
                                                 numDetObjPerCfar, obj->sumAbs, obj->numDopplerBins, 
                                                 MAX_NUM_DET_PER_RANGE_GATE);
            
            /* Find the list of doppler gates to perform 2D CFAR on. */
            for (detIdx1 = 0; detIdx1 < numDetObjPerCfar; detIdx1++)
            {
                detObj1DRawIdx = rangeIdx*MAX_NUM_DET_PER_RANGE_GATE + detIdx1;
                obj->detObj1DRaw[detObj1DRawIdx].dopplerIdx = obj->cfarDetObjIndexBuf[detIdx1];
                obj->detObj1DRaw[detObj1DRawIdx].rangeIdx = rangeIdx;
                obj->detObj1DRaw[detObj1DRawIdx].dopplerSNRdB = obj->cfarDetObjSNR[detIdx1] >> obj->log2numVirtAnt;

                /* Estimate the velocity from the 'fast chirps'. Also, perform an interpolation 
                 * operation to get close to the true doppler (to avoid being bounded by the 
                 * doppler resolution) . */                
                interpOffset = quadraticInterpLog2ShortPeakLoc(obj->cfarDetObjIndexBuf, obj->numDopplerBins, detIdx1, (CFARTHRESHOLD_N_BIT_FRAC + obj->log2numVirtAnt));
                
                fastChirpVelIdx = (int16_t) obj->cfarDetObjIndexBuf[detIdx1];
                if (fastChirpVelIdx > (obj->numDopplerBins >> 1) - 1)
                {
                    fastChirpVelIdx -= obj->numDopplerBins;
                }
                
                fastChirpVelIdxFlt = ((float)fastChirpVelIdx) + interpOffset;
                fastChirpVel = fastChirpVelIdxFlt * ((float)obj->maxVelEnhStruct.velResolutionFastChirp);


                /* velocity disambiguation using chinese remainder theorem. */
                /* Note : 'disambiguateVel' irrevocably alters  the sumAbsSlowChirp buffer */
            /*    obj->detObj1DRaw[detObj1DRawIdx].velDisambFacValidity =
                            disambiguateVel(
                                obj->sumAbsSlowChirp,
                                fastChirpVel,
                                obj->sumAbs[obj->cfarDetObjIndexBuf[detIdx1]],
                                obj);
*/
                numDetObj1D++;
            }

            for (detIdx1 = numDetObjPerCfar; detIdx1 < MAX_NUM_DET_PER_RANGE_GATE; detIdx1++)
            {
                detObj1DRawIdx = ((rangeIdx*MAX_NUM_DET_PER_RANGE_GATE) + detIdx1);
                obj->detObj1DRaw[detObj1DRawIdx].velDisambFacValidity = -2;
            }
        }

        /* Decide which doppler 'gates' are to be subjected to the range-CFAR. We only need to do so 
         * if a detected object from the doppler-CFAR is detected at that 'doppler gate' . */
        if (numDetObjPerCfar > 0)
        {
            for (detIdx1 = 0; detIdx1 < numDetObjPerCfar; detIdx1++)
            {
                if (!MmwDemo_isSetDopplerLine(&obj->detDopplerLines, obj->cfarDetObjIndexBuf[detIdx1]))
                {
                    MmwDemo_setDopplerLine(&obj->detDopplerLines, obj->cfarDetObjIndexBuf[detIdx1]);
                    numDetDopplerLine1D++;
                }
            }
        }
        /* populate the pre-detection matrix */
        MmwDemo_startDmaTransfer(
            obj->edmaHandle[EDMA_INSTANCE_DSS],
            MRR_SF0_EDMA_CH_DET_MATRIX,
            subframeIndx);
       
    }

    startTimeWait = Cycleprofiler_getTimeStamp();
    MmwDemo_dataPathWaitTransDetMatrix(obj, subframeIndx);
    waitingTime += Cycleprofiler_getTimeStamp() - startTimeWait;
    
    /*
     * Perform CFAR detection along range lines. Only those doppler bins which were
     * detected in the earlier CFAR along doppler dimension are considered
     */
    if (numDetDopplerLine1D > 0)
    {
        uint8_t chId;
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_DET_MATRIX2;
        }
       /* else
        {
            chId = MRR_SF1_EDMA_CH_DET_MATRIX2;
        } */

        dopplerLine = MmwDemo_getDopplerLine(&obj->detDopplerLines);
        EDMAutil_triggerType3(obj->edmaHandle[EDMA_INSTANCE_DSS],
            /*(uint8_t *)(&obj->detMatrix[dopplerLine]),*/
            (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->detMatrix[dopplerLine]),
                SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
            (uint8_t *)(SOC_translateAddress((uint32_t)(&obj->sumAbsRange[0]),
                SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
            (uint8_t)chId,
            (uint8_t)MRR_EDMA_TRIGGER_ENABLE);
    }

    numDetObj2D = 0;
    for (detIdx1 = 0; detIdx1 < numDetDopplerLine1D; detIdx1++)
    {
        /* wait for DMA transfer of current range line to complete */
        startTimeWait = Cycleprofiler_getTimeStamp();
        MmwDemo_dataPathWaitTransDetMatrix2(obj, subframeIndx);
        waitingTime += Cycleprofiler_getTimeStamp() - startTimeWait;

        /* Trigger next DMA */
        if (detIdx1 < (numDetDopplerLine1D - 1))
        {
            uint8_t chId;
            dopplerLineNext = MmwDemo_getDopplerLine(&obj->detDopplerLines);

            if (subframeIndx == 0)
            {
                chId = MRR_SF0_EDMA_CH_DET_MATRIX2;
            }
            /* else
            {
                chId = MRR_SF1_EDMA_CH_DET_MATRIX2;
            } */

            EDMAutil_triggerType3(obj->edmaHandle[EDMA_INSTANCE_DSS],
                /*(uint8_t*)(&obj->detMatrix[dopplerLineNext]),*/
                (uint8_t*)(SOC_translateAddress(
                    (uint32_t)(&obj->detMatrix[dopplerLineNext]),
                    SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                (uint8_t*)(SOC_translateAddress(
                    (uint32_t)(&obj->sumAbsRange[((detIdx1 + 1) & 0x1) * obj->numRangeBins]),
                    SOC_TranslateAddr_Dir_TO_EDMA, NULL)),
                (uint8_t)chId,
                (uint8_t)MRR_EDMA_TRIGGER_ENABLE);
        }
        
        /* On the detected doppler line, use CFAR to find range peaks among numRangeBins samples.
         * Note : This is a modified version of the mmwavelib CFAR function. We were interested in 
         * the SNR (as computed by the CFAR) as well for the tracking algorithms */
        numDetObjPerCfar = cfarCadB_SO_withSNR(
            &obj->sumAbsRange[(detIdx1 & 0x1) * obj->numRangeBins],
            obj->cfarDetObjIndexBuf,
            obj->cfarDetObjSNR,
            obj->numRangeBins,
            obj->cfarCfgRange.thresholdScale,
            obj->cfarCfgRange.noiseDivShift,
            obj->cfarCfgRange.guardLen,
            obj->cfarCfgRange.winLen,
            obj->cfarCfgRange_minIndxToIgnoreHPF);

        if (numDetObjPerCfar > 0)
        {
            if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
            {
                /*  
                 * Since the point tracker works on strong unambiguous peaks, we 
                 * prune the list of objects further to generate targets that are
                 * peaks in the range dimension. 
                 * 
                 * Note that we've previously pruned the detected objects to be
                 * peaks in the doppler dimension. With this next step, the list of 
                 * objects are guaranteed to be peaks in both dimensions */
                 
                numDetObjPerCfar = pruneToPeaks(obj->cfarDetObjIndexBuf,
                                            obj->cfarDetObjSNR, 
                                            numDetObjPerCfar,
                                            &obj->sumAbsRange[(detIdx1 & 0x1) * obj->numRangeBins],
                                            obj->numRangeBins);

                
                numDetObj2D = findandPopulateIntersectionOfDetectedObjects(obj,
                                        numDetObjPerCfar, dopplerLine, numDetObj2D,
                                        &obj->sumAbsRange[(detIdx1 & 0x1) * obj->numRangeBins]);
            }
            else
            {
                if (detIdx1 != 0)
                {
                    /* Prune to only neighbours of peaks (or peaks). This increases the point cloud 
                     * density but helps avoid having too many detections around one object. If this
                     * condition wasn't added, there would be ~3 detections around every target, 
                     * corresponding to the peak, and the neighbours of the peak. 
                     *
                     * This is not performed for the zero doppler case because in case the car has 
                     * stopped at an intersection, and there are many cars around it, every detected 
                     * point counts. */
                    numDetObjPerCfar = pruneToPeaksOrNeighbourOfPeaks(obj->cfarDetObjIndexBuf,
                                                obj->cfarDetObjSNR, 
                                                numDetObjPerCfar,
                                                &obj->sumAbsRange[(detIdx1 & 0x1) * obj->numRangeBins],
                                                obj->numRangeBins);
                }
                numDetObj2D =  findandPopulateDetectedObjects(obj, numDetObjPerCfar, dopplerLine,
                                    numDetObj2D, &obj->sumAbsRange[(detIdx1 & 0x1) * obj->numRangeBins]);
            }
        }
        dopplerLine = dopplerLineNext;
    }

    if (obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        /* Peak grouping 
         * Another pruning step for the point cloud because we haven't made 
         * sure that the objects are peaks in doppler. */
        numDetObj2D = cfarPeakGroupingAlongDoppler( obj->detObj2DRaw, numDetObj2D, 
                                                    obj->detMatrix, obj->numRangeBins,
                                                    obj->numDopplerBins);
    }
    
    obj->numDetObjRaw = numDetObj2D;

    /* We would like to modify/prune the detection results to take care of the 
     * following issues. 
     * 1. remove objects less than minimum range or greater than maximum range.   
     * 2. Higher SNR/peakVal requirement for close-by objects - to avoid ground 
     *    clutter 
     */
    numDetObj2D = rangeBasedPruning(obj->detObj2D, obj->detObj2DRaw, 
                                    obj->SNRThresholds,obj->peakValThresholds,
                                    numDetObj2D, obj->numDopplerBins, obj->maxRange,
                                    obj->minRange);
                                            
    obj->numDetObj = numDetObj2D;
    
    /* Azimuth  Processing. */
    waitingTime += aziEleProcessing(obj, subframeIndx);
    
    /* Clustering. */
    clusteringDBscanRun(&(obj->dbScanInstance), obj, obj->numDetObj, &(obj->dbScanReport), obj->trackingInput);
    
    /* Tracking. */
    if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
    {
        uint32_t numTrackingInput;
        /* Remove tracking inputs with poor SNR, and at grazing angles. */        
        numTrackingInput = pruneTrackingInput(obj->trackingInput, obj->dbScanReport.numCluster);
        
        ekfRun(numTrackingInput, obj->trackingInput, obj, obj->trackerQvecList, FRAME_PERIODICITY_SEC);  
        
    }
    
    /* Associate clustering outputs */
    if (obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        float thresholdFlt  = (2 * obj->dbScanInstance.epsilon * obj->dbScanInstance.fixedPointScale);
        int32_t threshold = _spint(thresholdFlt*thresholdFlt);
        int32_t vFactorFixed = (int32_t)(obj->dbScanInstance.vFactor * obj->dbScanInstance.fixedPointScale);
        
        associateClustering(&(obj->dbScanReport), obj->dbScanState, obj->dbScanInstance.maxClusters, threshold, vFactorFixed);
    }
    
    /* Create the smallest meaningful array for the data that being sent out to the PC because 
     * the UART rate is quite low. */
    populateOutputs(obj);
    
    gCycleLog.interFrameProcessingTime += Cycleprofiler_getTimeStamp() - startTime - waitingTime;
    gCycleLog.interFrameWaitTime += waitingTime;
}

/**
*  @b Description
*  @n
*    Chirp processing. It is called from MmwDemo_dssDataPathProcessEvents. It
*    is executed per chirp. 
*
*    The range FFT output is transferred in a transpose manner to L3 using an EDMA. This is done
*    so that the 2nd FFT data can be pulled out using a non-transpose EDMA (which is more efficient)
*
*    The EDMA transfer requires a destination offset (radarCubeOffset) that is proportional with 
*    the chirp number. 
*    
*    For the MAX_VEL_ENH chirp, there are two chirp types (fast and slow), they are 
*    stored consecutively ( for e.g. chirp 1 of the fast chirp is directly followed by chirp 1
*    of the slow chirp. 
*
*  @retval
*      Not Applicable.
*/
//uint32_t logRadarOffset[128];
//uint32_t idx =0;
//uint32_t logChirpcnt[128];
void MmwDemo_processChirp(DSS_DataPathObj *obj, uint8_t subframeIndx)
{
    volatile uint32_t startTime;
    uint32_t radarCubeOffset;
    uint8_t chId;
        
    /** 1. Book keeping. */
    startTime = Cycleprofiler_getTimeStamp();

    if (obj->chirpCount > 1) //verify if ping(or pong) buffer is free for odd(or even) chirps
    {
        MmwDemo_dataPathWait1DOutputData(obj, pingPongId(obj->chirpCount), subframeIndx);
    }
    gCycleLog.interChirpWaitTime += Cycleprofiler_getTimeStamp() - startTime;

    /** 2.  Range processing. */
    MmwDemo_interChirpProcessing(obj, pingPongId(obj->chirpCount), subframeIndx);

    /* Modify destination address in Param set and DMA for sending 1DFFT output (for all antennas) to L3  */
    if (isPong(obj->chirpCount))
    {
        /* select the appropriate channel based on the index of the subframe. */
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_OUT_PONG;
        }
        /* else
        {
            chId = MRR_SF1_EDMA_CH_1D_OUT_PONG;
        } */
        
        radarCubeOffset = (obj->numDopplerBins * obj->numRxAntennas * (obj->txAntennaCount)) 
                            + obj->dopplerBinCount
                            + (obj->numDopplerBins * obj->numRxAntennas * obj->numTxAntennas * obj->chirpTypeCount);
        EDMAutil_triggerType3(
            obj->edmaHandle[EDMA_INSTANCE_DSS],
            (uint8_t *)NULL,
            (uint8_t *)(&obj->radarCube[radarCubeOffset]),
            (uint8_t)chId,
            (uint8_t)MRR_EDMA_TRIGGER_ENABLE);
    }
    else
    {
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_1D_OUT_PING;
        }
        /* else
        {
            chId = MRR_SF1_EDMA_CH_1D_OUT_PING;
        }*/
        radarCubeOffset = (obj->numDopplerBins * obj->numRxAntennas * (obj->txAntennaCount)) 
                            + obj->dopplerBinCount
                            + (obj->numDopplerBins * obj->numRxAntennas * obj->numTxAntennas * obj->chirpTypeCount);

        EDMAutil_triggerType3(
            obj->edmaHandle[EDMA_INSTANCE_DSS],
            (uint8_t *)NULL,
            (uint8_t *)(&obj->radarCube[radarCubeOffset]),
            (uint8_t)chId,
            (uint8_t)MRR_EDMA_TRIGGER_ENABLE);
    }

    //logRadarOffset[idx] = radarCubeOffset;
    //
    //logChirpcnt[idx] = obj->chirpCount;
    //idx++;
     
     
    obj->chirpCount++;
    obj->txAntennaCount++;
    if (obj->txAntennaCount == obj->numTxAntennas)
    {
        obj->txAntennaCount = 0;
        obj->dopplerBinCount++;
        if (obj->dopplerBinCount == obj->numDopplerBins)
        {
            if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
            {    
                obj->chirpTypeCount++;
                obj->dopplerBinCount = 0;
                /* if (obj->chirpTypeCount == SUBFRAME_MRR_NUM_CHIRPTYPES)
                {
                    obj->chirpTypeCount = 0;
                    obj->chirpCount = 0;
                } */
            }
//            else
            {
               obj->chirpTypeCount = 0; 
               obj->dopplerBinCount = 0;
               obj->chirpCount = 0;
            }
        }
    }
}

/**
*  @b Description
*  @n
*  Wait for transfer of data corresponding to the last 2 chirps (ping/pong)
*  to the radarCube matrix before starting interframe processing.
*  @retval
*      Not Applicable.
*/
void MmwDemo_waitEndOfChirps(DSS_DataPathObj *obj, uint8_t subframeIndx)
{
    volatile uint32_t startTime;

    startTime = Cycleprofiler_getTimeStamp();
    /* Wait for transfer of data corresponding to last 2 chirps (ping/pong) */
    MmwDemo_dataPathWait1DOutputData(obj, 0, subframeIndx);
    MmwDemo_dataPathWait1DOutputData(obj, 1, subframeIndx);

    gCycleLog.interChirpWaitTime += Cycleprofiler_getTimeStamp() - startTime;
}
void calc_cmplx_exp(cmplx16ImRe_t *dftSinCos, float i, uint32_t dftLen)
{
    int32_t itemp;
    float temp;
	temp = ONE_Q15 * -sin(2 * PI_*i / dftLen);
	itemp = (int32_t)ROUND(temp);

	if (itemp >= ONE_Q15)
	{
		itemp = ONE_Q15 - 1;
	}
	dftSinCos->imag = itemp;

	temp = ONE_Q15 * cos(2 * PI_*i / dftLen);
	itemp = (int32_t)ROUND(temp);

	if (itemp >= ONE_Q15)
	{
		itemp = ONE_Q15 - 1;
	}
	dftSinCos->real = itemp;
}

/**
*  @b Description
*  @n
*  Generate SIN/COS table in Q15 (SIN to even int16 location, COS to
*  odd int16 location. Also generates Sin/Cos at half the bin value
*  The table is generated as
*  T[i]=cos[2*pi*i/N] - 1j*sin[2*pi*i/N] for i=0,...,N where N is dftLen
*  The half bn value is calculated as:
*  TH = cos[2*pi*0.5/N] - 1j*sin[2*pi*0.5/N]
*
*  @param[out]    dftSinCosTable Array with generated Sin Cos table
*  @param[out]    dftHalfBinVal  Sin/Cos value at half the bin
*  @param[in]     dftLen Length of the DFT
*
*  @retval
*      Not Applicable.
*/
void MmwDemo_genDftSinCosTable(cmplx16ImRe_t *dftSinCosTable,
    cmplx16ImRe_t *dftHalfBinVal,
	cmplx16ImRe_t *dftThirdBinVal,
	cmplx16ImRe_t *dftTwoThirdBinVal,
    uint32_t dftLen)
{
    uint32_t i;
   
	for (i = 0; i < dftLen; i++)
	{
		calc_cmplx_exp(&dftSinCosTable[i], (float)i, dftLen);
	}

	/*Calculate half bin value*/
	calc_cmplx_exp(dftHalfBinVal, 0.5f, dftLen);
	/*Calculate at a third bin value*/
	calc_cmplx_exp(dftThirdBinVal, 0.33333333333f, dftLen);
	/*Calculate at two third bin value*/
	calc_cmplx_exp(dftTwoThirdBinVal, 0.66666666666f, dftLen);
}



/**
*  @brief
*  This is a callback function for EDMA  errors.
*
*  @param[in] handle:       EDMA Handle.
*  @param[in] errorInfo:    EDMA error info.
*
*  @retval void.
*/
void DSS_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    dssAssert(0);
}

/**
*  @brief
*   This is a callback function for EDMA TC errors. 
*  
*  @param[in] handle:       EDMA Handle.
*  @param[in] errorInfo:    EDMA TC error info.
*
*  @retval void.
*/
void DSS_edmaTransferControllerErrorCallbackFxn(EDMA_Handle handle, EDMA_transferControllerErrorInfo_t *errorInfo)
{
    DSS_DataPathObj * dataPathObj;
    /* Copy the error into the output structure (for debug) */
    dataPathObj = &gMCB.dataPathObj[gMCB.subframeIndx];
    dataPathObj->EDMA_transferControllerErrorInfo = *errorInfo;
    dssAssert(0);
}


/**
*  @brief
*  This function initializes some of the states (counters) used for 1D processing.
*  
*  @param[in]   obj:     data path object.
*  @retval      int32_t: success(0)/failure(-1).
*/
void DSS_dataPathInit1Dstate(DSS_DataPathObj *obj)
{
    int8_t subframeIndx = 0;
    for (subframeIndx = 0; subframeIndx < NUM_SUBFRAMES; subframeIndx++, obj++)
    {
        obj->chirpCount = 0;
        obj->dopplerBinCount = 0;
        obj->txAntennaCount = 0;
        obj->chirpTypeCount = 0;
    }

    /* reset profiling logs before start of frame */
    memset((void *)&gCycleLog, 0, sizeof(cycleLog_t));
}

/**
*  @brief
*  This function copies the EDMA handles to all of the remaining data path objects.
*  
*  @param[in]   obj:     data path object.
*  @retval      int32_t: success(0)/failure(-1).
*/
int32_t DSS_dataPathInitEdma(DSS_DataPathObj *obj)
{
    uint8_t             numInstances;
    int32_t             errorCode;
    EDMA_Handle         handle;
    EDMA_errorConfig_t  errorConfig;
    uint32_t            instanceId;
    EDMA_instanceInfo_t instanceInfo;

    numInstances = EDMA_getNumInstances();

    /* Initialize the edma instance to be tested */
    for (instanceId = 0; instanceId < numInstances; instanceId++)
    {
        EDMA_init(instanceId);

        handle = EDMA_open(instanceId, &errorCode, &instanceInfo);
        if (handle == NULL)
        {
            System_printf("[DSS] \t [Error][EDMA] Unable to open the edma Instance, erorCode = %d\n", errorCode);
            return -1;
        }
        obj->edmaHandle[instanceId] = handle;

        errorConfig.isConfigAllEventQueues = true;
        errorConfig.isConfigAllTransferControllers = true;
        errorConfig.isEventQueueThresholdingEnabled = true;
        errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
        errorConfig.isEnableAllTransferControllerErrors = true;
        errorConfig.callbackFxn = DSS_edmaErrorCallbackFxn;
        errorConfig.transferControllerCallbackFxn = DSS_edmaTransferControllerErrorCallbackFxn;
        if ((errorCode = EDMA_configErrorMonitoring(handle, &errorConfig)) != EDMA_NO_ERROR)
        {
            System_printf("[DSS] \t [Debug][EDMA] EDMA_configErrorMonitoring() failed with errorCode = %d\n", errorCode);
            return -1;
        }
    }
    return 0;
}

/**
*  @b Description
*  @n
*   This function copies the EDMA handles to all of the remaining data path objects.  
*  
*  @param[in,out] obj             data path object.
*
*  @retval success.
*/
int32_t MmwDemo_dataPathCopyEdmaHandle(DSS_DataPathObj *objOutput, DSS_DataPathObj *objInput)
{
    uint8_t numInstances;
    uint32_t instanceId;

    numInstances = EDMA_getNumInstances();

    /* Initialize the edma instance to be tested */
    for (instanceId = 0; instanceId < numInstances; instanceId++)
    {
        objOutput->edmaHandle[instanceId] = objInput->edmaHandle[instanceId];
    }
    return 0;
}

/**
*  @b Description
*  @n
*   This function holds the last remaining 'printf' in the entire MRR, and prints the space 
*   remaining in the global heap. 
*  
*  @param[in,out] obj             data path object.
*
*  @retval na.
*/
void MmwDemo_printHeapStats(char *name, uint32_t heapUsed, uint32_t heapSize)
{
    System_printf("Heap %s : size %d (0x%x), free %d (0x%x)\n", name, heapSize, heapSize, heapSize - heapUsed, heapSize - heapUsed);
}

/**
*  @b Description
*  @n
*   This function assigns memory locations to the different data buffers used in the MRR design. 
*
*   Processing radar signals require a large number of scratch buffers for each step each
*   of the processing stages (be it 1D-FFT, 2D-FFT, 3D-FFT, detection, angle estimation etc.
*   However, since these stages occur serially, the memory assigned to a scratch buffer
*   used in a previous stage can be re-used in the current stage. The Macro MMW_ALLOC_BUF
*   in the following code allows specifying the start addresses such that the memory
*   locations can be overlaid for efficient memory utilization.
*
*   In the MRR TI Design, there are two sub-frames per frame, and both sub-frames are processed
*   separately. Therefore, nearly every scratch buffer memory location can be overlaid
*   between the two. The allocation code is called twice to allocate memory for both
*   sub-frames.
*
*   Certain memory locations are only necessary for a given processing path and are left
*   unassigned for different programming paths. 
*
*   Memory locations that correspond to the windowing functions, and  twiddle factors, and
*   estimated mean chirp need to be saved between sub-frames and as such cannot be overlaid.
*  
*  @param[in,out] obj             data path object.
*
*  @retval na.
*/
#define SOC_MAX_NUM_RX_ANTENNAS 4
#define SOC_MAX_NUM_TX_ANTENNAS 3
void DSS_dataPathConfigBuffers(DSS_DataPathObj *objIn, uint32_t adcBufAddress)
{

    volatile DSS_DataPathObj *obj = &objIn[0];

    volatile uint32_t prev_end;
    volatile uint32_t l2HeapEndLocationForSubframe[NUM_SUBFRAMES];

    /* below defines for debugging purposes, do not remove as overlays can be hard to debug */

#define ALIGN(x,a)  (((x)+((a)-1))&~((a)-1))

#define MMW_ALLOC_BUF(name, nameType, startAddr, alignment, size) \
                obj->name = (nameType *) ALIGN(startAddr, alignment); \
                uint32_t name##_end = (uint32_t)obj->name + (size) * sizeof(nameType);

    uint32_t subframeIndx;
    uint32_t numChirpTypes = 1;
    volatile uint32_t heapUsed;
    uint32_t heapL1start = (uint32_t)&gMmwL1[0];
    uint32_t heapL2start = (uint32_t)&gMmwL2[0];
    uint32_t heapL3start = (uint32_t)&gMmwL3[0];

    uint32_t azimuthMagSqrLen;
    uint32_t azimuthInLen;
    volatile uint32_t size;
    /* L3 is overlaid with one-time only accessed code. Although heap is not
    required to be initialized to 0, it may help during debugging when viewing memory
    in CCS */
    memset((void *)heapL3start, 0, sizeof(gMmwL3));

    for (subframeIndx = 0; subframeIndx < NUM_SUBFRAMES; subframeIndx++, obj++)
    {
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            numChirpTypes = 2; 
        }
        else
        {
            numChirpTypes = 1;
        }
        

        /* L1 allocation

        Buffers are overlaid in the following order. Notation "|" indicates parallel
        and "+" means cascade

        { 1D
            (adcDataIn)
        } |
        { 2D
            (dstPingPong +  fftOut2D) +
            (windowingBuf2D | log2Abs) + sumAbs  + sumAbsSlowChirp (only for subframe MAX_VEL_ENH)
             + detObj1DRaw (must_be_after detObj2DRaw, only for subframe MAX_VEL_ENH)
        } |
        { 3D
            (detObj2DRaw + 
                ((azimuthIn (must be at least beyond dstPingPong) 
                    + azimuthOut + azimuthMagSqr) )
        } |
        { Clustering (scratch pad)
          + 
          Tracking (scratch pad)
        } |
        {
          Final outputs. 
        }
        */
        /* 1D FFT */
        MMW_ALLOC_BUF(adcDataIn, cmplx16ReIm_t,
            heapL1start, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            2 * obj->numRangeBins);
        memset((void *)obj->adcDataIn, 0, 2 * obj->numRangeBins * sizeof(cmplx16ReIm_t));

        /* 2D FFT. */
        MMW_ALLOC_BUF(dstPingPong, cmplx16ReIm_t,
            heapL1start, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            2 * obj->numDopplerBins);

        MMW_ALLOC_BUF(fftOut2D, cmplx32ReIm_t,
            dstPingPong_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numDopplerBins);

        MMW_ALLOC_BUF(windowingBuf2D, cmplx32ReIm_t,
            fftOut2D_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numDopplerBins);

        MMW_ALLOC_BUF(log2Abs, uint16_t,
            fftOut2D_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numDopplerBins);

        MMW_ALLOC_BUF(sumAbs, uint16_t,
            MAX(log2Abs_end, windowingBuf2D_end), MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            2 * obj->numDopplerBins);
            
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            MMW_ALLOC_BUF(sumAbsSlowChirp, uint16_t,
                MAX(sumAbs_end, windowingBuf2D_end), MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
                2 * obj->numDopplerBins);
                
            sumAbs_end = sumAbsSlowChirp_end;
        }
        
        /* Detected objects (2D). */
        MMW_ALLOC_BUF(detObj2DRaw, MmwDemo_objRaw2D_t,
            heapL1start, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            obj->maxNumObj2DRaw);

        /* 3D FFT. */
        if (obj->processingPath == POINT_CLOUD_PROCESSING)
        {
            /* For the 'point cloud processing' we use 2-tx mimo and in order to have 2x velocity 
             * disambiguation, we perform the azimuth-FFT twice (once with a 180 phase offset across 
             * the two Tx.So we need extra space to save input to azimuth FFT for second call */
            azimuthInLen = obj->numAngleBins + obj->numVirtualAntAzim;
        }
        else
        {
            azimuthInLen = obj->numAngleBins;
        }
        
        MMW_ALLOC_BUF(antInp, cmplx32ReIm_t,
            MAX(detObj2DRaw_end, dstPingPong_end), MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numVirtualAntennas );
        
        MMW_ALLOC_BUF(azimuthIn, cmplx32ReIm_t,
            antInp_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            azimuthInLen);

        MMW_ALLOC_BUF(azimuthOut, cmplx32ReIm_t,
            azimuthIn_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN, obj->numAngleBins);
    
        MMW_ALLOC_BUF(elevationIn, cmplx32ReIm_t,
            azimuthOut_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            azimuthInLen);

        MMW_ALLOC_BUF(elevationOut, cmplx32ReIm_t,
            elevationIn_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN, obj->numAngleBins);
            
        if (obj->processingPath == POINT_CLOUD_PROCESSING)
        {
            /* 2 sets for velocity disambiguation, see above comment. */
            azimuthMagSqrLen = obj->numAngleBins * 2;  
        }
        else
        {
            azimuthMagSqrLen = obj->numAngleBins;
        }
        MMW_ALLOC_BUF(azimuthMagSqr, float, elevationOut_end, sizeof(float), azimuthMagSqrLen);

        /* Detected objects (1D). */
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            MMW_ALLOC_BUF(detObj1DRaw, MmwDemo_objRaw1D_t,
            MAX(detObj2DRaw_end, sumAbs_end), MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            (obj->numRangeBins * MAX_NUM_DET_PER_RANGE_GATE));
            detObj2DRaw_end = detObj1DRaw_end;
            sumAbs_end = detObj1DRaw_end;
        }

        /* Clustering. */
        MMW_ALLOC_BUF(dBscanScratchPad, uint8_t,
            heapL1start, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            MRR_MAX_OBJ_OUT * 4);
        
        MMW_ALLOC_BUF(dbscanOutputDataIndexArray, uint16_t,
            dBscanScratchPad_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            MRR_MAX_OBJ_OUT);
        
        size = obj->dbScanInstance.maxClusters;
        MMW_ALLOC_BUF(dbscanOutputDataReport, clusteringDBscanReport_t,
            dbscanOutputDataIndexArray_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            size);

        /* Tracking buffers are only necessary for the long range subframe. */    
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            size = obj->dbScanInstance.maxClusters;
            MMW_ALLOC_BUF(trackingInput, trackingInputReport_t,
                dbscanOutputDataReport_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
                size);
            
            MMW_ALLOC_BUF(trackerScratchPadFlt, float,
                trackingInput_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
                TRACKER_SCRATCHPAD_FLT_SIZE);
            
            MMW_ALLOC_BUF(trackerScratchPadShort, int16_t,
                trackerScratchPadFlt_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
                TRACKER_SCRATCHPAD_SHORT_SIZE);
            dbscanOutputDataReport_end = trackerScratchPadShort_end;
        }
        
        MMW_ALLOC_BUF(detObjFinal, MmwDemo_detectedObjForTx,
            heapL1start, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            MRR_MAX_OBJ_OUT);
        
        size = obj->dbScanInstance.maxClusters;
        MMW_ALLOC_BUF(clusterOpFinal, clusteringDBscanReportForTx,
            detObjFinal_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            size);

        size = obj->trackerInstance.maxTrackers;
        MMW_ALLOC_BUF(trackerOpFinal, trackingReportForTx, clusterOpFinal_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN, size);
  
        size = obj->parkingAssistNumBins;
        MMW_ALLOC_BUF(parkingAssistBins, uint16_t, trackerOpFinal_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN, size);
  
        prev_end = MAX(MAX(MAX(sumAbs_end, adcDataIn_end),azimuthMagSqr_end),detObj2DRaw_end); 
        prev_end = MAX(MAX(prev_end,dbscanOutputDataReport_end), parkingAssistBins_end);
        heapUsed = prev_end - heapL1start;
        //MmwDemo_printHeapStats("L1", heapUsed, MMW_L1_HEAP_SIZE);
        dssAssert(heapUsed <= MMW_L1_HEAP_SIZE);
        
        MmwDemo_printHeapStats("L1", heapUsed, MMW_L1_HEAP_SIZE);
        
        /* L2 allocation (part 1)
           The L2 hallocation is done in two parts, the first part consists of memory buffers that can be
           shared between the two subframes. The 2nd part consists of memory buffers that hold state
           information and constants (like the twiddle factors, or the windowing array). These 
           cannot be shared (or overlaid in any way).
           
           The last occupied memory of L2 after the allocation of the first part is stored in  
           the array 'l2HeapEndLocationForSubframe' for each subframe, and is used to compute the 
           starting address for the 2nd part of L2 allocation. 
           
           The allocations are : 
            {
                {
                    { 1D
                    (fftOut1D)
                    } |
                    { 2D + 3D
                    (cfarDetObjIndexBuf + detDopplerLines.dopplerLineMask) + sumAbsRange + detMatrix + detObj2D
                    }
                }  
            }
        
        */

        MMW_ALLOC_BUF(fftOut1D, cmplx16ReIm_t,
            heapL2start, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            2 * obj->numRxAntennas * obj->numRangeBins);

        MMW_ALLOC_BUF(cfarDetObjIndexBuf, uint16_t,
            heapL2start, sizeof(uint16_t),
            MAX(obj->numRangeBins, obj->numDopplerBins));

        MMW_ALLOC_BUF(cfarDetObjSNR,    uint16_t,
            cfarDetObjIndexBuf_end, sizeof(uint16_t),
            MAX(obj->numRangeBins, obj->numDopplerBins));


        /* Expansion of below macro (macro cannot be used due to x.y type reference)
            MMW_ALLOC_BUF(detDopplerLines.dopplerLineMask, uint32_t,
            cfarDetObjIndexBuf_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            MAX((obj->numDopplerBins>>5),1));
        */
        obj->detDopplerLines.dopplerLineMask = (uint32_t *)ALIGN(cfarDetObjSNR_end,
            MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN);
        uint32_t detDopplerLines_dopplerLineMask_end = (uint32_t)obj->detDopplerLines.dopplerLineMask +
            MAX((obj->numDopplerBins >> 5), 1) * sizeof(uint32_t); /* should be ceil */

        obj->detDopplerLines.dopplerLineMaskLen = MAX((obj->numDopplerBins >> 5), 1);

        MMW_ALLOC_BUF(sumAbsRange, uint16_t,
            detDopplerLines_dopplerLineMask_end, sizeof(uint16_t),
            2 * obj->numRangeBins);

        MMW_ALLOC_BUF(detMatrix, uint16_t,
            sumAbsRange_end, sizeof(uint16_t),
            obj->numRangeBins * obj->numDopplerBins);
            
        MMW_ALLOC_BUF(detObj2D, MmwDemo_detectedObjActual,
            detMatrix_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            MRR_MAX_OBJ_OUT);    
        
        
        l2HeapEndLocationForSubframe[subframeIndx] = MAX(fftOut1D_end, detObj2D_end);

        /* L3 allocation:
            radarCube +
            detMatrix
        */
        obj->ADCdataBuf = (cmplx16ReIm_t *)adcBufAddress;

        MMW_ALLOC_BUF(radarCube, cmplx16ReIm_t,
            heapL3start, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numRangeBins * obj->numDopplerBins * obj->numRxAntennas * obj->numTxAntennas * numChirpTypes);
        heapUsed = radarCube_end - heapL3start;
        MmwDemo_printHeapStats("L3", heapUsed, sizeof(gMmwL3));
        
        dssAssert(heapUsed <= sizeof(gMmwL3));
    }

    volatile uint32_t prevL2End = 0; 
    
    /* Find the last occupied memory of the L2, for the subsequent assignments. */
    for (subframeIndx = 0; subframeIndx < NUM_SUBFRAMES; subframeIndx ++)
    {
        if (prevL2End < l2HeapEndLocationForSubframe[subframeIndx])
        {
            prevL2End =  l2HeapEndLocationForSubframe[subframeIndx];
        }
    }
    
    /*   L2 allocation (part 2)
     *   These allocations are static, and are used as long as the radar is alive. 
     *   They correspond to constants for the FFT and the tracking 'state' and the 
     *   clustering 'state' 
    {
        twiddle16x16_1D +
        window1D +
        twiddle16x32_2D +
        window2D +
        azimuthTwiddle16x32 +
        azimuthModCoefs + 
        trackerState + trackerQvecList +  
        dbScanState + parkingAssistBinsState + parkingAssistBinsStateCnt + rxChPhaseComp
    }
    */
    obj = &objIn[0];    
    for (subframeIndx = 0; subframeIndx < NUM_SUBFRAMES; subframeIndx++, obj++)
    {

        MMW_ALLOC_BUF(twiddle16x16_1D, cmplx16ReIm_t,
            prevL2End, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numRangeBins);
        
        MMW_ALLOC_BUF(window1D, int16_t,
            twiddle16x16_1D_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numAdcSamples / 2);

        MMW_ALLOC_BUF(twiddle32x32_2D, cmplx32ReIm_t,
            window1D_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numDopplerBins);

        MMW_ALLOC_BUF(window2D, int32_t,
            twiddle32x32_2D_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numDopplerBins / 2);

        MMW_ALLOC_BUF(azimuthTwiddle32x32, cmplx32ReIm_t,
            window2D_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numAngleBins);

        MMW_ALLOC_BUF(azimuthModCoefs, cmplx16ImRe_t,
            azimuthTwiddle32x32_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numDopplerBins);

        MMW_ALLOC_BUF(dcRangeSigMean, cmplx32ImRe_t,
            azimuthModCoefs_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            SOC_MAX_NUM_TX_ANTENNAS * SOC_MAX_NUM_RX_ANTENNAS * DC_RANGE_SIGNATURE_COMP_MAX_BIN_SIZE);
        
        MMW_ALLOC_BUF(rxChPhaseComp, cmplx16ImRe_t,
            dcRangeSigMean_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            obj->numTxAntennas*obj->numRxAntennas);      
         
        if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
        {
            /* If the tracker is being used assign memory for that. */              
            size = obj->trackerInstance.maxTrackers;
            MMW_ALLOC_BUF(trackerState, KFstate_t,
            rxChPhaseComp_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            size);
            
            MMW_ALLOC_BUF(trackerQvecList, float,
            trackerState_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            3*N_STATES);
            
            prevL2End = trackerQvecList_end;
        }
        else if (obj->processingPath == POINT_CLOUD_PROCESSING)
        {
            /* For the point cloud processing, we keep a record of 
             * the clusters, and associate them between frames. */
            MMW_ALLOC_BUF(dbScanState, clusteringDBscanReport_t,
            rxChPhaseComp_end, MMWDEMO_MEMORY_ALLOC_MAX_STRUCT_ALIGN,
            obj->dbScanInstance.maxClusters);
        
            size = obj->parkingAssistNumBins;
            MMW_ALLOC_BUF(parkingAssistBinsState, uint16_t,
            dbScanState_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            size);
            
            MMW_ALLOC_BUF(parkingAssistBinsStateCnt, uint16_t,
            parkingAssistBinsState_end, MMWDEMO_MEMORY_ALLOC_DOUBLE_WORD_ALIGN,
            size);
          
            
            prevL2End = parkingAssistBinsStateCnt_end;
        }
        else
        {
            dssAssert(0);
        }
               
           
        heapUsed = prevL2End - heapL2start;
        dssAssert(heapUsed <= MMW_L2_HEAP_SIZE);
        
        MmwDemo_printHeapStats("L2", heapUsed, MMW_L2_HEAP_SIZE);
        
    }
}

/**
*  @b Description
*  @n
*      Function to populate the twiddle factors for FFTS needed for the data path object. 
*
*  @param[in,out] obj             data path object.
*
*  @retval waitingTime.
*/
void DSS_dataPathConfigFFTs(DSS_DataPathObj *obj)
{
    MmwDemo_genWindow((void *)obj->window1D,
        FFT_WINDOW_INT16,
        obj->numAdcSamples,
        obj->numAdcSamples / 2,
        ONE_Q15,
        MMW_WIN_HANNING_RECT);

    MmwDemo_genWindow((void *)obj->window2D,
        FFT_WINDOW_INT32,
        obj->numDopplerBins,
        obj->numDopplerBins / 2,
        ONE_Q19,
        MMW_WIN_HAMMING);

    /* Generate twiddle factors for 1D FFT. */
    gen_twiddle_fft16x16((int16_t *)obj->twiddle16x16_1D, obj->numRangeBins);

    /* Generate twiddle factors for 2D FFT */
    gen_twiddle_fft32x32((int32_t *)obj->twiddle32x32_2D, obj->numDopplerBins, 2147483647.5);

    /* Generate twiddle factors for azimuth FFT */
    gen_twiddle_fft32x32((int32_t *)obj->azimuthTwiddle32x32, obj->numAngleBins, 2147483647.5);

    /* Generate SIN/COS table for single point DFT */
    MmwDemo_genDftSinCosTable(obj->azimuthModCoefs,
        &obj->azimuthModCoefsHalfBin,
        &obj->azimuthModCoefsThirdBin,
        &obj->azimuthModCoefsTwoThirdBin,
        obj->numDopplerBins);
}

/**
*  @b Description
*  @n
*      Function to generate a single FFT window sample.
*
*  @param[out] win             Pointer to calculated window samples.
*  @param[in]  windowDatumType Window samples data format. For windowDatumType = @ref FFT_WINDOW_INT16,
*              the samples format is int16_t. For windowDatumType = @ref FFT_WINDOW_INT32,
*              the samples format is int32_t.
*  @param[in]  winLen          Nominal window length
*  @param[in]  winGenLen       Number of generated samples
*  @param[in]  oneQformat      Q format of samples, oneQformat is the value of
*                              one in the desired format.
*  @param[in]  winType         Type of window, one of @ref MMW_WIN_BLACKMAN, @ref MMW_WIN_HANNING,
*              or @ref MMW_WIN_RECT.
*  @retval none.
*/
void MmwDemo_genWindow(void *win,
    uint32_t windowDatumType,
    uint32_t winLen,
    uint32_t winGenLen,
    int32_t oneQformat,
    uint32_t winType)
{
    uint32_t winIndx;
    int32_t winVal;
    int16_t * win16 = (int16_t *)win;
    int32_t * win32 = (int32_t *)win;

    float phi = 2 * PI_ / ((float)winLen - 1);

    if (winType == MMW_WIN_BLACKMAN)
    {
        //Blackman window
        float a0 = 0.42;
        float a1 = 0.5;
        float a2 = 0.08;
        for (winIndx = 0; winIndx < winGenLen; winIndx++)
        {
            winVal = (int32_t)((oneQformat * (a0 - a1*cos(phi * winIndx) +
                a2*cos(2 * phi * winIndx))) + 0.5);
            if (winVal >= oneQformat)
            {
                winVal = oneQformat - 1;
            }
            switch (windowDatumType)
            {
            case FFT_WINDOW_INT16:
                win16[winIndx] = (int16_t)winVal;
                break;
            case FFT_WINDOW_INT32:
                win32[winIndx] = (int32_t)winVal;
                break;
            }

        }
    }
    else if (winType == MMW_WIN_HAMMING)
    {
        //Hanning window
        for (winIndx = 0; winIndx < winGenLen; winIndx++)
        {
            winVal = (int32_t)((oneQformat *  (0.53836 - (0.46164*cos(phi * winIndx)))) + 0.5);
            
            if (winVal >= oneQformat)
            {
                winVal = oneQformat - 1;
            }
            
            switch (windowDatumType)
            {
            case FFT_WINDOW_INT16:
                win16[winIndx] = (int16_t)winVal;
                break;
            case FFT_WINDOW_INT32:
                win32[winIndx] = (int32_t)winVal;
                break;
            }
        }
    }
    else if (winType == MMW_WIN_RECT)
    {
        //Rectangular window
        for (winIndx = 0; winIndx < winGenLen; winIndx++)
        {
            switch (windowDatumType)
            {
            case FFT_WINDOW_INT16:
                win16[winIndx] = (int16_t)(oneQformat - 1);
                break;
            case FFT_WINDOW_INT32:
                win32[winIndx] = (int32_t)(oneQformat - 1);
                break;
            }
        }
    }
    else if (winType == MMW_WIN_HANNING_RECT)
    {
        
        phi = 2 * PI_ / ((float)(winLen/8) - 1);

        //Rectangular window
        for (winIndx = 0; winIndx < winGenLen; winIndx++)
        {
            if (winIndx <= winLen/16) 
            {
                winVal = (int32_t)((oneQformat * 0.5* (1 - cos(phi * winIndx))) + 0.5);
            }
            else
            {
                winVal = oneQformat - 1;
            }
            
            if (winVal >= oneQformat)
            {
                winVal = oneQformat - 1;
            }
        
            switch (windowDatumType)
            {
            case FFT_WINDOW_INT16:
                win16[winIndx] = (int16_t)(winVal);
                break;
            case FFT_WINDOW_INT32:
                win32[winIndx] = (int32_t)(winVal);
                break;
            }
        }
    }
}


/**
*  @b Description
*  @n
*      Function to perform 2D-FFT on all Rxs corresponding to one range gatewindow sample.
*      Following the FFT it computes the Log2 Abs and optionally stores it in detMatrix.
*
*  @param[in,out]   obj                 Data path object.
*  @param[out]      sumAbs                Sum of the log2 of absolute value.
*  @param[in]       checkDetMatrixTx    Optionally check whether the detection matrix has been 
*                                       transferred to L3. 
*  @param[in]       rangeIdx            The index of the range gate being processed.
*  @param[in, out]  pingPongIdxPtr      Pointer to the current ping-pong indx
*
*  @retval waitingTime.
*/
uint32_t secondDimFFTandLog2Computation(DSS_DataPathObj * restrict obj, uint16_t * restrict sumAbs, uint16_t checkDetMatrixTx, uint16_t rangeIdx, uint32_t * pingPongIdxPtr)
{
    int32_t rxAntIdx, idx;
    volatile uint32_t startTime;
    volatile uint32_t startTimeWait;
    uint32_t waitingTime = 0;
    uint16_t subframeIndx = obj->subframeIndx;
    uint16_t continueDMA = ((obj->processingPath == MAX_VEL_ENH_PROCESSING) && 
                            (checkDetMatrixTx == 1));
    uint32_t pingPongIdx = *pingPongIdxPtr;
    
    /* 2nd Dimension FFT is done here */
    for (rxAntIdx = 0; rxAntIdx < (obj->numRxAntennas * obj->numTxAntennas); rxAntIdx++)
    {
        /* verify that previous DMA has completed */
        startTimeWait = Cycleprofiler_getTimeStamp();
        MmwDemo_dataPathWait2DInputData(obj, pingPongId(pingPongIdx), subframeIndx);
        waitingTime += Cycleprofiler_getTimeStamp() - startTimeWait;

        /* kick off next DMA */
        if ((rangeIdx < obj->numRangeBins - 1) || 
            (rxAntIdx < (obj->numRxAntennas * obj->numTxAntennas) - 1) ||
            (continueDMA == 1))
        {
            if (isPong(pingPongIdx))
            {
                MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
                    MRR_SF0_EDMA_CH_2D_IN_PING,
                    subframeIndx);
            }
            else
            {
                MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
                    MRR_SF0_EDMA_CH_2D_IN_PONG,
                    subframeIndx);
            }
        }
        
        /* process data that has just been DMA-ed  */
        mmwavelib_windowing16x32(
            (int16_t *)&obj->dstPingPong[pingPongId(pingPongIdx) * obj->numDopplerBins],
            obj->window2D,
            (int32_t *)obj->windowingBuf2D,
            obj->numDopplerBins);

        DSP_fft32x32(
            (int32_t *)obj->twiddle32x32_2D,
            obj->numDopplerBins,
            (int32_t *)obj->windowingBuf2D,
            (int32_t *)obj->fftOut2D);

        mmwavelib_log2Abs32(
            (int32_t *)obj->fftOut2D,
            obj->log2Abs,
            obj->numDopplerBins);

        if (rxAntIdx == 0)
        {
            /* check if previous sumAbs has been transferred */
            if ((rangeIdx > 0) && (checkDetMatrixTx == 1))
            {
                startTimeWait = Cycleprofiler_getTimeStamp();
                MmwDemo_dataPathWaitTransDetMatrix(obj, subframeIndx);
                waitingTime += Cycleprofiler_getTimeStamp() - startTimeWait;
            }

            _nassert((uint32_t) sumAbs % 8 == 0);
            _nassert((uint32_t) obj->log2Abs % 8 == 0);
            _nassert(obj->numDopplerBins % 8 == 0);
            for (idx = 0; idx < obj->numDopplerBins; idx++)
            {
                sumAbs[idx] = obj->log2Abs[idx];
            }
        }
        else
        {
            mmwavelib_accum16(obj->log2Abs, sumAbs, obj->numDopplerBins);
        }
        pingPongIdx ^= 1;
    }

    *pingPongIdxPtr = pingPongIdx;
    return waitingTime;
}

/**
*  @b Description
*  @n
*      This function finds the intersection of the 1D cfar objects (computed
* previously) and the outputs of the 2D CFAR function. The purpose is to 
* select only those objects which have been detected in both the 1D and 2D 
* CFARs. 
*
*
*  @param[in,out] obj             data path object.
*  @param[in]  numDetObjPerCfar      number of detected objects from the CFAR function.
*  @param[in]  dopplerLine          The index of the doppler gate being processed. 
*  @param[in]  numDetObj2D        The total number of detected objects.
*  @param[in]  sumAbsRange        The sumAbs Array for the range dimension. It is used 
*                                  to populate the 'peakVal' on a per object basis. 
*  @retval 
*    The total number of detected objects (including the results of the current 
*     intersection).
*/

uint32_t findandPopulateIntersectionOfDetectedObjects(
    DSS_DataPathObj * restrict obj,
    uint32_t numDetObjPerCfar,
    uint16_t dopplerLine,
    uint32_t numDetObj2D,
    uint16_t * restrict sumAbsRange)
{

    uint32_t detIdx1, detIdx2;
    uint16_t rangeIdx;
    float disambiguatedSpeed;
    float range;
    uint32_t detObj1DRawIdx;
    int16_t dopplerIdxActual;
    uint32_t oneQFormat = (1 << obj->xyzOutputQFormat);
    MmwDemo_objRaw2D_t* restrict detObj2DRaw = obj->detObj2DRaw;
    MmwDemo_objRaw1D_t* restrict detObj1DRaw = obj->detObj1DRaw;
    uint16_t * restrict cfarDetObjIndexBuf = obj->cfarDetObjIndexBuf;
    uint16_t * restrict cfarDetObjSNR = obj->cfarDetObjSNR;
    uint16_t maxNumObj2DRaw = obj->maxNumObj2DRaw;
    
    /* For each object in the CFAR detected object list, */
    for (detIdx2 = 0; detIdx2 < numDetObjPerCfar; detIdx2++)
    {
        /* if there is space in the detObj2DRaw matrix, */
        if (numDetObj2D < maxNumObj2DRaw)
        {
            /* locate the 1D CFAR corresponding to the current range gate. */
            for (detIdx1 = 0; detIdx1 < MAX_NUM_DET_PER_RANGE_GATE; detIdx1++)
            {
                rangeIdx = cfarDetObjIndexBuf[detIdx2];
                
                detObj1DRawIdx = (rangeIdx*MAX_NUM_DET_PER_RANGE_GATE) + detIdx1;
                
                /* Check if the 1D CFAR matches the current objects doppler. 
                  * Also, check if the velocity disambiguation output is valid. */
                if ((detObj1DRaw[detObj1DRawIdx].velDisambFacValidity >= 0) &&
                    (detObj1DRaw[detObj1DRawIdx].dopplerIdx == dopplerLine))
                {
                        
                    /* Calculate 
                     * 1. The speed (after disambiguation). */
                    if (dopplerLine >= (obj->numDopplerBins >> 1))
                    {
                        dopplerIdxActual = dopplerLine - obj->numDopplerBins;
                    }
                    else
                    {
                        dopplerIdxActual = dopplerLine;
                    }
                    
                    disambiguatedSpeed = ((float)dopplerIdxActual * obj->velResolution) + 
                                         ((float)(2*(detObj1DRaw[detObj1DRawIdx].velDisambFacValidity - 1)) * obj->maxUnambiguousVel);
                    
                    /* 2. The range (after correcting for the antenna delay). */
                    range = (( (float)rangeIdx * obj->rangeResolution) - MIN_RANGE_OFFSET_METERS);
                    
                    if (range < 0.0f)
                    {
                        range = 0.0f;
                    }
                
                    /* 3. Populate the output Array. */
                    detObj2DRaw[numDetObj2D].dopplerIdx = dopplerLine; 
                    detObj2DRaw[numDetObj2D].rangeIdx = rangeIdx;
                    
                    detObj2DRaw[numDetObj2D].speed = (int16_t) (disambiguatedSpeed * oneQFormat); 
                    detObj2DRaw[numDetObj2D].range = (uint16_t)(range * oneQFormat);
                    /* 4. Note that the peakVal is taken from the sumAbsRange. */
                    detObj2DRaw[numDetObj2D].peakVal = sumAbsRange[rangeIdx] >> obj->log2numVirtAnt;;
                    /* 5. Note that the SNR is taken from the CFAR output. */
                    detObj2DRaw[numDetObj2D].rangeSNRdB = cfarDetObjSNR[detIdx2] >> obj->log2numVirtAnt;
                    detObj2DRaw[numDetObj2D].dopplerSNRdB = detObj1DRaw[detObj1DRawIdx].dopplerSNRdB;
                    numDetObj2D++;
                    break;
                }
            }
        }
    }

    return numDetObj2D;
}

/**
*  @b Description
*  @n
*      This function populates the 2D cfar object 
*
*
*  @param[in,out] obj             data path object.
*  @param[in]  numDetObjPerCfar      number of detected objects from the CFAR function.
*  @param[in]  dopplerLine          The index of the doppler gate being processed. 
*  @param[in]  numDetObj2D        The total number of detected objects.
*  @param[in]  sumAbsRange        The sumAbs Array for the range dimension. It is used 
*                                  to populate the 'peakVal' on a per object basis. 
*  @retval 
*    The total number of detected objects (including the results of the current 
*     intersection).
*/
uint32_t findandPopulateDetectedObjects(
    DSS_DataPathObj * restrict obj,
    uint32_t numDetObjPerCfar,
    uint16_t dopplerLine,
    uint32_t numDetObj2D,
    uint16_t * restrict sumAbsRange)
{

    uint32_t  detIdx2;
    uint16_t rangeIdx;
    float speed, range;
    int16_t dopplerIdxActual;
    uint32_t oneQFormat = (1 << obj->xyzOutputQFormat);
    MmwDemo_objRaw2D_t* restrict detObj2DRaw = obj->detObj2DRaw;
    uint16_t * restrict cfarDetObjIndexBuf = obj->cfarDetObjIndexBuf;
    uint16_t * restrict cfarDetObjSNR = obj->cfarDetObjSNR;
    
    /* For each object in the CFAR detected object list, */
    for (detIdx2 = 0; detIdx2 < numDetObjPerCfar; detIdx2++)
    {
        /* if there is space in the detObj2DRaw matrix, */
        if (numDetObj2D < obj->maxNumObj2DRaw)
        {
            rangeIdx = cfarDetObjIndexBuf[detIdx2];
                
            /* Calculate 
             * 1. The speed. */
            if (dopplerLine >= (obj->numDopplerBins >> 1))
            {
                dopplerIdxActual = (int16_t) dopplerLine - (int16_t)obj->numDopplerBins;
            }
            else
            {
                dopplerIdxActual = (int16_t)dopplerLine;
            }
                    
            speed = ((float)dopplerIdxActual) * obj->velResolution;
            /* 2. The range (after correcting for the antenna delay). */
            range = (((float)rangeIdx) * obj->rangeResolution) - MIN_RANGE_OFFSET_METERS;
            if (range < 0.0f)
            {
                range = 0.0f;
            }
            
            /* 3. Populate the output Array. */
            detObj2DRaw[numDetObj2D].rangeIdx = rangeIdx;
            detObj2DRaw[numDetObj2D].dopplerIdx = dopplerLine; 
            
            detObj2DRaw[numDetObj2D].speed = (int16_t) (speed * oneQFormat); 
            detObj2DRaw[numDetObj2D].range = (uint16_t)(range * oneQFormat);
            /* 4. Note that the peakVal is taken from the sumAbsRange. */
            detObj2DRaw[numDetObj2D].peakVal = sumAbsRange[rangeIdx] >> obj->log2numVirtAnt;
            /* 5. Note that the SNR is taken from the CFAR output. */
            detObj2DRaw[numDetObj2D].rangeSNRdB = cfarDetObjSNR[detIdx2] >> obj->log2numVirtAnt;
            /* 6. Since we have no estimate of the doppler SNR, set it to 0. */
            detObj2DRaw[numDetObj2D].dopplerSNRdB = 0;
            
            numDetObj2D++;
        }
    }
    return numDetObj2D;
}


/**
*  @b Description
*  @n
*      This function pruneToPeaks selects the peaks from within the list of objects
*    detected by CFAR.
*
*  @param[in,out] cfarDetObjIndexBuf  The indices of the detected objects.
*  @param[in,out] cfarDetObjSNR      The SNR of the detected objects.
*  @param[in]  numDetObjPerCfar   The number of detected objects. 
*  @param[in]  sumAbs             The sumAbs array on which the CFAR was run.
*
*  @retval 
*    The number of detected objects that are peaks.
*/
uint32_t pruneToPeaks(uint16_t* restrict cfarDetObjIndexBuf, 
                      uint16_t* restrict cfarDetObjSNR, 
                      uint32_t numDetObjPerCfar, 
                      uint16_t* restrict sumAbs, 
                      uint16_t numBins)
{
    uint32_t detIdx2;
    uint32_t numDetObjPerCfarActual = 0;
    uint16_t currObjLoc, prevIdx, nextIdx;

    if (numDetObjPerCfar == 0)
    {
        return 0;
    }
    /* Prune to peaks */
    for (detIdx2 = 0; detIdx2 < numDetObjPerCfar; detIdx2++)
    {
        currObjLoc = cfarDetObjIndexBuf[detIdx2];
        
        if (currObjLoc == 0)
        {
            prevIdx = numBins - 1;
        }    
        else
        {
            prevIdx = currObjLoc - 1;
        }

        if (currObjLoc == numBins - 1)
        {
            nextIdx = 0;
        }
        else
        {
            nextIdx = currObjLoc + 1;
        }

        if ((sumAbs[nextIdx] < sumAbs[currObjLoc])
            && (sumAbs[prevIdx] < sumAbs[currObjLoc]))
        {
            cfarDetObjIndexBuf[numDetObjPerCfarActual] = currObjLoc;
            cfarDetObjSNR[numDetObjPerCfarActual] = cfarDetObjSNR[detIdx2];
            numDetObjPerCfarActual++;
        }
    }
  
    return numDetObjPerCfarActual;
}

/**
*  @b Description
*  @n
*      This function finds the K strongest objects in a given list of objects. The 
*      'strength' of an object is its 'peak value' corresponding to its index in the 
*      sumAbs Array. 
*
*  @param[in,out] cfarDetObjIndexBuf  The indices of the detected objects.
*  @param[in,out] cfarDetObjSNR      The SNR of the detected objects.
*  @param[in]  numDetObjPerCfar   The number of detected objects. 
*  @param[in]  sumAbs             The sumAbs array on which the CFAR was run.
*  @param[in]  numBins            The length of the cfarDetObjSNR and 
*                                   cfarDetObjIndexBuf array.
*  @param[in]  K                  The maximum number of objects to be returned by 
*                                   this  function.
*
*  @retval 
*    min(K, numDetObjPerCfar).
*/
uint32_t findKLargestPeaks(uint16_t * restrict cfarDetObjIndexBuf,
                           uint16_t * restrict cfarDetObjSNR, 
                           uint32_t numDetObjPerCfar, 
                           uint16_t * restrict sumAbs, 
                           uint16_t numBins, 
                           uint16_t K)
{
    uint32_t detIdx1, detIdx2;
    uint16_t currMax;
    uint16_t currMaxIdx;
    uint16_t tempIdx;
    uint16_t tempSNR;
    
    if (numDetObjPerCfar <= 1)
    {
        return numDetObjPerCfar;
    }
    
    /* if the number of peaks is already less than or equal to K, we would still like the algorithm
    * to run, so as to organise the peaks in descending order. . */
    if (K >= numDetObjPerCfar)
    {
        K = numDetObjPerCfar;
    }
    
    /* Find the largest K peaks. K is typically a small value (1, 2 or 3). Hence the suboptimal
     * algorithm. */
    _nassert((uint32_t) K > 0);
    for (detIdx1 = 0; detIdx1 < K; detIdx1++)
    {
        currMax = 0;
        for (detIdx2 = detIdx1; detIdx2 < numDetObjPerCfar; detIdx2++)
        {
            if (currMax <= sumAbs[cfarDetObjIndexBuf[detIdx2]])
            {
                currMax = sumAbs[cfarDetObjIndexBuf[detIdx2]];
                currMaxIdx = detIdx2;
            }
        }
     
        /* Replace the cfarDetObjIndexBuf[detIdx1] with the obj at detIdx1. */
        tempIdx = cfarDetObjIndexBuf[detIdx1];
        tempSNR = cfarDetObjSNR[detIdx1];
        cfarDetObjIndexBuf[detIdx1] = cfarDetObjIndexBuf[currMaxIdx];
        cfarDetObjSNR[detIdx1] = cfarDetObjSNR[currMaxIdx];
        cfarDetObjIndexBuf[currMaxIdx] = tempIdx;
        cfarDetObjSNR[currMaxIdx] = tempSNR;        
    }

    return K;
}


/**
*  @b Description
*  @n
*  The MRR subframe achieves a maximum unambiguous velocity of 90kmph 
*  by using signal processing techniques that help disambiguate 
*  velocity.  This method works by using two different estimates of 
*  velocity from the two kinds of chirps (‘fast chirps’ and ‘slow 
*  chirps’) transmitted in the MRR subframe. If the two velocity estimates 
*  do not agree, then velocity disambiguation is necessary. To 
*  disambiguate it is necessary to rationalize the two velocity 
*  measurements, and find out the disambiguation factor, k.  If the 
*  naive maximum unambiguous velocity of the 'fast chirp' is v_f, and 
*  that of the 'slow chirp' is v_s. Then after the disambiguation process, 
*  the disambiguated velocity would  〖2kv〗_f+v, where v is the naïve 
*  estimated velocity from the ‘fast chirps’.
* 
*  The disambiguation process works by using the 'fast chirp' velocity to 
*  compute different disambiguated velocity hypotheses. This is done by 
*  taking the 'fast chirp' velocity and adding 2k v_f, where k ∈ {-1,0,1} 
*  (an unwrapping process on the velocity estimate). These hypotheses are 
*  then converted to indices of the 'slow chirp' by finding the equivalent 
*  estimated velocities in the 'slow chirp' configuration ( essentially, 
*  undoing the unwrapping using v_s as the maximum unambiguous velocity). 
*
*  If the index corresponding to one of the hypotheses has significant 
*  energy, then that hypothesis is considered to be valid. Disambiguation of 
*  up to 3x of the naive max-velocity is possible with this method, however, 
*  testing has only been done up to 90 kmph.  
*
*  @param[in,out] sumAbs    The slow chirps doppler bins at a certain range.  
*  @param[in] fastChirpVel    The velocity estimate using the fast chirps (pre-disambiguation).  
*  @param[in] fastChirpPeakVal    The peak value of the index of the detected 
*                                object from the fast chirp. 
*  @param[in] obj   data path object. 
*  
*
*  @retval 
*    Ambiguity index. 
*/


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    cfarCadB_SO_withSNR
 *
 * \par
 * <b>Description</b>  :    Performs a CFAR SO on an 16-bit unsigned input vector. The input values are assumed to be
 *                          in lograthimic scale. So the comparision between the CUT and the noise samples is additive
 *                          rather than multiplicative. 
 *
 * @param[in]               inp         : input array (16 bit unsigned numbers)
 * @param[in]               out         : output array (indices of detected peaks (zero based counting))
 * @param[in]               outSNR      : output array (SNR of detected peaks)
 * @param[in]               len         : number of elements in input array
 * @param[in]               const1,const2 : used to compare the Cell Under Test (CUT) to the sum of the noise cells:
 *                                          [noise sum /(2^(const2-1))]+const1 for one sided comparison 
 *                                          (at the begining and end of the input vector).
 *                                          [noise sum /(2^(const2))]+const1 for two sided comparison
 * @param[in]               guardLen    : one sided guard length
 * @param[in]               noiseLen    : one sided noise length
 * @param[in]                 minIndxToIgnoreHPF : the number of indices to force one sided CFAR, so as to avoid false 
 *                          detections due to effect of the HPF.
 * @param[out]              out         : output array with indices of the detected peaks
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre                     Input (inp) and Output (out) arrays are non-aliased.
 * @ingroup                 MMWAVELIB_DETECT
 *
 *******************************************************************************************************************
 */
uint32_t cfarCadB_SO_withSNR(const uint16_t inp[restrict],
    uint16_t out[restrict],
    uint16_t outSNR[restrict], uint32_t len,
    uint32_t const1, uint32_t const2,
    uint32_t guardLen, uint32_t noiseLen, uint32_t minIndxToIgnoreHPF)
{
    uint32_t idx, idxLeftNext, idxLeftPrev, idxRightNext,
        idxRightPrev, outIdx, idxCUT;
    uint32_t sum, sumLeft, sumRight;

    /* initializations */
    outIdx = 0;
    sumLeft = 0;
    sumRight = 0;
    for (idx = 0; idx < noiseLen; idx++)
    {
        sumRight += inp[idx + guardLen + 1U];
    }

    /* One-sided comparision for the first segment (for the first noiseLen+gaurdLen samples */
    idxCUT = 0;
    if ((uint32_t) inp[idxCUT] > ((sumRight >> (const2 - 1U)) + const1))
    {
        out[outIdx] = (uint16_t)idxCUT;
        /* SNR (dB) is simply the peak - noise floor) */
        outSNR[outIdx] = inp[idxCUT] - (sumRight >> (const2 - 1));
        outIdx++;
    }
    idxCUT++;

    idxLeftNext = 0;
    idxRightPrev = idxCUT + guardLen;
    idxRightNext = idxRightPrev + noiseLen;
    for (idx = 0; idx < (noiseLen + guardLen - 1U); idx++)
    {
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;

        if (idx < noiseLen)
        {
            sumLeft += inp[idxLeftNext];
            idxLeftNext++;
        }

        if ((uint32_t) inp[idxCUT] > ((sumRight >> (const2 - 1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outSNR[outIdx] = inp[idxCUT] - (sumRight >> (const2 - 1));
            outIdx++;
        }
        idxCUT++;
    }

    /* Two-sided comparision for the middle segment */
    sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
    idxRightNext++;
    idxRightPrev++;


    {
        sum = (sumLeft < sumRight) ? sumLeft : sumRight;
        if ((uint32_t) inp[idxCUT] > ((sum >> (const2-1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outSNR[outIdx] = inp[idxCUT] - (sum >> (const2 - 1));
            outIdx++;
        }
        idxCUT++;

        idxLeftPrev = 0;
        for (idx = 0; idx < (len - 2U*(noiseLen + guardLen) - 1U); idx++)
        {
            sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
            sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
            idxLeftNext++;
            idxLeftPrev++;
            idxRightNext++;
            idxRightPrev++;

            if (idx < minIndxToIgnoreHPF)
            { 
                sum = sumRight; 
            }
            else
            { 
                sum = (sumLeft < sumRight) ? sumLeft : sumRight;
            }
            
            if ((uint32_t)(inp[idxCUT]) >((sum >> (const2 - 1U)) + const1))
            {
                out[outIdx] = idxCUT;
                outSNR[outIdx] = inp[idxCUT] - (sum >> (const2 - 1U));
                outIdx++;
            }
            idxCUT++;
        }
    } /*CFAR_CASO*/

      /* One-sided comparision for the last segment (for the last noiseLen+gaurdLen samples) */
    for (idx = 0; idx < (noiseLen + guardLen); idx++)
    {
        sumLeft += inp[idxLeftNext] - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        if ((uint32_t)inp[idxCUT] >((sumLeft >> (const2 - 1)) + const1))
        {
            out[outIdx] = idxCUT;
            outSNR[outIdx] = inp[idxCUT] - (sumLeft >> (const2 - 1));
            outIdx++;
        }
        idxCUT++;
    }

    return (outIdx);

}  


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    cfarCa_SO_dBwrap_withSNR
 *
 * \par
 * <b>Description</b>  :    Performs a CFAR on an 16-bit unsigned input vector (CFAR-CA). The input values are assumed to be
 *                          in logarithmic scale. So the comparison between the CUT and the noise samples is additive
 *                          rather than multiplicative. Comparison is two-sided (wrap around when needed) for all CUTs.
 *
 * @param[in]               inp      : input array (16 bit unsigned numbers)
 * @param[in]               out      : output array (indices of detected peaks (zero based counting))
 * @param[in]               outSNR   : SNR array (SNR of detected peaks)
 * @param[in]               len      : number of elements in input array
 * @param[in]               const1,const2 : used to compare the Cell Under Test (CUT) to the sum of the noise cells:
 *                                          [noise sum /(2^(const2))] +const1 for two sided comparison.
 * @param[in]               guardLen : one sided guard length
 * @param[in]               noiseLen : one sided Noise length
 *
 * @param[out]              out      : output array with indices of the detected peaks
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre                     Input (inp) and Output (out) arrays are non-aliased.
 * @ingroup                 MMWAVELIB_DETECT
 *
 *******************************************************************************************************************
 */
uint32_t cfarCa_SO_dBwrap_withSNR(const uint16_t inp[restrict],
                                uint16_t out[restrict], 
                                uint16_t outSNR[restrict], 
                                uint32_t len,
                                uint32_t const1, uint32_t const2,
                                uint32_t guardLen, uint32_t noiseLen)
{
    uint32_t idx, idxLeftNext, idxLeftPrev, idxRightNext,
        idxRightPrev, outIdx, idxCUT;
    uint32_t sum, sumLeft, sumRight;

    /*initializations */
    outIdx = 0;
    sumLeft = 0;
    sumRight = 0;
    for (idx = 1; idx <= noiseLen; idx++)
    {
        sumLeft += inp[len - guardLen - idx];
    }

    for (idx = 1; idx <= noiseLen; idx++)
    {
        sumRight += inp[idx + guardLen];
    }

    /*CUT 0: */
    sum = (sumLeft < sumRight) ? sumLeft:sumRight;
    if ((uint32_t) inp[0] > ((sum >> (const2-1)) + const1))
    {
        out[outIdx] = 0;
        outSNR[outIdx] = inp[0]  - (sum >> (const2 - 1)); 
        outIdx++;
    }

    /* CUT 1 to guardLen: */
    idxLeftPrev = len - guardLen - noiseLen;    /*e.g. 32-4-8 = 20 */
    idxLeftNext = idxLeftPrev + noiseLen; /*e.g. 28 */
    idxRightPrev = 1 + guardLen;    /*e.g. 1+4=5 */
    idxRightNext = idxRightPrev + noiseLen;   /*e.g. 13 */
    for (idxCUT = 1; idxCUT <= guardLen; idxCUT++)
    {
        sumLeft += inp[idxLeftNext] - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight += inp[idxRightNext] - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = (sumLeft < sumRight) ? sumLeft:sumRight;
        
        if ((uint32_t) (inp[idxCUT]) > ((sum >> (const2 - 1)) + const1))
        {
            out[outIdx] = idxCUT;
            outSNR[outIdx] = inp[idxCUT]  - (sum >> (const2 - 1));
            outIdx++;
        }
    }

    /* CUT guardLen+1 to guardLen+noiseLen: e.g. CUT 5 to 12 */
    idxLeftNext = 0;
    for (idxCUT = guardLen + 1; idxCUT <= guardLen + noiseLen;
         idxCUT++)
    {
        sumLeft += inp[idxLeftNext] - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight += inp[idxRightNext] - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = (sumLeft < sumRight) ? sumLeft:sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> (const2 - 1)) + const1))
        {
            out[outIdx] = idxCUT;
            outSNR[outIdx] = inp[idxCUT]  - (sum >> (const2 - 1));
            outIdx++;
        }
    }

    /* CUTs in the middle. e.g. CUT 13 to 19 */
    idxLeftPrev = 0;
    for (idxCUT = guardLen + noiseLen + 1;
         idxCUT < len - (noiseLen + guardLen); idxCUT++)
    {
        sumLeft += inp[idxLeftNext] - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight += inp[idxRightNext] - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = (sumLeft < sumRight) ? sumLeft:sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> (const2 - 1)) + const1))
        {
            out[outIdx] = idxCUT;
            outSNR[outIdx] = inp[idxCUT]  - (sum >> (const2 - 1));            
            outIdx++;
        }
    }

    /* noiseLen number of CUTs before the last guardLen CUTs. e.g. CUT 20 to 27 */
    idxRightNext = 0;
    for (idxCUT = len - (noiseLen + guardLen);
         idxCUT < len - guardLen; idxCUT++)
    {
        sumLeft += inp[idxLeftNext] - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight += inp[idxRightNext] - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = (sumLeft < sumRight) ? sumLeft:sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> (const2 - 1)) + const1))
        {
            out[outIdx] = idxCUT;
            outSNR[outIdx] = inp[idxCUT]  - (sum >> (const2 - 1));            
            outIdx++;
        }
    }

    /* The last guardLen number of CUTs */
    idxRightPrev = 0;
    for (idxCUT = len - guardLen; idxCUT < len; idxCUT++)
    {
        sumLeft += inp[idxLeftNext] - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight += inp[idxRightNext] - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = (sumLeft < sumRight) ? sumLeft:sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> (const2 - 1)) + const1))
        {
            out[outIdx] = idxCUT;
            outSNR[outIdx] = inp[idxCUT]  - (sum >> (const2 - 1));          
            outIdx++;
        }
    }
    return (outIdx);
}  /* cfarCa_SO_dBwrap_withSNR  */


/**
 *  @b Description
 *  @n
 *    The function groups neighboring peaks into one. The grouping is done
 *    according to two input flags: groupInDopplerDirection and
 *    groupInDopplerDirection. For each detected peak the function checks
 *    if the peak is greater than its neighbors. If this is true, the peak is
 *    copied to the output list of detected objects. The neighboring peaks that are used
 *    for checking are taken from the detection matrix and copied into 3x3 kernel
 *    regardless of whether they are CFAR detected or not.
 *    Note: Function always reads 9 samples per detected object
 *    from L3 memory into local array tempBuff, but it only needs to read according to input flags.
 *    For example if only the groupInDopplerDirection flag is set, it only needs
 *    to read middle row of the kernel, i.e. 3 samples per target from detection matrix.
 *  @param[out]   objOut             Output array of  detected objects after peak grouping
 *  @param[in]    objRaw             Array of detected objects after CFAR detection
 *  @param[in]    numDetectedObjects Number of detected objects by CFAR
 *  @param[in]    detMatrix          Detection Range/Doppler matrix
 *  @param[in]    numDopplerBins     Number of Doppler bins
 *  @param[in]    groupInDopplerDirection Flag enables grouping in Doppler directiuon
 *  @param[in]    groupInRangeDirection   Flag enables grouping in Range directiuon
 *
 *  @retval
 *      Number of detected objects after grouping
 */
uint32_t MmwDemo_cfarPeakGrouping(
                                MmwDemo_detectedObjActual*  objOut,
                                uint32_t numDetectedObjects,
                                uint16_t* detMatrix,
                                uint32_t numRangeBins,
                                uint32_t numDopplerBins,
                                uint32_t groupInDopplerDirection,
                                uint32_t groupInRangeDirection)
{
    int32_t i, j;
    int32_t rowStart, rowEnd;
    uint32_t numObjOut = 0;
    uint32_t rangeIdx, dopplerIdx;
    uint16_t *tempPtr;
    uint16_t kernel[9], detectedObjFlag;
    int32_t k, l;
    uint32_t startInd, stepInd, endInd;
    MmwDemo_detectedObjActual * objRaw = objOut;

    if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 1))
    {
        /* Grouping both in Range and Doppler direction */
        startInd = 0;
        stepInd = 1;
        endInd = 8;
    }
    else if ((groupInDopplerDirection == 0) && (groupInRangeDirection == 1))
    {
        /* Grouping only in Range direction */
        startInd = 1;
        stepInd = 3;
        endInd = 7;
    }
    else if ((groupInDopplerDirection == 1) && (groupInRangeDirection == 0))
    {
        /* Grouping only in Doppler direction */
        startInd = 3;
        stepInd = 1;
        endInd = 5;
    }
    else
    {
        /* No grouping, copy all detected objects to the output matrix within specified min max range*/
        return numDetectedObjects;
    }

    /* Start checking */
    for(i = 0; i < numDetectedObjects; i++)
    {
        rangeIdx = objRaw[i].rangeIdx;
        dopplerIdx = objRaw[i].dopplerIdx;
        
        detectedObjFlag = 1;

        /* Fill local 3x3 kernel from detection matrix in L3*/
        tempPtr = detMatrix + (rangeIdx-1)*numDopplerBins;
        rowStart = 0;
        rowEnd = 2;
        
        if (rangeIdx == 0)
        {
            tempPtr = detMatrix + (rangeIdx)*numDopplerBins;
            rowStart = 1;
            memset((void *) kernel, 0, 3 * sizeof(uint16_t));
        }
        else if (rangeIdx == numRangeBins-1)
        {
            rowEnd = 1;
            memset((void *) &kernel[6], 0, 3 * sizeof(uint16_t));
        }

        for (j = rowStart; j <= rowEnd; j++)
        {
            for (k = 0; k < 3; k++)
            {
                l = dopplerIdx + (k - 1);
                if(l < 0)
                {
                    l += numDopplerBins;
                }
                else if(l >= numDopplerBins)
                {
                    l -= numDopplerBins;
                }
                kernel[j*3+k] = tempPtr[l];
            }
            tempPtr += numDopplerBins;
        }

        /* Compare the detected object to its neighbors.
         * Detected object is at index 4*/
        for (k = startInd; k <= endInd; k += stepInd)
        {
            if(kernel[k] > kernel[4])
            {
                detectedObjFlag = 0;
            }
        }

        if (detectedObjFlag == 1)
        {
            objOut[numObjOut] = objRaw[i];
            numObjOut++;
        }

        if (numObjOut >= MRR_MAX_OBJ_OUT)
        {
            break;
        }
    }

    return (numObjOut);
}

/**
 *  @b Description
 *  @n
 *    The function groups neighboring peaks (only in the doppler direction) into one. For each  
 *    detected peak the function checks if the peak is greater than its neighbors. If this is true, 
 *    the peak is copied to the output list of detected objects. The neighboring peaks that are used
 *    for checking are taken from the detection matrix and copied into 1x3 kernel regardless of 
 *    whether they are CFAR detected or not.
 *
 *    Note: Function always reads 3 samples per detected object from L3 memory into local array. 
 *
 *  @param[out]   objOut             Output array of  detected objects after peak grouping
 *  @param[in]    objRaw             Array of detected objects after CFAR detection
 *  @param[in]    numDetectedObjects Number of detected objects by CFAR
 *  @param[in]    detMatrix          Detection Range/Doppler matrix
 *  @param[in]    numDopplerBins     Number of Doppler bins3401

 *
 *  @retval
 *      Number of detected objects after grouping
 */
uint32_t cfarPeakGroupingAlongDoppler(
                                MmwDemo_objRaw2D_t * restrict objOut,
                                uint32_t numDetectedObjects,
                                uint16_t * restrict detMatrix,
                                uint32_t numRangeBins,
                                uint32_t numDopplerBins)
{
    int32_t i;
    uint32_t numObjOut = 0;
    uint32_t rangeIdx, dopplerIdx;
    uint16_t *tempPtr;
    uint16_t kernel[3], detectedObjFlag;
    int32_t k, l;
    
    /* Grouping only in Doppler direction */
   
    /* Start checking */
    for(i = 0; i < numDetectedObjects; i++)
    {
        rangeIdx = objOut[i].rangeIdx;
        dopplerIdx = objOut[i].dopplerIdx;
        
        detectedObjFlag = 1;

        /* Fill local 1x3 kernel from detection matrix in L3*/
        tempPtr = detMatrix + (rangeIdx)*numDopplerBins;
        
        for (k = 0; k < 3; k++)
        {
            l = dopplerIdx + (k - 1);
            
            if(l < 0)
            {
                l += numDopplerBins;
            }
            else if(l >= numDopplerBins)
            {
                l -= numDopplerBins;
            }
            
            kernel[k] = tempPtr[l];
        }
        
        /* Compare the detected object to its neighbors.*/
        if ( (kernel[1] < kernel[0]) || 
             (kernel[1] < kernel[2]))
        {
            detectedObjFlag = 0;
        }
           
        if (detectedObjFlag == 1)
        {
            objOut[numObjOut] = objOut[i];
            numObjOut++;
        }
    }

    return (numObjOut);
}

/**
 *  @b Description
 *  @n
 *    The function
 *  @param[out]   azimuthMagSqr      Input array of  the sum of the squares of the zero padded FFT 
 *                                   output.
 *  @param[in]    azimIdx            The location of the peak of the detected object.
 *  @param[in]    numVirtualAntAzim  the size of the FFT input.
 *  @param[in]    numAngleBins       The size of the FFT output.
 *  @param[in]    xyzOutputQFormat           number of fractional bits in the output.
 *
 *  @retval
 *      SNRlinear with the programmed fractional bitwidth
 */
uint16_t computeSinAzimSNR(float * azimuthMagSqr, uint16_t azimIdx, uint16_t numVirtualAntAzim, uint16_t numAngleBins, uint16_t xyzOutputQFormat)
{
    uint16_t ik, Idx, stepSize;
    uint16_t SNR;
    stepSize = numAngleBins/numVirtualAntAzim;
    float NoiseFloorSqrSum = 0;
    float SNRflt;
    
    for (ik = 1; ik < numVirtualAntAzim ; ik ++)
    {
        Idx = stepSize*ik + azimIdx;
        if (Idx > (numAngleBins - 1))
        {
            Idx -= numAngleBins;
        }
        /* azimuthMagSqrLim[Idx]| Idx== 0 is the peak index, azimuthMagSqrLim[Idx] | 
         * Idx == 1:numVirtualAntAzim-1] are the noise samples. */
        NoiseFloorSqrSum += azimuthMagSqr[Idx];
    }
    
    /* SNR = azimuthMagSqr[azimIdx]/(NoiseFloorSqrSum/(numVirtualAntAzim-1)) */
    SNRflt = divsp((azimuthMagSqr[azimIdx]*(numVirtualAntAzim-1)),NoiseFloorSqrSum);
    
    /* Some checks before converting to uint16_t */
    SNRflt = (SNRflt * (float) (1  << xyzOutputQFormat));
    if (SNRflt > 65535.0f)
    {
        SNR = 65535;
    }
    else
    {
        SNR = (uint16_t) _spint(SNRflt);
    }
    
    return SNR;
}


/**
 *  @b Description
 *  @n
 *    The function computes the CRLB of the given estimate given an  SNR input (dB)
 *    and the number of samples used in the estimate, and the resolution of the estimate. 
 *    
 *  @param[in]    SNRdB              16 bit input with specified bitwidth.
 *  @param[in]    bitW               input fractional bitwidth (for SNR in dB).
 *  @param[in]    n_samples          number of samples per chirp.
 *  @param[in]    resolution         range resolution in meters.
 *
 *  @retval
 *      CRLB in the specified resolution (with some lower bounds).
 */
float convertSNRdBToVar(uint16_t SNRdB,uint16_t bitW, uint16_t n_samples, float resolution)
{
    float fVar, RVar;
    float scaleFac  = (n_samples*resolution);
    float resThresh = 2 * resolution * resolution;
    float invSNRlin = antilog2(-SNRdB, bitW) * 2; // We assume our estimator is 3dB worse than the CRLB. 
    
    /* CRLB for a frequency estimate */
    fVar = (float)invSNRlin * (6.0f/((2.0f*PI_)*(2.0f*PI_))) * recipsp((n_samples*n_samples - 1));
        
    /* Convert to a parameter variance using the scalefactor. */
    RVar = fVar*scaleFac*scaleFac;
   
    if (RVar < resThresh)
    {
        RVar = resThresh;
    }
    return RVar;
    
}

/**
 *  @b Description
 *  @n
 *    The function computes the CRLB of the given estimate given an  SNR input (linear)
 *    and the number of samples used in the estimate, and the resolution of the estimate. 
 *
 *    The CRLB is lower bounded by the resolution.
 *    
 *  @param[in]    SNRdB              16 bit input with specified bitwidth.
 *  @param[in]    bitW               input fractional bitwidth.
 *  @param[in]    n_samples          number of samples per chirp.
 *  @param[in]    resolution         resolution in meters.
 *
 *  @retval
 *      2^(input/(2^fracBitIn)) 
 */
float convertSNRLinToVar(uint16_t SNRLin,uint16_t bitW, uint16_t n_samples, float resolution)
{
    float fVar, RVar;
    float scaleFac  = (n_samples*resolution);
    float invSNRlin; 
    
    DebugP_assert(SNRLin);    
    invSNRlin = recipsp( (float) SNRLin) * ((float) (1 << bitW)); 
    
    /* CRLB for a frequency estimate */
    fVar = (float)invSNRlin * (6.0f/((2.0f*PI_)*(2.0f*PI_))) * recipsp((n_samples*n_samples - 1));
    
    /* Convert to a parameter variance using the scalefactor. */
    RVar = fVar*scaleFac*scaleFac;
    
    return RVar;
}

/**
 *  @b Description
 *  @n
 *    The function computes an antilog2 on the input which has a0
 *  specified bitwidth.  
 *  @param[in]    input              16 bit input with specified bitwidth.
 *  @param[in]    fracBitIn          input fractional bitwidth.
 *
 *  @retval
 *      2^(input/(2^fracBitIn)) 
 */
float antilog2(int32_t inputActual, uint16_t fracBitIn)
{
    float output;
    float input = (float)inputActual;
    
    input = divsp(input , (float)(1 << fracBitIn));
    output =  exp2sp(input); 
    
    return output;
}

/**
 *  @b Description
 *  @n
 *    The function performs the third dimension processing, including
 *  the computation of the azimuth, and the x and y co-ordinate.
 *
 *  @param[in]    input              data path object.
 *
 *  @retval
 *      none 
 */
uint32_t aziEleProcessing(DSS_DataPathObj *obj, uint32_t subframeIndx)
{
    volatile uint32_t detIdx2;
    uint32_t numChirpTypes = 1;
    uint32_t numDetObj2D = obj->numDetObj;
    volatile uint32_t startTime;
    volatile uint32_t startTimeWait;
    uint32_t waitingTime = 0;
    uint32_t rxAntIdx;
    uint32_t cubeOffset;

    if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
    {
        numChirpTypes = 2;
    }

    /**
     *  Azimuth calculation
     **/
    for (detIdx2 = 0; detIdx2 < numDetObj2D; detIdx2++)
    {
        uint8_t chId;

        
        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_3D_IN_PING;
        }
        /* else
        {
            chId = MRR_SF1_EDMA_CH_3D_IN_PING;
        }*/

        /* Set source for first (ping) DMA and trigger it, and set source second (Pong) DMA */
        cubeOffset = obj->numDopplerBins * obj->numRxAntennas *
                obj->numTxAntennas * (uint32_t) obj->detObj2D[detIdx2].rangeIdx * numChirpTypes;
        EDMAutil_triggerType3(
            obj->edmaHandle[EDMA_INSTANCE_DSS],
            (uint8_t *)(&obj->radarCube[cubeOffset]),
            (uint8_t *)NULL,
            (uint8_t)chId,
            (uint8_t)MRR_EDMA_TRIGGER_ENABLE);

        if (subframeIndx == 0)
        {
            chId = MRR_SF0_EDMA_CH_3D_IN_PONG;
        }
        /*else
        {
            chId = MRR_SF1_EDMA_CH_3D_IN_PONG;
        } */

        cubeOffset = ((obj->numDopplerBins * obj->numRxAntennas *
                obj->numTxAntennas * ((uint32_t)obj->detObj2D[detIdx2].rangeIdx) * numChirpTypes) + obj->numDopplerBins);
        EDMAutil_triggerType3(
            obj->edmaHandle[EDMA_INSTANCE_DSS],
            (uint8_t *)(&obj->radarCube[cubeOffset]),
            (uint8_t *)NULL,
            (uint8_t)chId,
            (uint8_t)MRR_EDMA_TRIGGER_DISABLE);

        for (rxAntIdx = 0; rxAntIdx < (obj->numRxAntennas * obj->numTxAntennas); rxAntIdx++)
        {
            /* verify that previous DMA has completed. */
            startTimeWait = Cycleprofiler_getTimeStamp();
            MmwDemo_dataPathWait3DInputData(obj, pingPongId(rxAntIdx), subframeIndx);
            waitingTime += Cycleprofiler_getTimeStamp() - startTimeWait;

            /* kick off next DMA. */
            if (rxAntIdx < (obj->numRxAntennas * obj->numTxAntennas) - 1)
            {
                if (isPong(rxAntIdx))
                {
                    MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
                        MRR_SF0_EDMA_CH_3D_IN_PING,
                        subframeIndx);
                }
                else
                {
                    MmwDemo_startDmaTransfer(obj->edmaHandle[EDMA_INSTANCE_DSS],
                        MRR_SF0_EDMA_CH_3D_IN_PONG,
                        subframeIndx);
                }
            }

            /* Calculate one bin DFT, at detected doppler index.  */
            mmwavelib_dftSingleBinWithWindow(
                        (uint32_t *)&obj->dstPingPong[pingPongId(rxAntIdx) * obj->numDopplerBins],
                        (uint32_t *) obj->azimuthModCoefs,
                        obj->window2D,
                        (uint64_t *) &obj->antInp[rxAntIdx],
                        obj->numDopplerBins,
                        DOPPLER_IDX_TO_UNSIGNED(obj->detObj2D[detIdx2].dopplerIdx, obj->numDopplerBins));
                        
                         
        }
        /* Rx channel gain/phase offset compensation. */
        MmwDemo_rxChanPhaseBiasCompensation((uint32_t *) obj->rxChPhaseComp,
                                            (int64_t *) obj->antInp,
                                            obj->numTxAntennas * obj->numRxAntennas);


        /* Reset input buffer to azimuth FFT */
        memset((uint8_t *)obj->azimuthIn, 0, obj->numAngleBins * sizeof(cmplx32ReIm_t));

        /* Reset input buffer to azimuth FFT */
        memset((uint8_t *)obj->elevationIn, 0, obj->numAngleBins * sizeof(cmplx32ReIm_t));

        /* Populate the first four. */
        for (rxAntIdx = 0; rxAntIdx < (obj->numRxAntennas); rxAntIdx++)
        {
            obj->azimuthIn[rxAntIdx].real = obj->antInp[rxAntIdx].real/8;
			obj->azimuthIn[rxAntIdx].imag = obj->antInp[rxAntIdx].imag/8;
        }
        
		if (obj->numTxAntennas == 2)
        {
			/* Populate the second four. */        
			for (rxAntIdx = 0; rxAntIdx < (obj->numRxAntennas); rxAntIdx++)
			{
				obj->azimuthIn[rxAntIdx+obj->numRxAntennas].real = obj->antInp[rxAntIdx+obj->numRxAntennas].real/8;
				obj->azimuthIn[rxAntIdx+obj->numRxAntennas].imag = obj->antInp[rxAntIdx+obj->numRxAntennas].imag/8;
			}
		
		}
		else if(obj->numTxAntennas == 3)
		{
			/* Populate the second four. */        
			for (rxAntIdx = 0; rxAntIdx < (obj->numRxAntennas); rxAntIdx++)
			{
				obj->azimuthIn[rxAntIdx+obj->numRxAntennas].real = obj->antInp[rxAntIdx + (2*(obj->numRxAntennas))].real/8;
				obj->azimuthIn[rxAntIdx+obj->numRxAntennas].imag = obj->antInp[rxAntIdx + (2*(obj->numRxAntennas))].imag/8;
			}
			
			/* Populate the samples corresponding to the 'Tx' that is offset in elevation. */        
			for (rxAntIdx = 0; rxAntIdx < (obj->numRxAntennas); rxAntIdx++)
			{
				/* The 'virtual antenna' corresponding to the tx is offset in elevation. */				
				obj->elevationIn[rxAntIdx + 2].real = obj->antInp[rxAntIdx + obj->numRxAntennas].real/8;
				obj->elevationIn[rxAntIdx + 2].imag = obj->antInp[rxAntIdx + obj->numRxAntennas].imag/8;
			}
		}

        /* Compensation of Doppler phase shift in the virtual antennas, (correponding
        * to second Tx antenna chirps). Symbols corresponding to virtual antennas,
        * are rotated by half of the Doppler phase shift measured by Doppler FFT.
        * The phase shift read from the table using half of the object
        * Doppler index  value. If the Doppler index is odd, an extra half
        * of the bin phase shift is added. */
        if (obj->numTxAntennas > 1)
        {
            #if 1
            /* Add doppler compensation for the virtual antennas ([8 9 10 11] corresponding to the second half of the azimuthIn vector.  */
            MmwDemo_addDopplerCompensation(obj->detObj2D[detIdx2].dopplerIdx,
                                        (int32_t) obj->numDopplerBins,
                                        (uint32_t *) obj->azimuthModCoefs,
                                        (uint32_t *) &obj->azimuthModCoefsThirdBin,
										(uint32_t *) &obj->azimuthModCoefsTwoThirdBin,
				                        (int64_t *) &obj->azimuthIn[obj->numRxAntennas],
                                        obj->numRxAntennas * (1),
                                        obj->numTxAntennas, 2/*For Tx3*/);
            /* Add doppler compensation for the virtual antennas ([4 5 6 7] corresponding to the elevationIn vector.  */
            MmwDemo_addDopplerCompensation(obj->detObj2D[detIdx2].dopplerIdx,
                                        (int32_t) obj->numDopplerBins,
                                        (uint32_t *) obj->azimuthModCoefs,
										(uint32_t *)&obj->azimuthModCoefsThirdBin,
										(uint32_t *)&obj->azimuthModCoefsTwoThirdBin,
										(int64_t *) &obj->elevationIn[0],
                                        obj->numRxAntennas * (1),
                                        obj->numTxAntennas, 1 /*for Tx2*/); 
            #endif
                                        
        }
        
        /* FFT on the antenna array [4 5 6 7]. */
        if (obj->processingPath == POINT_CLOUD_PROCESSING)
        {
            /* 3D-FFT (Azimuth FFT on the second row) */
            
            DSP_fft32x32((int32_t *)obj->azimuthTwiddle32x32, obj->numAngleBins,
                         (int32_t *)obj->elevationIn, (int32_t *)obj->elevationOut);
        }
        
        

        
        if ((obj->processingPath == POINT_CLOUD_PROCESSING) && (MAX_VEL_POINT_CLOUD_PROCESSING_IS_ENABLED))
        {
            //TODO DIsable for 3 Tx mode. 
            /* Save copy for the flipped version for Velocity disambiguation */
            memcpy((void *) &obj->azimuthIn[obj->numAngleBins],
                   (void *) &obj->azimuthIn[0],
                   obj->numVirtualAntAzim * sizeof(cmplx32ReIm_t));
        }
        
        /* 3D-FFT (Azimuth FFT on the first row) i.e [0 1 2 3 8 9 10 11]. */
        DSP_fft32x32((int32_t *)obj->azimuthTwiddle32x32, obj->numAngleBins,
                     (int32_t *)obj->azimuthIn, (int32_t *)obj->azimuthOut);

        MmwDemo_magnitudeSquared(obj->azimuthOut, obj->azimuthMagSqr, obj->numAngleBins);

        
        if ((obj->processingPath == POINT_CLOUD_PROCESSING) && (MAX_VEL_POINT_CLOUD_PROCESSING_IS_ENABLED))
        {
            /* Zero padding */
            memset((void *) &obj->azimuthIn[obj->numVirtualAntAzim], 0,
                   (obj->numAngleBins - obj->numVirtualAntAzim) * sizeof(cmplx32ReIm_t));
            /* Retrieve saved copy of FFT input symbols */
            memcpy((void *) &obj->azimuthIn[0], (void *) &obj->azimuthIn[obj->numAngleBins],
                   obj->numVirtualAntAzim * sizeof(cmplx32ReIm_t));

            {
                /* Negate symbols corresponding to Tx2 antenna */
                uint32_t jj;
                for(jj = obj->numRxAntennas; jj<obj->numVirtualAntAzim; jj++)
                {
                    obj->azimuthIn[jj].imag = -obj->azimuthIn[jj].imag;
                    obj->azimuthIn[jj].real = -obj->azimuthIn[jj].real;
                }
            }

            /* 3D-FFT (Azimuth FFT) */
            DSP_fft32x32(
                (int32_t *)obj->azimuthTwiddle32x32,
                obj->numAngleBins,
                (int32_t *) obj->azimuthIn,
                (int32_t *) obj->azimuthOut);

            MmwDemo_magnitudeSquared(
                obj->azimuthOut,
                &obj->azimuthMagSqr[obj->numAngleBins],
                obj->numAngleBins);
        }
        
        /* Calculate XY coordinates in meters */
        if (obj->processingPath == POINT_CLOUD_PROCESSING)
        {
            MmwDemo_XYZestimation(obj, detIdx2);
        }
        else
        {
            MmwDemo_XYestimation(obj, detIdx2);
        }
    }
    
    return waitingTime;
}

/**
 *  @b Description
 *  @n
 *    The function performs an association between the pre-existing
 *  clusters and the new clusters, with the intent that the 
 *  cluster sizes are filtered. 
 *
 *  @param[in]    output             The output of the clustering algorithm.
 *  @param[in]    state              The previous clustering output.
 *  @param[in]    maxNumTrackers     The maximum number of trackers. 
 *  @param[in]    epsilon2           distance metric param for association.
 *
 *  @retval
 *      none 
 */
void associateClustering(clusteringDBscanOutput_t * restrict output,
                         clusteringDBscanReport_t * restrict state, 
                         uint16_t maxNumClusters, 
                         int32_t  epsilon2, 
                         int32_t  vFactor)
{
    uint32_t outputIdx, stateIdx; 
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t vFactorTmp;
    int32_t assocIndx = -1;
    uint32_t distSqMax;
    uint32_t distSq;
  
    for (stateIdx = 0; stateIdx < maxNumClusters; stateIdx++)
    {
        int32_t isAssociated = 0;
    
        if (state[stateIdx].numPoints == 0)
        {
            continue;
        }
        
        assocIndx = -1;
        distSqMax = UINT32_MAX;
        for (outputIdx = 0; outputIdx < output->numCluster; outputIdx++)
        {
            /* Check if it is a valid cluster. */
            if (output->report[outputIdx].numPoints == 0)
            {
                continue;
            }

            /* Check if it's velocity is close. */
            c = (state[stateIdx].avgVel - output->report[outputIdx].avgVel);

            vFactorTmp        =    vFactor;
            
            if (_abs(state[stateIdx].avgVel) < vFactorTmp)
            {
                vFactorTmp = _abs(state[stateIdx].avgVel);
            }
            
            if (_abs(c) > vFactorTmp)
            {
                continue;
            }    
                    
            /* Finally, associate using distance */
            a = (state[stateIdx].xCenter - output->report[outputIdx].xCenter);     
            b = (state[stateIdx].yCenter - output->report[outputIdx].yCenter);
            distSq = (a*a + b*b);
            
            
            if (distSq < epsilon2)
            {
                if (distSq < distSqMax)
                {
                    distSqMax = distSq;
                    assocIndx = outputIdx;
                }
            }
        }

        if (distSqMax < UINT32_MAX)
        {
           state[stateIdx].xCenter     =   output->report[assocIndx].xCenter;
           state[stateIdx].yCenter     =   output->report[assocIndx].yCenter;
           state[stateIdx].numPoints   =   output->report[assocIndx].numPoints;
           state[stateIdx].avgVel      =  output->report[assocIndx].avgVel;
            
           /* IIR filter the sizes, so that they don't change too rapidly. */
           state[stateIdx].xSize = (int16_t)(7*((int32_t)state[stateIdx].xSize) + (int32_t)(output->report[assocIndx].xSize)) >> 3;
           state[stateIdx].ySize = (int16_t)(7*((int32_t)state[stateIdx].ySize) + (int32_t)(output->report[assocIndx].ySize)) >> 3;
           
           
           /* Having been associate, clear out this report. */
           output->report[assocIndx].numPoints = 0;
           isAssociated = 1;
        }
        
        if (isAssociated == 0)
        {
            state[stateIdx].numPoints = 0;
        }
    }
    
    for (outputIdx = 0; outputIdx < output->numCluster; outputIdx++)
    {
        if (output->report[outputIdx].numPoints == 0)
        {
            /* Already associated, so ignore. */
            continue;
        }
    
        for (stateIdx = 0; stateIdx < maxNumClusters; stateIdx++)
        {
            /* Find an empty state. */
            if (state[stateIdx].numPoints == 0)
            {
                state[stateIdx] = output->report[outputIdx];
                output->report[outputIdx].numPoints = 0; 
                break;
            }
        }
    }
}


/**
 *  @b Description
 *  @n
 *    The function populates the object location arrays 
 *   for transmission to MSS. The reason we do this 
 *   additional step is to minimize the size of the 
 *   the transmission because it shouldn't saturate the 
 *   hold only the minimum information necessary for the 
 *   external GUI are populated. 
 *
 *  @param[in]    obj              data path object.
 *
 *  @retval
 *      none 
 */
 
#define TWENTY_TWO_DB_DOPPLER_SNR ((22 *(256))/6)
#define EIGHTEEN_DB_DOPPLER_SNR ((18 *(256))/6)
#define ZERO_POINT_FIVE_METERS  (0.5f * 128)
#define FOUR_POINT_ZERO_METERS  (4 * 128)
void populateOutputs(DSS_DataPathObj *obj)
{
    uint32_t ik,jk;
    float oneQFormat = (float) (1U << obj->xyzOutputQFormat);
    
    MmwDemo_detectedObjActual * restrict detObj2D = obj->detObj2D;
    MmwDemo_detectedObjForTx * restrict detObjFinal = obj->detObjFinal;
    clusteringDBscanReport_t * restrict clusterReport = obj->dbScanState;
    clusteringDBscanReportForTx * restrict clusterRepFinal = obj->clusterOpFinal;
    KFstate_t * restrict trackerState  = obj->trackerState; 
    trackingReportForTx * restrict trackerOpFinal = obj->trackerOpFinal;
    
    _nassert((uint32_t) detObjFinal % 8 == 0);
    _nassert((uint32_t) detObj2D % 8 == 0);

  
    /* 1. Detected Object List */
    for (ik = 0; ik < obj->numDetObj; ik ++)
    {
        detObjFinal[ik].speed   = detObj2D[ik].speed;         
        detObjFinal[ik].peakVal = detObj2D[ik].peakVal;      
        detObjFinal[ik].x       = detObj2D[ik].x;
        detObjFinal[ik].y       = detObj2D[ik].y;        
        detObjFinal[ik].z       = detObj2D[ik].z;        
        
#ifdef SEND_SNR_INFO
        detObjFinal[ik].rangeSNRdB   = detObj2D[ik].rangeSNRdB; 
        detObjFinal[ik].dopplerSNRdB = detObj2D[ik].dopplerSNRdB; 
#endif
    }
    
    
    /* 2. Clustering output for the point cloud subframe. */
    if (obj->processingPath == POINT_CLOUD_PROCESSING)
    {
        _nassert((uint32_t) clusterReport % 8 == 0);
        _nassert((uint32_t) clusterRepFinal % 8 == 0);
     
        jk = 0; 
        for (ik = 0; ik < obj->dbScanInstance.maxClusters; ik ++)
        {
            if (clusterReport[ik].numPoints == 0)
            {
                continue;
            }
            
            clusterRepFinal[jk].xCenter      =  clusterReport[ik].xCenter;         
            clusterRepFinal[jk].yCenter      =  clusterReport[ik].yCenter;         
            clusterRepFinal[jk].xSize        =  clusterReport[ik].xSize;         
            clusterRepFinal[jk].ySize        =  clusterReport[ik].ySize;         
            jk ++;
        }
        dssAssert(jk == obj->dbScanReport.numCluster);        
    }

    /* 3. Tracking output for the max-vel-enh subframe. */
    if (obj->processingPath == MAX_VEL_ENH_PROCESSING)
    {
        _nassert((uint32_t) trackerState % 8 == 0);
        _nassert((uint32_t) trackerOpFinal % 8 == 0);
        
        jk = 0; 

        dssAssert(obj->trackerInstance.maxTrackers == obj->dbScanInstance.maxClusters);
        for (ik = 0; ik <  obj->trackerInstance.maxTrackers; ik ++)
        {
            if (trackerState[ik].validity == 0)
            {
                continue;
            }
            
            if (trackerState[ik].tick < MIN_TICK_FOR_TX) 
            {
                continue;
            }
            
            if (trackerState[ik].age > AGED_OBJ_DELETION_THRESH) 
            {
                continue;
            }
        
            trackerOpFinal[jk].x = (int16_t) (obj->trackerState[ik].vec[iX]*oneQFormat); 
            trackerOpFinal[jk].y = (int16_t) (obj->trackerState[ik].vec[iY]*oneQFormat); 
            trackerOpFinal[jk].xd = (int16_t) (obj->trackerState[ik].vec[iXd]*oneQFormat); 
            trackerOpFinal[jk].yd = (int16_t) (obj->trackerState[ik].vec[iYd]*oneQFormat); 
            trackerOpFinal[jk].xSize        =  obj->trackerState[ik].xSize;         
            trackerOpFinal[jk].ySize        =  obj->trackerState[ik].ySize;         
            
            jk ++;         
        }
        obj->numActiveTrackers = jk;
    }

    /* 4. Parking assist : i.e Closest obstruction as a function of angle.*/
    if (obj->processingPath == POINT_CLOUD_PROCESSING) 
    {
        uint16_t range;
        int32_t sinAzim;
        int16_t binIdx, binIdxTmp, nExp;

        _nassert((uint32_t) obj->parkingAssistBins % 8 == 0);
        _nassert((uint32_t) obj->parkingAssistBinsState % 8 == 0);
        
        /* Initialize the parking grid to the maximum value. */
        for (ik = 0; ik  < obj->parkingAssistNumBins; ik++)
        {
            obj->parkingAssistBins[ik] = obj->parkingAssistMaxRange;
        }
        
        for (ik = 0; ik < obj->numDetObj; ik ++)
        {
            range = (uint16_t) detObj2D[ik].range;
     
            if (range > obj->parkingAssistMaxRange)
                continue;
            
            if (range < obj->parkingAssistMinRange)
                continue;
            
            /* Compute the angle (i.e. sin (azim) ). */ 
            sinAzim = (int32_t) detObj2D[ik].sinAzim; 
            if (sinAzim < 0) 
            {
                /* Since sinAzim can vary from -2^sinAzimQFormat to + 2^sinAzimQFormat, 
                 * convert it to a positive number. */
                sinAzim = sinAzim + (1 << (obj->sinAzimQFormat+1));
            }
            /* Quantize the angle to a number between 0 and parkingAssistNumBins */
            binIdx = (int16_t) (sinAzim >> (1 + obj->sinAzimQFormat - obj->parkingAssistNumBinsLog2));
            
            if (binIdx > obj->parkingAssistNumBins - 1)
            {
                binIdx = obj->parkingAssistNumBins - 1;
            }
            
            /* The default object obstructs only one bin. However, if the object is stationary, the
             * poor angle resolution of 4x2 antenna array needs to be compensated for. */
            nExp = 1;
            
            if (detObj2D[ik].dopplerIdx == 0)
            {
                /* If the object has zero relative velocity w.r.t the radar, the point cloud it can 
                 * generate would be quite sparse. Therefore, we use the RCS (peakVal is a function
                 * of RCS), and expand the object size based on heuristics related to peakVal.   */
                if (detObj2D[ik].dopplerSNRdB > TWENTY_TWO_DB_DOPPLER_SNR) 
                {
                    nExp = 5; 
                }
                else if (detObj2D[ik].dopplerSNRdB > EIGHTEEN_DB_DOPPLER_SNR)
                {
                    nExp = 3;
                }
                else 
                {
                    nExp = 2;
                }
            }
            
            for (jk = 0; jk < 2*nExp + 1; jk++)
            {
                binIdxTmp = binIdx - nExp + jk;
                
                if (binIdxTmp > obj->parkingAssistNumBins - 1)
                {
                    binIdxTmp = binIdxTmp  - obj->parkingAssistNumBins ;
                }
                
                if (obj->parkingAssistBins[binIdxTmp] >  range)
                {
                    obj->parkingAssistBins[binIdxTmp] = range;
                }
            }
        }

        for (ik = 0; ik  < obj->parkingAssistNumBins; ik++)
        {
            uint32_t State = obj->parkingAssistBinsState[ik];
            uint16_t StateCnt = obj->parkingAssistBinsStateCnt[ik];
            uint32_t Meas = obj->parkingAssistBins[ik];
            
            /* In general, we would like to update the parking grid instantaneously based on the 
             * current list of detected objects, however, in most cases, clutter, and inconsistent
             * detection results can give rise to cases where objects blink in and out of existance. 
             * the instantaeous output may not best reflect the scene in front of the car. Some 
             * filtering on the grid positions is necessary. The state of the grid is saved across 
             * frames and updated in a 2-stage manner.   
             *
             * The filtering is performed in two stages, the first adds hysterisis to the grid. It
             * takes into account the fact that some objects are intermittent (i.e they tend to 
             * appear and disappear), and some objects located at the boundary of two neighbouring
             * grids, and could move between the two. Each grid is assigned an age, and only 
             * updated after the age exceeds a threshold. The thresholds are different if the object
             * is before the grid or after the grid because objects appearing before a grid (i.e a 
             * a target moving closer) is considered higher priority.  There is a threshold
             *
             *
             * The 2nd stage is a simple IIR filter with different constants depending on whether the
             * updated position is before or behind the current grid location. */ 
             
             
            if ( (Meas >  State) && ((Meas - State) > ZERO_POINT_FIVE_METERS) && (StateCnt < 3) )
            {
                /* If a detected point disappears, wait for three epochs to begin updating it. */
                obj->parkingAssistBinsState[ik] = obj->parkingAssistBinsState[ik]; 
                obj->parkingAssistBins[ik] = obj->parkingAssistBinsState[ik];
                if (obj->parkingAssistBinsStateCnt[ik] < 8)
                {
                    obj->parkingAssistBinsStateCnt[ik]++; 
                }
            }
            else if ((Meas >  State) && ((Meas - State) > ZERO_POINT_FIVE_METERS) && (StateCnt < 10))
            {
                /* If a point moves away rapidly (ie 0.5 meters in about 60ms), let it update slower. */
                obj->parkingAssistBinsState[ik]  = (uint16_t) (15*(State) + Meas) >> 4;
                obj->parkingAssistBins[ik] = obj->parkingAssistBinsState[ik];
                if (obj->parkingAssistBinsStateCnt[ik] < 16)
                {
                    obj->parkingAssistBinsStateCnt[ik]++; 
                }                
            }
            else if ((Meas < State) && ((State - Meas) > FOUR_POINT_ZERO_METERS) && (StateCnt < 1))
            {
                /* If a point disappears, wait for one epoch to begin updating it. */
                obj->parkingAssistBinsState[ik] = obj->parkingAssistBinsState[ik]; 
                obj->parkingAssistBins[ik] = obj->parkingAssistBinsState[ik];
                if (obj->parkingAssistBinsStateCnt[ik] < 1)
                {
                    obj->parkingAssistBinsStateCnt[ik]++; 
                }                
            }
            else
            {
                /* Regular IIR update of the parking distance. */
                obj->parkingAssistBinsState[ik]  = (uint16_t) (3*(State) + Meas) >> 2;
                obj->parkingAssistBins[ik] = obj->parkingAssistBinsState[ik];
                obj->parkingAssistBinsStateCnt[ik]  = 0;                 
            }
        }
    }  
}


/**
 *  @b Description
 *  @n
 *    The function removes objects from extreme angles and with poor angle
 *  SNR from the tracking input. 
 *
 *  @param[in]    trackinginput             List of tracking inputs. 
 *  @param[in]    numClusters               number of tracking inputs (from the
 *                                          clustering output). 
 *  @retval
 *      number of tracking inputs after pruning. 
 */
uint32_t pruneTrackingInput(trackingInputReport_t * trackingInput, const uint32_t numCluster)
{
    uint32_t ik = 0; 
    uint32_t jk = 0; 
    
    for (ik = 0; ik < numCluster; ik ++)
    {
        if (   (_fabsf(trackingInput[ik].measVec[iSIN_AZIM]) < SIN_55_DEGREES)
            || (trackingInput[ik].measCovVec[iSIN_AZIM] < TRK_SIN_AZIM_THRESH))
        {
            if (ik > jk)
            {
                trackingInput[jk] = trackingInput[ik];
            }
            jk++;
        }
    }
    
    return jk;
}

/**
 *  @b Description
 *  @n
 *    The function performs a quadractic peak interpolation to compute the 
 *  fractional offset of the peak location. It is primarily intended to be 
 *  used in oversampled FFTs. 
 *
 *  @param[in]    y             the array of data. 
 *  @param[in]    len           length of the array. 
 *  @param[in]    indx          coarse peak location. 
 *
 *  @retval
 *      interpolated peak location (varies from -1 to +1). 
 */
float quadraticInterpFltPeakLoc(float * restrict y, int32_t len, int32_t indx)
{
    float y0 = y[indx]; 
    float ym1, yp1, thetaPk; 
    float Den;
    if (indx == len - 1)
    {
        yp1 = y[0]; 
    }
    else
    {
        yp1 = y[indx + 1];
    }
    
    if (indx == 0)
    {
        ym1 = y[len - 1]; 
    }
    else
    {
        ym1 = y[indx - 1];
    }
    
    Den = (2.0f*( (2.0f*y0) - yp1 - ym1));
    
    /* A reasonable restriction on the inverse. 
     * Note that y0 is expected to be larger than 
     * yp1 and ym1. */  
    if (Den > 0.15)
    {
        thetaPk = (yp1 - ym1)* recipsp(Den);
    }
    else
    {
        thetaPk = 0.0f;
    }
    return thetaPk;
}

/**
 *  @b Description
 *  @n
 *    The function performs a quadractic peak interpolation to compute the 
 *  fractional offset of the peak location. It is primarily intended to be 
 *  used in oversampled FFTs. The input is assumed to be an unsigned short.
 *
 *  @param[in]    y             the array of data. 
 *  @param[in]    len           length of the array. 
 *  @param[in]    indx          coarse peak location. 
 *  @param[in]    fracBitIn     fractional bits in the input array.  
 *
 *  @retval
 *      interpolated peak location (varies from -1 to +1). 
 */
float quadraticInterpLog2ShortPeakLoc(uint16_t * restrict y, int32_t len, int32_t indx, uint16_t fracBitIn)
{
    float y0 = (float)antilog2(y[indx], fracBitIn); 
    float ym1, yp1, thetaPk; 
    float Den;
    
    /* Circular shifting for finding the neighbour at the extremes. */
    if (indx == len - 1)
    {
        yp1 = antilog2(y[0], fracBitIn); 
    }
    else
    {
        yp1 = antilog2(y[indx + 1], fracBitIn);
    }
    
    if (indx == 0)
    {
        ym1 = antilog2(y[len - 1], fracBitIn); 
    }
    else
    {
        ym1 = antilog2(y[indx - 1], fracBitIn);
    }
    
    Den = (2.0f*( (2.0f*y0) - yp1 - ym1));
    
    /* A reasonable restriction on the inverse. 
     * Note that y0 is expected to be larger than 
     * yp1 and ym1. */  
    if (Den > 0.15)
    {
        /* Compute the interpolated maxima as 
         *   thetaPk = (yp1 - ym1)/(2.0f*( (2.0f*y0) - yp1 - ym1))
         */   
        thetaPk = (yp1 - ym1)* recipsp(Den);
    }
    else
    {
        thetaPk = 0.0f;
    }
    
    return thetaPk;
}


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    MmwDemo_DopplerCompensation
 *
 * \par
 * <b>Description</b>  : Compensation of Doppler phase shift in the virtual antennas,
 *                       (corresponding to second Tx antenna chirps). Symbols
 *                       corresponding to virtual antennas, are rotated by half
 *                       of the Doppler phase shift measured by Doppler FFT.
 *                       The phase shift read from the table using half of the
 *                       object Doppler index  value. If the Doppler index is
 *                       odd, an extra half of the bin phase shift is added.
 *
 * @param[in]               dopplerIdx     : Doppler index of the object
 * @param[in]               numDopplerBins : Number of Doppler bins
 * @param[in]               azimuthModCoefs: Table with cos/sin values SIN in even position, COS in odd position
 *                                           exp(1j*2*pi*k/N) for k=0,...,N-1 where N is number of Doppler bins.
 * @param[out]              azimuthModCoefsHalfBin :  exp(1j*2*pi* 0.5 /N) //TODO change to 1/3 instead of 1/2 for the correction.
 * @param[in,out]           azimuthIn        :Pointer to antenna symbols to be Doppler compensated
 * @param[in]              numAnt       : Number of antenna symbols to be Doppler compensated
 *
 * @return                  void
 *
 *******************************************************************************************************************
 */
void MmwDemo_addDopplerCompensation(int32_t dopplerIdx,
                                    int32_t numDopplerBins,
                                    uint32_t *azimuthModCoefs,
                                    uint32_t *azimuthModCoefsThirdBin,
									uint32_t *azimuthModCoefsTwoThirdBin,
                                    int64_t *azimuthIn,
                                    uint32_t numAnt,
                                    uint32_t numTxAnt,
                                    uint16_t txAntIdx)
{
    uint32_t expDoppComp;
    int32_t dopplerCompensationIdx = dopplerIdx;
    int64_t azimuthVal;
    int32_t Re, Im;
    uint32_t antIndx;

    if (numAnt == 0)
    {
        return;
    }

    /*Divide Doppler index by 2*/
    if (dopplerCompensationIdx >= (numDopplerBins >> 1))
    {
        dopplerCompensationIdx -=  (int32_t)numDopplerBins;
    }
   // dopplerCompensationIdx = (dopplerCompensationIdx >> 1);
   /* Doppler phase correction is 1/2 or (1/3 in elevation case) of the phase between two chirps of the same antenna */
    dopplerCompensationIdx = ((dopplerCompensationIdx * txAntIdx )/numTxAnt);
    
    if (dopplerCompensationIdx < 0)
    {
        dopplerCompensationIdx +=  (int32_t) numDopplerBins;
    }
    expDoppComp = azimuthModCoefs[dopplerCompensationIdx];
	/* Add third bin rotation if Doppler index was odd */
    if ((dopplerIdx%numTxAnt) == 1)
    {
        expDoppComp = _cmpyr1(expDoppComp, *azimuthModCoefsThirdBin);
    }
	else if((dopplerIdx%numTxAnt) == 2)
	{
		expDoppComp = _cmpyr1(expDoppComp, *azimuthModCoefsTwoThirdBin);
	}

    /* Rotate symbols */
    for (antIndx = 0; antIndx < numAnt; antIndx++)
    {
        azimuthVal = _amem8(&azimuthIn[antIndx]);
        Re = _ssub(_mpyhir(expDoppComp, _loll(azimuthVal) ),
                    _mpylir(expDoppComp, _hill(azimuthVal)));
        Im = _sadd(_mpylir(expDoppComp, _loll(azimuthVal)),
                    _mpyhir(expDoppComp, _hill(azimuthVal)));
        _amem8(&azimuthIn[antIndx]) =  _itoll(Im, Re);
    }
}

//#ifdef COMPENSATE_FOR_GAIN_PHASE_OFFSET
/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    MmwDemo_rxChanPhaseBiasCompensation
 *
 * \par
 * <b>Description</b>  : Compensation of rx channel phase bias
 *
 * @param[in]               rxChComp : rx channel compensation coefficient
 * @param[in]               input : 32-bit complex input symbols must be 64 bit aligned
 * @param[in]               numAnt : number of symbols
 *
 * @return                  void
 *
 *******************************************************************************************************************
 */
static inline void MmwDemo_rxChanPhaseBiasCompensation(uint32_t *rxChComp,
                                         int64_t *input,
                                         uint32_t numAnt)
{
    int64_t azimuthVal;
    int32_t Re, Im;
    uint32_t antIndx;
    uint32_t rxChCompVal;


    /* Compensation */
    for (antIndx = 0; antIndx < numAnt; antIndx++)
    {
        azimuthVal = _amem8(&input[antIndx]);

        rxChCompVal = _amem4(&rxChComp[antIndx]);
        
        

        Re = _ssub(_mpyhir(rxChCompVal, _loll(azimuthVal) ),
                    _mpylir(rxChCompVal, _hill(azimuthVal)));
        Im = _sadd(_mpylir(rxChCompVal, _loll(azimuthVal)),
                    _mpyhir(rxChCompVal, _hill(azimuthVal)));
        _amem8(&input[antIndx]) =  _itoll(Im, Re);
    }
}
//#endif

/**
 *  @b Description
 *  @n
 *      Calculates X/Y coordinates in meters based on the maximum position in
 *      the magnitude square of the azimuth FFT output. The function is called
 *      per detected object. 
 *
 *  @param[in] obj                Pointer to data path object
 *  @param[in] objIndex           Detected object index
 *  @param[in] azimIdx            Index of the peak position in Azimuth FFT output
 *  @param[in] azimuthMagSqr      azimuth energy array
 *
 *  @retval
 *      NONE
 */
void MmwDemo_XYcalc (DSS_DataPathObj *obj,
                     uint32_t objIndex,
                     uint16_t azimIdx,
                     float * azimuthMagSqr)
{
    int32_t sMaxIdx;
    float temp;
    float Wx, offset, sMaxIdxFlt;
    float range;
    float x, y;
    uint32_t xyzOutputQFormat = obj->xyzOutputQFormat;
    uint32_t oneQFormat = (1 << xyzOutputQFormat);
    float invOneQFormat = obj->invOneQFormat;
    uint32_t sinAzimOneQFormat = (1 << obj->sinAzimQFormat);
    
    uint32_t numAngleBins = obj->numAngleBins;
    
    /* Calculate X and Y coordiantes in meters in Q8 format */
    range = ((float) obj->detObj2D[objIndex].range) * invOneQFormat;

    if (azimIdx > ((numAngleBins >> 1) - 1))
    {
        sMaxIdx = azimIdx - numAngleBins;
    }
    else
    {
        sMaxIdx = azimIdx;
    }
    
    /* Add in the offset from the quadratic interpolation. */
    offset =  quadraticInterpFltPeakLoc(azimuthMagSqr, numAngleBins, azimIdx);
    sMaxIdxFlt = ((float)sMaxIdx) + offset;

    Wx = 2 * sMaxIdxFlt *obj->invNumAngleBins;
    x = range * Wx;

    /* y = sqrt(range^2 - x^2) */
    temp = range*range - x*x;
    if (temp > 0)
    {
        y = sqrtsp(temp);
    }
    else
    {
        y = 0;
    }

    /* Convert to Q8 format */
    obj->detObj2D[objIndex].x = (int16_t)ROUNDF(x * oneQFormat);
    obj->detObj2D[objIndex].y = (int16_t)ROUNDF(y * oneQFormat);
    obj->detObj2D[objIndex].z = 0;
    obj->detObj2D[objIndex].sinAzim = (int16_t)ROUNDF(Wx * sinAzimOneQFormat);
    obj->detObj2D[objIndex].sinAzimSNRLin = computeSinAzimSNR(azimuthMagSqr, azimIdx, obj->numVirtualAntAzim, obj->numAngleBins, obj->xyzOutputQFormat);
}

/**
 *  @b Description
 *  @n
 *      Calculates X/Y coordinates in meters based on the maximum position in
 *      the magnitude square of the azimuth FFT output. The function is called
 *      per detected object. 
 *
 *  @param[in] obj                Pointer to data path object
 *  @param[in] objIndex           Detected object index
 *  @param[in] azimIdx            Index of the peak position in Azimuth FFT output
 *  @param[in] azimuthMagSqr      azimuth energy array
 *
 *  @retval
 *      NONE
 */
void MmwDemo_XYZcalc (DSS_DataPathObj *obj,
                     uint32_t objIndex,
                     uint16_t azimIdx,
                     float * azimuthMagSqr)
{
    int32_t sMaxIdx;
    float temp;
    float Wx, offset, sMaxIdxFlt, Wz;
    float range;
    float x, y , z;
    uint32_t xyzOutputQFormat = obj->xyzOutputQFormat;
    uint32_t oneQFormat = (1 << xyzOutputQFormat);
    float invOneQFormat = obj->invOneQFormat;
    uint32_t sinAzimOneQFormat = (1 << obj->sinAzimQFormat);
    //int32_t *azimFFTPtr, *elevFFTPtr;
    float peakAzimRe, peakAzimIm, peakElevRe, peakElevIm;
	float tempRe, tempIm;
    
    
    uint32_t numAngleBins = obj->numAngleBins;
    
    /* Calculate X, Y and Z coordiantes in meters in Q8 format */
    range = ((float) obj->detObj2D[objIndex].range) * invOneQFormat;

    if (azimIdx > ((numAngleBins >> 1) - 1))
    {
        sMaxIdx = azimIdx - numAngleBins;
    }
    else
    {
        sMaxIdx = azimIdx;
    }
    
    /* Add in the offset from the quadratic interpolation. */
    offset =  quadraticInterpFltPeakLoc(azimuthMagSqr, numAngleBins, azimIdx);
    sMaxIdxFlt = ((float)sMaxIdx) + offset;

    Wx = 2 * sMaxIdxFlt *obj->invNumAngleBins;
    
    x = range * Wx;

        
        peakAzimIm = (float) obj->azimuthOut[azimIdx].imag;
        peakAzimRe = (float) obj->azimuthOut[azimIdx].real;

        peakElevIm = (float) obj->elevationOut[azimIdx].imag;
        peakElevRe = (float) obj->elevationOut[azimIdx].real;
    
	tempIm = (peakAzimIm * peakElevRe) - (peakAzimRe * peakElevIm);
	tempRe = (peakAzimRe * peakElevRe) + (peakAzimIm * peakElevIm);
	
    Wz = (float) atan2sp(tempIm, tempRe) * (1.0f/PI_) ;
        if (Wz > 1)
        {
            Wz = Wz - 2;
        }
        else if (Wz < -1)
        {
            Wz = Wz + 2;
        }
        z = range * Wz;               

    
    /* y = sqrt(range^2 - x^2) */
    temp = (range*range) - (x*x) - (z*z);
    if (temp > 0)
    {
        y = sqrtsp(temp);
    }
    else
    {
        y = 0;
    }

    /* Convert to Q8 format */
    obj->detObj2D[objIndex].x = (int16_t)ROUNDF(x * oneQFormat);
    obj->detObj2D[objIndex].y = (int16_t)ROUNDF(y * oneQFormat);
    obj->detObj2D[objIndex].z = (int16_t)ROUNDF(z * oneQFormat);
    obj->detObj2D[objIndex].sinAzim = (int16_t)ROUNDF(Wx * sinAzimOneQFormat);
    obj->detObj2D[objIndex].sinAzimSNRLin = computeSinAzimSNR(azimuthMagSqr, azimIdx, obj->numVirtualAntAzim, obj->numAngleBins, obj->xyzOutputQFormat);
}


/**
 *  @b Description
 *  @n
 *      Initialize the 'parking assist bins' state which is essentially the 
 *  closest obstruction upper bounded by its maximum value. 
 *
 *  @param[in] obj                Pointer to data path object
 *
 *  @retval
 *      NONE
 */
void parkingAssistInit(DSS_DataPathObj *obj)
{
    uint32_t ik; 
    for (ik = 0; ik < obj->parkingAssistNumBins; ik++)
    {
        obj->parkingAssistBinsState[ik] = obj->parkingAssistMaxRange;
        obj->parkingAssistBinsStateCnt[ik] = 0; 
    }
}

/**
 *  @b Description
 *  @n
 *      A slightly weaker implementation of the 'pruneToPeaks' 
 *      algorithm. 
 *      This variation passes peaks as well as their largest neighbour. 
 *      
 *      
 *  @param[in,out] cfarDetObjIndexBuf  The indices of the detected objects.
 *  @param[in,out] cfarDetObjSNR      The SNR of the detected objects.
 *  @param[in]  numDetObjPerCfar   The number of detected objects. 
 *  @param[in]  sumAbs             The sumAbs array on which the CFAR was run.
 *  @param[in]  numBins            The length of the sumAbs array.
 *
 *  @retval
 *      NONE
 */
uint32_t pruneToPeaksOrNeighbourOfPeaks(uint16_t* restrict cfarDetObjIndexBuf, 
                      uint16_t* restrict cfarDetObjSNR, 
                      uint32_t numDetObjPerCfar, 
                      uint16_t* restrict sumAbs, 
                      uint16_t numBins)
{
    
    uint32_t detIdx2;
    uint32_t numDetObjPerCfarActual = 0;
    uint16_t currObjLoc, prevIdx, nextIdx;
    uint16_t prevPrevIdx, nextNextIdx;
    uint16_t is_peak, is_neighbourOfPeakNext, is_neighbourOfPeakPrev;
    
    if (numDetObjPerCfar == 0)
    {
        return 0;
    }
    /* Prune to peaks or neighbours of peaks. */
    for (detIdx2 = 0; detIdx2 < numDetObjPerCfar; detIdx2++)
    {
        currObjLoc = cfarDetObjIndexBuf[detIdx2];
    
        if (currObjLoc == 0)
        {
            prevIdx = numBins - 1;
        }
        else
        {
            prevIdx = currObjLoc - 1;
        }
        
        if (prevIdx == 0)
        {
            prevPrevIdx = numBins - 1;
        }
        else
        {
            prevPrevIdx = prevIdx ;
        }
    
        if (currObjLoc == numBins - 1)
        {
            nextIdx = 0;
        }
        else
        {
            nextIdx = currObjLoc + 1;
        }
    
        if (nextIdx == numBins - 1)
        {
            nextNextIdx = 0;
        }
        else
        {
            nextNextIdx = currObjLoc + 1;
        }
        
        is_peak = ((sumAbs[nextIdx] < sumAbs[currObjLoc]) && (sumAbs[prevIdx] < sumAbs[currObjLoc]));
        is_neighbourOfPeakNext = ((sumAbs[nextNextIdx] < sumAbs[currObjLoc]) && (sumAbs[prevIdx] < sumAbs[currObjLoc]));
        is_neighbourOfPeakPrev = ((sumAbs[nextIdx] < sumAbs[currObjLoc]) && (sumAbs[prevPrevIdx] < sumAbs[currObjLoc]));
        if (is_peak || is_neighbourOfPeakNext || is_neighbourOfPeakPrev)
        {
            cfarDetObjIndexBuf[numDetObjPerCfarActual] = currObjLoc;
            cfarDetObjSNR[numDetObjPerCfarActual] = cfarDetObjSNR[detIdx2];
            numDetObjPerCfarActual++;
        }
    }

    return numDetObjPerCfarActual;
}
