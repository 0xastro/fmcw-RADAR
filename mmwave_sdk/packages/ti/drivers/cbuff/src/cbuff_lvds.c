/**
 *   @file  cbuff_lvds.c
 *
 *   @brief
 *      The file implements the CBUFF Driver LVDS Interface
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
 *********************** CBUFF Driver LVDS Definitions ********************
 **************************************************************************/

/**
 * @brief   1-Lane Format0
 */
#define CBUFF_LANES1_FMT0                   0x76543210U

/**
 * @brief   1-Lane Format1
 */
#define CBUFF_LANES1_FMT1                   0x76543210U

/**
 * @brief   2-Lane Format0 for Real data
 */
#define CBUFF_LANES2_REAL_FMT0              0x75316420U

/**
 * @brief   2-Lane Format1 for Real data
 */
#define CBUFF_LANES2_REAL_FMT1              0x64207531U

/**
 * @brief   2-Lane Format0 for Complex data
 */
#define CBUFF_LANES2_COMPLEX_FMT0           0x76325410U

/**
 * @brief   2-Lane Format1 for Complex data
 */
#define CBUFF_LANES2_COMPLEX_FMT1           0x54107632U

/**
 * @brief   3-Lane Format0 for Real data
 */
#define CBUFF_LANES3_REAL_FMT0              0x00524130U

/**
 * @brief   3-Lane Format1 for Real data
 */
#define CBUFF_LANES3_REAL_FMT1              0x00304152U

/**
 * @brief   3-Lane Format0 for Complex data
 */
#define CBUFF_LANES3_COMPLEX_FMT0           0x00543210U

/**
 * @brief   3-Lane Format1 for Complex data
 */
#define CBUFF_LANES3_COMPLEX_FMT1           0x00103254U

/**
 * @brief   4-Lane Format0 for Real data
 */
#define CBUFF_LANES4_REAL_FMT0              0x73625140U

/**
 * @brief   4-Lane Format1 for Real data
 */
#define CBUFF_LANES4_REAL_FMT1              0x40516273U

/**
 * @brief   4-Lane Format0 for Complex data
 */
#define CBUFF_LANES4_COMPLEX_FMT0           0x76543210U

/**
 * @brief   4-Lane Format1 for Complex data
 */
#define CBUFF_LANES4_COMPLEX_FMT1           0x10325476U

