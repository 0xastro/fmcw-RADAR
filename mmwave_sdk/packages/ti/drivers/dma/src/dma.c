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
  *  ======== dma.c ========
  */
 
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <ti/common/sys_common.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>

#include <ti/drivers/dma/dma.h>
#include <ti/drivers/dma/include/reg_dma.h>
#include <ti/drivers/dma/include/reg_dmaram.h>
#include <ti/drivers/dma/include/dma_internal.h>

#define DMA_PARAM_CHECK


extern DMA_HWAttrs gDMAHwCfg[DMA_NUM_INSTANCES];
extern DMA_Driver *gDMADriverPtr[DMA_NUM_INSTANCES];


/*  Internal Function Prototypes */
static void DMA_ftcISR(uintptr_t arg);
static void DMA_btcISR(uintptr_t arg);
static void DMA_berISR(uintptr_t arg);
static void DMA_lfsISR(uintptr_t arg);
static void DMA_hbcISR(uintptr_t arg);
static void DMA_deleteInstance(DMA_Driver *ptrDMADriver);


/* Default DMA parameters structure */
DMA_Params DMA_defaultParams = {
    DMA_PriScheme_Rotation,
    DMA_PriScheme_Rotation,
    false,
    DMA_DebugMode_Immediate
};


#define DMA_GET_DRIVER_STRUCT(handle) \
        {\
            ptrDMADriver = (DMA_Driver *)(handle);\
        }

/** @addtogroup DMA_DRIVER_INTERNAL_FUNCTION
 @{ */

/*!
 *  @brief  Frame transfer completer interrupt handler
 *
 *  @details The handler performs the following:
 *           - read the flag registers in local variable
 *           - clear the pending interrupts based on the read value
 *           - processes the channels for whom the interrupts were read from flag register. If application
 *             registered a callback for this channels FTC interrupt, then that callback is called.
 *
 *  @param  arg   arg registered by driver when registering this handler for FTC interrupt using HwiP_create
 *
 *  @return nothing
 *
 *  @sa     DMA_open()
 */
 static void DMA_ftcISR(uintptr_t arg)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uint32_t            ftcFlag, ftcEn;
    uint32_t            loopCnt;

    DMA_GET_DRIVER_STRUCT(arg); /* this fills the ptrDMADriver */
    
    if ((ptrDMADriver != NULL) && (ptrDMADriver->refCnt > 0U)) 
    {
        volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
        /* read the interrupt flag register */
        ftcFlag = ctrlBaseAddr->FTCFLAG;
        /* filter the pending ISR based on the enable bit */
        ftcEn = ctrlBaseAddr->FTCINTENAS;
        ftcFlag &= ftcEn;
        /* clear the interrupt flag by writing 1 to it */
        ctrlBaseAddr->FTCFLAG = ftcFlag;

        /* now process the interrupt */
        for(loopCnt = 0U; (ftcFlag != 0U) && (loopCnt < ptrDMADriver->hwAttrs->numDmaChannels); loopCnt++)
        {
            if((ftcFlag & ((uint32_t)1U << loopCnt)) != 0U)
            {
                DMA_InterruptCtx *interruptCtx = &ptrDMADriver->interruptCtx[DMA_IntType_FTC][loopCnt];
                if((interruptCtx!=NULL) && (interruptCtx->callbackFn != NULL))
                {
                    (interruptCtx->callbackFn)(loopCnt, DMA_IntType_FTC, interruptCtx->callbackArg);
                }
                ftcFlag &= ~((uint32_t)1U << loopCnt);
            }
        }
    }
    else
    {
        /* Ignore interrupt as driver is not in valid state */
        DebugP_assert (0);
    }
}

/*!
 *  @brief  Last frame transfer start interrupt handler
 *
 *  @details The handler performs the following:
 *           - read the flag registers in local variable
 *           - clear the pending interrupts based on the read value
 *           - processes the channels for whom the interrupts were read from flag register. If application
 *             registered a callback for this channels LFS interrupt, then that callback is called.
 *
 *  @param  arg   arg registered by driver when registering this handler for LFS interrupt using HwiP_create
 *
 *  @return nothing
 *
 *  @sa     DMA_open()
 */
static void DMA_lfsISR(uintptr_t arg)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uint32_t            lfsFlag, lfsEn;
    uint32_t            loopCnt;
    
    DMA_GET_DRIVER_STRUCT(arg); /* this fills the ptrDMADriver */
    
    if ((ptrDMADriver != NULL) && (ptrDMADriver->refCnt > 0U)) 
    {
        volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
        /* read the interrupt flag register */
        lfsFlag = ctrlBaseAddr->LFSFLAG;
        /* filter the pending ISR based on the enable bit */
        lfsEn = ctrlBaseAddr->LFSINTENAS;
        lfsFlag &= lfsEn;
        /* clear the interrupt flag by writing 1 to it */
        ctrlBaseAddr->LFSFLAG = lfsFlag;

        /* now process the interrupt */
        for(loopCnt = 0U; (lfsFlag != 0U) && (loopCnt < ptrDMADriver->hwAttrs->numDmaChannels); loopCnt++)
        {
            if((lfsFlag & ((uint32_t)1U << loopCnt)) != 0U)
            {
                DMA_InterruptCtx *interruptCtx = &ptrDMADriver->interruptCtx[DMA_IntType_LFS][loopCnt];
                if((interruptCtx!=NULL) && (interruptCtx->callbackFn != NULL))
                {
                    (interruptCtx->callbackFn)(loopCnt, DMA_IntType_LFS,interruptCtx->callbackArg);
                }
                lfsFlag &= ~((uint32_t)1U << loopCnt);
            }
        }
    }
    else
    {
        /* Ignore interrupt as driver is not in valid state */
        DebugP_assert (0);
    }
}


/*!
 *  @brief  Half block transfer complete interrupt handler
 *
 *  @details The handler performs the following:
 *           - read the flag registers in local variable
 *           - clear the pending interrupts based on the read value
 *           - processes the channels for whom the interrupts were read from flag register. If application
 *             registered a callback for this channels HBC interrupt, then that callback is called.
 *
 *  @param  arg   arg registered by driver when registering this handler for HBC interrupt using HwiP_create
 *
 *  @return nothing
 *
 *  @sa     DMA_open()
 */
static void DMA_hbcISR(uintptr_t arg)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uint32_t            hbcFlag, hbcEn;
    uint32_t            loopCnt;
    
    DMA_GET_DRIVER_STRUCT(arg); /* this fills the ptrDMADriver */
    
    if ((ptrDMADriver != NULL) && (ptrDMADriver->refCnt > 0U)) 
    {
        volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
        /* read the interrupt flag register */
        hbcFlag = ctrlBaseAddr->HBCFLAG;
        /* filter the pending ISR based on the enable bit */
        hbcEn = ctrlBaseAddr->HBCINTENAS;
        hbcFlag &= hbcEn;
        /* clear the interrupt flag by writing 1 to it */
        ctrlBaseAddr->HBCFLAG = hbcFlag;

        /* now process the interrupt */
        for(loopCnt = 0U; (hbcFlag != 0U) && (loopCnt < ptrDMADriver->hwAttrs->numDmaChannels); loopCnt++)
        {
            if((hbcFlag & ((uint32_t)1U << loopCnt)) != 0U)
            {
                DMA_InterruptCtx *interruptCtx = &ptrDMADriver->interruptCtx[DMA_IntType_HBC][loopCnt];
                if((interruptCtx!=NULL) && (interruptCtx->callbackFn != NULL))
                {
                    (interruptCtx->callbackFn)(loopCnt, DMA_IntType_HBC,interruptCtx->callbackArg);
                }
                hbcFlag &= ~((uint32_t)1U << loopCnt);
            }
        }
    }
    else
    {
        /* Ignore interrupt as driver is not in valid state */
        DebugP_assert (0);
    }
}

