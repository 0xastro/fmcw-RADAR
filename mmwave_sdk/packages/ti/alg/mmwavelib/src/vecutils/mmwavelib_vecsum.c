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
 *   @file     mmwavelib_vecsum.c
 *
 *   @brief    Routine to sum vector elements
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_vecsum
 *
 * \par
 * <b>Description</b> :    Sum the elements in 16-bit complex vector. 
 *                         The math is as follows:
 *                         for ( i=0; i < ncplx; i++)
 *                         {
 *                             output[0] += input[2*i];
 *                             output[1] += input[2*i+1];
 *                         }
 *                         The order of I&Q in output is same as that in input.
 *                         The function can be used for 16-bit real vector too,
 *                         and user can simply do output[0]+output[1] after the function.
 *
 * @param[in]              input  : Input complex array (e.g.16bit I followed by 16bit Q each).
 * @param[in]              ncplx  : Number of complex elements in input.
 * @param[out]             output : Output sum (e.g. 32bit sum I followed by 32bit sum Q).
 *
 * @return                 void
 *
 * @pre                    (1)input and output are 8 byte aligned.
 * @pre                    (2)Number of complex elements is a multiple of 4.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  0.5*Ncplx + ovh (0.25*NumOf16bitElements + ovh), ovh=~30 cgt8.1.3
 **************************************************************************************************
 */
void mmwavelib_vecsum(const int16_t input[restrict],
                      int32_t output[restrict],
                      int32_t ncplx)
{
    uint64_t doubleWord;
    uint32_t upperWord, packWordI;
    uint32_t lowerWord, packWordQ;
    int32_t sumI, sumQ;
    int32_t i;
    const uint64_t *restrict inptr = (const uint64_t*)input;
    uint64_t *restrict outptr = (uint64_t*)output;
    int32_t one = 0x00010001;

    sumI = 0;
    sumQ = 0;
    _nassert((uint32_t) inptr % 8U == 0); /* 8 byte aligned */
    _nassert(ncplx % 4U == 0);
    #pragma MUST_ITERATE(2,,2)
    for(i = 0; i < (ncplx/2); i++)
    {
        doubleWord = inptr[i];

        lowerWord = (_loll(doubleWord));
        upperWord = (_hill(doubleWord));
        packWordI = _pack2(lowerWord, upperWord);
        packWordQ = _packh2(lowerWord, upperWord);

        sumI += _dotp2((int32_t)packWordI, one);
        sumQ += _dotp2((int32_t)packWordQ, one);
    }

    outptr[0] = (uint64_t)(_itoll((uint32_t)sumQ, (uint32_t)sumI));

}
