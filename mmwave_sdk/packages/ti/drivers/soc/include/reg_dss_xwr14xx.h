/**
 *  @file    reg_dss_xwr14xx.h
 *
 *  @brief
 *    This file gives register definitions of DSS_REG module.
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

#ifndef REG_DSS_H
#define REG_DSS_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field EVT0 in Register RTIEVENTCAPTURESEL */
#define RTIEVENTCAPTURESEL_EVT0_BIT_START                   0U
#define RTIEVENTCAPTURESEL_EVT0_BIT_END                     3U

/* Definition for field EVT1 in Register RTIEVENTCAPTURESEL */
#define RTIEVENTCAPTURESEL_EVT1_BIT_START                   16U
#define RTIEVENTCAPTURESEL_EVT1_BIT_END                     19U

/* Definition for field ADCBUFREALONLYMODE in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_START             2U
#define ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_END               2U

/* Definition for field ADCBUFIQSWAP in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFIQSWAP_BIT_START                   5U
#define ADCBUFCFG1_ADCBUFIQSWAP_BIT_END                     5U

/* Definition for field RX0EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX0EN_BIT_START                          6U
#define ADCBUFCFG1_RX0EN_BIT_END                            6U

/* Definition for field RX1EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX1EN_BIT_START                          7U
#define ADCBUFCFG1_RX1EN_BIT_END                            7U

/* Definition for field RX2EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX2EN_BIT_START                          8U
#define ADCBUFCFG1_RX2EN_BIT_END                            8U

/* Definition for field RX3EN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_RX3EN_BIT_START                          9U
#define ADCBUFCFG1_RX3EN_BIT_END                            9U

/* Definition for field ADCBUFWRITEMODE in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFWRITEMODE_BIT_START                12U
#define ADCBUFCFG1_ADCBUFWRITEMODE_BIT_END                  12U

/* Definition for field ADCBUFCONTMODEEN in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFCONTMODEEN_BIT_START               13U
#define ADCBUFCFG1_ADCBUFCONTMODEEN_BIT_END                 13U

/* Definition for field ADCBUFCONTSTRTPL in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFCONTSTRTPL_BIT_START               14U
#define ADCBUFCFG1_ADCBUFCONTSTRTPL_BIT_END                 14U

/* Definition for field ADCBUFCONTSTOPPL in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFCONTSTOPPL_BIT_START               15U
#define ADCBUFCFG1_ADCBUFCONTSTOPPL_BIT_END                 15U

/* Definition for field ADCBUFNUMCHRP in Register ADCBUFCFG1 */
#define ADCBUFCFG1_ADCBUFNUMCHRP_BIT_START                  17U
#define ADCBUFCFG1_ADCBUFNUMCHRP_BIT_END                    21U

/* Definition for field ADCBUFADDRX0 in Register ADCBUFCFG2 */
#define ADCBUFCFG2_ADCBUFADDRX0_BIT_START                   0U
#define ADCBUFCFG2_ADCBUFADDRX0_BIT_END                     9U

/* Definition for field ADCBUFADDRX1 in Register ADCBUFCFG2 */
#define ADCBUFCFG2_ADCBUFADDRX1_BIT_START                   16U
#define ADCBUFCFG2_ADCBUFADDRX1_BIT_END                     25U

/* Definition for field ADCBUFADDRX2 in Register ADCBUFCFG3 */
#define ADCBUFCFG3_ADCBUFADDRX2_BIT_START                   0U
#define ADCBUFCFG3_ADCBUFADDRX2_BIT_END                     9U

/* Definition for field ADCBUFADDRX3 in Register ADCBUFCFG3 */
#define ADCBUFCFG3_ADCBUFADDRX3_BIT_START                   16U
#define ADCBUFCFG3_ADCBUFADDRX3_BIT_END                     25U

/* Definition for field ADCBUFSAMPCNT in Register ADCBUFCFG4 */
#define ADCBUFCFG4_ADCBUFSAMPCNT_BIT_START                  0U
#define ADCBUFCFG4_ADCBUFSAMPCNT_BIT_END                    13U

/* Definition for field CQDATAWIDTH in Register CQCFG1 */
#define CQCFG1_CQDATAWIDTH_BIT_START                        0U
#define CQCFG1_CQDATAWIDTH_BIT_END                          1U

/* Definition for field CQ96BITPACKEN in Register CQCFG1 */
#define CQCFG1_CQ96BITPACKEN_BIT_START                      3U
#define CQCFG1_CQ96BITPACKEN_BIT_END                        3U

/* Definition for field CQ0BASEADDR in Register CQCFG1 */
#define CQCFG1_CQ0BASEADDR_BIT_START                        4U
#define CQCFG1_CQ0BASEADDR_BIT_END                          10U

/* Definition for field CQ1BASEADDR in Register CQCFG1 */
#define CQCFG1_CQ1BASEADDR_BIT_START                        16U
#define CQCFG1_CQ1BASEADDR_BIT_END                          22U

/* Definition for field CQ2BASEADDR in Register CQCFG1 */
#define CQCFG1_CQ2BASEADDR_BIT_START                        24U
#define CQCFG1_CQ2BASEADDR_BIT_END                          30U

/* Definition for field TPCCPARITYSTAT in Register TPCCPARSTATCFG */
#define TPCCPARSTATCFG_TPCCPARITYSTAT_BIT_START             0U
#define TPCCPARSTATCFG_TPCCPARITYSTAT_BIT_END               7U

/* Definition for field TPCCPARITYCLR in Register TPCCPARSTATCFG */
#define TPCCPARSTATCFG_TPCCPARITYCLR_BIT_START              8U
#define TPCCPARSTATCFG_TPCCPARITYCLR_BIT_END                8U

/* Definition for field TPCCPARITYEN in Register TPCCPARSTATCFG */
#define TPCCPARSTATCFG_TPCCPARITYEN_BIT_START               9U
#define TPCCPARSTATCFG_TPCCPARITYEN_BIT_END                 9U

