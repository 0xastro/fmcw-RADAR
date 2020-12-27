/*************************************************************************************************
 * FileName    : mmwavelink.h
 *
 * Description : This file includes all the header files which needs to be included by application
 *
 *************************************************************************************************
 * (C) Copyright 2014, Texas Instruments Incorporated. - TI web address www.ti.com
 *------------------------------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted
 *  provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific prior
 *    written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 *  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*************************************************************************************************
 * FILE INCLUSION PROTECTION
 *************************************************************************************************
 */
#ifndef MMWAVELINK_H
#define MMWAVELINK_H
/*LDRA_NOANALYSIS*/
 /*!
 \mainpage mmWaveLink Framework

 \section intro_sec Introduction

 TI Automotive and Industrial mmWave Radar products are highly-integrated 77GHz CMOS millimeter
 wave devices.The devices integrate all of the RF and Analog functionality, including VCO, PLL,
 PA, LNA, Mixer and ADC for multiple TX/RX channels into a single chip.\n
 -# The AWR1243 is an RF transceiver device and it includes 4 receiver channels and 3 transmit
 channels in a single chip. AWR1243 also support multi-chip cascading. \n
 -# The AWR1443/IWR1443 is a mmwave radar-on-a-chip device, which includes 4 receive channels
 and 3 transmit channels and additionally an Cortex R4F and hardware FFT accelerator.\n
 -# AWR1642 and IWR1642 are mmwave radar-on-a-chip device, which includes 4 receive channels and 2
 transmit channels and additionally an Cortex R4F and C674x DSP for signal processing

 TI mmWave radar devices include a mmwave front end or BIST (Built-in Self-Test) processor, which
 is responsible to configure the RF/Analog and digital front-end in real-time, as well as to
 periodically schedule calibration  and functional safety monitoring.This enables the mmwave
 front-end(BIST processor) to be self-contained and capable of adapting itself to handle
 temperature and ageing effects, and to enable significant ease-of-use from an external host
 perspective.

 TI mmwave front end is a closed subsystem whose internal blocks are configurable using messages
 coming over mailbox.\n TI mmWaveLink provides APIs generates these message and sends it to mmwave
 front end over mailbox. It also includes acknowledement and CRC for each message to provide a
 reliable communication

 TI mmWaveLink Framework:
 - Is a link between application and mmwave front end.
 - Provides low level APIs to configure the front end and handles the communication with the front
   end.
 - Is platform and OS independent which means it can be ported into any processor which provides
   communication interface such as SPI and basic OS routines. The mmWaveLink framework can also
   run in single threaded environment
 - Is integrated into mmWave SDK and can run on R4F or DSP and communicates with mmwave front
   end over Mailbox interface

 *  @image html mmwl_block_diagram.png

  \section modules_sec Modules
  To make it simple, TI's mmWaveLink framework capabilities are divided into modules.\n
  These capabilities include device control, RF/Analog configuration, ADC configuration,
  Data path(LVDS/CSI2) cofiguration, FMCW chirp configuration and more.\n
  Listed below are the various modules in the mmWaveLink framework:
      -# \ref Device - Controls mmwave radar device which include:

            * Initialization, such as: mmwave device power On/Off, Firmware Patch download
            * Cascade device configuration such as Add/Connect multiple mmWave devices

      -# \ref Sensor - The RF/Sensor Configuration module controls the different HW blocks
             inside mmWave Front end.

       *  @image html mmwave_frontend.png

         * mmWave Front End has below key blocks
         -# Chirp sequencer (Radar Timing Engine) - This block is responsible for constructing
            the sequence of FMCW chirps or frames and programming the timing engine
         -# Rx/Tx Channel - This defines how many Rx and Tx channel needs to be enabled. Also it
            defines how to configure the mmWave front end in cascade mode for Imaging Radar
         -# Rx Analog Chain - This defines how the received signal is mixed and how different
            filters in the chain can be configured
         -# ADC and Digital Front End Configuration - This defines how the IF data is digitized
            and how it is sampled for further processing in the DSP or Hardware Accelerator.
            Same ADC data can be sent over LVDS/CSI2 interface to an extenal processor

       * The configuration APIs can further be categorized as.\n
       -# mmwave static configuration, such as: Tx and Rx channel, ADC configuration etc
       -# mmwave dynamic configuration, such as FMCW Chirp configuration, profile configuration
       -# mmwave advance configuration such as Binary phase modulation, Dynamic power save etc
       -# mmwave sensor control, such as: Frame start/stop

      -# \ref Data_Path - The Data path Configuration module controls the high speed interface
              in mmWave device.

       *  @image html data_path.png

       * The configuration APIs include.\n
       -# High Speed interface(LVDS/CSI2) selection
       -# Data format and rate configuration
       -# ADC, Chirp Profile(CP), Chirp Quality(CQ) data transfer sequence
       -# Lane configurations
       -# LVDS/CSI2 specific configuration

      -# \ref Monitoring - The Monitoring/Calibration module configures the calibration and
              functional safety monitoring in mmWave device

     * TI mmWave Front end includes built-in processor that is programmed by TI to handle RF
      calibrations and functional safety monitoring.  The RF calibrations ensure that the
      performance of the device is maintained across temperature and process corners

      -# \ref Communication_Protocol - The mmWave communication protocol ensures reliable
              communication between host(internal or external) and mmWave Front end.

        -# It is a simple stop and wait protocol. Each message needs to be acknowledged
            by receiver before next message can be sent.
        -# Messages are classifieds as "Command", "Response" and "Asynchronous Event"
        -# If Command can not be processed immediately, ACK response is sent immediately
            (If requested). "Asynchronous Event"  is sent upon completion of the command
            execution.

       *  @image html comm_prot.png

     \section         proting_sec     Porting Guide

 The porting of the mmWaveLink driver to any new platform is based on few simple steps.
 This guide takes you through this process step by step. Please follow the instructions
 carefully to avoid any problems during this process and to enable efficient and proper
 work with the device.
 Please notice that all modifications and porting adjustments of the driver should be
 made in the application only and driver should not be modified.
 Changes in the application file will ensure smoothly transaction to
 new versions of the driver at the future!

 \subsection     porting_step1   Step 1 - Define mmWaveLink client callback structure
The mmWaveLink framework is ported to different platforms using mmWaveLink client callbacks. These
callbacks are grouped as different structures such as OS callbacks, Communication Interface
callbacks and others. Application needs to define these callbacks and initialize the mmWaveLink
framework with the structure.

 *  @code
 *  rlClientCbs_t clientCtx = { 0 };
 *  rlReturnVal_t retVal;
 *  rlUInt32_t deviceMap = RL_DEVICE_MAP_CASCADED_1;
 *  @endcode

 Refer to \ref rlClientCbs_t for more details

 \subsection     porting_step2   Step 2 - Implement Communication Interface Callbacks
 The mmWaveLink device support several standard communication protocol among SPI and MailBox
  Depending on device variant, one need to choose the communication channel. For e.g
 xWR1443/xWR1642/xWR1843 requires Mailbox interface and AWR1243 supports SPI interface.
 The interface for this communication channel should include 4 simple access functions:
 -# rlComIfOpen
 -# rlComIfClose
 -# rlComIfRead
 -# rlComIfWrite

 *  @code
 *  clientCtx.comIfCb.rlComIfOpen = Host_spiOpen;
 *  clientCtx.comIfCb.rlComIfClose = Host_spiClose;
 *  clientCtx.comIfCb.rlComIfRead = Host_spiRead;
 *  clientCtx.comIfCb.rlComIfWrite = Host_spiWrite;
 *  @endcode

 Refer to \ref rlComIfCbs_t for interface details

  \subsection     porting_step3   Step 3 - Implement Device Control Interface
 The mmWaveLink driver internally powers on/off the mmWave radar device. The exact implementation
 of these interface is platform dependent, hence you need to implement below functions:
 -# rlDeviceEnable
 -# rlDeviceDisable
 -# rlRegisterInterruptHandler

 *  @code
 *  clientCtx.devCtrlCb.rlDeviceDisable = Host_disableDevice;
 *  clientCtx.devCtrlCb.rlDeviceEnable = Host_enableDevice;
 *  clientCtx.devCtrlCb.rlDeviceMaskHostIrq = Host_spiIRQMask;
 *  clientCtx.devCtrlCb.rlDeviceUnMaskHostIrq = Host_spiIRQUnMask;
 *  clientCtx.devCtrlCb.rlRegisterInterruptHandler = Host_registerInterruptHandler;
 *  clientCtx.devCtrlCb.rlDeviceWaitIrqStatus = Host_deviceWaitIrqStatus;
 *  @endcode

 Refer to \ref rlDeviceCtrlCbs_t for interface details

 \subsection     porting_step4     Step 4 - Implement Event Handlers
 The mmWaveLink driver reports asynchronous event indicating mmwave radar device status,
 exceptions etc. Application can register this callback to receive these notification and take
 appropriate actions

 *  @code
 *  clientCtx.eventCb.rlAsyncEvent = Host_asyncEventHandler;
 *  @endcode

 Refer to \ref rlEventCbs_t for interface details

 \subsection     porting_step5     Step 5 - Implement OS Interface
 The mmWaveLink driver can work in both OS and NonOS environment. If Application prefers to use
 operating system, it needs to implement basic OS routines such as tasks, mutex and Semaphore
 
 *  @code
 *  clientCtx.osiCb.mutex.rlOsiMutexCreate = Host_osiLockObjCreate;
 *  clientCtx.osiCb.mutex.rlOsiMutexLock = Host_osiLockObjLock;
 *  clientCtx.osiCb.mutex.rlOsiMutexUnLock = Host_osiLockObjUnlock;
 *  clientCtx.osiCb.mutex.rlOsiMutexDelete = Host_osiLockObjDelete;
 *
 *  clientCtx.osiCb.sem.rlOsiSemCreate = Host_osiSyncObjCreate;
 *  clientCtx.osiCb.sem.rlOsiSemWait = Host_osiSyncObjWait;
 *  clientCtx.osiCb.sem.rlOsiSemSignal = Host_osiSyncObjSignal;
 *  clientCtx.osiCb.sem.rlOsiSemDelete = Host_osiSyncObjDelete;
 *
 *  clientCtx.osiCb.queue.rlOsiSpawn = Host_osiSpawn;
 *
 *  clientCtx.timerCb.rlDelay = Host_osiSleep;
 *  @endcode


 Refer to \ref rlOsiCbs_t for interface details

 \subsection     porting_step6     Step 6 - Implement CRC Interface
 The mmWaveLink driver uses CRC for message integrity. If Application prefers to use
 CRC, it needs to implement CRC routine.

 *  @code
 *  clientCtx.crcCb.rlComputeCRC = Host_computeCRC;
 *  @endcode

 Refer to \ref rlCrcCbs_t for interface details

 \subsection     porting_step7     Step 7 - Implement Debug Interface
 The mmWaveLink driver can print the debug message. If Application prefers to enable
 debug messages, it needs to implement debug callback.

 Refer to \ref rlDbgCb_t for interface details

 \subsection     porting_final     Final Step - Initializing mmWaveLink Driver
 Once all the above Interfaces are implemented, Application need to fill these callbacks
 in \ref rlClientCbs_t and Initialize mmWaveLink by passing the client callbacks. 
 Application also need to define where the mmWaveLink driver is running, for e.g,
 External Host in case of AWR1243 or MSS/DSS in case of xWR1642/xWR1843.
 
 *
 *  @code
 *  clientCtx.platform = RL_PLATFORM_HOST;
 *  clientCtx.arDevType = RL_AR_DEVICETYPE_12XX;
 *
 *  retVal = rlDevicePowerOn(deviceMap, clientCtx);
 *  @endcode
 *
  \subsection     porting_be     Big Endian Support
 The mmWaveLink driver by default is enabled for Little Endian host. Support for
 Big Endian is provided as compile time option using a Pre-processor Macro MMWL_BIG_ENDIAN.\n
 For memory optimizations, mmWaveLink doesn't swap the data elements in structure buffer. It
 is the responsibility of the application to swap multi byte data elements before passing the
 structure buffer to mmWaveLink API. Since SPI word-size is 16bit, Swap of 32 bit fields such
 as integer needs to be done at 16bit boundary.
 */
/*LDRA_ANALYSIS*/

 /****************************************************************************************
 * INCLUDE FILES
 *****************************************************************************************
 */
#include <stdint.h>
#include <ti/control/mmwavelink/include/rl_datatypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************
 * MACRO DEFINITIONS
 *****************************************************************************************
 */

/* Export Macro for DLL */
#if defined(WIN32) || defined(WIN32_) || defined(_MSC_VER)
#define MMWL_EXPORT __declspec(dllexport)
#else
#define MMWL_EXPORT
#endif

/*! mmWaveLink Version */
#define RL_MMWAVELINK_VERSION               "1.2.0.1.24.9.18"
#define RL_MMWAVELINK_VERSION_MAJOR          (1U)
#define RL_MMWAVELINK_VERSION_MINOR          (2U)
#define RL_MMWAVELINK_VERSION_BUILD          (0U)
#define RL_MMWAVELINK_VERSION_DEBUG          (1U)
#define RL_MMWAVELINK_VERSION_DAY            (24U)
#define RL_MMWAVELINK_VERSION_MONTH          (9U)
#define RL_MMWAVELINK_VERSION_YEAR           (18U)

/*! mmWaveLink Error Codes */
#define RL_RET_CODE_OK                      ((rlReturnVal_t)0) /* no-error */
#define RL_RET_CODE_PROTOCOL_ERROR          (-1)        /* mmWaveLink Protocol error */
#define RL_RET_CODE_INVALID_INPUT           (-2)        /* invalid input from the application */
#define RL_RET_CODE_SELF_ERROR              (-3)        /* error in mmWaveLink itself */
#define RL_RET_CODE_RADAR_IF_ERROR          (-4)        /* Radar HW/SW interface error */
#define RL_RET_CODE_MALLOC_ERROR            (-5)        /* memory allocation error */
#define RL_RET_CODE_CRC_FAILED              (-6)        /* CRC value mismatched wrt
                                                                received data */
#define RL_RET_CODE_CHKSUM_FAILED           (-7)        /* Checksum value mismatched wrt to
                                                                received data */
#define RL_RET_CODE_RESP_TIMEOUT            (-8)        /* device failed to send response
                                                                within time */
#define RL_RET_CODE_FATAL_ERROR             (-9)        /* Fatal error internal to
                                                                mmWaveLink APIs */
#define RL_RET_CODE_RADAR_OSIF_ERROR        (-10)       /* OS interface failure */
#define RL_RET_CODE_INVALID_STATE_ERROR     (-11)       /* Invalid state within mmWaveLink */
#define RL_RET_CODE_API_NOT_SUPPORTED       (-12)       /* API called is not supported */
#define RL_RET_CODE_MSGID_MISMATCHED        (-13)       /* Message-ID mismatched in
                                                                response data */
#define RL_RET_CODE_NULL_PTR                (-14)       /* Null pointer error */
#define RL_RET_CODE_INTERFACE_CB_NULL       (-15)       /* Interface callback passed as NULL */

/*! RF Error Codes */
#define RL_RET_CODE_INVLD_OPCODE                      (1U)    /* Incorrect opcode/Msg ID */
#define RL_RET_CODE_INVLD_NUM_SB                      (2U)    /* Incorrect no. of Sub-Block */
#define RL_RET_CODE_INVLD_SB_ID                       (3U)    /* Incorrect Sub-Block ID */
#define RL_RET_CODE_INVLD_SB_LEN                      (4U)    /* Incorrect Sub-Block Length */
#define RL_RET_CODE_SB_INVL_DATA                      (5U)    /* Incorrect Sub-Block Data */
#define RL_RET_CODE_SB_PROCESS_ERR                    (6U)    /* Error in Sub Block processing */
#define RL_RET_CODE_MISMATCH_FILE_CRC                 (7U)    /* Mismatch in File CRC */
#define RL_RET_CODE_MISMATCH_FILE_TYPE                (8U)    /* Mismatch in File Type */

/*! Frame start stop API */
#define RL_RET_CODE_FRAME_ALREADY_STARTED             (20U) /* Frames are already started when the
                                                                FRAME_START command was issued */
#define RL_RET_CODE_FRAME_ALREADY_ENDED               (21U) /* Frames are already stopped when the
                                                                FRAME_STOP command was issued */
#define RL_RET_CODE_FRAME_CFG_NOT_RECVD               (22U) /* No valid frame configuration API was
                                                                issued and frames are started */
#define RL_RET_CODE_FRAME_TRIG_INVL_IN                (23U) /* START_STOP_CMD parameter is out of
                                                                range*/

/*! Channel Config API */
#define RL_RET_CODE_CH_CFG_RX_INVAL_IN                (24U) /* RX_CHAN_EN parameter is out of range
                                                                may vary based on device */
#define RL_RET_CODE_CH_CFG_TX_INVAL_IN                (25U) /* TX_CHAN_EN parameter is out of range
                                                                may vary based on device */
#define RL_RET_CODE_CH_CFG_CASC_INVAL_IN              (26U) /* CASCADING_CFG parameter is out of
                                                                range [0, 2] */

/*! ADC out API */
#define RL_RET_CODE_ADC_BITS_INVAL_IN                 (27U) /* NUM_ADC_BITS parameter is out of
                                                                range [0, 2] */
#define RL_RET_CODE_ADC_FORM_INVAL_IN                 (28U) /* ADC_OUT_FMT parameter is out of
                                                                range [0, 3] */

