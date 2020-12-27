/**
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the EDMA driver.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#if defined (SUBSYS_DSS)
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#endif
#include <ti/utils/cycleprofiler/cycle_profiler.h>
#include <ti/drivers/osal/DebugP.h>
#include <assert.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include "ti/utils/testlogger/logger.h"

/**************************************************************************
 *************************** edma Test Constants & Variables *****************
 **************************************************************************/


/**************************************************************************
 *************************** edma Test Function prototye *********************
 **************************************************************************/


/**************************************************************************
 *************************** edma Test Functions **************************
 **************************************************************************/
#define TEST_NO_ERROR 0
#define TEST_E_EDMA_API -1

#define EDMA_UNTIED_CH0  63
#define EDMA_UNTIED_CH1  32
#define EDMA_UNTIED_CH2  31
#define EDMA_UNTIED_CH3  0
#define EDMA_UNTIED_CH4  4
#define EDMA_UNTIED_CH5  5
#define EDMA_UNTIED_CH6  6

#define TEST_TRANSFER_COMPLETION_CODE_CH0 0
#define TEST_TRANSFER_COMPLETION_CODE_CH1 31
#define TEST_TRANSFER_COMPLETION_CODE_CH2 32
#define TEST_TRANSFER_COMPLETION_CODE_CH3 63

#define EDMA_EVENT_QUEUE_ID_CH0  0
#define EDMA_EVENT_QUEUE_ID_CH1  0
#define EDMA_EVENT_QUEUE_ID_CH2  0
#define EDMA_EVENT_QUEUE_ID_CH3  0

#define TEST_PARAM_ID 0

#define TEST_TIMEOUT_CYCLES 1000000U

#define TEST_CH0 0
#define TEST_CH1 1
#define TEST_CH2 2
#define TEST_CH3 3

#define EDMA_QDMA_CH0 0
#define EDMA_QDMA_CH1 1
#define EDMA_QDMA_CH2 2
#define EDMA_QDMA_CH3 3
#define EDMA_QDMA_CH4 4
#define EDMA_QDMA_CH5 5
#define EDMA_QDMA_CH6 6
#define EDMA_QDMA_CH7 7

/* divide up params equally among QDMA channels for convenience */
#define MAX_NUM_PARAMS 128
#define NUM_PARAMS_ALLOCATED_PER_QDMA_CH ((MAX_NUM_PARAMS - EDMA_NUM_DMA_CHANNELS)/EDMA_NUM_QDMA_CHANNELS)
#define TEST_QDMA_PARAM_ID_CH0_START  EDMA_NUM_DMA_CHANNELS
#define TEST_QDMA_PARAM_ID_CH0_END    (TEST_QDMA_PARAM_ID_CH0_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)
#define TEST_QDMA_PARAM_ID_CH1_START  (TEST_QDMA_PARAM_ID_CH0_END + 1)
#define TEST_QDMA_PARAM_ID_CH1_END    (TEST_QDMA_PARAM_ID_CH1_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)
#define TEST_QDMA_PARAM_ID_CH2_START  (TEST_QDMA_PARAM_ID_CH1_END + 1)
#define TEST_QDMA_PARAM_ID_CH2_END    (TEST_QDMA_PARAM_ID_CH2_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)
#define TEST_QDMA_PARAM_ID_CH3_START  (TEST_QDMA_PARAM_ID_CH2_END + 1)
#define TEST_QDMA_PARAM_ID_CH3_END    (TEST_QDMA_PARAM_ID_CH3_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)
#define TEST_QDMA_PARAM_ID_CH4_START  (TEST_QDMA_PARAM_ID_CH3_END + 1)
#define TEST_QDMA_PARAM_ID_CH4_END    (TEST_QDMA_PARAM_ID_CH4_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)
#define TEST_QDMA_PARAM_ID_CH5_START  (TEST_QDMA_PARAM_ID_CH4_END + 1)
#define TEST_QDMA_PARAM_ID_CH5_END    (TEST_QDMA_PARAM_ID_CH5_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)
#define TEST_QDMA_PARAM_ID_CH6_START  (TEST_QDMA_PARAM_ID_CH5_END + 1)
#define TEST_QDMA_PARAM_ID_CH6_END    (TEST_QDMA_PARAM_ID_CH6_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)
#define TEST_QDMA_PARAM_ID_CH7_START  (TEST_QDMA_PARAM_ID_CH6_END + 1)
#define TEST_QDMA_PARAM_ID_CH7_END    (TEST_QDMA_PARAM_ID_CH7_START + NUM_PARAMS_ALLOCATED_PER_QDMA_CH - 1)

#define TEST_MAX_NUM_SIMULTANEOUS_CHANNELS 4
#define TEST_INVALID_CHANNEL (TEST_MAX_NUM_SIMULTANEOUS_CHANNELS + 1)
#define TEST_MAX_A_COUNT 2000 //convenient because CCS shows in chunks of 100 in "expressions"
#define TEST_MAX_B_COUNT 5
#define TEST_MAX_C_COUNT 1
#define TEST_BUF_LENGTH (TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * TEST_MAX_C_COUNT)

#define ADDR_TRANSLATE_CPU_TO_EDMA(x)       SOC_translateAddress(x,SOC_TranslateAddr_Dir_TO_EDMA,NULL)

#define TEST_INVALID_SOURCE_ADDRESS_ID      1
#define TEST_INVALID_DESTINATION_ADDRESS_ID 2
#define TEST_INVALID_TRANSFER_REQUEST_ID    3

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

#pragma DATA_SECTION(testSrcBuf, ".l3ram");
uint8_t testSrcBuf[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS]
                  [TEST_BUF_LENGTH];

#ifdef SUBSYS_DSS
#pragma DATA_SECTION(testDstBuf, ".l2ram");
#else
#pragma DATA_SECTION(testDstBuf, ".l3ram");
#endif
uint8_t testDstBuf[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS]
                  [TEST_BUF_LENGTH];

EDMA_Handle testEdmaHandle;
volatile int8_t gInstanceId;

typedef struct testChannelConfig_t_
{
    EDMA_channelConfig_t config;
    bool isUseSpecialStartAPI;
} testChannelConfig_t;

typedef struct testFailureDiagnostics_t_
{
    bool isTransferCompletionCodeInvalid;
} testFailureDiagnostics_t;

typedef struct testChannelState_t_
{
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn;
    uint8_t transferCompletionCode;
    volatile bool isTransferDone;
    volatile uint32_t transferStartTimeStamp;
    volatile uint32_t transferEndTimeStamp;
    bool isPass;
} testChannelState_t;

typedef struct testTransferControllerErrorInfo_t_
{
    EDMA_transferControllerErrorInfo_t info;
    volatile uint32_t callbackFxnCount;
} testTransferControllerErrorInfo_t;

typedef struct testErrorInfo_t_
{
    uint32_t dmaChannelEventMissCount[EDMA_NUM_DMA_CHANNELS];
    uint32_t qdmaChannelEventMissCount[EDMA_NUM_QDMA_CHANNELS];
    uint32_t outstandingTransferCompletionTransfersExceededLimitCount;
    bool eventQueueThresholdExceededCount[EDMA_MAX_NUM_EVENT_QUEUES];
} testErrorInfo_t;

typedef struct testState_t_
{
    testChannelState_t channelState[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS];
    testFailureDiagnostics_t failureDiagnostics;
    uint8_t channelStart;
    uint8_t channelEnd;
    testErrorInfo_t errorInfo;
    volatile uint32_t errorCallbackFxnCount;
    testTransferControllerErrorInfo_t transferControllerErrorInfo[EDMA_MAX_NUM_TRANSFER_CONTROLLERS];
    EDMA_statusInfo_t statusInfo;
    bool isTestingTransferControllerError;
    bool isTestingError;
} testState_t;

testState_t testState;

void Test_delay(uint32_t cpuCycles)
{
    volatile uint32_t tStart;

    tStart = Cycleprofiler_getTimeStamp();
    while (((int32_t)Cycleprofiler_getTimeStamp() - (int32_t)tStart) >= cpuCycles);
}