/* Definition for field TPCCPARITYTSTEN in Register TPCCPARSTATCFG */
#define TPCCPARSTATCFG_TPCCPARITYTSTEN_BIT_START            10U
#define TPCCPARSTATCFG_TPCCPARITYTSTEN_BIT_END              10U

/* Definition for field CSI2TXPARITYSTAT in Register CSI2TXPARSTATCFG */
#define CSI2TXPARSTATCFG_CSI2TXPARITYSTAT_BIT_START         0U
#define CSI2TXPARSTATCFG_CSI2TXPARITYSTAT_BIT_END           6U

/* Definition for field CSI2TXPARITYCLR in Register CSI2TXPARSTATCFG */
#define CSI2TXPARSTATCFG_CSI2TXPARITYCLR_BIT_START          8U
#define CSI2TXPARSTATCFG_CSI2TXPARITYCLR_BIT_END            8U

/* Definition for field CSI2TXPARITYEN in Register CSI2TXPARSTATCFG */
#define CSI2TXPARSTATCFG_CSI2TXPARITYEN_BIT_START           9U
#define CSI2TXPARSTATCFG_CSI2TXPARITYEN_BIT_END             9U

/* Definition for field CSI2TXPARITYTSTEN in Register CSI2TXPARSTATCFG */
#define CSI2TXPARSTATCFG_CSI2TXPARITYTSTEN_BIT_START        10U
#define CSI2TXPARSTATCFG_CSI2TXPARITYTSTEN_BIT_END          10U

/* Definition for field CSIMIDLEREQ in Register CSICFG1 */
#define CSICFG1_CSIMIDLEREQ_BIT_START                       9U
#define CSICFG1_CSIMIDLEREQ_BIT_END                         9U

/* Definition for field CSILANEENABLE in Register CSICFG1 */
#define CSICFG1_CSILANEENABLE_BIT_START                     12U
#define CSICFG1_CSILANEENABLE_BIT_END                       16U

/* Definition for field CSISIDLEACK in Register CSICFG1 */
#define CSICFG1_CSISIDLEACK_BIT_START                       17U
#define CSICFG1_CSISIDLEACK_BIT_END                         17U

/* Definition for field CSIPIPD0 in Register CSICFG1 */
#define CSICFG1_CSIPIPD0_BIT_START                          19U
#define CSICFG1_CSIPIPD0_BIT_END                            19U

/* Definition for field CSIPIPD1 in Register CSICFG1 */
#define CSICFG1_CSIPIPD1_BIT_START                          20U
#define CSICFG1_CSIPIPD1_BIT_END                            20U

/* Definition for field CSIPIPD2 in Register CSICFG1 */
#define CSICFG1_CSIPIPD2_BIT_START                          21U
#define CSICFG1_CSIPIPD2_BIT_END                            21U

/* Definition for field CSIPIPD3 in Register CSICFG1 */
#define CSICFG1_CSIPIPD3_BIT_START                          22U
#define CSICFG1_CSIPIPD3_BIT_END                            22U

/* Definition for field CSIPIPD4 in Register CSICFG1 */
#define CSICFG1_CSIPIPD4_BIT_START                          23U
#define CSICFG1_CSIPIPD4_BIT_END                            23U

/* Definition for field TPTC0WRMPUSTADD0 in Register TPTC0WRMPUSTADD0 */
#define TPTC0WRMPUSTADD0_TPTC0WRMPUSTADD0_BIT_START         0U
#define TPTC0WRMPUSTADD0_TPTC0WRMPUSTADD0_BIT_END           31U

/* Definition for field TPTC0WRMPUSTADD1 in Register TPTC0WRMPUSTADD1 */
#define TPTC0WRMPUSTADD1_TPTC0WRMPUSTADD1_BIT_START         0U
#define TPTC0WRMPUSTADD1_TPTC0WRMPUSTADD1_BIT_END           31U

/* Definition for field TPTC0WRMPUSTADD2 in Register TPTC0WRMPUSTADD2 */
#define TPTC0WRMPUSTADD2_TPTC0WRMPUSTADD2_BIT_START         0U
#define TPTC0WRMPUSTADD2_TPTC0WRMPUSTADD2_BIT_END           31U

/* Definition for field TPTC0WRMPUSTADD3 in Register TPTC0WRMPUSTADD3 */
#define TPTC0WRMPUSTADD3_TPTC0WRMPUSTADD3_BIT_START         0U
#define TPTC0WRMPUSTADD3_TPTC0WRMPUSTADD3_BIT_END           31U

/* Definition for field TPTC0WRMPUSTADD4 in Register TPTC0WRMPUSTADD4 */
#define TPTC0WRMPUSTADD4_TPTC0WRMPUSTADD4_BIT_START         0U
#define TPTC0WRMPUSTADD4_TPTC0WRMPUSTADD4_BIT_END           31U

/* Definition for field TPTC0WRMPUSTADD5 in Register TPTC0WRMPUSTADD5 */
#define TPTC0WRMPUSTADD5_TPTC0WRMPUSTADD5_BIT_START         0U
#define TPTC0WRMPUSTADD5_TPTC0WRMPUSTADD5_BIT_END           31U

/* Definition for field TPTC0WRMPUSTADD6 in Register TPTC0WRMPUSTADD6 */
#define TPTC0WRMPUSTADD6_TPTC0WRMPUSTADD6_BIT_START         0U
#define TPTC0WRMPUSTADD6_TPTC0WRMPUSTADD6_BIT_END           31U

/* Definition for field TPTC0WRMPUSTADD7 in Register TPTC0WRMPUSTADD7 */
#define TPTC0WRMPUSTADD7_TPTC0WRMPUSTADD7_BIT_START         0U
#define TPTC0WRMPUSTADD7_TPTC0WRMPUSTADD7_BIT_END           31U

/* Definition for field TPTC0WRMPUENDADD0 in Register TPTC0WRMPUENDADD0 */
#define TPTC0WRMPUENDADD0_TPTC0WRMPUENDADD0_BIT_START       0U
#define TPTC0WRMPUENDADD0_TPTC0WRMPUENDADD0_BIT_END         31U

