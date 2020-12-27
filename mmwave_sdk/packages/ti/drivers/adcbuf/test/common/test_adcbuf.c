/*
 *   @file  test_adcbuf.c
 *
 *   @brief
 *      ADCBUF common Unit Test code across mmWave devices
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

/* OSAL Library: */
#include <ti/drivers/osal/SemaphoreP.h>

/* ADCBUF test include files */
#include <ti/drivers/adcbuf/test/common/test_adcbuf.h>

/* MCPI logger include files */
#include <ti/utils/testlogger/logger.h>

/* EDMA include files */
#include <ti/drivers/edma/edma.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>

/* SOC include files */
#include <ti/drivers/soc/soc.h>

/**************************************************************************
 *************************** ADCBUF Test Constants & Variables *****************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))

/* Compile flag to use EDMA for buffer copying */
#define ENABLE_EDMA_COPY                             0

/* Test Pattern generation */
#define TOTAL_TEST_CASES                             4

/* Test cases with different data format */
ADCBuf_dataFormat dataFmtTestCase_xwr16xx[TOTAL_TEST_CASES]=
{
    /* Complex, Q+I, non-interleaved */
    {0, 0, 0},
    
    /* Complex, I+Q, non-interleaved */
    {0, 1, 0},

    /* Real, I, non-interleaved */
    {1, 0, 0},

    /* Real, Q, non-interleaved */
    {1, 1, 0},
};

#else
/* Test Pattern generation */
#define TOTAL_TEST_CASES                             8

/* Test cases with different data format */
ADCBuf_dataFormat dataFmtTestCase_xwr14xx[TOTAL_TEST_CASES]=
{
    /* Complex, Q+I, non-interleaved */
    {0, 0, 0},

    /* Complex, Q+I, interleaved */
    {0, 0, 1},
    
    /* Complex, I+Q, non-interleaved */
    {0, 1, 0},

    /* Complex, I+Q, interleaved */
    {0, 1, 1},

    /* Real, I, non-interleaved */
    {1, 0, 0},

    /* Real, I, interleaved */
    {1, 0, 1},

    /* Real, Q, non-interleaved */
    {1, 1, 0},

    /* Real, Q, interleaved */
    {1, 1, 1}
};
#endif

/* Test Control variables */
uint32_t  gTestRun = 1;
SemaphoreP_Handle  gIntSemaHandle = NULL;

/* Test statistics */
uint32_t  gIntCounter = 0;
uint32_t  gVerifyFailCount = 0;

/**************************************************************************
 *************************** ADCBUF Test extern definitions *********************
 **************************************************************************/

/* Device specific test parameters */
ADCBUFTestParams* gPtrADCBUFTestParam = NULL;

/**************************************************************************
 *************************** ADCBUF Test Internal Function prototype *************
 **************************************************************************/
static int32_t Test_ADCBUFApiTest(void);
static void  Test_ADCBUFIntCallBackFunc(uintptr_t arg);
static void Test_ADCBUFContModeConfig(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat);
static void Test_ADCBUFContDataPathWithTestPattern(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat);
static int32_t Test_ADCBUFCmdParamsCheckTest(void);
static uint32_t Test_ADCBUFVerifyDataPattern(void *memAddr, ADCBuf_dataFormat *ptrDataFormat);

#if ENABLE_EDMA_COPY
static EDMA_Handle Test_ADCBUFInitEDMA(void);
static int32_t Test_ADCBUFEDMA_setup_shadow_link (EDMA_Handle handle, uint32_t chId, uint32_t linkChId,
    EDMA_paramSetConfig_t *config, EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn);
static void Test_ADCBUFCloseEDMA(EDMA_Handle handle);
static int32_t Test_ADCBUFConfigEdma(EDMA_Handle handle, uint8_t chId, 
    uint32_t* pSrcAddress, uint32_t * pDestAddress,
    uint16_t aCount, uint16_t bCount, uint16_t cCount, 
    uint16_t linkChId);

static void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo);
static void Test_edmaTransferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo);
#endif
/**************************************************************************
 *************************** ADCBUF Test Functions **************************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      Interrupt handler callback for chirp available. It runs in the ISR context.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *
 *  @retval
 *      Not Applicable.
 */
void Test_ADCBUFIntCallBackFunc(uintptr_t arg)
{
    ADCBuf_Handle     handle;

    handle = (ADCBuf_Handle)arg;
    
    /* Increment interrupt counter for debugging purpose */
    gIntCounter++;

    /* Check the buffer every 10 chirp avail interrupt */
    if(gIntCounter % 10 == 0)
    {
        /* Stop Continuous mode */
        ADCBuf_control(handle, ADCBufMMWave_CMD_STOP_CONTINUOUS_MODE, NULL);
    
        /* Stop Test Pattern generation */
        ADCBuf_control(handle, ADCBufMMWave_CMD_STOP_TEST_PATTERN, NULL);

        /* Semaphore post for test thread to copy samples and verify */
        SemaphoreP_post(gIntSemaHandle);
    }
}

