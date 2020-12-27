/*
 *   @file  hsiheader.c
 *
 *   @brief
 *      This is the HSI Header Module implementation
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* mmWave SDK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/common/mmwave_sdk_version.h>
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/utils/hsiheader/hsiheader.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/cbuff/include/cbuff_internal.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/**
 * @brief
 *  This is the maximum number of identifiers which are handled by the
 *  HSI Header module
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_DEFINITION
 */
#define HSIHEADER_MAX_ID         8

/**************************************************************************
 ************************* Local Structures *******************************
 **************************************************************************/

/**
 * @brief
 *  HSI Identifier Tracker
 *
 * @details
 *  The structure is used to track the allocation of track identifiers. It
 *  is possible to have multiple sessions and each of these sessions need
 *  to have a unique track identifier allocated to it.
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_DATA_STRUCTURE
 */
typedef struct HSIHeaderId_Track_t
{
    /**
     * @brief   Status of the tracker identifier.
     */
    bool            isAllocated;

    /**
     * @brief   Tracker Identifier
     */
    uint64_t        id;
}HSIHeaderId_Track;

/**
 * @brief
 *  HSI Header MCB
 *
 * @details
 *  This is the master control block of the HSI Header module.
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_DATA_STRUCTURE
 */
typedef struct HSIHeader_MCB_t
{
    /**
     * @brief   Flag which indicates that the HSI Module is operational
     */
    bool                hsiOperational;

    /**
     * @brief   This is the high speed interface which is being used.
     */
    CBUFF_Interface     hsiInterface;

    /**
     * @brief   This is the number of LVDS lanes which have been configured
     * in the CBUFF driver
     */
    uint8_t             numLVDSLanes;

    /**
     * @brief   Size of the data which is being streamed out.
     */
    uint8_t             dataSize;

    /**
     * @brief   Tracker Identifier Management
     */
    HSIHeaderId_Track   idTracker[HSIHEADER_MAX_ID];
}HSIHeader_MCB;

/**************************************************************************
 ************************* Global Declarations ****************************
 **************************************************************************/

/**
 * @brief
 *  This is the global variable which is associated with the Data card Module
 */
HSIHeader_MCB       gHSIHeaderMCB;

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern CBUFF_Hw_Attrib  gCBUFFHwAttribute;

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/
static int32_t  HSIHeader_idAllocate (uint64_t* ptrId);
static int32_t  HSIHeader_idFree (uint64_t id);
static uint16_t HSIHeader_computeTransferSize(CBUFF_SessionCfg* ptrSessionCfg, HSIHeader* ptrHeader);
static uint16_t HSIHeader_computePaddingSize(CBUFF_SessionCfg* ptrSessionCfg, bool bAlignDataCard, uint16_t totalTransferSize);
static uint16_t HSIHeader_processUserBuffer(CBUFF_BufferCfg* ptrUserBufferInfo, HSIHeader* ptrHeader);
static uint16_t HSIHeader_processCQ (CBUFF_SessionCfg* ptrSessionCfg, HSIHeader* ptrHeader);

