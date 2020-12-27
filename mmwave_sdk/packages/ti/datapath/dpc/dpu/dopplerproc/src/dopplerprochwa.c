/**
 *   @file  dopplerprochwa.c
 *
 *   @brief
 *      Implements Data path Doppler processing Unit using HWA.
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

/* mmWave SDK driver/common Include Files */
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/hwa/hwa.h>

/* Utils */
#include <ti/utils/mathutils/mathutils.h>

/* Data Path Include files */
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpc/dpu/dopplerproc/dopplerprochwa.h>
#include <ti/datapath/dpc/dpu/dopplerproc/include/dopplerprochwainternal.h>

/* Flag to check input parameters */
#define DEBUG_CHECK_PARAMS   1

/* HWA ping/pong buffers offset */
#define DPU_DOPPLERPROCHWA_SRC_PING_OFFSET   ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[0])
#define DPU_DOPPLERPROCHWA_SRC_PONG_OFFSET   ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[1])
#define DPU_DOPPLERPROCHWA_DST_PING_OFFSET   ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[2])
#define DPU_DOPPLERPROCHWA_DST_PONG_OFFSET   ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[3])

/*===========================================================
 *                    Internal Functions
 *===========================================================*/

/**
 *  @b Description
 *  @n
 *      HWA processing completion call back function.
 *  \ingroup    DPU_DOPPLERPROCHWA_INTERNAL_FUNCTION
 */
static void DPU_DopplerProcHWA_hwaDoneIsrCallback(void * arg)
{
    if (arg != NULL) {
        SemaphoreP_post((SemaphoreP_Handle)arg);
    }
}

/**
 *  @b Description
 *  @n
 *      EDMA completion call back function.
 *  \ingroup    DPU_DOPPLERPROCHWA_INTERNAL_FUNCTION
 */
static void DPU_DopplerProcHWA_edmaDoneIsrCallback(uintptr_t arg,
    uint8_t transferCompletionCode)
{
    if (arg != NULL) {
        SemaphoreP_post((SemaphoreP_Handle)arg);
    }
}

/**
 *  @b Description
 *  @n
 *      Configures HWA for Doppler processing.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROCHWA_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */ 
