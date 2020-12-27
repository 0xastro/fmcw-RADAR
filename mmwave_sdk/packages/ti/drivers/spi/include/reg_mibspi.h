/**
 *  @file    reg_mibspi.h
 *
 *  @brief
 *    This file gives register definitions of MSS_MIBSPIA module.
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

#ifndef REG_MIBSPI_H
#define REG_MIBSPI_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field nRESET in Register SPIGCR0 */
#define SPIGCR0_NRESET_BIT_START                            0U
#define SPIGCR0_NRESET_BIT_END                              0U

/* Definition for field NU in Register SPIGCR0 */
#define SPIGCR0_NU_BIT_START                                1U
#define SPIGCR0_NU_BIT_END                                  31U

/* Definition for field MASTER in Register SPIGCR1 */
#define SPIGCR1_MASTER_BIT_START                            0U
#define SPIGCR1_MASTER_BIT_END                              0U

/* Definition for field CLKMOD in Register SPIGCR1 */
#define SPIGCR1_CLKMOD_BIT_START                            1U
#define SPIGCR1_CLKMOD_BIT_END                              1U

/* Definition for field NU1 in Register SPIGCR1 */
#define SPIGCR1_NU1_BIT_START                               2U
#define SPIGCR1_NU1_BIT_END                                 7U

/* Definition for field POWERDOWN in Register SPIGCR1 */
#define SPIGCR1_POWERDOWN_BIT_START                         8U
#define SPIGCR1_POWERDOWN_BIT_END                           8U

/* Definition for field NU2 in Register SPIGCR1 */
#define SPIGCR1_NU2_BIT_START                               9U
#define SPIGCR1_NU2_BIT_END                                 15U

/* Definition for field LOOPBACK in Register SPIGCR1 */
#define SPIGCR1_LOOPBACK_BIT_START                          16U
#define SPIGCR1_LOOPBACK_BIT_END                            16U

/* Definition for field NU3 in Register SPIGCR1 */
#define SPIGCR1_NU3_BIT_START                               17U
#define SPIGCR1_NU3_BIT_END                                 23U

/* Definition for field SPIEN in Register SPIGCR1 */
#define SPIGCR1_SPIEN_BIT_START                             24U
#define SPIGCR1_SPIEN_BIT_END                               24U

/* Definition for field NU4 in Register SPIGCR1 */
#define SPIGCR1_NU4_BIT_START                               25U
#define SPIGCR1_NU4_BIT_END                                 31U

/* Definition for field DLENERRENA in Register SPIINT0 */
#define SPIINT0_DLENERRENA_BIT_START                        0U
#define SPIINT0_DLENERRENA_BIT_END                          0U

/* Definition for field TIMEOUTENA in Register SPIINT0 */
#define SPIINT0_TIMEOUTENA_BIT_START                        1U
#define SPIINT0_TIMEOUTENA_BIT_END                          1U

/* Definition for field PARERRENA in Register SPIINT0 */
#define SPIINT0_PARERRENA_BIT_START                         2U
#define SPIINT0_PARERRENA_BIT_END                           2U

/* Definition for field DESYNCENA in Register SPIINT0 */
#define SPIINT0_DESYNCENA_BIT_START                         3U
#define SPIINT0_DESYNCENA_BIT_END                           3U

/* Definition for field BITERRENA in Register SPIINT0 */
#define SPIINT0_BITERRENA_BIT_START                         4U
#define SPIINT0_BITERRENA_BIT_END                           4U

/* Definition for field NU1 in Register SPIINT0 */
#define SPIINT0_NU1_BIT_START                               5U
#define SPIINT0_NU1_BIT_END                                 5U

/* Definition for field OVRNINTENA in Register SPIINT0 */
#define SPIINT0_OVRNINTENA_BIT_START                        6U
#define SPIINT0_OVRNINTENA_BIT_END                          6U

/* Definition for field NU2 in Register SPIINT0 */
#define SPIINT0_NU2_BIT_START                               7U
#define SPIINT0_NU2_BIT_END                                 7U

/* Definition for field RXINTENA in Register SPIINT0 */
#define SPIINT0_RXINTENA_BIT_START                          8U
#define SPIINT0_RXINTENA_BIT_END                            8U

/* Definition for field TXINTENA in Register SPIINT0 */
#define SPIINT0_TXINTENA_BIT_START                          9U
#define SPIINT0_TXINTENA_BIT_END                            9U

/* Definition for field NU3 in Register SPIINT0 */
#define SPIINT0_NU3_BIT_START                               10U
#define SPIINT0_NU3_BIT_END                                 15U

/* Definition for field DMAREQEN in Register SPIINT0 */
#define SPIINT0_DMAREQEN_BIT_START                          16U
#define SPIINT0_DMAREQEN_BIT_END                            16U

/* Definition for field NU4 in Register SPIINT0 */
#define SPIINT0_NU4_BIT_START                               17U
#define SPIINT0_NU4_BIT_END                                 23U

/* Definition for field ENABLEHIGHZ in Register SPIINT0 */
#define SPIINT0_ENABLEHIGHZ_BIT_START                       24U
#define SPIINT0_ENABLEHIGHZ_BIT_END                         24U

/* Definition for field NU5 in Register SPIINT0 */
#define SPIINT0_NU5_BIT_START                               25U
#define SPIINT0_NU5_BIT_END                                 31U

/* Definition for field DLENERRLVL in Register SPILVL */
#define SPILVL_DLENERRLVL_BIT_START                         0U
#define SPILVL_DLENERRLVL_BIT_END                           0U

/* Definition for field TIMEOUTLVL in Register SPILVL */
#define SPILVL_TIMEOUTLVL_BIT_START                         1U
#define SPILVL_TIMEOUTLVL_BIT_END                           1U

/* Definition for field PARERRLVL in Register SPILVL */
#define SPILVL_PARERRLVL_BIT_START                          2U
#define SPILVL_PARERRLVL_BIT_END                            2U

/* Definition for field DESYNCLVL in Register SPILVL */
#define SPILVL_DESYNCLVL_BIT_START                          3U
#define SPILVL_DESYNCLVL_BIT_END                            3U

/* Definition for field BITERRLVL in Register SPILVL */
#define SPILVL_BITERRLVL_BIT_START                          4U
#define SPILVL_BITERRLVL_BIT_END                            4U

/* Definition for field NU1 in Register SPILVL */
#define SPILVL_NU1_BIT_START                                5U
#define SPILVL_NU1_BIT_END                                  5U

/* Definition for field OVRNINTLVL in Register SPILVL */
#define SPILVL_OVRNINTLVL_BIT_START                         6U
#define SPILVL_OVRNINTLVL_BIT_END                           6U

/* Definition for field NU2 in Register SPILVL */
#define SPILVL_NU2_BIT_START                                7U
#define SPILVL_NU2_BIT_END                                  7U

/* Definition for field RXINTLVL in Register SPILVL */
#define SPILVL_RXINTLVL_BIT_START                           8U
#define SPILVL_RXINTLVL_BIT_END                             8U

/* Definition for field TXINTLVL in Register SPILVL */
#define SPILVL_TXINTLVL_BIT_START                           9U
#define SPILVL_TXINTLVL_BIT_END                             9U

/* Definition for field NU3 in Register SPILVL */
#define SPILVL_NU3_BIT_START                                10U
#define SPILVL_NU3_BIT_END                                  31U

/* Definition for field DLENERRFLG in Register SPIFLG */
#define SPIFLG_DLENERRFLG_BIT_START                         0U
#define SPIFLG_DLENERRFLG_BIT_END                           0U

/* Definition for field TIMEOUTFLG in Register SPIFLG */
#define SPIFLG_TIMEOUTFLG_BIT_START                         1U
#define SPIFLG_TIMEOUTFLG_BIT_END                           1U

/* Definition for field PARERRFLG in Register SPIFLG */
#define SPIFLG_PARERRFLG_BIT_START                          2U
#define SPIFLG_PARERRFLG_BIT_END                            2U

/* Definition for field DESYNCFLG in Register SPIFLG */
#define SPIFLG_DESYNCFLG_BIT_START                          3U
#define SPIFLG_DESYNCFLG_BIT_END                            3U

/* Definition for field BITERRFLG in Register SPIFLG */
#define SPIFLG_BITERRFLG_BIT_START                          4U
#define SPIFLG_BITERRFLG_BIT_END                            4U

/* Definition for field NU1 in Register SPIFLG */
#define SPIFLG_NU1_BIT_START                                5U
#define SPIFLG_NU1_BIT_END                                  5U

/* Definition for field OVRNINTFLG in Register SPIFLG */
#define SPIFLG_OVRNINTFLG_BIT_START                         6U
#define SPIFLG_OVRNINTFLG_BIT_END                           6U

/* Definition for field NU2 in Register SPIFLG */
#define SPIFLG_NU2_BIT_START                                7U
#define SPIFLG_NU2_BIT_END                                  7U

/* Definition for field RXINTFLG in Register SPIFLG */
#define SPIFLG_RXINTFLG_BIT_START                           8U
#define SPIFLG_RXINTFLG_BIT_END                             8U

/* Definition for field TXINTFLG in Register SPIFLG */
#define SPIFLG_TXINTFLG_BIT_START                           9U
#define SPIFLG_TXINTFLG_BIT_END                             9U

/* Definition for field NU3 in Register SPIFLG */
#define SPIFLG_NU3_BIT_START                                10U
#define SPIFLG_NU3_BIT_END                                  23U

/* Definition for field BUFINITACTIVE in Register SPIFLG */
#define SPIFLG_BUFINITACTIVE_BIT_START                      24U
#define SPIFLG_BUFINITACTIVE_BIT_END                        24U

/* Definition for field NU4 in Register SPIFLG */
#define SPIFLG_NU4_BIT_START                                25U
#define SPIFLG_NU4_BIT_END                                  31U

/* Definition for field SCSFUN in Register SPIPC0 */
#define SPIPC0_SCSFUN_BIT_START                             0U
#define SPIPC0_SCSFUN_BIT_END                               7U

/* Definition for field ENAFUN in Register SPIPC0 */
#define SPIPC0_ENAFUN_BIT_START                             8U
#define SPIPC0_ENAFUN_BIT_END                               8U

/* Definition for field CLKFUN in Register SPIPC0 */
#define SPIPC0_CLKFUN_BIT_START                             9U
#define SPIPC0_CLKFUN_BIT_END                               9U

/* Definition for field SIMOFUN0 in Register SPIPC0 */
#define SPIPC0_SIMOFUN0_BIT_START                           10U
#define SPIPC0_SIMOFUN0_BIT_END                             10U

/* Definition for field SOMIFUN0 in Register SPIPC0 */
#define SPIPC0_SOMIFUN0_BIT_START                           11U
#define SPIPC0_SOMIFUN0_BIT_END                             11U

/* Definition for field NU in Register SPIPC0 */
#define SPIPC0_NU_BIT_START                                 12U
#define SPIPC0_NU_BIT_END                                   15U

/* Definition for field SIMOFUN in Register SPIPC0 */
#define SPIPC0_SIMOFUN_BIT_START                            16U
#define SPIPC0_SIMOFUN_BIT_END                              23U

/* Definition for field SOMIFUN in Register SPIPC0 */
#define SPIPC0_SOMIFUN_BIT_START                            24U
#define SPIPC0_SOMIFUN_BIT_END                              31U

