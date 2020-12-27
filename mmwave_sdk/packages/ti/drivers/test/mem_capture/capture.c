/**
 *   @file  capture.c
 *
 *   @brief
 *      Memory Capture Profile written to conform to the Test Framework.
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

/* mmWave SDK Include Files: */
#include <ti/drivers/soc/soc.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/test/common/framework.h>

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/

static TestFmk_ProfileHandle TestFmkCapture_init
(
    TestFmk_Handle      fmkHandle,
    TestFmk_InitCfg*    ptrInitCfg,
    SemaphoreP_Handle   fmkSemHandle,
    int32_t*            errCode
);
static int32_t TestFmkCapture_open
(
    TestFmk_ProfileHandle   handle,
    MMWave_OpenCfg*         ptrOpenCfg,
    int32_t*                errCode
);
static int32_t TestFmkCapture_finalizeCfg
(
    TestFmk_ProfileHandle   handle,
    TestFmk_Cfg*            ptrTestFmkCfg,
    int32_t*                errCode
);
static int32_t TestFmkCapture_execute
(
    TestFmk_ProfileHandle handle,
    TestFmk_Result*       ptrResult,
    int32_t*              errCode
);
static int32_t TestFmkCapture_control
(
    TestFmk_ProfileHandle   handle,
    uint32_t                cmd,
    void*                   arg,
    uint32_t                argLen,
    int32_t*                errCode
);
static int32_t TestFmkCapture_start  (TestFmk_ProfileHandle handle, int32_t* errCode);
static int32_t TestFmkCapture_stop   (TestFmk_ProfileHandle handle, int32_t* errCode);
static int32_t TestFmkCapture_close  (TestFmk_ProfileHandle handle, int32_t* errCode);
static int32_t TestFmkCapture_deinit (TestFmk_ProfileHandle handle, int32_t* errCode);
static void    TestFmkCapture_chirpAvailable (TestFmk_ProfileHandle handle);

/**************************************************************************
 ************************* Global Declarations ****************************
 **************************************************************************/

/** @addtogroup CAPTURE_PROFILE_GLOBAL
 @{ */

/**
 * @brief   Capture Profile Configuration: This should be used while loading
 * the profile into the Test Framework
 */
TestFmk_ProfileCfg gCaptureProfileCfg =
{
    TestFmkCapture_init,            /* Initialization Function:         */
    TestFmkCapture_open,            /* Open Function:                   */
    TestFmkCapture_finalizeCfg,     /* Finalize Configuration Function: */
    TestFmkCapture_start,           /* Start Function:                  */
    TestFmkCapture_execute,         /* Execute Function:                */
    TestFmkCapture_control,         /* Control Function:                */
    TestFmkCapture_stop,            /* Stop Function:                   */
    TestFmkCapture_close,           /* Close Function:                  */
    TestFmkCapture_deinit,          /* Deinitialization Function:       */
    TestFmkCapture_chirpAvailable,  /* Chirp Available Function:        */
    NULL                            /* Frame Start Function:            */
};

/**
 * @brief   This is the data cube data buffer which is exposed to the application and is
 * used to store the ADC samples. Applications would need to place this in an appropriate
 * memory section using the linker command file.
 */
#pragma DATA_SECTION(gDataCube, ".dataCubeMemory");
#pragma DATA_ALIGN(gDataCube, 8);
#ifdef SOC_XWR16XX
#ifdef SUBSYS_MSS
uint8_t gDataCube[SOC_XWR16XX_MSS_L3RAM_SIZE];
#else
uint8_t gDataCube[SOC_XWR16XX_DSS_L3RAM_SIZE];
#endif
#elif defined(SOC_XWR18XX)
#ifdef SUBSYS_MSS
uint8_t gDataCube[SOC_L3RAM_SIZE];
#else
uint8_t gDataCube[SOC_L3RAM_SIZE];
#endif
#elif defined(SOC_XWR68XX)
#ifdef SUBSYS_MSS
uint8_t gDataCube[SOC_L3RAM_SIZE];
#else
uint8_t gDataCube[SOC_L3RAM_SIZE];
#endif
#elif defined (SOC_XWR14XX)
uint8_t gDataCube[SOC_XWR14XX_MSS_L3RAM_SIZE];
#else
#error "Error: Unsupported platform"
#endif

/**
@}
*/

/**************************************************************************
 ************************** Local Structures ******************************
 **************************************************************************/

