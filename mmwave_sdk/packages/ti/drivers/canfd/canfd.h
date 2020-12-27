/**
 *   @file  canfd.h
 *
 *   @brief
 *      This is the header file for the CANFD driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the CANFD driver.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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

/** @mainpage CANFD Driver
 *
 *  The CANFD driver provides functionality of transferring data between CANFD peripherals.
 *  This driver does not interpret any of the data sent to or received from using this peripheral.
 *
 *
 *  The CANFD header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/canfd/canfd.h>
 *   @endcode
 *
 *  ## Initializing the driver #
 *  The CANFD Driver needs to be initialized once across the System. This is
 *  done using the #CANFD_init. None of the CANFD API's can be used without invoking
 *  this API.
 *
 *  Once the CANFD Driver has been initialized; the bit timing can be configured using #CANFD_configBitTime.
 *  This APIs can be called multiple times to reconfigure bit timings.
 *
 * ## Creating the message objects #
 *  Message objects are used to transmit or receive data over the CANFD peripheral. A message object is created
 *  using #CANFD_createMsgObject.
 *
 * ## Sending and receiving data #
 *  Data is transmitted using the #CANFD_transmitData. The application will be notified when the transmit is
 *  complete if it has enabled dataInterruptEnable and registered a callback function appDataCallBack when
 *  initializing the CANFD driver.
 *
 *  If the receive interrupts are enabled using dataInterruptEnable field and a callback function appCallBack
 *  has been registered when initializing the CANFD driver, the driver notifies the application
 *  when the data has arrived. The application needs to call the #CANFD_getData function to read the received data.

 *
 * ## Error and status handling #
 *  The application can monitor the ECC error, Bus off error and Protocol Errors by enabling the error interrupts errInterruptEnable.
 *  The driver will call the registered callback function appErrCallBack to indicate which error fields caused the interrupt.
 *  It is up to the application to take appropriate action.
 *
 * ## Get/Set Options #
 *  Helper APIs to get and set various statistics, error counters, ECC diagnostics, power down have been provided.
 *  Refer to #CANFD_Option for more information.
 *
 * ## Limitation #
 *  The CANFD driver does not support the DMA or power down.
 *
 *  The canfd/include/hw_mcanss.h has the register layer definitons for the CANFD controller module.
 *
 */

/** @defgroup CANFD_DRIVER      CANFD Driver
 */

#ifndef CANFD_H_
#define CANFD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>
#include <ti/drivers/canfd/include/hw_mcanss.h>

/**
@defgroup CANFD_DRIVER_EXTERNAL_FUNCTION     CANFD Driver External Functions
@ingroup CANFD_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the CANFD driver
*/

/**
@defgroup CANFD_DRIVER_EXTERNAL_DATA_STRUCTURE      CANFD Driver External Data Structures
@ingroup CANFD_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/

/**
@defgroup CANFD_DRIVER_ERROR_CODE            CANFD Driver Error code
@ingroup CANFD_DRIVER
@brief
*   The section has a list of all the error codes which are returned to the application.
*   Base error code for the CANFD module is defined in the
*  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   No Error
 */
#define CANFD_EOK                       (0)

/**
 * @brief   Error Code: Invalid argument
 */
#define CANFD_EINVAL                    (MMWAVE_ERRNO_CANFD_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define CANFD_EINUSE                    (MMWAVE_ERRNO_CANFD_BASE-2)

/**
 * @brief   Error Code: Operation is not implemented.
 */
#define CANFD_ENOTIMPL                  (MMWAVE_ERRNO_CANFD_BASE-3)

/**
 * @brief   Error Code: Out of memory
 */
#define CANFD_ENOMEM                    (MMWAVE_ERRNO_CANFD_BASE-4)
/** @}*/


/** @addtogroup CANFD_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief      CANFD module handle returned by the CANFD_init() API call.
 */
typedef void* CANFD_Handle;

/*!
 *  @brief      CANFD message object handle returned by the CANFD_createMsgObject() API call.
 */
typedef void* CANFD_MsgObjHandle;

/*!
 *  @brief    Enumerates the values used to represent the MCAN mode of operation
 */
typedef enum CANFD_MCANOperationMode_t
{
    /*! MCAN normal mode */
    CANFD_MCANOperationMode_NORMAL = 0U,

    /*! MCAN SW initialization mode */
    CANFD_MCANOperationMode_SW_INIT = 1U
}CANFD_MCANOperationMode;

/*!
 *  @brief    This enumeration defines the values used to set the direction of message object
 */
typedef enum CANFD_Direction_t
{
    /*! Message object used to receive data */
    CANFD_Direction_RX,

    /*! Message object used to transmit data */
    CANFD_Direction_TX
} CANFD_Direction;

