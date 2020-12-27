/**
 *   @file  dpm_internal.h
 *
 *   @brief
 *      Internal header file used by the DPM module.
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
#ifndef DPM_INTERNAL_H
#define DPM_INTERNAL_H

/* mmWave SDK Include Files: */
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/control/dpm/include/dpm_listlib.h>

/** @addtogroup DPM_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief   DPM Processing Chain Status: Flag which indicates that the
 * processing chain has been started.
 */
#define DPM_PROC_CHAIN_STATUS_STARTED              0x1

/**
 * @brief   This is the maximum queue depth of the local IPC queues.
 */
#define DPM_MAX_LOCAL_QUEUE                         4U

/**
 * @brief
 *  Enumeration for DPM Message Identifiers
 *
 * @details
 *  The enumeration describes the message identifers which can be exchanged
 *  between the DPM entities.
 */
typedef enum DPM_MessageId_e
{
    /**
     * @brief   Invalid message identifier:
     */
    DPM_MessageId_INVALID = 0x1,

    /**
     * @brief   Message sent to indicate that the DPC has been started
     */
    DPM_MessageId_START,

    /**
     * @brief   Message sent to indicate that the DPC has been stopped
     */
    DPM_MessageId_STOP,

    /**
     * @brief   DPC configuration command: This is not interpreted by the DPM
     * framework but is passed as is to the processing chain.
     */
    DPM_MessageId_DPC_CFG,

    /**
     * @brief   Message sent to indicate that there was a CPU/ESM Fault reported
     * by the BSS.
     */
    DPM_MessageId_BSS_FAULT,

    /**
     * @brief   Message sent from the DPC to indicate that an assertion was
     * detected.
     */
    DPM_MessageId_DPC_ASSERT,

    /**
     * @brief   Message sent from the DPC to indicate that the results are available
     */
    DPM_MessageId_RESULT,

    /**
     * @brief   Message sent from the DPC to indicate that the custom information
     * needs to be reported to the application
     */
    DPM_MessageId_DPC_INFO,

    /**
     * @brief   Response message
     */
    DPM_MessageId_RESPONSE,

    /**
     * @brief   Maximum Message: Nothing beyond this.
     */
    DPM_MessageId_MAX
}DPM_MessageId;

/**
 * @brief
 *  Message Type enumeration
 *
 * @details
 *  The enumeration describes the message types which can be exchanged
 *  between the DPM entities.
 */
typedef enum DPM_MessageType_e
{
    /**
     * @brief   Normal Message Type:
     */
    DPM_MessageType_NORMAL = 0x1,

    /**
     * @brief   Relayed Message Type: This is used in the distrubuted domain model
     * when messages are relayed by the DPM from one domain to another.
     */
    DPM_MessageType_RELAY
}DPM_MessageType;

/**
 * @brief
 *  DPM Processing Chain configuration message
 *
 * @details
 *  The structure describes the DPC message. DPC messages are not interpreted by
 *  the DPM module. These are simply transported as is to the processing chain.
 *  Interpretation of these messages is done by the DPC itself.
 */
typedef struct DPM_DPCCfg_t
{
    /**
     * @brief   Configuration command:
     */
    uint32_t        cmd;

    /**
     * @brief   Length of the command data:
     */
    uint32_t        cmdLen;

    /**
     * @brief   Data Payload
     */
    uint8_t         data[DPM_MAX_DPC_DATA];
}DPM_DPCCfg;

/**
 * @brief
 *  DPM Result Informational Message
 *
 * @details
 *  The DPM result informational messages are exchanged between the peer DPM domains
 *  and can be used to pass the DPC results between them.
 */
typedef struct DPM_ResultInfo_t
{
    /**
     * @brief   Flag to indicate if the acknowledgment of the result is needed or not.
     */
    bool        isAckNeeded;

    /**
     * @brief   Processing Chain Result:
     */
    DPM_Buffer  result;
}DPM_ResultInfo;

/**
 * @brief
 *  DPM Response Format
 *
 * @details
 *  The structure describes the response. Responses are sent back to the
 *  peer which initiated the DPM IPC Request to indicate the status of
 *  the operation. Applications are notified though the reporting mechanism
 */
