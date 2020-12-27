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
 *   @file     mmwavelib_vecmul.c
 *
 *   @brief    Routines to multiply(element-wise) complex vectors
 *
 */

#include <stdint.h>
#include <c6x.h>
#include "mmwavelib_vecutils.h"

/*!************************************************************************************************
 * @brief              Function multiply two 16-bit complex vectors element by element.
 *                     The math is as follows:
 *                     for ( i=0; i < nx; i++)
 *                     {
 *                         outreal[i] = sat((inreal[i]*wreal[i]-inimag[i]*wimag[i]+0x4000)*2)/2^16;
 *                         outimag[i] = sat((inreal[i]*wimag[i]+inimag[i]*wreal[i]+0x4000)*2)/2^16;
 *                     }
 *
 * @param[inout]       x  : Input array. Output will be written in place.
 * @param[in]          w  : The second input array, e.g. the array of windowing coefficients.
 * @param[in]          nx : Number of complex elements.
 *
 * @return             void
 *
 * @pre                (1)Arrays are 8 byte aligned.
 * @pre                (2)nx is a multiple of 4.
 * @pre                (3)Imaginary/real components are stored in adjacent locations in the arrays.
 *                        The imaginary components are stored at even array indices,
 *                        and the real components are stored at odd array indices. 
 *
 * @ingroup            MMWAVELIB_VEC_UTILS
 *
 * Cycles              0.75*nx + 38 (cgt8.1.3)
 **************************************************************************************************
 */
void mmwavelib_vecmul16x16(int16_t * restrict x,
                           const int16_t * restrict w,
                           uint32_t nx)
{
    const int64_t *wl;
    int64_t *xl;
    uint32_t i;
    uint32_t insamp1, insamp2;
    uint32_t outsamp1, outsamp2;

    xl = ( int64_t *)x;
    wl = (const int64_t *)w;

    _nassert((uint32_t)xl % 8U == 0);/* 8 byte aligned */
    _nassert((uint32_t)wl % 8U == 0);
    _nassert(nx % 4U == 0);
#pragma UNROLL(2)
    for (i = 0; i < nx/2U; i++)
    {
        insamp1 = _loll(xl[i]);
        insamp2 = _hill(xl[i]);
        /*
         * In the cases where input array x has real in even locations and imag in odd locations,
         * will need to swap real and imag, in order to have real in MSB16 in 32-bit sample.
         * Just use the two lines below to replace above 2 lines.
        insamp1 = _swap2(_loll(xl[i]));
        insamp2 = _swap2(_hill(xl[i]));

         * The _cmpyr1 intrinsic assumes 32-bit samples have real in MSB16, imag in LSB16.
         * The 32-bit outsamp1, outsamp2 each has its real in MSB16, imag in LSB16.
         * i.e. output data in the 16bit array has real in odd locations, imag in even locations 
         * on Little Endian DSPs.
         */
        outsamp1 = _cmpyr1(insamp1,_loll(wl[i]));
        outsamp2 = _cmpyr1(insamp2,_hill(wl[i]));
        xl[i]    = _itoll(outsamp2, outsamp1);
    }

}/* mmwavelib_vecmul16x16 */


/*!************************************************************************************************
 * @brief              Function performs element-wise complex multiplication on a 16bit vector
 *                     and a 32bit vector.
 *                     The math is as follows:
 *                     for ( i=0; i < nx; i++)
 *                     {
 *                         yreal[i] = round(xreal[i]*wreal[i]/2^15)-round(ximag[i]*wimag[i]/2^15);
 *                         yimag[i] = round(xreal[i]*wimag[i]/2^15)+round(ximag[i]*wreal[i]/2^15);
 *                     }
 *
 * @param[in]          x : Pointer to 16-bit input array.
 * @param[in]          w : Pointer to 32-bit input array.
 * @param[out]         y : Pointer to 32-bit output array.
 * @param[in]          nx: Number of complex elements.
 *
 * @return             void
 *
 * @pre                (1)input and output arrays are 8 byte aligned.
 * @pre                (2)nx is a multiple of 4.
 * @pre                (3)Input x,w: The imaginary components are stored at even array indices,
 *                        and the real components are stored at odd array indices.
 *                        Output y: The real components are stored at even array indices,
 *                        and the imaginary components are stored at odd array indices. 
 *
 * @ingroup            MMWAVELIB_VEC_UTILS
 *
 * Cycles              1.5*nx + 43 (cgt8.1.3)
 **************************************************************************************************
 */
