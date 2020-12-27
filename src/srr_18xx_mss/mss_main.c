/*******************************************************************************
 * @file    mss_main.c
 * @brief   The application runs on top of the Master Sub-system (MSS)
 *          which is powered by an ARM Cortex-R4F processor
 *          Main tasks is to running user Application code.
 *          Control RF-Front End core Cortex-R4F Radar subsystem (BSS)
 *          via API messages.Perform radar signal processing.
 *          Finally, It Sends obtained raw-data to the DSP subsystem for
 *          further digital processing to the (DSS) core.
 * @author  A. Astro, a.astro7x@gmail.com
 * @date    Jan 13 2020
 * @version 0.1
 *******************************************************************************/


/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

// Standard Include Files
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// BIOS/XDC  Include Files
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

// mmWave SDK Include Files
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/common/sys_common.h>


// TI Drivers Include Files
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/crc/crc.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/pinmux/include/pinmux_xwr18xx.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/canfd/canfd.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/cbuff/cbuff.h>

// Utilities Include Files
#include <ti/utils/testlogger/logger.h>
#include <ti/utils/cli/cli.h>

// Application Include Files
#include "common/mmw_messages.h"
#include "common/mmWave_XSS.h"
#include <common/app_cfg.h>


#define TASK_PRIO_1	1
#define TASK_PRIO_2	2
#define TASK_PRIO_3	3
#define TASK_PRIO_4	4
#define TASK_PRIO_5	5
#define TASK_PRIO_6	6



/* ******************************************************************************
	ENABLE APPLICATION SPECIFIC INSTRUCTIONS
 *******************************************************************************/
// #define CAN
// #define CAN_FD
// #define AWR1843_GPIO


#if defined  (CAN)  || defined (CAN_FD)

/*
 *   CAN Driver Based Functions -----------------------
 * 	 @CAN Related
 *	 Adopted from SPRACF7 April 2018
 * 	 Document Name: "Adding CAN-FD Tx and Rx to an Exi-
 * 	 sting mmWave Project"-----------------------------
 */
static void MCANAppInitParams(CANFD_MCANInitParams* mcanCfgParams);
static void MCANAppCallback(CANFD_MsgObjHandle handle, CANFD_Reason reason);
static void MCANAppErrStatusCallback(CANFD_Handle handle, CANFD_Reason reason, CANFD_ErrStatusResp* errStatusResp);
static void Can_Initialize(void);
static uint32_t Get_CanMessageIdentifier(MmwDemo_output_message_type type);
static int32_t Can_Transmit_Schedule( uint32_t msg_id, uint8_t *txmsg, uint32_t len);

/* User-Defined Function to deploy the CAN module in our application */
static void CAN_init(void);

/*---------------------------------------------------
 * Enable CANFD confugiuration from MCAN Module
 * MCAN := Modular Controller Area Network
 * Possible Configs: Classical CAN, CAN FD
 */

#ifdef CAN
CANFD_MCANFrameType     frameType = CANFD_MCANFrameType_CLASSIC;
#endif

#ifdef CANFD
CANFD_MCANFrameType     frameType = CANFD_MCANFrameType_FD;
#endif

/*
 *   Public-scope Variables DEFINITIONS ---------------
 * 	 @CAN Related
 *	 Adopted from SPRACF7 April 2018
 * 	 Document Name: "Adding CAN-FD Tx and Rx to an Exi-
 * 	 sting mmWave Project"-----------------------------
 */
volatile uint32_t       gTxDoneFlag = 0;
volatile uint32_t       gRxDoneFlag = 0;
volatile uint32_t       gTxPkts = 0;
volatile uint32_t 		gRxPkts = 0;
volatile uint32_t 	 	gErrStatusInt = 0;
volatile uint32_t		iterationCount = 0U;
uint32_t                dataLength = 0U;
uint8_t                 rxData[64U];
uint32_t                txDataLength;
uint32_t                rxDataLength;

CANFD_Handle                canHandle;
CANFD_MsgObjHandle          txMsgObjHandle;
CANFD_MCANMsgObjCfgParams   txMsgObjectParams;

#endif



#pragma DATA_ALIGN(gMCB, 16);
/** ---------------------------------------------------
 *  \brief gMCB structure contains the tracking
 *  information required by the design
 *  is aligned using DATA_ALIGN paragma
 *  to increase the performance of MSS core by aligning
 *  the structure gMCB (Master Control Block) to be
 *  divisable by 16.
 *  TI-reference: spnu151j.pdf section #5.10.6
 *  ----------------------------------------------------
 */
MCB	gMCB;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */ // MCPI_LOGBUF_INIT(9216);


 /* ******************************************************************************
 	mmWave SPECIFIC Functions
  *******************************************************************************/

