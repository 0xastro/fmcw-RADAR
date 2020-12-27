/**
 *  @file    reg_rcm_xwr16xx.h
 *
 *  @brief
 *    This file gives register definitions of MSS_RCM module.
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

#ifndef REG_RCM_H
#define REG_RCM_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field CR4SYSRST in Register SOFTRST1 */
#define SOFTRST1_CR4SYSRST_BIT_START                        0U
#define SOFTRST1_CR4SYSRST_BIT_END                          7U

/* Definition for field VIMRST in Register SOFTRST2 */
#define SOFTRST2_VIMRST_BIT_START                           24U
#define SOFTRST2_VIMRST_BIT_END                             31U

/* Definition for field WDTRST in Register SOFTRST3 */
#define SOFTRST3_WDTRST_BIT_START                           0U
#define SOFTRST3_WDTRST_BIT_END                             7U

/* Definition for field VCLKCLKDIV in Register CLKDIVCTL0 */
#define CLKDIVCTL0_VCLKCLKDIV_BIT_START                     8U
#define CLKDIVCTL0_VCLKCLKDIV_BIT_END                       15U

/* Definition for field DCANCLKDIV in Register CLKDIVCTL0 */
#define CLKDIVCTL0_DCANCLKDIV_BIT_START                     16U
#define CLKDIVCTL0_DCANCLKDIV_BIT_END                       23U

/* Definition for field FDCANCLKDIV in Register CLKDIVCTL0 */
#define CLKDIVCTL0_FDCANCLKDIV_BIT_START                    24U
#define CLKDIVCTL0_FDCANCLKDIV_BIT_END                      31U

/* Definition for field DCANCLKSRCSEL in Register CLKSRCSEL0 */
#define CLKSRCSEL0_DCANCLKSRCSEL_BIT_START                  0U
#define CLKSRCSEL0_DCANCLKSRCSEL_BIT_END                    3U

/* Definition for field FDCANCLKSRCSEL in Register CLKSRCSEL0 */
#define CLKSRCSEL0_FDCANCLKSRCSEL_BIT_START                 8U
#define CLKSRCSEL0_FDCANCLKSRCSEL_BIT_END                   11U

/* Definition for field QSPICLKSRCSEL in Register CLKSRCSEL0 */
#define CLKSRCSEL0_QSPICLKSRCSEL_BIT_START                  16U
#define CLKSRCSEL0_QSPICLKSRCSEL_BIT_END                    19U

/* Definition for field CR4MEMSWAP in Register CR4CTL */
#define CR4CTL_CR4MEMSWAP_BIT_START                         8U
#define CR4CTL_CR4MEMSWAP_BIT_END                           15U

/* Definition for field MEMSWAPWAIT in Register CR4CTL */
#define CR4CTL_MEMSWAPWAIT_BIT_START                        16U
#define CR4CTL_MEMSWAPWAIT_BIT_END                          23U

/* Definition for field QSPICLKGATE in Register CLKGATE */
#define CLKGATE_QSPICLKGATE_BIT_START                       3U
#define CLKGATE_QSPICLKGATE_BIT_END                         3U

/* Definition for field DCANCLKGATE in Register CLKGATE */
#define CLKGATE_DCANCLKGATE_BIT_START                       4U
#define CLKGATE_DCANCLKGATE_BIT_END                         4U

/* Definition for field FDCANCLKGATE in Register CLKGATE */
#define CLKGATE_FDCANCLKGATE_BIT_START                      5U
#define CLKGATE_FDCANCLKGATE_BIT_END                        5U

/* Definition for field VCLKCLKSRCSEL in Register CLKSRCSEL1 */
#define CLKSRCSEL1_VCLKCLKSRCSEL_BIT_START                  0U
#define CLKSRCSEL1_VCLKCLKSRCSEL_BIT_END                    3U

/* Definition for field VCLKCURRCLKDIV in Register CURRCLKDIV0 */
#define CURRCLKDIV0_VCLKCURRCLKDIV_BIT_START                8U
#define CURRCLKDIV0_VCLKCURRCLKDIV_BIT_END                  15U

/* Definition for field DCANCURRCLKDIV in Register CURRCLKDIV0 */
#define CURRCLKDIV0_DCANCURRCLKDIV_BIT_START                16U
#define CURRCLKDIV0_DCANCURRCLKDIV_BIT_END                  23U

