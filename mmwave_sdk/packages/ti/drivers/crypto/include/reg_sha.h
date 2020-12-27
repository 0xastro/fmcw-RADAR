/**
 *  @file    reg_sha.h
 *
 *  @brief
 *    This file gives register definitions of MSS_SHA module.
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

#ifndef REG_SHA_H
#define REG_SHA_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field DATA in Register S_ODIGEST_A */
#define S_ODIGEST_A_DATA_BIT_START                          0U
#define S_ODIGEST_A_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_ODIGEST_B */
#define S_ODIGEST_B_DATA_BIT_START                          0U
#define S_ODIGEST_B_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_ODIGEST_C */
#define S_ODIGEST_C_DATA_BIT_START                          0U
#define S_ODIGEST_C_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_ODIGEST_D */
#define S_ODIGEST_D_DATA_BIT_START                          0U
#define S_ODIGEST_D_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_ODIGEST_E */
#define S_ODIGEST_E_DATA_BIT_START                          0U
#define S_ODIGEST_E_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_ODIGEST_F */
#define S_ODIGEST_F_DATA_BIT_START                          0U
#define S_ODIGEST_F_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_ODIGEST_G */
#define S_ODIGEST_G_DATA_BIT_START                          0U
#define S_ODIGEST_G_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_ODIGEST_H */
#define S_ODIGEST_H_DATA_BIT_START                          0U
#define S_ODIGEST_H_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_A */
#define S_IDIGEST_A_DATA_BIT_START                          0U
#define S_IDIGEST_A_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_B */
#define S_IDIGEST_B_DATA_BIT_START                          0U
#define S_IDIGEST_B_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_C */
#define S_IDIGEST_C_DATA_BIT_START                          0U
#define S_IDIGEST_C_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_D */
#define S_IDIGEST_D_DATA_BIT_START                          0U
#define S_IDIGEST_D_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_E */
#define S_IDIGEST_E_DATA_BIT_START                          0U
#define S_IDIGEST_E_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_F */
#define S_IDIGEST_F_DATA_BIT_START                          0U
#define S_IDIGEST_F_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_G */
#define S_IDIGEST_G_DATA_BIT_START                          0U
#define S_IDIGEST_G_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_IDIGEST_H */
#define S_IDIGEST_H_DATA_BIT_START                          0U
#define S_IDIGEST_H_DATA_BIT_END                            31U

/* Definition for field DATA in Register S_DIGEST_COUNT */
#define S_DIGEST_COUNT_DATA_BIT_START                       0U
#define S_DIGEST_COUNT_DATA_BIT_END                         31U

/* Definition for field ALGO in Register S_MODE */
#define S_MODE_ALGO_BIT_START                               1U
#define S_MODE_ALGO_BIT_END                                 2U

/* Definition for field ALGO_CONSTANT in Register S_MODE */
#define S_MODE_ALGO_CONSTANT_BIT_START                      3U
#define S_MODE_ALGO_CONSTANT_BIT_END                        3U

/* Definition for field CLOSE_HASH in Register S_MODE */
#define S_MODE_CLOSE_HASH_BIT_START                         4U
#define S_MODE_CLOSE_HASH_BIT_END                           4U

/* Definition for field HMAC_KEY_PROC in Register S_MODE */
#define S_MODE_HMAC_KEY_PROC_BIT_START                      5U
#define S_MODE_HMAC_KEY_PROC_BIT_END                        5U

/* Definition for field HMAC_OUTER_HASH in Register S_MODE */
#define S_MODE_HMAC_OUTER_HASH_BIT_START                    7U
#define S_MODE_HMAC_OUTER_HASH_BIT_END                      7U

/* Definition for field DATA in Register S_LENGTH */
#define S_LENGTH_DATA_BIT_START                             0U
#define S_LENGTH_DATA_BIT_END                               31U

/* Definition for field DATA0_IN in Register S_DATA0_IN */
#define S_DATA0_IN_DATA0_IN_BIT_START                       0U
#define S_DATA0_IN_DATA0_IN_BIT_END                         31U

/* Definition for field DATA1_IN in Register S_DATA1_IN */
#define S_DATA1_IN_DATA1_IN_BIT_START                       0U
#define S_DATA1_IN_DATA1_IN_BIT_END                         31U

/* Definition for field DATA2_IN in Register S_DATA2_IN */
#define S_DATA2_IN_DATA2_IN_BIT_START                       0U
#define S_DATA2_IN_DATA2_IN_BIT_END                         31U

/* Definition for field DATA3_IN in Register S_DATA3_IN */
#define S_DATA3_IN_DATA3_IN_BIT_START                       0U
#define S_DATA3_IN_DATA3_IN_BIT_END                         31U

/* Definition for field DATA4_IN in Register S_DATA4_IN */
#define S_DATA4_IN_DATA4_IN_BIT_START                       0U
#define S_DATA4_IN_DATA4_IN_BIT_END                         31U

/* Definition for field DATA5_IN in Register S_DATA5_IN */
#define S_DATA5_IN_DATA5_IN_BIT_START                       0U
#define S_DATA5_IN_DATA5_IN_BIT_END                         31U

/* Definition for field DATA6_IN in Register S_DATA6_IN */
#define S_DATA6_IN_DATA6_IN_BIT_START                       0U
#define S_DATA6_IN_DATA6_IN_BIT_END                         31U

/* Definition for field DATA7_IN in Register S_DATA7_IN */
#define S_DATA7_IN_DATA7_IN_BIT_START                       0U
#define S_DATA7_IN_DATA7_IN_BIT_END                         31U

/* Definition for field DATA8_IN in Register S_DATA8_IN */
#define S_DATA8_IN_DATA8_IN_BIT_START                       0U
#define S_DATA8_IN_DATA8_IN_BIT_END                         31U

/* Definition for field DATA9_IN in Register S_DATA9_IN */
#define S_DATA9_IN_DATA9_IN_BIT_START                       0U
#define S_DATA9_IN_DATA9_IN_BIT_END                         31U

/* Definition for field DATA10_IN in Register S_DATA10_IN */
#define S_DATA10_IN_DATA10_IN_BIT_START                     0U
#define S_DATA10_IN_DATA10_IN_BIT_END                       31U

/* Definition for field DATA11_IN in Register S_DATA11_IN */
#define S_DATA11_IN_DATA11_IN_BIT_START                     0U
#define S_DATA11_IN_DATA11_IN_BIT_END                       31U

/* Definition for field DATA12_IN in Register S_DATA12_IN */
#define S_DATA12_IN_DATA12_IN_BIT_START                     0U
#define S_DATA12_IN_DATA12_IN_BIT_END                       31U

/* Definition for field DATA13_IN in Register S_DATA13_IN */
#define S_DATA13_IN_DATA13_IN_BIT_START                     0U
#define S_DATA13_IN_DATA13_IN_BIT_END                       31U

/* Definition for field DATA14_IN in Register S_DATA14_IN */
#define S_DATA14_IN_DATA14_IN_BIT_START                     0U
#define S_DATA14_IN_DATA14_IN_BIT_END                       31U

/* Definition for field DATA15_IN in Register S_DATA15_IN */
#define S_DATA15_IN_DATA15_IN_BIT_START                     0U
#define S_DATA15_IN_DATA15_IN_BIT_END                       31U

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

/* Definition for field PIT_EN in Register S_SYSCONFIG */
#define S_SYSCONFIG_PIT_EN_BIT_START                        2U
#define S_SYSCONFIG_PIT_EN_BIT_END                          2U

