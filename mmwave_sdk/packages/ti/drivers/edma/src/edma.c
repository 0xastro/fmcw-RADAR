/**
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 /**
 *  \file  edma.c
 *
 *  \brief This file contains implementation of the EDMA driver APIs in edma.h.
 *      It uses at a lower level the edma_low_level.h APIs
 */

#define EDMA_PARAM_CHECK
#define EDMA_DBG

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/edma/include/edma_low_level.h>
#include <ti/drivers/edma/include/hw_edma_tc.h>
#include <ti/drivers/edma/include/hw_edma_tpcc.h>
#include <ti/drivers/edma/include/edma_xwr1xxx.h>
#include <ti/common/hw_types.h>

#include <ti/common/mmwave_error.h>

#define EDMA_READ_RATE_RANGE_CHECK_MAX      (EDMA_TC_RDRATE_RDRATE_READEVERY32CYCLES)
#define EDMA_QUEUE_PRIORITIY_RANGE_CHECK_MAX     ((uint8_t)EDMA_TPCC_QUEPRI_PRIQ0_PRIORITY7)

/**
@defgroup EDMA_INTERNAL_FUNCTION            EDMA Driver Internal Functions
@ingroup EDMA_INTERNAL
*/
/**
@defgroup EDMA_INTERNAL_DATA_STRUCTURE      EDMA Driver Internal Data Structures
@ingroup EDMA_INTERNAL
*/

/** @addtogroup EDMA_INTERNAL_DATA_STRUCTURE
 @{ */

/*! @brief QDMA object (state) information used to trigger the QDMA channel
        after configuring. */
typedef struct EDMA_qdmaObject_t_
{
    /*! @brief Trigger word address. */
    uint32_t triggerWordAddress;

    /*! @brief Trigger word value. */
    uint32_t triggerWordValue;
} EDMA_qdmaObject_t;

/*! @brief  Transfer controller error Isr argument information storage. */
typedef struct EDMA_transferControllerErrorIsrArgInfo_t_
{
    /*! @brief handle to EDMA obtained from call to @ref EDMA_open. */
    EDMA_Handle handle;

    /*! @brief Id of transfer controller. */
    uint8_t transferControllerId;
} EDMA_transferControllerErrorIsrArgInfo_t;

/*! @brief EDMA internal object (state) information structure that is required
        to be preserved between calls to EDMA APIs. */
typedef struct EDMA_Object_t_
{
    /*! @brief true if EDMA is open else false, used for error checking only,
           say if open is called when already open. */
    bool isOpen;

    /*! @brief handle for transfer completion interrupt object of the OS.
            Required to be stored to be able to delete during @ref EDMA_close API */
    HwiP_Handle hwiTransferCompleteHandle;

    /*! @brief handle for CC error interrupt object of the OS.
            Required to be stored to be able to delete during @ref EDMA_close API */
    HwiP_Handle hwiErrorHandle;

    /*! @brief handles for TC error interrupt object of the OS for each tc Id.
            Required to be stored to be able to delete during @ref EDMA_close API */
    HwiP_Handle hwiTransferControllerErrorHandle[EDMA_MAX_NUM_TC];

    /*! @brief Transfer completion call back function storage for each transfer completion code. */
    EDMA_transferCompletionCallbackFxn_t transferCompleteCallbackFxn[EDMA_NUM_TCC];

    /*! @brief Storage for argument to be returned when calling transfer completion call back
            in addition to transfer completion code, for each transfer completion code. */
    uintptr_t transferCompleteCallbackFxnArg[EDMA_NUM_TCC];

    /*! @brief QDMA object storage for each QDMA channel. */
    EDMA_qdmaObject_t qdmaObj[EDMA_NUM_QDMA_CHANNELS];

    /*! @brief CC Error call back function. */
    EDMA_errorCallbackFxn_t errorCallbackFxn;

    /*! @brief Transfer controller error call back function. */
    edmaTransferControllerErrorCallbackFxn_t transferControllerErrorCallbackFxn;

    /*! Transfer controller error Isr argument storage for each tc. */
    EDMA_transferControllerErrorIsrArgInfo_t transferControllerErrorIsrArgInfo[EDMA_MAX_NUM_TC];
} EDMA_Object_t;

/*! @brief Config structure containing object and hardware attributes. This
       will be casted to the EDMA's handle and returned in open API */
typedef struct EDMA_Config_t_
{
    /*! @brief Pointer to EDMA object (state). */
    EDMA_Object_t *object;

    /*! @brief Pointer to EDMA Hardware attributes structure. */
    EDMA_hwAttrs_t const *hwAttrs;
} EDMA_Config_t;

#ifdef EDMA_DBG
typedef struct edmaDbg_t_
{
    uint32_t transCompIsrCount;
    uint32_t transCompIsrIevalCount;
    uint32_t errorIsrCount;
    EDMA_Object_t *edmaObj;
} edmaDbg_t;
edmaDbg_t edmaDbg;
#endif

/*! @brief EDMA objects storage for each CC. */
static EDMA_Object_t EDMA_object[EDMA_NUM_CC];

/** @} */ /* end of @addtogroup EDMA_INTERNAL_DATA_STRUCTURE */

/* Private function prototype definitions */
#ifdef EDMA_PARAM_CHECK
static int32_t EDMA_validate_channelIdchannelType(uint8_t channelId, uint8_t channelType);

static int32_t EDMA_validate_channel_config(
    EDMA_Handle handle,
    EDMA_hwAttrs_t const *hwAttrs,
    EDMA_channelConfig_t const *config);

static inline int32_t EDMA_validate_param_config(EDMA_Handle handle,
    EDMA_hwAttrs_t const *hwAttrs, uint16_t paramId,
    EDMA_paramSetConfig_t const *config,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn
);
#endif
static void EDMA_paramSetConfig_assist (uint32_t ccBaseAddr, uint16_t paramId,
                                EDMA_paramSetConfig_t const *pSetCfg);

static void EDMA_paramConfig_assist(uint32_t ccBaseAddr, uint16_t paramId,
    EDMA_paramSetConfig_t const *pSetCfg,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t transferCompletionCallbackFxnArg, EDMA_Object_t *edmaObj);

static inline int32_t EDMA_startTransfer_assist(EDMA_Handle handle, uint8_t channelId,
    uint8_t channelType, bool isStaticSet);

static void EDMA_transferComplete_isr (uintptr_t arg);

static void EDMA_update_queue_entries (
    uint32_t qEntry0Addr,
    uint32_t qEntryAddrJump,
    uint8_t  numEntries,
    uint8_t  *qEntryIndx,
    EDMA_queueEntryInfo_t *qEntry);

static void EDMA_transferController_error_isr (uintptr_t arg);

static void EDMA_error_isr (uintptr_t arg);

static void EDMA_configQueueThreshold(uint32_t ccBaseAddr, uint8_t queueId,
                               uint8_t threshold);

static void EDMA_configQueuePriority(uint32_t ccBaseAddr, uint8_t transferController,
                               uint8_t priority);

static bool EDMA_isError(uint32_t ccBaseAddr);

static void EDMA_getErrorStatusInfo(EDMA_hwAttrs_t const *hwAttrs, uint32_t ccBaseAddr,
                                    EDMA_errorInfo_t *errorInfo);

static void EDMA_clearErrors(EDMA_hwAttrs_t const *hwAttrs, uint32_t ccBaseAddr,
                             EDMA_errorInfo_t const *errorInfo);

static bool EDMA_isTransferControllerError(uint32_t tcBaseAddr);

static void EDMA_getTransferControllerErrorStatusInfo(uint32_t tcBaseAddr,
                EDMA_transferControllerErrorInfo_t *errorInfo);

static void EDMA_clearTransferControllerErrors(uint32_t tcBaseAddr,
                EDMA_transferControllerErrorInfo_t *errorInfo);

#ifdef EDMA_PARAM_CHECK
/**
 *  @b Description
 *  @n
 *      Utility function for validating channel Id and channel Type parameters.
 *
 *  @param[in]  channelId  channel Id
 *  @param[in]  channelType channed Type
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      one of @ref EDMA_ERROR_CODES
 */
static int32_t EDMA_validate_channelIdchannelType(uint8_t channelId, uint8_t channelType)
{
    int32_t errorCode = EDMA_NO_ERROR;

    if ((channelType != (uint8_t)EDMA3_CHANNEL_TYPE_DMA) &&
        (channelType != (uint8_t)EDMA3_CHANNEL_TYPE_QDMA))
    {
        errorCode = EDMA_E_INVALID__DMA_CHANNEL_TYPE;
        goto exit;
    }

    if ((channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA) &&
        (channelId >= (uint8_t)EDMA_NUM_DMA_CHANNELS))
    {
        errorCode = EDMA_E_INVALID__DMA_CHANNEL_ID;
        goto exit;
    }

    if ((channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA) &&
        (channelId >= EDMA_NUM_QDMA_CHANNELS))
    {
        errorCode = EDMA_E_INVALID__QDMA_CHANNEL_ID;
        goto exit;
    }
exit:
    return(errorCode);
}
/**
 *  @b Description
 *  @n
 *      Utility function for validating parameters in @ref EDMA_channelConfig_t.
 *
 *  @param[in]  handle  EDMA handle.
 *  @param[in]  hwAttrs Pointer to hardware attributes.
 *  @param[in]  config Pointer to EDMA channel configuration structure.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      one of @ref EDMA_ERROR_CODES
 */
