/**
 *   @file  canfd.c
 *
 *   @brief
 *      The file implements the Controller Area Network Driver Flexible data
 *      for XWR16xx/XWR18xx/XWR68xx.
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
#include <stdio.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/common/hw_types.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/canfd/include/canfd_internal.h>

static void CANFD_processFIFOElements(CANFD_DriverMCB* ptrCanFdMCB, MCAN_RxFIFONum fifoNum);
static void CANFD_MCANInt0Isr (uintptr_t arg);
static void CANFD_MCANInt1Isr (uintptr_t arg);
static void CANFD_configMessageRAM(const CANFD_DriverMCB* ptrCanFdMCB, const CANFD_MCANMsgRAMCfgParams* configParams, int32_t* errCode);

/**
 *  @b Description
 *  @n
 *      Function processes the Rx FIFO and calls application callback function to receive data.
 *
 *  @param[in]  ptrCanFdMCB
 *      Pointer to the CANFD driver MCB
 *  @param[in]  fifoNum
 *      Rx FiFO number
 *
 *  @retval
 *      Success -   Handle to the CAN Driver
 *  @retval
 *      Error   -   NULL
 */

static void CANFD_processFIFOElements(CANFD_DriverMCB* ptrCanFdMCB, MCAN_RxFIFONum fifoNum)
{
    MCAN_RxFIFOStatus       fifoStatus;
    uint32_t                fillLevel;
    uint32_t                index;
    CANFD_MessageObject*    ptrCanMsgObj;
    uint32_t                baseAddr;

    baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

    /* Get the FIFO status */
    fifoStatus.num = (uint32_t)fifoNum;
    MCAN_getRxFIFOStatus(baseAddr, &fifoStatus);
    fillLevel = fifoStatus.fillLvl;

    for(index = 0; index < fillLevel; index++)
    {
        MCAN_readMsgRam(baseAddr, MCAN_MemType_FIFO, fifoStatus.getIdx, (uint32_t)fifoNum, &ptrCanFdMCB->rxBuffElem);

        /* Get the message object pointer */
        ptrCanMsgObj = ptrCanFdMCB->rxMapping[ptrCanFdMCB->rxBuffElem.fidx];

        /* Increment the number of interrupts received */
        ptrCanMsgObj->interruptsRxed++;

        /* Call the registered callback. */
        ptrCanFdMCB->appDataCallBack((CANFD_MsgObjHandle)ptrCanMsgObj, CANFD_Reason_RX);

        /* Acknowledge the data read */
        MCAN_writeRxFIFOAck(baseAddr, (uint32_t)fifoNum, fifoStatus.getIdx);

        MCAN_getRxFIFOStatus(baseAddr, &fifoStatus);
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered interrupt 0 ISR for the CANFD Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering the ISR
 *
 *  \ingroup CANFD_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CANFD_MCANInt0Isr (uintptr_t arg)
{
    CANFD_MessageObject*    ptrCanMsgObj;
    CANFD_DriverMCB*        ptrCanFdMCB;
    uint32_t                baseAddr;
    uint32_t                intrStatus;
    uint32_t                index, status, buffIndex;
    MCAN_RxNewDataStatus    newDataStatus;

    /* Get the pointer to the CAN Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)arg;

    /* Increment the number of interrupts received */
    ptrCanFdMCB->interrupts++;
    baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

    intrStatus = MCAN_getIntrStatus(baseAddr);
    MCAN_clearIntrStatus(baseAddr, intrStatus);

    /* Process Bus-Off condition */
    if ((intrStatus & MCAN_INTR_SRC_BUS_OFF_STATUS) == MCAN_INTR_SRC_BUS_OFF_STATUS)
    {
        /* Increment the number of interrupts received */
        ptrCanFdMCB->busOffInterrupts++;

        ptrCanFdMCB->state = CANFD_DriverState_STOPPED;

        /* Call the registered callback. */
        if (ptrCanFdMCB->appErrCallBack != NULL)
        {
            ptrCanFdMCB->appErrCallBack((CANFD_Handle)ptrCanFdMCB, CANFD_Reason_BUSOFF, NULL);
        }
    }

    /* Process Protocol error in data phase condition */
    if ((intrStatus & MCAN_INTR_SRC_PROTOCOL_ERR_DATA) == MCAN_INTR_SRC_PROTOCOL_ERR_DATA)
    {
        /* Increment the number of interrupts received */
        ptrCanFdMCB->protoDataErrInterrupts++;

        /* Call the registered callback. */
        if (ptrCanFdMCB->appErrCallBack != NULL)
        {
            ptrCanFdMCB->appErrCallBack((CANFD_Handle)ptrCanFdMCB, CANFD_Reason_PROTOCOL_ERR_DATA_PHASE, NULL);
        }
    }

    /* Process Protocol error in arbitration phase condition */
    if ((intrStatus & MCAN_INTR_SRC_PROTOCOL_ERR_ARB) == MCAN_INTR_SRC_PROTOCOL_ERR_ARB)
    {
        /* Increment the number of interrupts received */
        ptrCanFdMCB->protoArbErrInterrupts++;

        /* Call the registered callback. */
        if (ptrCanFdMCB->appErrCallBack != NULL)
        {
            ptrCanFdMCB->appErrCallBack((CANFD_Handle)ptrCanFdMCB, CANFD_Reason_PROTOCOL_ERR_ARB_PHASE, NULL);
        }
    }

    /* Process Transmit complete interrrupt */
    if ((intrStatus & MCAN_INTR_SRC_TRANS_COMPLETE) == MCAN_INTR_SRC_TRANS_COMPLETE)
    {
        status = MCAN_getTxBufTransmissionStatus(baseAddr);

        /* Process all 32 Tx buffers */
        for(index = 0; index < MCAN_MAX_TX_BUFFERS; index++)
        {
            buffIndex = ((uint32_t)1U << index);
            if(buffIndex == (status & buffIndex))
            {
                /* Get the message object pointer */
                ptrCanMsgObj = ptrCanFdMCB->txMapping[index];

                /* Increment the number of interrupts received */
                ptrCanMsgObj->interruptsRxed++;

                if(ptrCanFdMCB->txStatus[ptrCanMsgObj->txElement] == 1)
                {
                    ptrCanFdMCB->txStatus[ptrCanMsgObj->txElement] = 0;

                    /* Call the registered callback. */
                    ptrCanFdMCB->appDataCallBack((CANFD_MsgObjHandle)ptrCanMsgObj, CANFD_Reason_TX_COMPLETION);
                }
            }
            status = (status & ~buffIndex);
            if (status == 0)
            {
                break;
            }
        }
    }

    /* Process Receive buffer interrupt */
    if ((intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG) == MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG)
    {
        /* Get the new data status */
        MCAN_getNewDataStatus(baseAddr, &newDataStatus);

        /* Clear NewData status to accept new messages */
        MCAN_clearNewDataStatus(baseAddr, &newDataStatus);

        /* Process the low 32 buffers */
        status = newDataStatus.statusLow;
        index = 0;
        while (status != 0)
        {
            if ((status & 1U) == 1U)
            {
                /* Get the message object pointer */
                ptrCanMsgObj = ptrCanFdMCB->rxMapping[index];

                /* Increment the number of interrupts received */
                ptrCanMsgObj->interruptsRxed++;

                /* Read the pending data */
                MCAN_readMsgRam(baseAddr, ptrCanMsgObj->memType, ptrCanMsgObj->rxElement, 0, &ptrCanFdMCB->rxBuffElem);

                /* Call the registered callback. */
                ptrCanFdMCB->appDataCallBack((CANFD_MsgObjHandle)ptrCanMsgObj, CANFD_Reason_RX);
            }
            index++;
            status = (status >> 1U);
        }
    }

    /* Process Receive FIFO interrupts */
    if ((intrStatus & MCAN_INTR_SRC_RX_FIFO0_NEW_MSG) == MCAN_INTR_SRC_RX_FIFO0_NEW_MSG)
    {
        /* Process FIFO 0 */
        CANFD_processFIFOElements(ptrCanFdMCB, MCAN_RxFIFONum_0);
    }

    if ((intrStatus & MCAN_INTR_SRC_RX_FIFO1_NEW_MSG) == MCAN_INTR_SRC_RX_FIFO1_NEW_MSG)
    {
        /* Process FIFO 1 */
        CANFD_processFIFOElements(ptrCanFdMCB, MCAN_RxFIFONum_1);
    }

    /* Process Transmit cancel interrrupt */
    if ((intrStatus & MCAN_INTR_SRC_TRANS_CANCEL_FINISH) == MCAN_INTR_SRC_TRANS_CANCEL_FINISH)
    {
        status = MCAN_txBufCancellationStatus(baseAddr);

        /* Process all 32 Tx buffers */
        for(index = 0; index < MCAN_MAX_TX_BUFFERS; index++)
        {
            buffIndex = ((uint32_t)1U << index);
            if(buffIndex == (status & buffIndex))
            {
                /* Get the message object pointer */
                ptrCanMsgObj = ptrCanFdMCB->txMapping[index];

                /* Increment the number of interrupts received */
                ptrCanMsgObj->interruptsRxed++;

                if(ptrCanFdMCB->txStatus[ptrCanMsgObj->txElement] == 1)
                {
                    ptrCanFdMCB->txStatus[ptrCanMsgObj->txElement] = 0;

                    /* Call the registered callback. */
                    ptrCanFdMCB->appDataCallBack((CANFD_MsgObjHandle)ptrCanMsgObj, CANFD_Reason_TX_CANCELED);
                }
            }
            status = (status & ~buffIndex);
            if (status == 0)
            {
                break;
            }
        }
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered interrupt 1 ISR for the CANFD Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering the ISR
 *
 *  \ingroup CANFD_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CANFD_MCANInt1Isr (uintptr_t arg)
{
    CANFD_DriverMCB*    ptrCanFdMCB;
    uint32_t            baseAddr;
    CANFD_ErrStatusResp errStatusResp;

    /* Get the pointer to the CAN Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)arg;
    baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

    /* Increment the number of interrupts received */
    ptrCanFdMCB->eccInterrupts++;

    /* Read ECC error status */
    MCAN_eccGetErrorStatus(baseAddr, &errStatusResp.u.eccErrStatus);
    if (errStatusResp.u.eccErrStatus.secErr == 1U)
    {
        MCAN_eccClearErrorStatus(baseAddr, CANFD_MCANECCErrType_SEC);
    }
    if (errStatusResp.u.eccErrStatus.dedErr == 1U)
    {
        MCAN_eccClearErrorStatus(baseAddr, CANFD_MCANECCErrType_DED);
    }

    /* Call the registered callback. */
    if (ptrCanFdMCB->appErrCallBack != NULL)
    {
        ptrCanFdMCB->appErrCallBack((CANFD_Handle)ptrCanFdMCB, CANFD_Reason_ECC_ERROR, &errStatusResp);
    }

    MCAN_eccWriteEOI(baseAddr, CANFD_MCANECCErrType_SEC);
    MCAN_eccWriteEOI(baseAddr, CANFD_MCANECCErrType_DED);

    return;
}

/**
 *  @b Description
 *  @n
 *      Function initializes the CANFD driver instance with the specified hardware attributes.
 *      It resets and configures the MCAN module, sets up the Message RAM and ECC Aggregator.
 *      It configures the CANFD driver with the control parameters.
 *
 *  @param[in]  ptrCanFdMCB
 *      Pointer to the CANFD driver MCB
 *  @param[in]  configParams
 *      CAN module configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the CAN Driver
 *  @retval
 *      Error   -   NULL
 */

static void CANFD_configMessageRAM(const CANFD_DriverMCB* ptrCanFdMCB, const CANFD_MCANMsgRAMCfgParams* configParams, int32_t* errCode)
{
    MCAN_MsgRAMConfigParams msgRAMConfigParams;
    uint32_t                startAddr;

    /* Compute the start Address and populate the Message RAM configuration parameters */
    startAddr = 0;

    /* 11-bit filter configuration */
    msgRAMConfigParams.flssa = startAddr;
    msgRAMConfigParams.lss = configParams->lss;

    /* 29-bit filter configuration */
    startAddr += ((configParams->lss + 1U) * MCAN_MSG_RAM_STD_ELEM_SIZE * 4U);
    msgRAMConfigParams.flesa = startAddr;
    msgRAMConfigParams.lse = configParams->lse;

    /* Tx buffer configuration */
    startAddr += ((configParams->lse + 1U) * MCAN_MSG_RAM_EXT_ELEM_SIZE * 4U);
    msgRAMConfigParams.txStartAddr = startAddr;
    msgRAMConfigParams.txBufNum = configParams->txBufNum;
    msgRAMConfigParams.txFIFOSize = configParams->txFIFOSize;
    msgRAMConfigParams.txBufMode = 1U;
    msgRAMConfigParams.txBufElemSize = CANFD_MCANElemSize_64BYTES;

    /* Tx Event FIFO configuration */
    msgRAMConfigParams.txEventFIFOStartAddr = 0;
    msgRAMConfigParams.txEventFIFOSize = 0;
    msgRAMConfigParams.txEventFIFOWaterMark = 0;

    /* Rx FIFO 0 configuration */
    startAddr += ((configParams->txBufNum + 1U) * MCAN_MSG_RAM_TX_RX_ELEM_SIZE * 4U);
    startAddr += ((configParams->txFIFOSize + 1U) * MCAN_MSG_RAM_TX_RX_ELEM_SIZE * 4U);
    msgRAMConfigParams.rxFIFO0startAddr = startAddr;
    msgRAMConfigParams.rxFIFO0size = configParams->rxFIFO0size;
    msgRAMConfigParams.rxFIFO0waterMark = 0;
    msgRAMConfigParams.rxFIFO0ElemSize = CANFD_MCANElemSize_64BYTES;
    msgRAMConfigParams.rxFIFO0OpMode = configParams->rxFIFO0OpMode;

    /* Rx FIFO 1 configuration */
    startAddr += ((configParams->rxFIFO0size + 1U) * MCAN_MSG_RAM_TX_RX_ELEM_SIZE * 4U);
    msgRAMConfigParams.rxFIFO1startAddr = startAddr;
    msgRAMConfigParams.rxFIFO1size = configParams->rxFIFO1size;
    msgRAMConfigParams.rxFIFO1waterMark = 0;
    msgRAMConfigParams.rxFIFO1ElemSize = CANFD_MCANElemSize_64BYTES;
    msgRAMConfigParams.rxFIFO1OpMode = configParams->rxFIFO1OpMode;

    /* Rx Buffer configuration */
    startAddr += ((configParams->rxFIFO1size + 1U) * MCAN_MSG_RAM_TX_RX_ELEM_SIZE * 4U);
    msgRAMConfigParams.rxBufStartAddr = startAddr;
    msgRAMConfigParams.rxBufElemSize = CANFD_MCANElemSize_64BYTES;

    /* Configure Message RAM */
    *errCode = MCAN_msgRAMConfig(ptrCanFdMCB->hwCfg.regBaseAddress, &msgRAMConfigParams);
}

/**
 *  @b Description
 *  @n
 *      Function initializes the CANFD driver instance with the specified hardware attributes.
 *      It resets and configures the MCAN module, sets up the Message RAM and ECC Aggregator.
 *      It configures the CANFD driver with the control parameters.
 *
 *  @param[in]  configParams
 *      CANFD module configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the CANFD Driver
 *  @retval
 *      Error   -   NULL
 */

CANFD_Handle CANFD_init(const CANFD_MCANInitParams* configParams, int32_t* errCode)
{
    CANFD_DriverMCB*        ptrCanFdMCB;
    HwiP_Params             hwiParams;
    uint32_t                baseAddr;
    MCAN_RxNewDataStatus    newDataStatus;
    volatile uint32_t       status;

    *errCode = CANFD_EOK;

    if ((configParams == NULL) || (configParams->appDataCallBack == NULL))
    {
        *errCode = CANFD_EINVAL;
        ptrCanFdMCB = NULL;
    }
    else
    {
        /* Allocate memory for the CANFD Driver */
        ptrCanFdMCB = MemoryP_ctrlAlloc (sizeof(CANFD_DriverMCB), 0U);
        if (ptrCanFdMCB == NULL)
        {
            /* Error: Unable to allocate the memory */
            *errCode = CANFD_ENOMEM;
            ptrCanFdMCB = NULL;
        }
        else
        {
            /* Initialize the allocated memory */
            memset ((void *)ptrCanFdMCB, 0, sizeof(CANFD_DriverMCB));

            /* Initialize the CANFD driver state */
            ptrCanFdMCB->state = CANFD_DriverState_UNINIT;

            /* Copy over the configuration */
            memcpy ((void *)&ptrCanFdMCB->hwCfg, (void*)&gCanFdHwCfg, sizeof(CANFD_HwCfg));

            baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

            /* Check if Message RAM initialization is done or not TODO add timeout */
            do
            {
                status = MCAN_isMemInitDone(baseAddr);
                if (status == 1U)
                {
                    break;
                }
            } while (1);

            /* Reset the MCAN module. */
            MCAN_reset(baseAddr);

            /* Wait while MCAN is in reset. TODO add timeout */
            do
            {
            } while (MCAN_isInReset(baseAddr) == 1U);

            /* Put MCAN in SW initialization mode */
            MCAN_setOpMode(baseAddr, CANFD_MCANOperationMode_SW_INIT);

            /* Wait while MCAN is not in init mode. TODO add timeout */
            do
            {
            } while (MCAN_getOpMode(baseAddr) != CANFD_MCANOperationMode_SW_INIT);

            /* Initialize the MCAN module */
            *errCode  = MCAN_init (baseAddr, configParams);
            if (*errCode != CANFD_EOK)
            {
                MemoryP_ctrlFree (ptrCanFdMCB, sizeof(CANFD_DriverMCB));
                ptrCanFdMCB = NULL;
            }

            /* Configure the MCAN module */
            if (*errCode == CANFD_EOK)
            {
                *errCode  = MCAN_config (baseAddr, configParams);
                if (*errCode != CANFD_EOK)
                {
                    MemoryP_ctrlFree (ptrCanFdMCB, sizeof(CANFD_DriverMCB));
                    ptrCanFdMCB = NULL;
                }
            }

            /* Configure the Message RAM */
            if (*errCode == CANFD_EOK)
            {
                CANFD_configMessageRAM(ptrCanFdMCB, &configParams->msgRAMConfig, errCode);
                if (*errCode != CANFD_EOK)
                {
                    MemoryP_ctrlFree (ptrCanFdMCB, sizeof(CANFD_DriverMCB));
                    ptrCanFdMCB = NULL;
                }
            }

            if (*errCode == CANFD_EOK)
            {
                /* Configure the ECC Aggregator */
                MCAN_eccConfig(baseAddr, &configParams->eccConfig);

                /* Clear all pending error flags and status */
                MCAN_clearIntrStatus(baseAddr, MCAN_INTR_MASK);
                newDataStatus.statusLow  = MCAN_NDAT1_CLEAR;
                newDataStatus.statusHigh = MCAN_NDAT2_CLEAR;
                MCAN_clearNewDataStatus(baseAddr, &newDataStatus);

                /* Register the Data Interrupt Handler. */
                if (configParams->dataInterruptEnable == 1U)
                {
                    HwiP_Params_init(&hwiParams);
                    hwiParams.name = "CANFD_0";
                    hwiParams.arg  = (uintptr_t)ptrCanFdMCB;
                    ptrCanFdMCB->hwiHandle0 = HwiP_create(ptrCanFdMCB->hwCfg.interruptNum0, CANFD_MCANInt0Isr, &hwiParams);
                }

                /* Register the ECC, error Interrupt Handler. */
                if (configParams->errInterruptEnable == 1U)
                {
                    HwiP_Params_init(&hwiParams);
                    hwiParams.name = "CANFD_1";
                    hwiParams.arg  = (uintptr_t)ptrCanFdMCB;
                    ptrCanFdMCB->hwiHandle1 = HwiP_create(ptrCanFdMCB->hwCfg.interruptNum1, CANFD_MCANInt1Isr, &hwiParams);
                }

                if (configParams->eccConfig.enable == 1U)
                {
                    /* Enable ECC Interrupt */
                    MCAN_eccEnableIntr(baseAddr, CANFD_MCANECCErrType_SEC, 1U);
                    MCAN_eccEnableIntr(baseAddr, CANFD_MCANECCErrType_DED, 1U);
                }

                /* Enable the MCAN interrupts */
                MCAN_enableIntr(baseAddr, MCAN_INTR_MASK, 1U);
                MCAN_selectIntrLine(baseAddr, MCAN_INTR_MASK, MCAN_IntrLineNum_0);
                MCAN_enableIntrLine(baseAddr, MCAN_IntrLineNum_0, 1U);

                /* Put MCAN in opertional mode */
                MCAN_setOpMode(baseAddr, CANFD_MCANOperationMode_NORMAL);

                /* Wait while MCAN is not in operational mode. TODO add timeout */
                do
                {
                } while (MCAN_getOpMode(baseAddr) != CANFD_MCANOperationMode_NORMAL);

                /* Initialize the datalength to DLC mapping */
                ptrCanFdMCB->mcanDataSize[0] = 0;
                ptrCanFdMCB->mcanDataSize[1] = 1U;
                ptrCanFdMCB->mcanDataSize[2] = 2U;
                ptrCanFdMCB->mcanDataSize[3] = 3U;
                ptrCanFdMCB->mcanDataSize[4] = 4U;
                ptrCanFdMCB->mcanDataSize[5] = 5U;
                ptrCanFdMCB->mcanDataSize[6] = 6U;
                ptrCanFdMCB->mcanDataSize[7] = 7U;
                ptrCanFdMCB->mcanDataSize[8] = 8U;
                ptrCanFdMCB->mcanDataSize[9] = 12U;
                ptrCanFdMCB->mcanDataSize[10] = 16U;
                ptrCanFdMCB->mcanDataSize[11] = 20U;
                ptrCanFdMCB->mcanDataSize[12] = 24U;
                ptrCanFdMCB->mcanDataSize[13] = 32U;
                ptrCanFdMCB->mcanDataSize[14] = 48U;
                ptrCanFdMCB->mcanDataSize[15] = 64U;

                /* Store the relevant fields from the configuration */
                ptrCanFdMCB->appDataCallBack =  configParams->appDataCallBack;
                ptrCanFdMCB->appErrCallBack =  configParams->appErrCallBack;

                /* Initialize the CAN driver state */
                ptrCanFdMCB->state = CANFD_DriverState_STARTED;
            }
        }
    }
    return (CANFD_Handle)ptrCanFdMCB;
}

/**
 *  @b Description
 *  @n
 *      Function closes the CANFD driver instance and cleanups all the memory allocated by the CANFD driver.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_deinit(CANFD_Handle handle, int32_t* errCode)
{
    CANFD_DriverMCB*      ptrCanFdMCB;
    uint32_t            index;
    int32_t             retVal = 0;

    /* Get the pointer to the CAN Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)handle;
    if (ptrCanFdMCB == NULL)
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Update the driver state */
        ptrCanFdMCB->state = CANFD_DriverState_STOPPED;

        /* Clean up interrupt handlers */
        if (ptrCanFdMCB->hwiHandle0)
        {
            /* Delete and unregister the interrupt0 handler. */
            HwiP_delete(ptrCanFdMCB->hwiHandle0);
        }
        if (ptrCanFdMCB->hwiHandle1)
        {
            /* Delete and unregister the interrupt1 handler. */
            HwiP_delete(ptrCanFdMCB->hwiHandle1);
        }

        /* Delete the message objects */
        for (index = 0; index < MCAN_MAX_MSG_OBJECTS; index++)
        {
            if (ptrCanFdMCB->msgObjectHandle[index] != NULL)
            {
                MemoryP_ctrlFree (ptrCanFdMCB->msgObjectHandle[index], sizeof(CANFD_MessageObject));
                ptrCanFdMCB->msgObjectHandle[index] = NULL;
            }
        }
        MemoryP_ctrlFree (ptrCanFdMCB, sizeof(CANFD_DriverMCB));
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function configures the bit time parameters for the CANFD module.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in]  bitTimeParams
 *      Bit time configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_configBitTime(CANFD_Handle handle, const CANFD_MCANBitTimingParams* bitTimeParams, int32_t* errCode)
{
    CANFD_DriverMCB*        ptrCanFdMCB;
    MCAN_BitTimingParams    mcanBitTimingParams;
    uint32_t                baseAddr;
    int32_t                 retVal = 0;

    /* Get the pointer to the CANFD Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)handle;
    if ((ptrCanFdMCB == NULL) || (bitTimeParams == NULL))
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

        /* Put MCAN in SW initialization mode */
        MCAN_setOpMode(baseAddr, CANFD_MCANOperationMode_SW_INIT);

        /* Wait while MCAN is not in init mode. TODO add timeout */
        do
        {
        } while (MCAN_getOpMode(baseAddr) != CANFD_MCANOperationMode_SW_INIT);

        /* Calculate the MCAN bit timing parameters */
        mcanBitTimingParams.nomRatePrescalar   = bitTimeParams->nomBrp - 1U;
        mcanBitTimingParams.nomTimeSeg1        = bitTimeParams->nomPropSeg + bitTimeParams->nomPseg1 - 1U;
        mcanBitTimingParams.nomTimeSeg2        = bitTimeParams->nomPseg2 - 1U;
        mcanBitTimingParams.nomSynchJumpWidth  = bitTimeParams->nomSjw - 1U;

        mcanBitTimingParams.dataRatePrescalar  = bitTimeParams->dataBrp - 1U;
        mcanBitTimingParams.dataTimeSeg1       = bitTimeParams->dataPropSeg + bitTimeParams->dataPseg1 - 1U;
        mcanBitTimingParams.dataTimeSeg2       = bitTimeParams->dataPseg2 - 1U;
        mcanBitTimingParams.dataSynchJumpWidth = bitTimeParams->dataSjw - 1U;

        /* Set the bit timing values */
        *errCode = MCAN_setBitTime(baseAddr, &mcanBitTimingParams);
        if (*errCode != CANFD_EOK)
        {
            retVal = MINUS_ONE;
        }
        /* Put MCAN in opertional mode */
        MCAN_setOpMode(baseAddr, CANFD_MCANOperationMode_NORMAL);

        /* Wait while MCAN is not in operational mode. TODO add timeout */
        do
        {
        } while (MCAN_getOpMode(baseAddr) != CANFD_MCANOperationMode_NORMAL);
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function configures the receive or transmit message object.
 *      It also enables Tx completion and Tx cancelation interrupts.
 *      The callback function will be invoked on data transmit complete for transmit message objects
 *      OR
 *      upon receiving data for receive message objects. The application MUST then call CANFD_getData() API to process the received data.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in]  msgObjectParams
 *      Message Object configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the message object.
 *  @retval
 *      Error   -   NULL
 */
CANFD_MsgObjHandle CANFD_createMsgObject(CANFD_Handle handle, const CANFD_MCANMsgObjCfgParams* msgObjectParams, int32_t* errCode)
{
    CANFD_DriverMCB*            ptrCanFdMCB;
    uint32_t                    baseAddr;
    CANFD_MessageObject*        ptrCanMsgObj;
    MCAN_StdMsgIDFilterElement  stdMsgIdFilter;
    MCAN_ExtMsgIDFilterElement  extMsgIdFilter;
    uint32_t                    i;

    /* Get the pointer to the CAN Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)handle;
    if ((ptrCanFdMCB == NULL) || (msgObjectParams == NULL))
    {
        *errCode = CANFD_EINVAL;
        ptrCanMsgObj =  NULL;
    }
    else
    {
        *errCode = CANFD_EOK;
        baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

        /* Allocate memory for the CAN Message Object */
        ptrCanMsgObj = MemoryP_ctrlAlloc (sizeof(CANFD_MessageObject), 0U);
        if (ptrCanMsgObj == NULL)
        {
            /* Error: Unable to allocate the memory */
            *errCode = CANFD_ENOMEM;
            ptrCanMsgObj = NULL;
        }
        else
        {
            /* Initialize the allocated memory */
            memset ((void *)ptrCanMsgObj, 0, sizeof(CANFD_MessageObject));

            /* Save the specified parameters */
            ptrCanMsgObj->ptrDriverMCB = ptrCanFdMCB;
            ptrCanMsgObj->direction = msgObjectParams->direction;
            ptrCanMsgObj->msgIdType = msgObjectParams->msgIdType;
            ptrCanMsgObj->startMsgId = ptrCanMsgObj->endMsgId = msgObjectParams->msgIdentifier;

            for (i = 0; i < MCAN_MAX_MSG_OBJECTS; i++)
            {
                if (ptrCanFdMCB->msgObjectHandle[i] == NULL)
                {
                    break;
                }
            }
            if (i == MCAN_MAX_MSG_OBJECTS)
            {
                /* Error: Unable to allocate the memory */
                *errCode = CANFD_ENOMEM;
                MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CANFD_MessageObject));
                ptrCanMsgObj = NULL;
            }
            else
            {
                /* Store the message object handle for book keeping */
                ptrCanMsgObj->messageObjNum = i;
                ptrCanFdMCB->msgObjectHandle[i] = ptrCanMsgObj;
            }

            /* Configure the Tx Message Id */
            if ((*errCode == CANFD_EOK) && (ptrCanMsgObj->direction == CANFD_Direction_TX))
            {
                /* Get a free Tx element */
                for (i = 0; i < MCAN_MAX_TX_MSG_OBJECTS; i++)
                {
                    if (ptrCanFdMCB->txMapping[i] == NULL)
                    {
                        break;
                    }
                }
                if (i == MCAN_MAX_TX_MSG_OBJECTS)
                {
                    /* Error: Unable to allocate the memory */
                    *errCode = CANFD_ENOMEM;
                    ptrCanFdMCB->msgObjectHandle[ptrCanMsgObj->messageObjNum] = NULL;
                    MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CANFD_MessageObject));
                    ptrCanMsgObj = NULL;
                }
                else
                {
                    /* Store the tx to message object handle mapping */
                    ptrCanFdMCB->txMapping[i] = ptrCanMsgObj;
                    ptrCanMsgObj->txElement = i;

                    /* Store the message RAM memory access type */
                    ptrCanMsgObj->memType = MCAN_MemType_BUF;

                    /* Enable Tx completion interrupt*/
                    MCAN_txBufTransIntrEnable(baseAddr, i, 1U);

                    /* Enable Tx cancelation interrupt*/
                    MCAN_getTxBufCancellationIntrEnable(baseAddr, i, 1U);
                }
            }

            /* Configure the Rx Message Id */
            if ((*errCode == CANFD_EOK) && (ptrCanMsgObj->direction == CANFD_Direction_RX))
            {
                /* Get a free Rx element */
                for (i = 0; i < MCAN_MAX_RX_MSG_OBJECTS; i++)
                {
                    if (ptrCanFdMCB->rxMapping[i] == NULL)
                    {
                        break;
                    }
                }
                if (i == MCAN_MAX_RX_MSG_OBJECTS)
                {
                    /* Error: Unable to allocate the memory */
                    *errCode = CANFD_ENOMEM;
                    ptrCanFdMCB->msgObjectHandle[ptrCanMsgObj->messageObjNum] = NULL;
                    MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CANFD_MessageObject));
                    ptrCanMsgObj = NULL;
                }
                else
                {
                    /* Store the rx to message object handle mapping */
                    ptrCanFdMCB->rxMapping[i] = ptrCanMsgObj;
                    ptrCanMsgObj->rxElement = i;

                    /* Store the message RAM memory access type */
                    ptrCanMsgObj->memType = MCAN_MemType_BUF;

                    /* Add the filter to message RAM */
                    if (ptrCanMsgObj->msgIdType == CANFD_MCANXidType_11_BIT)
                    {
                        stdMsgIdFilter.sfid1 = ptrCanMsgObj->startMsgId & STD_MSGID_MASK;
                        stdMsgIdFilter.sfid2 = i;

                        /* Store the message in rx buffer */
                        stdMsgIdFilter.sfec = 0x7U;
                        stdMsgIdFilter.sft = 0;
                        MCAN_addStdMsgIDFilter(baseAddr, i, &stdMsgIdFilter);
                    }
                    else
                    {
                        extMsgIdFilter.efid1 = ptrCanMsgObj->startMsgId & XTD_MSGID_MASK;
                        extMsgIdFilter.efid2 = i;

                        /* Store the message in rx buffer */
                        extMsgIdFilter.efec = 0x7U;
                        extMsgIdFilter.eft = 0;
                        MCAN_addExtMsgIDFilter(baseAddr, i, &extMsgIdFilter);
                    }
                }
            }
        }
    }
    return (CANFD_MsgObjHandle)ptrCanMsgObj;
}

