/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
 *  ======== ADCBuf.c ========
 */


#include <stdint.h>
#include <stdlib.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/adcbuf/ADCBuf.h>

/* ADC configuration array from application */
extern const ADCBuf_Config ADCBuf_config[2];

/* Used to check status and initialization */
static int32_t ADCBuf_count = -((int32_t)1);

#ifdef SOC_XWR14XX
/* Default ADC parameters structure */
const ADCBuf_Params ADCBuf_defaultParams = {
    /* ADC buffer source, DFE or HIL */
    ADCBUF_SOURCE_DFE,
    /* Continuous mode selection */
    0,
    /* Chirp Threshold for non-continous operation */
    1,
    /* Custom configuration */
    NULL
};
#endif

#if defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX)
/* Default ADC parameters structure */
const ADCBuf_Params ADCBuf_defaultParams = {
    /* ADC buffer source, DFE or HIL */
    ADCBUF_SOURCE_DFE,
    /* Continuous mode selection */
    0,
    /* Ping buffer Chirp Threshold for non-continous operation */
    1,
    /* Pong buffer Chirp Threshold for non-continous operation */
    1,
    /* Custom configuration */
    NULL
};
#endif

/*
 *  ======== ADCBuf_close ========
 */
void ADCBuf_close(ADCBuf_Handle handle) {
    handle->fxnTablePtr->closeFxn(handle);
}
/*
 *  ======== ADCBuf_control ========
 */
int_fast16_t ADCBuf_control(ADCBuf_Handle handle, uint_fast8_t cmd, void *arg) {
    return handle->fxnTablePtr->controlFxn(handle, cmd, arg);
}

/*
 *  ======== ADCBuf_getChanBufAddr ========
 */
uint32_t ADCBuf_getChanBufAddr(ADCBuf_Handle handle, uint8_t channel, int32_t* errCode) {
    return handle->fxnTablePtr->getChanBufAddr(handle, channel, errCode);
}

/*
 *  ======== ADCBuf_init ========
 */
void ADCBuf_init(void) {
    /* Call each driver's init function */
    for (ADCBuf_count = 0; ADCBuf_config[ADCBuf_count].fxnTablePtr != NULL; ADCBuf_count++) {
        ADCBuf_config[ADCBuf_count].fxnTablePtr->initFxn((ADCBuf_Handle) & (ADCBuf_config[ADCBuf_count]));
    }
}

/*
 *  ======== ADCBuf_open ========
 */
ADCBuf_Handle ADCBuf_open(uint_fast8_t index, ADCBuf_Params *params) {
    ADCBuf_Handle handle;

    if ((int32_t)index >= ADCBuf_count) {
        handle = (ADCBuf_Handle)NULL;
    }
    else
    {
        /* If params are NULL use defaults */
        if (params == NULL) {
            params = (ADCBuf_Params *) &ADCBuf_defaultParams;
        }

        /* Get handle for this driver instance */
        handle = (ADCBuf_Handle)&(ADCBuf_config[index]);

        /* Open the ADCBUF mmWave Driver: */
        handle =  handle->fxnTablePtr->openFxn(handle, params);
    }
    return (handle);
}

/*
 *  ======== ADCBuf_Params_init ========
 */
void ADCBuf_Params_init(ADCBuf_Params *params) {
    *params = ADCBuf_defaultParams;
}

