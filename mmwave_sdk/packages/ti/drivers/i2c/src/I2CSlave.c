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
 *  ======== I2CSlave.c ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/i2c/I2CSlave.h>

/* Externs */
extern const I2CSlave_Config I2CSlave_config[];

/* Used to check status and initialization */
static int32_t I2CSlave_count = MINUS_ONE;

/* Default I2CSlave parameters structure */
const I2CSlave_Params I2CSlave_defaultParams = {
    I2CSLAVE_MODE_BLOCKING,  /* transferMode */
    0,                       /* slaveAddress */
    (uintptr_t) NULL         /* custom */
};

/*
 *  ======== I2CSlave_close ========
 */
void I2CSlave_close(I2CSlave_Handle handle)
{
    handle->fxnTablePtr->closeFxn(handle);
}

/*
 *  ======== I2CSlave_control ========
 */
int32_t I2CSlave_control(I2CSlave_Handle handle, uint32_t cmd, void *arg)
{
    return (handle->fxnTablePtr->controlFxn(handle, cmd, arg));
}

/*
 *  ======== I2CSlave_init ========
 */
void I2CSlave_init(void)
{
    if (I2CSlave_count == MINUS_ONE) {
        /* Call each driver's init function */
        for (I2CSlave_count = 0;
                I2CSlave_config[I2CSlave_count].fxnTablePtr != NULL;
                    I2CSlave_count++) {
            I2CSlave_config[I2CSlave_count].fxnTablePtr-> \
                initFxn((I2CSlave_Handle)&(I2CSlave_config[I2CSlave_count]));
        }
    }
}

/*
 *  ======== I2CSlave_open ========
 */
I2CSlave_Handle I2CSlave_open(uint32_t index, I2CSlave_Params *params)
{
    I2CSlave_Handle handle;

    if ((int32_t)index >= I2CSlave_count)
    {
        handle = NULL;
    }
    else
    {
        /* Use defaults if params are NULL. */
        if (params == NULL)
        {
            params = (I2CSlave_Params *) &I2CSlave_defaultParams;
        }

        /* Get handle for this driver instance */
        handle = (I2CSlave_Handle)&(I2CSlave_config[index]);

        /* Open the I2C slave driver */
        handle = handle->fxnTablePtr->openFxn(handle, params);
    }
    return (handle);
}

/*
 *  ======== I2CSlave_Params_init =======
 */
void I2CSlave_Params_init(I2CSlave_Params *params)
{
    *params = I2CSlave_defaultParams;
}

/*
 *  ======== I2CSlave_read ========
 */
bool I2CSlave_read(I2CSlave_Handle handle, void *buffer, size_t size)
{
    return (handle->fxnTablePtr->readFxn(handle, buffer, size));
}

/*
 *  ======== I2CSlave_write ========
 */
bool I2CSlave_write(I2CSlave_Handle handle, const void *buffer, size_t size)
{
    return (handle->fxnTablePtr->writeFxn(handle, buffer, size));
}
