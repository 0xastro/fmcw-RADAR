/**
 *   @file  uartsci_dma.c
 *
 *   @brief
 *      The file implements the UART driver DMA Interface functionality
 *      which is available only for the R4 i.e. the MSS.
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
#ifdef SUBSYS_MSS

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/uart/include/uartsci.h>

/**************************************************************************
 ************************** Local Structures ******************************
 **************************************************************************/

/**
 * @brief
 *  SCI Driver DMA Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the UART DMA.
 */
typedef struct UartSci_DriverDMA_t
{
    /**
     * @brief   This is a pointer to the hardware configuration.
     */
    UartSci_HwCfg*          ptrHwCfg;

    /**
     * @brief   DMA Control Packet Parameters which is used to configure the
     * receive DMA channel. This is applicable & initialized only if the UART
     * instance is configured to operate in DMA mode.
     */
    DMA_CtrlPktParams       rxDMACtrlPktParams;

    /**
     * @brief   Flag which is used to indicate if the receive DMA operation
     * has been completed or not. This is used only in the DMA mode.
     */
    volatile bool           isRxDMACompleted;

    /**
     * @brief   Polling Mode flag:
     *  - TRUE:  Indicates that the callee is polling till the DMA operation
     *           is complete.
     *  - FALSE: Indicates that the callee is blocked till the DMA operation
     *           is complete
     */
    volatile bool           rxPollingMode;

    /**
     * @brief   DMA Control Packet Parameters which is used to configure the
     * transmit DMA channel. This is applicable & initialized only if the UART
     * instance is configured to operate in DMA mode.
     */
    DMA_CtrlPktParams       txDMACtrlPktParams;
}UartSci_DriverDMA;

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

static void UartSci_TxDMACallbackFxn(uint32_t channel, DMA_IntType inttype, void* arg);
static void UartSci_RxDMACallbackFxn(uint32_t channel, DMA_IntType inttype, void* arg);

