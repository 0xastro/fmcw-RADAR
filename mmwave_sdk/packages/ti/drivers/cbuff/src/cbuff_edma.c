/**
 *   @file  cbuff_edma.c
 *
 *   @brief
 *      The file implements the CBUFF Driver EDMA3 Interface
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
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/cbuff/include/cbuff_internal.h>
#include <ti/drivers/cbuff/include/reg_cbuff.h>
#include <ti/drivers/edma/edma.h>

/**************************************************************************
 ************************ CBUFF EDMA3 Local Functions *********************
 **************************************************************************/
static int32_t CBUFF_setupEDMAShadowLink
(
    CBUFF_Session*                          ptrSession,
    uint8_t                                 chId,
    uint16_t                                linkChId,
    EDMA_paramSetConfig_t*                  config,
    EDMA_transferCompletionCallbackFxn_t    transferCompletionCallbackFxn
);
static void CBUFF_displayDebugInfo
(
    CBUFF_Session* ptrSession,
    uint32_t       srcAddress,
    uint32_t       transferSize,
    bool           isLast
);

/**************************************************************************
 *************************** CBUFF EDMA3 Functions ************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to setup the EDMA channel with the
 *      specified configuration
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[in]  chId
 *      EDMA Channel Identifier
 *  @param[in]  linkChId
 *      EDMA Channel Identifer to be linked
 *  @param[in]  config
 *      EDMA Parameter set configuration
 *  @param[in]  transferCompletionCallbackFxn
 *      Option Transfer completion callback function to be invoked once
 *      the transfer is complete.
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - EDMA No Error (EDMA_NO_ERR)
 *  @retval
 *      Error   - EDMA Error code
 */
static int32_t CBUFF_setupEDMAShadowLink
(
    CBUFF_Session*                          ptrSession,
    uint8_t                                 chId,
    uint16_t                                linkChId,
    EDMA_paramSetConfig_t*                  config,
    EDMA_transferCompletionCallbackFxn_t    transferCompletionCallbackFxn
)
{
    EDMA_paramConfig_t  paramConfig;
    int32_t             errCode;

    /* Populate the param configuration: */
    paramConfig.paramSetConfig                      = *config;
    paramConfig.transferCompletionCallbackFxnArg    = (uintptr_t)ptrSession->ptrDriverMCB;
    paramConfig.transferCompletionCallbackFxn       = transferCompletionCallbackFxn;
    errCode = EDMA_configParamSet(ptrSession->sessionCfg.edmaHandle, linkChId, &paramConfig);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: This should never fail */
        DebugP_assert (0);
        goto exit;
    }

    /* Link the channels: */
    errCode = EDMA_linkParamSets(ptrSession->sessionCfg.edmaHandle, (uint16_t)chId, linkChId);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: This should never fail */
        DebugP_assert (0);
        goto exit;
    }

    /* Link the Parameter sets: */
    errCode = EDMA_linkParamSets(ptrSession->sessionCfg.edmaHandle, linkChId, linkChId);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: This should never fail */
        DebugP_assert (0);
        goto exit;
    }

exit:
    return(errCode);
}

/**
 *  @b Description
 *  @n
 *      The function is used to display for debug the information
 *      which has been programmed into EDMA channels.
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[in]  srcAddress
 *      Source Address where the data to be transfered is located
 *  @param[in]  transferSize
 *      Size of the data to be transferred to the FIFO
 *  @param[in]  isLast
 *      Flag which if set indicates this is the last transaction
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_displayDebugInfo
(
    CBUFF_Session*      ptrSession,
    uint32_t            srcAddress,
    uint32_t            transferSize,
    bool                isLast
)
{
#if 0
#if DebugP_LOG_ENABLED
    char*           srcAddressDisplayString = NULL;
    uint16_t        index;
    uint32_t        adcBufferBaseAddress;
    uint32_t        adcBufferSize;

    /* Setup the base address & size for the ADC Buffer; this is dependent upon the platform. */