/*! Low power ADC API */
#define RL_RET_CODE_LP_ADC_INVAL_IN                   (29U) /* LP_ADC_MODE parameter is out of
                                                                range [0, 1] */

/*! Dynamic power save API */
#define RL_RET_CODE_DYN_PS_INVAL_IN                   (30U) /* BLOCK_CFG parameter is out of
                                                                range [0, 7] */

/*! HSI config API */
#define RL_RET_CODE_HSI_DIV_INVAL_IN                  (31U) /* HSI clock rate code[1:0] is 0 */
#define RL_RET_CODE_RESERVED0                         (32U)
#define RL_RET_CODE_HSI_DIV_INVAL_1IN                 (33U) /* HSI clock rate code[3:2] is 3  &
                                                                HSI clock rate code[1:0] is 2 */
#define RL_RET_CODE_HSI_DIV_INVAL_2IN                 (34U) /* HSI clock rate code[3:2] is 3  &
                                                                HSI clock rate code[1:0] is 2 */

/*! Profile config API */
#define RL_RET_CODE_PF_IND_INVAL_IN                   (35U) /* PF indx >= 4 */
#define RL_RET_CODE_PF_START_FREQ_INVAL_IN            (36U) /* PF freq const is not
                                                                with[76GHz,81GHz] in limit */
#define RL_RET_CODE_PF_IDLE_TIME_INVAL_IN             (37U) /* PF idle time const > 5.24ms */
#define RL_RET_CODE_PF_IDLE_TIME_1INVAL_IN            (38U) /* Maximum DFE spill time >
                                                                PF idle time const */
#define RL_RET_CODE_PF_ADC_START_INVAL_IN             (39U) /* PF ADC start time const > 4095 */
#define RL_RET_CODE_PF_RAMP_END_INVAL_IN              (40U) /* PF ramp end time > 524287 */
#define RL_RET_CODE_PF_RAMP_END_1INVAL_IN             (41U) /* PF ramp end time < PF ADC start
                                                                time const + ADC sampling time */
#define RL_RET_CODE_PF_TX0_INVAL_IN                   (42U) /* PF_TX_OUTPUT_POWER_BACKOFF for
                                                                TX0 > 30 */
#define RL_RET_CODE_PF_TX1_INVAL_IN                   (43U) /* PF_TX_OUTPUT_POWER_BACKOFF for
                                                                TX1 > 30 */
#define RL_RET_CODE_PF_TX2_INVAL_IN                   (44U) /* PF_TX_OUTPUT_POWER_BACKOFF for
                                                                TX2 > 30 */
#define RL_RET_CODE_RESERVED1                         (45U)
#define RL_RET_CODE_PF_FREQ_SLOPE_1INVAL_IN           (46U) /* Ramp end freq is not
                                                                with[76GHz,81GHz] in limits */
#define RL_RET_CODE_PF_TX_START_INVAL_IN              (47U) /* Absolute value of TX_START_TIME
                                                                is > 38.45us */
#define RL_RET_CODE_PF_NUM_ADC_SMAP_INVAL_IN          (48U) /* Number of ADC samples is not
                                                                within [64,8192] */
#define RL_RET_CODE_PF_DFE_SAMP_RATE_INVAL_IN         (49U) /* Output sampling rate is not
                                                                within [2, 37.5]Msps */
#define RL_RET_CODE_PF_HPF1_CF_INVAL_IN               (50U) /* HPF1 corner frequency > 700 kHz */
#define RL_RET_CODE_PF_HPF2_CF_INVAL_IN               (51U) /* HPF2 corner frequency > 2.8 MHz */
#define RL_RET_CODE_PF_RX_GAIN_INVAL_IN               (52U) /* PF_RX_GAIN is not within [24, 52] dB
                                                                orPF_RX_GAIN is an odd number */
#define RL_RET_CODE_RESERVED2                         (53U)
#define RL_RET_CODE_RESERVED3                         (54U)
#define RL_RET_CODE_RESERVED4                         (55U)
#define RL_RET_CODE_RESERVED5                         (56U)
#define RL_RET_CODE_RESERVED6                         (57U)
#define RL_RET_CODE_RESERVED7                         (58U)

/*! Chirp config API */
#define RL_RET_CODE_CHIRP_START_INVAL_IN              (59U) /* Chirp Start indx >= 512 */
#define RL_RET_CODE_CHIRP_END_INVAL_IN                (60U) /* Chirp End indx >= 512 */
#define RL_RET_CODE_CHIRP_END_1INVAL_IN               (61U) /* Chirp Start indx > Chirp End indx */
#define RL_RET_CODE_CHIRP_PF_IND_INVAL_IN             (62U) /* PF indx >= 4 */
#define RL_RET_CODE_CHIRP_PF_IND_1INVAL_IN            (63U) /* PF corresponding to PF indx is not
                                                                defined */
#define RL_RET_CODE_CHIRP_START_FREQ_INVAL_IN         (64U) /* Chirp freq start > 8388607 */
#define RL_RET_CODE_CHIRP_SLOPE_INVAL_IN              (65U) /* Chirp freq slope > 63 */
#define RL_RET_CODE_CHIRP_SLOPE_1INVAL_IN             (66U) /* Chirp start or end
                                                                freq[76GHz,81GHz] is outside */
#define RL_RET_CODE_CHIRP_IDLE_TIME_INVAL_IN          (67U) /* Chirp Idle time > 4095 */
#define RL_RET_CODE_CHIRP_ADC_START_INVAL_IN          (68U) /* Chirp ADC start time > 4095 */
#define RL_RET_CODE_CHIRP_ADC_START_1INVAL_IN         (69U) /* Ramp end time < ADC start time +
                                                                ADC sampling time */
#define RL_RET_CODE_CHIRP_TX_ENA_INVAL_IN             (70U) /* Chirp TX enable > 7 */
#define RL_RET_CODE_CHIRP_TX_ENA_1INVAL_IN            (71U) /* Chirp TX enable indicates to enable
                                                                a TX which is not enabled in
                                                                Channel config */

/*! Frame config API */
#define RL_RET_CODE_FRAME_CHIRP_STR_INVAL_IN          (72U) /* Chirp Start indx >= 512 */
#define RL_RET_CODE_FRAME_CHIRP_END_INVAL_IN          (73U) /* Chirp End indx >= 512 */
#define RL_RET_CODE_FRAME_CHIRP_END_1INVAL_IN         (74U) /* Chirp Start indx > Chirp End indx */
#define RL_RET_CODE_FRAME_CHIRP_END_2INVAL_IN         (75U) /* Chirp used in frame is not
                                                                configured by Chirp config */
#define RL_RET_CODE_FRAME_CHIRP_PF_INVAL_IN           (76U) /* Profile used in frame is not
                                                                configured by PF config */
#define RL_RET_CODE_FRAME_CHIRP_LOOPS_INVAL_IN        (77U) /* No. of loops is outside[1,255] */
#define RL_RET_CODE_RESERVED8                         (78U)
#define RL_RET_CODE_FRAME_PERIOD_INVAL_IN             (79U) /* Frame periodicity is
                                                                outside[100us,1.342s] */
#define RL_RET_CODE_FRAME_PERIOD_1INVAL_IN            (80U) /* Frame ON time < Frame periodicity */
#define RL_RET_CODE_FRAME_TRIG_SEL_INVAL_IN           (81U) /* Trigger select is outside[1,2] */
#define RL_RET_CODE_FRAME_TRIG_DELAY_INVAL_IN         (82U) /* Frame Trigger delay > 100us */
#define RL_RET_CODE_FRAME_IS_ONGOING                  (83U) /* API issued when frame is ongoing */

/*! Advance Frame config API */
#define RL_RET_CODE_AFRAME_NUM_SUBF_INVAL_IN          (84U) /* No. Sub Frames is outside[1,4] */
#define RL_RET_CODE_AFRAME_FORCE_PF_INVAL_IN          (85U) /* Force single Profile is
                                                                outside[1,4] */
#define RL_RET_CODE_AFRAME_PF_IND_INVAL_IN            (86U) /* Force single Profile >= 4 */
#define RL_RET_CODE_AFRAME_PF_IND_1INVAL_IN           (87U) /* Profile defined by Force Single
                                                                Profile is not defined */
#define RL_RET_CODE_AFRAME_CHIRP_STR_INVAL_IN         (88U) /* Sub Frame Chirp Start indx >= 512 */
#define RL_RET_CODE_AFRAME_NCHIRP_INVAL_IN            (89U) /* Sub Frame NO. of unique chirps per
                                                                Burst is outside[1,512] */
#define RL_RET_CODE_AFRAME_NCHIRP_1INVAL_IN           (90U) /* Chirp used in frame is not
                                                                configured by Chirp config */
#define RL_RET_CODE_AFRAME_CHIRP_PF_INVAL_IN          (91U) /* Profie used in the frame is not
                                                                configured by profile config */
#define RL_RET_CODE_AFRAME_CHIRP_LOOPS_INVAL_IN       (92U) /* Sub Frame No. of loops is
                                                                outside[1,225] */
#define RL_RET_CODE_AFRAME_BURST_PERIOD_INVAL_IN      (93U) /* Sub Frame burst period is
                                                                outside[100us,1.342s] */
#define RL_RET_CODE_AFRAME_BURST_PER_1INVAL_IN        (94U) /* Burst ON time > Burst period */
#define RL_RET_CODE_AFRAME_BURST_STIND_INVAL_IN       (95U) /* Sub Frame Chirp start indx
                                                                offset >= 512 */
#define RL_RET_CODE_AFRAME_BURST_SIND_1INVAL_IN       (96U) /* Sub Frame Chirp start indx >= 512
                                                                or (Sub Frame Chirp start indx +
                                                                Sub Frame No. unique Chirps per
                                                                burst - 1) >= 512*/
#define RL_RET_CODE_AFRAME_NUM_BURSTS_INVAL_IN        (97U) /* Sub Frame No. bursts is
                                                                outside[1,512] */
#define RL_RET_CODE_AFRAME_BURST_LOOPS_INVAL_IN       (98U) /* Sub Frame No. outer loops is
                                                                outside[1,64] */
#define RL_RET_CODE_AFRAME_SF_PERIOD_INVAL_IN         (99U) /* Sub Frame period is
                                                                outside[100us,1.342s] */
#define RL_RET_CODE_AFRAME_SF_PERIOD_1INVAL_IN        (100U) /* Sub Frame ontime > Sub Frame period
                                                                or when test source enabled, Sub
                                                                Frame idale time < 150us */
#define RL_RET_CODE_RESERVED9                         (101U)
#define RL_RET_CODE_AFRAME_TRIG_SEL_INVAL_IN          (102U) /* Trigger select is outside[1,2] */
#define RL_RET_CODE_AFRAME_TRIG_DELAY_INVAL_IN        (103U) /* Frame trigger delay is > 100us */
#define RL_RET_CODE_AFRAME_IS_ONGOING                 (104U) /* API issued when frame is ongoing */

/*! Test source config API */
#define RL_RET_CODE_TS_POS_VECY_INVAL_IN              (105U) /* position vector x[y] < 0 */
#define RL_RET_CODE_RESERVED10                        (106U)
#define RL_RET_CODE_TS_VEL_VECXYZ_INVAL_IN            (107U) /* position vector x[x] < 5000 or
                                                                position vector x[y] < 5000 or
                                                                position vector x[x] < 5000 */
#define RL_RET_CODE_TS_SIG_LEVEL_INVAL_IN             (108U) /* SIG_LEV_VECx > 950 */
#define RL_RET_CODE_TS_RX_ANT_POS_INVAL_IN            (109U) /* RX_ANT_POS_XZ[Bytex] > 120 */
#define RL_RET_CODE_RESERVED11                        (110U)

/*! Programmable filter config API */
#define RL_RET_CODE_PROG_FILT_STARTINDX_INVALID       (111U) /* Prog. Filter coefficient start
                                                                 indx is odd number */
#define RL_RET_CODE_PROG_FILT_PROFILE_INVALID         (112U) /* Pro indx >= 4 */
#define RL_RET_CODE_PROG_FILT_UNSUPPORTED_DEV         (113U) /* API issued for non AWR1642 device*/

/*! Per chirp phase shifter API */
#define RL_RET_CODE_PERCHIRPPHSHIFT_UNSUPPORTED_DEV   (114U) /* API issued for non AWR1243 device*/
#define RL_RET_CODE_PERCHIRPPHSHIFT_STIND             (115U) /* Chirp Start indx >= 512 */
#define RL_RET_CODE_PERCHIRPPHSHIFT_ENIND             (116U) /* Chirp End indx >= 512 */
#define RL_RET_CODE_PERCHIRPPHSHIFT_WRONG_STIND       (117U) /* Chirp Start indx > End indx */

/*! Calibration config APIs */
#define RL_RET_CODE_RF_INIT_NOT_DONE                  (118U) /* Boot time calibrations are not
                                                         done so cannot run runtime calibrations */
#define RL_RET_CODE_FREQ_LIMIT_OUT_RANGE              (119U) /* Freq. is outside[76GHz,81GHz] or
                                                                 Freq. low limit > high limit */
#define RL_RET_CODE_CAL_MON_TIME_INVALID              (120U) /* CALIB_MON_TIME_UNIT <= 0 */
#define RL_RET_CODE_RUN_CAL_PERIOD_INVALID            (121U) /* CALIBRATION_ PERIODICITY = 0 */
#define RL_RET_CODE_CONT_STREAM_MODE_EN               (122U) /* API is issued when continuous
                                                                streaming mode is on */
#define RL_RET_CODE_RX_GAIN_BOOT_CAL_NOT_DONE         (123U) /* RX gain run time calibration was
                                                                requested but boot time calibration
                                                                was not performed */
#define RL_RET_CODE_LO_DIST_BOOT_CAL_NOT_DONE         (124U) /* LO distribution run time
                                                            calibration was requested but boot time
                                                            calibration was not performed */
#define RL_RET_CODE_TX_PWR_BOOT_CAL_NOT_DONE          (125U) /* TX power run time calibration was
                                                                requested but boot time calibration
                                                                was not performed */
#define RL_RET_CODE_PROG_FILTR_UNSUPPORTED_DFEMODE    (126U) /* DFE mode is pseudo real */
#define RL_RET_CODE_ADC_BITS_FULL_SCALE_REDUC_INVAL   (127U) /* FULL_SCALE_REDUCTION_FACTOR is > 0
                                                                for 16 bit ADC, or > 2 for 14 bit
                                                             ADC mode or > 4 for 12 bit ADC mode */

#define RL_RET_CODE_CH_CFG_DEV_VRNT_CASC_INVAL_IN     (130U)    /* Device variant does not allow
                                                                   cascading but API is issued to
                                                                   enable cascading mode */

/*! Loopback burst error */
#define RL_RET_CODE_INVAL_LOOPBACK_TYPE               (132U)
#define RL_RET_CODE_INVAL_LOOPBACK_BURST_IND          (133U)
#define RL_RET_CODE_INVAL_LOOPBACK_CONFIG             (134U)
#define RL_RET_CODE_DYN_CHIRP_INVAL_SEG               (135U)
#define RL_RET_CODE_DYN_PERCHIRP_PHSHFT_INVA_SEG      (136U)
#define RL_RET_CODE_INVALID_CAL_CHUNK_ID              (137U)
#define RL_RET_CODE_INVALID_CAL_CHUNK_DATA            (138U)

/*! Inter Chirp Block Control Config */
#define RL_RET_CODE_RX02_RF_TURN_OFF_TIME_INVALID     (139U) /* RX02_RF_TURN_OFF_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX13_RF_TURN_OFF_TIME_INVALID     (140U) /* RX13_RF_TURN_OFF_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX02_BB_TURN_OFF_TIME_INVALID     (141U) /* RX02_BB_TURN_OFF_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX13_BB_TURN_OFF_TIME_INVALID     (142U) /* RX13_BB_TURN_OFF_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX02_RF_PREENABLE_TIME_INVALID    (143U) /* RX02_RF_PREENABLE_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX13_RF_PREENABLE_TIME_INVALID    (144U) /* RX13_RF_PREENABLE_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX02_BB_PREENABLE_TIME_INVALID    (145U) /* RX02_BB_PREENABLE_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX13_BB_PREENABLE_TIME_INVALID    (146U) /* RX13_BB_PREENABLE_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX02_RF_TURN_ON_TIME_INVALID      (147U) /* RX02_RF_TURN_ON_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX13_RF_TURN_ON_TIME_INVALID      (148U) /* RX13_RF_TURN_ON_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX02_BB_TURN_ON_TIME_INVALID      (149U) /* RX02_BB_TURN_ON_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX13_BB_TURN_ON_TIME_INVALID      (150U) /* RX13_BB_TURN_ON_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX_LO_TURN_OFF_TIME_INVALID       (151U) /* RX_LO_TURN_OFF_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_TX_LO_TURN_OFF_TIME_INVALID       (152U) /* TX_LO_TURN_OFF_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_RX_LO_TURN_ON_TIME_INVALID        (153U) /* RX_LO_TURN_ON_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_TX_LO_TURN_ON_TIME_INVALID        (154U) /* TX_LO_TURN_ON_TIME is not
                                                                within the range [-1024, 1023] */
#define RL_RET_CODE_SUBFRAME_TRIGGER_INVALID          (155U) /* Sub frame trigger option is not
                                                                enabled but sub frame trigger API
                                                                is issued or frame is configured
                                                                for software trigger mode and
                                                                sub-frame trigger API is issued */
#define RL_RET_CODE_REGULAR_ADC_MODE_INVALID          (156U) /* Regular ADC mode is issued on a
                                                                5 MHz part variant */