/*!
 *  @brief    This enumeration defines the values used to represent the CAN Identifier Type
 */
typedef enum CANFD_MCANXidType_t
{
    /*! 11bit MCAN Identifier */
    CANFD_MCANXidType_11_BIT,

    /*! 29bit MCAN Identifier */
    CANFD_MCANXidType_29_BIT
} CANFD_MCANXidType;

/*!
 *  @brief    This enumeration defines the CAN frame type
 */
typedef enum CANFD_MCANFrameType_t
{
    /*! Classic Frame */
    CANFD_MCANFrameType_CLASSIC,

    /*! FD Frame */
    CANFD_MCANFrameType_FD
} CANFD_MCANFrameType;

/**
 *  @brief    This enumeration defines the MCAN timeout counter configuration
 */
typedef enum CANFD_MCANTimeOutSelect_t
{
    /*! Continuous operation Mode */
    CANFD_MCANTimeOutSelect_CONT = 0U,

    /*! Timeout controlled by Tx Event FIFO */
    CANFD_MCANTimeOutSelect_TX_EVENT_FIFO = 1U,

    /*! Timeout controlled by Rx FIFO 0 */
    CANFD_MCANTimeOutSelect_RX_FIFO0 = 2U,

    /*! Timeout controlled by Rx FIFO 1 */
    CANFD_MCANTimeOutSelect_RX_FIFO1 = 3U
}CANFD_MCANTimeOutSelect;

/**
 *  @brief    This enumeration defines the MCAN ECC Error Types
 */
typedef enum CANFD_MCANECCErrType_t
{
    /*! ECC Single Error Correction */
    CANFD_MCANECCErrType_SEC = 0U,

    /*! ECC Single Error Detection */
    CANFD_MCANECCErrType_DED = 1U
}CANFD_MCANECCErrType;

/**
 *  @brief    This enumeration defines the MCAN Loopback mode
 */
typedef enum CANFD_MCANLoopBackMode_t
{
    /*! This mode can be used for hot self-test and this mode will not affect bus state. */
    CANFD_MCANLoopBackMode_INTERNAL = 0U,

    /*! In this mode, MCAN the MCAN treats its own transmitted messages as received messages
     *  and stores them (if they pass acceptance filtering) into an Rx Buffer or an Rx FIFO.
     *  This mode will affect bus state.
     */
    CANFD_MCANLoopBackMode_EXTERNAL = 1U
}CANFD_MCANLoopBackMode;

/**
 *  @brief    This enumeration defines the MCAN's communication state
 */
typedef enum CANFD_MCANCommState_t
{
    /*! MCAN is synchronizing on CANFD communication */
    CANFD_MCANCommState_SYNCHRONIZING = 0U,

    /*! MCAN is neither receiver nor transmitter */
    CANFD_MCANCommState_IDLE = 1U,

    /*! MCAN is operating as receiver */
    CANFD_MCANCommState_RECEIVER = 2U,

    /*! MCAN is operating as transmitter */
    CANFD_MCANCommState_TRANSMITTER = 3U
}CANFD_MCANCommState;

/**
 *  @brief    This enumeration defines the  MCAN's Error Code
 */
typedef enum CANFD_MCANErrCode_t
{
    /*! No error occurred since LEC has been reset by
     *  successful reception or transmission.
     */
    CANFD_MCANErrCode_NO_ERROR = 0U,

    /*! More than 5 equal bits in a sequence have occurred in a part of
     *  a received message where this is not allowed.
     */
    CANFD_MCANErrCode_STUFF_ERROR = 1U,

    /*! A fixed format part of a received frame has the wrong format. */
    CANFD_MCANErrCode_FORM_ERROR = 2U,

    /*! The message transmitted by the MCAN was not acknowledged by another node. */
    CANFD_MCANErrCode_ACK_ERROR = 3U,

    /*! During the transmission of a message (with the exception of
     *  the arbitration field), the device wanted to send a
     *  recessive level (bit of logical value 1),
     *  but the monitored bus value was dominant.
     */
    CANFD_MCANErrCode_BIT1_ERROR = 4U,

    /*! During the transmission of a message (or acknowledge bit,
     *  or active error flag, or overload flag), the device wanted to send
     *  a dominant level (data or identifier bit logical value 0),
     *  but the monitored bus value was recessive. During Bus_Off recovery
     *  this status is set each time a sequence of 11 recessive bits has been
     *  monitored. This enables the CPU to monitor the proceeding of
     *  the Bus_Off recovery sequence (indicating the bus is not stuck at
     *  dominant or continuously disturbed).
     */
    CANFD_MCANErrCode_BIT0_ERROR = 5U,

    /*! The CRC check sum of a received message was incorrect.
     *   The CRC of an incoming message does not match with the
     *   CRC calculated from the received data.
     */
    CANFD_MCANErrCode_CRC_ERROR = 6U,

    /*! Any read access to the Protocol Status Register re-initializes the LEC to 7.
     * When the LEC shows the value 7, no CANFD bus event was detected since the last
     * CPU read access to the Protocol Status Register.
     */
    CANFD_MCANErrCode_NO_CHANGE = 7U
}CANFD_MCANErrCode;

