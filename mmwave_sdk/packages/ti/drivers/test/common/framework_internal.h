/**
 *   @file  framework_internal.h
 *
 *   @brief
 *      Internal header file used by the TestFmk module.
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
#ifndef TEST_FRAMEWORK_INTERNAL_H
#define TEST_FRAMEWORK_INTERNAL_H

/* mmWave SDK Include Files: */
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/test/common/framework_listlib.h>

/** @addtogroup TEST_FRAMEWORK_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief   Test Framework Profile Load/Unload status:
 */
#define TEST_FMK_PROFILE_STATUS_LOADED               0x1

/**
 * @brief   Test Framework Profile Load/Unload status:
 */
#define TEST_FMK_PROFILE_STATUS_OPENED               0x2

/**
 * @brief   Test Framework Profile Configured/Unconfigured status:
 */
#define TEST_FMK_PROFILE_STATUS_CONFIGURED           0x4

/**
 * @brief   Test Framework Profile Started/Stopped status:
 */
#define TEST_FMK_PROFILE_STATUS_STARTED              0x8

/**
 * @brief   This is the maximum queue depth of the local IPC queues.
 */
#define TEST_FMK_MAX_LOCAL_QUEUE                     4U

/**
 * @brief   This is the maximum size of profile specific data which can be
 * transported by the Test Framework module.
 */
#define TEST_FMK_MAX_PROFILE_DATA                    512U

/**
 * @brief
 *  Enumeration for Test Framework IPC messages
 *
 * @details
 *  The Test Framework IPC module exchanges messages between the
 *  Test Framework Peers. The enumeration describes the different
 *  types of messages which can be exchanged.
 */
typedef enum TestFmk_MessageType_e
{
    /**
     * @brief   Message to indicate that the profile has been loaded
     */
    TestFmk_MessageType_LOAD,

    /**
     * @brief   Message sent to indicate that the profile has been opened
     */
    TestFmk_MessageType_OPEN,

    /**
     * @brief   Message sent to indicate that the profile has been closed
     */
    TestFmk_MessageType_CLOSE,

    /**
     * @brief   Message sent to indicate that the profile has been started
     */
    TestFmk_MessageType_START,

    /**
     * @brief   Message sent to indicate that the profile has received the
     * configuration
     */
    TestFmk_MessageType_SET_TESTFMK_CFG,

    /**
     * @brief   Message sent to indicate that the profile has been stopped
     */
    TestFmk_MessageType_STOP,

    /**
     * @brief   Message sent to indicate that the profile has been unloaded
     */
    TestFmk_MessageType_UNLOAD,

    /**
     * @brief   Profile specific message. This is simply passed to the profile
     * without interpretation.
     */
    TestFmk_MessageType_PROFILE,

    /**
     * @brief   Message sent to indicate that there was a CPU/ESM Fault reported
     * by the ESM Layer.
     */
    TestFmk_MessageType_FAULT,

    /**
     * @brief   Response message
     */
    TestFmk_MessageType_RESPONSE,
}TestFmk_MessageType;

/**
 * @brief
 *  Test Framework Profile specific message
 *
 * @details
 *  The structure describes the profile message. Profile specific messages
 *  are not interpreted by the Test Framework module. These are simply transported
 *  as is to the profile. Interpretation of these messages is done by the
 *  profile.
 */
typedef struct TestFmk_ProfileMsg_t
{
    /**
     * @brief   Profile specific command:
     */
    uint32_t        cmd;

    /**
     * @brief   Profile specific command length:
     */
    uint32_t        cmdLen;

    /**
     * @brief   Generic Data Payload: This is used to send profile specific messages
     */
    uint8_t         profileData[TEST_FMK_MAX_PROFILE_DATA];
}TestFmk_ProfileMsg;

/**
 * @brief
 *  Test Framework Response Format
 *
 * @details
 *  The structure describes the response. Responses are sent back to the
 *  peer which initiated the Test Framework IPC Request to indicate the status of
 *  the operation. Applications are notified though the reporting mechanism
 */
typedef struct TestFmk_ResponseMsg_t
{
    /**
     * @brief   Response Message to the request
     */
    TestFmk_MessageType     requestMsgType;

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
     * @brief   Response Profile Message Inlined. This is useful to pass
     * profile specific data between the Test Framework entities.
     */
    TestFmk_ProfileMsg      profileMsg;
}TestFmk_ResponseMsg;

/**
 * @brief
 *  Test Framework Message Format
 *
 * @details
 *  The structure describes the Test Framework message format
 */
typedef struct TestFmk_Msg_t
{
    /**
     * @brief   Header: This is the type of message being exchanged
     */
    TestFmk_MessageType     msgType;

    /**
     * @brief   Message Body: This is specific to the message type
     */
    union
    {
        /**
         * @brief   Test Framework configuration: Valid only if the message type is TestFmkCFG
         */
        TestFmk_Cfg             testfmkCfg;

        /**
         * @brief   Response Message sent back to indicate the status of the operation
         */
        TestFmk_ResponseMsg     responseMsg;

        /**
         * @brief   Open configuration:
         */
        MMWave_OpenCfg          mmWaveOpenCfg;

        /**
         * @brief   Profile specific message:
         */
        TestFmk_ProfileMsg      profileMsg;
    }u;
}TestFmk_Msg;

/**
 * @brief
 *  Test Framework HW Attributes
 *
 * @details
 *  This the Test Framework SOC HW Attributes which will allow the module to be ported
 */
typedef struct TestFmk_HWAttribute_t
{
    /**
     * @brief   Chirp Available System Interrupt
     */
    uint32_t        chirpAvailableSysInterrupt;

    /**
     * @brief   Frame Start System Interrupt
     */
    uint32_t        frameStartSysInterrupt;
}TestFmk_HWAttribute;

