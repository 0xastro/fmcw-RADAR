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
 *   @file     mmwavelib_vecutils.h
 *
 *   @brief    Header file for vector utility mmwavelib library routines
 */

#ifndef MMWAVELIB_VECUTILS_H
#define MMWAVELIB_VECUTILS_H 1


void mmwavelib_accum16(const uint16_t inp[restrict], uint16_t out[restrict],
                       uint32_t len);

void mmwavelib_accum16to32(const int16_t input[restrict],
                           int32_t output[restrict],
                           uint32_t len);

void mmwavelib_vecsum(const int16_t input[restrict],
                      int32_t output[restrict],
                      int32_t ncplx);

void mmwavelib_vecsubc(int16_t *restrict input,
                       int16_t *restrict output,
                       const uint32_t subval,
                       int32_t ncplx);

void mmwavelib_vecsub16( int16_t *restrict input1,
                         int16_t *restrict input2,
                         int16_t *restrict output,
                         int32_t len);

void mmwavelib_shiftright16(int16_t input[restrict],
                            int16_t output[restrict],
                            uint8_t numshift,
                            uint32_t len);

void mmwavelib_shiftright32(int32_t input[restrict],
                            int32_t output[restrict],
                            uint8_t numshift,
                            uint32_t len);

void mmwavelib_shiftright32to16(int32_t input[restrict],
                                int16_t output[restrict],
                                uint8_t numshift,
                                uint32_t len);

void mmwavelib_histogram(uint16_t input[restrict],
                         uint16_t hist[restrict],
                         uint16_t histTemp[restrict],
                         uint32_t inpLen,
                         uint32_t histLen,
                         uint8_t numshift);

int32_t mmwavelib_vecsumabs(const int16_t input[], int32_t len);

void mmwavelib_vecmul16x16(int16_t * restrict x,
                           const int16_t * restrict w,
                           uint32_t nx);

void mmwavelib_vecmul16x32(const int16_t * restrict x,
                           int32_t * restrict w,
                           int32_t * restrict y,
                           uint32_t nx);

void mmwavelib_vecmul16x32_anylen(const uint32_t * restrict x,
                             int64_t * restrict w,
                             int64_t * restrict y,
                             uint32_t len);

void mmwavelib_vecmul32x16c(const uint32_t x,
                       int64_t * restrict w,
                       int64_t * restrict y,
                       uint32_t len);

int32_t mmwavelib_maxpow(const int32_t * restrict input,
                         int32_t Ncplx,
                         float32_t * maxpow);

int32_t mmwavelib_powerAndMax(const int32_t * restrict input,
                              int32_t Ncplx,
                              float32_t * restrict outpowbuf,
                              float32_t * maxpow);

uint16_t mmwavelib_multiPeakSearch(float32_t * x,
                                   uint16_t  * peakIdxBuf,
                                   int32_t     len,
                                   float32_t   thresholdScale);

int32_t mmwavelib_secondPeakSearch(float32_t * x,
                                   int32_t   len,
                                   int32_t   maxIdx);

void mmwavelib_power(int32_t * restrict input,
                     int32_t Ncplx,
                     float32_t * outpowbuf);

void mmwavelib_power_float(float32_t * restrict input,
                           int32_t Ncplx,
                           float32_t * restrict outpowbuf);

void mmwavelib_dcRemovalFloat(uint32_t ncplx,
                              float32_t * input,
                              float32_t * output);

#endif   /* _MMWAVELIB_VECUTILS_H_ */