#ifdef SOC_XWR16XX
    #ifdef SUBSYS_MSS
        adcBufferBaseAddress = EDMA3_DSS_ADCBUFF_BASE;
        adcBufferSize        = SOC_XWR16XX_MSS_ADCBUF_SIZE;
    #else
        adcBufferBaseAddress = EDMA3_DSS_ADCBUFF_BASE;
        adcBufferSize        = SOC_XWR16XX_DSS_ADCBUF_SIZE;
    #endif
#elif defined (SOC_XWR18XX)
    #ifdef SUBSYS_MSS
        adcBufferBaseAddress = EDMA3_DSS_ADCBUFF_BASE;
        adcBufferSize        = SOC_ADCBUF_SIZE;
    #else
        adcBufferBaseAddress = EDMA3_DSS_ADCBUFF_BASE;
        adcBufferSize        = SOC_ADCBUF_SIZE;
    #endif
#elif defined (SOC_XWR68XX)
    #ifdef SUBSYS_MSS
        adcBufferBaseAddress = EDMA3_DSS_ADCBUFF_BASE;
        adcBufferSize        = SOC_ADCBUF_SIZE;
    #else
        adcBufferBaseAddress = EDMA3_DSS_ADCBUFF_BASE;
        adcBufferSize        = SOC_ADCBUF_SIZE;
    #endif
#elif defined (SOC_XWR14XX)
    adcBufferBaseAddress = EDMA3_DSS_ADCBUFF_BASE;
    adcBufferSize        = SOC_XWR14XX_MSS_ADCBUF_SIZE;
#else
    #error "Error: Unsupported platform" 
#endif

    /* Get the debug string for the display: */
    switch (srcAddress)
    {
        case EDMA3_DSS_CQ0BUFF_BASE:
        case EDMA3_DSS_CQ1BUFF_BASE:
        case EDMA3_DSS_CQ2BUFF_BASE:
        {
            srcAddressDisplayString = "Chirp Quality";
            break;
        }
        case EDMA3_DSS_CP0_BASE:
        case EDMA3_DSS_CP1_BASE:
        case EDMA3_DSS_CP2_BASE:
        case EDMA3_DSS_CP3_BASE:
        {
            srcAddressDisplayString = "Chirp Parameter";
            break;
        }
        default:
        {
            /**********************************************************************
             * ADC Buffer:
             **********************************************************************/
            if ((srcAddress >= adcBufferBaseAddress) && srcAddress <= (adcBufferBaseAddress + adcBufferSize))
            {
                srcAddressDisplayString = "ADC Buffer";
                break;
            }

            /**********************************************************************
             * User Buffer:
             **********************************************************************/
            for (index = 0U; index < CBUFF_MAX_USER_BUFFER; index++)
            {
                if (ptrSession->sessionCfg.userBufferInfo[index].size != 0U)
                {
                    /* YES: Do we have a match */
                    if (srcAddress == ptrSession->sessionCfg.userBufferInfo[index].address)
                    {
                        srcAddressDisplayString = "User Buffer";
                        break;
                    }
                }
            }
            if (index < CBUFF_MAX_USER_BUFFER)
            {
                break;
            }

            /* Are we operating in custom header? */
            if (ptrSession->sessionCfg.headerMode == CBUFF_HeaderMode_CUSTOM)
            {
                /**********************************************************************
                 * ADC Buffer Header:
                 **********************************************************************/
                for (index = 0U; index < SYS_COMMON_NUM_RX_CHANNEL; index++)
                {
                    if (ptrSession->sessionCfg.headerCfg.adcBufferHeader[index].size != 0U)
                    {
                        /* YES: Do we have a match */
                        if (srcAddress == ptrSession->sessionCfg.headerCfg.adcBufferHeader[index].address)
                        {
                            srcAddressDisplayString = "ADC Buffer Header";
                            break;
                        }
                    }
                }
                if (index < SYS_COMMON_NUM_RX_CHANNEL)
                {
                    break;
                }

                /**********************************************************************
                 * User Buffer Header:
                 **********************************************************************/
                for (index = 0U; index < CBUFF_MAX_USER_BUFFER; index++)
                {
                    if (ptrSession->sessionCfg.headerCfg.userBufferHeader[index].size != 0U)
                    {
                        /* YES: Do we have a match */
                        if (srcAddress == ptrSession->sessionCfg.headerCfg.userBufferHeader[index].address)
                        {
                            srcAddressDisplayString = "User Buffer Header";
                            break;
                        }
                    }
                }
                if (index < CBUFF_MAX_USER_BUFFER)
                {
                    break;
                }

                /**********************************************************************
                 * Continuous Header:
                 **********************************************************************/
                if (srcAddress == ptrSession->sessionCfg.headerCfg.continuousHeader.address)
                {
                    srcAddressDisplayString = "Continuous Header";
                    break;
                }

                /**********************************************************************
                 * Chirp Profile Header:
                 **********************************************************************/
                if (srcAddress == ptrSession->sessionCfg.headerCfg.cpHeader.address)
                {
                    srcAddressDisplayString = "CP Header";
                    break;
                }

                /**********************************************************************
                 * Chirp Quality Header:
                 **********************************************************************/
                if (srcAddress == ptrSession->sessionCfg.headerCfg.cqHeader.address)
                {
                    srcAddressDisplayString = "CQ Header";
                    break;
                }
            }
            break;
        }
    }

    /* We should have found a matching string: */
    if (srcAddressDisplayString == NULL)
    {
        DebugP_assert (0);
        srcAddressDisplayString = "Error [Unknown Address in CBUFF EDMA]";
    }


    /* Debug Message: */
    DebugP_log3 ("Debug: Entry %d EDMA3 Source Address: %s [0x%x]\n",
                (uintptr_t)ptrSession->edmaChannelCount, (uintptr_t)srcAddressDisplayString, (uintptr_t)srcAddress);
    DebugP_log2 ("Debug: Transfer Size: %d bytes [%s]\n",
                (uintptr_t)transferSize, (uintptr_t)((isLast == 0U) ? "Chained" : "Last"));