/* Definition for field SCSDIR in Register SPIPC1 */
#define SPIPC1_SCSDIR_BIT_START                             0U
#define SPIPC1_SCSDIR_BIT_END                               7U

/* Definition for field ENADIR in Register SPIPC1 */
#define SPIPC1_ENADIR_BIT_START                             8U
#define SPIPC1_ENADIR_BIT_END                               8U

/* Definition for field CLKDIR in Register SPIPC1 */
#define SPIPC1_CLKDIR_BIT_START                             9U
#define SPIPC1_CLKDIR_BIT_END                               9U

/* Definition for field SIMODIR0 in Register SPIPC1 */
#define SPIPC1_SIMODIR0_BIT_START                           10U
#define SPIPC1_SIMODIR0_BIT_END                             10U

/* Definition for field SOMIDIR0 in Register SPIPC1 */
#define SPIPC1_SOMIDIR0_BIT_START                           11U
#define SPIPC1_SOMIDIR0_BIT_END                             11U

/* Definition for field NU in Register SPIPC1 */
#define SPIPC1_NU_BIT_START                                 12U
#define SPIPC1_NU_BIT_END                                   15U

/* Definition for field SIMODIR in Register SPIPC1 */
#define SPIPC1_SIMODIR_BIT_START                            16U
#define SPIPC1_SIMODIR_BIT_END                              23U

/* Definition for field SOMIDIR in Register SPIPC1 */
#define SPIPC1_SOMIDIR_BIT_START                            24U
#define SPIPC1_SOMIDIR_BIT_END                              31U

/* Definition for field SCSDIN in Register SPIPC2 */
#define SPIPC2_SCSDIN_BIT_START                             0U
#define SPIPC2_SCSDIN_BIT_END                               7U

/* Definition for field ENADIN in Register SPIPC2 */
#define SPIPC2_ENADIN_BIT_START                             8U
#define SPIPC2_ENADIN_BIT_END                               8U

/* Definition for field CLKDIN in Register SPIPC2 */
#define SPIPC2_CLKDIN_BIT_START                             9U
#define SPIPC2_CLKDIN_BIT_END                               9U

/* Definition for field SIMODIN0 in Register SPIPC2 */
#define SPIPC2_SIMODIN0_BIT_START                           10U
#define SPIPC2_SIMODIN0_BIT_END                             10U

/* Definition for field SOMIDIN0 in Register SPIPC2 */
#define SPIPC2_SOMIDIN0_BIT_START                           11U
#define SPIPC2_SOMIDIN0_BIT_END                             11U

/* Definition for field NU in Register SPIPC2 */
#define SPIPC2_NU_BIT_START                                 12U
#define SPIPC2_NU_BIT_END                                   15U

/* Definition for field SIMODIN in Register SPIPC2 */
#define SPIPC2_SIMODIN_BIT_START                            16U
#define SPIPC2_SIMODIN_BIT_END                              23U

/* Definition for field SOMIDIN in Register SPIPC2 */
#define SPIPC2_SOMIDIN_BIT_START                            24U
#define SPIPC2_SOMIDIN_BIT_END                              31U

/* Definition for field SCSDOUT in Register SPIPC3 */
#define SPIPC3_SCSDOUT_BIT_START                            0U
#define SPIPC3_SCSDOUT_BIT_END                              7U

/* Definition for field ENADOUT in Register SPIPC3 */
#define SPIPC3_ENADOUT_BIT_START                            8U
#define SPIPC3_ENADOUT_BIT_END                              8U

/* Definition for field CLKDOUT in Register SPIPC3 */
#define SPIPC3_CLKDOUT_BIT_START                            9U
#define SPIPC3_CLKDOUT_BIT_END                              9U

/* Definition for field SIMODOUT0 in Register SPIPC3 */
#define SPIPC3_SIMODOUT0_BIT_START                          10U
#define SPIPC3_SIMODOUT0_BIT_END                            10U

/* Definition for field SOMIDOUT0 in Register SPIPC3 */
#define SPIPC3_SOMIDOUT0_BIT_START                          11U
#define SPIPC3_SOMIDOUT0_BIT_END                            11U

/* Definition for field NU in Register SPIPC3 */
#define SPIPC3_NU_BIT_START                                 12U
#define SPIPC3_NU_BIT_END                                   15U

/* Definition for field SIMODOUT in Register SPIPC3 */
#define SPIPC3_SIMODOUT_BIT_START                           16U
#define SPIPC3_SIMODOUT_BIT_END                             23U

/* Definition for field SOMIDOUT in Register SPIPC3 */
#define SPIPC3_SOMIDOUT_BIT_START                           24U
#define SPIPC3_SOMIDOUT_BIT_END                             31U

/* Definition for field SCSSET in Register SPIPC4 */
#define SPIPC4_SCSSET_BIT_START                             0U
#define SPIPC4_SCSSET_BIT_END                               7U

/* Definition for field ENASET in Register SPIPC4 */
#define SPIPC4_ENASET_BIT_START                             8U
#define SPIPC4_ENASET_BIT_END                               8U

/* Definition for field CLKSET in Register SPIPC4 */
#define SPIPC4_CLKSET_BIT_START                             9U
#define SPIPC4_CLKSET_BIT_END                               9U

/* Definition for field SIMOSET0 in Register SPIPC4 */
#define SPIPC4_SIMOSET0_BIT_START                           10U
#define SPIPC4_SIMOSET0_BIT_END                             10U

/* Definition for field SOMISET0 in Register SPIPC4 */
#define SPIPC4_SOMISET0_BIT_START                           11U
#define SPIPC4_SOMISET0_BIT_END                             11U

/* Definition for field NU in Register SPIPC4 */
#define SPIPC4_NU_BIT_START                                 12U
#define SPIPC4_NU_BIT_END                                   15U

/* Definition for field SIMOSET in Register SPIPC4 */
#define SPIPC4_SIMOSET_BIT_START                            16U
#define SPIPC4_SIMOSET_BIT_END                              23U

/* Definition for field SOMISET in Register SPIPC4 */
#define SPIPC4_SOMISET_BIT_START                            24U
#define SPIPC4_SOMISET_BIT_END                              31U

/* Definition for field SCSCLR in Register SPIPC5 */
#define SPIPC5_SCSCLR_BIT_START                             0U
#define SPIPC5_SCSCLR_BIT_END                               7U

/* Definition for field ENACLR in Register SPIPC5 */
#define SPIPC5_ENACLR_BIT_START                             8U
#define SPIPC5_ENACLR_BIT_END                               8U

/* Definition for field CLKCLR in Register SPIPC5 */
#define SPIPC5_CLKCLR_BIT_START                             9U
#define SPIPC5_CLKCLR_BIT_END                               9U

/* Definition for field SIMOCLR0 in Register SPIPC5 */
#define SPIPC5_SIMOCLR0_BIT_START                           10U
#define SPIPC5_SIMOCLR0_BIT_END                             10U

/* Definition for field SOMICLR0 in Register SPIPC5 */
#define SPIPC5_SOMICLR0_BIT_START                           11U
#define SPIPC5_SOMICLR0_BIT_END                             11U

/* Definition for field NU in Register SPIPC5 */
#define SPIPC5_NU_BIT_START                                 12U
#define SPIPC5_NU_BIT_END                                   15U

/* Definition for field SIMOCLR in Register SPIPC5 */
#define SPIPC5_SIMOCLR_BIT_START                            16U
#define SPIPC5_SIMOCLR_BIT_END                              23U

/* Definition for field SOMICLR in Register SPIPC5 */
#define SPIPC5_SOMICLR_BIT_START                            24U
#define SPIPC5_SOMICLR_BIT_END                              31U

/* Definition for field SCSPDR in Register SPIPC6 */
#define SPIPC6_SCSPDR_BIT_START                             0U
#define SPIPC6_SCSPDR_BIT_END                               7U

/* Definition for field ENAPDR in Register SPIPC6 */
#define SPIPC6_ENAPDR_BIT_START                             8U
#define SPIPC6_ENAPDR_BIT_END                               8U

/* Definition for field CLKPDR in Register SPIPC6 */
#define SPIPC6_CLKPDR_BIT_START                             9U
#define SPIPC6_CLKPDR_BIT_END                               9U

/* Definition for field SIMOPDR0 in Register SPIPC6 */
#define SPIPC6_SIMOPDR0_BIT_START                           10U
#define SPIPC6_SIMOPDR0_BIT_END                             10U

/* Definition for field SOMIPDR0 in Register SPIPC6 */
#define SPIPC6_SOMIPDR0_BIT_START                           11U
#define SPIPC6_SOMIPDR0_BIT_END                             11U

/* Definition for field NU in Register SPIPC6 */
#define SPIPC6_NU_BIT_START                                 12U
#define SPIPC6_NU_BIT_END                                   15U

/* Definition for field SIMOPDR in Register SPIPC6 */
#define SPIPC6_SIMOPDR_BIT_START                            16U
#define SPIPC6_SIMOPDR_BIT_END                              23U

/* Definition for field SOMIPDR in Register SPIPC6 */
#define SPIPC6_SOMIPDR_BIT_START                            24U
#define SPIPC6_SOMIPDR_BIT_END                              31U

/* Definition for field TXDATA in Register SPIDAT0 */
#define SPIDAT0_TXDATA_BIT_START                            0U
#define SPIDAT0_TXDATA_BIT_END                              15U

/* Definition for field NU in Register SPIDAT0 */
#define SPIDAT0_NU_BIT_START                                16U
#define SPIDAT0_NU_BIT_END                                  31U

/* Definition for field TXDATA in Register SPIDAT1 */
#define SPIDAT1_TXDATA_BIT_START                            0U
#define SPIDAT1_TXDATA_BIT_END                              15U

/* Definition for field CSNR in Register SPIDAT1 */
#define SPIDAT1_CSNR_BIT_START                              16U
#define SPIDAT1_CSNR_BIT_END                                23U

/* Definition for field DFSEL in Register SPIDAT1 */
#define SPIDAT1_DFSEL_BIT_START                             24U
#define SPIDAT1_DFSEL_BIT_END                               25U

/* Definition for field WDEL in Register SPIDAT1 */
#define SPIDAT1_WDEL_BIT_START                              26U
#define SPIDAT1_WDEL_BIT_END                                26U

/* Definition for field NU1 in Register SPIDAT1 */
#define SPIDAT1_NU1_BIT_START                               27U
#define SPIDAT1_NU1_BIT_END                                 27U

/* Definition for field CSHOLD in Register SPIDAT1 */
#define SPIDAT1_CSHOLD_BIT_START                            28U
#define SPIDAT1_CSHOLD_BIT_END                              28U

/* Definition for field NU2 in Register SPIDAT1 */
#define SPIDAT1_NU2_BIT_START                               29U
#define SPIDAT1_NU2_BIT_END                                 31U

/* Definition for field RXDATA in Register SPIBUF */
#define SPIBUF_RXDATA_BIT_START                             0U
#define SPIBUF_RXDATA_BIT_END                               15U

/* Definition for field LCSNR in Register SPIBUF */
#define SPIBUF_LCSNR_BIT_START                              16U
#define SPIBUF_LCSNR_BIT_END                                23U

/* Definition for field DLENERR in Register SPIBUF */
#define SPIBUF_DLENERR_BIT_START                            24U
#define SPIBUF_DLENERR_BIT_END                              24U

