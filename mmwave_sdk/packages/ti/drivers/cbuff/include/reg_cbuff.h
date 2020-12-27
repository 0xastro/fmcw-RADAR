/**
 *  @file    reg_cbuff.h
 *
 *  @brief
 *    This file gives register definitions of DSS_CBUFF module.
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

#ifndef REG_DSSCBUFF_H
#define REG_DSSCBUFF_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field CFG_1LVDS_0CSI in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CFG_1LVDS_0CSI_BIT_START               0U
#define CONFIG_REG_0_CFG_1LVDS_0CSI_BIT_END                 0U

/* Definition for field CFG_ECC_EN in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CFG_ECC_EN_BIT_START                   1U
#define CONFIG_REG_0_CFG_ECC_EN_BIT_END                     1U

/* Definition for field cftrigen in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CFTRIGEN_BIT_START                     2U
#define CONFIG_REG_0_CFTRIGEN_BIT_END                       2U

/* Definition for field CFG_SW_TRIG_EN in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CFG_SW_TRIG_EN_BIT_START               3U
#define CONFIG_REG_0_CFG_SW_TRIG_EN_BIT_END                 3U

/* Definition for field NU1 in Register CONFIG_REG_0 */
#define CONFIG_REG_0_NU1_BIT_START                          4U
#define CONFIG_REG_0_NU1_BIT_END                            7U

/* Definition for field cvc0en in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CVC0EN_BIT_START                       10U
#define CONFIG_REG_0_CVC0EN_BIT_END                         11U

/* Definition for field cvc1en in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CVC1EN_BIT_START                       12U
#define CONFIG_REG_0_CVC1EN_BIT_END                         13U

/* Definition for field cvc2en in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CVC2EN_BIT_START                       14U
#define CONFIG_REG_0_CVC2EN_BIT_END                         15U

/* Definition for field cvc3en in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CVC3EN_BIT_START                       16U
#define CONFIG_REG_0_CVC3EN_BIT_END                         17U

/* Definition for field CFG_CHIRP_AVAIL_TRIG in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CFG_CHIRP_AVAIL_TRIG_BIT_START         24U
#define CONFIG_REG_0_CFG_CHIRP_AVAIL_TRIG_BIT_END           24U

/* Definition for field CFG_FRAME_START_TRIG in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CFG_FRAME_START_TRIG_BIT_START         25U
#define CONFIG_REG_0_CFG_FRAME_START_TRIG_BIT_END           25U

/* Definition for field cswcrst in Register CONFIG_REG_0 */
#define CONFIG_REG_0_CSWCRST_BIT_START                      27U
#define CONFIG_REG_0_CSWCRST_BIT_END                        27U

/* Definition for field CFG_SPHDR_ADDRESS in Register CFG_SPHDR_ADDRESS */
#define CFG_SPHDR_ADDRESS_CFG_SPHDR_ADDRESS_BIT_START       0U
#define CFG_SPHDR_ADDRESS_CFG_SPHDR_ADDRESS_BIT_END         31U

/* Definition for field CFG_CMD_HSVAL in Register CFG_CMD_HSVAL */
#define CFG_CMD_HSVAL_CFG_CMD_HSVAL_BIT_START               0U
#define CFG_CMD_HSVAL_CFG_CMD_HSVAL_BIT_END                 31U

/* Definition for field CFG_CMD_HEVAL in Register CFG_CMD_HEVAL */
#define CFG_CMD_HEVAL_CFG_CMD_HEVAL_BIT_START               0U
#define CFG_CMD_HEVAL_CFG_CMD_HEVAL_BIT_END                 31U

/* Definition for field CFG_CMD_VSVAL in Register CFG_CMD_VSVAL */
#define CFG_CMD_VSVAL_CFG_CMD_VSVAL_BIT_START               0U
#define CFG_CMD_VSVAL_CFG_CMD_VSVAL_BIT_END                 31U

/* Definition for field CFG_CMD_VEVAL in Register CFG_CMD_VEVAL */
#define CFG_CMD_VEVAL_CFG_CMD_VEVAL_BIT_START               0U
#define CFG_CMD_VEVAL_CFG_CMD_VEVAL_BIT_END                 31U

/* Definition for field CFG_LPHDR_ADDRESS in Register CFG_LPHDR_ADDRESS */
#define CFG_LPHDR_ADDRESS_CFG_LPHDR_ADDRESS_BIT_START       0U
#define CFG_LPHDR_ADDRESS_CFG_LPHDR_ADDRESS_BIT_END         31U

/* Definition for field CFG_CHIRPS_PER_FRAME in Register CFG_CHIRPS_PER_FRAME */
#define CFG_CHIRPS_PER_FRAME_CFG_CHIRPS_PER_FRAME_BIT_START 0U
#define CFG_CHIRPS_PER_FRAME_CFG_CHIRPS_PER_FRAME_BIT_END   31U

/* Definition for field CFG_FIFO_FREE_THRESHOLD0 in Register CFG_FIFO_FREE_THRESHOLD */
#define CFG_FIFO_FREE_THRESHOLD_CFG_FIFO_FREE_THRESHOLD0_BIT_START 0U
#define CFG_FIFO_FREE_THRESHOLD_CFG_FIFO_FREE_THRESHOLD0_BIT_END 7U

/* Definition for field CFG_LPPYLD_ADDRESS in Register CFG_LPPYLD_ADDRESS */
#define CFG_LPPYLD_ADDRESS_CFG_LPPYLD_ADDRESS_BIT_START     0U
#define CFG_LPPYLD_ADDRESS_CFG_LPPYLD_ADDRESS_BIT_END       31U

/* Definition for field NU in Register CFG_DELAY_CONFIG */
#define CFG_DELAY_CONFIG_NU_BIT_START                       24U
#define CFG_DELAY_CONFIG_NU_BIT_END                         31U

/* Definition for field LL0_VALID in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_VALID_BIT_START                    0U
#define CFG_DATA_LL0_LL0_VALID_BIT_END                      0U

/* Definition for field LL0_HE in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_HE_BIT_START                       1U
#define CFG_DATA_LL0_LL0_HE_BIT_END                         1U

/* Definition for field LL0_HS in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_HS_BIT_START                       2U
#define CFG_DATA_LL0_LL0_HS_BIT_END                         2U

/* Definition for field LL0_VCNUM in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_VCNUM_BIT_START                    3U
#define CFG_DATA_LL0_LL0_VCNUM_BIT_END                      4U

/* Definition for field LL0_FMT in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_FMT_BIT_START                      5U
#define CFG_DATA_LL0_LL0_FMT_BIT_END                        6U

/* Definition for field LL0_FMT_MAP in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL0_LL0_FMT_MAP_BIT_END                    7U

/* Definition for field LL0_FMT_IN in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL0_LL0_FMT_IN_BIT_END                     8U

/* Definition for field LL0_SIZE in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_SIZE_BIT_START                     9U
#define CFG_DATA_LL0_LL0_SIZE_BIT_END                       22U

/* Definition for field LL0_LPHDR_EN in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL0_LL0_LPHDR_EN_BIT_END                   27U

/* Definition for field LL0_CRC_EN in Register CFG_DATA_LL0 */
#define CFG_DATA_LL0_LL0_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL0_LL0_CRC_EN_BIT_END                     28U

/* Definition for field LL0_LPHDR_VAL in Register CFG_DATA_LL0_LPHDR_VAL */
#define CFG_DATA_LL0_LPHDR_VAL_LL0_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL0_LPHDR_VAL_LL0_LPHDR_VAL_BIT_END        31U

/* Definition for field LL0_RD_THRESHOLD in Register CFG_DATA_LL0_THRESHOLD */
#define CFG_DATA_LL0_THRESHOLD_LL0_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL0_THRESHOLD_LL0_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL0_THRESHOLD */
#define CFG_DATA_LL0_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL0_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL0_WR_THRESHOLD in Register CFG_DATA_LL0_THRESHOLD */
#define CFG_DATA_LL0_THRESHOLD_LL0_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL0_THRESHOLD_LL0_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL0_THRESHOLD */
#define CFG_DATA_LL0_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL0_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll0dman in Register CFG_DATA_LL0_THRESHOLD */
#define CFG_DATA_LL0_THRESHOLD_LL0DMAN_BIT_START            16U
#define CFG_DATA_LL0_THRESHOLD_LL0DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL0_THRESHOLD */
#define CFG_DATA_LL0_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL0_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL1_VALID in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_VALID_BIT_START                    0U
#define CFG_DATA_LL1_LL1_VALID_BIT_END                      0U

/* Definition for field LL1_HE in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_HE_BIT_START                       1U
#define CFG_DATA_LL1_LL1_HE_BIT_END                         1U

/* Definition for field LL1_HS in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_HS_BIT_START                       2U
#define CFG_DATA_LL1_LL1_HS_BIT_END                         2U

/* Definition for field LL1_VCNUM in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_VCNUM_BIT_START                    3U
#define CFG_DATA_LL1_LL1_VCNUM_BIT_END                      4U

/* Definition for field LL1_FMT in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_FMT_BIT_START                      5U
#define CFG_DATA_LL1_LL1_FMT_BIT_END                        6U

/* Definition for field LL1_FMT_MAP in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL1_LL1_FMT_MAP_BIT_END                    7U

/* Definition for field LL1_FMT_IN in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL1_LL1_FMT_IN_BIT_END                     8U

/* Definition for field LL1_SIZE in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_SIZE_BIT_START                     9U
#define CFG_DATA_LL1_LL1_SIZE_BIT_END                       22U

/* Definition for field LL1_LPHDR_EN in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL1_LL1_LPHDR_EN_BIT_END                   27U

/* Definition for field LL1_CRC_EN in Register CFG_DATA_LL1 */
#define CFG_DATA_LL1_LL1_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL1_LL1_CRC_EN_BIT_END                     28U

/* Definition for field LL1_LPHDR_VAL in Register CFG_DATA_LL1_LPHDR_VAL */
#define CFG_DATA_LL1_LPHDR_VAL_LL1_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL1_LPHDR_VAL_LL1_LPHDR_VAL_BIT_END        31U

/* Definition for field LL1_RD_THRESHOLD in Register CFG_DATA_LL1_THRESHOLD */
#define CFG_DATA_LL1_THRESHOLD_LL1_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL1_THRESHOLD_LL1_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL1_THRESHOLD */
#define CFG_DATA_LL1_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL1_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL1_WR_THRESHOLD in Register CFG_DATA_LL1_THRESHOLD */
#define CFG_DATA_LL1_THRESHOLD_LL1_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL1_THRESHOLD_LL1_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL1_THRESHOLD */
#define CFG_DATA_LL1_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL1_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll1dman in Register CFG_DATA_LL1_THRESHOLD */
#define CFG_DATA_LL1_THRESHOLD_LL1DMAN_BIT_START            16U
#define CFG_DATA_LL1_THRESHOLD_LL1DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL1_THRESHOLD */
#define CFG_DATA_LL1_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL1_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL2_VALID in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_VALID_BIT_START                    0U
#define CFG_DATA_LL2_LL2_VALID_BIT_END                      0U

/* Definition for field LL2_HE in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_HE_BIT_START                       1U
#define CFG_DATA_LL2_LL2_HE_BIT_END                         1U

/* Definition for field LL2_HS in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_HS_BIT_START                       2U
#define CFG_DATA_LL2_LL2_HS_BIT_END                         2U

/* Definition for field LL2_VCNUM in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_VCNUM_BIT_START                    3U
#define CFG_DATA_LL2_LL2_VCNUM_BIT_END                      4U

/* Definition for field LL2_FMT in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_FMT_BIT_START                      5U
#define CFG_DATA_LL2_LL2_FMT_BIT_END                        6U

/* Definition for field LL2_FMT_MAP in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL2_LL2_FMT_MAP_BIT_END                    7U

/* Definition for field LL2_FMT_IN in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL2_LL2_FMT_IN_BIT_END                     8U

/* Definition for field LL2_SIZE in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_SIZE_BIT_START                     9U
#define CFG_DATA_LL2_LL2_SIZE_BIT_END                       22U

/* Definition for field LL2_LPHDR_EN in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL2_LL2_LPHDR_EN_BIT_END                   27U

/* Definition for field LL2_CRC_EN in Register CFG_DATA_LL2 */
#define CFG_DATA_LL2_LL2_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL2_LL2_CRC_EN_BIT_END                     28U

/* Definition for field LL2_LPHDR_VAL in Register CFG_DATA_LL2_LPHDR_VAL */
#define CFG_DATA_LL2_LPHDR_VAL_LL2_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL2_LPHDR_VAL_LL2_LPHDR_VAL_BIT_END        31U

/* Definition for field LL2_RD_THRESHOLD in Register CFG_DATA_LL2_THRESHOLD */
#define CFG_DATA_LL2_THRESHOLD_LL2_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL2_THRESHOLD_LL2_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL2_THRESHOLD */
#define CFG_DATA_LL2_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL2_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL2_WR_THRESHOLD in Register CFG_DATA_LL2_THRESHOLD */
#define CFG_DATA_LL2_THRESHOLD_LL2_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL2_THRESHOLD_LL2_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL2_THRESHOLD */
#define CFG_DATA_LL2_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL2_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll2dman in Register CFG_DATA_LL2_THRESHOLD */
#define CFG_DATA_LL2_THRESHOLD_LL2DMAN_BIT_START            16U
#define CFG_DATA_LL2_THRESHOLD_LL2DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL2_THRESHOLD */
#define CFG_DATA_LL2_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL2_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL3_VALID in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_VALID_BIT_START                    0U
#define CFG_DATA_LL3_LL3_VALID_BIT_END                      0U

/* Definition for field LL3_HE in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_HE_BIT_START                       1U
#define CFG_DATA_LL3_LL3_HE_BIT_END                         1U

/* Definition for field LL3_HS in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_HS_BIT_START                       2U
#define CFG_DATA_LL3_LL3_HS_BIT_END                         2U

/* Definition for field LL3_VCNUM in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_VCNUM_BIT_START                    3U
#define CFG_DATA_LL3_LL3_VCNUM_BIT_END                      4U

/* Definition for field LL3_FMT in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_FMT_BIT_START                      5U
#define CFG_DATA_LL3_LL3_FMT_BIT_END                        6U

/* Definition for field LL3_FMT_MAP in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL3_LL3_FMT_MAP_BIT_END                    7U

/* Definition for field LL3_FMT_IN in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL3_LL3_FMT_IN_BIT_END                     8U

/* Definition for field LL3_SIZE in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_SIZE_BIT_START                     9U
#define CFG_DATA_LL3_LL3_SIZE_BIT_END                       22U

/* Definition for field LL3_LPHDR_EN in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL3_LL3_LPHDR_EN_BIT_END                   27U

/* Definition for field LL3_CRC_EN in Register CFG_DATA_LL3 */
#define CFG_DATA_LL3_LL3_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL3_LL3_CRC_EN_BIT_END                     28U

/* Definition for field LL3_LPHDR_VAL in Register CFG_DATA_LL3_LPHDR_VAL */
#define CFG_DATA_LL3_LPHDR_VAL_LL3_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL3_LPHDR_VAL_LL3_LPHDR_VAL_BIT_END        31U