#else
    return;
#endif
#endif
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the EDMA3 Transfer from the specified
 *      source address to the CBUFF FIFO.
 *
 *  @param[in]  ptrSession
 *      Pointer to the Session
 *  @param[in]  srcAddress
 *      Source Address where the data to be transfered is located
 *  @param[in]  transferSize
 *      Size of the data to be transferred to the FIFO
 *  @param[in]  isLast
 *      Flag which if set indicates this is the last transaction
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - < 0 [Error code: EDMA/CBUFF Error]
 */
int32_t CBUFF_configEDMA
(
    CBUFF_Session*      ptrSession,
    uint32_t            srcAddress,
    uint32_t            transferSize,
    bool                isLast
)
{
    uint8_t                 chId;
    uint8_t                 chainChannel;
    uint16_t                linkChannel;
    uint16_t                prevLinkChannel;
    EDMA_channelConfig_t    config;
    CBUFF_EDMAChannelCfg    cbuffEDMAChannelCfg;
    uint8_t                 edmaChannelCount;
    int32_t                 errCode;
    uint8_t                 edmaInstance;
    bool                    isEventTriggered;
    CBUFF_EDMAInfo          edmaInfo;

    /* Initialize the error code: */
    errCode = 0;

    /* Sanity Check: Was a valid transfer size specified? */
    if (transferSize == 0)
    {
        /* No transfer size is specified. This is not an error but we will not
         * configure the EDMA Transfer because there is nothing to be transfered */
        goto exit;
    }

    /* Keep track of the EDMA Channels which have been configured. */
    edmaChannelCount = ptrSession->edmaChannelCount;

    /* Sanity Check: Do we have space to record this information */
    DebugP_assert (edmaChannelCount < CBUFF_EDMA_MAX_NUM_CHANNELS);

    /* Initialize the EDMA Information: */
    memset ((void*)&edmaInfo, 0, sizeof(CBUFF_EDMAInfo));

    /* Is this the first EDMA Channel being allocated? */
    if (edmaChannelCount == 0U)
    {
        /* YES: Setup the flag */
        edmaInfo.isFirstEDMAChannel = true;
    }
    else
    {
        /* NO: Reset the flag */
        edmaInfo.isFirstEDMAChannel = false;
    }
    edmaInfo.edmaHandle = ptrSession->sessionCfg.edmaHandle;
    edmaInfo.dmaNum     = ptrSession->dmaNum;

    /* Allocate an EDMA channel using the application registered callback function */
    errCode = ptrSession->sessionCfg.allocateEDMAChannelFxn (&edmaInfo, &cbuffEDMAChannelCfg);
    if (errCode < 0)
    {
        /* Error: Unable to allocate an EDMA channel. Setup the error code and exit */
        errCode = CBUFF_EDMA_FAIL;
        goto exit;
    }

    /* First Channel: This is a special case and the we always need to have this hooked up
     * via the CBUFF EDMA Physical channel identifier. Applications should know about this
     * and also because we still want the application to allocate the shadow channels */
    if (edmaChannelCount == 0)
    {
        bool isValidEDMAChannel = false;

        /* YES: Cycle through all the EDMA Instances and ensure that the physical channel
         * used is correct one. */
        for (edmaInstance = 0; edmaInstance < EDMA_NUM_CC; edmaInstance++)
        {
            /* Sanity Check: Did the application give us the correct CBUFF EDMA Channel identifier. */
            if (cbuffEDMAChannelCfg.chainChannelsId == gCBUFFHwAttribute.cbuffChannelId[edmaInstance][edmaInfo.dmaNum])
            {
                /* YES: Excellent we are done the application gave us the correct EDMA channel identifier */
                isValidEDMAChannel = true;
                break;
            }
        }
        /* Was there a match? */
        if (isValidEDMAChannel == false)
        {
            /* NO: This is an invalid use case; we could return an invalid argument but modified
             * the error code to clearly show that the invalid EDMA allocation was incorrect. */
            errCode = CBUFF_EDMA_FAIL;
            goto exit;
        }
    }

    /* Track the EDMA Information which is being added */
    memcpy ((void *)&ptrSession->edmaTrackingEntry[edmaChannelCount].cbuffEDMAChannelCfg, (void*)&cbuffEDMAChannelCfg,
            sizeof(CBUFF_EDMAChannelCfg));
    ptrSession->edmaTrackingEntry[edmaChannelCount].transferSize = transferSize;
    ptrSession->edmaTrackingEntry[edmaChannelCount].srcAddress   = srcAddress;

    /* Setup the channel identifier which is to be used. */
    chId = cbuffEDMAChannelCfg.chainChannelsId;

    /* Dump the debug information: */
    CBUFF_displayDebugInfo (ptrSession, srcAddress, transferSize, isLast);

    /* Populate the EDMA Channel configuration: */
    config.channelId                     = chId;
    config.channelType                   = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId                       = chId;
    config.eventQueueId                  = 0;
    config.transferCompletionCallbackFxn = NULL;

    /* Populate the EDMA Channel Parameter Set Configuration: */
    config.paramSetConfig.sourceAddress                          = srcAddress;
    config.paramSetConfig.destinationAddress                     = (uint32_t)gCBUFFHwAttribute.fifoBaseAddress;
    config.paramSetConfig.aCount                                 = (uint16_t)((2U * transferSize) & 0xFFFFU);
    config.paramSetConfig.bCount                                 = (uint16_t)ptrSession->numChirpsPerFrame;
    config.paramSetConfig.cCount                                 = (uint16_t)1;
    config.paramSetConfig.bCountReload                           = config.paramSetConfig.bCount;
    config.paramSetConfig.sourceBindex                           = 0;
    config.paramSetConfig.destinationBindex                      = 0;
    config.paramSetConfig.sourceCindex                           = 0;
    config.paramSetConfig.destinationCindex                      = 0;
    config.paramSetConfig.linkAddress                            = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType                           = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.transferCompletionCode                 = chId;
    config.paramSetConfig.sourceAddressingMode                   = (uint8_t)EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode              = (uint8_t)EDMA3_ADDRESSING_MODE_FIFO_WRAP;
    config.paramSetConfig.fifoWidth                              = (uint8_t)EDMA3_FIFO_WIDTH_128BIT;
    config.paramSetConfig.isStaticSet                            = false;
    config.paramSetConfig.isEarlyCompletion                      = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled        = isLast;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = isLast;
    config.paramSetConfig.isFinalChainingEnabled                 = (!isLast);
    config.paramSetConfig.isIntermediateChainingEnabled          = (!isLast);
    config.transferCompletionCallbackFxnArg                      = (uintptr_t)ptrSession;

    /* cbuff channel is event triggered, chain channels are not */
    if (edmaChannelCount == 0U)
    {
        isEventTriggered = true;
    }
    else
    {
        isEventTriggered = false;
    }

    /* Configure the EDMA Channel: */
    errCode = EDMA_configChannel(ptrSession->sessionCfg.edmaHandle, &config, isEventTriggered);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: EDMA channel configuration should not fail */
        DebugP_assert(0);
        goto exit;
    }

    /* Link the channels: */
    linkChannel = cbuffEDMAChannelCfg.shadowLinkChannelsId;
    errCode = CBUFF_setupEDMAShadowLink(ptrSession, chId, linkChannel, &config.paramSetConfig, NULL);
    if (errCode != EDMA_NO_ERROR)
    {
        /* Error: This should NOT fail and we have already asserted */
        goto exit;
    }

    /* Do we need to chain the channels? We need to skip the the head of the chain. */
    if (edmaChannelCount != 0U)
    {
        /* YES: Get the previous chain & link channel from the saved EDMA configuration */
        chainChannel    = ptrSession->edmaTrackingEntry[edmaChannelCount - 1U].cbuffEDMAChannelCfg.chainChannelsId;
        prevLinkChannel = ptrSession->edmaTrackingEntry[edmaChannelCount - 1U].cbuffEDMAChannelCfg.shadowLinkChannelsId;

        /* Chain the channels */
        errCode = EDMA_chainChannels(ptrSession->sessionCfg.edmaHandle, chainChannel, chId);
        if (errCode != EDMA_NO_ERROR)
        {
            /* Error: This should NOT fail */
            DebugP_assert(0);
            goto exit;
        }

        /* Setup the chain in the linked param set as well */
        errCode = EDMA_chainChannels(ptrSession->sessionCfg.edmaHandle, prevLinkChannel, chId);
        if (errCode != EDMA_NO_ERROR)
        {
            /* Error: This should NOT fail */
            DebugP_assert(0);
            goto exit;
        }
    }

    /* Increment the number of channels which are being setup: */
    ptrSession->edmaChannelCount = edmaChannelCount + 1;