/**************************************************************************
 ************************ HSI Header Functions ****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to allocate an identifier
 *
 *  @param[out] ptrId
 *      Populated with the allocated identifier
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static int32_t HSIHeader_idAllocate (uint64_t* ptrId)
{
    uint8_t     index;
    uintptr_t   key;
    int32_t     retVal = MINUS_ONE;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Cycle through all the supported identifiers: */
    for (index = 0; index < HSIHEADER_MAX_ID; index++)
    {
        /* Is the identifier allocated? */
        if (gHSIHeaderMCB.idTracker[index].isAllocated == false)
        {
            /* NO: Allocate it */
            *ptrId = gHSIHeaderMCB.idTracker[index].id;
            gHSIHeaderMCB.idTracker[index].isAllocated = true;
            retVal = 0;
            break;
        }
    }

    /* Restore the interrupts: */
    HwiP_restore (key);
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to free an identifier
 *
 *  @param[in] id
 *      Identifier to be cleaned up
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static int32_t HSIHeader_idFree (uint64_t id)
{
    uint8_t     index;
    uintptr_t   key;
    int32_t     retVal = MINUS_ONE;

    /* Disable the interrupts: */
    key = HwiP_disable ();

    /* Cycle through all the supported identifiers: */
    for (index = 0; index < HSIHEADER_MAX_ID; index++)
    {
        /* Are we trying to clean up this identifier? */
        if (gHSIHeaderMCB.idTracker[index].id == id)
        {
            /* YES: Clean it up. */
            gHSIHeaderMCB.idTracker[index].isAllocated = false;
            retVal = 0;
            break;
        }
    }

    /* Restore the interrupts: */
    HwiP_restore (key);
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to compute the padding
 *
 *  @param[in] ptrSessionCfg
 *      Pointer to the session configuration
 *  @param[in]  bAlignDataCard
 *      Flag which indicates if the header created should be aligned
 *      in accordance with the data card requirements.
 *  @param[in] totalTransferSize
 *      Total Transfer Size of all data in CBUFF Units
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Padding Size in CBUFF Units
 *  @retval
 *      Error   - 0
 */
static uint16_t HSIHeader_computePaddingSize
(
    CBUFF_SessionCfg*   ptrSessionCfg,
    bool                bAlignDataCard,
    uint16_t            totalTransferSize
)
{
    uint16_t    paddingSize;
    uint16_t    alignmentSize;

    /* Which interface are we executing on? */
    if (gHSIHeaderMCB.hsiInterface == CBUFF_Interface_LVDS)
    {
        /*********************************************************
         * LVDS: Do we need to align for the Data Card?
         *********************************************************/
        if (bAlignDataCard == false)
        {
            /* Alignment as documented by the TRM is needed on the 8 word boundary */
            alignmentSize = 8U;
        }
        else
        {
            /* Alignment is needed on the 256 byte boundary */
            alignmentSize = HSIHeader_toCBUFFUnits(HSI_DATA_CARD_ALIGNMENT);
        }

        /* Determine if the total transfer size is a multiple of the alignment */
        paddingSize = totalTransferSize % alignmentSize;
        if (paddingSize != 0U)
        {
            /* Alignment required: */
            paddingSize = alignmentSize - paddingSize;
        }
    }
    else
    {
        /*********************************************************
         * CSI: No alignment requirements
         *********************************************************/
        paddingSize = 0U;
    }
    return paddingSize;
}

/**
 *  @b Description
 *  @n
 *      The function is used to compute the total transfer size
 *      for the user buffers and populated the appropriate fields in
 *      the HSI Header
 *
 *  @param[in]  ptrUserBufferInfo
 *      Pointer to the User Buffer Information
 *  @param[out]  ptrHeader
 *      Pointer to the HSI header populated by the API
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Size of the transfer in CBUFF Units
 *  @retval
 *      Error   - 0
 */
static uint16_t HSIHeader_processUserBuffer
(
    CBUFF_BufferCfg*    ptrUserBufferInfo,
    HSIHeader*          ptrHeader
)
{
    uint16_t    userTotalTransferSize = 0U;
    uint8_t     index;

    /* Cycle through all the user buffers: */
    for (index = 0; index < HSI_HEADER_MAX_USER_BUFFER; index++)
    {
        /* Was a user buffer specified? */
        if (ptrUserBufferInfo[index].size != 0U)
        {
            /* YES: */
            userTotalTransferSize = userTotalTransferSize + ptrUserBufferInfo[index].size;
            ptrHeader->sdkHeader.userBufSize[index] = HSIHeader_htons(ptrUserBufferInfo[index].size);
        }
    }
    return userTotalTransferSize;
}

/**
 *  @b Description
 *  @n
 *      The function is used to compute the total transfer size
 *      for the Chirp Quality and to populate the appropriate fields in
 *      the HSI Header
 *
 *  @param[in]  ptrSessionCfg
 *      Pointer to the session configuration
 *  @param[in]  ptrHeader
 *      Pointer to the HSI Header
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Size of the CQ Transfer in CBUFF Units
 *  @retval
 *      Error   - 0
 */
static uint16_t HSIHeader_processCQ
(
    CBUFF_SessionCfg*   ptrSessionCfg,
    HSIHeader*          ptrHeader
)
{
    uint8_t     index;
    uint16_t    cqTransferSize = 0U;

    /* Cycle through all the chirp quality */
    for (index = 0; index < HSI_HEADER_MAX_CQ; index++)
    {
        /* Track the total CQ Transfer Size: */
        cqTransferSize = cqTransferSize + ptrSessionCfg->u.hwCfg.cqSize[index];

        /* Populate the HSI Header with the CQx Size: */
        ptrHeader->sdkHeader.cqDataSize[index] = HSIHeader_htons(ptrSessionCfg->u.hwCfg.cqSize[index]);
    }
    return cqTransferSize;
}

/**
 *  @b Description
 *  @n
 *      The function is used to compute the total transfer size
 *
 *  @param[in]  ptrSessionCfg
 *      Pointer to the CBUFF session configuration
 *  @param[out] ptrHeader
 *      Pointer to the HSI Header
 *
 *  \ingroup HSI_HEADER_UTIL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Size of the transfer in CBUFF Units
 *  @retval
 *      Error   - 0
 */
static uint16_t HSIHeader_computeTransferSize
(
    CBUFF_SessionCfg*   ptrSessionCfg,
    HSIHeader*          ptrHeader
)
{
    uint8_t    numActiveADCChannels  = 0U;
    uint16_t   cqTotalTransferSize   = 0U;
    uint16_t   adcTransferSize       = 0U;
    uint16_t   totalTransferSize     = 0U;
    uint16_t   userTotalTransferSize;
    uint16_t   index;
    uint32_t   channelAddress;
    int32_t    errCode;

    /****************************************************************
     * Setup the TI Meta Information Header
     * - Data Size is specified during CBUFF Initialization and is the
     *   same across all sessions.
     * - Data Type i.e. Real or Complex
     ****************************************************************/
    ptrHeader->sdkHeader.dataSize = gHSIHeaderMCB.dataSize;
    if (ptrSessionCfg->dataType == CBUFF_DataType_REAL)
    {
        /* Real Data: */
        ptrHeader->sdkHeader.dataType = HSI_HEADER_TYPE_REAL;
    }
    else
    {
        /* Complex Data: */
        ptrHeader->sdkHeader.dataType = HSI_HEADER_TYPE_COMPLEX;
    }

    /* Determine the mode in which the session is operating? */
    if (ptrSessionCfg->executionMode == CBUFF_SessionExecuteMode_HW)
    {
        /* Is the session operating in Interleaved or Non-Interleaved mode? */
        if (ptrSessionCfg->u.hwCfg.dataMode == CBUFF_DataMode_INTERLEAVED)
        {
            /* Interleaved Mode: */
            ptrHeader->sdkHeader.interleavedMode = HSI_HEADER_MODE_INTERLEAVED;
        }
        else
        {
            /* Non-Interleaved Mode: */
            ptrHeader->sdkHeader.interleavedMode = HSI_HEADER_MODE_NON_INTERLEAVED;
        }

        /************************************************************************
         * HW Session:
         * - Cycle through all the channels to determine the active channels
         ************************************************************************/
        for (index = 0U; index < SYS_COMMON_NUM_RX_CHANNEL; index++)
        {
            /* Get the ADCBUF Channel Address from the ADC Driver */
            channelAddress = ADCBuf_getChanBufAddr (ptrSessionCfg->u.hwCfg.adcBufHandle, index, &errCode);
            if (channelAddress != 0U)
            {
                /* YES: Increment the number of active ADC channels */
                numActiveADCChannels++;

                /* Setup the receive channel status: */
                ptrHeader->sdkHeader.rxChannelStatus = ptrHeader->sdkHeader.rxChannelStatus | (1 << index);
            }
        }

        /****************************************************************
         * ADC Transfer Size:
         * - For Complex ADC samples account for I/Q
         ****************************************************************/
        if (ptrSessionCfg->dataType == CBUFF_DataType_COMPLEX)
        {
            /* Complex: */
            adcTransferSize = ptrSessionCfg->u.hwCfg.numADCSamples * 2U;
        }
        else
        {
            /* Real: */
            adcTransferSize = ptrSessionCfg->u.hwCfg.numADCSamples;
        }

        /* Setup the chirp mode: */
        ptrHeader->sdkHeader.chirpMode = HSIHeader_htons(ptrSessionCfg->u.hwCfg.chirpMode);

        /****************************************************************
         * Determine the total transfer size:
         ****************************************************************/
        switch (ptrSessionCfg->u.hwCfg.dataFormat)
        {
            case CBUFF_DataFmt_ADC_DATA:
            {
                /* Populate the TI Meta-Information for the data format: */
                ptrHeader->sdkHeader.dataFmt     = HSI_HEADER_FMT_ADC;
                ptrHeader->sdkHeader.adcDataSize = HSIHeader_htons(adcTransferSize);

                /* Are we operating in Chirp or Continuous Mode? */
                if (ptrHeader->sdkHeader.chirpMode != 0U)
                {
                    /* Chirp Mode: Single or Multiple */
                    totalTransferSize = adcTransferSize * numActiveADCChannels * ptrHeader->sdkHeader.chirpMode;
                }
                else
                {
                    /* Continuous Mode: Only ADC Data is going to be streamed out */
                    totalTransferSize = adcTransferSize * numActiveADCChannels;
                }
                break;
            }
            case CBUFF_DataFmt_CP_ADC:
            {
                /* Populate the TI Meta-Information for the data format: */
                ptrHeader->sdkHeader.dataFmt     = HSI_HEADER_FMT_CP_ADC;
                ptrHeader->sdkHeader.adcDataSize = HSIHeader_htons(adcTransferSize);
                ptrHeader->sdkHeader.cpDataSize  = HSIHeader_htons(SYS_COMMON_CP_SIZE_CBUFF_UNITS);

                /* Single or Multiple Chirp Mode? */
                if (ptrHeader->sdkHeader.chirpMode == 1U)
                {
                    /* Single Chirp Mode:
                     * Total Data Transfer Size: CP + ADC Data is going to be streamed out */
                    totalTransferSize = (SYS_COMMON_CP_SIZE_CBUFF_UNITS + adcTransferSize) * numActiveADCChannels;
                }
                else
                {
                    /* Multiple Chirp Mode:
                     *  Total Data Transfer Size:
                     *  CP (For Multiple Chirps & All Channels) + ADC Data is going to be streamed out */
                    totalTransferSize = (SYS_COMMON_CP_SIZE_CBUFF_UNITS * SYS_COMMON_NUM_RX_CHANNEL) * ptrHeader->sdkHeader.chirpMode;
                    totalTransferSize = totalTransferSize + (adcTransferSize * numActiveADCChannels * ptrHeader->sdkHeader.chirpMode);
                }
                break;
            }
            case CBUFF_DataFmt_ADC_CP:
            {
                /* Populate the header: */
                ptrHeader->sdkHeader.dataFmt     = HSI_HEADER_FMT_ADC_CP;
                ptrHeader->sdkHeader.adcDataSize = HSIHeader_htons(adcTransferSize);
                ptrHeader->sdkHeader.cpDataSize  = HSIHeader_htons(SYS_COMMON_CP_SIZE_CBUFF_UNITS);

                /* Single or Multiple Chirp Mode? */
                if (ptrHeader->sdkHeader.chirpMode == 1U)
                {
                    /* Single Chirp Mode:
                     * Total Data Transfer Size: CP + ADC Data is going to be streamed out */
                    totalTransferSize = (SYS_COMMON_CP_SIZE_CBUFF_UNITS + adcTransferSize) * numActiveADCChannels;
                }
                else
                {
                    /* Multiple Chirp Mode:
                     *  Total Data Transfer Size:
                     *  CP (For Multiple Chirps & All Channels) + ADC Data is going to be streamed out */
                    totalTransferSize = (SYS_COMMON_CP_SIZE_CBUFF_UNITS * SYS_COMMON_NUM_RX_CHANNEL) * ptrHeader->sdkHeader.chirpMode;
                    totalTransferSize = totalTransferSize + (adcTransferSize * numActiveADCChannels * ptrHeader->sdkHeader.chirpMode);
                }
                break;
            }
            case CBUFF_DataFmt_CP_ADC_CQ:
            {
                /* Populate the header: */
                ptrHeader->sdkHeader.dataFmt     = HSI_HEADER_FMT_CP_ADC_CQ;
                ptrHeader->sdkHeader.adcDataSize = HSIHeader_htons(adcTransferSize);
                ptrHeader->sdkHeader.cpDataSize  = HSIHeader_htons(SYS_COMMON_CP_SIZE_CBUFF_UNITS);

                /****************************************************************
                 * Chirp Quality:
                 ****************************************************************/
                cqTotalTransferSize = HSIHeader_processCQ (ptrSessionCfg, ptrHeader);

                /* Single or Multiple Chirp Mode? */
                if (ptrHeader->sdkHeader.chirpMode == 1U)
                {
                    /* Single Chirp Mode:
                     * Total Data Transfer Size: CP + ADC + CQ Data is going to be streamed out */
                    totalTransferSize = ((SYS_COMMON_CP_SIZE_CBUFF_UNITS + adcTransferSize) * numActiveADCChannels);
                    totalTransferSize = totalTransferSize + cqTotalTransferSize;
                }
                else
                {
                    /* Multiple Chirp Mode:
                     *  Total Data Transfer Size:
                     *  CP (For Multiple Chirps & All Channels) + ADC Data is going to be streamed out + CQ */
                    totalTransferSize = (SYS_COMMON_CP_SIZE_CBUFF_UNITS * SYS_COMMON_NUM_RX_CHANNEL) * ptrHeader->sdkHeader.chirpMode;
                    totalTransferSize = totalTransferSize + (adcTransferSize * numActiveADCChannels * ptrHeader->sdkHeader.chirpMode);
                    totalTransferSize = totalTransferSize + cqTotalTransferSize;
                }
                break;
            }
            case CBUFF_DataFmt_ADC_USER:
            {
                /* Populate the header: */
                ptrHeader->sdkHeader.dataFmt     = HSI_HEADER_FMT_ADC_USER;
                ptrHeader->sdkHeader.adcDataSize = HSIHeader_htons(adcTransferSize);

                /****************************************************************
                 * User Buffers:
                 ****************************************************************/
                userTotalTransferSize = HSIHeader_processUserBuffer (&ptrSessionCfg->u.hwCfg.userBufferInfo[0], ptrHeader);

                /* Total Data Transfer Size: ADC + User Data is going to be streamed out */
                totalTransferSize = (adcTransferSize * numActiveADCChannels * ptrHeader->sdkHeader.chirpMode) + userTotalTransferSize;
                break;
            }
            case CBUFF_DataFmt_CP_ADC_CQ_USER:
            {
                /* Populate the header: */
                ptrHeader->sdkHeader.dataFmt     = HSI_HEADER_FMT_CP_ADC_CQ_USER;
                ptrHeader->sdkHeader.adcDataSize = HSIHeader_htons(adcTransferSize);
                ptrHeader->sdkHeader.cpDataSize  = HSIHeader_htons(SYS_COMMON_CP_SIZE_CBUFF_UNITS);

                /****************************************************************
                 * User Buffers:
                 ****************************************************************/
                userTotalTransferSize = HSIHeader_processUserBuffer (&ptrSessionCfg->u.hwCfg.userBufferInfo[0], ptrHeader);

                /****************************************************************
                 * Chirp Quality:
                 ****************************************************************/
                cqTotalTransferSize = HSIHeader_processCQ (ptrSessionCfg, ptrHeader);

                /* Single or Multiple Chirp Mode? */
                if (ptrHeader->sdkHeader.chirpMode == 1U)
                {
                    /* Single Chirp Mode:
                     * Total Data Transfer Size: CP + ADC + CQ + User Data is going to be streamed out */
                    totalTransferSize = ((SYS_COMMON_CP_SIZE_CBUFF_UNITS + adcTransferSize) * numActiveADCChannels) +
                                         cqTotalTransferSize + userTotalTransferSize;
                }
                else
                {
                    /* Multiple Chirp Mode:
                     *  Total Data Transfer Size:
                     *  CP (For Multiple Chirps & All Channels) + ADC Data is going to be streamed out + CQ + User Data */
                    totalTransferSize = (SYS_COMMON_CP_SIZE_CBUFF_UNITS * SYS_COMMON_NUM_RX_CHANNEL) * ptrHeader->sdkHeader.chirpMode;
                    totalTransferSize = totalTransferSize + (adcTransferSize * numActiveADCChannels * ptrHeader->sdkHeader.chirpMode);
                    totalTransferSize = totalTransferSize + cqTotalTransferSize;
                    totalTransferSize = totalTransferSize + userTotalTransferSize;
                }
                break;
            }
            default:
            {
                /* Error: This is not a valid supported case and control should never come here. */
                totalTransferSize = 0;
                break;
            }
        }
    }
    else
    {
        /****************************************************************
         * Software Triggered Mode: Only user data is being transferred
         * in this mode. Setup the header
         * - Non Interleaved Data Mode
         * - No Receive channel status since there is no ADC Data
         * - Chirp Profile & Chirp Quality Sizes are set to 0
         ****************************************************************/
        ptrHeader->sdkHeader.interleavedMode = HSI_HEADER_MODE_NON_INTERLEAVED;
        ptrHeader->sdkHeader.dataFmt         = HSI_HEADER_FMT_USER;

        /* Get the user transfer size: This is the only data being sent in this mode */
        totalTransferSize = HSIHeader_processUserBuffer (&ptrSessionCfg->u.swCfg.userBufferInfo[0], ptrHeader);
    }

    /* Return the total transfer size */
    return totalTransferSize;
}

/**
 *  @b Description
 *  @n
 *      This is the function which is used to populate the HSI header
 *      using the CBUFF Session. This header should then be used to
 *      create the CBUFF session.
 *
 *  @param[in]  ptrSessionCfg
 *      Pointer to the CBUFF session configuration
 *  @param[in]  bAlignDataCard
 *      Flag which indicates if the header created should be aligned
 *      in accordance with the data card requirements. Headers created
 *      in this mode could have a large amount of padding and could
 *      waste bandwidth.
 *  @param[out] ptrHeader
 *      Pointer to the HSI Header which will be populated by the API.
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup HSI_HEADER_UTIL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t HSIHeader_createHeader
(
    CBUFF_SessionCfg*   ptrSessionCfg,
    bool                bAlignDataCard,
    HSIHeader*          ptrHeader,
    int32_t*            errCode
)
{
    int32_t         retVal = MINUS_ONE;
    uint16_t        transferSize;
    uint16_t        paddingSize;
    uint32_t        totalLength;
    uint16_t        headerSize;

    /* Initialize the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    if ((ptrSessionCfg == NULL) || (ptrHeader == NULL))
    {
        /* Error: Invalid arguments */
        *errCode = HSIHEADER_EINVAL;
        goto exit;
    }

    /* Sanity Check: Has the HSI module been initialized? */
    if (gHSIHeaderMCB.hsiOperational == false)
    {
        /* Error: Module has not been initialized */
        *errCode = HSIHEADER_EINVAL;
        goto exit;
    }

    /* Initialize the memory of the header: */
    memset ((void*)ptrHeader, 0, sizeof(HSIHeader));

    /* Reset the contents of the padding buffer: */
    memset ((void*)&ptrHeader->paddingBuffer[0], 0x0F, sizeof(ptrHeader->paddingBuffer));

    /* Allocate a unique identifier */
    if (HSIHeader_idAllocate (&ptrHeader->dataCardHeader.id) < 0)
    {
        /* Error: Unable to allocate an identifier; cannot proceed with
         * header creation; since all the identifiers seem to be allocated */
        *errCode = HSIHEADER_ENOSPACE;
        goto exit;
    }

    /* Populate the version & convert to network order: */
    ptrHeader->sdkHeader.version = MMWAVE_SDK_VERSION_BUILD           |
                                   (MMWAVE_SDK_VERSION_BUGFIX << 3U)  |
                                   (MMWAVE_SDK_VERSION_MINOR  << 6U)  |
                                   (MMWAVE_SDK_VERSION_MAJOR  << 9U);
    ptrHeader->sdkHeader.version = HSIHeader_htons (ptrHeader->sdkHeader.version);

    /* Populate the platform: */