/**************************************************************************
 ************************** UART DMA Functions ****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked after
 *      the Transmit DMA has completed its operation.
 *
 *  @param[in]  channel
 *      UART Transmit DMA Channel
 *  @param[in]  inttype
 *      DMA Interrupt Type
 *  @param[in]  arg
 *      Argument registered during callback function which has been set to the
 *      UART Driver Instance.
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_TxDMACallbackFxn(uint32_t channel, DMA_IntType inttype, void* arg)
{
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_DriverDMA*  ptrDriverDMA;

    /* Get the UART Driver: */
    ptrUartSciDriver = (UartSci_Driver*)arg;

    /* Get the DMA Block: */
    ptrDriverDMA = (UartSci_DriverDMA*)ptrUartSciDriver->ptrDMAInfo;

    /* Disable the UART Transmit DMA mode */
    UartSci_disableTxDMA(ptrDriverDMA->ptrHwCfg->ptrSCIRegs);

    /* Increment the number of transmit DMA interrupts */
    ptrUartSciDriver->stats.numTxDMAInterrupts++;

    /* Write operation for the driver is complete */
    ptrUartSciDriver->writeCount = ptrUartSciDriver->writeSize;
    ptrUartSciDriver->writeSize  = 0U;

    /* The callee was blocked so post the semaphore to wakeup the callee */
    SemaphoreP_postFromISR (ptrUartSciDriver->writeSem);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked after
 *      the Receive DMA has completed its operation.
 *
 *  @param[in]  channel
 *      UART Transmit DMA Channel
 *  @param[in]  inttype
 *      DMA Interrupt Type
 *  @param[in]  arg
 *      Argument registered during callback function which has been set to the
 *      UART Driver Instance.
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_RxDMACallbackFxn(uint32_t channel, DMA_IntType inttype, void* arg)
{
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_DriverDMA*  ptrDriverDMA;

    /* Get the UART Driver: */
    ptrUartSciDriver = (UartSci_Driver*)arg;

    /* Get the DMA Block: */
    ptrDriverDMA = (UartSci_DriverDMA*)ptrUartSciDriver->ptrDMAInfo;

    /* Disable the UART Receive DMA mode */
    UartSci_disableRxDMA(ptrDriverDMA->ptrHwCfg->ptrSCIRegs);

    /* Increment the number of receive DMA interrupts */
    ptrUartSciDriver->stats.numRxDMAInterrupts++;

    /* Read operation for the driver is complete */
    ptrUartSciDriver->readCount = ptrUartSciDriver->readSize;
    ptrUartSciDriver->readSize  = 0U;

    /* The callee was blocked so post the semaphore to wakeup the callee */
    SemaphoreP_postFromISR (ptrUartSciDriver->readSem);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function returns the status of the DMA configuration.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      true    - DMA services are enabled
 *  @retval
 *      false   - DMA services are not enabled
 */
bool UartSci_isDMAEnabled (UartSci_Driver* ptrUartSciDriver)
{
    bool bIsEnabled;

    /* Is the DMA Information Block registered? */
    if (ptrUartSciDriver->ptrDMAInfo != NULL)
    {
        /* YES: DMA has been configured and can be used */
        bIsEnabled = true;
    }
    else
    {
        /* NO: DMA has not been configured and the services are not available. */
        bIsEnabled = false;
    }
    return bIsEnabled;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initiate the receive DMA
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  dstAddress
 *      Destination address of the buffer where the data is to be placed
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to true to indicate that we need to poll for completion else
 *      set to false
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error Code  -   <0
 */
int32_t UartSci_initiateRxDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        dstAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    UartSci_DriverDMA*  ptrDriverDMA;
    int32_t             retVal;
    int32_t             errCode    = 0;
    uint32_t            pollStatus = 0U;

    /* Get the DMA Block: */
    ptrDriverDMA = (UartSci_DriverDMA*)ptrUartSciDriver->ptrDMAInfo;

    /* Disable the UART  Receive DMA mode */
    UartSci_disableRxDMA(ptrDriverDMA->ptrHwCfg->ptrSCIRegs);

    /* Most of the fields for the Receive DMA Control packet are initialized.
     * Only setup the data fields which change on a per DMA Transaction basis */
    ptrDriverDMA->rxDMACtrlPktParams.destAddr     = dstAddress;
    ptrDriverDMA->rxDMACtrlPktParams.frameXferCnt = numBytes;

    /* Setup the channel parameters: */
    retVal = DMA_setChannelParams(ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.rxDMAChannel,
                                  &ptrDriverDMA->rxDMACtrlPktParams);
    if (retVal != 0)
    {
        /* Error: Unable to set the DMA Channel parameters */
        goto exit;
    }

    /* Setup the trigger for the channel to be done by the Hardware */
    retVal = DMA_enableChannel(ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.rxDMAChannel,
                               DMA_ChTriggerType_HW);
    if (retVal != 0)
    {
        /* Error: Unable to enable the DMA channel */
        goto exit;
    }

    /* Are we operating in Polling or Blocking mode? */
    if (pollingMode == true)
    {
        /* Polling Mode: Disable the DMA interrupt */
        DMA_disableInterrupt (ptrUartSciDriver->params.dmaHandle,
                              ptrUartSciDriver->params.rxDMAChannel,
                              DMA_IntType_BTC);
    }
    else
    {
        /* Blocking Mode: Register for Receive DMA interrupts for Block Transfers */
        DMA_enableInterrupt (ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.rxDMAChannel,
                             DMA_IntType_BTC,
                             UartSci_RxDMACallbackFxn,
                             (void*)ptrUartSciDriver);
    }

    /* Enable the UART to operate in Receive DMA mode */
    UartSci_enableRxDMA(ptrDriverDMA->ptrHwCfg->ptrSCIRegs);

    /* Are we operating in polled mode or blocking mode? */
    if (pollingMode == true)
    {
        /* Polling Mode: Poll for the DMA completion status here */
        while (pollStatus == 0U)
        {
            pollStatus = DMA_getChannelInterruptFlag (ptrUartSciDriver->params.dmaHandle,
                                                      ptrUartSciDriver->params.rxDMAChannel,
                                                      DMA_IntType_BTC,
                                                      &errCode);
        }

        /* Control comes here implies that the DMA transfer is complete. Acknowlege and
         * clear the interrupt status flag. */
        DMA_clearChannelInterruptFlag (ptrUartSciDriver->params.dmaHandle,
                                       ptrUartSciDriver->params.rxDMAChannel,
                                       DMA_IntType_BTC);

        /* Read operation for the driver is complete */
        ptrUartSciDriver->readCount = ptrUartSciDriver->readSize;
        ptrUartSciDriver->readSize  = 0U;
    }
    else
    {
        /* Blocking Mode: Fall through and allow the UART Driver to pend on the semaphore
         * The Rx DMA interrupt has been enabled which will post the semaphore. */
    }

    /* Setup the return value: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initiate the DMA for the UART transmit
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  srcAddress
 *      Source address of the buffer which is to be transmitted
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to true to indicate that we need to poll for completion else
 *      set to false
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error Code  -   <0
 */
int32_t UartSci_initiateTxDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        srcAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    UartSci_DriverDMA*  ptrDriverDMA;
    int32_t             retVal;
    uint32_t            pollStatus = 0U;
    int32_t             errCode    = 0;

    /* Get the DMA Block: */
    ptrDriverDMA = (UartSci_DriverDMA*)ptrUartSciDriver->ptrDMAInfo;

    /* Disable the UART Transmit DMA mode */
    UartSci_disableTxDMA(ptrDriverDMA->ptrHwCfg->ptrSCIRegs);

    /* Most of the fields for the Transmit DMA Control packet are initialized.
     * Only setup the data fields which change on a per DMA Transaction basis */
    ptrDriverDMA->txDMACtrlPktParams.srcAddr      = srcAddress;
    ptrDriverDMA->txDMACtrlPktParams.frameXferCnt = numBytes;

    /* Setup the channel parameters: */
    retVal = DMA_setChannelParams(ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.txDMAChannel,
                                  &ptrDriverDMA->txDMACtrlPktParams);
    if (retVal != 0)
    {
        /* Error: Unable to set the DMA Channel parameters */
        goto exit;
    }

    /* Setup the trigger for the channel to be done by the Hardware */
    retVal = DMA_enableChannel(ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.txDMAChannel,
                               DMA_ChTriggerType_HW);
    if (retVal != 0)
    {
        /* Error: Unable to enable the DMA channel */
        goto exit;
    }

    /* Are we operating in Polling or Blocking mode? */
    if (pollingMode == true)
    {
        /* Polling Mode: Disable the DMA interrupt */
        DMA_disableInterrupt (ptrUartSciDriver->params.dmaHandle,
                              ptrUartSciDriver->params.txDMAChannel,
                              DMA_IntType_BTC);
    }
    else
    {
        /* Blocking Mode: Register for Transmit DMA interrupts for Block Transfers */
        DMA_enableInterrupt (ptrUartSciDriver->params.dmaHandle,
                             ptrUartSciDriver->params.txDMAChannel,
                             DMA_IntType_BTC,
                             UartSci_TxDMACallbackFxn,
                             (void*)ptrUartSciDriver);
    }

    /* Enable the UART to operate in Transmit DMA mode */
    UartSci_enableTxDMA(ptrDriverDMA->ptrHwCfg->ptrSCIRegs);

    /* Are we operating in polled mode or blocking mode? */
    if (pollingMode == true)
    {
        /* Polling Mode: Poll for the DMA completion status here */
        while (pollStatus == 0U)
        {
            pollStatus = DMA_getChannelInterruptFlag (ptrUartSciDriver->params.dmaHandle,
                                                      ptrUartSciDriver->params.txDMAChannel,
                                                      DMA_IntType_BTC,
                                                      &errCode);
        }

        /* Control comes here implies that the DMA transfer is complete. Acknowlege and
         * clear the interrupt status flag. */
        DMA_clearChannelInterruptFlag (ptrUartSciDriver->params.dmaHandle,
                                       ptrUartSciDriver->params.txDMAChannel,
                                       DMA_IntType_BTC);

        /* Write operation for the driver is complete */
        ptrUartSciDriver->writeCount = ptrUartSciDriver->writeSize;
        ptrUartSciDriver->writeSize  = 0U;
    }
    else
    {
        /* Blocking Mode: Fall through and allow the UART Driver to pend on the semaphore
         * The Tx DMA interrupt has been enabled which will post the semaphore. */
    }

    /* Setup the return value: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the DMA
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART SCI Driver
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART SCI Hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t UartSci_openDMA (UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg)
{
    int32_t             errCode;
    int32_t             retVal = MINUS_ONE;
    UartSci_DriverDMA*  ptrDriverDMA = NULL;

    /* Do we need to use the DMA? */
    if (ptrUartSciDriver->params.dmaHandle != NULL)
    {
        /* Allocate memory for the DMA Information Block: */
        ptrDriverDMA = MemoryP_ctrlAlloc((uint32_t)sizeof(UartSci_DriverDMA), 0);
        if (ptrDriverDMA == NULL)
        {
            /* Error: Memory allocation failed */
            goto exit;
        }

        /* Initialize the allocated memory: */
        memset ((void *)ptrDriverDMA, 0, sizeof(UartSci_DriverDMA));

        /* Populate the DMA Information Block: */
        ptrDriverDMA->ptrHwCfg = ptrHwCfg;

        /* Register the DMA Information Block with the UART Driver: */
        ptrUartSciDriver->ptrDMAInfo = (void*)ptrDriverDMA;

        /* YES: Assign the Transmit DMA channel to the correct DMA request line */
        if (DMA_assignChannel (ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.txDMAChannel,
                               ptrHwCfg->txDMARequestLine, &errCode) == DMA_CHANNEL_NONE)
        {
            /* Error: DMA Channel assignment failed, unfortuantely the error code cannot be propogated
             * because the UART MCPI does not allow the error code to be propogated. But we can log
             * this error */
            DebugP_log2 ("Error: UART Driver (%p) Tx DMA Mapping Failed [Error code %d]\n",
                          ptrHwCfg->ptrSCIRegs, errCode);
            goto exit;
        }

        /* YES: Assign the Receive DMA channel to the correct DMA request line */
        if (DMA_assignChannel (ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.rxDMAChannel,
                               ptrHwCfg->rxDMARequestLine, &errCode) == DMA_CHANNEL_NONE)
        {
            /* Error: DMA Channel assignment failed, unfortuantely the error code cannot be propogated
             * because the UART MCPI does not allow the error code to be propogated. But we can log
             * this error */
            DebugP_log2 ("Error: UART Driver (%p) Rx DMA Mapping Failed [Error code %d]\n",
                         ptrHwCfg->ptrSCIRegs, errCode);
            goto exit;
        }

        /*********************************************************************************
         * Initialize the DMA Control Packet for Receive:
         * - We dont initialize all the fields here but most of them dont change in
         *   the data path.
         *********************************************************************************/
        ptrDriverDMA->rxDMACtrlPktParams.srcAddr              = (uint32_t)&ptrHwCfg->ptrSCIRegs->SCIRD;
        ptrDriverDMA->rxDMACtrlPktParams.elemXferCnt          = 1U;
        ptrDriverDMA->rxDMACtrlPktParams.nextChannel          = DMA_CHANNEL_NONE;
        ptrDriverDMA->rxDMACtrlPktParams.srcElemSize          = DMA_ElemSize_8bit;
        ptrDriverDMA->rxDMACtrlPktParams.destElemSize         = DMA_ElemSize_8bit;
        ptrDriverDMA->rxDMACtrlPktParams.autoInitiation       = 0U;
        ptrDriverDMA->rxDMACtrlPktParams.xferType             = DMA_XferType_Frame;
        ptrDriverDMA->rxDMACtrlPktParams.srcAddrMode          = DMA_AddrMode_Constant;
        ptrDriverDMA->rxDMACtrlPktParams.destAddrMode         = DMA_AddrMode_PostIncrement;
        ptrDriverDMA->rxDMACtrlPktParams.srcElemIndexOffset   = 0U;
        ptrDriverDMA->rxDMACtrlPktParams.destElemIndexOffset  = 0U;
        ptrDriverDMA->rxDMACtrlPktParams.srcFrameIndexOffset  = 0U;
        ptrDriverDMA->rxDMACtrlPktParams.destFrameIndexOffset = 0U;

        /*********************************************************************************
         * Initialize the DMA Control Packet for Transmit:
         * - We dont initialize all the fields here but most of them dont change in
         *   the data path.
         *********************************************************************************/
        ptrDriverDMA->txDMACtrlPktParams.destAddr             = (uint32_t)&ptrHwCfg->ptrSCIRegs->SCITD;
        ptrDriverDMA->txDMACtrlPktParams.elemXferCnt          = 1U;
        ptrDriverDMA->txDMACtrlPktParams.nextChannel          = DMA_CHANNEL_NONE;
        ptrDriverDMA->txDMACtrlPktParams.srcElemSize          = DMA_ElemSize_8bit;
        ptrDriverDMA->txDMACtrlPktParams.destElemSize         = DMA_ElemSize_8bit;
        ptrDriverDMA->txDMACtrlPktParams.autoInitiation       = 0U;
        ptrDriverDMA->txDMACtrlPktParams.xferType             = DMA_XferType_Frame;
        ptrDriverDMA->txDMACtrlPktParams.srcAddrMode          = DMA_AddrMode_PostIncrement;
        ptrDriverDMA->txDMACtrlPktParams.destAddrMode         = DMA_AddrMode_Constant;
        ptrDriverDMA->txDMACtrlPktParams.srcElemIndexOffset   = 0U;
        ptrDriverDMA->txDMACtrlPktParams.destElemIndexOffset  = 0U;
        ptrDriverDMA->txDMACtrlPktParams.srcFrameIndexOffset  = 0U;
        ptrDriverDMA->txDMACtrlPktParams.destFrameIndexOffset = 0U;

        /* Setup the return value: */
        retVal = 0;
    }
    else
    {
        /* DMA was not configured: Configuration was completed successfully. */
        retVal = 0;
    }

exit:
    /* Was there an error? */
    if (retVal < 0)
    {
        /* YES: Do we need to clean up the allocated memory */
        if (ptrDriverDMA != NULL)
        {
            /* YES: Free up the memory */
            MemoryP_ctrlFree (ptrDriverDMA, sizeof(UartSci_DriverDMA));
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the DMA for the specific UART Instance
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART driver for which the DMA is being closed
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t UartSci_closeDMA (UartSci_Driver* ptrUartSciDriver)
{
    int32_t             retVal = 0;
    UartSci_DriverDMA*  ptrDriverDMA;

    /* Were we using the DMA? */
    if (ptrUartSciDriver->ptrDMAInfo != NULL)
    {
        /* Get the pointer to the UART Driver DMA Block: */
        ptrDriverDMA = (UartSci_DriverDMA*)ptrUartSciDriver->ptrDMAInfo;

        /* Disable the interrupt: */
        DMA_disableInterrupt (ptrUartSciDriver->params.dmaHandle,
                              ptrUartSciDriver->params.txDMAChannel,
                              DMA_IntType_BTC);

        /* Disable the channel: */
        DMA_disableChannel (ptrUartSciDriver->params.dmaHandle,
                            ptrUartSciDriver->params.txDMAChannel,
                            DMA_ChTriggerType_HW);

        /* Free the Transmit DMA channel from the DMA request line */
        DMA_freeChannel (ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.txDMAChannel,
                         ptrDriverDMA->ptrHwCfg->txDMARequestLine);

        /* Disable the interrupt: */
        DMA_disableInterrupt (ptrUartSciDriver->params.dmaHandle,
                              ptrUartSciDriver->params.rxDMAChannel,
                              DMA_IntType_BTC);

        /* Disable the channel: */
        DMA_disableChannel (ptrUartSciDriver->params.dmaHandle,
                            ptrUartSciDriver->params.rxDMAChannel,
                            DMA_ChTriggerType_HW);

        /* Free the Receive DMA channel from the DMA request line */
        DMA_freeChannel (ptrUartSciDriver->params.dmaHandle, ptrUartSciDriver->params.rxDMAChannel,
                         ptrDriverDMA->ptrHwCfg->rxDMARequestLine);

        /* Cleanup the memory allocated for the DMA Information. */
        MemoryP_ctrlFree (ptrDriverDMA, sizeof(UartSci_DriverDMA));

        /* Deregister the DMA Information: */
        ptrUartSciDriver->ptrDMAInfo = NULL;
    }
    return retVal;
}

#endif /* SUBSYS_MSS */