void Test_indicateResultToAutomation(char *st, bool isPass)
{
    if (isPass == true)
    {
        MCPI_setFeatureTestResult(st, MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult(st, MCPI_TestResult_FAIL);        
    }
}

static inline uint32_t Test_getTransferTime(testChannelState_t *channelState)
{
    return(channelState->transferEndTimeStamp - channelState->transferStartTimeStamp);
}


bool Test_isTransferSuccessful(uint8_t channel)
{
    uint32_t indx;
    bool result = true;

    for (indx = 0; indx < TEST_BUF_LENGTH; indx++)
    {
        if (testDstBuf[channel][indx] != testSrcBuf[channel][indx])
        {
            result = false;
            break;
        }
    }
    return(result);
}

static void test_transferCompletionCallbackFxn(uintptr_t arg, uint8_t transferCompletionCode)
{
    uint8_t channel;
    uint8_t channelFound = TEST_INVALID_CHANNEL;

    volatile uint32_t entryTimeStamp = Cycleprofiler_getTimeStamp();

    DebugP_assert(arg == (uintptr_t) testEdmaHandle);

    for (channel = testState.channelStart; channel <= testState.channelEnd; channel++)
    {
        if (testState.channelState[channel].transferCompletionCallbackFxn != NULL)
        {
            if (testState.channelState[channel].transferCompletionCode == transferCompletionCode)
            {
                channelFound = channel;
                testState.channelState[channel].isTransferDone = true;
                testState.channelState[channel].transferEndTimeStamp = entryTimeStamp;
                break;
            }
        }
    }

    if (channelFound == TEST_INVALID_CHANNEL)
    {
        testState.failureDiagnostics.isTransferCompletionCodeInvalid = true;
        goto exit;
    }

exit:
    return;
}

/* A type single transfer (bCnt = 1 AND cCnt = 1), mix QDMA and DMA */
#define TEST_A_SINGLE_XFER_MIX_QDMA_DMA__NUM_SIMULTANEOUS_CHANNELS 4
testChannelConfig_t testChannelConfig__A_SINGLE_XFER_MIX_QDMA_DMA
    [TEST_A_SINGLE_XFER_MIX_QDMA_DMA__NUM_SIMULTANEOUS_CHANNELS] =
{
    /* 1st channel */
    {
    .config.channelId = EDMA_UNTIED_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH0,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 2nd channel */
    {
    .config.channelId = EDMA_UNTIED_CH1,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH1,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH1,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 3rd channel */
    {
    .config.channelId = EDMA_QDMA_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_QDMA,
    .config.paramId = TEST_QDMA_PARAM_ID_CH0_START,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH2,
    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_SRC,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH2][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH2][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH2,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = true,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 4th channel */
    {
    .config.channelId = EDMA_QDMA_CH7,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_QDMA,
    .config.paramId = TEST_QDMA_PARAM_ID_CH7_START,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH3,
    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_DEFAULT,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH3][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH3][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH3,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = true,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    }
};

#define TEST_DBG__NUM_SIMULTANEOUS_CHANNELS 1
testChannelConfig_t testChannelConfig__DBG[TEST_DBG__NUM_SIMULTANEOUS_CHANNELS] =
{
    /* 1st channel */
    {
    .config.channelId = EDMA_QDMA_CH7,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_QDMA,
    .config.paramId = TEST_QDMA_PARAM_ID_CH0_START,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH1,
//    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_SRC_DST_CIDX,
//    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_SRC_DST_BIDX,
    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_LINK_BCNTRLD,
//    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_OPT,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    }
};

/* A type multi transfers (bCnt != 1 AND cCnt != 0) */
#define TEST_A_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS 4
testChannelConfig_t testChannelConfig__A_MULTI_XFER[TEST_A_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS] =
{
    /* 1st channel, test negative jumps also */
    {
    .config.channelId = EDMA_UNTIED_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH0,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][TEST_BUF_LENGTH]) - 
        /*aCount = */ TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][TEST_BUF_LENGTH]) - 
        /*aCount = */ TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = -(int16_t)(TEST_MAX_A_COUNT * sizeof(uint8_t)),
    .config.paramSetConfig.destinationBindex = -(int16_t)(TEST_MAX_A_COUNT * sizeof(uint8_t)),
    .config.paramSetConfig.sourceCindex = -(int16_t)(TEST_MAX_A_COUNT * sizeof(uint8_t)),
    .config.paramSetConfig.destinationCindex = -(int16_t)(TEST_MAX_A_COUNT * sizeof(uint8_t)),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 2nd channel */
#if 0
    {
    .config.channelId = EDMA_UNTIED_CH1,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH1,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH1,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = true,
    },
#else
    {
    .config.channelId = EDMA_QDMA_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_QDMA,
    .config.paramId = TEST_QDMA_PARAM_ID_CH0_START,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH1,
    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_OPT,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = true,
    },
#endif
    /* 3rd channel */
    {
    .config.channelId = EDMA_UNTIED_CH2,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH2,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH2,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH2][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH2][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH2,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 4th channel */
#if 1
    {
    .config.channelId = EDMA_UNTIED_CH3,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH3,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH3,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH3][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH3][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH3,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    }
#else
    {
    .config.channelId = EDMA_QDMA_CH1,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_QDMA,
    .config.paramId = TEST_QDMA_PARAM_ID_CH1_START,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH3,
    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_LINK_BCNTRLD, //EDMA_QDMA_TRIG_WORD_OFFSET_OPT,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH3][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH3][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH3,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    }
#endif
};

/* AB type multi transfers (cCnt != 1) */
#define TEST_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS 3
testChannelConfig_t testChannelConfig__AB_MULTI_XFER[TEST_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS] =
{
    /* 1st channel */
    {
    .config.channelId = EDMA_UNTIED_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH0,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_AB,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 2nd channel */
    {
    .config.channelId = EDMA_UNTIED_CH1,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH1,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH1,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_AB,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 3rd channel */
    {
    .config.channelId = EDMA_UNTIED_CH2,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH2,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH2,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH2][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH2][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_AB,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH2,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    }
};

/* mix of A and AB type multi transfers */
#define TEST_A_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS 4
testChannelConfig_t testChannelConfig__A_AB_MULTI_XFER[TEST_A_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS] =
{
    /* 1st channel */
    {
    .config.channelId = EDMA_UNTIED_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH0,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_AB,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 2nd channel */
    {
    .config.channelId = EDMA_UNTIED_CH1,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH1,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH1,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 3rd channel */
    {
    .config.channelId = EDMA_UNTIED_CH2,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH2,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH2,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH2][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH2][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = TEST_MAX_B_COUNT,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH2,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 4th channel */
    {
    .config.channelId = EDMA_UNTIED_CH3,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH3,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH3,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH3][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH3][0]),
    .config.paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.bCount = TEST_MAX_B_COUNT,
    .config.paramSetConfig.cCount = TEST_MAX_C_COUNT,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * TEST_MAX_B_COUNT * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_AB,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH3,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = true,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    }
};

bool Test_unchainedUnlinked(EDMA_Handle handle, EDMA_instanceInfo_t *instInfo,
    uint8_t numChannels, testChannelConfig_t *testChannelConfig)
{
    EDMA_channelConfig_t *config;
    uint8_t channelId;
    uint8_t channelType;
    int32_t errorCode;
    bool isTestPass = false;
    uint8_t channel;
    volatile bool isTransfersDone;
    volatile bool isChannelTransferDone;
    volatile uint32_t timeout;
    uint8_t channelStart = testState.channelStart;
    uint8_t channelEnd = testState.channelEnd;
    uint32_t transferCount[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS];
    uint16_t bCount, cCount;
    bool isEnableChannel;

    /* configure all test channels */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        config = &testChannelConfig[channel].config;
        config->transferCompletionCallbackFxnArg = (uintptr_t) handle;

        channelType = config->channelType;
        channelId = config->channelId;
        if ((channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA) &&
            (instInfo->isChannelMapExist == false))
        {
            config->paramId = channelId;
        }

        if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
        {
            /* no event triggering */
            isEnableChannel = false;
        }
        else
        {
            isEnableChannel = true;
        }

        if ((errorCode = EDMA_configChannel(handle, config, isEnableChannel)) != EDMA_NO_ERROR)
        {
            printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
            goto exit;
        }
    }

    /* start the test channels */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        config = &testChannelConfig[channel].config;
        channelType = config->channelType;
        channelId = config->channelId;
        testState.channelState[channel].transferStartTimeStamp = Cycleprofiler_getTimeStamp();
        if (testChannelConfig[channel].isUseSpecialStartAPI == true)
        {
            if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
            {
                errorCode = EDMA_startDmaTransfer(handle, channelId);
            }
            else
            {
                errorCode = EDMA_startQdmaTransfer(handle, channelId);
            }
            if (errorCode != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_Dma(Qdma)StartTransfer() of test channel %d failed with error code = %d\n", channel, errorCode);
                goto exit;
            }
        }
        else
        {
            if ((errorCode = EDMA_startTransfer(handle, channelId, channelType)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_StartTransfer() of test channel %d failed with error code = %d\n", channel, errorCode);
                goto exit;
            }
        }
    }

    /* determine additional transfers and initialize transferCount to the additional
       transfers */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        bCount = testChannelConfig[channel].config.paramSetConfig.bCount;
        cCount = testChannelConfig[channel].config.paramSetConfig.cCount;
        if (testChannelConfig[channel].config.paramSetConfig.transferType == (uint8_t)EDMA3_SYNC_A) {
            transferCount[channel] = (bCount * cCount) - 1;
        }
        else /* transferType is (uint8_t)EDMA3_SYNC_AB */
        {
            transferCount[channel] = cCount - 1;
        }
    }

    /* wait until all transfers done, for channels with call back configuration,
       check test state (that was set in call back function) for done-ness,
       for those without call-back use driver API to check for completion */
    timeout = TEST_TIMEOUT_CYCLES;
    do {
        isTransfersDone = true;
        for(channel = channelStart; channel <= channelEnd; channel++)
        {
            config = &testChannelConfig[channel].config;
            channelType = config->channelType;
            channelId = config->channelId;
            if (config->transferCompletionCallbackFxn == NULL)
            {
                /* cannot have repeat isTransferDone if transfer already finished because
                   isTransferDone clears state that it is polling (to enabled new transfer) */
                if (testState.channelState[channel].isTransferDone == false)
                {
                    if ((errorCode = EDMA_isTransferComplete(handle,
                            config->paramSetConfig.transferCompletionCode,
                            (bool *) &testState.channelState[channel].isTransferDone)) != EDMA_NO_ERROR)
                    {
                        printf("Error: EDMA_isTransferComplete() failed with error code = %d\n", errorCode);
                        goto exit;
                    }
                    testState.channelState[channel].transferEndTimeStamp = Cycleprofiler_getTimeStamp();
                }
            }

            /* kick additional transfers if done with previous */
            if (testState.channelState[channel].isTransferDone == true)
            {
                if (transferCount[channel] != 0)
                {
                    testState.channelState[channel].isTransferDone = false;

                    /* last QDMA transfer of a multi-transfer must use a different API to start transfer */
                    if ((channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA) && (transferCount[channel] == 1))
                    {
                        EDMA_startFinalQdmaTransfer(handle, channelId);
                    }
                    else
                    {
                        if (testChannelConfig[channel].isUseSpecialStartAPI == true)
                        {
                            if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
                            {
                                errorCode = EDMA_startDmaTransfer(handle, channelId);
                            }
                            else
                            {
                                errorCode = EDMA_startQdmaTransfer(handle, channelId);
                            }
                            if (errorCode != EDMA_NO_ERROR)
                            {
                                printf("Error: EDMA_Dma(Qdma)StartTransfer() of test channel %d failed with error code = %d\n",
                                    channel, errorCode);
                                goto exit;
                            }
                        }
                        else
                        {
                            if ((errorCode = EDMA_startTransfer(handle, channelId, channelType)) != EDMA_NO_ERROR)
                            {
                                printf("Error: EDMA_StartTransfer() of test channel %d failed with error code = %d\n", channel, errorCode);
                                goto exit;
                            }
                        }
                    }
                    transferCount[channel]--;
                }
            }

            if (transferCount[channel] != 0)
            {
                isChannelTransferDone = false;
            }
            else
            {
                isChannelTransferDone = testState.channelState[channel].isTransferDone;
            }
            isTransfersDone = isTransfersDone && isChannelTransferDone;
        }
        timeout--;
    } while ((isTransfersDone == false) && (timeout != 0));

    if (timeout == 0)
    {
        printf("FAIL: Test timed out\n");
    }

    /* check if completed transfers have been successful */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        if (testState.channelState[channel].isTransferDone == true)
        {
            testState.channelState[channel].isPass = Test_isTransferSuccessful(channel);
        }
    }

    /* print results */
    isTestPass = true;
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        if (testState.channelState[channel].isTransferDone == true)
        {
            if (testState.channelState[channel].isPass == true)
            {
                printf("Test channel %d transfer completed successfully\n", channel);
                printf("..transfer time in CPU cycles = %u\n", 
                    Test_getTransferTime(&testState.channelState[channel]));
            }
            else
            {
                printf("FAIL: Test channel %d transfer completed but unsuccessful\n", channel);
                isTestPass = false;
            }
        }
        else
        {
            printf("FAIL: Test channel %d transfer did not complete\n", channel);
            isTestPass = false;
        }
    }
exit:

    /* cleanup: important to disable channels after transfers are done before next test
       so that next test may not prematurely trigger transfer, one example is :
       A QDMA channel was programmed and transfer completed with a trigger word
       The same channel Id for next test is now a DMA channel. When configure API
       for this one is issued, it will only deal with DMA channel registers as the channel
       type is DMA and will leave the QDMA channel enabled. When the param Id (same
       as channel Id for AR devices) is configured for this DMA channel, it will trigger
       the QDMA channel that was left armed prior to config !!! (This is also a test
       for disableChannel API which was created for cleanup like this. */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        config = &testChannelConfig[channel].config;
        channelType = config->channelType;
        channelId = config->channelId;
        EDMA_disableChannel(handle, channelId, channelType);
    }

    return(isTestPass);
}

    uint32_t outstandingTransferCompletionTransfersExceededLimitCount;
    bool eventQueueThresholdExceededCount[EDMA_MAX_NUM_EVENT_QUEUES];

void Test_ErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    uint8_t channel, queue;

    for (channel = 0; channel < EDMA_NUM_DMA_CHANNELS; channel++)
    {
        testState.errorInfo.dmaChannelEventMissCount[channel] +=
            errorInfo->isDmaChannelEventMiss[channel];
    }
    for (channel = 0; channel < EDMA_NUM_QDMA_CHANNELS; channel++)
    {
        testState.errorInfo.qdmaChannelEventMissCount[channel] +=
            errorInfo->isQdmaChannelEventMiss[channel];
    }
    testState.errorInfo.outstandingTransferCompletionTransfersExceededLimitCount +=
        errorInfo->isOutstandingTransferCompletionTransfersExceededLimit;

    for(queue = 0; queue < errorInfo->numEventQueues; queue++)
    {
        testState.errorInfo.eventQueueThresholdExceededCount[queue] +=
            errorInfo->isEventQueueThresholdExceeded[queue];
    }
    testState.errorCallbackFxnCount++;
    if (testState.isTestingError == false)
    {
        DebugP_assert(0);
    }
}

void Test_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    uint8_t tcId = errorInfo->transferControllerId;

    memcpy(&testState.transferControllerErrorInfo[tcId].info,
        errorInfo, sizeof(EDMA_transferControllerErrorInfo_t));

    EDMA_getStatusInfo(handle, &testState.statusInfo);
    testState.transferControllerErrorInfo[tcId].callbackFxnCount++;
    if (testState.isTestingTransferControllerError == false)
    {
        DebugP_assert(0);
    }
}

