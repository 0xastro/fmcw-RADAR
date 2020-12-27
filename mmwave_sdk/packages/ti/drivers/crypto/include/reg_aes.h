/**
 *  @file    reg_aes.h
 *
 *  @brief
 *    This file gives register definitions of MSS_AES module.
 *
 *  This file is auto-generated on 11/2/2017.
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

#ifndef REG_AES_H
#define REG_AES_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field KEY in Register S_KEY2_6 */
#define S_KEY2_6_KEY_BIT_START                              0U
#define S_KEY2_6_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY2_7 */
#define S_KEY2_7_KEY_BIT_START                              0U
#define S_KEY2_7_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY2_4 */
#define S_KEY2_4_KEY_BIT_START                              0U
#define S_KEY2_4_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY2_5 */
#define S_KEY2_5_KEY_BIT_START                              0U
#define S_KEY2_5_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY2_2 */
#define S_KEY2_2_KEY_BIT_START                              0U
#define S_KEY2_2_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY2_3 */
#define S_KEY2_3_KEY_BIT_START                              0U
#define S_KEY2_3_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY2_0 */
#define S_KEY2_0_KEY_BIT_START                              0U
#define S_KEY2_0_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY2_1 */
#define S_KEY2_1_KEY_BIT_START                              0U
#define S_KEY2_1_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_6 */
#define S_KEY1_6_KEY_BIT_START                              0U
#define S_KEY1_6_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_7 */
#define S_KEY1_7_KEY_BIT_START                              0U
#define S_KEY1_7_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_4 */
#define S_KEY1_4_KEY_BIT_START                              0U
#define S_KEY1_4_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_5 */
#define S_KEY1_5_KEY_BIT_START                              0U
#define S_KEY1_5_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_2 */
#define S_KEY1_2_KEY_BIT_START                              0U
#define S_KEY1_2_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_3 */
#define S_KEY1_3_KEY_BIT_START                              0U
#define S_KEY1_3_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_0 */
#define S_KEY1_0_KEY_BIT_START                              0U
#define S_KEY1_0_KEY_BIT_END                                31U

/* Definition for field KEY in Register S_KEY1_1 */
#define S_KEY1_1_KEY_BIT_START                              0U
#define S_KEY1_1_KEY_BIT_END                                31U

/* Definition for field DATA in Register S_IV_IN_0 */
#define S_IV_IN_0_DATA_BIT_START                            0U
#define S_IV_IN_0_DATA_BIT_END                              31U

/* Definition for field DATA in Register S_IV_IN_1 */
#define S_IV_IN_1_DATA_BIT_START                            0U
#define S_IV_IN_1_DATA_BIT_END                              31U

/* Definition for field DATA in Register S_IV_IN_2 */
#define S_IV_IN_2_DATA_BIT_START                            0U
#define S_IV_IN_2_DATA_BIT_END                              31U

/* Definition for field DATA in Register S_IV_IN_3 */
#define S_IV_IN_3_DATA_BIT_START                            0U
#define S_IV_IN_3_DATA_BIT_END                              31U

/* Definition for field OUTPUT_READY in Register S_CTRL */
#define S_CTRL_OUTPUT_READY_BIT_START                       0U
#define S_CTRL_OUTPUT_READY_BIT_END                         0U

/* Definition for field INPUT_READY in Register S_CTRL */
#define S_CTRL_INPUT_READY_BIT_START                        1U
#define S_CTRL_INPUT_READY_BIT_END                          1U

/* Definition for field DIRECTION in Register S_CTRL */
#define S_CTRL_DIRECTION_BIT_START                          2U
#define S_CTRL_DIRECTION_BIT_END                            2U

/* Definition for field KEY_SIZE in Register S_CTRL */
#define S_CTRL_KEY_SIZE_BIT_START                           3U
#define S_CTRL_KEY_SIZE_BIT_END                             4U

/* Definition for field MODE in Register S_CTRL */
#define S_CTRL_MODE_BIT_START                               5U
#define S_CTRL_MODE_BIT_END                                 5U

/* Definition for field CTR in Register S_CTRL */
#define S_CTRL_CTR_BIT_START                                6U
#define S_CTRL_CTR_BIT_END                                  6U

/* Definition for field CTR_WIDTH in Register S_CTRL */
#define S_CTRL_CTR_WIDTH_BIT_START                          7U
#define S_CTRL_CTR_WIDTH_BIT_END                            8U

/* Definition for field ICM in Register S_CTRL */
#define S_CTRL_ICM_BIT_START                                9U
#define S_CTRL_ICM_BIT_END                                  9U

/* Definition for field CFB in Register S_CTRL */
#define S_CTRL_CFB_BIT_START                                10U
#define S_CTRL_CFB_BIT_END                                  10U

