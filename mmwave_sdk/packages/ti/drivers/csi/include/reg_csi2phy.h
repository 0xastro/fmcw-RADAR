/**
 *  @file    reg_csi2phy.h
 *
 *  @brief
 *    This file gives register definitions of CSI2_PHY module.
 *
 *  This file is auto-generated on 3/22/2017.
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

#ifndef REG_CSI2PHY_H
#define REG_CSI2PHY_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field REG_THSEXIT in Register REGISTER0 */
#define REGISTER0_REG_THSEXIT_BIT_START                     0U
#define REGISTER0_REG_THSEXIT_BIT_END                       7U

/* Definition for field REG_THSTRAIL in Register REGISTER0 */
#define REGISTER0_REG_THSTRAIL_BIT_START                    8U
#define REGISTER0_REG_THSTRAIL_BIT_END                      15U

/* Definition for field REG_THSPRPR_THSZERO in Register REGISTER0 */
#define REGISTER0_REG_THSPRPR_THSZERO_BIT_START             16U
#define REGISTER0_REG_THSPRPR_THSZERO_BIT_END               23U

/* Definition for field REG_THSPREPARE in Register REGISTER0 */
#define REGISTER0_REG_THSPREPARE_BIT_START                  24U
#define REGISTER0_REG_THSPREPARE_BIT_END                    31U

/* Definition for field REG_TCLKZERO in Register REGISTER1 */
#define REGISTER1_REG_TCLKZERO_BIT_START                    0U
#define REGISTER1_REG_TCLKZERO_BIT_END                      7U

/* Definition for field REG_TCLKTRAIL in Register REGISTER1 */
#define REGISTER1_REG_TCLKTRAIL_BIT_START                   8U
#define REGISTER1_REG_TCLKTRAIL_BIT_END                     15U

/* Definition for field REG_TLPXBY2 in Register REGISTER1 */
#define REGISTER1_REG_TLPXBY2_BIT_START                     16U
#define REGISTER1_REG_TLPXBY2_BIT_END                       20U

/* Definition for field EMPTY in Register REGISTER1 */
#define REGISTER1_EMPTY_BIT_START                           21U
#define REGISTER1_EMPTY_BIT_END                             23U

/* Definition for field REG_TTAGET in Register REGISTER1 */
#define REGISTER1_REG_TTAGET_BIT_START                      24U
#define REGISTER1_REG_TTAGET_BIT_END                        26U

/* Definition for field REG_TTASURE in Register REGISTER1 */
#define REGISTER1_REG_TTASURE_BIT_START                     27U
#define REGISTER1_REG_TTASURE_BIT_END                       28U

/* Definition for field REG_TTAGO in Register REGISTER1 */
#define REGISTER1_REG_TTAGO_BIT_START                       29U
#define REGISTER1_REG_TTAGO_BIT_END                         31U

/* Definition for field REG_TCLKPREPARE in Register REGISTER2 */
#define REGISTER2_REG_TCLKPREPARE_BIT_START                 0U
#define REGISTER2_REG_TCLKPREPARE_BIT_END                   7U

/* Definition for field EMPTY in Register REGISTER2 */
#define REGISTER2_EMPTY_BIT_START                           8U
#define REGISTER2_EMPTY_BIT_END                             10U

/* Definition for field REGULPMTX in Register REGISTER2 */
#define REGISTER2_REGULPMTX_BIT_START                       11U
#define REGISTER2_REGULPMTX_BIT_END                         15U

/* Definition for field OVRRDULPMTX in Register REGISTER2 */
#define REGISTER2_OVRRDULPMTX_BIT_START                     16U
#define REGISTER2_OVRRDULPMTX_BIT_END                       16U

/* Definition for field REGLPTXGZ in Register REGISTER2 */
#define REGISTER2_REGLPTXGZ_BIT_START                       17U
#define REGISTER2_REGLPTXGZ_BIT_END                         21U

/* Definition for field OVRRDLPTXGZ in Register REGISTER2 */
#define REGISTER2_OVRRDLPTXGZ_BIT_START                     22U
#define REGISTER2_OVRRDLPTXGZ_BIT_END                       22U

