/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
 /*
  *  ======== hwa.c ========
  */


#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include <ti/common/sys_common.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>

#include <ti/drivers/hwa/hwa.h>
#include <ti/drivers/hwa/include/reg_dsshwacc.h>
#include <ti/drivers/hwa/include/reg_dsshwaccparam.h>
#include <ti/drivers/hwa/include/hwa_internal.h>


#define HWA_PARAM_CHECK


#define HWA_GET_DRIVER_STRUCT(handle) \
    {\
        ptrHWADriver = (HWA_Driver *)handle;\
    }

extern HWA_HWAttrs gHWAHwCfg[HWA_NUM_INSTANCES];
extern HWA_Driver *gHWADriverPtr[HWA_NUM_INSTANCES];


/* HWA dma destination channel index to EDMA channel id mapping */
uint8_t  gHwaEDMAChanMapping [16] =
{
    EDMA_TPCC0_REQ_HWACC_0,
    EDMA_TPCC0_REQ_HWACC_1,
    EDMA_TPCC0_REQ_HWACC_2,
    EDMA_TPCC0_REQ_HWACC_3,
    EDMA_TPCC0_REQ_HWACC_4,
    EDMA_TPCC0_REQ_HWACC_5,
    EDMA_TPCC0_REQ_HWACC_6,
    EDMA_TPCC0_REQ_HWACC_7,
    EDMA_TPCC0_REQ_HWACC_8,
    EDMA_TPCC0_REQ_HWACC_9,
    EDMA_TPCC0_REQ_HWACC_10,
    EDMA_TPCC0_REQ_HWACC_11,
    EDMA_TPCC0_REQ_HWACC_12,
    EDMA_TPCC0_REQ_HWACC_13,
    EDMA_TPCC0_REQ_HWACC_14,
    EDMA_TPCC0_REQ_HWACC_15
};

/* INTERNAL FUNCTIONS */

/* PROTOTYPES */
static int32_t HWA_getDriverAccess(HWA_Driver *ptrHWADriver, bool checkConfigProgress, bool checkParamProgress, uint32_t paramsetIdx);
static void HWA_releaseDriverAccess(HWA_Driver *ptrHWADriver, bool configProgress, bool paramProgress, uint32_t paramsetIdx);
static int32_t HWA_validateParamSetConfig(HWA_Driver *ptrHWADriver, HWA_ParamConfig *paramConfig);
static void HWA_deleteInstance(HWA_Driver *ptrHWADriver);
static void HWA_paramDoneISR(uintptr_t arg);
static void HWA_allParamDoneISR(uintptr_t arg);
static inline void HWA_doReset(DSSHWACCRegs  *ctrlBaseAddr);



/** @addtogroup HWA_DRIVER_INTERNAL_FUNCTION
 @{ */


/*!
 *  @brief  Function to validate the state of the driver and gain access.
 *
 *  @param  ptrHWADriver         Internal HWA driver object that will be checked for sanity
 *
 *  @param  checkConfigProgress  flag to indicate whether to check driver's configInProgress bit
 *
 *  @param  checkParamProgress   flag to indicate whether to check driver's paramSetMapInProgress bitmap
 *
 *  @param  paramsetIdx          should be valid if checkParamProgress is set to true.
 *
 *  @return 0 if driver access is allowed; error code if access is denied.
 *
 */
static int32_t HWA_getDriverAccess(HWA_Driver *ptrHWADriver, bool checkConfigProgress, bool checkParamProgress, uint32_t paramsetIdx)
{
    int32_t retCode = 0;
    uintptr_t           key;

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

    /* check state of driver */
    if ((ptrHWADriver == NULL)  || (ptrHWADriver->refCnt == 0U))
    {
        retCode = HWA_ENOINIT;
    } 
    else 
    {
        /* check config first */
        if (checkConfigProgress==true)
        {
            if (ptrHWADriver->configInProgress == 1)
            {
                retCode = HWA_EINUSE;
            }
            else
            {
                ptrHWADriver->configInProgress = 1;
            }
        }
        /* next check paramset */
        else if (checkParamProgress==true)
        {
            /*first check for valid index */
            if (paramsetIdx>=ptrHWADriver->hwAttrs->numHwaParamSets)
            {
                retCode = HWA_EINVAL;
            }
            /* next check for inProgress bit */
            else if ((ptrHWADriver->paramSetMapInProgress & (uint32_t)(1U <<paramsetIdx))==1)
            {
                retCode = HWA_EINUSE;
            }
            else
            {   
                /* no error */
                retCode = 0;
                ptrHWADriver->paramSetMapInProgress = 
                    ptrHWADriver->paramSetMapInProgress | (uint32_t)(1U <<paramsetIdx);
        
            }
        }
        else
        {
            /* no error */
            retCode = 0;
        }
    }

    /* Restore the interrupts: */
    HwiP_restore(key);

    return retCode;
}



/*!
 *  @brief  Function to release access to the driver.
 *
 *  @param  ptrHWADriver         Internal HWA driver object that will be checked for sanity
 *
 *  @param  configProgress  flag to indicate whether to release driver's configInProgress bit
 *
 *  @param  paramProgress   flag to indicate whether to release driver's paramSetMapInProgress bitmap
 *
 *  @param  paramsetIdx          should be valid if paramProgress is set to true.
 *
 *  @return none
 *
 */
static void HWA_releaseDriverAccess(HWA_Driver *ptrHWADriver, bool configProgress, bool paramProgress, uint32_t paramsetIdx)
{
    uintptr_t           key;

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

    /* check config first */
    if (configProgress==true)
    {
        ptrHWADriver->configInProgress = 0;
    }
    /* next check paramset */
    if (paramProgress==true)
    {
        ptrHWADriver->paramSetMapInProgress=
            (ptrHWADriver->paramSetMapInProgress & (uint32_t)(~(1U <<paramsetIdx)));
    }

    /* Restore the interrupts: */
    HwiP_restore(key);

    return;
}


/*!
 *  @brief  Function to validate the user passed paramConfig. This function gets compiled out
 *          if HWA_PARAM_CHECK is disabled.
 *
 *  @param  ptrHWADriver         Internal HWA driver object that will be checked for sanity
 *
 *  @param  paramConfig          user supplied paramSet Config
 *
 *  @return 0 if valid paramSet config if provided else a valid error code.
 *
 */