static void MSS_mmWaveInitTASK (UArg arg0, UArg arg1);
static void MSS_mmWaveConfigCallbackFxn(MMWave_CtrlCfg* ptrCtrlCfg);
static void MSS_chirpIntCallback(uintptr_t arg);
static void MSS_mmWaveStartCallbackFxn(MMWave_CalibrationCfg* ptrCalibrationCfg);
static void MSS_mmWaveStopCallbackFxn(void);
static void MSS_frameStartIntCallback(uintptr_t arg);
static void MSS_mmWaveCtrlTask (UArg arg0, UArg arg1);
static void MSS_mmWaveOpenCallbackFxn(MMWave_OpenCfg* ptrOpenCfg);
static void MSS_mmWaveCloseCallbackFxn(void);
static int32_t MSS_eventFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload);

/* ******************************************************************************
	Mailbox SPECIFIC Functions
 *******************************************************************************/
static void mboxCallbackFxn_MSS_ch0(Mbox_Handle handle, Mailbox_Type peer);
static void mboxIn_uartOut_TASK(UArg arg0, UArg arg1);
int32_t MSS_mboxWrite(mmWaveMSG	*msg);


/*@**********************************************************************
 * MSS_chirpIntCallback
 ************************************************************************/
 /**  \brief
 *  This is the callback function registered with the ADC Driver
 *  which is invoked when a chirp is available. This is executed
 *  in the ISR context  which is registered to the listener function
 *  listenerFxn; defined within SOC_SysIntListenerCfg. Hence; In our
 *  design, there's no frame segmentation (Advanced Frame Configuration)
 *  (in other words, frame := 1 sub-frame).
 *  However, advanced frame configuration will allow the use of
 *  different subframes in the radar configuration to support
 *  and meet the requirements of different ranges:
 *  i.e ultra-short, short, medium, long ranges.
 *  Accordingly, different subframes results
 *  in different chirp sizes which is totally depending on the each
 *  sub-frameType. Therefore, It's important to identify this for each
 *  sub-frame before the start of the next subframe, at the end of the
 *  previous subframes last chirp.
 *
 *  \arg[0]  Application registered argument
 *
 *  @todo	 Add sub-frames configurations and measure the performance
 *
 */
static void MSS_chirpIntCallback(uintptr_t arg) {

	gMCB.chirpInt++;
	//gMCB.chirpIntcumSum++;

	if (gMCB.chirpInt == gMCB.numChirpsPerSubframe[gMCB.subframeId]) {
		gMCB.subframeCntFromChirpInt++;
		gMCB.subframeId = gMCB.subframeId + 1;
		if (gMCB.subframeId == NUM_SUBFRAMES) {
			gMCB.subframeId = 0;
		} else {
			return;
		}
		gMCB.chirpInt = 0;
	} else {
		return;
	}
}

/*@**********************************************************************
 * MSS_frameStartIntCallback
 ************************************************************************/
/**
 *  \brief
 *  This is the callback function registered with the ADC Driver
 *  which is invoked when a frame is started. This is executed
 *  in the ISR context which is registered to the listener function
 *  listenerFxn; defined within SOC_SysIntListenerCfg
 *  Inside the function we Check if the frames are coming correctly,
 *  and no chirps have been missed.
 *	\arg[0]  Application registered argument
 *	\return  void
 */

static void MSS_frameStartIntCallback(uintptr_t arg) {

	gMCB.frameStartToken++;
	//gMSSMCB.subframeCntFromFrameStart++;
	if (gMCB.frameStartToken == 1) {
		if (gMCB.chirpInt == 0) {
			gMCB.frameStartToken = 0;
		}else {
			DebugP_assert(0);
		}
	} else {
		return;
	}
}

/*@**********************************************************************
 * MSS_eventFxn
 ************************************************************************/
/**
 *  @brief
 *  This is a registered event function which is invoked when an
 *  event is recieved from the BSS. This event is passed to the peer
 *  domain/core BSS.
 *  @arg[0] Message Identifier
 *  @arg[1] Subblock identifier
 *  @arg[2] Length of the subblock
 *  @arg[3] Pointer to the payload buffer
 *  @return 0
 */