/* Definition for field FDCANCURRCLKDIV in Register CURRCLKDIV0 */
#define CURRCLKDIV0_FDCANCURRCLKDIV_BIT_START               24U
#define CURRCLKDIV0_FDCANCURRCLKDIV_BIT_END                 31U

/* Definition for field CR4TCMAMEM in Register MEMINITSTART */
#define MEMINITSTART_CR4TCMAMEM_BIT_START                   0U
#define MEMINITSTART_CR4TCMAMEM_BIT_END                     0U

/* Definition for field CR4TCMBMEM in Register MEMINITSTART */
#define MEMINITSTART_CR4TCMBMEM_BIT_START                   1U
#define MEMINITSTART_CR4TCMBMEM_BIT_END                     1U

/* Definition for field DMAMEM in Register MEMINITSTART */
#define MEMINITSTART_DMAMEM_BIT_START                       2U
#define MEMINITSTART_DMAMEM_BIT_END                         2U

/* Definition for field VIMMEM in Register MEMINITSTART */
#define MEMINITSTART_VIMMEM_BIT_START                       3U
#define MEMINITSTART_VIMMEM_BIT_END                         3U

/* Definition for field SPIAMEM in Register MEMINITSTART */
#define MEMINITSTART_SPIAMEM_BIT_START                      4U
#define MEMINITSTART_SPIAMEM_BIT_END                        4U

/* Definition for field SPIBMEM in Register MEMINITSTART */
#define MEMINITSTART_SPIBMEM_BIT_START                      5U
#define MEMINITSTART_SPIBMEM_BIT_END                        5U

/* Definition for field DCANMEM in Register MEMINITSTART */
#define MEMINITSTART_DCANMEM_BIT_START                      6U
#define MEMINITSTART_DCANMEM_BIT_END                        6U

/* Definition for field MSSMBOX4BSSMEM in Register MEMINITSTART */
#define MEMINITSTART_MSSMBOX4BSSMEM_BIT_START               7U
#define MEMINITSTART_MSSMBOX4BSSMEM_BIT_END                 7U

/* Definition for field BSSMBOX4MSSMEM in Register MEMINITSTART */
#define MEMINITSTART_BSSMBOX4MSSMEM_BIT_START               8U
#define MEMINITSTART_BSSMBOX4MSSMEM_BIT_END                 8U

/* Definition for field DMA2MEM in Register MEMINITSTART */
#define MEMINITSTART_DMA2MEM_BIT_START                      9U
#define MEMINITSTART_DMA2MEM_BIT_END                        9U

/* Definition for field GEMMBOX4BSSMEM in Register MEMINITSTART */
#define MEMINITSTART_GEMMBOX4BSSMEM_BIT_START               14U
#define MEMINITSTART_GEMMBOX4BSSMEM_BIT_END                 14U

/* Definition for field GEMMBOX4MSSMEM in Register MEMINITSTART */
#define MEMINITSTART_GEMMBOX4MSSMEM_BIT_START               15U
#define MEMINITSTART_GEMMBOX4MSSMEM_BIT_END                 15U

/* Definition for field MSSMBOX4GEMMEM in Register MEMINITSTART */
#define MEMINITSTART_MSSMBOX4GEMMEM_BIT_START               16U
#define MEMINITSTART_MSSMBOX4GEMMEM_BIT_END                 16U

/* Definition for field BSSMBOX4GEMMEM in Register MEMINITSTART */
#define MEMINITSTART_BSSMBOX4GEMMEM_BIT_START               17U
#define MEMINITSTART_BSSMBOX4GEMMEM_BIT_END                 17U

/* Definition for field MEMINITKEY in Register MEMINITSTART */
#define MEMINITSTART_MEMINITKEY_BIT_START                   24U
#define MEMINITSTART_MEMINITKEY_BIT_END                     31U

/* Definition for field QSPICURRCLKDIV in Register CURRCLKDIV1 */
#define CURRCLKDIV1_QSPICURRCLKDIV_BIT_START                0U
#define CURRCLKDIV1_QSPICURRCLKDIV_BIT_END                  7U

/* Definition for field CR4TCMAMEM in Register MEMINITDONE */
#define MEMINITDONE_CR4TCMAMEM_BIT_START                    0U
#define MEMINITDONE_CR4TCMAMEM_BIT_END                      0U