/* Definition for field XTS in Register S_CTRL */
#define S_CTRL_XTS_BIT_START                                11U
#define S_CTRL_XTS_BIT_END                                  12U

/* Definition for field F8 in Register S_CTRL */
#define S_CTRL_F8_BIT_START                                 13U
#define S_CTRL_F8_BIT_END                                   13U

/* Definition for field F9 in Register S_CTRL */
#define S_CTRL_F9_BIT_START                                 14U
#define S_CTRL_F9_BIT_END                                   14U

/* Definition for field CBCMAC in Register S_CTRL */
#define S_CTRL_CBCMAC_BIT_START                             15U
#define S_CTRL_CBCMAC_BIT_END                               15U

/* Definition for field GCM in Register S_CTRL */
#define S_CTRL_GCM_BIT_START                                16U
#define S_CTRL_GCM_BIT_END                                  17U

/* Definition for field CCM in Register S_CTRL */
#define S_CTRL_CCM_BIT_START                                18U
#define S_CTRL_CCM_BIT_END                                  18U

/* Definition for field CCM_L in Register S_CTRL */
#define S_CTRL_CCM_L_BIT_START                              19U
#define S_CTRL_CCM_L_BIT_END                                21U

/* Definition for field CCM_M in Register S_CTRL */
#define S_CTRL_CCM_M_BIT_START                              22U
#define S_CTRL_CCM_M_BIT_END                                24U

/* Definition for field SAVE_CONTEXT in Register S_CTRL */
#define S_CTRL_SAVE_CONTEXT_BIT_START                       29U
#define S_CTRL_SAVE_CONTEXT_BIT_END                         29U

/* Definition for field SAVE_CONTEXT_READY in Register S_CTRL */
#define S_CTRL_SAVE_CONTEXT_READY_BIT_START                 30U
#define S_CTRL_SAVE_CONTEXT_READY_BIT_END                   30U

/* Definition for field CONTEXT_READY in Register S_CTRL */
#define S_CTRL_CONTEXT_READY_BIT_START                      31U
#define S_CTRL_CONTEXT_READY_BIT_END                        31U

/* Definition for field LENGTH in Register S_C_LENGTH_1 */
#define S_C_LENGTH_1_LENGTH_BIT_START                       0U
#define S_C_LENGTH_1_LENGTH_BIT_END                         28U

/* Definition for field AUTH in Register S_AUTH_LENGTH */
#define S_AUTH_LENGTH_AUTH_BIT_START                        0U
#define S_AUTH_LENGTH_AUTH_BIT_END                          31U

/* Definition for field DATA in Register S_DATA_IN_0 */
#define S_DATA_IN_0_DATA_BIT_START                          0U
#define S_DATA_IN_0_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_DATA_IN_1 */
#define S_DATA_IN_1_DATA_BIT_START                          0U
#define S_DATA_IN_1_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_DATA_IN_2 */
#define S_DATA_IN_2_DATA_BIT_START                          0U
#define S_DATA_IN_2_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_DATA_IN_3 */
#define S_DATA_IN_3_DATA_BIT_START                          0U
#define S_DATA_IN_3_DATA_BIT_END                            31U

/* Definition for field HASH in Register S_TAG_OUT_0 */
#define S_TAG_OUT_0_HASH_BIT_START                          0U
#define S_TAG_OUT_0_HASH_BIT_END                            31U

/* Definition for field HASH in Register S_TAG_OUT_1 */
#define S_TAG_OUT_1_HASH_BIT_START                          0U
#define S_TAG_OUT_1_HASH_BIT_END                            31U

/* Definition for field HASH in Register S_TAG_OUT_2 */
#define S_TAG_OUT_2_HASH_BIT_START                          0U
#define S_TAG_OUT_2_HASH_BIT_END                            31U

/* Definition for field HASH in Register S_TAG_OUT_3 */
#define S_TAG_OUT_3_HASH_BIT_START                          0U
#define S_TAG_OUT_3_HASH_BIT_END                            31U

/* Definition for field Y_MINOR in Register S_REVISION */
#define S_REVISION_Y_MINOR_BIT_START                        0U
#define S_REVISION_Y_MINOR_BIT_END                          5U

/* Definition for field CUSTOM in Register S_REVISION */
#define S_REVISION_CUSTOM_BIT_START                         6U
#define S_REVISION_CUSTOM_BIT_END                           7U

/* Definition for field X_MAJOR in Register S_REVISION */
#define S_REVISION_X_MAJOR_BIT_START                        8U
#define S_REVISION_X_MAJOR_BIT_END                          10U

/* Definition for field R_RTL in Register S_REVISION */
#define S_REVISION_R_RTL_BIT_START                          11U
#define S_REVISION_R_RTL_BIT_END                            15U