static int32_t EDMA_validate_channel_config(
    EDMA_Handle handle,
    EDMA_hwAttrs_t const *hwAttrs,
    EDMA_channelConfig_t const *config)
{
    int32_t errorCode = EDMA_NO_ERROR;

    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }

    if (config == NULL)
    {
        errorCode = EDMA_E_INVALID__CONFIG_POINTER_NULL;
        goto exit;
    }

    errorCode = EDMA_validate_channelIdchannelType(config->channelId,
        config->channelType);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    if (config->channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
        if (config->qdmaParamTriggerWordOffset > 7) {
            errorCode = EDMA_E_INVALID__QDMA_TRIGGER_WORD;
            goto exit;
        }
    }

    /* if channel mappping is not supported, paramId must be same as channelId
       for DMA channel */
    if ((hwAttrs->isChannelMapExist == false) &&
        (config->channelType == (uint8_t)EDMA3_CHANNEL_TYPE_DMA) &&
        (config->paramId != (uint16_t)config->channelId))
    {
        errorCode = EDMA_E_INVALID__PARAM_ID_NOT_SAME_AS_CHANNEL_ID;
        goto exit;
    }

    if (config->eventQueueId >= hwAttrs->numEventQueues)
    {
        errorCode = EDMA_E_INVALID__EVENT_QUEUE_ID;
        goto exit;
    }

    errorCode = EDMA_validate_param_config(handle, hwAttrs, config->paramId, &config->paramSetConfig,
        config->transferCompletionCallbackFxn);

exit:
    return(errorCode);
}
/**
 *  @b Description
 *  @n
 *      Utility function for validating parameters relalted to param configuration.
 *
 *  @param[in]  handle  EDMA handle.
 *  @param[in]  hwAttrs Pointer to hardware attributes.
 *  @param[in]  paramId PaRAM Set Id.
 *  @param[in]  config Pointer to PaRAM Set configuration structure.
 *  @param[in]  transferCompletionCallbackFxn transfer completion call-back function configuration.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      one of @ref EDMA_ERROR_CODES
 */
static inline int32_t EDMA_validate_param_config(
    EDMA_Handle handle,
    EDMA_hwAttrs_t const *hwAttrs,
    uint16_t paramId,
    EDMA_paramSetConfig_t const *config,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn
    )
{
    int32_t errorCode = EDMA_NO_ERROR;

    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }

    if (paramId >= hwAttrs->numParamSets)
    {
        errorCode = EDMA_E_INVALID__PARAM_ID;
        goto exit;
    }

    if (config->transferCompletionCode >= EDMA_NUM_TCC)
    {
        errorCode = EDMA_E_INVALID__TRANSFER_COMPLETION_CODE;
        goto exit;
    }

    /* if all of interrupt completion flags are false, then transfer completion
       call back function must be NULL */
    if  ((transferCompletionCallbackFxn != NULL) &&
         ((config->isFinalTransferInterruptEnabled == false) &&
          (config->isIntermediateTransferInterruptEnabled == false)))
    {
        errorCode = EDMA_E_INVALID__TRNSFR_COMPLETION_PARAMS;
        goto exit;
    }

    /* if completion interrupt not connected and transfer completion call back
     * is not NULL, then generate error
     */
    if  ((hwAttrs->transferCompletionInterruptNum == EDMA_INTERRUPT_NOT_CONNECTED_ID) &&
          (transferCompletionCallbackFxn != NULL))
    {
        errorCode = EDMA_E_INVALID__TRNSFR_COMPLETION_PARAMS;
        goto exit;
    }

    if ((config->transferType != (uint8_t)EDMA3_SYNC_A) &&
        (config->transferType != (uint8_t)EDMA3_SYNC_AB))
    {
        errorCode = EDMA_E_INVALID__TRANSFER_TYPE;
        goto exit;
    }

exit:
    return(errorCode);
}
#endif

/* Not using EDMA3SetPaRAM because of JIRA : PRSDK-1055.
   Also risky because depends on compiler realization of EDMA3CCPaRAMEntry structure
   to match PaRAM layout */
#if 0
static void EDMA_paramSetConfig_assist (uint32_t ccBaseAddr, uint16_t paramId,
                                EDMA_paramSetConfig_t const *pSetCfg)
{
    EDMA3CCPaRAMEntry paramSet;

    /* program PaRAM set */
    paramSet.srcAddr  = pSetCfg->sourceAddress;
    paramSet.destAddr = pSetCfg->destinationAddress;
    paramSet.aCnt     = pSetCfg->aCount;
    paramSet.bCnt     = pSetCfg->bCount;
    paramSet.cCnt     = pSetCfg->cCount;
    paramSet.srcBIdx  = pSetCfg->sourceBindex;
    paramSet.destBIdx = pSetCfg->destinationBindex;
    paramSet.srcCIdx  = pSetCfg->sourceCindex;
    paramSet.destCIdx = pSetCfg->destinationCindex;
    paramSet.linkAddr = pSetCfg->linkAddress;
    paramSet.bCntReload = pSetCfg->bCountReload;

    /* Parameters not programmed are : PRIV = 0; PRIVID = 0; */
    paramSet.opt = 0;
    paramSet.opt |= (
    ((uint32_t)(pSetCfg->transferType == (uint8_t)EDMA3_SYNC_AB) << EDMA_TPCC_OPT_SYNCDIM_SHIFT) |
    ((uint32_t)(pSetCfg->sourceAddressingMode == (uint8_t)EDMA3_ADDRESSING_MODE_FIFO_WRAP) <<
        EDMA_TPCC_OPT_SAM_SHIFT) |
    ((uint32_t)(pSetCfg->destinationAddressingMode == (uint8_t)EDMA3_ADDRESSING_MODE_FIFO_WRAP) <<
        EDMA_TPCC_OPT_DAM_SHIFT) |
    ((uint32_t)pSetCfg->fifoWidth << EDMA_TPCC_OPT_FWID_SHIFT) |
    ((uint32_t)pSetCfg->transferCompletionCode << EDMA_TPCC_OPT_TCC_SHIFT) |
    ((uint32_t)pSetCfg->isStaticSet << EDMA_TPCC_OPT_STATIC_SHIFT) |
    ((uint32_t)pSetCfg->isEarlyCompletion << EDMA_TPCC_OPT_TCCMODE_SHIFT) |
    ((uint32_t)pSetCfg->isFinalTransferInterruptEnabled << EDMA_TPCC_OPT_TCINTEN_SHIFT) |
    ((uint32_t)pSetCfg->isIntermediateTransferInterruptEnabled << EDMA_TPCC_OPT_ITCINTEN_SHIFT) |
    ((uint32_t)pSetCfg->isFinalChainingEnabled << EDMA_TPCC_OPT_TCCHEN_SHIFT) |
    ((uint32_t)pSetCfg->isIntermediateChainingEnabled << EDMA_TPCC_OPT_ITCCHEN_SHIFT)
    );
    EDMA3SetPaRAM(ccBaseAddr, paramId, &paramSet);
}
#else
/**
 *  @b Description
 *  @n
 *      Utility function for configuring a PaRAM Set.
 *      Note: HW_WR_FIELD32 APIs are avoided for (assumed) efficiency when writing
 *      param fields (avoid unnecessary masking because fields are defined to be right
 *      sized (e.g aCount is uint16_t, matching Param ACNT size).
 *
 *  @param[in]  ccBaseAddr  CC base address.
 *  @param[in]  paramId PaRAM Set Id.
 *  @param[in]  pSetCfg Pointer to PaRAM Set configuration.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_paramSetConfig_assist (uint32_t ccBaseAddr, uint16_t paramId,
                                EDMA_paramSetConfig_t const *pSetCfg)
{
    uint32_t opt;
    uint32_t paramStartAddr = ccBaseAddr + EDMA_TPCC_OPT((uint32_t)paramId);
    uint32_t paramFieldAddr;

    paramFieldAddr = paramStartAddr;

    /* opt parameters not programmed are : PRIV = 0; PRIVID = 0; */
    opt = (
    ((uint32_t)(pSetCfg->transferType == (uint8_t)EDMA3_SYNC_AB) << EDMA_TPCC_OPT_SYNCDIM_SHIFT) |
    ((uint32_t)(pSetCfg->sourceAddressingMode == (uint8_t)EDMA3_ADDRESSING_MODE_FIFO_WRAP) <<
        EDMA_TPCC_OPT_SAM_SHIFT) |
    ((uint32_t)(pSetCfg->destinationAddressingMode == (uint8_t)EDMA3_ADDRESSING_MODE_FIFO_WRAP) <<
        EDMA_TPCC_OPT_DAM_SHIFT) |
    ((uint32_t)pSetCfg->fifoWidth << EDMA_TPCC_OPT_FWID_SHIFT) |
    ((uint32_t)pSetCfg->transferCompletionCode << EDMA_TPCC_OPT_TCC_SHIFT) |
    ((uint32_t)pSetCfg->isStaticSet << EDMA_TPCC_OPT_STATIC_SHIFT) |
    ((uint32_t)pSetCfg->isEarlyCompletion << EDMA_TPCC_OPT_TCCMODE_SHIFT) |
    ((uint32_t)pSetCfg->isFinalTransferInterruptEnabled << EDMA_TPCC_OPT_TCINTEN_SHIFT) |
    ((uint32_t)pSetCfg->isIntermediateTransferInterruptEnabled << EDMA_TPCC_OPT_ITCINTEN_SHIFT) |
    ((uint32_t)pSetCfg->isFinalChainingEnabled << EDMA_TPCC_OPT_TCCHEN_SHIFT) |
    ((uint32_t)pSetCfg->isIntermediateChainingEnabled << EDMA_TPCC_OPT_ITCCHEN_SHIFT)
    );
    HW_WR_REG32(paramFieldAddr, opt);
    paramFieldAddr += sizeof(uint32_t);

    /* SRC */
    HW_WR_REG32(paramFieldAddr, pSetCfg->sourceAddress);
    paramFieldAddr += sizeof(uint32_t);

    /* BCNT_ACNT */
    HW_WR_REG32(paramFieldAddr, ((uint32_t)pSetCfg->bCount << EDMA_TPCC_ABCNT_BCNT_SHIFT) |
        ((uint32_t)pSetCfg->aCount << EDMA_TPCC_ABCNT_ACNT_SHIFT));
    paramFieldAddr += sizeof(uint32_t);

    /* DST */
    HW_WR_REG32(paramFieldAddr, pSetCfg->destinationAddress);
    paramFieldAddr += sizeof(uint32_t);

    /* DSTBIDX_SRCBIDX */
    /* Note: the cast to uint16_t must be done for source index because it is signed 16
       and simply casting to unsigned 32-bit does not make it signed, it simply extends
       the sign to 32-bit. */
    HW_WR_REG32(paramFieldAddr, 
        ((uint32_t)((uint16_t)pSetCfg->destinationBindex) << EDMA_TPCC_BIDX_DBIDX_SHIFT) |
        ((uint32_t)((uint16_t)pSetCfg->sourceBindex) << EDMA_TPCC_BIDX_SBIDX_SHIFT));
    paramFieldAddr += sizeof(uint32_t);

    /* BCNTRLD_LINK */
    HW_WR_REG32(paramFieldAddr, ((uint32_t)pSetCfg->bCountReload << EDMA_TPCC_LNK_BCNTRLD_SHIFT) |
        ((uint32_t)pSetCfg->linkAddress << EDMA_TPCC_LNK_LINK_SHIFT));
    paramFieldAddr += sizeof(uint32_t);

    /* DSTCIDX_SRCCIDX */
    /* Note: the cast to uint16_t must be done for source index because it is signed 16
       and simply casting to unsigned 32-bit does not make it signed, it simply extends
       the sign to 32-bit */
    HW_WR_REG32(paramFieldAddr, 
        ((uint32_t)((uint16_t)pSetCfg->destinationCindex) << EDMA_TPCC_CIDX_DCIDX_SHIFT) |
        ((uint32_t)((uint16_t)pSetCfg->sourceCindex) << EDMA_TPCC_CIDX_SCIDX_SHIFT));
    paramFieldAddr += sizeof(uint32_t);

    /* CCNT */
    HW_WR_REG32(paramFieldAddr, (uint32_t)pSetCfg->cCount << EDMA_TPCC_CCNT_CCNT_SHIFT);
}
#endif