/* Definition for field DATARATE in Register REGISTER2 */
#define REGISTER2_DATARATE_BIT_START                        23U
#define REGISTER2_DATARATE_BIT_END                          23U

/* Definition for field HSSYNCPATTERN in Register REGISTER2 */
#define REGISTER2_HSSYNCPATTERN_BIT_START                   24U
#define REGISTER2_HSSYNCPATTERN_BIT_END                     31U

/* Definition for field REG_TXTRIGGERESC0 in Register REGISTER3 */
#define REGISTER3_REG_TXTRIGGERESC0_BIT_START               0U
#define REGISTER3_REG_TXTRIGGERESC0_BIT_END                 7U

/* Definition for field REG_TXTRIGGERESC1 in Register REGISTER3 */
#define REGISTER3_REG_TXTRIGGERESC1_BIT_START               8U
#define REGISTER3_REG_TXTRIGGERESC1_BIT_END                 15U

/* Definition for field REG_TXTRIGGERESC2 in Register REGISTER3 */
#define REGISTER3_REG_TXTRIGGERESC2_BIT_START               16U
#define REGISTER3_REG_TXTRIGGERESC2_BIT_END                 23U

/* Definition for field REG_TXTRIGGERESC3 in Register REGISTER3 */
#define REGISTER3_REG_TXTRIGGERESC3_BIT_START               24U
#define REGISTER3_REG_TXTRIGGERESC3_BIT_END                 31U

/* Definition for field REG_RXTRIGGERESC0 in Register REGISTER4 */
#define REGISTER4_REG_RXTRIGGERESC0_BIT_START               0U
#define REGISTER4_REG_RXTRIGGERESC0_BIT_END                 7U

/* Definition for field REG_RXTRIGGERESC1 in Register REGISTER4 */
#define REGISTER4_REG_RXTRIGGERESC1_BIT_START               8U
#define REGISTER4_REG_RXTRIGGERESC1_BIT_END                 15U

/* Definition for field REG_RXTRIGGERESC2 in Register REGISTER4 */
#define REGISTER4_REG_RXTRIGGERESC2_BIT_START               16U
#define REGISTER4_REG_RXTRIGGERESC2_BIT_END                 23U

/* Definition for field REG_RXTRIGGERESC3 in Register REGISTER4 */
#define REGISTER4_REG_RXTRIGGERESC3_BIT_START               24U
#define REGISTER4_REG_RXTRIGGERESC3_BIT_END                 31U

/* Definition for field EMPTY in Register REGISTER5 */
#define REGISTER5_EMPTY_BIT_START                           0U
#define REGISTER5_EMPTY_BIT_END                             23U

/* Definition for field RESETDONETXCLKESC3 in Register REGISTER5 */
#define REGISTER5_RESETDONETXCLKESC3_BIT_START              27U
#define REGISTER5_RESETDONETXCLKESC3_BIT_END                27U

/* Definition for field RESETDONEPWRCLK in Register REGISTER5 */
#define REGISTER5_RESETDONEPWRCLK_BIT_START                 29U
#define REGISTER5_RESETDONEPWRCLK_BIT_END                   29U

/* Definition for field RESETDONESCPCLK in Register REGISTER5 */
#define REGISTER5_RESETDONESCPCLK_BIT_START                 30U
#define REGISTER5_RESETDONESCPCLK_BIT_END                   30U

/* Definition for field RESETDONETXBYTECLK in Register REGISTER5 */
#define REGISTER5_RESETDONETXBYTECLK_BIT_START              31U
#define REGISTER5_RESETDONETXBYTECLK_BIT_END                31U

/* Definition for field RESETDONETXCLKESC0 in Register REGISTER5 */
#define REGISTER5_RESETDONETXCLKESC0_BIT_START              24U
#define REGISTER5_RESETDONETXCLKESC0_BIT_END                24U

/* Definition for field RESETDONETXCLKESC1 in Register REGISTER5 */
#define REGISTER5_RESETDONETXCLKESC1_BIT_START              25U
#define REGISTER5_RESETDONETXCLKESC1_BIT_END                25U

/* Definition for field RESETDONETXCLKESC2 in Register REGISTER5 */
#define REGISTER5_RESETDONETXCLKESC2_BIT_START              26U
#define REGISTER5_RESETDONETXCLKESC2_BIT_END                26U

