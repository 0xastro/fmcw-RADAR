/*
 *   @file  framework_ipc.c
 *
 *   @brief
 *      This the TestFmk-IPC communication functionality. The module
 *      is responsible for the TestFmk message exchanges between the
 *      following:-
 *         - Local:  Multiple threads on the same core
 *         - Remote: MSS & DSS on XWR16xx/XWR18xx/XWR68xx
 *
 *      Local IPC is common both XWR1xx while Remote IPC is
 *      platform specific
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
 ************************ TestFmk-IPC Local Functions *********************
 **************************************************************************/

static int32_t TestFmk_IPCResponseMsgHandler (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrResponseMessage, int32_t* errCode);
static int32_t TestFmk_IPCProcessMsg (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode);

/**************************************************************************
 *************************** TestFmk-IPC Functions ****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is a utility function used to initialize the TestFmk IPC
 *      message
 *
 *  @param[in]  ptrMessage
 *      Pointer to the TestFmk message to be initialized
 *  @param[in]  msgType
 *      Message Type to be setup
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void TestFmk_IPCMsgInit (TestFmk_Msg* ptrMessage, TestFmk_MessageType msgType)
{
    /* Initialize the message: */
    memset ((void *)ptrMessage, 0, sizeof(TestFmk_Msg));

    /* Populate the message type: */
    ptrMessage->msgType = msgType;
    return;
}

