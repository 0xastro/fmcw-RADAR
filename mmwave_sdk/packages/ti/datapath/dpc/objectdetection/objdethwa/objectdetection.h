/**
 *   @file  objectdetection.h
 *
 *   @brief
 *      Object Detection DPC Header File
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
#ifndef DPC_OBJECTDETECTION__H
#define DPC_OBJECTDETECTION__H

/* MMWAVE Driver Include Files */
#include <ti/common/mmwave_error.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/hwa/hwa.h>
#include <ti/control/dpm/dpm.h>

#include <ti/datapath/dpu/rangeproc/rangeprochwa.h>
#include <ti/datapath/dpc/dpu/staticclutterproc/staticclutterproc.h>
#include <ti/datapath/dpc/dpu/cfarcaproc/cfarcaprochwa.h>
#include <ti/datapath/dpc/dpu/aoaproc/aoaprochwa.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup DPC_OBJDET_EXTERNAL        Object Detection DPC (Data-path Processing Chain) External
 *
 * This DPC performs processes ADC samples and generates detected object list.
 */
/**
@defgroup DPC_OBJDET__GLOBAL                             Object Detection DPC Globals
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the globals exposed by the Object detection DPC.
*/
/**
@defgroup DPC_OBJDET_IOCTL__DATA_STRUCTURES               Object Detection DPC Data Structures
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the data structures which are a part of the DPC module
*   and which are exposed to the application
*/
/**
@defgroup DPC_OBJDET_IOCTL__DEFINITIONS                   Object Detection DPC Definitions
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the definitions which are used by the Object Detection DPC.
*/
/**
@defgroup DPC_OBJDET_IOCTL__COMMAND                       Object Detection DPC Configuration Commands
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the commands which are supported by the DPC.
*   All commands of the type IOCTL__STATIC_<...> can only be issued either before the
*   first call to @ref DPM_start (DPC_ObjectDetection_start) or after the @ref DPM_stop (DPC_ObjectDetection_stop)
*   All commands of the type IOCTL__DYNAMIC_<...> can be issued between at
*   the inter-frame boundary i.e when the result is available from @ref DPM_execute (DPC_ObjectDetection_execute).
*   All commands of the type IOCTL__STATIC_<..> must be issued
*   before @ref DPM_start (DPC_ObjectDetection_start) because there are no defaults.
*/
/**
@defgroup DPC_OBJECTDETECTION_ERROR_CODE                   Object Detection DPC Error Codes
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the error codes returned when calling Object Detection DPC functions
*   during error conditions.
*/


/** @addtogroup DPC_OBJDET_IOCTL__DEFINITIONS
 @{ */
/* Currently nothing here */
/**
@}
*/

/** @addtogroup DPC_OBJDET_IOCTL__DATA_STRUCTURES
 @{ */

/**
 * @brief
 *  DC range signature compensation
 *
 * @details
 *  The structure contains the DC range antenna signature removeal configuration used in data path
 */
typedef struct DPC_ObjectDetection_CalibDcRangeSigCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
    uint8_t subFrameNum;

    /*! @brief   Calibration DC Range configuration */
    DPU_RangeProc_CalibDcRangeSigCfg cfg;
}DPC_ObjectDetection_CalibDcRangeSigCfg;

/**
 * @brief
 *  CFAR Configuration
 *
 * @details
 *  The structure contains the cfar configuration used in data path
 */
typedef struct DPC_ObjectDetection_CfarCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
    uint8_t subFrameNum;

    /*! @brief   CFAR Configuration */
    DPU_CFARCAProc_CfarCfg cfg;
}DPC_ObjectDetection_CfarCfg;

/*! @brief      Field of view configuration in range domain */
typedef struct DPC_ObjectDetection_fovRangeCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
   uint8_t subFrameNum;

   /*! @brief      Field of view configuration in range domain */
   DPU_CFARCAProc_FovCfg cfg;
} DPC_ObjectDetection_fovRangeCfg;

/*! @brief      Field of view configuration in Doppler domain */
typedef struct DPC_ObjectDetection_fovDopplerCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
   uint8_t subFrameNum;

   /*! @brief      Field of view configuration in Doppler domain */
   DPU_CFARCAProc_FovCfg cfg;
} DPC_ObjectDetection_fovDopplerCfg;