static int32_t MSS_eventFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload) {

    System_printf ("Debug: BSS Event MsgId: %d [Sub Block Id: %d Sub Block Length: %d]\n",msgId, sbId, sbLen);
#if 0
	uint16_t asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);
	// Process the received message
	switch (msgId) {
		case RL_RF_ASYNC_EVENT_MSG: {
			// Received Asychronous Message:
			switch (asyncSB) {
				case RL_RF_AE_CPUFAULT_SB: {
					// Post event to datapath task notify BSS events
					//Event_post(gMSSMCB.eventHandle, MMWDEMO_BSS_CPUFAULT_EVT);
					break;
				}
				case RL_RF_AE_ESMFAULT_SB: {
					/* Post event to datapath task notify BSS events */
					//Event_post(gMSSMCB.eventHandle, MMWDEMO_BSS_ESMFAULT_EVT);
					break;
				}
				case RL_RF_AE_INITCALIBSTATUS_SB: {
					/* This event should be handled by mmwave internally, ignore the event here */
					break;
				}
				case RL_RF_AE_FRAME_TRIGGER_RDY_SB: {
					break;
				}
				case RL_RF_AE_MON_TIMING_FAIL_REPORT_SB: {
					break;
				}
				case RL_RF_AE_RUN_TIME_CALIB_REPORT_SB: {
					break;
				}
				default: {
					System_printf ("[MSS] \t [ERROR] Asynchronous Event SB Id %d not handled\n", asyncSB);
					break;
				}
			}
			break;
		}
		default: {
		   System_printf ("[MSS] \t [ERROR] Asynchronous message %d is NOT handled\n", msgId);
			 break;
		}
	}
#endif
	return 0;
}

/*@**********************************************************************
 * MSS_mmWaveConfigCallbackFxn
 ************************************************************************/
/**
 * \brief
 *   This is the application registered callback function which is invoked
 *   after the configuration has been used to configure the mmWave link
 *   and the BSS.
 *
 *  \arg[0]  Pointer to the control configuration
 *  \return  void
 */
//***********************************************************************/

static void MSS_mmWaveConfigCallbackFxn(MMWave_CtrlCfg* ptrCtrlCfg) {

	DebugP_assert(0);
}

/*@**********************************************************************
 * MSS_mmWaveStartCallbackFxn
 ************************************************************************/
/**
 * \brief
 *  Application registered callback function which is invoked on the
 *  peer domain just before the mmWave link is started on the BSS.
 *  \arg[0]		Pointer to the calibration configuration
 *  \return		void
 */

static void MSS_mmWaveStartCallbackFxn(MMWave_CalibrationCfg* ptrCalibrationCfg) {

	/* Post an event to main data path task. 
	   This function in only called when mmwave_start() is called on DSS */
	gMCB.stats.datapathStartEvt ++;

	/* Post event to Notify that event start is done */
	Event_post(gMCB.eventHandle, DSS_START_COMPLETED_EVT);
}

/*@**********************************************************************
 * MSS_mmWaveStopCallbackFxn
 ************************************************************************/
/**
 * \brief
 *  This is the application registered callback function which is invoked
 *  after the MSS has been stopped. Hence, possible sceanarios:
 *  1. CLI sensorStop command triggers mmwave_stop() to be called from MSS
 *  2. In case of Error, mmwave_stop() will be triggered from MSS or DSS
 *  \arg	N/A
 *  \return N/A
 */

static void MSS_mmWaveStopCallbackFxn(void) {

	gMCB.stats.datapathStopEvt ++;
}

/*@**********************************************************************
 * MSS_mmWaveOpenCallbackFxn
 ************************************************************************/
/**
 * \brief
 *  This is the application registered callback function which is invoked
 *  after the MSS has been opened.
 *  \arg[0]	Pointer to the open configuration
 *  \return N/A
 */
static void MSS_mmWaveOpenCallbackFxn(MMWave_OpenCfg* ptrOpenCfg) {

	return;
}

/*@**********************************************************************
 * MSS_mmWaveCloseCallbackFxn
 ************************************************************************/
/**
 * \brief
 *  This is the application registered callback function which is invoked
 *  after the MSS has been closed.
 *  \arg	N/A
 *  \return N/A
 */

static void MSS_mmWaveCloseCallbackFxn(void){

	return;
}

/*@**********************************************************************
 * MmwDemo_mboxWrite
 ************************************************************************/
/**
 * \brief
 *  \arg[0]	Pointer to the MMW demo message ASTRO.
 *  \return
 *      Success    - 0
 *      Fail       < -1
 */

int32_t MSS_mboxWrite(mmWaveMSG	*msg) {

	int32_t	retVal = -1;
	
	retVal = Mailbox_write (gMCB.mboxHandle, (uint8_t*)msg, sizeof(mmWaveMSG));
	if (retVal == sizeof(mmWaveMSG)) {
		retVal = 0;
	}
	return retVal;
}

/*@**********************************************************************
 * mboxIn_uartOut_TASK
 ************************************************************************/
/**
 * \brief
 * The Task is used to handle the recieved messages from the DSS Peer 
 * over the mailbox virtual communication channel.
 *  \arg[0] arg0 of the Task.
 *  \arg[1] arg1 of the Task.
 *  \return
 *      Success		-0
 *      Fail		< -1
 */