/**
 * @brief
 *  Test Framework Local IPC Message
 *
 * @details
 *  The structure is used to exchange local IPC messages
 */
typedef struct TestFmk_LocalIPCMsg_t
{
    /**
     * @brief   This is the links to other local IPC Messages
     */
    TestFmk_ListNode        links;

    /**
     * @brief   This is the local IPC message which is being exchanged
     */
    TestFmk_Msg             msg;
}TestFmk_LocalIPCMsg;

/**
 * @brief
 *  Test Framework Master control block
 *
 * @details
 *  The structure is used to hold all the relevant information required to
 *  execute the Test Framework module
 */
typedef struct TestFmk_MCB_t
{
    /**
     * @brief   This is the configuration which was used to initialize the module.
     */
    TestFmk_InitCfg                     initCfg;

    /**
     * @brief   Loaded Profile configuration
     */
    TestFmk_ProfileCfg                  profileCfg;

    /**
     * @brief   Profile Handle: This is the profile which has been loaded and initialized
     */
    TestFmk_ProfileHandle               profileHandle;

    /**
     * @brief   Handle to the ADCBUF Driver: Valid only if the instance is executing the
     * profile.
     */
    ADCBuf_Handle                       adcBufHandle;

    /**
     * @brief   Handle to the EDMA Driver. There can be multiple instances of the EDMA.
     * This is valid only if the instance is executing the profile.
     */
    EDMA_Handle                         edmaHandle[EDMA_NUM_CC];

    /**
     * @brief   Chirp Available System Interrupt Listener Handle:
     * This is valid only if the instance is executing the profile and the profile is
     * waiting for chirp interrupts.
     */
    SOC_SysIntListenerHandle            chirpAvailableListenerHandle;

    /**
     * @brief   Frame Start System Interrupt Listener Handle:
     * This is valid only if the instance is executing the profile and the profile is
     * waiting for frame start interrupts.
     */
    SOC_SysIntListenerHandle            frameStartListenerHandle;

    /**
     * @brief   Status of the profile
     */
    uint32_t                            profileStatus;

    /**
     * @brief   Test Framework Configuration which is available to the Test Framework instance.
     */
    TestFmk_Cfg                         testfmkCfg;

    /**
     * @brief   Semaphore Handle: This is the main semaphore handle which is posted:
     * - Reception of chirp data
     * - Reception of Test Framework IPC Messages
     */
    SemaphoreP_Handle                   semaphoreHandle;

    /**
     * @brief   Remote IPC Mailbox Handle: This is used by the IPC module to communicate
     * with the remote peer to exchange Test Framework IPC messages.
     */
    Mbox_Handle                      ipcMailboxHandle;

    /**
     * @brief   Local IPC Message Queue: These are the messages which are exchanged by
     * the IPC module for Local IPC.
     */
    TestFmk_LocalIPCMsg                 localMsgQueue[TEST_FMK_MAX_LOCAL_QUEUE];

    /**
     * @brief   Local IPC Pending Message Queue: This is the queue of all the Local
     * IPC Messages which have been posted but have still not been processed.
     */
    TestFmk_LocalIPCMsg*                localIPCMessagePendingQueue;

    /**
     * @brief   Local IPC Free Message Queue: This is the queue of all the Local
     * IPC Messages which are free and which can be posted.
     */
    TestFmk_LocalIPCMsg*                localIPCMessageFreeQueue;
}TestFmk_MCB;

/**
@}
*/

/**************************************************************************
 * Global Declarations:
 **************************************************************************/
extern TestFmk_HWAttribute  gTestFmkHwAttrib;
extern SemaphoreP_Handle    gTestFmkSemaphoreHandle;

/**************************************************************************
 * Exported IPC Module API:
 **************************************************************************/
extern void    TestFmk_IPCMsgInit (TestFmk_Msg* ptrMessage, TestFmk_MessageType msgType);
extern int32_t TestFmk_IPCInit (TestFmk_MCB* ptrTestFmk, int32_t* errCode);
extern int32_t TestFmk_IPCSend (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode);
extern int32_t TestFmk_IPCRecv (TestFmk_MCB* ptrTestFmk, int32_t* errCode);
extern int32_t TestFmk_IPCDeinit (TestFmk_MCB* ptrTestFmk, int32_t* errCode);

/* Remote IPC Exported API: These are Platform specific */
extern int32_t TestFmk_IPCRemoteRecv (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode);
extern int32_t TestFmk_IPCRemoteSend (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode);
extern int32_t TestFmk_IPCRemoteInit (TestFmk_MCB* ptrTestFmk, int32_t* errCode);
extern int32_t TestFmk_IPCRemoteDeinit (TestFmk_MCB* ptrTestFmk, int32_t* errCode);

/* Local IPC Exported API: */
extern int32_t TestFmk_IPCLocalRecv (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode);
extern int32_t TestFmk_IPCLocalSend (TestFmk_MCB* ptrTestFmk, TestFmk_Msg* ptrMessage, int32_t* errCode);
extern int32_t TestFmk_IPCLocalInit (TestFmk_MCB* ptrTestFmk, int32_t* errCode);
extern int32_t TestFmk_IPCLocalDeinit (TestFmk_MCB* ptrTestFmk, int32_t* errCode);

/* Core Exported API: */
extern int32_t TestFmk_enableADC  (TestFmk_MCB* ptrTestFmk,TestFmk_Cfg* ptrTestFmkCfg, int32_t* errCode);
extern int32_t TestFmk_disableADC (TestFmk_MCB* ptrTestFmk, int32_t* errCode);

#endif /* TEST_FRAMEWORK_INTERNAL_H */

