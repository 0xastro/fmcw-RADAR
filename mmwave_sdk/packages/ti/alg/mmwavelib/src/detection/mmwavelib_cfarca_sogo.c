 /*
  * (C) Copyright 2016, Texas Instruments Incorporated -  http://www.ti.com/
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
  *   @file     mmwavelib_cfarca_sogo.c
  *
  *   @brief    This file contains routine supporting CFAR CA, CASO, CAGO.
*/

#include <stdint.h>
#include <ti/alg/mmwavelib/src/detection/mmwavelib_detection.h>


/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarCadB_SOGO
 *
 * \par
 * <b>Description</b>  :    Performs a CFAR on an 16-bit unsigned input vector. The input values are assumed to be
 *                          in lograthimic scale. So the comparision between the CUT and the noise samples is additive
 *                          rather than multiplicative. Supports CA, CA SO, and CA GO.
 *
 * @param[in]               inp         : input array (16 bit unsigned numbers)
 * @param[in]               out         : output array (indices of detected peaks (zero based counting))
 * @param[in]               len         : number of elements in input array
 * @param[in]               cfartype    : type of noise floor calculation when two sides of noise available.
 *                                        CFAR_CA; use sum of both sides then average.
 *                                        CFAR_CASO; use smaller of the left and right side.
 *                                        CFAR_CAGO; use greater of the left and right side.
 * @param[in]               const1,const2 : used to compare the Cell Under Test (CUT) to the sum of the noise cells:
 *                                          [noise sum /(2^(const2-1))]+const1 for one sided comparison 
 *                                          (at the begining and end of the input vector).
 *                                          [noise sum /(2^(const2))]+const1 for two sided comparison
 * @param[in]               guardLen    : one sided guard length
 * @param[in]               noiseLen    : one sided noise length
 *
 * @param[out]              out         : output array with indices of the detected peaks
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre                     Input (inp) and Output (out) arrays are non-aliased.
 * @ingroup                 MMWAVELIB_DETECT
 *
 * Cycles (cgt 8.1.3)       type CA: 3*len +111; type CASO: 4*len +114; type CAGO: 4*len +108
 *******************************************************************************************************************
 */
uint32_t mmwavelib_cfarCadB_SOGO(const uint16_t inp[restrict],
                            uint16_t out[restrict], uint32_t len,
                            uint16_t cfartype,
                            uint32_t const1, uint32_t const2,
                            uint32_t guardLen, uint32_t noiseLen)
{
    uint32_t idx, idxLeftNext, idxLeftPrev, idxRightNext,
             idxRightPrev, outIdx, idxCUT;
    uint32_t sum, sumLeft, sumRight;

    /* initializations */
    outIdx = 0;
    sumLeft = 0;
    sumRight = 0;
    for (idx = 0; idx < noiseLen; idx++)
    {
        sumRight += inp[idx + guardLen + 1U];
    }

    /*********************************************************************************************/
    /* One-sided comparision for the first segment (for the first noiseLen+gaurdLen samples */
    idxCUT = 0;
    if ((uint32_t) inp[idxCUT] > ((sumRight >> (const2 - 1U)) + const1))
    {
        out[outIdx] = (uint16_t)idxCUT;
        outIdx++;
    }
    idxCUT++;

    idxLeftNext = 0;
    idxRightPrev = idxCUT + guardLen;
    idxRightNext = idxRightPrev + noiseLen;
    for (idx = 0; idx < (noiseLen + guardLen - 1U); idx++)
    {
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;

        if (idx < noiseLen)
        {
            sumLeft += inp[idxLeftNext];
            idxLeftNext++;
        }

        if ((uint32_t) inp[idxCUT] > ((sumRight >> (const2 - 1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;
    }

    /*********************************************************************************************/
    /* Two-sided comparision for the middle segment */
    sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
    idxRightNext++;
    idxRightPrev++;

    if (cfartype == CFAR_CA) 
    {
        sum = sumRight + sumLeft;
        if ((uint32_t) inp[idxCUT] > ((sum >> const2) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;

        idxLeftPrev = 0;
        for (idx = 0; idx < (len - 2U*(noiseLen + guardLen) - 1U); idx++)
        {
            sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
            sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
            idxLeftNext++;
            idxLeftPrev++;
            idxRightNext++;
            idxRightPrev++;
            sum = sumLeft + sumRight;

            if ((uint32_t) (inp[idxCUT]) > ((sum >> const2) + const1))
            {
                out[outIdx] = (uint16_t)idxCUT;
                outIdx++;
            }
            idxCUT++;
        }
    } /*CFAR_CA*/
    else if (cfartype == CFAR_CASO)
    {
        sum = (sumLeft < sumRight) ? sumLeft:sumRight;
        if ((uint32_t) inp[idxCUT] > ((sum >> (const2-1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;

        idxLeftPrev = 0;
        for (idx = 0; idx < (len - 2U*(noiseLen + guardLen) - 1U); idx++)
        {
            sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
            sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
            idxLeftNext++;
            idxLeftPrev++;
            idxRightNext++;
            idxRightPrev++;

            sum = (sumLeft < sumRight) ? sumLeft:sumRight;

            if ((uint32_t) (inp[idxCUT]) > ((sum >> (const2-1U)) + const1))
            {
                out[outIdx] = (uint16_t)idxCUT;
                outIdx++;
            }
            idxCUT++;
        }
    } /*CFAR_CASO*/
    else /*CFAR_CAGO*/
    {
        sum = (sumLeft > sumRight) ? sumLeft:sumRight;
        if ((uint32_t) inp[idxCUT] > ((sum >> (const2-1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;

        idxLeftPrev = 0;
        sum = sumLeft + sumRight;
        for (idx = 0; idx < (len - 2U*(noiseLen + guardLen) - 1U); idx++)
        {
            sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
            sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
            idxLeftNext++;
            idxLeftPrev++;
            idxRightNext++;
            idxRightPrev++;

            sum = (sumLeft > sumRight) ? sumLeft:sumRight;

            if ((uint32_t) (inp[idxCUT]) > ((sum >> (const2-1U)) + const1))
            {
                out[outIdx] = (uint16_t)idxCUT;
                outIdx++;
            }
            idxCUT++;
         }
    } /*CFAR_CAGO*/

    /*********************************************************************************************/
    /* One-sided comparision for the last segment (for the last noiseLen+gaurdLen samples) */
    for (idx = 0; idx < (noiseLen + guardLen); idx++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        if ((uint32_t) inp[idxCUT] > ((sumLeft >> (const2 - 1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;
    }
    /*********************************************************************************************/

    return (outIdx);

}  /* mmwavelib_cfarCadB_SOGO */