/** @addtogroup CAPTURE_PROFILE_INTERNAL_DATA_STRUCTURES
 @{ */

/**
 * @brief
 *  Capture Profile MCB
 *
 * @details
 *  The structure is used to track all the information which is required for
 *  the Capture Profile to execute.
 */
typedef struct CaptureProfile_MCB_t
{
    /**
     * @brief   SOC Handle
     */
    SOC_Handle          socHandle;

    /**
     * @brief   Test Framework Handle:
     */
    TestFmk_Handle      fmkHandle;

    /**
     * @brief   EDMA Handle
     */
    EDMA_Handle         edmaHandle[EDMA_NUM_CC];

    /**
     * @brief   Handle to the framework semaphore:
     */
    SemaphoreP_Handle   fmkSemHandle;

    /**
     * @brief   Flag which indicates the status of the EDMA Transfer
     */
    volatile bool       isEDMATranferStarted;

    /**
     * @brief   Flag which indicates that the chirp was detected
     */
    volatile bool       chirpAvailableDetected;

    /**
     * @brief   Counter which tracks the number of Chirp Available Interrupts.
     */
    uint32_t            numChirpAvailable;

    /**
     * @brief   Counter which tracks the number of EDMA Transfer Interrupts.
     */
    uint32_t            dmaIntCounter;

    /**
     * @brief   EDMA Channel Identifier
     */
    uint8_t             channelId;

    /**
     * @brief   Link EDMA Channel Identifier
     */
    uint8_t             linkChannelId;

    /**
     * @brief   Frame Count used to configure DMA channel
     */
    uint8_t             dmaFrameCnt;

    /**
     * @brief   Element size (in number of bytes) used to configure DMA channel
     */
    uint16_t            dmaElemSize;

    /**
     * @brief   Number of bytes of each dma transfer. It is used to calculate next buffer address.
     */
    uint16_t            dmaBlockSize;

    /**
     * @brief   Current data cube address
     */
    uint32_t            currDstAddr;

    /**
     * @brief   End of data cube address
     */
    uint32_t            endDstAddr;
}CaptureProfile_MCB;

/**
@}
*/

/**************************************************************************
 ************************** Capture Functions *****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Profile Chirp Available Function which is invoked by the framework
 *      on the reception of the chirp available ISR
 *
 *  @param[in]  handle
 *      Profile handle
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmkCapture_chirpAvailable (TestFmk_ProfileHandle handle)
{
    CaptureProfile_MCB*     ptrCaptureProfile;

    /* Get the capture profile MCB */
    ptrCaptureProfile = (CaptureProfile_MCB*)handle;

    /* Increment the counter: */
    ptrCaptureProfile->numChirpAvailable++;

    /* Set the flag to indicate that the chirp is available: */
    ptrCaptureProfile->chirpAvailableDetected = true;
    return;
}

/**
 *  @b Description
 *  @n
 *      This is the function which is registered by the profile and is invoked
 *      on the completion of the EDMA Transfer
 *
 *  @param[in]  arg
 *      This is the handle to the profile
 *  @param[in]  tcCode
 *      Transfer completion code for the channel that completes transfer.
 *
 *  @retval
 *      Not Applicable.
 */
static void TestFmkCapture_edmaCallbackFxn(uintptr_t arg, uint8_t tcCode)
{
    CaptureProfile_MCB*     ptrCaptureProfile;

    /* Get the capture profile MCB */
    ptrCaptureProfile = (CaptureProfile_MCB*)arg;

    /* Track the number of EDMA Interrupts */
    ptrCaptureProfile->dmaIntCounter++;

    /* Reset the flag to indicate that the transfer was completed: */
    ptrCaptureProfile->isEDMATranferStarted = false;

    /* Update the current destination address: */
    ptrCaptureProfile->currDstAddr = ptrCaptureProfile->currDstAddr + ptrCaptureProfile->dmaBlockSize;
}