/* Definition for field TIMEOUT in Register SPIBUF */
#define SPIBUF_TIMEOUT_BIT_START                            25U
#define SPIBUF_TIMEOUT_BIT_END                              25U

/* Definition for field PARITYERR in Register SPIBUF */
#define SPIBUF_PARITYERR_BIT_START                          26U
#define SPIBUF_PARITYERR_BIT_END                            26U

/* Definition for field DESYNC in Register SPIBUF */
#define SPIBUF_DESYNC_BIT_START                             27U
#define SPIBUF_DESYNC_BIT_END                               27U

/* Definition for field BITERR in Register SPIBUF */
#define SPIBUF_BITERR_BIT_START                             28U
#define SPIBUF_BITERR_BIT_END                               28U

/* Definition for field TXFULL in Register SPIBUF */
#define SPIBUF_TXFULL_BIT_START                             29U
#define SPIBUF_TXFULL_BIT_END                               29U

/* Definition for field RXOVR in Register SPIBUF */
#define SPIBUF_RXOVR_BIT_START                              30U
#define SPIBUF_RXOVR_BIT_END                                30U

/* Definition for field RXEMPTY in Register SPIBUF */
#define SPIBUF_RXEMPTY_BIT_START                            31U
#define SPIBUF_RXEMPTY_BIT_END                              31U

/* Definition for field RXDATA in Register SPIEMU */
#define SPIEMU_RXDATA_BIT_START                             0U
#define SPIEMU_RXDATA_BIT_END                               15U

/* Definition for field LCSNR in Register SPIEMU */
#define SPIEMU_LCSNR_BIT_START                              16U
#define SPIEMU_LCSNR_BIT_END                                23U

/* Definition for field DLENERR in Register SPIEMU */
#define SPIEMU_DLENERR_BIT_START                            24U
#define SPIEMU_DLENERR_BIT_END                              24U

/* Definition for field TIMEOUT in Register SPIEMU */
#define SPIEMU_TIMEOUT_BIT_START                            25U
#define SPIEMU_TIMEOUT_BIT_END                              25U

/* Definition for field PARITYERR in Register SPIEMU */
#define SPIEMU_PARITYERR_BIT_START                          26U
#define SPIEMU_PARITYERR_BIT_END                            26U

/* Definition for field DESYNC in Register SPIEMU */
#define SPIEMU_DESYNC_BIT_START                             27U
#define SPIEMU_DESYNC_BIT_END                               27U

/* Definition for field BITERR in Register SPIEMU */
#define SPIEMU_BITERR_BIT_START                             28U
#define SPIEMU_BITERR_BIT_END                               28U

/* Definition for field TXFULL in Register SPIEMU */
#define SPIEMU_TXFULL_BIT_START                             29U
#define SPIEMU_TXFULL_BIT_END                               29U

/* Definition for field RXOVR in Register SPIEMU */
#define SPIEMU_RXOVR_BIT_START                              30U
#define SPIEMU_RXOVR_BIT_END                                30U

/* Definition for field RXEMPTY in Register SPIEMU */
#define SPIEMU_RXEMPTY_BIT_START                            31U
#define SPIEMU_RXEMPTY_BIT_END                              31U

/* Definition for field C2EDELAY in Register SPIDELAY */
#define SPIDELAY_C2EDELAY_BIT_START                         0U
#define SPIDELAY_C2EDELAY_BIT_END                           7U

/* Definition for field T2EDELAY in Register SPIDELAY */
#define SPIDELAY_T2EDELAY_BIT_START                         8U
#define SPIDELAY_T2EDELAY_BIT_END                           15U

/* Definition for field T2CDELAY in Register SPIDELAY */
#define SPIDELAY_T2CDELAY_BIT_START                         16U
#define SPIDELAY_T2CDELAY_BIT_END                           23U

/* Definition for field C2TDELAY in Register SPIDELAY */
#define SPIDELAY_C2TDELAY_BIT_START                         24U
#define SPIDELAY_C2TDELAY_BIT_END                           31U

/* Definition for field CSDEF0 in Register SPIDEF */
#define SPIDEF_CSDEF0_BIT_START                             0U
#define SPIDEF_CSDEF0_BIT_END                               7U

/* Definition for field NU in Register SPIDEF */
#define SPIDEF_NU_BIT_START                                 8U
#define SPIDEF_NU_BIT_END                                   31U

/* Definition for field CHARLEN in Register SPIFMT0 */
#define SPIFMT0_CHARLEN_BIT_START                           0U
#define SPIFMT0_CHARLEN_BIT_END                             4U

/* Definition for field NU in Register SPIFMT0 */
#define SPIFMT0_NU_BIT_START                                5U
#define SPIFMT0_NU_BIT_END                                  7U

/* Definition for field PRESCALE in Register SPIFMT0 */
#define SPIFMT0_PRESCALE_BIT_START                          8U
#define SPIFMT0_PRESCALE_BIT_END                            15U

/* Definition for field PHASE in Register SPIFMT0 */
#define SPIFMT0_PHASE_BIT_START                             16U
#define SPIFMT0_PHASE_BIT_END                               16U

/* Definition for field POLARITY in Register SPIFMT0 */
#define SPIFMT0_POLARITY_BIT_START                          17U
#define SPIFMT0_POLARITY_BIT_END                            17U

/* Definition for field DISCSTIMERS in Register SPIFMT0 */
#define SPIFMT0_DISCSTIMERS_BIT_START                       18U
#define SPIFMT0_DISCSTIMERS_BIT_END                         18U

/* Definition for field HDUPLEX_ENA in Register SPIFMT0 */
#define SPIFMT0_HDUPLEX_ENA_BIT_START                       19U
#define SPIFMT0_HDUPLEX_ENA_BIT_END                         19U

/* Definition for field SHIFTDIR in Register SPIFMT0 */
#define SPIFMT0_SHIFTDIR_BIT_START                          20U
#define SPIFMT0_SHIFTDIR_BIT_END                            20U

/* Definition for field WAITENA in Register SPIFMT0 */
#define SPIFMT0_WAITENA_BIT_START                           21U
#define SPIFMT0_WAITENA_BIT_END                             21U

/* Definition for field PARITYENA in Register SPIFMT0 */
#define SPIFMT0_PARITYENA_BIT_START                         22U
#define SPIFMT0_PARITYENA_BIT_END                           22U

/* Definition for field PARPOL in Register SPIFMT0 */
#define SPIFMT0_PARPOL_BIT_START                            23U
#define SPIFMT0_PARPOL_BIT_END                              23U

/* Definition for field WDELAY in Register SPIFMT0 */
#define SPIFMT0_WDELAY_BIT_START                            24U
#define SPIFMT0_WDELAY_BIT_END                              31U

/* Definition for field CHARLEN in Register SPIFMT1 */
#define SPIFMT1_CHARLEN_BIT_START                           0U
#define SPIFMT1_CHARLEN_BIT_END                             4U

/* Definition for field NU in Register SPIFMT1 */
#define SPIFMT1_NU_BIT_START                                5U
#define SPIFMT1_NU_BIT_END                                  7U

/* Definition for field PRESCALE in Register SPIFMT1 */
#define SPIFMT1_PRESCALE_BIT_START                          8U
#define SPIFMT1_PRESCALE_BIT_END                            15U

/* Definition for field PHASE in Register SPIFMT1 */
#define SPIFMT1_PHASE_BIT_START                             16U
#define SPIFMT1_PHASE_BIT_END                               16U

/* Definition for field POLARITY in Register SPIFMT1 */
#define SPIFMT1_POLARITY_BIT_START                          17U
#define SPIFMT1_POLARITY_BIT_END                            17U

/* Definition for field DISCSTIMERS in Register SPIFMT1 */
#define SPIFMT1_DISCSTIMERS_BIT_START                       18U
#define SPIFMT1_DISCSTIMERS_BIT_END                         18U

/* Definition for field HDUPLEX_ENA in Register SPIFMT1 */
#define SPIFMT1_HDUPLEX_ENA_BIT_START                       19U
#define SPIFMT1_HDUPLEX_ENA_BIT_END                         19U

/* Definition for field SHIFTDIR in Register SPIFMT1 */
#define SPIFMT1_SHIFTDIR_BIT_START                          20U
#define SPIFMT1_SHIFTDIR_BIT_END                            20U

/* Definition for field WAITENA in Register SPIFMT1 */
#define SPIFMT1_WAITENA_BIT_START                           21U
#define SPIFMT1_WAITENA_BIT_END                             21U

/* Definition for field PARITYENA in Register SPIFMT1 */
#define SPIFMT1_PARITYENA_BIT_START                         22U
#define SPIFMT1_PARITYENA_BIT_END                           22U

/* Definition for field PARPOL in Register SPIFMT1 */
#define SPIFMT1_PARPOL_BIT_START                            23U
#define SPIFMT1_PARPOL_BIT_END                              23U

/* Definition for field WDELAY in Register SPIFMT1 */
#define SPIFMT1_WDELAY_BIT_START                            24U
#define SPIFMT1_WDELAY_BIT_END                              31U

/* Definition for field CHARLEN in Register SPIFMT2 */
#define SPIFMT2_CHARLEN_BIT_START                           0U
#define SPIFMT2_CHARLEN_BIT_END                             4U

/* Definition for field NU in Register SPIFMT2 */
#define SPIFMT2_NU_BIT_START                                5U
#define SPIFMT2_NU_BIT_END                                  7U

/* Definition for field PRESCALE in Register SPIFMT2 */
#define SPIFMT2_PRESCALE_BIT_START                          8U
#define SPIFMT2_PRESCALE_BIT_END                            15U

/* Definition for field PHASE in Register SPIFMT2 */
#define SPIFMT2_PHASE_BIT_START                             16U
#define SPIFMT2_PHASE_BIT_END                               16U

/* Definition for field POLARITY in Register SPIFMT2 */
#define SPIFMT2_POLARITY_BIT_START                          17U
#define SPIFMT2_POLARITY_BIT_END                            17U

/* Definition for field DISCSTIMERS in Register SPIFMT2 */
#define SPIFMT2_DISCSTIMERS_BIT_START                       18U
#define SPIFMT2_DISCSTIMERS_BIT_END                         18U

/* Definition for field HDUPLEX_ENA in Register SPIFMT2 */
#define SPIFMT2_HDUPLEX_ENA_BIT_START                       19U
#define SPIFMT2_HDUPLEX_ENA_BIT_END                         19U

/* Definition for field SHIFTDIR in Register SPIFMT2 */
#define SPIFMT2_SHIFTDIR_BIT_START                          20U
#define SPIFMT2_SHIFTDIR_BIT_END                            20U

/* Definition for field WAITENA in Register SPIFMT2 */
#define SPIFMT2_WAITENA_BIT_START                           21U
#define SPIFMT2_WAITENA_BIT_END                             21U

/* Definition for field PARITYENA in Register SPIFMT2 */
#define SPIFMT2_PARITYENA_BIT_START                         22U
#define SPIFMT2_PARITYENA_BIT_END                           22U

/* Definition for field PARPOL in Register SPIFMT2 */
#define SPIFMT2_PARPOL_BIT_START                            23U
#define SPIFMT2_PARPOL_BIT_END                              23U

/* Definition for field WDELAY in Register SPIFMT2 */
#define SPIFMT2_WDELAY_BIT_START                            24U
#define SPIFMT2_WDELAY_BIT_END                              31U

