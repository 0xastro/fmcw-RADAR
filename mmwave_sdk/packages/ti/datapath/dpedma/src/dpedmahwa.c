/**
 *   @file  dpedmahwa.c
 *
 *   @brief
 *      EDMA Configuration Utility API implementation for HWA related functions.
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


/* mmWave SDK Include Files */
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/hwa/hwa.h>
#include <ti/drivers/soc/soc.h>

#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/include/dpedmainternal.h>

/**
 *  @b Description
 *  @n
 *     Utility function for configuring a simple EDMA transfer used for transferring
 *     a specific  32-bit word (from a table of 16 words each containing a 1-hot signature)
 *     to the HWA's DMA completion register. The counts and addresses
 *     can be obtained from HWA APIs issued for configuring HWA.
 *     This is used to implement the DMA based trigger mode
 *     of the HWACC (search for "DMA based trigger" in the HWACC spec document)
 *
 *  @param[in]  edmaHandle          EDMA driver handle.
 *  @param[in]  chanCfg             pointer to EDMA Channel configuration
 *  @param[in]  hwaHandle           HWA driver handle.
 *  @param[in]  dmaTriggerSource    HWA paramset trigger source
 *  @param[in]  isEventTriggered    true if chId is event triggered else false
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMAHWA_configOneHotSignature
(
    EDMA_Handle         edmaHandle,
    DPEDMA_ChanCfg      *chanCfg,
    HWA_Handle          hwaHandle,
    uint8_t             dmaTriggerSource,
    bool                isEventTriggered
)
{
    EDMA_channelConfig_t    config;
    int32_t                 errorCode = EDMA_NO_ERROR;
    HWA_SrcDMAConfig        dmaConfig;

    /* Get DMA trigger configuration from HWA driver */
    HWA_getDMAconfig(hwaHandle, dmaTriggerSource, &dmaConfig);

    config.channelId = chanCfg->channel;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = chanCfg->channel;
    config.eventQueueId = chanCfg->eventQueue;

    config.paramSetConfig.sourceAddress = SOC_translateAddress(
                                            dmaConfig.srcAddr,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);

    config.paramSetConfig.destinationAddress =SOC_translateAddress(
                                            dmaConfig.destAddr,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);

    config.paramSetConfig.aCount = dmaConfig.aCnt;
    config.paramSetConfig.bCount = dmaConfig.bCnt;
    config.paramSetConfig.cCount = dmaConfig.cCnt;
    config.paramSetConfig.bCountReload = config.paramSetConfig.bCount;

    config.paramSetConfig.sourceBindex = 0;
    config.paramSetConfig.destinationBindex = 0;

    config.paramSetConfig.sourceCindex = 0;
    config.paramSetConfig.destinationCindex = 0;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.transferCompletionCode = 0;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;

    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled = false;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = false;
    config.paramSetConfig.isFinalChainingEnabled = false;
    config.paramSetConfig.isIntermediateChainingEnabled = false;
    config.transferCompletionCallbackFxn = NULL;

    if ((errorCode = EDMA_configChannel(edmaHandle, &config, isEventTriggered)) != EDMA_NO_ERROR)
    {
        //System_printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        goto exit;
    }

    errorCode = DPEDMA_setup_shadow_link(edmaHandle, chanCfg->channel, chanCfg->channelShadow,
        &config.paramSetConfig, config.transferCompletionCallbackFxn, NULL);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *     Utility function for configuring a simple EDMA transfer used for transferring
 *     two specific 32-bit word (from a table of 16 words each containing a 1-hot signature)
 *     to the HWA's DMA completion register. The counts and addresses
 *     can be obtained from HWA APIs issued for configuring HWA.
 *     This is used to implement the DMA based trigger mode
 *     of the HWACC (search for "DMA based trigger" in the HWACC spec document)
 *
 *  @param[in]  edmaHandle          EDMA driver handle.
 *  @param[in]  chanCfg             pointer to EDMA Channel configuration
 *  @param[in]  hwaHandle           HWA driver handle.
 *  @param[in]  dmaTriggerSource1   HWA paramset trigger source 1
 *  @param[in]  dmaTriggerSource2   HWA paramset trigger source 2
 *  @param[in]  isEventTriggered    true if chId is event triggered else false
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMAHWA_configTwoHotSignature
(
    EDMA_Handle         edmaHandle,
    DPEDMA_ChanCfg      *chanCfg,
    HWA_Handle          hwaHandle,
    uint8_t             dmaTriggerSource1,
    uint8_t             dmaTriggerSource2,
    bool                isEventTriggered
)
{
    EDMA_channelConfig_t    config;
    int32_t                 errorCode = EDMA_NO_ERROR;
    HWA_SrcDMAConfig        dmaConfig[2];

    /* Get DMA trigger configuration from HWA driver */
    HWA_getDMAconfig(hwaHandle, dmaTriggerSource1, &dmaConfig[0]);
    HWA_getDMAconfig(hwaHandle, dmaTriggerSource2, &dmaConfig[1]);

    config.channelId = chanCfg->channel;
    config.channelType = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    config.paramId = chanCfg->channel;
    config.eventQueueId = chanCfg->eventQueue;

    config.paramSetConfig.sourceAddress = SOC_translateAddress(
                                            dmaConfig[0].srcAddr,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);

    config.paramSetConfig.destinationAddress =SOC_translateAddress(
                                            dmaConfig[0].destAddr,
                                            SOC_TranslateAddr_Dir_TO_EDMA,
                                            NULL);

    config.paramSetConfig.aCount = dmaConfig[0].aCnt;
    config.paramSetConfig.bCount = dmaConfig[0].bCnt* 2U;
    config.paramSetConfig.cCount = dmaConfig[0].cCnt;
    config.paramSetConfig.bCountReload = config.paramSetConfig.bCount;

    config.paramSetConfig.sourceBindex = dmaConfig[1].srcAddr - dmaConfig[0].srcAddr;
    config.paramSetConfig.destinationBindex = 0;

    config.paramSetConfig.sourceCindex = 0;
    config.paramSetConfig.destinationCindex = 0;

    config.paramSetConfig.linkAddress = EDMA_NULL_LINK_ADDRESS;
    config.paramSetConfig.transferType = (uint8_t)EDMA3_SYNC_A;
    config.paramSetConfig.transferCompletionCode = 0;
    config.paramSetConfig.sourceAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;
    config.paramSetConfig.destinationAddressingMode = (uint8_t) EDMA3_ADDRESSING_MODE_LINEAR;

    /* don't care because of linear addressing modes above */
    config.paramSetConfig.fifoWidth = (uint8_t) EDMA3_FIFO_WIDTH_8BIT;

    config.paramSetConfig.isStaticSet = false;
    config.paramSetConfig.isEarlyCompletion = false;
    config.paramSetConfig.isFinalTransferInterruptEnabled = false;
    config.paramSetConfig.isIntermediateTransferInterruptEnabled = false;
    config.paramSetConfig.isFinalChainingEnabled = false;
    config.paramSetConfig.isIntermediateChainingEnabled = false;
    config.transferCompletionCallbackFxn = NULL;

    if ((errorCode = EDMA_configChannel(edmaHandle, &config, isEventTriggered)) != EDMA_NO_ERROR)
    {
        //System_printf("Error: EDMA_configChannel() failed with error code = %d\n", errorCode);
        goto exit;
    }

    errorCode = DPEDMA_setup_shadow_link(edmaHandle, chanCfg->channel, chanCfg->channelShadow,
        &config.paramSetConfig, config.transferCompletionCallbackFxn, NULL);

exit:
    return(errorCode);
}

