/**
 *   @file  dpm_mailbox.c
 *
 *   @brief
 *      DPM Remote/Distributed Domain Support through the Mailbox
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
#include <stdint.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/include/dpm_internal.h>

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/

static void DPM_mboxRemoteMailboxCallbackFxn(Mbox_Handle handle,Mailbox_Type peer);

static int32_t DPM_mboxInit (DPM_MCB* ptrDPM, int32_t* errCode);
static int32_t DPM_mboxSynch (DPM_MCB* ptrDPM, int32_t* errCode);
static int32_t DPM_mboxMsgSend (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode);
static int32_t DPM_mboxSendResult(DPM_MCB* ptrDPM, bool isAckNeeded, bool isRelayResult, DPM_Buffer* ptrResult, int32_t* errCode);
static int32_t DPM_mboxDeinit (DPM_MCB* ptrDPM, int32_t* errCode);

/**************************************************************************
 ********************** DPM Mailbox Domain Globals ************************
 **************************************************************************/

/**
 * @brief   Global DPM Remote/Distributed Domain Function Table
 */
DPM_DomainTable gDPMRemoteTable =
{
    &DPM_mboxInit,
    &DPM_mboxSynch,
    &DPM_mboxMsgSend,
    &DPM_mboxSendResult,
    &DPM_mboxDeinit
};

/**
 * @brief   Global DPM MCB: We need to use this since the
 * Mailbox does not allow an optional argument to be passed to the
 * callback function.
 */
DPM_MCB*  gDPM = NULL;

/**************************************************************************
 ********************** DPM Mailbox Domain Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the callback function which is registered with the mailbox
 *      and is invoked when a message is received from the remote peer.
 *
 *  @param[in]  handle
 *      Handle to the Mailbox on which data was received
 *  @param[in]  peer
 *      Peer from which data was received
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
static void DPM_mboxRemoteMailboxCallbackFxn
(
    Mbox_Handle     handle,
    Mailbox_Type    peer
)
{
    int32_t     retVal;
    int32_t     errCode;
    DPM_Msg     message;

    /* Read the header from the mailbox: */
    retVal = Mailbox_read(gDPM->ipcMailboxHandle, (uint8_t*)&message.header, sizeof(DPM_HeaderMsg));

    /* Sanity Checking: We should have always read the header */
    DebugP_assert (retVal == sizeof(DPM_HeaderMsg));

    /* Is there an associated payload? */
    if (message.header.payloadLength != 0U)
    {
        /* YES: Read the payload */
        retVal = Mailbox_read (gDPM->ipcMailboxHandle,
                               (uint8_t*)&message.u.msgPayload,
                               message.header.payloadLength);

        /* Sanity Checking: We should have received the entire payload. */
        DebugP_assert (retVal == message.header.payloadLength);
    }

    /* Flush out the contents of the mailbox to indicate that we are done with the message. This will
     * allow us to receive another message in the mailbox while we process the received message.
     * This will also cause the peer to get unblocked since the message is acked immediately. */
    Mailbox_readFlush (gDPM->ipcMailboxHandle);

    /* Place the message into the pipe: This message will then be picked up and processed in the
     * context of the DPM Task. */
    (void)DPM_pipeSend (gDPM, &message, &errCode);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to send a message to the remote IPC Module
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] ptrMessage
 *      Pointer to the DPM message to be sent
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success   - 0
 *  @retval
 *      Error     - <0
 */
