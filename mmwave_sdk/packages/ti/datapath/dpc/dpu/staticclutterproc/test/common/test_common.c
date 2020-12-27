/**
 *   @file  test_common.c
 *
 *   @brief
 *      Common APIs used on unit uest code for static clutter removal.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
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
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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
#include <ti/sysbios/knl/Event.h>
#define DebugP_ASSERT_ENABLED 1
#include <ti/drivers/osal/DebugP.h>
#include <assert.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/utils/cycleprofiler/cycle_profiler.h> 


#include <ti/datapath/dpc/dpu/staticclutterproc/staticclutterproc.h>

#include "test_common.h"

 /** @mainpage Static Clutter Removal DPU unit test
 *
 *  This is the unit test for the Static Clutter Removal DPU.
 *  The test performs a reference clutter removal implementation and compares against the 
 *  EDMA based optimized DPU static clutter removal implementation.
 *  Several scenarios are tested, varying the number of RX and TX antennas, 
 *  number of Doppler bins and number of range bins.\n
 *
 *  Suppose the available L3 memory is M bytes. M/2 bytes is used for the reference
 *  clutter removal and M/2 bytes is used for the DPU static clutter removal implementation.
 *  For each scenario the algorithm output is compared and must be bit-exact, otherwise failure 
 *  is declared.
 */


/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

void Test_fillRadarCube(uint8_t* radarCubeRefPtr, uint8_t* radarCubeTestPtr, uint32_t radarCubeSizeInBytes)
{
    uint32_t i;
    uint32_t *ref;
    uint32_t *out;

    //printf("radarCubeSizeInBytes %d radarCubeRefPtr 0x%x radarCubeTestPtr 0x%x\n",radarCubeSizeInBytes, (unsigned int)radarCubeRefPtr, (unsigned int)radarCubeTestPtr);

    /*First half of L3 is the reference cube. Second half is the EDMA based clutter removal.
      They are both initialized identically so that after clutter removal both should be identical.*/    
    ref = (uint32_t*)radarCubeRefPtr; 
    out = (uint32_t*)radarCubeTestPtr;
    
    /* set the seed value*/
    ref[0] = out[0] = 0x12345678;
    
    /*This sequence of integers has a period long enough to not repeat within the data cube.
      This ensures that if test passes it is not due to repetition in the cube data pattern
      which may mask some computational mistake.*/    
    for(i = 1; i < radarCubeSizeInBytes/sizeof(uint32_t); i++)
    {
        ref[i] = out[i] = (3*ref[i-1] + 5);
    }
}

static inline int16_t Test_saturateSubtract(int16_t a, int16_t b)
{
    int32_t tempSub = a-b;
    if((tempSub >= -32768) && (tempSub <= 32767))
        return (int16_t) tempSub;
    else if (tempSub > 32767)   
        return 32767;
    else
        return -32768;
}

/* Computes reference static clutter removal algorithm*/
void Test_referenceClutterAlgorithm(uint8_t* radarCubeRefPtr, testConfiguration* testCfg)
{
    int32_t txIdx, adx, dopIdx;
    cmplx16ImRe_t *inPtr = (cmplx16ImRe_t *) radarCubeRefPtr;
    cmplx32ImRe_t meanVal;
    uint32_t midx;
    
    for (txIdx = 0; txIdx < testCfg->numTxAnt; txIdx++)
    {
        for (adx = 0; adx < testCfg->numRxAnt*testCfg->numRanBin; adx++)
        {
                meanVal.real = 0;
                meanVal.imag = 0;
                for (dopIdx = 0; dopIdx < testCfg->numDopChirps; dopIdx++)
                {
                    midx = txIdx*testCfg->numDopChirps*testCfg->numRanBin*testCfg->numRxAnt + 
                           adx + dopIdx*testCfg->numRanBin*testCfg->numRxAnt;                        
                    meanVal.real += inPtr[midx].real;
                    meanVal.imag += inPtr[midx].imag;
                }
                
                meanVal.real = meanVal.real/testCfg->numDopChirps;
                meanVal.imag = meanVal.imag/testCfg->numDopChirps;
                
                /* subtraction */                
                for (dopIdx = 0; dopIdx < testCfg->numDopChirps; dopIdx++)
                {
                    midx = txIdx*testCfg->numDopChirps*testCfg->numRanBin*testCfg->numRxAnt + 
                           adx + dopIdx*testCfg->numRanBin*testCfg->numRxAnt;                        
                    /* Always saturate in the subtraction step. DSP uses mmwavelib which saturates.*/
                    inPtr[midx].real = Test_saturateSubtract(inPtr[midx].real, meanVal.real);
                    inPtr[midx].imag = Test_saturateSubtract(inPtr[midx].imag, meanVal.imag);
                }
        }
    }
}


