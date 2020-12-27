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
 *  ======== HwiP_tirtos.c ========
 */

#include <ti/drivers/osal/HwiP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#ifdef SUBSYS_MSS
#include <ti/sysbios/family/arm/v7r/vim/Hwi.h>
#elif defined (SUBSYS_DSS)
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#else
#error "Error: Compiler Flag for the MSS *OR* DSS Subsystem is not defined"
#endif

#include "tirtos_config.h"
#include <ti/drivers/osal/DebugP.h>

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(int32_t interruptNum)
{
#ifdef SUBSYS_MSS
    Hwi_clearInterrupt(interruptNum);
#else
    DebugP_assert(0);
#endif
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(int32_t interruptNum, HwiP_Fxn hwiFxn,
                        HwiP_Params *params)
{
#ifdef SUBSYS_MSS
    Hwi_Handle handle;
    Error_Block eb;
    Hwi_Params  hwiParams;

    Error_init(&eb);
    Hwi_Params_init(&hwiParams);

    /* Overwrite the BIOS HWI Parameters with the supplied arguments */
    hwiParams.instance->name = params->name;
    hwiParams.arg            = (xdc_UArg)(params->arg);
    hwiParams.priority       = (int32_t)params->priority;
    hwiParams.type           = params->type==HwiP_Type_FIQ? Hwi_Type_FIQ : Hwi_Type_IRQ;
    hwiParams.enableInt      = params->enableInt;

    /* Register the Interrupt: */
    handle = Hwi_create(interruptNum, (Hwi_FuncPtr)hwiFxn, &hwiParams, &eb);
    DebugP_assert(handle != NULL);

    return ((HwiP_Handle)handle);
#endif
#ifdef SUBSYS_DSS
    /* go through event combiner */
    EventCombiner_dispatchPlug(interruptNum, hwiFxn, params->arg, params->enableInt);
    return((HwiP_Handle)interruptNum);
#endif

}

/*
 *  ======== HwiP_delete ========
 */
HwiP_Status HwiP_delete(HwiP_Handle handle)
{
    DebugP_assert(handle != NULL);

#ifdef SUBSYS_MSS
    Hwi_delete((Hwi_Handle *)&handle);
#endif
#ifdef SUBSYS_DSS
    EventCombiner_disableEvent((int32_t)handle);
#endif
    return (HwiP_OK);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    uintptr_t key;

    key = Hwi_disable();

    return (key);
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(int32_t interruptNum)
{
#ifdef SUBSYS_DSS
    EventCombiner_disableEvent(interruptNum);
#else
    Hwi_disableInterrupt(interruptNum);
#endif
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(int32_t interruptNum)
{
#ifdef SUBSYS_DSS
    EventCombiner_enableEvent(interruptNum);
#else
    Hwi_enableInterrupt(interruptNum);
#endif
}

/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    params->name = NULL;
    params->arg = 0;
    params->priority = ~0;
    params->enableInt = TRUE;
#ifdef SUBSYS_MSS
    params->type = HwiP_Type_IRQ;
#endif
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t key)
{
    Hwi_restore(key);
}
