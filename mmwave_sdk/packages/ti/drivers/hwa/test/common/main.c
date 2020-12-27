/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the HWA
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
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/hwa/hwa.h>
#include <ti/drivers/soc/soc.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/**************************************************************************
 *************************** Global Definitions ****************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* using this as global to facilitate debugging */
HWA_ParamConfig gHWATestParamConfig[16];
HWA_CommonConfig gCommonConfig;

uint32_t gHWATestParamSetISR = 0;
uint32_t gHWATestDoneISR = 0;

SOC_Handle gSOCHandle;



/* input vector */
#include "1dFFT_in.h"
#include "1dFFT_out.h"
#include "fft_window.h"

/* ADC data charateristics */
#define HWA_TEST_NUM_SAMPLES     225
#define HWA_TEST_1DFFT_SIZE      256
#define HWA_TEST_NUM_RX_ANT         4
#define HWA_TEST_COMPLEX_16BIT_SIZE 4
/* Input data characteristics */
#define HWA_TEST_IS_INPUT_REAL            0
#define HWA_TEST_IS_INPUT_32BIT           0
#define HWA_TEST_IS_INPUT_SIGNED          1
/* window RAM */
#define HWA_TEST_1DFFT_WINDOW_START       0
/* DMA channel */
#define HWA_TEST_SRC_TRIGGER_DMACH0       0
#define HWA_TEST_SRC_TRIGGER_DMACH1       1

#define HWA_MEMn_SIZE                     16*1024 //16K size

#ifdef SOC_XWR14XX
#define SOC_HWA_MEM0 SOC_XWR14XX_MSS_HWA_MEM0_BASE_ADDRESS
#define SOC_HWA_MEM2 SOC_XWR14XX_MSS_HWA_MEM2_BASE_ADDRESS
#endif

#ifdef SOC_XWR18XX
#ifdef SUBSYS_MSS
#define SOC_HWA_MEM0 SOC_XWR18XX_MSS_HWA_MEM0_BASE_ADDRESS
#define SOC_HWA_MEM2 SOC_XWR18XX_MSS_HWA_MEM2_BASE_ADDRESS
#else
#define SOC_HWA_MEM0 SOC_XWR18XX_DSS_HWA_MEM0_BASE_ADDRESS
#define SOC_HWA_MEM2 SOC_XWR18XX_DSS_HWA_MEM2_BASE_ADDRESS
#endif
#endif

#ifdef SOC_XWR68XX
#ifdef SUBSYS_MSS
#define SOC_HWA_MEM0 SOC_XWR68XX_MSS_HWA_MEM0_BASE_ADDRESS
#define SOC_HWA_MEM2 SOC_XWR68XX_MSS_HWA_MEM2_BASE_ADDRESS
#else
#define SOC_HWA_MEM0 SOC_XWR68XX_DSS_HWA_MEM0_BASE_ADDRESS
#define SOC_HWA_MEM2 SOC_XWR68XX_DSS_HWA_MEM2_BASE_ADDRESS
#endif
#endif

/**************************************************************************
 *************************** HWA Test Functions **************************
 **************************************************************************/
void HWA_Test_ParamSetISR_Callback(uint32_t paramSet, void * arg)
{
    SemaphoreP_Handle       semHandle;
    gHWATestParamSetISR++;

    if (arg!=NULL) {
        semHandle = (SemaphoreP_Handle)arg;
        SemaphoreP_post(semHandle);
    }
}

void HWA_Test_DoneISR_Callback(void * arg)
{
    SemaphoreP_Handle       semHandle;
    gHWATestDoneISR++;

    if (arg!=NULL) {
        semHandle = (SemaphoreP_Handle)arg;
        SemaphoreP_post(semHandle);
    }
}