/* Definition for field RESETDONETXCLKESC4 in Register REGISTER5 */
#define REGISTER5_RESETDONETXCLKESC4_BIT_START              28U
#define REGISTER5_RESETDONETXCLKESC4_BIT_END                28U

/* Definition for field BYPASSCOMP in Register REGISTER6 */
#define REGISTER6_BYPASSCOMP_BIT_START                      0U
#define REGISTER6_BYPASSCOMP_BIT_END                        0U

/* Definition for field BYPASSCOMPFILT in Register REGISTER6 */
#define REGISTER6_BYPASSCOMPFILT_BIT_START                  1U
#define REGISTER6_BYPASSCOMPFILT_BIT_END                    1U

/* Definition for field EMPTY in Register REGISTER6 */
#define REGISTER6_EMPTY_BIT_START                           2U
#define REGISTER6_EMPTY_BIT_END                             3U

/* Definition for field REGLPTXEN in Register REGISTER6 */
#define REGISTER6_REGLPTXEN_BIT_START                       4U
#define REGISTER6_REGLPTXEN_BIT_END                         8U

/* Definition for field OVRRDLPTXEN in Register REGISTER6 */
#define REGISTER6_OVRRDLPTXEN_BIT_START                     9U
#define REGISTER6_OVRRDLPTXEN_BIT_END                       9U

/* Definition for field REGDEEMPDISABLE in Register REGISTER6 */
#define REGISTER6_REGDEEMPDISABLE_BIT_START                 10U
#define REGISTER6_REGDEEMPDISABLE_BIT_END                   14U

/* Definition for field OVRRDDEEMPDISABLE in Register REGISTER6 */
#define REGISTER6_OVRRDDEEMPDISABLE_BIT_START               15U
#define REGISTER6_OVRRDDEEMPDISABLE_BIT_END                 15U

/* Definition for field REGCLKLANEADDR in Register REGISTER6 */
#define REGISTER6_REGCLKLANEADDR_BIT_START                  16U
#define REGISTER6_REGCLKLANEADDR_BIT_END                    18U

/* Definition for field OVRRDCLKLANEADDR in Register REGISTER6 */
#define REGISTER6_OVRRDCLKLANEADDR_BIT_START                19U
#define REGISTER6_OVRRDCLKLANEADDR_BIT_END                  19U

/* Definition for field REGHSTXTERMEN in Register REGISTER6 */
#define REGISTER6_REGHSTXTERMEN_BIT_START                   20U
#define REGISTER6_REGHSTXTERMEN_BIT_END                     24U

/* Definition for field OVRRDHSTXTERMEN in Register REGISTER6 */
#define REGISTER6_OVRRDHSTXTERMEN_BIT_START                 25U
#define REGISTER6_OVRRDHSTXTERMEN_BIT_END                   25U

/* Definition for field REGHSTXEN in Register REGISTER6 */
#define REGISTER6_REGHSTXEN_BIT_START                       26U
#define REGISTER6_REGHSTXEN_BIT_END                         30U

/* Definition for field OVRRDHSTXEN in Register REGISTER6 */
#define REGISTER6_OVRRDHSTXEN_BIT_START                     31U
#define REGISTER6_OVRRDHSTXEN_BIT_END                       31U

/* Definition for field OVRRDBIASGENTRIMMODE in Register REGISTER7 */
#define REGISTER7_OVRRDBIASGENTRIMMODE_BIT_START            9U
#define REGISTER7_OVRRDBIASGENTRIMMODE_BIT_END              9U

/* Definition for field REGHSLDOOBSERVE in Register REGISTER7 */
#define REGISTER7_REGHSLDOOBSERVE_BIT_START                 10U
#define REGISTER7_REGHSLDOOBSERVE_BIT_END                   10U

/* Definition for field OVRRDHSLDOOBSERVE in Register REGISTER7 */
#define REGISTER7_OVRRDHSLDOOBSERVE_BIT_START               11U
#define REGISTER7_OVRRDHSLDOOBSERVE_BIT_END                 11U