static inline int32_t DPU_DopplerProcHWA_configHwa
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{
    HWA_ParamConfig         hwaParamCfg[DPU_DOPPLERPROCHWA_MAX_NUM_HWA_PARAMSET];
    HWA_InterruptConfig     paramISRConfig;
    uint32_t                paramsetIdx = 0;
    uint32_t                pingSumParamsetIdx;
    int32_t                 retVal = 0U;
    uint32_t                k;
    uint8_t                 destChan;
    
    /* Check if we have the correct number of paramsets.*/
    if(cfg->hwRes.hwaCfg.numParamSets != (2 * cfg->staticCfg.numTxAntennas + 2))
    {    
        retVal = DPU_DOPPLERPROCHWA_EHWARES;
        goto exit;
    }    
    
    memset((void*) &hwaParamCfg[paramsetIdx], 0, sizeof(HWA_ParamConfig));
    
    /************* Doppler FFT for first TX antenna *********************************/
    hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA; 
    hwaParamCfg[paramsetIdx].dmaTriggerSrc = obj->hwaDmaTriggerSourcePing; 
    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_FFT; 

    hwaParamCfg[paramsetIdx].source.srcAddr = DPU_DOPPLERPROCHWA_SRC_PING_OFFSET; // address is relative to start of MEM0
    hwaParamCfg[paramsetIdx].source.srcAcnt = cfg->staticCfg.numDopplerChirps - 1; //size in samples - 1

    hwaParamCfg[paramsetIdx].source.srcAIdx = cfg->staticCfg.numRxAntennas * sizeof(cmplx16ImRe_t); 
    hwaParamCfg[paramsetIdx].source.srcBcnt = cfg->staticCfg.numRxAntennas - 1; 
    hwaParamCfg[paramsetIdx].source.srcBIdx = sizeof(cmplx16ImRe_t); 
    hwaParamCfg[paramsetIdx].source.srcShift = 0; 
    hwaParamCfg[paramsetIdx].source.srcCircShiftWrap = 0; 
    hwaParamCfg[paramsetIdx].source.srcRealComplex = 0; //complex data
    hwaParamCfg[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; 
    hwaParamCfg[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED; 
    hwaParamCfg[paramsetIdx].source.srcConjugate = 0; //no conjugate
    hwaParamCfg[paramsetIdx].source.srcScale = 0;
    hwaParamCfg[paramsetIdx].source.bpmEnable = 0; 
    hwaParamCfg[paramsetIdx].source.bpmPhase = 0; 

    hwaParamCfg[paramsetIdx].dest.dstAddr = DPU_DOPPLERPROCHWA_DST_PING_OFFSET; // address is relative to start of MEM0
    hwaParamCfg[paramsetIdx].dest.dstAcnt = cfg->staticCfg.numDopplerBins - 1; //this is samples - 1
    hwaParamCfg[paramsetIdx].dest.dstAIdx = cfg->staticCfg.numVirtualAntennas * sizeof(uint16_t); 
    hwaParamCfg[paramsetIdx].dest.dstBIdx = sizeof(uint16_t);
    hwaParamCfg[paramsetIdx].dest.dstRealComplex = 1; //Real
    hwaParamCfg[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; 
    hwaParamCfg[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED; 
    hwaParamCfg[paramsetIdx].dest.dstConjugate = 0; //no conjugate
    hwaParamCfg[paramsetIdx].dest.dstScale = 0;
    hwaParamCfg[paramsetIdx].dest.dstSkipInit = 0; 

    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftSize = cfg->staticCfg.log2NumDopplerBins;

    /* scaling is enabled in all stages except for the first stage which is defined by user*/
    if(cfg->hwRes.hwaCfg.firstStageScaling == DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED)
    {
        /* Enable scaling on all stages except first one.*/
        hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = (cfg->staticCfg.numDopplerBins - 1) >> 1;
    }    
    else
    {
        /* Enable scaling on all stages.*/
        hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = (cfg->staticCfg.numDopplerBins - 1);
    }    

    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.interfZeroOutEn = 0; //disabled
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowEn = 1; //enabled
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowStart = cfg->hwRes.hwaCfg.winRamOffset; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.winSymm = cfg->hwRes.hwaCfg.winSym; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.winInterpolateMode = 0; //fftsize is less than 1K
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

    hwaParamCfg[paramsetIdx].complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    retVal = HWA_configParamSet(obj->hwaHandle, 
                                cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx, 
                                &hwaParamCfg[paramsetIdx], NULL);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Make sure DMA interrupt/trigger is disabled for this paramset*/
    retVal = HWA_disableParamSetInterrupt(obj->hwaHandle, 
                                          cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                          HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
    if (retVal != 0)
    {
        goto exit;
    }

    /************* Doppler FFT for remaining TX antennas *********************************/
    for (paramsetIdx = 1; paramsetIdx < cfg->staticCfg.numTxAntennas; paramsetIdx++)
    {
        hwaParamCfg[paramsetIdx] = hwaParamCfg[paramsetIdx-1];
        hwaParamCfg[paramsetIdx].source.srcAddr += sizeof(cmplx16ImRe_t) * cfg->staticCfg.numRxAntennas * cfg->staticCfg.numDopplerChirps;
        hwaParamCfg[paramsetIdx].dest.dstAddr   += sizeof(uint16_t) * cfg->staticCfg.numRxAntennas;
        hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;

        retVal = HWA_configParamSet(obj->hwaHandle, 
                                    cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx, 
                                    &hwaParamCfg[paramsetIdx], NULL);
        if (retVal != 0)
        {
            goto exit;
        }
        
        /* Make sure DMA interrupt/trigger is disabled for this paramset*/
        retVal = HWA_disableParamSetInterrupt(obj->hwaHandle, 
                                              cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                              HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
        if (retVal != 0)
        {
            goto exit;
        }
    }

    /************* Sum of magnitudes *********************************/
    pingSumParamsetIdx = paramsetIdx;
    memset( (void*) &hwaParamCfg[paramsetIdx], 0, sizeof(HWA_ParamConfig));
    hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE; 
    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_FFT; 

    hwaParamCfg[paramsetIdx].source.srcAddr = DPU_DOPPLERPROCHWA_DST_PING_OFFSET; // address is relative to start of MEM0
    hwaParamCfg[paramsetIdx].source.srcAcnt = cfg->staticCfg.numVirtualAntennas-1; //size in samples - 1

    hwaParamCfg[paramsetIdx].source.srcAIdx = sizeof(uint16_t); 
    hwaParamCfg[paramsetIdx].source.srcBcnt = cfg->staticCfg.numDopplerBins - 1; 
    hwaParamCfg[paramsetIdx].source.srcBIdx = cfg->staticCfg.numVirtualAntennas * sizeof(uint16_t); 
    hwaParamCfg[paramsetIdx].source.srcShift = 0; 
    hwaParamCfg[paramsetIdx].source.srcCircShiftWrap = 0; 
    hwaParamCfg[paramsetIdx].source.srcRealComplex = 1; //real data
    hwaParamCfg[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; 
    hwaParamCfg[paramsetIdx].source.srcSign = HWA_SAMPLES_UNSIGNED; 
    hwaParamCfg[paramsetIdx].source.srcConjugate = 0; //no conjugate
    hwaParamCfg[paramsetIdx].source.srcScale = 2;
    hwaParamCfg[paramsetIdx].source.bpmEnable = 0; 
    hwaParamCfg[paramsetIdx].source.bpmPhase = 0; 

    hwaParamCfg[paramsetIdx].dest.dstAddr = DPU_DOPPLERPROCHWA_SRC_PING_OFFSET; // address is relative to start of MEM0
        
    hwaParamCfg[paramsetIdx].dest.dstAcnt = 1 - 1; //get only bin zero
    hwaParamCfg[paramsetIdx].dest.dstAIdx = sizeof(uint16_t); 
    hwaParamCfg[paramsetIdx].dest.dstBIdx = sizeof(uint16_t); // size of one output sample
    hwaParamCfg[paramsetIdx].dest.dstRealComplex = 1; 
    hwaParamCfg[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; 
    hwaParamCfg[paramsetIdx].dest.dstSign = HWA_SAMPLES_UNSIGNED; 
    hwaParamCfg[paramsetIdx].dest.dstConjugate = 0; //no conjugate
    hwaParamCfg[paramsetIdx].dest.dstScale = 8;
    hwaParamCfg[paramsetIdx].dest.dstSkipInit = 0; // no skipping

    if(cfg->staticCfg.numVirtualAntennas == 1)
    {
        /*If number of virtual antennas is 1, do not use FFT to compute sum magnitude.*/
        hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftEn = 0;
        hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftSize = 1;
    }
    else
    {
        hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftEn = 1;
        hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(cfg->staticCfg.numVirtualAntennas);
    }
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.butterflyScaling = 0x3FF; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.interfZeroOutEn = 0; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowEn = 0; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowStart = 0; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.winSymm = cfg->hwRes.hwaCfg.winSym; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.winInterpolateMode = 0; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED; 
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT; 

    hwaParamCfg[paramsetIdx].complexMultiply.mode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
    retVal = HWA_configParamSet(obj->hwaHandle, 
                                cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                &hwaParamCfg[paramsetIdx], 
                                NULL);
    if (retVal != 0)
    {
        goto exit;
    }

    /************ Enable the DMA hookup to this paramset so that data gets copied out ***********/
    /* First, make sure all DMA interrupt/trigger are disabled for this paramset*/
    retVal = HWA_disableParamSetInterrupt(obj->hwaHandle, 
                                          cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                          HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = HWA_getDMAChanIndex(obj->hwaHandle, 
                                 cfg->hwRes.edmaCfg.edmaOut.ping.channel,
                                 &destChan);
    if (retVal != 0)
    {
        goto exit;
    }
    /* Now enable interrupt */
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChan;
    paramISRConfig.cpu.callbackArg = NULL;
    retVal = HWA_enableParamSetInterrupt(obj->hwaHandle, cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx, &paramISRConfig);
    if (retVal != 0)
    {
        goto exit;
    }
    paramsetIdx++;

    /******************** programming HWACC for the pong buffer ****************************/
    /************* Doppler FFT for all TX antennas on pong *********************************/
    for (k=0; k < cfg->staticCfg.numTxAntennas; k++)
    {
        hwaParamCfg[paramsetIdx] = hwaParamCfg[k];

        if (k == 0)
        {
            hwaParamCfg[paramsetIdx].dmaTriggerSrc = obj->hwaDmaTriggerSourcePong;
        }
        
        hwaParamCfg[paramsetIdx].source.srcAddr = DPU_DOPPLERPROCHWA_SRC_PONG_OFFSET + 
                                                  k * (sizeof(cmplx16ImRe_t) * cfg->staticCfg.numRxAntennas * cfg->staticCfg.numDopplerChirps);
        hwaParamCfg[paramsetIdx].dest.dstAddr   = DPU_DOPPLERPROCHWA_DST_PONG_OFFSET + 
                                                  k * (sizeof(uint16_t) * cfg->staticCfg.numRxAntennas);

        retVal = HWA_configParamSet(obj->hwaHandle,
                                    cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                    &hwaParamCfg[paramsetIdx],
                                    NULL);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Make sure DMA interrupt/trigger is disabled for this paramset*/
        retVal = HWA_disableParamSetInterrupt(obj->hwaHandle, 
                                              cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                              HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
        if (retVal != 0)
        {
            goto exit;
        }
        
        paramsetIdx++;
    }

    /************* Sum of magnitudes for pong*********************************/
    hwaParamCfg[paramsetIdx] = hwaParamCfg[pingSumParamsetIdx];
    hwaParamCfg[paramsetIdx].source.srcAddr = DPU_DOPPLERPROCHWA_DST_PONG_OFFSET;
    hwaParamCfg[paramsetIdx].dest.dstAddr   = DPU_DOPPLERPROCHWA_SRC_PONG_OFFSET; 

    retVal = HWA_configParamSet(obj->hwaHandle,
                                cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                &hwaParamCfg[paramsetIdx],
                                NULL);
    if (retVal != 0)
    {
        goto exit;
    }
  
    /************ Enable the DMA hookup to this paramset so that data gets copied out ***********/
    /* First, make sure all DMA interrupt/trigger are disabled for this paramset*/
    retVal = HWA_disableParamSetInterrupt(obj->hwaHandle, 
                                          cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                          HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU);
    if (retVal != 0)
    {
        goto exit;
    }
    
    retVal = HWA_getDMAChanIndex(obj->hwaHandle, 
                                 cfg->hwRes.edmaCfg.edmaOut.pong.channel,
                                 &destChan);
    if (retVal != 0)
    {
        goto exit;
    }
    
    /*Now enable interrupt*/
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChan; //EDMA channel to trigger to copy the data out
    paramISRConfig.cpu.callbackArg = NULL;
    retVal = HWA_enableParamSetInterrupt(obj->hwaHandle,
                                        cfg->hwRes.hwaCfg.paramSetStartIdx + paramsetIdx,
                                        &paramISRConfig);
    if (retVal != 0)
    {
        goto exit;
    }
    
exit:
    return(retVal);
 }

/**
 *  @b Description
 *  @n
 *  Doppler DPU EDMA configuration.
 *  This implementation of doppler processing involves Ping/Pong 
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROCHWA_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static inline int32_t DPU_DopplerProcHWA_configEdma
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{
    int32_t             retVal = EDMA_NO_ERROR;
    cmplx16ImRe_t       *radarCubeBase = (cmplx16ImRe_t *)cfg->hwRes.radarCube.data;
    uint16_t            *detMatrixBase = (uint16_t *)cfg->hwRes.detMatrix.data;
    int16_t             sampleLenInBytes = sizeof(cmplx16ImRe_t);
    uint32_t            sizeOfAbsTransfer = cfg->staticCfg.numDopplerBins;
    uint32_t            sizeOfAbsTransferBytes = sizeOfAbsTransfer * sizeof(uint16_t);
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncACfg     syncACfg;
    DPEDMA_syncABCfg    syncABCfg;

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    
    /**************PROGRAM DMA'S FOR PING**************************************/
        
    /***************************************************************************
     *  PROGRAM DMA channel  to transfer 2D abs data from accelerator output
     *  buffer (ping) to L3
     **************************************************************************/
    chainingCfg.chainingChan                  = cfg->hwRes.edmaCfg.edmaIn.ping.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = false;

    syncACfg.srcAddress  = (uint32_t)obj->hwaMemBankAddr[0];
    syncACfg.destAddress = (uint32_t)(&detMatrixBase[0]);
    syncACfg.aCount      = sizeOfAbsTransferBytes;
    syncACfg.bCount      = cfg->staticCfg.numRangeBins / 2U; //factor of 2 due to ping/pong
    syncACfg.srcBIdx     = 0;
    syncACfg.dstBIdx     = sizeOfAbsTransferBytes * 2U;//factor of 2 due to ping/pong
         
    retVal = DPEDMA_configSyncA_singleFrame(cfg->hwRes.edmaCfg.edmaHandle,
                                &cfg->hwRes.edmaCfg.edmaOut.ping,
                                &chainingCfg,
                                &syncACfg,
                                true, //isEventTriggered
                                false,//isIntermediateTransferInterruptEnabled
                                false,//isTransferCompletionEnabled
                                NULL, //transferCompletionCallbackFxn
                                NULL);//transferCompletionCallbackFxnArg

    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /******************************************************************************************
    *  PROGRAM DMA channel  to transfer data from Radar cube to accelerator input buffer (ping)
    ******************************************************************************************/    
    chainingCfg.chainingChan                  = cfg->hwRes.edmaCfg.edmaHotSig.ping.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = true;

    syncABCfg.srcAddress  = (uint32_t)(&radarCubeBase[0]);
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[0]);
    syncABCfg.aCount      = sampleLenInBytes;
    syncABCfg.bCount      = cfg->staticCfg.numRxAntennas * cfg->staticCfg.numDopplerChirps *  cfg->staticCfg.numTxAntennas;
    syncABCfg.cCount      = cfg->staticCfg.numRangeBins / 2;//factor of 2 due to ping/pong
    syncABCfg.srcBIdx     = cfg->staticCfg.numRangeBins * sampleLenInBytes;
    syncABCfg.dstBIdx     = sampleLenInBytes;
    syncABCfg.srcCIdx     = 2 * sampleLenInBytes;//factor of 2 due to ping/pong
    syncABCfg.dstCIdx     = 0;

    retVal = DPEDMA_configSyncAB(cfg->hwRes.edmaCfg.edmaHandle,
                                 &cfg->hwRes.edmaCfg.edmaIn.ping,
                                 &chainingCfg,
                                 &syncABCfg,
                                 false,//isEventTriggered
                                 true, //isIntermediateTransferCompletionEnabled
                                 false,//isTransferCompletionEnabled
                                 NULL, //transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg

    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }
    
    /******************************************************************************************
    *  PROGRAM DMA channel to trigger HWA for processing of input (ping)
    ******************************************************************************************/            
    retVal = DPEDMAHWA_configOneHotSignature(cfg->hwRes.edmaCfg.edmaHandle,
                                             &cfg->hwRes.edmaCfg.edmaHotSig.ping,
                                             obj->hwaHandle,
                                             obj->hwaDmaTriggerSourcePing,
                                             false);

    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /**************PROGRAM DMA'S FOR PONG********************************************************/

    /******************************************************************************************
    *  PROGRAM DMA channel  to transfer 2D abs data from accelerator output buffer (pong) to L3
    ******************************************************************************************/
    chainingCfg.chainingChan                  = cfg->hwRes.edmaCfg.edmaIn.pong.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = false;

    /* Transfer parameters are the same as ping, except for src/dst addresses */
    syncACfg.srcAddress  = (uint32_t)obj->hwaMemBankAddr[1];
    syncACfg.destAddress = (uint32_t)(&detMatrixBase[sizeOfAbsTransfer]);
    
    retVal = DPEDMA_configSyncA_singleFrame(cfg->hwRes.edmaCfg.edmaHandle,
                                &cfg->hwRes.edmaCfg.edmaOut.pong,
                                &chainingCfg,
                                &syncACfg,
                                true, //isEventTriggered
                                false,//isIntermediateTransferInterruptEnabled
                                true, //isTransferCompletionEnabled
                                DPU_DopplerProcHWA_edmaDoneIsrCallback, //transferCompletionCallbackFxn
                                (uintptr_t)obj->edmaDoneSemaHandle); //transferCompletionCallbackFxnArg
    
    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /******************************************************************************************
     *  PROGRAM DMA channel  to transfer data from L3 to accelerator input buffer (pong)
     ******************************************************************************************/ 
    chainingCfg.chainingChan                  = cfg->hwRes.edmaCfg.edmaHotSig.pong.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = true;

    /* Transfer parameters are the same as ping, except for src/dst addresses */
    syncABCfg.srcAddress  = (uint32_t)(&radarCubeBase[1]);
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[1]);

    retVal = DPEDMA_configSyncAB(cfg->hwRes.edmaCfg.edmaHandle,
                                 &cfg->hwRes.edmaCfg.edmaIn.pong,
                                 &chainingCfg,
                                 &syncABCfg,
                                 false,//isEventTriggered
                                 true, //isIntermediateTransferCompletionEnabled
                                 false,//isTransferCompletionEnabled
                                 NULL, //transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg
    
    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /******************************************************************************************
    *  PROGRAM DMA channel to trigger HWA for processing of input (pong)
    ******************************************************************************************/  
    retVal = DPEDMAHWA_configOneHotSignature(cfg->hwRes.edmaCfg.edmaHandle,
                                             &cfg->hwRes.edmaCfg.edmaHotSig.pong,
                                             obj->hwaHandle,
                                             obj->hwaDmaTriggerSourcePong,
                                             false);

    if (retVal != EDMA_NO_ERROR)
    {
        goto exit;
    }

exit:
    return(retVal);
} 

/*===========================================================
 *                    Doppler Proc External APIs
 *===========================================================*/

/**
 *  @b Description
 *  @n
 *      dopplerProc DPU init function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]   initCfg Pointer to initial configuration parameters
 *  @param[out]  errCode Pointer to errCode generates by the API
 *
 *  \ingroup    DPU_DOPPLERPROCHWA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid handle
 *  @retval
 *      Error       - NULL
 */
DPU_DopplerProcHWA_Handle DPU_DopplerProcHWA_init
(
    DPU_DopplerProcHWA_InitParams *initCfg,
    int32_t                    *errCode
)
{
    DPU_DopplerProcHWA_Obj  *obj = NULL;
    SemaphoreP_Params       semParams;
    HWA_MemInfo             hwaMemInfo;
    uint32_t                i;

    *errCode       = 0;
    
    if((initCfg == NULL) || (initCfg->hwaHandle == NULL))
    {
        *errCode = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }    

    /* Allocate memory */
    obj = MemoryP_ctrlAlloc(sizeof(DPU_DopplerProcHWA_Obj), 0U);
    if(obj == NULL)
    {
        *errCode = DPU_DOPPLERPROCHWA_ENOMEM;
        goto exit;
    }

    /* Initialize memory */
    memset((void *)obj, 0U, sizeof(DPU_DopplerProcHWA_Obj));
    
    /* Save init config params */
    obj->hwaHandle   = initCfg->hwaHandle;

    /* Create DPU semaphores */
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    obj->edmaDoneSemaHandle = SemaphoreP_create(0, &semParams);
    if(obj->edmaDoneSemaHandle == NULL)
    {
        *errCode = DPU_DOPPLERPROCHWA_ESEMA;
        goto exit;
    }

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    obj->hwaDoneSemaHandle = SemaphoreP_create(0, &semParams);
    if(obj->hwaDoneSemaHandle == NULL)
    {
        *errCode = DPU_DOPPLERPROCHWA_ESEMA;
        goto exit;
    }    
    
    /* Populate HWA base addresses and offsets. This is done only once, at init time.*/
    *errCode =  HWA_getHWAMemInfo(obj->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {       
        goto exit;
    }
    
    /* check if we have enough memory banks*/
    if(hwaMemInfo.numBanks < DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS)
    {    
        *errCode = DPU_DOPPLERPROCHWA_EHWARES;
        goto exit;
    }
    
    for (i = 0; i < DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS; i++)
    {
        obj->hwaMemBankAddr[i] = hwaMemInfo.baseAddress + i * hwaMemInfo.bankSize;
    }
    
exit:    

    if(*errCode < 0)
    {
        if(obj != NULL)
        {
            MemoryP_ctrlFree(obj, sizeof(DPU_DopplerProcHWA_Obj));
            obj = NULL;
        }
    }
   return ((DPU_DopplerProcHWA_Handle)obj);
}

/**
  *  @b Description
  *  @n
  *   Doppler DPU configuration 
  *
  *  @param[in]   handle     DPU handle.
  *  @param[in]   cfg        Pointer to configuration parameters.
  *
  *  \ingroup    DPU_DOPPLERPROCHWA_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success      = 0
  *  @retval
  *      Error       != 0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_config
(
    DPU_DopplerProcHWA_Handle    handle,
    DPU_DopplerProcHWA_Config    *cfg
)
{
    DPU_DopplerProcHWA_Obj   *obj;
    int32_t                  retVal = 0;
    uint16_t                 expectedWinSamples;


    obj = (DPU_DopplerProcHWA_Obj *)handle;
    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    
#if DEBUG_CHECK_PARAMS
    /* Validate params */
    if(!cfg ||
       !cfg->hwRes.edmaCfg.edmaHandle ||
       !cfg->hwRes.hwaCfg.window || 
       !cfg->hwRes.radarCube.data ||
       !cfg->hwRes.detMatrix.data
      )
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    /* Check if radar cube format is supported by DPU*/
    if(cfg->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_1)
    {
        retVal = DPU_DOPPLERPROCHWA_ECUBEFORMAT;
        goto exit;
    }

    /* Check if detection matrix format is supported by DPU*/
    if(cfg->hwRes.detMatrix.datafmt != DPIF_DETMATRIX_FORMAT_1)
    {
        retVal = DPU_DOPPLERPROCHWA_EDETMFORMAT;
        goto exit;
    }
    
    /* Check if radar cube column fits into one HWA memory bank */
    if((cfg->staticCfg.numTxAntennas * cfg->staticCfg.numRxAntennas * 
        cfg->staticCfg.numDopplerChirps * sizeof(cmplx16ImRe_t)) > (SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS))
    {
        retVal = DPU_DOPPLERPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check if abs value of log2 of 2D FFT fits in one HWA memory bank */
    if((cfg->staticCfg.numTxAntennas * cfg->staticCfg.numRxAntennas * 
        cfg->staticCfg.numDopplerBins * sizeof(uint16_t)) > (SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS))
    {
        retVal = DPU_DOPPLERPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check if number of range bins is even*/
    if((cfg->staticCfg.numRangeBins % 2) != 0)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    
    /* Check if detection matrix size is sufficient*/
    if(cfg->hwRes.detMatrix.dataSize < (cfg->staticCfg.numRangeBins *
                                        cfg->staticCfg.numDopplerBins * sizeof(uint16_t)))
    {
        retVal = DPU_DOPPLERPROCHWA_EDETMSIZE;
        goto exit;
    }

    /* Check window Size */
    if(cfg->hwRes.hwaCfg.winSym == HWA_FFT_WINDOW_NONSYMMETRIC)
    {
        expectedWinSamples = cfg->staticCfg.numDopplerChirps;
    }
    else
    {
        /* odd samples have to be rounded up per HWA */
        expectedWinSamples = (cfg->staticCfg.numDopplerChirps + 1) / 2;
    }

    if (cfg->hwRes.hwaCfg.windowSize != expectedWinSamples * sizeof(int32_t)) 
    {
            retVal = DPU_DOPPLERPROCHWA_EWINDSIZE;
            goto exit;
    }        
#endif

    /* Save necessary parameters to DPU object that will be used during Process time */
    
    /* EDMA parameters needed to trigger first EDMA transfer*/
    obj->edmaHandle  = cfg->hwRes.edmaCfg.edmaHandle;
    memcpy((void*)(&obj->edmaIn), (void *)(&cfg->hwRes.edmaCfg.edmaIn), sizeof(DPU_DopplerProc_Edma));
    
    /*HWA parameters needed for the HWA common configuration*/
    obj->hwaNumLoops      = cfg->staticCfg.numRangeBins / 2U;
    obj->hwaParamStartIdx = cfg->hwRes.hwaCfg.paramSetStartIdx;    
    obj->hwaParamStopIdx  = cfg->hwRes.hwaCfg.paramSetStartIdx + cfg->hwRes.hwaCfg.numParamSets - 1;
    
    /* Disable the HWA */
    retVal = HWA_enable(obj->hwaHandle, 0); 
    if (retVal != 0)
    {
        goto exit;
    }
    
    /* HWA window configuration */
    retVal = HWA_configRam(obj->hwaHandle,
                           HWA_RAM_TYPE_WINDOW_RAM,
                           (uint8_t *)cfg->hwRes.hwaCfg.window,
                           cfg->hwRes.hwaCfg.windowSize, //size in bytes
                           cfg->hwRes.hwaCfg.winRamOffset * sizeof(int32_t)); 
    if (retVal != 0)
    {
        goto exit;
    }
    
    /*******************************/
    /**  Configure HWA            **/
    /*******************************/
    /*Compute source DMA channels that will be programmed in both HWA and EDMA.   
      The DMA channels are set to be equal to the paramSetIdx used by HWA*/
    /* Ping DMA channel (Ping uses the first paramset)*/  
    obj->hwaDmaTriggerSourcePing = cfg->hwRes.hwaCfg.paramSetStartIdx;
    /* Pong DMA channel*/  
    obj->hwaDmaTriggerSourcePong = cfg->hwRes.hwaCfg.paramSetStartIdx + cfg->staticCfg.numTxAntennas + 1;
    retVal = DPU_DopplerProcHWA_configHwa(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }
                    
    /*******************************/
    /**  Configure EDMA           **/
    /*******************************/    
    retVal = DPU_DopplerProcHWA_configEdma(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return retVal;
}

 /**
  *  @b Description
  *  @n Doppler DPU process function. 
  *   
  *  @param[in]   handle     DPU handle.
  *  @param[out]  outParams  Output parameters.
  *
  *  \ingroup    DPU_DOPPLERPROCHWA_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success     =0
  *  @retval
  *      Error      !=0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_process
(
    DPU_DopplerProcHWA_Handle    handle,
    DPU_DopplerProcHWA_OutParams *outParams
)
{
    volatile uint32_t   startTime;
    DPU_DopplerProcHWA_Obj *obj;
    int32_t             retVal = 0;
    bool                status;
    HWA_CommonConfig    hwaCommonConfig;

    obj = (DPU_DopplerProcHWA_Obj *)handle;
    if (obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    /* Set inProgress state */
    obj->inProgress = true;

    startTime = Cycleprofiler_getTimeStamp();
    
    /**********************************************/
    /* ENABLE NUMLOOPS DONE INTERRUPT FROM HWA */
    /**********************************************/
    retVal = HWA_enableDoneInterrupt(obj->hwaHandle,
                                       DPU_DopplerProcHWA_hwaDoneIsrCallback,
                                       obj->hwaDoneSemaHandle);
    if (retVal != 0)
    {
        goto exit;
    }
    
    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    memset((void*) &hwaCommonConfig, 0, sizeof(HWA_CommonConfig));

    /* Config Common Registers */
    hwaCommonConfig.configMask =
        HWA_COMMONCONFIG_MASK_NUMLOOPS |
        HWA_COMMONCONFIG_MASK_PARAMSTARTIDX |
        HWA_COMMONCONFIG_MASK_PARAMSTOPIDX |
        HWA_COMMONCONFIG_MASK_FFT1DENABLE |
        HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD;

    hwaCommonConfig.numLoops      = obj->hwaNumLoops;     
    hwaCommonConfig.paramStartIdx = obj->hwaParamStartIdx;
    hwaCommonConfig.paramStopIdx  = obj->hwaParamStopIdx; 
    hwaCommonConfig.fftConfig.fft1DEnable = HWA_FEATURE_BIT_DISABLE;
    hwaCommonConfig.fftConfig.interferenceThreshold = 0xFFFFFF;
    
    retVal = HWA_configCommon(obj->hwaHandle, &hwaCommonConfig);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(obj->hwaHandle,1); 
    if (retVal != 0)
    {
        goto exit;
    }

    EDMA_startTransfer(obj->edmaHandle, obj->edmaIn.ping.channel, EDMA3_CHANNEL_TYPE_DMA);
    EDMA_startTransfer(obj->edmaHandle, obj->edmaIn.pong.channel, EDMA3_CHANNEL_TYPE_DMA);
    
    /**********************************************/
    /* WAIT FOR HWA NUMLOOPS INTERRUPT            */
    /**********************************************/
    status = SemaphoreP_pend(obj->hwaDoneSemaHandle, SemaphoreP_WAIT_FOREVER);
    
    if (status != SemaphoreP_OK)
    {
        retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
        goto exit;
    }

    HWA_disableDoneInterrupt(obj->hwaHandle);

    /* Disable the HWA */
    retVal = HWA_enable(obj->hwaHandle, 0); 
    if (retVal != 0)
    {
        goto exit;
    }
    
    /**********************************************/
    /* WAIT FOR EDMA DONE INTERRUPT            */
    /**********************************************/
    status = SemaphoreP_pend(obj->edmaDoneSemaHandle, SemaphoreP_WAIT_FOREVER);
    if (status != SemaphoreP_OK)
    {
        retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
        goto exit;
    }
    
    outParams->stats.numProcess++;
    outParams->stats.processingTime = Cycleprofiler_getTimeStamp() - startTime;
    
exit:
    if (obj != NULL)
    {
        obj->inProgress = false;
    }    
    
    return retVal;
}

/**
  *  @b Description
  *  @n
  *  Doppler DPU deinit 
  *
  *  @param[in]   handle   DPU handle.
  *
  *  \ingroup    DPU_DOPPLERPROCHWA_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success      =0
  *  @retval
  *      Error       !=0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_deinit(DPU_DopplerProcHWA_Handle handle)
{
    int32_t     retVal = 0;
    
    /* Free memory */
    if(handle == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
    }
    else
    {
        MemoryP_ctrlFree(handle, sizeof(DPU_DopplerProcHWA_Obj));
    }
    
    return retVal;
}