void mmwavelib_vecmul16x32(const int16_t * restrict x,
                           int32_t * restrict w,
                           int32_t * restrict y,
                           uint32_t nx)
{
    const int64_t * restrict xl;
    int64_t * restrict wl;
    int64_t * restrict yl;
    uint32_t insamp;
    int32_t wI, wQ;
    int32_t yI, yQ;
    int64_t lltempa, lltempb;
    uint32_t i;

    xl = (const int64_t *) x;
    wl = (int64_t *)w;
    yl = (int64_t *) y;

    _nassert((uint32_t)xl % 8U == 0);/* 8 byte aligned */
    _nassert((uint32_t)wl % 8U == 0);
    _nassert((uint32_t)yl % 8U == 0);
    _nassert(nx % 4U == 0);
#pragma MUST_ITERATE(2,,2)
#pragma UNROLL(2)
    for(i = 0; i < nx/2U; i ++)
    {
        /*insamp: real in MSB16, imag in LSB16
         * when array x has real in odd locations on Little Endian DSPs.*/
        insamp  = _loll(xl[i]);

        /* Assuming array w has real in odd locations on Little Endian DSPs.*/
        wI      = (int32_t) _hill(wl[2*i]);
        wQ      = (int32_t) _loll(wl[2*i]);

        /* lsb32((msb16(insamp) * wI +0x4000) >>15) --> msb32 of lltempa */
        /* lsb32((lsb16(insamp) * wI +0x4000) >>15) --> lsb32 of lltempa */
        lltempa = _mpy2ir(insamp, wI);
        /* lsb32((msb16(insamp) * wQ +0x4000) >>15) --> msb32 of lltempb */
        /* lsb32((lsb16(insamp) * wQ +0x4000) >>15) --> lsb32 of lltempb */
        lltempb = _mpy2ir(insamp, wQ);

        yI      = (int32_t)_hill(lltempa) - (int32_t)_loll(lltempb);
        yQ      = (int32_t)_hill(lltempb) + (int32_t)_loll(lltempa);
        /*64bit yl: imag in MSB32, real in LSB32;
         * i.e. real will be in even locations of 32bit array y on Little Endian DSPs.
         * In case array y needs real in odd locations, just use _itoll(yI, yQ)*/
        yl[2*i] = _itoll(yQ, yI);

        /* Process the next complex sample: */
        insamp  = _hill(xl[i]);
        wI      = (int32_t) _hill(wl[2*i+1]);
        wQ      = (int32_t) _loll(wl[2*i+1]);

        lltempa = _mpy2ir(insamp, wI);
        lltempb = _mpy2ir(insamp, wQ);

        yI      = (int32_t)_hill(lltempa) - (int32_t)_loll(lltempb);
        yQ      = (int32_t)_hill(lltempb) + (int32_t)_loll(lltempa);
        yl[2*i+1] = _itoll(yQ, yI);
    }

}/* mmwavelib_vecmul16x32 */


/*!************************************************************************************************
 * @brief              Function performs element-wise complex multiplication on a 16bit vector
 *                     and a 32bit vector.
 *                     The math is as follows:
 *                     for ( i=0; i < nx; i++)
 *                     {
 *                         yreal[i] = round(xreal[i]*wreal[i]/2^15)-round(ximag[i]*wimag[i]/2^15);
 *                         yimag[i] = round(xreal[i]*wimag[i]/2^15)+round(ximag[i]*wreal[i]/2^15);
 *                     }
 *
 * @param[in]          x  : Pointer to 16-bit complex input array.
 * @param[in]          w  : Pointer to 32-bit complex input array.
 * @param[out]         y  : Pointer to 32-bit complex output array.
 * @param[in]          len: Number of complex elements.
 *
 * @return             void
 *
 * @pre                (1)input and output arrays are 8 byte aligned.
 * @pre                (2)x: The imaginary components are in lower 16bits of 32bits,
 *                        and the real components are in higher 16bits of 32bits.
 *                        w, y: The real components are in lower 32bits of 64bits,
 *                        and the imaginary components are in higher 32bits of 64bits. 
 *
 * @ingroup            MMWAVELIB_VEC_UTILS
 *
 * Cycles              2.25*len + 50 (cgt8.1.3)
 **************************************************************************************************
 */
