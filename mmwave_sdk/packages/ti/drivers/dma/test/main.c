/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the DMA
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

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/dma/dma.h>
#include "ti/utils/testlogger/logger.h"
#include "ti/utils/cycleprofiler/cycle_profiler.h"


/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/
#define DMA_TEST_ERROR                  (MMWAVE_ERRNO_DMA_BASE-100)

#define DMA_TEST_ALIGN_ADDRESS(addr,elemSz)    ((((addr-1)>>elemSz)+1)<<elemSz)

#define FEATURE_STRING_LEN 200


#define START_TEST_PRINT(featureString,instanceId, testNum) \
    {\
        System_printf("********************************************************\n");\
        System_printf("Start Test %d (DMA%d): %s \n",testNum, instanceId,featureString);\
        System_printf("********************************************************\n");\
    }

/**************************************************************************
 *************************** Global Definitions ****************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* assume global source and destination */
#pragma DATA_SECTION(gDMATestSource, ".l3Data");
uint8_t  gDMATestSource[8192];
#pragma DATA_SECTION(gDMATestDest, ".l3Data");
uint8_t  gDMATestDest[8192];
uint32_t gDMAInrCnt=0;
uint32_t gPrintStats = 0;
bool    gInQuietMode = 0; // to disable regular prints during benchmarking


#define BENCHMARKING_NUM_BUFFER_SIZES 14
#define BENCHMARKING_NUM_ELEM_SIZE 4
uint16_t benchmark_buffer_sizes[BENCHMARKING_NUM_BUFFER_SIZES] = {1,2,4, 8, 16, 32,64,128, 256, 512, 1024, 2048, 4096, 8192};
uint8_t  benchmark_bufferElem_sizes[BENCHMARKING_NUM_ELEM_SIZE] = {1,2,4,8};
uint32_t benchmark_data[BENCHMARKING_NUM_BUFFER_SIZES][BENCHMARKING_NUM_ELEM_SIZE];


SemaphoreP_Handle       gErrorSemaphore;
uint32_t                gErrorChannelNum;



/**************************************************************************
 *************************** DMA Test Structures **************************
 **************************************************************************/

typedef struct DMA_TestParams_t {
    uint32_t            channelNum;
    DMA_CtrlPktParams   ctrlPacket;
    DMA_IntType         intType;
    bool                isChainedChannel;
    bool                doPolling;
    
    /* internal use */
    SemaphoreP_Handle       xferSem;
    DMA_ChannelStatus       pChanStatus;
    uint32_t                pendXferCnt;
    uint32_t                chainedXferCnt; /* used if isChainedChannel == true */
    uint32_t                configTicks;
    uint32_t                triggerStartTicks;
    uint32_t                dmaCopyDoneTicks;
    uint32_t                dmaIntEntryTicks;
    uint32_t                pendStartTicks;
} DMA_TestParams;

/**************************************************************************
 *************************** DMA Test Function Prototypes *****************
 **************************************************************************/

void Test_indicateResultToAutomation(char *st, int32_t errCode);

/**************************************************************************
 *************************** DMA Test Functions **************************
 **************************************************************************/
void DMA_TestErrorInterruptCb(uint32_t channel, DMA_IntType inttype, void * arg)
{
    gErrorChannelNum = channel;
    if (gErrorSemaphore != NULL)
    {
        SemaphoreP_post(gErrorSemaphore);
    }
    else
    { /* unexpected BER interrupt */
        DebugP_assert(true);
    }
}


void DMA_TestInterruptCb(uint32_t channel, DMA_IntType inttype, void * arg)
{
    DMA_TestParams *dmaChTestParams = (DMA_TestParams *)arg;
    dmaChTestParams->dmaIntEntryTicks = Cycleprofiler_getTimeStamp();
    gDMAInrCnt++;

    if (dmaChTestParams!=NULL)
    {
        if (dmaChTestParams->pendXferCnt>0)
        {
            SemaphoreP_post(dmaChTestParams->xferSem);
        }
        else
        {
            DebugP_assert(true);
        }
    }
}