/* Definition for field PDMA_EN in Register S_SYSCONFIG */
#define S_SYSCONFIG_PDMA_EN_BIT_START                       3U
#define S_SYSCONFIG_PDMA_EN_BIT_END                         3U

/* Definition for field PCONT_SWT in Register S_SYSCONFIG */
#define S_SYSCONFIG_PCONT_SWT_BIT_START                     6U
#define S_SYSCONFIG_PCONT_SWT_BIT_END                       6U

/* Definition for field PADVANCED in Register S_SYSCONFIG */
#define S_SYSCONFIG_PADVANCED_BIT_START                     7U
#define S_SYSCONFIG_PADVANCED_BIT_END                       7U

/* Definition for field RESETDONE in Register S_SYSSTATUS */
#define S_SYSSTATUS_RESETDONE_BIT_START                     0U
#define S_SYSSTATUS_RESETDONE_BIT_END                       0U

/* Definition for field OUTPUT_READY in Register S_IRQSTATUS */
#define S_IRQSTATUS_OUTPUT_READY_BIT_START                  0U
#define S_IRQSTATUS_OUTPUT_READY_BIT_END                    0U

/* Definition for field INPUT_READY in Register S_IRQSTATUS */
#define S_IRQSTATUS_INPUT_READY_BIT_START                   1U
#define S_IRQSTATUS_INPUT_READY_BIT_END                     1U

/* Definition for field PARTHASH_READY in Register S_IRQSTATUS */
#define S_IRQSTATUS_PARTHASH_READY_BIT_START                2U
#define S_IRQSTATUS_PARTHASH_READY_BIT_END                  2U

/* Definition for field CONTEXT_READY in Register S_IRQSTATUS */
#define S_IRQSTATUS_CONTEXT_READY_BIT_START                 3U
#define S_IRQSTATUS_CONTEXT_READY_BIT_END                   3U

/* Definition for field M_OUTPUT_READY in Register S_IRQENABLE */
#define S_IRQENABLE_M_OUTPUT_READY_BIT_START                0U
#define S_IRQENABLE_M_OUTPUT_READY_BIT_END                  0U

/* Definition for field M_INPUT_READY in Register S_IRQENABLE */
#define S_IRQENABLE_M_INPUT_READY_BIT_START                 1U
#define S_IRQENABLE_M_INPUT_READY_BIT_END                   1U

/* Definition for field M_PARTHASH_READY in Register S_IRQENABLE */
#define S_IRQENABLE_M_PARTHASH_READY_BIT_START              2U
#define S_IRQENABLE_M_PARTHASH_READY_BIT_END                2U

/* Definition for field M_CONTEXT_READY in Register S_IRQENABLE */
#define S_IRQENABLE_M_CONTEXT_READY_BIT_START               3U
#define S_IRQENABLE_M_CONTEXT_READY_BIT_END                 3U

/* Definition for field SACCESSED in Register S_XSSTATUS */
#define S_XSSTATUS_SACCESSED_BIT_START                      0U
#define S_XSSTATUS_SACCESSED_BIT_END                        0U

/* Definition for field SDIRTY in Register S_XSSTATUS */
#define S_XSSTATUS_SDIRTY_BIT_START                         1U
#define S_XSSTATUS_SDIRTY_BIT_END                           1U

/* Definition for field PACCESSED in Register S_XSSTATUS */
#define S_XSSTATUS_PACCESSED_BIT_START                      2U
#define S_XSSTATUS_PACCESSED_BIT_END                        2U

/* Definition for field PDIRTY in Register S_XSSTATUS */
#define S_XSSTATUS_PDIRTY_BIT_START                         3U
#define S_XSSTATUS_PDIRTY_BIT_END                           3U

/* Definition for field LOCK_ALGO in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_ALGO_BIT_START                      0U
#define S_LOCKDOWN_LOCK_ALGO_BIT_END                        2U

/* Definition for field LOCK_USE_ALOG_CONST in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_USE_ALOG_CONST_BIT_START            3U
#define S_LOCKDOWN_LOCK_USE_ALOG_CONST_BIT_END              3U

/* Definition for field LOCK_CLOSE_HASH in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_CLOSE_HASH_BIT_START                4U
#define S_LOCKDOWN_LOCK_CLOSE_HASH_BIT_END                  4U

/* Definition for field LOCK_HMAC_KEY in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_HMAC_KEY_BIT_START                  5U
#define S_LOCKDOWN_LOCK_HMAC_KEY_BIT_END                    5U

/* Definition for field LOCK_REUSE_HMAC_KEY in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_REUSE_HMAC_KEY_BIT_START            6U
#define S_LOCKDOWN_LOCK_REUSE_HMAC_KEY_BIT_END              6U

/* Definition for field LOCK_HMAC_OUTER_HASH in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_HMAC_OUTER_HASH_BIT_START           7U
#define S_LOCKDOWN_LOCK_HMAC_OUTER_HASH_BIT_END             7U

/* Definition for field LOCK_IDIGEST in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_IDIGEST_BIT_START                   24U
#define S_LOCKDOWN_LOCK_IDIGEST_BIT_END                     24U

/* Definition for field LOCK_ODIGEST in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_ODIGEST_BIT_START                   25U
#define S_LOCKDOWN_LOCK_ODIGEST_BIT_END                     25U

/* Definition for field LOCK_DIGESTCOUNT in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_DIGESTCOUNT_BIT_START               26U
#define S_LOCKDOWN_LOCK_DIGESTCOUNT_BIT_END                 26U

/* Definition for field LOCK_LENGTH in Register S_LOCKDOWN */
#define S_LOCKDOWN_LOCK_LENGTH_BIT_START                    27U
#define S_LOCKDOWN_LOCK_LENGTH_BIT_END                      27U

/* Definition for field BLOCK_ODIGEST_RD in Register S_LOCKDOWN */
#define S_LOCKDOWN_BLOCK_ODIGEST_RD_BIT_START               28U
#define S_LOCKDOWN_BLOCK_ODIGEST_RD_BIT_END                 28U

/* Definition for field ODIGEST_A in Register S_HASH512_ODIGEST_A */
#define S_HASH512_ODIGEST_A_ODIGEST_A_BIT_START             0U
#define S_HASH512_ODIGEST_A_ODIGEST_A_BIT_END               31U

/* Definition for field ODIGEST_B in Register S_HASH512_ODIGEST_B */
#define S_HASH512_ODIGEST_B_ODIGEST_B_BIT_START             0U
#define S_HASH512_ODIGEST_B_ODIGEST_B_BIT_END               31U

/* Definition for field ODIGEST_C in Register S_HASH512_ODIGEST_C */
#define S_HASH512_ODIGEST_C_ODIGEST_C_BIT_START             0U
#define S_HASH512_ODIGEST_C_ODIGEST_C_BIT_END               31U

/* Definition for field ODIGEST_D in Register S_HASH512_ODIGEST_D */
#define S_HASH512_ODIGEST_D_ODIGEST_D_BIT_START             0U
#define S_HASH512_ODIGEST_D_ODIGEST_D_BIT_END               31U

/* Definition for field ODIGEST_E in Register S_HASH512_ODIGEST_E */
#define S_HASH512_ODIGEST_E_ODIGEST_E_BIT_START             0U
#define S_HASH512_ODIGEST_E_ODIGEST_E_BIT_END               31U

/* Definition for field ODIGEST_F in Register S_HASH512_ODIGEST_F */
#define S_HASH512_ODIGEST_F_ODIGEST_F_BIT_START             0U
#define S_HASH512_ODIGEST_F_ODIGEST_F_BIT_END               31U

