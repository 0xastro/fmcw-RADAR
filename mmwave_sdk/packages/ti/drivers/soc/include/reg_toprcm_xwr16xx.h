/**
 *  @file    reg_toprcm_xwr16xx.h
 *
 *  @brief
 *    This file gives register definitions of MSS_TOPRCM module.
 *
 *  This file is auto-generated on 2/2/2018.
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

/* Definition for field BSSCPUHALT in Register BSSCTL */
#define BSSCTL_BSSCPUHALT_BIT_START                         24U
#define BSSCTL_BSSCPUHALT_BIT_END                           31U

/* Definition for field GEMPORZ in Register DSSCTL */
#define DSSCTL_GEMPORZ_BIT_START                            24U
#define DSSCTL_GEMPORZ_BIT_END                              24U

/* Definition for field GEMGRSTN in Register DSSCTL */
#define DSSCTL_GEMGRSTN_BIT_START                           25U
#define DSSCTL_GEMGRSTN_BIT_END                             25U

/* Definition for field GEMLRSTN in Register DSSCTL */
#define DSSCTL_GEMLRSTN_BIT_START                           26U
#define DSSCTL_GEMLRSTN_BIT_END                             26U

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

/* Definition for field DFTREG5 in Register DFTREG5 */
#define DFTREG5_DFTREG5_BIT_START                           0U
#define DFTREG5_DFTREG5_BIT_END                             31U

/* Definition for field LSPRSTVAL in Register DSSIOCTL */
#define DSSIOCTL_LSPRSTVAL_BIT_START                        0U
#define DSSIOCTL_LSPRSTVAL_BIT_END                          21U

/* Definition for field EMCCFGCLK in Register DSSIOCTL */
#define DSSIOCTL_EMCCFGCLK_BIT_START                        24U
#define DSSIOCTL_EMCCFGCLK_BIT_END                          25U

/* Definition for field EMCDMACLK in Register DSSIOCTL */
#define DSSIOCTL_EMCDMACLK_BIT_START                        26U
#define DSSIOCTL_EMCDMACLK_BIT_END                          27U

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
/* Definition for field EFUSEREGROW10 in Register EFUSEREGROW10 */
#define EFUSEREGROW10_EFUSEREGROW10_BIT_START               0U
#define EFUSEREGROW10_EFUSEREGROW10_BIT_END                 25U
#else
/* Definition for field PART_NUMBER in Register EFUSEREGROW10 */
#define EFUSEREGROW10_PART_NUMBER_BIT_START                 18U
#define EFUSEREGROW10_PART_NUMBER_BIT_END                   25U
#endif

/* Definition for field GEMBOOTSTCEN in Register GEMBOOTSTCEN */
#define GEMBOOTSTCEN_GEMBOOTSTCEN_BIT_START                 0U
#define GEMBOOTSTCEN_GEMBOOTSTCEN_BIT_END                   0U

/* Definition for field USERMODEEN2 in Register USERMODEEN2 */
#define USERMODEEN2_USERMODEEN2_BIT_START                   0U
#define USERMODEEN2_USERMODEEN2_BIT_END                     31U

/* Definition for field SYSTICK in Register SYSTICK */
#define SYSTICK_SYSTICK_BIT_START                           0U
#define SYSTICK_SYSTICK_BIT_END                             31U

/* Definition for field CUSTCEKFIREWALLEN in Register SECURECFGREG1 */
#define SECURECFGREG1_CUSTCEKFIREWALLEN_BIT_START           4U
#define SECURECFGREG1_CUSTCEKFIREWALLEN_BIT_END             6U

/* Definition for field CRYPTOFIREWALLEN in Register SECURECFGREG1 */
#define SECURECFGREG1_CRYPTOFIREWALLEN_BIT_START            8U
#define SECURECFGREG1_CRYPTOFIREWALLEN_BIT_END              10U