/* Definition for field TPTC0WRMPUENDADD1 in Register TPTC0WRMPUENDADD1 */
#define TPTC0WRMPUENDADD1_TPTC0WRMPUENDADD1_BIT_START       0U
#define TPTC0WRMPUENDADD1_TPTC0WRMPUENDADD1_BIT_END         31U

/* Definition for field TPTC0WRMPUENDADD2 in Register TPTC0WRMPUENDADD2 */
#define TPTC0WRMPUENDADD2_TPTC0WRMPUENDADD2_BIT_START       0U
#define TPTC0WRMPUENDADD2_TPTC0WRMPUENDADD2_BIT_END         31U

/* Definition for field TPTC0WRMPUENDADD3 in Register TPTC0WRMPUENDADD3 */
#define TPTC0WRMPUENDADD3_TPTC0WRMPUENDADD3_BIT_START       0U
#define TPTC0WRMPUENDADD3_TPTC0WRMPUENDADD3_BIT_END         31U

/* Definition for field TPTC0WRMPUENDADD4 in Register TPTC0WRMPUENDADD4 */
#define TPTC0WRMPUENDADD4_TPTC0WRMPUENDADD4_BIT_START       0U
#define TPTC0WRMPUENDADD4_TPTC0WRMPUENDADD4_BIT_END         31U

/* Definition for field TPTC0WRMPUENDADD5 in Register TPTC0WRMPUENDADD5 */
#define TPTC0WRMPUENDADD5_TPTC0WRMPUENDADD5_BIT_START       0U
#define TPTC0WRMPUENDADD5_TPTC0WRMPUENDADD5_BIT_END         31U

/* Definition for field TPTC0WRMPUENDADD6 in Register TPTC0WRMPUENDADD6 */
#define TPTC0WRMPUENDADD6_TPTC0WRMPUENDADD6_BIT_START       0U
#define TPTC0WRMPUENDADD6_TPTC0WRMPUENDADD6_BIT_END         31U

/* Definition for field TPTC0WRMPUENDADD7 in Register TPTC0WRMPUENDADD7 */
#define TPTC0WRMPUENDADD7_TPTC0WRMPUENDADD7_BIT_START       0U
#define TPTC0WRMPUENDADD7_TPTC0WRMPUENDADD7_BIT_END         31U

/* Definition for field TPTC0WRMPUERRADD in Register TPTC0WRMPUERRADD */
#define TPTC0WRMPUERRADD_TPTC0WRMPUERRADD_BIT_START         0U
#define TPTC0WRMPUERRADD_TPTC0WRMPUERRADD_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD0 in Register TPTC0RDMPUSTADD0 */
#define TPTC0RDMPUSTADD0_TPTC0RDMPUSTADD0_BIT_START         0U
#define TPTC0RDMPUSTADD0_TPTC0RDMPUSTADD0_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD1 in Register TPTC0RDMPUSTADD1 */
#define TPTC0RDMPUSTADD1_TPTC0RDMPUSTADD1_BIT_START         0U
#define TPTC0RDMPUSTADD1_TPTC0RDMPUSTADD1_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD2 in Register TPTC0RDMPUSTADD2 */
#define TPTC0RDMPUSTADD2_TPTC0RDMPUSTADD2_BIT_START         0U
#define TPTC0RDMPUSTADD2_TPTC0RDMPUSTADD2_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD3 in Register TPTC0RDMPUSTADD3 */
#define TPTC0RDMPUSTADD3_TPTC0RDMPUSTADD3_BIT_START         0U
#define TPTC0RDMPUSTADD3_TPTC0RDMPUSTADD3_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD4 in Register TPTC0RDMPUSTADD4 */
#define TPTC0RDMPUSTADD4_TPTC0RDMPUSTADD4_BIT_START         0U
#define TPTC0RDMPUSTADD4_TPTC0RDMPUSTADD4_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD5 in Register TPTC0RDMPUSTADD5 */
#define TPTC0RDMPUSTADD5_TPTC0RDMPUSTADD5_BIT_START         0U
#define TPTC0RDMPUSTADD5_TPTC0RDMPUSTADD5_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD6 in Register TPTC0RDMPUSTADD6 */
#define TPTC0RDMPUSTADD6_TPTC0RDMPUSTADD6_BIT_START         0U
#define TPTC0RDMPUSTADD6_TPTC0RDMPUSTADD6_BIT_END           31U

/* Definition for field TPTC0RDMPUSTADD7 in Register TPTC0RDMPUSTADD7 */
#define TPTC0RDMPUSTADD7_TPTC0RDMPUSTADD7_BIT_START         0U
#define TPTC0RDMPUSTADD7_TPTC0RDMPUSTADD7_BIT_END           31U

/* Definition for field TPTC0RDMPUENDADD0 in Register TPTC0RDMPUENDADD0 */
#define TPTC0RDMPUENDADD0_TPTC0RDMPUENDADD0_BIT_START       0U
#define TPTC0RDMPUENDADD0_TPTC0RDMPUENDADD0_BIT_END         31U

/* Definition for field TPTC0RDMPUENDADD1 in Register TPTC0RDMPUENDADD1 */
#define TPTC0RDMPUENDADD1_TPTC0RDMPUENDADD1_BIT_START       0U
#define TPTC0RDMPUENDADD1_TPTC0RDMPUENDADD1_BIT_END         31U

/* Definition for field TPTC0RDMPUENDADD2 in Register TPTC0RDMPUENDADD2 */
#define TPTC0RDMPUENDADD2_TPTC0RDMPUENDADD2_BIT_START       0U
#define TPTC0RDMPUENDADD2_TPTC0RDMPUENDADD2_BIT_END         31U

/* Definition for field TPTC0RDMPUENDADD3 in Register TPTC0RDMPUENDADD3 */
#define TPTC0RDMPUENDADD3_TPTC0RDMPUENDADD3_BIT_START       0U
#define TPTC0RDMPUENDADD3_TPTC0RDMPUENDADD3_BIT_END         31U

