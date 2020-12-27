/*!
 *   @file  soc_xwr68xx_mss.c
 *
 *   @brief
 *      This is the XWR68xx MSS platform specific wrapper for the SOC module.
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
#include <stdio.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/soc/include/soc_internal.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/include/soc_xwr68xx.h>
#include <ti/drivers/soc/include/reg_toprcm_xwr16xx.h>
#include <ti/drivers/osal/DebugP.h>

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/
static void SOC_mpu_config(void);
static int32_t SOC_unhaltDSS(SOC_Handle handle, int32_t* errCode);
static uint32_t SOC_getL3RAMNumBanks(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode);
static uint32_t SOC_getTCMANumBanks(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode);
static uint32_t SOC_getTCMBNumBanks(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode);

/**************************************************************************
 ************************** Global Definitions ****************************
 **************************************************************************/

/**
 * @brief
 *  SOC Hardware Attributes
 *
 * @details
 *  The structure describes the hardware attributes which is needed
 *  to initialize and setup the SOC Driver. This is defined for the XWR68xx
 *  MSS and should NOT be changed
 */
SOC_HwAttrib gSOCHwAttrib =
{
    SOC_XWR68XX_MSS_RCM_BASE_ADDRESS,       /* RCM Base Address:            */
    SOC_XWR68XX_MSS_TOP_RCM_BASE_ADDRESS,   /* TOP RCM Base Address:        */
    SOC_XWR68XX_MSS_GPCFG_BASE_ADDRESS,     /* GPCFG Base Address:          */
    SOC_XWR68XX_MSS_DSSREG_BASE_ADDRESS,    /* DSS Base Address:            */
    SOC_XWR68XX_MSS_DSSREG2_BASE_ADDRESS    /* DSSREG2 Base Address         */
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
            inAddrLUTtype= SOC_TranslateAddr_LUT_Type_MSS;
        break;

        case SOC_TranslateAddr_Dir_FROM_EDMA:
            inAddrLUTtype= SOC_TranslateAddr_LUT_Type_EDMA;
        break;

        case SOC_TranslateAddr_Dir_FROM_OTHER_CPU:
            inAddrLUTtype= SOC_TranslateAddr_LUT_Type_DSS;
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
            outAddrLUTtype = SOC_TranslateAddr_LUT_Type_MSS;
        break;

        case SOC_TranslateAddr_Dir_TO_OTHER_CPU:
            outAddrLUTtype = SOC_TranslateAddr_LUT_Type_DSS;
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
        while (CSL_FEXTR(ptrSOCDriverMCB->ptrTopRCMRegs->SPARE0, 18U, 16U) != 0x7)
        {
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize MPU (Memory Protection Unit)
 *      settings. This is valid only for the R4. Information about MPU
 *      programming is available at:-
 *
 *      http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0363g/Chdcahcf.html.
 *
   -# This code comes up after the C initialization (from _c_int00 to main).
      Presently, the application comes up in privilege mode, not user mode. So the
      access permissions are set accordingly, even though user mode permissions
      are set as if true user mode is there (in case in future we decide to do user mode).
   -# There is no cache on the R4F on the AR parts, so regions that would otherwise
      make sense to be cacheable are made non-cache.
   -# Peripheral space is strongly ordered instead of device because writes need
      to go through when they are issued and not at a later (uncertain time when next
      read forces the writes sitting in write buffer to be flushed out,
      reference: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0363g/Chdcahcf.html).
      An alternative way is to use the Device type instead of strongly orderered but this
      requires all driver code to be scrutinized for APIs that do a write at the end
      and for such APIs a "dsb" instruction needs to be issued. This is prone to errors
      during maintenance work so it is safer to choose strongly-ordered. Presently,
      the architecture is more memory size constrained than cycles (MIPS) constrained
      so safety benefits at potentially increased cycles are a good trade-off.
   -# L3 Memory has been set as normal. This means <B> for any transfer of ownership from
      R4F to non-R4F (peripherals, HW acc), a "dsb" instruction should be issued after the
      last write and before notifying/triggering the non-R4F entity.</B>
   -# Mailbox Memory has been set as normal. This means <B> for any transfer of ownership from
      R4F to non-R4F (BSS for example), a "dsb" instruction (MEM_BARRIER()) should be issued after the last
      write to the mailbox message memory before notifying/triggering the non-R4F entity.</B>
   -# HSRAM Memory has been set as normal. This means <B> for any transfer of ownership from
      R4F to DSS, a "dsb" instruction (MEM_BARRIER()) should be issued after the last
      write to the HSRAM memory before notifying/triggering the DSS.</B>
   -# ADC buffer, Chirp buffer, FFTC buffers (Mem0-3) (starting at 0x5200_0000)
      are combined with DSS peripheral region (starting at 0x5207_0000 + 128 bytes)
      are marked as strongly ordered even though probably the first three could be
      not strongly ordered and enforcement done through dsb when transferring
      ownership (similar to L3 case above). In practice the use cases for R4F to
      directly operate on these buffers is assumed to be for debug purposes so
      should not impact cycles really for production use cases.
      Combining L3 memory with ADC + Chirp + FFTc  was not feasible because of the
      power of 2 size constraint for the regions (if we do the math, it will overlap into the
      DSS peripheral region which needs strong ordering).
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void SOC_mpu_config(void)
{
   SOC_MPUDisable();
   SOC_MPUDisableBackgroundRegion();

   /* Background Region */
   SOC_MPUSetRegion(SOC_MPU_REGION1);
   SOC_MPUSetRegionBaseAddress(0x0);
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_STRONGLYORDERED_SHAREABLE, SOC_MPU_PRIV_NA_USER_NA_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_4_GB);

   /*
      Setup region 2, TCMA IRAM, base address = MSS_TCMA_BASE_ADDRESS, size = 512 KB
      Note: Actual size is 128 KB (ROM) + 256 KB (RAM), next power of 2 is 512 KB
      Access Permissions:  Read Only (Privileged / User)
      Execute Permissions: Execute,
      Memory Type:         Non-Cached, Non-Shared, Normal
    */
#if ((SOC_XWR68XX_MSS_TCMA_BASE_ADDRESS & (512U * ONE_KB - 1)) != 0)
#error SOC_XWR68XX_MSS_TCMA_BASE_ADDRESS not aligned to 512 KB
#endif
   SOC_MPUSetRegion(SOC_MPU_REGION2);
   SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_TCMA_BASE_ADDRESS);
#ifdef DOWNLOAD_FROM_CCS
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RW_EXEC);
#else
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RO_USER_RO_EXEC);
#endif
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_512_KB);

   /*
      Setup region 3, TCMB DRAM, base address = MSS_TCMB_BASE_ADDRESS, size = 256 KB (actual 192 KB),
      Access Permissions:  Full Access
      Execute Permissions: Execute Never
      Memory Type:         Non-Cached, Non-Shared, Normal
    */
