/**
 *   @file  cbuff_csi.c
 *
 *   @brief
 *      The file implements the CBUFF Driver CSI-2 Interface
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
#include <ti/drivers/cbuff/include/cbuff_internal.h>

/**************************************************************************
 ************************** CBUFF CSI Local Defintions ********************
 **************************************************************************/

#define CSI_DATAID_RAW8                  0x2AU
#define CSI_DATAID_RAW12                 0x2CU
#define CSI_DATAID_RAW14                 0x2DU

#define CSI_DATAID_VSYNC_START           0x0U
#define CSI_DATAID_HSYNC_START           0x2U
#define CSI_DATAID_VSYNC_END             0x1U
#define CSI_DATAID_HSYNC_END             0x3U

/**************************************************************************
 ************************** CBUFF CSI Interface Functions *****************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to populate and initialize the linked list
 *      configuration data structure when the high speed interface is CSI-2
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[out] ptrLinkListParams
 *      Pointer to the linked parameters which is populated
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CBUFF_initCSILinkListParams
(
    CBUFF_Session*          ptrSession,
    CBUFF_LinkListParams*   ptrLinkListParams
)
{
    /* Sanity Check: Ensure that the back pointer is valid. */
    DebugP_assert (ptrSession->ptrDriverMCB != NULL);

    /* CSI-2: Setup the default linked list configuration */
    ptrLinkListParams->vcNum             = 0U;
    ptrLinkListParams->hsyncStart        = 1U;
    ptrLinkListParams->hsyncEnd          = 0U;
    ptrLinkListParams->crcEnable         = 0U;
    ptrLinkListParams->lpHdrEn           = 1U;
    ptrLinkListParams->transferSize      = 0U;
    ptrLinkListParams->totalTransferSize = 0U;

    /* TODO: 96bit alignment check needs to be added */
    ptrLinkListParams->align96       = 0U;
    ptrLinkListParams->dataFmtMap    = 0U;

    /* Setup the CSI-2 Output Format: Translate the output format to the TRM specified values */
    switch (ptrSession->ptrDriverMCB->initCfg.outputDataFmt)
    {
        case CBUFF_OutputDataFmt_12bit:
        {
            ptrLinkListParams->outputDataFmt = 2U;
            break;
        }
        case CBUFF_OutputDataFmt_14bit:
        {
            ptrLinkListParams->outputDataFmt = 1U;
            break;
        }
        case CBUFF_OutputDataFmt_16bit:
        {
            ptrLinkListParams->outputDataFmt = 0U;
            break;
        }
        default:
        {
            /* Error: Control should never come here */
            DebugP_assert (0);
            break;
        }
    }

    /* Setup the threshold */
    ptrLinkListParams->threshold = CSL_FMKR (18U, 16U, (uint32_t)ptrSession->dmaNum)    |
                                   CSL_FMKR (14U,  8U, CBUFF_LL_WRITE_THRESHOLD)        |
                                   CSL_FMKR (6U,   0U, CBUFF_LL_READ_THRESHOLD);
}

