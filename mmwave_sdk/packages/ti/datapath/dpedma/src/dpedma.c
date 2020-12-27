/**
 *   @file  dpedma.c
 *
 *   @brief
 *      EDMA Configuration Utility API implementation.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
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

#include <ti/drivers/edma/edma.h>
#include <ti/drivers/soc/soc.h>
#include <ti/datapath/dpedma/dpedma.h>

/**
 *  @b Description
 *  @n
 *     Utility function for linking EDMA channel with a shadow link paramset
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  chId            EDMA channel id
 *  @param[in]  shadowParamId   EDMA channel shadow id
 *  @param[in]  config          Pointer to EDMA paramset configuration
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *
 *  \ingroup    DPEDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMA_setup_shadow_link
(
    EDMA_Handle     handle,
    uint8_t         chId,
    uint16_t        shadowParamId,
    EDMA_paramSetConfig_t *config,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t       transferCompletionCallbackFxnArg
)
{
    EDMA_paramConfig_t paramConfig;
    int32_t errorCode = EDMA_NO_ERROR;

    paramConfig.paramSetConfig = *config; //this will copy the entire param set config
    paramConfig.transferCompletionCallbackFxn = transferCompletionCallbackFxn;
    paramConfig.transferCompletionCallbackFxnArg = transferCompletionCallbackFxnArg;
    if ((errorCode = EDMA_configParamSet(handle,
                        shadowParamId, &paramConfig)) != EDMA_NO_ERROR)
    {
        //System_printf("Error: EDMA_configParamSet() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if ((errorCode = EDMA_linkParamSets(handle,
                        (uint16_t) chId, shadowParamId)) != EDMA_NO_ERROR)
    {
        //System_printf("Error: EDMA_linkParamSets() failed with error code = %d\n", errorCode);
        goto exit;
    }

    if ((errorCode = EDMA_linkParamSets(handle,
                        shadowParamId, shadowParamId)) != EDMA_NO_ERROR)
    {
        //System_printf("Error: EDMA_linkParamSets() failed with error code = %d\n", errorCode);
        goto exit;
    }

exit:
    return(errorCode);
}


/**
 *  @b Description
 *  @n
 *     EDMA utility function for sync AB type transfers.
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  chanCfg         Pointer to datapath EDMA channel configuration
 *  @param[in]  chainingCfg     Pointer to datapath EDMA channel chaining configuration
 *  @param[in]  syncABCfg       Pointer to syncAB type configuration
 *  @param[in]  isEventTriggered  Flag indicates if the channel is event triggered
 *  @param[in]  isIntermediateTransferCompletionEnabled Set to 'true' if intermedate transfer completion
 *                 indication is to be enabled.
 *  @param[in]  isTransferCompletionEnabled Set to 'true' if final transfer completion
 *                              indication is to be enabled.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
 int32_t DPEDMA_configSyncAB
(
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    DPEDMA_syncABCfg        *syncABCfg,
    bool                    isEventTriggered,
    bool                    isIntermediateTransferCompletionEnabled,
    bool                    isTransferCompletionEnabled,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t               transferCompletionCallbackFxnArg
)
{
    EDMA_channelConfig_t config;
    int32_t errorCode = EDMA_NO_ERROR;

    if((chanCfg == NULL) || (syncABCfg == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    config.channelId = chanCfg->channel;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = chanCfg->channel;
    config.eventQueueId = chanCfg->eventQueue;

    config.paramSetConfig.sourceAddress = SOC_translateAddress(
                                            syncABCfg->srcAddress,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);

    config.paramSetConfig.destinationAddress =SOC_translateAddress(
                                            syncABCfg->destAddress,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);


    config.paramSetConfig.aCount = syncABCfg->aCount;
    config.paramSetConfig.bCount = syncABCfg->bCount;
    config.paramSetConfig.cCount = syncABCfg->cCount;
    config.paramSetConfig.bCountReload = 0; //config.paramSetConfig.bCount;

    config.paramSetConfig.sourceBindex = syncABCfg->srcBIdx;
    config.paramSetConfig.destinationBindex = syncABCfg->dstBIdx;

    config.paramSetConfig.sourceCindex = syncABCfg->srcCIdx;
    config.paramSetConfig.destinationCindex = syncABCfg->dstCIdx;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_AB;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;
    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled = isTransferCompletionEnabled;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = 
        isIntermediateTransferCompletionEnabled;

    if(chainingCfg == NULL)
    {
        config.paramSetConfig.transferCompletionCode = chanCfg->channel;
        config.paramSetConfig.isFinalChainingEnabled = false;
        config.paramSetConfig.isIntermediateChainingEnabled = false;
    }
    else
    {
        config.paramSetConfig.transferCompletionCode = chainingCfg->chainingChan;
        config.paramSetConfig.isFinalChainingEnabled =
            chainingCfg->isFinalChainingEnabled;
        config.paramSetConfig.isIntermediateChainingEnabled =
            chainingCfg->isIntermediateChainingEnabled;
    }
    config.transferCompletionCallbackFxn = transferCompletionCallbackFxn;

    if (transferCompletionCallbackFxn != NULL) {
        config.transferCompletionCallbackFxnArg = (uintptr_t) transferCompletionCallbackFxnArg;
    }

    if ((errorCode = EDMA_configChannel(handle, &config, isEventTriggered)) != EDMA_NO_ERROR)
    {
        goto exit;
    }

    errorCode = DPEDMA_setup_shadow_link(handle, chanCfg->channel, chanCfg->channelShadow,
        &config.paramSetConfig, config.transferCompletionCallbackFxn, transferCompletionCallbackFxnArg);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *     EDMA utility function for sync A type transfers.
 *     Here single frame means (EDMA) C count is 1.
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  chanCfg         Pointer to datapath EDMA channel configuration
 *  @param[in]  chainingCfg     Pointer to datapath EDMA channel chaining configuration
 *  @param[in]  syncACfg        Pointer to syncA type configuration
 *  @param[in]  isEventTriggered  Flag indicates if the channel is event triggered
 *  @param[in]  isIntermediateTransferInterruptEnabled Set to 'true' if intermediate transfer completion
 *                 indication is to be enabled.
 *  @param[in]  isTransferCompletionEnabled Set to 'true' if final transfer completion
 *                              indication is to be enabled.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMA_configSyncA_singleFrame
(
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    DPEDMA_syncACfg         *syncACfg,
    bool                    isEventTriggered,
    bool                    isIntermediateTransferInterruptEnabled,
    bool                    isTransferCompletionEnabled,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t               transferCompletionCallbackFxnArg
)
{
    EDMA_channelConfig_t config;
    int32_t errorCode = EDMA_NO_ERROR;

    if ((chanCfg == NULL) || (syncACfg == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    config.channelId = chanCfg->channel;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = chanCfg->channel;
    config.eventQueueId = chanCfg->eventQueue;

    config.paramSetConfig.sourceAddress = SOC_translateAddress(
                                            syncACfg->srcAddress,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);

    config.paramSetConfig.destinationAddress =SOC_translateAddress(
                                            syncACfg->destAddress,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);

    config.paramSetConfig.aCount = syncACfg->aCount;
    config.paramSetConfig.bCount = syncACfg->bCount;
    config.paramSetConfig.cCount = 1U;
    config.paramSetConfig.bCountReload = config.paramSetConfig.bCount;

    config.paramSetConfig.sourceBindex = syncACfg->srcBIdx;
    config.paramSetConfig.destinationBindex = syncACfg->dstBIdx;

    config.paramSetConfig.sourceCindex = 0;
    config.paramSetConfig.destinationCindex = 0;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;

    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled =
        isTransferCompletionEnabled;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = 
        isIntermediateTransferInterruptEnabled;
    config.transferCompletionCallbackFxn = transferCompletionCallbackFxn;

    if(chainingCfg == NULL)
    {
        config.paramSetConfig.transferCompletionCode = chanCfg->channel;
        config.paramSetConfig.isFinalChainingEnabled = false;
        config.paramSetConfig.isIntermediateChainingEnabled = false;
    }
    else
    {
        config.paramSetConfig.transferCompletionCode = chainingCfg->chainingChan;
        config.paramSetConfig.isFinalChainingEnabled =
            chainingCfg->isFinalChainingEnabled;
        config.paramSetConfig.isIntermediateChainingEnabled =
            chainingCfg->isIntermediateChainingEnabled;
    }

    if (transferCompletionCallbackFxn != NULL) {
        config.transferCompletionCallbackFxnArg = transferCompletionCallbackFxnArg;
    }

    if ((errorCode = EDMA_configChannel(handle, &config, isEventTriggered)) != EDMA_NO_ERROR)
    {
        //System_printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        goto exit;
    }

    errorCode = DPEDMA_setup_shadow_link(handle, chanCfg->channel, chanCfg->channelShadow,
        &config.paramSetConfig, config.transferCompletionCallbackFxn, transferCompletionCallbackFxnArg);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *    Utility function that reconfigures source and destination addresses of a given channel Id
 *    (whose param Id is assumed to be already configured to be same as channel Id)
 *    and then starts a transfer on that channel.
 *
 *  @param[in]  handle         EDMA handle.
 *  @param[in]  srcAddress     Source Address. If NULL, does not update.
 *  @param[in]  destAddress    Destination Address. If NULL, does not update.
 *  @param[in]  channel        EDMA channel Id.
 *  @param[in]  triggerEnabled =1: trigger EDMA, =0: does not trigger EDMA
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 *
 */
int32_t DPEDMA_updateAddressAndTrigger(EDMA_Handle handle,
    uint32_t  srcAddress,
    uint32_t  destAddress,
    uint8_t   channel,
    uint8_t   triggerEnabled)
{
    int32_t errorCode = EDMA_NO_ERROR;

    if(srcAddress != NULL)
    {
        errorCode = EDMA_setSourceAddress(handle, 
                                          (uint16_t) channel,
                                          SOC_translateAddress(srcAddress, SOC_TranslateAddr_Dir_TO_EDMA, NULL));
        if(errorCode != EDMA_NO_ERROR)
        {
            goto exit;
        }
    }

    if(destAddress != NULL)
    {
        errorCode = EDMA_setSourceAddress(handle, 
                                          (uint16_t) channel,
                                          SOC_translateAddress(destAddress, SOC_TranslateAddr_Dir_TO_EDMA, NULL));
        if(errorCode != EDMA_NO_ERROR)
        {
            goto exit;
        }
    }

    if(triggerEnabled)
    {
        if ((errorCode = EDMA_startDmaTransfer(handle, channel)) != EDMA_NO_ERROR)
        {
            goto exit;
        }
    }

exit:
    return(errorCode);
}
