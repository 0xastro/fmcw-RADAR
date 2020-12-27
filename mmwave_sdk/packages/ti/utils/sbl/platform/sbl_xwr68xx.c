/*
 *   @file  sbl_xwr68xx.c
 *
 *   @brief
 *      XWR68xx SBL platform specific configuration.
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

/* MMWSDK include files */
#include <ti/common/sys_common.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/soc/soc.h>

/* SBL internal include file. */
#include <ti/utils/sbl/include/sbl_internal.h>

/**************************************************************************
 *************************** Local defines ********************************
 **************************************************************************/
/* Define for 1KB */
#define ONE_KB                              (1024U)

/* Define for 1MB */
#define ONE_MB                              (ONE_KB * ONE_KB)

/* The following defines are used to verify the start/end addresses of various sections.
 * Note: The addresses defined here for different section are as seen by MSS
 */
#define SBL_MSS_SHARED_MEM_OFFSET           (0x00000000U)

/* MSS offset value required for correct addressing as seen by the MSS */
#define SBL_MSS_SHARED_MEM_TCMB_OFFSET      (0x00000000U)

/* BSS offset value required for correct addressing as seen by the MSS */
#define SBL_BSS_SHARED_MEM_OFFSET           (0x40000000U)

/* BSS TCMB offset value required for correct addressing as seen by the MSS */
#define SBL_BSS_SHARED_MEM_TCMB_OFFSET      (0x39000000U)

/* DSS offset value required for correct addressing as seen by the MSS */
#define SBL_DSS_L2_SHARED_MEM_OFFSET        (0x57000000U)

/* DSS offset value required for correct addressing as seen by the MSS */
#define SBL_DSS_L3_SHARED_MEM_OFFSET        (0x31000000U)

/* MSS TCMA extended section size */
#define SBL_MSS_EXT_TCMA_SECTION_SIZE       (0x00180000U)

/* MSS TCMB section start address */
#define SBL_MSS_TCMB_SECTION_START_ADDRESS  (0x08030000U)

/* MSS TCMB section end address */
#define SBL_MSS_TCMB_SECTION_END_ADDRESS    (0x080B0000U)

/* BSS TCMB section start address */
#define SBL_BSS_TCMB_SECTION_START_ADDRESS  (0x08000000U)

/* BSS TCMB section end address */
#define SBL_BSS_TCMB_SECTION_END_ADDRESS    (0x08010000U)

/* BSS section end address */
#define SBL_BSS_SECTION_END_ADDRESS         (0x00200000U)

/* DSS L2 section start address */
#define SBL_DSS_L2_SECTION_START_ADDRESS    (0x007E0000U)

/* DSS L2 section end address */
#define SBL_DSS_L2_SECTION_END_ADDRESS      (0x00E00000U)

/* DSS L3 section start address */
#define SBL_DSS_L3_SECTION_START_ADDRESS    (0x20000000U)

/* DSS L3 section end address */
#define SBL_DSS_L3_SECTION_END_ADDRESS      (0x20200000U)

