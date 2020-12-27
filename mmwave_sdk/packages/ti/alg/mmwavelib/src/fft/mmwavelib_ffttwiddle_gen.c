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
 *   @file     mmwavelib_ffttwiddle_gen.c
 *
 *   @brief   Contains functions related to generate FFT twiddle factors. The functions
 *     are optimized for speed to allow quick reconfiguration when switching sub-frames
 *     in advanced frame mode. For FFT size less than or equal to 1024, the implementation is 
 *     to store look-up table which is very high in memory consumption. For FFT size 
 *     2048, 4096, and 8192, the implementation is to use trigonometry formulas for interpolation 
 *     from the look-up table. 
 * 
 *
 */

#include <stdint.h>
#include <ti/alg/mmwavelib/src/fft/mmwavelib_fft.h>


/** @brief Lookup table for FFT twiddle factors generation(FFT size less than or equal to 1024) 
 * and DFT single bin DFT calculation. It contains 256 complex exponentials 
 * e(k) = cos(2*pi*k/1024)+j*sin(2*pi*k/1024), k=0,...,255,
 * Imaginary values are in even, and real values are in odd locations. Values are in Q31 format,
 * saturated to +/- 2147483647
 *
 * Following Matlab code was used to generate this table:
 *
 * %Generates lookup table for fast twiddle table generation for DSP lib FFT
 * %functions 16x16 and 32x32 for FFT sizes up to 1024. It saturates the
 * %amplitude to +/- 2147483647. The values are saved to file as imaginary
 * %(sine) to even, and real (cosine) to odd index positions.
 * M = 2147483647.5;
 * nMax = 1024;
 * table=round(M*exp(1j*2*pi/1024*[0:1*nMax/4-1]'));
 * table=max(min(real(table),2147483647),-2147483647) +
 *               1j* max(min(imag(table),2147483647),-2147483647);
 * fid = fopen('twiddle_table_all.dat','w');
 *
 * tableRI = [imag(table) real(table)]';
 * tableRI = tableRI(:);
 * tableRI(tableRI<0) = tableRI(tableRI<0) + 4294967296;
 *
 * fprintf(fid, [repmat(' 0x%08x,', 1, 8) '\n'], tableRI);
 *
**/
#pragma DATA_ALIGN(mmwavelib_twiddleTableCommon, 8)
const int32_t mmwavelib_twiddleTableCommon[2*256] = {
0x00000000, 0x7fffffff, 0x00c90f88, 0x7fff6216, 0x01921d20, 0x7ffd885a, 0x025b26d7, 0x7ffa72d1,
0x03242abf, 0x7ff62182, 0x03ed26e6, 0x7ff09477, 0x04b6195d, 0x7fe9cbbf, 0x057f0035, 0x7fe1c76b,
0x0647d97c, 0x7fd8878d, 0x0710a345, 0x7fce0c3e, 0x07d95b9e, 0x7fc25596, 0x08a2009a, 0x7fb563b2,
0x096a9049, 0x7fa736b4, 0x0a3308bd, 0x7f97cebc, 0x0afb6805, 0x7f872bf2, 0x0bc3ac35, 0x7f754e7f,
0x0c8bd35e, 0x7f62368f, 0x0d53db92, 0x7f4de450, 0x0e1bc2e4, 0x7f3857f5, 0x0ee38766, 0x7f2191b3,
0x0fab272b, 0x7f0991c3, 0x1072a048, 0x7ef0585f, 0x1139f0cf, 0x7ed5e5c6, 0x120116d5, 0x7eba3a39,
0x12c8106e, 0x7e9d55fc, 0x138edbb1, 0x7e7f3956, 0x145576b1, 0x7e5fe493, 0x151bdf86, 0x7e3f57fe,
0x15e21444, 0x7e1d93e9, 0x16a81305, 0x7dfa98a7, 0x176dd9de, 0x7dd6668e, 0x183366e9, 0x7db0fdf7,
0x18f8b83c, 0x7d8a5f3f, 0x19bdcbf3, 0x7d628ac5, 0x1a82a026, 0x7d3980ec, 0x1b4732ef, 0x7d0f4218,
0x1c0b826a, 0x7ce3ceb1, 0x1ccf8cb3, 0x7cb72724, 0x1d934fe5, 0x7c894bdd, 0x1e56ca1e, 0x7c5a3d4f,
0x1f19f97b, 0x7c29fbee, 0x1fdcdc1b, 0x7bf88830, 0x209f701c, 0x7bc5e28f, 0x2161b3a0, 0x7b920b89,
0x2223a4c5, 0x7b5d039d, 0x22e541af, 0x7b26cb4f, 0x23a6887e, 0x7aef6323, 0x24677757, 0x7ab6cba3,
0x25280c5e, 0x7a7d055b, 0x25e845b6, 0x7a4210d8, 0x26a82186, 0x7a05eead, 0x27679df4, 0x79c89f6d,
0x2826b928, 0x798a23b1, 0x28e5714b, 0x794a7c11, 0x29a3c485, 0x7909a92c, 0x2a61b101, 0x78c7aba1,
0x2b1f34eb, 0x78848413, 0x2bdc4e6f, 0x78403328, 0x2c98fbba, 0x77fab988, 0x2d553afb, 0x77b417df,
0x2e110a62, 0x776c4edb, 0x2ecc681e, 0x77235f2d, 0x2f875262, 0x76d94988, 0x3041c760, 0x768e0ea5,
0x30fbc54d, 0x7641af3c, 0x31b54a5d, 0x75f42c0a, 0x326e54c7, 0x75a585cf, 0x3326e2c2, 0x7555bd4b,
0x33def287, 0x7504d345, 0x34968250, 0x74b2c883, 0x354d9057, 0x745f9dd1, 0x36041ad9, 0x740b53fa,
0x36ba2014, 0x73b5ebd0, 0x376f9e46, 0x735f6626, 0x382493b0, 0x7307c3d0, 0x38d8fe93, 0x72af05a6,
0x398cdd32, 0x72552c84, 0x3a402dd2, 0x71fa3948, 0x3af2eeb7, 0x719e2cd2, 0x3ba51e29, 0x71410804,
0x3c56ba70, 0x70e2cbc6, 0x3d07c1d6, 0x708378fe, 0x3db832a6, 0x70231099, 0x3e680b2c, 0x6fc19385,
0x3f1749b8, 0x6f5f02b1, 0x3fc5ec98, 0x6efb5f12, 0x4073f21d, 0x6e96a99c, 0x4121589a, 0x6e30e349,
0x41ce1e64, 0x6dca0d14, 0x427a41d0, 0x6d6227fa, 0x4325c135, 0x6cf934fb, 0x43d09aec, 0x6c8f351c,
0x447acd50, 0x6c242960, 0x452456bd, 0x6bb812d1, 0x45cd358f, 0x6b4af278, 0x46756828, 0x6adcc964,
0x471cece6, 0x6a6d98a4, 0x47c3c22f, 0x69fd614a, 0x4869e665, 0x698c246c, 0x490f57ee, 0x6919e320,
0x49b41533, 0x68a69e81, 0x4a581c9d, 0x683257ab, 0x4afb6c98, 0x67bd0fbc, 0x4b9e038f, 0x6746c7d7,
0x4c3fdff3, 0x66cf811f, 0x4ce10034, 0x66573cbb, 0x4d8162c4, 0x65ddfbd3, 0x4e210617, 0x6563bf92,
0x4ebfe8a4, 0x64e88926, 0x4f5e08e3, 0x646c59bf, 0x4ffb654d, 0x63ef328f, 0x5097fc5e, 0x637114cc,
0x5133cc94, 0x62f201ac, 0x51ced46e, 0x6271fa69, 0x5269126e, 0x61f1003f, 0x53028517, 0x616f146b,
0x539b2aef, 0x60ec3830, 0x5433027d, 0x60686cce, 0x54ca0a4a, 0x5fe3b38d, 0x556040e2, 0x5f5e0db3,
0x55f5a4d2, 0x5ed77c89, 0x568a34a9, 0x5e50015d, 0x571deef9, 0x5dc79d7c, 0x57b0d256, 0x5d3e5236,
0x5842dd54, 0x5cb420df, 0x58d40e8c, 0x5c290acc, 0x59646497, 0x5b9d1153, 0x59f3de12, 0x5b1035cf,
0x5a82799a, 0x5a82799a, 0x5b1035cf, 0x59f3de12, 0x5b9d1153, 0x59646497, 0x5c290acc, 0x58d40e8c,
0x5cb420df, 0x5842dd54, 0x5d3e5236, 0x57b0d256, 0x5dc79d7c, 0x571deef9, 0x5e50015d, 0x568a34a9,
0x5ed77c89, 0x55f5a4d2, 0x5f5e0db3, 0x556040e2, 0x5fe3b38d, 0x54ca0a4a, 0x60686cce, 0x5433027d,
0x60ec3830, 0x539b2aef, 0x616f146b, 0x53028517, 0x61f1003f, 0x5269126e, 0x6271fa69, 0x51ced46e,
0x62f201ac, 0x5133cc94, 0x637114cc, 0x5097fc5e, 0x63ef328f, 0x4ffb654d, 0x646c59bf, 0x4f5e08e3,
0x64e88926, 0x4ebfe8a4, 0x6563bf92, 0x4e210617, 0x65ddfbd3, 0x4d8162c4, 0x66573cbb, 0x4ce10034,
0x66cf811f, 0x4c3fdff3, 0x6746c7d7, 0x4b9e038f, 0x67bd0fbc, 0x4afb6c98, 0x683257ab, 0x4a581c9d,
0x68a69e81, 0x49b41533, 0x6919e320, 0x490f57ee, 0x698c246c, 0x4869e665, 0x69fd614a, 0x47c3c22f,
0x6a6d98a4, 0x471cece6, 0x6adcc964, 0x46756828, 0x6b4af278, 0x45cd358f, 0x6bb812d1, 0x452456bd,
0x6c242960, 0x447acd50, 0x6c8f351c, 0x43d09aec, 0x6cf934fb, 0x4325c135, 0x6d6227fa, 0x427a41d0,
0x6dca0d14, 0x41ce1e64, 0x6e30e349, 0x4121589a, 0x6e96a99c, 0x4073f21d, 0x6efb5f12, 0x3fc5ec98,
0x6f5f02b1, 0x3f1749b8, 0x6fc19385, 0x3e680b2c, 0x70231099, 0x3db832a6, 0x708378fe, 0x3d07c1d6,
0x70e2cbc6, 0x3c56ba70, 0x71410804, 0x3ba51e29, 0x719e2cd2, 0x3af2eeb7, 0x71fa3948, 0x3a402dd2,
0x72552c84, 0x398cdd32, 0x72af05a6, 0x38d8fe93, 0x7307c3d0, 0x382493b0, 0x735f6626, 0x376f9e46,
0x73b5ebd0, 0x36ba2014, 0x740b53fa, 0x36041ad9, 0x745f9dd1, 0x354d9057, 0x74b2c883, 0x34968250,
0x7504d345, 0x33def287, 0x7555bd4b, 0x3326e2c2, 0x75a585cf, 0x326e54c7, 0x75f42c0a, 0x31b54a5d,
0x7641af3c, 0x30fbc54d, 0x768e0ea5, 0x3041c760, 0x76d94988, 0x2f875262, 0x77235f2d, 0x2ecc681e,
0x776c4edb, 0x2e110a62, 0x77b417df, 0x2d553afb, 0x77fab988, 0x2c98fbba, 0x78403328, 0x2bdc4e6f,
0x78848413, 0x2b1f34eb, 0x78c7aba1, 0x2a61b101, 0x7909a92c, 0x29a3c485, 0x794a7c11, 0x28e5714b,
0x798a23b1, 0x2826b928, 0x79c89f6d, 0x27679df4, 0x7a05eead, 0x26a82186, 0x7a4210d8, 0x25e845b6,
0x7a7d055b, 0x25280c5e, 0x7ab6cba3, 0x24677757, 0x7aef6323, 0x23a6887e, 0x7b26cb4f, 0x22e541af,
0x7b5d039d, 0x2223a4c5, 0x7b920b89, 0x2161b3a0, 0x7bc5e28f, 0x209f701c, 0x7bf88830, 0x1fdcdc1b,
0x7c29fbee, 0x1f19f97b, 0x7c5a3d4f, 0x1e56ca1e, 0x7c894bdd, 0x1d934fe5, 0x7cb72724, 0x1ccf8cb3,
0x7ce3ceb1, 0x1c0b826a, 0x7d0f4218, 0x1b4732ef, 0x7d3980ec, 0x1a82a026, 0x7d628ac5, 0x19bdcbf3,
0x7d8a5f3f, 0x18f8b83c, 0x7db0fdf7, 0x183366e9, 0x7dd6668e, 0x176dd9de, 0x7dfa98a7, 0x16a81305,
0x7e1d93e9, 0x15e21444, 0x7e3f57fe, 0x151bdf86, 0x7e5fe493, 0x145576b1, 0x7e7f3956, 0x138edbb1,
0x7e9d55fc, 0x12c8106e, 0x7eba3a39, 0x120116d5, 0x7ed5e5c6, 0x1139f0cf, 0x7ef0585f, 0x1072a048,
0x7f0991c3, 0x0fab272b, 0x7f2191b3, 0x0ee38766, 0x7f3857f5, 0x0e1bc2e4, 0x7f4de450, 0x0d53db92,
0x7f62368f, 0x0c8bd35e, 0x7f754e7f, 0x0bc3ac35, 0x7f872bf2, 0x0afb6805, 0x7f97cebc, 0x0a3308bd,
0x7fa736b4, 0x096a9049, 0x7fb563b2, 0x08a2009a, 0x7fc25596, 0x07d95b9e, 0x7fce0c3e, 0x0710a345,
0x7fd8878d, 0x0647d97c, 0x7fe1c76b, 0x057f0035, 0x7fe9cbbf, 0x04b6195d, 0x7ff09477, 0x03ed26e6,
0x7ff62182, 0x03242abf, 0x7ffa72d1, 0x025b26d7, 0x7ffd885a, 0x01921d20, 0x7fff6216, 0x00c90f88
};

