/**
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the EDMA + HWA drivers.
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
#include <ti/sysbios/family/arm/v7a/Pmu.h>

#include <ti/common/sys_common.h>
#include <ti/utils/testlogger/logger.h>

#include <ti/drivers/esm/esm.h>
#define DebugP_ASSERT_ENABLED 1
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/edma/edma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpu/rangeproc/rangeprochwa.h>
//#include <ti/utils/cycleprofiler/cycle_profiler.h>

#define  MAX_NUM_RANGEBIN   1024
#define  MAX_NUM_RX_ANTENNA 4
#define  MAX_NUM_ADCSAMPLE_PERCHIRP 4096   // MAX_NUM_RANGEBIN *  MAX_NUM_RX_ANTENNA
#define  MAX_NUM_TX_ANTENNA 3
#define  MAX_NUM_CHIRPS_PERFRAME  6       // for tx=3, number of chirps = 6, otherwise = 4     
#define  MAX_ALLOWED_ERROR 8

/* data memeory */
#pragma DATA_SECTION(dataInBuffer, ".dataBuf");
uint32_t  dataInBuffer[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME]; /*to save all adc buffer data */
#ifdef SUBSYS_MSS
#pragma DATA_SECTION(adcDataIn, ".dataBuf");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(adcDataIn, ".dataBufL2");
#endif
uint32_t  adcDataIn[MAX_NUM_ADCSAMPLE_PERCHIRP]; /*adc buffer for one chirp*/

#ifdef SUBSYS_MSS
#pragma DATA_SECTION(window1DCoef, ".dataBuf");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(window1DCoef, ".dataBufL2");
#endif
#pragma DATA_ALIGN(window1DCoef,8);
uint32_t  window1DCoef[MAX_NUM_RANGEBIN];  //hwa 18 bits, whole length
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(dcRangeSigMean, ".dataBufL2");
#endif
#ifdef SUBSYS_MSS
#pragma DATA_SECTION(dcRangeSigMean, ".dataBuf");
#endif
cmplx32ImRe_t dcRangeSigMean[MAX_NUM_TX_ANTENNA * MAX_NUM_RX_ANTENNA * 32];


#pragma DATA_SECTION(radarCube, ".l3data");
cmplx16ImRe_t radarCube[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME];
#pragma DATA_SECTION(fft1DOut_ref, ".l3data");
uint32_t fft1DOut_ref[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME]; /* fixed to 16I and 16Q */
#ifdef SUBSYS_MSS
#pragma DATA_SECTION(dataWinOut, ".dataBuf");
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(dataWinOut, ".dataBufL2");
#endif
uint32_t dataWinOut[MAX_NUM_RANGEBIN]; /* per rx antenna, per chirp for windowing results */


typedef struct rangeProcTestConfig_t_ {
    uint32_t numTxAntennas;
    uint32_t numRxAntennas;
    uint32_t numAdcSamples;
    uint32_t numRangeBins;
    uint32_t numChirpsPerFrame;
   // uint32_t numDopplerBins;
    uint8_t  cmplxIQswapFlag;    
    uint8_t  rxChanInterleave;
    uint8_t  radarCubeLayoutFmt;
} rangeProcTestConfig_t;

#define  MMW_HWA_WINDOWRAM_1D_OFFSET         0

/* EDMA resource partitioning */
#define EDMA_SHADOW_LNK_PARAM_BASE_ID       EDMA_NUM_DMA_CHANNELS

/* 1D Input */
#define MMW_EDMA_1DIN_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 4)
#define MMW_EDMA_1DINSIGNATURE_CH_ID (EDMA_TPCC0_REQ_FREE_2)
#define MMW_EDMA_1DINSIGNATURE_PING_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 5)

/* 1D -ping */
#define MMW_EDMA_1D_PING_CH_ID              EDMA_TPCC0_REQ_HWACC_0//EDMA_TPCC0_REQ_HWACC_0
#define MMW_EDMA_1D_PING_SHADOW_LINK_CH_ID  EDMA_SHADOW_LNK_PARAM_BASE_ID
#define MMW_EDMA_1D_PING_CHAIN_CH_ID            EDMA_TPCC0_REQ_FREE_0
#define MMW_EDMA_1D_PING_ONE_HOT_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 2)

/* 1D - pong */
#define MMW_EDMA_1D_PONG_CH_ID               EDMA_TPCC0_REQ_HWACC_1
#define MMW_EDMA_1D_PONG_SHADOW_LINK_CH_ID  (EDMA_SHADOW_LNK_PARAM_BASE_ID + 1)