/* Definition for field REGLDOVDDTRACKING in Register REGISTER7 */
#define REGISTER7_REGLDOVDDTRACKING_BIT_START               12U
#define REGISTER7_REGLDOVDDTRACKING_BIT_END                 12U

/* Definition for field REGULPRXEN in Register REGISTER7 */
#define REGISTER7_REGULPRXEN_BIT_START                      14U
#define REGISTER7_REGULPRXEN_BIT_END                        18U

/* Definition for field OVRRDULPRXEN in Register REGISTER7 */
#define REGISTER7_OVRRDULPRXEN_BIT_START                    19U
#define REGISTER7_OVRRDULPRXEN_BIT_END                      19U

/* Definition for field REGLPCDEN in Register REGISTER7 */
#define REGISTER7_REGLPCDEN_BIT_START                       20U
#define REGISTER7_REGLPCDEN_BIT_END                         24U

/* Definition for field OVRRDLPCDEN in Register REGISTER7 */
#define REGISTER7_OVRRDLPCDEN_BIT_START                     25U
#define REGISTER7_OVRRDLPCDEN_BIT_END                       25U

/* Definition for field REGLPRXEN in Register REGISTER7 */
#define REGISTER7_REGLPRXEN_BIT_START                       26U
#define REGISTER7_REGLPRXEN_BIT_END                         30U

/* Definition for field OVRRDLDOVDDTRACKING in Register REGISTER7 */
#define REGISTER7_OVRRDLDOVDDTRACKING_BIT_START             13U
#define REGISTER7_OVRRDLDOVDDTRACKING_BIT_END               13U

/* Definition for field OVRRDLPRXEN in Register REGISTER7 */
#define REGISTER7_OVRRDLPRXEN_BIT_START                     31U
#define REGISTER7_OVRRDLPRXEN_BIT_END                       31U

/* Definition for field REGBIASGENTRIMMODE in Register REGISTER7 */
#define REGISTER7_REGBIASGENTRIMMODE_BIT_START              8U
#define REGISTER7_REGBIASGENTRIMMODE_BIT_END                8U

/* Definition for field REGHSTXCOREEN in Register REGISTER7 */
#define REGISTER7_REGHSTXCOREEN_BIT_START                   2U
#define REGISTER7_REGHSTXCOREEN_BIT_END                     6U

/* Definition for field OVRRDHSTXCOREEN in Register REGISTER7 */
#define REGISTER7_OVRRDHSTXCOREEN_BIT_START                 7U
#define REGISTER7_OVRRDHSTXCOREEN_BIT_END                   7U

/* Definition for field EMPTY in Register REGISTER7 */
#define REGISTER7_EMPTY_BIT_START                           0U
#define REGISTER7_EMPTY_BIT_END                             1U

/* Definition for field BIASGENCODE in Register REGISTER8 */
#define REGISTER8_BIASGENCODE_BIT_START                     0U
#define REGISTER8_BIASGENCODE_BIT_END                       4U

/* Definition for field OVRRDEFUSEBIASGEN in Register REGISTER8 */
#define REGISTER8_OVRRDEFUSEBIASGEN_BIT_START               5U
#define REGISTER8_OVRRDEFUSEBIASGEN_BIT_END                 5U

/* Definition for field REGHSTXTERMRES in Register REGISTER8 */
#define REGISTER8_REGHSTXTERMRES_BIT_START                  6U
#define REGISTER8_REGHSTXTERMRES_BIT_END                    10U

/* Definition for field OVRRDHSTXTERMRES in Register REGISTER8 */
#define REGISTER8_OVRRDHSTXTERMRES_BIT_START                11U
#define REGISTER8_OVRRDHSTXTERMRES_BIT_END                  11U

/* Definition for field REGLPTXIMPBYPASS in Register REGISTER9 */
#define REGISTER9_REGLPTXIMPBYPASS_BIT_START                5U
#define REGISTER9_REGLPTXIMPBYPASS_BIT_END                  5U

/* Definition for field ENLPTXIMPBYPASS in Register REGISTER9 */
#define REGISTER9_ENLPTXIMPBYPASS_BIT_START                 6U
#define REGISTER9_ENLPTXIMPBYPASS_BIT_END                   6U

