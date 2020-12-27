/*!
 *   @file  can_internal.h
 *
 *   @brief
 *      This is an internal header file used by the CAN module and should
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
@defgroup CAN_DRIVER_INTERNAL_FUNCTION            CAN Driver Internal Functions
@ingroup CAN_DRIVER
@brief
*   The section has a list of all the internal functions used by the driver
*/
/**
@defgroup CAN_DRIVER_INTERNAL_DATA_STRUCTURE      CAN Driver Internal Data Structures
@ingroup CAN_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifndef CAN_INTERNAL_H
#define CAN_INTERNAL_H

#include <ti/drivers/osal/HwiP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/can/include/hw_dcan.h>
#include <ti/drivers/can/can.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief      Maximum number of message objects that can be stored in Message RAM
 */
#define DCAN_MAX_MSG_OBJECTS             (64U)

/*!
 * @brief Message Object Size*/
#define DCAN_MSG_OBJ_SIZE                (0x20U)

/** @addtogroup CAN_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief    Enumerates the values used to represent the DCAN interrupt lines
 */
typedef enum CAN_DCANIntrLineNum_t
{
    /*! DCAN interrupt line 0 */
    CAN_DCANIntrLineNum_0,

   /*! DCAN interrupt line 1 */
   CAN_DCANIntrLineNum_1
} CAN_DCANIntrLineNum;

/*!
 *  @brief    This enumeration defines the DCAN interface register numbers
 */
typedef enum CAN_DCANIfRegNum_t
{
    /*! DCAN interface register number 1 used for both read and write */
    CAN_DCANIfRegNum_1 = 1U,

    /*! DCAN interface register number 2 used for both read and write */
    CAN_DCANIfRegNum_2 = 2U,

    /*! DCAN interface register number 3 used only for read only */
    CAN_DCANIfRegNum_3 = 3U
} CAN_DCANIfRegNum;

/*!
 *  @brief    This enumeration defines the values used to represent the CAN driver state
 */
typedef enum CAN_DriverState_t
{
    /*! Can controller not initialized */
    CAN_DriverState_UNINIT,

    /*! Can controller started */
    CAN_DriverState_STARTED,

    /*! Can controller stopped */
    CAN_DriverState_STOPPED,

    /*! Can controller in sleep mode */
    CAN_DriverState_SLEEP
}CAN_DriverState;

/**
 * @brief
 *  CAN Hardware Atrributes
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
typedef struct CAN_HwCfg_t
{
    /*!
     * @brief   Base address of the register address space to be used.
     */
    uint32_t            regBaseAddress;

    /*!
     * @brief   Base address of the Message RAM address space to be used.
     */
    uint32_t            memBaseAddress;

    /*!
     * @brief   Interrupt level 0 Number
     */
    uint32_t            interruptNum0;

    /*!
     * @brief   Interrupt level 1 Number
     */
    uint32_t            interruptNum1;
}CAN_HwCfg;

/**
 * @brief
 *  CAN Master Control Block
 *
 * @details
 *  The structure describes the CAN Driver and is used to hold the relevant
 *  information with respect to the CAN module.
 */
typedef struct CAN_DriverMCB_t
{
    /**
     * @brief   CAN driver internal state
     */
    CAN_DriverState    state;

    /**
     * @brief   Hardware Configuration of the CAN instance
     */
    CAN_HwCfg           hwCfg;

    /**
     * @brief   Configuration used to initialize the CAN module
     */
    CAN_DCANCfgParams   cfg;

    /**
     * @brief   Message object handle book keeping
     */
    struct CAN_MessageObject_t*  msgObjectHandle[DCAN_MAX_MSG_OBJECTS];

    /**
     * @brief   Registered Interrupt Handler. Indicates if interrupt 0 is configured. This needs to be done only
     * during the first message object create. Subsequent interrupts are enabled using interrupt muxing
     */
    HwiP_Handle         hwiHandle0;

    /**
     * @brief   Registered Interrupt Handler. Indicates if interrupt 0 is configured. This needs to be done only
     * during the first message object create. Subsequent interrupts are enabled using interrupt muxing
     */
    HwiP_Handle         hwiHandle1;

    /**
     * @brief   Number of error and status interrupts received
     */
    uint32_t            errStatusInterrupts;

    /**
     * @brief   Number of interrupts received for message Tx or Rx */
    uint32_t            messageInterrupts;

    /**
     * @brief   Number of spurious interrupts received
     */
    uint32_t            spuriousInterrupts;
}CAN_DriverMCB;

/**
 * @brief
 *  CAN message object block
 *
 * @details
 *  The structure defines the message object
 */
