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
 *   @file     mmwavelib_mattrans.c
 *
 *   @brief    Matrix transpose routine
 */

#include <stdint.h>
#include <c6x.h>
#include <ti/alg/mmwavelib/src/matutils/mmwavelib_matutils.h>


/*!*******************************************************************************************
 * @brief       This function transposes the matrix x[] and writes the result to matrix y[].
 *              It is the DSPLIB function DSPF_sp_mat_trans modified by only swapping
 *              rows to the inner loop and cols to the outer loop in the function,
 *              in order to be cycles efficient for matrix with rows larger than columns.
 *
 * @param[in]    x      : Input matrix containing rows*cols 32bit floating-point numbers.
 * @param[in]    rows   : Number of rows in matrix x. Also number of columns in matrix y.
 * @param[in]    cols   : Number of columns in matrix x. Also number of rows in matrix y.
 * @param[out]   y      : Output matrix containing cols*rows 32bit floating-point numbers.
 *
 * @return      void
 *
 * @pre         (1)x and y are 8 byte aligned, and do not overlap.
 * @pre         (2)rows and cols are multiple of 2, and >=2.
 *
 * \ingroup     MMWAVELIB_MAT_UTILS
 *
 * Cycles       0.75*rows*cols + 3*cols + 35 (cols <=6)
 *              1*rows*cols + 3*cols + 35 (cols >=8) (cgt 8.1.3)
 *********************************************************************************************
 */
void mmwavelib_mattrans(const float32_t *restrict x,
                        const int32_t rows,
                        const int32_t cols,
                        float32_t *restrict y)
{
    int32_t    i, j;
    __float2_t x_10, x_32;

    _nassert(rows >= 2);
    _nassert(cols >= 2);
    _nassert(rows % 2 == 0);
    _nassert(cols % 2 == 0);
    _nassert((int32_t)x % 8 == 0);
    _nassert((int32_t)y % 8 == 0);
  
    #pragma MUST_ITERATE(1,,)
    for(j = 0; j < cols; j+=2)
    {
        #pragma MUST_ITERATE(1,,)
        for(i = 0; i < rows; i+=2)
        {
            x_10 = _amem8_f2_const(&x[j + cols * i   ]);
            x_32 = _amem8_f2_const(&x[j + cols *(i+1)]);
            _amem8_f2(&y[rows * j    + i]) = _ftof2(_lof2(x_32), _lof2(x_10));
            _amem8_f2(&y[rows *(j+1) + i]) = _ftof2(_hif2(x_32), _hif2(x_10));
        }
    }
}
