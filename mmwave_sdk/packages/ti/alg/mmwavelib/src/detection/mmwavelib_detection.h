 /*
 * (C) Copyright 2016, Texas Instruments Incorporated -  http://www.ti.com/
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
 *   @file     mmwavelib_detection.h
 *
 *   @brief    Header file for mmwavelib routines related to detection 
 */

#ifndef MMWAVELIB_DETECTION_H
#define MMWAVELIB_DETECTION_H 1


#define CFAR_CA 0       /**< CA-CFAR type: cell average*/
#define CFAR_CAGO 1U    /**< CA-CFAR type: greater of two side noise variances*/
#define CFAR_CASO 2U    /**< CA-CFAR type: smaller of two side noise variances*/
#define CFAR_CACC 3U    /**< CA-CFAR type: cell accumulation*/

#define WINDOW_SIZE 32   /**< OS(Ordered Statistics) CFAR search window size(Multiple of 8)*/
#define N_FRAC_BITS_ALPHA 6 /**< OS CFAR: Q-format of the threshold scaling factor alpha */


void mmwavelib_log2Abs32(const int32_t inp[restrict],
                         uint16_t out[restrict], uint32_t len);
void mmwavelib_log2Abs16(const int16_t inp[restrict],
                         uint16_t out[restrict], uint32_t len);
uint32_t mmwavelib_cfarCa(const uint16_t inp[restrict],
                          uint16_t out[restrict], uint32_t len,
                          uint32_t const1, uint32_t const2,
                          uint32_t guardLen, uint32_t noiseLen);
uint32_t mmwavelib_cfarCadB(const uint16_t inp[restrict],
                            uint16_t out[restrict], uint32_t len,
                            uint32_t const1, uint32_t const2,
                            uint32_t guardLen, uint32_t noiseLen);
uint32_t mmwavelib_cfarCadBwrap(const uint16_t inp[restrict],
                                uint16_t out[restrict], uint32_t len,
                                uint32_t const1, uint32_t const2,
                                uint32_t guardLen, uint32_t noiseLen);
uint32_t mmwavelib_cfarCadB_SOGO(const uint16_t inp[restrict],
                            uint16_t out[restrict], uint32_t len,
                            uint16_t cfartype,
                            uint32_t const1, uint32_t const2,
                            uint32_t guardLen, uint32_t noiseLen);

uint16_t mmwavelib_cfarOS(const uint16_t * restrict data,
                        const uint32_t len,
                        const uint32_t lenCUTend,
                        const int16_t k,
                        const int16_t alpha,
                        uint16_t * restrict detectedList,
                        uint16_t * restrict kosList);

#endif /* MMWAVELIB_DETECTION_H */