/*!
 *  @brief  Block transfer completer interrupt handler
 *
 *  @details The handler performs the following:
 *           - read the flag registers in local variable
 *           - clear the pending interrupts based on the read value
 *           - processes the channels for whom the interrupts were read from flag register. If application
 *             registered a callback for this channels BTC interrupt, then that callback is called.
 *
 *  @param  arg   arg registered by driver when registering this handler for BTC interrupt using HwiP_create
 *
 *  @return nothing
 *
 *  @sa     DMA_open()
 */
static void DMA_btcISR(uintptr_t arg)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uint32_t            btcFlag, btcEn;
    uint32_t            loopCnt;
    
    DMA_GET_DRIVER_STRUCT(arg); /* this fills the ptrDMADriver */
    
    if ((ptrDMADriver != NULL) && (ptrDMADriver->refCnt > 0U)) 
    {
        volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
        /* read the interrupt flag register */
        btcFlag = ctrlBaseAddr->BTCFLAG;
        /* filter the pending ISR based on the enable bit */
        btcEn = ctrlBaseAddr->BTCINTENAS;
        btcFlag &= btcEn;
        /* clear the interrupt flag by writing 1 to it */
        ctrlBaseAddr->BTCFLAG = btcFlag;

        /* now process the interrupt */
        for(loopCnt = 0U; (btcFlag != 0U) && (loopCnt < ptrDMADriver->hwAttrs->numDmaChannels); loopCnt++)
        {
            if((btcFlag & ((uint32_t)1U << loopCnt)) != 0U)
            {
                DMA_InterruptCtx *interruptCtx = &ptrDMADriver->interruptCtx[DMA_IntType_BTC][loopCnt];
                if((interruptCtx!=NULL) && (interruptCtx->callbackFn != NULL))
                {
                    (interruptCtx->callbackFn)(loopCnt, DMA_IntType_BTC,interruptCtx->callbackArg);
                }
                btcFlag &= ~((uint32_t)1U << loopCnt);
            }
        }
    }
    else
    {
        /* Ignore interrupt as driver is not in valid state */
        DebugP_assert (0);
    }
}

/*!
 *  @brief  Bus error interrupt handler
 *
 *  @details The handler performs the following:
 *           - read the flag registers in local variable
 *           - clear the pending interrupts based on the read value
 *           - processes the channels for whom the interrupts were read from flag register. If application
 *             registered a callback for this channels BER interrupt, then that callback is called.
 *
 *  @param  arg   arg registered by driver when registering this handler for BER interrupt using HwiP_create
 *
 *  @return nothing
 *
 *  @sa     DMA_open()
 */
static void DMA_berISR(uintptr_t arg)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uint32_t            berFlag, berEn;
    uint32_t            loopCnt;
    
    DMA_GET_DRIVER_STRUCT(arg); /* this fills the ptrDMADriver */
    
    if ((ptrDMADriver != NULL) && (ptrDMADriver->refCnt > 0U)) 
    {
        volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
        /* read the interrupt flag register */
        berFlag = ctrlBaseAddr->BERFLAG;
        /* filter the pending ISR based on the enable bit */
        berEn = ctrlBaseAddr->GCHIENAS;
        berFlag &= berEn;
        /* clear the interrupt flag by writing 1 to it */
        ctrlBaseAddr->BERFLAG = berFlag;

        /* now process the interrupt */
        for(loopCnt = 0U; (berFlag != 0U) && (loopCnt < ptrDMADriver->hwAttrs->numDmaChannels); loopCnt++)
        {
            if((berFlag & ((uint32_t)1U << loopCnt)) != 0U)
            {
                DMA_InterruptCtx *interruptCtx = &ptrDMADriver->interruptCtx[DMA_IntType_BER][loopCnt];
                if((interruptCtx!=NULL) && (interruptCtx->callbackFn != NULL))
                {
                    (interruptCtx->callbackFn)(loopCnt, DMA_IntType_BER,interruptCtx->callbackArg);
                }
                berFlag &= ~((uint32_t)1U << loopCnt);
            }
        }
    }
    else
    {
        /* Ignore interrupt as driver is not in valid state */
        DebugP_assert (0);
    }
}


/*!
 *  @brief  Internal API to free and delete the driver instance
 *
 *  @param  ptrDMADriver   valid DMA driver object (internal)
 *
 *  @return nothing
 *
 *  @sa     DMA_close()
 */
static void DMA_deleteInstance(DMA_Driver *ptrDMADriver)
{
    uint32_t            intNum;
    uint32_t            memReqSize = 0U;
    uint32_t            index  = ptrDMADriver->instanceNum;
    
    /* un-register the interrupts */
    if (ptrDMADriver->hwiHandle[DMA_IntType_FTC]!=NULL)
    {
        HwiP_delete(ptrDMADriver->hwiHandle[DMA_IntType_FTC]);
        ptrDMADriver->hwiHandle[DMA_IntType_FTC]=NULL;
    }
    if (ptrDMADriver->hwiHandle[DMA_IntType_LFS]!=NULL)
    {
        HwiP_delete(ptrDMADriver->hwiHandle[DMA_IntType_LFS]);
        ptrDMADriver->hwiHandle[DMA_IntType_LFS]=NULL;
    }
    if (ptrDMADriver->hwiHandle[DMA_IntType_HBC]!=NULL)
    {
        HwiP_delete(ptrDMADriver->hwiHandle[DMA_IntType_HBC]);
        ptrDMADriver->hwiHandle[DMA_IntType_HBC]=NULL;
    }
    if (ptrDMADriver->hwiHandle[DMA_IntType_BTC]!=NULL)
    {
        HwiP_delete(ptrDMADriver->hwiHandle[DMA_IntType_BTC]);
        ptrDMADriver->hwiHandle[DMA_IntType_BTC]=NULL;
    }
    if (ptrDMADriver->hwiHandle[DMA_IntType_BER]!=NULL)
    {
        HwiP_delete(ptrDMADriver->hwiHandle[DMA_IntType_BER]);
        ptrDMADriver->hwiHandle[DMA_IntType_BER]=NULL;
    }
    /* free the allocated memory */
    memReqSize = (uint32_t)(sizeof(DMA_ChanMapTable) * ptrDMADriver->hwAttrs->numDmaChannels);
    if (ptrDMADriver->chanMapTable != NULL) {
        MemoryP_ctrlFree(ptrDMADriver->chanMapTable,memReqSize);
        ptrDMADriver->chanMapTable = NULL;
    }
    for (intNum=0; intNum < DMA_IntType_NUM; intNum++) 
    {
        memReqSize = (uint32_t)(sizeof(DMA_InterruptCtx) * ptrDMADriver->hwAttrs->numDmaChannels);
        if (ptrDMADriver->interruptCtx[intNum] != NULL) {
            MemoryP_ctrlFree(ptrDMADriver->interruptCtx[intNum],memReqSize);
            ptrDMADriver->interruptCtx[intNum] = NULL;
        }
    }
    MemoryP_ctrlFree(ptrDMADriver,(uint32_t)sizeof(DMA_Driver));
    gDMADriverPtr[index]=NULL; /* reset the driver's cached handle too */

}
/** @}*/


/*!
 *  @brief  Function to initialize the DMA module
 *
 *  
 *
 *  @pre    This function must be called once per system and before
 *          any other DMA driver APIs. It resets all the DMA H/W instances in the system.
 *
 */
void DMA_init(void)
{
    return;
}

/*!
 *  @brief  Function to initialize the DMA params strcuture needed for DMA_open() call
 *
 *  @param  params   DMA params for creating a instance for the first time
 *
 *  @return nothing
 *
 *
 *  
 *
 *  @sa     DMA_open()
 */
void DMA_Params_init(DMA_Params *params)
{
    *params = DMA_defaultParams;
}


