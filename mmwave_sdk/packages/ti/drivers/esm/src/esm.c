/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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
 *  ======== esm.c ========
 */
/**
    @defgroup ESM_DRIVER_INTERNAL_FUNCTION            ESM Driver Internal Functions
    @ingroup ESM_DRIVER
    @brief
 *   The section has a list of all the internal functions used by the driver
 */

#include <stdint.h>
#include <string.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/esm/include/esm_internal.h>

extern ESM_HWAttrs gESMHwCfgAttrs;

/** @addtogroup ESM_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/* Master control block for ESM driver */
ESM_DriverMCB   gEsmMCB;
/** @}*/

/* Function prototypes */
void interrupt ESM_highpriority_FIQ(void);
void ESM_lowpriority_IRQ(uintptr_t arg);
void ESM_processInterrupt (uint32_t vec, int32_t* groupNum, int32_t* vecNum);


/** @addtogroup ESM_DRIVER_INTERNAL_FUNCTION
 @{ */

/* INTERNAL FUNCTIONS */
/** @brief ESM FIQ interrupt handler: handles Group1 and Group2 high priority errors.
 *         Group 3 errors do not raise a FIQ and hence not handled here.
 *
 */
void interrupt ESM_highpriority_FIQ(void)
{
    uint32_t            esmioffhr;
    uint32_t            vec;
    int32_t             groupNum = MINUS_ONE, vecNum = MINUS_ONE;

    esmioffhr = gEsmMCB.ptrESMBase->ESMIOFFHR;
    vec = esmioffhr - 1U;

    ESM_processInterrupt(vec, &groupNum, &vecNum);
}

/* INTERNAL FUNCTIONS */
/** @brief ESM IRQ interrupt handler: handles Group1 and Group2 low priority errors.
 *         Group 3 errors are not handled here.
 *
 */
void ESM_lowpriority_IRQ(uintptr_t arg)
{
    uint32_t            esmioffhr;
    uint32_t            vec;
    int32_t             groupNum = MINUS_ONE, vecNum = MINUS_ONE;

    esmioffhr = gEsmMCB.ptrESMBase->ESMIOFFLR;
    vec = esmioffhr - 1U;

    ESM_processInterrupt(vec, &groupNum, &vecNum);
}

/** @brief Process the ESM interrupts.
*
*    @param[in] vec: vector offset of the highest pending interrupt.
*
*    @param[out] groupNum: Group number to which the offseted input vector belongs.
*
*    @param[out] vecNum: Vector number in the corresponding group number above.
*
*/
void ESM_processInterrupt (uint32_t vec, int32_t* groupNum, int32_t* vecNum)
{
    uint32_t            index;

    if (vec < 32U)
    {
        /* group 1 0-31 errors */
        gEsmMCB.ptrESMBase->ESMSR1 = CSL_FMKR ((uint8_t)vec, (uint8_t)vec, 1U);
#ifdef ESM_DEBUG
        gEsmMCB.debugEsmISRCount[0]++;
#endif
        *groupNum = 1;
        *vecNum = vec;
    }
    else if (vec < 64U)
    {
        /* group 2 0-31 errors */
        vec = vec - 32;
        gEsmMCB.ptrESMBase->ESMSR2 = CSL_FMKR ((uint8_t)vec, (uint8_t)vec, 1U);
#ifdef ESM_DEBUG
        gEsmMCB.debugEsmISRCount[1]++;
#endif
        *groupNum = 2;
        *vecNum = vec;
    }
    else if (vec < 96U)
    {
        /* group 1 32-63 errors */
        vec = vec - 64;
        gEsmMCB.ptrESMBase->ESMSR4 = CSL_FMKR ((uint8_t)vec, (uint8_t)vec, 1U);
#ifdef ESM_DEBUG
        gEsmMCB.debugEsmISRCount[2]++;
#endif
        *groupNum = 1;
        *vecNum = vec + 32;
    }
    else
    {
        DebugP_assert(0);
    }
    if (*groupNum != MINUS_ONE)
    {
        /* Check if notify function was registered? */
        for (index = 0; index < ESM_MAX_NOTIFIERS; index++)
        {
            if ((*vecNum == gEsmMCB.notifyParams[index].errorNumber) &&
                    (*groupNum == gEsmMCB.notifyParams[index].groupNumber))
            {
                gEsmMCB.notifyParams[index].notify(gEsmMCB.notifyParams[index].arg);
                break;
            }
        }
    }
}
/** @}*/


/* EXTERNAL FUNCTIONS */

