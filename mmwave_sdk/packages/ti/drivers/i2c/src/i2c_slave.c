/**
 *   @file  i2c_slave.c
 *
 *   @brief
 *      The file implements the I2C Slave Driver for the XWR14xx/XWR16xx/XWR18xx/XWR68xx.
 *      The file implements the driver to conform to the MCPI standards.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/i2c/I2CSlave.h>
#include <ti/drivers/i2c/include/i2c_slave.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/* Registered callback functions: */
static void             I2C_slave_close(I2CSlave_Handle handle);
static int32_t          I2C_slave_control(I2CSlave_Handle handle, uint32_t cmd, void *arg);
static void             I2C_slave_init(I2CSlave_Handle handle);
static I2CSlave_Handle  I2C_slave_open(I2CSlave_Handle handle, const I2CSlave_Params *params);
bool                    I2C_slave_read(I2CSlave_Handle handle, void *buffer, size_t size);
bool                    I2C_slave_write(I2CSlave_Handle handle, const void *buffer, size_t size);


/* I2C Slave Driver API: */
static void     I2CSlave_blockingTransferCallback(I2CSlave_Handle handle, bool transferStatus);
static bool     I2CSlave_setupTransfer(I2CSlave_Handle handle, uint8_t* buffer, size_t size, uint32_t readWriteFlag);
static void     I2CSlave_ISR (uintptr_t arg);

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   I2C Slave Driver Function Table
 */
I2CSlave_FxnTable gI2CSlaveFxnTable =
{
    &I2C_slave_close,
    &I2C_slave_control,
    &I2C_slave_init,
    &I2C_slave_open,
    &I2C_slave_read,
    &I2C_slave_write
};