/**
 *  @b Description
 *  @n
 *      Platform specific intializations.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void SBL_platformInit(void)
{
    /* Setup the PINMUX to bring out the XWR68xx UART pins */
   	Pinmux_Set_FuncSel(SOC_XWR68XX_PINN5_PADBE, SOC_XWR68XX_PINN5_PADBE_MSS_UARTA_TX);
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINN5_PADBE, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINN4_PADBD, SOC_XWR68XX_PINN4_PADBD_MSS_UARTA_RX);
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINN4_PADBD, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);

    /* Setup the PINMUX to bring out the XWR68xx QSPI pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINR12_PADAP, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINR12_PADAP, SOC_XWR68XX_PINR12_PADAP_QSPI_CLK);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINP11_PADAQ, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINP11_PADAQ, SOC_XWR68XX_PINP11_PADAQ_QSPI_CSN);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINR13_PADAL, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINR13_PADAL, SOC_XWR68XX_PINR13_PADAL_QSPI_D0);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINN12_PADAM, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINN12_PADAM, SOC_XWR68XX_PINN12_PADAM_QSPI_D1);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINR14_PADAN, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINR14_PADAN, SOC_XWR68XX_PINR14_PADAN_QSPI_D2);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINP12_PADAO, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINP12_PADAO, SOC_XWR68XX_PINP12_PADAO_QSPI_D3);
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
 * -# This code comes up after the C initialization (from _c_int00 to main).
 *    Presently, the application comes up in privilege mode, not user mode. So the
 *    access permissions are set accordingly.
 * -# There is no cache on the R4F on the AR parts, so regions that would otherwise
 *     make sense to be cacheable are made non-cache.
 * -# Peripheral space is strongly ordered instead of device because writes need
 *    to go through when they are issued and not at a later (uncertain time when next
 *    read forces the writes sitting in write buffer to be flushed out,
 *    reference: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0363g/Chdcahcf.html).
 *    An alternative way is to use the Device type instead of strongly orderered but this
 *    requires all driver code to be scrutinized for APIs that do a write at the end
 *    and for such APIs a "dsb" instruction needs to be issued. This is prone to errors
 *    during maintenance work so it is safer to choose strongly-ordered. Presently,
 *    the architecture is more memory size constrained than cycles (MIPS) constrained
 *    so safety benefits at potentially increased cycles are a good trade-off.
 * -# L3 Memory has been set as normal. This means <B> for any transfer of ownership from
 *    R4F to non-R4F (peripherals, HW acc), a "dsb" instruction should be issued after the
 *    last write and before notifying/triggering the non-R4F entity.</B>
 * -# Mailbox Memory has been set as normal. This means <B> for any transfer of ownership from
 *    R4F to non-R4F (BSS for example), a "dsb" instruction (MEM_BARRIER()) should be issued after the last
 *    write to the mailbox message memory before notifying/triggering the non-R4F entity.</B>
 * -# BSS Memory will be configured when the BSS sections are updated. In the default MPU configuration,
 *    these sections are marked as reserved.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void SBL_mpuConfigDefault(void)
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
        Access Permissions:  Full Access
        Execute Permissions: Execute,
        Memory Type:         Non-Cached, Non-Shared, Normal
    */
#if ((SOC_XWR68XX_MSS_TCMA_BASE_ADDRESS & (512U * ONE_KB - 1)) != 0)
#error SOC_XWR68XX_MSS_TCMA_BASE_ADDRESS not aligned to 512 KB
#endif
    SOC_MPUSetRegion(SOC_MPU_REGION2);
    SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_TCMA_BASE_ADDRESS);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_EXEC);
    SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_512_KB);

    /*
        Setup region 3, TCMB DRAM, base address = MSS_TCMB_BASE_ADDRESS, size = 256 KB (actual 192 KB),
        Access Permissions:  Full Access
        Execute Permissions: Execute
        Memory Type:         Non-Cached, Non-Shared, Normal
    */
#if ((SOC_XWR68XX_MSS_TCMB_BASE_ADDRESS & (256U * ONE_KB - 1)) != 0)
#error SOC_XWR68XX_MSS_TCMB_BASE_ADDRESS not aligned to 256 KB
#endif
    SOC_MPUSetRegion(SOC_MPU_REGION3);
    SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_TCMB_BASE_ADDRESS);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_EXEC);
    SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_256_KB);


    /*
        Setup region 4, [Ext Flash-QSPI], base address = EXT_FLASH_BASE_ADDRESS, size = 8 MB,
        Access Permissions:  Full Access
        Execute Permissions: Execute Never
        Memory Type:         Non-Cached, Non-Shared, Normal
    */
