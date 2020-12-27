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
 *   @file     mmwavelib_dcremoval_float.c
 *
 *   @brief    Floating point dc removal routine
 *
 */

#include <stdint.h>
#include <c6x.h>
#include <ti/alg/mmwavelib/include/mmwavelib_c674_emulate_c66_intrinsics.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_dcRemovalFloat
 *
 * \par
 * <b>Description</b> :    Calculate the average (dc value) of 32-bit float complex vector 
 *                         (by sum all complex values and divide it by ncplx)
 *                         and remove i.e subtract it from each input complex value.
 *                         The math is as follows:
 *                         for ( i=0; i < ncplx; i++)
 *                         {
 *                             sumI += input[2*i];
 *                             sumQ += input[2*i+1];
 *                         }
 *                         avgI = sumI/ncplx; avgQ = sumQ/ncplx;
 *                         for ( i=0; i < ncplx; i++)
 *                         {
 *                             output[2*i] = input[2*i] - avgI;
 *                             output[2*i+1] = input[2*i+1] - avgQ;
 *                         }
 *                         The order of I&Q in output is same as that in input.
 *
 * @param[in]              ncplx  : Number of complex elements in input/output array.
 * @param[in]              input  : Input array (e.g. each float 32bit I followed by float 32bit Q)
 * @param[out]             output : Output array, can be the same array as input.
 *
 * @return                 void
 *
 * @pre                    (1)input and output are 8 byte aligned.
 * @pre                    (2)input and output point to either the same array, or two arrays that
 *                            do not overlap with each other.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles  (cgt8.1.3)      4.5*N + ovh, for N%8 = 0; 
 *                         3.5*N + ovh, for N%8 = 2,4,
 *                         4*N + ovh, for N%8 = 6;
 *                         for odd number N>1, cycles are same as the cycles for (N-1);
 *                         where N=ncplx; overhead cycles ovh=84 to 89. 
 **************************************************************************************************
 */
void mmwavelib_dcRemovalFloat(uint32_t ncplx,
                              float32_t * input,
                              float32_t * output)
{
    int32_t     j;
    __float2_t  * restrict input1;
    __float2_t  * restrict input2;
    __float2_t  * restrict output1;
    __float2_t  * restrict output2;
    __float2_t  sum1, sum2, avg, oneOverN;
    float32_t   temp;

    input1      =   (__float2_t  *) &input[0];
    input2      =   (__float2_t  *) &input[(ncplx >> 1) << 1]; /* second half of input array */
    sum1        =   _ftof2(0.f, 0.f);
    sum2        =   _ftof2(0.f, 0.f);
    /* if ncplx is odd, initialize sum2 with the very last complex input sample */
    if (ncplx & 1)
    {
        sum2    =   input1[ncplx-1];
    }

    /* sum all complex values in 1st half of array, and sum 2nd half of array */
    for (j = 0; j < (int32_t) (ncplx>>1); j++)
    {
        sum1       =   _daddsp(sum1, input1[j]); /* first half of input array */
        sum2       =   _daddsp(sum2, input2[j]); /* second half of input array */
    }
    /* add 1st half sum and second half sum */
    /* 32bits sumI and 32bits sumQ are packed in 64bits */
    sum1        =   _daddsp(sum1, sum2);

    /* Calculate 1/ncplx */
    temp        =   _rcpsp((float32_t)ncplx); 
    temp        =   temp * (2.f - temp * (float32_t)ncplx);
    temp        =   temp * (2.f - temp * (float32_t)ncplx);
    oneOverN    =   _ftof2(temp, temp); /* high and low 32bits both stored 1/ncplx  */

    /* multiply sumI and sumQ each with 1/ncplx to get the average i.e. dc value */
    avg         =   _dmpysp(sum1, oneOverN);

    output1     =   (__float2_t  *) &output[0];
    output2     =   (__float2_t  *) &output[(ncplx >> 1)  << 1];

    /* subtract complex dc value from each input complex value */
    for (j = 0; j < (int32_t) (ncplx>>1); j++)
    {
        output1[j]   =   _dsubsp(input1[j], avg); /* first half of array */
        output2[j]   =   _dsubsp(input2[j], avg); /* second half of array */
    }

    /* if ncplx is odd, process the very last complex sample */
    if (ncplx & 1)
    {
        output1[ncplx-1]   =   _dsubsp(input1[ncplx-1], avg);
    }

}