static void mboxIn_uartOut_TASK(UArg arg0, UArg arg1)
{
	mmWaveMSG      msg;
	int32_t        retVal = 0;
	
	/* wait for new message and process all the messages received from the peer */
	while(1) {

		Semaphore_pend(gMCB.mboxSemHandle, BIOS_WAIT_FOREVER);
		
		/* Read the message from the peer mailbox: We are not trying to protect the read
		 * from the peer mailbox because this is only being invoked from a single thread */
		retVal = Mailbox_read(gMCB.mboxHandle, (uint8_t*)&msg, sizeof(mmWaveMSG));
		if (retVal < 0) {
			/* Error: Unable to read the message. Setup the error code and return values */
			System_printf ("[MSS] \t [Error][MAILBOX]: Mailbox read failed [Error code %d]\n", retVal);
		}
		else if(retVal == 0) {
			/* We are done: There are no messages available from the peer execution domain. */
			continue;
		}
		else {
			/* Flush out the contents of the mailbox to indicate that we are done with the message. This will
			 * allow us to receive another message in the mailbox while we process the received message. */
			Mailbox_readFlush(gMCB.mboxHandle);

			/* Process the received message: */
			switch(msg.type) {
				case MBOX_DSS2MSS_DETOBJ_READY:
					/* Got detected objects , to be shipped out through UART */
					{
						uint32_t totalPacketLen;
						uint32_t numPaddingBytes;
						uint32_t itemIdx;
						uint32_t AddressSOC;

						/* Send header */
						totalPacketLen = sizeof(mmWave_OUT_MSG_header);
						UART_writePolling (gMCB.loggingUartHandle,
										   (uint8_t*)&msg.body.detObj.header,
										   sizeof(mmWave_OUT_MSG_header));
#if defined  (CAN)  || defined (CAN_FD)
						txMsgObjectParams.msgIdentifier = Get_CanMessageIdentifier((MmwDemo_output_message_type)MMWDEMO_HEADER);
              			Can_Transmit_Schedule( txMsgObjectParams.msgIdentifier, (uint8_t*)&message.body.detObj.header,sizeof(mmWave_OUT_MSG_header));
#endif

						/* Send TLVs */
						for (itemIdx = 0;  itemIdx < msg.body.detObj.header.numTLVs; itemIdx++){

							UART_writePolling (gMCB.loggingUartHandle,
											   (uint8_t*)&msg.body.detObj.tlv[itemIdx],
											   sizeof(mmWave_OUT_MSG_tl));

							/* Address has to be translated from the DSS core
							 * SOC_translateAddress
							 * SOC_translateAddress(<some pointer>, SOC_TranslateAddr_Dir_FROM_OTHER_CPU, NULL)
							 * must be used on any pointers that came from the DSP
							 *  in order to access the corresponding data.
							 *  This is easy to overlook, but you will get garbage results or cause other problems
							 *  if you don't translate the pointer before accessing its contents.
							 */
							AddressSOC = SOC_translateAddress(msg.body.detObj.tlv[itemIdx].address, SOC_TranslateAddr_Dir_FROM_OTHER_CPU, NULL);
                            UART_writePolling (gMCB.loggingUartHandle,
                                               (uint8_t*)AddressSOC,
                                               msg.body.detObj.tlv[itemIdx].length);

#if defined  (CAN)  || defined (CAN_FD)
							txMsgObjectParams.msgIdentifier = Get_CanMessageIdentifier((MmwDemo_output_message_type)msg.body.detObj.tlv[itemIdx].type);
							Can_Transmit_Schedule(txMsgObjectParams.msgIdentifier, (uint8_t*)&msg.body.detObj.tlv[itemIdx],sizeof(mmWave_OUT_MSG_tl));
							Can_Transmit_Schedule( txMsgObjectParams.msgIdentifier, (uint8_t*)SOC_translateAddress(msg.body.detObj.tlv[itemIdx].address,SOC_TranslateAddr_Dir_FROM_OTHER_CPU,NULL), msg.body.detObj.tlv[itemIdx].length);
#endif
							/* The totalPacketLen element of the detection results header
							 *  must be a multiple of 32 bytes.
							 *  The DSP rounds up to the nearest multiple,
							 *  but when modifying it in the MSS you must redo this step after making any additions.
							 *  You may notice data missing in your serial frames if you don't round to the proper length.*/
							totalPacketLen += sizeof(mmWave_OUT_MSG_tl) + msg.body.detObj.tlv[itemIdx].length;
						}

						/* Send padding to make total packet length multiple of MMWDEMO_OUTPUT_MSG_SEGMENT_LEN */
						numPaddingBytes = MMW_OUTPUT_MSG_SEGMENT_LEN - (totalPacketLen & (MMW_OUTPUT_MSG_SEGMENT_LEN-1));
						if (numPaddingBytes<MMW_OUTPUT_MSG_SEGMENT_LEN) {

							uint8_t padding[MMW_OUTPUT_MSG_SEGMENT_LEN];
							memset(&padding, 0xf, MMW_OUTPUT_MSG_SEGMENT_LEN);
							UART_writePolling (gMCB.loggingUartHandle, padding, numPaddingBytes);

#if defined  (CAN)  || defined (CAN_FD)
                            txMsgObjectParams.msgIdentifier = Get_CanMessageIdentifier((MmwDemo_output_message_type)MMWDEMO_PADDING);
							Can_Transmit_Schedule( txMsgObjectParams.msgIdentifier, padding,numPaddingBytes);
#endif
						}
					}

					/* Send a message to MSS to log the output data */
					memset((void *)&msg, 0, sizeof(mmWaveMSG));
					msg.type = MBOX_MSS2DSS_DETOBJ_SHIPPED;
								
					retVal = MSS_mboxWrite(&msg);
					
					if (retVal != 0) {
						System_printf ("[MSS] \t [ERROR][MAILBOX] Mailbox send message id=%d failed \n", msg.type);
					}

					break;
				
				case MBOX_DSS2MSS_ASSERT_INFO:
				{
				    /* Send the received DSS assert info through CLI */
					CLI_write("[MSS] \t [DSS][EXCEP] DSS Exception: %s, line %d.\n", msg.body.assertInfo.file, msg.body.assertInfo.line);
					break;
				}
				default:
				{
					/* Message not support */
					System_printf ("[MSS] \t [ERROR][MAILBOX] unsupport Mailbox message id=%d\n", msg.type);
					break;
				}
			}
		}
	}
}