typedef struct CAN_MessageObject_t
{
    /**
     * @brief   Message Object to which the configuration belongs
     */
    uint32_t                msgObjectNum;

    /**
     * @brief   Pointer to the CAN driver MCB
     */
    CAN_DriverMCB*          ptrDriverMCB;

    /**
     * @brief   Message object direction.
     */
    CAN_Direction           direction;

    /**
     * @brief   Application specified callback function which is invoked
     * by the CAN driver once Tx complete or data receieve interrupt has been received for the
     * specified message object.
     */
    CAN_MsgObjAppCallBack   appCallBack;

    /**
     * @brief   Number of interrupts received
     */
    uint32_t                interruptsRxed;

    /**
     * @brief   Number of messages processed
     */
    uint32_t                messageProcessed;
}CAN_MessageObject;

/** @}*/

extern CAN_HwCfg gCanHwCfg[1];

/** @addtogroup CAN_DRIVER_INTERNAL_FUNCTION
 @{ */

/**
 * \brief     This API will configure the operating mode of DCAN
 *            In Init mode the communication on CAN bus is stopped and the
 *            controller needs to be placed in Init mode for initialization
 *            To start communication on CAN bus please keep the DCAN controller
 *            in Normal mode of operation
 *
 * \param     baseAddr    Base Address of the DCAN module Register
 * \param     mode        DCAN mode to be configured
 *                        For valid values refer enum #CAN_DCANMode
 *
 * \return    None
 */
void DCANSetMode(uint32_t baseAddr, uint32_t mode);


/**
 * \brief   This API configures bit time parameters for DCAN module
 *
 * \param   baseAddr       Base Address of the DCAN module Register
 * \param   pBitTimePrms   Pointer which holds DCAN bit time parameters
 *
 * \return  None
 */
void DCANSetBitTime(uint32_t baseAddr, const CAN_DCANBitTimeParams* pBitTimePrms);

/**
 * \brief     This API will reset the DCAN module and configure the DCAN TX/RX
 *            IO Control register
 *
 * \param     baseAddr    Base Address of the DCAN module Register
 * \param     retry       Retry value
 *
 * \return    CAN_EOK on success else <0 when DCAN reset fails
 *
 * \note      This API has to be called before the driver can be used
 */
int32_t DCANReset(uint32_t baseAddr, uint32_t retry);


/**
 * \brief     This API configures DCAN controller
 *
 * \param     baseAddr     Base Address of the DCAN module Register
 * \param     pDcanCfgPrms Pointer which holds DCAN controller config parameters
 *
 * \return    None
 *
 * \note      The API DCANReset has to be called before this API can
 *            be used
 */
void DCANConfig(uint32_t baseAddr, const CAN_DCANCfgParams* pDcanCfgPrms);

/**
 * \brief     This API will configure which interrupt line is used to service
 *            interrupts for message objects
 *
 * \param     baseAddr         Base Address of the DCAN module register
 * \param     msgObj           Message object number
 * \param     intrLineNum      Interrupt line number to be configured
 *                             For valid values refer enum #CAN_DCANIntrLineNum
 *
 * \return    None
 */
void DCANConfigIntrMux(uint32_t baseAddr,
                       uint32_t intrLineNum,
                       uint32_t msgObj);

/**
 * \brief     This API configures DCAN TX/RX message object
 *
 * \param     baseAddr          Base Address of the DCAN module register
 * \param     msgObj            Message object number of CAN node
 *                              Valid values: 1 to 64
 * \param     ifRegNum          DCAN interface register number
 *                              For valid value refer enum #CAN_DCANIfRegNum
 * \param     pMsgObjCfgPrms    Pointer which holds DCAN message object Config
 *                              parameters of TX/RX
 *
 * \return    CAN_EOK on success else <0 when IF register busy flag is
 *            set
 */
int32_t DCANConfigMsgObj(uint32_t                       baseAddr,
                         uint32_t                       msgObj,
                         uint32_t                       ifRegNum,
                         const CAN_DCANMsgObjCfgParams* pMsgObjCfgPrms);

/**
 * \brief     This API transmits a CAN message and writes a CAN message into a
 *            CAN message object
 *
 * \param     baseAddr    Base Address of the DCAN module register
 * \param     msgObj      Message object number of CAN node
 *                        Valid values: 1 to 64
 * \param     ifRegNum    DCAN interface register number
 *                        For valid values refer enum #CAN_DCANIfRegNum
 * \param     pDcanTxPrms Pointer which holds DCAN transmit parameters
 * \param     retry       Retry value
 *
 * \return    CAN_EOK on success else <0 when IF register busy flag is
 *            set
 */