/**
 *  @b Description
 *  @n
 *      This is the notify function which is invoked to send a message
 *      to the remote peer
 *
 *  @param[in]  ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in]  ptrMessage
 *      Pointer to the message to be sent
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
int32_t TestFmk_IPCSend (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode)
{
    int32_t     retVal;

    /* Send out the message via the Local IPC */
    retVal = TestFmk_IPCLocalSend (ptrTestFmk, ptrMessage, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to send out the message to the local IPC Module. */
        goto exit;
    }

    /* Send out the message via the Remote IPC */
    retVal = TestFmk_IPCRemoteSend (ptrTestFmk, ptrMessage, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to send out the message to the remote IPC Module. */
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
 *      The function is used to process the response message. Response messages
 *      are received in lieu of a request. Successful responses will change the
 *      profile status and will ensure that the TestFmk instances are synchronized
 *      with each other
 *
 *  @param[in]  ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in]  ptrResponseMessage
 *      Pointer to the TestFmk response message which is to be processed.
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
static int32_t TestFmk_IPCResponseMsgHandler
(
    TestFmk_MCB*    ptrTestFmk,
    TestFmk_Msg*    ptrResponseMessage,
    int32_t*        errCode
)
{
    int32_t     retVal = 0;

    /* Process the response message: */
    switch (ptrResponseMessage->u.responseMsg.requestMsgType)
    {
        case TestFmk_MessageType_LOAD:
        {
            /* Were there any errors? */
            if (ptrResponseMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile was loaded successfully */
                ptrTestFmk->profileStatus = ptrTestFmk->profileStatus | TEST_FMK_PROFILE_STATUS_LOADED;
            }

            /* Profile Load Status: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_PROFILE_LOADED,
                                       ptrResponseMessage->u.responseMsg.errCode,
                                       0U,
                                       0U);
            break;
        }
        case TestFmk_MessageType_OPEN:
        {
            /* Were there any errors? */
            if (ptrResponseMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile has been opened successfully */
                ptrTestFmk->profileStatus = ptrTestFmk->profileStatus | TEST_FMK_PROFILE_STATUS_OPENED;
            }

            /* Profile opened status: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_PROFILE_OPENED,
                                       ptrResponseMessage->u.responseMsg.errCode,
                                       0U,
                                       0U);
            break;
        }
        case TestFmk_MessageType_CLOSE:
        {
            /* Were there any errors? */
            if (ptrResponseMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile has been closed successfully */
                ptrTestFmk->profileStatus = ptrTestFmk->profileStatus & ~TEST_FMK_PROFILE_STATUS_OPENED;
            }

            /* Profile opened status: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_PROFILE_CLOSED,
                                       ptrResponseMessage->u.responseMsg.errCode,
                                       0U,
                                       0U);
            break;
        }
        case TestFmk_MessageType_SET_TESTFMK_CFG:
        {
            /* Were there any errors? */
            if (ptrResponseMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile has been configured successfully */
                ptrTestFmk->profileStatus = ptrTestFmk->profileStatus | TEST_FMK_PROFILE_STATUS_CONFIGURED;
            }

            /* Profile Finalized Configuration: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_SET_CFG,
                                           ptrResponseMessage->u.responseMsg.errCode,
                                           (uint32_t)&ptrTestFmk->testfmkCfg,
                                           0U);
            break;
        }
        case TestFmk_MessageType_START:
        {
            /* Were there any errors? */
            if (ptrResponseMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile was started successfully */
                ptrTestFmk->profileStatus = ptrTestFmk->profileStatus | TEST_FMK_PROFILE_STATUS_STARTED;
            }

            /* Profile Start Status: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_PROFILE_STARTED,
                                           ptrResponseMessage->u.responseMsg.errCode,
                                           0U,
                                           0U);
            break;
        }
        case TestFmk_MessageType_STOP:
        {
            /* Were there any errors? */
            if (ptrResponseMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile was stopped successfully */
                ptrTestFmk->profileStatus = ptrTestFmk->profileStatus & ~TEST_FMK_PROFILE_STATUS_STARTED;
            }

            /* Profile Stop Status: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_PROFILE_STOPPED,
                                       ptrResponseMessage->u.responseMsg.errCode,
                                       0U,
                                       0U);
            break;
        }
        case TestFmk_MessageType_PROFILE:
        {
            /* Profile Control: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_IOCTL,
                                           ptrResponseMessage->u.responseMsg.errCode,
                                           ptrResponseMessage->u.responseMsg.arg0,
                                           (uint32_t)&ptrResponseMessage->u.responseMsg.profileMsg.profileData[0]);
            break;
        }
        case TestFmk_MessageType_UNLOAD:
        {
            /* Were there any errors? */
            if (ptrResponseMessage->u.responseMsg.errCode == 0U)
            {
                /* NO: Profile was unloaded */
                ptrTestFmk->profileStatus = 0;
            }

            /* Profile has been unloaded: Report this to the application */
            ptrTestFmk->initCfg.reportFxn (TestFmk_Report_PROFILE_UNLOADED,
                                           ptrResponseMessage->u.responseMsg.errCode,
                                           0U,
                                           0U);
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
 *      This is the function which is invoked to process the TestFmk IPC Message.
 *
 *  @param[in]  ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in]  ptrMessage
 *      Pointer to the TestFmk message which is to be processed.
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
static int32_t TestFmk_IPCProcessMsg
(
    TestFmk_MCB*    ptrTestFmk,
    TestFmk_Msg*    ptrMessage,
    int32_t*        errCode
)
{
    TestFmk_Msg     responseMsg;
    int32_t     retVal = 0;

    /* Initialize the error code: */
    *errCode = 0;

    /* Process the received message: */
    switch (ptrMessage->msgType)
    {
        case TestFmk_MessageType_OPEN:
        {
            /* Is the TestFmk Instance responsible for the profile? */
            if (ptrTestFmk->profileHandle != NULL)
            {
                /* YES: Open the profile through the registered handler */
                retVal = ptrTestFmk->profileCfg.openFxn (ptrTestFmk->profileHandle, &ptrMessage->u.mmWaveOpenCfg, errCode);

                /* Setup the response message: */
                TestFmk_IPCMsgInit (&responseMsg, TestFmk_MessageType_RESPONSE);
                responseMsg.u.responseMsg.requestMsgType = TestFmk_MessageType_OPEN;
                responseMsg.u.responseMsg.errCode        = *errCode;
                responseMsg.u.responseMsg.arg0           = 0U;
                responseMsg.u.responseMsg.arg1           = 0U;
                TestFmk_IPCSend (ptrTestFmk, &responseMsg, errCode);
            }
            break;
        }
        case TestFmk_MessageType_CLOSE:
        {
            /* Is the TestFmk Instance responsible for the profile? */
            if (ptrTestFmk->profileHandle != NULL)
            {
                /* YES: Close the profile through the registered handler */
                retVal = ptrTestFmk->profileCfg.closeFxn (ptrTestFmk->profileHandle, errCode);

                /* Setup the response message: */
                TestFmk_IPCMsgInit (&responseMsg, TestFmk_MessageType_RESPONSE);
                responseMsg.u.responseMsg.requestMsgType = TestFmk_MessageType_CLOSE;
                responseMsg.u.responseMsg.errCode        = *errCode;
                responseMsg.u.responseMsg.arg0           = 0U;
                responseMsg.u.responseMsg.arg1           = 0U;
                TestFmk_IPCSend (ptrTestFmk, &responseMsg, errCode);
            }
            break;
        }
        case TestFmk_MessageType_START:
        {
            /* Is the TestFmk Instance responsible for the profile? */
            if (ptrTestFmk->profileHandle != NULL)
            {
                /* Start the profile */
                retVal = ptrTestFmk->profileCfg.startFxn (ptrTestFmk->profileHandle, errCode);

                /* Setup the response message: */
                TestFmk_IPCMsgInit (&responseMsg, TestFmk_MessageType_RESPONSE);
                responseMsg.u.responseMsg.requestMsgType = TestFmk_MessageType_START;
                responseMsg.u.responseMsg.errCode        = *errCode;
                responseMsg.u.responseMsg.arg0           = 0U;
                responseMsg.u.responseMsg.arg1           = 0U;
                TestFmk_IPCSend (ptrTestFmk, &responseMsg, errCode);
            }
            break;
        }
        case TestFmk_MessageType_STOP:
        {
            /* Is the TestFmk Instance responsible for the profile? */
            if (ptrTestFmk->profileHandle != NULL)
            {
                /* YES: Stop the profile through the registered handler */
                retVal = ptrTestFmk->profileCfg.stopFxn (ptrTestFmk->profileHandle, errCode);

                /* Setup the response message: */
                TestFmk_IPCMsgInit (&responseMsg, TestFmk_MessageType_RESPONSE);
                responseMsg.u.responseMsg.requestMsgType = TestFmk_MessageType_STOP;
                responseMsg.u.responseMsg.errCode        = *errCode;
                responseMsg.u.responseMsg.arg0           = 0U;
                responseMsg.u.responseMsg.arg1           = 0U;
                TestFmk_IPCSend (ptrTestFmk, &responseMsg, errCode);
            }
            break;
        }
        case TestFmk_MessageType_SET_TESTFMK_CFG:
        {
            /* Copy over the configuration into the TestFmk: */
            memcpy ((void*)&ptrTestFmk->testfmkCfg, (void*)&ptrMessage->u.testfmkCfg, sizeof(TestFmk_Cfg));

            /* Is the TestFmk Instance responsible for the profile? */
            if (ptrTestFmk->profileHandle != NULL)
            {
                /* YES: Reset the ADC Channel configuration. */
                DebugP_assert (TestFmk_disableADC (ptrTestFmk, errCode) == 0);
                DebugP_assert (TestFmk_enableADC (ptrTestFmk, &ptrTestFmk->testfmkCfg, errCode) == 0);

                /* YES: Finalize the configuration by passing this to the profile. */
                retVal = ptrTestFmk->profileCfg.finalizeCfgFxn (ptrTestFmk->profileHandle, &ptrTestFmk->testfmkCfg, errCode);

                /* Setup the response message: */
                TestFmk_IPCMsgInit (&responseMsg, TestFmk_MessageType_RESPONSE);
                responseMsg.u.responseMsg.requestMsgType = TestFmk_MessageType_SET_TESTFMK_CFG;
                responseMsg.u.responseMsg.errCode        = *errCode;
                responseMsg.u.responseMsg.arg0           = 0U;
                responseMsg.u.responseMsg.arg1           = 0U;
                TestFmk_IPCSend (ptrTestFmk, &responseMsg, errCode);
            }
            break;
        }
        case TestFmk_MessageType_PROFILE:
        {
            /* Is the TestFmk Instance responsible for the profile? */
            if (ptrTestFmk->profileHandle != NULL)
            {
                /* YES: Pass the message to the profile control function */
                retVal = ptrTestFmk->profileCfg.controlFxn (ptrTestFmk->profileHandle,
                                                        ptrMessage->u.profileMsg.cmd,
                                                        &ptrMessage->u.profileMsg.profileData[0],
                                                        ptrMessage->u.profileMsg.cmdLen,
                                                        errCode);

                /* Setup the response message: */
                TestFmk_IPCMsgInit (&responseMsg, TestFmk_MessageType_RESPONSE);
                responseMsg.u.responseMsg.requestMsgType    = TestFmk_MessageType_PROFILE;
                responseMsg.u.responseMsg.errCode           = *errCode;
                responseMsg.u.responseMsg.arg0              = ptrMessage->u.profileMsg.cmd;
                responseMsg.u.responseMsg.arg1              = 0U;
                responseMsg.u.responseMsg.profileMsg.cmd    = ptrMessage->u.profileMsg.cmd;
                responseMsg.u.responseMsg.profileMsg.cmdLen = ptrMessage->u.profileMsg.cmdLen;
                memcpy ((void*)&responseMsg.u.responseMsg.profileMsg.profileData[0],
                        (void*)&ptrMessage->u.profileMsg.profileData[0],
                        responseMsg.u.responseMsg.profileMsg.cmdLen);

                TestFmk_IPCSend (ptrTestFmk, &responseMsg, errCode);
            }
            break;
        }
        case TestFmk_MessageType_FAULT:
        {
            /* Is the TestFmk Instance responsible for the profile? CPU Fault has been detected.
             * So pass this along to the registered profile to take appropriate action. */
            if (ptrTestFmk->profileHandle != NULL)
            {
                /* YES: Pass the message to the profile control function */
                retVal = ptrTestFmk->profileCfg.controlFxn (ptrTestFmk->profileHandle,
                                                            TEST_FMK_CMD_NOTIFY_FAULT,
                                                            NULL, 0,
                                                            errCode);
            }
            break;
        }
        case TestFmk_MessageType_RESPONSE:
        {
            /* Process the response: */
            retVal = TestFmk_IPCResponseMsgHandler (ptrTestFmk, ptrMessage, errCode);
            break;
        }
        default:
        {
            /* Error: Unhandled message */
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
 *          (a) Local: One thread is trying to control the TestFmk Profile
 *          (b) Remote: One core is trying to control the profile executing
 *              on the remote domain.
 *      Process all the messages
 *
 *  @param[in]  ptrTestFmk
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
int32_t TestFmk_IPCRecv (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    int32_t     retVal;
    TestFmk_Msg     message;
    bool        done = false;
    bool        processMessage;

    /* Cycle through until all the messages have been processed. */
    while (done == false)
    {
        /* Initialize the process message flag: */
        processMessage = false;

        /* Initialize the message: */
        memset ((void *)&message, 0, sizeof(TestFmk_Msg));

        /* Read the message from the local pipes: */
        retVal = TestFmk_IPCLocalRecv (ptrTestFmk, &message, errCode);
        if (retVal < 0)
        {
            /* Error: Unable to read the message from the local IPC */
            done = true;
        }
        else if (retVal == 0)
        {
            /* Local IPC is empty: Read a message from the remote IPC */
            retVal = TestFmk_IPCRemoteRecv (ptrTestFmk, &message, errCode);
            if (retVal < 0)
            {
                /* Error: Unable to read the message from the remote IPC */
                done = true;
            }
            else if (retVal == 0)
            {
                /* Remote IPC is empty too. We are done. */
                done = true;
            }
            else
            {
                /* Remote IPC has a message to be processed. */
                processMessage = true;
            }
        }
        else
        {
            /* Local IPC has a message to be processed. */
            processMessage = true;
        }

        /* Is there a message to be processed? */
        if (processMessage == true)
        {
            /* YES: Process the message */
            retVal = TestFmk_IPCProcessMsg (ptrTestFmk, &message, errCode);
            if (retVal < 0)
            {
                /* Error: Unable to process the message */
                done = true;
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the TestFmk IPC module.
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
int32_t TestFmk_IPCInit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    int32_t     retVal;

    /* Initialize the Local IPC: */
    retVal = TestFmk_IPCLocalInit (ptrTestFmk, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to initialize the local IPC. Error code is already setup */
        goto exit;
    }

    /* Initialize the Remote IPC: */
    retVal = TestFmk_IPCRemoteInit (ptrTestFmk, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to initialize the remote IPC. Error code is already setup */
        goto exit;
    }

    /* Setup the return value: The IPC module has been initialized successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the TestFmk IPC module.
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
int32_t TestFmk_IPCDeinit (TestFmk_MCB* ptrTestFmk, int32_t* errCode)
{
    int32_t retVal;

    /* Deinitialize the Remote IPC Module: */
    retVal = TestFmk_IPCRemoteDeinit (ptrTestFmk, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to deinitialize the remote IPC. Error code is already setup */
        goto exit;
    }

    /* Deinitialize the Local IPC Module: */
    retVal = TestFmk_IPCLocalDeinit (ptrTestFmk, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to deinitialize the local IPC. Error code is already setup */
        goto exit;
    }

    /* Setup the return value: */
    retVal = 0;

exit:
    return retVal;
}

