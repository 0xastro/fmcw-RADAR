/**
 *   @file  objectdetectioninternal.h
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

/** @mainpage Object Detection Data-path Processing Chain (DPC)
 *
 *  @section objdetintro Introduction
 *
 *  The Object detection DPC provides the functionality of processing ADC samples
 *  to detected objects during the frame acquisition and inter-frame processing
 *  periods. It can be used by the application by registering with the DPM
 *  framework and invoked using DPM APIs. The external interface of Object detection
 *  DPC can be seen at @ref DPC_OBJDET_EXTERNAL
 *
 *  @section objdethwa_datapath Data Path
 *   @image html object_detection_datapath.png "Object Detection Data Path Processing Chain"
 *   \n
 *   \n
 *   The data path processing consists of:
 *   - Processing during the chirps as seen in the timing diagram:
 *     - This consists of 1D (range) FFT processing that takes input from multiple
 *     receive antennae from the ADC buffer for every chirp (corresponding to the
 *     chirping pattern on the transmit antennae)
 *     and performs FFT on it and generates output into the L3 RAM in the format
 *     defined by @ref DPIF_RADARCUBE_FORMAT_1. For more details, see the doxygen
 *     documentation of range processing DPU (Data Path Unit) located at:
 *     @verbatim
       ti/datapath/dpu/rangeproc/docs/doxygen/html/index.html
       @endverbatim
 *   - Processing during the time between the end of chirps until the beginning of the
 *     next chirping period, shown as "Inter frame Period" in the timing diagram.
 *     This processing consists of:
 *     - Static clutter removal when enabled, removes all static objects from the scene.
 *       For more details, see:
 *       @verbatim
          ti/datapath/dpc/dpu/staticclutterproc/docs/doxygen/html/index.html
         @endverbatim
 *     - 2D (velocity) FFT processing that takes input from 1D output in L3 RAM and performs
 *       FFT to give a (range,velocity) matrix in the L3 RAM. For more details, see:
 *       @verbatim
          ti/datapath/dpc/dpu/dopplerproc/docs/doxygen/html/index.html
         @endverbatim
 *     - CFAR processing and peak grouping on detection matrix output of doppler processing.
 *       For more details, see:
 *       @verbatim
          ti/datapath/dpc/dpu/cfarcaproc/docs/doxygen/html/index.html
         @endverbatim
 *     - Angle (Azimuth, Elevation) of Arrival processing to produce a final list of
 *       detected objects with position coordinates (x,y,z) and velocity.
 *       For more details, see:
 *       @verbatim
          ti/datapath/dpc/dpu/aoaproc/docs/doxygen/html/index.html
         @endverbatim
 *
 *  @section appdpcFlow Application-DPC Execution Flow
 *   Following diagram shows the application-DPC execution Flow.
 *
 *  @image html dpc_flow.png "Application-DPC Execution Flow"
 *
 *   The flow above shows the sequencing of initialization, configuration, execution and
 *   dynamic control operations of the DPC and some level of detail of
 *   what happens under these operations.
 *   Most of the hardware resource (e.g HWA, EDMA related) configuration
 *   for the DPUs that is issued by the DPC as part of processing
 *   @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG commands is provided by the
 *   application at build time using a resource file (DPC sources are built
 *   as part of building the application, there is no separate DPC library object).
 *   This file is passed as a compiler command line define
 *   @verbatim --define=APP_RESOURCE_FILE="fileName" @endverbatim The "fileName"
 *   above includes the path as if to include the file when building the the DPC sources
 *   as part of building the application, and any DPC source that needs to refer
 *   to this file (currently objectdetection.c) has the following code
 *    @verbatim #include APP_RESOURCE_FILE @endverbatim
 *   One of the demos that uses this DPC is located at ti/demo/xwr68xx/mmw. The
 *   resource file in this demo is mmw_res.h, this file shows all the definitions
 *   that are needed by the DPC from the application. This file is provided
 *   on compiler command line when building as follows:
 *   @verbatim --define=APP_RESOURCE_FILE="<ti/demo/xwr68xx/mmw/mmw_res.h>" @endverbatim
 *
 *   The partitioning of L3 and Core Local RAM (hitherto referred in short as LRAM)
 *   memories provided by the application (in @ref DPC_ObjectDetection_InitParams_t::L3ramCfg
 *   and @ref DPC_ObjectDetection_InitParams_t::CoreLocalRamCfg) during
 *   @ref DPC_ObjectDetection_init (invoked by application through @ref DPM_init)
 *   happens during the processing of @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG command
 *   and is shown in the following figure. The allocation from application system heap
 *   (typically in LRAM) using the MemoryP_osal API is done during @ref DPC_ObjectDetection_init
 *   (object instances of DPC and DPUs for all sub-frames) and during the
 *   processing of @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG command (range DPUs
 *   dc antenna coupling signature buffer that is unique for each sub-frame)
 *   is also shown in the figure.
 *
 *   @image html memory_allocation.png "Data memory allocation"
 *
 *   In the above picture, the LRAM shows allocation
 *   of the "cfarRngDopSnrList" (@ref DPIF_CFARDetList_t) outside of scratch usage
 *   as this is shared buffer between CFAR and AoA in the processing flow and
 *   therefore needs to persist within the sub-frame until AoA is executed at the
 *   end of the processing chain. The buffers labeled "windowBuffer" in the picture
 *   for range and doppler DPUs are consumed during the DPU config processing
 *   into the HWA config RAM (sequentially) and so could be generated in
 *   local stack memory but this would require large stack
 *   depth, so the LRAM is used instead. The AoA interface buffers are allocated
 *   from the LRAM from the end of windowBuffer of
 *   the doppler DPU (that is configured prior to the AoA configuration) because
 *   AoA needs to be provided the same window as that of doppler as it uses
 *   it to recompute the 2D doppler FFT. This sequencing and arrangement in memory
 *   prevents window recomputation for AoA, although AoA will (redundant to doppler DPU)
 *   recopy the window to the HWA config RAM at the
 *   same window RAM offset as that of the doppler DPU.
 *   The AoA DPU API has been designed to require 2D-FFT window configuration
 *   (i.e window configuration is not optional) because it may be used in
 *   contexts (unit test, other DPC flavors) where doppler processing may not exist.
 *   The AoA interface buffers are consumed by the application at the end of the
 *   DPC execute API.
 *
 *   @subsection reconfig DPU reconfiguration related to data path processing within and across sub-frames
 *   When number of transmit antennas is 3 (versus less than 3), the HWA resources (param sets)
 *   between CFAR and AoA are overlapped because total HWA param sets needed for all the DPUs
 *   exceeds the total available in the HWA (16). In this case, within each sub-frame's processing
 *   (in the DPC's execute API implementation), the CFAR and AoA's
 *   HWA configuration on overlapping param sets needs to be redone before invoking
 *   their process APIs. Currently there are no special partial configuration APIs
 *   for DPUs to just reconfigure the desired overlapping param sets, the
 *   full DPU configuration (xxx_config) must be issued (this includes for example FFT
 *   window configuration in relevant DPUs). Additionally, such a case of
 *   reconfiguration is also required when switching sub-frames because all HWA param
 *   sets and EDMA resources are overlapped across sub-frames. Note that the DPU's xxx_config API
 *   is a full configuration API beyond the HWA and EDMA resources configuration (e.g static
 *   and dynamic configuration) so restricting to the full configuration
 *   would imply that no sub-frame specific
 *   DPU instantiation is necessary. However, the code illustrates separate instances of
 *   DPUs for each sub-frame to demonstrate generality of the sub-frame solution,
 *   in the case where there may be specialized (partial) configuration APIs in an
 *   optimized implementation (that only configured the overlapped EDMA/HWA resources).
 *   The limiting to full configuration also means that all the code required to build the
 *   configuration structures of the DPUs during the pre-start config time either
 *   has to be repeated or alternatively, the configurations that were created
 *   during the pre-start config processing be saved and reused later.
 *   The latter path has been taken, all DPU configuration that is built
 *   during the pre-start processing is stored in separate storage, this can be
 *   located at @ref SubFrameObj_t::dpuCfg. However, parts of this reconfiguration that
 *   cannot be captured in this storage need to be repeated, namely, window generation
 *   (@ref DPC_ObjDet_GenDopplerWindow, @ref DPC_ObjDet_GenRangeWindow) and rx phase compensation
 *   (@ref DPC_ObjDet_GetRxChPhaseComp).
 *
 *   The DPU top-level dynamic configuration structure contains
 *   pointers to the individual configurations (e.g see @ref DPU_AoAProc_DynamicConfig_t)
 *   so DPC stores the dynamic configuration in pre-start config (@ref DPC_ObjectDetection_DynCfg_t)
 *   in permanent storage (@ref SubFrameObj_t::dynCfg) and DPUs are passed from this storage area
 *   so that their pointers point to this permanent storage during reconfiguration.
 *   The static configuration during pre-start config (@ref DPC_ObjectDetection_StaticCfg_t)
 *   is also stored (@ref SubFrameObj_t::staticCfg) for convenient referencing when doing
 *   range bias and rx phase measurement (@ref DPC_ObjDet_rangeBiasRxChPhaseMeasure)
 *   and the above referred rx ch phase computation during reconfiguration (@ref DPC_ObjDet_GetRxChPhaseComp).
 *   Saving this entire static configuration is probably more than necessary (versus
 *   storing only the parameters that need to be referred in the above mentioned functions)
 *   but it keeps implementation simpler/cleaner and may be useful in debugging.
 *
 *  @section objdethwa_calibration Range Bias and Rx Channel Gain/Offset Measurement and Compensation
 *
 *    Because of imperfections in antenna layouts on the board, RF delays in SOC, etc,
 *    there is need to calibrate the sensor to compensate for bias in the range estimation and
 *    receive channel gain and phase imperfections. The DPC provides the ability to
 *    do the measurement and compensation.
 *
 *    @anchor Figure_calibration
 *    @image html calibration.png "Range Bias and Rx Channel Gain/Offset Measurement and Compensation"
 *
 *   @subsection measurement Measurement
 *     If the measurement (@ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t::enabled) is enabled in pre-start
 *     common config (@ref DPC_ObjectDetection_PreStartCommonCfg_t::measureRxChannelBiasCfg)
 *     or dynamic config (@ref DPC_OBJDET_IOCTL__DYNAMIC_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE),
 *     it is assumed that a strong target like a corner reflector is kept at boresight at
 *     a distance of X = @ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t::targetDistance.
 *     Peak search is done after the 2D FFT in the 0th Doppler
 *     of the detection matrix within distance D = @ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t::searchWinSize
 *     around X i.e peak is searched between -D/2 + X and X + D/2.
 *     The peak position is then used to compute the
 *     square root of the sum of the magnitude squares of the virtual antennas
 *     (taken from 1D-FFT Radar Cube matrix) for the
 *     peak and its two nearest neighbors. These three magnitudes and their positions
 *     are used to do parabolic interpolation to find the more accurate peak location from
 *     which the range bias is estimated as this interpolated peak position minus
 *     the configured target distance X.
 *     The rx channel phase and gain estimation is done by finding the minimum
 *     of the magnitude squared of the virtual antennas and this minimum is used
 *     to scale the individual antennas so that the magnitude of the coefficients is
 *     always less than or equal to 1. Rx channel compensation coefficients are
 *     calculated according to following equations. The radar cube matrix is
 *     laid out as four dimensional matrix defined as @ref DPIF_RADARCUBE_FORMAT_1, i.e. as
 *       \f[
 *           \mbox{RadarCube}[\mbox{numTxPatterns}][\mbox{numDopplerChirps}][\mbox{numRxAnt}][\mbox{numRangeBins}]
 *       \f]
 *
 *     The received symbols are extracted from the 1D-FFT radar cube from the first chirp
 *     at the range index \f$i_{Max}\f$corresponding to the maximum in the searched range as
 *
 *       \f[
 *           x(i_{Tx},i_{Rx}) = \mbox{RadarCube}(i_{Tx},0,i_{Rx},i_{Max}), \;\;\;\; i_{Tx}=0,...,N_{TxAnt}-1, \;\;\;\; i_{Rx}=0,...,N_{RxAnt}-1
 *       \f]
 *
 *     The coefficients are calculated as
 *
 *       \f[
 *           c(i_{Tx},i_{Rx}) = \frac{x^*(i_{Tx},i_{Rx})}{|x(i_{Tx},i_{Rx})|^2}\;x_{Min}, \;\;\;\; i_{Tx}=0,...,N_{TxAnt}-1, \;\;\;\; i_{Rx}=0,...,N_{RxAnt}-1
 *       \f]
 *
 *     where
 *
 *       \f[
 *           x_{Min} = \min{|x(i_{Tx},i_{Rx})|}
 *       \f]
 *
 *     The compensation rx coefficients are in Q15 format.
 *     Refer to the function @ref DPC_ObjDet_rangeBiasRxChPhaseMeasure
 *     which performs the measurements, the measurement
 *     result (@ref DPU_AoAProc_compRxChannelBiasCfg_t) is given out of the DPC
 *     to the application as part of the result structure produced by DPC's execute
 *     API (@ref DPC_ObjectDetection_ExecuteResult_t::compRxChanBiasMeasurement).
 *   @subsection compensation Compensation
 *     When compensation configuration (DPU_AoAProc_compRxChannelBiasCfg_t) is provided as part of
 *     pre-start common config (@ref DPC_ObjectDetection_PreStartCommonCfg_t::compRxChanCfg) or
 *     dynamic config (@ref DPC_OBJDET_IOCTL__DYNAMIC_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE),
 *     as seen in figure @ref Figure_calibration, for each sub-frame the DPC
 *     determines the rx compensation vector (@ref  DPC_ObjDet_GetRxChPhaseComp) to apply
 *     for that sub-frame (the resulting compensation vector is such that
 *     all antennas enabled for that configuration are contiguous in storage)
 *     depending on the antenna configuration for that sub-frame and passes this to the
 *     AoA DPU. During processing, AoA DPU will apply this contiguously stored vector
 *     on the virtual antenna array before performing angle computation.
 *
 *     @note Although the range bias estimation is immune to motion because
 *       the bias estimation is based on searching peak position after 2D FFT,
 *       the rx compensation coefficients are calculated from 1D FFT output,
 *       so this will not be immune to motion in the scene. Hence, during calibration
 *       care must exercised to ensure that there are no moving objects in the search
 *       window range.
 *
 *
 *  @section objdethwa_designNotes Data Path Design Notes
 *
 *    @subsection objdethwa_scaling Scaling
 *      The HWA uses 24-bit fixed point arithmetic for the data path processing.
 *      In order to prevent overflows in the FFT processing, the scaling factors have to be
 *      set appropriately in the HWA configuration. The HWA has up to 10 stages of processing
 *      with ability to scale by 1/2 for each stage.
 *      - 1D processing: If the HWA's FFT scale is set to \f$\frac{1}{2^k}\f$
 *      where \f$k\f$ is the number of stages for which the scaling is enabled, and
 *      input to the FFT were a pure tone at one of the bins, then
 *      the output magnitude of the FFT at that bin will be \f$\frac{N}{2^k}\f$ (\f$N\f$ is the FFT order) times
 *      the input tone amplitude (because tone is complex, this implies that the individual real and
 *      imaginary components will also be amplified by a maximum of this scale).
 *      Because we do a Blackman window before the FFT, the overall scale is about 1/2.4 of the FFT scale.
 *      This means for example for 256 point FFT, the windowing + FFT scale will be \f$\frac{106.7}{2^k}\f$.
 *      For k=2 which is currently how it is in the implementation (no matter the FFT order), this will be 26.7.
 *      Therefore, the ADC output when it is a pure tone should not exceed +/-2^15/26.7 = 1228 for
 *      the I and Q components (even though HWA is internally 24-bit, the FFT output is stored
 *      as 16-bit before 2D processing, hence 2^15).
 *      The XWR68xx EVM when presented with a strong single reflector
 *      reasonably close to it (with Rx dB gain of 30 dB in the chirp profile)
 *      shows ADC samples to be a max of about 2000 and while this exceeds
 *      the 1228 maximum, is not a pure tone, the energy of the FFT is seen in other bins
 *      also and the solution still works well and detects the strong object.
 *      - 2D processing: For the 2D FFT, given that the input is the output of 1D FFT that
 *      can amplify its input as mentioned in previous section, it is more appropriate to use
 *      full scale. So currently in the implementation, scaling is enabled for all stages of the FFT
 *      except for the first stage because of Hanning window related scaling by 1/2.
 *
 */

