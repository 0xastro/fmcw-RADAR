/**
 *   @file  rangeProcDSP_test.c
 *
 *   @brief
 *      Unit Test code for rangeProcDSP DPU.
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

#include <ti/common/sys_common.h>
#include <ti/utils/testlogger/logger.h>

#include <ti/drivers/esm/esm.h>
#define DebugP_ASSERT_ENABLED 1
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/edma/edma.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpu/rangeproc/rangeprocdsp.h>
#pragma diag_push
#pragma diag_suppress 48
#include <ti/mathlib/mathlib.h>
#pragma diag_pop

#include <ti/utils/cycleprofiler/cycle_profiler.h> 


#define  MAX_NUM_RANGEBIN   2048
#define  MAX_NUM_RX_ANTENNA 4
#define  MAX_NUM_ADCSAMPLE_PERCHIRP 4096   // 1,2 rx, maximum is 2048, if 4 rx, maximum is 1024
#define  MAX_NUM_TX_ANTENNA 3
#define  MAX_NUM_CHIRPS_PERFRAME  6       // for tx=3, number of chirps = 6, otherwise = 4     
#define  MAX_ALLOWED_ERROR 18/* 10 for upto 1024 ADCSamples, 18 for 2048 ADC samples*/ 
#define  MAX_NUM_CHIRPEVENT 2

/* Data memeory */

/* L3 RAM buffers */
#pragma DATA_SECTION(dataInBuffer, ".l3data");
uint32_t  dataInBuffer[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME]; /*to save all adc buffer data */
#pragma DATA_SECTION(dataInBuffer, ".l3data");
uint32_t  adcDataInBuf[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPEVENT]; /*only for chirp*/
uint32_t * adcDataIn;

#pragma DATA_SECTION(radarCube, ".l3data");
cmplx16ImRe_t radarCube[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME];
#pragma DATA_SECTION(fft1DOut_ref, ".l3data");
uint32_t fft1DOut_ref[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME]; /* fixed to 16I and 16Q */
#pragma DATA_SECTION(dataWinOut, ".l3data");
uint32_t dataWinOut[MAX_NUM_RANGEBIN]; /* per rx antenna, per chirp for windowing results */

#pragma DATA_SECTION(dcRangeSigMean, ".dataBufL2");
cmplx32ImRe_t dcRangeSigMean[MAX_NUM_TX_ANTENNA * MAX_NUM_RX_ANTENNA *32 ]; 


#define MAX_1D_ADCSAMPLES 2048
#define MAX_1D_RANGEBINS 2048

/* L1 RAM buffers */
#pragma DATA_SECTION(adcDataInScratch ,".l1data");
#pragma DATA_ALIGN(adcDataInScratch,16);
cmplx16ImRe_t adcDataInScratch[MAX_1D_ADCSAMPLES * 2U];

/* L2 RAM buffers */
#pragma DATA_SECTION(window1DCoef ,".dataBufL2");
#pragma DATA_ALIGN(window1DCoef,8);
int16_t window1DCoef[MAX_NUM_RANGEBIN];

#pragma DATA_SECTION(twiddle16x16_1D ,".dataBufL2");
#pragma DATA_ALIGN(twiddle16x16_1D,16);
cmplx16ImRe_t twiddle16x16_1D[MAX_1D_RANGEBINS];

#pragma DATA_SECTION(fftOut1DScratch,".dataBufL2");
#pragma DATA_ALIGN(fftOut1DScratch,16);
cmplx16ImRe_t fftOut1DScratch[MAX_NUM_ADCSAMPLE_PERCHIRP * 2U];

typedef struct rangeProcTestConfig_t_ {
    uint32_t numTxAntennas;
    uint32_t numRxAntennas;
    uint32_t numAdcSamples;
    uint32_t numRangeBins;
    uint32_t numChirpsPerFrame;
    uint8_t  cmplxIQswapFlag;    
    uint8_t  rxChanInterleave;
    uint8_t  radarCubeLayoutFmt;
    uint8_t  numChirpEvents;
} rangeProcTestConfig_t;

#define     MMW_HWA_WINDOWRAM_1D_OFFSET         0

/* EDMA resource partitioning */
#define EDMA_SHADOW_LNK_PARAM_BASE_ID       EDMA_NUM_DMA_CHANNELS

#define PI_ 3.14159265

DPU_RangeProc_CalibDcRangeSigCfg gCalibDcConfig=
{
    0,  //disabled
    -5,
    5,
    128
};
 
SOC_Handle          socHandle;
EDMA_Handle         edmaHandle;
/*! @brief   EDMA error Information when there are errors like missing events */
EDMA_errorInfo_t  EDMA_errorInfo;
/*! @brief EDMA transfer controller error information. */
EDMA_transferControllerErrorInfo_t EDMA_transferControllerErrorInfo;

