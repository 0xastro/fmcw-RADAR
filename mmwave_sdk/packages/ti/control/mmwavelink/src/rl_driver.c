/****************************************************************************************
 * FileName     : rl_driver.c
 *
 * Description  : This file implements the mmwave radar Host Communication
 *                Protocol
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

 /*
 ****************************************************************************************
 * Revision History   :
 *---------------------------------------------------------------------------------------
 * Version  Date        Author             Defect No               Description
 *---------------------------------------------------------------------------------------
 * 0.1.0    12May2015   Kaushal Kukkar    -               Initial Version
 *
 * 0.5.2    23Sep2016   Kaushal Kukkar    AUTORADAR-541   xWR1642 Support
 *
 * 0.6.0    15Nov2016   Kaushal Kukkar    AUTORADAR-666   Logging Feature
 *                      Kaushal Kukkar    AUTORADAR-716   Cascade API change
 *
 * 0.7.0    11May2017   Kaushal Kukkar    MMWSDK-362      LDRA static analysis Issue Fix
 *
 * 0.8.6    24Jul2017   Jitendra Gupta    MMWL-25         Support for 2K Message size
 *                      Kaushal Kukkar    MMWL-23         Big Endian Support
 *
 * 0.9.1       -        Jitendra Gupta    MMWL-5          Code size optimization
 ****************************************************************************************
 */

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stdlib.h>
#include <string.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/control/mmwavelink/include/rl_messages.h>
#include <ti/control/mmwavelink/include/rl_controller.h>
#include <ti/control/mmwavelink/include/rl_protocol.h>
#include <ti/control/mmwavelink/include/rl_trace.h>

/****************************************************************************************
* MACRO DEFINITIONS
****************************************************************************************
*/
/*  Check if Interrupt is pending */
#define RL_PENDING_RX_MSG(driverData, index)   \
     (((driverData)->rxIrqCnt[(index)]) != ((driverData)->rxDoneCnt[(index)]))

/*  mmwave radar Communication Interface Read/Write  */
#define RL_IF_WRITE(fd,pBuff,len)   \
            rl_driverData.clientCtx.comIfCb.rlComIfWrite((fd),(pBuff),(len))
#define RL_IF_READ(fd,pBuff,len)    \
           rl_driverData.clientCtx.comIfCb.rlComIfRead((fd),(pBuff),(len))


/*  mmwave radar Communication Interface Read/Write Error Check  */
#define RL_IF_WRITE_CHECK(fd,pBuff,len) \
            {\
                if (((rlInt32_t)(len)) != RL_IF_WRITE((fd), (pBuff), (len)))\
                {\
                     return RL_RET_CODE_RADAR_IF_ERROR;\
                }\
            }
#define RL_IF_READ_CHECK(fd,pBuff,len)  \
            {\
                if (((rlInt32_t)(len)) != RL_IF_READ((fd), (pBuff), (len)))\
                {\
                     return RL_RET_CODE_RADAR_IF_ERROR;\
                }\
            }

/* MAX Retry of SYNC Match */
#define RL_SYNC_SCAN_THRESHOLD           (1000U)

/* mmWaveLink Header Opcode Class */
#define RL_RHCP_HDR_OPCODE_CLASS(ptr)  \
         (rlUInt8_t) (((rlProtHeader_t *)(ptr))->opcode.b2MsgType)

/* mmWaveLink Header Payload */
#define RL_RHCP_HDR_PL_LENGTH(ptr)     (((rlProtHeader_t *)(ptr))->len)

#define PATTERN_NOT_MATCHED             ((rlInt32_t)0x0)
#define SYNC_PATTERN_MATCHED            ((rlInt32_t)0x1)
#define CNYS_PATTERN_MATCHED            ((rlInt32_t)0x2)

/*  Protocol SYNC Patterns */
#define D2H_SYNC_PATTERN_1              (0xDCBAU)
#define D2H_SYNC_PATTERN_2              (0xABCDU)
#define H2D_SYNC_PATTERN_1              (0x1234U)
#define H2D_SYNC_PATTERN_2              (0x4321U)
#define H2D_CNYS_PATTERN_1              (0x5678U)
#define H2D_CNYS_PATTERN_2              (0x8765U)

/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/* mmwave radar Driver global structure */
#ifdef ccs
#pragma DATA_ALIGN(rl_driverData, 4);
#endif
static rlDriverData_t rl_driverData = {0};

/* mmwave radar Driver Command/Response Buffer */
#ifdef ccs
#pragma DATA_ALIGN(rl_txMsg, 8);
#endif
rlRhcpMsg_t rl_txMsg = {0};
#ifdef ccs
#pragma DATA_ALIGN(rl_rxMsg, 8);
#endif
rlRhcpMsg_t rl_rxMsg = {0};

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */
static rlReturnVal_t  rlDriverOriginDirCheck(rlUInt8_t deviceRunId, rlUInt16_t dataDir);
static rlUInt8_t rlDriverReceiveSync(rlComIfHdl_t comIfHdl, rlUInt8_t syncBuf[],
                                 rlInt32_t *syncType);
static rlReturnVal_t rlDriverClientCbCheck(rlClientCbs_t clientCb);
static rlReturnVal_t rlDriverOsiCbCheck(rlClientCbs_t clientCb);
static rlReturnVal_t rlDriverCmdWriter(rlUInt8_t devIndex, rlDriverMsg_t* outMsg);


/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */

/** @fn void rlDriverShiftDWord(rlUInt8_t buf[])
*
*   @brief Shifts one byte in the byte array
*   @param[in] buf - Byte array
*
*   Shifts one byte in the byte array
*/
/* SourceId :  */
/* DesignId :  */
/* Requirements :  */
void rlDriverShiftDWord(rlUInt8_t buf[])
{
    rlUInt8_t shiftIdx;

    /* shift each byte in recevied byte array */
    for (shiftIdx = 0U; shiftIdx < 7U; shiftIdx++)
    {
        /* overwritting each data byte with next data byte of array */
        buf[shiftIdx] = buf[shiftIdx + 1U];
    }

    /* set last byte to zero */
    buf[7U] = 0U;
}

