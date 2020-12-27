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
 *   @file     mmwavelib_multipeak.c
 *
 *   @brief    Routine to search for multiple peaks in single-precision float vectors.
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * \brief
 * Function Name      :    mmwavelib_multiPeakSearch
 *
 * \par
 * <b>Description</b> :    Find from 32bit float vector the peaks that have value satisfies:
 *                         x[i] >= x[i-1] & mask and
 *                         x[i] >= x[i+1] & mask and
 *                         x[i] > the maximum value in vector * thresholdScale
 *                         where mask = len-1.
 *
 * @param[in]              x: Input array of float values.
 * @param[out]             peakIdxBuf: output buffer to store the index of detected peaks.
 * @param[in]              len: Number of elements in input.
 * @param[in]              thresholdScale: maximum value multiplied by this will be the threshold
 *                         for search other peaks.
 *
 * @return                 Return the number of detected peaks.
 *
 * @pre                    (1)input array x is 8 byte aligned.
 * @pre                    (2)len is a multiple of 4.
 * @pre                    (3)Maximum value max(x)>0.0.
 * @pre                    (4)0.0 <=thresholdScale <=1.0.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  11.75*len +53 (cgt8.1.3)
 **************************************************************************************************
 */
uint16_t mmwavelib_multiPeakSearch(float32_t * x,
                                   uint16_t  * peakIdxBuf,
                                   int32_t     len,
                                   float32_t   thresholdScale)
{

    uint16_t   numDetPeaks = 0;
    int32_t    i;
    float32_t  maxval;
    float32_t  threshold;
    float32_t  x0, x1, x2, x3, max0, max1, max2, max3;
    __float2_t x_01, x_23;
    float32_t  left, mid, right;
    uint16_t   found;


/*  Search for maximum value. Natural code:
    maxval = 0.0;
    for (i = 0; i < len; i++)
    {
        if (x[i] > maxval)
        {
            maxval = x[i];
        }
    }
*/
    max0 = 0.0;
    max1 = 0.0;
    max2 = 0.0;
    max3 = 0.0;

    _nassert((uint32_t)len % 4U == 0);
    _nassert(len > 0);
    _nassert((uint32_t)x % 8U == 0);

    #pragma MUST_ITERATE(1,,)
    for (i = 0; i < len; i+=4)
    {
        x_01 = _amem8_f2((void*)&x[i]);
        x_23 = _amem8_f2((void*)&x[i+2]);
        x0   = _hif2(x_01);
        x1   = _lof2(x_01);
        x2   = _hif2(x_23);
        x3   = _lof2(x_23);
        if (x0 > max0)
        {  max0 = x0; }
        if (x1 > max1)
        {  max1 = x1; }
        if (x2 > max2)
        {  max2 = x2; }
        if (x3 > max3)
        {  max3 = x3; }
    }
    if (max0 > max1)
    {  max1 = max0; }
    if (max2 > max3)
    {  max3 = max2; }
    if (max3 > max1)
    {  maxval = max3; }
    else
    {  maxval = max1; }

    threshold = maxval * thresholdScale;
    /* Search for peaks. non-optimized code:
    {
    int32_t  iLeft, iRight;
    uint32_t mask = len - 1;
    for (i = 0; i < len; i++)
    {
        iLeft = (i - 1) & mask;
        iRight = (i + 1) & mask;

        if ((x[i] >= x[iLeft]) && (x[i] >= x[iRight]) && (x[i] > threshold))
        {
            peakIdxBuf[numDetPeaks] = i;
            numDetPeaks++;
        }
    }
    */
    left  = x[len-1];
    mid   = x[0];
    right = x[1];

    for (i = 0; i < len; i++)
    {
        right = x[i+1];
        if ((i+1)==len)
        {    right = x[0]; }

        found = 0;
        if (mid > threshold)
        {    found = 1; }
        if (left > mid)
        {    found = 0; }
        if (right > mid)
        {    found = 0; }

        if (found == 1)
        {
            peakIdxBuf[numDetPeaks] = (uint16_t)i;
            numDetPeaks++;
        }
        left = mid;
        mid  = right;
    }

    return numDetPeaks;
}
