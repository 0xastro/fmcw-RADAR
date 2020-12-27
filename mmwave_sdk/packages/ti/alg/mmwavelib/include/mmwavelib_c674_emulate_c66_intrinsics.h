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
 *   @file     mmwavelib_c674_emulate_c66_intrinsics.h
 *
 *   @brief    header file contains inline functions for C674x used in MMWAVELIB,
*              these functions implement TI compiler intrinsics which are for C66x only. 
 *             
 *
 */

 
#ifndef _RADARC674X_H
#define _RADARC674X_H

#include <c6x.h>
#ifndef _TMS320C6600
static inline __float2_t _complex_conjugate_mpysp(__float2_t x, __float2_t y)
{
	float32_t zreal, zimg;
	zreal = _hif2(x) * _hif2(y) + _lof2(x) * _lof2(y);
	zimg = _hif2(x) * _lof2(y) - _lof2(x) * _hif2(y);
	return(_ftof2(zreal, zimg));
}

static inline __float2_t _complex_mpysp(__float2_t x, __float2_t y)
{
	float32_t zreal, zimg;
	zreal = _hif2(x) * _hif2(y) - _lof2(x) * _lof2(y);
	zimg = _hif2(x) * _lof2(y) + _lof2(x) * _hif2(y);
	return(_ftof2(zreal, zimg));
}

static inline __float2_t _daddsp(__float2_t x, __float2_t y)
{
	float32_t zreal, zimg;
	zreal = _hif2(x) + _hif2(y);
	zimg = _lof2(x) + _lof2(y);
	return(_ftof2(zreal, zimg));
}

static inline __float2_t _dsubsp(__float2_t x, __float2_t y)
{
	float32_t zreal, zimg;
	zreal = _hif2(x) - _hif2(y);
	zimg = _lof2(x) - _lof2(y);
	return(_ftof2(zreal, zimg));
}

static inline __float2_t _dmpysp(__float2_t x, __float2_t y)
{
	float32_t zreal, zimg;
	zreal = _hif2(x) * _hif2(y);
	zimg = _lof2(x) * _lof2(y);
	return(_ftof2(zreal, zimg));
}


static inline __float2_t _dinthsp(int32_t input)
{
	float32_t zreal, zimg;
	zreal	=	(float32_t)_ext(input, 0, 16U);
	zimg	=	(float32_t)_ext(input, 16U, 16U);
	return(_ftof2(zreal, zimg));
}

static inline int32_t _dspinth(__float2_t input)
{
	int32_t zreal, zimg;
	zreal	=	_spint(_hif2(input));
	zimg	=	_spint(_lof2(input));
	return((int32_t)_pack2((uint32_t)zreal, (uint32_t)zimg));
}

static inline int64_t _dspint(__float2_t input)
{
	int32_t zreal, zimg;
	zreal	=	_spint(_hif2(input));
	zimg	=	_spint(_lof2(input));
	return(_itoll((uint32_t)zreal, (uint32_t)zimg));
}
static inline int64_t _davg2(int64_t x, int64_t y)
{
	int32_t zhi, zlo;
	zhi = _avg2((int32_t)_hill(x), (int32_t)_hill(y));
	zlo = _avg2((int32_t)_loll(x), (int32_t)_loll(y));
	return(_itoll((uint32_t)zhi, (uint32_t)zlo));
}

static inline int64_t _dssub2(int64_t x, int64_t y)
{
	int32_t zhi, zlo;
	zhi = _ssub2((int32_t)_hill(x), (int32_t)_hill(y));
	zlo = _ssub2((int32_t)_loll(x), (int32_t)_loll(y));
	return(_itoll((uint32_t)zhi, (uint32_t)zlo));
}

static inline int64_t _dcmpyr1(int64_t x, int64_t y)
{
	uint32_t zhi, zlo;
	zhi = _cmpyr1(_hill(x), _hill(y));
	zlo = _cmpyr1(_loll(x), _loll(y));
	return(_itoll(zhi, zlo));
}

static inline int64_t _dshl2(int64_t x, int32_t y)
{
	int32_t zhi, zlo;
	zhi = _sshl((int32_t)_hill(x), (uint32_t)y);
	zlo = _sshl((int32_t)_loll(x), (uint32_t)y);
	return(_itoll((uint32_t)zhi,(uint32_t) zlo));
}

static inline int64_t _dsadd(int64_t x, int64_t y)
{
	int32_t zreal, zimg;
	zreal = (int32_t) _hill(x) + (int32_t)_hill(y);
	zimg = (int32_t)_loll(x) + (int32_t)_loll(y);
	return(_itoll((uint32_t)zreal, (uint32_t)zimg));
}

static inline int64_t _dssub(int64_t x, int64_t y)
{
	int32_t zreal, zimg;
	zreal = (int32_t) _hill(x) - (int32_t)_hill(y);
	zimg = (int32_t)_loll(x) - (int32_t)_loll(y);
	return(_itoll((uint32_t)zreal, (uint32_t)zimg));
}


static inline int64_t _dshl(int64_t x, int32_t y)
{
	int32_t zhi, zlo;
	zhi = _sshl((int32_t)_hill(x), (uint32_t)y);
	zlo = _sshl((int32_t)_loll(x), (uint32_t)y);
	return(_itoll((uint32_t)zhi, (uint32_t)zlo));
}

static inline int64_t _dcmpgt2(int64_t x, int64_t y)
{
	int32_t zhi, zlo;
	zhi = _cmpgt2((int32_t)_hill(x), (int32_t)_hill(y));
	zlo = _cmpgt2((int32_t)_loll(x), (int32_t)_loll(y));
	return(((uint32_t)zhi << 2U) | (uint32_t)zlo);
    
}

static inline int64_t _dxpnd2(int32_t x)
{
	uint32_t zhi, zlo;
	zhi = _xpnd2((uint32_t)x >> 2U);
	zlo = _xpnd2((uint32_t)x & 0x3U);
	return(_itoll(zhi, zlo));
}

#endif


#endif /*_RADARC674X_H */