/** @fn rlReturnVal_t rlDriverCalCRC(rlUInt8_t* data, rlUInt16_t dataLen,
*                      rlUInt8_t crcType, rlUInt8_t crc[RL_CRC_LEN_MAX])
*
*   @brief Calculates 16bit/32bit/64bit CRC
*   @param[in] data - Byte array
*   @param[in] dataLen - Length of the byte array
*   @param[in] crcType - Type of CRC 16/32/64bit
*   @param[in] crc - Received CRC
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Calculates 16bit/32bit/64bit CRC
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-774 */
rlReturnVal_t rlDriverCalCRC(rlUInt8_t* data, rlUInt16_t dataLen,
                           rlUInt8_t crcType, rlUInt8_t crc[RL_CRC_LEN_MAX])
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDriverCalCRC starts...\n");

    /* check if API is defined by the application during powerOn */
    if (rl_driverData.clientCtx.crcCb.rlComputeCRC != NULL)
    {
        /* compute CRC on given data */
        retVal = rl_driverData.clientCtx.crcCb.rlComputeCRC(data, dataLen, crcType, &crc[0U]);
    }
    else
    {
        /* set error code if CRC compute API is not set by Application */
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    RL_LOGV_ARG0("rlDriverCalCRC ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverVerifyCRC(rlUInt8_t* data, rlUInt16_t dataLen,
                         rlUInt8_t crcType, rlUInt8_t crc[RL_CRC_LEN_MAX])
*
*   @brief Compares received CRC with Calculated CRC
*   @param[in] data - Byte array
*   @param[in] dataLen - Length of the byte array
*   @param[in] crcType - Type of CRC 16/32/64bit
*   @param[in] crc - Received CRC
*
*   @return rlReturnVal_t CRC Check Pass - 0,
*                         CRC Check Fail - RL_RET_CODE_CRC_FAILED
*
*   Compares received CRC with Calculated CRC
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-780 */
rlReturnVal_t rlDriverVerifyCRC(rlUInt8_t* data, rlUInt16_t dataLen,
                        rlUInt8_t crcType, rlUInt8_t crc[RL_CRC_LEN_MAX])
{
    rlUInt8_t indx;
    rlUInt8_t crcByte[RL_CRC_LEN_MAX];
    rlReturnVal_t retVal = RL_RET_CODE_OK;

    RL_LOGV_ARG0("rlDriverVerifyCRC starts...\n");

    /* compute CRC on given data */
    if (RL_RET_CODE_OK != rl_driverData.clientCtx.crcCb.rlComputeCRC(data, dataLen, \
                                                                     crcType, &crcByte[0U]))
    {
        /* set error code if CRC callback returns non-zero */
        retVal += RL_RET_CODE_RADAR_IF_ERROR;
    }
    else
    {
        /* compare computed and received CRC value */
        for (indx = 0U; indx < (2U << crcType); indx++)
        {
            if (crcByte[indx] != crc[indx])
            {
                /* set error code if computed and received CRC value mismatched */
                retVal += RL_RET_CODE_CRC_FAILED;
                break;
            }
        }
    }
    RL_LOGV_ARG0("rlDriverVerifyCRC ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverCalChkSum(rlUInt8_t* data, rlUInt8_t len,
*                             rlUInt16_t* checksum)
*
*   @brief Calculates the 16 bit Checksum on a byte array
*   @param[in] hdrData - Header data
*   @param[in] len -  Length of the byte array
*   @param[out] checksum - Calcualted checksum of the byte array
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Compares Calculates the 16 bit checksum on a byte array
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-774, AUTORADAR_REQ-777 */
rlReturnVal_t rlDriverCalChkSum(rlProtHeader_t* hdrData, rlUInt8_t len,
                                   rlUInt16_t* checksum)
{
    /* TBD - MISRA compliant Checksum code */
    rlUInt32_t checkSumVal = 0U;
    rlUInt8_t *localGenHdr = (rlUInt8_t *)hdrData;
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDriverCalChkSum starts...\n");

    /* if received data pointer is not NULL */
    if (localGenHdr != RL_NULL_PTR)
    {
        /* if length is 2 or more bytes     */
        while (len > 1U)
        {
            /* AR_CODE_REVIEW MR:R.11.2 <APPROVED> "Pointer conversion
             * from data array
             * to calculate Checksum" */
            /*LDRA_INSPECTED 94 S */
            /*LDRA_INSPECTED 95 S */
            checkSumVal += *((rlUInt16_t*) localGenHdr);
            localGenHdr += 2U;

            /* If high order bit set, fold */
            if ((checkSumVal & 0x80000000U) != 0U)
            {
                checkSumVal = (checkSumVal & 0xFFFFU) + (checkSumVal >> 16U);
            }
            /* decrement length by 2 as checkSum is calculated on each 2 bytes */
            len -= 2U;
        }
        /* Take care of left over byte */
        if (len > 0U)
        {
            checkSumVal += (rlUInt32_t) (*((rlUInt8_t *)localGenHdr));
        }
        /* Add all half words to calcuated SUM */
        while ((checkSumVal >> 16U) != 0U)
        {
            checkSumVal = (checkSumVal & 0xFFFFU) + (checkSumVal >> 16U);
        }
        RL_LOGV_ARG1("Final checksum 0x%X\n", *checksum);

        /* Calculate Checksum as compliment of the SUM */
        *(checksum) = (rlUInt16_t)~checkSumVal;
        retVal = RL_RET_CODE_OK;
    }
    else
    {
        retVal = RL_RET_CODE_FATAL_ERROR;
    }

    RL_LOGV_ARG0("rlDriverCalChkSum ends...\n");

    return retVal;
}


/** @fn rlReturnVal_t rlDriverValidateHdr(rlProtHeader_t protHdr)
*
*   @brief Validates the header by comparing Checksum
*   @param[in] protHdr - Protocol Header
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Validates the header by comparing checksum
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-777 */
rlReturnVal_t rlDriverValidateHdr(rlProtHeader_t protHdr)
{
    rlReturnVal_t retVal;
    rlUInt16_t checkSum = 0U;

    /* Calculate checksum*/
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Checksum is getting updated in called function" */
    /*LDRA_INSPECTED 8 D */
    if (RL_RET_CODE_OK != rlDriverCalChkSum(&protHdr, ((rlUInt8_t)(RHCP_HEADER_LEN - 2U)),
                                                                                &checkSum))
    {
        /* Set error code if checksum callback returns non-zero value */
        retVal = RL_RET_CODE_RADAR_IF_ERROR;
    }
    else
    {
        /* Compare calcualted and received checksum*/
        if (protHdr.chksum != checkSum)
        {
            /* Checksum doesn't match, return error*/
            retVal = RL_RET_CODE_CHKSUM_FAILED;
            RL_LOGE_ARG0("Checksum validation failed for header\n");
        }
        else
        {
            /* checksum successfully matched */
            RL_LOGD_ARG0("Checksum validation is successful\n");
            retVal= RL_RET_CODE_OK;
        }
    }

    return retVal;
}

/** @fn rlUInt8_t rlDeviceIdentifyCmdDir(rlUInt16_t msgId, rlUInt8_t platform)
*
*   @brief Get the direction of command packet based on MsgID and platform
*   @param[in] msgId - Message ID of data packet
*   @param[in] platform - Platform Type where mmWaveLink instance is running
*
*   @return rlUInt8_t command packet direction
*
*   Returns direction of command packet based on MsgID and platform.
*/
/* DesignId :  */
/* Requirements :  */
rlUInt8_t rlDeviceIdentifyCmdDir(rlUInt16_t msgId, rlUInt8_t platform)
{
    rlUInt8_t cmdDir;

    RL_LOGV_ARG0("rlDeviceIdentifyCmdDir starts...\n");

    /* if MsgId is for radarSS */
    if ((RL_RF_RESP_ERROR_MSG < msgId) && (RL_RF_ASYNC_EVENT_MSG > msgId))
    {
        /* if mmWaveLink is running on MSS */
        if (RL_PLATFORM_MSS == platform)
        {
            cmdDir = RL_API_DIR_MSS_TO_BSS;
        }
        /* if mmWaveLink is running on Host */
        else if (RL_PLATFORM_HOST == platform)
        {
            cmdDir = RL_API_DIR_HOST_TO_BSS;
        }
        /* if mmWaveLink is running on DSS */
        else
        {
            cmdDir = RL_API_DIR_DSS_TO_BSS;
        }
        RL_LOGV_ARG1("cmdDir for radarSS = %d (1:host2Bss, 10:dss2Bss, 8:mss2bss)\n",\
         cmdDir);
    }
    /* if MsgId is for MSS */
    else if ((RL_DEV_POWERUP_MSG <= msgId) && (RL_DEV_ASYNC_EVENT_MSG > msgId))
    {
        /* if mmWaveLink is running on Host */
        if (RL_PLATFORM_HOST == platform)
        {
            /* MsgId for MSS need not send to MSS itself */
            cmdDir = RL_API_DIR_HOST_TO_MSS;
        }
        else
        {
            /* If MSS wants to configure these MsgID to DSS */
            cmdDir = RL_API_DIR_MSS_TO_DSS;
        }
        RL_LOGV_ARG1("cmdDir for MSS = %d (5:host2Mss, 12:dss2Mss)\n", cmdDir);
    }
    /* if msgId is for DSS */
    else if ((RL_DSP_RESERVED0_MSG <= msgId) && (RL_DSP_ASYNC_EVENT_MSG > msgId))
    {
        /* MsgId for DSS need not send to DSS itself */

        /* if mmWaveLink is running on MSS */
        if (RL_PLATFORM_MSS == platform)
        {
            /* set direction MSS_TO_DSS */
            cmdDir = RL_API_DIR_MSS_TO_DSS;
        }
        /* if mmWaveLink is running on Host */
        else
        {
            /* set direction HOST_TO_DSS */
            cmdDir = RL_API_DIR_HOST_TO_DSS;
        }
        RL_LOGV_ARG1("cmdDir for DSS = %d (11:Mss2Dss, 3:hostDss)\n", cmdDir);
    }
    else
    {
        /* set direction INVALID */
        cmdDir = RL_API_DIR_INVALID;
        RL_LOGE_ARG1("rlDeviceIdentifyCmdDir: Invalid cmd, platform: [%d]\n", platform);
    }
    RL_LOGV_ARG0("rlDeviceIdentifyCmdDir ends...\n");

    return cmdDir;
}

/** @fn rlReturnVal_t rlDriverAsyncEventHandler(rlUInt8_t devIndex,
*               rlUInt16_t nsbc, rlUInt8_t *payload, rlUInt16_t payloadLen)
*
*   @brief Handles asynchronous response/error from mmwave radar device
*   @param[in] devIndex - Device Index
*   @param[in] nsbc - No. of Sub-Block in MSG
*   @param[in] payload - Protocol payload message data
*   @param[in] payloadLen - Length of payload message data
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Handles asynchronous response/error from mmwave radar device
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-783 */
rlReturnVal_t rlDriverAsyncEventHandler(rlUInt8_t devIndex,
                                       rlUInt16_t nsbc, rlUInt8_t *payload,
                                       rlUInt16_t payloadLen)
{
    rlUInt16_t indx;
    rlUInt16_t sbLen = 0U;
    rlUInt16_t sbcId = 0U;
    rlUInt16_t recSbsLen = 0U;
    rlReturnVal_t retVal = RL_RET_CODE_OK;
    rlUInt8_t *payldAddr = payload;

    RL_LOGV_ARG0("rlDriverAsyncEventHandler starts...\n");

    /* Lood for all the Events Sub Block and call event handler */
    for (indx = 0U; indx < nsbc; indx++)
    {
        /* check for payload pointer for NULL */
        if ((payldAddr == RL_NULL_PTR) || (recSbsLen > payloadLen))
        {
            /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Error value set on top of default value" */
            /*LDRA_INSPECTED 8 D */
            retVal = RL_RET_CODE_FATAL_ERROR;
            break;
        }
        /* Read Sub Block Id */
        rlGetSubBlockId(payldAddr, &sbcId);

        /* Read Sub Block Len */
        rlGetSubBlockLen(payldAddr, &sbLen);

        /* Call Application registered callback to indicate event*/
        if (RL_NULL_PTR != rl_driverData.clientCtx.eventCb.rlAsyncEvent)
        {
            /* Call event handler and pass event payload */
            /* AR_CODE_REVIEW MR:R.18.1,R.18.4 <APPROVED> "pointer arithmetic required" */
            /*LDRA_INSPECTED 567 S */
            /*LDRA_INSPECTED 87 S */
            rl_driverData.clientCtx.eventCb.rlAsyncEvent(devIndex, sbcId, \
                                 ((rlUInt16_t)(sbLen - RL_MIN_SBC_LEN)),\
                                 (payldAddr + RL_SBC_PL_INDEX));
            /* Increase received payload length*/
            recSbsLen += (sbLen);

            /* increment payload address */
            payldAddr += sbLen;
        }
    }
    RL_LOGV_ARG0("rlDriverAsyncEventHandler ends...\n");

    return retVal;
}

/** @fn void rlDriverHostIrqHandler(rlUInt8_t deviceIndex, void *pValue)
*
*   @brief Interrupt Service Routine to handle host interrupt from mmwave
*          radar device
*   @param[in] deviceIndex - source device Index
*   @param[in] pValue - Interrupt Routine Argument
*
*   @return none
*
*   Interrupt Service Routine to handle host interrupt from mmwave radar device
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-777 */
void rlDriverHostIrqHandler(rlUInt8_t deviceIndex, void *pValue)
{
    rlUInt8_t tempVar;
    /* get rlDriver global structure pointer */
    rlDriverData_t* rlDrvData = rlDriverGetHandle();

    RL_LOGV_ARG0("rlDriverHostIrqHandler starts...\n");

    /* check for NULL pointer */
    if (pValue == RL_NULL_PTR)
    {
        /* No Argument Passed */
        RL_LOGD_ARG0("rlDriverHostIrqHandler Input arg is NULL\n");
    }

    /* Mask the Interrupt - Required if interrupt is Level triggered*/
    /* AR_CODE_REVIEW MR:D.4.1,D.4.14 <APPROVED> "rlDrvData is pointer to a global strcture,
        can't be NULL" */
    /*LDRA_INSPECTED 45 D */
    if ((RL_NULL_PTR != rlDrvData->clientCtx.devCtrlCb.rlDeviceMaskHostIrq)\
          && ((rlComIfHdl_t)RL_NULL_PTR != rlDrvData->commDevIdx.comIfHdl[deviceIndex]))
    {
        /* Mask Host IRQ */
        rlDrvData->clientCtx.devCtrlCb.rlDeviceMaskHostIrq((rlComIfHdl_t) \
                              rlDrvData->commDevIdx.comIfHdl[deviceIndex]);
        RL_LOGD_ARG0("rlDriverHostIrqHandler Mask the Interrupt\n");
    }

    /* Increment the Irq count*/
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "storing incremented value to global structure" */
    /*LDRA_INSPECTED 105 D */
    rlDrvData->rxIrqCnt[deviceIndex] = (rlDrvData->rxIrqCnt[deviceIndex] + 1U);

    /* store deviceIndex in Global */
    rlDrvData->commDevIdx.rlDevIndex[deviceIndex] = deviceIndex;

    tempVar = (rlDrvData->isCmdRespWaited[deviceIndex] | \
               rlDrvData->isRespWriteWaited[deviceIndex]);
    /* Check if command transaction is in progress*/
    if (RL_TRUE == tempVar)
    {
        /* Release command response wait semaphore to unblock command thread*/
        (void)rlDrvData->clientCtx.osiCb.sem.rlOsiSemSignal(
                                        &rlDrvData->cmdSem);
        RL_LOGD_ARG0("rlDriverHostIrqHandler Release command response \n");
    }
    else
    {
        /* No response is expected, Add to the Spawn queue to be hanled
         * in different context*/
        (void)rlDrvData->clientCtx.osiCb.queue.rlOsiSpawn(
            (RL_P_OSI_SPAWN_ENTRY)rlDriverMsgReadSpawnCtx,
            &rlDrvData->commDevIdx.rlDevIndex[deviceIndex], 0);
        RL_LOGD_ARG0("rlDriverHostIrqHandler No response is expected \n");
    }

    RL_LOGV_ARG0("rlDriverHostIrqHandler ends...\n");

    return;
}

/** @fn rlReturnVal_t rlDriverProcRdMsg(rlUInt8_t devIdx, rlReturnVal_t inVal)
*
*   @brief Process received message for Async event message
*   @param[in] devIdx - device Index
*   @param[in] inVal  - status value
*
*   @return error status
*
*   Process received message for Async event message
*/
rlReturnVal_t rlDriverProcRdMsg(rlUInt8_t devIdx, rlReturnVal_t inVal)
{
    /* get rlDriver global structure pointer */
    rlDriverData_t* rlDrvData = rlDriverGetHandle();
    rlReturnVal_t retVal;

    /* Increment IRQ done IR*/
    /* AR_CODE_REVIEW MR:D.4.1,D.4.14 <APPROVED> "rlDrvData is pointer to a global strcture,
        can't be NULL" */
    /* AR_CODE_REVIEW MR:R.2.2  <APPROVED> "storing incremented value to global structure" */
    /*LDRA_INSPECTED 105 D */
    /*LDRA_INSPECTED 45 D */
    rlDrvData->rxDoneCnt[devIdx] = (rlDrvData->rxDoneCnt[devIdx] + 1U);

    /* Check received message class*/
    switch (rlDrvData->funcParams.rxMsgClass)
    {
        case RL_API_CLASS_ASYNC:
            /* If CRC check passed then only parse Async Event */
            if (inVal == RL_RET_CODE_OK)
            {
                /* parse received Async Event Message */
                /*LDRA_INSPECTED 95 S */
                retVal = rlDriverAsyncEventHandler(devIdx,\
                            rlDrvData->funcParams.asyncEvt.evtMsg.hdr.nsbc,\
                            (rlUInt8_t *)&rlDrvData->funcParams.asyncEvt.evtMsg.payload[0],\
                            (rlUInt16_t)(rlDrvData->funcParams.asyncEvt.evtMsg.hdr.len - \
                            RHCP_HEADER_LEN));
            }
            else
            {
                retVal = RL_RET_CODE_OK;
            }
            break;
        case RL_API_CLASS_NACK:
            /* These types are legal in this context. Do nothing */
            retVal = RL_RET_CODE_OK;
            break;
        case RL_API_CLASS_RSP:
            /* Command response is illegal in this context. */
            retVal = RL_RET_CODE_OK;
            break;
        /* if mmWaveLink is running on MSS/DSS and receives command from other Core/HOST */
        case RL_API_CLASS_CMD:
        case RL_API_CLASS_BYPASS:
            {
                /* Check if command parser API callback is assigned by the application */
                if (rl_driverData.clientCtx.cmdParserCb.rlCmdParser != RL_NULL_PTR)
                {
                    /* invoke callback to parse the received command packet */
                    (void)rl_driverData.clientCtx.cmdParserCb.rlCmdParser(
                                        rlDrvData->funcParams.rxMsgClass, inVal);
                }
                else
                {
                    /* do nothing */
                }
                retVal = RL_RET_CODE_OK;
            }
            break;
        case RL_API_CLASS_MAX:
            /* do nothing */
            retVal = RL_RET_CODE_OK;
            break;
        default:
            /* set error code */
            retVal = RL_RET_CODE_PROTOCOL_ERROR;
            RL_LOGE_ARG0("rlDriverProcRdMsg, Rl protocol error\n");
            break;
    }

    return retVal;
}

/** @fn rlReturnVal_t rlDriverMsgReadSpawnCtx(void *pValue)
*
*   @brief Handles Interrupt in application(user) context
*   @param[in] pValue - Interrupt Routine Argument
*
*   @return none
*
*   Handles Interrupt in application(user) context
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-782 */
rlReturnVal_t rlDriverMsgReadSpawnCtx(const void *pValue)
{
    rlReturnVal_t retVal, msgRdRetVal;
    rlUInt8_t deviceIndex;
    rlUInt8_t lclRxIrqCnt, lclRxDoneCnt;
    /* get rlDriver global structure pointer */
    rlDriverData_t* rlDrvData = rlDriverGetHandle();

    RL_LOGV_ARG0("rlDriverMsgReadSpawnCtx starts...\n");

    /* check for NULL pointer */
    if (pValue == RL_NULL_PTR)
    {
        /* No Argument Passed */
        deviceIndex = 0U;
        RL_LOGE_ARG0("rlDriverMsgReadSpawnCtx Input arg is NULL\n");
    }
    else
    {
        /* argument passed is Device Index */
        deviceIndex = *(const rlUInt8_t*)pValue;
    }

    /* This function pointer is always being checked on powerOn and mmwavelink
       fails poweron with error return value, in this failure case no API call is allowed */

    /* AR_CODE_REVIEW MR:D.4.1,D.4.14 <APPROVED> "rlDrvData is pointer to a global strcture,
        can't be NULL" */
    /*LDRA_INSPECTED 45 D */
    (void)rlDrvData->clientCtx.osiCb.mutex.rlOsiMutexLock
              (&rlDrvData->globalMutex, (rlOsiTime_t)RL_OSI_WAIT_FOREVER);

    /* Messages might have been read by CmdResp context. Therefore after getting LockObj,
       check again where the Pending Rx Msg is still present. */
    lclRxIrqCnt  = rlDrvData->rxIrqCnt[deviceIndex];
    lclRxDoneCnt = rlDrvData->rxDoneCnt[deviceIndex];

    /* unlock Mutex if all Received IRQ are handled i.e. DONE */
    if (lclRxIrqCnt == lclRxDoneCnt)
    {
        (void)rlDrvData->clientCtx.osiCb.mutex.rlOsiMutexUnLock(
                &rlDrvData->globalMutex);
        RL_LOGD_ARG0("rlDriverMsgReadSpawnCtx, RxIrqCnt is equal to RxDoneCnt\n");
        retVal = RL_RET_CODE_OK;
    }
    else
    {
        /* Receive data over communicaton channel*/
        /* AR_CODE_REVIEW MR:D.4.7,R.17.7 <APPROVED> "variable is used in next function call" */
        /*LDRA_INSPECTED 91 D */
        msgRdRetVal = rlDriverMsgRead(rlDrvData, deviceIndex);
        /* process received message */
        retVal = rlDriverProcRdMsg(deviceIndex, msgRdRetVal);

        /* Unlock Global Mutex */
        (void)rlDrvData->clientCtx.osiCb.mutex.rlOsiMutexUnLock(&rlDrvData->globalMutex);
    }

    RL_LOGV_ARG0("rlDriverMsgReadSpawnCtx ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverMsgCmdReply(rlDriverData_t* rlDrvData,
*                                           rlUInt8_t devIndex)
*
*   @brief Wait and handle command response
*   @param[in] rlDrvData - Pointer to mmWaveLink global structure
*   @param[in] devIndex - Device index of slave device from where response
*                         is expected
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Wait and handle command response
*/
rlReturnVal_t rlDriverMsgCmdReply(rlDriverData_t* rlDrvData, rlUInt8_t devIndex)
{
    rlReturnVal_t retVal, retStatus;
    rlUInt8_t tempRxDoneCnt, tempVar;

    RL_LOGV_ARG0("rlDriverMsgCmdReply starts...\n");

    /* Receive data over communication channel */
    retStatus = rlDriverMsgRead(rlDrvData, devIndex);

    if (RL_RET_CODE_OK != (rlInt32_t)retStatus)
    {
        /* update rxDoneCnt */
        tempRxDoneCnt = rlDrvData->rxDoneCnt[devIndex];
        rlDrvData->rxDoneCnt[devIndex] = (tempRxDoneCnt + 1U);
        /* Error in received data, return error */
        retVal = retStatus;
        RL_LOGE_ARG0("Error in received data\n");
    }
    else
    {
        /* Data received successfully */
        rlDrvData->rxDoneCnt[devIndex]++;

        /* Check received message class */
        if (RL_API_CLASS_RSP == rlDrvData->funcParams.rxMsgClass)
        {
            /* Command response received, clear the Wait flag to exit loop */
            tempVar = RL_FALSE;
            rlDrvData->isCmdRespWaited[devIndex] = tempVar;

            /* check if received msg-ID doesn't match with CMD msg-ID */
            if (rl_rxMsg.hdr.opcode.b10MsgId != rl_txMsg.hdr.opcode.b10MsgId)
            {
                /* set error code if MsgId of response doesn't match with CMD msg ID */
                retVal = RL_RET_CODE_MSGID_MISMATCHED;
                RL_LOGE_ARG0("Msg id is mis-matched\n");
            }
            else
            {
                /* response MSG ID matched with response MSG ID,set return value to OK */
                retVal = RL_RET_CODE_OK;
            }

            /* In case CmdResp has been read without  waiting on cmdSem that */
            /* Sem object. That to prevent old signal to be processed. */
            /* Clear the Semaphore */
            rlDrvData->clientCtx.osiCb.sem.rlOsiSemWait
                       (&(rlDrvData->cmdSem), (rlOsiTime_t)RL_OSI_NO_WAIT);
        }
        else if (RL_API_CLASS_ASYNC == rlDrvData->funcParams.rxMsgClass)
        {
            /* Async event received when command response is awaited,
             Handle the event and keep waiting for the response*/
            /* AR_CODE_REVIEW MR:R.10.3  <APPROVED> "All parameter types are matching to function
                                                       argument type. LDRA tool issue." */
            /*LDRA_INSPECTED 458 S */
            /*LDRA_INSPECTED 95 S */
            (void)rlDriverAsyncEventHandler(devIndex,
                                           rlDrvData->funcParams.asyncEvt.evtMsg.hdr.nsbc,
                                   (rlUInt8_t *)&rlDrvData->funcParams.asyncEvt.evtMsg.payload[0],
                                            rlDrvData->funcParams.asyncEvt.evtMsg.hdr.len);

            retVal = RL_RET_CODE_OK;

            /* If CRC check fails, the Async Event is Ignored */
        }
        else if (RL_API_CLASS_NACK == rlDrvData->funcParams.rxMsgClass)
        {
            /* If NACK received for the CMD sent, set CMD Response Wait TAG to FALSE */
            tempVar = RL_FALSE;
            rlDrvData->isCmdRespWaited[devIndex] = tempVar;
            /* set error code to CRC Failed */
            retVal = RL_RET_CODE_CRC_FAILED;
            RL_LOGE_ARG0("CRC check fails, the Async Event is Ignored\n");
        }
        else
        {
            /* Invalid Class*/
            retVal = RL_RET_CODE_INVALID_STATE_ERROR;
            RL_LOGE_ARG0("Invalid Class\n");
        }
    }
    RL_LOGV_ARG0("rlDriverMsgCmdReply ends...\n");

    return retVal;
}


/** @fn rlReturnVal_t rlDriverMsgReadCmdCtx(rlUInt8_t devIndex)
*
*   @brief Wait and handle command response
*   @param[in] devIndex - Device index of slave device from where response
*                         is expected
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Wait and handle command response
*/
/* DesignId : MMWL_DesignId_102 */
/* Requirements : AUTORADAR_REQ-782 */
rlReturnVal_t rlDriverMsgReadCmdCtx(rlUInt8_t devIndex)
{
    rlReturnVal_t retVal;
    rlDriverData_t* rlDrvData = rlDriverGetHandle();

    RL_LOGV_ARG0("rlDriverMsgReadCmdCtx starts...\n");
    /* check for NULL pointer */
    if (RL_NULL_PTR == rlDrvData)
    {
        retVal = RL_RET_CODE_FATAL_ERROR;
    }
    else
    {
        /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Return value is being set under different
             conditions, where it's set with default value first" */
        /*LDRA_INSPECTED 8 D */

        retVal = RL_RET_CODE_OK;
        /*    after command response is received and isCmdRespWaited flag is set FALSE, it is
        necessary to read out all Async messages in Commands context, because slave device
        could have dispatched some Async messages before receiving the command */

        /* AR_CODE_REVIEW MR:D.2.1 <APPROVED> "This is function must loop till
         * command response is received successfully or retry timer expires ,
         * if any Hw hang then WDT reset recovers from this error" */
        /*LDRA_INSPECTED 28 D */
        while ((RL_TRUE == rlDrvData->isCmdRespWaited[devIndex]) && (RL_RET_CODE_OK == retVal))
        {
            if (RL_PENDING_RX_MSG(rlDrvData, devIndex))
            {
                /* init to illegal value and verify it's overwritten with the
                 * valid one */
                rlDrvData->funcParams.rxMsgClass = RL_API_CLASS_MAX;

                /* Receive data over communication channel */
                retVal += rlDriverMsgCmdReply(rlDrvData, devIndex);
            }
            /* AR_CODE_REVIEW MR:D.2.1 <APPROVED> "This is function must loop till
             * command response is received successfully or retry timer expires,
             * if any Hw hang then WDT reset recovers from this error" */
            /*LDRA_INSPECTED 28 D */
            else
            {
                /* cmdSem will be signaled by IRQ */
                if (RL_RET_CODE_OK != rlDrvData->clientCtx.osiCb.sem.rlOsiSemWait(
                                                   &(rlDrvData->cmdSem),
                                                   (rlOsiTime_t)rlDrvData->clientCtx.ackTimeout))
                {
                    /* setCmd Response Wait Tag to False when timer expires */
                    rlDrvData->isCmdRespWaited[devIndex] = RL_FALSE;

                    RL_LOGE_ARG0("CmdSem should be signaled by IRQ but respTimeout\n");
                    retVal += RL_RET_CODE_RESP_TIMEOUT;
                    break;
                }
            }
        }

        /* if any Rx Msg is pending to process and ACK Timout error hasn't happend */
        if (RL_PENDING_RX_MSG(rlDrvData, devIndex) && (retVal != RL_RET_CODE_FATAL_ERROR) \
            && (rlDrvData->isCmdRespWaited[devIndex] == RL_FALSE))
        {
            /* Spawn a thread/task to read pending Rx Msg */
            if (rlDrvData->clientCtx.osiCb.queue.rlOsiSpawn
                ((RL_P_OSI_SPAWN_ENTRY)rlDriverMsgReadSpawnCtx, \
                &rlDrvData->commDevIdx.rlDevIndex[devIndex], 0U) != RL_RET_CODE_OK)
            {
                retVal = RL_RET_CODE_RADAR_OSIF_ERROR;
            }
        }
    }
    RL_LOGV_ARG0("rlDriverMsgReadCmdCtx ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverOriginDirCheck(rlUInt8_t deviceRunId,
*                            rlUInt16_t dataDir)
*
*   @brief Checks for Direction of data received if that matches with
*          where mmWaveLink is running
*   @param[in] deviceRunId - ID where mmWaveLink is running
*   @param[in] dataDir - direction in rcvd data packet
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Checks for Direction of data received if that matches with where
*   mmWaveLink is running
*/
/* DesignId :  */
/* Requirements :  */
static rlReturnVal_t  rlDriverOriginDirCheck(rlUInt8_t deviceRunId,
                               rlUInt16_t dataDir)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDriverOriginDirCheck starts...\n");

    switch (deviceRunId)
    {
        /* if mmWaveLink instance is running on Host */
        case RL_PLATFORM_HOST:
            if ((RL_API_DIR_BSS_TO_HOST == dataDir) ||
               (RL_API_DIR_MSS_TO_HOST == dataDir) ||
               (RL_API_DIR_DSS_TO_HOST == dataDir))
            {
                /* set OK to return value, i.e. requested DataDir is correct
                  as per running instance of mmWaveLink */
                retVal = RL_RET_CODE_OK;
            }
            else
            {
                /* request DataDir is invalid as per running instance of mmWaveLink */
                retVal = RL_RET_CODE_INVALID_INPUT;
            }
            break;
        /* if mmWaveLink instance is running on MSS */
        case RL_PLATFORM_MSS:
            if ((RL_API_DIR_BSS_TO_MSS == dataDir) ||
               (RL_API_DIR_DSS_TO_MSS == dataDir) ||
               (RL_API_DIR_HOST_TO_MSS == dataDir))
            {
                /* set OK to return value, i.e. requested DataDir is correct
                  as per running instance of mmWaveLink */
                retVal = RL_RET_CODE_OK;
            }
            else
            {
                /* request DataDir is invalid as per running instance of mmWaveLink */
                retVal = RL_RET_CODE_INVALID_INPUT;
            }
            break;
        /* if mmWaveLink instance is running on DSS */
        case RL_PLATFORM_DSS:
            /* if Data direction is towards DSS */
            if ((RL_API_DIR_BSS_TO_DSS == dataDir) ||
               (RL_API_DIR_MSS_TO_DSS == dataDir) ||
               (RL_API_DIR_HOST_TO_DSS == dataDir))
            {
                /* set OK to return value, i.e. requested DataDir is correct
                  as per running instance of mmWaveLink */
                retVal = RL_RET_CODE_OK;
            }
            else
            {
                /* request DataDir is invalid as per running instance of mmWaveLink */
                retVal = RL_RET_CODE_INVALID_INPUT;
            }
            break;
        default:
            /* Invalid: set error code */
            retVal = RL_RET_CODE_INVALID_INPUT;
            RL_LOGE_ARG2("Unknown platform %d, retVal = %d\n", \
                dataDir, retVal);
            break;
    }
    RL_LOGV_ARG0("rlDriverOriginDirCheck ends...\n");

    return retVal;
}

rlReturnVal_t rlDriverRdVerifyMsg(rlReadBuf_t readBuf, rlUInt8_t devIndex)
{
    rlUInt16_t   rxLengthRecv;
    rlReturnVal_t retVal, readRetVal;
    rlUInt16_t   payloadLen, msgCrcLen, msgCrcType;
    rlUInt8_t    isCrcPresent;

    /* get Received Message length excluding SYNC */
    rxLengthRecv = RL_RHCP_HDR_PL_LENGTH(&(readBuf.syncHeader.protHdr));

    /* check if received msg length is under valid Msg size */
    if (rxLengthRecv >= (RL_MAX_SIZE_MSG - SYNC_PATTERN_LEN))
    {
        retVal = RL_RET_CODE_PROTOCOL_ERROR;
    }
    else
    {
        rlRhcpMsg_t   *rhcpMsg;

        /* Get Rx Message Class from received buffer header */
        /* Process Header Here - Verify each field */
        rl_driverData.funcParams.rxMsgClass =
            RL_RHCP_HDR_OPCODE_CLASS(&(readBuf.syncHeader.protHdr));

        /* get the Payload Value removing Header length from Rx Msg Length */
        payloadLen = rxLengthRecv - (rlUInt16_t)RHCP_HEADER_LEN;

        if (RL_API_CLASS_ASYNC == rl_driverData.funcParams.rxMsgClass)
        {
            /* Get RHCP message strcture pointer */
            /* AR_CODE_REVIEW MR:D.4.1,D.4.14 <APPROVED> "rlDrvData is pointer to a global
                strcture, can't be NULL" */
            /*LDRA_INSPECTED 45 D */
            rhcpMsg = &rl_driverData.funcParams.asyncEvt.evtMsg;
        }
        else
        {
            /* Get RHCP message strcture pointer */
            rhcpMsg = &rl_rxMsg;
        }

        /* copy header to global strucutre */
        rhcpMsg->hdr = readBuf.syncHeader.protHdr;
        /* Copy SYNC from Communication Channel*/
        rhcpMsg->syncPattern = readBuf.syncHeader.syncPattern;

        /* Check whether CRC is present*/
        if (rhcpMsg->hdr.flags.b2Crc != 0U)
        {
            /* if CRC is not present in Msg the reset crc variables */
            isCrcPresent = RL_FALSE;
            msgCrcLen = 0U;
            msgCrcType = 0U;
        }
        else
        {
            isCrcPresent = RL_TRUE;
            /* It may be size 2/4/8 based on 16/32/64 bit */
            msgCrcType = rhcpMsg->hdr.flags.b2CrcLen;
            /* set CRC length in bytes based on CRC Type */
            msgCrcLen = (2U << (msgCrcType & 0x3U));
        }
        /* Calculate payload length from header legnth*/
        payloadLen = ((isCrcPresent == RL_TRUE) ? (payloadLen - msgCrcLen) : payloadLen);

        /* This is an Response/AsyncEvent message. Read the rest of it. */
        if (payloadLen > 0U)
        {
            if (RL_IF_READ(rl_driverData.commDevIdx.comIfHdl[devIndex],
                           &rhcpMsg->payload[0U], payloadLen) != (rlInt32_t)payloadLen)
            {
                /* If Read from Communication channel failed then set Error code */
                readRetVal = RL_RET_CODE_RADAR_IF_ERROR;
            }
            else
            {
                readRetVal = RL_RET_CODE_OK;
            }
        }
        else
        {
            readRetVal = RL_RET_CODE_OK;
        }

        /* If CRC is present - Read and verify*/
        if ((isCrcPresent == RL_TRUE) && (readRetVal == RL_RET_CODE_OK))
        {
            /* Read the CRC Bytes */
            if (RL_IF_READ(rl_driverData.commDevIdx.comIfHdl[devIndex],
                             rl_driverData.funcParams.msgCRC,
                             msgCrcLen) != (rlInt32_t)msgCrcLen)
            {
                /* Set the error code if read data fails */
                retVal = RL_RET_CODE_RADAR_IF_ERROR;
            }
            else
            {
                (void)memcpy(&rhcpMsg->payload[payloadLen], &rl_driverData.funcParams.msgCRC[0U], \
                             msgCrcLen);
                /* Check if CRC is enabled from the application and it's type
                   matched with received MSG CRC type */
                if ((rl_driverData.clientCtx.crcType != (rlUInt8_t)RL_CRC_TYPE_NO_CRC) && \
                    (rl_driverData.clientCtx.crcType == (rlUInt8_t)msgCrcType))
                {
                    /* Validate CRC first as Opcode might be corrupt as well */
                    /* AR_CODE_REVIEW MR:R.10.3  <APPROVED> "All parameter types are matching to
                                                       function argument type. LDRA tool issue." */
                    /*LDRA_INSPECTED 458 S */
                    retVal = rlDriverVerifyCRC((rlUInt8_t*)&rhcpMsg->hdr, (rxLengthRecv - \
                                                msgCrcLen), (rlUInt8_t)(msgCrcType), \
                                                rl_driverData.funcParams.msgCRC);
                }
                else
                {
                    retVal = RL_RET_CODE_OK;
                }
            }
        }
        else
        {
            retVal = RL_RET_CODE_OK;
        }
    }

    return retVal;
}

/** @fn rlReturnVal_t rlDriverMsgRead(rlDriverData_t* rlDrvData, rlUInt8_t devIndex)
*
*   @brief Receive and validate protocol header and payload
*   @param[in] rlDrvData - Pointer to mmwavelink global structure
*   @param[in] devIndex - Device index of slave device from where data is
*                         received
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Receive and validate protocol header and payload
*/
/* DesignId : MMWL_DesignId_026 */
/* Requirements : AUTORADAR_REQ-777, AUTORADAR_REQ-779 */
rlReturnVal_t rlDriverMsgRead(rlDriverData_t* rlDrvData, rlUInt8_t devIndex)
{
    rlReturnVal_t retVal, hdrRetVal;
    rlReadBuf_t readBuf  = {0};
    rlUInt16_t payloadLen;

    RL_LOGV_ARG0("rlDriverMsgRead starts...\n");
    /* check for NULL pointer */
    if (RL_NULL_PTR == rlDrvData)
    {
        /* set error code */
        retVal = RL_RET_CODE_FATAL_ERROR;
    }
    else
    {
        rlReturnVal_t hdrType;

        /* Read message Header from given device index */
        hdrType = rlDriverRxHdrRead(readBuf.tempBuf,
                      rlDrvData->commDevIdx.comIfHdl[devIndex]);
        RL_LOGV_ARG1("rlDriverRxHdrRead return val = %d\n", hdrType);

        /* if it's not CNYS pattern then calculate checksum before consuming or
         * forwarding to other core */
        if (SYNC_PATTERN_MATCHED == (rlInt32_t)(hdrType))
        {
            /* Verify Checksum, return value 0/-7 (RL_RET_CODE_CHKSUM_FAILED) */
            hdrRetVal = rlDriverValidateHdr(readBuf.syncHeader.protHdr);

            /* received msg can be command/response/async_event
             * and it is destined to the same device where this mmWaveLink ir running
             */
            if ((RL_RET_CODE_OK == hdrRetVal) && (RL_RET_CODE_OK == \
                rlDriverOriginDirCheck(rlDrvData->clientCtx.platform,
                                     readBuf.syncHeader.protHdr.opcode.b4Direction)))
            {
                retVal = rlDriverRdVerifyMsg(readBuf, devIndex);
            }
            else if (RL_RET_CODE_OK == hdrRetVal)
            {
                rlDrvData->funcParams.rxMsgClass = RL_API_CLASS_BYPASS;
                /* if rcvd data is not meant for the device where mmWaveLink is running,
                 * Then pass data to intended destination; Assumption: byPass of
                 * Data will happening in MSS only
                 */
                rl_rxMsg.syncPattern = readBuf.syncHeader.syncPattern;
                rl_rxMsg.hdr = readBuf.syncHeader.protHdr;

                /* this length includes the CRC field also */
                payloadLen = readBuf.syncHeader.protHdr.len - (rlUInt16_t)RHCP_HEADER_LEN;

                /* read full data before writing to destination COMM handle */
                if (RL_IF_READ(rlDrvData->commDevIdx.comIfHdl[devIndex], \
                         &rl_rxMsg.payload[0], payloadLen) != (rlInt32_t)payloadLen)
                {
                    /* Set error code if read is failed */
                    retVal = RL_RET_CODE_RADAR_IF_ERROR;
                }
                else
                {
                    /* Set as passed if data read returns zero */
                    retVal = RL_RET_CODE_OK;
                }
            }
            else
            {
                /* Set header Validate function return value to retVal */
                retVal = hdrRetVal;
            }

        }
        /* In case mmWaveLink instance is running on MSS, it may receive CNYS from Host Over SPI */
        else if (CNYS_PATTERN_MATCHED == (rlInt32_t)(hdrType))
        {
            /* Check callback is assigned by the application */
            if (rl_driverData.clientCtx.cmdParserCb.rlPostCnysStep != RL_NULL_PTR)
            {
                /* invoke function to complete the task required after receiving CNYS */
                retVal = rl_driverData.clientCtx.cmdParserCb.rlPostCnysStep(devIndex);
            }
            else
            {
                /* Set error code if callback is NULL */
                retVal = RL_RET_CODE_INTERFACE_CB_NULL;
            }
        }
        else
        {
            retVal = hdrType;
            /* Set Message class to MAX (invalid) */
            rlDrvData->funcParams.rxMsgClass = RL_API_CLASS_MAX;
        }

        /*  Unmask Interrupt call */
        if ((RL_NULL_PTR != rlDrvData->clientCtx.devCtrlCb.rlDeviceUnMaskHostIrq) &&\
            (retVal != RL_RET_CODE_FATAL_ERROR))
        {
            /* Un Mask Interrupt */
            rlDrvData->clientCtx.devCtrlCb.rlDeviceUnMaskHostIrq(\
                rlDrvData->commDevIdx.comIfHdl[devIndex]);
           RL_LOGD_ARG0("rlDriverMsgRead Unmask Interrupt call\n");
        }
    }

    RL_LOGV_ARG0("rlDriverMsgRead ends...\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverMsgWrite(rlDriverData_t* rlDrvData, rlComIfHdl_t comIfHdl)
*
*   @brief Write command header and payload data over communication channel
*   @param[in] rlDrvData - Pointer to mmwavelink global structure
*   @param[in] comIfHdl - Communication interface handle
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Write command header and payload data over communication channel
*/
/* DesignId : MMWL_DesignId_025 */
/* Requirements : AUTORADAR_REQ_772, AUTORADAR_REQ-774 */
rlReturnVal_t rlDriverMsgWrite(rlDriverData_t* rlDrvData, rlComIfHdl_t comIfHdl)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDriverMsgWrite starts...\n");

    /* check for NULL pointer */
    if (((rlComIfHdl_t)RL_NULL != comIfHdl) && (rlDrvData != RL_NULL_PTR))
    {
        rlUInt16_t checkSum = 0U, msgCrcLen;
        rlUInt16_t payloadLen;
        rlUInt16_t tempLen;
        /* Calculate Checksum */
        /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Checksum is getting updated in called function" */
        /*LDRA_INSPECTED 8 D */
        (void)rlDriverCalChkSum(&rl_txMsg.hdr, ((rlUInt8_t)(RHCP_HEADER_LEN - 2U)), &checkSum);
        rl_txMsg.hdr.chksum = checkSum;

        /* get Payload length removing Header length from Msg Length */
        payloadLen = rl_txMsg.hdr.len - (rlUInt16_t)RHCP_HEADER_LEN;
        RL_LOGV_ARG1("API Dir = %d \n", rl_txMsg.hdr.opcode.b4Direction);

        /* check for Data Direction to choose Sync Pattern */
        if ((RL_API_DIR_DSS_TO_HOST == rl_txMsg.hdr.opcode.b4Direction) || \
           (RL_API_DIR_MSS_TO_HOST == rl_txMsg.hdr.opcode.b4Direction) || \
           (RL_API_DIR_DSS_TO_MSS  == rl_txMsg.hdr.opcode.b4Direction))
        {
            /* set device to Host Sync Pattern */
            rl_txMsg.syncPattern.sync1 = D2H_SYNC_PATTERN_1;
            rl_txMsg.syncPattern.sync2 = D2H_SYNC_PATTERN_2;
        }
        else
        {
            /* set Host to device Sync Pattern */
            rl_txMsg.syncPattern.sync1 = H2D_SYNC_PATTERN_1;
            rl_txMsg.syncPattern.sync2 = H2D_SYNC_PATTERN_2;
        }

        /* Check if CRC is enabled, Calculate and update payload length*/
        if (rl_txMsg.hdr.flags.b2Crc == RL_HDR_FLAG_CRC)
        {
            /* It may be size 2/4/8 based on 16/32/64 bit */
            msgCrcLen = (2U << (rlDrvData->clientCtx.crcType & 0x3U));
            /* compute CRC */
            (void)rlDriverCalCRC((rlUInt8_t *)&rl_txMsg.hdr, (
                    rl_txMsg.hdr.len - msgCrcLen),
                    (rlUInt8_t)rlDrvData->clientCtx.crcType,
                    (rlUInt8_t *)rlDrvData->funcParams.msgCRC);
            /* copy computed CRC to Tx Msg buffer */
            (void)memcpy((rlUInt8_t*)(&rl_txMsg.payload[payloadLen - msgCrcLen]),
                   (rlUInt8_t*)&(rlDrvData->funcParams.msgCRC[0]), msgCrcLen);
        }
        tempLen = ((rlUInt16_t)(SYNC_PATTERN_LEN + RHCP_HEADER_LEN) + payloadLen);

        /* write Tx Msg to destination either over Mailbox internal to
           mmWave device or to External Host Over SPI */
        if (RL_IF_WRITE(comIfHdl, ((rlUInt8_t *)&rl_txMsg), \
                         tempLen) != (rlInt32_t)tempLen)
        {
            /* set error code */
            retVal = RL_RET_CODE_RADAR_IF_ERROR;
        }
        else
        {
            /* set Error code as OK */
            retVal = RL_RET_CODE_OK;
        }
    }
    else
    {
        /* set error code if pointers are NULL */
        retVal = RL_RET_CODE_FATAL_ERROR;
    }
    RL_LOGV_ARG0("rlDriverMsgWrite ends...\n");

    return retVal;
}

/** @fn rlUInt8_t rlDriverReceiveSync(rlComIfHdl_t comIfHdl, rlUInt8_t syncBuf[],
*                           rlInt32_t *syncType)
*
*   @brief: Parse received msg' header
*
*   @param[in] comIfHdl - Communication interface handle
*   @param[in] syncBuf  - Sync buffer
*   @param[in] syncType - sync type
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*    Parse received msg'header
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-777 */
static rlUInt8_t rlDriverReceiveSync(rlComIfHdl_t comIfHdl, rlUInt8_t syncBuf[],
                                 rlInt32_t *syncType)
{
    rlUInt16_t count = 0;
    rlInt32_t retVal;
    rlSyncPattern_t recSyncPattern = {0U, 0U};
    rlReturnVal_t errVal;

    RL_LOGV_ARG0("rlDriverReceiveSync starts...\n");

    /* check for NULL pointer */
    if (syncType != (rlInt32_t*)RL_NULL)
    {
        /* Read 4 bytes SYNC Pattern) */
        if (RL_IF_READ(comIfHdl, &syncBuf[0U], (rlUInt16_t)SYNC_PATTERN_LEN) != \
           (rlInt32_t)SYNC_PATTERN_LEN)
        {
            /* set error code */
            errVal = RL_RET_CODE_RADAR_IF_ERROR;
        }
        else
        {
            /* if SYNC pattern has been read properly then copy it */
            (void)memcpy(&recSyncPattern, &syncBuf[0U], SYNC_PATTERN_LEN);
            errVal = RL_RET_CODE_OK;
        }
        retVal = PATTERN_NOT_MATCHED;

        /* Wait for SYNC_PATTERN from the device (when mmWaveLink is running on Ext Host*/
        while (((retVal) == (rlInt32_t)PATTERN_NOT_MATCHED) && (errVal == RL_RET_CODE_OK))
        {
            /* check if matched with SYNC pattern Host-to-device or device-to-Host */
            if (((recSyncPattern.sync1 == H2D_SYNC_PATTERN_1) &&
                (recSyncPattern.sync2 == H2D_SYNC_PATTERN_2) ) || \
                ((recSyncPattern.sync1 == D2H_SYNC_PATTERN_1) &&
                (recSyncPattern.sync2 == D2H_SYNC_PATTERN_2)))
            {
                /* set to SYNC Matched flag if H2D or D2H SYNC pattern is matching
                    for big/little endian data */
                retVal = SYNC_PATTERN_MATCHED;
            }
            else
            {
                /* if mmwavelink running on device and connect to Host over SPI then
                   it may recieve CNYS to send data */
                if ((recSyncPattern.sync1 == H2D_CNYS_PATTERN_1) &&
                   (recSyncPattern.sync2 == H2D_CNYS_PATTERN_2))
                {
                    /* set to CNYS Matched flag if H2D CNYS pattern is matching
                        for big/little endian data */
                    retVal = CNYS_PATTERN_MATCHED;
                }
                /* check if count is beyond SYNC Scan threshold */
                else if (count >= (rlUInt16_t)RL_SYNC_SCAN_THRESHOLD)
                {
                    /* Set error code to terminate this loop */
                    errVal += RL_RET_CODE_PROTOCOL_ERROR;
                }
                else
                {
                    /*  Read next 4 bytes to Low 4 bytes of buffer */
                    if (0 == (count % SYNC_PATTERN_LEN))
                    {
                        /* Read 4 bytes SYNC Pattern) */
                        if (RL_IF_READ(comIfHdl, &syncBuf[SYNC_PATTERN_LEN],\
                           (rlUInt16_t)SYNC_PATTERN_LEN) != \
                           (rlInt32_t)SYNC_PATTERN_LEN)
                        {
                            /* Set error code to terminate this loop */
                            errVal += RL_RET_CODE_RADAR_IF_ERROR;
                            break;
                        }
                    }
                    /*  Shift Buffer Up for checking if the sync is shifted */
                    rlDriverShiftDWord(syncBuf);
                    /* copy data to recv sync pattern to compare further */
                    (void)memcpy(&recSyncPattern, &syncBuf[0U], SYNC_PATTERN_LEN);
                    /* increment read counter */
                    count++;
                }
            }
        }

        if (errVal == RL_RET_CODE_OK)
        {
            *(syncType) = retVal;
        }
        else
        {
            *(syncType) = errVal;
        }
    }
    count %= SYNC_PATTERN_LEN;
    RL_LOGV_ARG1("rlDriverReceiveSync, count = %d\n", count);
    RL_LOGV_ARG0("rlDriverReceiveSync ends...\n");

    return (rlUInt8_t)count;
}

/** @fn rlReturnVal_t rlDriverRxHdrRead(rlUInt8_t hdrBuf[RHCP_HEADER_LEN], rlComIfHdl_t comIfHdl)
*
*   @brief Read SYNC and Header from communication channel
*   @param[in] comIfHdl - Communication interface handle
*   @param[out] hdrBuf - Buffer to hold SyncPattern+header information
*
*   @return rlReturnVal_t pattern matched type, Failure - Error Code
*
*   Read SYNC and Header from communication channel
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-777 */
/*AR_CODE_REVIEW MR:R.2.2 <APPROVED> "errVal is re initialized under different
  if else conditions based on what the error is" */
/*LDRA_INSPECTED 8 D */
rlReturnVal_t rlDriverRxHdrRead(rlUInt8_t hdrBuf[RHCP_HEADER_LEN], rlComIfHdl_t comIfHdl)
{
    /* syncBuf: it should be 2/4 byte aligned, as in the application where
     * it uses DMA to Rd/Wr DMA might have limitation of src/dest address
     * alignement
     */
    rlUInt8_t syncBuf[SYNC_PATTERN_LEN * 2] = {0U};
    /* This buffer contains CNYS pattern (4Bytes) and 12Bytes of dummy sequence.
       Host writes this buffer in response to Host-IRQ raised by AWR device to indicate
       that device can now write response/async event data to its SPI buffer which will
       be read by Host. 
     */
    rlUInt16_t cnysBuf[8U] = {H2D_CNYS_PATTERN_1, H2D_CNYS_PATTERN_2,
                               0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU};
    rlInt32_t syncType     = PATTERN_NOT_MATCHED;
    rlUInt8_t syncCnt;
    rlInt32_t errVal;

    RL_LOGV_ARG0("rlDriverRxHdrRead starts...\n");

    /* check for NULL pointer */
    if ((rlComIfHdl_t)RL_NULL == comIfHdl)
    {
        errVal = RL_RET_CODE_FATAL_ERROR;
        syncType += errVal;
    }
    else
    {
        /* If mmWaveLink is running on Ext Host */
        if (rl_driverData.clientCtx.platform == RL_PLATFORM_HOST)
        {
            /* Write CNYS pattern to mmWave Radar  */
            if (RL_IF_WRITE(comIfHdl,
                (rlUInt8_t*)&cnysBuf[0U], (rlUInt16_t)(SYNC_PATTERN_LEN+ RHCP_HEADER_LEN)) !=\
                (rlInt32_t)(SYNC_PATTERN_LEN + RHCP_HEADER_LEN))
            {
                /* Set error code if data write function fails to write SYNC pattern */
                errVal = RL_RET_CODE_RADAR_IF_ERROR;
            }
            else
            {
                errVal = RL_RET_CODE_OK;
            }

            RL_LOGV_ARG2("Platform = %d, delay = %d \n",\
                         rl_driverData.clientCtx.platform, \
                         rl_driverData.clientCtx.timerCb.rlDelay);
             /* Need to wait till host Irq is down */
             /* Check if Host Irq Status can be polled, else use fixed delay */
             if ((rl_driverData.clientCtx.devCtrlCb.rlDeviceWaitIrqStatus == NULL)||
                (rl_driverData.clientCtx.devCtrlCb.rlDeviceWaitIrqStatus(comIfHdl,
                0) != RL_RET_CODE_OK))
             {
                /* Check if Delay callback is present and invoke */
                if (rl_driverData.clientCtx.timerCb.rlDelay != RL_NULL_PTR)
                {
                    /* add 1 mSec delay */
                    (void)rl_driverData.clientCtx.timerCb.rlDelay(1U);
                }
             }
        }
        else
        {
            errVal = RL_RET_CODE_OK;
        }

        if (errVal == RL_RET_CODE_OK)
        {
            /* AR_CODE_REVIEW MR:D.4.7  <APPROVED> "syncCnt is used when pattern is matched to
                                                      copy and read data" */
            /*LDRA_INSPECTED 91 D */
            syncCnt = rlDriverReceiveSync(comIfHdl, &syncBuf[0U], &syncType);
            RL_LOGV_ARG1("syncType = %d \n", syncType);

            if ((CNYS_PATTERN_MATCHED == syncType) || (SYNC_PATTERN_MATCHED == syncType))
            {
                rlUInt16_t tempLen;
                /* copying shifted data to hdrBuf */
                (void)memcpy(&hdrBuf[0], &syncBuf[0U], (SYNC_PATTERN_LEN + syncCnt));

                tempLen = RHCP_HEADER_LEN - (0x00FFU & syncCnt);
                /*  Here we've read Sync Pattern. Read the remaining header */
                if (RL_IF_READ(comIfHdl, (&hdrBuf[(SYNC_PATTERN_LEN + syncCnt)]), \
                                 tempLen) != (rlInt32_t)tempLen)
                {
                    syncType += RL_RET_CODE_RADAR_IF_ERROR;
                }
            }
            else
            {
                /* do nothing */
                RL_LOGD_ARG0("do nothing ");
            }
        }
        else
        {
            syncType += errVal;
        }

    }

    RL_LOGV_ARG0("rlDriverRxHdrRead ends...\n");

    return syncType;
}

/** @fn rlReturnVal_t rlDriverOsiInit(void)
*
*   @brief Initializes the OSI layer abstraction for mmwavelink
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Initializes the OS mutex, semaphore and queue interface for mmwavelink
*/
/* DesignId : MMWL_DesignId_002 */
/* Requirements : AUTORADAR_REQ-784 */
rlReturnVal_t rlDriverOsiInit(void)
{
    rlReturnVal_t retVal;
    rlInt32_t funcRetVal;

    /* Create Global Mutex Object */
    funcRetVal = rl_driverData.clientCtx.osiCb.mutex.rlOsiMutexCreate(
                             &rl_driverData.globalMutex, "GlobalLockObj");

    /* Create Command Semaphore */
    funcRetVal += rl_driverData.clientCtx.osiCb.sem.rlOsiSemCreate(
                                         &rl_driverData.cmdSem, "CmdSem");

    /* check for above function call return value */
    if (funcRetVal != RL_RET_CODE_OK)
    {
        /* set error code */
        retVal = RL_RET_CODE_RADAR_OSIF_ERROR;
    }
    else
    {
        retVal = RL_RET_CODE_OK;
    }

    return retVal;
}

static rlReturnVal_t rlDriverOsiCbCheck(rlClientCbs_t clientCb)
{
    rlReturnVal_t retVal;

    /* Check if application has passed mutex interace functions */
    if ((RL_NULL_PTR == clientCb.osiCb.mutex.rlOsiMutexCreate) ||
        (RL_NULL_PTR == clientCb.osiCb.mutex.rlOsiMutexLock  ) ||
        (RL_NULL_PTR == clientCb.osiCb.mutex.rlOsiMutexUnLock) ||
        (RL_NULL_PTR == clientCb.osiCb.mutex.rlOsiMutexDelete))
    {
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    /* Check if application has passed semaphore interface functions */
    else if ((RL_NULL_PTR == clientCb.osiCb.sem.rlOsiSemCreate) ||
             (RL_NULL_PTR == clientCb.osiCb.sem.rlOsiSemWait  ) ||
             (RL_NULL_PTR == clientCb.osiCb.sem.rlOsiSemSignal) ||
             (RL_NULL_PTR == clientCb.osiCb.sem.rlOsiSemDelete))
    {
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    /* Check if application has passed communication interface functions */
    else if ((RL_NULL_PTR == (void*)clientCb.comIfCb.rlComIfOpen ) ||
             (RL_NULL_PTR == clientCb.comIfCb.rlComIfClose) ||
             (RL_NULL_PTR == clientCb.comIfCb.rlComIfRead ) ||
             (RL_NULL_PTR == clientCb.comIfCb.rlComIfWrite))
    {
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    else
    {
        /* if no error then set return value as OK */
        retVal = RL_RET_CODE_OK;
    }

    return retVal;
}

/** @fn rlReturnVal_t rlDriverClientCbCheck(rlClientCbs_t clientCb)
*
*   @brief Check for interace callbacks passed by application during rlDevicePowerOn.
*   @param[in] clientCb - Client callbacks
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Check for interace functions passed by application during rlDevicePowerOn.
*/
static rlReturnVal_t rlDriverClientCbCheck(rlClientCbs_t clientCb)
{
    rlReturnVal_t retVal;

    if (RL_RET_CODE_OK != rlDriverOsiCbCheck(clientCb))
    {
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    /* Check if application has passed device interace functions */
    else if ((RL_NULL_PTR == clientCb.eventCb.rlAsyncEvent           ) ||
             (RL_NULL_PTR == clientCb.devCtrlCb.rlDeviceDisable      ) ||
             (RL_NULL_PTR == clientCb.devCtrlCb.rlDeviceEnable       ) ||
             (RL_NULL_PTR == clientCb.devCtrlCb.rlDeviceMaskHostIrq  ))
    {
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    /* When mmWaveLink instance is running on HOST, check for interface for IRQ and delay */
    else if (((clientCb.platform == RL_PLATFORM_HOST) &&
             (((RL_NULL_PTR == clientCb.devCtrlCb.rlDeviceWaitIrqStatus) &&
              (RL_NULL_PTR == clientCb.timerCb.rlDelay))))  ||
              (RL_NULL_PTR == clientCb.osiCb.queue.rlOsiSpawn))
    {
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    /* If CRC is enabled from application then check for CRC compute interface function */
    else if ((RL_NULL_PTR == clientCb.devCtrlCb.rlDeviceUnMaskHostIrq) ||
             (RL_NULL_PTR == clientCb.devCtrlCb.rlRegisterInterruptHandler) ||
             ((RL_CRC_TYPE_NO_CRC != clientCb.crcType) &&
             (RL_NULL_PTR == clientCb.crcCb.rlComputeCRC)))
    {
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }
    else
    {
        retVal = RL_RET_CODE_OK;
    }

    return retVal;
}

/** @fn rlReturnVal_t rlDriverInit(rlClientCbs_t clientCb, rlUInt8_t deviceMap)
*
*   @brief Initializes the mmwave radar Driver
*   @param[in] clientCb - Client callbacks
*   @param[in] deviceMap - Bitmap of devices to be connected
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Initializes the mmwave radar Driver. Initialize memory, create OS objects,
*   Open communication channel, Register Interrupt Handler
*/
/* DesignId : MMWL_DesignId_003 */
/* Requirements : AUTORADAR_REQ-707 */
rlReturnVal_t rlDriverInit(rlUInt8_t deviceMap, rlClientCbs_t clientCb)
{
    rlReturnVal_t retVal, cbCheck;
    rlUInt8_t index = 0U;

    /* check for all interface APIs passed by the Application */
    cbCheck = rlDriverClientCbCheck(clientCb);

    if (RL_RET_CODE_OK != cbCheck)
    {
        retVal = cbCheck;
    }
    /* Initialize Driver Global Data */
    else if (deviceMap <= ((1U << RL_DEVICE_CONNECTED_MAX) - 0x1U))
    {
        /* Initialize Driver Global Data */
        for (; index < RL_DEVICE_CONNECTED_MAX; index++)
        {
            rl_driverData.isCmdRespWaited[index] = RL_FALSE;
            rl_driverData.isRespWriteWaited[index] = RL_FALSE;
            rl_driverData.rxDoneCnt[index] = 0U;
            rl_driverData.rxIrqCnt[index] = 0U;
        }

        /*Copy Client Context Info*/
        rl_driverData.clientCtx = clientCb;

        rl_driverData.deviceMap = deviceMap;
        /* intialize and stitch all OS interfaces */
        retVal = rlDriverOsiInit();

        rl_driverData.txMsgPtr = &rl_txMsg;
        rl_driverData.rxMsgPtr = &rl_rxMsg;
#if !(RL_DISABLE_LOGGING)
        if (RL_RET_CODE_OK == retVal)
        {
            /*All callback copied in global strcuture, Init log parameter */
            retVal += rlLogInit();
            RL_LOGV_ARG0("Logging is enabled \n");
        }
#endif
        index = 0U;

        do
        {
            /* If deviceIndex is set in devceMap requested by application */
            if (((deviceMap & (1U << index)) != 0U) && (retVal == RL_RET_CODE_OK))
            {
                /* reset to zero sequence number for that deviceIndex */
                rl_driverData.cmdSeqNum[index] = 0U;
                /* store the deviceIndex in a global structure */
                rl_driverData.commDevIdx.rlDevIndex[index] = index;
                /* Open communication interface handle */
                rl_driverData.commDevIdx.comIfHdl[index]   =
                    rl_driverData.clientCtx.comIfCb.rlComIfOpen(index, 0U);

                /* check for NULL pointer */
                if ((rlComIfHdl_t)RL_NULL_PTR == rl_driverData.commDevIdx.comIfHdl[index])
                {
                    retVal += RL_RET_CODE_RADAR_IF_ERROR;
                }
                else
                {
                    /* Register Host Interrupt Handler */
                    if (rl_driverData.clientCtx.devCtrlCb.rlRegisterInterruptHandler(\
                                 index, (RL_P_EVENT_HANDLER)rlDriverHostIrqHandler, \
                                 (void*)RL_NULL) != RL_RET_CODE_OK)
                    {
                        retVal +=RL_RET_CODE_RADAR_IF_ERROR;
                    }
                }
                /* Get next Device Map based on index */
                deviceMap &= ~(1U << index);

                /* check for any error */
                if (retVal != RL_RET_CODE_OK)
                {
                    /* If error occured then break from this loop */
                    break;
                }
            }
            /* increment Device Index */
            index++;
       }
       while ((deviceMap != 0U) && (index < RL_DEVICE_CONNECTED_MAX));
    }
    else
    {
        retVal = RL_RET_CODE_INVALID_INPUT;
        RL_LOGE_ARG0("Invaid input argument\n");
    }

    /* If no error during deviceInit then set the flag to 1 */
    if (retVal == RL_RET_CODE_OK)
    {
        rl_driverData.isDriverInitialized = 1U;
        RL_LOGD_ARG0("Driver init flag is set\n");
    }
    else
    {
        RL_LOGE_ARG0("rlDriverInit, Init failed\n");
    }
    return retVal;
}

/** @fn rlReturnVal_t rlDriverAddDevice(rlUInt8_t deviceMap)
*
*   @brief Adds mmwave radar device
*   @param[in] deviceMap - Bitmap of device to be connected
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Adds mmwave radar device
*   Open communication channel, Register Interrupt Handler with device
*/
/* DesignId :  */
/* Requirements :  */
rlReturnVal_t rlDriverAddDevice(rlUInt8_t deviceMap)
{
    rlReturnVal_t retVal;
    rlUInt8_t index = 0U;

    RL_LOGV_ARG0("rlDeviceAddDevices starts\n");

    if ((rl_driverData.isDriverInitialized != (rlUInt8_t)0U) && \
        (deviceMap <= ((1U << RL_DEVICE_CONNECTED_MAX) - 0x1U)))
    {
        /* Add to the global device map */
        rl_driverData.deviceMap |= deviceMap;

        do
        {
            /* If deviceIndex is set in devceMap requested by application */
            if ((deviceMap & (1U << index)) != 0U)
            {
                /* reset to zero sequence number for that deviceIndex */
                rl_driverData.cmdSeqNum[index] = 0U;
                /* store the deviceIndex in a global structure */
                rl_driverData.commDevIdx.rlDevIndex[index] = index;
                /* Open communication interface handle */
                rl_driverData.commDevIdx.comIfHdl[index] = \
                                    rl_driverData.clientCtx.comIfCb.rlComIfOpen(index, 0U);

                if ((rlComIfHdl_t)RL_NULL_PTR != rl_driverData.commDevIdx.comIfHdl[index])
                {
                    /* register Interrupt handler */
                    if ((rl_driverData.clientCtx.devCtrlCb.rlRegisterInterruptHandler(\
                                    index, (RL_P_EVENT_HANDLER)rlDriverHostIrqHandler, \
                                    (void*)RL_NULL) != RL_RET_CODE_OK) ||
                                    (rl_driverData.clientCtx.devCtrlCb.rlDeviceEnable(index) < 0))
                    {
                        /* set the error code */
                        retVal = RL_RET_CODE_RADAR_IF_ERROR;
                    }
                    else
                    {
                        retVal = RL_RET_CODE_OK;
                    }
                }
                else
                {
                    retVal =RL_RET_CODE_RADAR_IF_ERROR;
                }
                deviceMap &= ~(1U << index);
            }
            else
            {
                retVal = RL_RET_CODE_OK;
            }

            /* break the loop if any error occured during above callbacks */
            if (RL_RET_CODE_OK != retVal)
            {
                break;
            }
            /* increment device index */
            index++;
       }
       while ((deviceMap != 0U) && (index < RL_DEVICE_CONNECTED_MAX));
    }
    else
    {
        if (deviceMap > ((1U << RL_DEVICE_CONNECTED_MAX) - 0x1U))
        {
            retVal = RL_RET_CODE_INVALID_INPUT;
            RL_LOGE_ARG0("Invaid input argument\n");
        }
        else
        {
            retVal =  RL_RET_CODE_INVALID_STATE_ERROR;
            RL_LOGE_ARG0("rlDriverAddDevice, Invalid state \n");
        }
    }
    RL_LOGV_ARG0("rlDeviceAddDevices completes\n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverRemoveDevices(rlUInt8_t deviceMap)
*
*   @brief Disconnects the mmwave radar devices
*   @param[in] deviceMap - Bitmap of mmwave devices to be disconnected
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   disconnects the mmwave radar devices
*   Close communication channel, Unregister Interrupt Handler
*/
/* DesignId :  */
/* Requirements :  */
rlReturnVal_t rlDriverRemoveDevices(rlUInt8_t deviceMap)
{
    rlUInt8_t index;
    rlReturnVal_t retVal = RL_RET_CODE_OK;

    RL_LOGV_ARG0("rlDriverRemoveDevices starts...\n");

    /* Clear the device map from driver data */
    rl_driverData.deviceMap &= ~deviceMap;

    for (index = 0U; ((deviceMap != 0U) && (index < RL_DEVICE_CONNECTED_MAX));\
        index++)
    {
        if ((deviceMap & (1U << index)) != 0U)
        {
            /* Close mmwave radar device communication Channel */
            if (rl_driverData.commDevIdx.comIfHdl[index] != RL_NULL_PTR)
            {
                retVal += rl_driverData.clientCtx.comIfCb.rlComIfClose( \
                                         rl_driverData.commDevIdx.comIfHdl[index]);
                rl_driverData.commDevIdx.comIfHdl[index] = RL_NULL_PTR;
            }

            /* Un Register Interrupt Handler */
            (void)rl_driverData.clientCtx.devCtrlCb.rlRegisterInterruptHandler(
                index, RL_NULL, RL_NULL);

            deviceMap &= ~(1U << index);
        }
    }
    RL_LOGV_ARG0("rlDriverRemoveDevices completes\n");
    return retVal;
}

/** @fn rlReturnVal_t rlDriverDeInit(void)
*
*   @brief De Initializes the mmwave radar Driver
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   DeInitializes the mmwave radar Driver. Clear memory, destroy OS objects,
*   Close communication channel, Unregister Interrupt Handler
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-711 */
rlReturnVal_t rlDriverDeInit(void)
{
    rlUInt8_t index;
    rlUInt8_t deviceMap = rl_driverData.deviceMap;
    rlReturnVal_t retVal = RL_RET_CODE_OK;

    RL_LOGV_ARG0("rlDriverDeInit starts...\n");

    for (index = 0U; ((deviceMap != 0U) && (index < RL_DEVICE_CONNECTED_MAX));\
        index++)
    {
        if ((deviceMap & (1U << index)) != 0U)
        {
            /* Close mmwave radar device communication Channel */
            if (rl_driverData.commDevIdx.comIfHdl[index] != RL_NULL_PTR)
            {
                retVal += rl_driverData.clientCtx.comIfCb.rlComIfClose( \
                                     rl_driverData.commDevIdx.comIfHdl[index]);
                rl_driverData.commDevIdx.comIfHdl[index] = RL_NULL_PTR;
            }

            /* Un Register Interrupt Handler */
            (void)rl_driverData.clientCtx.devCtrlCb.rlRegisterInterruptHandler(
                                                      index, RL_NULL, RL_NULL);

            deviceMap &= ~(1U << index);
        }
    }
    /* Destroy Global Mutex */
    if (rl_driverData.globalMutex != RL_NULL_PTR)
    {
        retVal += rl_driverData.clientCtx.osiCb.mutex.rlOsiMutexDelete(
                                                  &rl_driverData.globalMutex);
        rl_driverData.globalMutex = RL_NULL_PTR;
         RL_LOGD_ARG0("Destroy Global Mutex\n");
    }

    /* Destroy Command Semaphore */
    if (rl_driverData.cmdSem != RL_NULL_PTR)
    {
        retVal += rl_driverData.clientCtx.osiCb.sem.rlOsiSemDelete(\
                                                &rl_driverData.cmdSem);
        rl_driverData.cmdSem = RL_NULL_PTR;
        RL_LOGD_ARG0("Destroy Command Semaphore\n");
    }

    rl_driverData.deviceMap = 0U;
    rl_driverData.isDriverInitialized = 0U;
    RL_LOGV_ARG0("rlDriverDeInit ends...\n");

    return retVal;
}

/** @fn rlDriverData_t* rlDriverGetHandle(void)
*
*   @brief Returns mmwave radar Driver Global Structure
*
*   @return rlDriverData_t Pointer to mmwave radar Driver Global Structure
*
*   Returns mmwave radar Driver Global Structure
*/
/* DesignId :  */
/* Requirements :  */
rlDriverData_t* rlDriverGetHandle(void)
{
    /* return driverData pointer/handle */
    return (&rl_driverData);
}

/** @fn rlDriverGetPlatformId(void)
*
*   @brief Returns RL Platform ID (i.e. where mmWaveLink is executing)
*
*   @return rlUInt8_t platform
*
*   Returns Device run ID (i.e. where mmWaveLink is executing)
*/
/* DesignId :  */
/* Requirements :  */
rlUInt8_t rlDriverGetPlatformId(void)
{
    RL_LOGV_ARG0("rlDriverGetPlatformId is called \n");
    /* return platform ID */
    return (rl_driverData.clientCtx.platform);
}


/** @fn rlDriverGetArDeviceType(void)
*
*   @brief Returns AR device type which mmWavelink is communicating
*
*   @return rlUInt8_t platform
*
*   Returns AR device type which mmWavelink is communicating
*/
/* DesignId :  */
/* Requirements :  */
rlUInt8_t rlDriverGetArDeviceType(void)
{
    RL_LOGV_ARG0("rlDriverGetArDeviceType is called \n");
    /* return AWR device Type */
    return (rl_driverData.clientCtx.arDevType);
}

/** @fn rlUInt8_t rlDriverIsDeviceMapValid(rlUInt8_t deviceMap)
*
*   @brief Checks if given deviecMap is valid wrt to global DeviceMap
 *         set to mmWaveLink
*   @param[in] deviceMap - Bitmap of devices to send the message
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   @brief It enables RF/Analog Subsystem.
*/
/* DesignId :  */
/* Requirements :  */
rlReturnVal_t rlDriverIsDeviceMapValid(rlUInt8_t deviceMap)
{
    rlUInt8_t storedDevMap;
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDriverIsDeviceMapValid starts \n");

    /* get the no. of connected device */
    storedDevMap = rl_driverData.deviceMap;

    if ((storedDevMap & deviceMap) != 0U)
    {
        /* set return value to success */
        retVal = 0;
    }
    else
    {
        /* set return value to failure */
        retVal = -1;
        RL_LOGE_ARG0("Device map is In-valid \n");
    }
    RL_LOGV_ARG0("rlDriverIsDeviceMapValid ends \n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverWaitForResponse(rlUInt8_t devIndex,
*                                             rlDriverMsg_t* outMsg)
*
*   @brief: Wait for Device's response
*
*   @param[in ] devIndex - devIndex is index of device not the deviceMap
*   @param[out] outMsg   - Driver msg
*
*   @return Success : 0, Fail : Error code
*
*   Wait for driver response
*/
/* DesignId : MMWL_DesignId_024 */
/* Requirements : AUTORADAR_REQ-774 */
/*AR_CODE_REVIEW MR:R.2.2 <APPROVED> "payloadLen is re initialized after each pass to the
  rlGetSubBlock fucntion as it is passed again to the function" */
/*LDRA_INSPECTED 8 D */
rlReturnVal_t rlDriverWaitForResponse(rlUInt8_t devIndex,
                            rlDriverMsg_t* outMsg)
{
    rlUInt16_t rspChunks;
    rlPayloadSb_t errorSB;
    rlReturnVal_t retVal, retVal1;
    rlUInt16_t indx;
    rlUInt8_t payloadLen = 0U;

    RL_LOGV_ARG0("rlDriverWaitForResponse starts... \n");

    do
    {
        /* Wait for Host Interrupt and Read the Response */
        retVal1 = rlDriverMsgReadCmdCtx(devIndex);
        if (RL_RET_CODE_OK == retVal1)
        {
            /* Get number of chunks in the response */
            rspChunks = rl_rxMsg.hdr.remChunks;

            /* Check if Number of Sub Block doesn't exceed expected number */
            if (outMsg->opcode.nsbc >= rl_rxMsg.hdr.nsbc)
            {
                outMsg->opcode.nsbc = rl_rxMsg.hdr.nsbc;
                /* Loop for number of chunks and copy payload */
                for (indx = 0U; indx < (outMsg->opcode.nsbc); indx++)
                {
                    /* Copy Payload to output variable*/
                    /* AR_CODE_REVIEW MR:R.18.4 <APPROVED> "pointer arithmetic required" */
                    /*LDRA_INSPECTED 87 S */
                    (void)rlGetSubBlock(rl_rxMsg.payload + payloadLen,
                                        &(outMsg->subblocks[indx].sbid),
                                        &(outMsg->subblocks[indx].len),
                                        outMsg->subblocks[indx].pSblkData);
                    payloadLen += (rlUInt8_t)(outMsg->subblocks[indx].len);

                }
            }
            retVal = retVal1;
        }
        else if (RL_RET_CODE_MSGID_MISMATCHED == retVal1)
        {
            /* Number of Sub Block is unexpected. Error Case */
            /* AR_CODE_REVIEW MR:R.2.2  <APPROVED> "Values are used by called function.
               LDRA Tool Issue" */
            /*LDRA_INSPECTED 105 D */
            rlErrorResp_t errMsgSbData = {(rlSysNRespType_t)0U, (rlUInt16_t)0U};

            errorSB.pSblkData = (rlUInt8_t* )&errMsgSbData;

            /* Copy Payload to local variable*/
            /* AR_CODE_REVIEW MR:R.18.4 <APPROVED> "pointer arithmetic required" */
            /*LDRA_INSPECTED 87 S */
            (void)rlGetSubBlock(rl_rxMsg.payload + payloadLen,
                            &(errorSB.sbid), &(errorSB.len),
                            errorSB.pSblkData);

            /* Return Error to indicate command failure */
            retVal = (rlReturnVal_t)(errMsgSbData.errorType);
            rspChunks = 0U;

            RL_LOGE_ARG0("msg id mis-match, command failure\n");

        }
        else
        {
            /* Timeout in receiving response*/
            rspChunks = 0U;
            retVal = retVal1;
            RL_LOGE_ARG0("Timeout in receiving response\n");
        }
    }
    while (rspChunks > 0U);

    RL_LOGV_ARG0("rlDriverWaitForResponse ends... \n");

    return retVal;
}

static rlReturnVal_t rlDriverCmdWriter(rlUInt8_t devIndex, rlDriverMsg_t* outMsg)
{
    rlReturnVal_t retVal;
    rlUInt8_t retryCount = 0U;
    rlDriverData_t *rlDrvData = rlDriverGetHandle();
    rlUInt8_t isPayloadValid = RL_FALSE;

    do
    {
        if (retryCount != 0U)
        {
            /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Global variable is used by
            other function" */
            /*LDRA_INSPECTED 105 D */
            /* AR_CODE_REVIEW MR:D.4.1,D.4.14 <APPROVED> "rlDrvData Can't be NULL" */
            /*LDRA_INSPECTED 45 D */
             rlDrvData->funcParams.cmd->hdr.flags.b2RetryFlag = RL_HDR_FLAG_RETRY;
        }
        retryCount++;
    
        if ( rlDrvData->clientCtx.ackTimeout != 0U)
        {
            /* Set flag to true and check for it in Host IRQ  handler routine*/
            /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Global variable is used by
            other function" */
            /*LDRA_INSPECTED 105 D */
             rlDrvData->isCmdRespWaited[devIndex] = RL_TRUE;
        }
        /* send the command to mmWave Radar device */
        retVal = rlDriverMsgWrite(rlDrvData,  rlDrvData->commDevIdx.comIfHdl[devIndex]);
    
        if (RL_RET_CODE_OK == retVal)
        {
            /* Check if It needs to wait for ACK*/
            if (rlDrvData->clientCtx.ackTimeout != 0U)
            {
                /* wait for respond */
                retVal += rlDriverWaitForResponse(devIndex, outMsg);
                RL_LOGD_ARG0("rlDriverCmdWriter, wait for respond\n");
            }
    
            /* Response received Successfully */
            if ((RL_RET_CODE_OK == retVal) || \
               ((RL_RET_CODE_CRC_FAILED != retVal) && \
               (RL_RET_CODE_RESP_TIMEOUT != retVal)))
            {
                /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Set to TRUE for valid payload" */
                /*LDRA_INSPECTED 8 D */
                isPayloadValid = RL_TRUE;
                /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Global variable is used by
                other function" */
                /*LDRA_INSPECTED 105 D */
                 rlDrvData->isCmdRespWaited[devIndex] = RL_FALSE;
                /* Increment the Sequence Number */
                 rlDrvData->cmdSeqNum[devIndex]++;
            }
    
        }
        else
        {
            /* Error in command message write */
            if (rlDrvData->clientCtx.ackTimeout != 0U)
            {
                 rlDrvData->isCmdRespWaited[devIndex] = RL_FALSE;
            }
            RL_LOGE_ARG0("rlDriverCmdWriter, Error in cmd msg write\n");
            break;
        }
    }
    while ((retryCount < RL_API_CMD_RETRY_COUNT) && (isPayloadValid == RL_FALSE));

    return retVal;
}

/** @fn rlReturnVal_t rlDriverCmdSendRetry(rlUInt8_t deviceMap, rlDriverMsg_t* outMsg)
*
*   @brief: Send command and wait for response
*
*   @param[in ] deviceMap - device map contains bitmap of all device connected
*   @param[out] outMsg   - output msg
*
*   @return Success : 0, Fail : Error code
*
*   Send command and wait for response
*/
/* DesignId : MMWL_DesignId_023 */
/* Requirements : AUTORADAR_REQ-772, AUTORADAR_REQ-774, AUTORADAR_REQ-775, AUTORADAR_REQ-778,\
 AUTORADAR_REQ-781 */
rlReturnVal_t rlDriverCmdSendRetry(rlUInt8_t deviceMap, rlDriverMsg_t* outMsg)
{
    rlReturnVal_t retVal;
    rlUInt8_t devIndex = 0U;
    rlDriverData_t *rlDrvData = rlDriverGetHandle();

    if ((rlDrvData == RL_NULL) || (rlDrvData->funcParams.cmd == RL_NULL))
    {
        retVal = RL_RET_CODE_FATAL_ERROR;
    }
    else
    {
        retVal = RL_RET_CODE_OK;
    }

    /* Send Command to Device connected and wait for response one by one */
    /* AR_CODE_REVIEW MR:D.2.1 <APPROVED> "This loop terminates when it sends commands to all
     * connected devices or when any of devices returns -ve response" */
    /*LDRA_INSPECTED 28 D */
    while ((deviceMap != 0U) && (RL_RET_CODE_OK == retVal))
    {
        if ((deviceMap & (1U << devIndex)) != 0U)
        {
            /* Fill Command Sequence Number */
            /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Global variable is used by other function" */
            /*LDRA_INSPECTED 105 D */
            /*LDRA_INSPECTED 8 D */
            rlDrvData->funcParams.cmd->hdr.flags.b4SeqNum =
                            rlDrvData->cmdSeqNum[devIndex] % 16U;

            /* Write command to slave device */
            retVal += rlDriverCmdWriter(devIndex, outMsg);

            deviceMap &= ~(1U << devIndex);
        }/* End of If */
        /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Global variable is used by other function" */
        /*LDRA_INSPECTED 105 D */
         rlDrvData->isCmdRespWaited[devIndex] = RL_FALSE;
        devIndex++;
    }/* End for Loop */

    return retVal;
}

/** @fn rlReturnVal_t rlDriverCmdInvoke(rlUInt8_t deviceMap,
*                        rlDriverMsg_t inMsg, rlDriverMsg_t* outMsg)
*
*   @brief Invokes a command to mmwave radar Device. Implements mmwave radar
*          Host Communication Protocol(RHCP)
*
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] inMsg - Command Opcode(Direction, Class, MsgId) and Subblocks
*   @param[out] outMsg - Response Opcode(Direction, Class, MsgId) and Subblocks
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Invokes a command to mmwave radar Device. Waits for Response if ACK is
*   requested. Computes
*   CRC/Checksum according to mmwave radar Host Communication Protocol(RHCP)
*/
/* DesignId : MMWL_DesignId_023 */
/* Requirements : AUTORADAR_REQ-772, AUTORADAR_REQ-774, AUTORADAR_REQ-776 */
rlReturnVal_t rlDriverCmdInvoke(rlUInt8_t deviceMap, rlDriverMsg_t inMsg,
                              rlDriverMsg_t* outMsg)
{
    rlReturnVal_t retVal;
    rlUInt8_t protAlignSize;
    rlUInt16_t indx, msgCrcLen;
    rlUInt16_t payloadLen = 0U;

    RL_LOGV_ARG0("rlDriverCmdInvoke starts... \n");

    if (rl_driverData.isDriverInitialized != (rlUInt8_t)0U)
    {
        rlReturnVal_t retVal1 = RL_RET_CODE_OK;
        (void)rl_driverData.clientCtx.osiCb.mutex.rlOsiMutexLock
          (&(rl_driverData.globalMutex), (rlOsiTime_t)RL_OSI_WAIT_FOREVER);

        /* Fill Command Header */
        /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Some of variables are re-assigned based
                on different conditions."*/
        /*LDRA_INSPECTED 8 D */
        rl_txMsg.hdr.opcode.b4Direction = inMsg.opcode.dir;
        rl_txMsg.hdr.opcode.b2MsgType   = inMsg.opcode.msgType;
        rl_txMsg.hdr.opcode.b10MsgId    = inMsg.opcode.msgId;
        rl_txMsg.hdr.remChunks          = inMsg.remChunks;
        rl_txMsg.hdr.nsbc               = inMsg.opcode.nsbc;
        rl_txMsg.hdr.flags.b2AckFlag = 0U;
        rl_txMsg.hdr.flags.b2Crc = 0U;
        rl_txMsg.hdr.flags.b2CrcLen = 0U;
        rl_txMsg.hdr.flags.b2RetryFlag = 0U;
        rl_txMsg.hdr.flags.b4SeqNum = 0U;
        rl_txMsg.hdr.flags.b4Version = 0U;

        /* Fill Payload */
        for (indx = 0U; indx < inMsg.opcode.nsbc; indx++)
        {
            /* append all subblock len, id and data to one global structure */
            retVal1 += rlAppendSubBlock(&rl_txMsg.payload[payloadLen],
                                        inMsg.subblocks[indx].sbid,
                                        inMsg.subblocks[indx].len,
                                        inMsg.subblocks[indx].pSblkData);
            /* increment payload length as appending each sub-block data in a message */
            payloadLen += inMsg.subblocks[indx].len + \
                                        (rlUInt16_t)(RL_SBC_ID_SIZE + RL_SBC_LEN_SIZE);
            if (RL_RET_CODE_OK != retVal1)
            {
                break;
            }
        }

        /* if above for loop is not terminated due to any error */
        if (indx == inMsg.opcode.nsbc)
        {
            rl_txMsg.hdr.len = RHCP_HEADER_LEN + payloadLen;

            /* Update command and response buffers */
            rl_driverData.funcParams.cmd = &rl_txMsg;
            rl_driverData.funcParams.rsp = &rl_rxMsg;

            /* Check if ACK is Requested*/
            if (rl_driverData.clientCtx.ackTimeout == 0U)
            {
                /* No ACK Requested */
                rl_txMsg.hdr.flags.b2AckFlag = RL_HDR_FLAG_NO_ACK;
            }

            /* Check if CRC is required to be sent*/
            if (rl_driverData.clientCtx.crcType == RL_CRC_TYPE_NO_CRC)
            {
                /* CRC Not Included */
                rl_txMsg.hdr.flags.b2Crc = RL_HDR_FLAG_NO_CRC;
            }

            /* Append Dummy Bytes if CRC is present */
            if (rl_driverData.funcParams.cmd->hdr.flags.b2Crc == RL_HDR_FLAG_CRC)
            {
                rl_txMsg.hdr.flags.b2CrcLen = rl_driverData.clientCtx.crcType;

                /* It may be size 2/4/8 based on 16/32/64 bit */
                msgCrcLen = (2U << (rl_driverData.clientCtx.crcType & 0x3U));

                protAlignSize = (rl_driverData.clientCtx.crcType < RL_CRC_TYPE_64BIT_ISO) ? \
                                 RL_PAYLOAD_MSG_4BYTE_MULT : RL_PAYLOAD_MSG_8BYTE_MULT;

                /* Add Padding Byte to payload - This is required before CRC calculation*/
                if ((rl_driverData.funcParams.cmd->hdr.len % protAlignSize) != 0U)
                {
                    /* AR_CODE_REVIEW MR:R.18.1,R.18.4 <APPROVED> "pointer arithmetic required" */
                    /*LDRA_INSPECTED 567 S */
                    /* AR_CODE_REVIEW MR:R.10.3  <APPROVED> "All parameter types are matching to
                                           function argument type. LDRA tool issue." */
                    /*LDRA_INSPECTED 458 S */
                    /*LDRA_INSPECTED 87 S */
                    (void)rlAppendDummy((rlUInt8_t*)((rlUInt8_t*)rl_driverData.funcParams.cmd + \
                         rl_driverData.funcParams.cmd->hdr.len + SYNC_PATTERN_LEN), \
                         (rlUInt8_t)(protAlignSize - \
                         (rlUInt8_t)(rl_driverData.funcParams.cmd->hdr.len % protAlignSize)));
                    rl_driverData.funcParams.cmd->hdr.len += (rlUInt16_t)(protAlignSize - \
                        (rlUInt16_t)(rl_driverData.funcParams.cmd->hdr.len % protAlignSize));
                }
               rl_driverData.funcParams.cmd->hdr.len += msgCrcLen;
            }

            retVal = rlDriverCmdSendRetry(deviceMap, outMsg);
            /* Release the Global Mutex */
            (void)rl_driverData.clientCtx.osiCb.mutex.rlOsiMutexUnLock(\
                                             &(rl_driverData.globalMutex));
        }
        else
        {
            retVal = retVal1;
        }
    }
    else
    {
        retVal = RL_RET_CODE_INVALID_STATE_ERROR;
        RL_LOGE_ARG0("rlDriverCmdInvoke, Invalid input \n");
    }

    RL_LOGV_ARG0("rlDriverCmdInvoke ends... \n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverConfigureCrc(rlCrcType_t crcType)
*
*   @brief  Configures the CRC Type in mmwavelink Driver
*   @param[in] crcType - CRC Types
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Configures the CRC Type in mmwavelink Driver
*/
rlReturnVal_t rlDriverConfigureCrc(rlCrcType_t crcType)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDriverConfigureCrc starts... \n");
    /* Check if driver is initialized */
    if (rl_driverData.isDriverInitialized == 1U)
    {
        /* Set CRC Type to global structure */
        rl_driverData.clientCtx.crcType = crcType;
        RL_LOGD_ARG0("rlDriverConfigureCrc is success\n");
        retVal = RL_RET_CODE_OK;
    }
    else
    {
         /* if driver is not initialized then return and error value */
        retVal = RL_RET_CODE_INVALID_STATE_ERROR;
    }
    RL_LOGV_ARG0("rlDriverConfigureCrc ends... \n");

    return retVal;
}

/** @fn rlReturnVal_t rlDriverConfigureAckTimeout(rlUInt32_t ackTimeout)
*
*   @brief  Configures the Acknowledgement timeout in mmwavelink Driver
*   @param[in] ackTimeout - ACK timeout, 0 - No ACK
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Configures the Acknowledgement timeout in mmwavelink Driver
*/
/* DesignId :  */
/* Requirements :  */
rlReturnVal_t rlDriverConfigureAckTimeout(rlUInt32_t ackTimeout)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlDriverConfigureAckTimeout starts... \n");
    /* Check if driver is initialized */
    if (rl_driverData.isDriverInitialized == 1U)
    {
        /* set ACK timeout to global structure */
        rl_driverData.clientCtx.ackTimeout = ackTimeout;
        RL_LOGD_ARG0("rlDriverConfigureAckTimeout is success\n");
        retVal = RL_RET_CODE_OK;
    }
    else
    {
        /* if driver is not initialized then return and error value */
        retVal = RL_RET_CODE_INVALID_STATE_ERROR;
    }
    RL_LOGV_ARG0("rlDriverConfigureAckTimeout ends... \n");

    return retVal;
}

/** @fn rlPrintFptr rlGetLogFptr(rlUInt8_t dbgLevel)
*
*   @brief: Configure loggin function to print error message
*   @param[in] dbgLevel - Debug level
*
*   @return rlPrintFptr Success - valid fucntion pointer,
*                       Failure - NULL
*
*   Configures the logging function for error in mmwavelink Driver
*/
/* DesignId :  */
/* Requirements :  */
rlPrintFptr rlGetLogFptr(rlUInt8_t dbgLevel)
{
    rlPrintFptr retFuncPtr;
    if (dbgLevel > 0U)
    {
        /* If debug level is valid the set debug function pointer */
        retFuncPtr = rl_driverData.logObj.rlPrintAr[dbgLevel - (rlUInt8_t)1U];
    }
    else
    {
        /* If debug level is valid the set debug function pointer to NULL */
        retFuncPtr = RL_NULL_PTR;
    }

    return retFuncPtr;
}

/** @fn rlReturnVal_t rlLogInit(void)
*
*   @brief: Initialise logging function pointers.
*
*   @return rlReturnVal_t Success - ( 0),
*                         Failure - (-1)
*
*   Initialise logging functions in mmwavelink Driver
*/
/* DesignId : MMWL_DesignId_031 */
/* Requirements : AUTORADAR_REQ-712 */
rlReturnVal_t rlLogInit(void)
{
    rlReturnVal_t retVal;
    rlDriverData_t* rlDrvData = &rl_driverData;
    rlPrintFptr fPtr;
    rlUInt8_t level, idx;
    /* store debug level to local variable */
    level = rlDrvData->clientCtx.dbgCb.dbgLevel;
    /* store Function pointer to local variable */
    fPtr  = rlDrvData->clientCtx.dbgCb.rlPrint;

    /* check for Function pointer for NON-NULL */
    if (fPtr != RL_NULL_PTR)
    {
        switch (level)
        {
            case RL_DBG_LEVEL_VERBOSE :
            case RL_DBG_LEVEL_DEBUG   :
            case RL_DBG_LEVEL_INFO    :
            case RL_DBG_LEVEL_WARNING :
            case RL_DBG_LEVEL_ERROR   :
                for (idx = level; idx > RL_DBG_LEVEL_NONE; idx--)
                {
                    rlDrvData->logObj.rlPrintAr[idx - 1U] = fPtr;
                }
                break;
            case RL_DBG_LEVEL_NONE    :
            {
                (void)fPtr("INFO: MMWAVELINK Logging is disabled\n");
                /* reset all function pointers to NULL */
                (void)memset(&rlDrvData->logObj.rlPrintAr[0], 0U, sizeof(rlLogCtx_t));
                break;
            }
            default                   :
            {
                (void)fPtr("INFO: Invalid MMWAVELINK Logging, hence disbled\n");
                /* if dbgLevel is set beyond expected value then assign NONE */
                rlDrvData->clientCtx.dbgCb.dbgLevel = RL_DBG_LEVEL_NONE;

                /* reset all function pointers to NULL */
                (void)memset(&rlDrvData->logObj.rlPrintAr[0], 0U, sizeof(rlLogCtx_t));
                break;
            }
        }
        retVal = RL_RET_CODE_OK;
    }
    else
    {
        /* reset all function pointers to NULL */
        (void)memset(&rlDrvData->logObj.rlPrintAr[0], 0U, sizeof(rlLogCtx_t));
        retVal = RL_RET_CODE_INTERFACE_CB_NULL;
    }

    return retVal;
}

/** @fn void rlDriverConstructInMsg(rlUInt16_t msgId, rlDriverMsg_t* inMsg,
*    rlPayloadSb_t* payloadPtr)
*
*   @brief: Construct command packet (inMsg) based on given message-ID and payload
*
*   @param[in] msgId - message ID of command packet
*   @param[out] inMsg - Message strucutre input pointer
*   @param[in] payloadPtr - payload data pointer
*   @return - none
*
*   Construct Set command packet based on given message-ID and payload
*/
void rlDriverConstructInMsg(rlUInt16_t msgId, rlDriverMsg_t* inMsg, rlPayloadSb_t* payloadPtr)
{
    /* check for NULL pointer */
    if (inMsg != NULL)
    {
        rlUInt8_t cmdDir;
        /* Set Command Header Opcode */
        inMsg->opcode.nsbc = 1U;
        inMsg->opcode.msgType = RL_API_CLASS_CMD;
        inMsg->opcode.msgId = msgId;
        inMsg->remChunks = 0U;
        /* get command direction based on requested MsgId */
        cmdDir = rlDeviceIdentifyCmdDir(msgId, rlDriverGetPlatformId());
        inMsg->opcode.dir = cmdDir;
        inMsg->subblocks = payloadPtr;
    }
    else
    {
        /* Error: iutMsg is sent as NULL */
        RL_LOGV_ARG0("rlDriverConstructInMsg construct InMsg failed!!!\n");
    }
}

/** @fn void rlDriverConstructOutMsg(rlUInt16_t numSblk, rlDriverMsg_t* outMsg,
*                                       rlPayloadSb_t* payloadPtr)
*
*   @brief: Construct command packet based on given message-ID and payload
*
*   @param[in] numSblk - number of sub-blocks in message
*   @param[out] outMsg - Message strucutre input pointer
*   @param[in] payloadPtr - payload data pointer
*   @return - none
*
*   Construct Get command packet based on given message-ID and payload
*/
void rlDriverConstructOutMsg(rlUInt16_t numSblk, rlDriverMsg_t* outMsg,
                                                        rlPayloadSb_t* payloadPtr)
{
    /* check for NULL pointer */
    if (outMsg != NULL)
    {
        /* Set num of sub-block to outMsg Opcode field */
        outMsg->opcode.nsbc = numSblk;
        /* assign payload pointer to subblock ot outMsg */
        outMsg->subblocks = payloadPtr;
    }
    else
    {
        /* Error: outMsg is sent as NULL */
        RL_LOGV_ARG0("rlDriverConstructOutMsg construct OutMsg failed!!!\n");
    }
}

/** @fn void rlDriverFillPayload(rlUInt16_t msgId, rlUInt16_t sbcID, rlPayloadSb_t* payloadPtr,
*    rlUInt8_t* data, rlUInt16_t inLen)
*
*   @brief: Fill payload based on given message-ID, sub-block ID and data.
*   @param[in] msgId - message ID of command packet
*   @param[in] sbcID  - sub block ID of command packet
*   @param[in] payloadPtr - payload data pointer
*   @param[out] data - data pointer
*   @param[in] inLen - lenght of data
*   @return - none
*
*   Fill payload based on given message-ID, sub-block ID and data.
*/
void rlDriverFillPayload(rlUInt16_t msgId, rlUInt16_t sbcID, rlPayloadSb_t* payloadPtr,
                                rlUInt8_t* data, rlUInt16_t inLen)
{
    /* check for NULL pointer */
    if (payloadPtr != NULL)
    {
        /* get Unique Sub-Block ID and asign it to Command Sub Block */
        payloadPtr->sbid = (rlUInt16_t)RL_GET_UNIQUE_SBID(msgId, sbcID);

         /* set Command Sub-block length to sizeof command strcuture type */
        payloadPtr->len  = inLen;
         /* set sub-block data pointer to payload structure */
        payloadPtr->pSblkData = data;
    }
    else
    {
        RL_LOGV_ARG0("rlDriverFillPayload data fill failed!!!\n");
    }
}

/** @fn rlReturnVal_t rlDriverExecuteGetApi(rlUInt8_t deviceMap, rlUInt16_t msgId,
*                                           rlUInt16_t sbcID, rlUInt8_t *msgData)
*
*   @brief: Construct get message and invoke command.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] msgId - message ID of command packet
*   @param[in] sbID  - sub block ID of command packet
*   @param[out] data - message data pointer
*   @param[in] inLen - message payload length
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Construct get message and invoke command where it waits for response from device.
*/
rlReturnVal_t rlDriverExecuteGetApi(rlUInt8_t deviceMap, rlUInt16_t msgId,
                                     rlUInt16_t sbcID, rlUInt8_t *msgData, rlUInt16_t inLen)
{
    /*LDRA waiver   8 D - DD data flow anomalies found- */
    rlReturnVal_t retVal;
    /* Initialize Command and Response Sub Blocks */
    /* Initialize in-message structure to zero */
    rlDriverMsg_t inMsg = {0};
    /* Initialize out-message structure to zero */
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Values are used by called function. LDRA Tool Issue" */
    /*LDRA_INSPECTED 105 D */
    rlDriverMsg_t outMsg = {0};
    /* Initialize in-payload sub-block structure to zero */
    rlPayloadSb_t inPayloadSb = {0};
    /* Initialize out-payload sub-block structure to zero */
    rlPayloadSb_t outPayloadSb = {0};

    /* Construct command packet */
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Values are used by called function. LDRA Tool Issue" */
    /*LDRA_INSPECTED 8 D */
    rlDriverConstructInMsg(msgId, &inMsg, &inPayloadSb);

    /* Fill in-message Payload */
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Values are used by called function. LDRA Tool Issue" */
    /*LDRA_INSPECTED 105 D */
    rlDriverFillPayload(msgId, sbcID, &inPayloadSb, NULL , 0U);

    /* Construct response packet */
    rlDriverConstructOutMsg(1U, &outMsg, &outPayloadSb);

    /* Fill out-message Payload */
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Values are used by called function. LDRA Tool Issue" */
    /*LDRA_INSPECTED 105 D */
    rlDriverFillPayload(0U, 0U, &outPayloadSb, msgData, inLen);

    /* Send Command to mmWave Radar Device */
    /* AR_CODE_REVIEW MR:D.4.1,D.4.14 <APPROVED> "Can't be NULL" */
    /*LDRA_INSPECTED 45 D */
    retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);

    return retVal;
}

/** @fn rlReturnVal_t rlDriverExecuteSetApi(rlUInt8_t deviceMap, rlUInt16_t msgId,
*                                           rlUInt16_t sbcID, rlUInt8_t *msgData)
*
*   @brief: Construct set message and invoke command.
*   @param[in] deviceMap - Bitmap of devices to send the message
*   @param[in] msgId - message ID of command packet
*   @param[in] sbID  - sub block ID of command packet
*   @param[in] data - message data pointer
*   @param[in] inLen - message payload length
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Construct set message and invoke command where it waits for response from device.
*/
rlReturnVal_t rlDriverExecuteSetApi(rlUInt8_t deviceMap, rlUInt16_t msgId,
                                    rlUInt16_t sbcID, rlUInt8_t *msgData, rlUInt16_t inLen)
{
    /*LDRA waiver   8 D - DD data flow anomalies found- */
    rlReturnVal_t retVal;
    /* Initialize in-message structure to zero */
    rlDriverMsg_t inMsg = {0};
    /* Initialize out-message structure to zero */
    rlDriverMsg_t outMsg = {0};
    /* Initialize in-payload sub-block structure to zero */
    rlPayloadSb_t inPayloadSb = {0};

    /* Construct command packet */
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Values are updated in other function.
                                                                     LDRA Tool Issue" */
    /*LDRA_INSPECTED 8 D */
    rlDriverConstructInMsg(msgId, &inMsg, &inPayloadSb);

    /* Fill in-message Payload */
    /* AR_CODE_REVIEW MR:R.2.2 <APPROVED> "Values are used by called function.
                                                                     LDRA Tool Issue" */
    /*LDRA_INSPECTED 105 D */
    rlDriverFillPayload(msgId, sbcID, &inPayloadSb, msgData, inLen);

    /* Send Command to mmWave Radar Device */
    /* AR_CODE_REVIEW MR:D.4.1,D.4.14 <APPROVED> "Can't be NULL" */
    /*LDRA_INSPECTED 45 D */
    retVal = rlDriverCmdInvoke(deviceMap, inMsg, &outMsg);

    return retVal;
}

/*
 * END OF rl_driver.c FILE
 */