/* Definition for field TPTC0RDMPUENDADD4 in Register TPTC0RDMPUENDADD4 */
#define TPTC0RDMPUENDADD4_TPTC0RDMPUENDADD4_BIT_START       0U
#define TPTC0RDMPUENDADD4_TPTC0RDMPUENDADD4_BIT_END         31U

/* Definition for field TPTC0RDMPUENDADD5 in Register TPTC0RDMPUENDADD5 */
#define TPTC0RDMPUENDADD5_TPTC0RDMPUENDADD5_BIT_START       0U
#define TPTC0RDMPUENDADD5_TPTC0RDMPUENDADD5_BIT_END         31U

/* Definition for field TPTC0RDMPUENDADD6 in Register TPTC0RDMPUENDADD6 */
#define TPTC0RDMPUENDADD6_TPTC0RDMPUENDADD6_BIT_START       0U
#define TPTC0RDMPUENDADD6_TPTC0RDMPUENDADD6_BIT_END         31U

/* Definition for field TPTC0RDMPUENDADD7 in Register TPTC0RDMPUENDADD7 */
#define TPTC0RDMPUENDADD7_TPTC0RDMPUENDADD7_BIT_START       0U
#define TPTC0RDMPUENDADD7_TPTC0RDMPUENDADD7_BIT_END         31U

/* Definition for field TPTC0RDMPUERRADD in Register TPTC0RDMPUERRADD */
#define TPTC0RDMPUERRADD_TPTC0RDMPUERRADD_BIT_START         0U
#define TPTC0RDMPUERRADD_TPTC0RDMPUERRADD_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD0 in Register TPTC1WRMPUSTADD0 */
#define TPTC1WRMPUSTADD0_TPTC1WRMPUSTADD0_BIT_START         0U
#define TPTC1WRMPUSTADD0_TPTC1WRMPUSTADD0_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD1 in Register TPTC1WRMPUSTADD1 */
#define TPTC1WRMPUSTADD1_TPTC1WRMPUSTADD1_BIT_START         0U
#define TPTC1WRMPUSTADD1_TPTC1WRMPUSTADD1_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD2 in Register TPTC1WRMPUSTADD2 */
#define TPTC1WRMPUSTADD2_TPTC1WRMPUSTADD2_BIT_START         0U
#define TPTC1WRMPUSTADD2_TPTC1WRMPUSTADD2_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD3 in Register TPTC1WRMPUSTADD3 */
#define TPTC1WRMPUSTADD3_TPTC1WRMPUSTADD3_BIT_START         0U
#define TPTC1WRMPUSTADD3_TPTC1WRMPUSTADD3_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD4 in Register TPTC1WRMPUSTADD4 */
#define TPTC1WRMPUSTADD4_TPTC1WRMPUSTADD4_BIT_START         0U
#define TPTC1WRMPUSTADD4_TPTC1WRMPUSTADD4_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD5 in Register TPTC1WRMPUSTADD5 */
#define TPTC1WRMPUSTADD5_TPTC1WRMPUSTADD5_BIT_START         0U
#define TPTC1WRMPUSTADD5_TPTC1WRMPUSTADD5_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD6 in Register TPTC1WRMPUSTADD6 */
#define TPTC1WRMPUSTADD6_TPTC1WRMPUSTADD6_BIT_START         0U
#define TPTC1WRMPUSTADD6_TPTC1WRMPUSTADD6_BIT_END           31U

/* Definition for field TPTC1WRMPUSTADD7 in Register TPTC1WRMPUSTADD7 */
#define TPTC1WRMPUSTADD7_TPTC1WRMPUSTADD7_BIT_START         0U
#define TPTC1WRMPUSTADD7_TPTC1WRMPUSTADD7_BIT_END           31U

/* Definition for field TPTC1WRMPUENDADD0 in Register TPTC1WRMPUENDADD0 */
#define TPTC1WRMPUENDADD0_TPTC1WRMPUENDADD0_BIT_START       0U
#define TPTC1WRMPUENDADD0_TPTC1WRMPUENDADD0_BIT_END         31U

/* Definition for field TPTC1WRMPUENDADD1 in Register TPTC1WRMPUENDADD1 */
#define TPTC1WRMPUENDADD1_TPTC1WRMPUENDADD1_BIT_START       0U
#define TPTC1WRMPUENDADD1_TPTC1WRMPUENDADD1_BIT_END         31U

/* Definition for field TPTC1WRMPUENDADD2 in Register TPTC1WRMPUENDADD2 */
#define TPTC1WRMPUENDADD2_TPTC1WRMPUENDADD2_BIT_START       0U
#define TPTC1WRMPUENDADD2_TPTC1WRMPUENDADD2_BIT_END         31U

/* Definition for field TPTC1WRMPUENDADD3 in Register TPTC1WRMPUENDADD3 */
#define TPTC1WRMPUENDADD3_TPTC1WRMPUENDADD3_BIT_START       0U
#define TPTC1WRMPUENDADD3_TPTC1WRMPUENDADD3_BIT_END         31U

/* Definition for field TPTC1WRMPUENDADD4 in Register TPTC1WRMPUENDADD4 */
#define TPTC1WRMPUENDADD4_TPTC1WRMPUENDADD4_BIT_START       0U
#define TPTC1WRMPUENDADD4_TPTC1WRMPUENDADD4_BIT_END         31U

/* Definition for field TPTC1WRMPUENDADD5 in Register TPTC1WRMPUENDADD5 */
#define TPTC1WRMPUENDADD5_TPTC1WRMPUENDADD5_BIT_START       0U
#define TPTC1WRMPUENDADD5_TPTC1WRMPUENDADD5_BIT_END         31U

/* Definition for field TPTC1WRMPUENDADD6 in Register TPTC1WRMPUENDADD6 */
#define TPTC1WRMPUENDADD6_TPTC1WRMPUENDADD6_BIT_START       0U
#define TPTC1WRMPUENDADD6_TPTC1WRMPUENDADD6_BIT_END         31U

