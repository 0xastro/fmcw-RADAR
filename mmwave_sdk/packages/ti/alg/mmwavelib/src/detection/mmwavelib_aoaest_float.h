/*
 * (C) Copyright 2017, Texas Instruments Incorporated -  http://www.ti.com/
 *-------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 *   @file     mmwavelib_aoaest_float.h
 *
 *   @brief    Header file for floating point AOA estimation mmwavelib routines
 */


#ifndef MMWAVELIB_AOAEST_FLOAT_H    
#define MMWAVELIB_AOAEST_FLOAT_H 1

#include <ti/alg/mmwavelib/include/mmwavelib_cplx_types.h>

/*internal structure */
typedef struct 
{
    uint32_t  peakLoc;          /**< peak location */
    float32_t peakValue;        /**< peak value */
    uint32_t  peakWidth;        /**< peak gamma bandwidth */
}mmwavelib_aoAEstBF_peakRecord_t;


int32_t  mmwavelib_aoaEstBFSinglePeak(
                              uint8_t numAnt,
                              float32_t   noise,
                              float32_t   estResolution,
                              cplxf_t sigIn[],
                              int32_t steeringVecSize,
                              cplxf_t * steeringVec,
                              float32_t scratchPad[],
                              float32_t * estVar,
                              int32_t * angleEst);

int32_t mmwavelib_aoaEstBFSinglePeakDet(
                             uint8_t numAnt,
                             cplxf_t sigIn[],
                             int32_t steeringVecSize,
                             cplxf_t * steeringVec,
                             int32_t * angleEst);


int32_t mmwavelib_aoaEstBFMultiPeak(
                                uint8_t numAnt,
                                float32_t gamma,
                                float32_t sidelobeLevel,
                                float32_t widthAdjust_3dB,
                                float32_t inputSNR,
                                float32_t estResolution,
                                cplxf_t * sigIn,
                                cplxf_t * steeringVec,
                                int32_t steeringVecSize,
                                float32_t * scratchPad,
                                float32_t * estVar,
                                int32_t * angleEst);


int32_t mmwavelib_aoaEstBFMultiPeakDet(
                                       uint8_t numAnt,
                                       float32_t gamma,
                                       float32_t sidelobeLevel,
                                       cplxf_t * sigIn,
                                       cplxf_t * steeringVec,
                                       int32_t steeringVecSize,
                                       float32_t * scratchPad,
                                       int32_t * angleEst);

#endif  /* MMWAVELIB_AOAEST_FLOAT_H */
