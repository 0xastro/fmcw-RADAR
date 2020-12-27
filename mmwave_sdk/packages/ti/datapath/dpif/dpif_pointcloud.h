/**
 *   @file  dpif_pointcloud.h
 *
 *   @brief
 *      Defines the point cloud interfaces used in data path processing.
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
#ifndef DPIF_POINTCLOUD_H
#define DPIF_POINTCLOUD_H

/* MMWAVE SDK Include Files */
#include <ti/common/sys_types.h>
#include <ti/datapath/dpif/dpif_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Point cloud definition in Cartesian coordinate system
 */
typedef struct DPIF_PointCloudCartesian_t
{
    /*! @brief  x - coordinate in meters */
    float  x;

    /*! @brief  y - coordinate in meters */
    float  y;

    /*! @brief  z - coordinate in meters */
    float  z;

    /*! @brief  Doppler velocity estimate in m/s. Positive velocity means target
     *          is moving away from the sensor and negative velocity means target
     *          is moving towards the sensor. */
    float    velocity;
}DPIF_PointCloudCartesian;

/**
 * @brief
 *  Alignment for memory allocation purposes for structure @ref DPIF_PointCloudCartesian_t
 *  when structure is accessed by the CPU. Alignment is the maximum sized field in the structure.
 */
#define DPIF_POINT_CLOUD_CARTESIAN_CPU_BYTE_ALIGNMENT   (sizeof(float))

/**
 * @brief
 *  Point cloud side information such as SNR and noise level
 *
 * @details
 *  The structure describes the field for a point cloud in XYZ format
 */
typedef struct DPIF_PointCloudSideInfo_t
{
    /*! @brief  snr - CFAR cell to side noise ratio in dB expressed in 0.1 steps of dB */
    int16_t  snr;

    /*! @brief  y - CFAR noise level of the side of the detected cell in dB expressed in 0.1 steps of dB */
    int16_t  noise;
}DPIF_PointCloudSideInfo;

/**
 * @brief
 *  Alignment for memory allocation purposes for structure @ref DPIF_PointCloudSideInfo_t
 *  when structure is accessed by the CPU. Alignment is the maximum sized field in the structure.
 */
#define DPIF_POINT_CLOUD_SIDE_INFO_CPU_BYTE_ALIGNMENT   (sizeof(int16_t))

/**
 * @brief
 *  Point cloud definition in spherical coordinate system
 */
typedef struct DPIF_PointCloudSpherical_t
{
    /*!< @brief     Range in meters */
    float  range;

    /*!< @brief     Azimuth angle in degrees */
    float  azimuthAngle;

    /*!< @brief     Elevation angle in degrees */
    float  elevAngle;

    /*! @brief  Doppler velocity estimate in m/s. Positive velocity means target
     *          is moving away from the sensor and negative velocity means target
     *          is moving towards the sensor. */
    float    velocity;
}DPIF_PointCloudSpherical;

/**
 * @brief
 *  Alignment for memory allocation purposes for structure @ref DPIF_PointCloudSpherical_t
 *  when structure is accessed by the CPU. Alignment is the maximum sized field in the structure.
 */
#define DPIF_POINT_CLOUD_SPHERICAL_CPU_BYTE_ALIGNMENT   (sizeof(float))


/**
 * @brief
 *  CFAR detection output
 *
 * @details
 *  The holds CFAR detections with SNR
 *
 */
typedef volatile struct DPIF_CFARDetList_t
{
    uint16_t   rangeIdx;   /*!< Range index */
    uint16_t   dopplerIdx; /*!< Doppler index */
    int16_t    snr;        /*!< Signal to noise power ratio in steps of 0.1 dB */
    int16_t    noise;      /*!< Noise level in steps of 0.1 dB */
} DPIF_CFARDetList;

/**
 * @brief
 *  Alignment for memory allocation purposes for structure @ref DPIF_CFARDetList
 *  when structure is accessed by the CPU. Alignment is the maximum sized field in the structure.
 */
#define DPIF_CFAR_DET_LIST_CPU_BYTE_ALIGNMENT   (sizeof(int16_t))

#ifdef __cplusplus
}
#endif

#endif /* DPIF_POINTCLOUD_H */
