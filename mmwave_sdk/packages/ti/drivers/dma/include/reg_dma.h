/**
 *  @file    reg_dma.h
 *
 *  @brief
 *    This file gives register definitions of MSS_DMA_REG module.
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

#ifndef REG_DMA_H
#define REG_DMA_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field DMARES in Register GCTRL */
#define GCTRL_DMARES_BIT_START                              0U
#define GCTRL_DMARES_BIT_END                                0U

/* Definition for field DEBUGMODE in Register GCTRL */
#define GCTRL_DEBUGMODE_BIT_START                           8U
#define GCTRL_DEBUGMODE_BIT_END                             9U

/* Definition for field BUSBUSY in Register GCTRL */
#define GCTRL_BUSBUSY_BIT_START                             14U
#define GCTRL_BUSBUSY_BIT_END                               14U

/* Definition for field DMAEN in Register GCTRL */
#define GCTRL_DMAEN_BIT_START                               16U
#define GCTRL_DMAEN_BIT_END                                 16U

/* Definition for field PEND in Register PEND */
#define PEND_PEND_BIT_START                                 0U
#define PEND_PEND_BIT_END                                   31U

/* Definition for field VBUSPFB in Register FBREG */
#define FBREG_VBUSPFB_BIT_START                             0U
#define FBREG_VBUSPFB_BIT_END                               3U

/* Definition for field FSMFB in Register FBREG */
#define FBREG_FSMFB_BIT_START                               8U
#define FBREG_FSMFB_BIT_END                                 11U

/* Definition for field STCH in Register DMASTAT */
#define DMASTAT_STCH_BIT_START                              0U
#define DMASTAT_STCH_BIT_END                                31U

/* Definition for field HWCHENA in Register HWCHENAS */
#define HWCHENAS_HWCHENA_BIT_START                          0U
#define HWCHENAS_HWCHENA_BIT_END                            31U

/* Definition for field HWCHDIS in Register HWCHENAR */
#define HWCHENAR_HWCHDIS_BIT_START                          0U
#define HWCHENAR_HWCHDIS_BIT_END                            31U

/* Definition for field SWCHENA in Register SWCHENAS */
#define SWCHENAS_SWCHENA_BIT_START                          0U
#define SWCHENAS_SWCHENA_BIT_END                            31U

/* Definition for field SWCHDIS in Register SWCHENAR */
#define SWCHENAR_SWCHDIS_BIT_START                          0U
#define SWCHENAR_SWCHDIS_BIT_END                            31U

/* Definition for field CPH in Register CHPRIOS */
#define CHPRIOS_CPH_BIT_START                               0U
#define CHPRIOS_CPH_BIT_END                                 31U

/* Definition for field CPL in Register CHPRIOR */
#define CHPRIOR_CPL_BIT_START                               0U
#define CHPRIOR_CPL_BIT_END                                 31U

/* Definition for field GCHIE in Register GCHIENAS */
#define GCHIENAS_GCHIE_BIT_START                            0U
#define GCHIENAS_GCHIE_BIT_END                              31U

/* Definition for field GCHID in Register GCHIENAR */
#define GCHIENAR_GCHID_BIT_START                            0U
#define GCHIENAR_GCHID_BIT_END                              31U

/* Definition for field CH3ASI_5_0 in Register DREQASI0 */
#define DREQASI0_CH3ASI_5_0_BIT_START                       0U
#define DREQASI0_CH3ASI_5_0_BIT_END                         5U

/* Definition for field CH2ASI_5_0 in Register DREQASI0 */
#define DREQASI0_CH2ASI_5_0_BIT_START                       8U
#define DREQASI0_CH2ASI_5_0_BIT_END                         13U

/* Definition for field CH1ASI_5_0 in Register DREQASI0 */
#define DREQASI0_CH1ASI_5_0_BIT_START                       16U
#define DREQASI0_CH1ASI_5_0_BIT_END                         21U

/* Definition for field CH0ASI_5_0 in Register DREQASI0 */
#define DREQASI0_CH0ASI_5_0_BIT_START                       24U
#define DREQASI0_CH0ASI_5_0_BIT_END                         29U

/* Definition for field CH7ASI_5_0 in Register DREQASI1 */
#define DREQASI1_CH7ASI_5_0_BIT_START                       0U
#define DREQASI1_CH7ASI_5_0_BIT_END                         5U

/* Definition for field CH6ASI_5_0 in Register DREQASI1 */
#define DREQASI1_CH6ASI_5_0_BIT_START                       8U
#define DREQASI1_CH6ASI_5_0_BIT_END                         13U

/* Definition for field CH5ASI_5_0 in Register DREQASI1 */
#define DREQASI1_CH5ASI_5_0_BIT_START                       16U
#define DREQASI1_CH5ASI_5_0_BIT_END                         21U

/* Definition for field CH4ASI_5_0 in Register DREQASI1 */
#define DREQASI1_CH4ASI_5_0_BIT_START                       24U
#define DREQASI1_CH4ASI_5_0_BIT_END                         29U

/* Definition for field CH11ASI_5_0 in Register DREQASI2 */
#define DREQASI2_CH11ASI_5_0_BIT_START                      0U
#define DREQASI2_CH11ASI_5_0_BIT_END                        5U

/* Definition for field CH10ASI_5_0 in Register DREQASI2 */
#define DREQASI2_CH10ASI_5_0_BIT_START                      8U
#define DREQASI2_CH10ASI_5_0_BIT_END                        13U

/* Definition for field CH9ASI_5_0 in Register DREQASI2 */
#define DREQASI2_CH9ASI_5_0_BIT_START                       16U
#define DREQASI2_CH9ASI_5_0_BIT_END                         21U

/* Definition for field CH8ASI_5_0 in Register DREQASI2 */
#define DREQASI2_CH8ASI_5_0_BIT_START                       24U
#define DREQASI2_CH8ASI_5_0_BIT_END                         29U