static int32_t HWA_validateParamSetConfig(HWA_Driver *ptrHWADriver, HWA_ParamConfig *paramConfig)
{
    int32_t retCode = 0;
#ifdef HWA_PARAM_CHECK
    if (paramConfig == NULL) 
    {
        /* invalid config */
        retCode = HWA_EINVAL;
    }
    else if (
            (paramConfig->triggerMode>HWA_TRIG_MODE_DMA) ||
            (paramConfig->dmaTriggerSrc>(ptrHWADriver->hwAttrs->numHwaParamSets-1)) ||
            (paramConfig->accelMode>HWA_ACCELMODE_NONE) ||
            (paramConfig->source.srcAcnt>=(1<<12)) || 
            (paramConfig->source.srcBcnt>=(1<<12)) || 
            (paramConfig->source.srcShift>=(1<<12)) || 
            (paramConfig->source.srcCircShiftWrap>=(1<<4)) || 
            (paramConfig->source.srcRealComplex>HWA_SAMPLES_FORMAT_REAL) || 
            (paramConfig->source.srcWidth>HWA_SAMPLES_WIDTH_32BIT) || 
            (paramConfig->source.srcSign>HWA_SAMPLES_SIGNED) || 
            (paramConfig->source.srcConjugate>HWA_FEATURE_BIT_ENABLE) || 
            (paramConfig->source.srcScale>8) || 
            (paramConfig->source.bpmEnable>HWA_FEATURE_BIT_ENABLE) || 
            (paramConfig->source.bpmPhase>=(1<<4)) || 
            (paramConfig->dest.dstAcnt>=(1<<12)) ||
            (paramConfig->dest.dstRealComplex>HWA_SAMPLES_FORMAT_REAL) ||
            (paramConfig->dest.dstWidth>HWA_SAMPLES_WIDTH_32BIT) ||
            (paramConfig->dest.dstSign>HWA_SAMPLES_SIGNED) ||
            (paramConfig->dest.dstConjugate>HWA_FEATURE_BIT_ENABLE) ||
            (paramConfig->dest.dstScale>8) ||
            (paramConfig->dest.dstSkipInit>=(1<<10)) ||
            ((paramConfig->accelMode!=HWA_ACCELMODE_NONE) && (paramConfig->source.srcAddr==paramConfig->dest.dstAddr))
           )
    {
    
        /* invalid config params */
        retCode = HWA_EINVAL;
    }
    else if ((paramConfig->accelMode==HWA_ACCELMODE_FFT) &&
             (  /* if FFT mode, then fftEn should be checked for correct values */
                (paramConfig->accelModeArgs.fftMode.fftEn>HWA_FEATURE_BIT_ENABLE) ||
                ( /* if fftEn is set to Enable, then fftSize should be checked for correct values */
                    (paramConfig->accelModeArgs.fftMode.fftEn==HWA_FEATURE_BIT_ENABLE) && 
                    (   
                        (paramConfig->accelModeArgs.fftMode.fftSize<1) ||
                        (paramConfig->accelModeArgs.fftMode.fftSize>10)
                    )
                 ) ||
                (paramConfig->accelModeArgs.fftMode.butterflyScaling>=(1<<10)) ||
                (paramConfig->accelModeArgs.fftMode.interfZeroOutEn>HWA_FEATURE_BIT_ENABLE) ||
                (paramConfig->accelModeArgs.fftMode.windowEn>HWA_FEATURE_BIT_ENABLE) ||
                (paramConfig->accelModeArgs.fftMode.windowStart>=(1<<10)) ||
                (paramConfig->accelModeArgs.fftMode.winSymm>HWA_FFT_WINDOW_SYMMETRIC) ||
                (paramConfig->accelModeArgs.fftMode.winInterpolateMode>HWA_FFT_WINDOW_INTERPOLATE_MODE_2K) ||
                (paramConfig->accelModeArgs.fftMode.magLogEn>HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED) ||
                (paramConfig->accelModeArgs.fftMode.fftOutMode>HWA_FFT_MODE_OUTPUT_SUM_STATS)
             )
            )
    {
        /* invalid config params */
        retCode = HWA_EINVAL;
    }
    else if ((paramConfig->accelMode==HWA_ACCELMODE_CFAR) &&
             (
                (paramConfig->accelModeArgs.cfarMode.numNoiseSamplesLeft>=(1<<6)) ||
                (paramConfig->accelModeArgs.cfarMode.numNoiseSamplesRight>=(1<<6)) ||
                (paramConfig->accelModeArgs.cfarMode.numGuardCells>=(1<<3)) ||
                (paramConfig->accelModeArgs.cfarMode.nAvgDivFactor>8) ||
                (paramConfig->accelModeArgs.cfarMode.nAvgMode>HWA_NOISE_AVG_MODE_CFAR_CASO) ||
                (paramConfig->accelModeArgs.cfarMode.operMode>HWA_CFAR_OPER_MODE_MAG_SQR_INPUT_COMPLEX) ||
                (paramConfig->accelModeArgs.cfarMode.outputMode>HWA_CFAR_OUTPUT_MODE_I_PEAK_IDX_Q_CUT) ||
                (paramConfig->accelModeArgs.cfarMode.peakGroupEn>HWA_FEATURE_BIT_ENABLE) ||
                (paramConfig->accelModeArgs.cfarMode.cyclicModeEn>HWA_FEATURE_BIT_ENABLE)
             )
            )
    {
        /* invalid config params */
        retCode = HWA_EINVAL;
    }
    else
    {
        retCode = 0;
    }
#endif
    return retCode;
}


/*!
 *  @brief  Function to de-initialize HWA specified by the passed driver object. No driver
 *  functions should be invoked after this call.
 *
 *  @pre    HWA_open() has been called
 *
 *  @param  ptrHWADriver         Internal HWA driver object
 *
 *  @return none
 *
 *  @sa     HWA_close()
 */
static void HWA_deleteInstance(HWA_Driver *ptrHWADriver)
{
    uint32_t            memReqSize = 0;
    uint32_t            index  = ptrHWADriver->instanceNum;
    
    /* un-register the interrupts */
    if (ptrHWADriver->hwiHandleParamSet!=NULL)
    {
        HwiP_delete(ptrHWADriver->hwiHandleParamSet);
        ptrHWADriver->hwiHandleParamSet=NULL;
    }
    if (ptrHWADriver->hwiHandleDone!=NULL)
    {
        HwiP_delete(ptrHWADriver->hwiHandleDone);
        ptrHWADriver->hwiHandleDone=NULL;
    }
    /* free the allocated memory */
    memReqSize = (uint32_t)(sizeof(HWA_InterruptCtx) * ptrHWADriver->hwAttrs->numHwaParamSets);
    if (ptrHWADriver->interruptCtxParamSet != NULL) {
        MemoryP_ctrlFree(ptrHWADriver->interruptCtxParamSet,memReqSize);
        ptrHWADriver->interruptCtxParamSet = NULL;
    }
    MemoryP_ctrlFree(ptrHWADriver,(uint32_t)sizeof(HWA_Driver));
    gHWADriverPtr[index]=NULL; /* reset the driver's cached handle too */

}


/*!
 *  @brief  Function to handle each paramset completion ISR
 *
 *  @pre    HWA_open() has been called
 *
 *  @param  arg         Internal HWA driver object
 *
 *  @return none
 *
 *  @sa     HWA_enableParamSetInterrupt()
 */
static void HWA_paramDoneISR(uintptr_t arg)
{
    HWA_Driver          *ptrHWADriver = NULL;
    uint32_t   paramDoneFlag;
    uint32_t            loopCnt;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(arg); /* this fills the ptrHWADriver */
    
    if ((ptrHWADriver != NULL) && (ptrHWADriver->refCnt > 0U)) 
    {
        ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;

        /* read the interrupt flag register */
        paramDoneFlag = CSL_FEXTR(ctrlBaseAddr->HWACCREG4,15U,0U);
        /* clear the interrupt flag by writing 1 to it */
        ctrlBaseAddr->HWACCREG4 = CSL_FINSR(ctrlBaseAddr->HWACCREG4,31U,16U,paramDoneFlag);

        /* now process the interrupt */
        for(loopCnt = 0U; (paramDoneFlag != 0U) && (loopCnt < ptrHWADriver->hwAttrs->numHwaParamSets); loopCnt++)
        {
            if((paramDoneFlag & ((uint32_t)1U << loopCnt)) != 0U)
            {
                HWA_InterruptCtx *interruptCtx = &ptrHWADriver->interruptCtxParamSet[loopCnt];
                if ((interruptCtx->bIsEnabled==1U) && (interruptCtx->callbackFn != NULL))
                {
                    (interruptCtx->callbackFn)(loopCnt, interruptCtx->callbackArg);
                }
                paramDoneFlag &= ~((uint32_t)1U << loopCnt);
            }
        }
    }
    else
    {
        /* Throw fatal error as driver is not in valid state */
        DebugP_assert (0U);
    }
}



/*!
 *  @brief  Function to handle completion ISR after all paramset are executed
 *
 *  @pre    HWA_open() has been called
 *
 *  @param  arg         Internal HWA driver object
 *
 *  @return none
 *
 *  @sa     HWA_enableDoneInterrupt()
 */
static void HWA_allParamDoneISR(uintptr_t arg)
{
    HWA_Driver          *ptrHWADriver = NULL;

    HWA_GET_DRIVER_STRUCT(arg); /* this fills the ptrHWADriver */
    
    if ((ptrHWADriver != NULL) && (ptrHWADriver->refCnt > 0U)) 
    {
        /* now process the interrupt */
        HWA_DoneInterruptCtx *interruptCtx = &ptrHWADriver->interruptCtxDone;
        if ((interruptCtx->bIsEnabled==true) && (interruptCtx->callbackFn != NULL))
        {
            (interruptCtx->callbackFn)(interruptCtx->callbackArg);
        }
    }
    else
    {
        /* Fatal error as driver is not in valid state */
        DebugP_assert (0U);
    }
}


