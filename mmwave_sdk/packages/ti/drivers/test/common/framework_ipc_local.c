/*
 *   @file  framework_ipc_local.c
 *
 *   @brief
 *      This the TestFmk-IPC Local communication functionality.
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
#include <ti/drivers/test/common/framework.h>
#include <ti/drivers/test/common/framework_internal.h>

/**************************************************************************
 ************************ TestFmk-IPC Local Functions *************************
 **************************************************************************/

/* Local IPC Functions: */
static TestFmk_LocalIPCMsg* TestFmk_IPCLocalQueueAllocate (TestFmk_MCB* ptrTestFmk);
static TestFmk_LocalIPCMsg* TestFmk_IPCLocalDequeue (TestFmk_MCB* ptrTestFmk);
static void TestFmk_IPCLocalQueueFree (TestFmk_MCB* ptrTestFmk, TestFmk_LocalIPCMsg* ptrLocalIPCMessage);
static void TestFmk_IPCLocalEnqueue (TestFmk_MCB* ptrTestFmk, TestFmk_LocalIPCMsg* ptrLocalIPCMessage);

/**************************************************************************
 *************************** TestFmk-IPC Functions ****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to allocate a message from the Local IPC Queue.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success  -   Message was allocated
 *  @retval
 *      NULL     -   Unable to allocate the message
 */
static TestFmk_LocalIPCMsg* TestFmk_IPCLocalQueueAllocate (TestFmk_MCB* ptrTestFmk)
{
    TestFmk_LocalIPCMsg*    ptrLocalIPCMessage;
    uintptr_t               key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Allocate a message from the free queue: */
    ptrLocalIPCMessage = (TestFmk_LocalIPCMsg*)TestFmk_listRemove ((TestFmk_ListNode**)&ptrTestFmk->localIPCMessageFreeQueue);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return ptrLocalIPCMessage;
}

/**
 *  @b Description
 *  @n
 *      The function is used to free a message from the Local IPC Queue.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in] ptrLocalIPCMessage
 *      Pointer to the TestFmk Local message to be freed up
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmk_IPCLocalQueueFree (TestFmk_MCB* ptrTestFmk, TestFmk_LocalIPCMsg* ptrLocalIPCMessage)
{
    uintptr_t   key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Add a message to the free queue: */
    TestFmk_listAdd ((TestFmk_ListNode**)&ptrTestFmk->localIPCMessageFreeQueue, (TestFmk_ListNode*)ptrLocalIPCMessage);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to enqueue a message to the Local IPC Queue.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in] ptrLocalIPCMessage
 *      Pointer to the local IPC message to be enqueued
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmk_IPCLocalEnqueue (TestFmk_MCB* ptrTestFmk, TestFmk_LocalIPCMsg* ptrLocalIPCMessage)
{
    uintptr_t   key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Enqueue the message to the pending queue: */
    TestFmk_listCat ((TestFmk_ListNode**)&ptrTestFmk->localIPCMessagePendingQueue, (TestFmk_ListNode**)&ptrLocalIPCMessage);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to dequeue a message to the Local IPC Queue.
 *
 *  @param[in] ptrTestFmk
 *      Pointer to the TestFmk module
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success  -   Dequeued message
 *  @retval
 *      NULL     -   No message was available
 */
static TestFmk_LocalIPCMsg* TestFmk_IPCLocalDequeue (TestFmk_MCB* ptrTestFmk)
{
    TestFmk_LocalIPCMsg*    ptrLocalIPCMessage;
    uintptr_t           key;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Dequeue the message from the pending queue: */
    ptrLocalIPCMessage = (TestFmk_LocalIPCMsg*)TestFmk_listRemove ((TestFmk_ListNode**)&ptrTestFmk->localIPCMessagePendingQueue);

    /* Restore the interrupt: */
    HwiP_restore (key);
    return ptrLocalIPCMessage;
}

/**
 *  @b Description
 *  @n
 *      The function is used to receive a message from the local IPC Module
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
int32_t TestFmk_IPCLocalRecv (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode)
{
    TestFmk_LocalIPCMsg*    ptrLocalIPCMessage;
    int32_t             retVal = MINUS_ONE;

    /* Dequeue a message from the pending queue: */
    ptrLocalIPCMessage = TestFmk_IPCLocalDequeue (ptrTestFmk);
    if (ptrLocalIPCMessage == NULL)
    {
        /* There are no more messages. */
        retVal = 0;
        goto exit;
    }

    /* Copy over the message: */
    memcpy ((void*)ptrMessage, (void*)&ptrLocalIPCMessage->msg, sizeof(TestFmk_Msg));

    /* Cleanup the local message: */
    TestFmk_IPCLocalQueueFree (ptrTestFmk, ptrLocalIPCMessage);

    /* There is one message received: Setup the return value */
    retVal = 1U;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to send a message to the Local IPC Module
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
int32_t TestFmk_IPCLocalSend (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode)
{
    TestFmk_LocalIPCMsg*    ptrLocalMessage;
    int32_t             retVal = MINUS_ONE;

    /* Allocate a message for the local IPC message: */
    ptrLocalMessage = TestFmk_IPCLocalQueueAllocate (ptrTestFmk);
    if (ptrLocalMessage == NULL)
    {
        *errCode = TESTFMK_ENOMEM;
        goto exit;
    }

    /* Copy over the message: */
    memcpy ((void*)&ptrLocalMessage->msg, (void*)ptrMessage, sizeof(TestFmk_Msg));

    /* Add the message to the pending queue: */
    TestFmk_IPCLocalEnqueue (ptrTestFmk, ptrLocalMessage);

    /* Post the TestFmk Semaphore: */
    SemaphoreP_post (ptrTestFmk->semaphoreHandle);

    /* Setup the return value: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the TestFmk Local IPC module.
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
int32_t TestFmk_IPCLocalInit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    uint8_t     index;

    /* Initialize the IPC Local Free Message Queue */
    for (index = 0U; index < TEST_FMK_MAX_LOCAL_QUEUE; index++)
    {
        TestFmk_IPCLocalQueueFree (ptrTestFmk, &ptrTestFmk->localMsgQueue[index]);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the TestFmk Local IPC module.
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
int32_t TestFmk_IPCLocalDeinit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    TestFmk_LocalIPCMsg*    ptrLocalIPCMessage;

    /* Dequeue all the messages from the pending queue and add them back to
     * the free queue. The TestFmk Module is being deinitialized. There should
     * be no more messages which need to be transferred. */
    while (1)
    {
        /* Dequeue a message from the pending queue: */
        ptrLocalIPCMessage = TestFmk_IPCLocalDequeue (ptrTestFmk);
        if (ptrLocalIPCMessage == NULL)
        {
            /* No more messages in the pending queue */
            break;
        }

        /* Add the message back to the free queue */
        TestFmk_IPCLocalQueueFree (ptrTestFmk, ptrLocalIPCMessage);
    }
    return 0;
}

