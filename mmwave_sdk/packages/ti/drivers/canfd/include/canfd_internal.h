/*!
 *   @file  canfd_internal.h
 *
 *   @brief
 *      This is an internal header file used by the CANFD module and should
 *      not be included directly by the applications.
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

/**
@defgroup CANFD_DRIVER_INTERNAL_FUNCTION            CANFD Driver Internal Functions
@ingroup CANFD_DRIVER
@brief
*   The section has a list of all the internal functions used by the driver
*/
/**
@defgroup CANFD_DRIVER_INTERNAL_DATA_STRUCTURE      CANFD Driver Internal Data Structures
@ingroup CANFD_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifndef CANFD_INTERNAL_H
#define CANFD_INTERNAL_H

#include <ti/drivers/osal/HwiP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/canfd/include/hw_mcanss.h>
#include <ti/drivers/canfd/canfd.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief      Maximum number of Tx message objects that can be supported by CANFD
 */
#define MCAN_MAX_TX_MSG_OBJECTS             (32U)

/*!
 *  @brief      Maximum number of Rx message objects that can be supported by CANFD
 */
#define MCAN_MAX_RX_MSG_OBJECTS             (32U)

/*!
 *  @brief      Maximum number of message objects that can be supported by CANFD
 */
#define MCAN_MAX_MSG_OBJECTS                (MCAN_MAX_TX_MSG_OBJECTS + MCAN_MAX_RX_MSG_OBJECTS)

/*! @brief  Standard ID Filter Element Size */
#define MCAN_MSG_RAM_STD_ELEM_SIZE          (1U)

/*! @brief  Extended ID Filter Element Size */
#define MCAN_MSG_RAM_EXT_ELEM_SIZE          (2U)

/*! @brief  Tx/Rx Element Size.
 * 18 words = 18 * 4 = 72 bytes: 8 bytes of header and 64 bytes of data */
#define MCAN_MSG_RAM_TX_RX_ELEM_SIZE        (18U)

/*! @brief  Message Identifier Masks */
#define XTD_MSGID_MASK                      (0x1fffffffU)
#define STD_MSGID_MASK                      (0x7ffU)
#define STD_MSGID_SHIFT                     (18U)

/*! @brief  Maximum payload supported by CAN-FD protocol in bytes. */
#define MCAN_MAX_PAYLOAD_BYTES              (64U)

/*! @brief  Maximum number of Rx buffers. */
#define MCAN_MAX_RX_BUFFERS                 (64U)

/*! @brief  Maximum number of Tx buffers. */
#define MCAN_MAX_TX_BUFFERS                 (32U)

/** @addtogroup CANFD_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief    Enumerates the values used to represent the MCAN interrupt lines
 */
typedef enum MCAN_IntrLineNum_t
{
    /*! MCAN interrupt line 0 */
    MCAN_IntrLineNum_0,

    /*! MCAN interrupt line 1 */
    MCAN_IntrLineNum_1
}MCAN_IntrLineNum;

/*!
 *  @brief    This enumeration defines the values used to represent the MCAN Message RAM type
 */
typedef enum MCAN_MemType_t
{
    /*! MCAN Message RAM buffers */
    MCAN_MemType_BUF = 0U,

    /**< MCAN Message RAM FIFO/Queue */
    MCAN_MemType_FIFO = 1U
}MCAN_MemType;

/*!
 *  @brief    This enumeration defines the MCAN receive FIFO number
 */
typedef enum MCAN_RxFIFONum_t
{
    /*! MCAN Rx FIFO 0 */
    MCAN_RxFIFONum_0 = 0U,

    /*! MCAN Rx FIFO 1 */
    MCAN_RxFIFONum_1 = 1U
}MCAN_RxFIFONum;

/**
 *  @brief    This enumeration defines the MCAN FIFO/Buffer element Size
 */
typedef enum CANFD_MCANElemSize_t
{
    /*! 8 byte data field */
    CANFD_MCANElemSize_8BYTES = 0U,

    /*! 12 byte data field */
    CANFD_MCANElemSize_12BYTES = 1U,

    /*! 16 byte data field */
    CANFD_MCANElemSize_16BYTES = 2U,

    /*! 20 byte data field */
    CANFD_MCANElemSize_20BYTES = 3U,

    /*! 24 byte data field */
    CANFD_MCANElemSize_24BYTES = 4U,

    /*! 32 byte data field */
    CANFD_MCANElemSize_32BYTES = 5U,

    /*! 48 byte data field */
    CANFD_MCANElemSize_48BYTES = 6U,

    /*! 64 byte data field */
    CANFD_MCANElemSize_64BYTES = 7U
}CANFD_MCANElemSize;

/*!
 *  @brief    This enumeration defines the values used to represent the CANFD driver state
 */
typedef enum CANFD_DriverState_t
{
    /*! CANFD controller not initialized */
    CANFD_DriverState_UNINIT,

    /*! CANFD controller started */
    CANFD_DriverState_STARTED,

    /*! CANFD controller stopped */
    CANFD_DriverState_STOPPED,

    /*! CANFD controller in sleep mode */
    CANFD_DriverState_SLEEP
}CANFD_DriverState;

/*!
 * @brief       This enumeration defines the values for MCAN interrupts.
 */