void DMA_TestPrintChStatus(uint32_t channel, DMA_ChannelStatus *pChanStatus)
{
    /* channel status */
    System_printf("channel status \n");
    System_printf("channel %d: chPend: 0x%x \n",channel, pChanStatus->chPend);
    System_printf("channel %d: chActive: 0x%x \n",channel, pChanStatus->chActive);
    /* channel trigger status */
    System_printf("channel trigger status \n");
    System_printf("channel %d: chHwEnable: 0x%x \n",channel, pChanStatus->chHwEnable);
    System_printf("channel %d: chSwEnable: 0x%x \n",channel, pChanStatus->chSwEnable);
    /* channel assignment status */
    System_printf("channel assignment status \n");
    System_printf("channel %d: chIsHighPri: 0x%x \n",channel, pChanStatus->chIsHighPri);
    System_printf("channel %d: chReqAssign: 0x%x \n",channel, pChanStatus->chReqAssign);
    System_printf("channel %d: chPortAssign: 0x%x \n",channel, pChanStatus->chPortAssign);
    /* channel Interrupt Enable status */
    System_printf("channel Interrupt Enable status \n");
    System_printf("channel %d: chIsIntEn: 0x%x \n",channel, pChanStatus->chIsIntEn);
    System_printf("channel %d: chIsFTCIntEn: 0x%x \n",channel, pChanStatus->chIsFTCIntEn);
    System_printf("channel %d: chIsLFSIntEn: 0x%x \n",channel, pChanStatus->chIsLFSIntEn);
    System_printf("channel %d: chIsHBCIntEn: 0x%x \n",channel, pChanStatus->chIsHBCIntEn);
    System_printf("channel %d: chIsBTCIntEn: 0x%x \n",channel, pChanStatus->chIsBTCIntEn);
    /* channel Interrupt Pending status */
    System_printf("channel Interrupt Pending status \n");
    System_printf("channel %d: chIsIntPend: 0x%x \n",channel, pChanStatus->chIsIntPend);
    System_printf("channel %d: chIsFTCIntPend: 0x%x \n",channel, pChanStatus->chIsFTCIntPend);
    System_printf("channel %d: chIsLFSIntPend: 0x%x \n",channel, pChanStatus->chIsLFSIntPend);
    System_printf("channel %d: chIsHBCIntPend: 0x%x \n",channel, pChanStatus->chIsHBCIntPend);
    System_printf("channel %d: chIsBTCIntPend: 0x%x \n",channel, pChanStatus->chIsBTCIntPend);
    /* channel initial value in control packet ram */
    System_printf("channel initial value in control packet ram \n");
    System_printf("channel %d: chInitSrcAddr: 0x%x \n",channel, pChanStatus->chInitSrcAddr);
    System_printf("channel %d: chInitDstAddr: 0x%x \n",channel, pChanStatus->chInitDstAddr);
    System_printf("channel %d: chInitElemCnt: 0x%x \n",channel, pChanStatus->chInitElemCnt);
    System_printf("channel %d: chInitFrameCnt: 0x%x \n",channel, pChanStatus->chInitFrameCnt);
    System_printf("channel %d: chControlReg: 0x%x \n",channel, pChanStatus->chControlReg);
    System_printf("channel %d: chSrcElemIndex: 0x%x \n",channel, pChanStatus->chSrcElemIndex);
    System_printf("channel %d: chDstElemIndex: 0x%x \n",channel, pChanStatus->chDstElemIndex);
    System_printf("channel %d: chSrcFrameIndex: 0x%x \n",channel, pChanStatus->chSrcFrameIndex);
    System_printf("channel %d: chDstFrameIndex: 0x%x \n",channel, pChanStatus->chDstFrameIndex);
    /* channel working value in control packet ram */
    System_printf("channel working value in control packet ram \n");
    System_printf("channel %d: chCurrSrcAddr: 0x%x \n",channel, pChanStatus->chCurrSrcAddr);
    System_printf("channel %d: chCurrDstAddr: 0x%x \n",channel, pChanStatus->chCurrDstAddr);
    System_printf("channel %d: chCurrElemCnt: 0x%x \n",channel, pChanStatus->chCurrElemCnt);
    System_printf("channel %d: chCurrFrameCnt: 0x%x \n",channel, pChanStatus->chCurrFrameCnt);
}


static uint32_t DMA_Test_CheckMemCopyPerFrame(DMA_TestParams *dmaChTestParams, uint32_t frameCnt)
{
    /* test the destination memory */
    uint8_t * srcAddr = (uint8_t *)dmaChTestParams->ctrlPacket.srcAddr;
    uint8_t * destAddr = (uint8_t *)dmaChTestParams->ctrlPacket.destAddr;
    uint32_t  srcElemSize = (1<<dmaChTestParams->ctrlPacket.srcElemSize);
    uint32_t  destElemSize = (1<<dmaChTestParams->ctrlPacket.destElemSize);

    uint32_t  elemCnt = 0;
    uint32_t  cmpVal=0;

    elemCnt = dmaChTestParams->ctrlPacket.elemXferCnt;

    /* change destAddr for the frame */
    if (dmaChTestParams->ctrlPacket.destAddrMode == DMA_AddrMode_PostIncrement)
    {
        destAddr=(uint8_t *)dmaChTestParams->ctrlPacket.destAddr+frameCnt*(elemCnt*destElemSize);
    }
    else if (dmaChTestParams->ctrlPacket.destAddrMode == DMA_AddrMode_Indexed)
    {
        destAddr=(uint8_t *)dmaChTestParams->ctrlPacket.destAddr+frameCnt*dmaChTestParams->ctrlPacket.destFrameIndexOffset;
    }
    else
    {
        /* no increments in constant mode */
    }
    /* change srcAddr for the frame */
    if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_PostIncrement)
    {
        srcAddr=(uint8_t *)dmaChTestParams->ctrlPacket.srcAddr+frameCnt*(elemCnt*srcElemSize);
    }
    else if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_Indexed)
    {
        srcAddr=(uint8_t *)dmaChTestParams->ctrlPacket.srcAddr+frameCnt*dmaChTestParams->ctrlPacket.srcFrameIndexOffset;
    }
    else
    {
        /* no increments in constant mode */
    }

    for (;elemCnt>0;elemCnt--)
    {
        cmpVal+=memcmp(srcAddr,destAddr,srcElemSize);
        
        if (cmpVal!= 0)
        {
            System_printf("Error: DMA_Test_CheckMemCopy found mismatch at srcAddr=0x%x and destAddr=0x%x for elemSize=%d\n",srcAddr,destAddr,srcElemSize);
        }
        
        if (dmaChTestParams->ctrlPacket.destAddrMode == DMA_AddrMode_PostIncrement)
        {
            destAddr+=destElemSize;
        }
        else if (dmaChTestParams->ctrlPacket.destAddrMode == DMA_AddrMode_Indexed)
        {
            destAddr+=dmaChTestParams->ctrlPacket.destElemIndexOffset;
        }
        else
        {
            /* no increments in constant mode */
        }
        if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_PostIncrement)
        {
            srcAddr+=srcElemSize;
        }
        else if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_Indexed)
        {
            srcAddr+=dmaChTestParams->ctrlPacket.srcElemIndexOffset;
        }
        else
        {
            /* no increments in constant mode */
        }
    }

    return cmpVal;

    
}

static uint32_t DMA_Test_CheckMemCopy(DMA_TestParams *dmaChTestParams)
{
    /* test the destination memory */
    uint32_t  frameCnt = 0;
    uint32_t  cmpVal=0;

    for (frameCnt=0;frameCnt<dmaChTestParams->ctrlPacket.frameXferCnt;frameCnt++)
    {
        cmpVal+=DMA_Test_CheckMemCopyPerFrame(dmaChTestParams,frameCnt);
    }
    return cmpVal;
}