/** @brief Lookup table for big size(2048, 4096, 8192) FFT twiddle factor generation. 
 * It contains the interpolation values for FFT size 2048, 4096, and 8192.  
 * For FFT size 2048, two complex interpolation values, exp(j* 2*pi*[0:1]/2048),
 * For FFT size 4096, four complex interpolation values exp(j* 2*pi*[0:3]/4096),
 * For FFT size 8192, eight complex interpolation values exp(j*2*pi*[0:7]/8192).
 * Imaginary values are in even, and real values are in odd locations. Values are in Q31 format,
 * saturated to +/- 2147483647.
 * 
 */
#pragma DATA_ALIGN(mmwavelib_twiddleTableLargeFFTSize, 8)
const int32_t mmwavelib_twiddleTableLargeFFTSize[2 * 14] = {
    /* j* 2*pi*[0:1]/2048 */
    0x00000000, 0x7FFFFFFF,
    0x006487E3, 0x7FFFD885,   
    /* j*2*pi*[0:1:3]/4096 */
    0x00000000, 0x7FFFFFFF,
    0x003243F5, 0x7FFFF621, 0x006487E3, 0x7FFFD885, 0x0096CBC1, 0x7FFFA72C,  
    /* j*2*pi*[0:7]/8192 */
    0x00000000, 0x7FFFFFFF,
    0x001921FB, 0x7FFFFD88, 0x003243F5, 0x7FFFF621, 0x004B65EE, 0x7FFFE9CB,  
    0x006487E3, 0x7FFFD885, 0x007DA9D4, 0x7FFFC250, 0x0096CBC1, 0x7FFFA72C, 0x00AFEDA8, 0x7FFF8718
};

