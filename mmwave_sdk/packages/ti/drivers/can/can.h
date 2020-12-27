/**
 *   @file  can.h
 *
 *   @brief
 *      This is the header file for the CAN driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the CAN driver.
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

/** @mainpage CAN Driver
 *
 *  The CAN driver provides functionality of transferring data between CAN peripherals.
 *  This driver does not interpret any of the data sent to or received from using this peripheral.
 *
 *
 *  The CAN header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/can/can.h>
 *   @endcode
 *
 *  ## Initializing the driver #
 *  The CAN Driver needs to be initialized once across the System. This is
 *  done using the #CAN_init. None of the CAN API's can be used without invoking
 *  this API.
 *
 *  Once the CAN Driver has been initialized; the bit timing can be configured using #CAN_configBitTime.
 *  This APIs can be called multiple times to reconfigure bit timings.
 *
 * ## Creating the message objects #
 *  Message objects are used to transmit or receive data over the CAN peripheral. A message object is created
 *  using #CAN_createMsgObject.
 *  The attributes of a message object can be reconfigured.
 *
 * ## Sending and receiving data #
 *  Data is transmitted using the #CAN_transmitData. The application can choose to receive a
 *  transmit complete interrupt by enabling the interrupt intEnable field and registering a callback
 *  function appCallBack when creating the transmit message object.
 *
 *  If the receive interrupts are enabled using intEnable field and a callback function appCallBack
 *  has been registered when creating the receive message object, the driver notifies the application
 *  when the data has arrived. The application needs to call the #CAN_getData function to read the received data.
 *
 * ## Error and status handling #
 *  The application can monitor the parity error, Bus off error and Error warning limits by enabling the error interrupts.
 *  The application can monitor the Wakeup Pending and LEC by enabling the status interrupts. The driver will call the
 *  registered callback function to indicate which error and status fields caused the interrupt.
 *  It is up to the application to take appropriate action.
 *
 * ## Get/Set Options #
 *  Helper APIs to get and set various statistics, error counters, ECC diagnostics, power down have been provided.
 *  Refer to #CAN_Option for more information.
 *
 * ## Limitation #
 *  The CAN driver does not support the DMA or fifo mode.
 *
 *  The can/include/hw_dcan.h has the register layer definitons for the DCAN controller module.
 *
 */

/** @defgroup CAN_DRIVER      CAN Driver
 */

#ifndef CAN_H_
#define CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>
#include <ti/drivers/can/include/hw_dcan.h>

/**
@defgroup CAN_DRIVER_EXTERNAL_FUNCTION     CAN Driver External Functions
@ingroup CAN_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the CAN driver
*/

/**
@defgroup CAN_DRIVER_EXTERNAL_DATA_STRUCTURE      CAN Driver External Data Structures
@ingroup CAN_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/

/**
@defgroup CAN_DRIVER_ERROR_CODE            CAN Driver Error code
@ingroup CAN_DRIVER
@brief
*   The section has a list of all the error codes which are returned to the application.
*   Base error code for the CAN module is defined in the
*  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   No Error
 */
#define CAN_EOK                     (0)

/**
 * @brief   Error Code: Invalid argument
 */
#define CAN_EINVAL                  (MMWAVE_ERRNO_CAN_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define CAN_EINUSE                  (MMWAVE_ERRNO_CAN_BASE-2)

/**
 * @brief   Error Code: Operation is not implemented.
 */
#define CAN_ENOTIMPL                (MMWAVE_ERRNO_CAN_BASE-3)

/**
 * @brief   Error Code: Out of memory
 */
#define CAN_ENOMEM                  (MMWAVE_ERRNO_SOC_BASE-4)

/** @}*/


/** @addtogroup CAN_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief      Maximum data length that can be transmitted or received in bytes.
 *  in a single message object.
 */
#define DCAN_MAX_MSG_LENGTH              (8U)

/*!
 *  @brief      CAN module handle returned by the CAN_init() API call.
 */
typedef void* CAN_Handle;

/*!
 *  @brief      CAN message object handle returned by the CAN_createMsgObject() API call.
 */
