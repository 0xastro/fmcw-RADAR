/**
 *  @file    reg_toprcm_xwr14xx.h
 *
 *  @brief
 *    This file gives register definitions of MSS_TOPRCM module.
 *
 *  This file is auto-generated on 3/27/2017.
 *
 */

 /*
 *   (C) Copyright 2016, Texas Instruments Incorporated. - TI web address www.ti.com
 *---------------------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef REG_TOPRCM_H
#define REG_TOPRCM_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field DSSCLKDIV in Register CLKDIV */
#define CLKDIV_DSSCLKDIV_BIT_START                          0U
#define CLKDIV_DSSCLKDIV_BIT_END                            3U

/* Definition for field DSSCLKSRCSEL in Register CLKSRCSEL */
#define CLKSRCSEL_DSSCLKSRCSEL_BIT_START                    0U
#define CLKSRCSEL_DSSCLKSRCSEL_BIT_END                      3U

/* Definition for field BSSCPUHALT in Register BSSCTL */
#define BSSCTL_BSSCPUHALT_BIT_START                         24U
#define BSSCTL_BSSCPUHALT_BIT_END                           31U

/* Definition for field EXTCLK1DIV in Register EXTCLKDIV */
#define EXTCLKDIV_EXTCLK1DIV_BIT_START                      0U
#define EXTCLKDIV_EXTCLK1DIV_BIT_END                        7U

/* Definition for field EXTCLK2DIV in Register EXTCLKDIV */
#define EXTCLKDIV_EXTCLK2DIV_BIT_START                      8U
#define EXTCLKDIV_EXTCLK2DIV_BIT_END                        15U

/* Definition for field EXTCLK1SRCSEL in Register EXTCLKSRCSEL */
#define EXTCLKSRCSEL_EXTCLK1SRCSEL_BIT_START                0U
#define EXTCLKSRCSEL_EXTCLK1SRCSEL_BIT_END                  3U

/* Definition for field EXTCLK2SRCSEL in Register EXTCLKSRCSEL */
#define EXTCLKSRCSEL_EXTCLK2SRCSEL_BIT_START                8U
#define EXTCLKSRCSEL_EXTCLK2SRCSEL_BIT_END                  11U

/* Definition for field EXTCLK1GATE in Register EXTCLKCTL */
#define EXTCLKCTL_EXTCLK1GATE_BIT_START                     0U
#define EXTCLKCTL_EXTCLK1GATE_BIT_END                       7U

/* Definition for field EXTCLK2GATE in Register EXTCLKCTL */
#define EXTCLKCTL_EXTCLK2GATE_BIT_START                     8U
#define EXTCLKCTL_EXTCLK2GATE_BIT_END                       15U

/* Definition for field SOFTSYSRST in Register SOFTSYSRST */
#define SOFTSYSRST_SOFTSYSRST_BIT_START                     0U
#define SOFTSYSRST_SOFTSYSRST_BIT_END                       7U

/* Definition for field WDRSTEN in Register WDRSTEN */
#define WDRSTEN_WDRSTEN_BIT_START                           0U
#define WDRSTEN_WDRSTEN_BIT_END                             7U

/* Definition for field SYSRSTCAUSE in Register SYSRSTCAUSE */
#define SYSRSTCAUSE_SYSRSTCAUSE_BIT_START                   0U
#define SYSRSTCAUSE_SYSRSTCAUSE_BIT_END                     3U

/* Definition for field SYSRSTCAUSECLR in Register SYSRSTCAUSECLR */
#define SYSRSTCAUSECLR_SYSRSTCAUSECLR_BIT_START             0U
#define SYSRSTCAUSECLR_SYSRSTCAUSECLR_BIT_END               7U

/* Definition for field MISCCAPT in Register MISCCAPT */
#define MISCCAPT_MISCCAPT_BIT_START                         0U
#define MISCCAPT_MISCCAPT_BIT_END                           31U

/* Definition for field DCDCCTL0 in Register DCDCCTL0 */
#define DCDCCTL0_DCDCCTL0_BIT_START                         0U
#define DCDCCTL0_DCDCCTL0_BIT_END                           31U

/* Definition for field DCDCRST in Register DCDCCTL1 */
#define DCDCCTL1_DCDCRST_BIT_START                          0U
#define DCDCCTL1_DCDCRST_BIT_END                            0U

/* Definition for field DCDCLKEN in Register DCDCCTL1 */
#define DCDCCTL1_DCDCLKEN_BIT_START                         1U
#define DCDCCTL1_DCDCLKEN_BIT_END                           1U

