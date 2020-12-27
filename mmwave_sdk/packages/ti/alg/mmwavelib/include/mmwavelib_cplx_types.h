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
 *   @file     mmwavelib_cplx_types.h
 *
 *   @brief    header file contains several structures for complex values, used in MMWAVELIB
 *
 */

#ifndef _CPLX_TYPES

#define  _CPLX_TYPES

#ifdef _LITTLE_ENDIAN
typedef struct _CPLX8
{
    int8_t imag;
    int8_t real;
} cplx8_t;

typedef struct _CPLX16
{
    int16_t imag;
    int16_t real;
} cplx16_t;

typedef struct _CPLX32
{
    int32_t imag;
    int32_t real;
} cplx32_t;

typedef struct _CPLXF
{
    float32_t imag;
    float32_t real;
} cplxf_t;
#endif /* _LITTLE_ENDIAN */

#ifdef _BIG_ENDIAN
typedef struct _CPLX8
{
    int8_t real;
    int8_t imag;
} cplx8_t;

typedef struct _CPLX16
{
    int16_t real;
    int16_t imag;
} cplx16_t;

typedef struct _CPLX32
{
    int32_t real;
    int32_t imag;
} cplx32_t;

typedef struct _CPLXF
{
    float32_t real;
    float32_t imag;
} cplxf_t;
#endif /* _BIG_ENDIAN */

/*
typedef union _CPLX32U
{
    cplx32_t cplx32;
    uint64_t realimag;
} cplx32u_t;

typedef union _CPLX16U
{
    cplx16_t cplx16;
    uint32_t realimag;
} cplx16u_t;

*/
#endif /* _CPLX_TYPES */
