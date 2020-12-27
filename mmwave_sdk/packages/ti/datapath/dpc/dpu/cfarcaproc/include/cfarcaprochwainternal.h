/**
 *   @file  cfarcaprochwainternal.h
 *
 *   @brief
 *      Implements internal data structure for CFAR Processing with HWA.
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

/** @mainpage HWA-CFAR DPU
 *
 * HWA-CFAR DPU
 *
 * @section intro_section Introduction
 * This DPU implements CFAR object detection from the Range/Doppler detection
 * matrix using hardware accelerator (HWA).
 * CFAR detection is performed in both Range and Doppler domains. The DPU exports
 * the list of objects that are detected both in range and in Doppler domains.
 * A high level data flow is illustrated in figure below.
 *
 * @image html hwa_cfar_top_level.png "HWA-CFAR high level data flow"
 *
 * The following figure depicts the DPU implementation:
 *
 * @image html hwa_cfar.png "HWA-CFAR implementation"
 *
 * The input is detection matrix
 * as per the format @ref DPIF_DetMatrix @ref DPIF_DETMATRIX_FORMAT_1, the elements
 * of this matrix are sum  of log2 magnitudes across received antennas of the 2D FFT.
 * This matrix must be no bigger than what can fit in M0+M1 memory (error checking
 * is done during @ref DPU_CFARCAProcHWA_config and violation will generate error code
 * @ref DPU_CFARCAPROCHWA_ENOMEM__DET_MATRIX_EXCEEDS_HWA_INP_MEM)
 *
 * The detection process is initiated by triggering input EDMA (@ref DPU_CFARCAProcHWA_HW_Resources::edmaHwaIn)
 * that copies detection matrix
 * from L3 memory to internal HWA memory (@ref DPU_CFARCAProcHWA_HW_Resources::hwaMemInp, typically
 * (M0+M1)). Signature EDMA (@ref DPU_CFARCAProcHWA_HW_Resources::edmaHwaInSignature)
 * then triggers HWA CFAR which is executed
 * with one param set that performs CFAR in Doppler domain.
 * The results are stored in HWA memory
 * (@ref DPU_CFARCAProcHWA_HW_Resources::hwaMemOutDoppler, typically M2) as a list with each element represented
 * as @ref DPU_CFARCAProcHWA_CfarDetOutput_t.
 * Upon completion, the CPU triggers the HWA CFAR in range domain with the results
 * directed to HWA memory (@ref DPU_CFARCAProcHWA_HW_Resources::hwaMemOutRange, typically in M3)
 * and then in parallel calculates
 * (@ref CFARHWA_convHwaCfarDetListToDetMask) the bit mask of detected points
 * and puts it in to the @ref DPU_CFARCAProcHWA_HW_Resources::cfarDopplerDetOutBitMask
 * that is allocated by the application (typically in core local memory).
 * The bit position in the array corresponding to element x[rangeIdx][dopplerIdx]
 * is calculated as shown in the figure above (expressions "word = , bit = ").
 * \n
 * After the HWA CFAR detection is completed, the CPU (@ref CFARHWA_cfarRange_AND_cfarDoppler)
 * takes detected objects from the range CFAR detection list (M3 memory) and
 * populates in the output list (@ref DPIF_CFARDetList_t)
 * if the following conditions are satisfied:
 * 1. The object is also detected in the Doppler domain (i.e. if the corresponding
 *    bit is set in the Doppler detection bit mask).
 * 2. The range and doppler indices are within those determined from the
 *    FOV configuration given by the application when issuing
 *    @ref DPU_CFARCAProcHWA_config (@ref DPU_CFARCAProcHWA_DynamicConfig::fovRange,
 *    @ref DPU_CFARCAProcHWA_DynamicConfig::fovDoppler) or when issuing
 *    @ref DPU_CFARCAProcHWA_control (@ref DPU_CFARCAProcHWA_Cmd_FovRangeCfg,
 *    @ref DPU_CFARCAProcHWA_Cmd_FovDopplerCfg).
 *
 * In this output list, side information of noise (from the range CFAR HWA processing)
 * and SNR is also populated for each of the generated points in this list. The
 * SNR is calculated using the noise (from range CFAR HWA) and the detection matrix
 * value corresponding to the detected range,doppler position.
 *
 * The HWA CFAR function, can also perform peak grouping functionality. It (enable/disable)
 * is configured from the application when issuing @ref DPU_CFARCAProcHWA_config (@ref DPU_CFARCAProcHWA_DynamicConfig::cfarCfgRange,
 * @ref DPU_CFARCAProcHWA_DynamicConfig::cfarCfgDoppler) or when issuing @ref DPU_CFARCAProcHWA_control (@ref DPU_CFARCAProcHWA_Cmd_CfarRangeCfg,
 * @ref DPU_CFARCAProcHWA_Cmd_CfarDopplerCfg). These configurations also allow CFAR parameters
 * like threshold scale and window/guard lengths to be set. The CFAR detection processing in
 * the Doppler domain can be bypassed by setting doppler direction's detection threshold to zero.
 * Note that in this case the peak grouping in the Doppler domain is unavailable.
 *
 * The HWA-CFAR timing diagram is illustrated in figure below.
 *
 * @image html hwa_cfar_timing.png "HWA-CFAR timing"
 *
 * @section CFARCAProcHWA APIs
 *
 * - @ref DPU_CFARCAProcHWA_init DPU initialization initialization function.\n
 *
 * - @ref DPU_CFARCAProcHWA_config DPU configuration function. The configuration can only be done after
 * the DPU has been initialized using @ref DPU_CFARCAProcHWA_init.\n
 *
 * - @ref DPU_CFARCAProcHWA_process DPU processing function. This will execute the CFAR detection
 * algorithm using HWA. This processing can only be done after the DPU has been configured through
 * @ref DPU_CFARCAProcHWA_config. If the parameters used by this DPU do not
 * change from one frame to the next, @ref DPU_CFARCAProcHWA_config can be called only once for the first
 * frame and for every frame @ref DPU_CFARCAProcHWA_process can be executed without the need of reconfiguring the DPU.
 *
 * - @ref DPU_CFARCAProcHWA_control DPU control function. It processes the following messages received from DPC:
 *      - @ref DPU_CFARCAProcHWA_Cmd_CfarRangeCfg,
 *      - @ref DPU_CFARCAProcHWA_Cmd_CfarDopplerCfg,
 *      - @ref DPU_CFARCAProcHWA_Cmd_FovRangeCfg,
 *      - @ref DPU_CFARCAProcHWA_Cmd_FovDopplerCfg
 *
 *  Full API details can be seen at @ref CFARCA_PROC_DPU_EXTERNAL
 *
 */

