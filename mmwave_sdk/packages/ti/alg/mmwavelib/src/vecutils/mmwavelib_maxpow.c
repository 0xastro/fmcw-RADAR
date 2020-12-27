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
 *   @file     mmwavelib_maxpow.c
 *
 *   @brief    Routines to find from complex vector the maximum sample power.
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_maxpow
 *
 * \par
 * <b>Description</b> :    Find from 32bit complex vector the maximum sample power(I^2+Q^2),
 *                         and the index of the corresponding complex sample.
 *                         The math is as follows:
 *                         for ( i=0; i < Ncplx; i++)
 *                         {
 *                             pow = input[2*i]*input[2*i] + input[2*i+1]*input[2*i+1];
 *                             if (pow > max) {maxpow = pow; maxidx = i;}
 *                         }
 *
 * @param[in]              input : Input array of complex samples (32bit I 32bit Q or vice versa).
 * @param[in]              Ncplx : Number of complex elements in input.
 * @param[out]             maxpow : Output pointer to store the maximum power found.
 *
 * @return                 Return the index of the complex input sample that has the max I^2+Q^2.
 *
 * @pre                    (1)input is 8 byte aligned.
 * @pre                    (2)Ncplx is a multiple of 2.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  1.5*Ncplx + 46 (cgt8.1.3)
 **************************************************************************************************
 */
int32_t mmwavelib_maxpow(const int32_t * restrict input,
                         int32_t Ncplx,
                         float32_t * maxpow)
{
    const int64_t * restrict inp;
    int32_t   i;
    int64_t   data1, data2;
    float32_t power1, power2;
    float32_t maxpow1, maxpow2;
    int32_t   maxidx=0, maxidx1=0, maxidx2=0;

    maxpow1 = 0.0f;
    maxpow2 = 0.0f;
    inp     = (const int64_t *) input;

    _nassert((uint32_t) inp % 8U == 0); /* 8 byte aligned */

    for (i = 0; i < Ncplx; i+=2)
    {
        data1  = inp[i];
        power1 = (float32_t)(int32_t)_hill(data1) * (int32_t)_hill(data1)
                 + (float32_t)(int32_t)_loll(data1) * (int32_t)_loll(data1);
        if (power1 > maxpow1)
        {
            maxpow1 = power1;
            maxidx1 = i;
        }
        data2  = inp[i+1];
        power2 = (float32_t)(int32_t)_hill(data2) * (int32_t)_hill(data2)
                 + (float32_t)(int32_t)_loll(data2) * (int32_t)_loll(data2);
        if (power2 > maxpow2)
        {
            maxpow2 = power2;
            maxidx2 = i+1;
        }
    }

    if (maxpow1 > maxpow2)
    {
        *maxpow = maxpow1;
        maxidx  = maxidx1;
    }
    else
    {
        *maxpow = maxpow2;
        maxidx  = maxidx2;
    }

    return maxidx;
} /*  mmwavelib_maxpow */


/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_powerAndMax
 *
 * \par
 * <b>Description</b> :    Find from 32bit complex vector the maximum sample power(I^2+Q^2),
 *                         and the index of the corresponding complex sample.
 *                         The sample power of each complex sample will be stored to output buffer
 *                         which is the difference between this function and mmwavelib_maxpow.
 *                         The math is as follows:
 *                         for ( i=0; i < Ncplx; i++)
 *                         {
 *                             power[i] = input[2*i]*input[2*i] + input[2*i+1]*input[2*i+1];
 *                             if (power[i] > max) {maxpow = pow; maxidx = i;}
 *                         }
 *
 * @param[in]              input : Input array of complex samples (32bit I 32bit Q or vice versa).
 * @param[in]              Ncplx : Number of complex elements in input.
 * @param[out]             outpowbuf : Output buffer to store sample powers in float type.
 * @param[out]             maxpow : Output pointer to store the maximum power found.
 *
 * @return                 Return the index of the complex input sample that has the maximum power.
 *
 * @pre                    (1)input and outpowbuf are 8 byte aligned.
 * @pre                    (2)Ncplx is a multiple of 2.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  1.5*Ncplx + 51 (cgt8.1.3)
 **************************************************************************************************
 */
int32_t mmwavelib_powerAndMax(const int32_t * restrict input,
                              int32_t Ncplx,
                              float32_t * restrict outpowbuf,
                              float32_t * maxpow)
{
    const int64_t * restrict inp;
    int32_t   i;
    int64_t   data1, data2;
    float32_t power1, power2;
    float32_t maxpow1, maxpow2;
    int32_t   maxidx=0, maxidx1=0, maxidx2=0;
    __float2_t * restrict outptr;

    maxpow1 = 0.0f;
    maxpow2 = 0.0f;
    inp     = (const int64_t *) input;
	outptr = (__float2_t *)outpowbuf;

    _nassert((uint32_t) inp % 8U == 0); /* 8 byte aligned */
    _nassert((uint32_t) outptr % 8U == 0);

    for (i = 0; i < Ncplx; i+=2)
    {
        data1  = inp[i];
        power1 = (float32_t)(int32_t)_hill(data1) * (int32_t)_hill(data1)
                 + (float32_t)(int32_t)_loll(data1) * (int32_t)_loll(data1);
        if (power1 > maxpow1)
        {
            maxpow1 = power1;
            maxidx1 = i;
        }
        data2  = inp[i+1];
        power2 = (float32_t)(int32_t)_hill(data2) * (int32_t)_hill(data2)
                 + (float32_t)(int32_t)_loll(data2) * (int32_t)_loll(data2);
        if (power2 > maxpow2)
        {
            maxpow2 = power2;
            maxidx2 = i+1;
        }
        *outptr = _ftof2(power2, power1);
        outptr++;
    }

    if (maxpow1 > maxpow2)
    {
        *maxpow = maxpow1;
        maxidx  = maxidx1;
    }
    else
    {
        *maxpow = maxpow2;
        maxidx  = maxidx2;
    }

    return maxidx;
}