uint8_t     pingChan[3] = 
{
    EDMA_TPCC0_REQ_FREE_8,
    EDMA_TPCC0_REQ_FREE_9,
    EDMA_TPCC0_REQ_FREE_10
};
uint8_t     pingDummyChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 7,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 8,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 9
};
uint8_t     pingChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 10,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 11,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 12
};
uint8_t     pongChan[3] = 
{
    EDMA_TPCC0_REQ_FREE_11,
    EDMA_TPCC0_REQ_FREE_12,
    EDMA_TPCC0_REQ_FREE_13
};
uint8_t     pongDummyChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 13,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 14,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 15
};
uint8_t     pongChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 16,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 17,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 18
};


#define PI_ 3.14159265

 
DPU_RangeProc_CalibDcRangeSigCfg gCalibDcConfig=
{
    0,  //disabled
    -5, // negativeBinIdx
    5,  //positiveBinIdx
    128 //number of average chirps
};
 
SOC_Handle          socHandle;
HWA_Handle          hwaHandle;
EDMA_Handle         edmaHandle;
/*! @brief   EDMA error Information when there are errors like missing events */
EDMA_errorInfo_t  EDMA_errorInfo;
/*! @brief EDMA transfer controller error information. */
EDMA_transferControllerErrorInfo_t EDMA_transferControllerErrorInfo;

DPU_RangeProcHWA_Handle rangeProcDpuHandle;
DPU_RangeProcHWA_Config rangeProcDpuCfg;

uint8_t testFeatureStatus;

uint8_t checkResultFlag;

uint8_t finalResults ;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);


FILE * fileId;   
 
//volatile cycleLog_t gCycleLog;

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
    //DebugP_assert(0);
    checkResultFlag = 0;
   // System_printf("EDMA Run Time error\n");
}