/* Definition for field LL3_RD_THRESHOLD in Register CFG_DATA_LL3_THRESHOLD */
#define CFG_DATA_LL3_THRESHOLD_LL3_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL3_THRESHOLD_LL3_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL3_THRESHOLD */
#define CFG_DATA_LL3_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL3_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL3_WR_THRESHOLD in Register CFG_DATA_LL3_THRESHOLD */
#define CFG_DATA_LL3_THRESHOLD_LL3_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL3_THRESHOLD_LL3_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL3_THRESHOLD */
#define CFG_DATA_LL3_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL3_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll3dman in Register CFG_DATA_LL3_THRESHOLD */
#define CFG_DATA_LL3_THRESHOLD_LL3DMAN_BIT_START            16U
#define CFG_DATA_LL3_THRESHOLD_LL3DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL3_THRESHOLD */
#define CFG_DATA_LL3_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL3_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL4_VALID in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_VALID_BIT_START                    0U
#define CFG_DATA_LL4_LL4_VALID_BIT_END                      0U

/* Definition for field LL4_HE in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_HE_BIT_START                       1U
#define CFG_DATA_LL4_LL4_HE_BIT_END                         1U

/* Definition for field LL4_HS in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_HS_BIT_START                       2U
#define CFG_DATA_LL4_LL4_HS_BIT_END                         2U

/* Definition for field LL4_VCNUM in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_VCNUM_BIT_START                    3U
#define CFG_DATA_LL4_LL4_VCNUM_BIT_END                      4U

/* Definition for field LL4_FMT in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_FMT_BIT_START                      5U
#define CFG_DATA_LL4_LL4_FMT_BIT_END                        6U

/* Definition for field LL4_FMT_MAP in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL4_LL4_FMT_MAP_BIT_END                    7U

/* Definition for field LL4_FMT_IN in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL4_LL4_FMT_IN_BIT_END                     8U

/* Definition for field LL4_SIZE in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_SIZE_BIT_START                     9U
#define CFG_DATA_LL4_LL4_SIZE_BIT_END                       22U

/* Definition for field LL4_LPHDR_EN in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL4_LL4_LPHDR_EN_BIT_END                   27U

/* Definition for field LL4_CRC_EN in Register CFG_DATA_LL4 */
#define CFG_DATA_LL4_LL4_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL4_LL4_CRC_EN_BIT_END                     28U

/* Definition for field LL4_LPHDR_VAL in Register CFG_DATA_LL4_LPHDR_VAL */
#define CFG_DATA_LL4_LPHDR_VAL_LL4_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL4_LPHDR_VAL_LL4_LPHDR_VAL_BIT_END        31U

/* Definition for field LL4_RD_THRESHOLD in Register CFG_DATA_LL4_THRESHOLD */
#define CFG_DATA_LL4_THRESHOLD_LL4_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL4_THRESHOLD_LL4_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL4_THRESHOLD */
#define CFG_DATA_LL4_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL4_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL4_WR_THRESHOLD in Register CFG_DATA_LL4_THRESHOLD */
#define CFG_DATA_LL4_THRESHOLD_LL4_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL4_THRESHOLD_LL4_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL4_THRESHOLD */
#define CFG_DATA_LL4_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL4_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll4dman in Register CFG_DATA_LL4_THRESHOLD */
#define CFG_DATA_LL4_THRESHOLD_LL4DMAN_BIT_START            16U
#define CFG_DATA_LL4_THRESHOLD_LL4DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL4_THRESHOLD */
#define CFG_DATA_LL4_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL4_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL5_VALID in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_VALID_BIT_START                    0U
#define CFG_DATA_LL5_LL5_VALID_BIT_END                      0U

/* Definition for field LL5_HE in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_HE_BIT_START                       1U
#define CFG_DATA_LL5_LL5_HE_BIT_END                         1U

/* Definition for field LL5_HS in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_HS_BIT_START                       2U
#define CFG_DATA_LL5_LL5_HS_BIT_END                         2U

/* Definition for field LL5_VCNUM in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_VCNUM_BIT_START                    3U
#define CFG_DATA_LL5_LL5_VCNUM_BIT_END                      4U

/* Definition for field LL5_FMT in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_FMT_BIT_START                      5U
#define CFG_DATA_LL5_LL5_FMT_BIT_END                        6U

/* Definition for field LL5_FMT_MAP in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL5_LL5_FMT_MAP_BIT_END                    7U

/* Definition for field LL5_FMT_IN in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL5_LL5_FMT_IN_BIT_END                     8U

/* Definition for field LL5_SIZE in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_SIZE_BIT_START                     9U
#define CFG_DATA_LL5_LL5_SIZE_BIT_END                       22U

/* Definition for field LL5_LPHDR_EN in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL5_LL5_LPHDR_EN_BIT_END                   27U

/* Definition for field LL5_CRC_EN in Register CFG_DATA_LL5 */
#define CFG_DATA_LL5_LL5_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL5_LL5_CRC_EN_BIT_END                     28U

/* Definition for field LL5_LPHDR_VAL in Register CFG_DATA_LL5_LPHDR_VAL */
#define CFG_DATA_LL5_LPHDR_VAL_LL5_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL5_LPHDR_VAL_LL5_LPHDR_VAL_BIT_END        31U

/* Definition for field LL5_RD_THRESHOLD in Register CFG_DATA_LL5_THRESHOLD */
#define CFG_DATA_LL5_THRESHOLD_LL5_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL5_THRESHOLD_LL5_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL5_THRESHOLD */
#define CFG_DATA_LL5_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL5_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL5_WR_THRESHOLD in Register CFG_DATA_LL5_THRESHOLD */
#define CFG_DATA_LL5_THRESHOLD_LL5_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL5_THRESHOLD_LL5_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL5_THRESHOLD */
#define CFG_DATA_LL5_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL5_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll5dman in Register CFG_DATA_LL5_THRESHOLD */
#define CFG_DATA_LL5_THRESHOLD_LL5DMAN_BIT_START            16U
#define CFG_DATA_LL5_THRESHOLD_LL5DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL5_THRESHOLD */
#define CFG_DATA_LL5_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL5_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL6_VALID in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_VALID_BIT_START                    0U
#define CFG_DATA_LL6_LL6_VALID_BIT_END                      0U

/* Definition for field LL6_HE in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_HE_BIT_START                       1U
#define CFG_DATA_LL6_LL6_HE_BIT_END                         1U

/* Definition for field LL6_HS in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_HS_BIT_START                       2U
#define CFG_DATA_LL6_LL6_HS_BIT_END                         2U

/* Definition for field LL6_VCNUM in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_VCNUM_BIT_START                    3U
#define CFG_DATA_LL6_LL6_VCNUM_BIT_END                      4U

/* Definition for field LL6_FMT in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_FMT_BIT_START                      5U
#define CFG_DATA_LL6_LL6_FMT_BIT_END                        6U

/* Definition for field LL6_FMT_MAP in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL6_LL6_FMT_MAP_BIT_END                    7U

/* Definition for field LL6_FMT_IN in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL6_LL6_FMT_IN_BIT_END                     8U

/* Definition for field LL6_SIZE in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_SIZE_BIT_START                     9U
#define CFG_DATA_LL6_LL6_SIZE_BIT_END                       22U

/* Definition for field LL6_LPHDR_EN in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL6_LL6_LPHDR_EN_BIT_END                   27U

/* Definition for field LL6_CRC_EN in Register CFG_DATA_LL6 */
#define CFG_DATA_LL6_LL6_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL6_LL6_CRC_EN_BIT_END                     28U

/* Definition for field LL6_LPHDR_VAL in Register CFG_DATA_LL6_LPHDR_VAL */
#define CFG_DATA_LL6_LPHDR_VAL_LL6_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL6_LPHDR_VAL_LL6_LPHDR_VAL_BIT_END        31U

/* Definition for field LL6_RD_THRESHOLD in Register CFG_DATA_LL6_THRESHOLD */
#define CFG_DATA_LL6_THRESHOLD_LL6_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL6_THRESHOLD_LL6_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL6_THRESHOLD */
#define CFG_DATA_LL6_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL6_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL6_WR_THRESHOLD in Register CFG_DATA_LL6_THRESHOLD */
#define CFG_DATA_LL6_THRESHOLD_LL6_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL6_THRESHOLD_LL6_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL6_THRESHOLD */
#define CFG_DATA_LL6_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL6_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll6dman in Register CFG_DATA_LL6_THRESHOLD */
#define CFG_DATA_LL6_THRESHOLD_LL6DMAN_BIT_START            16U
#define CFG_DATA_LL6_THRESHOLD_LL6DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL6_THRESHOLD */
#define CFG_DATA_LL6_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL6_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL7_VALID in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_VALID_BIT_START                    0U
#define CFG_DATA_LL7_LL7_VALID_BIT_END                      0U

/* Definition for field LL7_HE in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_HE_BIT_START                       1U
#define CFG_DATA_LL7_LL7_HE_BIT_END                         1U

/* Definition for field LL7_HS in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_HS_BIT_START                       2U
#define CFG_DATA_LL7_LL7_HS_BIT_END                         2U

/* Definition for field LL7_VCNUM in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_VCNUM_BIT_START                    3U
#define CFG_DATA_LL7_LL7_VCNUM_BIT_END                      4U

/* Definition for field LL7_FMT in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_FMT_BIT_START                      5U
#define CFG_DATA_LL7_LL7_FMT_BIT_END                        6U

/* Definition for field LL7_FMT_MAP in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL7_LL7_FMT_MAP_BIT_END                    7U

/* Definition for field LL7_FMT_IN in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL7_LL7_FMT_IN_BIT_END                     8U

/* Definition for field LL7_SIZE in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_SIZE_BIT_START                     9U
#define CFG_DATA_LL7_LL7_SIZE_BIT_END                       22U

/* Definition for field LL7_LPHDR_EN in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL7_LL7_LPHDR_EN_BIT_END                   27U

/* Definition for field LL7_CRC_EN in Register CFG_DATA_LL7 */
#define CFG_DATA_LL7_LL7_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL7_LL7_CRC_EN_BIT_END                     28U

/* Definition for field LL7_LPHDR_VAL in Register CFG_DATA_LL7_LPHDR_VAL */
#define CFG_DATA_LL7_LPHDR_VAL_LL7_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL7_LPHDR_VAL_LL7_LPHDR_VAL_BIT_END        31U

/* Definition for field LL7_RD_THRESHOLD in Register CFG_DATA_LL7_THRESHOLD */
#define CFG_DATA_LL7_THRESHOLD_LL7_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL7_THRESHOLD_LL7_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL7_THRESHOLD */
#define CFG_DATA_LL7_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL7_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL7_WR_THRESHOLD in Register CFG_DATA_LL7_THRESHOLD */
#define CFG_DATA_LL7_THRESHOLD_LL7_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL7_THRESHOLD_LL7_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL7_THRESHOLD */
#define CFG_DATA_LL7_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL7_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll7dman in Register CFG_DATA_LL7_THRESHOLD */
#define CFG_DATA_LL7_THRESHOLD_LL7DMAN_BIT_START            16U
#define CFG_DATA_LL7_THRESHOLD_LL7DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL7_THRESHOLD */
#define CFG_DATA_LL7_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL7_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL8_VALID in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_VALID_BIT_START                    0U
#define CFG_DATA_LL8_LL8_VALID_BIT_END                      0U

/* Definition for field LL8_HE in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_HE_BIT_START                       1U
#define CFG_DATA_LL8_LL8_HE_BIT_END                         1U

/* Definition for field LL8_HS in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_HS_BIT_START                       2U
#define CFG_DATA_LL8_LL8_HS_BIT_END                         2U

/* Definition for field LL8_VCNUM in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_VCNUM_BIT_START                    3U
#define CFG_DATA_LL8_LL8_VCNUM_BIT_END                      4U

/* Definition for field LL8_FMT in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_FMT_BIT_START                      5U
#define CFG_DATA_LL8_LL8_FMT_BIT_END                        6U

/* Definition for field LL8_FMT_MAP in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL8_LL8_FMT_MAP_BIT_END                    7U

/* Definition for field LL8_FMT_IN in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL8_LL8_FMT_IN_BIT_END                     8U

/* Definition for field LL8_SIZE in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_SIZE_BIT_START                     9U
#define CFG_DATA_LL8_LL8_SIZE_BIT_END                       22U

/* Definition for field LL8_LPHDR_EN in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL8_LL8_LPHDR_EN_BIT_END                   27U

/* Definition for field LL8_CRC_EN in Register CFG_DATA_LL8 */
#define CFG_DATA_LL8_LL8_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL8_LL8_CRC_EN_BIT_END                     28U

/* Definition for field LL8_LPHDR_VAL in Register CFG_DATA_LL8_LPHDR_VAL */
#define CFG_DATA_LL8_LPHDR_VAL_LL8_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL8_LPHDR_VAL_LL8_LPHDR_VAL_BIT_END        31U

/* Definition for field LL8_RD_THRESHOLD in Register CFG_DATA_LL8_THRESHOLD */
#define CFG_DATA_LL8_THRESHOLD_LL8_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL8_THRESHOLD_LL8_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL8_THRESHOLD */
#define CFG_DATA_LL8_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL8_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL8_WR_THRESHOLD in Register CFG_DATA_LL8_THRESHOLD */
#define CFG_DATA_LL8_THRESHOLD_LL8_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL8_THRESHOLD_LL8_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL8_THRESHOLD */
#define CFG_DATA_LL8_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL8_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll8dman in Register CFG_DATA_LL8_THRESHOLD */
#define CFG_DATA_LL8_THRESHOLD_LL8DMAN_BIT_START            16U
#define CFG_DATA_LL8_THRESHOLD_LL8DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL8_THRESHOLD */
#define CFG_DATA_LL8_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL8_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL9_VALID in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_VALID_BIT_START                    0U
#define CFG_DATA_LL9_LL9_VALID_BIT_END                      0U

/* Definition for field LL9_HE in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_HE_BIT_START                       1U
#define CFG_DATA_LL9_LL9_HE_BIT_END                         1U

/* Definition for field LL9_HS in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_HS_BIT_START                       2U
#define CFG_DATA_LL9_LL9_HS_BIT_END                         2U

/* Definition for field LL9_VCNUM in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_VCNUM_BIT_START                    3U
#define CFG_DATA_LL9_LL9_VCNUM_BIT_END                      4U

/* Definition for field LL9_FMT in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_FMT_BIT_START                      5U
#define CFG_DATA_LL9_LL9_FMT_BIT_END                        6U

/* Definition for field LL9_FMT_MAP in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_FMT_MAP_BIT_START                  7U
#define CFG_DATA_LL9_LL9_FMT_MAP_BIT_END                    7U

/* Definition for field LL9_FMT_IN in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_FMT_IN_BIT_START                   8U
#define CFG_DATA_LL9_LL9_FMT_IN_BIT_END                     8U

/* Definition for field LL9_SIZE in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_SIZE_BIT_START                     9U
#define CFG_DATA_LL9_LL9_SIZE_BIT_END                       22U

/* Definition for field LL9_LPHDR_EN in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_LPHDR_EN_BIT_START                 27U
#define CFG_DATA_LL9_LL9_LPHDR_EN_BIT_END                   27U

/* Definition for field LL9_CRC_EN in Register CFG_DATA_LL9 */
#define CFG_DATA_LL9_LL9_CRC_EN_BIT_START                   28U
#define CFG_DATA_LL9_LL9_CRC_EN_BIT_END                     28U

