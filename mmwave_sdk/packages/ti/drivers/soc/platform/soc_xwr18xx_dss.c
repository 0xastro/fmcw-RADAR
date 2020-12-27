/*!
 *   @file  soc_xwr18xx_dss.c
 *
 *   @brief
 *      This is the XWR18xx DSS platform specific wrapper for the SOC module.
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
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/soc/include/soc_internal.h>
#include <ti/drivers/osal/DebugP.h>

/**************************************************************************
 ************************** Global Definitions ****************************
 **************************************************************************/

/**
 * @brief
 *  SOC Hardware Attributes
 *
 * @details
 *  The structure describes the hardware attributes which is needed
 *  to initialize and setup the SOC Driver. This is defined for the XWR18xx
 *  DSS and should NOT be changed
 */
SOC_HwAttrib gSOCHwAttrib =
{
    SOC_XWR18XX_DSS_RCM_BASE_ADDRESS,       /* RCM Base Address:            */
    SOC_XWR18XX_DSS_TOP_RCM_BASE_ADDRESS,   /* TOP RCM Base Address:        */
    SOC_XWR18XX_DSS_GPCFG_BASE_ADDRESS,     /* GPCFG Base Address:          */
    SOC_XWR18XX_DSS_DSSREG_BASE_ADDRESS,    /* DSS Base Address:            */
    SOC_XWR18XX_DSS_DSSREG2_BASE_ADDRESS,   /* DSSREG2 Base Address         */
    SOC_XWR18XX_DSP_ICFG_BASE_ADDRESS       /* DSP ICFG Base Address        */
};

/**************************************************************************
 *************************** SOC Platform Functions ***********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This function returns the address domain for input address
 *
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid domain
 *  @retval
 *      Error       - SOC_TranslateAddr_LUT_Type_INVALID
 */
SOC_TranslateAddr_LUT_Type SOC_getInAddr_LUTType(SOC_TranslateAddr_Dir direction)
{
    SOC_TranslateAddr_LUT_Type inAddrLUTtype;

    /* get the inAddr domains based on direction */
    switch(direction)
    {
        case SOC_TranslateAddr_Dir_TO_EDMA:
        case SOC_TranslateAddr_Dir_TO_OTHER_CPU:
            inAddrLUTtype= SOC_TranslateAddr_LUT_Type_DSS;
        break;

        case SOC_TranslateAddr_Dir_FROM_EDMA:
            inAddrLUTtype= SOC_TranslateAddr_LUT_Type_EDMA;
        break;

        case SOC_TranslateAddr_Dir_FROM_OTHER_CPU:
            inAddrLUTtype= SOC_TranslateAddr_LUT_Type_MSS;
        break;

        default:
            inAddrLUTtype = SOC_TranslateAddr_LUT_Type_INVALID;
        break;
    }

    return inAddrLUTtype;
}

/**
 *  @b Description
 *  @n
 *      This function returns the address domain for output address
 *
 *  @param[in] direction
 *      hint on which domain to translate from and to
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid domain
 *  @retval
 *      Error       - SOC_TranslateAddr_LUT_Type_INVALID
 */
SOC_TranslateAddr_LUT_Type SOC_getOutAddr_LUTType(SOC_TranslateAddr_Dir direction)
{
    SOC_TranslateAddr_LUT_Type outAddrLUTtype;

    /* get the inAddr domains based on direction */
    switch(direction)
    {
        case SOC_TranslateAddr_Dir_TO_EDMA:
            outAddrLUTtype = SOC_TranslateAddr_LUT_Type_EDMA;
        break;

        case SOC_TranslateAddr_Dir_FROM_EDMA:
        case SOC_TranslateAddr_Dir_FROM_OTHER_CPU:
            outAddrLUTtype = SOC_TranslateAddr_LUT_Type_DSS;
        break;

        case SOC_TranslateAddr_Dir_TO_OTHER_CPU:
            outAddrLUTtype = SOC_TranslateAddr_LUT_Type_MSS;
        break;

        default:
            outAddrLUTtype = SOC_TranslateAddr_LUT_Type_INVALID;
        break;

    }

    return outAddrLUTtype;
}



