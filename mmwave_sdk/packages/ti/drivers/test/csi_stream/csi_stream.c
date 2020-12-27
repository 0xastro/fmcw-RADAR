/**
 *   @file  csi_stream.c
 *
 *   @brief
 *      CSI Streaming Profile written for the XWR14xx
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
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/drivers/csi/csi.h>
#include <ti/drivers/test/common/framework.h>
#include <ti/drivers/test/csi_stream/csi_stream.h>

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/

static TestFmk_ProfileHandle TestFmkCSIStream_init
(
    TestFmk_Handle      fmkHandle,
    TestFmk_InitCfg*    ptrInitCfg,
    SemaphoreP_Handle   fmkSemHandle,
    int32_t*            errCode
);
static int32_t TestFmkCSIStream_open
(
    TestFmk_ProfileHandle   handle,
    MMWave_OpenCfg*         ptrOpenCfg,
    int32_t*                errCode
);
static int32_t TestFmkCSIStream_finalizeCfg
(
    TestFmk_ProfileHandle   handle,
    TestFmk_Cfg*            ptrTestFmkCfg,
    int32_t*                errCode
);
static int32_t TestFmkCSIStream_execute
(
    TestFmk_ProfileHandle handle,
    TestFmk_Result*       ptrResult,
    int32_t*              errCode
);
static int32_t TestFmkCSIStream_control
(
    TestFmk_ProfileHandle   handle,
    uint32_t                cmd,
    void*                   arg,
    uint32_t                argLen,
    int32_t*                errCode
);
static int32_t TestFmkCSIStream_start  (TestFmk_ProfileHandle handle, int32_t* errCode);
static int32_t TestFmkCSIStream_stop   (TestFmk_ProfileHandle handle, int32_t* errCode);
static int32_t TestFmkCSIStream_close  (TestFmk_ProfileHandle handle, int32_t* errCode);
static int32_t TestFmkCSIStream_deinit (TestFmk_ProfileHandle handle, int32_t* errCode);
static void    TestFmkCSIStream_chirpAvailable (TestFmk_ProfileHandle handle);

static int32_t TestFmkCSIStream_EDMAAllocateCBUFFChannel (CBUFF_EDMAInfo* ptrEDMAInfo, CBUFF_EDMAChannelCfg* ptrEDMACfg);
static void TestFmkCSIStream_EDMAFreeCBUFFChannel (CBUFF_EDMAChannelCfg* ptrEDMACfg);

/**************************************************************************
 ************************** Local Structures ******************************
 **************************************************************************/

/** @addtogroup CSI_STREAM_PROFILE_INTERNAL_DATA_STRUCTURES
 @{ */

/**
 * @brief   This is the maximum number of EDMA Channels which is used by
 * the CSI Stream Profile.
 */
#define CSI_STREAM_PROFILE_MAX_EDMA_CHANNEL                     4U

/**
 * @brief
 *  CSI Stream Profile MCB
 *
 * @details
 *  The structure is used to track all the information which is required for
 *  the CSI Stream Profile to execute.
 */