/**
 *  @b Description
 *  @n
 *      Profile Execute Function which is invoked by the framework
 *      to execute the profile
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[out]  ptrResult
 *      Pointer to the result
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmkCapture_execute
(
    TestFmk_ProfileHandle   handle,
    TestFmk_Result*         ptrResult,
    int32_t*                errCode
)
{
    CaptureProfile_MCB*     ptrCaptureProfile;
    int32_t                 retVal = MINUS_ONE;

    /* Get the capture profile MCB */
    ptrCaptureProfile = (CaptureProfile_MCB*)handle;

    /* Did we get a chirp interrupt? */
    if (ptrCaptureProfile->chirpAvailableDetected == false)
    {
        /* NO: There is nothing to do. This is not an error */
        retVal = 0;
        goto exit;
    }

    /* Reset the flag: */
    ptrCaptureProfile->chirpAvailableDetected = false;

    /* Sanity Check: Previous EDMA Transfers should be completed before initiating a new transfer */
    DebugP_assert (ptrCaptureProfile->isEDMATranferStarted == false);

    /* Can we DMA the data? Do we have sufficient space? */
    if( (ptrCaptureProfile->currDstAddr + ptrCaptureProfile->dmaBlockSize) < ptrCaptureProfile->endDstAddr)
    {
        /* YES: The ADC sample can be placed into the Data Cube Memory: */
        *errCode = EDMA_setDestinationAddress (ptrCaptureProfile->edmaHandle[0],
                                               (uint16_t)ptrCaptureProfile->channelId,
                                               ptrCaptureProfile->currDstAddr);
        if(*errCode != EDMA_NO_ERROR)
        {
            /* Error: Unable to set the destination address. Error code is setup. */
            goto exit;
        }

        /* Trigger DMA transfer */
        *errCode = EDMA_startDmaTransfer(ptrCaptureProfile->edmaHandle[0], ptrCaptureProfile->channelId);
        if(*errCode != EDMA_NO_ERROR)
        {
            /* Error: Unable to trigger the DMA Transfer. Error code is setup. */
            goto exit;
        }

        /* EDMA Transfer has been started: */
        ptrCaptureProfile->isEDMATranferStarted = true;
    }

    /* We have successfully executed the profile: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Profile Start Function which is invoked by the framework
 *      during the start process.
 *
 *  @param[in]  handle
 *      Handle to the Profile
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCapture_start (TestFmk_ProfileHandle handle, int32_t* errCode)
{
    CaptureProfile_MCB*     ptrCaptureProfile;
    uint32_t                dstAddress;

    /* Get the capture profile MCB */
    ptrCaptureProfile = (CaptureProfile_MCB*)handle;

    /* Reset the statistics: */
    ptrCaptureProfile->dmaIntCounter     = 0U;
    ptrCaptureProfile->numChirpAvailable = 0U;

    /* Get the destination address: */
    dstAddress = SOC_translateAddress((uint32_t)&gDataCube[0], SOC_TranslateAddr_Dir_TO_EDMA, NULL);

    /* Reset the current destination address: */
    ptrCaptureProfile->currDstAddr = dstAddress;
    ptrCaptureProfile->endDstAddr  = (uint32_t)(dstAddress + sizeof(gDataCube));

    /* Reset the data cube memory: */
    memset ((void*)&gDataCube[0], 0xCC, sizeof(gDataCube));

    /* There is no chirp available interrupt detected */
    ptrCaptureProfile->chirpAvailableDetected = false;

    /* There is no EDMA Transfer which has been started */
    ptrCaptureProfile->isEDMATranferStarted   = false;
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Profile Stop Function which is invoked by the framework
 *      during the stop process.
 *
 *  @param[in]  handle
 *      Handle to the Profile
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCapture_stop (TestFmk_ProfileHandle handle, int32_t* errCode)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Profile registered function which provides "Capture Profile" specific
 *      configurability.
 *
 *  @param[in]  handle
 *      Handle to the Module
 *  @param[in]  cmd
 *      Capture Profile specific commands
 *  @param[in]  arg
 *      Command specific arguments
 *  @param[in]  argLen
 *      Length of the arguments which is also command specific
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCapture_control
(
    TestFmk_ProfileHandle   handle,
    uint32_t                cmd,
    void*                   arg,
    uint32_t                argLen,
    int32_t*                errCode
)
{
    /* The capture profile does not support any command. */
    *errCode = TESTFMK_ENOTSUP;
    return -1;
}

