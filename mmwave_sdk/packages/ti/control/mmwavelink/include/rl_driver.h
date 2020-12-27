/****************************************************************************************
 * FileName     : rl_driver.h
 *
 * Description  : This file implements the mmwave radar Host Communication Protocol.
 *
 ****************************************************************************************
 * (C) Copyright 2014, Texas Instruments Incorporated. - TI web address www.ti.com
 *---------------------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */


/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
#ifndef RL_DRIVER_H
#define RL_DRIVER_H

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include "rl_datatypes.h"
#include "rl_protocol.h"
#include "rl_messages.h"

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************************
* MACRO DEFINITIONS
****************************************************************************************
*/


#define RL_API_CMD_RETRY_COUNT              (3U)     /* RHCP Retry Count */
#define RL_SBC_ERR_SB_SIZE                  (4U)     /* RHCP ERROR SB Size */
#define RL_PROTOCOL_ALIGN_SIZE              (4U) /* Alignment required for CRC computation,
                                                4 - 16/32bit CRC, 8 - 64bit CRC*/
#define RL_PROTOCOL_DUMMY_BYTE              (0xFFU) /* Dummy Byte used by Protocol*/
#define RL_RSP_ERR_MSG_LEN                  (SYNC_PATTERN_LEN + RHCP_HEADER_LEN + \
                                            RL_SBC_ERR_SB_SIZE + RL_CRC_LEN_MAX)

/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/*! \brief
* RHCP Message Direction
*/
#define RL_API_DIR_INVALID                  (0x0U)
#define RL_API_DIR_HOST_TO_BSS              (0x1U)
#define RL_API_DIR_BSS_TO_HOST              (0x2U)
#define RL_API_DIR_HOST_TO_DSS              (0x3U)
#define RL_API_DIR_DSS_TO_HOST              (0x4U)
#define RL_API_DIR_HOST_TO_MSS              (0x5U)
#define RL_API_DIR_MSS_TO_HOST              (0x6U)
#define RL_API_DIR_BSS_TO_MSS               (0x7U)
#define RL_API_DIR_MSS_TO_BSS               (0x8U)
#define RL_API_DIR_BSS_TO_DSS               (0x9U)
#define RL_API_DIR_DSS_TO_BSS               (0xAU)
#define RL_API_DIR_MSS_TO_DSS               (0xBU)
#define RL_API_DIR_DSS_TO_MSS               (0xCU)
#define RL_API_DIR_RESRVD                   (0xFU)

/*! \brief
* RHCP Message Class
*/
#define RL_API_CLASS_CMD                    (0x0U)
#define RL_API_CLASS_RSP                    (0x1U)
#define RL_API_CLASS_NACK                   (0x2U)
#define RL_API_CLASS_ASYNC                  (0x3U)

#define RL_API_CLASS_BYPASS                 (0x4U)
#define RL_API_CLASS_MAX                    (0x5U)

/*! \brief
* RHCP retry flag
*/
#define RL_HDR_FLAG_NO_RETRY               (0x0U)
#define RL_HDR_FLAG_RETRY                  (0x3U)

/*! \brief
* RHCP ACK flag
*/
#define RL_HDR_FLAG_ACK                    (0x0U)
#define RL_HDR_FLAG_NO_ACK                 (0x3U)

/*! \brief
* Command Request type : SET/GET
*/
#define RL_API_RTYP_SET                     (0x0U)
#define RL_API_RTYP_GET                     (0x1U)

/*! \brief
* RHCP protocol version value
*/
#define RL_PROT_VERSION_0                   (0x0U)
#define RL_PROT_VERSION_MAX                 (0xFU)

/*! \brief
* RHCP CRC flag
*/
#define RL_HDR_FLAG_CRC                     (0x0U)
#define RL_HDR_FLAG_NO_CRC                  (0x3U)

/*! \brief
* RHCP CRC length flag value
*/
#define RL_HDR_FLAG_CRC_16BIT               (0x0U)
#define RL_HDR_FLAG_CRC_32BIT               (0x1U)
#define RL_HDR_FLAG_CRC_64BIT               (0x2U)

/*! \brief
* RHCP CRC Data
*/
typedef rlUInt16_t rlCrcData_t;