static int32_t DPM_mboxMsgSend (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode)
{
    int32_t     retVal = MINUS_ONE;
    uint32_t    sizeMessage;

    /* Size of the message being sent out:
     * - Header + Payload (if any) */
    sizeMessage = sizeof(DPM_HeaderMsg) + ptrMessage->header.payloadLength;

    /* Send out the message: */
    retVal = Mailbox_write (ptrDPM->ipcMailboxHandle, (const uint8_t *)ptrMessage, sizeMessage);
    if (retVal != sizeMessage)
    {
        /* Error: Unable to send the message. */
        *errCode = retVal;
        goto exit;
    }

    /* Message was sent successfully. */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Device specific function which is used to send the processing
 *      chain results to the remote DPM domain
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] isAckNeeded
 *      Flag which indicates if the results need to be acknowledged
 *  @param[in] isRelayResult
 *      Flag which indicates if the results are being relayed or not
 *  @param[in] ptrResult
 *      Results which need to be sent to the remote domain
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
static int32_t DPM_mboxSendResult
(
    DPM_MCB*    ptrDPM,
    bool        isAckNeeded,
    bool        isRelayResult,
    DPM_Buffer* ptrResult,
    int32_t*    errCode
)
{
    DPM_Msg     message;

    /* Send out the result message to the peer domain */
    DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_RESULT, sizeof(DPM_ResultInfo));

    /* Is this a relayed result? */
    if (isRelayResult == true)
    {
        /* YES: Ovewrite the default message type */
        message.header.type = DPM_MessageType_RELAY;
    }

    message.u.resultInfoMsg.isAckNeeded    = isAckNeeded;
    memcpy ((void*)&message.u.resultInfoMsg.result, (void*)ptrResult, sizeof(DPM_Buffer));
    return DPM_mboxMsgSend (ptrDPM, &message, errCode);
}

/**
 *  @b Description
 *  @n
 *      The function is used to synchronize the DPM Module on the device.
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
static int32_t DPM_mboxSynch (DPM_MCB* ptrDPM, int32_t* errCode)
{
#ifdef SUBSYS_DSS
    return SOC_isMSSOperational (ptrDPM->initCfg.socHandle, errCode);
#else
    return SOC_isDSSOperational (ptrDPM->initCfg.socHandle, errCode);
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the DPM Remote IPC module.
 *
 *  @param[in] ptrDPM
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
static int32_t DPM_mboxInit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    Mailbox_Config  mboxCfg;
    Mailbox_Type    remoteEndpoint;
    int32_t         retVal = MINUS_ONE;

    /* Setup the remote endpoint: */
#ifdef SUBSYS_MSS
    remoteEndpoint = MAILBOX_TYPE_DSS;
#else
    remoteEndpoint = MAILBOX_TYPE_MSS;
#endif

    /*****************************************************************************
     * Open the Mailbox IPC Channel:
     *****************************************************************************/
    Mailbox_Config_init (&mboxCfg);

    /* Setup the configuration: */
    mboxCfg.chType           = MAILBOX_CHTYPE_MULTI;
    mboxCfg.chId             = MAILBOX_CH_ID_6;
    mboxCfg.writeMode        = MAILBOX_MODE_BLOCKING;
    mboxCfg.readMode         = MAILBOX_MODE_CALLBACK;
    mboxCfg.readCallback     = &DPM_mboxRemoteMailboxCallbackFxn;
    ptrDPM->ipcMailboxHandle = Mailbox_open (remoteEndpoint, &mboxCfg, errCode);
    if (ptrDPM->ipcMailboxHandle == NULL)
    {
        /* Error: Unable to open the mailbox */
        goto exit;
    }

    /* Setup the global: This is needed for the mailbox semaphore handle */
    gDPM = ptrDPM;

#ifdef SUBSYS_DSS
    (void)SOC_setDSSLinkState (ptrDPM->initCfg.socHandle, 1U, errCode);
#else
    (void)SOC_setMSSLinkState (ptrDPM->initCfg.socHandle, 1U, errCode);
#endif

    /* Setup the return value: The IPC module has been initialized successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the DPM Remote IPC module.
 *
 *  @param[in] ptrDPM
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
static int32_t DPM_mboxDeinit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    /* Close the mailbox: */
    if (ptrDPM->ipcMailboxHandle != NULL)
    {
        Mailbox_close (ptrDPM->ipcMailboxHandle);
        ptrDPM->ipcMailboxHandle = NULL;
    }

    /* Reset the global handle: */
    gDPM = NULL;

#ifdef SUBSYS_DSS
    return SOC_setDSSLinkState (ptrDPM->initCfg.socHandle, 0U, errCode);
#else
    return SOC_setMSSLinkState (ptrDPM->initCfg.socHandle, 0U, errCode);
#endif
}

