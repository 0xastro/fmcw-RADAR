/**
 *   @file  dopplerprochwa_test_mss.c
 *
 *   @brief
 *      Unit Test code for doppler processing.
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
#include <ti/datapath/dpc/dpu/dopplerproc/dopplerprochwa.h>
#include <ti/utils/mathutils/mathutils.h>

 
 /* EDMA resource partitioning */
#define EDMA_SHADOW_LNK_PARAM_BASE_ID       EDMA_NUM_DMA_CHANNELS
/* EDMA channels */
#define TEST_EDMA_DOPPLERPROC_IN_PING         EDMA_TPCC0_REQ_FREE_4
#define TEST_EDMA_DOPPLERPROC_IN_PONG         EDMA_TPCC0_REQ_FREE_5

/*This has to match the HWA DMA number*/
#define TEST_EDMA_DOPPLERPROC_OUT_PING        EDMA_TPCC0_REQ_HWACC_2

/*This has to match the HWA DMA number*/
#define TEST_EDMA_DOPPLERPROC_OUT_PONG        EDMA_TPCC0_REQ_HWACC_3

#define TEST_EDMA_DOPPLERPROC_HOTSIG_PING     EDMA_TPCC0_REQ_FREE_8
#define TEST_EDMA_DOPPLERPROC_HOTSIG_PONG     EDMA_TPCC0_REQ_FREE_9

/*EDMA shadow channels*/        
#define TEST_EDMA_DOPPLERPROC_IN_PING_SHADOW     (EDMA_SHADOW_LNK_PARAM_BASE_ID + 4U)
#define TEST_EDMA_DOPPLERPROC_IN_PONG_SHADOW     (EDMA_SHADOW_LNK_PARAM_BASE_ID + 5U)
#define TEST_EDMA_DOPPLERPROC_OUT_PING_SHADOW    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 6U)
#define TEST_EDMA_DOPPLERPROC_OUT_PONG_SHADOW    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 7U)
#define TEST_EDMA_DOPPLERPROC_HOTSIG_PING_SHADOW (EDMA_SHADOW_LNK_PARAM_BASE_ID + 8U)
#define TEST_EDMA_DOPPLERPROC_HOTSIG_PONG_SHADOW (EDMA_SHADOW_LNK_PARAM_BASE_ID + 9U)



#define TEST_MAX_NUM_DOPPLER_BINS 512
#define TEST_MAX_NUM_RANGE_BINS   1024


#if (defined (SOC_XWR68XX))
#define TEST_MAX_RADAR_CUBE_SIZE  (42 * 1024 * 4)
#endif

#if (defined (SOC_XWR18XX))
//1 Tx, 1, 2, 4 rx, the maximum size is 1024(range)*32(doppler)
//2 Tx, 2 rx, the maximum size is 1024(range)*32(doppler) , 4 rx, the maximum size is 1024(range)*16(doppler)
//3 tx, 4 rx, the maximum size is 1024(range)*16 (doppler)
#define TEST_MAX_RADAR_CUBE_SIZE  (48 * 1024 * 4)
#endif


/*cube */
cmplx16ImRe_t testRadarCube[TEST_MAX_RADAR_CUBE_SIZE];       //msb is real, lsb 16 is imag
#pragma DATA_SECTION(testRadarCube, ".l3ram");

/*detection matrix*/
#define TEST_MAX_DETMATRIX_SIZE    (32 * 1024)

#pragma DATA_SECTION(calculatedDetMatrix, ".l3ram");
int16_t calculatedDetMatrix[TEST_MAX_DETMATRIX_SIZE]; //log2(abs) sum over all tx and rx antenna

#ifdef SUBSYS_MSS
#pragma DATA_SECTION(testDopplerWindow, ".tcmb");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(testDopplerWindow, ".l2data");
#endif
int32_t testDopplerWindow[TEST_MAX_NUM_DOPPLER_BINS];
#ifdef SUBSYS_MSS
#pragma DATA_SECTION(fftDataInBuf, ".tcmb");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(fftDataInBuf, ".l2data");
#endif
uint32_t fftDataInBuf[TEST_MAX_NUM_DOPPLER_BINS];

#ifdef SUBSYS_MSS
#pragma DATA_SECTION(fftDataWindow, ".tcmb");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(fftDataWindow, ".l2data");
#endif
int32_t fftDataWindow[TEST_MAX_NUM_DOPPLER_BINS * 2];

#ifdef SUBSYS_MSS
#pragma DATA_SECTION(fftDataOutBuf, ".tcmb");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(fftDataOutBuf, ".l2data");
#endif
int32_t fftDataOutBuf[TEST_MAX_NUM_DOPPLER_BINS * 2];

#ifdef SUBSYS_MSS
#pragma DATA_SECTION(detMatRefTemp, ".tcmb");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(detMatRefTemp, ".l2data");
#endif
int32_t detMatRefTemp[TEST_MAX_NUM_DOPPLER_BINS];  /* save the reference   */

#ifdef SUBSYS_MSS
#pragma DATA_SECTION(detMatRef, ".tcmb");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(detMatRef, ".l2data");
#endif
uint16_t detMatRef[TEST_MAX_NUM_DOPPLER_BINS];  /* shift it into 16 bits */