/*!
 *  @brief  Function to initialize a given DMA peripheral instance
 *
 *  Function to initialize a given DMA peripheral instance specified by the
 *  particular index value. Only the first call to this function initializes the peripheral -
 *  subsequent calls just return the already initialized handle.
 *
 *  @pre    DMA_init() has been called
 *
 *  @param  index         DMA instance number
 *
 *  @param  params        DMA params for creating a instance for the first time. This 
 *                        structure can be pre-populated with default values using
 *                        DMA_Params_init() API. This will be used in the first DMA_open() call;
 *                        it will be ignored for subsequent open calls.
 *
 *  @param  errCode       [out] valid errorCode if NULL handle returned.
 *
 *  @return A DMA_Handle upon success. NULL if an error occurs.
 *
 *
 *  
 *
 *  @sa     DMA_init()
 *  @sa     DMA_close()
 */
DMA_Handle DMA_open(uint32_t  index, DMA_Params* params, int32_t* errCode)
{
    DMA_Handle          handle = NULL;
    uint32_t            intNum;
    HwiP_Params         hwiParams;
    uint32_t            memReqSize = 0U;
    uintptr_t           key;
    int32_t             retCode = 0;

    if (index >= DMA_NUM_INSTANCES)
    {
        retCode = DMA_EOUTOFRANGE;
    }
    else /* start of if valid index */
    {
        
        /* Disable preemption while opening the driver */
        key = HwiP_disable();
        
        /*
         * check if driver is already init 
         */
        if (gDMADriverPtr[index] == NULL) 
        {
            /* Allocate memory for the driver and initialize it */
            gDMADriverPtr[index] = MemoryP_ctrlAlloc ((uint32_t)sizeof(DMA_Driver), 0U);
            if (gDMADriverPtr[index] == NULL)
            {
                /* Error: Out of memory */
                DebugP_log2 ("Debug: DMA Driver (%d) Out of memory (requested size: %d)\n",index,(uint32_t)sizeof(DMA_Driver));
                retCode = DMA_EOUTOFMEM;
            }
            else
            {
                memset ((void *)gDMADriverPtr[index], 0U, sizeof(DMA_Driver));
                gDMADriverPtr[index]->hwAttrs = &gDMAHwCfg[index];
                gDMADriverPtr[index]->instanceNum = index;
                /* initialize internal driver struture memory */
                /* malloc chanMapTable */
                memReqSize = (uint32_t)(sizeof(DMA_ChanMapTable) * gDMADriverPtr[index]->hwAttrs->numDmaChannels);
                gDMADriverPtr[index]->chanMapTable = (DMA_ChanMapTable *)MemoryP_ctrlAlloc (memReqSize, 0U);
                if (gDMADriverPtr[index]->chanMapTable == NULL)
                {
                    /* Error: Out of memory */
                    DebugP_log2 ("Debug: DMA Driver (%d) Out of memory for chanMapTable(requested size: %d)\n",
                                    index,memReqSize);
                    retCode = DMA_EOUTOFMEM;
                }
                else 
                {
                    memset(gDMADriverPtr[index]->chanMapTable,0U,memReqSize);
                    /* dont set the handle yet as we dont know if all the mem allocs are goind to succeed */
                }            
                /* malloc interruptCtrx */
                for (intNum=0U; ((retCode == 0U) && (intNum < DMA_IntType_NUM)); intNum++) 
                {
                    memReqSize = (uint32_t)(sizeof(DMA_InterruptCtx) * gDMADriverPtr[index]->hwAttrs->numDmaChannels);
                    gDMADriverPtr[index]->interruptCtx[intNum] = (DMA_InterruptCtx *)MemoryP_ctrlAlloc (memReqSize, 0U);
                    if (gDMADriverPtr[index]->interruptCtx[intNum] == NULL)
                    {
                        /* Error: Out of memory */
                        DebugP_log3 ("Debug: DMA Driver (%d) Out of memory for interruptCtx[%d](requested size: %d)\n",
                                      index,intNum,memReqSize);
                        retCode = DMA_EOUTOFMEM;
                    }
                    else 
                    {
                        memset(gDMADriverPtr[index]->interruptCtx[intNum],0U,memReqSize);
                        /* dont set the handle yet as we dont know if all the mem allocs are going to succeed */
                    }
                }
            }
        }

        /* 
         * now do config if we got the driver memory allocated and primed 
         */
        if ((retCode==0U) && (gDMADriverPtr[index] != NULL) && (gDMADriverPtr[index]->refCnt==0U))
        {
            uint8_t    channel = 0;
            volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)gDMADriverPtr[index]->hwAttrs->ctrlBaseAddr;

            /* reset the DMA */
            ctrlBaseAddr->GCTRL = CSL_FMKR (31U, 0U, 0x1U);

            /* Register interrupt handlers */
           /* FTC interrupt */
            HwiP_Params_init(&hwiParams);
            hwiParams.name = "DMA-FTC";
            hwiParams.arg  = (uintptr_t)gDMADriverPtr[index];
            gDMADriverPtr[index]->hwiHandle[DMA_IntType_FTC] = HwiP_create(gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_FTC], DMA_ftcISR, &hwiParams);
            DebugP_log2 ("Debug: DMA Driver Registering HWI ISR [%p] for FTC Interrupt %d\n",
                                  gDMADriverPtr[index]->hwiHandle[DMA_IntType_FTC], gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_FTC]);
            /* LFS interrupt */
            HwiP_Params_init(&hwiParams);
            hwiParams.name = "DMA-LFS";
            hwiParams.arg  = (uintptr_t)gDMADriverPtr[index];
            gDMADriverPtr[index]->hwiHandle[DMA_IntType_LFS] = HwiP_create(gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_LFS], DMA_lfsISR, &hwiParams);
            DebugP_log2 ("Debug: DMA Driver Registering HWI ISR [%p] for LFS Interrupt %d\n",
                                  gDMADriverPtr[index]->hwiHandle[DMA_IntType_LFS], gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_LFS]);
            /* HBC interrupt */
            HwiP_Params_init(&hwiParams);
            hwiParams.name = "DMA-HBC";
            hwiParams.arg  = (uintptr_t)gDMADriverPtr[index];
            gDMADriverPtr[index]->hwiHandle[DMA_IntType_HBC] = HwiP_create(gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_HBC], DMA_hbcISR, &hwiParams);
            DebugP_log2 ("Debug: DMA Driver Registering HWI ISR [%p] for HBC Interrupt %d\n",
                                  gDMADriverPtr[index]->hwiHandle[DMA_IntType_HBC], gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_HBC]);
            /* BTC interrupt */
            HwiP_Params_init(&hwiParams);
            hwiParams.name = "DMA-BTC";
            hwiParams.arg  = (uintptr_t)gDMADriverPtr[index];
            gDMADriverPtr[index]->hwiHandle[DMA_IntType_BTC] = HwiP_create(gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_BTC], DMA_btcISR, &hwiParams);
            DebugP_log2 ("Debug: DMA Driver Registering HWI ISR [%p] for BTC Interrupt %d\n",
                                  gDMADriverPtr[index]->hwiHandle[DMA_IntType_BTC], gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_BTC]);
            /* BER interrupt */
            HwiP_Params_init(&hwiParams);
            hwiParams.name = "DMA-BER";
            hwiParams.arg  = (uintptr_t)gDMADriverPtr[index];
            gDMADriverPtr[index]->hwiHandle[DMA_IntType_BER] = HwiP_create(gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_BER], DMA_berISR, &hwiParams);
            DebugP_log2 ("Debug: DMA Driver Registering HWI ISR [%p] for BER Interrupt %d\n",
                                  gDMADriverPtr[index]->hwiHandle[DMA_IntType_BER], gDMADriverPtr[index]->hwAttrs->intNum[DMA_IntType_BER]);

            /* reset other registers? */
            
            /* Set the enable bit, 0 for the reset bit*/
            ctrlBaseAddr->GCTRL = 0x00010000U;

            /* set the suspend bit */
            switch (params->dmaDebugMode)
            {
                case DMA_DebugMode_IgnoreSuspend:
                case DMA_DebugMode_FinishBlock:
                case DMA_DebugMode_FinishFrame:
                case DMA_DebugMode_Immediate:
                    ctrlBaseAddr->GCTRL = CSL_FINSR(ctrlBaseAddr->GCTRL,9U,8U,params->dmaDebugMode);
                break;
                default:
                    /* Error: Invalid parameter */
                    DebugP_log2 ("Debug: DMA Driver (%d) Invalid parameter: params->dmaDebugMode: %d\n",
                                  index,params->dmaDebugMode);
                    retCode = DMA_EINVAL;
                break;
            }
            /* set the fixed priority scheme */
            switch (params->lowPriScheme)
            {
                case DMA_PriScheme_Fixed:
                case DMA_PriScheme_Rotation:
                    ctrlBaseAddr->PTCRL= CSL_FINSR(ctrlBaseAddr->PTCRL,16U,16U,params->lowPriScheme);
                break;
                default:
                    /* Error: Invalid parameter */
                    DebugP_log2 ("Debug: DMA Driver (%d) Invalid parameter: params->lowPriScheme: %d\n",
                                  index,params->lowPriScheme);
                    retCode = DMA_EINVAL;
                break;
            }
            switch (params->highPriScheme)
            {
                case DMA_PriScheme_Fixed:
                case DMA_PriScheme_Rotation:
                    ctrlBaseAddr->PTCRL= CSL_FINSR(ctrlBaseAddr->PTCRL,17U,17U,params->highPriScheme);
                break;
                default:
                    /* Error: Invalid parameter */
                    DebugP_log2 ("Debug: DMA Driver (%d) Invalid parameter: params->highPriScheme: %d\n",
                                  index,params->highPriScheme);
                    retCode = DMA_EINVAL;
                break;
            }
            /* set the bypass FIFO bit */
            if (params->bypassFifo==true)
            {
                ctrlBaseAddr->PTCRL= CSL_FINSR(ctrlBaseAddr->PTCRL,18U,18U,1U);
            }
            else 
            {
                ctrlBaseAddr->PTCRL = CSL_FINSR(ctrlBaseAddr->PTCRL,18U,18U,0U);
            }
            /* do default port assignments for all channels */
            for (channel=0;channel<gDMADriverPtr[index]->hwAttrs->numDmaChannels;channel++)
            {
                uint8_t i,j;
                /* hard code the Port assignment register to use Port B for this channel */
                i = channel >> 3U;                /* Find the register to write                    */
                j = channel - (uint8_t)(i << 3U);          /* Find the offset of the 4th bit                */
                j = (uint8_t)((uint8_t)7U - j); /* Reverse the order of the 4th bit offset       */
                j = (uint8_t)(j << 2U);                      /* Find the bit location of the 4th bit to write */
                ctrlBaseAddr->PAR[i] = CSL_FINSR(ctrlBaseAddr->PAR[i],
                                            (uint8_t)((j+3)-1),j,gDMADriverPtr[index]->hwAttrs->defaultPortAssignment); 
            }

        }

        /*
         * check before returning 
         */
        if (retCode == 0U)
        {
            /* increment reference count */
            gDMADriverPtr[index]->refCnt++;
            /* Setup the return handle: */
            handle = (void *)gDMADriverPtr[index];
        }
        else
        {
            handle = NULL;
            /* errCode is already set */
            DMA_deleteInstance(gDMADriverPtr[index]);
            gDMADriverPtr[index]=NULL;
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
 *  @brief  Function to close a DMA peripheral specified by the DMA handle
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle      A DMA_Handle returned from DMA_open()
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
int32_t DMA_close(DMA_Handle handle)
{
    DMA_Driver          *ptrDMADriver = NULL;
    int32_t             retCode = 0;
    uintptr_t           key;
    
    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while opening the driver */
    key = HwiP_disable();

    if (ptrDMADriver!=NULL)
    {
        if (ptrDMADriver->inUseChannelMap == 0U) 
        {
            /* decrement the refCount and if this is the last reference, delete the instance */
            if (ptrDMADriver->refCnt>0U)
            {
                ptrDMADriver->refCnt--;
            }
            if (ptrDMADriver->refCnt==0U)
            {
                volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
                
                /* disable the interrupts and clear all pending flags */
                ctrlBaseAddr->GCHIENAR   = 0xFFFFU;
                ctrlBaseAddr->FTCINTENAR = 0xFFFFU;
                ctrlBaseAddr->LFSINTENAR = 0xFFFFU;
                ctrlBaseAddr->HBCINTENAR = 0xFFFFU;
                ctrlBaseAddr->BTCINTENAR = 0xFFFFU;

                ctrlBaseAddr->FTCFLAG = 0xFFFFU;
                ctrlBaseAddr->LFSFLAG = 0xFFFFU;
                ctrlBaseAddr->HBCFLAG = 0xFFFFU;
                ctrlBaseAddr->BTCFLAG = 0xFFFFU;
                ctrlBaseAddr->BERFLAG = 0xFFFFU;
                
                DMA_deleteInstance(ptrDMADriver);
            }
        }
        else 
        {
            /* return invalid code */
            retCode = DMA_EINUSE;
        }
    }
    else 
    {
        /* return invalid code */
        retCode = DMA_EINVAL;
    }
    /* Restore the interrupts */
    HwiP_restore(key);

    return retCode;
}

/*!
 *  @brief  Function to assign DMA request lines to channels. This should be used only
 *          if the channel is meant for H/W triggering. For S/W triggered channel, ignore this API
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel. This can be a specific channel
 *
 *  @param  reqline         DMA request line
 *
 *  @param  errCode       [out] valid errorCode if NULL handle returned.
 *
 *  @return The channel number upon success. 'DMA_CHANNEL_NONE' if an error occurs.
 *
 *  @sa     DMA_open()
 */
uint32_t DMA_assignChannel(DMA_Handle handle, uint32_t channel, uint32_t reqline, int32_t* errCode)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    uint32_t            retChannel = DMA_CHANNEL_NONE;
    int32_t             retCode = 0;
    
    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    
    /* Disable preemption while setting the registers */
    key = HwiP_disable();
#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL) && (ptrDMADriver->refCnt > 0U))
    {
        if ((reqline>=ptrDMADriver->hwAttrs->numReqLines) || (channel >= ptrDMADriver->hwAttrs->numDmaChannels))
        {
            retCode = DMA_EINVAL;
            retChannel = DMA_CHANNEL_NONE;
        }
        else
#endif
        {
            retChannel = channel;
            
            /* assume retChannel is valid at this point, now check the mapping table*/
            if (ptrDMADriver->chanMapTable[retChannel].inUse==1U)
            {
                retCode = DMA_EINUSE;
                retChannel = DMA_CHANNEL_NONE;
            }
            else
            {
                uint8_t i,j;
                volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
                
                i = (uint8_t)channel >> 2U;                  /* Find the register to configure */
                j = (uint8_t)channel - (uint8_t)(i << 2U);            /* Find the offset of the type    */
                j = (uint8_t) ((uint8_t)3U - j);  /* reverse the byte order         */
                j = (uint8_t)(j << 3U);                        /* find the bit location          */

                /* set the h/w */
                ctrlBaseAddr->DREQASI[i] = CSL_FINSR (ctrlBaseAddr->DREQASI[i], 
                                                        (uint8_t)(j+8-1), j, reqline);
                /* store the state */
                ptrDMADriver->chanMapTable[retChannel].reqLine = reqline+1U; /* TBD */
                ptrDMADriver->chanMapTable[retChannel].inUse=1U;
            }
        }
#ifdef DMA_PARAM_CHECK
    }
    else 
    {
        retCode = DMA_ENOINIT;
        retChannel = DMA_CHANNEL_NONE;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    if (errCode!=NULL)
    {
        *errCode = retCode;
    }
    return retChannel;
}

/*!
 *  @brief  Function to remove the DMA request lines to channel mapping 
 *          and free the channel for another assignment
 *
 *  @pre    DMA_assignChannel() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel. 
 *
 *  @param  reqline         DMA request line
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
int32_t DMA_freeChannel(DMA_Handle handle, uint32_t channel, uint32_t reqline)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    
    /* Disable preemption while setting the registers */
    key = HwiP_disable();
#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if (channel >= ptrDMADriver->hwAttrs->numDmaChannels)
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else if ((ptrDMADriver->chanMapTable[channel].inUse==1U) && 
                 (ptrDMADriver->chanMapTable[channel].reqLine == (reqline+1U)))
        {
#endif
            ptrDMADriver->chanMapTable[channel].reqLine = 0U; 
            ptrDMADriver->chanMapTable[channel].inUse = 0U;
#ifdef DMA_PARAM_CHECK
        }
        else
        {
            retCode = DMA_EINVAL;
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to enable a given interrupt type for a given channel.
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @param  callbackFn      User supplied callback function that the ISR will call
 *                          on receiving this interrupt. Care should be taken by the user to 
 *                          do minimal processing in this callback function. Typical implementation
 *                          of this function would do some kind of semaphore or event post.
 *
 *  @param  callbackArg     User supplied callback functions arg that will be provided back to user 
 *                          when the callback function is called from the ISR.
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
 int32_t DMA_enableInterrupt(DMA_Handle handle, 
                                          uint32_t channel, 
                                          DMA_IntType intType, 
                                          DMA_IntHandlerFuncPTR callbackFn, 
                                          void * callbackArg)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if ((channel >= ptrDMADriver->hwAttrs->numDmaChannels) || (intType >= DMA_IntType_NUM))
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else 
        {
#endif
            volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
            
            /* save the interrupt context */
            ptrDMADriver->interruptCtx[intType][channel].bIsEnabled = 1U;
            ptrDMADriver->interruptCtx[intType][channel].callbackFn = callbackFn;
            ptrDMADriver->interruptCtx[intType][channel].callbackArg = callbackArg;
            /* enable the global interrupt */            
            ctrlBaseAddr->GCHIENAS = CSL_FINSR (ctrlBaseAddr->GCHIENAS, 
                                                    u8_channel, u8_channel, 0x1U);
            /* now enable the channel interrupt for the intType */
            switch (intType)
            {
                case DMA_IntType_FTC:
                {
                    ctrlBaseAddr->FTCINTENAS = CSL_FINSR (ctrlBaseAddr->FTCINTENAS, 
                                                            u8_channel, u8_channel, 0x1U);
                }
                break;
                
                case DMA_IntType_LFS:
                {
                    ctrlBaseAddr->LFSINTENAS = CSL_FINSR (ctrlBaseAddr->LFSINTENAS, 
                                                            u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_HBC:
                {
                    ctrlBaseAddr->HBCINTENAS = CSL_FINSR (ctrlBaseAddr->HBCINTENAS, 
                                                            u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_BTC:
                {
                    ctrlBaseAddr->BTCINTENAS = CSL_FINSR (ctrlBaseAddr->BTCINTENAS, 
                                                            u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_BER:
                break;
                default :
                /* Didnt match any type - control will never reach here since 
                   parameter validation is done at the start of this function*/
                break;
            }
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}



/*!
 *  @brief  Function to disable a given interrupt type for a given channel
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
int32_t DMA_disableInterrupt(DMA_Handle handle, uint32_t channel, DMA_IntType intType)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint32_t            localIntType;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();
#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if ((channel >= ptrDMADriver->hwAttrs->numDmaChannels) || (intType >= DMA_IntType_NUM))
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else 
        {
#endif
            volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
            /* disable the global interrupt first */
            ctrlBaseAddr->GCHIENAR= CSL_FINSR (ctrlBaseAddr->GCHIENAR, 
                                                    channel, channel, 0x1U);

            
            /* disable the channel interrupt for the intType */
            switch (intType)
            {
                case DMA_IntType_FTC:
                {
                    ctrlBaseAddr->FTCINTENAR = CSL_FINSR (ctrlBaseAddr->FTCINTENAR, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                
                case DMA_IntType_LFS:
                {
                    ctrlBaseAddr->LFSINTENAR = CSL_FINSR (ctrlBaseAddr->LFSINTENAR, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_HBC:
                {
                    ctrlBaseAddr->HBCINTENAR = CSL_FINSR (ctrlBaseAddr->HBCINTENAR, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_BTC:
                {
                    ctrlBaseAddr->BTCINTENAR = CSL_FINSR (ctrlBaseAddr->BTCINTENAR, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_BER:
                break;
                default :
                /* Didnt match any type - control will never reach here since 
                   parameter validation is done at the start of this function*/
                break;
            }

            /* reset the interrupt context */
            ptrDMADriver->interruptCtx[intType][channel].bIsEnabled = 0;
            ptrDMADriver->interruptCtx[intType][channel].callbackFn = NULL;
            ptrDMADriver->interruptCtx[intType][channel].callbackArg = NULL;
            /* check if global interrupts need to be enabled back 
               if other interrupt types are requested for this channel.
             */
            for (localIntType=DMA_IntType_FTC;localIntType<DMA_IntType_NUM;localIntType++)
            {
                if (ptrDMADriver->interruptCtx[localIntType][channel].bIsEnabled)
                {
                    /* re-enable the global interrupt */
                    ctrlBaseAddr->GCHIENAS = CSL_FINSR (ctrlBaseAddr->GCHIENAS, 
                                                u8_channel, u8_channel, 0x1U);
                    break;
                }
            }

#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}



/*!
 *  @brief  Function to set the control packet RAM for a given channel
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  ctrlPacket      DMA Control Packet Parameters
 *
 *  @return 0 upon success. error code if an error occurs.
 *
 *  @sa     DMA_open()
 */
int32_t DMA_setChannelParams(DMA_Handle handle, uint32_t channel, DMA_CtrlPktParams *ctrlPacket)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    
#ifdef DMA_PARAM_CHECK
    /* Disable preemption while setting the registers */
    key = HwiP_disable();
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if ((channel >= ptrDMADriver->hwAttrs->numDmaChannels) ||
            (ctrlPacket == NULL))
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
            /* Restore the interrupts */
            HwiP_restore(key);
        }
        else if ((ptrDMADriver->inUseChannelMap & (uint32_t)(1U <<channel))==1)
        {
            /* channel in use*/
            retCode = DMA_EINUSE;
            /* Restore the interrupts */
            HwiP_restore(key);
        }
        else 
        {
            ptrDMADriver->inUseChannelMap = ptrDMADriver->inUseChannelMap | (uint32_t)(1U <<channel);
            /* Restore the interrupts */
            HwiP_restore(key);
            
            /* validate channel params */
            if ((ctrlPacket->frameXferCnt > 0x1FFFU) || (ctrlPacket->elemXferCnt > 0x1FFFU))
            {
                /* invalid frame and/or elem transfer count*/
                retCode = DMA_EINVAL;
            }
            else if ((ctrlPacket->nextChannel!= DMA_CHANNEL_NONE) &&
                (ctrlPacket->nextChannel >= ptrDMADriver->hwAttrs->numDmaChannels))
            {
                /* invalid next channel */
                retCode = DMA_EINVAL;
            } 
            else if ((ctrlPacket->srcElemSize>=DMA_ElemSize_Last) || (ctrlPacket->destElemSize>=DMA_ElemSize_Last))
            {
                /* invalid element size */
                retCode = DMA_EINVAL;
            }
            else if ((ctrlPacket->xferType>=DMA_XferType_Last) ||
                     (ctrlPacket->srcAddrMode>=DMA_AddrMode_Last) ||
                     (ctrlPacket->destAddrMode>=DMA_AddrMode_Last))
            {
                /* invalid xfer type, src/dest addr mode */
                retCode = DMA_EINVAL;
            }
            else if ((ctrlPacket->srcAddrMode==DMA_AddrMode_Indexed) &&
                     ((ctrlPacket->srcElemIndexOffset > 0x1FFFU) || (ctrlPacket->srcFrameIndexOffset > 0x1FFFU)))
            {
                /* invalid elem or frame Index offset */
                retCode = DMA_EINVAL;
            }
            else if ((ctrlPacket->destAddrMode==DMA_AddrMode_Indexed) &&
                     ((ctrlPacket->destElemIndexOffset > 0x1FFFU) || (ctrlPacket->destFrameIndexOffset > 0x1FFFU)))
            {
                /* invalid elem or frame Index offset */
                retCode = DMA_EINVAL;
            }
            else if (((ctrlPacket->srcAddr % (uint32_t)(1U<<ctrlPacket->srcElemSize))!=0U) ||
                     ((ctrlPacket->destAddr % (uint32_t)(1U<<ctrlPacket->destElemSize))!=0U))
            {
                /* invalid elem or frame Index offset */
                retCode = DMA_ENOTALIGNED;
            }
            else 
            {
#endif
                /* set the registers */
                volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
                volatile DMARAMRegs *pktBaseAddr = (volatile DMARAMRegs *)ptrDMADriver->hwAttrs->pktBaseAddr;
            
                /* set the source address */
                pktBaseAddr->PRIMARYCONTROLPACKET[channel].ISADDR = ctrlPacket->srcAddr;
                /* set the dest address */
                pktBaseAddr->PRIMARYCONTROLPACKET[channel].IDADDR  = ctrlPacket->destAddr;
                /* set the frame and element transfer count */
                pktBaseAddr->PRIMARYCONTROLPACKET[channel].ITCOUNT = (((uint32_t)ctrlPacket->frameXferCnt << 16U) | (uint32_t)ctrlPacket->elemXferCnt);
                /* set the channel control register - this will disable chaining */
                pktBaseAddr->PRIMARYCONTROLPACKET[channel].CHCTRL  = (((uint32_t)(ctrlPacket->srcElemSize) << 14U) | 
                                                                ((uint32_t)(ctrlPacket->destElemSize) << 12U) | 
                                                                ((uint32_t)ctrlPacket->xferType << 8U)| 
                                                                ((uint32_t)ctrlPacket->srcAddrMode << 3U ) | 
                                                                ((uint32_t)ctrlPacket->destAddrMode<< 1U ) | 
                                                                ((uint32_t)ctrlPacket->autoInitiation));
                /* enable chaining if valid channel is provided */
                if (ctrlPacket->nextChannel!= DMA_CHANNEL_NONE)
                {
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].CHCTRL = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].CHCTRL,
                                                                    21U,16U,(uint32_t)ctrlPacket->nextChannel+1U);
                }
                else
                {
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].CHCTRL = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].CHCTRL,
                                                                    21U,16U,0U);
                }
                /* source - set the element and frame offset index register if indexed addressing mode is selected */
                if (ctrlPacket->srcAddrMode==DMA_AddrMode_Indexed)
                {
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF,
                                                                    12U,0U,(uint32_t)ctrlPacket->srcElemIndexOffset);
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF,
                                                                    12U,0U,(uint32_t)ctrlPacket->srcFrameIndexOffset);
                }
                else 
                {
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF,
                                                                    12U,0U,0U);
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF,
                                                                    12U,0U,0U);
                }
                /* destination - set the element and frame offset index register if indexed addressing mode is selected */
                if (ctrlPacket->destAddrMode==DMA_AddrMode_Indexed)
                {
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF,
                                                                    28U,16U,(uint32_t)ctrlPacket->destElemIndexOffset);
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF,
                                                                    28U,16U,(uint32_t)ctrlPacket->destFrameIndexOffset);
                }
                else
                {
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].EIOFF,
                                                                    28U,16U,0U);
                    pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF = CSL_FINSR(pktBaseAddr->PRIMARYCONTROLPACKET[channel].FIOFF,
                                                                    28U,16U,0U);
                }
#ifdef DMA_PARAM_CHECK
            }
            key = HwiP_disable();
            ptrDMADriver->inUseChannelMap = (ptrDMADriver->inUseChannelMap & (uint32_t)(~(1U <<channel)));
            HwiP_restore(key);
        }
        
    }
    else 
    {
        retCode = DMA_ENOINIT;
        /* Restore the interrupts */
        HwiP_restore(key);
    }