/* Definition for field CH15ASI_5_0 in Register DREQASI3 */
#define DREQASI3_CH15ASI_5_0_BIT_START                      0U
#define DREQASI3_CH15ASI_5_0_BIT_END                        5U

/* Definition for field CH14ASI_5_0 in Register DREQASI3 */
#define DREQASI3_CH14ASI_5_0_BIT_START                      8U
#define DREQASI3_CH14ASI_5_0_BIT_END                        13U

/* Definition for field CH13ASI_5_0 in Register DREQASI3 */
#define DREQASI3_CH13ASI_5_0_BIT_START                      16U
#define DREQASI3_CH13ASI_5_0_BIT_END                        21U

/* Definition for field CH12ASI_5_0 in Register DREQASI3 */
#define DREQASI3_CH12ASI_5_0_BIT_START                      24U
#define DREQASI3_CH12ASI_5_0_BIT_END                        29U

/* Definition for field CH19ASI_5_0 in Register DREQASI4 */
#define DREQASI4_CH19ASI_5_0_BIT_START                      0U
#define DREQASI4_CH19ASI_5_0_BIT_END                        5U

/* Definition for field CH18ASI_5_0 in Register DREQASI4 */
#define DREQASI4_CH18ASI_5_0_BIT_START                      8U
#define DREQASI4_CH18ASI_5_0_BIT_END                        13U

/* Definition for field CH17ASI_5_0 in Register DREQASI4 */
#define DREQASI4_CH17ASI_5_0_BIT_START                      16U
#define DREQASI4_CH17ASI_5_0_BIT_END                        21U

/* Definition for field CH16ASI_5_0 in Register DREQASI4 */
#define DREQASI4_CH16ASI_5_0_BIT_START                      24U
#define DREQASI4_CH16ASI_5_0_BIT_END                        29U

/* Definition for field CH23ASI_5_0 in Register DREQASI5 */
#define DREQASI5_CH23ASI_5_0_BIT_START                      0U
#define DREQASI5_CH23ASI_5_0_BIT_END                        5U

/* Definition for field CH22ASI_5_0 in Register DREQASI5 */
#define DREQASI5_CH22ASI_5_0_BIT_START                      8U
#define DREQASI5_CH22ASI_5_0_BIT_END                        13U

/* Definition for field CH21ASI_5_0 in Register DREQASI5 */
#define DREQASI5_CH21ASI_5_0_BIT_START                      16U
#define DREQASI5_CH21ASI_5_0_BIT_END                        21U

/* Definition for field CH20ASI_5_0 in Register DREQASI5 */
#define DREQASI5_CH20ASI_5_0_BIT_START                      24U
#define DREQASI5_CH20ASI_5_0_BIT_END                        29U

/* Definition for field CH27ASI_5_0 in Register DREQASI6 */
#define DREQASI6_CH27ASI_5_0_BIT_START                      0U
#define DREQASI6_CH27ASI_5_0_BIT_END                        5U

/* Definition for field CH26ASI_5_0 in Register DREQASI6 */
#define DREQASI6_CH26ASI_5_0_BIT_START                      8U
#define DREQASI6_CH26ASI_5_0_BIT_END                        13U

/* Definition for field CH25ASI_5_0 in Register DREQASI6 */
#define DREQASI6_CH25ASI_5_0_BIT_START                      16U
#define DREQASI6_CH25ASI_5_0_BIT_END                        21U

/* Definition for field CH24ASI_5_0 in Register DREQASI6 */
#define DREQASI6_CH24ASI_5_0_BIT_START                      24U
#define DREQASI6_CH24ASI_5_0_BIT_END                        29U

/* Definition for field CH31ASI_5_0 in Register DREQASI7 */
#define DREQASI7_CH31ASI_5_0_BIT_START                      0U
#define DREQASI7_CH31ASI_5_0_BIT_END                        5U

/* Definition for field CH30ASI_5_0 in Register DREQASI7 */
#define DREQASI7_CH30ASI_5_0_BIT_START                      8U
#define DREQASI7_CH30ASI_5_0_BIT_END                        13U

/* Definition for field CH29ASI_5_0 in Register DREQASI7 */
#define DREQASI7_CH29ASI_5_0_BIT_START                      16U
#define DREQASI7_CH29ASI_5_0_BIT_END                        21U

/* Definition for field CH28ASI_5_0 in Register DREQASI7 */
#define DREQASI7_CH28ASI_5_0_BIT_START                      24U
#define DREQASI7_CH28ASI_5_0_BIT_END                        29U

/* Definition for field CH7PA_2_0 in Register PAR0 */
#define PAR0_CH7PA_2_0_BIT_START                            0U
#define PAR0_CH7PA_2_0_BIT_END                              2U

/* Definition for field CH6PA_2_0 in Register PAR0 */
#define PAR0_CH6PA_2_0_BIT_START                            4U
#define PAR0_CH6PA_2_0_BIT_END                              6U

/* Definition for field CH5PA_2_0 in Register PAR0 */
#define PAR0_CH5PA_2_0_BIT_START                            8U
#define PAR0_CH5PA_2_0_BIT_END                              10U

/* Definition for field CH4PA_2_0 in Register PAR0 */
#define PAR0_CH4PA_2_0_BIT_START                            12U
#define PAR0_CH4PA_2_0_BIT_END                              14U

/* Definition for field CH3PA_2_0 in Register PAR0 */
#define PAR0_CH3PA_2_0_BIT_START                            16U
#define PAR0_CH3PA_2_0_BIT_END                              18U

/* Definition for field CH2PA_2_0 in Register PAR0 */
#define PAR0_CH2PA_2_0_BIT_START                            20U
#define PAR0_CH2PA_2_0_BIT_END                              22U

/* Definition for field CH1PA_2_0 in Register PAR0 */
#define PAR0_CH1PA_2_0_BIT_START                            24U
#define PAR0_CH1PA_2_0_BIT_END                              26U