int32_t DCANTransmitData(uint32_t               baseAddr,
                         uint32_t               msgObj,
                         uint32_t               ifRegNum,
                         const CAN_DCANData*    pDcanTxPrms,
                         uint32_t               retry);

/**
 * \brief     This API gets received CAN message from message RAM
 *
 * \param     baseAddr       Base Address of the DCAN module register
 * \param     msgObj         Message object number of CAN node
 *                           Valid values : 1 to 64
 *
 * \param     ifRegNum       DCAN interface register number
 *                           For valid values refer enum #CAN_DCANIfRegNum
 * \param     pDcanRxPrms    Pointer which holds DCAN receive parameters
 * \param     retry          Retry value
 *
 * \return    CAN_EOK on success else <0 when IF register busy flag is
 *            set
 *
 * \note      The API will receive valid data of size dataLength
 */
int32_t DCANGetData(uint32_t        baseAddr,
                    uint32_t        msgObj,
                    uint32_t        ifRegNum,
                    CAN_DCANData*   pDcanRxPrms,
                    uint32_t        retry);

/**
 * \brief     This API returns Tx message object transmission status whether Tx
 *            message object has a pending Tx request or not
 *
 * \param     baseAddr    Base Address of the DCAN module register
 * \param     msgObj      Message object number of CAN node
 *                        Valid values: 1 to 64
 *
 * \return    0 when data transmission is successful else 1 when data
 *            transmission is not successful
 */
uint32_t DCANIsTxMsgPending(uint32_t baseAddr, uint32_t msgObj);

/**
 * \brief     This API returns Rx message object reception status whether Rx
 *            message object has a pending Rx data or not
 *
 * \param     baseAddr    Base Address of the DCAN module register
 * \param     msgObj      Message object number of CAN node
 *                        Valid values: 1 to 64
 *
 * \return     1 when new data has arrived else 0 when new data has not
 *             arrived
 */
uint32_t DCANHasRxMsgArrived(uint32_t baseAddr, uint32_t msgObj);

/**
 * \brief     This API checks for whether msgObj is valid or not
 *
 * \param     baseAddr         Base Address of the DCAN Module Registers.
 * \param     msgObj           Message object number whose message valid status
 *                             is to be returned.
 *
 * \return    1 when message object is valid else 0 when message object
 *            is invalid
 */
uint32_t DCANIsMsgValid(uint32_t baseAddr, uint32_t msgObj);

/**
 * \brief     This API checks for whether DCAN IF Register is busy or not
 *
 * \param     baseAddr       Base Address of the DCAN module register
 * \param     ifRegNum       DCAN interface register number
 *                           For valid values refer enum #CAN_DCANIfRegNum
 *
 * \return     1 when DCAN IF Register is busy else 0 when DCAN IF
 *             Register is not busy
 */
uint32_t DCANIsIfRegBusy(uint32_t baseAddr, uint32_t ifRegNum);


/**
 * \brief     This API returns interrupt line status of DCAN
 *
 * \param     baseAddr         Base Address of the DCAN module register
 * \param     intrLineNum      DCAN interrupt line number
 *                             For valid values refer enum #CAN_DCANIntrLineNum
 *
 * \return    message object number with highest pending interrupt or
 *            number which indicates Source of the interrupt
 *            0x0000        : No interrupt is pending
 *            0x0001-0x0040 : Message object Number which caused the interrupt
 *            0x0041-0x7FFF : Unused
 *            0x8000        : DCAN_ES value is not 0x07.
 *            0x8001-0xFFFF : Unused
 */
uint32_t DCANGetIntrStatus(uint32_t baseAddr, uint32_t intrLineNum);

/**
 * \brief     This API returns message object interrupt pending status
 *
 * \param     baseAddr    Base Address of the DCAN module register
 * \param     msgObj      Message object number of CAN node
 *                        Valid values: 1 to 64
 *
 * \return     1 when message object has pending interrupt else 0 when
 *             message object has no pending interrupt
 */
uint32_t DCANIsMsgObjIntrPending(uint32_t baseAddr, uint32_t msgObj);

/**
 * \brief   This API will clear the interrupt pending status of received message
 *          objects after a new message is received
 *
 * \param   baseAddr      Base Address of the DCAN module register
 * \param   msgObj        Message object number of CAN node
 *                        Valid values : 1 to 64
 * \param   ifRegNum      DCAN interface register number
 *                        For valid values refer enum #CAN_DCANIfRegNum
 * \return  None
 */
void DCANIntrClearStatus(uint32_t baseAddr, uint32_t msgObj, uint32_t ifRegNum);