/*!
 *  @brief  Function to perform the hardware reset of HWA peripheral
 *
 *  @param  ctrlBaseAddr         HWA peripheral's control base address
 *
 *  @return none
 *
 *  @sa     HWA_close(), HWA_open(), HWA_reset()
 */
static inline void HWA_doReset(DSSHWACCRegs  *ctrlBaseAddr)
{
    ctrlBaseAddr->HWACCREG1 = CSL_FINSR (ctrlBaseAddr->HWACCREG1, 6U, 4U, 0x7U);
    ctrlBaseAddr->HWACCREG1 = CSL_FINSR (ctrlBaseAddr->HWACCREG1, 6U, 4U, 0x0U);

    return;
}

/** @}*/


/* EXTERNAL FUNCTIONS */


/*!
 *  @brief  Function to initialize the HWA module
 *
 *  @pre    This function must be called once per system and before
 *          any other HWA driver APIs. It resets the HWA H/W instances in the system.
 *
 */
void HWA_init(void)
{
    return;
}

/*!
 *  @brief  Function to initialize HWA specified by the
 *  particular index value. 
 *
 *  @pre    HWA_init() has been called
 *
 *  @param  index         HWA instance number 
 *
 *  @param  socHandle     SOC Handle (can be obtained used SOC_init by the caller) 
 *
 *  @param  errCode       [out] valid errorCode if NULL handle returned.
 *
 *  @return A HWA_Handle upon success. NULL if an error occurs.
 *
 *  @sa     HWA_init()
 *  @sa     HWA_close()
 */
HWA_Handle HWA_open(uint32_t  index, SOC_Handle socHandle, int32_t* errCode)
{
    HWA_Handle          handle = NULL;
    HwiP_Params         hwiParams;
    uint32_t            memReqSize = 0;
    uintptr_t           key;
    int32_t             retCode = 0;

    if ((index >= HWA_NUM_INSTANCES) || (socHandle == NULL))
    {
        retCode = HWA_EOUTOFRANGE;
    }
    else /* start of if valid index */
    {
        
        /* Disable preemption while opening the driver */
        key = HwiP_disable();
        
        /*
         * check if driver is already init 
         */
        if (gHWADriverPtr[index] == NULL)
        {
            /* Allocate memory for the driver and initialize it */
            gHWADriverPtr[index] = MemoryP_ctrlAlloc ((uint32_t)sizeof(HWA_Driver), 0U);
            if (gHWADriverPtr[index] == NULL)
            {
                /* Error: Out of memory */
                DebugP_log2 ("Debug: HWA Driver (%d) Out of memory (requested size: %d)\n",index,(uint32_t)sizeof(HWA_Driver));
                retCode = HWA_EOUTOFMEM;
            }
            else
            {
                memset ((void *)gHWADriverPtr[index], 0U, sizeof(HWA_Driver));
                gHWADriverPtr[index]->hwAttrs = &gHWAHwCfg[index];
                gHWADriverPtr[index]->instanceNum = index;
                gHWADriverPtr[index]->socHandle = socHandle;
                /* initialize internal driver struture memory */
                /* malloc interruptCtrx */
                memReqSize = (uint32_t)(sizeof(HWA_InterruptCtx) * gHWADriverPtr[index]->hwAttrs->numHwaParamSets);
                gHWADriverPtr[index]->interruptCtxParamSet = (HWA_InterruptCtx *)MemoryP_ctrlAlloc (memReqSize, 0U);
                if (gHWADriverPtr[index]->interruptCtxParamSet == NULL)
                {
                    /* Error: Out of memory */
                    DebugP_log2 ("Debug: HWA Driver (%d) Out of memory for interruptCtxParamSet(requested size: %d)\n",
                                  index,memReqSize);
                    retCode = HWA_EOUTOFMEM;
                }
                else 
                {
                    memset(gHWADriverPtr[index]->interruptCtxParamSet,0U,memReqSize);
                    /* dont set the handle yet as we dont know if all the mem allocs are going to succeed */
                }
            }
        }

        /* 
         * now do config if we got the driver memory allocated and primed 
         */
        if ((retCode==0U) && (gHWADriverPtr[index] != NULL) && (gHWADriverPtr[index]->refCnt==0U))
        {
            DSSHWACCRegs *ctrlBaseAddr = (DSSHWACCRegs *)gHWADriverPtr[index]->hwAttrs->ctrlBaseAddr;
            DSSHWACCPARAMRegs *paramBaseAddr = (DSSHWACCPARAMRegs *)(gHWADriverPtr[index]->hwAttrs->paramBaseAddr);


            /* Register interrupt handlers */
            /* Each Paramset done interrupt */
            HwiP_Params_init(&hwiParams);
            hwiParams.name = "HWA-Param";
            hwiParams.arg  = (uintptr_t)gHWADriverPtr[index];
            gHWADriverPtr[index]->hwiHandleParamSet = HwiP_create(gHWADriverPtr[index]->hwAttrs->intNumParamSet, (HwiP_Fxn)HWA_paramDoneISR, &hwiParams);
            DebugP_log2 ("Debug: HWA Driver Registering HWI ISR [%p] for a Paramset Done Interrupt %d\n",
                                  gHWADriverPtr[index]->hwiHandleParamSet, gHWADriverPtr[index]->hwAttrs->intNumParamSet);
            /* All programmed Paramsets done interrupt */
            HwiP_Params_init(&hwiParams);
            hwiParams.name = "HWA-Done";
            hwiParams.arg  = (uintptr_t)gHWADriverPtr[index];
            hwiParams.enableInt = 0; /* keep the interrupt disabled until user enables it via HWA_EnableInterrupt_Global*/
            gHWADriverPtr[index]->hwiHandleDone = HwiP_create(gHWADriverPtr[index]->hwAttrs->intNumDone, (HwiP_Fxn)HWA_allParamDoneISR, &hwiParams);
            DebugP_log2 ("Debug: HWA Driver Registering HWI ISR [%p] for all Paramsets Done Interrupt %d\n",
                                  gHWADriverPtr[index]->hwiHandleDone, gHWADriverPtr[index]->hwAttrs->intNumDone);
            
            /*enable access to hardware accelerator config memory*/
            if (SOC_enableHWA (gHWADriverPtr[index]->socHandle, &retCode) == 0)
            {
                /*disable accelerator and clock*/
                ctrlBaseAddr->HWACCREG1 = 0x0U;

                /*now reset the hwa*/
                HWA_doReset(ctrlBaseAddr);
                /*leave hw_acc disabled but enable the clock*/
                ctrlBaseAddr->HWACCREG1 = 0x8U;

                /* reset other registers? */
                memset((void *) paramBaseAddr,0U,sizeof(DSSHWACCPARAMRegs)*gHWADriverPtr[index]->hwAttrs->numHwaParamSets);
            }
        }

        /*
         * check before returning 
         */
        if (retCode == 0U)
        {
            /* increment reference count */
            gHWADriverPtr[index]->refCnt++;
            /* Setup the return handle: */
            handle = (void *)gHWADriverPtr[index];
        }
        else
        {
            handle = NULL;
            /* errCode is already set */
            HWA_deleteInstance(gHWADriverPtr[index]);
        }

        /* Restore the interrupts: */
        HwiP_restore(key);

    }/* end of if valid index */

    /* return */
    if (errCode!=NULL)
    {
        *errCode = retCode;
    }
    return handle;
}


