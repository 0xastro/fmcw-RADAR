/**
 *   @file  i2c_master.c
 *
 *   @brief
 *      The file implements the I2C Master Driver for the XWR14xx/XWR16xx/XWR18xx/XWR68xx.
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
#include <ti/drivers/i2c/I2C.h>
#include <ti/drivers/i2c/include/i2c_master.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/
#define I2C_MODULE_OUTPUT_CLK_100KHZ    (100000U)
#define I2C_MODULE_OUTPUT_CLK_400KHZ    (400000U)

#define I2C_MODULE_INTERNAL_CLK_8MHZ    (8000000U)
#define I2C_MODULE_INTERNAL_CLK_12MHZ   (12000000U)
#define I2C_MODULE_INPUT_CLK_200MHZ     (200000000U)

/* Registered callback functions: */
static void         I2CMaster_cancel(I2C_Handle handle);
static void         I2CMaster_close(I2C_Handle handle);
static int32_t      I2CMaster_control(I2C_Handle handle, uint32_t cmd, void *arg);
static void         I2CMaster_init(I2C_Handle handle);
static I2C_Handle   I2CMaster_open(I2C_Handle handle, I2C_Params *params);
static bool         I2CMaster_transfer(I2C_Handle handle, I2C_Transaction *transaction);

/* I2C Driver API: */
static void         I2CMaster_blockingTransferCallback(I2C_Handle handle, bool transferStatus);
static void         I2CMaster_setupTransfer(I2C_MasterMCB* ptrI2CMCB, I2C_HwCfg* ptrHwCfg, I2C_Transaction *transaction);
static void         I2C_ISR (uintptr_t arg);

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   I2C Master Driver Function Table
 */
I2C_FxnTable gI2CMasterFxnTable =
{
    &I2CMaster_cancel,
    &I2CMaster_close,
    &I2CMaster_control,
    &I2CMaster_init,
    &I2CMaster_open,
    &I2CMaster_transfer
};