#define RL_RET_CODE_CHIRP_ROW_SELECT_INVAL_IN         (159U) /* Chirp row select is not with in
                                                                the range [0x00, 0x30] */
/*! Monitoring config APIs */
#define RL_RET_CODE_DEVICE_NOT_ASILB_TYPE             (250U) /* Device type is not ASILB */
#define RL_RET_CODE_FRAME_ONGOING                     (251U) /* Fault injection API or Digital
                                                                 latent fault API is issued when
                                                                 frames are ongoing */
#define RL_RET_CODE_INVLD_REPO_MODE                   (252U) /* Invalid reporting mode */
#define RL_RET_CODE_INVLD_PROFILE_ID                  (253U) /* Configured profile ID is not
                                                                 within [0,3] */
#define RL_RET_CODE_INVLD_PROFILE                     (254U) /* Monitoring profile ID is not
                                                                 configured yet */
#define RL_RET_CODE_INVLD_EXTSIG_SETLTIME             (255U) /* Settling time is configured is
                                                                 more than 12us */
#define RL_RET_CODE_INVLD_NO_RX_ENABLED               (256U) /* None of the RXs are enabled */
#define RL_RET_CODE_INVLD_TX0_NOT_ENABLED             (257U) /* TX0 is not enabled */
#define RL_RET_CODE_INVLD_TX1_NOT_ENABLED             (258U) /* TX1 is not enabled */
#define RL_RET_CODE_INVLD_TX2_NOT_ENABLED             (259U) /* TX2 is not enabled */
#define RL_RET_CODE_MON_INVALID_RF_BIT_MASK           (260U) /* Invalid RF bit mask */
#define RL_RET_CODE_RESERVED12                        (261U)
#define RL_RET_CODE_RESERVED13                        (262U)
#define RL_RET_CODE_MON_TX_EN_CHK_FAIL                (263U) /* Monitored TX is not enabled */
#define RL_RET_CODE_MON_RX_CH_EN_CHK_FAIL             (264U) /* Monitored RX is not enabled */
#define RL_RET_CODE_MON_TX_CH_PS_LB                   (265U) /* TX selected for RX gain phase
                                                                 monitor is TX2 (Only TX0 or TX1 is
                                                                 allowed) */
#define RL_RET_CODE_INVLD_SAT_MON_SEL                 (266U) /* SAT_MON_SEL is not in [0, 3] */
#define RL_RET_CODE_INVLD_SAT_MON_PRI_SLICE_DUR       (267U) /* SAT_MON_PRIMARY_TIME_SLICE_DURATION
                                                                 is less than 0.64us or greater
                                                                 than ADC sampling time */
#define RL_RET_CODE_INVLD_SAT_MON_NUM_SLICES          (268U) /* SAT_MON_NUM_SLICES is 0 or
                                                                 greater than 127 */
#define RL_RET_CODE_INVLD_SIG_IMG_SLICENUM            (269U) /* SIG_IMG_MON_NUM_SLICES is 0 or
                                                                 greater than 127 */
#define RL_RET_CODE_INVLD_SIG_IMG_NUMSAMPPERSLICE     (270U) /* NUM_SAMPLES_ PER_PRIMARY_TIME_SLICE
                                                            is odd, or less than 4 in Complex1x
                                                            mode or less than 8 in non-Complex1x
                                                            modes or greater than NUM_ADC_SAMPLES*/
#define RL_RET_CODE_INVLD_SYNTH_L1_LIN                (271U)
#define RL_RET_CODE_INVLD_SYNTH_L2_LIN                (272U)
#define RL_RET_CODE_INVLD_SYNTH_N_LIN                 (273U)
#define RL_RET_CODE_INVLD_SYNTH_MON_START_TIME        (274U)
#define RL_RET_CODE_INVLD_SYNTH_MON_LIN_RAM_ADDR      (275U)
#define RL_RET_CODE_LDO_BYPASSED                      (279U) /* LDO fault inject is requested but
                                                                  LDOs are bypassed */
#define RL_RET_CODE_INVLD_SIG_IMG_BAND_MONTR          (280U) /* Signal and image band monitor is
                                                                not supported */
#define RL_RET_CODE_ANALOG_MONITOR_NOT_SUPPORTED      (281U)
#define RL_RET_CODE_ISSUE_TO_ENABLE_CASCASE_MODE      (282U) /* Device variant does not allow 
                                                                cascading but API is issued to
                                                                enable cascading mode */
#define RL_RET_CODE_RX_SAT_MON_NOT_SUPPORTED          (283U)

#define RL_RET_CODE_CHIRP_FAIL                        (290U) /* Monitoring chirp error */
#define RL_RET_CODE_PD_PWR_LVL                        (291U) /* Loopback power measured by PD
                                                                is below -40 dBm */
#define RL_RET_CODE_ADC_PWR_LVL                       (292U) /* ADC power is higher than 7 dBm */
#define RL_RET_CODE_NOISE_FIG_LOW                     (293U) /* Noise figure is less than 0 */
#define RL_RET_CODE_PD_CDS_ON_FAIL                    (294U) /* PD measurement with RF on is less
                                                                than with RF off */
#define RL_RET_CODE_PGA_GAIN_FAIL                     (295U) /* Incorrect PGA gain for monitoring*/

/* ADC Config API */
#define RL_RET_CODE_RX_CHAN_EN_OOR                    (1001U)  /* numADCBits out of Range */
#define RL_RET_CODE_NUM_ADC_BITS_OOR                  (1002U)  /* rxChannelEn out of Range */
#define RL_RET_CODE_ADC_OUT_FMT_OOR                   (1003U)  /* adcOutFormat out of Range */
#define RL_RET_CODE_IQ_SWAP_SEL_OOR                   (1004U)  /* sampleInterleave out of
                                                                      Range */
#define RL_RET_CODE_CHAN_INTERLEAVE_OOR               (1005U)  /* channelInterleave out of
                                                                      Range */

/* Data Path Config API */
#define RL_RET_CODE_DATA_INTF_SEL_OOR                 (1006U)  /* dataIntfSel out of Range */
#define RL_RET_CODE_DATA_FMT_PKT0_INVALID             (1007U)  /* dataTransPkt0Format
                                                                      Unsupporetd */
#define RL_RET_CODE_DATA_FMT_PKT1_INVALID             (1008U)  /* dataTransPkt1Format
                                                                      Unsupporetd */

/* Lane Enable config API */
#define RL_RET_CODE_LANE_ENABLE_OOR                   (1009U)  /* laneEnable is out of range */
#define RL_RET_CODE_LANE_ENABLE_INVALID               (1010U)  /* laneEnable is not supported */

/* Lane Clock config API */
#define RL_RET_CODE_LANE_CLK_CFG_OOR                  (1011U)  /* laneClkCfg is out of range */
#define RL_RET_CODE_LANE_CLK_CFG_INVALID              (1012U)  /* laneClkCfg is not supported */
#define RL_RET_CODE_DATA_RATE_OOR                     (1013U)  /* dataRate is out of range */

/* LVDS config API */
#define RL_RET_CODE_LANE_FMT_MAP_OOR                  (1014U)  /* laneFmtMap is out of range */
#define RL_RET_CODE_LANE_PARAM_CFG_OOR                (1015U)  /* laneParamCfg is out of range */

/* Continuous Streaming Mode API */
#define RL_RET_CODE_CONT_STREAM_MODE_OOR              (1016U)  /* contStreamMode is out of
                                                                      range */
#define RL_RET_CODE_CONT_STREAM_MODE_INVALID          (1017U)  /* contStreamMode is already
                                                                      in requested mode */

/* CSI2 Lane Config API */
#define RL_RET_CODE_LANE0_POS_POL_OOR                 (1018U)  /* lane0 pos is out of range */
#define RL_RET_CODE_LANE1_POS_POL_OOR                 (1019U)  /* lane1 pos is out of range */
#define RL_RET_CODE_LANE2_POS_POL_OOR                 (1020U)  /* lane2 pos is out of range */
#define RL_RET_CODE_LANE3_POS_POL_OOR                 (1021U)  /* lane3 pos is out of range */
#define RL_RET_CODE_CLOCK_POS_OOR                     (1022U)  /* ClockPos is out of range */

/* Frame Config Apply API */
#define RL_RET_CODE_HALF_WORDS_PER_CHIRP_OOR          (1023U)  /* adcOutSize is out of range */

/* Advanced Frame Config API */
#define RL_RET_CODE_NUM_SUBFRAMES_OOR                 (1024U)  /* numSubFrames is out of range */

#define RL_RET_CODE_SF1_TOT_NUM_CHIRPS_OOR            (1025U)  /* totNumChirps is out of range */
#define RL_RET_CODE_SF1_NUM_ADC_SAMP_OOR              (1026U)  /* numADCSamplesInPkt is out
                                                                      of range */
#define RL_RET_CODE_SF1_NUM_CHIRPS_OOR                (1027U)  /* numChirpsInPkt is out of
                                                                      range */

#define RL_RET_CODE_SF2_TOT_NUM_CHIRPS_OOR            (1028U)  /* totNumChirps is out of
                                                                      range */
#define RL_RET_CODE_SF2_NUM_ADC_SAMP_OOR              (1029U)  /* numADCSamplesInPkt is out
                                                                      of range */
#define RL_RET_CODE_SF2_NUM_CHIRPS_OOR                (1030U)  /* numChirpsInPkt is out of
                                                                      range */

#define RL_RET_CODE_SF3_TOT_NUM_CHIRPS_OOR            (1031U)  /* totNumChirps is out of
                                                                      range */
#define RL_RET_CODE_SF3_NUM_ADC_SAMP_OOR              (1032U)  /* numADCSamplesInPkt is out of
                                                                      range */
#define RL_RET_CODE_SF3_NUM_CHIRPS_OOR                (1033U)  /* numChirpsInPkt is out of
                                                                      range */

#define RL_RET_CODE_SF4_TOT_NUM_CHIRPS_OOR            (1034U)  /* totNumChirps is out of
                                                                      range */
#define RL_RET_CODE_SF4_NUM_ADC_SAMP_OOR              (1035U)  /* numADCSamplesInPkt is out of
                                                                      range */
#define RL_RET_CODE_SF4_NUM_CHIRPS_OOR                (1036U)  /* numChirpsInPkt is out of range */

#define RL_RET_CODE_MCUCLOCK_CTRL_OOR                 (1040U)  /* mcuClkOutEn is out of range */
#define RL_RET_CODE_MCUCLOCK_SRC_OOR                  (1041U)  /* mcuClkOutSrc is out of range */

#define RL_RET_CODE_PMICCLOCK_CTRL_OOR                (1042U)  /* pmicClkOutEn is out of range */
#define RL_RET_CODE_PMICCLOCK_SRC_OOR                 (1043U)  /* pmicClkOutSrc is out of range */
#define RL_RET_CODE_PMICMODE_SELECT_OOR               (1044U)  /* modeSel is out of range */
#define RL_RET_CODE_PMICFREQ_SLOPE_OOR                (1045U)  /* freqSlope is out of range */
#define RL_RET_CODE_PMICCLK_DITHER_EN_OOR             (1046U)  /* clkDitherEn is out of range */

#define RL_RET_CODE_TESTPATTERN_EN_OOR                (1047U)  /* testPatternGenEn is out of
                                                                    range */
#define RL_RET_CODE_LFAULTTEST_UNSUPPORTED_OOR        (1048U)


/*! \brief
 * mmwavelink MACRO to enable/disable logging.
 * To enable logging set this MACRO to '0' and set proper function pointer
 * dbgCb.rlPrint and debug level dbgCb.dbgLevel out of RL_DBG_LEVEL_* during rlDevicePowerOn
 */
#define RL_DISABLE_LOGGING                    1

/* mmwavelink MACROs for Error Checks */
#define RL_OSI_RET_CODE_OK                  (0)
#define RL_IF_RET_CODE_OK                   (0)

#ifdef RL_EXTENDED_MESSAGE /* build time MACRO to change message size */
/* if mmWaveLink instance is running inside xWR1443/1642 device then Max size of packet can
        be (2048 -4) bytes, where 4 bytes are reserved for mailbox header */
#define RL_MAX_SIZE_MSG                     (2044U)  /*!< This includes payload+HR+CRC */
#else
#define RL_MAX_SIZE_MSG                     (256U)  /*!< This includes payload+HR+CRC */
#endif


/** NOTE
 * When Running On MSS:
 *         RL_DEVICE_INDEX_INTERNAL_BSS: for radarSS communication
 *         RL_DEVICE_INDEX_INTERNAL_DSS_MSS: for DSS communication
 *         RL_DEVICE_INDEX_INTERNAL_HOST: for SPI communication
 * When Running On DSS:
 *         RL_DEVICE_INDEX_INTERNAL_BSS: for radarSS communication
 *         RL_DEVICE_INDEX_INTERNAL_DSS_MSS: for MSS communication
 */

#define RL_DEVICE_MAP_NATIVE                        (0U)    /*!< When calling native API */
#define RL_DEVICE_MAP_CASCADED_1                    (1U)    /*!< AWR1243 Cascaded Device Id 1 */
#define RL_DEVICE_MAP_CASCADED_2                    (2U)    /*!< AWR1243 Cascaded Device Id 2 */
#define RL_DEVICE_MAP_CASCADED_3                    (4U)    /*!< AWR1243 Cascaded Device Id 3 */
#define RL_DEVICE_MAP_CASCADED_4                    (8U)    /*!< AWR1243 Cascaded Device Id 4 */

/* AWR1243 Device Map - Max Cascading */
#define RL_DEVICE_MAP_CASCADED_ALL                  (RL_DEVICE_MAP_CASCADED_1 |\
                                                     RL_DEVICE_MAP_CASCADED_2 |\
                                                     RL_DEVICE_MAP_CASCADED_3 |\
                                                     RL_DEVICE_MAP_CASCADED_4)

/* Device Index for SubSystem */
#define RL_DEVICE_INDEX_INTERNAL_BSS                (0U)    /*!< xWR1443/xWR1642/xWR1843 radarSS */
#define RL_DEVICE_INDEX_INTERNAL_DSS_MSS            (1U)    /*!< xWR1642/xWR1843 DSS/MSS */
#define RL_DEVICE_INDEX_INTERNAL_HOST               (2U)    /*!< xWR1443/xWR1642/xWR1843 External
                                                                 Host */

#define RL_DEVICE_MAP_INTERNAL_BSS       (RL_DEVICE_MAP_CASCADED_1) /*!< xWR1443/xWR1642/xWR1843 
                                                                         radarSS */
#define RL_DEVICE_MAP_INTERNAL_DSS_MSS   (RL_DEVICE_MAP_CASCADED_2) /*!< xWR1642/xWR1843 DSS/MSS */
#define RL_DEVICE_MAP_INTERNAL_HOST      (RL_DEVICE_MAP_CASCADED_3) /*!< xWR1443/xWR1642/xWR1843
                                                                         External Host */

#define RL_DEVICE_CONNECTED_MAX                     (4U)

/*! \brief
* mmWaveLink CRC Types
*/
#define RL_CRC_TYPE_16BIT_CCITT             (0U)        /*!< CCITT */
#define RL_CRC_TYPE_32BIT                   (1U)        /*!< ISO 3309, Ethernet */
#define RL_CRC_TYPE_64BIT_ISO               (2U)        /*!< 64 Bit ISO */
#define RL_CRC_TYPE_NO_CRC                  (3U)        /*!< CRC Disabled */

/*! \brief
* mmWaveLink Execution Home type
*/
#define RL_PLATFORM_HOST                     (0x0U)     /*!< running on Ext. Host */
#define RL_PLATFORM_MSS                      (0x1U)     /*!< running on MSS core */
#define RL_PLATFORM_DSS                      (0x2U)     /*!< running on DSS core */

/*! \brief
* mmWave Device Types
*/
#define RL_AR_DEVICETYPE_12XX                (0x0U)     /*!< mmWavelink device 12XX */
#define RL_AR_DEVICETYPE_14XX                (0x1U)     /*!< mmWavelink device 14XX */
#define RL_AR_DEVICETYPE_16XX                (0x2U)     /*!< mmWavelink device 16XX */
#define RL_AR_DEVICETYPE_18XX                (0x3U)     /*!< mmWavelink device 18XX */
#define RL_AR_DEVICETYPE_68XX                (0x4U)     /*!< mmWavelink device 68XX */

/*! \brief
* mmWaveLink Debug Levels
*/
#define RL_DBG_LEVEL_NONE                    ((rlUInt8_t)0U)
#define RL_DBG_LEVEL_ERROR                   ((rlUInt8_t)1U)
#define RL_DBG_LEVEL_WARNING                 ((rlUInt8_t)2U)
#define RL_DBG_LEVEL_INFO                    ((rlUInt8_t)3U)
#define RL_DBG_LEVEL_DEBUG                   ((rlUInt8_t)4U)
#define RL_DBG_LEVEL_VERBOSE                 ((rlUInt8_t)5U)

/*! \brief
* GPADC sensors macros
*/
#define RL_SENSOR_ANTENA_ONE                    (0U)
#define RL_SENSOR_ANTENA_TWO                    (1U)
#define RL_SENSOR_ANTENA_THREE                  (2U)
#define RL_SENSOR_ANTENA_FOUR                   (3U)
#define RL_SENSOR_IFORCE                        (4U)
#define RL_SENSOR_VSENSE                        (5U)
#define RL_SENSOR_IFORCEBUF                     (6U)
#define RL_SENSOR_RESERVED0                     (7U)
#define RL_SENSOR_RESERVED1                     (8U)
#define RL_SENSOR_RESERVED2                     (9U)
#define RL_SENSOR_RESERVED3                     (10U)
#define RL_SENSOR_RESERVED4                     (11U)
#define RL_MAX_GPADC_SENSORS                    (12U)