/* Definition for field REGVREGBIASCURR in Register REGISTER9 */
#define REGISTER9_REGVREGBIASCURR_BIT_START                 7U
#define REGISTER9_REGVREGBIASCURR_BIT_END                   8U

/* Definition for field REGVREGLOAD in Register REGISTER9 */
#define REGISTER9_REGVREGLOAD_BIT_START                     9U
#define REGISTER9_REGVREGLOAD_BIT_END                       10U

/* Definition for field ENVREGCONTROL in Register REGISTER9 */
#define REGISTER9_ENVREGCONTROL_BIT_START                   11U
#define REGISTER9_ENVREGCONTROL_BIT_END                     11U

/* Definition for field REGBIASGENTESTMODES in Register REGISTER9 */
#define REGISTER9_REGBIASGENTESTMODES_BIT_START             12U
#define REGISTER9_REGBIASGENTESTMODES_BIT_END               13U

/* Definition for field ENBIASGENCONTROL in Register REGISTER9 */
#define REGISTER9_ENBIASGENCONTROL_BIT_START                14U
#define REGISTER9_ENBIASGENCONTROL_BIT_END                  14U

/* Definition for field REGLDOVOLTAGE in Register REGISTER9 */
#define REGISTER9_REGLDOVOLTAGE_BIT_START                   15U
#define REGISTER9_REGLDOVOLTAGE_BIT_END                     18U

/* Definition for field ENLDOVOLTAGECONTROL in Register REGISTER9 */
#define REGISTER9_ENLDOVOLTAGECONTROL_BIT_START             19U
#define REGISTER9_ENLDOVOLTAGECONTROL_BIT_END               19U

/* Definition for field ENBIASGENCURROUT in Register REGISTER9 */
#define REGISTER9_ENBIASGENCURROUT_BIT_START                20U
#define REGISTER9_ENBIASGENCURROUT_BIT_END                  20U

/* Definition for field REGBANDGAPEN in Register REGISTER9 */
#define REGISTER9_REGBANDGAPEN_BIT_START                    21U
#define REGISTER9_REGBANDGAPEN_BIT_END                      21U

/* Definition for field OVRRDBANDGAPEN in Register REGISTER9 */
#define REGISTER9_OVRRDBANDGAPEN_BIT_START                  22U
#define REGISTER9_OVRRDBANDGAPEN_BIT_END                    22U

/* Definition for field REGBIASGENEN in Register REGISTER9 */
#define REGISTER9_REGBIASGENEN_BIT_START                    23U
#define REGISTER9_REGBIASGENEN_BIT_END                      23U

/* Definition for field OVRRDBIASGENEN in Register REGISTER9 */
#define REGISTER9_OVRRDBIASGENEN_BIT_START                  24U
#define REGISTER9_OVRRDBIASGENEN_BIT_END                    24U

/* Definition for field EMPTY in Register REGISTER9 */
#define REGISTER9_EMPTY_BIT_START                           25U
#define REGISTER9_EMPTY_BIT_END                             25U

/* Definition for field REGPOLARITY3TO0 in Register REGISTER9 */
#define REGISTER9_REGPOLARITY3TO0_BIT_START                 26U
#define REGISTER9_REGPOLARITY3TO0_BIT_END                   30U

/* Definition for field OVRRDPOLARITY in Register REGISTER9 */
#define REGISTER9_OVRRDPOLARITY_BIT_START                   31U
#define REGISTER9_OVRRDPOLARITY_BIT_END                     31U

/* Definition for field REGBYPASSACKZ in Register REGISTER9 */
#define REGISTER9_REGBYPASSACKZ_BIT_START                   0U
#define REGISTER9_REGBYPASSACKZ_BIT_END                     0U

/* Definition for field REGCLKINEN in Register REGISTER9 */
#define REGISTER9_REGCLKINEN_BIT_START                      2U
#define REGISTER9_REGCLKINEN_BIT_END                        2U

/* Definition for field REGBYPASSEN in Register REGISTER9 */
#define REGISTER9_REGBYPASSEN_BIT_START                     3U
#define REGISTER9_REGBYPASSEN_BIT_END                       3U