/*@**********************************************************************
 * mboxCallbackFxn_MSS_ch0
 ************************************************************************/
/**
 *  \brief
 *  mboxCallbackFxn_MSS_ch0 is a callback function. Function is invoked
 *  for each received messege from the DSS peer endpoint.
 *  When invoked, release the resources and wakeup the mmWave thread to
 *  process the received message.
 *  Hence: the address of this function is passed to mboxCfg.readCallback
 *
 *  \arg[0]  Handle to the Mailbox on which data was received
 *  \arg[1]  Peer from which data was received (DSS)
 *  \return  void
 */

static void mboxCallbackFxn_MSS_ch0(Mbox_Handle handle, Mailbox_Type peer)
{
	Semaphore_post(gMCB.mboxSemHandle);
}

/*@**********************************************************************
 * MSS_mmWaveCtrlTask
 ************************************************************************/
/**
 *  \brief
 *  This is the task which provides an execution
 *  context for the mmWave control module.
 *
 *  \arg[0]  arg0 Argument0 with which the task was created
 *  \arg[1]  arg1 Argument1 with which the task was created
 *  \return  void
 */

static void MSS_mmWaveCtrlTask (UArg arg0, UArg arg1) {
	int32_t errCode;

	while (1) {
		/* Execute the mmWave control module: */
		if (MMWave_execute (gMCB.ctrlHandle, &errCode) < 0) {

			System_printf ("Error: mmWave control execution failed [Error code %d]\n", errCode);
		}
	}
}


/*@**********************************************************************
 * MSS_mmWaveInitTASK
 ************************************************************************/
/**
 * \brief
 * MSS Initialization Task which initializes the various
 * components in the MSS subsystem including UART for
 * logging and configurations and Mailbox for
 * bi-directional communication between MSS and DSS
 * subsystems.
 *
 * \arg[0] Argument0 with which the task was created
 * \arg[1] Argument1 with which the task was created
 * \return void
 */

static void MSS_mmWaveInitTASK (UArg arg0, UArg arg1) {

	Task_Params             taskParams;
	UART_Params             uartParams;
	Semaphore_Params 		semParams;
	Mailbox_Config 			mboxCfg;
	SOC_SysIntListenerCfg   listenerCfg;
	MMWave_InitCfg          initCfg;
	int32_t                 errCode;

	/* ************************************************
	 * Initialize and Configure the Device Drivers
	 * UART, CAN, Mailbox, and GPIOs
	 * ************************************************/
	System_printf("[MSS] \t Launched the Initialization Task\n");

	/**
	 * > setup the PINMUX (pin multiplexing) for bringing out the UART; (UART settings configuring, I/O cell characteristics specification)
	 */ //\ref PINMUXTOOL-V4-CLOUD

	Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PINN5_PADBE, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);    
	Pinmux_Set_FuncSel(SOC_XWR18XX_PINN5_PADBE, SOC_XWR18XX_PINN5_PADBE_MSS_UARTA_TX);

	Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PINN4_PADBD, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);    
	Pinmux_Set_FuncSel(SOC_XWR18XX_PINN4_PADBD, SOC_XWR18XX_PINN4_PADBD_MSS_UARTA_RX);

	Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PINF14_PADAJ, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
	Pinmux_Set_FuncSel(SOC_XWR18XX_PINF14_PADAJ, SOC_XWR18XX_PINF14_PADAJ_MSS_UARTB_TX);

	Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PINP8_PADBM, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
	Pinmux_Set_FuncSel(SOC_XWR18XX_PINP8_PADBM, SOC_XWR18XX_PINP8_PADBM_DSS_UART_TX);

	/** > Initialize the UART */
	UART_init();
	System_printf("[MSS] \t UART initialized .. \n");

	/** > Initialize the CAN interface
	 * \todo CAN Interface is Semi-deployed in this system due to the hardware limitations. It Will be added anyway for future considerations
	 */
    #if defined  (CAN)  || defined (CAN_FD)
	CAN_init();
    System_printf("[MSS] \t CAN initialized .. \n");
    #endif