#define PI 3.141592653589793
#define MAX_ERROR 26.f         //the output is Q9 format, 


 /*
 * Generates a random number 0 - 0x7fff
 */
uint16_t RandSimple(void)
{
    static int64_t next = 1;
    uint16_t ret2;

    next = (next * 16807) % 2147483647;
    ret2 = (uint16_t)(next & 0x7fff);

    return (ret2);
}

/*
 *  Generates Gausian distributed random number, with unit variance
*/
void GaussianNoise(
    float *NoiseVariance,
    float *pxn,
    float *pyn)
{

    float y, dx2, pi2, r, t, temp;
    static float pi = (float)3.1415926535897;
    uint16_t itemp1;
    float gauss_var;

    pi2 = 2.f * pi;
    gauss_var = *NoiseVariance / 2; /* half per dimension */

    itemp1 = RandSimple();
    while (itemp1 == 0)
        itemp1 = RandSimple();
    r = (float)(itemp1) / 32767.0f;

    temp = 2.f*gauss_var*(float)log(1. / r);
    y = (float)sqrt(temp);

    itemp1 = RandSimple();
    t = (float)(itemp1) / 32767.0f;

    dx2 = pi2*t;
    *pxn = y*(float)cos(dx2);
    *pyn = y*(float)sin(dx2);
}



typedef struct dopplerProcDpuTestConfig_t {

    uint32_t numTxAntennas;
    uint32_t numRxAntennas;
    uint32_t numRangeBins;
    uint32_t numDopplerBins;
    uint32_t numChirps; //numChirps * numTxAnt = numChirps per frame
    uint32_t winSymFlag; //0 - symmetric , 1 -non-symmetric 
    uint32_t winType;   //0 - hanning window, 1 - rec window

}dopplerProcDpuTestConfig_t;

SOC_Handle  socHandle;
EDMA_Handle edmaHandle;
HWA_Handle  hwaHandle;

/* test configuration */
DPU_DopplerProcHWA_Config    dopplerProcDpuCfg;
DPU_DopplerProcHWA_Handle    dopplerProcDpuHandle;

MCPI_LOGBUF_INIT(9216);

uint8_t finalResults;
uint8_t checkResults;


void MmwDemo_EDMA_errorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

void MmwDemo_EDMA_transferControllerErrorCallbackFxn(EDMA_Handle handle,
    EDMA_transferControllerErrorInfo_t *errorInfo)
{
    checkResults = 0; //dpu crashes
   // DebugP_assert(0);
}