/**
 * @brief
 *  Multi-object beam forming Configuration.
 */
typedef struct DPC_ObjectDetection_MultiObjBeamFormingCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
    uint8_t subFrameNum;

    /*! @brief   Multi Object Beam Forming configuration */
    DPU_AoAProc_MultiObjBeamFormingCfg cfg;
}DPC_ObjectDetection_MultiObjBeamFormingCfg;

/*
 * @brief Field of view configuration for AoA.
 *
 */
typedef struct DPC_ObjectDetection_fovAoaCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
   uint8_t subFrameNum;

   /*! @brief    Field of view configuration for AoA */
   DPU_AoAProc_FovAoaCfg cfg;
} DPC_ObjectDetection_fovAoaCfg;

/*
 * @brief Range Bias and rx channel gain/phase compensation configuration.
 *
 */
typedef struct DPC_ObjectDetection_RangeAzimuthHeatMapCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
   uint8_t subFrameNum;

   /*!< @brief     Flag indicates to prepare data for azimuth heat-map */
   bool  prepareRangeAzimuthHeatMap;
} DPC_ObjectDetection_RangeAzimuthHeatMapCfg;

/**
 * @brief
 *  Static clutter removal base (subframe agnostic) configuration
 *
 * @details
 *  The structure contains Static clutter removal configuration
 */
typedef struct DPC_ObjectDetection_StaticClutterRemovalCfg_Base_t
{
    /*! @brief   enabled flag:  1-enabled 0-disabled */
    uint8_t enabled;
} DPC_ObjectDetection_StaticClutterRemovalCfg_Base;

/**
 * @brief
 *  Static clutter removal configuration
 *
 * @details
 *  The structure contains static clutter removal configuration
 */
typedef struct DPC_ObjectDetection_StaticClutterRemovalCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
    uint8_t subFrameNum;

    /*! @brief   Static clutter Removal Cfg */
    DPC_ObjectDetection_StaticClutterRemovalCfg_Base cfg;
} DPC_ObjectDetection_StaticClutterRemovalCfg;

/**
 * @brief Range Bias and rx channel gain/phase measurement configuration.
 *
 */
typedef struct DPC_ObjectDetection_MeasureRxChannelBiasCfg_t
{
    /*! @brief  1-enabled 0-disabled */
    uint8_t enabled;

    /*! @brief  Target distance during measurement (in meters) */
    float targetDistance;

    /*! @brief  Search window size (in meters), the search is done in range
     *          [-searchWinSize/2 + targetDistance, targetDistance + searchWinSize/2] */
    float searchWinSize;
} DPC_ObjectDetection_MeasureRxChannelBiasCfg;

/*!
*  @brief      Call back function type for calling back during process
*  @param[out] subFrameIndx Sub-frame indx [0..(numSubFrames-1)]
*/
typedef void (*DPC_ObjectDetection_processCallBackFxn_t)(uint8_t subFrameIndx);

/*! @brief  Process call backs configuration */
typedef struct DPC_ObjectDetection_ProcessCallBackCfg_t
{
    /*! @brief  Call back function that will be called at the beginning of frame
     *          processing (beginning of 1D) */
    DPC_ObjectDetection_processCallBackFxn_t processFrameBeginCallBackFxn;

    /*! @brief  Call back function that will be called at the beginning of inter-frame
     *          processing (beginning of 2D) */
    DPC_ObjectDetection_processCallBackFxn_t processInterFrameBeginCallBackFxn;
} DPC_ObjectDetection_ProcessCallBackCfg;

/*
 * @brief Memory Configuration used during init API
 */
typedef struct DPC_ObjectDetection_MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which DPC will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC */
    uint32_t size;
} DPC_ObjectDetection_MemCfg;

/*
 * @brief Configuration for DPM's init API.
 *        DPM_init's arg = pointer to this structure.
 *        DPM_init's argLen = size of this structure.
 *
 */
