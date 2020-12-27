/**
 *   @file  main.c
 *
 *   @brief
 *      This is the main file which implements the millimeter wave Demo
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
/** @mainpage Millimeter Wave (mmw) Demo for XWR68XX
 *
 *  @section intro_sec Introduction
 *
 *  @image html toplevel.png
 *
 *  The millimeter wave demo shows some of the capabilities of the XWR68xx SoC
 *  using the drivers in the mmWave SDK (Software Development Kit).
 *  It allows user to specify the chirping profile and displays the detected
 *  objects and other information in real-time.
 *
 *  Following is a high level description of the features of this demo:
 *  - Be able to specify desired chirping profile through command line interface (CLI)
 *    on a UART port or through the TI Gallery App - **mmWave Demo Visualizer** -
 *    that allows user to provide a variety of profile configurations via the UART input port
 *    and displays the streamed detected output from another UART port in real-time,
 *    as seen in picture above.
 *  - Some sample profile configurations have been provided in the demo directory that can be
 *    used with CLI directly or via **mmWave Demo Visualizer** under
 *    @verbatim mmw/profiles @endverbatim directory.
 *  - Do 1D, 2D, CFAR, Azimuth and Elevation processing and stream out velocity
 *    and three spatial coordinates (x,y,z) of the detected objects in real-time.
 *    The demo can also be configured to do 2D only detection (velocity and x,y coordinates).
 *  - Various display options besides object detection like azimuth heat map and
 *    Doppler-range heat map.
 *
 *  @section limit Limitations
 *  - Because of UART speed limit (< 1 Mbps), the frame time is more restrictive.
 *    For example, for the azimuth and Doppler heat maps for 256 FFT range and
 *    16 point FFT Doppler, it takes about 200 ms to transmit.
 *  - For most boards, a range bias of few centimeters has been observed. User can estimate
 *    the range bias on their board and correct using the calibration procedure
 *    described in @ref Calibration_section.
 *
 *  @section systemFlow System Execution Flow
 *  The millimeter wave demo runs on R4F (MSS). Following diagram shows the system
 *  execution flow
 *
 *  @image html system_flow.png "System Execution Flow"
 *
 *  @section tasks Software Tasks
 *    The demo consists of the following (SYSBIOS) tasks:
 *    - @ref MmwDemo_initTask. This task is created/launched by @ref main and is a
 *      one-time active task whose main functionality is to initialize drivers (\<driver\>_init),
 *      MMWave module (MMWave_init), DPM module (DPM_init), open UART and data
 *      path related drivers (EDMA, HWA), and create/launch the following tasks
 *      (the @ref CLI_task is launched indirectly by calling @ref CLI_open).
 *    - @ref CLI_task. This command line interface task provides a simplified 'shell' interface
 *      which allows the configuration of the BSS via the mmWave interface (MMWave_config).
 *      It parses input CLI configuration commands like chirp profile and GUI configuration.
 *      When sensor start CLI command is parsed, all actions related to starting sensor and
 *      starting the processing the data path are taken.
 *      When sensor stop CLI command is parsed, all actions related to stopping the sensor and
 *      stopping the processing of the data path are taken
 *    - @ref MmwDemo_mmWaveCtrlTask. This task is used to provide an execution
 *      context for the mmWave control, it calls in an endless loop the MMWave_execute API.
 *    - @ref MmwDemo_DPC_ObjectDetection_dpmTask. This task is used to provide an execution
 *      context for DPM (Data Path Manager) execution, it calls in an endless loop
 *      the DPM_execute API. In this context, all of the registered object detection
 *      DPC (Data Path Chain) APIs like configuration, control and execute will
 *      take place. In this task. When the DPC's execute API produces the detected objects and other
 *      results, they are transmitted out of the UART port for display using the visualizer.
 *
 *  @section datapath Data Path
 *   @image html datapath_overall.png "Top Level Data Path Processing Chain"
 *   \n
 *   \n
 *   @image html datapath_overall_timing.png "Top Level Data Path Timing"
 *
 *   The data path processing consists of taking ADC samples as input and producing
 *   detected objects (point-cloud and other information) to be shipped out of
 *   UART port to the PC. The algorithm processing is realized using
 *   the DPM registered Object Detection DPC. The details of the processing in DPC
 *   can be seen from the following doxygen documentation:
 *   @verbatim
      ti/datapath/dpc/objectdetection/objdethwa/docs/doxygen/html/index.html
     @endverbatim
 *
 *  @subsection output Output information sent to host
 *      Output packets with the detection information are sent out every frame
 *      through the UART. Each packet consists of the header @ref MmwDemo_output_message_header_t
 *      and the number of TLV items containing various data information with
 *      types enumerated in @ref MmwDemo_output_message_type_e. The numerical values
 *      of the types can be found in @ref mmw_output.h. Each TLV
 *      item consists of type, length (@ref MmwDemo_output_message_tl_t) and payload information.
 *      The structure of the output packet is illustrated in the following figure.
 *      Since the length of the packet depends on the number of detected objects
 *      it can vary from frame to frame. The end of the packet is padded so that
 *      the total packet length is always multiple of 32 Bytes.
 *
 *      @image html output_packet_uart.png "Output packet structure sent to UART"
 *
 *      The following subsections describe the structure of each TLV.
 *
 *      @subsubsection tlv1 List of detected objects
 *       Type: (@ref MMWDEMO_OUTPUT_MSG_DETECTED_POINTS)
 *
 *       Length: (Number of detected objects) x (size of @ref DPIF_PointCloudCartesian_t)
 *
 *       Value: Array of detected objects. The information of each detected object
 *       is as per the structure @ref DPIF_PointCloudCartesian_t. When the number of
 *       detected objects is zero, this TLV item is not sent.
 *
 *       The orientation of x,y and z axes relative to the sensor is as per the
 *       following figure.
 *
 *        @image html coordinate_geometry.png "Coordinate Geometry"
 *
 *       The whole detected objects TLV structure is illustrated in figure below.
 *
 *       @image html detected_objects_tlv.png "Detected objects TLV"
 *
 *      @subsubsection tlv2 Range profile
 *       Type: (@ref MMWDEMO_OUTPUT_MSG_RANGE_PROFILE)
 *
 *       Length: (Range FFT size) x (size of uint16_t)
 *
 *       Value: Array of profile points at 0th Doppler (stationary objects).
 *       The points represent the sum of log2 magnitudes of received antennas
 *       expressed in Q9 format.
 *
 *      @subsubsection tlv3 Noise floor profile
 *       Type: (@ref MMWDEMO_OUTPUT_MSG_NOISE_PROFILE)
 *
 *       Length: (Range FFT size) x (size of uint16_t)
 *
 *       Value: This is the same format as range profile but the profile
 *       is at the maximum Doppler bin (maximum speed
 *       objects). In general for stationary scene, there would be no objects or clutter at
 *       maximum speed so the range profile at such speed represents the
 *       receiver noise floor.
 *
 *      @subsubsection tlv4 Azimuth static heatmap
 *       Type: (@ref MMWDEMO_OUTPUT_MSG_AZIMUT_STATIC_HEAT_MAP)
 *
 *       Length: (Range FFT size) x (Number of virtual antennas) (size of @ref cmplx16ImRe_t_)
 *
 *       Value: Array @ref DPU_AoAProcHWA_HW_Resources::azimuthStaticHeatMap. The antenna data
 *       are complex symbols, with imaginary first and real second in the following order:\n
 *       @verbatim
             Imag(ant 0, range 0), Real(ant 0, range 0),...,Imag(ant N-1, range 0),Real(ant N-1, range 0)
             ...
             Imag(ant 0, range R-1), Real(ant 0, range R-1),...,Imag(ant N-1, range R-1),Real(ant N-1, range R-1)
         @endverbatim
 *       Based on this data the static azimuth heat map is constructed by the GUI
 *       running on the host.
 *
 *      @subsubsection tlv5 Range/Doppler heatmap
 *       Type: (@ref MMWDEMO_OUTPUT_MSG_RANGE_DOPPLER_HEAT_MAP)
 *
 *       Length: (Range FFT size) x (Doppler FFT size) (size of uint16_t)
 *
 *       Value: Detection matrix @ref DPIF_DetMatrix::data.
 *       The order is : \n
 *       @verbatim
            X(range bin 0, Doppler bin 0),...,X(range bin 0, Doppler bin D-1),
            ...
            X(range bin R-1, Doppler bin 0),...,X(range bin R-1, Doppler bin D-1)
         @endverbatim
 *
 *      @subsubsection tlv6 Stats information
 *       Type: (@ref MMWDEMO_OUTPUT_MSG_STATS )
 *
 *       Length: (size of @ref MmwDemo_output_message_stats_t)
 *
 *       Value: Timing information as per @ref MmwDemo_output_message_stats_t.
 *       See timing diagram below related to the stats.
 *
 *      @image html processing_timing.png "Processing timing"
 *
 *       Note:
 *       -#  The @ref MmwDemo_output_message_stats_t::interChirpProcessingMargin is not
 *           computed (it is always set to 0). This is because there is no CPU involvement
 *           in the 1D processing (only HWA and EDMA are involved), and it is not possible to
 *           know how much margin is there in chirp processing without CPU being notified
 *           at every chirp when processing begins (chirp event) and when the HWA-EDMA
 *           computation ends. The CPU is intentionally kept free during 1D processing
 *           because a real application may use this time for doing some post-processing
 *           algorithm execution.
 *       -#  While the
 *           @ref MmwDemo_output_message_stats_t::interFrameProcessingTime reported
 *           will be of the current sub-frame/frame,
 *           the @ref MmwDemo_output_message_stats_t::interFrameProcessingMargin and
 *           @ref MmwDemo_output_message_stats_t::transmitOutputTime
 *           will be of the previous sub-frame (of the same
 *           @ref MmwDemo_output_message_header_t::subFrameNumber as that of the
 *           current sub-frame) or of the previous frame.
 *       -#  The @ref MmwDemo_output_message_stats_t::interFrameProcessingMargin excludes
 *           the UART transmission time (available as
 *           @ref MmwDemo_output_message_stats_t::transmitOutputTime). This is done
 *           intentionally to inform the user of a genuine inter-frame processing margin
 *           without being influenced by a slow transport like UART, this transport time
 *           can be significantly longer for example when streaming out debug information like
 *           heat maps. Also, in a real product deployment, higher speed interfaces (e.g LVDS)
 *           are likely to be used instead of UART. User can calculate the margin
 *           that includes transport overhead (say to determine the max frame rate
 *           that a particular demo configuration will allow) using the stats
 *           because they also contain the UART transmission time.
 *
 *      The CLI command \"guMonitor\" specifies which TLV element will
 *      be sent out within the output packet. The arguments of the CLI command are stored
 *      in the structure @ref MmwDemo_GuiMonSel_t.
 *
 *      @subsubsection tlv7 Side information of detected objects
 *       Type: (@ref MMWDEMO_OUTPUT_MSG_DETECTED_POINTS_SIDE_INFO)
 *
 *       Length: (Number of detected objects) x (size of @ref DPIF_PointCloudSideInfo_t)
 *
 *       Value: Array of detected objects side information. The side information
 *       of each detected object is as per the structure @ref DPIF_PointCloudSideInfo_t).
 *       When the number of detected objects is zero, this TLV item is not sent.
 *
 *  @subsection Calibration_section Range Bias and Rx Channel Gain/Phase Measurement and Compensation
 *
 *     Because of imperfections in antenna layouts on the board, RF delays in SOC, etc,
 *     there is need to calibrate the sensor to compensate for bias in the range estimation and
 *     receive channel gain and phase imperfections. The following figure illustrates
 *     the calibration procedure.
 *
 *     @anchor Figure_calibration_ladder_diagram
 *     @image html calibration_ladder_diagram.png "Calibration procedure ladder diagram"
 *
 *      The calibration procedure includes the following steps:
 *     -# Set a strong target like corner reflector at the distance of X meter
 *     (X less than 50 cm is not recommended) at boresight.
 *     -# Set the following command
 *     in the configuration profile in .../profiles/profile_calibration.cfg,
 *     to reflect the position X as follows:
 *     @verbatim
       measureRangeBiasAndRxChanPhase 1 X D
       @endverbatim
 *     where D (in meters) is the distance of window around X where the peak will be searched.
 *     The purpose of the search window is to allow the test environment from not being overly constrained
 *     say because it may not be possible to clear it of all reflectors that may be stronger than the one used
 *     for calibration. The window size is recommended to be at least the distance equivalent of a few range bins.
 *     One range bin for the calibration profile (profile_calibration.cfg) is about 5 cm.
 *     The first argument "1" is to enable the measurement. The stated configuration
 *     profile (.cfg) must be used otherwise the calibration may not work as expected
 *     (this profile ensures all transmit and receive antennas are engaged among
 *     other things needed for calibration).
 *     -# Start the sensor with the configuration file.
 *     -# In the configuration file, the measurement is enabled because of which
 *     the DPC will be configured to perform the measurement and generate
 *     the measurement result (@ref DPU_AoAProc_compRxChannelBiasCfg_t) in its
 *     result structure (@ref DPC_ObjectDetection_ExecuteResult_t::compRxChanBiasMeasurement),
 *     the measurement results are written out on the CLI port (@ref MmwDemo_measurementResultOutput)
 *     in the format below:
 *     @verbatim
       compRangeBiasAndRxChanPhase <rangeBias> <Re(0,0)> <Im(0,0)> <Re(0,1)> <Im(0,1)> ... <Re(0,R-1)> <Im(0,R-1)> <Re(1,0)> <Im(1,0)> ... <Re(T-1,R-1)> <Im(T-1,R-1)>
       @endverbatim
       For details of how DPC performs the measurement, see the DPC documentation.
 *     -# The command printed out on the CLI now can be copied and pasted in any
 *     configuration file for correction purposes. This configuration will be
 *     passed to the DPC for the purpose  of applying compensation during angle
 *     computation, the details of this can be seen in the DPC documentation.
 *     If compensation is not desired,
 *     the following command should be given
 *     @verbatim
       compRangeBiasAndRxChanPhase 0.0   1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
       @endverbatim
 *     Above sets the range bias to 0 and
 *     the phase coefficients to unity so that there is no correction. Note
 *     the two commands must always be given in any configuration file, typically
 *     the measure commmand will be disabled when the correction command is the
 *     desired one.
 *
 *  @section bypassCLI How to bypass CLI
 *
 *    Re-implement the file mmw_cli.c as follows:
 *
 *    -# @ref MmwDemo_CLIInit should just create a task with input taskPriority.
 *       Lets say the task is called "MmwDemo_sensorConfig_task".
 *    -# All other functions are not needed
 *    -# Implement the MmwDemo_sensorConfig_task as follows:
 *       - Fill gMmwMCB.cfg.openCfg
 *       - Fill gMmwMCB.cfg.ctrlCfg
 *       - Add profiles and chirps using @ref MMWave_addProfile and @ref MMWave_addChirp functions
 *       - Call @ref MmwDemo_CfgUpdate for every offset in @ref configStoreOffsets
 *         (MMWDEMO_xxx_OFFSET in mmw.h)
 *       - Fill gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg
 *       - Call @ref MmwDemo_openSensor
 *       - Call @ref MmwDemo_startSensor (One can use helper function
 *         @ref MmwDemo_isAllCfgInPendingState to know if all dynamic config was provided)
 *
 *  @section resourceAlloc Hardware Resource Allocation
 *    The Object Detection DPC needs to configure the DPUs hardware resources (HWA, EDMA).
 *    Even though the hardware resources currently are only required to be allocated
 *    for this one and only DPC in the system, the resource partitioning is shown to be in
 *    the ownership of the demo. This is to illustrate the general
 *    case of resource allocation across more than one DPCs and/or demo's own
 *    processing that is post-DPC processing. This partitioning
 *    can be seen in the mmw_res.h file. This file is passed as a compiler command line
 *    define @verbatim "--define=APP_RESOURCE_FILE="<ti/demo/xwr68xx/mmw/mmw_res.h>" @endverbatim in mmw.mak
 *    when building the DPC sources as part of building the demo application and
 *    is referred in object detection DPC sources where needed as
 *    @verbatim #include APP_RESOURCE_FILE @endverbatim
 *
 *  @section memoryUsage Memory Usage
 *  @subsection memUsageSummary Memory usage summary
 *    The table below shows the usage of various memories available on the device across
 *    the demo application and other SDK components. The table is generated using the demo's
 *    map file and applying some mapping rules to it to generate a condensed summary.
 *    For the mapping rules, please refer to <a href="../../demo_mss_mapping.txt">demo_mss_mapping.txt</a>.
 *    The numeric values shown here represent bytes.
 *    Refer to the <a href="../../xwr68xx_mmw_demo_mss_mem_analysis_detailed.txt">xwr68xx_mmw_demo_mss_mem_analysis_detailed.txt</a>
 *    for detailed analysis of the memory usage across drivers and control/alg components and to
 *    <a href="../../demo_mss_mapping_detailed.txt">demo_mss_mapping_detailed.txt</a> for detailed mapping rules .
 *
 *  \include xwr68xx_mmw_demo_mss_mem_analysis.txt
 *
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/arm/v7a/Pmu.h>
#include <ti/sysbios/family/arm/v7r/vim/Hwi.h>
#include <ti/sysbios/utils/Load.h>


/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/common/mmwave_sdk_version.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/dpm/dpm.h>
#include <ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/uart/UART.h>
#include <ti/utils/cli/cli.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h>
#include <ti/demo/utils/mmwdemo_rfparser.h>
#include <ti/demo/xwr68xx/mmw/mmw_output.h>

/* Demo Include Files */
#include <ti/demo/xwr68xx/mmw/mmw.h>
#include <ti/demo/xwr68xx/mmw/data_path.h>
#include <ti/demo/xwr68xx/mmw/mmw_config.h>