/**
 *  @b Description
 *  @n
 *      Continuous mode with Test patern generation test.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *  @param[in]  ptrDataFormat
 *      Pointer to the ADCBuf Data format configuration
 *
 *  @retval
 *      Not Applicable.
 */
void Test_ADCBUFContModeConfig(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat)
{
    uint32_t                             arg;
    ADCBuf_RxChanConf        rxChanConf;
    ADCBuf_TestPatternConf   testPatternConf;
    uint8_t                             channel;
    uint16_t                           offset = 0;
    uint32_t                           numSamples;
    int32_t                             retVal = 0;
    
    /* Configure ADC buffer data format */
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CONF_DATA_FORMAT, (void *)ptrDataFormat)) < 0)
        System_printf("Error: ADCBufMMWave_CMD_CONF_DATA_FORMAT failed with [Error=%d]\n", retVal);

    /* Enable all 4  Rx Channel */
    for (channel=0; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++)
    {
        rxChanConf.channel = channel;
        rxChanConf.offset    = offset;
        if((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CHANNEL_ENABLE, (void *)&rxChanConf)) < 0)
            System_printf("Error: ADCBufMMWave_CMD_CHANNEL_ENABLE failed for channel %d offset =0x%x with [Error = %d]\n", 
                                     channel, offset, retVal);
        /* Test purpose only : Verify channel address */
        {
            uint32_t channelAddr = 0;

            if ( (channelAddr = ADCBuf_getChanBufAddr(handle, channel, &retVal)) != 0)
            {
                channelAddr -= gPtrADCBUFTestParam->ADCBUFMemAddr;

                if(channelAddr != offset)
                {
                    System_printf("Error: ADCBuf_getChanBufAddr() return mismatched channel(%d) buffer address [%x: %x]\n", 
                                 channel, offset, channelAddr);

                    retVal = -1;
                }
            }
            else
            {
                System_printf("Error: ADCBuf_getChanBufAddr failed for channel %d with [Error = %d]\n", 
                                         channel, retVal);
            }
        }
        
        offset += gPtrADCBUFTestParam->numSamples * 4;
    }

    /* Configure ADC buffer in continuous mode */
    arg = 1;
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_SET_CONTINUOUS_MODE, (void *)&arg)) < 0)
        System_printf("Error: ADCBufMMWave_CMD_SET_CONTINUOUS_MODE failed with [Error=%d]\n", retVal);

    /* Start the continuous streaming mode in ADCBUFF */
    numSamples = gPtrADCBUFTestParam->numSamples ;
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_START_CONTINUOUS_MODE, (void *)&numSamples)) < 0)
        System_printf("Error: ADCBufMMWave_CMD_START_CONTINUOUS_MODE failed with [Error=%d]\n", retVal);
    
    /* Configure Test Pattern generation */
    testPatternConf.period = 255;
    testPatternConf.numSamples = gPtrADCBUFTestParam->numSamples;

    for (channel=0; channel < 4; channel++)
    {
        testPatternConf.rxConfig[channel].rxIOffset   = 0x0000;
        testPatternConf.rxConfig[channel].rxIInc       = 2;
        testPatternConf.rxConfig[channel].rxQOffset  = 0x1000 ;
        testPatternConf.rxConfig[channel].rxQInc      = 2;
    }

    /* Send control command to driver */    
    if ( (retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CONF_TEST_PATTERN, (void *)&testPatternConf)) < 0)
        System_printf("Error: ADCBufMMWave_CMD_CONF_TEST_PATTERN failed with [Error=%d]\n", retVal);
    
}

/**
 *  @b Description
 *  @n
 *      ADCBuf driver API test.
 *
 *  @retval
 *      =0       Test passed
 *      <0       Test Failed
 */