void Test_initBufs(uint8_t channelStart, uint8_t channelEnd)
{
    uint32_t indx;
    uint8_t channel;

    for (channel = channelStart; channel <= channelEnd; channel++)
    {
        for (indx = 0; indx < TEST_BUF_LENGTH; indx++)
        {
            testSrcBuf[channel][indx] = (indx & 0xFF);
        }

        for (indx = 0; indx < TEST_BUF_LENGTH; indx++)
        {
            testDstBuf[channel][indx] = (0xFF - indx) & 0xFF;
        }
    }
}

void Test_init(uint8_t channelStart, uint8_t channelEnd,
    EDMA_transferCompletionCallbackFxn_t *transferCompletionCallbackFxn,
    uint8_t *transferCompletionCode)
{
    uint8_t channel, tcId;

    if (channelEnd >= TEST_MAX_NUM_SIMULTANEOUS_CHANNELS)
    {
        DebugP_assert(0);
    }

    Test_initBufs(channelStart, channelEnd);

    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        testState.channelState[channel].isTransferDone = false;
        testState.channelState[channel].transferCompletionCallbackFxn =
            transferCompletionCallbackFxn[channel];
        testState.channelState[channel].transferCompletionCode =
            transferCompletionCode[channel];
    }
    testState.failureDiagnostics.isTransferCompletionCodeInvalid = false;
    testState.channelStart = channelStart;
    testState.channelEnd = channelEnd;

    memset(&testState.errorInfo, 0, sizeof(testState.errorInfo));
    testState.errorCallbackFxnCount = 0;

    for(tcId = 0; tcId < EDMA_MAX_NUM_TRANSFER_CONTROLLERS; tcId++)
    {
        memset(&testState.transferControllerErrorInfo[tcId], 0,
            sizeof(testTransferControllerErrorInfo_t));
    }
    memset(&testState.statusInfo, 0, sizeof(EDMA_statusInfo_t));
    testState.isTestingTransferControllerError = false;
    testState.isTestingError = false;
}

void Test_init_testChannelConfig(uint8_t channelStart,
    uint8_t channelEnd, uint8_t numChannels, testChannelConfig_t *testChannelConfig,
    bool isTransferCompletionInterruptConnected)
{
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS];
    uint8_t transferCompletionCode[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS];
    uint8_t channel;

    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        if (isTransferCompletionInterruptConnected == false)
        {
            testChannelConfig[channel].config.transferCompletionCallbackFxn = NULL;
        }

        transferCompletionCallbackFxn[channel] =
            testChannelConfig[channel].config.transferCompletionCallbackFxn;
        transferCompletionCode[channel] =
            testChannelConfig[channel].config.paramSetConfig.transferCompletionCode;
    }

    Test_init(channelStart, channelEnd, transferCompletionCallbackFxn,
        transferCompletionCode);

}

void Test_init_testChannelConfig_translateAddrs(uint8_t channelStart,
    uint8_t channelEnd, uint8_t numChannels, testChannelConfig_t *testChannelConfig)
{
    uint8_t channel;

    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        testChannelConfig[channel].config.paramSetConfig.sourceAddress = 
            ADDR_TRANSLATE_CPU_TO_EDMA(testChannelConfig[channel].config.paramSetConfig.sourceAddress);
        testChannelConfig[channel].config.paramSetConfig.destinationAddress= 
            ADDR_TRANSLATE_CPU_TO_EDMA(testChannelConfig[channel].config.paramSetConfig.destinationAddress);
    }

}


