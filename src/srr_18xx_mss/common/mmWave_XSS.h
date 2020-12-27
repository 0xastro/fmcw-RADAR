/*******************************************************************************
 * @file    mmWave_XSS.h
 * @brief   This is the main common header file for both the MSS and DSS
 * @author  A. Astro, a.astro7x@gmail.com
 * @date    Jan 13 2020
 * @version 0.1
 *******************************************************************************/


#ifndef MMWAVE_XSS_H
#define MMWAVE_XSS_H

/* To be enabled on MSS Core */
#define MSS
/* To be enabled on DSS Core */
//#define DSS

/* MMWAVE Driver Include Files */
#include <ti/common/mmwave_error.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/osal/DebugP.h>


/* BIOS/XDC Include Files */
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Event.h>
/* MMWAVE library Include Files */
#include <ti/control/mmwave/mmwave.h>
#include <common/app_cfg.h>

/* MMW Demo Include Files */
#ifdef DSS
#include "dss_data_path.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*! @brief   sensor start CLI event
 *  SYS/BIOS events are a means of communication between Tasks and threads
 *  SYS/BIOS objects include semaphores, mailboxes, message queues, etc.
 *  Only tasks can WAIT for events;
 *  whereas tasks, Hwis, Swis, or SYS/BIOS objects can POST them.
    -ref
    http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/sysbios/6_41_02_41/exports/bios_6_41_02_41/docs/cdoc/ti/sysbios/knl/Event.html
 */
/*
 #define MMWDEMO_CLI_SENSORSTART_EVT                    Event_Id_00
///> @brief   sensor stop CLI  event
#define MMWDEMO_CLI_SENSORSTOP_EVT                      Event_Id_01
///> @brief   sensor frame start CLI  event
#define MMWDEMO_CLI_FRAMESTART_EVT                      Event_Id_02
///> @brief   BSS CPUFAULT event
#define MMWDEMO_BSS_CPUFAULT_EVT                        Event_Id_03
///> @brief   BSS ESMFAULT event
#define MMWDEMO_BSS_ESMFAULT_EVT                        Event_Id_04
///> @brief   Monitoring report event
#define MMWDEMO_BSS_MONITORING_REP_EVT                  Event_Id_05
///> @brief   BSS Calibration report event
#define MMWDEMO_BSS_CALIBRATION_REP_EVT                 Event_Id_06
///> @brief   start completed event from DSS/MSS
#define MMWDEMO_DSS_START_COMPLETED_EVT                 Event_Id_07
///> @brief   stop completed event from DSS
#define MMWDEMO_DSS_STOP_COMPLETED_EVT                  Event_Id_08
///> @brief   start failed event from DSS/MSS
#define MMWDEMO_DSS_START_FAILED_EVT                    Event_Id_09
*/

#define DSS_START_COMPLETED_EVT                 Event_Id_07


/* All CLI events */
#define MMWDEMO_CLI_EVENTS                              (MMWDEMO_CLI_SENSORSTART_EVT    | \
                                                         MMWDEMO_CLI_SENSORSTOP_EVT     | \
                                                         MMWDEMO_CLI_FRAMESTART_EVT)


/* All BSS faults events */
#define MMWDEMO_BSS_FAULT_EVENTS                        (MMWDEMO_BSS_CPUFAULT_EVT | \
                                                         MMWDEMO_BSS_ESMFAULT_EVT )




#ifdef DSS
/**
 * @brief
 *  Millimeter Wave Demo state
 *
 * @details
 *  The enumeration is used to hold the data path states for the
 *  Millimeter Wave demo
 */
typedef enum MmwDemo_DSS_STATE_e
{
    /*! @brief   State after data path is initialized */
    MmwDemo_DSS_STATE_INIT = 0,

    /*! @brief   State after data path is started */
    MmwDemo_DSS_STATE_STARTED,

    /*! @brief   State after data path is stopped */
    MmwDemo_DSS_STATE_STOPPED,

    /*! @brief   State after STOP request was received by DSP
                 but complete stop is on-going */
    MmwDemo_DSS_STATE_STOP_PENDING

}MmwDemo_DSS_STATE;