/* Definition for field FUNC in Register S_REVISION */
#define S_REVISION_FUNC_BIT_START                           16U
#define S_REVISION_FUNC_BIT_END                             27U

/* Definition for field SCHEME in Register S_REVISION */
#define S_REVISION_SCHEME_BIT_START                         30U
#define S_REVISION_SCHEME_BIT_END                           31U

/* Definition for field AUTOIDLE in Register S_SYSCONFIG */
#define S_SYSCONFIG_AUTOIDLE_BIT_START                      0U
#define S_SYSCONFIG_AUTOIDLE_BIT_END                        0U

/* Definition for field SOFTRESET in Register S_SYSCONFIG */
#define S_SYSCONFIG_SOFTRESET_BIT_START                     1U
#define S_SYSCONFIG_SOFTRESET_BIT_END                       1U

/* Definition for field SIDLE in Register S_SYSCONFIG */
#define S_SYSCONFIG_SIDLE_BIT_START                         2U
#define S_SYSCONFIG_SIDLE_BIT_END                           3U

/* Definition for field DIRECTBUSEN in Register S_SYSCONFIG */
#define S_SYSCONFIG_DIRECTBUSEN_BIT_START                   4U
#define S_SYSCONFIG_DIRECTBUSEN_BIT_END                     4U

/* Definition for field DMA_REQ_DATA_IN_EN in Register S_SYSCONFIG */
#define S_SYSCONFIG_DMA_REQ_DATA_IN_EN_BIT_START            5U
#define S_SYSCONFIG_DMA_REQ_DATA_IN_EN_BIT_END              5U

/* Definition for field DMA_REQ_DATA_OUT_EN in Register S_SYSCONFIG */
#define S_SYSCONFIG_DMA_REQ_DATA_OUT_EN_BIT_START           6U
#define S_SYSCONFIG_DMA_REQ_DATA_OUT_EN_BIT_END             6U

/* Definition for field DMA_REQ_CONTEXT_IN_EN in Register S_SYSCONFIG */
#define S_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_BIT_START         7U
#define S_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_BIT_END           7U

/* Definition for field DMA_REQ_CONTEXT_OUT_EN in Register S_SYSCONFIG */
#define S_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_BIT_START        8U
#define S_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_BIT_END          8U

/* Definition for field MAP_CONTEXT_OUT_ON_DATA_OUT in Register S_SYSCONFIG */
#define S_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_BIT_START   9U
#define S_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_BIT_END     9U

/* Definition for field KEK_MODE in Register S_SYSCONFIG */
#define S_SYSCONFIG_KEK_MODE_BIT_START                      10U
#define S_SYSCONFIG_KEK_MODE_BIT_END                        10U

/* Definition for field KEY_ENC in Register S_SYSCONFIG */
#define S_SYSCONFIG_KEY_ENC_BIT_START                       11U
#define S_SYSCONFIG_KEY_ENC_BIT_END                         11U

/* Definition for field K3 in Register S_SYSCONFIG */
#define S_SYSCONFIG_K3_BIT_START                            12U
#define S_SYSCONFIG_K3_BIT_END                              12U

/* Definition for field RESETDONE in Register S_SYSSTATUS */
#define S_SYSSTATUS_RESETDONE_BIT_START                     0U
#define S_SYSSTATUS_RESETDONE_BIT_END                       0U

/* Definition for field CONTEX_IN in Register S_IRQSTATUS */
#define S_IRQSTATUS_CONTEX_IN_BIT_START                     0U
#define S_IRQSTATUS_CONTEX_IN_BIT_END                       0U

/* Definition for field DATA_IN in Register S_IRQSTATUS */
#define S_IRQSTATUS_DATA_IN_BIT_START                       1U
#define S_IRQSTATUS_DATA_IN_BIT_END                         1U

/* Definition for field DATA_OUT in Register S_IRQSTATUS */
#define S_IRQSTATUS_DATA_OUT_BIT_START                      2U
#define S_IRQSTATUS_DATA_OUT_BIT_END                        2U

/* Definition for field CONTEXT_OUT in Register S_IRQSTATUS */
#define S_IRQSTATUS_CONTEXT_OUT_BIT_START                   3U
#define S_IRQSTATUS_CONTEXT_OUT_BIT_END                     3U

/* Definition for field CONTEX_IN in Register S_IRQENABLE */
#define S_IRQENABLE_CONTEX_IN_BIT_START                     0U
#define S_IRQENABLE_CONTEX_IN_BIT_END                       0U

/* Definition for field DATA_IN in Register S_IRQENABLE */
#define S_IRQENABLE_DATA_IN_BIT_START                       1U
#define S_IRQENABLE_DATA_IN_BIT_END                         1U