typedef struct DPC_ObjectDetection_InitParams_t
{
   /*! @brief   Handle to the hardware accelerator */
   HWA_Handle hwaHandle;

   /*! @brief   Handle of the EDMA driver. */
   EDMA_Handle edmaHandle[EDMA_NUM_CC];

   /*! @brief L3 RAM configuration. DPC will allocate memory from this
    *         as needed and report the amount of memory consumed through 
    *         @ref DPC_ObjectDetection_PreStartCfg to application */
   DPC_ObjectDetection_MemCfg L3ramCfg;

   /*! @brief Core Local RAM configuration (e.g data TCM for R4F).
    *         DPC will allocate memory from this as needed and report the amount
    *         of memory consumed through @ref DPC_ObjectDetection_PreStartCfg
    *         to the application */
   DPC_ObjectDetection_MemCfg CoreLocalRamCfg;

   /*! @brief   Process call back function configuration */
   DPC_ObjectDetection_ProcessCallBackCfg processCallBackCfg;
} DPC_ObjectDetection_InitParams;

/*
 * @brief Static Configuration that is part of the pre-start configuration.
 */
typedef struct DPC_ObjectDetection_StaticCfg_t
{
   /*!< @brief      ADCBuf buffer interface */
   DPIF_ADCBufData  ADCBufData;

   /*! @brief  Rx Antenna order */
   uint8_t     rxAntOrder[SYS_COMMON_NUM_RX_CHANNEL];

   /*! @brief  Tx Antenna order */
   uint8_t     txAntOrder[SYS_COMMON_NUM_TX_ANTENNAS];

   /*! @brief  Number of transmit antennas */
   uint8_t     numTxAntennas;

   /*! @brief  Number of virtual antennas */
   uint8_t     numVirtualAntennas;

   /*! @brief  Number of virtual azimuth antennas */
   uint8_t     numVirtualAntAzim;

   /*! @brief  Number of virtual elevation antennas */
   uint8_t     numVirtualAntElev;

   /*! @brief  Number of range FFT bins, this is at a minimum the next power of 2 of
               @ref DPIF_ADCBufProperty_t::numAdcSamples. If range zoom is supported, this can be bigger than
               the minimum. */
   uint16_t    numRangeBins;

   /*! @brief  Number of chirps per frame */
   uint16_t    numChirpsPerFrame;

   /*! @brief Number of chirps for Doppler computation purposes.
              For example, in TDM/BPM-MIMO scheme, this is the physical chirps
              in a frame per transmit antenna
              i.e numDopplerChirps = @ref numChirpsPerFrame / @ref numTxAntennas */
   uint16_t    numDopplerChirps;

   /*! @brief  Number of Doppler FFT bins, this is at a minimum the next power of 2 of
               @ref numDopplerChirps. If Doppler zoom is supported, this can be bigger
               than the minimum. */
   uint16_t    numDopplerBins;

   /*! @brief  Range conversion factor for FFT range index to meters */
   float       rangeStep;

   /*! @brief  Doppler conversion factor for Doppler FFT index to m/s */
   float       dopplerStep;

   /*! @brief   1 if valid profile has one Tx per chirp else 0 */
  uint8_t      isValidProfileHasOneTxPerChirp;
} DPC_ObjectDetection_StaticCfg;

/*
 * @brief Dynamic Configuration that is part of the pre-start configuration.
 */
typedef struct DPC_ObjectDetection_DynCfg_t
{
    /*! @brief   Calibration DC Range configuration */
    DPU_RangeProc_CalibDcRangeSigCfg calibDcRangeSigCfg;

    /*! @brief      CFAR configuration in range direction */
    DPU_CFARCAProc_CfarCfg cfarCfgRange;

    /*! @brief      CFAR configuration in Doppler direction */
    DPU_CFARCAProc_CfarCfg cfarCfgDoppler;

    /*! @brief      Field of view configuration in range domain */
    DPU_CFARCAProc_FovCfg fovRange;

    /*! @brief      Field of view configuration in Doppler domain */
    DPU_CFARCAProc_FovCfg fovDoppler;

    /*! @brief   Multi Object Beam Forming configuration */
    DPU_AoAProc_MultiObjBeamFormingCfg multiObjBeamFormingCfg;

    /*!< @brief     Flag indicates to prepare data for azimuth heat-map */
    bool  prepareRangeAzimuthHeatMap;

    /*! @brief      Field of view configuration for AoA */
    DPU_AoAProc_FovAoaCfg fovAoaCfg;

    /*! @brief   Static Clutter Removal Cfg */
    DPC_ObjectDetection_StaticClutterRemovalCfg_Base staticClutterRemovalCfg;
} DPC_ObjectDetection_DynCfg;