/* Definition for field OVRRDCLKIN4DDRSIGNALS in Register REGISTER9 */
#define REGISTER9_OVRRDCLKIN4DDRSIGNALS_BIT_START           4U
#define REGISTER9_OVRRDCLKIN4DDRSIGNALS_BIT_END             4U

/* Definition for field REGCLKIN4DDRGODDBAR in Register REGISTER9 */
#define REGISTER9_REGCLKIN4DDRGODDBAR_BIT_START             1U
#define REGISTER9_REGCLKIN4DDRGODDBAR_BIT_END               1U

/* Definition for field REGHSTXSCPDAT4TO0 in Register REGISTER10 */
#define REGISTER10_REGHSTXSCPDAT4TO0_BIT_START              11U
#define REGISTER10_REGHSTXSCPDAT4TO0_BIT_END                15U

/* Definition for field ENHSTXSCPDAT in Register REGISTER10 */
#define REGISTER10_ENHSTXSCPDAT_BIT_START                   16U
#define REGISTER10_ENHSTXSCPDAT_BIT_END                     16U

/* Definition for field REGLPTXSCPDAT4TO0DXDY in Register REGISTER10 */
#define REGISTER10_REGLPTXSCPDAT4TO0DXDY_BIT_START          17U
#define REGISTER10_REGLPTXSCPDAT4TO0DXDY_BIT_END            26U

/* Definition for field ENLPTXSCPDAT in Register REGISTER10 */
#define REGISTER10_ENLPTXSCPDAT_BIT_START                   27U
#define REGISTER10_ENLPTXSCPDAT_BIT_END                     27U

/* Definition for field LPPOWERUPTIME in Register REGISTER10 */
#define REGISTER10_LPPOWERUPTIME_BIT_START                  28U
#define REGISTER10_LPPOWERUPTIME_BIT_END                    29U

/* Definition for field LDOWAKEUPTIME in Register REGISTER10 */
#define REGISTER10_LDOWAKEUPTIME_BIT_START                  30U
#define REGISTER10_LDOWAKEUPTIME_BIT_END                    31U

/* Definition for field EMPTY in Register REGISTER10 */
#define REGISTER10_EMPTY_BIT_START                          0U
#define REGISTER10_EMPTY_BIT_END                            3U

/* Definition for field REGLDOEN in Register REGISTER10 */
#define REGISTER10_REGLDOEN_BIT_START                       4U
#define REGISTER10_REGLDOEN_BIT_END                         9U

/* Definition for field OVRRDLDOEN in Register REGISTER10 */
#define REGISTER10_OVRRDLDOEN_BIT_START                     10U
#define REGISTER10_OVRRDLDOEN_BIT_END                       10U

/* Definition for field LOOPBACKDATABYTE0 in Register REGISTER11 */
#define REGISTER11_LOOPBACKDATABYTE0_BIT_START              0U
#define REGISTER11_LOOPBACKDATABYTE0_BIT_END                7U

/* Definition for field LOOPBACKDATABYTE1 in Register REGISTER11 */
#define REGISTER11_LOOPBACKDATABYTE1_BIT_START              8U
#define REGISTER11_LOOPBACKDATABYTE1_BIT_END                15U

/* Definition for field LOOPBACKDATABYTE2 in Register REGISTER11 */
#define REGISTER11_LOOPBACKDATABYTE2_BIT_START              16U
#define REGISTER11_LOOPBACKDATABYTE2_BIT_END                23U

/* Definition for field LOOPBACKDATABYTE3 in Register REGISTER11 */
#define REGISTER11_LOOPBACKDATABYTE3_BIT_START              24U
#define REGISTER11_LOOPBACKDATABYTE3_BIT_END                31U

/* Definition for field BGTRIMBITS in Register REGISTER12 */
#define REGISTER12_BGTRIMBITS_BIT_START                     0U
#define REGISTER12_BGTRIMBITS_BIT_END                       7U

/* Definition for field EMPTY in Register REGISTER12 */
#define REGISTER12_EMPTY_BIT_START                          8U
#define REGISTER12_EMPTY_BIT_END                            9U

/* Definition for field REGLANEENABLE in Register REGISTER12 */
#define REGISTER12_REGLANEENABLE_BIT_START                  10U
#define REGISTER12_REGLANEENABLE_BIT_END                    14U