/**
 *  @b Description
 *  @n
 *      Function configures a receive message objects for a range of message Identifiersmessage object.
 *      It also enables Rx interrupts.
 *      The callback function will be invoked upon receiving data for receive message objects.
 *      The application MUST then call CANFD_getData() API to process the received data.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in]  msgObjectParams
 *      Message Object configuration parameters
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success -   Handle to the message object.
 *  @retval
 *      Error   -   NULL
 */
CANFD_MsgObjHandle CANFD_createRxRangeMsgObject(CANFD_Handle handle, const CANFD_MCANRxMsgObjRangeCfgParams* msgObjectParams, int32_t* errCode)
{
    CANFD_DriverMCB*            ptrCanFdMCB;
    uint32_t                    baseAddr;
    CANFD_MessageObject*        ptrCanMsgObj;
    MCAN_StdMsgIDFilterElement  stdMsgIdFilter;
    MCAN_ExtMsgIDFilterElement  extMsgIdFilter;
    uint32_t                    i;

    /* Get the pointer to the CAN Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)handle;
    if ((ptrCanFdMCB == NULL) || (msgObjectParams == NULL) || (msgObjectParams->startMsgIdentifier > msgObjectParams->endMsgIdentifier))
    {
        *errCode = CANFD_EINVAL;
        ptrCanMsgObj =  NULL;
    }
    else
    {
        *errCode = CANFD_EOK;
        baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

        /* Allocate memory for the CAN Message Object */
        ptrCanMsgObj = MemoryP_ctrlAlloc (sizeof(CANFD_MessageObject), 0U);
        if (ptrCanMsgObj == NULL)
        {
            /* Error: Unable to allocate the memory */
            *errCode = CANFD_ENOMEM;
            ptrCanMsgObj = NULL;
        }
        else
        {
            /* Initialize the allocated memory */
            memset ((void *)ptrCanMsgObj, 0, sizeof(CANFD_MessageObject));

            /* Save the specified parameters */
            ptrCanMsgObj->ptrDriverMCB = ptrCanFdMCB;
            ptrCanMsgObj->direction = CANFD_Direction_RX;
            ptrCanMsgObj->msgIdType = msgObjectParams->msgIdType;
            ptrCanMsgObj->startMsgId = msgObjectParams->startMsgIdentifier;
            ptrCanMsgObj->endMsgId = msgObjectParams->endMsgIdentifier;

            for (i = 0; i < MCAN_MAX_MSG_OBJECTS; i++)
            {
                if (ptrCanFdMCB->msgObjectHandle[i] == NULL)
                {
                    break;
                }
            }
            if (i == MCAN_MAX_MSG_OBJECTS)
            {
                /* Error: Unable to allocate the memory */
                *errCode = CANFD_ENOMEM;
                MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CANFD_MessageObject));
                ptrCanMsgObj = NULL;
            }
            else
            {
                /* Store the message object handle for book keeping */
                ptrCanMsgObj->messageObjNum = i;
                ptrCanFdMCB->msgObjectHandle[i] = ptrCanMsgObj;
            }

            /* Configure the Rx Message Id */
            if (*errCode == CANFD_EOK)
            {
                /* Get a free Rx element */
                for (i = 0; i < MCAN_MAX_RX_MSG_OBJECTS; i++)
                {
                    if (ptrCanFdMCB->rxMapping[i] == NULL)
                    {
                        break;
                    }
                }
                if (i == MCAN_MAX_RX_MSG_OBJECTS)
                {
                    /* Error: Unable to allocate the memory */
                    *errCode = CANFD_ENOMEM;
                    ptrCanFdMCB->msgObjectHandle[ptrCanMsgObj->messageObjNum] = NULL;
                    MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CANFD_MessageObject));
                    ptrCanMsgObj = NULL;
                }
                else
                {
                    /* Store the rx to message object handle mapping */
                    ptrCanFdMCB->rxMapping[i] = ptrCanMsgObj;
                    ptrCanMsgObj->rxElement = i;

                    /* Store the message RAM memory access type */
                    ptrCanMsgObj->memType = MCAN_MemType_FIFO;

                    /* Add the filter to message RAM */
                    if (ptrCanMsgObj->msgIdType == CANFD_MCANXidType_11_BIT)
                    {
                        stdMsgIdFilter.sfid1 = ptrCanMsgObj->startMsgId & STD_MSGID_MASK;
                        stdMsgIdFilter.sfid2 = ptrCanMsgObj->endMsgId & STD_MSGID_MASK;

                        /* Store the message in FIFO */
                        stdMsgIdFilter.sfec = (ptrCanFdMCB->useFifoNum + 1U);
                        stdMsgIdFilter.sft = 0;
                        MCAN_addStdMsgIDFilter(baseAddr, i, &stdMsgIdFilter);
                    }
                    else
                    {
                        extMsgIdFilter.efid1 = ptrCanMsgObj->startMsgId & XTD_MSGID_MASK;
                        extMsgIdFilter.efid2 = ptrCanMsgObj->endMsgId & XTD_MSGID_MASK;

                        /* Store the message in FIFO */
                        extMsgIdFilter.efec = (ptrCanFdMCB->useFifoNum + 1U);
                        extMsgIdFilter.eft = 0;
                        MCAN_addExtMsgIDFilter(baseAddr, i, &extMsgIdFilter);
                    }

                    /* Toggle the FIFO number for the next message Id */
                    ptrCanFdMCB->useFifoNum = 1U - ptrCanFdMCB->useFifoNum;
                }
            }
        }
    }
    return (CANFD_MsgObjHandle)ptrCanMsgObj;
}

