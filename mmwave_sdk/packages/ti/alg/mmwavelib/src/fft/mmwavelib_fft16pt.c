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
 *   @file     mmwavelib_fft16pt.c
 *
 *   @brief    Routine to compute 16pt FFT of a vector of length 8 (assumed
 *             to be zero-padded to length 16)
 *
 */

#include <stdint.h>
#include <ti/alg/mmwavelib/src/fft/mmwavelib_fft.h>

static void fft16pt_stage1(const int32_t * restrict inp,
                           const int32_t * restrict twiddle,
                           int32_t * restrict out, uint32_t numLines,
                           uint32_t numSkipSamples);
static void fft16pt_stage2(const int32_t * restrict inp,
                           int32_t * restrict out, uint32_t numLines);

/*!*****************************************************************************************************************
 * @brief               mmwavelib_fft16ptZeroPadded32x32
 *                      Computes the 16pt FFT of vectors of length 8. The input vectors are assumed to be in
 *                      transpose format (in inp). Hence the FFT can be thought of as operating along the columns
 *                      of inp
 *
 *
 * @param[in]           inp      : input complex array   (32 bit I, 32 bit Q). The input array is assumed to be
 *                                 a x[8][numLines*2] array, *2 is for complex, with each FFT operating along
 *                                 each columns resultingin an ouput of length 16.
 * @param[in]           twiddle  : twiddle factors array (32 bit I, 32 bit Q)
 * @param[in]           temp     : temporary buffer (sufficient to  store 16*numLines complex samples)
 * @param[in]           out      : output complex array (32 bit I, 32 bit Q) (size: out[numLines][16*2])
 * @param[in]           numLines : number of consecutive columns of inp for which the FFT must be run.
 * @param[in]           numSkipSamples : number of total columns L of inp if L>numLines.
 *                                       otherwise this equals to numLines.
 *
 * @return              void
 *
 * @pre                 Input array (inp), output array (out), twiddle factor array (twiddle), temp buffer (temp)
 *                      are all 8 byte aligned
 *
 * @ingroup             MMWAVELIB_FFT
 *******************************************************************************************************************
 */
void mmwavelib_fft16ptZeroPadded32x32(const int32_t * restrict inp,
                                      const int32_t * restrict twiddle,
                                      int32_t * restrict temp,
                                      int32_t * restrict out,
                                      uint32_t numLines,
                                      uint32_t numSkipSamples)
{
    /*
     *    A 16 pt FFT has two radix-4 stages.
     *    Since the FFT is zero padded (8 zeros), the first stage uses a simplified radix-4 butterfly with only two non-zero inputs.
     *    The second (last) stage, uses radix-4 butterfly with no multiplies. Each routine below is specially optimized for each of
     *    these stages.
     */
    fft16pt_stage1(inp, twiddle, temp, numLines, numSkipSamples);  /* This function runs the first stage  of the radix-4 FFT */
    fft16pt_stage2(temp, out, numLines);    /* This function runs the second stage  of the radix-4 FFT */
}

