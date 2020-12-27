/**
 *   @file  aoaprochwa_internal.h
 *
 *   @brief
 *      Implements internal data structure for AoA Processing with HWA.
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

/** @mainpage HWA-AOA DPU
 *
 * @section section_HWA-AOA HWA-AOA DPU
 *
 * @subsection section_AOA_Description Description
 * This DPU implements angle of arrival (AoA) estimation using HWA and CPU.
 * It generates information about detected objects, into two lists: 1) list with coordinates
 * defined by @ref DPIF_PointCloudCartesian_t and 2) side information list defined by @ref DPIF_PointCloudSideInfo_t.
 * The DPU receives 1D-FFT Radar Cube matrix calculated by range processing DPU
 * and the list of detected objects (@ref DPIF_CFARDetList_t) which is assumed to
 * be populated by previous CFAR algorithm processing. A high level data flow is
 * illustrated in figure below.
 *
 * @image html hwa_aoa_top_level.png "HWA-AOA high level data flow"
 *
 * The following figure depicts detailed DPU implementation. DPU processes data
 * from the input CFAR detection list in chunks to overcome HWA memory limitation.
 * In the loop per chunk, it reads group of detected objects from the input list,
 * performs the processing, and appends the result to the output lists.
 * The processing per chunk of detected objects consists of three phases:
 * 1. Calculation of the array of 2D-FFT Rx virtual antenna symbols as input to Azimuth FFT computation.
 *    Processing is done in the loop for all objects within chunk. Each loop consists of following steps:
 *    1. Input EDMA pages from radarCube memory to HWA M1 memory the data corresponding to the range index
 *       of the detected object. Note the input size limiting condition:
 *       \f$N_{TxAnt}*N_{RxAnt}*N_{DopplerChirps}*sizeof(cmplx16ImRe\_t) \leq 16\;KB\f$
 *    2. HWA calculates 2D FFT from M1 memory and produces output in M2+M3 memory.
 *       Note the output size limiting condition:
 *       \f$N_{TxAnt}*N_{RxAnt}*N_{DopplerBins}*sizeof(cmplx16ImRe\_t) \leq 32\;KB\f$
 *    3. Output EDMA picks the data of the Doppler bin corresponding to the detected object's
 *       Doppler index from the above FFT output and copies into core local memory.
 *    4. CPU performs Rx channel phase/gain compensation on above data in core local memory and produces
 *       results in core local memory.
 *    5. CPU performs Doppler compensation and writes result to the HWA M0 memory,
 *       where it holds symbols of both azimuth and elevation Tx antennas.
 *
 *    Thus, at the end of this processing, the M0 will hold the azimuth
 *    and elevation 2D FFT output data of all detected objects.
 * 2. Azimuth FFT calculation using HWA for all detected objects within chunk. In the loop
 *    for each detected object, the HWA calculates:
 *    1. Azimuth FFT (complex output) on elevation Tx antennas in M0 to M3
 *       only when 3 Tx antennas are configured.
 *    2. Azimuth FFT on azimuth Tx antennas in M0 and then log2 magnitude of this FFT to M2.
 *    3. Azimuth FFT (complex output) on azimuth Tx antennas in M0 to M1 only when
 *       3 Tx antennas are configured.
 *
 *    Note that the maximum number of detected objects per chunk is limited due to HWA memory size:
 *    \f$N_{objPerChunk} \leq 16\;KB / (sizeof(cmplx16ImRe\_t) * N_{AngleFFT})\f$ when 3Tx antennas are configured, and
 *    \f$N_{objPerChunk} \leq 16\;KB / (sizeof(uint16\_t) * N_{AngleFFT})\f$ when 2Tx antennas are configured.
 *
 * 3. Final azimuth and elevation estimation and x/y/z calculation performed by CPU.
 *    The calculated data of detected objects is appended to the output lists.
 *
 * @image html hwa_aoa.png "HWA-AOA implementation (for 3 Tx antennas)"
 *
 * In addition to AoA estimation, this DPU also generates data for computing
 * azimuth heat-map (e.g for visualization purposes), this data is 2D FFT of the
 * phase compensated Doppler bin 0 (static objects) output of each of the range bins.
 * The following figure depicts data generation for azimuth heat-map visualization.
 *
 * @image html hwa_aoa_azimuth_heatmap.png "HWA-AOA implementation"
 *
 *  @subsection Sect_dopplerComp Doppler Compensation
 *    Doppler compensation function  (@ref aoaHwa_dopplerCompensation) is done by CPU after
 *    Rx channel phase/gain compensation. It performs compensation for the Doppler phase shift on
 *    the symbols corresponding to the virtual Rx antennas. In case of 2Tx MIMO scheme, the second
 *    set of Rx symbols is rotated by half of the estimated Doppler phase shift between subsequent
 *    chirps corresponding to the same Tx antenna. In case of 3Tx MIMO elevation scheme, the second
 *    set of Rx symbols is rotated by 1/3 of the estimated Doppler phase shift, while the third set
 *    of Rx symbols is rotated by 2/3 of the estimated Doppler phase shift. Refer to the pictures below.
 *    @anchor Figure_doppler
 *    @image html angle_doppler_compensation.png "Figure_doppler: Doppler Compensation"
 *
 *  @subsection dataXYZ Data Path - Direction of Arrival Estimation (x,y,z)
 *    This processing is done on CPU in the function @ref AoAProcHWA_angleEstimationAzimElev.
 *    The actual computation per detected object is performed by @ref AoAProcHWA_XYZestimation.
 *    \anchor Figure_geometry
 *    @image html coordinate_geometry.png "Figure A: Coordinate Geometry"
 *    \n
 *    \anchor Figure_wz
 *    @image html coordinate_geometry_wz.png "Figure wz"
 *    \n
 *    \anchor Figures_wx
 *    @image html coordinate_geometry_wx.png "Figures wx"
 *    \ref Figure_geometry shows orientation of x,y,z axes with respect to the sensor/antenna positions. The objective is to estimate
 *    the (x,y,z) coordinates of each detected object.
 *    \f$w_x\f$ is the phase difference between consecutive receive azimuth antennas of the 2D FFT and \f$w_z\f$ is the phase difference between
 *    azimuth and corresponding elevation antenna above the azimuth antenna. The phases for each antenna are shown in the \ref Figure_doppler.
 *    \ref Figure_wz shows that the distance AB which represents the relative distance between wavefronts intersecting consecutive elevation
 *    antennas is \f$AB = \frac{\lambda}{2} \sin (\phi)\f$. Therefore \f$W_z = \frac{2\pi}{\lambda} \cdot AB\f$, therefore \f$W_z = \pi \sin (\phi)\f$.
 *    Note that the phase of the lower antenna is advanced compared to the upper
 *    antenna which is why picture X shows -Wz term in the upper antenna.
 *    \ref Figures_wx show that distance CD which represents the relative distance between wavefronts intersecting consecutive azimuth
 *    antennas is \f$CD = \frac{\lambda}{2} \sin (\theta) \cos (\phi)\f$ Therefore \f$w_x =  \frac{2\pi}{\lambda} \cdot CD\f$,
 *    therefore \f$w_x = \pi \sin (\theta) \cos (\phi)\f$.
 *    For a single obstacle, the signal at the 8 azimuth antennas will be (\f$A_1\f$ and \f$\psi\f$ are the arbitrary starting
 *    amplitude/phase at the first antenna):
 *    \f[
 *    A_1 e^{j\psi} [ 1 \; e^{jw_x} \; e^{j2w_x} \; e^{j3w_x} \; e^{j4w_x} \; e^{j5w_x} \; e^{j6w_x} \; e^{j7w_x} ]
 *    \f]
 *
 *    An FFT of the above signal will yield a peak \f$P_1\f$ at \f$w_x\f$, with the phase of this peak being \f$\psi\f$ i.e
 *    \f[
 *       P_1 = A_1 e^{j\psi}
 *    \f]
 *    If \f$k_{MAX}\f$ is the index of the peak in log magnitude FFT represented as
 *    signed index in range \f$[-\frac{N}{2}, \frac{N}{2}-1]\f$, then \f$ w_x \f$ will be
 *       \f[
 *            w_x = \frac{2\pi}{N}k_{MAX}
 *       \f]
 *
 *    The signal at the 4 elevation antennas will be:
 *     \f[
 *      A_2 e^{j(\psi + 2 w_x - w_z)} [ 1 \; e^{jw_x} \; e^{j2w_x} \; e^{j3w_x}]
 *     \f]
 *
 *    An FFT of the above signal will yield a peak \f$P_2\f$ at \f$w_x\f$, with the phase of this peak being \f$\psi + 2w_x - w_z\f$.
 *     \f[
 *        P_2 = A_2 e^{j(\psi+ 2 w_x - w_z)}
 *     \f]
 *
 *    From above,
 *     \f[
 *        P_1 \cdot P_2^* = A_1 \cdot A_2 e^{j(\psi - (\psi+ 2 w_x - w_z))}
 *     \f]
 *
 *    Therefore,
 *     \f[
 *       w_z=\angle (P_1 \cdot P_2^* \cdot e^{j2w_x})
 *     \f]
 *
 *
 *    Calculate range (in meters) as:
 *       \f[
 *           R=k_r\frac{c \cdot F_{SAMP}}{2 \cdot S \cdot N_{FFT}}
 *       \f]
 *       where, \f$c\f$ is the speed of light (m/sec), \f$k_r\f$ is range index, \f$F_{SAMP}\f$ is the sampling frequency (Hz),
 *       \f$S\f$ is chirp slope (Hz/sec), \f$N_{FFT}\f$ is 1D FFT size.
 *    Based on above calculations of \f$R\f$, \f$w_x\f$ and \f$w_z\f$, the \f$(x,y,z)\f$ position of the object
 *    can be calculated as seen in the \ref Figure_geometry,
 *       \f[
 *           x = R\cos(\phi)\sin(\theta) = R\frac{w_x}{\pi}, \;\;\;\;\; z = R\sin(\phi) = R\frac{w_z}{\pi},\;\;\;\;\; y = \sqrt{R^2-x^2-z^2}
 *       \f]
 *    The computed \f$(x,y,z)\f$ along with the Doppler value for each detected object are populated in the output
 *    list, structured as @ref DPIF_PointCloudCartesian_t.
 *
 *    If the multi object beam forming feature is enabled, (it can be dynamically controlled from CLI), the algorithm
 *    searches for the second peak in the azimuth FFT output and compares its height relative to the first peak height,
 *    and if detected, it creates new object in the output list with the same Doppler
 *    value and calculated (x,y,z) coordinates.
 *
 * @subsection fovSubSection Elimination of detected objects based on FoV limits
 *
 * The AoA DPU filters out detected objects whose azimuth, \f$\theta\f$, and elevation, \f$\phi\f$, angles
 * are not within configured FoV limits. The DPU receives the following FoV parameters:
 *
 * \f$\phi_{min},\;\phi_{max},\f$ - elevation angle FoV limits
 *
 * \f$\theta_{min},\;\theta_{max}\f$ - azimuth angle FoV limits
 *
 * The DPU computes \f$W_z\f$  and \f$W_x\f$ directly from azimuth FFT output data, where
 *
 * \f$W_z = w_z/\pi\f$
 *
 * \f$W_x = w_x/\pi\f$
 *
 * From equations in previous section the following holds
 *
 * \f$W_z = \sin(\phi)\f$
 *
 * \f$W_x = \cos(\phi)  \sin(\theta)\f$
 *
 * Because sine is a monotonic increasing function in the maximum possible range
 * of -90 deg, 90 deg, the elevation angle \f$\phi\f$ FoV limits can be checked on \f$W_z\f$
 * as follows
 *
 * \f[
 * \sin(\phi_{min}) \leq W_z \leq \sin(\phi_{max})
 * \f]
 *
 * Since \f$W_x\f$ is not monotonic in \f$\phi\f$ but monotonic on \f$\theta\f$,
 * the azimuth angle \f$\theta\f$ limits can be checked by checking \f$W_x\f$ after computing
 * the \f$\cos(\phi)\f$ on the measured \f$\phi\f$.
 *
 * \f[
 * \cos(\phi)\sin(\theta_{min}) \leq W_x \leq \cos(\phi) \sin(\theta_{max})
 * \f]
 *
 * where \f$\cos(\phi) = \sqrt{1 - \sin(\phi)^2} = \sqrt{1 - W_z^2}\f$.
 * Note that the terms  \f$\sin(\phi_{min})\f$, \f$\sin(\phi_{max})\f$,
 * \f$\sin(\theta_{min})\f$ and \f$\sin(\theta_{max})\f$ are precomputed during configuration time
 * and stored in the dpu's instance (@ref DPU_AoAProc_fovAoaLocalCfg_t) to be used
 * in the real-time limit checks above.
 * The brute-force method of checking limits on \f$\phi\f$ and \f$\theta\f$ directly
 * requires computing these, which involves two sin inverses, one cosine and one
 * division which are computationally more expensive than the above method.
 *
 * @section api_section AoAProcHWA APIs
 *
 * - @ref DPU_AoAProcHWA_init - DPUs initialization function.
 * - @ref DPU_AoAProcHWA_config - DPUs configuration function. The configuration can only be done after
 *   the DPU has been initialized using @ref DPU_AoAProcHWA_init. If the parameters used by this DPU do not
 *   change from one frame to the next, this function can be called only once, for the first
 *   frame, otherwise it has to be called before each @ref DPU_AoAProcHWA_process.
 * - @ref DPU_AoAProcHWA_process - DPUs processing function performs AoA estimation of
 *   detected objects in the given frame. This processing can only be done after the
 *   DPU has been configured.
 * - @ref DPU_AoAProcHWA_control - DPUs control function. It processes following messages from DPC:
 *   - @ref DPU_AoAProcHWA_Cmd_FovAoACfg,
 *   - @ref DPU_AoAProcHWA_Cmd_MultiObjBeamFormingCfg,
 *   - @ref DPU_AoAProcHWA_Cmd_CompRxChannelBiasCfg,
 *   - @ref DPU_AoAProcHWA_Cmd_PrepareRangeAzimuthHeatMap
 *
 *  Full API details can be seen at @ref DPU_AOAPROC_EXTERNAL
 */