#if ((SOC_XWR68XX_MSS_EXT_FLASH_BASE_ADDRESS & (8U * ONE_MB - 1)) != 0)
#error SOC_XWR68XX_MSS_EXT_FLASH_BASE_ADDRESS not aligned to 8 MB
#endif
    SOC_MPUSetRegion(SOC_MPU_REGION4);
    SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_EXT_FLASH_BASE_ADDRESS);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
    SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_8_MB);

    /*
        Setup region 5, [QSPI Reg], base address = MSS_QSPI_BASE_ADDRESS, size = 128 Bytes,
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
        NOTE: The choice is because of MPU number of regions and size being power of 2 limitation.
            Even though there is no physical space from 0xF000_0000 to
            SOC_XWR68XX_MSS_MBOX_BSS_MSS_REG_BASE_ADDRESS (0xF060_8600), we program from
            0xF000_0000 to cover all the way to the last address at 0xFFFFFFFF as register space.
    */
    SOC_MPUSetRegion(SOC_MPU_REGION6);
    SOC_MPUSetRegionBaseAddress(0xF0000000U);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_STRONGLYORDERED_SHAREABLE, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
    SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_256_MB);

    /* Memory regions 7-9 are reserved for BSS MPU configuration.
     * They will be configured when the BSS sections are updated.
     */

    /*
        Setup region 10, L2 memory, base address = 0x57000000, size = 16MB
        Access Permissions:  Full access
        Execute Permissions: Execute Never
        Memory Type:         Non-Cached, Non-Shared, Normal
    */
    SOC_MPUSetRegion(SOC_MPU_REGION10);
    SOC_MPUSetRegionBaseAddress(SBL_DSS_L2_SHARED_MEM_OFFSET);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
    SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_16_MB);

    /*
        Setup region 11, L3 memory, base address = 0x51000000, size = 2 MB,
        Access Permissions:  Full Access
        Execute Permissions: Execute Never
        Memory Type:         Non-Cached, Non-Shared, Normal
    */
    SOC_MPUSetRegion(SOC_MPU_REGION11);
    SOC_MPUSetRegionBaseAddress(SOC_XWR68XX_MSS_L3RAM_BASE_ADDRESS);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
    SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_2_MB);

    SOC_MPUEnableBackgroundRegion();
    SOC_MPUEnable();
}

/*!
 *  @b Description
 *  @n
 *      This function configures the MPU for BSS regions.
 *
 *  @param[in]  enable
 *      Flag to enable or disable the MPU settings for BSS regions.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable.
 */
void SBL_mpuConfigBSS(bool enable)
{
    /*
       Setup region 7, BSS-TCMA RAM, base address = 0x40000000, size = 256KBytes,
       Access Permissions:  Full Access
       Execute Permissions: Execute Never
       Memory Type:         Non-Cached, Non-Shared, Normal
    */
    SOC_MPUSetRegion(SOC_MPU_REGION7);
    SOC_MPUSetRegionBaseAddress(0x40000000);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);

    if (enable == true)
    {
        SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_256_KB);
    }
    else
    {
        SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_DISABLE);
    }

    /*
       Setup region 8, BSS-TCMA RAM, base address = 0x40080000, size = 32KBytes,
       Access Permissions:  Full Access
       Execute Permissions: Execute Never
       Memory Type:         Non-Cached, Non-Shared, Normal
    */
    SOC_MPUSetRegion(SOC_MPU_REGION8);
    SOC_MPUSetRegionBaseAddress(0x40080000);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);

    if (enable == true)
    {
        SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_32_KB);
    }
    else
    {
        SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_DISABLE);
    }

    /*
       Setup region 9, BSS-TCMB RAM, base address = 0x41000000, size = 64KBytes,
       Access Permissions:  Full Access
       Execute Permissions: Execute Never
       Memory Type:         Non-Cached, Non-Shared, Normal
    */
    SOC_MPUSetRegion(SOC_MPU_REGION9);
    SOC_MPUSetRegionBaseAddress(0x41000000);
    SOC_MPUSetRegionTypeAndPermission(SOC_MPU_NORMAL_OINC_NONSHARED, SOC_MPU_PRIV_RW_USER_RO_NOEXEC);
    if (enable == true)
    {
        SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_ENABLE | SOC_MPU_64_KB);
    }
    else
    {
        SOC_MPUSetRegionSizeRegister(SOC_MPU_REGION_DISABLE);
    }

    if (enable == true)
    {
        SOC_MPUEnable();
    }
    else
    {
        SOC_MPUDisable();
    }
}

/*!
 *  @b Description
 *  @n
 *      This function calculates the RAM file offset.
 *
 *  @param[in]  sectionPtr
 *      Section start address.
 *  @param[in]  sectionLen
 *      Section Length.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      RAM Offset
 */