/**
 *  @b Description
 *  @n
 *      The function is used to finalize the linked list parameters
 *      for the CSI interface
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[in]  ptrLinkListParams
 *      Pointer to the linked list parameters
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CBUFF_finalizeCSILinkListParams
(
    CBUFF_Session*          ptrSession,
    CBUFF_LinkListParams*   ptrLinkListParams
)
{
    uint32_t                lpDataID;
    uint32_t                lpDataSzInBytes;
    CBUFF_LinkListParams*   ptrFirstLinkListParams;

    /* Sanity Check: Ensure that the back pointer is valid. */
    DebugP_assert (ptrSession->ptrDriverMCB != NULL);

    switch (ptrSession->ptrDriverMCB->initCfg.outputDataFmt)
    {
        case CBUFF_OutputDataFmt_16bit:
        {
            lpDataID        = CSI_DATAID_RAW8;
            lpDataSzInBytes = (ptrLinkListParams->transferSize * 16U) / 8U;
            break;
        }
        case CBUFF_OutputDataFmt_14bit:
        {
            lpDataID        = CSI_DATAID_RAW14;
            lpDataSzInBytes = (ptrLinkListParams->transferSize * 14U) / 8U;
            break;
        }
        case CBUFF_OutputDataFmt_12bit:
        {
            lpDataID        = CSI_DATAID_RAW12;
            lpDataSzInBytes = (ptrLinkListParams->transferSize * 12U) / 8U;
            break;
        }
        default:
        {
            /* Error: This should not occur. Bad argument */
            DebugP_assert (0);
            break;
        }
    }

    /********************************************************************************
     * In the case of CSI-2: We need to program the long packet header only for the
     * first LL Entry. Are we the first LL Entry being added?
     ********************************************************************************/
    if (ptrLinkListParams->lpHdrEn == 1U)
    {
        /****************************************************************************
         * YES: This is the FIRST LL Entry.
         * - Track the total transfer size
         * - Program the LP Header accounting for the total transfer size
         ****************************************************************************/
        ptrLinkListParams->totalTransferSize = ptrLinkListParams->totalTransferSize + lpDataSzInBytes;
        ptrLinkListParams->lpHeaderValue = CSL_FMKR (31U, 24U, 0xEC)                                            |
                                           CSL_FMKR (23U, 8U, (uint32_t)ptrLinkListParams->totalTransferSize)   |
                                           CSL_FMKR (7U,  0U, lpDataID);
    }
    else
    {
        /****************************************************************************
         * NO: This is not the FIRST LL Entry.
         * - Get the first LL Entry
         * - Program the LP Header accounting for the total transfer size
         ****************************************************************************/
        ptrFirstLinkListParams = &ptrSession->linkedListTracker[0];

        /* Sanity Check: LP Header should always be set for this entry */
        DebugP_assert (ptrFirstLinkListParams->lpHdrEn == 1U);

        /* There are certain fields which are applicable only for the FIRST LL Entry */
        ptrLinkListParams->lpHeaderValue     = 0U;
        ptrLinkListParams->totalTransferSize = 0U;

        /* Update the first LL Entry fields:-
         * - Keep track of the total transfer size
         * - Initialize the LONG Packet Header */
        ptrFirstLinkListParams->totalTransferSize = ptrFirstLinkListParams->totalTransferSize + lpDataSzInBytes;
        ptrFirstLinkListParams->lpHeaderValue = CSL_FMKR (31U, 24U, 0xEC)                                               |
                                                CSL_FMKR (23U, 8U, (uint32_t)ptrFirstLinkListParams->totalTransferSize) |
                                                CSL_FMKR (7U,  0U, lpDataID);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the CSI Interface for the specific session
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t CBUFF_openCSI (CBUFF_Session* ptrSession, int32_t* errCode)
{
    int32_t     retVal = -1;

    /* Sanity Check: The CSI High speed interface does not support continuous mode */
    if (ptrSession->sessionCfg.executionMode == CBUFF_SessionExecuteMode_HW)
    {
        if (ptrSession->sessionCfg.u.hwCfg.opMode == CBUFF_OperationalMode_CONTINUOUS)
        {
            /* Error: The feature is not supported */
            *errCode = CBUFF_ENOTSUP;
            goto exit;
        }
    }

    /* CSI has been opened for the session */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the CSI Interface for the specific session
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t CBUFF_closeCSI (CBUFF_Session* ptrSession, int32_t* errCode)
{
    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to open the CSI Interface
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the CBUFF Driver MCB
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t CBUFF_initCSI (CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode)
{
    int32_t retVal = 0;

    /* Sanity Check: Validate the arguments */
    if (ptrDriverMCB->initCfg.u.csiCfg.handle == NULL)
    {
        /* Error: Invalid arguments */
        *errCode = CBUFF_EINVAL;
        retVal   = -1;
        goto exit;
    }

    /* Get the pointer to the CSI protocol engine */
    ptrDriverMCB->ptrCSIProtReg = (CSI2PROTOCOLENGINERegs*)gCBUFFHwAttribute.csiProtocolEngineAddress;

    /* Select the CSI-2 as the high speed interface */
    ptrDriverMCB->ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CONFIG_REG_0, 0U, 0U, 0U);

    /* Enable 1 Virtual channel on the CSI-2 Interface */
    CBUFF_setVC (ptrDriverMCB->ptrCBUFFReg, 0U, 1U, 1U);

    /* Setup the FIFO free threshold */
    ptrDriverMCB->ptrCBUFFReg->CFG_FIFO_FREE_THRESHOLD = 0x8U;

    /***************************************************************************************
     * TODO: Need clarifications on this configuration
     ***************************************************************************************/
    {
        /* TODO: Sync code packet values */
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_HSVAL = ((uint32_t)0xEC << 24)|(CSI_DATAID_HSYNC_START);
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_HEVAL = ((uint32_t)0xEC << 24)|(CSI_DATAID_HSYNC_END);
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_VSVAL = ((uint32_t)0xEC << 24)|(CSI_DATAID_VSYNC_START);
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_VEVAL = ((uint32_t)0xEC << 24)|(CSI_DATAID_VSYNC_END);
    }

    /* Configure the Short Packet, Long Packet and Payload address:
     * TODO: Need to check why only the lower order 24 bits are set. */
    ptrDriverMCB->ptrCBUFFReg->CFG_SPHDR_ADDRESS =
            (((uint32_t)&ptrDriverMCB->ptrCSIProtReg->CSI2VCPARAMS[0].CSI2_VC_SHORT_PACKET_HEADER) & 0xFFFFFFFU);
    ptrDriverMCB->ptrCBUFFReg->CFG_LPHDR_ADDRESS =
            (((uint32_t)&ptrDriverMCB->ptrCSIProtReg->CSI2VCPARAMS[0].CSI2_VC_LONG_PACKET_HEADER)  & 0xFFFFFFFU);
    ptrDriverMCB->ptrCBUFFReg->CFG_LPPYLD_ADDRESS =
            (((uint32_t)&ptrDriverMCB->ptrCSIProtReg->CSI2VCPARAMS[0].CSI2_VC_LONG_PACKET_PAYLOAD) & 0xFFFFFFFU);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the CSI Interface
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the CBUFF Driver MCB
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t CBUFF_deinitCSI (CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode)
{
    return 0;
}