#endif
    
    /* return */
    return retCode;
}

/*!
 *  @brief  Function to enable the channel given the trigger type
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  triggerType     DMA channel trigger type
 *
 *  @sa     DMA_open()
 */
int32_t DMA_enableChannel(DMA_Handle handle, uint32_t channel, DMA_ChTriggerType triggerType)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();
#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if (channel >= ptrDMADriver->hwAttrs->numDmaChannels)
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else 
        {
#endif
            volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
            
            /* enable the channel based on the trigger type */
            switch (triggerType)
            {
                case DMA_ChTriggerType_HW:
                {
                    ctrlBaseAddr->HWCHENAS = CSL_FINSR (ctrlBaseAddr->HWCHENAS, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                
                case DMA_ChTriggerType_SW:
                {
                    ctrlBaseAddr->SWCHENAS = CSL_FINSR (ctrlBaseAddr->SWCHENAS, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                default :
                /* Didnt match any type */
                {
                    retCode = DMA_EINVAL;
                }
                break;
            }
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to disable the channel given the trigger type
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  triggerType     DMA channel trigger type
 *
 *  @sa     DMA_open()
 */
int32_t DMA_disableChannel(DMA_Handle handle, uint32_t channel, DMA_ChTriggerType triggerType)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if (channel >= ptrDMADriver->hwAttrs->numDmaChannels)
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else 
        {
#endif
            volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
            
            /* disable the channel based on the trigger type */
            switch (triggerType)
            {
                case DMA_ChTriggerType_HW:
                {
                    ctrlBaseAddr->HWCHENAR = CSL_FINSR (ctrlBaseAddr->HWCHENAR, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                
                case DMA_ChTriggerType_SW:
                {
                    ctrlBaseAddr->SWCHENAR = CSL_FINSR (ctrlBaseAddr->SWCHENAR, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                default :
                /* Didnt match any type */
                {
                    retCode = DMA_EINVAL;
                }
                break;
            }
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to set the channel priority
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  priority        DMA channel priority
 *
 *  @sa     DMA_open()
 */
int32_t DMA_setChannelPriority(DMA_Handle handle, uint32_t channel, DMA_ChPriority priority)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();
    
#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if (channel >= ptrDMADriver->hwAttrs->numDmaChannels)
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else 
        {
#endif
            volatile DMARegs *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;

            /* set the channel based on the priority */
            switch (priority)
            {
                case DMA_ChPriority_Low:
                {
                    ctrlBaseAddr->CHPRIOR = CSL_FINSR (ctrlBaseAddr->CHPRIOR, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                
                case DMA_ChPriority_High:
                {
                    ctrlBaseAddr->CHPRIOS = CSL_FINSR (ctrlBaseAddr->CHPRIOS, 
                                                                u8_channel, u8_channel, 0x1U);
                }
                break;
                default :
                /* Didnt match any type */
                {
                    retCode = DMA_EINVAL;
                }
                break;
            }
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif

    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}


/*!
 *  @brief  Function to get the channel status
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  pChanStatus     [out] Output block filled by the driver with the channel status
 *
 *  @sa     DMA_open()
 */
int32_t DMA_getChannelStatus(DMA_Handle handle, uint32_t channel, DMA_ChannelStatus *pChanStatus)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if ((channel >= ptrDMADriver->hwAttrs->numDmaChannels) ||
            (pChanStatus == NULL))
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else 
        {
#endif
            volatile DMARegs      *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;
            volatile DMARAMRegs   *pktBaseAddr = (volatile DMARAMRegs *)ptrDMADriver->hwAttrs->pktBaseAddr;
            uint8_t i,j;
            
            pChanStatus->chPend = CSL_FEXTR(ctrlBaseAddr->PEND,u8_channel,u8_channel);
            pChanStatus->chActive = CSL_FEXTR(ctrlBaseAddr->DMASTAT,u8_channel,u8_channel);
            pChanStatus->chHwEnable = CSL_FEXTR(ctrlBaseAddr->HWCHENAS,u8_channel,u8_channel);
            pChanStatus->chSwEnable = CSL_FEXTR(ctrlBaseAddr->SWCHENAS,u8_channel,u8_channel);
            pChanStatus->chIsHighPri = CSL_FEXTR(ctrlBaseAddr->CHPRIOS,u8_channel,u8_channel);
            
            i = u8_channel >> 2U;                  /* Find the register to configure */
            j = u8_channel - (uint8_t)(i << 2U);            /* Find the offset of the type    */
            j = (uint8_t) ((uint8_t)3U - j);  /* reverse the byte order         */
            j = (uint8_t)(j << 3U);                        /* find the bit location          */
            pChanStatus->chReqAssign = CSL_FEXTR(ctrlBaseAddr->DREQASI[i],(uint8_t)((j+8)-1),j);

            i = u8_channel >> 3U;                /* Find the register to write                    */
            j = u8_channel - (uint8_t)(i << 3U);          /* Find the offset of the 4th bit                */
            j = (uint8_t)((uint8_t)7U - j); /* Reverse the order of the 4th bit offset       */
            j = (uint8_t)(j << 2U);                      /* Find the bit location of the 4th bit to write */
            pChanStatus->chPortAssign = CSL_FEXTR(ctrlBaseAddr->PAR[i],(uint8_t)((j+3)-1),j);

            pChanStatus->chIsIntEn = CSL_FEXTR(ctrlBaseAddr->GCHIENAS,u8_channel,u8_channel);
            pChanStatus->chIsFTCIntEn = CSL_FEXTR(ctrlBaseAddr->FTCINTENAS,u8_channel,u8_channel);
            pChanStatus->chIsLFSIntEn = CSL_FEXTR(ctrlBaseAddr->LFSINTENAS,u8_channel,u8_channel);
            pChanStatus->chIsHBCIntEn = CSL_FEXTR(ctrlBaseAddr->HBCINTENAS,u8_channel,u8_channel);
            pChanStatus->chIsBTCIntEn = CSL_FEXTR(ctrlBaseAddr->BTCINTENAS,u8_channel,u8_channel);

            pChanStatus->chIsIntPend = CSL_FEXTR(ctrlBaseAddr->GINTFLAG,u8_channel,u8_channel);
            pChanStatus->chIsFTCIntPend = CSL_FEXTR(ctrlBaseAddr->FTCFLAG,u8_channel,u8_channel);
            pChanStatus->chIsLFSIntPend = CSL_FEXTR(ctrlBaseAddr->LFSFLAG,u8_channel,u8_channel);
            pChanStatus->chIsHBCIntPend = CSL_FEXTR(ctrlBaseAddr->HBCFLAG,u8_channel,u8_channel);
            pChanStatus->chIsBTCIntPend = CSL_FEXTR(ctrlBaseAddr->BTCFLAG,u8_channel,u8_channel);
            pChanStatus->chIsBERIntPend = CSL_FEXTR(ctrlBaseAddr->BERFLAG,u8_channel,u8_channel);

            pChanStatus->chInitSrcAddr = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].ISADDR,31U,0U);
            pChanStatus->chInitDstAddr = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].IDADDR,31U,0U);
            pChanStatus->chInitElemCnt = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].ITCOUNT,12U,0U);
            pChanStatus->chInitFrameCnt = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].ITCOUNT,28U,16U);
            pChanStatus->chControlReg = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].CHCTRL,31U,0U);
            pChanStatus->chSrcElemIndex = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].EIOFF,12U,0U);
            pChanStatus->chDstElemIndex = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].EIOFF,28U,16U);
            pChanStatus->chSrcFrameIndex = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].FIOFF,12U,0U);
            pChanStatus->chDstFrameIndex = CSL_FEXTR(pktBaseAddr->PRIMARYCONTROLPACKET[u8_channel].FIOFF,28U,16U);

            pChanStatus->chCurrSrcAddr = CSL_FEXTR(pktBaseAddr->WORKINGCONTROLPACKET[u8_channel].CSADDR,31U,0U);
            pChanStatus->chCurrDstAddr = CSL_FEXTR(pktBaseAddr->WORKINGCONTROLPACKET[u8_channel].CDADDR,31U,0U);
            pChanStatus->chCurrElemCnt = CSL_FEXTR(pktBaseAddr->WORKINGCONTROLPACKET[u8_channel].CTCOUNT,12U,0U);
            pChanStatus->chCurrFrameCnt = CSL_FEXTR(pktBaseAddr->WORKINGCONTROLPACKET[u8_channel].CTCOUNT,28U,16U);
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}