/*
 * @brief Configuration related to IOCTL API for command
 *        @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG. This is independent
 *        of sub frame.
 */
typedef struct DPC_ObjectDetection_PreStartCommonCfg_t
{
    /*! @brief   Number of sub-frames */
    uint8_t numSubFrames;

    /*! @brief   Range Bias and rx channel gain/phase measurement configuration */
    DPC_ObjectDetection_MeasureRxChannelBiasCfg measureRxChannelBiasCfg;

    /*! @brief   Range Bias and rx channel gain/phase compensation configuration */
    DPU_AoAProc_compRxChannelBiasCfg compRxChanCfg;
} DPC_ObjectDetection_PreStartCommonCfg;

/*
 * @brief  Structure related to @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG
 *        IOCTL command. When the pre-start IOCTL is processed, it will report
 *        memory usage as part of @DPC_ObjectDetection_PreStartCfg.
 */
typedef struct DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage_t
{
    /*! @brief   Indicates number of bytes of L3 memory allocated to be used by DPC */
    uint32_t L3RamTotal;

    /*! @brief   Indicates number of bytes of L3 memory used by DPC from the allocated
     *           amount indicated through @ref DPC_ObjectDetection_InitParams */
    uint32_t L3RamUsage;

    /*! @brief   Indicates number of bytes of Core Local memory allocated to be used by DPC */
    uint32_t CoreLocalRamTotal;

    /*! @brief   Indicates number of bytes of Core Local memory used by DPC from the allocated
     *           amount indicated through @ref DPC_ObjectDetection_InitParams */
    uint32_t CoreLocalRamUsage;

    /*! @brief   Indicates number of bytes of system heap allocated */
    uint32_t SystemHeapTotal;

    /*! @brief   Indicates number of bytes of system heap used at the end of PreStartCfg */
    uint32_t SystemHeapUsed;

    /*! @brief   Indicates number of bytes of system heap used by DCP at the end of PreStartCfg */
    uint32_t SystemHeapDPCUsed;
} DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage;

/*
 * @brief Configuration related to IOCTL API for command
 *        @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG.
 *
 */
typedef struct DPC_ObjectDetection_PreStartCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
    uint8_t subFrameNum;

    /*! Static configuration */
    DPC_ObjectDetection_StaticCfg staticCfg;

    /*! Dynamic configuration */
    DPC_ObjectDetection_DynCfg dynCfg;

    /*! Memory usage after the preStartCfg is applied */
    DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage memUsage;
} DPC_ObjectDetection_PreStartCfg;

/*
 * @brief Stats structure to convey to Application timing and related information.
 */
typedef struct DPC_ObjectDetection_Stats_t
{
    /*! @brief   interChirpProcess margin in CPU cyctes */
    uint32_t      interChirpProcessingMargin;

    /*! @brief   Counter which tracks the number of frame start interrupt */
    uint32_t      frameStartIntCounter;

    /*! @brief   Frame start CPU time stamp */
    uint32_t      frameStartTimeStamp;

    /*! @brief   Inter-frame start CPU time stamp */
    uint32_t      interFrameStartTimeStamp;

    /*! @brief   Inter-frame end CPU time stamp */
    uint32_t      interFrameEndTimeStamp;

    /*! @brief Sub frame preparation cycles. Note when this is reported as part of
     *         the process result reporting, then it indicates the cycles that took
     *         place in the previous sub-frame/frame for preparing to switch to
     *         the sub-frame that is being reported because switching happens
     *         in the processing of DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED,
     *         which is after the DPC process. */
    uint32_t      subFramePreparationCycles;
} DPC_ObjectDetection_Stats;

/*
 * @brief This is the result structure reported from DPC's registered processing function
 *        to the application through the DPM_Buffer structure. The DPM_Buffer's
 *        first fields will be populated as follows:
 *        pointer[0] = pointer to this structure.
 *        size[0] = size of this structure i.e sizeof(DPC_ObjectDetection_Result)
 *
 *        pointer[1..3] = NULL and size[1..3] = 0.
 */