#if ((SOC_XWR68XX_MSS_TCMB_BASE_ADDRESS & (256U * ONE_KB - 1)) != 0)
#error SOC_XWR68XX_MSS_TCMB_BASE_ADDRESS not aligned to 256 KB
#endif
    SOC_MPUSetRegion(SOC_MPU_REGION3);
   SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_TCMB_BASE_ADDRESS);
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RW_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_256_KB);


   /* Setup region 4, [Ext Flash-QSPI], base address = EXT_FLASH_BASE_ADDRESS, size = 8 MB,
      Access Permissions:  Full Access
      Execute Permissions: Execute Never
      Memory Type:         Non-Cached, Non-Shared, Normal
    */
#if ((SOC_XWR68XX_MSS_EXT_FLASH_BASE_ADDRESS & (8U * ONE_MB - 1)) != 0)
#error SOC_XWR68XX_MSS_EXT_FLASH_BASE_ADDRESS not aligned to 8 MB
#endif
   SOC_MPUSetRegion(SOC_MPU_REGION4);
   SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_EXT_FLASH_BASE_ADDRESS);
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RW_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_8_MB);

   /* Setup region 5, [QSPI Reg], base address = MSS_QSPI_BASE_ADDRESS, size = 128 Bytes,
      Access Permissions:  Privilege - full access, User - read only
      Execute Permissions: Execute Never
      Memory Type:         Strongly-ordered, shareable
    */