exit:
    return(errCode);
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the EDMA3 channels which have been configured
 *      This is invoked once the CBUFF module is closed
 *
 *  @param[in]  ptrSession
 *      Pointer to the CBUFF Session
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CBUFF_closeEDMA (CBUFF_Session* ptrSession)
{
    uint8_t     index;
    int32_t     errCode;

    /* Sanity Check: Ensure that the back pointer to the driver is valid */
    DebugP_assert (ptrSession->ptrDriverMCB != NULL);

    /* Disable all the chained channels */
    for (index = 0U; index < ptrSession->edmaChannelCount; index++)
    {
        errCode = EDMA_disableChannel(ptrSession->sessionCfg.edmaHandle,
                                      ptrSession->edmaTrackingEntry[index].cbuffEDMAChannelCfg.chainChannelsId,
                                      (uint8_t)EDMA3_CHANNEL_TYPE_DMA);
        if (errCode != EDMA_NO_ERROR)
        {
            /* Error: This should NOT fail */
            DebugP_assert(0);
            goto exit;
        }

        /* Inform the application that the EDMA channel has been freed. */
        ptrSession->sessionCfg.freeEDMAChannelFxn (&ptrSession->edmaTrackingEntry[index].cbuffEDMAChannelCfg);

        /* Reset the tracking entry: */
        memset ((void *)&ptrSession->edmaTrackingEntry[index], 0, sizeof(CBUFF_EDMATrackingEntry));
    }

    /* Reset the EDMA Channel count in the driver; all the channels have been cleaned up. */
    ptrSession->edmaChannelCount = 0;

exit:
    return;
}

