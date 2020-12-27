/**************************************************************************
 * File Name    : mmwavelib_scaling.c
 *
 * Description  : various routines for vector scaling
 *
 **************************************************************************
 * (C) Copyright 2014, Texas Instruments Incorporated -  http://www.ti.com/
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

#include <stdint.h>

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_complexScale32
 *
 * \par
 * <b>Description</b>  :    Multiplies a 32 bit complex vector with a 32 bit complex constant.The 64-bit result is
 *                          shifted left by 1 with saturation, and the 32 most-significant bits of the shifted value
 *                           are written to the output array
 *
 *
 * \param[in]               inp         : input complex array   (32 bit Q, 32 bit I)
 *                          constReal: real part of the complex constant
 *                          constImag: imaginary part of the complex constant
 *                          len         : number of complex samples in input array
 *
 * \param[in,out]      :
 *
 * \return             :     void
 *
 * \par
 * Assumptions         :    (1)Input (inp) is 8 byte aligned
 *                          (2)Input array length (len) is a multiple of 2
 * Cycles              :    2N+41
 *******************************************************************************************************************
 */

void mmwavelib_complexScale32(int32_t inp[restrict],
                              const int32_t constReal,
                              const int32_t constImag, uint32_t len)
{
    int64_t *restrict xl_inp;
    uint32_t idx;
    int32_t outReal, outImag;

    xl_inp = (int64_t *) inp;

    _nassert((uint32_t) xl_inp % 8 == 0);
    _nassert(len % 2 == 0);

    for (idx = 0; idx < len; idx++)
    {
        outImag =
            _smpy32(_loll(xl_inp[idx]),
                    constReal) + _smpy32(_hill(xl_inp[idx]), constImag);
        outReal =
            -_smpy32(_loll(xl_inp[idx]),
                     constImag) + _smpy32(_hill(xl_inp[idx]),
                                             constReal);
        xl_inp[idx] = _itoll(outReal, outImag);

    }

}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_complexScale16
 *
 * \par
 * <b>Description</b>  :    Multiplies an input complex vector  with a complex constant. The input
 *                          vector and the constant are 16 bit I and 16 bit Q. The output is written
 *                          in place and is also 16 bit I,16 bit Q (after appropriate rounding)
 *
 *
 *
 * \param[in]          :    inp          : input complex array   (16 bit I, 16 bit Q)
 *                          complexConst : complex constant (16 bit I, 16 bit Q)
 *                          len         : number of complex samples in input array
 *
 * \param[in,out]      :
 *
 * \return             :     void
 *
 * \par
 * Assumptions         :    (1)Input (inp) is double word aligned
 *                          (2)Input array length (len) is a multiple of 2
 *
 * Cycles              :     0.5*N+34
 *
 *******************************************************************************************************************
 */

void mmwavelib_complexScale16(int16_t inp[restrict],
                              const int32_t complexConst, uint32_t len)
{
    int64_t *restrict xl_inp;
    uint32_t idx;
    int32_t mult1, mult2;

    xl_inp = (int64_t *) inp;

    _nassert((uint32_t) xl_inp % 8 == 0);
    _nassert(len % 4 == 0);

    for (idx = 0; idx < len / 2; idx++)
    {
        mult1 = _cmpyr(_loll(xl_inp[idx]), complexConst);
        mult2 = _cmpyr(_hill(xl_inp[idx]), complexConst);
        xl_inp[idx] = _itoll(mult2, mult1);

    }

}

/*!*****************************************************************************************************************
 * \brief
 * Function Name       : mmwavelib_shiftRight
 *
 * \par
 * <b>Description</b>  : Right shift a real vector by a specified value
 *
 *
 * \param[in]            inp        : input array
 *                       out        : output array
 *                       len         : number of samples in input array
 *
 * \param[in,out]      :
 *
 * \return             :     void
 *
 * \par
 * Assumptions         :    (1)Input (inp), Output (out)
 *                          (2)Input array length (len) is a multiple of 16
 *                          (3)Input and output arrays are not aliased
 *
 *******************************************************************************************************************
 */

void mmwavelib_shiftRight16(const int16_t inp[restrict],
                            int16_t out[restrict], uint32_t shiftVal,
                            uint32_t len)
{
    int64_t *restrict xl_inp, *restrict xl_out;
    uint32_t idx;

    xl_inp = (int64_t *) inp;
    xl_out = (int64_t *) out;
    _nassert((uint32_t) xl_inp % 8 == 0);
    _nassert((uint32_t) xl_out % 8 == 0);
    _nassert(len % 8 == 0);
    #pragma UNROLL(2)
    for (idx = 0; idx < len >> 2; idx++)
    {
        xl_out[idx] =
            _itoll(_shr2(_hill(xl_inp[idx]), shiftVal),
                   _shr2(_loll(xl_inp[idx]), shiftVal));
    }

}
