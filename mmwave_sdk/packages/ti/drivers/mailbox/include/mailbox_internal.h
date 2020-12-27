/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       mailbox_internal.h
 *
 *  Mailbox Internal definitions
 *  ============================================================================
 */

#ifndef ti_drivers_Mailbox_internal__include
#define ti_drivers_Mailbox_internal__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/mailbox/include/reg_mailbox.h>
#include <ti/drivers/mailbox/mailbox.h>

/** @addtogroup MAILBOX_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief   Mailbox header size in bytes. Header is used only between MSS and DSS communication when mailbox channel type is multi-channel.
 */
#define MAILBOX_MULTI_CH_HEADER_SIZE   (4U)

/**
 * @brief   Mailbox unused channel ID
 */
#define MAILBOX_UNUSED_CHANNEL_ID (0xFFU)

/*!
 *  @brief      Mailbox TX box status enum
 *
 *  This enum defines the possible TX box status.
 *
 */
typedef enum
{
    /*!
      *  Status of TX box is empty. Acknowledge of the last TX message was received
      *  and a new message can be transmitted.
      */
    MAILBOX_TX_BOX_EMPTY,
    /*!
      *  Status of TX box is full. Acknowledge of the last TX message was not received
      *  and a new message can not be transmitted.
      */
    MAILBOX_TX_BOX_FULL
} Mailbox_TxBoxStatus;

/*!
 *  @brief  Mailbox Acknowledge flag 
 *
 *  This enum defines the possible acknowledge flag values. Used for write polling mode only.
 *
 */
typedef enum
{
    /*!
      *  Ack not received
      */
    MAILBOX_TX_ACK_NOT_RECEIVED,
    /*!
      *  Ack received
      */
    MAILBOX_TX_ACK_RECEIVED
} Mailbox_TxAckFlag;

/*!
 *  @brief      Mailbox new message flag 
 *
 *  This enum defines the possible acknowledge flag values. Used for read polling mode only.
 *
 */
typedef enum
{
    /*!
      *  new message not received
      */
    MAILBOX_NEW_MESSAGE_NOT_RECEIVED,
    /*!
      *  New message received
      */
    MAILBOX_NEW_MESSAGE_RECEIVED
} Mailbox_NewMsgFlag;

/*!
 *  @brief Mailbox hardware addresses for one mailbox in one direction
 *
 *  This Structure contains the register and data memory addresses for one logical mailbox in one direction.
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_DATA_STRUCTURE 
 */
typedef struct Mailbox_Base_t
{
    /**
     * @brief  Register base.
     */
    MAILBOXRegs   *reg;
    
    /**
     * @brief  Data memory base.
     */
    uint8_t        *data;
    
} Mailbox_Base;

/**
 * @brief
 *  Mailbox Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 *
 */
typedef struct Mailbox_HwCfg_t
{
    /**
     * @brief  Register and memory base for localEndpoint to remoteEndpoint communication.
     */
    Mailbox_Base       baseLocalToRemote;
    /**
     * @brief  Register and memory base for remoteEndpoint to localEndpoint communication.
     */
    Mailbox_Base       baseRemoteToLocal;
    /**
     * @brief  Mailbox Full Interrupt Number
     */
    uint32_t           boxFullIntNum;
    /**
     * @brief  Mailbox Empty Interrupt Number
     */
    uint32_t           boxEmptyIntNum;
}Mailbox_HwCfg;

/**
 * @brief
 *  Mailbox Driver
 *
 * @details
 *  The structure describes the Mailbox Driver and tracks all the information
 *  relevant to the driver instance
 *
 */
typedef struct Mailbox_Driver_t
{
    /**
     * @brief   Mailbox Configuration which was used to instantiate the driver
     */
    Mailbox_Config        cfg;
    /**
     * @brief   Local type of Mailbox
     */
    Mailbox_Type          localEndpoint;  
    /**
     * @brief  Remote type of Mailbox
     */
    Mailbox_Type          remoteEndpoint;       
    /**
     * @brief  Handle to the Read semaphore.
     */
    SemaphoreP_Handle     readSem;
    /**
     * @brief  Handle to the Write semaphore.
     */
    SemaphoreP_Handle     writeSem;    
    /**
     * @brief   Status of TX box (full or empty)
     */
    Mailbox_TxBoxStatus   txBoxStatus;  
    /**
     * @brief   Number of TX messages successfully sent (acknowledgement has been received)
     */
    uint16_t              txCount;  
    /**
     * @brief   Number of RX messages received
     */
    uint16_t              rxCount;  
    /**
     * @brief   Flag used for read polling mode. Indicate if a new message has arrived. 
     */
    Mailbox_NewMsgFlag    newMessageFlag;
    /**
     * @brief   Number of bytes already read by past calls to the read API in current message
     */
    /*Code review: This variable can be 16bit, however it is used in expressions with 32bit variables. For simplicity it is 32bit.*/ 
    uint32_t              numBytesRead;
    /**
     * @brief  Pointer to hardware configuration.
     */
    Mailbox_HwCfg         *hwCfg;    
    /**
     * @brief   Number of full box isr received
     */
    uint16_t              boxFullIsrCount;  
    /**
     * @brief   Number of empty box isr received
     */
    uint16_t              boxEmptyIsrCount;  
    /**
     * @brief   Number of times the readFlush API was called.
     */
    uint16_t              readFlushCount;  
}Mailbox_Driver;