typedef struct DPC_ObjectDetection_ExecuteResult_t
{
    /*! @brief      Sub-frame index, this is in the range [0..numSubFrames - 1] */
    uint8_t         subFrameIdx;

    /*! @brief      Number of detected objects */
    uint32_t         numObjOut;

    /*! @brief      Detected objects output list of @ref numObjOut elements */
    DPIF_PointCloudCartesian *objOut;

    /*! @brief      Radar Cube structure */
    DPIF_RadarCube  radarCube;

    /*! @brief      Detection Matrix structure */
    DPIF_DetMatrix  detMatrix;

    /*! @brief      Detected objects side information (snr + noise) output list,
     *              of @ref numObjOut elements  */
    DPIF_PointCloudSideInfo *objOutSideInfo;

    /*! @brief      Pointer to range-azimuth static heat map, this is a 2D FFT
     *              array in range direction (cmplx16ImRe_t x[numRangeBins][numVirtualAntAzim]),
     *              at doppler index 0 */
    cmplx16ImRe_t   *azimuthStaticHeatMap;

    /*! @brief      Number of elements of @ref azimuthStaticHeatMap, this will be
     *              @ref DPC_ObjectDetection_StaticCfg_t::numVirtualAntAzim *
     *              @ref DPC_ObjectDetection_StaticCfg_t::numRangeBins */
    uint32_t        azimuthStaticHeatMapSize;

    /*! @brief      Pointer to DPC stats structure */
    DPC_ObjectDetection_Stats *stats;

    /*! @brief   Pointer to Range Bias and rx channel gain/phase compensation measurement
     *           result. Note the contents of this pointer are independent of sub-frame
     *           i.e all sub-frames will report the same result although it is
     *           expected that when measurement is enabled,
     *           the number of sub-frames will be 1 (i.e advanced frame
     *           feature will be disabled). If measurement
     *           was not enabled, then this pointer will be NULL. */
    DPU_AoAProc_compRxChannelBiasCfg *compRxChanBiasMeasurement;
} DPC_ObjectDetection_ExecuteResult;

/*
 * @brief This is the informational structure related to the IOCTL command
 *        @ref DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED.
 */
typedef struct DPC_ObjectDetection_ExecuteResultExportedInfo_t
{
    /*! @brief      Sub-frame index, this is in the range [0..numSubFrames - 1].
     *              This is the sub-frame whose results have been exported.
     *              Although this DPC implementation knows what sub-frame to expect as the exports
     *              are expected to be sequential in sub-frames, this field helps
     *              in error checking when for example the application could miss
     *              exporting/consuming a sub-frame in a timely manner or have out of order
     *              export/consumption. */
    uint8_t         subFrameIdx;
} DPC_ObjectDetection_ExecuteResultExportedInfo;

/**
@}
*/

/** @addtogroup DPC_OBJDET_IOCTL__COMMAND
 @{ */

/**
 * @brief Command associated with @ref DPC_ObjectDetection_PreStartCfg_t. In this IOCTL, the sub-frame's
 *        configurations will be processed by configuring individual DPUs.
 *        The @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG must be issued
 *        before issuing this IOCTL.
 */
#define DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG                            (DPM_CMD_DPC_START_INDEX + 0U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_PreStartCommonCfg_t. Must be issued before
 *        issuing @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG
 */