uint32_t SBL_calculateImageOffset(uint32_t sectionPtr, uint32_t sectionLen)
{
    uint32_t        filetype;
    uint32_t        offset;

    filetype = gSblMCB.imageParams.fileType;

    /* Calculate the shared mem offset to append to the section start addr
      This is required as the RPRC images are converted from the .out files */
    if ((((filetype & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT) == SBL_MSS_SUBSYSTEM) ||
        (((filetype & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT) == SBL_MSS_SUBSYSTEM_DEBUG))
    {
        /* Support for load to TCMB (512KB ext). */
        if ((sectionPtr >= SBL_MSS_TCMB_SECTION_START_ADDRESS) &&
            ((sectionPtr + sectionLen) <= SBL_MSS_TCMB_SECTION_END_ADDRESS))
        {
            offset = (uint32_t)SBL_MSS_SHARED_MEM_TCMB_OFFSET;
        }
        else if ((sectionPtr < SBL_BSS_TCMB_SECTION_START_ADDRESS) &&
                 ((sectionPtr + sectionLen) <= SBL_MSS_EXT_TCMA_SECTION_SIZE))
        {
            offset = (uint32_t)SBL_MSS_SHARED_MEM_OFFSET;
        }
        else
        {
            offset = 0U;
            gSblMCB.errorStatus |= SBL_RPRC_PARSER_MSS_FILE_OFFSET_MISMATCH;
        }
    }
    else if (((filetype & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT) == SBL_BSS_SUBSYSTEM)
    {
        /* Support for load of tables into TCMB too. */
        if ((sectionPtr >= SBL_BSS_TCMB_SECTION_START_ADDRESS) &&
            ((sectionPtr + sectionLen) <= SBL_BSS_TCMB_SECTION_END_ADDRESS))
        {
            offset = (uint32_t)SBL_BSS_SHARED_MEM_TCMB_OFFSET;
        }
        else if ((sectionPtr + sectionLen) <= SBL_BSS_SECTION_END_ADDRESS)
        {
            offset = (uint32_t)SBL_BSS_SHARED_MEM_OFFSET;
        }
        else
        {
            offset = 0U;
            gSblMCB.errorStatus |= SBL_RPRC_PARSER_BSS_FILE_OFFSET_MISMATCH;
        }

        /* Configure the MPU settings for BSS section */
        if (gSblMCB.bssMpuInit == 0)
        {
            gSblMCB.bssMpuInit = 1U;

            /* Enable the regions */
            SBL_mpuConfigBSS(true);
        }
    }
    else if (((filetype & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT) == SBL_DSS_SUBSYSTEM)
    {
        /* Load to L2 memory for DSS */
        if ((sectionPtr >= SBL_DSS_L2_SECTION_START_ADDRESS) &&
            ((sectionPtr + sectionLen) <= SBL_DSS_L2_SECTION_END_ADDRESS))
        {
            offset = (uint32_t)SBL_DSS_L2_SHARED_MEM_OFFSET;
        }

        /* Load to L3 memory for DSS */
        else if ((sectionPtr >= SBL_DSS_L3_SECTION_START_ADDRESS) &&
                ((sectionPtr + sectionLen) <= SBL_DSS_L3_SECTION_END_ADDRESS))
        {
            offset = (uint32_t)SBL_DSS_L3_SHARED_MEM_OFFSET;
        }
        else
        {
            offset = 0U;
            gSblMCB.errorStatus |= SBL_RPRC_PARSER_DSS_FILE_OFFSET_MISMATCH;
        }
    }
    else if ((((filetype & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT) == SBL_CONFIG_SUBSYSTEM))
    {
        if ((sectionPtr == 0U) && (sectionLen <= SBL_CONFIG_FILE_SIZE))
        {
            offset = (uint32_t)&gSblMCB.configFile[0U];
        }
        else
        {
            offset = 0U;
            gSblMCB.errorStatus |= SBL_RPRC_PARSER_CONFIG_FILE_OFFSET_MISMATCH;
        }
    }
    else
    {
        /* Not valid */
        offset = 0U;
        gSblMCB.errorStatus |= SBL_CERT_FIELD_INVALID_SUBSYSTEM;
    }

    return(offset);
}