/*!
 *  @brief  Function to get the DMA peripheral status
 *
 *  @pre    DMA_open() has been called.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  pDmaStatus      [out] Output block filled by the driver with the peripheral status
 *
 *  @sa     DMA_open()
 */
int32_t DMA_getStatus(DMA_Handle handle, uint32_t channel, DMA_Status *pDmaStatus)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if ((channel >= ptrDMADriver->hwAttrs->numDmaChannels) ||
            (pDmaStatus == NULL))
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else 
        {
#endif
            volatile DMARegs      *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;

            pDmaStatus->dmaEnable = CSL_FEXTR(ctrlBaseAddr->GCTRL,16U,16U);
            pDmaStatus->dmaBusBusy = CSL_FEXTR(ctrlBaseAddr->GCTRL,14U,14U);
            pDmaStatus->dmaDbgMode = CSL_FEXTR(ctrlBaseAddr->GCTRL,9U,8U);
            pDmaStatus->dmaReset = CSL_FEXTR(ctrlBaseAddr->GCTRL,0U,0U);

            pDmaStatus->dmaXferPend = CSL_FEXTR(ctrlBaseAddr->PTCRL,24U,24U);
            pDmaStatus->dmaFIFOBypass = CSL_FEXTR(ctrlBaseAddr->PTCRL,18U,18U);
            pDmaStatus->dmaHighPriSch = CSL_FEXTR(ctrlBaseAddr->PTCRL,17U,17U);
            pDmaStatus->dmaLowPriSch = CSL_FEXTR(ctrlBaseAddr->PTCRL,16U,16U);

            pDmaStatus->dmaChPend = CSL_FEXTR(ctrlBaseAddr->PEND,31U,0U);
            pDmaStatus->dmaChActive = CSL_FEXTR(ctrlBaseAddr->DMASTAT,31U,0U);

            pDmaStatus->dmaChHwEnable = CSL_FEXTR(ctrlBaseAddr->HWCHENAS,31U,0U);
            pDmaStatus->dmaChSwEnable = CSL_FEXTR(ctrlBaseAddr->SWCHENAS,31U,0U);
            pDmaStatus->dmaChHighPri = CSL_FEXTR(ctrlBaseAddr->CHPRIOS,31U,0U);

            pDmaStatus->dmaChReqAssign[0] = CSL_FEXTR(ctrlBaseAddr->DREQASI[0],31U,0U);
            pDmaStatus->dmaChReqAssign[1] = CSL_FEXTR(ctrlBaseAddr->DREQASI[1],31U,0U);
            pDmaStatus->dmaChReqAssign[2] = CSL_FEXTR(ctrlBaseAddr->DREQASI[2],31U,0U);
            pDmaStatus->dmaChReqAssign[3] = CSL_FEXTR(ctrlBaseAddr->DREQASI[3],31U,0U);
            pDmaStatus->dmaChReqAssign[4] = CSL_FEXTR(ctrlBaseAddr->DREQASI[4],31U,0U);
            pDmaStatus->dmaChReqAssign[5] = CSL_FEXTR(ctrlBaseAddr->DREQASI[5],31U,0U);
            pDmaStatus->dmaChReqAssign[6] = CSL_FEXTR(ctrlBaseAddr->DREQASI[6],31U,0U);
            pDmaStatus->dmaChReqAssign[7] = CSL_FEXTR(ctrlBaseAddr->DREQASI[7],31U,0U);

            pDmaStatus->dmaChPortAssign[0] = CSL_FEXTR(ctrlBaseAddr->PAR[0],31U,0U);
            pDmaStatus->dmaChPortAssign[1] = CSL_FEXTR(ctrlBaseAddr->PAR[1],31U,0U);
            pDmaStatus->dmaChPortAssign[2] = CSL_FEXTR(ctrlBaseAddr->PAR[2],31U,0U);
            pDmaStatus->dmaChPortAssign[3] = CSL_FEXTR(ctrlBaseAddr->PAR[3],31U,0U);

            pDmaStatus->dmaChIntEn = CSL_FEXTR(ctrlBaseAddr->GCHIENAS,31U,0U);
            pDmaStatus->dmaChFTCIntEn = CSL_FEXTR(ctrlBaseAddr->FTCINTENAS,31U,0U);
            pDmaStatus->dmaChLFSIntEn = CSL_FEXTR(ctrlBaseAddr->LFSINTENAS,31U,0U);
            pDmaStatus->dmaChHBCIntEn = CSL_FEXTR(ctrlBaseAddr->HBCINTENAS,31U,0U);
            pDmaStatus->dmaChBTCIntEn = CSL_FEXTR(ctrlBaseAddr->BTCINTENAS,31U,0U);

            pDmaStatus->dmaChIntPend = CSL_FEXTR(ctrlBaseAddr->GINTFLAG,31U,0U);
            pDmaStatus->dmaChFTCIntPend = CSL_FEXTR(ctrlBaseAddr->FTCFLAG,31U,0U);
            pDmaStatus->dmaChLFSIntPend = CSL_FEXTR(ctrlBaseAddr->LFSFLAG,31U,0U);
            pDmaStatus->dmaChHBCIntPend = CSL_FEXTR(ctrlBaseAddr->HBCFLAG,31U,0U);
            pDmaStatus->dmaChBTCIntPend = CSL_FEXTR(ctrlBaseAddr->BTCFLAG,31U,0U);
            pDmaStatus->dmaChBERIntPend = CSL_FEXTR(ctrlBaseAddr->BERFLAG,31U,0U);

            pDmaStatus->dmaActiveSrcAddr = CSL_FEXTR(ctrlBaseAddr->PBACSADDR,31U,0U);
            pDmaStatus->dmaActiveDstAddr = CSL_FEXTR(ctrlBaseAddr->PBACDADDR,31U,0U);
            pDmaStatus->dmaActiveElemCnt = CSL_FEXTR(ctrlBaseAddr->PBACTC,12U,0U);
            pDmaStatus->dmaActiveFrameCnt = CSL_FEXTR(ctrlBaseAddr->PBACTC,28U,16U);
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}

