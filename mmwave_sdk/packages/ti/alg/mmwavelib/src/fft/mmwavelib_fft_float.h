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
 *   @file     mmwavelib_fft_float.h
 *
 *   @brief    Header file for floating point FFT utility routines
 */


#ifndef MMWAVELIB_FFT_FLOAT_H
#define MMWAVELIB_FFT_FLOAT_H 1


void mmwavelib_windowing1DFltp(
                              uint32_t nSamplesValidPerChirp,
                              uint32_t fftSize1D,
                              int16_t  fftWin1D[restrict],
                              int16_t * inputPtr,
                              float32_t * outputPtr);

void mmwavelib_windowing2DFltp(
                            uint32_t fftSize1D,
                            float32_t inWin,
                            float32_t inOutPtr[restrict]);


void  mmwavelib_chirpProcWin2DFxdpinFltOut(
                              uint32_t     fftSize1D,
                              int16_t     inWin,
                              void         * inoutPtr);


void  mmwavelib_chirpProcWin2DFxdpinFltOut_IQswap(
                              uint32_t     fftSize1D,
                              int16_t      inWin,
                              void         * inoutPtr);


void mmwavelib_dopplerProcWin2DFxdpinFltOut(
                              uint32_t length,
                              int16_t * inputPtr,
                              int16_t * restrict inWinFunc,
                              float32_t * outputPtr);


void mmwavelib_dopplerProcWin2DFxdpinFltOut_IQswap(
                              uint32_t length,
                              int16_t * inputPtr,
                              int16_t * restrict inWinFunc,
                              float32_t * outputPtr);


#endif /* MMWAVELIB_FFT_FLOAT_H_ */
