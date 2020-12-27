/**
 *   @file  can.c
 *
 *   @brief
 *      The file implements the Controller Area Network Driver
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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
#include <ti/drivers/can/include/can_internal.h>

/**
 *  @b Description
 *  @n
 *      The function is the registered interrupt 0 ISR for the CAN Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering the ISR
 *
 *  \ingroup CAN_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CAN_DCANInt0Isr (uintptr_t arg)
{
    CAN_DriverMCB*      ptrCanMCB;
    uint32_t            baseAddr;
    uint32_t            value;
     uint32_t           interruptId;
    CAN_ErrStatusResp   errStatusResp;

    /* Get the pointer to the CAN Driver Block */
    ptrCanMCB = (CAN_DriverMCB*)arg;
    if (ptrCanMCB == NULL)
    {
        /* This should never happen */
        DebugP_assert(0);
    }
    else
    {
        baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

        /* Read the interrupt Id register to get the source of interrupt */
        interruptId = DCANGetIntrStatus(baseAddr, CAN_DCANIntrLineNum_0);
        while (interruptId != 0U)
        {
            /* Check if it is a error or status interrupt? */
            if (interruptId != 0x8000U)
            {
                ptrCanMCB->spuriousInterrupts++;
            }
            else
            {
                ptrCanMCB->errStatusInterrupts++;

                /* Call the registered callback. */
                if (ptrCanMCB->cfg.appCallBack != NULL)
                {
                    /* Read the error status register and fill out the response structure */
                    value = DCANGetErrStatus(baseAddr);

                    errStatusResp.powerDownAck = ((value & DCAN_ES_PDA_MASK) >> DCAN_ES_PDA_SHIFT);
                    errStatusResp.wakeUpPend = ((value & DCAN_ES_WAKEUP_PND_MASK) >> DCAN_ES_WAKEUP_PND_SHIFT);
                    errStatusResp.parityError = ((value & DCAN_ES_PER_MASK) >> DCAN_ES_PER_SHIFT);
                    errStatusResp.busOff = ((value & DCAN_ES_BOFF_MASK) >> DCAN_ES_BOFF_SHIFT);
                    errStatusResp.eWarn = ((value & DCAN_ES_EWARN_MASK) >> DCAN_ES_EWARN_SHIFT);
                    errStatusResp.ePass = ((value & DCAN_ES_EPASS_MASK) >> DCAN_ES_EPASS_SHIFT);
                    errStatusResp.lec = ((value & DCAN_ES_LEC_MASK) >> DCAN_ES_LEC_SHIFT);

                    if ((errStatusResp.powerDownAck == 1U) || (errStatusResp.wakeUpPend == 1U) ||
                            (errStatusResp.parityError == 1U) || (errStatusResp.busOff == 1U) ||
                            (errStatusResp.eWarn == 1U) || (errStatusResp.ePass == 1U) ||
                            ((errStatusResp.lec > 0U) && (errStatusResp.lec < 7U)))
                    {
                        ptrCanMCB->cfg.appCallBack((CAN_Handle)ptrCanMCB, &errStatusResp);
                    }
                }
            }
            interruptId = DCANGetIntrStatus(baseAddr, CAN_DCANIntrLineNum_0);
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered interrupt 1 ISR for the CAN Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering the ISR
 *
 *  \ingroup CAN_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CAN_DCANInt1Isr (uintptr_t arg)
{
    CAN_MessageObject*  ptrCanMsgObj;
    CAN_DriverMCB*      ptrCanMCB;
    uint32_t            baseAddr;
    uint32_t            ifRegNum;
    uint32_t            interruptId;

    /* Get the pointer to the CAN Driver Block */
    ptrCanMCB = (CAN_DriverMCB*)arg;
    if (ptrCanMCB == NULL)
    {
        /* This should never happen */
        DebugP_assert(0);
    }
    else
    {
        baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

        interruptId = DCANGetIntrStatus(baseAddr, CAN_DCANIntrLineNum_1);
        while (interruptId != 0U)
        {
            if (interruptId > DCAN_MAX_MSG_OBJECTS)
            {
                ptrCanMCB->spuriousInterrupts++;
            }
            else
            {
                ptrCanMCB->messageInterrupts++;

                /* Get the message object pointer */
                ptrCanMsgObj = ptrCanMCB->msgObjectHandle[interruptId - 1U];

                /* Increment the number of interrupts received */
                ptrCanMsgObj->interruptsRxed++;

                if (ptrCanMsgObj->direction == CAN_Direction_TX)
                {
                    ifRegNum = CAN_DCANIfRegNum_1;
                }
                else
                {
                    ifRegNum = CAN_DCANIfRegNum_2;
                }

                if ((DCANIsMsgObjIntrPending(baseAddr, ptrCanMsgObj->msgObjectNum)) == 1U)
                {
                    /* Clear the interrupt to indicate the transmit complete indication has been read */
                    if (ptrCanMsgObj->direction == CAN_Direction_TX)
                    {
                        ifRegNum = CAN_DCANIfRegNum_1;
                        do
                        {
                            /* Processing loop? */
                            if (DCANIsIfRegBusy(baseAddr, ifRegNum) == 0U)
                            {
                                break;
                            }
                        }
                        while (1);

                        /* Clear the interrupts for message object */
                        DCANIntrClearStatus(baseAddr, ptrCanMsgObj->msgObjectNum, ifRegNum);

                        /* Wait for the operation to complete */
                        do
                        {
                            /* Processing loop? */
                            if (DCANIsIfRegBusy(baseAddr, ifRegNum) == 0U)
                            {
                                break;
                            }
                        }
                        while (1);
                    }

                    /* Call the registered callback for both tx and Rx.
                     * Incase of Rx application will call CAN_getData to read the received data */
                    if (ptrCanMsgObj->appCallBack != NULL)
                    {
                        ptrCanMsgObj->appCallBack((CAN_MsgObjHandle)ptrCanMsgObj, ptrCanMsgObj->msgObjectNum, ptrCanMsgObj->direction);
                    }
                }
            }
            interruptId = DCANGetIntrStatus(baseAddr, CAN_DCANIntrLineNum_1);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Function initializes the CAN driver instance with the specified hardware attributes.
 *      It resets the CAN module and configures the DCAN TX/RX IO Control register.
 *      It configures the CAN driver with the control parameters.
 *
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

CAN_Handle CAN_init(const CAN_DCANCfgParams* configParams, int32_t* errCode)
{
    CAN_DriverMCB*  ptrCanMCB = NULL;
    HwiP_Params     hwiParams;
    uint32_t        baseAddr;

    if (configParams == NULL)
    {
        *errCode = CAN_EINVAL;
        goto endInit;
    }

    /* If error or status interrupt is enabled then line0 must be enabled */
    if ((configParams->errIntrEnable == 1U) || (configParams->stsChangeIntrEnable == 1U))
    {
        if (configParams->intrLine0Enable == 0U)
        {
            *errCode = CAN_EINVAL;
            goto endInit;
        }
    }

    /* Allocate memory for the CAN Driver */
    ptrCanMCB = MemoryP_ctrlAlloc (sizeof(CAN_DriverMCB), 0U);
    if (ptrCanMCB == NULL)
    {
        /* Error: Unable to allocate the memory */
        *errCode = CAN_ENOMEM;
        goto endInit;
    }

    /* Initialize the allocated memory */
    memset ((void *)ptrCanMCB, 0, sizeof(CAN_DriverMCB));

    /* Initialize the CAN driver state */
    ptrCanMCB->state = CAN_DriverState_UNINIT;

    /* Copy over the configuration */
    memcpy ((void *)&ptrCanMCB->hwCfg, (void*)&gCanHwCfg, sizeof(CAN_HwCfg));

    baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

    /* Reset the DCAN module. */
    DCANSetMode(baseAddr, CAN_DCANMode_INIT);

    HW_WR_FIELD32((baseAddr + DCAN_CTL), DCAN_CTL_SWR, DCAN_CTL_SWR_EN_2_0X1);

    /* Wait until the reset is done */
    do
    {
        if (DCAN_CTL_SWR_MASK != (HW_RD_REG32(baseAddr + DCAN_CTL) & DCAN_CTL_SWR_MASK))
        {
            break;
        }
    }
    while (1);

    /* Configure TX I/O Control Register */
    HW_WR_FIELD32(
        (baseAddr + DCAN_TIOC),
        DCAN_TIOC_FUNC,
        DCAN_TIOC_FUNC_EN_2_0X1);

    /* Configure RX I/O Control Register */
    HW_WR_FIELD32(
        (baseAddr + DCAN_RIOC),
        DCAN_RIOC_FUNC,
        DCAN_RIOC_FUNC_EN_2_0X1);

    /* Configure the DCAN module */
    DCANConfig (ptrCanMCB->hwCfg.regBaseAddress, configParams);

    /* Register interrupts that are enabled */
    if (configParams->intrLine0Enable == 1U)
    {
        /* Register the Interrupt Handler. */
        HwiP_Params_init(&hwiParams);
        hwiParams.name = "CAN_0";
        hwiParams.arg  = (uintptr_t)ptrCanMCB;
        ptrCanMCB->hwiHandle0 = HwiP_create(ptrCanMCB->hwCfg.interruptNum0, CAN_DCANInt0Isr, &hwiParams);
    }

    /* Register interrupts that are enabled */
    if (configParams->intrLine1Enable == 1U)
    {
        /* Register the Interrupt Handler. */
        HwiP_Params_init(&hwiParams);
        hwiParams.name = "CAN_1";
        hwiParams.arg  = (uintptr_t)ptrCanMCB;
        ptrCanMCB->hwiHandle1 = HwiP_create(ptrCanMCB->hwCfg.interruptNum1, CAN_DCANInt1Isr, &hwiParams);
    }

    /* Copy over the configuration */
    memcpy ((void *)&ptrCanMCB->cfg, (void*)configParams, sizeof(CAN_DCANCfgParams));

    /* Initialize the CAN driver state */
    ptrCanMCB->state = CAN_DriverState_STARTED;

endInit:
    return (CAN_Handle)ptrCanMCB;
}

/**
 *  @b Description
 *  @n
 *      Function closes the CAN driver instance and cleanups all the memory allocatedby the CAN driver.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CAN_deinit(CAN_Handle handle, int32_t* errCode)
{
    CAN_DriverMCB*      ptrCanMCB;
    uint32_t            index;
    int32_t             retVal = 0;

    /* Get the pointer to the CAN Driver Block */
    ptrCanMCB = (CAN_DriverMCB*)handle;
    if (ptrCanMCB == NULL)
    {
        *errCode = CAN_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Update the driver state */
        ptrCanMCB->state = CAN_DriverState_STOPPED;

        /* Clean up interrupt handlers */
        if (ptrCanMCB->hwiHandle0)
        {
            /* Delete and unregister the interrupt0 handler. */
            HwiP_delete(ptrCanMCB->hwiHandle0);
        }
        if (ptrCanMCB->hwiHandle1)
        {
            /* Delete and unregister the interrupt1 handler. */
            HwiP_delete(ptrCanMCB->hwiHandle1);
        }

        /* Delete the message objects */
        for (index = 0; index < DCAN_MAX_MSG_OBJECTS; index++)
        {
            if (ptrCanMCB->msgObjectHandle[index] != NULL)
            {
                MemoryP_ctrlFree (ptrCanMCB->msgObjectHandle[index], sizeof(CAN_MessageObject));
                ptrCanMCB->msgObjectHandle[index] = NULL;
            }
        }
        MemoryP_ctrlFree (ptrCanMCB, sizeof(CAN_DriverMCB));
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function configures the bit time parameters for the CAN module.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
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
int32_t CAN_configBitTime(CAN_Handle handle, const CAN_DCANBitTimeParams* bitTimeParams, int32_t* errCode)
{
    CAN_DriverMCB*      ptrCanMCB;
    uint32_t            baseAddr;
    int32_t             retVal = 0;

    /* Get the pointer to the CAN Driver Block */
    ptrCanMCB = (CAN_DriverMCB*)handle;
    if ((ptrCanMCB == NULL) || (bitTimeParams == NULL))
    {
        *errCode = CAN_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

        /* Set the mode to Init before accessing the bit timing registers */
        DCANSetMode(baseAddr, CAN_DCANMode_INIT);

        /* Set the bit timing values */
        DCANSetBitTime(baseAddr, bitTimeParams);

        /* Set the mode to Normal so the module can be operational */
        DCANSetMode(baseAddr, CAN_DCANMode_NORMAL);
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function configures the receive or transmit message object.
 *      It also registers interrupts if enabled and registers the application provided callback function.
 *      The callback function will be invoked on data transmit complete for transmit message objects
 *      OR
 *      upon receiving data for receive message objects.
 *
 *  @param[in]  handle
 *      Handle to the CAN Driver
 *  @param[in]  msgObjectNum
 *      Message Object number which has to be configured.
 *      Valid values are 1 to 64. The message object number also specifies its priority. Lower the number higher the priority.
 *
 *      NOTE: Since the the message object number denotes its priority, the CAN driver doesn't allocate the next available message
 *      object instead it is specified by the application.
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
CAN_MsgObjHandle CAN_createMsgObject(CAN_Handle handle, uint32_t msgObjectNum, const CAN_DCANMsgObjCfgParams* msgObjectParams, int32_t* errCode)
{
    CAN_DriverMCB*      ptrCanMCB;
    uint32_t            baseAddr;
    CAN_MessageObject*  ptrCanMsgObj = NULL;
    uint32_t            ifRegNum;

    /* Get the pointer to the CAN Driver Block */
    ptrCanMCB = (CAN_DriverMCB*)handle;
    if ((ptrCanMCB == NULL) || (msgObjectParams == NULL))
    {
        *errCode = CAN_EINVAL;
    }
    else
    {
        if (msgObjectParams->intEnable)
        {
            /* Check if module interrupts are enabled? */
            if (ptrCanMCB->cfg.intrLine1Enable == 0U)
            {
                *errCode = CAN_EINVAL;
                goto endCreateMsgObject;
            }
        }

        if ((msgObjectNum < 1U) || (msgObjectNum > DCAN_MAX_MSG_OBJECTS))
        {
            *errCode = CAN_EINVAL;
            goto endCreateMsgObject;
        }

        /* Check if this is a new message object or a reconfiguration? */
        if (ptrCanMCB->msgObjectHandle[msgObjectNum - 1U] == NULL)
        {
            /* Allocate memory for the CAN Message Object */
            ptrCanMsgObj = MemoryP_ctrlAlloc (sizeof(CAN_MessageObject), 0U);
            if (ptrCanMsgObj == NULL)
            {
                /* Error: Unable to allocate the memory */
                *errCode = CAN_ENOMEM;
                goto endCreateMsgObject;
            }
            /* Initialize the allocated memory */
            memset ((void *)ptrCanMsgObj, 0, sizeof(CAN_MessageObject));

            /* Save the specified parameters */
            ptrCanMsgObj->ptrDriverMCB = ptrCanMCB;
            ptrCanMsgObj->msgObjectNum = msgObjectNum;
            ptrCanMsgObj->direction = msgObjectParams->direction;
            ptrCanMsgObj->appCallBack = msgObjectParams->appCallBack;

            /* Store the message object handle for book keeping */
            ptrCanMCB->msgObjectHandle[msgObjectNum - 1U] = ptrCanMsgObj;
        }
        else
        {
            ptrCanMsgObj = ptrCanMCB->msgObjectHandle[msgObjectNum - 1U];
        }

        baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

        /*  Optimize the interrupt usage between data, error and status.
         * Enable message object interrupt to route interrupt line 1.
         * Interrupt line 0 is used for status and error interrupts.
         */
        if (msgObjectParams->intEnable)
        {
            DCANConfigIntrMux(baseAddr, CAN_DCANIntrLineNum_1, msgObjectNum);
        }

        if (ptrCanMsgObj->direction == CAN_Direction_TX)
        {
            ifRegNum = CAN_DCANIfRegNum_1;
        }
        else
        {
            ifRegNum = CAN_DCANIfRegNum_2;
        }

        *errCode = DCANConfigMsgObj (baseAddr, msgObjectNum, ifRegNum, msgObjectParams);
        if (*errCode != CAN_EOK)
        {
            ptrCanMCB->msgObjectHandle[msgObjectNum - 1U] = NULL;
            MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CAN_MessageObject));
            ptrCanMsgObj = NULL;
        }
    }
endCreateMsgObject:
    return (CAN_MsgObjHandle)ptrCanMsgObj;
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
int32_t CAN_deleteMsgObject(CAN_MsgObjHandle handle, int32_t* errCode)
{
    CAN_MessageObject*  ptrCanMsgObj;
    CAN_DriverMCB*      ptrCanMCB;
    int32_t             retVal = 0;

    ptrCanMsgObj = (CAN_MessageObject*)handle;
    if (ptrCanMsgObj == NULL)
    {
        *errCode = CAN_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Get the pointer to the CAN Driver Block */
        ptrCanMCB = (CAN_DriverMCB*)ptrCanMsgObj->ptrDriverMCB;

        if (ptrCanMCB == NULL)
        {
            /* This should never happen */
            DebugP_assert(0);
            retVal = MINUS_ONE;
        }
        else
        {
            ptrCanMCB->msgObjectHandle[ptrCanMsgObj->msgObjectNum - 1U] = NULL;
            MemoryP_ctrlFree (ptrCanMsgObj, sizeof(CAN_MessageObject));
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function used by the application to transmit data using a transmit message object.
 *
 *  @param[in]  handle
 *      Handle to the message object
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
int32_t CAN_transmitData(CAN_MsgObjHandle handle, const CAN_DCANData* data, int32_t* errCode)
{
    CAN_MessageObject*  ptrCanMsgObj;
    CAN_DriverMCB*      ptrCanMCB;
    int32_t             retVal = 0;
    uint32_t            baseAddr;
    uint32_t            ifRegNum;

    /* Get the message object pointer */
    ptrCanMsgObj = (CAN_MessageObject*)handle;
    if ((ptrCanMsgObj == NULL) || (data == NULL))
    {
        *errCode = CAN_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Get the pointer to the CAN Driver Block */
        ptrCanMCB = (CAN_DriverMCB*)ptrCanMsgObj->ptrDriverMCB;

        if (ptrCanMCB == NULL)
        {
            *errCode = CAN_EINVAL;
            retVal = MINUS_ONE;
        }
        else
        {
            baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

            ifRegNum = CAN_DCANIfRegNum_1;

            *errCode = DCANTransmitData(baseAddr, ptrCanMsgObj->msgObjectNum, ifRegNum, data, 100U);
            if (*errCode != CAN_EOK)
            {
                retVal = MINUS_ONE;
            }
            else
            {
                /* Increment the stats */
                ptrCanMsgObj->messageProcessed++;
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Function is used by the application to get the CAN message from message RAM using a receive message object.
 *
 *  @param[in]  handle
 *      Handle to the message object
 *  @param[out]  data
 *      Pointer to the data bytes received, length and message lost flag.
 *      Populated only when there is no error. If there is an error, the function returns the reason for error in errCode.
 *  @param[out]  errCode
 *      Error code populated on error
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CAN_getData(CAN_MsgObjHandle handle, CAN_DCANData* data, int32_t* errCode)
{
    CAN_MessageObject*  ptrCanMsgObj;
    CAN_DriverMCB*      ptrCanMCB;
    int32_t             retVal = 0;
    uint32_t            baseAddr;
    uint32_t            ifRegNum;

    /* Get the message object pointer */
    ptrCanMsgObj = (CAN_MessageObject*)handle;
    if ((ptrCanMsgObj == NULL) || (data == NULL))
    {
        *errCode = CAN_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Get the pointer to the CAN Driver Block */
        ptrCanMCB = (CAN_DriverMCB*)ptrCanMsgObj->ptrDriverMCB;

        if (ptrCanMCB == NULL)
        {
            *errCode = CAN_EINVAL;
            retVal = MINUS_ONE;
        }
        else
        {
            baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

            ifRegNum = CAN_DCANIfRegNum_2;

            /* Read the pending data */
            *errCode = DCANGetData(baseAddr, ptrCanMsgObj->msgObjectNum, ifRegNum, data, 100U);
            if (*errCode == CAN_EOK)
            {
                /* Increment the stats */
                ptrCanMsgObj->messageProcessed++;
                do
                {
                    /* Processing loop? */
                    if (DCANIsIfRegBusy(baseAddr, ifRegNum) == 0U)
                    {
                        break;
                    }
                }
                while (1);

                /* Clear the interrupts for message object */
                DCANIntrClearStatus(baseAddr, ptrCanMsgObj->msgObjectNum, ifRegNum);

                /* Wait for the operation to complete */
                do
                {
                    /* Processing loop? */
                    if (DCANIsIfRegBusy(baseAddr, ifRegNum) == 0U)
                    {
                        break;
                    }
                }
                while (1);
            }
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
int32_t CAN_getOptions(CAN_Handle handle, CAN_OptionTLV* ptrOptInfo, int32_t* errCode)
{
    CAN_DriverMCB*          ptrCanMCB;
    CAN_DCANErrorCounter*   ptrErrCounter;
    CAN_DCANParityError*    ptrParityError;
    CAN_MessageObject*      ptrCanMsgObj;
    CAN_DCANMsgObjectStats* ptrMsgObjectStats;
    CAN_DCANEccErrSts*      ptrEccErrSts;
    uint32_t                value;
    int32_t                 retVal = 0;

    /* Get the pointer to the CAN Driver Block */
    ptrCanMCB = (CAN_DriverMCB*)handle;
    if ((ptrCanMCB == NULL) || (ptrOptInfo == NULL))
    {
        *errCode = CAN_EINVAL;
        retVal = MINUS_ONE;
        goto endGetOptions;
    }

    /* Process the supported options */
    switch (ptrOptInfo->type)
    {
        case CAN_Option_DCAN_ERROR_COUNTER:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(CAN_DCANErrorCounter))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            /* Populate the counters */
            ptrErrCounter = (CAN_DCANErrorCounter*) ptrOptInfo->value;
            value = DCANGetErrCntrStatus(ptrCanMCB->hwCfg.regBaseAddress);

            ptrErrCounter->rxErrPassive = ((value & DCAN_ERRC_RP_MASK) >> DCAN_ERRC_RP_SHIFT);
            ptrErrCounter->txErrCounter = ((value & DCAN_ERRC_TEC_MASK) >> DCAN_ERRC_TEC_SHIFT);
            ptrErrCounter->rxErrCounter = ((value & DCAN_ERRC_REC_MASK) >> DCAN_ERRC_REC_SHIFT);
            break;
        }
        case CAN_Option_DCAN_PARITY_ERROR:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(CAN_DCANParityError))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            /* Populate the counters */
            ptrParityError = (CAN_DCANParityError*) ptrOptInfo->value;
            value = DCANGetParityErrStatus(ptrCanMCB->hwCfg.regBaseAddress);

            ptrParityError->messageNum = ((value & DCAN_PERR_MESSAGE_NUMBER_MASK) >> DCAN_PERR_MESSAGE_NUMBER_SHIFT);
            ptrParityError->wordNum = ((value & DCAN_PERR_WORD_NUMBER_MASK) >> DCAN_PERR_WORD_NUMBER_SHIFT);
            break;
        }
        case CAN_Option_DCAN_MSG_OBJECT_STATS:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(CAN_DCANMsgObjectStats))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            ptrMsgObjectStats = (CAN_DCANMsgObjectStats*) ptrOptInfo->value;
            if (ptrMsgObjectStats == NULL)
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            ptrCanMsgObj = (CAN_MessageObject*) ptrMsgObjectStats->handle;
            if (ptrCanMsgObj == NULL)
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            /* Populate the stats */
            ptrMsgObjectStats->msgObjectNum = ptrCanMsgObj->msgObjectNum;
            ptrMsgObjectStats->direction = ptrCanMsgObj->direction;
            ptrMsgObjectStats->interruptsRxed = ptrCanMsgObj->interruptsRxed;
            ptrMsgObjectStats->messageProcessed = ptrCanMsgObj->messageProcessed;
            break;
        }
        case CAN_Option_DCAN_ECC_ERROR_STATUS:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(CAN_DCANEccErrSts))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            ptrEccErrSts = (CAN_DCANEccErrSts*) ptrOptInfo->value;
            if (ptrEccErrSts == NULL)
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            /* Populate the error status*/
            value = DCANGetEccErrStatus(ptrCanMCB->hwCfg.regBaseAddress);
            ptrEccErrSts->singleBitErr = (value & DCAN_ECCCS_SE_FLAG_MASK);
            ptrEccErrSts->doubleBitErr = ((value & DCAN_ECCCS_DE_FLAG_MASK) >> DCAN_ECCCS_DE_FLAG_SHIFT);
            ptrEccErrSts->messageNum = DCANGetMsgObjBitErrDetected(ptrCanMCB->hwCfg.regBaseAddress);
            break;
        }
        case CAN_Option_DCAN_ECC_DIAG_ERROR_STATUS:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(CAN_DCANEccErrSts))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            ptrEccErrSts = (CAN_DCANEccErrSts*) ptrOptInfo->value;
            if (ptrEccErrSts == NULL)
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endGetOptions;
            }

            /* Populate the error status*/
            value = DCANGetEccDiagErrStatus(ptrCanMCB->hwCfg.regBaseAddress);
            ptrEccErrSts->singleBitErr = (value & DCAN_ECCDIAG_STAT_SE_FLAG_MASK);
            ptrEccErrSts->doubleBitErr = ((value & DCAN_ECCDIAG_STAT_DE_FLAG_MASK) >> DCAN_ECCDIAG_STAT_DE_FLAG_SHIFT);
            ptrEccErrSts->messageNum = 0U;
            break;
        }
        default:
        {
            /* Option is NOT supported */
            *errCode = CAN_EINVAL;
            retVal = MINUS_ONE;
            break;
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
 *      Handle to the CAN Driver
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
int32_t CAN_setOptions(CAN_Handle handle, CAN_OptionTLV* ptrOptInfo, int32_t* errCode)
{
    CAN_DriverMCB*          ptrCanMCB;
    uint32_t                value;
    uint32_t                baseAddr;
    int32_t                 retVal = 0;

    /* Get the pointer to the CAN Driver Block */
    ptrCanMCB = (CAN_DriverMCB*)handle;
    if ((ptrCanMCB == NULL) || (ptrOptInfo == NULL))
    {
        *errCode = CAN_EINVAL;
        retVal = MINUS_ONE;
        goto endSetOptions;
    }

    /* Process the supported options */
    switch (ptrOptInfo->type)
    {
        case CAN_Option_DCAN_MODE:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

            /* Set the DCAN mode */
            if ((*(uint8_t*)ptrOptInfo->value == 1U) || (*(uint8_t*)ptrOptInfo->value == 0U))
            {
                DCANSetMode(ptrCanMCB->hwCfg.regBaseAddress, *(uint8_t*)ptrOptInfo->value);
            }
            else
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        case CAN_Option_DCAN_PARITY:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            /* Enable or disable Parity */
            if ((*(uint8_t*)ptrOptInfo->value == 1U) || (*(uint8_t*)ptrOptInfo->value == 0U))
            {
                DCANParityEnable(ptrCanMCB->hwCfg.regBaseAddress, *(uint8_t*)ptrOptInfo->value);
            }
            else
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        case CAN_Option_DCAN_ECC_DIAG:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            baseAddr = ptrCanMCB->hwCfg.regBaseAddress;

            /* Enable or disable ECC Diagnostics */
            if (*(uint8_t*)ptrOptInfo->value == 1U)
            {
                /* Enable test mode */
                HW_WR_FIELD32((baseAddr + DCAN_CTL), DCAN_CTL_TEST, 1U);

                /* Enable RDA mode */
                HW_WR_FIELD32((baseAddr + DCAN_TEST), DCAN_TEST_RDA, 1U);

                /* Enable ECC Diagnostics mode */
                 DCANEccDiagModeEnable(baseAddr, 1U);
            }
            else if (*(uint8_t*)ptrOptInfo->value == 0U)
            {
                /* Disable RDA mode */
                HW_WR_FIELD32((baseAddr + DCAN_TEST), DCAN_TEST_RDA, 0U);

                /* Disable test mode */
                HW_WR_FIELD32((baseAddr + DCAN_CTL), DCAN_CTL_TEST, 0U);

                /* Disable ECC Diagnostics mode */
                 DCANEccDiagModeEnable(baseAddr, 0U);
            }
            else
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        case CAN_Option_DCAN_CLEAR_ECC_ERROR_STATUS:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            /* Clear the DCAN ECC error status */
            if (*(uint8_t*)ptrOptInfo->value == 1U)
            {
                DCANClrEccErrStatus(ptrCanMCB->hwCfg.regBaseAddress);
            }
            else
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        case CAN_Option_DCAN_CLEAR_ECC_DIAG_ERROR_STATUS:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            /* Clear the DCAN ECC Diagnostics error status */
            if (*(uint8_t*)ptrOptInfo->value == 1U)
            {
                DCANClrEccDiagErrStatus(ptrCanMCB->hwCfg.regBaseAddress);
            }
            else
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        case CAN_Option_DCAN_POWER_DOWN:
        {
            /* Sanity Check: Validate the arguments. */
            if (ptrOptInfo->length != sizeof(uint8_t))
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
                goto endSetOptions;
            }

            if (*(uint8_t*)ptrOptInfo->value == 0U)
            {
                /* Write to te control register to wakeup from local power down */
                HW_WR_FIELD32((ptrCanMCB->hwCfg.regBaseAddress + DCAN_CTL), DCAN_CTL_PDR, DCAN_CTL_PDR_EN_1_0X0);

                /* Wait for wake up to be successful */
                do
                {
                    value = DCANGetErrStatus(ptrCanMCB->hwCfg.regBaseAddress);
                }while ((value & DCAN_ES_PDA_MASK) == DCAN_ES_PDA_MASK);

                /* Set the mode to Normal so the module can be operational */
                DCANSetMode(ptrCanMCB->hwCfg.regBaseAddress, CAN_DCANMode_NORMAL);

                /* Update the state information */
                ptrCanMCB->state = CAN_DriverState_STARTED;
            }
            else if (*(uint8_t*)ptrOptInfo->value == 1U)
            {
                /* Write to te control register to enter local power down */
                HW_WR_FIELD32((ptrCanMCB->hwCfg.regBaseAddress + DCAN_CTL), DCAN_CTL_PDR, DCAN_CTL_PDR_EN_2_0X1);

                /* Wait for power down to be successful */
                do
                {
                    value = DCANGetErrStatus(ptrCanMCB->hwCfg.regBaseAddress);
                }while ((value & DCAN_ES_PDA_MASK) != DCAN_ES_PDA_MASK);

                /* Update the state information */
                ptrCanMCB->state = CAN_DriverState_SLEEP;

            }
            else
            {
                *errCode = CAN_EINVAL;
                retVal = MINUS_ONE;
            }
            break;
        }
        default:
        {
            /* Option is NOT supported */
            *errCode = CAN_EINVAL;
            retVal = MINUS_ONE;
            break;
        }
    }
endSetOptions:
    return retVal;
}