/*!************************************************************************************************
 * \brief
 * Function Name       :     mmwavelib_gen_twiddle_fft16x16
 *
 * \par
 * <b>Description</b>  :  this function is equivalent of the dsplib's gen_twiddle_fft16x16() 
 *                        function with optimization. The maximum error with respect to the 
 *                        dsplib function is in the LSB (+/- 1). The math for twiddle factors 
 *                        is defined as follows
 *                          for (j = 1, k = 0; j < n >> 2; j = j << 2) 
 *                          {
 *                              for (i = 0; i < n >> 2; i += j << 1) 
 *                              {
 *                                  w[k + 0] =  exp(2.0 * PI * i / n);
 *                                  w[k + 1] =  exp(2.0 * PI * (i + j) / n);
 *                                  w[k + 2] = -exp(4.0 * PI * i / n);
 *                                  w[k + 3] = -exp(4.0 * PI * (i + j) / n);
 *                                  w[k + 4] =  exp(6.0 * PI * i / n);
 *                                  w[k + 5] =  exp(6.0 * PI * (i + j) / n);
 *                                  k += 6;
 *                               }
 *                           }
 *
 * @param[in]               n    : the FFT size
 * @param[out]              w    : Pointer to twiddle-factor array 
 *
 * @return                  the actual length of the twiddle factor array
 *
 * @pre                     FFT size must be power of 2.
 * @pre                     the maximum FFT size is 8192
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 *************************************************************************************************
 */
int32_t mmwavelib_gen_twiddle_fft16x16(int16_t *w, int32_t n)
{
    int32_t factorLength;
    
    if (n <= 1024)
    {
        factorLength = mmwavelib_gen_twiddle_smallfft_16x16(w, n);
    }
    else
    {
        factorLength = mmwavelib_gen_twiddle_largefft_16x16(w, n);
    }
    return factorLength;
}


/*!************************************************************************************************
* \brief
* Function Name       :     mmwavelib_gen_twiddle_fft16x16_imre_sa
*
* \par
* <b>Description</b>  :  this function is equivalent of the dsplib's gen_twiddle_fft16x16_imre_sa()
*                        function with optimization. The maximum error with respect to the
*                        dsplib function is in the LSB (+/- 1). The math for twiddle factors
*                        is defined as follows
*                          for (j = 1, k = 0; j < n >> 2; j = j << 2)
*                          {
*                              for (i = 0; i < n >> 2; i += j << 1)
*                              {
*                                  w[k + 0] =  exp(-2.0 * PI * i / n);
*                                  w[k + 1] =  exp(-2.0 * PI * (i + j) / n);
*                                  w[k + 2] =  exp(PI-(4.0 * PI * i / n));
*                                  w[k + 3] =  exp(PI-(4.0 * PI * (i + j) / n));
*                                  w[k + 4] =  exp(-6.0 * PI * i / n);
*                                  w[k + 5] =  exp(-6.0 * PI * (i + j) / n);
*                                  k += 6;
*                               }
*                           }
*
* @param[in]               n    : the FFT size
* @param[out]              w    : Pointer to twiddle-factor array
*
* @return                  the actual length of the twiddle factor array
*
* @pre                     FFT size must be power of 2.
* @pre                     the maximum FFT size is 4096
*
* @ingroup                 MMWAVELIB_FFT
*
*************************************************************************************************
*/
int32_t mmwavelib_gen_twiddle_fft16x16_imre_sa(int16_t *w, int32_t n)
{
    int32_t factorLength;

    if (n <= 1024)
    {
        factorLength = mmwavelib_gen_twiddle_smallfft_16x16_imre_sa(w, n);
    }
    else
    {
        factorLength = mmwavelib_gen_twiddle_largefft_16x16_imre_sa(w, n);
    }
    return factorLength;
}


/*!**********************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_gen_twiddle_smallfft_16x16
 *
 * \par
 * <b>Description</b>  :  This function is equivalent to the dsplib's gen_twiddle_fft16x16()
 *                        function with optimization. The optimization is to use the look up 
 *                        table. The sin and cos values of the angles from 0 to pi/2 in the 
 *                        step of 2*pi/1024 are saved in the lookup table in Q31 format. 
 *                        For FFT size, which is power of 2, and less than or equal to 1024, 
 *                        the twiddle factor angles are always multiple of 2*pi/1024. If the
 *                        twiddle factor angle is in the range of [0 pi/2], the function reads 
 *                        the sin/cos values directly from the table, then converts into Q16 
 *                        format. If the factor angle is in the range of [pi/2 pi], the 
 *                        function uses the formula sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a), 
 *                        to find the twiddle factor values. If the twiddle factor angle is 
 *                        in the range of [pi 3*pi/2], the function uses the formula 
 *                        sin(pi+a) = -sin(a), cos(pi+a)= -cos(a) to find the twiddle factor
 *                        values. No twiddle factor angle is in the range of [3*pi/2 2*pi]. 
 *                        The maximum error with respect to the dsplib function is in 
 *                        the LSB (+/- 1). It supports up to FFT size 1024.
 *
 * @param[in]               n     : the FFT size
 * @param[out]              w     : Pointer to twiddle-factor array 
 *
 * @return                  the actual length of the generated twiddle factor array
 *
 * @pre                     FFT size must be power of 2.
 * @pre                     The maximum FFT size is 1024
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 *                          Cycles 4.45 * n + 181 (cgt 8.1.3)  
 *                          
 **************************************************************************************************
 */
int32_t mmwavelib_gen_twiddle_smallfft_16x16(int16_t *w, int32_t n)
{
    int32_t i, j, k;
    int32_t log2n = 30 - _norm(n); /*Note n is always power of 2 */
    int32_t step = 1024 >> log2n;
    int32_t step6 = 3 * step;
    int32_t step4 = 2 * step;
    int32_t step2 = 1 * step;
    int32_t ind, indLsb, indMsb;
    int64_t * restrict table = (int64_t *) mmwavelib_twiddleTableCommon;
    uint32_t * restrict wd = (uint32_t *) w;
    int32_t xRe;
    int32_t xIm;

    
    k = 0;
    for (j = 1; j < (n >> 2); j = j << 2) 
    {
        for (i = 0; i < (n >> 2); i += (j << 1)) 
        {
            ind = step2 * (i + j);
            indLsb = ind & 0xFF;   /* mod (ind, 256) */
            /* mod ((ind/256), 4), 
             * 0->[0:pi/2], direct read from table
             * 1->[pi/2: pi], sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a)
             * 2->[pi:3*pi/2], sin(pi+a) = -sin(a), cos(pi+a)= -cos(a),
             *                     */
            indMsb = (ind >> 8) & 0x3;  
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 1] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 1] =  _pack2(-xIm, xRe); 
            }
            if (indMsb == 2)
            {
                wd[k + 1] =  _pack2(-xRe, -xIm);
            }

            ind = step2 * (i);
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 0] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 0] =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 0] =  _pack2(-xRe, -xIm);
            }

            ind = step4 * (i + j);
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 3] =  _pack2(-xRe, -xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 3] =  _pack2(xIm, -xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 3] =  _pack2(xRe, xIm);
            }

            ind = step4 * (i);
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 2] =  _pack2(-xRe, -xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 2] =  _pack2(xIm, -xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 2] =  _pack2(xRe, xIm);
            }

            ind = step6 * (i + j);
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 5] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 5] =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 5] =  _pack2(-xRe, -xIm);
            }

            ind = step6 * (i);
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 4] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 4] =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 4] =  _pack2(-xRe, -xIm);
            }
         k += 6;
        }
    }
    return 2 * k;
}