/* Definition for field ODIGEST_G in Register S_HASH512_ODIGEST_G */
#define S_HASH512_ODIGEST_G_ODIGEST_G_BIT_START             0U
#define S_HASH512_ODIGEST_G_ODIGEST_G_BIT_END               31U

/* Definition for field ODIGEST_H in Register S_HASH512_ODIGEST_H */
#define S_HASH512_ODIGEST_H_ODIGEST_H_BIT_START             0U
#define S_HASH512_ODIGEST_H_ODIGEST_H_BIT_END               31U

/* Definition for field ODIGEST_I in Register S_HASH512_ODIGEST_I */
#define S_HASH512_ODIGEST_I_ODIGEST_I_BIT_START             0U
#define S_HASH512_ODIGEST_I_ODIGEST_I_BIT_END               31U

/* Definition for field ODIGEST_J in Register S_HASH512_ODIGEST_J */
#define S_HASH512_ODIGEST_J_ODIGEST_J_BIT_START             0U
#define S_HASH512_ODIGEST_J_ODIGEST_J_BIT_END               31U

/* Definition for field ODIGEST_K in Register S_HASH512_ODIGEST_K */
#define S_HASH512_ODIGEST_K_ODIGEST_K_BIT_START             0U
#define S_HASH512_ODIGEST_K_ODIGEST_K_BIT_END               31U

/* Definition for field ODIGEST_L in Register S_HASH512_ODIGEST_L */
#define S_HASH512_ODIGEST_L_ODIGEST_L_BIT_START             0U
#define S_HASH512_ODIGEST_L_ODIGEST_L_BIT_END               31U

/* Definition for field ODIGEST_M in Register S_HASH512_ODIGEST_M */
#define S_HASH512_ODIGEST_M_ODIGEST_M_BIT_START             0U
#define S_HASH512_ODIGEST_M_ODIGEST_M_BIT_END               31U

/* Definition for field ODIGEST_N in Register S_HASH512_ODIGEST_N */
#define S_HASH512_ODIGEST_N_ODIGEST_N_BIT_START             0U
#define S_HASH512_ODIGEST_N_ODIGEST_N_BIT_END               31U

/* Definition for field ODIGEST_O in Register S_HASH512_ODIGEST_O */
#define S_HASH512_ODIGEST_O_ODIGEST_O_BIT_START             0U
#define S_HASH512_ODIGEST_O_ODIGEST_O_BIT_END               31U

/* Definition for field ODIGEST_P in Register S_HASH512_ODIGEST_P */
#define S_HASH512_ODIGEST_P_ODIGEST_P_BIT_START             0U
#define S_HASH512_ODIGEST_P_ODIGEST_P_BIT_END               31U

/* Definition for field IDIGEST_A in Register S_HASH512_IDIGEST_A */
#define S_HASH512_IDIGEST_A_IDIGEST_A_BIT_START             0U
#define S_HASH512_IDIGEST_A_IDIGEST_A_BIT_END               31U

/* Definition for field IDIGEST_B in Register S_HASH512_IDIGEST_B */
#define S_HASH512_IDIGEST_B_IDIGEST_B_BIT_START             0U
#define S_HASH512_IDIGEST_B_IDIGEST_B_BIT_END               31U

/* Definition for field IDIGEST_C in Register S_HASH512_IDIGEST_C */
#define S_HASH512_IDIGEST_C_IDIGEST_C_BIT_START             0U
#define S_HASH512_IDIGEST_C_IDIGEST_C_BIT_END               31U

/* Definition for field IDIGEST_D in Register S_HASH512_IDIGEST_D */
#define S_HASH512_IDIGEST_D_IDIGEST_D_BIT_START             0U
#define S_HASH512_IDIGEST_D_IDIGEST_D_BIT_END               31U

/* Definition for field IDIGEST_E in Register S_HASH512_IDIGEST_E */
#define S_HASH512_IDIGEST_E_IDIGEST_E_BIT_START             0U
#define S_HASH512_IDIGEST_E_IDIGEST_E_BIT_END               31U

/* Definition for field IDIGEST_F in Register S_HASH512_IDIGEST_F */
#define S_HASH512_IDIGEST_F_IDIGEST_F_BIT_START             0U
#define S_HASH512_IDIGEST_F_IDIGEST_F_BIT_END               31U

/* Definition for field IDIGEST_G in Register S_HASH512_IDIGEST_G */
#define S_HASH512_IDIGEST_G_IDIGEST_G_BIT_START             0U
#define S_HASH512_IDIGEST_G_IDIGEST_G_BIT_END               31U

/* Definition for field IDIGEST_H in Register S_HASH512_IDIGEST_H */
#define S_HASH512_IDIGEST_H_IDIGEST_H_BIT_START             0U
#define S_HASH512_IDIGEST_H_IDIGEST_H_BIT_END               31U

/* Definition for field IDIGEST_I in Register S_HASH512_IDIGEST_I */
#define S_HASH512_IDIGEST_I_IDIGEST_I_BIT_START             0U
#define S_HASH512_IDIGEST_I_IDIGEST_I_BIT_END               31U

/* Definition for field IDIGEST_J in Register S_HASH512_IDIGEST_J */
#define S_HASH512_IDIGEST_J_IDIGEST_J_BIT_START             0U
#define S_HASH512_IDIGEST_J_IDIGEST_J_BIT_END               31U

/* Definition for field IDIGEST_K in Register S_HASH512_IDIGEST_K */
#define S_HASH512_IDIGEST_K_IDIGEST_K_BIT_START             0U
#define S_HASH512_IDIGEST_K_IDIGEST_K_BIT_END               31U

/* Definition for field IDIGEST_L in Register S_HASH512_IDIGEST_L */
#define S_HASH512_IDIGEST_L_IDIGEST_L_BIT_START             0U
#define S_HASH512_IDIGEST_L_IDIGEST_L_BIT_END               31U

/* Definition for field IDIGEST_M in Register S_HASH512_IDIGEST_M */
#define S_HASH512_IDIGEST_M_IDIGEST_M_BIT_START             0U
#define S_HASH512_IDIGEST_M_IDIGEST_M_BIT_END               31U

/* Definition for field IDIGEST_N in Register S_HASH512_IDIGEST_N */
#define S_HASH512_IDIGEST_N_IDIGEST_N_BIT_START             0U
#define S_HASH512_IDIGEST_N_IDIGEST_N_BIT_END               31U

/* Definition for field IDIGEST_O in Register S_HASH512_IDIGEST_O */
#define S_HASH512_IDIGEST_O_IDIGEST_O_BIT_START             0U
#define S_HASH512_IDIGEST_O_IDIGEST_O_BIT_END               31U

/* Definition for field IDIGEST_P in Register S_HASH512_IDIGEST_P */
#define S_HASH512_IDIGEST_P_IDIGEST_P_BIT_START             0U
#define S_HASH512_IDIGEST_P_IDIGEST_P_BIT_END               31U

/* Definition for field DIGEST_COUNT in Register S_HASH512_DIGEST_COUNT */
#define S_HASH512_DIGEST_COUNT_DIGEST_COUNT_BIT_START       0U
#define S_HASH512_DIGEST_COUNT_DIGEST_COUNT_BIT_END         31U

/* Definition for field ALGORITHM in Register S_HASH512_MODE */
#define S_HASH512_MODE_ALGORITHM_BIT_START                  0U
#define S_HASH512_MODE_ALGORITHM_BIT_END                    2U