/*!
 *  @brief  Function to close a HWA peripheral specified by the HWA handle
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle      A HWA_Handle returned from HWA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_close(HWA_Handle handle)
{
    HWA_Driver          *ptrHWADriver = NULL;
    int32_t             retCode = 0;
    uintptr_t           key;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* Disable preemption while opening the driver */
    key = HwiP_disable();

    if (ptrHWADriver!=NULL)
    {
        if ((ptrHWADriver->configInProgress == 0U) && (ptrHWADriver->paramSetMapInProgress == 0U))
        {
            /* decrement the refCount and if this is the last reference, delete the instance */
            if (ptrHWADriver->refCnt>0U)
            {
                ptrHWADriver->refCnt--;
            }
            if (ptrHWADriver->refCnt==0U)
            {
                ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
                /* disable the HWA state machine */
                ctrlBaseAddr->HWACCREG1 = CSL_FINSR(ctrlBaseAddr->HWACCREG1,2U,0U,0x0U);
                /* do hardware reset */
                HWA_doReset(ctrlBaseAddr);
                /* what else to reset?? */
                /* delete the instance */
                HWA_deleteInstance(ptrHWADriver);
            }
        }
        else 
        {
            /* return inuse code */
            retCode = HWA_EINUSE;
        }
    }
    else 
    {
        /* return invalid code */
        retCode = HWA_EINVAL;
    }
    /* Restore the interrupts */
    HwiP_restore(key);

    return retCode;
}


/*!
 *  @brief  Function to reset the internal state machine of the HWA
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */

int32_t HWA_reset(HWA_Handle handle)
{
    HWA_Driver          *ptrHWADriver = NULL;
    int32_t             retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */
    
    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
        ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
        HWA_doReset(ctrlBaseAddr);
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }
    
    /* return */
    return retCode;
}