/**
 *  @b Description
 *  @n
 *      Profile registered function which is invoked to finalize
 *      the configuration.
 *
 *  @param[in]  handle
 *      Handle to the profile
 *  @param[in]  ptrTestFmkCfg
 *      Pointer to the Test Framework configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCapture_finalizeCfg
(
    TestFmk_ProfileHandle   handle,
    TestFmk_Cfg*            ptrTestFmkCfg,
    int32_t*                errCode
)
{
    CaptureProfile_MCB*     ptrCaptureProfile;
    int32_t                 retVal = MINUS_ONE;
    uint8_t                 numBytesPerSample;
    uint32_t                maxADCSize;
    uint32_t                srcAddress;
    uint32_t                dstAddress;
    uint8_t                 channel;
    uint8_t                 numRxChannels = 0U;
    uint32_t                adcBaseAddress;
    EDMA_channelConfig_t    edmaChannelCfg;
    EDMA_paramSetConfig_t*  ptrEDMAParamSetCfg;
    EDMA_paramConfig_t      edmaParamCfg;

    /* Get the capture profile MCB */
    ptrCaptureProfile = (CaptureProfile_MCB*)handle;
    DebugP_assert (ptrCaptureProfile != NULL);

    /* Sanity Checking: The capture profile only supports Frame Mode */
    if (ptrTestFmkCfg->dfeDataOutputMode != MMWave_DFEDataOutputMode_FRAME)
    {
        /* Error: Profile does not support this mode */
        *errCode = TESTFMK_ENOTSUP;
        goto exit;
    }

    /* Sanity Checking: There should be at least 1 active channel */
    if (ptrTestFmkCfg->rxChannelEn == 0U)
    {
        /* Error: Invalid configuration */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Get the EDMA Handles from the Framework Module: */
    if (TestFmk_ioctl (ptrCaptureProfile->fmkHandle, TEST_FMK_CMD_GET_EDMA_HANDLE,
                       (void*)&ptrCaptureProfile->edmaHandle[0],
                       sizeof(ptrCaptureProfile->edmaHandle),
                       errCode) < 0)
    {
        /* Error: Unable to get the EDMA Handle: */
        goto exit;
    }

    /* Disable the EDMA Channel Identifier */
    *errCode = EDMA_disableChannel(ptrCaptureProfile->edmaHandle[0], ptrCaptureProfile->channelId,
                                   EDMA3_CHANNEL_TYPE_DMA);
    if (*errCode != EDMA_NO_ERROR)
    {
        /* Error: Unable to disable the channel */
        goto exit;
    }

    /* Calculate the number of bytes per sample for REAL or COMPLEX */
    if (ptrTestFmkCfg->adcFmt == 0)
    {
        /* COMPLEX: There are 4 bytes per sample */
        numBytesPerSample = 4U;
    }
    else
    {
        /* REAL: There are 2 bytes per sample */
        numBytesPerSample = 2U;
    }

    /* Cycle through and determine the number of channels which are active: */
    for (channel = 0U; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++)
    {
        /* Is the channel enabled? */
        if (ptrTestFmkCfg->rxChannelEn & (0x1<<channel))
        {
            /* Increment and track the number of active channels */
            numRxChannels = numRxChannels + 1U;
        }
    }

    /************************************************************************
     * Platform Specific Definitions:
     ************************************************************************/
#ifdef SOC_XWR16XX
#ifdef SUBSYS_MSS
    adcBaseAddress = SOC_XWR16XX_MSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_XWR16XX_MSS_ADCBUF_SIZE;
#else
    adcBaseAddress = SOC_XWR16XX_DSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_XWR16XX_DSS_ADCBUF_SIZE;
#endif
#elif defined(SOC_XWR18XX)
#ifdef SUBSYS_MSS
    adcBaseAddress = SOC_XWR18XX_MSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_ADCBUF_SIZE;
#else
    adcBaseAddress = SOC_XWR18XX_DSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_ADCBUF_SIZE;
#endif
#elif defined(SOC_XWR68XX)
#ifdef SUBSYS_MSS
    adcBaseAddress = SOC_XWR68XX_MSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_ADCBUF_SIZE;
#else
    adcBaseAddress = SOC_XWR68XX_DSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_ADCBUF_SIZE;
#endif
#elif defined(SOC_XWR14XX)
    adcBaseAddress = SOC_XWR14XX_MSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_XWR14XX_MSS_ADCBUF_SIZE;
#else
#error "Error: Unsupported platform"
#endif

    /* Setup the DMA Parameters: */
    ptrCaptureProfile->dmaElemSize  = ptrTestFmkCfg->u.frameCfg.numADCSamples * numBytesPerSample * numRxChannels;
    ptrCaptureProfile->dmaFrameCnt  = ptrTestFmkCfg->chirpThreshold;
    ptrCaptureProfile->dmaBlockSize = ptrCaptureProfile->dmaElemSize * ptrCaptureProfile->dmaFrameCnt;

    /* Validate data size: We cannot DMA beyond the maximum ADC size for the device */
    if ((ptrCaptureProfile->dmaElemSize * ptrCaptureProfile->dmaFrameCnt) > maxADCSize)
    {
        /* Error: Invalid size */
        *errCode = TESTFMK_EPROFERR;
        goto exit;
    }

    /* Get the source & destination address: */
    srcAddress = SOC_translateAddress(adcBaseAddress, SOC_TranslateAddr_Dir_TO_EDMA, NULL);
    dstAddress = SOC_translateAddress((uint32_t)&gDataCube[0], SOC_TranslateAddr_Dir_TO_EDMA,NULL);

    /* Initialize EDMA channel configuration */
    memset((void *)&edmaChannelCfg, 0, sizeof(EDMA_channelConfig_t));

    /* Populate the EDMA channel configuration: */
    edmaChannelCfg.channelId                        = ptrCaptureProfile->channelId;
    edmaChannelCfg.channelType                      = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    edmaChannelCfg.paramId                          = ptrCaptureProfile->channelId;
    edmaChannelCfg.eventQueueId                     = 1U;
    edmaChannelCfg.transferCompletionCallbackFxn    = TestFmkCapture_edmaCallbackFxn;
    edmaChannelCfg.transferCompletionCallbackFxnArg = (uintptr_t)ptrCaptureProfile;

    /* Get the EDMA Channel Parameter configuration: */
    ptrEDMAParamSetCfg = &edmaChannelCfg.paramSetConfig;

    /* Populate the EDMA Channel Param set configuration: */
    ptrEDMAParamSetCfg->sourceAddress                          = (uint32_t)srcAddress;
    ptrEDMAParamSetCfg->destinationAddress                     = (uint32_t)dstAddress;
    ptrEDMAParamSetCfg->aCount                                 = ptrCaptureProfile->dmaElemSize;
    ptrEDMAParamSetCfg->bCount                                 = ptrCaptureProfile->dmaFrameCnt;
    ptrEDMAParamSetCfg->cCount                                 = 1U;
    ptrEDMAParamSetCfg->bCountReload                           = ptrEDMAParamSetCfg->bCount;
    ptrEDMAParamSetCfg->sourceBindex                           = ptrCaptureProfile->dmaElemSize;
    ptrEDMAParamSetCfg->destinationBindex                      = ptrCaptureProfile->dmaElemSize;
    ptrEDMAParamSetCfg->sourceCindex                           = 0;
    ptrEDMAParamSetCfg->destinationCindex                      = 0;
    ptrEDMAParamSetCfg->linkAddress                            = EDMA_NULL_LINK_ADDRESS;
    ptrEDMAParamSetCfg->transferType                           = (uint8_t)EDMA3_SYNC_AB;
    ptrEDMAParamSetCfg->transferCompletionCode                 = ptrCaptureProfile->channelId;
    ptrEDMAParamSetCfg->sourceAddressingMode                   = (uint8_t)EDMA3_ADDRESSING_MODE_LINEAR;
    ptrEDMAParamSetCfg->destinationAddressingMode              = (uint8_t)EDMA3_ADDRESSING_MODE_LINEAR;
    ptrEDMAParamSetCfg->fifoWidth                              = (uint8_t)EDMA3_FIFO_WIDTH_8BIT;
    ptrEDMAParamSetCfg->isStaticSet                            = false;
    ptrEDMAParamSetCfg->isEarlyCompletion                      = false;
    ptrEDMAParamSetCfg->isFinalTransferInterruptEnabled        = true;
    ptrEDMAParamSetCfg->isIntermediateTransferInterruptEnabled = false;
    ptrEDMAParamSetCfg->isFinalChainingEnabled                 = false;
    ptrEDMAParamSetCfg->isIntermediateChainingEnabled          = false;

    /* Configure the EDMA Channel: */
    *errCode = EDMA_configChannel(ptrCaptureProfile->edmaHandle[0], &edmaChannelCfg, false);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Populate the EDMA Parameter configuration: */
    memcpy ((void *)&edmaParamCfg.paramSetConfig, (void*)ptrEDMAParamSetCfg, sizeof(EDMA_paramSetConfig_t));
    edmaParamCfg.transferCompletionCallbackFxn    = TestFmkCapture_edmaCallbackFxn;
    edmaParamCfg.transferCompletionCallbackFxnArg = (uintptr_t)ptrCaptureProfile;

    /* Configure the EDMA Parameter set: */
    *errCode = EDMA_configParamSet(ptrCaptureProfile->edmaHandle[0], ptrCaptureProfile->linkChannelId, &edmaParamCfg);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Link the channels: */
    *errCode = EDMA_linkParamSets (ptrCaptureProfile->edmaHandle[0], ptrCaptureProfile->channelId, ptrCaptureProfile->linkChannelId);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Link the channels: */
    *errCode = EDMA_linkParamSets (ptrCaptureProfile->edmaHandle[0], ptrCaptureProfile->linkChannelId, ptrCaptureProfile->linkChannelId);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* There is no EDMA Transfer in progress: */
    ptrCaptureProfile->isEDMATranferStarted = false;

    /* Control comes here implies that the capture profile has been configured successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the registered streaming profile function which is invoked once the
 *      Module is opened.
 *
 *  @param[in]  handle
 *      Handle to the profile
 *  @param[in]  ptrOpenCfg
 *      Pointer to the mmWave open configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCapture_open
(
    TestFmk_ProfileHandle   handle,
    MMWave_OpenCfg*         ptrOpenCfg,
    int32_t*                errCode
)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the registered streaming profile function which is invoked once the
 *      Module is closed
 *
 *  @param[in]  handle
 *      Handle to the profile
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCapture_close
(
    TestFmk_ProfileHandle   handle,
    int32_t*                errCode
)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Profile Initialization Function which is invoked by the framework
 *      during the Initialization process.
 *
 *  @param[in]  fmkHandle
 *      Handle to the Test Framework Module
 *  @param[in]  ptrInitCfg
 *      Pointer to the initialization configuration
 *  @param[in]  fmkSemHandle
 *      Handle to the framework semaphore
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static TestFmk_ProfileHandle TestFmkCapture_init
(
    TestFmk_Handle      fmkHandle,
    TestFmk_InitCfg*    ptrInitCfg,
    SemaphoreP_Handle   fmkSemHandle,
    int32_t*            errCode
)
{
    CaptureProfile_MCB*     ptrCaptureProfile;
    TestFmk_ProfileHandle   retHandle = NULL;

    /* Allocate memory for the streaming profile: */
    ptrCaptureProfile = (CaptureProfile_MCB*)MemoryP_ctrlAlloc (sizeof(CaptureProfile_MCB), 0);
    if (ptrCaptureProfile == NULL)
    {
        *errCode = TESTFMK_ENOMEM;
        goto exit;
    }

    /* Initialize the allocated memory */
    memset ((void*)ptrCaptureProfile, 0, sizeof(CaptureProfile_MCB));

    /* Populate the capture profile: */
    ptrCaptureProfile->socHandle     = ptrInitCfg->socHandle;
    ptrCaptureProfile->fmkHandle     = fmkHandle;
    ptrCaptureProfile->fmkSemHandle  = fmkSemHandle;
    ptrCaptureProfile->channelId     = EDMA_TPCC0_REQ_FREE_13;
    ptrCaptureProfile->linkChannelId = EDMA_NUM_DMA_CHANNELS;

    /* Setup the profile handle: */
    retHandle = (TestFmk_ProfileHandle)ptrCaptureProfile;

exit:
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      This is the streaming profile initialization function which is invoked
 *      after the profile has been deinitialized by the framework
 *
 *  @param[in]  handle
 *      Handle to the profile
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CAPTURE_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCapture_deinit (TestFmk_ProfileHandle handle, int32_t* errCode)
{
    CaptureProfile_MCB*  ptrCaptureProfile;
    int32_t              retVal = MINUS_ONE;

    /* Get the streaming profile */
    ptrCaptureProfile = (CaptureProfile_MCB*)handle;
    DebugP_assert (ptrCaptureProfile != NULL);

    /* Cleanup the memory: */
    MemoryP_ctrlFree (ptrCaptureProfile, sizeof(CaptureProfile_MCB));

    /* Streaming Profile has been deinitialized*/
    retVal = 0;

    return retVal;
}