/*! \brief
* Payload Byte Alighment
*/
#define RL_PAYLOAD_MSG_4BYTE_MULT       (0x4U)     /* MSG must be multiple of this value */
#define RL_PAYLOAD_MSG_8BYTE_MULT       (0x8U)     /* MSG must be multiple of this value */

/*! \brief
* RHCP Payload Structure
*/
 typedef struct rlPayloadSb
{
    rlUInt16_t    sbid;
    rlUInt16_t    len;
    rlUInt8_t     *pSblkData;
}rlPayloadSb_t;


/*! \brief
* RHCP Payload Structure
*/
typedef struct rlFixPayloadSb
{
   rlUInt16_t    sbid;
   rlUInt16_t    len;
   rlUInt8_t     sblkData[240];
}rlFixPayloadSb_t;


/*! \brief
* RHCP Async Event structure
*/
typedef struct rlAsyncEvt
{
    rlRhcpMsg_t evtMsg;
}rlAsyncEvt_t;

/*! \brief
* The passing message type of Tx task of API module
*  - Pass the op code ID to frame message (command type)
*  - Pass ACK type to request from receiver (command type)
*/
typedef struct rlDriverRespMsg
{
    rlProtHeader_t         hdr;
    rlPayloadSb_t*         subblocks;
    rlUInt8_t              isSendToSender;
}rlDriverRespMsg_t;

/*! \brief
* mmwave radar Driver Function Params
*/
typedef struct rlFunctionParams
{
    rlRhcpMsg_t*       cmd;
    rlRhcpMsg_t*       rsp;
    rlAsyncEvt_t       asyncEvt;
    rlUInt8_t            rxMsgClass;
    rlUInt8_t            alignReserved1;
    rlUInt8_t            alignReserved2;
    rlUInt8_t            alignReserved3;
    rlUInt8_t            msgCRC[RL_CRC_LEN_MAX];
}rlFunctionParams_t;

/*! \brief
* Communication handle and device-index for deifferent devices connected to Host
*/
typedef struct rlComDevInx
{
    /**
     * @brief  Communication Interface Handles
     */
    rlComIfHdl_t        comIfHdl[RL_DEVICE_CONNECTED_MAX];
    /**
     * @brief  stores device Index
     */
    rlUInt8_t           rlDevIndex[RL_DEVICE_CONNECTED_MAX];
}rlComDevInx_t;

/*! \brief
* mmwave Logging functions
*/
typedef struct loggingFunctions
{
    rlPrintFptr rlPrintAr[RL_DBG_LEVEL_VERBOSE];
}rlLogCtx_t;

/*! \brief
* mmwave radar Driver Global Structure
*/
typedef struct rlDriverData
{
    /**
     * @brief  Current API parameters
     */
    rlFunctionParams_t  funcParams;
    /**
     * @brief  Driver Status
     */
    rlUInt8_t           isDriverInitialized;
    /**
     * @brief  Bitmap of devices connected radarSS/DSS Mailbox in case of 16xx autonomous
     */
    rlUInt8_t           deviceMap;
    /**
     * @brief  Driver Command Wait Flag
     */
    volatile rlUInt8_t    isCmdRespWaited[RL_DEVICE_CONNECTED_MAX];
    /**
     * @brief  if writing a data waits for Host IRQ
     */
    rlUInt8_t             isRespWriteWaited[RL_DEVICE_CONNECTED_MAX];
    /**
     * @brief  Driver Host Interrupt count
     */
    volatile rlUInt8_t    rxIrqCnt[RL_DEVICE_CONNECTED_MAX];
    /**
     * @brief  Driver serviced Host Interrupt count
     */
    volatile rlUInt8_t    rxDoneCnt[RL_DEVICE_CONNECTED_MAX];
    /**
     * @brief  Driver command sequnce number
     */
    rlUInt16_t             cmdSeqNum[RL_DEVICE_CONNECTED_MAX];
    /**
     * @brief Communication handle and device-index for deifferent devices connected to Host
     */
    rlComDevInx_t        commDevIdx;
    /**
     * @brief  Driver Global Lock Mutex
     */
    rlOsiMutexHdl_t     globalMutex;
    /**
     * @brief  Driver Command Wait Semaphore
     */
    rlOsiSemHdl_t       cmdSem;
    /**
     * @brief  Driver Spawn queue
     */
    rlOsiMsgQHdl_t      spawnQueue;
    /**
     * @brief  Client context
     */
    rlClientCbs_t       clientCtx;
    /**
     * @brief  As per debug level callback functions will be assinged
     */
    rlLogCtx_t          logObj;
    rlRhcpMsg_t         *txMsgPtr;
    rlRhcpMsg_t         *rxMsgPtr;
}rlDriverData_t;