/* Definition for field LL9_LPHDR_VAL in Register CFG_DATA_LL9_LPHDR_VAL */
#define CFG_DATA_LL9_LPHDR_VAL_LL9_LPHDR_VAL_BIT_START      0U
#define CFG_DATA_LL9_LPHDR_VAL_LL9_LPHDR_VAL_BIT_END        31U

/* Definition for field LL9_RD_THRESHOLD in Register CFG_DATA_LL9_THRESHOLD */
#define CFG_DATA_LL9_THRESHOLD_LL9_RD_THRESHOLD_BIT_START   0U
#define CFG_DATA_LL9_THRESHOLD_LL9_RD_THRESHOLD_BIT_END     6U

/* Definition for field NU1 in Register CFG_DATA_LL9_THRESHOLD */
#define CFG_DATA_LL9_THRESHOLD_NU1_BIT_START                7U
#define CFG_DATA_LL9_THRESHOLD_NU1_BIT_END                  7U

/* Definition for field LL9_WR_THRESHOLD in Register CFG_DATA_LL9_THRESHOLD */
#define CFG_DATA_LL9_THRESHOLD_LL9_WR_THRESHOLD_BIT_START   8U
#define CFG_DATA_LL9_THRESHOLD_LL9_WR_THRESHOLD_BIT_END     14U

/* Definition for field NU2 in Register CFG_DATA_LL9_THRESHOLD */
#define CFG_DATA_LL9_THRESHOLD_NU2_BIT_START                15U
#define CFG_DATA_LL9_THRESHOLD_NU2_BIT_END                  15U

/* Definition for field ll9dman in Register CFG_DATA_LL9_THRESHOLD */
#define CFG_DATA_LL9_THRESHOLD_LL9DMAN_BIT_START            16U
#define CFG_DATA_LL9_THRESHOLD_LL9DMAN_BIT_END              18U

/* Definition for field NU3 in Register CFG_DATA_LL9_THRESHOLD */
#define CFG_DATA_LL9_THRESHOLD_NU3_BIT_START                19U
#define CFG_DATA_LL9_THRESHOLD_NU3_BIT_END                  31U

/* Definition for field LL10_VALID in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_VALID_BIT_START                  0U
#define CFG_DATA_LL10_LL10_VALID_BIT_END                    0U

/* Definition for field LL10_HE in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_HE_BIT_START                     1U
#define CFG_DATA_LL10_LL10_HE_BIT_END                       1U

/* Definition for field LL10_HS in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_HS_BIT_START                     2U
#define CFG_DATA_LL10_LL10_HS_BIT_END                       2U

/* Definition for field LL10_VCNUM in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_VCNUM_BIT_START                  3U
#define CFG_DATA_LL10_LL10_VCNUM_BIT_END                    4U

/* Definition for field LL10_FMT in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_FMT_BIT_START                    5U
#define CFG_DATA_LL10_LL10_FMT_BIT_END                      6U

/* Definition for field LL10_FMT_MAP in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL10_LL10_FMT_MAP_BIT_END                  7U

/* Definition for field LL10_FMT_IN in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL10_LL10_FMT_IN_BIT_END                   8U

/* Definition for field LL10_SIZE in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_SIZE_BIT_START                   9U
#define CFG_DATA_LL10_LL10_SIZE_BIT_END                     22U

/* Definition for field LL10_LPHDR_EN in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL10_LL10_LPHDR_EN_BIT_END                 27U

/* Definition for field LL10_CRC_EN in Register CFG_DATA_LL10 */
#define CFG_DATA_LL10_LL10_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL10_LL10_CRC_EN_BIT_END                   28U

/* Definition for field LL10_LPHDR_VAL in Register CFG_DATA_LL10_LPHDR_VAL */
#define CFG_DATA_LL10_LPHDR_VAL_LL10_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL10_LPHDR_VAL_LL10_LPHDR_VAL_BIT_END      31U

/* Definition for field LL10_RD_THRESHOLD in Register CFG_DATA_LL10_THRESHOLD */
#define CFG_DATA_LL10_THRESHOLD_LL10_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL10_THRESHOLD_LL10_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL10_THRESHOLD */
#define CFG_DATA_LL10_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL10_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL10_WR_THRESHOLD in Register CFG_DATA_LL10_THRESHOLD */
#define CFG_DATA_LL10_THRESHOLD_LL10_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL10_THRESHOLD_LL10_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL10_THRESHOLD */
#define CFG_DATA_LL10_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL10_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll10dman in Register CFG_DATA_LL10_THRESHOLD */
#define CFG_DATA_LL10_THRESHOLD_LL10DMAN_BIT_START          16U
#define CFG_DATA_LL10_THRESHOLD_LL10DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL10_THRESHOLD */
#define CFG_DATA_LL10_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL10_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL11_VALID in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_VALID_BIT_START                  0U
#define CFG_DATA_LL11_LL11_VALID_BIT_END                    0U

/* Definition for field LL11_HE in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_HE_BIT_START                     1U
#define CFG_DATA_LL11_LL11_HE_BIT_END                       1U

/* Definition for field LL11_HS in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_HS_BIT_START                     2U
#define CFG_DATA_LL11_LL11_HS_BIT_END                       2U

/* Definition for field LL11_VCNUM in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_VCNUM_BIT_START                  3U
#define CFG_DATA_LL11_LL11_VCNUM_BIT_END                    4U

/* Definition for field LL11_FMT in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_FMT_BIT_START                    5U
#define CFG_DATA_LL11_LL11_FMT_BIT_END                      6U

/* Definition for field LL11_FMT_MAP in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL11_LL11_FMT_MAP_BIT_END                  7U

/* Definition for field LL11_FMT_IN in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL11_LL11_FMT_IN_BIT_END                   8U

/* Definition for field LL11_SIZE in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_SIZE_BIT_START                   9U
#define CFG_DATA_LL11_LL11_SIZE_BIT_END                     22U

/* Definition for field LL11_LPHDR_EN in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL11_LL11_LPHDR_EN_BIT_END                 27U

/* Definition for field LL11_CRC_EN in Register CFG_DATA_LL11 */
#define CFG_DATA_LL11_LL11_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL11_LL11_CRC_EN_BIT_END                   28U

/* Definition for field LL11_LPHDR_VAL in Register CFG_DATA_LL11_LPHDR_VAL */
#define CFG_DATA_LL11_LPHDR_VAL_LL11_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL11_LPHDR_VAL_LL11_LPHDR_VAL_BIT_END      31U

/* Definition for field LL11_RD_THRESHOLD in Register CFG_DATA_LL11_THRESHOLD */
#define CFG_DATA_LL11_THRESHOLD_LL11_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL11_THRESHOLD_LL11_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL11_THRESHOLD */
#define CFG_DATA_LL11_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL11_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL11_WR_THRESHOLD in Register CFG_DATA_LL11_THRESHOLD */
#define CFG_DATA_LL11_THRESHOLD_LL11_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL11_THRESHOLD_LL11_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL11_THRESHOLD */
#define CFG_DATA_LL11_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL11_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll11dman in Register CFG_DATA_LL11_THRESHOLD */
#define CFG_DATA_LL11_THRESHOLD_LL11DMAN_BIT_START          16U
#define CFG_DATA_LL11_THRESHOLD_LL11DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL11_THRESHOLD */
#define CFG_DATA_LL11_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL11_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL12_VALID in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_VALID_BIT_START                  0U
#define CFG_DATA_LL12_LL12_VALID_BIT_END                    0U

/* Definition for field LL12_HE in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_HE_BIT_START                     1U
#define CFG_DATA_LL12_LL12_HE_BIT_END                       1U

/* Definition for field LL12_HS in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_HS_BIT_START                     2U
#define CFG_DATA_LL12_LL12_HS_BIT_END                       2U

/* Definition for field LL12_VCNUM in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_VCNUM_BIT_START                  3U
#define CFG_DATA_LL12_LL12_VCNUM_BIT_END                    4U

/* Definition for field LL12_FMT in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_FMT_BIT_START                    5U
#define CFG_DATA_LL12_LL12_FMT_BIT_END                      6U

/* Definition for field LL12_FMT_MAP in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL12_LL12_FMT_MAP_BIT_END                  7U

/* Definition for field LL12_FMT_IN in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL12_LL12_FMT_IN_BIT_END                   8U

/* Definition for field LL12_SIZE in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_SIZE_BIT_START                   9U
#define CFG_DATA_LL12_LL12_SIZE_BIT_END                     22U

/* Definition for field LL12_LPHDR_EN in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL12_LL12_LPHDR_EN_BIT_END                 27U

/* Definition for field LL12_CRC_EN in Register CFG_DATA_LL12 */
#define CFG_DATA_LL12_LL12_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL12_LL12_CRC_EN_BIT_END                   28U

/* Definition for field LL12_LPHDR_VAL in Register CFG_DATA_LL12_LPHDR_VAL */
#define CFG_DATA_LL12_LPHDR_VAL_LL12_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL12_LPHDR_VAL_LL12_LPHDR_VAL_BIT_END      31U

/* Definition for field LL12_RD_THRESHOLD in Register CFG_DATA_LL12_THRESHOLD */
#define CFG_DATA_LL12_THRESHOLD_LL12_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL12_THRESHOLD_LL12_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL12_THRESHOLD */
#define CFG_DATA_LL12_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL12_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL12_WR_THRESHOLD in Register CFG_DATA_LL12_THRESHOLD */
#define CFG_DATA_LL12_THRESHOLD_LL12_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL12_THRESHOLD_LL12_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL12_THRESHOLD */
#define CFG_DATA_LL12_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL12_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll12dman in Register CFG_DATA_LL12_THRESHOLD */
#define CFG_DATA_LL12_THRESHOLD_LL12DMAN_BIT_START          16U
#define CFG_DATA_LL12_THRESHOLD_LL12DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL12_THRESHOLD */
#define CFG_DATA_LL12_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL12_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL13_VALID in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_VALID_BIT_START                  0U
#define CFG_DATA_LL13_LL13_VALID_BIT_END                    0U

/* Definition for field LL13_HE in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_HE_BIT_START                     1U
#define CFG_DATA_LL13_LL13_HE_BIT_END                       1U

/* Definition for field LL13_HS in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_HS_BIT_START                     2U
#define CFG_DATA_LL13_LL13_HS_BIT_END                       2U

/* Definition for field LL13_VCNUM in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_VCNUM_BIT_START                  3U
#define CFG_DATA_LL13_LL13_VCNUM_BIT_END                    4U

/* Definition for field LL13_FMT in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_FMT_BIT_START                    5U
#define CFG_DATA_LL13_LL13_FMT_BIT_END                      6U

/* Definition for field LL13_FMT_MAP in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL13_LL13_FMT_MAP_BIT_END                  7U

/* Definition for field LL13_FMT_IN in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL13_LL13_FMT_IN_BIT_END                   8U

/* Definition for field LL13_SIZE in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_SIZE_BIT_START                   9U
#define CFG_DATA_LL13_LL13_SIZE_BIT_END                     22U

/* Definition for field LL13_LPHDR_EN in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL13_LL13_LPHDR_EN_BIT_END                 27U

/* Definition for field LL13_CRC_EN in Register CFG_DATA_LL13 */
#define CFG_DATA_LL13_LL13_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL13_LL13_CRC_EN_BIT_END                   28U

/* Definition for field LL13_LPHDR_VAL in Register CFG_DATA_LL13_LPHDR_VAL */
#define CFG_DATA_LL13_LPHDR_VAL_LL13_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL13_LPHDR_VAL_LL13_LPHDR_VAL_BIT_END      31U

/* Definition for field LL13_RD_THRESHOLD in Register CFG_DATA_LL13_THRESHOLD */
#define CFG_DATA_LL13_THRESHOLD_LL13_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL13_THRESHOLD_LL13_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL13_THRESHOLD */
#define CFG_DATA_LL13_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL13_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL13_WR_THRESHOLD in Register CFG_DATA_LL13_THRESHOLD */
#define CFG_DATA_LL13_THRESHOLD_LL13_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL13_THRESHOLD_LL13_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL13_THRESHOLD */
#define CFG_DATA_LL13_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL13_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll13dman in Register CFG_DATA_LL13_THRESHOLD */
#define CFG_DATA_LL13_THRESHOLD_LL13DMAN_BIT_START          16U
#define CFG_DATA_LL13_THRESHOLD_LL13DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL13_THRESHOLD */
#define CFG_DATA_LL13_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL13_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL14_VALID in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_VALID_BIT_START                  0U
#define CFG_DATA_LL14_LL14_VALID_BIT_END                    0U

/* Definition for field LL14_HE in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_HE_BIT_START                     1U
#define CFG_DATA_LL14_LL14_HE_BIT_END                       1U

/* Definition for field LL14_HS in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_HS_BIT_START                     2U
#define CFG_DATA_LL14_LL14_HS_BIT_END                       2U

/* Definition for field LL14_VCNUM in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_VCNUM_BIT_START                  3U
#define CFG_DATA_LL14_LL14_VCNUM_BIT_END                    4U

/* Definition for field LL14_FMT in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_FMT_BIT_START                    5U
#define CFG_DATA_LL14_LL14_FMT_BIT_END                      6U

/* Definition for field LL14_FMT_MAP in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL14_LL14_FMT_MAP_BIT_END                  7U

/* Definition for field LL14_FMT_IN in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL14_LL14_FMT_IN_BIT_END                   8U

/* Definition for field LL14_SIZE in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_SIZE_BIT_START                   9U
#define CFG_DATA_LL14_LL14_SIZE_BIT_END                     22U

/* Definition for field LL14_LPHDR_EN in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL14_LL14_LPHDR_EN_BIT_END                 27U

/* Definition for field LL14_CRC_EN in Register CFG_DATA_LL14 */
#define CFG_DATA_LL14_LL14_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL14_LL14_CRC_EN_BIT_END                   28U

/* Definition for field LL14_LPHDR_VAL in Register CFG_DATA_LL14_LPHDR_VAL */
#define CFG_DATA_LL14_LPHDR_VAL_LL14_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL14_LPHDR_VAL_LL14_LPHDR_VAL_BIT_END      31U

/* Definition for field LL14_RD_THRESHOLD in Register CFG_DATA_LL14_THRESHOLD */
#define CFG_DATA_LL14_THRESHOLD_LL14_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL14_THRESHOLD_LL14_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL14_THRESHOLD */
#define CFG_DATA_LL14_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL14_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL14_WR_THRESHOLD in Register CFG_DATA_LL14_THRESHOLD */
#define CFG_DATA_LL14_THRESHOLD_LL14_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL14_THRESHOLD_LL14_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL14_THRESHOLD */
#define CFG_DATA_LL14_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL14_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll14dman in Register CFG_DATA_LL14_THRESHOLD */
#define CFG_DATA_LL14_THRESHOLD_LL14DMAN_BIT_START          16U
#define CFG_DATA_LL14_THRESHOLD_LL14DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL14_THRESHOLD */
#define CFG_DATA_LL14_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL14_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL15_VALID in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_VALID_BIT_START                  0U
#define CFG_DATA_LL15_LL15_VALID_BIT_END                    0U