/* Definition for field USE_ALG_CONSTANTS in Register S_HASH512_MODE */
#define S_HASH512_MODE_USE_ALG_CONSTANTS_BIT_START          3U
#define S_HASH512_MODE_USE_ALG_CONSTANTS_BIT_END            3U

/* Definition for field CLOSE_HASH in Register S_HASH512_MODE */
#define S_HASH512_MODE_CLOSE_HASH_BIT_START                 4U
#define S_HASH512_MODE_CLOSE_HASH_BIT_END                   4U

/* Definition for field HMAC_KEY_PROCESSING in Register S_HASH512_MODE */
#define S_HASH512_MODE_HMAC_KEY_PROCESSING_BIT_START        5U
#define S_HASH512_MODE_HMAC_KEY_PROCESSING_BIT_END          5U

/* Definition for field REUSE_HMAC_KEY in Register S_HASH512_MODE */
#define S_HASH512_MODE_REUSE_HMAC_KEY_BIT_START             6U
#define S_HASH512_MODE_REUSE_HMAC_KEY_BIT_END               6U

/* Definition for field HMAC_OUTER_HASH in Register S_HASH512_MODE */
#define S_HASH512_MODE_HMAC_OUTER_HASH_BIT_START            7U
#define S_HASH512_MODE_HMAC_OUTER_HASH_BIT_END              7U

/* Definition for field LENGTH in Register S_HASH512_LENGTH */
#define S_HASH512_LENGTH_LENGTH_BIT_START                   0U
#define S_HASH512_LENGTH_LENGTH_BIT_END                     31U

/* Definition for field DATA in Register P_ODIGEST_A */
#define P_ODIGEST_A_DATA_BIT_START                          0U
#define P_ODIGEST_A_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_ODIGEST_B */
#define P_ODIGEST_B_DATA_BIT_START                          0U
#define P_ODIGEST_B_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_ODIGEST_C */
#define P_ODIGEST_C_DATA_BIT_START                          0U
#define P_ODIGEST_C_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_ODIGEST_D */
#define P_ODIGEST_D_DATA_BIT_START                          0U
#define P_ODIGEST_D_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_ODIGEST_E */
#define P_ODIGEST_E_DATA_BIT_START                          0U
#define P_ODIGEST_E_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_ODIGEST_F */
#define P_ODIGEST_F_DATA_BIT_START                          0U
#define P_ODIGEST_F_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_ODIGEST_G */
#define P_ODIGEST_G_DATA_BIT_START                          0U
#define P_ODIGEST_G_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_ODIGEST_H */
#define P_ODIGEST_H_DATA_BIT_START                          0U
#define P_ODIGEST_H_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_A */
#define P_IDIGEST_A_DATA_BIT_START                          0U
#define P_IDIGEST_A_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_B */
#define P_IDIGEST_B_DATA_BIT_START                          0U
#define P_IDIGEST_B_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_C */
#define P_IDIGEST_C_DATA_BIT_START                          0U
#define P_IDIGEST_C_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_D */
#define P_IDIGEST_D_DATA_BIT_START                          0U
#define P_IDIGEST_D_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_E */
#define P_IDIGEST_E_DATA_BIT_START                          0U
#define P_IDIGEST_E_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_F */
#define P_IDIGEST_F_DATA_BIT_START                          0U
#define P_IDIGEST_F_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_G */
#define P_IDIGEST_G_DATA_BIT_START                          0U
#define P_IDIGEST_G_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_IDIGEST_H */
#define P_IDIGEST_H_DATA_BIT_START                          0U
#define P_IDIGEST_H_DATA_BIT_END                            31U

/* Definition for field DATA in Register P_DIGEST_COUNT */
#define P_DIGEST_COUNT_DATA_BIT_START                       0U
#define P_DIGEST_COUNT_DATA_BIT_END                         31U

/* Definition for field ALGO in Register P_MODE */
#define P_MODE_ALGO_BIT_START                               1U
#define P_MODE_ALGO_BIT_END                                 2U

/* Definition for field ALGO_CONSTANT in Register P_MODE */
#define P_MODE_ALGO_CONSTANT_BIT_START                      3U
#define P_MODE_ALGO_CONSTANT_BIT_END                        3U

/* Definition for field CLOSE_HASH in Register P_MODE */
#define P_MODE_CLOSE_HASH_BIT_START                         4U
#define P_MODE_CLOSE_HASH_BIT_END                           4U

/* Definition for field HMAC_KEY_PROC in Register P_MODE */
#define P_MODE_HMAC_KEY_PROC_BIT_START                      5U
#define P_MODE_HMAC_KEY_PROC_BIT_END                        5U

/* Definition for field HMAC_OUTER_HASH in Register P_MODE */
#define P_MODE_HMAC_OUTER_HASH_BIT_START                    7U
#define P_MODE_HMAC_OUTER_HASH_BIT_END                      7U

/* Definition for field DATA in Register P_LENGTH */
#define P_LENGTH_DATA_BIT_START                             0U
#define P_LENGTH_DATA_BIT_END                               31U

/* Definition for field DATA0_IN in Register P_DATA0_IN */
#define P_DATA0_IN_DATA0_IN_BIT_START                       0U
#define P_DATA0_IN_DATA0_IN_BIT_END                         31U

/* Definition for field DATA1_IN in Register P_DATA1_IN */
#define P_DATA1_IN_DATA1_IN_BIT_START                       0U
#define P_DATA1_IN_DATA1_IN_BIT_END                         31U

/* Definition for field DATA2_IN in Register P_DATA2_IN */
#define P_DATA2_IN_DATA2_IN_BIT_START                       0U
#define P_DATA2_IN_DATA2_IN_BIT_END                         31U

/* Definition for field DATA3_IN in Register P_DATA3_IN */
#define P_DATA3_IN_DATA3_IN_BIT_START                       0U
#define P_DATA3_IN_DATA3_IN_BIT_END                         31U

/* Definition for field DATA4_IN in Register P_DATA4_IN */
#define P_DATA4_IN_DATA4_IN_BIT_START                       0U
#define P_DATA4_IN_DATA4_IN_BIT_END                         31U

/* Definition for field DATA5_IN in Register P_DATA5_IN */
#define P_DATA5_IN_DATA5_IN_BIT_START                       0U
#define P_DATA5_IN_DATA5_IN_BIT_END                         31U

/* Definition for field DATA6_IN in Register P_DATA6_IN */
#define P_DATA6_IN_DATA6_IN_BIT_START                       0U
#define P_DATA6_IN_DATA6_IN_BIT_END                         31U

/* Definition for field DATA7_IN in Register P_DATA7_IN */
#define P_DATA7_IN_DATA7_IN_BIT_START                       0U
#define P_DATA7_IN_DATA7_IN_BIT_END                         31U

/* Definition for field DATA8_IN in Register P_DATA8_IN */
#define P_DATA8_IN_DATA8_IN_BIT_START                       0U
#define P_DATA8_IN_DATA8_IN_BIT_END                         31U

/* Definition for field DATA9_IN in Register P_DATA9_IN */
#define P_DATA9_IN_DATA9_IN_BIT_START                       0U
#define P_DATA9_IN_DATA9_IN_BIT_END                         31U

/* Definition for field DATA10_IN in Register P_DATA10_IN */
#define P_DATA10_IN_DATA10_IN_BIT_START                     0U
#define P_DATA10_IN_DATA10_IN_BIT_END                       31U