typedef void* CAN_MsgObjHandle;

/*!
 *  @brief    This enumeration defines the values used to set the direction of message object
 */
typedef enum CAN_Direction_t
{
    /*! Message object used to receive data */
    CAN_Direction_RX,

    /*! Message object used to transmit data */
    CAN_Direction_TX
} CAN_Direction;

/*!
 *  @brief    This enumeration defines DCAN test modes
 */
typedef enum CAN_DCANTestMode_t
{
    /*! All DCAN test mode is disabled. Use for normal operation. */
    CAN_DCANTestMode_NONE,

    /*! DCAN Silent test mode */
    CAN_DCANTestMode_SILENT,

    /*! DCAN loopback test mode */
    CAN_DCANTestMode_LPBACK,

    /*! DCAN loopback with silent test mode */
    CAN_DCANTestMode_LPBACK_SILENT,

    /*! DCAN external loopback test mode */
    CAN_DCANTestMode_EXT_LPBACK
} CAN_DCANTestMode;

/*!
 *  @brief    This enumeration defines the values used to represent the DCAN Identifier Type
 */
typedef enum CAN_DCANXidType_t
{
    /*! 11bit DCAN Identifier */
    CAN_DCANXidType_11_BIT,

    /*! 29bit DCAN Identifier */
    CAN_DCANXidType_29_BIT
} CAN_DCANXidType;

/*!
 *  @brief    This enumeration defines the values used to represent the DCAN mode of operation
 */
typedef enum CAN_DCANMode_t
{
    /*! DCAN normal operational mode */
    CAN_DCANMode_NORMAL,

    /*! DCAN initialization mode */
    CAN_DCANMode_INIT
} CAN_DCANMode;

/*!
 *  @brief    This enumeration defines the values used to represent the GET/SET options
 *
 *  @sa
 *  CAN_OptionTLV
 */
typedef enum CAN_Option_t
{
    /*! Used to get the DCAN Tx and Rx error counters
     * @sa CAN_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CAN_DCANErrorCounter) for this option.
     */
    CAN_Option_DCAN_ERROR_COUNTER,

    /*! Used to get the DCAN parity error information
     * @sa CAN_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CAN_DCANParityError) for this option.
     *
     */
    CAN_Option_DCAN_PARITY_ERROR,

    /*! Used to get the DCAN message object software maintained statistics
     * @sa CAN_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CAN_DCANMsgObjectStats) for this option.
     *      Application must fill in the message object handle for which the statistics is requested.
     *
     */
    CAN_Option_DCAN_MSG_OBJECT_STATS,

    /*! Used to get the DCAN ECC error status
     * @sa CAN_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CAN_DCANEccErrSts) for this option.
     *
     */
    CAN_Option_DCAN_ECC_ERROR_STATUS,

    /*! Used to get the DCAN ECC Diagnostics error status
     * @sa CAN_getOptions
     *
     * NOTE: The length in the TLV should be sizeof(CAN_DCANEccErrSts) for this option.
     *
     */
    CAN_Option_DCAN_ECC_DIAG_ERROR_STATUS,

    /*! Used to put the DCAN module in init or operational state
     * @sa CAN_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option.
     * Valid values: Refer to (CAN_DCANMode)
     */
    CAN_Option_DCAN_MODE,

    /*! Used to enable or disable parity
     * @sa CAN_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option.
     * Valid values are
     *  1   -   Parity is enabled
     *  0   -   Parity is disabled
     */
    CAN_Option_DCAN_PARITY,

    /*! Used to enable or disable ECC Diagnostics
     * @sa CAN_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option.
     * Valid values are
     *  1   -   ECC Diagnostics is enabled
     *  0   -   ECC Diagnostics is disabled
     */
    CAN_Option_DCAN_ECC_DIAG,

    /*! Used to clear the DCAN ECC error status
     * @sa CAN_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option. Valid values is 1.
     *
     */
    CAN_Option_DCAN_CLEAR_ECC_ERROR_STATUS,

    /*! Used to clear the DCAN ECC Diagnostics error status
     * @sa CAN_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option. Valid values is 1.
     *
     */
    CAN_Option_DCAN_CLEAR_ECC_DIAG_ERROR_STATUS,

    /*! Used to request a local power down or wakeup from a local power down
     * @sa CAN_setOptions
     *
     * NOTE: The length in the TLV should be 1 byte for this option.
     * Valid values are
     *  1   -   DCAN Sleep
     *  0   -   DCAN Wakeup
     *
     */
    CAN_Option_DCAN_POWER_DOWN,
} CAN_Option;