uint32_t log2Approx(uint32_t x)
{
    uint32_t idx,detectFlag=0;

    if ( x < 2)
    {
        return (0);
    }

    idx=32;
    while((detectFlag==0)||(idx==0))
    {
        if(x&0x80000000)
        {
            detectFlag=1;
        }
        x<<=1;
        idx--;
    }

    return(idx);
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
    HWA_Handle              handle;
    int32_t                 errCode, compareCode1, compareCode2;
    uint8_t                 paramsetIdx = 0, pingParamSetIdx = 0;// pongParamSetIdx = 0;
    uint8_t                *srcAddr = (uint8_t*)SOC_HWA_MEM0;
    uint8_t                *dstAddr = (uint8_t*)SOC_HWA_MEM2;
#if 1
    HWA_InterruptConfig     paramISRConfig;
    SemaphoreP_Handle       paramSetSem;
#endif
    SemaphoreP_Handle       doneSem;
    SemaphoreP_Params       semParams;
    SemaphoreP_Status       status;
//    HWA_Stats appHWAStats[3];

    /**************************************************************************
     * Initialize the HWA
     **************************************************************************/
    HWA_init();
    MCPI_setFeatureTestResult("API HWA_init()", MCPI_TestResult_PASS);
    /* Debug Message: */
    System_printf ("Debug: HWA has been initialized\n");

    /**************************************************************************
     * Open the HWA Instance
     **************************************************************************/
    handle = HWA_open(0, gSOCHandle, &errCode);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the HWA Instance err:%d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_open()", MCPI_TestResult_FAIL);
        return;
    }
    System_printf("Debug: HWA Instance %p has been opened successfully\n", handle);
    MCPI_setFeatureTestResult("API HWA_open()", MCPI_TestResult_PASS);

    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    errCode = HWA_close(handle);
    if (errCode == 0) {
        System_printf("Debug: HWA Instance %p closed successfully\n", handle);
        MCPI_setFeatureTestResult("API HWA_close", MCPI_TestResult_PASS);
    }
    else {
        System_printf("Error: HWA Instance %p closed with err:%d\n", handle,errCode);
        MCPI_setFeatureTestResult("API HWA_close()", MCPI_TestResult_FAIL);
        return;
    }

    /**************************************************************************
     * Test: Reopen the driver
     **************************************************************************/
    handle = HWA_open(0, gSOCHandle, &errCode);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the HWA Instance err:%d\n",errCode);
        MCPI_setFeatureTestResult("HWA reopen", MCPI_TestResult_FAIL);
        return;
    }
    System_printf("Debug: HWA Instance %p has been reopened successfully\n", handle);



    /**************************************************************************
     **************************************************************************
     * Test: DMA triggered mode
     **************************************************************************
     **************************************************************************/
    System_printf("Debug: HWA Tests start for DMA triggered mode \n");
    /* Do dma trigger mode test - simple FFT - one pass */
    paramsetIdx = 0;
    memset(gHWATestParamConfig,0,sizeof(gHWATestParamConfig));
    /**************************************************************************
     * Step 1 - config the paramset
     **************************************************************************/
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA; //Software triggered  - in demo this will be HWA_TRIG_MODE_DMA
    gHWATestParamConfig[paramsetIdx].dmaTriggerSrc = HWA_TEST_SRC_TRIGGER_DMACH0; //in demo this will be first EDMA Src channel id
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_NONE; //dummy
    errCode = HWA_configParamSet(handle,paramsetIdx,&gHWATestParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        System_printf("Error: HWA_configParamSet(%d) (DMA Trigger paramset for ping buffer, AccelMode None) returned %d\n",errCode,paramsetIdx);
        MCPI_setFeatureTestResult("API HWA_configParamSet() [DMA Trigger paramset for ping buffer, AccelMode None]", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_configParamSet() [DMA Trigger paramset for ping buffer, AccelMode None]", MCPI_TestResult_PASS);
    }
    paramsetIdx++;
    pingParamSetIdx = paramsetIdx;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE; //Immediate following first - in demo this should be HWA_TRIG_MODE_DFE
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_FFT; //do FFT
    gHWATestParamConfig[paramsetIdx].source.srcAddr = (uint16_t)((uint32_t)srcAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].source.srcAcnt = HWA_TEST_NUM_SAMPLES-1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].source.srcAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].source.srcBcnt = HWA_TEST_NUM_RX_ANT-1; //no iterations here
    gHWATestParamConfig[paramsetIdx].source.srcBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].source.srcShift = 0; //no shift
    gHWATestParamConfig[paramsetIdx].source.srcCircShiftWrap = 0; //no shift
    gHWATestParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
    gHWATestParamConfig[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
    gHWATestParamConfig[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; //signed
    gHWATestParamConfig[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].source.srcScale = 8;
    gHWATestParamConfig[paramsetIdx].source.bpmEnable = HWA_FEATURE_BIT_DISABLE; //bpm removal not enabled
    gHWATestParamConfig[paramsetIdx].source.bpmPhase = 0; //dont care
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint16_t)((uint32_t)dstAddr - SOC_HWA_MEM0); // address is relative to start of MEM0
    gHWATestParamConfig[paramsetIdx].dest.dstAcnt = HWA_TEST_1DFFT_SIZE-1; //this is samples - 1
    gHWATestParamConfig[paramsetIdx].dest.dstAIdx = HWA_TEST_NUM_RX_ANT * HWA_TEST_COMPLEX_16BIT_SIZE; // 16 bytes
    gHWATestParamConfig[paramsetIdx].dest.dstBIdx = HWA_TEST_COMPLEX_16BIT_SIZE; //should be dont care
    gHWATestParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
    gHWATestParamConfig[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
    gHWATestParamConfig[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
    gHWATestParamConfig[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE; //no conjugate
    gHWATestParamConfig[paramsetIdx].dest.dstScale = 0;
    gHWATestParamConfig[paramsetIdx].dest.dstSkipInit = 0; // no skipping
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = log2Approx(HWA_TEST_1DFFT_SIZE);
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x3; //LSB fftSize bits are relevant - revisit this for all FFT size and data size
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.interfZeroOutEn = HWA_FEATURE_BIT_DISABLE; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE; //enabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = HWA_TEST_1DFFT_WINDOW_START; //start of window RAM
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = HWA_FFT_WINDOW_NONSYMMETRIC; //non-symmetric - in demo do we make this symmetric
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.winInterpolateMode = HWA_FFT_WINDOW_INTERPOLATE_MODE_NONE; //fftsize is less than 1K
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; //disabled
    gHWATestParamConfig[paramsetIdx].accelModeArgs.fftMode.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; // output FFT samples
    gHWATestParamConfig[paramsetIdx].complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    errCode = HWA_configParamSet(handle,paramsetIdx,&gHWATestParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        System_printf("Error: HWA_configParamSet(%d) returned %d\n",errCode,paramsetIdx);
        MCPI_setFeatureTestResult("API HWA_configParamSet() [Immediate Triggered, Ping buffer FFT processing]", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_configParamSet() [Immediate Triggered, Ping buffer FFT processing]", MCPI_TestResult_PASS);
    }
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA; //Immediate triggered  - in demo this will be HWA_TRIG_MODE_DMA
    gHWATestParamConfig[paramsetIdx].dmaTriggerSrc = HWA_TEST_SRC_TRIGGER_DMACH1; //in demo this will be second EDMA Src channel id
    gHWATestParamConfig[paramsetIdx].accelMode = HWA_ACCELMODE_NONE; //dummy
    errCode = HWA_configParamSet(handle,paramsetIdx,&gHWATestParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        System_printf("Error: HWA_configParamSet(%d) returned %d\n",errCode,paramsetIdx);
        MCPI_setFeatureTestResult("API HWA_configParamSet() [DMA Trigger paramset for pong buffer, AccelMode None]", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_configParamSet() [DMA Trigger paramset for pong buffer, AccelMode None]", MCPI_TestResult_PASS);
    }
    paramsetIdx++;
    gHWATestParamConfig[paramsetIdx] = gHWATestParamConfig[pingParamSetIdx];
    gHWATestParamConfig[paramsetIdx].dest.dstAddr = (uint16_t)((uint32_t)dstAddr + HWA_MEMn_SIZE - SOC_HWA_MEM0); // address is relative to start of MEM0
    errCode = HWA_configParamSet(handle,paramsetIdx,&gHWATestParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        System_printf("Error: HWA_configParamSet(%d) returned %d\n",errCode,paramsetIdx);
        MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, Pong buffer FFT processing)", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_configParamSet() (Immediate Triggered, Pong buffer FFT processing)", MCPI_TestResult_PASS);
    }

    /**************************************************************************
     * Step 2 - load window coefficients
     **************************************************************************/
    /* if windowing is enabled, load the window coefficients in RAM */
    errCode = HWA_configRam(handle,HWA_RAM_TYPE_WINDOW_RAM,(uint8_t *)win_data225,sizeof(win_data225),HWA_TEST_1DFFT_WINDOW_START);
    if (errCode != 0)
    {
        System_printf("Error: HWA_configRam() returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_configRam()", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_configRam()", MCPI_TestResult_PASS);
    }

#if 1
    /* Enable Paramset Interrupt for CPU and create semaphore*/
    SemaphoreP_Params_init(&semParams);
    semParams.mode             = SemaphoreP_Mode_BINARY;
    paramSetSem = SemaphoreP_create(0, &semParams);
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU;
    paramISRConfig.cpu.callbackFn = HWA_Test_ParamSetISR_Callback;
    paramISRConfig.cpu.callbackArg = paramSetSem;
    errCode = HWA_enableParamSetInterrupt(handle,paramsetIdx,&paramISRConfig);
    if (errCode != 0)
    {
        //retCode = HWA_TEST_ERROR;
        System_printf("Error: HWA_enableParamSetInterrupt returned %d\n",errCode);
        MCPI_setFeatureTestResult("HWA_enableParamSetInterrupt", MCPI_TestResult_FAIL);
        return;
    }
#endif

    /**************************************************************************
     * Step 3 - Enable Interrupts
     **************************************************************************/
    /* Enable Done Interrut */
    SemaphoreP_Params_init(&semParams);
    semParams.mode             = SemaphoreP_Mode_BINARY;
    doneSem = SemaphoreP_create(0, &semParams);
    errCode = HWA_enableDoneInterrupt(handle,HWA_Test_DoneISR_Callback,doneSem);
    if (errCode != 0)
    {
        System_printf("Error: HWA_enableDoneInterrupt returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_enableDoneInterrupt()", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_enableDoneInterrupt()", MCPI_TestResult_PASS);
    }

    /**************************************************************************
     * Step 4 - Config Common Registers
     **************************************************************************/
    gCommonConfig.configMask = HWA_COMMONCONFIG_MASK_NUMLOOPS|
                               HWA_COMMONCONFIG_MASK_PARAMSTARTIDX|
                               HWA_COMMONCONFIG_MASK_PARAMSTOPIDX|
                               HWA_COMMONCONFIG_MASK_FFT1DENABLE|
                               HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD;
    gCommonConfig.numLoops = 1; //do only one iteration
    gCommonConfig.paramStartIdx = 0; 
    gCommonConfig.paramStopIdx = 3;
    gCommonConfig.fftConfig.fft1DEnable = HWA_FEATURE_BIT_DISABLE; //disable this for test vector type of testing so that MEM0/MEM1 are separate memories
    gCommonConfig.fftConfig.interferenceThreshold = 0xFFFFFF;
    errCode = HWA_configCommon(handle,&gCommonConfig);
    if (errCode != 0)
    {
        System_printf("Error: HWA_configCommon returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_configCommon()", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_configCommon()", MCPI_TestResult_PASS);
    }

    /**************************************************************************
     * Step 5 - Enable the HWA
     **************************************************************************/
    errCode = HWA_enable(handle,1); // set 1 to enable
    if (errCode != 0)
    {
        System_printf("Error: HWA_enable(1) returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_enable() to enable HWA", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_enable() to enable HWA", MCPI_TestResult_PASS);
    }
    /**************************************************************************
     * Step 6 - Reset the HWA state machine
     **************************************************************************/
    errCode = HWA_reset(handle);
    if (errCode != 0)
    {
        System_printf("Error: HWA_reset returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_reset()", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_reset()", MCPI_TestResult_PASS);
    }

    /**************************************************************************
     * Prepare the input data for test vector based testing
     **************************************************************************/
    /* memcopy the data to MEM0 */
    memcpy(srcAddr,(uint8_t *)gHWATest_1DFFT_input,
           HWA_TEST_NUM_SAMPLES*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    memset((uint8_t *)dstAddr, 0, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    memset((uint8_t *)((uint32_t)dstAddr+ HWA_MEMn_SIZE), 0, HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);

#if 0
    /* trigger the HWA manually using HWA_setSoftwareTrigger if triggerMode is set to Software */
    errCode = HWA_setSoftwareTrigger(handle);

    if (errCode != 0)
    {
        //retCode = HWA_TEST_ERROR;
        System_printf("Error: HWA_setSoftwareTrigger returned %d\n",errCode);
        MCPI_setFeatureTestResult("HWA_setSoftwareTrigger", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_setSoftwareTrigger", MCPI_TestResult_PASS);
    }
    
#else
    /**************************************************************************
     * Step 7: trigger HWA
     **************************************************************************/
    /* trigger the HWA using HWA_setDMA2ACCManualTrig if triggerMode is set to DMA */
    errCode = HWA_setDMA2ACCManualTrig(handle,HWA_TEST_SRC_TRIGGER_DMACH0);
    if (errCode != 0)
    {
        System_printf("Error: HWA_setDMA2ACCManualTrig(0) returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_setDMA2ACCManualTrig(ch:0)", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_setDMA2ACCManualTrig(ch:0)", MCPI_TestResult_PASS);
    }
    errCode = HWA_setDMA2ACCManualTrig(handle,HWA_TEST_SRC_TRIGGER_DMACH1);
    if (errCode != 0)
    {
        System_printf("Error: HWA_setDMA2ACCManualTrig(1) returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_setDMA2ACCManualTrig(ch:1)", MCPI_TestResult_FAIL);
        return;
    }
    else
    {
        MCPI_setFeatureTestResult("API HWA_setDMA2ACCManualTrig(ch:1)", MCPI_TestResult_PASS);
    }
#endif

    /**************************************************************************
     * Step 7: Wait for completion
     **************************************************************************/
    /* now wait for the interrupts */
#if 1
    /* first wait for the paramSet done interrupt */
    status = SemaphoreP_pend(paramSetSem,BIOS_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        //retCode = HWA_TEST_ERROR;
        System_printf("Error: SemaphoreP_pend returned %d\n",status);
        MCPI_setFeatureTestResult("SemaphoreP_pend", MCPI_TestResult_FAIL);
    }
#endif
    /* then wait for the all paramSets done interrupt */
    status = SemaphoreP_pend(doneSem,BIOS_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        System_printf("Error: Wait for all paramsets done failed %d\n",status);
        MCPI_setFeatureTestResult("Wait for all paramsets done:", MCPI_TestResult_FAIL);
        return;
    }

    /**************************************************************************
     * Check output data
     **************************************************************************/
    /* output data is ready - compare */
    compareCode1= memcmp(dstAddr,(uint8_t *)gHWATest_1DFFT_output,
            HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    errCode = compareCode1;
    if (compareCode1 != 0)
    {
        System_printf("Error: output produced by HWA in MEM2 found incorrect: error %d\n",compareCode1);
    }
    compareCode2 = memcmp((uint8_t *)((uint32_t)dstAddr+ HWA_MEMn_SIZE),(uint8_t *)gHWATest_1DFFT_output,
            HWA_TEST_1DFFT_SIZE*HWA_TEST_NUM_RX_ANT*HWA_TEST_COMPLEX_16BIT_SIZE);
    errCode += compareCode2;
    if (compareCode2 != 0)
    {
        System_printf("Error: output produced by HWA in MEM3 found incorrect: error %d\n",compareCode2);
    }
    if (errCode != 0)
    {
        MCPI_setFeatureTestResult("DMA triggered HWA Tests", MCPI_TestResult_FAIL);
    } else {
        MCPI_setFeatureTestResult("DMA triggered HWA Tests", MCPI_TestResult_PASS);
    }

    //HWA_readStatsReg(handle,appHWAStats,3);

    /**************************************************************************
     * Cleanup
     **************************************************************************/
    errCode = HWA_disableParamSetInterrupt(handle,paramsetIdx,HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
    if (errCode != 0)
    {
        System_printf("Error: HWA_disableParamSetInterrupt returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_disableParamSetInterrupt() ", MCPI_TestResult_FAIL);
    } else {
        MCPI_setFeatureTestResult("API HWA_disableParamSetInterrupt() ", MCPI_TestResult_PASS);
    }
    errCode = HWA_disableDoneInterrupt(handle);
    if (errCode != 0)
    {
        System_printf("Error: HWA_disableDoneInterrupt returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_disableDoneInterrupt() ", MCPI_TestResult_FAIL);
    } else {
        MCPI_setFeatureTestResult("API HWA_disableDoneInterrupt() ", MCPI_TestResult_PASS);
    }
    /* disable HWA*/
    errCode = HWA_enable(handle,0); // set 0 to disable
    if (errCode != 0)
    {
        System_printf("Error: HWA_enable(0) returned %d\n",errCode);
        MCPI_setFeatureTestResult("API HWA_enable() to disable HWA", MCPI_TestResult_FAIL);
        return;
    } else {
        MCPI_setFeatureTestResult("API HWA_enable() to disable HWA", MCPI_TestResult_PASS);
    }
#if 0
    /* cleanup semaphores */
    if (paramSetSem!=NULL)
        SemaphoreP_delete(paramSetSem);
#endif
    if (doneSem!=NULL)
        SemaphoreP_delete(doneSem);

    /* Close the driver: */
    errCode = HWA_close(handle);
    if (errCode != 0) {
        System_printf("Error: HWA_close() failed with err:%d\n",errCode);
        MCPI_setFeatureTestResult("HWA_close()", MCPI_TestResult_FAIL);
    } else {
        System_printf("Debug: HWA Tests passed successfully\n");
    }

    MCPI_setTestResult ();
    
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
    int32_t         errCode;
    SOC_Cfg         socCfg;

#ifdef SYSTEM_MSS
    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);
#endif

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    gSOCHandle = SOC_init (&socCfg, &errCode);
    if (gSOCHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

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