/* Definition for field CR4TCMBMEM in Register MEMINITDONE */
#define MEMINITDONE_CR4TCMBMEM_BIT_START                    1U
#define MEMINITDONE_CR4TCMBMEM_BIT_END                      1U

/* Definition for field DMAMEM in Register MEMINITDONE */
#define MEMINITDONE_DMAMEM_BIT_START                        2U
#define MEMINITDONE_DMAMEM_BIT_END                          2U

/* Definition for field VIMMEM in Register MEMINITDONE */
#define MEMINITDONE_VIMMEM_BIT_START                        3U
#define MEMINITDONE_VIMMEM_BIT_END                          3U

/* Definition for field SPIAMEM in Register MEMINITDONE */
#define MEMINITDONE_SPIAMEM_BIT_START                       4U
#define MEMINITDONE_SPIAMEM_BIT_END                         4U

/* Definition for field SPIBMEM in Register MEMINITDONE */
#define MEMINITDONE_SPIBMEM_BIT_START                       5U
#define MEMINITDONE_SPIBMEM_BIT_END                         5U

/* Definition for field DCANMEM in Register MEMINITDONE */
#define MEMINITDONE_DCANMEM_BIT_START                       6U
#define MEMINITDONE_DCANMEM_BIT_END                         6U

/* Definition for field MSSMBOX4BSSMEM in Register MEMINITDONE */
#define MEMINITDONE_MSSMBOX4BSSMEM_BIT_START                7U
#define MEMINITDONE_MSSMBOX4BSSMEM_BIT_END                  7U

/* Definition for field BSSMBOX4MSSMEM in Register MEMINITDONE */
#define MEMINITDONE_BSSMBOX4MSSMEM_BIT_START                8U
#define MEMINITDONE_BSSMBOX4MSSMEM_BIT_END                  8U

/* Definition for field DMA2MEM in Register MEMINITDONE */
#define MEMINITDONE_DMA2MEM_BIT_START                       9U
#define MEMINITDONE_DMA2MEM_BIT_END                         9U

/* Definition for field GEMMBOX4BSSMEM in Register MEMINITDONE */
#define MEMINITDONE_GEMMBOX4BSSMEM_BIT_START                14U
#define MEMINITDONE_GEMMBOX4BSSMEM_BIT_END                  14U

/* Definition for field GEMMBOX4MSSMEM in Register MEMINITDONE */
#define MEMINITDONE_GEMMBOX4MSSMEM_BIT_START                15U
#define MEMINITDONE_GEMMBOX4MSSMEM_BIT_END                  15U

/* Definition for field MSSMBOX4GEMMEM in Register MEMINITDONE */
#define MEMINITDONE_MSSMBOX4GEMMEM_BIT_START                16U
#define MEMINITDONE_MSSMBOX4GEMMEM_BIT_END                  16U

/* Definition for field BSSMBOX4GEMMEM in Register MEMINITDONE */
#define MEMINITDONE_BSSMBOX4GEMMEM_BIT_START                17U
#define MEMINITDONE_BSSMBOX4GEMMEM_BIT_END                  17U

/* Definition for field ECCENMSSGEM in Register ECCENMSSGEM */
#define ECCENMSSGEM_ECCENMSSGEM_BIT_START                   0U
#define ECCENMSSGEM_ECCENMSSGEM_BIT_END                     31U

/* Definition for field ECCCAPTMSSGEM in Register ECCCAPTMSSGEM */
#define ECCCAPTMSSGEM_ECCCAPTMSSGEM_BIT_START               0U
#define ECCCAPTMSSGEM_ECCCAPTMSSGEM_BIT_END                 31U

/* Definition for field ECCENBSSGEM in Register ECCENBSSGEM */
#define ECCENBSSGEM_ECCENBSSGEM_BIT_START                   0U
#define ECCENBSSGEM_ECCENBSSGEM_BIT_END                     31U

/* Definition for field ECCCAPTBSSGEM in Register ECCCAPTBSSGEM */
#define ECCCAPTBSSGEM_ECCCAPTBSSGEM_BIT_START               0U
#define ECCCAPTBSSGEM_ECCCAPTBSSGEM_BIT_END                 31U