typedef struct MmwDemo_DSS_STATS_t
{
    /*! @brief   Counter which tracks the number of config events
                 The config event is triggered in mmwave config callback function
                 when remote sends configuration */
    uint8_t      configEvt;

    /*! @brief   Counter which tracks the number of start events
                 The start event is triggered in mmwave start callback function
                 when remote calls mmwave_start() */
    uint8_t      startEvt;

    /*! @brief   Counter which tracks the number of stop events
                 The start event is triggered in mmwave stop callback function
                 when remote calls mmwave_stop() */
    uint8_t      stopEvt;

    /*! @brief   Counter which tracks the number of chirp interrupt skipped due to stopped state of sensor */
    uint32_t     chirpIntSkipCounter;

    /*! @brief   Counter which tracks the number of chirp interrupt skipped due to stopped state of sensor */
    uint32_t     frameIntSkipCounter;

    /*! @brief   Counter which tracks the number of chirp interrupt detected */
    uint32_t     chirpIntCounter;

    /*! @brief   Counter which tracks the number of frame start interrupt  detected */
    uint32_t     frameStartIntCounter;

    /*! @brief   Counter which tracks the number of chirp event detected
                 The chirp event is triggered in the ISR for chirp interrupt */
    uint32_t     chirpEvt;

    /*! @brief   Counter which tracks the number of frame start event detected
                 The frame start event is triggered in the ISR for frame start interrupt */
    uint32_t     frameStartEvt;

    /*! @brief   Counter which tracks the number of frames triggered in BSS detected
                 The frame trigger event is triggered in the mmwave async event callback function */
    uint32_t     frameTrigEvt;

    /*! @brief   Counter which tracks the number of Failed Timing Reports received from BSS  */
    uint32_t     numFailedTimingReports;

    /*! @brief   Counter which tracks the number of Calibration Report received from BSS  */
    uint32_t     numCalibrationReports;

    /*! @brief   Counter which tracks the number of times saving detected objects in
                 logging buffer is skipped */
    uint32_t     detObjLoggingSkip;

    /*! @brief   Counter which tracks the number of times saving detected objects in
                 logging buffer has an error */
    uint32_t     detObjLoggingErr;
}MmwDemo_DSS_STATS;

#else
/** ---------------------------------------------------
 * @brief
 * The structure is used to hold the statistics
 * information for the Millimeter Wave Application
 * Design
 * ---------------------------------------------------
 */
typedef struct mmW_MSS_STATS_t
{

    uint8_t      cliSensorStartEvt;                 ///! CLI event for sensorStar
    uint8_t      cliSensorStopEvt;                  ///! CLI event for sensorStop
    uint8_t      cliFrameStartEvt;                  ///! CLI event for frameStart
                                                    ///! The event below are triggered in mmwave start callback function
    uint8_t      datapathConfigEvt;                 ///! Counter which tracks the number of datapath config
    uint8_t      datapathStartEvt;                  ///! Counter which tracks the number of datapath start event detected
    uint8_t      datapathStopEvt;                   ///! Counter which tracks the number of datapath stop event detected
                                                    ///! The events below are triggered by asynchronous events from the BSS
    uint32_t     numFailedTimingReports;            ///! Counter which tracks the number of failed calibration reports
    uint32_t     numCalibrationReports;             ///! Counter which tracks the number of calibration reports received
}mmW_MSS_STATS;
#endif

/** ---------------------------------------------------
 * \brief
 *  DSP-Subsystem (DSS) Master control block (MCB)
 *  The structure is used to hold handling information,
 *  flags and stats relative to the radar design
 *
 *   ---------------------------------------------------
 */