/** @defgroup CFARCA_PROC_DPU_INTERNAL       cfarcaProc DPU Internal
 */

/**
@defgroup DPU_CFARCAPROC_INTERNAL_FUNCTION            cfarcaProc DPU Internal Functions
@ingroup CFARCA_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup DPU_CFARCAPROC_INTERNAL_DATA_STRUCTURE      cfarcaProc DPU Internal Data Structures
@ingroup CFARCA_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal data structures which are used internally
*   by the cfarcaProc DPU module.
*/
/**
@defgroup DPU_CFARCAPROC_INTERNAL_DEFINITION          cfarcaProc DPU Internal Definitions
@ingroup CFARCA_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal definitions which are used internally
*   by the cfarcaProc DPU.
*/

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DPU_CFAR_HWA_H
#define DPU_CFAR_HWA_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* mmWave SDK Include Files */
#include <ti/utils/cycleprofiler/cycle_profiler.h>
#include <ti/datapath/dpc/dpu/cfarcaproc/cfarcaprochwa.h>
#include <ti/utils/mathutils/mathutils.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief    Field of view indices as integers. For range domain values are [0
 *
 *  \ingroup DPU_CFARCAPROC_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_CFARCAProc_FovIdxCfg_t
{
    /*! Minimum Range or Doppler Bin index */
    int16_t minIdx;
    /*! Maximum Range or Doppler Bin index */
    int16_t maxIdx;
} DPU_CFARCAProc_FovIdxCfg;

/**
 * @brief
 *  HWA CFAR configuration
 *
 * @details
 *  The structure is used to hold the HWA configuration used for CFAR
 *
 *  \ingroup DPU_CFARCAPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct CFARHwaObj_t
{
    /*! @brief      number of detected objection from HWA */
    uint16_t                numHwaCfarObjs;

    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;

    /*! @brief Hardware resources */
    DPU_CFARCAProcHWA_HW_Resources res;

    /*! @brief     HWA Processing Done semaphore Handle */
    SemaphoreP_Handle    hwaDone_semaHandle;

    /*! @brief      CFAR configuration in range direction */
    DPU_CFARCAProc_CfarCfg   cfarCfgRange;

    /*! @brief      CFAR configuration in Doppler direction */
    DPU_CFARCAProc_CfarCfg   cfarCfgDoppler;

    /*! @brief      Field of view configuration in range domain */
    DPU_CFARCAProc_FovIdxCfg fovRange;

    /*! @brief      Field of view configuration in Doppler domain */
    DPU_CFARCAProc_FovIdxCfg fovDoppler;

    DPU_CFARCAProcHWA_StaticConfig staticCfg;

    /*! @brief total number of calls of DPU processing */
    uint32_t            numProcess;

}CFARHwaObj;



#ifdef __cplusplus
}
#endif

#endif /* DPU_CFAR_HWA_H */