/*!
 *  @brief      Mailbox ISR error counters
 *
 *  This Structure contains the error counters for all ISRs
 */
typedef struct
{
    /**
     * @brief  BSS mailbox full ISR
     */
    uint16_t bssFull;   
    /**
     * @brief  BSS mailbox empty ISR
     */
    uint16_t bssEmpty;   
    /**
     * @brief  MSS or DSS mailbox full ISR
     */
    uint16_t mssDssFull;   
    /**
     * @brief  MSS or DSS mailbox empty ISR
     */
    uint16_t mssDssEmpty;       
} Mailbox_ISRErrorCnt_t;

/*!
 *  @brief      Mailbox Interrupt Handles
 *
 *  This Structure contains the mailbox interrupt handles for all ISRs
 */
typedef struct
{
    /**
     * @brief  Registered Mailbox Full Interrupt Handler to communicate with BSS.
     */
    HwiP_Handle bssFull;   
    /**
     * @brief  Registered Mailbox empty Interrupt Handler to communicate with BSS.
     */
    HwiP_Handle bssEmpty;   
    /**
     * @brief  Registered Mailbox Full Interrupt Handler to communicate with MSS or DSS.
     */
    HwiP_Handle mssDssFull;   
    /**
     * @brief  Registered Mailbox empty Interrupt Handler to communicate with MSS or DSS.
     */
    HwiP_Handle mssDssEmpty;       
} Mailbox_HwiHandles_t;


/*!
 *  @brief      Mailbox MCB
 *
 *  Structure for tracking information required by the mailbox driver
 */
typedef struct Mailbox_MCB_t
{
    /**
     * @brief
     *  Flag that checks if the Driver has been initialized. It can be initialized only once per master (DSS or MSS).
     */
    uint8_t        initFlag;
    /**
     * @brief
     *  Stores the local endpoint which must be the same across all instances of the driver for
     *  a given master.
     */
    Mailbox_Type   localEndpoint;
    /**
     * @brief
     *  Assures that only one instance of the driver is open (for this master) to the BSS
     */
    uint8_t        bssInstCount;
    /**
     * @brief
     *  Counts the number of MSS<->DSS instances of the driver (for this master).
     */
    uint8_t        mssDssInstCount;
    /**
     * @brief
     *  Stores status of read channel. Used to prevent multiple instances of MSS-DSS
     *  mailbox to use the channel at the same time.
     */
    uint8_t        mssDssReadChIDInUse;    
    /**
     * @brief
     *  Stores status of write channel. Used to prevent multiple instances of MSS-DSS
     *  mailbox to use the channel at the same time.
     */
    uint8_t        mssDssWriteChIDInUse;    
    /**
     * @brief
     *  Indicates if multichannel is enabled between MSS<->DSS.
     *  All mailbox instances between MSS and DSS must have the same channel type configuration.
     */
    Mailbox_ChType mssDssChType;
    /**
     * @brief
     *  Stores the handle of the driver instance for the last message that was sent. 
     *  Used during ISR to determine which driver received the acknowledgement as ISR is received without message. 
     *  Used in extended mode for MSS<->DSS communication only.
     */
    Mbox_Handle lastMsgSentHandle;
    /**
     * @brief
     *  Stores a pointer for the hardware configuration used by the MSS<->DSS communication. 
     */
    Mailbox_HwCfg* mssDssHwCfgPtr;    
    /**
     * @brief   Registered Mailbox Interrupt Handlers
     */
    Mailbox_HwiHandles_t hwiHandles;
    /**
     * @brief
     *  Store ISR error conditions.
     */
    Mailbox_ISRErrorCnt_t errCnt; 
    /**
     * @brief
     *  Array that stores handles for all mailbox instances. Used in extended mode for MSS<->DSS communication only.
     */
    Mbox_Handle handleArray[MAILBOX_CH_ID_MAX + 1];
} Mailbox_MCB;



/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_Mailbox_internal__include */
