/**
 *   @file  data_path.h
 *
 *   @brief
 *      This is the data path processing header.
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
#ifndef DATA_PATH_H
#define DATA_PATH_H

#include <ti/sysbios/knl/Semaphore.h>

#include <ti/common/sys_common.h>
#include <ti/common/mmwave_error.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/hwa/hwa.h>
#include <ti/demo/xwr68xx/mmw/mmw_config.h>
#include <ti/demo/utils/mmwdemo_adcconfig.h>
#include <ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h>
#include <ti/demo/xwr68xx/mmw/mmw_output.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  @brief Sub-frame specific Dynamic config storage (CLI parses input commands
 *  and stores in this structure) and associated book-keeping required to process
 *  the commands after inter-frame processing.
 */
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

/**
 * @brief
 *  Same functionality as @ref MmwDemo_DPC_ObjDet_DynCfg but for common configuration
 */
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

/**
 * @brief Data path Object information.
 */
typedef struct MmwDemo_DataPathObj_t
{
    /*! @brief   ADCBUF handle. */
    ADCBuf_Handle adcbufHandle;

    /*! @brief   Handle of the EDMA driver. */
    EDMA_Handle edmaHandle[EDMA_NUM_CC];

    /*! @brief   EDMA error Information when there are errors like missing events */
    EDMA_errorInfo_t  EDMA_errorInfo;

    /*! @brief EDMA transfer controller error information. */
    EDMA_transferControllerErrorInfo_t EDMA_transferControllerErrorInfo;

    /*! @brief Handle to hardware accelerator driver. */
    HWA_Handle  hwaHandle;

    /*! @brief dpm Handle */
    DPM_Handle objDetDpmHandle;

    /*! @brief Object Detection DPC common configuration */
    MmwDemo_DPC_ObjDet_CommonCfg objDetCommonCfg;
} MmwDemo_DataPathObj;

void MmwDemo_hwaInit(MmwDemo_DataPathObj *obj);
void MmwDemo_edmaInit(MmwDemo_DataPathObj *obj);
void MmwDemo_hwaOpen(MmwDemo_DataPathObj *obj, SOC_Handle socHandle);
void MmwDemo_edmaOpen(MmwDemo_DataPathObj *obj);
void MmwDemo_EDMA_errorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo);
void MmwDemo_EDMA_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo);

void MmwDemo_dataPathObjInit(MmwDemo_DataPathObj *obj);
void MmwDemo_edmaClose(MmwDemo_DataPathObj *obj);
void MmwDemo_hwaClose(MmwDemo_DataPathObj *obj);

#ifdef __cplusplus
}
#endif

#endif /* DATA_PATH_H */