/* Definition for field OVRRDLANEENABLE in Register REGISTER12 */
#define REGISTER12_OVRRDLANEENABLE_BIT_START                15U
#define REGISTER12_OVRRDLANEENABLE_BIT_END                  15U

/* Definition for field TCLKPOST in Register REGISTER12 */
#define REGISTER12_TCLKPOST_BIT_START                       16U
#define REGISTER12_TCLKPOST_BIT_END                         23U

/* Definition for field TCLKPRE in Register REGISTER12 */
#define REGISTER12_TCLKPRE_BIT_START                        24U
#define REGISTER12_TCLKPRE_BIT_END                          31U

/* Definition for field ANALOGTESTMODES in Register REGISTER13 */
#define REGISTER13_ANALOGTESTMODES_BIT_START                8U
#define REGISTER13_ANALOGTESTMODES_BIT_END                  31U

/* Definition for field EMPTY in Register REGISTER13 */
#define REGISTER13_EMPTY_BIT_START                          0U
#define REGISTER13_EMPTY_BIT_END                            7U

/* Definition for field EMPTY in Register REGISTER14 */
#define REGISTER14_EMPTY_BIT_START                          0U
#define REGISTER14_EMPTY_BIT_END                            5U

/* Definition for field REGHSTXDELAYSLAVECTRL in Register REGISTER14 */
#define REGISTER14_REGHSTXDELAYSLAVECTRL_BIT_START          6U
#define REGISTER14_REGHSTXDELAYSLAVECTRL_BIT_END            9U

/* Definition for field OVRRDHSTXDELAYSLAVECTRL in Register REGISTER14 */
#define REGISTER14_OVRRDHSTXDELAYSLAVECTRL_BIT_START        10U
#define REGISTER14_OVRRDHSTXDELAYSLAVECTRL_BIT_END          10U

/* Definition for field OVRRDEFUSEBG in Register REGISTER14 */
#define REGISTER14_OVRRDEFUSEBG_BIT_START                   11U
#define REGISTER14_OVRRDEFUSEBG_BIT_END                     11U

/* Definition for field LPRXSAMPLEOUTEN in Register REGISTER14 */
#define REGISTER14_LPRXSAMPLEOUTEN_BIT_START                12U
#define REGISTER14_LPRXSAMPLEOUTEN_BIT_END                  12U

/* Definition for field LPCDSAMPLEOUTEN in Register REGISTER14 */
#define REGISTER14_LPCDSAMPLEOUTEN_BIT_START                13U
#define REGISTER14_LPCDSAMPLEOUTEN_BIT_END                  13U

/* Definition for field REGHSTXDELAYMASTERCTRL in Register REGISTER14 */
#define REGISTER14_REGHSTXDELAYMASTERCTRL_BIT_START         14U
#define REGISTER14_REGHSTXDELAYMASTERCTRL_BIT_END           17U

/* Definition for field OVRRDHSTXDELAYMASTERCTRL in Register REGISTER14 */
#define REGISTER14_OVRRDHSTXDELAYMASTERCTRL_BIT_START       18U
#define REGISTER14_OVRRDHSTXDELAYMASTERCTRL_BIT_END         18U

/* Definition for field REGHSDELAYCALIBCLRZ in Register REGISTER14 */
#define REGISTER14_REGHSDELAYCALIBCLRZ_BIT_START            19U
#define REGISTER14_REGHSDELAYCALIBCLRZ_BIT_END              19U

/* Definition for field OVRRDHSDELAYCALIBCLRZ in Register REGISTER14 */
#define REGISTER14_OVRRDHSDELAYCALIBCLRZ_BIT_START          20U
#define REGISTER14_OVRRDHSDELAYCALIBCLRZ_BIT_END            20U

/* Definition for field REGHSDELAYCALIBEN in Register REGISTER14 */
#define REGISTER14_REGHSDELAYCALIBEN_BIT_START              21U
#define REGISTER14_REGHSDELAYCALIBEN_BIT_END                21U