/* Definition for field TPTC1WRMPUENDADD7 in Register TPTC1WRMPUENDADD7 */
#define TPTC1WRMPUENDADD7_TPTC1WRMPUENDADD7_BIT_START       0U
#define TPTC1WRMPUENDADD7_TPTC1WRMPUENDADD7_BIT_END         31U

/* Definition for field TPTC1WRMPUERRADD in Register TPTC1WRMPUERRADD */
#define TPTC1WRMPUERRADD_TPTC1WRMPUERRADD_BIT_START         0U
#define TPTC1WRMPUERRADD_TPTC1WRMPUERRADD_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD0 in Register TPTC1RDMPUSTADD0 */
#define TPTC1RDMPUSTADD0_TPTC1RDMPUSTADD0_BIT_START         0U
#define TPTC1RDMPUSTADD0_TPTC1RDMPUSTADD0_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD1 in Register TPTC1RDMPUSTADD1 */
#define TPTC1RDMPUSTADD1_TPTC1RDMPUSTADD1_BIT_START         0U
#define TPTC1RDMPUSTADD1_TPTC1RDMPUSTADD1_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD2 in Register TPTC1RDMPUSTADD2 */
#define TPTC1RDMPUSTADD2_TPTC1RDMPUSTADD2_BIT_START         0U
#define TPTC1RDMPUSTADD2_TPTC1RDMPUSTADD2_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD3 in Register TPTC1RDMPUSTADD3 */
#define TPTC1RDMPUSTADD3_TPTC1RDMPUSTADD3_BIT_START         0U
#define TPTC1RDMPUSTADD3_TPTC1RDMPUSTADD3_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD4 in Register TPTC1RDMPUSTADD4 */
#define TPTC1RDMPUSTADD4_TPTC1RDMPUSTADD4_BIT_START         0U
#define TPTC1RDMPUSTADD4_TPTC1RDMPUSTADD4_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD5 in Register TPTC1RDMPUSTADD5 */
#define TPTC1RDMPUSTADD5_TPTC1RDMPUSTADD5_BIT_START         0U
#define TPTC1RDMPUSTADD5_TPTC1RDMPUSTADD5_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD6 in Register TPTC1RDMPUSTADD6 */
#define TPTC1RDMPUSTADD6_TPTC1RDMPUSTADD6_BIT_START         0U
#define TPTC1RDMPUSTADD6_TPTC1RDMPUSTADD6_BIT_END           31U

/* Definition for field TPTC1RDMPUSTADD7 in Register TPTC1RDMPUSTADD7 */
#define TPTC1RDMPUSTADD7_TPTC1RDMPUSTADD7_BIT_START         0U
#define TPTC1RDMPUSTADD7_TPTC1RDMPUSTADD7_BIT_END           31U

/* Definition for field TPTC1RDMPUENDADD0 in Register TPTC1RDMPUENDADD0 */
#define TPTC1RDMPUENDADD0_TPTC1RDMPUENDADD0_BIT_START       0U
#define TPTC1RDMPUENDADD0_TPTC1RDMPUENDADD0_BIT_END         31U

/* Definition for field TPTC1RDMPUENDADD1 in Register TPTC1RDMPUENDADD1 */
#define TPTC1RDMPUENDADD1_TPTC1RDMPUENDADD1_BIT_START       0U
#define TPTC1RDMPUENDADD1_TPTC1RDMPUENDADD1_BIT_END         31U

/* Definition for field TPTC1RDMPUENDADD2 in Register TPTC1RDMPUENDADD2 */
#define TPTC1RDMPUENDADD2_TPTC1RDMPUENDADD2_BIT_START       0U
#define TPTC1RDMPUENDADD2_TPTC1RDMPUENDADD2_BIT_END         31U

/* Definition for field TPTC1RDMPUENDADD3 in Register TPTC1RDMPUENDADD3 */
#define TPTC1RDMPUENDADD3_TPTC1RDMPUENDADD3_BIT_START       0U
#define TPTC1RDMPUENDADD3_TPTC1RDMPUENDADD3_BIT_END         31U

/* Definition for field TPTC1RDMPUENDADD4 in Register TPTC1RDMPUENDADD4 */
#define TPTC1RDMPUENDADD4_TPTC1RDMPUENDADD4_BIT_START       0U
#define TPTC1RDMPUENDADD4_TPTC1RDMPUENDADD4_BIT_END         31U

/* Definition for field TPTC1RDMPUENDADD5 in Register TPTC1RDMPUENDADD5 */
#define TPTC1RDMPUENDADD5_TPTC1RDMPUENDADD5_BIT_START       0U
#define TPTC1RDMPUENDADD5_TPTC1RDMPUENDADD5_BIT_END         31U

/* Definition for field TPTC1RDMPUENDADD6 in Register TPTC1RDMPUENDADD6 */
#define TPTC1RDMPUENDADD6_TPTC1RDMPUENDADD6_BIT_START       0U
#define TPTC1RDMPUENDADD6_TPTC1RDMPUENDADD6_BIT_END         31U

/* Definition for field TPTC1RDMPUENDADD7 in Register TPTC1RDMPUENDADD7 */
#define TPTC1RDMPUENDADD7_TPTC1RDMPUENDADD7_BIT_START       0U
#define TPTC1RDMPUENDADD7_TPTC1RDMPUENDADD7_BIT_END         31U

/* Definition for field TPTC1RDMPUERRADD in Register TPTC1RDMPUERRADD */
#define TPTC1RDMPUERRADD_TPTC1RDMPUERRADD_BIT_START         0U
#define TPTC1RDMPUERRADD_TPTC1RDMPUERRADD_BIT_END           31U

/* Definition for field TPTC0WRMPURNGVLD in Register TPTCMPUVALIDCFG */
#define TPTCMPUVALIDCFG_TPTC0WRMPURNGVLD_BIT_START          0U
#define TPTCMPUVALIDCFG_TPTC0WRMPURNGVLD_BIT_END            7U

