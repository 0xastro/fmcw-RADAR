/**
 *   @file  dpif_radarcube.h
 *
 *   @brief
 *      Defines the data path radar cube data interface.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DPIF_RADARCUBE_H
#define DPIF_RADARCUBE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup DPIF_RADARCUBE_FORMAT     DPIF_RADARCUBE_FORMAT
 * @brief    Combination of C structure declaration and Content that uniquely describes the radar cube
 *
 *
 * # |Declaration                                                           |Content
 *---| ---------------------------------------------------------------------|-----------------------------
 * 1 |cmplx16ImRe_t x[numTXPatterns][numDopplerChirps][numRX][numRangeBins] |1D Range FFT output
 * 2 |cmplx16ImRe_t x[numRangeBins][numDopplerChirps][numTXPatterns][numRX] |1D Range FFT output
 * 3 |cmplx16ImRe_t x[numRangeBins][numTXPatterns][numRX][numDopplerChirps] |1D Range FFT output
 * 4 |cmplx16ImRe_t x[numRangeBins][numDopplerBins][numTXPatterns][numRX]   |2D (Range+Doppler) FFT output
 * 5 |cmplx16ImRe_t x[numRangeBins][numTXPatterns][numRX][numDopplerBins]   |2D (Range+Doppler) FFT output
 * @{
 */
#define DPIF_RADARCUBE_FORMAT_1   1  /*!<  This format is for 1D FFT output and it separates out different
                                           TX patterns in distinct groups while keeping the 
                                           "[numRX][numRangeBins]" samples in the same non-interleaved 
                                           format of ADC Data. */

#define DPIF_RADARCUBE_FORMAT_2   2  /*!<  This format is for 1D FFT output and it keeps the "[numRX]"
                                           samples in the same interleaved format of ADC Data
                                           @sa DPIF_RADARCUBE_FORMAT_4.*/

#define DPIF_RADARCUBE_FORMAT_3   3  /*!<  This format is for 1D FFT output and it linearizes and
                                          transposes the "[numTXPatterns][numRX][numDopplerChirps]" 
                                          samples as compared to non-interleaved format of ADC Data. 
                                          This format could be used when overlaying the same memory 
                                          with 2D FFT output. 
                                          @sa DPIF_RADARCUBE_FORMAT_5. */

#define DPIF_RADARCUBE_FORMAT_4   4  /*!<  This format is same as @ref DPIF_RADARCUBE_FORMAT_2 except that
                                          it contains 2D FFT output. */

#define DPIF_RADARCUBE_FORMAT_5   5  /*!<  This format is same as @ref DPIF_RADARCUBE_FORMAT_3 except that
                                          it contains 2D FFT output. */

/** @}*/ /*DPIF_RADARCUBE_FORMAT*/


/**
 * @brief
 *  Radar Cube Buffer Interface
 *
 * @details
 *  The structure defines the radar cube buffer interface, including
 * property, size and data pointer.
 */
typedef struct DPIF_RadarCube_t
{
    /*! @brief  Radar Cube data Format @ref DPIF_RADARCUBE_FORMAT */
    uint32_t                datafmt;

    /*! @brief  Radar Cube buffer size in bytes */
    uint32_t                dataSize;

    /*! @brief  Radar Cube data pointer
                User could remap this to specific typedef using 
                information in @ref DPIF_RADARCUBE_FORMAT */
    void                    *data;
}DPIF_RadarCube;


#ifdef __cplusplus
}
#endif

#endif /* DPIF_RADARCUBE_H */