/**
 *  @b Description
 *  @n
 *      Transfer Completion Isr which will trigger when a channel transfer completes
 *      and if the channel is configured for a non-NULL completion call back function
 *      during channel configuration.
 *      Checks which transfer completion codes are set and
 *      calls the corresponding registered application call back function.
 *
 *  @param[in]  arg  EDMA handle which was set when Isr was registered using
 *                   HwiP_create OSAL API.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_transferComplete_isr (uintptr_t arg)
{
    EDMA_Handle handle = (EDMA_Handle) arg;
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    uint32_t lowIntrStatus, highIntrStatus;
    uint8_t transCompCode;
    uint8_t half, full;

#ifdef EDMA_DBG
    edmaDbg.transCompIsrCount++;
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;

    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

    half = EDMA_NUM_DMA_CHANNELS/2U;
    full = EDMA_NUM_DMA_CHANNELS;

    /* scan only whose interrupts are enabled */
    lowIntrStatus = EDMA3GetIntrStatus(ccBaseAddr) & HW_RD_REG32(ccBaseAddr + EDMA_TPCC_IER);
    highIntrStatus = EDMA3IntrStatusHighGet(ccBaseAddr) & HW_RD_REG32(ccBaseAddr + EDMA_TPCC_IERH);
    if ((lowIntrStatus != 0U) || (highIntrStatus != 0U))
    {
        /* scan low status */
        for (transCompCode = 0U; transCompCode < half; transCompCode++)
        {
            if ((lowIntrStatus & ((uint32_t)1 << transCompCode)) == ((uint32_t)1 << transCompCode))
            {
                /* clear interrupt */
                EDMA3ClrIntr(ccBaseAddr, (uint32_t)transCompCode);

                /* call registered call back function for the transferCompletionCode */
                if (edmaObj->transferCompleteCallbackFxn[transCompCode] != NULL)
                {
                    (*edmaObj->transferCompleteCallbackFxn[transCompCode])(
                        edmaObj->transferCompleteCallbackFxnArg[transCompCode],
                        transCompCode);
                }
                else
                {
                    /* interrupt indicated but no call back function */
                    DebugP_assert(0);
                }
            }
        }

        /* scan high status */
        for (transCompCode = half; transCompCode < full; transCompCode++)
        {
            if ((highIntrStatus & ((uint32_t)1 << (transCompCode -half))) ==
                ((uint32_t)1 << (transCompCode - half)))
            {
                /* clear interrupt */
                EDMA3ClrIntr(ccBaseAddr, (uint32_t)transCompCode);

                /* call registered call back function for the transferCompletionCode */
                if (edmaObj->transferCompleteCallbackFxn[transCompCode] != NULL)
                {
                    (*edmaObj->transferCompleteCallbackFxn[transCompCode])(
                        edmaObj->transferCompleteCallbackFxnArg[transCompCode],
                        transCompCode);
                }
                else
                {
                    /* interrupt indicated but no call back function */
                    DebugP_assert(0);
                }
            }
        }
    }

    /* check conditions again, if any detected, then notify hardware.
       The alternative is to do the above code in a loop but this can hold up
       other interrupts in the system as multiple transfers could complete
       in this time given the pocessing of call-back functions also, so the IEVAL
       approach is preferred. For more details,
       see EDMA User Guide section "EDMA3 Interrupt Servicing" */
    lowIntrStatus = EDMA3GetIntrStatus(ccBaseAddr) & HW_RD_REG32(ccBaseAddr + EDMA_TPCC_IER);
    highIntrStatus = EDMA3IntrStatusHighGet(ccBaseAddr) & HW_RD_REG32(ccBaseAddr + EDMA_TPCC_IERH);
    if ((lowIntrStatus != 0) || (highIntrStatus != 0))
    {
#ifdef EDMA_DBG
        edmaDbg.transCompIsrIevalCount++;
#endif
        /* Based on GINT (Global interrupt) */
        HW_WR_FIELD32(ccBaseAddr + EDMA_TPCC_IEVAL, EDMA_TPCC_IEVAL_EVAL, 1);
    }
}

/**
 *  @b Description
 *  @n
 *      Utility function to update queue entry event number and event type
 *        information as read from the QxEy registers. Exploits the symmetric structure
 *        of these registers.
 *
 *  @param[in]  qEntry0Addr  Address of queue entry 0.
 *  @param[in]  qEntryAddrJump Queue entry address jump.
 *  @param[in]  numEntries Number of entries to iterate on.
 *  @param[in,out]  qEntryIndx Pointer to queue entry index. Will be incremented
 *                  in a circular manner.
 *  @param[out] qEntry Pointer to queue Entry array which needs to be populated.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_update_queue_entries (
    uint32_t qEntry0Addr,
    uint32_t qEntryAddrJump,
    uint8_t  numEntries,
    uint8_t  *qEntryIndx,
    EDMA_queueEntryInfo_t *qEntry)
{
    uint8_t qEntryCount;
    uint32_t qEntryAddr;

    for (qEntryCount = 0; qEntryCount < numEntries; qEntryCount++)
    {
        qEntryAddr = qEntry0Addr + *qEntryIndx * qEntryAddrJump;

        qEntry[qEntryCount].eventNumber =
            HW_RD_FIELD32(qEntryAddr, EDMA_TPCC_QNE0_ENUM);

        qEntry[qEntryCount].eventType =
            HW_RD_FIELD32(qEntryAddr, EDMA_TPCC_QNE0_ETYPE);

        *qEntryIndx += 1U;
        if (*qEntryIndx >= EDMA_NUM_QUEUE_ENTRIES)
        {
            *qEntryIndx = 0U;
        }
    }
}

/**
 *  @b Description
 *  @n
 *      Utility function to check if any of the EDMA errors happened. Suitable
 *      for polling purposes.
 *
 *  @param[in]  ccBaseAddr CC base address.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      true if any EDMA error else false
 */
static bool EDMA_isError(uint32_t ccBaseAddr)
{
    return((EDMA3GetErrIntrStatus(ccBaseAddr) != 0U) ||
            (EDMA3ErrIntrHighStatusGet(ccBaseAddr) != 0U) ||
            (EDMA3QdmaGetErrIntrStatus(ccBaseAddr) != 0U) ||
            (EDMA3GetCCErrStatus(ccBaseAddr) != 0U));
}