/* Definition for field TPTC0RDMPURNGVLD in Register TPTCMPUVALIDCFG */
#define TPTCMPUVALIDCFG_TPTC0RDMPURNGVLD_BIT_START          8U
#define TPTCMPUVALIDCFG_TPTC0RDMPURNGVLD_BIT_END            15U

/* Definition for field TPTC1WRMPURNGVLD in Register TPTCMPUVALIDCFG */
#define TPTCMPUVALIDCFG_TPTC1WRMPURNGVLD_BIT_START          16U
#define TPTCMPUVALIDCFG_TPTC1WRMPURNGVLD_BIT_END            23U

/* Definition for field TPTC1RDMPURNGVLD in Register TPTCMPUVALIDCFG */
#define TPTCMPUVALIDCFG_TPTC1RDMPURNGVLD_BIT_START          24U
#define TPTCMPUVALIDCFG_TPTC1RDMPURNGVLD_BIT_END            31U

/* Definition for field TPTC0WRMPUEN in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC0WRMPUEN_BIT_START                 0U
#define TPTCMPUENCFG_TPTC0WRMPUEN_BIT_END                   0U

/* Definition for field TPTC0RDMPUEN in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC0RDMPUEN_BIT_START                 1U
#define TPTCMPUENCFG_TPTC0RDMPUEN_BIT_END                   1U

/* Definition for field TPTC1WRMPUEN in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC1WRMPUEN_BIT_START                 2U
#define TPTCMPUENCFG_TPTC1WRMPUEN_BIT_END                   2U

/* Definition for field TPTC1RDMPUEN in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC1RDMPUEN_BIT_START                 3U
#define TPTCMPUENCFG_TPTC1RDMPUEN_BIT_END                   3U

/* Definition for field TPTC0WRMPUERRCLR in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC0WRMPUERRCLR_BIT_START             4U
#define TPTCMPUENCFG_TPTC0WRMPUERRCLR_BIT_END               4U

/* Definition for field TPTC0RDMPUERRCLR in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC0RDMPUERRCLR_BIT_START             5U
#define TPTCMPUENCFG_TPTC0RDMPUERRCLR_BIT_END               5U

/* Definition for field TPTC1WRMPUERRCLR in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC1WRMPUERRCLR_BIT_START             6U
#define TPTCMPUENCFG_TPTC1WRMPUERRCLR_BIT_END               6U

/* Definition for field TPTC1RDMPUERRCLR in Register TPTCMPUENCFG */
#define TPTCMPUENCFG_TPTC1RDMPUERRCLR_BIT_START             7U
#define TPTCMPUENCFG_TPTC1RDMPUERRCLR_BIT_END               7U

/* Definition for field TSTPATRX1IOFFSET in Register TESTPATTERNRX1ICFG */
#define TESTPATTERNRX1ICFG_TSTPATRX1IOFFSET_BIT_START       0U
#define TESTPATTERNRX1ICFG_TSTPATRX1IOFFSET_BIT_END         15U

/* Definition for field TSTPATRX1IINCR in Register TESTPATTERNRX1ICFG */
#define TESTPATTERNRX1ICFG_TSTPATRX1IINCR_BIT_START         16U
#define TESTPATTERNRX1ICFG_TSTPATRX1IINCR_BIT_END           31U

/* Definition for field TSTPATRX2IOFFSET in Register TESTPATTERNRX2ICFG */
#define TESTPATTERNRX2ICFG_TSTPATRX2IOFFSET_BIT_START       0U
#define TESTPATTERNRX2ICFG_TSTPATRX2IOFFSET_BIT_END         15U

/* Definition for field TSTPATRX2IINCR in Register TESTPATTERNRX2ICFG */
#define TESTPATTERNRX2ICFG_TSTPATRX2IINCR_BIT_START         16U
#define TESTPATTERNRX2ICFG_TSTPATRX2IINCR_BIT_END           31U

/* Definition for field TSTPATRX3IOFFSET in Register TESTPATTERNRX3ICFG */
#define TESTPATTERNRX3ICFG_TSTPATRX3IOFFSET_BIT_START       0U
#define TESTPATTERNRX3ICFG_TSTPATRX3IOFFSET_BIT_END         15U

/* Definition for field TSTPATRX3IINCR in Register TESTPATTERNRX3ICFG */
#define TESTPATTERNRX3ICFG_TSTPATRX3IINCR_BIT_START         16U
#define TESTPATTERNRX3ICFG_TSTPATRX3IINCR_BIT_END           31U

/* Definition for field TSTPATRX4IOFFSET in Register TESTPATTERNRX4ICFG */
#define TESTPATTERNRX4ICFG_TSTPATRX4IOFFSET_BIT_START       0U
#define TESTPATTERNRX4ICFG_TSTPATRX4IOFFSET_BIT_END         15U

/* Definition for field TSTPATRX4IINCR in Register TESTPATTERNRX4ICFG */
#define TESTPATTERNRX4ICFG_TSTPATRX4IINCR_BIT_START         16U
#define TESTPATTERNRX4ICFG_TSTPATRX4IINCR_BIT_END           31U

/* Definition for field TSTPATRX1QOFFSET in Register TESTPATTERNRX1QCFG */
#define TESTPATTERNRX1QCFG_TSTPATRX1QOFFSET_BIT_START       0U
#define TESTPATTERNRX1QCFG_TSTPATRX1QOFFSET_BIT_END         15U

/* Definition for field TSTPATRX1QINCR in Register TESTPATTERNRX1QCFG */
#define TESTPATTERNRX1QCFG_TSTPATRX1QINCR_BIT_START         16U
#define TESTPATTERNRX1QCFG_TSTPATRX1QINCR_BIT_END           31U

/* Definition for field TSTPATRX2QOFFSET in Register TESTPATTERNRX2QCFG */
#define TESTPATTERNRX2QCFG_TSTPATRX2QOFFSET_BIT_START       0U
#define TESTPATTERNRX2QCFG_TSTPATRX2QOFFSET_BIT_END         15U

