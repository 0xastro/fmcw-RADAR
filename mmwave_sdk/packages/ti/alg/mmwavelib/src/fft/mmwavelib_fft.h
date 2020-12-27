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
 *   @file     mmwavelib_fft.h
 *
 *   @brief    Header file for FFT utility routines
 */

#ifndef MMWAVELIB_FFT_H
#define MMWAVELIB_FFT_H 1

#include <stdint.h>
#include <c6x.h>

extern const int16_t mmwavelib_dftHalfBinExpValue[2*9];

/* window coefficients data format   */
/*! window coefficient data format is int16_t */
#define MMWAVELIB_WINDOW_INT16 0
/*! window coefficient data format is int32_t */
#define MMWAVELIB_WINDOW_INT32 1

/* FFT Window Types */
/*! Hanning window */
#define MMWAVELIB_WIN_HANNING  0
/*! Blackman window */
#define MMWAVELIB_WIN_BLACKMAN 1U
/*! Rectangular window */
#define MMWAVELIB_WIN_RECT     2U
/*! Hamming window */
#define MMWAVELIB_WIN_HAMMING  3U



void mmwavelib_windowing16x32(const int16_t inp[restrict],
                              const int32_t win[restrict],
                              int32_t out[restrict], uint32_t len);


void mmwavelib_windowing16x32_IQswap(const int16_t inp[restrict],
                              const int32_t win[restrict],
                              int32_t out[restrict], uint32_t len);

void mmwavelib_windowing16x16(int16_t inp[restrict],
                              const int16_t win[restrict], uint32_t len);

void mmwavelib_windowing16x16_evenlen(int16_t inp[restrict],
                                      const int16_t win[restrict],
                                      uint32_t len);
									  
void mmwavelib_windowCoef_gen(void *  winCoef,
                               uint32_t windowDatumType,
                              float initR,
                              float initI,
                              uint32_t winCoefLen,
                              int32_t  Qformat,
                              uint32_t winType);

void mmwavelib_fft16ptZeroPadded32x32(const int32_t * restrict inp,
                                      const int32_t * restrict twiddle,
                                      int32_t * restrict temp,
                                      int32_t * restrict out,
                                      uint32_t numLines,
                                      uint32_t numSkipSamples);

void mmwavelib_dftSingleBin(const uint32_t inBuf[restrict],
                            const uint32_t sincos[restrict],
                            uint64_t * restrict output,
                            uint32_t length, uint32_t doppInd);

void mmwavelib_dftSingleBinWithWindow(
                   const uint32_t inBuf[restrict],
                   const uint32_t sincos[restrict],
                   const int32_t win[restrict],
                   uint64_t * restrict output,
                   uint32_t length,
                   uint32_t doppInd);


void mmwavelib_dftSingleBinWithWindow_ReIm(
                   const uint32_t inBuf[restrict],
                   const uint32_t sincos[restrict],
                   const int32_t win[restrict],
                   uint64_t * restrict output,
                   uint32_t length,
                   uint32_t doppInd);

void mmwavelib_dftSinCosTabelGen(uint32_t dftLength, uint32_t *dftSinCosTbl);

int32_t mmwavelib_gen_twiddle_fft16x16(int16_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_fft16x16_imre_sa(int16_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_smallfft_16x16(int16_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_smallfft_16x16_imre_sa(int16_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_largefftUp8k_16x16(int16_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_largefft_16x16(int16_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_largefft_16x16_imre_sa(int16_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_fft32x32(int32_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_smallfft_32x32(int32_t *w, int32_t n);
int32_t mmwavelib_gen_twiddle_largefft_32x32(int32_t *w, int32_t n);
#endif                          /* _MMWAVELIB_FFT_H_ */