/*! \brief
* Swap 2 half words in 32 bit Integer. Required for Big Endian Host
*/
#define RL_SWAP_32(x) (((x) & 0x0000FFFFU)<<16U)|(((x) & 0xFFFF0000U)>>16U);

/******************************************************************************
 * TYPE-DEFINE STRUCT/ENUM/UNION DEFINITIONS
 ******************************************************************************
 */

/* DesignId : MMWL_DesignId_001 */
/* Requirements : AUTORADAR_REQ-697, AUTORADAR_REQ-698, AUTORADAR_REQ-699, AUTORADAR_REQ-700,
                  AUTORADAR_REQ-701, AUTORADAR_REQ-702, AUTORADAR_REQ-703, AUTORADAR_REQ-704,
                  AUTORADAR_REQ-705, AUTORADAR_REQ-706, AUTORADAR_REQ-830, AUTORADAR_REQ-831,
                  AUTORADAR_REQ-832, AUTORADAR_REQ-888, AUTORADAR_REQ-889, AUTORADAR_REQ-890
*/

/*! \brief
* mmWaveLink API return type
*/
typedef rlInt32_t   rlReturnVal_t;

/*! \brief
* mmWaveLink Support CRC type
*/
typedef rlUInt8_t   rlCrcType_t;

/* Function pointers for spawn task function and event handlers*/

/*! \brief
* mmWaveLink Spawn Task Function
*/
typedef void (*RL_P_OSI_SPAWN_ENTRY)(const void* pValue);

/*! \brief
* mmWaveLink Event Handler callback
*/
typedef void (*RL_P_EVENT_HANDLER)(rlUInt8_t deviceIndex, void* pValue);

/*! \brief
* Communication interface(SPI, MailBox, UART etc) callback functions
*/
typedef struct rlComIfCbs
{
    /** @fn rlComIfHdl_t (*rlComIfOpen)(rlUInt8_t deviceIndex, rlUInt32_t flags)
    *
    *...@brief Open Communication interface
    *   @param[in] deviceIndex - Communication Interface to Open
    *   @param[in] flags - Flags to configure the interface
    *
    *   @return rlComIfHdl_t Handle to access the communication interface
    *
    *   Open Communication interface
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-785 */
    rlComIfHdl_t (*rlComIfOpen)(rlUInt8_t deviceIndex, rlUInt32_t flags);

    /** @fn rlInt32_t (*rlComIfRead)(rlComIfHdl_t fd, rlUInt8_t *pBuff, rlUInt16_t len)
    *
    *   @brief Read Data from Communication interface
    *   @param[in] fd - Handle to access the communication interface
    *   @param[out] pBuff - Buffer to store data from communication interface
    *   @param[in] len - Read size in bytes
    *
    *   @return rlInt32_t Length of received data
    *
    *   Read Data from Communication interface
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-785 */
    rlInt32_t (*rlComIfRead)(rlComIfHdl_t fd, rlUInt8_t *pBuff, rlUInt16_t len);

    /** @fn rlInt32_t (*rlComIfWrite)(rlComIfHdl_t fd, rlUInt8_t *pBuff, rlUInt16_t len)
    *
    *   @brief Write Data over Communication interface
    *   @param[in] fd - Handle to access the communication interface
    *   @param[in] pBuff - Buffer containing data to write over communication interface
    *   @param[in] len - write size in bytes
    *
    *   @return rlInt32_t Length of data sent
    *
    *   Write Data over Communication interface
    */
    /* DesignId :  */
    /* Requirements : AUTORADAR_REQ-785 */
    rlInt32_t (*rlComIfWrite)(rlComIfHdl_t fd, rlUInt8_t *pBuff, rlUInt16_t len);

    /** @fn rlInt32_t (*rlComIfClose)(rlComIfHdl_t fd)
    *
    *   @brief Close the Communication interface
    *   @param[in] fd - Handle to access the communication interface
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Close the Communication interface
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-785 */
    rlInt32_t (*rlComIfClose)(rlComIfHdl_t fd);
}rlComIfCbs_t;