/* Definition for field TSTPATRX2QINCR in Register TESTPATTERNRX2QCFG */
#define TESTPATTERNRX2QCFG_TSTPATRX2QINCR_BIT_START         16U
#define TESTPATTERNRX2QCFG_TSTPATRX2QINCR_BIT_END           31U

/* Definition for field TSTPATRX3QOFFSET in Register TESTPATTERNRX3QCFG */
#define TESTPATTERNRX3QCFG_TSTPATRX3QOFFSET_BIT_START       0U
#define TESTPATTERNRX3QCFG_TSTPATRX3QOFFSET_BIT_END         15U

/* Definition for field TSTPATRX3QINCR in Register TESTPATTERNRX3QCFG */
#define TESTPATTERNRX3QCFG_TSTPATRX3QINCR_BIT_START         16U
#define TESTPATTERNRX3QCFG_TSTPATRX3QINCR_BIT_END           31U

/* Definition for field TSTPATRX4QOFFSET in Register TESTPATTERNRX4QCFG */
#define TESTPATTERNRX4QCFG_TSTPATRX4QOFFSET_BIT_START       0U
#define TESTPATTERNRX4QCFG_TSTPATRX4QOFFSET_BIT_END         15U

/* Definition for field TSTPATRX4QINCR in Register TESTPATTERNRX4QCFG */
#define TESTPATTERNRX4QCFG_TSTPATRX4QINCR_BIT_START         16U
#define TESTPATTERNRX4QCFG_TSTPATRX4QINCR_BIT_END           31U

/* Definition for field TSTPATVLDCNT in Register TESTPATTERNVLDCFG */
#define TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_START            0U
#define TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_END              7U

/* Definition for field TSTPATGENEN in Register TESTPATTERNVLDCFG */
#define TESTPATTERNVLDCFG_TSTPATGENEN_BIT_START             8U
#define TESTPATTERNVLDCFG_TSTPATGENEN_BIT_END               10U

/* Definition for field FFTACCSLVEN in Register DSSMISC */
#define DSSMISC_FFTACCSLVEN_BIT_START                       6U
#define DSSMISC_FFTACCSLVEN_BIT_END                         8U



/**
 * @struct DSSRegs_t
 * @brief
 *   Module DSS_REG Register Definition
 * @details
 *   This structure is used to access the DSS_REG module registers.
 */
/**
 * @typedef DSSRegs
 * @brief
 *   Module DSS_REG Register Frame type Definition
 * @details
 *   This type is used to access the DSS_REG module registers.
 */