typedef enum MCAN_IntrSrc_t
{
    /*! Rx FIFO 0 New Message interrupt */
    MCAN_INTR_SRC_RX_FIFO0_NEW_MSG = MCAN_IR_RF0N_MASK,

    /*! Rx FIFO 0 Watermark Reached interrupt */
    MCAN_INTR_SRC_RX_FIFO0_WATERMARK = MCAN_IR_RF0W_MASK,

    /*! Rx FIFO 0 Full interrupt */
    MCAN_INTR_SRC_RX_FIFO0_FULL = MCAN_IR_RF0F_MASK,

    /*! Rx FIFO 0 Message Lost interrupt */
    MCAN_INTR_SRC_RX_FIFO0_MSG_LOST = MCAN_IR_RF0L_MASK,

    /*! Rx FIFO 1 New Message interrupt */
    MCAN_INTR_SRC_RX_FIFO1_NEW_MSG = MCAN_IR_RF1N_MASK,

    /*! Rx FIFO 1 Watermark Reached interrupt */
    MCAN_INTR_SRC_RX_FIFO1_WATERMARK = MCAN_IR_RF1W_MASK,

    /*! Rx FIFO 1 Full interrupt */
    MCAN_INTR_SRC_RX_FIFO1_FULL = MCAN_IR_RF1F_MASK,

    /*! Rx FIFO 1 Message Lost interrupt */
    MCAN_INTR_SRC_RX_FIFO1_MSG_LOST = MCAN_IR_RF1L_MASK,

    /*! High Priority Message interrupt */
    MCAN_INTR_SRC_HIGH_PRIO_MSG = MCAN_IR_HPM_MASK,

    /*! Transmission Completed interrupt */
    MCAN_INTR_SRC_TRANS_COMPLETE = MCAN_IR_TC_MASK,

    /*! Transmission Cancellation Finished interrupt */
    MCAN_INTR_SRC_TRANS_CANCEL_FINISH = MCAN_IR_TCF_MASK,

    /*! Tx FIFO Empty interrupt */
    MCAN_INTR_SRC_TX_FIFO_EMPTY = MCAN_IR_TFE_MASK,

    /*! Tx Event FIFO New Entry interrupt */
    MCAN_INTR_SRC_TX_EVT_FIFO_NEW_ENTRY = MCAN_IR_TEFN_MASK,

    /*! Tx Event FIFO Watermark Reached interrupt */
    MCAN_INTR_SRC_TX_EVT_FIFO_WATERMARK = MCAN_IR_TEFW_MASK,

    /*! Tx Event FIFO Full interrupt */
    MCAN_INTR_SRC_TX_EVT_FIFO_FULL = MCAN_IR_TEFF_MASK,

    /*! Tx Event FIFO Element Lost interrupt */
    MCAN_INTR_SRC_TX_EVT_FIFO_ELEM_LOST = MCAN_IR_TEFL_MASK,

    /*! Timestamp Wraparound interrupt */
    MCAN_INTR_SRC_TIMESTAMP_WRAPAROUND = MCAN_IR_TSW_MASK,

    /*! Message RAM Access Failure interrupt */
    MCAN_INTR_SRC_MSG_RAM_ACCESS_FAILURE = MCAN_IR_MRAF_MASK,

    /*! Timeout Occurred interrupt */
    MCAN_INTR_SRC_TIMEOUT = MCAN_IR_TOO_MASK,

    /*! Message stored to Dedicated Rx Buffer interrupt */
    MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG = MCAN_IR_DRX_MASK,

    /*! Bit Error Corrected interrupt */
    MCAN_INTR_SRC_BIT_ERR_CORRECTED = MCAN_IR_BEC_MASK,

    /*! Bit Error Uncorrected interrupt */
    MCAN_INTR_SRC_BIT_ERR_UNCORRECTED = MCAN_IR_BEU_MASK,

    /*! Error Logging Overflow interrupt */
    MCAN_INTR_SRC_ERR_LOG_OVRFLW = MCAN_IR_ELO_MASK,

    /*! Error Passive interrupt */
    MCAN_INTR_SRC_ERR_PASSIVE = MCAN_IR_EP_MASK,

    /*! Warning Status interrupt */
    MCAN_INTR_SRC_WARNING_STATUS = MCAN_IR_EW_MASK,

    /*! Bus_Off Status interrupt */
    MCAN_INTR_SRC_BUS_OFF_STATUS = MCAN_IR_BO_MASK,

    /*! Watchdog Interrupt interrupt */
    MCAN_INTR_SRC_WATCHDOG = MCAN_IR_WDI_MASK,

    /*! Protocol Error in Arbitration Phase interrupt */
    MCAN_INTR_SRC_PROTOCOL_ERR_ARB = MCAN_IR_PEA_MASK,

    /*! Protocol Error in Data Phase interrupt */
    MCAN_INTR_SRC_PROTOCOL_ERR_DATA = MCAN_IR_PED_MASK,

    /*! Access to Reserved Address interrupt */
    MCAN_INTR_SRC_RES_ADDR_ACCESS = MCAN_IR_ARA_MASK
}MCAN_IntrSrc;

/**
 * @brief  Data structure defines the MCAN configuration parameters.
 */
typedef struct MCAN_ConfigParams_t
{
    /*! Bus Monitoring Mode
     *   0 - Bus Monitoring Mode is disabled
     *   1 - Bus Monitoring Mode is enabled
     */
    uint32_t                monEnable;

    /*! Restricted Operation Mode
     *   0 - Normal CAN operation
     *   1 - Restricted Operation Mode active
     *   This mode should not be combined with test modes.
     */
    uint32_t                asmEnable;

    /*! Timestamp Counter Prescaler.
     *   Range:[0x0-0xF]
     */
    uint32_t                tsPrescalar;

    /*! Time-out source selection.
     *   00 - Timestamp counter value always 0x0000
     *   01 - Timestamp counter value incremented according to tsPrescalar
     *   10 - External timestamp counter value used
     *   11 - Same as 00
     */
    uint32_t                tsSelect;

    /*! Time-out counter source select.
     *   Refer enum #CANFD_MCANTimeOutSelect.
     */
    uint32_t                timeoutSelect;

    /*! Start value of the Timeout Counter (down-counter).
     *   The Timeout Counter is decremented in multiples of CAN bit times [1-16]
     *   depending on the configuration of the tsPrescalar.
     *   Range:[0x0-0xFFFF]
     */
    uint32_t                timeoutPreload;

    /*! Time-out Counter Enable
     *   0 - Time-out Counter is disabled
     *   1 - Time-out Counter is enabled
     */
    uint32_t                timeoutCntEnable;

    /*! Global Filter Configuration parameters.
     *    Refer struct #CANFD_MCANGlobalFiltConfig.
     */
    CANFD_MCANGlobalFiltConfig filterConfig;
}MCAN_ConfigParams;

/**
 * @brief  Data structure defines the MCAN new data flag for Rx buffer.
 */
typedef struct MCAN_RxNewDataStatus_t
{
    /*! New data flag for Rx buffer no. 0 to 31 */
    uint32_t                statusLow;

    /*! New data flag for Rx buffer no. 32 to 63 */
    uint32_t                statusHigh;
}MCAN_RxNewDataStatus;

/**
 * @brief  Data structure defines the MCAN Rx FIFO Status.
 */
typedef struct MCAN_RxFIFOStatus_t
{
    /*! Rx FIFO number
     *   Refer enum #MCAN_RxFIFONum
     */
    uint32_t                num;

    /*! Rx FIFO Fill Level */
    uint32_t                fillLvl;

    /*! Rx FIFO Get Index */
    uint32_t                getIdx;

    /*! Rx FIFO Put Index */
    uint32_t                putIdx;

    /*! Rx FIFO Full
     *   0 - Rx FIFO not full
     *   1 - Rx FIFO full
     */
    uint32_t                fifoFull;

    /*! Rx FIFO Message Lost */
    uint32_t                msgLost;
}MCAN_RxFIFOStatus;

/**
 * @brief  Data structure defines the MCAN Tx FIFO Status.
 */
typedef struct MCAN_TxFIFOStatus_t
{
    /*! Tx FIFO Free Level */
    uint32_t                freeLvl;

    /*! Tx FIFO Get Index
     *   Read as zero when Tx Queue operation is configured.
     */
    uint32_t                getIdx;

    /*! Tx FIFO/Queue Put Index */
    uint32_t                putIdx;

    /*! Tx FIFO/Queue Full
     *   0 - Tx FIFO/Queue not full
     *   1 - Tx FIFO/Queue full
     */
    uint32_t                fifoFull;
}MCAN_TxFIFOStatus;

/**
 * @brief  Data structure defines the MCAN Tx Event FIFO Status.
 */
typedef struct MCAN_TxEventFIFOStatus_t
{
    /*! Event FIFO Fill Level */
    uint32_t                fillLvl;

    /*! Event FIFO Gut Index */
    uint32_t                getIdx;

    /*! Event FIFO Put Index */
    uint32_t                putIdx;

   /*! Event FIFO Full
     *   0 - Tx Event FIFO not full
     *   1 - Tx Event FIFO full
     */
    uint32_t                fifoFull;

   /*! Tx Event FIFO Element Lost
     *   0 - No Tx Event FIFO element lost
     *   1 - Tx Event FIFO element lost, also set after write attempt to
     *       Tx Event FIFO of size zero.
     */
    uint32_t                eleLost;
}MCAN_TxEventFIFOStatus;

/**
 * @brief  Data structure defines the MCAN Tx Buffer element.
 */