/*!
 *  @brief  Function to get the DMA channel completion status based on interrupt type. This API
 *          can be used for polling channel transfer completion when interrupts are not
 *          enabled for that channel by using FTC or BTC as the intType
 *
 *  @pre    DMA_open() has been called and DMA_enableInterrupt()
 *          has not been called for this channel/request type.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @param  errCode         [out] valid errorCode.
 *
 *  @return 1-if the channel's corresponding intType flag is set.
 *          0-if the channel's corresponding intType flag is not set.
 *
 *  @sa     DMA_open()
 */
uint32_t DMA_getChannelInterruptFlag(DMA_Handle handle, uint32_t channel, DMA_IntType intType, int32_t* errCode)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint32_t            pollFlag = 0;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if ((channel >= ptrDMADriver->hwAttrs->numDmaChannels) || (intType >= DMA_IntType_NUM))
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else if (ptrDMADriver->interruptCtx[intType][channel].bIsEnabled==1)
        {
            retCode = DMA_EPOLLINGNOTALLOWED;
        }
        else 
        {
#endif
            volatile DMARegs      *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;

            switch (intType)
            {
                case DMA_IntType_FTC:
                {
                    pollFlag = CSL_FEXTR(ctrlBaseAddr->FTCFLAG,u8_channel,u8_channel);
                }
                break;
                case DMA_IntType_LFS:
                {
                    pollFlag = CSL_FEXTR(ctrlBaseAddr->LFSFLAG,u8_channel,u8_channel);
                }
                break;
                case DMA_IntType_HBC:
                {
                    pollFlag = CSL_FEXTR(ctrlBaseAddr->HBCFLAG,u8_channel,u8_channel);
                }
                break;
                case DMA_IntType_BTC:
                {
                    pollFlag = CSL_FEXTR(ctrlBaseAddr->BTCFLAG,u8_channel,u8_channel);
                }
                break;
                case DMA_IntType_BER:
                {
                    pollFlag = CSL_FEXTR(ctrlBaseAddr->BERFLAG,u8_channel,u8_channel);
                }
                break;
                default :
                /* Didnt match any type - control will never reach here since 
                   parameter validation is done at the start of this function*/
                break;
            }
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* fill error code*/
    if (errCode!=NULL)
    {
        *errCode = retCode;
    }

    /* return */
    return pollFlag;
}


