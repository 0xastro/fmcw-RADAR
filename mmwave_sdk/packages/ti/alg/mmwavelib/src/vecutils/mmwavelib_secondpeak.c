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
 *   @file     mmwavelib_secondpeak.c
 *
 *   @brief    Routine to search for the second peak in float vectors.
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_secondPeakSearch
 *
 * \par
 * <b>Description</b> :    Find from 32bit float vector the second peak and return its index i.
 *                         A peak satisfies:
 *                         x[i] > x[i-1] & lenLess1 and
 *                         x[i] > x[i+1] & lenLess1 where lenLess1 = len-1
 *                         The second peak x[i] <= x[maxIdx] but greater than all other peaks.
 *
 * @param[in]              x: Input array of float values.
 * @param[in]              len: Number of elements in input.
 * @param[in]              maxIdx: Index of the element with the maximum value in the array.
 *
 * @return                 Return the index of the second peak.
 *
 * @pre                    (1)Elements in x are all >=0.
 * @pre                    (2)len is a multiple of 2.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  4*len + 100 (cgt8.1.3)
 **************************************************************************************************
 */
int32_t mmwavelib_secondPeakSearch(float32_t * x,
                                   int32_t   len,
                                   int32_t   maxIdx)
{
    int32_t    i, k;
    float32_t  valleyR, valleyL;
    uint32_t   lenLess1 = (uint32_t)len-1U;
    int32_t    leftSearchIdx;
    int32_t    rightSearchIdx;
    int32_t    searchLen;
    uint32_t   iModLen;
    int32_t    maxIdx2;
    float32_t  maxVal2;


    /* Find right edge of the first peak */
    valleyR = x[maxIdx];
    leftSearchIdx = (maxIdx + 1) & lenLess1;
    k = len;
    while ((valleyR >= x[leftSearchIdx]) && (k > 0))
    {
        valleyR = x[leftSearchIdx];
        leftSearchIdx ++;
        if (leftSearchIdx == len)
        {
            leftSearchIdx =0;
        }
        k--;
    }

    /* Find left edge of the first peak */
    valleyL = x[maxIdx];
    rightSearchIdx = (maxIdx - 1) & lenLess1;
    k = len;
    while ((valleyL >= x[rightSearchIdx]) && (k > 0))
    {
        valleyL = x[rightSearchIdx];
        rightSearchIdx--;
        if (rightSearchIdx < 0)
        {
            rightSearchIdx = lenLess1;
        }
        k--;
    }

    /* Search for the second peak */
    searchLen = rightSearchIdx - leftSearchIdx;
    if (searchLen < 0)
    {
        searchLen += len;
    }
    maxVal2 = x[leftSearchIdx];
    maxIdx2 = leftSearchIdx;
    for (i = leftSearchIdx+1; i <= (leftSearchIdx + searchLen); i++)
    {
        iModLen = i & lenLess1;
        if (x[iModLen] > maxVal2)
        {
            maxIdx2 = iModLen;
            maxVal2 = x[iModLen];
        }
    }

    return maxIdx2;

} /* mmwavelib_secondPeakSearch */