/** @brief Function intializes the ESM driver
*
*    @param[in] bClearErrors: boolean value to indicate if old ESM pending errors should be cleared or not
*                             value = 0: do not clear
*                             value = 1: clear all ESM group errors
*                             hint: If you using TI RTOS, then ESM errors are cleared before
*                                   entering main and this flag can be set to 0. For any other RTOS, check the
*                                   the RTOS implementation or set this flag to 1
*
*    @return    Handle to the ESM Driver
*
*/
ESM_Handle ESM_init(uint8_t bClearErrors)
{
    /* Initialize the allocated memory */
    memset ((void *)&gEsmMCB, 0U, sizeof(ESM_DriverMCB));

    gEsmMCB.ptrESMBase = (volatile ESMRegs*)gESMHwCfgAttrs.baseAddr;

#ifdef SUBSYS_MSS
    HwiP_Params     hwiParams;

    HwiP_Params_init(&hwiParams);
    hwiParams.name = "ESM_FIQ";
    hwiParams.type = HwiP_Type_FIQ;
    gEsmMCB.hwiHandleHi = HwiP_create(gESMHwCfgAttrs.highPrioIntNum, (HwiP_Fxn)ESM_highpriority_FIQ, &hwiParams);

    /* Debug Message: */
    DebugP_log2 ("Debug: ESM Driver Registering HWI(FIQ) ISR [%p] for Interrupt %d\n",
                      gEsmMCB.hwiHandleHi, gESMHwCfgAttrs.highPrioIntNum);

    HwiP_Params_init(&hwiParams);
    hwiParams.name = "ESM_IRQ";
    hwiParams.type = HwiP_Type_IRQ;
    gEsmMCB.hwiHandleLo = HwiP_create(gESMHwCfgAttrs.lowPrioIntNum, (HwiP_Fxn)ESM_lowpriority_IRQ, &hwiParams);

    /* Debug Message: */
    DebugP_log2 ("Debug: ESM Driver Registering HWI(IRQ) ISR [%p] for Interrupt %d\n",
                      gEsmMCB.hwiHandleLo, gESMHwCfgAttrs.lowPrioIntNum);
#endif

    if (bClearErrors == 1U)
    {
        /* ESM Group 1: 0-31 errors */

        /* read */
        gEsmMCB.esmInitStatus[0U] = gEsmMCB.ptrESMBase->ESMSR1;

        /*  clear */
        gEsmMCB.ptrESMBase->ESMSR1 = gEsmMCB.esmInitStatus[0U];


        /* ESM Group 2: 0-31 errors */

        /* read */
        gEsmMCB.esmInitStatus[1U] = gEsmMCB.ptrESMBase->ESMSR2;

        /*  clear */
        gEsmMCB.ptrESMBase->ESMSR2 = gEsmMCB.esmInitStatus[1U];


        /* ESM Group 3: 0-31 errors */

        /* read */
        gEsmMCB.esmInitStatus[2U] = gEsmMCB.ptrESMBase->ESMSR3;

        /*  clear */
        gEsmMCB.ptrESMBase->ESMSR3 = gEsmMCB.esmInitStatus[2U];


        /* ESM Group 1: 32-63 errors */

        /* read */
        gEsmMCB.esmInitStatus[3U] = gEsmMCB.ptrESMBase->ESMSR4;

        /*  clear */
        gEsmMCB.ptrESMBase->ESMSR4 = gEsmMCB.esmInitStatus[3U];


        /* ESM Group 2 Shadow register: 0-31 errors */

        /* read */
        gEsmMCB.esmInitStatus[4U] = gEsmMCB.ptrESMBase->ESMSSR2;

        /*  clear */
        gEsmMCB.ptrESMBase->ESMSSR2 = gEsmMCB.esmInitStatus[4U];
    }
    return (ESM_Handle)&gEsmMCB;
}

/** @fn int32_t ESM_close(void)
*   @brief Close the ESM driver
*
*    @param[in] handle: Handle to the ESM Driver
*
*    @return    Value < 0 - in case of error
*               Value > 0 - Success
*
*/
int32_t ESM_close(ESM_Handle handle)
{
    ESM_DriverMCB*      ptrEsmMCB = NULL;
    int32_t             retVal = 0;

    /* Get the pointer to the ESM Driver Block */
    ptrEsmMCB = (ESM_DriverMCB*)handle;
    if (ptrEsmMCB == NULL)
    {
        retVal = MINUS_ONE;
    }
    else
    {
        /* Was the HWI registered?  */
        if (ptrEsmMCB->hwiHandleHi)
        {
            /* YES: Delete and unregister the interrupt handler. */
            HwiP_delete(ptrEsmMCB->hwiHandleHi);
        }
        /* Was the HWI registered?  */
        if (ptrEsmMCB->hwiHandleLo)
        {
            /* YES: Delete and unregister the interrupt handler. */
            HwiP_delete(ptrEsmMCB->hwiHandleLo);
        }
    }
    return retVal;
}