/*! \brief
* OS mutex callback functions
*/
typedef struct rlOsiMutexCbs
{
    /** @fn rlInt32_t (*rlOsiMutexCreate)(rlOsiMutexHdl_t* mutexHdl, rlInt8_t* name)
    *
    *   @brief Create Mutex Object
    *   @param[in] name      - Name to associate with Mutex Object
    *   @param[out] mutexHdl - Pointer to Mutex object
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Create Mutex Object
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiMutexCreate)(rlOsiMutexHdl_t* mutexHdl, rlInt8_t* name);

    /** @fn rlInt32_t (*rlOsiMutexLock)(rlOsiMutexHdl_t* mutexHdl, rlOsiTime_t timeout)
    *
    *   @brief Lock Mutex Object
    *   @param[in] mutexHdl - Pointer to Mutex object
    *   @param[in] timeout  - Maximum Time to wait for Mutex Lock
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Lock Mutex Object
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiMutexLock)(rlOsiMutexHdl_t* mutexHdl, rlOsiTime_t timeout);

    /** @fn rlInt32_t (*rlOsiMutexUnLock)(rlOsiMutexHdl_t* mutexHdl)
    *
    *   @brief Unlock Mutex Object
    *   @param[in] mutexHdl - Pointer to Mutex object
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Unlock Mutex Object
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiMutexUnLock)(rlOsiMutexHdl_t* mutexHdl);

    /** @fn rlInt32_t (*rlOsiMutexDelete)(rlOsiMutexHdl_t* mutexHdl)
    *
    *   @brief Destroy Mutex Object
    *   @param[in] mutexHdl - Pointer to Mutex object
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Destroy Mutex Object
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiMutexDelete)(rlOsiMutexHdl_t* mutexHdl);
}rlOsiMutexCbs_t;

/*! \brief
* OS semaphore callback functions
*/
typedef struct rlOsiSemCbs
{
    /** @fn rlInt32_t (*rlOsiSemCreate)(rlOsiSemHdl_t* semHdl, rlInt8_t* name)
    *
    *   @brief Create Semaphore Object
    *   @param[in] name      - Name to associate with Sem Object
    *   @param[out] semHdl   - Pointer to Sem object
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Create Semaphore Object
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiSemCreate)(rlOsiSemHdl_t* semHdl, rlInt8_t* name);

    /** @fn rlInt32_t (*rlOsiSemWait)(rlOsiSemHdl_t* semHdl, rlOsiTime_t timeout)
    *
    *   @brief Wait for Semaphore
    *   @param[in] semHdl   - Pointer to Sem object
    *   @param[in] timeout  - Maximum Time to wait for Semaphore
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Wait for Semaphore
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiSemWait)(rlOsiSemHdl_t* semHdl, rlOsiTime_t timeout);

    /** @fn rlInt32_t (*rlOsiSemSignal)(rlOsiSemHdl_t* semHdl)
    *
    *   @brief Release Semaphore
    *   @param[in] semHdl - Pointer to Sem object
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Release Semaphore
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiSemSignal)(rlOsiSemHdl_t* semHdl);

    /** @fn rlInt32_t (*rlOsiSemDelete)(rlOsiSemHdl_t* semHdl)
    *
    *   @brief Destroy Semaphore Object
    *   @param[in] semHdl - Pointer to Sem object
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Destroy Semaphore Object
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiSemDelete)(rlOsiSemHdl_t* semHdl);
}rlOsiSemCbs_t;

/*! \brief
* OS message queue/Spawn callback functions
*/
typedef struct rlOsiMsgQCbs
{
    /** @fn rlInt32_t (*rlOsiSpawn)(RL_P_OSI_SPAWN_ENTRY pEntry, void* pValue, rlUInt32_t flags)
    *
    *   @brief Calls a function in a different context
    *   @param[in] pEntry - Pointer to Entry Function
    *   @param[in] pValue - Pointer to data passed to function
    *   @param[in] flags  - Flag to indicate preference
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Calls a function in a different context. mmWaveLink Driver Interrupt handler function
    *   invokes this interface to switch context so that Interrupt Service Routine is executed
    *   immediately.
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-784 */
    rlInt32_t (*rlOsiSpawn)(RL_P_OSI_SPAWN_ENTRY pEntry, const void* pValue, rlUInt32_t flags);
}rlOsiMsgQCbs_t;


/*! \brief
* OS services callback functions
*/
typedef struct rlOsiCbs
{
    /*! \brief
    * Mutex callback functions.
    */
    rlOsiMutexCbs_t mutex;
    /*! \brief
    * Semaphore callback functions.
    */
    rlOsiSemCbs_t sem;
    /*! \brief
    * OS message queue/Spawn callback functions.
    */
    rlOsiMsgQCbs_t queue;
}rlOsiCbs_t;

/*! \brief
* mmWaveLink Asynchronous event callback function
*/
typedef struct rlEventCbs
{
    /** @fn void (*rlAsyncEvent)(rlUInt8_t devIndex, rlUInt16_t sbId, rlUInt16_t sbLen,
    *                            rlUInt8_t *payload)
    *
    *   @brief Reports Asynchronous events from mmwave radar device such as
    *   device status, exceptions etc
    *   @param[in] devIndex - Device Index to identify source of event
    *   @param[in] subId -  Sub-block Id
    *   @param[in] subLen -  Sub-block data length
    *   @param[in] payload - Sub-block data starting memory address
    *
    *   Reports Asynchronous events from mmwave radar device such as
    *   device status, exceptions etc
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-783 */
    void (*rlAsyncEvent)(rlUInt8_t devIndex, rlUInt16_t subId, rlUInt16_t subLen,
                                                                rlUInt8_t *payload);
}rlEventCbs_t;

/*! \brief
* mmWaveLink Timer callback functions
*/
typedef struct rlTimerCbs
{
    rlInt32_t (*rlDelay)(rlUInt32_t delay);
}rlTimerCbs_t;

/*! \brief
* mmWaveLink callback functions for Command parser
*/
typedef struct rlCmdParserCbs
{
    rlInt32_t (*rlCmdParser)(rlUInt8_t rxMsgClass, rlInt32_t inVal);
    rlInt32_t (*rlPostCnysStep)(rlUInt8_t devIndex);
}rlCmdParserCbs_t;

/*! \brief
* mmWaveLink CRC callback function
*/
typedef struct rlCrcCbs
{
    /** @fn rlInt32_t (*rlComputeCRC)(rlUInt8_t* data, rlUInt32_t dataLen, rlUInt8_t crcType,
    *                  rlUInt8_t* crc)
    *
    *   @brief Compute CRC on the input message
    *   @param[in] data - input message
    *   @param[in] dataLen - size of input message in bytes
    *   @param[in] crcType  - CRC type (0:RL_CRC_TYPE_16BIT_CCITT, 1:RL_CRC_TYPE_32BIT,
    *                         2:RL_CRC_TYPE_64BIT_ISO)
    *   @param[out] crc   - Computed CRC
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Compute CRC on the input message
    */
    /* DesignId :  */
    /* Requirements :  */
    rlInt32_t (*rlComputeCRC)(rlUInt8_t* data, rlUInt32_t dataLen, rlUInt8_t crcType,
                              rlUInt8_t* crc);
}rlCrcCbs_t;

/*! \brief
* mmWaveLink Device Control, Interrupt callback functions
*/
typedef struct rlDeviceCtrlCbs
{
    /** @fn rlInt32_t (*rlDeviceEnable)(rlUInt8_t deviceIndex)
    *
    *   @brief Bring mmWave radar device out of Reset
    *   @param[in] deviceIndex -  Index of the device to be enabled
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Bring mmWave radar device out of Reset. Implement this function to assert the nReset
    *   Pin in mmWave device. Optionally It might require to assert Sense on Power(SOP) Pins
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-786 */
    rlInt32_t (*rlDeviceEnable)(rlUInt8_t deviceIndex);

    /** @fn rlInt32_t (*rlDeviceDisable)(rlUInt8_t deviceIndex)
    *
    *   @brief Power off mmWave radar device
    *   @param[in] deviceIndex -  Index of the device to be disbaled
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Power off mmWave radar device. Implement this function to de-assert the Reset
    *   Pin in mmWave device
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-786 */
    rlInt32_t (*rlDeviceDisable)(rlUInt8_t deviceIndex);

    /** @fn void (*rlDeviceMaskHostIrq)(rlComIfHdl_t fd)
    *
    *   @brief Masks Host Interrupt
    *   @param[in] fd -  Handle of the device for which interrupt need to be masked
    *
    *   Masks Host Interrupt. If GPIO Interrupt is Level Triggered,
    *   host need to mask the interrupt until the interrupt is serviced
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-787 */
    void (*rlDeviceMaskHostIrq)(rlComIfHdl_t fd);

    /** @fn void (*rlDeviceUnMaskHostIrq)(rlComIfHdl_t fd)
    *
    *   @brief Unmask Host Interrupt
    *   @param[in] fd -  Handle of the device for which interrupt need to be unmasked
    *
    *   Unmask Host Interrupt. If GPIO Interrupt is Level Triggered,
    *   host need to unmask the interrupt once interrupt is processed
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-787 */
    void (*rlDeviceUnMaskHostIrq)(rlComIfHdl_t fd);

    /** @fn rlInt32_t (*rlDeviceWaitIrqStatus)(rlComIfHdl_t fd, rlUInt8_t highLow)
    *
    *   @brief Polls Host Interrupt Status
    *   @param[in] fd -  Handle of the device for which interrupt need to be polled
    *   @param[in] highLow - Wait for IRQ Level(high/low)
    *
    *   @return rlInt32_t IRQ Line Low - 0, IRQ Line High - 1
    *
    *   mmWave Radar device asserts host IRQ pin to get Host attention. After receiving host
    *   interrupt, host polls Host Interrupt Status, Low on Host IRQ indicate that mmWave device
    *   has written data on communication Interface. This callback should Wait for the IRQ status.
    *   The function waits for the IRQ Level(low/high) based on second argument returns once the
    *   IRQ Level occurs.
    */
    /* DesignId :MMWL_DesignId_004  */
    /* Requirements : AUTORADAR_REQ-787 */
    rlInt32_t (*rlDeviceWaitIrqStatus)(rlComIfHdl_t fd, rlUInt8_t highLow);

    /** @fn rlUInt16_t (*rlCommIfAssertIrq)(rlUInt8_t highLow)
    *
    *   @brief It assert/de-assert Host IRQ hig/low on MSS to Host for SPI communication
    *   @param[in] highLow - High/low value for Host IRQ
    *
    *   @return rlUInt16_t Success - 0, Failure - Error code
    *
    *   It assert/de-assert Host IRQ hig/low on MSS to Host for SPI communication
    */
    /* DesignId : MMWL_DesignId_004 */
    /* Requirements : AUTORADAR_REQ-787 */
    rlUInt16_t (*rlCommIfAssertIrq)(rlUInt8_t highLow);

    /** @fn rlInt32_t (*rlRegisterInterruptHandler)(rlUInt8_t deviceIndex,
    *                                               RL_P_EVENT_HANDLER pHandler, void* pValue)
    *
    *   @brief Register Host Interrupt Handler
    *   @param[in] deviceIndex - Device Index to identify source of Host Interrupt
    *   @param[in] pHandler - Interrupt Handler routine
    *   @param[in] pValue   - To Pass any additional data
    *
    *   @return rlInt32_t Success - 0, Failure - Error code
    *
    *   Register Host Interrupt Handler. The Application should store this function handler
    *   and invoke when it receives host Interrupt. The application need to enable GPIO interrupt
    *   in this callback. Event Handler Callback doesn't process the interrupt in the same context
    *   so it is safe to call this handler from ISR directly.
    */
    /* DesignId : MMWL_DesignId_026 */
    /* Requirements : AUTORADAR_REQ-777 */
    rlInt32_t (*rlRegisterInterruptHandler)(rlUInt8_t deviceIndex,
                                            RL_P_EVENT_HANDLER pHandler, void* pValue);
}rlDeviceCtrlCbs_t;
/*! \brief
* mmWaveLink print function prototype
*/
typedef rlInt32_t (*rlPrintFptr)(const rlInt8_t* format, ...);

/*! \brief
* mmWaveLink debug callback structure
*/
typedef struct rlDbgCb
{
    /** @fn rlInt32_t (*rlPrint)(const rlInt8_t* format, ...)
    *
    *   @brief Print input message as per the format in the input arguments
    *   @param[in] format  - Format of message and arguments to be printed
    *   @param[in] ...     - Multiple input arguments to be printed
    *
    *   @return rlInt32_t Success- Length of the message written in user's output console in bytes
    *                     Failure- Negative value
    *
    *   Print input message as per the format in the input arguments
    */
    /* DesignId :  */
    /* Requirements :  */
    rlPrintFptr rlPrint;
    /**
     * @brief User needs to set debug level such as error, warning, debug, verbose
     */
    rlUInt8_t   dbgLevel;
}rlDbgCb_t;

/*! \brief
* mmWaveLink client callback structure
*/
typedef struct rlClientCbs
{
    /**
     * @brief  Comunication Interface Callback
     */
    rlComIfCbs_t       comIfCb;
    /**
     * @brief  Operating System Callback
     */
    rlOsiCbs_t         osiCb;
    /**
     * @brief  Event Callback, required to receive notification
     */
    rlEventCbs_t       eventCb;
    /**
     * @brief  Device Control Callback
     */
    rlDeviceCtrlCbs_t  devCtrlCb;
    /**
     * @brief  Timer Callback, required when ACK is enabled
     */
    rlTimerCbs_t       timerCb;
    /**
     * @brief Call back for parsing the Command received at MSS from the Host
                TI Internal Use only
     */
    rlCmdParserCbs_t   cmdParserCb;
    /**
     * @brief  CRC Callback, required when CRC is enabled
     */
    rlCrcCbs_t         crcCb;
    /**
     * @brief  CRC Types rlCrcType_t 16/32/64
     */
    rlCrcType_t        crcType;
    /**
     * @brief  ACK wait timeout in Milliseconds, 0 - No ACK
               Configuration of the timeout should consider interrupt
               latency in the processor.
     */
    rlUInt32_t         ackTimeout;
    /**
     * @brief  0x0: mmWaveLink runs on Ext Host, \n
               0x1: mmWaveLink runs on MSS, \n
               0x2: mmWaveLink runs on DSS  \n
     */
    rlUInt8_t          platform;
    /**
     * @brief  xWR1243 + HOST = 0x0, xWR1443 MSS = 0x1, xWR1642 MSS/DSS = 0x2, \n
               xWR1843 MSS/DSS = 0x3, xWR6843 MSS/DSS = 0x4 \n
     */
    rlUInt8_t          arDevType;
    /**
     * @brief  Debug Callback, required to receive Debug information
     */
    rlDbgCb_t          dbgCb;
}rlClientCbs_t;

/*! \brief
* mmWaveLink Init Complete data structure for event RL_DEV_AE_MSSPOWERUPDONE_SB
* @note : The functional APIs shall be sent to radar device only after receiving 
*         RL_DEV_AE_MSSPOWERUPDONE_SB Async-event after power cycle.
*/
typedef struct rlInitComplete
{
    /**
    * @brief  masterSS powerup time, 1LSB = 5ns
    */
    rlUInt32_t powerUpTime;
    /**
     * @brief  masterSS Bootup Status over SPI, 0 - PASS, 1- Fail \n
                Bit   Error-description                                    \n
                0     certificate authentication failure                   \n
                1     certificate parser failure                           \n
                2     Rprc image1 authentication failure                   \n
                3     Rprc image2 authentication failure                   \n
                4     Rprc image3 authentication failure                   \n
                5     Rprc header not found                                \n
                6     Meta header not found                                \n
                7     S/W anti roll back check failure                     \n
                8     Efuse integrity failure                              \n
                9     certificate field validity failure                   \n
                10     certificate field invalid authentication key index  \n
                11     certificate field invalid hash type                 \n
                12     certificate field invalid subsystem                 \n
                13     certificate field invalid decrypt key index         \n
                14     certificate field check efuse mismatch              \n
                15     certificate field check 1 efuse mismatch            \n
                16     certificate field check 2 efuse mismatch            \n
                17     certificate field invalid subsystem bank allocation \n
                18     certificate field invalid total banks allocation    \n
                19     Rprc parser file length mismatch                    \n
                20     Rprc parser MSS file offset mismatch                \n
                21     Rprc parser BSS file offset mismatch                \n
                22     Rprc parser DSS file offset mismatch                \n
                23     certificate field invalid decrypt key               \n
                24     certificate field invalid authentication key        \n
                25     HS device certificate not present                   \n
                26     Test port enabling failed                           \n
                27     Shared memory allocation failed                     \n
                28     MSS image not found                                 \n
                29     Meta header num files error                         \n
                30     Meta header CRC failure                             \n
                63:31  RESERVED
     */
    rlUInt64_t powerUpStatus;
    /**
    * @brief  masterSS Boot Test Status, 1 - PASS, 0 - FAIL
                Bit      Status Information                      \n
                0        MibSPI self-test                        \n
                1        DMA self-test                           \n
                2        Watchdog self-test                      \n
                3        RTI self-test                           \n
                4        ESM self-test                           \n
                5        EDMA self-test                          \n
                6        CRC self-test                           \n
                7        VIM self-test                           \n
                8        MPU self-test                           \n
                9        Mailbox self-test                       \n
                10       LVDS pattern generation test            \n
                11       CSI2 pattern generation test            \n
                12       NERROR generation test                  \n
                13       MibSPI single bit error test            \n
                14       MibSPI double bit error test            \n
                15       DMA Parity error test                   \n
                16       TCMA RAM single bit error test          \n
                17       TCMB RAM single bit error test          \n
                18       TCMA RAM double bit error test          \n
                19       TCMB RAM double bit error test          \n
                20       TCMA RAM parity error test              \n
                21       TCMB RAM parity error test              \n
                22       VIM lockstep test                       \n
                23       CCM R4 lockstep test                    \n
                24       DMA MPU region test                     \n
                25       MSS Mailbox single bit error test       \n
                26       MSS Mailbox double bit error test       \n
                27       BSS Mailbox single bit error test       \n
                28       BSS Mailbox double bit error test       \n
                29       EDMA MPU test                           \n
                30       EDMA parity test                        \n
                31       CSI2 parity test                        \n
                32       PBIST (VIM RAM/TCM RAM/MibSPI SRAM/Mailbox/EDMA/DMA/CSI2) \n
                33       LBIST (VIM/CR4)                         \n
                63:34    RESERVED
    */
    rlUInt64_t bootTestStatus;
}rlInitComplete_t;

/*! \brief
* mmWaveLink RF Start Complete data structure for event RL_DEV_AE_RFPOWERUPDONE_SB
*/
typedef struct rlStartComplete
{
    /**
     * @brief  radarSS Boot Status, 1 - PASS, 0 - FAIL  \n
               Bit Status Information                   \n
               0    ROM CRC check                       \n
               1    CR4 and VIM lockstep test           \n
               2    RESERVED                            \n
               3    VIM test                            \n
               4    STC test of diagnostic              \n
               5    CR4 STC                             \n
               6    CRC test                            \n
               7    RAMPGEN memory ECC test             \n
               8    DFE Parity test                     \n
               9    DFE memory ECC                      \n
               10   RAMPGEN lockstep test               \n
               11   FRC lockstep test                   \n
               12   DFE memory PBIST                    \n
               13   RAMPGEN memory PBIST                \n
               14   PBIST test                          \n
               15   WDT test                            \n
               16   ESM test                            \n
               17   DFE STC                             \n
               18   RESERVED                            \n
               19   ATCM, BTCM ECC test                 \n
               20   ATCM, BTCM parity test              \n
               21   RESERVED                            \n
               22   RESERVED                            \n
               23   RESERVED                            \n
               24   FFT test                            \n
               25   RTI test                            \n
               26   PCR test                            \n
               27-31 RESERVED
     */
    rlUInt32_t status;
    /**
     * @brief  radarSS powerup time, 1LSB = 5ns
     */
    rlUInt32_t powerUpTime;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved0;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved1;
}rlStartComplete_t;


/*! \brief
 * Structure to hold the MSS ESM Fault data strucutre for event RL_DEV_AE_MSS_ESMFAULT_SB
 */
typedef struct rlMssEsmFault
{
    /**
     * @brief  Bits Definition
                 0  NERROR in sync \n
                 1  RESERVED \n
                 2  DMA MPU Region tests \n
                 3  DMA Parity error \n
                 4  RESERVED \n
                 5  RESERVED \n
                 6  Access error interrupt from FFT ACC \n
                 7  CSI TX FIFO Parity Err \n
                 8  TPCC parity error \n
                 9  CBUF ECC single bit error \n
                 10 RESERVED \n
                 11 RESERVED \n
                 12 RESERVED \n
                 13 Error response from the Peripheral when a DMA transfer is done \n
                 14 RESERVED \n
                 15 VIM RAM double bit errors \n
                 16 RESERVED \n
                 17 MibSPI double bit error test \n
                 18 ECC error on CBUFF \n
                 19 RESERVED \n
                 20 VIM RAM single bit errors \n
                 21 RESERVED \n
                 22 RESERVED \n
                 23 RESERVED \n
                 24 RESERVED \n
                 25 MibSPI single bit error test \n
                 26 TCMB RAM single bit errors \n
                 27 STC error \n
                 28 TCMB RAM single bit errors \n
                 29 TPTC0 read to protected memory \n
                 30 DCC compare error \n
                 31 CR4F self-test error.(test of error path by error forcing)  \n
     */
    rlUInt32_t esmGrp1Err;
    /**
     * @brief  Bits Definition \n
                 0  TCMA RAM single bit errors \n
                 1  RESERVED \n
                 2  RESERVED \n
                 3  TPTC0 write to protected memory \n
                 4  RESERVED \n
                 5  RESERVED \n
                 6  Access error interrupt from FFT ACC \n
                 7  VIM Self-Test Error \n
                 8  RESERVED \n
                 9  RESERVED \n
                 10 RESERVED \n
                 11 RESERVED \n
                 12 QSPI not able to perform the Write to FLASH \n
                 13 RESERVED \n
                 14 RESERVED \n
                 15 RESERVED \n
                 16 RESERVED \n
                 17 RESERVED \n
                 18 RESERVED \n
                 19 RESERVED \n
                 20 RESERVED \n
                 21 RESERVED \n
                 22 RESERVED \n
                 23 RESERVED \n
                 24 RESERVED \n
                 25 RESERVED \n
                 26 radarSS Mailbox single bit errors \n
                 27 radarSS Mailbox double bit errors \n
                 28 MSS Mailbox single bit errors \n
                 29 MSS Mailbox double bit errors \n
                 30 RESERVED \n
                 31 RESERVED \n
     */
    rlUInt32_t esmGrp2Err;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
}rlMssEsmFault_t;

/*! \brief
 * Structure to hold the MSS Boot error status data strucutre when booted over SPI
 * for event RL_DEV_AE_MSS_BOOTERRSTATUS_SB
 * @note : The functional APIs shall be sent to radar device only after receiving
 *         RL_DEV_AE_MSS_BOOTERRSTATUS_SB Async-event after power-cycle.
 */
typedef struct rlMssBootErrStatus
{
    /**
    * @brief  masterSS powerup time, 1LSB = 5ns
    */
    rlUInt32_t powerUpTime;
    /**
    * @brief  masterSS Bootup Status over SPI, 0 - PASS, 1- Fail \n
    *          Bit    Status Information                                  \n
               0      certificate authentication failure                  \n
               1      certificate parser failure                          \n
               2      Rprc image1 authentication failure                  \n
               3      Rprc image2 authentication failure                  \n
               4      Rprc image3 authentication failure                  \n
               5      Rprc header not found                               \n
               6      Meta header not found                               \n
               7      S/W anti roll back check failure                    \n
               8      Efuse integrity failure                             \n
               9      certificate field validity failure                  \n
               10     certificate field invalid authentication key index  \n
               11     certificate field invalid hash type                 \n
               12     certificate field invalid subsystem                 \n
               13     certificate field invalid decrypt key index         \n
               14     certificate field check efuse mismatch              \n
               15     certificate field check 1 efuse mismatch            \n
               16     certificate field check 2 efuse mismatch            \n
               17     certificate field invalid subsystem bank allocation \n
               18     certificate field invalid total banks allocation    \n
               19     Rprc parser file length mismatch                    \n
               20     Rprc parser MSS file offset mismatch                \n
               21     Rprc parser BSS file offset mismatch                \n
               22     Rprc parser DSS file offset mismatch                \n
               23     certificate field invalid decrypt key               \n
               24     certificate field invalid authentication key        \n
               25     HS device certificate not present                   \n
               26     Test port enabling failed                           \n
               27     Shared memory allocation failed                     \n
               28     MSS image not found                                 \n
               29     Meta header num files error                         \n
               30     Meta header CRC failure                             \n
               63:31  RESERVED                                            \n
    */
    rlUInt64_t powerUpStatus;
    /**
    * @brief  masterSS Boot Test Status, 1 - PASS, 0 - FAIL               \n
                Bit      Status Information                               \n
                0        MibSPI self-test                                 \n
                1        DMA self-test                                    \n
                2        Watchdog self-test                               \n
                3        RTI self-test                                    \n
                4        ESM self-test                                    \n
                5        EDMA self-test                                   \n
                6        CRC self-test                                    \n
                7        VIM self-test                                    \n
                8        MPU self-test                                    \n
                9        Mailbox self-test                                \n
                10       LVDS pattern generation test                     \n
                11       CSI2 pattern generation test                     \n
                12       NERROR generation test                           \n
                13       MibSPI single bit error test                     \n
                14       MibSPI double bit error test                     \n
                15       DMA Parity error test                            \n
                16       TCMA RAM single bit error test                   \n
                17       TCMB RAM single bit error test                   \n
                18       TCMA RAM double bit error test                   \n
                19       TCMB RAM double bit error test                   \n
                20       TCMA RAM parity error test                       \n
                21       TCMB RAM parity error test                       \n
                22       VIM lockstep test                                \n
                23       CCM R4 lockstep test                             \n
                24       DMA MPU region test                              \n
                25       MSS Mailbox single bit error test                \n
                26       MSS Mailbox double bit error test                \n
                27       BSS Mailbox single bit error test                \n
                28       BSS Mailbox double bit error test                \n
                29       EDMA MPU test                                    \n
                30       EDMA parity test                                 \n
                31       CSI2 parity test                                 \n
                32       PBIST (VIM RAM/TCM RAM/MibSPI SRAM/Mailbox/EDMA/DMA/CSI2) \n
                33       LBIST (VIM/CR4)                                  \n
                63:34    RESERVED
    */
    rlUInt64_t bootTestStatus;
}rlMssBootErrStatus_t;

/*! \brief
 * Structure to hold the test status report of the latent fault tests data strucutre
 * for event RL_DEV_AE_MSS_LATENTFLT_TEST_REPORT_SB
 */
typedef struct rlMssLatentFaultReport
{
    /**
     * @brief  1 - PASS, 0 - FAIL \n
                Bits Definition \n
                0  MibSPI self-test \n
                1  DMA self-test \n
                2  Reserved \n
                3  RTI self-test \n
                4  ESM self-test \n
                5  EDMA self-test \n
                6  CRC self-test \n
                7  VIM self-test \n
                8  Reserved \n
                9  Mailbox self-test \n
                10 LVDS pattern generation test \n
                11 CSI2 pattern generation test \n
                12 Generating NERROR \n
                13 MibSPI single bit error test \n
                14 MibSPI double bit error test \n
                15 DMA Parity error \n
                16 TCMA RAM single bit errors \n
                17 TCMB RAM single bit errors \n
                18 TCMA RAM double bit errors \n
                19 TCMB RAM double bit errors \n
                20 TCMA RAM parity errors. \n
                21 TCMB RAM parity errors. \n
                22 Reserved \n
                23 Reserved \n
                24 DMA MPU Region tests \n
                25 MSS Mailbox single bit errors \n
                26 MSS Mailbox double bit errors \n
                27 radarSS Mailbox single bit errors \n
                28 radarSS Mailbox double bit errors \n
                29 EDMA MPU test \n
                30 EDMA parity test \n
                31 CSI2 parity test \n
     */
    rlUInt32_t testStatusFlg;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
}rlMssLatentFaultReport_t;


/*! \brief
 * Structure to hold data strucutre for test status of the periodic tests
 * for event RL_DEV_AE_MSS_PERIODIC_TEST_STATUS_SB
 */
typedef struct rlMssPeriodicTestStatus
{
    /**
     * @brief  1 - PASS, 0 - FAIL \n
                Bits Definition \n
                0    Periodic read back of static registers \n
                1    ESM self-test \n
                31:2 RESERVED \n
     */
    rlUInt32_t testStatusFlg;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
}rlMssPeriodicTestStatus_t;


/*! \brief
 * Structure to hold data strucutre for RF-error status send by MSS
 * for event RL_DEV_AE_MSS_RF_ERROR_STATUS_SB
 */
typedef struct rlMssRfErrStatus
{
    /**
     * @brief  Value  Definition \n
                  0   No fault \n
                  1   radarSS FW assert \n
                  2   radarSS FW abort \n
                  3   radarSS ESM GROUP1 ERROR \n
                  4   radarSS ESM GROUP2 ERROR \n
                  31:5    RESERVED \n
     */
    rlUInt32_t errStatusFlg;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
}rlMssRfErrStatus_t;

/*! \brief
* Structure to hold the BSS ESM Fault data strucutre for event RL_RF_AE_ESMFAULT_SB
*/
typedef struct rlBssEsmFault
{
    /**
    * @brief  Bits Definition
               0   Ramp gen sub block error \n
               1   RESERVED \n
               2   GPADC RAM sub block error \n
               3   VIM RAM sub block error \n
               4   DFE self test error \n
               5   VIM self test error \n
               6   B0 TCM sub block error \n
               7   B1 TCM sub block error \n
               8   CCMR4 selftest error \n
               9   ATCM sub block error \n
               10  Ramp gen self test error \n
               11  Ramp gen parity self test error \n
               12  Sequence extinguisher self test error \n
               13  Sequence extinguisher sub block error \n
               14  RESERVED \n
               15  AGC RAM sub block error \n
               16  B1 TCM parity check error \n
               17  B0 TCM parity check error \n
               18  ATCM parity check error \n
               19  Mailbox MSS to BSS sub block error \n
               20  Mailbox BSS to MSS sub block error \n
               31:21  RESERVED \n
    */
    rlUInt32_t esmGrp1Err;
    /**
    * @brief  Bits Definition \n
               0   DFE STC error \n
               1   CR4 STC error \n
               2   CCMR4 comparator error \n
               3   B0TCM DB error \n
               4   B1TCM DB error \n
               5   ATCM DB error \n
               6   DCC error \n
               7   Sequence extinguisher error \n
               8   Synthesizer frequency monitoring error \n
               9   DFE parity error \n
               10  Ramp gen DB error \n
               11  Bubble correctin fail \n
               12  Ramp gen lockstep error \n
               13  RTI reset error \n
               14  GPADC RAM DB error \n
               15  VIM comparator error \n
               16  CR4 live clock error \n
               17  Watch dog timer NMI error \n
               18  VIM RAM DB error \n
               19  Ramp gen parity error \n
               20  Sequence extinguisher DB error \n
               21  DMA MPU error \n
               22  AGC RAM DB error \n
               23  CRC comparator error \n
               24  Wakeup status error \n
               25  Short circuit error \n
               26  B1 TCM parity error \n
               27  B0 TCM parity error \n
               28  ATCM parity error \n
               29  Mailbox MSS to BSS DB error \n
               30  Mailbox BSS to MSS DB error \n
               31  CCC error \n
    */
    rlUInt32_t esmGrp2Err;
}rlBssEsmFault_t;

/*! \brief
 *  This sub block indicates fault in analog supplies or LDO short circuit condition. Once a fault
 *  is detected the functionality cannot be resumed from then on and the sensor needs to be
 *  re-started. This Async Event is generated with SB-ID RL_DEV_AE_MSS_VMON_ERROR_STATUS_SB.
 */
typedef struct rlVmonErrStatus
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value  Definition          \n
     *          0     NO FAULT            \n
     *          1     ANALOG_SUPPLY_FAULT \n
     *      Others    RESERVED            \n
     */
    rlUInt8_t faultType;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Value  Definition          \n
     *          0     NO FAULT            \n
     *          1     ANALOG_SUPPLY_FAULT \n
     *      Others    RESERVED            \n
     */
    rlUInt8_t faultType;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  Fault signature \n
     *         Bit  Definition \n
     *          0   VDDIN under voltage indication \n
     *          1   VDDIN over voltage indication  \n
     *          2   VIN_18CLK supply fault         \n
     *          3   VIOIN supply fault (Unable to resolve between 1.8V and 3.3V) \n
     *          4   VIN_SRAM under voltage indication \n
     *          5   VIOIN_18 supply fault             \n
     *          6   APLL_VCO_LDO short circuit        \n
     *        31:7  RESERVED
     */
    rlUInt32_t faultSig;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved2;
}rlVmonErrStatus_t;

/*! \brief
 *  This async event is in response to the command (RL_DEV_CONFIG_SET_MSG:
 *  RL_DEV_RX_DATA_PATH_CONF_SET_SB) which indicates ADC data needs to be transferred over
 *  SPI. This async event contains the ADC data followed by more such async events for
 *  additional data. SB-ID : RL_DEV_AE_MSS_ADC_DATA_SB
 */
typedef struct rlRcvAdcData
{
    /**
     * @brief  Number of remaining chunks expected. (Remaining length / 220 bytes)
     */
    rlUInt16_t remChunks;
    /**
     * @brief  ADC data captured by the MMIC. Variable length [4-220] Bytes.
     */
    rlSInt8_t adcData[220U];
}rlRcvAdcData_t;

/*! \brief
* mmWaveLink RF Init Complete data structure for event RL_RF_AE_INITCALIBSTATUS_SB
*/
typedef struct rlRfInitComplete
{
    /**
     * @brief  RF Calibration Status, bit value: 1 - SUCCESS, 0 - FAILURE \n
                     Bit Calibration \n
                     0   RESERVED \n
                     1   APLL tuning \n
                     2   SYNTH VCO1 tuning \n
                     3   SYNTH VCO2 tuning \n
                     4   LODIST calibration \n
                     5   RX ADC DC offset calibration \n
                     6   HPF cutoff calibration \n
                     7   LPF cutoff calibration \n
                     8   Peak detector calibration \n
                     9   TX Power calibration \n
                     10  RX gain calibration \n
                     11  TX Phase calibration \n
                     12  RX IQMM calibration \n
                     31:13   [Reserved] \n
                     NOTE - CALIBRATION_STATUS should be \n
                     checked only if CALIBRATION_ENABLE bit \n
                     is set to 1. \n
     */
    rlUInt32_t calibStatus;
    /**
     * @brief  this field is set only for updated calibration. It has same bit definition as \n
                     CALIBRATION_STATUS  \n
     */
    rlUInt32_t calibUpdate;
    /**
     * @brief Measured temperature, based on average of temperature sensors near all enabled TX \n
                     and RX channels at the time of calibration. \n
                     1 LSB = 1o Celsius \n
     */
    rlUInt16_t temperature;
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved0;
    /**
     * @brief  This field indicates when the calibration updates were performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved1;
}rlRfInitComplete_t;

/*! \brief
 * mmWaveLink RF Run time calibration report for event RL_RF_AE_RUN_TIME_CALIB_REPORT_SB
 * @note : In IWR6843 ES1.0, only APLL, SYNTH1, SYNTH2 and LODIST runtime calibrations are
 *         supported.
 */
typedef struct rlRfRunTimeCalibReport
{
    /**
     * @brief  1 = calibration is passed, 0 = calibration is failed or not enabled/performed at \n
                    least once. \n
                    Bit: Calibration \n
                    0: [Reserved] \n
                    1: APLL tuning \n
                    2: SYNTH VCO1 tuning \n
                    3: SYNTH VCO2 tuning \n
                    4: LODIST calibration \n
                    5: [Reserved] \n
                    6: [Reserved] \n
                    7: [Reserved] \n
                    8: PD calibration \n
                    9: TX Power calibration \n
                    10: RX gain calibration \n
                    11: [Reserved] \n
                    12: [Reserved] \n
                    31:13: [Reserved] \n
     */
    rlUInt32_t calibErrorFlag;
    /**
     * @brief  Whether each calibration resulted in a reconfiguration of RF is indicated by a \n
                     value of 1 in the respective bit in this field. It has bit definition as \n
                     above \n
     */
    rlUInt32_t calibUpdateStatus;
    /**
     * @brief  Measured temperature, based on average of temperature sensors near all enabled \n
                     TX and RX channels at the time of calibration. \n
                     1 LSB = 1 degree Celsius \n
     */
    rlInt16_t  temperature;
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved0;
    /**
     * @brief  This field indicates when the calibration updates were performed. \n
                     1 LSB = 1 millisecond \n
                     (the stamp rolls over upon exceeding allotted bit width)  \n
     */
    rlUInt32_t timeStamp;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved1;
}rlRfRunTimeCalibReport_t;

/*! \brief
* API APLL closed loop cal Status Get Sub block structure
*/
typedef struct rlRfApllCalDone
{
    rlUInt16_t  apllClCalStatus;
    /**
     * @brief  Tolerance
     */
    rlUInt16_t  cccTolerance;
    /**
     * @brief  Window
     */
    rlUInt16_t  cccCount0;
    /**
     * @brief  Measured 0.1/3 MHz Unit
     */
    rlUInt16_t  measFreqCount;
    /**
     * @brief  Expected
     */
    rlUInt32_t  cccCount1;
}rlRfApllCalDone_t;

/*! \brief
* RF characterization Time and Temperature data structure
*/
typedef struct rlRfTempData
{
    /**
     * @brief  radarSS local Time from device powerup. 1 LSB = 1 ms
     */
    rlUInt32_t time;
    /**
     * @brief  RX0 temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpRx0Sens;
    /**
     * @brief  RX1 temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpRx1Sens;
    /**
     * @brief  RX2 temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpRx2Sens;
    /**
     * @brief  RX3 temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpRx3Sens;
    /**
     * @brief  TX0 temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpTx0Sens;
    /**
     * @brief  TX1 temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpTx1Sens;
    /**
     * @brief  TX2 temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpTx2Sens;
    /**
     * @brief  PM temperature sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpPmSens;
    /**
     * @brief  Digital temp sensor reading (signed value). 1 LSB = 1 deg C
     */
    rlInt16_t tmpDig0Sens;
    /**
     * @brief  Second digital temp sensor reading (signed value).( applicable only in \n
     *         xWR1642/IWR6843/xWR1843.) \n
     *         1 LSB = 1 deg C \n
     */
    rlInt16_t tmpDig1Sens;
}rlRfTempData_t;

/*! \brief
 * Structure to hold the MSS/radarSS CPU Fault data strucutre for
 * event RL_DEV_AE_MSS_CPUFAULT_SB and RL_RF_AE_CPUFAULT_SB
 */
typedef struct rlCpuFault
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  0x0    RF Processor Undefined Instruction Abort \n
               0x1    RF Processor Instruction pre-fetch Abort \n
               0x2    RF Processor Data Access Abort \n
               0x3    RF Processor Firmware Fatal Error \n
               0x4 - 0xFF Reserved \n
     */
    rlUInt8_t faultType;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  0x0    RF Processor Undefined Instruction Abort \n
               0x1    RF Processor Instruction pre-fetch Abort \n
               0x2    RF Processor Data Access Abort \n
               0x3    RF Processor Firmware Fatal Error \n
               0x4 - 0xFF Reserved \n
     */
    rlUInt8_t faultType;
#endif
    /**
     * @brief  Valid only in case of FAULT type is 0x3, provides the source \n
                     line number at which fatal error occurred. \n
     */
    rlUInt16_t lineNum;
    /**
     * @brief  The instruction PC address at which Fault occurred
     */
    rlUInt32_t faultLR;
    /**
     * @brief  The return address of the function from which fault function \n
                     has been called (Call stack LR) \n
     */
    rlUInt32_t faultPrevLR;
    /**
     * @brief  The CPSR register value at which fault occurred
     */
    rlUInt32_t faultSpsr;
    /**
     * @brief  The SP register value at which fault occurred
     */
    rlUInt32_t faultSp;
    /**
     * @brief  The address access at which Fault occurred (valid only for fault \n
                     type 0x0 to 0x2) \n
     */
    rlUInt32_t faultAddr;
    /**
     * @brief  The status of Error (Error Cause type - valid only for \n
                     fault type 0x0 to 0x2) \n
                     0x000  BACKGROUND_ERR \n
                     0x001  ALIGNMENT_ERR \n
                     0x002  DEBUG_EVENT \n
                     0x00D  PERMISSION_ERR \n
                     0x008  SYNCH_EXTER_ERR \n
                     0x406  ASYNCH_EXTER_ERR \n
                     0x409  SYNCH_ECC_ERR \n
                     0x408  ASYNCH_ECC_ERR \n
     */
    rlUInt16_t faultErrStatus;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  The Source of the Error (Error Source type- valid only for fault type 0x0 to 0x2)\n
                     0x0  ERR_SOURCE_AXI_MASTER \n
                     0x1  ERR_SOURCE_ATCM \n
                     0x2  ERR_SOURCE_BTCM  \n
     */
    rlUInt8_t faultErrSrc;
    /**
     * @brief  The AXI Error type (Error Source type - valid only for fault type 0x0 to 0x2) \n
                     0x0  AXI_DECOD_ERR \n
                     0x1  AXI_SLAVE_ERR  \n
     */
    rlUInt8_t faultAxiErrType;
     /**
      * @brief  The Error Access type (Error Access type- valid only for fault type 0x0 to 0x2) \n
                     0x0  READ_ERR \n
                     0x1  WRITE_ERR \n
     */
    rlUInt8_t faultAccType;
    /**
     * @brief  The Error Recovery type (Error Recovery type - Valid only for fault \n
                     type 0x0 to 0x2) \n
                     0x0  UNRECOVERY \n
                     0x1  RECOVERY  \n
     */
    rlUInt8_t faultRecovType;
#else
    /**
     * @brief  The AXI Error type (Error Source type - valid only for fault type 0x0 to 0x2) \n
                     0x0  AXI_DECOD_ERR \n
                     0x1  AXI_SLAVE_ERR  \n
     */
    rlUInt8_t faultAxiErrType;
    /**
     * @brief  The Source of the Error (Error Source type- valid only for fault type 0x0 to 0x2)\n
                     0x0  ERR_SOURCE_AXI_MASTER \n
                     0x1  ERR_SOURCE_ATCM \n
                     0x2  ERR_SOURCE_BTCM  \n
     */
    rlUInt8_t faultErrSrc;
    /**
     * @brief  The Error Recovery type (Error Recovery type - Valid only for fault \n
                     type 0x0 to 0x2) \n
                     0x0  UNRECOVERY \n
                     0x1  RECOVERY  \n
     */
    rlUInt8_t faultRecovType;
     /**
      * @brief  The Error Access type (Error Access type- valid only for fault type 0x0 to 0x2) \n
                     0x0  READ_ERR \n
                     0x1  WRITE_ERR  \n
     */
    rlUInt8_t faultAccType;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
}rlCpuFault_t;

/*! \brief
* mmWaveLink firmware version structure
*/
typedef struct rlFwVersionParam
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  HW variant number
     */
    rlUInt8_t hwVarient;
    /**
     * @brief  HW version major number
     */
    rlUInt8_t hwMajor;
    /**
     * @brief  HW version minor number
     */
    rlUInt8_t hwMinor;
    /**
     * @brief  FW version major number
     */
    rlUInt8_t fwMajor;
    /**
     * @brief  FW version major number
     */
    rlUInt8_t fwMinor;
    /**
     * @brief  FW version build number
     */
    rlUInt8_t fwBuild;
    /**
     * @brief  FW version debug number
     */
    rlUInt8_t fwDebug;
    /**
     * @brief  FW Release Year
     */
    rlUInt8_t fwYear;
    /**
     * @brief  FW Release Month
     */
    rlUInt8_t fwMonth;
    /**
     * @brief  FW Release Day
     */
    rlUInt8_t fwDay;
    /**
     * @brief  Patch version major number
     */
    rlUInt8_t patchMajor;
    /**
     * @brief  Patch version minor number
     */
    rlUInt8_t patchMinor;
    /**
     * @brief  Patch Release Year
     */
    rlUInt8_t patchYear;
    /**
     * @brief  Patch Release Month
     */
    rlUInt8_t patchMonth;
    /**
     * @brief  Patch Release Day
     */
    rlUInt8_t patchDay;
    /**
     * @brief  Debug and build version
     *         b3:0   Debug version
     *         b7:4   Build version
     */
    rlUInt8_t patchBuildDebug;
#else
    /**
     * @brief  version major number
     */
    rlUInt8_t hwMajor;
    /**
     * @brief  HW variant number
     */
    rlUInt8_t hwVarient;
    /**
     * @brief  FW version major number
     */
    rlUInt8_t fwMajor;
    /**
     * @brief HW version minor number
     */
    rlUInt8_t hwMinor;
    /**
     * @brief  FW version build number
     */
    rlUInt8_t fwBuild;
    /**
     * @brief  FW version major number
     */
    rlUInt8_t fwMinor;
    /**
     * @brief  FW Release Year
     */
    rlUInt8_t fwYear;
    /**
     * @brief  FW version debug number
     */
    rlUInt8_t fwDebug;
    /**
     * @brief  FW Release Day
     */
    rlUInt8_t fwDay;
    /**
     * @brief  FW Release Month
     */
    rlUInt8_t fwMonth;
    /**
     * @brief  Patch version minor number
     */
    rlUInt8_t patchMinor;
    /**
     * @brief  Patch version major number
     */
    rlUInt8_t patchMajor;
    /**
     * @brief  Patch Release Month
     */
    rlUInt8_t patchMonth;
    /**
     * @brief  Patch Release Year
     */
    rlUInt8_t patchYear;
    /**
     * @brief  Debug and build version
     *         b3:0   Debug version
     *         b7:4   Build version
     */
    rlUInt8_t patchBuildDebug;
    /**
     * @brief  Patch Release Day
     */
    rlUInt8_t patchDay;
#endif
}rlFwVersionParam_t;