/**
 *  @b Description
 *  @n
 *      This function checks TopRCM SPARE0 Register and wait until BSS is powered up.
 *
 *  @param[in] handle
 *      Handle to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t SOC_waitBSSPowerUp(SOC_Handle handle, int32_t* errCode)
{
    SOC_DriverMCB*  ptrSOCDriverMCB;
    int32_t         retVal = 0;

    /* Get the pointer to the SOC Driver Block: */
    ptrSOCDriverMCB = (SOC_DriverMCB*)handle;
    if (ptrSOCDriverMCB == NULL)
    {
        *errCode = SOC_EINVAL;
        retVal   = MINUS_ONE;
    }
    else
    {
        /* Wait until BSS powerup is done */
        while (CSL_FEXTR(ptrSOCDriverMCB->ptrTopRCMRegs->SPARE0, 18U, 16U) != 0x7U)
        {
        }
    }
    return retVal;
}

/**
 * @brief
 *  SOC_configMARs
 *
 * @details
 *  Configures the MAR (Memory Attribute Register) registers. Note that all peripheral
 *  config/control space is already in read-only non-cacheable MARs as documented
 *  in section "Special MAR registers" in
 *  C674 Megamodule UG: http://www.ti.com/lit/ug/sprufk5a/sprufk5a.pdf.
 *  The only MAR to be controlled presently is the Mailbox memory which we are
 *  marking to be non-cacheable after some considerations. Mailbox driver currently
 *  assumes non-cacheable Mailbox memory. Note non-cacheable is also the chip reset state
 *  of MARs.
*/
static void SOC_configMARs(void)
{
    DSPICFGRegs *reg = (DSPICFGRegs*)gSOCHwAttrib.dspIcfgRegBaseAddress;

    /* Mailbox, MAR80 - 5000 0000h - 50FF FFFFh, set non-cacheable(PC=0) */
    CSL_FINSR (reg->MAR_80, 0U, 0U, 0);

    /* In future, CBUFF-FIFO may need to be configured if we allow DSP CPU to
       I/O in the CBUFF driver (presently not, only EDMA accesses CBUFF FIFO) so
       MAR setting is don't care. The CBUFF FIFO is MAR33 - 2100 0000h - 21FF FFFFh.
       Note however this also covers the HS-RAM and ADC buffers i.e all three
       of these go together, their cacheabilities cannot be set independently */
    /* CSL_FINSR (reg->MAR_33, 0U, 0U, 0); */

    /* Drivers do not operate on L3 RAM and HS-RAM, so L3/HS-RAM cacheability is left to
       application/demo code to set and do cache (writeback/invalidate etc)
       operations from the application as necessary, depending on the use cases.
       L3 MAR is MAR32 -> 2000_0000h - 20FF_FFFFh
       HS-RAM MAR is MAR33 -> 2100_0000h - 21FF_FFFFh
    */
}

/**
 *  @b Description
 *  @n
 *      This is a utility function which has been provided to
 *      add a delay in micro-second granularity.
 *
 *  @param[in] delayInMircoSecs
 *      Delay in microsecond
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void SOC_microDelay (uint32_t delayInMircoSecs)
{
    uint32_t index;

    for (index = 0; index < (DSP_CLOCK_MHZ * delayInMircoSecs); index++)
    {
        asm("");
    }
}

/**
 *  @b Description
 *  @n
 *      SOC Device specific initialization which is implemented for the XWR18xx DSS
 *
 *  @param[in] ptrSOCDriverMCB
 *      Pointer to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval   None
 */
void SOC_deviceInit (SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode)
{
    HwiP_Params     hwiParams;
    uint8_t         sysIntIndex = 0U;

    /* The DSS2 Register are valid only for the XWR16xx/XWR18xx */
    ptrSOCDriverMCB->ptrDSS2Regs = (DSS2Regs*)gSOCHwAttrib.dssReg2BaseAddress;
    ptrSOCDriverMCB->ptrDSPICFGRegs = (DSPICFGRegs*)gSOCHwAttrib.dspIcfgRegBaseAddress;
    SOC_configMARs();

    /***********************************************************************************
     * Frame Start System Interrupt
     ***********************************************************************************/
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "FrameStartISR";
    hwiParams.arg  = (uintptr_t)&ptrSOCDriverMCB->sysIntTable[sysIntIndex];
    ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum    = SOC_XWR18XX_DSS_INTC_EVENT_FRAME_START;
    ptrSOCDriverMCB->sysIntTable[sysIntIndex].hwiHandle = HwiP_create(ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum,
                                                                      SOC_SystemISR, &hwiParams);
    HwiP_disableInterrupt (ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum);
    sysIntIndex++;

    /***********************************************************************************
     * Chirp System Interrupt
     ***********************************************************************************/
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "ChirpAvailableISR";
    hwiParams.arg  = (uintptr_t)&ptrSOCDriverMCB->sysIntTable[sysIntIndex];
    ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum    = SOC_XWR18XX_DSS_INTC_EVENT_CHIRP_AVAIL;
    ptrSOCDriverMCB->sysIntTable[sysIntIndex].hwiHandle = HwiP_create(ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum,
                                                                      SOC_SystemISR, &hwiParams);
    HwiP_disableInterrupt (ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum);
    sysIntIndex++;

    /* Sanity Check: Ensure that the number of interrupts registered does not exceed the internal limits */
    DebugP_assert(sysIntIndex < SOC_MAX_SYSTEM_INTERRUPT);
}