/**
 *  @brief  This enumeration describes a list of all the reasons for which the driver will
 *          invoke application callback functions.
 */
typedef enum CANFD_Reason_t
{
    /**
     * @brief  Data has been received and the application is required to read and process the data.
     */
    CANFD_Reason_RX                         = 0x1,

    /**
     * @brief  Data has been succesfully transmitted.
     */
    CANFD_Reason_TX_COMPLETION              = 0x2,

    /**
     * @brief  Data transmission is succesfully canceled.
     */
    CANFD_Reason_TX_CANCELED                = 0x3,

    /**
     * @brief  Data has been succesfully transmitted.
     */
    CANFD_Reason_ECC_ERROR                  = 0x4,

    /**
     * @brief  Bus Off condition detected.
     */
    CANFD_Reason_BUSOFF                     = 0x5,

    /**
     * @brief  Protocol error in data phase detected.
     */
    CANFD_Reason_PROTOCOL_ERR_DATA_PHASE    = 0x6,

    /**
     * @brief  Protocol error in arbitration phase detected.
     */
    CANFD_Reason_PROTOCOL_ERR_ARB_PHASE     = 0x7
}CANFD_Reason;

/*!
 *  @brief    This enumeration defines the values used to represent the GET/SET options
 *
 *  @sa
 *  CANFD_OptionTLV
 */
typedef enum CANFD_Option_t
{
    /*! Used to get the MCAN Tx and Rx error counters
     * @sa CANFD_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CANFD_MCANErrCntStatus) for this option.
     */
    CANFD_Option_MCAN_ERROR_COUNTER,

    /*! Used to get the MCAN protocol status
     * @sa CANFD_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CANFD_MCANProtocolStatus) for this option.
     */
    CANFD_Option_MCAN_PROTOCOL_STATUS,

    /*! Used to get the MCAN message object software maintained statistics
     * @sa CANFD_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CANFD_MCANMsgObjectStats) for this option.
     *      Application must fill in the message object handle for which the statistics is requested.
     *
     */
    CANFD_Option_MCAN_MSG_OBJECT_STATS,

    /*! Used to put the MCAN module in init or operational state
     * @sa CANFD_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option.
     * Valid values: Refer to (CANFD_MCANOperationMode)
     */
    CANFD_Option_MCAN_MODE,

    /*! Used to enable or disable internal/external loopback mode
     * @sa CANFD_setOptions
     *
     * NOTE: The length in the TLV should be sizeof(CANFD_MCANLoopbackCfgParams) for this option.
     *
     */
    CANFD_Option_MCAN_LOOPBACK,

    /*! Used to request a local power down or wakeup from a local power down
     * @sa CANFD_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option.
     * Valid values are
     *  1   -   MCAN Sleep
     *  0   -   MCAN Wakeup
     *
     */
    CANFD_Option_MCAN_POWER_DOWN
} CANFD_Option;

/**
 * @brief  Data structure defines the MCAN Loopback parameters.
 */
typedef struct CANFD_MCANLoopbackCfgParams_t
{
    /*! Enable or disable loopback mode
     * Valid values are
     *  0   -   Disable
     *  1   -   Enable
     */
    uint32_t                enable;

    /*! Loopback mode: Internal or External */
    CANFD_MCANLoopBackMode  mode;
}CANFD_MCANLoopbackCfgParams;

/**
 * @brief   Data structure defines the parameters for bit timing calculation.
 *          Bit timing related to data phase will be valid only in case where
 *          MCAN is put in CANFD mode and will be '0' otherwise.
 */
typedef struct CANFD_MCANBitTimingParams_t
{
    /*! Nominal Baud Rate Pre-scaler */
    uint32_t        nomBrp;

    /*! NominalProp Segment value */
    uint32_t        nomPropSeg;

    /*! NominalPhase Segment1 value */
    uint32_t        nomPseg1;

    /*! NominalPhase Segment2 value */
    uint32_t        nomPseg2;

    /*! Nominal (Re)Synchronization Jump Width */
    uint32_t        nomSjw;

    /*! Nominal Baud Rate Pre-scaler */
    uint32_t        dataBrp;

    /*! NominalProp Segment value */
    uint32_t        dataPropSeg;

    /*! NominalPhase Segment1 value */
    uint32_t        dataPseg1;

    /*! NominalPhase Segment2 value */
    uint32_t        dataPseg2;

    /*! Nominal (Re)Synchronization Jump Width */
    uint32_t        dataSjw;
}CANFD_MCANBitTimingParams;