/* Definition for field DCDCCTL1 in Register DCDCCTL1 */
#define DCDCCTL1_DCDCCTL1_BIT_START                         2U
#define DCDCCTL1_DCDCCTL1_BIT_END                           31U

/* Definition for field USERMODEEN in Register USERMODEEN */
#define USERMODEEN_USERMODEEN_BIT_START                     0U
#define USERMODEEN_USERMODEEN_BIT_END                       31U

/* Definition for field LVDSPADCTL0 in Register LVDSPADCTL0 */
#define LVDSPADCTL0_LVDSPADCTL0_BIT_START                   0U
#define LVDSPADCTL0_LVDSPADCTL0_BIT_END                     31U

/* Definition for field LVDSPADCTL1 in Register LVDSPADCTL1 */
#define LVDSPADCTL1_LVDSPADCTL1_BIT_START                   0U
#define LVDSPADCTL1_LVDSPADCTL1_BIT_END                     31U

/* Definition for field DFTREG0 in Register DFTREG0 */
#define DFTREG0_DFTREG0_BIT_START                           0U
#define DFTREG0_DFTREG0_BIT_END                             31U

/* Definition for field DFTREG1 in Register DFTREG1 */
#define DFTREG1_DFTREG1_BIT_START                           0U
#define DFTREG1_DFTREG1_BIT_END                             31U

/* Definition for field DFTREG2 in Register DFTREG2 */
#define DFTREG2_DFTREG2_BIT_START                           0U
#define DFTREG2_DFTREG2_BIT_END                             31U

/* Definition for field DFTREG3 in Register DFTREG3 */
#define DFTREG3_DFTREG3_BIT_START                           0U
#define DFTREG3_DFTREG3_BIT_END                             31U

/* Definition for field DFTREG4 in Register DFTREG4 */
#define DFTREG4_DFTREG4_BIT_START                           0U
#define DFTREG4_DFTREG4_BIT_END                             31U

/* Definition for field DFTREG5 in Register DFTREG5 */
#define DFTREG5_DFTREG5_BIT_START                           0U
#define DFTREG5_DFTREG5_BIT_END                             31U

/* Definition for field INITSTARTADDR in Register SHMEMINITADDR */
#define SHMEMINITADDR_INITSTARTADDR_BIT_START               0U
#define SHMEMINITADDR_INITSTARTADDR_BIT_END                 15U

/* Definition for field INITENDADDR in Register SHMEMINITADDR */
#define SHMEMINITADDR_INITENDADDR_BIT_START                 16U
#define SHMEMINITADDR_INITENDADDR_BIT_END                   31U

/* Definition for field INITECCVAL in Register SHMEMINITECC */
#define SHMEMINITECC_INITECCVAL_BIT_START                   0U
#define SHMEMINITECC_INITECCVAL_BIT_END                     7U

/* Definition for field DSSBANKEN in Register DSSMEMBANKEN */
#define DSSMEMBANKEN_DSSBANKEN_BIT_START                    0U
#define DSSMEMBANKEN_DSSBANKEN_BIT_END                      15U

/* Definition for field DSSMEMTAB0 in Register DSSMEMTAB0 */
#define DSSMEMTAB0_DSSMEMTAB0_BIT_START                     0U
#define DSSMEMTAB0_DSSMEMTAB0_BIT_END                       31U

/* Definition for field DSSMEMTAB1 in Register DSSMEMTAB1 */
#define DSSMEMTAB1_DSSMEMTAB1_BIT_START                     0U
#define DSSMEMTAB1_DSSMEMTAB1_BIT_END                       31U

/* Definition for field TCMABANKEN in Register TCMAMEMBANK_EN */
#define TCMAMEMBANK_EN_TCMABANKEN_BIT_START                 0U
#define TCMAMEMBANK_EN_TCMABANKEN_BIT_END                   15U

/* Definition for field TCMAMEMTAB0 in Register TCMAMEMTAB0 */
#define TCMAMEMTAB0_TCMAMEMTAB0_BIT_START                   0U
#define TCMAMEMTAB0_TCMAMEMTAB0_BIT_END                     31U

/* Definition for field TCMAMEMTAB1 in Register TCMAMEMTAB1 */
#define TCMAMEMTAB1_TCMAMEMTAB1_BIT_START                   0U
#define TCMAMEMTAB1_TCMAMEMTAB1_BIT_END                     31U