void mmwavelib_vecmul16x32_anylen(const uint32_t * restrict x,
                             int64_t * restrict w,
                             int64_t * restrict y,
                             uint32_t len)
{
    int64_t wQI;
    uint32_t xIQ;
    int32_t wI, wQ;
    int32_t yI, yQ;
    int64_t lltempa, lltempb;
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        xIQ = x[i]; /* I in msb16(x) Q in lsb16(x)*/
        wQI = w[i];
        wI  = (int32_t) _loll(wQI);
        wQ  = (int32_t) _hill(wQI);

        /* lsb32((xI * wI +0x4000) >>15) --> msb32 of lltempa */
        /* lsb32((xQ * wI +0x4000) >>15) --> lsb32 of lltempa */
        lltempa = _mpy2ir(xIQ, wI);
        /* lsb32((xI * wQ +0x4000) >>15) --> msb32 of lltempb */
        /* lsb32((xQ * wQ +0x4000) >>15) --> lsb32 of lltempb */
        lltempb = _mpy2ir(xIQ, wQ);

        yI      = _ssub((int32_t)_hill(lltempa), (int32_t)_loll(lltempb));
        yQ      = _sadd((int32_t)_hill(lltempb), (int32_t)_loll(lltempa));

        y[i] =  _itoll(yQ, yI);
    }
}

/*!************************************************************************************************
 * @brief              Function multiplies a 16bit complex constant to a 32bit complex vector.
 *                     The math is as follows:
 *                     for ( i=0; i < nx; i++)
 *                     {
 *                         yreal[i] = round(xreal*wreal[i]/2^15)-round(ximag*wimag[i]/2^15);
 *                         yimag[i] = round(xreal*wimag[i]/2^15)+round(ximag*wreal[i]/2^15);
 *                     }
 *
 * @param[in]          x  : constant complex value.16bitI|16bitQ packed in 32bits.
 * @param[in]          w  : Pointer to input array. 32bitQ|32bitI packed in each 64bits.
 * @param[out]         y  : Pointer to output array. 32bitQ|32bitI packed in each 64bits.
 * @param[in]          len: Number of complex elements.
 *
 * @return             void
 *
 * @pre                (1)input and output arrays are 8 byte aligned.
 * @pre                (2)x: The imaginary component is in lower 16bits of 32bits,
 *                        and the real component is in higher 16bits of 32bits.
 *                        w and y: The real components are in lower 32bits of 64bits,
 *                        and the imaginary components are in higher 32bits of 64bits. 
 *
 * @ingroup            MMWAVELIB_VEC_UTILS
 *
 * Cycles              2*len + 54 (cgt8.1.3)
 **************************************************************************************************
 */
void mmwavelib_vecmul32x16c(const uint32_t x,
                       int64_t * restrict w,
                       int64_t * restrict y,
                       uint32_t len)
{
    int64_t wQI;
    int32_t wI, wQ;
    int32_t yI, yQ;
    int64_t lltempa, lltempb;
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        wQI = w[i];
        wI  = (int32_t) _loll(wQI);
        wQ  = (int32_t) _hill(wQI);

        /* x: I in msb16(x) Q in lsb16(x)*/
        /* lsb32((msb16(x) * wI +0x4000) >>15) --> msb32 of lltempa */
        /* lsb32((lsb16(x) * wI +0x4000) >>15) --> lsb32 of lltempa */
        lltempa = _mpy2ir(x, wI);
        /* lsb32((msb16(x) * wQ +0x4000) >>15) --> msb32 of lltempb */
        /* lsb32((lsb16(x) * wQ +0x4000) >>15) --> lsb32 of lltempb */
        lltempb = _mpy2ir(x, wQ);

        yI      = _ssub((int32_t)_hill(lltempa), (int32_t)_loll(lltempb));
        yQ      = _sadd((int32_t)_hill(lltempb), (int32_t)_loll(lltempa));

        y[i] =  _itoll(yQ, yI);
    }
}
