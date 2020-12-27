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
 *   @file     mmwavelib_vecsub.c
 *
 *   @brief    Routin for vector subtraction
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"


/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_vecsub16
 *
 * \par
 * <b>Description</b> :    16-bit vector subtraction. The math is as follows:
 *                            output[n]= input1[n] - input2[n];
 *                         The function can also be used for complex vector (16bit I 16bit Q),
 *                         as long as input1 and input2 have the same I/Q order.
 *                         (2*number of complex samples) shall be len for the function.
 *
 * @param[in]              input1 : pointer to first input array
 * @param[in]              input2 : pointer to second input array
 * @param[out]             output : pointer to output, can be the same array as input1
 * @param[in]              len    : Number of 16-bit elements
 *
 * @return                 void
 *
 * @pre                    (1)input1, input2 and output 8 byte aligned.
 * @pre                    (2)input1 and output either point to the same array or do not overlap
 *                            with each other, and they don't overlap with input2.
 * @pre                    (3)len is multiple of 2.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles  (cgt8.1.3)      0.375*len +ovh, ovh=~45(len%8=0) +8.25,9.5 or 10.75(if len%8=2,4,or 6)
 **************************************************************************************************
 */
void mmwavelib_vecsub16( int16_t *restrict input1,
                         int16_t *restrict input2,
                         int16_t *restrict output,
                         int32_t len)
{
    int32_t  i;

    _nassert((uint32_t) input1 % 8U == 0); /* 8 byte aligned */
    _nassert((uint32_t) input2 % 8U == 0); /* 8 byte aligned */
    _nassert((uint32_t) output % 8U == 0); /* 8 byte aligned */
    _nassert((uint32_t) len % 2U == 0);

#pragma UNROLL(8)/* Process 8 16-bit samples a time in loop kernel */
    for(i = 0; i < len; i++)
    {
        output[i] = input1[i] - input2[i];
    }

} /* mmwavelib_vecsub16 */
