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
 *   @file     mmwavelib_windowing.c
 *
 *   @brief    Contains functions related to windowing of data (typically
 *             prior to an FFT)
 *
 */

#include <stdint.h>
#include <ti/alg/mmwavelib/src/fft/mmwavelib_fft.h>

/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_windowing16xl6
 *
 * \par
 * <b>Description</b>  :    Multiplies a 16 bit complex vector with a 16 bit real windowing vector.
 *                          The rounded result is stored inplace.
 *                          The math is as follows:
 *                            output_real[n]=round(input_real[n]*window[n]/2^15);
 *                            output_imag[n]=round(input_imag[n]*window[n]/2^15)
 *
 * @param[in]               inp     : Input complex array (16bit I followed by 16bit Q).
 *                                    Output is written inplace.
 * @param[in]               win     : Windowing real array (16 bit) of length len/2.
 * @param[in]               len     : Number of complex elements in inp.
 *
 * @return                  void
 *
 * @pre                     (1)Input (inp) and Windowing array (win) are all 8 byte aligned.
 * @pre                     (2)Input array length (len) is a multiple of 8.
 * @pre                     (3)Window is assumed to be symmetric. So only the
 *                             first len/2 elements of the window array are accessed.
 * @ingroup                 MMWAVELIB_FFT
 *
 * Cycles (cgt 8.1.3)       0.59375*len +54
 **************************************************************************************************
 */
void mmwavelib_windowing16x16(int16_t inp[restrict],
                              const int16_t win[restrict], uint32_t len)
{

    uint32_t temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
    uint32_t wina, winb;
    uint32_t tempw1, tempw2, tempw3, tempw4;
    uint32_t lenBy2, idx;
    int64_t *restrict xl_top, *restrict xl_bottom;
    int64_t *restrict xl_win;

    xl_top = (int64_t *) inp;
    xl_bottom = (int64_t *) inp;
    xl_win = (int64_t *) win;
    lenBy2 = len >> 1;

    _nassert((uint32_t)xl_top % 8U == 0);
    _nassert((uint32_t)xl_bottom % 8U == 0);
    _nassert((uint32_t)win % 8U == 0);
    _nassert(len % 16U == 0);
    _nassert(lenBy2 % 8U == 0);

#pragma UNROLL(2)
    for (idx = 0; idx < len / 8U; idx++)
    {
        /* load 4 16-bit real window factors  */
        wina = _loll(xl_win[idx]);
        winb = _hill(xl_win[idx]);
        tempw1 = (wina & 0xFFFFU) << 16U;
        tempw2 = (wina) & 0xFFFF0000U;
        tempw3 = (winb & 0xFFFFU) << 16U;
        tempw4 = (winb) & 0xFFFF0000U;

        /* process 4 complex input samples in top-down direction  */
        temp1 = _cmpyr1(_loll(xl_top[2U * idx]), (tempw1));
        temp2 = _cmpyr1(_hill(xl_top[2U * idx]), (tempw2));
        xl_top[2U * idx] = _itoll(temp2, temp1);

        temp3 = _cmpyr1(_loll(xl_top[2U * idx + 1U]), (tempw3));
        temp4 = _cmpyr1(_hill(xl_top[2U * idx + 1U]), (tempw4));
        xl_top[2U*idx + 1U] = _itoll(temp4, temp3);

        /* process 4 complex input samples in bottom-up direction  */
        temp5 =
            _cmpyr1(_loll(xl_bottom[lenBy2 - 2U * idx - 1U]), (tempw2));
        temp6 =
            _cmpyr1(_hill(xl_bottom[lenBy2 - 2U * idx - 1U]), (tempw1));
        xl_bottom[lenBy2 - 2U * idx - 1U] = _itoll(temp6, temp5);

        temp7 =
            _cmpyr1(_loll(xl_bottom[lenBy2 - 2U * idx - 2U]), (tempw4));
        temp8 =
            _cmpyr1(_hill(xl_bottom[lenBy2 - 2U * idx - 2U]), (tempw3));
        xl_bottom[lenBy2 - 2U * idx - 2U] = _itoll(temp8, temp7);
    }

} /* mmwavelib_windowing16x16 */

