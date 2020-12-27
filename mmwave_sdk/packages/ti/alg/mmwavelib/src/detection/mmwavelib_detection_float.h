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
 *   @file     mmwavelib_detection_float.h
 *
 *   @brief    Header file for floating point detection mmwavelib routines
 */
#ifndef MMWAVELIB_DETECTION_FLOAT_H
#define MMWAVELIB_DETECTION_FLOAT_H 1

uint32_t mmwavelib_cfarfloat_caall(float32_t * InputPower,
                                   uint32_t  fftSize,
                                   uint8_t   cfarType,
                                   uint32_t  guardLen,
                                   uint32_t  searchLen,
                                   uint32_t  leftSkipLen,
                                   uint32_t  rightSkipLen,
                                   float32_t relThr,
                                   uint32_t * index,
                                   float32_t* noise);

uint32_t mmwavelib_cfarfloat_caall_opt(float32_t * InputPower,
                                   uint32_t  fftSize,
                                   uint8_t   cfarType,
                                   uint32_t  guardLen,
                                   uint32_t  searchLen,
                                   uint32_t  leftSkipLen,
                                   uint32_t  rightSkipLen,
                                   float32_t relThr,
                                   uint32_t * index,
                                   float32_t* noise);

uint32_t mmwavelib_cfarfloat_wrap(float32_t * InputPower,
                                  uint32_t  fftSize,
                                  uint8_t   cfarType,
                                  uint32_t  guardLen,
                                  uint32_t  searchLen,
                                  float32_t relThr,
                                  uint32_t * index,
                                  float32_t* noise);

uint32_t mmwavelib_cfarfloat_wrap_opt(float32_t * InputPower,
                                      uint32_t  fftSize,
                                      uint8_t   cfarType,
                                      uint32_t  guardLen,
                                      uint32_t  searchLen,
                                      float32_t relThr,
                                      uint32_t * index,
                                      float32_t* noise);

uint8_t mmwavelib_cfarfloat_point(float32_t * InputPower,
                                  uint32_t fftSize,
                                  uint8_t cfarType,
                                  uint32_t guardLen,
                                  uint32_t searchLen,
                                  float32_t relThr,
                                  uint32_t index,
                                  float32_t * noise);


#endif                          /* MMWAVELIB_DETECTION_FLOAT_H */
