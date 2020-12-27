/**************************************************************************
 *   @file  mathutils.h
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
 
#ifndef MATHUTILS_H
#define MATHUTILS_H

/**
@defgroup MATHUTILS_EXTERNAL_FUNCTION            Math Utility External Functions
@ingroup RANGE_PROC_DPU
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the mathUtils
*/

#ifdef __cplusplus
extern "C" {
#endif

/*! PI constant */
#define PI_ 3.1415926535897

/**
 * @defgroup  FFT_WINDOW_TYPES        FFT_WINDOW_TYPES
 * @brief    FFT Window Types supptored in mathutils library
 * @{
 */
/*! @brief Hanning window */
#define MATHUTILS_WIN_HANNING     0U

/*! @brief Blackman window */
#define MATHUTILS_WIN_BLACKMAN 1U

/*! @brief Rectangular window */
#define MATHUTILS_WIN_RECT     2U
/** @}*/ /*FFT_WINDOW_TYPES*/

/*! Round macro used to round float to integer, integer can be any size depending
 *  on usage e.g float y = 2.7; int32_t x = (int32_t)MATHUTILS_ROUND_FLOAT(y), x will be 3 */
#define MATHUTILS_ROUND_FLOAT(x) ((x) < 0 ? ((x) - 0.5) : ((x) + 0.5) )

/*! Saturate to 16-bit high macro. Usage y = MATHUTILS_SATURATE16_HIGH(x) */
#define MATHUTILS_SATURATE16_HIGH(x) ( (x) > 32767 ? 32767 : (x))

/*! Saturate to 16-bit Low macro. Usage y = MATHUTILS_SATURATE16_HIGH(x) */
#define MATHUTILS_SATURATE16_LOW(x) ( (x) < -32768 ? -32768 : (x))

/*! Saturate to 16-bit macro. Usage MATHUTILS_SATURATE16(x), saturates x back to x */
#define MATHUTILS_SATURATE16(x) \
    if (x > 32767) { \
        x = 32767;   \
    }                \
    if (x < -32768)  \
    {                \
        x = -32768;  \
    }

/*! Rounding to Q15. Usage y = MATHUTILS_ROUND_Q15(x) */
#define MATHUTILS_ROUND_Q15(x) \
    ((x) + 0x4000) >> 15;

/*! Round and saturate to Q15. Usage MATHUTILS_ROUND_AND_SATURATE_Q15(x), rounds
 *  and saturates x back to itself */
#define MATHUTILS_ROUND_AND_SATURATE_Q15(x) \
        x = MATHUTILS_ROUND_Q15(x) \
        MATHUTILS_SATURATE16(x)

uint32_t mathUtils_ceilLog2(uint32_t x);
uint32_t mathUtils_floorLog2(uint32_t x);
uint32_t mathUtils_pow2roundup(uint32_t x);
void mathUtils_genWindow(uint32_t *win, uint32_t winLen,
                         uint32_t winGenLen, uint32_t winType, uint32_t qFormat);

#ifdef __cplusplus
}
#endif

#endif
