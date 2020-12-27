/**
 *   @file  mathutils.c
 *
 *   @brief
 *      This file contains math utility functions.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

#include <ti/utils/mathutils/mathutils.h>

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to find floor(log2(x))
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     floor(log2(x))
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_floorLog2(uint32_t x)
{
#ifdef SUBSYS_DSS
    return (30 - _norm(x)); // On DSP _norm(x) counts number of number of redundant sign bits
#else
    return (31 - _norm(x)); // On Arm _norm(x) counts number of leading zeros
#endif
}

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to find ceil(log2(x))
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     ceil(log2(x))
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_ceilLog2(uint32_t x)
{
    uint32_t    idx;

    idx = mathUtils_floorLog2(x);

    if((0x1U<<idx) != x)
    {
        idx++;
    }
    return (idx);
}

/**
 *  @b Description
 *  @n
 *      Takes a 32bit integer to roundup value of power of 2.
 *
 *  @param[in]  x       32bit integer.
 *
 *  @retval     power of 2 roundup value.
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
uint32_t mathUtils_pow2roundup(uint32_t x)
{
    uint32_t power = 1U;

    while(power < x)
        power*=2U;

    return (power);
}

/**
 *  @b Description
 *  @n
 *      Function to generate window coefficients for FFT.
 *
 *  @param[out] win Pointer to output calculated window samples in Q17 format.
 *  @param[in]  winLen Length of window.
 *  @param[in]  winGenLen Length of window to be generated.
 *  @param[in]  winType Type of window, one of @ref FFT_WINDOW_TYPES
 *  @param[in]  qFormat Q format of generated window samples
 *  @retval none.
 *
 *  \ingroup MATHUTILS_EXTERNAL_FUNCTION
 */
void mathUtils_genWindow(uint32_t *win, uint32_t winLen,
                         uint32_t winGenLen, uint32_t winType, uint32_t qFormat)
{
    uint32_t  oneQFormat = (1U << qFormat);
    uint32_t    winIndx;
    float       phi;
    uint32_t    *pWinBuf;

    pWinBuf = win;

    phi = 2 * PI_ / ((float) winLen - 1);

    for(winIndx = 0; winIndx < winGenLen; winIndx++)
    {
        if(winType == MATHUTILS_WIN_BLACKMAN)
        {
            //Blackman window
            float a0 = 0.42;
            float a1 = 0.5;
            float a2 = 0.08;
            *pWinBuf = (uint32_t) ((oneQFormat * (a0 - a1*cos(phi * winIndx) +
                a2*cos(2 * phi * winIndx))) + 0.5);
            if(*pWinBuf >= oneQFormat)
            {
                *pWinBuf = oneQFormat - 1;
            }
        }
        else if(winType == MATHUTILS_WIN_HANNING)
        {
            //Hanning window
            *pWinBuf = (uint32_t) ((oneQFormat * 0.5* (1 - cos(phi * winIndx))) + 0.5);
            if(*pWinBuf >= oneQFormat)
            {
                *pWinBuf = oneQFormat - 1;
            }
        }
        else if(winType == MATHUTILS_WIN_RECT)
        {
            //Rectangular window
            *pWinBuf= (uint32_t) (oneQFormat/16);
        }
        pWinBuf++;
    }
}