/*!**********************************************************************************************
* \brief
* Function Name       :    mmwavelib_gen_twiddle_smallfft_16x16_imre_sa
*
* \par
* <b>Description</b>  :  This function is equivalent to the dsplib's gen_twiddle_fft16x16_imre_sa()
*                        function with optimization. The optimization is to use the look up
*                        table. The sin and cos values of the angles from 0 to pi/2 in the
*                        step of 2*pi/1024 are saved in the lookup table in Q31 format.
*                        For FFT size, which is power of 2, and less than or equal to 1024,
*                        the twiddle factor angles are always multiple of 2*pi/1024. If the
*                        twiddle factor angle is in the range of [0 pi/2], the function reads
*                        the sin/cos values directly from the table, then converts into Q16
*                        format. If the factor angle is in the range of [pi/2 pi], the
*                        function uses the formula sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a),
*                        to find the twiddle factor values. If the twiddle factor angle is
*                        in the range of [pi 3*pi/2], the function uses the formula
*                        sin(pi+a) = -sin(a), cos(pi+a)= -cos(a) to find the twiddle factor
*                        values. No twiddle factor angle is in the range of [3*pi/2 2*pi].
*                        The maximum error with respect to the dsplib function is in
*                        the LSB (+/- 1). It supports up to FFT size 1024.
*
* @param[in]               n     : the FFT size
* @param[out]              w     : Pointer to twiddle-factor array
*
* @return                  the actual length of the generated twiddle factor array
*
* @pre                     FFT size must be power of 2.
* @pre                     The maximum FFT size is 1024
*
* @ingroup                 MMWAVELIB_FFT
*
*                          Cycles 6.2 * n + 345 (cgt 8.1.3)
*
**************************************************************************************************
*/
int32_t mmwavelib_gen_twiddle_smallfft_16x16_imre_sa(int16_t *w, int32_t n)
{
    int32_t i, j, k;
    int32_t log2n = 30 - _norm(n); /*Note n is always power of 2 */
    int32_t step = 1024 >> log2n;
    int32_t loopIdx;
    int32_t ind, indLsb, indMsb;
    int64_t * restrict table = (int64_t *)mmwavelib_twiddleTableCommon;
    uint32_t * restrict wd = (uint32_t *)w;
    int32_t xRe,xReResult;
    int32_t xIm,xImResult;

    for (loopIdx = 0; loopIdx < 3; loopIdx++)
    {
        k = 0;
        for (j = 1; j < (n >> 2); j = j << 2)
        {
            for (i = 0; i < (n >> 2); i += (j << 1))
            {
            
                ind = (loopIdx + 1) * step * (i + j);  /* 1, 2, 3 * step */
                indLsb = ind & 0xFF;   /* mod (ind, 256) */
                /* mod ((ind/256), 4),
                * 0->[0:pi/2], direct read from table
                * 1->[pi/2: pi], sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a)
                * 2->[pi:3*pi/2], sin(pi+a) = -sin(a), cos(pi+a)= -cos(a),
                *                     */
                indMsb = (ind >> 8) & 0x3;
                xRe = ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
                xIm = ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
                xReResult = xRe;
                xImResult = xIm;
                if (indMsb == 1)
                {
                    xReResult = -xIm;
                    xImResult = xRe;
                }
                if (indMsb == 2)
                {
                    xReResult = -xRe;
                    xImResult = -xIm;
                }

                if (loopIdx == 1)
                {
                    wd[k + loopIdx * 2 + 1] = _pack2(-xReResult, xImResult); /* -cos, sin */
                }
                else
                {
                    wd[k + loopIdx * 2 + 1] = _pack2(xReResult, -xImResult); /*cos -sin*/
                }
                

                ind = (loopIdx + 1) * step  * (i);
                indLsb = ind & 0xFF;
                indMsb = (ind >> 8) & 0x3;
                xRe = ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
                xIm = ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
                xReResult = xRe;
                xImResult = xIm;
                if (indMsb == 1)
                {
                    xReResult = -xIm;
                    xImResult = xRe;
                }
                if (indMsb == 2)
                {
                    xReResult = -xRe;
                    xImResult = -xIm;
                }

                if (loopIdx == 1)
                {
                    wd[k + 2 * loopIdx] = _pack2(-xReResult, xImResult); /* -cos, sin*/
                }
                else
                {
                    wd[k + 2 * loopIdx] = _pack2(xReResult, -xImResult); /* cos, -sin*/
                }
                
                k += 6;
            }
            
        }
    }
    return 2 * k;
}

/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_gen_twiddle_largefft_16x16
 *
 * \par
 * <b>Description</b>  : This function is equivalent of the dsplib's gen_twiddle_fft16x16() 
 *                       function with optimization. The optimization is similar to the function 
 *                       mmwavelib_gen_twiddle_smallfft_16x16. This function supports FFT 
 *                       size 2048, 4096. If the twiddle factor angle is the multiple of 
 *                       2*pi/1024, the twiddle factor values are obtained the same 
 *                       way as the function mmwavelib_gen_twiddle_smallfft_16x16. If the 
 *                       twiddle factor angle is not multiple of 2*pi/1024, the function 
 *                       uses the formula
 *                          cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
 *                           sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
 *                       to calculate the twiddle factor values, where a is the multiple 
 *                       of 2*pi/1024, and b is the remainder. The corresponding sin/cos
 *                       values for the remainder (b) are saved in the 
 *                       mmwavelib_twiddleTableLargeFFTSize table.The maximum error with respect
 *                       to the dsplib function is in the LSB (+/- 1). 
 *                        
 *
 * @param[in]               n    : the FFT size
 * @param[out]              w    : Pointer to twiddle-factor array 
 *
 * @return                  The actual length of the twiddle factor array
 *
 * @pre                     It supports FFT size 2048, 4096.
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 *                          Cycles: 8.15 * n + 678 (cgt 8.1.3)       
 *                          
 **************************************************************************************************
 */

int32_t mmwavelib_gen_twiddle_largefft_16x16(int16_t *w, int32_t n)
{
    int32_t i, j, k;
    int32_t ind, indLsb, indMsb;
    int64_t * restrict table = (int64_t *) mmwavelib_twiddleTableCommon;
    uint32_t * restrict wd = (uint32_t *) w;
    int32_t xRe, xIm;
    uint32_t nn;
    int64_t xReNew, xImNew;
    int32_t interIdx,factorIdx;
    int64_t * restrict extTable = (int64_t *) mmwavelib_twiddleTableLargeFFTSize;
    int32_t interReal, interImag;
    int32_t log2n;
    int32_t mm;
    /*left shift value for FFT size 2048,4096, 8192 relative to 1024 */
    int32_t shiftTbl[3] = {1, 2, 3};  
    
    if (n == 4096)
    {
        extTable += 2;
    }
    
    nn = n>>10; /*2, 4, 8 2048, 4096, 8192*/
    /*log2n = 30 - _norm(nn); */
    log2n = shiftTbl[29 - _norm(nn)];
    
   /*for (j = 1; j < (n >> 2); j = j << 2) */
    j = 1;
    for (mm = 0; mm < 3; mm++)
    {
        k = 0;
        for (i = 0; i < (n >> 2); i += 2) 
        {
            factorIdx = i * (mm + 1);  /* i, 2 * i, 3 * i */
            ind = factorIdx >> log2n;        
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);

            /* if (indMsb == 0) */
            xRe = _hill(table[indLsb]);
            xIm = _loll(table[indLsb]);
            
            if (indMsb == 1)
            {
                /* sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a) */
                xRe = _ssub(0, _loll(table[indLsb]));
                xIm = _hill(table[indLsb]);
            }
            if (indMsb==2)
            {
                /* sin(pi+a) = -sin(a), cos(pi+a)= -cos(a) */
                xRe = _ssub(0, _hill(table[indLsb]));
                xIm = _ssub(0, _loll(table[indLsb]));
            }

            /*cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
              sin(a+b) = sin(a)cos(b) + cos(a)sin(b)*/
            xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
            xRe = (int32_t)(xReNew >> 31);
            xIm = (int32_t)(xImNew >> 31);
            xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
            xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
            if (mm == 1)
            {
                wd[k + 2 * mm] =  _pack2(-xRe, -xIm);
            }
            else
            {
                wd[k + 2 * mm] =  _pack2(xRe, xIm);  
            }
            /* (i + j)*/
            factorIdx = (i + j) * (mm + 1); /*(i+j) 2*(i+j) 3 * (i+j)*/ 
            ind = factorIdx >> log2n;        
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            /*if (indMsb == 0) */
            xRe = _hill(table[indLsb]);
            xIm = _loll(table[indLsb]);

            if (indMsb == 1)
            {
                xRe = _ssub(0, _loll(table[indLsb]));
                xIm = _hill(table[indLsb]);
            }
            if (indMsb==2)
            {
                xRe = _ssub(0, _hill(table[indLsb]));
                xIm = _ssub(0, _loll(table[indLsb]));
            }
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);
            xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
            xRe = (int32_t)(xReNew >> 31);
            xIm = (int32_t)(xImNew >> 31);
            
            xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
            xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
            if (mm == 1)
            {
                wd[k +  2 * mm + 1] =  _pack2(-xRe, -xIm);
            }
            else
            {
                wd[k +  2 * mm + 1] =  _pack2(xRe, xIm); 
            }
              
            k += 6;
        } /* i loop */
    }/* mm loop */
    
    /* j = 4, mod (ind, 2,4) = 0*/
    for (j = 4; j < (n >> 2); j = j << 2) 
    {
        for (i = 0; i < (n >> 2); i += (j << 1)) 
        {
            ind = i >> log2n;
            indLsb = ind & 0xFF; /* mod (ind, 256) */
            indMsb = (ind >> 8) & 0x3;  
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            /*if (indMsb == 0) */
            {
                wd[k] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k] =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k] =  _pack2(-xRe, -xIm);
            }
            
            ind = (i + j) >> log2n ;
            indLsb = ind & 0xFF;  
            indMsb = (ind >> 8) & 0x3;  
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            /*if (indMsb == 0) */
            {
                wd[k + 1] = _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 1] =  _pack2(-xIm, xRe); 
            }
            if (indMsb == 2)
            {
                wd[k + 1] =  _pack2(-xRe, -xIm); 
            }
            
            ind = (i << 1)>> log2n  ;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            /*if (indMsb == 0)*/
            {
                wd[k + 2] =  _pack2(-xRe, -xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 2] =  _pack2(xIm, -xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 2] =  _pack2(xRe, xIm);
            }
            
            
            
            ind =  (2 * (i + j)) >> log2n;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            /*if (indMsb == 0) */
            {
                wd[k + 3] =  _pack2(-xRe, -xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 3] =  _pack2(xIm, -xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 3]  =  _pack2(xRe, xIm);
            }
            
            ind = (3 * i) >> log2n;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            /*if (indMsb == 0)*/
            {
                wd[k + 4] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 4]  =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 4] =  _pack2(-xRe, -xIm);
            }
            
            ind = (3 * (i + j))>> log2n ;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            /*if (indMsb == 0)*/
            {
                wd[k + 5] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 5] =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 5] =  _pack2(-xRe, -xIm);
            }
            
            
            k += 6;
        } /* end of i loop */
    } /* end of j loop */
    return 2 * k;
}