/* Definition for field LL15_HE in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_HE_BIT_START                     1U
#define CFG_DATA_LL15_LL15_HE_BIT_END                       1U

/* Definition for field LL15_HS in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_HS_BIT_START                     2U
#define CFG_DATA_LL15_LL15_HS_BIT_END                       2U

/* Definition for field LL15_VCNUM in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_VCNUM_BIT_START                  3U
#define CFG_DATA_LL15_LL15_VCNUM_BIT_END                    4U

/* Definition for field LL15_FMT in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_FMT_BIT_START                    5U
#define CFG_DATA_LL15_LL15_FMT_BIT_END                      6U

/* Definition for field LL15_FMT_MAP in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL15_LL15_FMT_MAP_BIT_END                  7U

/* Definition for field LL15_FMT_IN in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL15_LL15_FMT_IN_BIT_END                   8U

/* Definition for field LL15_SIZE in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_SIZE_BIT_START                   9U
#define CFG_DATA_LL15_LL15_SIZE_BIT_END                     22U

/* Definition for field LL15_LPHDR_EN in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL15_LL15_LPHDR_EN_BIT_END                 27U

/* Definition for field LL15_CRC_EN in Register CFG_DATA_LL15 */
#define CFG_DATA_LL15_LL15_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL15_LL15_CRC_EN_BIT_END                   28U

/* Definition for field LL15_LPHDR_VAL in Register CFG_DATA_LL15_LPHDR_VAL */
#define CFG_DATA_LL15_LPHDR_VAL_LL15_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL15_LPHDR_VAL_LL15_LPHDR_VAL_BIT_END      31U

/* Definition for field LL15_RD_THRESHOLD in Register CFG_DATA_LL15_THRESHOLD */
#define CFG_DATA_LL15_THRESHOLD_LL15_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL15_THRESHOLD_LL15_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL15_THRESHOLD */
#define CFG_DATA_LL15_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL15_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL15_WR_THRESHOLD in Register CFG_DATA_LL15_THRESHOLD */
#define CFG_DATA_LL15_THRESHOLD_LL15_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL15_THRESHOLD_LL15_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL15_THRESHOLD */
#define CFG_DATA_LL15_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL15_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll15dman in Register CFG_DATA_LL15_THRESHOLD */
#define CFG_DATA_LL15_THRESHOLD_LL15DMAN_BIT_START          16U
#define CFG_DATA_LL15_THRESHOLD_LL15DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL15_THRESHOLD */
#define CFG_DATA_LL15_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL15_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL16_VALID in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_VALID_BIT_START                  0U
#define CFG_DATA_LL16_LL16_VALID_BIT_END                    0U

/* Definition for field LL16_HE in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_HE_BIT_START                     1U
#define CFG_DATA_LL16_LL16_HE_BIT_END                       1U

/* Definition for field LL16_HS in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_HS_BIT_START                     2U
#define CFG_DATA_LL16_LL16_HS_BIT_END                       2U

/* Definition for field LL16_VCNUM in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_VCNUM_BIT_START                  3U
#define CFG_DATA_LL16_LL16_VCNUM_BIT_END                    4U

/* Definition for field LL16_FMT in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_FMT_BIT_START                    5U
#define CFG_DATA_LL16_LL16_FMT_BIT_END                      6U

/* Definition for field LL16_FMT_MAP in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL16_LL16_FMT_MAP_BIT_END                  7U

/* Definition for field LL16_FMT_IN in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL16_LL16_FMT_IN_BIT_END                   8U

/* Definition for field LL16_SIZE in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_SIZE_BIT_START                   9U
#define CFG_DATA_LL16_LL16_SIZE_BIT_END                     22U

/* Definition for field LL16_LPHDR_EN in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL16_LL16_LPHDR_EN_BIT_END                 27U

/* Definition for field LL16_CRC_EN in Register CFG_DATA_LL16 */
#define CFG_DATA_LL16_LL16_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL16_LL16_CRC_EN_BIT_END                   28U

/* Definition for field LL16_LPHDR_VAL in Register CFG_DATA_LL16_LPHDR_VAL */
#define CFG_DATA_LL16_LPHDR_VAL_LL16_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL16_LPHDR_VAL_LL16_LPHDR_VAL_BIT_END      31U

/* Definition for field LL16_RD_THRESHOLD in Register CFG_DATA_LL16_THRESHOLD */
#define CFG_DATA_LL16_THRESHOLD_LL16_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL16_THRESHOLD_LL16_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL16_THRESHOLD */
#define CFG_DATA_LL16_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL16_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL16_WR_THRESHOLD in Register CFG_DATA_LL16_THRESHOLD */
#define CFG_DATA_LL16_THRESHOLD_LL16_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL16_THRESHOLD_LL16_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL16_THRESHOLD */
#define CFG_DATA_LL16_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL16_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll16dman in Register CFG_DATA_LL16_THRESHOLD */
#define CFG_DATA_LL16_THRESHOLD_LL16DMAN_BIT_START          16U
#define CFG_DATA_LL16_THRESHOLD_LL16DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL16_THRESHOLD */
#define CFG_DATA_LL16_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL16_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL17_VALID in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_VALID_BIT_START                  0U
#define CFG_DATA_LL17_LL17_VALID_BIT_END                    0U

/* Definition for field LL17_HE in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_HE_BIT_START                     1U
#define CFG_DATA_LL17_LL17_HE_BIT_END                       1U

/* Definition for field LL17_HS in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_HS_BIT_START                     2U
#define CFG_DATA_LL17_LL17_HS_BIT_END                       2U

/* Definition for field LL17_VCNUM in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_VCNUM_BIT_START                  3U
#define CFG_DATA_LL17_LL17_VCNUM_BIT_END                    4U

/* Definition for field LL17_FMT in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_FMT_BIT_START                    5U
#define CFG_DATA_LL17_LL17_FMT_BIT_END                      6U

/* Definition for field LL17_FMT_MAP in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL17_LL17_FMT_MAP_BIT_END                  7U

/* Definition for field LL17_FMT_IN in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL17_LL17_FMT_IN_BIT_END                   8U

/* Definition for field LL17_SIZE in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_SIZE_BIT_START                   9U
#define CFG_DATA_LL17_LL17_SIZE_BIT_END                     22U

/* Definition for field LL17_LPHDR_EN in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL17_LL17_LPHDR_EN_BIT_END                 27U

/* Definition for field LL17_CRC_EN in Register CFG_DATA_LL17 */
#define CFG_DATA_LL17_LL17_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL17_LL17_CRC_EN_BIT_END                   28U

/* Definition for field LL17_LPHDR_VAL in Register CFG_DATA_LL17_LPHDR_VAL */
#define CFG_DATA_LL17_LPHDR_VAL_LL17_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL17_LPHDR_VAL_LL17_LPHDR_VAL_BIT_END      31U

/* Definition for field LL17_RD_THRESHOLD in Register CFG_DATA_LL17_THRESHOLD */
#define CFG_DATA_LL17_THRESHOLD_LL17_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL17_THRESHOLD_LL17_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL17_THRESHOLD */
#define CFG_DATA_LL17_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL17_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL17_WR_THRESHOLD in Register CFG_DATA_LL17_THRESHOLD */
#define CFG_DATA_LL17_THRESHOLD_LL17_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL17_THRESHOLD_LL17_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL17_THRESHOLD */
#define CFG_DATA_LL17_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL17_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll17dman in Register CFG_DATA_LL17_THRESHOLD */
#define CFG_DATA_LL17_THRESHOLD_LL17DMAN_BIT_START          16U
#define CFG_DATA_LL17_THRESHOLD_LL17DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL17_THRESHOLD */
#define CFG_DATA_LL17_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL17_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL18_VALID in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_VALID_BIT_START                  0U
#define CFG_DATA_LL18_LL18_VALID_BIT_END                    0U

/* Definition for field LL18_HE in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_HE_BIT_START                     1U
#define CFG_DATA_LL18_LL18_HE_BIT_END                       1U

/* Definition for field LL18_HS in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_HS_BIT_START                     2U
#define CFG_DATA_LL18_LL18_HS_BIT_END                       2U

/* Definition for field LL18_VCNUM in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_VCNUM_BIT_START                  3U
#define CFG_DATA_LL18_LL18_VCNUM_BIT_END                    4U

/* Definition for field LL18_FMT in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_FMT_BIT_START                    5U
#define CFG_DATA_LL18_LL18_FMT_BIT_END                      6U

/* Definition for field LL18_FMT_MAP in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL18_LL18_FMT_MAP_BIT_END                  7U

/* Definition for field LL18_FMT_IN in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL18_LL18_FMT_IN_BIT_END                   8U

/* Definition for field LL18_SIZE in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_SIZE_BIT_START                   9U
#define CFG_DATA_LL18_LL18_SIZE_BIT_END                     22U

/* Definition for field LL18_LPHDR_EN in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL18_LL18_LPHDR_EN_BIT_END                 27U

/* Definition for field LL18_CRC_EN in Register CFG_DATA_LL18 */
#define CFG_DATA_LL18_LL18_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL18_LL18_CRC_EN_BIT_END                   28U

/* Definition for field LL18_LPHDR_VAL in Register CFG_DATA_LL18_LPHDR_VAL */
#define CFG_DATA_LL18_LPHDR_VAL_LL18_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL18_LPHDR_VAL_LL18_LPHDR_VAL_BIT_END      31U

/* Definition for field LL18_RD_THRESHOLD in Register CFG_DATA_LL18_THRESHOLD */
#define CFG_DATA_LL18_THRESHOLD_LL18_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL18_THRESHOLD_LL18_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL18_THRESHOLD */
#define CFG_DATA_LL18_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL18_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL18_WR_THRESHOLD in Register CFG_DATA_LL18_THRESHOLD */
#define CFG_DATA_LL18_THRESHOLD_LL18_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL18_THRESHOLD_LL18_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL18_THRESHOLD */
#define CFG_DATA_LL18_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL18_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll18dman in Register CFG_DATA_LL18_THRESHOLD */
#define CFG_DATA_LL18_THRESHOLD_LL18DMAN_BIT_START          16U
#define CFG_DATA_LL18_THRESHOLD_LL18DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL18_THRESHOLD */
#define CFG_DATA_LL18_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL18_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL19_VALID in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_VALID_BIT_START                  0U
#define CFG_DATA_LL19_LL19_VALID_BIT_END                    0U

/* Definition for field LL19_HE in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_HE_BIT_START                     1U
#define CFG_DATA_LL19_LL19_HE_BIT_END                       1U

/* Definition for field LL19_HS in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_HS_BIT_START                     2U
#define CFG_DATA_LL19_LL19_HS_BIT_END                       2U

/* Definition for field LL19_VCNUM in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_VCNUM_BIT_START                  3U
#define CFG_DATA_LL19_LL19_VCNUM_BIT_END                    4U

/* Definition for field LL19_FMT in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_FMT_BIT_START                    5U
#define CFG_DATA_LL19_LL19_FMT_BIT_END                      6U

/* Definition for field LL19_FMT_MAP in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL19_LL19_FMT_MAP_BIT_END                  7U

/* Definition for field LL19_FMT_IN in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL19_LL19_FMT_IN_BIT_END                   8U

/* Definition for field LL19_SIZE in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_SIZE_BIT_START                   9U
#define CFG_DATA_LL19_LL19_SIZE_BIT_END                     22U

/* Definition for field LL19_LPHDR_EN in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL19_LL19_LPHDR_EN_BIT_END                 27U

/* Definition for field LL19_CRC_EN in Register CFG_DATA_LL19 */
#define CFG_DATA_LL19_LL19_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL19_LL19_CRC_EN_BIT_END                   28U

/* Definition for field LL19_LPHDR_VAL in Register CFG_DATA_LL19_LPHDR_VAL */
#define CFG_DATA_LL19_LPHDR_VAL_LL19_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL19_LPHDR_VAL_LL19_LPHDR_VAL_BIT_END      31U

/* Definition for field LL19_RD_THRESHOLD in Register CFG_DATA_LL19_THRESHOLD */
#define CFG_DATA_LL19_THRESHOLD_LL19_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL19_THRESHOLD_LL19_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL19_THRESHOLD */
#define CFG_DATA_LL19_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL19_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL19_WR_THRESHOLD in Register CFG_DATA_LL19_THRESHOLD */
#define CFG_DATA_LL19_THRESHOLD_LL19_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL19_THRESHOLD_LL19_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL19_THRESHOLD */
#define CFG_DATA_LL19_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL19_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll19dman in Register CFG_DATA_LL19_THRESHOLD */
#define CFG_DATA_LL19_THRESHOLD_LL19DMAN_BIT_START          16U
#define CFG_DATA_LL19_THRESHOLD_LL19DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL19_THRESHOLD */
#define CFG_DATA_LL19_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL19_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL20_VALID in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_VALID_BIT_START                  0U
#define CFG_DATA_LL20_LL20_VALID_BIT_END                    0U

/* Definition for field LL20_HE in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_HE_BIT_START                     1U
#define CFG_DATA_LL20_LL20_HE_BIT_END                       1U

/* Definition for field LL20_HS in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_HS_BIT_START                     2U
#define CFG_DATA_LL20_LL20_HS_BIT_END                       2U

/* Definition for field LL20_VCNUM in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_VCNUM_BIT_START                  3U
#define CFG_DATA_LL20_LL20_VCNUM_BIT_END                    4U

/* Definition for field LL20_FMT in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_FMT_BIT_START                    5U
#define CFG_DATA_LL20_LL20_FMT_BIT_END                      6U

/* Definition for field LL20_FMT_MAP in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL20_LL20_FMT_MAP_BIT_END                  7U

/* Definition for field LL20_FMT_IN in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL20_LL20_FMT_IN_BIT_END                   8U

/* Definition for field LL20_SIZE in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_SIZE_BIT_START                   9U
#define CFG_DATA_LL20_LL20_SIZE_BIT_END                     22U

/* Definition for field LL20_LPHDR_EN in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL20_LL20_LPHDR_EN_BIT_END                 27U

/* Definition for field LL20_CRC_EN in Register CFG_DATA_LL20 */
#define CFG_DATA_LL20_LL20_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL20_LL20_CRC_EN_BIT_END                   28U

/* Definition for field LL20_LPHDR_VAL in Register CFG_DATA_LL20_LPHDR_VAL */
#define CFG_DATA_LL20_LPHDR_VAL_LL20_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL20_LPHDR_VAL_LL20_LPHDR_VAL_BIT_END      31U