/* Definition for field DATA11_IN in Register P_DATA11_IN */
#define P_DATA11_IN_DATA11_IN_BIT_START                     0U
#define P_DATA11_IN_DATA11_IN_BIT_END                       31U

/* Definition for field DATA12_IN in Register P_DATA12_IN */
#define P_DATA12_IN_DATA12_IN_BIT_START                     0U
#define P_DATA12_IN_DATA12_IN_BIT_END                       31U

/* Definition for field DATA13_IN in Register P_DATA13_IN */
#define P_DATA13_IN_DATA13_IN_BIT_START                     0U
#define P_DATA13_IN_DATA13_IN_BIT_END                       31U

/* Definition for field DATA14_IN in Register P_DATA14_IN */
#define P_DATA14_IN_DATA14_IN_BIT_START                     0U
#define P_DATA14_IN_DATA14_IN_BIT_END                       31U

/* Definition for field DATA15_IN in Register P_DATA15_IN */
#define P_DATA15_IN_DATA15_IN_BIT_START                     0U
#define P_DATA15_IN_DATA15_IN_BIT_END                       31U

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

/* Definition for field PIT_EN in Register P_SYSCONFIG */
#define P_SYSCONFIG_PIT_EN_BIT_START                        2U
#define P_SYSCONFIG_PIT_EN_BIT_END                          2U

/* Definition for field PDMA_EN in Register P_SYSCONFIG */
#define P_SYSCONFIG_PDMA_EN_BIT_START                       3U
#define P_SYSCONFIG_PDMA_EN_BIT_END                         3U

/* Definition for field PCONT_SWT in Register P_SYSCONFIG */
#define P_SYSCONFIG_PCONT_SWT_BIT_START                     6U
#define P_SYSCONFIG_PCONT_SWT_BIT_END                       6U

/* Definition for field PADVANCED in Register P_SYSCONFIG */
#define P_SYSCONFIG_PADVANCED_BIT_START                     7U
#define P_SYSCONFIG_PADVANCED_BIT_END                       7U

/* Definition for field RESETDONE in Register P_SYSSTATUS */
#define P_SYSSTATUS_RESETDONE_BIT_START                     0U
#define P_SYSSTATUS_RESETDONE_BIT_END                       0U

/* Definition for field OUTPUT_READY in Register P_IRQSTATUS */
#define P_IRQSTATUS_OUTPUT_READY_BIT_START                  0U
#define P_IRQSTATUS_OUTPUT_READY_BIT_END                    0U

/* Definition for field INPUT_READY in Register P_IRQSTATUS */
#define P_IRQSTATUS_INPUT_READY_BIT_START                   1U
#define P_IRQSTATUS_INPUT_READY_BIT_END                     1U

/* Definition for field PARTHASH_READY in Register P_IRQSTATUS */
#define P_IRQSTATUS_PARTHASH_READY_BIT_START                2U
#define P_IRQSTATUS_PARTHASH_READY_BIT_END                  2U

/* Definition for field CONTEXT_READY in Register P_IRQSTATUS */
#define P_IRQSTATUS_CONTEXT_READY_BIT_START                 3U
#define P_IRQSTATUS_CONTEXT_READY_BIT_END                   3U

/* Definition for field M_OUTPUT_READY in Register P_IRQENABLE */
#define P_IRQENABLE_M_OUTPUT_READY_BIT_START                0U
#define P_IRQENABLE_M_OUTPUT_READY_BIT_END                  0U

/* Definition for field M_INPUT_READY in Register P_IRQENABLE */
#define P_IRQENABLE_M_INPUT_READY_BIT_START                 1U
#define P_IRQENABLE_M_INPUT_READY_BIT_END                   1U

/* Definition for field M_PARTHASH_READY in Register P_IRQENABLE */
#define P_IRQENABLE_M_PARTHASH_READY_BIT_START              2U
#define P_IRQENABLE_M_PARTHASH_READY_BIT_END                2U

/* Definition for field M_CONTEXT_READY in Register P_IRQENABLE */
#define P_IRQENABLE_M_CONTEXT_READY_BIT_START               3U
#define P_IRQENABLE_M_CONTEXT_READY_BIT_END                 3U

/* Definition for field ODIGEST_A in Register P_HASH512_ODIGEST_A */
#define P_HASH512_ODIGEST_A_ODIGEST_A_BIT_START             0U
#define P_HASH512_ODIGEST_A_ODIGEST_A_BIT_END               31U

/* Definition for field ODIGEST_B in Register P_HASH512_ODIGEST_B */
#define P_HASH512_ODIGEST_B_ODIGEST_B_BIT_START             0U
#define P_HASH512_ODIGEST_B_ODIGEST_B_BIT_END               31U

/* Definition for field ODIGEST_C in Register P_HASH512_ODIGEST_C */
#define P_HASH512_ODIGEST_C_ODIGEST_C_BIT_START             0U
#define P_HASH512_ODIGEST_C_ODIGEST_C_BIT_END               31U

/* Definition for field ODIGEST_D in Register P_HASH512_ODIGEST_D */
#define P_HASH512_ODIGEST_D_ODIGEST_D_BIT_START             0U
#define P_HASH512_ODIGEST_D_ODIGEST_D_BIT_END               31U

/* Definition for field ODIGEST_E in Register P_HASH512_ODIGEST_E */
#define P_HASH512_ODIGEST_E_ODIGEST_E_BIT_START             0U
#define P_HASH512_ODIGEST_E_ODIGEST_E_BIT_END               31U

/* Definition for field ODIGEST_F in Register P_HASH512_ODIGEST_F */
#define P_HASH512_ODIGEST_F_ODIGEST_F_BIT_START             0U
#define P_HASH512_ODIGEST_F_ODIGEST_F_BIT_END               31U

/* Definition for field ODIGEST_G in Register P_HASH512_ODIGEST_G */
#define P_HASH512_ODIGEST_G_ODIGEST_G_BIT_START             0U
#define P_HASH512_ODIGEST_G_ODIGEST_G_BIT_END               31U

/* Definition for field ODIGEST_H in Register P_HASH512_ODIGEST_H */
#define P_HASH512_ODIGEST_H_ODIGEST_H_BIT_START             0U
#define P_HASH512_ODIGEST_H_ODIGEST_H_BIT_END               31U

/* Definition for field ODIGEST_I in Register P_HASH512_ODIGEST_I */
#define P_HASH512_ODIGEST_I_ODIGEST_I_BIT_START             0U
#define P_HASH512_ODIGEST_I_ODIGEST_I_BIT_END               31U

/* Definition for field ODIGEST_J in Register P_HASH512_ODIGEST_J */
#define P_HASH512_ODIGEST_J_ODIGEST_J_BIT_START             0U
#define P_HASH512_ODIGEST_J_ODIGEST_J_BIT_END               31U

/* Definition for field ODIGEST_K in Register P_HASH512_ODIGEST_K */
#define P_HASH512_ODIGEST_K_ODIGEST_K_BIT_START             0U
#define P_HASH512_ODIGEST_K_ODIGEST_K_BIT_END               31U

/* Definition for field ODIGEST_L in Register P_HASH512_ODIGEST_L */
#define P_HASH512_ODIGEST_L_ODIGEST_L_BIT_START             0U
#define P_HASH512_ODIGEST_L_ODIGEST_L_BIT_END               31U

/* Definition for field ODIGEST_M in Register P_HASH512_ODIGEST_M */
#define P_HASH512_ODIGEST_M_ODIGEST_M_BIT_START             0U
#define P_HASH512_ODIGEST_M_ODIGEST_M_BIT_END               31U