void dopplerProc_test_hwaEdmainit()
{
    uint8_t             edmaNumInstances;
    uint8_t             inst;
    EDMA_instanceInfo_t edmaInstanceInfo;
    EDMA_errorConfig_t  errorConfig;
    int32_t             errorCode;
    EDMA_errorInfo_t  EDMA_errorInfo;
    EDMA_transferControllerErrorInfo_t EDMA_transferControllerErrorInfo;


    /* Initialize the HWA */
    HWA_init();

    hwaHandle = HWA_open(0, socHandle, &errorCode);
    if (hwaHandle == NULL)
    {
        System_printf("HWA failed to open\n");
        DebugP_assert(0);
        return;
    }

    /* Initialize the EDMA */
    edmaNumInstances = EDMA_getNumInstances();
    for (inst = 0; inst < edmaNumInstances; inst++)
    {
        errorCode = EDMA_init(inst);
        if (errorCode != EDMA_NO_ERROR)
        {
            System_printf("Debug: EDMA instance %d initialization returned error %d\n", errorCode);
            DebugP_assert(0);
            return;
        }
    }
    memset(&EDMA_errorInfo, 0, sizeof(EDMA_errorInfo));
    memset(&EDMA_transferControllerErrorInfo, 0, sizeof(EDMA_transferControllerErrorInfo));

    edmaHandle = EDMA_open(0, &errorCode, &edmaInstanceInfo);
    if (edmaHandle == NULL)
    {
        System_printf("Error: Unable to open the EDMA Instance err:%d\n", errorCode);
        DebugP_assert(0);
        return;
    }

    errorConfig.isConfigAllEventQueues = true;
    errorConfig.isConfigAllTransferControllers = true;
    errorConfig.isEventQueueThresholdingEnabled = true;
    errorConfig.eventQueueThreshold = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    errorConfig.callbackFxn = MmwDemo_EDMA_errorCallbackFxn;
    errorConfig.transferControllerCallbackFxn = MmwDemo_EDMA_transferControllerErrorCallbackFxn;
    if ((errorCode = EDMA_configErrorMonitoring(edmaHandle, &errorConfig)) != EDMA_NO_ERROR)
    {
        System_printf("Error: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errorCode);
        DebugP_assert(0);
        return;
    }


    

}

void dopplerProc_test_dpuInit()
{
    int32_t errorCode;
    DPU_DopplerProcHWA_InitParams   initCfg;

    initCfg.hwaHandle = hwaHandle;
    dopplerProcDpuHandle = DPU_DopplerProcHWA_init(&initCfg, &errorCode);
    if (dopplerProcDpuHandle == NULL)
    {
        System_printf("doppler DPU failed to initialize. Error %d.\n", errorCode);
        return;
    }
}

void dopplerProc_test_dpuConfig()
{
    memset((void*)&dopplerProcDpuCfg, 0, sizeof(DPU_DopplerProcHWA_Config));

    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHandle = edmaHandle;

    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.ping.channel = TEST_EDMA_DOPPLERPROC_IN_PING;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.ping.channelShadow = TEST_EDMA_DOPPLERPROC_IN_PING_SHADOW;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.ping.eventQueue = 0;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pong.channel = TEST_EDMA_DOPPLERPROC_IN_PONG;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pong.channelShadow = TEST_EDMA_DOPPLERPROC_IN_PONG_SHADOW;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pong.eventQueue = 0;

    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.ping.channel = TEST_EDMA_DOPPLERPROC_OUT_PING;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.ping.channelShadow = TEST_EDMA_DOPPLERPROC_OUT_PING_SHADOW;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.ping.eventQueue = 0;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pong.channel = TEST_EDMA_DOPPLERPROC_OUT_PONG;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pong.channelShadow = TEST_EDMA_DOPPLERPROC_OUT_PONG_SHADOW;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pong.eventQueue = 0;

    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.ping.channel = TEST_EDMA_DOPPLERPROC_HOTSIG_PING;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.ping.channelShadow = TEST_EDMA_DOPPLERPROC_HOTSIG_PING_SHADOW;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.ping.eventQueue = 0;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pong.channel = TEST_EDMA_DOPPLERPROC_HOTSIG_PONG;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pong.channelShadow = TEST_EDMA_DOPPLERPROC_HOTSIG_PONG_SHADOW;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pong.eventQueue = 0;

    /* window factor */
    dopplerProcDpuCfg.hwRes.hwaCfg.window = testDopplerWindow;
    //dopplerProcDpuCfg.hwRes.hwaCfg.winRamOffset = 256;   
   // dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED; // DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED
    dopplerProcDpuCfg.hwRes.hwaCfg.paramSetStartIdx = 0;

    /* cube input*/
    dopplerProcDpuCfg.hwRes.radarCube.datafmt = DPIF_RADARCUBE_FORMAT_1; //only format 1
    dopplerProcDpuCfg.hwRes.radarCube.data = (void *)testRadarCube;

    /* output */
    dopplerProcDpuCfg.hwRes.detMatrix.datafmt = DPIF_DETMATRIX_FORMAT_1;
    dopplerProcDpuCfg.hwRes.detMatrix.data = (void *)calculatedDetMatrix;
}

/* set up the doppler proc dpu configuration per test*/
void Test_setProfile(dopplerProcDpuTestConfig_t * testConfig)
{
    DPU_DopplerProcHWA_StaticConfig * params;
    DPU_DopplerProcHWA_HW_Resources * hwResParams;

    uint32_t ii;

    params = &dopplerProcDpuCfg.staticCfg;
    params->numTxAntennas = testConfig->numTxAntennas;
    params->numRxAntennas = testConfig->numRxAntennas;
    params->numVirtualAntennas = testConfig->numTxAntennas * testConfig->numRxAntennas;
    params->numRangeBins = testConfig->numRangeBins;
    params->numDopplerChirps = testConfig->numChirps;
    params->numDopplerBins = testConfig->numDopplerBins;
    ii = 1;
    while ((1 << ii) < testConfig->numDopplerBins)
    {
        ii++;
    }
    params->log2NumDopplerBins = ii;

    hwResParams = &dopplerProcDpuCfg.hwRes;
    /* windowing */
    dopplerProcDpuCfg.hwRes.hwaCfg.winSym = testConfig->winSymFlag; // HWA_FFT_WINDOW_SYMMETRIC;
    dopplerProcDpuCfg.hwRes.hwaCfg.winRamOffset = 1024 - (testConfig->numChirps + 64);


    //dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED; // DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED;  
    if (testConfig->winType == 1)     //if rec(1) win, enable, if hanning(0) disable
        dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED;
    else
        dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED;

    if (dopplerProcDpuCfg.hwRes.hwaCfg.winSym == HWA_FFT_WINDOW_NONSYMMETRIC)
    {
        hwResParams->hwaCfg.windowSize = testConfig->numChirps * sizeof(int32_t);
    }
    else
    {
        hwResParams->hwaCfg.windowSize = (testConfig->numChirps + 1) / 2 * sizeof(int32_t);
    }
    hwResParams->hwaCfg.numParamSets = 2 * testConfig->numTxAntennas + 2;
    /* cube */
    hwResParams->radarCube.dataSize = testConfig->numTxAntennas * testConfig->numRangeBins * testConfig->numDopplerBins * testConfig->numRxAntennas * 4;
   /* output*/
    hwResParams->detMatrix.dataSize = testConfig->numRangeBins * testConfig->numDopplerBins * sizeof(int16_t);     //output is uint16

}
 /*
 * generates the cuba data in format 1. for each range bin, it generates the exp signal with one peak. the peak 
   is at mod ((2*rangeBinIdx), numberofchirps);

 */

void Test_cubedata_gen(dopplerProcDpuTestConfig_t * testConfig)
{

    uint16_t ii, jj, kk;
    uint16_t numChirpsFrame;
    uint32_t radarCubeIdx;
    float amplitude; //signal amplitude
    //float snr; //signal snr
    float noiseScale; //noiseScale^2 is noisevaraince
    float dopplerIncr, dopplerAngle;
    float real, imag, noiseRe, noiseIm;
    uint32_t doppleBin;
    uint32_t txAntIdx;
    float var = 1;
    int32_t tempvalue;

    if (testConfig->numDopplerBins == 512)
    {
        amplitude = 25.f; // peak amplitude
        noiseScale = 620.f; // noise scale
    }
    else if (testConfig->numDopplerBins == 256)
    {
        amplitude = 25.f;
        noiseScale = 650.f;
    }
    else if (testConfig->numDopplerBins == 128)
    {
        amplitude = 50.f;
        noiseScale = 650.f;
    }
    else
    {
        amplitude = 60.f;
        noiseScale = 800.f;
    }
    
    //amplitude = 200.f;
    //noiseScale = 10.f;
    
    numChirpsFrame = testConfig->numTxAntennas * testConfig->numChirps;

    /* generate radar cube 1D fft output*/
    for (ii = 0; ii < numChirpsFrame; ii++) //chirp loop
    {
        for (jj = 0; jj < testConfig->numRxAntennas; jj++) //rx antenna loop
        {
            for (kk = 0; kk < testConfig->numRangeBins; kk++)
            {
               
                if (testConfig->numChirps < 8)
                    /* peak is at mod (kk, numChirps) */
                    doppleBin = kk - kk/ testConfig->numChirps * testConfig->numChirps;
                else
                    /* peak is at mod (2*kk, numChirps) */
                    doppleBin = (2 * kk) - (2 * kk) / testConfig->numChirps * testConfig->numChirps;
                dopplerIncr = 2.f * PI *  (float)doppleBin / (float)numChirpsFrame;
                dopplerAngle = ii * dopplerIncr;  //increase every chirp
                
                if (testConfig->numTxAntennas == 1)
                {
                    radarCubeIdx = ii * testConfig->numRxAntennas * testConfig->numRangeBins;
                    
                }
                else
                {
                    txAntIdx = ii - ii / testConfig->numTxAntennas * testConfig->numTxAntennas; //mod(ii, numTx)
                    radarCubeIdx = txAntIdx * testConfig->numChirps * testConfig->numRxAntennas * testConfig->numRangeBins;
                    radarCubeIdx += (ii / testConfig->numTxAntennas) * testConfig->numRxAntennas * testConfig->numRangeBins;
                }

                radarCubeIdx += jj * testConfig->numRangeBins;
                radarCubeIdx += kk;


                GaussianNoise(&var, &noiseRe, &noiseIm);   
                real = amplitude * cosf(dopplerAngle) + noiseScale * noiseRe;
                imag = amplitude * sinf(dopplerAngle) + noiseScale * noiseIm;
                tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(real);
                MATHUTILS_SATURATE16(tempvalue);
                testRadarCube[radarCubeIdx].real = (int16_t)tempvalue;
                tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(imag);
                MATHUTILS_SATURATE16(tempvalue);
                testRadarCube[radarCubeIdx].imag = (int16_t)tempvalue;

            }
        }/* end of rx antenna loop*/
    }//end of chirp loop

}

/* generates the rectangle and hanning window coefficients */
void Test_window2DCoef_gen(uint32_t numSamples, uint8_t winType)
{
    float ephyR, ephyI;
    float phi = 2 * PI_ / ((float)numSamples - 1);


    uint32_t ii;
    float a0;
    float winValue;
    int32_t winValueFixed;
    float cosValue, sinValue ;
    float temp;
    
    
    cosValue = 1.f;
    sinValue = 0.f;
    
    ephyR = cos(phi);         
    ephyI = sin(phi);   
    a0 = 0.5f;
   

    if (winType == 1)
    {
        for (ii = 0; ii < numSamples; ii++)
        {
            testDopplerWindow[ii] = (1 << 17) - 1; //rec window, for debug
        }
    }
    else
    {
        /* hanning window */
        for (ii = 0; ii < numSamples; ii++)
        {
            winValue = a0 * (1.f - cosValue);       //0.5*(1-cos(2*pi*n/N-1))

            winValue = winValue * (1 << 17) + 0.5;     //Q17 format
            winValueFixed = (uint32_t)winValue;
            if (winValueFixed >= (1 << 17))
            {
                winValueFixed = (1 << 17) - 1;
            }
            temp = cosValue;
            cosValue = cosValue * ephyR - sinValue * ephyI;
            sinValue = temp * ephyI + sinValue * ephyR;

            //testDopplerWindow[ii] = (1<<17) - 1; //rec window, for debug
            testDopplerWindow[ii] = winValueFixed;
        }
    }

}
/*  calculates fft, input is 32 bits I/Q, output is also 32x32 bits I/Q*/
void Test_2Dfft_ref(int32_t * dataOut, int32_t * dataIn, uint32_t fftSize, uint32_t numAdcSamples)
{
    uint32_t ii, jj;
    float cosValue, sinValue;
    float cosStep, sinStep;
    float alpha;
    float realValueTemp, imagValueTemp;
    float realValue, imagValue;
    float inReal, inImag;
    int32_t outReal, outImag;
    float tempValue;
    uint16_t divValue;

    /* hard code all the scale, mathc with hwa fft scale settings, asssume first fft scale is disabled. */
	divValue = 0;
    if (fftSize == 2)
        divValue = 0;
    else if (fftSize == 4)
        divValue = 1;
    else if (fftSize == 8)
        divValue = 2;
    else if (fftSize == 16)
        divValue = 3; //3 fft right shift, first one disabled
    else if (fftSize == 32)
        divValue = 4;
    else if (fftSize == 64)
        divValue = 5;
    else if (fftSize == 128)
        divValue = 6;
    else if (fftSize == 256)
        divValue = 7;
    else if (fftSize == 512)
        divValue = 8;
    else if (fftSize == 1024)
        divValue = 9;

    /* if first fft scale is enabled, scale + 1*/
    if (dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling == DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED)
        divValue += 1;

    //in 32 bits input, Real16/imag16
    for (ii = 0; ii < fftSize; ii++)
    {
        alpha = -2.f * ii * PI_ / (float)fftSize;
        cosValue = 1.f;
        sinValue = 0.f;
        cosStep = cos(alpha);
        sinStep = sin(alpha);
        //realValue = (float)((int16_t)(dataIn[0] >> 16));
        //imagValue = (float)((int16_t)(dataIn[0] & 0xFFFF));   //lsb at even
        realValue = (float)dataIn[1];
        imagValue = (float)dataIn[0];
        for (jj = 1; jj < numAdcSamples; jj++)
        {
            tempValue = cosValue;
            cosValue = cosValue * cosStep - sinValue * sinStep;
            sinValue = sinValue * cosStep + tempValue *sinStep;

            //inReal = (float)((int16_t)(dataIn[jj] >> 16));
            //inImag = (float)((int16_t)(dataIn[jj] & 0xFFFF));
            inReal = (float)dataIn[2 * jj + 1];
            inImag = (float)dataIn[2 * jj];

            realValueTemp = inReal * cosValue - inImag * sinValue;
            realValue += realValueTemp;
            imagValueTemp = inReal * sinValue + inImag * cosValue;
            imagValue += imagValueTemp;
        }
        /*match with hwa hardware */
        realValue = realValue / (float)(1 << divValue);
        outReal = (int32_t)MATHUTILS_ROUND_FLOAT(realValue);
        
        imagValue = imagValue / (float)(1 << divValue);
        outImag = (int32_t)MATHUTILS_ROUND_FLOAT(imagValue);

        dataOut[2 * ii] = outReal;
        dataOut[2 * ii + 1] = outImag ;
    }
}

/* 
    Generates the reference for Doppler proc DPU. 
    it implements windowing 16bits I/Q input,  32 bits I/Q output
    fft calculation,
    abs calculation, 
    log2 calcuations is Q 11 format 
*/
void  Test_2Dfft_refGen(uint32_t numSamples, uint32_t fftSize)
{
    uint32_t ii;
    int32_t winCoef;
    uint32_t dataSample;
    int64_t realValue, imagValue;
    int32_t fftoutR, fftoutI;
    double absValue, log2Value;
    int64_t tempValue;
    //int16_t leftScale;
    
    int32_t log2Temp;

    /* based on hwa setting,
    srcScale = 0; ---> shift 16bits input left (8-srcScale) bits
    test DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED mode, so fft scale is
    (cfg->staticCfg.numDopplerBins - 1) >> 1 ---> every stage, 1/2 scale, except the first one
    dstScale = 0; --> right shift dstScale bits,
    output is Q11 format
    */
    
    /* windowing, input is 16x16 complex,  output is 32 bits
       to match with hwa settings, the input is left shift 8 bits
    */
    for (ii = 0; ii < numSamples; ii++)
    {
        winCoef = (int32_t)testDopplerWindow[ii];
        dataSample = fftDataInBuf[ii];

        imagValue = (int64_t)  ((int16_t)(dataSample & 0x0000FFFF));      //lsb is imag
        imagValue = imagValue<< 8;
        tempValue = (imagValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
        fftDataWindow[ii * 2] = (int32_t) tempValue;       //imag is even

        realValue = (int64_t) ((int16_t)(dataSample >> 16)) ;
        realValue = realValue << 8;
        tempValue = (realValue * winCoef + 0x10000) >> 17;
        fftDataWindow[ii * 2 + 1] = (int32_t)tempValue;     //real is odd
    }
    
   
    /* fft */
    Test_2Dfft_ref(fftDataOutBuf, fftDataWindow, fftSize, numSamples );

    /* abs, log2 sum*/
    for (ii = 0; ii < fftSize; ii++)
    {
      
        fftoutR = fftDataOutBuf[2 * ii] ;
        fftoutI = fftDataOutBuf[2 * ii + 1] ;

        absValue = sqrt(((double)fftoutR * fftoutR + (double) fftoutI * fftoutI));
        if (absValue == 0)
            log2Value = 0;      //for debug
        else
            log2Value = log2(absValue);

        log2Value = log2Value  * 2048.f;  //in Q 11 format 

        if (log2Value >= 0)
            log2Temp = (int32_t)(log2Value + 0.5);
        else
            log2Temp = (int32_t)(log2Value - 0.5);

        detMatRefTemp[ii] += log2Temp;

    }
}

/* check the results from dpu with the reference */
void Test_printResults(dopplerProcDpuTestConfig_t * testConfig)
{
    uint32_t ii;
    uint32_t jj, kk, mm;
    uint32_t radarCubeIdx;
    int16_t rightScale;
    float sumTemp;
    uint32_t numZeros;
    uint8_t testFlag;
    

    uint32_t *cubePtr = (uint32_t *)testRadarCube;     //msb is real, lsb is imag

    testFlag = 1;
    /* to match with sum done by hwa fft
    srcScale = 2, left shift 6 bits
    fftstage shift is enabled, fftsize is testConfig->numTxAntennas * testConfig->numRxAntennas
    dstScale = 8,  right shift 8
    */
    rightScale = 0;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 1)
         // no fft, but still goes to input and output
        rightScale = 2; //right shift by 2
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 2)      //fft every stage >> 1
        rightScale = 3;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 4)      //fft every stage >> 1
        rightScale = 4;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 8)
        rightScale = 5;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 12)
        rightScale = 6;

    
    for (ii = 0; ii < testConfig->numRangeBins; ii++) //range bin
    {
        /* for accumulation over all tx antenna and rx antenna*/
        memset((void *)detMatRefTemp, 0, sizeof(int32_t) * testConfig->numDopplerBins);
        numZeros = 0;
        for (jj = 0; jj < testConfig->numTxAntennas; jj++)
        {
            for (kk = 0; kk < testConfig->numRxAntennas; kk++)
            {
                for (mm = 0; mm < testConfig->numChirps; mm++)
                {
                    radarCubeIdx = jj * testConfig->numChirps * testConfig->numRxAntennas *  testConfig->numRangeBins;
                    radarCubeIdx += (mm * testConfig->numRxAntennas * testConfig->numRangeBins);
                    radarCubeIdx += (kk * testConfig->numRangeBins);
                    radarCubeIdx += ii;

                    fftDataInBuf[mm] = cubePtr[radarCubeIdx];
                   // System_printf("%d,%d\n", testRadarCube[radarCubeIdx].real, testRadarCube[radarCubeIdx].imag);
                    
                }  /* end of doppler bin loop*/

                /* call reference generation */
                Test_2Dfft_refGen(testConfig->numChirps, testConfig->numDopplerBins);
               

            } //end of receiver antenna

        }//end of tx antenna

        
        for (mm = 0; mm < testConfig->numDopplerBins; mm++)
        {
            sumTemp = detMatRefTemp[mm] >> rightScale;
            detMatRef[mm] = sumTemp;
        }


        /* verify the test results*/
        for (mm = 0; mm < testConfig->numDopplerBins; mm++)
        {
            /* dpu output is doppler, then range bin */
            if (abs((detMatRef[mm] - calculatedDetMatrix[ii * testConfig->numDopplerBins + mm])) > MAX_ERROR)
            {
                System_printf("Rangebin = %d, DopplerBin = %d, DopplerSize = %d, Ref=%d, dpu=%d err= %d fail\n", ii, mm, testConfig->numDopplerBins, detMatRef[mm], calculatedDetMatrix[ii * testConfig->numDopplerBins + mm], abs(detMatRef[mm] - calculatedDetMatrix[ii * testConfig->numDopplerBins + mm]));
                finalResults = 0;
                testFlag = 0;
            }

            if (calculatedDetMatrix[ii * testConfig->numDopplerBins + mm] < 2)
                numZeros++;
            
       }

        if ((testConfig->numDopplerBins < 8) && (numZeros == testConfig->numDopplerBins))
        {
            System_printf("Rangebin = %d, DopplerSize = %d, all zero output, fail\n", ii, testConfig->numDopplerBins);
            finalResults = 0;
            testFlag = 0;
        }
        if ((testConfig->numDopplerBins >= 8) && (numZeros > testConfig->numDopplerBins - 5))
        {
            System_printf("Rangebin = %d, DopplerSize = %d, all zero output, fail\n", ii, testConfig->numDopplerBins);
            finalResults = 0;
            testFlag = 0;
        }

    }//end of rangebin
    
    if (testFlag == 1)
    {
        System_printf("Test Pass\n");
    }
    else
    {
        System_printf("Test Fail\n");
    }
    System_printf("\n");
}
/*
    test main task
*/
void Test_task(UArg arg0, UArg arg1)
{
    uint32_t ii,jj;
    uint32_t numRangeBin[7] = {16, 32, 64, 128, 256, 512, 1024 };
    uint32_t numDopplerBin[9] = {2, 4, 8, 16, 32, 64, 128, 256, 512 };
    uint32_t numRxAnt[3] = {1, 2, 4 };
    uint32_t numTxAnt[3] = {1, 2, 3 };
    uint32_t txAntIdx, rxAntIdx, numTestRangeBin;
    uint32_t numTests;
    dopplerProcDpuTestConfig_t testConfig;
    char featureName[200];
    uint32_t startTime, bench1, bench2;
    int32_t   retVal;
    DPU_DopplerProcHWA_OutParams out;
    uint32_t testCount;
    uint32_t preNumChirps;
    uint32_t maxNumChirps;

    finalResults = 1;

    System_printf("...... Initialization ...... \n");

    /* hwa, edma, dpu inialization */
    dopplerProc_test_hwaEdmainit();
    dopplerProc_test_dpuInit();

    /* doppler dpu config*/
    dopplerProc_test_dpuConfig();

    System_printf("Doppler Proc DPU Test start ...... \n");

    /* to fit cuba data in L3 memory */
    /*1tx, dopplerbin <= 16, upto 1024 7 range bin sizes
           dopplerbin = 32, upto 1024 7 range bin sizes
           dopplerbin = 64, upto 512, 6 range bin sizes,
           dopplerbin = 128, upto 256, 5 range bin sizes,
           dopplerbin = 256, upto 128, 4 range bin sizes,
           doppler bin = 512, upto 64, 3 range bin sizes
    for rx = 1, 2, 4 3 cases
    */
    numTests = (7 * 5 + 6 + 5 + 4 + 3) * 3;
    /*2tx, 2rx 
    dopplerbin <= 16, upto 1024 7 range bin sizes
    dopplerbin = 32, upto 1024 7 range bin sizes
    dopplerbin = 64, upto 512, 6 range bin sizes,
    dopplerbin = 128, upto 256, 5 range bin sizes,
    dopplerbin = 256, upto 128, 4 range bin sizes,
    doppler bin = 512, upto 64, 3 range bin sizes
    4rx
    dopplerbin <= 16, upto 1024 7 range bin sizes
    dopplerbin = 32, upto 512, 6 range bin sizes,
    dopplerbin = 64, upto 256, 5 range bin sizes,
    dopplerbin = 128, upto 128, 4 range bin sizes,
    doppler bin = 256, upto 64, 3 range bin sizes
    doppler bin = 512, upto 64, 2 range bin sizes
    */

    numTests += ((7 * 5 + 6 + 5 + 4 + 3) + (7 * 4 + 6 + 5 + 4 + 3 + 2));
    /* 3tx,  4rx same as 2tx, 4 rx*/
    numTests += (7 * 4 + 6 + 5 + 4 + 3 + 2);
   
    printf("\n...... %d Tests to be run .......\n\n", numTests);

    testCount = 0;
    
    for (txAntIdx = 0; txAntIdx < 3; txAntIdx++)        //tx=1,2,3
    {
        for (rxAntIdx = txAntIdx; rxAntIdx < 3; rxAntIdx++) //1/1,2,4, 2/2,4, 3/4
        {
            testConfig.numTxAntennas = numTxAnt[txAntIdx];
            testConfig.numRxAntennas = numRxAnt[rxAntIdx];
            


            for (ii = 0; ii < 9; ii++) /* doppler bin loop, 2, 4, 8, 16, 32, 64, 128, 256, 512 */
            {
                testConfig.numDopplerBins = numDopplerBin[ii];
                /* numTx*numRx*numberchips * 4(16I/Q) < 16384 */
                maxNumChirps = 16384 / testConfig.numTxAntennas / testConfig.numRxAntennas / 4;
                if (maxNumChirps > numDopplerBin[ii])
                    maxNumChirps = numDopplerBin[ii];

                preNumChirps = 0; 
                {
                   
                    //1 tx, 1,2,4 rx, or 2tx/2rx, 16, 32 doppler bin, upto 1024 range bin
                    if ((testConfig.numTxAntennas == 1) || ((testConfig.numTxAntennas == 2) && (testConfig.numRxAntennas == 2)))
                    {
                        if (ii < 5)      
                            numTestRangeBin = 7;
                        else
                            numTestRangeBin = 11 - ii;    
                    }
                    else  //2tx/4rx, 3tx/4 rx only <=16 doppler bin, upto 1024 range bin
                    {
                        if (ii < 4)
                            numTestRangeBin = 7;
                        else
                            numTestRangeBin = 10 - ii;
                    }

                    for (jj = 0; jj < numTestRangeBin; jj++)
                    {

                        checkResults = 1;

                        if (numDopplerBin[ii] == 2)
                            testConfig.numChirps = 2;
                        else if (numDopplerBin[ii] == 4)
                        {
                            testConfig.numChirps = 3 + (jj & 0x1);
                        }
                        else if (numDopplerBin[ii] == 8)
                        {
                            testConfig.numChirps = 5 + (jj & 0x3);
                        }
                        else if (numDopplerBin[ii] == 16)
                        {
                            testConfig.numChirps = 9 + (jj & 0x7);
                        }
                        else
                        {
                            if ((jj & 0x3) == 0)
                                testConfig.numChirps = maxNumChirps;
                            else
                            {
                                testConfig.numChirps = maxNumChirps - (ii + 1) * 4 - jj / 2 * 2;
                            }

                            if (testConfig.numChirps == preNumChirps)
                                testConfig.numChirps++;

                            if ((numTestRangeBin == 2) && (jj) && (maxNumChirps == numDopplerBin[ii]))        //for doppler size is 512, add an odd number chirp tests
                                testConfig.numChirps++;
                        }

                        preNumChirps = testConfig.numChirps;

                        if ( (testCount & 0x7 )== 0) /* mod (8) ==0, set to rec window*/
                            testConfig.winType = 1;
                        else
                            testConfig.winType = 0; //hanning window


                        if (testConfig.numChirps == 2)
                            testConfig.winType = 1; //if only 2 chirp samples, can only use rec window, hanning window coefficients are all zero.

                        /* generate the windowing factors*/
                        Test_window2DCoef_gen(testConfig.numChirps, testConfig.winType);


                        testConfig.numRangeBins = numRangeBin[jj];
                       
#if (defined (SOC_XWR68XX))
                        if ((testConfig.numTxAntennas == 3) && (testConfig.numRxAntennas == 4))
                        {
                            if ((testConfig.numDopplerBins == 16) && (testConfig.numRangeBins == 1024))
                                testConfig.numRangeBins = 512;
                            if ((testConfig.numDopplerBins == 64) && (testConfig.numRangeBins == 256))
                                testConfig.numRangeBins = 128;
                        }
                       
#endif
                        testConfig.winSymFlag = (ii + jj + txAntIdx + rxAntIdx) & 0x1;

                        System_printf("\nTest #%d  start\r\n", testCount);

                          /* set up test profile based on each test*/
                        Test_setProfile(&testConfig);

                        sprintf(featureName, "%s%d%s%d%s%d%s%d%s%d%s%d%s%d",
                            ":Tx", testConfig.numTxAntennas,
                            " Rx", testConfig.numRxAntennas,
                            " Chirp", testConfig.numChirps,
                            " R", testConfig.numRangeBins,
                            " D", testConfig.numDopplerBins,
                            " WinSym", testConfig.winSymFlag,
                            " 1stFftScale", dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling);

                        System_printf("Test feature : %s\n", featureName);
                      
                        /* generate test 1D fft output*/
                        Test_cubedata_gen(&testConfig);

                        /* call the doppler dpu*/
                        startTime = Cycleprofiler_getTimeStamp();
                        retVal = DPU_DopplerProcHWA_config(dopplerProcDpuHandle, &dopplerProcDpuCfg);
                        if (retVal < 0)
                        {
                            System_printf("doppler DPU failed to configure. Error %d.\n", retVal);
                            DebugP_assert(0);
                            return;
                        }
                        bench1 = Cycleprofiler_getTimeStamp() - startTime;

                        System_printf("Doppler DPU configured. Cycles = %d\n", bench1);

                        startTime = Cycleprofiler_getTimeStamp();
                        retVal = DPU_DopplerProcHWA_process(dopplerProcDpuHandle, &out);
                        if (retVal < 0)
                        {
                            System_printf("doppler DPU failed to run. Error %d.\n", retVal);
                            DebugP_assert(0);
                            return;
                        }
                        bench2 = Cycleprofiler_getTimeStamp() - startTime;
                        System_printf("Doppler DPU Process. Cycles = %d. \n", bench2);

                        if (checkResults)
                        {
                            Test_printResults(&testConfig);
                            System_printf("Test #%d finished!\n\r", testCount);
                        }
                        else
                        {
                            System_printf("Test #%d crashes, KNOW ISSUE!\n\r", testCount);
                        }

                        testCount++;
                    }
                }

            }
        }
    }

    EDMA_close(edmaHandle);
    HWA_close(hwaHandle);

    retVal = DPU_DopplerProcHWA_deinit(dopplerProcDpuHandle);
    if (retVal < 0)
    {
        System_printf("doppler DPU failed to deinit. Error %d.\n", retVal);
        DebugP_assert(0);
        return;
    }

    
    
    System_printf("----------------------------\n\n");
    if (finalResults == 1)
    {
        System_printf("All Tests PASSED!\n");
        MCPI_setFeatureTestResult("doppler_DPU", MCPI_TestResult_PASS);
    }
    else
    {
        System_printf("Test FAILED!\n");
        MCPI_setFeatureTestResult("doppler_DPU", MCPI_TestResult_FAIL);
    }

    System_printf("Test Finished\n");
    MCPI_setTestResult();

    /* Exit BIOS */
    BIOS_exit(0);


}


int main (void)
{
    Task_Params taskParams;
    int32_t     errCode;
    SOC_Cfg     socCfg;

    /* Initialize test logger */
    MCPI_Initialize();

#ifdef SUBSYS_MSS
    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);
#endif
    Cycleprofiler_init();

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        DebugP_assert(0);
        return -1;
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 8*1024;
    Task_create(Test_task, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    
    return 0;
}