/**
 *  @b Description
 *  @n
 *      Function deletes a message object.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_deleteMsgObject(CANFD_MsgObjHandle handle, int32_t* errCode)
{
    CANFD_MessageObject*  ptrCanMsgObj;
    CANFD_DriverMCB*      ptrCanFdMCB;
    int32_t             retVal = 0;

    ptrCanMsgObj = (CANFD_MessageObject*)handle;
    if (ptrCanMsgObj == NULL)
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Get the pointer to the CAN Driver Block */
        ptrCanFdMCB = (CANFD_DriverMCB*)ptrCanMsgObj->ptrDriverMCB;

        if (ptrCanFdMCB == NULL)
        {
            /* This should never happen */
            DebugP_assert(0);
            retVal = MINUS_ONE;
        }
        else
        {
            ptrCanFdMCB->msgObjectHandle[ptrCanMsgObj->messageObjNum] = NULL;

            if (ptrCanMsgObj->direction == CANFD_Direction_TX)
            {
                ptrCanFdMCB->txMapping[ptrCanMsgObj->txElement] = NULL;
            }

            if (ptrCanMsgObj->direction == CANFD_Direction_RX)
            {
                ptrCanFdMCB->rxMapping[ptrCanMsgObj->rxElement] = NULL;
            }

            MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CANFD_MessageObject));
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function used by the application to transmit data.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[in]  id
 *      Message Identifier
 *  @param[in]  frameType
 *      Frame type - Classic or FD
 *  @param[in]  dataLength
 *      Data Length to be transmitted.
 *      Valid values: 1 to 64 bytes.
 *  @param[in]  data
 *      Data to be transmitted
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_transmitData(CANFD_MsgObjHandle handle, uint32_t id, CANFD_MCANFrameType frameType, uint32_t dataLength, const uint8_t* data, int32_t* errCode)
{
    CANFD_MessageObject*    ptrCanMsgObj;
    CANFD_DriverMCB*        ptrCanFdMCB;
    int32_t                 retVal = 0;
    uint32_t                baseAddr;
    MCAN_TxBufElement       txBuffElem;
    uint32_t                index;
    uintptr_t               key;
    uint8_t                 padSize = 0U;

    /* Get the message object pointer */
    ptrCanMsgObj = (CANFD_MessageObject*)handle;
    if ((ptrCanMsgObj == NULL) || (data == NULL) || (dataLength < 1U) || (dataLength > 64U))
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Get the pointer to the CAN Driver Block */
        ptrCanFdMCB = (CANFD_DriverMCB*)ptrCanMsgObj->ptrDriverMCB;

        if (ptrCanFdMCB == NULL)
        {
            *errCode = CANFD_EINVAL;
            retVal = MINUS_ONE;
        }
        else
        {
            baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

            /* Check for pending messages */
            index = (uint32_t)1U << ptrCanMsgObj->txElement;
            if (index == (MCAN_getTxBufReqPend(baseAddr) & index))
            {
                *errCode = CANFD_EINUSE;
                retVal = MINUS_ONE;
            }
            else
            {
                /* populate the Tx buffer message element */
                txBuffElem.rtr = 0;
                txBuffElem.esi = 0;
                txBuffElem.efc = 0;
                txBuffElem.mm = 0;

                if(frameType == CANFD_MCANFrameType_CLASSIC)
                {
                    txBuffElem.brs = 0;
                    txBuffElem.fdf = 0;
                }
                else
                {
                    txBuffElem.brs = 1U;
                    txBuffElem.fdf = 1U;
                }
                /* Populate the Id */
                if (ptrCanMsgObj->msgIdType == CANFD_MCANXidType_11_BIT)
                {
                    txBuffElem.xtd = CANFD_MCANXidType_11_BIT;
                    txBuffElem.id = (id & STD_MSGID_MASK) << STD_MSGID_SHIFT;
                }
                else
                {
                    txBuffElem.xtd = CANFD_MCANXidType_29_BIT;
                    txBuffElem.id = id & XTD_MSGID_MASK;
                }

                /* Copy the data */
                memcpy ((void*)&txBuffElem.data, data, dataLength);

                /* Compute the DLC value */
                for(index = 0U ; index < 16U ; index++)
                {
                    if(dataLength <= ptrCanFdMCB->mcanDataSize[index])
                    {
                        txBuffElem.dlc = index;
                        padSize = ptrCanFdMCB->mcanDataSize[index] - dataLength;
                        break;
                    }
                }
                txBuffElem.dlc = index;
                if (index == 16)
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                }
                else
                {
                    /* Pad the unused data in payload */
                    index = dataLength;
                    while (padSize != 0)
                    {
                        txBuffElem.data[index++] = (uint8_t)0xCCU;
                        padSize--;
                    }

                    MCAN_writeMsgRam(baseAddr, MCAN_MemType_BUF, ptrCanMsgObj->txElement, &txBuffElem);

                    /* Critical Section Protection */
                    key = HwiP_disable();

                    MCAN_txBufAddReq(baseAddr, ptrCanMsgObj->txElement);
                    ptrCanFdMCB->txStatus[ptrCanMsgObj->txElement] = 1;

                    /* Release the critical section: */
                    HwiP_restore(key);

                    /* Increment the stats */
                    ptrCanMsgObj->messageProcessed++;
                }
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function used by the application to cancel a pending data transmit.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_transmitDataCancel(CANFD_MsgObjHandle handle, int32_t* errCode)
{
    CANFD_MessageObject*    ptrCanMsgObj;
    CANFD_DriverMCB*        ptrCanFdMCB;
    int32_t                 retVal = 0;
    uint32_t                baseAddr;

    /* Get the message object pointer */
    ptrCanMsgObj = (CANFD_MessageObject*)handle;
    if (ptrCanMsgObj == NULL)
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Get the pointer to the CAN Driver Block */
        ptrCanFdMCB = (CANFD_DriverMCB*)ptrCanMsgObj->ptrDriverMCB;

        if (ptrCanFdMCB == NULL)
        {
            *errCode = CANFD_EINVAL;
            retVal = MINUS_ONE;
        }
        else
        {
            baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

            /* Cancel the pending transmit */
            MCAN_txBufCancellationReq(baseAddr, ptrCanMsgObj->txElement);
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function is used by the application to get the CAN message from message RAM using a receive message object.
 *      NOTE: This API must ONLY be called from the callback context.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  id
 *      Message Identifier
 *  @param[out]  ptrFrameType
 *      Frame type - Classic or FD
 *  @param[out]  idType
 *      Meassage Id type - 11 bit standard or 29 bit extended
 *  @param[out]  ptrDataLength
 *      Data Length of the received frame.
 *      Valid values: 1 to 64 bytes.
 *  @param[out]  data
 *      Received data.
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_getData(CANFD_MsgObjHandle handle, uint32_t* id, CANFD_MCANFrameType* ptrFrameType, CANFD_MCANXidType* idType, uint32_t* ptrDataLength, uint8_t* data, int32_t* errCode)
{
    CANFD_MessageObject*    ptrCanMsgObj;
    CANFD_DriverMCB*        ptrCanFdMCB;
    int32_t                 retVal = 0;

    /* Get the message object pointer */
    ptrCanMsgObj = (CANFD_MessageObject*)handle;
    if ((ptrCanMsgObj == NULL) || (id == NULL) || (ptrDataLength == NULL) || (data == NULL))
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Get the pointer to the CAN Driver Block */
        ptrCanFdMCB = (CANFD_DriverMCB*)ptrCanMsgObj->ptrDriverMCB;

        if (ptrCanFdMCB == NULL)
        {
            *errCode = CANFD_EINVAL;
            retVal = MINUS_ONE;
        }
        else
        {
            /* Get the data length from DLC */
            *ptrDataLength = ptrCanFdMCB->mcanDataSize[ptrCanFdMCB->rxBuffElem.dlc];

            /* Get the message Identifier */
            if(ptrCanFdMCB->rxBuffElem.xtd == 1U)
            {
                /* Received frame with Extended ID */
                *id = (uint32_t)(ptrCanFdMCB->rxBuffElem.id);
                *idType = CANFD_MCANXidType_29_BIT;
            }
            else
            {
                /* Received frame with Standard ID */
                *id = (uint32_t)((ptrCanFdMCB->rxBuffElem.id >> 18U) & 0x7FFU);
                *idType = CANFD_MCANXidType_11_BIT;
            }

            /* Get the frame type */
            if(ptrCanFdMCB->rxBuffElem.fdf == 1U)
            {
                /* FD frame Received */
                *ptrFrameType = CANFD_MCANFrameType_FD;
            }
            else
            {
                /* Classic frame Received */
                *ptrFrameType = CANFD_MCANFrameType_CLASSIC;
            }

            /* Copy the data */
            memcpy ((void *)data, ptrCanFdMCB->rxBuffElem.dataPtr, *ptrDataLength);

            /* Increment the stats */
            ptrCanMsgObj->messageProcessed++;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function is used by the application to get the error and status information from the driver.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
 *  @param[out] ptrOptInfo
 *      Option info in TLV format which is populated with the requested information
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_getOptions(CANFD_Handle handle, const CANFD_OptionTLV* ptrOptInfo, int32_t* errCode)
{

    CANFD_DriverMCB*            ptrCanFdMCB;
    CANFD_MessageObject*        ptrCanMsgObj;
    CANFD_MCANErrCntStatus*     ptrErrCounter;
    CANFD_MCANProtocolStatus*   ptrProtoStatus;
    CANFD_MCANMsgObjectStats*   ptrMsgObjectStats;
    int32_t                     retVal = 0;

    /* Get the pointer to the CAN Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)handle;
    if ((ptrCanFdMCB == NULL) || (ptrOptInfo == NULL))
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Process the supported options */
        switch (ptrOptInfo->type)
        {
            case CANFD_Option_MCAN_ERROR_COUNTER:
            {
                /* Sanity Check: Validate the arguments. */
                if (ptrOptInfo->length != sizeof(CANFD_MCANErrCntStatus))
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                    goto endGetOptions;
                }

                ptrErrCounter = (CANFD_MCANErrCntStatus*) ptrOptInfo->value;
                if (ptrErrCounter == NULL)
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                    goto endGetOptions;
                }

                /* Populate the stats */
                MCAN_getErrCounters (ptrCanFdMCB->hwCfg.regBaseAddress, ptrErrCounter);
                break;
            }
            case CANFD_Option_MCAN_PROTOCOL_STATUS:
            {
                /* Sanity Check: Validate the arguments. */
                if (ptrOptInfo->length != sizeof(CANFD_MCANProtocolStatus))
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                    goto endGetOptions;
                }

                ptrProtoStatus = (CANFD_MCANProtocolStatus*) ptrOptInfo->value;
                if (ptrProtoStatus == NULL)
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                    goto endGetOptions;
                }

                /* Populate the stats */
                MCAN_getProtocolStatus (ptrCanFdMCB->hwCfg.regBaseAddress, ptrProtoStatus);
                break;
            }
            case CANFD_Option_MCAN_MSG_OBJECT_STATS:
            {
                /* Sanity Check: Validate the arguments. */
                if (ptrOptInfo->length != sizeof(CANFD_MCANMsgObjectStats))
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                    goto endGetOptions;
                }

                ptrMsgObjectStats = (CANFD_MCANMsgObjectStats*) ptrOptInfo->value;
                if (ptrMsgObjectStats == NULL)
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                    goto endGetOptions;
                }

                ptrCanMsgObj = (CANFD_MessageObject*) ptrMsgObjectStats->handle;
                if (ptrCanMsgObj == NULL)
                {
                    *errCode = CANFD_EINVAL;
                    retVal = MINUS_ONE;
                    goto endGetOptions;
                }

                /* Populate the stats */
                ptrMsgObjectStats->startMsgIdentifier = ptrCanMsgObj->startMsgId;
                ptrMsgObjectStats->endMsgIdentifier = ptrCanMsgObj->endMsgId;
                ptrMsgObjectStats->direction = ptrCanMsgObj->direction;
                ptrMsgObjectStats->interruptsRxed = ptrCanMsgObj->interruptsRxed;
                ptrMsgObjectStats->messageProcessed = ptrCanMsgObj->messageProcessed;
                break;
            }
            default:
            {
                /* Option is NOT supported */
                *errCode = CANFD_EINVAL;
                retVal = MINUS_ONE;
                break;
            }
        }
    }