/** @defgroup DPU_AOAPROC_INTERNAL       aoaProcHWA DPU Internal
 *
 */
/**
@defgroup DPU_AOAPROC_INTERNAL_FUNCTION            aoaProcHWA DPU Internal Functions
@ingroup DPU_AOAPROC_INTERNAL
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup DPU_AOAPROC_INTERNAL_DATA_STRUCTURE      aoaProcHWA DPU Internal Data Structures
@ingroup DPU_AOAPROC_INTERNAL
@brief
*   The section has a list of all internal data structures which are used internally
*   by the aoaProc DPU module.
*/
/**
@defgroup DPU_AOAPROC_INTERNAL_DEFINITION          aoaProcHWA DPU Internal Definitions
@ingroup DPU_AOAPROC_INTERNAL
@brief
*   The section has a list of all internal definitions which are used internally
*   by the aoaProc DPU.
*/


/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef AOAPROCHWA_INTERNAL_H
#define AOAPROCHWA_INTERNAL_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* mmWave SDK Include Files */
#include <ti/common/sys_common.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>
#include <ti/datapath/dpc/dpu/aoaproc/aoaprochwa.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Field of view - AoA Configuration
 *
 * @details
 *  The structure contains the field of view - Angle of arrival configuration used in data path
 *
 *  \ingroup DPU_AOAPROC_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProc_fovAoaLocalCfg_t
{
    /*! @brief    minimum azimuth angle (expressed as sine value) */
    float        minAzimuthSineVal;

    /*! @brief    maximum azimuth angle (expressed as sine value) */
    float        maxAzimuthSineVal;

    /*! @brief    minimum elevation angle (expressed as sine value) */
    float        minElevationSineVal;

    /*! @brief    maximum elevation angle (expressed as sine value) */
    float        maxElevationSineVal;
} DPU_AoAProc_fovAoaLocalCfg;