typedef struct MCB_t
{

    SOC_Handle                  socHandle;                            ///! Handle to the SOC Module
    Mbox_Handle                 mboxHandle;                           ///! Handle to the peer Mailbox  used to exchange messages between the MSS and DSS
    Semaphore_Handle            mboxSemHandle;                        ///! Semaphore handle for the mailbox communication
    SOC_SysIntListenerHandle    chirpIntHandle;                       ///! Handle to the SOC chirp interrupt listener Handle
    SOC_SysIntListenerHandle    frameStartIntHandle;                  ///! Handle to the SOC frame interrupt listener Handle
    MMWave_Handle               ctrlHandle;                           /** \brief mmWave control handle use to initialize the link infrastructure,
                                                                       * which allows communication between the MSS and BSS

                                                                       */

#ifdef DSS
    ADCBuf_Handle               adcBufHandle;                         ///! Handle to the ADCBUF Driver
    MmwDemo_DSS_STATE           state;                                ///!  mmw Demo state
    MmwDemo_DSS_STATS           stats;                                ///!  mmw Demo statistics
    MmwDemo_DSS_DataPathObj     dataPathObj[NUM_SUBFRAMES];           ///!  Data Path object
    uint8_t                     loggingBufferAvailable;               ///!  Logging buffer flag
    uint8_t                     subframeIndx;                         ///! Subframe index
    uint8_t                     interFrameProcToken;                  ///! inter frameProc token
    uint8_t                     frameStartIntToken;                   ///! frame start token
    uint8_t                     chirpProcToken;                       ///! chirpProc token
    uint8_t                     mboxProcToken;                        ///! 'mailbox has a message' token
    uint8_t                     frameProcToken;                       ///! frame process token

#else
    Event_Handle                eventHandle;                          ///! MSS system event handle
    UART_Handle                 loggingUartHandle;                    ///! UART Logging Handle
    UART_Handle                 commandUartHandle;                    ///! UART Command Handle used to interface with the CLI
    mmW_MSS_STATS           stats;                                /// !mmWave stats
    bool                        cfgStatus;                            ///! flag which indicates if the mmWave link has been configured
    bool                        runningStatus;                        ///! flag which indicates if the radar is transmitting or not
    bool                        isMMWaveOpen;                         ///! flag which indicates if the basic radar configuration is completed.
    int32_t                     frameStartToken;                      ///! token for frame start events.
    int32_t                     subframeCntFromChirpInt;              ///! The number of sub-frames transmitted derived from  chirp available interrupts.
    int32_t                     subframeCntFromFrameStart;            ///! The number of subframes transmitted derived from the frame start interrupts.
    int32_t                     chirpIntcumSum;                       ///! The total number of chirp available interrupts.
    int32_t                     chirpInt;                             ///! A counter for chirp interrupts. It is reset every subframe.
    int32_t                     numChirpsPerSubframe[NUM_SUBFRAMES];  ///! The number of chirps per subframe.
    int32_t                     subframeId;                           ///! An indicator for the current subframe.
#endif

}MCB;

extern MCB gMCB;

#ifdef DSS
extern void _MmwDemo_dssAssert(int32_t expression, const char *file, int32_t line);
#define MmwDemo_dssAssert(expression) {                                      \
                                    _MmwDemo_dssAssert(expression,           \
                                             __FILE__, __LINE__);         \
                                    DebugP_assert(expression);            \
                                   }
#else
/*******************************************************************************************
 * Extern CLI API:
 *******************************************************************************************/
extern void MSS_CLIInit (void);

#endif


/*******************************************************************************************
 * Extern CFG API:
 *******************************************************************************************/
extern void Cfg_AdvFrameCfgInitParams (rlAdvFrameCfg_t* ptrAdvFrameCfg);
extern void Cfg_FrameCfgInitParams (rlFrameCfg_t* ptrFrameCfg);
extern void Cfg_ProfileCfgInitParams (uint8_t profileNum, rlProfileCfg_t* ptrProfileCfg);
extern void Cfg_ChirpCfgInitParams (uint8_t chirpNum, rlChirpCfg_t* ptrChirpCfg);
extern void Cfg_LowPowerModeInitParams (rlLowPowerModeCfg_t* ptrLowPowerMode);
extern void Cfg_ChannelCfgInitParams (rlChanCfg_t* ptrChannelCfg);
extern void Cfg_ADCOutCfgInitParams (rlAdcOutCfg_t* ptrADCOutCfg);

#ifdef __cplusplus
}
#endif

#endif /* MMWAVE_XSS */