/*! \brief
* mmwavelink software version structure
*/
typedef struct rlSwVersionParam
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  SW version major number
     */
    rlUInt8_t major;
    /**
     * @brief  SW version minor number
     */
    rlUInt8_t minor;
    /**
     * @brief  SW version buid number
     */
    rlUInt8_t build;
    /**
     * @brief  SW version debug number
     */
    rlUInt8_t debug;
    /**
     * @brief  Software Release Year
     */
    rlUInt8_t year;
    /**
     * @brief  Software Release Month
     */
    rlUInt8_t month;
    /**
     * @brief  Software Release Day
     */
    rlUInt8_t day;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
#else
    /**
     * @brief  SW version minor number
     */
    rlUInt8_t minor;
    /**
     * @brief  SW version major number
     */
    rlUInt8_t major;
    /**
     * @brief  SW version debug number
     */
    rlUInt8_t debug;
    /**
     * @brief  SW version buid number
     */
    rlUInt8_t build;
    /**
     * @brief  Software Release Month
     */
    rlUInt8_t month;
    /**
     * @brief  Software Release Year
     */
    rlUInt8_t year;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
    /**
     * @brief  Software Release Day
     */
    rlUInt8_t day;
#endif
}rlSwVersionParam_t;

/*! \brief
* mmwavelink version structure
*/
typedef struct rlVersion
{
    /**
     * @brief  Master Sub System version
     */
    rlFwVersionParam_t master;
    /**
     * @brief  RF Sub System version
     */
    rlFwVersionParam_t rf;
    /**
     * @brief  mmWaveLink version
     */
    rlSwVersionParam_t mmWaveLink;
}rlVersion_t;

/*! \brief
* GPADC measurement data for sensors
*/
typedef struct rlGpAdcData
{
    /**
     * @brief  Min value of GP ADC data
     */
    rlUInt16_t min;
    /**
     * @brief  Max value of GP ADC data
     */
    rlUInt16_t max;
    /**
     * @brief  Avg value of GP ADC data
     */
    rlUInt16_t avg;
} rlGpAdcData_t;