/* Definition for field CHARLEN in Register SPIFMT3 */
#define SPIFMT3_CHARLEN_BIT_START                           0U
#define SPIFMT3_CHARLEN_BIT_END                             4U

/* Definition for field NU in Register SPIFMT3 */
#define SPIFMT3_NU_BIT_START                                5U
#define SPIFMT3_NU_BIT_END                                  7U

/* Definition for field PRESCALE in Register SPIFMT3 */
#define SPIFMT3_PRESCALE_BIT_START                          8U
#define SPIFMT3_PRESCALE_BIT_END                            15U

/* Definition for field PHASE in Register SPIFMT3 */
#define SPIFMT3_PHASE_BIT_START                             16U
#define SPIFMT3_PHASE_BIT_END                               16U

/* Definition for field POLARITY in Register SPIFMT3 */
#define SPIFMT3_POLARITY_BIT_START                          17U
#define SPIFMT3_POLARITY_BIT_END                            17U

/* Definition for field DISCSTIMERS in Register SPIFMT3 */
#define SPIFMT3_DISCSTIMERS_BIT_START                       18U
#define SPIFMT3_DISCSTIMERS_BIT_END                         18U

/* Definition for field HDUPLEX_ENA in Register SPIFMT3 */
#define SPIFMT3_HDUPLEX_ENA_BIT_START                       19U
#define SPIFMT3_HDUPLEX_ENA_BIT_END                         19U

/* Definition for field SHIFTDIR in Register SPIFMT3 */
#define SPIFMT3_SHIFTDIR_BIT_START                          20U
#define SPIFMT3_SHIFTDIR_BIT_END                            20U

/* Definition for field WAITENA in Register SPIFMT3 */
#define SPIFMT3_WAITENA_BIT_START                           21U
#define SPIFMT3_WAITENA_BIT_END                             21U

/* Definition for field PARITYENA in Register SPIFMT3 */
#define SPIFMT3_PARITYENA_BIT_START                         22U
#define SPIFMT3_PARITYENA_BIT_END                           22U

/* Definition for field PARPOL in Register SPIFMT3 */
#define SPIFMT3_PARPOL_BIT_START                            23U
#define SPIFMT3_PARPOL_BIT_END                              23U

/* Definition for field WDELAY in Register SPIFMT3 */
#define SPIFMT3_WDELAY_BIT_START                            24U
#define SPIFMT3_WDELAY_BIT_END                              31U

/* Definition for field SUSPEND0 in Register TGINTVECT0 */
#define TGINTVECT0_SUSPEND0_BIT_START                       0U
#define TGINTVECT0_SUSPEND0_BIT_END                         0U

/* Definition for field INTVECT0 in Register TGINTVECT0 */
#define TGINTVECT0_INTVECT0_BIT_START                       1U
#define TGINTVECT0_INTVECT0_BIT_END                         5U

/* Definition for field NU in Register TGINTVECT0 */
#define TGINTVECT0_NU_BIT_START                             6U
#define TGINTVECT0_NU_BIT_END                               31U

/* Definition for field SUSPEND1 in Register TGINTVECT1 */
#define TGINTVECT1_SUSPEND1_BIT_START                       0U
#define TGINTVECT1_SUSPEND1_BIT_END                         0U

/* Definition for field INTVECT1 in Register TGINTVECT1 */
#define TGINTVECT1_INTVECT1_BIT_START                       1U
#define TGINTVECT1_INTVECT1_BIT_END                         5U

/* Definition for field NU in Register TGINTVECT1 */
#define TGINTVECT1_NU_BIT_START                             6U
#define TGINTVECT1_NU_BIT_END                               31U

/* Definition for field SCSSRS in Register SPIPC9 */
#define SPIPC9_SCSSRS_BIT_START                             0U
#define SPIPC9_SCSSRS_BIT_END                               7U

/* Definition for field ENASRS in Register SPIPC9 */
#define SPIPC9_ENASRS_BIT_START                             8U
#define SPIPC9_ENASRS_BIT_END                               8U

/* Definition for field CLKSRS in Register SPIPC9 */
#define SPIPC9_CLKSRS_BIT_START                             9U
#define SPIPC9_CLKSRS_BIT_END                               9U

/* Definition for field SIMOSRS0 in Register SPIPC9 */
#define SPIPC9_SIMOSRS0_BIT_START                           10U
#define SPIPC9_SIMOSRS0_BIT_END                             10U

/* Definition for field SOMISRS0 in Register SPIPC9 */
#define SPIPC9_SOMISRS0_BIT_START                           11U
#define SPIPC9_SOMISRS0_BIT_END                             11U

/* Definition for field NU in Register SPIPC9 */
#define SPIPC9_NU_BIT_START                                 12U
#define SPIPC9_NU_BIT_END                                   15U

/* Definition for field SIMOSRS7 in Register SPIPC9 */
#define SPIPC9_SIMOSRS7_BIT_START                           16U
#define SPIPC9_SIMOSRS7_BIT_END                             23U

/* Definition for field SOMISRS7 in Register SPIPC9 */
#define SPIPC9_SOMISRS7_BIT_START                           24U
#define SPIPC9_SOMISRS7_BIT_END                             31U

/* Definition for field PMODE0 in Register SPIPMCTRL */
#define SPIPMCTRL_PMODE0_BIT_START                          0U
#define SPIPMCTRL_PMODE0_BIT_END                            1U

/* Definition for field MMODE0 in Register SPIPMCTRL */
#define SPIPMCTRL_MMODE0_BIT_START                          2U
#define SPIPMCTRL_MMODE0_BIT_END                            4U

/* Definition for field MODCLKPOL0 in Register SPIPMCTRL */
#define SPIPMCTRL_MODCLKPOL0_BIT_START                      5U
#define SPIPMCTRL_MODCLKPOL0_BIT_END                        5U

/* Definition for field HSM_MODE0 in Register SPIPMCTRL */
#define SPIPMCTRL_HSM_MODE0_BIT_START                       6U
#define SPIPMCTRL_HSM_MODE0_BIT_END                         6U

/* Definition for field NU1 in Register SPIPMCTRL */
#define SPIPMCTRL_NU1_BIT_START                             7U
#define SPIPMCTRL_NU1_BIT_END                               7U

/* Definition for field PMODE1 in Register SPIPMCTRL */
#define SPIPMCTRL_PMODE1_BIT_START                          8U
#define SPIPMCTRL_PMODE1_BIT_END                            9U

/* Definition for field MMODE1 in Register SPIPMCTRL */
#define SPIPMCTRL_MMODE1_BIT_START                          10U
#define SPIPMCTRL_MMODE1_BIT_END                            12U

/* Definition for field MODCLKPOL1 in Register SPIPMCTRL */
#define SPIPMCTRL_MODCLKPOL1_BIT_START                      13U
#define SPIPMCTRL_MODCLKPOL1_BIT_END                        13U

/* Definition for field HSM_MODE1 in Register SPIPMCTRL */
#define SPIPMCTRL_HSM_MODE1_BIT_START                       14U
#define SPIPMCTRL_HSM_MODE1_BIT_END                         14U

/* Definition for field NU2 in Register SPIPMCTRL */
#define SPIPMCTRL_NU2_BIT_START                             15U
#define SPIPMCTRL_NU2_BIT_END                               15U

/* Definition for field PMODE2 in Register SPIPMCTRL */
#define SPIPMCTRL_PMODE2_BIT_START                          16U
#define SPIPMCTRL_PMODE2_BIT_END                            17U

/* Definition for field MMODE2 in Register SPIPMCTRL */
#define SPIPMCTRL_MMODE2_BIT_START                          18U
#define SPIPMCTRL_MMODE2_BIT_END                            20U

/* Definition for field MODCLKPOL2 in Register SPIPMCTRL */
#define SPIPMCTRL_MODCLKPOL2_BIT_START                      21U
#define SPIPMCTRL_MODCLKPOL2_BIT_END                        21U

/* Definition for field HSM_MODE2 in Register SPIPMCTRL */
#define SPIPMCTRL_HSM_MODE2_BIT_START                       22U
#define SPIPMCTRL_HSM_MODE2_BIT_END                         22U

/* Definition for field NU3 in Register SPIPMCTRL */
#define SPIPMCTRL_NU3_BIT_START                             23U
#define SPIPMCTRL_NU3_BIT_END                               23U

/* Definition for field PMODE3 in Register SPIPMCTRL */
#define SPIPMCTRL_PMODE3_BIT_START                          24U
#define SPIPMCTRL_PMODE3_BIT_END                            25U

/* Definition for field MMODE3 in Register SPIPMCTRL */
#define SPIPMCTRL_MMODE3_BIT_START                          26U
#define SPIPMCTRL_MMODE3_BIT_END                            28U

/* Definition for field MODCLKPOL3 in Register SPIPMCTRL */
#define SPIPMCTRL_MODCLKPOL3_BIT_START                      29U
#define SPIPMCTRL_MODCLKPOL3_BIT_END                        29U

/* Definition for field HSM_MODE3 in Register SPIPMCTRL */
#define SPIPMCTRL_HSM_MODE3_BIT_START                       30U
#define SPIPMCTRL_HSM_MODE3_BIT_END                         30U

/* Definition for field NU4 in Register SPIPMCTRL */
#define SPIPMCTRL_NU4_BIT_START                             31U
#define SPIPMCTRL_NU4_BIT_END                               31U

/* Definition for field MSPIENA in Register MIBSPIE */
#define MIBSPIE_MSPIENA_BIT_START                           0U
#define MIBSPIE_MSPIENA_BIT_END                             0U

/* Definition for field NU1 in Register MIBSPIE */
#define MIBSPIE_NU1_BIT_START                               1U
#define MIBSPIE_NU1_BIT_END                                 7U

/* Definition for field EXTENDED_BUF_ENA in Register MIBSPIE */
#define MIBSPIE_EXTENDED_BUF_ENA_BIT_START                  8U
#define MIBSPIE_EXTENDED_BUF_ENA_BIT_END                    11U

/* Definition for field NU2 in Register MIBSPIE */
#define MIBSPIE_NU2_BIT_START                               12U
#define MIBSPIE_NU2_BIT_END                                 15U

/* Definition for field RXRAMACCESS in Register MIBSPIE */
#define MIBSPIE_RXRAMACCESS_BIT_START                       16U
#define MIBSPIE_RXRAMACCESS_BIT_END                         16U

/* Definition for field NU3 in Register MIBSPIE */
#define MIBSPIE_NU3_BIT_START                               17U
#define MIBSPIE_NU3_BIT_END                                 31U

/* Definition for field SETINTENSUS in Register TGITENST */
#define TGITENST_SETINTENSUS_BIT_START                      0U
#define TGITENST_SETINTENSUS_BIT_END                        15U

/* Definition for field SETINTENRDY in Register TGITENST */
#define TGITENST_SETINTENRDY_BIT_START                      16U
#define TGITENST_SETINTENRDY_BIT_END                        31U

/* Definition for field CLRINTENSUS in Register TGITENCR */
#define TGITENCR_CLRINTENSUS_BIT_START                      0U
#define TGITENCR_CLRINTENSUS_BIT_END                        15U

/* Definition for field CLRINTENRDY in Register TGITENCR */
#define TGITENCR_CLRINTENRDY_BIT_START                      16U
#define TGITENCR_CLRINTENRDY_BIT_END                        31U