static void fft16pt_stage1(const int32_t * restrict inp,
                    const int32_t * restrict twiddle,
                    int32_t * restrict out, uint32_t numLines,
                    uint32_t numSkipSamples)
{
    uint32_t idx, idx1, idx2, idx3, idx4, idx5;
    int32_t a, b, tempOutReal, tempOutImag, twiddleReal, twiddleImag;
    const int64_t *restrict xl_inp, *restrict xl_twiddle;
    int64_t *restrict xl_out;
    int64_t xl_tempIn1, xl_tempIn2;

    xl_inp = (const int64_t *) inp;
    xl_out = (int64_t *) out;
    xl_twiddle = (const int64_t *) twiddle;

    _nassert((uint32_t) xl_inp % 8U == 0);
    _nassert((uint32_t) xl_out % 8U == 0);
    _nassert((uint32_t) xl_twiddle % 8U == 0);

    for (idx = 0; idx < numLines*4U; idx++)
    {
        idx1 = idx & 3U;
        idx2 = idx >> 2;
        idx3 = (idx2 * 16U) + idx1;

        idx4 = (idx1 * numSkipSamples) + idx2;
        idx5 = idx4 + 4U * numSkipSamples;

        xl_tempIn1 = xl_inp[idx4];
        xl_tempIn2 = xl_inp[idx5];

        b = _hill(xl_tempIn1) + _hill(xl_tempIn2);
        a = _loll(xl_tempIn1) + _loll(xl_tempIn2);
        tempOutReal = a;
        tempOutImag = b;
        xl_out[idx3] = _itoll(tempOutImag, tempOutReal);

        b = _hill(xl_tempIn1) - _loll(xl_tempIn2);
        a = _loll(xl_tempIn1) + _hill(xl_tempIn2);
        twiddleImag = _hill(xl_twiddle[idx1]);
        twiddleReal = _loll(xl_twiddle[idx1]);
        tempOutReal =
            _smpy32(a, twiddleReal) - _smpy32(b, twiddleImag);
        tempOutImag =
            _smpy32(a, twiddleImag) + _smpy32(b, twiddleReal);
        xl_out[idx3 + 4U] = _itoll(tempOutImag, tempOutReal);

        b = _hill(xl_tempIn1) - _hill(xl_tempIn2);
        a = _loll(xl_tempIn1) - _loll(xl_tempIn2);
        twiddleImag = _hill(xl_twiddle[2U * idx1]);
        twiddleReal = _loll(xl_twiddle[2U * idx1]);
        tempOutReal =
            _smpy32(a, twiddleReal) - _smpy32(b, twiddleImag);
        tempOutImag =
            _smpy32(a, twiddleImag) + _smpy32(b, twiddleReal);
        xl_out[idx3 + 8U] = _itoll(tempOutImag, tempOutReal);

        b = _hill(xl_tempIn1) + _loll(xl_tempIn2);
        a = _loll(xl_tempIn1) - _hill(xl_tempIn2);
        twiddleImag = _hill(xl_twiddle[3U * idx1]);
        twiddleReal = _loll(xl_twiddle[3U * idx1]);
        tempOutReal =
            _smpy32(a, twiddleReal) - _smpy32(b, twiddleImag);
        tempOutImag =
            _smpy32(a, twiddleImag) + _smpy32(b, twiddleReal);
        xl_out[idx3 + 12U] = _itoll(tempOutImag, tempOutReal);

    }
}

static void fft16pt_stage2(const int32_t * restrict inp, int32_t * restrict out,
                    uint32_t numLines)
{
    uint32_t idx, idx1, idx2;
    int32_t aReal, aImag, bReal, bImag, cReal, cImag, dReal, dImag;
    const int64_t *restrict xl_inp;
    int64_t *restrict xl_out;
    int64_t xl_tempIn1, xl_tempIn2, xl_tempIn3, xl_tempIn4;

    xl_inp = (const int64_t *) inp;
    xl_out = (int64_t *) out;

    _nassert((uint32_t) xl_inp % 8U == 0);
    _nassert((uint32_t) xl_out % 8U == 0);

    for (idx = 0; idx < 4U * numLines; idx++)
    {
        idx1 = idx & 3U;
        idx2 = (idx & 0xfffffffcU) * 4U + idx1;
        xl_tempIn1 = xl_inp[4U * idx];
        xl_tempIn2 = xl_inp[4U * idx + 1U];
        xl_tempIn3 = xl_inp[4U * idx + 2U];
        xl_tempIn4 = xl_inp[4U * idx + 3U];

        aImag = _hill(xl_tempIn1) + _hill(xl_tempIn3);
        aReal = _loll(xl_tempIn1) + _loll(xl_tempIn3);

        bImag = _hill(xl_tempIn1) - _hill(xl_tempIn3);
        bReal = _loll(xl_tempIn1) - _loll(xl_tempIn3);

        cImag = _hill(xl_tempIn2) + _hill(xl_tempIn4);
        cReal = _loll(xl_tempIn2) + _loll(xl_tempIn4);

        dImag = _hill(xl_tempIn2) - _hill(xl_tempIn4);
        dReal = _loll(xl_tempIn2) - _loll(xl_tempIn4);

        xl_out[idx2] = _itoll(aImag + cImag, aReal + cReal);
        xl_out[idx2 + 4U] = _itoll(bImag - dReal, bReal + dImag);
        xl_out[idx2 + 8U] = _itoll(aImag - cImag, aReal - cReal);
        xl_out[idx2 + 12U] = _itoll(bImag + dReal, bReal - dImag);

    }

}
