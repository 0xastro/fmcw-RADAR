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
  *   @file     mmwavelib_cfarca.c
  *
  *   @brief    This file contains routines related to CFAR-CA (cell averaging) processing
*/

#include <stdint.h>
#include <ti/alg/mmwavelib/src/detection/mmwavelib_detection.h>

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarCa
 *
 * \par
 * <b>Description</b>  :    Performs a regular Cell Averaging CFAR on an 16-bit unsigned input vector (CFAR-CA)
 *
 * @param        [in]       inp      : input array (16 bit unsigned numbers)
 * @param        [in]       out      : output array (indices of detected peaks (zero based counting))
 * @param        [in]       len      : number of elements in input array
 * @param        [in]       const1,const2 : const1,const2 : used to compare the Cell Under Test (CUT) to the sum of the noise cells:
 *                                          [noise sum  *const1 /(2^(const2-1))] for one sided comparison 
 *                                          (at the begining and end of the input vector).
 *                                          [noise sum  *const1 /(2^(const2))] for two sided comparison 
 * @param        [in]       guardLen : one sided guard length
 * @param        [in]       noiseLen : one sided Noise length
 *
 * @param        [out]      out      : output array with indices of the detected peaks
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre                     Input (inp) and Output (out) arrays are non-aliased.
 *
 * @ingroup                 MMWAVELIB_DETECT
 *******************************************************************************************************************
 */
uint32_t mmwavelib_cfarCa(const uint16_t inp[restrict],
                          uint16_t out[restrict], uint32_t len,
                          uint32_t const1, uint32_t const2,
                          uint32_t guardLen, uint32_t noiseLen)
{
    /*CAUTION : Max value of numbers */
    uint32_t idx, idxLeftNext, idxLeftPrev, idxRightNext,
        idxRightPrev, outIdx, idxCUT;
    uint32_t sum, sumLeft, sumRight;

    /*initializations */
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
    if ((uint32_t)inp[idxCUT] > ((sumRight * const1) >> (const2 - 1U)))
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

        if ((uint32_t) inp[idxCUT] >
            ((sumRight * const1) >> (const2 - 1U)))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;
    }
    /*********************************************************************************************/
    /*Two-sided comparision for the middle segment */
    sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
    idxRightNext++;
    idxRightPrev++;
    
    sum = sumRight + sumLeft;
    if ((uint32_t) inp[idxCUT] > ((sum * const1) >> const2))
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
        idxLeftNext++;
        idxLeftPrev++;
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = sumLeft + sumRight; /*CFAR-CA */
        /*sum=(sumLeft>sumRight)?sumLeft:sumRight;//CFAR-CA-GO */
        if ((uint32_t) (inp[idxCUT]) > ((sum * const1) >> const2))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;
    }
    /*********************************************************************************************/
    /* One-sided comparision for the last segment (for the last noiseLen+gaurdLen samples) */
    for (idx = 0; idx < (noiseLen + guardLen); idx++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        if ((uint32_t) inp[idxCUT] >
            ((sumLeft * const1) >> (const2 - 1U)))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;

    }
    /*********************************************************************************************/

    return (outIdx);

}  /* mmwavelib_cfarCa */

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarCadB
 *
 * \par
 * <b>Description</b>  :    Performs a CFAR on an 16-bit unsigned input vector (CFAR-CA). The input values are assumed to be
 *                          in lograthimic scale. So the comparision between the CUT and the noise samples is additive
 *                          rather than multiplicative.
 *
 * @param[in]               inp         : input array (16 bit unsigned numbers)
 * @param[in]               out         : output array (indices of detected peaks (zero based counting))
 * @param[in]               len         : number of elements in input array
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
 * Cycles (cgt 8.1.3)       3*len + 121
 *******************************************************************************************************************
 */