typedef struct MCAN_TxBufElement_t
{
    /*! Identifier */
    uint32_t                id;

    /*! Remote Transmission Request
     *   0 - Transmit data frame
     *   1 - Transmit remote frame
     */
    uint32_t                rtr;

    /*! Extended Identifier
     *   0 - 11-bit standard identifier
     *   1 - 29-bit extended identifier
     */
    uint32_t                xtd;

    /*! Error State Indicator
     *   0 - ESI bit in CAN FD format depends only on error passive flag
     *   1 - ESI bit in CAN FD format transmitted recessive
     */
    uint32_t                esi;

    /*! Data Length Code
     *   0-8  - CAN + CAN FD: transmit frame has 0-8 data bytes
     *   9-15 - CAN: transmit frame has 8 data bytes
     *   9-15 - CAN FD: transmit frame has 12/16/20/24/32/48/64 data bytes
     */
    uint32_t                dlc;

    /*! Bit Rate Switching
     *   0 - CAN FD frames transmitted without bit rate switching
     *   1 - CAN FD frames transmitted with bit rate switching
     */
    uint32_t                brs;

    /*! FD Format
     *   0 - Frame transmitted in Classic CAN format
     *   1 - Frame transmitted in CAN FD format
     */
    uint32_t                fdf;

    /*! Event FIFO Control
     *   0 - Do not store Tx events
     *   1 - Store Tx events
     */
    uint32_t                efc;

    /*! Message Marker */
    uint32_t                mm;

    /*! Data bytes.
     *   Only first dlc number of bytes are valid.
     */
    uint8_t                 data[MCAN_MAX_PAYLOAD_BYTES];
}MCAN_TxBufElement;

/**
 * @brief  Data structure defines the MCAN Rx Buffer element.
 */
typedef struct MCAN_RxBufElement_t
{
    /*! Identifier */
    uint32_t                id;

    /*! Remote Transmission Request
     *   0 - Received frame is a data frame
     *   1 - Received frame is a remote frame
     */
    uint32_t                rtr;

    /*! Extended Identifier
     *   0 - 11-bit standard identifier
     *   1 - 29-bit extended identifier
     */
    uint32_t                xtd;

    /*! Error State Indicator
     *   0 - Transmitting node is error active
     *   1 - Transmitting node is error passive
     */
    uint32_t                esi;

    /*! Rx Timestamp */
    uint32_t                rxts;

    /*! Data Length Code
     *   0-8  - CAN + CAN FD: received frame has 0-8 data bytes
     *   9-15 - CAN: received frame has 8 data bytes
     *   9-15 - CAN FD: received frame has 12/16/20/24/32/48/64 data bytes
     */
    uint32_t                dlc;

    /*! Bit Rat Switching
     *   0 - Frame received without bit rate switching
     *   1 - Frame received with bit rate switching
     */
    uint32_t                brs;

    /*! FD Format
     *   0 - Standard frame format
     *   1 - CAN FD frame format (new DLC-coding and CRC)
     */
    uint32_t                fdf;

    /*! Filter Index */
    uint32_t                fidx;

    /*! Accepted Non-matching Frame
     *   0 - Received frame matching filter index FIDX
     *   1 - Received frame did not match any Rx filter element
     */
    uint32_t                anmf;

    /*! Data buffer pointer.
     */
    void*                   dataPtr;
}MCAN_RxBufElement;

/**
 * @brief  Data structure defines the MCAN Tx Event FIFO element.
 */
typedef struct MCAN_TxEventFIFOElement_t
{
    /*! Identifier */
    uint32_t                id;

    /*! Remote Transmission Request
     *   0 - Data frame transmitted
     *   1 - Remote frame transmitted
     */
    uint32_t                rtr;

    /*! Extended Identifier
     *   0 - 11-bit standard identifier
     *   1 - 29-bit extended identifier
     */
    uint32_t                xtd;

    /*! Error State Indicator
     *   0 - Transmitting node is error active
     *   1 - Transmitting node is error passive
     */
    uint32_t                esi;

    /*! Tx Timestamp */
    uint32_t                txts;

    /*! Data Length Code
     *   0-8  - CAN + CAN FD: frame with 0-8 data bytes transmitted
     *   9-15 - CAN: frame with 8 data bytes transmitted
     *   9-15 - CAN FD: frame with 12/16/20/24/32/48/64 data bytes transmitted
     */
    uint32_t                dlc;

    /*! Bit Rate Switching
     *   0 - Frame transmitted without bit rate switching
     *   1 - Frame transmitted with bit rate switching
     */
    uint32_t                brs;

    /*! FD Format
     *   0 - Standard frame format
     *   1 - CAN FD frame format (new DLC-coding and CRC)
     */
    uint32_t                fdf;

    /*! Event Type
     *   00 - Reserved
     *   01 - Tx event
     *   10 - Transmission in spite of cancellation
     *        (always set for transmissions in DAR mode)
     *   11 - Reserved
     */
    uint32_t                et;

    /*! Message Marker */
    uint32_t                mm;
}MCAN_TxEventFIFOElement;

/**
 * @brief  Data structure defines the MCAN Standard Message ID Filter Element.
 */
typedef struct MCAN_StdMsgIDFilterElement_t
{
    /*! Standard Filter ID 2 */
    uint32_t                sfid2;

    /*! Standard Filter ID 1 */
    uint32_t                sfid1;

    /*! Standard Filter Element Configuration
     *   000 - Disable filter element
     *   001 - Store in Rx FIFO 0 if filter matches
     *   010 - Store in Rx FIFO 1 if filter matches
     *   011 - Reject ID if filter matches
     *   100 - Set priority if filter matches
     *   101 - Set priority and store in FIFO 0 if filter matches
     *   110 - Set priority and store in FIFO 1 if filter matches
     *   111 - Store into Rx Buffer or as debug message,
     *         configuration of SFT[1:0] ignored.
     *   If SFEC - 100b, 101b, or 110b a match sets high priority
     *   message event is generated.
     */
    uint32_t                sfec;

   /*! Standard Filter Type
     *   00 - Range filter from SFID1 to SFID2 (SFID2 >= SFID1)
     *   01 - Dual ID filter for SFID1 or SFID2
     *   10 - Classic filter: SFID1 = filter, SFID2 = mask
     *   11 - Filter element disabled
     */
    uint32_t                sft;
}MCAN_StdMsgIDFilterElement;

/**
 * @brief  Data structure defines the MCAN Extended Message ID Filter Element.
 */
typedef struct MCAN_ExtMsgIDFilterElement_t
{
    /*! Extended Filter ID 1 */
    uint32_t                efid1;

    /*! Extended Filter Element Configuration
     *   000 - Disable filter element
     *   001 - Store in Rx FIFO 0 if filter matches
     *   010 - Store in Rx FIFO 1 if filter matches
     *   011 - Reject ID if filter matches
     *   100 - Set priority if filter matches
     *   101 - Set priority and store in FIFO 0 if filter matches
     *   110 - Set priority and store in FIFO 1 if filter matches
     *   111 - Store into Rx Buffer or as debug message,
     *         configuration of SFT[1:0] ignored.
     *   If EFEC = 100b, 101b, or 110b a match sets high priority
     *   message event is generated.
     */
    uint32_t                efec;

    /*! Extended Filter ID 2 */
    uint32_t                efid2;

    /*! Extended Filter Type
     *   00 - Range filter from EFID1 to EFID2 (EFID2 >= EFID1)
     *   01 - Dual ID filter for EFID1 or EFID2
     *   10 - Classic filter: EFID1 = filter, EFID2 = mask
     *   11 - Range filter from EFID1 to EFID2 (EFID2 >= EFID1),
     *        XIDAM mask not applied
     */
    uint32_t                eft;
}MCAN_ExtMsgIDFilterElement;