/**
 *  @b Description
 *  @n
 *      Utility function to get error status information.
 *
 *  @param[in]  hwAttrs pointer to hardware attributes.
 *  @param[in]  ccBaseAddr cc base address.
 *  @param[out] errorInfo pointer to error information in with status of errors
 *                         will be populated.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_getErrorStatusInfo(EDMA_hwAttrs_t const *hwAttrs, uint32_t ccBaseAddr,
                                    EDMA_errorInfo_t *errorInfo)
{
    uint8_t queueId;
    uint8_t channelId;
    uint8_t half, full;
    uint32_t shift;
    uint32_t errorStatus;

    errorInfo->numEventQueues = hwAttrs->numEventQueues;

    /* DMA channels status processing */
    half = EDMA_NUM_DMA_CHANNELS/2U;
    full = EDMA_NUM_DMA_CHANNELS;

    /* low processing */
    errorStatus = EDMA3GetErrIntrStatus(ccBaseAddr);
    for (channelId = 0U; channelId < half; channelId++)
    {
        errorInfo->isDmaChannelEventMiss[channelId] = (bool)((errorStatus >> channelId) & 1U);
    }
    /* high processing */
    errorStatus = EDMA3ErrIntrHighStatusGet(ccBaseAddr);
    for (channelId = half; channelId < full; channelId++)
    {
        errorInfo->isDmaChannelEventMiss[channelId] =
            (bool)((errorStatus >> (channelId - half)) & 1U);
    }

    /* QDMA channels status processing */
    errorStatus = EDMA3QdmaGetErrIntrStatus(ccBaseAddr);
    for (channelId = 0U; channelId < EDMA_NUM_QDMA_CHANNELS; channelId++)
    {
        errorInfo->isQdmaChannelEventMiss[channelId] = (bool)((errorStatus >> channelId) & 1U);
    }

    /* CC error status processing */
    errorStatus = EDMA3GetCCErrStatus(ccBaseAddr);
    for(queueId = 0U; queueId < hwAttrs->numEventQueues; queueId++)
    {
        shift = (uint32_t)queueId * (uint32_t)(EDMA_TPCC_CCERR_QTHRXCD1_SHIFT - 
                                               EDMA_TPCC_CCERR_QTHRXCD0_SHIFT);
        errorInfo->isEventQueueThresholdExceeded[queueId] = (bool)((errorStatus >> shift) & 1U);
    }
    errorInfo->isOutstandingTransferCompletionTransfersExceededLimit =
        (bool)((errorStatus >> EDMA_TPCC_CCERR_TCERR_SHIFT) & 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function to clear EDMA errors.
 *
 *  @param[in]  hwAttrs pointer to hardware attributes.
 *  @param[in]  ccBaseAddr cc base address.
 *  @param[in]  errorInfo pointer to error information which is used to determine
 *                        what errors will be cleared so that hardware can detect
 *                        new errors.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_clearErrors(EDMA_hwAttrs_t const *hwAttrs, uint32_t ccBaseAddr,
                             EDMA_errorInfo_t const *errorInfo)
{
    uint8_t queueId;
    uint8_t channelId;
    uint8_t full;
    uint32_t shift;

    full = EDMA_NUM_DMA_CHANNELS;

    /* clear conditions that were detected */
    /* dma */
    for (channelId = 0U; channelId < full; channelId++)
    {
        if (errorInfo->isDmaChannelEventMiss[channelId] == true)
        {
            EDMA3ClrMissEvt(ccBaseAddr, (uint32_t)channelId);
        }
    }
    /* qdma */
    for (channelId = 0U; channelId < EDMA_NUM_QDMA_CHANNELS; channelId++)
    {
        if (errorInfo->isQdmaChannelEventMiss[channelId] == true)
        {
            EDMA3QdmaClrMissEvt(ccBaseAddr, (uint32_t)channelId);
        }
    }
    /* queues */
    for (queueId = 0U; queueId < hwAttrs->numEventQueues; queueId++)
    {
        shift = (uint32_t)queueId * (EDMA_TPCC_CCERRCLR_QTHRXCD1_SHIFT - EDMA_TPCC_CCERRCLR_QTHRXCD0_SHIFT);
        if (errorInfo->isEventQueueThresholdExceeded[queueId] == true)
        {
            EDMA3ClrCCErr(ccBaseAddr, ((uint32_t)1 << shift));
        }
    }
    /* outstanding transfer completion error */
    if (errorInfo->isOutstandingTransferCompletionTransfersExceededLimit == true)
    {
        EDMA3ClrCCErr(ccBaseAddr, ((uint32_t)1 << EDMA_TPCC_CCERRCLR_TCERR_SHIFT));
    }
}

int32_t EDMA_getErrorStatus(EDMA_Handle handle, bool *isAnyError, EDMA_errorInfo_t *errorInfo)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs = edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

    if (*isAnyError = EDMA_isError(ccBaseAddr))
    {
        EDMA_getErrorStatusInfo(hwAttrs, ccBaseAddr, errorInfo);
        EDMA_clearErrors(hwAttrs, ccBaseAddr, errorInfo);
    }

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      CC error isr which will trigger when any CC errors happen. It will call
 *      application registered error call back function with the error information.
 *
 *  @param[out]  arg  @ref EDMA_Handle which was set when Isr was registered
 *                    using OSAL API HwiP_create.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_error_isr (uintptr_t arg)
{
    EDMA_Handle handle = (EDMA_Handle) arg;
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    EDMA_errorInfo_t errorInfo;

#ifdef EDMA_DBG
    edmaDbg.errorIsrCount++;
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;
    hwAttrs = edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

    EDMA_getErrorStatusInfo(hwAttrs, ccBaseAddr, &errorInfo);

    /* application call back processing */
    if (edmaObj->errorCallbackFxn != NULL)
    {
        (*edmaObj->errorCallbackFxn)(handle, &errorInfo);
    }

    EDMA_clearErrors(hwAttrs, ccBaseAddr, &errorInfo);

    /* Check conditions again and set EEVAL if any detected, this procedure is
       similar to the transfer completion isr's (EEVAL similar to IEVAL) */
    if (EDMA_isError(ccBaseAddr))
    {
        HW_WR_FIELD32(ccBaseAddr + EDMA_TPCC_EEVAL, EDMA_TPCC_EEVAL_EVAL, (uint32_t)1);
    }
}

static bool EDMA_isTransferControllerError(uint32_t tcBaseAddr)
{
    uint32_t errStatRegAddr = tcBaseAddr + EDMA_TC_ERRSTAT;

    return(HW_RD_REG32(errStatRegAddr) != 0);
}

static void EDMA_getTransferControllerErrorStatusInfo(uint32_t tcBaseAddr,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    uint32_t errStatRegAddr, errDetRegAddr;
    EDMA_transferControllerBusErrorInfo_t *busErrorInfo;

    busErrorInfo = &errorInfo->busErrorInfo;

    /* ERRSTAT processing */
    errStatRegAddr = tcBaseAddr + EDMA_TC_ERRSTAT;
    errorInfo->isTransferRequestError =
        HW_RD_FIELD32(errStatRegAddr, EDMA_TC_ERRSTAT_TRERR);

    errorInfo->isWriteToReservedConfigMemoryMap =
        HW_RD_FIELD32(errStatRegAddr, EDMA_TC_ERRSTAT_MMRAERR);

    errorInfo->isBusError =
        HW_RD_FIELD32(errStatRegAddr, EDMA_TC_ERRSTAT_BUSERR);

    /* ERRDET processing */
    errDetRegAddr = tcBaseAddr + EDMA_TC_ERRDET;
    if (errorInfo->isBusError == true)
    {
        busErrorInfo->errorCode =
            HW_RD_FIELD32(errDetRegAddr, EDMA_TC_ERRDET_STAT);

        busErrorInfo->transferCompletionCode =
            HW_RD_FIELD32(errDetRegAddr, EDMA_TC_ERRDET_TCC);

        busErrorInfo->isFinalChainingEnabled =
            HW_RD_FIELD32(errDetRegAddr, EDMA_TC_ERRDET_TCCHEN);

        busErrorInfo->isFinalTransferInterruptEnabled =
            HW_RD_FIELD32(errDetRegAddr, EDMA_TC_ERRDET_TCINTEN);
    }
}

static void EDMA_clearTransferControllerErrors(uint32_t tcBaseAddr,
                EDMA_transferControllerErrorInfo_t *errorInfo)
{
    uint32_t errClrRegAddr;

    errClrRegAddr = tcBaseAddr + EDMA_TC_ERRCLR;
    HW_WR_FIELD32(errClrRegAddr, EDMA_TC_ERRCLR_TRERR, errorInfo->isTransferRequestError);
    HW_WR_FIELD32(errClrRegAddr, EDMA_TC_ERRCLR_MMRAERR, errorInfo->isWriteToReservedConfigMemoryMap);
    HW_WR_FIELD32(errClrRegAddr, EDMA_TC_ERRCLR_BUSERR, errorInfo->isBusError);
}

int32_t EDMA_getTransferControllerErrorStatus(EDMA_Handle handle, uint8_t transferControllerId,
    bool *isAnyError, EDMA_transferControllerErrorInfo_t *errorInfo)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t tcBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs = edmaConfig->hwAttrs;
    tcBaseAddr = hwAttrs->TCbaseAddress[transferControllerId];

    if (*isAnyError = EDMA_isTransferControllerError(tcBaseAddr))
    {
        EDMA_getTransferControllerErrorStatusInfo(tcBaseAddr, errorInfo);
        EDMA_clearTransferControllerErrors(tcBaseAddr, errorInfo);
    }

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      TC error isr which will trigger when any TC errors happen. It will call
 *      application registered error call back function with the error information,
 *      the error information will also report which transfer controller Id was
 *      triggered.
 *
 *  @param[out]  arg pointer to @ref EDMA_transferControllerErrorIsrArgInfo_t which
 *               was set when the ISR was registered using HwiP_create OSAL API.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_transferController_error_isr (uintptr_t arg)
{
    EDMA_transferControllerErrorIsrArgInfo_t *argInfo =
        (EDMA_transferControllerErrorIsrArgInfo_t *) arg;
    EDMA_Handle handle = (EDMA_Handle) argInfo->handle;
    uint8_t transferControllerId = argInfo->transferControllerId;
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    EDMA_transferControllerErrorInfo_t errorInfo;
    uint32_t tcBaseAddr;

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;
    hwAttrs = edmaConfig->hwAttrs;
    tcBaseAddr = hwAttrs->TCbaseAddress[transferControllerId];

    errorInfo.transferControllerId = transferControllerId;

    EDMA_getTransferControllerErrorStatusInfo(tcBaseAddr, &errorInfo);

    /* application call back */
    if (edmaObj->transferControllerErrorCallbackFxn != NULL)
    {
        (*edmaObj->transferControllerErrorCallbackFxn)(handle, &errorInfo);
    }

    /* clear conditions */
    EDMA_clearTransferControllerErrors(tcBaseAddr, &errorInfo);

    /* Check conditions again and set EVAL if any detected, this procedure is
       similar to the transfer completion isr's (EVAL similar to IEVAL) */
    if (EDMA_isTransferControllerError(tcBaseAddr))
    {
        HW_WR_FIELD32(tcBaseAddr + EDMA_TC_ERRCMD, EDMA_TC_ERRCMD_EVAL, (uint32_t)1);
    }
}

/**
 *  @b Description
 *  @n
 *      Utility API to configure queue threshold. It exploits the symmetric
 *      structure of QWMTHR(A/B) registers.
 *
 *  @param[in]  ccBaseAddr CC base address.
 *  @param[in]  queueId    Queue Id of the queue to be configured.
 *  @param[in]  threshold  Queue threshold to be configured.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_configQueueThreshold(uint32_t ccBaseAddr, uint8_t queueId,
                               uint8_t threshold)
{
    uint32_t shift, mask;

    shift = (uint32_t)queueId * (EDMA_TPCC_QWMTHRA_Q1_SHIFT - EDMA_TPCC_QWMTHRA_Q0_SHIFT);
    mask = (uint32_t)EDMA_TPCC_QWMTHRA_Q0_MASK << shift;
        
    if (queueId <= 3U)
    {
        /* program queue threshold A reg */
        HW_WR_FIELD32_RAW(ccBaseAddr + EDMA_TPCC_QWMTHRA, mask, shift, (uint32_t)threshold);
    }
    else
    {
        /* program queue threshold B reg, note the B in QWMTHRB below */
        HW_WR_FIELD32_RAW(ccBaseAddr + EDMA_TPCC_QWMTHRB, mask, shift, (uint32_t)threshold);
    }
}