bool Test_simultaneousUnchainedUnlinkedTransfersSuite(EDMA_Handle handle,
    EDMA_instanceInfo_t *instanceInfo, uint32_t *testIteration)
{
    bool areTestsPass = true;
    bool isTestPass;
#define FEATURE_STRING_LEN 200    
    char featureString[FEATURE_STRING_LEN];

#ifdef testChannelConfig__DBG
    printf("\nTest#%d...\n", *testIteration);
    Test_init_testChannelConfig(

        /* startChannel, endChannel */
        TEST_CH0, TEST_DBG__NUM_SIMULTANEOUS_CHANNELS - 1,

        /* test buffer dimension and test buffer pointer */
        TEST_DBG__NUM_SIMULTANEOUS_CHANNELS, testChannelConfig__DBG,

        instanceInfo->isTransferCompletionInterruptConnected);

    areTestsPass = areTestsPass && Test_unchainedUnlinked(handle, instanceInfo, TEST_DBG__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__DBG);

        *testIteration += 1;
#endif
    strncpy(featureString, "A type single transfer (bCount = 1 AND cCount = 1), mix QDMA and DMA", 
        FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    Test_init_testChannelConfig(

        /* startChannel, endChannel */
        TEST_CH0, TEST_A_SINGLE_XFER_MIX_QDMA_DMA__NUM_SIMULTANEOUS_CHANNELS - 1,

        /* test buffer dimension and test buffer pointer */
        TEST_A_SINGLE_XFER_MIX_QDMA_DMA__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_SINGLE_XFER_MIX_QDMA_DMA,

        instanceInfo->isTransferCompletionInterruptConnected);

    areTestsPass = areTestsPass && (isTestPass = Test_unchainedUnlinked(handle, instanceInfo,
        TEST_A_SINGLE_XFER_MIX_QDMA_DMA__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_SINGLE_XFER_MIX_QDMA_DMA));
        
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    strncpy(featureString, "A type multi transfers (bCount != 1 AND cCount != 1)", 
        FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);

    Test_init_testChannelConfig(

        /* startChannel, endChannel */
        TEST_CH0, TEST_A_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS - 1,

        /* test buffer dimension and test buffer pointer */
        TEST_A_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_MULTI_XFER,

        instanceInfo->isTransferCompletionInterruptConnected);

    areTestsPass = areTestsPass && (isTestPass = Test_unchainedUnlinked(handle, instanceInfo,
        TEST_A_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_MULTI_XFER));
        
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    strncpy(featureString, "AB type multi transfers (cCount != 1)", 
        FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    Test_init_testChannelConfig(

        /* startChannel, endChannel */
        TEST_CH0, TEST_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS - 1,

        /* test buffer dimension and test buffer pointer */
        TEST_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__AB_MULTI_XFER,

        instanceInfo->isTransferCompletionInterruptConnected);

    areTestsPass = areTestsPass && (isTestPass = Test_unchainedUnlinked(handle, instanceInfo,
        TEST_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__AB_MULTI_XFER));
    
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    strncpy(featureString, "Mix of A and AB type multi transfers",
        FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    Test_init_testChannelConfig(

        /* startChannel, endChannel */
        TEST_CH0, TEST_A_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS - 1,

        /* test buffer dimension and test buffer pointer */
        TEST_A_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_AB_MULTI_XFER,

        instanceInfo->isTransferCompletionInterruptConnected);

    areTestsPass = areTestsPass && (isTestPass = Test_unchainedUnlinked(handle, instanceInfo,
        TEST_A_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_AB_MULTI_XFER));
        
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    return(areTestsPass);
}

/* chained transfers of A type single transfer (bCnt = 1 AND cCnt = 1) */
#define TEST_A_SINGLE_XFER_CHAIN__NUM_CHAINED_CHANNELS 4
testChannelConfig_t testChannelConfig__A_SINGLE_XFER_CHAIN
    [TEST_A_SINGLE_XFER_CHAIN__NUM_CHAINED_CHANNELS] =
{
    /* 1st channel */
    {
    .config.channelId = EDMA_UNTIED_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH0,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = false,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = true,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 2nd channel */
    {
    .config.channelId = EDMA_UNTIED_CH1,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH1,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH1,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = false,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = true,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL,
    .isUseSpecialStartAPI = false,
    },
    /* 3rd channel */
    {
    .config.channelId = EDMA_UNTIED_CH2,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH2,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH2,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH2][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH2][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH2,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = false,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = true,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    },
    /* 4th channel */
    {
    .config.channelId = EDMA_UNTIED_CH3,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH3,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH3,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH3][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH3][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH3,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,
    .isUseSpecialStartAPI = false,
    }
};

bool Test_chainedTransfers(EDMA_Handle handle, EDMA_instanceInfo_t *instInfo,
    uint8_t numChannels, testChannelConfig_t *testChannelConfig)
{
    EDMA_channelConfig_t *config, *fromChannelConfig, *toChannelConfig;
    uint8_t channelId;
    uint8_t channelType;
    int32_t errorCode;
    bool isTestPass = false;
    uint8_t channel;
    bool overallPass;
    volatile uint32_t timeout;
    uint8_t channelStart = testState.channelStart;
    uint8_t channelEnd = testState.channelEnd;
    bool isEnableChannel;

    if (channelStart >= channelEnd)
    {
        goto exit;
    }

    /* configure all channels */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        config = &testChannelConfig[channel].config;
        config->transferCompletionCallbackFxnArg = (uintptr_t) handle;
        channelType = config->channelType;
        channelId = config->channelId;
        if ((channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA) &&
            (instInfo->isChannelMapExist == false))
        {
            config->paramId = channelId;
        }

        /* cannot presently handle multi-transfer with chaining in the test code, so give
           error for multi-transfer */
        if (((config->paramSetConfig.transferType == (uint8_t)EDMA3_SYNC_A) &&
             ((config->paramSetConfig.bCount != 1) || (config->paramSetConfig.cCount != 1))) ||
            ((config->paramSetConfig.transferType == (uint8_t)EDMA3_SYNC_AB) && (config->paramSetConfig.cCount != 1)))
        {
            printf("Error: cannot presently handle multi-transfer with chaining in the test code\n");
            goto exit;
        }

        if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
        {
            /* no event triggering */
            isEnableChannel = false;
        }
        else
        {
            isEnableChannel = true;
        }

        if ((errorCode = EDMA_configChannel(handle, config, isEnableChannel)) != EDMA_NO_ERROR)
        {
            printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
            goto exit;
        }
    }

    /* chain channels */
    for(channel = channelStart; channel < channelEnd; channel++)
    {
        fromChannelConfig = &testChannelConfig[channel].config;
        toChannelConfig = &testChannelConfig[channel+1].config;

        /* check if transfer chaining and completion indication flags are set as expected */
        if (!(/*(fromChannelConfig->paramSetConfig.isFinalTransferInterruptEnabled == false) && */
             (fromChannelConfig->paramSetConfig.isIntermediateTransferInterruptEnabled == false) &&
             (fromChannelConfig->paramSetConfig.isFinalChainingEnabled == true) ||
             (fromChannelConfig->paramSetConfig.isIntermediateChainingEnabled == true)))
        {
            printf("Bad configuration of intermediate chained channel %d\n", channel);
            goto exit;
        }

        EDMA_chainChannels(handle, fromChannelConfig->paramId, toChannelConfig->channelId);
    }
    /* check last channel's flags */
    if (!((toChannelConfig->paramSetConfig.isFinalTransferInterruptEnabled == true) &&
         (toChannelConfig->paramSetConfig.isIntermediateTransferInterruptEnabled == false) &&
         (toChannelConfig->paramSetConfig.isFinalChainingEnabled == false) &&
         (toChannelConfig->paramSetConfig.isIntermediateChainingEnabled == false)))
    {
        printf("Bad configuration of last channel in the chain = %d\n", channelEnd);
        goto exit;
    }

    /* kick the first channel in the link */
    channel = channelStart;
    config = &testChannelConfig[channel].config;
    channelType = config->channelType;
    channelId = config->channelId;

    testState.channelState[channel].transferStartTimeStamp = Cycleprofiler_getTimeStamp();
    if (testChannelConfig[channel].isUseSpecialStartAPI == true)
    {
        if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
        {
            errorCode = EDMA_startDmaTransfer(handle, channelId);
        }
        else
        {
            errorCode = EDMA_startQdmaTransfer(handle, channelId);
        }
        if (errorCode != EDMA_NO_ERROR)
        {
            printf("Error: EDMA_Dma(Qdma)StartTransfer() of test channel %d failed with error code = %d\n", channel, errorCode);
            goto exit;
        }
    }
    else
    {
        if ((errorCode = EDMA_startTransfer(handle, channelId, channelType)) != EDMA_NO_ERROR)
        {
            printf("Error: EDMA_StartTransfer() of test channel %d failed with error code = %d\n", channel, errorCode);
            goto exit;
        }
    }

    /* wait until all last channel transfer done */
    timeout = TEST_TIMEOUT_CYCLES;
    channel = channelEnd;
    do
    {
        config = &testChannelConfig[channel].config;
        if (config->transferCompletionCallbackFxn == NULL)
        {
            if ((errorCode = EDMA_isTransferComplete(handle,
                    config->paramSetConfig.transferCompletionCode,
                    (bool *) &testState.channelState[channel].isTransferDone)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_isTransferComplete() failed with error code = %d\n", errorCode);
                goto exit;
            }
            testState.channelState[channel].transferEndTimeStamp = Cycleprofiler_getTimeStamp();
        }
        timeout--;
    } while ((testState.channelState[channel].isTransferDone == false) && (timeout != 0));

    if (timeout == 0)
    {
        printf("FAIL: Test timed out\n");
    }

    overallPass = true;
    /* check if completed transfers have been successful */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        if (testState.channelState[channelEnd].isTransferDone == true)
        {
            testState.channelState[channel].isPass = Test_isTransferSuccessful(channel);
            overallPass = overallPass && testState.channelState[channel].isPass;
        }
    }

    /* print results */
    isTestPass = true;
    if (testState.channelState[channelEnd].isTransferDone == true)
    {
        if (overallPass == true)
        {
            printf("Transfers completed successfully\n");
            printf("..transfer time in CPU cycles = %u\n", 
                testState.channelState[channelEnd].transferEndTimeStamp - 
                testState.channelState[channelStart].transferStartTimeStamp);
        }
        else
        {
            printf("FAIL: Transfers completed but unsuccessful\n");
            isTestPass = false;
        }
    }
    else
    {
        printf("FAIL: Transfers did not complete\n");
        isTestPass = false;
    }
exit:
    /* cleanup: important to disable channels after transfers are done before next test
       so that next test may not prematurely trigger transfer, one example is :
       A QDMA channel was programmed and transfer completed with a trigger word
       The same channel Id for next test is now a DMA channel. When configure API
       for this one is issued, it will only deal with DMA channel registers as the channel
       type is DMA and will leave the QDMA channel enabled. When the param Id (same
       as channel Id for AR devices) is configured for this DMA channel, it will trigger
       the QDMA channel that was left armed prior to config !!! (This is also a test
       for disableChannel API which was created for cleanup like this. */
    for(channel = channelStart; channel <= channelEnd; channel++)
    {
        config = &testChannelConfig[channel].config;
        channelType = config->channelType;
        channelId = config->channelId;
        EDMA_disableChannel(handle, channelId, channelType);
    }
    return(isTestPass);
}

bool Test_chainedTransfersSuite(EDMA_Handle handle,
    EDMA_instanceInfo_t *instanceInfo, uint32_t *testIteration)
{
    bool areTestPass = true;
    bool isTestPass;
#define FEATURE_STRING_LEN 200    
    char featureString[FEATURE_STRING_LEN];

    strncpy(featureString, "Chained Transfers of A type single transfer (bCount = 1 AND cCount = 1)", 
        FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    Test_init_testChannelConfig(

        /* startChannel, endChannel */
        TEST_CH0, TEST_A_SINGLE_XFER_CHAIN__NUM_CHAINED_CHANNELS - 1,

        /* test buffer dimension and test buffer pointer */
        TEST_A_SINGLE_XFER_CHAIN__NUM_CHAINED_CHANNELS,
        testChannelConfig__A_SINGLE_XFER_CHAIN,

        instanceInfo->isTransferCompletionInterruptConnected);

    areTestPass = areTestPass && (isTestPass = Test_chainedTransfers(handle, instanceInfo,
            TEST_A_SINGLE_XFER_CHAIN__NUM_CHAINED_CHANNELS - 1,
            testChannelConfig__A_SINGLE_XFER_CHAIN));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;
    return(areTestPass);
}

#define TEST_NUM_PARAM_LINKS 2
typedef struct testLinkedChannelsConfig_t_
{
    EDMA_channelConfig_t config;
    uint16_t paramId[TEST_NUM_PARAM_LINKS];
    EDMA_paramConfig_t  paramConfig[TEST_NUM_PARAM_LINKS];
} testLinkedChannelsConfig_t;

/* DMA linked transfers of A type single transfer (bCnt = 1 AND cCnt = 1) */
#define EDMA_PARAM_ID_FREE_0 EDMA_UNTIED_CH1
#define EDMA_PARAM_ID_FREE_1 EDMA_UNTIED_CH2
testLinkedChannelsConfig_t testChannelConfig__A_SINGLE_XFER_DMA_LINK =
{
    .config.channelId = EDMA_UNTIED_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
    .config.paramId = EDMA_UNTIED_CH0,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = true,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,

    .paramId[0] = EDMA_PARAM_ID_FREE_0,
    .paramConfig[0].paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .paramConfig[0].paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .paramConfig[0].paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.bCount = 1,
    .paramConfig[0].paramSetConfig.cCount = 1,
    .paramConfig[0].paramSetConfig.bCountReload = 0,
    .paramConfig[0].paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .paramConfig[0].paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .paramConfig[0].paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH1,
    .paramConfig[0].paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .paramConfig[0].paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .paramConfig[0].paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .paramConfig[0].paramSetConfig.isStaticSet = false,
    .paramConfig[0].paramSetConfig.isEarlyCompletion = false,
    .paramConfig[0].paramSetConfig.isFinalTransferInterruptEnabled = true,
    .paramConfig[0].paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .paramConfig[0].paramSetConfig.isFinalChainingEnabled = false,
    .paramConfig[0].paramSetConfig.isIntermediateChainingEnabled = false,
    .paramConfig[0].transferCompletionCallbackFxn = test_transferCompletionCallbackFxn,

    .paramId[1] = EDMA_PARAM_ID_FREE_1,
    .paramConfig[1].paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH2][0]),
    .paramConfig[1].paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH2][0]),
    .paramConfig[1].paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.bCount = 1,
    .paramConfig[1].paramSetConfig.cCount = 1,
    .paramConfig[1].paramSetConfig.bCountReload = 0,
    .paramConfig[1].paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .paramConfig[1].paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .paramConfig[1].paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH2,
    .paramConfig[1].paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .paramConfig[1].paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .paramConfig[1].paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .paramConfig[1].paramSetConfig.isStaticSet = false,
    .paramConfig[1].paramSetConfig.isEarlyCompletion = false,
    .paramConfig[1].paramSetConfig.isFinalTransferInterruptEnabled = true,
    .paramConfig[1].paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .paramConfig[1].paramSetConfig.isFinalChainingEnabled = false,
    .paramConfig[1].paramSetConfig.isIntermediateChainingEnabled = false,
    .paramConfig[1].transferCompletionCallbackFxn = NULL,
};