/*!************************************************************************************************
* \brief
* Function Name       :    mmwavelib_gen_twiddle_largefft_16x16_imre_sa
*
* \par
* <b>Description</b>  : This function is equivalent of the dsplib's gen_twiddle_fft16x16_imre_sa()
*                       function with optimization. The optimization is similar to the function
*                       mmwavelib_gen_twiddle_smallfft_16x16. This function supports FFT
*                       size 2048, 4096. If the twiddle factor angle is the multiple of
*                       2*pi/1024, the twiddle factor values are obtained the same
*                       way as the function mmwavelib_gen_twiddle_smallfft_16x16. If the
*                       twiddle factor angle is not multiple of 2*pi/1024, the function
*                       uses the formula
*                          cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
*                          sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
*                       to calculate the twiddle factor values, where a is the multiple
*                       of 2*pi/1024, and b is the remainder. The corresponding sin/cos
*                       values for the remainder (b) are saved in the
*                       mmwavelib_twiddleTableLargeFFTSize table.The maximum error with respect
*                       to the dsplib function is in the LSB (+/- 1).
*
*
* @param[in]               n    : the FFT size
* @param[out]              w    : Pointer to twiddle-factor array
*
* @return                  The actual length of the twiddle factor array
*
* @pre                     It supports FFT size 2048, 4096.
*
* @ingroup                 MMWAVELIB_FFT
*
*                          Cycles: 8.23 * n + 780 (cgt 8.1.3)
*
**************************************************************************************************
*/

int32_t mmwavelib_gen_twiddle_largefft_16x16_imre_sa(int16_t *w, int32_t n)
{
    int32_t i, j, k, tempk;
    int32_t ind, indLsb, indMsb;
    int64_t * restrict table = (int64_t *)mmwavelib_twiddleTableCommon;
    uint32_t * restrict wd = (uint32_t *)w;
    int32_t xRe, xIm;
    uint32_t nn;
    int64_t xReNew, xImNew;
    int32_t interIdx, factorIdx;
    int64_t * restrict extTable = (int64_t *)mmwavelib_twiddleTableLargeFFTSize;
    int32_t interReal, interImag;
    int32_t log2n;
    int32_t mm;
    int32_t xReResult, xImResult;
    /*left shift value for FFT size 2048,4096, 8192 relative to 1024 */
    int32_t shiftTbl[3] = { 1, 2, 3 };

    if (n == 4096)
    {
        extTable += 2;
    }

    nn = n >> 10; /*2, 4, 2048, 4096*/
                  /*log2n = 30 - _norm(nn); */
    log2n = shiftTbl[29 - _norm(nn)];

    /*for (j = 1; j < (n >> 2); j = j << 2) */
    j = 1;
    for (mm = 0; mm < 3; mm++)
    {
        k = 0;
        for (i = 0; i < (n >> 2); i += 2)
        {
            factorIdx = i * (mm + 1);  /* i, 2 * i, 3 * i */
            ind = factorIdx >> log2n;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);

            /* if  (indMsb == 0) */
            xRe = _hill(table[indLsb]);
            xIm = _loll(table[indLsb]);
            
            if (indMsb == 1)
            {
                /* sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a) */
                xRe = _ssub(0, _loll(table[indLsb]));
                xIm = _hill(table[indLsb]);
            }
            if (indMsb == 2)
            {
                /* sin(pi+a) = -sin(a), cos(pi+a)= -cos(a) */
                xRe = _ssub(0, _hill(table[indLsb]));
                xIm = _ssub(0, _loll(table[indLsb]));
            }

            /*cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
            sin(a+b) = sin(a)cos(b) + cos(a)sin(b)*/
            xReNew = _mpyidll(xRe, interReal) - _mpyidll(xIm, interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe, interImag);
            xRe = (int32_t)(xReNew >> 31);
            xIm = (int32_t)(xImNew >> 31);
            xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
            xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
            if (mm == 1)
            {
                wd[k + 2 * mm] = _pack2(-xRe, xIm);     /*-cos sin*/
            }
            else
            {
                wd[k + 2 * mm] = _pack2(xRe, -xIm);     /* cos -sin*/
            }
            
            
            /* (i + j)*/
            factorIdx = (i + j) * (mm + 1); /*(i+j) 2*(i+j) 3 * (i+j)*/
            ind = factorIdx >> log2n;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            /*if (indMsb == 0)*/
            xRe = _hill(table[indLsb]);
            xIm = _loll(table[indLsb]);

            if (indMsb == 1)
            {
                xRe = _ssub(0, _loll(table[indLsb]));
                xIm = _hill(table[indLsb]);
            }
            if (indMsb == 2)
            {
                xRe = _ssub(0, _hill(table[indLsb]));
                xIm = _ssub(0, _loll(table[indLsb]));
            }
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);
            xReNew = _mpyidll(xRe, interReal) - _mpyidll(xIm, interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe, interImag);
            xRe = (int32_t)(xReNew >> 31);
            xIm = (int32_t)(xImNew >> 31);

            xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
            xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
            if (mm == 1)
            {
                wd[k + 2 * mm + 1] = _pack2(-xRe, xIm);   /*-cos sin*/
            }
            else
            {
                wd[k + 2 * mm + 1] = _pack2(xRe, -xIm);   /* cos -sin*/
            }
           
            

            k += 6;
        } /* i loop */
    }/* mm loop */
    tempk = k;
     /* j = 4, mod (ind, 2,4) = 0*/
    for (mm = 0; mm < 3; mm++)
    {
        k = tempk;
        for (j = 4; j < (n >> 2); j = j << 2)
        {
            for (i = 0; i < (n >> 2); i += (j << 1))
            {
            
                ind = (mm + 1) * i;
                ind = ind >> log2n;
                indLsb = ind & 0xFF; /* mod (ind, 256) */
                indMsb = (ind >> 8) & 0x3;
                xRe = ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
                xIm = ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
                xReResult = xRe;
                xImResult = xIm;
                if (indMsb == 1)
                {
                    xReResult = -xIm;
                    xImResult = xRe;
                }
                if (indMsb == 2)
                {
                    xReResult = -xRe;
                    xImResult = -xIm;
                }
                if (mm == 1)
                {
                    wd[k + 2 * mm] = _pack2(-xReResult, xImResult); /* -cos, sin */
                }
                else
                {
                    wd[k + 2 * mm] = _pack2(xReResult, -xImResult); /* cos, -sin */
                }
                
                ind = (mm + 1) * (i + j);
                ind = ind >> log2n;
                indLsb = ind & 0xFF;
                indMsb = (ind >> 8) & 0x3;
                xRe = ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
                xIm = ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
                xReResult = xRe;
                xImResult = xIm;
                if (indMsb == 1)
                {
                    xReResult = -xIm;
                    xImResult = xRe;
                }
                if (indMsb == 2)
                {
                    xReResult = -xRe;
                    xImResult = -xIm;
                }

                if (mm == 1)
                {
                    wd[k + 2 * mm + 1] = _pack2(-xReResult, xImResult);
                }
                else
                {
                    wd[k + 2 * mm + 1] = _pack2(xReResult, -xImResult);
                }
                
                k += 6;
            }
   
        } /* end of i loop */
    } /* end of j loop */
    return 2 * k;
}


