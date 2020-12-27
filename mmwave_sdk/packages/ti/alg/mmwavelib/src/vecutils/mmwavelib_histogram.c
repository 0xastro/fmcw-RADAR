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
 *   @file     mmwavelib_histogram.c
 *
 *   @brief    Routine to calculate histogram
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * @brief                  Function calculates the histogram of the right-shifted input array
 *                         The math is as follows:
 *                         for ( i=0; i < len; i++)
 *                         {
 *                             hist[input[i]>>k] ++;
 *                         }
 *
 * @param[in]              input    : input array of unsigned 16-bit elements.
 * @param[out]             hist     : output histogram array.
 * @param[inout]           histTemp : temp scratch buffer.
 * @param[in]              inpLen   : number of 16-bit elements in input.
 * @param[in]              histLen  : length of historgram, = 1<<(16-numshift) normally.
 * @param[in]              numshift : number of bits to be right-shifted.
 *
 * @return                 void
 *
 * @pre                    (1)input and histTemp are 8 byte aligned.
 * @pre                    (2)inpLen is multiple of 8, and histLen is multiple of 2.
 * @pre                    (3)histTemp buffer's length is 8*histLen.
 *
 * @ingroup                MMWAVELIB_VEC_UTILS
 *
 * Cycles                  inpLen*1.125 + histLen*2.75 + overhead (cgt8.1.3)
 **************************************************************************************************
 */
void mmwavelib_histogram(uint16_t input[restrict],
                         uint16_t hist[restrict],
                         uint16_t histTemp[restrict],
                         uint32_t inpLen,
                         uint32_t histLen,
                         uint8_t  numshift)
{
    uint32_t i;
    uint64_t doubleWord;
    uint32_t hiWord, loWord;
    uint32_t tempword;
    uint32_t histIdx0, histIdx1, histIdx2, histIdx3;
    uint32_t histIdx4, histIdx5, histIdx6, histIdx7;
    uint64_t *restrict inptr = (uint64_t*)input;
    uint64_t *restrict temphistPtr = (uint64_t*)histTemp;
    uint16_t *restrict hist0,*restrict hist1, *restrict hist2, *restrict hist3;
    uint16_t *restrict hist4,*restrict hist5, *restrict hist6, *restrict hist7;


    hist0 = histTemp;
    hist1 = &histTemp[1]; 
    hist2 = &histTemp[2]; 
    hist3 = &histTemp[3]; 
    hist4 = &histTemp[4]; 
    hist5 = &histTemp[5]; 
    hist6 = &histTemp[6]; 
    hist7 = &histTemp[7]; 

    /* Initialize histTemp buffer(histLen*8 16-bit) to 0s */
    for(i = 0; i < histLen*8/4; i++) /* unroll 2x, 1 cycle per 128 bits */
    {
        temphistPtr[i] = 0;
    }

    _nassert((uint32_t) inptr % 8U == 0); /* 8 byte aligned */
    _nassert(inpLen % 8U == 0);
    for(i = 0; i < inpLen/4U; i+=2)
    {
        doubleWord = inptr[i]; /*four 16-bit input*/

        loWord = _shru2(_loll(doubleWord), numshift);
        hiWord = _shru2(_hill(doubleWord), numshift);

        histIdx0 = loWord & 0xFFFFU;
        hist0[ histIdx0 *8 ] ++;

        histIdx1 = (loWord>>16U) & 0xFFFFU;
        hist1[ histIdx1 *8 ] ++;

        histIdx2 = hiWord &0xFFFFU;
        hist2[ histIdx2 *8 ] ++;

        histIdx3 = (hiWord>>16U)&0xFFFFU;
        hist3[ histIdx3 *8 ] ++;

        doubleWord = inptr[i+1]; /*another four 16-bit input*/
        loWord = _shru2(_loll(doubleWord), numshift);
        hiWord = _shru2(_hill(doubleWord), numshift);

        histIdx4 = loWord & 0xFFFFU;
        hist4[ histIdx4 *8 ] ++;

        histIdx5 = (loWord>>16U) & 0xFFFFU;
        hist5[ histIdx5 *8 ] ++;

        histIdx6 = hiWord &0xFFFFU;
        hist6[ histIdx6 *8 ] ++;

        histIdx7 = (hiWord>>16U)&0xFFFFU;
        hist7[ histIdx7 *8 ] ++;

    }

    _nassert((uint32_t) temphistPtr % 8U == 0); /* 8 byte aligned */
    _nassert(histLen % 2U == 0); /* unroll 2x */
    for (i = 0; i < histLen; i++)
    {
        tempword = _add2( _add2(_loll(temphistPtr[2*i]), _hill(temphistPtr[2*i])),
                          _add2(_loll(temphistPtr[2*i+1]), _hill(temphistPtr[2*i+1])) );
        hist[i] = (tempword & 0xFFFFU) + (tempword >>16U);
    }

}
