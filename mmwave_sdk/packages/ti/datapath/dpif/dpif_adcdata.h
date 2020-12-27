/*  \par
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
 
/**
 *   @file  dpif_adcdata.h
 *
 *   @brief
 *      Defines RF ADCBuf interface.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DPIF_ADCDATA_H
#define DPIF_ADCDATA_H

/* MMWAVE SDK Include Files */
#include <ti/common/sys_common.h>
#include <ti/datapath/dpif/dpif_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  RX channel ADC data interleave mode
 *
 * @details
 *  The enumeration describes the data interleave mode across RF RX channels
 */
typedef enum DPIF_RXCHAN_INTERLEAVE_e
{
    /*! @brief  Non-Interleave mode */
    DPIF_RXCHAN_NON_INTERLEAVE_MODE=0U,

    /*! @brief  Interleave mode */
    DPIF_RXCHAN_INTERLEAVE_MODE
}DPIF_RXCHAN_INTERLEAVE;

/**
 * @brief
 *  ADC Data buffer property
 *
 * @details
 *  The structure describes the properties for ADC data buffers
 */
typedef struct DPIF_ADCBufProperty_t
{
    /*! @brief  Data format in adcbuf */
    DPIF_DATAFORMAT     dataFmt;

    /*! @brief  Data in interleave or non-interleave mode */
    DPIF_RXCHAN_INTERLEAVE  interleave;

    /*! @brief  ADCBUF will generate chirp interrupt event every this many chirps - chirpthreshold */
    uint8_t             numChirpsPerChirpEvent;

    /*! @brief  ADC out bits 
     *          0(12 Bits), 1(14 Bits), 2(16 Bits)
                refer to rlAdcBitFormat_t for details
     */
    uint8_t             adcBits;

    /*! @brief  Number of receive antennas */
    uint8_t             numRxAntennas;

    /*! @brief  Number of ADC samples */
    uint16_t            numAdcSamples;

    /*! @brief  rxChan offset in ADCBuf, it is required in non-interleave mode
                The offset array is set for numRxAntennas starting from index 0 contiguously
                The offset is in number of bytes
     */
    uint16_t            rxChanOffset[SYS_COMMON_NUM_RX_CHANNEL];
}DPIF_ADCBufProperty;

/**
 * @brief
 *  ADC Data buffer definition
 *
 * @details
 *  The structure defines the ADC data buffer ,including data property, data size and data pointer
 */
typedef struct DPIF_ADCBufData_t
{
    /*! @brief  ADCBuf data property */
    DPIF_ADCBufProperty     dataProperty;

    /*! @brief  ADCBuf  buffer size in bytes */
    uint32_t                dataSize;

    /*! @brief  ADCBuf data pointer */
    void                    *data;
}DPIF_ADCBufData;


#ifdef __cplusplus
}
#endif

#endif /* DPIF_ADCDATA_H */