/* QDMA linked transfers of A type single transfer (bCnt = 1 AND cCnt = 1) */
#define EDMA_PARAM_ID_FREE_0 EDMA_UNTIED_CH1
#define EDMA_PARAM_ID_FREE_1 EDMA_UNTIED_CH2
testLinkedChannelsConfig_t testChannelConfig__A_SINGLE_XFER_QDMA_LINK =
{
    .config.channelId = EDMA_QDMA_CH0,
    .config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_QDMA,
    .config.paramId = EDMA_QDMA_CH0,
    .config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_SRC,
    .config.eventQueueId = EDMA_EVENT_QUEUE_ID_CH0,
    .config.paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH0][0]),
    .config.paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH0][0]),
    .config.paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.bCount = 1,
    .config.paramSetConfig.cCount = 1,
    .config.paramSetConfig.bCountReload = 0,
    .config.paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .config.paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .config.paramSetConfig.isStaticSet = false,
    .config.paramSetConfig.isEarlyCompletion = false,
    .config.paramSetConfig.isFinalTransferInterruptEnabled = false,
    .config.paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .config.paramSetConfig.isFinalChainingEnabled = false,
    .config.paramSetConfig.isIntermediateChainingEnabled = false,
    .config.transferCompletionCallbackFxn = NULL, //test_transferCompletionCallbackFxn,

    .paramId[0] = EDMA_PARAM_ID_FREE_0,
    .paramConfig[0].paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH1][0]),
    .paramConfig[0].paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH1][0]),
    .paramConfig[0].paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.bCount = 1,
    .paramConfig[0].paramSetConfig.cCount = 1,
    .paramConfig[0].paramSetConfig.bCountReload = 0,
    .paramConfig[0].paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[0].paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .paramConfig[0].paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .paramConfig[0].paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .paramConfig[0].paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .paramConfig[0].paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .paramConfig[0].paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .paramConfig[0].paramSetConfig.isStaticSet = false,
    .paramConfig[0].paramSetConfig.isEarlyCompletion = false,
    .paramConfig[0].paramSetConfig.isFinalTransferInterruptEnabled = false,
    .paramConfig[0].paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .paramConfig[0].paramSetConfig.isFinalChainingEnabled = false,
    .paramConfig[0].paramSetConfig.isIntermediateChainingEnabled = false,
    .paramConfig[0].transferCompletionCallbackFxn = NULL,

    .paramId[1] = EDMA_PARAM_ID_FREE_1,
    .paramConfig[1].paramSetConfig.sourceAddress = (uint32_t) (&testSrcBuf[TEST_CH2][0]),
    .paramConfig[1].paramSetConfig.destinationAddress = (uint32_t)(&testDstBuf[TEST_CH2][0]),
    .paramConfig[1].paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.bCount = 1,
    .paramConfig[1].paramSetConfig.cCount = 1,
    .paramConfig[1].paramSetConfig.bCountReload = 0,
    .paramConfig[1].paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t),
    .paramConfig[1].paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS,
    .paramConfig[1].paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A,
    .paramConfig[1].paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0,
    .paramConfig[1].paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
    .paramConfig[1].paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

    /* don't care because of linear addressing modes above */
    .paramConfig[1].paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

    .paramConfig[1].paramSetConfig.isStaticSet = true,
    .paramConfig[1].paramSetConfig.isEarlyCompletion = false,
    .paramConfig[1].paramSetConfig.isFinalTransferInterruptEnabled = true,
    .paramConfig[1].paramSetConfig.isIntermediateTransferInterruptEnabled = false,
    .paramConfig[1].paramSetConfig.isFinalChainingEnabled = false,
    .paramConfig[1].paramSetConfig.isIntermediateChainingEnabled = false,
    .paramConfig[1].transferCompletionCallbackFxn = NULL,
};

void Test_initLinkedConfig(testLinkedChannelsConfig_t *testConfig,
                           bool isTransferCompletionInterruptConnected)
{
    uint8_t channel;

    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS];
    uint8_t transferCompletionCode[TEST_MAX_NUM_SIMULTANEOUS_CHANNELS];

    for(channel = 0; channel < TEST_NUM_PARAM_LINKS; channel++)
    {
        if (channel == 0)
        {
            if (isTransferCompletionInterruptConnected == false)
            {
                testConfig->config.transferCompletionCallbackFxn = NULL;
            }

            transferCompletionCallbackFxn[channel] =
                testConfig->config.transferCompletionCallbackFxn;
            transferCompletionCode[channel] =
                testConfig->config.paramSetConfig.transferCompletionCode;
            }
        else
        {
            if (isTransferCompletionInterruptConnected == false)
            {
                testConfig->paramConfig[channel-1].transferCompletionCallbackFxn = NULL;
            }

            transferCompletionCallbackFxn[channel] =
                testConfig->paramConfig[channel-1].transferCompletionCallbackFxn;
            transferCompletionCode[channel] =
                testConfig->paramConfig[channel-1].paramSetConfig.transferCompletionCode;
        }
    }

    Test_init(0, TEST_NUM_PARAM_LINKS, transferCompletionCallbackFxn, transferCompletionCode);

}

void Test_initLinkedConfig_translateAddrs(testLinkedChannelsConfig_t *testConfig)
{
    uint8_t channel;

    testConfig->config.paramSetConfig.sourceAddress = 
        ADDR_TRANSLATE_CPU_TO_EDMA(testConfig->config.paramSetConfig.sourceAddress);
    
    testConfig->config.paramSetConfig.destinationAddress = 
        ADDR_TRANSLATE_CPU_TO_EDMA(testConfig->config.paramSetConfig.destinationAddress);

    for(channel = 0; channel < TEST_NUM_PARAM_LINKS; channel++)
    {

        testConfig->paramConfig[channel].paramSetConfig.sourceAddress = 
            ADDR_TRANSLATE_CPU_TO_EDMA(testConfig->paramConfig[channel].paramSetConfig.sourceAddress);
        testConfig->paramConfig[channel].paramSetConfig.destinationAddress= 
            ADDR_TRANSLATE_CPU_TO_EDMA(testConfig->paramConfig[channel].paramSetConfig.destinationAddress);
    }
}