/**
 *  @b Description
 *  @n
 *      Utility API to configure queue priority. It exploits the symmetric
 *      structure of QUEPRI registers.
 *
 *  @param[in]  ccBaseAddr CC base address.
 *  @param[in]  transferController  TC (or queue) Id.
 *  @param[in]  priority  Queue priority to be configured.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_configQueuePriority(uint32_t ccBaseAddr, uint8_t transferController,
                               uint8_t priority)
{
    uint32_t shift, mask;

    shift = (uint32_t)transferController * (EDMA_TPCC_QUEPRI_PRIQ1_SHIFT - EDMA_TPCC_QUEPRI_PRIQ0_SHIFT);
    mask = (uint32_t)EDMA_TPCC_QUEPRI_PRIQ0_MASK << shift;
    HW_WR_FIELD32_RAW(ccBaseAddr + EDMA_TPCC_QUEPRI, mask, shift, (uint32_t)priority);
}

/**
 *  @b Description
 *  @n
 *      Utility API to configure a transfer controller's error detection settings.
 *
 *  @param[in]  tcBaseAddr TC base address.
 *  @param[in]  isEnableAllErrors   true if all error checking is to be enabled.
 *  @param[in]  config Pointer to @ref EDMA_transferControllerErrorConfig_t when
 *                     selective errors are to be configured.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      None.
 */
static void EDMA_configTransferControllerError(uint32_t tcBaseAddr,
    bool isEnableAllErrors, EDMA_transferControllerErrorConfig_t const *config)
{
    uint32_t errenRegAddr;
    bool isBusErrorEnabled, isTransferRequestErrorEnabled,
         isWriteToReservedConfigMemoryMapEnabled;

    errenRegAddr = tcBaseAddr + EDMA_TC_ERREN;
    if (isEnableAllErrors == true)
    {
        isBusErrorEnabled = true;
        isTransferRequestErrorEnabled = true;
        isWriteToReservedConfigMemoryMapEnabled = true;
    }
    else
    {
        isBusErrorEnabled = config->isBusErrorEnabled;
        isTransferRequestErrorEnabled = config->isTransferRequestErrorEnabled;
        isWriteToReservedConfigMemoryMapEnabled =
            config->isWriteToReservedConfigMemoryMapEnabled;
    }
    HW_WR_FIELD32(errenRegAddr, EDMA_TC_ERREN_BUSERR, isBusErrorEnabled);
    HW_WR_FIELD32(errenRegAddr, EDMA_TC_ERREN_TRERR, isTransferRequestErrorEnabled);
    HW_WR_FIELD32(errenRegAddr, EDMA_TC_ERREN_MMRAERR, isWriteToReservedConfigMemoryMapEnabled);
}

/**
 *  @b Description
 *  @n
 *      Utility function for starting a transfer.
 *
 *  @param[in]  handle  EDMA handle.
 *  @param[in]  channelId  channel Id.
 *  @param[in]  channelType channed Type.
 *  @param[in]  isStaticSet true if STATIC bit is to be set else false.
 *
 *  \ingroup EDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      one of @ref EDMA_ERROR_CODES
 */
static inline int32_t EDMA_startTransfer_assist(EDMA_Handle handle, uint8_t channelId,
    uint8_t channelType, bool isStaticSet)
{
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;
    uint16_t paramId;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
    if ((channelType != (uint8_t)EDMA3_CHANNEL_TYPE_DMA) &&
        (channelType != (uint8_t)EDMA3_CHANNEL_TYPE_QDMA))
    {
        errorCode = EDMA_E_INVALID__DMA_CHANNEL_TYPE;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;
    hwAttrs = edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
#ifdef EDMA_PARAM_CHECK
        /* error checking */
        if (channelId >= EDMA_NUM_QDMA_CHANNELS)
        {
            errorCode = EDMA_E_INVALID__QDMA_CHANNEL_ID;
            goto exit;
        }
#endif

        if (EDMA3QdmaGetErrIntrStatus(ccBaseAddr) & ((uint32_t)1 << channelId))
        {
            errorCode = EDMA_E_UNEXPECTED__QDMA_EVENT_MISS_DETECTED;
            /* clear previous missed events : QSECR and QEMCR */
            EDMA3QdmaClrMissEvt(ccBaseAddr, (uint32_t)channelId);
            goto exit;
        }

        if (isStaticSet == true)
        {
            if (HW_RD_FIELD32(ccBaseAddr + EDMA_TPCC_QCHMAPN((uint32_t)channelId),
                    EDMA_TPCC_QCHMAPN_TRWORD)  == EDMA_QDMA_TRIG_WORD_OFFSET_OPT)
            {
                /* modify triggerWordValue before triggering */
                HW_WR_FIELD32(&edmaObj->qdmaObj[channelId].triggerWordValue,
                    EDMA_TPCC_OPT_STATIC, 1);
            }
            else
            {
                /* update OPT field */
                paramId = HW_RD_FIELD32(ccBaseAddr + EDMA_TPCC_QCHMAPN((uint32_t)channelId),
                    EDMA_TPCC_QCHMAPN_PAENTRY);
                HW_WR_FIELD32(ccBaseAddr + EDMA_TPCC_OPT((uint32_t)paramId), EDMA_TPCC_OPT_STATIC, 1);

            }
        }
        /* start transfer of specified Qdma channel, trigger word was programmed in config API */
        HW_WR_REG32(edmaObj->qdmaObj[channelId].triggerWordAddress,
                                           edmaObj->qdmaObj[channelId].triggerWordValue);
    }
    else /* channelType is EDMA3_CHANNEL_TYPE_DMA */
    {
#ifdef EDMA_PARAM_CHECK
        /* error checking */
        if (channelId >= EDMA_NUM_DMA_CHANNELS)
        {
            errorCode = EDMA_E_INVALID__DMA_CHANNEL_ID;
            goto exit;
        }
#endif

        if (channelId < 32U) {
            if (EDMA3GetErrIntrStatus(ccBaseAddr) & ((uint32_t)1 << channelId))
            {
                errorCode = EDMA_E_UNEXPECTED__DMA_EVENT_MISS_DETECTED;
            }
        }
        else
        {
            if (EDMA3ErrIntrHighStatusGet(ccBaseAddr) & ((uint32_t)1 << ((uint32_t)channelId-32U)))
            {
                errorCode = EDMA_E_UNEXPECTED__DMA_EVENT_MISS_DETECTED;
            }
        }

        if (errorCode != EDMA_NO_ERROR)
        {
            /* clear missed events */
            EDMA3ClrMissEvt(ccBaseAddr, (uint32_t)channelId);
            goto exit;
        }

        /* trigger the event */
        EDMA3SetEvt(ccBaseAddr, (uint32_t)channelId);
    }

exit:
    return(errorCode);
}

int32_t EDMA_configChannel(EDMA_Handle handle, EDMA_channelConfig_t const *config,
    bool isEnableChannel)
{
    EDMA_Config_t *edmaConfig = (EDMA_Config_t *) handle;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;
    EDMA_paramSetConfig_t const *pSetCfg;
    uint8_t channelId = config->channelId;
    uint16_t paramId = config->paramId;

    edmaObj = edmaConfig->object;
    hwAttrs = edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    /* validate configuration */
    errorCode = EDMA_validate_channel_config(handle, hwAttrs, config);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }
#endif

    pSetCfg = &config->paramSetConfig;

    EDMA3EnableChInShadowReg(ccBaseAddr, (uint32_t)config->channelType, (uint32_t)channelId);
    EDMA3MapChToEvtQ(ccBaseAddr, (uint32_t)config->channelType, (uint32_t)channelId, 
        (uint32_t)config->eventQueueId);

    /* disable channel (event) first */
    errorCode = EDMA_disableChannel(handle, channelId, config->channelType);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    if (config->channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
        /* map channel to param set */
        {
            uint32_t temp = (uint32_t) paramId;
            EDMA3MapQdmaChToPaRAM(ccBaseAddr, (uint32_t)channelId, &temp);
        }

        /* set trigger word */
        EDMA3SetQdmaTrigWord(ccBaseAddr, (uint32_t)channelId, config->qdmaParamTriggerWordOffset);

        edmaObj->qdmaObj[channelId].triggerWordAddress = ccBaseAddr +
              EDMA_TPCC_OPT((uint32_t)paramId) + config->qdmaParamTriggerWordOffset * sizeof(uint32_t);
    }
    else
    {   /* config->channelType is EDMA3_CHANNEL_TYPE_DMA */
        if (hwAttrs->isChannelMapExist == true)
        {
            /* map channel to param set */
            EDMA3ChannelToParamMap(ccBaseAddr, (uint32_t)channelId, (uint32_t)paramId);
        }
    }

    EDMA3ClrIntr(ccBaseAddr, (uint32_t)pSetCfg->transferCompletionCode);

    EDMA_paramConfig_assist(ccBaseAddr, paramId, pSetCfg, config->transferCompletionCallbackFxn,
        config->transferCompletionCallbackFxnArg, edmaObj);

    /* store trigger word param value for Qdma channel */
    if (config->channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
        uint32_t *pArray; /* param Set as array of uint32_t */

        pArray = (uint32_t *)(ccBaseAddr + EDMA_TPCC_OPT((uint32_t)paramId));
        edmaObj->qdmaObj[channelId].triggerWordValue = pArray[config->qdmaParamTriggerWordOffset];
    }

    if (isEnableChannel == true)
    {
        errorCode = EDMA_enableChannel(handle, channelId, config->channelType);
    }