/* Definition for field CH0PA_2_0 in Register PAR0 */
#define PAR0_CH0PA_2_0_BIT_START                            28U
#define PAR0_CH0PA_2_0_BIT_END                              30U

/* Definition for field CH15PA_2_0 in Register PAR1 */
#define PAR1_CH15PA_2_0_BIT_START                           0U
#define PAR1_CH15PA_2_0_BIT_END                             2U

/* Definition for field CH14PA_2_0 in Register PAR1 */
#define PAR1_CH14PA_2_0_BIT_START                           4U
#define PAR1_CH14PA_2_0_BIT_END                             6U

/* Definition for field CH13PA_2_0 in Register PAR1 */
#define PAR1_CH13PA_2_0_BIT_START                           8U
#define PAR1_CH13PA_2_0_BIT_END                             10U

/* Definition for field CH12PA_2_0 in Register PAR1 */
#define PAR1_CH12PA_2_0_BIT_START                           12U
#define PAR1_CH12PA_2_0_BIT_END                             14U

/* Definition for field CH11PA_2_0 in Register PAR1 */
#define PAR1_CH11PA_2_0_BIT_START                           16U
#define PAR1_CH11PA_2_0_BIT_END                             18U

/* Definition for field CH10PA_2_0 in Register PAR1 */
#define PAR1_CH10PA_2_0_BIT_START                           20U
#define PAR1_CH10PA_2_0_BIT_END                             22U

/* Definition for field CH9PA_2_0 in Register PAR1 */
#define PAR1_CH9PA_2_0_BIT_START                            24U
#define PAR1_CH9PA_2_0_BIT_END                              26U

/* Definition for field CH8PA_2_0 in Register PAR1 */
#define PAR1_CH8PA_2_0_BIT_START                            28U
#define PAR1_CH8PA_2_0_BIT_END                              30U

/* Definition for field CH23PA_2_0 in Register PAR2 */
#define PAR2_CH23PA_2_0_BIT_START                           0U
#define PAR2_CH23PA_2_0_BIT_END                             2U

/* Definition for field CH22PA_2_0 in Register PAR2 */
#define PAR2_CH22PA_2_0_BIT_START                           4U
#define PAR2_CH22PA_2_0_BIT_END                             6U

/* Definition for field CH21PA_2_0 in Register PAR2 */
#define PAR2_CH21PA_2_0_BIT_START                           8U
#define PAR2_CH21PA_2_0_BIT_END                             10U

/* Definition for field CH20PA_2_0 in Register PAR2 */
#define PAR2_CH20PA_2_0_BIT_START                           12U
#define PAR2_CH20PA_2_0_BIT_END                             14U

/* Definition for field CH19PA_2_0 in Register PAR2 */
#define PAR2_CH19PA_2_0_BIT_START                           16U
#define PAR2_CH19PA_2_0_BIT_END                             18U

/* Definition for field CH18PA_2_0 in Register PAR2 */
#define PAR2_CH18PA_2_0_BIT_START                           20U
#define PAR2_CH18PA_2_0_BIT_END                             22U

/* Definition for field CH17PA_2_0 in Register PAR2 */
#define PAR2_CH17PA_2_0_BIT_START                           24U
#define PAR2_CH17PA_2_0_BIT_END                             26U

/* Definition for field CH16PA_2_0 in Register PAR2 */
#define PAR2_CH16PA_2_0_BIT_START                           28U
#define PAR2_CH16PA_2_0_BIT_END                             30U

/* Definition for field CH31PA_2_0 in Register PAR3 */
#define PAR3_CH31PA_2_0_BIT_START                           0U
#define PAR3_CH31PA_2_0_BIT_END                             2U

/* Definition for field CH30PA_2_0 in Register PAR3 */
#define PAR3_CH30PA_2_0_BIT_START                           4U
#define PAR3_CH30PA_2_0_BIT_END                             6U

/* Definition for field CH29PA_2_0 in Register PAR3 */
#define PAR3_CH29PA_2_0_BIT_START                           8U
#define PAR3_CH29PA_2_0_BIT_END                             10U

/* Definition for field CH28PA_2_0 in Register PAR3 */
#define PAR3_CH28PA_2_0_BIT_START                           12U
#define PAR3_CH28PA_2_0_BIT_END                             14U

/* Definition for field CH27PA_2_0 in Register PAR3 */
#define PAR3_CH27PA_2_0_BIT_START                           16U
#define PAR3_CH27PA_2_0_BIT_END                             18U

/* Definition for field CH26PA_2_0 in Register PAR3 */
#define PAR3_CH26PA_2_0_BIT_START                           20U
#define PAR3_CH26PA_2_0_BIT_END                             22U

/* Definition for field CH25PA_2_0 in Register PAR3 */
#define PAR3_CH25PA_2_0_BIT_START                           24U
#define PAR3_CH25PA_2_0_BIT_END                             26U

/* Definition for field CH24PA_2_0 in Register PAR3 */
#define PAR3_CH24PA_2_0_BIT_START                           28U
#define PAR3_CH24PA_2_0_BIT_END                             30U

/* Definition for field FTCAB in Register FTCMAP */
#define FTCMAP_FTCAB_BIT_START                              0U
#define FTCMAP_FTCAB_BIT_END                                31U

/* Definition for field LFSAB in Register LFSMAP */
#define LFSMAP_LFSAB_BIT_START                              0U
#define LFSMAP_LFSAB_BIT_END                                31U

/* Definition for field HBCAB in Register HBCMAP */
#define HBCMAP_HBCAB_BIT_START                              0U
#define HBCMAP_HBCAB_BIT_END                                31U

/* Definition for field BTCAB in Register BTCMAP */
#define BTCMAP_BTCAB_BIT_START                              0U
#define BTCMAP_BTCAB_BIT_END                                31U

/* Definition for field BERAB in Register BERMAP */
#define BERMAP_BERAB_BIT_START                              0U
#define BERMAP_BERAB_BIT_END                                31U