/* Definition for field USERMODEEN in Register USERMODEEN */
#define USERMODEEN_USERMODEEN_BIT_START                     0U
#define USERMODEEN_USERMODEEN_BIT_END                       31U

/* Definition for field NSYSPERUSERMODEN in Register NSYSPERUSERMODEN */
#define NSYSPERUSERMODEN_NSYSPERUSERMODEN_BIT_START         0U
#define NSYSPERUSERMODEN_NSYSPERUSERMODEN_BIT_END           31U

/* Definition for field SECURERAMINIT in Register SECURERAMMMI */
#define SECURERAMMMI_SECURERAMINIT_BIT_START                0U
#define SECURERAMMMI_SECURERAMINIT_BIT_END                  0U

/* Definition for field SECURERAMINITDONE in Register SECURERAMMMI */
#define SECURERAMMMI_SECURERAMINITDONE_BIT_START            16U
#define SECURERAMMMI_SECURERAMINITDONE_BIT_END              16U

/* Definition for field SECURERAMECCEN in Register SECURERAMECC */
#define SECURERAMECC_SECURERAMECCEN_BIT_START               0U
#define SECURERAMECC_SECURERAMECCEN_BIT_END                 7U

/* Definition for field SECURERAMECCCLR in Register SECURERAMECC */
#define SECURERAMECC_SECURERAMECCCLR_BIT_START              8U
#define SECURERAMECC_SECURERAMECCCLR_BIT_END                10U

/* Definition for field SECURERAMADDR in Register SECURERAMECC */
#define SECURERAMECC_SECURERAMADDR_BIT_START                16U
#define SECURERAMECC_SECURERAMADDR_BIT_END                  23U

/* Definition for field SECURERAMBIT in Register SECURERAMECC */
#define SECURERAMECC_SECURERAMBIT_BIT_START                 24U
#define SECURERAMECC_SECURERAMBIT_BIT_END                   31U

/* Definition for field ESMGATE0 in Register ESMGATE0 */
#define ESMGATE0_ESMGATE0_BIT_START                         0U
#define ESMGATE0_ESMGATE0_BIT_END                           31U

/* Definition for field ESMGATE1 in Register ESMGATE1 */
#define ESMGATE1_ESMGATE1_BIT_START                         0U
#define ESMGATE1_ESMGATE1_BIT_END                           31U

/* Definition for field ESMGATE2 in Register ESMGATE2 */
#define ESMGATE2_ESMGATE2_BIT_START                         0U
#define ESMGATE2_ESMGATE2_BIT_END                           31U

/* Definition for field ESMGATE3 in Register ESMGATE3 */
#define ESMGATE3_ESMGATE3_BIT_START                         0U
#define ESMGATE3_ESMGATE3_BIT_END                           31U

/* Definition for field ESMGATE4 in Register ESMGATE4 */
#define ESMGATE4_ESMGATE4_BIT_START                         0U
#define ESMGATE4_ESMGATE4_BIT_END                           31U

/* Definition for field KEY in Register KEY */
#define KEY_KEY_BIT_START                                   0U
#define KEY_KEY_BIT_END                                     31U

/* Definition for field SWIRQ0DAT in Register SWIRQA */
#define SWIRQA_SWIRQ0DAT_BIT_START                          0U
#define SWIRQA_SWIRQ0DAT_BIT_END                            7U

/* Definition for field SWIRQ0 in Register SWIRQA */
#define SWIRQA_SWIRQ0_BIT_START                             8U
#define SWIRQA_SWIRQ0_BIT_END                               15U

/* Definition for field SWIRQ1DAT in Register SWIRQA */
#define SWIRQA_SWIRQ1DAT_BIT_START                          16U
#define SWIRQA_SWIRQ1DAT_BIT_END                            23U

/* Definition for field SWIRQ1 in Register SWIRQA */
#define SWIRQA_SWIRQ1_BIT_START                             24U
#define SWIRQA_SWIRQ1_BIT_END                               31U

/* Definition for field SWIRQ2DAT in Register SWIRQB */
#define SWIRQB_SWIRQ2DAT_BIT_START                          0U
#define SWIRQB_SWIRQ2DAT_BIT_END                            7U

/* Definition for field SWIRQ2 in Register SWIRQB */
#define SWIRQB_SWIRQ2_BIT_START                             8U
#define SWIRQB_SWIRQ2_BIT_END                               15U

