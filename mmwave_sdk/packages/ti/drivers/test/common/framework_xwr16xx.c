/**
 *   @file  framework_xwr16xx.c
 *
 *   @brief
 *      Framework Ported Layer for the XWR16xx Platform
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
#include <ti/drivers/test/common/framework.h>
#include <ti/drivers/test/common/framework_internal.h>

/**************************************************************************
 *********************** Global Platform Functions ************************
 **************************************************************************/

/**
 * @brief
 *  This is the TestFmk Hardware Attributes ported for the XWR14xx
 */
TestFmk_HWAttribute  gTestFmkHwAttrib =
{
#ifdef SUBSYS_DSS
    SOC_XWR16XX_DSS_INTC_EVENT_CHIRP_AVAIL,
    SOC_XWR16XX_DSS_INTC_EVENT_FRAME_START
#else
    SOC_XWR16XX_MSS_CHIRP_AVAIL_IRQ,
    SOC_XWR16XX_MSS_FRAME_START_INT
#endif
};

/**
 * @brief   Global TestFmk Semaphore Handle: We need to use this since the
 * Mailbox does not allow an optional argument to be passed to the
 * callback function.
 */
SemaphoreP_Handle   gTestFmkSemaphoreHandle = NULL;

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/

static void TestFmk_IPCRemoteMailboxCallbackFxn(Mbox_Handle handle,Mailbox_Type peer);

/**************************************************************************
 ********************** TestFmk-IPC Platform Functions ************************
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
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
static void TestFmk_IPCRemoteMailboxCallbackFxn
(
    Mbox_Handle  handle,
    Mailbox_Type    peer
)
{
    /* Message has been received from the peer endpoint. */
    SemaphoreP_post (gTestFmkSemaphoreHandle);
}

/**
 *  @b Description
 *  @n
 *      The function is used to receive a message from the remote IPC Module
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[out] ptrMessage
 *      Pointer to the TestFmk message populated by the API
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      0   -   No more message available
 *  @retval
 *      1   -   Message available
 *  @retval
 *      <0  -   Error
 */
int32_t TestFmk_IPCRemoteRecv (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode)
{
    int32_t     retVal;

    /* Read the message from the remote mailbox: */
    retVal = Mailbox_read(ptrTestFmk->ipcMailboxHandle, (uint8_t*)ptrMessage, sizeof(TestFmk_Msg));
    if (retVal < 0)
    {
        /* Error: Unable to read the message.  */
        goto exit;
    }
    if (retVal == 0)
    {
        /* We are done: There are no messages available */
        goto exit;
    }
    else
    {
        /* Flush out the contents of the mailbox to indicate that we are done with the message. This will
         * allow us to receive another message in the mailbox while we process the received message. */
        Mailbox_readFlush (ptrTestFmk->ipcMailboxHandle);
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to send a message to the remote IPC Module
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in] ptrMessage
 *      Pointer to the TestFmk message to be sent
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success   - 0
 *  @retval
 *      Error     - <0
 */
int32_t TestFmk_IPCRemoteSend (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode)
{
    int32_t     retVal;

    /* Send out the message: */
    retVal = Mailbox_write (ptrTestFmk->ipcMailboxHandle, (const uint8_t *)ptrMessage, sizeof(TestFmk_Msg));
    if (retVal != sizeof(TestFmk_Msg))
    {
        /* Error: Unable to send the message. */
        *errCode = retVal;
        retVal   = MINUS_ONE;
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
 *      The function is used to initialize the TestFmk Remote IPC module.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_IPCRemoteInit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
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
    mboxCfg.chType       = MAILBOX_CHTYPE_MULTI;
    mboxCfg.chId         = MAILBOX_CH_ID_0;
    mboxCfg.writeMode    = MAILBOX_MODE_BLOCKING;
    mboxCfg.readMode     = MAILBOX_MODE_CALLBACK;
    mboxCfg.readCallback = &TestFmk_IPCRemoteMailboxCallbackFxn;
    ptrTestFmk->ipcMailboxHandle = Mailbox_open (remoteEndpoint, &mboxCfg, errCode);
    if (ptrTestFmk->ipcMailboxHandle == NULL)
    {
        /* Error: Unable to open the mailbox */
        goto exit;
    }

    /* Setup the global semaphore handle: This is needed for the mailbox callback function */
    gTestFmkSemaphoreHandle = ptrTestFmk->semaphoreHandle;

    /* Setup the return value: The IPC module has been initialized successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the TestFmk Remote IPC module.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_IPCRemoteDeinit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    /* Close the mailbox: */
    if (ptrTestFmk->ipcMailboxHandle != NULL)
    {
        Mailbox_close (ptrTestFmk->ipcMailboxHandle);
        ptrTestFmk->ipcMailboxHandle = NULL;
    }

    /* Reset the global semaphore handle: */
    gTestFmkSemaphoreHandle = NULL;
    return 0;
}