/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_gen_twiddle_largefftUp8k_16x16
 *
 * \par
 * <b>Description</b>  : This function is equivalent of the dsplib's gen_twiddle_fft16x16() 
 *                       function with optimization. The optimization is similar to the function 
 *                       mmwavelib_gen_twiddle_smallfft_16x16. This function supports FFT 
 *                       size 2048, 4096, and 8192. If the twiddle factor angle is the multiple of 
 *                       2*pi/1024, the twiddle factor values are obtained the same 
 *                       way as the function mmwavelib_gen_twiddle_smallfft_16x16. If the 
 *                       twiddle factor angle is not multiple of 2*pi/1024, the function 
 *                       uses the formula
 *                          cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
                            sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
 *                       to calculate the twiddle factor values, where a is the multiple 
 *                       of 2*pi/1024, and b is the remainder. The corresponding sin/cos
 *                       values for the remainder (b) are saved in the 
 *                       mmwavelib_twiddleTableLargeFFTSize table.The maximum error with respect
 *                       to the dsplib function is in the LSB (+/- 1). 
 *                        
 *
 * @param[in]               n    : the FFT size
 * @param[out]              w    : Pointer to twiddle-factor array 
 *
 * @return                  The actual length of the twiddle factor array
 *
 * @pre                     It supports FFT size 2048, 4096, 8192.
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 *                          Cycles: 21 * n + 432 (cgt 8.1.3)       
 *                          
 **************************************************************************************************
 */

int32_t mmwavelib_gen_twiddle_largefftUp8k_16x16(int16_t *w, int32_t n)
{
    int32_t i, j, k;
    int32_t ind, indLsb, indMsb;
    int64_t * restrict table = (int64_t *) mmwavelib_twiddleTableCommon;
    uint32_t * restrict wd = (uint32_t *) w;
    int32_t xRe, xIm;
    uint32_t nn;
    int64_t xReNew, xImNew;
    int32_t interIdx,factorIdx;
    int64_t * restrict extTable = (int64_t *) mmwavelib_twiddleTableLargeFFTSize;
    int32_t interReal, interImag;
    int32_t log2n;
    int32_t mm;
    /*left shift value for FFT size 2048,4096, 8192 relative to 1024 */
    int32_t shiftTbl[3] = {1, 2, 3};  
    
    if (n >= 4096)
    {
        extTable += 2;
    }
    if (n >= 8192)
    {
        extTable += 4;
    }
    nn = n>>10; /*2, 4, 8 2048, 4096, 8192*/
    /*log2n = 30 - _norm(nn); */
    log2n = shiftTbl[29 - _norm(nn)];
    
   /*for (j = 1; j < (n >> 2); j = j << 2) */
    j = 1;
    for (mm = 0; mm < 3; mm++)
    {
        k = 0;
        for (i = 0; i < (n >> 2); i += 2) 
        {
            factorIdx = i * (mm + 1);  /* i, 2 * i, 3 * i */
            ind = factorIdx >> log2n;        
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);

            /*if (indMsb == 0) */
            xRe = _hill(table[indLsb]);
            xIm = _loll(table[indLsb]);
            
            if (indMsb == 1)
            {
                /* sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a) */
                xRe = _ssub(0, _loll(table[indLsb]));
                xIm = _hill(table[indLsb]);
            }
            if (indMsb==2)
            {
                /* sin(pi+a) = -sin(a), cos(pi+a)= -cos(a) */
                xRe = _ssub(0, _hill(table[indLsb]));
                xIm = _ssub(0, _loll(table[indLsb]));
            }

            /*cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
              sin(a+b) = sin(a)cos(b) + cos(a)sin(b)*/
            xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
            xRe = (int32_t)(xReNew >> 31);
            xIm = (int32_t)(xImNew >> 31);
            xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
            xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
            if (mm == 1)
            {
                wd[k + 2 * mm] =  _pack2(-xRe, -xIm);
            }
            else
            {
                wd[k + 2 * mm] =  _pack2(xRe, xIm);  
            }
            /* (i + j)*/
            factorIdx = (i + j) * (mm + 1); /*(i+j) 2*(i+j) 3 * (i+j)*/ 
            ind = factorIdx >> log2n;        
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            if (indMsb == 0)
            {
                xRe = _hill(table[indLsb]);
                xIm = _loll(table[indLsb]);
            } 
            if (indMsb == 1)
            {
                xRe = _ssub(0, _loll(table[indLsb]));
                xIm = _hill(table[indLsb]);
            }
            if (indMsb==2)
            {
                xRe = _ssub(0, _hill(table[indLsb]));
                xIm = _ssub(0, _loll(table[indLsb]));
            }
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);
            xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
            xRe = (int32_t)(xReNew >> 31);
            xIm = (int32_t)(xImNew >> 31);
            
            xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
            xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
            if (mm == 1)
            {
                wd[k +  2 * mm + 1] =  _pack2(-xRe, -xIm);
            }
            else
            {
                wd[k +  2 * mm + 1] =  _pack2(xRe, xIm); 
            }
              
            k += 6;
        } /* i loop */
    }/* mm loop */
    
    /* j = 4*/
    j = 4;
    for (i = 0; i < (n >> 2); i += 8) 
    {
        factorIdx = i ;  /* mod 2,4,8 = 0 */
        ind = factorIdx >> log2n;       
        indLsb = ind & 0xFF;
        indMsb = (ind >> 8) & 0x3;
        xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
        xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
        if (indMsb == 0)
        {
            wd[k] =  _pack2(xRe, xIm);
        }
        if (indMsb == 1)
        {
            wd[k] =  _pack2(-xIm, xRe);
        }
        if (indMsb == 2)
        {
            wd[k] =  _pack2(-xRe, -xIm);
        }
        factorIdx = (i + j); /* mod 2,4,8 = 0, 0, 4 */
        ind = factorIdx >> log2n;     
        indLsb = ind & 0xFF;
        indMsb = (ind >> 8) & 0x3;
        interIdx = factorIdx & (nn-1);
        if (indMsb == 0)
        {
            xRe = _hill(table[indLsb]);
            xIm = _loll(table[indLsb]);
        } 
        if (indMsb == 1)
        {
            xRe = _ssub(0, _loll(table[indLsb]));
            xIm = _hill(table[indLsb]);
        }
        if (indMsb==2)
        {
            xRe = _ssub(0, _hill(table[indLsb]));
            xIm = _ssub(0, _loll(table[indLsb]));
        }
        
        interReal = _hill(extTable[interIdx]);
        interImag = _loll(extTable[interIdx]);
        xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
        xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
        xRe = (int32_t)(xReNew >> 31);
        xIm = (int32_t)(xImNew >> 31);
        
        xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
        xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
        wd[k + 1] =  _pack2(xRe, xIm);  
                
        factorIdx = 2 * i ;  /* mod 2,4,8 = 0 */
        ind = factorIdx >> log2n;       
        indLsb = ind & 0xFF;
        indMsb = (ind >> 8) & 0x3;
        xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
        xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
        if (indMsb == 0)
        {
            wd[k + 2] =  _pack2(-xRe, -xIm);
        }
        if (indMsb == 1)
        {
            wd[k + 2] =  _pack2(xIm, -xRe); 
        }
        if (indMsb == 2)
        {
            wd[k + 2] =  _pack2(xRe, xIm); 
        }
        
        factorIdx = 2 * (i + j) ;  /* mod 2,4,8 = 0 */
        ind = factorIdx >> log2n;       
        indLsb = ind & 0xFF;
        indMsb = (ind >> 8) & 0x3;
        xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
        xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
        if (indMsb == 0)
        {
            wd[k + 3] =  _pack2(-xRe, -xIm);
        }
        if (indMsb == 1)
        {
            wd[k + 3] =  _pack2(xIm, -xRe);
        }
        if (indMsb == 2)
        {
            wd[k + 3] =  _pack2(xRe, xIm);
        }
        
        factorIdx = 3 * i  ;  /* mod 2,4,8 = 0 */
        ind = factorIdx >> log2n;       
        indLsb = ind & 0xFF;
        indMsb = (ind >> 8) & 0x3;
        xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
        xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
        if (indMsb == 0)
        {
            wd[k + 4] =  _pack2(xRe, xIm);
        }
        if (indMsb == 1)
        {
            wd[k + 4] =  _pack2(-xIm, xRe); 
        }
        if (indMsb == 2)
        {
            wd[k + 4] =  _pack2(-xRe, -xIm);
        }
                 
        factorIdx = 3 * (i + j); /* mod 2,4,8 != 0 */
        ind = factorIdx >> log2n;        
        indLsb = ind & 0xFF;
        indMsb = (ind >> 8) & 0x3;
        interIdx = factorIdx & (nn - 1);
        if (indMsb == 0)
        {
            xRe = _hill(table[indLsb]);
            xIm = _loll(table[indLsb]);
        } 
        if (indMsb == 1)
        {
            xRe = _ssub(0, _loll(table[indLsb]));
            xIm = _hill(table[indLsb]);
        }
        if (indMsb==2)
        {
            xRe = _ssub(0, _hill(table[indLsb]));
            xIm = _ssub(0, _loll(table[indLsb]));
        }
        interReal = _hill(extTable[interIdx]);
        interImag = _loll(extTable[interIdx]);
        xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
        xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
        xRe = (int32_t)(xReNew >> 31);
        xIm = (int32_t)(xImNew >> 31);
        
        xRe = ((int32_t)_sadd(xRe, 0x00008000)) >> 16;
        xIm = ((int32_t)_sadd(xIm, 0x00008000)) >> 16;
        wd[k + 5] =  _pack2(xRe, xIm); 
        k += 6;
    } 
    /* j = 16, mod (ind, 2,4,8) = 0*/
    for (j = 16; j < (n >> 2); j = j << 2) 
    {
        for (i = 0; i < (n >> 2); i += (j << 1)) 
        {
            ind = i >> log2n;
            indLsb = ind & 0xFF; /* mod (ind, 256) */
            indMsb = (ind >> 8) & 0x3;  
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k] =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k] =  _pack2(-xRe, -xIm);
            }
            
            ind = (i + j) >> log2n ;
            indLsb = ind & 0xFF;  
            indMsb = (ind >> 8) & 0x3;  
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 1] = _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 1] =  _pack2(-xIm, xRe); 
            }
            if (indMsb == 2)
            {
                wd[k + 1] =  _pack2(-xRe, -xIm); 
            }
            
            ind = (i << 1)>> log2n  ;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 2] =  _pack2(-xRe, -xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 2] =  _pack2(xIm, -xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 2] =  _pack2(xRe, xIm);
            }
            
            
            
            ind =  (2 * (i + j)) >> log2n;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 3] =  _pack2(-xRe, -xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 3] =  _pack2(xIm, -xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 3]  =  _pack2(xRe, xIm);
            }
            
            ind = (3 * i) >> log2n;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 4] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 4]  =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 4] =  _pack2(-xRe, -xIm);
            }
            
            ind = (3 * (i + j))>> log2n ;
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            xRe =  ((int32_t)_sadd(_hill(table[indLsb]), 0x00008000)) >> 16;
            xIm =  ((int32_t)_sadd(_loll(table[indLsb]), 0x00008000)) >> 16;
            if (indMsb == 0)
            {
                wd[k + 5] =  _pack2(xRe, xIm);
            }
            if (indMsb == 1)
            {
                wd[k + 5] =  _pack2(-xIm, xRe);
            }
            if (indMsb == 2)
            {
                wd[k + 5] =  _pack2(-xRe, -xIm);
            }
            
            
            k += 6;
        } /* end of i loop */
    } /* end of j loop */
    return 2 * k;
}