/*!
 *  @brief  Function to set the common HWA configuration parameters 
 *          needed for the next operations/iterations/paramsets of HWA
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  commonConfig    HWA Common Config Parameters
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_configCommon(HWA_Handle handle, HWA_CommonConfig *commonConfig)
{
    HWA_Driver          *ptrHWADriver = NULL;
    int32_t             retCode = 0;
    DSSHWACCRegs *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if (commonConfig == NULL)
        {
            /* invalid config */
            retCode = HWA_EINVAL;
        }
        else if (((commonConfig->configMask & HWA_COMMONCONFIG_MASK_NUMLOOPS) && (commonConfig->numLoops > 4095U)) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_PARAMSTARTIDX) && (commonConfig->paramStartIdx>15U)) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_PARAMSTOPIDX) && (commonConfig->paramStopIdx>15U)) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_FFT1DENABLE) && (commonConfig->fftConfig.fft1DEnable>HWA_FEATURE_BIT_ENABLE)) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_BPMRATE) && (commonConfig->fftConfig.bpmRate>(1<<10U))) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD) && (commonConfig->fftConfig.interferenceThreshold>(1<<24U))) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE) && (commonConfig->fftConfig.twidDitherEnable>HWA_FEATURE_BIT_ENABLE)) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_LFSRSEED) && (commonConfig->fftConfig.lfsrSeed>(1<<29U))) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_FFTSUMDIV) && (commonConfig->fftConfig.fftSumDiv>(1<<5U))) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_CFARTHRESHOLDSCALE) && (commonConfig->cfarConfig.cfarThresholdScale>(1<<18U))) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_I_CMULT_SCALE) && (commonConfig->scalarMult.i_cmult_scale>(1<<21U))) ||
            ((commonConfig->configMask & HWA_COMMONCONFIG_MASK_Q_CMULT_SCALE) && (commonConfig->scalarMult.q_cmult_scale>(1<<21U))))
        {
            /* invalid config params */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
            
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_NUMLOOPS)
            {
                ctrlBaseAddr->HWACCREG1 = CSL_FINSR(ctrlBaseAddr->HWACCREG1,19U,8U,commonConfig->numLoops);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_PARAMSTARTIDX) 
            {
                ctrlBaseAddr->HWACCREG1 = CSL_FINSR(ctrlBaseAddr->HWACCREG1,23U,20U,commonConfig->paramStartIdx);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_PARAMSTOPIDX) 
            {
                ctrlBaseAddr->HWACCREG1 = CSL_FINSR(ctrlBaseAddr->HWACCREG1,27U,24U,commonConfig->paramStopIdx);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_FFT1DENABLE) 
            {
                ctrlBaseAddr->HWACCREG1 = CSL_FINSR(ctrlBaseAddr->HWACCREG1,28U,28U,commonConfig->fftConfig.fft1DEnable);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_BPMRATE) 
            {
                ctrlBaseAddr->HWACCREG7 = CSL_FINSR(ctrlBaseAddr->HWACCREG7,9U,0U,commonConfig->fftConfig.bpmRate);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_BPMPATTERN) 
            {
                ctrlBaseAddr->HWACCREG6 = commonConfig->fftConfig.bpmPattern[0];
                ctrlBaseAddr->HWACCREG5 = commonConfig->fftConfig.bpmPattern[1];
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_INTERFERENCETHRESHOLD) 
            {
                ctrlBaseAddr->HWACCREG8 = CSL_FINSR(ctrlBaseAddr->HWACCREG8,23U,0U,commonConfig->fftConfig.interferenceThreshold);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE) 
            {
                ctrlBaseAddr->HWACCREG7 = CSL_FINSR(ctrlBaseAddr->HWACCREG7,16U,16U,commonConfig->fftConfig.twidDitherEnable);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_LFSRSEED) 
            {
                ctrlBaseAddr->HWACCREG11 = CSL_FINSR(ctrlBaseAddr->HWACCREG11,28U,0U,commonConfig->fftConfig.lfsrSeed);
                /* Pulse (set and reset)single register-bit REG_LFSRLOAD */
                ctrlBaseAddr->HWACCREG11 = CSL_FINSR(ctrlBaseAddr->HWACCREG11,31U,31U,1);
                ctrlBaseAddr->HWACCREG11 = CSL_FINSR(ctrlBaseAddr->HWACCREG11,31U,31U,0);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_FFTSUMDIV) 
            {
                ctrlBaseAddr->HWACCREG8 = CSL_FINSR(ctrlBaseAddr->HWACCREG8,28U,24U,commonConfig->fftConfig.fftSumDiv);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_CFARTHRESHOLDSCALE) 
            {
                ctrlBaseAddr->HWACCREG13 = CSL_FINSR(ctrlBaseAddr->HWACCREG13,17U,0U,commonConfig->cfarConfig.cfarThresholdScale);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_I_CMULT_SCALE) 
            {
                ctrlBaseAddr->HWACCREG9 = CSL_FINSR(ctrlBaseAddr->HWACCREG9,20U,0U,commonConfig->scalarMult.i_cmult_scale);
            }
            if (commonConfig->configMask & HWA_COMMONCONFIG_MASK_Q_CMULT_SCALE) 
            {
                ctrlBaseAddr->HWACCREG10 = CSL_FINSR(ctrlBaseAddr->HWACCREG10,20U,0U,commonConfig->scalarMult.q_cmult_scale);
            }
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to set the HWA configuration parameters for a given paramSet
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  paramsetIdx     A valid paramSet index for which the paramConfig is provided.
 *
 *  @param  paramConfig     HWA ParamSet Config Parameters
 *
 *  @param  dmaConfig       [out] This parameter is set by the driver with values that user
 *                                should use to program the source trigger DMA. user should provide
 *                                a valid buffer here if the triggerMode is set to DMA in paramConfig
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_configParamSet(HWA_Handle handle, uint8_t paramsetIdx, HWA_ParamConfig *paramConfig, HWA_SrcDMAConfig *dmaConfig)
{
    HWA_Driver          *ptrHWADriver = NULL;
    int32_t             retCode = 0;
    DSSHWACCPARAMRegs *paramBaseAddr;
    DSSHWACCRegs *ctrlBaseAddr;
    DSSHWACCPARAMRegs paramReg={0};

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,false,true,paramsetIdx);
    if (retCode==0) 
    {
        /* validate the paramConfig */
        retCode = HWA_validateParamSetConfig(ptrHWADriver,paramConfig);
        if (retCode==0)
        {
            /* valid driver access and valid paramConfig */
            paramBaseAddr = (DSSHWACCPARAMRegs *)(ptrHWADriver->hwAttrs->paramBaseAddr+paramsetIdx*sizeof(DSSHWACCPARAMRegs));

            /* Get paramset interrupt settings which will be preserved */
            paramReg.PARAMn_0 = CSL_FEXTR(paramBaseAddr->PARAMn_0,12U,7U) << 7U;

            /* All register start with value 0 except PARAMn_0 */
            paramReg.PARAMn_0 |= CSL_FMKR(2U,0U,paramConfig->triggerMode);
            if (paramConfig->triggerMode == HWA_TRIG_MODE_DMA)
            {
                paramReg.PARAMn_0 |= CSL_FMKR(6U,3U,paramConfig->dmaTriggerSrc);
                /* provide the user with the DMA programming config to facilitate the DMA triggering of HWA */
                if (dmaConfig != NULL)
                {
                    ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
                    
                    dmaConfig->destAddr = (uint32_t)(&ctrlBaseAddr->HWACCREG2);
                    dmaConfig->srcAddr = (uint32_t)(&ctrlBaseAddr->SIGDMACHxDONE[paramConfig->dmaTriggerSrc]);
                    dmaConfig->aCnt = sizeof(uint32_t); 
                    dmaConfig->bCnt = 1;
                    dmaConfig->cCnt = 1;
                }
            }

            paramReg.PARAMn_0 |= CSL_FMKR(22U,21U,paramConfig->accelMode);
            paramReg.PARAMn_1 |= CSL_FMKR(15U,0U,paramConfig->source.srcAddr);
            paramReg.PARAMn_2 |= CSL_FMKR(11U,0U,paramConfig->source.srcAcnt);
            paramReg.PARAMn_3 |= CSL_FMKR(15U,0U,paramConfig->source.srcAIdx);
            paramReg.PARAMn_5 |= CSL_FMKR(11U,0U,paramConfig->source.srcBcnt);
            paramReg.PARAMn_4 |= CSL_FMKR(15U,0U,paramConfig->source.srcBIdx);
            paramReg.PARAMn_7 |= CSL_FMKR(11U,0U,paramConfig->source.srcShift);
            paramReg.PARAMn_7 |= CSL_FMKR(31U,28U,paramConfig->source.srcCircShiftWrap);
            paramReg.PARAMn_2 |= CSL_FMKR(12U,12U,paramConfig->source.srcRealComplex);
            paramReg.PARAMn_2 |= CSL_FMKR(13U,13U,paramConfig->source.srcWidth);
            paramReg.PARAMn_2 |= CSL_FMKR(14U,14U,paramConfig->source.srcSign);
            paramReg.PARAMn_2 |= CSL_FMKR(15U,15U,paramConfig->source.srcConjugate);
            paramReg.PARAMn_5 |= CSL_FMKR(15U,12U,paramConfig->source.srcScale);
            paramReg.PARAMn_0 |= CSL_FMKR(13U,13U,paramConfig->source.bpmEnable);
            paramReg.PARAMn_6 |= CSL_FMKR(11U,8U,paramConfig->source.bpmPhase);

            paramReg.PARAMn_1 |= CSL_FMKR(31U,16U,paramConfig->dest.dstAddr);
            paramReg.PARAMn_2 |= CSL_FMKR(27U,16U,paramConfig->dest.dstAcnt);
            paramReg.PARAMn_3 |= CSL_FMKR(31U,16U,paramConfig->dest.dstAIdx);
            paramReg.PARAMn_4 |= CSL_FMKR(31U,16U,paramConfig->dest.dstBIdx);
            paramReg.PARAMn_2 |= CSL_FMKR(28U,28U,paramConfig->dest.dstRealComplex);
            paramReg.PARAMn_2 |= CSL_FMKR(29U,29U,paramConfig->dest.dstWidth);
            paramReg.PARAMn_2 |= CSL_FMKR(30U,30U,paramConfig->dest.dstSign);
            paramReg.PARAMn_2 |= CSL_FMKR(31U,31U,paramConfig->dest.dstConjugate);
            paramReg.PARAMn_5 |= CSL_FMKR(19U,16U,paramConfig->dest.dstScale);
            paramReg.PARAMn_5 |= CSL_FMKR(29U,20U,paramConfig->dest.dstSkipInit);

            if (paramConfig->accelMode==HWA_ACCELMODE_FFT) 
            {
                paramReg.PARAMn_0 |= CSL_FMKR(14U,14U,paramConfig->accelModeArgs.fftMode.fftEn);
                paramReg.PARAMn_6 |= CSL_FMKR(5U,2U,paramConfig->accelModeArgs.fftMode.fftSize);
                paramReg.PARAMn_6 |= CSL_FMKR(31U,22U,paramConfig->accelModeArgs.fftMode.butterflyScaling);
                paramReg.PARAMn_6 |= CSL_FMKR(7U,7U,paramConfig->accelModeArgs.fftMode.interfZeroOutEn);
                paramReg.PARAMn_0 |= CSL_FMKR(15U,15U,paramConfig->accelModeArgs.fftMode.windowEn);
                paramReg.PARAMn_6 |= CSL_FMKR(21U,12U,paramConfig->accelModeArgs.fftMode.windowStart);
                paramReg.PARAMn_6 |= CSL_FMKR(6U,6U,paramConfig->accelModeArgs.fftMode.winSymm);
                paramReg.PARAMn_7 |= CSL_FMKR(27U,26U,paramConfig->accelModeArgs.fftMode.winInterpolateMode);
                paramReg.PARAMn_0 |= CSL_FMKR(17U,16U,paramConfig->accelModeArgs.fftMode.magLogEn);
                paramReg.PARAMn_0 |= CSL_FMKR(24U,23U,paramConfig->accelModeArgs.fftMode.fftOutMode);
                if (paramConfig->accelModeArgs.fftMode.fftOutMode != HWA_FFT_MODE_OUTPUT_DEFAULT)
                {
                    /* reset the destination parameters if the output is statistics */
                    paramReg.PARAMn_2 |= CSL_FMKR(27U,16U,4095U);
                    paramReg.PARAMn_3 |= CSL_FMKR(31U,16U,8U);
                    paramReg.PARAMn_4 |= CSL_FMKR(31U,16U,8U);
                    paramReg.PARAMn_2 |= CSL_FMKR(28U,28U,0U);
                    paramReg.PARAMn_2 |= CSL_FMKR(29U,29U,1U);
                }
            }

            if (paramConfig->accelMode==HWA_ACCELMODE_CFAR) 
            {
                paramReg.PARAMn_7 |= CSL_FMKR(25U,20U,paramConfig->accelModeArgs.cfarMode.numNoiseSamplesLeft);
                paramReg.PARAMn_7 |= CSL_FMKR(19U,14U,paramConfig->accelModeArgs.cfarMode.numNoiseSamplesRight);
                paramReg.PARAMn_6 |= CSL_FMKR(14U,12U,paramConfig->accelModeArgs.cfarMode.numGuardCells);
                paramReg.PARAMn_6 |= CSL_FMKR(18U,15U,paramConfig->accelModeArgs.cfarMode.nAvgDivFactor);
                paramReg.PARAMn_7 |= CSL_FMKR(13U,12U,paramConfig->accelModeArgs.cfarMode.nAvgMode);
                switch (paramConfig->accelModeArgs.cfarMode.operMode)
                {
                    case HWA_CFAR_OPER_MODE_LOG_INPUT_REAL:           //0xC     // CFAR_LOG_MODE=1, CFAR_INP_MODE=1, CFAR_ABS_MODE=dont care    1100
                        paramReg.PARAMn_0 |= CSL_FMKR(19U,16U,0xCU);
                    break;
                    case HWA_CFAR_OPER_MODE_LOG_INPUT_COMPLEX:        //0x7     // CFAR_LOG_MODE=1, CFAR_INP_MODE=0, CFAR_ABS_MODE=3            0111
                        paramReg.PARAMn_0 |= CSL_FMKR(19U,16U,0x7U);
                    break;
                    case HWA_CFAR_OPER_MODE_MAG_INPUT_REAL:           //0x8     // CFAR_LOG_MODE=0, CFAR_INP_MODE=1, CFAR_ABS_MODE=dont care    1000
                        paramReg.PARAMn_0 |= CSL_FMKR(19U,16U,0x8U);
                    break;
                    case HWA_CFAR_OPER_MODE_MAG_INPUT_COMPLEX:        //0x2     // CFAR_LOG_MODE=0, CFAR_INP_MODE=0, CFAR_ABS_MODE=2            0010
                        paramReg.PARAMn_0 |= CSL_FMKR(19U,16U,0x2U);
                    break;
                    case HWA_CFAR_OPER_MODE_MAG_SQR_INPUT_REAL:       //0x8     // CFAR_LOG_MODE=0, CFAR_INP_MODE=1, CFAR_ABS_MODE=dont care    1000
                        paramReg.PARAMn_0 |= CSL_FMKR(19U,16U,0x8U);
                    break;
                    case HWA_CFAR_OPER_MODE_MAG_SQR_INPUT_COMPLEX:    //0x0     // CFAR_LOG_MODE=0, CFAR_INP_MODE=0, CFAR_ABS_MODE=0            0000
                        paramReg.PARAMn_0 |= CSL_FMKR(19U,16U,0x0U);
                    break;
                    default:
                    break;
                }
                paramReg.PARAMn_7 |= CSL_FMKR(27U,26U,paramConfig->accelModeArgs.cfarMode.outputMode);
                paramReg.PARAMn_0 |= CSL_FMKR(15U,15U,paramConfig->accelModeArgs.cfarMode.peakGroupEn);
                paramReg.PARAMn_0 |= CSL_FMKR(20U,20U,paramConfig->accelModeArgs.cfarMode.cyclicModeEn);
            }
            if ((paramConfig->accelMode==HWA_ACCELMODE_FFT) || (paramConfig->accelMode==HWA_ACCELMODE_NONE))
            {
                if (paramConfig->complexMultiply.mode<=HWA_COMPLEX_MULTIPLY_MODE_VECTOR_MULT_2)
                {
                    paramReg.PARAMn_0 |= CSL_FMKR(20U,18U,paramConfig->complexMultiply.mode);
                    
                    if (paramConfig->complexMultiply.mode==HWA_COMPLEX_MULTIPLY_MODE_FREQ_SHIFTER)
                    {
                        paramReg.PARAMn_7 |= CSL_FMKR(25U,12U,paramConfig->complexMultiply.cmpMulArgs.twidIncrement);
                    }
                    if (paramConfig->complexMultiply.mode==HWA_COMPLEX_MULTIPLY_MODE_SLOW_DFT)
                    {
                        paramReg.PARAMn_7 |= CSL_FMKR(25U,12U,paramConfig->complexMultiply.cmpMulArgs.dft.startFreq);
                        paramReg.PARAMn_6 |= CSL_FMKR(5U,2U,paramConfig->complexMultiply.cmpMulArgs.dft.freqIncrement);
                    }
                    if (paramConfig->complexMultiply.mode==HWA_COMPLEX_MULTIPLY_MODE_FFT_STITCHING)
                    {
                        paramReg.PARAMn_7 |= CSL_FMKR(13U,12U,paramConfig->complexMultiply.cmpMulArgs.twidFactorPattern);
                    }
                }
            }

            /* All register is constructed, write the value to hardware */
            memcpy((void *)paramBaseAddr, (void *)&paramReg, sizeof(DSSHWACCPARAMRegs));

        }
        
        HWA_releaseDriverAccess(ptrHWADriver,false,true,paramsetIdx);
    }

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to get the config to program the DMA for a given DMA Trigger channel.
 *          Application should use the returned config to program the DMA so that it can then 
 *          in turn trigger the paramset. Application should make sure that the channel provided
 *          here in dmaTriggerSrc should match the \ref HWA_ParamConfig_t::dmaTriggerSrc passed
 *          to HWA_configParamSet()
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  dmaTriggerSrc   Same as \ref HWA_ParamConfig_t::dmaTriggerSrc of the paramset for 
 *                          whom this DMA is getting configured
 *
 *  @param  dmaConfig       [out]This parameter is set by the driver with values that user
 *                               should use to program the source trigger DMA. user should provide
 *                               a valid buffer here if the triggerMode is set to DMA in paramConfig
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_getDMAconfig(HWA_Handle handle, uint8_t dmaTriggerSrc, HWA_SrcDMAConfig *dmaConfig)
{
    HWA_Driver          *ptrHWADriver = NULL;
    int32_t             retCode = 0;
    DSSHWACCRegs *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if ((dmaTriggerSrc > (ptrHWADriver->hwAttrs->numHwaParamSets-1)) || (dmaConfig == NULL))
        {
            /* invalid params */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
            dmaConfig->destAddr = (uint32_t)(&ctrlBaseAddr->HWACCREG2);
            dmaConfig->srcAddr = (uint32_t)(&ctrlBaseAddr->SIGDMACHxDONE[dmaTriggerSrc]);
            dmaConfig->aCnt = sizeof(uint32_t); 
            dmaConfig->bCnt = 1;
            dmaConfig->cCnt = 1;
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }
    
    return(retCode);
}

