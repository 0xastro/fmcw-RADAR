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
 *   @file     mmwavelib_vecsumabs.c
 *
 *   @brief    Routine to sum absolute values of vector elements
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_vecsumabs
 *
 * \par
 * <b>Description</b> :    Sum the absolute values of 16-bit elements in vector. 
 *                         The math is as follows:
 *                         for ( i=0; i < len; i++)
 *                         {
 *                             sum += abs(input[i]);
 *                         }
 *
 * @param[in]              input : Input array.
 * @param[in]              len   : Number of 16-bit elements in input.
 *
 * @return                 Returns the 32-bit sum
 *
 * @pre                    (1)input is 8 byte aligned.
 * @pre                    (2)len is a multiple of 4.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  0.25*len + ovh, ovh=37 cgt8.1.3
 **************************************************************************************************
 */
int32_t mmwavelib_vecsumabs(const int16_t input[], int32_t len)
{
    uint64_t doubleWord;
    int32_t upperWord, lowerWord;
    int32_t sum = 0;
    int32_t i;
    const uint64_t *restrict inptr = (const uint64_t*)input;
    int32_t one = 0x00010001;

    _nassert((uint32_t) inptr % 8U == 0); /* 8 byte aligned */
    _nassert(len % 4 == 0);
    for(i = 0; i < len/4; i++)
    {
        doubleWord = inptr[i];

        lowerWord = _abs2(_loll(doubleWord));
        upperWord = _abs2(_hill(doubleWord));

        sum += _dotp2(lowerWord, one);
        sum += _dotp2(upperWord, one);
    }

    return sum;
}