/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_windowing16x32
 *
 * \par
 * <b>Description</b>  :    Multiplies a 16 bit complex vector with a 32 bit real symmetric 
 *                          window vector. The result is stored as a 32 bit complex vector, with
 *                          same Re/Im order as input complex signal.
 *                          The math is as follows:
 *                            output_real[n]=round(input_real[n]*window[n]/2^15);
 *                            output_imag[n]=round(input_imag[n]*window[n]/2^15)
 *
 * @param[in]               inp     : Input complex array (16bit I followed by 16 bit Q)
 * @param[in]               win     : Windowing real array (32 bit) of length len/2 
 *                                    (since its a symmetric window)
 * @param[in]               out     : Output complex array (32 bit I followed by 32 bit Q)
 * @param[in]               len     : Number of complex elements in inp
 *
 * @return                  void
 *
 * @pre                     (1)Input (inp), Output (out) and Windowing array (win) are all 8 
 *                             byte aligned.
 * @pre                     (2)Input array length (len) is a multiple of 4
 * @pre                     (3)Window is assumed to be symmetric. So only the first len/2 
 *                             elements of the window array are accessed.
 * @ingroup                 MMWAVELIB_FFT
 *
 * Cycles (cgt 8.1.3)       1.25*len +47
 **************************************************************************************************
 */
void mmwavelib_windowing16x32(const int16_t inp[restrict],
                              const int32_t win[restrict],
                              int32_t out[restrict], uint32_t len)
{
    const int64_t *restrict xl_inp;
    int64_t *restrict xl_out;
    int64_t xl_temp1, xl_temp2, xl_temp3, xl_temp4;
    uint32_t idx;
    int32_t wina, winb;
    const int64_t *xl_win;

    xl_inp = (const int64_t *)inp;
    xl_out = (int64_t *) out;
    xl_win = (const int64_t *)win;

    /*
     * Each iteration of the loop performs the windowing operation on 4 complex input samples.
     *  So the first iteration processes input complex vectors 0,1,len-1,len-2.
     *    The second iteration processes input complex vectors  2,3,len-3,len-4 etc.
     */
    _nassert((uint32_t) xl_inp % 8U == 0);
    _nassert((uint32_t) xl_out % 8U == 0);
    _nassert((uint32_t) xl_win % 8U == 0);
    _nassert(len % 8U == 0);


    for (idx = 0; idx < len / 4U; idx++)
    {
        /* read 2 window elements */
        wina = (int32_t) _loll(xl_win[idx]);
        winb = (int32_t) _hill(xl_win[idx]);

        /* perform windowing on 4 complex input samples */
        xl_temp1 = _mpy2ir((uint32_t) _loll(xl_inp[idx]), wina);
        xl_temp2 = _mpy2ir((uint32_t) _hill(xl_inp[idx]), winb);
        xl_temp3 =
            _mpy2ir((uint32_t)
                    _hill(xl_inp[(uint32_t) (len / 2U - idx - 1U)]), wina);
        xl_temp4 =
            _mpy2ir((uint32_t)
                    _loll(xl_inp[(uint32_t) (len / 2U - idx - 1U)]), winb);

        /* output data for 4 complex samples */
        xl_out[2U*idx] = xl_temp1;
        xl_out[2U*idx + 1U] = xl_temp2;
        xl_out[len - 2U*idx - 1U] = xl_temp3;
        xl_out[len - 2U*idx - 2U] = xl_temp4;
    }

} /* mmwavelib_windowing16x32 */

/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_windowing16x32_IQswap
 *
 * \par
 * <b>Description</b>  :    Multiplies a 16 bit complex vector with a 32 bit real symmetric 
 *                          window vector, The result is stored as a 32 bit complex vector, with 
 *                          swapped Re/Im order compared with the input signal.
 *                          The math is as follows:
 *                            output_real[n]=round(input_real[n]*window[n]/2^15);
 *                            output_imag[n]=round(input_imag[n]*window[n]/2^15)
 *
 * @param[in]               inp     : Input complex array (16bit I followed by 16 bit Q)
 * @param[in]               win     : Windowing real array (32 bit) of length len/2 
 *                                    (since its a symmetric window)
 * @param[in]               out     : Output complex array (32 bit Q followed by 32 bit I)
 * @param[in]               len     : Number of complex elements in inp
 *
 * @return                  void
 *
 * @pre                     (1)Input (inp), Output (out) and Windowing array (win) are all 
 *                             8 byte aligned.
 * @pre                     (2)Input array length (len) is a multiple of 4
 * @pre                     (3)Window is assumed to be symmetric. So only the first len/2 
 *                             elements of the window array are accessed.
 * @ingroup                 MMWAVELIB_FFT
 *
 * Cycles (cgt 8.1.3)       1.25*len + 36
 **************************************************************************************************
 */
