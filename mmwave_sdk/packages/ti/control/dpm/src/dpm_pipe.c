/**
 *   @file  dpm_pipe.c
 *
 *   @brief
 *      This the DPM Pipe communication functionality which is used for
 *      thread communication on the same domain.
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

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/include/dpm_internal.h>

/**************************************************************************
 *************************** DPM Pipe Functions ***************************
 **************************************************************************/

static DPM_PipeMsg* DPM_pipeQueueAllocate (DPM_MCB* ptrDPM);
static DPM_PipeMsg* DPM_pipeDequeue (DPM_MCB* ptrDPM);
static void DPM_pipeQueueFree (DPM_MCB* ptrDPM, DPM_PipeMsg* ptrPipeMsg);
static void DPM_pipeEnqueue (DPM_MCB* ptrDPM, DPM_PipeMsg* ptrPipeMsg);

/**************************************************************************
 *************************** DPM-IPC Functions ****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to allocate a message
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success  -   Message was allocated
 *  @retval
 *      NULL     -   Unable to allocate the message
 */
static DPM_PipeMsg* DPM_pipeQueueAllocate (DPM_MCB* ptrDPM)
{
    DPM_PipeMsg*    ptrPipeMsg;
    uintptr_t       key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Allocate a message from the free queue: */
    ptrPipeMsg = (DPM_PipeMsg*)DPM_listRemove ((DPM_ListNode**)&ptrDPM->pipeFreeQueue);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return ptrPipeMsg;
}

/**
 *  @b Description
 *  @n
 *      The function is used to free a previously allocated message
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] ptrPipeMsg
 *      Pointer to the pipe message to be freed up
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void DPM_pipeQueueFree (DPM_MCB* ptrDPM, DPM_PipeMsg* ptrPipeMsg)
{
    uintptr_t   key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Add a message to the free queue: */
    DPM_listAdd ((DPM_ListNode**)&ptrDPM->pipeFreeQueue, (DPM_ListNode*)ptrPipeMsg);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to enqueue a message to the IPC Queue.
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[in] ptrPipeMsg
 *      Pointer to the message to be enqueued
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void DPM_pipeEnqueue (DPM_MCB* ptrDPM, DPM_PipeMsg* ptrPipeMsg)
{
    uintptr_t   key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Enqueue the message to the pending queue: */
    DPM_listCat ((DPM_ListNode**)&ptrDPM->pipePendingQueue, (DPM_ListNode**)&ptrPipeMsg);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to dequeue a message to the IPC Queue.
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success  -   Dequeued message
 *  @retval
 *      NULL     -   No message was available
 */
static DPM_PipeMsg* DPM_pipeDequeue (DPM_MCB* ptrDPM)
{
    DPM_PipeMsg*    ptrPipeMsg;
    uintptr_t       key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Dequeue the message from the pending queue: */
    ptrPipeMsg = (DPM_PipeMsg*)DPM_listRemove ((DPM_ListNode**)&ptrDPM->pipePendingQueue);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return ptrPipeMsg;
}

/**
 *  @b Description
 *  @n
 *      The function is used to receive a message from the pipe module
 *
 *  @param[in] ptrDPM
 *      Pointer to the DPM module
 *  @param[out] ptrMessage
 *      Pointer to the DPM message populated by the API
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      0   -   No more message available
 *  @retval
 *      1   -   Message available
 *  @retval
 *      <0  -   Error
 */
int32_t DPM_pipeRecv (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode)
{
    DPM_PipeMsg*    ptrPipeMsg;
    int32_t         retVal = MINUS_ONE;

    /* Dequeue a message from the pending queue: */
    ptrPipeMsg = DPM_pipeDequeue (ptrDPM);
    if (ptrPipeMsg == NULL)
    {
        /* There are no more messages. */
        retVal = 0;
        goto exit;
    }

    /* Copy over the header */
    memcpy ((void*)&ptrMessage->header, (void*)&ptrPipeMsg->msg.header, sizeof(DPM_HeaderMsg));

    /* Does the message have any payload? */
    if (ptrMessage->header.payloadLength != 0U)
    {
        /* YES: Copy the payload */
        memcpy ((void*)&ptrMessage->u.msgPayload,
                (void*)&ptrPipeMsg->msg.u.msgPayload,
                ptrMessage->header.payloadLength);
    }

    /* Free the message: */
    DPM_pipeQueueFree (ptrDPM, ptrPipeMsg);

    /* There is one message received: Setup the return value */
    retVal = 1U;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to send a message to the IPC Module
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
int32_t DPM_pipeSend (DPM_MCB* ptrDPM, DPM_Msg* ptrMessage, int32_t* errCode)
{
    DPM_PipeMsg*    ptrPipeMsg;

    /* Allocate the pipe message: */
    ptrPipeMsg = DPM_pipeQueueAllocate (ptrDPM);

    /* In order to ensure that the provisioning is correct;
     * we should always have a message */
    DebugP_assert (ptrPipeMsg != NULL);

    /* Copy over the header: */
    memcpy ((void*)&ptrPipeMsg->msg.header, (void*)&ptrMessage->header, sizeof(DPM_HeaderMsg));

    /* Does the message have any payload? */
    if (ptrMessage->header.payloadLength != 0U)
    {
        /* YES: Copy the payload */
        memcpy ((void*)&ptrPipeMsg->msg.u.msgPayload,
                (void*)&ptrMessage->u.msgPayload,
                ptrMessage->header.payloadLength);
    }

    /* Add the message to the pending queue: */
    DPM_pipeEnqueue (ptrDPM, ptrPipeMsg);

    /* Post the DPM Semaphore: */
    SemaphoreP_post (ptrDPM->semaphoreHandle);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the DPM Pipe Module
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
int32_t DPM_pipeInit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    uint8_t     index;

    /* Initialize the Message Queue */
    for (index = 0U; index < DPM_MAX_LOCAL_QUEUE; index++)
    {
        DPM_pipeQueueFree (ptrDPM, &ptrDPM->pipeMsgQueue[index]);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the DPM Pipe Module
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
int32_t DPM_pipeDeinit (DPM_MCB* ptrDPM, int32_t* errCode)
{
    DPM_PipeMsg*    ptrPipeMsg;

    /* Dequeue all the messages from the pending queue and add them back to
     * the free queue. The DPM Module is being deinitialized. There should
     * be no more messages which need to be transferred. */
    while (1)
    {
        /* Dequeue a message from the pending queue: */
        ptrPipeMsg = DPM_pipeDequeue (ptrDPM);
        if (ptrPipeMsg == NULL)
        {
            /* No more messages in the pending queue */
            break;
        }

        /* Add the message back to the free queue */
        DPM_pipeQueueFree (ptrDPM, ptrPipeMsg);
    }
    return 0;
}