typedef struct DPM_Response_t
{
    /**
     * @brief   Response Message to the request
     */
    DPM_MessageId           requestMsgId;

    /**
     * @brief   Error code associated with the response
     */
    int32_t                 errCode;

    /**
     * @brief   Optional Argument 1 associated with the response
     */
    uint32_t                arg0;

    /**
     * @brief   Optional Argument 2 associated with the response
     */
    uint32_t                arg1;

    /**
     * @brief   Response Processing Chain Message Inlined. This is useful to pass
     * DPC specific data between the DPM entities.
     */
    DPM_DPCCfg           dpcCfgMsg;
}DPM_Response;

/**
 * @brief
 *  DPM Header
 *
 * @details
 *  The structure describes the header which is attached to all the
 *  messages which are exchanged within the DPM Module.
 */
typedef struct DPM_HeaderMsg_t
{
    /**
     * @brief   Message Identifier
     */
    DPM_MessageId       id;

    /**
     * @brief   Message Type Property: This is used in distributed domain
     */
    DPM_MessageType     type;

    /**
     * @brief   Instance identifier populating the message
     */
    uint32_t            instanceId;

    /**
     * @brief   Length of the payload:
     */
    uint32_t            payloadLength;
}DPM_HeaderMsg;

/**
 * @brief
 *  DPM Message Format
 *
 * @details
 *  The structure describes the DPM message format
 */
typedef struct DPM_Msg_t
{
    /**
     * @brief   DPM Message Header:
     */
    DPM_HeaderMsg       header;

    /**
     * @brief   Message Payload: This is specific to the message type
     */
    union
    {
        /**
         * @brief   Starting of the message payload: Alias used
         */
        uint8_t                 msgPayload;

        /**
         * @brief   Response Message sent back to indicate the status of the operation
         */
        DPM_Response            responseMsg;

        /**
         * @brief   Processing Chain Specific Message:
         */
        DPM_DPCCfg              dpcCfgMsg;

        /**
         * @brief   Processing Chain Assert:
         */
        DPM_DPCAssert           assertMsg;

        /**
         * @brief   Processing chain *custom* informational message:
         */
        DPM_DPCInfo             dpcInfoMsg;

        /**
         * @brief   Processing chain result message:
         */
        DPM_ResultInfo          resultInfoMsg;
    }u;
}DPM_Msg;

/**
 * @brief
 *  DPM HW Attributes
 *
 * @details
 *  This the DPM SOC HW Attributes which will allow the module to be ported
 */
typedef struct DPM_HWAttribute_t
{
    /**
     * @brief   Chirp Available System Interrupt
     */
    uint32_t        chirpAvailableSysInterrupt;

    /**
     * @brief   Frame Start System Interrupt
     */
    uint32_t        frameStartSysInterrupt;
}DPM_HWAttribute;

/**
 * @brief
 *  DPM Pipe Message
 *
 * @details
 *  The structure is used to exchange messages over the DPM Pipe module
 */
typedef struct DPM_PipeMsg_t
{
    /**
     * @brief   Links to other messages
     */
    DPM_ListNode        links;

    /**
     * @brief   Message which is being exchanged
     */
    DPM_Msg             msg;
}DPM_PipeMsg;

/**
 * @brief
 *  DPM Master control block
 *
 * @details
 *  The structure is used to hold all the relevant information required to
 *  execute the DPM module
 */
typedef struct DPM_MCB_t
{
    /**
     * @brief   This is the configuration which was used to initialize the module.
     */
    DPM_InitCfg                     initCfg;

    /**
     * @brief   Loaded processing chain configuration
     */
    DPM_ProcChainCfg                procChainCfg;

    /**
     * @brief   Processing Chain Handle
     */
    DPM_DPCHandle                   dpcHandle;

    /**
     * @brief   Chirp Available System Interrupt Listener Handle:
     */
    SOC_SysIntListenerHandle        chirpAvailableListenerHandle;

    /**
     * @brief   Frame Start System Interrupt Listener Handle:
     */
    SOC_SysIntListenerHandle        frameStartListenerHandle;

    /**
     * @brief   Status of the processing chain
     */
    uint32_t                        procChainStatus;

    /**
     * @brief   Semaphore Handle: This is the main semaphore handle which is posted:
     * - Injection of data
     * - Reception of DPM IPC Messages
     */
    SemaphoreP_Handle               semaphoreHandle;

    /**
     * @brief   Flag set to true which indicates that the DPC has notified the DPM
     * module to invoke the execute method.
     */
    bool                            executeDPC;

    /**
     * @brief   Remote IPC Mailbox Handle: This is used by the IPC module to communicate
     * with the remote peer to exchange DPM IPC messages.
     */
    Mbox_Handle                     ipcMailboxHandle;

    /**
     * @brief   DPM Domain Table
     */
    struct DPM_DomainTable_t*       ptrDomainFxnTable;

    /**
     * @brief   Pipe Message Queue: These are the messages which are exchanged by
     * by the DPM for thread communication
     */
    DPM_PipeMsg                     pipeMsgQueue[DPM_MAX_LOCAL_QUEUE];

    /**
     * @brief   Pipe Message Pending Queue: This is the queue of all the messages
     * which have been posted but have still not been processed by the Pipe module
     */
    DPM_PipeMsg*                    pipePendingQueue;

    /**
     * @brief   Pipe Free Message Queue: This is the queue of all the messages
     * which are free. These messages are only used by the Pipe module.
     */
    DPM_PipeMsg*                    pipeFreeQueue;
}DPM_MCB;