/**************************************************************************
 ************************* CBUFF LVDS Interface Functions *****************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to populate and initialize the linked list
 *      configuration data structure
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
void CBUFF_initLVDSLinkListParams
(
    CBUFF_Session*          ptrSession,
    CBUFF_LinkListParams*   ptrLinkListParams
)
{
    /* Sanity Check: Ensure that the back pointer is valid. */
    DebugP_assert (ptrSession->ptrDriverMCB != NULL);

    /* LVDS: Setup the default linked list configuration */
    ptrLinkListParams->vcNum             = 0U;
    ptrLinkListParams->hsyncStart        = 1U;
    ptrLinkListParams->hsyncEnd          = 0U;
    ptrLinkListParams->crcEnable         = 0U;
    ptrLinkListParams->lpHdrEn           = 1U;
    ptrLinkListParams->transferSize      = 0U;
    ptrLinkListParams->totalTransferSize = 0U;

    /* TODO: 96bit alignment check needs to be added */
    ptrLinkListParams->align96       = 0U;
    ptrLinkListParams->dataFmtMap    = ptrSession->ptrDriverMCB->initCfg.u.lvdsCfg.laneFormat;

    /* Setup the LVDS Output Format: Translate the output format to the TRM specified values */
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

    /* Set the Long Packet Header as specified in the specification */
    ptrLinkListParams->lpHeaderValue = 0xBBBBBBBBU;

    /* Setup the threshold */
    ptrLinkListParams->threshold = CSL_FMKR (18U, 16U, (uint32_t)ptrSession->dmaNum)    |
                                   CSL_FMKR (14U,  8U, CBUFF_LL_WRITE_THRESHOLD)        |
                                   CSL_FMKR (6U,   0U, CBUFF_LL_READ_THRESHOLD);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to finalize the linked list parameters
 *      for the LVDS.
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
void CBUFF_finalizeLVDSLinkListParams
(
    CBUFF_Session*          ptrSession,
    CBUFF_LinkListParams*   ptrLinkListParams
)
{
    CBUFF_LinkListParams*   ptrFirstLinkListParams;

    /* Is this the first entry being added? */
    if (ptrLinkListParams->lpHdrEn == 1U)
    {
        /* YES: Track the total transfer size in the first entry */
        ptrLinkListParams->totalTransferSize = ptrLinkListParams->transferSize;
    }
    else
    {
        /* NO: Get the first linked list entry for the session */
        ptrFirstLinkListParams = &ptrSession->linkedListTracker[0];

        /* Track the total transfer size in the session: */
        ptrFirstLinkListParams->totalTransferSize = ptrFirstLinkListParams->totalTransferSize + ptrLinkListParams->transferSize;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the LVDS Interface for the specific session
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
int32_t CBUFF_openLVDS (CBUFF_Session* ptrSession, int32_t* errCode)
{
    int32_t     retVal = 0;
    uint32_t    format0[4];
    uint32_t    format1[4];
    uint32_t    format0Value;
    uint32_t    format1Value;
    uint32_t    fmtIndex;
    uint32_t    index;

    /* Sanity Check: Ensure that the back pointer to the driver is valid. */
    DebugP_assert (ptrSession->ptrDriverMCB != NULL);

    /* Initialize the format values */
    memset ((void *)&format0, 0, sizeof(format0));
    memset ((void *)&format1, 0, sizeof(format1));

    /* Configuration depends upon the number of LVDS Lanes: */
    switch (ptrSession->ptrDriverMCB->numActiveLVDSLanes)
    {
        case 1:
        {
            /**************************************************************************
             * 1 Lane: Bit 3 sets up the valid bit, Bit [2:0] indicates the sample
             * out of [0-7]
             **************************************************************************/
            format0[0] = ((CBUFF_LANES1_FMT0 & 0xffffffffU) | 0x88888888U);
            format1[0] = ((CBUFF_LANES1_FMT1 & 0xffffffffU) | 0x88888888U);
            break;
        }
        case 2:
        {
            /* Two Lanes: */
            if (ptrSession->sessionCfg.dataType == CBUFF_DataType_REAL)
            {
                format0Value = CBUFF_LANES2_REAL_FMT0;
                format1Value = CBUFF_LANES2_REAL_FMT1;
            }
            else
            {
                format0Value = CBUFF_LANES2_COMPLEX_FMT0;
                format1Value = CBUFF_LANES2_COMPLEX_FMT1;
            }

            /**************************************************************************
             * 2 Lanes: Bit 3 sets up the valid bit, Bit [2:0] indicates the sample
             * out of [0-7]
             **************************************************************************/
            format0[0] = ((format0Value & 0x0000ffffU)  | 0x00008888U);
            format1[0] = ((format1Value & 0x0000ffffU)  | 0x00008888U);
            format0[1] = (((format0Value & 0xffff0000U) | 0x88880000U) >> 16U);
            format1[1] = (((format1Value & 0xffff0000U) | 0x88880000U) >> 16U);
            break;
        }
        case 3:
        {
            /* Three Lanes: */
            if (ptrSession->sessionCfg.dataType == CBUFF_DataType_REAL)
            {
                format0Value = CBUFF_LANES3_REAL_FMT0;
                format1Value = CBUFF_LANES3_REAL_FMT1;
            }
            else
            {
                format0Value = CBUFF_LANES3_COMPLEX_FMT0;
                format1Value = CBUFF_LANES3_COMPLEX_FMT1;
            }

            /**************************************************************************
             * 3 Lanes: Bit 3 sets up the valid bit, Bit [2:0] indicates the sample
             * out of [0-7]
             **************************************************************************/
            format0[0] = ((format0Value & 0x000000ffU)  | 0x00000088U);
            format1[0] = ((format1Value & 0x000000ffU)  | 0x00000088U);
            format0[1] = (((format0Value & 0x0000ff00U) | 0x00008800U) >> 8U);
            format1[1] = (((format1Value & 0x0000ff00U) | 0x00008800U) >> 8U);
            format0[2] = (((format0Value & 0x00ff0000U) | 0x00880000U) >> 16U);
            format1[2] = (((format1Value & 0x00ff0000U) | 0x00880000U) >> 16U);
            break;
        }
        case 4:
        {
            /* Four Lanes: */
            if (ptrSession->sessionCfg.dataType == CBUFF_DataType_REAL)
            {
                format0Value = CBUFF_LANES4_REAL_FMT0;
                format1Value = CBUFF_LANES4_REAL_FMT1;
            }
            else
            {
                format0Value = CBUFF_LANES4_COMPLEX_FMT0;
                format1Value = CBUFF_LANES4_COMPLEX_FMT1;
            }

            /**************************************************************************
             * 4 Lanes: Bit 3 sets up the valid bit, Bit [2:0] indicates the sample
             * out of [0-7]
             **************************************************************************/
            format0[0] = ((format0Value & 0x000000ffU)  | 0x00000088U);
            format1[0] = ((format1Value & 0x000000ffU)  | 0x00000088U);
            format0[1] = (((format0Value & 0x0000ff00U) | 0x00008800U) >> 8U);
            format1[1] = (((format1Value & 0x0000ff00U) | 0x00008800U) >> 8U);
            format0[2] = (((format0Value & 0x00ff0000U) | 0x00880000U) >> 16U);
            format1[2] = (((format1Value & 0x00ff0000U) | 0x00880000U) >> 16U);
            format0[3] = (((format0Value & 0xff000000U) | 0x88000000U) >> 24U);
            format1[3] = (((format1Value & 0xff000000U) | 0x88000000U) >> 24U);
            break;
        }
        default:
        {
            /* Error: This condition should never occur. We have already verified the
             * number of lanes before the function is invoked. */
            DebugP_assert (0);
            break;
        }
    }

    /* Debug: Display the LVDS Lane Format */
    for (index = 0U; index < 4U; index++)
    {
        DebugP_log3 ("Debug: LVDS Lane %d Format0 0x%x Format1 0x%x\n", (index+1), format0[index], format1[index]);
    }

    /* We now need to set the LVDS Lane Format registers: */
    fmtIndex = 0U;
    for (index = 0U; index < 4U; index++)
    {
        /* Is the LVDS Lane enabled? */
        if (CSL_FEXTR(ptrSession->ptrDriverMCB->initCfg.u.lvdsCfg.lvdsLaneEnable, index, index) == 1U)
        {
            /* YES: LVDS Lane is enabled; configure the Format0 and Format1 registers */
            ptrSession->ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_MAPPING_LANE_FMT[fmtIndex]      = format0[index];
            ptrSession->ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_MAPPING_LANE_FMT[fmtIndex + 4U] = format1[index];
            fmtIndex = fmtIndex + 1U;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the LVDS Interface for the specific session
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
int32_t CBUFF_closeLVDS (CBUFF_Session* ptrSession, int32_t* errCode)
{
    uint32_t    fmtIndex;
    uint32_t    index;

    /* We now need to reset the LVDS Lane Format registers: */
    fmtIndex = 0U;
    for (index = 0U; index < 4U; index++)
    {
        /* Is the LVDS Lane enabled? */
        if (CSL_FEXTR(ptrSession->ptrDriverMCB->initCfg.u.lvdsCfg.lvdsLaneEnable, index, index) == 1U)
        {
            /* YES: LVDS Lane is enabled; reset the FORMAT registers */
            ptrSession->ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_MAPPING_LANE_FMT[fmtIndex]      = 0U;
            ptrSession->ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_MAPPING_LANE_FMT[fmtIndex + 4U] = 0U;
            fmtIndex = fmtIndex + 1U;
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the LVDS interface
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
int32_t CBUFF_initLVDS (CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode)
{
    int32_t     retVal = -1;
    uint32_t    index;

    /* Enable the LVDS Interface: */
    if (SOC_enableLVDS(ptrDriverMCB->initCfg.socHandle, errCode) < 0)
    {
        /* Error: Unable to enable the LVDS Interface */
        goto exit;
    }

    /* Set the number of LVDS lanes: */
    ptrDriverMCB->numActiveLVDSLanes = 0U;

    /* Determine the number of active LVDS Lanes: */
    for (index = 0; index < 4U; index++)
    {
        /* Is the enable bit set? */
        if (CSL_FEXTR(ptrDriverMCB->initCfg.u.lvdsCfg.lvdsLaneEnable, index, index) == 1U)
        {
            /* YES: Increment the number of active LVDS Lanes: */
            ptrDriverMCB->numActiveLVDSLanes++;
        }
    }

    /* Sanity Check: Were there any active LVDS Lanes configured? *OR*
     * Do we exceed the maximum allowed? */
    if ((ptrDriverMCB->numActiveLVDSLanes == 0U) ||
        (ptrDriverMCB->numActiveLVDSLanes > gCBUFFHwAttribute.maxLVDSLanesSupported))
    {
        /* Error: There are no LVDS lanes configured or it exceeds the maximum */
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Configure the CFG_LVDS_GEN_0 register:
     * - Setup the LVDS Clock configuration for alignment enabled
     * - CRC Enabled/Disabled as per the configuration
     * - LVDS Async FIFO Threshold is set to 8 as per the programming model
     * - LSB/MSB first as per the configuration
     * - DDR/SDR Mode Clock Mux
     * - Regular operation
     * - SDR/DDR clock mode as per the configuration */
    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                           31U, 30U, 1U);
    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                           28U, 28U, ptrDriverMCB->initCfg.u.lvdsCfg.crcEnable);
    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                           27U, 24U, 8U);
    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                           23U, 23U, ptrDriverMCB->initCfg.u.lvdsCfg.msbFirst);
    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                           22U, 22U, 1U);

    if (ptrDriverMCB->initCfg.u.lvdsCfg.ddrClockModeMux == 1U)
    {
        /* DDR Mode Clock Mux: */
        ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                               15U, 15U, 0U);
    }
    else
    {
        /* SDR Mode Clock Mux: */
        ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                               15U, 15U, 1U);
    }

    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                           11U, 11U, 0U);
    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                           10U, 10U, ptrDriverMCB->initCfg.u.lvdsCfg.ddrClockMode);

    /* We now need to enable all the LVDS configured lanes. So cycle through all the enabled
     * lanes and enable them */
    for (index = 0; index < 4U; index++)
    {
        /* Is the enable bit set? */
        if (CSL_FEXTR(ptrDriverMCB->initCfg.u.lvdsCfg.lvdsLaneEnable, index, index) == 1U)
        {
            /* YES: Ensure that the corresponding bit is enabled */
            ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_0,
                                                                   index, index, 1U);
        }
    }

    /* Configure the CFG_LVDS_GEN_1 register:
     * - [TODO] Disable the 3Channel 3Lane LVDS as per the configuration
     *   The 3Channel 3Lane mode is not currently supported. */
    ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_1 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CFG_LVDS_GEN_1, 2U, 2U, 0U);

    /* There is only 1 Virtual channel enabled on the LVDS */
    CBUFF_setVC (ptrDriverMCB->ptrCBUFFReg, 0U, 1U, 1U);

    /* Is the CRC Enabled on the LVDS? */
    if (ptrDriverMCB->initCfg.u.lvdsCfg.crcEnable == 0U)
    {
        /* NO: CRC is not enabled on the LVDS; setup the HSVAL and HEVAL
         * as per the specification */
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_HSVAL = 0xAAAAAAAAU;
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_HEVAL = 0xAAAAAAAAU;
    }
    else
    {
        /* YES: CRC is enabled on the LVDS; setup the HSVAL and HEVAL
         * as per the specification */
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_HSVAL = 0x55555555U;
        ptrDriverMCB->ptrCBUFFReg->CFG_CMD_HEVAL = 0x33333333U;
    }

    /* Setup the registers as per the specification: */
    ptrDriverMCB->ptrCBUFFReg->CFG_SPHDR_ADDRESS = 0x55555555U;
    ptrDriverMCB->ptrCBUFFReg->CFG_LPHDR_ADDRESS = 0x55555555U;
    ptrDriverMCB->ptrCBUFFReg->CFG_CMD_VSVAL     = 0xAAAAAAAAU;
    ptrDriverMCB->ptrCBUFFReg->CFG_CMD_VEVAL     = 0xAAAAAAAAU;

    /* Select the LVDS high speed interface */
    ptrDriverMCB->ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrDriverMCB->ptrCBUFFReg->CONFIG_REG_0, 0U, 0U, 1U);

    /* The FIFO Free Threshold is set to 0 since this is not used for LVDS Interface */
    ptrDriverMCB->ptrCBUFFReg->CFG_FIFO_FREE_THRESHOLD = 0x0U;

    /* Successfully initialized the LVDS interface: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize the LVDS interface
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
int32_t CBUFF_deinitLVDS (CBUFF_DriverMCB* ptrDriverMCB, int32_t* errCode)
{
    return 0;
}