endGetOptions:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function is used by the application to configure the driver options.
 *
 *  @param[in]  handle
 *      Handle to the CANFD Driver
 *  @param[in] ptrOptInfo
 *      Option info in TLV format which is used to configure the driver
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CANFD_setOptions(CANFD_Handle handle, const CANFD_OptionTLV* ptrOptInfo, int32_t* errCode)
{
    CANFD_DriverMCB*                ptrCanFdMCB;
    uint32_t                        baseAddr;
    CANFD_MCANLoopbackCfgParams*    ptrMcanLoopBackCfg;
    int32_t                         retVal = 0;
    /* Get the pointer to the CAN Driver Block */
    ptrCanFdMCB = (CANFD_DriverMCB*)handle;
    if ((ptrCanFdMCB == NULL) || (ptrOptInfo == NULL))
    {
        *errCode = CANFD_EINVAL;
        retVal = MINUS_ONE;
        goto endSetOptions;
    }

    /* Process the supported options */
    switch (ptrOptInfo->type)
    {
        case CANFD_Option_MCAN_MODE:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CANFD_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

            if ((*(uint8_t*)ptrOptInfo->value == 1U) || (*(uint8_t*)ptrOptInfo->value == 0U))
            {
                /* Put MCAN in soecified mode */
                MCAN_setOpMode(baseAddr, (uint32_t)(*(uint8_t*)ptrOptInfo->value));

                /* Wait while MCAN is not in init mode. TODO add timeout */
                do
                {
                } while (MCAN_getOpMode(baseAddr) != *(uint8_t*)ptrOptInfo->value);
            }
            else
            {
                *errCode = CANFD_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        case CANFD_Option_MCAN_LOOPBACK:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(CANFD_MCANLoopbackCfgParams))
            {
                *errCode = CANFD_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

            ptrMcanLoopBackCfg = (CANFD_MCANLoopbackCfgParams*) ptrOptInfo->value;

            /* Put MCAN in SW initialization mode */
            MCAN_setOpMode(baseAddr, CANFD_MCANOperationMode_SW_INIT);

            /* Wait while MCAN is not in init mode. TODO add timeout */
            do
            {
            } while (MCAN_getOpMode(baseAddr) != CANFD_MCANOperationMode_SW_INIT);

            /* Disable loopback mode */
            if (ptrMcanLoopBackCfg->enable == 0)
            {
                MCAN_lpbkModeEnable(baseAddr, ptrMcanLoopBackCfg->mode, 0U);
            }
            else
            {
                /* Enable loopback mode */
                MCAN_lpbkModeEnable(baseAddr, ptrMcanLoopBackCfg->mode, 1U);
            }
            /* Put MCAN in opertional mode */
            MCAN_setOpMode(baseAddr, CANFD_MCANOperationMode_NORMAL);

            /* Wait while MCAN is not in operational mode. TODO add timeout */
            do
            {
            } while (MCAN_getOpMode(baseAddr) != CANFD_MCANOperationMode_NORMAL);

            break;
        }
        case CANFD_Option_MCAN_POWER_DOWN:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CANFD_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            baseAddr = ptrCanFdMCB->hwCfg.regBaseAddress;

            if (*(uint8_t*)ptrOptInfo->value == 1U)
            {
                /* Request a clock stop to enter local power down */
                MCAN_addClockStopRequest(baseAddr, 1U);

                /* Wait for power down to be successful */
                do
                {
                } while (MCAN_getClkStopAck(baseAddr) != MCAN_CCCR_CSA_ACK);

                /* Update the state information */
                ptrCanFdMCB->state = CANFD_DriverState_SLEEP;
            }
            else if (*(uint8_t*)ptrOptInfo->value == 0U)
            {
                /* Turn on the local clocks to wakeup from local power down */
                MCAN_addClockStopRequest(baseAddr, 0);

                /* Wait for wake up to be successful */
                do
                {
                } while (MCAN_getClkStopAck(baseAddr) != MCAN_CCCR_CSA_NO_ACK);

                /* Put MCAN in opertional mode */
                MCAN_setOpMode(baseAddr, CANFD_MCANOperationMode_NORMAL);

                /* Wait while MCAN is not in operational mode. TODO add timeout */
                do
                {
                } while (MCAN_getOpMode(baseAddr) != CANFD_MCANOperationMode_NORMAL);

                /* Update the state information */
                ptrCanFdMCB->state = CANFD_DriverState_STARTED;
            }
            else
            {
                *errCode = CANFD_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        default:
        {
            /* Option is NOT supported */
            *errCode = CANFD_EINVAL;
            retVal = MINUS_ONE;
            break;
        }
    }
endSetOptions:
    return retVal;
}