/* Definition for field SETINTLVLSUS in Register TGITLVST */
#define TGITLVST_SETINTLVLSUS_BIT_START                     0U
#define TGITLVST_SETINTLVLSUS_BIT_END                       15U

/* Definition for field SETINTLVLRDY in Register TGITLVST */
#define TGITLVST_SETINTLVLRDY_BIT_START                     16U
#define TGITLVST_SETINTLVLRDY_BIT_END                       31U

/* Definition for field CLRINTLVLSUS in Register TGITLVCR */
#define TGITLVCR_CLRINTLVLSUS_BIT_START                     0U
#define TGITLVCR_CLRINTLVLSUS_BIT_END                       15U

/* Definition for field CLRINTLVLRDY in Register TGITLVCR */
#define TGITLVCR_CLRINTLVLRDY_BIT_START                     16U
#define TGITLVCR_CLRINTLVLRDY_BIT_END                       31U

/* Definition for field INTFLGSUS in Register TGINTFLAG */
#define TGINTFLAG_INTFLGSUS_BIT_START                       0U
#define TGINTFLAG_INTFLGSUS_BIT_END                         15U

/* Definition for field INTFLGRDY in Register TGINTFLAG */
#define TGINTFLAG_INTFLGRDY_BIT_START                       16U
#define TGINTFLAG_INTFLGRDY_BIT_END                         31U

/* Definition for field TICKVALUE in Register TICKCNT */
#define TICKCNT_TICKVALUE_BIT_START                         0U
#define TICKCNT_TICKVALUE_BIT_END                           15U

/* Definition for field NU in Register TICKCNT */
#define TICKCNT_NU_BIT_START                                16U
#define TICKCNT_NU_BIT_END                                  27U

/* Definition for field CLKCTRL in Register TICKCNT */
#define TICKCNT_CLKCTRL_BIT_START                           28U
#define TICKCNT_CLKCTRL_BIT_END                             29U

/* Definition for field RELOAD in Register TICKCNT */
#define TICKCNT_RELOAD_BIT_START                            30U
#define TICKCNT_RELOAD_BIT_END                              30U

/* Definition for field TICKENA in Register TICKCNT */
#define TICKCNT_TICKENA_BIT_START                           31U
#define TICKCNT_TICKENA_BIT_END                             31U

/* Definition for field NU1 in Register LTGPEND */
#define LTGPEND_NU1_BIT_START                               0U
#define LTGPEND_NU1_BIT_END                                 7U

/* Definition for field LPEND in Register LTGPEND */
#define LTGPEND_LPEND_BIT_START                             8U
#define LTGPEND_LPEND_BIT_END                               15U

/* Definition for field NU2 in Register LTGPEND */
#define LTGPEND_NU2_BIT_START                               16U
#define LTGPEND_NU2_BIT_END                                 23U

/* Definition for field TGINSERVICE in Register LTGPEND */
#define LTGPEND_TGINSERVICE_BIT_START                       24U
#define LTGPEND_TGINSERVICE_BIT_END                         28U

/* Definition for field NU3 in Register LTGPEND */
#define LTGPEND_NU3_BIT_START                               29U
#define LTGPEND_NU3_BIT_END                                 31U

/* Definition for field PCURRENT in Register TG0CTRL */
#define TG0CTRL_PCURRENT_BIT_START                          0U
#define TG0CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG0CTRL */
#define TG0CTRL_PSTART_BIT_START                            8U
#define TG0CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG0CTRL */
#define TG0CTRL_TRIGSRC_BIT_START                           16U
#define TG0CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG0CTRL */
#define TG0CTRL_TRIGEVT_BIT_START                           20U
#define TG0CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG0CTRL */
#define TG0CTRL_NU_BIT_START                                24U
#define TG0CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG0CTRL */
#define TG0CTRL_TGTD_BIT_START                              28U
#define TG0CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG0CTRL */
#define TG0CTRL_PRST_BIT_START                              29U
#define TG0CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG0CTRL */
#define TG0CTRL_ONESHOT_BIT_START                           30U
#define TG0CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG0CTRL */
#define TG0CTRL_TGENA_BIT_START                             31U
#define TG0CTRL_TGENA_BIT_END                               31U

/* Definition for field PCURRENT in Register TG1CTRL */
#define TG1CTRL_PCURRENT_BIT_START                          0U
#define TG1CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG1CTRL */
#define TG1CTRL_PSTART_BIT_START                            8U
#define TG1CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG1CTRL */
#define TG1CTRL_TRIGSRC_BIT_START                           16U
#define TG1CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG1CTRL */
#define TG1CTRL_TRIGEVT_BIT_START                           20U
#define TG1CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG1CTRL */
#define TG1CTRL_NU_BIT_START                                24U
#define TG1CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG1CTRL */
#define TG1CTRL_TGTD_BIT_START                              28U
#define TG1CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG1CTRL */
#define TG1CTRL_PRST_BIT_START                              29U
#define TG1CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG1CTRL */
#define TG1CTRL_ONESHOT_BIT_START                           30U
#define TG1CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG1CTRL */
#define TG1CTRL_TGENA_BIT_START                             31U
#define TG1CTRL_TGENA_BIT_END                               31U

/* Definition for field PCURRENT in Register TG2CTRL */
#define TG2CTRL_PCURRENT_BIT_START                          0U
#define TG2CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG2CTRL */
#define TG2CTRL_PSTART_BIT_START                            8U
#define TG2CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG2CTRL */
#define TG2CTRL_TRIGSRC_BIT_START                           16U
#define TG2CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG2CTRL */
#define TG2CTRL_TRIGEVT_BIT_START                           20U
#define TG2CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG2CTRL */
#define TG2CTRL_NU_BIT_START                                24U
#define TG2CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG2CTRL */
#define TG2CTRL_TGTD_BIT_START                              28U
#define TG2CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG2CTRL */
#define TG2CTRL_PRST_BIT_START                              29U
#define TG2CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG2CTRL */
#define TG2CTRL_ONESHOT_BIT_START                           30U
#define TG2CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG2CTRL */
#define TG2CTRL_TGENA_BIT_START                             31U
#define TG2CTRL_TGENA_BIT_END                               31U

/* Definition for field PCURRENT in Register TG3CTRL */
#define TG3CTRL_PCURRENT_BIT_START                          0U
#define TG3CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG3CTRL */
#define TG3CTRL_PSTART_BIT_START                            8U
#define TG3CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG3CTRL */
#define TG3CTRL_TRIGSRC_BIT_START                           16U
#define TG3CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG3CTRL */
#define TG3CTRL_TRIGEVT_BIT_START                           20U
#define TG3CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG3CTRL */
#define TG3CTRL_NU_BIT_START                                24U
#define TG3CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG3CTRL */
#define TG3CTRL_TGTD_BIT_START                              28U
#define TG3CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG3CTRL */
#define TG3CTRL_PRST_BIT_START                              29U
#define TG3CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG3CTRL */
#define TG3CTRL_ONESHOT_BIT_START                           30U
#define TG3CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG3CTRL */
#define TG3CTRL_TGENA_BIT_START                             31U
#define TG3CTRL_TGENA_BIT_END                               31U

/* Definition for field PCURRENT in Register TG4CTRL */
#define TG4CTRL_PCURRENT_BIT_START                          0U
#define TG4CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG4CTRL */
#define TG4CTRL_PSTART_BIT_START                            8U
#define TG4CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG4CTRL */
#define TG4CTRL_TRIGSRC_BIT_START                           16U
#define TG4CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG4CTRL */
#define TG4CTRL_TRIGEVT_BIT_START                           20U
#define TG4CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG4CTRL */
#define TG4CTRL_NU_BIT_START                                24U
#define TG4CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG4CTRL */
#define TG4CTRL_TGTD_BIT_START                              28U
#define TG4CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG4CTRL */
#define TG4CTRL_PRST_BIT_START                              29U
#define TG4CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG4CTRL */
#define TG4CTRL_ONESHOT_BIT_START                           30U
#define TG4CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG4CTRL */
#define TG4CTRL_TGENA_BIT_START                             31U
#define TG4CTRL_TGENA_BIT_END                               31U

/* Definition for field PCURRENT in Register TG5CTRL */
#define TG5CTRL_PCURRENT_BIT_START                          0U
#define TG5CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG5CTRL */
#define TG5CTRL_PSTART_BIT_START                            8U
#define TG5CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG5CTRL */
#define TG5CTRL_TRIGSRC_BIT_START                           16U
#define TG5CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG5CTRL */
#define TG5CTRL_TRIGEVT_BIT_START                           20U
#define TG5CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG5CTRL */
#define TG5CTRL_NU_BIT_START                                24U
#define TG5CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG5CTRL */
#define TG5CTRL_TGTD_BIT_START                              28U
#define TG5CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG5CTRL */
#define TG5CTRL_PRST_BIT_START                              29U
#define TG5CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG5CTRL */
#define TG5CTRL_ONESHOT_BIT_START                           30U
#define TG5CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG5CTRL */
#define TG5CTRL_TGENA_BIT_START                             31U
#define TG5CTRL_TGENA_BIT_END                               31U

/* Definition for field PCURRENT in Register TG6CTRL */
#define TG6CTRL_PCURRENT_BIT_START                          0U
#define TG6CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG6CTRL */
#define TG6CTRL_PSTART_BIT_START                            8U
#define TG6CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG6CTRL */
#define TG6CTRL_TRIGSRC_BIT_START                           16U
#define TG6CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG6CTRL */
#define TG6CTRL_TRIGEVT_BIT_START                           20U
#define TG6CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG6CTRL */
#define TG6CTRL_NU_BIT_START                                24U
#define TG6CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG6CTRL */
#define TG6CTRL_TGTD_BIT_START                              28U
#define TG6CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG6CTRL */
#define TG6CTRL_PRST_BIT_START                              29U
#define TG6CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG6CTRL */
#define TG6CTRL_ONESHOT_BIT_START                           30U
#define TG6CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG6CTRL */
#define TG6CTRL_TGENA_BIT_START                             31U
#define TG6CTRL_TGENA_BIT_END                               31U

/* Definition for field PCURRENT in Register TG7CTRL */
#define TG7CTRL_PCURRENT_BIT_START                          0U
#define TG7CTRL_PCURRENT_BIT_END                            7U

/* Definition for field PSTART in Register TG7CTRL */
#define TG7CTRL_PSTART_BIT_START                            8U
#define TG7CTRL_PSTART_BIT_END                              15U

/* Definition for field TRIGSRC in Register TG7CTRL */
#define TG7CTRL_TRIGSRC_BIT_START                           16U
#define TG7CTRL_TRIGSRC_BIT_END                             19U

/* Definition for field TRIGEVT in Register TG7CTRL */
#define TG7CTRL_TRIGEVT_BIT_START                           20U
#define TG7CTRL_TRIGEVT_BIT_END                             23U

/* Definition for field NU in Register TG7CTRL */
#define TG7CTRL_NU_BIT_START                                24U
#define TG7CTRL_NU_BIT_END                                  27U

/* Definition for field TGTD in Register TG7CTRL */
#define TG7CTRL_TGTD_BIT_START                              28U
#define TG7CTRL_TGTD_BIT_END                                28U

/* Definition for field PRST in Register TG7CTRL */
#define TG7CTRL_PRST_BIT_START                              29U
#define TG7CTRL_PRST_BIT_END                                29U