/* Definition for field FTCINTENA in Register FTCINTENAS */
#define FTCINTENAS_FTCINTENA_BIT_START                      0U
#define FTCINTENAS_FTCINTENA_BIT_END                        31U

/* Definition for field FTCINTDIS in Register FTCINTENAR */
#define FTCINTENAR_FTCINTDIS_BIT_START                      0U
#define FTCINTENAR_FTCINTDIS_BIT_END                        31U

/* Definition for field LFSINTENA in Register LFSINTENAS */
#define LFSINTENAS_LFSINTENA_BIT_START                      0U
#define LFSINTENAS_LFSINTENA_BIT_END                        31U

/* Definition for field LFSINTDIS in Register LFSINTENAR */
#define LFSINTENAR_LFSINTDIS_BIT_START                      0U
#define LFSINTENAR_LFSINTDIS_BIT_END                        31U

/* Definition for field HBCINTENA in Register HBCINTENAS */
#define HBCINTENAS_HBCINTENA_BIT_START                      0U
#define HBCINTENAS_HBCINTENA_BIT_END                        31U

/* Definition for field HBCINTDIS in Register HBCINTENAR */
#define HBCINTENAR_HBCINTDIS_BIT_START                      0U
#define HBCINTENAR_HBCINTDIS_BIT_END                        31U

/* Definition for field BTCINTENA in Register BTCINTENAS */
#define BTCINTENAS_BTCINTENA_BIT_START                      0U
#define BTCINTENAS_BTCINTENA_BIT_END                        31U

/* Definition for field BTCINTDIS in Register BTCINTENAR */
#define BTCINTENAR_BTCINTDIS_BIT_START                      0U
#define BTCINTENAR_BTCINTDIS_BIT_END                        31U

/* Definition for field GINT in Register GINTFLAG */
#define GINTFLAG_GINT_BIT_START                             0U
#define GINTFLAG_GINT_BIT_END                               31U

/* Definition for field FTC in Register FTCFLAG */
#define FTCFLAG_FTC_BIT_START                               0U
#define FTCFLAG_FTC_BIT_END                                 31U

/* Definition for field LFSI in Register LFSFLAG */
#define LFSFLAG_LFSI_BIT_START                              0U
#define LFSFLAG_LFSI_BIT_END                                31U

/* Definition for field HBCI in Register HBCFLAG */
#define HBCFLAG_HBCI_BIT_START                              0U
#define HBCFLAG_HBCI_BIT_END                                31U

/* Definition for field BTCI in Register BTCFLAG */
#define BTCFLAG_BTCI_BIT_START                              0U
#define BTCFLAG_BTCI_BIT_END                                31U

/* Definition for field BERI in Register BERFLAG */
#define BERFLAG_BERI_BIT_START                              0U
#define BERFLAG_BERI_BIT_END                                31U

/* Definition for field FTCA_5_0 in Register FTCAOFFSET */
#define FTCAOFFSET_FTCA_5_0_BIT_START                       0U
#define FTCAOFFSET_FTCA_5_0_BIT_END                         5U

/* Definition for field sbz in Register FTCAOFFSET */
#define FTCAOFFSET_SBZ_BIT_START                            6U
#define FTCAOFFSET_SBZ_BIT_END                              7U

/* Definition for field LFSA_5_0 in Register LFSAOFFSET */
#define LFSAOFFSET_LFSA_5_0_BIT_START                       0U
#define LFSAOFFSET_LFSA_5_0_BIT_END                         5U

/* Definition for field sbz in Register LFSAOFFSET */
#define LFSAOFFSET_SBZ_BIT_START                            6U
#define LFSAOFFSET_SBZ_BIT_END                              7U

/* Definition for field HBCA_5_0 in Register HBCAOFFSET */
#define HBCAOFFSET_HBCA_5_0_BIT_START                       0U
#define HBCAOFFSET_HBCA_5_0_BIT_END                         5U

/* Definition for field sbz in Register HBCAOFFSET */
#define HBCAOFFSET_SBZ_BIT_START                            6U
#define HBCAOFFSET_SBZ_BIT_END                              7U

/* Definition for field BTCA_5_0 in Register BTCAOFFSET */
#define BTCAOFFSET_BTCA_5_0_BIT_START                       0U
#define BTCAOFFSET_BTCA_5_0_BIT_END                         5U

/* Definition for field sbz in Register BTCAOFFSET */
#define BTCAOFFSET_SBZ_BIT_START                            6U
#define BTCAOFFSET_SBZ_BIT_END                              7U

/* Definition for field BERA_5_0 in Register BERAOFFSET */
#define BERAOFFSET_BERA_5_0_BIT_START                       0U
#define BERAOFFSET_BERA_5_0_BIT_END                         5U

/* Definition for field sbz in Register BERAOFFSET */
#define BERAOFFSET_SBZ_BIT_START                            6U
#define BERAOFFSET_SBZ_BIT_END                              7U

/* Definition for field FTCB_5_0 in Register FTCBOFFSET */
#define FTCBOFFSET_FTCB_5_0_BIT_START                       0U
#define FTCBOFFSET_FTCB_5_0_BIT_END                         5U

/* Definition for field sbz in Register FTCBOFFSET */
#define FTCBOFFSET_SBZ_BIT_START                            6U
#define FTCBOFFSET_SBZ_BIT_END                              7U

/* Definition for field LFSB_5_0 in Register LFSBOFFSET */
#define LFSBOFFSET_LFSB_5_0_BIT_START                       0U
#define LFSBOFFSET_LFSB_5_0_BIT_END                         5U

/* Definition for field sbz in Register LFSBOFFSET */
#define LFSBOFFSET_SBZ_BIT_START                            6U
#define LFSBOFFSET_SBZ_BIT_END                              7U