/* Definition for field LL20_RD_THRESHOLD in Register CFG_DATA_LL20_THRESHOLD */
#define CFG_DATA_LL20_THRESHOLD_LL20_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL20_THRESHOLD_LL20_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL20_THRESHOLD */
#define CFG_DATA_LL20_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL20_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL20_WR_THRESHOLD in Register CFG_DATA_LL20_THRESHOLD */
#define CFG_DATA_LL20_THRESHOLD_LL20_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL20_THRESHOLD_LL20_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL20_THRESHOLD */
#define CFG_DATA_LL20_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL20_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll20dman in Register CFG_DATA_LL20_THRESHOLD */
#define CFG_DATA_LL20_THRESHOLD_LL20DMAN_BIT_START          16U
#define CFG_DATA_LL20_THRESHOLD_LL20DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL20_THRESHOLD */
#define CFG_DATA_LL20_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL20_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL21_VALID in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_VALID_BIT_START                  0U
#define CFG_DATA_LL21_LL21_VALID_BIT_END                    0U

/* Definition for field LL21_HE in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_HE_BIT_START                     1U
#define CFG_DATA_LL21_LL21_HE_BIT_END                       1U

/* Definition for field LL21_HS in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_HS_BIT_START                     2U
#define CFG_DATA_LL21_LL21_HS_BIT_END                       2U

/* Definition for field LL21_VCNUM in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_VCNUM_BIT_START                  3U
#define CFG_DATA_LL21_LL21_VCNUM_BIT_END                    4U

/* Definition for field LL21_FMT in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_FMT_BIT_START                    5U
#define CFG_DATA_LL21_LL21_FMT_BIT_END                      6U

/* Definition for field LL21_FMT_MAP in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL21_LL21_FMT_MAP_BIT_END                  7U

/* Definition for field LL21_FMT_IN in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL21_LL21_FMT_IN_BIT_END                   8U

/* Definition for field LL21_SIZE in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_SIZE_BIT_START                   9U
#define CFG_DATA_LL21_LL21_SIZE_BIT_END                     22U

/* Definition for field LL21_LPHDR_EN in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL21_LL21_LPHDR_EN_BIT_END                 27U

/* Definition for field LL21_CRC_EN in Register CFG_DATA_LL21 */
#define CFG_DATA_LL21_LL21_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL21_LL21_CRC_EN_BIT_END                   28U

/* Definition for field LL21_LPHDR_VAL in Register CFG_DATA_LL21_LPHDR_VAL */
#define CFG_DATA_LL21_LPHDR_VAL_LL21_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL21_LPHDR_VAL_LL21_LPHDR_VAL_BIT_END      31U

/* Definition for field LL21_RD_THRESHOLD in Register CFG_DATA_LL21_THRESHOLD */
#define CFG_DATA_LL21_THRESHOLD_LL21_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL21_THRESHOLD_LL21_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL21_THRESHOLD */
#define CFG_DATA_LL21_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL21_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL21_WR_THRESHOLD in Register CFG_DATA_LL21_THRESHOLD */
#define CFG_DATA_LL21_THRESHOLD_LL21_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL21_THRESHOLD_LL21_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL21_THRESHOLD */
#define CFG_DATA_LL21_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL21_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll21dman in Register CFG_DATA_LL21_THRESHOLD */
#define CFG_DATA_LL21_THRESHOLD_LL21DMAN_BIT_START          16U
#define CFG_DATA_LL21_THRESHOLD_LL21DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL21_THRESHOLD */
#define CFG_DATA_LL21_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL21_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL22_VALID in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_VALID_BIT_START                  0U
#define CFG_DATA_LL22_LL22_VALID_BIT_END                    0U

/* Definition for field LL22_HE in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_HE_BIT_START                     1U
#define CFG_DATA_LL22_LL22_HE_BIT_END                       1U

/* Definition for field LL22_HS in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_HS_BIT_START                     2U
#define CFG_DATA_LL22_LL22_HS_BIT_END                       2U

/* Definition for field LL22_VCNUM in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_VCNUM_BIT_START                  3U
#define CFG_DATA_LL22_LL22_VCNUM_BIT_END                    4U

/* Definition for field LL22_FMT in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_FMT_BIT_START                    5U
#define CFG_DATA_LL22_LL22_FMT_BIT_END                      6U

/* Definition for field LL22_FMT_MAP in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL22_LL22_FMT_MAP_BIT_END                  7U

/* Definition for field LL22_FMT_IN in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL22_LL22_FMT_IN_BIT_END                   8U

/* Definition for field LL22_SIZE in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_SIZE_BIT_START                   9U
#define CFG_DATA_LL22_LL22_SIZE_BIT_END                     22U

/* Definition for field LL22_LPHDR_EN in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL22_LL22_LPHDR_EN_BIT_END                 27U

/* Definition for field LL22_CRC_EN in Register CFG_DATA_LL22 */
#define CFG_DATA_LL22_LL22_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL22_LL22_CRC_EN_BIT_END                   28U

/* Definition for field LL22_LPHDR_VAL in Register CFG_DATA_LL22_LPHDR_VAL */
#define CFG_DATA_LL22_LPHDR_VAL_LL22_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL22_LPHDR_VAL_LL22_LPHDR_VAL_BIT_END      31U

/* Definition for field LL22_RD_THRESHOLD in Register CFG_DATA_LL22_THRESHOLD */
#define CFG_DATA_LL22_THRESHOLD_LL22_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL22_THRESHOLD_LL22_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL22_THRESHOLD */
#define CFG_DATA_LL22_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL22_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL22_WR_THRESHOLD in Register CFG_DATA_LL22_THRESHOLD */
#define CFG_DATA_LL22_THRESHOLD_LL22_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL22_THRESHOLD_LL22_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL22_THRESHOLD */
#define CFG_DATA_LL22_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL22_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll22dman in Register CFG_DATA_LL22_THRESHOLD */
#define CFG_DATA_LL22_THRESHOLD_LL22DMAN_BIT_START          16U
#define CFG_DATA_LL22_THRESHOLD_LL22DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL22_THRESHOLD */
#define CFG_DATA_LL22_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL22_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL23_VALID in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_VALID_BIT_START                  0U
#define CFG_DATA_LL23_LL23_VALID_BIT_END                    0U

/* Definition for field LL23_HE in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_HE_BIT_START                     1U
#define CFG_DATA_LL23_LL23_HE_BIT_END                       1U

/* Definition for field LL23_HS in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_HS_BIT_START                     2U
#define CFG_DATA_LL23_LL23_HS_BIT_END                       2U

/* Definition for field LL23_VCNUM in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_VCNUM_BIT_START                  3U
#define CFG_DATA_LL23_LL23_VCNUM_BIT_END                    4U

/* Definition for field LL23_FMT in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_FMT_BIT_START                    5U
#define CFG_DATA_LL23_LL23_FMT_BIT_END                      6U

/* Definition for field LL23_FMT_MAP in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL23_LL23_FMT_MAP_BIT_END                  7U

/* Definition for field LL23_FMT_IN in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL23_LL23_FMT_IN_BIT_END                   8U

/* Definition for field LL23_SIZE in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_SIZE_BIT_START                   9U
#define CFG_DATA_LL23_LL23_SIZE_BIT_END                     22U

/* Definition for field LL23_LPHDR_EN in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL23_LL23_LPHDR_EN_BIT_END                 27U

/* Definition for field LL23_CRC_EN in Register CFG_DATA_LL23 */
#define CFG_DATA_LL23_LL23_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL23_LL23_CRC_EN_BIT_END                   28U

/* Definition for field LL23_LPHDR_VAL in Register CFG_DATA_LL23_LPHDR_VAL */
#define CFG_DATA_LL23_LPHDR_VAL_LL23_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL23_LPHDR_VAL_LL23_LPHDR_VAL_BIT_END      31U

/* Definition for field LL23_RD_THRESHOLD in Register CFG_DATA_LL23_THRESHOLD */
#define CFG_DATA_LL23_THRESHOLD_LL23_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL23_THRESHOLD_LL23_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL23_THRESHOLD */
#define CFG_DATA_LL23_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL23_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL23_WR_THRESHOLD in Register CFG_DATA_LL23_THRESHOLD */
#define CFG_DATA_LL23_THRESHOLD_LL23_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL23_THRESHOLD_LL23_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL23_THRESHOLD */
#define CFG_DATA_LL23_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL23_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll23dman in Register CFG_DATA_LL23_THRESHOLD */
#define CFG_DATA_LL23_THRESHOLD_LL23DMAN_BIT_START          16U
#define CFG_DATA_LL23_THRESHOLD_LL23DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL23_THRESHOLD */
#define CFG_DATA_LL23_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL23_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL24_VALID in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_VALID_BIT_START                  0U
#define CFG_DATA_LL24_LL24_VALID_BIT_END                    0U

/* Definition for field LL24_HE in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_HE_BIT_START                     1U
#define CFG_DATA_LL24_LL24_HE_BIT_END                       1U

/* Definition for field LL24_HS in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_HS_BIT_START                     2U
#define CFG_DATA_LL24_LL24_HS_BIT_END                       2U

/* Definition for field LL24_VCNUM in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_VCNUM_BIT_START                  3U
#define CFG_DATA_LL24_LL24_VCNUM_BIT_END                    4U

/* Definition for field LL24_FMT in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_FMT_BIT_START                    5U
#define CFG_DATA_LL24_LL24_FMT_BIT_END                      6U

/* Definition for field LL24_FMT_MAP in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL24_LL24_FMT_MAP_BIT_END                  7U

/* Definition for field LL24_FMT_IN in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL24_LL24_FMT_IN_BIT_END                   8U

/* Definition for field LL24_SIZE in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_SIZE_BIT_START                   9U
#define CFG_DATA_LL24_LL24_SIZE_BIT_END                     22U

/* Definition for field LL24_LPHDR_EN in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL24_LL24_LPHDR_EN_BIT_END                 27U

/* Definition for field LL24_CRC_EN in Register CFG_DATA_LL24 */
#define CFG_DATA_LL24_LL24_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL24_LL24_CRC_EN_BIT_END                   28U

/* Definition for field LL24_LPHDR_VAL in Register CFG_DATA_LL24_LPHDR_VAL */
#define CFG_DATA_LL24_LPHDR_VAL_LL24_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL24_LPHDR_VAL_LL24_LPHDR_VAL_BIT_END      31U

/* Definition for field LL24_RD_THRESHOLD in Register CFG_DATA_LL24_THRESHOLD */
#define CFG_DATA_LL24_THRESHOLD_LL24_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL24_THRESHOLD_LL24_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL24_THRESHOLD */
#define CFG_DATA_LL24_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL24_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL24_WR_THRESHOLD in Register CFG_DATA_LL24_THRESHOLD */
#define CFG_DATA_LL24_THRESHOLD_LL24_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL24_THRESHOLD_LL24_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL24_THRESHOLD */
#define CFG_DATA_LL24_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL24_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll24dman in Register CFG_DATA_LL24_THRESHOLD */
#define CFG_DATA_LL24_THRESHOLD_LL24DMAN_BIT_START          16U
#define CFG_DATA_LL24_THRESHOLD_LL24DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL24_THRESHOLD */
#define CFG_DATA_LL24_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL24_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL25_VALID in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_VALID_BIT_START                  0U
#define CFG_DATA_LL25_LL25_VALID_BIT_END                    0U

/* Definition for field LL25_HE in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_HE_BIT_START                     1U
#define CFG_DATA_LL25_LL25_HE_BIT_END                       1U

/* Definition for field LL25_HS in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_HS_BIT_START                     2U
#define CFG_DATA_LL25_LL25_HS_BIT_END                       2U

/* Definition for field LL25_VCNUM in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_VCNUM_BIT_START                  3U
#define CFG_DATA_LL25_LL25_VCNUM_BIT_END                    4U

/* Definition for field LL25_FMT in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_FMT_BIT_START                    5U
#define CFG_DATA_LL25_LL25_FMT_BIT_END                      6U

/* Definition for field LL25_FMT_MAP in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL25_LL25_FMT_MAP_BIT_END                  7U

/* Definition for field LL25_FMT_IN in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL25_LL25_FMT_IN_BIT_END                   8U

/* Definition for field LL25_SIZE in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_SIZE_BIT_START                   9U
#define CFG_DATA_LL25_LL25_SIZE_BIT_END                     22U

/* Definition for field LL25_LPHDR_EN in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL25_LL25_LPHDR_EN_BIT_END                 27U

/* Definition for field LL25_CRC_EN in Register CFG_DATA_LL25 */
#define CFG_DATA_LL25_LL25_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL25_LL25_CRC_EN_BIT_END                   28U

/* Definition for field LL25_LPHDR_VAL in Register CFG_DATA_LL25_LPHDR_VAL */
#define CFG_DATA_LL25_LPHDR_VAL_LL25_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL25_LPHDR_VAL_LL25_LPHDR_VAL_BIT_END      31U

/* Definition for field LL25_RD_THRESHOLD in Register CFG_DATA_LL25_THRESHOLD */
#define CFG_DATA_LL25_THRESHOLD_LL25_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL25_THRESHOLD_LL25_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL25_THRESHOLD */
#define CFG_DATA_LL25_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL25_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL25_WR_THRESHOLD in Register CFG_DATA_LL25_THRESHOLD */
#define CFG_DATA_LL25_THRESHOLD_LL25_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL25_THRESHOLD_LL25_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL25_THRESHOLD */
#define CFG_DATA_LL25_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL25_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll25dman in Register CFG_DATA_LL25_THRESHOLD */
#define CFG_DATA_LL25_THRESHOLD_LL25DMAN_BIT_START          16U
#define CFG_DATA_LL25_THRESHOLD_LL25DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL25_THRESHOLD */
#define CFG_DATA_LL25_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL25_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL26_VALID in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_VALID_BIT_START                  0U
#define CFG_DATA_LL26_LL26_VALID_BIT_END                    0U

/* Definition for field LL26_HE in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_HE_BIT_START                     1U
#define CFG_DATA_LL26_LL26_HE_BIT_END                       1U

/* Definition for field LL26_HS in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_HS_BIT_START                     2U
#define CFG_DATA_LL26_LL26_HS_BIT_END                       2U

/* Definition for field LL26_VCNUM in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_VCNUM_BIT_START                  3U
#define CFG_DATA_LL26_LL26_VCNUM_BIT_END                    4U

/* Definition for field LL26_FMT in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_FMT_BIT_START                    5U
#define CFG_DATA_LL26_LL26_FMT_BIT_END                      6U

/* Definition for field LL26_FMT_MAP in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL26_LL26_FMT_MAP_BIT_END                  7U

/* Definition for field LL26_FMT_IN in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL26_LL26_FMT_IN_BIT_END                   8U

/* Definition for field LL26_SIZE in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_SIZE_BIT_START                   9U
#define CFG_DATA_LL26_LL26_SIZE_BIT_END                     22U

/* Definition for field LL26_LPHDR_EN in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL26_LL26_LPHDR_EN_BIT_END                 27U

/* Definition for field LL26_CRC_EN in Register CFG_DATA_LL26 */
#define CFG_DATA_LL26_LL26_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL26_LL26_CRC_EN_BIT_END                   28U

