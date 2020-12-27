/**
 *  @file    reg_dspicfg.h
 *
 *  @brief
 *    This file gives register definitions of DSP_ICFG module.
 *
 *  This file is auto-generated on 3/7/2017.
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

#ifndef REG_DSPICFG_H
#define REG_DSPICFG_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field EF in Register EVTFLAG0 */
#define EVTFLAG0_EF_BIT_START                               0U
#define EVTFLAG0_EF_BIT_END                                 31U

/* Definition for field EF in Register EVTFLAG1 */
#define EVTFLAG1_EF_BIT_START                               0U
#define EVTFLAG1_EF_BIT_END                                 31U

/* Definition for field EF in Register EVTFLAG2 */
#define EVTFLAG2_EF_BIT_START                               0U
#define EVTFLAG2_EF_BIT_END                                 31U

/* Definition for field EF in Register EVTFLAG3 */
#define EVTFLAG3_EF_BIT_START                               0U
#define EVTFLAG3_EF_BIT_END                                 31U

/* Definition for field ES in Register EVTSET0 */
#define EVTSET0_ES_BIT_START                                0U
#define EVTSET0_ES_BIT_END                                  31U

/* Definition for field ES in Register EVTSET1 */
#define EVTSET1_ES_BIT_START                                0U
#define EVTSET1_ES_BIT_END                                  31U

/* Definition for field ES in Register EVTSET2 */
#define EVTSET2_ES_BIT_START                                0U
#define EVTSET2_ES_BIT_END                                  31U

/* Definition for field ES in Register EVTSET3 */
#define EVTSET3_ES_BIT_START                                0U
#define EVTSET3_ES_BIT_END                                  31U

/* Definition for field EC in Register EVTCLR0 */
#define EVTCLR0_EC_BIT_START                                0U
#define EVTCLR0_EC_BIT_END                                  31U

/* Definition for field EC in Register EVTCLR1 */
#define EVTCLR1_EC_BIT_START                                0U
#define EVTCLR1_EC_BIT_END                                  31U

/* Definition for field EC in Register EVTCLR2 */
#define EVTCLR2_EC_BIT_START                                0U
#define EVTCLR2_EC_BIT_END                                  31U

/* Definition for field EC in Register EVTCLR3 */
#define EVTCLR3_EC_BIT_START                                0U
#define EVTCLR3_EC_BIT_END                                  31U

/* Definition for field EM in Register EVTMASK0 */
#define EVTMASK0_EM_BIT_START                               0U
#define EVTMASK0_EM_BIT_END                                 31U

/* Definition for field EM in Register EVTMASK1 */
#define EVTMASK1_EM_BIT_START                               0U
#define EVTMASK1_EM_BIT_END                                 31U

/* Definition for field EM in Register EVTMASK2 */
#define EVTMASK2_EM_BIT_START                               0U
#define EVTMASK2_EM_BIT_END                                 31U

/* Definition for field EM in Register EVTMASK3 */
#define EVTMASK3_EM_BIT_START                               0U
#define EVTMASK3_EM_BIT_END                                 31U

/* Definition for field MEVTFLAG in Register MEVTFLAG0 */
#define MEVTFLAG0_MEVTFLAG_BIT_START                        0U
#define MEVTFLAG0_MEVTFLAG_BIT_END                          31U

/* Definition for field MEVTFLAG in Register MEVTFLAG1 */
#define MEVTFLAG1_MEVTFLAG_BIT_START                        0U
#define MEVTFLAG1_MEVTFLAG_BIT_END                          31U

/* Definition for field MEVTFLAG in Register MEVTFLAG2 */
#define MEVTFLAG2_MEVTFLAG_BIT_START                        0U
#define MEVTFLAG2_MEVTFLAG_BIT_END                          31U

/* Definition for field MEVTFLAG in Register MEVTFLAG3 */
#define MEVTFLAG3_MEVTFLAG_BIT_START                        0U
#define MEVTFLAG3_MEVTFLAG_BIT_END                          31U

/* Definition for field XM in Register EXPMASK0 */
#define EXPMASK0_XM_BIT_START                               0U
#define EXPMASK0_XM_BIT_END                                 31U

/* Definition for field XM in Register EXPMASK1 */
#define EXPMASK1_XM_BIT_START                               0U
#define EXPMASK1_XM_BIT_END                                 31U

/* Definition for field XM in Register EXPMASK2 */
#define EXPMASK2_XM_BIT_START                               0U
#define EXPMASK2_XM_BIT_END                                 31U

/* Definition for field XM in Register EXPMASK3 */
#define EXPMASK3_XM_BIT_START                               0U
#define EXPMASK3_XM_BIT_END                                 31U

/* Definition for field MEXPFLAG in Register MEXPFLAG0 */
#define MEXPFLAG0_MEXPFLAG_BIT_START                        0U
#define MEXPFLAG0_MEXPFLAG_BIT_END                          31U

/* Definition for field MEXPFLAG in Register MEXPFLAG1 */
#define MEXPFLAG1_MEXPFLAG_BIT_START                        0U
#define MEXPFLAG1_MEXPFLAG_BIT_END                          31U

/* Definition for field MEXPFLAG in Register MEXPFLAG2 */
#define MEXPFLAG2_MEXPFLAG_BIT_START                        0U
#define MEXPFLAG2_MEXPFLAG_BIT_END                          31U

/* Definition for field MEXPFLAG in Register MEXPFLAG3 */
#define MEXPFLAG3_MEXPFLAG_BIT_START                        0U
#define MEXPFLAG3_MEXPFLAG_BIT_END                          31U

/* Definition for field _RESV3 in Register INTMUX1 */
#define INTMUX1__RESV3_BIT_START                            31U
#define INTMUX1__RESV3_BIT_END                              31U

/* Definition for field INTSEL7 in Register INTMUX1 */
#define INTMUX1_INTSEL7_BIT_START                           24U
#define INTMUX1_INTSEL7_BIT_END                             30U

/* Definition for field _RESV2 in Register INTMUX1 */
#define INTMUX1__RESV2_BIT_START                            23U
#define INTMUX1__RESV2_BIT_END                              23U

/* Definition for field INTSEL6 in Register INTMUX1 */
#define INTMUX1_INTSEL6_BIT_START                           16U
#define INTMUX1_INTSEL6_BIT_END                             22U

/* Definition for field _RESV1 in Register INTMUX1 */
#define INTMUX1__RESV1_BIT_START                            15U
#define INTMUX1__RESV1_BIT_END                              15U

/* Definition for field INTSEL5 in Register INTMUX1 */
#define INTMUX1_INTSEL5_BIT_START                           8U
#define INTMUX1_INTSEL5_BIT_END                             14U

/* Definition for field _RESV0 in Register INTMUX1 */
#define INTMUX1__RESV0_BIT_START                            7U
#define INTMUX1__RESV0_BIT_END                              7U

/* Definition for field INTSEL4 in Register INTMUX1 */
#define INTMUX1_INTSEL4_BIT_START                           0U
#define INTMUX1_INTSEL4_BIT_END                             6U

/* Definition for field _RESV3 in Register INTMUX2 */
#define INTMUX2__RESV3_BIT_START                            31U
#define INTMUX2__RESV3_BIT_END                              31U

/* Definition for field INTSEL11 in Register INTMUX2 */
#define INTMUX2_INTSEL11_BIT_START                          24U
#define INTMUX2_INTSEL11_BIT_END                            30U

/* Definition for field _RESV2 in Register INTMUX2 */
#define INTMUX2__RESV2_BIT_START                            23U
#define INTMUX2__RESV2_BIT_END                              23U

/* Definition for field INTSEL10 in Register INTMUX2 */
#define INTMUX2_INTSEL10_BIT_START                          16U
#define INTMUX2_INTSEL10_BIT_END                            22U

/* Definition for field _RESV1 in Register INTMUX2 */
#define INTMUX2__RESV1_BIT_START                            15U
#define INTMUX2__RESV1_BIT_END                              15U

/* Definition for field INTSEL9 in Register INTMUX2 */
#define INTMUX2_INTSEL9_BIT_START                           8U
#define INTMUX2_INTSEL9_BIT_END                             14U

/* Definition for field _RESV0 in Register INTMUX2 */
#define INTMUX2__RESV0_BIT_START                            7U
#define INTMUX2__RESV0_BIT_END                              7U

/* Definition for field INTSEL8 in Register INTMUX2 */
#define INTMUX2_INTSEL8_BIT_START                           0U
#define INTMUX2_INTSEL8_BIT_END                             6U

/* Definition for field _RESV3 in Register INTMUX3 */
#define INTMUX3__RESV3_BIT_START                            31U
#define INTMUX3__RESV3_BIT_END                              31U

/* Definition for field INTSEL15 in Register INTMUX3 */
#define INTMUX3_INTSEL15_BIT_START                          24U
#define INTMUX3_INTSEL15_BIT_END                            30U

/* Definition for field _RESV2 in Register INTMUX3 */
#define INTMUX3__RESV2_BIT_START                            23U
#define INTMUX3__RESV2_BIT_END                              23U

/* Definition for field INTSEL14 in Register INTMUX3 */
#define INTMUX3_INTSEL14_BIT_START                          16U
#define INTMUX3_INTSEL14_BIT_END                            22U

/* Definition for field _RESV1 in Register INTMUX3 */
#define INTMUX3__RESV1_BIT_START                            15U
#define INTMUX3__RESV1_BIT_END                              15U

/* Definition for field INTSEL13 in Register INTMUX3 */
#define INTMUX3_INTSEL13_BIT_START                          8U
#define INTMUX3_INTSEL13_BIT_END                            14U

/* Definition for field _RESV0 in Register INTMUX3 */
#define INTMUX3__RESV0_BIT_START                            7U
#define INTMUX3__RESV0_BIT_END                              7U

/* Definition for field INTSEL12 in Register INTMUX3 */
#define INTMUX3_INTSEL12_BIT_START                          0U
#define INTMUX3_INTSEL12_BIT_END                            6U

/* Definition for field RES1 in Register AEGMUX0 */
#define AEGMUX0_RES1_BIT_START                              31U
#define AEGMUX0_RES1_BIT_END                                31U

/* Definition for field AEGSEL3 in Register AEGMUX0 */
#define AEGMUX0_AEGSEL3_BIT_START                           24U
#define AEGMUX0_AEGSEL3_BIT_END                             30U

/* Definition for field RES2 in Register AEGMUX0 */
#define AEGMUX0_RES2_BIT_START                              23U
#define AEGMUX0_RES2_BIT_END                                23U

/* Definition for field AEGSEL2 in Register AEGMUX0 */
#define AEGMUX0_AEGSEL2_BIT_START                           16U
#define AEGMUX0_AEGSEL2_BIT_END                             22U

/* Definition for field RES3 in Register AEGMUX0 */
#define AEGMUX0_RES3_BIT_START                              15U
#define AEGMUX0_RES3_BIT_END                                15U

/* Definition for field AEGSEL1 in Register AEGMUX0 */
#define AEGMUX0_AEGSEL1_BIT_START                           8U
#define AEGMUX0_AEGSEL1_BIT_END                             14U

/* Definition for field RES4 in Register AEGMUX0 */
#define AEGMUX0_RES4_BIT_START                              7U
#define AEGMUX0_RES4_BIT_END                                7U

/* Definition for field AEGSEL0 in Register AEGMUX0 */
#define AEGMUX0_AEGSEL0_BIT_START                           0U
#define AEGMUX0_AEGSEL0_BIT_END                             6U

/* Definition for field RES5 in Register AEGMUX1 */
#define AEGMUX1_RES5_BIT_START                              31U
#define AEGMUX1_RES5_BIT_END                                31U

/* Definition for field AEGSEL7 in Register AEGMUX1 */
#define AEGMUX1_AEGSEL7_BIT_START                           24U
#define AEGMUX1_AEGSEL7_BIT_END                             30U

/* Definition for field RES6 in Register AEGMUX1 */
#define AEGMUX1_RES6_BIT_START                              23U
#define AEGMUX1_RES6_BIT_END                                23U

/* Definition for field AEGSEL6 in Register AEGMUX1 */
#define AEGMUX1_AEGSEL6_BIT_START                           16U
#define AEGMUX1_AEGSEL6_BIT_END                             22U

/* Definition for field RES7 in Register AEGMUX1 */
#define AEGMUX1_RES7_BIT_START                              15U
#define AEGMUX1_RES7_BIT_END                                15U

/* Definition for field AEGSEL5 in Register AEGMUX1 */
#define AEGMUX1_AEGSEL5_BIT_START                           8U
#define AEGMUX1_AEGSEL5_BIT_END                             14U

/* Definition for field RES8 in Register AEGMUX1 */
#define AEGMUX1_RES8_BIT_START                              7U
#define AEGMUX1_RES8_BIT_END                                7U

/* Definition for field AEGSEL4 in Register AEGMUX1 */
#define AEGMUX1_AEGSEL4_BIT_START                           0U
#define AEGMUX1_AEGSEL4_BIT_END                             6U

/* Definition for field SYSINT in Register INTXSTAT */
#define INTXSTAT_SYSINT_BIT_START                           24U
#define INTXSTAT_SYSINT_BIT_END                             31U

/* Definition for field CPUINT in Register INTXSTAT */
#define INTXSTAT_CPUINT_BIT_START                           16U
#define INTXSTAT_CPUINT_BIT_END                             23U

/* Definition for field _RESV3 in Register INTXSTAT */
#define INTXSTAT__RESV3_BIT_START                           1U
#define INTXSTAT__RESV3_BIT_END                             15U

/* Definition for field DROP in Register INTXSTAT */
#define INTXSTAT_DROP_BIT_START                             0U
#define INTXSTAT_DROP_BIT_END                               0U

/* Definition for field _RESV3 in Register INTXCLR */
#define INTXCLR__RESV3_BIT_START                            1U
#define INTXCLR__RESV3_BIT_END                              31U

/* Definition for field CLEAR in Register INTXCLR */
#define INTXCLR_CLEAR_BIT_START                             0U
#define INTXCLR_CLEAR_BIT_END                               0U

/* Definition for field _RESV1 in Register INTDMASK */
#define INTDMASK__RESV1_BIT_START                           16U
#define INTDMASK__RESV1_BIT_END                             31U

/* Definition for field IDM in Register INTDMASK */
#define INTDMASK_IDM_BIT_START                              4U
#define INTDMASK_IDM_BIT_END                                15U

/* Definition for field _RESV0 in Register INTDMASK */
#define INTDMASK__RESV0_BIT_START                           0U
#define INTDMASK__RESV0_BIT_END                             3U

/* Definition for field MRA in Register EVTASRT */
#define EVTASRT_MRA_BIT_START                               31U
#define EVTASRT_MRA_BIT_END                                 31U

/* Definition for field SRA in Register EVTASRT */
#define EVTASRT_SRA_BIT_START                               30U
#define EVTASRT_SRA_BIT_END                                 30U

/* Definition for field CRA in Register EVTASRT */
#define EVTASRT_CRA_BIT_START                               29U
#define EVTASRT_CRA_BIT_END                                 29U

/* Definition for field LRA in Register EVTASRT */
#define EVTASRT_LRA_BIT_START                               28U
#define EVTASRT_LRA_BIT_END                                 28U

/* Definition for field _RESV0 in Register EVTASRT */
#define EVTASRT__RESV0_BIT_START                            8U
#define EVTASRT__RESV0_BIT_END                              27U

/* Definition for field EA in Register EVTASRT */
#define EVTASRT_EA_BIT_START                                0U
#define EVTASRT_EA_BIT_END                                  7U

/* Definition for field RESV_1 in Register PDCCMD */
#define PDCCMD_RESV_1_BIT_START                             17U
#define PDCCMD_RESV_1_BIT_END                               31U

/* Definition for field GEMPD in Register PDCCMD */
#define PDCCMD_GEMPD_BIT_START                              16U
#define PDCCMD_GEMPD_BIT_END                                16U

/* Definition for field RESV_2 in Register PDCCMD */
#define PDCCMD_RESV_2_BIT_START                             14U
#define PDCCMD_RESV_2_BIT_END                               15U

/* Definition for field EMCLOG in Register PDCCMD */
#define PDCCMD_EMCLOG_BIT_START                             12U
#define PDCCMD_EMCLOG_BIT_END                               13U

/* Definition for field RESV_3 in Register PDCCMD */
#define PDCCMD_RESV_3_BIT_START                             10U
#define PDCCMD_RESV_3_BIT_END                               11U

/* Definition for field UMCLOG in Register PDCCMD */
#define PDCCMD_UMCLOG_BIT_START                             8U
#define PDCCMD_UMCLOG_BIT_END                               9U

/* Definition for field DMCMEM in Register PDCCMD */
#define PDCCMD_DMCMEM_BIT_START                             6U
#define PDCCMD_DMCMEM_BIT_END                               7U

/* Definition for field DMCLOG in Register PDCCMD */
#define PDCCMD_DMCLOG_BIT_START                             4U
#define PDCCMD_DMCLOG_BIT_END                               5U

/* Definition for field PMCMEM in Register PDCCMD */
#define PDCCMD_PMCMEM_BIT_START                             2U
#define PDCCMD_PMCMEM_BIT_END                               3U

/* Definition for field PMCLOG in Register PDCCMD */
#define PDCCMD_PMCLOG_BIT_START                             0U
#define PDCCMD_PMCLOG_BIT_END                               1U

/* Definition for field MM_REVID in Register MM_REVID */
#define MM_REVID_MM_REVID_BIT_START                         0U
#define MM_REVID_MM_REVID_BIT_END                           31U

/* Definition for field _RESV1 in Register IDMA0_STAT */
#define IDMA0_STAT__RESV1_BIT_START                         2U
#define IDMA0_STAT__RESV1_BIT_END                           31U

/* Definition for field PEND in Register IDMA0_STAT */
#define IDMA0_STAT_PEND_BIT_START                           1U
#define IDMA0_STAT_PEND_BIT_END                             1U

/* Definition for field ACTV in Register IDMA0_STAT */
#define IDMA0_STAT_ACTV_BIT_START                           0U
#define IDMA0_STAT_ACTV_BIT_END                             0U

/* Definition for field MASK in Register IDMA0_MASK */
#define IDMA0_MASK_MASK_BIT_START                           0U
#define IDMA0_MASK_MASK_BIT_END                             31U

/* Definition for field SOURCEADDR in Register IDMA0_SOURCE */
#define IDMA0_SOURCE_SOURCEADDR_BIT_START                   5U
#define IDMA0_SOURCE_SOURCEADDR_BIT_END                     31U

/* Definition for field _RESV1 in Register IDMA0_SOURCE */
#define IDMA0_SOURCE__RESV1_BIT_START                       0U
#define IDMA0_SOURCE__RESV1_BIT_END                         4U

/* Definition for field DESTADDR in Register IDMA0_DEST */
#define IDMA0_DEST_DESTADDR_BIT_START                       5U
#define IDMA0_DEST_DESTADDR_BIT_END                         31U

/* Definition for field _RESV1 in Register IDMA0_DEST */
#define IDMA0_DEST__RESV1_BIT_START                         0U
#define IDMA0_DEST__RESV1_BIT_END                           4U

/* Definition for field _RESV1 in Register IDMA0_COUNT */
#define IDMA0_COUNT__RESV1_BIT_START                        29U
#define IDMA0_COUNT__RESV1_BIT_END                          31U

/* Definition for field INT in Register IDMA0_COUNT */
#define IDMA0_COUNT_INT_BIT_START                           28U
#define IDMA0_COUNT_INT_BIT_END                             28U

/* Definition for field _RESV2 in Register IDMA0_COUNT */
#define IDMA0_COUNT__RESV2_BIT_START                        4U
#define IDMA0_COUNT__RESV2_BIT_END                          27U

/* Definition for field COUNT in Register IDMA0_COUNT */
#define IDMA0_COUNT_COUNT_BIT_START                         0U
#define IDMA0_COUNT_COUNT_BIT_END                           3U

/* Definition for field _RESV1 in Register IDMA1_STAT */
#define IDMA1_STAT__RESV1_BIT_START                         2U
#define IDMA1_STAT__RESV1_BIT_END                           31U

/* Definition for field PEND in Register IDMA1_STAT */
#define IDMA1_STAT_PEND_BIT_START                           1U
#define IDMA1_STAT_PEND_BIT_END                             1U

/* Definition for field ACTV in Register IDMA1_STAT */
#define IDMA1_STAT_ACTV_BIT_START                           0U
#define IDMA1_STAT_ACTV_BIT_END                             0U

/* Definition for field SOURCEADDR in Register IDMA1_SOURCE */
#define IDMA1_SOURCE_SOURCEADDR_BIT_START                   2U
#define IDMA1_SOURCE_SOURCEADDR_BIT_END                     31U

/* Definition for field _RESV1 in Register IDMA1_SOURCE */
#define IDMA1_SOURCE__RESV1_BIT_START                       0U
#define IDMA1_SOURCE__RESV1_BIT_END                         1U

/* Definition for field DESTADDR in Register IDMA1_DEST */
#define IDMA1_DEST_DESTADDR_BIT_START                       2U
#define IDMA1_DEST_DESTADDR_BIT_END                         31U

/* Definition for field _RESV1 in Register IDMA1_DEST */
#define IDMA1_DEST__RESV1_BIT_START                         0U
#define IDMA1_DEST__RESV1_BIT_END                           1U

/* Definition for field PRI in Register IDMA1_COUNT */
#define IDMA1_COUNT_PRI_BIT_START                           29U
#define IDMA1_COUNT_PRI_BIT_END                             31U

/* Definition for field INT in Register IDMA1_COUNT */
#define IDMA1_COUNT_INT_BIT_START                           28U
#define IDMA1_COUNT_INT_BIT_END                             28U

/* Definition for field _RESV1 in Register IDMA1_COUNT */
#define IDMA1_COUNT__RESV1_BIT_START                        17U
#define IDMA1_COUNT__RESV1_BIT_END                          27U

/* Definition for field FILL in Register IDMA1_COUNT */
#define IDMA1_COUNT_FILL_BIT_START                          16U
#define IDMA1_COUNT_FILL_BIT_END                            16U

/* Definition for field COUNT in Register IDMA1_COUNT */
#define IDMA1_COUNT_COUNT_BIT_START                         2U
#define IDMA1_COUNT_COUNT_BIT_END                           15U

/* Definition for field _RESV2 in Register IDMA1_COUNT */
#define IDMA1_COUNT__RESV2_BIT_START                        0U
#define IDMA1_COUNT__RESV2_BIT_END                          1U

/* Definition for field RESV_1 in Register CPUARBE */
#define CPUARBE_RESV_1_BIT_START                            19U
#define CPUARBE_RESV_1_BIT_END                              31U

/* Definition for field PRI in Register CPUARBE */
#define CPUARBE_PRI_BIT_START                               16U
#define CPUARBE_PRI_BIT_END                                 18U

/* Definition for field RESV_2 in Register CPUARBE */
#define CPUARBE_RESV_2_BIT_START                            6U
#define CPUARBE_RESV_2_BIT_END                              15U

/* Definition for field MAXWAIT in Register CPUARBE */
#define CPUARBE_MAXWAIT_BIT_START                           0U
#define CPUARBE_MAXWAIT_BIT_END                             5U

/* Definition for field RESV_1 in Register IDMAARBE */
#define IDMAARBE_RESV_1_BIT_START                           6U
#define IDMAARBE_RESV_1_BIT_END                             31U

/* Definition for field MAXWAIT in Register IDMAARBE */
#define IDMAARBE_MAXWAIT_BIT_START                          0U
#define IDMAARBE_MAXWAIT_BIT_END                            5U

/* Definition for field RESV_1 in Register SDMAARBE */
#define SDMAARBE_RESV_1_BIT_START                           6U
#define SDMAARBE_RESV_1_BIT_END                             31U

/* Definition for field MAXWAIT in Register SDMAARBE */
#define SDMAARBE_MAXWAIT_BIT_START                          0U
#define SDMAARBE_MAXWAIT_BIT_END                            5U

/* Definition for field RESV_2 in Register ECFGARBE */
#define ECFGARBE_RESV_2_BIT_START                           19U
#define ECFGARBE_RESV_2_BIT_END                             31U

/* Definition for field PRI in Register ECFGARBE */
#define ECFGARBE_PRI_BIT_START                              16U
#define ECFGARBE_PRI_BIT_END                                18U

/* Definition for field RESV_1 in Register ECFGARBE */
#define ECFGARBE_RESV_1_BIT_START                           0U
#define ECFGARBE_RESV_1_BIT_END                             15U

/* Definition for field ADDR in Register ICFGMPFAR */
#define ICFGMPFAR_ADDR_BIT_START                            0U
#define ICFGMPFAR_ADDR_BIT_END                              31U

/* Definition for field RESV_1 in Register ICFGMPFSR */
#define ICFGMPFSR_RESV_1_BIT_START                          16U
#define ICFGMPFSR_RESV_1_BIT_END                            31U

/* Definition for field FID in Register ICFGMPFSR */
#define ICFGMPFSR_FID_BIT_START                             8U
#define ICFGMPFSR_FID_BIT_END                               15U

/* Definition for field SECE in Register ICFGMPFSR */
#define ICFGMPFSR_SECE_BIT_START                            7U
#define ICFGMPFSR_SECE_BIT_END                              7U

/* Definition for field RESV_2 in Register ICFGMPFSR */
#define ICFGMPFSR_RESV_2_BIT_START                          6U
#define ICFGMPFSR_RESV_2_BIT_END                            6U

/* Definition for field SR in Register ICFGMPFSR */
#define ICFGMPFSR_SR_BIT_START                              5U
#define ICFGMPFSR_SR_BIT_END                                5U

/* Definition for field SW in Register ICFGMPFSR */
#define ICFGMPFSR_SW_BIT_START                              4U
#define ICFGMPFSR_SW_BIT_END                                4U

/* Definition for field SX in Register ICFGMPFSR */
#define ICFGMPFSR_SX_BIT_START                              3U
#define ICFGMPFSR_SX_BIT_END                                3U

/* Definition for field UR in Register ICFGMPFSR */
#define ICFGMPFSR_UR_BIT_START                              2U
#define ICFGMPFSR_UR_BIT_END                                2U

/* Definition for field UW in Register ICFGMPFSR */
#define ICFGMPFSR_UW_BIT_START                              1U
#define ICFGMPFSR_UW_BIT_END                                1U

/* Definition for field UX in Register ICFGMPFSR */
#define ICFGMPFSR_UX_BIT_START                              0U
#define ICFGMPFSR_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register ICFGMPFCR */
#define ICFGMPFCR_RESV_1_BIT_START                          1U
#define ICFGMPFCR_RESV_1_BIT_END                            31U

/* Definition for field MPFCLR in Register ICFGMPFCR */
#define ICFGMPFCR_MPFCLR_BIT_START                          0U
#define ICFGMPFCR_MPFCLR_BIT_END                            0U

/* Definition for field ERR in Register ECFGERR */
#define ECFGERR_ERR_BIT_START                               29U
#define ECFGERR_ERR_BIT_END                                 31U

/* Definition for field RESV_2 in Register ECFGERR */
#define ECFGERR_RESV_2_BIT_START                            12U
#define ECFGERR_RESV_2_BIT_END                              28U

/* Definition for field XID in Register ECFGERR */
#define ECFGERR_XID_BIT_START                               8U
#define ECFGERR_XID_BIT_END                                 11U

/* Definition for field RESV_1 in Register ECFGERR */
#define ECFGERR_RESV_1_BIT_START                            3U
#define ECFGERR_RESV_1_BIT_END                              7U

/* Definition for field STAT in Register ECFGERR */
#define ECFGERR_STAT_BIT_START                              0U
#define ECFGERR_STAT_BIT_END                                2U

/* Definition for field RESV_1 in Register ECFGERRCLR */
#define ECFGERRCLR_RESV_1_BIT_START                         1U
#define ECFGERRCLR_RESV_1_BIT_END                           31U

/* Definition for field CLR in Register ECFGERRCLR */
#define ECFGERRCLR_CLR_BIT_START                            0U
#define ECFGERRCLR_CLR_BIT_END                              0U

/* Definition for field _RESV1 in Register PAMAP0 */
#define PAMAP0__RESV1_BIT_START                             3U
#define PAMAP0__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP0 */
#define PAMAP0_AID_BIT_START                                0U
#define PAMAP0_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP1 */
#define PAMAP1__RESV1_BIT_START                             3U
#define PAMAP1__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP1 */
#define PAMAP1_AID_BIT_START                                0U
#define PAMAP1_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP2 */
#define PAMAP2__RESV1_BIT_START                             3U
#define PAMAP2__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP2 */
#define PAMAP2_AID_BIT_START                                0U
#define PAMAP2_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP3 */
#define PAMAP3__RESV1_BIT_START                             3U
#define PAMAP3__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP3 */
#define PAMAP3_AID_BIT_START                                0U
#define PAMAP3_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP4 */
#define PAMAP4__RESV1_BIT_START                             3U
#define PAMAP4__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP4 */
#define PAMAP4_AID_BIT_START                                0U
#define PAMAP4_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP5 */
#define PAMAP5__RESV1_BIT_START                             3U
#define PAMAP5__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP5 */
#define PAMAP5_AID_BIT_START                                0U
#define PAMAP5_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP6 */
#define PAMAP6__RESV1_BIT_START                             3U
#define PAMAP6__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP6 */
#define PAMAP6_AID_BIT_START                                0U
#define PAMAP6_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP7 */
#define PAMAP7__RESV1_BIT_START                             3U
#define PAMAP7__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP7 */
#define PAMAP7_AID_BIT_START                                0U
#define PAMAP7_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP8 */
#define PAMAP8__RESV1_BIT_START                             3U
#define PAMAP8__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP8 */
#define PAMAP8_AID_BIT_START                                0U
#define PAMAP8_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP9 */
#define PAMAP9__RESV1_BIT_START                             3U
#define PAMAP9__RESV1_BIT_END                               31U

/* Definition for field AID in Register PAMAP9 */
#define PAMAP9_AID_BIT_START                                0U
#define PAMAP9_AID_BIT_END                                  2U

/* Definition for field _RESV1 in Register PAMAP10 */
#define PAMAP10__RESV1_BIT_START                            3U
#define PAMAP10__RESV1_BIT_END                              31U

/* Definition for field AID in Register PAMAP10 */
#define PAMAP10_AID_BIT_START                               0U
#define PAMAP10_AID_BIT_END                                 2U

/* Definition for field _RESV1 in Register PAMAP11 */
#define PAMAP11__RESV1_BIT_START                            3U
#define PAMAP11__RESV1_BIT_END                              31U

/* Definition for field AID in Register PAMAP11 */
#define PAMAP11_AID_BIT_START                               0U
#define PAMAP11_AID_BIT_END                                 2U

/* Definition for field _RESV1 in Register PAMAP12 */
#define PAMAP12__RESV1_BIT_START                            3U
#define PAMAP12__RESV1_BIT_END                              31U

/* Definition for field AID in Register PAMAP12 */
#define PAMAP12_AID_BIT_START                               0U
#define PAMAP12_AID_BIT_END                                 2U

/* Definition for field _RESV1 in Register PAMAP13 */
#define PAMAP13__RESV1_BIT_START                            3U
#define PAMAP13__RESV1_BIT_END                              31U

/* Definition for field AID in Register PAMAP13 */
#define PAMAP13_AID_BIT_START                               0U
#define PAMAP13_AID_BIT_END                                 2U

/* Definition for field _RESV1 in Register PAMAP14 */
#define PAMAP14__RESV1_BIT_START                            3U
#define PAMAP14__RESV1_BIT_END                              31U

/* Definition for field AID in Register PAMAP14 */
#define PAMAP14_AID_BIT_START                               0U
#define PAMAP14_AID_BIT_END                                 2U

/* Definition for field _RESV1 in Register PAMAP15 */
#define PAMAP15__RESV1_BIT_START                            3U
#define PAMAP15__RESV1_BIT_END                              31U

/* Definition for field AID in Register PAMAP15 */
#define PAMAP15_AID_BIT_START                               0U
#define PAMAP15_AID_BIT_END                                 2U

/* Definition for field RESV_1 in Register L2CFG */
#define L2CFG_RESV_1_BIT_START                              28U
#define L2CFG_RESV_1_BIT_END                                31U

/* Definition for field NUM_MM in Register L2CFG */
#define L2CFG_NUM_MM_BIT_START                              24U
#define L2CFG_NUM_MM_BIT_END                                27U

/* Definition for field RESV_2 in Register L2CFG */
#define L2CFG_RESV_2_BIT_START                              20U
#define L2CFG_RESV_2_BIT_END                                23U

/* Definition for field MMID in Register L2CFG */
#define L2CFG_MMID_BIT_START                                16U
#define L2CFG_MMID_BIT_END                                  19U

/* Definition for field RESV_3 in Register L2CFG */
#define L2CFG_RESV_3_BIT_START                              10U
#define L2CFG_RESV_3_BIT_END                                15U

/* Definition for field IP in Register L2CFG */
#define L2CFG_IP_BIT_START                                  9U
#define L2CFG_IP_BIT_END                                    9U

/* Definition for field ID in Register L2CFG */
#define L2CFG_ID_BIT_START                                  8U
#define L2CFG_ID_BIT_END                                    8U

/* Definition for field RESV_4 in Register L2CFG */
#define L2CFG_RESV_4_BIT_START                              5U
#define L2CFG_RESV_4_BIT_END                                7U

/* Definition for field L2CC in Register L2CFG */
#define L2CFG_L2CC_BIT_START                                3U
#define L2CFG_L2CC_BIT_END                                  4U

/* Definition for field L2MODE in Register L2CFG */
#define L2CFG_L2MODE_BIT_START                              0U
#define L2CFG_L2MODE_BIT_END                                2U

/* Definition for field RESV_1 in Register L1PCFG */
#define L1PCFG_RESV_1_BIT_START                             3U
#define L1PCFG_RESV_1_BIT_END                               31U

/* Definition for field L1PMODE in Register L1PCFG */
#define L1PCFG_L1PMODE_BIT_START                            0U
#define L1PCFG_L1PMODE_BIT_END                              2U

/* Definition for field RESV_1 in Register L1PCC */
#define L1PCC_RESV_1_BIT_START                              19U
#define L1PCC_RESV_1_BIT_END                                31U

/* Definition for field POPER in Register L1PCC */
#define L1PCC_POPER_BIT_START                               16U
#define L1PCC_POPER_BIT_END                                 18U

/* Definition for field RESV_2 in Register L1PCC */
#define L1PCC_RESV_2_BIT_START                              3U
#define L1PCC_RESV_2_BIT_END                                15U

/* Definition for field OPER in Register L1PCC */
#define L1PCC_OPER_BIT_START                                0U
#define L1PCC_OPER_BIT_END                                  2U

/* Definition for field RESV_1 in Register L1DCFG */
#define L1DCFG_RESV_1_BIT_START                             3U
#define L1DCFG_RESV_1_BIT_END                               31U

/* Definition for field L1DMODE in Register L1DCFG */
#define L1DCFG_L1DMODE_BIT_START                            0U
#define L1DCFG_L1DMODE_BIT_END                              2U

/* Definition for field RESV_1 in Register L1DCC */
#define L1DCC_RESV_1_BIT_START                              19U
#define L1DCC_RESV_1_BIT_END                                31U

/* Definition for field POPER in Register L1DCC */
#define L1DCC_POPER_BIT_START                               16U
#define L1DCC_POPER_BIT_END                                 18U

/* Definition for field RESV_2 in Register L1DCC */
#define L1DCC_RESV_2_BIT_START                              3U
#define L1DCC_RESV_2_BIT_END                                15U

/* Definition for field OPER in Register L1DCC */
#define L1DCC_OPER_BIT_START                                0U
#define L1DCC_OPER_BIT_END                                  2U

/* Definition for field RESV_1 in Register CPUARBU */
#define CPUARBU_RESV_1_BIT_START                            19U
#define CPUARBU_RESV_1_BIT_END                              31U

/* Definition for field PRI in Register CPUARBU */
#define CPUARBU_PRI_BIT_START                               16U
#define CPUARBU_PRI_BIT_END                                 18U

/* Definition for field RESV_2 in Register CPUARBU */
#define CPUARBU_RESV_2_BIT_START                            6U
#define CPUARBU_RESV_2_BIT_END                              15U

/* Definition for field MAXWAIT in Register CPUARBU */
#define CPUARBU_MAXWAIT_BIT_START                           0U
#define CPUARBU_MAXWAIT_BIT_END                             5U

/* Definition for field RESV_1 in Register IDMAARBU */
#define IDMAARBU_RESV_1_BIT_START                           6U
#define IDMAARBU_RESV_1_BIT_END                             31U

/* Definition for field MAXWAIT in Register IDMAARBU */
#define IDMAARBU_MAXWAIT_BIT_START                          0U
#define IDMAARBU_MAXWAIT_BIT_END                            5U

/* Definition for field RESV_1 in Register SDMAARBU */
#define SDMAARBU_RESV_1_BIT_START                           6U
#define SDMAARBU_RESV_1_BIT_END                             31U

/* Definition for field MAXWAIT in Register SDMAARBU */
#define SDMAARBU_MAXWAIT_BIT_START                          0U
#define SDMAARBU_MAXWAIT_BIT_END                            5U

/* Definition for field RESV_1 in Register UCARBU */
#define UCARBU_RESV_1_BIT_START                             6U
#define UCARBU_RESV_1_BIT_END                               31U

/* Definition for field MAXWAIT in Register UCARBU */
#define UCARBU_MAXWAIT_BIT_START                            0U
#define UCARBU_MAXWAIT_BIT_END                              5U

/* Definition for field RESV_1 in Register MDMAARBU */
#define MDMAARBU_RESV_1_BIT_START                           27U
#define MDMAARBU_RESV_1_BIT_END                             31U

/* Definition for field UPRI in Register MDMAARBU */
#define MDMAARBU_UPRI_BIT_START                             24U
#define MDMAARBU_UPRI_BIT_END                               26U

/* Definition for field RESV_2 in Register MDMAARBU */
#define MDMAARBU_RESV_2_BIT_START                           19U
#define MDMAARBU_RESV_2_BIT_END                             23U

/* Definition for field PRI in Register MDMAARBU */
#define MDMAARBU_PRI_BIT_START                              16U
#define MDMAARBU_PRI_BIT_END                                18U

/* Definition for field RESV_3 in Register MDMAARBU */
#define MDMAARBU_RESV_3_BIT_START                           0U
#define MDMAARBU_RESV_3_BIT_END                             15U

/* Definition for field RESV_1 in Register CPUARBD */
#define CPUARBD_RESV_1_BIT_START                            19U
#define CPUARBD_RESV_1_BIT_END                              31U

/* Definition for field PRI in Register CPUARBD */
#define CPUARBD_PRI_BIT_START                               16U
#define CPUARBD_PRI_BIT_END                                 18U

/* Definition for field RESV_2 in Register CPUARBD */
#define CPUARBD_RESV_2_BIT_START                            6U
#define CPUARBD_RESV_2_BIT_END                              15U

/* Definition for field MAXWAIT in Register CPUARBD */
#define CPUARBD_MAXWAIT_BIT_START                           0U
#define CPUARBD_MAXWAIT_BIT_END                             5U

/* Definition for field RESV_1 in Register IDMAARBD */
#define IDMAARBD_RESV_1_BIT_START                           6U
#define IDMAARBD_RESV_1_BIT_END                             31U

/* Definition for field MAXWAIT in Register IDMAARBD */
#define IDMAARBD_MAXWAIT_BIT_START                          0U
#define IDMAARBD_MAXWAIT_BIT_END                            5U

/* Definition for field RESV_1 in Register SDMAARBD */
#define SDMAARBD_RESV_1_BIT_START                           6U
#define SDMAARBD_RESV_1_BIT_END                             31U

/* Definition for field MAXWAIT in Register SDMAARBD */
#define SDMAARBD_MAXWAIT_BIT_START                          0U
#define SDMAARBD_MAXWAIT_BIT_END                            5U

/* Definition for field RESV_1 in Register UCARBD */
#define UCARBD_RESV_1_BIT_START                             6U
#define UCARBD_RESV_1_BIT_END                               31U

/* Definition for field MAXWAIT in Register UCARBD */
#define UCARBD_MAXWAIT_BIT_START                            0U
#define UCARBD_MAXWAIT_BIT_END                              5U

/* Definition for field ADDR in Register L2WBAR */
#define L2WBAR_ADDR_BIT_START                               0U
#define L2WBAR_ADDR_BIT_END                                 31U

/* Definition for field RESV_1 in Register L2WWC */
#define L2WWC_RESV_1_BIT_START                              16U
#define L2WWC_RESV_1_BIT_END                                31U

/* Definition for field WC in Register L2WWC */
#define L2WWC_WC_BIT_START                                  0U
#define L2WWC_WC_BIT_END                                    15U

/* Definition for field ADDR in Register L2WIBAR */
#define L2WIBAR_ADDR_BIT_START                              0U
#define L2WIBAR_ADDR_BIT_END                                31U

/* Definition for field RESV_1 in Register L2WIWC */
#define L2WIWC_RESV_1_BIT_START                             16U
#define L2WIWC_RESV_1_BIT_END                               31U

/* Definition for field WC in Register L2WIWC */
#define L2WIWC_WC_BIT_START                                 0U
#define L2WIWC_WC_BIT_END                                   15U

/* Definition for field ADDR in Register L2IBAR */
#define L2IBAR_ADDR_BIT_START                               0U
#define L2IBAR_ADDR_BIT_END                                 31U

/* Definition for field RESV_1 in Register L2IWC */
#define L2IWC_RESV_1_BIT_START                              16U
#define L2IWC_RESV_1_BIT_END                                31U

/* Definition for field WC in Register L2IWC */
#define L2IWC_WC_BIT_START                                  0U
#define L2IWC_WC_BIT_END                                    15U

/* Definition for field ADDR in Register L1PIBAR */
#define L1PIBAR_ADDR_BIT_START                              0U
#define L1PIBAR_ADDR_BIT_END                                31U

/* Definition for field RESV_1 in Register L1PIWC */
#define L1PIWC_RESV_1_BIT_START                             16U
#define L1PIWC_RESV_1_BIT_END                               31U

/* Definition for field WC in Register L1PIWC */
#define L1PIWC_WC_BIT_START                                 0U
#define L1PIWC_WC_BIT_END                                   15U

/* Definition for field ADDR in Register L1DWIBAR */
#define L1DWIBAR_ADDR_BIT_START                             0U
#define L1DWIBAR_ADDR_BIT_END                               31U

/* Definition for field RESV_1 in Register L1DWIWC */
#define L1DWIWC_RESV_1_BIT_START                            16U
#define L1DWIWC_RESV_1_BIT_END                              31U

/* Definition for field WC in Register L1DWIWC */
#define L1DWIWC_WC_BIT_START                                0U
#define L1DWIWC_WC_BIT_END                                  15U

/* Definition for field ADDR in Register L1DWBAR */
#define L1DWBAR_ADDR_BIT_START                              0U
#define L1DWBAR_ADDR_BIT_END                                31U

/* Definition for field RESV_1 in Register L1DWWC */
#define L1DWWC_RESV_1_BIT_START                             16U
#define L1DWWC_RESV_1_BIT_END                               31U

/* Definition for field WC in Register L1DWWC */
#define L1DWWC_WC_BIT_START                                 0U
#define L1DWWC_WC_BIT_END                                   15U

/* Definition for field ADDR in Register L1DIBAR */
#define L1DIBAR_ADDR_BIT_START                              0U
#define L1DIBAR_ADDR_BIT_END                                31U

/* Definition for field RESV_1 in Register L1DIWC */
#define L1DIWC_RESV_1_BIT_START                             16U
#define L1DIWC_RESV_1_BIT_END                               31U

/* Definition for field WC in Register L1DIWC */
#define L1DIWC_WC_BIT_START                                 0U
#define L1DIWC_WC_BIT_END                                   15U

/* Definition for field C in Register L2WB */
#define L2WB_C_BIT_START                                    0U
#define L2WB_C_BIT_END                                      31U

/* Definition for field C in Register L2WBINV */
#define L2WBINV_C_BIT_START                                 0U
#define L2WBINV_C_BIT_END                                   31U

/* Definition for field I in Register L2INV */
#define L2INV_I_BIT_START                                   0U
#define L2INV_I_BIT_END                                     31U

/* Definition for field I in Register L1PINV */
#define L1PINV_I_BIT_START                                  0U
#define L1PINV_I_BIT_END                                    31U

/* Definition for field C in Register L1DWB */
#define L1DWB_C_BIT_START                                   0U
#define L1DWB_C_BIT_END                                     31U

/* Definition for field C in Register L1DWBINV */
#define L1DWBINV_C_BIT_START                                0U
#define L1DWBINV_C_BIT_END                                  31U

/* Definition for field I in Register L1DINV */
#define L1DINV_I_BIT_START                                  0U
#define L1DINV_I_BIT_END                                    31U

/* Definition for field RESV1 in Register L2EDSTAT */
#define L2EDSTAT_RESV1_BIT_START                            24U
#define L2EDSTAT_RESV1_BIT_END                              31U

/* Definition for field BITPOS in Register L2EDSTAT */
#define L2EDSTAT_BITPOS_BIT_START                           16U
#define L2EDSTAT_BITPOS_BIT_END                             23U

/* Definition for field RESV2 in Register L2EDSTAT */
#define L2EDSTAT_RESV2_BIT_START                            10U
#define L2EDSTAT_RESV2_BIT_END                              15U

/* Definition for field NERR in Register L2EDSTAT */
#define L2EDSTAT_NERR_BIT_START                             8U
#define L2EDSTAT_NERR_BIT_END                               9U

/* Definition for field VERR in Register L2EDSTAT */
#define L2EDSTAT_VERR_BIT_START                             7U
#define L2EDSTAT_VERR_BIT_END                               7U

/* Definition for field DMAERR in Register L2EDSTAT */
#define L2EDSTAT_DMAERR_BIT_START                           6U
#define L2EDSTAT_DMAERR_BIT_END                             6U

/* Definition for field PERR in Register L2EDSTAT */
#define L2EDSTAT_PERR_BIT_START                             5U
#define L2EDSTAT_PERR_BIT_END                               5U

/* Definition for field DERR in Register L2EDSTAT */
#define L2EDSTAT_DERR_BIT_START                             4U
#define L2EDSTAT_DERR_BIT_END                               4U

/* Definition for field SUSP in Register L2EDSTAT */
#define L2EDSTAT_SUSP_BIT_START                             3U
#define L2EDSTAT_SUSP_BIT_END                               3U

/* Definition for field DIS in Register L2EDSTAT */
#define L2EDSTAT_DIS_BIT_START                              2U
#define L2EDSTAT_DIS_BIT_END                                2U

/* Definition for field RESV3 in Register L2EDSTAT */
#define L2EDSTAT_RESV3_BIT_START                            1U
#define L2EDSTAT_RESV3_BIT_END                              1U

/* Definition for field EN in Register L2EDSTAT */
#define L2EDSTAT_EN_BIT_START                               0U
#define L2EDSTAT_EN_BIT_END                                 0U

/* Definition for field RESV1 in Register L2EDCMD */
#define L2EDCMD_RESV1_BIT_START                             8U
#define L2EDCMD_RESV1_BIT_END                               31U

/* Definition for field VCLR in Register L2EDCMD */
#define L2EDCMD_VCLR_BIT_START                              7U
#define L2EDCMD_VCLR_BIT_END                                7U

/* Definition for field DMACLR in Register L2EDCMD */
#define L2EDCMD_DMACLR_BIT_START                            6U
#define L2EDCMD_DMACLR_BIT_END                              6U

/* Definition for field PCLR in Register L2EDCMD */
#define L2EDCMD_PCLR_BIT_START                              5U
#define L2EDCMD_PCLR_BIT_END                                5U

/* Definition for field DCLR in Register L2EDCMD */
#define L2EDCMD_DCLR_BIT_START                              4U
#define L2EDCMD_DCLR_BIT_END                                4U

/* Definition for field SUSP in Register L2EDCMD */
#define L2EDCMD_SUSP_BIT_START                              3U
#define L2EDCMD_SUSP_BIT_END                                3U

/* Definition for field DIS in Register L2EDCMD */
#define L2EDCMD_DIS_BIT_START                               2U
#define L2EDCMD_DIS_BIT_END                                 2U

/* Definition for field RESV3 in Register L2EDCMD */
#define L2EDCMD_RESV3_BIT_START                             1U
#define L2EDCMD_RESV3_BIT_END                               1U

/* Definition for field EN in Register L2EDCMD */
#define L2EDCMD_EN_BIT_START                                0U
#define L2EDCMD_EN_BIT_END                                  0U

/* Definition for field ADDR in Register L2EDADDR */
#define L2EDADDR_ADDR_BIT_START                             5U
#define L2EDADDR_ADDR_BIT_END                               31U

/* Definition for field WAY in Register L2EDADDR */
#define L2EDADDR_WAY_BIT_START                              3U
#define L2EDADDR_WAY_BIT_END                                4U

/* Definition for field RESV1 in Register L2EDADDR */
#define L2EDADDR_RESV1_BIT_START                            1U
#define L2EDADDR_RESV1_BIT_END                              2U

/* Definition for field RAM in Register L2EDADDR */
#define L2EDADDR_RAM_BIT_START                              0U
#define L2EDADDR_RAM_BIT_END                                0U

/* Definition for field L2EDPEN0 in Register L2EDPEN0 */
#define L2EDPEN0_L2EDPEN0_BIT_START                         0U
#define L2EDPEN0_L2EDPEN0_BIT_END                           31U

/* Definition for field L2EDPEN1 in Register L2EDPEN1 */
#define L2EDPEN1_L2EDPEN1_BIT_START                         0U
#define L2EDPEN1_L2EDPEN1_BIT_END                           31U

/* Definition for field RESV1 in Register L2EDCPEC */
#define L2EDCPEC_RESV1_BIT_START                            8U
#define L2EDCPEC_RESV1_BIT_END                              31U

/* Definition for field CNT in Register L2EDCPEC */
#define L2EDCPEC_CNT_BIT_START                              0U
#define L2EDCPEC_CNT_BIT_END                                7U

/* Definition for field RESV1 in Register L2EDCNEC */
#define L2EDCNEC_RESV1_BIT_START                            8U
#define L2EDCNEC_RESV1_BIT_END                              31U

/* Definition for field CNT in Register L2EDCNEC */
#define L2EDCNEC_CNT_BIT_START                              0U
#define L2EDCNEC_CNT_BIT_END                                7U

/* Definition for field ERR in Register MDMAERR */
#define MDMAERR_ERR_BIT_START                               29U
#define MDMAERR_ERR_BIT_END                                 31U

/* Definition for field RESV_2 in Register MDMAERR */
#define MDMAERR_RESV_2_BIT_START                            12U
#define MDMAERR_RESV_2_BIT_END                              28U

/* Definition for field XID in Register MDMAERR */
#define MDMAERR_XID_BIT_START                               8U
#define MDMAERR_XID_BIT_END                                 11U

/* Definition for field RESV_1 in Register MDMAERR */
#define MDMAERR_RESV_1_BIT_START                            3U
#define MDMAERR_RESV_1_BIT_END                              7U

/* Definition for field STAT in Register MDMAERR */
#define MDMAERR_STAT_BIT_START                              0U
#define MDMAERR_STAT_BIT_END                                2U

/* Definition for field RESV_1 in Register MDMAERRCLR */
#define MDMAERRCLR_RESV_1_BIT_START                         1U
#define MDMAERRCLR_RESV_1_BIT_END                           31U

/* Definition for field CLR in Register MDMAERRCLR */
#define MDMAERRCLR_CLR_BIT_START                            0U
#define MDMAERRCLR_CLR_BIT_END                              0U

/* Definition for field RESV1 in Register L2EDCEN */
#define L2EDCEN_RESV1_BIT_START                             5U
#define L2EDCEN_RESV1_BIT_END                               31U

/* Definition for field SDMAEN in Register L2EDCEN */
#define L2EDCEN_SDMAEN_BIT_START                            4U
#define L2EDCEN_SDMAEN_BIT_END                              4U

/* Definition for field PL2SEN in Register L2EDCEN */
#define L2EDCEN_PL2SEN_BIT_START                            3U
#define L2EDCEN_PL2SEN_BIT_END                              3U

/* Definition for field DL2SEN in Register L2EDCEN */
#define L2EDCEN_DL2SEN_BIT_START                            2U
#define L2EDCEN_DL2SEN_BIT_END                              2U

/* Definition for field PL2CEN in Register L2EDCEN */
#define L2EDCEN_PL2CEN_BIT_START                            1U
#define L2EDCEN_PL2CEN_BIT_END                              1U

/* Definition for field DL2CEN in Register L2EDCEN */
#define L2EDCEN_DL2CEN_BIT_START                            0U
#define L2EDCEN_DL2CEN_BIT_END                              0U

/* Definition for field RESV1 in Register L1PEDSTAT */
#define L1PEDSTAT_RESV1_BIT_START                           7U
#define L1PEDSTAT_RESV1_BIT_END                             31U

/* Definition for field DMAERR in Register L1PEDSTAT */
#define L1PEDSTAT_DMAERR_BIT_START                          6U
#define L1PEDSTAT_DMAERR_BIT_END                            6U

/* Definition for field PERR in Register L1PEDSTAT */
#define L1PEDSTAT_PERR_BIT_START                            5U
#define L1PEDSTAT_PERR_BIT_END                              5U

/* Definition for field RESV2 in Register L1PEDSTAT */
#define L1PEDSTAT_RESV2_BIT_START                           4U
#define L1PEDSTAT_RESV2_BIT_END                             4U

/* Definition for field SUSP in Register L1PEDSTAT */
#define L1PEDSTAT_SUSP_BIT_START                            3U
#define L1PEDSTAT_SUSP_BIT_END                              3U

/* Definition for field DIS in Register L1PEDSTAT */
#define L1PEDSTAT_DIS_BIT_START                             2U
#define L1PEDSTAT_DIS_BIT_END                               2U

/* Definition for field RESV3 in Register L1PEDSTAT */
#define L1PEDSTAT_RESV3_BIT_START                           1U
#define L1PEDSTAT_RESV3_BIT_END                             1U

/* Definition for field EN in Register L1PEDSTAT */
#define L1PEDSTAT_EN_BIT_START                              0U
#define L1PEDSTAT_EN_BIT_END                                0U

/* Definition for field RESV1 in Register L1PEDCMD */
#define L1PEDCMD_RESV1_BIT_START                            7U
#define L1PEDCMD_RESV1_BIT_END                              31U

/* Definition for field DMACLR in Register L1PEDCMD */
#define L1PEDCMD_DMACLR_BIT_START                           6U
#define L1PEDCMD_DMACLR_BIT_END                             6U

/* Definition for field PCLR in Register L1PEDCMD */
#define L1PEDCMD_PCLR_BIT_START                             5U
#define L1PEDCMD_PCLR_BIT_END                               5U

/* Definition for field RESV2 in Register L1PEDCMD */
#define L1PEDCMD_RESV2_BIT_START                            4U
#define L1PEDCMD_RESV2_BIT_END                              4U

/* Definition for field SUSP in Register L1PEDCMD */
#define L1PEDCMD_SUSP_BIT_START                             3U
#define L1PEDCMD_SUSP_BIT_END                               3U

/* Definition for field DIS in Register L1PEDCMD */
#define L1PEDCMD_DIS_BIT_START                              2U
#define L1PEDCMD_DIS_BIT_END                                2U

/* Definition for field RESV3 in Register L1PEDCMD */
#define L1PEDCMD_RESV3_BIT_START                            1U
#define L1PEDCMD_RESV3_BIT_END                              1U

/* Definition for field EN in Register L1PEDCMD */
#define L1PEDCMD_EN_BIT_START                               0U
#define L1PEDCMD_EN_BIT_END                                 0U

/* Definition for field ADDR in Register L1PEDADDR */
#define L1PEDADDR_ADDR_BIT_START                            5U
#define L1PEDADDR_ADDR_BIT_END                              31U

/* Definition for field RESV1 in Register L1PEDADDR */
#define L1PEDADDR_RESV1_BIT_START                           1U
#define L1PEDADDR_RESV1_BIT_END                             4U

/* Definition for field RAM in Register L1PEDADDR */
#define L1PEDADDR_RAM_BIT_START                             0U
#define L1PEDADDR_RAM_BIT_END                               0U

#if 0
/* Definition for field RESV_1 in Register MAR_0 */
#define MAR_0_RESV_1_BIT_START                              4U
#define MAR_0_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_0 */
#define MAR_0_PFX_BIT_START                                 3U
#define MAR_0_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_0 */
#define MAR_0_PCX_BIT_START                                 2U
#define MAR_0_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_0 */
#define MAR_0_WTE_BIT_START                                 1U
#define MAR_0_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_0 */
#define MAR_0_PC_BIT_START                                  0U
#define MAR_0_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_1 */
#define MAR_1_RESV_1_BIT_START                              4U
#define MAR_1_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_1 */
#define MAR_1_PFX_BIT_START                                 3U
#define MAR_1_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_1 */
#define MAR_1_PCX_BIT_START                                 2U
#define MAR_1_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_1 */
#define MAR_1_WTE_BIT_START                                 1U
#define MAR_1_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_1 */
#define MAR_1_PC_BIT_START                                  0U
#define MAR_1_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_2 */
#define MAR_2_RESV_1_BIT_START                              4U
#define MAR_2_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_2 */
#define MAR_2_PFX_BIT_START                                 3U
#define MAR_2_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_2 */
#define MAR_2_PCX_BIT_START                                 2U
#define MAR_2_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_2 */
#define MAR_2_WTE_BIT_START                                 1U
#define MAR_2_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_2 */
#define MAR_2_PC_BIT_START                                  0U
#define MAR_2_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_3 */
#define MAR_3_RESV_1_BIT_START                              4U
#define MAR_3_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_3 */
#define MAR_3_PFX_BIT_START                                 3U
#define MAR_3_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_3 */
#define MAR_3_PCX_BIT_START                                 2U
#define MAR_3_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_3 */
#define MAR_3_WTE_BIT_START                                 1U
#define MAR_3_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_3 */
#define MAR_3_PC_BIT_START                                  0U
#define MAR_3_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_4 */
#define MAR_4_RESV_1_BIT_START                              4U
#define MAR_4_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_4 */
#define MAR_4_PFX_BIT_START                                 3U
#define MAR_4_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_4 */
#define MAR_4_PCX_BIT_START                                 2U
#define MAR_4_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_4 */
#define MAR_4_WTE_BIT_START                                 1U
#define MAR_4_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_4 */
#define MAR_4_PC_BIT_START                                  0U
#define MAR_4_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_5 */
#define MAR_5_RESV_1_BIT_START                              4U
#define MAR_5_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_5 */
#define MAR_5_PFX_BIT_START                                 3U
#define MAR_5_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_5 */
#define MAR_5_PCX_BIT_START                                 2U
#define MAR_5_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_5 */
#define MAR_5_WTE_BIT_START                                 1U
#define MAR_5_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_5 */
#define MAR_5_PC_BIT_START                                  0U
#define MAR_5_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_6 */
#define MAR_6_RESV_1_BIT_START                              4U
#define MAR_6_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_6 */
#define MAR_6_PFX_BIT_START                                 3U
#define MAR_6_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_6 */
#define MAR_6_PCX_BIT_START                                 2U
#define MAR_6_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_6 */
#define MAR_6_WTE_BIT_START                                 1U
#define MAR_6_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_6 */
#define MAR_6_PC_BIT_START                                  0U
#define MAR_6_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_7 */
#define MAR_7_RESV_1_BIT_START                              4U
#define MAR_7_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_7 */
#define MAR_7_PFX_BIT_START                                 3U
#define MAR_7_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_7 */
#define MAR_7_PCX_BIT_START                                 2U
#define MAR_7_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_7 */
#define MAR_7_WTE_BIT_START                                 1U
#define MAR_7_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_7 */
#define MAR_7_PC_BIT_START                                  0U
#define MAR_7_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_8 */
#define MAR_8_RESV_1_BIT_START                              4U
#define MAR_8_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_8 */
#define MAR_8_PFX_BIT_START                                 3U
#define MAR_8_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_8 */
#define MAR_8_PCX_BIT_START                                 2U
#define MAR_8_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_8 */
#define MAR_8_WTE_BIT_START                                 1U
#define MAR_8_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_8 */
#define MAR_8_PC_BIT_START                                  0U
#define MAR_8_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_9 */
#define MAR_9_RESV_1_BIT_START                              4U
#define MAR_9_RESV_1_BIT_END                                31U

/* Definition for field PFX in Register MAR_9 */
#define MAR_9_PFX_BIT_START                                 3U
#define MAR_9_PFX_BIT_END                                   3U

/* Definition for field PCX in Register MAR_9 */
#define MAR_9_PCX_BIT_START                                 2U
#define MAR_9_PCX_BIT_END                                   2U

/* Definition for field WTE in Register MAR_9 */
#define MAR_9_WTE_BIT_START                                 1U
#define MAR_9_WTE_BIT_END                                   1U

/* Definition for field PC in Register MAR_9 */
#define MAR_9_PC_BIT_START                                  0U
#define MAR_9_PC_BIT_END                                    0U

/* Definition for field RESV_1 in Register MAR_10 */
#define MAR_10_RESV_1_BIT_START                             4U
#define MAR_10_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_10 */
#define MAR_10_PFX_BIT_START                                3U
#define MAR_10_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_10 */
#define MAR_10_PCX_BIT_START                                2U
#define MAR_10_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_10 */
#define MAR_10_WTE_BIT_START                                1U
#define MAR_10_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_10 */
#define MAR_10_PC_BIT_START                                 0U
#define MAR_10_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_11 */
#define MAR_11_RESV_1_BIT_START                             4U
#define MAR_11_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_11 */
#define MAR_11_PFX_BIT_START                                3U
#define MAR_11_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_11 */
#define MAR_11_PCX_BIT_START                                2U
#define MAR_11_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_11 */
#define MAR_11_WTE_BIT_START                                1U
#define MAR_11_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_11 */
#define MAR_11_PC_BIT_START                                 0U
#define MAR_11_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_12 */
#define MAR_12_RESV_1_BIT_START                             4U
#define MAR_12_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_12 */
#define MAR_12_PFX_BIT_START                                3U
#define MAR_12_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_12 */
#define MAR_12_PCX_BIT_START                                2U
#define MAR_12_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_12 */
#define MAR_12_WTE_BIT_START                                1U
#define MAR_12_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_12 */
#define MAR_12_PC_BIT_START                                 0U
#define MAR_12_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_13 */
#define MAR_13_RESV_1_BIT_START                             4U
#define MAR_13_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_13 */
#define MAR_13_PFX_BIT_START                                3U
#define MAR_13_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_13 */
#define MAR_13_PCX_BIT_START                                2U
#define MAR_13_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_13 */
#define MAR_13_WTE_BIT_START                                1U
#define MAR_13_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_13 */
#define MAR_13_PC_BIT_START                                 0U
#define MAR_13_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_14 */
#define MAR_14_RESV_1_BIT_START                             4U
#define MAR_14_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_14 */
#define MAR_14_PFX_BIT_START                                3U
#define MAR_14_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_14 */
#define MAR_14_PCX_BIT_START                                2U
#define MAR_14_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_14 */
#define MAR_14_WTE_BIT_START                                1U
#define MAR_14_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_14 */
#define MAR_14_PC_BIT_START                                 0U
#define MAR_14_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_15 */
#define MAR_15_RESV_1_BIT_START                             4U
#define MAR_15_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_15 */
#define MAR_15_PFX_BIT_START                                3U
#define MAR_15_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_15 */
#define MAR_15_PCX_BIT_START                                2U
#define MAR_15_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_15 */
#define MAR_15_WTE_BIT_START                                1U
#define MAR_15_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_15 */
#define MAR_15_PC_BIT_START                                 0U
#define MAR_15_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_16 */
#define MAR_16_RESV_1_BIT_START                             4U
#define MAR_16_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_16 */
#define MAR_16_PFX_BIT_START                                3U
#define MAR_16_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_16 */
#define MAR_16_PCX_BIT_START                                2U
#define MAR_16_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_16 */
#define MAR_16_WTE_BIT_START                                1U
#define MAR_16_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_16 */
#define MAR_16_PC_BIT_START                                 0U
#define MAR_16_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_17 */
#define MAR_17_RESV_1_BIT_START                             4U
#define MAR_17_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_17 */
#define MAR_17_PFX_BIT_START                                3U
#define MAR_17_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_17 */
#define MAR_17_PCX_BIT_START                                2U
#define MAR_17_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_17 */
#define MAR_17_WTE_BIT_START                                1U
#define MAR_17_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_17 */
#define MAR_17_PC_BIT_START                                 0U
#define MAR_17_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_18 */
#define MAR_18_RESV_1_BIT_START                             4U
#define MAR_18_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_18 */
#define MAR_18_PFX_BIT_START                                3U
#define MAR_18_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_18 */
#define MAR_18_PCX_BIT_START                                2U
#define MAR_18_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_18 */
#define MAR_18_WTE_BIT_START                                1U
#define MAR_18_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_18 */
#define MAR_18_PC_BIT_START                                 0U
#define MAR_18_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_19 */
#define MAR_19_RESV_1_BIT_START                             4U
#define MAR_19_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_19 */
#define MAR_19_PFX_BIT_START                                3U
#define MAR_19_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_19 */
#define MAR_19_PCX_BIT_START                                2U
#define MAR_19_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_19 */
#define MAR_19_WTE_BIT_START                                1U
#define MAR_19_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_19 */
#define MAR_19_PC_BIT_START                                 0U
#define MAR_19_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_20 */
#define MAR_20_RESV_1_BIT_START                             4U
#define MAR_20_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_20 */
#define MAR_20_PFX_BIT_START                                3U
#define MAR_20_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_20 */
#define MAR_20_PCX_BIT_START                                2U
#define MAR_20_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_20 */
#define MAR_20_WTE_BIT_START                                1U
#define MAR_20_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_20 */
#define MAR_20_PC_BIT_START                                 0U
#define MAR_20_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_21 */
#define MAR_21_RESV_1_BIT_START                             4U
#define MAR_21_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_21 */
#define MAR_21_PFX_BIT_START                                3U
#define MAR_21_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_21 */
#define MAR_21_PCX_BIT_START                                2U
#define MAR_21_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_21 */
#define MAR_21_WTE_BIT_START                                1U
#define MAR_21_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_21 */
#define MAR_21_PC_BIT_START                                 0U
#define MAR_21_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_22 */
#define MAR_22_RESV_1_BIT_START                             4U
#define MAR_22_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_22 */
#define MAR_22_PFX_BIT_START                                3U
#define MAR_22_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_22 */
#define MAR_22_PCX_BIT_START                                2U
#define MAR_22_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_22 */
#define MAR_22_WTE_BIT_START                                1U
#define MAR_22_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_22 */
#define MAR_22_PC_BIT_START                                 0U
#define MAR_22_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_23 */
#define MAR_23_RESV_1_BIT_START                             4U
#define MAR_23_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_23 */
#define MAR_23_PFX_BIT_START                                3U
#define MAR_23_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_23 */
#define MAR_23_PCX_BIT_START                                2U
#define MAR_23_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_23 */
#define MAR_23_WTE_BIT_START                                1U
#define MAR_23_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_23 */
#define MAR_23_PC_BIT_START                                 0U
#define MAR_23_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_24 */
#define MAR_24_RESV_1_BIT_START                             4U
#define MAR_24_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_24 */
#define MAR_24_PFX_BIT_START                                3U
#define MAR_24_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_24 */
#define MAR_24_PCX_BIT_START                                2U
#define MAR_24_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_24 */
#define MAR_24_WTE_BIT_START                                1U
#define MAR_24_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_24 */
#define MAR_24_PC_BIT_START                                 0U
#define MAR_24_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_25 */
#define MAR_25_RESV_1_BIT_START                             4U
#define MAR_25_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_25 */
#define MAR_25_PFX_BIT_START                                3U
#define MAR_25_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_25 */
#define MAR_25_PCX_BIT_START                                2U
#define MAR_25_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_25 */
#define MAR_25_WTE_BIT_START                                1U
#define MAR_25_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_25 */
#define MAR_25_PC_BIT_START                                 0U
#define MAR_25_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_26 */
#define MAR_26_RESV_1_BIT_START                             4U
#define MAR_26_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_26 */
#define MAR_26_PFX_BIT_START                                3U
#define MAR_26_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_26 */
#define MAR_26_PCX_BIT_START                                2U
#define MAR_26_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_26 */
#define MAR_26_WTE_BIT_START                                1U
#define MAR_26_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_26 */
#define MAR_26_PC_BIT_START                                 0U
#define MAR_26_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_27 */
#define MAR_27_RESV_1_BIT_START                             4U
#define MAR_27_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_27 */
#define MAR_27_PFX_BIT_START                                3U
#define MAR_27_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_27 */
#define MAR_27_PCX_BIT_START                                2U
#define MAR_27_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_27 */
#define MAR_27_WTE_BIT_START                                1U
#define MAR_27_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_27 */
#define MAR_27_PC_BIT_START                                 0U
#define MAR_27_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_28 */
#define MAR_28_RESV_1_BIT_START                             4U
#define MAR_28_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_28 */
#define MAR_28_PFX_BIT_START                                3U
#define MAR_28_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_28 */
#define MAR_28_PCX_BIT_START                                2U
#define MAR_28_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_28 */
#define MAR_28_WTE_BIT_START                                1U
#define MAR_28_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_28 */
#define MAR_28_PC_BIT_START                                 0U
#define MAR_28_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_29 */
#define MAR_29_RESV_1_BIT_START                             4U
#define MAR_29_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_29 */
#define MAR_29_PFX_BIT_START                                3U
#define MAR_29_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_29 */
#define MAR_29_PCX_BIT_START                                2U
#define MAR_29_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_29 */
#define MAR_29_WTE_BIT_START                                1U
#define MAR_29_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_29 */
#define MAR_29_PC_BIT_START                                 0U
#define MAR_29_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_30 */
#define MAR_30_RESV_1_BIT_START                             4U
#define MAR_30_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_30 */
#define MAR_30_PFX_BIT_START                                3U
#define MAR_30_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_30 */
#define MAR_30_PCX_BIT_START                                2U
#define MAR_30_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_30 */
#define MAR_30_WTE_BIT_START                                1U
#define MAR_30_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_30 */
#define MAR_30_PC_BIT_START                                 0U
#define MAR_30_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_31 */
#define MAR_31_RESV_1_BIT_START                             4U
#define MAR_31_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_31 */
#define MAR_31_PFX_BIT_START                                3U
#define MAR_31_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_31 */
#define MAR_31_PCX_BIT_START                                2U
#define MAR_31_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_31 */
#define MAR_31_WTE_BIT_START                                1U
#define MAR_31_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_31 */
#define MAR_31_PC_BIT_START                                 0U
#define MAR_31_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_32 */
#define MAR_32_RESV_1_BIT_START                             4U
#define MAR_32_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_32 */
#define MAR_32_PFX_BIT_START                                3U
#define MAR_32_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_32 */
#define MAR_32_PCX_BIT_START                                2U
#define MAR_32_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_32 */
#define MAR_32_WTE_BIT_START                                1U
#define MAR_32_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_32 */
#define MAR_32_PC_BIT_START                                 0U
#define MAR_32_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_33 */
#define MAR_33_RESV_1_BIT_START                             4U
#define MAR_33_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_33 */
#define MAR_33_PFX_BIT_START                                3U
#define MAR_33_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_33 */
#define MAR_33_PCX_BIT_START                                2U
#define MAR_33_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_33 */
#define MAR_33_WTE_BIT_START                                1U
#define MAR_33_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_33 */
#define MAR_33_PC_BIT_START                                 0U
#define MAR_33_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_34 */
#define MAR_34_RESV_1_BIT_START                             4U
#define MAR_34_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_34 */
#define MAR_34_PFX_BIT_START                                3U
#define MAR_34_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_34 */
#define MAR_34_PCX_BIT_START                                2U
#define MAR_34_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_34 */
#define MAR_34_WTE_BIT_START                                1U
#define MAR_34_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_34 */
#define MAR_34_PC_BIT_START                                 0U
#define MAR_34_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_35 */
#define MAR_35_RESV_1_BIT_START                             4U
#define MAR_35_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_35 */
#define MAR_35_PFX_BIT_START                                3U
#define MAR_35_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_35 */
#define MAR_35_PCX_BIT_START                                2U
#define MAR_35_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_35 */
#define MAR_35_WTE_BIT_START                                1U
#define MAR_35_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_35 */
#define MAR_35_PC_BIT_START                                 0U
#define MAR_35_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_36 */
#define MAR_36_RESV_1_BIT_START                             4U
#define MAR_36_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_36 */
#define MAR_36_PFX_BIT_START                                3U
#define MAR_36_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_36 */
#define MAR_36_PCX_BIT_START                                2U
#define MAR_36_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_36 */
#define MAR_36_WTE_BIT_START                                1U
#define MAR_36_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_36 */
#define MAR_36_PC_BIT_START                                 0U
#define MAR_36_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_37 */
#define MAR_37_RESV_1_BIT_START                             4U
#define MAR_37_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_37 */
#define MAR_37_PFX_BIT_START                                3U
#define MAR_37_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_37 */
#define MAR_37_PCX_BIT_START                                2U
#define MAR_37_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_37 */
#define MAR_37_WTE_BIT_START                                1U
#define MAR_37_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_37 */
#define MAR_37_PC_BIT_START                                 0U
#define MAR_37_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_38 */
#define MAR_38_RESV_1_BIT_START                             4U
#define MAR_38_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_38 */
#define MAR_38_PFX_BIT_START                                3U
#define MAR_38_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_38 */
#define MAR_38_PCX_BIT_START                                2U
#define MAR_38_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_38 */
#define MAR_38_WTE_BIT_START                                1U
#define MAR_38_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_38 */
#define MAR_38_PC_BIT_START                                 0U
#define MAR_38_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_39 */
#define MAR_39_RESV_1_BIT_START                             4U
#define MAR_39_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_39 */
#define MAR_39_PFX_BIT_START                                3U
#define MAR_39_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_39 */
#define MAR_39_PCX_BIT_START                                2U
#define MAR_39_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_39 */
#define MAR_39_WTE_BIT_START                                1U
#define MAR_39_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_39 */
#define MAR_39_PC_BIT_START                                 0U
#define MAR_39_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_40 */
#define MAR_40_RESV_1_BIT_START                             4U
#define MAR_40_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_40 */
#define MAR_40_PFX_BIT_START                                3U
#define MAR_40_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_40 */
#define MAR_40_PCX_BIT_START                                2U
#define MAR_40_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_40 */
#define MAR_40_WTE_BIT_START                                1U
#define MAR_40_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_40 */
#define MAR_40_PC_BIT_START                                 0U
#define MAR_40_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_41 */
#define MAR_41_RESV_1_BIT_START                             4U
#define MAR_41_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_41 */
#define MAR_41_PFX_BIT_START                                3U
#define MAR_41_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_41 */
#define MAR_41_PCX_BIT_START                                2U
#define MAR_41_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_41 */
#define MAR_41_WTE_BIT_START                                1U
#define MAR_41_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_41 */
#define MAR_41_PC_BIT_START                                 0U
#define MAR_41_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_42 */
#define MAR_42_RESV_1_BIT_START                             4U
#define MAR_42_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_42 */
#define MAR_42_PFX_BIT_START                                3U
#define MAR_42_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_42 */
#define MAR_42_PCX_BIT_START                                2U
#define MAR_42_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_42 */
#define MAR_42_WTE_BIT_START                                1U
#define MAR_42_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_42 */
#define MAR_42_PC_BIT_START                                 0U
#define MAR_42_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_43 */
#define MAR_43_RESV_1_BIT_START                             4U
#define MAR_43_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_43 */
#define MAR_43_PFX_BIT_START                                3U
#define MAR_43_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_43 */
#define MAR_43_PCX_BIT_START                                2U
#define MAR_43_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_43 */
#define MAR_43_WTE_BIT_START                                1U
#define MAR_43_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_43 */
#define MAR_43_PC_BIT_START                                 0U
#define MAR_43_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_44 */
#define MAR_44_RESV_1_BIT_START                             4U
#define MAR_44_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_44 */
#define MAR_44_PFX_BIT_START                                3U
#define MAR_44_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_44 */
#define MAR_44_PCX_BIT_START                                2U
#define MAR_44_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_44 */
#define MAR_44_WTE_BIT_START                                1U
#define MAR_44_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_44 */
#define MAR_44_PC_BIT_START                                 0U
#define MAR_44_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_45 */
#define MAR_45_RESV_1_BIT_START                             4U
#define MAR_45_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_45 */
#define MAR_45_PFX_BIT_START                                3U
#define MAR_45_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_45 */
#define MAR_45_PCX_BIT_START                                2U
#define MAR_45_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_45 */
#define MAR_45_WTE_BIT_START                                1U
#define MAR_45_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_45 */
#define MAR_45_PC_BIT_START                                 0U
#define MAR_45_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_46 */
#define MAR_46_RESV_1_BIT_START                             4U
#define MAR_46_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_46 */
#define MAR_46_PFX_BIT_START                                3U
#define MAR_46_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_46 */
#define MAR_46_PCX_BIT_START                                2U
#define MAR_46_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_46 */
#define MAR_46_WTE_BIT_START                                1U
#define MAR_46_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_46 */
#define MAR_46_PC_BIT_START                                 0U
#define MAR_46_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_47 */
#define MAR_47_RESV_1_BIT_START                             4U
#define MAR_47_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_47 */
#define MAR_47_PFX_BIT_START                                3U
#define MAR_47_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_47 */
#define MAR_47_PCX_BIT_START                                2U
#define MAR_47_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_47 */
#define MAR_47_WTE_BIT_START                                1U
#define MAR_47_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_47 */
#define MAR_47_PC_BIT_START                                 0U
#define MAR_47_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_48 */
#define MAR_48_RESV_1_BIT_START                             4U
#define MAR_48_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_48 */
#define MAR_48_PFX_BIT_START                                3U
#define MAR_48_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_48 */
#define MAR_48_PCX_BIT_START                                2U
#define MAR_48_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_48 */
#define MAR_48_WTE_BIT_START                                1U
#define MAR_48_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_48 */
#define MAR_48_PC_BIT_START                                 0U
#define MAR_48_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_49 */
#define MAR_49_RESV_1_BIT_START                             4U
#define MAR_49_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_49 */
#define MAR_49_PFX_BIT_START                                3U
#define MAR_49_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_49 */
#define MAR_49_PCX_BIT_START                                2U
#define MAR_49_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_49 */
#define MAR_49_WTE_BIT_START                                1U
#define MAR_49_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_49 */
#define MAR_49_PC_BIT_START                                 0U
#define MAR_49_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_50 */
#define MAR_50_RESV_1_BIT_START                             4U
#define MAR_50_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_50 */
#define MAR_50_PFX_BIT_START                                3U
#define MAR_50_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_50 */
#define MAR_50_PCX_BIT_START                                2U
#define MAR_50_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_50 */
#define MAR_50_WTE_BIT_START                                1U
#define MAR_50_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_50 */
#define MAR_50_PC_BIT_START                                 0U
#define MAR_50_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_51 */
#define MAR_51_RESV_1_BIT_START                             4U
#define MAR_51_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_51 */
#define MAR_51_PFX_BIT_START                                3U
#define MAR_51_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_51 */
#define MAR_51_PCX_BIT_START                                2U
#define MAR_51_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_51 */
#define MAR_51_WTE_BIT_START                                1U
#define MAR_51_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_51 */
#define MAR_51_PC_BIT_START                                 0U
#define MAR_51_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_52 */
#define MAR_52_RESV_1_BIT_START                             4U
#define MAR_52_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_52 */
#define MAR_52_PFX_BIT_START                                3U
#define MAR_52_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_52 */
#define MAR_52_PCX_BIT_START                                2U
#define MAR_52_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_52 */
#define MAR_52_WTE_BIT_START                                1U
#define MAR_52_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_52 */
#define MAR_52_PC_BIT_START                                 0U
#define MAR_52_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_53 */
#define MAR_53_RESV_1_BIT_START                             4U
#define MAR_53_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_53 */
#define MAR_53_PFX_BIT_START                                3U
#define MAR_53_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_53 */
#define MAR_53_PCX_BIT_START                                2U
#define MAR_53_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_53 */
#define MAR_53_WTE_BIT_START                                1U
#define MAR_53_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_53 */
#define MAR_53_PC_BIT_START                                 0U
#define MAR_53_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_54 */
#define MAR_54_RESV_1_BIT_START                             4U
#define MAR_54_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_54 */
#define MAR_54_PFX_BIT_START                                3U
#define MAR_54_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_54 */
#define MAR_54_PCX_BIT_START                                2U
#define MAR_54_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_54 */
#define MAR_54_WTE_BIT_START                                1U
#define MAR_54_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_54 */
#define MAR_54_PC_BIT_START                                 0U
#define MAR_54_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_55 */
#define MAR_55_RESV_1_BIT_START                             4U
#define MAR_55_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_55 */
#define MAR_55_PFX_BIT_START                                3U
#define MAR_55_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_55 */
#define MAR_55_PCX_BIT_START                                2U
#define MAR_55_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_55 */
#define MAR_55_WTE_BIT_START                                1U
#define MAR_55_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_55 */
#define MAR_55_PC_BIT_START                                 0U
#define MAR_55_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_56 */
#define MAR_56_RESV_1_BIT_START                             4U
#define MAR_56_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_56 */
#define MAR_56_PFX_BIT_START                                3U
#define MAR_56_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_56 */
#define MAR_56_PCX_BIT_START                                2U
#define MAR_56_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_56 */
#define MAR_56_WTE_BIT_START                                1U
#define MAR_56_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_56 */
#define MAR_56_PC_BIT_START                                 0U
#define MAR_56_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_57 */
#define MAR_57_RESV_1_BIT_START                             4U
#define MAR_57_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_57 */
#define MAR_57_PFX_BIT_START                                3U
#define MAR_57_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_57 */
#define MAR_57_PCX_BIT_START                                2U
#define MAR_57_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_57 */
#define MAR_57_WTE_BIT_START                                1U
#define MAR_57_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_57 */
#define MAR_57_PC_BIT_START                                 0U
#define MAR_57_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_58 */
#define MAR_58_RESV_1_BIT_START                             4U
#define MAR_58_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_58 */
#define MAR_58_PFX_BIT_START                                3U
#define MAR_58_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_58 */
#define MAR_58_PCX_BIT_START                                2U
#define MAR_58_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_58 */
#define MAR_58_WTE_BIT_START                                1U
#define MAR_58_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_58 */
#define MAR_58_PC_BIT_START                                 0U
#define MAR_58_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_59 */
#define MAR_59_RESV_1_BIT_START                             4U
#define MAR_59_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_59 */
#define MAR_59_PFX_BIT_START                                3U
#define MAR_59_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_59 */
#define MAR_59_PCX_BIT_START                                2U
#define MAR_59_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_59 */
#define MAR_59_WTE_BIT_START                                1U
#define MAR_59_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_59 */
#define MAR_59_PC_BIT_START                                 0U
#define MAR_59_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_60 */
#define MAR_60_RESV_1_BIT_START                             4U
#define MAR_60_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_60 */
#define MAR_60_PFX_BIT_START                                3U
#define MAR_60_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_60 */
#define MAR_60_PCX_BIT_START                                2U
#define MAR_60_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_60 */
#define MAR_60_WTE_BIT_START                                1U
#define MAR_60_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_60 */
#define MAR_60_PC_BIT_START                                 0U
#define MAR_60_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_61 */
#define MAR_61_RESV_1_BIT_START                             4U
#define MAR_61_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_61 */
#define MAR_61_PFX_BIT_START                                3U
#define MAR_61_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_61 */
#define MAR_61_PCX_BIT_START                                2U
#define MAR_61_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_61 */
#define MAR_61_WTE_BIT_START                                1U
#define MAR_61_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_61 */
#define MAR_61_PC_BIT_START                                 0U
#define MAR_61_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_62 */
#define MAR_62_RESV_1_BIT_START                             4U
#define MAR_62_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_62 */
#define MAR_62_PFX_BIT_START                                3U
#define MAR_62_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_62 */
#define MAR_62_PCX_BIT_START                                2U
#define MAR_62_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_62 */
#define MAR_62_WTE_BIT_START                                1U
#define MAR_62_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_62 */
#define MAR_62_PC_BIT_START                                 0U
#define MAR_62_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_63 */
#define MAR_63_RESV_1_BIT_START                             4U
#define MAR_63_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_63 */
#define MAR_63_PFX_BIT_START                                3U
#define MAR_63_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_63 */
#define MAR_63_PCX_BIT_START                                2U
#define MAR_63_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_63 */
#define MAR_63_WTE_BIT_START                                1U
#define MAR_63_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_63 */
#define MAR_63_PC_BIT_START                                 0U
#define MAR_63_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_64 */
#define MAR_64_RESV_1_BIT_START                             4U
#define MAR_64_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_64 */
#define MAR_64_PFX_BIT_START                                3U
#define MAR_64_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_64 */
#define MAR_64_PCX_BIT_START                                2U
#define MAR_64_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_64 */
#define MAR_64_WTE_BIT_START                                1U
#define MAR_64_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_64 */
#define MAR_64_PC_BIT_START                                 0U
#define MAR_64_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_65 */
#define MAR_65_RESV_1_BIT_START                             4U
#define MAR_65_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_65 */
#define MAR_65_PFX_BIT_START                                3U
#define MAR_65_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_65 */
#define MAR_65_PCX_BIT_START                                2U
#define MAR_65_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_65 */
#define MAR_65_WTE_BIT_START                                1U
#define MAR_65_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_65 */
#define MAR_65_PC_BIT_START                                 0U
#define MAR_65_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_66 */
#define MAR_66_RESV_1_BIT_START                             4U
#define MAR_66_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_66 */
#define MAR_66_PFX_BIT_START                                3U
#define MAR_66_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_66 */
#define MAR_66_PCX_BIT_START                                2U
#define MAR_66_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_66 */
#define MAR_66_WTE_BIT_START                                1U
#define MAR_66_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_66 */
#define MAR_66_PC_BIT_START                                 0U
#define MAR_66_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_67 */
#define MAR_67_RESV_1_BIT_START                             4U
#define MAR_67_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_67 */
#define MAR_67_PFX_BIT_START                                3U
#define MAR_67_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_67 */
#define MAR_67_PCX_BIT_START                                2U
#define MAR_67_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_67 */
#define MAR_67_WTE_BIT_START                                1U
#define MAR_67_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_67 */
#define MAR_67_PC_BIT_START                                 0U
#define MAR_67_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_68 */
#define MAR_68_RESV_1_BIT_START                             4U
#define MAR_68_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_68 */
#define MAR_68_PFX_BIT_START                                3U
#define MAR_68_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_68 */
#define MAR_68_PCX_BIT_START                                2U
#define MAR_68_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_68 */
#define MAR_68_WTE_BIT_START                                1U
#define MAR_68_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_68 */
#define MAR_68_PC_BIT_START                                 0U
#define MAR_68_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_69 */
#define MAR_69_RESV_1_BIT_START                             4U
#define MAR_69_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_69 */
#define MAR_69_PFX_BIT_START                                3U
#define MAR_69_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_69 */
#define MAR_69_PCX_BIT_START                                2U
#define MAR_69_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_69 */
#define MAR_69_WTE_BIT_START                                1U
#define MAR_69_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_69 */
#define MAR_69_PC_BIT_START                                 0U
#define MAR_69_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_70 */
#define MAR_70_RESV_1_BIT_START                             4U
#define MAR_70_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_70 */
#define MAR_70_PFX_BIT_START                                3U
#define MAR_70_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_70 */
#define MAR_70_PCX_BIT_START                                2U
#define MAR_70_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_70 */
#define MAR_70_WTE_BIT_START                                1U
#define MAR_70_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_70 */
#define MAR_70_PC_BIT_START                                 0U
#define MAR_70_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_71 */
#define MAR_71_RESV_1_BIT_START                             4U
#define MAR_71_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_71 */
#define MAR_71_PFX_BIT_START                                3U
#define MAR_71_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_71 */
#define MAR_71_PCX_BIT_START                                2U
#define MAR_71_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_71 */
#define MAR_71_WTE_BIT_START                                1U
#define MAR_71_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_71 */
#define MAR_71_PC_BIT_START                                 0U
#define MAR_71_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_72 */
#define MAR_72_RESV_1_BIT_START                             4U
#define MAR_72_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_72 */
#define MAR_72_PFX_BIT_START                                3U
#define MAR_72_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_72 */
#define MAR_72_PCX_BIT_START                                2U
#define MAR_72_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_72 */
#define MAR_72_WTE_BIT_START                                1U
#define MAR_72_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_72 */
#define MAR_72_PC_BIT_START                                 0U
#define MAR_72_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_73 */
#define MAR_73_RESV_1_BIT_START                             4U
#define MAR_73_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_73 */
#define MAR_73_PFX_BIT_START                                3U
#define MAR_73_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_73 */
#define MAR_73_PCX_BIT_START                                2U
#define MAR_73_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_73 */
#define MAR_73_WTE_BIT_START                                1U
#define MAR_73_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_73 */
#define MAR_73_PC_BIT_START                                 0U
#define MAR_73_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_74 */
#define MAR_74_RESV_1_BIT_START                             4U
#define MAR_74_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_74 */
#define MAR_74_PFX_BIT_START                                3U
#define MAR_74_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_74 */
#define MAR_74_PCX_BIT_START                                2U
#define MAR_74_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_74 */
#define MAR_74_WTE_BIT_START                                1U
#define MAR_74_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_74 */
#define MAR_74_PC_BIT_START                                 0U
#define MAR_74_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_75 */
#define MAR_75_RESV_1_BIT_START                             4U
#define MAR_75_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_75 */
#define MAR_75_PFX_BIT_START                                3U
#define MAR_75_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_75 */
#define MAR_75_PCX_BIT_START                                2U
#define MAR_75_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_75 */
#define MAR_75_WTE_BIT_START                                1U
#define MAR_75_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_75 */
#define MAR_75_PC_BIT_START                                 0U
#define MAR_75_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_76 */
#define MAR_76_RESV_1_BIT_START                             4U
#define MAR_76_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_76 */
#define MAR_76_PFX_BIT_START                                3U
#define MAR_76_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_76 */
#define MAR_76_PCX_BIT_START                                2U
#define MAR_76_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_76 */
#define MAR_76_WTE_BIT_START                                1U
#define MAR_76_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_76 */
#define MAR_76_PC_BIT_START                                 0U
#define MAR_76_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_77 */
#define MAR_77_RESV_1_BIT_START                             4U
#define MAR_77_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_77 */
#define MAR_77_PFX_BIT_START                                3U
#define MAR_77_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_77 */
#define MAR_77_PCX_BIT_START                                2U
#define MAR_77_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_77 */
#define MAR_77_WTE_BIT_START                                1U
#define MAR_77_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_77 */
#define MAR_77_PC_BIT_START                                 0U
#define MAR_77_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_78 */
#define MAR_78_RESV_1_BIT_START                             4U
#define MAR_78_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_78 */
#define MAR_78_PFX_BIT_START                                3U
#define MAR_78_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_78 */
#define MAR_78_PCX_BIT_START                                2U
#define MAR_78_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_78 */
#define MAR_78_WTE_BIT_START                                1U
#define MAR_78_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_78 */
#define MAR_78_PC_BIT_START                                 0U
#define MAR_78_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_79 */
#define MAR_79_RESV_1_BIT_START                             4U
#define MAR_79_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_79 */
#define MAR_79_PFX_BIT_START                                3U
#define MAR_79_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_79 */
#define MAR_79_PCX_BIT_START                                2U
#define MAR_79_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_79 */
#define MAR_79_WTE_BIT_START                                1U
#define MAR_79_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_79 */
#define MAR_79_PC_BIT_START                                 0U
#define MAR_79_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_80 */
#define MAR_80_RESV_1_BIT_START                             4U
#define MAR_80_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_80 */
#define MAR_80_PFX_BIT_START                                3U
#define MAR_80_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_80 */
#define MAR_80_PCX_BIT_START                                2U
#define MAR_80_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_80 */
#define MAR_80_WTE_BIT_START                                1U
#define MAR_80_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_80 */
#define MAR_80_PC_BIT_START                                 0U
#define MAR_80_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_81 */
#define MAR_81_RESV_1_BIT_START                             4U
#define MAR_81_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_81 */
#define MAR_81_PFX_BIT_START                                3U
#define MAR_81_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_81 */
#define MAR_81_PCX_BIT_START                                2U
#define MAR_81_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_81 */
#define MAR_81_WTE_BIT_START                                1U
#define MAR_81_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_81 */
#define MAR_81_PC_BIT_START                                 0U
#define MAR_81_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_82 */
#define MAR_82_RESV_1_BIT_START                             4U
#define MAR_82_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_82 */
#define MAR_82_PFX_BIT_START                                3U
#define MAR_82_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_82 */
#define MAR_82_PCX_BIT_START                                2U
#define MAR_82_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_82 */
#define MAR_82_WTE_BIT_START                                1U
#define MAR_82_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_82 */
#define MAR_82_PC_BIT_START                                 0U
#define MAR_82_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_83 */
#define MAR_83_RESV_1_BIT_START                             4U
#define MAR_83_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_83 */
#define MAR_83_PFX_BIT_START                                3U
#define MAR_83_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_83 */
#define MAR_83_PCX_BIT_START                                2U
#define MAR_83_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_83 */
#define MAR_83_WTE_BIT_START                                1U
#define MAR_83_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_83 */
#define MAR_83_PC_BIT_START                                 0U
#define MAR_83_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_84 */
#define MAR_84_RESV_1_BIT_START                             4U
#define MAR_84_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_84 */
#define MAR_84_PFX_BIT_START                                3U
#define MAR_84_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_84 */
#define MAR_84_PCX_BIT_START                                2U
#define MAR_84_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_84 */
#define MAR_84_WTE_BIT_START                                1U
#define MAR_84_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_84 */
#define MAR_84_PC_BIT_START                                 0U
#define MAR_84_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_85 */
#define MAR_85_RESV_1_BIT_START                             4U
#define MAR_85_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_85 */
#define MAR_85_PFX_BIT_START                                3U
#define MAR_85_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_85 */
#define MAR_85_PCX_BIT_START                                2U
#define MAR_85_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_85 */
#define MAR_85_WTE_BIT_START                                1U
#define MAR_85_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_85 */
#define MAR_85_PC_BIT_START                                 0U
#define MAR_85_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_86 */
#define MAR_86_RESV_1_BIT_START                             4U
#define MAR_86_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_86 */
#define MAR_86_PFX_BIT_START                                3U
#define MAR_86_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_86 */
#define MAR_86_PCX_BIT_START                                2U
#define MAR_86_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_86 */
#define MAR_86_WTE_BIT_START                                1U
#define MAR_86_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_86 */
#define MAR_86_PC_BIT_START                                 0U
#define MAR_86_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_87 */
#define MAR_87_RESV_1_BIT_START                             4U
#define MAR_87_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_87 */
#define MAR_87_PFX_BIT_START                                3U
#define MAR_87_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_87 */
#define MAR_87_PCX_BIT_START                                2U
#define MAR_87_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_87 */
#define MAR_87_WTE_BIT_START                                1U
#define MAR_87_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_87 */
#define MAR_87_PC_BIT_START                                 0U
#define MAR_87_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_88 */
#define MAR_88_RESV_1_BIT_START                             4U
#define MAR_88_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_88 */
#define MAR_88_PFX_BIT_START                                3U
#define MAR_88_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_88 */
#define MAR_88_PCX_BIT_START                                2U
#define MAR_88_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_88 */
#define MAR_88_WTE_BIT_START                                1U
#define MAR_88_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_88 */
#define MAR_88_PC_BIT_START                                 0U
#define MAR_88_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_89 */
#define MAR_89_RESV_1_BIT_START                             4U
#define MAR_89_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_89 */
#define MAR_89_PFX_BIT_START                                3U
#define MAR_89_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_89 */
#define MAR_89_PCX_BIT_START                                2U
#define MAR_89_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_89 */
#define MAR_89_WTE_BIT_START                                1U
#define MAR_89_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_89 */
#define MAR_89_PC_BIT_START                                 0U
#define MAR_89_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_90 */
#define MAR_90_RESV_1_BIT_START                             4U
#define MAR_90_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_90 */
#define MAR_90_PFX_BIT_START                                3U
#define MAR_90_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_90 */
#define MAR_90_PCX_BIT_START                                2U
#define MAR_90_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_90 */
#define MAR_90_WTE_BIT_START                                1U
#define MAR_90_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_90 */
#define MAR_90_PC_BIT_START                                 0U
#define MAR_90_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_91 */
#define MAR_91_RESV_1_BIT_START                             4U
#define MAR_91_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_91 */
#define MAR_91_PFX_BIT_START                                3U
#define MAR_91_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_91 */
#define MAR_91_PCX_BIT_START                                2U
#define MAR_91_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_91 */
#define MAR_91_WTE_BIT_START                                1U
#define MAR_91_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_91 */
#define MAR_91_PC_BIT_START                                 0U
#define MAR_91_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_92 */
#define MAR_92_RESV_1_BIT_START                             4U
#define MAR_92_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_92 */
#define MAR_92_PFX_BIT_START                                3U
#define MAR_92_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_92 */
#define MAR_92_PCX_BIT_START                                2U
#define MAR_92_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_92 */
#define MAR_92_WTE_BIT_START                                1U
#define MAR_92_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_92 */
#define MAR_92_PC_BIT_START                                 0U
#define MAR_92_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_93 */
#define MAR_93_RESV_1_BIT_START                             4U
#define MAR_93_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_93 */
#define MAR_93_PFX_BIT_START                                3U
#define MAR_93_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_93 */
#define MAR_93_PCX_BIT_START                                2U
#define MAR_93_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_93 */
#define MAR_93_WTE_BIT_START                                1U
#define MAR_93_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_93 */
#define MAR_93_PC_BIT_START                                 0U
#define MAR_93_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_94 */
#define MAR_94_RESV_1_BIT_START                             4U
#define MAR_94_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_94 */
#define MAR_94_PFX_BIT_START                                3U
#define MAR_94_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_94 */
#define MAR_94_PCX_BIT_START                                2U
#define MAR_94_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_94 */
#define MAR_94_WTE_BIT_START                                1U
#define MAR_94_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_94 */
#define MAR_94_PC_BIT_START                                 0U
#define MAR_94_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_95 */
#define MAR_95_RESV_1_BIT_START                             4U
#define MAR_95_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_95 */
#define MAR_95_PFX_BIT_START                                3U
#define MAR_95_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_95 */
#define MAR_95_PCX_BIT_START                                2U
#define MAR_95_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_95 */
#define MAR_95_WTE_BIT_START                                1U
#define MAR_95_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_95 */
#define MAR_95_PC_BIT_START                                 0U
#define MAR_95_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_96 */
#define MAR_96_RESV_1_BIT_START                             4U
#define MAR_96_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_96 */
#define MAR_96_PFX_BIT_START                                3U
#define MAR_96_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_96 */
#define MAR_96_PCX_BIT_START                                2U
#define MAR_96_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_96 */
#define MAR_96_WTE_BIT_START                                1U
#define MAR_96_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_96 */
#define MAR_96_PC_BIT_START                                 0U
#define MAR_96_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_97 */
#define MAR_97_RESV_1_BIT_START                             4U
#define MAR_97_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_97 */
#define MAR_97_PFX_BIT_START                                3U
#define MAR_97_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_97 */
#define MAR_97_PCX_BIT_START                                2U
#define MAR_97_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_97 */
#define MAR_97_WTE_BIT_START                                1U
#define MAR_97_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_97 */
#define MAR_97_PC_BIT_START                                 0U
#define MAR_97_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_98 */
#define MAR_98_RESV_1_BIT_START                             4U
#define MAR_98_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_98 */
#define MAR_98_PFX_BIT_START                                3U
#define MAR_98_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_98 */
#define MAR_98_PCX_BIT_START                                2U
#define MAR_98_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_98 */
#define MAR_98_WTE_BIT_START                                1U
#define MAR_98_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_98 */
#define MAR_98_PC_BIT_START                                 0U
#define MAR_98_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_99 */
#define MAR_99_RESV_1_BIT_START                             4U
#define MAR_99_RESV_1_BIT_END                               31U

/* Definition for field PFX in Register MAR_99 */
#define MAR_99_PFX_BIT_START                                3U
#define MAR_99_PFX_BIT_END                                  3U

/* Definition for field PCX in Register MAR_99 */
#define MAR_99_PCX_BIT_START                                2U
#define MAR_99_PCX_BIT_END                                  2U

/* Definition for field WTE in Register MAR_99 */
#define MAR_99_WTE_BIT_START                                1U
#define MAR_99_WTE_BIT_END                                  1U

/* Definition for field PC in Register MAR_99 */
#define MAR_99_PC_BIT_START                                 0U
#define MAR_99_PC_BIT_END                                   0U

/* Definition for field RESV_1 in Register MAR_100 */
#define MAR_100_RESV_1_BIT_START                            4U
#define MAR_100_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_100 */
#define MAR_100_PFX_BIT_START                               3U
#define MAR_100_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_100 */
#define MAR_100_PCX_BIT_START                               2U
#define MAR_100_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_100 */
#define MAR_100_WTE_BIT_START                               1U
#define MAR_100_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_100 */
#define MAR_100_PC_BIT_START                                0U
#define MAR_100_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_101 */
#define MAR_101_RESV_1_BIT_START                            4U
#define MAR_101_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_101 */
#define MAR_101_PFX_BIT_START                               3U
#define MAR_101_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_101 */
#define MAR_101_PCX_BIT_START                               2U
#define MAR_101_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_101 */
#define MAR_101_WTE_BIT_START                               1U
#define MAR_101_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_101 */
#define MAR_101_PC_BIT_START                                0U
#define MAR_101_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_102 */
#define MAR_102_RESV_1_BIT_START                            4U
#define MAR_102_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_102 */
#define MAR_102_PFX_BIT_START                               3U
#define MAR_102_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_102 */
#define MAR_102_PCX_BIT_START                               2U
#define MAR_102_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_102 */
#define MAR_102_WTE_BIT_START                               1U
#define MAR_102_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_102 */
#define MAR_102_PC_BIT_START                                0U
#define MAR_102_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_103 */
#define MAR_103_RESV_1_BIT_START                            4U
#define MAR_103_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_103 */
#define MAR_103_PFX_BIT_START                               3U
#define MAR_103_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_103 */
#define MAR_103_PCX_BIT_START                               2U
#define MAR_103_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_103 */
#define MAR_103_WTE_BIT_START                               1U
#define MAR_103_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_103 */
#define MAR_103_PC_BIT_START                                0U
#define MAR_103_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_104 */
#define MAR_104_RESV_1_BIT_START                            4U
#define MAR_104_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_104 */
#define MAR_104_PFX_BIT_START                               3U
#define MAR_104_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_104 */
#define MAR_104_PCX_BIT_START                               2U
#define MAR_104_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_104 */
#define MAR_104_WTE_BIT_START                               1U
#define MAR_104_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_104 */
#define MAR_104_PC_BIT_START                                0U
#define MAR_104_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_105 */
#define MAR_105_RESV_1_BIT_START                            4U
#define MAR_105_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_105 */
#define MAR_105_PFX_BIT_START                               3U
#define MAR_105_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_105 */
#define MAR_105_PCX_BIT_START                               2U
#define MAR_105_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_105 */
#define MAR_105_WTE_BIT_START                               1U
#define MAR_105_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_105 */
#define MAR_105_PC_BIT_START                                0U
#define MAR_105_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_106 */
#define MAR_106_RESV_1_BIT_START                            4U
#define MAR_106_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_106 */
#define MAR_106_PFX_BIT_START                               3U
#define MAR_106_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_106 */
#define MAR_106_PCX_BIT_START                               2U
#define MAR_106_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_106 */
#define MAR_106_WTE_BIT_START                               1U
#define MAR_106_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_106 */
#define MAR_106_PC_BIT_START                                0U
#define MAR_106_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_107 */
#define MAR_107_RESV_1_BIT_START                            4U
#define MAR_107_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_107 */
#define MAR_107_PFX_BIT_START                               3U
#define MAR_107_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_107 */
#define MAR_107_PCX_BIT_START                               2U
#define MAR_107_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_107 */
#define MAR_107_WTE_BIT_START                               1U
#define MAR_107_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_107 */
#define MAR_107_PC_BIT_START                                0U
#define MAR_107_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_108 */
#define MAR_108_RESV_1_BIT_START                            4U
#define MAR_108_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_108 */
#define MAR_108_PFX_BIT_START                               3U
#define MAR_108_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_108 */
#define MAR_108_PCX_BIT_START                               2U
#define MAR_108_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_108 */
#define MAR_108_WTE_BIT_START                               1U
#define MAR_108_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_108 */
#define MAR_108_PC_BIT_START                                0U
#define MAR_108_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_109 */
#define MAR_109_RESV_1_BIT_START                            4U
#define MAR_109_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_109 */
#define MAR_109_PFX_BIT_START                               3U
#define MAR_109_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_109 */
#define MAR_109_PCX_BIT_START                               2U
#define MAR_109_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_109 */
#define MAR_109_WTE_BIT_START                               1U
#define MAR_109_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_109 */
#define MAR_109_PC_BIT_START                                0U
#define MAR_109_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_110 */
#define MAR_110_RESV_1_BIT_START                            4U
#define MAR_110_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_110 */
#define MAR_110_PFX_BIT_START                               3U
#define MAR_110_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_110 */
#define MAR_110_PCX_BIT_START                               2U
#define MAR_110_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_110 */
#define MAR_110_WTE_BIT_START                               1U
#define MAR_110_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_110 */
#define MAR_110_PC_BIT_START                                0U
#define MAR_110_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_111 */
#define MAR_111_RESV_1_BIT_START                            4U
#define MAR_111_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_111 */
#define MAR_111_PFX_BIT_START                               3U
#define MAR_111_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_111 */
#define MAR_111_PCX_BIT_START                               2U
#define MAR_111_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_111 */
#define MAR_111_WTE_BIT_START                               1U
#define MAR_111_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_111 */
#define MAR_111_PC_BIT_START                                0U
#define MAR_111_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_112 */
#define MAR_112_RESV_1_BIT_START                            4U
#define MAR_112_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_112 */
#define MAR_112_PFX_BIT_START                               3U
#define MAR_112_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_112 */
#define MAR_112_PCX_BIT_START                               2U
#define MAR_112_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_112 */
#define MAR_112_WTE_BIT_START                               1U
#define MAR_112_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_112 */
#define MAR_112_PC_BIT_START                                0U
#define MAR_112_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_113 */
#define MAR_113_RESV_1_BIT_START                            4U
#define MAR_113_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_113 */
#define MAR_113_PFX_BIT_START                               3U
#define MAR_113_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_113 */
#define MAR_113_PCX_BIT_START                               2U
#define MAR_113_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_113 */
#define MAR_113_WTE_BIT_START                               1U
#define MAR_113_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_113 */
#define MAR_113_PC_BIT_START                                0U
#define MAR_113_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_114 */
#define MAR_114_RESV_1_BIT_START                            4U
#define MAR_114_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_114 */
#define MAR_114_PFX_BIT_START                               3U
#define MAR_114_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_114 */
#define MAR_114_PCX_BIT_START                               2U
#define MAR_114_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_114 */
#define MAR_114_WTE_BIT_START                               1U
#define MAR_114_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_114 */
#define MAR_114_PC_BIT_START                                0U
#define MAR_114_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_115 */
#define MAR_115_RESV_1_BIT_START                            4U
#define MAR_115_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_115 */
#define MAR_115_PFX_BIT_START                               3U
#define MAR_115_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_115 */
#define MAR_115_PCX_BIT_START                               2U
#define MAR_115_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_115 */
#define MAR_115_WTE_BIT_START                               1U
#define MAR_115_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_115 */
#define MAR_115_PC_BIT_START                                0U
#define MAR_115_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_116 */
#define MAR_116_RESV_1_BIT_START                            4U
#define MAR_116_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_116 */
#define MAR_116_PFX_BIT_START                               3U
#define MAR_116_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_116 */
#define MAR_116_PCX_BIT_START                               2U
#define MAR_116_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_116 */
#define MAR_116_WTE_BIT_START                               1U
#define MAR_116_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_116 */
#define MAR_116_PC_BIT_START                                0U
#define MAR_116_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_117 */
#define MAR_117_RESV_1_BIT_START                            4U
#define MAR_117_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_117 */
#define MAR_117_PFX_BIT_START                               3U
#define MAR_117_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_117 */
#define MAR_117_PCX_BIT_START                               2U
#define MAR_117_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_117 */
#define MAR_117_WTE_BIT_START                               1U
#define MAR_117_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_117 */
#define MAR_117_PC_BIT_START                                0U
#define MAR_117_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_118 */
#define MAR_118_RESV_1_BIT_START                            4U
#define MAR_118_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_118 */
#define MAR_118_PFX_BIT_START                               3U
#define MAR_118_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_118 */
#define MAR_118_PCX_BIT_START                               2U
#define MAR_118_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_118 */
#define MAR_118_WTE_BIT_START                               1U
#define MAR_118_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_118 */
#define MAR_118_PC_BIT_START                                0U
#define MAR_118_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_119 */
#define MAR_119_RESV_1_BIT_START                            4U
#define MAR_119_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_119 */
#define MAR_119_PFX_BIT_START                               3U
#define MAR_119_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_119 */
#define MAR_119_PCX_BIT_START                               2U
#define MAR_119_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_119 */
#define MAR_119_WTE_BIT_START                               1U
#define MAR_119_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_119 */
#define MAR_119_PC_BIT_START                                0U
#define MAR_119_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_120 */
#define MAR_120_RESV_1_BIT_START                            4U
#define MAR_120_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_120 */
#define MAR_120_PFX_BIT_START                               3U
#define MAR_120_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_120 */
#define MAR_120_PCX_BIT_START                               2U
#define MAR_120_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_120 */
#define MAR_120_WTE_BIT_START                               1U
#define MAR_120_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_120 */
#define MAR_120_PC_BIT_START                                0U
#define MAR_120_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_121 */
#define MAR_121_RESV_1_BIT_START                            4U
#define MAR_121_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_121 */
#define MAR_121_PFX_BIT_START                               3U
#define MAR_121_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_121 */
#define MAR_121_PCX_BIT_START                               2U
#define MAR_121_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_121 */
#define MAR_121_WTE_BIT_START                               1U
#define MAR_121_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_121 */
#define MAR_121_PC_BIT_START                                0U
#define MAR_121_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_122 */
#define MAR_122_RESV_1_BIT_START                            4U
#define MAR_122_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_122 */
#define MAR_122_PFX_BIT_START                               3U
#define MAR_122_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_122 */
#define MAR_122_PCX_BIT_START                               2U
#define MAR_122_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_122 */
#define MAR_122_WTE_BIT_START                               1U
#define MAR_122_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_122 */
#define MAR_122_PC_BIT_START                                0U
#define MAR_122_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_123 */
#define MAR_123_RESV_1_BIT_START                            4U
#define MAR_123_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_123 */
#define MAR_123_PFX_BIT_START                               3U
#define MAR_123_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_123 */
#define MAR_123_PCX_BIT_START                               2U
#define MAR_123_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_123 */
#define MAR_123_WTE_BIT_START                               1U
#define MAR_123_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_123 */
#define MAR_123_PC_BIT_START                                0U
#define MAR_123_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_124 */
#define MAR_124_RESV_1_BIT_START                            4U
#define MAR_124_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_124 */
#define MAR_124_PFX_BIT_START                               3U
#define MAR_124_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_124 */
#define MAR_124_PCX_BIT_START                               2U
#define MAR_124_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_124 */
#define MAR_124_WTE_BIT_START                               1U
#define MAR_124_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_124 */
#define MAR_124_PC_BIT_START                                0U
#define MAR_124_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_125 */
#define MAR_125_RESV_1_BIT_START                            4U
#define MAR_125_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_125 */
#define MAR_125_PFX_BIT_START                               3U
#define MAR_125_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_125 */
#define MAR_125_PCX_BIT_START                               2U
#define MAR_125_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_125 */
#define MAR_125_WTE_BIT_START                               1U
#define MAR_125_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_125 */
#define MAR_125_PC_BIT_START                                0U
#define MAR_125_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_126 */
#define MAR_126_RESV_1_BIT_START                            4U
#define MAR_126_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_126 */
#define MAR_126_PFX_BIT_START                               3U
#define MAR_126_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_126 */
#define MAR_126_PCX_BIT_START                               2U
#define MAR_126_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_126 */
#define MAR_126_WTE_BIT_START                               1U
#define MAR_126_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_126 */
#define MAR_126_PC_BIT_START                                0U
#define MAR_126_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_127 */
#define MAR_127_RESV_1_BIT_START                            4U
#define MAR_127_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_127 */
#define MAR_127_PFX_BIT_START                               3U
#define MAR_127_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_127 */
#define MAR_127_PCX_BIT_START                               2U
#define MAR_127_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_127 */
#define MAR_127_WTE_BIT_START                               1U
#define MAR_127_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_127 */
#define MAR_127_PC_BIT_START                                0U
#define MAR_127_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_128 */
#define MAR_128_RESV_1_BIT_START                            4U
#define MAR_128_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_128 */
#define MAR_128_PFX_BIT_START                               3U
#define MAR_128_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_128 */
#define MAR_128_PCX_BIT_START                               2U
#define MAR_128_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_128 */
#define MAR_128_WTE_BIT_START                               1U
#define MAR_128_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_128 */
#define MAR_128_PC_BIT_START                                0U
#define MAR_128_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_129 */
#define MAR_129_RESV_1_BIT_START                            4U
#define MAR_129_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_129 */
#define MAR_129_PFX_BIT_START                               3U
#define MAR_129_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_129 */
#define MAR_129_PCX_BIT_START                               2U
#define MAR_129_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_129 */
#define MAR_129_WTE_BIT_START                               1U
#define MAR_129_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_129 */
#define MAR_129_PC_BIT_START                                0U
#define MAR_129_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_130 */
#define MAR_130_RESV_1_BIT_START                            4U
#define MAR_130_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_130 */
#define MAR_130_PFX_BIT_START                               3U
#define MAR_130_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_130 */
#define MAR_130_PCX_BIT_START                               2U
#define MAR_130_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_130 */
#define MAR_130_WTE_BIT_START                               1U
#define MAR_130_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_130 */
#define MAR_130_PC_BIT_START                                0U
#define MAR_130_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_131 */
#define MAR_131_RESV_1_BIT_START                            4U
#define MAR_131_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_131 */
#define MAR_131_PFX_BIT_START                               3U
#define MAR_131_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_131 */
#define MAR_131_PCX_BIT_START                               2U
#define MAR_131_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_131 */
#define MAR_131_WTE_BIT_START                               1U
#define MAR_131_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_131 */
#define MAR_131_PC_BIT_START                                0U
#define MAR_131_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_132 */
#define MAR_132_RESV_1_BIT_START                            4U
#define MAR_132_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_132 */
#define MAR_132_PFX_BIT_START                               3U
#define MAR_132_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_132 */
#define MAR_132_PCX_BIT_START                               2U
#define MAR_132_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_132 */
#define MAR_132_WTE_BIT_START                               1U
#define MAR_132_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_132 */
#define MAR_132_PC_BIT_START                                0U
#define MAR_132_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_133 */
#define MAR_133_RESV_1_BIT_START                            4U
#define MAR_133_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_133 */
#define MAR_133_PFX_BIT_START                               3U
#define MAR_133_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_133 */
#define MAR_133_PCX_BIT_START                               2U
#define MAR_133_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_133 */
#define MAR_133_WTE_BIT_START                               1U
#define MAR_133_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_133 */
#define MAR_133_PC_BIT_START                                0U
#define MAR_133_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_134 */
#define MAR_134_RESV_1_BIT_START                            4U
#define MAR_134_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_134 */
#define MAR_134_PFX_BIT_START                               3U
#define MAR_134_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_134 */
#define MAR_134_PCX_BIT_START                               2U
#define MAR_134_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_134 */
#define MAR_134_WTE_BIT_START                               1U
#define MAR_134_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_134 */
#define MAR_134_PC_BIT_START                                0U
#define MAR_134_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_135 */
#define MAR_135_RESV_1_BIT_START                            4U
#define MAR_135_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_135 */
#define MAR_135_PFX_BIT_START                               3U
#define MAR_135_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_135 */
#define MAR_135_PCX_BIT_START                               2U
#define MAR_135_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_135 */
#define MAR_135_WTE_BIT_START                               1U
#define MAR_135_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_135 */
#define MAR_135_PC_BIT_START                                0U
#define MAR_135_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_136 */
#define MAR_136_RESV_1_BIT_START                            4U
#define MAR_136_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_136 */
#define MAR_136_PFX_BIT_START                               3U
#define MAR_136_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_136 */
#define MAR_136_PCX_BIT_START                               2U
#define MAR_136_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_136 */
#define MAR_136_WTE_BIT_START                               1U
#define MAR_136_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_136 */
#define MAR_136_PC_BIT_START                                0U
#define MAR_136_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_137 */
#define MAR_137_RESV_1_BIT_START                            4U
#define MAR_137_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_137 */
#define MAR_137_PFX_BIT_START                               3U
#define MAR_137_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_137 */
#define MAR_137_PCX_BIT_START                               2U
#define MAR_137_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_137 */
#define MAR_137_WTE_BIT_START                               1U
#define MAR_137_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_137 */
#define MAR_137_PC_BIT_START                                0U
#define MAR_137_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_138 */
#define MAR_138_RESV_1_BIT_START                            4U
#define MAR_138_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_138 */
#define MAR_138_PFX_BIT_START                               3U
#define MAR_138_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_138 */
#define MAR_138_PCX_BIT_START                               2U
#define MAR_138_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_138 */
#define MAR_138_WTE_BIT_START                               1U
#define MAR_138_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_138 */
#define MAR_138_PC_BIT_START                                0U
#define MAR_138_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_139 */
#define MAR_139_RESV_1_BIT_START                            4U
#define MAR_139_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_139 */
#define MAR_139_PFX_BIT_START                               3U
#define MAR_139_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_139 */
#define MAR_139_PCX_BIT_START                               2U
#define MAR_139_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_139 */
#define MAR_139_WTE_BIT_START                               1U
#define MAR_139_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_139 */
#define MAR_139_PC_BIT_START                                0U
#define MAR_139_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_140 */
#define MAR_140_RESV_1_BIT_START                            4U
#define MAR_140_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_140 */
#define MAR_140_PFX_BIT_START                               3U
#define MAR_140_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_140 */
#define MAR_140_PCX_BIT_START                               2U
#define MAR_140_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_140 */
#define MAR_140_WTE_BIT_START                               1U
#define MAR_140_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_140 */
#define MAR_140_PC_BIT_START                                0U
#define MAR_140_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_141 */
#define MAR_141_RESV_1_BIT_START                            4U
#define MAR_141_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_141 */
#define MAR_141_PFX_BIT_START                               3U
#define MAR_141_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_141 */
#define MAR_141_PCX_BIT_START                               2U
#define MAR_141_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_141 */
#define MAR_141_WTE_BIT_START                               1U
#define MAR_141_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_141 */
#define MAR_141_PC_BIT_START                                0U
#define MAR_141_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_142 */
#define MAR_142_RESV_1_BIT_START                            4U
#define MAR_142_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_142 */
#define MAR_142_PFX_BIT_START                               3U
#define MAR_142_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_142 */
#define MAR_142_PCX_BIT_START                               2U
#define MAR_142_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_142 */
#define MAR_142_WTE_BIT_START                               1U
#define MAR_142_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_142 */
#define MAR_142_PC_BIT_START                                0U
#define MAR_142_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_143 */
#define MAR_143_RESV_1_BIT_START                            4U
#define MAR_143_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_143 */
#define MAR_143_PFX_BIT_START                               3U
#define MAR_143_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_143 */
#define MAR_143_PCX_BIT_START                               2U
#define MAR_143_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_143 */
#define MAR_143_WTE_BIT_START                               1U
#define MAR_143_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_143 */
#define MAR_143_PC_BIT_START                                0U
#define MAR_143_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_144 */
#define MAR_144_RESV_1_BIT_START                            4U
#define MAR_144_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_144 */
#define MAR_144_PFX_BIT_START                               3U
#define MAR_144_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_144 */
#define MAR_144_PCX_BIT_START                               2U
#define MAR_144_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_144 */
#define MAR_144_WTE_BIT_START                               1U
#define MAR_144_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_144 */
#define MAR_144_PC_BIT_START                                0U
#define MAR_144_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_145 */
#define MAR_145_RESV_1_BIT_START                            4U
#define MAR_145_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_145 */
#define MAR_145_PFX_BIT_START                               3U
#define MAR_145_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_145 */
#define MAR_145_PCX_BIT_START                               2U
#define MAR_145_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_145 */
#define MAR_145_WTE_BIT_START                               1U
#define MAR_145_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_145 */
#define MAR_145_PC_BIT_START                                0U
#define MAR_145_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_146 */
#define MAR_146_RESV_1_BIT_START                            4U
#define MAR_146_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_146 */
#define MAR_146_PFX_BIT_START                               3U
#define MAR_146_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_146 */
#define MAR_146_PCX_BIT_START                               2U
#define MAR_146_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_146 */
#define MAR_146_WTE_BIT_START                               1U
#define MAR_146_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_146 */
#define MAR_146_PC_BIT_START                                0U
#define MAR_146_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_147 */
#define MAR_147_RESV_1_BIT_START                            4U
#define MAR_147_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_147 */
#define MAR_147_PFX_BIT_START                               3U
#define MAR_147_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_147 */
#define MAR_147_PCX_BIT_START                               2U
#define MAR_147_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_147 */
#define MAR_147_WTE_BIT_START                               1U
#define MAR_147_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_147 */
#define MAR_147_PC_BIT_START                                0U
#define MAR_147_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_148 */
#define MAR_148_RESV_1_BIT_START                            4U
#define MAR_148_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_148 */
#define MAR_148_PFX_BIT_START                               3U
#define MAR_148_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_148 */
#define MAR_148_PCX_BIT_START                               2U
#define MAR_148_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_148 */
#define MAR_148_WTE_BIT_START                               1U
#define MAR_148_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_148 */
#define MAR_148_PC_BIT_START                                0U
#define MAR_148_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_149 */
#define MAR_149_RESV_1_BIT_START                            4U
#define MAR_149_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_149 */
#define MAR_149_PFX_BIT_START                               3U
#define MAR_149_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_149 */
#define MAR_149_PCX_BIT_START                               2U
#define MAR_149_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_149 */
#define MAR_149_WTE_BIT_START                               1U
#define MAR_149_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_149 */
#define MAR_149_PC_BIT_START                                0U
#define MAR_149_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_150 */
#define MAR_150_RESV_1_BIT_START                            4U
#define MAR_150_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_150 */
#define MAR_150_PFX_BIT_START                               3U
#define MAR_150_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_150 */
#define MAR_150_PCX_BIT_START                               2U
#define MAR_150_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_150 */
#define MAR_150_WTE_BIT_START                               1U
#define MAR_150_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_150 */
#define MAR_150_PC_BIT_START                                0U
#define MAR_150_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_151 */
#define MAR_151_RESV_1_BIT_START                            4U
#define MAR_151_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_151 */
#define MAR_151_PFX_BIT_START                               3U
#define MAR_151_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_151 */
#define MAR_151_PCX_BIT_START                               2U
#define MAR_151_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_151 */
#define MAR_151_WTE_BIT_START                               1U
#define MAR_151_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_151 */
#define MAR_151_PC_BIT_START                                0U
#define MAR_151_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_152 */
#define MAR_152_RESV_1_BIT_START                            4U
#define MAR_152_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_152 */
#define MAR_152_PFX_BIT_START                               3U
#define MAR_152_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_152 */
#define MAR_152_PCX_BIT_START                               2U
#define MAR_152_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_152 */
#define MAR_152_WTE_BIT_START                               1U
#define MAR_152_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_152 */
#define MAR_152_PC_BIT_START                                0U
#define MAR_152_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_153 */
#define MAR_153_RESV_1_BIT_START                            4U
#define MAR_153_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_153 */
#define MAR_153_PFX_BIT_START                               3U
#define MAR_153_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_153 */
#define MAR_153_PCX_BIT_START                               2U
#define MAR_153_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_153 */
#define MAR_153_WTE_BIT_START                               1U
#define MAR_153_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_153 */
#define MAR_153_PC_BIT_START                                0U
#define MAR_153_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_154 */
#define MAR_154_RESV_1_BIT_START                            4U
#define MAR_154_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_154 */
#define MAR_154_PFX_BIT_START                               3U
#define MAR_154_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_154 */
#define MAR_154_PCX_BIT_START                               2U
#define MAR_154_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_154 */
#define MAR_154_WTE_BIT_START                               1U
#define MAR_154_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_154 */
#define MAR_154_PC_BIT_START                                0U
#define MAR_154_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_155 */
#define MAR_155_RESV_1_BIT_START                            4U
#define MAR_155_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_155 */
#define MAR_155_PFX_BIT_START                               3U
#define MAR_155_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_155 */
#define MAR_155_PCX_BIT_START                               2U
#define MAR_155_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_155 */
#define MAR_155_WTE_BIT_START                               1U
#define MAR_155_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_155 */
#define MAR_155_PC_BIT_START                                0U
#define MAR_155_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_156 */
#define MAR_156_RESV_1_BIT_START                            4U
#define MAR_156_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_156 */
#define MAR_156_PFX_BIT_START                               3U
#define MAR_156_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_156 */
#define MAR_156_PCX_BIT_START                               2U
#define MAR_156_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_156 */
#define MAR_156_WTE_BIT_START                               1U
#define MAR_156_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_156 */
#define MAR_156_PC_BIT_START                                0U
#define MAR_156_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_157 */
#define MAR_157_RESV_1_BIT_START                            4U
#define MAR_157_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_157 */
#define MAR_157_PFX_BIT_START                               3U
#define MAR_157_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_157 */
#define MAR_157_PCX_BIT_START                               2U
#define MAR_157_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_157 */
#define MAR_157_WTE_BIT_START                               1U
#define MAR_157_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_157 */
#define MAR_157_PC_BIT_START                                0U
#define MAR_157_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_158 */
#define MAR_158_RESV_1_BIT_START                            4U
#define MAR_158_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_158 */
#define MAR_158_PFX_BIT_START                               3U
#define MAR_158_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_158 */
#define MAR_158_PCX_BIT_START                               2U
#define MAR_158_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_158 */
#define MAR_158_WTE_BIT_START                               1U
#define MAR_158_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_158 */
#define MAR_158_PC_BIT_START                                0U
#define MAR_158_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_159 */
#define MAR_159_RESV_1_BIT_START                            4U
#define MAR_159_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_159 */
#define MAR_159_PFX_BIT_START                               3U
#define MAR_159_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_159 */
#define MAR_159_PCX_BIT_START                               2U
#define MAR_159_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_159 */
#define MAR_159_WTE_BIT_START                               1U
#define MAR_159_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_159 */
#define MAR_159_PC_BIT_START                                0U
#define MAR_159_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_160 */
#define MAR_160_RESV_1_BIT_START                            4U
#define MAR_160_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_160 */
#define MAR_160_PFX_BIT_START                               3U
#define MAR_160_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_160 */
#define MAR_160_PCX_BIT_START                               2U
#define MAR_160_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_160 */
#define MAR_160_WTE_BIT_START                               1U
#define MAR_160_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_160 */
#define MAR_160_PC_BIT_START                                0U
#define MAR_160_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_161 */
#define MAR_161_RESV_1_BIT_START                            4U
#define MAR_161_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_161 */
#define MAR_161_PFX_BIT_START                               3U
#define MAR_161_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_161 */
#define MAR_161_PCX_BIT_START                               2U
#define MAR_161_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_161 */
#define MAR_161_WTE_BIT_START                               1U
#define MAR_161_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_161 */
#define MAR_161_PC_BIT_START                                0U
#define MAR_161_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_162 */
#define MAR_162_RESV_1_BIT_START                            4U
#define MAR_162_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_162 */
#define MAR_162_PFX_BIT_START                               3U
#define MAR_162_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_162 */
#define MAR_162_PCX_BIT_START                               2U
#define MAR_162_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_162 */
#define MAR_162_WTE_BIT_START                               1U
#define MAR_162_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_162 */
#define MAR_162_PC_BIT_START                                0U
#define MAR_162_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_163 */
#define MAR_163_RESV_1_BIT_START                            4U
#define MAR_163_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_163 */
#define MAR_163_PFX_BIT_START                               3U
#define MAR_163_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_163 */
#define MAR_163_PCX_BIT_START                               2U
#define MAR_163_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_163 */
#define MAR_163_WTE_BIT_START                               1U
#define MAR_163_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_163 */
#define MAR_163_PC_BIT_START                                0U
#define MAR_163_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_164 */
#define MAR_164_RESV_1_BIT_START                            4U
#define MAR_164_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_164 */
#define MAR_164_PFX_BIT_START                               3U
#define MAR_164_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_164 */
#define MAR_164_PCX_BIT_START                               2U
#define MAR_164_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_164 */
#define MAR_164_WTE_BIT_START                               1U
#define MAR_164_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_164 */
#define MAR_164_PC_BIT_START                                0U
#define MAR_164_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_165 */
#define MAR_165_RESV_1_BIT_START                            4U
#define MAR_165_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_165 */
#define MAR_165_PFX_BIT_START                               3U
#define MAR_165_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_165 */
#define MAR_165_PCX_BIT_START                               2U
#define MAR_165_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_165 */
#define MAR_165_WTE_BIT_START                               1U
#define MAR_165_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_165 */
#define MAR_165_PC_BIT_START                                0U
#define MAR_165_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_166 */
#define MAR_166_RESV_1_BIT_START                            4U
#define MAR_166_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_166 */
#define MAR_166_PFX_BIT_START                               3U
#define MAR_166_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_166 */
#define MAR_166_PCX_BIT_START                               2U
#define MAR_166_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_166 */
#define MAR_166_WTE_BIT_START                               1U
#define MAR_166_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_166 */
#define MAR_166_PC_BIT_START                                0U
#define MAR_166_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_167 */
#define MAR_167_RESV_1_BIT_START                            4U
#define MAR_167_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_167 */
#define MAR_167_PFX_BIT_START                               3U
#define MAR_167_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_167 */
#define MAR_167_PCX_BIT_START                               2U
#define MAR_167_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_167 */
#define MAR_167_WTE_BIT_START                               1U
#define MAR_167_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_167 */
#define MAR_167_PC_BIT_START                                0U
#define MAR_167_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_168 */
#define MAR_168_RESV_1_BIT_START                            4U
#define MAR_168_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_168 */
#define MAR_168_PFX_BIT_START                               3U
#define MAR_168_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_168 */
#define MAR_168_PCX_BIT_START                               2U
#define MAR_168_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_168 */
#define MAR_168_WTE_BIT_START                               1U
#define MAR_168_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_168 */
#define MAR_168_PC_BIT_START                                0U
#define MAR_168_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_169 */
#define MAR_169_RESV_1_BIT_START                            4U
#define MAR_169_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_169 */
#define MAR_169_PFX_BIT_START                               3U
#define MAR_169_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_169 */
#define MAR_169_PCX_BIT_START                               2U
#define MAR_169_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_169 */
#define MAR_169_WTE_BIT_START                               1U
#define MAR_169_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_169 */
#define MAR_169_PC_BIT_START                                0U
#define MAR_169_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_170 */
#define MAR_170_RESV_1_BIT_START                            4U
#define MAR_170_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_170 */
#define MAR_170_PFX_BIT_START                               3U
#define MAR_170_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_170 */
#define MAR_170_PCX_BIT_START                               2U
#define MAR_170_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_170 */
#define MAR_170_WTE_BIT_START                               1U
#define MAR_170_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_170 */
#define MAR_170_PC_BIT_START                                0U
#define MAR_170_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_171 */
#define MAR_171_RESV_1_BIT_START                            4U
#define MAR_171_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_171 */
#define MAR_171_PFX_BIT_START                               3U
#define MAR_171_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_171 */
#define MAR_171_PCX_BIT_START                               2U
#define MAR_171_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_171 */
#define MAR_171_WTE_BIT_START                               1U
#define MAR_171_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_171 */
#define MAR_171_PC_BIT_START                                0U
#define MAR_171_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_172 */
#define MAR_172_RESV_1_BIT_START                            4U
#define MAR_172_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_172 */
#define MAR_172_PFX_BIT_START                               3U
#define MAR_172_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_172 */
#define MAR_172_PCX_BIT_START                               2U
#define MAR_172_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_172 */
#define MAR_172_WTE_BIT_START                               1U
#define MAR_172_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_172 */
#define MAR_172_PC_BIT_START                                0U
#define MAR_172_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_173 */
#define MAR_173_RESV_1_BIT_START                            4U
#define MAR_173_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_173 */
#define MAR_173_PFX_BIT_START                               3U
#define MAR_173_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_173 */
#define MAR_173_PCX_BIT_START                               2U
#define MAR_173_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_173 */
#define MAR_173_WTE_BIT_START                               1U
#define MAR_173_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_173 */
#define MAR_173_PC_BIT_START                                0U
#define MAR_173_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_174 */
#define MAR_174_RESV_1_BIT_START                            4U
#define MAR_174_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_174 */
#define MAR_174_PFX_BIT_START                               3U
#define MAR_174_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_174 */
#define MAR_174_PCX_BIT_START                               2U
#define MAR_174_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_174 */
#define MAR_174_WTE_BIT_START                               1U
#define MAR_174_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_174 */
#define MAR_174_PC_BIT_START                                0U
#define MAR_174_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_175 */
#define MAR_175_RESV_1_BIT_START                            4U
#define MAR_175_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_175 */
#define MAR_175_PFX_BIT_START                               3U
#define MAR_175_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_175 */
#define MAR_175_PCX_BIT_START                               2U
#define MAR_175_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_175 */
#define MAR_175_WTE_BIT_START                               1U
#define MAR_175_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_175 */
#define MAR_175_PC_BIT_START                                0U
#define MAR_175_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_176 */
#define MAR_176_RESV_1_BIT_START                            4U
#define MAR_176_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_176 */
#define MAR_176_PFX_BIT_START                               3U
#define MAR_176_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_176 */
#define MAR_176_PCX_BIT_START                               2U
#define MAR_176_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_176 */
#define MAR_176_WTE_BIT_START                               1U
#define MAR_176_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_176 */
#define MAR_176_PC_BIT_START                                0U
#define MAR_176_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_177 */
#define MAR_177_RESV_1_BIT_START                            4U
#define MAR_177_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_177 */
#define MAR_177_PFX_BIT_START                               3U
#define MAR_177_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_177 */
#define MAR_177_PCX_BIT_START                               2U
#define MAR_177_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_177 */
#define MAR_177_WTE_BIT_START                               1U
#define MAR_177_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_177 */
#define MAR_177_PC_BIT_START                                0U
#define MAR_177_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_178 */
#define MAR_178_RESV_1_BIT_START                            4U
#define MAR_178_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_178 */
#define MAR_178_PFX_BIT_START                               3U
#define MAR_178_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_178 */
#define MAR_178_PCX_BIT_START                               2U
#define MAR_178_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_178 */
#define MAR_178_WTE_BIT_START                               1U
#define MAR_178_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_178 */
#define MAR_178_PC_BIT_START                                0U
#define MAR_178_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_179 */
#define MAR_179_RESV_1_BIT_START                            4U
#define MAR_179_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_179 */
#define MAR_179_PFX_BIT_START                               3U
#define MAR_179_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_179 */
#define MAR_179_PCX_BIT_START                               2U
#define MAR_179_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_179 */
#define MAR_179_WTE_BIT_START                               1U
#define MAR_179_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_179 */
#define MAR_179_PC_BIT_START                                0U
#define MAR_179_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_180 */
#define MAR_180_RESV_1_BIT_START                            4U
#define MAR_180_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_180 */
#define MAR_180_PFX_BIT_START                               3U
#define MAR_180_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_180 */
#define MAR_180_PCX_BIT_START                               2U
#define MAR_180_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_180 */
#define MAR_180_WTE_BIT_START                               1U
#define MAR_180_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_180 */
#define MAR_180_PC_BIT_START                                0U
#define MAR_180_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_181 */
#define MAR_181_RESV_1_BIT_START                            4U
#define MAR_181_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_181 */
#define MAR_181_PFX_BIT_START                               3U
#define MAR_181_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_181 */
#define MAR_181_PCX_BIT_START                               2U
#define MAR_181_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_181 */
#define MAR_181_WTE_BIT_START                               1U
#define MAR_181_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_181 */
#define MAR_181_PC_BIT_START                                0U
#define MAR_181_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_182 */
#define MAR_182_RESV_1_BIT_START                            4U
#define MAR_182_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_182 */
#define MAR_182_PFX_BIT_START                               3U
#define MAR_182_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_182 */
#define MAR_182_PCX_BIT_START                               2U
#define MAR_182_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_182 */
#define MAR_182_WTE_BIT_START                               1U
#define MAR_182_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_182 */
#define MAR_182_PC_BIT_START                                0U
#define MAR_182_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_183 */
#define MAR_183_RESV_1_BIT_START                            4U
#define MAR_183_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_183 */
#define MAR_183_PFX_BIT_START                               3U
#define MAR_183_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_183 */
#define MAR_183_PCX_BIT_START                               2U
#define MAR_183_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_183 */
#define MAR_183_WTE_BIT_START                               1U
#define MAR_183_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_183 */
#define MAR_183_PC_BIT_START                                0U
#define MAR_183_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_184 */
#define MAR_184_RESV_1_BIT_START                            4U
#define MAR_184_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_184 */
#define MAR_184_PFX_BIT_START                               3U
#define MAR_184_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_184 */
#define MAR_184_PCX_BIT_START                               2U
#define MAR_184_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_184 */
#define MAR_184_WTE_BIT_START                               1U
#define MAR_184_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_184 */
#define MAR_184_PC_BIT_START                                0U
#define MAR_184_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_185 */
#define MAR_185_RESV_1_BIT_START                            4U
#define MAR_185_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_185 */
#define MAR_185_PFX_BIT_START                               3U
#define MAR_185_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_185 */
#define MAR_185_PCX_BIT_START                               2U
#define MAR_185_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_185 */
#define MAR_185_WTE_BIT_START                               1U
#define MAR_185_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_185 */
#define MAR_185_PC_BIT_START                                0U
#define MAR_185_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_186 */
#define MAR_186_RESV_1_BIT_START                            4U
#define MAR_186_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_186 */
#define MAR_186_PFX_BIT_START                               3U
#define MAR_186_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_186 */
#define MAR_186_PCX_BIT_START                               2U
#define MAR_186_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_186 */
#define MAR_186_WTE_BIT_START                               1U
#define MAR_186_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_186 */
#define MAR_186_PC_BIT_START                                0U
#define MAR_186_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_187 */
#define MAR_187_RESV_1_BIT_START                            4U
#define MAR_187_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_187 */
#define MAR_187_PFX_BIT_START                               3U
#define MAR_187_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_187 */
#define MAR_187_PCX_BIT_START                               2U
#define MAR_187_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_187 */
#define MAR_187_WTE_BIT_START                               1U
#define MAR_187_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_187 */
#define MAR_187_PC_BIT_START                                0U
#define MAR_187_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_188 */
#define MAR_188_RESV_1_BIT_START                            4U
#define MAR_188_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_188 */
#define MAR_188_PFX_BIT_START                               3U
#define MAR_188_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_188 */
#define MAR_188_PCX_BIT_START                               2U
#define MAR_188_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_188 */
#define MAR_188_WTE_BIT_START                               1U
#define MAR_188_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_188 */
#define MAR_188_PC_BIT_START                                0U
#define MAR_188_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_189 */
#define MAR_189_RESV_1_BIT_START                            4U
#define MAR_189_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_189 */
#define MAR_189_PFX_BIT_START                               3U
#define MAR_189_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_189 */
#define MAR_189_PCX_BIT_START                               2U
#define MAR_189_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_189 */
#define MAR_189_WTE_BIT_START                               1U
#define MAR_189_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_189 */
#define MAR_189_PC_BIT_START                                0U
#define MAR_189_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_190 */
#define MAR_190_RESV_1_BIT_START                            4U
#define MAR_190_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_190 */
#define MAR_190_PFX_BIT_START                               3U
#define MAR_190_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_190 */
#define MAR_190_PCX_BIT_START                               2U
#define MAR_190_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_190 */
#define MAR_190_WTE_BIT_START                               1U
#define MAR_190_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_190 */
#define MAR_190_PC_BIT_START                                0U
#define MAR_190_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_191 */
#define MAR_191_RESV_1_BIT_START                            4U
#define MAR_191_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_191 */
#define MAR_191_PFX_BIT_START                               3U
#define MAR_191_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_191 */
#define MAR_191_PCX_BIT_START                               2U
#define MAR_191_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_191 */
#define MAR_191_WTE_BIT_START                               1U
#define MAR_191_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_191 */
#define MAR_191_PC_BIT_START                                0U
#define MAR_191_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_192 */
#define MAR_192_RESV_1_BIT_START                            4U
#define MAR_192_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_192 */
#define MAR_192_PFX_BIT_START                               3U
#define MAR_192_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_192 */
#define MAR_192_PCX_BIT_START                               2U
#define MAR_192_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_192 */
#define MAR_192_WTE_BIT_START                               1U
#define MAR_192_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_192 */
#define MAR_192_PC_BIT_START                                0U
#define MAR_192_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_193 */
#define MAR_193_RESV_1_BIT_START                            4U
#define MAR_193_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_193 */
#define MAR_193_PFX_BIT_START                               3U
#define MAR_193_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_193 */
#define MAR_193_PCX_BIT_START                               2U
#define MAR_193_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_193 */
#define MAR_193_WTE_BIT_START                               1U
#define MAR_193_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_193 */
#define MAR_193_PC_BIT_START                                0U
#define MAR_193_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_194 */
#define MAR_194_RESV_1_BIT_START                            4U
#define MAR_194_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_194 */
#define MAR_194_PFX_BIT_START                               3U
#define MAR_194_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_194 */
#define MAR_194_PCX_BIT_START                               2U
#define MAR_194_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_194 */
#define MAR_194_WTE_BIT_START                               1U
#define MAR_194_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_194 */
#define MAR_194_PC_BIT_START                                0U
#define MAR_194_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_195 */
#define MAR_195_RESV_1_BIT_START                            4U
#define MAR_195_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_195 */
#define MAR_195_PFX_BIT_START                               3U
#define MAR_195_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_195 */
#define MAR_195_PCX_BIT_START                               2U
#define MAR_195_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_195 */
#define MAR_195_WTE_BIT_START                               1U
#define MAR_195_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_195 */
#define MAR_195_PC_BIT_START                                0U
#define MAR_195_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_196 */
#define MAR_196_RESV_1_BIT_START                            4U
#define MAR_196_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_196 */
#define MAR_196_PFX_BIT_START                               3U
#define MAR_196_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_196 */
#define MAR_196_PCX_BIT_START                               2U
#define MAR_196_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_196 */
#define MAR_196_WTE_BIT_START                               1U
#define MAR_196_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_196 */
#define MAR_196_PC_BIT_START                                0U
#define MAR_196_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_197 */
#define MAR_197_RESV_1_BIT_START                            4U
#define MAR_197_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_197 */
#define MAR_197_PFX_BIT_START                               3U
#define MAR_197_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_197 */
#define MAR_197_PCX_BIT_START                               2U
#define MAR_197_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_197 */
#define MAR_197_WTE_BIT_START                               1U
#define MAR_197_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_197 */
#define MAR_197_PC_BIT_START                                0U
#define MAR_197_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_198 */
#define MAR_198_RESV_1_BIT_START                            4U
#define MAR_198_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_198 */
#define MAR_198_PFX_BIT_START                               3U
#define MAR_198_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_198 */
#define MAR_198_PCX_BIT_START                               2U
#define MAR_198_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_198 */
#define MAR_198_WTE_BIT_START                               1U
#define MAR_198_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_198 */
#define MAR_198_PC_BIT_START                                0U
#define MAR_198_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_199 */
#define MAR_199_RESV_1_BIT_START                            4U
#define MAR_199_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_199 */
#define MAR_199_PFX_BIT_START                               3U
#define MAR_199_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_199 */
#define MAR_199_PCX_BIT_START                               2U
#define MAR_199_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_199 */
#define MAR_199_WTE_BIT_START                               1U
#define MAR_199_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_199 */
#define MAR_199_PC_BIT_START                                0U
#define MAR_199_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_200 */
#define MAR_200_RESV_1_BIT_START                            4U
#define MAR_200_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_200 */
#define MAR_200_PFX_BIT_START                               3U
#define MAR_200_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_200 */
#define MAR_200_PCX_BIT_START                               2U
#define MAR_200_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_200 */
#define MAR_200_WTE_BIT_START                               1U
#define MAR_200_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_200 */
#define MAR_200_PC_BIT_START                                0U
#define MAR_200_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_201 */
#define MAR_201_RESV_1_BIT_START                            4U
#define MAR_201_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_201 */
#define MAR_201_PFX_BIT_START                               3U
#define MAR_201_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_201 */
#define MAR_201_PCX_BIT_START                               2U
#define MAR_201_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_201 */
#define MAR_201_WTE_BIT_START                               1U
#define MAR_201_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_201 */
#define MAR_201_PC_BIT_START                                0U
#define MAR_201_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_202 */
#define MAR_202_RESV_1_BIT_START                            4U
#define MAR_202_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_202 */
#define MAR_202_PFX_BIT_START                               3U
#define MAR_202_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_202 */
#define MAR_202_PCX_BIT_START                               2U
#define MAR_202_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_202 */
#define MAR_202_WTE_BIT_START                               1U
#define MAR_202_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_202 */
#define MAR_202_PC_BIT_START                                0U
#define MAR_202_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_203 */
#define MAR_203_RESV_1_BIT_START                            4U
#define MAR_203_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_203 */
#define MAR_203_PFX_BIT_START                               3U
#define MAR_203_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_203 */
#define MAR_203_PCX_BIT_START                               2U
#define MAR_203_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_203 */
#define MAR_203_WTE_BIT_START                               1U
#define MAR_203_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_203 */
#define MAR_203_PC_BIT_START                                0U
#define MAR_203_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_204 */
#define MAR_204_RESV_1_BIT_START                            4U
#define MAR_204_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_204 */
#define MAR_204_PFX_BIT_START                               3U
#define MAR_204_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_204 */
#define MAR_204_PCX_BIT_START                               2U
#define MAR_204_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_204 */
#define MAR_204_WTE_BIT_START                               1U
#define MAR_204_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_204 */
#define MAR_204_PC_BIT_START                                0U
#define MAR_204_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_205 */
#define MAR_205_RESV_1_BIT_START                            4U
#define MAR_205_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_205 */
#define MAR_205_PFX_BIT_START                               3U
#define MAR_205_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_205 */
#define MAR_205_PCX_BIT_START                               2U
#define MAR_205_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_205 */
#define MAR_205_WTE_BIT_START                               1U
#define MAR_205_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_205 */
#define MAR_205_PC_BIT_START                                0U
#define MAR_205_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_206 */
#define MAR_206_RESV_1_BIT_START                            4U
#define MAR_206_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_206 */
#define MAR_206_PFX_BIT_START                               3U
#define MAR_206_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_206 */
#define MAR_206_PCX_BIT_START                               2U
#define MAR_206_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_206 */
#define MAR_206_WTE_BIT_START                               1U
#define MAR_206_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_206 */
#define MAR_206_PC_BIT_START                                0U
#define MAR_206_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_207 */
#define MAR_207_RESV_1_BIT_START                            4U
#define MAR_207_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_207 */
#define MAR_207_PFX_BIT_START                               3U
#define MAR_207_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_207 */
#define MAR_207_PCX_BIT_START                               2U
#define MAR_207_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_207 */
#define MAR_207_WTE_BIT_START                               1U
#define MAR_207_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_207 */
#define MAR_207_PC_BIT_START                                0U
#define MAR_207_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_208 */
#define MAR_208_RESV_1_BIT_START                            4U
#define MAR_208_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_208 */
#define MAR_208_PFX_BIT_START                               3U
#define MAR_208_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_208 */
#define MAR_208_PCX_BIT_START                               2U
#define MAR_208_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_208 */
#define MAR_208_WTE_BIT_START                               1U
#define MAR_208_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_208 */
#define MAR_208_PC_BIT_START                                0U
#define MAR_208_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_209 */
#define MAR_209_RESV_1_BIT_START                            4U
#define MAR_209_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_209 */
#define MAR_209_PFX_BIT_START                               3U
#define MAR_209_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_209 */
#define MAR_209_PCX_BIT_START                               2U
#define MAR_209_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_209 */
#define MAR_209_WTE_BIT_START                               1U
#define MAR_209_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_209 */
#define MAR_209_PC_BIT_START                                0U
#define MAR_209_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_210 */
#define MAR_210_RESV_1_BIT_START                            4U
#define MAR_210_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_210 */
#define MAR_210_PFX_BIT_START                               3U
#define MAR_210_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_210 */
#define MAR_210_PCX_BIT_START                               2U
#define MAR_210_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_210 */
#define MAR_210_WTE_BIT_START                               1U
#define MAR_210_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_210 */
#define MAR_210_PC_BIT_START                                0U
#define MAR_210_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_211 */
#define MAR_211_RESV_1_BIT_START                            4U
#define MAR_211_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_211 */
#define MAR_211_PFX_BIT_START                               3U
#define MAR_211_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_211 */
#define MAR_211_PCX_BIT_START                               2U
#define MAR_211_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_211 */
#define MAR_211_WTE_BIT_START                               1U
#define MAR_211_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_211 */
#define MAR_211_PC_BIT_START                                0U
#define MAR_211_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_212 */
#define MAR_212_RESV_1_BIT_START                            4U
#define MAR_212_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_212 */
#define MAR_212_PFX_BIT_START                               3U
#define MAR_212_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_212 */
#define MAR_212_PCX_BIT_START                               2U
#define MAR_212_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_212 */
#define MAR_212_WTE_BIT_START                               1U
#define MAR_212_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_212 */
#define MAR_212_PC_BIT_START                                0U
#define MAR_212_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_213 */
#define MAR_213_RESV_1_BIT_START                            4U
#define MAR_213_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_213 */
#define MAR_213_PFX_BIT_START                               3U
#define MAR_213_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_213 */
#define MAR_213_PCX_BIT_START                               2U
#define MAR_213_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_213 */
#define MAR_213_WTE_BIT_START                               1U
#define MAR_213_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_213 */
#define MAR_213_PC_BIT_START                                0U
#define MAR_213_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_214 */
#define MAR_214_RESV_1_BIT_START                            4U
#define MAR_214_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_214 */
#define MAR_214_PFX_BIT_START                               3U
#define MAR_214_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_214 */
#define MAR_214_PCX_BIT_START                               2U
#define MAR_214_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_214 */
#define MAR_214_WTE_BIT_START                               1U
#define MAR_214_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_214 */
#define MAR_214_PC_BIT_START                                0U
#define MAR_214_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_215 */
#define MAR_215_RESV_1_BIT_START                            4U
#define MAR_215_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_215 */
#define MAR_215_PFX_BIT_START                               3U
#define MAR_215_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_215 */
#define MAR_215_PCX_BIT_START                               2U
#define MAR_215_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_215 */
#define MAR_215_WTE_BIT_START                               1U
#define MAR_215_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_215 */
#define MAR_215_PC_BIT_START                                0U
#define MAR_215_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_216 */
#define MAR_216_RESV_1_BIT_START                            4U
#define MAR_216_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_216 */
#define MAR_216_PFX_BIT_START                               3U
#define MAR_216_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_216 */
#define MAR_216_PCX_BIT_START                               2U
#define MAR_216_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_216 */
#define MAR_216_WTE_BIT_START                               1U
#define MAR_216_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_216 */
#define MAR_216_PC_BIT_START                                0U
#define MAR_216_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_217 */
#define MAR_217_RESV_1_BIT_START                            4U
#define MAR_217_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_217 */
#define MAR_217_PFX_BIT_START                               3U
#define MAR_217_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_217 */
#define MAR_217_PCX_BIT_START                               2U
#define MAR_217_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_217 */
#define MAR_217_WTE_BIT_START                               1U
#define MAR_217_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_217 */
#define MAR_217_PC_BIT_START                                0U
#define MAR_217_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_218 */
#define MAR_218_RESV_1_BIT_START                            4U
#define MAR_218_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_218 */
#define MAR_218_PFX_BIT_START                               3U
#define MAR_218_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_218 */
#define MAR_218_PCX_BIT_START                               2U
#define MAR_218_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_218 */
#define MAR_218_WTE_BIT_START                               1U
#define MAR_218_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_218 */
#define MAR_218_PC_BIT_START                                0U
#define MAR_218_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_219 */
#define MAR_219_RESV_1_BIT_START                            4U
#define MAR_219_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_219 */
#define MAR_219_PFX_BIT_START                               3U
#define MAR_219_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_219 */
#define MAR_219_PCX_BIT_START                               2U
#define MAR_219_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_219 */
#define MAR_219_WTE_BIT_START                               1U
#define MAR_219_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_219 */
#define MAR_219_PC_BIT_START                                0U
#define MAR_219_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_220 */
#define MAR_220_RESV_1_BIT_START                            4U
#define MAR_220_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_220 */
#define MAR_220_PFX_BIT_START                               3U
#define MAR_220_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_220 */
#define MAR_220_PCX_BIT_START                               2U
#define MAR_220_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_220 */
#define MAR_220_WTE_BIT_START                               1U
#define MAR_220_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_220 */
#define MAR_220_PC_BIT_START                                0U
#define MAR_220_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_221 */
#define MAR_221_RESV_1_BIT_START                            4U
#define MAR_221_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_221 */
#define MAR_221_PFX_BIT_START                               3U
#define MAR_221_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_221 */
#define MAR_221_PCX_BIT_START                               2U
#define MAR_221_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_221 */
#define MAR_221_WTE_BIT_START                               1U
#define MAR_221_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_221 */
#define MAR_221_PC_BIT_START                                0U
#define MAR_221_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_222 */
#define MAR_222_RESV_1_BIT_START                            4U
#define MAR_222_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_222 */
#define MAR_222_PFX_BIT_START                               3U
#define MAR_222_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_222 */
#define MAR_222_PCX_BIT_START                               2U
#define MAR_222_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_222 */
#define MAR_222_WTE_BIT_START                               1U
#define MAR_222_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_222 */
#define MAR_222_PC_BIT_START                                0U
#define MAR_222_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_223 */
#define MAR_223_RESV_1_BIT_START                            4U
#define MAR_223_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_223 */
#define MAR_223_PFX_BIT_START                               3U
#define MAR_223_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_223 */
#define MAR_223_PCX_BIT_START                               2U
#define MAR_223_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_223 */
#define MAR_223_WTE_BIT_START                               1U
#define MAR_223_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_223 */
#define MAR_223_PC_BIT_START                                0U
#define MAR_223_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_224 */
#define MAR_224_RESV_1_BIT_START                            4U
#define MAR_224_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_224 */
#define MAR_224_PFX_BIT_START                               3U
#define MAR_224_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_224 */
#define MAR_224_PCX_BIT_START                               2U
#define MAR_224_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_224 */
#define MAR_224_WTE_BIT_START                               1U
#define MAR_224_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_224 */
#define MAR_224_PC_BIT_START                                0U
#define MAR_224_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_225 */
#define MAR_225_RESV_1_BIT_START                            4U
#define MAR_225_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_225 */
#define MAR_225_PFX_BIT_START                               3U
#define MAR_225_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_225 */
#define MAR_225_PCX_BIT_START                               2U
#define MAR_225_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_225 */
#define MAR_225_WTE_BIT_START                               1U
#define MAR_225_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_225 */
#define MAR_225_PC_BIT_START                                0U
#define MAR_225_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_226 */
#define MAR_226_RESV_1_BIT_START                            4U
#define MAR_226_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_226 */
#define MAR_226_PFX_BIT_START                               3U
#define MAR_226_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_226 */
#define MAR_226_PCX_BIT_START                               2U
#define MAR_226_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_226 */
#define MAR_226_WTE_BIT_START                               1U
#define MAR_226_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_226 */
#define MAR_226_PC_BIT_START                                0U
#define MAR_226_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_227 */
#define MAR_227_RESV_1_BIT_START                            4U
#define MAR_227_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_227 */
#define MAR_227_PFX_BIT_START                               3U
#define MAR_227_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_227 */
#define MAR_227_PCX_BIT_START                               2U
#define MAR_227_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_227 */
#define MAR_227_WTE_BIT_START                               1U
#define MAR_227_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_227 */
#define MAR_227_PC_BIT_START                                0U
#define MAR_227_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_228 */
#define MAR_228_RESV_1_BIT_START                            4U
#define MAR_228_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_228 */
#define MAR_228_PFX_BIT_START                               3U
#define MAR_228_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_228 */
#define MAR_228_PCX_BIT_START                               2U
#define MAR_228_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_228 */
#define MAR_228_WTE_BIT_START                               1U
#define MAR_228_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_228 */
#define MAR_228_PC_BIT_START                                0U
#define MAR_228_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_229 */
#define MAR_229_RESV_1_BIT_START                            4U
#define MAR_229_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_229 */
#define MAR_229_PFX_BIT_START                               3U
#define MAR_229_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_229 */
#define MAR_229_PCX_BIT_START                               2U
#define MAR_229_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_229 */
#define MAR_229_WTE_BIT_START                               1U
#define MAR_229_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_229 */
#define MAR_229_PC_BIT_START                                0U
#define MAR_229_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_230 */
#define MAR_230_RESV_1_BIT_START                            4U
#define MAR_230_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_230 */
#define MAR_230_PFX_BIT_START                               3U
#define MAR_230_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_230 */
#define MAR_230_PCX_BIT_START                               2U
#define MAR_230_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_230 */
#define MAR_230_WTE_BIT_START                               1U
#define MAR_230_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_230 */
#define MAR_230_PC_BIT_START                                0U
#define MAR_230_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_231 */
#define MAR_231_RESV_1_BIT_START                            4U
#define MAR_231_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_231 */
#define MAR_231_PFX_BIT_START                               3U
#define MAR_231_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_231 */
#define MAR_231_PCX_BIT_START                               2U
#define MAR_231_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_231 */
#define MAR_231_WTE_BIT_START                               1U
#define MAR_231_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_231 */
#define MAR_231_PC_BIT_START                                0U
#define MAR_231_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_232 */
#define MAR_232_RESV_1_BIT_START                            4U
#define MAR_232_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_232 */
#define MAR_232_PFX_BIT_START                               3U
#define MAR_232_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_232 */
#define MAR_232_PCX_BIT_START                               2U
#define MAR_232_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_232 */
#define MAR_232_WTE_BIT_START                               1U
#define MAR_232_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_232 */
#define MAR_232_PC_BIT_START                                0U
#define MAR_232_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_233 */
#define MAR_233_RESV_1_BIT_START                            4U
#define MAR_233_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_233 */
#define MAR_233_PFX_BIT_START                               3U
#define MAR_233_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_233 */
#define MAR_233_PCX_BIT_START                               2U
#define MAR_233_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_233 */
#define MAR_233_WTE_BIT_START                               1U
#define MAR_233_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_233 */
#define MAR_233_PC_BIT_START                                0U
#define MAR_233_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_234 */
#define MAR_234_RESV_1_BIT_START                            4U
#define MAR_234_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_234 */
#define MAR_234_PFX_BIT_START                               3U
#define MAR_234_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_234 */
#define MAR_234_PCX_BIT_START                               2U
#define MAR_234_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_234 */
#define MAR_234_WTE_BIT_START                               1U
#define MAR_234_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_234 */
#define MAR_234_PC_BIT_START                                0U
#define MAR_234_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_235 */
#define MAR_235_RESV_1_BIT_START                            4U
#define MAR_235_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_235 */
#define MAR_235_PFX_BIT_START                               3U
#define MAR_235_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_235 */
#define MAR_235_PCX_BIT_START                               2U
#define MAR_235_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_235 */
#define MAR_235_WTE_BIT_START                               1U
#define MAR_235_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_235 */
#define MAR_235_PC_BIT_START                                0U
#define MAR_235_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_236 */
#define MAR_236_RESV_1_BIT_START                            4U
#define MAR_236_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_236 */
#define MAR_236_PFX_BIT_START                               3U
#define MAR_236_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_236 */
#define MAR_236_PCX_BIT_START                               2U
#define MAR_236_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_236 */
#define MAR_236_WTE_BIT_START                               1U
#define MAR_236_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_236 */
#define MAR_236_PC_BIT_START                                0U
#define MAR_236_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_237 */
#define MAR_237_RESV_1_BIT_START                            4U
#define MAR_237_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_237 */
#define MAR_237_PFX_BIT_START                               3U
#define MAR_237_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_237 */
#define MAR_237_PCX_BIT_START                               2U
#define MAR_237_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_237 */
#define MAR_237_WTE_BIT_START                               1U
#define MAR_237_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_237 */
#define MAR_237_PC_BIT_START                                0U
#define MAR_237_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_238 */
#define MAR_238_RESV_1_BIT_START                            4U
#define MAR_238_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_238 */
#define MAR_238_PFX_BIT_START                               3U
#define MAR_238_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_238 */
#define MAR_238_PCX_BIT_START                               2U
#define MAR_238_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_238 */
#define MAR_238_WTE_BIT_START                               1U
#define MAR_238_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_238 */
#define MAR_238_PC_BIT_START                                0U
#define MAR_238_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_239 */
#define MAR_239_RESV_1_BIT_START                            4U
#define MAR_239_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_239 */
#define MAR_239_PFX_BIT_START                               3U
#define MAR_239_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_239 */
#define MAR_239_PCX_BIT_START                               2U
#define MAR_239_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_239 */
#define MAR_239_WTE_BIT_START                               1U
#define MAR_239_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_239 */
#define MAR_239_PC_BIT_START                                0U
#define MAR_239_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_240 */
#define MAR_240_RESV_1_BIT_START                            4U
#define MAR_240_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_240 */
#define MAR_240_PFX_BIT_START                               3U
#define MAR_240_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_240 */
#define MAR_240_PCX_BIT_START                               2U
#define MAR_240_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_240 */
#define MAR_240_WTE_BIT_START                               1U
#define MAR_240_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_240 */
#define MAR_240_PC_BIT_START                                0U
#define MAR_240_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_241 */
#define MAR_241_RESV_1_BIT_START                            4U
#define MAR_241_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_241 */
#define MAR_241_PFX_BIT_START                               3U
#define MAR_241_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_241 */
#define MAR_241_PCX_BIT_START                               2U
#define MAR_241_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_241 */
#define MAR_241_WTE_BIT_START                               1U
#define MAR_241_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_241 */
#define MAR_241_PC_BIT_START                                0U
#define MAR_241_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_242 */
#define MAR_242_RESV_1_BIT_START                            4U
#define MAR_242_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_242 */
#define MAR_242_PFX_BIT_START                               3U
#define MAR_242_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_242 */
#define MAR_242_PCX_BIT_START                               2U
#define MAR_242_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_242 */
#define MAR_242_WTE_BIT_START                               1U
#define MAR_242_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_242 */
#define MAR_242_PC_BIT_START                                0U
#define MAR_242_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_243 */
#define MAR_243_RESV_1_BIT_START                            4U
#define MAR_243_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_243 */
#define MAR_243_PFX_BIT_START                               3U
#define MAR_243_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_243 */
#define MAR_243_PCX_BIT_START                               2U
#define MAR_243_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_243 */
#define MAR_243_WTE_BIT_START                               1U
#define MAR_243_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_243 */
#define MAR_243_PC_BIT_START                                0U
#define MAR_243_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_244 */
#define MAR_244_RESV_1_BIT_START                            4U
#define MAR_244_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_244 */
#define MAR_244_PFX_BIT_START                               3U
#define MAR_244_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_244 */
#define MAR_244_PCX_BIT_START                               2U
#define MAR_244_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_244 */
#define MAR_244_WTE_BIT_START                               1U
#define MAR_244_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_244 */
#define MAR_244_PC_BIT_START                                0U
#define MAR_244_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_245 */
#define MAR_245_RESV_1_BIT_START                            4U
#define MAR_245_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_245 */
#define MAR_245_PFX_BIT_START                               3U
#define MAR_245_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_245 */
#define MAR_245_PCX_BIT_START                               2U
#define MAR_245_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_245 */
#define MAR_245_WTE_BIT_START                               1U
#define MAR_245_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_245 */
#define MAR_245_PC_BIT_START                                0U
#define MAR_245_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_246 */
#define MAR_246_RESV_1_BIT_START                            4U
#define MAR_246_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_246 */
#define MAR_246_PFX_BIT_START                               3U
#define MAR_246_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_246 */
#define MAR_246_PCX_BIT_START                               2U
#define MAR_246_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_246 */
#define MAR_246_WTE_BIT_START                               1U
#define MAR_246_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_246 */
#define MAR_246_PC_BIT_START                                0U
#define MAR_246_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_247 */
#define MAR_247_RESV_1_BIT_START                            4U
#define MAR_247_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_247 */
#define MAR_247_PFX_BIT_START                               3U
#define MAR_247_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_247 */
#define MAR_247_PCX_BIT_START                               2U
#define MAR_247_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_247 */
#define MAR_247_WTE_BIT_START                               1U
#define MAR_247_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_247 */
#define MAR_247_PC_BIT_START                                0U
#define MAR_247_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_248 */
#define MAR_248_RESV_1_BIT_START                            4U
#define MAR_248_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_248 */
#define MAR_248_PFX_BIT_START                               3U
#define MAR_248_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_248 */
#define MAR_248_PCX_BIT_START                               2U
#define MAR_248_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_248 */
#define MAR_248_WTE_BIT_START                               1U
#define MAR_248_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_248 */
#define MAR_248_PC_BIT_START                                0U
#define MAR_248_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_249 */
#define MAR_249_RESV_1_BIT_START                            4U
#define MAR_249_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_249 */
#define MAR_249_PFX_BIT_START                               3U
#define MAR_249_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_249 */
#define MAR_249_PCX_BIT_START                               2U
#define MAR_249_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_249 */
#define MAR_249_WTE_BIT_START                               1U
#define MAR_249_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_249 */
#define MAR_249_PC_BIT_START                                0U
#define MAR_249_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_250 */
#define MAR_250_RESV_1_BIT_START                            4U
#define MAR_250_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_250 */
#define MAR_250_PFX_BIT_START                               3U
#define MAR_250_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_250 */
#define MAR_250_PCX_BIT_START                               2U
#define MAR_250_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_250 */
#define MAR_250_WTE_BIT_START                               1U
#define MAR_250_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_250 */
#define MAR_250_PC_BIT_START                                0U
#define MAR_250_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_251 */
#define MAR_251_RESV_1_BIT_START                            4U
#define MAR_251_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_251 */
#define MAR_251_PFX_BIT_START                               3U
#define MAR_251_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_251 */
#define MAR_251_PCX_BIT_START                               2U
#define MAR_251_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_251 */
#define MAR_251_WTE_BIT_START                               1U
#define MAR_251_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_251 */
#define MAR_251_PC_BIT_START                                0U
#define MAR_251_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_252 */
#define MAR_252_RESV_1_BIT_START                            4U
#define MAR_252_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_252 */
#define MAR_252_PFX_BIT_START                               3U
#define MAR_252_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_252 */
#define MAR_252_PCX_BIT_START                               2U
#define MAR_252_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_252 */
#define MAR_252_WTE_BIT_START                               1U
#define MAR_252_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_252 */
#define MAR_252_PC_BIT_START                                0U
#define MAR_252_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_253 */
#define MAR_253_RESV_1_BIT_START                            4U
#define MAR_253_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_253 */
#define MAR_253_PFX_BIT_START                               3U
#define MAR_253_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_253 */
#define MAR_253_PCX_BIT_START                               2U
#define MAR_253_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_253 */
#define MAR_253_WTE_BIT_START                               1U
#define MAR_253_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_253 */
#define MAR_253_PC_BIT_START                                0U
#define MAR_253_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_254 */
#define MAR_254_RESV_1_BIT_START                            4U
#define MAR_254_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_254 */
#define MAR_254_PFX_BIT_START                               3U
#define MAR_254_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_254 */
#define MAR_254_PCX_BIT_START                               2U
#define MAR_254_PCX_BIT_END                                 2U

/* Definition for field WTE in Register MAR_254 */
#define MAR_254_WTE_BIT_START                               1U
#define MAR_254_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_254 */
#define MAR_254_PC_BIT_START                                0U
#define MAR_254_PC_BIT_END                                  0U

/* Definition for field RESV_1 in Register MAR_255 */
#define MAR_255_RESV_1_BIT_START                            4U
#define MAR_255_RESV_1_BIT_END                              31U

/* Definition for field PFX in Register MAR_255 */
#define MAR_255_PFX_BIT_START                               3U
#define MAR_255_PFX_BIT_END                                 3U

/* Definition for field PCX in Register MAR_255 */
#define MAR_255_PCX_BIT_START                               2U
#define MAR_255_PCX_BIT_END                                 2U
/* Definition for field WTE in Register MAR_255 */
#define MAR_255_WTE_BIT_START                               1U
#define MAR_255_WTE_BIT_END                                 1U

/* Definition for field PC in Register MAR_255 */
#define MAR_255_PC_BIT_START                                0U
#define MAR_255_PC_BIT_END                                  0U

#else 
/* Definition for field RESV_1 in Register MAR_255 */
#define MAR_RESV_1_BIT_START                                4U
#define MAR_RESV_1_BIT_END                                  31U

/* Definition for field PFX in Register MAR_255 */
#define MAR_PFX_BIT_START                                   3U
#define MAR_PFX_BIT_END                                     3U

/* Definition for field PCX in Register MAR_255 */
#define MAR_PCX_BIT_START                                   2U
#define MAR_PCX_BIT_END                                     2U

/* Definition for field WTE in Register MAR_255 */
#define MAR_WTE_BIT_START                                   1U
#define MAR_WTE_BIT_END                                     1U

/* Definition for field PC in Register MAR_255 */
#define MAR_PC_BIT_START                                    0U
#define MAR_PC_BIT_END                                      0U
#endif

/* Definition for field ADDR in Register L2MPFAR */
#define L2MPFAR_ADDR_BIT_START                              0U
#define L2MPFAR_ADDR_BIT_END                                31U

/* Definition for field RESV_1 in Register L2MPFSR */
#define L2MPFSR_RESV_1_BIT_START                            16U
#define L2MPFSR_RESV_1_BIT_END                              31U

/* Definition for field FID in Register L2MPFSR */
#define L2MPFSR_FID_BIT_START                               9U
#define L2MPFSR_FID_BIT_END                                 15U

/* Definition for field LOCAL in Register L2MPFSR */
#define L2MPFSR_LOCAL_BIT_START                             8U
#define L2MPFSR_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPFSR */
#define L2MPFSR_NS_BIT_START                                7U
#define L2MPFSR_NS_BIT_END                                  7U

/* Definition for field RESV_2 in Register L2MPFSR */
#define L2MPFSR_RESV_2_BIT_START                            6U
#define L2MPFSR_RESV_2_BIT_END                              6U

/* Definition for field SR in Register L2MPFSR */
#define L2MPFSR_SR_BIT_START                                5U
#define L2MPFSR_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPFSR */
#define L2MPFSR_SW_BIT_START                                4U
#define L2MPFSR_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPFSR */
#define L2MPFSR_SX_BIT_START                                3U
#define L2MPFSR_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPFSR */
#define L2MPFSR_UR_BIT_START                                2U
#define L2MPFSR_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPFSR */
#define L2MPFSR_UW_BIT_START                                1U
#define L2MPFSR_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPFSR */
#define L2MPFSR_UX_BIT_START                                0U
#define L2MPFSR_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPFCR */
#define L2MPFCR_RESV_1_BIT_START                            1U
#define L2MPFCR_RESV_1_BIT_END                              31U

/* Definition for field MPFCLR in Register L2MPFCR */
#define L2MPFCR_MPFCLR_BIT_START                            0U
#define L2MPFCR_MPFCLR_BIT_END                              0U

/* Definition for field RESV_1 in Register L2MPPA0 */
#define L2MPPA0_RESV_1_BIT_START                            16U
#define L2MPPA0_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA0 */
#define L2MPPA0_AID5_BIT_START                              15U
#define L2MPPA0_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA0 */
#define L2MPPA0_AID4_BIT_START                              14U
#define L2MPPA0_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA0 */
#define L2MPPA0_AID3_BIT_START                              13U
#define L2MPPA0_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA0 */
#define L2MPPA0_AID2_BIT_START                              12U
#define L2MPPA0_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA0 */
#define L2MPPA0_AID1_BIT_START                              11U
#define L2MPPA0_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA0 */
#define L2MPPA0_AID0_BIT_START                              10U
#define L2MPPA0_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA0 */
#define L2MPPA0_AIDX_BIT_START                              9U
#define L2MPPA0_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA0 */
#define L2MPPA0_LOCAL_BIT_START                             8U
#define L2MPPA0_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA0 */
#define L2MPPA0_NS_BIT_START                                7U
#define L2MPPA0_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA0 */
#define L2MPPA0_EMU_BIT_START                               6U
#define L2MPPA0_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA0 */
#define L2MPPA0_SR_BIT_START                                5U
#define L2MPPA0_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA0 */
#define L2MPPA0_SW_BIT_START                                4U
#define L2MPPA0_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA0 */
#define L2MPPA0_SX_BIT_START                                3U
#define L2MPPA0_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA0 */
#define L2MPPA0_UR_BIT_START                                2U
#define L2MPPA0_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA0 */
#define L2MPPA0_UW_BIT_START                                1U
#define L2MPPA0_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA0 */
#define L2MPPA0_UX_BIT_START                                0U
#define L2MPPA0_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA1 */
#define L2MPPA1_RESV_1_BIT_START                            16U
#define L2MPPA1_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA1 */
#define L2MPPA1_AID5_BIT_START                              15U
#define L2MPPA1_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA1 */
#define L2MPPA1_AID4_BIT_START                              14U
#define L2MPPA1_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA1 */
#define L2MPPA1_AID3_BIT_START                              13U
#define L2MPPA1_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA1 */
#define L2MPPA1_AID2_BIT_START                              12U
#define L2MPPA1_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA1 */
#define L2MPPA1_AID1_BIT_START                              11U
#define L2MPPA1_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA1 */
#define L2MPPA1_AID0_BIT_START                              10U
#define L2MPPA1_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA1 */
#define L2MPPA1_AIDX_BIT_START                              9U
#define L2MPPA1_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA1 */
#define L2MPPA1_LOCAL_BIT_START                             8U
#define L2MPPA1_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA1 */
#define L2MPPA1_NS_BIT_START                                7U
#define L2MPPA1_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA1 */
#define L2MPPA1_EMU_BIT_START                               6U
#define L2MPPA1_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA1 */
#define L2MPPA1_SR_BIT_START                                5U
#define L2MPPA1_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA1 */
#define L2MPPA1_SW_BIT_START                                4U
#define L2MPPA1_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA1 */
#define L2MPPA1_SX_BIT_START                                3U
#define L2MPPA1_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA1 */
#define L2MPPA1_UR_BIT_START                                2U
#define L2MPPA1_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA1 */
#define L2MPPA1_UW_BIT_START                                1U
#define L2MPPA1_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA1 */
#define L2MPPA1_UX_BIT_START                                0U
#define L2MPPA1_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA2 */
#define L2MPPA2_RESV_1_BIT_START                            16U
#define L2MPPA2_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA2 */
#define L2MPPA2_AID5_BIT_START                              15U
#define L2MPPA2_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA2 */
#define L2MPPA2_AID4_BIT_START                              14U
#define L2MPPA2_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA2 */
#define L2MPPA2_AID3_BIT_START                              13U
#define L2MPPA2_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA2 */
#define L2MPPA2_AID2_BIT_START                              12U
#define L2MPPA2_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA2 */
#define L2MPPA2_AID1_BIT_START                              11U
#define L2MPPA2_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA2 */
#define L2MPPA2_AID0_BIT_START                              10U
#define L2MPPA2_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA2 */
#define L2MPPA2_AIDX_BIT_START                              9U
#define L2MPPA2_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA2 */
#define L2MPPA2_LOCAL_BIT_START                             8U
#define L2MPPA2_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA2 */
#define L2MPPA2_NS_BIT_START                                7U
#define L2MPPA2_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA2 */
#define L2MPPA2_EMU_BIT_START                               6U
#define L2MPPA2_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA2 */
#define L2MPPA2_SR_BIT_START                                5U
#define L2MPPA2_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA2 */
#define L2MPPA2_SW_BIT_START                                4U
#define L2MPPA2_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA2 */
#define L2MPPA2_SX_BIT_START                                3U
#define L2MPPA2_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA2 */
#define L2MPPA2_UR_BIT_START                                2U
#define L2MPPA2_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA2 */
#define L2MPPA2_UW_BIT_START                                1U
#define L2MPPA2_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA2 */
#define L2MPPA2_UX_BIT_START                                0U
#define L2MPPA2_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA3 */
#define L2MPPA3_RESV_1_BIT_START                            16U
#define L2MPPA3_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA3 */
#define L2MPPA3_AID5_BIT_START                              15U
#define L2MPPA3_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA3 */
#define L2MPPA3_AID4_BIT_START                              14U
#define L2MPPA3_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA3 */
#define L2MPPA3_AID3_BIT_START                              13U
#define L2MPPA3_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA3 */
#define L2MPPA3_AID2_BIT_START                              12U
#define L2MPPA3_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA3 */
#define L2MPPA3_AID1_BIT_START                              11U
#define L2MPPA3_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA3 */
#define L2MPPA3_AID0_BIT_START                              10U
#define L2MPPA3_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA3 */
#define L2MPPA3_AIDX_BIT_START                              9U
#define L2MPPA3_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA3 */
#define L2MPPA3_LOCAL_BIT_START                             8U
#define L2MPPA3_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA3 */
#define L2MPPA3_NS_BIT_START                                7U
#define L2MPPA3_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA3 */
#define L2MPPA3_EMU_BIT_START                               6U
#define L2MPPA3_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA3 */
#define L2MPPA3_SR_BIT_START                                5U
#define L2MPPA3_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA3 */
#define L2MPPA3_SW_BIT_START                                4U
#define L2MPPA3_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA3 */
#define L2MPPA3_SX_BIT_START                                3U
#define L2MPPA3_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA3 */
#define L2MPPA3_UR_BIT_START                                2U
#define L2MPPA3_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA3 */
#define L2MPPA3_UW_BIT_START                                1U
#define L2MPPA3_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA3 */
#define L2MPPA3_UX_BIT_START                                0U
#define L2MPPA3_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA4 */
#define L2MPPA4_RESV_1_BIT_START                            16U
#define L2MPPA4_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA4 */
#define L2MPPA4_AID5_BIT_START                              15U
#define L2MPPA4_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA4 */
#define L2MPPA4_AID4_BIT_START                              14U
#define L2MPPA4_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA4 */
#define L2MPPA4_AID3_BIT_START                              13U
#define L2MPPA4_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA4 */
#define L2MPPA4_AID2_BIT_START                              12U
#define L2MPPA4_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA4 */
#define L2MPPA4_AID1_BIT_START                              11U
#define L2MPPA4_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA4 */
#define L2MPPA4_AID0_BIT_START                              10U
#define L2MPPA4_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA4 */
#define L2MPPA4_AIDX_BIT_START                              9U
#define L2MPPA4_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA4 */
#define L2MPPA4_LOCAL_BIT_START                             8U
#define L2MPPA4_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA4 */
#define L2MPPA4_NS_BIT_START                                7U
#define L2MPPA4_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA4 */
#define L2MPPA4_EMU_BIT_START                               6U
#define L2MPPA4_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA4 */
#define L2MPPA4_SR_BIT_START                                5U
#define L2MPPA4_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA4 */
#define L2MPPA4_SW_BIT_START                                4U
#define L2MPPA4_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA4 */
#define L2MPPA4_SX_BIT_START                                3U
#define L2MPPA4_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA4 */
#define L2MPPA4_UR_BIT_START                                2U
#define L2MPPA4_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA4 */
#define L2MPPA4_UW_BIT_START                                1U
#define L2MPPA4_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA4 */
#define L2MPPA4_UX_BIT_START                                0U
#define L2MPPA4_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA5 */
#define L2MPPA5_RESV_1_BIT_START                            16U
#define L2MPPA5_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA5 */
#define L2MPPA5_AID5_BIT_START                              15U
#define L2MPPA5_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA5 */
#define L2MPPA5_AID4_BIT_START                              14U
#define L2MPPA5_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA5 */
#define L2MPPA5_AID3_BIT_START                              13U
#define L2MPPA5_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA5 */
#define L2MPPA5_AID2_BIT_START                              12U
#define L2MPPA5_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA5 */
#define L2MPPA5_AID1_BIT_START                              11U
#define L2MPPA5_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA5 */
#define L2MPPA5_AID0_BIT_START                              10U
#define L2MPPA5_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA5 */
#define L2MPPA5_AIDX_BIT_START                              9U
#define L2MPPA5_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA5 */
#define L2MPPA5_LOCAL_BIT_START                             8U
#define L2MPPA5_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA5 */
#define L2MPPA5_NS_BIT_START                                7U
#define L2MPPA5_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA5 */
#define L2MPPA5_EMU_BIT_START                               6U
#define L2MPPA5_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA5 */
#define L2MPPA5_SR_BIT_START                                5U
#define L2MPPA5_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA5 */
#define L2MPPA5_SW_BIT_START                                4U
#define L2MPPA5_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA5 */
#define L2MPPA5_SX_BIT_START                                3U
#define L2MPPA5_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA5 */
#define L2MPPA5_UR_BIT_START                                2U
#define L2MPPA5_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA5 */
#define L2MPPA5_UW_BIT_START                                1U
#define L2MPPA5_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA5 */
#define L2MPPA5_UX_BIT_START                                0U
#define L2MPPA5_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA6 */
#define L2MPPA6_RESV_1_BIT_START                            16U
#define L2MPPA6_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA6 */
#define L2MPPA6_AID5_BIT_START                              15U
#define L2MPPA6_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA6 */
#define L2MPPA6_AID4_BIT_START                              14U
#define L2MPPA6_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA6 */
#define L2MPPA6_AID3_BIT_START                              13U
#define L2MPPA6_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA6 */
#define L2MPPA6_AID2_BIT_START                              12U
#define L2MPPA6_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA6 */
#define L2MPPA6_AID1_BIT_START                              11U
#define L2MPPA6_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA6 */
#define L2MPPA6_AID0_BIT_START                              10U
#define L2MPPA6_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA6 */
#define L2MPPA6_AIDX_BIT_START                              9U
#define L2MPPA6_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA6 */
#define L2MPPA6_LOCAL_BIT_START                             8U
#define L2MPPA6_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA6 */
#define L2MPPA6_NS_BIT_START                                7U
#define L2MPPA6_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA6 */
#define L2MPPA6_EMU_BIT_START                               6U
#define L2MPPA6_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA6 */
#define L2MPPA6_SR_BIT_START                                5U
#define L2MPPA6_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA6 */
#define L2MPPA6_SW_BIT_START                                4U
#define L2MPPA6_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA6 */
#define L2MPPA6_SX_BIT_START                                3U
#define L2MPPA6_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA6 */
#define L2MPPA6_UR_BIT_START                                2U
#define L2MPPA6_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA6 */
#define L2MPPA6_UW_BIT_START                                1U
#define L2MPPA6_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA6 */
#define L2MPPA6_UX_BIT_START                                0U
#define L2MPPA6_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA7 */
#define L2MPPA7_RESV_1_BIT_START                            16U
#define L2MPPA7_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA7 */
#define L2MPPA7_AID5_BIT_START                              15U
#define L2MPPA7_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA7 */
#define L2MPPA7_AID4_BIT_START                              14U
#define L2MPPA7_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA7 */
#define L2MPPA7_AID3_BIT_START                              13U
#define L2MPPA7_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA7 */
#define L2MPPA7_AID2_BIT_START                              12U
#define L2MPPA7_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA7 */
#define L2MPPA7_AID1_BIT_START                              11U
#define L2MPPA7_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA7 */
#define L2MPPA7_AID0_BIT_START                              10U
#define L2MPPA7_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA7 */
#define L2MPPA7_AIDX_BIT_START                              9U
#define L2MPPA7_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA7 */
#define L2MPPA7_LOCAL_BIT_START                             8U
#define L2MPPA7_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA7 */
#define L2MPPA7_NS_BIT_START                                7U
#define L2MPPA7_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA7 */
#define L2MPPA7_EMU_BIT_START                               6U
#define L2MPPA7_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA7 */
#define L2MPPA7_SR_BIT_START                                5U
#define L2MPPA7_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA7 */
#define L2MPPA7_SW_BIT_START                                4U
#define L2MPPA7_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA7 */
#define L2MPPA7_SX_BIT_START                                3U
#define L2MPPA7_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA7 */
#define L2MPPA7_UR_BIT_START                                2U
#define L2MPPA7_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA7 */
#define L2MPPA7_UW_BIT_START                                1U
#define L2MPPA7_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA7 */
#define L2MPPA7_UX_BIT_START                                0U
#define L2MPPA7_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA8 */
#define L2MPPA8_RESV_1_BIT_START                            16U
#define L2MPPA8_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA8 */
#define L2MPPA8_AID5_BIT_START                              15U
#define L2MPPA8_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA8 */
#define L2MPPA8_AID4_BIT_START                              14U
#define L2MPPA8_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA8 */
#define L2MPPA8_AID3_BIT_START                              13U
#define L2MPPA8_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA8 */
#define L2MPPA8_AID2_BIT_START                              12U
#define L2MPPA8_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA8 */
#define L2MPPA8_AID1_BIT_START                              11U
#define L2MPPA8_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA8 */
#define L2MPPA8_AID0_BIT_START                              10U
#define L2MPPA8_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA8 */
#define L2MPPA8_AIDX_BIT_START                              9U
#define L2MPPA8_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA8 */
#define L2MPPA8_LOCAL_BIT_START                             8U
#define L2MPPA8_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA8 */
#define L2MPPA8_NS_BIT_START                                7U
#define L2MPPA8_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA8 */
#define L2MPPA8_EMU_BIT_START                               6U
#define L2MPPA8_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA8 */
#define L2MPPA8_SR_BIT_START                                5U
#define L2MPPA8_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA8 */
#define L2MPPA8_SW_BIT_START                                4U
#define L2MPPA8_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA8 */
#define L2MPPA8_SX_BIT_START                                3U
#define L2MPPA8_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA8 */
#define L2MPPA8_UR_BIT_START                                2U
#define L2MPPA8_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA8 */
#define L2MPPA8_UW_BIT_START                                1U
#define L2MPPA8_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA8 */
#define L2MPPA8_UX_BIT_START                                0U
#define L2MPPA8_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA9 */
#define L2MPPA9_RESV_1_BIT_START                            16U
#define L2MPPA9_RESV_1_BIT_END                              31U

/* Definition for field AID5 in Register L2MPPA9 */
#define L2MPPA9_AID5_BIT_START                              15U
#define L2MPPA9_AID5_BIT_END                                15U

/* Definition for field AID4 in Register L2MPPA9 */
#define L2MPPA9_AID4_BIT_START                              14U
#define L2MPPA9_AID4_BIT_END                                14U

/* Definition for field AID3 in Register L2MPPA9 */
#define L2MPPA9_AID3_BIT_START                              13U
#define L2MPPA9_AID3_BIT_END                                13U

/* Definition for field AID2 in Register L2MPPA9 */
#define L2MPPA9_AID2_BIT_START                              12U
#define L2MPPA9_AID2_BIT_END                                12U

/* Definition for field AID1 in Register L2MPPA9 */
#define L2MPPA9_AID1_BIT_START                              11U
#define L2MPPA9_AID1_BIT_END                                11U

/* Definition for field AID0 in Register L2MPPA9 */
#define L2MPPA9_AID0_BIT_START                              10U
#define L2MPPA9_AID0_BIT_END                                10U

/* Definition for field AIDX in Register L2MPPA9 */
#define L2MPPA9_AIDX_BIT_START                              9U
#define L2MPPA9_AIDX_BIT_END                                9U

/* Definition for field LOCAL in Register L2MPPA9 */
#define L2MPPA9_LOCAL_BIT_START                             8U
#define L2MPPA9_LOCAL_BIT_END                               8U

/* Definition for field NS in Register L2MPPA9 */
#define L2MPPA9_NS_BIT_START                                7U
#define L2MPPA9_NS_BIT_END                                  7U

/* Definition for field EMU in Register L2MPPA9 */
#define L2MPPA9_EMU_BIT_START                               6U
#define L2MPPA9_EMU_BIT_END                                 6U

/* Definition for field SR in Register L2MPPA9 */
#define L2MPPA9_SR_BIT_START                                5U
#define L2MPPA9_SR_BIT_END                                  5U

/* Definition for field SW in Register L2MPPA9 */
#define L2MPPA9_SW_BIT_START                                4U
#define L2MPPA9_SW_BIT_END                                  4U

/* Definition for field SX in Register L2MPPA9 */
#define L2MPPA9_SX_BIT_START                                3U
#define L2MPPA9_SX_BIT_END                                  3U

/* Definition for field UR in Register L2MPPA9 */
#define L2MPPA9_UR_BIT_START                                2U
#define L2MPPA9_UR_BIT_END                                  2U

/* Definition for field UW in Register L2MPPA9 */
#define L2MPPA9_UW_BIT_START                                1U
#define L2MPPA9_UW_BIT_END                                  1U

/* Definition for field UX in Register L2MPPA9 */
#define L2MPPA9_UX_BIT_START                                0U
#define L2MPPA9_UX_BIT_END                                  0U

/* Definition for field RESV_1 in Register L2MPPA10 */
#define L2MPPA10_RESV_1_BIT_START                           16U
#define L2MPPA10_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA10 */
#define L2MPPA10_AID5_BIT_START                             15U
#define L2MPPA10_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA10 */
#define L2MPPA10_AID4_BIT_START                             14U
#define L2MPPA10_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA10 */
#define L2MPPA10_AID3_BIT_START                             13U
#define L2MPPA10_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA10 */
#define L2MPPA10_AID2_BIT_START                             12U
#define L2MPPA10_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA10 */
#define L2MPPA10_AID1_BIT_START                             11U
#define L2MPPA10_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA10 */
#define L2MPPA10_AID0_BIT_START                             10U
#define L2MPPA10_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA10 */
#define L2MPPA10_AIDX_BIT_START                             9U
#define L2MPPA10_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA10 */
#define L2MPPA10_LOCAL_BIT_START                            8U
#define L2MPPA10_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA10 */
#define L2MPPA10_NS_BIT_START                               7U
#define L2MPPA10_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA10 */
#define L2MPPA10_EMU_BIT_START                              6U
#define L2MPPA10_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA10 */
#define L2MPPA10_SR_BIT_START                               5U
#define L2MPPA10_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA10 */
#define L2MPPA10_SW_BIT_START                               4U
#define L2MPPA10_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA10 */
#define L2MPPA10_SX_BIT_START                               3U
#define L2MPPA10_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA10 */
#define L2MPPA10_UR_BIT_START                               2U
#define L2MPPA10_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA10 */
#define L2MPPA10_UW_BIT_START                               1U
#define L2MPPA10_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA10 */
#define L2MPPA10_UX_BIT_START                               0U
#define L2MPPA10_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA11 */
#define L2MPPA11_RESV_1_BIT_START                           16U
#define L2MPPA11_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA11 */
#define L2MPPA11_AID5_BIT_START                             15U
#define L2MPPA11_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA11 */
#define L2MPPA11_AID4_BIT_START                             14U
#define L2MPPA11_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA11 */
#define L2MPPA11_AID3_BIT_START                             13U
#define L2MPPA11_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA11 */
#define L2MPPA11_AID2_BIT_START                             12U
#define L2MPPA11_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA11 */
#define L2MPPA11_AID1_BIT_START                             11U
#define L2MPPA11_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA11 */
#define L2MPPA11_AID0_BIT_START                             10U
#define L2MPPA11_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA11 */
#define L2MPPA11_AIDX_BIT_START                             9U
#define L2MPPA11_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA11 */
#define L2MPPA11_LOCAL_BIT_START                            8U
#define L2MPPA11_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA11 */
#define L2MPPA11_NS_BIT_START                               7U
#define L2MPPA11_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA11 */
#define L2MPPA11_EMU_BIT_START                              6U
#define L2MPPA11_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA11 */
#define L2MPPA11_SR_BIT_START                               5U
#define L2MPPA11_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA11 */
#define L2MPPA11_SW_BIT_START                               4U
#define L2MPPA11_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA11 */
#define L2MPPA11_SX_BIT_START                               3U
#define L2MPPA11_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA11 */
#define L2MPPA11_UR_BIT_START                               2U
#define L2MPPA11_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA11 */
#define L2MPPA11_UW_BIT_START                               1U
#define L2MPPA11_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA11 */
#define L2MPPA11_UX_BIT_START                               0U
#define L2MPPA11_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA12 */
#define L2MPPA12_RESV_1_BIT_START                           16U
#define L2MPPA12_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA12 */
#define L2MPPA12_AID5_BIT_START                             15U
#define L2MPPA12_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA12 */
#define L2MPPA12_AID4_BIT_START                             14U
#define L2MPPA12_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA12 */
#define L2MPPA12_AID3_BIT_START                             13U
#define L2MPPA12_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA12 */
#define L2MPPA12_AID2_BIT_START                             12U
#define L2MPPA12_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA12 */
#define L2MPPA12_AID1_BIT_START                             11U
#define L2MPPA12_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA12 */
#define L2MPPA12_AID0_BIT_START                             10U
#define L2MPPA12_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA12 */
#define L2MPPA12_AIDX_BIT_START                             9U
#define L2MPPA12_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA12 */
#define L2MPPA12_LOCAL_BIT_START                            8U
#define L2MPPA12_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA12 */
#define L2MPPA12_NS_BIT_START                               7U
#define L2MPPA12_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA12 */
#define L2MPPA12_EMU_BIT_START                              6U
#define L2MPPA12_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA12 */
#define L2MPPA12_SR_BIT_START                               5U
#define L2MPPA12_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA12 */
#define L2MPPA12_SW_BIT_START                               4U
#define L2MPPA12_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA12 */
#define L2MPPA12_SX_BIT_START                               3U
#define L2MPPA12_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA12 */
#define L2MPPA12_UR_BIT_START                               2U
#define L2MPPA12_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA12 */
#define L2MPPA12_UW_BIT_START                               1U
#define L2MPPA12_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA12 */
#define L2MPPA12_UX_BIT_START                               0U
#define L2MPPA12_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA13 */
#define L2MPPA13_RESV_1_BIT_START                           16U
#define L2MPPA13_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA13 */
#define L2MPPA13_AID5_BIT_START                             15U
#define L2MPPA13_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA13 */
#define L2MPPA13_AID4_BIT_START                             14U
#define L2MPPA13_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA13 */
#define L2MPPA13_AID3_BIT_START                             13U
#define L2MPPA13_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA13 */
#define L2MPPA13_AID2_BIT_START                             12U
#define L2MPPA13_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA13 */
#define L2MPPA13_AID1_BIT_START                             11U
#define L2MPPA13_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA13 */
#define L2MPPA13_AID0_BIT_START                             10U
#define L2MPPA13_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA13 */
#define L2MPPA13_AIDX_BIT_START                             9U
#define L2MPPA13_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA13 */
#define L2MPPA13_LOCAL_BIT_START                            8U
#define L2MPPA13_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA13 */
#define L2MPPA13_NS_BIT_START                               7U
#define L2MPPA13_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA13 */
#define L2MPPA13_EMU_BIT_START                              6U
#define L2MPPA13_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA13 */
#define L2MPPA13_SR_BIT_START                               5U
#define L2MPPA13_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA13 */
#define L2MPPA13_SW_BIT_START                               4U
#define L2MPPA13_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA13 */
#define L2MPPA13_SX_BIT_START                               3U
#define L2MPPA13_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA13 */
#define L2MPPA13_UR_BIT_START                               2U
#define L2MPPA13_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA13 */
#define L2MPPA13_UW_BIT_START                               1U
#define L2MPPA13_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA13 */
#define L2MPPA13_UX_BIT_START                               0U
#define L2MPPA13_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA14 */
#define L2MPPA14_RESV_1_BIT_START                           16U
#define L2MPPA14_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA14 */
#define L2MPPA14_AID5_BIT_START                             15U
#define L2MPPA14_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA14 */
#define L2MPPA14_AID4_BIT_START                             14U
#define L2MPPA14_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA14 */
#define L2MPPA14_AID3_BIT_START                             13U
#define L2MPPA14_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA14 */
#define L2MPPA14_AID2_BIT_START                             12U
#define L2MPPA14_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA14 */
#define L2MPPA14_AID1_BIT_START                             11U
#define L2MPPA14_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA14 */
#define L2MPPA14_AID0_BIT_START                             10U
#define L2MPPA14_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA14 */
#define L2MPPA14_AIDX_BIT_START                             9U
#define L2MPPA14_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA14 */
#define L2MPPA14_LOCAL_BIT_START                            8U
#define L2MPPA14_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA14 */
#define L2MPPA14_NS_BIT_START                               7U
#define L2MPPA14_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA14 */
#define L2MPPA14_EMU_BIT_START                              6U
#define L2MPPA14_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA14 */
#define L2MPPA14_SR_BIT_START                               5U
#define L2MPPA14_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA14 */
#define L2MPPA14_SW_BIT_START                               4U
#define L2MPPA14_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA14 */
#define L2MPPA14_SX_BIT_START                               3U
#define L2MPPA14_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA14 */
#define L2MPPA14_UR_BIT_START                               2U
#define L2MPPA14_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA14 */
#define L2MPPA14_UW_BIT_START                               1U
#define L2MPPA14_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA14 */
#define L2MPPA14_UX_BIT_START                               0U
#define L2MPPA14_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA15 */
#define L2MPPA15_RESV_1_BIT_START                           16U
#define L2MPPA15_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA15 */
#define L2MPPA15_AID5_BIT_START                             15U
#define L2MPPA15_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA15 */
#define L2MPPA15_AID4_BIT_START                             14U
#define L2MPPA15_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA15 */
#define L2MPPA15_AID3_BIT_START                             13U
#define L2MPPA15_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA15 */
#define L2MPPA15_AID2_BIT_START                             12U
#define L2MPPA15_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA15 */
#define L2MPPA15_AID1_BIT_START                             11U
#define L2MPPA15_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA15 */
#define L2MPPA15_AID0_BIT_START                             10U
#define L2MPPA15_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA15 */
#define L2MPPA15_AIDX_BIT_START                             9U
#define L2MPPA15_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA15 */
#define L2MPPA15_LOCAL_BIT_START                            8U
#define L2MPPA15_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA15 */
#define L2MPPA15_NS_BIT_START                               7U
#define L2MPPA15_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA15 */
#define L2MPPA15_EMU_BIT_START                              6U
#define L2MPPA15_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA15 */
#define L2MPPA15_SR_BIT_START                               5U
#define L2MPPA15_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA15 */
#define L2MPPA15_SW_BIT_START                               4U
#define L2MPPA15_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA15 */
#define L2MPPA15_SX_BIT_START                               3U
#define L2MPPA15_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA15 */
#define L2MPPA15_UR_BIT_START                               2U
#define L2MPPA15_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA15 */
#define L2MPPA15_UW_BIT_START                               1U
#define L2MPPA15_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA15 */
#define L2MPPA15_UX_BIT_START                               0U
#define L2MPPA15_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA16 */
#define L2MPPA16_RESV_1_BIT_START                           16U
#define L2MPPA16_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA16 */
#define L2MPPA16_AID5_BIT_START                             15U
#define L2MPPA16_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA16 */
#define L2MPPA16_AID4_BIT_START                             14U
#define L2MPPA16_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA16 */
#define L2MPPA16_AID3_BIT_START                             13U
#define L2MPPA16_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA16 */
#define L2MPPA16_AID2_BIT_START                             12U
#define L2MPPA16_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA16 */
#define L2MPPA16_AID1_BIT_START                             11U
#define L2MPPA16_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA16 */
#define L2MPPA16_AID0_BIT_START                             10U
#define L2MPPA16_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA16 */
#define L2MPPA16_AIDX_BIT_START                             9U
#define L2MPPA16_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA16 */
#define L2MPPA16_LOCAL_BIT_START                            8U
#define L2MPPA16_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA16 */
#define L2MPPA16_NS_BIT_START                               7U
#define L2MPPA16_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA16 */
#define L2MPPA16_EMU_BIT_START                              6U
#define L2MPPA16_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA16 */
#define L2MPPA16_SR_BIT_START                               5U
#define L2MPPA16_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA16 */
#define L2MPPA16_SW_BIT_START                               4U
#define L2MPPA16_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA16 */
#define L2MPPA16_SX_BIT_START                               3U
#define L2MPPA16_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA16 */
#define L2MPPA16_UR_BIT_START                               2U
#define L2MPPA16_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA16 */
#define L2MPPA16_UW_BIT_START                               1U
#define L2MPPA16_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA16 */
#define L2MPPA16_UX_BIT_START                               0U
#define L2MPPA16_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA17 */
#define L2MPPA17_RESV_1_BIT_START                           16U
#define L2MPPA17_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA17 */
#define L2MPPA17_AID5_BIT_START                             15U
#define L2MPPA17_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA17 */
#define L2MPPA17_AID4_BIT_START                             14U
#define L2MPPA17_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA17 */
#define L2MPPA17_AID3_BIT_START                             13U
#define L2MPPA17_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA17 */
#define L2MPPA17_AID2_BIT_START                             12U
#define L2MPPA17_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA17 */
#define L2MPPA17_AID1_BIT_START                             11U
#define L2MPPA17_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA17 */
#define L2MPPA17_AID0_BIT_START                             10U
#define L2MPPA17_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA17 */
#define L2MPPA17_AIDX_BIT_START                             9U
#define L2MPPA17_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA17 */
#define L2MPPA17_LOCAL_BIT_START                            8U
#define L2MPPA17_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA17 */
#define L2MPPA17_NS_BIT_START                               7U
#define L2MPPA17_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA17 */
#define L2MPPA17_EMU_BIT_START                              6U
#define L2MPPA17_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA17 */
#define L2MPPA17_SR_BIT_START                               5U
#define L2MPPA17_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA17 */
#define L2MPPA17_SW_BIT_START                               4U
#define L2MPPA17_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA17 */
#define L2MPPA17_SX_BIT_START                               3U
#define L2MPPA17_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA17 */
#define L2MPPA17_UR_BIT_START                               2U
#define L2MPPA17_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA17 */
#define L2MPPA17_UW_BIT_START                               1U
#define L2MPPA17_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA17 */
#define L2MPPA17_UX_BIT_START                               0U
#define L2MPPA17_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA18 */
#define L2MPPA18_RESV_1_BIT_START                           16U
#define L2MPPA18_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA18 */
#define L2MPPA18_AID5_BIT_START                             15U
#define L2MPPA18_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA18 */
#define L2MPPA18_AID4_BIT_START                             14U
#define L2MPPA18_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA18 */
#define L2MPPA18_AID3_BIT_START                             13U
#define L2MPPA18_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA18 */
#define L2MPPA18_AID2_BIT_START                             12U
#define L2MPPA18_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA18 */
#define L2MPPA18_AID1_BIT_START                             11U
#define L2MPPA18_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA18 */
#define L2MPPA18_AID0_BIT_START                             10U
#define L2MPPA18_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA18 */
#define L2MPPA18_AIDX_BIT_START                             9U
#define L2MPPA18_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA18 */
#define L2MPPA18_LOCAL_BIT_START                            8U
#define L2MPPA18_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA18 */
#define L2MPPA18_NS_BIT_START                               7U
#define L2MPPA18_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA18 */
#define L2MPPA18_EMU_BIT_START                              6U
#define L2MPPA18_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA18 */
#define L2MPPA18_SR_BIT_START                               5U
#define L2MPPA18_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA18 */
#define L2MPPA18_SW_BIT_START                               4U
#define L2MPPA18_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA18 */
#define L2MPPA18_SX_BIT_START                               3U
#define L2MPPA18_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA18 */
#define L2MPPA18_UR_BIT_START                               2U
#define L2MPPA18_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA18 */
#define L2MPPA18_UW_BIT_START                               1U
#define L2MPPA18_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA18 */
#define L2MPPA18_UX_BIT_START                               0U
#define L2MPPA18_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA19 */
#define L2MPPA19_RESV_1_BIT_START                           16U
#define L2MPPA19_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA19 */
#define L2MPPA19_AID5_BIT_START                             15U
#define L2MPPA19_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA19 */
#define L2MPPA19_AID4_BIT_START                             14U
#define L2MPPA19_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA19 */
#define L2MPPA19_AID3_BIT_START                             13U
#define L2MPPA19_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA19 */
#define L2MPPA19_AID2_BIT_START                             12U
#define L2MPPA19_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA19 */
#define L2MPPA19_AID1_BIT_START                             11U
#define L2MPPA19_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA19 */
#define L2MPPA19_AID0_BIT_START                             10U
#define L2MPPA19_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA19 */
#define L2MPPA19_AIDX_BIT_START                             9U
#define L2MPPA19_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA19 */
#define L2MPPA19_LOCAL_BIT_START                            8U
#define L2MPPA19_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA19 */
#define L2MPPA19_NS_BIT_START                               7U
#define L2MPPA19_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA19 */
#define L2MPPA19_EMU_BIT_START                              6U
#define L2MPPA19_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA19 */
#define L2MPPA19_SR_BIT_START                               5U
#define L2MPPA19_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA19 */
#define L2MPPA19_SW_BIT_START                               4U
#define L2MPPA19_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA19 */
#define L2MPPA19_SX_BIT_START                               3U
#define L2MPPA19_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA19 */
#define L2MPPA19_UR_BIT_START                               2U
#define L2MPPA19_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA19 */
#define L2MPPA19_UW_BIT_START                               1U
#define L2MPPA19_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA19 */
#define L2MPPA19_UX_BIT_START                               0U
#define L2MPPA19_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA20 */
#define L2MPPA20_RESV_1_BIT_START                           16U
#define L2MPPA20_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA20 */
#define L2MPPA20_AID5_BIT_START                             15U
#define L2MPPA20_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA20 */
#define L2MPPA20_AID4_BIT_START                             14U
#define L2MPPA20_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA20 */
#define L2MPPA20_AID3_BIT_START                             13U
#define L2MPPA20_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA20 */
#define L2MPPA20_AID2_BIT_START                             12U
#define L2MPPA20_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA20 */
#define L2MPPA20_AID1_BIT_START                             11U
#define L2MPPA20_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA20 */
#define L2MPPA20_AID0_BIT_START                             10U
#define L2MPPA20_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA20 */
#define L2MPPA20_AIDX_BIT_START                             9U
#define L2MPPA20_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA20 */
#define L2MPPA20_LOCAL_BIT_START                            8U
#define L2MPPA20_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA20 */
#define L2MPPA20_NS_BIT_START                               7U
#define L2MPPA20_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA20 */
#define L2MPPA20_EMU_BIT_START                              6U
#define L2MPPA20_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA20 */
#define L2MPPA20_SR_BIT_START                               5U
#define L2MPPA20_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA20 */
#define L2MPPA20_SW_BIT_START                               4U
#define L2MPPA20_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA20 */
#define L2MPPA20_SX_BIT_START                               3U
#define L2MPPA20_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA20 */
#define L2MPPA20_UR_BIT_START                               2U
#define L2MPPA20_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA20 */
#define L2MPPA20_UW_BIT_START                               1U
#define L2MPPA20_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA20 */
#define L2MPPA20_UX_BIT_START                               0U
#define L2MPPA20_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA21 */
#define L2MPPA21_RESV_1_BIT_START                           16U
#define L2MPPA21_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA21 */
#define L2MPPA21_AID5_BIT_START                             15U
#define L2MPPA21_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA21 */
#define L2MPPA21_AID4_BIT_START                             14U
#define L2MPPA21_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA21 */
#define L2MPPA21_AID3_BIT_START                             13U
#define L2MPPA21_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA21 */
#define L2MPPA21_AID2_BIT_START                             12U
#define L2MPPA21_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA21 */
#define L2MPPA21_AID1_BIT_START                             11U
#define L2MPPA21_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA21 */
#define L2MPPA21_AID0_BIT_START                             10U
#define L2MPPA21_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA21 */
#define L2MPPA21_AIDX_BIT_START                             9U
#define L2MPPA21_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA21 */
#define L2MPPA21_LOCAL_BIT_START                            8U
#define L2MPPA21_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA21 */
#define L2MPPA21_NS_BIT_START                               7U
#define L2MPPA21_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA21 */
#define L2MPPA21_EMU_BIT_START                              6U
#define L2MPPA21_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA21 */
#define L2MPPA21_SR_BIT_START                               5U
#define L2MPPA21_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA21 */
#define L2MPPA21_SW_BIT_START                               4U
#define L2MPPA21_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA21 */
#define L2MPPA21_SX_BIT_START                               3U
#define L2MPPA21_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA21 */
#define L2MPPA21_UR_BIT_START                               2U
#define L2MPPA21_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA21 */
#define L2MPPA21_UW_BIT_START                               1U
#define L2MPPA21_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA21 */
#define L2MPPA21_UX_BIT_START                               0U
#define L2MPPA21_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA22 */
#define L2MPPA22_RESV_1_BIT_START                           16U
#define L2MPPA22_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA22 */
#define L2MPPA22_AID5_BIT_START                             15U
#define L2MPPA22_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA22 */
#define L2MPPA22_AID4_BIT_START                             14U
#define L2MPPA22_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA22 */
#define L2MPPA22_AID3_BIT_START                             13U
#define L2MPPA22_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA22 */
#define L2MPPA22_AID2_BIT_START                             12U
#define L2MPPA22_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA22 */
#define L2MPPA22_AID1_BIT_START                             11U
#define L2MPPA22_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA22 */
#define L2MPPA22_AID0_BIT_START                             10U
#define L2MPPA22_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA22 */
#define L2MPPA22_AIDX_BIT_START                             9U
#define L2MPPA22_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA22 */
#define L2MPPA22_LOCAL_BIT_START                            8U
#define L2MPPA22_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA22 */
#define L2MPPA22_NS_BIT_START                               7U
#define L2MPPA22_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA22 */
#define L2MPPA22_EMU_BIT_START                              6U
#define L2MPPA22_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA22 */
#define L2MPPA22_SR_BIT_START                               5U
#define L2MPPA22_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA22 */
#define L2MPPA22_SW_BIT_START                               4U
#define L2MPPA22_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA22 */
#define L2MPPA22_SX_BIT_START                               3U
#define L2MPPA22_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA22 */
#define L2MPPA22_UR_BIT_START                               2U
#define L2MPPA22_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA22 */
#define L2MPPA22_UW_BIT_START                               1U
#define L2MPPA22_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA22 */
#define L2MPPA22_UX_BIT_START                               0U
#define L2MPPA22_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA23 */
#define L2MPPA23_RESV_1_BIT_START                           16U
#define L2MPPA23_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA23 */
#define L2MPPA23_AID5_BIT_START                             15U
#define L2MPPA23_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA23 */
#define L2MPPA23_AID4_BIT_START                             14U
#define L2MPPA23_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA23 */
#define L2MPPA23_AID3_BIT_START                             13U
#define L2MPPA23_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA23 */
#define L2MPPA23_AID2_BIT_START                             12U
#define L2MPPA23_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA23 */
#define L2MPPA23_AID1_BIT_START                             11U
#define L2MPPA23_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA23 */
#define L2MPPA23_AID0_BIT_START                             10U
#define L2MPPA23_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA23 */
#define L2MPPA23_AIDX_BIT_START                             9U
#define L2MPPA23_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA23 */
#define L2MPPA23_LOCAL_BIT_START                            8U
#define L2MPPA23_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA23 */
#define L2MPPA23_NS_BIT_START                               7U
#define L2MPPA23_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA23 */
#define L2MPPA23_EMU_BIT_START                              6U
#define L2MPPA23_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA23 */
#define L2MPPA23_SR_BIT_START                               5U
#define L2MPPA23_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA23 */
#define L2MPPA23_SW_BIT_START                               4U
#define L2MPPA23_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA23 */
#define L2MPPA23_SX_BIT_START                               3U
#define L2MPPA23_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA23 */
#define L2MPPA23_UR_BIT_START                               2U
#define L2MPPA23_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA23 */
#define L2MPPA23_UW_BIT_START                               1U
#define L2MPPA23_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA23 */
#define L2MPPA23_UX_BIT_START                               0U
#define L2MPPA23_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA24 */
#define L2MPPA24_RESV_1_BIT_START                           16U
#define L2MPPA24_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA24 */
#define L2MPPA24_AID5_BIT_START                             15U
#define L2MPPA24_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA24 */
#define L2MPPA24_AID4_BIT_START                             14U
#define L2MPPA24_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA24 */
#define L2MPPA24_AID3_BIT_START                             13U
#define L2MPPA24_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA24 */
#define L2MPPA24_AID2_BIT_START                             12U
#define L2MPPA24_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA24 */
#define L2MPPA24_AID1_BIT_START                             11U
#define L2MPPA24_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA24 */
#define L2MPPA24_AID0_BIT_START                             10U
#define L2MPPA24_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA24 */
#define L2MPPA24_AIDX_BIT_START                             9U
#define L2MPPA24_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA24 */
#define L2MPPA24_LOCAL_BIT_START                            8U
#define L2MPPA24_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA24 */
#define L2MPPA24_NS_BIT_START                               7U
#define L2MPPA24_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA24 */
#define L2MPPA24_EMU_BIT_START                              6U
#define L2MPPA24_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA24 */
#define L2MPPA24_SR_BIT_START                               5U
#define L2MPPA24_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA24 */
#define L2MPPA24_SW_BIT_START                               4U
#define L2MPPA24_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA24 */
#define L2MPPA24_SX_BIT_START                               3U
#define L2MPPA24_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA24 */
#define L2MPPA24_UR_BIT_START                               2U
#define L2MPPA24_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA24 */
#define L2MPPA24_UW_BIT_START                               1U
#define L2MPPA24_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA24 */
#define L2MPPA24_UX_BIT_START                               0U
#define L2MPPA24_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA25 */
#define L2MPPA25_RESV_1_BIT_START                           16U
#define L2MPPA25_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA25 */
#define L2MPPA25_AID5_BIT_START                             15U
#define L2MPPA25_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA25 */
#define L2MPPA25_AID4_BIT_START                             14U
#define L2MPPA25_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA25 */
#define L2MPPA25_AID3_BIT_START                             13U
#define L2MPPA25_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA25 */
#define L2MPPA25_AID2_BIT_START                             12U
#define L2MPPA25_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA25 */
#define L2MPPA25_AID1_BIT_START                             11U
#define L2MPPA25_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA25 */
#define L2MPPA25_AID0_BIT_START                             10U
#define L2MPPA25_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA25 */
#define L2MPPA25_AIDX_BIT_START                             9U
#define L2MPPA25_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA25 */
#define L2MPPA25_LOCAL_BIT_START                            8U
#define L2MPPA25_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA25 */
#define L2MPPA25_NS_BIT_START                               7U
#define L2MPPA25_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA25 */
#define L2MPPA25_EMU_BIT_START                              6U
#define L2MPPA25_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA25 */
#define L2MPPA25_SR_BIT_START                               5U
#define L2MPPA25_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA25 */
#define L2MPPA25_SW_BIT_START                               4U
#define L2MPPA25_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA25 */
#define L2MPPA25_SX_BIT_START                               3U
#define L2MPPA25_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA25 */
#define L2MPPA25_UR_BIT_START                               2U
#define L2MPPA25_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA25 */
#define L2MPPA25_UW_BIT_START                               1U
#define L2MPPA25_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA25 */
#define L2MPPA25_UX_BIT_START                               0U
#define L2MPPA25_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA26 */
#define L2MPPA26_RESV_1_BIT_START                           16U
#define L2MPPA26_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA26 */
#define L2MPPA26_AID5_BIT_START                             15U
#define L2MPPA26_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA26 */
#define L2MPPA26_AID4_BIT_START                             14U
#define L2MPPA26_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA26 */
#define L2MPPA26_AID3_BIT_START                             13U
#define L2MPPA26_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA26 */
#define L2MPPA26_AID2_BIT_START                             12U
#define L2MPPA26_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA26 */
#define L2MPPA26_AID1_BIT_START                             11U
#define L2MPPA26_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA26 */
#define L2MPPA26_AID0_BIT_START                             10U
#define L2MPPA26_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA26 */
#define L2MPPA26_AIDX_BIT_START                             9U
#define L2MPPA26_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA26 */
#define L2MPPA26_LOCAL_BIT_START                            8U
#define L2MPPA26_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA26 */
#define L2MPPA26_NS_BIT_START                               7U
#define L2MPPA26_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA26 */
#define L2MPPA26_EMU_BIT_START                              6U
#define L2MPPA26_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA26 */
#define L2MPPA26_SR_BIT_START                               5U
#define L2MPPA26_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA26 */
#define L2MPPA26_SW_BIT_START                               4U
#define L2MPPA26_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA26 */
#define L2MPPA26_SX_BIT_START                               3U
#define L2MPPA26_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA26 */
#define L2MPPA26_UR_BIT_START                               2U
#define L2MPPA26_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA26 */
#define L2MPPA26_UW_BIT_START                               1U
#define L2MPPA26_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA26 */
#define L2MPPA26_UX_BIT_START                               0U
#define L2MPPA26_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA27 */
#define L2MPPA27_RESV_1_BIT_START                           16U
#define L2MPPA27_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA27 */
#define L2MPPA27_AID5_BIT_START                             15U
#define L2MPPA27_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA27 */
#define L2MPPA27_AID4_BIT_START                             14U
#define L2MPPA27_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA27 */
#define L2MPPA27_AID3_BIT_START                             13U
#define L2MPPA27_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA27 */
#define L2MPPA27_AID2_BIT_START                             12U
#define L2MPPA27_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA27 */
#define L2MPPA27_AID1_BIT_START                             11U
#define L2MPPA27_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA27 */
#define L2MPPA27_AID0_BIT_START                             10U
#define L2MPPA27_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA27 */
#define L2MPPA27_AIDX_BIT_START                             9U
#define L2MPPA27_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA27 */
#define L2MPPA27_LOCAL_BIT_START                            8U
#define L2MPPA27_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA27 */
#define L2MPPA27_NS_BIT_START                               7U
#define L2MPPA27_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA27 */
#define L2MPPA27_EMU_BIT_START                              6U
#define L2MPPA27_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA27 */
#define L2MPPA27_SR_BIT_START                               5U
#define L2MPPA27_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA27 */
#define L2MPPA27_SW_BIT_START                               4U
#define L2MPPA27_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA27 */
#define L2MPPA27_SX_BIT_START                               3U
#define L2MPPA27_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA27 */
#define L2MPPA27_UR_BIT_START                               2U
#define L2MPPA27_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA27 */
#define L2MPPA27_UW_BIT_START                               1U
#define L2MPPA27_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA27 */
#define L2MPPA27_UX_BIT_START                               0U
#define L2MPPA27_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA28 */
#define L2MPPA28_RESV_1_BIT_START                           16U
#define L2MPPA28_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA28 */
#define L2MPPA28_AID5_BIT_START                             15U
#define L2MPPA28_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA28 */
#define L2MPPA28_AID4_BIT_START                             14U
#define L2MPPA28_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA28 */
#define L2MPPA28_AID3_BIT_START                             13U
#define L2MPPA28_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA28 */
#define L2MPPA28_AID2_BIT_START                             12U
#define L2MPPA28_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA28 */
#define L2MPPA28_AID1_BIT_START                             11U
#define L2MPPA28_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA28 */
#define L2MPPA28_AID0_BIT_START                             10U
#define L2MPPA28_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA28 */
#define L2MPPA28_AIDX_BIT_START                             9U
#define L2MPPA28_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA28 */
#define L2MPPA28_LOCAL_BIT_START                            8U
#define L2MPPA28_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA28 */
#define L2MPPA28_NS_BIT_START                               7U
#define L2MPPA28_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA28 */
#define L2MPPA28_EMU_BIT_START                              6U
#define L2MPPA28_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA28 */
#define L2MPPA28_SR_BIT_START                               5U
#define L2MPPA28_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA28 */
#define L2MPPA28_SW_BIT_START                               4U
#define L2MPPA28_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA28 */
#define L2MPPA28_SX_BIT_START                               3U
#define L2MPPA28_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA28 */
#define L2MPPA28_UR_BIT_START                               2U
#define L2MPPA28_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA28 */
#define L2MPPA28_UW_BIT_START                               1U
#define L2MPPA28_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA28 */
#define L2MPPA28_UX_BIT_START                               0U
#define L2MPPA28_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA29 */
#define L2MPPA29_RESV_1_BIT_START                           16U
#define L2MPPA29_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA29 */
#define L2MPPA29_AID5_BIT_START                             15U
#define L2MPPA29_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA29 */
#define L2MPPA29_AID4_BIT_START                             14U
#define L2MPPA29_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA29 */
#define L2MPPA29_AID3_BIT_START                             13U
#define L2MPPA29_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA29 */
#define L2MPPA29_AID2_BIT_START                             12U
#define L2MPPA29_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA29 */
#define L2MPPA29_AID1_BIT_START                             11U
#define L2MPPA29_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA29 */
#define L2MPPA29_AID0_BIT_START                             10U
#define L2MPPA29_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA29 */
#define L2MPPA29_AIDX_BIT_START                             9U
#define L2MPPA29_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA29 */
#define L2MPPA29_LOCAL_BIT_START                            8U
#define L2MPPA29_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA29 */
#define L2MPPA29_NS_BIT_START                               7U
#define L2MPPA29_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA29 */
#define L2MPPA29_EMU_BIT_START                              6U
#define L2MPPA29_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA29 */
#define L2MPPA29_SR_BIT_START                               5U
#define L2MPPA29_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA29 */
#define L2MPPA29_SW_BIT_START                               4U
#define L2MPPA29_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA29 */
#define L2MPPA29_SX_BIT_START                               3U
#define L2MPPA29_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA29 */
#define L2MPPA29_UR_BIT_START                               2U
#define L2MPPA29_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA29 */
#define L2MPPA29_UW_BIT_START                               1U
#define L2MPPA29_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA29 */
#define L2MPPA29_UX_BIT_START                               0U
#define L2MPPA29_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA30 */
#define L2MPPA30_RESV_1_BIT_START                           16U
#define L2MPPA30_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA30 */
#define L2MPPA30_AID5_BIT_START                             15U
#define L2MPPA30_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA30 */
#define L2MPPA30_AID4_BIT_START                             14U
#define L2MPPA30_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA30 */
#define L2MPPA30_AID3_BIT_START                             13U
#define L2MPPA30_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA30 */
#define L2MPPA30_AID2_BIT_START                             12U
#define L2MPPA30_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA30 */
#define L2MPPA30_AID1_BIT_START                             11U
#define L2MPPA30_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA30 */
#define L2MPPA30_AID0_BIT_START                             10U
#define L2MPPA30_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA30 */
#define L2MPPA30_AIDX_BIT_START                             9U
#define L2MPPA30_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA30 */
#define L2MPPA30_LOCAL_BIT_START                            8U
#define L2MPPA30_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA30 */
#define L2MPPA30_NS_BIT_START                               7U
#define L2MPPA30_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA30 */
#define L2MPPA30_EMU_BIT_START                              6U
#define L2MPPA30_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA30 */
#define L2MPPA30_SR_BIT_START                               5U
#define L2MPPA30_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA30 */
#define L2MPPA30_SW_BIT_START                               4U
#define L2MPPA30_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA30 */
#define L2MPPA30_SX_BIT_START                               3U
#define L2MPPA30_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA30 */
#define L2MPPA30_UR_BIT_START                               2U
#define L2MPPA30_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA30 */
#define L2MPPA30_UW_BIT_START                               1U
#define L2MPPA30_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA30 */
#define L2MPPA30_UX_BIT_START                               0U
#define L2MPPA30_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L2MPPA31 */
#define L2MPPA31_RESV_1_BIT_START                           16U
#define L2MPPA31_RESV_1_BIT_END                             31U

/* Definition for field AID5 in Register L2MPPA31 */
#define L2MPPA31_AID5_BIT_START                             15U
#define L2MPPA31_AID5_BIT_END                               15U

/* Definition for field AID4 in Register L2MPPA31 */
#define L2MPPA31_AID4_BIT_START                             14U
#define L2MPPA31_AID4_BIT_END                               14U

/* Definition for field AID3 in Register L2MPPA31 */
#define L2MPPA31_AID3_BIT_START                             13U
#define L2MPPA31_AID3_BIT_END                               13U

/* Definition for field AID2 in Register L2MPPA31 */
#define L2MPPA31_AID2_BIT_START                             12U
#define L2MPPA31_AID2_BIT_END                               12U

/* Definition for field AID1 in Register L2MPPA31 */
#define L2MPPA31_AID1_BIT_START                             11U
#define L2MPPA31_AID1_BIT_END                               11U

/* Definition for field AID0 in Register L2MPPA31 */
#define L2MPPA31_AID0_BIT_START                             10U
#define L2MPPA31_AID0_BIT_END                               10U

/* Definition for field AIDX in Register L2MPPA31 */
#define L2MPPA31_AIDX_BIT_START                             9U
#define L2MPPA31_AIDX_BIT_END                               9U

/* Definition for field LOCAL in Register L2MPPA31 */
#define L2MPPA31_LOCAL_BIT_START                            8U
#define L2MPPA31_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L2MPPA31 */
#define L2MPPA31_NS_BIT_START                               7U
#define L2MPPA31_NS_BIT_END                                 7U

/* Definition for field EMU in Register L2MPPA31 */
#define L2MPPA31_EMU_BIT_START                              6U
#define L2MPPA31_EMU_BIT_END                                6U

/* Definition for field SR in Register L2MPPA31 */
#define L2MPPA31_SR_BIT_START                               5U
#define L2MPPA31_SR_BIT_END                                 5U

/* Definition for field SW in Register L2MPPA31 */
#define L2MPPA31_SW_BIT_START                               4U
#define L2MPPA31_SW_BIT_END                                 4U

/* Definition for field SX in Register L2MPPA31 */
#define L2MPPA31_SX_BIT_START                               3U
#define L2MPPA31_SX_BIT_END                                 3U

/* Definition for field UR in Register L2MPPA31 */
#define L2MPPA31_UR_BIT_START                               2U
#define L2MPPA31_UR_BIT_END                                 2U

/* Definition for field UW in Register L2MPPA31 */
#define L2MPPA31_UW_BIT_START                               1U
#define L2MPPA31_UW_BIT_END                                 1U

/* Definition for field UX in Register L2MPPA31 */
#define L2MPPA31_UX_BIT_START                               0U
#define L2MPPA31_UX_BIT_END                                 0U

/* Definition for field ADDR in Register L1PMPFAR */
#define L1PMPFAR_ADDR_BIT_START                             0U
#define L1PMPFAR_ADDR_BIT_END                               31U

/* Definition for field RESV_1 in Register L1PMPFSR */
#define L1PMPFSR_RESV_1_BIT_START                           16U
#define L1PMPFSR_RESV_1_BIT_END                             31U

/* Definition for field FID in Register L1PMPFSR */
#define L1PMPFSR_FID_BIT_START                              9U
#define L1PMPFSR_FID_BIT_END                                15U

/* Definition for field LOCAL in Register L1PMPFSR */
#define L1PMPFSR_LOCAL_BIT_START                            8U
#define L1PMPFSR_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L1PMPFSR */
#define L1PMPFSR_NS_BIT_START                               7U
#define L1PMPFSR_NS_BIT_END                                 7U

/* Definition for field RESV_2 in Register L1PMPFSR */
#define L1PMPFSR_RESV_2_BIT_START                           6U
#define L1PMPFSR_RESV_2_BIT_END                             6U

/* Definition for field SR in Register L1PMPFSR */
#define L1PMPFSR_SR_BIT_START                               5U
#define L1PMPFSR_SR_BIT_END                                 5U

/* Definition for field SW in Register L1PMPFSR */
#define L1PMPFSR_SW_BIT_START                               4U
#define L1PMPFSR_SW_BIT_END                                 4U

/* Definition for field SX in Register L1PMPFSR */
#define L1PMPFSR_SX_BIT_START                               3U
#define L1PMPFSR_SX_BIT_END                                 3U

/* Definition for field UR in Register L1PMPFSR */
#define L1PMPFSR_UR_BIT_START                               2U
#define L1PMPFSR_UR_BIT_END                                 2U

/* Definition for field UW in Register L1PMPFSR */
#define L1PMPFSR_UW_BIT_START                               1U
#define L1PMPFSR_UW_BIT_END                                 1U

/* Definition for field UX in Register L1PMPFSR */
#define L1PMPFSR_UX_BIT_START                               0U
#define L1PMPFSR_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L1PMPFCR */
#define L1PMPFCR_RESV_1_BIT_START                           1U
#define L1PMPFCR_RESV_1_BIT_END                             31U

/* Definition for field MPFCLR in Register L1PMPFCR */
#define L1PMPFCR_MPFCLR_BIT_START                           0U
#define L1PMPFCR_MPFCLR_BIT_END                             0U

/* Definition for field RESV_1 in Register L1PMPPA16 */
#define L1PMPPA16_RESV_1_BIT_START                          16U
#define L1PMPPA16_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA16 */
#define L1PMPPA16_AID5_BIT_START                            15U
#define L1PMPPA16_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA16 */
#define L1PMPPA16_AID4_BIT_START                            14U
#define L1PMPPA16_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA16 */
#define L1PMPPA16_AID3_BIT_START                            13U
#define L1PMPPA16_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA16 */
#define L1PMPPA16_AID2_BIT_START                            12U
#define L1PMPPA16_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA16 */
#define L1PMPPA16_AID1_BIT_START                            11U
#define L1PMPPA16_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA16 */
#define L1PMPPA16_AID0_BIT_START                            10U
#define L1PMPPA16_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA16 */
#define L1PMPPA16_AIDX_BIT_START                            9U
#define L1PMPPA16_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA16 */
#define L1PMPPA16_LOCAL_BIT_START                           8U
#define L1PMPPA16_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA16 */
#define L1PMPPA16_NS_BIT_START                              7U
#define L1PMPPA16_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA16 */
#define L1PMPPA16_EMU_BIT_START                             6U
#define L1PMPPA16_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA16 */
#define L1PMPPA16_SR_BIT_START                              5U
#define L1PMPPA16_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA16 */
#define L1PMPPA16_SW_BIT_START                              4U
#define L1PMPPA16_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA16 */
#define L1PMPPA16_SX_BIT_START                              3U
#define L1PMPPA16_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA16 */
#define L1PMPPA16_UR_BIT_START                              2U
#define L1PMPPA16_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA16 */
#define L1PMPPA16_UW_BIT_START                              1U
#define L1PMPPA16_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA16 */
#define L1PMPPA16_UX_BIT_START                              0U
#define L1PMPPA16_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA17 */
#define L1PMPPA17_RESV_1_BIT_START                          16U
#define L1PMPPA17_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA17 */
#define L1PMPPA17_AID5_BIT_START                            15U
#define L1PMPPA17_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA17 */
#define L1PMPPA17_AID4_BIT_START                            14U
#define L1PMPPA17_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA17 */
#define L1PMPPA17_AID3_BIT_START                            13U
#define L1PMPPA17_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA17 */
#define L1PMPPA17_AID2_BIT_START                            12U
#define L1PMPPA17_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA17 */
#define L1PMPPA17_AID1_BIT_START                            11U
#define L1PMPPA17_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA17 */
#define L1PMPPA17_AID0_BIT_START                            10U
#define L1PMPPA17_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA17 */
#define L1PMPPA17_AIDX_BIT_START                            9U
#define L1PMPPA17_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA17 */
#define L1PMPPA17_LOCAL_BIT_START                           8U
#define L1PMPPA17_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA17 */
#define L1PMPPA17_NS_BIT_START                              7U
#define L1PMPPA17_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA17 */
#define L1PMPPA17_EMU_BIT_START                             6U
#define L1PMPPA17_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA17 */
#define L1PMPPA17_SR_BIT_START                              5U
#define L1PMPPA17_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA17 */
#define L1PMPPA17_SW_BIT_START                              4U
#define L1PMPPA17_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA17 */
#define L1PMPPA17_SX_BIT_START                              3U
#define L1PMPPA17_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA17 */
#define L1PMPPA17_UR_BIT_START                              2U
#define L1PMPPA17_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA17 */
#define L1PMPPA17_UW_BIT_START                              1U
#define L1PMPPA17_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA17 */
#define L1PMPPA17_UX_BIT_START                              0U
#define L1PMPPA17_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA18 */
#define L1PMPPA18_RESV_1_BIT_START                          16U
#define L1PMPPA18_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA18 */
#define L1PMPPA18_AID5_BIT_START                            15U
#define L1PMPPA18_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA18 */
#define L1PMPPA18_AID4_BIT_START                            14U
#define L1PMPPA18_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA18 */
#define L1PMPPA18_AID3_BIT_START                            13U
#define L1PMPPA18_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA18 */
#define L1PMPPA18_AID2_BIT_START                            12U
#define L1PMPPA18_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA18 */
#define L1PMPPA18_AID1_BIT_START                            11U
#define L1PMPPA18_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA18 */
#define L1PMPPA18_AID0_BIT_START                            10U
#define L1PMPPA18_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA18 */
#define L1PMPPA18_AIDX_BIT_START                            9U
#define L1PMPPA18_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA18 */
#define L1PMPPA18_LOCAL_BIT_START                           8U
#define L1PMPPA18_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA18 */
#define L1PMPPA18_NS_BIT_START                              7U
#define L1PMPPA18_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA18 */
#define L1PMPPA18_EMU_BIT_START                             6U
#define L1PMPPA18_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA18 */
#define L1PMPPA18_SR_BIT_START                              5U
#define L1PMPPA18_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA18 */
#define L1PMPPA18_SW_BIT_START                              4U
#define L1PMPPA18_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA18 */
#define L1PMPPA18_SX_BIT_START                              3U
#define L1PMPPA18_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA18 */
#define L1PMPPA18_UR_BIT_START                              2U
#define L1PMPPA18_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA18 */
#define L1PMPPA18_UW_BIT_START                              1U
#define L1PMPPA18_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA18 */
#define L1PMPPA18_UX_BIT_START                              0U
#define L1PMPPA18_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA19 */
#define L1PMPPA19_RESV_1_BIT_START                          16U
#define L1PMPPA19_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA19 */
#define L1PMPPA19_AID5_BIT_START                            15U
#define L1PMPPA19_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA19 */
#define L1PMPPA19_AID4_BIT_START                            14U
#define L1PMPPA19_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA19 */
#define L1PMPPA19_AID3_BIT_START                            13U
#define L1PMPPA19_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA19 */
#define L1PMPPA19_AID2_BIT_START                            12U
#define L1PMPPA19_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA19 */
#define L1PMPPA19_AID1_BIT_START                            11U
#define L1PMPPA19_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA19 */
#define L1PMPPA19_AID0_BIT_START                            10U
#define L1PMPPA19_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA19 */
#define L1PMPPA19_AIDX_BIT_START                            9U
#define L1PMPPA19_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA19 */
#define L1PMPPA19_LOCAL_BIT_START                           8U
#define L1PMPPA19_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA19 */
#define L1PMPPA19_NS_BIT_START                              7U
#define L1PMPPA19_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA19 */
#define L1PMPPA19_EMU_BIT_START                             6U
#define L1PMPPA19_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA19 */
#define L1PMPPA19_SR_BIT_START                              5U
#define L1PMPPA19_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA19 */
#define L1PMPPA19_SW_BIT_START                              4U
#define L1PMPPA19_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA19 */
#define L1PMPPA19_SX_BIT_START                              3U
#define L1PMPPA19_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA19 */
#define L1PMPPA19_UR_BIT_START                              2U
#define L1PMPPA19_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA19 */
#define L1PMPPA19_UW_BIT_START                              1U
#define L1PMPPA19_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA19 */
#define L1PMPPA19_UX_BIT_START                              0U
#define L1PMPPA19_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA20 */
#define L1PMPPA20_RESV_1_BIT_START                          16U
#define L1PMPPA20_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA20 */
#define L1PMPPA20_AID5_BIT_START                            15U
#define L1PMPPA20_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA20 */
#define L1PMPPA20_AID4_BIT_START                            14U
#define L1PMPPA20_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA20 */
#define L1PMPPA20_AID3_BIT_START                            13U
#define L1PMPPA20_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA20 */
#define L1PMPPA20_AID2_BIT_START                            12U
#define L1PMPPA20_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA20 */
#define L1PMPPA20_AID1_BIT_START                            11U
#define L1PMPPA20_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA20 */
#define L1PMPPA20_AID0_BIT_START                            10U
#define L1PMPPA20_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA20 */
#define L1PMPPA20_AIDX_BIT_START                            9U
#define L1PMPPA20_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA20 */
#define L1PMPPA20_LOCAL_BIT_START                           8U
#define L1PMPPA20_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA20 */
#define L1PMPPA20_NS_BIT_START                              7U
#define L1PMPPA20_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA20 */
#define L1PMPPA20_EMU_BIT_START                             6U
#define L1PMPPA20_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA20 */
#define L1PMPPA20_SR_BIT_START                              5U
#define L1PMPPA20_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA20 */
#define L1PMPPA20_SW_BIT_START                              4U
#define L1PMPPA20_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA20 */
#define L1PMPPA20_SX_BIT_START                              3U
#define L1PMPPA20_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA20 */
#define L1PMPPA20_UR_BIT_START                              2U
#define L1PMPPA20_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA20 */
#define L1PMPPA20_UW_BIT_START                              1U
#define L1PMPPA20_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA20 */
#define L1PMPPA20_UX_BIT_START                              0U
#define L1PMPPA20_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA21 */
#define L1PMPPA21_RESV_1_BIT_START                          16U
#define L1PMPPA21_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA21 */
#define L1PMPPA21_AID5_BIT_START                            15U
#define L1PMPPA21_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA21 */
#define L1PMPPA21_AID4_BIT_START                            14U
#define L1PMPPA21_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA21 */
#define L1PMPPA21_AID3_BIT_START                            13U
#define L1PMPPA21_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA21 */
#define L1PMPPA21_AID2_BIT_START                            12U
#define L1PMPPA21_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA21 */
#define L1PMPPA21_AID1_BIT_START                            11U
#define L1PMPPA21_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA21 */
#define L1PMPPA21_AID0_BIT_START                            10U
#define L1PMPPA21_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA21 */
#define L1PMPPA21_AIDX_BIT_START                            9U
#define L1PMPPA21_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA21 */
#define L1PMPPA21_LOCAL_BIT_START                           8U
#define L1PMPPA21_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA21 */
#define L1PMPPA21_NS_BIT_START                              7U
#define L1PMPPA21_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA21 */
#define L1PMPPA21_EMU_BIT_START                             6U
#define L1PMPPA21_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA21 */
#define L1PMPPA21_SR_BIT_START                              5U
#define L1PMPPA21_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA21 */
#define L1PMPPA21_SW_BIT_START                              4U
#define L1PMPPA21_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA21 */
#define L1PMPPA21_SX_BIT_START                              3U
#define L1PMPPA21_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA21 */
#define L1PMPPA21_UR_BIT_START                              2U
#define L1PMPPA21_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA21 */
#define L1PMPPA21_UW_BIT_START                              1U
#define L1PMPPA21_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA21 */
#define L1PMPPA21_UX_BIT_START                              0U
#define L1PMPPA21_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA22 */
#define L1PMPPA22_RESV_1_BIT_START                          16U
#define L1PMPPA22_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA22 */
#define L1PMPPA22_AID5_BIT_START                            15U
#define L1PMPPA22_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA22 */
#define L1PMPPA22_AID4_BIT_START                            14U
#define L1PMPPA22_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA22 */
#define L1PMPPA22_AID3_BIT_START                            13U
#define L1PMPPA22_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA22 */
#define L1PMPPA22_AID2_BIT_START                            12U
#define L1PMPPA22_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA22 */
#define L1PMPPA22_AID1_BIT_START                            11U
#define L1PMPPA22_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA22 */
#define L1PMPPA22_AID0_BIT_START                            10U
#define L1PMPPA22_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA22 */
#define L1PMPPA22_AIDX_BIT_START                            9U
#define L1PMPPA22_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA22 */
#define L1PMPPA22_LOCAL_BIT_START                           8U
#define L1PMPPA22_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA22 */
#define L1PMPPA22_NS_BIT_START                              7U
#define L1PMPPA22_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA22 */
#define L1PMPPA22_EMU_BIT_START                             6U
#define L1PMPPA22_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA22 */
#define L1PMPPA22_SR_BIT_START                              5U
#define L1PMPPA22_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA22 */
#define L1PMPPA22_SW_BIT_START                              4U
#define L1PMPPA22_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA22 */
#define L1PMPPA22_SX_BIT_START                              3U
#define L1PMPPA22_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA22 */
#define L1PMPPA22_UR_BIT_START                              2U
#define L1PMPPA22_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA22 */
#define L1PMPPA22_UW_BIT_START                              1U
#define L1PMPPA22_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA22 */
#define L1PMPPA22_UX_BIT_START                              0U
#define L1PMPPA22_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA23 */
#define L1PMPPA23_RESV_1_BIT_START                          16U
#define L1PMPPA23_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA23 */
#define L1PMPPA23_AID5_BIT_START                            15U
#define L1PMPPA23_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA23 */
#define L1PMPPA23_AID4_BIT_START                            14U
#define L1PMPPA23_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA23 */
#define L1PMPPA23_AID3_BIT_START                            13U
#define L1PMPPA23_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA23 */
#define L1PMPPA23_AID2_BIT_START                            12U
#define L1PMPPA23_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA23 */
#define L1PMPPA23_AID1_BIT_START                            11U
#define L1PMPPA23_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA23 */
#define L1PMPPA23_AID0_BIT_START                            10U
#define L1PMPPA23_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA23 */
#define L1PMPPA23_AIDX_BIT_START                            9U
#define L1PMPPA23_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA23 */
#define L1PMPPA23_LOCAL_BIT_START                           8U
#define L1PMPPA23_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA23 */
#define L1PMPPA23_NS_BIT_START                              7U
#define L1PMPPA23_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA23 */
#define L1PMPPA23_EMU_BIT_START                             6U
#define L1PMPPA23_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA23 */
#define L1PMPPA23_SR_BIT_START                              5U
#define L1PMPPA23_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA23 */
#define L1PMPPA23_SW_BIT_START                              4U
#define L1PMPPA23_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA23 */
#define L1PMPPA23_SX_BIT_START                              3U
#define L1PMPPA23_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA23 */
#define L1PMPPA23_UR_BIT_START                              2U
#define L1PMPPA23_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA23 */
#define L1PMPPA23_UW_BIT_START                              1U
#define L1PMPPA23_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA23 */
#define L1PMPPA23_UX_BIT_START                              0U
#define L1PMPPA23_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA24 */
#define L1PMPPA24_RESV_1_BIT_START                          16U
#define L1PMPPA24_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA24 */
#define L1PMPPA24_AID5_BIT_START                            15U
#define L1PMPPA24_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA24 */
#define L1PMPPA24_AID4_BIT_START                            14U
#define L1PMPPA24_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA24 */
#define L1PMPPA24_AID3_BIT_START                            13U
#define L1PMPPA24_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA24 */
#define L1PMPPA24_AID2_BIT_START                            12U
#define L1PMPPA24_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA24 */
#define L1PMPPA24_AID1_BIT_START                            11U
#define L1PMPPA24_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA24 */
#define L1PMPPA24_AID0_BIT_START                            10U
#define L1PMPPA24_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA24 */
#define L1PMPPA24_AIDX_BIT_START                            9U
#define L1PMPPA24_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA24 */
#define L1PMPPA24_LOCAL_BIT_START                           8U
#define L1PMPPA24_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA24 */
#define L1PMPPA24_NS_BIT_START                              7U
#define L1PMPPA24_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA24 */
#define L1PMPPA24_EMU_BIT_START                             6U
#define L1PMPPA24_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA24 */
#define L1PMPPA24_SR_BIT_START                              5U
#define L1PMPPA24_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA24 */
#define L1PMPPA24_SW_BIT_START                              4U
#define L1PMPPA24_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA24 */
#define L1PMPPA24_SX_BIT_START                              3U
#define L1PMPPA24_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA24 */
#define L1PMPPA24_UR_BIT_START                              2U
#define L1PMPPA24_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA24 */
#define L1PMPPA24_UW_BIT_START                              1U
#define L1PMPPA24_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA24 */
#define L1PMPPA24_UX_BIT_START                              0U
#define L1PMPPA24_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA25 */
#define L1PMPPA25_RESV_1_BIT_START                          16U
#define L1PMPPA25_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA25 */
#define L1PMPPA25_AID5_BIT_START                            15U
#define L1PMPPA25_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA25 */
#define L1PMPPA25_AID4_BIT_START                            14U
#define L1PMPPA25_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA25 */
#define L1PMPPA25_AID3_BIT_START                            13U
#define L1PMPPA25_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA25 */
#define L1PMPPA25_AID2_BIT_START                            12U
#define L1PMPPA25_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA25 */
#define L1PMPPA25_AID1_BIT_START                            11U
#define L1PMPPA25_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA25 */
#define L1PMPPA25_AID0_BIT_START                            10U
#define L1PMPPA25_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA25 */
#define L1PMPPA25_AIDX_BIT_START                            9U
#define L1PMPPA25_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA25 */
#define L1PMPPA25_LOCAL_BIT_START                           8U
#define L1PMPPA25_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA25 */
#define L1PMPPA25_NS_BIT_START                              7U
#define L1PMPPA25_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA25 */
#define L1PMPPA25_EMU_BIT_START                             6U
#define L1PMPPA25_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA25 */
#define L1PMPPA25_SR_BIT_START                              5U
#define L1PMPPA25_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA25 */
#define L1PMPPA25_SW_BIT_START                              4U
#define L1PMPPA25_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA25 */
#define L1PMPPA25_SX_BIT_START                              3U
#define L1PMPPA25_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA25 */
#define L1PMPPA25_UR_BIT_START                              2U
#define L1PMPPA25_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA25 */
#define L1PMPPA25_UW_BIT_START                              1U
#define L1PMPPA25_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA25 */
#define L1PMPPA25_UX_BIT_START                              0U
#define L1PMPPA25_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA26 */
#define L1PMPPA26_RESV_1_BIT_START                          16U
#define L1PMPPA26_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA26 */
#define L1PMPPA26_AID5_BIT_START                            15U
#define L1PMPPA26_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA26 */
#define L1PMPPA26_AID4_BIT_START                            14U
#define L1PMPPA26_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA26 */
#define L1PMPPA26_AID3_BIT_START                            13U
#define L1PMPPA26_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA26 */
#define L1PMPPA26_AID2_BIT_START                            12U
#define L1PMPPA26_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA26 */
#define L1PMPPA26_AID1_BIT_START                            11U
#define L1PMPPA26_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA26 */
#define L1PMPPA26_AID0_BIT_START                            10U
#define L1PMPPA26_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA26 */
#define L1PMPPA26_AIDX_BIT_START                            9U
#define L1PMPPA26_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA26 */
#define L1PMPPA26_LOCAL_BIT_START                           8U
#define L1PMPPA26_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA26 */
#define L1PMPPA26_NS_BIT_START                              7U
#define L1PMPPA26_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA26 */
#define L1PMPPA26_EMU_BIT_START                             6U
#define L1PMPPA26_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA26 */
#define L1PMPPA26_SR_BIT_START                              5U
#define L1PMPPA26_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA26 */
#define L1PMPPA26_SW_BIT_START                              4U
#define L1PMPPA26_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA26 */
#define L1PMPPA26_SX_BIT_START                              3U
#define L1PMPPA26_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA26 */
#define L1PMPPA26_UR_BIT_START                              2U
#define L1PMPPA26_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA26 */
#define L1PMPPA26_UW_BIT_START                              1U
#define L1PMPPA26_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA26 */
#define L1PMPPA26_UX_BIT_START                              0U
#define L1PMPPA26_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA27 */
#define L1PMPPA27_RESV_1_BIT_START                          16U
#define L1PMPPA27_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA27 */
#define L1PMPPA27_AID5_BIT_START                            15U
#define L1PMPPA27_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA27 */
#define L1PMPPA27_AID4_BIT_START                            14U
#define L1PMPPA27_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA27 */
#define L1PMPPA27_AID3_BIT_START                            13U
#define L1PMPPA27_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA27 */
#define L1PMPPA27_AID2_BIT_START                            12U
#define L1PMPPA27_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA27 */
#define L1PMPPA27_AID1_BIT_START                            11U
#define L1PMPPA27_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA27 */
#define L1PMPPA27_AID0_BIT_START                            10U
#define L1PMPPA27_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA27 */
#define L1PMPPA27_AIDX_BIT_START                            9U
#define L1PMPPA27_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA27 */
#define L1PMPPA27_LOCAL_BIT_START                           8U
#define L1PMPPA27_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA27 */
#define L1PMPPA27_NS_BIT_START                              7U
#define L1PMPPA27_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA27 */
#define L1PMPPA27_EMU_BIT_START                             6U
#define L1PMPPA27_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA27 */
#define L1PMPPA27_SR_BIT_START                              5U
#define L1PMPPA27_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA27 */
#define L1PMPPA27_SW_BIT_START                              4U
#define L1PMPPA27_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA27 */
#define L1PMPPA27_SX_BIT_START                              3U
#define L1PMPPA27_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA27 */
#define L1PMPPA27_UR_BIT_START                              2U
#define L1PMPPA27_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA27 */
#define L1PMPPA27_UW_BIT_START                              1U
#define L1PMPPA27_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA27 */
#define L1PMPPA27_UX_BIT_START                              0U
#define L1PMPPA27_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA28 */
#define L1PMPPA28_RESV_1_BIT_START                          16U
#define L1PMPPA28_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA28 */
#define L1PMPPA28_AID5_BIT_START                            15U
#define L1PMPPA28_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA28 */
#define L1PMPPA28_AID4_BIT_START                            14U
#define L1PMPPA28_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA28 */
#define L1PMPPA28_AID3_BIT_START                            13U
#define L1PMPPA28_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA28 */
#define L1PMPPA28_AID2_BIT_START                            12U
#define L1PMPPA28_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA28 */
#define L1PMPPA28_AID1_BIT_START                            11U
#define L1PMPPA28_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA28 */
#define L1PMPPA28_AID0_BIT_START                            10U
#define L1PMPPA28_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA28 */
#define L1PMPPA28_AIDX_BIT_START                            9U
#define L1PMPPA28_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA28 */
#define L1PMPPA28_LOCAL_BIT_START                           8U
#define L1PMPPA28_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA28 */
#define L1PMPPA28_NS_BIT_START                              7U
#define L1PMPPA28_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA28 */
#define L1PMPPA28_EMU_BIT_START                             6U
#define L1PMPPA28_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA28 */
#define L1PMPPA28_SR_BIT_START                              5U
#define L1PMPPA28_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA28 */
#define L1PMPPA28_SW_BIT_START                              4U
#define L1PMPPA28_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA28 */
#define L1PMPPA28_SX_BIT_START                              3U
#define L1PMPPA28_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA28 */
#define L1PMPPA28_UR_BIT_START                              2U
#define L1PMPPA28_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA28 */
#define L1PMPPA28_UW_BIT_START                              1U
#define L1PMPPA28_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA28 */
#define L1PMPPA28_UX_BIT_START                              0U
#define L1PMPPA28_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA29 */
#define L1PMPPA29_RESV_1_BIT_START                          16U
#define L1PMPPA29_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA29 */
#define L1PMPPA29_AID5_BIT_START                            15U
#define L1PMPPA29_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA29 */
#define L1PMPPA29_AID4_BIT_START                            14U
#define L1PMPPA29_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA29 */
#define L1PMPPA29_AID3_BIT_START                            13U
#define L1PMPPA29_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA29 */
#define L1PMPPA29_AID2_BIT_START                            12U
#define L1PMPPA29_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA29 */
#define L1PMPPA29_AID1_BIT_START                            11U
#define L1PMPPA29_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA29 */
#define L1PMPPA29_AID0_BIT_START                            10U
#define L1PMPPA29_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA29 */
#define L1PMPPA29_AIDX_BIT_START                            9U
#define L1PMPPA29_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA29 */
#define L1PMPPA29_LOCAL_BIT_START                           8U
#define L1PMPPA29_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA29 */
#define L1PMPPA29_NS_BIT_START                              7U
#define L1PMPPA29_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA29 */
#define L1PMPPA29_EMU_BIT_START                             6U
#define L1PMPPA29_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA29 */
#define L1PMPPA29_SR_BIT_START                              5U
#define L1PMPPA29_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA29 */
#define L1PMPPA29_SW_BIT_START                              4U
#define L1PMPPA29_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA29 */
#define L1PMPPA29_SX_BIT_START                              3U
#define L1PMPPA29_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA29 */
#define L1PMPPA29_UR_BIT_START                              2U
#define L1PMPPA29_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA29 */
#define L1PMPPA29_UW_BIT_START                              1U
#define L1PMPPA29_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA29 */
#define L1PMPPA29_UX_BIT_START                              0U
#define L1PMPPA29_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA30 */
#define L1PMPPA30_RESV_1_BIT_START                          16U
#define L1PMPPA30_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA30 */
#define L1PMPPA30_AID5_BIT_START                            15U
#define L1PMPPA30_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA30 */
#define L1PMPPA30_AID4_BIT_START                            14U
#define L1PMPPA30_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA30 */
#define L1PMPPA30_AID3_BIT_START                            13U
#define L1PMPPA30_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA30 */
#define L1PMPPA30_AID2_BIT_START                            12U
#define L1PMPPA30_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA30 */
#define L1PMPPA30_AID1_BIT_START                            11U
#define L1PMPPA30_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA30 */
#define L1PMPPA30_AID0_BIT_START                            10U
#define L1PMPPA30_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA30 */
#define L1PMPPA30_AIDX_BIT_START                            9U
#define L1PMPPA30_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA30 */
#define L1PMPPA30_LOCAL_BIT_START                           8U
#define L1PMPPA30_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA30 */
#define L1PMPPA30_NS_BIT_START                              7U
#define L1PMPPA30_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA30 */
#define L1PMPPA30_EMU_BIT_START                             6U
#define L1PMPPA30_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA30 */
#define L1PMPPA30_SR_BIT_START                              5U
#define L1PMPPA30_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA30 */
#define L1PMPPA30_SW_BIT_START                              4U
#define L1PMPPA30_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA30 */
#define L1PMPPA30_SX_BIT_START                              3U
#define L1PMPPA30_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA30 */
#define L1PMPPA30_UR_BIT_START                              2U
#define L1PMPPA30_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA30 */
#define L1PMPPA30_UW_BIT_START                              1U
#define L1PMPPA30_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA30 */
#define L1PMPPA30_UX_BIT_START                              0U
#define L1PMPPA30_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1PMPPA31 */
#define L1PMPPA31_RESV_1_BIT_START                          16U
#define L1PMPPA31_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1PMPPA31 */
#define L1PMPPA31_AID5_BIT_START                            15U
#define L1PMPPA31_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1PMPPA31 */
#define L1PMPPA31_AID4_BIT_START                            14U
#define L1PMPPA31_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1PMPPA31 */
#define L1PMPPA31_AID3_BIT_START                            13U
#define L1PMPPA31_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1PMPPA31 */
#define L1PMPPA31_AID2_BIT_START                            12U
#define L1PMPPA31_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1PMPPA31 */
#define L1PMPPA31_AID1_BIT_START                            11U
#define L1PMPPA31_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1PMPPA31 */
#define L1PMPPA31_AID0_BIT_START                            10U
#define L1PMPPA31_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1PMPPA31 */
#define L1PMPPA31_AIDX_BIT_START                            9U
#define L1PMPPA31_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1PMPPA31 */
#define L1PMPPA31_LOCAL_BIT_START                           8U
#define L1PMPPA31_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1PMPPA31 */
#define L1PMPPA31_NS_BIT_START                              7U
#define L1PMPPA31_NS_BIT_END                                7U

/* Definition for field EMU in Register L1PMPPA31 */
#define L1PMPPA31_EMU_BIT_START                             6U
#define L1PMPPA31_EMU_BIT_END                               6U

/* Definition for field SR in Register L1PMPPA31 */
#define L1PMPPA31_SR_BIT_START                              5U
#define L1PMPPA31_SR_BIT_END                                5U

/* Definition for field SW in Register L1PMPPA31 */
#define L1PMPPA31_SW_BIT_START                              4U
#define L1PMPPA31_SW_BIT_END                                4U

/* Definition for field SX in Register L1PMPPA31 */
#define L1PMPPA31_SX_BIT_START                              3U
#define L1PMPPA31_SX_BIT_END                                3U

/* Definition for field UR in Register L1PMPPA31 */
#define L1PMPPA31_UR_BIT_START                              2U
#define L1PMPPA31_UR_BIT_END                                2U

/* Definition for field UW in Register L1PMPPA31 */
#define L1PMPPA31_UW_BIT_START                              1U
#define L1PMPPA31_UW_BIT_END                                1U

/* Definition for field UX in Register L1PMPPA31 */
#define L1PMPPA31_UX_BIT_START                              0U
#define L1PMPPA31_UX_BIT_END                                0U

/* Definition for field ADDR in Register L1DMPFAR */
#define L1DMPFAR_ADDR_BIT_START                             0U
#define L1DMPFAR_ADDR_BIT_END                               31U

/* Definition for field RESV_1 in Register L1DMPFSR */
#define L1DMPFSR_RESV_1_BIT_START                           16U
#define L1DMPFSR_RESV_1_BIT_END                             31U

/* Definition for field FID in Register L1DMPFSR */
#define L1DMPFSR_FID_BIT_START                              9U
#define L1DMPFSR_FID_BIT_END                                15U

/* Definition for field LOCAL in Register L1DMPFSR */
#define L1DMPFSR_LOCAL_BIT_START                            8U
#define L1DMPFSR_LOCAL_BIT_END                              8U

/* Definition for field NS in Register L1DMPFSR */
#define L1DMPFSR_NS_BIT_START                               7U
#define L1DMPFSR_NS_BIT_END                                 7U

/* Definition for field RESV_2 in Register L1DMPFSR */
#define L1DMPFSR_RESV_2_BIT_START                           6U
#define L1DMPFSR_RESV_2_BIT_END                             6U

/* Definition for field SR in Register L1DMPFSR */
#define L1DMPFSR_SR_BIT_START                               5U
#define L1DMPFSR_SR_BIT_END                                 5U

/* Definition for field SW in Register L1DMPFSR */
#define L1DMPFSR_SW_BIT_START                               4U
#define L1DMPFSR_SW_BIT_END                                 4U

/* Definition for field SX in Register L1DMPFSR */
#define L1DMPFSR_SX_BIT_START                               3U
#define L1DMPFSR_SX_BIT_END                                 3U

/* Definition for field UR in Register L1DMPFSR */
#define L1DMPFSR_UR_BIT_START                               2U
#define L1DMPFSR_UR_BIT_END                                 2U

/* Definition for field UW in Register L1DMPFSR */
#define L1DMPFSR_UW_BIT_START                               1U
#define L1DMPFSR_UW_BIT_END                                 1U

/* Definition for field UX in Register L1DMPFSR */
#define L1DMPFSR_UX_BIT_START                               0U
#define L1DMPFSR_UX_BIT_END                                 0U

/* Definition for field RESV_1 in Register L1DMPFCR */
#define L1DMPFCR_RESV_1_BIT_START                           1U
#define L1DMPFCR_RESV_1_BIT_END                             31U

/* Definition for field MPFCLR in Register L1DMPFCR */
#define L1DMPFCR_MPFCLR_BIT_START                           0U
#define L1DMPFCR_MPFCLR_BIT_END                             0U

/* Definition for field MPLK in Register MPLK0 */
#define MPLK0_MPLK_BIT_START                                0U
#define MPLK0_MPLK_BIT_END                                  31U

/* Definition for field MPLK in Register MPLK1 */
#define MPLK1_MPLK_BIT_START                                0U
#define MPLK1_MPLK_BIT_END                                  31U

/* Definition for field MPLK in Register MPLK2 */
#define MPLK2_MPLK_BIT_START                                0U
#define MPLK2_MPLK_BIT_END                                  31U

/* Definition for field MPLK in Register MPLK3 */
#define MPLK3_MPLK_BIT_START                                0U
#define MPLK3_MPLK_BIT_END                                  31U

/* Definition for field RESV_1 in Register MPLKCMD */
#define MPLKCMD_RESV_1_BIT_START                            3U
#define MPLKCMD_RESV_1_BIT_END                              31U

/* Definition for field KEYR in Register MPLKCMD */
#define MPLKCMD_KEYR_BIT_START                              2U
#define MPLKCMD_KEYR_BIT_END                                2U

/* Definition for field LOCK in Register MPLKCMD */
#define MPLKCMD_LOCK_BIT_START                              1U
#define MPLKCMD_LOCK_BIT_END                                1U

/* Definition for field UNLOCK in Register MPLKCMD */
#define MPLKCMD_UNLOCK_BIT_START                            0U
#define MPLKCMD_UNLOCK_BIT_END                              0U

/* Definition for field RESV_1 in Register MPLKSTAT */
#define MPLKSTAT_RESV_1_BIT_START                           2U
#define MPLKSTAT_RESV_1_BIT_END                             31U

/* Definition for field NSL in Register MPLKSTAT */
#define MPLKSTAT_NSL_BIT_START                              1U
#define MPLKSTAT_NSL_BIT_END                                1U

/* Definition for field LK in Register MPLKSTAT */
#define MPLKSTAT_LK_BIT_START                               0U
#define MPLKSTAT_LK_BIT_END                                 0U

/* Definition for field RESV_1 in Register L1DMPPA16 */
#define L1DMPPA16_RESV_1_BIT_START                          16U
#define L1DMPPA16_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA16 */
#define L1DMPPA16_AID5_BIT_START                            15U
#define L1DMPPA16_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA16 */
#define L1DMPPA16_AID4_BIT_START                            14U
#define L1DMPPA16_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA16 */
#define L1DMPPA16_AID3_BIT_START                            13U
#define L1DMPPA16_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA16 */
#define L1DMPPA16_AID2_BIT_START                            12U
#define L1DMPPA16_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA16 */
#define L1DMPPA16_AID1_BIT_START                            11U
#define L1DMPPA16_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA16 */
#define L1DMPPA16_AID0_BIT_START                            10U
#define L1DMPPA16_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA16 */
#define L1DMPPA16_AIDX_BIT_START                            9U
#define L1DMPPA16_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA16 */
#define L1DMPPA16_LOCAL_BIT_START                           8U
#define L1DMPPA16_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA16 */
#define L1DMPPA16_NS_BIT_START                              7U
#define L1DMPPA16_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA16 */
#define L1DMPPA16_EMU_BIT_START                             6U
#define L1DMPPA16_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA16 */
#define L1DMPPA16_SR_BIT_START                              5U
#define L1DMPPA16_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA16 */
#define L1DMPPA16_SW_BIT_START                              4U
#define L1DMPPA16_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA16 */
#define L1DMPPA16_SX_BIT_START                              3U
#define L1DMPPA16_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA16 */
#define L1DMPPA16_UR_BIT_START                              2U
#define L1DMPPA16_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA16 */
#define L1DMPPA16_UW_BIT_START                              1U
#define L1DMPPA16_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA16 */
#define L1DMPPA16_UX_BIT_START                              0U
#define L1DMPPA16_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA17 */
#define L1DMPPA17_RESV_1_BIT_START                          16U
#define L1DMPPA17_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA17 */
#define L1DMPPA17_AID5_BIT_START                            15U
#define L1DMPPA17_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA17 */
#define L1DMPPA17_AID4_BIT_START                            14U
#define L1DMPPA17_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA17 */
#define L1DMPPA17_AID3_BIT_START                            13U
#define L1DMPPA17_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA17 */
#define L1DMPPA17_AID2_BIT_START                            12U
#define L1DMPPA17_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA17 */
#define L1DMPPA17_AID1_BIT_START                            11U
#define L1DMPPA17_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA17 */
#define L1DMPPA17_AID0_BIT_START                            10U
#define L1DMPPA17_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA17 */
#define L1DMPPA17_AIDX_BIT_START                            9U
#define L1DMPPA17_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA17 */
#define L1DMPPA17_LOCAL_BIT_START                           8U
#define L1DMPPA17_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA17 */
#define L1DMPPA17_NS_BIT_START                              7U
#define L1DMPPA17_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA17 */
#define L1DMPPA17_EMU_BIT_START                             6U
#define L1DMPPA17_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA17 */
#define L1DMPPA17_SR_BIT_START                              5U
#define L1DMPPA17_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA17 */
#define L1DMPPA17_SW_BIT_START                              4U
#define L1DMPPA17_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA17 */
#define L1DMPPA17_SX_BIT_START                              3U
#define L1DMPPA17_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA17 */
#define L1DMPPA17_UR_BIT_START                              2U
#define L1DMPPA17_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA17 */
#define L1DMPPA17_UW_BIT_START                              1U
#define L1DMPPA17_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA17 */
#define L1DMPPA17_UX_BIT_START                              0U
#define L1DMPPA17_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA18 */
#define L1DMPPA18_RESV_1_BIT_START                          16U
#define L1DMPPA18_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA18 */
#define L1DMPPA18_AID5_BIT_START                            15U
#define L1DMPPA18_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA18 */
#define L1DMPPA18_AID4_BIT_START                            14U
#define L1DMPPA18_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA18 */
#define L1DMPPA18_AID3_BIT_START                            13U
#define L1DMPPA18_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA18 */
#define L1DMPPA18_AID2_BIT_START                            12U
#define L1DMPPA18_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA18 */
#define L1DMPPA18_AID1_BIT_START                            11U
#define L1DMPPA18_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA18 */
#define L1DMPPA18_AID0_BIT_START                            10U
#define L1DMPPA18_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA18 */
#define L1DMPPA18_AIDX_BIT_START                            9U
#define L1DMPPA18_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA18 */
#define L1DMPPA18_LOCAL_BIT_START                           8U
#define L1DMPPA18_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA18 */
#define L1DMPPA18_NS_BIT_START                              7U
#define L1DMPPA18_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA18 */
#define L1DMPPA18_EMU_BIT_START                             6U
#define L1DMPPA18_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA18 */
#define L1DMPPA18_SR_BIT_START                              5U
#define L1DMPPA18_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA18 */
#define L1DMPPA18_SW_BIT_START                              4U
#define L1DMPPA18_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA18 */
#define L1DMPPA18_SX_BIT_START                              3U
#define L1DMPPA18_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA18 */
#define L1DMPPA18_UR_BIT_START                              2U
#define L1DMPPA18_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA18 */
#define L1DMPPA18_UW_BIT_START                              1U
#define L1DMPPA18_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA18 */
#define L1DMPPA18_UX_BIT_START                              0U
#define L1DMPPA18_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA19 */
#define L1DMPPA19_RESV_1_BIT_START                          16U
#define L1DMPPA19_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA19 */
#define L1DMPPA19_AID5_BIT_START                            15U
#define L1DMPPA19_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA19 */
#define L1DMPPA19_AID4_BIT_START                            14U
#define L1DMPPA19_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA19 */
#define L1DMPPA19_AID3_BIT_START                            13U
#define L1DMPPA19_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA19 */
#define L1DMPPA19_AID2_BIT_START                            12U
#define L1DMPPA19_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA19 */
#define L1DMPPA19_AID1_BIT_START                            11U
#define L1DMPPA19_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA19 */
#define L1DMPPA19_AID0_BIT_START                            10U
#define L1DMPPA19_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA19 */
#define L1DMPPA19_AIDX_BIT_START                            9U
#define L1DMPPA19_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA19 */
#define L1DMPPA19_LOCAL_BIT_START                           8U
#define L1DMPPA19_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA19 */
#define L1DMPPA19_NS_BIT_START                              7U
#define L1DMPPA19_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA19 */
#define L1DMPPA19_EMU_BIT_START                             6U
#define L1DMPPA19_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA19 */
#define L1DMPPA19_SR_BIT_START                              5U
#define L1DMPPA19_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA19 */
#define L1DMPPA19_SW_BIT_START                              4U
#define L1DMPPA19_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA19 */
#define L1DMPPA19_SX_BIT_START                              3U
#define L1DMPPA19_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA19 */
#define L1DMPPA19_UR_BIT_START                              2U
#define L1DMPPA19_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA19 */
#define L1DMPPA19_UW_BIT_START                              1U
#define L1DMPPA19_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA19 */
#define L1DMPPA19_UX_BIT_START                              0U
#define L1DMPPA19_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA20 */
#define L1DMPPA20_RESV_1_BIT_START                          16U
#define L1DMPPA20_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA20 */
#define L1DMPPA20_AID5_BIT_START                            15U
#define L1DMPPA20_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA20 */
#define L1DMPPA20_AID4_BIT_START                            14U
#define L1DMPPA20_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA20 */
#define L1DMPPA20_AID3_BIT_START                            13U
#define L1DMPPA20_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA20 */
#define L1DMPPA20_AID2_BIT_START                            12U
#define L1DMPPA20_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA20 */
#define L1DMPPA20_AID1_BIT_START                            11U
#define L1DMPPA20_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA20 */
#define L1DMPPA20_AID0_BIT_START                            10U
#define L1DMPPA20_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA20 */
#define L1DMPPA20_AIDX_BIT_START                            9U
#define L1DMPPA20_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA20 */
#define L1DMPPA20_LOCAL_BIT_START                           8U
#define L1DMPPA20_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA20 */
#define L1DMPPA20_NS_BIT_START                              7U
#define L1DMPPA20_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA20 */
#define L1DMPPA20_EMU_BIT_START                             6U
#define L1DMPPA20_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA20 */
#define L1DMPPA20_SR_BIT_START                              5U
#define L1DMPPA20_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA20 */
#define L1DMPPA20_SW_BIT_START                              4U
#define L1DMPPA20_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA20 */
#define L1DMPPA20_SX_BIT_START                              3U
#define L1DMPPA20_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA20 */
#define L1DMPPA20_UR_BIT_START                              2U
#define L1DMPPA20_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA20 */
#define L1DMPPA20_UW_BIT_START                              1U
#define L1DMPPA20_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA20 */
#define L1DMPPA20_UX_BIT_START                              0U
#define L1DMPPA20_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA21 */
#define L1DMPPA21_RESV_1_BIT_START                          16U
#define L1DMPPA21_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA21 */
#define L1DMPPA21_AID5_BIT_START                            15U
#define L1DMPPA21_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA21 */
#define L1DMPPA21_AID4_BIT_START                            14U
#define L1DMPPA21_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA21 */
#define L1DMPPA21_AID3_BIT_START                            13U
#define L1DMPPA21_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA21 */
#define L1DMPPA21_AID2_BIT_START                            12U
#define L1DMPPA21_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA21 */
#define L1DMPPA21_AID1_BIT_START                            11U
#define L1DMPPA21_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA21 */
#define L1DMPPA21_AID0_BIT_START                            10U
#define L1DMPPA21_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA21 */
#define L1DMPPA21_AIDX_BIT_START                            9U
#define L1DMPPA21_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA21 */
#define L1DMPPA21_LOCAL_BIT_START                           8U
#define L1DMPPA21_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA21 */
#define L1DMPPA21_NS_BIT_START                              7U
#define L1DMPPA21_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA21 */
#define L1DMPPA21_EMU_BIT_START                             6U
#define L1DMPPA21_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA21 */
#define L1DMPPA21_SR_BIT_START                              5U
#define L1DMPPA21_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA21 */
#define L1DMPPA21_SW_BIT_START                              4U
#define L1DMPPA21_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA21 */
#define L1DMPPA21_SX_BIT_START                              3U
#define L1DMPPA21_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA21 */
#define L1DMPPA21_UR_BIT_START                              2U
#define L1DMPPA21_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA21 */
#define L1DMPPA21_UW_BIT_START                              1U
#define L1DMPPA21_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA21 */
#define L1DMPPA21_UX_BIT_START                              0U
#define L1DMPPA21_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA22 */
#define L1DMPPA22_RESV_1_BIT_START                          16U
#define L1DMPPA22_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA22 */
#define L1DMPPA22_AID5_BIT_START                            15U
#define L1DMPPA22_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA22 */
#define L1DMPPA22_AID4_BIT_START                            14U
#define L1DMPPA22_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA22 */
#define L1DMPPA22_AID3_BIT_START                            13U
#define L1DMPPA22_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA22 */
#define L1DMPPA22_AID2_BIT_START                            12U
#define L1DMPPA22_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA22 */
#define L1DMPPA22_AID1_BIT_START                            11U
#define L1DMPPA22_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA22 */
#define L1DMPPA22_AID0_BIT_START                            10U
#define L1DMPPA22_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA22 */
#define L1DMPPA22_AIDX_BIT_START                            9U
#define L1DMPPA22_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA22 */
#define L1DMPPA22_LOCAL_BIT_START                           8U
#define L1DMPPA22_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA22 */
#define L1DMPPA22_NS_BIT_START                              7U
#define L1DMPPA22_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA22 */
#define L1DMPPA22_EMU_BIT_START                             6U
#define L1DMPPA22_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA22 */
#define L1DMPPA22_SR_BIT_START                              5U
#define L1DMPPA22_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA22 */
#define L1DMPPA22_SW_BIT_START                              4U
#define L1DMPPA22_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA22 */
#define L1DMPPA22_SX_BIT_START                              3U
#define L1DMPPA22_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA22 */
#define L1DMPPA22_UR_BIT_START                              2U
#define L1DMPPA22_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA22 */
#define L1DMPPA22_UW_BIT_START                              1U
#define L1DMPPA22_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA22 */
#define L1DMPPA22_UX_BIT_START                              0U
#define L1DMPPA22_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA23 */
#define L1DMPPA23_RESV_1_BIT_START                          16U
#define L1DMPPA23_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA23 */
#define L1DMPPA23_AID5_BIT_START                            15U
#define L1DMPPA23_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA23 */
#define L1DMPPA23_AID4_BIT_START                            14U
#define L1DMPPA23_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA23 */
#define L1DMPPA23_AID3_BIT_START                            13U
#define L1DMPPA23_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA23 */
#define L1DMPPA23_AID2_BIT_START                            12U
#define L1DMPPA23_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA23 */
#define L1DMPPA23_AID1_BIT_START                            11U
#define L1DMPPA23_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA23 */
#define L1DMPPA23_AID0_BIT_START                            10U
#define L1DMPPA23_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA23 */
#define L1DMPPA23_AIDX_BIT_START                            9U
#define L1DMPPA23_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA23 */
#define L1DMPPA23_LOCAL_BIT_START                           8U
#define L1DMPPA23_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA23 */
#define L1DMPPA23_NS_BIT_START                              7U
#define L1DMPPA23_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA23 */
#define L1DMPPA23_EMU_BIT_START                             6U
#define L1DMPPA23_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA23 */
#define L1DMPPA23_SR_BIT_START                              5U
#define L1DMPPA23_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA23 */
#define L1DMPPA23_SW_BIT_START                              4U
#define L1DMPPA23_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA23 */
#define L1DMPPA23_SX_BIT_START                              3U
#define L1DMPPA23_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA23 */
#define L1DMPPA23_UR_BIT_START                              2U
#define L1DMPPA23_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA23 */
#define L1DMPPA23_UW_BIT_START                              1U
#define L1DMPPA23_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA23 */
#define L1DMPPA23_UX_BIT_START                              0U
#define L1DMPPA23_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA24 */
#define L1DMPPA24_RESV_1_BIT_START                          16U
#define L1DMPPA24_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA24 */
#define L1DMPPA24_AID5_BIT_START                            15U
#define L1DMPPA24_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA24 */
#define L1DMPPA24_AID4_BIT_START                            14U
#define L1DMPPA24_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA24 */
#define L1DMPPA24_AID3_BIT_START                            13U
#define L1DMPPA24_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA24 */
#define L1DMPPA24_AID2_BIT_START                            12U
#define L1DMPPA24_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA24 */
#define L1DMPPA24_AID1_BIT_START                            11U
#define L1DMPPA24_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA24 */
#define L1DMPPA24_AID0_BIT_START                            10U
#define L1DMPPA24_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA24 */
#define L1DMPPA24_AIDX_BIT_START                            9U
#define L1DMPPA24_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA24 */
#define L1DMPPA24_LOCAL_BIT_START                           8U
#define L1DMPPA24_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA24 */
#define L1DMPPA24_NS_BIT_START                              7U
#define L1DMPPA24_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA24 */
#define L1DMPPA24_EMU_BIT_START                             6U
#define L1DMPPA24_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA24 */
#define L1DMPPA24_SR_BIT_START                              5U
#define L1DMPPA24_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA24 */
#define L1DMPPA24_SW_BIT_START                              4U
#define L1DMPPA24_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA24 */
#define L1DMPPA24_SX_BIT_START                              3U
#define L1DMPPA24_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA24 */
#define L1DMPPA24_UR_BIT_START                              2U
#define L1DMPPA24_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA24 */
#define L1DMPPA24_UW_BIT_START                              1U
#define L1DMPPA24_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA24 */
#define L1DMPPA24_UX_BIT_START                              0U
#define L1DMPPA24_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA25 */
#define L1DMPPA25_RESV_1_BIT_START                          16U
#define L1DMPPA25_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA25 */
#define L1DMPPA25_AID5_BIT_START                            15U
#define L1DMPPA25_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA25 */
#define L1DMPPA25_AID4_BIT_START                            14U
#define L1DMPPA25_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA25 */
#define L1DMPPA25_AID3_BIT_START                            13U
#define L1DMPPA25_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA25 */
#define L1DMPPA25_AID2_BIT_START                            12U
#define L1DMPPA25_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA25 */
#define L1DMPPA25_AID1_BIT_START                            11U
#define L1DMPPA25_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA25 */
#define L1DMPPA25_AID0_BIT_START                            10U
#define L1DMPPA25_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA25 */
#define L1DMPPA25_AIDX_BIT_START                            9U
#define L1DMPPA25_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA25 */
#define L1DMPPA25_LOCAL_BIT_START                           8U
#define L1DMPPA25_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA25 */
#define L1DMPPA25_NS_BIT_START                              7U
#define L1DMPPA25_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA25 */
#define L1DMPPA25_EMU_BIT_START                             6U
#define L1DMPPA25_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA25 */
#define L1DMPPA25_SR_BIT_START                              5U
#define L1DMPPA25_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA25 */
#define L1DMPPA25_SW_BIT_START                              4U
#define L1DMPPA25_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA25 */
#define L1DMPPA25_SX_BIT_START                              3U
#define L1DMPPA25_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA25 */
#define L1DMPPA25_UR_BIT_START                              2U
#define L1DMPPA25_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA25 */
#define L1DMPPA25_UW_BIT_START                              1U
#define L1DMPPA25_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA25 */
#define L1DMPPA25_UX_BIT_START                              0U
#define L1DMPPA25_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA26 */
#define L1DMPPA26_RESV_1_BIT_START                          16U
#define L1DMPPA26_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA26 */
#define L1DMPPA26_AID5_BIT_START                            15U
#define L1DMPPA26_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA26 */
#define L1DMPPA26_AID4_BIT_START                            14U
#define L1DMPPA26_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA26 */
#define L1DMPPA26_AID3_BIT_START                            13U
#define L1DMPPA26_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA26 */
#define L1DMPPA26_AID2_BIT_START                            12U
#define L1DMPPA26_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA26 */
#define L1DMPPA26_AID1_BIT_START                            11U
#define L1DMPPA26_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA26 */
#define L1DMPPA26_AID0_BIT_START                            10U
#define L1DMPPA26_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA26 */
#define L1DMPPA26_AIDX_BIT_START                            9U
#define L1DMPPA26_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA26 */
#define L1DMPPA26_LOCAL_BIT_START                           8U
#define L1DMPPA26_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA26 */
#define L1DMPPA26_NS_BIT_START                              7U
#define L1DMPPA26_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA26 */
#define L1DMPPA26_EMU_BIT_START                             6U
#define L1DMPPA26_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA26 */
#define L1DMPPA26_SR_BIT_START                              5U
#define L1DMPPA26_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA26 */
#define L1DMPPA26_SW_BIT_START                              4U
#define L1DMPPA26_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA26 */
#define L1DMPPA26_SX_BIT_START                              3U
#define L1DMPPA26_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA26 */
#define L1DMPPA26_UR_BIT_START                              2U
#define L1DMPPA26_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA26 */
#define L1DMPPA26_UW_BIT_START                              1U
#define L1DMPPA26_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA26 */
#define L1DMPPA26_UX_BIT_START                              0U
#define L1DMPPA26_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA27 */
#define L1DMPPA27_RESV_1_BIT_START                          16U
#define L1DMPPA27_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA27 */
#define L1DMPPA27_AID5_BIT_START                            15U
#define L1DMPPA27_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA27 */
#define L1DMPPA27_AID4_BIT_START                            14U
#define L1DMPPA27_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA27 */
#define L1DMPPA27_AID3_BIT_START                            13U
#define L1DMPPA27_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA27 */
#define L1DMPPA27_AID2_BIT_START                            12U
#define L1DMPPA27_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA27 */
#define L1DMPPA27_AID1_BIT_START                            11U
#define L1DMPPA27_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA27 */
#define L1DMPPA27_AID0_BIT_START                            10U
#define L1DMPPA27_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA27 */
#define L1DMPPA27_AIDX_BIT_START                            9U
#define L1DMPPA27_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA27 */
#define L1DMPPA27_LOCAL_BIT_START                           8U
#define L1DMPPA27_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA27 */
#define L1DMPPA27_NS_BIT_START                              7U
#define L1DMPPA27_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA27 */
#define L1DMPPA27_EMU_BIT_START                             6U
#define L1DMPPA27_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA27 */
#define L1DMPPA27_SR_BIT_START                              5U
#define L1DMPPA27_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA27 */
#define L1DMPPA27_SW_BIT_START                              4U
#define L1DMPPA27_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA27 */
#define L1DMPPA27_SX_BIT_START                              3U
#define L1DMPPA27_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA27 */
#define L1DMPPA27_UR_BIT_START                              2U
#define L1DMPPA27_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA27 */
#define L1DMPPA27_UW_BIT_START                              1U
#define L1DMPPA27_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA27 */
#define L1DMPPA27_UX_BIT_START                              0U
#define L1DMPPA27_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA28 */
#define L1DMPPA28_RESV_1_BIT_START                          16U
#define L1DMPPA28_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA28 */
#define L1DMPPA28_AID5_BIT_START                            15U
#define L1DMPPA28_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA28 */
#define L1DMPPA28_AID4_BIT_START                            14U
#define L1DMPPA28_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA28 */
#define L1DMPPA28_AID3_BIT_START                            13U
#define L1DMPPA28_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA28 */
#define L1DMPPA28_AID2_BIT_START                            12U
#define L1DMPPA28_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA28 */
#define L1DMPPA28_AID1_BIT_START                            11U
#define L1DMPPA28_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA28 */
#define L1DMPPA28_AID0_BIT_START                            10U
#define L1DMPPA28_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA28 */
#define L1DMPPA28_AIDX_BIT_START                            9U
#define L1DMPPA28_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA28 */
#define L1DMPPA28_LOCAL_BIT_START                           8U
#define L1DMPPA28_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA28 */
#define L1DMPPA28_NS_BIT_START                              7U
#define L1DMPPA28_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA28 */
#define L1DMPPA28_EMU_BIT_START                             6U
#define L1DMPPA28_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA28 */
#define L1DMPPA28_SR_BIT_START                              5U
#define L1DMPPA28_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA28 */
#define L1DMPPA28_SW_BIT_START                              4U
#define L1DMPPA28_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA28 */
#define L1DMPPA28_SX_BIT_START                              3U
#define L1DMPPA28_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA28 */
#define L1DMPPA28_UR_BIT_START                              2U
#define L1DMPPA28_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA28 */
#define L1DMPPA28_UW_BIT_START                              1U
#define L1DMPPA28_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA28 */
#define L1DMPPA28_UX_BIT_START                              0U
#define L1DMPPA28_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA29 */
#define L1DMPPA29_RESV_1_BIT_START                          16U
#define L1DMPPA29_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA29 */
#define L1DMPPA29_AID5_BIT_START                            15U
#define L1DMPPA29_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA29 */
#define L1DMPPA29_AID4_BIT_START                            14U
#define L1DMPPA29_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA29 */
#define L1DMPPA29_AID3_BIT_START                            13U
#define L1DMPPA29_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA29 */
#define L1DMPPA29_AID2_BIT_START                            12U
#define L1DMPPA29_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA29 */
#define L1DMPPA29_AID1_BIT_START                            11U
#define L1DMPPA29_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA29 */
#define L1DMPPA29_AID0_BIT_START                            10U
#define L1DMPPA29_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA29 */
#define L1DMPPA29_AIDX_BIT_START                            9U
#define L1DMPPA29_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA29 */
#define L1DMPPA29_LOCAL_BIT_START                           8U
#define L1DMPPA29_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA29 */
#define L1DMPPA29_NS_BIT_START                              7U
#define L1DMPPA29_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA29 */
#define L1DMPPA29_EMU_BIT_START                             6U
#define L1DMPPA29_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA29 */
#define L1DMPPA29_SR_BIT_START                              5U
#define L1DMPPA29_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA29 */
#define L1DMPPA29_SW_BIT_START                              4U
#define L1DMPPA29_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA29 */
#define L1DMPPA29_SX_BIT_START                              3U
#define L1DMPPA29_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA29 */
#define L1DMPPA29_UR_BIT_START                              2U
#define L1DMPPA29_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA29 */
#define L1DMPPA29_UW_BIT_START                              1U
#define L1DMPPA29_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA29 */
#define L1DMPPA29_UX_BIT_START                              0U
#define L1DMPPA29_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA30 */
#define L1DMPPA30_RESV_1_BIT_START                          16U
#define L1DMPPA30_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA30 */
#define L1DMPPA30_AID5_BIT_START                            15U
#define L1DMPPA30_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA30 */
#define L1DMPPA30_AID4_BIT_START                            14U
#define L1DMPPA30_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA30 */
#define L1DMPPA30_AID3_BIT_START                            13U
#define L1DMPPA30_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA30 */
#define L1DMPPA30_AID2_BIT_START                            12U
#define L1DMPPA30_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA30 */
#define L1DMPPA30_AID1_BIT_START                            11U
#define L1DMPPA30_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA30 */
#define L1DMPPA30_AID0_BIT_START                            10U
#define L1DMPPA30_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA30 */
#define L1DMPPA30_AIDX_BIT_START                            9U
#define L1DMPPA30_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA30 */
#define L1DMPPA30_LOCAL_BIT_START                           8U
#define L1DMPPA30_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA30 */
#define L1DMPPA30_NS_BIT_START                              7U
#define L1DMPPA30_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA30 */
#define L1DMPPA30_EMU_BIT_START                             6U
#define L1DMPPA30_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA30 */
#define L1DMPPA30_SR_BIT_START                              5U
#define L1DMPPA30_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA30 */
#define L1DMPPA30_SW_BIT_START                              4U
#define L1DMPPA30_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA30 */
#define L1DMPPA30_SX_BIT_START                              3U
#define L1DMPPA30_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA30 */
#define L1DMPPA30_UR_BIT_START                              2U
#define L1DMPPA30_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA30 */
#define L1DMPPA30_UW_BIT_START                              1U
#define L1DMPPA30_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA30 */
#define L1DMPPA30_UX_BIT_START                              0U
#define L1DMPPA30_UX_BIT_END                                0U

/* Definition for field RESV_1 in Register L1DMPPA31 */
#define L1DMPPA31_RESV_1_BIT_START                          16U
#define L1DMPPA31_RESV_1_BIT_END                            31U

/* Definition for field AID5 in Register L1DMPPA31 */
#define L1DMPPA31_AID5_BIT_START                            15U
#define L1DMPPA31_AID5_BIT_END                              15U

/* Definition for field AID4 in Register L1DMPPA31 */
#define L1DMPPA31_AID4_BIT_START                            14U
#define L1DMPPA31_AID4_BIT_END                              14U

/* Definition for field AID3 in Register L1DMPPA31 */
#define L1DMPPA31_AID3_BIT_START                            13U
#define L1DMPPA31_AID3_BIT_END                              13U

/* Definition for field AID2 in Register L1DMPPA31 */
#define L1DMPPA31_AID2_BIT_START                            12U
#define L1DMPPA31_AID2_BIT_END                              12U

/* Definition for field AID1 in Register L1DMPPA31 */
#define L1DMPPA31_AID1_BIT_START                            11U
#define L1DMPPA31_AID1_BIT_END                              11U

/* Definition for field AID0 in Register L1DMPPA31 */
#define L1DMPPA31_AID0_BIT_START                            10U
#define L1DMPPA31_AID0_BIT_END                              10U

/* Definition for field AIDX in Register L1DMPPA31 */
#define L1DMPPA31_AIDX_BIT_START                            9U
#define L1DMPPA31_AIDX_BIT_END                              9U

/* Definition for field LOCAL in Register L1DMPPA31 */
#define L1DMPPA31_LOCAL_BIT_START                           8U
#define L1DMPPA31_LOCAL_BIT_END                             8U

/* Definition for field NS in Register L1DMPPA31 */
#define L1DMPPA31_NS_BIT_START                              7U
#define L1DMPPA31_NS_BIT_END                                7U

/* Definition for field EMU in Register L1DMPPA31 */
#define L1DMPPA31_EMU_BIT_START                             6U
#define L1DMPPA31_EMU_BIT_END                               6U

/* Definition for field SR in Register L1DMPPA31 */
#define L1DMPPA31_SR_BIT_START                              5U
#define L1DMPPA31_SR_BIT_END                                5U

/* Definition for field SW in Register L1DMPPA31 */
#define L1DMPPA31_SW_BIT_START                              4U
#define L1DMPPA31_SW_BIT_END                                4U

/* Definition for field SX in Register L1DMPPA31 */
#define L1DMPPA31_SX_BIT_START                              3U
#define L1DMPPA31_SX_BIT_END                                3U

/* Definition for field UR in Register L1DMPPA31 */
#define L1DMPPA31_UR_BIT_START                              2U
#define L1DMPPA31_UR_BIT_END                                2U

/* Definition for field UW in Register L1DMPPA31 */
#define L1DMPPA31_UW_BIT_START                              1U
#define L1DMPPA31_UW_BIT_END                                1U

/* Definition for field UX in Register L1DMPPA31 */
#define L1DMPPA31_UX_BIT_START                              0U
#define L1DMPPA31_UX_BIT_END                                0U



/**
 * @struct DSPICFGRegs_t
 * @brief
 *   Module DSP_ICFG Register Definition
 * @details
 *   This structure is used to access the DSP_ICFG module registers.
 */
/**
 * @typedef DSPICFGRegs
 * @brief
 *   Module DSP_ICFG Register Frame type Definition
 * @details
 *   This type is used to access the DSP_ICFG module registers.
 */
typedef volatile struct DSPICFGRegs_t
{

#if 0 /* Redundent area due to the address offset not equal to zero */
    uint32_t    RESTRICTED0[2097152]               ;        /* Offset = 0x000 */
#endif
    uint32_t    EVTFLAG0                           ;        /* Offset = 0x800000 */
    uint32_t    EVTFLAG1                           ;        /* Offset = 0x800004 */
    uint32_t    EVTFLAG2                           ;        /* Offset = 0x800008 */
    uint32_t    EVTFLAG3                           ;        /* Offset = 0x80000C */
    uint32_t    RESTRICTED1[4]                     ;        /* Offset = 0x800010 */
    uint32_t    EVTSET0                            ;        /* Offset = 0x800020 */
    uint32_t    EVTSET1                            ;        /* Offset = 0x800024 */
    uint32_t    EVTSET2                            ;        /* Offset = 0x800028 */
    uint32_t    EVTSET3                            ;        /* Offset = 0x80002C */
    uint32_t    RESTRICTED2[4]                     ;        /* Offset = 0x800030 */
    uint32_t    EVTCLR0                            ;        /* Offset = 0x800040 */
    uint32_t    EVTCLR1                            ;        /* Offset = 0x800044 */
    uint32_t    EVTCLR2                            ;        /* Offset = 0x800048 */
    uint32_t    EVTCLR3                            ;        /* Offset = 0x80004C */
    uint32_t    RESTRICTED3[12]                    ;        /* Offset = 0x800050 */
    uint32_t    EVTMASK0                           ;        /* Offset = 0x800080 */
    uint32_t    EVTMASK1                           ;        /* Offset = 0x800084 */
    uint32_t    EVTMASK2                           ;        /* Offset = 0x800088 */
    uint32_t    EVTMASK3                           ;        /* Offset = 0x80008C */
    uint32_t    RESTRICTED4[4]                     ;        /* Offset = 0x800090 */
    uint32_t    MEVTFLAG0                          ;        /* Offset = 0x8000A0 */
    uint32_t    MEVTFLAG1                          ;        /* Offset = 0x8000A4 */
    uint32_t    MEVTFLAG2                          ;        /* Offset = 0x8000A8 */
    uint32_t    MEVTFLAG3                          ;        /* Offset = 0x8000AC */
    uint32_t    RESTRICTED5[4]                     ;        /* Offset = 0x8000B0 */
    uint32_t    EXPMASK0                           ;        /* Offset = 0x8000C0 */
    uint32_t    EXPMASK1                           ;        /* Offset = 0x8000C4 */
    uint32_t    EXPMASK2                           ;        /* Offset = 0x8000C8 */
    uint32_t    EXPMASK3                           ;        /* Offset = 0x8000CC */
    uint32_t    RESTRICTED6[4]                     ;        /* Offset = 0x8000D0 */
    uint32_t    MEXPFLAG0                          ;        /* Offset = 0x8000E0 */
    uint32_t    MEXPFLAG1                          ;        /* Offset = 0x8000E4 */
    uint32_t    MEXPFLAG2                          ;        /* Offset = 0x8000E8 */
    uint32_t    MEXPFLAG3                          ;        /* Offset = 0x8000EC */
    uint32_t    RESTRICTED7[5]                     ;        /* Offset = 0x8000F0 */
    uint32_t    INTMUX1                            ;        /* Offset = 0x800104 */
    uint32_t    INTMUX2                            ;        /* Offset = 0x800108 */
    uint32_t    INTMUX3                            ;        /* Offset = 0x80010C */
    uint32_t    RESTRICTED8[12]                    ;        /* Offset = 0x800110 */
    uint32_t    AEGMUX0                            ;        /* Offset = 0x800140 */
    uint32_t    AEGMUX1                            ;        /* Offset = 0x800144 */
    uint32_t    RESTRICTED9[14]                    ;        /* Offset = 0x800148 */
    uint32_t    INTXSTAT                           ;        /* Offset = 0x800180 */
    uint32_t    INTXCLR                            ;        /* Offset = 0x800184 */
    uint32_t    INTDMASK                           ;        /* Offset = 0x800188 */
    uint32_t    RESTRICTED10[13]                   ;        /* Offset = 0x80018C */
    uint32_t    EVTASRT                            ;        /* Offset = 0x8001C0 */
    uint32_t    RESTRICTED11[16271]                ;        /* Offset = 0x8001C4 */
    uint32_t    PDCCMD                             ;        /* Offset = 0x810000 */
    uint32_t    RESTRICTED12[2047]                 ;        /* Offset = 0x810004 */
    uint32_t    MM_REVID                           ;        /* Offset = 0x812000 */
    uint32_t    RESTRICTED13[14335]                ;        /* Offset = 0x812004 */
    uint32_t    IDMA0_STAT                         ;        /* Offset = 0x820000 */
    uint32_t    IDMA0_MASK                         ;        /* Offset = 0x820004 */
    uint32_t    IDMA0_SOURCE                       ;        /* Offset = 0x820008 */
    uint32_t    IDMA0_DEST                         ;        /* Offset = 0x82000C */
    uint32_t    IDMA0_COUNT                        ;        /* Offset = 0x820010 */
    uint32_t    RESTRICTED14[59]                   ;        /* Offset = 0x820014 */
    uint32_t    IDMA1_STAT                         ;        /* Offset = 0x820100 */
    uint32_t    RESTRICTED15                       ;        /* Offset = 0x820104 */
    uint32_t    IDMA1_SOURCE                       ;        /* Offset = 0x820108 */
    uint32_t    IDMA1_DEST                         ;        /* Offset = 0x82010C */
    uint32_t    IDMA1_COUNT                        ;        /* Offset = 0x820110 */
    uint32_t    RESTRICTED16[59]                   ;        /* Offset = 0x820114 */
    uint32_t    CPUARBE                            ;        /* Offset = 0x820200 */
    uint32_t    IDMAARBE                           ;        /* Offset = 0x820204 */
    uint32_t    SDMAARBE                           ;        /* Offset = 0x820208 */
    uint32_t    RESTRICTED17                       ;        /* Offset = 0x82020C */
    uint32_t    ECFGARBE                           ;        /* Offset = 0x820210 */
    uint32_t    RESTRICTED18[59]                   ;        /* Offset = 0x820214 */
    uint32_t    ICFGMPFAR                          ;        /* Offset = 0x820300 */
    uint32_t    ICFGMPFSR                          ;        /* Offset = 0x820304 */
    uint32_t    ICFGMPFCR                          ;        /* Offset = 0x820308 */
    uint32_t    RESTRICTED19[63]                   ;        /* Offset = 0x82030C */
    uint32_t    ECFGERR                            ;        /* Offset = 0x820408 */
    uint32_t    ECFGERRCLR                         ;        /* Offset = 0x82040C */
    uint32_t    RESTRICTED20[60]                   ;        /* Offset = 0x820410 */
    uint32_t    PAMAP0                             ;        /* Offset = 0x820500 */
    uint32_t    PAMAP1                             ;        /* Offset = 0x820504 */
    uint32_t    PAMAP2                             ;        /* Offset = 0x820508 */
    uint32_t    PAMAP3                             ;        /* Offset = 0x82050C */
    uint32_t    PAMAP4                             ;        /* Offset = 0x820510 */
    uint32_t    PAMAP5                             ;        /* Offset = 0x820514 */
    uint32_t    PAMAP6                             ;        /* Offset = 0x820518 */
    uint32_t    PAMAP7                             ;        /* Offset = 0x82051C */
    uint32_t    PAMAP8                             ;        /* Offset = 0x820520 */
    uint32_t    PAMAP9                             ;        /* Offset = 0x820524 */
    uint32_t    PAMAP10                            ;        /* Offset = 0x820528 */
    uint32_t    PAMAP11                            ;        /* Offset = 0x82052C */
    uint32_t    PAMAP12                            ;        /* Offset = 0x820530 */
    uint32_t    PAMAP13                            ;        /* Offset = 0x820534 */
    uint32_t    PAMAP14                            ;        /* Offset = 0x820538 */
    uint32_t    PAMAP15                            ;        /* Offset = 0x82053C */
    uint32_t    RESTRICTED21[32432]                ;        /* Offset = 0x820540 */
    uint32_t    L2CFG                              ;        /* Offset = 0x840000 */
    uint32_t    RESTRICTED22[7]                    ;        /* Offset = 0x840004 */
    uint32_t    L1PCFG                             ;        /* Offset = 0x840020 */
    uint32_t    L1PCC                              ;        /* Offset = 0x840024 */
    uint32_t    RESTRICTED23[6]                    ;        /* Offset = 0x840028 */
    uint32_t    L1DCFG                             ;        /* Offset = 0x840040 */
    uint32_t    L1DCC                              ;        /* Offset = 0x840044 */
    uint32_t    RESTRICTED24[1006]                 ;        /* Offset = 0x840048 */
    uint32_t    CPUARBU                            ;        /* Offset = 0x841000 */
    uint32_t    IDMAARBU                           ;        /* Offset = 0x841004 */
    uint32_t    SDMAARBU                           ;        /* Offset = 0x841008 */
    uint32_t    UCARBU                             ;        /* Offset = 0x84100C */
    uint32_t    MDMAARBU                           ;        /* Offset = 0x841010 */
    uint32_t    RESTRICTED25[11]                   ;        /* Offset = 0x841014 */
    uint32_t    CPUARBD                            ;        /* Offset = 0x841040 */
    uint32_t    IDMAARBD                           ;        /* Offset = 0x841044 */
    uint32_t    SDMAARBD                           ;        /* Offset = 0x841048 */
    uint32_t    UCARBD                             ;        /* Offset = 0x84104C */
    uint32_t    RESTRICTED26[3052]                 ;        /* Offset = 0x841050 */
    uint32_t    L2WBAR                             ;        /* Offset = 0x844000 */
    uint32_t    L2WWC                              ;        /* Offset = 0x844004 */
    uint32_t    RESTRICTED27[2]                    ;        /* Offset = 0x844008 */
    uint32_t    L2WIBAR                            ;        /* Offset = 0x844010 */
    uint32_t    L2WIWC                             ;        /* Offset = 0x844014 */
    uint32_t    L2IBAR                             ;        /* Offset = 0x844018 */
    uint32_t    L2IWC                              ;        /* Offset = 0x84401C */
    uint32_t    L1PIBAR                            ;        /* Offset = 0x844020 */
    uint32_t    L1PIWC                             ;        /* Offset = 0x844024 */
    uint32_t    RESTRICTED28[2]                    ;        /* Offset = 0x844028 */
    uint32_t    L1DWIBAR                           ;        /* Offset = 0x844030 */
    uint32_t    L1DWIWC                            ;        /* Offset = 0x844034 */
    uint32_t    RESTRICTED29[2]                    ;        /* Offset = 0x844038 */
    uint32_t    L1DWBAR                            ;        /* Offset = 0x844040 */
    uint32_t    L1DWWC                             ;        /* Offset = 0x844044 */
    uint32_t    L1DIBAR                            ;        /* Offset = 0x844048 */
    uint32_t    L1DIWC                             ;        /* Offset = 0x84404C */
    uint32_t    RESTRICTED30[1004]                 ;        /* Offset = 0x844050 */
    uint32_t    L2WB                               ;        /* Offset = 0x845000 */
    uint32_t    L2WBINV                            ;        /* Offset = 0x845004 */
    uint32_t    L2INV                              ;        /* Offset = 0x845008 */
    uint32_t    RESTRICTED31[7]                    ;        /* Offset = 0x84500C */
    uint32_t    L1PINV                             ;        /* Offset = 0x845028 */
    uint32_t    RESTRICTED32[5]                    ;        /* Offset = 0x84502C */
    uint32_t    L1DWB                              ;        /* Offset = 0x845040 */
    uint32_t    L1DWBINV                           ;        /* Offset = 0x845044 */
    uint32_t    L1DINV                             ;        /* Offset = 0x845048 */
    uint32_t    RESTRICTED33[1006]                 ;        /* Offset = 0x84504C */
    uint32_t    L2EDSTAT                           ;        /* Offset = 0x846004 */
    uint32_t    L2EDCMD                            ;        /* Offset = 0x846008 */
    uint32_t    L2EDADDR                           ;        /* Offset = 0x84600C */
    uint32_t    L2EDPEN0                           ;        /* Offset = 0x846010 */
    uint32_t    L2EDPEN1                           ;        /* Offset = 0x846014 */
    uint32_t    L2EDCPEC                           ;        /* Offset = 0x846018 */
    uint32_t    L2EDCNEC                           ;        /* Offset = 0x84601C */
    uint32_t    MDMAERR                            ;        /* Offset = 0x846020 */
    uint32_t    MDMAERRCLR                         ;        /* Offset = 0x846024 */
    uint32_t    RESTRICTED34[2]                    ;        /* Offset = 0x846028 */
    uint32_t    L2EDCEN                            ;        /* Offset = 0x846030 */
    uint32_t    RESTRICTED35[244]                  ;        /* Offset = 0x846034 */
    uint32_t    L1PEDSTAT                          ;        /* Offset = 0x846404 */
    uint32_t    L1PEDCMD                           ;        /* Offset = 0x846408 */
    uint32_t    L1PEDADDR                          ;        /* Offset = 0x84640C */
    uint32_t    RESTRICTED36[1788]                 ;        /* Offset = 0x846410 */
    uint32_t    MAR_0                              ;        /* Offset = 0x848000 */
    uint32_t    MAR_1                              ;        /* Offset = 0x848004 */
    uint32_t    MAR_2                              ;        /* Offset = 0x848008 */
    uint32_t    MAR_3                              ;        /* Offset = 0x84800C */
    uint32_t    MAR_4                              ;        /* Offset = 0x848010 */
    uint32_t    MAR_5                              ;        /* Offset = 0x848014 */
    uint32_t    MAR_6                              ;        /* Offset = 0x848018 */
    uint32_t    MAR_7                              ;        /* Offset = 0x84801C */
    uint32_t    MAR_8                              ;        /* Offset = 0x848020 */
    uint32_t    MAR_9                              ;        /* Offset = 0x848024 */
    uint32_t    MAR_10                             ;        /* Offset = 0x848028 */
    uint32_t    MAR_11                             ;        /* Offset = 0x84802C */
    uint32_t    MAR_12                             ;        /* Offset = 0x848030 */
    uint32_t    MAR_13                             ;        /* Offset = 0x848034 */
    uint32_t    MAR_14                             ;        /* Offset = 0x848038 */
    uint32_t    MAR_15                             ;        /* Offset = 0x84803C */
    uint32_t    MAR_16                             ;        /* Offset = 0x848040 */
    uint32_t    MAR_17                             ;        /* Offset = 0x848044 */
    uint32_t    MAR_18                             ;        /* Offset = 0x848048 */
    uint32_t    MAR_19                             ;        /* Offset = 0x84804C */
    uint32_t    MAR_20                             ;        /* Offset = 0x848050 */
    uint32_t    MAR_21                             ;        /* Offset = 0x848054 */
    uint32_t    MAR_22                             ;        /* Offset = 0x848058 */
    uint32_t    MAR_23                             ;        /* Offset = 0x84805C */
    uint32_t    MAR_24                             ;        /* Offset = 0x848060 */
    uint32_t    MAR_25                             ;        /* Offset = 0x848064 */
    uint32_t    MAR_26                             ;        /* Offset = 0x848068 */
    uint32_t    MAR_27                             ;        /* Offset = 0x84806C */
    uint32_t    MAR_28                             ;        /* Offset = 0x848070 */
    uint32_t    MAR_29                             ;        /* Offset = 0x848074 */
    uint32_t    MAR_30                             ;        /* Offset = 0x848078 */
    uint32_t    MAR_31                             ;        /* Offset = 0x84807C */
    uint32_t    MAR_32                             ;        /* Offset = 0x848080 */
    uint32_t    MAR_33                             ;        /* Offset = 0x848084 */
    uint32_t    MAR_34                             ;        /* Offset = 0x848088 */
    uint32_t    MAR_35                             ;        /* Offset = 0x84808C */
    uint32_t    MAR_36                             ;        /* Offset = 0x848090 */
    uint32_t    MAR_37                             ;        /* Offset = 0x848094 */
    uint32_t    MAR_38                             ;        /* Offset = 0x848098 */
    uint32_t    MAR_39                             ;        /* Offset = 0x84809C */
    uint32_t    MAR_40                             ;        /* Offset = 0x8480A0 */
    uint32_t    MAR_41                             ;        /* Offset = 0x8480A4 */
    uint32_t    MAR_42                             ;        /* Offset = 0x8480A8 */
    uint32_t    MAR_43                             ;        /* Offset = 0x8480AC */
    uint32_t    MAR_44                             ;        /* Offset = 0x8480B0 */
    uint32_t    MAR_45                             ;        /* Offset = 0x8480B4 */
    uint32_t    MAR_46                             ;        /* Offset = 0x8480B8 */
    uint32_t    MAR_47                             ;        /* Offset = 0x8480BC */
    uint32_t    MAR_48                             ;        /* Offset = 0x8480C0 */
    uint32_t    MAR_49                             ;        /* Offset = 0x8480C4 */
    uint32_t    MAR_50                             ;        /* Offset = 0x8480C8 */
    uint32_t    MAR_51                             ;        /* Offset = 0x8480CC */
    uint32_t    MAR_52                             ;        /* Offset = 0x8480D0 */
    uint32_t    MAR_53                             ;        /* Offset = 0x8480D4 */
    uint32_t    MAR_54                             ;        /* Offset = 0x8480D8 */
    uint32_t    MAR_55                             ;        /* Offset = 0x8480DC */
    uint32_t    MAR_56                             ;        /* Offset = 0x8480E0 */
    uint32_t    MAR_57                             ;        /* Offset = 0x8480E4 */
    uint32_t    MAR_58                             ;        /* Offset = 0x8480E8 */
    uint32_t    MAR_59                             ;        /* Offset = 0x8480EC */
    uint32_t    MAR_60                             ;        /* Offset = 0x8480F0 */
    uint32_t    MAR_61                             ;        /* Offset = 0x8480F4 */
    uint32_t    MAR_62                             ;        /* Offset = 0x8480F8 */
    uint32_t    MAR_63                             ;        /* Offset = 0x8480FC */
    uint32_t    MAR_64                             ;        /* Offset = 0x848100 */
    uint32_t    MAR_65                             ;        /* Offset = 0x848104 */
    uint32_t    MAR_66                             ;        /* Offset = 0x848108 */
    uint32_t    MAR_67                             ;        /* Offset = 0x84810C */
    uint32_t    MAR_68                             ;        /* Offset = 0x848110 */
    uint32_t    MAR_69                             ;        /* Offset = 0x848114 */
    uint32_t    MAR_70                             ;        /* Offset = 0x848118 */
    uint32_t    MAR_71                             ;        /* Offset = 0x84811C */
    uint32_t    MAR_72                             ;        /* Offset = 0x848120 */
    uint32_t    MAR_73                             ;        /* Offset = 0x848124 */
    uint32_t    MAR_74                             ;        /* Offset = 0x848128 */
    uint32_t    MAR_75                             ;        /* Offset = 0x84812C */
    uint32_t    MAR_76                             ;        /* Offset = 0x848130 */
    uint32_t    MAR_77                             ;        /* Offset = 0x848134 */
    uint32_t    MAR_78                             ;        /* Offset = 0x848138 */
    uint32_t    MAR_79                             ;        /* Offset = 0x84813C */
    uint32_t    MAR_80                             ;        /* Offset = 0x848140 */
    uint32_t    MAR_81                             ;        /* Offset = 0x848144 */
    uint32_t    MAR_82                             ;        /* Offset = 0x848148 */
    uint32_t    MAR_83                             ;        /* Offset = 0x84814C */
    uint32_t    MAR_84                             ;        /* Offset = 0x848150 */
    uint32_t    MAR_85                             ;        /* Offset = 0x848154 */
    uint32_t    MAR_86                             ;        /* Offset = 0x848158 */
    uint32_t    MAR_87                             ;        /* Offset = 0x84815C */
    uint32_t    MAR_88                             ;        /* Offset = 0x848160 */
    uint32_t    MAR_89                             ;        /* Offset = 0x848164 */
    uint32_t    MAR_90                             ;        /* Offset = 0x848168 */
    uint32_t    MAR_91                             ;        /* Offset = 0x84816C */
    uint32_t    MAR_92                             ;        /* Offset = 0x848170 */
    uint32_t    MAR_93                             ;        /* Offset = 0x848174 */
    uint32_t    MAR_94                             ;        /* Offset = 0x848178 */
    uint32_t    MAR_95                             ;        /* Offset = 0x84817C */
    uint32_t    MAR_96                             ;        /* Offset = 0x848180 */
    uint32_t    MAR_97                             ;        /* Offset = 0x848184 */
    uint32_t    MAR_98                             ;        /* Offset = 0x848188 */
    uint32_t    MAR_99                             ;        /* Offset = 0x84818C */
    uint32_t    MAR_100                            ;        /* Offset = 0x848190 */
    uint32_t    MAR_101                            ;        /* Offset = 0x848194 */
    uint32_t    MAR_102                            ;        /* Offset = 0x848198 */
    uint32_t    MAR_103                            ;        /* Offset = 0x84819C */
    uint32_t    MAR_104                            ;        /* Offset = 0x8481A0 */
    uint32_t    MAR_105                            ;        /* Offset = 0x8481A4 */
    uint32_t    MAR_106                            ;        /* Offset = 0x8481A8 */
    uint32_t    MAR_107                            ;        /* Offset = 0x8481AC */
    uint32_t    MAR_108                            ;        /* Offset = 0x8481B0 */
    uint32_t    MAR_109                            ;        /* Offset = 0x8481B4 */
    uint32_t    MAR_110                            ;        /* Offset = 0x8481B8 */
    uint32_t    MAR_111                            ;        /* Offset = 0x8481BC */
    uint32_t    MAR_112                            ;        /* Offset = 0x8481C0 */
    uint32_t    MAR_113                            ;        /* Offset = 0x8481C4 */
    uint32_t    MAR_114                            ;        /* Offset = 0x8481C8 */
    uint32_t    MAR_115                            ;        /* Offset = 0x8481CC */
    uint32_t    MAR_116                            ;        /* Offset = 0x8481D0 */
    uint32_t    MAR_117                            ;        /* Offset = 0x8481D4 */
    uint32_t    MAR_118                            ;        /* Offset = 0x8481D8 */
    uint32_t    MAR_119                            ;        /* Offset = 0x8481DC */
    uint32_t    MAR_120                            ;        /* Offset = 0x8481E0 */
    uint32_t    MAR_121                            ;        /* Offset = 0x8481E4 */
    uint32_t    MAR_122                            ;        /* Offset = 0x8481E8 */
    uint32_t    MAR_123                            ;        /* Offset = 0x8481EC */
    uint32_t    MAR_124                            ;        /* Offset = 0x8481F0 */
    uint32_t    MAR_125                            ;        /* Offset = 0x8481F4 */
    uint32_t    MAR_126                            ;        /* Offset = 0x8481F8 */
    uint32_t    MAR_127                            ;        /* Offset = 0x8481FC */
    uint32_t    MAR_128                            ;        /* Offset = 0x848200 */
    uint32_t    MAR_129                            ;        /* Offset = 0x848204 */
    uint32_t    MAR_130                            ;        /* Offset = 0x848208 */
    uint32_t    MAR_131                            ;        /* Offset = 0x84820C */
    uint32_t    MAR_132                            ;        /* Offset = 0x848210 */
    uint32_t    MAR_133                            ;        /* Offset = 0x848214 */
    uint32_t    MAR_134                            ;        /* Offset = 0x848218 */
    uint32_t    MAR_135                            ;        /* Offset = 0x84821C */
    uint32_t    MAR_136                            ;        /* Offset = 0x848220 */
    uint32_t    MAR_137                            ;        /* Offset = 0x848224 */
    uint32_t    MAR_138                            ;        /* Offset = 0x848228 */
    uint32_t    MAR_139                            ;        /* Offset = 0x84822C */
    uint32_t    MAR_140                            ;        /* Offset = 0x848230 */
    uint32_t    MAR_141                            ;        /* Offset = 0x848234 */
    uint32_t    MAR_142                            ;        /* Offset = 0x848238 */
    uint32_t    MAR_143                            ;        /* Offset = 0x84823C */
    uint32_t    MAR_144                            ;        /* Offset = 0x848240 */
    uint32_t    MAR_145                            ;        /* Offset = 0x848244 */
    uint32_t    MAR_146                            ;        /* Offset = 0x848248 */
    uint32_t    MAR_147                            ;        /* Offset = 0x84824C */
    uint32_t    MAR_148                            ;        /* Offset = 0x848250 */
    uint32_t    MAR_149                            ;        /* Offset = 0x848254 */
    uint32_t    MAR_150                            ;        /* Offset = 0x848258 */
    uint32_t    MAR_151                            ;        /* Offset = 0x84825C */
    uint32_t    MAR_152                            ;        /* Offset = 0x848260 */
    uint32_t    MAR_153                            ;        /* Offset = 0x848264 */
    uint32_t    MAR_154                            ;        /* Offset = 0x848268 */
    uint32_t    MAR_155                            ;        /* Offset = 0x84826C */
    uint32_t    MAR_156                            ;        /* Offset = 0x848270 */
    uint32_t    MAR_157                            ;        /* Offset = 0x848274 */
    uint32_t    MAR_158                            ;        /* Offset = 0x848278 */
    uint32_t    MAR_159                            ;        /* Offset = 0x84827C */
    uint32_t    MAR_160                            ;        /* Offset = 0x848280 */
    uint32_t    MAR_161                            ;        /* Offset = 0x848284 */
    uint32_t    MAR_162                            ;        /* Offset = 0x848288 */
    uint32_t    MAR_163                            ;        /* Offset = 0x84828C */
    uint32_t    MAR_164                            ;        /* Offset = 0x848290 */
    uint32_t    MAR_165                            ;        /* Offset = 0x848294 */
    uint32_t    MAR_166                            ;        /* Offset = 0x848298 */
    uint32_t    MAR_167                            ;        /* Offset = 0x84829C */
    uint32_t    MAR_168                            ;        /* Offset = 0x8482A0 */
    uint32_t    MAR_169                            ;        /* Offset = 0x8482A4 */
    uint32_t    MAR_170                            ;        /* Offset = 0x8482A8 */
    uint32_t    MAR_171                            ;        /* Offset = 0x8482AC */
    uint32_t    MAR_172                            ;        /* Offset = 0x8482B0 */
    uint32_t    MAR_173                            ;        /* Offset = 0x8482B4 */
    uint32_t    MAR_174                            ;        /* Offset = 0x8482B8 */
    uint32_t    MAR_175                            ;        /* Offset = 0x8482BC */
    uint32_t    MAR_176                            ;        /* Offset = 0x8482C0 */
    uint32_t    MAR_177                            ;        /* Offset = 0x8482C4 */
    uint32_t    MAR_178                            ;        /* Offset = 0x8482C8 */
    uint32_t    MAR_179                            ;        /* Offset = 0x8482CC */
    uint32_t    MAR_180                            ;        /* Offset = 0x8482D0 */
    uint32_t    MAR_181                            ;        /* Offset = 0x8482D4 */
    uint32_t    MAR_182                            ;        /* Offset = 0x8482D8 */
    uint32_t    MAR_183                            ;        /* Offset = 0x8482DC */
    uint32_t    MAR_184                            ;        /* Offset = 0x8482E0 */
    uint32_t    MAR_185                            ;        /* Offset = 0x8482E4 */
    uint32_t    MAR_186                            ;        /* Offset = 0x8482E8 */
    uint32_t    MAR_187                            ;        /* Offset = 0x8482EC */
    uint32_t    MAR_188                            ;        /* Offset = 0x8482F0 */
    uint32_t    MAR_189                            ;        /* Offset = 0x8482F4 */
    uint32_t    MAR_190                            ;        /* Offset = 0x8482F8 */
    uint32_t    MAR_191                            ;        /* Offset = 0x8482FC */
    uint32_t    MAR_192                            ;        /* Offset = 0x848300 */
    uint32_t    MAR_193                            ;        /* Offset = 0x848304 */
    uint32_t    MAR_194                            ;        /* Offset = 0x848308 */
    uint32_t    MAR_195                            ;        /* Offset = 0x84830C */
    uint32_t    MAR_196                            ;        /* Offset = 0x848310 */
    uint32_t    MAR_197                            ;        /* Offset = 0x848314 */
    uint32_t    MAR_198                            ;        /* Offset = 0x848318 */
    uint32_t    MAR_199                            ;        /* Offset = 0x84831C */
    uint32_t    MAR_200                            ;        /* Offset = 0x848320 */
    uint32_t    MAR_201                            ;        /* Offset = 0x848324 */
    uint32_t    MAR_202                            ;        /* Offset = 0x848328 */
    uint32_t    MAR_203                            ;        /* Offset = 0x84832C */
    uint32_t    MAR_204                            ;        /* Offset = 0x848330 */
    uint32_t    MAR_205                            ;        /* Offset = 0x848334 */
    uint32_t    MAR_206                            ;        /* Offset = 0x848338 */
    uint32_t    MAR_207                            ;        /* Offset = 0x84833C */
    uint32_t    MAR_208                            ;        /* Offset = 0x848340 */
    uint32_t    MAR_209                            ;        /* Offset = 0x848344 */
    uint32_t    MAR_210                            ;        /* Offset = 0x848348 */
    uint32_t    MAR_211                            ;        /* Offset = 0x84834C */
    uint32_t    MAR_212                            ;        /* Offset = 0x848350 */
    uint32_t    MAR_213                            ;        /* Offset = 0x848354 */
    uint32_t    MAR_214                            ;        /* Offset = 0x848358 */
    uint32_t    MAR_215                            ;        /* Offset = 0x84835C */
    uint32_t    MAR_216                            ;        /* Offset = 0x848360 */
    uint32_t    MAR_217                            ;        /* Offset = 0x848364 */
    uint32_t    MAR_218                            ;        /* Offset = 0x848368 */
    uint32_t    MAR_219                            ;        /* Offset = 0x84836C */
    uint32_t    MAR_220                            ;        /* Offset = 0x848370 */
    uint32_t    MAR_221                            ;        /* Offset = 0x848374 */
    uint32_t    MAR_222                            ;        /* Offset = 0x848378 */
    uint32_t    MAR_223                            ;        /* Offset = 0x84837C */
    uint32_t    MAR_224                            ;        /* Offset = 0x848380 */
    uint32_t    MAR_225                            ;        /* Offset = 0x848384 */
    uint32_t    MAR_226                            ;        /* Offset = 0x848388 */
    uint32_t    MAR_227                            ;        /* Offset = 0x84838C */
    uint32_t    MAR_228                            ;        /* Offset = 0x848390 */
    uint32_t    MAR_229                            ;        /* Offset = 0x848394 */
    uint32_t    MAR_230                            ;        /* Offset = 0x848398 */
    uint32_t    MAR_231                            ;        /* Offset = 0x84839C */
    uint32_t    MAR_232                            ;        /* Offset = 0x8483A0 */
    uint32_t    MAR_233                            ;        /* Offset = 0x8483A4 */
    uint32_t    MAR_234                            ;        /* Offset = 0x8483A8 */
    uint32_t    MAR_235                            ;        /* Offset = 0x8483AC */
    uint32_t    MAR_236                            ;        /* Offset = 0x8483B0 */
    uint32_t    MAR_237                            ;        /* Offset = 0x8483B4 */
    uint32_t    MAR_238                            ;        /* Offset = 0x8483B8 */
    uint32_t    MAR_239                            ;        /* Offset = 0x8483BC */
    uint32_t    MAR_240                            ;        /* Offset = 0x8483C0 */
    uint32_t    MAR_241                            ;        /* Offset = 0x8483C4 */
    uint32_t    MAR_242                            ;        /* Offset = 0x8483C8 */
    uint32_t    MAR_243                            ;        /* Offset = 0x8483CC */
    uint32_t    MAR_244                            ;        /* Offset = 0x8483D0 */
    uint32_t    MAR_245                            ;        /* Offset = 0x8483D4 */
    uint32_t    MAR_246                            ;        /* Offset = 0x8483D8 */
    uint32_t    MAR_247                            ;        /* Offset = 0x8483DC */
    uint32_t    MAR_248                            ;        /* Offset = 0x8483E0 */
    uint32_t    MAR_249                            ;        /* Offset = 0x8483E4 */
    uint32_t    MAR_250                            ;        /* Offset = 0x8483E8 */
    uint32_t    MAR_251                            ;        /* Offset = 0x8483EC */
    uint32_t    MAR_252                            ;        /* Offset = 0x8483F0 */
    uint32_t    MAR_253                            ;        /* Offset = 0x8483F4 */
    uint32_t    MAR_254                            ;        /* Offset = 0x8483F8 */
    uint32_t    MAR_255                            ;        /* Offset = 0x8483FC */
    uint32_t    RESTRICTED37[1792]                 ;        /* Offset = 0x848400 */
    uint32_t    L2MPFAR                            ;        /* Offset = 0x84A000 */
    uint32_t    L2MPFSR                            ;        /* Offset = 0x84A004 */
    uint32_t    L2MPFCR                            ;        /* Offset = 0x84A008 */
    uint32_t    RESTRICTED38[125]                  ;        /* Offset = 0x84A00C */
    uint32_t    L2MPPA0                            ;        /* Offset = 0x84A200 */
    uint32_t    L2MPPA1                            ;        /* Offset = 0x84A204 */
    uint32_t    L2MPPA2                            ;        /* Offset = 0x84A208 */
    uint32_t    L2MPPA3                            ;        /* Offset = 0x84A20C */
    uint32_t    L2MPPA4                            ;        /* Offset = 0x84A210 */
    uint32_t    L2MPPA5                            ;        /* Offset = 0x84A214 */
    uint32_t    L2MPPA6                            ;        /* Offset = 0x84A218 */
    uint32_t    L2MPPA7                            ;        /* Offset = 0x84A21C */
    uint32_t    L2MPPA8                            ;        /* Offset = 0x84A220 */
    uint32_t    L2MPPA9                            ;        /* Offset = 0x84A224 */
    uint32_t    L2MPPA10                           ;        /* Offset = 0x84A228 */
    uint32_t    L2MPPA11                           ;        /* Offset = 0x84A22C */
    uint32_t    L2MPPA12                           ;        /* Offset = 0x84A230 */
    uint32_t    L2MPPA13                           ;        /* Offset = 0x84A234 */
    uint32_t    L2MPPA14                           ;        /* Offset = 0x84A238 */
    uint32_t    L2MPPA15                           ;        /* Offset = 0x84A23C */
    uint32_t    L2MPPA16                           ;        /* Offset = 0x84A240 */
    uint32_t    L2MPPA17                           ;        /* Offset = 0x84A244 */
    uint32_t    L2MPPA18                           ;        /* Offset = 0x84A248 */
    uint32_t    L2MPPA19                           ;        /* Offset = 0x84A24C */
    uint32_t    L2MPPA20                           ;        /* Offset = 0x84A250 */
    uint32_t    L2MPPA21                           ;        /* Offset = 0x84A254 */
    uint32_t    L2MPPA22                           ;        /* Offset = 0x84A258 */
    uint32_t    L2MPPA23                           ;        /* Offset = 0x84A25C */
    uint32_t    L2MPPA24                           ;        /* Offset = 0x84A260 */
    uint32_t    L2MPPA25                           ;        /* Offset = 0x84A264 */
    uint32_t    L2MPPA26                           ;        /* Offset = 0x84A268 */
    uint32_t    L2MPPA27                           ;        /* Offset = 0x84A26C */
    uint32_t    L2MPPA28                           ;        /* Offset = 0x84A270 */
    uint32_t    L2MPPA29                           ;        /* Offset = 0x84A274 */
    uint32_t    L2MPPA30                           ;        /* Offset = 0x84A278 */
    uint32_t    L2MPPA31                           ;        /* Offset = 0x84A27C */
    uint32_t    RESTRICTED39[96]                   ;        /* Offset = 0x84A280 */
    uint32_t    L1PMPFAR                           ;        /* Offset = 0x84A400 */
    uint32_t    L1PMPFSR                           ;        /* Offset = 0x84A404 */
    uint32_t    L1PMPFCR                           ;        /* Offset = 0x84A408 */
    uint32_t    RESTRICTED40[141]                  ;        /* Offset = 0x84A40C */
    uint32_t    L1PMPPA16                          ;        /* Offset = 0x84A640 */
    uint32_t    L1PMPPA17                          ;        /* Offset = 0x84A644 */
    uint32_t    L1PMPPA18                          ;        /* Offset = 0x84A648 */
    uint32_t    L1PMPPA19                          ;        /* Offset = 0x84A64C */
    uint32_t    L1PMPPA20                          ;        /* Offset = 0x84A650 */
    uint32_t    L1PMPPA21                          ;        /* Offset = 0x84A654 */
    uint32_t    L1PMPPA22                          ;        /* Offset = 0x84A658 */
    uint32_t    L1PMPPA23                          ;        /* Offset = 0x84A65C */
    uint32_t    L1PMPPA24                          ;        /* Offset = 0x84A660 */
    uint32_t    L1PMPPA25                          ;        /* Offset = 0x84A664 */
    uint32_t    L1PMPPA26                          ;        /* Offset = 0x84A668 */
    uint32_t    L1PMPPA27                          ;        /* Offset = 0x84A66C */
    uint32_t    L1PMPPA28                          ;        /* Offset = 0x84A670 */
    uint32_t    L1PMPPA29                          ;        /* Offset = 0x84A674 */
    uint32_t    L1PMPPA30                          ;        /* Offset = 0x84A678 */
    uint32_t    L1PMPPA31                          ;        /* Offset = 0x84A67C */
    uint32_t    RESTRICTED41[352]                  ;        /* Offset = 0x84A680 */
    uint32_t    L1DMPFAR                           ;        /* Offset = 0x84AC00 */
    uint32_t    L1DMPFSR                           ;        /* Offset = 0x84AC04 */
    uint32_t    L1DMPFCR                           ;        /* Offset = 0x84AC08 */
    uint32_t    RESTRICTED42[61]                   ;        /* Offset = 0x84AC0C */
    uint32_t    MPLK0                              ;        /* Offset = 0x84AD00 */
    uint32_t    MPLK1                              ;        /* Offset = 0x84AD04 */
    uint32_t    MPLK2                              ;        /* Offset = 0x84AD08 */
    uint32_t    MPLK3                              ;        /* Offset = 0x84AD0C */
    uint32_t    MPLKCMD                            ;        /* Offset = 0x84AD10 */
    uint32_t    MPLKSTAT                           ;        /* Offset = 0x84AD14 */
    uint32_t    RESTRICTED43[74]                   ;        /* Offset = 0x84AD18 */
    uint32_t    L1DMPPA16                          ;        /* Offset = 0x84AE40 */
    uint32_t    L1DMPPA17                          ;        /* Offset = 0x84AE44 */
    uint32_t    L1DMPPA18                          ;        /* Offset = 0x84AE48 */
    uint32_t    L1DMPPA19                          ;        /* Offset = 0x84AE4C */
    uint32_t    L1DMPPA20                          ;        /* Offset = 0x84AE50 */
    uint32_t    L1DMPPA21                          ;        /* Offset = 0x84AE54 */
    uint32_t    L1DMPPA22                          ;        /* Offset = 0x84AE58 */
    uint32_t    L1DMPPA23                          ;        /* Offset = 0x84AE5C */
    uint32_t    L1DMPPA24                          ;        /* Offset = 0x84AE60 */
    uint32_t    L1DMPPA25                          ;        /* Offset = 0x84AE64 */
    uint32_t    L1DMPPA26                          ;        /* Offset = 0x84AE68 */
    uint32_t    L1DMPPA27                          ;        /* Offset = 0x84AE6C */
    uint32_t    L1DMPPA28                          ;        /* Offset = 0x84AE70 */
    uint32_t    L1DMPPA29                          ;        /* Offset = 0x84AE74 */
    uint32_t    L1DMPPA30                          ;        /* Offset = 0x84AE78 */
    uint32_t    L1DMPPA31                          ;        /* Offset = 0x84AE7C */
} DSPICFGRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_DSPICFG_H */
/* END OF REG_DSPICFG_H */