exit:
    return(errorCode);
}

int32_t EDMA_enableChannel(EDMA_Handle handle, uint8_t channelId, uint8_t channelType)
{
    EDMA_Config_t *edmaConfig = (EDMA_Config_t *) handle;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
    errorCode = EDMA_validate_channelIdchannelType(channelId, channelType);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }
#endif

    /* enable events, clear if applicable */
    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
        /* clears QSECR and QEMCR */
        EDMA3QdmaClrMissEvt(ccBaseAddr, (uint32_t)channelId);

        /* enable the Qdma event */
        EDMA3EnableQdmaEvt(ccBaseAddr, (uint32_t)channelId);

        /* From this point onwards, QDMA channel is armed and ready to be
           triggered by either through @ref EDMA_startTransfer API or
           through @ref EDMA_startTransferQdmaTrigWordWrite API */
    }
    else /* config->channelType is EDMA3_CHANNEL_TYPE_DMA */
    {
        /* clear SECR & EMCR to clean any previous NULL request */
        EDMA3ClrMissEvt(ccBaseAddr, (uint32_t)channelId);

        /* Set EESR to enable event */
        EDMA3EnableDmaEvt(ccBaseAddr, (uint32_t)channelId);

        /* From this point onwards, DMA channel is armed and ready to be
           triggered by the event happening in the SoC using 
           the @ref edmaStartTransfer API */

        /* Note: For manual trigger we don't need to clear/enable DMA event as it is
           ignored when triggering channel manual (through a write to ESR) but not
           doing these additional operations above would require a config parameter
           to distinguish manual or event triggered (or both) just to serve the purpose
           of not executing code that is harmless/don't care for the manual trigger */

    }
exit:
    return(errorCode);
}

int32_t EDMA_disableChannel(EDMA_Handle handle, uint8_t channelId, uint8_t channelType)
{
    EDMA_Config_t *edmaConfig = (EDMA_Config_t *) handle;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
    errorCode = EDMA_validate_channelIdchannelType(channelId, channelType);
    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }
#endif

    /* enable events, clear if applicable */
    if (channelType == (uint8_t)EDMA3_CHANNEL_TYPE_QDMA)
    {
        /* disable the Qdma event */
        EDMA3DisableQdmaEvt(ccBaseAddr, (uint32_t)channelId);

        /* clears QSECR and QEMCR */
        EDMA3QdmaClrMissEvt(ccBaseAddr, (uint32_t)channelId);

        /* From this point onwards, QDMA channel is armed and ready to be
           triggered by either through @ref EDMA_startTransfer API or
           through @ref EDMA_startTransferQdmaTrigWordWrite API */
    }
    else /* config->channelType is EDMA3_CHANNEL_TYPE_DMA */
    {
        /* disable DMA event */
        EDMA3DisableDmaEvt(ccBaseAddr, (uint32_t)channelId);

        /* clear SECR & EMCR to clean any previous NULL request */
        EDMA3ClrMissEvt(ccBaseAddr, (uint32_t)channelId);

    }
exit:
    return(errorCode);
}

int32_t EDMA_configParamSet(EDMA_Handle handle, uint16_t paramId,
    EDMA_paramConfig_t const *config)
{
    EDMA_Config_t *edmaConfig = (EDMA_Config_t *) handle;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

    edmaObj = edmaConfig->object;
    hwAttrs = edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    if (config == NULL)
    {
        errorCode = EDMA_E_INVALID__CONFIG_POINTER_NULL;
        goto exit;
    }
    /* validate configuration */
    errorCode = EDMA_validate_param_config(handle, hwAttrs, paramId, &config->paramSetConfig,
        config->transferCompletionCallbackFxn);

    if (errorCode != EDMA_NO_ERROR)
    {
        goto exit;
    }
#endif

    EDMA_paramConfig_assist(ccBaseAddr, paramId, &config->paramSetConfig,
        config->transferCompletionCallbackFxn, config->transferCompletionCallbackFxnArg,
        edmaObj);

exit:
    return(errorCode);
}

static void EDMA_paramConfig_assist(uint32_t ccBaseAddr, uint16_t paramId,
    EDMA_paramSetConfig_t const *pSetCfg,
    EDMA_transferCompletionCallbackFxn_t transferCompletionCallbackFxn,
    uintptr_t transferCompletionCallbackFxnArg, EDMA_Object_t *edmaObj
    )
{
    EDMA_paramSetConfig_assist(ccBaseAddr, paramId, pSetCfg);

    /* Register transfer completion call back function */
    if (transferCompletionCallbackFxn != NULL)
    {
        edmaObj->transferCompleteCallbackFxn[pSetCfg->transferCompletionCode] =
            transferCompletionCallbackFxn;

        edmaObj->transferCompleteCallbackFxnArg[pSetCfg->transferCompletionCode] =
            transferCompletionCallbackFxnArg;

        /* enable interrupt */
        EDMA3EnableEvtIntr(ccBaseAddr, (uint32_t)pSetCfg->transferCompletionCode);
    }
    else
    {
        /* disable interrupt, this is important for polled mode transfers we don't
           want interrupt to trigger (there is an assert in the interrupt to capture
           this situation) */
        EDMA3DisableEvtIntr(ccBaseAddr, (uint32_t)pSetCfg->transferCompletionCode);
        
        edmaObj->transferCompleteCallbackFxn[pSetCfg->transferCompletionCode] =
            NULL;
    }
}

int32_t EDMA_linkParamSets(EDMA_Handle handle, uint16_t fromParamId, uint16_t toParamId)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    if ((fromParamId >= hwAttrs->numParamSets) ||
        (toParamId >= hwAttrs->numParamSets))
    {
        errorCode = EDMA_E_INVALID__PARAM_ID;
        goto exit;
    }
#endif
/* Do not use LinkChannel API, it changes toParamId's TCC to that of
   fromParamId and also has issue JIRA: PRSDK-1055 */
#if 0
    EDMA3LinkChannel(ccBaseAddr, (uint32_t)fromParamId, (uint32_t)toParamId);
#else
    HW_WR_FIELD32(ccBaseAddr + EDMA_TPCC_LNK((uint32_t)fromParamId), EDMA_TPCC_LNK_LINK,
        ccBaseAddr + EDMA_TPCC_OPT((uint32_t)toParamId));
#endif

exit:
    return(errorCode);
}

int32_t EDMA_chainChannels(EDMA_Handle handle, uint16_t fromParamId, uint8_t toChannelId)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
    if (toChannelId >= EDMA_NUM_DMA_CHANNELS)
    {
        errorCode = EDMA_E_INVALID__DMA_CHANNEL_ID;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    if (fromParamId >= hwAttrs->numParamSets)
    {
        errorCode = EDMA_E_INVALID__PARAM_ID;
        goto exit;
    }
#endif

    HW_WR_FIELD32(ccBaseAddr + EDMA_TPCC_OPT((uint32_t)fromParamId), EDMA_TPCC_OPT_TCC,
        toChannelId);

exit:
    return(errorCode);
}

int32_t EDMA_startTransfer(EDMA_Handle handle, uint8_t channelId, uint8_t channelType)
{
    return(EDMA_startTransfer_assist(handle, channelId, channelType, false));
}

int32_t EDMA_startDmaTransfer(EDMA_Handle handle, uint8_t channelId)
{
    return(EDMA_startTransfer_assist(handle, channelId,
        (uint8_t)EDMA3_CHANNEL_TYPE_DMA, false));
}

int32_t EDMA_startQdmaTransfer(EDMA_Handle handle, uint8_t channelId)
{
    return(EDMA_startTransfer_assist(handle, channelId,
        (uint8_t)EDMA3_CHANNEL_TYPE_QDMA, false));
}

int32_t EDMA_startFinalQdmaTransfer(EDMA_Handle handle, uint8_t channelId)
{
    return(EDMA_startTransfer_assist(handle, channelId,
        (uint8_t)EDMA3_CHANNEL_TYPE_QDMA, true));
}

int32_t EDMA_isTransferComplete(EDMA_Handle handle, uint8_t transferCompletionCode,
    bool *isTransferComplete)
{
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;    
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
    if (transferCompletionCode >= EDMA_NUM_TCC)
    {
        errorCode = EDMA_E_INVALID__TRANSFER_COMPLETION_CODE;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;
    
    /* Transfer completion call back function must not be registered for the
       transfer completion code */
    if (edmaObj->transferCompleteCallbackFxn[transferCompletionCode] != NULL)
    {
        errorCode = EDMA_E_UNEXPECTED__ATTEMPT_TO_TEST_COMPLETION;

        /* Additional protection in case user does not check for error code,
           make the transfer never succeed. */
        *isTransferComplete = false;
        goto exit;
    }
    
    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

    if (transferCompletionCode < 32U)
    {
        *isTransferComplete = (bool)((EDMA3GetIntrStatus(ccBaseAddr) &
                                   ((uint32_t)1 << transferCompletionCode)) != 0U);
    }
    else
    {
        *isTransferComplete = (bool)((EDMA3IntrStatusHighGet(ccBaseAddr) &
                                   ((uint32_t)1 << (transferCompletionCode-32U))) != 0U);
    }

    /* if transfer is complete, clear IPR(H) bit to allow new transfer */
    if (*isTransferComplete == true)
    {
        EDMA3ClrIntr(ccBaseAddr, (uint32_t)transferCompletionCode);
    }

exit:
    return(errorCode);
}

int32_t EDMA_setDestinationAddress(EDMA_Handle handle,
    uint16_t paramId,
    uint32_t destinationAddress)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK    
    if (paramId >= hwAttrs->numParamSets)
    {
        errorCode = EDMA_E_INVALID__PARAM_ID;
        goto exit;
    }    
#endif
    HW_WR_REG32(ccBaseAddr + EDMA_TPCC_DST((uint32_t)paramId), destinationAddress);
exit:
    return(errorCode);
}