/* Definition for field HBCB_5_0 in Register HBCBOFFSET */
#define HBCBOFFSET_HBCB_5_0_BIT_START                       0U
#define HBCBOFFSET_HBCB_5_0_BIT_END                         5U

/* Definition for field sbz in Register HBCBOFFSET */
#define HBCBOFFSET_SBZ_BIT_START                            6U
#define HBCBOFFSET_SBZ_BIT_END                              7U

/* Definition for field BTCB_5_0 in Register BTCBOFFSET */
#define BTCBOFFSET_BTCB_5_0_BIT_START                       0U
#define BTCBOFFSET_BTCB_5_0_BIT_END                         5U

/* Definition for field sbz in Register BTCBOFFSET */
#define BTCBOFFSET_SBZ_BIT_START                            6U
#define BTCBOFFSET_SBZ_BIT_END                              7U

/* Definition for field BERB_5_0 in Register BERBOFFSET */
#define BERBOFFSET_BERB_5_0_BIT_START                       0U
#define BERBOFFSET_BERB_5_0_BIT_END                         5U

/* Definition for field sbz in Register BERBOFFSET */
#define BERBOFFSET_SBZ_BIT_START                            6U
#define BERBOFFSET_SBZ_BIT_END                              7U

/* Definition for field PSFRLQPA in Register PTCRL */
#define PTCRL_PSFRLQPA_BIT_START                            0U
#define PTCRL_PSFRLQPA_BIT_END                              0U

/* Definition for field PSFRHQPA in Register PTCRL */
#define PTCRL_PSFRHQPA_BIT_START                            1U
#define PTCRL_PSFRHQPA_BIT_END                              1U

/* Definition for field BYA in Register PTCRL */
#define PTCRL_BYA_BIT_START                                 2U
#define PTCRL_BYA_BIT_END                                   2U

/* Definition for field PENDA in Register PTCRL */
#define PTCRL_PENDA_BIT_START                               8U
#define PTCRL_PENDA_BIT_END                                 8U

/* Definition for field PSFRLQPB in Register PTCRL */
#define PTCRL_PSFRLQPB_BIT_START                            16U
#define PTCRL_PSFRLQPB_BIT_END                              16U

/* Definition for field PSFRHQPB in Register PTCRL */
#define PTCRL_PSFRHQPB_BIT_START                            17U
#define PTCRL_PSFRHQPB_BIT_END                              17U

/* Definition for field BYB in Register PTCRL */
#define PTCRL_BYB_BIT_START                                 18U
#define PTCRL_BYB_BIT_END                                   18U

/* Definition for field PENDB in Register PTCRL */
#define PTCRL_PENDB_BIT_START                               24U
#define PTCRL_PENDB_BIT_END                                 24U

/* Definition for field RTC in Register RTCTRL */
#define RTCTRL_RTC_BIT_START                                0U
#define RTCTRL_RTC_BIT_END                                  0U

/* Definition for field DBGEN in Register DCTRL */
#define DCTRL_DBGEN_BIT_START                               0U
#define DCTRL_DBGEN_BIT_END                                 0U

/* Definition for field DMADBGS in Register DCTRL */
#define DCTRL_DMADBGS_BIT_START                             16U
#define DCTRL_DMADBGS_BIT_END                               16U

/* Definition for field CHNUM in Register DCTRL */
#define DCTRL_CHNUM_BIT_START                               24U
#define DCTRL_CHNUM_BIT_END                                 28U

/* Definition for field WP in Register WPR */
#define WPR_WP_BIT_START                                    0U
#define WPR_WP_BIT_END                                      31U

/* Definition for field WM in Register WMR */
#define WMR_WM_BIT_START                                    0U
#define WMR_WM_BIT_END                                      31U

/* Definition for field PAACSA in Register PAACSADDR */
#define PAACSADDR_PAACSA_BIT_START                          0U
#define PAACSADDR_PAACSA_BIT_END                            31U

/* Definition for field PAACDA in Register PAACDADDR */
#define PAACDADDR_PAACDA_BIT_START                          0U
#define PAACDADDR_PAACDA_BIT_END                            31U

/* Definition for field PAETCOUNT_12_0 in Register PAACTC */
#define PAACTC_PAETCOUNT_12_0_BIT_START                     0U
#define PAACTC_PAETCOUNT_12_0_BIT_END                       12U

/* Definition for field PAFTCOUNT_28_16 in Register PAACTC */
#define PAACTC_PAFTCOUNT_28_16_BIT_START                    16U
#define PAACTC_PAFTCOUNT_28_16_BIT_END                      28U

/* Definition for field PBACSA in Register PBACSADDR */
#define PBACSADDR_PBACSA_BIT_START                          0U
#define PBACSADDR_PBACSA_BIT_END                            31U

/* Definition for field PBACDA in Register PBACDADDR */
#define PBACDADDR_PBACDA_BIT_START                          0U
#define PBACDADDR_PBACDA_BIT_END                            31U

/* Definition for field PBETCOUNT_12_0 in Register PBACTC */
#define PBACTC_PBETCOUNT_12_0_BIT_START                     0U
#define PBACTC_PBETCOUNT_12_0_BIT_END                       12U

/* Definition for field PBFTCOUNT_28_16 in Register PBACTC */
#define PBACTC_PBFTCOUNT_28_16_BIT_START                    16U
#define PBACTC_PBFTCOUNT_28_16_BIT_END                      28U

/* Definition for field PARITY_ENA in Register DMAPCR */
#define DMAPCR_PARITY_ENA_BIT_START                         0U
#define DMAPCR_PARITY_ENA_BIT_END                           3U

/* Definition for field TEST in Register DMAPCR */
#define DMAPCR_TEST_BIT_START                               8U
#define DMAPCR_TEST_BIT_END                                 8U

/* Definition for field ERRA in Register DMAPCR */
#define DMAPCR_ERRA_BIT_START                               16U
#define DMAPCR_ERRA_BIT_END                                 16U