/* Definition for field LL26_LPHDR_VAL in Register CFG_DATA_LL26_LPHDR_VAL */
#define CFG_DATA_LL26_LPHDR_VAL_LL26_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL26_LPHDR_VAL_LL26_LPHDR_VAL_BIT_END      31U

/* Definition for field LL26_RD_THRESHOLD in Register CFG_DATA_LL26_THRESHOLD */
#define CFG_DATA_LL26_THRESHOLD_LL26_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL26_THRESHOLD_LL26_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL26_THRESHOLD */
#define CFG_DATA_LL26_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL26_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL26_WR_THRESHOLD in Register CFG_DATA_LL26_THRESHOLD */
#define CFG_DATA_LL26_THRESHOLD_LL26_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL26_THRESHOLD_LL26_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL26_THRESHOLD */
#define CFG_DATA_LL26_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL26_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll26dman in Register CFG_DATA_LL26_THRESHOLD */
#define CFG_DATA_LL26_THRESHOLD_LL26DMAN_BIT_START          16U
#define CFG_DATA_LL26_THRESHOLD_LL26DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL26_THRESHOLD */
#define CFG_DATA_LL26_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL26_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL27_VALID in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_VALID_BIT_START                  0U
#define CFG_DATA_LL27_LL27_VALID_BIT_END                    0U

/* Definition for field LL27_HE in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_HE_BIT_START                     1U
#define CFG_DATA_LL27_LL27_HE_BIT_END                       1U

/* Definition for field LL27_HS in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_HS_BIT_START                     2U
#define CFG_DATA_LL27_LL27_HS_BIT_END                       2U

/* Definition for field LL27_VCNUM in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_VCNUM_BIT_START                  3U
#define CFG_DATA_LL27_LL27_VCNUM_BIT_END                    4U

/* Definition for field LL27_FMT in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_FMT_BIT_START                    5U
#define CFG_DATA_LL27_LL27_FMT_BIT_END                      6U

/* Definition for field LL27_FMT_MAP in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL27_LL27_FMT_MAP_BIT_END                  7U

/* Definition for field LL27_FMT_IN in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL27_LL27_FMT_IN_BIT_END                   8U

/* Definition for field LL27_SIZE in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_SIZE_BIT_START                   9U
#define CFG_DATA_LL27_LL27_SIZE_BIT_END                     22U

/* Definition for field LL27_LPHDR_EN in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL27_LL27_LPHDR_EN_BIT_END                 27U

/* Definition for field LL27_CRC_EN in Register CFG_DATA_LL27 */
#define CFG_DATA_LL27_LL27_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL27_LL27_CRC_EN_BIT_END                   28U

/* Definition for field LL27_LPHDR_VAL in Register CFG_DATA_LL27_LPHDR_VAL */
#define CFG_DATA_LL27_LPHDR_VAL_LL27_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL27_LPHDR_VAL_LL27_LPHDR_VAL_BIT_END      31U

/* Definition for field LL27_RD_THRESHOLD in Register CFG_DATA_LL27_THRESHOLD */
#define CFG_DATA_LL27_THRESHOLD_LL27_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL27_THRESHOLD_LL27_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL27_THRESHOLD */
#define CFG_DATA_LL27_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL27_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL27_WR_THRESHOLD in Register CFG_DATA_LL27_THRESHOLD */
#define CFG_DATA_LL27_THRESHOLD_LL27_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL27_THRESHOLD_LL27_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL27_THRESHOLD */
#define CFG_DATA_LL27_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL27_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll27dman in Register CFG_DATA_LL27_THRESHOLD */
#define CFG_DATA_LL27_THRESHOLD_LL27DMAN_BIT_START          16U
#define CFG_DATA_LL27_THRESHOLD_LL27DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL27_THRESHOLD */
#define CFG_DATA_LL27_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL27_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL28_VALID in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_VALID_BIT_START                  0U
#define CFG_DATA_LL28_LL28_VALID_BIT_END                    0U

/* Definition for field LL28_HE in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_HE_BIT_START                     1U
#define CFG_DATA_LL28_LL28_HE_BIT_END                       1U

/* Definition for field LL28_HS in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_HS_BIT_START                     2U
#define CFG_DATA_LL28_LL28_HS_BIT_END                       2U

/* Definition for field LL28_VCNUM in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_VCNUM_BIT_START                  3U
#define CFG_DATA_LL28_LL28_VCNUM_BIT_END                    4U

/* Definition for field LL28_FMT in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_FMT_BIT_START                    5U
#define CFG_DATA_LL28_LL28_FMT_BIT_END                      6U

/* Definition for field LL28_FMT_MAP in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL28_LL28_FMT_MAP_BIT_END                  7U

/* Definition for field LL28_FMT_IN in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL28_LL28_FMT_IN_BIT_END                   8U

/* Definition for field LL28_SIZE in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_SIZE_BIT_START                   9U
#define CFG_DATA_LL28_LL28_SIZE_BIT_END                     22U

/* Definition for field LL28_LPHDR_EN in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL28_LL28_LPHDR_EN_BIT_END                 27U

/* Definition for field LL28_CRC_EN in Register CFG_DATA_LL28 */
#define CFG_DATA_LL28_LL28_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL28_LL28_CRC_EN_BIT_END                   28U

/* Definition for field LL28_LPHDR_VAL in Register CFG_DATA_LL28_LPHDR_VAL */
#define CFG_DATA_LL28_LPHDR_VAL_LL28_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL28_LPHDR_VAL_LL28_LPHDR_VAL_BIT_END      31U

/* Definition for field LL28_RD_THRESHOLD in Register CFG_DATA_LL28_THRESHOLD */
#define CFG_DATA_LL28_THRESHOLD_LL28_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL28_THRESHOLD_LL28_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL28_THRESHOLD */
#define CFG_DATA_LL28_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL28_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL28_WR_THRESHOLD in Register CFG_DATA_LL28_THRESHOLD */
#define CFG_DATA_LL28_THRESHOLD_LL28_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL28_THRESHOLD_LL28_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL28_THRESHOLD */
#define CFG_DATA_LL28_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL28_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll28dman in Register CFG_DATA_LL28_THRESHOLD */
#define CFG_DATA_LL28_THRESHOLD_LL28DMAN_BIT_START          16U
#define CFG_DATA_LL28_THRESHOLD_LL28DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL28_THRESHOLD */
#define CFG_DATA_LL28_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL28_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL29_VALID in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_VALID_BIT_START                  0U
#define CFG_DATA_LL29_LL29_VALID_BIT_END                    0U

/* Definition for field LL29_HE in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_HE_BIT_START                     1U
#define CFG_DATA_LL29_LL29_HE_BIT_END                       1U

/* Definition for field LL29_HS in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_HS_BIT_START                     2U
#define CFG_DATA_LL29_LL29_HS_BIT_END                       2U

/* Definition for field LL29_VCNUM in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_VCNUM_BIT_START                  3U
#define CFG_DATA_LL29_LL29_VCNUM_BIT_END                    4U

/* Definition for field LL29_FMT in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_FMT_BIT_START                    5U
#define CFG_DATA_LL29_LL29_FMT_BIT_END                      6U

/* Definition for field LL29_FMT_MAP in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL29_LL29_FMT_MAP_BIT_END                  7U

/* Definition for field LL29_FMT_IN in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL29_LL29_FMT_IN_BIT_END                   8U

/* Definition for field LL29_SIZE in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_SIZE_BIT_START                   9U
#define CFG_DATA_LL29_LL29_SIZE_BIT_END                     22U

/* Definition for field LL29_LPHDR_EN in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL29_LL29_LPHDR_EN_BIT_END                 27U

/* Definition for field LL29_CRC_EN in Register CFG_DATA_LL29 */
#define CFG_DATA_LL29_LL29_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL29_LL29_CRC_EN_BIT_END                   28U

/* Definition for field LL29_LPHDR_VAL in Register CFG_DATA_LL29_LPHDR_VAL */
#define CFG_DATA_LL29_LPHDR_VAL_LL29_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL29_LPHDR_VAL_LL29_LPHDR_VAL_BIT_END      31U

/* Definition for field LL29_RD_THRESHOLD in Register CFG_DATA_LL29_THRESHOLD */
#define CFG_DATA_LL29_THRESHOLD_LL29_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL29_THRESHOLD_LL29_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL29_THRESHOLD */
#define CFG_DATA_LL29_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL29_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL29_WR_THRESHOLD in Register CFG_DATA_LL29_THRESHOLD */
#define CFG_DATA_LL29_THRESHOLD_LL29_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL29_THRESHOLD_LL29_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL29_THRESHOLD */
#define CFG_DATA_LL29_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL29_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll29dman in Register CFG_DATA_LL29_THRESHOLD */
#define CFG_DATA_LL29_THRESHOLD_LL29DMAN_BIT_START          16U
#define CFG_DATA_LL29_THRESHOLD_LL29DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL29_THRESHOLD */
#define CFG_DATA_LL29_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL29_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL30_VALID in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_VALID_BIT_START                  0U
#define CFG_DATA_LL30_LL30_VALID_BIT_END                    0U

/* Definition for field LL30_HE in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_HE_BIT_START                     1U
#define CFG_DATA_LL30_LL30_HE_BIT_END                       1U

/* Definition for field LL30_HS in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_HS_BIT_START                     2U
#define CFG_DATA_LL30_LL30_HS_BIT_END                       2U

/* Definition for field LL30_VCNUM in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_VCNUM_BIT_START                  3U
#define CFG_DATA_LL30_LL30_VCNUM_BIT_END                    4U

/* Definition for field LL30_FMT in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_FMT_BIT_START                    5U
#define CFG_DATA_LL30_LL30_FMT_BIT_END                      6U

/* Definition for field LL30_FMT_MAP in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL30_LL30_FMT_MAP_BIT_END                  7U

/* Definition for field LL30_FMT_IN in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL30_LL30_FMT_IN_BIT_END                   8U

/* Definition for field LL30_SIZE in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_SIZE_BIT_START                   9U
#define CFG_DATA_LL30_LL30_SIZE_BIT_END                     22U

/* Definition for field LL30_LPHDR_EN in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL30_LL30_LPHDR_EN_BIT_END                 27U

/* Definition for field LL30_CRC_EN in Register CFG_DATA_LL30 */
#define CFG_DATA_LL30_LL30_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL30_LL30_CRC_EN_BIT_END                   28U

/* Definition for field LL30_LPHDR_VAL in Register CFG_DATA_LL30_LPHDR_VAL */
#define CFG_DATA_LL30_LPHDR_VAL_LL30_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL30_LPHDR_VAL_LL30_LPHDR_VAL_BIT_END      31U

/* Definition for field LL30_RD_THRESHOLD in Register CFG_DATA_LL30_THRESHOLD */
#define CFG_DATA_LL30_THRESHOLD_LL30_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL30_THRESHOLD_LL30_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL30_THRESHOLD */
#define CFG_DATA_LL30_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL30_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL30_WR_THRESHOLD in Register CFG_DATA_LL30_THRESHOLD */
#define CFG_DATA_LL30_THRESHOLD_LL30_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL30_THRESHOLD_LL30_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL30_THRESHOLD */
#define CFG_DATA_LL30_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL30_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll30dman in Register CFG_DATA_LL30_THRESHOLD */
#define CFG_DATA_LL30_THRESHOLD_LL30DMAN_BIT_START          16U
#define CFG_DATA_LL30_THRESHOLD_LL30DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL30_THRESHOLD */
#define CFG_DATA_LL30_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL30_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field LL31_VALID in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_VALID_BIT_START                  0U
#define CFG_DATA_LL31_LL31_VALID_BIT_END                    0U

/* Definition for field LL31_HE in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_HE_BIT_START                     1U
#define CFG_DATA_LL31_LL31_HE_BIT_END                       1U

/* Definition for field LL31_HS in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_HS_BIT_START                     2U
#define CFG_DATA_LL31_LL31_HS_BIT_END                       2U

/* Definition for field LL31_VCNUM in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_VCNUM_BIT_START                  3U
#define CFG_DATA_LL31_LL31_VCNUM_BIT_END                    4U

/* Definition for field LL31_FMT in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_FMT_BIT_START                    5U
#define CFG_DATA_LL31_LL31_FMT_BIT_END                      6U

/* Definition for field LL31_FMT_MAP in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_FMT_MAP_BIT_START                7U
#define CFG_DATA_LL31_LL31_FMT_MAP_BIT_END                  7U

/* Definition for field LL31_FMT_IN in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_FMT_IN_BIT_START                 8U
#define CFG_DATA_LL31_LL31_FMT_IN_BIT_END                   8U

/* Definition for field LL31_SIZE in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_SIZE_BIT_START                   9U
#define CFG_DATA_LL31_LL31_SIZE_BIT_END                     22U

/* Definition for field LL31_LPHDR_EN in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_LPHDR_EN_BIT_START               27U
#define CFG_DATA_LL31_LL31_LPHDR_EN_BIT_END                 27U

/* Definition for field LL31_CRC_EN in Register CFG_DATA_LL31 */
#define CFG_DATA_LL31_LL31_CRC_EN_BIT_START                 28U
#define CFG_DATA_LL31_LL31_CRC_EN_BIT_END                   28U

/* Definition for field LL31_LPHDR_VAL in Register CFG_DATA_LL31_LPHDR_VAL */
#define CFG_DATA_LL31_LPHDR_VAL_LL31_LPHDR_VAL_BIT_START    0U
#define CFG_DATA_LL31_LPHDR_VAL_LL31_LPHDR_VAL_BIT_END      31U

/* Definition for field LL31_RD_THRESHOLD in Register CFG_DATA_LL31_THRESHOLD */
#define CFG_DATA_LL31_THRESHOLD_LL31_RD_THRESHOLD_BIT_START 0U
#define CFG_DATA_LL31_THRESHOLD_LL31_RD_THRESHOLD_BIT_END   6U

/* Definition for field NU1 in Register CFG_DATA_LL31_THRESHOLD */
#define CFG_DATA_LL31_THRESHOLD_NU1_BIT_START               7U
#define CFG_DATA_LL31_THRESHOLD_NU1_BIT_END                 7U

/* Definition for field LL31_WR_THRESHOLD in Register CFG_DATA_LL31_THRESHOLD */
#define CFG_DATA_LL31_THRESHOLD_LL31_WR_THRESHOLD_BIT_START 8U
#define CFG_DATA_LL31_THRESHOLD_LL31_WR_THRESHOLD_BIT_END   14U

/* Definition for field NU2 in Register CFG_DATA_LL31_THRESHOLD */
#define CFG_DATA_LL31_THRESHOLD_NU2_BIT_START               15U
#define CFG_DATA_LL31_THRESHOLD_NU2_BIT_END                 15U

/* Definition for field ll31dman in Register CFG_DATA_LL31_THRESHOLD */
#define CFG_DATA_LL31_THRESHOLD_LL31DMAN_BIT_START          16U
#define CFG_DATA_LL31_THRESHOLD_LL31DMAN_BIT_END            18U