/**************************************************************************
 ************************* I2C Slave Driver Functions ********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is the callback function which is invoked to indicate a
 *      transfer is complete when BLOCKING mode is used.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *  @param[in]  transferStatus
 *      Status of the transfer
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2CSlave_blockingTransferCallback(I2CSlave_Handle handle, bool transferStatus)
{
    I2CSlave_Config*    ptrI2CConfig;
    I2C_SlaveMCB*       ptrI2CMCB;

    /* Get the I2C Configuration */
    ptrI2CConfig = (I2CSlave_Config*)handle;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_SlaveMCB*)ptrI2CConfig->object;

    DebugP_log1 ("Debug: I2C Slave Driver BLOCKING MODE - posting transfer complete semaphore. Status : %d\n", transferStatus);

    /* Post the semaphore to indicate transfer is complete */
    SemaphoreP_post(ptrI2CMCB->transferComplete);
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the I2C Slave Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2CSlave_ISR (uintptr_t arg)
{
    I2CSlave_Config*    ptrI2CConfig;
    I2C_SlaveMCB*       ptrI2CMCB;
    I2C_HwCfg*          ptrHwCfg;
    uint32_t            intCode;
    volatile uint32_t   intStat;

    /* Get the I2C Configuration */
    ptrI2CConfig = (I2CSlave_Config*)arg;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_SlaveMCB*)ptrI2CConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

    /* Increment the ISR counter */
    ptrI2CMCB->interruptsRxed++;

    intCode = I2CGetIntVector(ptrHwCfg->ptrI2cBase);
    intStat = I2CGetIntStatus(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);

    switch (intCode)
    {
        case I2C_IVR_INTCODE_AAS:
        {
            ptrI2CMCB->mode = I2CSLAVE_START_MODE;

            DebugP_log1("Debug: I2C Slave Driver(%p) ISR START DETECTED\n", ptrHwCfg->ptrI2cBase);
            if(ptrI2CMCB->transferInProgress)
            {
                ptrI2CMCB->transferInProgress = 0;

                /* Restart condition */
                /* Post the semaphore to indicate transfer is complete */
                SemaphoreP_post(ptrI2CMCB->transferComplete);
            }
            else
            {
                /* Addressed by Master for the first time. Enable interrupts and return */
                I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);
                I2CIntEnable(ptrHwCfg->ptrI2cBase,
                                                I2C_INT_MASK_TRANSMIT_READY | I2C_INT_MASK_RECV_READY |
                                                I2C_INT_MASK_ADRR_READY_ACESS | I2C_INT_MASK_ADRR_SLAVE |
                                                I2C_INT_MASK_NO_ACK | I2C_INT_MASK_STOP_CONDITION);
            }
            break;
        }
        case I2C_IVR_INTCODE_XRDY:
        {
            ptrI2CMCB->transferInProgress = 1;
            ptrI2CMCB->mode = I2CSLAVE_WRITE_MODE;
            if (ptrI2CMCB->count)
            {
                I2CPutData(ptrHwCfg->ptrI2cBase, *(ptrI2CMCB->ptrWriteBuffer));
                DebugP_log2("Debug: I2C Slave Driver(%p) ISR I2CSLAVE_WRITE_MODE Writing data 0x%x\n",
                                ptrHwCfg->ptrI2cBase, *(ptrI2CMCB->ptrWriteBuffer));
                ptrI2CMCB->count--;
                ptrI2CMCB->ptrWriteBuffer++;
            }
            else
            {
                DebugP_log1("Debug: I2C Slave Driver(%p) ISR I2CSLAVE_WRITE_MODE No more bytes available to write. Writing 0xFF\n",
                                ptrHwCfg->ptrI2cBase);
                I2CPutData(ptrHwCfg->ptrI2cBase, 0xFF);
            }
            break;
        }
        case I2C_IVR_INTCODE_NACK:
        {
            /* Get a NACK from master, stop the transfer and callback */
            ptrI2CMCB->mode = I2CSLAVE_ERROR;
            ptrI2CMCB->transferInProgress = 0;

            DebugP_log1("Debug: I2C Slave Driver(%p) ISR NACK DETECTED\n", ptrHwCfg->ptrI2cBase);

            /* Callback to application or post semaphore */
            I2CSlave_blockingTransferCallback((I2CSlave_Handle) arg, false);

            I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS_MASK);
            I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);
            break;
        }
        case I2C_IVR_INTCODE_ARDY:
        {
            I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_INT_ADRR_READY_ACESS);
            break;
        }
        case I2C_IVR_INTCODE_RRDY:
        {
            ptrI2CMCB->transferInProgress = 1;
            ptrI2CMCB->mode = I2CSLAVE_READ_MODE;

            /* Data read from RXBUF and next byte has already been shifted */
            if(ptrI2CMCB->count)
            {
                *(ptrI2CMCB->ptrReadBuffer) = I2CGetData(ptrHwCfg->ptrI2cBase);

                DebugP_log2("Debug: I2C Slave Driver(%p) ISR I2CSLAVE_READ_MODE Read data: 0x%x\n",
                                ptrHwCfg->ptrI2cBase, *(ptrI2CMCB->ptrReadBuffer));

                ptrI2CMCB->ptrReadBuffer++;
                ptrI2CMCB->count--;
            }
            else
            {
                ptrI2CMCB->mode = I2CSLAVE_ERROR;
                ptrI2CMCB->transferInProgress = 0;
                DebugP_log1("Debug: I2C Slave Driver(%p) ISR I2CSLAVE_READ_MODE Buffer overun\n",
                                ptrHwCfg->ptrI2cBase);

                /* Buffer over run, drop the data received */
                I2CGetData(ptrHwCfg->ptrI2cBase);

                /* Callback to application or post semaphore */
                I2CSlave_blockingTransferCallback((I2CSlave_Handle) arg, false);
            }
            I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_INT_RECV_READY);
            break;
        }
        case I2C_IVR_INTCODE_SCD:
        {
            ptrI2CMCB->mode = I2CSLAVE_IDLE_MODE;
            ptrI2CMCB->transferInProgress = 0;

            DebugP_log1("Debug: I2C Slave Driver(%p) ISR STOP DETECTED\n", ptrHwCfg->ptrI2cBase);

            /* stop condition detected, end of current transfer */
            I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);
            I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS_MASK);

            /* Callback to application or post semaphore */
            I2CSlave_blockingTransferCallback((I2CSlave_Handle) arg, true);
            break;
        }
        default:
        {
            ptrI2CMCB->mode = I2CSLAVE_ERROR;
            ptrI2CMCB->transferInProgress = 0;

            /* Callback to application or post semaphore */
            I2CSlave_blockingTransferCallback((I2CSlave_Handle) arg, false);
            break;
        }
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function sets up the read or write transfer.
 *
 *  @param[in]  handle