/* Definition for field ERRORADDRESS_11_0 in Register DMAPAR */
#define DMAPAR_ERRORADDRESS_11_0_BIT_START                  0U
#define DMAPAR_ERRORADDRESS_11_0_BIT_END                    11U

/* Definition for field EDFLG in Register DMAPAR */
#define DMAPAR_EDFLG_BIT_START                              24U
#define DMAPAR_EDFLG_BIT_END                                24U

/* Definition for field REG0ENA in Register DMAMPCTRL */
#define DMAMPCTRL_REG0ENA_BIT_START                         0U
#define DMAMPCTRL_REG0ENA_BIT_END                           0U

/* Definition for field REG0AP in Register DMAMPCTRL */
#define DMAMPCTRL_REG0AP_BIT_START                          1U
#define DMAMPCTRL_REG0AP_BIT_END                            2U

/* Definition for field INT0ENA in Register DMAMPCTRL */
#define DMAMPCTRL_INT0ENA_BIT_START                         3U
#define DMAMPCTRL_INT0ENA_BIT_END                           3U

/* Definition for field INT0AB in Register DMAMPCTRL */
#define DMAMPCTRL_INT0AB_BIT_START                          4U
#define DMAMPCTRL_INT0AB_BIT_END                            4U

/* Definition for field REG1ENA in Register DMAMPCTRL */
#define DMAMPCTRL_REG1ENA_BIT_START                         8U
#define DMAMPCTRL_REG1ENA_BIT_END                           8U

/* Definition for field REG1AP in Register DMAMPCTRL */
#define DMAMPCTRL_REG1AP_BIT_START                          9U
#define DMAMPCTRL_REG1AP_BIT_END                            10U

/* Definition for field INT1ENA in Register DMAMPCTRL */
#define DMAMPCTRL_INT1ENA_BIT_START                         11U
#define DMAMPCTRL_INT1ENA_BIT_END                           11U

/* Definition for field INT1AB in Register DMAMPCTRL */
#define DMAMPCTRL_INT1AB_BIT_START                          12U
#define DMAMPCTRL_INT1AB_BIT_END                            12U

/* Definition for field REG2ENA in Register DMAMPCTRL */
#define DMAMPCTRL_REG2ENA_BIT_START                         16U
#define DMAMPCTRL_REG2ENA_BIT_END                           16U

/* Definition for field REG2AP in Register DMAMPCTRL */
#define DMAMPCTRL_REG2AP_BIT_START                          17U
#define DMAMPCTRL_REG2AP_BIT_END                            18U

/* Definition for field INT2ENA in Register DMAMPCTRL */
#define DMAMPCTRL_INT2ENA_BIT_START                         19U
#define DMAMPCTRL_INT2ENA_BIT_END                           19U

/* Definition for field INT2AB in Register DMAMPCTRL */
#define DMAMPCTRL_INT2AB_BIT_START                          20U
#define DMAMPCTRL_INT2AB_BIT_END                            20U

/* Definition for field REG3ENA in Register DMAMPCTRL */
#define DMAMPCTRL_REG3ENA_BIT_START                         24U
#define DMAMPCTRL_REG3ENA_BIT_END                           24U

/* Definition for field REG3AP in Register DMAMPCTRL */
#define DMAMPCTRL_REG3AP_BIT_START                          25U
#define DMAMPCTRL_REG3AP_BIT_END                            26U

/* Definition for field INT3ENA in Register DMAMPCTRL */
#define DMAMPCTRL_INT3ENA_BIT_START                         27U
#define DMAMPCTRL_INT3ENA_BIT_END                           27U

/* Definition for field INT3AB in Register DMAMPCTRL */
#define DMAMPCTRL_INT3AB_BIT_START                          28U
#define DMAMPCTRL_INT3AB_BIT_END                            28U

/* Definition for field REG0FT in Register DMAMPST */
#define DMAMPST_REG0FT_BIT_START                            0U
#define DMAMPST_REG0FT_BIT_END                              0U

/* Definition for field REG1FT in Register DMAMPST */
#define DMAMPST_REG1FT_BIT_START                            8U
#define DMAMPST_REG1FT_BIT_END                              8U

/* Definition for field REG2FT in Register DMAMPST */
#define DMAMPST_REG2FT_BIT_START                            16U
#define DMAMPST_REG2FT_BIT_END                              16U

/* Definition for field REG3FT in Register DMAMPST */
#define DMAMPST_REG3FT_BIT_START                            24U
#define DMAMPST_REG3FT_BIT_END                              24U

/* Definition for field STARTADDRESS in Register DMAMPR0S */
#define DMAMPR0S_STARTADDRESS_BIT_START                     0U
#define DMAMPR0S_STARTADDRESS_BIT_END                       31U

/* Definition for field ENDADDRESS in Register DMAMPR0E */
#define DMAMPR0E_ENDADDRESS_BIT_START                       0U
#define DMAMPR0E_ENDADDRESS_BIT_END                         31U

/* Definition for field STARTADDRESS in Register DMAMPR1S */
#define DMAMPR1S_STARTADDRESS_BIT_START                     0U
#define DMAMPR1S_STARTADDRESS_BIT_END                       31U

/* Definition for field ENDADDRESS in Register DMAMPR1E */
#define DMAMPR1E_ENDADDRESS_BIT_START                       0U
#define DMAMPR1E_ENDADDRESS_BIT_END                         31U

/* Definition for field STARTADDRESS in Register DMAMPR2S */
#define DMAMPR2S_STARTADDRESS_BIT_START                     0U
#define DMAMPR2S_STARTADDRESS_BIT_END                       31U

/* Definition for field ENDADDRESS in Register DMAMPR2E */
#define DMAMPR2E_ENDADDRESS_BIT_START                       0U
#define DMAMPR2E_ENDADDRESS_BIT_END                         31U

/* Definition for field STARTADDRESS in Register DMAMPR3S */
#define DMAMPR3S_STARTADDRESS_BIT_START                     0U
#define DMAMPR3S_STARTADDRESS_BIT_END                       31U