/* Definition for field TRACEFIREWALLEN in Register SECURECFGREG1 */
#define SECURECFGREG1_TRACEFIREWALLEN_BIT_START             12U
#define SECURECFGREG1_TRACEFIREWALLEN_BIT_END               14U

/* Definition for field LOGGERFIREWALLEN in Register SECURECFGREG1 */
#define SECURECFGREG1_LOGGERFIREWALLEN_BIT_START            16U
#define SECURECFGREG1_LOGGERFIREWALLEN_BIT_END              18U

/* Definition for field SECURERAMFIREWALLEN in Register SECURECFGREG1 */
#define SECURECFGREG1_SECURERAMFIREWALLEN_BIT_START         20U
#define SECURECFGREG1_SECURERAMFIREWALLEN_BIT_END           22U

/* Definition for field JTAGFIREWALLEN in Register SECURECFGREG1 */
#define SECURECFGREG1_JTAGFIREWALLEN_BIT_START              28U
#define SECURECFGREG1_JTAGFIREWALLEN_BIT_END                30U

/* Definition for field DMMFIREWALLEN in Register SECURECFGREG2 */
#define SECURECFGREG2_DMMFIREWALLEN_BIT_START               0U
#define SECURECFGREG2_DMMFIREWALLEN_BIT_END                 2U

/* Definition for field CUSTKEYERASE in Register SECURECFGREG2 */
#define SECURECFGREG2_CUSTKEYERASE_BIT_START                8U
#define SECURECFGREG2_CUSTKEYERASE_BIT_END                  10U

/* Definition for field LOGGERSTICKYBIT in Register SECURECFGREG3 */
#define SECURECFGREG3_LOGGERSTICKYBIT_BIT_START             0U
#define SECURECFGREG3_LOGGERSTICKYBIT_BIT_END               2U

/* Definition for field CUSTCEKSTICKYBIT in Register SECURECFGREG3 */
#define SECURECFGREG3_CUSTCEKSTICKYBIT_BIT_START            8U
#define SECURECFGREG3_CUSTCEKSTICKYBIT_BIT_END              10U

/* Definition for field CRYPTOSTICKYBIT in Register SECURECFGREG3 */
#define SECURECFGREG3_CRYPTOSTICKYBIT_BIT_START             12U
#define SECURECFGREG3_CRYPTOSTICKYBIT_BIT_END               14U

/* Definition for field TRACESTICKYBIT in Register SECURECFGREG3 */
#define SECURECFGREG3_TRACESTICKYBIT_BIT_START              16U
#define SECURECFGREG3_TRACESTICKYBIT_BIT_END                18U

/* Definition for field SECURERAMSTICKYBIT in Register SECURECFGREG3 */
#define SECURECFGREG3_SECURERAMSTICKYBIT_BIT_START          20U
#define SECURECFGREG3_SECURERAMSTICKYBIT_BIT_END            22U

/* Definition for field JTAGSTICKYBIT in Register SECURECFGREG3 */
#define SECURECFGREG3_JTAGSTICKYBIT_BIT_START               28U
#define SECURECFGREG3_JTAGSTICKYBIT_BIT_END                 30U

/* Definition for field DMMSTICKYBIT in Register SECURECFGREG4 */
#define SECURECFGREG4_DMMSTICKYBIT_BIT_START                0U
#define SECURECFGREG4_DMMSTICKYBIT_BIT_END                  2U

/* Definition for field SECURERAMRDDONE in Register SECURERAMREG */
#define SECURERAMREG_SECURERAMRDDONE_BIT_START              0U
#define SECURERAMREG_SECURERAMRDDONE_BIT_END                0U

/* Definition for field SECURERAMKEYRD in Register SECURERAMREG */
#define SECURERAMREG_SECURERAMKEYRD_BIT_START               8U
#define SECURERAMREG_SECURERAMKEYRD_BIT_END                 8U