/**
 * @brief  Data structure defines the MCAN Transmitter Delay Compensation parameters.
 */
typedef struct CANFD_MCANTdcConfig_t
{
    /*! Transmitter Delay Compensation Filter Window Length
     *   Range: [0x0-0x7F]
     */
    uint32_t        tdcf;

    /*! Transmitter Delay Compensation Offset
     *   Range: [0x0-0x7F]
     */
    uint32_t        tdco;
}CANFD_MCANTdcConfig;

/**
 * @brief   Data structure defines the MCAN Global Filter Configuration parameters.
 */
typedef struct CANFD_MCANGlobalFiltConfig_t
{
    /*! Reject Remote Frames Extended
     *   0 = Filter remote frames with 29-bit extended IDs
     *   1 = Reject all remote frames with 29-bit extended IDs
     */
    uint32_t        rrfe;

    /*! Reject Remote Frames Standard
     *   0 = Filter remote frames with 11-bit standard IDs
     *   1 = Reject all remote frames with 11-bit standard IDs
     */
    uint32_t        rrfs;

    /*! Accept Non-matching Frames Extended
     *   0 = Accept in Rx FIFO 0
     *   1 = Accept in Rx FIFO 1
     *   others = Reject
     */
    uint32_t        anfe;

    /*! Accept Non-matching Frames Standard
     *   0 = Accept in Rx FIFO 0
     *   1 = Accept in Rx FIFO 1
     *   others = Reject
     */
    uint32_t        anfs;
}CANFD_MCANGlobalFiltConfig;

/**
 * @brief   Data structure defines the MCAN Message RAM Configuration Parameters.
 *          Message RAM can contain following sections:
 *          Standard ID filters, Extended ID filters, TX FIFO(or TX Q),
 *          TX Buffers, TX EventFIFO, RX FIFO0, RX FIFO1, RX Buffer.
 *          Note: If particular section in the RAM is not used then it's size
 *          should be initialized to '0'
 *          (Number of buffers in case of Tx/Rx buffer).
 */
typedef struct CANFD_MCANMsgRAMCfgParams_t
{
    /*! List Size: Standard ID
     *   0 = No standard Message ID filter
     *   1-127 = Number of standard Message ID filter elements
     *   others = Values greater than 128 are interpreted as 128
     */
    uint32_t            lss;

    /*! List Size: Extended ID
     *   0 = No standard Message ID filter
     *   1-64 = Number of standard Message ID filter elements
     *   others = Values greater than 64 are interpreted as 64
     */
    uint32_t            lse;

    /*! Number of Dedicated Transmit Buffers
     *   0 = No Dedicated Tx Buffers
     *   1-32 = Number of Dedicated Tx Buffers
     *   others = Values greater than 32 are interpreted as 32
     */
    uint32_t            txBufNum;

    /*! Transmit FIFO/Queue Size
     *   0 = No Tx FIFO/Queue
     *   1-32 = Number of Tx Buffers used for Tx FIFO/Queue
     *   others = Values greater than 32 are interpreted as 32
     */
    uint32_t            txFIFOSize;

    /*! Tx FIFO/Queue Mode
     *   0 = Tx FIFO operation
     *   1 = Tx Queue operation
     */
    uint32_t            txBufMode;

    /*! Event FIFO Size
     *   0 = Tx Event FIFO disabled
     *   1-32 = Number of Tx Event FIFO elements
     *   others = Values greater than 32 are interpreted as 32
     */
    uint32_t            txEventFIFOSize;

    /*! Tx Event FIFO Watermark
     *   0 = Watermark interrupt disabled
     *   1-32 = Level for Tx Event FIFO watermark interrupt
     *   others = Watermark interrupt disabled
     */
    uint32_t            txEventFIFOWaterMark;

    /*! Rx FIFO0 Size
     *   0 = No Rx FIFO
     *   1-64 = Number of Rx FIFO elements
     *   others = Values greater than 64 are interpreted as 64
     */
    uint32_t            rxFIFO0size;

    /*! Rx FIFO0 Watermark
     *   0 = Watermark interrupt disabled
     *   1-63 = Level for Rx FIFO 0 watermark interrupt
     *   others = Watermark interrupt disabled
     */
    uint32_t            rxFIFO0waterMark;

    /*! Rx FIFO0 Operation Mode
     *   0 = FIFO blocking mode
     *   1 = FIFO overwrite mode
     */
    uint32_t            rxFIFO0OpMode;

    /*! Rx FIFO1 Size
     *   0 = No Rx FIFO
     *   1-64 = Number of Rx FIFO elements
     *   others = Values greater than 64 are interpreted as 64
     */
    uint32_t            rxFIFO1size;

    /*! Rx FIFO1 Watermark
     *   0 = Watermark interrupt disabled
     *   1-63 = Level for Rx FIFO 1 watermark interrupt
     *   others = Watermark interrupt disabled
     */
    uint32_t            rxFIFO1waterMark;

    /*! Rx FIFO1 Operation Mode
     *   0 = FIFO blocking mode
     *   1 = FIFO overwrite mode
     */
    uint32_t            rxFIFO1OpMode;
}CANFD_MCANMsgRAMCfgParams;