static uint32_t DMA_Test_FillPatternPerFrame(DMA_TestParams *dmaChTestParams, uint32_t frameCnt)
{
    /* test the destination memory */
    uint8_t * srcAddr = (uint8_t *)dmaChTestParams->ctrlPacket.srcAddr;
    uint32_t  srcElemSize = (1<<dmaChTestParams->ctrlPacket.srcElemSize);

    uint32_t  elemCnt = 0;

    elemCnt = dmaChTestParams->ctrlPacket.elemXferCnt;

    /* do increments for the frame */
    if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_PostIncrement)
    {
        srcAddr=(uint8_t *)dmaChTestParams->ctrlPacket.srcAddr+frameCnt*(srcElemSize*elemCnt);
    }
    else if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_Indexed)
    {
        srcAddr=(uint8_t *)dmaChTestParams->ctrlPacket.srcAddr+frameCnt*dmaChTestParams->ctrlPacket.srcFrameIndexOffset;
    }
    else
    {
        /* no increments in constant mode */
    }

    for (;elemCnt>0;elemCnt--)
    {
        uint32_t i=0;
        for (i=0;i<srcElemSize;i++) {
            srcAddr[i]=(dmaChTestParams->ctrlPacket.elemXferCnt*srcElemSize*frameCnt)+(srcElemSize*elemCnt)+i;
        }
        if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_PostIncrement)
        {
            srcAddr+=srcElemSize;
        }
        else if (dmaChTestParams->ctrlPacket.srcAddrMode == DMA_AddrMode_Indexed)
        {
            srcAddr+=dmaChTestParams->ctrlPacket.srcElemIndexOffset;
        }
        else
        {
            /* no increments in constant mode */
        }
    }

    return 0;
}


static uint32_t DMA_Test_FillPattern(DMA_TestParams *dmaChTestParams)
{
    uint32_t  frameCnt = 0;
    for (frameCnt=0;frameCnt<dmaChTestParams->ctrlPacket.frameXferCnt;frameCnt++)
    {
        DMA_Test_FillPatternPerFrame(dmaChTestParams,frameCnt);
    } 
    return 0;
}

static int32_t DMA_Test_config_channel_transfer(DMA_Handle handle, DMA_TestParams *dmaChTestParams)
{
    int32_t            retCode = 0;
    SemaphoreP_Params   semParams;
    
    uint32_t           start_ticks;

    /* set the channel paramters */
    start_ticks = Cycleprofiler_getTimeStamp();
    retCode = DMA_setChannelParams(handle, dmaChTestParams->channelNum,&dmaChTestParams->ctrlPacket);
    dmaChTestParams->configTicks = Cycleprofiler_getTimeStamp() - start_ticks;
    if (retCode) System_printf("Error: DMA_setChannelParams (%d) returned %d\n",dmaChTestParams->channelNum,retCode);

    if (retCode == 0)
    {
        if (dmaChTestParams->doPolling != true) {
            /* register the interrupt */
            SemaphoreP_Params_init(&semParams);
            semParams.mode             = SemaphoreP_Mode_BINARY;
            dmaChTestParams->xferSem = SemaphoreP_create(0, &semParams);
            retCode = DMA_enableInterrupt(handle,dmaChTestParams->channelNum,dmaChTestParams->intType,DMA_TestInterruptCb,(void *)dmaChTestParams);
            if (retCode) System_printf("Error: DMA_enableInterrupt(%d) returned %d\n",dmaChTestParams->channelNum,retCode);
        } else {
            dmaChTestParams->xferSem = NULL;
            retCode = DMA_disableInterrupt(handle,dmaChTestParams->channelNum,dmaChTestParams->intType);
            if (retCode) System_printf("Error: DMA_disableInterrupt(%d) returned %d\n",dmaChTestParams->channelNum,retCode);
            retCode = DMA_clearChannelInterruptFlag(handle,dmaChTestParams->channelNum,dmaChTestParams->intType);
            if (retCode) System_printf("Error: DMA_clearChannelInterruptFlag(%d) returned %d\n",dmaChTestParams->channelNum,retCode);
        }
    }
    if ((retCode == 0) && (dmaChTestParams->isChainedChannel == true))
    {
        dmaChTestParams->pendXferCnt++;
        dmaChTestParams->triggerStartTicks = Cycleprofiler_getTimeStamp();
        retCode = DMA_enableChannel(handle,dmaChTestParams->channelNum, DMA_ChTriggerType_HW);
        if (retCode) System_printf("Error: DMA_enableChannel(HW) for chained channel %d returned %d\n",
                                        dmaChTestParams->channelNum,retCode);
        /* decrement the count if channel was not enabled */
        if (retCode != 0)
        {
            dmaChTestParams->pendXferCnt--;
        }
    }
    if (retCode == 0)
    {
        /* set the priority (optional) */
        /* TBA */
    }
    return retCode;
}

static int32_t DMA_Test_unconfig_channel_transfer(DMA_Handle handle, DMA_TestParams *dmaChTestParams)
{
    int32_t            retCode = 0;
    retCode = DMA_disableInterrupt(handle,dmaChTestParams->channelNum,dmaChTestParams->intType);
    if (retCode) System_printf("Error: DMA_disableInterrupt(%d) returned %d\n",
                                        dmaChTestParams->channelNum,retCode);
    if (dmaChTestParams->xferSem != NULL)
        SemaphoreP_delete(dmaChTestParams->xferSem);
    return retCode;
}