/* Definition for field SWIRQ3DAT in Register SWIRQB */
#define SWIRQB_SWIRQ3DAT_BIT_START                          16U
#define SWIRQB_SWIRQ3DAT_BIT_END                            23U

/* Definition for field SWIRQ3 in Register SWIRQB */
#define SWIRQB_SWIRQ3_BIT_START                             24U
#define SWIRQB_SWIRQ3_BIT_END                               31U

/* Definition for field TCMAEZDIS in Register MISCCTL0 */
#define MISCCTL0_TCMAEZDIS_BIT_START                        16U
#define MISCCTL0_TCMAEZDIS_BIT_END                          18U

/* Definition for field TCMB0EZDIS in Register MISCCTL0 */
#define MISCCTL0_TCMB0EZDIS_BIT_START                       19U
#define MISCCTL0_TCMB0EZDIS_BIT_END                         21U

/* Definition for field TCMB1EZDIS in Register MISCCTL0 */
#define MISCCTL0_TCMB1EZDIS_BIT_START                       22U
#define MISCCTL0_TCMB1EZDIS_BIT_END                         24U

/* Definition for field ERRATCADDRCLR in Register ATCMERRCAPTCTL */
#define ATCMERRCAPTCTL_ERRATCADDRCLR_BIT_START              0U
#define ATCMERRCAPTCTL_ERRATCADDRCLR_BIT_END                2U

/* Definition for field ATCFORCEERR in Register ATCMERRCAPTCTL */
#define ATCMERRCAPTCTL_ATCFORCEERR_BIT_START                3U
#define ATCMERRCAPTCTL_ATCFORCEERR_BIT_END                  5U

/* Definition for field ERRATCADDR in Register ATCMERRCAPTCTL */
#define ATCMERRCAPTCTL_ERRATCADDR_BIT_START                 8U
#define ATCMERRCAPTCTL_ERRATCADDR_BIT_END                   27U

/* Definition for field ERRB0TCADDRCLR in Register B0TCMERRCAPTCTL */
#define B0TCMERRCAPTCTL_ERRB0TCADDRCLR_BIT_START            0U
#define B0TCMERRCAPTCTL_ERRB0TCADDRCLR_BIT_END              2U

/* Definition for field B0TCFORCEERR in Register B0TCMERRCAPTCTL */
#define B0TCMERRCAPTCTL_B0TCFORCEERR_BIT_START              3U
#define B0TCMERRCAPTCTL_B0TCFORCEERR_BIT_END                5U

/* Definition for field ERRB0TCADDR in Register B0TCMERRCAPTCTL */
#define B0TCMERRCAPTCTL_ERRB0TCADDR_BIT_START               8U
#define B0TCMERRCAPTCTL_ERRB0TCADDR_BIT_END                 27U

/* Definition for field ERRB1TCADDRCLR in Register B1TCMERRCAPTCTL */
#define B1TCMERRCAPTCTL_ERRB1TCADDRCLR_BIT_START            0U
#define B1TCMERRCAPTCTL_ERRB1TCADDRCLR_BIT_END              2U

/* Definition for field B1TCFORCEERR in Register B1TCMERRCAPTCTL */
#define B1TCMERRCAPTCTL_B1TCFORCEERR_BIT_START              3U
#define B1TCMERRCAPTCTL_B1TCFORCEERR_BIT_END                5U

/* Definition for field ERRB1TCADDR in Register B1TCMERRCAPTCTL */
#define B1TCMERRCAPTCTL_ERRB1TCADDR_BIT_START               8U
#define B1TCMERRCAPTCTL_ERRB1TCADDR_BIT_END                 27U

/* Definition for field RSTTOASSRTDLY in Register SOFTCORERST */
#define SOFTCORERST_RSTTOASSRTDLY_BIT_START                 8U
#define SOFTCORERST_RSTTOASSRTDLY_BIT_END                   15U

/* Definition for field RST_WFICHECKEN in Register SOFTCORERST */
#define SOFTCORERST_RST_WFICHECKEN_BIT_START                24U
#define SOFTCORERST_RST_WFICHECKEN_BIT_END                  31U

/* Definition for field RSTCAUSE in Register RSTCAUSE */
#define RSTCAUSE_RSTCAUSE_BIT_START                         0U
#define RSTCAUSE_RSTCAUSE_BIT_END                           7U

