/**
 *   @file  dpif_detmatrix.h
 *
 *   @brief
 *      Defines the detection matrix buffer interface
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
#ifndef DPIF_DETMATRIX_H
#define DPIF_DETMATRIX_H


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup DPIF_DETMATRIX_FORMAT     DPIF_DETMATRIX_FORMAT
 * @brief    Combination of C structure declaration and Content that uniquely describes the detection matrix
 *
 *
 * # |Declaration                              |Content
 *---| ----------------------------------------|------------------------------
 * 1 |uint16_t x[numRangeBins][numDopplerBins] |Range-Doppler Detection Matrix
 * @{
 */
#define DPIF_DETMATRIX_FORMAT_1   1  /*!< This format has doppler bins laid linearly for a given range bin */

/** @}*/ /*DPIF_DETMATRIX_FORMAT*/

/**
 * @brief
 *  Detection matrix buffer interface
 *
 * @details
 *  The structure defines the detection matrix buffer interface, including data property, 
 * size and pointer
 */
typedef struct DPIF_DetMatrix_t
{
    /*! @brief  Detection Matrix data Format @ref DPIF_DETMATRIX_FORMAT */
    uint32_t                datafmt;

    /*! @brief  Detection Matrix buffer size in bytes */
    uint32_t                dataSize;

    /*! @brief  Detection Matrix data pointer
                User could remap this to specific typedef using 
                information in @ref DPIF_DETMATRIX_FORMAT */
    void                    *data;
}DPIF_DetMatrix;

#ifdef __cplusplus
}
#endif

#endif /* DPIF_DETMATRIX_H */