/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_gen_twiddle_fft32x32
 *
 * \par
 * <b>Description</b>  :  This function is equivalent to the dsplib's gen_twiddle_fft32x32()
 *                        function with optimization. The maximum error with respect to th dsplib
 *                        function is in the LSB (+/- 2). It supports FFT size upto 8192. The  
 *                        math for twiddle factors is defined as follows
 *                          for (j = 1; j < n >> 2; j = j << 2) 
 *                          {
 *                             for (i = 0; i < n >> 2; i += j) 
 *                             {
 *                                  w[k + 0] =  exp(2.0 * PI * i / n);
 *                                  w[k + 1] =  exp(4.0 * PI * i / n);
 *                                  w[k + 2] =  exp(6.0 * PI * i / n);
 *                                  k += 3;
 *                              }
 *                          }         
 * 
 * @param[in]               n    : the FFT size
 * @param[out]              w    : Pointer to twiddle-factor array 
 *
 * @return                  the actual length of the twiddle factor array
 *
 * @pre                     FFT size must be power of 2.
 * @pre                     the maximum FFT size is 8192
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 **************************************************************************************************
 */
int32_t mmwavelib_gen_twiddle_fft32x32(int32_t *w, int32_t n)
{
    int32_t factorLength;
    if (n <= 1024)
    {
        factorLength = mmwavelib_gen_twiddle_smallfft_32x32(w,n);
    }
    else
    {
        factorLength = mmwavelib_gen_twiddle_largefft_32x32(w,n);
    }
    return factorLength;
}
  
/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_gen_twiddle_smallfft_32x32
 *
 * \par
 * <b>Description</b>  :  This function is equivalent to the dsplib's gen_twiddle_fft32x32() 
 *                        function with optimization. The optimization is to use look up table 
 *                        for twiddle factor generation. The sin and cos values of the angles 
 *                        from 0 to pi/2 in the step of 2*pi/1024 are saved in the lookup table
 *                        in Q31 format. For FFT size, which is power of 2, less than and equal
 *                        to 1024, the twiddle factor angles are always multiple of 2*pi/1024.
 *                        If the twiddle factor angle is in the range of [0 pi/2], the function
 *                        reads the factor values directly from the table. If the factor angle 
 *                        is in the range of [pi/2 pi], the function uses the formula 
 *                        sin(pi/2+a)=cos(a), cos(pi/2+a)=-sin(a), to find the twiddle factor 
 *                        values. If the twiddle factor angle is in the range of [pi 3*pi/2], 
 *                        the function uses the formula sin(pi+a) = -sin(a), cos(pi+a)= -cos(a)
 *                        to find the twiddle factor values. No twiddle factor angle is 
 *                        in the range of [3*pi/2 2*pi]. The maximum error with respect 
 *                        to the dsplib function is in the LSB (+/- 1). It supports FFT size
 *                        upto 1024.
 *
 * @param[in]               n    : the FFT size
 * @param[out]              w    : Pointer to twiddle-factor array 
 *
 * @return                  the actual length of the twiddle factor array
 *
 * @pre                     FFT size must be power of 2.
 * @pre                     The maximum FFT size is 1024
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 *                          Cycles 4.60 * n + 204 (cgt 8.1.3)        
 
 **************************************************************************************************
 */