/*!
 *  @brief  Function to get HWA processing Memory information including address, 
 *          size and number of banks.
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  memInfo         Pointer to save HWA processing memory information
 *
  *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_getHWAMemInfo(HWA_Handle handle, HWA_MemInfo *memInfo)
{
    HWA_Driver          *ptrHWADriver = NULL;
    int32_t             retCode = 0;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if (memInfo == NULL)
        {
            /* invalid params */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            memInfo->baseAddress    = ptrHWADriver->hwAttrs->accelMemBaseAddr;
            memInfo->numBanks       = 4U;
            memInfo->bankSize       = ptrHWADriver->hwAttrs->accelMemSize / memInfo->numBanks;
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    return(retCode);
}

/*!
 *  @brief  Function to get the dma destination index with a given EDMA channel number 
 *          This function assumes the EDMA channel number is from the first EDMA instance.
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  edmaChanId      EDMA channell id
 *
 *  @param  hwaDestChan     Pointer to save destination channel index
 *
 *  @return     =0          Success
 *              <0          Error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_getDMAChanIndex(HWA_Handle handle, uint8_t edmaChanId, uint8_t *hwaDestChan)
{
    HWA_Driver      *ptrHWADriver = NULL;
    int32_t         retCode = 0;
    uint8_t         index;
    bool            foundChan = false;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if (hwaDestChan == NULL)
        {
            /* invalid params */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            for(index = 0; index < ptrHWADriver->hwAttrs->numDmaChannels; index ++)
            {
                if(edmaChanId == gHwaEDMAChanMapping[index])
                {
                    foundChan = true;
                    *hwaDestChan = index;
                    break;
                }
            }
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    if(foundChan == false)
    {
        retCode = HWA_EINVAL;
    }
    return(retCode);
}

/*!
 *  @brief  Function to get the edma EDMA channel number from a given HWA paramset destination channel.
 *          This function assumes the EDMA channel number is from the first EDMA instance.
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  hwaDMAdestChan  Destination channle id set in a paramset
 *
 *  @return     >=0         Upon success, EDMA channel number
 *              <0          Error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_getEDMAChanId(HWA_Handle handle, uint8_t hwaDMAdestChan)
{
    HWA_Driver          *ptrHWADriver = NULL;
    int32_t             retCode = 0;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if (hwaDMAdestChan > (ptrHWADriver->hwAttrs->numDmaChannels-1))
        {
            /* invalid params */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            retCode = gHwaEDMAChanMapping[hwaDMAdestChan];
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    return(retCode);
}