/* Definition for field DATA_OUT in Register S_IRQENABLE */
#define S_IRQENABLE_DATA_OUT_BIT_START                      2U
#define S_IRQENABLE_DATA_OUT_BIT_END                        2U

/* Definition for field CONTEXT_OUT in Register S_IRQENABLE */
#define S_IRQENABLE_CONTEXT_OUT_BIT_START                   3U
#define S_IRQENABLE_CONTEXT_OUT_BIT_END                     3U

/* Definition for field S_ACCESS in Register S_DIRTYBITS */
#define S_DIRTYBITS_S_ACCESS_BIT_START                      0U
#define S_DIRTYBITS_S_ACCESS_BIT_END                        0U

/* Definition for field S_DIRTY in Register S_DIRTYBITS */
#define S_DIRTYBITS_S_DIRTY_BIT_START                       1U
#define S_DIRTYBITS_S_DIRTY_BIT_END                         1U

/* Definition for field P_ACCESS in Register S_DIRTYBITS */
#define S_DIRTYBITS_P_ACCESS_BIT_START                      2U
#define S_DIRTYBITS_P_ACCESS_BIT_END                        2U

/* Definition for field P_DIRTY in Register S_DIRTYBITS */
#define S_DIRTYBITS_P_DIRTY_BIT_START                       3U
#define S_DIRTYBITS_P_DIRTY_BIT_END                         3U

/* Definition for field KEY_LOCK in Register S_LOCKDOWN */
#define S_LOCKDOWN_KEY_LOCK_BIT_START                       0U
#define S_LOCKDOWN_KEY_LOCK_BIT_END                         0U

/* Definition for field KEY2_LOCK in Register S_LOCKDOWN */
#define S_LOCKDOWN_KEY2_LOCK_BIT_START                      1U
#define S_LOCKDOWN_KEY2_LOCK_BIT_END                        1U

/* Definition for field KEY3_LOCK in Register S_LOCKDOWN */
#define S_LOCKDOWN_KEY3_LOCK_BIT_START                      2U
#define S_LOCKDOWN_KEY3_LOCK_BIT_END                        2U

/* Definition for field IV_LOCK in Register S_LOCKDOWN */
#define S_LOCKDOWN_IV_LOCK_BIT_START                        3U
#define S_LOCKDOWN_IV_LOCK_BIT_END                          3U

/* Definition for field CONTROL_LOCK in Register S_LOCKDOWN */
#define S_LOCKDOWN_CONTROL_LOCK_BIT_START                   4U
#define S_LOCKDOWN_CONTROL_LOCK_BIT_END                     4U

/* Definition for field LENGTH_LOCK in Register S_LOCKDOWN */
#define S_LOCKDOWN_LENGTH_LOCK_BIT_START                    5U
#define S_LOCKDOWN_LENGTH_LOCK_BIT_END                      5U

/* Definition for field KEY in Register P_KEY2_6 */
#define P_KEY2_6_KEY_BIT_START                              0U
#define P_KEY2_6_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY2_7 */
#define P_KEY2_7_KEY_BIT_START                              0U
#define P_KEY2_7_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY2_4 */
#define P_KEY2_4_KEY_BIT_START                              0U
#define P_KEY2_4_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY2_5 */
#define P_KEY2_5_KEY_BIT_START                              0U
#define P_KEY2_5_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY2_2 */
#define P_KEY2_2_KEY_BIT_START                              0U
#define P_KEY2_2_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY2_3 */
#define P_KEY2_3_KEY_BIT_START                              0U
#define P_KEY2_3_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY2_0 */
#define P_KEY2_0_KEY_BIT_START                              0U
#define P_KEY2_0_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY2_1 */
#define P_KEY2_1_KEY_BIT_START                              0U
#define P_KEY2_1_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_6 */
#define P_KEY1_6_KEY_BIT_START                              0U
#define P_KEY1_6_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_7 */
#define P_KEY1_7_KEY_BIT_START                              0U
#define P_KEY1_7_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_4 */
#define P_KEY1_4_KEY_BIT_START                              0U
#define P_KEY1_4_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_5 */
#define P_KEY1_5_KEY_BIT_START                              0U
#define P_KEY1_5_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_2 */
#define P_KEY1_2_KEY_BIT_START                              0U
#define P_KEY1_2_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_3 */
#define P_KEY1_3_KEY_BIT_START                              0U
#define P_KEY1_3_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_0 */
#define P_KEY1_0_KEY_BIT_START                              0U
#define P_KEY1_0_KEY_BIT_END                                31U

/* Definition for field KEY in Register P_KEY1_1 */
#define P_KEY1_1_KEY_BIT_START                              0U
#define P_KEY1_1_KEY_BIT_END                                31U

