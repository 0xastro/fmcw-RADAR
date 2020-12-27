/**
 *   @file  dpm_msg.c
 *
 *   @brief
 *      The file implements the DPM messaging module
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/include/dpm_internal.h>

/**************************************************************************
 ************************ DPM Message Local Functions *********************
 **************************************************************************/
static void    DPM_msgInitResponse(DPM_Msg* ptrResponseMsg,DPM_MessageId reqId, int32_t errCode, uint32_t arg0, uint32_t arg1);
static int32_t DPM_msgPostProcess (DPM_MCB* ptrDPM, DPM_Msg* ptrRxedMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgStartHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgStopHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgDPCCfgHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgBSSFaultHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgAssertHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgResultHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgDPCInfoHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);
static int32_t DPM_msgResponseHandler (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, DPM_Msg* ptrResponseMsg, int32_t* errCode);

/**************************************************************************
 ************************ DPM Message Local Structures ********************
 **************************************************************************/

/**
 * @brief   DPM Message Handler Table:
 */
const DPM_MsgHandler gDPMMessageFxnTable[DPM_MessageId_MAX] =
{
    NULL,                           /* Invalid      : DPM_MessageId_INVALID         */
    DPM_msgStartHandler,            /* Start        : DPM_MessageId_START           */
    DPM_msgStopHandler,             /* Stop         : DPM_MessageId_STOP            */
    DPM_msgDPCCfgHandler,           /* DPC Cfg      : DPM_MessageId_DPC_CFG         */
    DPM_msgBSSFaultHandler,         /* BSS Fault    : DPM_MessageId_BSS_FAULT       */
    DPM_msgAssertHandler,           /* Assert       : DPM_MessageId_DPC_ASSERT      */
    DPM_msgResultHandler,           /* Result       : DPM_MessageId_RESULT          */
    DPM_msgDPCInfoHandler,          /* DPC Info     : DPM_MessageId_DPC_INFO        */
    DPM_msgResponseHandler          /* Response     : DPM_MessageId_RESPONSE        */
};

/**************************************************************************
 *********************** DPM Message Functions ****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Initialize the message header
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM Module
 *  @param[in]  ptrMessage
 *      Pointer to the DPM message to be initialized
 *  @param[in]  msgId
 *      Message Id to be setup
 *  @param[in]  payloadLength
 *      Length of the payload
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void DPM_msgInitHeader
(
    DPM_MCB*        ptrDPM,
    DPM_Msg*        ptrMessage,
    DPM_MessageId   msgId,
    uint32_t        payloadLength
)
{
    /* Initialize the message: */
    memset ((void *)ptrMessage, 0, sizeof(DPM_Msg));

    /* Populate the message header: */
    ptrMessage->header.id            = msgId;
    ptrMessage->header.type          = DPM_MessageType_NORMAL;
    ptrMessage->header.instanceId    = ptrDPM->initCfg.instanceId;
    ptrMessage->header.payloadLength = payloadLength;
}