/*!
 * @brief
 *  Parameters used to configure the CAN bit timing.
 */
typedef struct CAN_DCANBitTimeParams_t
{
    /*! Baud rate prescaler
     *   Value by which the CAN_CLK frequency is divided for generating the bit
     *   time quanta. The bit time is built up from a multiple of this quanta
     *   Valid programmed values are 0 to 63. The actual BRP value interpreted
     *   for the bit timing will be programmed as BRP value + 1.
     */
    uint32_t baudRatePrescaler;

    /*! Synchronization Jump Width
     *   Valid programmed values are 0 to 3. The actual SJW value interpreted
     *   for the synchronization will be as programmed SJW value + 1.
     */
    uint32_t syncJumpWidth;

    /*! Time segment before the sample point
     *   Valid programmed values are 1 to 15. The actual TSeg1 value interpreted
     *   for the bit timing will be programmed as TSeg1 value + 1.
     */
    uint32_t timeSegment1;

    /*! Time segment after the sample point
     *   Valid programmed values are 0 to 7. The actual TSeg2 value interpreted
     *   for the bit timing will be the programmed as TSeg2 value + 1.
     */
    uint32_t timeSegment2;

    /*! Baud rate prescaler extension
     *   Valid programmed values are 0 to 15. By programming BRPE, the baud rate
     *   prescaler can be extended to values up to 1024.
     */
    uint32_t baudRatePrescalerExt;
} CAN_DCANBitTimeParams;

/*!
 * @brief
 *  Response structure definition for Error and status information.
 */
typedef struct CAN_ErrStatusResp_t
{
    /*! Local power down mode acknowledge
     * Valid values are:
     * 0 - DCAN is not in local power down mode.
     * 1 - Application request for setting DCAN to local power down mode was
     *      successful. DCAN is in local power down mode.
     */
    uint8_t powerDownAck;

    /*! Wake Up Pending. This bit can be used by the CPU to identify the DCAN as the source to wake up the system.
     * Valid values are:
     * 0 - No Wake Up is requested by DCAN.
     * 1 - DCAN has initiated a wake up of the system due to dominant CAN bus while module power down.
     */
    uint8_t wakeUpPend;

    /*! Parity/single/double bit Error Detected.
     * Valid values are:
     * 0 - No parity/single/double bit error has been detected since last read access.
     * 1 - The parity check mechanism has detected a parity error in the Message RAM.
     */
    uint8_t parityError;

    /*! Bus-Off state.
     * Valid values are:
     * 0 - The CAN module is not in Bus-Off state.
     * 1 - The CAN module is in Bus-Off state.
     */
    uint8_t busOff;

    /*! Warning state.
     * Valid values are:
     * 0 - Both error counters are below the error warning limit of 96.
     * 1 - At least one of the error counters has reached the error warning limit of 96.
     */
    uint8_t eWarn;

    /*! Error Passive state.
     * Valid values are:
     * 0 - On CAN Bus error, the DCAN could send active error frames.
     * 1 - The CAN Core is in the error passive state
     */
    uint8_t ePass;

    /*! Last Error Code.
     * Valid values are:
     * 0 - No Error.
     * 1 - Stuff Error: More than five equal bits in a row have been detected in
     *          a part of a received message where this is not allowed.
     * 2 - Form Error: A fixed format part of a received frame has the wrong format.
     * 3 - Ack Error: The message this CAN Core transmitted was not acknowledged by another node.
     * 4 - Bit1 Error: During the transmission of a message (with the exception of the arbitration field), the
     *          device wanted to send a recessive level (bit of logical value ‘1’), but the monitored bus
     *          value was dominant.
     * 5 - Bit0 Error: During the transmission of a message (or acknowledge bit, or active error flag, or overload flag),
     *          the device wanted to send a dominant level (logical value ‘0’), but the monitored bus level was
     *          recessive.
     * 6 - CRC Error: In a received message, the CRC check sum was incorrect.
     * 7 - No CAN bus event was detected since the last time when CPU has read the Error and Status Register.
                Any read access to the Error and Status Register re-initializes the LEC to value ‘7’.
     */
    uint8_t lec;
} CAN_ErrStatusResp;

