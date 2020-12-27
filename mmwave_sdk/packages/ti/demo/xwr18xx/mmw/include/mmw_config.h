/**
 *   @file  mmw_config.h
 *
 *   @brief
 *      This is the header file that describes configurations for the Millimeter
 *   Wave Demo.
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
#ifndef MMW_CONFIG_H
#define MMW_CONFIG_H

/* MMWAVE library Include Files */
#include <ti/control/mmwave/mmwave.h>
#include <ti/common/sys_common.h>
#include <ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Millimeter Wave Demo Gui Monitor Selection
 *
 * @details
 *  The structure contains the selection for what information is placed to
 *  the output packet, and sent out to GUI. Unless otherwise specified,
 *  if the flag is set to 1, information
 *  is sent out. If the flag is set to 0, information is not sent out.
 *
 */
typedef struct MmwDemo_GuiMonSel_t
{
    /*! @brief   if 1: Send list of detected objects (see @ref DPIF_PointCloudCartesian) and
     *                 side info (@ref DPIF_PointCloudSideInfo).\n
     *           if 2: Send list of detected objects only (no side info)\n
     *           if 0: Don't send anything */
    uint8_t        detectedObjects;

    /*! @brief   Send log magnitude range array  */
    uint8_t        logMagRange;

    /*! @brief   Send noise floor profile */
    uint8_t        noiseProfile;

    /*! @brief   Send complex range bins at zero doppler, all antenna symbols for range-azimuth heat map */
    uint8_t        rangeAzimuthHeatMap;

    /*! @brief   Send complex range bins at zero doppler, (all antenna symbols), for range-azimuth heat map */
    uint8_t        rangeDopplerHeatMap;

    /*! @brief   Send stats */
    uint8_t        statsInfo;
} MmwDemo_GuiMonSel;

/**
 * @brief
 *  Millimeter Wave Demo Data Path Information.
 *
 * @details
 *  The structure is used to hold all the relevant information for
 *  the data path.
 */
typedef struct MmwDemo_platformCfg_t
{
    /*! @brief   GPIO index for sensor status */
    uint32_t            SensorStatusGPIO;
    
    /*! @brief   CPU Clock Frequency. */
    uint32_t            sysClockFrequency;

    /*! @brief   UART Logging Baud Rate. */
    uint32_t            loggingBaudRate;

    /*! @brief   UART Command Baud Rate. */
    uint32_t            commandBaudRate;
} MmwDemo_platformCfg;

/**
 * @brief
 *  Millimeter Wave Demo configuration
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  which is used to execute the Millimeter Wave Demo.
 */
typedef struct MmwDemo_Cfg_t
{
    /*! @brief   mmWave Control Configuration. */
    MMWave_CtrlCfg      ctrlCfg;

    /*! @brief   mmWave Open Configuration. */
    MMWave_OpenCfg      openCfg;

    /*! @brief   Platform specific configuration. */
    MmwDemo_platformCfg platformCfg;

    /*! @brief   Datapath output loggerSetting
                 0 (default): MSS UART logger
                 1: DSS UART logger
     */
    uint8_t              dataLogger;
} MmwDemo_Cfg;

typedef struct MmwDemo_DPC_ObjDet_CommonCfg_t
{
    /*! @brief Flag indicating if new MeasureRxChannelBiasCfg configuration is
     *         pending issuance to DPC */
    uint8_t isMeasureRxChannelBiasCfgPending : 1;

    /*! @brief Flag indicating if new CompRxChannelBiasCfg configuration is
     *         pending issuance to DPC */
    uint8_t isCompRxChannelBiasCfgPending    : 1;

    /*! @brief pre start common config */
    DPC_ObjectDetection_PreStartCommonCfg   preStartCommonCfg;
} MmwDemo_DPC_ObjDet_CommonCfg;

typedef struct MmwDemo_DPC_ObjDet_DynCfg_t
{
    /*! @brief Flag indicating if new CalibDcRangeSigCfg configuration is
     *         pending issuance to DPC */
    uint16_t isCalibDcRangeSigCfg : 1;

    /*! @brief Flag indicating if new CfarCfgRange configuration is
     *         pending issuance to DPC */
    uint16_t isCfarCfgRangePending : 1;

    /*! @brief Flag indicating if new CfarCfgDoppler configuration is
     *         pending issuance to DPC */
    uint16_t isCfarCfgDopplerPending : 1;

    /*! @brief Flag indicating if new FovRange configuration is
     *         pending issuance to DPC */
    uint16_t isFovRangePending : 1;

    /*! @brief Flag indicating if new FovDoppler configuration is
     *         pending issuance to DPC */
    uint16_t isFovDopplerPending : 1;

    /*! @brief Flag indicating if new multiObjBeamFormingCfg configuration is
     *         pending issuance to DPC */
    uint16_t isMultiObjBeamFormingCfgPending : 1;

    /*! @brief Flag indicating if new PrepareRangeAzimuthHeatMap configuration is
     *         pending issuance to DPC */
    uint16_t isPrepareRangeAzimuthHeatMapPending : 1;

    /*! @brief Flag indicating if new fovAoaCfg configuration is
     *         pending issuance to DPC */
    uint16_t isFovAoaCfgPending : 1;

    /*! @brief Flag indicating if new StaticClutterRemovalCfg configuration is
     *         pending issuance to DPC */
    uint16_t isStaticClutterRemovalCfgPending : 1;

    /*! @brief dynamic config */
    DPC_ObjectDetection_DynCfg   dynCfg;
} MmwDemo_DPC_ObjDet_DynCfg;

#ifdef __cplusplus
}
#endif

#endif /* MMW_CONFIG_H */