/*! \brief
* Sensors GPADC measurement data for event RL_RF_AE_GPADC_MEAS_DATA_SB
*/
typedef struct rlRecvdGpAdcData
{
    /**
     * @brief  collected all GP ADC data
     */
    rlGpAdcData_t sensor[RL_MAX_GPADC_SENSORS];
} rlRecvdGpAdcData_t;

/*! \brief
* Analog fault strucure for event RL_RF_AE_ANALOG_FAULT_SB
*/
typedef struct rlAnalogFaultReportData
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Indicates the analog fault type
     *         Value   Definition
     *           0     No fault
     *           1     Analog supply fault
     *         Others  RESERVED
     */
    rlUInt8_t   faultType;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t   reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t   reserved0;
    /**
     * @brief  Indicates the analog fault type
     *         Value   Definition
     *           0     No fault
     *           1     Analog supply fault
     *         Others  RESERVED
     */
    rlUInt8_t   faultType;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t  reserved1;
    /**
     * @brief  Indicates which analog supply is in fault
     *         Bit    Definition
     *          0     1.8V BB Analog supply fault
     *          1     1.3V/1.0V RF supply fault
     *          2     Synth VCO LDO short circuit detected
     *          3     PA LDO short circuit detected
     *          31:4  RESERVED
     */
    rlUInt32_t  faultSig;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t  reserved2;
} rlAnalogFaultReportData_t;

/*! \brief
* Calibration monitoring timing error data for event RL_RF_AE_MON_TIMING_FAIL_REPORT_SB
* @note : In IWR6843 ES1.0, Periodic Digital and Analog Monitoring are not supported.
*/
typedef struct rlCalMonTimingErrorReportData
{
    /**
     * @brief  [b0] 1 = Total monitoring time does not fit in one CALIB_MON_TIME_UNIT when one \n
                        time calibration is enabled, \n
                    0 = No failure \n
               [b1] 1 = Total monitoring and calibration time don't fit in one calibMonTimeUnit \n
                        when periodic calibration is enabled, \n
                    0 = No failure \n
               [b2] 1 = Runtime timing violation: Monitoring functions or calibrations could \n
                        not be completed in one calibMonTimeUnit, \n
                    0 = No failure \n
               [b3-b15] RESERVED  \n
     */
    rlUInt16_t timingFailCode;
    rlUInt16_t reserved;
}rlCalMonTimingErrorReportData_t;

/*! \brief
* Latent fault digital monitoring status data for event RL_RF_AE_DIG_LATENTFAULT_REPORT_AE_SB
*/
typedef struct rlDigLatentFaultReportData
{
    /**
     * @brief  1 - PASS, 0 - FAIL \n
                 Bit     Definition \n
                 0       RESERVED \n
                 1       CR4_VIM_LOCKSTEP_MONITORING \n
                 2       RESERVED \n
                 3       VIM_MONITORING \n
                 4       RESERVED \n
                 5       RESERVED \n
                 6       CRC_MONITORING \n
                 7       RAMPGEN_ECC_MONITORING \n
                 8       DFE_PARITY_MONITORNG \n
                 9       DFE_ECC_MONITORING \n
                 10      RAMPGEN_LOCKSTEP_MONITORING \n
                 11      FRC_LOCKSTEP_MONITORING \n
                 12      RESERVED \n
                 13      RESERVED \n
                 14      RESERVED \n
                 15      RESERVED \n
                 16      ESM_MONITORING \n
                 17      DFE_STC_MONITORING \n
                 18      RESERVED \n
                 19      ATCM_BTCM_ECC_MONITORING  \n
                 20      ATCM_BTCM_PARITY_MONITORING \n
                 21      RESERVED \n
                 22      RESERVED \n
                 23      RESERVED \n
                 24      FFT_MONITORING \n
                 25      RTI_MONITORING \n
                 26      PCR_MONITORING \n
                 31:27   RESERVED  \n
     */
    rlUInt32_t digMonLatentFault;
}rlDigLatentFaultReportData_t;

/*! \brief
* The report header includes common information across all enabled monitors
* like current FTTI number and current temperature. event: RL_RF_AE_MON_REPORT_HEADER_SB
*/
typedef struct rlMonReportHdrData
{
    /**
     * @brief  FTTI free running counter value, incremented every CAL_MON_TIME_UNIT
     */
    rlUInt32_t fttiCount;
    /**
     * @brief  Average temperature at which was monitoring performed
     */
    rlUInt16_t avgTemp;
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved0;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved1;
}rlMonReportHdrData_t;

/*! \brief
* This async event is sent periodically to indicate the status of periodic
* digital monitoring tests.Event: RL_RF_AE_MON_DIG_PERIODIC_REPORT_SB
*/
typedef struct rlDigPeriodicReportData
{
    /**
     * @brief  1 - PASS, 0 - FAIL \n
                 Bit    Monitoring type \n
                 [0]    PERIODIC_CONFG_REGISTER_READ \n
                 [1]    ESM_MONITORING \n
                 [2]    DFE_STC \n
                 [3]    FRAME_TIMING_MONITORING \n
                 [31:4] RESERVED  \n
     */
    rlUInt32_t digMonPeriodicStatus;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlDigPeriodicReportData_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host,
* containing the measured temperature near various RF analog and digital modules.
* The xWR device sends this to host at the programmed periodicity or when failure occurs,
* as programmed by the configuration API SB. Event:RL_RF_AE_MON_TEMPERATURE_REPORT_SB
*/
typedef struct rlMonTempReportData
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to \n
                   various threshold checks under this monitor. \n
                   Bit  STATUS_FLAG for monitor \n
                   [0]  STATUS_ANA_TEMP_MIN \n
                   [1]  STATUS_ANA_TEMP_MAX \n
                   [2]  STATUS_DIG_TEMP_MIN \n
                   [3]  STATUS_DIG_TEMP_MAX \n
                   [4]  STATUS_TEMP_DIFF_THRESH \n
                   [15:5]RESERVED \n
                   0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
    /**
     * @brief  The measured onchip temperatures are reported here. \n
                     Byte numbers corresponding to different temperature sensors \n
                     reported in this field are here: \n
                     Bytes       SIGNAL \n
                     [1:0]       TEMP_RX0 \n
                     [3:2]       TEMP_RX1 \n
                     [5:4]       TEMP_RX2 \n
                     [7:6]       TEMP_RX3 \n
                     [9:8]       TEMP_TX0 \n
                     [11:10]     TEMP_TX1 \n
                     [13:12]     TEMP_TX2 \n
                     [15:14]     TEMP_PM \n
                     [17:16]     TEMP_DIG1 \n
                     [19:18]     TEMP_DIG2 (Applicable only in xWR1642 & xWR1843) \n
                     1 LSB = 1 degree C, signed number  \n
     */
    rlInt16_t tempValues[10U];
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond  \n
                     (the stamp rolls over upon exceeding allotted bit width)  \n
     */
    rlUInt32_t timeStamp;
}rlMonTempReportData_t;

/*! \brief
* This API is a Monitoring report which RadarSS sends to the host,
* containing the measured RX gain and phase values. RadarSS sends this
* to host at the programmed periodicity or when failure occurs, as programmed
* by the configuration API SB. Event: RL_RF_AE_MON_RX_GAIN_PHASE_REPORT
*/
typedef struct rlMonRxGainPhRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding \n
                     to various threshold checks under this monitor. \n
                     Bit  STATUS_FLAG for monitor \n
                     [0]  STATUS_RX_GAIN_ABS \n
                     [1]  STATUS_RX_GAIN_MISMATCH \n
                     [2]  STATUS_RX_GAIN_FLATNESS \n
                     [3]  STATUS_RX_PHASE_MISMATCH \n
                     [15:4]RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring  Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
     /**
      * @brief  The measured RX gain for each enabled channel, at each enabled RF frequency \n
                     (i.e., lowest, center and highest in the profile's RF band) is reported \n
                     here. Byte numbers corresponding to different RX and RF, in this field \n
                     are here:  \n
                             RF1     RF2     RF3 \n
                     RX0     1:0     9:8     17:16 \n
                     RX1     3:2     11:10   19:18 \n
                     RX2     5:4     13:12   21:20 \n
                     RX3     7:6     15:14   23:22 \n
                     1 LSB = 0.1 dB \n
                     Only the entries of enabled RF Frequencies and enabled RX channels are \n
                     valid. \n
     */
    rlUInt16_t rxGainVal[12U];
    /**
     * @brief  The measured RX phase for each enabled channel, at each enabled RF frequency is \n
                     reported here. Byte numbers corresponding to different RX and RF,  \n
                     in this field are here: \n
                             RF1     RF2     RF3 \n
                     RX0     1:0     9:8     17:16 \n
                     RX1     3:2     11:10   19:18 \n
                     RX2     5:4     13:12   21:20 \n
                     RX3     7:6     15:14   23:22 \n
                     LSB = 360 (degree)/2^16. \n
                     Only the entries of enabled RF Frequencies and enabled RX channels are \n
                     valid. \n
                     Note: these phases include an unknown bias common to all RX channels. \n
     */
    rlUInt16_t rxPhaseVal[12U];
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved2;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved3;
    /**
     * @brief  This field indicates when the last monitoring  in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlMonRxGainPhRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends
* to the host, containing the measured RX noise figure values
* corresponding to the full IF band of a profile. RadarSS sends
* this to host at the programmed periodicity or when failure occurs,
* as programmed by the configuration API SB. Event: RL_RF_AE_MON_RX_NOISE_FIG_REPORT
*/
typedef struct rlMonRxNoiseFigRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit  STATUS_FLAG for monitor \n
                     [0]  STATUS_RX_NOISE_FIGURE \n
                     [15:1]RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  TThe measured RX input referred for each enabled channel, at each enabled RF \n
                     frequency is reported here. Byte numbers corresponding to different RX \n
                     and RF, in this field are here: \n
                             RF1     RF2     RF3 \n
                     RX0     1:0     9:8     17:16 \n
                     RX1     3:2     11:10   19:18 \n
                     RX2     5:4     13:12   21:20 \n
                     RX3     7:6     15:14   23:22 \n
                     1 LSB = 0.1 dB \n
                     Only the entries of enabled RF Frequencies and enabled RX channels are \n
                     valid. \n
     */
    rlUInt16_t rxNoiseFigVal[12U];
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved2;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved3;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved4;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon \n
                     exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonRxNoiseFigRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing the measured
* RX IF filter attenuation values at the given IF frequencies. RadarSS sends this to host
* at the programmed periodicity or when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_RX_IF_STAGE_REPORT
*/
typedef struct rlMonRxIfStageRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit  STATUS_FLAG for monitor \n
                     [0]  STATUS_RX_HPF_ERROR \n
                     [1]  STATUS_RX_LPF_ERROR \n
                     [2]  STATUS_RX_IFA_GAIN_ERROR \n
                     [15:3]RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  The deviations of RX IFA HPF cutoff frequency from the ideally expected values \n
                     for all the enabled RX channels are reported here. \n
                     HPF_CUTOFF_FREQ_ERROR = 100*(Measured Cutoff Frequency /  \n
                     Expected Cutoff Frequency) - 100, \n
                     for RX IF filter in the HPF region. \n
                     Byte numbers corresponding to measured cutoff frequency error  \n
                     on different RX channels, in this field are here: \n
                             I channel   Q channel \n
                     RX0     0           4 \n
                     RX1     1           5 \n
                     RX2     2           6 \n
                     RX3     3           7 \n
                     1 LSB = 1%, signed number \n
                     Applicable only for the enabled channels. \n
     */
    rlInt8_t hpfCutOffFreqEr[8U];
    /**
     * @brief  The deviations of RX IFA LPF cutoff frequency \n
                     from the ideally expected values for all the enabled \n
                     RX channels are reported here. \n
                     LPF_CUTOFF_FREQ_ERROR = 100*(Measured Cutoff Frequency / \n
                     Expected Cutoff Frequency) - 100, for RX IF filter in the LPF \n
                     region. \n
                     Byte numbers corresponding to measured cutoff frequency error \n
                     on different RX channels, in this field are here: \n
                             I channel   Q channel \n
                     RX0     0           4 \n
                     RX1     1           5 \n
                     RX2     2           6 \n
                     RX3     3           7 \n
                     1 LSB = 1%, signed number \n
                     Applicable only for the enabled channels. \n
     */
    rlInt8_t lpfCutOffFreqEr[8U];
    /**
     * @brief  The deviations of RX IFA Gain from the ideally expected \n
                     values for all the enabled RX channels are reported here. \n
                     Byte numbers corresponding to measured cutoff frequency error \n
                     on different RX channels and HPF/LPF, in this field are here: \n
                     I channel   Q channel \n
                     RX0     0           4 \n
                     RX1     1           5 \n
                     RX2     2           6 \n
                     RX3     3           7 \n
                     1 LSB = 0.1dB, signed number \n
                     Applicable only for the enabled channels. \n
     */
    rlInt8_t rxIfaGainErVal[8U];

    /**
     * @brief  Expected IF gain
     * 1 LSB = 1 dB, 8 bit signed number
     */
    rlInt8_t ifGainExp;

    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved2;
#else
     /**
      * @brief  The deviations of RX IFA HPF cutoff frequency \n
                    from the ideally expected values for all the enabled \n
                    RX channels are reported here. \n
                    HPF_CUTOFF_FREQ_ERROR = 100*(Measured Cutoff Frequency / \n
                    Expected Cutoff Frequency) - 100, \n
                    for RX IF filter in the HPF region. \n
                    Byte numbers corresponding to measured cutoff frequency error \n
                    on different RX channels, in this field are here: \n
                            I channel   Q channel \n
                    RX1     0           4 \n
                    RX0     1           5 \n
                    RX3     2           6 \n
                    RX2     3           7 \n
                    1 LSB = 1%, signed number \n
                    Applicable only for the enabled channels. \n
     */
    rlInt8_t hpfCutOffFreqEr[8U];
    /**
     * @brief  The deviations of RX IFA LPF cutoff frequency \n
                     from the ideally expected values for all the enabled \n
                     RX channels are reported here. \n
                     LPF_CUTOFF_FREQ_ERROR = 100*(Measured Cutoff Frequency / \n
                     Expected Cutoff Frequency) - 100, for RX IF filter in the LPF \n
                     region. \n
                     Byte numbers corresponding to measured cutoff frequency error \n
                     on different RX channels, in this field are here: \n
                             I channel   Q channel \n
                     RX1     0           4 \n
                     RX0     1           5 \n
                     RX3     2           6 \n
                     RX2     3           7 \n
                     1 LSB = 1%, signed number \n
                     Applicable only for the enabled channels. \n
     */
    rlInt8_t lpfCutOffFreqEr[8U];
    /**
     * @brief  The deviations of RX IFA Gain from the ideally expected \n
                     values for all the enabled RX channels are reported here. \n
                     Byte numbers corresponding to measured cutoff frequency error \n
                     on different RX channels and HPF/LPF, in this field are here: \n
                     I channel   Q channel \n
                     RX1     0           4 \n
                     RX0     1           5 \n
                     RX3     2           6 \n
                     RX2     3           7 \n
                     1 LSB = 0.1dB, signed number \n
                     Applicable only for the enabled channels. \n
     */
    rlInt8_t rxIfaGainErVal[8U];

    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved2;

    /**
     * @brief  Expected IF gain
     * 1 LSB = 1 dB, 8 bit signed number
     */
    rlInt8_t ifGainExp;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved3;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved4;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonRxIfStageRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing the
* measured TX power values during an explicit monitoring chirp. RadarSS sends this to
* host at the programmed periodicity or when failure occurs, as programmed by the
* configuration API SB. Same structure is application for Tx0/Tx1/Tx2 power report.
* Event: RL_RF_AE_MON_TXn_POWER_REPORT
*/
typedef struct rlMonTxPowRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding \n
                     to various threshold checks under this monitor. \n
                     Bit  STATUS_FLAG for monitor \n
                     [0]  STATUS_ABS_ERR \n
                     [1]  STATUS_FLATNESS_ERR \n
                     [15:2]RESERVED \n
                       0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  The measured TX power for each enabled channel, at each enabled RF frequency is \n
               reported here. Byte numbers corresponding to different TX and RF, in this \n
               field are here: \n
                       RF1     RF2     RF3 \n
               TX      1:0     3:2     5:4 \n
               (other bytes are reserved) \n
               1 LSB = 0.1 dBm, signed number \n
               Only the entries of enabled RF Frequencies and enabled RX \n
               channels are valid. \n
     */
    rlInt16_t txPowVal[3U];
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved2;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon \n
                     exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonTxPowRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing the measured