/**
 * @brief
 * Application specified callback function which is invoked
 * by the CAN driver once transmit is complete or data has been received for the
 * specified message object.
 *
 *  @param[in]  handle
 *      Message object handle for which the callback function is invoked.
 *  @param[in]  msgObjectNum
 *      Message Object number for which the callback is invoked.
 *  @param[in]  direction
 *      Indicates transmit or receive
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CAN_MsgObjAppCallBack)(CAN_MsgObjHandle handle, uint32_t msgObjectNum, CAN_Direction direction);

/**
 * @brief
 * Application specified callback function which is invoked
 * by the CAN driver on error or status change.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
 *  @param[out]  errStatusResp
 *      Response structure populated with the value of the fields that caused the error or status interrupt.
 *
 *  @retval
 *      Not applicable
 */
typedef void (*CAN_ErrStatusAppCallBack)(CAN_Handle handle, CAN_ErrStatusResp* errStatusResp);

/*!
 * @brief
 *  Parameters used to configure the DCAN module.
 */
typedef struct CAN_DCANCfgParams_t
{
    /*! Parity/ECC Enable or Disable
     *   Valid values: 1 or 0.
     */
    uint32_t                    parityEnable;

    /*! Enable/disable SECDED single bit error correction
     *   Valid values: 1 or 0.
     */
    uint32_t                    eccModeEnable;

    /*! Enable/disable SECDED Diagnostic Mode
     *   Valid values: 1 or 0.
     */
    uint32_t                    eccDiagModeEnable;

    /*! Enable/disable SECDED single bit error event
     *   Valid values: 1 or 0.
     */
    uint32_t                    sbeEventEnable;

    /*! Interrupt line 0 enable/disable. Valid values: 1 or 0.
     * Note: Must be enabled to receive error and status interrupts. */
    uint32_t                    intrLine0Enable;

    /*! Interrupt line 1 enable/disable. Valid values: 1 or 0.
     * Note: Must be enabled to receive transmit complete and data receive interrupts. */
    uint32_t                    intrLine1Enable;

    /*! Status Change Interrupt enable/disable. Valid values: 1 or 0.
     * NOTE: Application must register a callback function, #CAN_ErrStatusAppCallBack, if it needs to be notified */
    uint32_t                    stsChangeIntrEnable;

    /*! Error Interrupt enable/disable. Valid values: 1 or 0.
     * NOTE: Application must register a callback function, #CAN_ErrStatusAppCallBack, if it needs to be notified */
    uint32_t                    errIntrEnable;

    /*! IF1 DMA enable/disable. Valid values: 1 or 0. */
    uint32_t                    if1DmaEnable;

    /*! IF2 DMA enable/disable. Valid values: 1 or 0. */
    uint32_t                    if2DmaEnable;

    /*! IF3 DMA enable/disable. Valid values: 1 or 0. */
    uint32_t                    if3DmaEnable;

    /*! Disable Automatic retransmission of failure messages
     *   Valid values: 1 or 0.
     */
    uint32_t                    autoRetransmitDisable;

    /*! Auto Bus on enable/disable. Valid values: 1 or 0. */
    uint32_t                    autoBusOnEnable;

    /*! Auto Bus-On Timer Value in OCP clocks
     *   Valid only when autoBusOnEnable is set.
     */
    uint32_t                    autoBusOnTimerVal;

    /*! Test Mode enable/disable. Valid values: 1 or 0, */
    uint32_t                    testModeEnable;

    /*! Test mode. For valid values refer enum #CAN_DCANTestMode
     *   Valid only when testModeEnable is set,
     */
    uint32_t                    testMode;

    /*! Direct access to the RAM is enabled/disabled while in test mode
     *   Valid only when testModeEnable is set - Valid values: 1 or 0.
     */
    uint32_t                    ramAccessEnable;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CAN driver on error or status interrrupts.
     */
    CAN_ErrStatusAppCallBack    appCallBack;
} CAN_DCANCfgParams;