/**
 * @brief  Data structure defines the MCAN Message RAM Configuration Parameters.
 *         Message RAM can contain following sections:
 *         Standard ID filters, Extended ID filters, TX FIFO(or TX Q),
 *         TX Buffers, TX EventFIFO, RX FIFO0, RX FIFO1, RX Buffer.
 *         Note: If particular section in the RAM is not used then it's size
 *         should be initialized to '0'
 *         (Number of buffers in case of Tx/Rx buffer).
 */
typedef struct MCAN_MsgRAMConfigParams_t
{
    /*! Standard ID Filter List Start Address */
    uint32_t                flssa;

    /*! List Size: Standard ID
     *   0 - No standard Message ID filter
     *   1-127 - Number of standard Message ID filter elements
     *   others - Values greater than 128 are interpreted as 128
     */
    uint32_t                lss;

    /*! Extended ID Filter List Start Address */
    uint32_t                flesa;

    /*! List Size: Extended ID
     *   0 - No standard Message ID filter
     *   1-64 - Number of standard Message ID filter elements
     *   others - Values greater than 64 are interpreted as 64
     */
    uint32_t                lse;

    /*! Tx Buffers Start Address */
    uint32_t                txStartAddr;

    /*! Number of Dedicated Transmit Buffers
     *   0 - No Dedicated Tx Buffers
     *   1-32 - Number of Dedicated Tx Buffers
     *   others - Values greater than 32 are interpreted as 32
     */
    uint32_t                txBufNum;

    /*! Transmit FIFO/Queue Size
     *   0 - No Tx FIFO/Queue
     *   1-32 - Number of Tx Buffers used for Tx FIFO/Queue
     *   others - Values greater than 32 are interpreted as 32
     */
    uint32_t                txFIFOSize;

    /*! Tx FIFO/Queue Mode
     *   0 - Tx FIFO operation
     *   1 - Tx Queue operation
     */
    uint32_t                txBufMode;

    /*! Tx Buffer Element Size */
    uint32_t                txBufElemSize;

    /*! Tx Event FIFO Start Address */
    uint32_t                txEventFIFOStartAddr;

    /*! Event FIFO Size
     *   0 - Tx Event FIFO disabled
     *   1-32 - Number of Tx Event FIFO elements
     *   others - Values greater than 32 are interpreted as 32
     */
    uint32_t                txEventFIFOSize;

    /*! Tx Event FIFO Watermark
     *   0 - Watermark interrupt disabled
     *   1-32 - Level for Tx Event FIFO watermark interrupt
     *   others - Watermark interrupt disabled
     */
    uint32_t                txEventFIFOWaterMark;

    /*! Rx FIFO0 Start Address */
    uint32_t                rxFIFO0startAddr;

    /*! Rx FIFO0 Size
     *   0 - No Rx FIFO
     *   1-64 - Number of Rx FIFO elements
     *   others - Values greater than 64 are interpreted as 64
     */
    uint32_t                rxFIFO0size;

    /*! Rx FIFO0 Watermark
     *   0 - Watermark interrupt disabled
     *   1-63 - Level for Rx FIFO 0 watermark interrupt
     *   others - Watermark interrupt disabled
     */
    uint32_t                rxFIFO0waterMark;

    /*! Rx FIFO0 Operation Mode
     *   0 - FIFO blocking mode
     *   1 - FIFO overwrite mode
     */
    uint32_t                rxFIFO0OpMode;

    /*! Rx FIFO1 Start Address */
    uint32_t                rxFIFO1startAddr;

    /*! Rx FIFO1 Size
     *   0 - No Rx FIFO
     *   1-64 - Number of Rx FIFO elements
     *   others - Values greater than 64 are interpreted as 64
     */
    uint32_t                rxFIFO1size;

    /*! Rx FIFO1 Watermark
     *   0 - Watermark interrupt disabled
     *   1-63 - Level for Rx FIFO 1 watermark interrupt
     *   others - Watermark interrupt disabled
     */
    uint32_t                rxFIFO1waterMark;

    /*! Rx FIFO1 Operation Mode
     *   0 - FIFO blocking mode
     *   1 - FIFO overwrite mode
     */
    uint32_t                rxFIFO1OpMode;

    /*! Rx Buffer Start Address */
    uint32_t                rxBufStartAddr;

    /*! Rx Buffer Element Size */
    uint32_t                rxBufElemSize;

    /*! Rx FIFO0 Element Size */
    uint32_t                rxFIFO0ElemSize;

    /*! Rx FIFO1 Element Size */
    uint32_t                rxFIFO1ElemSize;
}MCAN_MsgRAMConfigParams;

/**
 * @brief  Data structure defines the MCAN High Priority Message.
 */
typedef struct MCAN_HighPriorityMsgInfo_t
{
    /*! Buffer Index
     *   Only valid when MSI[1] - 1.
     */
    uint32_t                bufIdx;

    /*! Message Storage Indicator
     *   MSI[1:0]
     *       00  - No FIFO selected
     *       01  - FIFO message lost
     *       10  - Message stored in FIFO 0
     *       11  - Message stored in FIFO 1
     */
    uint32_t                msi;

    /*! Filter Index */
    uint32_t                filterIdx;

    /*! Indicates the filter list of the matching filter element
     *   0 - Standard Filter List
     *   1 - Extended Filter List
     */
    uint32_t                filterList;
}MCAN_HighPriorityMsgInfo;

/**
 * @brief   Data structure defines the parameters for bit timing calculation.
 *          Bit timing related to data phase will be valid only in case where
 *          MCAN is put in CANFD mode and will be '0' otherwise.
 */
typedef struct MCAN_BitTimingParams_t
{
    /*! Nominal Baud Rate Pre-scaler
     *   Range: [0x0-0x1FF]
     */
    uint32_t        nomRatePrescalar;

    /*! Nominal Time segment before sample point
     *   Range: [0x0-0xFF]
     */
    uint32_t        nomTimeSeg1;

    /*! Nominal Time segment after sample point
     *   Range: [0x0-0x7F]
     */
    uint32_t        nomTimeSeg2;

    /*! Nominal (Re)Synchronization Jump Width
     *   Range: [0x0-0x7F]
     */
    uint32_t        nomSynchJumpWidth;

    /*! Data Baud Rate Pre-scaler
     *   Range: [0x0-0x1F]
     */
    uint32_t        dataRatePrescalar;

    /*! Data Time segment before sample point
     *   Range: [0x0-0x1F]
     */
    uint32_t        dataTimeSeg1;

    /*! Data Time segment after sample point
     *   Range: [0x0-0xF]
     */
    uint32_t        dataTimeSeg2;

    /*! Data (Re)Synchronization Jump Width
     *   Range: [0x0-0xF]
     */
    uint32_t        dataSynchJumpWidth;
}MCAN_BitTimingParams;

/**
 * @brief   Data structure defines the accessing Revision ID and Core Release Info.
 *          of MCAN module.
 */
typedef struct MCAN_RevisionId_t
{
    /*! Scheme */
    uint32_t                scheme;

    /*! Business Unit: 10 = Processors */
    uint32_t                bu;

    /*! Module ID */
    uint32_t                modId;

    /*! RTL revision */
    uint32_t                rtlRev;

    /*! Major revision */
    uint32_t                major;

    /*! Custom revision */
    uint32_t                custom;

    /*! Minor revision */
    uint32_t                minor;

    /*! Time Stamp Day. Two digits, BCD-coded. */
    uint32_t                day;

    /*! Time Stamp Month. Two digits, BCD-coded. */
    uint32_t                mon;

    /*! Time Stamp Year. Single digit, BCD-coded. */
    uint32_t                year;

    /*! Sub-step of Core Release Single digit, BCD-coded. */
    uint32_t                subStep;

    /*! Step of Core Release. Two digits, BCD-coded Single digit, BCD-coded. */
    uint32_t                step;

    /*! Core Release. Single digit, BCD-coded. */
    uint32_t                rel;
}MCAN_RevisionId;