/* Definition for field ODIGEST_N in Register P_HASH512_ODIGEST_N */
#define P_HASH512_ODIGEST_N_ODIGEST_N_BIT_START             0U
#define P_HASH512_ODIGEST_N_ODIGEST_N_BIT_END               31U

/* Definition for field ODIGEST_O in Register P_HASH512_ODIGEST_O */
#define P_HASH512_ODIGEST_O_ODIGEST_O_BIT_START             0U
#define P_HASH512_ODIGEST_O_ODIGEST_O_BIT_END               31U

/* Definition for field ODIGEST_P in Register P_HASH512_ODIGEST_P */
#define P_HASH512_ODIGEST_P_ODIGEST_P_BIT_START             0U
#define P_HASH512_ODIGEST_P_ODIGEST_P_BIT_END               31U

/* Definition for field IDIGEST_A in Register P_HASH512_IDIGEST_A */
#define P_HASH512_IDIGEST_A_IDIGEST_A_BIT_START             0U
#define P_HASH512_IDIGEST_A_IDIGEST_A_BIT_END               31U

/* Definition for field IDIGEST_B in Register P_HASH512_IDIGEST_B */
#define P_HASH512_IDIGEST_B_IDIGEST_B_BIT_START             0U
#define P_HASH512_IDIGEST_B_IDIGEST_B_BIT_END               31U

/* Definition for field IDIGEST_C in Register P_HASH512_IDIGEST_C */
#define P_HASH512_IDIGEST_C_IDIGEST_C_BIT_START             0U
#define P_HASH512_IDIGEST_C_IDIGEST_C_BIT_END               31U

/* Definition for field IDIGEST_D in Register P_HASH512_IDIGEST_D */
#define P_HASH512_IDIGEST_D_IDIGEST_D_BIT_START             0U
#define P_HASH512_IDIGEST_D_IDIGEST_D_BIT_END               31U

/* Definition for field IDIGEST_E in Register P_HASH512_IDIGEST_E */
#define P_HASH512_IDIGEST_E_IDIGEST_E_BIT_START             0U
#define P_HASH512_IDIGEST_E_IDIGEST_E_BIT_END               31U

/* Definition for field IDIGEST_F in Register P_HASH512_IDIGEST_F */
#define P_HASH512_IDIGEST_F_IDIGEST_F_BIT_START             0U
#define P_HASH512_IDIGEST_F_IDIGEST_F_BIT_END               31U

/* Definition for field IDIGEST_G in Register P_HASH512_IDIGEST_G */
#define P_HASH512_IDIGEST_G_IDIGEST_G_BIT_START             0U
#define P_HASH512_IDIGEST_G_IDIGEST_G_BIT_END               31U

/* Definition for field IDIGEST_H in Register P_HASH512_IDIGEST_H */
#define P_HASH512_IDIGEST_H_IDIGEST_H_BIT_START             0U
#define P_HASH512_IDIGEST_H_IDIGEST_H_BIT_END               31U

/* Definition for field IDIGEST_I in Register P_HASH512_IDIGEST_I */
#define P_HASH512_IDIGEST_I_IDIGEST_I_BIT_START             0U
#define P_HASH512_IDIGEST_I_IDIGEST_I_BIT_END               31U

/* Definition for field IDIGEST_J in Register P_HASH512_IDIGEST_J */
#define P_HASH512_IDIGEST_J_IDIGEST_J_BIT_START             0U
#define P_HASH512_IDIGEST_J_IDIGEST_J_BIT_END               31U

/* Definition for field IDIGEST_K in Register P_HASH512_IDIGEST_K */
#define P_HASH512_IDIGEST_K_IDIGEST_K_BIT_START             0U
#define P_HASH512_IDIGEST_K_IDIGEST_K_BIT_END               31U

/* Definition for field IDIGEST_L in Register P_HASH512_IDIGEST_L */
#define P_HASH512_IDIGEST_L_IDIGEST_L_BIT_START             0U
#define P_HASH512_IDIGEST_L_IDIGEST_L_BIT_END               31U

/* Definition for field IDIGEST_M in Register P_HASH512_IDIGEST_M */
#define P_HASH512_IDIGEST_M_IDIGEST_M_BIT_START             0U
#define P_HASH512_IDIGEST_M_IDIGEST_M_BIT_END               31U

/* Definition for field IDIGEST_N in Register P_HASH512_IDIGEST_N */
#define P_HASH512_IDIGEST_N_IDIGEST_N_BIT_START             0U
#define P_HASH512_IDIGEST_N_IDIGEST_N_BIT_END               31U

/* Definition for field IDIGEST_O in Register P_HASH512_IDIGEST_O */
#define P_HASH512_IDIGEST_O_IDIGEST_O_BIT_START             0U
#define P_HASH512_IDIGEST_O_IDIGEST_O_BIT_END               31U

/* Definition for field IDIGEST_P in Register P_HASH512_IDIGEST_P */
#define P_HASH512_IDIGEST_P_IDIGEST_P_BIT_START             0U
#define P_HASH512_IDIGEST_P_IDIGEST_P_BIT_END               31U

/* Definition for field DIGEST_COUNT in Register P_HASH512_DIGEST_COUNT */
#define P_HASH512_DIGEST_COUNT_DIGEST_COUNT_BIT_START       0U
#define P_HASH512_DIGEST_COUNT_DIGEST_COUNT_BIT_END         31U

/* Definition for field ALGORITHM in Register P_HASH512_MODE */
#define P_HASH512_MODE_ALGORITHM_BIT_START                  0U
#define P_HASH512_MODE_ALGORITHM_BIT_END                    2U

/* Definition for field USE_ALG_CONSTANTS in Register P_HASH512_MODE */
#define P_HASH512_MODE_USE_ALG_CONSTANTS_BIT_START          3U
#define P_HASH512_MODE_USE_ALG_CONSTANTS_BIT_END            3U

/* Definition for field CLOSE_HASH in Register P_HASH512_MODE */
#define P_HASH512_MODE_CLOSE_HASH_BIT_START                 4U
#define P_HASH512_MODE_CLOSE_HASH_BIT_END                   4U

/* Definition for field HMAC_KEY_PROCESSING in Register P_HASH512_MODE */
#define P_HASH512_MODE_HMAC_KEY_PROCESSING_BIT_START        5U
#define P_HASH512_MODE_HMAC_KEY_PROCESSING_BIT_END          5U

/* Definition for field REUSE_HMAC_KEY in Register P_HASH512_MODE */
#define P_HASH512_MODE_REUSE_HMAC_KEY_BIT_START             6U
#define P_HASH512_MODE_REUSE_HMAC_KEY_BIT_END               6U

/* Definition for field HMAC_OUTER_HASH in Register P_HASH512_MODE */
#define P_HASH512_MODE_HMAC_OUTER_HASH_BIT_START            7U
#define P_HASH512_MODE_HMAC_OUTER_HASH_BIT_END              7U

/* Definition for field LENGTH in Register P_HASH512_LENGTH */
#define P_HASH512_LENGTH_LENGTH_BIT_START                   0U
#define P_HASH512_LENGTH_LENGTH_BIT_END                     31U



/**
 * @struct SHARegs_t
 * @brief
 *   Module MSS_SHA Register Definition
 * @details
 *   This structure is used to access the MSS_SHA module registers.
 */
/**
 * @typedef SHARegs
 * @brief
 *   Module MSS_SHA Register Frame type Definition
 * @details
 *   This type is used to access the MSS_SHA module registers.
 */