typedef struct rlSyncHeader
{
    rlSyncPattern_t         syncPattern;
    rlProtHeader_t          protHdr;
}rlSyncHeader_t;

/*! \brief
* mmwave radar Driver Protocol header read buffer
*/
typedef union rlReadBuf
{
    rlUInt8_t               tempBuf[RHCP_HEADER_LEN + SYNC_PATTERN_LEN];
    rlSyncHeader_t          syncHeader;
}rlReadBuf_t;

/*! \brief
* mmwave radar Driver Opcode
*/
typedef struct rlDriverOpcode
{
    /**
     * @brief  Message Direction
     */
    rlUInt8_t dir;
    /**
     * @brief  Message Class
     */
    rlUInt8_t msgType;
    /**
     * @brief  Message Id
     */
    rlUInt16_t msgId;
    /**
     * @brief  Number of Sub Blocks in Payload
     */
    rlUInt16_t nsbc;
}rlDriverOpcode_t;

/*! \brief
* mmwave radar Driver Payload
*/
typedef struct rlDriverMsg
{
    rlDriverOpcode_t opcode;
    rlPayloadSb_t*    subblocks;
    rlUInt16_t remChunks;
}rlDriverMsg_t;


/**
*  @defgroup Communication_Protocol Communication Protocol
*  @brief mmwave Communication Driver Module
*
*  The mmwave radar Host Communication Driver Module implements the mmwave radar
*  communication protocol:
    -# It is a simple stop and wait protocol. Each message needs to be acknowledged
        by receiver before next message can be sent.
    -# Messages are classifieds as "Command", "Response" and "Asynchronous Event"
    -# If Command can not be processed immediately, ACK response is sent immediately
        (If requested). "Asynchronous Event"  is sent upon completion of the command
        execution.
*
*  @addtogroup Communication_Protocol
*  @{
*/


/******************************************************************************
 * FUNCTION DECLARATIONS
 ******************************************************************************
 */
rlReturnVal_t rlDriverInit(rlUInt8_t deviceMap, rlClientCbs_t clientCb);
rlReturnVal_t rlDriverOsiInit(void);
rlReturnVal_t rlDriverAddDevice(rlUInt8_t deviceMap);
rlReturnVal_t rlDriverRemoveDevices(rlUInt8_t deviceMap);
rlReturnVal_t rlDriverDeInit(void);
rlDriverData_t* rlDriverGetHandle(void);
rlUInt8_t rlDriverGetPlatformId(void);
rlUInt8_t rlDriverGetArDeviceType(void);
rlUInt8_t rlDriverGetNoOfConnectedDev(void);
rlUInt8_t rlDriverGetCmdDir(rlUInt16_t msgId);
rlReturnVal_t rlDriverIsDeviceMapValid(rlUInt8_t deviceMap);
rlReturnVal_t rlDriverWaitForResponse(rlUInt8_t devIndex, rlDriverMsg_t* outMsg);
rlReturnVal_t rlDriverCmdInvoke(rlUInt8_t deviceMap, rlDriverMsg_t inMsg,
                                rlDriverMsg_t* outMsg);
rlReturnVal_t rlDriverSendResponse(rlRhcpMsg_t *outMsg);
rlReturnVal_t rlDriverSendMsg(rlUInt8_t devIndex, rlUInt8_t* inMsg, rlUInt16_t inLen);
rlReturnVal_t rlDriverConfigureCrc(rlCrcType_t crcType);
rlReturnVal_t rlDriverConfigureAckTimeout(rlUInt32_t ackTimeout);

