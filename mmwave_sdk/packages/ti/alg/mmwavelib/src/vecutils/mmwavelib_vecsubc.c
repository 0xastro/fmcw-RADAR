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
 *   @file     mmwavelib_vecsubc.c
 *
 *   @brief    Routin to subtract const from vector
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_vecsubc
 *
 * \par
 * <b>Description</b> :    Subtract const value from each element in 16-bit complex vector.
 *                         The math is as follows:
 *                            output_real[n]= input_real[n] - subval_real;
 *                            output_imag[n]= input_imag[n] - subval_imag;
 *                         The function can be used for 16-bit real vector too,
 *                         by simply pack two subval_real in 32-bit subval.
 *
 * @param[in]              input  : pointer to input array (e.g. 16bit I followed by 16bit Q)
 * @param[out]             output : pointer to output, can be the same array as input
 * @param[in]              subval : Packed value to be subtracted (e.g. Q in high 16, I in low 16)
 * @param[in]              ncplx  : Number of complex elements
 *
 * @return                 void
 *
 * @pre                    (1)input and output 8 byte aligned.
 * @pre                    (2)ncplx is a multiple of 4.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  0.5*Ncplx + ovh (0.25*NumOf16bitElements + ovh), ovh=~40 cgt8.1.3
 **************************************************************************************************
 */
void mmwavelib_vecsubc(int16_t *restrict input,
                       int16_t *restrict output,
                       const uint32_t subval,
                       int32_t ncplx)
{
    uint64_t doubleWord;
    uint32_t inword1,inword2;
    uint32_t outword1, outword2;
    int32_t i;
    uint64_t *inptr = (uint64_t*)input;
    uint64_t *outptr = (uint64_t*)output;

    _nassert((uint32_t) inptr % 8U == 0); /* 8 byte aligned */
    _nassert((uint32_t) outptr % 8U == 0); /* 8 byte aligned */
    _nassert(ncplx % 4U == 0);

#pragma UNROLL(2)
    for(i = 0; i < ncplx; i+=2)
    {
        doubleWord = _amem8(inptr);
        inptr++;

        inword1 = _loll(doubleWord);
        inword2 = _hill(doubleWord);

        outword1 = _ssub2( inword1, subval);
        outword2 = _ssub2( inword2, subval);

        *outptr  = (uint64_t)_itoll(outword2, outword1);
        outptr++;
    }

}