/**
 *  @b Description
 *  @n
 *      Message Handler Function Prototype
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM Module
 *  @param[in]  ptrMessage
 *      Pointer to the received message
 *  @param[out] ptrResponseMsg
 *      Pointer to the response message generated if any
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
typedef int32_t (*DPM_MsgHandler) (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Domain Specific Initialization Function:
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM MCB
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
typedef int32_t (*DPM_domainInit) (DPM_MCB* ptrDPM, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Domain Specific Synchronization
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM MCB
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
typedef int32_t (*DPM_domainSynch) (DPM_MCB* ptrDPM, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Domain Specific Deinitialization Function:
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM MCB
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
typedef int32_t (*DPM_domainDeinit) (DPM_MCB* ptrDPM, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Domain Specific Message Send
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM MCB
 *  @param[in]  ptrMessage
 *      Pointer to the message to be sent
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
typedef int32_t (*DPM_domainMsgSend) (DPM_MCB*  ptrDPM,
                                      DPM_Msg*  ptrMessage,
                                      int32_t*  errCode);

/**
 *  @b Description
 *  @n
 *      Domain Specific Message to exchange the buffers from one domain
 *      to another.
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM MCB
 *  @param[in]  isAckNeeded
 *      Flag which indicates if the buffers need to be acked or not
 *  @param[in]  isRelayRequest
 *      Flag which indicates that the buffers are been relayed in
 *      distributed mode and whether the application should be notified via
 *      the reporting mechaism
 *  @param[in]  ptrBuffer
 *      Pointer to the DPM buffer
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
typedef int32_t (*DPM_domainMsgSendResult) (DPM_MCB*    ptrDPM,
                                            bool        isAckNeeded,
                                            bool        isRelayResult,
                                            DPM_Buffer* ptrBuffer,
                                            int32_t*    errCode);

/**
 * @brief
 *  DPM Domain Function Table
 *
 * @details
 *  DPM domain
 */
typedef struct DPM_DomainTable_t
{
    /**
     * @brief   Domain specific initialization
     */
    DPM_domainInit              init;

    /**
     * @brief   Domain specific synchronization
     */
    DPM_domainSynch             synch;

    /**
     * @brief   Domain specific message send
     */
    DPM_domainMsgSend           msgSend;

    /**
     * @brief   Domain specific send result
     */
    DPM_domainMsgSendResult     sendResult;

    /**
     * @brief   Domain specific deinitialization
     */
    DPM_domainDeinit            deinit;
}DPM_DomainTable;

/**
@}
*/

/**************************************************************************
 * Global Declarations:
 **************************************************************************/
extern DPM_HWAttribute      gDPMHwAttrib;
extern SemaphoreP_Handle    gDPMSemaphoreHandle;
extern DPM_DomainTable      gDPMLocalTable;
extern DPM_DomainTable      gDPMRemoteTable;

/* Msg Module: */
extern void    DPM_msgInitHeader (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_MessageId msgId,uint32_t payloadLength);
extern int32_t DPM_msgRecv (DPM_MCB* ptrDPM, int32_t* errCode);

/* Pipe Module: */
extern int32_t DPM_pipeInit   (DPM_MCB* ptrDPM, int32_t* errCode);
extern int32_t DPM_pipeSend   (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode);
extern int32_t DPM_pipeRecv   (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode);
extern int32_t DPM_pipeDeinit (DPM_MCB* ptrDPM, int32_t* errCode);

#endif /* DPM_INTERNAL_H */