bool Test_linkedTransfers(EDMA_Handle handle, EDMA_instanceInfo_t *instInfo,
    testLinkedChannelsConfig_t *testLinkChannelConfig)
{
    EDMA_channelConfig_t *config;
    uint8_t channelId;
    uint16_t fromParamId, toParamId, param;
    uint8_t channelType;
    int32_t errorCode;
    bool isTestPass = false;
    uint8_t channel;
    bool overallPass;
    volatile uint32_t timeout;
    uint8_t transferCompletionCode;
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn;
    bool isEnableChannel;

    /* configure the channel */

    config = &testLinkChannelConfig[0].config;
    config->transferCompletionCallbackFxnArg = (uintptr_t) handle;
    channelType = config->channelType;
    channelId = config->channelId;
    if ((channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA) &&
        (instInfo->isChannelMapExist == false))
    {
        config->paramId = channelId;
    }

    /* cannot presently handle multi-transfer with chaining in the test code, so give
       error for multi-transfer */
    if (((config->paramSetConfig.transferType == (uint8_t)EDMA3_SYNC_A) &&
         ((config->paramSetConfig.bCount != 1) || (config->paramSetConfig.cCount != 1))) ||
        ((config->paramSetConfig.transferType == (uint8_t)EDMA3_SYNC_AB) && (config->paramSetConfig.cCount != 1)))
    {
        printf("Error: cannot presently handle multi-transfer with chaining in the test code\n");
        goto exit;
    }

    /* TODO check to make sure all transfer completion codes in the link set are different */
    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
    {
        /* no event triggering */
        isEnableChannel = false;
    }
    else
    {
        isEnableChannel = true;
    }

    if ((errorCode = EDMA_configChannel(handle, config, isEnableChannel)) != EDMA_NO_ERROR)
    {
        printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        goto exit;
    }

    /* configure param sets */
    for(param = 0; param < TEST_NUM_PARAM_LINKS; param++)
    {
        testLinkChannelConfig[0].paramConfig[param].transferCompletionCallbackFxnArg = (uintptr_t) handle;
        if ((errorCode = EDMA_configParamSet(handle, testLinkChannelConfig[0].paramId[param],
            &testLinkChannelConfig[0].paramConfig[param])) != EDMA_NO_ERROR)
        {
            printf("Error: EDMA_configParamSet() failed with error code = %d\n", errorCode);
            goto exit;
        }
    }

    /* link param sets */
    fromParamId = config->paramId;
    for(param = 0; param < TEST_NUM_PARAM_LINKS; param++)
    {
        toParamId = testLinkChannelConfig[0].paramId[param];
        EDMA_linkParamSets(handle, fromParamId, toParamId);
        fromParamId = toParamId;
    }

    if (config->channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
    {
        for(channel = 0; channel <= TEST_NUM_PARAM_LINKS; channel++)
        {
            testState.channelState[channel].transferStartTimeStamp = Cycleprofiler_getTimeStamp();

            /* trigger the channel */
            if ((errorCode = EDMA_startTransfer(handle, channelId, channelType)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_StartTransfer() of test channel %d failed with error code = %d\n", channel, errorCode);
                goto exit;
            }

            if (channel == 0)
            {
                transferCompletionCode = config->paramSetConfig.transferCompletionCode;
                transferCompletionCallbackFxn = config->transferCompletionCallbackFxn;
            }
            else
            {
                transferCompletionCode = testLinkChannelConfig[0].
                    paramConfig[channel-1].paramSetConfig.transferCompletionCode;
                transferCompletionCallbackFxn = testLinkChannelConfig[0].
                    paramConfig[channel-1].transferCompletionCallbackFxn;
            }

            /* wait until channel transfer done */
            timeout = TEST_TIMEOUT_CYCLES;
            do
            {
                if (transferCompletionCallbackFxn == NULL)
                {
                    if ((errorCode = EDMA_isTransferComplete(handle,
                            transferCompletionCode,
                            (bool *) &testState.channelState[channel].isTransferDone)) != EDMA_NO_ERROR)
                    {
                        printf("Error: EDMA_isTransferComplete() failed with error code = %d\n", errorCode);
                        goto exit;
                    }
                    testState.channelState[channel].transferEndTimeStamp = Cycleprofiler_getTimeStamp();
                }
                timeout--;
            } while ((testState.channelState[channel].isTransferDone == false) && (timeout != 0));

            if (timeout == 0)
            {
                printf("FAIL: Test transfer #%d of linked transfers timed out\n", channel);
                goto exit;
            }
        }
    }
    else /* QDMA channel */
    {
        testState.channelState[0].transferStartTimeStamp = Cycleprofiler_getTimeStamp();

        /* trigger the QDMA channel */
        if ((errorCode = EDMA_startTransfer(handle, channelId, channelType)) != EDMA_NO_ERROR)
        {
            printf("Error: EDMA_StartTransfer() of test channel %d failed with \
                error code = %d\n", channelId, errorCode);
            goto exit;
        }

         /* wait until last transfer in the link done */
        timeout = TEST_TIMEOUT_CYCLES;
        do
        {
            if (testLinkChannelConfig[0].paramConfig[TEST_NUM_PARAM_LINKS-1].transferCompletionCallbackFxn == NULL)
            {
                if ((errorCode = EDMA_isTransferComplete(handle,
                        testLinkChannelConfig[0].paramConfig[TEST_NUM_PARAM_LINKS-1].
                            paramSetConfig.transferCompletionCode,
                        (bool *) &testState.channelState[TEST_NUM_PARAM_LINKS].isTransferDone)) != EDMA_NO_ERROR)
                {
                    printf("Error: EDMA_isTransferComplete() failed with error code = %d\n", errorCode);
                    goto exit;
                }
                testState.channelState[TEST_NUM_PARAM_LINKS].transferEndTimeStamp = Cycleprofiler_getTimeStamp();
            }
            timeout--;
        } while ((testState.channelState[TEST_NUM_PARAM_LINKS].isTransferDone == false) && (timeout != 0));

        if (timeout == 0)
        {
            printf("FAIL: Test transfer #%d of linked transfers timed out\n", TEST_NUM_PARAM_LINKS);
            goto exit;
        }
        /* since the last transfer was done, mark previous as done so that the next
           code (which is common between DMA and QDMA) works, in linked QDMA, the
           last transfer cannot complete if previous ones did not complete */
        for(channel = 0; channel < TEST_NUM_PARAM_LINKS; channel++)
        {
            testState.channelState[channel].isTransferDone = true;
        }
    }

    overallPass = true;
    /* check if completed transfers have been successful */
    for(channel = 0; channel <= TEST_NUM_PARAM_LINKS; channel++)
    {
        if (testState.channelState[channel].isTransferDone == true)
        {
            testState.channelState[channel].isPass = Test_isTransferSuccessful(channel);
            overallPass = overallPass && testState.channelState[channel].isPass;
        }
    }

    /* print results */
    if (testState.channelState[TEST_NUM_PARAM_LINKS].isTransferDone == true)
    {
        if (overallPass == true)
        {
            printf("Transfers completed successfully\n");
            printf("..transfer time in CPU cycles = %u\n", 
                testState.channelState[TEST_NUM_PARAM_LINKS].transferEndTimeStamp - 
                testState.channelState[0].transferStartTimeStamp);
            isTestPass = true;
        }
        else
        {
            printf("FAIL: Transfers completed but unsuccessful\n");
        }
    }
    else
    {
        printf("FAIL: Transfers did not complete\n");
    }

exit:
    /* cleanup: important to disable channels after transfers are done before next test
       so that next test may not prematurely trigger transfer, one example is :
       A QDMA channel was programmed and transfer completed with a trigger word
       The same channel Id for next test is now a DMA channel. When configure API
       for this one is issued, it will only deal with DMA channel registers as the channel
       type is DMA and will leave the QDMA channel enabled. When the param Id (same
       as channel Id for AR devices) is configured for this DMA channel, it will trigger
       the QDMA channel that was left armed prior to config !!! (This is also a test
       for disableChannel API which was created for cleanup like this. */
    EDMA_disableChannel(handle, channelId, channelType);

    return(isTestPass);
}

bool Test_linkedTransfersSuite(EDMA_Handle handle,
    EDMA_instanceInfo_t *instanceInfo, uint32_t *testIteration)
{
    bool areTestPass = true;
    bool isTestPass;
#define FEATURE_STRING_LEN 200
    char featureString[FEATURE_STRING_LEN];

    strncpy(featureString, "DMA Linked Transfers of A type single transfer (bCount = 1 AND cCount = 1)",
        FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    Test_initLinkedConfig(&testChannelConfig__A_SINGLE_XFER_DMA_LINK,
                          instanceInfo->isTransferCompletionInterruptConnected);
    areTestPass = areTestPass && (isTestPass = Test_linkedTransfers(handle, instanceInfo,
        &testChannelConfig__A_SINGLE_XFER_DMA_LINK));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    strncpy(featureString, "QDMA Linked Transfers of A type single transfer (bCount = 1 AND cCount = 1)",
        FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    Test_initLinkedConfig(&testChannelConfig__A_SINGLE_XFER_QDMA_LINK,
                          instanceInfo->isTransferCompletionInterruptConnected);
    areTestPass = areTestPass && (isTestPass = Test_linkedTransfers(handle, instanceInfo,
        &testChannelConfig__A_SINGLE_XFER_QDMA_LINK));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    return(areTestPass);
}

void Test_initDefaultErrorTestConfig(EDMA_channelConfig_t *config, uint8_t channelType)
{
    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
    {
        config->channelId = EDMA_UNTIED_CH0;
        config->channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
        config->paramId = EDMA_UNTIED_CH0;
        config->eventQueueId = EDMA_EVENT_QUEUE_ID_CH0;
        config->paramSetConfig.sourceAddress = (uint32_t) ADDR_TRANSLATE_CPU_TO_EDMA((uint32_t)(&testSrcBuf[TEST_CH0][0])),
        config->paramSetConfig.destinationAddress = (uint32_t)ADDR_TRANSLATE_CPU_TO_EDMA((uint32_t)(&testDstBuf[TEST_CH0][0])),
        config->paramSetConfig.aCount = TEST_BUF_LENGTH * sizeof(uint8_t);
        config->paramSetConfig.bCount = 1;
        config->paramSetConfig.cCount = 1;
        config->paramSetConfig.bCountReload = 0;
        config->paramSetConfig.sourceBindex = TEST_BUF_LENGTH * sizeof(uint8_t);
        config->paramSetConfig.destinationBindex = TEST_BUF_LENGTH * sizeof(uint8_t);
        config->paramSetConfig.sourceCindex = TEST_BUF_LENGTH * sizeof(uint8_t);
        config->paramSetConfig.destinationCindex = TEST_BUF_LENGTH * sizeof(uint8_t);
        config->paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
        config->paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
        config->paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0;
        config->paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
        config->paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

        /* don't care because of linear addressing modes above */
        config->paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

        config->paramSetConfig.isStaticSet = false;
        config->paramSetConfig.isEarlyCompletion = false;
        config->paramSetConfig.isFinalTransferInterruptEnabled = false;
        config->paramSetConfig.isIntermediateTransferInterruptEnabled = true;
        config->paramSetConfig.isFinalChainingEnabled = false;
        config->paramSetConfig.isIntermediateChainingEnabled = false;
        config->transferCompletionCallbackFxn = NULL; //test_transferCompletionCallbackFxn;
    }
    else
    {
        config->channelId = EDMA_QDMA_CH0;
        config->channelType = (uint8_t)EDMA3_CHANNEL_TYPE_QDMA;
        config->paramId = TEST_QDMA_PARAM_ID_CH0_START;
        config->eventQueueId = EDMA_EVENT_QUEUE_ID_CH0;
        config->qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_OPT;
        config->paramSetConfig.sourceAddress = (uint32_t) ADDR_TRANSLATE_CPU_TO_EDMA((uint32_t)(&testSrcBuf[TEST_CH0][0])),
        config->paramSetConfig.destinationAddress = (uint32_t)ADDR_TRANSLATE_CPU_TO_EDMA((uint32_t)(&testDstBuf[TEST_CH0][0])),
        config->paramSetConfig.aCount = TEST_MAX_A_COUNT * sizeof(uint8_t);
        config->paramSetConfig.bCount = TEST_MAX_B_COUNT;
        config->paramSetConfig.cCount = TEST_MAX_C_COUNT;
        config->paramSetConfig.bCountReload = TEST_MAX_B_COUNT;
        config->paramSetConfig.sourceBindex = TEST_MAX_A_COUNT * sizeof(uint8_t);
        config->paramSetConfig.destinationBindex = TEST_MAX_A_COUNT * sizeof(uint8_t);
        config->paramSetConfig.sourceCindex = TEST_MAX_A_COUNT * sizeof(uint8_t);
        config->paramSetConfig.destinationCindex = TEST_MAX_A_COUNT * sizeof(uint8_t);
        config->paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
        config->paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
        config->paramSetConfig.transferCompletionCode = TEST_TRANSFER_COMPLETION_CODE_CH0;
        config->paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,
        config->paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR,

        /* don't care because of linear addressing modes above */
        config->paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT,

        config->paramSetConfig.isStaticSet = false;
        config->paramSetConfig.isEarlyCompletion = false;
        config->paramSetConfig.isFinalTransferInterruptEnabled = true;
        config->paramSetConfig.isIntermediateTransferInterruptEnabled = true;
        config->paramSetConfig.isFinalChainingEnabled = false;
        config->paramSetConfig.isIntermediateChainingEnabled = false;
        config->transferCompletionCallbackFxn = NULL; //test_transferCompletionCallbackFxn;
    }
}

bool Test_missedEvents(EDMA_Handle handle, uint8_t channelType, bool isErrorInterruptConnected)
{
    bool isTestPass = false;
    EDMA_channelConfig_t config;
    /* select numbers below represent a compromise between testing all channels -
       one channel at a time (for verifying selectivity) and all channels simultaneous
       (for verifying simultaneity) */
    uint8_t channelId[] = {0}; //, 31, 32, EDMA_NUM_DMA_CHANNELS - 1};
    uint8_t channel, testChannel;
    volatile uint8_t iter;
    int32_t errorCode;
    uint8_t numChannels;
    bool isFound;
    uint32_t missCount;
    uint8_t numAllChannels;
    bool isEnableChannel;
    EDMA_errorInfo_t errorInfo;
    bool isAnyError;
    uint32_t i = 0;

    Test_initBufs(0, 0);

    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
        channelId[0] = 0;
       #if 0
        channelId[1] = 1;
        channelId[2] = EDMA_NUM_QDMA_CHANNELS -2;
        channelId[3] = EDMA_NUM_QDMA_CHANNELS -1;
       #endif
    }

    numChannels = sizeof(channelId)/sizeof(uint8_t);

    Test_initDefaultErrorTestConfig(&config, channelType);
    testState.isTestingError = true;
    memset(&testState.errorInfo, 0, sizeof(testState.errorInfo));
    testState.errorCallbackFxnCount = 0;

    config.channelType = channelType;
    config.transferCompletionCallbackFxnArg = (uintptr_t) handle;
#if 0
    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
        config.qdmaParamTriggerWordOffset = EDMA_QDMA_TRIG_WORD_OFFSET_SRC;
        config.paramSetConfig.isStaticSet = true;
    }
#endif

    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
    {
        /* no event triggering */
        isEnableChannel = false;
    }
    else
    {
        isEnableChannel = true;
    }

    /* configure channels */
    for (channel = 0; channel < numChannels; channel++)
    {
        config.channelId = channelId[channel];
        config.paramId = config.channelId;
        if ((errorCode = EDMA_configChannel(handle, &config, isEnableChannel)) != EDMA_NO_ERROR)
        {
            printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
            goto exit;
        }
    }

    /* start all channels doubly to create missed events */
    for (channel = 0; channel < numChannels; channel++)
    {
        for (iter = 0; iter < 2; iter++)
        {
            if ((errorCode = EDMA_startTransfer(handle, channelId[channel],
                config.channelType)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_StartTransfer() of test channel %d\
                    failed with error code = %d\n", channel, errorCode);
                goto exit;
            }
        }
    }

    if (isErrorInterruptConnected)
    {
        while (i++ <= 10000U)
        {
            if (testState.errorCallbackFxnCount)
            {
                break;
            }
        }
    }
    else
    {
        while (i++ <= 10000U)
        {
            if ((errorCode = EDMA_getErrorStatus(handle, &isAnyError, &errorInfo)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_getErrorStatus() failed with error code = %d\n", errorCode);
            }

            if (isAnyError == true)
            {
                Test_ErrorCallbackFxn(handle, &errorInfo);
                break;
            }
        }
    }

    if (i > 10000U)
    {
        printf("FAIL: Did not trigger error\n");
        goto exit;
    }

    if (isErrorInterruptConnected)
    {
        /* wait for some time for any re-triggerings to die down, assumptions is
         * die-down will happen in the delay period */
        Test_delay(100000U);
    }
    else
    {
        do
        {
            /* allow some delay for re-triggerings because error conditions have been cleared
             * so that eventually we have cleaned error conditions else it can affect
             * subsequent tests */
            Test_delay(1000);

            /* clean up re-triggerings which can happen as older state is cleared.  */
            if ((errorCode = EDMA_getErrorStatus(handle, &isAnyError, &errorInfo)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_getErrorStatus() failed with error code = %d\n", errorCode);
                goto exit;
            }
        }
        while (isAnyError == true);
    }

    /* errors should have happened by now, check reported errors */
    isTestPass = true;
    for (channel = 0; channel < numChannels; channel++)
    {
        if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
        {
            missCount = testState.errorInfo.dmaChannelEventMissCount[channelId[channel]];
        }
        else
        {
            missCount = testState.errorInfo.qdmaChannelEventMissCount[channelId[channel]];
        }
        if (missCount == 0)
        {
            printf("FAIL: Did not detect expected missed event of channel %d\n",
                channelId[channel]);
            isTestPass = false;
        }
    }
    /* check if other channels falsely missed */
    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
    {
        numAllChannels = EDMA_NUM_DMA_CHANNELS;
    }
    else
    {
        numAllChannels = EDMA_NUM_QDMA_CHANNELS;
    }
    for (channel = 0; channel < numAllChannels; channel++)
    {
        /* is channel in the test list? */
        isFound = false;
        for (testChannel = 0; testChannel < numChannels; testChannel++)
        {
            if (channel == channelId[testChannel]) {
                isFound = true;
                break;
            }
        }
        /* check if unexpected missed event on channels not in test */
        if (isFound == false)
        {
            if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA)
            {
                missCount = testState.errorInfo.dmaChannelEventMissCount[channel];
            }
            else
            {
                missCount = testState.errorInfo.qdmaChannelEventMissCount[channel];
            }
            if (missCount != 0)
            {
                printf("FAIL: Unexpected missed event of channel %d\n", channel);
                isTestPass = false;
            }
        }
    }
    if (isTestPass == true)
    {
        printf("All expected missed events detected and no false detection\n");
    }
exit:
    return(isTestPass);
}

bool Test_transferControllerErrors(EDMA_Handle handle, uint8_t testId,
                                   bool *isTransferControllerErrorInterruptConnected)
{
    bool isTestPass = false;
    EDMA_channelConfig_t config;
    int32_t errorCode;
    EDMA_transferControllerErrorInfo_t *info;
    uint32_t i = 0;
    EDMA_transferControllerErrorInfo_t errorInfo;
    bool isAnyError;

    Test_initBufs(0, 0);

    Test_initDefaultErrorTestConfig(&config, EDMA3_CHANNEL_TYPE_DMA);
    testState.isTestingTransferControllerError = true;
    memset(&testState.transferControllerErrorInfo, 0, sizeof(testState.transferControllerErrorInfo));
    testState.transferControllerErrorInfo[config.eventQueueId].callbackFxnCount = 0;

    config.transferCompletionCallbackFxnArg = (uintptr_t) handle;

    if (testId == TEST_INVALID_SOURCE_ADDRESS_ID)
    {
        config.paramSetConfig.sourceAddress = 0;
    }
    else if (testId == TEST_INVALID_DESTINATION_ADDRESS_ID)
    {
        config.paramSetConfig.destinationAddress = 0;
    } else if (testId == TEST_INVALID_TRANSFER_REQUEST_ID)
    {
        /* Note: Simulating this error with aCount or bCount of 0 will not help. This
         * condition is only at the TC level but CC will submit to TC a transfer that has
         * aCount or bCount of 0. So simulating this error is only possible with
         * FIFO wrap case with source or destination address violating alignment rules
         * or source or destination B index violating the multiplicity requirement.
         */
        config.paramSetConfig.sourceAddressingMode = EDMA3_ADDRESSING_MODE_FIFO_WRAP;
        config.paramSetConfig.fifoWidth = EDMA3_FIFO_WIDTH_128BIT;
        /* make this not an even multiple of 256-bits */
        config.paramSetConfig.sourceBindex = 16;
    }

    /* configure channel */
    if ((errorCode = EDMA_configChannel(handle, &config, true)) != EDMA_NO_ERROR)
    {
        printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if ((errorCode = EDMA_startTransfer(handle, config.channelId, config.channelType)) != EDMA_NO_ERROR)
    {
        printf("Error: EDMA_StartTransfer() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if (isTransferControllerErrorInterruptConnected[config.eventQueueId])
    {
        while (i++ <= 10000U)
        {
            if (testState.transferControllerErrorInfo[config.eventQueueId].callbackFxnCount)
            {
                break;
            }
        }
    }
    else
    {
        while (i++ <= 10000U)
        {
            if ((errorCode = EDMA_getTransferControllerErrorStatus(handle, config.eventQueueId,
                &isAnyError, &errorInfo)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_getTransferControllerErrorStatus() failed with error code = %d\n", errorCode);
                goto exit;
            }
            if (isAnyError)
            {
                Test_transferControllerErrorCallbackFxn(handle, &errorInfo);
                break;
            }
        }
    }

    if (i > 10000U)
    {
        printf("FAIL: Did not trigger error\n");
        goto exit;
    }

    if (isTransferControllerErrorInterruptConnected[config.eventQueueId])
    {
        /* wait for some time for any re-triggerings to die down, assumptions is
         * die-down will happen in the delay period */
        Test_delay(100000U);
    }
    else
    {
        do
        {
            /* allow some delay for re-triggerings because error conditions have been cleared
             * so that eventually we have cleaned error conditions else it can affect
             * subsequent tests */
            Test_delay(1000);

            /* clean up re-triggerings which can happen as older state is cleared.  */
            if ((errorCode = EDMA_getTransferControllerErrorStatus(handle, config.eventQueueId,
                &isAnyError, &errorInfo)) != EDMA_NO_ERROR)
            {
                printf("Error: EDMA_getTransferControllerErrorStatus() failed with error code = %d\n", errorCode);
                goto exit;
            }
        }
        while (isAnyError == true);
    }

    isTestPass = false;
    info = &testState.transferControllerErrorInfo[config.eventQueueId].info;

    if (testId == TEST_INVALID_SOURCE_ADDRESS_ID)
    {
        if (info->isBusError == true)
        {
            if ((info->busErrorInfo.errorCode >= 0x1) &&
                (info->busErrorInfo.errorCode <= 0x7))
            {
                isTestPass = true;
                printf("Detected invalid source address\n");
            }
            else
            {
                printf("FAIL: Did not detect invalid source address\n");
            }
        }
    }
    else if (testId == TEST_INVALID_DESTINATION_ADDRESS_ID)
    {
        if (info->isBusError == true)
        {
            if ((info->busErrorInfo.errorCode >= 0x8) &&
                (info->busErrorInfo.errorCode <= 0xF))
            {
                isTestPass = true;
                printf("Detected invalid destination address\n");
            }
            else
            {
                printf("FAIL: Did not detect invalid destination address\n");
            }
        }
    } else if (testId == TEST_INVALID_TRANSFER_REQUEST_ID)
    {
        if (info->isTransferRequestError == true)
        {
            isTestPass = true;
            printf("Detected invalid transfer request\n");
        }
        else
        {
            printf("FAIL: Did not detect invalid transfer request\n");
        }
    }

exit:
    return(isTestPass);
}

bool Test_outstandingTransfersExceeded(void)
{
    bool isTestPass = false;

    return(isTestPass);
}

bool Test_queueThresholdExceeded(void)
{
    bool isTestPass = false;

    return(isTestPass);
}

bool Test_errorDetectionSuite(EDMA_Handle handle,
    EDMA_instanceInfo_t *instanceInfo, uint32_t *testIteration)
{
    bool areTestsPass = true;
    bool isTestPass;
#define FEATURE_STRING_LEN 200    
    char featureString[FEATURE_STRING_LEN];

    strncpy(featureString, "Missed DMA events", FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    areTestsPass = areTestsPass && (isTestPass = Test_missedEvents(handle, (uint8_t)EDMA3_CHANNEL_TYPE_DMA,
                                                     instanceInfo->isErrorInterruptConnected));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

/* This is being discussed with Kyle */
#if 0
    strncpy(featureString, "Missed QDMA events", FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    areTestsPass = areTestsPass && (isTestPass = Test_missedEvents(handle, (uint8_t)EDMA3_CHANNEL_TYPE_QDMA,
                                                     instanceInfo->isErrorInterruptConnected));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;
#endif

#if 0
    printf("\nTest#%d...\n", *testIteration);
    strncpy(featureString, "Queue threshold exceeded", FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    areTestsPass = areTestsPass && (isTestPass = Test_outstandingTransfersExceeded());
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    printf("\nTest#%d..\n", *testIteration);
    strncpy(featureString, "Outstanding transfers exceeded", FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    areTestsPass = areTestsPass && (isTestPass = Test_queueThresholdExceeded());
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;
#endif

    strncpy(featureString, "Invalid source address", FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    areTestsPass = areTestsPass && (isTestPass = Test_transferControllerErrors(handle, TEST_INVALID_SOURCE_ADDRESS_ID,
                                                     &instanceInfo->isTransferControllerErrorInterruptConnected[0]));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    strncpy(featureString, "Invalid transfer request", FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    areTestsPass = areTestsPass && (isTestPass = Test_transferControllerErrors(handle, TEST_INVALID_TRANSFER_REQUEST_ID,
                                                     &instanceInfo->isTransferControllerErrorInterruptConnected[0]));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    strncpy(featureString, "Invalid destination address", FEATURE_STRING_LEN);
    printf("\nTest#%d..%s\n", *testIteration, featureString);
    areTestsPass = areTestsPass && (isTestPass = Test_transferControllerErrors(handle, TEST_INVALID_DESTINATION_ADDRESS_ID,
                                                     &instanceInfo->isTransferControllerErrorInterruptConnected[0]));
    Test_indicateResultToAutomation(featureString, isTestPass);
    *testIteration += 1;

    return(areTestsPass);
}

//#define TEST_DBG_DISABLE_UNLINKED_UNCHAINED
//#define TEST_DBG_DISABLE_CHAINED
//#define TEST_DBG_DISABLE_LINKED
//#define TEST_DBG_DISABLE_ERROR

bool Test_instance(uint8_t instanceId)
{
    EDMA_Handle         handle;
    int32_t errorCode;
    EDMA_instanceInfo_t instanceInfo;
    EDMA_errorConfig_t errorConfig;
    bool isTestPass = false;
    uint32_t testIteration = 1;
    bool isAnyTransferControllerErrorInterruptConnected;
    uint32_t tc;

    /* Initialize the edma instance to be tested */
    EDMA_init(instanceId);

    /* Open the first edma Instance */
    handle = EDMA_open(instanceId, &errorCode, &instanceInfo);
    if (handle == NULL)
    {
        printf("Error: Unable to open the edma Instance, erorCode = %d\n", errorCode);
        goto exit;
    }
    testEdmaHandle = handle;

    errorConfig.isConfigAllEventQueues = true;
    errorConfig.isConfigAllTransferControllers = true;
    errorConfig.isEventQueueThresholdingEnabled = true;
    errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    if (instanceInfo.isErrorInterruptConnected == true)
    {
        errorConfig.callbackFxn = Test_ErrorCallbackFxn;
    }
    else
    {
        errorConfig.callbackFxn = NULL;
    }

    isAnyTransferControllerErrorInterruptConnected = false;
    for(tc = 0; tc < instanceInfo.numEventQueues; tc++)
    {
        if (instanceInfo.isTransferControllerErrorInterruptConnected[tc] == true)
        {
            isAnyTransferControllerErrorInterruptConnected = true;
            break;
        }
    }

    if (isAnyTransferControllerErrorInterruptConnected == true)
    {
        errorConfig.transferControllerCallbackFxn = Test_transferControllerErrorCallbackFxn;
    }
    else
    {
        errorConfig.transferControllerCallbackFxn = NULL;
    }
    if ((errorCode = EDMA_configErrorMonitoring(handle, &errorConfig)) != EDMA_NO_ERROR)
    {
        printf("Debug: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errorCode);
        goto exit;
    }

    isTestPass = true;
#ifndef TEST_DBG_DISABLE_UNLINKED_UNCHAINED
    printf("\n==================================================\n");
    printf("Testing simultaneous unlinked unchained transfers\n");
    printf("==================================================\n");
    isTestPass = isTestPass &&
        Test_simultaneousUnchainedUnlinkedTransfersSuite(handle, &instanceInfo, &testIteration);
#endif
#ifndef TEST_DBG_DISABLE_CHAINED
    printf("\n==================================================\n");
    printf("Testing chained transfers\n");
    printf("==================================================\n");
    isTestPass = isTestPass &&
        Test_chainedTransfersSuite(handle, &instanceInfo, &testIteration);
#endif
#ifndef TEST_DBG_DISABLE_LINKED
    printf("\n==================================================\n");
    printf("Testing linked transfers\n");
    printf("==================================================\n");
    isTestPass = isTestPass &&
    Test_linkedTransfersSuite(handle, &instanceInfo, &testIteration);
#endif
#ifndef TEST_DBG_DISABLE_ERROR
    printf("\n==================================================\n");
    printf("Testing Error Detection\n");
    printf("==================================================\n");
    isTestPass = isTestPass &&
    Test_errorDetectionSuite(handle, &instanceInfo, &testIteration);
#endif
    if ((errorCode = EDMA_close(handle)) != EDMA_NO_ERROR)
    {
        printf("Debug: edma Instance %p closing error, errorCode = %d\n", handle, errorCode);
    }

    if (isTestPass == true)
    {
        printf("All Tests of Instance %d PASSED\n", instanceId);
    }
    else
    {
        printf("All Tests of Instance %d did NOT Pass\n", instanceId);
    }

exit:
    return(isTestPass);
}


void Test_updateTestConfig()
{

#ifdef testChannelConfig__DBG
    Test_init_testChannelConfig_translateAddrs(
        /* startChannel, endChannel */
        TEST_CH0, TEST_DBG__NUM_SIMULTANEOUS_CHANNELS - 1,
        /* test buffer dimension and test buffer pointer */
        TEST_DBG__NUM_SIMULTANEOUS_CHANNELS, testChannelConfig__DBG);
#endif
    Test_init_testChannelConfig_translateAddrs(
        /* startChannel, endChannel */
        TEST_CH0, TEST_A_SINGLE_XFER_MIX_QDMA_DMA__NUM_SIMULTANEOUS_CHANNELS - 1,
        /* test buffer dimension and test buffer pointer */
        TEST_A_SINGLE_XFER_MIX_QDMA_DMA__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_SINGLE_XFER_MIX_QDMA_DMA);

    Test_init_testChannelConfig_translateAddrs(
        /* startChannel, endChannel */
        TEST_CH0, TEST_A_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS - 1,
        /* test buffer dimension and test buffer pointer */
        TEST_A_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_MULTI_XFER);

    Test_init_testChannelConfig_translateAddrs(
        /* startChannel, endChannel */
        TEST_CH0, TEST_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS - 1,
        /* test buffer dimension and test buffer pointer */
        TEST_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__AB_MULTI_XFER);

    Test_init_testChannelConfig_translateAddrs(
        /* startChannel, endChannel */
        TEST_CH0, TEST_A_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS - 1,
        /* test buffer dimension and test buffer pointer */
        TEST_A_AB_MULTI_XFER__NUM_SIMULTANEOUS_CHANNELS,
        testChannelConfig__A_AB_MULTI_XFER);
    
    Test_init_testChannelConfig_translateAddrs(
            /* startChannel, endChannel */
            TEST_CH0, TEST_A_SINGLE_XFER_CHAIN__NUM_CHAINED_CHANNELS - 1,    
            /* test buffer dimension and test buffer pointer */
            TEST_A_SINGLE_XFER_CHAIN__NUM_CHAINED_CHANNELS,
            testChannelConfig__A_SINGLE_XFER_CHAIN);

    Test_initLinkedConfig_translateAddrs(&testChannelConfig__A_SINGLE_XFER_DMA_LINK);
    
    Test_initLinkedConfig_translateAddrs(&testChannelConfig__A_SINGLE_XFER_QDMA_LINK);

}

void Test_task(UArg arg0, UArg arg1)
{
    uint8_t numInstances;
    bool isTestPass = true;

    /* this necessary otherwise optimizer optimizes out if previous instance
       tests fail */
    volatile bool result;
    
    /* Initialize test logger */
    MCPI_Initialize();

    System_printf ("Updating the addresses in testConfig for access from EDMA\n");
    /* Run-time update of test configs */
    Test_updateTestConfig();


    numInstances = EDMA_getNumInstances();

    /* test one instance at a time, simultaneous instance testing not yet supported */
    for (gInstanceId = 0; gInstanceId < numInstances; gInstanceId++)
    {
        printf("\n=====================================================\n");
        printf("============ Testing EDMA instance #%d ==============\n", gInstanceId);
        printf("=====================================================\n");
        result = Test_instance(gInstanceId);
        isTestPass = isTestPass && result;
    }

    printf("edma test finished\n");
    if (isTestPass == true)
    {
        printf("All Tests PASSED\n");
    }
    else
    {
        printf("All Tests did NOT Pass\n");
    }

    MCPI_setTestResult();

    /* exit BIOS */
    BIOS_exit(0);
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
void main (void)
{
    Task_Params taskParams;

#if (defined(SOC_XWR14XX) || (defined(SOC_XWR16XX) && defined(SUBSYS_MSS)) || \
    (defined(SOC_XWR18XX) && defined(SUBSYS_MSS)) || \
    (defined(SOC_XWR68XX) && defined(SUBSYS_MSS)))
    SOC_Handle  socHandle;
    int32_t     errCode;
    SOC_Cfg     socCfg;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return;
    }

#endif

#ifdef SUBSYS_DSS

/* map event combiners to vectors 4-7 */
{
    Hwi_Params params;
    uint32_t i;

    Hwi_Params_init(&params);
    params.enableInt = TRUE;
    for (i = 0; i < 4; i++)
    {
        params.arg = i;
        params.eventId = i;
        if (Hwi_create(4 + i, &EventCombiner_dispatch, &params, NULL) == NULL)
        {
            System_printf("failed to create Hwi interrupt %d\n",4 + i);
        }
    }
}
#endif

    Cycleprofiler_init();

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_task, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return;
}