/**
 * @brief
 *  AoAProcHWA DPU dynamic configuration
 *
 * @details
 *  The structure is used to hold the dynamic configuration used for AoAProcHWA DPU
 *
 *  \ingroup DPU_AOAPROC_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProc_DynamicLocalConfig_t
{

    /*! @brief     Multi object beam forming configuration */
    DPU_AoAProc_MultiObjBeamFormingCfg multiObjBeamFormingCfg;

    /*! @brief     Flag indicates to prepare data for azimuth heat-map */
    bool  prepareRangeAzimuthHeatMap;

    /*! @brief     Pointer to Rx channel compensation configuration */
    DPU_AoAProc_compRxChannelBiasCfg compRxChanCfg;

    /*! @brief      Field of view configuration for AoA */
    DPU_AoAProc_fovAoaLocalCfg     fovAoaLocalCfg;

} DPU_AoAProc_DynamicLocalConfig;

/** @addtogroup DPU_AOAPROC_INTERNAL_DEFINITION
 *  HWA memory bank indices for various input/output operations
 *
 @{ */
/** @brief 2D-FFT single bin calculation - Input HWA memory bank index  */
#define AOAHWA_2DFFT_SINGLE_BIN_INP_HWA_MEM_BANK   1
/** @brief 2D-FFT single bin calculation - Output HWA memory bank index  */
#define AOAHWA_2DFFT_SINGLE_BIN_OUT_HWA_MEM_BANK   2
/** @brief Angle FFT calculation - Input HWA memory bank index  */
#define AOAHWA_ANGLE_INP_HWA_MEM_BANK              0
/** @brief Azimuth FFT calculation - Output (log2 magnitudes) HWA memory bank index  */
#define AOAHWA_ANGLE_AZIM_ABS_OUT_HWA_MEM_BANK     2
/** @brief Elevation FFT calculation - Output (Complex values) HWA memory bank index  */
#define AOAHWA_ANGLE_ELEV_CPLX_OUT_HWA_MEM_BANK    3
/** @brief Azimuth FFT calculation - Output (Complex values) HWA memory bank index  */
#define AOAHWA_ANGLE_AZIM_CPLX_OUT_HWA_MEM_BANK    1