/**
 * @brief   Data structure defines the accessing Revision ID of ECC AGGR.
 */
typedef struct MCAN_ECCAggrRevisionId_t
{
    /*! Scheme */
    uint32_t                scheme;

    /*! Business Unit: 10 = Processors */
    uint32_t                bu;

    /*! Module ID */
    uint32_t                modId;

    /*! RTL revision */
    uint32_t                rtlRev;

    /*! Major revision */
    uint32_t                major;

    /*! Custom revision */
    uint32_t                custom;

    /*! Minor revision */
    uint32_t                minor;
}MCAN_ECCAggrRevisionId;

/**
 * @brief   Data structure defines the accessing Revision ID of ECC wrapper.
 */
typedef struct MCAN_ECCWrapRevisionId_t
{
    /*! Scheme */
    uint32_t                scheme;

    /*! Business Unit: 10 = Processors */
    uint32_t                bu;

    /*! Module ID */
    uint32_t                modId;

    /*! RTL revision */
    uint32_t                rtlRev;

    /*! Major revision */
    uint32_t                major;

    /*! Custom revision */
    uint32_t                custom;

    /*! Minor revision */
    uint32_t                minor;
}MCAN_ECCWrapRevisionId;

/**
 * @brief  Defines all the interrupt are enabled.
 */
#define MCAN_INTR_MASK       ((uint32_t)MCAN_INTR_SRC_RX_FIFO0_NEW_MSG | \
                              (uint32_t)MCAN_INTR_SRC_RX_FIFO0_MSG_LOST |  \
                              (uint32_t)MCAN_INTR_SRC_RX_FIFO1_NEW_MSG | \
                              (uint32_t)MCAN_INTR_SRC_TRANS_COMPLETE |  \
                              (uint32_t)MCAN_INTR_SRC_TRANS_CANCEL_FINISH |  \
                              (uint32_t)MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG |  \
                              (uint32_t)MCAN_INTR_SRC_PROTOCOL_ERR_ARB |  \
                              (uint32_t)MCAN_INTR_SRC_PROTOCOL_ERR_DATA |  \
                              (uint32_t)MCAN_INTR_SRC_BUS_OFF_STATUS)
/**
 * @brief
 *  CANFD Hardware Atrributes
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
typedef struct CANFD_HwCfg_t
{
    /*!
     * @brief   Base address of the register address space to be used.
     */
    uint32_t                regBaseAddress;

    /*!
     * @brief   Interrupt level 0 Number
     */
    uint32_t                interruptNum0;

    /*!
     * @brief   Interrupt level 1 Number
     */
    uint32_t                interruptNum1;
}CANFD_HwCfg;

/**
 * @brief
 *  CANFD Master Control Block
 *
 * @details
 *  The structure describes the CANFD Driver and is used to hold the relevant
 *  information with respect to the CANFD module.
 */
typedef struct CANFD_DriverMCB_t
{
    /**
     * @brief   CANFD driver internal state
     */
    CANFD_DriverState               state;

    /**
     * @brief   Hardware Configuration of the CAN instance
     */
    CANFD_HwCfg                     hwCfg;

    /**
     * @brief   Data Length to DLC mapping
     */
    uint8_t                         mcanDataSize[16];

    /**
     * @brief   Message object handle book keeping
     */
    struct CANFD_MessageObject_t*  msgObjectHandle[MCAN_MAX_MSG_OBJECTS];

    /**
     * @brief   Tx Mapping to message handles for transmit post processing
     */
    struct CANFD_MessageObject_t*   txMapping[MCAN_MAX_TX_MSG_OBJECTS];

    /**
     * @brief   Rx Mapping to message handles for Rx processing
     */
    struct CANFD_MessageObject_t*   rxMapping[MCAN_MAX_RX_MSG_OBJECTS];

    /**
     * @brief   Registered Interrupt Handler. Indicates if interrupt 0 is configured. This needs to be done only
     * during the first message object create. Subsequent interrupts are enabled using interrupt muxing
     */
    HwiP_Handle                     hwiHandle0;

    /**
     * @brief   Registered Interrupt Handler. Indicates if interrupt 0 is configured. This needs to be done only
     * during the first message object create. Subsequent interrupts are enabled using interrupt muxing
     */
    HwiP_Handle                     hwiHandle1;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CANFD driver on error or status interrrupts.
     */
    CANFD_ErrStatusAppCallBack      appErrCallBack;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CANFD driver once Tx complete or data receive interrupt has been received.
     */
    CANFD_DataAppCallBack           appDataCallBack;

    /**
     * @brief   Number of error and status interrupts received
     */
    uint32_t                        errStatusInterrupts;

    /**
     * @brief   Number of interrupts received for message Tx or Rx
     */
    uint32_t                        interrupts;

    /**
     * @brief   Number of ECC interrupts received
     */
    uint32_t                        eccInterrupts;

    /**
     * @brief   Number of Bus-Off interrupts received
     */
    uint32_t                        busOffInterrupts;

    /**
     * @brief   Number of Protocol error in data phase interrupts received
     */
    uint32_t                        protoDataErrInterrupts;

    /**
     * @brief   Number of Protocol error in arbitration phase interrupts received
     */
    uint32_t                        protoArbErrInterrupts;

    /**
     * @brief   Tx Status of the message object
     */
    uint8_t                         txStatus[MCAN_MAX_TX_MSG_OBJECTS];

    /**
     * @brief   Flag to toggle the usage of FIFO 0 and FIFO 1. Valid values are 0 and 1.
     */
    uint32_t                        useFifoNum;

    /**
     * @brief   Buffer used to read message RAM.
     */
    MCAN_RxBufElement               rxBuffElem;
}CANFD_DriverMCB;

/**
 * @brief
 *  CAN message object block
 *
 * @details
 *  The structure defines the message object
 */
typedef struct CANFD_MessageObject_t
{
    /**
     * @brief   Starting range of the Message Id to which the configuration belongs.
     * For Tx and single Message Id objects the startMsgId = endMsgId.
     */
    uint32_t                startMsgId;

    /**
     * @brief   Ending range of the Message Id to which the configuration belongs
     * For Tx and single Message Id objects the startMsgId = endMsgId.
     */
    uint32_t                endMsgId;

    /**
     * @brief   Pointer to the CANFD driver MCB
     */
    CANFD_DriverMCB*        ptrDriverMCB;

    /**
     * @brief   Message object direction.
     */
    CANFD_Direction         direction;

    /**
     * @brief   Message object type.
     */
    CANFD_MCANXidType       msgIdType;

    /**
     * @brief   Allocated message object number
     */
    uint32_t                messageObjNum;

    /**
     * @brief   Tx buffer number used to send data
     */
    uint32_t                txElement;

    /**
     * @brief   Rx buffer number used to receive data
     */
    uint32_t                rxElement;

    /**
     * @brief   Part of message ram to accessed by this message object. Refer enum #MCAN_MemType.
     */
    MCAN_MemType            memType;

    /**
     * @brief   Number of interrupts received
     */
    uint32_t                interruptsRxed;

    /**
     * @brief   Number of messages processed
     */
    uint32_t                messageProcessed;
}CANFD_MessageObject;

/** @}*/

extern CANFD_HwCfg gCanFdHwCfg[1];

/** @addtogroup CANFD_DRIVER_INTERNAL_FUNCTION
 @{ */