/* Definition for field TCMBBANKEN in Register TCMBMEMBANKEN */
#define TCMBMEMBANKEN_TCMBBANKEN_BIT_START                  0U
#define TCMBMEMBANKEN_TCMBBANKEN_BIT_END                    15U

/* Definition for field TCMBMEMTAB0 in Register TCMBMEMTAB0 */
#define TCMBMEMTAB0_TCMBMEMTAB0_BIT_START                   0U
#define TCMBMEMTAB0_TCMBMEMTAB0_BIT_END                     31U

/* Definition for field TCMBMEMTAB1 in Register TCMBMEM_TAB1 */
#define TCMBMEM_TAB1_TCMBMEMTAB1_BIT_START                  0U
#define TCMBMEM_TAB1_TCMBMEMTAB1_BIT_END                    31U

/* Definition for field LSPRSTVAL in Register DSSIOCTL */
#define DSSIOCTL_LSPRSTVAL_BIT_START                        0U
#define DSSIOCTL_LSPRSTVAL_BIT_END                          21U

/* Definition for field EMCCFGCLK in Register DSSIOCTL */
#define DSSIOCTL_EMCCFGCLK_BIT_START                        24U
#define DSSIOCTL_EMCCFGCLK_BIT_END                          25U

/* Definition for field EMCDMACLK in Register DSSIOCTL */
#define DSSIOCTL_EMCDMACLK_BIT_START                        26U
#define DSSIOCTL_EMCDMACLK_BIT_END                          27U

/* Definition for field PCMDEFCACHEMOD in Register DSSIOCTL */
#define DSSIOCTL_PCMDEFCACHEMOD_BIT_START                   28U
#define DSSIOCTL_PCMDEFCACHEMOD_BIT_END                     28U

/* Definition for field SHMEMSTART in Register MEMINITSTART */
#define MEMINITSTART_SHMEMSTART_BIT_START                   16U
#define MEMINITSTART_SHMEMSTART_BIT_END                     16U

/* Definition for field SHMEM in Register MEMINITDONE */
#define MEMINITDONE_SHMEM_BIT_START                         16U
#define MEMINITDONE_SHMEM_BIT_END                           16U

/* Definition for field SPARE0 in Register SPARE0 */
#define SPARE0_SPARE0_BIT_START                             0U
#define SPARE0_SPARE0_BIT_END                               31U

/* Definition for field SPARE1 in Register SPARE1 */
#define SPARE1_SPARE1_BIT_START                             0U
#define SPARE1_SPARE1_BIT_END                               31U

/* Definition for field SPARE2 in Register SPARE2 */
#define SPARE2_SPARE2_BIT_START                             0U
#define SPARE2_SPARE2_BIT_END                               31U

/* Definition for field SPARE3 in Register SPARE3 */
#define SPARE3_SPARE3_BIT_START                             0U
#define SPARE3_SPARE3_BIT_END                               31U

/* Definition for field MSS_SIGNATURE in Register MSS_SIGNATURE */
#define MSS_SIGNATURE_MSS_SIGNATURE_BIT_START               0U
#define MSS_SIGNATURE_MSS_SIGNATURE_BIT_END                 31U

#if 0
    /* EFUSEREGROW10 was filter in auto-gen register file */
#else
/* Definition for field PART_NUMBER in Register EFUSEREGROW10 */
#define EFUSEREGROW10_PART_NUMBER_BIT_START                 18U
#define EFUSEREGROW10_PART_NUMBER_BIT_END                   25U
#endif

/* Definition for field USERMODEEN2 in Register USERMODEEN2 */
#define USERMODEEN2_USERMODEEN2_BIT_START                   0U
#define USERMODEEN2_USERMODEEN2_BIT_END                     31U

/* Definition for field SYSTICK in Register SYSTICK */
#define SYSTICK_SYSTICK_BIT_START                           0U
#define SYSTICK_SYSTICK_BIT_END                             31U

