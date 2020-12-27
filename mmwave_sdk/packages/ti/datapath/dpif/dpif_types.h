/**
 *   @file  dpif_types.h
 *
 *   @brief
 *      Defines the common data types used in data path data interface.
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
#ifndef DPIF_TYPES_H
#define DPIF_TYPES_H

/* MMWAVE SDK Include Files */
#include <ti/common/sys_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Data format for data Path interface buffers
 *
 * @details
 *  The enumeration describes the data format used in data interface buffers between data processing unit
 */
typedef enum DPIF_DATAFORMAT_e
{
    /*! @brief  Complex 16bit for imaginary/real */
    DPIF_DATAFORMAT_COMPLEX16_IMRE=0U,

    /*! @brief  Complex 16bit for real/imaginary */
    DPIF_DATAFORMAT_COMPLEX16_REIM,

    /*! @brief  Complex 32bit for imaginary/real */
    DPIF_DATAFORMAT_COMPLEX32_IMRE,

    /*! @brief  Complex 32bit for real/imaginary */
    DPIF_DATAFORMAT_COMPLEX32_REIM,

    /*! @brief  Float data format */
    DPIF_DATAFORMAT_FLOAT,

    /*! @brief  Real 16bit data */
    DPIF_DATAFORMAT_REAL16,

    /*! @brief  Real 32bit data */
    DPIF_DATAFORMAT_REAL32
}DPIF_DATAFORMAT;

#ifdef __cplusplus
}
#endif

#endif /* DPIF_TYPES_H */
