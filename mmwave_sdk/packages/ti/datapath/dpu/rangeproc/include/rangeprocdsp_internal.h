/*
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
/**
 *   @file  rangeprocdsp_internal.h
 *
 *   @brief
 *      Implements Data path processing functionality.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef RANGEPROCDSP_INTERNAL_H
#define RANGEPROCDSP_INTERNAL_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* mmWave SDK include files */
#include <ti/utils/cycleprofiler/cycle_profiler.h>

#include <ti/datapath/dpu/rangeproc/rangeprocdsp.h>
#include <ti/datapath/dpu/rangeproc/include/rangeproc_internal.h>

/* C674x mathlib */
/* Suppress the mathlib.h warnings
 *  #48-D: incompatible redefinition of macro "TRUE"
 *  #48-D: incompatible redefinition of macro "FALSE"
 */
#pragma diag_push
#pragma diag_suppress 48
#include <ti/mathlib/mathlib.h>
#pragma diag_pop

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  RangeProcDSP DPU Object
 *
 * @details
 *  The structure is used to hold RangeProcDSP DPU object
 *
 *  \ingroup DPU_RANGEPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct rangeProcDSPObj_t
{
    /*! @brief     Data path common parameters */
    rangeProc_dpParams      DPParams;

    /*! @brief      DC range signature calibration counter */
    uint16_t                dcRangeSigCalibCntr;

    /*! @brief      Chirp counter modulo number of chirps per frame */
    uint16_t                chirpCount;

    /*! @brief     ADC data buffer RX channel offset - fixed for all channels */
    uint16_t                rxChanOffset;

    /*! @brief     EDMA Handle */
    EDMA_Handle             edmaHandle;

    /*! @brief     Calibrate DC (zero) range signature Configuration */
    DPU_RangeProc_CalibDcRangeSigCfg   calibDcRangeSigCfg;

    /*! @brief     DMA channel trigger after HWA processing is done */
    uint8_t                 calibDcNumLog2AvgChirps;

    /*! @brief     Pointer to ADC buffer - this is the only format supported */
    cmplx16ImRe_t           *ADCdataBuf;

    /*! @brief     Pointer to Radar Cube buffer - this is the only format supported */
    cmplx16ImRe_t           *radarCubebuf;

    /*! @brief      Window coefficients for 1D FFT */
    int16_t                 *window;

    /*! @brief      Twiddle table for 1D FFT */
    cmplx16ImRe_t           *twiddle16x16;

    /*! @brief      ADCBUF input samples in scratch memory */
    cmplx16ImRe_t           *adcDataIn;

    /*! @brief      FFT1D out buffer in scratch memory */
    cmplx16ImRe_t           *fftOut1D;

    /*! @brief     Pointer to Calibrate DC Range signature buffer */
    cmplx32ImRe_t           *dcRangeSigMean;

    /*! @brief     DC range calibration scratch buffer size */
    uint32_t                dcRangeSigMeanBufSize;

    /*! @brief     Number of samples per chirp */
    uint32_t                numSamplePerChirp;

    /*! @brief     Number of samples per Tx */
    uint32_t                numSamplePerTx;

    /*! @brief      Data In EDMA channel id */
    uint8_t                 dataInChan[2];

    /*! @brief      Data Out EDMA channel id */
    uint8_t                 dataOutChan[2];

    /*! @brief     rangeProc DPU is in processing state */
    bool                    inProgress;

    /*! @brief     Total number of rangeProc DPU processing */
    uint32_t                numProcess;
}rangeProcDSPObj;

#ifdef __cplusplus
}
#endif

#endif