* TX reflection coefficient's magnitude values, meant for detecting TX ball break. RadarSS sends
* this to host at the programmed periodicity or when failure occurs, as programmed by the
* configuration API SB.
* Same strucuture is applicable for Tx0/Tx1/Tx2 ball break report.
* Event: RL_RF_AE_MON_TXn_BALLBREAK_REPORT
*/
typedef struct rlMonTxBallBreakRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding \n
               to various threshold checks under this monitor. \n
               Bit  STATUS_FLAG for monitor \n
               [0]  STATUS_TXn_BALLBREAK \n
               [15:1]RESERVED \n
               0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
    /**
     * @brief  The TX reflection coefficient's magnitude for this channel is reported here. \n
               1 LSB = 0.1 dB, signed number \n
     */
    rlInt16_t txReflCoefVal;
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved0;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved1;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
               1 LSB = 1 millisecond (the stamp rolls over upon \n
               exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonTxBallBreakRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing the measured Tx
* gain and phase mismatch values during an explicit monitoring chirp. RadarSS sends this to host
* at the programmed periodicity or when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_TX_GAIN_MISMATCH_REPORT
*/
typedef struct rlMonTxGainPhaMisRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding \n
               to various threshold checks under this monitor. \n
               Bit  STATUS_FLAG for monitor \n
               [0]  STATUS_TX_GAIN_MISMATCH \n
               [1]  STATUS_TX_PHASE_MISMATCH \n
               [15:2]RESERVED \n
               0 - FAIL or check wasn't done, 1 - PASS \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief   The measured TX PA loopback tone power at the RX ADC input, \n
                for each enabled TX channel, at each enabled RF frequency is reported \n
                here. Byte numbers corresponding to different TX and RF, in this field are \n
                here: \n
                        RF1     RF2     RF3 \n
                TX0     1:0     7:6     13:12 \n
                TX1     3:2     9:8     15:14 \n
                TX2     5:4     11:10   17:16 \n
                1 LSB = 0.1dBm, signed number \n
                Only the entries of enabled RF Frequencies and enabled TX channels are valid. \n
     */
    rlInt16_t txGainVal[9U];
    /**
     * @brief  The measured TX phase for each enabled channel, at each enabled RF \n
               frequency is reported here.Byte numbers corresponding to different TX and \n
               RF, in this field are here: \n
                       RF1     RF2     RF3 \n
               TX0     1:0     7:6     13:12 \n
               TX1     3:2     9:8     15:14 \n
               TX2     5:4     11:10   17:16 \n
               1 LSB = 360(degree)/2^16. \n
               Only the entries of enabled RF Frequencies and enabled TX channels are \n
               valid. \n
               Note: these phases include an unknown bias common to all TX channels. \n
     */
    rlUInt16_t txPhaVal[9U];
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved2;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved3;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                 1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonTxGainPhaMisRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing the measured
* TX1 BPM error values. RadarSS sends this to host at the programmed periodicity or when failure
* occurs, as programmed by the configuration API SB. Same structure is applicable for
* Tx0/Tx1/Tx2 BPM report data.
* Event: RL_RF_AE_MON_TXn_BPM_REPORT
*/
typedef struct rlMonTxBpmRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding \n
               to various threshold checks under this monitor. \n
               Bit  STATUS_FLAG for monitor \n
               [0]  STATUS_TXn_BPM_PHASE \n
               [1]  STATUS_TXn_BPM_AMPLITUDE \n
               [15:2]RESERVED \n
               0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  MSB of phase shifter monitor 2 for TX. \n
               phaseShifterMonVal2 = phaseShifterMonVal2Msb * pow(2,8) + \n
                                     phaseShifterMonVal2Lsb \n
               1 LSB = (360 degree) / pow(2,16) \n                     
     */
    rlUInt8_t phaseShifterMonVal2Msb;
#else
    /**
     * @brief  MSB of phase shifter monitor 2 for TX. \n
               phaseShifterMonVal2 = phaseShifterMonVal2Msb * pow(2,8) + \n
                                     phaseShifterMonVal2Lsb \n
               1 LSB = (360 degree) / pow(2,16) \n                     
     */
    rlUInt8_t phaseShifterMonVal2Msb;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Monitored phase shift for phase shifter monitor 1 for TX
               1 LSB = (360 degree) / pow(2,16) \n
     */
    rlUInt16_t phaseShifterMonVal1;
    /**
     * @brief  The TX output phase difference between the two BPM settings (phase for TX BPM \n
                    setting 0 - phase for TX BPM setting 1) is reported here. \n
                    1 LSB = 360(degree)/2^16. \n
     */
    rlUInt16_t txBpmPhaDiff;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  The deviation of the TX output amplitude difference between the two \n
                     BPM settings (amplitude for TX BPM setting 0 - amplitude for TX \n
                     BPM setting 1) from the ideal 0dB is reported here. \n
                     1 LSB = 0.1dB, signed number \n
     */
    rlInt8_t txBpmAmpDiff;
    /**
     * @brief  LSB of phase shifter monitor 2 for TX. \n
               phaseShifterMonVal2 = phaseShifterMonVal2Msb * pow(2,8) + \n
                                     phaseShifterMonVal2Lsb \n
               1 LSB = (360 degree) / pow(2,16) \n                     
     */
    rlUInt8_t phaseShifterMonVal2Lsb;
#else
    /**
     * @brief  LSB of phase shifter monitor 2 for TX. \n
               phaseShifterMonVal2 = phaseShifterMonVal2Msb * pow(2,8) + \n
                                     phaseShifterMonVal2Lsb \n
               1 LSB = (360 degree) / pow(2,16) \n                     
     */
    rlUInt8_t phaseShifterMonVal2Lsb;
    /**
     * @brief  The deviation of the TX output amplitude difference between the two BPM settings \n
                     (amplitude for TX BPM setting 0 - amplitude for TX PM setting 1) from the \n
                     ideal 0dB is reported here. \n
                     1 LSB = 0.1dB, signed number \n
     */
    rlInt8_t txBpmAmpDiff;
#endif
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlMonTxBpmRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing information
* related to measured frequency error during the chirp. RadarSS sends this to host at the
* programmed periodicity or when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_SYNTHESIZER_FREQ_REPORT
*/
typedef struct rlMonSynthFreqRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit  STATUS_FLAG for monitor \n
                     [0]  STATUS_SYNTH_FREQ_ERR \n
                     [15:1]RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  This field indicates the maximum instantaneous frequency error measured  \n
                     during the chirps for which frequency monitoring has been enabled in the \n
                     previous monitoring period. \n
                     Bits    Parameter \n
                     31:0    Maximum frequency error value, signed number. 1 LSB = 1kHz. \n
     */
    rlInt32_t maxFreqErVal;
    /**
     * @brief  This field indicates the number of times during chirping in the previous \n
                     monitoring period in which the measured frequency error violated the allowed\n
                     threshold. Frequency error threshold violation is counted every 10ns. \n
                     Bits    Parameter \n
                     31:19   RESERVED \n
                     18:0    Failure count, unsigned number. \n
     */
    rlUInt32_t freqFailCnt;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved2;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved3;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlMonSynthFreqRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing the external
* signal voltage values measured using the GPADC. RadarSS sends this to host at the programmed
* periodicity or when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_EXT_ANALOG_SIG_REPORT
*/
typedef struct rlMonExtAnaSigRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding \n
                     to various threshold checks under this monitor. \n
                     Bit  STATUS_FLAG for monitor \n
                     0    STATUS_ANALOGTEST1 \n
                     1    STATUS_ANALOGTEST2 \n
                     2    STATUS_ANALOGTEST3 \n
                     3    STATUS_ANALOGTEST4 \n
                     4    STATUS_ANAMUX \n
                     5    STATUS_VSENSE \n
                     15:3 RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
    /**
     * @brief  MEASURED_VALUE \n
                     Bytes   SIGNAL \n
                     1:0     ANALOGTEST1 \n
                     3:2     ANALOGTEST2 \n
                     5:4     ANALOGTEST3 \n
                     7:6     ANALOGTEST4 \n
                     9:8     ANAMUX \n
                     11:10   VSENSE \n
                     1 LSB = 1.8V/1024  \n
     */
    rlInt16_t extAnaSigVal[6U];
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlMonExtAnaSigRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing information
* about Internal TX internal analog signals. RadarSS sends this to host at the programmed
* periodicity or when failure occurs, as programmed by the configuration API SB. Same structure
* is applicable for Tx0/Tx1/Tx2 monitoring report. Event: RL_RF_AE_MON_TXn_INT_ANA_SIG_REPORT
*/
typedef struct rlMonTxIntAnaSigRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit  STATUS_FLAG for monitor \n
                     [0]  STATUS_SUPPLY_TXn \n
                     [1]  STATUS_PWRDET_TXn \n
                     [15:2]RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon \n
                     exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonTxIntAnaSigRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing information
* about Internal RX internal analog signals. RadarSS sends this to host at the programmed
* periodicity or when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_RX_INT_ANALOG_SIG_REPORT
*/
typedef struct rlMonRxIntAnaSigRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit      STATUS_FLAG for monitor \n
                     0        STATUS_SUPPLY_RX0 \n
                     1        STATUS_SUPPLY_RX1 \n
                     2        STATUS_SUPPLY_RX2 \n
                     3        STATUS_SUPPLY_RX3 \n
                     4        STATUS_DCBIAS_RX0 \n
                     5        STATUS_DCBIAS_RX1 \n
                     6        STATUS_DCBIAS_RX2 \n
                     7        STATUS_DCBIAS_RX3 \n
                     8        STATUS_PWRDET_RX0 \n
                     9        STATUS_PWRDET_RX1 \n
                     10       STATUS_PWRDET_RX2 \n
                     11       STATUS_PWRDET_RX3 \n
                     15:12     RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlMonRxIntAnaSigRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing information
* about Internal PM, CLK and LO subsystems' internal analog signals. RadarSS sends this to host
* at the programmed periodicity or when failure occurs, as programmed by the configuration API
* SB. Event: RL_RF_AE_MON_PMCLKLO_INT_ANA_SIG_REPORT
*/
typedef struct rlMonPmclkloIntAnaSigRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit      STATUS_FLAG for monitor \n
                     0        STATUS_SUPPLY_PMCLKLO \n
                     1        STATUS_DCBIAS_PMCLKLO \n
                     2        STATUS_PWRDET_PMCLKLO \n
                     3        STATUS_SYNC_20G \n
                     15:4     RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Monitored 20G power\n
     *         1 LSB = 0.5 dBm
     */
    rlInt8_t sync20GPower;
#else
    /**
     * @brief  Monitored 20G power\n
     *         1 LSB = 0.5 dBm
     */
    rlInt8_t sync20GPower;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon \n
                     exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonPmclkloIntAnaSigRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing information
* about Internal PM, CLK and LO subsystems' internal analog signals. RadarSS sends this to host
* at the programmed periodicity or when failure occurs, as programmed by the configuration API
* SB. Event: RL_RF_AE_MON_GPADC_INT_ANA_SIG_REPORT
*/
typedef struct rlMonGpadcIntAnaSigRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit      STATUS_FLAG for monitor \n
                     0        STATUS_GPADC_REF1 \n
                     1        STATUS_GPADC_REF1 \n
                     15:12    RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS  \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
    /**
     * @brief  The measured GPADC outputs corresponding to internal DC signal (GPADC_REF1, \n
                     expected level 0.45V) is reported here. \n
                     1 LSB = 1.8V/1024  \n
     */
    rlInt16_t gpadcRef1Val;
    /**
     * @brief  The measured GPADC outputs corresponding to internal DC signal (GPADC_REF2, \n
                     expected level 1.2V) is reported here. \n
                     1 LSB = 1.8V/1024  \n
     */
    rlUInt16_t gpadcRef2Val;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlMonGpadcIntAnaSigRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing the measured PLL
* control voltage values during explicit monitoring chirps. RadarSS sends this to host at the
* programmed periodicity or when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_PLL_CONTROL_VOLT_REPORT
*/
typedef struct rlMonPllConVoltRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit      STATUS_FLAG for monitor \n
                     0        STATUS_APLL_VCTRL \n
                     1        STATUS_SYNTH_VCO1_VCTRL_MAX_ FREQ \n
                     2        STATUS_SYNTH_VCO1_VCTRL_MIN_ FREQ \n
                     3        STATUS_SYNTH_VCO1_SLOPE \n
                     4        STATUS_SYNTH_VCO2_VCTRL_MAX_ FREQ \n
                     5        STATUS_SYNTH_VCO2_VCTRL_MIN_ FREQ \n
                     6        STATUS_SYNTH_VCO2_SLOPE \n
                     15:7    RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS \n
     */
    rlUInt16_t statusFlags;
    /**
      * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
      */
    rlUInt16_t errorCode;
    /**
     * @brief  The measured values of PLL control voltage levels and Synthesizer VCO slopes are \n
                     reported here. Byte numbers corresponding to different control voltage \n
                     values reported in this field are here:  \n
                     Bytes   SIGNAL                              1 LSB \n
                     1:0     APLL_VCTRL                          1mV  \n
                     3:2     SYNTH_VCO1_VCTRL_MAX_ FREQ          1mV \n
                     5:4     SYNTH_VCO1_VCTRL_MIN_ FREQ          1mV \n
                     7:6     SYNTH_VCO1_SLOPE                    1MHz/V \n
                     9:8     SYNTH_VCO2_VCTRL_MAX_ FREQ          1mV \n
                     11:10   SYNTH_VCO2_VCTRL_MIN_ FREQ          1mV \n
                     13:12   SYNTH_VCO2_SLOPE                    1MHz/V \n
                     15:14   RESERVED                            RESERVED \n
                     Only the fields corresponding to the enabled monitors are valid.  \n
                     The failure thresholds are based on the following: \n
                     Valid VCTRL values are [140 to 1400] mV. \n
                     Valid VCO1_SLOPE values are [1760 to 2640] MHz/V. \n
                     Valid VCO2_SLOPE values are [3520 to 5280] MHz/V. \n
     */
    rlInt16_t pllContVoltVal[8U];
    /**
      * @brief  Reserved for future use
      */
    rlUInt32_t reserved;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width)
     */
    rlUInt32_t timeStamp;
}rlMonPllConVoltRep_t;

/*! \brief
* This is the Monitoring report which RadarSS sends to the host, containing information about
* the relative frequency measurements. RadarSS sends this to host at the programmed periodicity or
* when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_DCC_CLK_FREQ_REPORT
*/
typedef struct rlMonDccClkFreqRep
{
    /**
     * @brief  Status flags indicating pass fail results corresponding to various threshold \n
                     checks under this monitor. \n
                     Bit      STATUS_FLAG for monitor \n
                     0        STATUS_CLK_PAIR0 \n
                     1        STATUS_CLK_PAIR1 \n
                     2        STATUS_CLK_PAIR2 \n
                     3        STATUS_CLK_PAIR3 \n
                     4        STATUS_CLK_PAIR4 \n
                     5        STATUS_CLK_PAIR5 \n
                     15:6    RESERVED \n
                     0 - FAIL or check wasn't done, 1 - PASS \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief  Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;
    /**
     * @brief  The measured clock frequencies from the enabled clock pair measurements are \n
                     reported here.Byte numbers corresponding to different frequency measurement \n
                     values reported in this field are here: \n
                     Bytes   CLOCK PAIR  MEASURED CLOCK FREQUENCY \n
                     1:0     0           BSS_600M \n
                     3:2     1           BSS_200M \n
                     5:4     2           BSS_100M \n
                     7:6     3           GPADC_10M \n
                     9:8     4           RCOSC_10M \n
                     11:10   5           RAMPGEN_100M \n
                     15:12q  RESERVED    RESERVED \n
                     1 LSB = 0.1 MHz, unsigned number \n
     */
    rlUInt16_t freqMeasVal[8U];
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
    /**
     * @brief  This field indicates when the last monitoring in the enabled set was performed. \n
                     1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit \n
                     width) \n
     */
    rlUInt32_t timeStamp;
}rlMonDccClkFreqRep_t;


/*! \brief
* This is the Monitoring report which the xWR device sends to the host, containing the
* measured RX mixer input voltage swing values. The xWR device sends this to host at the
* programmed periodicity or when failure occurs, as programmed by the configuration API SB.
* Event: RL_RF_AE_MON_RX_MIXER_IN_PWR_REPORT
*/
typedef struct rlMonRxMixrInPwrRep
{
    /**
     * @brief Status flags indicating pass fail results corresponding \n
                   to various threshold checks under this monitor. \n
                   Bit      STATUS_FLAG for monitor \n
                   0        STATUS_MIXER_IN_POWER_RX0 \n
                   1        STATUS_MIXER_IN_POWER_RX1 \n
                   2        STATUS_MIXER_IN_POWER_RX2 \n
                   3        STATUS_MIXER_IN_POWER_RX3 \n
                   15:4     RESERVED          \n
                   0 - FAIL or check wasn't done, 1 - PASS \n
     */
    rlUInt16_t statusFlags;
    /**
     * @brief Indicates any error reported during monitoring Value of 0 indicates no error
     */
    rlUInt16_t errorCode;

#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Profile Index for which this monitoring report applies.
     */
    rlUInt8_t profIndex;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief The measured RX mixer input voltage swing values are \n
                reported here. The byte location of the value for each \n
                receivers is tabulated here- \n
                     Byte location \n
                Rx0  0  \n
                Rx1  1  \n
                Rx2  2  \n
                Rx3  3  \n
                1 LSB = 1800 mV/256, unsigned number \n
                Only the entries of enabled RX channels are valid. \n
     */
     rlUInt32_t rxMixInVolt;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved2;
    /**
     * @brief This field indicates when the last monitoring in the enabled set was performed. \n
               1 LSB = 1 millisecond (the stamp rolls over upon exceeding allotted bit width) \n
     */
    rlUInt32_t timeStamp;
}rlMonRxMixrInPwrRep_t;


#include <ti/control/mmwavelink/include/rl_device.h>
#include <ti/control/mmwavelink/include/rl_sensor.h>
#include <ti/control/mmwavelink/include/rl_monitoring.h>
#include <ti/control/mmwavelink/include/rl_protocol.h>
#include <ti/control/mmwavelink/include/rl_messages.h>


/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */

#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF MMWAVELINK_H
 */