EDMA_Handle Test_edmaOpen(uint8_t instanceId)
{
    int32_t             errCode;
    EDMA_instanceInfo_t edmaInstanceInfo;
    EDMA_errorConfig_t  errorConfig;
    EDMA_Handle         edmaHandle = NULL;

    edmaHandle = EDMA_open(instanceId, &errCode, &edmaInstanceInfo);

    if (edmaHandle == NULL)
    {
        printf("Error: Unable to open the EDMA Instance err:%d\n",errCode);
        return NULL;
    }
    printf("Debug: EDMA Instance %p has been opened successfully\n", edmaHandle);

    errorConfig.isConfigAllEventQueues = true;
    errorConfig.isConfigAllTransferControllers = true;
    errorConfig.isEventQueueThresholdingEnabled = true;
    errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    errorConfig.callbackFxn = NULL;
    errorConfig.transferControllerCallbackFxn = NULL;
    if ((errCode = EDMA_configErrorMonitoring(edmaHandle, &errorConfig)) != EDMA_NO_ERROR)
    {
        printf("Error: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errCode);
        return NULL;
    }
    
    return edmaHandle;
}

int32_t Test_verifyResult(uint8_t* radarCubeRefPtr, uint8_t* radarCubeTestPtr, uint32_t radarCubeSizeInBytes)
{
    uint32_t i;

    /*First half of L3 is the reference cube. Second half is the EDMA based clutter removal.
      They are both initialized identically so that after clutter removal both should be identical.*/    
    for(i = 0; i < radarCubeSizeInBytes; i++)
    {
        if(radarCubeRefPtr[i] != radarCubeTestPtr[i])
        {
            printf("Verification failed. Byte %d. Reference value is 0x%x. Clutter DPU value 0x%x.\n",
                   i, radarCubeRefPtr[i], radarCubeTestPtr[i]);
            return MINUS_ONE;       
        }        
    }
    
    return 0;
}

/* Test main loop*/
void Test_main
(
    uint8_t                        numOfScenarios, 
    testConfiguration              *testConfigArray,
    DPU_StaticClutterProc_Config   *initialCfg,
    uint8_t                        EdmaInstanceId,
    uint8_t                        *radarCubeRefPtr,
    uint8_t                        *radarCubeTestPtr,
    uint32_t                       radarCubeSizeInBytes
 )
{
    DPU_StaticClutterProc_Config    cfgParams;
    DPU_StaticClutterProc_Handle    staticClutterHandle = NULL;
    int32_t                         errCode;
    EDMA_Handle                     edmaHandle = NULL;
    uint32_t                        cubeSize;
    uint32_t                        startTime, bench1, bench2;
    uint8_t                         testNum;
    uint8_t                         failFlag = 0;
    testConfiguration               testCfg;
    uint8_t                         edmaNumInstances, inst;
    DPU_StaticClutterProc_OutParams outParams;

    printf("Test started. Will run %d tests\n",numOfScenarios);

    /* Initialize the EDMA */
    edmaNumInstances = EDMA_getNumInstances();
    for (inst = 0; inst < edmaNumInstances; inst++)
    {
        errCode = EDMA_init(inst);
        if (errCode != EDMA_NO_ERROR)
        {
            System_printf ("Debug: EDMA instance %d initialization returned error %d\n", errCode);
            return;
        }
        //System_printf ("Debug: EDMA instance %d has been initialized\n", inst);
    }
    
    /* Initialize clutter removal module*/
    staticClutterHandle = DPU_StaticClutterProc_init(&errCode);
    if(staticClutterHandle == NULL)
    {
        printf("staticClutter DPU failed to initialize. Error %d.\n",errCode);
        MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_FAIL);
        return;
    }
    printf("staticClutterHandle = 0x%x\n",(uint32_t)staticClutterHandle);
    
    /* Open EDMA instance to be used by test*/
    edmaHandle = Test_edmaOpen(EdmaInstanceId);
    if(edmaHandle == NULL)
    {
        printf("EDMA failed to open\n");
        MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_FAIL);
        return;
    }
    
    /*Fill in configuration parameters that are common for all test case*/
    memcpy((void*)&cfgParams, (void*)initialCfg, sizeof(DPU_StaticClutterProc_Config));    
    cfgParams.hwRes.edmaHandle   = edmaHandle;

    for(testNum = 0; testNum < numOfScenarios; testNum++)
    {
        printf("\nTest number %d\n",testNum);
        memcpy((void*)&testCfg, (void*)&testConfigArray[testNum], sizeof(testConfiguration));

        cubeSize = sizeof(cmplx16ImRe_t)*testCfg.numTxAnt*testCfg.numRxAnt*testCfg.numDopChirps*testCfg.numRanBin;
        
        /* Check if dimensions are ok */
        if(cubeSize > radarCubeSizeInBytes)
        {
            printf("Test failed because radarCubeSizeInBytes is not big enough for the test params.\n");
            printf("Test cube size = %d > Maximum cube size = %d\n", cubeSize, radarCubeSizeInBytes);
            MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_FAIL);
            return;
        }
        else
        {
            printf("Test params:\nNumTxAnt=%d\nNumRxAnt=%d\nnumDopChirps=%d\nNumRanBin=%d\n",
                   testCfg.numTxAnt,testCfg.numRxAnt,testCfg.numDopChirps,testCfg.numRanBin);
            printf("Test cube size = %d. Maximum cube size = %d\n", cubeSize, radarCubeSizeInBytes);
        }
        
        /* Initialize the cube data*/
        Test_fillRadarCube(radarCubeRefPtr, radarCubeTestPtr, radarCubeSizeInBytes);
              
        /* Compute reference clutter removal */
        startTime = Cycleprofiler_getTimeStamp();
        Test_referenceClutterAlgorithm(radarCubeRefPtr, &testCfg);
        bench1 = Cycleprofiler_getTimeStamp() - startTime;
        printf("Reference clutter removal done\n");
                           
        /* Configure DPU parameters that vary per test case*/
        cfgParams.staticCfg.numTxAntennas    = testCfg.numTxAnt;
        cfgParams.staticCfg.numRxAntennas    = testCfg.numRxAnt;
        cfgParams.staticCfg.numRangeBins     = testCfg.numRanBin;
        cfgParams.staticCfg.numDopplerChirps = testCfg.numDopChirps;
        
        errCode = DPU_StaticClutterProc_config(staticClutterHandle, &cfgParams); 
        if(errCode != 0)
        {
            printf("staticClutter DPU failed to configure. Error %d.\n",errCode);
            MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_FAIL);
            return;
        }
        
        /* Run the static clutter DPU */
        startTime = Cycleprofiler_getTimeStamp();
        
        errCode = DPU_StaticClutterProc_process(staticClutterHandle, &outParams);
        if(errCode != 0)
        {
            printf("staticClutter DPU failed to run. Error %d.\n",errCode);
            MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_FAIL);
            return;
        }
        bench2 = Cycleprofiler_getTimeStamp() - startTime;
    
        printf("Reference benchmark %d. DPU benchmark %d. Gain %.1f\n", bench1, bench2, (float)bench1/bench2);
        
        /* Check the results */
        if(Test_verifyResult(radarCubeRefPtr, radarCubeTestPtr, radarCubeSizeInBytes) == 0)
        {
            printf("Test PASSED!\n");
        }
        else
        {
            printf("Test FAILED!\n");
            failFlag = 1;
        }
    }
    
    errCode = DPU_StaticClutterProc_deinit(staticClutterHandle);
    if(errCode != 0)
    {
        printf("staticClutter DPU failed to deinit. Error %d.\n",errCode);
        MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_FAIL);
        return;
    }

    printf("----------------------------\n\n");
    if(failFlag == 0)
    {
        printf("All Tests PASSED!\n");
        MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_PASS);
    }
    else
    {
        printf("Test FAILED!\n");
        MCPI_setFeatureTestResult("static_clutter_DPU", MCPI_TestResult_FAIL);
    }
    
    printf("Test Finished\n");
    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);
}