/**
*  @b Description
*  @n
*    Based on the test configuration, set up the range processing DPU configurations
*/
void Test_setProfile(rangeProcTestConfig_t * testConfig)
{
    DPU_RangeProcHWA_HW_Resources  *pHwConfig;
    DPU_RangeProcHWA_StaticConfig  * params;
    uint32_t index;
    uint32_t      bytesPerRxChan;
    
    pHwConfig = &rangeProcDpuCfg.hwRes; 
    params = &rangeProcDpuCfg.staticCfg; 
    
    /* overwrite the DPIF_commonParams with the test configuration*/
    params->numTxAntennas = testConfig->numTxAntennas;
    params->numVirtualAntennas = testConfig->numTxAntennas * testConfig->numRxAntennas;
    params->numRangeBins = testConfig->numRangeBins;
    params->numChirpsPerFrame = testConfig->numChirpsPerFrame;
    /* windowing */
    params->windowSize = sizeof(uint32_t) * ((testConfig->numAdcSamples +1 ) / 2); //hwa is 18 bits, whole length
    params->ADCBufData.dataSize = testConfig->numAdcSamples * testConfig->numRxAntennas * 4 ;  // Jennifer: TODO, be aligned size
    params->ADCBufData.dataProperty.numAdcSamples = testConfig->numAdcSamples;
    params->ADCBufData.dataProperty.numRxAntennas = testConfig->numRxAntennas;
    
  
    if (testConfig->rxChanInterleave)
    {
        //bytesPerRxChan = testConfig->numAdcSamples * sizeof(uint32_t);
        //bytesPerRxChan = (bytesPerRxChan + 15) / 16 * 16;        
        bytesPerRxChan = 0;
        for (index = 0; index < testConfig->numRxAntennas; index++)
        {
            params->ADCBufData.dataProperty.rxChanOffset[index] = index * bytesPerRxChan;
        }


        params->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_INTERLEAVE_MODE;
        
        /* edma set up*/
        /* Output Ping*/
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel  = MMW_EDMA_1D_PING_CH_ID;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channelShadow   = MMW_EDMA_1D_PING_SHADOW_LINK_CH_ID;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.eventQueue= 0;

        /* Output Pong*/
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel         = MMW_EDMA_1D_PONG_CH_ID;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channelShadow   = MMW_EDMA_1D_PONG_SHADOW_LINK_CH_ID;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.eventQueue       = 0;
        /* Output signature channel */
        pHwConfig->edmaOutCfg.dataOutSignature.channel  = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        pHwConfig->edmaOutCfg.dataOutSignature.channelShadow = MMW_EDMA_1D_PING_ONE_HOT_SHADOW_LINK_CH_ID;
        pHwConfig->edmaOutCfg.dataOutSignature.eventQueue = 0;
       
    }
    else
    {
        bytesPerRxChan = testConfig->numAdcSamples * sizeof(uint32_t);
        bytesPerRxChan = (bytesPerRxChan + 15) / 16 * 16;

        for (index = 0; index < testConfig->numRxAntennas; index++)
        {
            params->ADCBufData.dataProperty.rxChanOffset[index] = index * bytesPerRxChan;
        }

        params->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_NON_INTERLEAVE_MODE;
        /* Data Input EDMA */
        pHwConfig->edmaInCfg.dataIn.channel         = EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL;
        pHwConfig->edmaInCfg.dataIn.channelShadow   = MMW_EDMA_1DIN_SHADOW_LINK_CH_ID;
        pHwConfig->edmaInCfg.dataIn.eventQueue      = 0;
        pHwConfig->edmaInCfg.dataInSignature.channel         = MMW_EDMA_1DINSIGNATURE_CH_ID;
        pHwConfig->edmaInCfg.dataInSignature.channelShadow   = MMW_EDMA_1DINSIGNATURE_PING_SHADOW_LINK_CH_ID;
        pHwConfig->edmaInCfg.dataInSignature.eventQueue      = 0;

        if((testConfig->numTxAntennas == 3)&& (testConfig->radarCubeLayoutFmt == 1))
        {

            /* PING */
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.channel         = MMW_EDMA_1D_PING_CH_ID;
            for(index = 0; index< 3; index++)
            {
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.channelShadow[index]   = pingDummyChanShadow[index];
            }
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.eventQueue= 0;

            for(index = 0; index< 3; index++)
            {
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].channel = pingChan[index];
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].channelShadow = pingChanShadow[index];
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].eventQueue = 0;
            }

            /* PONG */
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.channel         = MMW_EDMA_1D_PONG_CH_ID;
            for(index = 0; index< 3; index++)
            {
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.channelShadow[index]   = pongDummyChanShadow[index];
            }
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.eventQueue       = 0;

            for(index = 0; index< 3; index++)
            {
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].channel = pongChan[index];
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].channelShadow = pongChanShadow[index];
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].eventQueue = 0;
            }
        }
        else
        {
            /* Output Ping*/
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel         = MMW_EDMA_1D_PING_CH_ID;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channelShadow   = MMW_EDMA_1D_PING_SHADOW_LINK_CH_ID;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.eventQueue= 0;
            /* Output Pong*/
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel         = MMW_EDMA_1D_PONG_CH_ID;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channelShadow   = MMW_EDMA_1D_PONG_SHADOW_LINK_CH_ID;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.eventQueue       = 0;
            
        }
        /* Output signature channel */
        pHwConfig->edmaOutCfg.dataOutSignature.channel  = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        pHwConfig->edmaOutCfg.dataOutSignature.channelShadow = MMW_EDMA_1D_PING_ONE_HOT_SHADOW_LINK_CH_ID;
        pHwConfig->edmaOutCfg.dataOutSignature.eventQueue = 0;
    }
        
   
    /* radar cube*/
    //only 1 tx is transmitted at one chirp 
    pHwConfig->radarCube.dataSize = testConfig->numRangeBins * testConfig->numRxAntennas * sizeof(uint32_t) * testConfig->numChirpsPerFrame;
    if (testConfig->radarCubeLayoutFmt == 0) 
        pHwConfig->radarCube.datafmt = DPIF_RADARCUBE_FORMAT_2;  
    else
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
            ptrCubeRef = &fft1DOut_ref[chirpIdx * testConfig->numRangeBins * MAX_NUM_RX_ANTENNA + jj * testConfig->numRangeBins];
            
            for (kk = 0; kk < testConfig->numRangeBins; kk++)
            {
               
                if (testConfig->radarCubeLayoutFmt==0)  //rx, tx, chirp, range
                {   
                    //if (testConfig->numTxAntennas == 1)
                    radarCubeIdx = kk * testConfig->numRxAntennas * testConfig->numChirpsPerFrame + chirpIdx * testConfig->numRxAntennas + jj;  
                    
                }
                else if (testConfig->radarCubeLayoutFmt==1) 
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
                
               // System_printf("%d,%d\n",radarCube[radarCubeIdx].real, radarCube[radarCubeIdx].imag );
                realValueRef = (int16_t)(ptrCubeRef[kk] & 0x0000FFFF);   //fixed the reference output as Imag16/Real16
                imagValueRef = (int16_t)(ptrCubeRef[kk] >> 16);
                
                if (flag == 1)
                {
                    if ((kk <= gCalibDcConfig.positiveBinIdx) || (kk>= testConfig->numRangeBins + gCalibDcConfig.negativeBinIdx))
                    {
                        //remove the mean from previous frame
                        sumReal = 0;
                        sumImag = 0;
                        for (ii = 0; ii < testConfig->numChirpsPerFrame / testConfig->numTxAntennas; ii++)
                        {

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
                if (((radarCube[radarCubeIdx].real - realValueRef) > MAX_ALLOWED_ERROR) || ((radarCube[radarCubeIdx].real  - realValueRef) < -MAX_ALLOWED_ERROR))
                {
                    testStatus = 0;
                    testFeatureStatus = 0;
                    finalResults = 0;
                    //System_printf("radar cube index %d\n", radarCubeIdx);
                    System_printf(" test fail at chirp %d, rx antenna %d, bin %d, cube=%d, ref=%d\n ", chirpIdx, jj, kk, radarCube[radarCubeIdx].real, realValueRef );
                }
                if (((radarCube[radarCubeIdx].imag - imagValueRef) > MAX_ALLOWED_ERROR) || ((radarCube[radarCubeIdx].imag - imagValueRef) < -MAX_ALLOWED_ERROR))
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
/**
*  @b Description
*  @n
*    Generate the windowing coefficients, only support BLACKMAN
*/
void Test_1DwindowCoef_gen(uint32_t numAdcSamples)
{
    float ephyR, ephyI;
    float phi = 2 * PI_ / ((float) numAdcSamples - 1);
    
    
    uint32_t ii;
    float ephyR2, ephyI2;
    float a0, a1, a2;
    float winValue;
    int32_t winValueFixed;
    float cosValue, sinValue, cosValue2, sinValue2;
    float temp;
    float initR, initI;
    
    a0 = 0.42;
    a1 = 0.5;
    a2 = 0.08;
    
    cosValue = 1.f;
    sinValue = 0.f; 
    cosValue2 = 1.f;
    sinValue2 = 0.f;

    initR  = cos(phi);
    initI  = sin(phi);
    ephyR = 1.f - (initI * initI) * 2.f;     /* cos(2a)=1-2*sin(a)^2 */
    ephyI = 2.f * initR * initI;             /* sin(2a)=2*sin(a)*cos(a) */
    /* cos (4*pi*2/(N-1)), sin(4*pi*2/(N-1) */
    ephyR2 = 1.f - (initI * initI) * (initR * initR) * 8.f;     /* cos(4a)=1-8*cos(a)^2*sin(a)^2 */
    ephyI2 = 4.f * initR * initI * (1 - 2.f * initI * initI);     /* sin(4a)=4*sin(a)*cos(a) *(1-2*sin(a)^2) */
        
    /* window is whole length for hwa*/
    for (ii = 0; ii < numAdcSamples; ii++)
    {
        winValue = a0 - a1 * cosValue + a2 * cosValue2 ;
        winValue = winValue * (1<<17) + 0.5;
        winValueFixed = (uint32_t) winValue;
        if (winValueFixed >= (1<<17))
        {
            winValueFixed = (1<<17) - 1;
        }
        temp = cosValue;
        cosValue = cosValue * ephyR - sinValue * ephyI;
        sinValue = temp * ephyI + sinValue * ephyR;
        
        temp = cosValue2;
        cosValue2 = cosValue2 * ephyR2 - sinValue2 * ephyI2;
        sinValue2 = temp * ephyI2 + sinValue2 * ephyR2;
        //window1DCoef[ii] = (1<<17) - 1; //rec window, for debug
        window1DCoef[ii] = winValueFixed;
    }

    
}

/**
*  @b Description
*  @n
*    FFT calculation, with output fixed to 32 bits, with Imag16/Real16
*/
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
    
    /* hard code all the scale, math with hwa fft results  */
    if (fftSize == 64)
       divValue = 4.f;
    else if (fftSize==128)
        divValue = 4.f;
    else if (fftSize == 256)
        divValue = 4.f;  //scale with hwa fft
    else if (fftSize == 512)
        divValue = 4.f;
    else if (fftSize == 1024)
        divValue = 4.f;
    else
        divValue = 4.f;
    //in 32 bits input, Real16/imag16
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
        dataOut[ii] += (outImag << 16);  
    }
}
/**
*  @b Description
*  @n
*    prepare the adc samples with interleave format
*/
void Test_channelInterleave(uint32_t * dataArray, uint32_t numRxAnt, uint32_t numAdcSamples)
{
    uint32_t antIdx;
    uint32_t leftIdx, rightIdx, index, rightIdx1[3]; //maximum is 4 antenna
    uint32_t temp;
    
    leftIdx = 1;
    rightIdx1[0] = 0;
    rightIdx1[1] = 0;
    rightIdx1[2] = 0;
    while (leftIdx < rightIdx1[0] + numAdcSamples)
    {
        for (antIdx = 1; antIdx < numRxAnt; antIdx++)
        {
            rightIdx = antIdx * numAdcSamples + rightIdx1[antIdx - 1]  ;
            for ( index = rightIdx; index > leftIdx; index--)  
            {
                temp = dataArray[index];
                dataArray[index] = dataArray[index - 1];
                dataArray[index - 1] = temp;
            }
            
            leftIdx++;
        }
        leftIdx++; //skip antenna 0 
        for (antIdx = 1; antIdx < numRxAnt; antIdx++)
        {
            rightIdx1[antIdx - 1] += (numRxAnt - antIdx);
        }
    }
    
}
/**
*  @b Description
*  @n
*    Generate the reference for radar cuba result check.
*/
void Test_1Dfft_refGen( uint32_t numAdcSamples, uint32_t numRxAnt, uint32_t fftSize, uint32_t * dataInPtr, uint32_t * fftOutPtr, uint8_t symWin)
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
        if(symWin == 1)
        {
            /* windowing */
            for (kk = 0; kk < numAdcSamples/2; kk++)
            {
                winCoef = (int32_t) window1DCoef[kk];
                dataSample = dataInPtr[kk];
                
                realValue =  (int16_t) (dataSample&0x0000FFFF);
                tempValue = (realValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
                if (tempValue >= (1<<15))
                    dataWinOut[kk] = 0x7fff;
                else
                    dataWinOut[kk] = (uint16_t) tempValue;
                
                imagValue = (int16_t) (dataSample>>16);
                tempValue = (imagValue * winCoef + 0x10000) >> 17;
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
                tempValue = (realValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
                if (tempValue >= (1<<15))
                    dataWinOut[kk] = 0x7fff;
                else
                    dataWinOut[kk] = (uint16_t) tempValue;
                
                imagValue = (int16_t) (dataSample>>16);
                tempValue = (imagValue * winCoef + 0x10000) >> 17;
                if (tempValue >= (1<<15))
                     dataWinOut[kk] += ( 0x7fff <<16);
                else
                     dataWinOut[kk] +=  ( ((uint16_t)tempValue) << 16);
            }
        
        }
        else
        {
            /* windowing */
            for (kk = 0; kk < numAdcSamples; kk++)
            {
                winCoef = (int32_t) window1DCoef[kk];
                dataSample = dataInPtr[kk];
                
                realValue =  (int16_t) (dataSample&0x0000FFFF);
                tempValue = (realValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
                if (tempValue >= (1<<15))
                    dataWinOut[kk] = 0x7fff;
                else
                    dataWinOut[kk] = (uint16_t) tempValue;
                
                imagValue = (int16_t) (dataSample>>16);
                tempValue = (imagValue * winCoef + 0x10000) >> 17;
                if (tempValue >= (1<<15))
                     dataWinOut[kk] += ( 0x7fff <<16);
                else
                     dataWinOut[kk] +=  ( ((uint16_t)tempValue) << 16);

            }
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

void rangeProcDpuTest_hwaInit()
{
    int32_t             errorCode = 0;
    
    HWA_init();
    hwaHandle = HWA_open(0, socHandle, &errorCode);
    if (hwaHandle == NULL)
    {
        System_printf("Error: Unable to open the HWA Instance err:%d\n",errorCode);
        DebugP_assert (0);
        return;
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
    DPU_RangeProcHWA_InitParams initParams;
    initParams.hwaHandle =  hwaHandle;
    /* generate the dpu handler*/
    rangeProcDpuHandle =  DPU_RangeProcHWA_init (&initParams, &errorCode);
    if (rangeProcDpuHandle == NULL)
    {
        System_printf ("Debug: RangeProc DPU initialization returned error %d\n", errorCode);
        DebugP_assert (0);
        return;
    }
}

/**
*  @b Description
*  @n
*        set up the range processing DPU configurations, do not change for each test 
*/
void rangeProcDpuTest_dpuCfg()
{
    DPU_RangeProcHWA_HW_Resources * pHwConfig;
    
    memset((void *)&rangeProcDpuCfg, 0, sizeof(DPU_RangeProcHWA_Config));
        
    
    /* hwi configuration */
    pHwConfig = &rangeProcDpuCfg.hwRes; 
        
    /* HWA configurations, not related to per test, common to all test */
    pHwConfig->hwaCfg.paramSetStartIdx = 5;
    pHwConfig->hwaCfg.numParamSet = DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS;
    pHwConfig->hwaCfg.hwaWinRamOffset  = MMW_HWA_WINDOWRAM_1D_OFFSET; 
    pHwConfig->hwaCfg.hwaWinSym = 1;
    pHwConfig->hwaCfg.dataInputMode = DPU_RangeProcHWA_InputMode_ISOLATED;

    /* edma configuration */
    pHwConfig->edmaHandle  = edmaHandle;  
    /* edma configuration depends on the interleave or non-interleave */
    
    /* windowing buffer is fixed, size will change*/
    rangeProcDpuCfg.staticCfg.window =  (int32_t *)&window1DCoef[0];
    
    /* adc buffer buffer, format fixed, interleave, size will change */
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_IMRE;
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.adcBits = 2U;
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.numChirpsPerChirpEvent = 1U;
    rangeProcDpuCfg.staticCfg.ADCBufData.data = (void *) adcDataIn;
    
    /* radar cube */
    rangeProcDpuCfg.hwRes.radarCube.data  = (cmplx16ImRe_t *) &radarCube[0];
    
    /* FFT1D calibration buffers */
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
    uint32_t i,j;
    uint32_t numDataReadIn;
    uint32_t numTests;
    uint32_t testIndx, testCount;
    char featureName[200]; 
    char testConfigInfo[200];
    uint32_t testLoop; //outputfmt and interleave loop
    rangeProcTestConfig_t testConfig;
    int32_t  retVal = 0;
    int32_t  errorCode = 0;
    DPU_RangeProcHWA_OutParams outParms;
    uint16_t rxIdx;
    uint16_t numRX[3]={1,2,4};
    uint16_t adcSampleLoop;
    uint16_t numAdcSampleLoop;
    uint16_t chipIdx;
    

    finalResults = 1;
    
    System_printf("...... Initialization ...... \n");
    
    /* hwa, edma, and rangeprocDPU initialization*/
    rangeProcDpuTest_hwaInit();
    rangeProcDpuTest_edmaInit();
    rangeProcDpuTest_dpuInit();
    /* config the range proc DPU*/
    rangeProcDpuTest_dpuCfg();

    /* start the test */
    fileId = fopen("testdata\\rangeprochwa_test_data.bin", "rb");
    if (fileId == NULL)
    {
        printf("Error:  Cannot open rangeprochwa_test_data.bin !\n");
        exit(0);
   }
   
    /* read in number of tests to be run */
    fread(&numDataReadIn, sizeof(uint32_t),1,fileId);
    /* 1Tx, insert 3 test between fft size,mod(4)=1,2,3,  (1,2,4) rx */
    numTests = ((numDataReadIn / 3) + (((numDataReadIn / 3) - 1) * 3)) * 3;
    /* 2Tx, insert 3 test between fft size, mod(4)=1,2,3, (2,4) rx */
    numTests += ((numDataReadIn / 3) + (((numDataReadIn / 3) - 1) * 3)) * 2;
    /* 3Tx, insert 3 test between fft size, mod(4)=1,2,3, 4 rx*/
    numTests += ((numDataReadIn / 3) + (((numDataReadIn / 3) - 1) * 3)) * 1;
   
    numTests = numTests * 3; // noninterleave, fmt0,1, interleave fmt 0
    
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
        
        
        /* read in all the adc samples MAX_NUM_RX_ANTENNA * testConfig.numRangeBins*testConfig.numChirpsPerFrame per test*/
        for(i=0; i<testConfig.numChirpsPerFrame; i++)
           fread( (uint32_t *)&dataInBuffer[i * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins], sizeof( uint32_t ), MAX_NUM_RX_ANTENNA *  testConfig.numRangeBins, fileId );
        
        if (testConfig.numRangeBins == 64)
            numAdcSampleLoop = 1;  //no insert
        else
            numAdcSampleLoop = 4; //insert 3 + fft size 
        for (adcSampleLoop = 0; adcSampleLoop<numAdcSampleLoop; adcSampleLoop++) 
        {
            if (numAdcSampleLoop == 1)
                testConfig.numAdcSamples = testConfig.numRangeBins;
            else
            {   /* mod(4)=1,2,3, numRangeBins*/
                testConfig.numAdcSamples = testConfig.numRangeBins - (numAdcSampleLoop - 1 - adcSampleLoop) * 4 + (numAdcSampleLoop - 1 - adcSampleLoop);
            }
            /* generate the 1D window coefficients */
            Test_1DwindowCoef_gen(testConfig.numAdcSamples);
            /* generate the reference for all rx antenna  */
            for(i=0; i<testConfig.numChirpsPerFrame; i++) 
            {
                Test_1Dfft_refGen(testConfig.numAdcSamples, MAX_NUM_RX_ANTENNA, testConfig.numRangeBins, &dataInBuffer[i * MAX_NUM_RX_ANTENNA *  testConfig.numRangeBins], &fft1DOut_ref[i * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins], 1);
            }    
        
            
            for (rxIdx = 0; rxIdx < (4 - testConfig.numTxAntennas); rxIdx++)
            {
                testConfig.numRxAntennas = numRX[ testConfig.numTxAntennas - 1 + rxIdx];
                
                //System_printf("numRxAntennas = %d\r", testConfig.numRxAntennas);  
                testFeatureStatus = 1;    
                sprintf(featureName, "%s%d%s%d%s%d%s%d",
                    ":Tx", testConfig.numTxAntennas,
                    " Rx", testConfig.numRxAntennas,
                    " R", testConfig.numRangeBins,
                    " D", testConfig.numChirpsPerFrame);
 
                for (testLoop = 0; testLoop < 3; testLoop++) //for output layout, and interleave,noninterleave loop
                {

                    testConfig.rxChanInterleave = testLoop >> 1;
                    testConfig.radarCubeLayoutFmt = testLoop&0x1;

                    if((testConfig.numRangeBins == 1024) &&
                      (testConfig.numRxAntennas == 4U) &&
                      (testConfig.numTxAntennas == 1U) &&
                      (testConfig.radarCubeLayoutFmt==1U))
                    {
                        System_printf("Test #%d NOT RUN, KNOWN ISSUE!\n\r", testCount);
                        testCount++;
                        continue;
                    }

                    /* set up the dpu configuration per test */
                    //add the dc calibartion test here
                    gCalibDcConfig.enabled = 1;
                    gCalibDcConfig.negativeBinIdx = -15;
                    gCalibDcConfig.positiveBinIdx = 16;          //maximum is 32
                    gCalibDcConfig.numAvgChirps = testConfig.numChirpsPerFrame/testConfig.numTxAntennas;

                    
                    System_printf("\nTest #%d  start\r\n", testCount);
                    
                    sprintf(testConfigInfo, "%s%d%s%d%s%d%s%d%s%d%s%d",
                        ":Tx", testConfig.numTxAntennas,
                        " Rx", testConfig.numRxAntennas,
                        " ADC", testConfig.numAdcSamples,
                        " Chps", testConfig.numChirpsPerFrame,
                       // " IQSwap", testConfig.cmplxIQswapFlag,
                        " IntLv", testConfig.rxChanInterleave,
                        " OutFmt", testConfig.radarCubeLayoutFmt);

                    System_printf("Configuration : %s\n", testConfigInfo);
                    
                    
                    Test_setProfile(&testConfig);
            
                    checkResultFlag = 1;
                    
                    
                    retVal = DPU_RangeProcHWA_config (rangeProcDpuHandle,
                                                      &rangeProcDpuCfg);

                    if(retVal < 0)
                    {
						if (retVal == DPU_RANGEPROCHWA_ENOTIMPL)
						{
							System_printf("NOT SUPPORTED!\n");
							System_printf("Test #%d finished!\n\r", testCount);
							testCount++;
							continue;
						}
						else
						{
							System_printf("DEBUG: fft1d config return error:%d \n", retVal);
							DebugP_assert(0);
						}
                    }
    
                    /* control the rangeproc hwa*/
                   retVal = DPU_RangeProcHWA_control(rangeProcDpuHandle, DPU_RangeProcHWA_Cmd_triggerProc, NULL, 0);
                   if(retVal < 0)
                   {
                       /* Not Expected */
                       System_printf("RangeProc DPU control error %d\n", retVal);
                       DebugP_assert(0);
                   }

                    /* process chirps loop in the frame*/
                    for(i=0; i< testConfig.numChirpsPerFrame ; i++)
                    {
                        chipIdx = i;
                        /* read in one chirp data */
                        for (j = 0; j < testConfig.numRxAntennas; j++)
                        {
                            //16 byte aligned, 4 uint32 symbols
                            memcpy((void *)&adcDataIn[j * ((testConfig.numAdcSamples+3)/4*4)], (void *)&dataInBuffer[chipIdx * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins + j *testConfig.numRangeBins], sizeof(uint32_t)*testConfig.numAdcSamples);
                        }
                        if (testConfig.rxChanInterleave)
                        {
                            /* end of rx antenna, data is aligned at 4 samples */
                            Test_channelInterleave(adcDataIn, testConfig.numRxAntennas, (testConfig.numAdcSamples+3)/4*4);
                        }

                        errorCode = EDMA_startTransfer(edmaHandle, EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL, EDMA3_CHANNEL_TYPE_DMA);
                        if (errorCode != 0)
                        {
                            System_printf("Error: EDMA start Transfer returned %d\n",errorCode);
                            return;
                        }
                        Task_sleep(1);
                    }/* end of chirp loop */
                    memset((void *)&outParms, 0, sizeof(DPU_RangeProcHWA_OutParams));

                    retVal = DPU_RangeProcHWA_process(rangeProcDpuHandle, &outParms);
                    //System_printf("DPU processing time %d, wait time %d\n", outParms.stats.processingTime, outParms.stats.waitTime);
                  
                   /* check the result */
                    System_printf("\n... DPU Finish, Check Cube data ....  : \n\n");
                    if (checkResultFlag == 1)
                    {
                        
                        Test_printResults(&testConfig, 0); // , (char*)&testConfigInfo);
                    }
                    else  //hard code to fail, if running into edma run time error
                    {
                        System_printf("Test Cube Data FAILED!\n");
                        testFeatureStatus = 0;
                        finalResults = 0;
                    }


                    if (gCalibDcConfig.enabled)
                    {
                        /* control the rangeproc hwa*/
                        retVal = DPU_RangeProcHWA_control(rangeProcDpuHandle, DPU_RangeProcHWA_Cmd_triggerProc, NULL, 0);
                        if (retVal < 0)
                        {
                            /* Not Expected */
                            System_printf("RangeProc DPU control error %d\n", retVal);
                            DebugP_assert(0);
                        }

                        for (i = 0; i< testConfig.numChirpsPerFrame; i++)
                        {
                            chipIdx = i;
                            /* read in one chirp data */
                            for (j = 0; j < testConfig.numRxAntennas; j++)
                            {
                                //16 byte aligned, 4 uint32 symbols
                                memcpy((void *)&adcDataIn[j * ((testConfig.numAdcSamples + 3) / 4 * 4)], (void *)&dataInBuffer[chipIdx * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins + j *testConfig.numRangeBins], sizeof(uint32_t)*testConfig.numAdcSamples);
                            }
                            if (testConfig.rxChanInterleave)
                            {
                                /* end of rx antenna, data is aligned at 4 samples */
                                Test_channelInterleave(adcDataIn, testConfig.numRxAntennas, (testConfig.numAdcSamples + 3) / 4 * 4);
                            }

                            errorCode = EDMA_startTransfer(edmaHandle, EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL, EDMA3_CHANNEL_TYPE_DMA);
                            if (errorCode != 0)
                            {
                                System_printf("Error: EDMA start Transfer returned %d\n", errorCode);
                                return;
                            }
                            Task_sleep(1);
                        }/* end of chirp loop */
                        memset((void *)&outParms, 0, sizeof(DPU_RangeProcHWA_OutParams));

                        retVal = DPU_RangeProcHWA_process(rangeProcDpuHandle, &outParms);

                        System_printf("\n... DPU Finish, Check Calibration data ....  : \n\n");
                        if (checkResultFlag == 1)
                        {
                            Test_printResults(&testConfig,1); // , (char*)&testConfigInfo);
                         }
                        else  //hard code to fail, if running into edma run time error
                        {
                            System_printf("Test Calibration Data FAILED!\n");
                            testFeatureStatus = 0;
                            finalResults = 0;
                        }

                    }

                   System_printf("Test #%d finished!\n\r", testCount);
                   testCount++;
                 }/* end of fmt, interleave loop*/
            
                if (testFeatureStatus == 1)
                {
                    MCPI_setFeatureTestResult(featureName, MCPI_TestResult_PASS);
                }
                else
                {
                    MCPI_setFeatureTestResult(featureName, MCPI_TestResult_FAIL);
                }

                System_printf("\n");

            }    /* end of rx antenna number loop*/
        }/* end of number adc samples loop*/
        
    }/* end of number of tx, and fft size loop */
    
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
    HWA_close(hwaHandle);
    DPU_RangeProcHWA_deinit(rangeProcDpuHandle);
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
    
   // Cycleprofiler_init();
    
    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 3*1024;
    Task_create(rangeProcDpuTest_Task, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    
    return 0;
}