/** @brief Maximum number of receive virtual antennas */
#define  DPU_AOAPROCHWA_MAX_NUM_RX_VIRTUAL_ANTENNAS (SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL)

/**
 @}
 */



/**
 * @brief
 *  AoAProcHWA DPU internal data Object
 *
 * @details
 *  The structure is used to hold AoAProcHWA DPU internal data object
 *
 *  \ingroup DPU_AOAPROC_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct AOAHwaObj_t
{
    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;

    /*! @brief     Data path common parameters */
    DPU_AoAProcHWA_StaticConfig   params;

    /*! @brief     AoA DPU hardware resources */
    DPU_AoAProcHWA_HW_Resources res;

    /*! @brief HWA Memory Bank addresses */
    uint32_t  hwaMemBankAddr[4];

    /*! @brief     DMA channel trigger after HWA processing is done */
    uint8_t     dmaDestChannel;

    /*! @brief     Local buffer with azimuth FFT input symbols */
    uint32_t    azimElevLocalBuf[DPU_AOAPROCHWA_MAX_NUM_RX_VIRTUAL_ANTENNAS];

    /*! @brief     Dynamic configuration */
    DPU_AoAProc_DynamicLocalConfig dynLocalCfg;

    /*! @brief     HWA Processing Done semaphore Handle */
    SemaphoreP_Handle    hwaDone_semaHandle;
}AOAHwaObj;


#ifdef __cplusplus
}
#endif

#endif /* DOPPLERPROC */