#if ((SOC_XWR68XX_MSS_QSPI_BASE_ADDRESS & (128U - 1)) != 0)
#error SOC_XWR68XX_MSS_QSPI_BASE_ADDRESS not aligned to 128 bytes
#endif
   SOC_MPUSetRegion(SOC_MPU_REGION5);
   SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_QSPI_BASE_ADDRESS);
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_STRONGLYORDERED_SHAREABLE, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_128_BYTES);

   /*
      Setup region 6, MailBox reg + MCRC + Peripheral region, base address = 0xF0000000, size = 256 MB,
      Access Permissions:  Privilege - full access, User - read only
      Execute Permissions: Execute Never
      Memory Type:         Strongly-ordered, shareable
      NOTE: This overlaps with next region 7. The choice is because of MPU number of
            regions and size being power of 2 limitation. Even though there is no
            physical space from 0xF000_0000 to SOC_XWR68XX_MSS_MBOX_BSS_MSS_REG_BASE_ADDRESS (0xF060_8600),
            we program from 0xF000_0000 to cover all the way to the last address at 0xFFFFFFFF
            as register space. The exception is the mailbox memory area in this space
            which the next region 7 is used to set as normal. Because 7 > 6,
            the region 7 will take effect for the mailbox memory.
    */
   SOC_MPUSetRegion(SOC_MPU_REGION6);
   SOC_MPUSetRegionBaseAddress(0xF0000000U);
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_STRONGLYORDERED_SHAREABLE, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_256_MB);

   /*
      Mailbox memories are defined in sys_commmon_xwr68xx_mss.h:

      #define SOC_XWR68XX_MSS_MBOX_MSS_BSS_MEM_BASE_ADDRESS 0xF0602000U
      #define SOC_XWR68XX_MSS_MBOX_BSS_MSS_MEM_BASE_ADDRESS 0xF0601000U
      #define SOC_XWR68XX_MSS_MBOX_MSS_DSS_MEM_BASE_ADDRESS 0xF0604000U
      #define SOC_XWR68XX_MSS_MBOX_DSS_MSS_MEM_BASE_ADDRESS 0xF0605000U

      so need to cover (in power of 2) 32 KB (0x8000)
      Note doubling of size compared to XWR14xx because of additional DSS on XWR68xx.
      The end of the section is now at F0600000 + 32 KB = F0608000 (the start of mailbox config space)

      Setup region 7, MailBox memory, base address = 0xF0600000, size = 32 KB,
      Access Permissions:  Privilege Full Access - User Read Only
      Execute Permissions: Execute Never
      Memory Type:         Non-Cached, Non-Shared, Normal
    */
   SOC_MPUSetRegion(SOC_MPU_REGION7);
   SOC_MPUSetRegionBaseAddress(SOC_MPU_ALIGN(SOC_XWR68XX_MSS_MBOX_BSS_MSS_MEM_BASE_ADDRESS, 32 * ONE_KB));
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_SHARED, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_32_KB);
   DebugP_log1("SOC MPU region 7: address = %x\n", SOC_MPU_ALIGN(SOC_XWR68XX_MSS_MBOX_BSS_MSS_MEM_BASE_ADDRESS, 32 * ONE_KB));

    /*
       Setup region 8, HSRAM, base address = 0x52080000, size = 32K Bytes,
       Access Permissions:  Full Access
       Execute Permissions: Execute Never
       Memory Type:         Non-Cached, Non-Shared, Normal
    */
   SOC_MPUSetRegion(SOC_MPU_REGION8);
   SOC_MPUSetRegionBaseAddress(SOC_MPU_ALIGN(SOC_XWR68XX_MSS_HSRAM_BASE_ADDRESS, 32U * ONE_KB));
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RW_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_32_KB);
   DebugP_log1("SOC MPU region 8: address = %x\n", SOC_MPU_ALIGN(SOC_XWR68XX_MSS_HSRAM_BASE_ADDRESS, 32 * ONE_KB));

   /*
       Setup region 9, DSS Peripheral region, base address = 0x50000000 , size = 2 MB,
       Access Permissions:  Privilege - full access, User - read only
       Execute Permissions: Execute Never
       Memory Type:         Strongly-ordered Shared
    */
   SOC_MPUSetRegion(SOC_MPU_REGION9);
   SOC_MPUSetRegionBaseAddress(SOC_MPU_ALIGN(EDMA_CC0_TC0_BASE_ADDRESS, 2*ONE_MB));
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_STRONGLYORDERED_SHAREABLE, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_2_MB);
   DebugP_log1("SOC MPU region 9: address = %x\n", SOC_MPU_ALIGN(EDMA_CC0_TC0_BASE_ADDRESS, 2*ONE_MB));

   /*
       Setup region 10, L3 memory, base address = 0x51000000, size = 1 MB (Actual size is 768 KB),
       Access Permissions:  Full Access
       Execute Permissions: Execute Never
       Memory Type:         Non-Cached, Non-Shared, Normal
    */
   SOC_MPUSetRegion(SOC_MPU_REGION10);
   SOC_MPUSetRegionBaseAddress(SOC_MPU_ALIGN(SOC_XWR68XX_MSS_L3RAM_BASE_ADDRESS, 1U * ONE_MB));
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RW_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_1_MB);
   DebugP_log1("SOC MPU region 10: address = %x\n", SOC_MPU_ALIGN(SOC_XWR68XX_MSS_L3RAM_BASE_ADDRESS, 1U * ONE_MB));

    /* Setup region 11, ADC buffer + Chirp + FFTC + DSS Peripheral region,
        base address = 0x52000000, size = 512 KB,
       Access Permissions:  Full access
       Execute Permissions: Execute Never
       Memory Type:         Strongly-ordered Shared
    */