/*!
 *  @brief  Function to set the HWA RAM (either Internal RAM or window RAM)
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  ramType         Use defines \ref HWA_RAM_TYPE
 *
 *  @param  data            data pointer that needs to be copied to RAM
 *
 *  @param  dataSize        Size of data to be copied
 *
 *  @param  startIdx        start index (in bytes) within RAM where data needs to be copied
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_configRam(HWA_Handle handle, uint8_t ramType, uint8_t *data, uint32_t dataSize, uint32_t startIdx)
{
    HWA_Driver            *ptrHWADriver = NULL;
    int32_t               retCode = 0;
    DSSHWACCRegs *ctrlBaseAddr;
    uint8_t      *ramBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if (data == NULL)
        {
            /* invalid data */
            retCode = HWA_EINVAL;
        }
        else if ((ramType!=HWA_RAM_TYPE_WINDOW_RAM) && (ramType!=HWA_RAM_TYPE_INTERNAL_RAM))
        {
            /* invalid config params */
            retCode = HWA_EINVAL;
        }
        else if ((dataSize==0) || (dataSize>HWA_RAM_WINDOW_SIZE_IN_BYTES))
        {
            /* invalid data size */
            retCode = HWA_EINVAL;
        }
        else if ((startIdx>=HWA_RAM_WINDOW_SIZE_IN_BYTES) || ((startIdx+dataSize)>HWA_RAM_WINDOW_SIZE_IN_BYTES))
        {
            /* invalid data size */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
            ramBaseAddr = (uint8_t*)ptrHWADriver->hwAttrs->ramBaseAddr;
            /* select the right RAM */
            if (ramType==HWA_RAM_TYPE_INTERNAL_RAM)
            {
                ctrlBaseAddr->HWACCREG7 = CSL_FINSR(ctrlBaseAddr->HWACCREG7,24U,24U,1U);
            }
            else
            {
                ctrlBaseAddr->HWACCREG7 = CSL_FINSR(ctrlBaseAddr->HWACCREG7,24U,24U,0U);
            }
            /* copy the RAM contents */
            memcpy((void *)&ramBaseAddr[startIdx],data,dataSize);
            /* reset the RAM selection - this is needed to avoid paramset corruption */
            ctrlBaseAddr->HWACCREG7 = CSL_FINSR(ctrlBaseAddr->HWACCREG7,24U,24U,0U);
        }
        
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }
    
    /* return */
    return retCode;
}

/*!
 *  @brief  Function to enable the CPU and/or DMA interrupt after a paramSet completion. 
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  paramsetIdx     A valid paramSet index for which the intrConfig is provided.
 *
 *  @param  intrConfig      HWA Interrupt Config Parameters
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_enableParamSetInterrupt(HWA_Handle handle, uint8_t paramsetIdx, HWA_InterruptConfig *intrConfig)
{
    HWA_Driver          *ptrHWADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    DSSHWACCPARAMRegs *paramBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,false,true,paramsetIdx);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if (intrConfig == NULL) 
        {
            /* invalid config */
            retCode = HWA_EINVAL;
        }
        else if ((intrConfig->interruptTypeFlag & HWA_PARAMDONE_INTERRUPT_TYPE_DMA) &&
                 (intrConfig->dma.dstChannel > (ptrHWADriver->hwAttrs->numDmaChannels-1)))
        {
            /* invalid config */
            retCode = HWA_EINVAL;
        }
        else 