/* Definition for field SECURERAMKEYIDX in Register SECURERAMREG */
#define SECURERAMREG_SECURERAMKEYIDX_BIT_START              16U
#define SECURERAMREG_SECURERAMKEYIDX_BIT_END                23U

/* Definition for field SECURERAMKEY255 in Register SECURERAMREG */
#define SECURERAMREG_SECURERAMKEY255_BIT_START              24U
#define SECURERAMREG_SECURERAMKEY255_BIT_END                24U

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

/* Definition for field UID31TO0 in Register UID31TO0 */
#define UID31TO0_UID31TO0_BIT_START                         0U
#define UID31TO0_UID31TO0_BIT_END                           31U

/* Definition for field UID63TO32 in Register UID63TO32 */
#define UID63TO32_UID63TO32_BIT_START                       0U
#define UID63TO32_UID63TO32_BIT_END                         31U

/* Definition for field UID95TO64 in Register UID95TO64 */
#define UID95TO64_UID95TO64_BIT_START                       0U
#define UID95TO64_UID95TO64_BIT_END                         31U

/* Definition for field UID119TO96 in Register UID119TO96 */
#define UID119TO96_UID119TO96_BIT_START                     0U
#define UID119TO96_UID119TO96_BIT_END                       31U

/* Definition for field MEMINITSTARTBANK0 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK0_BIT_START       0U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK0_BIT_END         0U

/* Definition for field MEMINITSTARTBANK1 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK1_BIT_START       1U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK1_BIT_END         1U

/* Definition for field MEMINITSTARTBANK2 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK2_BIT_START       2U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK2_BIT_END         2U

/* Definition for field MEMINITSTARTBANK3 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK3_BIT_START       3U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK3_BIT_END         3U

/* Definition for field MEMINITSTARTBANK4 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK4_BIT_START       4U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK4_BIT_END         4U

/* Definition for field MEMINITSTARTBANK5 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK5_BIT_START       5U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK5_BIT_END         5U

/* Definition for field MEMINITSTARTBANK6 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK6_BIT_START       6U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK6_BIT_END         6U

/* Definition for field MEMINITSTARTBANK7 in Register MEMINITSTARTSHMEM */
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK7_BIT_START       7U
#define MEMINITSTARTSHMEM_MEMINITSTARTBANK7_BIT_END         7U

/* Definition for field MEMINITDONEBANK0 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK0_BIT_START         0U
#define MEMINITDONESHMEM_MEMINITDONEBANK0_BIT_END           0U

/* Definition for field MEMINITDONEBANK1 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK1_BIT_START         1U
#define MEMINITDONESHMEM_MEMINITDONEBANK1_BIT_END           1U

/* Definition for field MEMINITDONEBANK2 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK2_BIT_START         2U
#define MEMINITDONESHMEM_MEMINITDONEBANK2_BIT_END           2U

/* Definition for field MEMINITDONEBANK3 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK3_BIT_START         3U
#define MEMINITDONESHMEM_MEMINITDONEBANK3_BIT_END           3U

/* Definition for field MEMINITDONEBANK4 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK4_BIT_START         4U
#define MEMINITDONESHMEM_MEMINITDONEBANK4_BIT_END           4U

/* Definition for field MEMINITDONEBANK5 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK5_BIT_START         5U
#define MEMINITDONESHMEM_MEMINITDONEBANK5_BIT_END           5U

/* Definition for field MEMINITDONEBANK6 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK6_BIT_START         6U
#define MEMINITDONESHMEM_MEMINITDONEBANK6_BIT_END           6U

/* Definition for field MEMINITDONEBANK7 in Register MEMINITDONESHMEM */
#define MEMINITDONESHMEM_MEMINITDONEBANK7_BIT_START         7U
#define MEMINITDONESHMEM_MEMINITDONEBANK7_BIT_END           7U

/* Definition for field DSSMEMTAB0 in Register DSSMEMTAB0 */
#define DSSMEMTAB0_DSSMEMTAB0_BIT_START                     0U
#define DSSMEMTAB0_DSSMEMTAB0_BIT_END                       31U

