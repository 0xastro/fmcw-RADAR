/**
 *   @file  uartsci_edma.c
 *
 *   @brief
 *      The file implements the UART driver EDMA Interface functionality
 *      which is available only for the C674 i.e. the DSS.
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
#ifdef SUBSYS_DSS

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/uart/include/uartsci.h>

/**************************************************************************
 ************************** Local Structures ******************************
 **************************************************************************/

/**
 * @brief
 *  SCI Driver EDMA Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the UART DMA.
 */
typedef struct UartSci_DriverEDMA_t
{
    /**
     * @brief   This is a pointer to the hardware configuration.
     */
    UartSci_HwCfg*          ptrHwCfg;

    /**
     * @brief   EDMA Channel configuration which is populated to setup
     * the transfer
     */
    EDMA_channelConfig_t    config;

    /**
     * @brief   Dummy EDMA Param configuration for the last transfer
     */
    EDMA_paramConfig_t      dummyParamConfig;

    /**
     * @brief   EDMA Translated Address for the SCI TD register
     */
    uint32_t                edmaTDAddress;
}UartSci_DriverEDMA;

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/
static void UartSci_TxEDMACallbackFxn(uintptr_t arg, uint8_t transferCompletionCode);

/**************************************************************************
 ************************** UART EDMA Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked after
 *      the Transmit DMA has completed its operation.
 *
 *  @param[in]  arg
 *      Argument registered during callback function which has been set to the
 *      UART Driver Instance.
 *  @param[in]  transferCompletionCode
 *      Transfer completion code
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_TxEDMACallbackFxn(uintptr_t arg, uint8_t transferCompletionCode)
{
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_DriverEDMA* ptrDriverEDMA;

    /* Get the UART Driver: */
    ptrUartSciDriver = (UartSci_Driver*)arg;

    /* Get the DMA Block: */
    ptrDriverEDMA = (UartSci_DriverEDMA*)ptrUartSciDriver->ptrDMAInfo;

    /* Disable the UART Transmit DMA mode */
    UartSci_disableTxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

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
bool UartSci_isEDMAEnabled (UartSci_Driver* ptrUartSciDriver)
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
 *      The function is used to initiate the receive DMA. Since the UART
 *      on the DSS is Transmit only this function should never be invoked
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
 *      Always returns error
 */