typedef volatile struct SHARegs_t
{
    uint32_t    S_ODIGEST_A                        ;        /* Offset = 0x000 */
    uint32_t    S_ODIGEST_B                        ;        /* Offset = 0x004 */
    uint32_t    S_ODIGEST_C                        ;        /* Offset = 0x008 */
    uint32_t    S_ODIGEST_D                        ;        /* Offset = 0x00C */
    uint32_t    S_ODIGEST_E                        ;        /* Offset = 0x010 */
    uint32_t    S_ODIGEST_F                        ;        /* Offset = 0x014 */
    uint32_t    S_ODIGEST_G                        ;        /* Offset = 0x018 */
    uint32_t    S_ODIGEST_H                        ;        /* Offset = 0x01C */
    uint32_t    S_IDIGEST_A                        ;        /* Offset = 0x020 */
    uint32_t    S_IDIGEST_B                        ;        /* Offset = 0x024 */
    uint32_t    S_IDIGEST_C                        ;        /* Offset = 0x028 */
    uint32_t    S_IDIGEST_D                        ;        /* Offset = 0x02C */
    uint32_t    S_IDIGEST_E                        ;        /* Offset = 0x030 */
    uint32_t    S_IDIGEST_F                        ;        /* Offset = 0x034 */
    uint32_t    S_IDIGEST_G                        ;        /* Offset = 0x038 */
    uint32_t    S_IDIGEST_H                        ;        /* Offset = 0x03C */
    uint32_t    S_DIGEST_COUNT                     ;        /* Offset = 0x040 */
    uint32_t    S_MODE                             ;        /* Offset = 0x044 */
    uint32_t    S_LENGTH                           ;        /* Offset = 0x048 */
    uint32_t    RESTRICTED1[13]                    ;        /* Offset = 0x04C */
    uint32_t    S_DATA0_IN                         ;        /* Offset = 0x080 */
    uint32_t    S_DATA1_IN                         ;        /* Offset = 0x084 */
    uint32_t    S_DATA2_IN                         ;        /* Offset = 0x088 */
    uint32_t    S_DATA3_IN                         ;        /* Offset = 0x08C */
    uint32_t    S_DATA4_IN                         ;        /* Offset = 0x090 */
    uint32_t    S_DATA5_IN                         ;        /* Offset = 0x094 */
    uint32_t    S_DATA6_IN                         ;        /* Offset = 0x098 */
    uint32_t    S_DATA7_IN                         ;        /* Offset = 0x09C */
    uint32_t    S_DATA8_IN                         ;        /* Offset = 0x0A0 */
    uint32_t    S_DATA9_IN                         ;        /* Offset = 0x0A4 */
    uint32_t    S_DATA10_IN                        ;        /* Offset = 0x0A8 */
    uint32_t    S_DATA11_IN                        ;        /* Offset = 0x0AC */
    uint32_t    S_DATA12_IN                        ;        /* Offset = 0x0B0 */
    uint32_t    S_DATA13_IN                        ;        /* Offset = 0x0B4 */
    uint32_t    S_DATA14_IN                        ;        /* Offset = 0x0B8 */
    uint32_t    S_DATA15_IN                        ;        /* Offset = 0x0BC */
    uint32_t    RESTRICTED2[16]                    ;        /* Offset = 0x0C0 */
    uint32_t    S_REVISION                         ;        /* Offset = 0x100 */
    uint32_t    RESTRICTED3[3]                     ;        /* Offset = 0x104 */
    uint32_t    S_SYSCONFIG                        ;        /* Offset = 0x110 */
    uint32_t    S_SYSSTATUS                        ;        /* Offset = 0x114 */
    uint32_t    S_IRQSTATUS                        ;        /* Offset = 0x118 */
    uint32_t    S_IRQENABLE                        ;        /* Offset = 0x11C */
    uint32_t    RESTRICTED4[8]                     ;        /* Offset = 0x120 */
    uint32_t    S_XSSTATUS                         ;        /* Offset = 0x140 */
    uint32_t    RESTRICTED5[2]                     ;        /* Offset = 0x144 */
    uint32_t    S_LOCKDOWN                         ;        /* Offset = 0x14C */
    uint32_t    RESTRICTED6[44]                    ;        /* Offset = 0x150 */
    uint32_t    S_HASH512_ODIGEST_A                ;        /* Offset = 0x200 */
    uint32_t    S_HASH512_ODIGEST_B                ;        /* Offset = 0x204 */
    uint32_t    S_HASH512_ODIGEST_C                ;        /* Offset = 0x208 */
    uint32_t    S_HASH512_ODIGEST_D                ;        /* Offset = 0x20C */
    uint32_t    S_HASH512_ODIGEST_E                ;        /* Offset = 0x210 */
    uint32_t    S_HASH512_ODIGEST_F                ;        /* Offset = 0x214 */
    uint32_t    S_HASH512_ODIGEST_G                ;        /* Offset = 0x218 */
    uint32_t    S_HASH512_ODIGEST_H                ;        /* Offset = 0x21C */
    uint32_t    S_HASH512_ODIGEST_I                ;        /* Offset = 0x220 */
    uint32_t    S_HASH512_ODIGEST_J                ;        /* Offset = 0x224 */
    uint32_t    S_HASH512_ODIGEST_K                ;        /* Offset = 0x228 */
    uint32_t    S_HASH512_ODIGEST_L                ;        /* Offset = 0x22C */
    uint32_t    S_HASH512_ODIGEST_M                ;        /* Offset = 0x230 */
    uint32_t    S_HASH512_ODIGEST_N                ;        /* Offset = 0x234 */
    uint32_t    S_HASH512_ODIGEST_O                ;        /* Offset = 0x238 */
    uint32_t    S_HASH512_ODIGEST_P                ;        /* Offset = 0x23C */
    uint32_t    S_HASH512_IDIGEST_A                ;        /* Offset = 0x240 */
    uint32_t    S_HASH512_IDIGEST_B                ;        /* Offset = 0x244 */
    uint32_t    S_HASH512_IDIGEST_C                ;        /* Offset = 0x248 */
    uint32_t    S_HASH512_IDIGEST_D                ;        /* Offset = 0x24C */
    uint32_t    S_HASH512_IDIGEST_E                ;        /* Offset = 0x250 */
    uint32_t    S_HASH512_IDIGEST_F                ;        /* Offset = 0x254 */
    uint32_t    S_HASH512_IDIGEST_G                ;        /* Offset = 0x258 */
    uint32_t    S_HASH512_IDIGEST_H                ;        /* Offset = 0x25C */
    uint32_t    S_HASH512_IDIGEST_I                ;        /* Offset = 0x260 */
    uint32_t    S_HASH512_IDIGEST_J                ;        /* Offset = 0x264 */
    uint32_t    S_HASH512_IDIGEST_K                ;        /* Offset = 0x268 */
    uint32_t    S_HASH512_IDIGEST_L                ;        /* Offset = 0x26C */
    uint32_t    S_HASH512_IDIGEST_M                ;        /* Offset = 0x270 */
    uint32_t    S_HASH512_IDIGEST_N                ;        /* Offset = 0x274 */
    uint32_t    S_HASH512_IDIGEST_O                ;        /* Offset = 0x278 */
    uint32_t    S_HASH512_IDIGEST_P                ;        /* Offset = 0x27C */
    uint32_t    S_HASH512_DIGEST_COUNT             ;        /* Offset = 0x280 */
    uint32_t    S_HASH512_MODE                     ;        /* Offset = 0x284 */
    uint32_t    S_HASH512_LENGTH                   ;        /* Offset = 0x288 */
    uint32_t    RESTRICTED7[861]                   ;        /* Offset = 0x28C */
    uint32_t    P_ODIGEST_A                        ;        /* Offset = 0x1000 */
    uint32_t    P_ODIGEST_B                        ;        /* Offset = 0x1004 */
    uint32_t    P_ODIGEST_C                        ;        /* Offset = 0x1008 */
    uint32_t    P_ODIGEST_D                        ;        /* Offset = 0x100C */
    uint32_t    P_ODIGEST_E                        ;        /* Offset = 0x1010 */
    uint32_t    P_ODIGEST_F                        ;        /* Offset = 0x1014 */
    uint32_t    P_ODIGEST_G                        ;        /* Offset = 0x1018 */
    uint32_t    P_ODIGEST_H                        ;        /* Offset = 0x101C */
    uint32_t    P_IDIGEST_A                        ;        /* Offset = 0x1020 */
    uint32_t    P_IDIGEST_B                        ;        /* Offset = 0x1024 */
    uint32_t    P_IDIGEST_C                        ;        /* Offset = 0x1028 */
    uint32_t    P_IDIGEST_D                        ;        /* Offset = 0x102C */
    uint32_t    P_IDIGEST_E                        ;        /* Offset = 0x1030 */
    uint32_t    P_IDIGEST_F                        ;        /* Offset = 0x1034 */
    uint32_t    P_IDIGEST_G                        ;        /* Offset = 0x1038 */
    uint32_t    P_IDIGEST_H                        ;        /* Offset = 0x103C */
    uint32_t    P_DIGEST_COUNT                     ;        /* Offset = 0x1040 */
    uint32_t    P_MODE                             ;        /* Offset = 0x1044 */
    uint32_t    P_LENGTH                           ;        /* Offset = 0x1048 */
    uint32_t    RESTRICTED8[13]                    ;        /* Offset = 0x104C */
    uint32_t    P_DATA0_IN                         ;        /* Offset = 0x1080 */
    uint32_t    P_DATA1_IN                         ;        /* Offset = 0x1084 */
    uint32_t    P_DATA2_IN                         ;        /* Offset = 0x1088 */
    uint32_t    P_DATA3_IN                         ;        /* Offset = 0x108C */
    uint32_t    P_DATA4_IN                         ;        /* Offset = 0x1090 */
    uint32_t    P_DATA5_IN                         ;        /* Offset = 0x1094 */
    uint32_t    P_DATA6_IN                         ;        /* Offset = 0x1098 */
    uint32_t    P_DATA7_IN                         ;        /* Offset = 0x109C */
    uint32_t    P_DATA8_IN                         ;        /* Offset = 0x10A0 */
    uint32_t    P_DATA9_IN                         ;        /* Offset = 0x10A4 */
    uint32_t    P_DATA10_IN                        ;        /* Offset = 0x10A8 */
    uint32_t    P_DATA11_IN                        ;        /* Offset = 0x10AC */
    uint32_t    P_DATA12_IN                        ;        /* Offset = 0x10B0 */
    uint32_t    P_DATA13_IN                        ;        /* Offset = 0x10B4 */
    uint32_t    P_DATA14_IN                        ;        /* Offset = 0x10B8 */
    uint32_t    P_DATA15_IN                        ;        /* Offset = 0x10BC */
    uint32_t    RESTRICTED9[16]                    ;        /* Offset = 0x10C0 */
    uint32_t    P_REVISION                         ;        /* Offset = 0x1100 */
    uint32_t    RESTRICTED10[3]                    ;        /* Offset = 0x1104 */
    uint32_t    P_SYSCONFIG                        ;        /* Offset = 0x1110 */
    uint32_t    P_SYSSTATUS                        ;        /* Offset = 0x1114 */
    uint32_t    P_IRQSTATUS                        ;        /* Offset = 0x1118 */
    uint32_t    P_IRQENABLE                        ;        /* Offset = 0x111C */
    uint32_t    RESTRICTED11[56]                   ;        /* Offset = 0x1120 */
    uint32_t    P_HASH512_ODIGEST_A                ;        /* Offset = 0x1200 */
    uint32_t    P_HASH512_ODIGEST_B                ;        /* Offset = 0x1204 */
    uint32_t    P_HASH512_ODIGEST_C                ;        /* Offset = 0x1208 */
    uint32_t    P_HASH512_ODIGEST_D                ;        /* Offset = 0x120C */
    uint32_t    P_HASH512_ODIGEST_E                ;        /* Offset = 0x1210 */
    uint32_t    P_HASH512_ODIGEST_F                ;        /* Offset = 0x1214 */
    uint32_t    P_HASH512_ODIGEST_G                ;        /* Offset = 0x1218 */
    uint32_t    P_HASH512_ODIGEST_H                ;        /* Offset = 0x121C */
    uint32_t    P_HASH512_ODIGEST_I                ;        /* Offset = 0x1220 */
    uint32_t    P_HASH512_ODIGEST_J                ;        /* Offset = 0x1224 */
    uint32_t    P_HASH512_ODIGEST_K                ;        /* Offset = 0x1228 */
    uint32_t    P_HASH512_ODIGEST_L                ;        /* Offset = 0x122C */
    uint32_t    P_HASH512_ODIGEST_M                ;        /* Offset = 0x1230 */
    uint32_t    P_HASH512_ODIGEST_N                ;        /* Offset = 0x1234 */
    uint32_t    P_HASH512_ODIGEST_O                ;        /* Offset = 0x1238 */
    uint32_t    P_HASH512_ODIGEST_P                ;        /* Offset = 0x123C */
    uint32_t    P_HASH512_IDIGEST_A                ;        /* Offset = 0x1240 */
    uint32_t    P_HASH512_IDIGEST_B                ;        /* Offset = 0x1244 */
    uint32_t    P_HASH512_IDIGEST_C                ;        /* Offset = 0x1248 */
    uint32_t    P_HASH512_IDIGEST_D                ;        /* Offset = 0x124C */
    uint32_t    P_HASH512_IDIGEST_E                ;        /* Offset = 0x1250 */
    uint32_t    P_HASH512_IDIGEST_F                ;        /* Offset = 0x1254 */
    uint32_t    P_HASH512_IDIGEST_G                ;        /* Offset = 0x1258 */
    uint32_t    P_HASH512_IDIGEST_H                ;        /* Offset = 0x125C */
    uint32_t    P_HASH512_IDIGEST_I                ;        /* Offset = 0x1260 */
    uint32_t    P_HASH512_IDIGEST_J                ;        /* Offset = 0x1264 */
    uint32_t    P_HASH512_IDIGEST_K                ;        /* Offset = 0x1268 */
    uint32_t    P_HASH512_IDIGEST_L                ;        /* Offset = 0x126C */
    uint32_t    P_HASH512_IDIGEST_M                ;        /* Offset = 0x1270 */
    uint32_t    P_HASH512_IDIGEST_N                ;        /* Offset = 0x1274 */
    uint32_t    P_HASH512_IDIGEST_O                ;        /* Offset = 0x1278 */
    uint32_t    P_HASH512_IDIGEST_P                ;        /* Offset = 0x127C */
    uint32_t    P_HASH512_DIGEST_COUNT             ;        /* Offset = 0x1280 */
    uint32_t    P_HASH512_MODE                     ;        /* Offset = 0x1284 */
    uint32_t    P_HASH512_LENGTH                   ;        /* Offset = 0x1288 */
} SHARegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_SHA_H */
/* END OF REG_SHA_H */