uint32_t mmwavelib_cfarCadB(const uint16_t inp[restrict],
                            uint16_t out[restrict], uint32_t len,
                            uint32_t const1, uint32_t const2,
                            uint32_t guardLen, uint32_t noiseLen)
{
    uint32_t idx, idxLeftNext, idxLeftPrev, idxRightNext, idxRightPrev, outIdx;
    uint32_t idxCUT;
    uint32_t sum, sumLeft, sumRight;

    /*initializations */
    outIdx = 0;
    sumLeft = 0;
    sumRight = 0;
    for (idx = 0; idx < noiseLen; idx++)
    {
        sumRight += inp[idx + guardLen + 1U];
    }

    /*********************************************************************************************/
    /*One-sided comparision for the first segment (for the first noiseLen+gaurdLen samples */
    idxCUT = 0;
    if ((uint32_t) inp[idxCUT] >
        ((sumRight >> (const2 - 1U)) + const1))
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

        if ((uint32_t) inp[idxCUT] >
            ((sumRight >> (const2 - 1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;
    }

    /*********************************************************************************************/
    /*Two-sided comparision for the middle segment  */
    sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
    idxRightNext++;
    idxRightPrev++;
    sum = sumRight + sumLeft;
    if ((uint32_t) inp[idxCUT] > ((sum >> const2) + const1))
    {
        out[outIdx] = (uint16_t)idxCUT;
        outIdx++;
    }
    
    idxCUT++;

    idxLeftPrev = 0;
    for (idx = 0; idx < (len - 2U * (noiseLen + guardLen) - 1U); idx++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = sumLeft + sumRight; /*CFAR-CA*/
        /*sum=(sumLeft>sumRight)?sumLeft:sumRight;//CFAR-CA-GO */
        if ((uint32_t) (inp[idxCUT]) > ((sum >> const2) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;
    }

    /*********************************************************************************************/
    /*One-sided comparision for the last segment (for the last noiseLen+gaurdLen samples)  */
    for (idx = 0; idx < (noiseLen + guardLen); idx++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        if ((uint32_t) inp[idxCUT] >
            ((sumLeft >> (const2 - 1U)) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
        idxCUT++;

    }
    /*********************************************************************************************/

    return (outIdx);

}  /* mmwavelib_cfarCadB */

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_cfarCadBwrap
 *
 * \par
 * <b>Description</b>  :    Performs a CFAR on an 16-bit unsigned input vector (CFAR-CA). The input values are assumed to be
 *                          in lograthimic scale. So the comparision between the CUT and the noise samples is additive
 *                          rather than multiplicative. Comparison is two-sided (wrap around when needed) for all CUTs.
 *
 * @param[in]               inp      : input array (16 bit unsigned numbers)
 * @param[in]               out      : output array (indices of detected peaks (zero based counting))
 * @param[in]               len      : number of elements in input array
 * @param[in]               const1,const2 : used to compare the Cell Under Test (CUT) to the sum of the noise cells:
 *                                          [noise sum /(2^(const2))] +const1 for two sided comparison.
 * @param[in]               guardLen : one sided guard length
 * @param[in]               noiseLen : one sided Noise length
 *
 * @param[out]              out      : output array with indices of the detected peaks
 *
 * @return                  Number of detected peaks (i.e length of out)
 *
 * @pre                     Input (inp) and Output (out) arrays are non-aliased.
 * @ingroup                 MMWAVELIB_DETECT
 *
 * Cycles (cgt 8.1.3)       3*len + 169
 *******************************************************************************************************************
 */
uint32_t mmwavelib_cfarCadBwrap(const uint16_t inp[restrict],
                                uint16_t out[restrict], uint32_t len,
                                uint32_t const1, uint32_t const2,
                                uint32_t guardLen, uint32_t noiseLen)
{
    uint32_t idx, idxLeftNext, idxLeftPrev, idxRightNext, idxRightPrev, outIdx;
    uint32_t idxCUT;
    uint32_t sum, sumLeft, sumRight;

    /*initializations */
    outIdx = 0U;
    sumLeft = 0U;
    sumRight = 0U;
    for (idx = 1U; idx <= noiseLen; idx++)
    {
        sumLeft += inp[len - guardLen - idx];
    }

    for (idx = 1U; idx <= noiseLen; idx++)
    {
        sumRight += inp[idx + guardLen];
    }

    /*CUT 0: */
    sum = sumLeft + sumRight;
    if ((uint32_t) inp[0] > ((sum >> const2) + const1))
    {
        out[outIdx] = 0U;
        outIdx++;
    }

    /* CUT 1 to guardLen: */
    idxLeftPrev = len - guardLen - noiseLen;    /*e.g. 32-4-8 = 20 */
    idxLeftNext = idxLeftPrev + noiseLen; /*e.g. 28 */
    idxRightPrev = 1U + guardLen;    /*e.g. 1+4=5 */
    idxRightNext = idxRightPrev + noiseLen;   /*e.g. 13 */
    for (idxCUT = 1U; idxCUT <= guardLen; idxCUT++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = sumLeft + sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> const2) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
    }

    /* CUT guardLen+1 to guardLen+noiseLen: e.g. CUT 5 to 12 */
    idxLeftNext = 0U;
    for (idxCUT = (guardLen + 1U); idxCUT <= (guardLen + noiseLen);
         idxCUT++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = sumLeft + sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> const2) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
    }

    /* CUTs in the middle. e.g. CUT 13 to 19 */
    idxLeftPrev = 0U;
    for (idxCUT = (guardLen + noiseLen + 1U);
         idxCUT < (len - (noiseLen + guardLen)); idxCUT++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = sumLeft + sumRight; /*CFAR-CA */
        /*sum=(sumLeft>sumRight)?sumLeft:sumRight;//CFAR-CA-GO */
        if ((uint32_t) (inp[idxCUT]) > ((sum >> const2) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
    }

    /*********************************************************************************************/
    /* noiseLen number of CUTs before the last guardLen CUTs. e.g. CUT 20 to 27 */
    idxRightNext = 0U;
    for (idxCUT = (len - (noiseLen + guardLen));
         idxCUT < (len - guardLen); idxCUT++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = sumLeft + sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> const2) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
    }

    /* The last guardLen number of CUTs */
    idxRightPrev = 0U;
    for (idxCUT = (len - guardLen); idxCUT < len; idxCUT++)
    {
        sumLeft = (sumLeft + inp[idxLeftNext]) - inp[idxLeftPrev];
        idxLeftNext++;
        idxLeftPrev++;
        sumRight = (sumRight + inp[idxRightNext]) - inp[idxRightPrev];
        idxRightNext++;
        idxRightPrev++;
        sum = sumLeft + sumRight;
        if ((uint32_t) (inp[idxCUT]) > ((sum >> const2) + const1))
        {
            out[outIdx] = (uint16_t)idxCUT;
            outIdx++;
        }
    }

    return (outIdx);

}  /* mmwavelib_cfarCadBwrap */