static int32_t DMA_Test_trigger_and_check_channel_transfer(DMA_Handle handle, DMA_TestParams *dmaChTestParams)
{
    int32_t            retCode = 0;
    uint32_t           loopCnt = 0;
    uint32_t           maxLoopCnt = 0;

    /* Since this is a software based trigger, 
       we need to trigger multiple times if transfer type is frame; 
       for block tranfer, only one trigger is sufficient.
       For H/W based trigger, this is automatically done in hardware and no S/W intervention is needed
     */
    if (dmaChTestParams->ctrlPacket.xferType == DMA_XferType_Frame)
    {
        maxLoopCnt = dmaChTestParams->ctrlPacket.frameXferCnt;
    }
    else
    {
        /* block based transfer - fill all the frames */
        maxLoopCnt = 1;
    }
    for (loopCnt=0;loopCnt<maxLoopCnt;loopCnt++)
    {
        /* every loop: prepare and then enable channel; Chained channels 
                       have to be set for HW triggered */
        if (!gInQuietMode) System_printf("Debug: Trigger channel ch:%d for loop#%d\n",dmaChTestParams->channelNum,loopCnt);
        
        if (dmaChTestParams->isChainedChannel == false)
        {
            /* update the source buffer for primary channel ony*/
            if (dmaChTestParams->ctrlPacket.xferType == DMA_XferType_Frame)
            {
                DMA_Test_FillPatternPerFrame(dmaChTestParams,loopCnt);
            }
            else
            {
                DMA_Test_FillPattern(dmaChTestParams);
            }
            
            /* enable the primary channel */
            dmaChTestParams->pendXferCnt++;
            dmaChTestParams->triggerStartTicks = Cycleprofiler_getTimeStamp();
            retCode = DMA_enableChannel(handle,dmaChTestParams->channelNum,DMA_ChTriggerType_SW);
            if (retCode) System_printf("Error: DMA_enableChannel for channel %d returned %d\n",
                                            dmaChTestParams->channelNum,retCode);
            /* decrement the count if channel was not enabled */
            if (retCode != 0)
            {
                dmaChTestParams->pendXferCnt--;
            }
        }
        /* wait for xfer complete */
        if (retCode == 0)
        {
            dmaChTestParams->pendStartTicks = Cycleprofiler_getTimeStamp();
            if (dmaChTestParams->doPolling != true) {
                /* wait for interrupt */
                SemaphoreP_Status status = SemaphoreP_OK;
                status = SemaphoreP_pend(dmaChTestParams->xferSem,BIOS_WAIT_FOREVER);
                dmaChTestParams->dmaCopyDoneTicks = Cycleprofiler_getTimeStamp();
                if (status != SemaphoreP_OK)
                {
                    retCode = DMA_TEST_ERROR;
                } else {
                    dmaChTestParams->pendXferCnt--;
                }
                
                if (retCode) System_printf("Error: SemaphoreP_pend returned %d\n",status);
            } else {
                /* do polling */
                volatile uint32_t pollFlag;
                do {
                    pollFlag = DMA_getChannelInterruptFlag(handle,dmaChTestParams->channelNum,dmaChTestParams->intType,&retCode);
                } while ((pollFlag == 0)&&(retCode==0));
                /* get benchmarking data */
                dmaChTestParams->dmaCopyDoneTicks = Cycleprofiler_getTimeStamp();
                /* clear flag for next polling/trigger to work */
                retCode = DMA_clearChannelInterruptFlag(handle,dmaChTestParams->channelNum,dmaChTestParams->intType);
                if (retCode) System_printf("Error: DMA_clearChannelInterruptFlag(%d) returned %d\n",dmaChTestParams->channelNum,retCode);
            }
        }
        /* get the status */
        if (retCode == 0)
        {
            retCode = DMA_getChannelStatus(handle,dmaChTestParams->channelNum,&dmaChTestParams->pChanStatus);
            if (retCode) System_printf("Error: DMA_getChannelStatus returned %d\n",retCode);
            if (retCode == 0)
            {
                if(gPrintStats) 
                {
                    DMA_TestPrintChStatus(dmaChTestParams->channelNum,&dmaChTestParams->pChanStatus);
                }
            }
        }
        /* check if memory copy completed successfully */
        if (dmaChTestParams->ctrlPacket.xferType == DMA_XferType_Frame)
        {
            /* check if memory copy completed successfully */
            if (DMA_Test_CheckMemCopyPerFrame(dmaChTestParams,loopCnt)>0)
            {
                if (retCode) System_printf("Error: DMA_Test_CheckMemCopyPerFrame failed for frame: %d\n",loopCnt);
                retCode = DMA_TEST_ERROR;
            }
        }
        else
        {
            /* check if memory copy completed successfully */
            if (DMA_Test_CheckMemCopy(dmaChTestParams)>0)
            {
                if (retCode) System_printf("Error: DMA_Test_CheckMemCopy failed\n");
                retCode = DMA_TEST_ERROR;
            }
        }
    }
    return retCode;
}

static int32_t DMA_Test_DoMemCopy(DMA_Handle handle, DMA_TestParams *dmaChTestParams)
{
    int32_t            retCode = 0;

    /* no need for parameter validation here except for dmaChTestParams pointer 
       DMA driver should throw an error on invalid params 
     */
    if (dmaChTestParams==NULL)
    {
        System_printf("Error: Invalid params passed to DMA_TestMemCopy\n");
        retCode = DMA_TEST_ERROR;
    } 
    else 
    {
        /* first config */
        retCode = DMA_Test_config_channel_transfer(handle,dmaChTestParams);

        /* next trigger and test if successful */
        if (retCode == 0) 
        {
            retCode = DMA_Test_trigger_and_check_channel_transfer(handle,dmaChTestParams);
        }

        /* cleanup before return*/
        if (retCode == 0) 
        {
            retCode = DMA_Test_unconfig_channel_transfer(handle,dmaChTestParams);
        }
    }
    return retCode;
}