/**
 * \brief   This API is used to initiate reset for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  None.
 */
void MCAN_reset(uint32_t baseAddr);

/**
 * \brief   This function checks if the MCAN module is in Reset.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \retval  state           Returns TRUE if not in reset.
 *                          Else returns FALSE.
 */
uint32_t MCAN_isInReset(uint32_t baseAddr);

/**
 * \brief   This API will return flexible data rate operation status
 *          MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  state           Returns TRUE if flexible data rate operation
 *                          is enabled. Else returns FALSE.
 */
uint32_t MCAN_isFDOpEnable(uint32_t baseAddr);

/**
 * \brief   This function checks if the memory initialization is done for
 *          MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \retval  state           Returns TRUE if memory initialization is done.
 *                          Else returns FALSE.
 */
uint32_t MCAN_isMemInitDone(uint32_t baseAddr);

/**
 * \brief   This API will set MCAN module mode of operation.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   mode            Mode of operation.
 *                          Refer enum #CANFD_MCANOperationMode.
 *
 * \return  None.
 */
void MCAN_setOpMode(uint32_t baseAddr, uint32_t mode);

/**
 * \brief   This API will return MCAN module mode of operation.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  mode            Mode of operation.
 *                          Refer enum #CANFD_MCANOperationMode.
 */
uint32_t MCAN_getOpMode(uint32_t baseAddr);

/**
 * \brief   This API will initialize MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   initParams      Initialization parameters.
 *                          Refer struct #CANFD_MCANInitParams.
 *
 * \return  status          Initialization status.
 */
int32_t MCAN_init(uint32_t baseAddr, const CANFD_MCANInitParams *initParams);

/**
 * \brief   This API will configure MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   configParams    configuration parameters.
 *                          Refer struct #CANFD_MCANInitParams.
 *
 * \return  status          Configuration status.
 */
int32_t MCAN_config(uint32_t baseAddr, const CANFD_MCANInitParams *configParams);

/**
 * \brief   This API will enable/disable ECC on the Message RAM.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   configParams    MCAN ECC Configuration Parameters.
 *                          Refer struct #CANFD_MCANECCConfigParams.
 *
 * \return  None.
 */
void MCAN_eccConfig(uint32_t                        baseAddr,
                    const CANFD_MCANECCConfigParams *configParams);

/**
 * \brief   This API will configure a bit timings for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   configParams    Configuration parameters for MCAN bit timing.
 *                          Refer struct #MCAN_BitTimingParams.
 *
 * \return  status          Bit Timings configuration status.
 */
int32_t MCAN_setBitTime(uint32_t                    baseAddr,
                        const MCAN_BitTimingParams *configParams);

/**
 * \brief   This API will configure Different sections of Message RAM.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   msgRAMConfigParams
 *                          Message RAM Configuration parameters.
 *                          Refer struct #MCAN_MsgRAMConfigParams.
 *
 * \return  status          Configuration Status.
 */
int32_t MCAN_msgRAMConfig(uint32_t                       baseAddr,
                          const MCAN_MsgRAMConfigParams *msgRAMConfigParams);

/**
 * \brief   This API will configure Extended ID AND Mask.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   idMask          Configuration parameters for MCAN Extended Id mask.
 *                          This value is 29 bit wide.
 *
 * \return  status          Extended ID AND Mask configuration status.
 */
int32_t MCAN_setExtIDAndMask(uint32_t baseAddr, uint32_t idMask);

/**
 * \brief   This API is used to write Tx message to message RAM.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   memType         Part of message ram to which given message to write.
 *                          Refer enum #MCAN_MemType.
 * \param   bufNum          Buffer  number where message to write.
 *                          This parameter will ignored if memType is FIFO/Q.
 * \param   elem            Message Object.
 *                          Refer struct #MCAN_TxBufElement.
 *
 * \return  None.
 */
void MCAN_writeMsgRam(uint32_t                 baseAddr,
                      uint32_t                 memType,
                      uint32_t                 bufNum,
                      const MCAN_TxBufElement *elem);

/**
 * \brief   This API will set Tx Buffer Add Request.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   bufNum          Tx Buffer number for which request is to be added.
 *
 * \return  status          Configuration Status.
 */
int32_t MCAN_txBufAddReq(uint32_t baseAddr, uint32_t bufNum);

/**
 * \brief   This API will return New Data Message Status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   newDataStatus   Rx Buffer new data status.
 *                          Refer struct #MCAN_RxNewDataStatus.
 *
 * \return  None.
 */
void  MCAN_getNewDataStatus(uint32_t              baseAddr,
                            MCAN_RxNewDataStatus *newDataStatus);

/**
 * \brief   This API clear New Data Message Status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   newDataStatus   Rx Buffer new data status.
 *                          Refer struct #MCAN_RxNewDataStatus.
 *
 * \return  None.
 */
void  MCAN_clearNewDataStatus(uint32_t                    baseAddr,
                              const MCAN_RxNewDataStatus *newDataStatus);

/**
 * \brief   This API is used to read received message form message RAM.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   memType         Part of message ram to which given message to write.
 *                          Refer enum #MCAN_MemType.
 * \param   bufNum          Buffer  number from where message is to read.
 *                          This parameter will ignored if memType is FIFO/Q.
 * \param  fifoNum          FIFOs number from where message is to read.
 *                          Refer enum #MCAN_RxFIFONum.
 *                          This parameter will ignored if memType is buffer.
 * \param   elem            Message Object.
 *                          Refer struct #MCAN_RxBufElement.
 *
 * \return  None.
 */
void MCAN_readMsgRam(uint32_t           baseAddr,
                     uint32_t           memType,
                     uint32_t           bufNum,
                     uint32_t           fifoNum,
                     MCAN_RxBufElement *elem);

/**
 * \brief   This API is used to add Standard Message ID Filter Element.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   filtNum         Filter number.
 * \param   elem            Filter Object.
 *                          Refer struct #MCAN_StdMsgIDFilterElement.
 *
 * \return  None.
 */
void MCAN_addStdMsgIDFilter(uint32_t                          baseAddr,
                            uint32_t                          filtNum,
                            const MCAN_StdMsgIDFilterElement *elem);

/**
 * \brief   This API is used to add Extended Message ID Filter Element.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   filtNum         Filter  number.
 * \param   elem            Filter Object.
 *                          Refer struct #MCAN_ExtMsgIDFilterElement.
 *
 * \return  None.
 */
void MCAN_addExtMsgIDFilter(uint32_t                          baseAddr,
                            uint32_t                          filtNum,
                            const MCAN_ExtMsgIDFilterElement *elem);

/**
 * \brief   This API will enable/disable Loop Back Test Mode for
 *          MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   lpbkMode        Loopback mode for MCAN.
 *                          Refer enum #CANFD_MCANLoopBackMode.
 * \param   enable          Loop Back Mode is enabled if it is TRUE.
 *                          Loop Back Mode is disabled  if it is FALSE.
 *
 * \return  None.
 * \note    This API can be called only when MCAN is in Software
 *          Initialization mode of operation.
 */
void MCAN_lpbkModeEnable(uint32_t baseAddr,
                         uint32_t lpbkMode,
                         uint32_t enable);

/**
 * \brief   This API will return error counter status for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   errCounter      Error Counter Status.
 *                          Refer struct #CANFD_MCANErrCntStatus.
 *
 * \return  None.
 */
void  MCAN_getErrCounters(uint32_t                  baseAddr,
                          CANFD_MCANErrCntStatus    *errCounter);

/**
 * \brief   This API will return protocol status for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   protStatus      Protocol Status.
 *                          Refer struct #CANFD_MCANProtocolStatus.
 *
 * \return  None.
 */