int32_t Test_ADCBUFApiTest(    void)
{
    ADCBuf_Params        params;
    ADCBuf_Handle        handle;
    int32_t              retVal = 0;

    /* Setup the default ADCBUF Parameters */
    ADCBuf_Params_init(&params);

    /* Open the first ADCBUF Instance */
    handle = ADCBuf_open(0, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the ADCBUF Instance\n");
        retVal = -1;
        goto Exit;
    }
    System_printf("Debug: ADCBUF Instance(0) %p has been opened successfully\n", handle);

    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    ADCBuf_close(handle);
    System_printf("Debug: ADCBUF Instance %p has been closed successfully\n", handle);

Exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      ADCBuf driver test to validate command parameters.
 *
 *  @retval
 *      =0       Test passed
 *      <0       Test Failed
 */
int32_t Test_ADCBUFCmdParamsCheckTest(void)
{
    ADCBuf_Params        params;
    ADCBuf_Handle        handle;
    uint32_t             arg;
    int32_t              retVal = 0;
    ADCBufMMWave_CMD     command;
    ADCBuf_dataFormat    dataFormat;
    ADCBuf_RxChanConf    rxChanConf;
    ADCBuf_CQConf        cqConf;
    int32_t              testResult = 0;

    /* Setup the default ADCBUF Parameters */
    ADCBuf_Params_init(&params);

    /* Open the first ADCBUF Instance */
    handle = ADCBuf_open(0, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the ADCBUF Instance\n");
        testResult = -1;
        goto Exit;
    }
    System_printf("Debug: ADCBUF Instance(0) %p has been opened successfully\n", handle);

    /* Params check Test */
    command = ADCBufMMWave_CMD_SET_SRC;
    arg = 0x5;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d with arg=%d\n", command, arg);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg=%d\n", command, arg);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_SET_CONTINUOUS_MODE;
    arg = 0x8;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d with arg=%d\n", command, arg);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg=%d\n", command, arg);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CONF_DATA_FORMAT;
    memset ((void *)&dataFormat, 0, sizeof(dataFormat));
    dataFormat.adcOutFormat = 2;
    if ((retVal = ADCBuf_control(handle, command, (void *)&dataFormat)) < 0)
        System_printf("Debug: Passed the param check for command=%d with adcOutFormat=%d \n", command, dataFormat.adcOutFormat);
    else
    {
        System_printf("Error: Failed the param check for command=%d with adcOutFormat=%d\n", command, dataFormat.adcOutFormat);
        testResult = -1;
    }

    memset ((void *)&dataFormat, 0, sizeof(dataFormat));
    dataFormat.sampleInterleave = 2;
    if ((retVal = ADCBuf_control(handle, command, (void *)&dataFormat)) < 0)
        System_printf("Debug: Passed the param check for command=%d with sampleInterleave=%d\n", command, dataFormat.sampleInterleave);
    else
    {
        System_printf("Error: Failed the param check for command=%d with sampleInterleave=%d\n", command, dataFormat.sampleInterleave);
        testResult = -1;
    }

    memset ((void *)&dataFormat, 0, sizeof(dataFormat));
    dataFormat.channelInterleave = 2;
    if ((retVal = ADCBuf_control(handle, command, (void *)&dataFormat)) < 0)
        System_printf("Debug: Passed the param check for command=%d channelInterleave=%d\n", command, dataFormat.channelInterleave);
    else
    {
        System_printf("Error: Failed the param check for command=%d with channelInterleave=%d\n", command, dataFormat.channelInterleave);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CONF_CQ;
    memset ((void *)&cqConf, 0, sizeof(cqConf));
    cqConf.cqDataWidth= 5;
    if ((retVal = ADCBuf_control(handle, command, (void *)&cqConf)) < 0)
        System_printf("Debug: Passed the param check for command=%d with cqDataWidth=%d \n", command, cqConf.cqDataWidth);
    else
    {
        System_printf("Error: Failed the param check for command=%d with cqDataWidth=%d\n", command, cqConf.cqDataWidth);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CONF_CQ;
    memset ((void *)&cqConf, 0, sizeof(cqConf));
    cqConf.cq96BitPackEn = 4;
    if ((retVal = ADCBuf_control(handle, command, (void *)&cqConf)) < 0)
        System_printf("Debug: Passed the param check for command=%d with cqDataWidth=%d \n", command, cqConf.cqDataWidth);
    else
    {
        System_printf("Error: Failed the param check for command=%d with cq96BitPackEn=%d\n", command, cqConf.cq96BitPackEn);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_START_CONTINUOUS_MODE;
    arg = 0x1U<<16;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d \n", command);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg=%d\n", command, arg);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CHANNEL_ENABLE;
    memset ((void *)&rxChanConf, 0, sizeof(rxChanConf));
    rxChanConf.channel = 6;
    if ((retVal = ADCBuf_control(handle, command, (void *)&rxChanConf)) < 0)
        System_printf("Debug: Passed the param check for command=%d with channel = %d\n", command, rxChanConf.channel);
    else
    {
        System_printf("Error: Failed the param check for command=%d with channel=%d\n", command, rxChanConf.channel);
        testResult = -1;
    }

    memset ((void *)&rxChanConf, 0, sizeof(rxChanConf));
    rxChanConf.channel = 2;
    rxChanConf.offset = 0x1U << 15U;
    if ((retVal = ADCBuf_control(handle, command, (void *)&rxChanConf)) < 0)
        System_printf("Debug: Passed the param check for command=%d with offset = %d\n", command, rxChanConf.offset);
    else
    {
        System_printf("Error: Failed the param check for command=%d with offset=%d\n", command, rxChanConf.offset);
        testResult = -1;
    }
    
    command = ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD;
    arg = 35;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg = %d , retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD;
    arg = 35;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD;
    arg = 35;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CHANNEL_DISABLE;
    arg = 0xc;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
    {
        System_printf("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }
    else
    {
        System_printf("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    }

    command = ADCBufMMWave_CMD_CHANNEL_DISABLE;
    arg = 0x1c;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    command = ADCBufMMWave_CMD_CHANNEL_DISABLE;
    arg = 0x0;
    if ((retVal = ADCBuf_control(handle, command, (void *)&arg)) < 0)
        System_printf("Debug: Passed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
    else
    {
        System_printf("Error: Failed the param check for command=%d with arg = %d, retVal=%d\n", command, arg, retVal);
        testResult = -1;
    }

    /* Close ADCbuf driver */
    ADCBuf_close(handle);
    System_printf("Debug: ADCBUF Instance %p has been closed successfully\n", handle);

Exit:
    return testResult;
}

/**
 *  @b Description
 *  @n
 *      Continuous mode with Test patern generation test.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *  @param[in]  ptrDataFormat
 *      Pointer to the ADCBuf Data format configuration
 *
 *  @retval
 *      Not Applicable.
 */
void Test_ADCBUFContDataPathWithTestPattern(ADCBuf_Handle handle, ADCBuf_dataFormat *ptrDataFormat)
{
    int32_t retVal = 0;
    
    /* Configurate test pattern */
    Test_ADCBUFContModeConfig(handle, ptrDataFormat);

    /* Start Test Pattern generation */
    if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_START_TEST_PATTERN, NULL) ) < 0)
       System_printf("Error: ADCBufMMWave_CMD_START_TEST_PATTERN failed with [Error=%d]\n", retVal);
}

/**
 *  @b Description
 *  @n
 *      Verify the test data pattern .
 *
 *  @param[in]  memAddr
 *      Memory base address that holds the data copied from ADC buffer 
 *  @param[in]  ptrDataFormat
 *      Pointer to the ADCBuf Data format configuration
 *
 *  @retval
 *      Not Applicable.
 */
uint32_t Test_ADCBUFVerifyDataPattern(void *memAddr, ADCBuf_dataFormat *ptrDataFormat)
{
    volatile uint32_t*   ptrDataAddress;
    uint8_t              channel;
    uint32_t             sampleIdx;
    volatile uint32_t    addrOffset;
    uint32_t             data, firstSample;
    uint16_t             idata, qdata;
    uint32_t             incr = 2;
    uint32_t             failFlag = 0;
    uint32_t             dataIncr;
    uint32_t             numSamples;
    
    ptrDataAddress = (volatile uint32_t *)memAddr;
    System_printf("Debug: First 4 32 bit words 0x%x     0x%x    0x%x    0x%x\n", ptrDataAddress[0], 
                        ptrDataAddress[1], ptrDataAddress[2], ptrDataAddress[3]);

    /* Set the address offset */
    addrOffset = 0;

    /* Get number of samples from test params */
    numSamples = gPtrADCBUFTestParam->numSamples;

    /* Get the first sample */
    firstSample = *ptrDataAddress;
    
    /* Channel interleaved-mode */
    if (ptrDataFormat->channelInterleave == 0)
    {
        /* Test 10 samples for all channels */
        for (sampleIdx=0; sampleIdx < numSamples; sampleIdx++)
        {
            for (channel = 0; channel < 4; channel++)
            {  
                addrOffset = sampleIdx * 4;
                if(ptrDataFormat->adcOutFormat == 0)
                {
                    data = ptrDataAddress[addrOffset + channel];

                    /* Complex data format, get I and Q data */
                    if(ptrDataFormat->sampleInterleave == 0)
                    {
                        qdata = (data & 0xffff0000) >> 16;
                        idata  = (data & 0xffff);
                    }
                    else
                    {
                        idata = (data & 0xffff0000) >> 16;
                        qdata  = (data & 0xffff);
                    }

                    /* Verify data 1: increment = 2, */
                    dataIncr =( ( sampleIdx * incr)<<16) + (incr * sampleIdx);
                    if((data - firstSample) != dataIncr)
                    {
                         failFlag = 1;
                    }

                    /* Verify data 2: Q = I + 0x1000 */
                    if( (idata + 0x1000) != qdata)
                    {
                         failFlag = 1;
                    }
                }
                else
                {
                    uint16_t *ptrRealData = (uint16_t *)ptrDataAddress;
                    uint16_t  realData;
                    uint16_t  firstRealSample;

                    /* Address offset in terms of 16bits real data */
                    addrOffset = sampleIdx * 4;
                    realData = ptrRealData[addrOffset + channel];
                    firstRealSample = ptrRealData[channel];
                
                    /* Verify data 1: increment = 2, */
                    dataIncr =incr * sampleIdx;
                    if((realData - firstRealSample) != dataIncr)
                    {
                         failFlag = 1;
                    }
                }
            }
        }
         
    }
    else
    {
        for (channel = 0; channel < 4; channel++)
        {
            addrOffset = (channel * numSamples) ;
            firstSample = ptrDataAddress[addrOffset];

            for (sampleIdx=0; sampleIdx<numSamples; sampleIdx++)
            {
                data = ptrDataAddress[addrOffset + sampleIdx];
                
                if(ptrDataFormat->adcOutFormat == 0)
                {
                    /* Complex data format, get I and Q data */
                    if(ptrDataFormat->sampleInterleave == 0)
                    {
                        qdata = (data & 0xffff0000) >> 16;
                        idata  = (data & 0xffff);
                    }
                    else
                    {
                        idata = (data & 0xffff0000) >> 16;
                        qdata  = (data & 0xffff);
                    }

                    /* Verify data 1: increment = 2, */
                    dataIncr =( ( sampleIdx * incr)<<16) + (incr * sampleIdx);
                    if((data - firstSample) != dataIncr)
                    {
                         failFlag = 1;
                    }

                    /* Verify data 2: Q = I + 0x1000 */
                    if( (idata + 0x1000) != qdata)
                    {
                         failFlag = 1;
                    }
                }
                else
                {
                    volatile uint16_t *ptrRealData = (uint16_t *)ptrDataAddress;
                    uint16_t  realData;
                    uint16_t  firstRealSample;

                    /* Address offset in terms of 16bits real data */
                    addrOffset = (channel * numSamples * 2) ;
                    realData = ptrRealData[addrOffset + sampleIdx];
                    firstRealSample = ptrRealData[addrOffset];
                
                    /* Verify data 1: increment = 2, */
                    dataIncr =incr * sampleIdx;
                    if((realData - firstRealSample) != dataIncr)
                    {
                         failFlag = 1;
                    }
                }
            }
            
        }
    }
    return ((failFlag == 0) ? 0 : 1);
}

#if ENABLE_EDMA_COPY
/**
 *  @b Description
 *  @n
 *      Call back function for EDMA CC (Channel controller) error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      Call back function for EDMA transfer controller error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void Test_edmaTransferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      Init EDMA driver for data buffer copy.
 *
 *  @retval
 *      Success     - EDMA handle    
 *      Fail        - NULL pointer
 */
EDMA_Handle Test_ADCBUFInitEDMA(void)
{
    EDMA_errorInfo_t  EDMAErrorInfo;
    EDMA_transferControllerErrorInfo_t EDMATransferControllerErrorInfo;
    EDMA_Handle         EdmaHandle = NULL;
    EDMA_instanceInfo_t instanceInfo;
    EDMA_errorConfig_t errorConfig;
    
    int32_t    retVal = 0;

    retVal = EDMA_init(0);
    if (retVal != EDMA_NO_ERROR)
    {
        System_printf ("Debug: EDMA instance 0 initialization returned error %d\n", retVal);
        return NULL;        
    }
    System_printf ("Debug: EDMA instance 0 has been initialized\n");

    memset(&EDMAErrorInfo, 0, sizeof(EDMAErrorInfo));
    memset(&EDMATransferControllerErrorInfo, 0, sizeof(EDMATransferControllerErrorInfo));

    /* Open the first EDMA Instance */
    EdmaHandle = EDMA_open(0, &retVal, &instanceInfo);
    if (EdmaHandle == NULL)
    {
        System_printf("Error: Unable to open the edma Instance, erorCode = %d\n", retVal);
    }

    /* Configurate EDMA Error Monitor */
    errorConfig.isConfigAllEventQueues = true;
    errorConfig.isConfigAllTransferControllers = true;
    errorConfig.isEventQueueThresholdingEnabled = true;
    errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    errorConfig.callbackFxn = Test_edmaErrorCallbackFxn;
    errorConfig.transferControllerCallbackFxn = Test_edmaTransferControllerErrorCallbackFxn;
    if ((retVal = EDMA_configErrorMonitoring(EdmaHandle, &errorConfig)) != EDMA_NO_ERROR)
    {
        System_printf("Debug: EDMA_configErrorMonitoring() failed with errorCode = %d\n", retVal);
        return NULL;
    }

    return EdmaHandle;
}

/**
 *  @b Description
 *  @n
 *      Setup shadow link for EDMA copy.
 *
 *  @param[in]  handle
 *      EDMA driver instance Handle 
 *  @param[in]  chId
 *      EDMA Channle id used to copy data buffer
 *  @param[in]  linkChId
 *      EDMA link id used to copy data buffer
 *  @param[in]  config
 *      EDMA param set used to copy data buffer
 *  @param[in]  transferCompletionCallbackFxn
 *      Callback function when data copy is completed
 *
 *  @retval
 *      Success    - 0
 *      Fail       < 0 (ErrorCode)
 */
static int32_t Test_ADCBUFEDMA_setup_shadow_link (EDMA_Handle handle, uint32_t chId, uint32_t linkChId,
    EDMA_paramSetConfig_t *config, EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn)
{
    EDMA_paramConfig_t paramConfig;
    int32_t errorCode = EDMA_NO_ERROR;

    paramConfig.paramSetConfig = *config; //this will copy the entire param set config
    paramConfig.transferCompletionCallbackFxn = transferCompletionCallbackFxn;
    paramConfig.transferCompletionCallbackFxnArg = (uintptr_t) handle;
    if ((errorCode = EDMA_configParamSet(handle, linkChId, &paramConfig)) != EDMA_NO_ERROR)
    {
        System_printf("Error: EDMA_configParamSet() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if ((errorCode = EDMA_linkParamSets(handle, chId, linkChId)) != EDMA_NO_ERROR)
    {
        System_printf("Error: EDMA_linkParamSets() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if ((errorCode = EDMA_linkParamSets(handle, linkChId, linkChId)) != EDMA_NO_ERROR)
    {
        System_printf("Error: EDMA_linkParamSets() failed with error code = %d\n", errorCode);
        goto exit;
    }

    
exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      Config EDMA channel for data buffer copy.
 *
 *  @param[in]  handle
 *      EDMA driver instance Handle 
 *  @param[in]  chId
 *      EDMA Channle id used to copy data buffer
 *  @param[in]  pSrcAddress
 *      Source address 
 *  @param[in]  pDestAddress
 *      Destination address 
 *  @param[in]  aCount
 *      EDMA channel Acount configuration
 *  @param[in]  bCount
 *      EDMA channel Bcount configuration
 *  @param[in]  cCount
 *      EDMA channel Ccount configuration
 *  @param[in]  linkChId
 *      EDMA link Id
 *
 *  @retval
 *      Success    - 0
 *      Fail       < 0 (ErrorCode)
 */
int32_t Test_ADCBUFConfigEdma(EDMA_Handle handle, uint8_t chId, 
                                    uint32_t* pSrcAddress, uint32_t * pDestAddress,
                                    uint16_t aCount, uint16_t bCount, uint16_t cCount, 
                                    uint16_t linkChId)
{
    EDMA_channelConfig_t config;
    int32_t errorCode = EDMA_NO_ERROR;

    config.channelId = chId;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = chId;
    config.eventQueueId = 0;

    config.paramSetConfig.sourceAddress = (uint32_t) pSrcAddress;
    config.paramSetConfig.destinationAddress = (uint32_t) pDestAddress;

    config.paramSetConfig.aCount = aCount;
    config.paramSetConfig.bCount = bCount;
    config.paramSetConfig.cCount = cCount;
    config.paramSetConfig.bCountReload = config.paramSetConfig.bCount;

    config.paramSetConfig.sourceBindex = 0;
    config.paramSetConfig.destinationBindex = 0;

    config.paramSetConfig.sourceCindex = 0;
    config.paramSetConfig.destinationCindex = 0;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.transferCompletionCode = 0;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;

    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled = true;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = false;
    config.paramSetConfig.isFinalChainingEnabled = false;
    config.paramSetConfig.isIntermediateChainingEnabled = false;
    config.transferCompletionCallbackFxn = NULL;

    if ((errorCode = EDMA_configChannel(handle, &config, false)) != EDMA_NO_ERROR)
    {
        System_printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        goto Exit;
    }

    errorCode = Test_ADCBUFEDMA_setup_shadow_link(handle, chId, linkChId,
        &config.paramSetConfig, config.transferCompletionCallbackFxn);

Exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      Close EDMA driver instance
 *
 *  @param[in]  handle
 *      EDMA driver instance Handle 
 *
 *  @retval
 *      Not applicable
 */
void Test_ADCBUFCloseEDMA(EDMA_Handle handle)
{
    int32_t    retVal = 0;

    if ((retVal = EDMA_close(handle)) != EDMA_NO_ERROR)
    {
        printf("Debug: edma Instance %p closing error, errorCode = %d\n", handle, retVal);
    }
}
#endif
/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
void Test_ADCBUFInitTask(UArg arg0, UArg arg1)
{
    ADCBuf_Params        params;
    ADCBuf_Handle        handle;
    SemaphoreP_Params    semParams;
    ADCBuf_dataFormat    dataFormat;
    uint8_t              index;
    int32_t              retVal = 0;
    char *               testCaseString[64];
    volatile uint32_t    srcMemAddr;
    volatile uint32_t    dstMemAddr;
    ADCBuf_dataFormat    *ptrDataFormat = &dataFormat;
    uint32_t             numSamples;
#if ENABLE_EDMA_COPY
    EDMA_Handle          EdmaHandle = NULL;
    bool                 edmaTransComplete ;
#endif
    SOC_SysIntListenerHandle chirpIntHandle;
    SOC_SysIntListenerCfg  chirpIntCfg;
    SOC_Cfg              socCfg;
    SOC_Handle           socHandle;
    int32_t              errCode;

    /**************************************************************************
     * Test: MCPI_Initialize
     **************************************************************************/
    /* Initialize MCPI logger framework */
    MCPI_Initialize ();

    /* Get Test params from input arg */
    if(arg0 != NULL)
        gPtrADCBUFTestParam = (ADCBUFTestParams*)arg0;
    else
        goto Exit;

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_BYPASS_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return ;
    }
    System_printf ("Debug: SOC Module Initialization succeeded, handle = %p\n", socHandle);
    
    /* Initialize the ADCBUF */
    ADCBuf_init();

    if(gPtrADCBUFTestParam->enableEdma)
    {
#if ENABLE_EDMA_COPY
        EdmaHandle = Test_ADCBUFInitEDMA();
        if(EdmaHandle == NULL)
        {
            System_printf("Debug: EDMA Init failed \n");
            goto Exit;
        }
#endif
    }

    /**************************************************************************
     * Test: MCPI Feature: Basic API test
     **************************************************************************/

    /* Basic API Test */
    if(Test_ADCBUFApiTest() < 0)
        MCPI_setFeatureTestResult ("ADCBUF Basic API test", MCPI_TestResult_FAIL);
    else
        MCPI_setFeatureTestResult ("ADCBUF Basic API test", MCPI_TestResult_PASS);

    /**************************************************************************
     * Test: MCPI Feature: Parameter validation test
     **************************************************************************/

    /* Test input parameters */
    if (Test_ADCBUFCmdParamsCheckTest() < 0)
        MCPI_setFeatureTestResult ("ADCBUF Input Parameters Validation Test", MCPI_TestResult_FAIL);
    else
        MCPI_setFeatureTestResult ("ADCBUF Input Parameters Validation Test", MCPI_TestResult_PASS);

    /**************************************************************************
     * Test: Reopen the driver to test in Continous Mode
     **************************************************************************/
    
    /* YES: Create a binary semaphore which is used to handle interrupt handling. */
    SemaphoreP_Params_init(&semParams);
    semParams.mode             = SemaphoreP_Mode_BINARY;
    gIntSemaHandle             = SemaphoreP_create(0, &semParams);

    
    /* Setup the default ADCBUF Parameters */
    ADCBuf_Params_init(&params);
    params.continousMode = 1;
    
    /**************************************************************************
     * Test: MCPI Feature: Test pattern validation test with different data format
     **************************************************************************/

    /* Go through different data format test cases */
    for (index = 0; index < TOTAL_TEST_CASES; index++)
    {
        handle = ADCBuf_open(0, &params);
        if (handle == NULL)
        {
            System_printf("Error: Unable to open the ADCBUF Instance\n");
            return;
        }
        System_printf("Debug: ADCBUF Instance %p has been reopened successfully\n", handle);

        /* Register chirp interrupt */
        memset((void *)&chirpIntCfg, 0 , sizeof(SOC_SysIntListenerCfg));
        chirpIntCfg.systemInterrupt    = gPtrADCBUFTestParam->chirpIntNumber;
        chirpIntCfg.listenerFxn        = Test_ADCBUFIntCallBackFunc;
        chirpIntCfg.arg                = (uintptr_t)handle;
        if ((chirpIntHandle = SOC_registerSysIntListener(socHandle, &chirpIntCfg, &errCode)) == NULL)
        {
            System_printf("Error: Unable to register chirp interrupt listener , error = %d\n", errCode);
            return;
        }

#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
        /* Initialize the data format for the test */
        dataFormat = dataFmtTestCase_xwr16xx[index];
#else
        /* Initialize the data format for the test */
        dataFormat = dataFmtTestCase_xwr14xx[index];
#endif
        /* Print out the test params */
        sprintf((char *)testCaseString, "ADCBUF Test Pattern with dataFormat=%d, sampleSwap=%d, interleave=%d", 
            dataFormat.adcOutFormat, dataFormat.sampleInterleave, dataFormat.channelInterleave);
        System_printf("Debug: Start %s\n", testCaseString);

        /* Number of loops that test run for current data Format
            Data is validated for every 10 ping/pong buffer swap. 
          */
        gTestRun = 20;

        /* ADCBuf Test Pattern Test, this function will be blocked until gTestRun is set to 0 */
        Test_ADCBUFContDataPathWithTestPattern(handle, &dataFormat);

        /* Assign memory base address to save the ADC samples */
        dstMemAddr = gPtrADCBUFTestParam->dstMemAddr;
        srcMemAddr = gPtrADCBUFTestParam->ADCBUFMemAddr;
        numSamples = gPtrADCBUFTestParam->numSamples;

        while(gTestRun)
        {
            /* Wait for the Semaphore */
            SemaphoreP_pend(gIntSemaHandle, BIOS_WAIT_FOREVER);


            if(gPtrADCBUFTestParam->enableEdma)
            {
#if ENABLE_EDMA_COPY
                edmaTransComplete = false;
                /* Trigger DMA copy */
                Test_ADCBUFConfigEdma(EdmaHandle, EDMA_TPCC0_REQ_FREE_0, 
                                      (uint32_t *)SOC_translateAddress(srcMemAddr,SOC_TranslateAddr_Dir_TO_EDMA,NULL), 
                                      (uint32_t *)SOC_translateAddress(dstMemAddr,SOC_TranslateAddr_Dir_TO_EDMA,NULL),
                                      16 * numSamples, 1, 1, EDMA_NUM_DMA_CHANNELS);
    
                EDMA_startDmaTransfer(EdmaHandle, EDMA_TPCC0_REQ_FREE_0);
    
                /* Polling transfer compeletion */
                while(edmaTransComplete == false)
                {
                    EDMA_isTransferComplete(EdmaHandle, 0, &edmaTransComplete);
                    Task_sleep(1);
                }
#endif                
            }
            else
            {
                /* Copy the adcbuf data to memory */
                memcpy( (void *)(dstMemAddr),  (void *)(srcMemAddr),  16 * numSamples );
            }
            
            /* Verify data intergrity */
            if (Test_ADCBUFVerifyDataPattern((void *)dstMemAddr, ptrDataFormat) !=0 )
            {
                gVerifyFailCount++;
            }
            
            gTestRun--;
            
            /* Re-Start Test Pattern generation if test is not stopped */
            if(gTestRun)
            {
                uint32_t  rxChanMask;

                /* Disable all channels */
                rxChanMask = 0xF;
                if ((retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_CHANNEL_DISABLE, (void *)&rxChanMask)) < 0)
                {
                   System_printf("Error: ADCBufMMWave_CMD_CHANNEL_DISABLE failed with [Error=%d]\n", retVal);
                }

                /* ADCBuf Test Pattern Test, this function will be blocked until gTestRun is set to 0 */
                Test_ADCBUFContDataPathWithTestPattern(handle, &dataFormat);
            }
        }

        /* StopTest Pattern generation */
        if (( retVal = ADCBuf_control(handle, ADCBufMMWave_CMD_STOP_TEST_PATTERN, NULL)) < 0)
           System_printf("Error: ADCBufMMWave_CMD_STOP_TEST_PATTERN failed with [Error=%d]\n", retVal);

        /* Close  ADCBuf driver */
        ADCBuf_close(handle);
        System_printf("Debug: ADCBUF Instance %p has been closed successfully\n", handle);

        /* Deregister chirp interrupt listener */
        if (SOC_deregisterSysIntListener(socHandle, 
                                       gPtrADCBUFTestParam->chirpIntNumber,
                                       chirpIntHandle,
                                       &errCode) < 0)
        {
            System_printf("Error: Unable to deregister chirp interrupt listener , error = %d\n", errCode);
            return;
        }
        chirpIntHandle = NULL;
        
        /* Send test results to logger */
        if (gVerifyFailCount > 0)
            MCPI_setFeatureTestResult ((const char *)testCaseString, MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult ((const char *)testCaseString, MCPI_TestResult_PASS);

        /* Clear the counter */
        gIntCounter = 0;
        gVerifyFailCount = 0;

    }
    
    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    /* Close EDMA driver */ 
#if ENABLE_EDMA_COPY
    if(EdmaHandle != NULL)
        Test_ADCBUFCloseEDMA(EdmaHandle);
#endif
    /* Delete semaphore */
    SemaphoreP_delete(gIntSemaHandle);
    System_printf("Debug: Semaphore Instance %p has been closed successfully\n", gIntSemaHandle);

Exit:
    System_printf("ADCBuf test finished. \n");

    MCPI_setTestResult ();

    /* After test all done, terminate DSP by calling BIOS_exit(). 
        This is required by MCPI test framework script. 
        MCPI test framework script waits for DSP halt, 
        if DSP doesn't halt, script will wait until timeout then claim test fail. 
     */ 
    BIOS_exit(0); 

    return;
}