/*!
 *  @brief  Function to clear the DMA channel completion status based on interrupt type. This API
 *          should be called before triggering the channel and using DMA_getChannelInterruptFlag() for polling.
 *
 *  @pre    DMA_open() has been called and DMA_enableInterrupt()
 *          has not been called for this channel/request type.
 *
 *  @param  handle          A DMA_Handle returned from DMA_open()
 *
 *  @param  channel         DMA channel.
 *
 *  @param  intType         DMA interrupt type
 *
 *  @return valid errorCode.
 *
 *  @sa     DMA_open()
 */
int32_t DMA_clearChannelInterruptFlag(DMA_Handle handle, uint32_t channel, DMA_IntType intType)
{
    DMA_Driver          *ptrDMADriver = NULL;
    uintptr_t           key;
    int32_t             retCode = 0;
    uint8_t             u8_channel = (uint8_t)channel;

    DMA_GET_DRIVER_STRUCT(handle); /* this fills the ptrDMADriver */

    /* Disable preemption while setting the registers */
    key = HwiP_disable();

#ifdef DMA_PARAM_CHECK
    if ((ptrDMADriver != NULL)  && (ptrDMADriver->refCnt > 0U))
    {
        if ((channel >= ptrDMADriver->hwAttrs->numDmaChannels) || (intType >= DMA_IntType_NUM))
        {
            /* invalid channel */
            retCode = DMA_EINVAL;
        }
        else if (ptrDMADriver->interruptCtx[intType][channel].bIsEnabled==1)
        {
            retCode = DMA_EPOLLINGNOTALLOWED;
        }
        else 
        {
#endif
            volatile DMARegs      *ctrlBaseAddr = (volatile DMARegs *)ptrDMADriver->hwAttrs->ctrlBaseAddr;

            switch (intType)
            {
                case DMA_IntType_FTC:
                {
                    ctrlBaseAddr->FTCFLAG = CSL_FINSR (ctrlBaseAddr->FTCFLAG, 
                                                       u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_LFS:
                {
                    ctrlBaseAddr->LFSFLAG = CSL_FINSR (ctrlBaseAddr->LFSFLAG, 
                                                       u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_HBC:
                {
                    ctrlBaseAddr->HBCFLAG = CSL_FINSR (ctrlBaseAddr->HBCFLAG, 
                                                       u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_BTC:
                {
                    ctrlBaseAddr->BTCFLAG = CSL_FINSR (ctrlBaseAddr->BTCFLAG, 
                                                       u8_channel, u8_channel, 0x1U);
                }
                break;
                case DMA_IntType_BER:
                {
                    ctrlBaseAddr->BERFLAG = CSL_FINSR (ctrlBaseAddr->BERFLAG, 
                                                       u8_channel, u8_channel, 0x1U);
                }
                break;
                default :
                /* Didnt match any type - control will never reach here since 
                   parameter validation is done at the start of this function*/
                break;
            }
#ifdef DMA_PARAM_CHECK
        }
    }
    else 
    {
        retCode = DMA_ENOINIT;
    }
#endif
    /* Restore the interrupts */
    HwiP_restore(key);

    /* return */
    return retCode;
}

