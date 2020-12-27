/**
 *   @file  cbuff.c
 *
 *   @brief
 *      The file implements the CBUFF Driver.
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

/**************************************************************************
 ************************ CBUFF Driver Local Functions ********************
 **************************************************************************/

/**************************************************************************
 * CBUFF Register Read/Write API:
 **************************************************************************/
static void CBUFF_reset          (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_releaseReset   (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_enableErrorInt (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_disableErrorInt(DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_disableFrameDoneInt (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_enableFrameDoneInt (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_disableChirpDoneInt (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_enableChirpDoneInt (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_setECC         (DSSCBUFFRegs* ptrCBUFFReg, uint8_t ecc);
static void CBUFF_getStatus      (DSSCBUFFRegs* ptrCBUFFReg, uint8_t* isFrameDone, uint8_t* isChirpDone);
static void CBUFF_clearStatus    (DSSCBUFFRegs* ptrCBUFFReg, uint8_t isFrameDone, uint8_t isChirpDone);
static void CBUFF_setFrameStartSWTrigger (DSSCBUFFRegs* ptrCBUFFReg, uint32_t enableSW);
static void CBUFF_setChirpAvailableSWTrigger (DSSCBUFFRegs* ptrCBUFFReg, uint32_t enableSW);
static void CBUFF_triggerFrameStart (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_triggerChirpAvailable (DSSCBUFFRegs* ptrCBUFFReg);
static void CBUFF_clearLLEntry (DSSCBUFFRegs* ptrCBUFFReg, uint8_t listIndex);
static void CBUFF_setLLEntry (DSSCBUFFRegs* ptrCBUFFReg, uint8_t listIndex, CBUFF_LinkListParams* ptrLLParams);
static void CBUFF_setNumberChirps (DSSCBUFFRegs* ptrCBUFFReg, uint32_t numChirps);

/* CBUFF Driver Internal API: */
static CBUFF_InterfaceFxn* CBUFF_isIfSupported (CBUFF_Interface interface);
static int32_t CBUFF_validateBufferCfg (CBUFF_BufferCfg* ptrUserBuffer, int32_t* errCode);

/* CBUFF Session Management API: */
static int32_t CBUFF_validateHWSessionCfg (CBUFF_Session* ptrSession, int32_t* errCode);
static int32_t CBUFF_validateSWSessionCfg (CBUFF_Session* ptrSession, int32_t* errCode);
static CBUFF_Session* CBUFF_allocateSession (CBUFF_DriverMCB* ptrDriverMCB);
static void CBUFF_freeSession (CBUFF_Session* ptrSession);

/* CBUFF Frame Done Management API: */
static void CBUFF_registerFrameDone (CBUFF_DriverMCB* ptrDriverMCB);
static void CBUFF_deregisterFrameDone (CBUFF_DriverMCB* ptrDriverMCB);

/**************************************************************************
 * CBUFF ISR:
 **************************************************************************/
static void CBUFF_ISR (uintptr_t arg);
static void CBUFF_ErrorISR (uintptr_t arg);
static void CBUFF_FrameStartISR (uintptr_t arg);

/**************************************************************************
 *************************** CBUFF Driver Functions ***********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to reset the CBUFF module.
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_reset (DSSCBUFFRegs* ptrCBUFFReg)
{
    ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 27U, 27U, 1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to release the reset of the CBUFF module.
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_releaseReset (DSSCBUFFRegs* ptrCBUFFReg)
{
    ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 27U, 27U, 0U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the ECC
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  ecc
 *      Flag to enable/disable ECC
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_setECC (DSSCBUFFRegs* ptrCBUFFReg, uint8_t ecc)
{
    ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 1U, 1U, (uint32_t)ecc);
}

/**
 *  @b Description
 *  @n
 *      The function is used to disable the frame done interrupts
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_disableFrameDoneInt (DSSCBUFFRegs* ptrCBUFFReg)
{
    /* Disable Frame Done:
     *  - Interrupts are disabled by writing a 1 to the corresponding bits */
    ptrCBUFFReg->CFG_MASK_REG0 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG0,
                                            STAT_CBUFF_REG0_S_FRAME_DONE_BIT_START,
                                            STAT_CBUFF_REG0_S_FRAME_DONE_BIT_START,
                                            1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the chirp done interrupt
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_enableFrameDoneInt (DSSCBUFFRegs* ptrCBUFFReg)
{
    /* Enable Frame Done:
     *  - Interrupts are enabled by writing a 0 to the corresponding bits */
    ptrCBUFFReg->CFG_MASK_REG0 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG0,
                                            STAT_CBUFF_REG0_S_FRAME_DONE_BIT_START,
                                            STAT_CBUFF_REG0_S_FRAME_DONE_BIT_START,
                                            0U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to disable the chirp done interrupts
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_disableChirpDoneInt (DSSCBUFFRegs* ptrCBUFFReg)
{
    /* Disable Chirp Done:
     *  - Interrupts are disabled by writing a 1 to the corresponding bits */
    ptrCBUFFReg->CFG_MASK_REG0 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG0,
                                            STAT_CBUFF_REG0_S_CHIRP_DONE_BIT_START,
                                            STAT_CBUFF_REG0_S_CHIRP_DONE_BIT_START,
                                            1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the chirp done interrupt
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_enableChirpDoneInt (DSSCBUFFRegs* ptrCBUFFReg)
{
    /* Enable Chirp Done:
     *  - Interrupts are enabled by writing a 0 to the corresponding bits */
    ptrCBUFFReg->CFG_MASK_REG0 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG0,
                                            STAT_CBUFF_REG0_S_CHIRP_DONE_BIT_START,
                                            STAT_CBUFF_REG0_S_CHIRP_DONE_BIT_START,
                                            0U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the error interrupts associated with the
 *      CBUFF Module
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_enableErrorInt (DSSCBUFFRegs* ptrCBUFFReg)
{
    /* Enable Frame Start Error & Chirp Error:
     *  - Interrupts are enabled by writing a 0 to the corresponding bits */
    ptrCBUFFReg->CFG_MASK_REG1 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG1,
                                            STAT_CBUFF_REG1_S_FRAME_ERR_BIT_START,
                                            STAT_CBUFF_REG1_S_FRAME_ERR_BIT_START,
                                            0U);
    ptrCBUFFReg->CFG_MASK_REG1 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG1,
                                            STAT_CBUFF_REG1_S_CHIRP_ERR_BIT_START,
                                            STAT_CBUFF_REG1_S_CHIRP_ERR_BIT_START,
                                            0U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to disable the error interrupts associated with the
 *      CBUFF Module
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_disableErrorInt(DSSCBUFFRegs* ptrCBUFFReg)
{
    /* Enable Frame Start Error & Chirp Error:
     *  - Interrupts are disabled by writing a 1 to the corresponding bits */
    ptrCBUFFReg->CFG_MASK_REG1 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG1,
                                            STAT_CBUFF_REG1_S_FRAME_ERR_BIT_START,
                                            STAT_CBUFF_REG1_S_FRAME_ERR_BIT_START,
                                            1U);
    ptrCBUFFReg->CFG_MASK_REG1 = CSL_FINSR (ptrCBUFFReg->CFG_MASK_REG1,
                                            STAT_CBUFF_REG1_S_CHIRP_ERR_BIT_START,
                                            STAT_CBUFF_REG1_S_CHIRP_ERR_BIT_START,
                                            1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the CBUFF status.
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[out]  isFrameDone
 *      Set to 1 to indicate that the frame has been done
 *  @param[out]  isChirpDone
 *      Set to 1 to indicate that the chirp has been done
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_getStatus (DSSCBUFFRegs* ptrCBUFFReg, uint8_t* isFrameDone, uint8_t* isChirpDone)
{
    *isFrameDone = (uint8_t)CSL_FEXTR (ptrCBUFFReg->STAT_CBUFF_REG0, 12U, 12U);
    *isChirpDone = (uint8_t)CSL_FEXTR (ptrCBUFFReg->STAT_CBUFF_REG0, 11U, 11U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to clear the CBUFF status.
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  isFrameDone
 *      Set to 1 to clear the Frame Done Status
 *  @param[out]  isChirpDone
 *      Set to 1 to clear the Chirp Done Status
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_clearStatus (DSSCBUFFRegs* ptrCBUFFReg, uint8_t isFrameDone, uint8_t isChirpDone)
{
    ptrCBUFFReg->CLR_CBUFF_REG0 = CSL_FINSR (ptrCBUFFReg->CLR_CBUFF_REG0, 12U, 12U, (uint32_t)isFrameDone);
    ptrCBUFFReg->CLR_CBUFF_REG0 = CSL_FINSR (ptrCBUFFReg->CLR_CBUFF_REG0, 11U, 11U, (uint32_t)isChirpDone);
}

/**
 *  @b Description
 *  @n
 *      The function is used to set the software triggerring capability of the frame start
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  enableSW
 *      Set to 1 to enable the triggering be done by the SW else set to 0
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_setFrameStartSWTrigger (DSSCBUFFRegs* ptrCBUFFReg, uint32_t enableSW)
{
    ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 2U, 2U, enableSW);
}

/**
 *  @b Description
 *  @n
 *      The function is used to set the software triggerring capability of the chirp available
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  enableSW
 *      Set to 1 to enable the triggering be done by the SW else set to 0
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_setChirpAvailableSWTrigger (DSSCBUFFRegs* ptrCBUFFReg, uint32_t enableSW)
{
    ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 3U, 3U, enableSW);
}

/**
 *  @b Description
 *  @n
 *      The function is used to trigger the frame start. This function should only be
 *      invoked if the software has assumed the responsibility for triggering the frame
 *      start
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_triggerFrameStart (DSSCBUFFRegs* ptrCBUFFReg)
{
    ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 25U, 25U, 1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to trigger the chirp available. This function should only be
 *      invoked if the software has assumed the responsibility for triggering the chirp
 *      available
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_triggerChirpAvailable (DSSCBUFFRegs* ptrCBUFFReg)
{
    ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 24U, 24U, 1U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to set the number of chirps
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  numChirps
 *      Number of chirps to be configured
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_setNumberChirps (DSSCBUFFRegs* ptrCBUFFReg, uint32_t numChirps)
{
    ptrCBUFFReg->CFG_CHIRPS_PER_FRAME = numChirps;
}

/**
 *  @b Description
 *  @n
 *      The function is used to clear the linked list entry in the CBUFF IP
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  listIndex
 *      Linked List index to be cleared.
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_clearLLEntry (DSSCBUFFRegs* ptrCBUFFReg, uint8_t listIndex)
{
    /* Reset the LL to the default values. */
    ptrCBUFFReg->LL_ENTRY[listIndex].CFG_DATA_LL           = 0U;
    ptrCBUFFReg->LL_ENTRY[listIndex].CFG_DATA_LL_LPHDR_VAL = 0U;
    ptrCBUFFReg->LL_ENTRY[listIndex].CFG_DATA_LL_THRESHOLD = CSL_FMKR (14U, 8U, 0x3FU);
}

/**
 *  @b Description
 *  @n
 *      The function is used to set the linked list entry in the CBUFF IP
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  listIndex
 *      Linked List index to be configured.
 *  @param[in]  ptrLLParams
 *      Pointer to the Linked list params used to set the LL entry
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_setLLEntry (DSSCBUFFRegs* ptrCBUFFReg, uint8_t listIndex, CBUFF_LinkListParams* ptrLLParams)
{
    /* Sanity Check: This should never occur and the transfer size should always be within the IP Limits */
    DebugP_assert (ptrLLParams->transferSize <= CBUFF_MAX_TRANSFER_SIZE_CBUFF_UNITS);

    /* Setup the linked list data:
     *  - CRC Enable as per the specified configuration
     *  - LP Header as per the specified configuration
     *  - Wait for Packet sent is enabled
     *  - Size as per the specified configuration
     *  - Data Formating of the incoming data sources [128bit(0) or 96bit (1)]
     *  - Data Format map applicable only for LVDS.
     *  - LVDS/CSI2 Output format i.e. 16bit, 14bit or 12bit
     *  - Number of the virtual channel
     *  - Horizontal Sync Start as per the specified configuration
     *  - Horizontal Sync End as per the specified configuration
     *  - Linked List entry is valid */
    ptrCBUFFReg->LL_ENTRY[listIndex].CFG_DATA_LL =
             CSL_FMKR (28U, 28U, ptrLLParams->crcEnable)        |
             CSL_FMKR (27U, 27U, ptrLLParams->lpHdrEn)          |
             CSL_FMKR (22U, 9U,  ptrLLParams->transferSize)     |
             CSL_FMKR (8U,  8U,  ptrLLParams->align96)          |
             CSL_FMKR (7U,  7U,  ptrLLParams->dataFmtMap)       |
             CSL_FMKR (6U,  5U,  ptrLLParams->outputDataFmt)    |
             CSL_FMKR (4U,  3U,  ptrLLParams->vcNum)            |
             CSL_FMKR (2U,  2U,  ptrLLParams->hsyncStart)       |
             CSL_FMKR (1U,  1U,  ptrLLParams->hsyncEnd)         |
             CSL_FMKR (0U,  0U,  1U);

    /* Set the Long Packet Header & Thresholds: */
    ptrCBUFFReg->LL_ENTRY[listIndex].CFG_DATA_LL_LPHDR_VAL = ptrLLParams->lpHeaderValue;
    ptrCBUFFReg->LL_ENTRY[listIndex].CFG_DATA_LL_THRESHOLD = ptrLLParams->threshold;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the VSYNC parameters for a specific
 *      virtual channel.
 *
 *  @param[in]  ptrCBUFFReg
 *      Pointer to the CBUFF Register space
 *  @param[in]  vc
 *      Virtual channel [Valid from 0 to 3]
 *  @param[in]  vcStart
 *      Flag to enable/disable the VSYNC Start
 *  @param[in]  vcEnd
 *      Flag to enable/disable the VSYNC End
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void CBUFF_setVC (DSSCBUFFRegs* ptrCBUFFReg, uint8_t vc, uint8_t vcStart, uint8_t vcEnd)
{
    switch (vc)
    {
        case 0U:
        {
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 10U, 10U, (uint32_t)vcStart);
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 11U, 11U, (uint32_t)vcEnd);
            break;
        }
        case 1U:
        {
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 12U, 12U, (uint32_t)vcStart);
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 13U, 13U, (uint32_t)vcEnd);
            break;
        }
        case 2U:
        {
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 14U, 14U, (uint32_t)vcStart);
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 15U, 15U, (uint32_t)vcEnd);
            break;
        }
        case 3U:
        {
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 16U, 16U, (uint32_t)vcStart);
            ptrCBUFFReg->CONFIG_REG_0 = CSL_FINSR (ptrCBUFFReg->CONFIG_REG_0, 17U, 17U, (uint32_t)vcEnd);
            break;
        }
        default:
        {
            DebugP_assert (0);
            break;
        }
    }
}

/**
 *  @b Description
 *  @n
 *      This is the function which is used to validate the buffers
 *      configuration. User buffers passed by the applications for
 *      either the header *OR* data buffers need to be placed into
 *      the EDMA address space.
 *
 *  @param[in]  ptrUserBuffer
 *      Pointer to the user buffer which needs to be validated
 *  @param[out] errCode
 *      Error code populated on the error
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CBUFF_validateBufferCfg (CBUFF_BufferCfg* ptrUserBuffer, int32_t* errCode)
{
    int32_t     retVal = MINUS_ONE;
    uint32_t    edmaAddress;

    /* Sanity Check: Ensure that the size does not exceed the MAXIMUM CBUFF IP Size */
    if (ptrUserBuffer->size > CBUFF_MAX_TRANSFER_SIZE_CBUFF_UNITS)
    {
        /* Error: Invalid size exceeds the max which the IP block can support */
        *errCode = CBUFF_ELIMIT;
        goto exit;
    }

    /* Is the address specified? */
    if (ptrUserBuffer->size != 0U)
    {
        /* YES: Translate to the EDMA space */
        edmaAddress = SOC_translateAddress(ptrUserBuffer->address, SOC_TranslateAddr_Dir_TO_EDMA, errCode);
        if (edmaAddress == SOC_TRANSLATEADDR_INVALID)
        {
            /* Error: Unable to translate the user address; error code is setup */
            goto exit;
        }

        /* Update the configuration with the EDMA Address: This is what we will be using to
         * setup the EDMA Transfers */
        ptrUserBuffer->address = edmaAddress;
    }

    /* User buffers have been validated: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the CBUFF to handle the CBUFF
 *      completion status
 *
 *  @param[in]  arg
 *      Pointer to the CBUFF Driver MCB
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_ISR (uintptr_t arg)
{
    CBUFF_DriverMCB*        ptrDriverMCB;
    uint8_t                 isFrameDone;
    uint8_t                 isChirpDone;
    CBUFF_Session*          ptrSession;

    /* Get the pointer to the driver MCB: */
    ptrDriverMCB = (CBUFF_DriverMCB*)arg;

    /* Sanity Check: */
    DebugP_assert (ptrDriverMCB != NULL);

    /* Get the CBUFF Status: */
    CBUFF_getStatus (ptrDriverMCB->ptrCBUFFReg, &isFrameDone, &isChirpDone);

    /* Clear the CBUFF Status: */
    CBUFF_clearStatus (ptrDriverMCB->ptrCBUFFReg, isFrameDone, isChirpDone);

    /* Get the currently active session: */
    ptrSession = ptrDriverMCB->ptrCurrentActiveSession;
    if (ptrSession == NULL)
    {
        /* Record the number of the interrupts. */
        ptrDriverMCB->totalNumNonActiveSessionInterrupts++;
        goto exit;
    }

    /* Was the frame processed? */
    if (isFrameDone == 1U)
    {
        /* YES: The frame was processed. Increment the statistics */
        ptrDriverMCB->totalNumFrameDone++;

        /* Increment the statistics for the session */
        ptrSession->numFrameDone++;

        /* Do we need to invoke the callback function? */
        if (ptrSession->sessionCfg.frameDoneCallbackFxn != NULL)
        {
            /* YES: Notify the callback function that the frame done was received for the
             * active session. */
            ptrSession->sessionCfg.frameDoneCallbackFxn ((CBUFF_SessionHandle)ptrSession);
        }
    }

    /* Are we operating in debug mode? */
    if (ptrDriverMCB->initCfg.enableDebugMode == true)
    {
        /* Was the chirp processed? */
        if (isChirpDone == 1U)
        {
            /* YES: The chirp was handled. Increment the statistics */
            ptrDriverMCB->totalNumChirpDone++;

            /* Increment the statistics for the session */
            ptrSession->numChirpDone++;
        }
    }

exit:
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the CBUFF to handle the
 *      CBUFF errors
 *
 *  @param[in]  arg
 *      Pointer to the CBUFF Driver MCB
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_ErrorISR (uintptr_t arg)
{
    CBUFF_DriverMCB*        ptrDriverMCB;

    /* Get the pointer to the driver MCB: */
    ptrDriverMCB = (CBUFF_DriverMCB*)arg;

    /* Sanity Check: */
    DebugP_assert (ptrDriverMCB != NULL);

    /* Increment the error counter */
    ptrDriverMCB->totalNumErrorInterrupts++;
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the CBUFF to handle
 *      the start of the frame
 *
 *  @param[in]  arg
 *      Pointer to the CBUFF Driver MCB
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_FrameStartISR (uintptr_t arg)
{
    CBUFF_DriverMCB*        ptrDriverMCB;

    /* Get the pointer to the driver MCB: */
    ptrDriverMCB = (CBUFF_DriverMCB*)arg;

    /* Sanity Check: */
    DebugP_assert (ptrDriverMCB != NULL);

    /* Increment the frame start counter */
    ptrDriverMCB->totalNumFrameStart++;
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the high speed interface with the platform
 *      supplied interface table.
 *
 *  @param[in]  interface
 *      Interface to be verified
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Supported interface function block
 *  @retval
 *      Error       - NULL
 */
static CBUFF_InterfaceFxn* CBUFF_isIfSupported (CBUFF_Interface interface)
{
    uint8_t             ifIndex = 0U;
    CBUFF_InterfaceFxn* ptrInterfaceFxn;
    int32_t             done = 0;

    /* Initialize the interface function to the platform supplied definition */
    while (done == 0)
    {
        /* Get the interface block: */
        ptrInterfaceFxn = &gCBUFFInterfaceFxn[ifIndex];

        /* Is this a valid interface or the end of the table? */
        if (ptrInterfaceFxn->initFxn != NULL)
        {
            /* YES: Do we have a match on the interface? */
            if (interface == ptrInterfaceFxn->interface)
            {
                /* YES: We have found a match. */
                done = 1;
            }
            else
            {
                /* NO: Skip to the next entry in the platform table */
                ifIndex++;
            }
        }
        else
        {
            /* NO: We have reached the end of the platform table and have not found a match. */
            ptrInterfaceFxn = NULL;
            done = 1;
        }
    }
    return ptrInterfaceFxn;
}

/**
 *  @b Description
 *  @n
 *      The function is used to free an allocated session
 *
 *  @param[in]  ptrSession
 *      Pointer to the session to be freed up
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_freeSession (CBUFF_Session* ptrSession)
{
    CBUFF_DriverMCB*    ptrDriverMCB;
    uint8_t             dmaNum;

    /* Keep a local copy of the session information */
    ptrDriverMCB = ptrSession->ptrDriverMCB;
    dmaNum       = ptrSession->dmaNum;

    /* Reset the contents of the session memory: */
    memset ((void*)ptrSession, 0, sizeof(CBUFF_Session));

    /* Update session back: */
    ptrSession->ptrDriverMCB = ptrDriverMCB;
    ptrSession->dmaNum       = dmaNum;
}

/**
 *  @b Description
 *  @n
 *      The function is used to allocate a session block
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the driver MCB
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Allocated session handle
 *  @retval
 *      Error   - NULL
 */
static CBUFF_Session* CBUFF_allocateSession (CBUFF_DriverMCB* ptrDriverMCB)
{
    uint8_t         sessionIndex;
    CBUFF_Session*  ptrSession;
    uintptr_t       key;

    /* Disable the interrupts: The session table is a critical resource which needs
     * to be protected against concurrent access from multiple threads. */
    key = HwiP_disable();

    /* Cycle through all the sessions: */
    for (sessionIndex = 0U; sessionIndex < ptrDriverMCB->initCfg.maxSessions; sessionIndex++)
    {
        /* Get the session: */
        ptrSession = &ptrDriverMCB->ptrSessionTable[sessionIndex];

        /* Is the session valid and in use? */
        if (ptrSession->isValid == false)
        {
            /* NO: Session is free and can be used. */
            ptrSession->isValid = true;
            break;
        }
    }

    /* Were we able to allocate a session? */
    if (sessionIndex == ptrDriverMCB->initCfg.maxSessions)
    {
        /* NO: Reached the end of the table and unable to find a free slot */
        ptrSession = NULL;
    }

    /* Restore the interrupts: Session allocation has been completed. */
    HwiP_restore (key);

    /* Return the session: */
    return ptrSession;
}

/**
 *  @b Description
 *  @n
 *      The function is used to register the frame done interrupts.
 *      If applications are executing only one session we dont want
 *      to overwhelm the processor with frame done interrupts. Frame
 *      done interrupts should only be used if there are multiple
 *      sessions which are getting activated and the application has
 *      registered for callback functions.
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the driver MCB
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_registerFrameDone (CBUFF_DriverMCB* ptrDriverMCB)
{
    uint8_t         sessionIndex;
    CBUFF_Session*  ptrSession;

    /* Is the debug mode enabled? */
    if (ptrDriverMCB->initCfg.enableDebugMode == true)
    {
        /* YES: Frame done interrupts are already registered. There is no need
         * to perform any other actions. */
        goto exit;
    }

    /* Cycle through all the sessions: */
    for (sessionIndex = 0U; sessionIndex < ptrDriverMCB->initCfg.maxSessions; sessionIndex++)
    {
        /* Get the session: */
        ptrSession = &ptrDriverMCB->ptrSessionTable[sessionIndex];

        /* Is the session valid and does it need the frame done callback? */
        if ((ptrSession->isValid == true) && (ptrSession->sessionCfg.frameDoneCallbackFxn != NULL))
        {
            /* YES: Frame Done interrupt notifications are required. */
            CBUFF_enableFrameDoneInt (ptrDriverMCB->ptrCBUFFReg);
            break;
        }
    }

exit:
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deregister the frame done interrupts.
 *      If there are no active sessions which require this functionality
 *      then we can deregister this interrupt and not take a performance
 *      hit for these interrupts.
 *
 *  @param[in]  ptrDriverMCB
 *      Pointer to the driver MCB
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CBUFF_deregisterFrameDone (CBUFF_DriverMCB* ptrDriverMCB)
{
    uint8_t         sessionIndex;
    CBUFF_Session*  ptrSession;
    uint8_t         isFrameDone;
    uint8_t         isChirpDone;

    /* Is the debug mode enabled? */
    if (ptrDriverMCB->initCfg.enableDebugMode == true)
    {
        /* YES: Executing in debug mode. There is no need to perform any deregisteration
         * We need to keep the interrupts always registered. */
        goto exit;
    }

    /* Cycle through all the sessions: */
    for (sessionIndex = 0U; sessionIndex < ptrDriverMCB->initCfg.maxSessions; sessionIndex++)
    {
        /* Get the session: */
        ptrSession = &ptrDriverMCB->ptrSessionTable[sessionIndex];

        /* Is the session valid and does it need the frame done callback? */
        if ((ptrSession->isValid == true) && (ptrSession->sessionCfg.frameDoneCallbackFxn != NULL))
        {
            break;
        }
    }
    if (sessionIndex == ptrDriverMCB->initCfg.maxSessions)
    {
        /* Control comes here indicates that there were no either:-
         * (a) No more active sessions  *OR*
         * (b) None of the sessions needed the frame done functionality
         *
         * So we can deregister the ISR and avoid the performance penalty. */
        CBUFF_disableFrameDoneInt (ptrDriverMCB->ptrCBUFFReg);

        /* We have disabled the interrupts but if there are any pending interrupts
         * then these need to be acknowledged here; else we will not get any more
         * interrupts. */
        CBUFF_getStatus (ptrDriverMCB->ptrCBUFFReg, &isFrameDone, &isChirpDone);
        CBUFF_clearStatus (ptrDriverMCB->ptrCBUFFReg, isFrameDone, isChirpDone);
    }

exit:
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deactivate the session.
 *
 *  @param[in]  sessionHandle
 *      Handle to the session to be activated
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CBUFF_deactivateSession (CBUFF_SessionHandle sessionHandle, int32_t* errCode)
{
    int32_t             retVal = MINUS_ONE;
    CBUFF_DriverMCB*    ptrDriverMCB;
    CBUFF_Session*      ptrSession;
    uint8_t             index;

    /* Get the pointer to the session: */
    ptrSession = (CBUFF_Session*)sessionHandle;
    if (ptrSession == NULL)
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Sanity Check: Ensure that there exists a back pointer to the CBUFF Driver */
    ptrDriverMCB = ptrSession->ptrDriverMCB;
    DebugP_assert (ptrDriverMCB != NULL);

    /* Sanity Check: Ensure that the session is currently active. If this is not active
     * we cannot deactivate it because we will cause random behavior. */
    if (ptrDriverMCB->ptrCurrentActiveSession != ptrSession)
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Clear out all the linked list entries in the CBUFF IP */
    for (index = 0U; index < CBUFF_MAX_LINKED_LIST_SIZE; index++)
    {
        CBUFF_clearLLEntry (ptrDriverMCB->ptrCBUFFReg, index);
    }

    /* Default State: The triggers are always owned by the software. Frame Starts from
     * the BSS can cause the CBUFF IP to falsely kickstart the state machine especially
     * if there is no active session */
    CBUFF_setFrameStartSWTrigger (ptrDriverMCB->ptrCBUFFReg, 1U);
    CBUFF_setChirpAvailableSWTrigger (ptrDriverMCB->ptrCBUFFReg, 1U);

    /* Close the high speed interface (HSI) for the session: */
    DebugP_assert (ptrDriverMCB->interfaceFxn->closeFxn (ptrSession, errCode) == 0);

    /* The session is no longer active: */
    ptrDriverMCB->ptrCurrentActiveSession = NULL;

    /* Session has been deactivated successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to activate the specific session.
 *
 *  @param[in]  sessionHandle
 *      Handle to the session to be activated
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CBUFF_activateSession (CBUFF_SessionHandle sessionHandle, int32_t* errCode)
{
    int32_t             retVal = MINUS_ONE;
    CBUFF_DriverMCB*    ptrDriverMCB;
    CBUFF_Session*      ptrSession;
    uint8_t             index;

    /* Get the pointer to the session: */
    ptrSession = (CBUFF_Session*)sessionHandle;
    if (ptrSession == NULL)
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Sanity Check: Ensure that there exists a back pointer to the CBUFF Driver */
    ptrDriverMCB = ptrSession->ptrDriverMCB;
    DebugP_assert (ptrDriverMCB != NULL);

    /* Is there a current session which is already active? */
    if (ptrDriverMCB->ptrCurrentActiveSession != NULL)
    {
        /* YES: Let the application deactivate it explicitly. */
        *errCode = CBUFF_EINUSE;
        goto exit;
    }

    /* The Linked List entries are already configured in the session. */
    for (index = 0U; index < ptrSession->linkListIndex; index++)
    {
        /* Configure the CBUFF IP Linked List Entry. */
        CBUFF_setLLEntry (ptrDriverMCB->ptrCBUFFReg, index, &ptrSession->linkedListTracker[index]);
    }

    /* Process the session configuration on the basis of the execution mode: */
    if (ptrSession->sessionCfg.executionMode == CBUFF_SessionExecuteMode_HW)
    {
        /* Hardware Triggered Execution Mode: Are we operating in chirp *or* continuous mode? */
        if (ptrSession->sessionCfg.u.hwCfg.opMode == CBUFF_OperationalMode_CONTINUOUS)
        {
            /* The Frame start needs to be triggered by the driver. */
            CBUFF_setFrameStartSWTrigger (ptrDriverMCB->ptrCBUFFReg, 1U);
            CBUFF_setChirpAvailableSWTrigger (ptrDriverMCB->ptrCBUFFReg, 0U);
        }
        else
        {
            /* HW Trigger Chirp Mode: The triggers are owned by the HW */
            CBUFF_setFrameStartSWTrigger (ptrDriverMCB->ptrCBUFFReg, 0U);
            CBUFF_setChirpAvailableSWTrigger (ptrDriverMCB->ptrCBUFFReg, 0U);
        }
    }
    else
    {
        /* Software Triggered Execution Mode: The chirp available & frame start needs to be triggered
         * by the driver. */
        CBUFF_setFrameStartSWTrigger (ptrDriverMCB->ptrCBUFFReg, 1U);
        CBUFF_setChirpAvailableSWTrigger (ptrDriverMCB->ptrCBUFFReg, 1U);
    }

    /* Setup the number of chirps per frame: */
    CBUFF_setNumberChirps (ptrDriverMCB->ptrCBUFFReg, ptrSession->numChirpsPerFrame);

    /* The session is now active and can be used. */
    ptrDriverMCB->ptrCurrentActiveSession = ptrSession;

    /* Open the high speed interface (HSI) for the session: */
    DebugP_assert (ptrDriverMCB->interfaceFxn->openFxn (ptrSession, errCode) == 0);

    /* We need to determine if we need to do the manual trigger sending of the packet.
     * This has to be done in the following cases:-
     *  (1) Hardware Triggerred Mode: Handle the continuous operational mode
     *  (2) Software Triggerred Mode */
    if (ptrSession->sessionCfg.executionMode == CBUFF_SessionExecuteMode_HW)
    {
        /* Are we operationg in continuous mode? */
        if (ptrSession->sessionCfg.u.hwCfg.opMode == CBUFF_OperationalMode_CONTINUOUS)
        {
            /* YES: We need to trigger the frame start manually for the first time only. */
            CBUFF_triggerFrameStart (ptrDriverMCB->ptrCBUFFReg);
        }
    }
    else
    {
        /* In Software Triggered mode: We need to trigger the frame start & chirp available */
        CBUFF_triggerFrameStart (ptrDriverMCB->ptrCBUFFReg);
        CBUFF_triggerChirpAvailable (ptrDriverMCB->ptrCBUFFReg);
    }

    /* Session has been activated: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the HW Session configuration.
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t CBUFF_validateHWSessionCfg (CBUFF_Session* ptrSession, int32_t* errCode)
{
    uint32_t            index;
    ADCBufMMWave_CQType cqType;
    int32_t             retVal = MINUS_ONE;

    /************************************************************************
     * Sanity Check: Continuous mode is *NOT* supported for all data formats
     ************************************************************************/
    if (ptrSession->sessionCfg.u.hwCfg.opMode == CBUFF_OperationalMode_CONTINUOUS)
    {
        /* Sanity Check: Curently only the ADC Data mode is supported with continuous mode */
        if (ptrSession->sessionCfg.u.hwCfg.dataFormat != CBUFF_DataFmt_ADC_DATA)
        {
            *errCode = CBUFF_ENOTSUP;
            goto exit;
        }

        /* Sanity Check: Ensure that the chirp mode is correctly configured */
        if (ptrSession->sessionCfg.u.hwCfg.chirpMode != 0U)
        {
            *errCode = CBUFF_EINVAL;
            goto exit;
        }
    }
    else
    {
        /* Sanity Check: Validate the range. (Single or Multiple Chirp) */
        if ((ptrSession->sessionCfg.u.hwCfg.chirpMode < gCBUFFHwAttribute.chirpModeStartIndex) ||
            (ptrSession->sessionCfg.u.hwCfg.chirpMode > gCBUFFHwAttribute.chirpModeEndIndex))
        {
            /* Error: The chirp mode is not within the supported range */
            *errCode = CBUFF_EINVAL;
            goto exit;
        }
    }

    /************************************************************************
     * Sanity Check: Validate the user buffers
     ************************************************************************/
    if ((ptrSession->sessionCfg.u.hwCfg.dataFormat == CBUFF_DataFmt_ADC_USER) ||
        (ptrSession->sessionCfg.u.hwCfg.dataFormat == CBUFF_DataFmt_CP_ADC_CQ_USER))
    {
        /* User data will be streamed out: */
        for (index = 0U; index < CBUFF_MAX_USER_BUFFER; index++)
        {
            /* Validate the user buffers to make sure these can be streamed out via the CBUFF */
            if (CBUFF_validateBufferCfg (&ptrSession->sessionCfg.u.hwCfg.userBufferInfo[index], errCode) < 0)
            {
                /* Error: Invalid user buffer configuration error code is already setup */
                goto exit;
            }
        }
    }

    /************************************************************************
     * Sanity Check: ADCBUF Handle should be valid.
     ************************************************************************/
    if (ptrSession->sessionCfg.u.hwCfg.adcBufHandle == NULL)
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Cycle through the configuration and process it */
    ptrSession->numActiveADCChannels = 0U;
    for (index = 0U; index < SYS_COMMON_NUM_RX_CHANNEL; index++)
    {
        /* Get the ADCBUF Channel Address from the ADC Driver */
        ptrSession->rxChannelAddress[index] = ADCBuf_getChanBufAddr (ptrSession->sessionCfg.u.hwCfg.adcBufHandle, index, errCode);
        if (ptrSession->rxChannelAddress[index] != 0U)
        {
            /* YES: Increment the number of active ADC channels */
            ptrSession->numActiveADCChannels++;

            /* Translate the address to EDMA:  */
            ptrSession->rxChannelAddress[index] = SOC_translateAddress(ptrSession->rxChannelAddress[index],
                                                                       SOC_TranslateAddr_Dir_TO_EDMA, errCode);
            if (ptrSession->rxChannelAddress[index] == SOC_TRANSLATEADDR_INVALID)
            {
                /* Error: Unable to translate the address; error code is already setup */
                goto exit;
            }

            /* Track the cumulative Chirp Parameter Transfer Size */
            ptrSession->cpTotalTransferSize = ptrSession->cpTotalTransferSize + SYS_COMMON_CP_SIZE_CBUFF_UNITS;
        }
    }

    /************************************************************************
     * Sanity Check: ADC channels should be configured and be enabled
     ************************************************************************/
    if (ptrSession->numActiveADCChannels == 0U)
    {
        /* Error: No ADC channel has been enabled. We cannot calculate the ADC channel address
         * and setup the EDMA chains before the ADC is configured. */
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /************************************************************************
     * Sanity Check: Chirp Quality
     ************************************************************************/
    if ((ptrSession->sessionCfg.u.hwCfg.dataFormat == CBUFF_DataFmt_CP_ADC_CQ) ||
        (ptrSession->sessionCfg.u.hwCfg.dataFormat == CBUFF_DataFmt_CP_ADC_CQ_USER))
    {
        /* Initialize the CQ */
        ptrSession->numCQ = 0U;

        /* Cycle through all the Chirp Quality: */
        for (cqType = ADCBufMMWave_CQType_CQ0; cqType < ADCBufMMWave_CQType_MAX_CQ; cqType++)
        {
            /* Do we have a valid CQ to be streamed out? */
            if (ptrSession->sessionCfg.u.hwCfg.cqSize[cqType] != 0U)
            {
                /* YES: Get the CQ Base address from the ADC Driver */
                ptrSession->cqBuffer[ptrSession->numCQ].address = ADCBUF_MMWave_getCQBufAddr (ptrSession->sessionCfg.u.hwCfg.adcBufHandle,
                                                                                              cqType, errCode);
                if (ptrSession->cqBuffer[ptrSession->numCQ].address == 0U)
                {
                    /* Error: Unable to get the base address; error code is already setup */
                    goto exit;
                }

                /* Translate the address to EDMA: */
                ptrSession->cqBuffer[ptrSession->numCQ].address = SOC_translateAddress(ptrSession->cqBuffer[ptrSession->numCQ].address,
                                                                                       SOC_TranslateAddr_Dir_TO_EDMA, errCode);
                if (ptrSession->cqBuffer[ptrSession->numCQ].address == SOC_TRANSLATEADDR_INVALID)
                {
                    /* Error: Unable to translate the address; error code is already setup */
                    goto exit;
                }

                /* Store the CQn Size: */
                ptrSession->cqBuffer[ptrSession->numCQ].size = ptrSession->sessionCfg.u.hwCfg.cqSize[cqType];

                /* Increment the detected number of CQ: */
                ptrSession->numCQ++;
            }
        }

        /* Sanity Check: Do we have at least 1 CQ to be streamed out. */
        if (ptrSession->numCQ == 0U)
        {
            /* Error: There is no CQ. */
            *errCode = CBUFF_EINVAL;
            goto exit;
        }
    }

    /* HW Session configuration has been validated */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the SW Session configuration.
 *
 *  @param[in]  ptrSession
 *      Pointer to the session
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t CBUFF_validateSWSessionCfg (CBUFF_Session* ptrSession, int32_t* errCode)
{
    uint32_t    index;
    int32_t     retVal = MINUS_ONE;

    /************************************************************************
     * Sanity Check: Validate the user buffers
     ************************************************************************/
    for (index = 0U; index < CBUFF_MAX_USER_BUFFER; index++)
    {
        /* Validate the user buffers to make sure these can be streamed out via the CBUFF */
        if (CBUFF_validateBufferCfg (&ptrSession->sessionCfg.u.swCfg.userBufferInfo[index], errCode) < 0)
        {
            /* Error: Invalid user buffer configuration error code is already setup */
            goto exit;
        }
    }

    /* SW Session configuration has been validated */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to create a session with the specified configuration
 *
 *  @param[in]  cbuffHandle
 *      Handle to the driver
 *  @param[in]  ptrSessionCfg
 *      Pointer to the session configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Handle to the session
 *  @retval
 *      Error       - NULL
 */
CBUFF_SessionHandle CBUFF_createSession (CBUFF_Handle cbuffHandle, CBUFF_SessionCfg* ptrSessionCfg, int32_t* errCode)
{
    CBUFF_DriverMCB*        ptrDriverMCB;
    CBUFF_SessionHandle     sessionHandle;
    CBUFF_Session*          ptrSession = NULL;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the driver MCB: */
    ptrDriverMCB = (CBUFF_DriverMCB*)cbuffHandle;

    /* Sanity Check: Validate the arguments */
    if ((ptrDriverMCB == NULL) || (ptrSessionCfg == NULL))
    {
        /* Error: Invalid configuration */
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the arguments */
    if ((ptrSessionCfg->edmaHandle             == NULL) ||
        (ptrSessionCfg->allocateEDMAChannelFxn == NULL) ||
        (ptrSessionCfg->freeEDMAChannelFxn     == NULL))
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Allocate a session: */
    ptrSession = CBUFF_allocateSession (ptrDriverMCB);
    if (ptrSession == NULL)
    {
        /* Error: Out of sessions. Limit reached. */
        *errCode = CBUFF_ELIMIT;
        goto exit;
    }

    /* Copy over the session configuration: */
    memcpy ((void *)&ptrSession->sessionCfg, (void*)ptrSessionCfg, sizeof (CBUFF_SessionCfg));

    /* Do we need to add a header to the stream? */
    if (CBUFF_validateBufferCfg (&ptrSession->sessionCfg.header, errCode) < 0)
    {
        /* Error: Invalid header configuration error code is already setup */
        goto exit;
    }

    /* Process the session configuration on the basis of the execution mode: */
    if (ptrSession->sessionCfg.executionMode == CBUFF_SessionExecuteMode_HW)
    {
        /* Sanity Check: Validate the HW Session configuration */
        if (CBUFF_validateHWSessionCfg (ptrSession, errCode) < 0)
        {
            /* Error: Session configuration was invalid. Error code is setup */
            goto exit;
        }

        /* Determine the ADC Transfer Size: For Complex Samples we send I/Q */
        ptrSession->adcTransferSize = 0U;
        if (ptrSession->sessionCfg.dataType == CBUFF_DataType_REAL)
        {
            /* Real: */
            ptrSession->adcTransferSize = ptrSession->sessionCfg.u.hwCfg.numADCSamples;
        }
        else
        {
            /* Complex: */
            ptrSession->adcTransferSize = ptrSession->sessionCfg.u.hwCfg.numADCSamples * 2;
        }

        /* Setup the number of chirps per frame: */
        if (ptrSession->sessionCfg.u.hwCfg.opMode == CBUFF_OperationalMode_CHIRP)
        {
            /* Chirp Mode: Use the provided configuration */
            ptrSession->numChirpsPerFrame = (ptrSession->sessionCfg.u.hwCfg.numChirpsPerFrame / ptrSession->sessionCfg.u.hwCfg.chirpMode);
            ptrSession->adcTransferSize   = ptrSession->adcTransferSize * ptrSession->sessionCfg.u.hwCfg.chirpMode;
        }
        else
        {
            /* Continuous Mode: There are infinite chirps per frame. */
            ptrSession->numChirpsPerFrame = 0xFFFFFFFFU;
        }
    }
    else
    {
        /* Sanity Check: Validate the SW Session configuration */
        if (CBUFF_validateSWSessionCfg (ptrSession, errCode) < 0)
        {
            /* Error: Session configuration was invalid. Error code is setup */
            goto exit;
        }

        /* There is only 1 chirp per frame. */
        ptrSession->numChirpsPerFrame = 1U;
    }

    /* Setup the transfer: */
    if (CBUFF_setupTransfer (ptrSession, errCode) < 0)
    {
        /* Error: Unable to setup the transfer. Error code is already setup */
        goto exit;
    }

    /* Register for the Frame Done Interrupts */
    CBUFF_registerFrameDone (ptrDriverMCB);

exit:
    /* Was there an error? */
    if (*errCode != 0)
    {
        int32_t     tmpErrCode;

        /* Delete the session: Use a temporary error code since we dont want the
         * original failure to be masked. */
        CBUFF_deleteSession ((CBUFF_SessionHandle)ptrSession, &tmpErrCode);
        sessionHandle = NULL;
    }
    else
    {
        /* NO: Set the session handle */
        sessionHandle = (CBUFF_SessionHandle)ptrSession;
    }
    return sessionHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is used to delete the specific session.
 *
 *  @param[in]  sessionHandle
 *      Handle to the session to be activated
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CBUFF_deleteSession (CBUFF_SessionHandle sessionHandle, int32_t* errCode)
{
    int32_t             retVal = MINUS_ONE;
    CBUFF_DriverMCB*    ptrDriverMCB;
    CBUFF_Session*      ptrSession;

    /* Get the pointer to the session: */
    ptrSession = (CBUFF_Session*)sessionHandle;
    if (ptrSession == NULL)
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Sanity Check: Ensure that there exists a back pointer to the CBUFF Driver */
    ptrDriverMCB = ptrSession->ptrDriverMCB;
    DebugP_assert (ptrDriverMCB != NULL);

    /* Is the session currently active? */
    if (ptrDriverMCB->ptrCurrentActiveSession == ptrSession)
    {
        /* YES: Application needs to deactivate the session before it
         * can be deleted. This needs to be done explicitly by the application. */
        *errCode = CBUFF_EINUSE;
        goto exit;
    }

    /* Shutdown the transfer: */
    if (CBUFF_shutdownTransfer (ptrSession, errCode) < 0)
    {
        /* Error: Unable to shutdown the transfer. Error code is already setup */
        goto exit;
    }

    /* Free the session: */
    CBUFF_freeSession (ptrSession);

    /* Deregister for the Frame Done Interrupts */
    CBUFF_deregisterFrameDone (ptrDriverMCB);

    /* Session has been successfully deleted */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get/set information from the CBUFF Driver
 *
 *  @param[in]  cBuffHandle
 *      Handle to the driver
 *  @param[in]  cmd
 *      CBUFF command
 *  @param[in]  arg
 *      Command specified pointer to the argument
 *  @param[in]  argLen
 *      Length of the argument
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CBUFF_control
(
    CBUFF_Handle    cBuffHandle,
    CBUFF_Command   cmd,
    void*           arg,
    uint32_t        argLen,
    int32_t*        errCode
)
{
    CBUFF_DriverMCB*    ptrDriverMCB;
    int32_t             retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrDriverMCB = (CBUFF_DriverMCB*)cBuffHandle;
    if (ptrDriverMCB == NULL)
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Process the command: */
    switch (cmd)
    {
        case CBUFF_Command_GET_CBUFF_STATS:
        {
            CBUFF_Stats*    ptrStats;
            uint32_t        stats;

            /* Sanity Check: Validate the command specific arguments */
            if ((arg == NULL) || (argLen != sizeof (CBUFF_Stats)))
            {
                *errCode = CBUFF_EINVAL;
                goto exit;
            }

            /* Get the pointer to the statistics: */
            ptrStats = (CBUFF_Stats*)arg;

            /* Read the statistics register */
            stats = ptrDriverMCB->ptrCBUFFReg->STAT_CBUFF_REG1;

            /* Populate the statistics: */
            ptrStats->frameStartError = CSL_FEXTR(stats, 17U, 17U);
            ptrStats->chirpError      = CSL_FEXTR(stats, 16U, 16U);

            /* Copy over the CBUFF statistics: */
            ptrStats->numFrameStart      = ptrDriverMCB->totalNumFrameStart;
            ptrStats->numFrameDone       = ptrDriverMCB->totalNumFrameDone;
            ptrStats->numChirpDone       = ptrDriverMCB->totalNumChirpDone;
            ptrStats->numErrorInterrupts = ptrDriverMCB->totalNumErrorInterrupts;

            /* Command executed successfully */
            retVal = 0;
            break;
        }
        case CBUFF_Command_CLEAR_CBUFF_STATS:
        {
            /* Reset the statistics: */
            ptrDriverMCB->totalNumFrameStart      = 0U;
            ptrDriverMCB->totalNumFrameDone       = 0U;
            ptrDriverMCB->totalNumChirpDone       = 0U;
            ptrDriverMCB->totalNumErrorInterrupts = 0U;

            /* Command executed successfully */
            retVal = 0;
            break;
        }
        case CBUFF_Command_GET_ACTIVE_SESSION:
        {
            CBUFF_SessionHandle* ptrActiveSessionHandle;

            /* Sanity Check: Validate the command specific arguments */
            if ((arg == NULL) || (argLen != sizeof (CBUFF_SessionHandle)))
            {
                *errCode = CBUFF_EINVAL;
                goto exit;
            }

            /* Get the pointer to the argument which is to be populated */
            ptrActiveSessionHandle = (CBUFF_SessionHandle*)arg;

            /* Return the active session handle back */
            *ptrActiveSessionHandle = (CBUFF_SessionHandle)ptrDriverMCB->ptrCurrentActiveSession;

            /* Command executed successfully */
            retVal = 0;
            break;
        }
        default:
        {
            /* Error: This is not a valid command. */
            *errCode = CBUFF_EINVAL;
            break;
        }
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the CBUFF driver.
 *
 *  @param[in]  ptrInitCfg
 *      Pointer to the CBUFF Initialization configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Handle to the CBUFF Driver
 *  @retval
 *      Error       - NULL
 */
CBUFF_Handle CBUFF_init (CBUFF_InitCfg* ptrInitCfg, int32_t* errCode)
{
    CBUFF_Handle            retHandle;
    CBUFF_DriverMCB*        ptrDriverMCB = NULL;
    HwiP_Params             hwiParams;
    SOC_SysIntListenerCfg   listenerCfg;
    int32_t                 tmpErrCode;
    uint8_t                 index;
    CBUFF_Session*          ptrSession;

    /* Initialize the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    if (ptrInitCfg == NULL)
    {
        /* Error: Invalid configuration */
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the maximum number of sessions */
    if ((ptrInitCfg->maxSessions == 0U) || (ptrInitCfg->maxSessions > CBUFF_MAX_NUM_SESSION))
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Allocate memory for the CBUFF Driver: */
    ptrDriverMCB = MemoryP_ctrlAlloc (sizeof(CBUFF_DriverMCB), 0);
    if (ptrDriverMCB == NULL)
    {
        /* Error: Out of memory */
        *errCode = CBUFF_ENOMEM;
        goto exit;
    }

    /* Initialize the allocated memory: */
    memset ((void *)ptrDriverMCB, 0, sizeof(CBUFF_DriverMCB));

    /* Allocate memory for the sessions: */
    ptrDriverMCB->ptrSessionTable = MemoryP_ctrlAlloc ((sizeof(CBUFF_Session) * ptrInitCfg->maxSessions), 0);
    if (ptrDriverMCB->ptrSessionTable == NULL)
    {
        /* Error: Out of memory */
        *errCode = CBUFF_ENOMEM;
        goto exit;
    }

    /* Initialize the allocated memory: */
    memset ((void *)ptrDriverMCB->ptrSessionTable, 0, (sizeof(CBUFF_Session) * ptrInitCfg->maxSessions));

    /* Populate the driver block: Copy over the initialization configuration */
    memcpy ((void *)&ptrDriverMCB->initCfg, (void*)ptrInitCfg, sizeof(CBUFF_InitCfg));

    /* Sanity Check: Ensure that the high speed interface is supported or not? */
    ptrDriverMCB->interfaceFxn = CBUFF_isIfSupported (ptrDriverMCB->initCfg.interface);
    if (ptrDriverMCB->interfaceFxn == NULL)
    {
        /* NO: High speed interface is not supported. This seems to be an invalid configuration */
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Initialize and configure the driver MCB: */
    ptrDriverMCB->ptrCBUFFReg = (DSSCBUFFRegs*)gCBUFFHwAttribute.baseAddress;

    /* Reset the CBUFF Module: */
    CBUFF_reset (ptrDriverMCB->ptrCBUFFReg);

    /* Register the CBUFF Interrupt Handler: */
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "CBUFF_ISR";
    hwiParams.arg  = (uintptr_t)ptrDriverMCB;
    ptrDriverMCB->hwiISRHandle = HwiP_create(gCBUFFHwAttribute.cbuffInterrupt, CBUFF_ISR, &hwiParams);

    /* Register the Error Handler always: */
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "CBUFF_ErrorISR";
    hwiParams.arg  = (uintptr_t)ptrDriverMCB;
    ptrDriverMCB->hwiErrorISRHandle = HwiP_create(gCBUFFHwAttribute.errorInterrupt, CBUFF_ErrorISR, &hwiParams);

    /* Enable all error interrupts: */
    CBUFF_enableErrorInt (ptrDriverMCB->ptrCBUFFReg);

    /* Register the CBUFF & Frame Start Interrupt Handlers only if the debug mode is enabled.
     * We do not want to overwhelm the system with a large number of interrupts. */
     if (ptrDriverMCB->initCfg.enableDebugMode == true)
     {
        /* Register the CBUFF Frame Start Listener: */
        memset ((void*)&listenerCfg, 0, sizeof(SOC_SysIntListenerCfg));
        listenerCfg.systemInterrupt = gCBUFFHwAttribute.frameStartInterrupt;
        listenerCfg.listenerFxn     = &CBUFF_FrameStartISR;
        listenerCfg.arg             = (uintptr_t)ptrDriverMCB;
        ptrDriverMCB->frameStartListener = SOC_registerSysIntListener (ptrDriverMCB->initCfg.socHandle, &listenerCfg, errCode);
        if (ptrDriverMCB->frameStartListener == NULL)
        {
            /* Error: Unable to register the frame start listener. Error code is already setup. */
            goto exit;
        }

        /* Debug Mode: Enable the Chirp & Frame Done Interrupt: */
        CBUFF_enableChirpDoneInt (ptrDriverMCB->ptrCBUFFReg);
        CBUFF_enableFrameDoneInt (ptrDriverMCB->ptrCBUFFReg);
    }
    else
    {
        /* Normal Mode: Disable the chirp & frame done interrupts. */
        CBUFF_disableChirpDoneInt (ptrDriverMCB->ptrCBUFFReg);
        CBUFF_disableFrameDoneInt (ptrDriverMCB->ptrCBUFFReg);
    }

    /* Cycle through and initialize the session table: */
    for (index = 0U; index < ptrDriverMCB->initCfg.maxSessions; index++)
    {
        /* Get the session: */
        ptrSession = &ptrDriverMCB->ptrSessionTable[index];

        /* Initialize the session: */
        ptrSession->ptrDriverMCB = ptrDriverMCB;
        ptrSession->isValid      = false;
        ptrSession->dmaNum       = index;
    }

    /* Initialize the high speed interface (HSI) on which the CBUFF is operating: */
    if (ptrDriverMCB->interfaceFxn->initFxn (ptrDriverMCB, errCode) < 0)
    {
        /* Error: Unable to initialize the HSI. Error code is already setup */
        goto exit;
    }

    /* Reset the linked list entries in the CBUFF IP: */
    for (index = 0U; index < CBUFF_MAX_LINKED_LIST_SIZE; index++)
    {
        CBUFF_clearLLEntry (ptrDriverMCB->ptrCBUFFReg, index);
    }

    /* Setup the ECC: */
    CBUFF_setECC (ptrDriverMCB->ptrCBUFFReg, ptrDriverMCB->initCfg.enableECC);

    /* Default State: The triggers are always owned by the software */
    CBUFF_setFrameStartSWTrigger (ptrDriverMCB->ptrCBUFFReg, 1U);
    CBUFF_setChirpAvailableSWTrigger (ptrDriverMCB->ptrCBUFFReg, 1U);

    /* Release the reset of the CBUFF Module.  */
    CBUFF_releaseReset (ptrDriverMCB->ptrCBUFFReg);

exit:
    /* Is there an error? */
    if (*errCode != 0)
    {
        /* YES: CBUFF Initialization failed; setup the return value*/
        retHandle = NULL;

        /* Do we need to shutdown the driver? */
        if (ptrDriverMCB != NULL)
        {
            /* Deinitialize and shutdown the driver.
             * - We use a temporary error code because we dont want to hide the real
             *   reason for the failure. */
            CBUFF_deinit (ptrDriverMCB, &tmpErrCode);
        }
    }
    else
    {
        /* NO. CBUFF Initialization was successful */
        retHandle = (CBUFF_Handle)ptrDriverMCB;
    }
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize and shutdown the CBUFF driver.
 *
 *  @param[in]  cBuffHandle
 *      Handle to the driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CBUFF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CBUFF_deinit (CBUFF_Handle cBuffHandle, int32_t* errCode)
{
    CBUFF_DriverMCB*    ptrDriverMCB;
    CBUFF_Session*      ptrSession;
    int32_t             retVal = MINUS_ONE;
    uint8_t             sessionIndex;

    /* Initialize the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrDriverMCB = (CBUFF_DriverMCB*)cBuffHandle;
    if (ptrDriverMCB == NULL)
    {
        *errCode = CBUFF_EINVAL;
        goto exit;
    }

    /* Sanity Check: Ensure that there are no activated sessions. Applications
     * should explicitly deactivate them */
    if (ptrDriverMCB->ptrCurrentActiveSession != NULL)
    {
        /* Error: There is already an active session */
        *errCode = CBUFF_EINUSE;
        goto exit;
    }

    /* Disable all the interrupts: The driver is going down. */
    CBUFF_disableErrorInt (ptrDriverMCB->ptrCBUFFReg);
    CBUFF_disableFrameDoneInt (ptrDriverMCB->ptrCBUFFReg);
    CBUFF_disableChirpDoneInt (ptrDriverMCB->ptrCBUFFReg);

    /* Was the session table created? */
    if (ptrDriverMCB->ptrSessionTable != NULL)
    {
        /* YES: Cycle through and delete all the valid sessions */
        for (sessionIndex = 0U; sessionIndex < ptrDriverMCB->initCfg.maxSessions; sessionIndex++)
        {
            /* Get the session: */
            ptrSession = &ptrDriverMCB->ptrSessionTable[sessionIndex];

            /* Was the session valid? */
            if (ptrSession->isValid == true)
            {
                /* YES: Delete the session */
                retVal = CBUFF_deleteSession ((CBUFF_SessionHandle)ptrSession, errCode);
                if (retVal < 0)
                {
                    /* Error: Unable to delete the session; error code is already populated */
                    goto exit;
                }
            }
        }

        /* Cleanup the memory for the allocated session table */
        MemoryP_ctrlFree (ptrDriverMCB->ptrSessionTable, (sizeof(CBUFF_Session) * ptrDriverMCB->initCfg.maxSessions));
    }

    /* Deregister the CBUFF Error ISR Handler only if had been registered */
    if (ptrDriverMCB->hwiErrorISRHandle != NULL)
    {
        HwiP_delete(ptrDriverMCB->hwiErrorISRHandle);
    }

    /* Deregister the CBUFF Interrupt handlers only if it had been registered */
    if (ptrDriverMCB->hwiISRHandle != NULL)
    {
        HwiP_delete(ptrDriverMCB->hwiISRHandle);
    }

    /* Deregister the Frame Start Interrupt listener only if had been registered */
    if (ptrDriverMCB->frameStartListener != NULL)
    {
        /* Deregister the frame start listener */
        retVal = SOC_deregisterSysIntListener (ptrDriverMCB->initCfg.socHandle, gCBUFFHwAttribute.frameStartInterrupt,
                                               ptrDriverMCB->frameStartListener, errCode);
        if (retVal < 0)
        {
            /* Error: Unable to deregister the frame start listener; error code is already setup. */
            goto exit;
        }
    }

    /* Deinitialize the high speed interface: */
    if (ptrDriverMCB->interfaceFxn->deinitFxn (ptrDriverMCB, errCode) < 0)
    {
        /* Error: Unable to deinitialize the HSI. Error code is already setup */
        goto exit;
    }

    /* Cleanup the memory */
    MemoryP_ctrlFree (ptrDriverMCB, sizeof(CBUFF_DriverMCB));

    /* CBUFF has been deinitialized successfully. */
    retVal = 0;

exit:
    return retVal;
}