/* Definition for field OVRRDHSDELAYCALIBEN in Register REGISTER14 */
#define REGISTER14_OVRRDHSDELAYCALIBEN_BIT_START            22U
#define REGISTER14_OVRRDHSDELAYCALIBEN_BIT_END              22U

/* Definition for field REGBGCONTROL in Register REGISTER14 */
#define REGISTER14_REGBGCONTROL_BIT_START                   23U
#define REGISTER14_REGBGCONTROL_BIT_END                     30U

/* Definition for field OVRRDBGCONTROL in Register REGISTER14 */
#define REGISTER14_OVRRDBGCONTROL_BIT_START                 31U
#define REGISTER14_OVRRDBGCONTROL_BIT_END                   31U

/* Definition for field REGNOTRANSITIONCTRL in Register REGISTER15 */
#define REGISTER15_REGNOTRANSITIONCTRL_BIT_START            0U
#define REGISTER15_REGNOTRANSITIONCTRL_BIT_END              2U

/* Definition for field REG_THSTXEN in Register REGISTER15 */
#define REGISTER15_REG_THSTXEN_BIT_START                    21U
#define REGISTER15_REG_THSTXEN_BIT_END                      24U

/* Definition for field REGLDOHIZEN in Register REGISTER15 */
#define REGISTER15_REGLDOHIZEN_BIT_START                    25U
#define REGISTER15_REGLDOHIZEN_BIT_END                      30U

/* Definition for field OVRRDLDOHIZEN in Register REGISTER15 */
#define REGISTER15_OVRRDLDOHIZEN_BIT_START                  31U
#define REGISTER15_OVRRDLDOHIZEN_BIT_END                    31U

/* Definition for field EMPTY in Register REGISTER15 */
#define REGISTER15_EMPTY_BIT_START                          9U
#define REGISTER15_EMPTY_BIT_END                            20U

/* Definition for field OVRRDNOTRANSITIONCTRL in Register REGISTER15 */
#define REGISTER15_OVRRDNOTRANSITIONCTRL_BIT_START          3U
#define REGISTER15_OVRRDNOTRANSITIONCTRL_BIT_END            3U

/* Definition for field NOTRANSITIONDISABLE in Register REGISTER15 */
#define REGISTER15_NOTRANSITIONDISABLE_BIT_START            4U
#define REGISTER15_NOTRANSITIONDISABLE_BIT_END              8U



/**
 * @struct CSI2PHYRegs_t
 * @brief
 *   Module CSI2_PHY Register Definition
 * @details
 *   This structure is used to access the CSI2_PHY module registers.
 */
/**
 * @typedef CSI2PHYRegs
 * @brief
 *   Module CSI2_PHY Register Frame type Definition
 * @details
 *   This type is used to access the CSI2_PHY module registers.
 */
typedef volatile struct CSI2PHYRegs_t
{
#if 0
    uint32_t    REGISTER0                          ;        /* Offset = 0x000 */
    uint32_t    REGISTER1                          ;        /* Offset = 0x004 */
    uint32_t    REGISTER2                          ;        /* Offset = 0x008 */
    uint32_t    REGISTER3                          ;        /* Offset = 0x00c */
    uint32_t    REGISTER4                          ;        /* Offset = 0x010 */
    uint32_t    REGISTER5                          ;        /* Offset = 0x014 */
    uint32_t    REGISTER6                          ;        /* Offset = 0x018 */
    uint32_t    REGISTER7                          ;        /* Offset = 0x01c */
    uint32_t    REGISTER8                          ;        /* Offset = 0x020 */
    uint32_t    REGISTER9                          ;        /* Offset = 0x024 */
    uint32_t    REGISTER10                         ;        /* Offset = 0x028 */
    uint32_t    REGISTER11                         ;        /* Offset = 0x02c */
    uint32_t    REGISTER12                         ;        /* Offset = 0x030 */
    uint32_t    REGISTER13                         ;        /* Offset = 0x034 */
    uint32_t    REGISTER14                         ;        /* Offset = 0x038 */
    uint32_t    REGISTER15                         ;        /* Offset = 0x03c */
#else
    uint32_t    REGISTER[16];
#endif
} CSI2PHYRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_CSI2PHY_H */
/* END OF REG_CSI2PHY_H */

