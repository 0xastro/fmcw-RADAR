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
 *   @file     mmwavelib_accum.c
 *
 *   @brief    Routines for vector accumulation
 */

#include <stdint.h>
#include <c6x.h>
#include <ti/alg/mmwavelib/src/vecutils/mmwavelib_vecutils.h>


/*!**************************************************************************************
 * @brief       Function mmwavelib_accum16 accumlates input vector to the output vector.
 *              It is the optimized implementation of the for loop:
 *              for ( idx=0; idx<len; idx++ )
 *              {    out[idx] += inp[idx];//addition saturated to 0xFFFF }
 *
 * @param[in]    inp    : input vector of 16-bit samples
 * @param[inout] out    : output vector of 16-bit samples
 * @param[in]    len    : number of samples in vector
 *
 * @return      void
 *
 * @pre         (1)Input and output arrays both are 8 byte aligned
 * @pre         (2)Array length (len) is a multiple of 4
 *
 * \ingroup     MMWAVELIB_VEC_UTILS
 *
 * Cycles       1.1N+40
 ****************************************************************************************
 */
void mmwavelib_accum16(const uint16_t inp[restrict], uint16_t out[restrict],
                       uint32_t len)
{
    uint64_t *restrict l_inp;
    uint64_t *restrict l_out;
    uint32_t l_sumLow, l_sumHigh;
    uint32_t idx;

    l_inp = (uint64_t *) inp;
    l_out = (uint64_t *) out;

    _nassert(((uint32_t) l_inp % 8U) == 0);
    _nassert(((uint32_t) l_out % 8U) == 0);
    _nassert((len % 4U) == 0);

    for (idx = 0; idx < (len / 4U); idx++)
    {
        l_sumLow = (uint32_t)_saddsu2( (int32_t)_loll(l_inp[idx]), _loll(l_out[idx]) );
        l_sumHigh = (uint32_t)_saddsu2( (int32_t)_hill(l_inp[idx]), _hill(l_out[idx]) );
        l_out[idx] = (uint64_t)_itoll(l_sumHigh, l_sumLow);
    }

} /* mmwavelib_accum16 */

/*!************************************************************************************************
 * @brief           Function accumlates(adds) 16-bit input vector to the 32-bit output vector.
 *                  It is the optimized implementation of the for loop:
 *                  for ( i = 0; i < len; i++ )
 *                  {    output[i] += input[i]; }
 *
 * @param[in]        input  : input vector of 16-bit samples
 * @param[inout]     output : output vector of 32-bit samples
 * @param[in]        len    : number of samples
 *
 * @return           void
 *
 * @pre              (1)input and output arrays both are 8 byte aligned.
 * @pre              (2)len is a multiple of 2.
 *
 * \ingroup          MMWAVELIB_VEC_UTILS
 *
 * Cycles            0.75*len + 58 (cgt 8.1.3)
 **************************************************************************************************
 */
void mmwavelib_accum16to32(const int16_t input[restrict],
                           int32_t output[restrict],
                           uint32_t len)
{
    uint64_t *restrict l_inp;
    uint64_t *restrict l_out;
    int32_t  sumEven, sumOdd;
    int16_t  inEven, inOdd;
    uint32_t i;

    l_inp = (uint64_t *) input;
    l_out = (uint64_t *) output;

    _nassert(((uint32_t) l_inp % 8U) == 0);
    _nassert(((uint32_t) l_out % 8U) == 0);
    _nassert((len % 2U) == 0);

    for (i = 0; i < (len >> 2U); i++)
    {
        /* process 1st and 2nd samples */
        inEven = _ext(_loll(l_inp[i]), 16, 16);
        inOdd  = _ext(_loll(l_inp[i]), 0, 16);
        sumEven  = inEven + (int32_t)_loll(_amem8(l_out));
        sumOdd   = inOdd + (int32_t)_hill(_amem8(l_out));
        _amem8(l_out) = (uint64_t)_itoll(sumOdd, sumEven);
        l_out ++;

        /* process 3rd and 4th samples */
        inEven = _ext(_hill(l_inp[i]), 16, 16);
        inOdd  = _ext(_hill(l_inp[i]), 0, 16);
        sumEven  = inEven + (int32_t)_loll(_amem8(l_out));
        sumOdd   = inOdd + (int32_t)_hill(_amem8(l_out));
        _amem8(l_out) = (uint64_t)_itoll(sumOdd, sumEven);
        l_out ++;
    }

    /* process the last two samples when len is multiple of 2 but not multiple of 4 */
    if (len & 2U)
    {
        inEven = _ext(_loll(l_inp[i]), 16, 16);
        inOdd  = _ext(_loll(l_inp[i]), 0, 16);
        sumEven  = inEven + (int32_t)_loll(_amem8(l_out));
        sumOdd   = inOdd + (int32_t)_hill(_amem8(l_out));
        _amem8(l_out) = (uint64_t)_itoll(sumOdd, sumEven);
        l_out ++;
    }

} /* mmwavelib_accum16to32 */