/* Definition for field ONESHOT in Register TG7CTRL */
#define TG7CTRL_ONESHOT_BIT_START                           30U
#define TG7CTRL_ONESHOT_BIT_END                             30U

/* Definition for field TGENA in Register TG7CTRL */
#define TG7CTRL_TGENA_BIT_START                             31U
#define TG7CTRL_TGENA_BIT_END                               31U

/* Definition for field COUNT in Register DMA0CTRL */
#define DMA0CTRL_COUNT_BIT_START                            0U
#define DMA0CTRL_COUNT_BIT_END                              5U

/* Definition for field COUNTBIT17 in Register DMA0CTRL */
#define DMA0CTRL_COUNTBIT17_BIT_START                       6U
#define DMA0CTRL_COUNTBIT17_BIT_END                         6U

/* Definition for field BUFID7 in Register DMA0CTRL */
#define DMA0CTRL_BUFID7_BIT_START                           7U
#define DMA0CTRL_BUFID7_BIT_END                             7U

/* Definition for field ICOUNT in Register DMA0CTRL */
#define DMA0CTRL_ICOUNT_BIT_START                           8U
#define DMA0CTRL_ICOUNT_BIT_END                             12U

/* Definition for field NOBRK in Register DMA0CTRL */
#define DMA0CTRL_NOBRK_BIT_START                            13U
#define DMA0CTRL_NOBRK_BIT_END                              13U

/* Definition for field TXDMAENA in Register DMA0CTRL */
#define DMA0CTRL_TXDMAENA_BIT_START                         14U
#define DMA0CTRL_TXDMAENA_BIT_END                           14U

/* Definition for field RXDMAENA in Register DMA0CTRL */
#define DMA0CTRL_RXDMAENA_BIT_START                         15U
#define DMA0CTRL_RXDMAENA_BIT_END                           15U

/* Definition for field TXDMA_MAP in Register DMA0CTRL */
#define DMA0CTRL_TXDMA_MAP_BIT_START                        16U
#define DMA0CTRL_TXDMA_MAP_BIT_END                          19U

/* Definition for field RXDMA_MAP in Register DMA0CTRL */
#define DMA0CTRL_RXDMA_MAP_BIT_START                        20U
#define DMA0CTRL_RXDMA_MAP_BIT_END                          23U

/* Definition for field BUFID in Register DMA0CTRL */
#define DMA0CTRL_BUFID_BIT_START                            24U
#define DMA0CTRL_BUFID_BIT_END                              30U

/* Definition for field ONESHOT in Register DMA0CTRL */
#define DMA0CTRL_ONESHOT_BIT_START                          31U
#define DMA0CTRL_ONESHOT_BIT_END                            31U

/* Definition for field COUNT in Register DMA1CTRL */
#define DMA1CTRL_COUNT_BIT_START                            0U
#define DMA1CTRL_COUNT_BIT_END                              5U

/* Definition for field COUNTBIT17 in Register DMA1CTRL */
#define DMA1CTRL_COUNTBIT17_BIT_START                       6U
#define DMA1CTRL_COUNTBIT17_BIT_END                         6U

/* Definition for field BUFID7 in Register DMA1CTRL */
#define DMA1CTRL_BUFID7_BIT_START                           7U
#define DMA1CTRL_BUFID7_BIT_END                             7U

/* Definition for field ICOUNT in Register DMA1CTRL */
#define DMA1CTRL_ICOUNT_BIT_START                           8U
#define DMA1CTRL_ICOUNT_BIT_END                             12U

/* Definition for field NOBRK in Register DMA1CTRL */
#define DMA1CTRL_NOBRK_BIT_START                            13U
#define DMA1CTRL_NOBRK_BIT_END                              13U

/* Definition for field TXDMAENA in Register DMA1CTRL */
#define DMA1CTRL_TXDMAENA_BIT_START                         14U
#define DMA1CTRL_TXDMAENA_BIT_END                           14U

/* Definition for field RXDMAENA in Register DMA1CTRL */
#define DMA1CTRL_RXDMAENA_BIT_START                         15U
#define DMA1CTRL_RXDMAENA_BIT_END                           15U

/* Definition for field TXDMA_MAP in Register DMA1CTRL */
#define DMA1CTRL_TXDMA_MAP_BIT_START                        16U
#define DMA1CTRL_TXDMA_MAP_BIT_END                          19U

/* Definition for field RXDMA_MAP in Register DMA1CTRL */
#define DMA1CTRL_RXDMA_MAP_BIT_START                        20U
#define DMA1CTRL_RXDMA_MAP_BIT_END                          23U

/* Definition for field BUFID in Register DMA1CTRL */
#define DMA1CTRL_BUFID_BIT_START                            24U
#define DMA1CTRL_BUFID_BIT_END                              30U

/* Definition for field ONESHOT in Register DMA1CTRL */
#define DMA1CTRL_ONESHOT_BIT_START                          31U
#define DMA1CTRL_ONESHOT_BIT_END                            31U

/* Definition for field COUNT in Register DMA2CTRL */
#define DMA2CTRL_COUNT_BIT_START                            0U
#define DMA2CTRL_COUNT_BIT_END                              5U

/* Definition for field COUNTBIT17 in Register DMA2CTRL */
#define DMA2CTRL_COUNTBIT17_BIT_START                       6U
#define DMA2CTRL_COUNTBIT17_BIT_END                         6U

/* Definition for field BUFID7 in Register DMA2CTRL */
#define DMA2CTRL_BUFID7_BIT_START                           7U
#define DMA2CTRL_BUFID7_BIT_END                             7U

/* Definition for field ICOUNT in Register DMA2CTRL */
#define DMA2CTRL_ICOUNT_BIT_START                           8U
#define DMA2CTRL_ICOUNT_BIT_END                             12U

/* Definition for field NOBRK in Register DMA2CTRL */
#define DMA2CTRL_NOBRK_BIT_START                            13U
#define DMA2CTRL_NOBRK_BIT_END                              13U

/* Definition for field TXDMAENA in Register DMA2CTRL */
#define DMA2CTRL_TXDMAENA_BIT_START                         14U
#define DMA2CTRL_TXDMAENA_BIT_END                           14U

/* Definition for field RXDMAENA in Register DMA2CTRL */
#define DMA2CTRL_RXDMAENA_BIT_START                         15U
#define DMA2CTRL_RXDMAENA_BIT_END                           15U

/* Definition for field TXDMA_MAP in Register DMA2CTRL */
#define DMA2CTRL_TXDMA_MAP_BIT_START                        16U
#define DMA2CTRL_TXDMA_MAP_BIT_END                          19U

/* Definition for field RXDMA_MAP in Register DMA2CTRL */
#define DMA2CTRL_RXDMA_MAP_BIT_START                        20U
#define DMA2CTRL_RXDMA_MAP_BIT_END                          23U

/* Definition for field BUFID in Register DMA2CTRL */
#define DMA2CTRL_BUFID_BIT_START                            24U
#define DMA2CTRL_BUFID_BIT_END                              30U

/* Definition for field ONESHOT in Register DMA2CTRL */
#define DMA2CTRL_ONESHOT_BIT_START                          31U
#define DMA2CTRL_ONESHOT_BIT_END                            31U

/* Definition for field COUNT in Register DMA3CTRL */
#define DMA3CTRL_COUNT_BIT_START                            0U
#define DMA3CTRL_COUNT_BIT_END                              5U

/* Definition for field COUNTBIT17 in Register DMA3CTRL */
#define DMA3CTRL_COUNTBIT17_BIT_START                       6U
#define DMA3CTRL_COUNTBIT17_BIT_END                         6U

/* Definition for field BUFID7 in Register DMA3CTRL */
#define DMA3CTRL_BUFID7_BIT_START                           7U
#define DMA3CTRL_BUFID7_BIT_END                             7U

/* Definition for field ICOUNT in Register DMA3CTRL */
#define DMA3CTRL_ICOUNT_BIT_START                           8U
#define DMA3CTRL_ICOUNT_BIT_END                             12U

/* Definition for field NOBRK in Register DMA3CTRL */
#define DMA3CTRL_NOBRK_BIT_START                            13U
#define DMA3CTRL_NOBRK_BIT_END                              13U

/* Definition for field TXDMAENA in Register DMA3CTRL */
#define DMA3CTRL_TXDMAENA_BIT_START                         14U
#define DMA3CTRL_TXDMAENA_BIT_END                           14U

/* Definition for field RXDMAENA in Register DMA3CTRL */
#define DMA3CTRL_RXDMAENA_BIT_START                         15U
#define DMA3CTRL_RXDMAENA_BIT_END                           15U

/* Definition for field TXDMA_MAP in Register DMA3CTRL */
#define DMA3CTRL_TXDMA_MAP_BIT_START                        16U
#define DMA3CTRL_TXDMA_MAP_BIT_END                          19U

/* Definition for field RXDMA_MAP in Register DMA3CTRL */
#define DMA3CTRL_RXDMA_MAP_BIT_START                        20U
#define DMA3CTRL_RXDMA_MAP_BIT_END                          23U

/* Definition for field BUFID in Register DMA3CTRL */
#define DMA3CTRL_BUFID_BIT_START                            24U
#define DMA3CTRL_BUFID_BIT_END                              30U

/* Definition for field ONESHOT in Register DMA3CTRL */
#define DMA3CTRL_ONESHOT_BIT_START                          31U
#define DMA3CTRL_ONESHOT_BIT_END                            31U

/* Definition for field COUNT in Register DMA4CTRL */
#define DMA4CTRL_COUNT_BIT_START                            0U
#define DMA4CTRL_COUNT_BIT_END                              5U

/* Definition for field COUNTBIT17 in Register DMA4CTRL */
#define DMA4CTRL_COUNTBIT17_BIT_START                       6U
#define DMA4CTRL_COUNTBIT17_BIT_END                         6U

/* Definition for field BUFID7 in Register DMA4CTRL */
#define DMA4CTRL_BUFID7_BIT_START                           7U
#define DMA4CTRL_BUFID7_BIT_END                             7U

/* Definition for field ICOUNT in Register DMA4CTRL */
#define DMA4CTRL_ICOUNT_BIT_START                           8U
#define DMA4CTRL_ICOUNT_BIT_END                             12U

/* Definition for field NOBRK in Register DMA4CTRL */
#define DMA4CTRL_NOBRK_BIT_START                            13U
#define DMA4CTRL_NOBRK_BIT_END                              13U

/* Definition for field TXDMAENA in Register DMA4CTRL */
#define DMA4CTRL_TXDMAENA_BIT_START                         14U
#define DMA4CTRL_TXDMAENA_BIT_END                           14U

/* Definition for field RXDMAENA in Register DMA4CTRL */
#define DMA4CTRL_RXDMAENA_BIT_START                         15U
#define DMA4CTRL_RXDMAENA_BIT_END                           15U

/* Definition for field TXDMA_MAP in Register DMA4CTRL */
#define DMA4CTRL_TXDMA_MAP_BIT_START                        16U
#define DMA4CTRL_TXDMA_MAP_BIT_END                          19U

/* Definition for field RXDMA_MAP in Register DMA4CTRL */
#define DMA4CTRL_RXDMA_MAP_BIT_START                        20U
#define DMA4CTRL_RXDMA_MAP_BIT_END                          23U