*      Handle to the I2C Driver
 *  @param[in]  buffer
 *     Data buffer to read data from or write data to
 *  @param[in]  size
 *      Number of bytes to  read or write
 *  @param[in]  readWriteFlag
 *      Indicate the type of operation
 *      1: Write operation
 *      0: Read operation
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static bool I2CSlave_setupTransfer(I2CSlave_Handle handle, uint8_t *buffer, size_t size, uint32_t readWriteFlag)
{
    I2CSlave_Config*    ptrI2CConfig;
    I2C_SlaveMCB*       ptrI2CMCB;
    I2C_HwCfg*          ptrHwCfg;
    bool                retVal = false;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((size == 0) || (handle == NULL) || (buffer == NULL))
    {
        goto endTransfer;
    }

    /* Get the I2C Configuration */
    ptrI2CConfig = (I2CSlave_Config*)handle;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_SlaveMCB*)ptrI2CConfig->object;

    /* Check if there is a transfer already in progress */
    if (ptrI2CMCB->transferInProgress == 1)
    {
        goto endTransfer;
    }

    /* Get the hardware configuration: */
    ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

    DebugP_log2 ("Debug: I2C Slave Driver(%p) Starting operation(%s)\n",
                    ptrHwCfg->ptrI2cBase, (readWriteFlag == 1) ? "Write" : "Read");

    /* Clear all interrupts */
    I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);

    /* Save the read/write operation parameters */
    ptrI2CMCB->count = size;
    ptrI2CMCB->ptrWriteBuffer = buffer;
    ptrI2CMCB->ptrReadBuffer = buffer;

    if (readWriteFlag == 1)
    {
        /* Enable all the WRITE interrupt flags */
        I2CIntEnable(ptrHwCfg->ptrI2cBase,
                        I2C_INT_MASK_ADRR_SLAVE | I2C_INT_MASK_STOP_CONDITION | I2C_INT_MASK_TRANSMIT_READY | I2C_INT_MASK_NO_ACK);
    }
    else
    {
        /* Enable all the READ interrupt flags */
        I2CIntEnable(ptrHwCfg->ptrI2cBase,
                        I2C_INT_MASK_ADRR_SLAVE | I2C_INT_MASK_STOP_CONDITION | I2C_INT_MASK_TRANSMIT_READY | I2C_INT_MASK_NO_ACK);
    }

    /* Check if we need to block for the transfer to complete or return immediately */
    if (ptrI2CMCB->params.transferMode == I2CSLAVE_MODE_BLOCKING)
    {
        /* Block until the transaction is complete. This will unblock on errors or sucessful completion  */
        SemaphoreP_pend(ptrI2CMCB->transferComplete, SemaphoreP_WAIT_FOREVER);
        if (ptrI2CMCB->mode == I2CSLAVE_IDLE_MODE)
        {
            DebugP_log1 ("Debug: I2C Slave Driver(%p) Transfer OK\n", ptrHwCfg->ptrI2cBase);
            retVal = true;
        }
        else
        {
            DebugP_log1 ("Debug: I2C Slave Driver(%p) Transfer ERROR\n", ptrHwCfg->ptrI2cBase);
            retVal = false;
        }
    }

endTransfer:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C Slave Driver is being passed a control command.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *  @param[in]  cmd
 *      I2C command
 *  @param[in]  arg
 *      Opaque handle to the argument
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   I2C Error code
 */
