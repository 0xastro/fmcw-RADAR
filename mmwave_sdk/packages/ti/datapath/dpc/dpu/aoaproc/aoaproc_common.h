/**
 *   @file  aoaproc_common.h
 *
 *   @brief
 *      Implements Common definition across cfarcaProc DPU.
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
#ifndef AOAPROC_COMMON_H
#define AOAPROC_COMMON_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Field of view - AoA Configuration
 *
 * @details
 *  The structure contains the field of view - AoA configuration used in data path
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProc_FovAoaCfg_t
{
    /*! @brief    minimum azimuth angle (in degrees) exported to Host*/
    float        minAzimuthDeg;

    /*! @brief    maximum azimuth angle (in degrees) exported to Host*/
    float        maxAzimuthDeg;

    /*! @brief    minimum elevation angle (in degrees) exported to Host*/
    float        minElevationDeg;

    /*! @brief    maximum elevation angle (in degrees) exported to Host*/
    float        maxElevationDeg;
} DPU_AoAProc_FovAoaCfg;

/**
 * @brief
 *  Multi object beam forming Configuration
 *
 * @details
 *  The structure contains the Peak grouping configuration used in data path
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProc_MultiObjBeamFormingCfg_t
{
    /*! @brief    enabled flag:  1-enabled 0-disabled */
    uint8_t     enabled;

    /*! @brief    second peak detection threshold */
    float        multiPeakThrsScal;

} DPU_AoAProc_MultiObjBeamFormingCfg;

/**
 * @brief Range Bias and rx channel gain/phase compensation configuration.
 *
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProc_compRxChannelBiasCfg_t
{

    /*! @brief  Compensation for range estimation bias in meters */
    float rangeBias;

    /*! @brief  Compensation for Rx channel phase bias in Q15 format.
     *          The order here is like x[tx][rx] where rx order is 0,1,....SYS_COMMON_NUM_RX_CHANNEL-1
     *          and tx order is 0, 1,...,SYS_COMMON_NUM_TX_ANTENNAS-1 */
    cmplx16ImRe_t rxChPhaseComp[SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL];

} DPU_AoAProc_compRxChannelBiasCfg;


/**
 * @brief
 *  AoAProc dynamic configuration
 *
 * @details
 *  The structure is used to hold the dynamic configuration used for AoA
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProc_DynamicConfig_t
{

    /** @brief     Multiobject beam forming configuration */
    DPU_AoAProc_MultiObjBeamFormingCfg *multiObjBeamFormingCfg;

    /** @brief     Flag indicates to prepare data for azimuth heat-map */
    bool  prepareRangeAzimuthHeatMap;

    /** @brief     Pointer to Rx channel compensation configuration */
    DPU_AoAProc_compRxChannelBiasCfg *compRxChanCfg;

    /** @brief      Field of view configuration for angle of arrival */
    DPU_AoAProc_FovAoaCfg     *fovAoaCfg;

} DPU_AoAProc_DynamicConfig;


/**
 * @brief
 *  Data processing Unit statistics
 *
 * @details
 *  The structure is used to hold the statistics of the DPU 
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_AoAProc_Stats_t
{
    /** @brief total processing time during all chirps in a frame excluding EDMA waiting time*/
    uint32_t            processingTime;

    /** @brief total wait time for EDMA data transfer during all chirps in a frame*/
    uint32_t            waitTime;
} DPU_AoAProc_Stats;

#ifdef __cplusplus
}
#endif

#endif 