/* Definition for field TCMAMEMTAB in Register TCMAMEMTAB */
#define TCMAMEMTAB_TCMAMEMTAB_BIT_START                     0U
#define TCMAMEMTAB_TCMAMEMTAB_BIT_END                       15U

/* Definition for field TCMBMEMTAB in Register TCMBMEMTAB */
#define TCMBMEMTAB_TCMBMEMTAB_BIT_START                     0U
#define TCMBMEMTAB_TCMBMEMTAB_BIT_END                       15U

/* Definition for field BANK0 in Register SHMEMBANKSEL3TO0 */
#define SHMEMBANKSEL3TO0_BANK0_BIT_START                    0U
#define SHMEMBANKSEL3TO0_BANK0_BIT_END                      7U

/* Definition for field BANK1 in Register SHMEMBANKSEL3TO0 */
#define SHMEMBANKSEL3TO0_BANK1_BIT_START                    8U
#define SHMEMBANKSEL3TO0_BANK1_BIT_END                      15U

/* Definition for field BANK2 in Register SHMEMBANKSEL3TO0 */
#define SHMEMBANKSEL3TO0_BANK2_BIT_START                    16U
#define SHMEMBANKSEL3TO0_BANK2_BIT_END                      23U

/* Definition for field BANK3 in Register SHMEMBANKSEL3TO0 */
#define SHMEMBANKSEL3TO0_BANK3_BIT_START                    24U
#define SHMEMBANKSEL3TO0_BANK3_BIT_END                      31U

/* Definition for field BANK4 in Register SHMEMBANKSEL7TO4 */
#define SHMEMBANKSEL7TO4_BANK4_BIT_START                    0U
#define SHMEMBANKSEL7TO4_BANK4_BIT_END                      7U

/* Definition for field BANK5 in Register SHMEMBANKSEL7TO4 */
#define SHMEMBANKSEL7TO4_BANK5_BIT_START                    8U
#define SHMEMBANKSEL7TO4_BANK5_BIT_END                      15U

/* Definition for field BANK6 in Register SHMEMBANKSEL7TO4 */
#define SHMEMBANKSEL7TO4_BANK6_BIT_START                    16U
#define SHMEMBANKSEL7TO4_BANK6_BIT_END                      23U

/* Definition for field BANK7 in Register SHMEMBANKSEL7TO4 */
#define SHMEMBANKSEL7TO4_BANK7_BIT_START                    24U
#define SHMEMBANKSEL7TO4_BANK7_BIT_END                      31U

/* Definition for field PBIST300MCLKDIV in Register PBISTCLKCTL */
#define PBISTCLKCTL_PBIST300MCLKDIV_BIT_START               0U
#define PBISTCLKCTL_PBIST300MCLKDIV_BIT_END                 3U

/* Definition for field PBIST300MCLKSRCSEL in Register PBISTCLKCTL */
#define PBISTCLKCTL_PBIST300MCLKSRCSEL_BIT_START            4U
#define PBISTCLKCTL_PBIST300MCLKSRCSEL_BIT_END              7U