#endif
        {
            paramBaseAddr = (DSSHWACCPARAMRegs *)(ptrHWADriver->hwAttrs->paramBaseAddr+paramsetIdx*sizeof(DSSHWACCPARAMRegs));
            if (intrConfig->interruptTypeFlag & HWA_PARAMDONE_INTERRUPT_TYPE_CPU)
            {
                
                /* Disable preemption while setting the registers; since these are accessed via ISR as well */
                key = HwiP_disable();
                
                /* save the interrupt context */
                ptrHWADriver->interruptCtxParamSet[paramsetIdx].bIsEnabled = true;
                ptrHWADriver->interruptCtxParamSet[paramsetIdx].callbackFn = intrConfig->cpu.callbackFn;
                ptrHWADriver->interruptCtxParamSet[paramsetIdx].callbackArg= intrConfig->cpu.callbackArg;

                /* enable the interrupt to CPU in H/W */
                paramBaseAddr->PARAMn_0 = CSL_FINSR(paramBaseAddr->PARAMn_0,7U,7U,1U);

                /* Restore the interrupts */
                HwiP_restore(key);
            }
            if (intrConfig->interruptTypeFlag & HWA_PARAMDONE_INTERRUPT_TYPE_DMA) 
            {
                /* set the destination channel number */
                paramBaseAddr->PARAMn_0 = CSL_FINSR(paramBaseAddr->PARAMn_0,12U,9U,intrConfig->dma.dstChannel);
                /* enable the interrupt to DMA */
                paramBaseAddr->PARAMn_0 = CSL_FINSR(paramBaseAddr->PARAMn_0,8U,8U,1U);
            }
        }
        HWA_releaseDriverAccess(ptrHWADriver,false,true,paramsetIdx);
    }

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to enable the CPU interrupt after all programmed paramSets have been completed. 
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  callbackFn      user defined callback function to be called when this interrupt is generated
 *
 *  @param  callbackArg     user defined callback arg for the callback function
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_enableDoneInterrupt(HWA_Handle handle, HWA_Done_IntHandlerFuncPTR callbackFn, void * callbackArg )
{
    HWA_Driver          *ptrHWADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

    /* check state of driver */
    if ((ptrHWADriver == NULL)  || (ptrHWADriver->refCnt == 0U))
    {
        retCode = HWA_ENOINIT;
    } 
    else 
    {
        /* save the interrupt context */
        ptrHWADriver->interruptCtxDone.bIsEnabled = true;
        ptrHWADriver->interruptCtxDone.callbackFn = callbackFn;
        ptrHWADriver->interruptCtxDone.callbackArg= callbackArg;

        /* enable the interrupt to CPU in H/W */
        HwiP_enableInterrupt(ptrHWADriver->hwAttrs->intNumDone);

        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* Restore the interrupts: */
    HwiP_restore(key);

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to disable the CPU and/or DMA interrupt after a paramSet completion. 
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle              A HWA_Handle returned from HWA_open()
 *
 *  @param  paramsetIdx         A valid paramSet index for which the interrupt is to be disabled
 *
 *  @param  interruptTypeFlag   Flag to indicate if CPU and/or DMA interrupts are to be disabled
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_disableParamSetInterrupt(HWA_Handle handle, uint8_t paramsetIdx, uint8_t interruptTypeFlag)
{
    HWA_Driver          *ptrHWADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    DSSHWACCPARAMRegs *paramBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,false,true,paramsetIdx);
    if (retCode==0) 
    {
        paramBaseAddr = (DSSHWACCPARAMRegs *)(ptrHWADriver->hwAttrs->paramBaseAddr+paramsetIdx*sizeof(DSSHWACCPARAMRegs));

        if (interruptTypeFlag & HWA_PARAMDONE_INTERRUPT_TYPE_CPU)
        {
            
            /* Disable preemption while setting the interrupt context */
            key = HwiP_disable();
            
            /* update the interrupt context */
            ptrHWADriver->interruptCtxParamSet[paramsetIdx].bIsEnabled = false;
            ptrHWADriver->interruptCtxParamSet[paramsetIdx].callbackFn = NULL;
            ptrHWADriver->interruptCtxParamSet[paramsetIdx].callbackArg= NULL;

            /* disable the interrupt to CPU in H/W */
            paramBaseAddr->PARAMn_0 = CSL_FINSR(paramBaseAddr->PARAMn_0,7U,7U,0U);

            /* Restore the interrupts */
            HwiP_restore(key);
        }
        if (interruptTypeFlag & HWA_PARAMDONE_INTERRUPT_TYPE_DMA) 
        {
            /* disable the interrupt to DMA */
            paramBaseAddr->PARAMn_0 = CSL_FINSR(paramBaseAddr->PARAMn_0,8U,8U,0U);
        }
        HWA_releaseDriverAccess(ptrHWADriver,false,true,paramsetIdx);
    }

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to disable the CPU interrupt after all programmed paramSets have been completed. 
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_disableDoneInterrupt(HWA_Handle handle)
{
    HWA_Driver              *ptrHWADriver = NULL;
    uintptr_t                key;
    int32_t                  retCode = 0;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

    /* check state of driver */
    if ((ptrHWADriver == NULL)  || (ptrHWADriver->refCnt == 0U))
    {
        retCode = HWA_ENOINIT;
    } 
    else
    {
        /* save the interrupt context */
        ptrHWADriver->interruptCtxDone.bIsEnabled = false;
        ptrHWADriver->interruptCtxDone.callbackFn = NULL;
        ptrHWADriver->interruptCtxDone.callbackArg= NULL;

        /* enable the interrupt to CPU in H/W */
        HwiP_disableInterrupt(ptrHWADriver->hwAttrs->intNumDone);
    }

    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to enable the state machine of the HWA. This should be called after 
 *          paramset and RAM have been programmed
 *
 *  @pre    HWA_open() HWA_ConfigCommon() HWA_ConfigParamSet HWA_ConfigRam has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  flagEnDis       Enable/Disable Flag: 0-disable, 1-enable
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_enable(HWA_Handle handle, uint8_t flagEnDis)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
        ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
        /* set the ACCENABLE bits to 0x7 */
        if (flagEnDis == 1U)
        {
            ctrlBaseAddr->HWACCREG1 = CSL_FINSR(ctrlBaseAddr->HWACCREG1,2U,0U,0x7U);
        }
        /* set the ACCENABLE bits to 0x0 */
        else
        {
            ctrlBaseAddr->HWACCREG1 = CSL_FINSR(ctrlBaseAddr->HWACCREG1,2U,0U,0x0U);
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to manually trigger the execution of the state machine via software
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_setSoftwareTrigger(HWA_Handle handle)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
        ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
        /* set the CR42ACCTRIG bits to 0x7 */
        ctrlBaseAddr->HWACCREG3 = CSL_FINSR(ctrlBaseAddr->HWACCREG3,0U,0U,1U);

        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to manually trigger the execution of the state machine waiting on DMA via software
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  idx            DMA channel number for whom software should simulate the trigger
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_setDMA2ACCManualTrig(HWA_Handle handle, uint16_t idx)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
        ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
        /* set the DMA2ACCTRIG bits to DMA Channel */
        ctrlBaseAddr->HWACCREG2 = CSL_FINSR(ctrlBaseAddr->HWACCREG2,15U,0U,(1<<idx));

        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to read the Clip Status registers
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  pbuf            pointer to a uint8_t size memory where value of Clip 
 *                          Status Register would be copied
 *
 *  @param  size            size (in bytes) of the pbuf register provided. 
 *                          It should be atleast 2 bytes.
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_readClipStatus(HWA_Handle handle, uint8_t *pbuf, uint8_t size)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;
    uint16_t                *clipStatus;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if ((size<2U) || (pbuf == NULL))
        {
            /* invalid config */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
            clipStatus = (uint16_t*)pbuf;
            /* read the FFTCLCIPSTAT bits of FFTCLIP */
            *clipStatus = (uint16_t)CSL_FEXTR(ctrlBaseAddr->FFTCLIP,9U,0U);
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to clear the Clip Status registers
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_clearClipStatus(HWA_Handle handle)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
        ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
        /* read the FFTCLCIPSTAT bits of FFTCLIP */
        ctrlBaseAddr->FFTCLIP = (uint16_t)CSL_FINSR(ctrlBaseAddr->FFTCLIP,16U,16U,1U);

        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to read the 4 sets of 'MAX' statistics register
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  pStats          pointer to a memory of type HWA_Stats where value of all the  
 *                          Max and statistics Registers would be copied
 *
 *  @param  numIter         number of iterations to read. Value 1-4 should be provided. 
 *                          User is expected to provide enough space for the pStats to hold 'numIter' worth of HWA_Stats 
 *                          Ex: HWA_Stats appHWAStats[3]; HWA_readStatsReg(appHWAhandle,appHWAStats,3);
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_readStatsReg(HWA_Handle handle, HWA_Stats *pStats, uint8_t numIter)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if ((pStats == NULL) || (numIter>4) || (numIter==0))
        {
            /* invalid config */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            uint8_t i=0;
            ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
            /* read the MAX and SUM registers*/
            for (i=0;i<numIter;i++)
            {
                pStats[i].maxValue = (uint32_t)CSL_FEXTR(ctrlBaseAddr->HWACCSTATSREG[i].MAXnVALUE,23U,0U);
                pStats[i].maxIndex = (uint16_t)CSL_FEXTR(ctrlBaseAddr->HWACCSTATSREG[i].MAXnINDEX,11U,0);
                pStats[i].iSumLSB  = (uint32_t)CSL_FEXTR(ctrlBaseAddr->HWACCSTATSREG[i].ISUMnLSB,31U,0U);
                pStats[i].iSumMSB  = (uint8_t)CSL_FEXTR(ctrlBaseAddr->HWACCSTATSREG[i].ISUMnMSB,3U,0U);
                pStats[i].qSumLSB  = (uint32_t)CSL_FEXTR(ctrlBaseAddr->HWACCSTATSREG[i].QSUMnLSB,31U,0U);
                pStats[i].qSumMSB  = (uint8_t)CSL_FEXTR(ctrlBaseAddr->HWACCSTATSREG[i].QSUMnMSB,3U,0U);
            }
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to read the PEAKCNT register
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  pbuf            pointer to a memory where value of the  
 *                          PEAKCNT Registers would be copied
 *
 *  @param  size            size (in bytes) of the pbuf register provided. 
 *                          It should be atleast 2 bytes.
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_readCFARPeakCountReg(HWA_Handle handle, uint8_t *pbuf, uint8_t size)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;
    uint16_t                *peakCnt;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if ((size<2) || (pbuf == NULL))
        {
            /* invalid config */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
            peakCnt = (uint16_t*)pbuf;
            /* read the FFTPEAKCNT register */
            *peakCnt = (uint16_t)CSL_FEXTR(ctrlBaseAddr->FFTPEAKCNT,11U,0U);
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to read the debug registers (paramcurr, loopcou, acc_trig_in_stat)
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @param  pStats          pointer to a memory of type HWA_debugStats where value of the
 *                          RDSTATUS and HWACCREG12 Registers would be copied
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_readDebugReg(HWA_Handle handle, HWA_DebugStats *pStats)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
#ifdef HWA_PARAM_CHECK
        if (pStats == NULL) 
        {
            /* invalid config */
            retCode = HWA_EINVAL;
        }
        else
#endif
        {
            ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
            /* read the Debug registers*/
            pStats->currentParamSet = (uint8_t)CSL_FEXTR(ctrlBaseAddr->RDSTATUS,3U,0U);
            pStats->currentLoopCount = (uint16_t)CSL_FEXTR(ctrlBaseAddr->RDSTATUS,15U,4U);
            pStats->dmaTrigStatus = (uint16_t)CSL_FEXTR(ctrlBaseAddr->HWACCREG12,18U,3U);
            pStats->dfePingPongStatus = (uint8_t)CSL_FEXTR(ctrlBaseAddr->HWACCREG12,2U,2U);
            pStats->swTrigStatus = (uint8_t)CSL_FEXTR(ctrlBaseAddr->HWACCREG12,1U,1U);
        }
        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to clear the debug registers (acc_trig_in_clr)
 *
 *  @pre    HWA_open() has been called.
 *
 *  @param  handle          A HWA_Handle returned from HWA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     HWA_open()
 */
int32_t HWA_clearDebugReg(HWA_Handle handle)
{
    HWA_Driver              *ptrHWADriver = NULL;
    int32_t                  retCode = 0;
    DSSHWACCRegs   *ctrlBaseAddr;

    HWA_GET_DRIVER_STRUCT(handle); /* this fills the ptrHWADriver */

    /* validate driver acccess */
    retCode = HWA_getDriverAccess(ptrHWADriver,true,false,0);
    if (retCode==0) 
    {
        ctrlBaseAddr = (DSSHWACCRegs *)ptrHWADriver->hwAttrs->ctrlBaseAddr;
        /* clear the Debug Reg */
        ctrlBaseAddr->HWACCREG12 = (uint16_t)CSL_FINSR(ctrlBaseAddr->HWACCREG12,24U,24U,1U);

        HWA_releaseDriverAccess(ptrHWADriver,true,false,0);
    }

    /* return */
    return retCode;
}


