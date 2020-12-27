/****************************************************************************************
 * FileName     : rl_protocol.h
 *
 * Description  : This file defines the functions required for Communication Protocol
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
#ifndef RL_PROTOCOL_H
#define RL_PROTOCOL_H

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/* RHCP Length Constants*/
#define SYNC_PATTERN_LEN                    (4U)
#define RHCP_HEADER_LEN                     (12U)
#define RL_CRC_LEN_MAX                      (8U)     /* RHCP CRC max Length in bytes */

/**< Max Payload Len (256 - 16(HDR) - 8(CRC)) */
#define RL_CMD_PL_LEN_MAX                   ((RL_MAX_SIZE_MSG - (SYNC_PATTERN_LEN \
                                             + RHCP_HEADER_LEN + RL_CRC_LEN_MAX)))

/*! \brief
* mmWaveLink API Error Type
*/
typedef rlUInt16_t rlSysNRespType_t;

/*! \brief
* mmWaveLink API Error Sub block structure
*/
typedef struct rlErrorResp
{
    /**
     *@ brief  16 bits error type
     */
    rlSysNRespType_t   errorType;
    /**
     *@ brief  16 bits SBC ID
     */
    rlUInt16_t            sbcID;
}rlErrorResp_t;


/*! \brief
* RHCP SYNC Pattern Structure
*/
typedef struct rlSyncPattern
{
    rlUInt16_t sync1;
    rlUInt16_t sync2;
}rlSyncPattern_t;

/*! \brief
* Command op-code ID contains 4 fields (16 bits)
* Bit 10-15: Reserved
* Bit 8-9: Operation - Set/Get.
* Bit 4-7: Operation Type
* Bit 0-3: Direction
*/
#ifndef MMWL_BIG_ENDIAN
typedef struct rlOpcode
{
    /**
     * @brief  Direction
     */
    rlUInt16_t     b4Direction         : 4;
    /**
     * @brief  Msg Type
     */
    rlUInt16_t     b2MsgType           : 2;
    /**
     * @brief  Message ID
     */
    rlUInt16_t     b10MsgId            : 10;

}rlOpcode_t;
#else
typedef struct rlOpcode
{
    /**
     * @brief  Message ID
     */
    rlUInt16_t     b10MsgId            : 10;
    /**
     * @brief  Msg Type
     */
    rlUInt16_t     b2MsgType           : 2;
    /**
     * @brief  rlUInt16_t
     */
    rlUInt16_t     b4Direction         : 4;
}rlOpcode_t;
#endif
/*! \brief
* Command op-code ID contains 4 fields (16 bits)
* Bit 10-15: Reserved
* Bit 8-9: Operation - Set/Get.
* Bit 4-7: Operation Type
* Bit 0-3: Direction
*/
#ifndef MMWL_BIG_ENDIAN
typedef struct rlHdrFlags
{
    /**
     * @brief  00- No Retry, 11 - Retry
     */
    rlUInt16_t     b2RetryFlag        : 2;
    /**
     * @brief  00- ACK requested, 11 - No ACK Requested
     */
    rlUInt16_t     b2AckFlag          : 2;
    /**
     * @brief  0000 - Invalid, 0001 - 1111 - Valid Range
     */
    rlUInt16_t     b4Version          : 4;
    /**
     * @brief  00 - CRC present, 11 - CRC not Present
     */
    rlUInt16_t     b2Crc              : 2;
    /**
     * @brief  Length of CRC appended to the message
                    00    16-bit CRC
                    01    32-bit CRC
                    10    64-bit CRC
                    11    Reserved
     */
    rlUInt16_t     b2CrcLen           : 2;
    /**
     * @brief  Sequence Number
     */
    rlUInt16_t     b4SeqNum           : 4;
}rlHdrFlags_t;
#else
typedef struct rlHdrFlags
{
    /**
     * @brief  Sequence Number
     */
    rlUInt16_t     b4SeqNum           : 4;
    /**
     * @brief  Length of CRC appended to the message
                    00    16-bit CRC
                    01    32-bit CRC
                    10    64-bit CRC
                    11    Reserved
     */
    rlUInt16_t     b2CrcLen           : 2;
    /**
     * @brief  00 - CRC present, 11 - CRC not Present
     */
    rlUInt16_t     b2Crc              : 2;
    /**
     * @brief  0000 - Invalid, 0001 - 1111 - Valid Range
     */
    rlUInt16_t     b4Version          : 4;
    /**
     * @brief  00- No ACK, 11 - ACK Req
     */
    rlUInt16_t     b2AckFlag          : 2;
    /**
     * @brief  00- No Retry, 11 - Retry
     */
    rlUInt16_t     b2RetryFlag        : 2;
}rlHdrFlags_t;

#endif

/*! \brief
* RHCP protocol header structure
*/
typedef struct rlProtHeader
{
    /**
     * @brief  rlUInt16_t, rlApiActionType,rlApiGetSetType
     */
    rlOpcode_t opcode;
    rlUInt16_t len;
    rlHdrFlags_t flags;
    rlUInt16_t remChunks;
    rlUInt16_t nsbc;
    rlUInt16_t chksum;
}rlProtHeader_t;

/*! \brief
* RHCP message structure
*/
typedef struct rlRhcpMsg
{
    rlSyncPattern_t syncPattern;
    rlProtHeader_t  hdr;
    rlUInt8_t      payload[RL_CMD_PL_LEN_MAX + RL_CRC_LEN_MAX];
}rlRhcpMsg_t;


#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF RL_PROTOCOL_H FILE
 */