void mmwavelib_windowing16x32_IQswap(const int16_t inp[restrict],
                              const int32_t win[restrict],
                              int32_t out[restrict], uint32_t len)
{
    const int64_t *restrict xl_inp;
    int64_t *restrict xl_out;
    int64_t xl_temp1, xl_temp2, xl_temp3, xl_temp4;
    uint32_t idx;
    int32_t wina, winb;
    const int64_t *xl_win;
    uint32_t sample0, sample1, sample2, sample3;

    xl_inp = (const int64_t *)inp;
    xl_out = (int64_t *) out;
    xl_win = (const int64_t *)win;

    /*
     * Each iteration of the loop performs the windowing operation on 4 complex input samples.
     * So the first iteration processes input complex vectors 0,1,len-1,len-2.
     * The second iteration processes input complex vectors  2,3,len-3,len-4 etc.
     */
    _nassert((uint32_t) xl_inp % 8U == 0);
    _nassert((uint32_t) xl_out % 8U == 0);
    _nassert((uint32_t) xl_win % 8U == 0);
    _nassert(len % 8U == 0);


    for (idx = 0; idx < len / 4U; idx++)
    {
        /* read 2 window elements */
        wina = (int32_t) _loll(xl_win[idx]);
        winb = (int32_t) _hill(xl_win[idx]);
        
        /* perform windowing on 4 complex input samples */
        sample0 = _swap2(_loll(xl_inp[idx]));
        sample1 = _swap2(_hill(xl_inp[idx]));
        xl_temp1 = _mpy2ir(sample0, wina);
        xl_temp2 = _mpy2ir(sample1, winb);
        
        sample2 = _swap2(_hill(xl_inp[(uint32_t) (len / 2U - idx - 1U)]));
        sample3 = _swap2(_loll(xl_inp[(uint32_t) (len / 2U - idx - 1U)]));
        xl_temp3 = _mpy2ir(sample2, wina);
        xl_temp4 = _mpy2ir(sample3, winb);

        /* output data for 4 complex samples */
        xl_out[2U*idx] = xl_temp1;
        xl_out[2U*idx + 1U] = xl_temp2;
        xl_out[len - 2U*idx - 1U] = xl_temp3;
        xl_out[len - 2U*idx - 2U] = xl_temp4;
    }

} /* mmwavelib_windowing16x32_IQswap */




/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_windowing16xl6_evenlen
 *
 * \par
 * <b>Description</b>  :    Multiplies a 16 bit complex vector with a 16 bit real windowing vector.
 *                          The rounded result is stored inplace.
 *                          The math is as follows:
 *                            output_real[n]=round(input_real[n]*window[n]/2^15);
 *                            output_imag[n]=round(input_imag[n]*window[n]/2^15)
 *
 * @param[in]               inp     : Input complex array (16bit I followed by 16bit Q).
 *                                    Output is written inplace.
 * @param[in]               win     : Windowing real array (16 bit) of length len/2.
 * @param[in]               len     : Number of complex elements in inp.
 *
 * @return                  void
 *
 * @pre                     (1)Input (inp) and Windowing array (win) are all 8 byte aligned.
 * @pre                     (2)Input array length (len) is multiple of 2.
 * @pre                     (3)Window is assumed to be symmetric. So only the
 *                             first len/2 elements of the window array are accessed.
 * @ingroup                 MMWAVELIB_FFT
 *
 * Cycles (cgt 8.1.3)       0.59375*len + ovh; len%16=0: ovh=~79; len%16=6,8,14: ovh=~88; 
 *                          0.6548 *len + ovh; len%16=2,4: ovh=~82; len%16=10,12: ovh=~90.
 **************************************************************************************************
 */