typedef struct CSIStreamProfile_MCB_t
{
    /**
     * @brief   SOC Handle
     */
    SOC_Handle              socHandle;

    /**
     * @brief   Test Framework Handle:
     */
    TestFmk_Handle          fmkHandle;

    /**
     * @brief   EDMA Handle
     */
    EDMA_Handle             edmaHandle[EDMA_NUM_CC];

    /**
     * @brief   ADCBUF Driver Handle:
     */
    ADCBuf_Handle           adcBufHandle;

    /**
     * @brief   Handle to the framework semaphore:
     */
    SemaphoreP_Handle       fmkSemHandle;

    /**
     * @brief   Handle to the CSI Driver
     */
    CSI_Handle              csiHandle;

    /**
     * @brief   Handle to the CBUFF Driver
     */
    CBUFF_Handle            cbuffHandle;

    /**
     * @brief   Handle to the HW CBUFF Session Handle
     */
    CBUFF_SessionHandle     hwSessionHandle;

    /**
     * @brief   Handle to the HW CBUFF Session Configuration
     */
    CBUFF_SessionCfg        sessionCfg;

    /**
     * @brief   EDMA Channel Allocator Index: This tracks EDMA channels which have been allocated
     * to the CBUFF Session
     */
    uint8_t                 edmaChannelAllocatorIndex;

    /**
     * @brief   EDMA Channel Resource Table: This is used for creating the CBUFF Session.
     */
    CBUFF_EDMAChannelCfg    sessionEDMAChannelTable[CSI_STREAM_PROFILE_MAX_EDMA_CHANNEL];

    /**
     * @brief   Flag which indicates the status of the EDMA Transfer
     */
    volatile bool           isEDMATranferStarted;

    /**
     * @brief   Flag which indicates that the chirp was detected
     */
    volatile bool           chirpAvailableDetected;

    /**
     * @brief   Number of chirps per frame configured in the CBUFF Session
     */
    uint32_t                numChirpsPerFrame;

    /**
     * @brief   Counter which tracks the number of Chirp Available Interrupts.
     */
    uint32_t                numChirpAvailable;

    /**
     * @brief   Counter which tracks the number of EDMA Transfer Interrupts.
     */
    uint32_t                dmaIntCounter;

    /**
     * @brief   EDMA Channel Identifier
     */
    uint8_t                 channelId;

    /**
     * @brief   Link EDMA Channel Identifier
     */
    uint8_t                 linkChannelId;

    /**
     * @brief   Frame Count used to configure DMA channel
     */
    uint8_t                 dmaFrameCnt;

    /**
     * @brief   Element size (in number of bytes) used to configure DMA channel
     */
    uint16_t                dmaElemSize;

    /**
     * @brief   Number of bytes of each dma transfer. It is used to calculate next buffer address.
     */
    uint16_t                dmaBlockSize;

    /**
     * @brief   Current data cube address
     */
    uint32_t                currDstAddr;

    /**
     * @brief   End of data cube address
     */
    uint32_t                endDstAddr;

    /**
     * @brief   Streaming statistics:
     */
    CBUFF_Stats             cbuffStats;
}CSIStreamProfile_MCB;

/**
@}
*/

/**************************************************************************
 ************************* Global Declarations ****************************
 **************************************************************************/

/** @addtogroup CSI_STREAM_PROFILE_GLOBAL
 @{ */

/**
 * @brief   CSI Stream Profile Configuration: This should be used while loading
 * the profile into the Test Framework
 */