/* Definition for field BUFID in Register DMA4CTRL */
#define DMA4CTRL_BUFID_BIT_START                            24U
#define DMA4CTRL_BUFID_BIT_END                              30U

/* Definition for field ONESHOT in Register DMA4CTRL */
#define DMA4CTRL_ONESHOT_BIT_START                          31U
#define DMA4CTRL_ONESHOT_BIT_END                            31U

/* Definition for field COUNT in Register ICOUNT0 */
#define ICOUNT0_COUNT_BIT_START                             0U
#define ICOUNT0_COUNT_BIT_END                               15U

/* Definition for field ICOUNT in Register ICOUNT0 */
#define ICOUNT0_ICOUNT_BIT_START                            16U
#define ICOUNT0_ICOUNT_BIT_END                              31U

/* Definition for field COUNT in Register ICOUNT1 */
#define ICOUNT1_COUNT_BIT_START                             0U
#define ICOUNT1_COUNT_BIT_END                               15U

/* Definition for field ICOUNT in Register ICOUNT1 */
#define ICOUNT1_ICOUNT_BIT_START                            16U
#define ICOUNT1_ICOUNT_BIT_END                              31U

/* Definition for field COUNT in Register ICOUNT2 */
#define ICOUNT2_COUNT_BIT_START                             0U
#define ICOUNT2_COUNT_BIT_END                               15U

/* Definition for field COUNT in Register ICOUNT3 */
#define ICOUNT3_COUNT_BIT_START                             0U
#define ICOUNT3_COUNT_BIT_END                               15U

/* Definition for field ICOUNT in Register ICOUNT3 */
#define ICOUNT3_ICOUNT_BIT_START                            16U
#define ICOUNT3_ICOUNT_BIT_END                              31U

/* Definition for field COUNT in Register ICOUNT4 */
#define ICOUNT4_COUNT_BIT_START                             0U
#define ICOUNT4_COUNT_BIT_END                               15U

/* Definition for field ICOUNT in Register ICOUNT4 */
#define ICOUNT4_ICOUNT_BIT_START                            16U
#define ICOUNT4_ICOUNT_BIT_END                              31U

/* Definition for field LARGE_COUNT in Register DMACNTLEN */
#define DMACNTLEN_LARGE_COUNT_BIT_START                     0U
#define DMACNTLEN_LARGE_COUNT_BIT_END                       0U

/* Definition for field NU in Register DMACNTLEN */
#define DMACNTLEN_NU_BIT_START                              1U
#define DMACNTLEN_NU_BIT_END                                31U

/* Definition for field EDEN in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_EDEN_BIT_START                         0U
#define PAR_ECC_CTRL_EDEN_BIT_END                           3U

/* Definition for field NU1 in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_NU1_BIT_START                          4U
#define PAR_ECC_CTRL_NU1_BIT_END                            7U

/* Definition for field PTESTEN in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_PTESTEN_BIT_START                      8U
#define PAR_ECC_CTRL_PTESTEN_BIT_END                        8U

/* Definition for field NU2 in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_NU2_BIT_START                          9U
#define PAR_ECC_CTRL_NU2_BIT_END                            15U

/* Definition for field EDAC_MODE in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_EDAC_MODE_BIT_START                    16U
#define PAR_ECC_CTRL_EDAC_MODE_BIT_END                      19U

/* Definition for field NU3 in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_NU3_BIT_START                          20U
#define PAR_ECC_CTRL_NU3_BIT_END                            23U

/* Definition for field SBE_EVT_EN in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_SBE_EVT_EN_BIT_START                   24U
#define PAR_ECC_CTRL_SBE_EVT_EN_BIT_END                     27U

/* Definition for field NU4 in Register PAR_ECC_CTRL */
#define PAR_ECC_CTRL_NU4_BIT_START                          28U
#define PAR_ECC_CTRL_NU4_BIT_END                            31U

/* Definition for field UERR_FLG0 in Register PAR_ECC_STAT */
#define PAR_ECC_STAT_UERR_FLG0_BIT_START                    0U
#define PAR_ECC_STAT_UERR_FLG0_BIT_END                      0U

/* Definition for field UERR_FLG1 in Register PAR_ECC_STAT */
#define PAR_ECC_STAT_UERR_FLG1_BIT_START                    1U
#define PAR_ECC_STAT_UERR_FLG1_BIT_END                      1U

/* Definition for field NU1 in Register PAR_ECC_STAT */
#define PAR_ECC_STAT_NU1_BIT_START                          2U
#define PAR_ECC_STAT_NU1_BIT_END                            7U

/* Definition for field SBE_FLG0 in Register PAR_ECC_STAT */
#define PAR_ECC_STAT_SBE_FLG0_BIT_START                     8U
#define PAR_ECC_STAT_SBE_FLG0_BIT_END                       8U

/* Definition for field SBE_FLG1 in Register PAR_ECC_STAT */
#define PAR_ECC_STAT_SBE_FLG1_BIT_START                     9U
#define PAR_ECC_STAT_SBE_FLG1_BIT_END                       9U

/* Definition for field NU2 in Register PAR_ECC_STAT */
#define PAR_ECC_STAT_NU2_BIT_START                          10U
#define PAR_ECC_STAT_NU2_BIT_END                            31U

/* Definition for field UERRADDR1 in Register UERRADDR1 */
#define UERRADDR1_UERRADDR1_BIT_START                       0U
#define UERRADDR1_UERRADDR1_BIT_END                         10U

/* Definition for field NU in Register UERRADDR1 */
#define UERRADDR1_NU_BIT_START                              11U
#define UERRADDR1_NU_BIT_END                                31U

/* Definition for field UERRADDR0 in Register UERRADDR0 */
#define UERRADDR0_UERRADDR0_BIT_START                       0U
#define UERRADDR0_UERRADDR0_BIT_END                         10U

/* Definition for field NU in Register UERRADDR0 */
#define UERRADDR0_NU_BIT_START                              11U
#define UERRADDR0_NU_BIT_END                                31U

/* Definition for field RXOVRN_BUF_ADDR in Register RXOVRN_BUF_ADDR */
#define RXOVRN_BUF_ADDR_RXOVRN_BUF_ADDR_BIT_START           0U
#define RXOVRN_BUF_ADDR_RXOVRN_BUF_ADDR_BIT_END             10U

/* Definition for field NU in Register RXOVRN_BUF_ADDR */
#define RXOVRN_BUF_ADDR_NU_BIT_START                        11U
#define RXOVRN_BUF_ADDR_NU_BIT_END                          31U

/* Definition for field RXPENA in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_RXPENA_BIT_START                        0U
#define IOLPBKTSTCR_RXPENA_BIT_END                          0U

/* Definition for field LPBKTYPE in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_LPBKTYPE_BIT_START                      1U
#define IOLPBKTSTCR_LPBKTYPE_BIT_END                        1U

/* Definition for field CTRLSCSPINERR in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_CTRLSCSPINERR_BIT_START                 2U
#define IOLPBKTSTCR_CTRLSCSPINERR_BIT_END                   2U

/* Definition for field ERRSCSPIN in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_ERRSCSPIN_BIT_START                     3U
#define IOLPBKTSTCR_ERRSCSPIN_BIT_END                       5U

/* Definition for field NU1 in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_NU1_BIT_START                           6U
#define IOLPBKTSTCR_NU1_BIT_END                             7U

/* Definition for field IOLPBKTSTENA in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_IOLPBKTSTENA_BIT_START                  8U
#define IOLPBKTSTCR_IOLPBKTSTENA_BIT_END                    11U

/* Definition for field NU2 in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_NU2_BIT_START                           12U
#define IOLPBKTSTCR_NU2_BIT_END                             15U

/* Definition for field CTRLDLENERR in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_CTRLDLENERR_BIT_START                   16U
#define IOLPBKTSTCR_CTRLDLENERR_BIT_END                     16U

/* Definition for field CTRLTIMEOUT in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_CTRLTIMEOUT_BIT_START                   17U
#define IOLPBKTSTCR_CTRLTIMEOUT_BIT_END                     17U

/* Definition for field CTRLPARERR in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_CTRLPARERR_BIT_START                    18U
#define IOLPBKTSTCR_CTRLPARERR_BIT_END                      18U

/* Definition for field CTRLDESYNC in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_CTRLDESYNC_BIT_START                    19U
#define IOLPBKTSTCR_CTRLDESYNC_BIT_END                      19U

/* Definition for field CTRLBITERR in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_CTRLBITERR_BIT_START                    20U
#define IOLPBKTSTCR_CTRLBITERR_BIT_END                      20U

/* Definition for field NU3 in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_NU3_BIT_START                           21U
#define IOLPBKTSTCR_NU3_BIT_END                             23U

/* Definition for field SCSFAILFLG in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_SCSFAILFLG_BIT_START                    24U
#define IOLPBKTSTCR_SCSFAILFLG_BIT_END                      24U

/* Definition for field NU4 in Register IOLPBKTSTCR */
#define IOLPBKTSTCR_NU4_BIT_START                           25U
#define IOLPBKTSTCR_NU4_BIT_END                             31U

/* Definition for field EPRESCLAE_FMT0 in Register EXTENDED_PRESCALE1 */
#define EXTENDED_PRESCALE1_EPRESCLAE_FMT0_BIT_START         0U
#define EXTENDED_PRESCALE1_EPRESCLAE_FMT0_BIT_END           10U

/* Definition for field NU1 in Register EXTENDED_PRESCALE1 */
#define EXTENDED_PRESCALE1_NU1_BIT_START                    11U
#define EXTENDED_PRESCALE1_NU1_BIT_END                      15U

/* Definition for field EPRESCLAE_FMT1 in Register EXTENDED_PRESCALE1 */
#define EXTENDED_PRESCALE1_EPRESCLAE_FMT1_BIT_START         16U
#define EXTENDED_PRESCALE1_EPRESCLAE_FMT1_BIT_END           26U

/* Definition for field NU2 in Register EXTENDED_PRESCALE1 */
#define EXTENDED_PRESCALE1_NU2_BIT_START                    27U
#define EXTENDED_PRESCALE1_NU2_BIT_END                      31U

/* Definition for field EPRESCLAE_FMT2 in Register EXTENDED_PRESCALE2 */
#define EXTENDED_PRESCALE2_EPRESCLAE_FMT2_BIT_START         0U
#define EXTENDED_PRESCALE2_EPRESCLAE_FMT2_BIT_END           10U

/* Definition for field NU3 in Register EXTENDED_PRESCALE2 */
#define EXTENDED_PRESCALE2_NU3_BIT_START                    11U
#define EXTENDED_PRESCALE2_NU3_BIT_END                      15U

/* Definition for field EPRESCLAE_FMT3 in Register EXTENDED_PRESCALE2 */
#define EXTENDED_PRESCALE2_EPRESCLAE_FMT3_BIT_START         16U
#define EXTENDED_PRESCALE2_EPRESCLAE_FMT3_BIT_END           26U

/* Definition for field NU4 in Register EXTENDED_PRESCALE2 */
#define EXTENDED_PRESCALE2_NU4_BIT_START                    27U
#define EXTENDED_PRESCALE2_NU4_BIT_END                      31U

/* Definition for field ECCDIAG_EN in Register ECCDIAG_CTRL */
#define ECCDIAG_CTRL_ECCDIAG_EN_BIT_START                   0U
#define ECCDIAG_CTRL_ECCDIAG_EN_BIT_END                     3U