/*!
 * @brief
 *  Parameters used to configure the receive and transmit message objects.
 */
typedef struct CAN_DCANMsgObjCfgParams_t
{
    /*! Message object direction. For valid values refer to enum #CAN_Direction. */
    CAN_Direction           direction;

    /*! Extended Identifier IDE bit Mask: Valid values: 1 or 0. */
    uint32_t                xIdFlagMask;

    /*! Message Direction Mask: Valid values: 1 or 0. */
    uint32_t                dirMask;

    /*! Identifier Mask - [28:0]  are valid bits. */
    uint32_t                msgIdentifierMask;

    /*! Extended IDE Flag : Standard / Extended Identifier. For valid values refer enum #CAN_DCANXidType. */
    CAN_DCANXidType         xIdFlag;

    /*! Message Identifier - [28:0] are valid bits. */
    uint32_t                msgIdentifier;

    /*! Valid or Invalid Message Objects. Valid values: 1 or 0. */
    uint32_t                msgValid;

    /*! Use mask (Msk[28:0], MXtd, and MDir) for acceptance filtering or not. */
    uint32_t                uMaskUsed;

    /*! Interrupt Enable : IntPnd will be triggered or not after the
     *   successful transmission/reception of a frame
     *   Valid values: 1 or 0.
     */
    uint32_t                intEnable;

    /*! At the reception of a remote frame, TxRqst is changed or not
     *   Valid only for TX - Valid values: 1 or 0.
     */
    uint32_t                remoteEnable;

    /*!  Message object is part of FIFO Buffer or not
     *    Valid only for RX - Valid values: 1 or 0.
     */
    uint32_t                fifoEOBFlag;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CAN driver once Tx complete or data receive interrupt has been received for the
     * specified message object.
     */
    CAN_MsgObjAppCallBack   appCallBack;
} CAN_DCANMsgObjCfgParams;

/*!
 *  @brief    Data structure used by DCAN to receive or transmit data using a message object.
 */
typedef struct CAN_DCANData_t
{
    /*!  Message object data length - Valid values: 1 to 8 bytes. */
    uint32_t                dataLength;

    /*! At the reception of a Message, Message is received or lost
     *   Valid only for RX - Valid values: 1 or 0.
     *   Ignored for TX.
     */
    uint32_t                msgLostFlag;

    /*! Data Value to be transmitted. */
    uint8_t                 msgData[DCAN_MAX_MSG_LENGTH];
} CAN_DCANData;

/*!
 *  @brief    Data structure defines the DCAN transmit and receiev Error Counter.
 */
typedef struct CAN_DCANErrorCounter_t
{
    /*!  Receieve Error Passive */
    uint8_t                 rxErrPassive;

    /*!  Receieve Error Counter */
    uint8_t                 rxErrCounter;

    /*!  Transmit Error Counter */
    uint8_t                 txErrCounter;
} CAN_DCANErrorCounter;

/*!
 *  @brief    Data structure defines the DCAN Parity Error.
 */
typedef struct CAN_DCANParityError_t
{
    /*!  Word number where parity error has been detected */
    uint8_t                 wordNum;

    /*!  Message Object number where parity error has been detected */
    uint8_t                 messageNum;
} CAN_DCANParityError;

/*!
 *  @brief    Data structure defines the DCAN ECC Error status.
 */