/* Definition for field SPAREMULTIBIT0 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT0_BIT_START              0U
#define SPAREMULTIBIT_SPAREMULTIBIT0_BIT_END                0U

/* Definition for field SPAREMULTIBIT1 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT1_BIT_START              1U
#define SPAREMULTIBIT_SPAREMULTIBIT1_BIT_END                1U

/* Definition for field SPAREMULTIBIT2 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT2_BIT_START              2U
#define SPAREMULTIBIT_SPAREMULTIBIT2_BIT_END                2U

/* Definition for field SPAREMULTIBIT3 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT3_BIT_START              3U
#define SPAREMULTIBIT_SPAREMULTIBIT3_BIT_END                3U

/* Definition for field SPAREMULTIBIT4 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT4_BIT_START              4U
#define SPAREMULTIBIT_SPAREMULTIBIT4_BIT_END                4U

/* Definition for field SPAREMULTIBIT5 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT5_BIT_START              5U
#define SPAREMULTIBIT_SPAREMULTIBIT5_BIT_END                5U

/* Definition for field SPAREMULTIBIT6 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT6_BIT_START              6U
#define SPAREMULTIBIT_SPAREMULTIBIT6_BIT_END                6U

/* Definition for field SPAREMULTIBIT7 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT7_BIT_START              7U
#define SPAREMULTIBIT_SPAREMULTIBIT7_BIT_END                7U

/* Definition for field SPAREMULTIBIT8 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT8_BIT_START              8U
#define SPAREMULTIBIT_SPAREMULTIBIT8_BIT_END                8U

/* Definition for field SPAREMULTIBIT9 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT9_BIT_START              9U
#define SPAREMULTIBIT_SPAREMULTIBIT9_BIT_END                9U

/* Definition for field SPAREMULTIBIT10 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT10_BIT_START             10U
#define SPAREMULTIBIT_SPAREMULTIBIT10_BIT_END               10U

/* Definition for field SPAREMULTIBIT11 in Register SPAREMULTIBIT */
#define SPAREMULTIBIT_SPAREMULTIBIT11_BIT_START             11U
#define SPAREMULTIBIT_SPAREMULTIBIT11_BIT_END               11U

/* Definition for field SPARE4 in Register SPARE4 */
#define SPARE4_SPARE4_BIT_START                             0U
#define SPARE4_SPARE4_BIT_END                               31U

/* Definition for field SPARE5 in Register SPARE5 */
#define SPARE5_SPARE5_BIT_START                             0U
#define SPARE5_SPARE5_BIT_END                               31U

/* Definition for field SPARE6 in Register SPARE6 */
#define SPARE6_SPARE6_BIT_START                             0U
#define SPARE6_SPARE6_BIT_END                               31U

/* Definition for field SPARE7 in Register SPARE7 */
#define SPARE7_SPARE7_BIT_START                             0U
#define SPARE7_SPARE7_BIT_END                               31U

/* Definition for field SPARE8 in Register SPARE8 */
#define SPARE8_SPARE8_BIT_START                             0U
#define SPARE8_SPARE8_BIT_END                               31U

/* Definition for field SPARE9 in Register SPARE9 */
#define SPARE9_SPARE9_BIT_START                             0U
#define SPARE9_SPARE9_BIT_END                               31U



/**
 * @struct TOPRCMRegs_t
 * @brief
 *   Module MSS_TOPRCM Register Definition
 * @details
 *   This structure is used to access the MSS_TOPRCM module registers.
 */
/**
 * @typedef TOPRCMRegs
 * @brief
 *   Module MSS_TOPRCM Register Frame type Definition
 * @details
 *   This type is used to access the MSS_TOPRCM module registers.
 */