/** @brief Register the notifers the ESM module will call back if error interrupt is detected.
*
*   @param[in] handle: Handle to the ESM Driver.
*
*   @param[in] params: Notifier error number and callback function.
*
*   @param[out]  errCode
*      Error code populated on error.
*
*    @return    Success -   Notifier index. Used when deregistering the notifier.
*               Error   -   ESM Error code
*
*/
int32_t ESM_registerNotifier(ESM_Handle handle, ESM_NotifyParams* params, int32_t* errCode)
{
    ESM_DriverMCB*      ptrEsmMCB = NULL;
    int32_t             retVal = 0;
    uintptr_t           key;
    int32_t             notifyIndex;

    /* Get the pointer to the ESM Driver Block */
    ptrEsmMCB = (ESM_DriverMCB*)handle;
    if ((ptrEsmMCB == NULL) || (params == NULL))
    {
        *errCode = ESM_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Critical Section Protection: Notify registration needs to be
         * protected against multiple threads */
        key = HwiP_disable();

        /* Find a free notifier index */
        for (notifyIndex = 0; notifyIndex < ESM_MAX_NOTIFIERS; notifyIndex++)
        {
            if (gEsmMCB.notifyParams[notifyIndex].groupNumber == 0)
            {
                break;
            }
        }

        if (notifyIndex == ESM_MAX_NOTIFIERS)
        {
            /* Max allowed notifiers have already been registered */
            *errCode = ESM_ENOMEM;
            retVal = MINUS_ONE;
        }
        else
        {
            /* Check if the notifier handles group 1 or group 2 errors.
               Group 2 errors are enabled by default. Group 1 errors have to be explicitly enabled.
             */
            if (params->groupNumber == 1)
            {
                if (params->errorNumber < 32)
                {
                    gEsmMCB.ptrESMBase->ESMIESR1 = CSL_FINSR (gEsmMCB.ptrESMBase->ESMIESR1,
                                                                params->errorNumber,
                                                                params->errorNumber,
                                                                1U);
                }
                else if ((params->errorNumber >= 32) && (params->errorNumber < 64))
                {
                    gEsmMCB.ptrESMBase->ESMIESR4 = CSL_FINSR (gEsmMCB.ptrESMBase->ESMIESR4,
                                                                (params->errorNumber - 32U),
                                                                (params->errorNumber - 32U),
                                                                1U);
                }
            }
            memcpy ((void *)&ptrEsmMCB->notifyParams[notifyIndex], (void *)params, sizeof (ESM_NotifyParams));
            retVal = notifyIndex;
        }
        /* Release the critical section: */
        HwiP_restore(key);
    }
    return retVal;
}

/** @brief Deregister the ESM notifers.
*
*   @param[in] handle: Handle to the ESM Driver.
*
*   @param[in] notifyIndex: Notifier index returned when the notifier was registered.
*
*   @param[out]  errCode
*      Error code populated on error.
*
*    @return    Success -   0
*               Error   -   ESM Error code
*
*/
int32_t ESM_deregisterNotifier(ESM_Handle handle, int32_t notifyIndex, int32_t* errCode)
{
    ESM_DriverMCB*      ptrEsmMCB = NULL;
    int32_t             retVal = 0;
    uintptr_t           key;
    uint32_t            groupNumber;
    uint32_t            errorNumber;

    /* Get the pointer to the ESM Driver Block */
    ptrEsmMCB = (ESM_DriverMCB*)handle;
    if ((ptrEsmMCB == NULL) || (notifyIndex >= ESM_MAX_NOTIFIERS))
    {
        *errCode = ESM_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Critical Section Protection: Notify registration needs to be
         * protected against multiple threads */
        key = HwiP_disable();

        groupNumber = gEsmMCB.notifyParams[notifyIndex].groupNumber;
        errorNumber = gEsmMCB.notifyParams[notifyIndex].errorNumber;

        /* Check if the notifier is to handle group 1 or group 2 errors.
         * Group 2 errors are enabled by default. Group 1 errors have to be explicitly enabled.
         */
        if (groupNumber == 1)
        {
            if (errorNumber < 32)
            {
                    gEsmMCB.ptrESMBase->ESMIECR1 = CSL_FINSR (gEsmMCB.ptrESMBase->ESMIECR1,
                                                                errorNumber,
                                                                errorNumber,
                                                                0);
            }
            else if ((errorNumber >= 32) && (errorNumber < 64))
            {
                gEsmMCB.ptrESMBase->ESMIECR4 = CSL_FINSR (gEsmMCB.ptrESMBase->ESMIECR4,
                                                                (errorNumber - 32U),
                                                                (errorNumber - 32U),
                                                                0);
            }
            memset ((void *)&ptrEsmMCB->notifyParams[notifyIndex], 0, sizeof (ESM_NotifyParams));
        }
        /* Release the critical section: */
        HwiP_restore(key);
    }
    return retVal;
}