/* Definition for field DATA in Register P_IV_IN_0 */
#define P_IV_IN_0_DATA_BIT_START                            0U
#define P_IV_IN_0_DATA_BIT_END                              31U

/* Definition for field DATA in Register P_IV_IN_1 */
#define P_IV_IN_1_DATA_BIT_START                            0U
#define P_IV_IN_1_DATA_BIT_END                              31U

/* Definition for field DATA in Register P_IV_IN_2 */
#define P_IV_IN_2_DATA_BIT_START                            0U
#define P_IV_IN_2_DATA_BIT_END                              31U

/* Definition for field DATA in Register P_IV_IN_3 */
#define P_IV_IN_3_DATA_BIT_START                            0U
#define P_IV_IN_3_DATA_BIT_END                              31U

/* Definition for field OUTPUT_READY in Register P_CTRL */
#define P_CTRL_OUTPUT_READY_BIT_START                       0U
#define P_CTRL_OUTPUT_READY_BIT_END                         0U

/* Definition for field INPUT_READY in Register P_CTRL */
#define P_CTRL_INPUT_READY_BIT_START                        1U
#define P_CTRL_INPUT_READY_BIT_END                          1U

/* Definition for field DIRECTION in Register P_CTRL */
#define P_CTRL_DIRECTION_BIT_START                          2U
#define P_CTRL_DIRECTION_BIT_END                            2U

/* Definition for field KEY_SIZE in Register P_CTRL */
#define P_CTRL_KEY_SIZE_BIT_START                           3U
#define P_CTRL_KEY_SIZE_BIT_END                             4U

/* Definition for field MODE in Register P_CTRL */
#define P_CTRL_MODE_BIT_START                               5U
#define P_CTRL_MODE_BIT_END                                 5U

/* Definition for field CTR in Register P_CTRL */
#define P_CTRL_CTR_BIT_START                                6U
#define P_CTRL_CTR_BIT_END                                  6U

/* Definition for field CTR_WIDTH in Register P_CTRL */
#define P_CTRL_CTR_WIDTH_BIT_START                          7U
#define P_CTRL_CTR_WIDTH_BIT_END                            8U

/* Definition for field ICM in Register P_CTRL */
#define P_CTRL_ICM_BIT_START                                9U
#define P_CTRL_ICM_BIT_END                                  9U

/* Definition for field CFB in Register P_CTRL */
#define P_CTRL_CFB_BIT_START                                10U
#define P_CTRL_CFB_BIT_END                                  10U

/* Definition for field XTS in Register P_CTRL */
#define P_CTRL_XTS_BIT_START                                11U
#define P_CTRL_XTS_BIT_END                                  12U

/* Definition for field F8 in Register P_CTRL */
#define P_CTRL_F8_BIT_START                                 13U
#define P_CTRL_F8_BIT_END                                   13U

/* Definition for field F9 in Register P_CTRL */
#define P_CTRL_F9_BIT_START                                 14U
#define P_CTRL_F9_BIT_END                                   14U

/* Definition for field CBCMAC in Register P_CTRL */
#define P_CTRL_CBCMAC_BIT_START                             15U
#define P_CTRL_CBCMAC_BIT_END                               15U

/* Definition for field GCM in Register P_CTRL */
#define P_CTRL_GCM_BIT_START                                16U
#define P_CTRL_GCM_BIT_END                                  17U

/* Definition for field CCM in Register P_CTRL */
#define P_CTRL_CCM_BIT_START                                18U
#define P_CTRL_CCM_BIT_END                                  18U

/* Definition for field CCM_L in Register P_CTRL */
#define P_CTRL_CCM_L_BIT_START                              19U
#define P_CTRL_CCM_L_BIT_END                                21U

/* Definition for field CCM_M in Register P_CTRL */
#define P_CTRL_CCM_M_BIT_START                              22U
#define P_CTRL_CCM_M_BIT_END                                24U

/* Definition for field SAVE_CONTEXT in Register P_CTRL */
#define P_CTRL_SAVE_CONTEXT_BIT_START                       29U
#define P_CTRL_SAVE_CONTEXT_BIT_END                         29U

/* Definition for field SAVE_CONTEXT_READY in Register P_CTRL */
#define P_CTRL_SAVE_CONTEXT_READY_BIT_START                 30U
#define P_CTRL_SAVE_CONTEXT_READY_BIT_END                   30U

/* Definition for field CONTEXT_READY in Register P_CTRL */
#define P_CTRL_CONTEXT_READY_BIT_START                      31U
#define P_CTRL_CONTEXT_READY_BIT_END                        31U

/* Definition for field LENGTH in Register P_C_LENGTH_1 */
#define P_C_LENGTH_1_LENGTH_BIT_START                       0U
#define P_C_LENGTH_1_LENGTH_BIT_END                         28U