/**
 * \brief     This API will read the error and status information of DCAN
 *            controller
 *
 * \param     baseAddr         Base Address of the DCAN module register
 *
 * \return    Returns the error and status values.
 *
 * \note      Reading the error and status register will clear/set certain bits
 *            in the error and status register. For more information please
 *            refer the DCAN Technical Reference Manual(TRM). For debug support,
 *            the auto clear functionality of error and status register is
 *            disabled when in debug/suspend mode
 */
uint32_t DCANGetErrStatus(uint32_t baseAddr);

/**
 * \brief     This API will read the error counter status of DCAN
 *
 * \param     baseAddr         Base Address of the DCAN module register
 *
 * \return    Returns the error counter status values.
 */
uint32_t DCANGetErrCntrStatus(uint32_t baseAddr);

/**
 * \brief     This API will read the DCAN parity error status
 *
 * \param     baseAddr         Base Address of the DCAN module register
 *
 * \return    Returns the DCAN parity error status.
 */
uint32_t DCANGetParityErrStatus(uint32_t baseAddr);

/**
 * \brief     This API returns message object number where DCAN ECC Single
 *            Bit Error is detected
 *
 * \param     baseAddr  Base Address of the DCAN module register
 *
 * \return    retVal    Returns the message object number where DCAN ECC Single
 *                      Bit Error is detected
 */
uint32_t DCANGetMsgObjBitErrDetected(uint32_t baseAddr);

/**
 * \brief     This API will read the ECC diagnostic status information of DCAN
 *            controller
 *
 * \param     baseAddr         Base Address of the DCAN module register
 *
 * \return    Returns the ECC disagnostic error status information.
 */
uint32_t DCANGetEccDiagErrStatus(uint32_t baseAddr);

/**
 * \brief     This API will read the ECC error status information of DCAN
 *            controller
 *
 * \param     baseAddr         Base Address of the DCAN module register
 *
 * \return    Returns the ECC error status information.
 */
uint32_t DCANGetEccErrStatus(uint32_t baseAddr);

/**
 * \brief     Enable/disable the DCAN parity/ECC
 *
 * \param     baseAddr    Base Address of the DCAN module register
 * \param     enablePMD   Enable/Disable DCAN parity
 *                        Valid values: 1 or 0
 *
 * \return  None
 */
void DCANParityEnable(uint32_t baseAddr, uint32_t enablePMD);

/**
 * \brief     Enable/ Disable the test mode of DCAN
 *
 * \param     baseAddr         Base Address of the DCAN module register
 * \param     enableTestMode   Enable/Disable test mode of DCAN.
 *                             Valid values: 1 or 0
 * \param     testMode         Select the DCAN test mode
 *                             For valid values refer enum #CAN_DCANTestMode
 *
 * \return  None
 */
void DCANTestModeEnable(uint32_t baseAddr,
                        uint32_t enableTestMode,
                        uint32_t testMode);

/**
 * \brief     Enable/disable the DCAN ECC Diagnostic Mode.
 *
 * \param     baseAddr              Base Address of the DCAN module register
 * \param     enableEccDiagMode     Enable/Disable DCAN ECC Diagnostic Mode
 *                                  Valid values: 1 or 0
 *
 * \return  None
 */
void DCANEccDiagModeEnable(uint32_t baseAddr, uint32_t enableEccDiagMode);

/**
 * \brief     This API is used to Enable/disable SECDED Single bit Error
 *            correction and SECDED Single bit Error Event
 *
 * \param     baseAddr              Base Address of the DCAN module register
 * \param     enableEccMode         Enable/Disable SECDED Single bit Error
 *                                  correction.
 *                                  Valid values: 1 or 0
 * \param     enableSbeEvent        Enable/Disable SECDED Single bit Error
 *                                  Event.
 *                                  Valid values: 1 or 0
 *
 * \return  None
 */
void DCANEccModeEnable(uint32_t baseAddr,
                       uint32_t enableEccMode,
                       uint32_t enableSbeEvent);

/**
 * \brief   This function clears the error status of DCAN ECC.
 *
 * \param     baseAddr              Base Address of the DCAN module register
 *
 * \return      None
 */
void DCANClrEccErrStatus(uint32_t baseAddr);

/**
 * \brief   This function clears the error status of DCAN ECC Diagnostic.
 *
 * \param       baseAddr              Base Address of the DCAN module register
 *
 * \return      None
 */
void DCANClrEccDiagErrStatus(uint32_t baseAddr);

/**
 * \brief   This function clears the status of parity error on
 *          DCAN_PARITY interrupt line
 *
 * \param       baseAddr              Base Address of the DCAN module register
 *
 * \return      None
 */
void DCANClrParityIntrStatus(uint32_t baseAddr);

/** @}*/
#ifdef __cplusplus
}
#endif

#endif /* CAN_INTERNAL_H */