rlReturnVal_t rlDriverCalCRC(rlUInt8_t* data,rlUInt16_t dataLen,
                             rlUInt8_t crcType, rlUInt8_t crc[RL_CRC_LEN_MAX]);

rlReturnVal_t rlDriverVerifyCRC(rlUInt8_t* data, rlUInt16_t dataLen,
                                rlUInt8_t crcType, rlUInt8_t crc[RL_CRC_LEN_MAX]);
void rlDriverShiftDWord(rlUInt8_t buf[]);
rlReturnVal_t rlDriverCalChkSum(rlProtHeader_t* hdrData, rlUInt8_t len, rlUInt16_t* checksum);
rlReturnVal_t rlDriverValidateHdr(rlProtHeader_t protHdr);
rlReturnVal_t rlDriverAsyncEventHandler(rlUInt8_t devIndex, rlUInt16_t nsbc,
                                        rlUInt8_t *payload,
                                        rlUInt16_t payloadLen);
rlUInt8_t rlDeviceIdentifyCmdDir(rlUInt16_t msgId, rlUInt8_t platform);
void rlDriverHostIrqHandler(rlUInt8_t deviceIndex, void *pValue);
rlUInt8_t rlDriverGetDeviceIndex(rlUInt16_t dataDir);
rlReturnVal_t rlDriverMsgReadSpawnCtx(const void *pValue);
rlReturnVal_t rlDriverMsgReadCmdCtx(rlUInt8_t devIndex);
rlReturnVal_t rlDriverMsgRead(rlDriverData_t* rlDrvData, rlUInt8_t devIndex);
rlReturnVal_t rlDriverMsgWrite(rlDriverData_t* rlDrvData, rlComIfHdl_t comIfHdl);
rlReturnVal_t   rlDriverRxHdrRead(rlUInt8_t hdrBuf[RHCP_HEADER_LEN],
                                  rlComIfHdl_t comIfHdl);
rlUInt8_t rlDriverPendingRxMsg(void);
void rlDriverFillPayload(rlUInt16_t msgId, rlUInt16_t sbcID, rlPayloadSb_t* payloadPtr,
                         rlUInt8_t* data, rlUInt16_t inLen);
void rlDriverConstructInMsg(rlUInt16_t msgId, rlDriverMsg_t* inMsg,
                            rlPayloadSb_t* payloadPtr);
void rlDriverConstructOutMsg(rlUInt16_t numSblk,  rlDriverMsg_t* outMsg,
                             rlPayloadSb_t* payloadPtr);
rlReturnVal_t rlDriverExecuteGetApi(rlUInt8_t deviceMap, rlUInt16_t msgId,
                                    rlUInt16_t sbcID, rlUInt8_t *msgData, rlUInt16_t inLen);
rlReturnVal_t rlDriverExecuteSetApi(rlUInt8_t deviceMap, rlUInt16_t msgId,
                                    rlUInt16_t sbcID, rlUInt8_t *msgData, rlUInt16_t inLen);
rlReturnVal_t rlDriverRdVerifyAsync(rlReadBuf_t readBuf, rlUInt8_t devIndex,
                                    rlUInt16_t rxLengthRecv);
rlReturnVal_t rlDriverProcRdMsg(rlUInt8_t devIdx, rlReturnVal_t inVal);
rlReturnVal_t rlDriverMsgCmdReply(rlDriverData_t* rlDrvData, rlUInt8_t devIndex);
rlReturnVal_t rlDriverRdVerifyCmd(rlReadBuf_t readBuf, rlUInt8_t devIndex,
                                  rlUInt16_t rxLengthRecv);
rlReturnVal_t rlDriverRdVerifyMsg(rlReadBuf_t readBuf, rlUInt8_t devIndex);
rlReturnVal_t rlDriverRdVerifyRsp(rlReadBuf_t readBuf, rlUInt8_t devIndex,
                                  rlUInt16_t rxLengthRecv);
rlReturnVal_t rlDriverCmdSendRetry(rlUInt8_t deviceMap, rlDriverMsg_t* outMsg);

/*!
 Close the Doxygen group.
 @}
 */

#ifdef __cplusplus
}
#endif

#endif

/*
 * END OF RL_DRIVER_H FILE
 */