#if ((SOC_XWR68XX_MSS_ADCBUF_BASE_ADDRESS & (512U * ONE_KB - 1)) != 0)
#error SOC_XWR68XX_MSS_ADCBUF_BASE_ADDRESS not aligned to 512 KB
#endif
   SOC_MPUSetRegion(SOC_MPU_REGION11);
   SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_ADCBUF_BASE_ADDRESS);
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_STRONGLYORDERED_SHAREABLE, SOC_MPU_PRIV_RW_USER_RW_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_512_KB);

    /*
       Setup region 12, Software buffer, base address = 0x0C200000, size = 8 KB,
       Access Permissions:  Full Access
       Execute Permissions: Execute Never
       Memory Type:         Non-Cached, Non-Shared, Normal
    */
#if ((SOC_XWR68XX_MSS_SW_BUFFER_BASE_ADDRESS & (8U * ONE_KB - 1)) != 0)
#error SOC_XWR68XX_MSS_SW_BUFFER_BASE_ADDRESS not aligned to 8 KB
#endif
   SOC_MPUSetRegion(SOC_MPU_REGION12);
   SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_SW_BUFFER_BASE_ADDRESS);
   SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RW_NOEXEC);
   SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_8_KB);

   SOC_MPUEnableBackgroundRegion();
   SOC_MPUEnable();
}