/**
 * @brief   Data structure defines the MCAN ECC configuration parameters.
 */
typedef struct CANFD_MCANECCConfigParams_t
{
    /*! Enable/disable ECC
     *   0 = Disable ECC
     *   1 = Enable ECC
     */
    uint32_t            enable;

    /*! Enable/disable ECC Check
     *   0 = Disable ECC Check
     *   1 = Enable ECC Check
     */
    uint32_t            enableChk;

    /*! Enable/disable Read Modify Write operation
     *   0 = Disable Read Modify Write operation
     *   1 = Enable Read Modify Write operation
     */
    uint32_t            enableRdModWr;
}CANFD_MCANECCConfigParams;

/**
 * @brief   Data structure defines the MCAN error logging counters status.
 */
typedef struct CANFD_MCANErrCntStatus_t
{
    /*! Transmit Error Counter */
    uint32_t            transErrLogCnt;

    /*! Receive Error Counter */
    uint32_t            recErrCnt;

    /*! Receive Error Passive
     *   0 = The Receive Error Counter is below the error passive level(128)
     *   1 = The Receive Error Counter has reached the error passive level(128)
     */
    uint32_t            rpStatus;

    /*! CAN Error Logging */
    uint32_t            canErrLogCnt;
}CANFD_MCANErrCntStatus;

/**
 * @brief   Data structure defines the MCAN protocol status.
 */
typedef struct CANFD_MCANProtocolStatus_t
{
    /*! Last Error Code
     *   Refer enum #CANFD_MCANErrCode
     */
    uint32_t                lastErrCode;

    /*! Activity - Monitors the module's CAN communication state.
     *   refer enum #CANFD_MCANCommState
     */
    uint32_t                act;

    /*! Error Passive
     *   0 = The M_CAN is in the Error_Active state
     *   1 = The M_CAN is in the Error_Passive state
     */
    uint32_t                errPassive;

    /*! Warning Status
     *   0 = Both error counters are below the Error_Warning limit of 96
     *   1 = At least one of error counter has reached the Error_Warning
     *       limit of 96
     */
    uint32_t                warningStatus;

    /*! Bus_Off Status
     *   0 = The M_CAN is not Bus_Off
     *   1 = The M_CAN is in Bus_Off state
     */
    uint32_t                busOffStatus;

    /*! Data Phase Last Error Code
     *   Refer enum #CANFD_MCANErrCode
     */
    uint32_t                dlec;

    /*! ESI flag of last received CAN FD Message
     *   0 = Last received CAN FD message did not have its ESI flag set
     *   1 = Last received CAN FD message had its ESI flag set
     */
    uint32_t                resi;

    /*! BRS flag of last received CAN FD Message
     *   0 = Last received CAN FD message did not have its BRS flag set
     *   1 = TLast received CAN FD message had its BRS flag set
     */
    uint32_t                rbrs;

    /*! Received a CAN FD Message
     *   0 = Since this bit was reset by the CPU, no CAN FD message has been
     *       received
     *   1 = Message in CAN FD format with FDF flag set has been received
     */
    uint32_t                rfdf;

    /*! Protocol Exception Event
     *   0 = No protocol exception event occurred since last read access
     *   1 = Protocol exception event occurred
     */
    uint32_t                pxe;

    /*! Transmitter Delay Compensation Value */
    uint32_t                tdcv;
}CANFD_MCANProtocolStatus;

/**
 * @brief   Data structure defines the ECC Error forcing.
 */
typedef struct CANFD_MCANECCErrForceParams_t
{
    /*! Error type to be forced
     *   Refer enum  #CANFD_MCANECCErrType.
     */
    uint32_t                errType;

    /*! Row address where error needs to be applied. */
    uint32_t                rowNum;

    /*! Column/Data bit that needs to be flipped when
     *   force_sec or force_ded is set
     */
    uint32_t                bit1;

    /*! Data bit that needs to be flipped when force_ded is set */
    uint32_t                bit2;

    /*! Force Error once
     *   1: The error will inject an error to the specified row only once
     */
    uint32_t                errOnce;

    /*! Force error on the next RAM read */
    uint32_t                errForce;
}CANFD_MCANECCErrForceParams;

/**
 * @brief   Data structure defines the ECC Error Status.
 */