/* Definition for field PBIST300MCLKGATE in Register PBISTCLKCTL */
#define PBISTCLKCTL_PBIST300MCLKGATE_BIT_START              8U
#define PBISTCLKCTL_PBIST300MCLKGATE_BIT_END                15U



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
    uint32_t    RESTRICTED1                        ;        /* Offset = 0x004 */
    uint32_t    BSSCTL                             ;        /* Offset = 0x008 */
    uint32_t    DSSCTL                             ;        /* Offset = 0x00C */
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
    uint32_t    RESTRICTED5[2]                     ;        /* Offset = 0x06C */
    uint32_t    DFTREG5                            ;        /* Offset = 0x074 */
    uint32_t    RESTRICTED6[22]                    ;        /* Offset = 0x078 */
    uint32_t    DSSIOCTL                           ;        /* Offset = 0x0D0 */
    uint32_t    RESTRICTED7[6]                     ;        /* Offset = 0x0D4 */
    uint32_t    SPARE0                             ;        /* Offset = 0x0EC */
    uint32_t    SPARE1                             ;        /* Offset = 0x0F0 */
    uint32_t    SPARE2                             ;        /* Offset = 0x0F4 */
    uint32_t    SPARE3                             ;        /* Offset = 0x0F8 */
    uint32_t    MSS_SIGNATURE                      ;        /* Offset = 0x0FC */
    uint32_t    RESTRICTED8[5]                     ;        /* Offset = 0x100 */
    uint32_t    EFUSEREGROW10                      ;        /* Offset = 0x114 */
    uint32_t    RESTRICTED9[16]                    ;        /* Offset = 0x118 */
    uint32_t    GEMBOOTSTCEN                       ;        /* Offset = 0x158 */
    uint32_t    RESTRICTED10[9]                    ;        /* Offset = 0x15C */
    uint32_t    USERMODEEN2                        ;        /* Offset = 0x180 */
    uint32_t    RESTRICTED11[2]                    ;        /* Offset = 0x184 */
    uint32_t    SYSTICK                            ;        /* Offset = 0x18C */
    uint32_t    RESTRICTED12[13]                   ;        /* Offset = 0x190 */
    uint32_t    SECURECFGREG1                      ;        /* Offset = 0x1C4 */
    uint32_t    SECURECFGREG2                      ;        /* Offset = 0x1C8 */
    uint32_t    SECURECFGREG3                      ;        /* Offset = 0x1CC */
    uint32_t    SECURECFGREG4                      ;        /* Offset = 0x1D0 */
    uint32_t    SECURERAMREG                       ;        /* Offset = 0x1D4 */
    uint32_t    RESTRICTED13[3]                    ;        /* Offset = 0x1D8 */
    uint32_t    SPAREMULTIBIT                      ;        /* Offset = 0x1E4 */
    uint32_t    SPARE4                             ;        /* Offset = 0x1E8 */
    uint32_t    SPARE5                             ;        /* Offset = 0x1EC */
    uint32_t    SPARE6                             ;        /* Offset = 0x1F0 */
    uint32_t    SPARE7                             ;        /* Offset = 0x1F4 */
    uint32_t    SPARE8                             ;        /* Offset = 0x1F8 */
    uint32_t    SPARE9                             ;        /* Offset = 0x1FC */
    uint32_t    UID31TO0                           ;        /* Offset = 0x200 */
    uint32_t    UID63TO32                          ;        /* Offset = 0x204 */
    uint32_t    UID95TO64                          ;        /* Offset = 0x208 */
    uint32_t    UID119TO96                         ;        /* Offset = 0x20C */
    uint32_t    RESTRICTED14[38]                   ;        /* Offset = 0x210 */
    uint32_t    MEMINITSTARTSHMEM                  ;        /* Offset = 0x2A8 */
    uint32_t    MEMINITDONESHMEM                   ;        /* Offset = 0x2AC */
    uint32_t    DSSMEMTAB0                         ;        /* Offset = 0x2B0 */
    uint32_t    RESTRICTED15[2]                    ;        /* Offset = 0x2B4 */
    uint32_t    TCMAMEMTAB                         ;        /* Offset = 0x2BC */
    uint32_t    TCMBMEMTAB                         ;        /* Offset = 0x2C0 */
    uint32_t    RESTRICTED16                       ;        /* Offset = 0x2C4 */
    uint32_t    SHMEMBANKSEL3TO0                   ;        /* Offset = 0x2C8 */
    uint32_t    SHMEMBANKSEL7TO4                   ;        /* Offset = 0x2CC */
    uint32_t    PBISTCLKCTL                        ;        /* Offset = 0x2D0 */
} TOPRCMRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_TOPRCM_H */
/* END OF REG_TOPRCM_H */

