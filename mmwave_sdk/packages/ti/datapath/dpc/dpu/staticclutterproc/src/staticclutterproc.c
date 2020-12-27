/**
 *   @file  staticclutterproc.c
 *
 *   @brief
 *      Implements staticclutterproc DPU processing.
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

/* mmWave SDK driver/osal Include files */
#include <ti/common/sys_types.h>
#include <ti/common/sys_defs.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/soc/soc.h>

/* Data path Include files */
#include <ti/datapath/dpc/dpu/staticclutterproc/include/staticclutterproc_internal.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>

/**************************************************************************
 *************************** Internal Functions ********************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This function configures the static clutter  DPU EDMAs.
 *
 *  @param[in]  obj         staticClutterDpu object.
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     =0
 *  @retval
 *      Error       !=0
 */    
int32_t DPU_StaticClutterProc_configEdma(DPU_StaticClutterProc_Obj *obj)
{
    int32_t             retVal = 0;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncABCfg    syncABCfg;
    uint16_t            sampleLenInBytes = sizeof(cmplx16ImRe_t);
    cmplx16ImRe_t       *radarCubeBase = (cmplx16ImRe_t *)obj->cfg.hwRes.radarCube.data;
    cmplx16ImRe_t       *scratchBase   = (cmplx16ImRe_t *)obj->cfg.hwRes.scratchBuf.buf;
    uint16_t            numRxTimesNumRange = obj->cfg.staticCfg.numRxAntennas * obj->cfg.staticCfg.numRangeBins;

     /* Ping input: from radar cube to internal memory*/
    chainingCfg.chainingChan                  = obj->cfg.hwRes.edmaIn.ping.channel;
    chainingCfg.isIntermediateChainingEnabled = false;
    chainingCfg.isFinalChainingEnabled        = false;

    syncABCfg.srcAddress  = (uint32_t)(&radarCubeBase[0]);
    syncABCfg.destAddress = (uint32_t)(&scratchBase[0]);
    syncABCfg.aCount      = sampleLenInBytes;
    syncABCfg.bCount      = obj->cfg.staticCfg.numDopplerChirps;
    syncABCfg.cCount      = numRxTimesNumRange / 2;//divided by 2 due to ping/pong
    syncABCfg.srcBIdx     = numRxTimesNumRange * sampleLenInBytes;
    syncABCfg.dstBIdx     = sampleLenInBytes;
    syncABCfg.srcCIdx     = 2 * sampleLenInBytes;//2 factor due to ping/pong
    syncABCfg.dstCIdx     = 0;

    retVal = DPEDMA_configSyncAB(obj->cfg.hwRes.edmaHandle,
                                 &obj->cfg.hwRes.edmaIn.ping,
                                 &chainingCfg,
                                 &syncABCfg,
                                 false,//isEventTriggered
                                 true,//isIntermediateTransferCompletionEnabled
                                 true,//isTransferCompletionEnabled
                                 NULL,//transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg

    if (retVal < 0)
    {
        goto exit;
    }

    /* Pong input from radar cube to internal memory*/
    /* Chaining info is the same except for chainingChan */
    chainingCfg.chainingChan = obj->cfg.hwRes.edmaIn.pong.channel;

    /* Same transfer parameters as Ping, except for src/dst addresses*/
    syncABCfg.srcAddress  = (uint32_t)(&radarCubeBase[1]);
    syncABCfg.destAddress = (uint32_t)(&scratchBase[obj->sizePingBufSamples]);

    retVal = DPEDMA_configSyncAB(obj->cfg.hwRes.edmaHandle,
                                 &obj->cfg.hwRes.edmaIn.pong,
                                 &chainingCfg,
                                 &syncABCfg,
                                 false,//isEventTriggered
                                 true,//isIntermediateTransferCompletionEnabled
                                 true,//isTransferCompletionEnabled
                                 NULL,//transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg
    
    if (retVal < 0)
    {
        goto exit;
    }

    /* Ping - output from internal memory to radar cube */
    /* Chaining info is the same except for chainingChan */
    chainingCfg.chainingChan = obj->cfg.hwRes.edmaOut.ping.channel;

    syncABCfg.srcAddress  = (uint32_t)(&scratchBase[0]);
    syncABCfg.destAddress = (uint32_t)(&radarCubeBase[0]);
    syncABCfg.aCount      = sampleLenInBytes;
    syncABCfg.bCount      = obj->cfg.staticCfg.numDopplerChirps;
    syncABCfg.cCount      = numRxTimesNumRange / 2;//divided by 2 due to ping/pong
    syncABCfg.srcBIdx     = sampleLenInBytes;
    syncABCfg.dstBIdx     = numRxTimesNumRange * sampleLenInBytes;
    syncABCfg.srcCIdx     = 0;
    syncABCfg.dstCIdx     = 2 * sampleLenInBytes;//2 factor due to ping/pong

    retVal = DPEDMA_configSyncAB(obj->cfg.hwRes.edmaHandle,
                                 &obj->cfg.hwRes.edmaOut.ping,
                                 &chainingCfg,
                                 &syncABCfg,
                                 false,//isEventTriggered
                                 true,//isIntermediateTransferCompletionEnabled
                                 true,//isTransferCompletionEnabled
                                 NULL,//transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg

    if (retVal < 0)
    {
        goto exit;
    }

    /* Pong - output from internal memory to radar cube */
    /* Chaining info is the same except for chainingChan */
    chainingCfg.chainingChan                  = obj->cfg.hwRes.edmaOut.pong.channel;

    /* Transfer parameters are the same as ping buffer, except for src/dst addresses*/
    syncABCfg.srcAddress  = (uint32_t)(&scratchBase[obj->sizePingBufSamples]);
    syncABCfg.destAddress = (uint32_t)(&radarCubeBase[1]);

    retVal = DPEDMA_configSyncAB(obj->cfg.hwRes.edmaHandle,
                                 &obj->cfg.hwRes.edmaOut.pong,
                                 &chainingCfg,
                                 &syncABCfg,
                                 false,//isEventTriggered
                                 true,//isIntermediateTransferCompletionEnabled
                                 true,//isTransferCompletionEnabled
                                 NULL,//transferCompletionCallbackFxn
                                 NULL);//transferCompletionCallbackFxnArg

    if (retVal < 0)
    {
        goto exit;
    }

exit:    
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This function waits for an input (from radar cube to scratch buffer)
 *      EDMA transfer to complete.
 *
 *  @param[in]  obj         staticClutterDpu object.
 *  @param[in]  pingPongId  Ping/pong ID
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     =0
 *  @retval
 *      Error       !=0
 */
int32_t DPU_StaticClutterProc_waitInData(DPU_StaticClutterProc_Obj *obj, uint32_t pingPongId)
{
    /* wait until transfer done */
    volatile bool isTransferDone;
    uint8_t       chId;
    int32_t       retVal = 0;
    
    if(pingPongId == DPU_STATICCLUTTERPROC_PING_IDX)
    {
        chId = obj->cfg.hwRes.edmaIn.ping.channel;
    }
    else
    {
        chId = obj->cfg.hwRes.edmaIn.pong.channel;
    }
    do 
    {
        retVal = EDMA_isTransferComplete(obj->cfg.hwRes.edmaHandle,
                                        chId,
                                        (bool *)&isTransferDone);
        if(retVal != EDMA_NO_ERROR)
        {
            return retVal;
        }
    } while (isTransferDone == false);
    
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This function waits for an output (from scratch buffer to radar cube)
 *      EDMA transfer to complete.
 *
 *  @param[in]  obj         staticClutterDpu object.
 *  @param[in]  pingPongId  Ping/pong ID
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     =0
 *  @retval
 *      Error       !=0
 */
int32_t DPU_StaticClutterProc_waitOutData(DPU_StaticClutterProc_Obj *obj, uint32_t pingPongId)
{
    /* wait until transfer done */
    volatile bool isTransferDone;
    uint8_t       chId;
    int32_t       retVal = 0;
    
    if(pingPongId == DPU_STATICCLUTTERPROC_PING_IDX)
    {
        chId = obj->cfg.hwRes.edmaOut.ping.channel;
    }
    else
    {
        chId = obj->cfg.hwRes.edmaOut.pong.channel;
    }
    do 
    {
        retVal = EDMA_isTransferComplete(obj->cfg.hwRes.edmaHandle,
                                    chId,
                                    (bool *)&isTransferDone);
        if(retVal != EDMA_NO_ERROR)
        {
            return retVal;
        }
    } while (isTransferDone == false);
    
    return 0;
}

#ifdef SUBSYS_DSS
#include <ti/alg/mmwavelib/mmwavelib.h>
/**
 *  @b Description
 *  @n
 *      This function executes the clutter removal computation.
 *
 *  @param[in]  obj         staticClutterDpu object.
 *  @param[in]  inputBuf    input buffer. Clutter removal algorithm will work on this data.
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_INTERNAL_FUNCTION
 *
 *  @retval N/A
 */
static inline void DPU_StaticClutterProc_clutterRemoval(DPU_StaticClutterProc_Obj *obj, cmplx16ImRe_t *inputBuf)
{
    uint32_t sumVal[2]={0};
    cmplx32ImRe_t *pSumVal  = (cmplx32ImRe_t *) sumVal;
    uint32_t meanVal;
    cmplx16ImRe_t *pMeanVal = (cmplx16ImRe_t *) &meanVal;

    /* Assume that scratch buffer is not initialized. Even if it is initialized to zeros,
       after every call of mmwavelib_vecsubc() below the full buffer will be updated (all samples).
       Therefore, we need to pad the scratch buffer with zeros in the last X samples. X can
       be 0,1,2,3. In fact, X = obj->sizePingBufSamples - obj->cfg.staticCfg.numDopplerChirps)
       This should be done every time this API is called, for both ping and pong.
       When this API is called inputBuf[] already has the contents of the radarCube transferred in,
       therefore only the trully padded samples should be set to zero.
       Also note that if numDopplerChirps is already a multiple of 4, then no padding should be done.       
       R4F does not use the padded samples so this is not necessary.*/
    if(obj->sizePingBufSamples > obj->cfg.staticCfg.numDopplerChirps)
    {
        memset((void*)&inputBuf[obj->cfg.staticCfg.numDopplerChirps], 
               0,
               (obj->sizePingBufSamples - obj->cfg.staticCfg.numDopplerChirps) * sizeof(cmplx16ImRe_t));
    }
    
    /*Lib requires size to be multiple of 4*/
    mmwavelib_vecsum((int16_t *) inputBuf,
                     (int32_t *) sumVal,
                     (int32_t)   obj->sizePingBufSamples);

    /*Divide by numDopplerChirps as the scratch buffer was padded to zero to make it a 
      multiple of 4 so it does not affect the average*/    
    pMeanVal->real = pSumVal->real / obj->cfg.staticCfg.numDopplerChirps ;
    pMeanVal->imag = pSumVal->imag / obj->cfg.staticCfg.numDopplerChirps ;
    
    /*Lib requires size to be multiple of 4*/
    mmwavelib_vecsubc((int16_t *) inputBuf,
                      (int16_t *) inputBuf,
                      (uint32_t)  meanVal,
                      (int32_t)   obj->sizePingBufSamples);
}
#endif

#ifdef SUBSYS_MSS
/**
 *  @b Description
 *  @n
 *      This function executes the clutter removal computation.
 *
 *  @param[in]  obj         staticClutterDpu object.
 *  @param[in]  inputBuf    input buffer. Clutter removal algorithm will work on this data.
 *
 *  \ingroup    DPU_STATICCLUTTERPROC_INTERNAL_FUNCTION
 *
 *  @retval N/A
 */
static inline void DPU_StaticClutterProc_clutterRemoval(DPU_StaticClutterProc_Obj *obj, cmplx16ImRe_t *inputBuf)
{
    uint32_t                  dopIdx;
    cmplx32ImRe_t             sumVal;
    cmplx16ImRe_t             meanVal;
    int32_t                   *a;
    int32_t                   *b;

    sumVal.real = 0;
    sumVal.imag = 0;
    
    for (dopIdx = 0; dopIdx < obj->cfg.staticCfg.numDopplerChirps; dopIdx++)
    {
        sumVal.real += inputBuf[dopIdx].real;
        sumVal.imag += inputBuf[dopIdx].imag;
    }
    
    meanVal.real = sumVal.real / obj->cfg.staticCfg.numDopplerChirps;
    meanVal.imag = sumVal.imag / obj->cfg.staticCfg.numDopplerChirps;
    
    b = (int32_t*)&meanVal;
    a = (int32_t*)&inputBuf[0];
    for (dopIdx = 0; dopIdx < obj->cfg.staticCfg.numDopplerChirps; dopIdx++)
    {
        /*Implementing the following subtraction
          inputBuf[dopIdx].real -= meanVal.real;
          inputBuf[dopIdx].imag -= meanVal.imag;
          using intrinsics below. 
          Note that the intrinsics below also 
          saturates instead of overflowing.
        */
        *a = _qsub16(*(a++),*b);
    }
}
#endif

/**************************************************************************
 *************************** External Functions ****************************
 **************************************************************************/
DPU_StaticClutterProc_Handle DPU_StaticClutterProc_init
(
    int32_t*            errCode
)
{
    DPU_StaticClutterProc_Obj    *obj = NULL;
    *errCode = 0;

    /* Allocate memory and save initParams */
    obj = MemoryP_ctrlAlloc(sizeof(DPU_StaticClutterProc_Obj), 0U);
    if(obj == NULL)
    {
        *errCode = DPU_STATICCLUTTERPROC_ENOMEM;
        goto exit;
    }

    /* Initialize memory */
    memset((void *)obj, 0U, sizeof(DPU_StaticClutterProc_Obj));

exit:
    return ((DPU_StaticClutterProc_Handle)obj);
}

int32_t DPU_StaticClutterProc_process
(
    DPU_StaticClutterProc_Handle handle, 
    DPU_StaticClutterProc_OutParams *outParams
)
{
    DPU_StaticClutterProc_Obj *obj;
    int32_t                   retVal = 0;
    uint32_t                  pingPongIdx = DPU_STATICCLUTTERPROC_PING_IDX;
    uint32_t                  nextPingPongIdx;
    uint32_t                  idx, txAntIdx, firstSampleIdx;
    cmplx16ImRe_t             *radarCubeBase;
    cmplx16ImRe_t             *scratchBase;
    
    if(handle == NULL)
    {
        retVal = DPU_STATICCLUTTERPROC_EINVAL;
        goto exit;
    }
    
    obj = (DPU_StaticClutterProc_Obj*) handle;
    
    radarCubeBase = (cmplx16ImRe_t *)obj->cfg.hwRes.radarCube.data;
    scratchBase   = (cmplx16ImRe_t *)obj->cfg.hwRes.scratchBuf.buf;

    for(txAntIdx = 0; txAntIdx < obj->cfg.staticCfg.numTxAntennas; txAntIdx++)
    {
        /*Need to set correct EDMA source address for the first transfer
          of every TX antenna.*/
        firstSampleIdx = txAntIdx*obj->cfg.staticCfg.numRangeBins*obj->cfg.staticCfg.numRxAntennas*obj->cfg.staticCfg.numDopplerChirps;
          
        /* EDMA IN channels */ 
        retVal = EDMA_setSourceAddress(obj->cfg.hwRes.edmaHandle, 
                                       obj->cfg.hwRes.edmaIn.ping.channel,
                                       (uint32_t)(SOC_translateAddress((uint32_t)(&radarCubeBase[firstSampleIdx]),
                                                                       SOC_TranslateAddr_Dir_TO_EDMA, NULL)));
        if(retVal < 0)
        {
            goto exit;
        }
                                                              
        retVal = EDMA_setSourceAddress(obj->cfg.hwRes.edmaHandle, 
                                       obj->cfg.hwRes.edmaIn.pong.channel,
                                       (uint32_t)(SOC_translateAddress((uint32_t)(&radarCubeBase[firstSampleIdx + 1]),
                                                                       SOC_TranslateAddr_Dir_TO_EDMA, NULL)));
                             
        if(retVal < 0)
        {
            goto exit;
        }
        
        /* EDMA OUT channels */
        retVal = EDMA_setDestinationAddress(obj->cfg.hwRes.edmaHandle, 
                                            obj->cfg.hwRes.edmaOut.ping.channel,
                                            (uint32_t)(SOC_translateAddress((uint32_t)(&radarCubeBase[firstSampleIdx]),
                                                                            SOC_TranslateAddr_Dir_TO_EDMA, NULL)));
        if(retVal < 0)
        {
            goto exit;
        }
                                                                   
        retVal = EDMA_setDestinationAddress(obj->cfg.hwRes.edmaHandle, 
                                            obj->cfg.hwRes.edmaOut.pong.channel,
                                            (uint32_t)(SOC_translateAddress((uint32_t)(&radarCubeBase[firstSampleIdx + 1]),
                                                                             SOC_TranslateAddr_Dir_TO_EDMA, NULL)));
        if(retVal < 0)
        {
            goto exit;
        }
                             
        /* Always start with ping buffer */
        pingPongIdx = DPU_STATICCLUTTERPROC_PING_IDX;
        
        /* trigger first DMA */
        retVal = EDMA_startDmaTransfer(obj->cfg.hwRes.edmaHandle, obj->cfg.hwRes.edmaIn.ping.channel);
        if(retVal < 0)
        {
            goto exit;
        }
        
        for (idx = 0; idx < obj->cfg.staticCfg.numRangeBins*obj->cfg.staticCfg.numRxAntennas; idx++)
        {           
            /* kick off next DMA */
            if (idx < obj->cfg.staticCfg.numRangeBins * obj->cfg.staticCfg.numRxAntennas - 1)
            {
                nextPingPongIdx = pingPongIdx^1;
                
                /* Wait for the previous output transfer to finish for the ping/pong
                   buffer that will be triggered now to make sure scratch buffer can be overwritten
                   with new input data.*/
                if(idx > 1)
                {
                    retVal = DPU_StaticClutterProc_waitOutData (obj, nextPingPongIdx);
                    if(retVal < 0)
                    {
                        goto exit;
                    }
                }    
                
                if (nextPingPongIdx == DPU_STATICCLUTTERPROC_PONG_IDX)
                {
                    retVal = EDMA_startDmaTransfer(obj->cfg.hwRes.edmaHandle, obj->cfg.hwRes.edmaIn.pong.channel);
                }
                else
                {
                    retVal = EDMA_startDmaTransfer(obj->cfg.hwRes.edmaHandle, obj->cfg.hwRes.edmaIn.ping.channel);
                }  
                if(retVal < 0)
                {
                    goto exit;
                }
            } 
            
            /* verify that previous DMA has completed */
            retVal = DPU_StaticClutterProc_waitInData (obj, pingPongIdx);
            if(retVal < 0)
            {
                goto exit;
            }
           
            /* Execute the clutter removal algorithm*/ 
            DPU_StaticClutterProc_clutterRemoval(obj, 
                                                 (cmplx16ImRe_t *)&scratchBase[pingPongIdx * obj->sizePingBufSamples]);
   
            /* Move data back to L3*/
            if (pingPongIdx == DPU_STATICCLUTTERPROC_PONG_IDX)
            {
                retVal = EDMA_startDmaTransfer(obj->cfg.hwRes.edmaHandle, obj->cfg.hwRes.edmaOut.pong.channel);
            }
            else
            {
                retVal = EDMA_startDmaTransfer(obj->cfg.hwRes.edmaHandle, obj->cfg.hwRes.edmaOut.ping.channel);
            }  
            if(retVal < 0)
            {
                goto exit;
            }
            
            pingPongIdx ^= 1; 
        }
    }    

    /*Wait for last transfer to finish*/
    /*Need to reset the index to the previous one that was sent out*/
    pingPongIdx ^= 1; 
    retVal = DPU_StaticClutterProc_waitOutData (obj, pingPongIdx);
    if(retVal < 0)
    {
        goto exit;
    }

exit:    
    return retVal;
}

int32_t DPU_StaticClutterProc_config
(
    DPU_StaticClutterProc_Handle    handle,
    DPU_StaticClutterProc_Config    *cfg
)
{
    int32_t     retVal = 0;
    DPU_StaticClutterProc_Obj *obj;

    /* Validate params */
    if(!handle ||
       !cfg ||
       !cfg->hwRes.edmaHandle ||
       !cfg->hwRes.radarCube.data ||
       !cfg->hwRes.scratchBuf.buf ||
       !cfg->staticCfg.numDopplerChirps
      )
    {
        retVal = DPU_STATICCLUTTERPROC_EINVAL;
        goto exit;
    }

    obj = (DPU_StaticClutterProc_Obj*) handle;

    /* Check if radar cube format is supported by DPU*/
    if(cfg->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_1)
    {
        retVal = DPU_STATICCLUTTERPROC_EINVAL;
        goto exit;
    }
    
    #ifdef SUBSYS_DSS
    /* Check if scratch buffer has correct alignment*/
    if MEM_IS_NOT_ALIGN(cfg->hwRes.scratchBuf.buf, DPU_STATICCLUTTERPROC_SCRATCHBUFFER_BYTE_ALIGNMENT_DSP)
    {
        retVal = DPU_STATICCLUTTERPROC_ESCRATCHBUF;
        goto exit;
    }
    
    /* The size of each scratch buffer (ping and pong - in samples) must be at minimum the
       smallest multiple of 4 greater or equal to numDopplerChirps.*/
    obj->sizePingBufSamples = ((cfg->staticCfg.numDopplerChirps + 3) & ~0x0003);    
    #else
    /* Check if scratch buffer has correct alignment*/
    if MEM_IS_NOT_ALIGN(cfg->hwRes.scratchBuf.buf, DPU_STATICCLUTTERPROC_SCRATCHBUFFER_BYTE_ALIGNMENT_R4F)
    {
        retVal = DPU_STATICCLUTTERPROC_ESCRATCHBUF;
        goto exit;
    }
    
    /* The size of each scratch buffer (ping and pong - in samples) must be at least as big as numDopplerChirps*/
    obj->sizePingBufSamples = cfg->staticCfg.numDopplerChirps;    
    #endif   
    
    /* Check if scratch buffer is big enough*/
    if(cfg->hwRes.scratchBuf.bufSize < obj->sizePingBufSamples * 2 * sizeof(cmplx16ImRe_t))
    {
        retVal = DPU_STATICCLUTTERPROC_ESCRATCHBUF;
        goto exit;
    }
    
    /* Save config parameters */
    memcpy((void *)&obj->cfg, (void *)cfg, sizeof(DPU_StaticClutterProc_Config));

    /* Configure EDMAs */
    retVal = DPU_StaticClutterProc_configEdma(obj);
    if(retVal < 0)
    {
        goto exit;
    }

exit:    
    return retVal;
}

int32_t DPU_StaticClutterProc_deinit
(
    DPU_StaticClutterProc_Handle   handle
)
{
    int32_t     retVal = 0;
    
    /* Free memory */
    if(handle == NULL)
    {
        retVal = DPU_STATICCLUTTERPROC_EINVAL;
    }
    else
    {
        MemoryP_ctrlFree(handle, sizeof(DPU_StaticClutterProc_Obj));
    }    
    
    return retVal;
}