/**
 *  @b Description
 *  @n
 *      SOC Device specific deinitialization which is implemented for the XWR18xx DSS
 *
 *  @param[in] ptrSOCDriverMCB
 *      Pointer to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval   None
 */
void SOC_deviceDeinit(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode)
{
    uint8_t sysIntIndex = 0U;

    /* Cycle through all the system interrupts and close them */
    for (sysIntIndex = 0; sysIntIndex < SOC_MAX_SYSTEM_INTERRUPT; sysIntIndex++)
    {
        if (ptrSOCDriverMCB->sysIntTable[sysIntIndex].hwiHandle != NULL)
        {
            HwiP_delete (ptrSOCDriverMCB->sysIntTable[sysIntIndex].hwiHandle);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      This function is used to trigger software interrupt from DSS to MSS.
 *
 *  @param[in] handle
 *      Handle to the SOC Driver.
 *  @param[in] interruptNumber
 *      Interrupt number.
 *  @param[out] errCode
 *      Error code (@ref SOC_DRIVER_ERROR_CODE) populated on error.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 */
int32_t SOC_triggerDSStoMSSsoftwareInterrupt(SOC_Handle handle, uint8_t interruptNumber,
    int32_t *errCode)
{
    SOC_DriverMCB*  ptrSOCDriverMCB;
    int32_t retVal = 0;

    /* Get the pointer to the SOC Driver Block: */
    ptrSOCDriverMCB = (SOC_DriverMCB*)handle;

    /* check for errors */
    if ((ptrSOCDriverMCB == NULL) || ((interruptNumber < 1U) || (interruptNumber > 2U)))
    {
        *errCode = SOC_EINVAL;
        retVal = MINUS_ONE;
        goto exit;
    }

    /* issue interrupt */
    if (interruptNumber == 1U)
    {
        ptrSOCDriverMCB->ptrDSS2Regs->DSS2MSSSWIRQ =
        CSL_FINSR (ptrSOCDriverMCB->ptrDSS2Regs->DSS2MSSSWIRQ, DSS2MSSSWIRQ_MSSSWIRQ1_BIT_END,
                   DSS2MSSSWIRQ_MSSSWIRQ1_BIT_START, 1U);
    }
    else
    {
        ptrSOCDriverMCB->ptrDSS2Regs->DSS2MSSSWIRQ =
        CSL_FINSR (ptrSOCDriverMCB->ptrDSS2Regs->DSS2MSSSWIRQ, DSS2MSSSWIRQ_MSSSWIRQ2_BIT_END,
               DSS2MSSSWIRQ_MSSSWIRQ2_BIT_START, 1U);
    }
exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      This function is used to configure the bit level mask for each of the group 2 error signal connected to DSS ESM input.
 *
 *  @param[in] handle
 *      Handle to the SOC Driver.
 *  @param[in] errorNumber
 *      ESM error number denoting the bit position that should be masked or unmasked.
 *  @param[in] mask
 *      Mask error flag, 0 - Unmask, 1 - Mask
 *  @param[out] errCode
 *      Error code (@ref SOC_DRIVER_ERROR_CODE) populated on error.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 */
int32_t SOC_configureDSSESMMask(SOC_Handle handle, uint8_t errorNumber, uint8_t mask, int32_t *errCode)
{
    SOC_DriverMCB*  ptrSOCDriverMCB;
    int32_t retVal = 0;

    /* Get the pointer to the SOC Driver Block: */
    ptrSOCDriverMCB = (SOC_DriverMCB*)handle;

    /* check for errors */
    if (ptrSOCDriverMCB == NULL)
    {
        *errCode = SOC_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Confgiure the mask */
        ptrSOCDriverMCB->ptrDSSRegs->ESMGRP2MASKCFG = CSL_FINSR (ptrSOCDriverMCB->ptrDSSRegs->ESMGRP2MASKCFG,
                                                                       errorNumber,
                                                                       errorNumber,
                                                                       (uint32_t)mask);
    }
    return(retVal);
}