/**************************************************************************
 ************************* I2C Master Driver Functions ********************
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
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2CMaster_blockingTransferCallback(I2C_Handle handle, bool transferStatus)
{
    I2C_Config*         ptrI2CConfig;
    I2C_MasterMCB*      ptrI2CMCB;

    /* Get the I2C Configuration: */
    ptrI2CConfig = (I2C_Config*)handle;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_MasterMCB*)ptrI2CConfig->object;

    DebugP_log1 ("Debug: I2C Driver BLOCKING MODE - posting transfer complete semaphore. Status : %d\n", transferStatus);


    /* Post the semaphore to indicate transfer is complete */
    SemaphoreP_post(ptrI2CMCB->transferComplete);
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the I2C Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering the ISR.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2C_ISR (uintptr_t arg)
{
    I2C_Config*         ptrI2CConfig;
    I2C_MasterMCB*      ptrI2CMCB;
    I2C_HwCfg*          ptrHwCfg;
    uint32_t            status;
    uint32_t            stopCondition;
    volatile uint32_t   intCode;

    /* Get the I2C Configuration */
    ptrI2CConfig = (I2C_Config*)arg;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_MasterMCB*)ptrI2CConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

    /* Increment the ISR counter */
    intCode = I2CGetIntVector(ptrHwCfg->ptrI2cBase);
    ptrI2CMCB->interruptsRxed++;

    /* Check for I2C Errors */
    status = I2CMasterErr(ptrHwCfg->ptrI2cBase);
    if (status & I2C_INT_STOP_CONDITION)
    {
        if ((ptrI2CMCB->writeCount == 0U) && (ptrI2CMCB->readCount == 0U))
        {
            /* End of transfer stop condition, not an error */
            stopCondition = I2C_INT_STOP_CONDITION;
            status &= (uint32_t)(~stopCondition);
        }
    }

    if ((status == 0) || (ptrI2CMCB->mode == I2C_ERROR))
    {
        switch (ptrI2CMCB->mode)
        {
            /*
             * ERROR case is OK because if an Error is detected, a STOP bit is
             * sent; which in turn will call another interrupt. This interrupt
             * call will then post the transferComplete semaphore to unblock the
             * I2C_transfer function
             */
            case I2C_ERROR:
            case I2C_IDLE_MODE:
            {
                DebugP_log1("Debug: I2C Driver(%p) ISR I2C_IDLE_MODE\n", ptrHwCfg->ptrI2cBase);

                /* callback to application or post Semaphore to unblock transfer fxn */
                ptrI2CMCB->currentTransaction->readCount -= ptrI2CMCB->readCount;
                ptrI2CMCB->currentTransaction->writeCount -= ptrI2CMCB->writeCount;
                ptrI2CMCB->currentTransaction = NULL;

                I2CMaster_blockingTransferCallback((I2C_Handle)arg,
                    (ptrI2CMCB->mode == I2C_ERROR) ? 0 : 1);

                /* Clear STOP condition interrupt */
                I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_INT_STOP_CONDITION);

                /* Disable STOP condition interrupt */
                I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_STOP_CONDITION);
                break;
            }
            case I2C_WRITE_MODE:
            {
                /* Check if data needs to be sent */
                if (ptrI2CMCB->writeCount)
                {
                    /* Write data into transmit register */
                    I2CPutData(ptrHwCfg->ptrI2cBase, *(ptrI2CMCB->ptrWriteBuffer));
                    DebugP_log2("Debug: I2C Driver(%p) ISR I2C_WRITE_MODE Write data: 0x%x\n",
                                ptrHwCfg->ptrI2cBase, *(ptrI2CMCB->ptrWriteBuffer));

                    (ptrI2CMCB->ptrWriteBuffer)++;
                    ptrI2CMCB->writeCount--;

                    if (ptrI2CMCB->writeCount == 0U)
                    {
                        /* End of write, disable Tx ready interrupt */
                        I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_TRANSMIT_READY);
                        if (ptrI2CMCB->readCount)
                        {
                            /* Get register access interrupt to start reading */
                            I2CIntEnable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_ADRR_READY_ACESS);
                        }
                        else
                        {
                            /* Done with all transmissions, wait for stop condition */
                            ptrI2CMCB->mode = I2C_IDLE_MODE;
                        }
                    }
                }
                else
                {
                    if (ptrI2CMCB->readCount)
                    {
                        /* Next state: Receive mode */
                        ptrI2CMCB->mode = I2C_READ_MODE;
                        I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_ADRR_READY_ACESS);

                        /* Set number of bytes to receive */
                        I2CSetDataCount(ptrHwCfg->ptrI2cBase, ptrI2CMCB->readCount);

                        /* Configure peripheral for I2C Receive mode with stop */
                        I2CMasterControl(ptrHwCfg->ptrI2cBase,
                                            I2C_CFG_MASK_RX | I2C_CFG_MASK_REPEAT_MODE,
                                            I2C_CFG_CMD_RX | I2C_CFG_CMD_REPEAT_MODE_OFF);

                        /* Enable RX interrupt to handle data received */
                        I2CIntEnable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_RECV_READY);

                        /* Start I2C peripheral in Rx mode */
                        I2CMasterStart(ptrHwCfg->ptrI2cBase);
                    }
                }
                break;
            }
            case I2C_READ_MODE:
            {
                /* Save the received data */
                *(ptrI2CMCB->ptrReadBuffer) = I2CGetData(ptrHwCfg->ptrI2cBase);

                DebugP_log2("Debug: I2C Driver(%p) ISR I2C_READ_MODE Read data: 0x%x\n",
                                ptrHwCfg->ptrI2cBase, *(ptrI2CMCB->ptrReadBuffer));

                ptrI2CMCB->ptrReadBuffer++;
                ptrI2CMCB->readCount--;

                if (ptrI2CMCB->readCount == 0U)
                {
                    /* No more data to receive, Next state: Idle mode */
                    ptrI2CMCB->mode = I2C_IDLE_MODE;

                    /* Disable RX interrupt, next interrupt will be from STOP */
                    I2CIntDisable(ptrHwCfg->ptrI2cBase,
                                          I2C_INT_MASK_RECV_READY);
                    /* Send stop */
                    I2CMasterStop(ptrHwCfg->ptrI2cBase);
                }
                break;
            }
            default:
            {
                ptrI2CMCB->mode = I2C_ERROR;
                break;
            }
        }
    }
    else
    {
        /* Error condition */
        ptrI2CMCB->mode = I2C_ERROR;

        /* Send a STOP bit to end all I2C communications immediately */
        I2CMasterStop(ptrHwCfg->ptrI2cBase);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function sets up a transfer to write or read or write followed by
 *      read transaction with a I2C slave peripheral.
 *
 *  @param[in]  ptrI2CMCB
 *      Pointer to the I2C driver master control block.
 *  @param[in]  ptrHwCfg
 *      Pointer to the hardware configuration
 *  @param[in]  transaction
 *      Pointer to the transaction
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2CMaster_setupTransfer(I2C_MasterMCB* ptrI2CMCB, I2C_HwCfg* ptrHwCfg, I2C_Transaction *transaction)
{
    /* Clear all interrupts */
    I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);

    /* Mark the transaction as current */
    ptrI2CMCB->currentTransaction = transaction;

    /* Update the read/write buffer pointers and counts */
    ptrI2CMCB->ptrWriteBuffer = transaction->writeBuf;
    ptrI2CMCB->writeCount = transaction->writeCount;

    ptrI2CMCB->ptrReadBuffer = transaction->readBuf;
    ptrI2CMCB->readCount = transaction->readCount;

    DebugP_log3 ("Debug: I2C Driver(%p) Starting transaction(%p) to Slave: 0x%x\n",
                    ptrHwCfg->ptrI2cBase, transaction, transaction->slaveAddress);

    /* Setup the slave address */
    I2CMasterSetSlaveAddr(ptrHwCfg->ptrI2cBase, ptrI2CMCB->currentTransaction->slaveAddress);

    /* Enable interrupts to detect STOP and ERROR conditions */
    I2CIntEnable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_STOP_CONDITION | I2C_INT_MASK_ARBITRATION_LOST | I2C_INT_MASK_NO_ACK);

    /* Perform a write transfer before a read transfer */
    if (ptrI2CMCB->writeCount != 0)
    {
        /* Start the master in transmit mode */
        ptrI2CMCB->mode = I2C_WRITE_MODE;

        /* Set number of bytes to transmit */
        I2CSetDataCount(ptrHwCfg->ptrI2cBase, ptrI2CMCB->writeCount);

        /* Set master direction as Tx and turn OFF repeat mode */
        I2CMasterControl(ptrHwCfg->ptrI2cBase, I2C_CFG_MASK_TX | I2C_CFG_MASK_REPEAT_MODE,
                                 I2C_CFG_CMD_TX | I2C_CFG_CMD_REPEAT_MODE_OFF);

        DebugP_log2 ("Debug: I2C Driver(%p) MODE transition from IDLE -> WRITE. Writing %d bytes\n",
                    ptrHwCfg->ptrI2cBase, ptrI2CMCB->writeCount);

        /* Finally Enable transmit interrupt. */
        I2CIntEnable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_TRANSMIT_READY);

        /* Send STOP if no read has to be performed post the write transaction */
        if (ptrI2CMCB->readCount == 0)
        {
            I2CMasterControl(ptrHwCfg->ptrI2cBase, I2C_CFG_MASK_STOP, I2C_CFG_CMD_STOP);
        }

        /* Start the I2C transfer in master transmit mode */
        I2CMasterStart(ptrHwCfg->ptrI2cBase);

    }
    else
    {
        /* Start the master in receive mode */
        ptrI2CMCB->mode = I2C_READ_MODE;

        /* Set number of bytes to transmit */
        I2CSetDataCount(ptrHwCfg->ptrI2cBase, ptrI2CMCB->readCount);

        /* Set master direction as Rx and turn OFF repeat mode */
        I2CMasterControl(ptrHwCfg->ptrI2cBase, I2C_CFG_MASK_RX | I2C_CFG_MASK_REPEAT_MODE,
                                 I2C_CFG_CMD_RX | I2C_CFG_CMD_REPEAT_MODE_OFF);

        DebugP_log2 ("Debug: I2C Driver(%p) MODE transition from IDLE -> READ. Writing %d bytes\n",
                    ptrHwCfg->ptrI2cBase, ptrI2CMCB->writeCount);

        /* Finally Enable receive interrupt. */
        I2CIntEnable(ptrHwCfg->ptrI2cBase, I2C_INT_MASK_RECV_READY);

        /* Send start bit */
        I2CMasterStart(ptrHwCfg->ptrI2cBase);
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C Driver is being passed a control command.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *  @param[in]  cmd
 *      I2C command
 *  @param[in]  arg
 *      Opaque handle to the argument
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   I2C Error code
 */
static int32_t I2CMaster_control(I2C_Handle handle, uint32_t cmd, void *arg)
{
    I2C_Config*         ptrI2CConfig;
    I2C_HwCfg*          ptrHwCfg;
    int32_t             errCode;

    /* Initialize and setup the error code: */
    errCode = 0;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (arg == NULL))
    {
        errCode = I2C_EINVAL;
    }
    else
    {
        /* Get the I2C Configuration: */
        ptrI2CConfig = (I2C_Config*)handle;

        /* Get the hardware configuration: */
        ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

        /* Processing is done on the basis of the command: */
        switch (cmd)
        {
            case I2C_CMD_LOOPBACK:
            {
                int32_t*   loopback;

                /* Get the loopback status from the argument */
                loopback = (int32_t*)arg;

                /* Do we need to enable/disable loopback */
                if (*loopback == 1)
                {
                    /* Enable Digital Loopback */
                    ptrHwCfg->ptrI2cBase->ICMDR = CSL_FINSR(ptrHwCfg->ptrI2cBase->ICMDR, ICMDR_DLB_BIT_END, ICMDR_DLB_BIT_START, 1U);

                    /* Ignore NACK */
                    /*ptrHwCfg->ptrI2cBase->ICEMDR = CSL_FINSR(ptrHwCfg->ptrI2cBase->ICEMDR, ICEMDR_IGNACK_BIT_END, ICEMDR_IGNACK_BIT_START, 1U); */
                }
                else
                {
                    /* Disable Digital Loopback */
                    ptrHwCfg->ptrI2cBase->ICMDR = CSL_FINSR(ptrHwCfg->ptrI2cBase->ICMDR, ICMDR_DLB_BIT_END, ICMDR_DLB_BIT_START, 0U);

                    /* Enable NACK processing */
                    ptrHwCfg->ptrI2cBase->ICEMDR = CSL_FINSR(ptrHwCfg->ptrI2cBase->ICEMDR, ICEMDR_IGNACK_BIT_END, ICEMDR_IGNACK_BIT_START, 0U);

                }
                break;
            }
            case I2C_CMD_ADDR_MODE:
            {
                uint32_t*   expandAddr;

                /* Get the addressing mode from the argument */
                expandAddr = (uint32_t*)arg;

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
            case I2C_CMD_I2COAR:
            {
                uint_fast16_t*   addr;

                /* Get the addressing mode from the argument */
                addr = (uint32_t*)arg;

                /* Set the address */
                I2CSetOwnAddress(ptrHwCfg->ptrI2cBase, *addr);
                break;
            }
            default:
            {
                /* Error: Unsuported/Invalid command specified */
                errCode = I2C_EINVAL;
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
 *      all pending I2C data transfers have to be canceled.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2CMaster_cancel(I2C_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C Driver instance is being closed
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2CMaster_close(I2C_Handle handle)
{
    I2C_Config*         ptrI2CConfig;
    I2C_MasterMCB*      ptrI2CMCB;
    I2C_HwCfg*          ptrHwCfg;

    /* Sanity check */
    DebugP_assert(handle != NULL);

    /* Get the I2C Configuration */
    ptrI2CConfig = (I2C_Config*)handle;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_MasterMCB*)ptrI2CConfig->object;

    if (ptrI2CMCB != NULL)
    {
        /* Get the hardware configuration: */
        ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

        /* Disable the interrupts */
        /* Mask I2C interrupts */
        I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS_MASK);

        /* Disable the I2C Master */
        I2CReset(ptrHwCfg->ptrI2cBase);

        /* Was the HWI registered?  */
        if (ptrI2CMCB->hwiHandle)
        {
            /* Delete and unregister the interrupt handler. */
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
        MemoryP_ctrlFree (ptrI2CMCB, (uint32_t)sizeof(I2C_MasterMCB));

        /* Unregister from the configuration */
        ptrI2CConfig->object = NULL;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C driver is initialized.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver which is to be initialized.
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void I2CMaster_init(I2C_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the I2C Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *  @param[in]  params
 *      I2C Parameters with which the driver is being opened
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
static I2C_Handle I2CMaster_open(I2C_Handle handle, I2C_Params* params)
{
    I2C_Config*         ptrI2CConfig;
    I2C_MasterMCB*      ptrI2CMCB;
    SemaphoreP_Params   semParams;
    HwiP_Params         hwiParams;
    I2C_HwCfg*          ptrHwCfg;
    I2C_Handle          retHandle = NULL;
    uint32_t            outputClk, internalClk;

    /* Get the I2C Configuration: */
    ptrI2CConfig = (I2C_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

    /* Ensure that the driver is NOT being used: */
    if (ptrI2CConfig->object != NULL)
    {
        /* Error: I2C Driver is already being used. */
        DebugP_log1 ("Debug: I2C Driver(%p) has already been opened\n", ptrHwCfg->ptrI2cBase);
    }
    else
    {
        /* Allocate memory for the driver: */
        ptrI2CMCB = MemoryP_ctrlAlloc ((uint32_t)sizeof(I2C_MasterMCB), 0);
        if (ptrI2CMCB == NULL)
        {
            /* Error: Out of memory */
            DebugP_log1 ("Debug: I2C Driver(%p) Out of memory\n", ptrHwCfg->ptrI2cBase);
        }
        else
        {
            /* Initialize the memory: */
            memset ((void *)ptrI2CMCB, 0, sizeof(I2C_MasterMCB));

            /* Setup the return handle: */
            retHandle = handle;
        }
    }

    /* The I2C driver requires some OS Services: Semaphores & ISR registerations are done here. */
    if (retHandle != NULL)
    {
        /* Copy over the I2C Parameters */
        memcpy ((void*)&ptrI2CMCB->params, (void *)params, sizeof(I2C_Params));

        /* Create a binary semaphore which is used to handle the Blocking operation. */
        SemaphoreP_Params_init(&semParams);
        semParams.mode              = SemaphoreP_Mode_BINARY;
        ptrI2CMCB->transferComplete = SemaphoreP_create(0, &semParams);

        /* Register the Interrupt Handler: */
        HwiP_Params_init(&hwiParams);
        hwiParams.name = "I2C Master";
        hwiParams.arg  = (uintptr_t)handle;
        ptrI2CMCB->hwiHandle = HwiP_create(ptrHwCfg->interruptNum, I2C_ISR, &hwiParams);

        /* Debug Message: */
        DebugP_log2 ("Debug: I2C Driver(%p) Registering HWI ISR for Interrupt %d\n",
                     ptrI2CMCB->hwiHandle, ptrHwCfg->interruptNum);

        /* Put I2C in reset state. Clocks can be configured in RESET state only */
        I2CReset(ptrHwCfg->ptrI2cBase);

        /* Compute the clock based on bit rate */
        switch(ptrI2CMCB->params.bitRate)
        {
            case I2C_100kHz:
            {
                outputClk = I2C_MODULE_OUTPUT_CLK_100KHZ;
                internalClk = I2C_MODULE_INTERNAL_CLK_8MHZ;
                break;
            }
            case I2C_400kHz:
            {
                outputClk = I2C_MODULE_OUTPUT_CLK_400KHZ;
                internalClk = I2C_MODULE_INTERNAL_CLK_12MHZ;
                break;
            }
            default:
            {
                /* Default case force it to 100 KHz bit rate */
                outputClk = I2C_MODULE_OUTPUT_CLK_100KHZ;
                internalClk = I2C_MODULE_INTERNAL_CLK_8MHZ;
                break;
             }
        }

        /* Set the I2C configuration */
        I2CMasterInitClk(ptrHwCfg->ptrI2cBase,
                                I2C_MODULE_INPUT_CLK_200MHZ,
                                internalClk,
                                outputClk);

        /* Clear any pending interrupts */
        I2CIntClear(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS);

        /* Mask off all interrupts */
        I2CIntDisable(ptrHwCfg->ptrI2cBase, I2C_ALL_INTS_MASK);

        /* Enable the I2C Master for operation */
        I2CMasterEnable(ptrHwCfg->ptrI2cBase);

        /* Intialize the book keeping */
        ptrI2CMCB->mode = I2C_IDLE_MODE;
        ptrI2CMCB->currentTransaction = NULL;

        /* Write this back to the I2C Configuration */
        ptrI2CConfig->object = (void *)ptrI2CMCB;

        /* Mark the driver to be operational */
        ptrI2CMCB->state = I2C_DriverState_OPERATIONAL;

        /* Debug Message: */
        DebugP_log1 ("Debug: I2C Driver(%p) opened in BLOCKING Mode\n", ptrHwCfg->ptrI2cBase);
    }
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked to perform a
 *      write or read or write followed by read transaction with a I2C slave peripheral.
 *
 *  @param[in]  handle
 *      Handle to the I2C Driver
 *  @param[in]  transaction
 *      Pointer to the transaction
 *
 *  \ingroup I2C_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   true
 *  @retval
 *      Error   -   false
 */
static bool I2CMaster_transfer(I2C_Handle handle, I2C_Transaction *transaction)
{
    I2C_Config*         ptrI2CConfig;
    I2C_MasterMCB*      ptrI2CMCB;
    I2C_HwCfg*          ptrHwCfg;
    bool                retVal = false;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (transaction == NULL))
    {
        goto endTransfer;
    }

    if ((transaction->writeCount == 0) && (transaction->readCount == 0))
    {
        goto endTransfer;
    }

    /* Get the I2C Configuration */
    ptrI2CConfig = (I2C_Config*)handle;

    /* Get the I2C Driver Object */
    ptrI2CMCB = (I2C_MasterMCB*)ptrI2CConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (I2C_HwCfg*)ptrI2CConfig->hwAttrs;

    /* Disable I2C interrupt to prevent I2C interrupt when the transaction is being setup */
    HwiP_disableInterrupt(ptrHwCfg->interruptNum);
    I2CMaster_setupTransfer(ptrI2CMCB, ptrHwCfg, transaction);
    HwiP_enableInterrupt(ptrHwCfg->interruptNum);

    /* Check if we need to block for the transfer to complete or return immediately */
    if (ptrI2CMCB->params.transferMode == I2C_MODE_BLOCKING)
    {
        /* Block until the transaction is complete. This will unblock on errors or sucessful completion  */
        SemaphoreP_pend(ptrI2CMCB->transferComplete, SemaphoreP_WAIT_FOREVER);
        if (ptrI2CMCB->mode == I2C_IDLE_MODE)
        {
            DebugP_log1 ("Debug: I2C Driver(%p) Transfer OK\n", ptrHwCfg->ptrI2cBase);
            retVal = true;
        }
        else
        {
            DebugP_log1 ("Debug: I2C Driver(%p) Transfer ERROR\n", ptrHwCfg->ptrI2cBase);
            retVal = false;
        }
    }

endTransfer:
    return retVal;
}