typedef volatile struct TOPRCMRegs_t
{
    uint32_t    CLKDIV                             ;        /* Offset = 0x000 */
    uint32_t    CLKSRCSEL                          ;        /* Offset = 0x004 */
    uint32_t    BSSCTL                             ;        /* Offset = 0x008 */
    uint32_t    RESTRICTED1                        ;        /* Offset = 0x00C */
    uint32_t    EXTCLKDIV                          ;        /* Offset = 0x010 */
    uint32_t    EXTCLKSRCSEL                       ;        /* Offset = 0x014 */
    uint32_t    EXTCLKCTL                          ;        /* Offset = 0x018 */
    uint32_t    SOFTSYSRST                         ;        /* Offset = 0x01C */
    uint32_t    WDRSTEN                            ;        /* Offset = 0x020 */
    uint32_t    SYSRSTCAUSE                        ;        /* Offset = 0x024 */
    uint32_t    SYSRSTCAUSECLR                     ;        /* Offset = 0x028 */
    uint32_t    RESTRICTED2[2]                     ;        /* Offset = 0x02C */
    uint32_t    MISCCAPT                           ;        /* Offset = 0x034 */
    uint32_t    DCDCCTL0                           ;        /* Offset = 0x038 */
    uint32_t    DCDCCTL1                           ;        /* Offset = 0x03C */
    uint32_t    RESTRICTED3[2]                     ;        /* Offset = 0x040 */
    uint32_t    USERMODEEN                         ;        /* Offset = 0x048 */
    uint32_t    LVDSPADCTL0                        ;        /* Offset = 0x04C */
    uint32_t    LVDSPADCTL1                        ;        /* Offset = 0x050 */
    uint32_t    RESTRICTED4[3]                     ;        /* Offset = 0x054 */
    uint32_t    DFTREG0                            ;        /* Offset = 0x060 */
    uint32_t    DFTREG1                            ;        /* Offset = 0x064 */
    uint32_t    DFTREG2                            ;        /* Offset = 0x068 */
    uint32_t    DFTREG3                            ;        /* Offset = 0x06C */
    uint32_t    DFTREG4                            ;        /* Offset = 0x070 */
    uint32_t    DFTREG5                            ;        /* Offset = 0x074 */
    uint32_t    SHMEMINITADDR                      ;        /* Offset = 0x078 */
    uint32_t    SHMEMINITECC                       ;        /* Offset = 0x07C */
    uint32_t    RESTRICTED5[8]                     ;        /* Offset = 0x080 */
    uint32_t    DSSMEMBANKEN                       ;        /* Offset = 0x0A0 */
    uint32_t    DSSMEMTAB0                         ;        /* Offset = 0x0A4 */
    uint32_t    DSSMEMTAB1                         ;        /* Offset = 0x0A8 */
    uint32_t    RESTRICTED6                        ;        /* Offset = 0x0AC */
    uint32_t    TCMAMEMBANK_EN                     ;        /* Offset = 0x0B0 */
    uint32_t    TCMAMEMTAB0                        ;        /* Offset = 0x0B4 */
    uint32_t    TCMAMEMTAB1                        ;        /* Offset = 0x0B8 */
    uint32_t    RESTRICTED7                        ;        /* Offset = 0x0BC */
    uint32_t    TCMBMEMBANKEN                      ;        /* Offset = 0x0C0 */
    uint32_t    TCMBMEMTAB0                        ;        /* Offset = 0x0C4 */
    uint32_t    TCMBMEM_TAB1                       ;        /* Offset = 0x0C8 */
    uint32_t    RESTRICTED8                        ;        /* Offset = 0x0CC */
    uint32_t    DSSIOCTL                           ;        /* Offset = 0x0D0 */
    uint32_t    RESTRICTED9                        ;        /* Offset = 0x0D4 */
    uint32_t    MEMINITSTART                       ;        /* Offset = 0x0D8 */
    uint32_t    MEMINITDONE                        ;        /* Offset = 0x0DC */
    uint32_t    RESTRICTED10[3]                    ;        /* Offset = 0x0E0 */
    uint32_t    SPARE0                             ;        /* Offset = 0x0EC */
    uint32_t    SPARE1                             ;        /* Offset = 0x0F0 */
    uint32_t    SPARE2                             ;        /* Offset = 0x0F4 */
    uint32_t    SPARE3                             ;        /* Offset = 0x0F8 */
    uint32_t    MSS_SIGNATURE                      ;        /* Offset = 0x0FC */
#if 0
    uint32_t    RESTRICTED11[32]                   ;        /* Offset = 0x100 */
#else
    uint32_t    RESTRICTED11[5]                    ;        /* Offset = 0x100 */
    uint32_t    EFUSEREGROW10                      ;        /* Offset = 0x114 */
    uint32_t    RESTRICTED14[26]                    ;        /* Offset = 0x118 */
#endif
    uint32_t    USERMODEEN2                        ;        /* Offset = 0x180 */
    uint32_t    RESTRICTED12[2]                    ;        /* Offset = 0x184 */
    uint32_t    SYSTICK                            ;        /* Offset = 0x18C */
    uint32_t    RESTRICTED13[21]                   ;        /* Offset = 0x190 */
    uint32_t    SPAREMULTIBIT                      ;        /* Offset = 0x1E4 */
    uint32_t    SPARE4                             ;        /* Offset = 0x1E8 */
    uint32_t    SPARE5                             ;        /* Offset = 0x1EC */
    uint32_t    SPARE6                             ;        /* Offset = 0x1F0 */
    uint32_t    SPARE7                             ;        /* Offset = 0x1F4 */
    uint32_t    SPARE8                             ;        /* Offset = 0x1F8 */
    uint32_t    SPARE9                             ;        /* Offset = 0x1FC */
} TOPRCMRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_TOPRCM_H */
/* END OF REG_TOPRCM_H */