#ifdef SOC_XWR16XX
    ptrHeader->sdkHeader.platform = HSI_PLATFORM_XWR16XX;
#elif defined SOC_XWR14XX
    ptrHeader->sdkHeader.platform = HSI_PLATFORM_XWR14XX;
#elif defined SOC_XWR18XX
    ptrHeader->sdkHeader.platform = HSI_PLATFORM_XWR18XX;
#elif defined SOC_XWR68XX
    ptrHeader->sdkHeader.platform = HSI_PLATFORM_XWR68XX;
#else
    #error "Error: The HSI Header module does not support the SOC"
#endif

    /* Compute the total transfer size: */
    transferSize = HSIHeader_computeTransferSize (ptrSessionCfg, ptrHeader);
    if (transferSize == 0)
    {
        /* Error: Invalid use case */
        *errCode = HSIHEADER_EINVAL;
        goto exit;
    }

    /* Initialize the *mandatory* header size: */
    headerSize = sizeof(HSIDataCardHeader) + sizeof(HSISDKHeader);
    DebugP_assert ((headerSize % 2) == 0);

    /* Convert the size into CBUFF Units: */
    headerSize = (uint8_t)HSIHeader_toCBUFFUnits(headerSize);

    /* Compute the padding size in the header: */
    paddingSize = HSIHeader_computePaddingSize (ptrSessionCfg, bAlignDataCard, (transferSize + headerSize));

    /* Header Size: Account for the padding and convert to network order */
    headerSize = headerSize + paddingSize;
    ptrHeader->sdkHeader.headerSize = HSIHeader_htons(headerSize);

    /* We now need to populate the total length of the packet. This is specified in bytes
     * and will include the size of the Header + Data. We need to discount the size of
     * the Data Card Header */
    totalLength = HSIHeader_toBytes (headerSize) + HSIHeader_toBytes(transferSize);
    totalLength = totalLength - sizeof(HSIDataCardHeader);

    /* Break out the lengths into 12bit (LSW & MSW) and convert to network order */
    ptrHeader->dataCardHeader.totalLengthLSW = HSIHeader_htons(totalLength & 0xFFF);
    ptrHeader->dataCardHeader.totalLengthMSW = HSIHeader_htons((totalLength & 0xFFF000) >> 12U);

    /* The header has been created */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the function which is used to delete the header and free up
 *      the identifer which had been allocated before.
 *
 *  @param[out] ptrHeader
 *      Pointer to the HSI Header which has to be deleted
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup HSI_HEADER_UTIL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t HSIHeader_deleteHeader
(
    HSIHeader*    ptrHeader,
    int32_t*      errCode
)
{
    int32_t retVal = MINUS_ONE;

    /* Initialize the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    if (ptrHeader == NULL)
    {
        /* Error: Invalid arguments */
        *errCode = HSIHEADER_EINVAL;
        goto exit;
    }

    /* Sanity Check: Has the HSI module been initialized? */
    if (gHSIHeaderMCB.hsiOperational == false)
    {
        /* Error: Module has not been initialized */
        *errCode = HSIHEADER_EINVAL;
        goto exit;
    }

    /* Free the identifier: */
    if (HSIHeader_idFree (ptrHeader->dataCardHeader.id) < 0)
    {
        /* Error: Invalid header was passed to the API. */
        *errCode = HSIHEADER_EINVAL;
        goto exit;
    }

    /* The header has been deleted and identifier successfully freed up */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the function which is used to initialize the HSI Header module
 *
 *  @param[in]  ptrInitCfg
 *      Pointer to the CBUFF Initialization configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup HSI_HEADER_UTIL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t HSIHeader_init (CBUFF_InitCfg* ptrInitCfg, int32_t* errCode)
{
    uint8_t     index;
    int32_t     retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    if (ptrInitCfg == NULL)
    {
        /* Error: Invalid configuration */
        *errCode = HSIHEADER_EINVAL;
        goto exit;
    }

    /* Initialize the MCB: */
    memset ((void*)&gHSIHeaderMCB, 0, sizeof(HSIHeader_MCB));

    /* Keep track of the HSI being used: */
    gHSIHeaderMCB.hsiInterface = ptrInitCfg->interface;

    /* In the exported protocol header file we cannot use the SDK definitions
     * since this will cause a dependency between the external applications
     * and the SDK. Instead we enforce the dependency validations and ensure
     * that the module definitons are consistent here. */
    DebugP_assert (HSI_HEADER_MAX_USER_BUFFER == CBUFF_MAX_USER_BUFFER);
    DebugP_assert (HSI_HEADER_MAX_CQ == ADCBufMMWave_CQType_MAX_CQ);

    /* Which interface are we using? */
    if (gHSIHeaderMCB.hsiInterface == CBUFF_Interface_LVDS)
    {
        /* LVDS: Determine the number of lanes */
        for (index = 0U; index < 4U; index++)
        {
            /* Is the enable bit set? */
            if (CSL_FEXTR(ptrInitCfg->u.lvdsCfg.lvdsLaneEnable, index, index) == 1U)
            {
                /* YES: Increment the number of active LVDS Lanes: */
                gHSIHeaderMCB.numLVDSLanes++;
            }
        }
    }
    else
    {
        /* CSI: Fall through. */
    }

    /* Determine the data size which is being transferred: */
    switch (ptrInitCfg->outputDataFmt)
    {
        case CBUFF_OutputDataFmt_16bit:
        {
            gHSIHeaderMCB.dataSize = HSI_HEADER_SIZE_16BIT;
            break;
        }
        case CBUFF_OutputDataFmt_14bit:
        {
            gHSIHeaderMCB.dataSize = HSI_HEADER_SIZE_14BIT;
            break;
        }
        case CBUFF_OutputDataFmt_12bit:
        {
            gHSIHeaderMCB.dataSize = HSI_HEADER_SIZE_12BIT;
            break;
        }
        default:
        {
            /* Error: The control should never come here */
            *errCode = HSIHEADER_EINVAL;
            goto exit;
        }
    }

    /* Initialize the ID Tracker: */
    gHSIHeaderMCB.idTracker[0].id            = HSIHeader_htonll(HSI_HEADER_ID1);
    gHSIHeaderMCB.idTracker[0].isAllocated   = false;
    gHSIHeaderMCB.idTracker[1].id            = HSIHeader_htonll(HSI_HEADER_ID2);
    gHSIHeaderMCB.idTracker[1].isAllocated   = false;
    gHSIHeaderMCB.idTracker[2].id            = HSIHeader_htonll(HSI_HEADER_ID3);
    gHSIHeaderMCB.idTracker[2].isAllocated   = false;
    gHSIHeaderMCB.idTracker[3].id            = HSIHeader_htonll(HSI_HEADER_ID4);
    gHSIHeaderMCB.idTracker[3].isAllocated   = false;
    gHSIHeaderMCB.idTracker[4].id            = HSIHeader_htonll(HSI_HEADER_ID5);
    gHSIHeaderMCB.idTracker[4].isAllocated   = false;
    gHSIHeaderMCB.idTracker[5].id            = HSIHeader_htonll(HSI_HEADER_ID6);
    gHSIHeaderMCB.idTracker[5].isAllocated   = false;
    gHSIHeaderMCB.idTracker[6].id            = HSIHeader_htonll(HSI_HEADER_ID7);
    gHSIHeaderMCB.idTracker[6].isAllocated   = false;
    gHSIHeaderMCB.idTracker[7].id            = HSIHeader_htonll(HSI_HEADER_ID8);
    gHSIHeaderMCB.idTracker[7].isAllocated   = false;

    /* Module is operational: */
    gHSIHeaderMCB.hsiOperational = true;

    /* Control comes here implies that the initialization was successful */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the function which is used to deinitialize the HSI Header module
 *
 *  \ingroup HSI_HEADER_UTIL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t HSIHeader_deinit (int32_t* errCode)
{
    /* Module is no longer operational: */
    gHSIHeaderMCB.hsiOperational = true;
    return 0;
}