int32_t UartSci_initiateRxEDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        dstAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    /* Error: The UART Instance on the DSS is Transmit only. Control should never
     * reach here; the UART SCI driver should have caught this error already */
    DebugP_assert (0);
    return MINUS_ONE;
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
int32_t UartSci_initiateTxEDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        srcAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    UartSci_DriverEDMA*     ptrDriverEDMA;
    EDMA_channelConfig_t*   ptrEDMACfg;
    EDMA_paramConfig_t*     ptrDummyEDMACfg;
    int32_t                 retVal = MINUS_ONE;
    int32_t                 errCode;
    uint32_t                translatedSrcAddress;
    bool                    isTransferComplete = false;

    /* Get the DMA Block: */
    ptrDriverEDMA = (UartSci_DriverEDMA*)ptrUartSciDriver->ptrDMAInfo;

    /* Disable the UART Transmit DMA mode */
    UartSci_disableTxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Translate the source address: */
    translatedSrcAddress = SOC_translateAddress (srcAddress, SOC_TranslateAddr_Dir_TO_EDMA, &errCode);
    if (translatedSrcAddress == SOC_TRANSLATEADDR_INVALID)
    {
        /* Error: The address cannot be translated into EDMA. Setup the return value */
        retVal = errCode;
        goto exit;
    }

    /* Get the pointer to the EDMA configuration */
    ptrEDMACfg      = &ptrDriverEDMA->config;
    ptrDummyEDMACfg = &ptrDriverEDMA->dummyParamConfig;

    /* Populate the remaining fields in the EDMA configuration: */
    ptrEDMACfg->paramSetConfig.sourceAddress = translatedSrcAddress;
    ptrEDMACfg->paramSetConfig.bCount        = numBytes;

    /* Configure the EDMA Channel: */
    errCode = EDMA_configChannel(ptrUartSciDriver->params.edmaHandle, ptrEDMACfg, true);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: Unable to configure the EDMA channel. Setup the return value */
        DebugP_assert(0);
        DebugP_log1 ("Error: UART EDMA configuration failed [Error code %d]\n", errCode);
        retVal = errCode;
        goto exit;
    }

    /* Is Polling Mode enabled? */
    if (pollingMode == true)
    {
        /* Polling Mode: There is no need to register the callback function */
        ptrDummyEDMACfg->transferCompletionCallbackFxn    = NULL;
        ptrDummyEDMACfg->transferCompletionCallbackFxnArg = 0U;
    }
    else
    {
        /* Blocking Mode: Register the callback function to be invoked on interrupt */
        ptrDummyEDMACfg->transferCompletionCallbackFxn    = UartSci_TxEDMACallbackFxn;
        ptrDummyEDMACfg->transferCompletionCallbackFxnArg = (uintptr_t)ptrUartSciDriver;
    }

    /* Configure the PaRAM set: */
    errCode = EDMA_configParamSet (ptrUartSciDriver->params.edmaHandle,
                                   ptrUartSciDriver->params.paramSetId,
                                   ptrDummyEDMACfg);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: Unable to configure the param set. Setup the return value */
        DebugP_assert(0);
        DebugP_log1 ("Error: UART EDMA configuration failed [Error code %d]\n", errCode);
        retVal = errCode;
        goto exit;
    }

    /* Link the PaRAM sets: */
    errCode = EDMA_linkParamSets (ptrUartSciDriver->params.edmaHandle,
                                  ptrEDMACfg->channelId,
                                  ptrUartSciDriver->params.paramSetId);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: Unable to link the param set. Setup the return value */
        DebugP_assert(0);
        DebugP_log1 ("Error: UART EDMA configuration failed [Error code %d]\n", errCode);
        retVal = errCode;
        goto exit;
    }

    /* Enable the UART to operate in Transmit DMA mode */
    UartSci_enableTxDMA(ptrDriverEDMA->ptrHwCfg->ptrSCIRegs);

    /* Are we operating in polled mode or blocking mode? */
    if (pollingMode == true)
    {
        /* Polling Mode: Poll for the DMA completion status here */
        while (isTransferComplete == false)
        {
            errCode = EDMA_isTransferComplete(ptrUartSciDriver->params.edmaHandle,
                                              ptrDriverEDMA->ptrHwCfg->txDMARequestLine,
                                              &isTransferComplete);

            /* Sanity Check: Ensure that there is no EDMA Error. */
            DebugP_assert (errCode == EDMA_NO_ERROR);
        }

        /* Write operation for the driver is complete */
        ptrUartSciDriver->writeCount = ptrUartSciDriver->writeSize;
        ptrUartSciDriver->writeSize  = 0U;
    }
    else
    {
        /* Blocking Mode: Fall through and allow the UART Driver to pend on the semaphore
         * The Tx DMA interrupt has been enabled which will post the semaphore. */
    }

    /* We have successfully completed the EDMA operation */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the EDMA
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
int32_t UartSci_openEDMA (UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg)
{
    int32_t                 retVal = MINUS_ONE;
    int32_t                 errCode;
    UartSci_DriverEDMA*     ptrDriverEDMA = NULL;
    EDMA_channelConfig_t*   ptrEDMACfg;
    EDMA_paramConfig_t*     ptrDummyEDMACfg;

    /* Do we need to use the EDMA? */
    if (ptrUartSciDriver->params.edmaHandle != NULL)
    {
        /* Allocate memory for the DMA Information Block: */
        ptrDriverEDMA = MemoryP_ctrlAlloc((uint32_t)sizeof(UartSci_DriverEDMA), 0);
        if (ptrDriverEDMA == NULL)
        {
            /* Error: Memory allocation failed */
            goto exit;
        }

        /* Initialize the allocated memory: */
        memset ((void *)ptrDriverEDMA, 0, sizeof(UartSci_DriverEDMA));

        /* Populate the DMA Information Block: */
        ptrDriverEDMA->ptrHwCfg      = ptrHwCfg;
        ptrDriverEDMA->edmaTDAddress = SOC_translateAddress ((uint32_t)&ptrHwCfg->ptrSCIRegs->SCITD,
                                                             SOC_TranslateAddr_Dir_TO_EDMA, &errCode);
        if (ptrDriverEDMA->edmaTDAddress == SOC_TRANSLATEADDR_INVALID)
        {
            /* Error: The address cannot be translated into EDMA. This should never occur */
            DebugP_assert (0);
            goto exit;
        }

        /* Get the pointer to the EDMA configuration */
        ptrEDMACfg      = &ptrDriverEDMA->config;
        ptrDummyEDMACfg = &ptrDriverEDMA->dummyParamConfig;

        /*********************************************************************************
         * Initialize the EDMA configuration:
         * - We dont initialize all the fields here but most of them dont change in
         *   the data path.
         *********************************************************************************/
        ptrEDMACfg->channelId                                             = ptrDriverEDMA->ptrHwCfg->txDMARequestLine;
        ptrEDMACfg->channelType                                           = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
        ptrEDMACfg->paramId                                               = ptrDriverEDMA->ptrHwCfg->txDMARequestLine;
        ptrEDMACfg->eventQueueId                                          = 0;
        ptrEDMACfg->transferCompletionCallbackFxn                         = NULL;
        ptrEDMACfg->transferCompletionCallbackFxnArg                      = 0U;
        ptrEDMACfg->paramSetConfig.destinationAddress                     = ptrDriverEDMA->edmaTDAddress;
        ptrEDMACfg->paramSetConfig.aCount                                 = 1U;
        ptrEDMACfg->paramSetConfig.cCount                                 = 1U;
        ptrEDMACfg->paramSetConfig.bCountReload                           = 0U;
        ptrEDMACfg->paramSetConfig.sourceBindex                           = 1U;
        ptrEDMACfg->paramSetConfig.destinationBindex                      = 0U;
        ptrEDMACfg->paramSetConfig.sourceCindex                           = 0U;
        ptrEDMACfg->paramSetConfig.destinationCindex                      = 0U;
        ptrEDMACfg->paramSetConfig.linkAddress                            = EDMA_NULL_LINK_ADDRESS;
        ptrEDMACfg->paramSetConfig.transferType                           = (uint8_t)EDMA3_SYNC_A;
        ptrEDMACfg->paramSetConfig.transferCompletionCode                 = ptrDriverEDMA->ptrHwCfg->txDMARequestLine;
        ptrEDMACfg->paramSetConfig.sourceAddressingMode                   = (uint8_t)EDMA3_ADDRESSING_MODE_LINEAR;
        ptrEDMACfg->paramSetConfig.destinationAddressingMode              = (uint8_t)EDMA3_ADDRESSING_MODE_LINEAR;
        ptrEDMACfg->paramSetConfig.fifoWidth                              = (uint8_t)EDMA3_FIFO_WIDTH_128BIT;
        ptrEDMACfg->paramSetConfig.isStaticSet                            = false;
        ptrEDMACfg->paramSetConfig.isEarlyCompletion                      = false;
        ptrEDMACfg->paramSetConfig.isFinalTransferInterruptEnabled        = false;
        ptrEDMACfg->paramSetConfig.isIntermediateTransferInterruptEnabled = false;
        ptrEDMACfg->paramSetConfig.isFinalChainingEnabled                 = false;
        ptrEDMACfg->paramSetConfig.isIntermediateChainingEnabled          = false;

        /* Copy over the EDMA configuration into the dummy configuration:
         *  This will reduce the code size instead of explicitly initializing all
         *  the fields. We will now modify only the fields which are different */
        memcpy ((void *)&ptrDummyEDMACfg->paramSetConfig, (void*)&ptrEDMACfg->paramSetConfig,
                sizeof(EDMA_paramSetConfig_t));

        /* Modify the fields which are different between the EDMA Transfers:-
         * - Set the bCount to be 0 to indicate that this is a Dummy Transfer
         * - Generate an interrupt once the final transfer is done. */
        ptrDummyEDMACfg->paramSetConfig.bCount                          = 0U;
        ptrDummyEDMACfg->paramSetConfig.isFinalTransferInterruptEnabled = true;

        /* Register the DMA Information Block with the UART Driver: */
        ptrUartSciDriver->ptrDMAInfo = (void*)ptrDriverEDMA;

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
        if (ptrDriverEDMA != NULL)
        {
            /* YES: Free up the memory */
            MemoryP_ctrlFree (ptrDriverEDMA, sizeof(UartSci_DriverEDMA));
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the EDMA for the specific UART Instance
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
int32_t UartSci_closeEDMA (UartSci_Driver* ptrUartSciDriver)
{
    int32_t              retVal = 0;
    UartSci_DriverEDMA*  ptrDriverEDMA;

    /* Were we using the DMA? */
    if (ptrUartSciDriver->ptrDMAInfo != NULL)
    {
        /* Get the pointer to the UART Driver DMA Block: */
        ptrDriverEDMA = (UartSci_DriverEDMA*)ptrUartSciDriver->ptrDMAInfo;

        /* Disable the EDMA channel */
        retVal = EDMA_disableChannel(ptrUartSciDriver->params.edmaHandle,
                                     ptrDriverEDMA->ptrHwCfg->txDMARequestLine,
                                     (uint8_t)EDMA3_CHANNEL_TYPE_DMA);
        if (retVal != EDMA_NO_ERROR)
        {
            goto exit;
        }

        /* Cleanup the memory allocated for the DMA Information. */
        MemoryP_ctrlFree (ptrDriverEDMA, sizeof(UartSci_DriverEDMA));

        /* Deregister the DMA Information: */
        ptrUartSciDriver->ptrDMAInfo = NULL;
    }
exit:
    return retVal;
}

#endif /* SUBSYS_DSS */


