/**
 *   @file  i2c_slave.h
 *
 *   @brief
 *      This is the internal Header for the I2C Slave Driver. This header
 *      file should *NOT* be directly included by applications.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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

#ifndef I2CSLAVE_INTERNAL_H
#define I2CSLAVE_INTERNAL_H

#include <ti/drivers/i2c/I2CSlave.h>
#include <ti/drivers/i2c/include/i2c_internal.h>

/**
 * @brief   This is the I2C Slave Driver registered function table
 */
extern I2CSlave_FxnTable gI2CSlaveFxnTable;

/**
 * @brief   This is the I2C Driver registered function table
 */
extern I2C_HwCfg gI2CHwCfg;

/**
@defgroup I2CSLAVE_INTERNAL_FUNCTION            I2C Slave Driver Internal Functions
@ingroup I2CSLAVE_DRIVER
*/
/**
@defgroup I2CSLAVE_INTERNAL_DATA_STRUCTURE      I2C Slave Driver Internal Data Structures
@ingroup I2CSLAVE_DRIVER
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup I2CSLAVE_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  I2C Driver Slave Control Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the I2C Slave Driver.
 */
typedef struct I2C_SlaveMCB_t
{
    /**
     * @brief   I2C driver internal state
     */
    I2C_DriverState         state;

    /**
     * @brief   Handle to the Semaphore used to notify application of I2C transfer complete
     */
    SemaphoreP_Handle       transferComplete;

    /**
     * @brief   Flag used to determine if a transfer is in progress.
     */
    uint32_t                transferInProgress;

    /**
     * @brief   Registered Interrupt Handler.
     */
    HwiP_Handle             hwiHandle;

    /**
     * @brief   Write buffer pointer.
     */
    uint8_t*                ptrWriteBuffer;

    /**
     * @brief   Internal write Count.
     */
    size_t                  count;

    /**
     * @brief   Read buffer pointer.
     */
    uint8_t*                ptrReadBuffer;

    /**
     * @brief   Mode of the I2C Slave driver state machine.
     */
    volatile I2CSlave_Mode  mode;

    /**
     * @brief   I2C Parameters which were used to initialize the driver instance
     */
    I2CSlave_Params         params;

    /**
     * @brief   Number of interrupts received
     */
    uint32_t                interruptsRxed;
}I2C_SlaveMCB;

I2CSlave_Config I2CSlave_config[] =
{
    {
        &gI2CSlaveFxnTable,             /* I2C Driver Function Table:     */
        (void *)NULL,                   /* I2C Driver Object:             */
        (void *)&gI2CHwCfg              /* I2C Hw configuration:          */
    },
    {
        NULL,                           /* I2C Driver Function Table:     */
        NULL,                           /* I2C Driver Object:             */
        NULL                            /* I2C Hw configuration:          */
    }
};

extern void I2CSlaveEnable(I2CRegs* i2cRegs);




/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* I2CSLAVE_INTERNAL_H */