/* Definition for field RSTCAUSECLR in Register RSTCAUSECLR */
#define RSTCAUSECLR_RSTCAUSECLR_BIT_START                   0U
#define RSTCAUSECLR_RSTCAUSECLR_BIT_END                     7U

/* Definition for field SPIATRIG0 in Register SPITRIGSRC */
#define SPITRIGSRC_SPIATRIG0_BIT_START                      0U
#define SPITRIGSRC_SPIATRIG0_BIT_END                        0U

/* Definition for field SPIATRIG1 in Register SPITRIGSRC */
#define SPITRIGSRC_SPIATRIG1_BIT_START                      8U
#define SPITRIGSRC_SPIATRIG1_BIT_END                        8U

/* Definition for field SPIBTRIG in Register SPITRIGSRC */
#define SPITRIGSRC_SPIBTRIG_BIT_START                       16U
#define SPITRIGSRC_SPIBTRIG_BIT_END                         26U

/* Definition for field VCLKINUSE in Register CLKINUSE */
#define CLKINUSE_VCLKINUSE_BIT_START                        0U
#define CLKINUSE_VCLKINUSE_BIT_END                          3U

/* Definition for field FDCANCLKINUSE in Register CLKINUSE */
#define CLKINUSE_FDCANCLKINUSE_BIT_START                    4U
#define CLKINUSE_FDCANCLKINUSE_BIT_END                      7U

/* Definition for field DCANCLKINUSE in Register CLKINUSE */
#define CLKINUSE_DCANCLKINUSE_BIT_START                     8U
#define CLKINUSE_DCANCLKINUSE_BIT_END                       11U

/* Definition for field QSPICLKINUSE in Register CLKINUSE */
#define CLKINUSE_QSPICLKINUSE_BIT_START                     12U
#define CLKINUSE_QSPICLKINUSE_BIT_END                       15U

/* Definition for field ECCENMSSBSS in Register ECCENMSSBSS */
#define ECCENMSSBSS_ECCENMSSBSS_BIT_START                   0U
#define ECCENMSSBSS_ECCENMSSBSS_BIT_END                     31U

/* Definition for field ECCCAPTMSSBSS in Register ECCCAPTMSSBSS */
#define ECCCAPTMSSBSS_ECCCAPTMSSBSS_BIT_START               0U
#define ECCCAPTMSSBSS_ECCCAPTMSSBSS_BIT_END                 31U

/* Definition for field QSPICLKDIV in Register CLKDIVCTL2 */
#define CLKDIVCTL2_QSPICLKDIV_BIT_START                     0U
#define CLKDIVCTL2_QSPICLKDIV_BIT_END                       7U

/* Definition for field SWIRQ4DAT in Register SWIRQC */
#define SWIRQC_SWIRQ4DAT_BIT_START                          0U
#define SWIRQC_SWIRQ4DAT_BIT_END                            7U

/* Definition for field SWIRQ4 in Register SWIRQC */
#define SWIRQC_SWIRQ4_BIT_START                             8U
#define SWIRQC_SWIRQ4_BIT_END                               15U

/* Definition for field SWIRQ5DAT in Register SWIRQC */
#define SWIRQC_SWIRQ5DAT_BIT_START                          16U
#define SWIRQC_SWIRQ5DAT_BIT_END                            23U

/* Definition for field SWIRQ5 in Register SWIRQC */
#define SWIRQC_SWIRQ5_BIT_START                             24U
#define SWIRQC_SWIRQ5_BIT_END                               31U



/**
 * @struct RCMRegs_t
 * @brief
 *   Module MSS_RCM Register Definition
 * @details
 *   This structure is used to access the MSS_RCM module registers.
 */
/**
 * @typedef RCMRegs
 * @brief
 *   Module MSS_RCM Register Frame type Definition
 * @details
 *   This type is used to access the MSS_RCM module registers.
 */