TestFmk_ProfileCfg gCSIStreamProfileCfg =
{
    TestFmkCSIStream_init,            /* Initialization Function:         */
    TestFmkCSIStream_open,            /* Open Function:                   */
    TestFmkCSIStream_finalizeCfg,     /* Finalize Configuration Function: */
    TestFmkCSIStream_start,           /* Start Function:                  */
    TestFmkCSIStream_execute,         /* Execute Function:                */
    TestFmkCSIStream_control,         /* Control Function:                */
    TestFmkCSIStream_stop,            /* Stop Function:                   */
    TestFmkCSIStream_close,           /* Close Function:                  */
    TestFmkCSIStream_deinit,          /* Deinitialization Function:       */
    TestFmkCSIStream_chirpAvailable,  /* Chirp Available Function:        */
    NULL                              /* Frame Start Function:            */
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
#else
uint8_t gDataCube[SOC_XWR14XX_MSS_L3RAM_SIZE];
#endif

/**
 * @brief   This is the pointer to the CSI Stream Profile MCB.
 */
CSIStreamProfile_MCB*   gPtrCSIStreamProfileMCB;

/**
@}
*/

/**************************************************************************
 ************************ CSI Stream Functions ****************************
 **************************************************************************/


/**
 *  @b Description
 *  @n
 *      This is the registered CBUFF EDMA channel allocation function
 *      which allocates EDMA channels for HW Triggered sessions
 *
 *  @param[in]  ptrEDMAInfo
 *      Pointer to the EDMA Information
 *  @param[out]  ptrEDMACfg
 *      Populated EDMA channel configuration
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_EDMAAllocateCBUFFChannel (CBUFF_EDMAInfo* ptrEDMAInfo, CBUFF_EDMAChannelCfg* ptrEDMACfg)
{
    int32_t     retVal = MINUS_ONE;

    /* The CSI Stream Profile only supports a single session. */
    DebugP_assert (ptrEDMAInfo->dmaNum == 0U);

    /* Sanity Check: Are there sufficient EDMA channels? */
    if (gPtrCSIStreamProfileMCB->edmaChannelAllocatorIndex >= CSI_STREAM_PROFILE_MAX_EDMA_CHANNEL)
    {
        /* Error: All the EDMA channels are allocated */
        goto exit;
    }

    /* Copy over the allocated EDMA configuration. */
    memcpy ((void *)ptrEDMACfg,
            (void*)&gPtrCSIStreamProfileMCB->sessionEDMAChannelTable[gPtrCSIStreamProfileMCB->edmaChannelAllocatorIndex],
            sizeof(CBUFF_EDMAChannelCfg));

    /* Increment the allocator index. */
    gPtrCSIStreamProfileMCB->edmaChannelAllocatorIndex++;

    /* EDMA Channel allocated successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the registered CBUFF EDMA channel free function which frees allocated
 *      EDMA channels for HW Triggered sessions
 *
 *  @retval
 *      Not applicable
 */
static void TestFmkCSIStream_EDMAFreeCBUFFChannel (CBUFF_EDMAChannelCfg* ptrEDMACfg)
{
    uint8_t    index;

    /* Cycle through all the CSI Stream EDMA Channel Table */
    for (index = 0U; index < CSI_STREAM_PROFILE_MAX_EDMA_CHANNEL; index++)
    {
        /* Do we have a match? */
        if (memcmp ((void*)ptrEDMACfg,
                    (void*)&gPtrCSIStreamProfileMCB->sessionEDMAChannelTable[index],
                    sizeof(CBUFF_EDMAChannelCfg)) == 0)
        {
            /* YES: Decrement the DMA-0 Table counter: */
            gPtrCSIStreamProfileMCB->edmaChannelAllocatorIndex--;
            break;
        }
    }

    /* Sanity Check: We should have had a match. An assertion is thrown to indicate that the EDMA channel
     * being cleaned up does not belong to the CSI Stream EDMA Channel Table */
    DebugP_assert (index < CSI_STREAM_PROFILE_MAX_EDMA_CHANNEL);
    return;
}

/**
 *  @b Description
 *  @n
 *      Profile Chirp Available Function which is invoked by the framework
 *      on the reception of the chirp available ISR
 *
 *  @param[in]  handle
 *      Profile handle
 *
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmkCSIStream_chirpAvailable (TestFmk_ProfileHandle handle)
{
    CSIStreamProfile_MCB*     ptrCSIStreamProfile;

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;

    /* Increment the counter: */
    ptrCSIStreamProfile->numChirpAvailable++;

    /* Set the flag to indicate that the chirp is available: */
    ptrCSIStreamProfile->chirpAvailableDetected = true;
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
static void TestFmkCSIStream_edmaCallbackFxn(uintptr_t arg, uint8_t tcCode)
{
    CSIStreamProfile_MCB*     ptrCSIStreamProfile;

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)arg;

    /* Track the number of EDMA Interrupts */
    ptrCSIStreamProfile->dmaIntCounter++;

    /* Reset the flag to indicate that the transfer was completed: */
    ptrCSIStreamProfile->isEDMATranferStarted = false;

    /* Update the current destination address: */
    ptrCSIStreamProfile->currDstAddr = ptrCSIStreamProfile->currDstAddr + ptrCSIStreamProfile->dmaBlockSize;
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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmkCSIStream_execute
(
    TestFmk_ProfileHandle   handle,
    TestFmk_Result*         ptrResult,
    int32_t*                errCode
)
{
    CSIStreamProfile_MCB*     ptrCSIStreamProfile;
    int32_t                 retVal = MINUS_ONE;

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;

    /* Did we get a chirp interrupt? */
    if (ptrCSIStreamProfile->chirpAvailableDetected == false)
    {
        /* NO: There is nothing to do. This is not an error */
        retVal = 0;
        goto exit;
    }

    /* Reset the flag: */
    ptrCSIStreamProfile->chirpAvailableDetected = false;

    /* Sanity Check: Previous EDMA Transfers should be completed before initiating a new transfer */
    DebugP_assert (ptrCSIStreamProfile->isEDMATranferStarted == false);

    /* Take a snapshot of the CBUFF statistics: */
    DebugP_assert (CBUFF_control (ptrCSIStreamProfile->cbuffHandle,
                                  CBUFF_Command_GET_CBUFF_STATS,
                                  &ptrCSIStreamProfile->cbuffStats,
                                  sizeof (CBUFF_Stats),
                                  errCode) == 0);

    /* Can we DMA the data? Do we have sufficient space? */
    if( (ptrCSIStreamProfile->currDstAddr + ptrCSIStreamProfile->dmaBlockSize) < ptrCSIStreamProfile->endDstAddr)
    {
        /* YES: The ADC sample can be placed into the Data Cube Memory: */
        *errCode = EDMA_setDestinationAddress (ptrCSIStreamProfile->edmaHandle[0],
                                               (uint16_t)ptrCSIStreamProfile->channelId,
                                               ptrCSIStreamProfile->currDstAddr);
        if(*errCode != EDMA_NO_ERROR)
        {
            /* Error: Unable to set the destination address. Error code is setup. */
            goto exit;
        }

        /* Trigger DMA transfer */
        *errCode = EDMA_startDmaTransfer(ptrCSIStreamProfile->edmaHandle[0], ptrCSIStreamProfile->channelId);
        if(*errCode != EDMA_NO_ERROR)
        {
            /* Error: Unable to trigger the DMA Transfer. Error code is setup. */
            goto exit;
        }

        /* EDMA Transfer has been started: */
        ptrCSIStreamProfile->isEDMATranferStarted = true;
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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_start (TestFmk_ProfileHandle handle, int32_t* errCode)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;
    uint32_t                dstAddress;

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;

    /* Reset the statistics: */
    ptrCSIStreamProfile->dmaIntCounter     = 0U;
    ptrCSIStreamProfile->numChirpAvailable = 0U;

    /* Get the destination address: */
    dstAddress = SOC_translateAddress((uint32_t)&gDataCube[0], SOC_TranslateAddr_Dir_TO_EDMA, NULL);

    /* Reset the current destination address: */
    ptrCSIStreamProfile->currDstAddr = dstAddress;
    ptrCSIStreamProfile->endDstAddr  = (uint32_t)(dstAddress + sizeof(gDataCube));

    /* Reset the data cube memory: */
    memset ((void*)&gDataCube[0], 0xCC, sizeof(gDataCube));

    /* There is no chirp available interrupt detected */
    ptrCSIStreamProfile->chirpAvailableDetected = false;

    /* There is no EDMA Transfer which has been started */
    ptrCSIStreamProfile->isEDMATranferStarted = false;

    /* Activate the CBUFF Session: */
    return CBUFF_activateSession (ptrCSIStreamProfile->hwSessionHandle, errCode);
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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_stop (TestFmk_ProfileHandle handle, int32_t* errCode)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;

    /* Deactivate the streaming session: */
    return CBUFF_deactivateSession (ptrCSIStreamProfile->hwSessionHandle, errCode);
}

/**
 *  @b Description
 *  @n
 *      Profile registered function which provides "CSI Stream Profile" specific
 *      configurability.
 *
 *  @param[in]  handle
 *      Handle to the Module
 *  @param[in]  cmd
 *      Profile specific commands
 *  @param[in]  arg
 *      Command specific arguments
 *  @param[in]  argLen
 *      Length of the arguments which is also command specific
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_control
(
    TestFmk_ProfileHandle   handle,
    uint32_t                cmd,
    void*                   arg,
    uint32_t                argLen,
    int32_t*                errCode
)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;
    int32_t                 retVal = MINUS_ONE;

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;

    /* Sanity Check: The CSI Stream Profile only supports the following command: */
    if (cmd != CSI_STREAM_PROFILE_SET_NUM_CHIRPS_PER_FRAME)
    {
        /* Error: The CSI Stream Profile does not support the command. Please refer
         * to the documentation */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Ensure that the command arguments are correct. */
    if (argLen != sizeof(uint32_t))
    {
        /* Error: Invalid argument. Please refer to the documentation */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Setup the number of chirps per frame. */
    ptrCSIStreamProfile->numChirpsPerFrame = *(uint32_t*)arg;

    /* Profile has been successfully configured: */
    retVal = 0U;

exit:
    return retVal;
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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_finalizeCfg
(
    TestFmk_ProfileHandle   handle,
    TestFmk_Cfg*            ptrTestFmkCfg,
    int32_t*                errCode
)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;
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

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;
    DebugP_assert (ptrCSIStreamProfile != NULL);

    /* Sanity Checking: The CSI Streaming profile only supports Frame Mode */
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

    /* Sanity Checking: Number of chirps per frame should have been configured */
    if (ptrCSIStreamProfile->numChirpsPerFrame == 0U)
    {
        /* Error: Invalid configuration */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Get the EDMA Handles from the Framework Module: */
    if (ptrCSIStreamProfile->edmaHandle[0] == NULL)
    {
        if (TestFmk_ioctl (ptrCSIStreamProfile->fmkHandle, TEST_FMK_CMD_GET_EDMA_HANDLE,
                           (void*)&ptrCSIStreamProfile->edmaHandle[0],
                           sizeof(ptrCSIStreamProfile->edmaHandle),
                           errCode) < 0)
        {
            /* Error: Unable to get the EDMA Handle: */
            goto exit;
        }
    }

    /* Get the ADC Handles from the Framework Module: */
    if (ptrCSIStreamProfile->adcBufHandle == NULL)
    {
        if (TestFmk_ioctl (ptrCSIStreamProfile->fmkHandle, TEST_FMK_CMD_GET_ADCBUF_HANDLE,
                           (void*)&ptrCSIStreamProfile->adcBufHandle,
                           sizeof(ptrCSIStreamProfile->adcBufHandle),
                           errCode) < 0)
        {
            /* Error: Unable to get the ADCBUF Handle: */
            goto exit;
        }
    }

    /* Are we getting reconfigured? */
    if (ptrCSIStreamProfile->hwSessionHandle != NULL)
    {
        /* YES: There was a previous session which existed. We need to delete
         * it because we will be creating a new session here. */
        if (CBUFF_deleteSession (ptrCSIStreamProfile->hwSessionHandle, errCode) < 0)
        {
            /* Error: Unable to delete the session. */
            goto exit;
        }
    }

    /* Disable the EDMA Channel Identifier */
    *errCode = EDMA_disableChannel(ptrCSIStreamProfile->edmaHandle[0],
                                   ptrCSIStreamProfile->channelId,
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

    /* Setup the ADC Base Address */
    adcBaseAddress = SOC_XWR14XX_MSS_ADCBUF_BASE_ADDRESS;
    maxADCSize     = SOC_XWR14XX_MSS_ADCBUF_SIZE;

    /* Setup the DMA Parameters: */
    ptrCSIStreamProfile->dmaElemSize  = ptrTestFmkCfg->u.frameCfg.numADCSamples * numBytesPerSample * numRxChannels;
    ptrCSIStreamProfile->dmaFrameCnt  = ptrTestFmkCfg->chirpThreshold;
    ptrCSIStreamProfile->dmaBlockSize = ptrCSIStreamProfile->dmaElemSize * ptrCSIStreamProfile->dmaFrameCnt;

    /* Validate data size: We cannot DMA beyond the maximum ADC size for the device */
    if ((ptrCSIStreamProfile->dmaElemSize * ptrCSIStreamProfile->dmaFrameCnt) > maxADCSize)
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
    edmaChannelCfg.channelId                        = ptrCSIStreamProfile->channelId;
    edmaChannelCfg.channelType                      = (uint8_t)EDMA3_CHANNEL_TYPE_DMA;
    edmaChannelCfg.paramId                          = ptrCSIStreamProfile->channelId;
    edmaChannelCfg.eventQueueId                     = 1U;
    edmaChannelCfg.transferCompletionCallbackFxn    = TestFmkCSIStream_edmaCallbackFxn;
    edmaChannelCfg.transferCompletionCallbackFxnArg = (uintptr_t)ptrCSIStreamProfile;

    /* Get the EDMA Channel Parameter configuration: */
    ptrEDMAParamSetCfg = &edmaChannelCfg.paramSetConfig;

    /* Populate the EDMA Channel Param set configuration: */
    ptrEDMAParamSetCfg->sourceAddress                          = (uint32_t)srcAddress;
    ptrEDMAParamSetCfg->destinationAddress                     = (uint32_t)dstAddress;
    ptrEDMAParamSetCfg->aCount                                 = ptrCSIStreamProfile->dmaElemSize;
    ptrEDMAParamSetCfg->bCount                                 = ptrCSIStreamProfile->dmaFrameCnt;
    ptrEDMAParamSetCfg->cCount                                 = 1U;
    ptrEDMAParamSetCfg->bCountReload                           = ptrEDMAParamSetCfg->bCount;
    ptrEDMAParamSetCfg->sourceBindex                           = ptrCSIStreamProfile->dmaElemSize;
    ptrEDMAParamSetCfg->destinationBindex                      = ptrCSIStreamProfile->dmaElemSize;
    ptrEDMAParamSetCfg->sourceCindex                           = 0;
    ptrEDMAParamSetCfg->destinationCindex                      = 0;
    ptrEDMAParamSetCfg->linkAddress                            = EDMA_NULL_LINK_ADDRESS;
    ptrEDMAParamSetCfg->transferType                           = (uint8_t)EDMA3_SYNC_AB;
    ptrEDMAParamSetCfg->transferCompletionCode                 = ptrCSIStreamProfile->channelId;
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
    *errCode = EDMA_configChannel(ptrCSIStreamProfile->edmaHandle[0], &edmaChannelCfg, false);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Populate the EDMA Parameter configuration: */
    memcpy ((void *)&edmaParamCfg.paramSetConfig, (void*)ptrEDMAParamSetCfg, sizeof(EDMA_paramSetConfig_t));
    edmaParamCfg.transferCompletionCallbackFxn    = TestFmkCSIStream_edmaCallbackFxn;
    edmaParamCfg.transferCompletionCallbackFxnArg = (uintptr_t)ptrCSIStreamProfile;

    /* Configure the EDMA Parameter set: */
    *errCode = EDMA_configParamSet(ptrCSIStreamProfile->edmaHandle[0], ptrCSIStreamProfile->linkChannelId, &edmaParamCfg);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Link the channels: */
    *errCode = EDMA_linkParamSets (ptrCSIStreamProfile->edmaHandle[0], ptrCSIStreamProfile->channelId, ptrCSIStreamProfile->linkChannelId);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Link the channels: */
    *errCode = EDMA_linkParamSets (ptrCSIStreamProfile->edmaHandle[0], ptrCSIStreamProfile->linkChannelId, ptrCSIStreamProfile->linkChannelId);
    if (*errCode != EDMA_NO_ERROR)
    {
        goto exit;
    }

    /* There is no EDMA Transfer in progress: */
    ptrCSIStreamProfile->isEDMATranferStarted = false;

    /********************************************************************************
     * Populate the CBUFF Session configuration
     ********************************************************************************/
    memset ((void*)&ptrCSIStreamProfile->sessionCfg, 0, sizeof(CBUFF_SessionCfg));

    /* Populate the configuration: */
    ptrCSIStreamProfile->sessionCfg.executionMode          = CBUFF_SessionExecuteMode_HW;
    ptrCSIStreamProfile->sessionCfg.edmaHandle             = ptrCSIStreamProfile->edmaHandle[0];
    ptrCSIStreamProfile->sessionCfg.allocateEDMAChannelFxn = TestFmkCSIStream_EDMAAllocateCBUFFChannel;
    ptrCSIStreamProfile->sessionCfg.freeEDMAChannelFxn     = TestFmkCSIStream_EDMAFreeCBUFFChannel;
    ptrCSIStreamProfile->sessionCfg.frameDoneCallbackFxn   = NULL;
    ptrCSIStreamProfile->sessionCfg.header.size            = 0U;
    ptrCSIStreamProfile->sessionCfg.header.address         = 0U;

    /* Setup the data type: */
    if (ptrTestFmkCfg->adcFmt == 0)
    {
        /* Complex: */
        ptrCSIStreamProfile->sessionCfg.dataType = CBUFF_DataType_COMPLEX;
    }
    else
    {
        /* Real: */
        ptrCSIStreamProfile->sessionCfg.dataType = CBUFF_DataType_REAL;
    }

    /* Setup the data mode: The CBUFF & ADCBUF should be aligned on the data mode */
    if (ptrTestFmkCfg->chInterleave == 1U)
    {
        /* Non-Interleaved data mode: */
        ptrCSIStreamProfile->sessionCfg.u.hwCfg.dataMode = CBUFF_DataMode_NON_INTERLEAVED;
    }
    else
    {
        /* Interleaved data mode: */
        ptrCSIStreamProfile->sessionCfg.u.hwCfg.dataMode = CBUFF_DataMode_INTERLEAVED;
    }

    /* Only ADC Data is going to be streamed out with no header: */
    ptrCSIStreamProfile->sessionCfg.u.hwCfg.adcBufHandle      = ptrCSIStreamProfile->adcBufHandle;
    ptrCSIStreamProfile->sessionCfg.u.hwCfg.dataFormat        = CBUFF_DataFmt_ADC_DATA;
    ptrCSIStreamProfile->sessionCfg.u.hwCfg.numADCSamples     = ptrTestFmkCfg->u.frameCfg.numADCSamples;
    ptrCSIStreamProfile->sessionCfg.u.hwCfg.opMode            = CBUFF_OperationalMode_CHIRP;
    ptrCSIStreamProfile->sessionCfg.u.hwCfg.numChirpsPerFrame = ptrCSIStreamProfile->numChirpsPerFrame;
    ptrCSIStreamProfile->sessionCfg.u.hwCfg.chirpMode         = 1U;

    /* Create the CBUFF session: */
    ptrCSIStreamProfile->hwSessionHandle = CBUFF_createSession (ptrCSIStreamProfile->cbuffHandle,
                                                                &ptrCSIStreamProfile->sessionCfg,
                                                                errCode);
    if (ptrCSIStreamProfile->hwSessionHandle == NULL)
    {
        /* Error: Unable to create the session. */
        goto exit;
    }

    /* Control comes here implies that the CSI Stream Profile has been initialized. */
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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_open
(
    TestFmk_ProfileHandle   handle,
    MMWave_OpenCfg*         ptrOpenCfg,
    int32_t*                errCode
)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;
    CBUFF_InitCfg           initCfg;
    CSI_Cfg                 csiCfg;
    int32_t                 retVal = MINUS_ONE;

    /* Get the CSI Streaming profile MCB */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;

    /*************************************************************************************
     * Open the CSI Driver:
     *************************************************************************************/
    memset ((void *)&csiCfg, 0, sizeof(CSI_Cfg));

    /* Populate the initialization configuration: */
    csiCfg.socHandle               = ptrCSIStreamProfile->socHandle;
    csiCfg.fifoArbiteration        = CSI_FIFO_Arbiteration_RR;
    csiCfg.dataRate                = CSI_DataRate_225Mbps;
    CSI_phyParamsInit (csiCfg.dataRate, &csiCfg.phyParams);

    /* Setup all the virtual channels: */
    csiCfg.virtualChannelCfg[0].channelId  = CSI_VirtualCh_CH0;
    csiCfg.virtualChannelCfg[0].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[0].txFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[1].channelId  = CSI_VirtualCh_CH1;
    csiCfg.virtualChannelCfg[1].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[1].txFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[2].channelId  = CSI_VirtualCh_CH2;
    csiCfg.virtualChannelCfg[2].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[2].txFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[3].channelId  = CSI_VirtualCh_CH3;
    csiCfg.virtualChannelCfg[3].rxFIFOSize = CSI_VC_FIFO_Size_32;
    csiCfg.virtualChannelCfg[3].txFIFOSize = CSI_VC_FIFO_Size_32;

    /* Initialize the CSI Driver: */
    ptrCSIStreamProfile->csiHandle = CSI_open (&csiCfg, errCode);
    if (ptrCSIStreamProfile->csiHandle == NULL)
    {
        /* Error: Unable to initialize the CSI Driver */
        goto exit;
    }

    /*************************************************************************************
     * Open the CBUFF Driver:
     *************************************************************************************/
    memset ((void *)&initCfg, 0, sizeof(CBUFF_InitCfg));

    /* Populate the configuration: */
    initCfg.socHandle                = ptrCSIStreamProfile->socHandle;
    initCfg.enableECC                = 0U;
    initCfg.crcEnable                = 1U;
    initCfg.maxSessions              = 1U;
    initCfg.enableDebugMode          = false;
    initCfg.interface                = CBUFF_Interface_CSI;
    initCfg.u.csiCfg.handle          = ptrCSIStreamProfile->csiHandle;

    /* Setup the data output format: */
    switch (ptrOpenCfg->adcOutCfg.fmt.b2AdcBits)
    {
        case 0U:
        {
            /* 12bit Data Output Format: */
            initCfg.outputDataFmt = CBUFF_OutputDataFmt_12bit;
            break;
        }
        case 1U:
        {
            /* 14bit Data Output Format: */
            initCfg.outputDataFmt = CBUFF_OutputDataFmt_14bit;
            break;
        }
        case 2U:
        {
            /* 16bit Data Output Format: */
            initCfg.outputDataFmt = CBUFF_OutputDataFmt_16bit;
            break;
        }
        default:
        {
            /* Bad bit format: */
            *errCode = TESTFMK_EINVAL;
            goto exit;
        }
    }

    /* Initialize the CBUFF Driver: */
    ptrCSIStreamProfile->cbuffHandle = CBUFF_init (&initCfg, errCode);
    if (ptrCSIStreamProfile->cbuffHandle == NULL)
    {
        /* Error: Unable to initialize the CBUFF Driver */
        goto exit;
    }

    /* CSI Stream Profile was opened: */
    retVal = 0;

exit:
    return retVal;
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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_close
(
    TestFmk_ProfileHandle   handle,
    int32_t*                errCode
)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;
    int32_t                 retVal = MINUS_ONE;

    /* Get the CSI Stream Profile: */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;

    /* Close the CBUFF Driver: */
    retVal = CBUFF_deinit (ptrCSIStreamProfile->cbuffHandle, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to deinitialize the CBUFF */
        goto exit;
    }

    /* Close the CSI Stream: */
    retVal = CSI_close (ptrCSIStreamProfile->csiHandle, errCode);

exit:
    return retVal;
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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static TestFmk_ProfileHandle TestFmkCSIStream_init
(
    TestFmk_Handle      fmkHandle,
    TestFmk_InitCfg*    ptrInitCfg,
    SemaphoreP_Handle   fmkSemHandle,
    int32_t*            errCode
)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;
    TestFmk_ProfileHandle   retHandle = NULL;

    /* Allocate memory for the streaming profile: */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)MemoryP_ctrlAlloc (sizeof(CSIStreamProfile_MCB), 0);
    if (ptrCSIStreamProfile == NULL)
    {
        *errCode = TESTFMK_ENOMEM;
        goto exit;
    }

    /* Initialize the allocated memory */
    memset ((void*)ptrCSIStreamProfile, 0, sizeof(CSIStreamProfile_MCB));

    /* Populate the CSI Streaming profile: */
    ptrCSIStreamProfile->socHandle     = ptrInitCfg->socHandle;
    ptrCSIStreamProfile->fmkHandle     = fmkHandle;
    ptrCSIStreamProfile->fmkSemHandle  = fmkSemHandle;
    ptrCSIStreamProfile->channelId     = EDMA_TPCC0_REQ_FREE_13;
    ptrCSIStreamProfile->linkChannelId = EDMA_NUM_DMA_CHANNELS + CSI_STREAM_PROFILE_MAX_EDMA_CHANNEL;
    ptrCSIStreamProfile->sessionEDMAChannelTable[0].chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_0;
    ptrCSIStreamProfile->sessionEDMAChannelTable[0].shadowLinkChannelsId = (EDMA_NUM_DMA_CHANNELS + 0U);
    ptrCSIStreamProfile->sessionEDMAChannelTable[1].chainChannelsId      = EDMA_TPCC0_REQ_FREE_0;
    ptrCSIStreamProfile->sessionEDMAChannelTable[1].shadowLinkChannelsId = (EDMA_NUM_DMA_CHANNELS + 1U);
    ptrCSIStreamProfile->sessionEDMAChannelTable[2].chainChannelsId      = EDMA_TPCC0_REQ_FREE_1;
    ptrCSIStreamProfile->sessionEDMAChannelTable[2].shadowLinkChannelsId = (EDMA_NUM_DMA_CHANNELS + 2U);
    ptrCSIStreamProfile->sessionEDMAChannelTable[3].chainChannelsId      = EDMA_TPCC0_REQ_FREE_2;
    ptrCSIStreamProfile->sessionEDMAChannelTable[3].shadowLinkChannelsId = (EDMA_NUM_DMA_CHANNELS + 3U);

    /* Setup the global stream profile: */
    gPtrCSIStreamProfileMCB = ptrCSIStreamProfile;

    /* Setup the profile handle: */
    retHandle = (TestFmk_ProfileHandle)ptrCSIStreamProfile;

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
 *  \ingroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t TestFmkCSIStream_deinit (TestFmk_ProfileHandle handle, int32_t* errCode)
{
    CSIStreamProfile_MCB*   ptrCSIStreamProfile;
    int32_t                 retVal = MINUS_ONE;

    /* Get the streaming profile */
    ptrCSIStreamProfile = (CSIStreamProfile_MCB*)handle;
    DebugP_assert (ptrCSIStreamProfile != NULL);

    /* Cleanup the memory: */
    MemoryP_ctrlFree (ptrCSIStreamProfile, sizeof(CSIStreamProfile_MCB));

    /* Reset the global variable. */
    gPtrCSIStreamProfileMCB = NULL;

    /* Streaming Profile has been deinitialized*/
    retVal = 0;

    return retVal;
}