void  MCAN_getProtocolStatus(uint32_t                   baseAddr,
                             CANFD_MCANProtocolStatus   *protStatus);

/**
 * \brief   This API is used to enable/disable interrupts.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   intrMask        Interrupts to enable.
 *                          Refer enum #MCAN_IntrSrc.
 * \param   enable          Interrupt is enabled if it is TRUE.
 *                          Interrupt is disabled  if it is FALSE.
 *
 * \return  None.
 */
void MCAN_enableIntr(uint32_t baseAddr, uint32_t intrMask, uint32_t enable);

/**
 * \brief   This API is used to select interrupt line.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   intrMask        Interrupt Number for which interrupt
 *                          line is to be selected. Refer enum #MCAN_IntrSrc.
 * \param   lineNum         Interrupt Line to select.
 *                          Refer enum #MCAN_IntrLineNum,
 *
 * \return  None.
 */
void MCAN_selectIntrLine(uint32_t baseAddr,
                         uint32_t intrMask,
                         uint32_t lineNum);

/**
 * \brief   This API is used to enable/disable selected interrupt line.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   lineNum         Interrupt Line to select.
 *                          Refer enum #MCAN_IntrLineNum,
 * \param   enable          Interrupt Line is enabled if it is 1.
 *                          Interrupt Line is disabled  if it is 0.
 *
 * \return  None.
 */
void MCAN_enableIntrLine(uint32_t baseAddr,
                         uint32_t lineNum,
                         uint32_t enable);

/**
 * \brief   This API will return interrupt status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  status          Interrupt Status.
 */
uint32_t MCAN_getIntrStatus(uint32_t baseAddr);

/**
 * \brief   This API is used to clear the interrupt status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   intrMask        Interrupts to clear status.
 *
 * \return  None.
 */
void MCAN_clearIntrStatus(uint32_t baseAddr, uint32_t intrMask);

/**
 * \brief   This API will return High Priority Message Status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   hpm             High Priority Message Status.
 *                          Refer struct #MCAN_HighPriorityMsgInfo.
 *
 * \return  None.
 */
void  MCAN_getHighPriorityMsgStatus(uint32_t                  baseAddr,
                                    MCAN_HighPriorityMsgInfo *hpm);

/**
 * \brief   This API will Rx FIFO status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   fifoStatus      Rx FIFO Status.
 *                          Refer struct #MCAN_RxFIFOStatus.
 *
 * \return  None.
 */
void MCAN_getRxFIFOStatus(uint32_t           baseAddr,
                          MCAN_RxFIFOStatus *fifoStatus);

/**
 * \brief   This API will write Rx FIFO Acknowledgement.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *                          Refer enum #MCAN_RxFIFONum.
 * \param   fifoNum         FIFO Number.
 * \param   idx             Rx FIFO Acknowledge Index
 *
 * \return  status          Acknowledgement Status.
 */
int32_t MCAN_writeRxFIFOAck(uint32_t baseAddr,
                            uint32_t fifoNum,
                            uint32_t idx);

/**
 * \brief   This API will Tx FIFO status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   fifoStatus      Tx FIFO Status.
 *                          Refer struct #MCAN_TxFIFOStatus.
 *
 * \return  None.
 */
void MCAN_getTxFIFOQueStatus(uint32_t           baseAddr,
                             MCAN_TxFIFOStatus *fifoStatus);

/**
 * \brief   This API will return Tx Buffer Request Pending status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  status          Tx Buffer Request Pending status.
 */
uint32_t MCAN_getTxBufReqPend(uint32_t baseAddr);

/**
 * \brief   This API will set Tx Buffer Cancellation Request.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   buffNum         Tx Buffer number for which request is to be added.
 *
 * \return  status          Configuration Status.
 */
int32_t MCAN_txBufCancellationReq(uint32_t baseAddr, uint32_t buffNum);

/**
 * \brief   This API will return Tx Buffer Transmission Occurred status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  status          Tx Buffer Transmission Occurred status.
 */
uint32_t MCAN_getTxBufTransmissionStatus(uint32_t baseAddr);

/**
 * \brief   This API will return Transmit Buffer Cancellation Finished status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  status          Transmit Buffer Cancellation Finished status.
 */
uint32_t MCAN_txBufCancellationStatus(uint32_t baseAddr);

/**
 * \brief   This API is used to enable/disable Tx Buffer Transmission Interrupt.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   bufNum          Buffer number for which interrupt is to enable.
 * \param   enable          Interrupt is enabled if it is TRUE.
 *                          Interrupt is disabled  if it is FALSE.
 *
 * \return  status          Configuration status.
 */
int32_t MCAN_txBufTransIntrEnable(uint32_t baseAddr,
                                  uint32_t bufNum,
                                  uint32_t enable);

/**
 * \brief   This API is used to enable/disable Tx Buffer Cancellation Finished
 *          Interrupt.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   bufNum         Buffer number for which interrupt is to enable.
 * \param   enable          Interrupt is enabled if it is TRUE.
 *                          Interrupt is disabled  if it is FALSE.
 *
 * \return  status          Configuration status.
 */
int32_t MCAN_getTxBufCancellationIntrEnable(uint32_t baseAddr,
                                            uint32_t bufNum,
                                            uint32_t enable);

/**
 * \brief   This API add clock stop request for MCAN module to put it in
 *          power down mode.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   enable          Add CLock Stop Request.
 *                          Adds Clock Clock stop Request is TRUE otherwise
 *                          removes it.
 *
 * \return  None.
 */
void MCAN_addClockStopRequest(uint32_t baseAddr, uint32_t enable);

/**
 * \brief   This API will Tx Event FIFO status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   fifoStatus      Tx Event FIFO Status.
 *                          Refer struct #MCAN_TxEventFIFOStatus.
 *
 * \return  None.
 */
void MCAN_getTxEventFIFOStatus(uint32_t                baseAddr,
                               MCAN_TxEventFIFOStatus *fifoStatus);

/**
 * \brief   This API will write Event FIFO Acknowledge Index.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   idx             Event FIFO Acknowledge Index
 *
 * \return  status          Acknowledgement Status.
 */
int32_t MCAN_writeTxEventFIFOAck(uint32_t baseAddr, uint32_t idx);

/**
 * \brief   This API will Force Error on ECC.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   eccErr          Force Error on ECC configuration.
 *                          Refer struct #CANFD_MCANECCErrForceParams.
 *
 * \return  status          configuration status.
 */
void MCAN_eccForceError(uint32_t                            baseAddr,
                        const CANFD_MCANECCErrForceParams   *eccErr);

/**
 * \brief   This API will return ECC Error status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   eccErr          ECC error status.
 *                          Refer struct #CANFD_MCANECCErrStatus.
 *
 * \return  None.
 */
void MCAN_eccGetErrorStatus(uint32_t                baseAddr,
                            CANFD_MCANECCErrStatus  *eccErr);

/**
 * \brief   This API is used to clear the ECC Error status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   errType         Error type to clear status.
 *                          Refer enum #CANFD_MCANECCErrType.
 *
 * \return  None.
 */
void MCAN_eccClearErrorStatus(uint32_t baseAddr, uint32_t errType);

/**
 * \brief   This API is used to write End of Interrupt for ECC interrupt.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   errType         Interrupt to enable.
 *                          Refer enum #CANFD_MCANECCErrType.
 *
 * \return  None.
 */
void MCAN_eccWriteEOI(uint32_t baseAddr, uint32_t errType);