typedef volatile struct RCMRegs_t
{
    uint32_t    SOFTRST0                           ;        /* Offset = 0x000 */
    uint32_t    SOFTRST1                           ;        /* Offset = 0x004 */
    uint32_t    SOFTRST2                           ;        /* Offset = 0x008 */
    uint32_t    SOFTRST3                           ;        /* Offset = 0x00C */
    uint32_t    RESTRICTED1[2]                     ;        /* Offset = 0x010 */
    uint32_t    CLKDIVCTL0                         ;        /* Offset = 0x018 */
    uint32_t    CLKSRCSEL0                         ;        /* Offset = 0x01C */
    uint32_t    CR4CTL                             ;        /* Offset = 0x020 */
    uint32_t    RESTRICTED2[6]                     ;        /* Offset = 0x024 */
    uint32_t    CLKGATE                            ;        /* Offset = 0x03C */
    uint32_t    RESTRICTED3                        ;        /* Offset = 0x040 */
    uint32_t    CLKSRCSEL1                         ;        /* Offset = 0x044 */
    uint32_t    RESTRICTED4[3]                     ;        /* Offset = 0x048 */
    uint32_t    CURRCLKDIV0                        ;        /* Offset = 0x054 */
    uint32_t    RESTRICTED5                        ;        /* Offset = 0x058 */
    uint32_t    MEMINITSTART                       ;        /* Offset = 0x05C */
    uint32_t    CURRCLKDIV1                        ;        /* Offset = 0x060 */
    uint32_t    RESTRICTED6[2]                     ;        /* Offset = 0x064 */
    uint32_t    MEMINITDONE                        ;        /* Offset = 0x06C */
    uint32_t    ECCENMSSGEM                        ;        /* Offset = 0x070 */
    uint32_t    ECCCAPTMSSGEM                      ;        /* Offset = 0x074 */
    uint32_t    ECCENBSSGEM                        ;        /* Offset = 0x078 */
    uint32_t    ECCCAPTBSSGEM                      ;        /* Offset = 0x07C */
    uint32_t    USERMODEEN                         ;        /* Offset = 0x080 */
    uint32_t    NSYSPERUSERMODEN                   ;        /* Offset = 0x084 */
    uint32_t    SECURERAMMMI                       ;        /* Offset = 0x088 */
    uint32_t    SECURERAMECC                       ;        /* Offset = 0x08C */
    uint32_t    ESMGATE0                           ;        /* Offset = 0x090 */
    uint32_t    ESMGATE1                           ;        /* Offset = 0x094 */
    uint32_t    ESMGATE2                           ;        /* Offset = 0x098 */
    uint32_t    ESMGATE3                           ;        /* Offset = 0x09C */
    uint32_t    ESMGATE4                           ;        /* Offset = 0x0A0 */
    uint32_t    RESTRICTED7[2]                     ;        /* Offset = 0x0A4 */
    uint32_t    KEY                                ;        /* Offset = 0x0AC */
    uint32_t    RESTRICTED8[2]                     ;        /* Offset = 0x0B0 */
    uint32_t    SWIRQA                             ;        /* Offset = 0x0B8 */
    uint32_t    SWIRQB                             ;        /* Offset = 0x0BC */
    uint32_t    MISCCTL0                           ;        /* Offset = 0x0C0 */
    uint32_t    ATCMERRCAPTCTL                     ;        /* Offset = 0x0C4 */
    uint32_t    B0TCMERRCAPTCTL                    ;        /* Offset = 0x0C8 */
    uint32_t    B1TCMERRCAPTCTL                    ;        /* Offset = 0x0CC */
    uint32_t    SOFTCORERST                        ;        /* Offset = 0x0D0 */
    uint32_t    RESTRICTED9                        ;        /* Offset = 0x0D4 */
    uint32_t    RSTCAUSE                           ;        /* Offset = 0x0D8 */
    uint32_t    RSTCAUSECLR                        ;        /* Offset = 0x0DC */
    uint32_t    SPITRIGSRC                         ;        /* Offset = 0x0E0 */
    uint32_t    CLKINUSE                           ;        /* Offset = 0x0E4 */
    uint32_t    ECCENMSSBSS                        ;        /* Offset = 0x0E8 */
    uint32_t    ECCCAPTMSSBSS                      ;        /* Offset = 0x0EC */
    uint32_t    CLKDIVCTL2                         ;        /* Offset = 0x0F0 */
    uint32_t    RESTRICTED10[2]                    ;        /* Offset = 0x0F4 */
    uint32_t    SWIRQC                             ;        /* Offset = 0x0FC */
} RCMRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_RCM_H */
/* END OF REG_RCM_H */