/**
 *  @b Description
 *  @n
 *      The function is used to unhalt the DSS
 *
 *  @param[in] handle
 *      Handle to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t SOC_unhaltDSS(SOC_Handle handle, int32_t* errCode)
{
    SOC_DriverMCB*      ptrSOCDriverMCB;
    int32_t             retVal = 0;
    volatile uint8_t    powerStatus;
    volatile uint8_t    unhaltStatus;

    /* Get the pointer to the SOC Driver Block: */
    ptrSOCDriverMCB = (SOC_DriverMCB*)handle;
    if (ptrSOCDriverMCB == NULL)
    {
        *errCode = SOC_EINVAL;
        retVal   = MINUS_ONE;
    }
    else
    {
        unhaltStatus = (uint8_t) CSL_FEXTR (ptrSOCDriverMCB->ptrDSSRegs->GEMPWRSMCFG4,
                                            GEMPWRSMCFG4_PWRSMLRSTHALT_BIT_END,
                                            GEMPWRSMCFG4_PWRSMLRSTHALT_BIT_START);
        if (unhaltStatus == 0x1)
        {
            volatile uint8_t stcStatus;
            volatile uint32_t *stcgstat = (volatile uint32_t *)0x50040014;

            /* check if DSS_STC is triggered by Bootloader */
            stcStatus = CSL_FEXTR (*stcgstat, 0U, 0U);
            if(stcStatus == 0x1)
            {
                /* clear 3rd bit before un-halting DSS */
                ptrSOCDriverMCB->ptrDSSRegs->STCPBISTSMCFG1 = CSL_FINSR(ptrSOCDriverMCB->ptrDSSRegs->STCPBISTSMCFG1,
                                                                        STCPBISTSMCFG1_STCPBISTLRSTDASRTHALT_BIT_END,
                                                                        STCPBISTSMCFG1_STCPBISTLRSTDASRTHALT_BIT_START,
                                                                        0x0U);
            }

            /* clear 17th bit - to un-halt DSS */
            ptrSOCDriverMCB->ptrDSSRegs->GEMPWRSMCFG4 = CSL_FINSR(ptrSOCDriverMCB->ptrDSSRegs->GEMPWRSMCFG4,
                                                                  GEMPWRSMCFG4_PWRSMLRSTHALT_BIT_END,
                                                                  GEMPWRSMCFG4_PWRSMLRSTHALT_BIT_START,
                                                                  0x0U);
            /* Wait till the DSS is powered on */
            while (1)
            {
                /* Get the power mode status: Have we transitioned? */
                powerStatus = (uint8_t)CSL_FEXTR (ptrSOCDriverMCB->ptrDSSRegs->GEMPWRSMCFG3, 19U, 18U);
                if (powerStatus == 0x3)
                {
                    /* YES: Transitioning has been done. */
                    break;
                }
            }

            /* clear 18th bit to enable monitoring event outside from DSS */
            ptrSOCDriverMCB->ptrDSSRegs->GEMPWRSMCFG4 = CSL_FINSR(ptrSOCDriverMCB->ptrDSSRegs->GEMPWRSMCFG4,
                                                                  GEMPWRSMCFG4_GEMEVENTMASK_BIT_END,
                                                                  GEMPWRSMCFG4_GEMEVENTMASK_BIT_START,
                                                                  0x0U);
        }
    }
    return retVal;
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

    /*********************************************************************
     * Looking closely at the dissassembly code there are 2 instructions
     * per loop iteration. One for the handling 'index' (1 cycle) and
     * the other is the loop terminator check (3 cycles).
     * So we need to execute the loop 1/4 times to get the correct
     * granularity in usec.
     *********************************************************************/
    for (index = 0; index < ((R4F_CLOCK_MHZ/4) * delayInMircoSecs); index++)
    {
        asm("");
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the peripheral module
 *  clock source and divisor.
 *
 *  @param[in] handle
 *      Handle to the SOC Driver
 *  @param[in] module
 *      Peripheral Module Id
 *  @param[in] clkSource
 *      Peripheral clock source
 *  @param[in] clkDivisor
 *      Peripheral clock divisor
 *  @param[out] errCode
 *      Error code populated by the function
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t SOC_setPeripheralClock
(
    SOC_Handle              handle,
    SOC_ModuleId            module,
    SOC_PeripheralClkSource clkSource,
    uint8_t                 clkDivisor,
    int32_t*                errCode
)
{
    SOC_DriverMCB*      ptrSOCDriverMCB;
    int32_t             retVal = 0;

    /* Get the pointer to the SOC Driver Block: */
    ptrSOCDriverMCB = (SOC_DriverMCB*)handle;
    if (ptrSOCDriverMCB == NULL)
    {
        *errCode = SOC_EINVAL;
        retVal = MINUS_ONE;
    }
    else
    {
        /* Clock init sequence:
         * 1. Gate clock
         * 2. set Divisor value
         * 3. set Source selection
         * 4. Ungate clcok
         */
        switch (module)
        {
            case SOC_MODULE_MCAN:
            {
                /* Gate clock */
                ptrSOCDriverMCB->ptrRCMRegs->CLKGATE = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKGATE,
                                                                  CLKGATE_FDCANCLKGATE_BIT_END,
                                                                  CLKGATE_FDCANCLKGATE_BIT_START,
                                                                  1U);

                /* Set clock divisor */
                ptrSOCDriverMCB->ptrRCMRegs->CLKDIVCTL0 = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKDIVCTL0,
                                                                     CLKDIVCTL0_FDCANCLKDIV_BIT_END,
                                                                     CLKDIVCTL0_FDCANCLKDIV_BIT_START,
                                                                     (uint32_t)clkDivisor);

                /* Set clock source */
                ptrSOCDriverMCB->ptrRCMRegs->CLKSRCSEL0 = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKSRCSEL0,
                                                                     CLKSRCSEL0_FDCANCLKSRCSEL_BIT_END,
                                                                     CLKSRCSEL0_FDCANCLKSRCSEL_BIT_START,
                                                                     (uint32_t)clkSource);

                /* Ungate clock */
                ptrSOCDriverMCB->ptrRCMRegs->CLKGATE = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKGATE,
                                                                  CLKGATE_FDCANCLKGATE_BIT_END,
                                                                  CLKGATE_FDCANCLKGATE_BIT_START,
                                                                  0U);
                break;
            }
            case SOC_MODULE_QSPI:
            {
                /* Gate clock */
                ptrSOCDriverMCB->ptrRCMRegs->CLKGATE = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKGATE,
                                                                  CLKGATE_QSPICLKGATE_BIT_END,
                                                                  CLKGATE_QSPICLKGATE_BIT_START,
                                                                  1U);

                /* Set clock divisor */
                ptrSOCDriverMCB->ptrRCMRegs->CLKDIVCTL2 = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKDIVCTL2,
                                                                     CLKDIVCTL2_QSPICLKDIV_BIT_END,
                                                                     CLKDIVCTL2_QSPICLKDIV_BIT_START,
                                                                     (uint32_t)clkDivisor);

                /* Set clock source */
                ptrSOCDriverMCB->ptrRCMRegs->CLKSRCSEL0 = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKSRCSEL0,
                                                                     CLKSRCSEL0_QSPICLKSRCSEL_BIT_END,
                                                                     CLKSRCSEL0_QSPICLKSRCSEL_BIT_START,
                                                                     (uint32_t)clkSource);

                /* Ungate clock */
                ptrSOCDriverMCB->ptrRCMRegs->CLKGATE = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->CLKGATE,
                                                                  CLKGATE_QSPICLKGATE_BIT_END,
                                                                  CLKGATE_QSPICLKGATE_BIT_START,
                                                                  0U);
                break;
            }
            default:
            {
                *errCode = SOC_EINVAL;
                retVal = MINUS_ONE;
                break;
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the Peripheral RAM
 *
 *  @param[in] handle
 *      Handle to the SOC Driver
 *  @param[in] module
 *      SOC mudule id
 *  @param[out] errCode
 *      Error code populated by the function
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t SOC_initPeripheralRam(SOC_Handle handle, SOC_ModuleId module, int32_t* errCode)
{
    SOC_DriverMCB*      ptrSOCDriverMCB;
    int32_t             retVal = 0;
    volatile uint32_t   value;
    uint8_t             memInitBit;

    /* Get the pointer to the SOC Driver Block: */
    ptrSOCDriverMCB = (SOC_DriverMCB*)handle;
    if (ptrSOCDriverMCB == NULL)
    {
        if(errCode != NULL)
        {
            *errCode = SOC_EINVAL;
        }
        retVal = MINUS_ONE;
    }
    else
    {
        /* Enable memory initialization by writing the key. */
        ptrSOCDriverMCB->ptrRCMRegs->MEMINITSTART = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->MEMINITSTART,
                                                                MEMINITSTART_MEMINITKEY_BIT_END,
                                                                MEMINITSTART_MEMINITKEY_BIT_START, 0xADU);

        switch (module)
        {
            case SOC_MODULE_DMA:
                memInitBit = MEMINITSTART_DMAMEM_BIT_START;
                break;
            case SOC_MODULE_DMA2:
                memInitBit = MEMINITSTART_DMA2MEM_BIT_START;
                break;
            default:
                if(errCode != NULL)
                {
                    *errCode = SOC_EINVAL;
                }
                retVal = MINUS_ONE;

                break;
        }

        if(retVal == 0)
        {
            /* If the memory initialization is done already , skip the step */
            value = CSL_FEXTR(ptrSOCDriverMCB->ptrRCMRegs->MEMINITDONE, memInitBit, memInitBit);
            if (value == 0U)
            {
                /* Writing a '1' to trigger the RAM initialization */
                ptrSOCDriverMCB->ptrRCMRegs->MEMINITSTART = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->MEMINITSTART, memInitBit, memInitBit, 0x1U);

                /* Wait until the memory initialization is complete. */
                do
                {
                    value = CSL_FEXTR(ptrSOCDriverMCB->ptrRCMRegs->MEMINITDONE, memInitBit, memInitBit);
                }while (value != 0x1U);
            }

            /* Disable memory initialization by resetting the key. */
            ptrSOCDriverMCB->ptrRCMRegs->MEMINITSTART = CSL_FINSR (ptrSOCDriverMCB->ptrRCMRegs->MEMINITSTART,
                                                                    MEMINITSTART_MEMINITKEY_BIT_END,
                                                                    MEMINITSTART_MEMINITKEY_BIT_START, 0x0U);
        }
        else
        {
            /* Error Condition, return error */
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the number of banks of L3 memory.
 *
 *  @param[in] ptrSOCDriverMCB
 *      Pointer to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static uint32_t SOC_getL3RAMNumBanks(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode)
{
    uint8_t     index;
    uint8_t     numBanks = 0U;
    uint8_t     bankbit;
    uint32_t    memSetting;

    /* 8 total banks, every 4bit indicate one bank mapping */
    bankbit = 28U;
    for(index = 0U; index < 8U; index++)
    {
        memSetting =  CSL_FEXTR (ptrSOCDriverMCB->ptrTopRCMRegs->DSSMEMTAB0,
                            bankbit + 4U,bankbit);

        /* Check how many banks are enabled for DSS
           The preceding zeros need to be ignored, 
           however the ending zero need to be counted
          */
        if( (memSetting != 0x0U) ||
           ((numBanks !=0U) && (memSetting == 0U)) )
        {
            numBanks++;
        }
        bankbit -= 4U;
    }
    return(numBanks);
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the number of banks of the shared TCMA.
 *
 *  @param[in] ptrSOCDriverMCB
 *      Pointer to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static uint32_t SOC_getTCMANumBanks(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode)
{
    uint8_t     index;
    uint8_t     numBanks = 0U;
    uint8_t     bankbit;
    uint32_t    memSetting;

    /* 8 total banks, every 4bit indicate one bank mapping */
    bankbit = 28U;
    for(index = 0U; index < 8U; index++)
    {
        memSetting =  CSL_FEXTR (ptrSOCDriverMCB->ptrTopRCMRegs->TCMAMEMTAB,

                            bankbit + 4U,bankbit);

        /* Check how many banks are enabled for TCMA
           The preceding zeros need to be ignored, 
           however the ending zero need to be counted
          */
        if( (memSetting != 0x0U) ||
           ((numBanks !=0U) && (memSetting == 0U)) )
        {
            numBanks++;
        }
        bankbit -= 4U;
    }
    return(numBanks);
}

/**
 *  @b Description
 *  @n
 *      The function is used to the number of banks of the shared TCMB. 
 *
 *  @param[in] ptrSOCDriverMCB
 *      Pointer to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static uint32_t SOC_getTCMBNumBanks(SOC_DriverMCB* ptrSOCDriverMCB, int32_t* errCode)
{
    uint8_t     index;
    uint8_t     numBanks = 0U;
    uint8_t     bankbit;
    uint32_t    memSetting;

    /* 8 total banks, every 4bit indicate one bank mapping */
    bankbit = 28U;
    for(index = 0U; index < 8U; index++)
    {
        memSetting =  CSL_FEXTR (ptrSOCDriverMCB->ptrTopRCMRegs->TCMBMEMTAB,
                            bankbit + 4U,bankbit);

        /* Check how many banks are enabled for TCMB
           The preceding zeros need to be ignored, 
           however the ending zero need to be counted
          */
        if( (memSetting != 0x0U) ||
           ((numBanks !=0U) && (memSetting == 0U)) )
        {
            numBanks++;
        }
        bankbit -= 4U;
    }
    return(numBanks);
}

/**
 *  @b Description
 *  @n
 *      SOC Device specific initialization which is implemented for the XWR68xx MSS
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
    uint32_t        l3RamNumBanks;
    uint32_t        tcmaNumBanks;
    uint32_t        tcmbNumBanks;

    /* SOC_L3RAM_NUM_BANK (the size from compilation) should be
       no bigger than the L3RAM size configured in hardware */
    l3RamNumBanks = SOC_getL3RAMNumBanks(ptrSOCDriverMCB, errCode);
    DebugP_assert(SOC_L3RAM_NUM_BANK <= l3RamNumBanks);

    /* SOC_XWR68XX_MSS_SHMEM_TCMA_NUM_BANK (the size from compilation) should be
       no bigger than the TCMA size configured in hardware */
    tcmaNumBanks = SOC_getTCMANumBanks(ptrSOCDriverMCB, errCode);
    DebugP_assert(SOC_XWR68XX_MSS_SHMEM_TCMA_NUM_BANK == tcmaNumBanks);

    /* SOC_XWR68XX_MSS_SHMEM_TCMB_NUM_BANK (the size from compilation) should be
       no bigger than the TCMB size configured in hardware */
    tcmbNumBanks = SOC_getTCMBNumBanks(ptrSOCDriverMCB, errCode);
    DebugP_assert(SOC_XWR68XX_MSS_SHMEM_TCMB_NUM_BANK == tcmbNumBanks);

    if (ptrSOCDriverMCB->cfg.mpuCfg == SOC_MPUCfg_CONFIG)
    {
        /* Initialize the MPU */
        SOC_mpu_config();
    }
    else
    {
        /* Bypass: MPU will be initialized by application. */
    }

    /* The DSS2 Register are valid only for the XWR16xx/XWR68xx */
    ptrSOCDriverMCB->ptrDSS2Regs = (DSS2Regs*)gSOCHwAttrib.dssReg2BaseAddress;

    if (ptrSOCDriverMCB->cfg.dssCfg == SOC_DSSCfg_UNHALT)
    {
        /* unhalt the DSS */
        SOC_unhaltDSS(ptrSOCDriverMCB, errCode);
    }
    else
    {
        /* Bypass: DSS maintains its original state. */
    }


    /***********************************************************************************
     * Frame Start System Interrupt
     ***********************************************************************************/
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "FrameStartISR";
    hwiParams.arg  = (uintptr_t)&ptrSOCDriverMCB->sysIntTable[sysIntIndex];
    ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum    = SOC_XWR68XX_MSS_FRAME_START_INT;
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
    ptrSOCDriverMCB->sysIntTable[sysIntIndex].intNum    = SOC_XWR68XX_MSS_CHIRP_AVAIL_IRQ;
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
 *      SOC Device specific deinitialization which is implemented for the XWR68xx MSS
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
 *      This function is used to check if the device is a secure device
 *
 *  @param[in] handle
 *      Handle to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Secure          -   1
 *  @retval
 *      Non_Secure      -   0
 *  @retval
 *      Error           -   <0
 */
int32_t SOC_isSecureDevice(SOC_Handle handle, int32_t* errCode)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This function is used to enable/disable secure firewall for specified modules 
 *      This function should be called only if the device is a secure device
 *
 *  @param[in] handle
 *      Handle to the SOC Driver
 *  @param[in] firewallModulesBitmap
 *      Bitmap of modules (specified by SOC_SecureFirewallModules) for which firewall needs to be modified
 *  @param[in] control
 *      SOC_SECURE_FIREWALL_DISABLE - Firewall disable (enables access to modules)
 *      SOC_SECURE_FIREWALL_ENABLE  - Firewall enable  (disables access to modules)
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
int32_t SOC_controlSecureFirewall(SOC_Handle handle, uint32_t firewallModulesBitmap, uint8_t control, int32_t* errCode)
{
    SOC_DriverMCB*  ptrSOCDriverMCB;
    uint32_t        secureCfgReg1Val;
    uint32_t        secureCfgReg2Val;
    uint32_t        secureCfgReg1ValNew;
    uint32_t        secureCfgReg2ValNew;
    uint32_t        val = 0;  /* default is enable firewall */
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
        /* The firewall enable/disable is most probably debug activity/one time configuration done by single module
         * and hence protecting this with critical section is not deemed necessary
         */

        /* Get the current value for the appropriate secureCfgRegs that have firewall bits */
        secureCfgReg1Val = ptrSOCDriverMCB->ptrTopRCMRegs->SECURECFGREG1;
        secureCfgReg2Val = ptrSOCDriverMCB->ptrTopRCMRegs->SECURECFGREG2;
        secureCfgReg1ValNew = secureCfgReg1Val;
        secureCfgReg2ValNew = secureCfgReg2Val;

        /* Disable firewall */
        if (control == SOC_SECURE_FIREWALL_DISABLE)
        {
            /* Firewall is disabled by writing 0x7 into appropriate bits for each module
             * Default val of 0 means firewall is enabled
             */
            val = 0x7U;
        }

        /* Check the modules bitmap and enable/disable firewall as needed */
        if (firewallModulesBitmap & SOC_SECURE_FIREWALL_JTAG)
        {
            secureCfgReg1ValNew = CSL_FINSR(secureCfgReg1ValNew,
                                            SECURECFGREG1_JTAGFIREWALLEN_BIT_END,
                                            SECURECFGREG1_JTAGFIREWALLEN_BIT_START,
                                            val);
        }
        if (firewallModulesBitmap & SOC_SECURE_FIREWALL_SECURERAM)
        {
            secureCfgReg1ValNew = CSL_FINSR(secureCfgReg1ValNew,
                                            SECURECFGREG1_SECURERAMFIREWALLEN_BIT_END,
                                            SECURECFGREG1_SECURERAMFIREWALLEN_BIT_START,
                                            val);
        }
        if (firewallModulesBitmap & SOC_SECURE_FIREWALL_LOGGER)
        {
            secureCfgReg1ValNew = CSL_FINSR(secureCfgReg1ValNew,
                                            SECURECFGREG1_LOGGERFIREWALLEN_BIT_END,
                                            SECURECFGREG1_LOGGERFIREWALLEN_BIT_START,
                                            val);
        }
        if (firewallModulesBitmap & SOC_SECURE_FIREWALL_TRACE)
        {
            secureCfgReg1ValNew = CSL_FINSR(secureCfgReg1ValNew,
                                            SECURECFGREG1_TRACEFIREWALLEN_BIT_END,
                                            SECURECFGREG1_TRACEFIREWALLEN_BIT_START,
                                            val);
        }
        if (firewallModulesBitmap & SOC_SECURE_FIREWALL_CRYPTO)
        {
            secureCfgReg1ValNew = CSL_FINSR(secureCfgReg1ValNew,
                                            SECURECFGREG1_CRYPTOFIREWALLEN_BIT_END,
                                            SECURECFGREG1_CRYPTOFIREWALLEN_BIT_START,
                                            val);
        }
        if (firewallModulesBitmap & SOC_SECURE_FIREWALL_CUSTCEK)
        {
            secureCfgReg1ValNew = CSL_FINSR(secureCfgReg1ValNew,
                                            SECURECFGREG1_CUSTCEKFIREWALLEN_BIT_END,
                                            SECURECFGREG1_CUSTCEKFIREWALLEN_BIT_START,
                                            val);
        }
        if (firewallModulesBitmap & SOC_SECURE_FIREWALL_DMM)
        {
            secureCfgReg2ValNew = CSL_FINSR(secureCfgReg2ValNew,
                                            SECURECFGREG2_DMMFIREWALLEN_BIT_END,
                                            SECURECFGREG2_DMMFIREWALLEN_BIT_START,
                                            val);
        }

        /* Check if the value has changed */
        if (secureCfgReg1ValNew != secureCfgReg1Val)
        {
            /* write new value */
            ptrSOCDriverMCB->ptrTopRCMRegs->SECURECFGREG1 = secureCfgReg1ValNew;
        }
        /* Check if the value has changed */
        if (secureCfgReg2ValNew != secureCfgReg2Val)
        {
            /* write new value */
            ptrSOCDriverMCB->ptrTopRCMRegs->SECURECFGREG2 = secureCfgReg2ValNew;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get the MSS VCLK clock frequency.
 *
 *  @param[in] handle
 *      Handle to the SOC Driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup SOC_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - MSS Clock frequency in MHz.
 *  @retval
 *      Error       - 0
 */
uint32_t SOC_getMSSVCLKFrequency(SOC_Handle handle, int32_t* errCode)
{
    SOC_DriverMCB*      ptrSOCDriverMCB;
    volatile uint32_t   clkFrequency = 0;

    ptrSOCDriverMCB = (SOC_DriverMCB*)handle;
    if (ptrSOCDriverMCB == NULL)
    {
        *errCode = SOC_EINVAL;
    }
    else
    {
        /* Read the SPARE2 register to get the APLL open loop clock frequency.
         * Convert to MSS VCLK MHz */
         clkFrequency = (((CSL_FEXTR (ptrSOCDriverMCB->ptrTopRCMRegs->SPARE2, 15U, 0U)) / 30U) * 1000000U);
    }
    return clkFrequency;
}