/* Definition for field NU3 in Register CFG_DATA_LL31_THRESHOLD */
#define CFG_DATA_LL31_THRESHOLD_NU3_BIT_START               19U
#define CFG_DATA_LL31_THRESHOLD_NU3_BIT_END                 31U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_A in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_B in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_C in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_D in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_E in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_F in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_G in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_0_H in Register CFG_LVDS_MAPPING_LANE0_FMT_0 */
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE0_FMT_0_CFG_LVDS_MAPPING_LANE0_FMT_0_H_BIT_END 31U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_A in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_B in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_C in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_D in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_E in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_F in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_G in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_0_H in Register CFG_LVDS_MAPPING_LANE1_FMT_0 */
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE1_FMT_0_CFG_LVDS_MAPPING_LANE1_FMT_0_H_BIT_END 31U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_A in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_B in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_C in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_D in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_E in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_F in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_G in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_0_H in Register CFG_LVDS_MAPPING_LANE2_FMT_0 */
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE2_FMT_0_CFG_LVDS_MAPPING_LANE2_FMT_0_H_BIT_END 31U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_A in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_B in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_C in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_D in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_E in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_F in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_G in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_0_H in Register CFG_LVDS_MAPPING_LANE3_FMT_0 */
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE3_FMT_0_CFG_LVDS_MAPPING_LANE3_FMT_0_H_BIT_END 31U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_A in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_B in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_C in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_D in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_E in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_F in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_G in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE0_FMT_1_H in Register CFG_LVDS_MAPPING_LANE0_FMT_1 */
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE0_FMT_1_CFG_LVDS_MAPPING_LANE0_FMT_1_H_BIT_END 31U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_A in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_B in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_C in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_D in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_E in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_F in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_G in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE1_FMT_1_H in Register CFG_LVDS_MAPPING_LANE1_FMT_1 */
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE1_FMT_1_CFG_LVDS_MAPPING_LANE1_FMT_1_H_BIT_END 31U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_A in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_B in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_C in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_D in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_E in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_F in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_G in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE2_FMT_1_H in Register CFG_LVDS_MAPPING_LANE2_FMT_1 */
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE2_FMT_1_CFG_LVDS_MAPPING_LANE2_FMT_1_H_BIT_END 31U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_A in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_A_BIT_START 0U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_A_BIT_END 3U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_B in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_B_BIT_START 4U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_B_BIT_END 7U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_C in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_C_BIT_START 8U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_C_BIT_END 11U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_D in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_D_BIT_START 12U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_D_BIT_END 15U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_E in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_E_BIT_START 16U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_E_BIT_END 19U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_F in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_F_BIT_START 20U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_F_BIT_END 23U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_G in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_G_BIT_START 24U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_G_BIT_END 27U

/* Definition for field CFG_LVDS_MAPPING_LANE3_FMT_1_H in Register CFG_LVDS_MAPPING_LANE3_FMT_1 */
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_H_BIT_START 28U
#define CFG_LVDS_MAPPING_LANE3_FMT_1_CFG_LVDS_MAPPING_LANE3_FMT_1_H_BIT_END 31U

/* Definition for field CFG_LVDS_LANE0_EN in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE0_EN_BIT_START          0U
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE0_EN_BIT_END            0U

/* Definition for field CFG_LVDS_LANE1_EN in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE1_EN_BIT_START          1U
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE1_EN_BIT_END            1U

/* Definition for field CFG_LVDS_LANE2_EN in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE2_EN_BIT_START          2U
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE2_EN_BIT_END            2U

/* Definition for field CFG_LVDS_LANE3_EN in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE3_EN_BIT_START          3U
#define CFG_LVDS_GEN_0_CFG_LVDS_LANE3_EN_BIT_END            3U

/* Definition for field CFG_BIT_CLK_MODE in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CFG_BIT_CLK_MODE_BIT_START           10U
#define CFG_LVDS_GEN_0_CFG_BIT_CLK_MODE_BIT_END             10U

/* Definition for field ccsmen in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CCSMEN_BIT_START                     11U
#define CFG_LVDS_GEN_0_CCSMEN_BIT_END                       11U

/* Definition for field cpossel in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CPOSSEL_BIT_START                    22U
#define CFG_LVDS_GEN_0_CPOSSEL_BIT_END                      22U

/* Definition for field cmsbf in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CMSBF_BIT_START                      23U
#define CFG_LVDS_GEN_0_CMSBF_BIT_END                        23U

/* Definition for field cfdly in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CFDLY_BIT_START                      24U
#define CFG_LVDS_GEN_0_CFDLY_BIT_END                        27U

/* Definition for field cbcrcen in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CBCRCEN_BIT_START                    28U
#define CFG_LVDS_GEN_0_CBCRCEN_BIT_END                      28U

/* Definition for field cpz in Register CFG_LVDS_GEN_0 */
#define CFG_LVDS_GEN_0_CPZ_BIT_START                        30U
#define CFG_LVDS_GEN_0_CPZ_BIT_END                          31U

/* Definition for field c3c3l in Register CFG_LVDS_GEN_1 */
#define CFG_LVDS_GEN_1_C3C3L_BIT_START                      2U
#define CFG_LVDS_GEN_1_C3C3L_BIT_END                        2U

/* Definition for field NU3 in Register CFG_LVDS_GEN_1 */
#define CFG_LVDS_GEN_1_NU3_BIT_START                        3U
#define CFG_LVDS_GEN_1_NU3_BIT_END                          3U

/* Definition for field NU1 in Register CFG_LVDS_GEN_1 */
#define CFG_LVDS_GEN_1_NU1_BIT_START                        7U
#define CFG_LVDS_GEN_1_NU1_BIT_END                          7U

/* Definition for field NU2 in Register CFG_LVDS_GEN_1 */
#define CFG_LVDS_GEN_1_NU2_BIT_START                        19U
#define CFG_LVDS_GEN_1_NU2_BIT_END                          31U

/* Definition for field CFG_LVDS_GEN_2 in Register CFG_LVDS_GEN_2 */
#define CFG_LVDS_GEN_2_CFG_LVDS_GEN_2_BIT_START             0U
#define CFG_LVDS_GEN_2_CFG_LVDS_GEN_2_BIT_END               31U

/* Definition for field CFG_MASK_REG0 in Register CFG_MASK_REG0 */
#define CFG_MASK_REG0_CFG_MASK_REG0_BIT_START               0U
#define CFG_MASK_REG0_CFG_MASK_REG0_BIT_END                 31U

/* Definition for field CFG_MASK_REG1 in Register CFG_MASK_REG1 */
#define CFG_MASK_REG1_CFG_MASK_REG1_BIT_START               0U
#define CFG_MASK_REG1_CFG_MASK_REG1_BIT_END                 31U

/* Definition for field CFG_MASK_REG2 in Register CFG_MASK_REG2 */
#define CFG_MASK_REG2_CFG_MASK_REG2_BIT_START               0U
#define CFG_MASK_REG2_CFG_MASK_REG2_BIT_END                 31U

/* Definition for field CFG_MASK_REG3 in Register CFG_MASK_REG3 */
#define CFG_MASK_REG3_CFG_MASK_REG3_BIT_START               0U
#define CFG_MASK_REG3_CFG_MASK_REG3_BIT_END                 31U

/* Definition for field S_CHIRP_DONE in Register STAT_CBUFF_REG0 */
#define STAT_CBUFF_REG0_S_CHIRP_DONE_BIT_START              11U
#define STAT_CBUFF_REG0_S_CHIRP_DONE_BIT_END                11U

/* Definition for field S_FRAME_DONE in Register STAT_CBUFF_REG0 */
#define STAT_CBUFF_REG0_S_FRAME_DONE_BIT_START              12U
#define STAT_CBUFF_REG0_S_FRAME_DONE_BIT_END                12U

/* Definition for field STAT_CBUFF_REG0_OTHERS in Register STAT_CBUFF_REG0 */
#define STAT_CBUFF_REG0_STAT_CBUFF_REG0_OTHERS_BIT_START    13U
#define STAT_CBUFF_REG0_STAT_CBUFF_REG0_OTHERS_BIT_END      31U

/* Definition for field S1_UNUSED1 in Register STAT_CBUFF_REG1 */
#define STAT_CBUFF_REG1_S1_UNUSED1_BIT_START                3U
#define STAT_CBUFF_REG1_S1_UNUSED1_BIT_END                  7U

/* Definition for field S1_UNUSED2 in Register STAT_CBUFF_REG1 */
#define STAT_CBUFF_REG1_S1_UNUSED2_BIT_START                12U
#define STAT_CBUFF_REG1_S1_UNUSED2_BIT_END                  15U

/* Definition for field S_CHIRP_ERR in Register STAT_CBUFF_REG1 */
#define STAT_CBUFF_REG1_S_CHIRP_ERR_BIT_START               16U
#define STAT_CBUFF_REG1_S_CHIRP_ERR_BIT_END                 16U

/* Definition for field S_FRAME_ERR in Register STAT_CBUFF_REG1 */
#define STAT_CBUFF_REG1_S_FRAME_ERR_BIT_START               17U
#define STAT_CBUFF_REG1_S_FRAME_ERR_BIT_END                 17U

/* Definition for field S1_UNUSED3 in Register STAT_CBUFF_REG1 */
#define STAT_CBUFF_REG1_S1_UNUSED3_BIT_START                21U
#define STAT_CBUFF_REG1_S1_UNUSED3_BIT_END                  31U

/* Definition for field STAT_CBUFF_REG2 in Register STAT_CBUFF_REG2 */
#define STAT_CBUFF_REG2_STAT_CBUFF_REG2_BIT_START           0U
#define STAT_CBUFF_REG2_STAT_CBUFF_REG2_BIT_END             31U

/* Definition for field STAT_CBUFF_REG3 in Register STAT_CBUFF_REG3 */
#define STAT_CBUFF_REG3_STAT_CBUFF_REG3_BIT_START           0U
#define STAT_CBUFF_REG3_STAT_CBUFF_REG3_BIT_END             31U

/* Definition for field C_CHIRP_DONE in Register CLR_CBUFF_REG0 */
#define CLR_CBUFF_REG0_C_CHIRP_DONE_BIT_START               11U
#define CLR_CBUFF_REG0_C_CHIRP_DONE_BIT_END                 11U

/* Definition for field C_FRAME_DONE in Register CLR_CBUFF_REG0 */
#define CLR_CBUFF_REG0_C_FRAME_DONE_BIT_START               12U
#define CLR_CBUFF_REG0_C_FRAME_DONE_BIT_END                 12U

/* Definition for field seccadd in Register STAT_CBUFF_ECC_REG */
#define STAT_CBUFF_ECC_REG_SECCADD_BIT_START                0U
#define STAT_CBUFF_ECC_REG_SECCADD_BIT_END                  5U

/* Definition for field NU1 in Register STAT_CBUFF_ECC_REG */
#define STAT_CBUFF_ECC_REG_NU1_BIT_START                    6U
#define STAT_CBUFF_ECC_REG_NU1_BIT_END                      7U

/* Definition for field seccsbe in Register STAT_CBUFF_ECC_REG */
#define STAT_CBUFF_ECC_REG_SECCSBE_BIT_START                8U
#define STAT_CBUFF_ECC_REG_SECCSBE_BIT_END                  8U

/* Definition for field seccdbe in Register STAT_CBUFF_ECC_REG */
#define STAT_CBUFF_ECC_REG_SECCDBE_BIT_START                9U
#define STAT_CBUFF_ECC_REG_SECCDBE_BIT_END                  9U

/* Definition for field NU2 in Register STAT_CBUFF_ECC_REG */
#define STAT_CBUFF_ECC_REG_NU2_BIT_START                    10U
#define STAT_CBUFF_ECC_REG_NU2_BIT_END                      31U

/* Definition for field NU1 in Register MASK_CBUFF_ECC_REG */
#define MASK_CBUFF_ECC_REG_NU1_BIT_START                    0U
#define MASK_CBUFF_ECC_REG_NU1_BIT_END                      7U

/* Definition for field meccsbe in Register MASK_CBUFF_ECC_REG */
#define MASK_CBUFF_ECC_REG_MECCSBE_BIT_START                8U
#define MASK_CBUFF_ECC_REG_MECCSBE_BIT_END                  8U

/* Definition for field meccdbe in Register MASK_CBUFF_ECC_REG */
#define MASK_CBUFF_ECC_REG_MECCDBE_BIT_START                9U
#define MASK_CBUFF_ECC_REG_MECCDBE_BIT_END                  9U

/* Definition for field NU2 in Register MASK_CBUFF_ECC_REG */
#define MASK_CBUFF_ECC_REG_NU2_BIT_START                    10U
#define MASK_CBUFF_ECC_REG_NU2_BIT_END                      31U

/* Definition for field ceccadd in Register CLR_CBUFF_ECC_REG */
#define CLR_CBUFF_ECC_REG_CECCADD_BIT_START                 0U
#define CLR_CBUFF_ECC_REG_CECCADD_BIT_END                   0U

/* Definition for field NU1 in Register CLR_CBUFF_ECC_REG */
#define CLR_CBUFF_ECC_REG_NU1_BIT_START                     1U
#define CLR_CBUFF_ECC_REG_NU1_BIT_END                       7U

/* Definition for field ceccsbe in Register CLR_CBUFF_ECC_REG */
#define CLR_CBUFF_ECC_REG_CECCSBE_BIT_START                 8U
#define CLR_CBUFF_ECC_REG_CECCSBE_BIT_END                   8U

/* Definition for field ceccdbe in Register CLR_CBUFF_ECC_REG */
#define CLR_CBUFF_ECC_REG_CECCDBE_BIT_START                 9U
#define CLR_CBUFF_ECC_REG_CECCDBE_BIT_END                   9U

/* Definition for field NU2 in Register CLR_CBUFF_ECC_REG */
#define CLR_CBUFF_ECC_REG_NU2_BIT_START                     10U
#define CLR_CBUFF_ECC_REG_NU2_BIT_END                       31U

/* Definition for field SAF_CRC in Register STAT_SAFETY */
#define STAT_SAFETY_SAF_CRC_BIT_START                       0U
#define STAT_SAFETY_SAF_CRC_BIT_END                         7U

/* Definition for field SAF_CHIRP_ERR in Register STAT_SAFETY */
#define STAT_SAFETY_SAF_CHIRP_ERR_BIT_START                 8U
#define STAT_SAFETY_SAF_CHIRP_ERR_BIT_END                   8U

/* Definition for field SAF_UNUSED1 in Register STAT_SAFETY */
#define STAT_SAFETY_SAF_UNUSED1_BIT_START                   9U
#define STAT_SAFETY_SAF_UNUSED1_BIT_END                     31U

/* Definition for field MASK_SAFETY in Register MASK_SAFETY */
#define MASK_SAFETY_MASK_SAFETY_BIT_START                   0U
#define MASK_SAFETY_MASK_SAFETY_BIT_END                     31U

/* Definition for field CLR_SAFETY in Register CLR_SAFETY */
#define CLR_SAFETY_CLR_SAFETY_BIT_START                     0U
#define CLR_SAFETY_CLR_SAFETY_BIT_END                       31U

/**
 * @brief
 *  CBUFF Linked List Entry
 *
 * @details
 *  The structure describes the Linked List Entry in the CBUFF register space
 */