typedef struct CANFD_MCANECCErrStatus_t
{
    /*! Single Bit Error Status
     *   0 = No Single Bit Error pending
     *   1 = Single Bit Error pending
     */
    uint32_t                secErr;

    /*! Double Bit Error Status
     *   0 = No Double Bit Error pending
     *   1 = Double Bit Error pending
     */
    uint32_t                dedErr;

    /*! Indicates the row/address where the single or double bit
     *   error occurred.
     */
    uint32_t                row;

    /*! Indicates the bit position in the ram data that is in error
     */
    uint32_t                bit1;

    /*! Indicates the bit position in the ram data that is in error
     *   Valid only in case of DED.
     */
    uint32_t                bit2;
}CANFD_MCANECCErrStatus;

/*!
 * @brief
 *  Response structure definition for Error and status information.
 */
typedef struct CANFD_ErrStatusResp_t
{
    union
    {
        /*! ECC Error Status. */
        CANFD_MCANECCErrStatus      eccErrStatus;

        /*! Protocol Status. */
        CANFD_MCANProtocolStatus    protocolStatus;
    }u;
} CANFD_ErrStatusResp;

/**
 * @brief
 * Application specified callback function which is invoked
 * by the CANFD driver once transmit is complete or data has been received for the
 * specified message object.
 *
 *  @param[in]  handle
 *      Message object handle for which the callback function is invoked.
 *  @param[in]  reason
 *      Cause of the interrupt which prompted the callback.
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CANFD_DataAppCallBack)(CANFD_MsgObjHandle handle, CANFD_Reason reason);

/**
 * @brief
 * Application specified callback function which is invoked
 * by the CANFD driver on error or status change.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in]  reason
 *      Cause of the interrupt which prompted the callback.
 *  @param[in]  errStatusResp
 *      Response structure populated with the value of the fields that caused the error or status interrupt.
 *      Processing of this structure is dependent on the callback reason.
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CANFD_ErrStatusAppCallBack)(CANFD_Handle handle, CANFD_Reason reason, CANFD_ErrStatusResp* errStatusResp);

/**
 * @brief   Data structure defines the MCAN initialization parameters.
 */