/**
 *  @b Description
 *  @n
 *      Initialize the message response
 *
 *  @param[in]  ptrResponseMsg
 *      Pointer to the DPM response message
 *  @param[in]  reqId
 *      Request Id which is generating the response
 *  @param[in]  errCode
 *      Error code in the response
 *  @param[in]  arg0
 *      Argument0 in the response
 *  @param[in]  arg1
 *      Argument1 in the response
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void DPM_msgInitResponse
(
    DPM_Msg*        ptrResponseMsg,
    DPM_MessageId   reqId,
    int32_t         errCode,
    uint32_t        arg0,
    uint32_t        arg1
)
{
    ptrResponseMsg->u.responseMsg.requestMsgId = reqId;
    ptrResponseMsg->u.responseMsg.errCode      = errCode;
    ptrResponseMsg->u.responseMsg.arg0         = arg0;
    ptrResponseMsg->u.responseMsg.arg1         = arg1;
}

/**
 *  @b Description
 *  @n
 *      The function handles the post processing of the message
 *      This is domain specific handling.
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrRxedMessage
 *      Pointer to the request message
 *  @param[in]  ptrResponseMsg
 *      Pointer to the response message
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPM_msgPostProcess
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrRxedMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    int32_t     retVal = MINUS_ONE;

    /* Domain Specific Handling: */
    switch (ptrDPM->initCfg.domain)
    {
        case DPM_Domain_LOCALIZED:
        {
            /******************************************************************************
             * Local domain. The Processing Chain & Control are executing on
             * the same domains.
             *
             * Service the response messages. Response messages are sent via the pipe to
             * the local application.
             ******************************************************************************/
            if (ptrResponseMsg->u.responseMsg.requestMsgId != DPM_MessageId_INVALID)
            {
                /* YES: Response message is available. Send it via the pipe */
                retVal = DPM_pipeSend (ptrDPM, ptrResponseMsg, errCode);
            }
            break;
        }
        case DPM_Domain_REMOTE:
        {
            /******************************************************************************
             * Remote domain. The Processing Chain  & Control are executing on
             * different domains.
             *
             * Service the response messages. Response messages are sent via the pipe to
             * the local application and are also relayed to the peer domain. This will
             * ensure that the applications executing on both the domains have been
             * reported with the appropriate information.
             ******************************************************************************/
            if (ptrResponseMsg->u.responseMsg.requestMsgId != DPM_MessageId_INVALID)
            {
                /* YES: Response message is available. Send it via the pipe */
                retVal = DPM_pipeSend (ptrDPM, ptrResponseMsg, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to send out the message */
                    goto exit;
                }

                /* Send out the message using the domain: */
                retVal = ptrDPM->ptrDomainFxnTable->msgSend (ptrDPM, ptrResponseMsg, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to send out the message via the domain */
                    goto exit;
                }
            }

            /*****************************************************************************
             * Do we need to relay the request message? The following are the rules which
             * are enforced:-
             *  - Response message are never relayed
             *  - Result messages are special.
             *  - All other *Request* messages which are *Normal* are relayed
             *
             * Initially all request messages are marked as NORMAL. Once the message is
             * sent from one domain to another we mark it as RELAYED. This is because we
             * dont want the other domain to RELAY the message back. This will then become
             * an endless loop.
             *****************************************************************************/
            if ((ptrRxedMessage->header.id   != DPM_MessageId_RESPONSE)    &&
                (ptrRxedMessage->header.id   != DPM_MessageId_RESULT)      &&
                (ptrRxedMessage->header.type == DPM_MessageType_NORMAL))
            {
                /* YES: Relay the message to the peer domain. */
                ptrRxedMessage->header.type = DPM_MessageType_RELAY;

                /* Send out the message using the domain: */
                retVal = ptrDPM->ptrDomainFxnTable->msgSend (ptrDPM, ptrRxedMessage, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to send out the message via the domain */
                    goto exit;
                }
            }
            else
            {
                /* Control comes here to indicate the following:-
                 * - Received message was a response which was received from the remote domain
                 *   In which case we dont need to relay it back
                 * - Received message was a relayed request which was received from the remote
                 *   domain. In which case we dont need to relay it back. */
            }
            break;
        }
        case DPM_Domain_DISTRIBUTED:
        {
            /******************************************************************************
             * This is a distributed domain. The Processing Chain is distributed
             * and executing on multiple domains. There are 2 fundamental types of
             * messages:-
             * - Response Messages
             * - Request Messages
             *
             * First, service the response messages if any. This is because if we get
             * an error we would like to notify the application before relaying the
             * requests to the remote domain.
             ******************************************************************************/
            if (ptrResponseMsg->u.responseMsg.requestMsgId != DPM_MessageId_INVALID)
            {
                /* YES: Use the pipe to send the message to the local domain */
                retVal = DPM_pipeSend (ptrDPM, ptrResponseMsg, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to send out the message */
                    goto exit;
                }

                /* Use the domain to send the message to the remote domain */
                retVal = ptrDPM->ptrDomainFxnTable->msgSend (ptrDPM, ptrResponseMsg, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to send out the message via the domain */
                    goto exit;
                }
            }
            else
            {
                /* Control comes here to indicate the following:-
                 *  - There is no response message. Response messages are generated only if
                 *    we processed a request message. There is no response message generated
                 *    if we had received a response message. */
            }

            /*****************************************************************************
             * Do we need to relay the request message? The following are the rules which
             * are enforced:-
             *  - Response message are never relayed
             *  - Result messages are special. We have partial/complete results.
             *  - All other *Request* messages which are *Normal* are relayed
             *
             * Initially all request messages are marked as NORMAL. Once the message is
             * sent from one domain to another we mark it as RELAYED. This is because we
             * dont want the other domain to RELAY the message back. This will then become
             * an endless loop.
             *****************************************************************************/
            if ((ptrRxedMessage->header.id   != DPM_MessageId_RESPONSE)    &&
                (ptrRxedMessage->header.id   != DPM_MessageId_RESULT)      &&
                (ptrRxedMessage->header.type == DPM_MessageType_NORMAL))
            {
                /* YES: Relay the message to the peer domain. */
                ptrRxedMessage->header.type = DPM_MessageType_RELAY;

                /* Send out the message using the domain: */
                retVal = ptrDPM->ptrDomainFxnTable->msgSend (ptrDPM, ptrRxedMessage, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to send out the message via the domain */
                    goto exit;
                }
            }
            else
            {
                /* Control comes here to indicate the following:-
                 * - Received message was a response which was received from the remote domain
                 *   In which case we dont need to relay it back
                 * - Received message was a relayed request which was received from the remote
                 *   domain. In which case we dont need to relay it back.
                 * - Result message
                 *
                 * No action is required. */
            }
            break;
        }
        default:
        {
            /* Error: This is an invalid use case and control should have never
             * come here since the parameters have already been validated. */
            DebugP_assert (0);
            *errCode = DPM_EINVAL;
            goto exit;
        }
    }

    /* Message was successfully sent: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the Start message
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the message
 *  @param[out]  ptrResponseMsg
 *      Pointer to the response message
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_msgStartHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    int32_t     retVal;

    /* Is the DPM Instance responsible for the profile? */
    if (ptrDPM->dpcHandle != NULL)
    {
        /* YES: Start the profile through the registered handler */
        retVal = ptrDPM->procChainCfg.startFxn (ptrDPM->dpcHandle);

        /* Setup the response message: */
        DPM_msgInitHeader (ptrDPM, ptrResponseMsg, DPM_MessageId_RESPONSE, sizeof(DPM_Response));
        DPM_msgInitResponse (ptrResponseMsg, DPM_MessageId_START, retVal, 0U, 0U);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the Stop message
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the message
 *  @param[out]  ptrResponseMsg
 *      Pointer to the response message
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_msgStopHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    int32_t     retVal;

    /* Is the DPM Instance responsible for the profile? */
    if (ptrDPM->dpcHandle != NULL)
    {
        /* YES: Stop the profile through the registered handler */
        retVal = ptrDPM->procChainCfg.stopFxn (ptrDPM->dpcHandle);

        /* Setup the response message: */
        DPM_msgInitHeader (ptrDPM, ptrResponseMsg, DPM_MessageId_RESPONSE, sizeof(DPM_Response));
        DPM_msgInitResponse (ptrResponseMsg, DPM_MessageId_STOP, retVal, 0U, 0U);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the Profile IOCTL message
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the message
 *  @param[out]  ptrResponseMsg
 *      Pointer to the response message
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_msgDPCCfgHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    int32_t     retVal = 0;
    bool        bSendResponse = true;

    /* Is the DPM Instance responsible for the profile? */
    if (ptrDPM->dpcHandle == NULL)
    {
        /* NO: In this case we are done. */
        goto exit;
    }

    /* YES: Pass the message to the profile control function */
    retVal = ptrDPM->procChainCfg.ioctlFxn (ptrDPM->dpcHandle,
                                            ptrMessage->u.dpcCfgMsg.cmd,
                                            &ptrMessage->u.dpcCfgMsg.data[0],
                                            ptrMessage->u.dpcCfgMsg.cmdLen);

    /* Did we get an error? */
    if (retVal < 0)
    {
        /* Profile configuration failed: Was this because an invalid command? */
        if (retVal == DPM_EINVCMD)
        {
            /* Are we executing in distributed domain mode? */
            if (ptrDPM->initCfg.domain == DPM_Domain_DISTRIBUTED)
            {
                /* YES: In the case of distributed domains the DPM will relay
                 * the control messages to the various domains. This error is
                 * not a fatal error and should be ignored. */
                 bSendResponse = false;
                 retVal        = 0;
            }
            else
            {
                /* NO: In the case of local/remote domains this is an indication
                 * that the application passed an invalid command to the profile
                 * Please refer to the profile documentation for more information
                 * We need to report this to the application through the reporting
                 * mechanism. */
            }
        }
    }

    /* Do we need to send a response message? */
    if (bSendResponse == true)
    {
        /* Setup the response message: */
        DPM_msgInitHeader (ptrDPM, ptrResponseMsg, DPM_MessageId_RESPONSE, sizeof(DPM_Response));
        DPM_msgInitResponse (ptrResponseMsg, DPM_MessageId_DPC_CFG, retVal,
                             ptrMessage->u.dpcCfgMsg.cmd, 0U);
        ptrResponseMsg->u.responseMsg.dpcCfgMsg.cmd    = ptrMessage->u.dpcCfgMsg.cmd;
        ptrResponseMsg->u.responseMsg.dpcCfgMsg.cmdLen = ptrMessage->u.dpcCfgMsg.cmdLen;
        memcpy ((void*)&ptrResponseMsg->u.responseMsg.dpcCfgMsg.data[0],
                (void*)&ptrMessage->u.dpcCfgMsg.data[0],
                ptrResponseMsg->u.responseMsg.dpcCfgMsg.cmdLen);
    }

exit:
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the mmWave link reported
 *      BSS Fault message
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the message
 *  @param[out] ptrResponseMsg
 *      Not populated
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_msgBSSFaultHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    /* Is the DPM Instance responsible for the profile? BSS Fault has been detected.
     * So pass this along to the registered profile to take appropriate action. */
    if (ptrDPM->dpcHandle != NULL)
    {
        /* YES: Pass the message to the profile control function */
        (void)ptrDPM->procChainCfg.ioctlFxn (ptrDPM->dpcHandle,
                                             DPM_CMD_BSS_FAULT,
                                             NULL,
                                             0);

        /* NOTE: There is no response message generated for the BSS Fault. */
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the profile assert
 *      message
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the message
 *  @param[out] ptrResponseMsg
 *      Not populated
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_msgAssertHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    /* Inform all the DPM Instances and applications that the profile has detected a FAULT */
    ptrDPM->initCfg.reportFxn (DPM_Report_DPC_ASSERT,
                               ptrMessage->header.instanceId,
                               0,
                               (uint32_t)&ptrMessage->u.assertMsg,
                               0U);

    /* NOTE: There is no response message generated for profile assertion. */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the profile assert
 *      message
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the message
 *  @param[out] ptrResponseMsg
 *      Pointer to the response message
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_msgResultHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    int32_t retVal = 0;

    /*********************************************************************************
     * Result Messages:
     * - Handling of these messages is domain specific
     *********************************************************************************/
    if (ptrDPM->initCfg.domain == DPM_Domain_LOCALIZED)
    {
        /* Local Domain: The control and data path profile are all executing in the
         * same domain. The results do not need to be sent out. Control should never
         * come here since the validations are already done in the context of the
         * sendResult API. */
         DebugP_assert (0);
    }

    if (ptrDPM->initCfg.domain == DPM_Domain_REMOTE)
    {
        /* Remote Domain: The results have been received from the domain executing
         * the data path. Is the DPM Instance executing the profile? */
        if (ptrDPM->dpcHandle == NULL)
        {
            /* NO: The DPM Instance is not executing the profile so we report the availability
             * of the profile results. For the DPM Instance executing the profile the results
             * are available after invoking the *execute* method. */
             ptrDPM->initCfg.reportFxn (DPM_Report_NOTIFY_DPC_RESULT,
                                        ptrMessage->header.instanceId,
                                        0,
                                        (uint32_t)&ptrMessage->u.resultInfoMsg.result,
                                        (uint32_t)DPM_MessageType_NORMAL);

            /* Do we need to send an acknowlegment back to the peer? */
            if (ptrMessage->u.resultInfoMsg.isAckNeeded == true)
            {
                /* Setup the response message: */
                DPM_msgInitHeader (ptrDPM, ptrResponseMsg, DPM_MessageId_RESPONSE, sizeof(DPM_Response));
                DPM_msgInitResponse (ptrResponseMsg, DPM_MessageId_RESULT, 0,
                                    (uint32_t)&ptrMessage->u.resultInfoMsg.result,
                                    (uint32_t)DPM_MessageType_NORMAL);
            }
        }
        else
        {
            /* Results are received on the domain executing the profile. The results are immediately
             * available after the invocation to the execute API. The only reason why we can land up
             * here is that an application is using the sendResult from the control domain. We throw
             * an assertion here to catch this condition to indicate application misbehavior. */
            DebugP_assert (0);
        }
    }

    if (ptrDPM->initCfg.domain == DPM_Domain_DISTRIBUTED)
    {
        /* Distributed Domain: In a distributed domain model; there are two types of
         * results which could have been exchanged. Intermediate results which are simply
         * relayed from one data path to another. Complete results which need to
         * be reported to the application */
        if (ptrMessage->header.type == DPM_MessageType_RELAY)
        {
            /* YES: Do we have a valid profile executing on the DPM Instance? */
            if (ptrDPM->dpcHandle != NULL)
            {
                /* YES: Does the profile support injection of data? */
                if (ptrDPM->procChainCfg.injectDataFxn == NULL)
                {
                    /* Error: This feature is not supported by the profile. */
                    DPM_msgInitHeader (ptrDPM, ptrResponseMsg, DPM_MessageId_RESPONSE, sizeof(DPM_Response));
                    DPM_msgInitResponse (ptrResponseMsg, DPM_MessageId_RESULT, DPM_ENOTSUP,
                                        (uint32_t)&ptrMessage->u.resultInfoMsg.result,
                                        (uint32_t)DPM_MessageType_RELAY);
                }
                else
                {
                    /* Immediately pass the data to the profile: */
                    retVal = ptrDPM->procChainCfg.injectDataFxn (ptrDPM->dpcHandle,
                                                                 (DPM_Buffer*)&ptrMessage->u.resultInfoMsg.result);
                    if (retVal < 0)
                    {
                        *errCode = retVal;
                    }
                }
            }
            else
            {
                /* NO: Valid Profile Instantiated. This is an invalid use case since relaying of results
                 * is only supported in a distributed domain. */
                DPM_msgInitHeader (ptrDPM, ptrResponseMsg, DPM_MessageId_RESPONSE, sizeof(DPM_Response));
                DPM_msgInitResponse (ptrResponseMsg, DPM_MessageId_RESULT, DPM_EINVAL,
                                    (uint32_t)&ptrMessage->u.resultInfoMsg.result,
                                    (uint32_t)DPM_MessageType_RELAY);
            }
        }
        else
        {
            /* Complete results are passed to the application via the reporting mechanism */
            ptrDPM->initCfg.reportFxn (DPM_Report_NOTIFY_DPC_RESULT,
                                       ptrMessage->header.instanceId,
                                       0,
                                       (uint32_t)&ptrMessage->u.resultInfoMsg.result,
                                       (uint32_t)DPM_MessageType_NORMAL);
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the profile information
 *      message
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the message
 *  @param[out] ptrResponseMsg
 *      Pointer to the response message which is not populated
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Always returns 0
 */
static int32_t DPM_msgDPCInfoHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    /* Inform all the DPM Instances and applications about the profile information */
    ptrDPM->initCfg.reportFxn (DPM_Report_DPC_INFO,
                               ptrMessage->header.instanceId,
                               0,
                               (uint32_t)&ptrMessage->u.dpcInfoMsg,
                               0U);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered handler which handles the Response message
 *      Response messages are received in lieu of a request. Successful
 *      responses will change the profile status and will ensure that
 *      the DPM instances across domains are synchronized with each other
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[in]  ptrMessage
 *      Pointer to the DPM response message which is to be processed.
 *  @param[out] ptrResponseMsg
 *      Not used
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPM_msgResponseHandler
(
    DPM_MCB*    ptrDPM,
    DPM_Msg*    ptrMessage,
    DPM_Msg*    ptrResponseMsg,
    int32_t*    errCode
)
{
    int32_t     retVal = 0;

    /* Process the response message: */
    switch (ptrMessage->u.responseMsg.requestMsgId)
    {
        case DPM_MessageId_START:
        {
            /* Were there any errors? */
            if (ptrMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile was started successfully */
                ptrDPM->procChainStatus = ptrDPM->procChainStatus | DPM_PROC_CHAIN_STATUS_STARTED;
            }

            /* Profile Start Status: Report this to the application */
            ptrDPM->initCfg.reportFxn (DPM_Report_DPC_STARTED,
                                       ptrMessage->header.instanceId,
                                       ptrMessage->u.responseMsg.errCode,
                                       0U,
                                       0U);
            break;
        }
        case DPM_MessageId_STOP:
        {
            /* Were there any errors? */
            if (ptrMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile was stopped successfully */
                ptrDPM->procChainStatus = ptrDPM->procChainStatus & ~DPM_PROC_CHAIN_STATUS_STARTED;
            }

            /* Profile Stop Status: Report this to the application */
            ptrDPM->initCfg.reportFxn (DPM_Report_DPC_STOPPED,
                                       ptrMessage->header.instanceId,
                                       ptrMessage->u.responseMsg.errCode,
                                       0U,
                                       0U);
            break;
        }
        case DPM_MessageId_DPC_CFG:
        {
            /* Profile Control: Report this to the application */
            ptrDPM->initCfg.reportFxn (DPM_Report_IOCTL,
                                       ptrMessage->header.instanceId,
                                       ptrMessage->u.responseMsg.errCode,
                                       ptrMessage->u.responseMsg.arg0,
                                       (uint32_t)&ptrMessage->u.responseMsg.dpcCfgMsg.data[0]);
            break;
        }
        case DPM_MessageId_RESULT:
        {
            /* Profile Results have been acknowledged by the peer DPM domain: Report this only
             * to the application executing the profile. */
            if (ptrDPM->dpcHandle != NULL)
            {
                ptrDPM->initCfg.reportFxn (DPM_Report_NOTIFY_DPC_RESULT_ACKED,
                                           ptrMessage->header.instanceId,
                                           0,
                                           ptrMessage->u.responseMsg.arg0,
                                           0U);
            }
            break;
        }
        default:
        {
            /* Error: This is an invalid request message. */
            DebugP_assert (0);
            retVal = MINUS_ONE;
            break;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the function which is invoked to receive an IPC message
 *      Messages can be received from either:-
 *          (a) Local: One thread is trying to control the DPM Profile
 *          (b) Remote: One core is trying to control the profile executing
 *              on the remote domain.
 *      Process all the messages
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t DPM_msgRecv (DPM_MCB* ptrDPM, int32_t* errCode)
{
    int32_t     retVal;
    DPM_Msg     rxMsg;
    DPM_Msg     respMsg;
    uint32_t    fxnTableIndex;
    bool        done = false;
    bool        processMessage;

    /* Cycle through until all the messages have been processed. */
    while (done == false)
    {
        /* Initialize the process message flag: */
        processMessage = false;

        /* Initialize the received message: */
        memset ((void *)&rxMsg, 0, sizeof(DPM_Msg));

        /* Read the message from the pipe:
         * - All DPM messages from the Local/Remote or Distributed domains are received
         *   and placed into the pipes. */
        retVal = DPM_pipeRecv (ptrDPM, &rxMsg, errCode);
        if (retVal < 0)
        {
            /* Error: Unable to receive the message: */
            done = true;
        }
        else if (retVal == 0)
        {
            /* There are no more messages to be processed: */
            done = true;
        }
        else
        {
            /* Process the received message: */
            processMessage = true;
        }

        /* Is there a message to be processed? */
        if (processMessage == true)
        {
            /* YES: Initialize the response message. */
            respMsg.u.responseMsg.requestMsgId = DPM_MessageId_INVALID;

            /* Initialize the error code: */
            *errCode = 0;

            /* Sanity Checking: Ensure that the message identifier is within range: */
            DebugP_assert (rxMsg.header.id > DPM_MessageId_INVALID);
            DebugP_assert (rxMsg.header.id < DPM_MessageId_MAX);

            /* Convert the message identifier into a function table index. */
            fxnTableIndex = (uint32_t)(rxMsg.header.id - DPM_MessageId_INVALID);
            DebugP_assert (gDPMMessageFxnTable[fxnTableIndex] != NULL);

            /* Process the message. Not all messages will result in a response message */
            retVal = gDPMMessageFxnTable[fxnTableIndex] (ptrDPM, &rxMsg, &respMsg, errCode);
            if (retVal < 0)
            {
                /* Error: Unable to process the message */
                done = true;
            }
            else
            {
                /* Message Processed: Domain specific Post Processing */
                retVal = DPM_msgPostProcess (ptrDPM, &rxMsg, &respMsg, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to send the message */
                    done = true;
                }
            }
        }
    }
    return retVal;
}