void mmwavelib_windowing16x16_evenlen(int16_t inp[restrict],
                                      const int16_t win[restrict],
                                      uint32_t len)
{

    uint32_t temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
    uint32_t wina, winb;
    uint32_t tempw1, tempw2, tempw3, tempw4;
    uint32_t lenBy2, idx=0, k;
    uint32_t lenBy16, lenMod16;
    int64_t *restrict xl_top, *restrict xl_bottom;
    const int64_t *restrict xl_win;
    uint32_t *restrict inword, *restrict inword_b;

    xl_top = (int64_t *) inp;
    xl_bottom = (int64_t *) inp;
    xl_win = (const int64_t *) win;
    lenBy2 = len >> 1U;
    lenBy16 = len >>4U;
    lenMod16 = len & 0xFU;

	inword = (uint32_t *) inp;
    inword_b = (uint32_t *) inp;
    
    /*Process len%16 complex input samples in the middle of input buffer.
    Natual code as below:
    int16_t *restrict inp_bottom = inp;
    for (k = len - lenMod16; k < len; k+=2)
    {
        inp[k] = ((int32_t)inp[k]*win[k/2] + 0x4000) >>15;
        inp[k+1] = ((int32_t)inp[k+1]*win[k/2] + 0x4000) >>15;
        inp_bottom[2*len -k -1] = ((int32_t)inp_bottom[2*len -k -1]*win[k/2] + 0x4000) >>15;
        inp_bottom[2*len -k -2] = ((int32_t)inp_bottom[2*len -k -2]*win[k/2] + 0x4000) >>15;
    }*/
    for (k = lenBy2 - lenMod16/2U; k < lenBy2; k++)
    {
        tempw1 = ((uint32_t)win[k] ) << 16U;
        inword[k] = _cmpyr1(inword[k], tempw1);
        inword_b[len -k -1U]=  _cmpyr1(inword_b[len - k -1U], tempw1);
    }

    /*Process total (len/16)*16 complex input samples, from buffer top/bottom towards middle.
    Natual code for (k = 0; k < len - lenMod16; k+=2) with same loop content as above.
    Optimized loop content below is to process 8 complex input samples,
    with UNROLL(2), 16 complex input sampels are preocessed at a time.*/
    _nassert((uint32_t)xl_top % 8U == 0);
    _nassert((uint32_t)xl_bottom % 8U == 0);
    _nassert((uint32_t)win % 8U == 0);

#pragma UNROLL(2) 
    for (idx = 0; idx < lenBy16*2U; idx++)
    {
        /* load 4 16-bit real window factors  */
        wina = _loll(xl_win[idx]);
        winb = _hill(xl_win[idx]);
        tempw1 = (wina & 0xFFFFU) << 16U;
        tempw2 = (wina) & 0xFFFF0000U;
        tempw3 = (winb & 0xFFFFU) << 16U;
        tempw4 = (winb) & 0xFFFF0000U;

        /* process 4 complex input samples in top-down direction  */
        temp1 = _cmpyr1(_loll(xl_top[2U * idx]), (tempw1));
        temp2 = _cmpyr1(_hill(xl_top[2U * idx]), (tempw2));
        xl_top[2U * idx] = _itoll(temp2, temp1);

        temp3 = _cmpyr1(_loll(xl_top[2U * idx + 1U]), (tempw3));
        temp4 = _cmpyr1(_hill(xl_top[2U * idx + 1U]), (tempw4));
        xl_top[2U*idx + 1U] = _itoll(temp4, temp3);

        /* process 4 complex input samples in bottom-up direction  */
        temp5 =
            _cmpyr1(_loll(xl_bottom[lenBy2 - 2U * idx - 1U]), (tempw2));
        temp6 =
            _cmpyr1(_hill(xl_bottom[lenBy2 - 2U * idx - 1U]), (tempw1));
        xl_bottom[lenBy2 - 2U * idx - 1U] = _itoll(temp6, temp5);

        temp7 =
            _cmpyr1(_loll(xl_bottom[lenBy2 - 2U * idx - 2U]), (tempw4));
        temp8 =
            _cmpyr1(_hill(xl_bottom[lenBy2 - 2U * idx - 2U]), (tempw3));
        xl_bottom[lenBy2 - 2U * idx - 2U] = _itoll(temp8, temp7);
    }

} /*mmwavelib_windowing16x16_evenlen*/