/* Definition for field AUTH in Register P_AUTH_LENGTH */
#define P_AUTH_LENGTH_AUTH_BIT_START                        0U
#define P_AUTH_LENGTH_AUTH_BIT_END                          31U

/* Definition for field DATA in Register P_DATA_IN_0 */
#define P_DATA_IN_0_DATA_BIT_START                          0U
#define P_DATA_IN_0_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_DATA_IN_1 */
#define P_DATA_IN_1_DATA_BIT_START                          0U
#define P_DATA_IN_1_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_DATA_IN_2 */
#define P_DATA_IN_2_DATA_BIT_START                          0U
#define P_DATA_IN_2_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_DATA_IN_3 */
#define P_DATA_IN_3_DATA_BIT_START                          0U
#define P_DATA_IN_3_DATA_BIT_END                            31U

/* Definition for field HASH in Register P_TAG_OUT_0 */
#define P_TAG_OUT_0_HASH_BIT_START                          0U
#define P_TAG_OUT_0_HASH_BIT_END                            31U

/* Definition for field HASH in Register P_TAG_OUT_1 */
#define P_TAG_OUT_1_HASH_BIT_START                          0U
#define P_TAG_OUT_1_HASH_BIT_END                            31U

/* Definition for field HASH in Register P_TAG_OUT_2 */
#define P_TAG_OUT_2_HASH_BIT_START                          0U
#define P_TAG_OUT_2_HASH_BIT_END                            31U

/* Definition for field HASH in Register P_TAG_OUT_3 */
#define P_TAG_OUT_3_HASH_BIT_START                          0U
#define P_TAG_OUT_3_HASH_BIT_END                            31U

/* Definition for field Y_MINOR in Register P_REVISION */
#define P_REVISION_Y_MINOR_BIT_START                        0U
#define P_REVISION_Y_MINOR_BIT_END                          5U

/* Definition for field CUSTOM in Register P_REVISION */
#define P_REVISION_CUSTOM_BIT_START                         6U
#define P_REVISION_CUSTOM_BIT_END                           7U

/* Definition for field X_MAJOR in Register P_REVISION */
#define P_REVISION_X_MAJOR_BIT_START                        8U
#define P_REVISION_X_MAJOR_BIT_END                          10U

/* Definition for field R_RTL in Register P_REVISION */
#define P_REVISION_R_RTL_BIT_START                          11U
#define P_REVISION_R_RTL_BIT_END                            15U

/* Definition for field FUNC in Register P_REVISION */
#define P_REVISION_FUNC_BIT_START                           16U
#define P_REVISION_FUNC_BIT_END                             27U

/* Definition for field SCHEME in Register P_REVISION */
#define P_REVISION_SCHEME_BIT_START                         30U
#define P_REVISION_SCHEME_BIT_END                           31U

/* Definition for field AUTOIDLE in Register P_SYSCONFIG */
#define P_SYSCONFIG_AUTOIDLE_BIT_START                      0U
#define P_SYSCONFIG_AUTOIDLE_BIT_END                        0U

/* Definition for field SOFTRESET in Register P_SYSCONFIG */
#define P_SYSCONFIG_SOFTRESET_BIT_START                     1U
#define P_SYSCONFIG_SOFTRESET_BIT_END                       1U

/* Definition for field SIDLE in Register P_SYSCONFIG */
#define P_SYSCONFIG_SIDLE_BIT_START                         2U
#define P_SYSCONFIG_SIDLE_BIT_END                           3U

/* Definition for field DIRECTBUSEN in Register P_SYSCONFIG */
#define P_SYSCONFIG_DIRECTBUSEN_BIT_START                   4U
#define P_SYSCONFIG_DIRECTBUSEN_BIT_END                     4U

/* Definition for field DMA_REQ_DATA_IN_EN in Register P_SYSCONFIG */
#define P_SYSCONFIG_DMA_REQ_DATA_IN_EN_BIT_START            5U
#define P_SYSCONFIG_DMA_REQ_DATA_IN_EN_BIT_END              5U

/* Definition for field DMA_REQ_DATA_OUT_EN in Register P_SYSCONFIG */
#define P_SYSCONFIG_DMA_REQ_DATA_OUT_EN_BIT_START           6U
#define P_SYSCONFIG_DMA_REQ_DATA_OUT_EN_BIT_END             6U

/* Definition for field DMA_REQ_CONTEXT_IN_EN in Register P_SYSCONFIG */
#define P_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_BIT_START         7U
#define P_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN_BIT_END           7U

/* Definition for field DMA_REQ_CONTEXT_OUT_EN in Register P_SYSCONFIG */
#define P_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_BIT_START        8U
#define P_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN_BIT_END          8U

