 /*
  * (C) Copyright 2018, Texas Instruments Incorporated -  http://www.ti.com/
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
 *   @file     mmwavelib_power.c
 *
 *   @brief    Routine to calculate complex sample power of complex vector.
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"
#include <ti/alg/mmwavelib/include/mmwavelib_c674_emulate_c66_intrinsics.h>

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_power
 *
 * \par
 * <b>Description</b> :    Calculate sample power(I^2+Q^2) for each 32bit complex sample in vector.
  *                        The math is as follows:
 *                         for ( i=0; i < Ncplx; i++)
 *                         {
 *                             power[i] = (float32_t)input[2*i]*input[2*i];
 *                                      + (float32_t)input[2*i+1]*input[2*i+1];
 *                         }
 *
 * @param[in]              input : Input array of complex samples (32bit I 32bit Q or vice versa).
 * @param[in]              Ncplx : Number of complex elements in input array.
 * @param[out]             outpowbuf : Output buffer to store sample powers in float type.
 *
 * @return                 None.
 *
 * @pre                    (1)input and outpowbuf are 8 byte aligned.
 * @pre                    (2)Ncplx is a multiple of 2.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  1.5*Ncplx + 45 (cgt8.1.3)
 **************************************************************************************************
 */
void mmwavelib_power(int32_t * restrict input,
                     int32_t Ncplx,
                     float32_t * restrict outpowbuf)
{
    int64_t * restrict inptr;
    int32_t   i;
    int64_t   data1, data2;
    float32_t power1, power2;
    __float2_t * restrict outptr;

    inptr    = (int64_t *) input;
    outptr = (__float2_t *)outpowbuf;
    for (i = 0; i < Ncplx; i+=2)
    {
        data1   = inptr[i];
        power1  = (float32_t)(int32_t)_hill(data1) * (int32_t)_hill(data1)
                  + (float32_t)(int32_t)_loll(data1) * (int32_t)_loll(data1);
        data2   = inptr[i+1];
        power2  = (float32_t)(int32_t)_hill(data2) * (int32_t)_hill(data2)
                  + (float32_t)(int32_t)_loll(data2) * (int32_t)_loll(data2);
        *outptr = _ftof2(power2, power1);
        outptr++;
    }

} /* mmwavelib_power */

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_power_float
 *
 * \par
 * <b>Description</b> :    Calculate sample power(I^2+Q^2) for each float complex sample in vector.
  *                        The math is as follows:
 *                         for ( i=0; i < Ncplx; i++)
 *                         {
 *                             power[i] = input[2*i]*input[2*i] + input[2*i+1]*input[2*i+1];
 *                         }
 *
 * @param[in]              input : Input array of floating point complex samples.
 * @param[in]              Ncplx : Number of complex elements in input array.
 * @param[out]             outpowbuf : Output buffer to store sample powers.
 *
 * @return                 None.	
 *
 * @pre                    (1)input and outpowbuf are 8 byte aligned.
 * @pre                    (2)Ncplx is a multiple of 2.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  1*Ncplx + 41 (cgt8.1.3)
 **************************************************************************************************
 */
void mmwavelib_power_float(float32_t * restrict input,
                           int32_t Ncplx,
                           float32_t * restrict outpowbuf)
{
    int32_t   i;
    __float2_t * restrict inptr;
    __float2_t  f2temp;
    __float2_t  results;
    float32_t * restrict outptr;

    inptr  = (__float2_t *) input;
    outptr = (float32_t *) outpowbuf;

    for (i = 0; i < Ncplx; i += 2)
    {
        f2temp      =   _amem8_f2(inptr);
        inptr++;
        results     =   _dmpysp(f2temp ,f2temp );
        *outptr     =   _hif2(results) + _lof2(results);
        outptr++;
        f2temp      =   _amem8_f2(inptr);
        inptr++;
        results     =   _dmpysp(f2temp,f2temp);
        *outptr     =   _hif2(results) + _lof2(results);
        outptr++;
    }

} /* mmwavelib_power_float */