typedef struct CAN_DCANEccErrSts_t
{
    /*!  DCAN ECC : When one bit Error occured on Message Ram */
    uint32_t                singleBitErr;

    /*!  DCAN ECC : When two bits Error occured on Message Ram */
    uint32_t                doubleBitErr;

    /*!  Message Object number where single bit error has occured.
     *   Valid when CAN_getOptions with (DCAN_ECC_ERROR_STATUS) is called.
     *   Not valid when CAN_getOptions with (DCAN_ECC_DIAG_ERROR_STATUS) is called.
     */
    uint32_t                messageNum;
} CAN_DCANEccErrSts;

/*!
 *  @brief    Data structure defines the software maintained message object statistics.
 */
typedef struct CAN_DCANMsgObjectStats_t
{
    /*! Message Object Handle for which the statistics is requested */
    CAN_MsgObjHandle        handle;

    /*! Message Object number */
    uint32_t                msgObjectNum;

    /*! Message Object direction */
    uint32_t                direction;

    /*! Number of interrupts received */
    uint32_t                interruptsRxed;

    /*! Number of messages processed */
    uint32_t                messageProcessed;
} CAN_DCANMsgObjectStats;

/**
 * @brief
 *  Options TLV data structure
 *
 * @details
 *  Specifies the option type, length, value.
 */
typedef struct CAN_OptionTLV_t
{
    /**
     * @brief   Option Name
     */
    CAN_Option              type;

    /**
     * @brief   Option Length
     */
    int32_t                 length;

    /**
     * @brief   Option Value
     */
    void*                   value;
}CAN_OptionTLV;

/** @}*/

/** @addtogroup CAN_DRIVER_EXTERNAL_FUNCTION
 @{ */

/**
 *  @b Description
 *  @n
 *      Function initializes the CAN driver instance with the specified hardware attributes.
 *      It resets the CAN module and configures the DCAN TX/RX IO Control register.
 *      It configures the CAN driver with the control parameters.
 *
 *  @param[in]  configParams
 *      CAN module configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the CAN Driver
 *  @retval
 *      Error   -   NULL
 */

CAN_Handle CAN_init(const CAN_DCANCfgParams* configParams, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function closes the CAN driver instance and cleanups all the memory allocated by the CAN driver.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */

extern int32_t CAN_deinit(CAN_Handle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function configures the bit time parameters for the CAN module.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
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
extern int32_t CAN_configBitTime(CAN_Handle handle, const CAN_DCANBitTimeParams* bitTimeParams, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function configures the receive or transmit message object.
 *      It also registers interrupts if enabled and registers the application provided callback function.
 *      The callback function will be invoked on data transmit complete for transmit message objects
 *      OR
 *      upon receiving data for receive message objects.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
 *  @param[in]  msgObjectNum
 *      Message Object number which has to be configured.
 *      Valid values are 1 to 64. The message object number also specifies its priority. Lower the number higher the priority.
 *
 *      NOTE: Since the the message object number denotes its priority, the CAN driver doesn't allocate the next available message
 *      object instead it is specified by the application.
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
extern CAN_MsgObjHandle CAN_createMsgObject(CAN_Handle handle, uint32_t msgObjectNum, const CAN_DCANMsgObjCfgParams* msgObjectParams, int32_t* errCode);

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
extern int32_t CAN_deleteMsgObject(CAN_MsgObjHandle handle, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function used by the application to transmit data using a transmit message object.
 *
 *  @param[in]  handle
 *      Handle to the message object
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
extern int32_t CAN_transmitData(CAN_MsgObjHandle handle, const CAN_DCANData* data, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function is used by the application to get the CAN message from message RAM using a receive message object.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  data
 *      Pointer to the data bytes received, length and message lost flag.
 *      Populated only when there is no error. If there is an error, the function returns the reason for error in errCode.
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
extern int32_t CAN_getData(CAN_MsgObjHandle handle, CAN_DCANData* data, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function is used by the application to get the error and status information from the driver.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
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
int32_t CAN_getOptions(CAN_Handle handle, CAN_OptionTLV* ptrOptInfo, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function is used by the application to configure the driver options.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
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
int32_t CAN_setOptions(CAN_Handle handle, CAN_OptionTLV* ptrOptInfo, int32_t* errCode);
/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CAN_H_ */