/* Definition for field MAP_CONTEXT_OUT_ON_DATA_OUT in Register P_SYSCONFIG */
#define P_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_BIT_START   9U
#define P_SYSCONFIG_MAP_CONTEXT_OUT_ON_DATA_OUT_BIT_END     9U

/* Definition for field RESETDONE in Register P_SYSSTATUS */
#define P_SYSSTATUS_RESETDONE_BIT_START                     0U
#define P_SYSSTATUS_RESETDONE_BIT_END                       0U

/* Definition for field CONTEX_IN in Register P_IRQSTATUS */
#define P_IRQSTATUS_CONTEX_IN_BIT_START                     0U
#define P_IRQSTATUS_CONTEX_IN_BIT_END                       0U

/* Definition for field DATA_IN in Register P_IRQSTATUS */
#define P_IRQSTATUS_DATA_IN_BIT_START                       1U
#define P_IRQSTATUS_DATA_IN_BIT_END                         1U

/* Definition for field DATA_OUT in Register P_IRQSTATUS */
#define P_IRQSTATUS_DATA_OUT_BIT_START                      2U
#define P_IRQSTATUS_DATA_OUT_BIT_END                        2U

/* Definition for field CONTEXT_OUT in Register P_IRQSTATUS */
#define P_IRQSTATUS_CONTEXT_OUT_BIT_START                   3U
#define P_IRQSTATUS_CONTEXT_OUT_BIT_END                     3U

/* Definition for field CONTEX_IN in Register P_IRQENABLE */
#define P_IRQENABLE_CONTEX_IN_BIT_START                     0U
#define P_IRQENABLE_CONTEX_IN_BIT_END                       0U

/* Definition for field DATA_IN in Register P_IRQENABLE */
#define P_IRQENABLE_DATA_IN_BIT_START                       1U
#define P_IRQENABLE_DATA_IN_BIT_END                         1U

/* Definition for field DATA_OUT in Register P_IRQENABLE */
#define P_IRQENABLE_DATA_OUT_BIT_START                      2U
#define P_IRQENABLE_DATA_OUT_BIT_END                        2U

/* Definition for field CONTEXT_OUT in Register P_IRQENABLE */
#define P_IRQENABLE_CONTEXT_OUT_BIT_START                   3U
#define P_IRQENABLE_CONTEXT_OUT_BIT_END                     3U



/**
 * @struct AESRegs_t
 * @brief
 *   Module MSS_AES Register Definition
 * @details
 *   This structure is used to access the MSS_AES module registers.
 */
/**
 * @typedef AESRegs
 * @brief
 *   Module MSS_AES Register Frame type Definition
 * @details
 *   This type is used to access the MSS_AES module registers.
 */