/** @defgroup DPC_OBJDET_INTERNAL       Object Detection DPC (Data Path Chain) Internal
 */
/**
@defgroup DPC_OBJDET_IOCTL__INTERNAL_DATA_STRUCTURES      Object Detection DPC Internal Data Structures
@ingroup DPC_OBJDET_INTERNAL
@brief
*   This section has a list of all the internal data structures which are a part of the DPC.
*/
/**
@defgroup DPC_OBJDET_IOCTL__INTERNAL_DEFINITIONS      Object Detection DPC Internal Definitions
@ingroup DPC_OBJDET_INTERNAL
@brief
*   This section has a list of all the internal defines which are a part of the DPC.
*/
/**
@defgroup DPC_OBJDET__INTERNAL_FUNCTION             Object Detection DPC Internal Functions
@ingroup DPC_OBJDET_INTERNAL
@brief
*   This section has a list of all the internal function which are a part of the DPC.
*   These are not exposed to the application.
*/

#ifndef DPC_OBJECTDETECTION_INTERNAL_H
#define DPC_OBJECTDETECTION_INTERNAL_H

/* MMWAVE Driver Include Files */
#include <ti/common/mmwave_error.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/dpm/dpm.h>

#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h>
#ifdef __cplusplus
extern "C" {
#endif

 /** @addtogroup DPC_OBJDET_IOCTL__INTERNAL_DATA_STRUCTURES
  @{ */

/**
 * @brief  The structure is used to hold all the dpu configurations that were
 *         constructed at the time of pre-start configuration, so that they
 *         do not need to be reconstructed when issuing for the purpose of
 *         reconfiguration within sub-frame due to overlapping h/w resources
 *         (e.g HWA resources) within sub-frame among dpus or across sub-frames
 *         due to overlapping h/w resources (e.g HWA, L3 and core Local memory).
 */
typedef struct DpuConfigs_t
{
    /*! brief   Range DPU configuration storage */
    DPU_RangeProcHWA_Config      rangeCfg;

    /*! brief   Doppler DPU configuration storage */
    DPU_DopplerProcHWA_Config    dopplerCfg;

    /*! brief   Static Clutter DPU configuration storage */
    DPU_StaticClutterProc_Config staticClutterCfg;

    /*! brief   CFARCA DPU configuration storage */
    DPU_CFARCAProcHWA_Config     cfarCfg;

    /*! brief   AoA DPU configuration storage */
    DPU_AoAProcHWA_Config        aoaCfg;
} DpuConfigs;

/**
 * @brief  The structure is used to hold all the relevant information for
 *         each of the sub-frames for the object detection DPC.
 */
typedef struct SubFrameObj_t
{
    /*! brief   Pointer to hold Range Proc DPU handle */
    DPU_RangeProcHWA_Handle dpuRangeObj;

    /*! brief   Pointer to hold Static Clutter DPU handle */
    DPU_StaticClutterProc_Handle dpuStaticClutterObj;

    /*! brief   Pointer to hold CFAR DPU handle */
    DPU_CFARCAProcHWA_Handle dpuCFARCAObj;

    /*! brief   Pointer to hold Doppler DPU handle */
    DPU_CFARCAProcHWA_Handle dpuDopplerObj;

    /*! brief   Pointer to hold AoA DPU handle */
    DPU_AoAProcHWA_Handle dpuAoAObj;

    /*! brief   Static configuration */
    DPC_ObjectDetection_StaticCfg staticCfg;

    /*! brief   Dynamic configuration */
    DPC_ObjectDetection_DynCfg dynCfg;

    /*! @brief  Log2 of number of doppler bins */
    uint8_t     log2NumDopplerBins;

    /*! @brief DPU configuration storage to use within and when switching
     * sub-frames due to shared resources among DPUs of same sub-frame and
     * sharing across sub-frames */
    DpuConfigs dpuCfg;

    /*! @brief true if HWA param set of AoA is overlapped with that of CFAR */
    bool isAoAHWAparamSetOverlappedWithCFAR;
} SubFrameObj;

/*
 * @brief Memory pool object to manage memory based on @ref DPC_ObjectDetection_MemCfg_t.
 */
typedef struct MemPoolObj_t
{
    /*! @brief Memory configuration */
    DPC_ObjectDetection_MemCfg cfg;

    /*! @brief   Pool running adress.*/
    uintptr_t currAddr;

    /*! @brief   Pool max address. This pool allows setting address to desired
     *           (e.g for rewinding purposes), so having a running maximum
     *           helps in finding max pool usage
     */
    uintptr_t maxCurrAddr;
} MemPoolObj;

/**
 * @brief
 *  Millimeter Object Detection DPC object/instance
 *
 * @details
 *  The structure is used to hold all the relevant information for the
 *  object detection DPC.
 */
typedef struct ObjDetObj_t
{
    /*! @brief DPM Initialization Configuration */
    DPM_InitCfg   dpmInitCfg;

    /*! @brief Handle to the DPM Module */
    DPM_Handle    dpmHandle;

    /*! @brief Handle to the SOC Module: */
    SOC_Handle    socHandle;

    /*! @brief   Per sub-frame object */
    SubFrameObj   subFrameObj[RL_MAX_SUBFRAMES];

    /*! @brief Sub-frame index */
    uint8_t       subFrameIndx;

    /*! @brief   Handle of the EDMA driver. */
    EDMA_Handle   edmaHandle[EDMA_NUM_CC];

    /*! @brief  Used for checking that inter-frame (inter-sub-frame) processing
     *          finished on time */
    int32_t       interSubFrameProcToken;

    /*! @brief L3 ram memory pool object */
    MemPoolObj    L3RamObj;

    /*! @brief Core Local ram memory pool object */
    MemPoolObj    CoreLocalRamObj;

    /*!< @brief  HWA memory bank addresses */
    uint32_t      hwaMemBankAddr[4];

    /*!< @brief  HWA memory single bank size in bytes */
    uint16_t      hwaMemBankSize;

    /*!< @brief  Common configuration storage */
    DPC_ObjectDetection_PreStartCommonCfg commonCfg;

    /*!< @brief Flag to make sure pre start common config is received by the DPC
     *          before any pre-start (sub-frame specific) configs are received. */
    bool     isCommonCfgReceived;

    /*! @brief DPC's execute API result storage */
    DPC_ObjectDetection_ExecuteResult executeResult;

    /*! @brief   Range Bias and rx channel gain/phase compensation output of
     *           measurement to send to application in the result of processing
     *           but not use it in the re-configuration of AoA during process */
    DPU_AoAProc_compRxChannelBiasCfg compRxChanCfgMeasureOut;

    /*! @brief   Stats structure to convey to Application timing and related information. */
    DPC_ObjectDetection_Stats stats;

    /*! @brief   Process call back function configuration */
    DPC_ObjectDetection_ProcessCallBackCfg processCallBackCfg;
} ObjDetObj;

/**
@}
*/
extern void _DPC_Objdet_Assert(DPM_Handle handle, int32_t expression, const char *file, int32_t line);
#define DPC_Objdet_Assert(handle, expression) {                \
              _DPC_Objdet_Assert(handle, expression,           \
                                 __FILE__, __LINE__);          \
               DebugP_assert(expression);                      \
                                      }

#endif /* objectdetectioninternal.h */

