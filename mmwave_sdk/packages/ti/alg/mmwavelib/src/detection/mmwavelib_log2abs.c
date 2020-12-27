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
 *   @file     mmwavelib_log2abs.c
 *
 *   @brief    This file contains routines for finding the log2 of absolute value of complex numbers
 */

#include <stdint.h>
#include <ti/alg/mmwavelib/src/detection/mmwavelib_detection.h>

static const uint8_t c_Log2LookUp[64] = {
    0, 6, 11, 17, 22, 28, 33, 38,
    44, 49, 54, 59, 63, 68, 73, 78,
    82, 87, 92, 96, 100, 105, 109, 113,
    118, 122, 126, 130, 134, 138, 142, 146,
    150, 154, 157, 161, 165, 169, 172, 176,
    179, 183, 186, 190, 193, 197, 200, 203,
    207, 210, 213, 216, 220, 223, 226, 229,
    232, 235, 238, 241, 244, 247, 250, 253
};

/*
From : http://dspguru.com/dsp/tricks/magnitude-estimator
=====================================================================
             Alpha * Max + Beta * Min Magnitude Estimator

Name                  Alpha           Beta       Avg Err   RMS   Peak
                                                 (linear)  (dB)  (dB)
---------------------------------------------------------------------
Min RMS Err      0.947543636291 0.392485425092   0.000547 -32.6 -25.6
Min Peak Err     0.960433870103 0.397824734759  -0.013049 -31.4 -28.1
Min RMS w/ Avg=0 0.948059448969 0.392699081699   0.000003 -32.6 -25.7
1, Min RMS Err   1.000000000000 0.323260990000  -0.020865 -28.7 -23.8
1, Min Peak Err  1.000000000000 0.335982538000  -0.025609 -28.3 -25.1
1, 1/2           1.000000000000 0.500000000000  -0.086775 -20.7 -18.6
1, 1/4           1.000000000000 0.250000000000   0.006456 -27.6 -18.7
Frerking         1.000000000000 0.400000000000  -0.049482 -25.1 -22.3
1, 11/32         1.000000000000 0.343750000000  -0.028505 -28.0 -24.8
1, 3/8           1.000000000000 0.375000000000  -0.040159 -26.4 -23.4 ***
15/16, 15/32     0.937500000000 0.468750000000  -0.018851 -29.2 -24.1
15/16, 1/2       0.937500000000 0.500000000000  -0.030505 -26.9 -24.1
31/32, 11/32     0.968750000000 0.343750000000  -0.000371 -31.6 -22.9
31/32, 3/8       0.968750000000 0.375000000000  -0.012024 -31.4 -26.1
61/64, 3/8       0.953125000000 0.375000000000   0.002043 -32.5 -24.3
61/64, 13/32     0.953125000000 0.406250000000  -0.009611 -31.8 -26.6
=====================================================================
*/
#define LUT_BASED_LOG_APPROX 1

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_log2Abs32
 *
 * @brief                   Takes a 32 bit complex input vector and computes the log2(abs(x[i])). The abs(a+j*b) is
 *                          approximated using (max(|a|,|b|) + min(|a|,|b|)*3/8). Subsequently, log2 is found using
 *                          a lookup table based approximation. The output is a 16 bit number in Q8 format. Thus each
 *                          value of the output is computed as round(log2(abs(x[i]))*2^8).
 *
 * @param[in]               inp     : input complex array   (32 bit I, 32 bit Q)
 * @param[in]               out     : output array (16 bit unsigned)
 * @param[in]               len      : number of complex elements in inp
 *
 * @return                  void
 *
 * @pre                     (1)Input and output arrays  are all 8 byte aligned.
 * @pre                     (2)Input array length (len) is a multiple of 4
 *
 * @ingroup                 MMWAVELIB_DETECT
 *
 * Cycles                   3.5*len + 89 (cgt 8.1.3)
 *******************************************************************************************************************
 */