#ifdef AWR1843_GPIO
	/** > Initialize the GPIO */
    GPIO_init();
    System_printf("[MSS] \t GPIO initialized .. \n");
#endif

    /** > Initialize the Mailbox */
	Mailbox_init(MAILBOX_TYPE_MSS);
	if(Mailbox_Config_init(&mboxCfg) < 0) {
        System_printf("[MSS] \t [Error][MAILBOX] Unable to initialize configuration\n");
	}else {
	    System_printf("[MSS] \t [Mailbox] Mailbox initialized .. \n");
	}

	/* ************************************************
	 * Open and Configure the Device Drivers
	 * UART, CAN, Mailbox, and GPIOs
	 * ************************************************/

	/* Init Params, Open and Setup the UART Driver: Command Instance */
	UART_Params_init(&uartParams);
	uartParams.baudRate			= 115200;
	uartParams.isPinMuxDone		= 1;
	uartParams.clockFrequency	= MSS_SYS_VCLK;
	 	 	 	 	 	 	 	  //MSS_SYS_VCLK := 200MHz

	gMCB.commandUartHandle	= UART_open(0, &uartParams);
	if (gMCB.commandUartHandle == NULL) {
	    System_printf("[MSS] \t [Error][UART] Unable to open the Command UART Instance\n");
	} else {
	    System_printf("[MSS] \t [UART][CFG_PORT] open the Command UART Instance \n");
	    System_printf("[MSS] \t [UART][CFG_PORT] clockFrequency=%fMHz \n", uartParams.clockFrequency/1e6);
	    System_printf("[MSS] \t [UART][CFG_PORT] baudRate=%d \n",uartParams.baudRate);
     }


	/* Init Params, Open and Setup the UART Driver: Logging Instance */
	UART_Params_init(&uartParams);
	uartParams.writeDataMode	= UART_DATA_BINARY;
	uartParams.readDataMode		= UART_DATA_BINARY;
	uartParams.baudRate			= 115200*8;
	uartParams.isPinMuxDone		= 1U;
	uartParams.clockFrequency 	= MSS_SYS_VCLK;
								 //MSS_SYS_VCLK : 200MHz
	gMCB.loggingUartHandle 	= UART_open(1, &uartParams);
	if (gMCB.loggingUartHandle == NULL) {
		System_printf("[MSS] \t [Error] Unable to open the logging UART Instance\n");
	} else {
	    System_printf("[MSS] \t [UART][DATA_PORT] open the logging UART Instance \n");
	    System_printf("[MSS] \t [UART][DATA_PORT] clockFrequency=%fMHz \n", uartParams.clockFrequency/1e6);
	    System_printf("[MSS] \t [UART][DATA_PORT] baudRate=%d \n",uartParams.baudRate);
	}


	/* ************************************************
	 * Creating communication channel between MSS & DSS
	 * ************************************************/

	// Create a binary semaphore to handle mailbox interrupt
	Semaphore_Params_init(&semParams);
	semParams.mode			= Semaphore_Mode_BINARY;
	gMCB.mboxSemHandle		= Semaphore_create(0, &semParams, NULL);

	// Setup the default mailbox configuration
	Mailbox_Config_init(&mboxCfg);
	// Setup the configuration:
	mboxCfg.chType			= MAILBOX_CHTYPE_MULTI;
	mboxCfg.chId			= MAILBOX_CH_ID_0;
	mboxCfg.writeMode		= MAILBOX_MODE_BLOCKING;
	mboxCfg.readMode		= MAILBOX_MODE_CALLBACK;
	mboxCfg.readCallback	= &mboxCallbackFxn_MSS_ch0;

	/*
	 * 	typedef void(* Mailbox_Callback) (Mbox_Handle handle, Mailbox_Type remoteEndpoint)
	 *  void mboxCallbackFxn_MSS_ch0(Mbox_Handle handle, Mailbox_Type peer) {
	 *          sem_post();
	 *  }
	 *  mboxCfg.readCallback	= &mboxCallbackFxn_MSS_ch0;
	 *
	 *  This creates a type, named Mailbox_Callback which is  a pointer to a function that
	 *  takes a 2 inputs handle: Which is returned by Mailbox_open
	 *  and  Mailbox_Type which is could be MAILBOX_TYPE_MSS or MAILBOX_TYPE_DSS
	 *  and returns nothing, which matches the signature of the function
	 *  mboxCallbackFxn_MSS_ch0.
	 *  To use it; we assign the function reference to the pointer readCallback
	 */

	// Initialization of Mailbox Virtual Channel
	gMCB.mboxHandle = Mailbox_open(MAILBOX_TYPE_DSS, &mboxCfg, &errCode);
	if (gMCB.mboxHandle == NULL) {
		System_printf("[MSS] \t [Error][MAILBOX] Unable to open the Mailbox to the DSS [Error code %d]\n", errCode);
	} else {
	    System_printf("[MSS] \t [MAILBOX] Opening Mailbox Channel to the DSP Subsystem [DSS] .. \n");
	}

	// Create task to handle mailbox messages
	Task_Params_init(&taskParams);
	taskParams.stackSize = 16*1024;
	Task_create(mboxIn_uartOut_TASK, &taskParams, NULL);
	
	// Register Chirp Available Listener, populate configurations
	// The hardware event SOC_XWR18XX_MSS_CHIRP_AVAIL_IRQ is the only way to know when the samples are available in the ADC buffer.
	// https://e2e.ti.com/support/sensors/f/1023/t/837299?IWR1443-ADC-data-to-L3
	memset ((void*)&listenerCfg, 0, sizeof(SOC_SysIntListenerCfg));
	listenerCfg.systemInterrupt   = SOC_XWR18XX_MSS_CHIRP_AVAIL_IRQ; 	// Chirp Available Interrupt
	listenerCfg.listenerFxn       = MSS_chirpIntCallback; 				//TestFmk_chirpISR
	listenerCfg.arg               = 0;

	gMCB.chirpIntHandle = SOC_registerSysIntListener (gMCB.socHandle, &listenerCfg, &errCode);
	if (gMCB.chirpIntHandle == NULL) {
		System_printf ("[MSS] \t [Error] Unable to register the Chirp Available Listener/Interrupt [Error code %d]\n", errCode);
		return;
	}

	// Register Frame Start Listener,  populate configurations
	memset ((void*)&listenerCfg, 0, sizeof(SOC_SysIntListenerCfg));
	listenerCfg.systemInterrupt   = SOC_XWR18XX_MSS_FRAME_START_INT; //Frame Start Interrupt
	listenerCfg.listenerFxn       = MSS_frameStartIntCallback;
	listenerCfg.arg               = 0;

	gMCB.frameStartIntHandle = SOC_registerSysIntListener (gMCB.socHandle, &listenerCfg, &errCode);

	if (gMCB.frameStartIntHandle == NULL) {
		System_printf("[MSS] \t [Error] Unable to register the Frame start Listener/Interrupt [Error code %d]\n", errCode);
		return ;
	}

	/* Initialize the mmWave module:*/
	memset ((void *)&initCfg, 0, sizeof(MMWave_InitCfg));

	/* Populate the initialization configuration: 
	 * The MMWAve is configured in minimal isolation mode. */
	initCfg.domain                      = MMWave_Domain_MSS;
	initCfg.socHandle                   = gMCB.socHandle;
	initCfg.eventFxn                    = &MSS_eventFxn;
	initCfg.cfgMode                     = MMWave_ConfigurationMode_MINIMAL; 
	initCfg.executionMode               = MMWave_ExecutionMode_ISOLATION;
	initCfg.linkCRCCfg.useCRCDriver     = 1U;
	initCfg.linkCRCCfg.crcChannel       = CRC_Channel_CH1;
	initCfg.cooperativeModeCfg.cfgFxn   = &MSS_mmWaveConfigCallbackFxn;
	initCfg.cooperativeModeCfg.startFxn = &MSS_mmWaveStartCallbackFxn;
	initCfg.cooperativeModeCfg.stopFxn  = &MSS_mmWaveStopCallbackFxn;
	initCfg.cooperativeModeCfg.openFxn  = &MSS_mmWaveOpenCallbackFxn;
	initCfg.cooperativeModeCfg.closeFxn = &MSS_mmWaveCloseCallbackFxn;
	
	// Initialize and setup the mmWave Control module
	gMCB.ctrlHandle = MMWave_init(&initCfg, &errCode);
	if (gMCB.ctrlHandle == NULL) {
		// Error: Unable to initialize the mmWave control module
		System_printf ("[MSS] \t [Error][Init][mmWAVE] mmWave Control Initialization failed [Error code %d]\n", errCode);
		return;
	} else {
		System_printf ("[MSS] \t [Debug][Init][mmWAVE] Initialized the mmWave module\n");
	}

	// Synchronize the mmWave module:
	while (1) {
		int32_t syncStatus;

		// Get the synchronization status:
		syncStatus = MMWave_sync(gMCB.ctrlHandle, &errCode);
		if (syncStatus < 0) {
			// Error: Unable to synchronize the mmWave control module
			System_printf ("Error: mmWave Control Synchronization failed [Error code %d]\n", errCode);
			return;
		}
		if (syncStatus == 1) {
			// Synchronization acheived:
			System_printf ("[MSS] \t [SYNCH][mmWAVE]: mmWave Control Synchronization Achieved\n");
		    // MCPI_setFeatureTestResult("MMWave MSS Synchronization", MCPI_TestResult_PASS);
			break;
		}
		// Sleep and poll again:
		Task_sleep(1);
	}
	
	System_printf ("[MSS] \t [DEBUG][SYNCH] Synchronized the mmWave module\n");

	// This one is defined @common/mrr_config_consts.h
	#ifdef SUBFRAME_CONF_MRR_USRR
		gMCB.numChirpsPerSubframe[0] = SUBFRAME_MRR_NUM_CHIRPS_TOTAL;
		gMCB.numChirpsPerSubframe[1] = SUBFRAME_USRR_NUM_CHIRPS_TOTAL;
	#else 
	#ifdef SUBFRAME_CONF_MRR
		gMCB.numChirpsPerSubframe[0] = SUBFRAME_MRR_NUM_CHIRPS_TOTAL;
	#endif 
	#ifdef SUBFRAME_CONF_USRR
		gMCB.numChirpsPerSubframe[0] = SUBFRAME_USRR_NUM_CHIRPS_TOTAL;
	#endif
	#endif


	/*
	 * Launch the mmWave control execution task
	 * This should have a higher priroity than any other task which uses the
	 * mmWave control API
	 */
	Task_Params_init(&taskParams);
	taskParams.priority  = TASK_PRIO_6;
	taskParams.stackSize = 3*1024;
	Task_create(MSS_mmWaveCtrlTask, &taskParams, NULL);

	// Setup the CLI
	MSS_CLIInit();
}