typedef volatile struct AESRegs_t{
    uint32_t    S_KEY2_6                           ;        /* Offset = 0x000 */
    uint32_t    S_KEY2_7                           ;        /* Offset = 0x004 */
    uint32_t    S_KEY2_4                           ;        /* Offset = 0x008 */
    uint32_t    S_KEY2_5                           ;        /* Offset = 0x00C */
    uint32_t    S_KEY2_2                           ;        /* Offset = 0x010 */
    uint32_t    S_KEY2_3                           ;        /* Offset = 0x014 */
    uint32_t    S_KEY2_0                           ;        /* Offset = 0x018 */
    uint32_t    S_KEY2_1                           ;        /* Offset = 0x01C */
    uint32_t    S_KEY1_6                           ;        /* Offset = 0x020 */
    uint32_t    S_KEY1_7                           ;        /* Offset = 0x024 */
    uint32_t    S_KEY1_4                           ;        /* Offset = 0x028 */
    uint32_t    S_KEY1_5                           ;        /* Offset = 0x02C */
    uint32_t    S_KEY1_2                           ;        /* Offset = 0x030 */
    uint32_t    S_KEY1_3                           ;        /* Offset = 0x034 */
    uint32_t    S_KEY1_0                           ;        /* Offset = 0x038 */
    uint32_t    S_KEY1_1                           ;        /* Offset = 0x03C */
    uint32_t    S_IV_IN_0                          ;        /* Offset = 0x040 */
    uint32_t    S_IV_IN_1                          ;        /* Offset = 0x044 */
    uint32_t    S_IV_IN_2                          ;        /* Offset = 0x048 */
    uint32_t    S_IV_IN_3                          ;        /* Offset = 0x04C */
    uint32_t    S_CTRL                             ;        /* Offset = 0x050 */
    uint32_t    S_C_LENGTH_0                       ;        /* Offset = 0x054 */
    uint32_t    S_C_LENGTH_1                       ;        /* Offset = 0x058 */
    uint32_t    S_AUTH_LENGTH                      ;        /* Offset = 0x05C */
    uint32_t    S_DATA_IN_0                        ;        /* Offset = 0x060 */
    uint32_t    S_DATA_IN_1                        ;        /* Offset = 0x064 */
    uint32_t    S_DATA_IN_2                        ;        /* Offset = 0x068 */
    uint32_t    S_DATA_IN_3                        ;        /* Offset = 0x06C */
    uint32_t    S_TAG_OUT_0                        ;        /* Offset = 0x070 */
    uint32_t    S_TAG_OUT_1                        ;        /* Offset = 0x074 */
    uint32_t    S_TAG_OUT_2                        ;        /* Offset = 0x078 */
    uint32_t    S_TAG_OUT_3                        ;        /* Offset = 0x07C */
    uint32_t    S_REVISION                         ;        /* Offset = 0x080 */
    uint32_t    S_SYSCONFIG                        ;        /* Offset = 0x084 */
    uint32_t    S_SYSSTATUS                        ;        /* Offset = 0x088 */
    uint32_t    S_IRQSTATUS                        ;        /* Offset = 0x08C */
    uint32_t    S_IRQENABLE                        ;        /* Offset = 0x090 */
    uint32_t    S_DIRTYBITS                        ;        /* Offset = 0x094 */
    uint32_t    S_LOCKDOWN                         ;        /* Offset = 0x098 */
    uint32_t    RESTRICTED1[985]                   ;        /* Offset = 0x09C */
    uint32_t    P_KEY2_6                           ;        /* Offset = 0x1000 */
    uint32_t    P_KEY2_7                           ;        /* Offset = 0x1004 */
    uint32_t    P_KEY2_4                           ;        /* Offset = 0x1008 */
    uint32_t    P_KEY2_5                           ;        /* Offset = 0x100C */
    uint32_t    P_KEY2_2                           ;        /* Offset = 0x1010 */
    uint32_t    P_KEY2_3                           ;        /* Offset = 0x1014 */
    uint32_t    P_KEY2_0                           ;        /* Offset = 0x1018 */
    uint32_t    P_KEY2_1                           ;        /* Offset = 0x101C */
    uint32_t    P_KEY1_6                           ;        /* Offset = 0x1020 */
    uint32_t    P_KEY1_7                           ;        /* Offset = 0x1024 */
    uint32_t    P_KEY1_4                           ;        /* Offset = 0x1028 */
    uint32_t    P_KEY1_5                           ;        /* Offset = 0x102C */
    uint32_t    P_KEY1_2                           ;        /* Offset = 0x1030 */
    uint32_t    P_KEY1_3                           ;        /* Offset = 0x1034 */
    uint32_t    P_KEY1_0                           ;        /* Offset = 0x1038 */
    uint32_t    P_KEY1_1                           ;        /* Offset = 0x103C */
    uint32_t    P_IV_IN_0                          ;        /* Offset = 0x1040 */
    uint32_t    P_IV_IN_1                          ;        /* Offset = 0x1044 */
    uint32_t    P_IV_IN_2                          ;        /* Offset = 0x1048 */
    uint32_t    P_IV_IN_3                          ;        /* Offset = 0x104C */
    uint32_t    P_CTRL                             ;        /* Offset = 0x1050 */
    uint32_t    P_C_LENGTH_0                       ;        /* Offset = 0x1054 */
    uint32_t    P_C_LENGTH_1                       ;        /* Offset = 0x1058 */
    uint32_t    P_AUTH_LENGTH                      ;        /* Offset = 0x105C */
    uint32_t    P_DATA_IN_0                        ;        /* Offset = 0x1060 */
    uint32_t    P_DATA_IN_1                        ;        /* Offset = 0x1064 */
    uint32_t    P_DATA_IN_2                        ;        /* Offset = 0x1068 */
    uint32_t    P_DATA_IN_3                        ;        /* Offset = 0x106C */
    uint32_t    P_TAG_OUT_0                        ;        /* Offset = 0x1070 */
    uint32_t    P_TAG_OUT_1                        ;        /* Offset = 0x1074 */
    uint32_t    P_TAG_OUT_2                        ;        /* Offset = 0x1078 */
    uint32_t    P_TAG_OUT_3                        ;        /* Offset = 0x107C */
    uint32_t    P_REVISION                         ;        /* Offset = 0x1080 */
    uint32_t    P_SYSCONFIG                        ;        /* Offset = 0x1084 */
    uint32_t    P_SYSSTATUS                        ;        /* Offset = 0x1088 */
    uint32_t    P_IRQSTATUS                        ;        /* Offset = 0x108C */
    uint32_t    P_IRQENABLE                        ;        /* Offset = 0x1090 */
} AESRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_AES_H */
/* END OF REG_AES_H */

