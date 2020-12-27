/**
 *   @file  i2c_master.h
 *
 *   @brief
 *      This is the internal Header for the I2C Master Driver. This header
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

#ifndef I2CMASTER_INTERNAL_H
#define I2CMASTER_INTERNAL_H

#include <ti/drivers/i2c/I2C.h>
#include <ti/drivers/i2c/include/i2c_internal.h>

/**
 * @brief   This is the I2C Driver registered function table
 */
extern I2C_FxnTable gI2CMasterFxnTable;

/**
 * @brief   This is the I2C Driver registered function table
 */
extern I2C_HwCfg gI2CHwCfg;

/**
@defgroup I2C_INTERNAL_FUNCTION             I2C Driver Internal Functions
@ingroup I2C_DRIVER
*/
/**
@defgroup I2C_INTERNAL_DATA_STRUCTURE       I2C Driver Internal Data Structures
@ingroup I2C_DRIVER
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup I2C_INTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief  I2C mode
 *
 *  This enum defines the state of the I2C Master driver's state-machine.
 */
typedef enum I2C_Mode_t
{
    I2C_IDLE_MODE = 0,  /* I2C Master is not performing a transaction */
    I2C_WRITE_MODE = 1,     /* I2C Master is currently performing write */
    I2C_READ_MODE = 2,      /* I2C Master is currently performing read */
    I2C_START_MODE = 3,     /* I2C Master received a START from a master */
    I2C_ERROR = 0xFF    /* I2C Master error has occurred, exit gracefully */
} I2C_Mode;

/**
 * @brief
 *  I2C Driver Master Control Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the I2C Driver.
 */
typedef struct I2C_MasterMCB_t
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
     * @brief   Registered Interrupt Handler.
     */
    HwiP_Handle             hwiHandle;

    /**
     * @brief   Pointer to the current active transaction
     */
    I2C_Transaction*        currentTransaction;

    /**
     * @brief   Data structures used to support queuing operation. Write buffer pointer.
     */
    uint8_t*                ptrWriteBuffer;

    /**
     * @brief   Data structures used to support queuing operation. Internal write Count.
     */
    size_t                  writeCount;

    /**
     * @brief   Data structures used to support queuing operation. Read buffer pointer.
     */
    uint8_t*                ptrReadBuffer;

    /**
     * @brief   Data structures used to support queuing operation. Internal Read Count.
     */
    size_t                  readCount;

    /**
     * @brief   Mode of the I2C driver state machine.
     */
    I2C_Mode                mode;

    /**
     * @brief   I2C Parameters which were used to initialize the driver instance
     */
    I2C_Params              params;

    /**
     * @brief   Number of interrupts received
     */
    uint32_t                interruptsRxed;
}I2C_MasterMCB;

I2C_Config I2C_config[] =
{
    {
        &gI2CMasterFxnTable,            /* I2C Driver Function Table:     */
        (void *)NULL,                   /* I2C Driver Object:             */
        (void *)&gI2CHwCfg              /* I2C Hw configuration:          */
    },
    {
        NULL,                           /* I2C Driver Function Table:     */
        NULL,                           /* I2C Driver Object:             */
        NULL                            /* I2C Hw configuration:          */
    }
};

extern void I2CMasterInitClk(I2CRegs* i2cRegs, uint32_t sysClk, uint32_t internalClk, uint32_t outputClk);
extern void I2CMasterEnable(I2CRegs* i2cRegs);
extern bool I2CMasterBusBusy(I2CRegs* i2cRegs);
extern uint32_t I2CMasterErr(I2CRegs* i2cRegs);
extern void I2CMasterControl(I2CRegs* i2cRegs, uint32_t ctrlMask, uint32_t ctrlCmds);
extern void I2CMasterStart(I2CRegs* i2cRegs);
extern void I2CMasterStop(I2CRegs* i2cRegs);
extern void I2CMasterSetSlaveAddr(I2CRegs* i2cRegs, uint32_t slaveAdd);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* I2CMASTER_INTERNAL_H */