DPU_RangeProcDSP_Handle rangeProcDpuHandle;
DPU_RangeProcDSP_Config rangeProcDpuCfg;

uint8_t testFeatureStatus;

uint8_t finalResults ;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);


FILE * fileId;   


/**
 *  @b Description
 *  @n
 *      Call back function for EDMA CC (Channel controller) error as per EDMA API.
 *      Declare fatal error if happens, the output errorInfo can be examined if code
 *      gets trapped here.
 */
void MmwDemo_EDMA_errorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
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
void MmwDemo_EDMA_transferControllerErrorCallbackFxn(EDMA_Handle handle,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

static inline void test_genWindow(void *win,
                        uint32_t winLen,
                        uint32_t winGenLen,
                        uint32_t winType)
{
    
    float ephyR, ephyI;
    float phi = 2 * PI_ / ((float) winLen - 1);
    
    ephyR  = cossp(phi);
    ephyI  = sinsp(phi);

    mmwavelib_windowCoef_gen(win,
                        MMWAVELIB_WINDOW_INT16,
                        ephyR,
                        ephyI,
                        winGenLen,
                        15,
                        winType);

}


void Test_setProfile(rangeProcTestConfig_t * testConfig)
{
    DPU_RangeProcDSP_HW_Resources  *pHwConfig;
    DPU_RangeProcDSP_StaticConfig  *params;
    uint32_t index;
    uint32_t      bytesPerRxChan;
    
    pHwConfig = &rangeProcDpuCfg.hwRes; 
    params = &rangeProcDpuCfg.staticCfg; 
    
    
    /* overwrite the DPIF_commonParams with the test configuration*/
    params->numTxAntennas = testConfig->numTxAntennas;
    params->numVirtualAntennas = testConfig->numTxAntennas * testConfig->numRxAntennas;
    params->numRangeBins = testConfig->numRangeBins;
    params->numChirpsPerFrame = testConfig->numChirpsPerFrame;

    params->ADCBufData.data = (void *)adcDataIn;


    /* windowing */
    pHwConfig->windowSize = sizeof(uint16_t) * testConfig->numAdcSamples; //hwa is 18 bits, whole length
    params->ADCBufData.dataSize = testConfig->numAdcSamples * testConfig->numRxAntennas * 4 ;
    params->ADCBufData.dataProperty.numAdcSamples = testConfig->numAdcSamples;
    params->ADCBufData.dataProperty.numRxAntennas = testConfig->numRxAntennas;
    pHwConfig->windowSize = sizeof(int16_t) * testConfig->numAdcSamples /2;
    bytesPerRxChan = testConfig->numAdcSamples * sizeof(uint32_t);
    bytesPerRxChan = (bytesPerRxChan + 15)/16 * 16;
    
    for (index = 0; index < testConfig->numRxAntennas; index++)
    {
        params->ADCBufData.dataProperty.rxChanOffset[index] = index * bytesPerRxChan * testConfig->numChirpEvents;
    }
    {
        params->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_NON_INTERLEAVE_MODE;

        /* Data Input EDMA */
        pHwConfig->edmaCfg.dataInPing.channel         = EDMA_TPCC0_REQ_FREE_0;
        pHwConfig->edmaCfg.dataInPing.channelShadow   = EDMA_SHADOW_LNK_PARAM_BASE_ID ;
        pHwConfig->edmaCfg.dataInPing.eventQueue      = 0;
        pHwConfig->edmaCfg.dataInPong.channel         = EDMA_TPCC0_REQ_FREE_1;
        pHwConfig->edmaCfg.dataInPong.channelShadow   = EDMA_SHADOW_LNK_PARAM_BASE_ID +1;
        pHwConfig->edmaCfg.dataInPong.eventQueue      = 0;

        /* Output Ping*/
        pHwConfig->edmaCfg.dataOutPing.channel         = EDMA_TPCC0_REQ_FREE_2;
        pHwConfig->edmaCfg.dataOutPing.channelShadow   = EDMA_SHADOW_LNK_PARAM_BASE_ID + 2;
        pHwConfig->edmaCfg.dataOutPing.eventQueue= 0;
        /* Output Pong*/
        pHwConfig->edmaCfg.dataOutPong.channel         = EDMA_TPCC0_REQ_FREE_3;
        pHwConfig->edmaCfg.dataOutPong.channelShadow   = EDMA_SHADOW_LNK_PARAM_BASE_ID +3;
        pHwConfig->edmaCfg.dataOutPong.eventQueue       = 0;
    }

    /* radar cube*/
    //only 1 tx is transmitted at one chirp 
    pHwConfig->radarCube.dataSize = testConfig->numRangeBins * testConfig->numRxAntennas * sizeof(uint32_t) * testConfig->numChirpsPerFrame;
    // only support format 1
    pHwConfig->radarCube.datafmt = DPIF_RADARCUBE_FORMAT_1;  

    if (gCalibDcConfig.enabled == 1)     //set the dc calibration buffer size
    {
        params->resetDcRangeSigMeanBuffer = 1;   //reset the buffer, first call
    }
}

/**
*  @b Description
*  @n
*    For each test, check the radar cube data with the references
*/
void Test_printResults(rangeProcTestConfig_t * testConfig, uint8_t flag) //, char* feature)
{
    uint32_t testStatus; 
    uint32_t jj, kk, ii;
    uint32_t chirpIdx;
    uint32_t radarCubeIdx;
    uint32_t * ptrCubeRef;
    int16_t realValueRef, imagValueRef;
    uint32_t txAntIdx;
    int32_t sumReal, sumImag;
    int16_t tempReal, tempImag;
    uint32_t tempValue;
    
    
    testStatus = 1;
    
    /* compare the output */
    for (chirpIdx = 0; chirpIdx < testConfig->numChirpsPerFrame; chirpIdx++)
    {
        for (jj = 0; jj < testConfig->numRxAntennas; jj++)
        {
            if (testConfig->numRangeBins == 2048)
                ptrCubeRef = &fft1DOut_ref[chirpIdx * testConfig->numRangeBins * 2 + jj * testConfig->numRangeBins];
            else
                ptrCubeRef = &fft1DOut_ref[chirpIdx * testConfig->numRangeBins * MAX_NUM_RX_ANTENNA + jj * testConfig->numRangeBins];

            for (kk = 0; kk < testConfig->numRangeBins; kk++)
            {
                /* only support format 1*/
                {
                    if (testConfig->numTxAntennas == 1)
                        radarCubeIdx = chirpIdx * testConfig->numRangeBins * testConfig->numRxAntennas + jj * testConfig->numRangeBins + kk;
                    else //if (testConfig->numTxAntennas == 2)
                    {
                        //txAntIdx = chirpIdx & 0x1;  // chirpIdx mod(2)=0, tx = 0, mod(2)=1, tx = 1
                        txAntIdx = chirpIdx - chirpIdx/testConfig->numTxAntennas * testConfig->numTxAntennas; //mod(numTxAnt)
                        radarCubeIdx = txAntIdx * (testConfig->numChirpsPerFrame/testConfig->numTxAntennas ) * testConfig->numRxAntennas * testConfig->numRangeBins ;
                        radarCubeIdx +=  (chirpIdx/testConfig->numTxAntennas) * testConfig->numRxAntennas * testConfig->numRangeBins  + jj * testConfig->numRangeBins + kk;
                    }
                }

                realValueRef =  (int16_t) (ptrCubeRef[kk] &0x0000FFFF);   //fixed the reference output as Imag16/Real16
                imagValueRef =  (int16_t) (ptrCubeRef[kk] >> 16);
                if (flag == 1)
                {
                    if ((kk <= gCalibDcConfig.positiveBinIdx) || (kk>= testConfig->numRangeBins + gCalibDcConfig.negativeBinIdx))
                    {
                        //remove the mean from previous frame
                        sumReal = 0;
                        sumImag = 0;
                        for (ii = 0; ii < testConfig->numChirpsPerFrame / testConfig->numTxAntennas; ii++)
                        {
                            if (testConfig->numRangeBins == 2048)
                                tempValue = fft1DOut_ref[(ii * testConfig->numTxAntennas + (chirpIdx - chirpIdx / testConfig->numTxAntennas * testConfig->numTxAntennas)) * testConfig->numRangeBins * 2 + jj * testConfig->numRangeBins + kk];
                            else
                                tempValue = fft1DOut_ref[(ii * testConfig->numTxAntennas + (chirpIdx - chirpIdx / testConfig->numTxAntennas * testConfig->numTxAntennas)) * testConfig->numRangeBins * MAX_NUM_RX_ANTENNA + jj * testConfig->numRangeBins + kk];

                            tempReal = (int16_t)(tempValue & 0x0000FFFF);
                            tempImag = (int16_t)(tempValue >> 16);
                            sumReal += tempReal;
                            sumImag += tempImag;
                        }

                        realValueRef -= (sumReal / testConfig->numChirpsPerFrame * testConfig->numTxAntennas);
                        imagValueRef -= (sumImag / testConfig->numChirpsPerFrame * testConfig->numTxAntennas);
                    }

                }
                
                if (abs(radarCube[radarCubeIdx].real - realValueRef) > MAX_ALLOWED_ERROR)
                {
                    testStatus = 0;
                    testFeatureStatus = 0;
                    finalResults = 0;
                    //System_printf("radar cube index %d\n", radarCubeIdx);
                    System_printf(" test fail at chirp %d, rx antenna %d, bin %d, cube=%d, ref=%d\n ", chirpIdx, jj, kk, radarCube[radarCubeIdx].real, realValueRef );
                }
                if (abs(radarCube[radarCubeIdx].imag - imagValueRef) > MAX_ALLOWED_ERROR)
                {
                    testStatus = 0;
                    testFeatureStatus = 0;
                    finalResults = 0;
                    System_printf(" test fail at chirp %d, rx antenna %d, bin %d, cube=%d, ref=%d\n ", chirpIdx, jj, kk,radarCube[radarCubeIdx].imag,imagValueRef);
                }
            }
        }
    }
    
    if(testStatus == 1)
    {
        System_printf("Test PASSED!\n");
   //     MCPI_setFeatureTestResult(feature, MCPI_TestResult_PASS);
    }
    else
    {
        System_printf("Test FAILED!\n");
     //   MCPI_setFeatureTestResult(feature, MCPI_TestResult_FAIL);
    }

    
}

void Test_fft_ref(uint32_t * dataOut, uint32_t * dataIn, uint32_t fftSize, uint32_t numAdcSamples)
{
    uint32_t ii,jj;
    float cosValue, sinValue;
    float cosStep, sinStep;
    float alpha;
    float realValueTemp, imagValueTemp;
    float realValue, imagValue;
    float inReal, inImag;
    int16_t outReal, outImag;
    float tempValue;
    float divValue;
    
    /* hard code all the  */
    if (fftSize == 64)
       divValue = 4.f;
    else if (fftSize==128)
        divValue = 8.f;
    else if (fftSize == 256)
        divValue = 8.f;
    else if (fftSize == 512)
        divValue = 16.f;
    else if (fftSize == 1024)
        divValue = 16.f;
    else if (fftSize == 2048)
        divValue = 32.f;
    else
        divValue = 4.f;
    for (ii = 0; ii < fftSize; ii++)
    {
        alpha = -2.f * ii * PI_ /(float) fftSize;
        cosValue = 1.f;
        sinValue = 0.f;
        cosStep = cos(alpha);
        sinStep = sin(alpha);
        realValue = (float)  ((int16_t)(dataIn[0] >> 16)); 
        imagValue = (float)  ((int16_t)(dataIn[0] &0xFFFF));
        for (jj = 1; jj < numAdcSamples; jj++)
        {
            tempValue = cosValue;
            cosValue = cosValue * cosStep - sinValue * sinStep;
            sinValue = sinValue * cosStep + tempValue *sinStep;
            
            inReal = (float)  ((int16_t)(dataIn[jj] >> 16));
            inImag = (float)  ((int16_t)(dataIn[jj] &0xFFFF));
            
            realValueTemp = inReal * cosValue - inImag * sinValue;
            realValue += realValueTemp;
            imagValueTemp = inReal * sinValue + inImag * cosValue;
            imagValue += imagValueTemp;
        }
        /*match with hwa hardware */
        realValue = realValue /divValue;
        if (realValue > 0)
            outReal = (int16_t) (realValue + 0.5);
        else
            outReal = (int16_t) (realValue - 0.5);
        imagValue = imagValue /divValue;
        if (imagValue > 0)
        {
            outImag = (int16_t) (imagValue + 0.5);
        }
        else
            outImag = (int16_t) (imagValue - 0.5);
        
        dataOut[ii] = outReal;
        dataOut[ii] += (outImag << 16);  //output I fixed to this way
    }
}

/**
*  @b Description
*  @n
*    Generate the reference for radar cuba result check.
*/
void Test_1Dfft_refGen( uint32_t numAdcSamples, uint32_t numRxAnt, uint32_t fftSize, uint32_t * dataInPtr, uint32_t * fftOutPtr)
{
    uint32_t kk, jj;
    int32_t winCoef;
    uint32_t dataSample;
    int16_t realValue, imagValue;
    int32_t tempValue;
    uint32_t * ptrFftRef;
 
    ptrFftRef = fftOutPtr;
    for (jj = 0; jj < numRxAnt; jj++)
    {
        
        /* windowing */
        for (kk = 0; kk < numAdcSamples/2; kk++)
        {
            winCoef = (int32_t) window1DCoef[kk];
            dataSample = dataInPtr[kk];
            
            realValue =  (int16_t) (dataSample&0x0000FFFF);
            /* Q15 format */
            tempValue = (realValue * winCoef + 0x4000) >> 15;   //window coefficients is Q15 format
            if (tempValue >= (1<<15))
                dataWinOut[kk] = 0x7fff;
            else
                dataWinOut[kk] = (uint16_t) tempValue;
            
            imagValue = (int16_t) (dataSample>>16);

            /* Q15 format */
            tempValue = (imagValue * winCoef + 0x4000) >> 15;   //window coefficients is Q15 format
            
            if (tempValue >= (1<<15))
                 dataWinOut[kk] += ( 0x7fff <<16);
            else
                 dataWinOut[kk] +=  ( ((uint16_t)tempValue) << 16);
        }
        for (kk = numAdcSamples/2; kk < numAdcSamples; kk++)
        {
            winCoef = (int32_t) window1DCoef[numAdcSamples - 1 - kk ];
            dataSample = dataInPtr[kk];
            
            realValue =  (int16_t) (dataSample&0x0000FFFF);
            /* Q15 format */
            tempValue = (realValue * winCoef + 0x4000) >> 15;   //window coefficients is Q15 format
            if (tempValue >= (1<<15))
                dataWinOut[kk] = 0x7fff;
            else
                dataWinOut[kk] = (uint16_t) tempValue;
            
            imagValue = (int16_t) (dataSample>>16);

            /* Q15 format */
            tempValue = (imagValue * winCoef + 0x4000) >> 15;   //window coefficients is Q15 format
            
            if (tempValue >= (1<<15))
                 dataWinOut[kk] += ( 0x7fff <<16);
            else
                 dataWinOut[kk] +=  ( ((uint16_t)tempValue) << 16);
        }
        /* append the zero */
        dataInPtr += fftSize;       //must be fft size, if adc samples < fft size, 
        for ( kk = numAdcSamples; kk < fftSize; kk++)
        {
            dataWinOut[kk] = 0;
        }
        
        /* fft */
        Test_fft_ref(ptrFftRef, dataWinOut, fftSize, numAdcSamples);
        ptrFftRef += fftSize;
    }
}

void rangeProcDpuTest_edmaInit()
{
    uint8_t             edmaNumInstances;
    uint8_t             inst;
    EDMA_instanceInfo_t edmaInstanceInfo;
    EDMA_errorConfig_t  errorConfig;
    int32_t             errorCode;
    
    edmaNumInstances = EDMA_getNumInstances();
    for (inst = 0; inst < edmaNumInstances; inst++)
    {
        errorCode = EDMA_init(inst);
        if (errorCode != EDMA_NO_ERROR)
        {
            System_printf ("Debug: EDMA instance %d initialization returned error %d\n", errorCode);
            DebugP_assert (0);
            return;
        }
    }
    memset(&EDMA_errorInfo, 0, sizeof(EDMA_errorInfo));
    memset(&EDMA_transferControllerErrorInfo, 0, sizeof(EDMA_transferControllerErrorInfo));

    edmaHandle = EDMA_open(0, &errorCode, &edmaInstanceInfo);
    if (edmaHandle == NULL)
    {
        System_printf("Error: Unable to open the EDMA Instance err:%d\n",errorCode);
        DebugP_assert (0);
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
        DebugP_assert (0);
        return;
    }
    
    
}
void rangeProcDpuTest_dpuInit()
{
    int32_t  errorCode = 0;
    /* generate the dpu handler*/
    rangeProcDpuHandle =  DPU_RangeProcDSP_init (&errorCode);
    if (rangeProcDpuHandle == NULL)
    {
        System_printf ("Debug: RangeProc DPU initialization returned error %d\n", errorCode);
        DebugP_assert (0);
            return;
    }
}
void rangeProcDpuTest_dpuCfg()
{
    DPU_RangeProcDSP_HW_Resources * pHwConfig;
    
    memset((void *)&rangeProcDpuCfg, 0, sizeof(DPU_RangeProcDSP_Config));

    /* hw configuration */
    pHwConfig = &rangeProcDpuCfg.hwRes; 

    /* edma configuration */
    pHwConfig->edmaCfg.edmaHandle  = edmaHandle;  
    
    /* adc buffer buffer, format fixed, interleave, size will change */
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_IMRE;
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.adcBits = 2U;
    //rangeProcDpuCfg.staticCfg.ADCBufData.data = (void *)adcDataIn;

    rangeProcDpuCfg.hwRes.window = (int16_t *) window1DCoef;
    rangeProcDpuCfg.hwRes.windowSize = sizeof(int16_t) * MAX_NUM_RANGEBIN;
        
    rangeProcDpuCfg.hwRes.twiddle16x16 = twiddle16x16_1D;
    rangeProcDpuCfg.hwRes.twiddleSize = sizeof(cmplx16ImRe_t) * MAX_NUM_RANGEBIN;
        
    rangeProcDpuCfg.hwRes.adcDataIn = adcDataInScratch;
    rangeProcDpuCfg.hwRes.adcDataInSize = sizeof(cmplx16ImRe_t) * MAX_1D_ADCSAMPLES * 2U;

    rangeProcDpuCfg.hwRes.fftOut1D = fftOut1DScratch;
    rangeProcDpuCfg.hwRes.fftOut1DSize = sizeof(cmplx16ImRe_t) * MAX_1D_RANGEBINS * MAX_NUM_RX_ANTENNA * 2U;

    /* radar cube */
    rangeProcDpuCfg.hwRes.radarCube.data  = (cmplx16ImRe_t *) &radarCube[0];
    rangeProcDpuCfg.hwRes.radarCube.dataSize =sizeof(cmplx16ImRe_t) *  MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME;
    /* DC removal calibration buffers */
    rangeProcDpuCfg.hwRes.dcRangeSigMean  = (cmplx32ImRe_t *) &dcRangeSigMean[0];
    rangeProcDpuCfg.dynCfg.calibDcRangeSigCfg = &gCalibDcConfig;
    pHwConfig->dcRangeSigMeanSize = MAX_NUM_TX_ANTENNA * MAX_NUM_RX_ANTENNA * 32 * 8; //complex is 32bitsI/32bitsQ, maximum 32 bits


}

/**
*  @b Description
*  @n
*        Testing Task
*/

void rangeProcDpuTest_Task(UArg arg0, UArg arg1)
{
    uint32_t i,j,k;
    uint32_t numDataReadIn;
    uint32_t numTests;
    uint32_t testIndx, testCount;
    char featureName[200]; 
    char testConfigInfo[200];
    //uint32_t testLoop; //outputfmt and interleave loop
    rangeProcTestConfig_t testConfig;
    int32_t  retVal = 0;
    //int32_t  errorCode = 0;
    DPU_RangeProc_OutParams outParms;
    uint16_t rxIdx;
    uint16_t numRX[3]={1,2,4};
    uint16_t adcSampleLoop;
    uint16_t numAdcSampleLoop;
    uint16_t numRxAntLoop;
    uint16_t numRxAnt;
    uint16_t chpEvtIdx, numChirpEvt;
    uint32_t numAlignedAdcSamples;
    uint16_t numLoops;

    finalResults = 1;
    
    System_printf("...... Initialization ...... \n");
    
    /* hwa, edma, and rangeprocDPU initialization*/
    rangeProcDpuTest_edmaInit();
    rangeProcDpuTest_dpuInit();
    /* config the range proc DPU*/
    rangeProcDpuTest_dpuCfg();

    /* start the test */
    fileId = fopen("testdata\\rangeprocdsp_test_data.bin", "rb");
    if (fileId == NULL)
    {
        printf("Error:  Cannot open rangeprocdsp_test_data.bin!\n");
        exit(0);
    }
   
    /* read in number of tests to be run */
    fread(&numDataReadIn, sizeof(uint32_t),1,fileId);
    /* 1Tx, insert 1 test between fft size, with even samples  (1,2, 4) rx */
    numTests = (numDataReadIn / 3 * 2 - 1)  * 3;
    /* 1Tx, for 4 rx, maximum fft size is 1024, */
    numTests += (2 * 2);
    /* 2Tx, insert 1 test between fft size, with even samples 2, 4 rx */
    numTests += (numDataReadIn / 3 * 2 - 1) * 2;
    /* 2Tx, for 4 rx, maximum fft size is 1024, */
    numTests += (2);
    /* 3Tx, insert 1 test between fft size, with even samples 4 rx*/
    numTests += ((numDataReadIn / 3) * 2 - 1);
   
    numTests = numTests * 2;  /* single chirp event, and 2 chirp event*/
   
    numTests += ((numDataReadIn / 3) * 2 - 1); //for 3 tx, add 3 chirp event 
    
    printf("\n...... %d Tests to be run .......\n\n", numTests);
    
    testCount = 0;
    /* process each test */
    for (testIndx = 0; testIndx < numDataReadIn;  testIndx++)
    {
        /* read in test config */
        fread(&testConfig.numTxAntennas, sizeof(uint32_t),1,fileId);
        fread(&testConfig.numRangeBins, sizeof(uint32_t),1,fileId);
        fread(&testConfig.numChirpsPerFrame, sizeof(uint32_t),1,fileId);
        
        testConfig.cmplxIQswapFlag = 0; /* for arm, not support IQ swap */
        
        System_printf("numTxAntennas = %d\r", testConfig.numTxAntennas);
        System_printf("numRangeBins = %d\r", testConfig.numRangeBins);  
        System_printf("numChirpsPerFrame = %d\n", testConfig.numChirpsPerFrame);
        System_printf("\n");
        
        numRxAnt = MAX_NUM_RX_ANTENNA;
        if (testConfig.numRangeBins == 2048)
            numRxAnt = 2; //for 2048 range bin, only 2 rx antenna data is generated
        /* read in all the adc samples for MAX_NUM_RX_ANTENNA, testConfig.numRangeBins testConfig.numChirpsPerFrame test*/
        for(i=0; i<testConfig.numChirpsPerFrame; i++)
           fread( (uint32_t *)&dataInBuffer[i * numRxAnt * testConfig.numRangeBins], sizeof( uint32_t ), numRxAnt *  testConfig.numRangeBins, fileId );

        if (testConfig.numRangeBins == 64)
            numAdcSampleLoop = 1;  //no insert
        else
            numAdcSampleLoop = 2; //insert 1 + fft size 

        if (testConfig.numRangeBins == 2048)
            numRxAntLoop = 4 - testConfig.numTxAntennas - 1;
        else
            numRxAntLoop = 4 - testConfig.numTxAntennas; 
            

        for (adcSampleLoop = 0; adcSampleLoop<numAdcSampleLoop; adcSampleLoop++) 
        {
            if (numAdcSampleLoop == 1)
                testConfig.numAdcSamples = testConfig.numRangeBins;
            else
            {   /* mod(4)=0,2, numRangeBins*/
                testConfig.numAdcSamples = testConfig.numRangeBins - (numAdcSampleLoop - 1 - adcSampleLoop) *2;
            }
             /* Window generation for DSP fft */
            test_genWindow(window1DCoef, 
                            testConfig.numAdcSamples, 
                            testConfig.numAdcSamples/2U,
                            MMWAVELIB_WIN_BLACKMAN);

            /* generate the reference for all rx antenna */
            for(i=0; i<testConfig.numChirpsPerFrame; i++) 
            {
                Test_1Dfft_refGen(testConfig.numAdcSamples, numRxAnt, testConfig.numRangeBins, &dataInBuffer[i * numRxAnt *  testConfig.numRangeBins], &fft1DOut_ref[i * numRxAnt * testConfig.numRangeBins]);
            }

            for (rxIdx = 0; rxIdx < numRxAntLoop; rxIdx++)
            {
                testConfig.numRxAntennas = numRX[ testConfig.numTxAntennas - 1 + rxIdx];

                System_printf("Test #%d start\r\n", testCount);
                testFeatureStatus = 1;    
                sprintf(featureName, "%s%d%s%d%s%d%s%d",
                    ":Tx", testConfig.numTxAntennas,
                    " Rx", testConfig.numRxAntennas,
                    " R", testConfig.numRangeBins,
                    " D", testConfig.numChirpsPerFrame);
 
                testConfig.rxChanInterleave = 0;  //no interleave
                testConfig.radarCubeLayoutFmt = 1; //only format 1

                /* set up the dpu configuration per test */
                //add the dc calibartion test here
                gCalibDcConfig.enabled = 1;
                gCalibDcConfig.negativeBinIdx = -15;
                gCalibDcConfig.positiveBinIdx = 16;          //maximum is 32
                gCalibDcConfig.numAvgChirps = testConfig.numChirpsPerFrame/testConfig.numTxAntennas;

                //add a loop for numChiprsPerChiprEven
                if (testConfig.numTxAntennas == 3)
                    numLoops = 3;
                else
                    numLoops = 2;

                for (chpEvtIdx = 0; chpEvtIdx < numLoops; chpEvtIdx++)
                {
                   
                    numChirpEvt = (chpEvtIdx + 1);   //1, or 2
                    
                    testConfig.numChirpEvents = numChirpEvt;
                    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.numChirpsPerChirpEvent = testConfig.numChirpEvents;

                    if (numChirpEvt < 3)
                        adcDataIn = adcDataInBuf;         //1, or 2 chirps event
                    else //more than 3 chirpts
                        adcDataIn = fft1DOut_ref;//use reference as adc in buffer

                    sprintf(testConfigInfo, "%s%d%s%d%s%d%s%d%s%d",
                        ":Tx", testConfig.numTxAntennas,
                        " Rx", testConfig.numRxAntennas,
                        " ADC", testConfig.numAdcSamples,
                        " ChpsEvt", testConfig.numChirpEvents,
                        " Chps", testConfig.numChirpsPerFrame
                        );

                    System_printf("Configuration : %s\n", testConfigInfo);
                    /* set up the dpu configuration per test */
                    Test_setProfile(&testConfig);

                    retVal = DPU_RangeProcDSP_config(rangeProcDpuHandle,
                        &rangeProcDpuCfg);

                    if (retVal < 0)
                    {
                        System_printf("DEBUG: fft1d config return error:%d \n", retVal);
                        DebugP_assert(0);
                    }

                    /* process chirps loop in the frame*/
                    //16 byte aligned, 4 uint32 symbols
                    numAlignedAdcSamples = (testConfig.numAdcSamples + 3) / 4 * 4;
                    for (i = 0; i < testConfig.numChirpsPerFrame / numChirpEvt; i++)
                    {
                        for (k = 0; k < numChirpEvt; k++)
                        {
                            /* read in one chirp data */
                            for (j = 0; j < testConfig.numRxAntennas; j++)
                            {
                               // memcpy((void *)&adcDataIn[k * testConfig.numRxAntennas * numAlignedAdcSamples + j * numAlignedAdcSamples], (void *)&dataInBuffer[(i * numChirpEvt + k) * numRxAnt *  testConfig.numRangeBins + j *testConfig.numRangeBins], sizeof(uint32_t)*testConfig.numAdcSamples);
                               memcpy((void *)&adcDataIn[j * numChirpEvt * numAlignedAdcSamples + k * numAlignedAdcSamples], (void *)&dataInBuffer[(i * numChirpEvt + k) * numRxAnt *  testConfig.numRangeBins + j *testConfig.numRangeBins], sizeof(uint32_t)*testConfig.numAdcSamples);
                               
                            }
                        }

                        Task_sleep(1);

                        memset((void *)&outParms, 0, sizeof(DPU_RangeProc_OutParams));

                        retVal = DPU_RangeProcDSP_process(rangeProcDpuHandle, &outParms);

                        System_printf("DPU processing time %d, wait time %d\n", outParms.stats.processingTime, outParms.stats.waitTime);

                    }/* end of chirp loop */
                    /* check the result */
                    System_printf("\n... DPU Finish, Check Cube data ....  : \n\n");
                    if (numChirpEvt>2) //need to regenerate the reference
                    {
                        for (i = 0; i<testConfig.numChirpsPerFrame; i++)
                        {
                            Test_1Dfft_refGen(testConfig.numAdcSamples, numRxAnt, testConfig.numRangeBins, &dataInBuffer[i * numRxAnt *  testConfig.numRangeBins], &fft1DOut_ref[i * numRxAnt * testConfig.numRangeBins]);
                        }
                    }
                    Test_printResults(&testConfig, 0); // , (char*)&testConfigInfo);

                    if (gCalibDcConfig.enabled)
                    {
                        /* process chirps loop in the frame*/
                        for (i = 0; i < testConfig.numChirpsPerFrame / numChirpEvt; i++)
                        {
                            for (k = 0; k < numChirpEvt; k++)
                            {
                                /* read in one chirp data */
                                for (j = 0; j < testConfig.numRxAntennas; j++)
                                {
                                   // memcpy((void *)&adcDataIn[k * testConfig.numRxAntennas * numAlignedAdcSamples + j * numAlignedAdcSamples], (void *)&dataInBuffer[(i * numChirpEvt + k) * numRxAnt *  testConfig.numRangeBins + j *testConfig.numRangeBins], sizeof(uint32_t)*testConfig.numAdcSamples);
                                    memcpy((void *)&adcDataIn[j * numChirpEvt * numAlignedAdcSamples + k * numAlignedAdcSamples], (void *)&dataInBuffer[(i * numChirpEvt + k) * numRxAnt *  testConfig.numRangeBins + j *testConfig.numRangeBins], sizeof(uint32_t)*testConfig.numAdcSamples);
                                }
                            }
                            Task_sleep(1);

                            memset((void *)&outParms, 0, sizeof(DPU_RangeProc_OutParams));

                            retVal = DPU_RangeProcDSP_process(rangeProcDpuHandle, &outParms);

                            System_printf("DPU processing time %d, wait time %d\n", outParms.stats.processingTime, outParms.stats.waitTime);

                        }/* end of chirp loop */
                         /* check the result */
                        System_printf("\n... DPU Finish, Check Calibration data ....  : \n\n");
                        if (numChirpEvt>2) //need to regenerate the reference
                        {
                            for (i = 0; i<testConfig.numChirpsPerFrame; i++)
                            {
                                Test_1Dfft_refGen(testConfig.numAdcSamples, numRxAnt, testConfig.numRangeBins, &dataInBuffer[i * numRxAnt *  testConfig.numRangeBins], &fft1DOut_ref[i * numRxAnt * testConfig.numRangeBins]);
                            }
                        }
                        Test_printResults(&testConfig, 1); // , (char*)&testConfigInfo);

                    }
                    System_printf("Test #%d finished!\n\r", testCount);
                    testCount++;

                }
                
            }
            if (testFeatureStatus == 1)
            {
                MCPI_setFeatureTestResult(featureName, MCPI_TestResult_PASS);
            }
            else
            {
                MCPI_setFeatureTestResult(featureName, MCPI_TestResult_FAIL);
            }

            System_printf("\n");
        }
    }
    System_printf("All %d Tests finished!\n\r", testCount);

    if (finalResults == 0)
    {
        System_printf("......TEST FAILS......\n");
    }
    else
    {
        System_printf("......TEST PASSES......\n");
    }

    fclose(fileId);
    
    /*delete the handler*/
    EDMA_close(edmaHandle);
    DPU_RangeProcDSP_deinit(rangeProcDpuHandle);
    MCPI_setTestResult ();

    /* Exit BIOS */
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
int main (void)
{
    Task_Params taskParams;
    int32_t         errCode;
    SOC_Cfg         socCfg;
    
#ifdef SUBSYS_MSS
    ESM_init(0U);
#endif
    /* Initialize test logger */
    MCPI_Initialize();
    
    
    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        //System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        DebugP_assert (0);
        return -1;
    }
    
    Cycleprofiler_init();
    
    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 3*1024;
    Task_create(rangeProcDpuTest_Task, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    
    return 0;
}