int32_t mmwavelib_gen_twiddle_smallfft_32x32(int32_t *w, int32_t n)
{
    int32_t i, j, k;
     int32_t log2n = 30 - _norm(n); 
     int32_t step = 1024 >> log2n;
     int32_t step6 = 3 * step;
     int32_t step4 = 2 * step;
     int32_t step2 = 1 * step;
     int32_t ind, indLsb, indMsb;
     int64_t * restrict table = (int64_t *) mmwavelib_twiddleTableCommon;
     int64_t * restrict wd = (int64_t *) w;
     int32_t xRe;
     int32_t xIm;

     for (j = 1, k = 0; j < n >> 2; j = j << 2) 
     {
         for (i =0; i < n >> 2; i += j) 
         {
             ind = step2 * (i);
             indLsb = ind & 0xFF;
             indMsb = (ind >> 8) & 0x3;
             xRe =  _hill(table[indLsb]);
             xIm =  _loll(table[indLsb]);
             if (indMsb == 0)
             {
                 wd[k + 0] =  _itoll(xRe, xIm);
             }
             if (indMsb == 1)
             {
                 wd[k + 0] =  _itoll(-xIm, xRe);
             }
             if (indMsb == 2)
             {
                 wd[k + 0] =  _itoll(-xRe, -xIm);
             }

             ind = step4 * (i);
             indLsb = ind & 0xFF;
             indMsb = (ind >> 8) & 0x3;
             xRe =  _hill(table[indLsb]);
             xIm =  _loll(table[indLsb]);
             if (indMsb == 0)
             {
                 wd[k + 1] =  _itoll(xRe, xIm);
             }
             if (indMsb == 1)
             {
                 wd[k + 1] =  _itoll(-xIm, xRe);
             }
             if (indMsb == 2)
             {
                 wd[k + 1] =  _itoll(-xRe, -xIm);
             }

             ind = step6 * (i);
             indLsb = ind & 0xFF;
             indMsb = (ind >> 8) & 0x3;
             xRe =  _hill(table[indLsb]);
             xIm =  _loll(table[indLsb]);
             if (indMsb == 0)
             {
                 wd[k + 2] =  _itoll(xRe, xIm);
             }
             if (indMsb == 1)
             {
                 wd[k + 2] =  _itoll(-xIm, xRe);
             }
             if (indMsb == 2)
             {
                 wd[k + 2] =  _itoll(-xRe, -xIm);
             }

             k += 3;
         }
     }
     return 2*k;
}


/*!************************************************************************************************
 * \brief
 * Function Name       :    mmwavelib_gen_twiddle_largefft_32x32
 *
 * \par
 * <b>Description</b>  :  This function is equivalent to the dsplib's gen_twiddle_fft32x32() 
 *                        function with optimization. The optimization is similar to the function 
 *                       mmwavelib_gen_twiddle_smallfft_32x32. If the twiddle factor angle is the
 *                       multiple of 2*pi/1024, the twiddle factor values are obtained the same 
 *                       way as the function mmwavelib_gen_twiddle_smallfft_32x32. If the 
 *                       twiddle factor angle is not multiple of 2*pi/1024, the function 
 *                       uses the formula
 *                          cos(a+b) = cos(a)cos(b) - sin(a)sin(b)
                            sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
 *                        to calculate the twiddle factor values, where a is the multiple 
 *                        of 2*pi/1024, and b is the remainder, the corresponding sin/cos
                          values for the remainder are saved in the 
 *                        mmwavelib_twiddleTableLargeFFTSize table. It supports FFT size 
 *                        2048, 4096, 8192. The maximum error with respect to the dsplib function
 *                        is in the LSB (+/- 1) for FFT size 2048, and (+/- 2) for FFT size 
 *                        2048 and 8192.
 * 
 * @param[in]               n    : the FFT size
 * @param[out]              w    : Pointer to twiddle-factor array 
 *
 * @return                  the actual length of the twiddle factor array
 *
 * @pre                     It supports FFT size 2048, 4096 and 8192.
 * 
 * @ingroup                 MMWAVELIB_FFT
 *
 *                          Cycles: 13.67 * n + 188 (cgt 8.1.3)  
 *                         
 **************************************************************************************************
 */
int32_t mmwavelib_gen_twiddle_largefft_32x32(int32_t *w, int32_t n)
{
    int32_t i, j, k;
    int32_t ind, indLsb, indMsb;
    int64_t * restrict table = (int64_t *) mmwavelib_twiddleTableCommon;
    int64_t * restrict wd = (int64_t *) w;
    int32_t xRe, xIm;
    uint32_t nn;
    int64_t xReNew, xImNew;
    int32_t interIdx;
    int32_t factorIdx;
    int64_t * restrict extTable = (int64_t *) mmwavelib_twiddleTableLargeFFTSize;
    int32_t interReal, interImag;
    int32_t log2n;  
            
    if (n >= 4096)
    {
        extTable += 2;
    }
    if (n >= 8192)
    {
        extTable += 4;
    }
            
    
    nn = n>>10; /* 2, 4, 8 */
    log2n = 30 - _norm(nn);  

    k = 0;
 
    for (j = 1; j < n >> 2; j = j << 2) 
    {
        for (i =0; i < n >> 2; i += j) 
        {
            factorIdx = i ;
            ind = factorIdx >> log2n;        
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            
            /*if (indMsb==0) */
            xRe =  _hill(table[indLsb]);
            xIm =  _loll(table[indLsb]);
            
            if (indMsb == 1)
            {
                xRe =  _ssub(0, _loll(table[indLsb]));
                xIm =  _hill(table[indLsb]);
            }
            if (indMsb == 2)
            {
                xRe =  _ssub(0, _hill(table[indLsb]));
                xIm =  _ssub(0, _loll(table[indLsb]));
            }
            
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);   
            xReNew = _mpyidll(xRe, interReal) - _mpyidll(xIm ,interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
            xRe = (int32_t)((xReNew + 0x40000000) >> 31);
            xIm = (int32_t)((xImNew + 0x40000000) >> 31);
            
            wd[k] = _itoll(xRe,xIm);
            
            factorIdx = i * 2;
            ind = factorIdx >> log2n;         
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            /* if (indMsb==0) */
            xRe =  _hill(table[indLsb]);
            xIm =  _loll(table[indLsb]);

            if (indMsb == 1)
            {
                xRe =  _ssub(0, _loll(table[indLsb]));
                xIm =  _hill(table[indLsb]);
            }
            if (indMsb == 2)
            {
                xRe =  _ssub(0, _hill(table[indLsb]));
                xIm =  _ssub(0, _loll(table[indLsb]));
            }
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);
            xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
            xRe = (int32_t)((xReNew + 0x40000000) >> 31);
            xIm = (int32_t)((xImNew + 0x40000000) >> 31);
             wd[k + 1] = _itoll(xRe,xIm);
            
            factorIdx = i * 3;
            ind = factorIdx >> log2n;         
            indLsb = ind & 0xFF;
            indMsb = (ind >> 8) & 0x3;
            interIdx = factorIdx & (nn - 1);
            /*if (indMsb==0)*/
            xRe =  _hill(table[indLsb]);
            xIm =  _loll(table[indLsb]);

            if (indMsb == 1)
            {
                xRe =  _ssub(0, _loll(table[indLsb]));
                xIm =  _hill(table[indLsb]);
            }
            if (indMsb == 2)
            {
                xRe =  _ssub(0, _hill(table[indLsb]));
                xIm =  _ssub(0, _loll(table[indLsb]));
            }
            
            interReal = _hill(extTable[interIdx]);
            interImag = _loll(extTable[interIdx]);
            xReNew = _mpyidll(xRe ,interReal) - _mpyidll(xIm ,interImag);
            xImNew = _mpyidll(xIm, interReal) + _mpyidll(xRe ,interImag);
            xRe = (int32_t)((xReNew + 0x40000000) >> 31);
            xIm = (int32_t)((xImNew + 0x40000000) >> 31);
            wd[k + 2] = _itoll(xRe,xIm);
            k += 3;
        }
    }
    return 2 * k;
}