static int32_t I2C_slave_control(I2CSlave_Handle handle, uint32_t cmd, void *arg)
{
    I2CSlave_Config*    ptrI2CConfig;
    I2C_HwCfg*          ptrHwCfg;
    int32_t             errCode = 0;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (arg == NULL))
    {
        errCode = I2CSLAVE_EINVAL;
    }
    else
    {
        /* Get the I2C Configuration: */
        ptrI2CConfig = (I2CSlave_Config*)handle;

        /* Get the hardware configuration: */
        ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

        /* Processing is done on the basis of the command: */
        switch (cmd)
        {
            case I2C_CMD_ADDR_MODE:
            {
                int32_t*   expandAddr;
                /* Get the addressing mode from the argument */
                expandAddr = (int32_t*)arg;

                    /* Is XA enabled or disabled */
                if (*expandAddr == 1)
                {
                    /* Enable Expand Address bit */
                    ptrHwCfg->ptrI2cBase->ICMDR = CSL_FINSR(ptrHwCfg->ptrI2cBase->ICMDR, ICMDR_XA_BIT_END, ICMDR_XA_BIT_START, 1U);
                }
                else
                {
                    /* Disable Expand Address bit  */
                    ptrHwCfg->ptrI2cBase->ICMDR = CSL_FINSR(ptrHwCfg->ptrI2cBase->ICMDR, ICMDR_XA_BIT_END, ICMDR_XA_BIT_START, 0U);
                }
                break;
            }
            default:
            {
                /* Error: Unsuported/Invalid command specified */
                errCode = I2CSLAVE_EINVAL;
                break;
            }
        }
    }
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C Slave Driver instance is being closed.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2C_slave_close(I2CSlave_Handle handle)
{
    I2CSlave_Config*    ptrI2CConfig;
    I2C_SlaveMCB*       ptrI2CMCB;
    I2C_HwCfg*          ptrHwCfg;

    /* Sanity check */
    DebugP_assert(handle != NULL);

    /* Get the I2C Configuration */
    ptrI2CConfig = (I2CSlave_Config*)handle;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_SlaveMCB*)ptrI2CConfig->object;

    if (ptrI2CMCB != NULL)
    {

        /* Get the hardware configuration: */
        ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

        /* Disable the interrupts */
        /* Mask I2C interrupts */
        I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS_MASK);

        /* Disable the I2C Slave */
        I2CReset(ptrHwCfg->ptrI2cBase);

        /* Was the HWI registered?  */
        if (ptrI2CMCB->hwiHandle)
        {
            /* YES: Delete and unregister the interrupt handler. */
            HwiP_delete(ptrI2CMCB->hwiHandle);
        }

        /* Delete the transfer complete semaphore */
        if (ptrI2CMCB->transferComplete)
        {
            SemaphoreP_delete (ptrI2CMCB->transferComplete);
        }

        /* The driver is not in use */
        ptrI2CMCB->state = I2C_DriverState_UNINIT;

        /* Cleanup the allocated memory */
        MemoryP_ctrlFree (ptrI2CMCB, (uint32_t)sizeof(I2C_SlaveMCB));

        /* Unregister from the configuration */
        ptrI2CConfig->object = NULL;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C Slave driver is initialized.
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2C_slave_init(I2CSlave_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C Slave Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *  @param[in]  params
 *      I2C Parameters with which the driver is being opened
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
static I2CSlave_Handle I2C_slave_open(I2CSlave_Handle handle, const I2CSlave_Params* params)
{
    I2CSlave_Config*    ptrI2CConfig;
    I2C_SlaveMCB*       ptrI2CMCB;
    SemaphoreP_Params   semParams;
    HwiP_Params         hwiParams;
    I2C_HwCfg*          ptrHwCfg;
    I2CSlave_Handle     retHandle = NULL;

    /* Get the I2C Configuration: */
    ptrI2CConfig = (I2CSlave_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

    /* Ensure that the driver is NOT being used: */
    if (ptrI2CConfig->object != NULL)
    {
        /* Error: I2C Driver is already being used. */
        DebugP_log1 ("Debug: I2C Slave Driver(%p) has already been opened\n", ptrHwCfg->ptrI2cBase);
    }
    else
    {
        /* Allocate memory for the driver: */
        ptrI2CMCB = MemoryP_ctrlAlloc ((uint32_t)sizeof(I2C_SlaveMCB), 0);
        if (ptrI2CMCB == NULL)
        {
            /* Error: Out of memory */
            DebugP_log1 ("Debug: I2C Slave Driver(%p) Out of memory\n", ptrHwCfg->ptrI2cBase);
        }
        else
        {
            /* Initialize the memory: */
            memset ((void *)ptrI2CMCB, 0, sizeof(I2C_SlaveMCB));

            /* Setup the return handle: */
            retHandle = handle;
        }
    }

    /* The I2C driver requires some OS Services: Semaphores & ISR registerations are done here. */
    if (retHandle != NULL)
    {
        /* Copy over the I2C Parameters */
        memcpy ((void*)&ptrI2CMCB->params, (void *)params, sizeof(I2CSlave_Params));

        /* Create a binary semaphore which is used to handle the Blocking operation. */
        SemaphoreP_Params_init(&semParams);
        semParams.mode              = SemaphoreP_Mode_BINARY;
        ptrI2CMCB->transferComplete = SemaphoreP_create(0, &semParams);

        /* Register the Interrupt Handler: */
        HwiP_Params_init(&hwiParams);
        hwiParams.name = "I2C Slave";
        hwiParams.arg  = (uintptr_t)handle;
        ptrI2CMCB->hwiHandle = HwiP_create(ptrHwCfg->interruptNum, I2CSlave_ISR, &hwiParams);

        /* Debug Message: */
        DebugP_log2 ("Debug: I2C Slave Driver(%p) Registering HWI ISR for Interrupt %d\n",
                     ptrI2CMCB->hwiHandle, ptrHwCfg->interruptNum);


        /* Put I2C in reset state. Clocks can be configuerd in RESET state only */
        I2CReset(ptrHwCfg->ptrI2cBase);

        /* Set the I2C configuration */
        I2CSetPSC(ptrHwCfg->ptrI2cBase, 0xF);

        /* Clear any pending interrupts */
        I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);

        /* Mask off all interrupts */
        I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS_MASK);

        I2CSetOwnAddress(ptrHwCfg->ptrI2cBase, params->slaveAddress);

        /* Enable Address as Slave interrupt which is the first interrupt received in slave mode */
        I2CIntEnable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_ADRR_SLAVE);

        /* Enable the I2C Slave for operation. Set the mode to Slave receiver. */
        I2CSlaveEnable(ptrHwCfg->ptrI2cBase);

        /* Intialize the book keeping */
        ptrI2CMCB->mode = I2CSLAVE_IDLE_MODE;
        ptrI2CMCB->transferInProgress = 0;

        /* Write this back to the I2C Configuration */
        ptrI2CConfig->object = (void *)ptrI2CMCB;

        /* Mark the driver to be operational */
        ptrI2CMCB->state = I2C_DriverState_OPERATIONAL;

        /* Debug Message: */
        DebugP_log1 ("Debug: I2C Slave Driver(%p) opened in BLOCKING Mode\n", ptrHwCfg->ptrI2cBase);
    }
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked to perform a
 *      slave write.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver.
 *  @param[in]  buffer
 *      Pointer to the buffer containing data to be written to the slave.
 *  @param[in]  size
 *      The number of bytes in buffer that should be written to the slave.
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   true
 *  @retval
 *      Error   -   false
 */
bool I2C_slave_write(I2CSlave_Handle handle, const void* buffer, size_t size)
{
    return (I2CSlave_setupTransfer(handle, (uint8_t* )buffer, size, 1));
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked to perform a
 *      slave read.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver.
 *  @param[in]  buffer
 *      Pointer to an empty buffer in which the received data should be written to.
 *  @param[in]  size
 *      The number of bytes to be written into buffer.
 *
 *  \ingroup I2CSLAVE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   true
 *  @retval
 *      Error   -   false
 */
bool I2C_slave_read(I2CSlave_Handle handle, void* buffer, size_t size)
{
    return (I2CSlave_setupTransfer(handle, (uint8_t* )buffer, size, 0));
}