typedef struct CANFD_MCANInitParams_t
{
    /*! FD Operation Enable
     *   0 = FD operation disabled
     *   1 = FD operation enabled
     */
    uint32_t                    fdMode;

    /*! Bit Rate Switch Enable
     *   This is valid only when opMode = 1.
     *   0 = Bit rate switching for transmissions disabled
     *   1 = Bit rate switching for transmissions enabled
     */
    uint32_t                    brsEnable;

    /*! Transmit Pause
     *   0 = Transmit pause disabled
     *   1 = Transmit pause enabled
     */
    uint32_t                    txpEnable;

    /*! FEdge Filtering during Bus Integration
     *   0 = Edge filtering disabled
     *   1 = Two consecutive dominant tq required to detect an edge for
     *       hard synchronization
     */
    uint32_t                    efbi;

    /*! Protocol Exception Handling Disable
     *   0 = Protocol exception handling enabled
     *   1 = Protocol exception handling disabled
     */
    uint32_t                    pxhddisable;

    /*! Disable Automatic Retransmission
     *   0 = Automatic retransmission of messages not transmitted successfully
     *       enabled
     *   1 = Automatic retransmission disabled
     */
    uint32_t                    darEnable;

    /*! Wakeup Request Enable
     *   0 = Wakeup request is disabled
     *   1 = Wakeup request is enabled
     */
    uint32_t                    wkupReqEnable;

    /*! Auto-Wakeup Enable
     *   0 = Auto-Wakeup is disabled
     *   1 = Auto-Wakeup is enabled
     */
    uint32_t                    autoWkupEnable;

    /*! Emulation/Debug Suspend Enable
     *   0 = Emulation/Debug Suspend is disabled
     *   1 = Emulation/Debug Suspend is enabled
     */
    uint32_t                    emulationEnable;

    /*! Emulation/Debug Suspend Fast Ack Enable
     *   0 = Emulation/Debug Suspend does not wait for idle/immediate effect
     *   1 = Emulation/Debug Suspend waits for idle/graceful stop
     */
    uint32_t                    emulationFAck;

    /*! Clock Stop Fast Ack Enable
     *   0 = Clock Stop does not wait for idle/immediate effect
     *   1 = Clock Stop waits for idle/graceful stop
     */
    uint32_t                    clkStopFAck;

    /*! Start value of the Message RAM Watchdog Counter
     *   Range:[0x0-0xFF]
     */
    uint32_t                    wdcPreload;

    /*! Transmitter Delay Compensation Enable
     *   0 = Transmitter Delay Compensation is disabled
     *   1 = Transmitter Delay Compensation is enabled
     */
    uint32_t                    tdcEnable;

    /*! Transmitter Delay Compensation parameters.
     *   Refer struct #CANFD_MCANTdcConfig.
     */
    CANFD_MCANTdcConfig         tdcConfig;

    /*! Bus Monitoring Mode
     *   0 = Bus Monitoring Mode is disabled
     *   1 = Bus Monitoring Mode is enabled
     */
    uint32_t                    monEnable;

    /*! Restricted Operation Mode
     *   0 = Normal CAN operation
     *   1 = Restricted Operation Mode active
     *   This mode should not be combined with test modes.
     */
    uint32_t                    asmEnable;

    /*! Timestamp Counter Prescaler.
     *   Range:[0x0-0xF]
     */
    uint32_t                    tsPrescalar;

    /*! Timeout source selection.
     *   00b: Timestamp counter value always 0x0000
     *   01b: Timestamp counter value incremented according to tsPrescalar
     *   10b: External timestamp counter value used
     *   11b: Same as 00b
     */
    uint32_t                    tsSelect;

    /*! Time-out counter source select.
     *   Refer enum #CANFD_MCANTimeOutSelect.
     */
    CANFD_MCANTimeOutSelect     timeoutSelect;

    /*! Start value of the Timeout Counter (down-counter).
     *   The Timeout Counter is decremented in multiples of CAN bit times [1-16]
     *   depending on the configuration of the tsPrescalar.
     *   Range: [0x0-0xFFFF]
     */
    uint32_t                    timeoutPreload;

    /*! Timeout Counter Enable
     *   0 - Timeout Counter is disabled
     *   1 - Timeout Counter is enabled
     */
    uint32_t                    timeoutCntEnable;

    /*! Global Filter Configuration parameters.
     *    Refer struct #CANFD_MCANGlobalFiltConfig.
     */
    CANFD_MCANGlobalFiltConfig  filterConfig;

    /*! Message RAM Configuration parameters.
     *    Refer struct #CANFD_MCANMsgRAMCfgParams.
     */
    CANFD_MCANMsgRAMCfgParams   msgRAMConfig;

    /*! ECC Configuration parameters.
     *    Refer struct #CANFD_MCANECCConfigParams.
     */
    CANFD_MCANECCConfigParams   eccConfig;

    /*! Enable/Disable error/status interrupts
     * Note: Must be enabled to receive error and status interrupts. */
    uint32_t                    errInterruptEnable;

    /*! Enable/Disable data interrupts.
     * Note: Must be enabled to receive transmit complete and data receive interrupts. */
    uint32_t                    dataInterruptEnable;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CANFD driver on error or status interrrupts if errInterruptEnable is set to 1.
     */
    CANFD_ErrStatusAppCallBack  appErrCallBack;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CANFD driver once Tx complete or data receive interrupt has been received
     * if dataInterruptEnable is set to 1.
     */
    CANFD_DataAppCallBack     appDataCallBack;
}CANFD_MCANInitParams;

/*!
 * @brief
 *  Parameters used to configure the receive and transmit message objects.
 */
typedef struct CAN_MCANMsgObjCfgParams_t
{
    /*! Message object direction. For valid values refer to enum #CANFD_Direction. */
    CANFD_Direction         direction;

    /*! MCAN Id type: Standard / Extended Identifier. For valid values refer enum #CANFD_MCANXidType. */
    CANFD_MCANXidType       msgIdType;

    /*! Message Identifier - [28:0] are valid bits. */
    uint32_t                msgIdentifier;
} CANFD_MCANMsgObjCfgParams;

/*!
 * @brief
 *  Parameters used to configure a receive message objects for a range of message identifiers.
 */
typedef struct CAN_MCANRxMsgObjRangeCfgParams_t
{
    /*! MCAN Id type: Standard / Extended Identifier. For valid values refer enum #CANFD_MCANXidType. */
    CANFD_MCANXidType       msgIdType;

    /*! Starting Message Identifier - [28:0] are valid bits. */
    uint32_t                startMsgIdentifier;

    /*! Ending Message Identifier - [28:0] are valid bits. */
    uint32_t                endMsgIdentifier;
} CANFD_MCANRxMsgObjRangeCfgParams;

/*!
 *  @brief    Data structure defines the software maintained message object statistics.
 */
typedef struct CANFD_MCANMsgObjectStats_t
{
    /*! Message Object Handle for which the statistics is requested */
    CANFD_MsgObjHandle      handle;

    /*! Message Object direction */
    uint32_t                direction;

    /*! Starting range of the Message Id to which the configuration belongs.
     * For Tx and single Message Id objects the startMsgIdentifier = endMsgIdentifier */
    uint32_t                startMsgIdentifier;

    /*! Ending range of the Message Id to which the configuration belongs
     * For Tx and single Message Id objects the startMsgIdentifier = endMsgIdentifier. */
    uint32_t                endMsgIdentifier;

    /*! Number of interrupts received */
    uint32_t                interruptsRxed;

    /*! Number of messages processed */
    uint32_t                messageProcessed;
} CANFD_MCANMsgObjectStats;