/**
 * @brief Task Priority settings:
 * Mmwave task is at higher priority because of potential async messages from BSS
 * that need quick action in real-time.
 *
 * CLI task must be at a lower priority than object detection
 * dpm task priority because the dynamic CLI command handling in the objection detection
 * dpm task assumes CLI task is held back during this processing. The alternative
 * is to use a semaphore between the two tasks.
 */
#define MMWDEMO_CLI_TASK_PRIORITY                 3
#define MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY      4
#define MMWDEMO_MMWAVE_CTRL_TASK_PRIORITY         5

#if (MMWDEMO_CLI_TASK_PRIORITY >= MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY)
#error CLI task priority must be < Object Detection DPM task priority
#endif

/*! L3 RAM buffer for object detection DPC */
uint8_t gMmwL3[SOC_L3RAM_SIZE];
#pragma DATA_SECTION(gMmwL3, ".l3ram");

 /*! TCM RAM buffer for object detection DPC */
#define MMWDEMO_OBJDET_TCM_SIZE (49U * 1024U)
uint8_t gDPC_ObjDetTCM[MMWDEMO_OBJDET_TCM_SIZE];
#pragma DATA_SECTION(gDPC_ObjDetTCM, ".DPC_objDetTcmbHeap");

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Global Variable for tracking information required by the mmw Demo
 */
MmwDemo_MCB    gMmwMCB;


/**************************************************************************
 *************************** Extern Definitions ***************************
 **************************************************************************/

extern void MmwDemo_CLIInit(uint8_t taskPriority);

/**************************************************************************
 ************************* Millimeter Wave Demo Functions **********************
 **************************************************************************/

void MmwDemo_mmWaveCtrlTask(UArg arg0, UArg arg1);

void MmwDemo_dataPathInit(MmwDemo_DataPathObj *obj);
int32_t MmwDemo_dataPathConfig(void);
void MmwDemo_dataPathOpen(MmwDemo_DataPathObj *obj);
void MmwDemo_dataPathStart (void);
void MmwDemo_dataPathStop (MmwDemo_DataPathObj *obj);

void MmwDemo_transmitProcessedOutput(UART_Handle uartHandle,
                                     DPC_ObjectDetection_ExecuteResult *result,
                                     MmwDemo_output_message_stats      *timingInfo);

void MmwDemo_initTask(UArg arg0, UArg arg1);
void MmwDemo_dataPathTask(UArg arg0, UArg arg1);
int32_t MmwDemo_eventCallbackFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload);

/* external sleep function when in idle (used in .cfg file) */
void MmwDemo_sleep(void);

/**
 *  @b Description
 *  @n
 *      Send assert information through CLI.
 */
void _MmwDemo_debugAssert(int32_t expression, const char *file, int32_t line)
{
    if (!expression) {
        CLI_write ("Exception: %s, line %d.\n", file, line);
    }
}

/**
 *  @b Description
 *  @n
 *      Utility function to set the pending state of configuration.
 *
 *  @param[in] cfg Sub-frame specific dynamic configuration
 *  @param[in] offset Configuration structure offset that uniquely identifies the
 *                    configuration to set to the pending state.
 *
 *  @retval None
 */
static void MmwDemo_setSubFramePendingState(MmwDemo_DPC_ObjDet_DynCfg *cfg, uint32_t offset)
{
    switch (offset)
    {
        case MMWDEMO_GUIMONSEL_OFFSET:
            cfg->isPrepareRangeAzimuthHeatMapPending = 1;
        break;
        case MMWDEMO_CFARCFGRANGE_OFFSET:
            cfg->isCfarCfgRangePending = 1;
        break;
        case MMWDEMO_CFARCFGDOPPLER_OFFSET:
            cfg->isCfarCfgDopplerPending = 1;
        break;
        case MMWDEMO_FOVRANGE_OFFSET:
            cfg->isFovRangePending = 1;
        break;
        case MMWDEMO_FOVDOPPLER_OFFSET:
            cfg->isFovDopplerPending = 1;
        break;
        case MMWDEMO_FOVAOA_OFFSET:
            cfg->isFovAoaCfgPending = 1;
        break;
        case MMWDEMO_MULTIOBJBEAMFORMING_OFFSET:
            cfg->isMultiObjBeamFormingCfgPending = 1;
        break;
        case MMWDEMO_CALIBDCRANGESIG_OFFSET:
            cfg->isCalibDcRangeSigCfg = 1;
        break;
        case MMWDEMO_STATICCLUTTERREMOFVAL_OFFSET:
            cfg->isStaticClutterRemovalCfgPending = 1;
        break;
        case MMWDEMO_ADCBUFCFG_OFFSET:
            /* do nothing */
        break;
        default:
            MmwDemo_debugAssert(0);
        break;
    }
}

/**
 *  @b Description
 *  @n
 *      Utility function to find out if all common configuration is pending
 *
 *  @param[in] cfg Pointer to Common configuration
 *
 *  @retval 1 if all common configuration is pending, else return 0.
 */