#define DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG                     (DPM_CMD_DPC_START_INDEX + 1U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_CfarCfg_t for range dimension.
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_CFAR_RANGE_CFG                          (DPM_CMD_DPC_START_INDEX + 2U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_CfarCfg_t for doppler dimension.
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_CFAR_DOPPLER_CFG                        (DPM_CMD_DPC_START_INDEX + 3U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_MultiObjBeamFormingCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_MULTI_OBJ_BEAM_FORM_CFG                 (DPM_CMD_DPC_START_INDEX + 4U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_CalibDcRangeSigCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_CALIB_DC_RANGE_SIG_CFG                  (DPM_CMD_DPC_START_INDEX + 5U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_StaticClutterRemovalCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_STATICCLUTTER_REMOVAL_CFG                     (DPM_CMD_DPC_START_INDEX + 6U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE   (DPM_CMD_DPC_START_INDEX + 7U)

/**
 * @brief Command associated with @ref DPU_AoAProc_compRxChannelBiasCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE       (DPM_CMD_DPC_START_INDEX + 8U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_fovRangeCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_FOV_RANGE                               (DPM_CMD_DPC_START_INDEX + 9U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_fovDopplerCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_FOV_DOPPLER                             (DPM_CMD_DPC_START_INDEX + 10U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_fovAoaCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_FOV_AOA                                 (DPM_CMD_DPC_START_INDEX + 11U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_RangeAzimuthHeatMapCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_RANGE_AZIMUTH_HEAT_MAP                  (DPM_CMD_DPC_START_INDEX + 12U)

/**
 * @brief This commands indicates to the DPC that the results DPC provided to the application
 *        through its execute API (which application will access through DPM_execute API)
 *        have been exported/consumed. The purpose of this command is for DPC to
 *        reclaim the memory resources associated with the results. The DPC may
 *        also perform sub-frame switching, and do error-checking to see
 *        if export was later than expected e.g the DPC design may be such that
 *        the previous frame/sub-frame's export notification may need to come
 *        after a new frame/sub-frame (this is the case currently with this
 *        object detection DPC). The DPC will also consider this command as a
 *        signal from the application that all its processing for the current frame/sub-frame
 *        has been done and so if a new frame/sub-frame interrupt (DPC has registered
 *        a frame interrupt with the DPM) comes before the last step in the
 *        processing of this command (which could be sub-frame switching and
 *        preparing for next sub-frame/frame), then the DPC will signal an assert
 *        to the application from its frame interrupt. The expected sequence is
 *        the following:
 *
 *        1. App consumes the process result of the DPC (e.g sending output on UART).
 *        2. App performs any dynamic configuration command processing by issuing DPC's
 *           IOCTL APIs for the next frame/sub-frame.
 *        3. App issues this result-exported IOCTL.
 *        4. DPC does its processing related to this IOCTL in the following sequence:
 *            a. May do error checking and preparing for next sub-frame/frame.
 *            b. Do book-keeping related to marking this as end of sub-frame/frame processing
 *               by the app. The DPC's registered frame start interrupt performs
 *               check on this information to see if next frame/sub-frame came before
 *               this end of processing in which case it will issue an assert to app.
 *
 *        An informational structure @ref DPC_ObjectDetection_ExecuteResultExportedInfo_t
 *        is associated with this command.
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED                     (DPM_CMD_DPC_START_INDEX + 13U)

/**
 * @brief This command is for non real-time (without RF) testing. When issued, it will simulate
 *        the trigger of frame start. No configuration structure is associated with this command.
 *        Must be issued between start and stop of DPC.
 */
#define DPC_OBJDET_IOCTL__TRIGGER_FRAME                                   (DPM_CMD_DPC_START_INDEX + 14U)
/**
@}
*/

/** @addtogroup DPC_OBJECTDETECTION_ERROR_CODE
 *  Base error code for the cfarcaProc DPU is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument general (such as NULL argument pointer)
 */
#define DPC_OBJECTDETECTION_EINVAL                  (DP_ERRNO_OBJECTDETECTION_BASE-1)

/**
 * @brief   Error Code: Invalid argSize in DPM_InitCfg provided to @ref DPC_ObjectDetection_init,
 *          does not match the expected size of @ref DPC_ObjectDetection_InitParams_t
 */
#define DPC_OBJECTDETECTION_EINVAL__INIT_CFG_ARGSIZE   (DP_ERRNO_OBJECTDETECTION_BASE-2)

/**
 * @brief   Error Code: Invalid argument in configuration @ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t
 */
#define DPC_OBJECTDETECTION_EINVAL__MEASURE_RX_CHANNEL_BIAS_CFG   (DP_ERRNO_OBJECTDETECTION_BASE-3)

/**
 * @brief   Error Code: Invalid argument bad command argument in DPM_ProcChainIoctlFxn for
 *                      Object detection DPC.
 */
#define DPC_OBJECTDETECTION_EINVAL__COMMAND         (DP_ERRNO_OBJECTDETECTION_BASE-4)

/**
 * @brief   Error Code: Out of general heap memory
 */
#define DPC_OBJECTDETECTION_ENOMEM                  (DP_ERRNO_OBJECTDETECTION_BASE-10)

/**
 * @brief   Error Code: Out of L3 RAM during radar cube allocation.
 */
#define DPC_OBJECTDETECTION_ENOMEM__L3_RAM_RADAR_CUBE            (DP_ERRNO_OBJECTDETECTION_BASE-11)

/**
 * @brief   Error Code: Out of L3 RAM during detection matrix allocation.
 */
#define DPC_OBJECTDETECTION_ENOMEM__L3_RAM_DET_MATRIX            (DP_ERRNO_OBJECTDETECTION_BASE-12)

/**
 * @brief   Error Code: Out of HWA Window RAM
 */
#define DPC_OBJECTDETECTION_ENOMEM_HWA_WINDOW_RAM            (DP_ERRNO_OBJECTDETECTION_BASE-13)

/**
 * @brief   Error Code: Out of Core Local RAM for generating window coefficients
 *          for HWA when doing range DPU Config.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW    (DP_ERRNO_OBJECTDETECTION_BASE-14)

/**
 * @brief   Error Code: Out of Core Local RAM for generating window coefficients
 *          for HWA when doing doppler DPU Config.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_HWA_WINDOW    (DP_ERRNO_OBJECTDETECTION_BASE-15)

/**
 * @brief   Error Code: When doing CFAR configuration, Core Local RAM allocation for cfarDopplerDetOutBitMask
 *          failed.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_CFARCA_DOPPLER_DET_OUT_BIT_MASK     (DP_ERRNO_OBJECTDETECTION_BASE-16)

/**
 * @brief   Error Code: When doing Static clutter configuration, Core Local RAM allocation for
 *          its scratch buffer failed.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_STATIC_CLUTTER_SCRATCH     (DP_ERRNO_OBJECTDETECTION_BASE-17)

/**
 * @brief   Error Code: Out of Core Local RAM allocation for cfarRngDopSnrList (DPIF_CFARDetList).
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_CFARCA_OUT_DET_LIST    (DP_ERRNO_OBJECTDETECTION_BASE-18)

/**
 * @brief   Error Code: Out of Core Local RAM allocation for AoA's detObjOut.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_OUT    (DP_ERRNO_OBJECTDETECTION_BASE-19)

/**
 * @brief   Error Code: Out of Core Local RAM allocation for AoA's detObjOutSideInfo.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_OUT_SIDE_INFO    (DP_ERRNO_OBJECTDETECTION_BASE-20)

/**
 * @brief   Error Code: Out of Core Local RAM allocation for AoA's detObj2dAzimIdx.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_2_AZIM_IDX    (DP_ERRNO_OBJECTDETECTION_BASE-21)

/**
 * @brief   Error Code: Out of Core Local RAM allocation for AoA's detObjElevationAngle.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_DET_OBJ_ELEVATION_ANGLE    (DP_ERRNO_OBJECTDETECTION_BASE-22)

/**
 * @brief   Error Code: Out of Core Local RAM allocation for AoA's azimuthStaticHeatMap.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_AOA_AZIMUTH_STATIC_HEAT_MAP    (DP_ERRNO_OBJECTDETECTION_BASE-23)

/**
 * @brief   Error Code: Pre-start config was received before pre-start common config.
 */
#define DPC_OBJECTDETECTION_PRE_START_CONFIG_BEFORE_PRE_START_COMMON_CONFIG  (DP_ERRNO_OBJECTDETECTION_BASE-30)

/**
 * @brief   Error Code: Internal error
 */
#define DPC_OBJECTDETECTION_EINTERNAL               (DP_ERRNO_OBJECTDETECTION_BASE-40)

/**
 * @brief   Error Code: Not implemented
 */
#define DPC_OBJECTDETECTION_ENOTIMPL                (DP_ERRNO_OBJECTDETECTION_BASE-50)

/**
@}
*/

/** @addtogroup DPC_OBJDET__GLOBAL
 @{ */

/*! Application developers: Use this configuration to load the Object Detection DPC
 *  within the DPM. */
extern DPM_ProcChainCfg  gDPC_ObjectDetectionCfg;

/**
@}
*/

#endif /* objectdetection.h */