static int32_t DMA_Test_DoChainedTransfer(DMA_Handle handle)
{
#define DMA_TEST_NUM_CHAINS 2
    DMA_TestParams dmaChainedChTestParams[DMA_TEST_NUM_CHAINS+1];
    uint8_t chainedChannel = 0;
    int8_t i = 0;
    int32_t retCode = 0;
    /* initialize */
    memset(&dmaChainedChTestParams,0,sizeof(dmaChainedChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */    
    dmaChainedChTestParams[chainedChannel].channelNum = 13; /* select any channel */
    dmaChainedChTestParams[chainedChannel].intType = DMA_IntType_BTC;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.frameXferCnt=2;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.elemXferCnt=4;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.nextChannel=14;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.autoInitiation=0;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.xferType=DMA_XferType_Block;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
    dmaChainedChTestParams[chainedChannel].isChainedChannel = false;
    

    chainedChannel++;
    dmaChainedChTestParams[chainedChannel].channelNum = 14; /* select any channel */
    dmaChainedChTestParams[chainedChannel].intType = DMA_IntType_BTC;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcAddr=(uint32_t)gDMATestDest;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destAddr=(uint32_t)gDMATestSource + 0x100;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.frameXferCnt=4;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.elemXferCnt=2;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.nextChannel=15;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.autoInitiation=0;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.xferType=DMA_XferType_Block;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destAddrMode=DMA_AddrMode_Indexed;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destElemIndexOffset=4;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destFrameIndexOffset=1;
    dmaChainedChTestParams[chainedChannel].isChainedChannel = true;

    chainedChannel++;
    dmaChainedChTestParams[chainedChannel].channelNum = 15; /* select any channel */
    dmaChainedChTestParams[chainedChannel].intType = DMA_IntType_BTC;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcAddr=(uint32_t)gDMATestSource+0x100;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destAddr=(uint32_t)gDMATestDest+ 0x100;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.frameXferCnt=4;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.elemXferCnt=2;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.autoInitiation=0;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.xferType=DMA_XferType_Block;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcAddrMode=DMA_AddrMode_Indexed;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destAddrMode=DMA_AddrMode_Indexed;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcElemIndexOffset=4; 
    dmaChainedChTestParams[chainedChannel].ctrlPacket.srcFrameIndexOffset=1;
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destElemIndexOffset=4; 
    dmaChainedChTestParams[chainedChannel].ctrlPacket.destFrameIndexOffset=8;
    dmaChainedChTestParams[chainedChannel].isChainedChannel = true;

    /* safe to configure chained channels first and then the primary channel */
    for (i=chainedChannel;i>=0;i--)
    {
        /* set the channel paramters */
        if (retCode == 0)
        {
            retCode = DMA_Test_config_channel_transfer(handle,&dmaChainedChTestParams[i]);
        }
    }

    /* now trigger the primary channel - assume that the first index in dmaChainedChTestParams 
       is primary channel */
    if (retCode == 0)
    {
        retCode = DMA_Test_trigger_and_check_channel_transfer(handle,&dmaChainedChTestParams[0]);
    }

    /* now test the success of chained transfers */
    for (i=1;i<=chainedChannel;i++)
    {
        if (retCode == 0)
        {
            retCode = DMA_Test_trigger_and_check_channel_transfer(handle,&dmaChainedChTestParams[i]);
        }
    }

    /* now unconfig the channels */
    for (i=0;i<=chainedChannel;i++)
    {
        DMA_Test_unconfig_channel_transfer(handle,&dmaChainedChTestParams[i]);
    }

    return retCode;
}

static void DMA_Test_PerformBusErrorCheckTests
                (DMA_Handle handle, 
                 char * featureString)
{
    
    DMA_TestParams dmaChTestParams;
    DMA_TestParams dmaChErrorTestParams;
    int32_t retCode, errCode = 0;
    volatile uint32_t errIntFlag = 0;
    SemaphoreP_Status status = SemaphoreP_OK;
    SemaphoreP_Params   semParams;
    
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(DMA_TestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 16; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.isChainedChannel = false;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=10;
    dmaChTestParams.ctrlPacket.elemXferCnt=4;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;


    
    /* BER tests */
    strncpy(featureString, "Check BER interrupt ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    /* enable interrupt */
    DMA_enableInterrupt(handle,dmaChErrorTestParams.channelNum,DMA_IntType_BER,DMA_TestErrorInterruptCb,NULL);
    /* create semaphore for proper handling */
    SemaphoreP_Params_init(&semParams);
    semParams.mode             = SemaphoreP_Mode_BINARY;
    gErrorSemaphore = SemaphoreP_create(0, &semParams);
    /* introduce error */
    dmaChErrorTestParams.ctrlPacket.srcAddr=0x10000000;
    DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    DMA_enableChannel(handle,dmaChErrorTestParams.channelNum,DMA_ChTriggerType_SW);
    /* wait for error interrupt */
    status = SemaphoreP_pend(gErrorSemaphore,BIOS_WAIT_FOREVER);
    if ((status != SemaphoreP_OK) || (gErrorChannelNum!=dmaChErrorTestParams.channelNum))
    {
        errCode = -1;
    } else {
        errCode = 0;
    }
    /* cleanup */
    DMA_disableInterrupt(handle,dmaChErrorTestParams.channelNum,DMA_IntType_BER);
    if (gErrorSemaphore != NULL)
        SemaphoreP_delete(gErrorSemaphore);
    /* set results */
    Test_indicateResultToAutomation(featureString,errCode);


    /* BER tests */
    strncpy(featureString, "Check BER polling flag ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    /* clear polling flag to begin with */
    DMA_clearChannelInterruptFlag(handle,dmaChErrorTestParams.channelNum,DMA_IntType_BER);
    /* introduce error */
    dmaChErrorTestParams.ctrlPacket.srcAddr=0x10000000;
    DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    DMA_enableChannel(handle,dmaChErrorTestParams.channelNum,DMA_ChTriggerType_SW);
    /* poll for BER interrupt flag */
    do {
        errIntFlag = DMA_getChannelInterruptFlag(handle,dmaChErrorTestParams.channelNum,DMA_IntType_BER,&retCode);
    } while(errIntFlag == 0);
    if (errIntFlag != 0)
        errCode = 0;
    else 
        errCode = -1;
    /* clear flag on exit */
    DMA_clearChannelInterruptFlag(handle,dmaChErrorTestParams.channelNum,DMA_IntType_BER);
    /* set results */
    Test_indicateResultToAutomation(featureString,errCode);

}


static void DMA_Test_PerformErrorCheckTests
                (DMA_Handle handle, 
                 char * featureString)
{
    DMA_TestParams dmaChTestParams;
    DMA_TestParams dmaChErrorTestParams;
    int32_t retCode, errCode = 0;

    /* initialize */
    memset(&dmaChTestParams,0,sizeof(DMA_TestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 16; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.isChainedChannel = false;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=10;
    dmaChTestParams.ctrlPacket.elemXferCnt=4;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;

    
    /****************************/
    /* error tests              */
    /****************************/
    /* invalid transfer size */
    strncpy(featureString, "Check error code for invalid element xfer cnt ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.elemXferCnt = 0x2000;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* invalid transfer size */
    strncpy(featureString, "Check error code for invalid frame xfer cnt ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.frameXferCnt = 0x2000;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* invalid chained channel */
    strncpy(featureString, "Check error code for invalid chained channel ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.nextChannel = 32;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* invalid channel */
    strncpy(featureString, "Check error code for invalid channel number ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.elemXferCnt = 0x2000;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* invalid src element index offset */
    strncpy(featureString, "Check error code for invalid src element index offset ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.srcAddrMode = DMA_AddrMode_Indexed;
    dmaChErrorTestParams.ctrlPacket.srcElemIndexOffset = 0x2000;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* invalid src frame index offset */
    strncpy(featureString, "Check error code for invalid src frame index offset ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.srcAddrMode = DMA_AddrMode_Indexed;
    dmaChErrorTestParams.ctrlPacket.srcFrameIndexOffset = 0x2000;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* invalid dest element index offset */
    strncpy(featureString, "Check error code for invalid dest element index offset ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.destAddrMode = DMA_AddrMode_Indexed;
    dmaChErrorTestParams.ctrlPacket.destElemIndexOffset = 0x2000;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* invalid dest frame index offset */
    strncpy(featureString, "Check error code for invalid dest frame index offset ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.destAddrMode = DMA_AddrMode_Indexed;
    dmaChErrorTestParams.ctrlPacket.destFrameIndexOffset = 0x2000;
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* unaligned src addr */
    strncpy(featureString, "Check error code for unaligned src addr ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.srcElemSize = DMA_ElemSize_16bit;
    dmaChErrorTestParams.ctrlPacket.srcAddr = (uint32_t)&gDMATestSource[1];
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);

    /* unaligned dest addr */
    strncpy(featureString, "Check error code for unaligned dest addr ", FEATURE_STRING_LEN);
    memcpy(&dmaChErrorTestParams,&dmaChTestParams,sizeof(DMA_TestParams));
    dmaChErrorTestParams.ctrlPacket.destElemSize = DMA_ElemSize_32bit;
    dmaChErrorTestParams.ctrlPacket.destAddr = (uint32_t)&gDMATestSource[2];
    retCode = DMA_setChannelParams(handle, dmaChErrorTestParams.channelNum,&dmaChErrorTestParams.ctrlPacket);
    if (retCode!= 0) {
        errCode = 0;
    } else {
        errCode = -1;
    }
    Test_indicateResultToAutomation(featureString,errCode);



    /****************************/
    /* end - error tests        */
    /****************************/
    return;
     
}

static int32_t DMA_Test_PerformBenchmarking(DMA_Handle handle, bool doPolling)
{
    uint8_t elemCntIdx = 0;
    DMA_ElemSize elemSize;
    int32_t        errCode;
    DMA_TestParams dmaChTestParams;

    System_printf("BufferSize,8-bit xfer,16-bit xfer,32-bit xfer,64-bit xfer\n");
    for (elemCntIdx = 0; elemCntIdx<(sizeof(benchmark_buffer_sizes)/sizeof(uint16_t)); elemCntIdx++)
    {
        System_printf("%d,",benchmark_buffer_sizes[elemCntIdx]); //no new line at the end
        for (elemSize = DMA_ElemSize_8bit; elemSize<= DMA_ElemSize_64bit; elemSize++)
        {
            if (benchmark_buffer_sizes[elemCntIdx]/benchmark_bufferElem_sizes[elemSize]==0) {
                System_printf(","); //no new line at the end
                continue;
            }
            if (benchmark_buffer_sizes[elemCntIdx]/benchmark_bufferElem_sizes[elemSize]>=8192) {
                System_printf(","); //no new line at the end
                continue;
            }
            /* initialize */
            memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
            memset(gDMATestSource,'S',sizeof(gDMATestSource));
            memset(gDMATestDest,'D',sizeof(gDMATestDest));
            /* set the params */
            dmaChTestParams.channelNum = 1; /* select any channel */
            dmaChTestParams.doPolling = doPolling;
            dmaChTestParams.intType = DMA_IntType_FTC;
            dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
            dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
            dmaChTestParams.ctrlPacket.frameXferCnt=1;
            dmaChTestParams.ctrlPacket.elemXferCnt=benchmark_buffer_sizes[elemCntIdx]/benchmark_bufferElem_sizes[elemSize];
            dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
            dmaChTestParams.ctrlPacket.srcElemSize=elemSize;
            dmaChTestParams.ctrlPacket.destElemSize=elemSize;
            dmaChTestParams.ctrlPacket.autoInitiation=0;
            dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
            dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
            dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
            /* do the memory transfer */
            errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
            if (errCode != 0)
            {
                System_printf("\nBenchmarking %d bytes using %d bits transfer failed! \n", 
                            benchmark_buffer_sizes[elemCntIdx], 
                            benchmark_bufferElem_sizes[elemSize]*8);
                return errCode;
            } else {
                if (doPolling){
                    benchmark_data[elemCntIdx][elemSize] = (dmaChTestParams.dmaCopyDoneTicks - dmaChTestParams.pendStartTicks);
                }
                else
                {
                    if (dmaChTestParams.pendStartTicks <=dmaChTestParams.dmaIntEntryTicks)
                        benchmark_data[elemCntIdx][elemSize] = (dmaChTestParams.dmaIntEntryTicks - dmaChTestParams.pendStartTicks);
                    else
                        benchmark_data[elemCntIdx][elemSize] = (dmaChTestParams.dmaIntEntryTicks - dmaChTestParams.triggerStartTicks);
                }
                System_printf("%d,",benchmark_data[elemCntIdx][elemSize]); //no new line at the end
            }
        }
        System_printf("\n");
    }
    return errCode;
}

#if 0
static void DMA_Test_PerformBenchmarking_CPUbased()
{
    uint8_t elemCntIdx = 0;
    uint32_t start_ticks = 0, time_for_memcpy = 0;
    DMA_ElemSize elemSize;

    System_printf("BufferSize,8-bit xfer,16-bit xfer,32-bit xfer,64-bit xfer\n");
    for (elemCntIdx = 0; elemCntIdx<(sizeof(benchmark_buffer_sizes)/sizeof(uint16_t)); elemCntIdx++)
    {
        System_printf("%d,",benchmark_buffer_sizes[elemCntIdx]); //no new line at the end
        for (elemSize = DMA_ElemSize_8bit; elemSize<= DMA_ElemSize_64bit; elemSize++)
        {
            memset(gDMATestSource,'S',sizeof(gDMATestSource));
            memset(gDMATestDest,'D',sizeof(gDMATestDest));
            start_ticks = Cycleprofiler_getTimeStamp();
            memcpy(gDMATestDest+benchmark_bufferElem_sizes[elemSize],gDMATestSource+benchmark_bufferElem_sizes[elemSize],benchmark_buffer_sizes[elemCntIdx]);
            time_for_memcpy = Cycleprofiler_getTimeStamp() - start_ticks;
            System_printf("%d,",time_for_memcpy); //no new line at the end
        }
        System_printf("\n");
    }
}
#endif

void Test_indicateResultToAutomation(char *st, int32_t errCode)
{
    if (errCode == 0)
    {
        MCPI_setFeatureTestResult(st, MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult(st, MCPI_TestResult_FAIL);
    }
}

static void Test_Instance(uint32_t instanceId)
{
    
    DMA_Params     params;
    DMA_Handle     handle;
    int32_t        errCode;
    DMA_TestParams dmaChTestParams;
    uint32_t       testNum = 0;
    char featureString[FEATURE_STRING_LEN];

    /* Setup the default DMA Parameters */
    DMA_Params_init(&params);
    /* Open the DMA Instance */
    handle = DMA_open(instanceId, &params, &errCode);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the DMA Instance %d  err:%d\n",instanceId,errCode);
        MCPI_setFeatureTestResult("DMA_open", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult("DMA_open", MCPI_TestResult_PASS);
    System_printf("Debug: DMA Instance (%d) has been opened successfully (handle: %p)\n",instanceId, handle);

    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    errCode = DMA_close(handle);
    System_printf("Debug: DMA Instance %p has been closed err:%d\n", handle,errCode);
    if (errCode == 0)
        MCPI_setFeatureTestResult("DMA_close", MCPI_TestResult_PASS);
    else
        MCPI_setFeatureTestResult("DMA_close", MCPI_TestResult_FAIL);

    /**************************************************************************
     * Test: Reopen the driver
     **************************************************************************/
    handle = DMA_open(instanceId, &params, &errCode);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the DMA Instance err:%d\n",errCode);
        MCPI_setFeatureTestResult("DMA_open", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult("DMA_open", MCPI_TestResult_PASS);
    System_printf("Debug: DMA Instance %p has been reopened successfully\n", handle);

    /* do some tests here */
    /****************************/
    /* Start mem - mem copy     */
    /* S/W triggered            */
    /****************************/
    /* TEST: frame trigger transfer */
    testNum++;
    strncpy(featureString, "Frame trigger transfer", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 1; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=10;
    dmaChTestParams.ctrlPacket.elemXferCnt=4;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* TEST: block trigger transfer */
    testNum++;
    strncpy(featureString, "Block trigger transfer", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 2; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_BTC;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=8;
    dmaChTestParams.ctrlPacket.elemXferCnt=4;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Block;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* TEST: More frame trigger based transfer */
    /* addressing mode: src: constant;  dest: constant */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Constant - Constant)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 3; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=10;
    dmaChTestParams.ctrlPacket.elemXferCnt=1;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_Constant;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_Constant;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: constant;  dest: post incr */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Constant - Incr)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 4; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=10;
    dmaChTestParams.ctrlPacket.elemXferCnt=1;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_Constant;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: constant;  dest: indexed */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Constant - Indexed)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 5; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=6;
    dmaChTestParams.ctrlPacket.elemXferCnt=4;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_16bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_16bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_Constant;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_Indexed;
    dmaChTestParams.ctrlPacket.destElemIndexOffset=0x40;
    dmaChTestParams.ctrlPacket.destFrameIndexOffset=0x8;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: post incr; dest: constant */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Incr - Constant)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 6; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=(uint32_t)gDMATestSource;
    dmaChTestParams.ctrlPacket.destAddr=(uint32_t)gDMATestDest;
    dmaChTestParams.ctrlPacket.frameXferCnt=5;
    dmaChTestParams.ctrlPacket.elemXferCnt=1;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_Constant;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: post incr; dest: post incr */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Incr - Incr)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 7; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestSource,DMA_ElemSize_64bit);
    dmaChTestParams.ctrlPacket.destAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestDest,DMA_ElemSize_64bit);
    dmaChTestParams.ctrlPacket.frameXferCnt=2;
    dmaChTestParams.ctrlPacket.elemXferCnt=5;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_64bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_64bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: post incr; dest: indexed */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Incr - Indexed)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 8; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestSource,DMA_ElemSize_32bit);
    dmaChTestParams.ctrlPacket.destAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestDest,DMA_ElemSize_32bit);
    dmaChTestParams.ctrlPacket.frameXferCnt=2;
    dmaChTestParams.ctrlPacket.elemXferCnt=5;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_32bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_PostIncrement;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_Indexed;
    dmaChTestParams.ctrlPacket.destElemIndexOffset=0x40;
    dmaChTestParams.ctrlPacket.destFrameIndexOffset=0x8;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: indexed;   dest: constant */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Indexed - constant)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 9; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestSource,DMA_ElemSize_8bit);
    dmaChTestParams.ctrlPacket.destAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestDest,DMA_ElemSize_8bit);
    dmaChTestParams.ctrlPacket.frameXferCnt=4;
    dmaChTestParams.ctrlPacket.elemXferCnt=1;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_8bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_Indexed;
    dmaChTestParams.ctrlPacket.srcElemIndexOffset=0x10; //will not be used as elemXferCnt is 1
    dmaChTestParams.ctrlPacket.srcFrameIndexOffset=0x4;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_Constant;
    /* do the memory transfer */
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: indexed;   dest: post incr */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Indexed - Incr)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 11; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestSource,DMA_ElemSize_16bit);
    dmaChTestParams.ctrlPacket.destAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestDest,DMA_ElemSize_16bit);
    dmaChTestParams.ctrlPacket.frameXferCnt=4;
    dmaChTestParams.ctrlPacket.elemXferCnt=2;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_16bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_16bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_Indexed;
    dmaChTestParams.ctrlPacket.srcElemIndexOffset=0x14;
    dmaChTestParams.ctrlPacket.srcFrameIndexOffset=0x4;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_PostIncrement;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    } 
    
    /* addressing mode: src: indexed;   dest: indexed */
    testNum++;
    strncpy(featureString,"Frame trigger transfer (Indexed - Indexed)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    /* initialize */
    memset(&dmaChTestParams,0,sizeof(dmaChTestParams));
    memset(gDMATestSource,'S',sizeof(gDMATestSource)); 
    memset(gDMATestDest,'D',sizeof(gDMATestDest));
    /* set the params */
    dmaChTestParams.channelNum = 12; /* select any channel */
    dmaChTestParams.intType = DMA_IntType_FTC;
    dmaChTestParams.ctrlPacket.srcAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestSource,DMA_ElemSize_64bit);
    dmaChTestParams.ctrlPacket.destAddr=DMA_TEST_ALIGN_ADDRESS((uint32_t)gDMATestDest,DMA_ElemSize_64bit);
    dmaChTestParams.ctrlPacket.frameXferCnt=3;
    dmaChTestParams.ctrlPacket.elemXferCnt=8;
    dmaChTestParams.ctrlPacket.nextChannel=DMA_CHANNEL_NONE;
    dmaChTestParams.ctrlPacket.srcElemSize=DMA_ElemSize_64bit;
    dmaChTestParams.ctrlPacket.destElemSize=DMA_ElemSize_64bit;
    dmaChTestParams.ctrlPacket.autoInitiation=0;
    dmaChTestParams.ctrlPacket.xferType=DMA_XferType_Frame;
    dmaChTestParams.ctrlPacket.srcAddrMode=DMA_AddrMode_Indexed;
    dmaChTestParams.ctrlPacket.srcElemIndexOffset=0x80;
    dmaChTestParams.ctrlPacket.srcFrameIndexOffset=0x10;
    dmaChTestParams.ctrlPacket.destAddrMode=DMA_AddrMode_Indexed;
    dmaChTestParams.ctrlPacket.destElemIndexOffset=0x10;
    dmaChTestParams.ctrlPacket.destFrameIndexOffset=0x80;
    /* do the memory transfer */
    errCode= DMA_Test_DoMemCopy(handle,&dmaChTestParams);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    }

    /* Auto-initiation mode */
    /* makes sense to be tested with H/W - see MIbSPI driver for example */

    /* channel chaining */
    testNum++;
    strncpy(featureString, "Channel Chaining", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    errCode = DMA_Test_DoChainedTransfer(handle);
    Test_indicateResultToAutomation(featureString,errCode);
    if (errCode != 0)
    {
        return;
    }

    /****************************/
    /* End mem - mem copy       */
    /****************************/

    /****************************/
    /* error tests              */
    /****************************/
    testNum++;
    strncpy(featureString, "Parameter error checking tests", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    DMA_Test_PerformErrorCheckTests(handle,featureString);

    testNum++;
    strncpy(featureString, "Bus error checking tests", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    DMA_Test_PerformBusErrorCheckTests(handle,featureString);
    
    /****************************/
    /* end - error tests        */
    /****************************/
    
    /****************************/
    /* misc functionality       */
    /****************************/
    /* benchmarking */
    testNum++;
    strncpy(featureString, "Benchmarking tests (interrupt based)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    gInQuietMode = 1;
    DMA_Test_PerformBenchmarking(handle,false);
    gInQuietMode = 0;
    Test_indicateResultToAutomation(featureString,errCode);

    testNum++;
    strncpy(featureString, "Benchmarking tests (polling based)", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    gInQuietMode = 1;
    DMA_Test_PerformBenchmarking(handle,true);
    gInQuietMode = 0;
    Test_indicateResultToAutomation(featureString,errCode);
    
#if 0
    testNum++;
    strncpy(featureString, "Benchmarking for CPU based copy", FEATURE_STRING_LEN);
    START_TEST_PRINT(featureString,instanceId,testNum);
    DMA_Test_PerformBenchmarking_CPUbased();
#endif
    
    /****************************/
    /* end - misc functionality */
    /****************************/


    /* Close the driver: */
    errCode = DMA_close(handle);
    System_printf("Debug: DMA Instance %p has been closed err:%d\n", handle,errCode);
    if (errCode == 0)
        MCPI_setFeatureTestResult("DMA_close", MCPI_TestResult_PASS);
    else
        MCPI_setFeatureTestResult("DMA_close", MCPI_TestResult_FAIL);

}


/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    uint32_t       instanceId;

    Cycleprofiler_init();

    /* Initialize the DMA */
    DMA_init();

    /* Debug Message: */
    System_printf ("Debug: DMA has been initialized\n");

    for (instanceId = 0; instanceId<DMA_NUM_INSTANCES; instanceId++)
    {
        Test_Instance(instanceId);
    }

    MCPI_setTestResult ();

    /* exit BIOS */
    BIOS_exit(0);

    return;
}


/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    Task_Params     taskParams;

    ESM_init(0U); //dont clear errors as TI RTOS does it

    /* Initialize test logger */
    MCPI_Initialize();

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Debug Message: */
    System_printf ("Debug: Launching BIOS \n");

    /* Start BIOS */
    BIOS_start();
    return 0;
}