int32_t EDMA_setSourceAddress(EDMA_Handle handle,
    uint16_t paramId,
    uint32_t sourceAddress)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK    
    if (paramId >= hwAttrs->numParamSets)
    {
        errorCode = EDMA_E_INVALID__PARAM_ID;
        goto exit;
    }    
#endif
    HW_WR_REG32(ccBaseAddr + EDMA_TPCC_SRC((uint32_t)paramId), sourceAddress);
exit:
    return(errorCode);
}
    
int32_t EDMA_close(EDMA_Handle handle)
{
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    int32_t errorCode = EDMA_NO_ERROR;
    uint8_t tc;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;
    hwAttrs = edmaConfig->hwAttrs;

    /*if (edmaObj->isOpen == false) {
        generate error
    }*/

    if (hwAttrs->transferCompletionInterruptNum != EDMA_INTERRUPT_NOT_CONNECTED_ID)
    {
        HwiP_delete(edmaObj->hwiTransferCompleteHandle);
    }

    if (hwAttrs->errorInterruptNum != EDMA_INTERRUPT_NOT_CONNECTED_ID)
    {
        HwiP_delete(edmaObj->hwiErrorHandle);
    }

    for (tc = 0; tc < hwAttrs->numEventQueues; tc++)
    {
        if (hwAttrs->transferControllerErrorInterruptNum[tc] != EDMA_INTERRUPT_NOT_CONNECTED_ID)
        {
            HwiP_delete(edmaObj->hwiTransferControllerErrorHandle[tc]);
        }
    }

    edmaObj->isOpen = false;

exit:
    return(errorCode);
}

uint8_t EDMA_getNumInstances(void)
{
    return((uint8_t)EDMA_NUM_CC);
}

int32_t EDMA_init(uint8_t instanceId)
{
    uint32_t ccBaseAddr, tcBaseAddr, errClrRegAddr;
    uint8_t transCompCode, tc;
    uint16_t paramId;
    uint8_t channelId;
    EDMA3CCPaRAMEntry paramSet;
    int32_t errorCode = EDMA_NO_ERROR;

#if (true != 1)
#error define "true" is not 1
#endif

#ifdef EDMA_PARAM_CHECK
    if (instanceId >= EDMA_NUM_CC)
    {
        errorCode = EDMA_E_INVALID__INSTANCE_ID;
        goto exit;
    }
#endif

    /* initialize EDMA object */
    memset(&EDMA_object[instanceId], 0, sizeof(EDMA_Object_t));
    EDMA_object[instanceId].isOpen = false;

    /* h/w reset values of param set */
    memset(&paramSet, 0, sizeof(paramSet));

#ifdef EDMA_DBG
    memset(&edmaDbg, 0, sizeof(edmaDbg));
#endif

    /* All AR devices have no-region, although internally regionId variable
       is initialized to 0, intentionally indicate this through API */
    EDMAsetRegion(0);

    ccBaseAddr = gEdmaHwAttrs[instanceId].CCbaseAddress;
    EDMA3Init(ccBaseAddr, (uint32_t)0);
    /* do things now that EDMA3Init is (unfortunately not doing) */
    /* disable DMA events */
    for (channelId = 0; channelId < EDMA_NUM_DMA_CHANNELS; channelId++)
    {
        EDMA3DisableDmaEvt(ccBaseAddr, (uint32_t)channelId);
        EDMA3ClrEvt(ccBaseAddr, (uint32_t)channelId);
        EDMA3ClrMissEvt(ccBaseAddr, (uint32_t)channelId);
    }
    /* disable and clear event interrupts */
    for (transCompCode = 0; transCompCode < EDMA_NUM_TCC; transCompCode++)
    {
        EDMA3DisableEvtIntr(ccBaseAddr, (uint32_t)transCompCode);
        EDMA3ClrIntr(ccBaseAddr, (uint32_t)transCompCode);
    }
    for (channelId = 0; channelId < EDMA_NUM_QDMA_CHANNELS; channelId++)
    {
        EDMA3DisableQdmaEvt(ccBaseAddr, (uint32_t)channelId);
        EDMA3QdmaClrMissEvt(ccBaseAddr, (uint32_t)channelId);
    }
    /* clear tansfer controller errors */
    for (tc = 0; tc < gEdmaHwAttrs[instanceId].numEventQueues; tc++)
    {
        tcBaseAddr = gEdmaHwAttrs[instanceId].TCbaseAddress[tc];
        errClrRegAddr = tcBaseAddr + EDMA_TC_ERRCLR;
        HW_WR_FIELD32(errClrRegAddr, EDMA_TC_ERRCLR_TRERR, 1);
        HW_WR_FIELD32(errClrRegAddr, EDMA_TC_ERRCLR_MMRAERR, 1);
        HW_WR_FIELD32(errClrRegAddr, EDMA_TC_ERRCLR_BUSERR, 1);
    }
    /* cleanup Params, note h/w reset state is all 0s, must be done after
       disabling/clearning channel events (in particular QDMA) */
    for (paramId = 0; paramId < gEdmaHwAttrs[instanceId].numParamSets; paramId++)
    {
        EDMA3SetPaRAM(ccBaseAddr, (uint32_t)paramId, &paramSet);
    }
exit:
    return(errorCode);
}

int32_t EDMA_getStatusInfo(EDMA_Handle handle, EDMA_statusInfo_t *status)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    uint8_t queueId;
    EDMA_queueStatusInfo_t *qStatus;
    uint8_t qEntryIndx, numOutstandingEntries;
    uint32_t qStatRegAddr, qEntry0Addr, qEntryAddrJump, ccStatRegAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }
    if (status == NULL)
    {
        errorCode = EDMA_E_INVALID__STATUS_POINTER_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

    /* queue status processing */
    for (queueId = 0; queueId < hwAttrs->numEventQueues; queueId++)
    {
        qStatRegAddr = ccBaseAddr + EDMA_TPCC_QSTATN((uint32_t)queueId);
        qStatus = &status->queue[queueId];

        qStatus->isThresholdExceeded =
            (bool) HW_RD_FIELD32(qStatRegAddr, EDMA_TPCC_QSTATN_THRXCD);

        qStatus->maxQueueEntries =
            (uint8_t) HW_RD_FIELD32(qStatRegAddr, EDMA_TPCC_QSTATN_WM);

        numOutstandingEntries =
            (uint8_t) HW_RD_FIELD32(qStatRegAddr, EDMA_TPCC_QSTATN_NUMVAL);
        qStatus->numOutstandingEntries = numOutstandingEntries;

        /* process outstanding queue entries first,
           starting from position indicated by the h/w, note queue circularity */
        qEntryIndx = (uint8_t) HW_RD_FIELD32(qStatRegAddr, EDMA_TPCC_QSTATN_STRTPTR);
        qEntry0Addr = ccBaseAddr + EDMA_TPCC_QNE0((uint8_t)queueId);
        qEntryAddrJump = EDMA_TPCC_QNE1(0U) - EDMA_TPCC_QNE0(0U);
        EDMA_update_queue_entries(qEntry0Addr, qEntryAddrJump,
                                  numOutstandingEntries,
                                  &qEntryIndx, qStatus->outstandingEntries);
        /* process non-outstanding entries */
        EDMA_update_queue_entries(qEntry0Addr, qEntryAddrJump,
                                 EDMA_NUM_QUEUE_ENTRIES - numOutstandingEntries,
                                 &qEntryIndx, qStatus->dequeuedEntries);
    }

    /* CCSTAT processing */
    ccStatRegAddr = ccBaseAddr + EDMA_TPCC_CCSTAT;

    status->numOutstandingCompletionRequests =
        HW_RD_FIELD32(ccStatRegAddr, EDMA_TPCC_CCSTAT_COMPACTV);

    status->isAnyDmaChannelActive =
        HW_RD_FIELD32(ccStatRegAddr, EDMA_TPCC_CCSTAT_EVTACTV);

    status->isAnyQdmaChannelActive =
        HW_RD_FIELD32(ccStatRegAddr, EDMA_TPCC_CCSTAT_QEVTACTV);

    status->isWriteStatusActive =
        HW_RD_FIELD32(ccStatRegAddr, EDMA_TPCC_CCSTAT_WSTATACTV);

    status->isAnyTransferActive =
        HW_RD_FIELD32(ccStatRegAddr, EDMA_TPCC_CCSTAT_TRACTV);

    status->isAnythingActive =
        HW_RD_FIELD32(ccStatRegAddr, EDMA_TPCC_CCSTAT_ACTV);

exit:
    return(errorCode);
}