typedef struct DSSCBUFF_LL_t
{
    uint32_t    CFG_DATA_LL;
    uint32_t    CFG_DATA_LL_LPHDR_VAL;
    uint32_t    CFG_DATA_LL_THRESHOLD;
}DSSCBUFF_LL;

/**
 * @struct DSSCBUFFRegs_t
 * @brief
 *   Module DSS_CBUFF Register Definition
 * @details
 *   This structure is used to access the DSS_CBUFF module registers.
 */
/**
 * @typedef DSSCBUFFRegs
 * @brief
 *   Module DSS_CBUFF Register Frame type Definition
 * @details
 *   This type is used to access the DSS_CBUFF module registers.
 */
typedef volatile struct DSSCBUFFRegs_t
{
    uint32_t    CONFIG_REG_0                       ;        /* Offset = 0x000 */
    uint32_t    CFG_SPHDR_ADDRESS                  ;        /* Offset = 0x004 */
    uint32_t    CFG_CMD_HSVAL                      ;        /* Offset = 0x008 */
    uint32_t    CFG_CMD_HEVAL                      ;        /* Offset = 0x00C */
    uint32_t    CFG_CMD_VSVAL                      ;        /* Offset = 0x010 */
    uint32_t    CFG_CMD_VEVAL                      ;        /* Offset = 0x014 */
    uint32_t    CFG_LPHDR_ADDRESS                  ;        /* Offset = 0x018 */
    uint32_t    RESTRICTED1                        ;        /* Offset = 0x01C */
    uint32_t    CFG_CHIRPS_PER_FRAME               ;        /* Offset = 0x020 */
    uint32_t    CFG_FIFO_FREE_THRESHOLD            ;        /* Offset = 0x024 */
    uint32_t    CFG_LPPYLD_ADDRESS                 ;        /* Offset = 0x028 */
    uint32_t    CFG_DELAY_CONFIG                   ;        /* Offset = 0x02C */

    /* SOFTWARE Modification: Making an array would make the driver easier to
     * code and save on memory. */
#if 0
    uint32_t    CFG_DATA_LL0                       ;        /* Offset = 0x030 */
    uint32_t    CFG_DATA_LL0_LPHDR_VAL             ;        /* Offset = 0x034 */
    uint32_t    CFG_DATA_LL0_THRESHOLD             ;        /* Offset = 0x038 */
    uint32_t    CFG_DATA_LL1                       ;        /* Offset = 0x03C */
    uint32_t    CFG_DATA_LL1_LPHDR_VAL             ;        /* Offset = 0x040 */
    uint32_t    CFG_DATA_LL1_THRESHOLD             ;        /* Offset = 0x044 */
    uint32_t    CFG_DATA_LL2                       ;        /* Offset = 0x048 */
    uint32_t    CFG_DATA_LL2_LPHDR_VAL             ;        /* Offset = 0x04C */
    uint32_t    CFG_DATA_LL2_THRESHOLD             ;        /* Offset = 0x050 */
    uint32_t    CFG_DATA_LL3                       ;        /* Offset = 0x054 */
    uint32_t    CFG_DATA_LL3_LPHDR_VAL             ;        /* Offset = 0x058 */
    uint32_t    CFG_DATA_LL3_THRESHOLD             ;        /* Offset = 0x05C */
    uint32_t    CFG_DATA_LL4                       ;        /* Offset = 0x060 */
    uint32_t    CFG_DATA_LL4_LPHDR_VAL             ;        /* Offset = 0x064 */
    uint32_t    CFG_DATA_LL4_THRESHOLD             ;        /* Offset = 0x068 */
    uint32_t    CFG_DATA_LL5                       ;        /* Offset = 0x06C */
    uint32_t    CFG_DATA_LL5_LPHDR_VAL             ;        /* Offset = 0x070 */
    uint32_t    CFG_DATA_LL5_THRESHOLD             ;        /* Offset = 0x074 */
    uint32_t    CFG_DATA_LL6                       ;        /* Offset = 0x078 */
    uint32_t    CFG_DATA_LL6_LPHDR_VAL             ;        /* Offset = 0x07C */
    uint32_t    CFG_DATA_LL6_THRESHOLD             ;        /* Offset = 0x080 */
    uint32_t    CFG_DATA_LL7                       ;        /* Offset = 0x084 */
    uint32_t    CFG_DATA_LL7_LPHDR_VAL             ;        /* Offset = 0x088 */
    uint32_t    CFG_DATA_LL7_THRESHOLD             ;        /* Offset = 0x08C */
    uint32_t    CFG_DATA_LL8                       ;        /* Offset = 0x090 */
    uint32_t    CFG_DATA_LL8_LPHDR_VAL             ;        /* Offset = 0x094 */
    uint32_t    CFG_DATA_LL8_THRESHOLD             ;        /* Offset = 0x098 */
    uint32_t    CFG_DATA_LL9                       ;        /* Offset = 0x09C */
    uint32_t    CFG_DATA_LL9_LPHDR_VAL             ;        /* Offset = 0x0A0 */
    uint32_t    CFG_DATA_LL9_THRESHOLD             ;        /* Offset = 0x0A4 */
    uint32_t    CFG_DATA_LL10                      ;        /* Offset = 0x0A8 */
    uint32_t    CFG_DATA_LL10_LPHDR_VAL            ;        /* Offset = 0x0AC */
    uint32_t    CFG_DATA_LL10_THRESHOLD            ;        /* Offset = 0x0B0 */
    uint32_t    CFG_DATA_LL11                      ;        /* Offset = 0x0B4 */
    uint32_t    CFG_DATA_LL11_LPHDR_VAL            ;        /* Offset = 0x0B8 */
    uint32_t    CFG_DATA_LL11_THRESHOLD            ;        /* Offset = 0x0BC */
    uint32_t    CFG_DATA_LL12                      ;        /* Offset = 0x0C0 */
    uint32_t    CFG_DATA_LL12_LPHDR_VAL            ;        /* Offset = 0x0C4 */
    uint32_t    CFG_DATA_LL12_THRESHOLD            ;        /* Offset = 0x0C8 */
    uint32_t    CFG_DATA_LL13                      ;        /* Offset = 0x0CC */
    uint32_t    CFG_DATA_LL13_LPHDR_VAL            ;        /* Offset = 0x0D0 */
    uint32_t    CFG_DATA_LL13_THRESHOLD            ;        /* Offset = 0x0D4 */
    uint32_t    CFG_DATA_LL14                      ;        /* Offset = 0x0D8 */
    uint32_t    CFG_DATA_LL14_LPHDR_VAL            ;        /* Offset = 0x0DC */
    uint32_t    CFG_DATA_LL14_THRESHOLD            ;        /* Offset = 0x0E0 */
    uint32_t    CFG_DATA_LL15                      ;        /* Offset = 0x0E4 */
    uint32_t    CFG_DATA_LL15_LPHDR_VAL            ;        /* Offset = 0x0E8 */
    uint32_t    CFG_DATA_LL15_THRESHOLD            ;        /* Offset = 0x0EC */
    uint32_t    CFG_DATA_LL16                      ;        /* Offset = 0x0F0 */
    uint32_t    CFG_DATA_LL16_LPHDR_VAL            ;        /* Offset = 0x0F4 */
    uint32_t    CFG_DATA_LL16_THRESHOLD            ;        /* Offset = 0x0F8 */
    uint32_t    CFG_DATA_LL17                      ;        /* Offset = 0x0FC */
    uint32_t    CFG_DATA_LL17_LPHDR_VAL            ;        /* Offset = 0x100 */
    uint32_t    CFG_DATA_LL17_THRESHOLD            ;        /* Offset = 0x104 */
    uint32_t    CFG_DATA_LL18                      ;        /* Offset = 0x108 */
    uint32_t    CFG_DATA_LL18_LPHDR_VAL            ;        /* Offset = 0x10C */
    uint32_t    CFG_DATA_LL18_THRESHOLD            ;        /* Offset = 0x110 */
    uint32_t    CFG_DATA_LL19                      ;        /* Offset = 0x114 */
    uint32_t    CFG_DATA_LL19_LPHDR_VAL            ;        /* Offset = 0x118 */
    uint32_t    CFG_DATA_LL19_THRESHOLD            ;        /* Offset = 0x11C */
    uint32_t    CFG_DATA_LL20                      ;        /* Offset = 0x120 */
    uint32_t    CFG_DATA_LL20_LPHDR_VAL            ;        /* Offset = 0x124 */
    uint32_t    CFG_DATA_LL20_THRESHOLD            ;        /* Offset = 0x128 */
    uint32_t    CFG_DATA_LL21                      ;        /* Offset = 0x12C */
    uint32_t    CFG_DATA_LL21_LPHDR_VAL            ;        /* Offset = 0x130 */
    uint32_t    CFG_DATA_LL21_THRESHOLD            ;        /* Offset = 0x134 */
    uint32_t    CFG_DATA_LL22                      ;        /* Offset = 0x138 */
    uint32_t    CFG_DATA_LL22_LPHDR_VAL            ;        /* Offset = 0x13C */
    uint32_t    CFG_DATA_LL22_THRESHOLD            ;        /* Offset = 0x140 */
    uint32_t    CFG_DATA_LL23                      ;        /* Offset = 0x144 */
    uint32_t    CFG_DATA_LL23_LPHDR_VAL            ;        /* Offset = 0x148 */
    uint32_t    CFG_DATA_LL23_THRESHOLD            ;        /* Offset = 0x14C */
    uint32_t    CFG_DATA_LL24                      ;        /* Offset = 0x150 */
    uint32_t    CFG_DATA_LL24_LPHDR_VAL            ;        /* Offset = 0x154 */
    uint32_t    CFG_DATA_LL24_THRESHOLD            ;        /* Offset = 0x158 */
    uint32_t    CFG_DATA_LL25                      ;        /* Offset = 0x15C */
    uint32_t    CFG_DATA_LL25_LPHDR_VAL            ;        /* Offset = 0x160 */
    uint32_t    CFG_DATA_LL25_THRESHOLD            ;        /* Offset = 0x164 */
    uint32_t    CFG_DATA_LL26                      ;        /* Offset = 0x168 */
    uint32_t    CFG_DATA_LL26_LPHDR_VAL            ;        /* Offset = 0x16C */
    uint32_t    CFG_DATA_LL26_THRESHOLD            ;        /* Offset = 0x170 */
    uint32_t    CFG_DATA_LL27                      ;        /* Offset = 0x174 */
    uint32_t    CFG_DATA_LL27_LPHDR_VAL            ;        /* Offset = 0x178 */
    uint32_t    CFG_DATA_LL27_THRESHOLD            ;        /* Offset = 0x17C */
    uint32_t    CFG_DATA_LL28                      ;        /* Offset = 0x180 */
    uint32_t    CFG_DATA_LL28_LPHDR_VAL            ;        /* Offset = 0x184 */
    uint32_t    CFG_DATA_LL28_THRESHOLD            ;        /* Offset = 0x188 */
    uint32_t    CFG_DATA_LL29                      ;        /* Offset = 0x18C */
    uint32_t    CFG_DATA_LL29_LPHDR_VAL            ;        /* Offset = 0x190 */
    uint32_t    CFG_DATA_LL29_THRESHOLD            ;        /* Offset = 0x194 */
    uint32_t    CFG_DATA_LL30                      ;        /* Offset = 0x198 */
    uint32_t    CFG_DATA_LL30_LPHDR_VAL            ;        /* Offset = 0x19C */
    uint32_t    CFG_DATA_LL30_THRESHOLD            ;        /* Offset = 0x1A0 */
    uint32_t    CFG_DATA_LL31                      ;        /* Offset = 0x1A4 */
    uint32_t    CFG_DATA_LL31_LPHDR_VAL            ;        /* Offset = 0x1A8 */
    uint32_t    CFG_DATA_LL31_THRESHOLD            ;        /* Offset = 0x1AC */
#else
    DSSCBUFF_LL LL_ENTRY[32];
#endif

    /* SOFTWARE Modification: Making an array would make the driver easier to
     * code and save on memory. */
#if 0
    uint32_t    CFG_LVDS_MAPPING_LANE0_FMT_0       ;        /* Offset = 0x1B0 */
    uint32_t    CFG_LVDS_MAPPING_LANE1_FMT_0       ;        /* Offset = 0x1B4 */
    uint32_t    CFG_LVDS_MAPPING_LANE2_FMT_0       ;        /* Offset = 0x1B8 */
    uint32_t    CFG_LVDS_MAPPING_LANE3_FMT_0       ;        /* Offset = 0x1BC */
    uint32_t    CFG_LVDS_MAPPING_LANE0_FMT_1       ;        /* Offset = 0x1C0 */
    uint32_t    CFG_LVDS_MAPPING_LANE1_FMT_1       ;        /* Offset = 0x1C4 */
    uint32_t    CFG_LVDS_MAPPING_LANE2_FMT_1       ;        /* Offset = 0x1C8 */
    uint32_t    CFG_LVDS_MAPPING_LANE3_FMT_1       ;        /* Offset = 0x1CC */
#else
    uint32_t    CFG_LVDS_MAPPING_LANE_FMT[8];
#endif
    uint32_t    CFG_LVDS_GEN_0                     ;        /* Offset = 0x1D0 */
    uint32_t    CFG_LVDS_GEN_1                     ;        /* Offset = 0x1D4 */
    uint32_t    CFG_LVDS_GEN_2                     ;        /* Offset = 0x1D8 */
    uint32_t    CFG_MASK_REG0                      ;        /* Offset = 0x1DC */
    uint32_t    CFG_MASK_REG1                      ;        /* Offset = 0x1E0 */
    uint32_t    CFG_MASK_REG2                      ;        /* Offset = 0x1E4 */
    uint32_t    CFG_MASK_REG3                      ;        /* Offset = 0x1E8 */
    uint32_t    STAT_CBUFF_REG0                    ;        /* Offset = 0x1EC */
    uint32_t    STAT_CBUFF_REG1                    ;        /* Offset = 0x1F0 */
    uint32_t    STAT_CBUFF_REG2                    ;        /* Offset = 0x1F4 */
    uint32_t    STAT_CBUFF_REG3                    ;        /* Offset = 0x1F8 */
    uint32_t    RESTRICTED2[4]                     ;        /* Offset = 0x1FC */
    uint32_t    CLR_CBUFF_REG0                     ;        /* Offset = 0x20C */
    uint32_t    RESTRICTED3[3]                     ;        /* Offset = 0x210 */
    uint32_t    STAT_CBUFF_ECC_REG                 ;        /* Offset = 0x21C */
    uint32_t    MASK_CBUFF_ECC_REG                 ;        /* Offset = 0x220 */
    uint32_t    CLR_CBUFF_ECC_REG                  ;        /* Offset = 0x224 */
    uint32_t    STAT_SAFETY                        ;        /* Offset = 0x228 */
    uint32_t    MASK_SAFETY                        ;        /* Offset = 0x22C */
    uint32_t    CLR_SAFETY                         ;        /* Offset = 0x230 */
} DSSCBUFFRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_DSSCBUFF_H */
/* END OF REG_DSSCBUFF_H */