/* Definition for field NU in Register ECCDIAG_CTRL */
#define ECCDIAG_CTRL_NU_BIT_START                           4U
#define ECCDIAG_CTRL_NU_BIT_END                             31U

/* Definition for field SEFLG0 in Register ECCDIAG_STAT */
#define ECCDIAG_STAT_SEFLG0_BIT_START                       0U
#define ECCDIAG_STAT_SEFLG0_BIT_END                         0U

/* Definition for field SEFLG1 in Register ECCDIAG_STAT */
#define ECCDIAG_STAT_SEFLG1_BIT_START                       1U
#define ECCDIAG_STAT_SEFLG1_BIT_END                         1U

/* Definition for field NU1 in Register ECCDIAG_STAT */
#define ECCDIAG_STAT_NU1_BIT_START                          2U
#define ECCDIAG_STAT_NU1_BIT_END                            15U

/* Definition for field DEFLG0 in Register ECCDIAG_STAT */
#define ECCDIAG_STAT_DEFLG0_BIT_START                       16U
#define ECCDIAG_STAT_DEFLG0_BIT_END                         16U

/* Definition for field DEFLG1 in Register ECCDIAG_STAT */
#define ECCDIAG_STAT_DEFLG1_BIT_START                       17U
#define ECCDIAG_STAT_DEFLG1_BIT_END                         17U

/* Definition for field NU2 in Register ECCDIAG_STAT */
#define ECCDIAG_STAT_NU2_BIT_START                          18U
#define ECCDIAG_STAT_NU2_BIT_END                            31U

/* Definition for field SBERRADDR1 in Register SBERRADDR1 */
#define SBERRADDR1_SBERRADDR1_BIT_START                     0U
#define SBERRADDR1_SBERRADDR1_BIT_END                       10U

/* Definition for field NU1 in Register SBERRADDR1 */
#define SBERRADDR1_NU1_BIT_START                            11U
#define SBERRADDR1_NU1_BIT_END                              31U

/* Definition for field SBERRADDR0 in Register SBERRADDR0 */
#define SBERRADDR0_SBERRADDR0_BIT_START                     0U
#define SBERRADDR0_SBERRADDR0_BIT_END                       10U

/* Definition for field NU2 in Register SBERRADDR0 */
#define SBERRADDR0_NU2_BIT_START                            11U
#define SBERRADDR0_NU2_BIT_END                              31U

/* Definition for field MINOR in Register SPIREV */
#define SPIREV_MINOR_BIT_START                              0U
#define SPIREV_MINOR_BIT_END                                5U

/* Definition for field CUSTOM in Register SPIREV */
#define SPIREV_CUSTOM_BIT_START                             6U
#define SPIREV_CUSTOM_BIT_END                               7U

/* Definition for field MAJOR in Register SPIREV */
#define SPIREV_MAJOR_BIT_START                              8U
#define SPIREV_MAJOR_BIT_END                                10U

/* Definition for field RTL in Register SPIREV */
#define SPIREV_RTL_BIT_START                                11U
#define SPIREV_RTL_BIT_END                                  15U

/* Definition for field FUNC in Register SPIREV */
#define SPIREV_FUNC_BIT_START                               16U
#define SPIREV_FUNC_BIT_END                                 27U

/* Definition for field NU in Register SPIREV */
#define SPIREV_NU_BIT_START                                 28U
#define SPIREV_NU_BIT_END                                   29U

/* Definition for field SCHEME in Register SPIREV */
#define SPIREV_SCHEME_BIT_START                             30U
#define SPIREV_SCHEME_BIT_END                               31U



/**
 * @struct MIBSPIRegs_t
 * @brief
 *   Module MSS_MIBSPIA Register Definition
 * @details
 *   This structure is used to access the MSS_MIBSPIA module registers.
 */
/**
 * @typedef MIBSPIRegs
 * @brief
 *   Module MSS_MIBSPIA Register Frame type Definition
 * @details
 *   This type is used to access the MSS_MIBSPIA module registers.
 */
typedef volatile struct MIBSPIRegs_t
{
    uint32_t    SPIGCR0                            ;        /* Offset = 0x000 */
    uint32_t    SPIGCR1                            ;        /* Offset = 0x004 */
    uint32_t    SPIINT0                            ;        /* Offset = 0x008 */
    uint32_t    SPILVL                             ;        /* Offset = 0x00C */
    uint32_t    SPIFLG                             ;        /* Offset = 0x010 */
    uint32_t    SPIPC0                             ;        /* Offset = 0x014 */
    uint32_t    SPIPC1                             ;        /* Offset = 0x018 */
    uint32_t    SPIPC2                             ;        /* Offset = 0x01C */
    uint32_t    SPIPC3                             ;        /* Offset = 0x020 */
    uint32_t    SPIPC4                             ;        /* Offset = 0x024 */
    uint32_t    SPIPC5                             ;        /* Offset = 0x028 */
    uint32_t    SPIPC6                             ;        /* Offset = 0x02C */
    uint32_t    SPIPC7                             ;        /* Offset = 0x030 */
    uint32_t    SPIPC8                             ;        /* Offset = 0x034 */
    uint32_t    SPIDAT0                            ;        /* Offset = 0x038 */
    uint32_t    SPIDAT1                            ;        /* Offset = 0x03C */
    uint32_t    SPIBUF                             ;        /* Offset = 0x040 */
    uint32_t    SPIEMU                             ;        /* Offset = 0x044 */
    uint32_t    SPIDELAY                           ;        /* Offset = 0x048 */
    uint32_t    SPIDEF                             ;        /* Offset = 0x04C */
    uint32_t    SPIFMT0                            ;        /* Offset = 0x050 */
    uint32_t    SPIFMT1                            ;        /* Offset = 0x054 */
    uint32_t    SPIFMT2                            ;        /* Offset = 0x058 */
    uint32_t    SPIFMT3                            ;        /* Offset = 0x05C */
    uint32_t    TGINTVECT0                         ;        /* Offset = 0x060 */
    uint32_t    TGINTVECT1                         ;        /* Offset = 0x064 */
    uint32_t    SPIPC9                             ;        /* Offset = 0x068 */
    uint32_t    SPIPMCTRL                          ;        /* Offset = 0x06C */
    uint32_t    MIBSPIE                            ;        /* Offset = 0x070 */
    uint32_t    TGITENST                           ;        /* Offset = 0x074 */
    uint32_t    TGITENCR                           ;        /* Offset = 0x078 */
    uint32_t    TGITLVST                           ;        /* Offset = 0x07C */
    uint32_t    TGITLVCR                           ;        /* Offset = 0x080 */
    uint32_t    TGINTFLAG                          ;        /* Offset = 0x084 */
    uint32_t    RESTRICTED2[2]                     ;        /* Offset = 0x088 */
    uint32_t    TICKCNT                            ;        /* Offset = 0x090 */
    uint32_t    LTGPEND                            ;        /* Offset = 0x094 */
#if 0    
    uint32_t    TG0CTRL                            ;        /* Offset = 0x098 */
    uint32_t    TG1CTRL                            ;        /* Offset = 0x09C */
    uint32_t    TG2CTRL                            ;        /* Offset = 0x0A0 */
    uint32_t    TG3CTRL                            ;        /* Offset = 0x0A4 */
    uint32_t    TG4CTRL                            ;        /* Offset = 0x0A8 */
    uint32_t    TG5CTRL                            ;        /* Offset = 0x0AC */
    uint32_t    TG6CTRL                            ;        /* Offset = 0x0B0 */
    uint32_t    TG7CTRL                            ;        /* Offset = 0x0B4 */
#else
    uint32_t    TGCTRL[8]                          ;        /* Offset = 0x098 */
#endif    

    uint32_t    RESTRICTED3[8]                     ;        /* Offset = 0x0B8 */
#if 0
    uint32_t    DMA0CTRL                           ;        /* Offset = 0x0D8 */
    uint32_t    DMA1CTRL                           ;        /* Offset = 0x0DC */
    uint32_t    DMA2CTRL                           ;        /* Offset = 0x0E0 */
    uint32_t    DMA3CTRL                           ;        /* Offset = 0x0E4 */
    uint32_t    DMA4CTRL                           ;        /* Offset = 0x0E8 */
#else 
    uint32_t    DMACTRL[5]                         ;        /* Offset = 0x0D8 */
#endif
    uint32_t    RESTRICTED4[3]                     ;        /* Offset = 0x0EC */
#if 0
    uint32_t    ICOUNT0                            ;        /* Offset = 0x0F8 */
    uint32_t    ICOUNT1                            ;        /* Offset = 0x0FC */
    uint32_t    ICOUNT2                            ;        /* Offset = 0x100 */
    uint32_t    ICOUNT3                            ;        /* Offset = 0x104 */
    uint32_t    ICOUNT4                            ;        /* Offset = 0x108 */
#else  
    uint32_t    ICOUNT[5]                          ;        /* Offset = 0x0F8 */
#endif
    uint32_t    RESTRICTED5[3]                     ;        /* Offset = 0x10C */
    uint32_t    DMACNTLEN                          ;        /* Offset = 0x118 */
    uint32_t    RESTRICTED6                        ;        /* Offset = 0x11C */
    uint32_t    PAR_ECC_CTRL                       ;        /* Offset = 0x120 */
    uint32_t    PAR_ECC_STAT                       ;        /* Offset = 0x124 */
    uint32_t    UERRADDR1                          ;        /* Offset = 0x128 */
    uint32_t    UERRADDR0                          ;        /* Offset = 0x12C */
    uint32_t    RXOVRN_BUF_ADDR                    ;        /* Offset = 0x130 */
    uint32_t    IOLPBKTSTCR                        ;        /* Offset = 0x134 */
    uint32_t    EXTENDED_PRESCALE1                 ;        /* Offset = 0x138 */
    uint32_t    EXTENDED_PRESCALE2                 ;        /* Offset = 0x13C */
    uint32_t    ECCDIAG_CTRL                       ;        /* Offset = 0x140 */
    uint32_t    ECCDIAG_STAT                       ;        /* Offset = 0x144 */
    uint32_t    SBERRADDR1                         ;        /* Offset = 0x148 */
    uint32_t    SBERRADDR0                         ;        /* Offset = 0x14C */
    uint32_t    RESTRICTED7[43]                    ;        /* Offset = 0x150 */
    uint32_t    SPIREV                             ;        /* Offset = 0x1FC */
} MIBSPIRegs;


/**
 * @struct MIBSPIRam_t
 * @brief
 *   MIBSPI Buffer RAM Definition
 * @details
 *   This structure is used to access the MSS_MIBSPIA buffer memory.
 */
/**
 * @typedef MIBSPIRam
 * @brief
 *   MIBSPI Buffer RAM Definition
 * @details
 *   This structure is used to access the MSS_MIBSPIA buffer memory.
 */
typedef volatile struct MIBSPIRam_t
{
    struct
    {
     uint16_t  data;     /**< tx buffer data    */
     uint16_t  control;  /**< tx buffer control */
    } tx[128];
    struct
    {
     uint16_t data;     /**< rx buffer data  */
     uint16_t flags;    /**< rx buffer flags */
    } rx[128];
} MIBSPIRam;


#ifdef __cplusplus
}
#endif

#endif /* REG_MIBSPI_H */
/* END OF REG_MIBSPI_H */