/* Definition for field ENDADDRESS in Register DMAMPR3E */
#define DMAMPR3E_ENDADDRESS_BIT_START                       0U
#define DMAMPR3E_ENDADDRESS_BIT_END                         31U



/**
 * @struct DMARegs_t
 * @brief
 *   Module MSS_DMA_REG Register Definition
 * @details
 *   This structure is used to access the MSS_DMA_REG module registers.
 */
/**
 * @typedef DMARegs
 * @brief
 *   Module MSS_DMA_REG Register Frame type Definition
 * @details
 *   This type is used to access the MSS_DMA_REG module registers.
 */
typedef volatile struct DMARegs_t
{
    uint32_t    GCTRL                              ;        /* Offset = 0x000 */
    uint32_t    PEND                               ;        /* Offset = 0x004 */
    uint32_t    FBREG                              ;        /* Offset = 0x008 */
    uint32_t    DMASTAT                            ;        /* Offset = 0x00C */
    uint32_t    MSS_DMA_RESERVED1                  ;        /* Offset = 0x010 */
    uint32_t    HWCHENAS                           ;        /* Offset = 0x014 */
    uint32_t    MSS_DMA_RESERVED2                  ;        /* Offset = 0x018 */
    uint32_t    HWCHENAR                           ;        /* Offset = 0x01C */
    uint32_t    MSS_DMA_RESERVED3                  ;        /* Offset = 0x020 */
    uint32_t    SWCHENAS                           ;        /* Offset = 0x024 */
    uint32_t    MSS_DMA_RESERVED4                  ;        /* Offset = 0x028 */
    uint32_t    SWCHENAR                           ;        /* Offset = 0x02C */
    uint32_t    MSS_DMA_RESERVED5                  ;        /* Offset = 0x030 */
    uint32_t    CHPRIOS                            ;        /* Offset = 0x034 */
    uint32_t    MSS_DMA_RESERVED6                  ;        /* Offset = 0x038 */
    uint32_t    CHPRIOR                            ;        /* Offset = 0x03C */
    uint32_t    MSS_DMA_RESERVED7                  ;        /* Offset = 0x040 */
    uint32_t    GCHIENAS                           ;        /* Offset = 0x044 */
    uint32_t    MSS_DMA_RESERVED8                  ;        /* Offset = 0x048 */
    uint32_t    GCHIENAR                           ;        /* Offset = 0x04C */
    uint32_t    MSS_DMA_RESERVED9                  ;        /* Offset = 0x050 */
#if 0 /* convert to array */
    uint32_t    DREQASI0                           ;        /* Offset = 0x054 */
    uint32_t    DREQASI1                           ;        /* Offset = 0x058 */
    uint32_t    DREQASI2                           ;        /* Offset = 0x05C */
    uint32_t    DREQASI3                           ;        /* Offset = 0x060 */
    uint32_t    DREQASI4                           ;        /* Offset = 0x064 */
    uint32_t    DREQASI5                           ;        /* Offset = 0x068 */
    uint32_t    DREQASI6                           ;        /* Offset = 0x06C */
    uint32_t    DREQASI7                           ;        /* Offset = 0x070 */
#else
    uint32_t DREQASI[8U]; /**<  0x0054 - 0x70: DMA Request Assignment Register */
#endif
    uint32_t    MSS_DMA_RESERVED10                 ;        /* Offset = 0x074 */
    uint32_t    MSS_DMA_RESERVED11                 ;        /* Offset = 0x078 */
    uint32_t    MSS_DMA_RESERVED12                 ;        /* Offset = 0x07C */
    uint32_t    MSS_DMA_RESERVED13                 ;        /* Offset = 0x080 */
    uint32_t    MSS_DMA_RESERVED14                 ;        /* Offset = 0x084 */
    uint32_t    MSS_DMA_RESERVED15                 ;        /* Offset = 0x088 */
    uint32_t    MSS_DMA_RESERVED16                 ;        /* Offset = 0x08C */
    uint32_t    MSS_DMA_RESERVED17                 ;        /* Offset = 0x090 */
#if 0 /* convert to array */
    uint32_t    PAR0                               ;        /* Offset = 0x094 */
    uint32_t    PAR1                               ;        /* Offset = 0x098 */
    uint32_t    PAR2                               ;        /* Offset = 0x09C */
    uint32_t    PAR3                               ;        /* Offset = 0x0A0 */
#else
    uint32_t PAR[4U];     /**<  0x0094 - 0xA0: Port Assignment Register        */
#endif
    uint32_t    MSS_DMA_RESERVED18                 ;        /* Offset = 0x0A4 */
    uint32_t    MSS_DMA_RESERVED19                 ;        /* Offset = 0x0A8 */
    uint32_t    MSS_DMA_RESERVED20                 ;        /* Offset = 0x0AC */
    uint32_t    MSS_DMA_RESERVED21                 ;        /* Offset = 0x0B0 */
    uint32_t    FTCMAP                             ;        /* Offset = 0x0B4 */
    uint32_t    MSS_DMA_RESERVED22                 ;        /* Offset = 0x0B8 */
    uint32_t    LFSMAP                             ;        /* Offset = 0x0BC */
    uint32_t    MSS_DMA_RESERVED23                 ;        /* Offset = 0x0C0 */
    uint32_t    HBCMAP                             ;        /* Offset = 0x0C4 */
    uint32_t    MSS_DMA_RESERVED24                 ;        /* Offset = 0x0C8 */
    uint32_t    BTCMAP                             ;        /* Offset = 0x0CC */
    uint32_t    MSS_DMA_RESERVED25                 ;        /* Offset = 0x0D0 */
    uint32_t    BERMAP                             ;        /* Offset = 0x0D4 */
    uint32_t    MSS_DMA_RESERVED26                 ;        /* Offset = 0x0D8 */
    uint32_t    FTCINTENAS                         ;        /* Offset = 0x0DC */
    uint32_t    MSS_DMA_RESERVED27                 ;        /* Offset = 0x0E0 */
    uint32_t    FTCINTENAR                         ;        /* Offset = 0x0E4 */
    uint32_t    MSS_DMA_RESERVED28                 ;        /* Offset = 0x0E8 */
    uint32_t    LFSINTENAS                         ;        /* Offset = 0x0EC */
    uint32_t    MSS_DMA_RESERVED29                 ;        /* Offset = 0x0F0 */
    uint32_t    LFSINTENAR                         ;        /* Offset = 0x0F4 */
    uint32_t    MSS_DMA_RESERVED30                 ;        /* Offset = 0x0F8 */
    uint32_t    HBCINTENAS                         ;        /* Offset = 0x0FC */
    uint32_t    MSS_DMA_RESERVED31                 ;        /* Offset = 0x100 */
    uint32_t    HBCINTENAR                         ;        /* Offset = 0x104 */
    uint32_t    RESERVED32                         ;        /* Offset = 0x108 */
    uint32_t    BTCINTENAS                         ;        /* Offset = 0x10C */
    uint32_t    MSS_DMA_RESERVED33                 ;        /* Offset = 0x110 */
    uint32_t    BTCINTENAR                         ;        /* Offset = 0x114 */
    uint32_t    MSS_DMA_RESERVED34                 ;        /* Offset = 0x118 */
    uint32_t    GINTFLAG                           ;        /* Offset = 0x11C */
    uint32_t    MSS_DMA_RESERVED35                 ;        /* Offset = 0x120 */
    uint32_t    FTCFLAG                            ;        /* Offset = 0x124 */
    uint32_t    MSS_DMA_RESERVED36                 ;        /* Offset = 0x128 */
    uint32_t    LFSFLAG                            ;        /* Offset = 0x12C */
    uint32_t    MSS_DMA_RESERVED37                 ;        /* Offset = 0x130 */
    uint32_t    HBCFLAG                            ;        /* Offset = 0x134 */
    uint32_t    MSS_DMA_RESERVED38                 ;        /* Offset = 0x138 */
    uint32_t    BTCFLAG                            ;        /* Offset = 0x13C */
    uint32_t    MSS_DMA_RESERVED39                 ;        /* Offset = 0x140 */
    uint32_t    BERFLAG                            ;        /* Offset = 0x144 */
    uint32_t    MSS_DMA_RESERVED40                 ;        /* Offset = 0x148 */
    uint32_t    FTCAOFFSET                         ;        /* Offset = 0x14C */
    uint32_t    LFSAOFFSET                         ;        /* Offset = 0x150 */
    uint32_t    HBCAOFFSET                         ;        /* Offset = 0x154 */
    uint32_t    BTCAOFFSET                         ;        /* Offset = 0x158 */
    uint32_t    BERAOFFSET                         ;        /* Offset = 0x15C */
    uint32_t    FTCBOFFSET                         ;        /* Offset = 0x160 */
    uint32_t    LFSBOFFSET                         ;        /* Offset = 0x164 */
    uint32_t    HBCBOFFSET                         ;        /* Offset = 0x168 */
    uint32_t    BTCBOFFSET                         ;        /* Offset = 0x16C */
    uint32_t    BERBOFFSET                         ;        /* Offset = 0x170 */
    uint32_t    MSS_DMA_RESERVED41                 ;        /* Offset = 0x174 */
    uint32_t    PTCRL                              ;        /* Offset = 0x178 */
    uint32_t    RTCTRL                             ;        /* Offset = 0x17C */
    uint32_t    DCTRL                              ;        /* Offset = 0x180 */
    uint32_t    WPR                                ;        /* Offset = 0x184 */
    uint32_t    WMR                                ;        /* Offset = 0x188 */
    uint32_t    PAACSADDR                          ;        /* Offset = 0x18C */
    uint32_t    PAACDADDR                          ;        /* Offset = 0x190 */
    uint32_t    PAACTC                             ;        /* Offset = 0x194 */
    uint32_t    PBACSADDR                          ;        /* Offset = 0x198 */
    uint32_t    PBACDADDR                          ;        /* Offset = 0x19C */
    uint32_t    PBACTC                             ;        /* Offset = 0x1A0 */
    uint32_t    MSS_DMA_RESERVED42                 ;        /* Offset = 0x1A4 */
    uint32_t    DMAPCR                             ;        /* Offset = 0x1A8 */
    uint32_t    DMAPAR                             ;        /* Offset = 0x1AC */
    uint32_t    DMAMPCTRL                          ;        /* Offset = 0x1B0 */
    uint32_t    DMAMPST                            ;        /* Offset = 0x1B4 */
#if 0 /* convert to array */
    uint32_t    DMAMPR0S                           ;        /* Offset = 0x1B8 */
    uint32_t    DMAMPR0E                           ;        /* Offset = 0x1BC */
    uint32_t    DMAMPR1S                           ;        /* Offset = 0x1C0 */
    uint32_t    DMAMPR1E                           ;        /* Offset = 0x1C4 */
    uint32_t    DMAMPR2S                           ;        /* Offset = 0x1C8 */
    uint32_t    DMAMPR2E                           ;        /* Offset = 0x1CC */
    uint32_t    DMAMPR3S                           ;        /* Offset = 0x1D0 */
    uint32_t    DMAMPR3E                           ;        /* Offset = 0x1D4 */
#else
    struct
    {
        uint32_t STARTADD; /**<  0x01B8, 0x01C0, 0x01C8, 0x1D0: DMA Memory Protection Region Start Address Register  */
        uint32_t ENDADD;   /**<  0x01B8, 0x01C0, 0x01C8, 0x1D0: DMA Memory Protection Region Start Address Register  */
    }DMAMPR[4U];
#endif
} DMARegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_DMA_H */
/* END OF REG_DMA_H */

