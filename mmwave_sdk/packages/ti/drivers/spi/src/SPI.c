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
 *  ======== SPI.c ========
 */

#include <stdint.h>
#include <stdlib.h>
#include <ti/drivers/spi/SPI.h>
#include <ti/drivers/dma/dma.h>
#include <ti/common/sys_common.h>

/* Externs */
extern const SPI_Config SPI_config[];

/* Used to check status and initialization */
static int32_t SPI_count = MINUS_ONE;

/* Default Master params */
const SPI_MasterModeParams SPI_defaultMasterParams =
{
    /* Master Params */
    5000000U,            /* bitRate */
    1,                   /* numSlaves */
    0,                   /* t2cDelay */
    0,                   /* c2tDelay */
    0,                   /* wDelay */
    {
        /* Slave - 0 */
        {
            (uint8_t)0U,                    /* CS0 */
            (uint8_t)MIBSPI_RAM_MAX_ELEM,   /* ramBufLen in number of dataSize */
            {
                DMA_CHANNEL_NONE,           /* TX DMA channel */
                DMA_CHANNEL_NONE            /* RX DMA channel */
            }
        },
        /* Slave - 1 - not valid */
        {
            (uint8_t)0xFFU,               /* CS_NONE */
            (uint8_t)0U,                    /* ramBufLen in number of dataSize */
            {
                DMA_CHANNEL_NONE,           /* TX DMA channel */
                DMA_CHANNEL_NONE            /* RX DMA channel */
            }
        },
        /* Slave - 2 - not valid */
        {
            (uint8_t)0xFFU,               /* CS_NONE */
            (uint8_t)0U,                    /* ramBufLen in number of dataSize */
            {
                DMA_CHANNEL_NONE,           /* TX DMA channel */
                DMA_CHANNEL_NONE            /* RX DMA channel */
            }
        }
    }
};

/* Default Slave params */
const SPI_SlaveModeParams SPI_defaultSlaveParams =
{
    /* Slave Params */
    {
        DMA_CHANNEL_NONE,   /* TX DMA channel */
        DMA_CHANNEL_NONE    /* RX DMA channel */
    },
    0U                      /* CS0 */
};

/* Default SPI parameters structure */
const SPI_Params SPI_defaultParams =
{
    .mode               = SPI_SLAVE,
    .pinMode            = SPI_PINMODE_4PIN_CS,
    .dataSize           = 16U,
    .frameFormat        = SPI_POL0_PHA0,
    .shiftFormat        = SPI_MSB_FIRST,
    .dmaEnable          = (uint8_t)1U,
    .dmaHandle          = (DMA_Handle)NULL,
    .eccEnable          = (uint8_t)1U,
    .csHold             = (uint8_t)0U,
    .txDummyValue       = (uint16_t)0xFFFFU,
    .transferMode       = SPI_MODE_BLOCKING,
    .transferTimeout    = SPI_WAIT_FOREVER,
    .transferCallbackFxn  = NULL,
    .custom             = (uintptr_t) NULL
};

/*
 *  ======== SPI_close ========
 */
void SPI_close(SPI_Handle handle)
{
    handle->fxnTablePtr->closeFxn(handle);
}

/*
 *  ======== SPI_control ========
 */
int32_t SPI_control(SPI_Handle handle, uint32_t cmd, void *arg)
{
    return (handle->fxnTablePtr->controlFxn(handle, cmd, arg));
}

/*
 *  ======== SPI_getStats ========
 */
int32_t SPI_getStats(SPI_Handle handle, SPI_Stats *ptrStats)
{
    return (handle->fxnTablePtr->getStatsFxn(handle, ptrStats));
}

/*
 *  ======== SPI_init ========
 */
void SPI_init(void)
{
    if (SPI_count == MINUS_ONE) {
        /* Call each driver's init function */
        for (SPI_count = 0; SPI_config[SPI_count].fxnTablePtr != NULL; SPI_count++) {
            SPI_config[SPI_count].fxnTablePtr->initFxn((SPI_Handle)&(SPI_config[SPI_count]));
        }
    }
}

/*
 *  ======== SPI_open ========
 */
SPI_Handle SPI_open(uint32_t index, SPI_Params *params)
{
    SPI_Handle handle;

    if ((int32_t)index >= SPI_count) {
        return ((SPI_Handle)NULL);
    }

    /* If params are NULL use defaults */
    if (params == NULL) {
        params = (SPI_Params *) &SPI_defaultParams;
    }

    /* Get handle for this driver instance */
    handle = (SPI_Handle)&(SPI_config[index]);

    return (handle->fxnTablePtr->openFxn(handle, params));
}

/*
 *  ======== SPI_Params_init ========
 */
void SPI_Params_init(SPI_Params *params)
{
    *params = SPI_defaultParams;

    if(params->mode == SPI_MASTER)
    {
        /* Set Master Params */
        params->u.masterParams = SPI_defaultMasterParams;
    }
    else
    {
        /* Set Slave Params */
        params->u.slaveParams = SPI_defaultSlaveParams;
    }
}

/*
 *  ======== SPI_serviceISR ========
 */
void SPI_serviceISR(SPI_Handle handle)
{
    handle->fxnTablePtr->serviceISRFxn(handle);
}

/*
 *  ======== SPI_transfer ========
 */
bool SPI_transfer(SPI_Handle handle, SPI_Transaction *transaction)
{
    return (handle->fxnTablePtr->transferFxn(handle, transaction));
}

/*
 *  ======== SPI_transferCancel ========
 */
void SPI_transferCancel(SPI_Handle handle)
{
    handle->fxnTablePtr->transferCancelFxn(handle);
}