typedef volatile struct DSSRegs_t
{
    uint32_t    DSSGPREG0                          ;        /* Offset = 0x000 */
    uint32_t    RESTRICTED1[19]                    ;        /* Offset = 0x004 */
    uint32_t    RTIEVENTCAPTURESEL                 ;        /* Offset = 0x050 */
    uint32_t    RESTRICTED2[2]                     ;        /* Offset = 0x054 */
    uint32_t    ADCBUFCFG1                         ;        /* Offset = 0x05C */
    uint32_t    ADCBUFCFG2                         ;        /* Offset = 0x060 */
    uint32_t    ADCBUFCFG3                         ;        /* Offset = 0x064 */
    uint32_t    ADCBUFCFG4                         ;        /* Offset = 0x068 */
    uint32_t    CQCFG1                             ;        /* Offset = 0x06C */
    uint32_t    RESTRICTED3[4]                     ;        /* Offset = 0x070 */
    uint32_t    TPCCPARSTATCFG                     ;        /* Offset = 0x080 */
    uint32_t    CSI2TXPARSTATCFG                   ;        /* Offset = 0x084 */
    uint32_t    RESTRICTED4[6]                     ;        /* Offset = 0x088 */
    uint32_t    CSICFG1                            ;        /* Offset = 0x0A0 */
    uint32_t    RESTRICTED5[24]                    ;        /* Offset = 0x0A4 */
    uint32_t    TPTC0WRMPUSTADD0                   ;        /* Offset = 0x104 */
    uint32_t    TPTC0WRMPUSTADD1                   ;        /* Offset = 0x108 */
    uint32_t    TPTC0WRMPUSTADD2                   ;        /* Offset = 0x10C */
    uint32_t    TPTC0WRMPUSTADD3                   ;        /* Offset = 0x110 */
    uint32_t    TPTC0WRMPUSTADD4                   ;        /* Offset = 0x114 */
    uint32_t    TPTC0WRMPUSTADD5                   ;        /* Offset = 0x118 */
    uint32_t    TPTC0WRMPUSTADD6                   ;        /* Offset = 0x11C */
    uint32_t    TPTC0WRMPUSTADD7                   ;        /* Offset = 0x120 */
    uint32_t    TPTC0WRMPUENDADD0                  ;        /* Offset = 0x124 */
    uint32_t    TPTC0WRMPUENDADD1                  ;        /* Offset = 0x128 */
    uint32_t    TPTC0WRMPUENDADD2                  ;        /* Offset = 0x12C */
    uint32_t    TPTC0WRMPUENDADD3                  ;        /* Offset = 0x130 */
    uint32_t    TPTC0WRMPUENDADD4                  ;        /* Offset = 0x134 */
    uint32_t    TPTC0WRMPUENDADD5                  ;        /* Offset = 0x138 */
    uint32_t    TPTC0WRMPUENDADD6                  ;        /* Offset = 0x13C */
    uint32_t    TPTC0WRMPUENDADD7                  ;        /* Offset = 0x140 */
    uint32_t    TPTC0WRMPUERRADD                   ;        /* Offset = 0x144 */
    uint32_t    TPTC0RDMPUSTADD0                   ;        /* Offset = 0x148 */
    uint32_t    TPTC0RDMPUSTADD1                   ;        /* Offset = 0x14C */
    uint32_t    TPTC0RDMPUSTADD2                   ;        /* Offset = 0x150 */
    uint32_t    TPTC0RDMPUSTADD3                   ;        /* Offset = 0x154 */
    uint32_t    TPTC0RDMPUSTADD4                   ;        /* Offset = 0x158 */
    uint32_t    TPTC0RDMPUSTADD5                   ;        /* Offset = 0x15C */
    uint32_t    TPTC0RDMPUSTADD6                   ;        /* Offset = 0x160 */
    uint32_t    TPTC0RDMPUSTADD7                   ;        /* Offset = 0x164 */
    uint32_t    TPTC0RDMPUENDADD0                  ;        /* Offset = 0x168 */
    uint32_t    TPTC0RDMPUENDADD1                  ;        /* Offset = 0x16C */
    uint32_t    TPTC0RDMPUENDADD2                  ;        /* Offset = 0x170 */
    uint32_t    TPTC0RDMPUENDADD3                  ;        /* Offset = 0x174 */
    uint32_t    TPTC0RDMPUENDADD4                  ;        /* Offset = 0x178 */
    uint32_t    TPTC0RDMPUENDADD5                  ;        /* Offset = 0x17C */
    uint32_t    TPTC0RDMPUENDADD6                  ;        /* Offset = 0x180 */
    uint32_t    TPTC0RDMPUENDADD7                  ;        /* Offset = 0x184 */
    uint32_t    TPTC0RDMPUERRADD                   ;        /* Offset = 0x188 */
    uint32_t    TPTC1WRMPUSTADD0                   ;        /* Offset = 0x18C */
    uint32_t    TPTC1WRMPUSTADD1                   ;        /* Offset = 0x190 */
    uint32_t    TPTC1WRMPUSTADD2                   ;        /* Offset = 0x194 */
    uint32_t    TPTC1WRMPUSTADD3                   ;        /* Offset = 0x198 */
    uint32_t    TPTC1WRMPUSTADD4                   ;        /* Offset = 0x19C */
    uint32_t    TPTC1WRMPUSTADD5                   ;        /* Offset = 0x1A0 */
    uint32_t    TPTC1WRMPUSTADD6                   ;        /* Offset = 0x1A4 */
    uint32_t    TPTC1WRMPUSTADD7                   ;        /* Offset = 0x1A8 */
    uint32_t    TPTC1WRMPUENDADD0                  ;        /* Offset = 0x1AC */
    uint32_t    TPTC1WRMPUENDADD1                  ;        /* Offset = 0x1B0 */
    uint32_t    TPTC1WRMPUENDADD2                  ;        /* Offset = 0x1B4 */
    uint32_t    TPTC1WRMPUENDADD3                  ;        /* Offset = 0x1B8 */
    uint32_t    TPTC1WRMPUENDADD4                  ;        /* Offset = 0x1BC */
    uint32_t    TPTC1WRMPUENDADD5                  ;        /* Offset = 0x1C0 */
    uint32_t    TPTC1WRMPUENDADD6                  ;        /* Offset = 0x1C4 */
    uint32_t    TPTC1WRMPUENDADD7                  ;        /* Offset = 0x1C8 */
    uint32_t    TPTC1WRMPUERRADD                   ;        /* Offset = 0x1CC */
    uint32_t    TPTC1RDMPUSTADD0                   ;        /* Offset = 0x1D0 */
    uint32_t    TPTC1RDMPUSTADD1                   ;        /* Offset = 0x1D4 */
    uint32_t    TPTC1RDMPUSTADD2                   ;        /* Offset = 0x1D8 */
    uint32_t    TPTC1RDMPUSTADD3                   ;        /* Offset = 0x1DC */
    uint32_t    TPTC1RDMPUSTADD4                   ;        /* Offset = 0x1E0 */
    uint32_t    TPTC1RDMPUSTADD5                   ;        /* Offset = 0x1E4 */
    uint32_t    TPTC1RDMPUSTADD6                   ;        /* Offset = 0x1E8 */
    uint32_t    TPTC1RDMPUSTADD7                   ;        /* Offset = 0x1EC */
    uint32_t    TPTC1RDMPUENDADD0                  ;        /* Offset = 0x1F0 */
    uint32_t    TPTC1RDMPUENDADD1                  ;        /* Offset = 0x1F4 */
    uint32_t    TPTC1RDMPUENDADD2                  ;        /* Offset = 0x1F8 */
    uint32_t    TPTC1RDMPUENDADD3                  ;        /* Offset = 0x1FC */
    uint32_t    TPTC1RDMPUENDADD4                  ;        /* Offset = 0x200 */
    uint32_t    TPTC1RDMPUENDADD5                  ;        /* Offset = 0x204 */
    uint32_t    TPTC1RDMPUENDADD6                  ;        /* Offset = 0x208 */
    uint32_t    TPTC1RDMPUENDADD7                  ;        /* Offset = 0x20C */
    uint32_t    TPTC1RDMPUERRADD                   ;        /* Offset = 0x210 */
    uint32_t    TPTCMPUVALIDCFG                    ;        /* Offset = 0x214 */
    uint32_t    TPTCMPUENCFG                       ;        /* Offset = 0x218 */
    uint32_t    TESTPATTERNRX1ICFG                 ;        /* Offset = 0x21C */
    uint32_t    TESTPATTERNRX2ICFG                 ;        /* Offset = 0x220 */
    uint32_t    TESTPATTERNRX3ICFG                 ;        /* Offset = 0x224 */
    uint32_t    TESTPATTERNRX4ICFG                 ;        /* Offset = 0x228 */
    uint32_t    TESTPATTERNRX1QCFG                 ;        /* Offset = 0x22C */
    uint32_t    TESTPATTERNRX2QCFG                 ;        /* Offset = 0x230 */
    uint32_t    TESTPATTERNRX3QCFG                 ;        /* Offset = 0x234 */
    uint32_t    TESTPATTERNRX4QCFG                 ;        /* Offset = 0x238 */
    uint32_t    TESTPATTERNVLDCFG                  ;        /* Offset = 0x23C */
    uint32_t    DSSMISC                            ;        /* Offset = 0x240 */
} DSSRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_DSS_H */
/* END OF REG_DSS_H */