//********************************************************
//* ********************MAIN******************************
//********************************************************
/**
 * @brief
 * Main Program entrypoint.
 * This is the entrypoint for the MSS firmware
 *  which describe the startup sequence for the
 *  AWR1843 application running on top of MSS.
 *  The MSS application must have the following
 *  startup sequence at bootup time.
 *  Clear all the ESM group errors and register
 *  Disable the firewall for JTAG and LOGGER (UART)
 *  Start the BIOS and further initialize required
 *  peripherals.
 *
 * @arg    N/A
 * @return 0
 */

 //**********************************************************

int32_t main(void) {

    Task_Params     taskParams; //<  define Task_Params instance config structure
	int32_t         errCode;    //<  define an integer to retrieve error messeges
	SOC_Cfg         socCfg;     //<  define SOC_Cfg structure to to open & initialize the SOC Driver.

	/* Initialize Error Signaling Module MSS_ESM for hardware diagnostics within the MSS core */
	ESM_init(0U);
	//MCPI_Initialize();
	/* Initialize the global structure Master Control Block MCB */
	memset((void*)&gMCB, 0, sizeof(MCB));

	/* Initialize the SOC configuration: */
	memset((void *)&socCfg, 0, sizeof(SOC_Cfg));

	/* Populate the SOC configuration: Set the clock source on 200MHz */
	socCfg.clockCfg = SOC_SysClock_INIT;

	/* Initialize the SOC Module to ensure that the Memory Protection Unit
	 * (MPU) settings  are correctly configured.
	 */

	gMCB.socHandle = SOC_init (&socCfg, &errCode);
	if (gMCB.socHandle == NULL) {
		System_printf ("[MSS] \t [Error][SOC] SOC Module Initialization failed [Error code %d]\n", errCode);
		return -1;
	}

	/* Check if the SOC is a secure device */
	if (SOC_isSecureDevice(gMCB.socHandle, &errCode)) {
		/* Disable firewall for JTAG and LOGGER (UART) */
		SOC_controlSecureFirewall(gMCB.socHandle,
		                          (uint32_t)(SOC_SECURE_FIREWALL_JTAG | SOC_SECURE_FIREWALL_LOGGER),
		                          SOC_SECURE_FIREWALL_DISABLE, &errCode);
	}

    System_printf ("**********************************************************\n");
    System_printf ("[MSS] \t [DEBUG] Launching the Millimeter Wave Application\n");
    System_printf ("**********************************************************\n");

	/* Initialize the Task Parameters. */
	Task_Params_init(&taskParams);
	taskParams.priority = TASK_PRIO_1; // Default 3
	Task_create(MSS_mmWaveInitTASK, &taskParams, NULL);

	/* Start BIOS */
	BIOS_start();
	return 0;
}