void mmwavelib_log2Abs32(const int32_t inp[restrict],
                         uint16_t out[restrict], uint32_t len)
{
    uint32_t idx, idx1;
    uint32_t real, imag, min, max, absvalue;
    uint32_t log2Int, log2Fract, log2Result, log2Result4[4];
    uint64_t *restrict l_out = (uint64_t *) out;
    int64_t  xl_inp;

    _nassert(((uint32_t)inp % 8U) == 0);
    _nassert(((uint32_t)out % 8U) == 0);

    for (idx = 0; idx < 2U * len; idx += 8U)
    {

#pragma UNROLL(4);
        for (idx1 = 0U; idx1 < 4U; idx1++)
        {
            xl_inp = (_mem8_const(&inp[idx + 2U * idx1]));

            real = _abs(_loll(xl_inp));
            imag = _abs(_hill(xl_inp));

            max = imag;
            min = real;

            if (real > imag)
            {
                max = real;
                min = imag;
            }

            /* Approximate absolute function.
             * abs (x[i] + ix[i+1]) \approx  (max + min*3/8) */
            absvalue = max + ((min * 3U) >> 3U);

            /* Extract the number of leading 0s to create the integer part of log2. */
            log2Int = 31U - _lmbd(1, absvalue);
            if (absvalue == 0)
            {
                log2Int = 0;
            }

            /* create a 6 bit fractional part */
            log2Fract = _extu(absvalue, 32U - log2Int, 26U);

            /* compute the log2 by combining the integer and the fractional
             * part. */

#ifdef LUT_BASED_LOG_APPROX
            log2Result = (log2Int << 8U) + c_Log2LookUp[log2Fract];
#else
            log2Result = (log2Int << 8U) + log2Fract;
#endif
            log2Result4[idx1] = log2Result;

        }

        l_out[idx >> 3U] =
            _itoll(_pack2(log2Result4[3], log2Result4[2]),
                   _pack2(log2Result4[1], log2Result4[0]));

    }
}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_log2Abs16
 *
 * \par
 * <b>Description</b>  :    Takes a 16 bit complex input vector and computes the log2(|x[i]|^2). First the magnitude
 *                          square of each complex numer is taken. Subsequently, log2 is found using a look-up
 *                          table (LUT) based approximation. The output is a 16 bit number in Q8 format.
 *                          Thus each value of the output is computed as round(log2(|x[i]|)*2^8),
 *                          where log2() is the LUT based approximation.
 *
 * @param[in]               inp     : input complex array   (16 bit I, 16 bit Q)
 * @param[in]               out     : output array (16 bit unsigned)
 * @param[in]               len     : number of complex elements in inp
 *
 * @return                  void
 *
 * @ingroup                 MMWAVELIB_DETECT
 *
 * @pre                     (1)Input and Output arrays are all 8 byte aligned.
 * @pre                     (2)Input array length (len) is a multiple of 2
 *
 *******************************************************************************************************************
 */
void mmwavelib_log2Abs16(const int16_t inp[restrict],
                         uint16_t out[restrict], uint32_t len)
{
    uint32_t idx;
    uint32_t magSq, log2Fract;
    uint32_t log2Int;
    uint32_t log2Result1, log2Result2;
    uint32_t *out32 = (uint32_t *) out;

    _nassert(((uint32_t)inp % 8U) == 0);
    _nassert(((uint32_t)out % 8U) == 0);

#pragma UNROLL (2)
    /* Each iteration below processes two consecutive complex numbers */
    for (idx = 0U; idx < (2U * len); idx += 4U)
    {

        /****************** Computations for first complex number **********************/
        magSq =
            _dotp2(_loll(_mem8_const(&inp[idx])),
                   _loll(_mem8_const(&inp[idx])));

        /* Extract the number of leading 0s to create the integer part of log2. */
        log2Int = 31U - _lmbd(1U, magSq);
        if (magSq==0)
        {
            log2Int = 0;
        }

        /* create a 6 bit fractional part */
        log2Fract = (magSq << (32U - log2Int)) >> 26U;

        /* compute the log2 by combining the integer and the fractional  part. */
        log2Result1 = (log2Int << 8U) + c_Log2LookUp[log2Fract];

        /********************************************************************************/

        /****************** Computations for second complex number **********************/
        magSq =
            _dotp2(_hill(_mem8_const(&inp[idx])),
                   _hill(_mem8_const(&inp[idx])));

        /* Extract the number of leading 0s to create the integer part of log2. */
        log2Int = 31U - _lmbd(1U, magSq);
        if (magSq==0)
        {
            log2Int = 0;
        }

        /* create a 6 bit fractional part */
        log2Fract = (magSq << (32U - log2Int)) >> 26;

        /* compute the log2 by combining the integer and the fractional part. */
        log2Result2 = (log2Int << 8U) + c_Log2LookUp[log2Fract];

        /********************************************************************************/

        out32[idx >> 2] = _pack2((int16_t)log2Result2, (int16_t)log2Result1);
    }

}