EDMA_Handle EDMA_open(uint8_t instanceId, int32_t *errorCode,
    EDMA_instanceInfo_t *instanceInfo)
{
    EDMA_Handle handle = NULL;
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    HwiP_Params hwiParams;
    uint8_t tc;
    /* EDMA configuration structure */
    static const EDMA_Config_t EDMA_config[EDMA_NUM_CC] =
    {
    #if defined(SOC_XWR14XX)
        {
            .object   = &EDMA_object[0],
            .hwAttrs  = &gEdmaHwAttrs[0]
        }
    #endif
    #if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
        {
            .object  = &EDMA_object[0],
            .hwAttrs = &gEdmaHwAttrs[0]
        },
        {
            .object  = &EDMA_object[1],
            .hwAttrs = &gEdmaHwAttrs[1]
        }
    #endif
    };

#ifdef EDMA_PARAM_CHECK
    /* error checking */
    if (errorCode == NULL)
    {
        handle = NULL;
        goto exit;
    }
    if (instanceId >= EDMA_NUM_CC)
    {
        *errorCode = EDMA_E_INVALID__INSTANCE_ID;
        handle = NULL;
        goto exit;
    }
    if (instanceInfo == NULL)
    {
        *errorCode = EDMA_E_INVALID__INSTANCEINFO_POINTER_NULL;
        handle = NULL;
        goto exit;
    }
#endif

    /* Get handle for this driver instance */
    handle = (EDMA_Handle)&(EDMA_config[instanceId]);

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;

#ifdef EDMA_DBG
    edmaDbg.edmaObj = edmaObj;
#endif

    if (edmaObj->isOpen == true) {
        *errorCode = EDMA_E_UNEXPECTED__EDMA_INSTANCE_REOPEN;
        goto exit;
    }

    hwAttrs =  edmaConfig->hwAttrs;
    instanceInfo->numEventQueues = hwAttrs->numEventQueues;
    instanceInfo->numParamSets = hwAttrs->numParamSets;
    instanceInfo->isChannelMapExist = hwAttrs->isChannelMapExist;
    instanceInfo->isTransferCompletionInterruptConnected =
        (hwAttrs->transferCompletionInterruptNum != EDMA_INTERRUPT_NOT_CONNECTED_ID);
    instanceInfo->isErrorInterruptConnected = (hwAttrs->errorInterruptNum != EDMA_INTERRUPT_NOT_CONNECTED_ID);
    for (tc = 0; tc < hwAttrs->numEventQueues; tc++)
    {
        instanceInfo->isTransferControllerErrorInterruptConnected[tc] =
            (hwAttrs->transferControllerErrorInterruptNum[tc] != EDMA_INTERRUPT_NOT_CONNECTED_ID);
    }

    /* register transfer complete interrupt handler */
    if (hwAttrs->transferCompletionInterruptNum != EDMA_INTERRUPT_NOT_CONNECTED_ID)
    {
        HwiP_Params_init(&hwiParams);
        hwiParams.name = "EDMA_transferComplete_isr";
        hwiParams.arg  = (uintptr_t)handle;
        edmaObj->hwiTransferCompleteHandle =
                HwiP_create((int32_t)hwAttrs->transferCompletionInterruptNum,
                            EDMA_transferComplete_isr, &hwiParams);

        if (edmaObj->hwiTransferCompleteHandle == NULL)
        {
            *errorCode = EDMA_E_OSAL__HWIP_CREATE_TRANSFER_COMPLETION_ISR_RETURNED_NULL;
            goto exit;
        }
    }

    /* register error interrupt handler */
    if (hwAttrs->errorInterruptNum != EDMA_INTERRUPT_NOT_CONNECTED_ID)
    {
        HwiP_Params_init(&hwiParams);
        hwiParams.name = "EDMA_error_isr";
        hwiParams.arg  = (uintptr_t)handle;
        edmaObj->hwiErrorHandle = HwiP_create((int32_t)hwAttrs->errorInterruptNum,
                                                  EDMA_error_isr, &hwiParams);

        if (edmaObj->hwiErrorHandle == NULL)
        {
            *errorCode = EDMA_E_OSAL__HWIP_CREATE_ERROR_ISR_RETURNED_NULL;
            goto exit;
        }
    }

    /* register transfer controller error handler */
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "EDMA_transferController_error_isr";
    for (tc = 0; tc < hwAttrs->numEventQueues; tc++)
    {
        if (hwAttrs->transferControllerErrorInterruptNum[tc] != EDMA_INTERRUPT_NOT_CONNECTED_ID)
        {
            edmaObj->transferControllerErrorIsrArgInfo[tc].handle = handle;
            edmaObj->transferControllerErrorIsrArgInfo[tc].transferControllerId = tc;
            hwiParams.arg  = (uintptr_t) &edmaObj->transferControllerErrorIsrArgInfo[tc];
            edmaObj->hwiTransferControllerErrorHandle[tc] =
                HwiP_create((int32_t)hwAttrs->transferControllerErrorInterruptNum[tc],
                            EDMA_transferController_error_isr, &hwiParams);
            if (edmaObj->hwiTransferControllerErrorHandle[tc] == NULL)
            {
                *errorCode = EDMA_E_OSAL__HWIP_CREATE_TRANSFER_CONTROLLER_ERROR_ISRS_RETURNED_NULL;
                goto exit;
            }
        }
    }

    edmaObj->isOpen = true;
    *errorCode = EDMA_NO_ERROR;

exit:
    return(handle);
}

int32_t EDMA_configErrorMonitoring(EDMA_Handle handle, EDMA_errorConfig_t const *config)
{
    EDMA_Config_t *edmaConfig;
    EDMA_Object_t *edmaObj;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t ccBaseAddr;
    uint8_t threshold;
    uint8_t queueId, tc;
    int32_t errorCode = EDMA_NO_ERROR;

    edmaConfig = (EDMA_Config_t *) handle;
    edmaObj = edmaConfig->object;
    hwAttrs = edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }

    if (config == NULL)
    {
        errorCode = EDMA_E_INVALID__CONFIG_POINTER_NULL;
        goto exit;
    }
#endif
    
    /* event queue threshold configuration */
    if (config->isEventQueueThresholdingEnabled == true)
    {
#ifdef EDMA_PARAM_CHECK
        if (config->eventQueueThreshold > EDMA_EVENT_QUEUE_THRESHOLD_MAX)
        {
            errorCode = EDMA_E_INVALID__EVENT_QUEUE_THRESHOLD;
            goto exit;
        }
        threshold = config->eventQueueThreshold;
#endif
    }
    else
    {
        threshold = EDMA_EVENT_QUEUE_THRESHOLDING_DISABLED;
    }

    if (config->isConfigAllEventQueues == true)
    {
        for (queueId = 0; queueId < hwAttrs->numEventQueues; queueId++)
        {
            EDMA_configQueueThreshold(ccBaseAddr, queueId, threshold);
        }
    }
    else
    {
#ifdef EDMA_PARAM_CHECK
        if (config->eventQueueId >= hwAttrs->numEventQueues) {
            errorCode = EDMA_E_INVALID__EVENT_QUEUE_ID;
            goto exit;
        }
#endif
        EDMA_configQueueThreshold(ccBaseAddr, config->eventQueueId, threshold);
    }

    /* transfer controller error configuration */
    if (config->isConfigAllTransferControllers == true)
    {
        for (tc = 0; tc < hwAttrs->numEventQueues; tc++)
        {
            EDMA_configTransferControllerError(hwAttrs->TCbaseAddress[tc],
                config->isEnableAllTransferControllerErrors,
                &config->transferControllerErrorConfig);
        }
    }
    else
    {
#ifdef EDMA_PARAM_CHECK
        if (config->transferControllerId >= hwAttrs->numEventQueues) {
            errorCode = EDMA_E_INVALID__TRANSFER_CONTROLLER_ID;
            goto exit;
        }
#endif
        EDMA_configTransferControllerError(hwAttrs->TCbaseAddress[config->transferControllerId],
            config->isEnableAllTransferControllerErrors,
            &config->transferControllerErrorConfig);
    }

    edmaObj->errorCallbackFxn = config->callbackFxn;
    edmaObj->transferControllerErrorCallbackFxn = config->transferControllerCallbackFxn;
exit:
    return(errorCode);
}

int32_t EDMA_configPerformance(EDMA_Handle handle,
    EDMA_performanceConfig_t const *config)
{
    EDMA_Config_t *edmaConfig;
    EDMA_hwAttrs_t const *hwAttrs;
    uint32_t tcBaseAddr;
    uint8_t tc;
    uint32_t ccBaseAddr;
    int32_t errorCode = EDMA_NO_ERROR;

#ifdef EDMA_PARAM_CHECK
    if (handle == NULL)
    {
        errorCode = EDMA_E_INVALID__HANDLE_NULL;
        goto exit;
    }

    if (config == NULL)
    {
        errorCode = EDMA_E_INVALID__CONFIG_POINTER_NULL;
        goto exit;
    }
#endif

    edmaConfig = (EDMA_Config_t *) handle;
    hwAttrs =  edmaConfig->hwAttrs;
    ccBaseAddr = hwAttrs->CCbaseAddress;

#ifdef EDMA_PARAM_CHECK
    if (config->transferControllerReadRate > EDMA_READ_RATE_RANGE_CHECK_MAX)
    {
        errorCode = EDMA_E_INVALID__TRANSFER_CONTROLLER_READ_RATE;
        goto exit;
    }

    if (config->queuePriority > EDMA_QUEUE_PRIORITIY_RANGE_CHECK_MAX)
    {
        errorCode = EDMA_E_INVALID__QUEUE_PRIORITY;
        goto exit;
    }
#endif

    if (config->isConfigAllTransferControllers == true)
    {
        for (tc = 0; tc < hwAttrs->numEventQueues; tc++)
        {
            tcBaseAddr = hwAttrs->TCbaseAddress[tc];
            HW_WR_FIELD32(tcBaseAddr + EDMA_TC_RDRATE, EDMA_TC_RDRATE_RDRATE,
                          config->transferControllerReadRate);

            EDMA_configQueuePriority(ccBaseAddr, tc, config->queuePriority);
        }
    }
    else
    {
#ifdef EDMA_PARAM_CHECK
        if (config->transferControllerId >= hwAttrs->numEventQueues) {
            errorCode = EDMA_E_INVALID__TRANSFER_CONTROLLER_ID;
            goto exit;
        }
#endif

        tcBaseAddr = hwAttrs->TCbaseAddress[config->transferControllerId];
        HW_WR_FIELD32(tcBaseAddr + EDMA_TC_RDRATE, EDMA_TC_RDRATE_RDRATE,
                      config->transferControllerReadRate);

        EDMA_configQueuePriority(ccBaseAddr, config->transferControllerId,
                                config->queuePriority);
    }

exit:
    return(errorCode);
}