/**
 * @brief
 *  Options TLV data structure
 *
 * @details
 *  Specifies the option type, length, value.
 */
typedef struct CANFD_OptionTLV_t
{
    /**
     * @brief   Option Name
     */
    CANFD_Option            type;

    /**
     * @brief   Option Length
     */
    int32_t                 length;

    /**
     * @brief   Option Value
     */
    void*                   value;
}CANFD_OptionTLV;

/** @}*/

/** @addtogroup CANFD_DRIVER_EXTERNAL_FUNCTION
 @{ */

/**
 *  @b Description
 *  @n
 *      Function initializes the CANFD driver instance with the specified hardware attributes.
 *      It resets and configures the MCAN module, sets up the Message RAM and ECC Aggregator.
 *      It configures the CANFD driver with the control parameters.
 *
 *  @param[in]  configParams
 *      CANFD module configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the CANFD Driver
 *  @retval
 *      Error   -   NULL
 */

CANFD_Handle CANFD_init(const CANFD_MCANInitParams* configParams, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function closes the CANFD driver instance and cleanups all the memory allocated by the CANFD driver.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */

extern int32_t CANFD_deinit(CANFD_Handle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function configures the bit time parameters for the CANFD module.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in]  bitTimeParams
 *      Bit time configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CANFD_configBitTime(CANFD_Handle handle, const CANFD_MCANBitTimingParams* bitTimeParams, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function configures the receive or transmit message object.
 *      It also enables Tx completion and Tx cancelation interrupts .
 *      The callback function will be invoked on data transmit complete for transmit message objects
 *      OR
 *      upon receiving data for receive message objects. The application MUST then call CANFD_getData() API to process the received data.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in]  msgObjectParams
 *      Message Object configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the message object.
 *  @retval
 *      Error   -   NULL
 */
extern CANFD_MsgObjHandle CANFD_createMsgObject(CANFD_Handle handle, const CANFD_MCANMsgObjCfgParams* msgObjectParams, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function configures a receive message objects for a range of message identifiers.
 *      It also enables Rx interrupts.
 *      The callback function will be invoked upon receiving data for receive message objects.
 *      The application MUST then call CANFD_getData() API to process the received data.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in]  msgObjectParams
 *      Message Object configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the message object.
 *  @retval
 *      Error   -   NULL
 */
extern CANFD_MsgObjHandle CANFD_createRxRangeMsgObject(CANFD_Handle handle, const CANFD_MCANRxMsgObjRangeCfgParams* msgObjectParams, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function deletes a message object.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CANFD_deleteMsgObject(CANFD_MsgObjHandle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function used by the application to transmit data.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[in]  id
 *      Message Identifier
 *  @param[in]  frameType
 *      Frame type - Classic or FD
 *  @param[in]  dataLength
 *      Data Length to be transmitted.
 *      Valid values: 1 to 64 bytes.
 *  @param[in]  data
 *      Data to be transmitted
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CANFD_transmitData(CANFD_MsgObjHandle handle, uint32_t id, CANFD_MCANFrameType frameType, uint32_t dataLength, const uint8_t* data, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function used by the application to cancel a pending data transmit.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CANFD_transmitDataCancel(CANFD_MsgObjHandle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function is used by the application to get the CAN message from message RAM using a receive message object.
 *      NOTE: This API must ONLY be called from the callback context.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  id
 *      Message Identifier
 *  @param[out]  ptrFrameType
 *      Frame type - Classic or FD
 *  @param[out]  idType
 *      Meassage Id type - 11 bit standard or 29 bit extended
 *  @param[out]  ptrDataLength
 *      Data Length of the received frame.
 *      Valid values: 1 to 64 bytes.
 *  @param[out]  data
 *      Received data.
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CANFD_getData(CANFD_MsgObjHandle handle, uint32_t* id, CANFD_MCANFrameType* ptrFrameType, CANFD_MCANXidType* idType, uint32_t* ptrDataLength, uint8_t* data, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function is used by the application to get the error and status information from the driver.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[out] ptrOptInfo
 *      Option info in TLV format which is populated with the requested information
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CANFD_getOptions(CANFD_Handle handle, const CANFD_OptionTLV* ptrOptInfo, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function is used by the application to configure the driver options.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in] ptrOptInfo
 *      Option info in TLV format which is used to configure the driver
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CANFD_setOptions(CANFD_Handle handle, const CANFD_OptionTLV* ptrOptInfo, int32_t* errCode);
/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CANFD_H_ */