static uint8_t MmwDemo_isObjDetCommonCfgPendingState(MmwDemo_DPC_ObjDet_CommonCfg *cfg)
{
    uint8_t retVal;

    retVal = (cfg->isCompRxChannelBiasCfgPending    == 1) &&
             (cfg->isMeasureRxChannelBiasCfgPending == 1);

    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Utility function to find out if all sub-frame specific dynamic
 *      configuration is pending
 *
 *  @param[in] cfg Pointer to sub-frame specific configuration
 *
 *  @retval 1 if all sub-frame specific dynamic configuration is pending, else return 0
 */
static uint8_t MmwDemo_isObjDynCfgPendingState(MmwDemo_DPC_ObjDet_DynCfg *cfg)
{
    uint8_t retVal;

    retVal = (cfg->isCalibDcRangeSigCfg    == 1) &&
             (cfg->isCfarCfgDopplerPending == 1) &&
             (cfg->isCfarCfgRangePending   == 1) &&
             (cfg->isFovDopplerPending     == 1) &&
             (cfg->isFovRangePending       == 1) &&
             (cfg->isMultiObjBeamFormingCfgPending     == 1) &&
             (cfg->isPrepareRangeAzimuthHeatMapPending == 1) &&
             (cfg->isStaticClutterRemovalCfgPending    == 1) &&
             (cfg->isFovAoaCfgPending                  == 1);

    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Utility function to find out if all common configuration is in non-pending (cleared)
 *      state.
 *
 *  @param[in] cfg Pointer to common specific configuration
 *
 *  @retval 1 if all common configuration is in non-pending state, else return 0
 */
static uint8_t MmwDemo_isObjDetCommonCfgInNonPendingState(MmwDemo_DPC_ObjDet_CommonCfg *cfg)
{
    uint8_t retVal;

    retVal = (cfg->isCompRxChannelBiasCfgPending    == 0) &&
             (cfg->isMeasureRxChannelBiasCfgPending == 0);

    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Utility function to find out if all sub-frame specific dynamic configuration
 *      is in non-pending (cleared) state.
 *
 *  @param[in] cfg Pointer to common specific configuration
 *
 *  @retval 1 if all sub-frame specific dynamic configuration is in non-pending
 *          state, else return 0
 */
static uint8_t MmwDemo_isObjDynCfgInNonPendingState(MmwDemo_DPC_ObjDet_DynCfg *cfg)
{
    uint8_t retVal;

    retVal = (cfg->isCalibDcRangeSigCfg    == 0) &&
             (cfg->isCfarCfgDopplerPending == 0) &&
             (cfg->isCfarCfgRangePending   == 0) &&
             (cfg->isFovDopplerPending     == 0) &&
             (cfg->isFovRangePending       == 0) &&
             (cfg->isMultiObjBeamFormingCfgPending     == 0) &&
             (cfg->isPrepareRangeAzimuthHeatMapPending == 0) &&
             (cfg->isStaticClutterRemovalCfgPending    == 0) &&
             (cfg->isFovAoaCfgPending                  == 0);

    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Resets (clears) all pending common configuration of Object Detection DPC
 *
 *  @param[in] cfg Object Detection DPC common configuration
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_resetObjDetCommonCfgPendingState(MmwDemo_DPC_ObjDet_CommonCfg *cfg)
{
    cfg->isCompRxChannelBiasCfgPending = 0;
    cfg->isMeasureRxChannelBiasCfgPending = 0;
}

/**
 *  @b Description
 *  @n
 *      Resets (clears) all pending sub-frame specific dynamic configuration
 *      of Object Detection DPC
 *
 *  @param[in] cfg Object Detection DPC sub-frame dynamic configuration
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_resetObjDetDynCfgPendingState(MmwDemo_DPC_ObjDet_DynCfg *cfg)
{
    cfg->isCalibDcRangeSigCfg    = 0;
    cfg->isCfarCfgDopplerPending = 0;
    cfg->isCfarCfgRangePending   = 0;
    cfg->isFovDopplerPending     = 0;
    cfg->isFovRangePending       = 0;
    cfg->isMultiObjBeamFormingCfgPending = 0;
    cfg->isPrepareRangeAzimuthHeatMapPending = 0;
    cfg->isStaticClutterRemovalCfgPending = 0;
    cfg->isFovAoaCfgPending = 0;
}

/**
 *  @b Description
 *  @n
 *      Utility function to find out if all configuration (common and sub-frame
 *      specific dynamic config) is in pending state.
 *
 *  @retval 1 if all configuration (common and sub-frame specific dynamic config)
 *            is in pending state, else return 0
 */
uint8_t MmwDemo_isAllCfgInPendingState(void)
{
    uint8_t indx, flag = 1;

    for(indx = 0; indx < gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg.numSubFrames; indx++)
    {
        flag = flag && MmwDemo_isObjDynCfgPendingState(&gMmwMCB.subFrameCfg[indx].objDetDynCfg);
    }

    return((MmwDemo_isObjDetCommonCfgPendingState(&gMmwMCB.dataPathObj.objDetCommonCfg) && flag));
}

/**
 *  @b Description
 *  @n
 *      Utility function to find out if all configuration (common and sub-frame
 *      specific dynamic config) is in non-pending (cleared) state.
 *
 *  @retval 1 if all configuration (common and sub-frame specific dynamic config)
 *            is in non-pending state, else return 0
 */
uint8_t MmwDemo_isAllCfgInNonPendingState(void)
{
    uint8_t indx, flag = 1;

    for(indx = 0; indx < gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg.numSubFrames; indx++)
    {
        flag = flag && MmwDemo_isObjDynCfgInNonPendingState(&gMmwMCB.subFrameCfg[indx].objDetDynCfg);
    }

    return((MmwDemo_isObjDetCommonCfgInNonPendingState(&gMmwMCB.dataPathObj.objDetCommonCfg) && flag));
}

/**
 *  @b Description
 *  @n
 *      Utility function to apply configuration to specified sub-frame
 *
 *  @param[in] srcPtr Pointer to configuration
 *  @param[in] offset Offset of configuration within the parent structure
 *  @param[in] size   Size of configuration
 *  @param[in] subFrameNum Sub-frame Number (0 based) to apply to, broadcast to
 *                         all sub-frames if special code MMWDEMO_SUBFRAME_NUM_FRAME_LEVEL_CONFIG
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
void MmwDemo_CfgUpdate(void *srcPtr, uint32_t offset, uint32_t size, int8_t subFrameNum)
{    
    /* if subFrameNum undefined, broadcast to all sub-frames */
    if(subFrameNum == MMWDEMO_SUBFRAME_NUM_FRAME_LEVEL_CONFIG)
    {
        uint8_t  indx;
        for(indx = 0; indx < RL_MAX_SUBFRAMES; indx++)
        {
            memcpy((void *)((uint32_t) &gMmwMCB.subFrameCfg[indx] + offset), srcPtr, size);
            MmwDemo_setSubFramePendingState(&gMmwMCB.subFrameCfg[indx].objDetDynCfg, offset);
        }
        
    }
    else
    {
        /* Apply configuration to specific subframe (or to position zero for the legacy case
           where there is no advanced frame config) */
        memcpy((void *)((uint32_t) &gMmwMCB.subFrameCfg[subFrameNum] + offset), srcPtr, size);
        MmwDemo_setSubFramePendingState(&gMmwMCB.subFrameCfg[subFrameNum].objDetDynCfg, offset);
    }
}


/**
 *  @b Description
 *  @n
 *      mmw demo helper Function to issue MMWave_stop. The DPM_stop will be called
 *      in the BSS frame done call back case.
 *
 *  @retval None
 */
void MmwDemo_MMWave_stop(void)
{
    int32_t     errCode;

    DebugP_log0("App: Issuing MMWave_stop\n");

    /* Stop the mmWave module: */
    if (MMWave_stop (gMmwMCB.ctrlHandle, &errCode) < 0)
    {
        MMWave_ErrorLevel   errorLevel;
        int16_t             mmWaveErrorCode;
        int16_t             subsysErrorCode;

        /* Error/Warning: Unable to stop the mmWave module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);
        if (errorLevel == MMWave_ErrorLevel_ERROR)
        {
            /* Error: Display the error message: */
            System_printf ("Error: mmWave Stop failed [Error code: %d Subsystem: %d]\n",
                            mmWaveErrorCode, subsysErrorCode);

            /* Not expected */
            MmwDemo_debugAssert(0);
        }
        else
        {
            /* Warning: This is treated as a successful stop. */
            System_printf ("mmWave Stop error ignored [Error code: %d Subsystem: %d]\n",
                            mmWaveErrorCode, subsysErrorCode);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Epilog processing after sensor has stopped
 *
 *  @retval None
 */
void MmwDemo_sensorStopEpilog(void)
{
    Task_Stat stat;
    Hwi_StackInfo stackInfo;
    Bool stackOverflow;

    /* Print task statistics, note data path has completely stopped due to
     * end of frame, so we can do non-real time processing like prints on
     * console */
    System_printf("Data Path Stopped (last frame processing done)\n");
    System_printf("Task Stack Usage (Note: CLI and sensor Management Task not reported) ==========\n");
    System_printf("%20s %12s %12s %12s\n", "Task Name", "Size", "Used", "Free");

    Task_stat(gMmwMCB.taskHandles.initTask, &stat);
    System_printf("%20s %12d %12d %12d\n", "Init",
                  stat.stackSize, stat.used, stat.stackSize - stat.used);

    Task_stat(gMmwMCB.taskHandles.mmwaveCtrl, &stat);
    System_printf("%20s %12d %12d %12d\n", "Mmwave Control",
                  stat.stackSize, stat.used, stat.stackSize - stat.used);

    Task_stat(gMmwMCB.taskHandles.objDetDpmTask, &stat);
    System_printf("%20s %12d %12d %12d\n", "ObjDet DPM",
                  stat.stackSize, stat.used, stat.stackSize - stat.used);

    System_printf("HWI Stack (same as System Stack) Usage ============\n");
    stackOverflow = Hwi_getStackInfo(&stackInfo, TRUE);
    if (stackOverflow == TRUE)
    {
        System_printf("HWI Stack overflowed\n");
        MmwDemo_debugAssert(0);
    }
    else
    {
        System_printf("%20s %12s %12s %12s\n", " ", "Size", "Used", "Free");
        System_printf("%20s %12d %12d %12d\n", " ",
                      stackInfo.hwiStackSize, stackInfo.hwiStackPeak,
                      stackInfo.hwiStackSize - stackInfo.hwiStackPeak);
    }
}


/**
 *  @b Description
 *  @n
 *      mmw demo helper Function to do one time sensor initialization. 
 *      User need to fill gMmwMCB.cfg.openCfg before calling this function
 *
 *  @param[in]  isFirstTimeOpen if true then issues MMwave_open 
 *
 *  @retval  0 if no error, -1 if error
 */
int32_t MmwDemo_openSensor(bool isFirstTimeOpen)
{
    int32_t     errCode;

    /*  Open mmWave module, this is only done once */
    if (isFirstTimeOpen == true)
    {
        /* Setup the calibration frequency:
         * TODO: Presently DFP does not support these for 68xx platform,
         * need to change when DFP is updated with the support */
        gMmwMCB.cfg.openCfg.freqLimitLow  = 0U;
        gMmwMCB.cfg.openCfg.freqLimitHigh = 0U;

        /* start/stop async events */
        gMmwMCB.cfg.openCfg.disableFrameStartAsyncEvent = false;
        gMmwMCB.cfg.openCfg.disableFrameStopAsyncEvent  = false;

        /* No custom calibration: */
        gMmwMCB.cfg.openCfg.useCustomCalibration        = false;
        gMmwMCB.cfg.openCfg.customCalibrationEnableMask = 0x0;

        /* Open the mmWave module: */
        if (MMWave_open (gMmwMCB.ctrlHandle, &gMmwMCB.cfg.openCfg, NULL, &errCode) < 0)
        {
            System_printf ("Error: mmWave Open failed %d\n", errCode);
            return -1;
        }
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      mmw demo helper Function to configure sensor. 
 *      User need to fill gMmwMCB.cfg.ctrlCfg and add profiles/chirp to mmWave
 *      before calling this function
 *
 *  @retval  0 if no error, error code otherwise
 */
int32_t MmwDemo_configSensor(void)
{
    int32_t     errCode = 0;

    /* Configure the mmWave module: */
    if (MMWave_config (gMmwMCB.ctrlHandle, &gMmwMCB.cfg.ctrlCfg, &errCode) < 0)
    {
        MMWave_ErrorLevel   errorLevel;
        int16_t             mmWaveErrorCode;
        int16_t             subsysErrorCode;

        /* Error: Report the error */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);
        System_printf ("Error: mmWave Config failed [Error code: %d Subsystem: %d]\n",
                        mmWaveErrorCode, subsysErrorCode);
        goto exit;
    }
    else
    {
        errCode = MmwDemo_dataPathConfig();
        goto exit;
    }

exit:
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      mmw demo helper Function to start sensor.
 *
 *  @retval  0 if no error, -1 if error
 */
int32_t MmwDemo_startSensor(void)
{
    int32_t     errCode;
    MMWave_CalibrationCfg   calibrationCfg;
    
    /*****************************************************************************
     * Data path :: start data path first - this will pend for DPC to ack
     *****************************************************************************/
    MmwDemo_dataPathStart();

    /*****************************************************************************
     * RF :: now start the RF and the real time ticking
     *****************************************************************************/
    /* Initialize the calibration configuration: */
    memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));
    /* Populate the calibration configuration: */
    calibrationCfg.dfeDataOutputMode = gMmwMCB.cfg.ctrlCfg.dfeDataOutputMode;
    calibrationCfg.u.chirpCalibrationCfg.enableCalibration    = true;
    calibrationCfg.u.chirpCalibrationCfg.enablePeriodicity    = true;
    calibrationCfg.u.chirpCalibrationCfg.periodicTimeInFrames = 10U;

    DebugP_log0("App: MMWave_start Issued\n");

    System_printf("Starting Sensor (issuing MMWave_start)\n");

    /* Start the mmWave module: The configuration has been applied successfully. */
    if (MMWave_start(gMmwMCB.ctrlHandle, &calibrationCfg, &errCode) < 0)
    {
        System_printf("Error: mmWave start failed [Error code %d]\n", errCode);
        return -1;
    }
    
    /*****************************************************************************
     * The sensor has been started successfully. Switch on the LED 
     *****************************************************************************/
    GPIO_write (gMmwMCB.cfg.platformCfg.SensorStatusGPIO, 1U);

    gMmwMCB.sensorStartCount++;
    return 0;
}



/**
 *  @b Description
 *  @n
 *      Stops the RF and datapath for the sensor. Blocks until both operation are not complete
 *      Prints epilog at the end
 *
 *  @retval  None
 */
void MmwDemo_stopSensor()
{

    MmwDemo_MMWave_stop();

    /* Wait until DPM_stop is completed */
    Semaphore_pend(gMmwMCB.DPMstopSemHandle, BIOS_WAIT_FOREVER);

    MmwDemo_sensorStopEpilog();

    /* The sensor has been stopped successfully. Switch off the LED */
    GPIO_write (gMmwMCB.cfg.platformCfg.SensorStatusGPIO, 0U);

    gMmwMCB.sensorStopCount++;

    /* print for user */
    System_printf("Sensor has been stopped: startCount: %d stopCount %d\n",
                  gMmwMCB.sensorStartCount,gMmwMCB.sensorStopCount);
}


/**
 *  @b Description
 *  @n
 *  This function is called at the init time from @ref MmwDemo_initTask.
 *  It initializes drivers: ADCBUF, HWA, EDMA, and semaphores used
 *  by  @ref MmwDemo_dataPathTask
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_dataPathInit(MmwDemo_DataPathObj *obj)
{
    MmwDemo_dataPathObjInit(obj);

    /* Initialize HWA */
    MmwDemo_hwaInit(obj);

    /* Initialize EDMA */
    MmwDemo_edmaInit(obj);
}

/**
 *  @b Description
 *  @n
 *      Opens HWA, EDMA and ADCBUF drivers
 *
 *  @param[in] obj pointer to data path object
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_dataPathOpen(MmwDemo_DataPathObj *obj)
{
    MmwDemo_hwaOpen(obj, gMmwMCB.socHandle);
    MmwDemo_edmaOpen(obj);
    obj->adcbufHandle = MmwDemo_ADCBufOpen();
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the data path based on the chirp profile.
 *      After this function is executed, the data path processing will ready to go
 *      when the ADC buffer starts receiving samples corresponding to the chirps.
 *
 *  @retval
 *      0 if no error, error code otherwise.
 */
int32_t MmwDemo_dataPathConfig (void)
{
    int32_t    errCode;
    MMWave_CtrlCfg      *ptrCtrlCfg;
    MmwDemo_DataPathObj *dataPathObj = &gMmwMCB.dataPathObj;
    MmwDemo_SubFrameCfg *subFrameCfg;
    int8_t subFrameIndx;
    MmwDemo_RFParserOutParams RFparserOutParams;

    /* Get data path object and control configuration */
    ptrCtrlCfg = &gMmwMCB.cfg.ctrlCfg;

    if (ptrCtrlCfg->dfeDataOutputMode == MMWave_DFEDataOutputMode_ADVANCED_FRAME)
    {
        gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg.numSubFrames =
            ptrCtrlCfg->u.advancedFrameCfg.frameCfg.frameSeq.numOfSubFrames;
    }
    else
    {
        gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg.numSubFrames = 1;
    }

    DebugP_log0("App: Issuing Pre-start Common Config IOCTL\n");

    gMmwMCB.rfFreqScaleFactor = SOC_getDeviceRFFreqScaleFactor(gMmwMCB.socHandle, &errCode);
    if (errCode < 0)
    {
        System_printf ("Error: Unable to get RF scale factor [Error:%d]\n", errCode);
        goto exit;
    }

    /* DPC pre-start common config */
    errCode = DPM_ioctl (dataPathObj->objDetDpmHandle,
                         DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG,
                         &dataPathObj->objDetCommonCfg.preStartCommonCfg,
                         sizeof (DPC_ObjectDetection_PreStartCommonCfg));

    MmwDemo_resetObjDetCommonCfgPendingState(&dataPathObj->objDetCommonCfg);

    if (errCode < 0)
    {
        System_printf ("Error: Unable to send DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG [Error:%d]\n", errCode);
        goto exit;
    }

    /* Reason for reverse loop is that when sensor is started, the first sub-frame
     * will be active and the ADC configuration needs to be done for that sub-frame
     * before starting (ADC buf hardware does not have notion of sub-frame, it will
     * be reconfigured every sub-frame). This cannot be alternatively done by calling
     * the MmwDemo_ADCBufConfig function only for the first sub-frame because this is
     * a utility API that computes the rxChanOffset that is part of ADC dataProperty
     * which will be used by range DPU and therefore this computation is required for
     * all sub-frames.
     */
    for(subFrameIndx = gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg.numSubFrames -1; subFrameIndx >= 0;
        subFrameIndx--)
    {
        subFrameCfg  = &gMmwMCB.subFrameCfg[subFrameIndx];

        /*****************************************************************************
         * Data path :: Algorithm Configuration
         *****************************************************************************/

        /* Parse the profile and chirp configs and get the valid number of TX Antennas */
        errCode = MmwDemo_RFParser_parseConfig(&RFparserOutParams, subFrameIndx,
                                         &gMmwMCB.cfg.openCfg, ptrCtrlCfg,
                                         &subFrameCfg->adcBufCfg,
                                         gMmwMCB.rfFreqScaleFactor);

        /* if number of doppler chirps is too low, interpolate to be able to detect
         * better with CFAR tuning. E.g. a 2-pt FFT will be problematic in terms
         * of distinguishing direction of motion */
        if (RFparserOutParams.numDopplerChirps <= 4)
        {
            RFparserOutParams.dopplerStep = RFparserOutParams.dopplerStep / (8 / RFparserOutParams.numDopplerBins);
            RFparserOutParams.numDopplerBins = 8;
        }

        if (errCode != 0)
        {
            System_printf ("Error: MmwDemo_RFParser_parseConfig [Error:%d]\n", errCode);
            goto exit;
        }
        {
            DPC_ObjectDetection_PreStartCfg objDetPreStartCfg;
            DPC_ObjectDetection_StaticCfg *staticCfg = &objDetPreStartCfg.staticCfg;

            subFrameCfg->numRangeBins = RFparserOutParams.numRangeBins;
            /* Workaround for range DPU limitation for FFT size 1024 and 12 virtual antennas case*/
            if ((RFparserOutParams.numVirtualAntennas == 12) && (RFparserOutParams.numRangeBins == 1024))
            {
                subFrameCfg->numRangeBins = 1022;
                RFparserOutParams.numRangeBins = 1022;
            }

            subFrameCfg->numDopplerBins = RFparserOutParams.numDopplerBins;
            subFrameCfg->numChirpsPerChirpEvent = RFparserOutParams.numChirpsPerChirpEvent;
            subFrameCfg->adcBufChanDataSize = RFparserOutParams.adcBufChanDataSize;
            subFrameCfg->objDetDynCfg.dynCfg.prepareRangeAzimuthHeatMap = subFrameCfg->guiMonSel.rangeAzimuthHeatMap;

            errCode = MmwDemo_ADCBufConfig(gMmwMCB.dataPathObj.adcbufHandle,
                                                 gMmwMCB.cfg.openCfg.chCfg.rxChannelEn,
                                                 subFrameCfg->numChirpsPerChirpEvent,
                                                 subFrameCfg->adcBufChanDataSize,
                                                 &subFrameCfg->adcBufCfg,
                                                 &staticCfg->ADCBufData.dataProperty.rxChanOffset[0]);
            if ( errCode < 0)
            {
                System_printf("Error: ADCBuf config failed with error [%d]\n", errCode);
                goto exit;
            }

            /* DPC pre-start config */
            {
                int32_t i;

                objDetPreStartCfg.subFrameNum = subFrameIndx;

                /* Fill static configuration */
                staticCfg->ADCBufData.data = (void *)SOC_XWR68XX_MSS_ADCBUF_BASE_ADDRESS;
                staticCfg->ADCBufData.dataProperty.adcBits = 2; /* 16-bit */

                /* only complex format supported */
                MmwDemo_debugAssert(subFrameCfg->adcBufCfg.adcFmt == 0);

                if (subFrameCfg->adcBufCfg.iqSwapSel == 1)
                {
                    staticCfg->ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_IMRE;
                }
                else
                {
                    staticCfg->ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_REIM;
                }
                if (subFrameCfg->adcBufCfg.chInterleave == 0)
                {
                    staticCfg->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_INTERLEAVE_MODE;
                }
                else
                {
                    staticCfg->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_NON_INTERLEAVE_MODE;
                }
                staticCfg->ADCBufData.dataProperty.numAdcSamples = RFparserOutParams.numAdcSamples;
                staticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent = RFparserOutParams.numChirpsPerChirpEvent;
                staticCfg->ADCBufData.dataProperty.numRxAntennas = RFparserOutParams.numRxAntennas;
                staticCfg->ADCBufData.dataSize = RFparserOutParams.numRxAntennas * RFparserOutParams.numAdcSamples * sizeof(cmplx16ImRe_t);
                staticCfg->dopplerStep = RFparserOutParams.dopplerStep;
                staticCfg->isValidProfileHasOneTxPerChirp = RFparserOutParams.validProfileHasOneTxPerChirp;
                staticCfg->numChirpsPerFrame = RFparserOutParams.numChirpsPerFrame;
                staticCfg->numDopplerBins = RFparserOutParams.numDopplerBins;
                staticCfg->numDopplerChirps = RFparserOutParams.numDopplerChirps;
                staticCfg->numRangeBins = RFparserOutParams.numRangeBins;
                staticCfg->numTxAntennas = RFparserOutParams.numTxAntennas;
                staticCfg->numVirtualAntAzim = RFparserOutParams.numVirtualAntAzim;
                staticCfg->numVirtualAntElev = RFparserOutParams.numVirtualAntElev;
                staticCfg->numVirtualAntennas = RFparserOutParams.numVirtualAntennas;
                staticCfg->rangeStep = RFparserOutParams.rangeStep;
                for (i = 0; i < RFparserOutParams.numRxAntennas; i++)
                {
                    staticCfg->rxAntOrder[i] = RFparserOutParams.rxAntOrder[i];
                }
                for (i = 0; i < RFparserOutParams.numTxAntennas; i++)
                {
                    staticCfg->txAntOrder[i] = RFparserOutParams.txAntOrder[i];
                }

                /* Fill dynamic configuration */
                objDetPreStartCfg.dynCfg = subFrameCfg->objDetDynCfg.dynCfg;

                DebugP_log1("App: Issuing Pre-start Config IOCTL (subFrameIndx = %d)\n", subFrameIndx);

                /* send pre-start config */
                errCode = DPM_ioctl (dataPathObj->objDetDpmHandle,
                                     DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG,
                                     &objDetPreStartCfg,
                                     sizeof (DPC_ObjectDetection_PreStartCfg));

                MmwDemo_resetObjDetDynCfgPendingState(&subFrameCfg->objDetDynCfg);

                if (errCode < 0)
                {
                    System_printf ("Error: Unable to send DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG [Error:%d]\n", errCode);
                    goto exit;
                }
            }
        }
    }

exit:
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is used to start the data path manager and
 *      underlying processing chain. It will wait for the processing chain to ack
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_dataPathStart (void)
{
    int32_t errCode;

    DebugP_log0("App: Issuing DPM_start\n");

    /* Start the DPM Profile: */
    if ((errCode = DPM_start(gMmwMCB.dataPathObj.objDetDpmHandle)) < 0)
    {
        /* Error: Unable to start the profile */
        System_printf("Error: Unable to start the DPM [Error: %d]\n", errCode);
        MmwDemo_debugAssert(0);
    }

    /* wait until start completed */
    Semaphore_pend(gMmwMCB.DPMstartSemHandle, BIOS_WAIT_FOREVER);

    DebugP_log0("App: DPM_start Done (post Semaphore_pend on reportFxn reporting start)\n");

}


/**
 *  @b Description
 *  @n
 *      The function is used to Stop data path.
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_dataPathStop (MmwDemo_DataPathObj *obj)
{
    int32_t retVal;

    DebugP_log0("App: Issuing DPM_stop\n");

    retVal = DPM_stop (obj->objDetDpmHandle);
    if (retVal < 0)
    {
        System_printf ("DPM_stop failed[Error code %d]\n", retVal);
        MmwDemo_debugAssert(0);
    }
}

/**
 *  @b Description
 *  @n
 *      The task is used to provide an execution context for the mmWave
 *      control task
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_mmWaveCtrlTask(UArg arg0, UArg arg1)
{
    int32_t errCode;

    while (1)
    {
        /* Execute the mmWave control module: */
        if (MMWave_execute (gMmwMCB.ctrlHandle, &errCode) < 0)
        {
            //System_printf ("Error: mmWave control execution failed [Error code %d]\n", errCode);
            MmwDemo_debugAssert (0);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Registered event function to mmwave which is invoked when an event from the
 *      BSS is received.
 *
 *  @param[in]  msgId
 *      Message Identifier
 *  @param[in]  sbId
 *      Subblock identifier
 *  @param[in]  sbLen
 *      Length of the subblock
 *  @param[in]  payload
 *      Pointer to the payload buffer
 *
 *  @retval
 *      Always return 0
 */
int32_t MmwDemo_eventCallbackFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload)
{
    uint16_t asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);

    /* Process the received message: */
    switch (msgId)
    {
        case RL_RF_ASYNC_EVENT_MSG:
        {
            /* Received Asychronous Message: */
            switch (asyncSB)
            {
                case RL_RF_AE_CPUFAULT_SB:
                {
                    MmwDemo_debugAssert(0);
                    break;
                }
                case RL_RF_AE_ESMFAULT_SB:
                {
                    MmwDemo_debugAssert(0);
                    break;
                }
                case RL_RF_AE_ANALOG_FAULT_SB:
                {
                    MmwDemo_debugAssert(0);
                    break;
                }
                case RL_RF_AE_INITCALIBSTATUS_SB:
                {
                    rlRfInitComplete_t*  ptrRFInitCompleteMessage;
                    uint32_t            calibrationStatus;

                    /* Get the RF-Init completion message: */
                    ptrRFInitCompleteMessage = (rlRfInitComplete_t*)payload;
                    calibrationStatus = ptrRFInitCompleteMessage->calibStatus & 0xFFFU;

                    /* Display the calibration status: */
                    CLI_write ("Debug: Init Calibration Status = 0x%x\n", calibrationStatus);
                    break;
                }
                case RL_RF_AE_FRAME_TRIGGER_RDY_SB:
                {
                    gMmwMCB.stats.frameTriggerReady++;
                    break;
                }
                case RL_RF_AE_MON_TIMING_FAIL_REPORT_SB:
                {
                    gMmwMCB.stats.failedTimingReports++;
                    break;
                }
                case RL_RF_AE_RUN_TIME_CALIB_REPORT_SB:
                {
                    gMmwMCB.stats.calibrationReports++;
                    break;
                }
                case RL_RF_AE_FRAME_END_SB:
                {
                    gMmwMCB.stats.sensorStopped++;

                    DebugP_log0("App: BSS stop (frame end) received\n");

                    MmwDemo_dataPathStop(&gMmwMCB.dataPathObj);
                    break;
                }
                default:
                {
                    System_printf ("Error: Asynchronous Event SB Id %d not handled\n", asyncSB);
                    break;
                }
            }
            break;
        }
        default:
        {
            System_printf ("Error: Asynchronous message %d is NOT handled\n", msgId);
            break;
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      DPM Registered Report Handler. The DPM Module uses this registered function to notify
 *      the application about DPM reports.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  instanceId
 *      Instance Identifier which generated the report
 *  @param[in]  errCode
 *      Error code if any.
 *  @param[in] arg0
 *      Argument 0 interpreted with the report type
 *  @param[in] arg1
 *      Argument 1 interpreted with the report type
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_DPC_ObjectDetection_reportFxn
(
    DPM_Report  reportType,
    uint32_t    instanceId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        System_printf ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        DebugP_assert (0);
    }

    /* Processing further is based on the reports received: This is the control of the profile
     * state machine: */
    switch (reportType)
    {
        case DPM_Report_IOCTL:
        {
            /*****************************************************************
             * DPC has been configured without an error:
             * - This is an indication that the profile configuration commands
             *   went through without any issues.
             *****************************************************************/
            DebugP_log1("App: DPM Report IOCTL, command = %d\n", arg0);

            if (arg0 == DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG)
            {
                DPC_ObjectDetection_PreStartCfg *cfg;
                DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage *memUsage;
                Memory_Stats stats;

                Memory_getStats(NULL, &stats);

                /* Get memory usage from preStartCfg */
                cfg = (DPC_ObjectDetection_PreStartCfg*)arg1;
                memUsage = &cfg->memUsage;

                System_printf(" ========== Memory Stats ==========\n");
                System_printf("%20s %12s %12s %12s %12s\n", " ", "Size", "Used", "Free", "DPCUsed");
                System_printf("%20s %12d %12d %12d %12d\n", "System Heap(TCM)",
                              memUsage->SystemHeapTotal, memUsage->SystemHeapUsed,
                              memUsage->SystemHeapTotal - memUsage->SystemHeapUsed,
                              memUsage->SystemHeapDPCUsed);

                System_printf("%20s %12d %12d %12d\n", "L3",
                              sizeof(gMmwL3),
                              memUsage->L3RamUsage,
                              sizeof(gMmwL3) - memUsage->L3RamUsage);

                System_printf("%20s %12d %12d %12d\n", "TCM",
                              sizeof(gDPC_ObjDetTCM),
                              memUsage->CoreLocalRamUsage,
                              sizeof(gDPC_ObjDetTCM) - memUsage->CoreLocalRamUsage);
            }

            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * DPC has been started without an error:
             * - notify sensor management task that DPC is started.
             *****************************************************************/
            DebugP_log0("App: DPM Report DPC Started\n");
            Semaphore_post(gMmwMCB.DPMstartSemHandle);
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            /* we cannot be getting this because we are operating in Local domain,
             * it is only meant for remote/distributed domain.
             */
            MmwDemo_debugAssert(0);
            break;
        }
        case DPM_Report_DPC_ASSERT:
        {
            DPM_DPCAssert*  ptrAssert;

            /*****************************************************************
             * DPC Fault has been detected:
             * - This implies that the DPC has crashed.
             * - The argument0 points to the DPC assertion information
             *****************************************************************/
            ptrAssert = (DPM_DPCAssert*)arg0;
            CLI_write("Obj Det DPC Exception: %s, line %d.\n", ptrAssert->fileName,
                       ptrAssert->lineNum);
            break;
        }
        case DPM_Report_DPC_STOPPED:
        {
            /*****************************************************************
             * DPC has been stopped without an error:
             * - This implies that the DPC can either be reconfigured or
             *   restarted.
             *****************************************************************/
            DebugP_log0("App: DPM Report DPC Stopped\n");
            Semaphore_post(gMmwMCB.DPMstopSemHandle);
            break;
        }
        case DPM_Report_DPC_INFO:
        {
            /* ObjDetHwa DPC does not support this IOCTL */
            break;
        }
        default:
        {
            DebugP_assert (0);
            break;
        }
    }
    return;
}

void MmwDemo_measurementResultOutput(DPU_AoAProc_compRxChannelBiasCfg *compRxChanCfg)
{
    CLI_write ("compRangeBiasAndRxChanPhase");
    CLI_write (" %.7f", compRxChanCfg->rangeBias);
    int32_t i;
    for (i = 0; i < SYS_COMMON_NUM_TX_ANTENNAS*SYS_COMMON_NUM_RX_CHANNEL; i++)
    {
        CLI_write (" %.5f", (float) compRxChanCfg->rxChPhaseComp[i].real/32768.);
        CLI_write (" %.5f", (float) compRxChanCfg->rxChPhaseComp[i].imag/32768.);
    }
    CLI_write ("\n");
}

/** @brief Transmits detection data over UART
*
*    The following data is transmitted:
*    1. Header (size = 32bytes), including "Magic word", (size = 8 bytes)
*       and including the number of TLV items
*    TLV Items:
*    2. If detectedObjects flag is 1 or 2, DPIF_PointCloudCartesian structure containing
*       X,Y,Z location and velocity for detected objects,
*       size = sizeof(DPIF_PointCloudCartesian) * number of detected objects
*    3. If detectedObjects flag is 1, DPIF_PointCloudSideInfo structure containing SNR
*       and noise for detected objects,
*       size = sizeof(DPIF_PointCloudCartesian) * number of detected objects
*    4. If logMagRange flag is set,  rangeProfile,
*       size = number of range bins * sizeof(uint16_t)
*    5. If noiseProfile flag is set,  noiseProfile,
*       size = number of range bins * sizeof(uint16_t)
*    6. If rangeAzimuthHeatMap flag is set, the zero Doppler column of the
*       range cubed matrix, size = number of Rx Azimuth virtual antennas *
*       number of chirps per frame * sizeof(uint32_t)
*    7. If rangeDopplerHeatMap flag is set, the log magnitude range-Doppler matrix,
*       size = number of range bins * number of Doppler bins * sizeof(uint16_t)
*    8. If statsInfo flag is set, the stats information
*   @param[in] uartHandle   UART driver handle
*   @param[in] result       Pointer to result from object detection DPC processing
*   @param[in] timingInfo   Pointer to sub-frame object stats that contains timing info
*/
void MmwDemo_transmitProcessedOutput(UART_Handle uartHandle,
                                     DPC_ObjectDetection_ExecuteResult *result,
                                     MmwDemo_output_message_stats      *timingInfo)
{
    MmwDemo_output_message_header header;
    MmwDemo_GuiMonSel   *pGuiMonSel;
    MmwDemo_SubFrameCfg *subFrameCfg;
    uint32_t tlvIdx = 0;
    uint32_t i;
    uint32_t numPaddingBytes;
    uint32_t packetLen;
    uint8_t padding[MMWDEMO_OUTPUT_MSG_SEGMENT_LEN];
    uint16_t *detMatrix = (uint16_t *)result->detMatrix.data;

    MmwDemo_output_message_tl   tl[MMWDEMO_OUTPUT_MSG_MAX];

    /* Get subframe configuration */
    subFrameCfg = &gMmwMCB.subFrameCfg[result->subFrameIdx];

    /* Get Gui Monitor configuration */
    pGuiMonSel = &subFrameCfg->guiMonSel;

    /* Clear message header */
    memset((void *)&header, 0, sizeof(MmwDemo_output_message_header));
    /* Header: */
    header.platform =  0xA6843;
    header.magicWord[0] = 0x0102;
    header.magicWord[1] = 0x0304;
    header.magicWord[2] = 0x0506;
    header.magicWord[3] = 0x0708;
    header.numDetectedObj = result->numObjOut;
    header.version =    MMWAVE_SDK_VERSION_BUILD |   //DEBUG_VERSION
                        (MMWAVE_SDK_VERSION_BUGFIX << 8) |
                        (MMWAVE_SDK_VERSION_MINOR << 16) |
                        (MMWAVE_SDK_VERSION_MAJOR << 24);

    packetLen = sizeof(MmwDemo_output_message_header);
    if ((pGuiMonSel->detectedObjects == 1) || (pGuiMonSel->detectedObjects == 2) &&
         (result->numObjOut > 0))
    {
        tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_DETECTED_POINTS;
        tl[tlvIdx].length = sizeof(DPIF_PointCloudCartesian) * result->numObjOut;
        packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
        tlvIdx++;
    }
    /* Side info */
    if ((pGuiMonSel->detectedObjects == 1) && (result->numObjOut > 0))
    {
        tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_DETECTED_POINTS_SIDE_INFO;
        tl[tlvIdx].length = sizeof(DPIF_PointCloudSideInfo) * result->numObjOut;
        packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
        tlvIdx++;
    }
    if (pGuiMonSel->logMagRange)
    {
        tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_RANGE_PROFILE;
        tl[tlvIdx].length = sizeof(uint16_t) * subFrameCfg->numRangeBins;
        packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
        tlvIdx++;
    }
    if (pGuiMonSel->noiseProfile)
    {
        tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_NOISE_PROFILE;
        tl[tlvIdx].length = sizeof(uint16_t) * subFrameCfg->numRangeBins;
        packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
        tlvIdx++;
    }
    if (pGuiMonSel->rangeAzimuthHeatMap)
    {
        tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_AZIMUT_STATIC_HEAT_MAP;
        tl[tlvIdx].length = result->azimuthStaticHeatMapSize * sizeof(cmplx16ImRe_t);
        packetLen += sizeof(MmwDemo_output_message_tl) +  tl[tlvIdx].length;
        tlvIdx++;
    }
    if (pGuiMonSel->rangeDopplerHeatMap)
    {
        tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_RANGE_DOPPLER_HEAT_MAP;
        tl[tlvIdx].length = subFrameCfg->numRangeBins * subFrameCfg->numDopplerBins * sizeof(uint16_t);
        packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
        tlvIdx++;
    }
    if (pGuiMonSel->statsInfo)
    {
        tl[tlvIdx].type = MMWDEMO_OUTPUT_MSG_STATS;
        tl[tlvIdx].length = sizeof(MmwDemo_output_message_stats);
        packetLen += sizeof(MmwDemo_output_message_tl) + tl[tlvIdx].length;
        tlvIdx++;
    }

    /* Fill header */
    header.numTLVs = tlvIdx;
    /* Round up packet length to multiple of MMWDEMO_OUTPUT_MSG_SEGMENT_LEN */
    header.totalPacketLen = MMWDEMO_OUTPUT_MSG_SEGMENT_LEN *
            ((packetLen + (MMWDEMO_OUTPUT_MSG_SEGMENT_LEN-1))/MMWDEMO_OUTPUT_MSG_SEGMENT_LEN);
    header.timeCpuCycles =  Pmu_getCount(0);
    header.frameNumber = result->stats->frameStartIntCounter;
    header.subFrameNumber = result->subFrameIdx;

    UART_writePolling (uartHandle,
                       (uint8_t*)&header,
                       sizeof(MmwDemo_output_message_header));

    tlvIdx = 0;
    /* Send detected Objects */
    if ((pGuiMonSel->detectedObjects == 1) || (pGuiMonSel->detectedObjects == 2) &&
        (result->numObjOut > 0))
    {
        UART_writePolling (uartHandle,
                           (uint8_t*)&tl[tlvIdx],
                           sizeof(MmwDemo_output_message_tl));

        /*Send array of objects */
        UART_writePolling (uartHandle, (uint8_t*)result->objOut,
                           sizeof(DPIF_PointCloudCartesian) * result->numObjOut);
        tlvIdx++;
    }

    /* Send detected Objects Side Info */
    if ((pGuiMonSel->detectedObjects == 1) && (result->numObjOut > 0))
    {
        UART_writePolling (uartHandle,
                           (uint8_t*)&tl[tlvIdx],
                           sizeof(MmwDemo_output_message_tl));

        /*Send array of objects */
        UART_writePolling (uartHandle, (uint8_t*)result->objOutSideInfo,
                           sizeof(DPIF_PointCloudSideInfo) * result->numObjOut);
        tlvIdx++;
    }

    /* Send Range profile */
    if (pGuiMonSel->logMagRange)
    {
        UART_writePolling (uartHandle,
                           (uint8_t*)&tl[tlvIdx],
                           sizeof(MmwDemo_output_message_tl));

        for(i = 0; i < subFrameCfg->numRangeBins; i++)
        {
            UART_writePolling (uartHandle,
                    (uint8_t*)&detMatrix[i * subFrameCfg->numDopplerBins],
                    sizeof(uint16_t));
        }
        tlvIdx++;
    }

    /* Send noise profile */
    if (pGuiMonSel->noiseProfile)
    {
        uint32_t maxDopIdx = subFrameCfg->numDopplerBins/2 -1;
        UART_writePolling (uartHandle,
                           (uint8_t*)&tl[tlvIdx],
                           sizeof(MmwDemo_output_message_tl));

        for(i = 0; i < subFrameCfg->numRangeBins; i++)
        {
            UART_writePolling (uartHandle,
                    (uint8_t*)&detMatrix[i * subFrameCfg->numDopplerBins + maxDopIdx],
                    sizeof(uint16_t));
        }
        tlvIdx++;
    }

    /* Send data for static azimuth heatmap */
    if (pGuiMonSel->rangeAzimuthHeatMap)
    {
        UART_writePolling (uartHandle,
                           (uint8_t*)&tl[tlvIdx],
                           sizeof(MmwDemo_output_message_tl));

        UART_writePolling (uartHandle,
                (uint8_t *) result->azimuthStaticHeatMap,
                result->azimuthStaticHeatMapSize * sizeof(cmplx16ImRe_t));

        tlvIdx++;
    }

    /* Send data for range/Doppler heatmap */
    if (pGuiMonSel->rangeDopplerHeatMap == 1)
    {
        UART_writePolling (uartHandle,
                           (uint8_t*)&tl[tlvIdx],
                           sizeof(MmwDemo_output_message_tl));
        UART_writePolling (uartHandle,
                (uint8_t*)detMatrix,
                tl[tlvIdx].length);
        tlvIdx++;
    }

    /* Send stats information */
    if (pGuiMonSel->statsInfo == 1)
    {
        UART_writePolling (uartHandle,
                           (uint8_t*)&tl[tlvIdx],
                           sizeof(MmwDemo_output_message_tl));
        UART_writePolling (uartHandle,
                           (uint8_t*)timingInfo,
                           tl[tlvIdx].length);
        tlvIdx++;
    }

    /* Send padding bytes */
    numPaddingBytes = MMWDEMO_OUTPUT_MSG_SEGMENT_LEN - (packetLen & (MMWDEMO_OUTPUT_MSG_SEGMENT_LEN-1));
    if (numPaddingBytes<MMWDEMO_OUTPUT_MSG_SEGMENT_LEN)
    {
        UART_writePolling (uartHandle,
                            (uint8_t*)padding,
                            numPaddingBytes);
    }
}


/**
 *  @b Description
 *  @n
 *      Utility function to get next sub-frame index
 *
 *  @param[in] currentIndx Current sub-frame index
 *  @param[in] numSubFrames Number of sub-frames
 *
 *  @retval
 *      Index of next sub-frame.
 */
static uint8_t MmwDemo_getNextSubFrameIndx(uint8_t currentIndx, uint8_t numSubFrames)
{
    uint8_t nextIndx;

    if (currentIndx == (numSubFrames - 1))
    {
        nextIndx = 0;
    }
    else
    {
        nextIndx = currentIndx + 1;
    }
    return(nextIndx);
}

/**
 *  @b Description
 *  @n
 *      Utility function to get previous sub-frame index
 *
 *  @param[in] currentIndx Current sub-frame index
 *  @param[in] numSubFrames Number of sub-frames
 *
 *  @retval
 *      Index of previous sub-frame
 */
static uint8_t MmwDemo_getPrevSubFrameIndx(uint8_t currentIndx, uint8_t numSubFrames)
{
    uint8_t prevIndx;

    if (currentIndx == 0)
    {
        prevIndx = numSubFrames - 1;
    }
    else
    {
        prevIndx = currentIndx - 1;
    }
    return(prevIndx);
}

/**
 *  @b Description
 *  @n
 *      Processes any pending dynamic configuration commands for the specified
 *      sub-frame by fanning out to the respective DPUs using IOCTL interface, and
 *      resets (clears) the pending state after processing.
 *
 *  @param[in] subFrameIndx Sub-frame index of desired sub-frame to process
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MmwDemo_processPendingDynamicCfgCommands(uint8_t subFrameIndx)
{
    int32_t retVal = 0;

    MmwDemo_DPC_ObjDet_CommonCfg *commonCfg = &gMmwMCB.dataPathObj.objDetCommonCfg;
    MmwDemo_DPC_ObjDet_DynCfg *subFrameCfg = &gMmwMCB.subFrameCfg[subFrameIndx].objDetDynCfg;

    /* perform globals ones if first sub-frame */
    if (subFrameIndx == 0)
    {
        if (commonCfg->isMeasureRxChannelBiasCfgPending == 1)
        {
            retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                                DPC_OBJDET_IOCTL__DYNAMIC_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE,
                                &commonCfg->preStartCommonCfg.measureRxChannelBiasCfg,
                                sizeof (DPC_ObjectDetection_MeasureRxChannelBiasCfg));
            if (retVal != 0)
            {
                goto exit;
            }
            commonCfg->isMeasureRxChannelBiasCfgPending = 0;
        }
        if (commonCfg->isCompRxChannelBiasCfgPending == 1)
        {
            retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                                DPC_OBJDET_IOCTL__DYNAMIC_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE,
                                &commonCfg->preStartCommonCfg.compRxChanCfg,
                                sizeof (DPU_AoAProc_compRxChannelBiasCfg));
            if (retVal != 0)
            {
                goto exit;
            }
            commonCfg->isCompRxChannelBiasCfgPending = 0;
        }
    }

    /* Perform sub-frame specific ones */
    if (subFrameCfg->isCalibDcRangeSigCfg == 1)
    {
        DPC_ObjectDetection_CalibDcRangeSigCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.calibDcRangeSigCfg;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_CALIB_DC_RANGE_SIG_CFG,
                            &cfg,
                            sizeof (DPC_ObjectDetection_CalibDcRangeSigCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isCalibDcRangeSigCfg = 0;
    }
    if (subFrameCfg->isCfarCfgDopplerPending == 1)
    {
        DPC_ObjectDetection_CfarCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.cfarCfgDoppler;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_CFAR_DOPPLER_CFG,
                            &cfg,
                            sizeof (DPC_ObjectDetection_CfarCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isCfarCfgDopplerPending = 0;
    }
    if (subFrameCfg->isCfarCfgRangePending == 1)
    {
        DPC_ObjectDetection_CfarCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.cfarCfgRange;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_CFAR_RANGE_CFG,
                            &cfg,
                            sizeof (DPC_ObjectDetection_CfarCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isCfarCfgRangePending = 0;
    }
    if (subFrameCfg->isFovDopplerPending == 1)
    {
        DPC_ObjectDetection_fovDopplerCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.fovDoppler;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_FOV_DOPPLER,
                            &cfg,
                            sizeof (DPC_ObjectDetection_fovDopplerCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isFovDopplerPending = 0;
    }
    if (subFrameCfg->isFovRangePending == 1)
    {
        DPC_ObjectDetection_fovRangeCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.fovRange;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_FOV_RANGE,
                            &cfg,
                            sizeof (DPC_ObjectDetection_fovRangeCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isFovRangePending = 0;
    }
    if (subFrameCfg->isMultiObjBeamFormingCfgPending == 1)
    {
        DPC_ObjectDetection_MultiObjBeamFormingCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.multiObjBeamFormingCfg;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_MULTI_OBJ_BEAM_FORM_CFG,
                            &cfg,
                            sizeof (DPC_ObjectDetection_MultiObjBeamFormingCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isMultiObjBeamFormingCfgPending = 0;
    }
    if (subFrameCfg->isPrepareRangeAzimuthHeatMapPending == 1)
    {
        DPC_ObjectDetection_RangeAzimuthHeatMapCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.prepareRangeAzimuthHeatMap = subFrameCfg->dynCfg.prepareRangeAzimuthHeatMap;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_RANGE_AZIMUTH_HEAT_MAP,
                            &cfg,
                            sizeof (DPC_ObjectDetection_RangeAzimuthHeatMapCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isPrepareRangeAzimuthHeatMapPending = 0;
    }
    if (subFrameCfg->isStaticClutterRemovalCfgPending == 1)
    {
        DPC_ObjectDetection_StaticClutterRemovalCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.staticClutterRemovalCfg;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_STATICCLUTTER_REMOVAL_CFG,
                            &cfg,
                            sizeof (DPC_ObjectDetection_StaticClutterRemovalCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isStaticClutterRemovalCfgPending = 0;
    }
    if (subFrameCfg->isFovAoaCfgPending == 1)
    {
        DPC_ObjectDetection_fovAoaCfg cfg;

        cfg.subFrameNum = subFrameIndx;
        cfg.cfg = subFrameCfg->dynCfg.fovAoaCfg;
        retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                            DPC_OBJDET_IOCTL__DYNAMIC_FOV_AOA,
                            &cfg,
                            sizeof (DPC_ObjectDetection_fovAoaCfg));
        if (retVal != 0)
        {
            goto exit;
        }
        subFrameCfg->isFovAoaCfgPending = 0;
    }

exit:
    return(retVal);
}
/**
 *  @b Description
 *  @n
 *      DPM Execution Task. DPM execute results are processed here:
 *      a) Transmits results through UART port.
 *      b) Updates book-keeping code for timing info.
 *      c) Notifies DPC that results have been exported (using DPC IOCTL command)
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwDemo_DPC_ObjectDetection_dpmTask(UArg arg0, UArg arg1)
{
    int32_t     retVal;
    DPM_Buffer  resultBuffer;
    DPC_ObjectDetection_ExecuteResultExportedInfo exportInfo;
    DPC_ObjectDetection_ExecuteResult *result;

    while (1)
    {
        /* Execute the DPM module: */
        retVal = DPM_execute (gMmwMCB.dataPathObj.objDetDpmHandle, &resultBuffer);
        if (retVal < 0) {
            System_printf ("Error: DPM execution failed [Error code %d]\n", retVal);
            MmwDemo_debugAssert (0);
        }
        else
        {
            if (resultBuffer.size[0] == sizeof(DPC_ObjectDetection_ExecuteResult))
            {
                volatile uint32_t startTime, currentInterFrameProcessingEndTime;
                MmwDemo_SubFrameStats *currSubFrameStats, *prevSubFrameStats;
                uint16_t dummyRxChanOffset[SYS_COMMON_NUM_RX_CHANNEL];

                uint8_t numSubFrames;
                uint8_t nextSubFrameIndx, prevSubFrameIndx;

                result = (DPC_ObjectDetection_ExecuteResult *)resultBuffer.ptrBuffer[0];
                currSubFrameStats = &gMmwMCB.subFrameStats[result->subFrameIdx];

                currentInterFrameProcessingEndTime = Cycleprofiler_getTimeStamp();
                currSubFrameStats->outputStats.interFrameProcessingTime =
                        (currentInterFrameProcessingEndTime - result->stats->interFrameStartTimeStamp)/R4F_CLOCK_MHZ;

                numSubFrames = gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg.numSubFrames;

                /* Update the processing end margin, note this is of the previous sub-frame
                 * and should work even when there is only 1 sub-frame (or non-advanced mode)
                 * as long as end time stamp is updated after using it. Note that
                 * "result->stats->subFramePreparationCycles" is of the previous sub-frame's
                 * preparation for the next (which is the current here) because sub-frame
                 * preparation happens when DPC processes the results exported IOCTL which
                 * is issued after all the result handling here. However, we also store
                 * this after using for debug purposes (to be able to see all sub-frames switching cycles
                 * in real-time)
                 */
                prevSubFrameIndx = MmwDemo_getPrevSubFrameIndx(result->subFrameIdx, numSubFrames);
                prevSubFrameStats = &gMmwMCB.subFrameStats[prevSubFrameIndx];
                prevSubFrameStats->outputStats.interFrameProcessingMargin =
                    (result->stats->frameStartTimeStamp - prevSubFrameStats->interFrameProcessingEndTime)/R4F_CLOCK_MHZ -
                    (result->stats->subFramePreparationCycles/R4F_CLOCK_MHZ + prevSubFrameStats->subFramePreparationTime) -
                    prevSubFrameStats->pendingConfigProcTime;

                /* Below is only for debug purposes */
                prevSubFrameStats->objDetSubFramePreparationTime = result->stats->subFramePreparationCycles/R4F_CLOCK_MHZ;

                /* This must be after above prevSubFrame processing */
                currSubFrameStats->interFrameProcessingEndTime = currentInterFrameProcessingEndTime;

                startTime = Cycleprofiler_getTimeStamp();

                /* Send out of CLI the range bias and phase config measurement if it was enabled. */
                if (gMmwMCB.dataPathObj.objDetCommonCfg.preStartCommonCfg.measureRxChannelBiasCfg.enabled == 1)
                {
                    if (result->compRxChanBiasMeasurement != NULL)
                    {
                        MmwDemo_measurementResultOutput(result->compRxChanBiasMeasurement);
                    }
                    else
                    {
                        /* DPC is not ready to ship the measurement results */
                    }
                }

                MmwDemo_transmitProcessedOutput(gMmwMCB.loggingUartHandle, result,
                                                &currSubFrameStats->outputStats);

                currSubFrameStats->outputStats.transmitOutputTime =
                    (Cycleprofiler_getTimeStamp() - startTime)/R4F_CLOCK_MHZ;

                startTime = Cycleprofiler_getTimeStamp();

                /* For non-advanced frame case:
                 *   process all pending dynamic config commands.
                 * For advanced-frame case:
                 *  Process next sub-frame related pending dynamic config commands.
                 *  If the next sub-frame was the first sub-frame of the frame,
                 *  then process common (sub-frame independent) pending dynamic config
                 *  commands. */
                nextSubFrameIndx = MmwDemo_getNextSubFrameIndx(result->subFrameIdx,
                                                               numSubFrames);
                retVal = MmwDemo_processPendingDynamicCfgCommands(nextSubFrameIndx);
                if (retVal != 0)
                {
                    System_printf ("Error: Executing Pending Dynamic Configuration Commands for nextSubFrameIndx = %d [Error code %d]\n",
                                   nextSubFrameIndx, retVal);
                    MmwDemo_debugAssert(0);
                }

                currSubFrameStats->pendingConfigProcTime =
                        (Cycleprofiler_getTimeStamp() - startTime)/R4F_CLOCK_MHZ;

                /* Configure ADC for next sub-frame */
                if (numSubFrames > 1)
                {
                    MmwDemo_SubFrameCfg  *nextSubFrameCfg;

                    startTime = Cycleprofiler_getTimeStamp();

                    nextSubFrameCfg =  &gMmwMCB.subFrameCfg[nextSubFrameIndx];
                    retVal = MmwDemo_ADCBufConfig(gMmwMCB.dataPathObj.adcbufHandle,
                                             gMmwMCB.cfg.openCfg.chCfg.rxChannelEn,
                                             nextSubFrameCfg->numChirpsPerChirpEvent,
                                             nextSubFrameCfg->adcBufChanDataSize,
                                             &nextSubFrameCfg->adcBufCfg,
                                             &dummyRxChanOffset[0]);
                    if (retVal < 0)
                    {
                        System_printf("Error: ADCBuf config failed with error [%d]\n", retVal);
                        MmwDemo_debugAssert (0);
                    }

                    currSubFrameStats->subFramePreparationTime =
                            (Cycleprofiler_getTimeStamp() - startTime)/R4F_CLOCK_MHZ;
                }
                else
                {
                    currSubFrameStats->subFramePreparationTime = 0;
                }

                /* indicate result consumed and end of frame/sub-frame processing */
                exportInfo.subFrameIdx = result->subFrameIdx;
                retVal = DPM_ioctl (gMmwMCB.dataPathObj.objDetDpmHandle,
                                     DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED,
                                     &exportInfo,
                                     sizeof (DPC_ObjectDetection_ExecuteResultExportedInfo));
                if (retVal < 0) {
                    System_printf ("Error: DPM DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED failed [Error code %d]\n",
                                   retVal);
                    MmwDemo_debugAssert(0);
                }
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Call back function that was registered during config time and is going
 *      to be called in DPC processing at the beginning of frame/sub-frame processing,
 *      we use this to issue BIOS calls for computing CPU load during inter-frame
 *
 *  @param[in] subFrameIndx Sub-frame index of the sub-frame during which processing
 *             this function was called.
 *
 *  @retval None
 */
void MmwDemo_DPC_ObjectDetection_processFrameBeginCallBackFxn(uint8_t subFrameIndx)
{
    MmwDemo_SubFrameStats *stats;

    stats = &gMmwMCB.subFrameStats[subFrameIndx];

    Load_update();
    stats->outputStats.interFrameCPULoad = Load_getCPULoad();
}

/**
 *  @b Description
 *  @n
 *      Call back function that was registered during config time and is going
 *      to be called in DPC processing at the beginning of
 *      inter-frame/inter-sub-frame processing,
 *      we use this to issue BIOS calls for computing CPU load during active frame
 *      (chirping)
 *
 *  @param[in] subFrameIndx Sub-frame index of the sub-frame during which processing
 *             this function was called.
 *
 *  @retval None
 */
void MmwDemo_DPC_ObjectDetection_processInterFrameBeginCallBackFxn(uint8_t subFrameIndx)
{
    MmwDemo_SubFrameStats *stats;

    stats = &gMmwMCB.subFrameStats[subFrameIndx];

    Load_update();
    stats->outputStats.activeFrameCPULoad = Load_getCPULoad();
}

/**
 *  @b Description
 *  @n
 *      Platform specific hardware initialization.
 *
 *  @retval
 *      Not Applicable.
 */

void MmwDemo_platformInit(MmwDemo_platformCfg *config)
{
    /* Setup the PINMUX to bring out the UART-1 */
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINN5_PADBE, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);    
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINN5_PADBE, SOC_XWR68XX_PINN5_PADBE_MSS_UARTA_TX);
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINN4_PADBD, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);    
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINN4_PADBD, SOC_XWR68XX_PINN4_PADBD_MSS_UARTA_RX);

    /* Setup the PINMUX to bring out the UART-3 */
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINF14_PADAJ, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINF14_PADAJ, SOC_XWR68XX_PINF14_PADAJ_MSS_UARTB_TX);

    /**********************************************************************
     * Setup the PINMUX:
     * - GPIO Output: Configure pin K13 as GPIO_2 output
     **********************************************************************/
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINK13_PADAZ, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINK13_PADAZ, SOC_XWR68XX_PINK13_PADAZ_GPIO_2);

    /**********************************************************************
     * Setup the GPIO:
     * - GPIO Output: Configure pin K13 as GPIO_2 output
     **********************************************************************/
    config->SensorStatusGPIO    = SOC_XWR68XX_GPIO_2;

    /* Initialize the DEMO configuration: */
    config->sysClockFrequency   = MSS_SYS_VCLK;
    config->loggingBaudRate     = 921600;
    config->commandBaudRate     = 115200;

    /**********************************************************************
     * Setup the DS3 LED on the EVM connected to GPIO_2
     **********************************************************************/
    GPIO_setConfig (config->SensorStatusGPIO, GPIO_CFG_OUTPUT);
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_initTask(UArg arg0, UArg arg1)
{
    int32_t             errCode;
    MMWave_InitCfg      initCfg;
    UART_Params         uartParams;
    Task_Params         taskParams;
    Semaphore_Params    semParams;
    DPM_InitCfg         dpmInitCfg;
    DPC_ObjectDetection_InitParams      objDetInitParams;
    uint32_t            edmaCCIdx;
    //int32_t             retVal;

    /* Debug Message: */
    System_printf("Debug: Launched the Initialization Task\n");

    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/

    /* Initialize the UART */
    UART_init();

    /* Initialize the GPIO */
    GPIO_init();

    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_MSS);

    /* Platform specific configuration */
    MmwDemo_platformInit(&gMmwMCB.cfg.platformCfg);

    /* Initialize the Data Path: */
    MmwDemo_dataPathInit(&gMmwMCB.dataPathObj);

    /* Setup the default UART Parameters */
    UART_Params_init(&uartParams);
    uartParams.clockFrequency = gMmwMCB.cfg.platformCfg.sysClockFrequency;
    uartParams.baudRate       = gMmwMCB.cfg.platformCfg.commandBaudRate;
    uartParams.isPinMuxDone   = 1;

    /* Open the UART Instance */
    gMmwMCB.commandUartHandle = UART_open(0, &uartParams);
    if (gMmwMCB.commandUartHandle == NULL)
    {
        //System_printf("Error: Unable to open the Command UART Instance\n");
        MmwDemo_debugAssert (0);
        return;
    }
    System_printf("Debug: UART Instance %p has been opened successfully\n", gMmwMCB.commandUartHandle);

    /* Setup the default UART Parameters */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.clockFrequency = gMmwMCB.cfg.platformCfg.sysClockFrequency;
    uartParams.baudRate       = gMmwMCB.cfg.platformCfg.loggingBaudRate;
    uartParams.isPinMuxDone   = 1U;

    /* Open the Logging UART Instance: */
    gMmwMCB.loggingUartHandle = UART_open(1, &uartParams);
    if (gMmwMCB.loggingUartHandle == NULL)
    {
        //System_printf("Error: Unable to open the Logging UART Instance\n");
        MmwDemo_debugAssert (0);
        return;
    }
    System_printf("Debug: UART Instance %p has been opened successfully\n", gMmwMCB.loggingUartHandle);

    /* Create a binary semaphores which are used to signal DPM_start/DPM_stop are done.
     * The signaling (Semaphore_post) will be done from DPM registered report function
     * (which will execute in the DPM execute task context). */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    gMmwMCB.DPMstartSemHandle = Semaphore_create(0, &semParams, NULL);
    gMmwMCB.DPMstopSemHandle  = Semaphore_create(0, &semParams, NULL);

    /*****************************************************************************
     * mmWave: Initialization of the high level module
     *****************************************************************************/

    /* Initialize the mmWave control init configuration */
    memset ((void*)&initCfg, 0 , sizeof(MMWave_InitCfg));

    /* Populate the init configuration: */
    initCfg.domain                  = MMWave_Domain_MSS;
    initCfg.socHandle               = gMmwMCB.socHandle;
    initCfg.eventFxn                = MmwDemo_eventCallbackFxn;
    initCfg.linkCRCCfg.useCRCDriver = 1U;
    initCfg.linkCRCCfg.crcChannel   = CRC_Channel_CH1;
    initCfg.cfgMode                 = MMWave_ConfigurationMode_FULL;
    initCfg.executionMode           = MMWave_ExecutionMode_ISOLATION;

    /* Initialize and setup the mmWave Control module */
    gMmwMCB.ctrlHandle = MMWave_init (&initCfg, &errCode);
    if (gMmwMCB.ctrlHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        //System_printf ("Error: mmWave Control Initialization failed [Error code %d]\n", errCode);
        MmwDemo_debugAssert (0);
        return;
    }
    System_printf ("Debug: mmWave Control Initialization was successful\n");

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequisite and always needs to be invoked. */
    if (MMWave_sync (gMmwMCB.ctrlHandle, &errCode) < 0)
    {
        /* Error: Unable to synchronize the mmWave control module */
        System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);
        MmwDemo_debugAssert (0);
        return;
    }
    System_printf ("Debug: mmWave Control Synchronization was successful\n");

    MmwDemo_dataPathOpen(&gMmwMCB.dataPathObj);

    /* Configure banchmark counter */
    Cycleprofiler_init();

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    Task_Params_init(&taskParams);
    taskParams.priority  = MMWDEMO_MMWAVE_CTRL_TASK_PRIORITY;
    taskParams.stackSize = 3*1024;
    gMmwMCB.taskHandles.mmwaveCtrl = Task_create(MmwDemo_mmWaveCtrlTask, &taskParams, NULL);

    /*****************************************************************************
     * Initialization of the DPM Module:
     *****************************************************************************/
    memset ((void *)&objDetInitParams, 0, sizeof(DPC_ObjectDetection_InitParams));

    /* Note this must be after MmwDemo_dataPathOpen() above which opens the hwa
     * and edma drivers */
    objDetInitParams.hwaHandle = gMmwMCB.dataPathObj.hwaHandle;
    for (edmaCCIdx = 0; edmaCCIdx < EDMA_NUM_CC; edmaCCIdx++)
    {
        objDetInitParams.edmaHandle[edmaCCIdx] = gMmwMCB.dataPathObj.edmaHandle[edmaCCIdx];
    }

    /* Memory related config */
    objDetInitParams.L3ramCfg.addr = (void *)&gMmwL3[0];
    objDetInitParams.L3ramCfg.size = sizeof(gMmwL3);
    objDetInitParams.CoreLocalRamCfg.addr = &gDPC_ObjDetTCM[0];
    objDetInitParams.CoreLocalRamCfg.size = sizeof(gDPC_ObjDetTCM);

    /* Call-back config */
    objDetInitParams.processCallBackCfg.processFrameBeginCallBackFxn =
        MmwDemo_DPC_ObjectDetection_processFrameBeginCallBackFxn;
    objDetInitParams.processCallBackCfg.processInterFrameBeginCallBackFxn =
        MmwDemo_DPC_ObjectDetection_processInterFrameBeginCallBackFxn;

    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Setup the configuration: */
    dpmInitCfg.socHandle        = gMmwMCB.socHandle;
    dpmInitCfg.ptrProcChainCfg  = &gDPC_ObjectDetectionCfg;
    dpmInitCfg.instanceId       = 0xFEEDFEED;
    dpmInitCfg.domain           = DPM_Domain_LOCALIZED;
    dpmInitCfg.reportFxn        = MmwDemo_DPC_ObjectDetection_reportFxn;
    dpmInitCfg.arg              = &objDetInitParams;
    dpmInitCfg.argSize          = sizeof(DPC_ObjectDetection_InitParams);

    /* Initialize the DPM Module: */
    gMmwMCB.dataPathObj.objDetDpmHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gMmwMCB.dataPathObj.objDetDpmHandle == NULL)
    {
        System_printf ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MmwDemo_debugAssert (0);
        return;
    }

    /* Launch the DPM Task */
    Task_Params_init(&taskParams);
    taskParams.priority  = MMWDEMO_DPC_OBJDET_DPM_TASK_PRIORITY;
    taskParams.stackSize = 4*1024;
    gMmwMCB.taskHandles.objDetDpmTask = Task_create(MmwDemo_DPC_ObjectDetection_dpmTask, &taskParams, NULL);

    /*****************************************************************************
     * Initialize the CLI Module:
     *      User can choose to create their own task here with the same priority
     *      instead that does hard coded config instead of interactive CLI
     *****************************************************************************/
    MmwDemo_CLIInit(MMWDEMO_CLI_TASK_PRIORITY);

    return;
}

/**
 *  @b Description
 *  @n
 *     Function to sleep the R4F using WFI (Wait For Interrupt) instruction.
 *     When R4F has no work left to do,
 *     the BIOS will be in Idle thread and will call this function. The R4F will
 *     wake-up on any interrupt (e.g chirp interrupt).
 *
 *  @retval
 *      Not Applicable.
 */
void MmwDemo_sleep(void)
{
    /* issue WFI (Wait For Interrupt) instruction */
    asm(" WFI ");
}

/**
 *  @b Description
 *  @n
 *      Entry point into the Millimeter Wave Demo
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params     taskParams;
    int32_t         errCode;
    SOC_Handle      socHandle;
    SOC_Cfg         socCfg;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;
    socCfg.dssCfg = SOC_DSSCfg_HALT;
    socCfg.mpuCfg = SOC_MPUCfg_CONFIG;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        //System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        MmwDemo_debugAssert (0);
        return -1;
    }

    /* Initialize and populate the demo MCB */
    memset ((void*)&gMmwMCB, 0, sizeof(MmwDemo_MCB));

    gMmwMCB.socHandle = socHandle;

    /* Debug Message: */
    System_printf ("**********************************************\n");
    System_printf ("Debug: Launching the MMW HWA Demo\n");
    System_printf ("**********************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    gMmwMCB.taskHandles.initTask = Task_create(MmwDemo_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}