/**
 * \brief   This API is used to enable ECC interrupt.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   errType         Interrupt to enable.
 *                          Refer enum #CANFD_MCANECCErrType.
 * \param   enable          ECC Interrupt is enabled if it is TRUE.
 *                          ECC Interrupt is disabled  if it is FALSE.
 *
 * \return  None.
 */
void MCAN_eccEnableIntr(uint32_t baseAddr, uint32_t errType, uint32_t enable);

/**
 * \brief   This API is used to get ECC interrupt status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   errType         Interrupt status to read.
 *                          Refer enum #CANFD_MCANECCErrType.
 *
 * \return  None.
 */
uint32_t MCAN_eccGetIntrStatus(uint32_t baseAddr, uint32_t errType);

/**
 * \brief   This API is used to clear ECC interrupt status.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   errType         Interrupt status to clear.
 *                          Refer enum #CANFD_MCANECCErrType.
 *
 * \return  None.
 */
void MCAN_eccClearIntrStatus(uint32_t baseAddr, uint32_t errType);

/**
 * \brief   This API will configure external timestamp counter for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   prescalar       Timestamp Counter Prescaler. Range:[0x0-0xFFFFFF]
 *
 * \return  None.
 *
 * \note    Actual value programmed prescalar values is (prescalar - 1).
 */
void MCAN_extTSCounterConfig(uint32_t baseAddr,
                             uint32_t prescalar);

/**
 * \brief   This API will enable/disable fast external time stamp counter for
 *          MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   enable          External TS is enabled if it is 1.
 *                          External TS is disabled  if it is 0.
 *
 * \return  None.
 */
void MCAN_extTSCounterEnable(uint32_t baseAddr, uint32_t enable);

/**
 * \brief   This API will enable/disable External TimeStamp Counter
 *          Overflow Interrupt for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   enable          External TimeStamp Counter Overflow Interrupt is
 *                          enabled if it is TRUE.
 *                          External TimeStamp Counter Overflow Interrupt is
 *                          disabled  if it is FALSE.
 *
 * \return  None.
 */
void MCAN_extTSEnableIntr(uint32_t baseAddr, uint32_t enable);

/**
 * \brief   This API is used to write End of Interrupt for External TimeStamp
 *          Counter Overflow Interrupt.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  None.
 */
void MCAN_extTSWriteEOI(uint32_t baseAddr);

/**
 * \brief   This API returns Number of unserviced rollover/overflow
 *          interrupts for external TimeStamp counter.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  status          Returns Number of unserviced rollover/overflow
 *                          interrupts for external TimeStamp counter.
 *                          Maximum number of unserviced interrupts is 0xF.
 */
uint32_t MCAN_extTSGetUnservicedIntrCount(uint32_t baseAddr);

/* ========================================================================== */
/*                          Advance Functions                                 */
/* ========================================================================== */

/**
 * \brief   This API is used get the MCAN revision ID.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   revId           Contains Revision ID of MCAN module.
 *                          Refer struct #MCAN_RevisionId.
 *
 * \return  None.
 */
void MCAN_getRevisionId(uint32_t baseAddr, MCAN_RevisionId *revId);

/**
 * \brief   This API get clock stop acknowledgement for MCAN module.
 *          It return whether MCAN is power down mode or not.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  status          Return Clock Stop Acknowledgement status.
 *                          Return '1' if M_CAN is set in power down mode else
 *                          returns '0'.
 */
uint32_t MCAN_getClockStopAck(uint32_t baseAddr);

/**
 * \brief   This API will set External TimeStamp Counter Overflow Interrupt
 *          Raw status for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  None.
 */
void MCAN_extTSSetRawStatus(uint32_t baseAddr);

/**
 * \brief   This API will clear External TimeStamp Counter Overflow Interrupt
 *          raw status for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  None.
 */
void MCAN_extTSClearRawStatus(uint32_t baseAddr);

/**
 * \brief   This API will return Rx pin state of MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  state           MCAN Rx Pin State.
 *                          0 - The CAN bus is dominant
 *                          1 - The CAN bus is recessive
 */
uint32_t MCAN_getRxPinState(uint32_t baseAddr);

/**
 * \brief   This API will set Tx pin state of MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   state           MCAN Tx Pin State.
 *                          00 - Reset value
 *                          01 - Sample Point can be monitored at tx pin
 *                          10 - The CAN bus is dominant
 *                          11 - The CAN bus is recessive
 *                          others - It will treated as 11.
 *
 * \return  None.
 */
void MCAN_setTxPinState(uint32_t baseAddr, uint32_t state);

/**
 * \brief   This API will return Tx pin state of MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  state           MCAN Tx Pin State.
 *                          00 - Reset value
 *                          01 - Sample Point can be monitored at tx pin
 *                          10 - The CAN bus is dominant
 *                          11 - The CAN bus is recessive
 */
uint32_t MCAN_getTxPinState(uint32_t baseAddr);

/**
 * \brief   This API will return current timestamp counter value.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  val             Current Timestamp counter value.
 */
uint32_t MCAN_getTSCounterVal(uint32_t baseAddr);

/**
 * \brief   This API will return clock stop acknowledgement
 *          for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  ack             Clock Stop Acknowledge
 *                          0 - No clock stop acknowledged
 *                          1 - M_CAN may be set in power down
 */
uint32_t MCAN_getClkStopAck(uint32_t baseAddr);

/**
 * \brief   This API will get the configured bit timings for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   configParams    Configuration parameters for MCAN bit timing.
 *                          Refer struct #MCAN_BitTimingParams.
 *
 * \return  None.
 */
void MCAN_getBitTime(uint32_t              baseAddr,
                     MCAN_BitTimingParams *configParams);

/**
 * \brief   This API will reset timestamp counter value.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  None.
 */
void MCAN_resetTSCounter(uint32_t baseAddr);

/**
 * \brief   This API will return current time-out counter value.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  val             Current Time-out counter value.
 */
uint32_t MCAN_getTOCounterVal(uint32_t baseAddr);

/**
 * \brief   This API is used get the ECC AGGR revision ID.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   revId           Contains Revision ID of ECC AGGR.
 *                          Refer struct #MCAN_ECCAggrRevisionId.
 *
 * \return  None.
 */
void MCAN_eccAggrGetRevisionId(uint32_t                baseAddr,
                               MCAN_ECCAggrRevisionId *revId);

/**
 * \brief   This API is used get the ECC Wrapper revision ID.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 * \param   revId           Contains Revision ID of ECC Wrapper
 *                          Refer struct #MCAN_ECCWrapRevisionId.
 *
 * \return  None.
 */
void MCAN_eccWrapGetRevisionId(uint32_t                baseAddr,
                               MCAN_ECCWrapRevisionId *revId);

/**
 * \brief   This API returns External TimeStamp Counter Overflow Interrupt
 *          enable status for MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  status          Returns TRUE if External TimeStamp Counter Overflow
 *                          Interrupt is enabled.
 *                          Else returns FALSE.
 */
uint32_t MCAN_extTSIsIntrEnable(uint32_t baseAddr);

/**
 * \brief   This function return endianness value of MCAN module.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \retval  val             Endianness value. (0x87654321)
 */
uint32_t MCAN_getEndianVal(uint32_t baseAddr);

/**
 * \brief   This API will get the configured Extended ID AND Mask.
 *
 * \param   baseAddr        Base Address of the MCAN Registers.
 *
 * \return  idMask          Extended ID AND Mask.
 */
uint32_t MCAN_getExtIDANDMassk(uint32_t baseAddr);

/** @}*/
#ifdef __cplusplus
}
#endif

#endif /* CANFD_INTERNAL_H */
