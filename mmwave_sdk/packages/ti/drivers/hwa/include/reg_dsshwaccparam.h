/**
 *  @file    reg_dsshwaccparam.h
 *
 *  @brief
 *    This file gives register definitions of DSS_HW_ACC_PARAM module.
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

#ifndef REG_DSSHWACCPARAM_H
#define REG_DSSHWACCPARAM_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 1
/* Definition for field TRIGMODE in Register PARAMn_0 */
#define PARAMn_0_TRIGMODE_BIT_START                         0U
#define PARAMn_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAMn_0 */
#define PARAMn_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAMn_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAMn_0 */
#define PARAMn_0_CR4INTREN_BIT_START                        7U
#define PARAMn_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAMn_0 */
#define PARAMn_0_DMATRIGEN_BIT_START                        8U
#define PARAMn_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAMn_0 */
#define PARAMn_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAMn_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAMn_0 */
#define PARAMn_0_BPM_EN_BIT_START                           13U
#define PARAMn_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAMn_0 */
#define PARAMn_0_FFT_EN_BIT_START                           14U
#define PARAMn_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAMn_0 */
#define PARAMn_0_WINDOW_EN_BIT_START                        15U
#define PARAMn_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAMn_0 */
#define PARAMn_0_LOG2EN_BIT_START                           16U
#define PARAMn_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAMn_0 */
#define PARAMn_0_ABSEN_BIT_START                            17U
#define PARAMn_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAMn_0 */
#define PARAMn_0_CMULT_MODE_BIT_START                       18U
#define PARAMn_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAMn_0 */
#define PARAMn_0_ACCEL_MODE_BIT_START                       21U
#define PARAMn_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAMn_0 */
#define PARAMn_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAMn_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAMn_0 */
#define PARAMn_0_NU_BIT_START                               25U
#define PARAMn_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAMn_1 */
#define PARAMn_1_SRCADDR_BIT_START                          0U
#define PARAMn_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAMn_1 */
#define PARAMn_1_DSTADDR_BIT_START                          16U
#define PARAMn_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAMn_2 */
#define PARAMn_2_SRCACNT_BIT_START                          0U
#define PARAMn_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAMn_2 */
#define PARAMn_2_SRCREAL_BIT_START                          12U
#define PARAMn_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAMn_2 */
#define PARAMn_2_SRC16B32B_BIT_START                        13U
#define PARAMn_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAMn_2 */
#define PARAMn_2_SRCSIGNED_BIT_START                        14U
#define PARAMn_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAMn_2 */
#define PARAMn_2_SRCCONJ_BIT_START                          15U
#define PARAMn_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAMn_2 */
#define PARAMn_2_DSTACNT_BIT_START                          16U
#define PARAMn_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAMn_2 */
#define PARAMn_2_DSTREAL_BIT_START                          28U
#define PARAMn_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAMn_2 */
#define PARAMn_2_DST16B32B_BIT_START                        29U
#define PARAMn_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAMn_2 */
#define PARAMn_2_DSTSIGNED_BIT_START                        30U
#define PARAMn_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAMn_2 */
#define PARAMn_2_DSTCONJ_BIT_START                          31U
#define PARAMn_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAMn_3 */
#define PARAMn_3_SRCAINDX_BIT_START                         0U
#define PARAMn_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAMn_3 */
#define PARAMn_3_DSTAINDX_BIT_START                         16U
#define PARAMn_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAMn_4 */
#define PARAMn_4_SRCBINDX_BIT_START                         0U
#define PARAMn_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAMn_4 */
#define PARAMn_4_DSTBINDX_BIT_START                         16U
#define PARAMn_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAMn_5 */
#define PARAMn_5_REG_BCNT_BIT_START                         0U
#define PARAMn_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAMn_5 */
#define PARAMn_5_REG_SRCSCAL_BIT_START                      12U
#define PARAMn_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAMn_5 */
#define PARAMn_5_REG_DSTSCAL_BIT_START                      16U
#define PARAMn_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAMn_5 */
#define PARAMn_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAMn_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAMn_5 */
#define PARAMn_5_NU_BIT_START                               30U
#define PARAMn_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAMn_6 */
#define PARAMn_6_DSTWIDTH_BIT_START                         0U
#define PARAMn_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAMn_6 */
#define PARAMn_6_FFTSIZE_BIT_START                          2U
#define PARAMn_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAMn_6 */
#define PARAMn_6_WINSYMM_BIT_START                          6U
#define PARAMn_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAMn_6 */
#define PARAMn_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAMn_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAMn_6 */
#define PARAMn_6_BPMPHASE_BIT_START                         8U
#define PARAMn_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAMn_6 */
#define PARAMn_6_WINDOW_START_BIT_START                     12U
#define PARAMn_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAMn_6 */
#define PARAMn_6_BFLY_SCALING_BIT_START                     22U
#define PARAMn_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAMn_7 */
#define PARAMn_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAMn_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAMn_7 */
#define PARAMn_7_TWIDINCR_BIT_START                         12U
#define PARAMn_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAMn_7 */
#define PARAMn_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAMn_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAMn_7 */
#define PARAMn_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAMn_7_CIRCSHIFTWRAP_BIT_END                      31U

/**
 * @struct DSSHWACCPARAMRegs_t
 * @brief
 *   Module DSS_HW_ACC_PARAM Register Definition
 * @details
 *   This structure is used to access the DSS_HW_ACC_PARAM module registers.
 */
/**
 * @typedef DSSHWACCPARAMRegs
 * @brief
 *   Module DSS_HW_ACC_PARAM Register Frame type Definition
 * @details
 *   This type is used to access the DSS_HW_ACC_PARAM module registers.
 */
typedef volatile struct DSSHWACCPARAMRegs_t
{
    uint32_t    PARAMn_0   ;        /* AddrOffset = 0x000 */
    uint32_t    PARAMn_1   ;        /* AddrOffset = 0x004 */
    uint32_t    PARAMn_2   ;        /* AddrOffset = 0x008 */
    uint32_t    PARAMn_3   ;        /* AddrOffset = 0x00C */
    uint32_t    PARAMn_4   ;        /* AddrOffset = 0x010 */
    uint32_t    PARAMn_5   ;        /* AddrOffset = 0x014 */
    uint32_t    PARAMn_6   ;        /* AddrOffset = 0x018 */
    uint32_t    PARAMn_7   ;        /* AddrOffset = 0x01C */
} DSSHWACCPARAMRegs;

#else
/* Definition for field TRIGMODE in Register PARAM1_0 */
#define PARAM1_0_TRIGMODE_BIT_START                         0U
#define PARAM1_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM1_0 */
#define PARAM1_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM1_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM1_0 */
#define PARAM1_0_CR4INTREN_BIT_START                        7U
#define PARAM1_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM1_0 */
#define PARAM1_0_DMATRIGEN_BIT_START                        8U
#define PARAM1_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM1_0 */
#define PARAM1_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM1_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM1_0 */
#define PARAM1_0_BPM_EN_BIT_START                           13U
#define PARAM1_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM1_0 */
#define PARAM1_0_FFT_EN_BIT_START                           14U
#define PARAM1_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM1_0 */
#define PARAM1_0_WINDOW_EN_BIT_START                        15U
#define PARAM1_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM1_0 */
#define PARAM1_0_LOG2EN_BIT_START                           16U
#define PARAM1_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM1_0 */
#define PARAM1_0_ABSEN_BIT_START                            17U
#define PARAM1_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM1_0 */
#define PARAM1_0_CMULT_MODE_BIT_START                       18U
#define PARAM1_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM1_0 */
#define PARAM1_0_ACCEL_MODE_BIT_START                       21U
#define PARAM1_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM1_0 */
#define PARAM1_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM1_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM1_0 */
#define PARAM1_0_NU_BIT_START                               25U
#define PARAM1_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM1_1 */
#define PARAM1_1_SRCADDR_BIT_START                          0U
#define PARAM1_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM1_1 */
#define PARAM1_1_DSTADDR_BIT_START                          16U
#define PARAM1_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM1_2 */
#define PARAM1_2_SRCACNT_BIT_START                          0U
#define PARAM1_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM1_2 */
#define PARAM1_2_SRCREAL_BIT_START                          12U
#define PARAM1_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM1_2 */
#define PARAM1_2_SRC16B32B_BIT_START                        13U
#define PARAM1_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM1_2 */
#define PARAM1_2_SRCSIGNED_BIT_START                        14U
#define PARAM1_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM1_2 */
#define PARAM1_2_SRCCONJ_BIT_START                          15U
#define PARAM1_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM1_2 */
#define PARAM1_2_DSTACNT_BIT_START                          16U
#define PARAM1_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM1_2 */
#define PARAM1_2_DSTREAL_BIT_START                          28U
#define PARAM1_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM1_2 */
#define PARAM1_2_DST16B32B_BIT_START                        29U
#define PARAM1_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM1_2 */
#define PARAM1_2_DSTSIGNED_BIT_START                        30U
#define PARAM1_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM1_2 */
#define PARAM1_2_DSTCONJ_BIT_START                          31U
#define PARAM1_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM1_3 */
#define PARAM1_3_SRCAINDX_BIT_START                         0U
#define PARAM1_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM1_3 */
#define PARAM1_3_DSTAINDX_BIT_START                         16U
#define PARAM1_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM1_4 */
#define PARAM1_4_SRCBINDX_BIT_START                         0U
#define PARAM1_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM1_4 */
#define PARAM1_4_DSTBINDX_BIT_START                         16U
#define PARAM1_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM1_5 */
#define PARAM1_5_REG_BCNT_BIT_START                         0U
#define PARAM1_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM1_5 */
#define PARAM1_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM1_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM1_5 */
#define PARAM1_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM1_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM1_5 */
#define PARAM1_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM1_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM1_5 */
#define PARAM1_5_NU_BIT_START                               30U
#define PARAM1_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM1_6 */
#define PARAM1_6_DSTWIDTH_BIT_START                         0U
#define PARAM1_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM1_6 */
#define PARAM1_6_FFTSIZE_BIT_START                          2U
#define PARAM1_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM1_6 */
#define PARAM1_6_WINSYMM_BIT_START                          6U
#define PARAM1_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM1_6 */
#define PARAM1_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM1_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM1_6 */
#define PARAM1_6_BPMPHASE_BIT_START                         8U
#define PARAM1_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM1_6 */
#define PARAM1_6_WINDOW_START_BIT_START                     12U
#define PARAM1_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM1_6 */
#define PARAM1_6_BFLY_SCALING_BIT_START                     22U
#define PARAM1_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM1_7 */
#define PARAM1_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM1_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM1_7 */
#define PARAM1_7_TWIDINCR_BIT_START                         12U
#define PARAM1_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM1_7 */
#define PARAM1_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM1_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM1_7 */
#define PARAM1_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM1_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM2_0 */
#define PARAM2_0_TRIGMODE_BIT_START                         0U
#define PARAM2_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM2_0 */
#define PARAM2_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM2_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM2_0 */
#define PARAM2_0_CR4INTREN_BIT_START                        7U
#define PARAM2_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM2_0 */
#define PARAM2_0_DMATRIGEN_BIT_START                        8U
#define PARAM2_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM2_0 */
#define PARAM2_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM2_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM2_0 */
#define PARAM2_0_BPM_EN_BIT_START                           13U
#define PARAM2_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM2_0 */
#define PARAM2_0_FFT_EN_BIT_START                           14U
#define PARAM2_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM2_0 */
#define PARAM2_0_WINDOW_EN_BIT_START                        15U
#define PARAM2_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM2_0 */
#define PARAM2_0_LOG2EN_BIT_START                           16U
#define PARAM2_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM2_0 */
#define PARAM2_0_ABSEN_BIT_START                            17U
#define PARAM2_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM2_0 */
#define PARAM2_0_CMULT_MODE_BIT_START                       18U
#define PARAM2_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM2_0 */
#define PARAM2_0_ACCEL_MODE_BIT_START                       21U
#define PARAM2_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM2_0 */
#define PARAM2_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM2_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM2_0 */
#define PARAM2_0_NU_BIT_START                               25U
#define PARAM2_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM2_1 */
#define PARAM2_1_SRCADDR_BIT_START                          0U
#define PARAM2_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM2_1 */
#define PARAM2_1_DSTADDR_BIT_START                          16U
#define PARAM2_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM2_2 */
#define PARAM2_2_SRCACNT_BIT_START                          0U
#define PARAM2_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM2_2 */
#define PARAM2_2_SRCREAL_BIT_START                          12U
#define PARAM2_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM2_2 */
#define PARAM2_2_SRC16B32B_BIT_START                        13U
#define PARAM2_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM2_2 */
#define PARAM2_2_SRCSIGNED_BIT_START                        14U
#define PARAM2_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM2_2 */
#define PARAM2_2_SRCCONJ_BIT_START                          15U
#define PARAM2_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM2_2 */
#define PARAM2_2_DSTACNT_BIT_START                          16U
#define PARAM2_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM2_2 */
#define PARAM2_2_DSTREAL_BIT_START                          28U
#define PARAM2_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM2_2 */
#define PARAM2_2_DST16B32B_BIT_START                        29U
#define PARAM2_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM2_2 */
#define PARAM2_2_DSTSIGNED_BIT_START                        30U
#define PARAM2_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM2_2 */
#define PARAM2_2_DSTCONJ_BIT_START                          31U
#define PARAM2_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM2_3 */
#define PARAM2_3_SRCAINDX_BIT_START                         0U
#define PARAM2_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM2_3 */
#define PARAM2_3_DSTAINDX_BIT_START                         16U
#define PARAM2_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM2_4 */
#define PARAM2_4_SRCBINDX_BIT_START                         0U
#define PARAM2_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM2_4 */
#define PARAM2_4_DSTBINDX_BIT_START                         16U
#define PARAM2_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM2_5 */
#define PARAM2_5_REG_BCNT_BIT_START                         0U
#define PARAM2_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM2_5 */
#define PARAM2_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM2_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM2_5 */
#define PARAM2_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM2_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM2_5 */
#define PARAM2_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM2_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM2_5 */
#define PARAM2_5_NU_BIT_START                               30U
#define PARAM2_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM2_6 */
#define PARAM2_6_DSTWIDTH_BIT_START                         0U
#define PARAM2_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM2_6 */
#define PARAM2_6_FFTSIZE_BIT_START                          2U
#define PARAM2_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM2_6 */
#define PARAM2_6_WINSYMM_BIT_START                          6U
#define PARAM2_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM2_6 */
#define PARAM2_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM2_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM2_6 */
#define PARAM2_6_BPMPHASE_BIT_START                         8U
#define PARAM2_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM2_6 */
#define PARAM2_6_WINDOW_START_BIT_START                     12U
#define PARAM2_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM2_6 */
#define PARAM2_6_BFLY_SCALING_BIT_START                     22U
#define PARAM2_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM2_7 */
#define PARAM2_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM2_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM2_7 */
#define PARAM2_7_TWIDINCR_BIT_START                         12U
#define PARAM2_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM2_7 */
#define PARAM2_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM2_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM2_7 */
#define PARAM2_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM2_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM3_0 */
#define PARAM3_0_TRIGMODE_BIT_START                         0U
#define PARAM3_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM3_0 */
#define PARAM3_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM3_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM3_0 */
#define PARAM3_0_CR4INTREN_BIT_START                        7U
#define PARAM3_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM3_0 */
#define PARAM3_0_DMATRIGEN_BIT_START                        8U
#define PARAM3_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM3_0 */
#define PARAM3_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM3_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM3_0 */
#define PARAM3_0_BPM_EN_BIT_START                           13U
#define PARAM3_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM3_0 */
#define PARAM3_0_FFT_EN_BIT_START                           14U
#define PARAM3_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM3_0 */
#define PARAM3_0_WINDOW_EN_BIT_START                        15U
#define PARAM3_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM3_0 */
#define PARAM3_0_LOG2EN_BIT_START                           16U
#define PARAM3_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM3_0 */
#define PARAM3_0_ABSEN_BIT_START                            17U
#define PARAM3_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM3_0 */
#define PARAM3_0_CMULT_MODE_BIT_START                       18U
#define PARAM3_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM3_0 */
#define PARAM3_0_ACCEL_MODE_BIT_START                       21U
#define PARAM3_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM3_0 */
#define PARAM3_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM3_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM3_0 */
#define PARAM3_0_NU_BIT_START                               25U
#define PARAM3_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM3_1 */
#define PARAM3_1_SRCADDR_BIT_START                          0U
#define PARAM3_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM3_1 */
#define PARAM3_1_DSTADDR_BIT_START                          16U
#define PARAM3_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM3_2 */
#define PARAM3_2_SRCACNT_BIT_START                          0U
#define PARAM3_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM3_2 */
#define PARAM3_2_SRCREAL_BIT_START                          12U
#define PARAM3_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM3_2 */
#define PARAM3_2_SRC16B32B_BIT_START                        13U
#define PARAM3_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM3_2 */
#define PARAM3_2_SRCSIGNED_BIT_START                        14U
#define PARAM3_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM3_2 */
#define PARAM3_2_SRCCONJ_BIT_START                          15U
#define PARAM3_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM3_2 */
#define PARAM3_2_DSTACNT_BIT_START                          16U
#define PARAM3_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM3_2 */
#define PARAM3_2_DSTREAL_BIT_START                          28U
#define PARAM3_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM3_2 */
#define PARAM3_2_DST16B32B_BIT_START                        29U
#define PARAM3_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM3_2 */
#define PARAM3_2_DSTSIGNED_BIT_START                        30U
#define PARAM3_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM3_2 */
#define PARAM3_2_DSTCONJ_BIT_START                          31U
#define PARAM3_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM3_3 */
#define PARAM3_3_SRCAINDX_BIT_START                         0U
#define PARAM3_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM3_3 */
#define PARAM3_3_DSTAINDX_BIT_START                         16U
#define PARAM3_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM3_4 */
#define PARAM3_4_SRCBINDX_BIT_START                         0U
#define PARAM3_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM3_4 */
#define PARAM3_4_DSTBINDX_BIT_START                         16U
#define PARAM3_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM3_5 */
#define PARAM3_5_REG_BCNT_BIT_START                         0U
#define PARAM3_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM3_5 */
#define PARAM3_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM3_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM3_5 */
#define PARAM3_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM3_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM3_5 */
#define PARAM3_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM3_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM3_5 */
#define PARAM3_5_NU_BIT_START                               30U
#define PARAM3_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM3_6 */
#define PARAM3_6_DSTWIDTH_BIT_START                         0U
#define PARAM3_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM3_6 */
#define PARAM3_6_FFTSIZE_BIT_START                          2U
#define PARAM3_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM3_6 */
#define PARAM3_6_WINSYMM_BIT_START                          6U
#define PARAM3_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM3_6 */
#define PARAM3_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM3_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM3_6 */
#define PARAM3_6_BPMPHASE_BIT_START                         8U
#define PARAM3_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM3_6 */
#define PARAM3_6_WINDOW_START_BIT_START                     12U
#define PARAM3_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM3_6 */
#define PARAM3_6_BFLY_SCALING_BIT_START                     22U
#define PARAM3_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM3_7 */
#define PARAM3_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM3_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM3_7 */
#define PARAM3_7_TWIDINCR_BIT_START                         12U
#define PARAM3_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM3_7 */
#define PARAM3_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM3_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM3_7 */
#define PARAM3_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM3_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM4_0 */
#define PARAM4_0_TRIGMODE_BIT_START                         0U
#define PARAM4_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM4_0 */
#define PARAM4_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM4_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM4_0 */
#define PARAM4_0_CR4INTREN_BIT_START                        7U
#define PARAM4_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM4_0 */
#define PARAM4_0_DMATRIGEN_BIT_START                        8U
#define PARAM4_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM4_0 */
#define PARAM4_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM4_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM4_0 */
#define PARAM4_0_BPM_EN_BIT_START                           13U
#define PARAM4_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM4_0 */
#define PARAM4_0_FFT_EN_BIT_START                           14U
#define PARAM4_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM4_0 */
#define PARAM4_0_WINDOW_EN_BIT_START                        15U
#define PARAM4_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM4_0 */
#define PARAM4_0_LOG2EN_BIT_START                           16U
#define PARAM4_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM4_0 */
#define PARAM4_0_ABSEN_BIT_START                            17U
#define PARAM4_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM4_0 */
#define PARAM4_0_CMULT_MODE_BIT_START                       18U
#define PARAM4_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM4_0 */
#define PARAM4_0_ACCEL_MODE_BIT_START                       21U
#define PARAM4_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM4_0 */
#define PARAM4_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM4_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM4_0 */
#define PARAM4_0_NU_BIT_START                               25U
#define PARAM4_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM4_1 */
#define PARAM4_1_SRCADDR_BIT_START                          0U
#define PARAM4_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM4_1 */
#define PARAM4_1_DSTADDR_BIT_START                          16U
#define PARAM4_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM4_2 */
#define PARAM4_2_SRCACNT_BIT_START                          0U
#define PARAM4_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM4_2 */
#define PARAM4_2_SRCREAL_BIT_START                          12U
#define PARAM4_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM4_2 */
#define PARAM4_2_SRC16B32B_BIT_START                        13U
#define PARAM4_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM4_2 */
#define PARAM4_2_SRCSIGNED_BIT_START                        14U
#define PARAM4_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM4_2 */
#define PARAM4_2_SRCCONJ_BIT_START                          15U
#define PARAM4_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM4_2 */
#define PARAM4_2_DSTACNT_BIT_START                          16U
#define PARAM4_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM4_2 */
#define PARAM4_2_DSTREAL_BIT_START                          28U
#define PARAM4_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM4_2 */
#define PARAM4_2_DST16B32B_BIT_START                        29U
#define PARAM4_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM4_2 */
#define PARAM4_2_DSTSIGNED_BIT_START                        30U
#define PARAM4_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM4_2 */
#define PARAM4_2_DSTCONJ_BIT_START                          31U
#define PARAM4_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM4_3 */
#define PARAM4_3_SRCAINDX_BIT_START                         0U
#define PARAM4_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM4_3 */
#define PARAM4_3_DSTAINDX_BIT_START                         16U
#define PARAM4_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM4_4 */
#define PARAM4_4_SRCBINDX_BIT_START                         0U
#define PARAM4_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM4_4 */
#define PARAM4_4_DSTBINDX_BIT_START                         16U
#define PARAM4_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM4_5 */
#define PARAM4_5_REG_BCNT_BIT_START                         0U
#define PARAM4_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM4_5 */
#define PARAM4_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM4_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM4_5 */
#define PARAM4_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM4_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM4_5 */
#define PARAM4_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM4_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM4_5 */
#define PARAM4_5_NU_BIT_START                               30U
#define PARAM4_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM4_6 */
#define PARAM4_6_DSTWIDTH_BIT_START                         0U
#define PARAM4_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM4_6 */
#define PARAM4_6_FFTSIZE_BIT_START                          2U
#define PARAM4_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM4_6 */
#define PARAM4_6_WINSYMM_BIT_START                          6U
#define PARAM4_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM4_6 */
#define PARAM4_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM4_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM4_6 */
#define PARAM4_6_BPMPHASE_BIT_START                         8U
#define PARAM4_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM4_6 */
#define PARAM4_6_WINDOW_START_BIT_START                     12U
#define PARAM4_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM4_6 */
#define PARAM4_6_BFLY_SCALING_BIT_START                     22U
#define PARAM4_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM4_7 */
#define PARAM4_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM4_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM4_7 */
#define PARAM4_7_TWIDINCR_BIT_START                         12U
#define PARAM4_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM4_7 */
#define PARAM4_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM4_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM4_7 */
#define PARAM4_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM4_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM5_0 */
#define PARAM5_0_TRIGMODE_BIT_START                         0U
#define PARAM5_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM5_0 */
#define PARAM5_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM5_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM5_0 */
#define PARAM5_0_CR4INTREN_BIT_START                        7U
#define PARAM5_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM5_0 */
#define PARAM5_0_DMATRIGEN_BIT_START                        8U
#define PARAM5_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM5_0 */
#define PARAM5_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM5_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM5_0 */
#define PARAM5_0_BPM_EN_BIT_START                           13U
#define PARAM5_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM5_0 */
#define PARAM5_0_FFT_EN_BIT_START                           14U
#define PARAM5_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM5_0 */
#define PARAM5_0_WINDOW_EN_BIT_START                        15U
#define PARAM5_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM5_0 */
#define PARAM5_0_LOG2EN_BIT_START                           16U
#define PARAM5_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM5_0 */
#define PARAM5_0_ABSEN_BIT_START                            17U
#define PARAM5_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM5_0 */
#define PARAM5_0_CMULT_MODE_BIT_START                       18U
#define PARAM5_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM5_0 */
#define PARAM5_0_ACCEL_MODE_BIT_START                       21U
#define PARAM5_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM5_0 */
#define PARAM5_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM5_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM5_0 */
#define PARAM5_0_NU_BIT_START                               25U
#define PARAM5_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM5_1 */
#define PARAM5_1_SRCADDR_BIT_START                          0U
#define PARAM5_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM5_1 */
#define PARAM5_1_DSTADDR_BIT_START                          16U
#define PARAM5_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM5_2 */
#define PARAM5_2_SRCACNT_BIT_START                          0U
#define PARAM5_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM5_2 */
#define PARAM5_2_SRCREAL_BIT_START                          12U
#define PARAM5_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM5_2 */
#define PARAM5_2_SRC16B32B_BIT_START                        13U
#define PARAM5_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM5_2 */
#define PARAM5_2_SRCSIGNED_BIT_START                        14U
#define PARAM5_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM5_2 */
#define PARAM5_2_SRCCONJ_BIT_START                          15U
#define PARAM5_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM5_2 */
#define PARAM5_2_DSTACNT_BIT_START                          16U
#define PARAM5_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM5_2 */
#define PARAM5_2_DSTREAL_BIT_START                          28U
#define PARAM5_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM5_2 */
#define PARAM5_2_DST16B32B_BIT_START                        29U
#define PARAM5_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM5_2 */
#define PARAM5_2_DSTSIGNED_BIT_START                        30U
#define PARAM5_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM5_2 */
#define PARAM5_2_DSTCONJ_BIT_START                          31U
#define PARAM5_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM5_3 */
#define PARAM5_3_SRCAINDX_BIT_START                         0U
#define PARAM5_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM5_3 */
#define PARAM5_3_DSTAINDX_BIT_START                         16U
#define PARAM5_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM5_4 */
#define PARAM5_4_SRCBINDX_BIT_START                         0U
#define PARAM5_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM5_4 */
#define PARAM5_4_DSTBINDX_BIT_START                         16U
#define PARAM5_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM5_5 */
#define PARAM5_5_REG_BCNT_BIT_START                         0U
#define PARAM5_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM5_5 */
#define PARAM5_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM5_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM5_5 */
#define PARAM5_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM5_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM5_5 */
#define PARAM5_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM5_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM5_5 */
#define PARAM5_5_NU_BIT_START                               30U
#define PARAM5_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM5_6 */
#define PARAM5_6_DSTWIDTH_BIT_START                         0U
#define PARAM5_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM5_6 */
#define PARAM5_6_FFTSIZE_BIT_START                          2U
#define PARAM5_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM5_6 */
#define PARAM5_6_WINSYMM_BIT_START                          6U
#define PARAM5_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM5_6 */
#define PARAM5_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM5_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM5_6 */
#define PARAM5_6_BPMPHASE_BIT_START                         8U
#define PARAM5_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM5_6 */
#define PARAM5_6_WINDOW_START_BIT_START                     12U
#define PARAM5_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM5_6 */
#define PARAM5_6_BFLY_SCALING_BIT_START                     22U
#define PARAM5_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM5_7 */
#define PARAM5_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM5_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM5_7 */
#define PARAM5_7_TWIDINCR_BIT_START                         12U
#define PARAM5_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM5_7 */
#define PARAM5_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM5_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM5_7 */
#define PARAM5_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM5_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM6_0 */
#define PARAM6_0_TRIGMODE_BIT_START                         0U
#define PARAM6_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM6_0 */
#define PARAM6_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM6_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM6_0 */
#define PARAM6_0_CR4INTREN_BIT_START                        7U
#define PARAM6_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM6_0 */
#define PARAM6_0_DMATRIGEN_BIT_START                        8U
#define PARAM6_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM6_0 */
#define PARAM6_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM6_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM6_0 */
#define PARAM6_0_BPM_EN_BIT_START                           13U
#define PARAM6_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM6_0 */
#define PARAM6_0_FFT_EN_BIT_START                           14U
#define PARAM6_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM6_0 */
#define PARAM6_0_WINDOW_EN_BIT_START                        15U
#define PARAM6_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM6_0 */
#define PARAM6_0_LOG2EN_BIT_START                           16U
#define PARAM6_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM6_0 */
#define PARAM6_0_ABSEN_BIT_START                            17U
#define PARAM6_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM6_0 */
#define PARAM6_0_CMULT_MODE_BIT_START                       18U
#define PARAM6_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM6_0 */
#define PARAM6_0_ACCEL_MODE_BIT_START                       21U
#define PARAM6_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM6_0 */
#define PARAM6_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM6_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM6_0 */
#define PARAM6_0_NU_BIT_START                               25U
#define PARAM6_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM6_1 */
#define PARAM6_1_SRCADDR_BIT_START                          0U
#define PARAM6_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM6_1 */
#define PARAM6_1_DSTADDR_BIT_START                          16U
#define PARAM6_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM6_2 */
#define PARAM6_2_SRCACNT_BIT_START                          0U
#define PARAM6_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM6_2 */
#define PARAM6_2_SRCREAL_BIT_START                          12U
#define PARAM6_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM6_2 */
#define PARAM6_2_SRC16B32B_BIT_START                        13U
#define PARAM6_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM6_2 */
#define PARAM6_2_SRCSIGNED_BIT_START                        14U
#define PARAM6_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM6_2 */
#define PARAM6_2_SRCCONJ_BIT_START                          15U
#define PARAM6_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM6_2 */
#define PARAM6_2_DSTACNT_BIT_START                          16U
#define PARAM6_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM6_2 */
#define PARAM6_2_DSTREAL_BIT_START                          28U
#define PARAM6_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM6_2 */
#define PARAM6_2_DST16B32B_BIT_START                        29U
#define PARAM6_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM6_2 */
#define PARAM6_2_DSTSIGNED_BIT_START                        30U
#define PARAM6_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM6_2 */
#define PARAM6_2_DSTCONJ_BIT_START                          31U
#define PARAM6_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM6_3 */
#define PARAM6_3_SRCAINDX_BIT_START                         0U
#define PARAM6_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM6_3 */
#define PARAM6_3_DSTAINDX_BIT_START                         16U
#define PARAM6_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM6_4 */
#define PARAM6_4_SRCBINDX_BIT_START                         0U
#define PARAM6_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM6_4 */
#define PARAM6_4_DSTBINDX_BIT_START                         16U
#define PARAM6_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM6_5 */
#define PARAM6_5_REG_BCNT_BIT_START                         0U
#define PARAM6_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM6_5 */
#define PARAM6_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM6_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM6_5 */
#define PARAM6_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM6_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM6_5 */
#define PARAM6_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM6_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM6_5 */
#define PARAM6_5_NU_BIT_START                               30U
#define PARAM6_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM6_6 */
#define PARAM6_6_DSTWIDTH_BIT_START                         0U
#define PARAM6_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM6_6 */
#define PARAM6_6_FFTSIZE_BIT_START                          2U
#define PARAM6_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM6_6 */
#define PARAM6_6_WINSYMM_BIT_START                          6U
#define PARAM6_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM6_6 */
#define PARAM6_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM6_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM6_6 */
#define PARAM6_6_BPMPHASE_BIT_START                         8U
#define PARAM6_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM6_6 */
#define PARAM6_6_WINDOW_START_BIT_START                     12U
#define PARAM6_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM6_6 */
#define PARAM6_6_BFLY_SCALING_BIT_START                     22U
#define PARAM6_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM6_7 */
#define PARAM6_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM6_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM6_7 */
#define PARAM6_7_TWIDINCR_BIT_START                         12U
#define PARAM6_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM6_7 */
#define PARAM6_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM6_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM6_7 */
#define PARAM6_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM6_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM7_0 */
#define PARAM7_0_TRIGMODE_BIT_START                         0U
#define PARAM7_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM7_0 */
#define PARAM7_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM7_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM7_0 */
#define PARAM7_0_CR4INTREN_BIT_START                        7U
#define PARAM7_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM7_0 */
#define PARAM7_0_DMATRIGEN_BIT_START                        8U
#define PARAM7_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM7_0 */
#define PARAM7_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM7_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM7_0 */
#define PARAM7_0_BPM_EN_BIT_START                           13U
#define PARAM7_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM7_0 */
#define PARAM7_0_FFT_EN_BIT_START                           14U
#define PARAM7_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM7_0 */
#define PARAM7_0_WINDOW_EN_BIT_START                        15U
#define PARAM7_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM7_0 */
#define PARAM7_0_LOG2EN_BIT_START                           16U
#define PARAM7_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM7_0 */
#define PARAM7_0_ABSEN_BIT_START                            17U
#define PARAM7_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM7_0 */
#define PARAM7_0_CMULT_MODE_BIT_START                       18U
#define PARAM7_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM7_0 */
#define PARAM7_0_ACCEL_MODE_BIT_START                       21U
#define PARAM7_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM7_0 */
#define PARAM7_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM7_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM7_0 */
#define PARAM7_0_NU_BIT_START                               25U
#define PARAM7_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM7_1 */
#define PARAM7_1_SRCADDR_BIT_START                          0U
#define PARAM7_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM7_1 */
#define PARAM7_1_DSTADDR_BIT_START                          16U
#define PARAM7_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM7_2 */
#define PARAM7_2_SRCACNT_BIT_START                          0U
#define PARAM7_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM7_2 */
#define PARAM7_2_SRCREAL_BIT_START                          12U
#define PARAM7_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM7_2 */
#define PARAM7_2_SRC16B32B_BIT_START                        13U
#define PARAM7_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM7_2 */
#define PARAM7_2_SRCSIGNED_BIT_START                        14U
#define PARAM7_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM7_2 */
#define PARAM7_2_SRCCONJ_BIT_START                          15U
#define PARAM7_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM7_2 */
#define PARAM7_2_DSTACNT_BIT_START                          16U
#define PARAM7_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM7_2 */
#define PARAM7_2_DSTREAL_BIT_START                          28U
#define PARAM7_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM7_2 */
#define PARAM7_2_DST16B32B_BIT_START                        29U
#define PARAM7_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM7_2 */
#define PARAM7_2_DSTSIGNED_BIT_START                        30U
#define PARAM7_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM7_2 */
#define PARAM7_2_DSTCONJ_BIT_START                          31U
#define PARAM7_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM7_3 */
#define PARAM7_3_SRCAINDX_BIT_START                         0U
#define PARAM7_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM7_3 */
#define PARAM7_3_DSTAINDX_BIT_START                         16U
#define PARAM7_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM7_4 */
#define PARAM7_4_SRCBINDX_BIT_START                         0U
#define PARAM7_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM7_4 */
#define PARAM7_4_DSTBINDX_BIT_START                         16U
#define PARAM7_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM7_5 */
#define PARAM7_5_REG_BCNT_BIT_START                         0U
#define PARAM7_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM7_5 */
#define PARAM7_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM7_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM7_5 */
#define PARAM7_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM7_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM7_5 */
#define PARAM7_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM7_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM7_5 */
#define PARAM7_5_NU_BIT_START                               30U
#define PARAM7_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM7_6 */
#define PARAM7_6_DSTWIDTH_BIT_START                         0U
#define PARAM7_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM7_6 */
#define PARAM7_6_FFTSIZE_BIT_START                          2U
#define PARAM7_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM7_6 */
#define PARAM7_6_WINSYMM_BIT_START                          6U
#define PARAM7_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM7_6 */
#define PARAM7_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM7_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM7_6 */
#define PARAM7_6_BPMPHASE_BIT_START                         8U
#define PARAM7_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM7_6 */
#define PARAM7_6_WINDOW_START_BIT_START                     12U
#define PARAM7_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM7_6 */
#define PARAM7_6_BFLY_SCALING_BIT_START                     22U
#define PARAM7_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM7_7 */
#define PARAM7_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM7_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM7_7 */
#define PARAM7_7_TWIDINCR_BIT_START                         12U
#define PARAM7_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM7_7 */
#define PARAM7_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM7_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM7_7 */
#define PARAM7_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM7_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM8_0 */
#define PARAM8_0_TRIGMODE_BIT_START                         0U
#define PARAM8_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM8_0 */
#define PARAM8_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM8_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM8_0 */
#define PARAM8_0_CR4INTREN_BIT_START                        7U
#define PARAM8_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM8_0 */
#define PARAM8_0_DMATRIGEN_BIT_START                        8U
#define PARAM8_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM8_0 */
#define PARAM8_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM8_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM8_0 */
#define PARAM8_0_BPM_EN_BIT_START                           13U
#define PARAM8_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM8_0 */
#define PARAM8_0_FFT_EN_BIT_START                           14U
#define PARAM8_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM8_0 */
#define PARAM8_0_WINDOW_EN_BIT_START                        15U
#define PARAM8_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM8_0 */
#define PARAM8_0_LOG2EN_BIT_START                           16U
#define PARAM8_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM8_0 */
#define PARAM8_0_ABSEN_BIT_START                            17U
#define PARAM8_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM8_0 */
#define PARAM8_0_CMULT_MODE_BIT_START                       18U
#define PARAM8_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM8_0 */
#define PARAM8_0_ACCEL_MODE_BIT_START                       21U
#define PARAM8_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM8_0 */
#define PARAM8_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM8_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM8_0 */
#define PARAM8_0_NU_BIT_START                               25U
#define PARAM8_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM8_1 */
#define PARAM8_1_SRCADDR_BIT_START                          0U
#define PARAM8_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM8_1 */
#define PARAM8_1_DSTADDR_BIT_START                          16U
#define PARAM8_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM8_2 */
#define PARAM8_2_SRCACNT_BIT_START                          0U
#define PARAM8_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM8_2 */
#define PARAM8_2_SRCREAL_BIT_START                          12U
#define PARAM8_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM8_2 */
#define PARAM8_2_SRC16B32B_BIT_START                        13U
#define PARAM8_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM8_2 */
#define PARAM8_2_SRCSIGNED_BIT_START                        14U
#define PARAM8_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM8_2 */
#define PARAM8_2_SRCCONJ_BIT_START                          15U
#define PARAM8_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM8_2 */
#define PARAM8_2_DSTACNT_BIT_START                          16U
#define PARAM8_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM8_2 */
#define PARAM8_2_DSTREAL_BIT_START                          28U
#define PARAM8_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM8_2 */
#define PARAM8_2_DST16B32B_BIT_START                        29U
#define PARAM8_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM8_2 */
#define PARAM8_2_DSTSIGNED_BIT_START                        30U
#define PARAM8_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM8_2 */
#define PARAM8_2_DSTCONJ_BIT_START                          31U
#define PARAM8_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM8_3 */
#define PARAM8_3_SRCAINDX_BIT_START                         0U
#define PARAM8_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM8_3 */
#define PARAM8_3_DSTAINDX_BIT_START                         16U
#define PARAM8_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM8_4 */
#define PARAM8_4_SRCBINDX_BIT_START                         0U
#define PARAM8_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM8_4 */
#define PARAM8_4_DSTBINDX_BIT_START                         16U
#define PARAM8_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM8_5 */
#define PARAM8_5_REG_BCNT_BIT_START                         0U
#define PARAM8_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM8_5 */
#define PARAM8_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM8_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM8_5 */
#define PARAM8_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM8_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM8_5 */
#define PARAM8_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM8_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM8_5 */
#define PARAM8_5_NU_BIT_START                               30U
#define PARAM8_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM8_6 */
#define PARAM8_6_DSTWIDTH_BIT_START                         0U
#define PARAM8_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM8_6 */
#define PARAM8_6_FFTSIZE_BIT_START                          2U
#define PARAM8_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM8_6 */
#define PARAM8_6_WINSYMM_BIT_START                          6U
#define PARAM8_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM8_6 */
#define PARAM8_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM8_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM8_6 */
#define PARAM8_6_BPMPHASE_BIT_START                         8U
#define PARAM8_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM8_6 */
#define PARAM8_6_WINDOW_START_BIT_START                     12U
#define PARAM8_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM8_6 */
#define PARAM8_6_BFLY_SCALING_BIT_START                     22U
#define PARAM8_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM8_7 */
#define PARAM8_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM8_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM8_7 */
#define PARAM8_7_TWIDINCR_BIT_START                         12U
#define PARAM8_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM8_7 */
#define PARAM8_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM8_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM8_7 */
#define PARAM8_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM8_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM9_0 */
#define PARAM9_0_TRIGMODE_BIT_START                         0U
#define PARAM9_0_TRIGMODE_BIT_END                           2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM9_0 */
#define PARAM9_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START          3U
#define PARAM9_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END            6U

/* Definition for field CR4INTREN in Register PARAM9_0 */
#define PARAM9_0_CR4INTREN_BIT_START                        7U
#define PARAM9_0_CR4INTREN_BIT_END                          7U

/* Definition for field DMATRIGEN in Register PARAM9_0 */
#define PARAM9_0_DMATRIGEN_BIT_START                        8U
#define PARAM9_0_DMATRIGEN_BIT_END                          8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM9_0 */
#define PARAM9_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START          9U
#define PARAM9_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END            12U

/* Definition for field BPM_EN in Register PARAM9_0 */
#define PARAM9_0_BPM_EN_BIT_START                           13U
#define PARAM9_0_BPM_EN_BIT_END                             13U

/* Definition for field FFT_EN in Register PARAM9_0 */
#define PARAM9_0_FFT_EN_BIT_START                           14U
#define PARAM9_0_FFT_EN_BIT_END                             14U

/* Definition for field WINDOW_EN in Register PARAM9_0 */
#define PARAM9_0_WINDOW_EN_BIT_START                        15U
#define PARAM9_0_WINDOW_EN_BIT_END                          15U

/* Definition for field LOG2EN in Register PARAM9_0 */
#define PARAM9_0_LOG2EN_BIT_START                           16U
#define PARAM9_0_LOG2EN_BIT_END                             16U

/* Definition for field ABSEN in Register PARAM9_0 */
#define PARAM9_0_ABSEN_BIT_START                            17U
#define PARAM9_0_ABSEN_BIT_END                              17U

/* Definition for field CMULT_MODE in Register PARAM9_0 */
#define PARAM9_0_CMULT_MODE_BIT_START                       18U
#define PARAM9_0_CMULT_MODE_BIT_END                         20U

/* Definition for field ACCEL_MODE in Register PARAM9_0 */
#define PARAM9_0_ACCEL_MODE_BIT_START                       21U
#define PARAM9_0_ACCEL_MODE_BIT_END                         22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM9_0 */
#define PARAM9_0_FFT_OUTPUT_MODE_BIT_START                  23U
#define PARAM9_0_FFT_OUTPUT_MODE_BIT_END                    24U

/* Definition for field NU in Register PARAM9_0 */
#define PARAM9_0_NU_BIT_START                               25U
#define PARAM9_0_NU_BIT_END                                 31U

/* Definition for field SRCADDR in Register PARAM9_1 */
#define PARAM9_1_SRCADDR_BIT_START                          0U
#define PARAM9_1_SRCADDR_BIT_END                            15U

/* Definition for field DSTADDR in Register PARAM9_1 */
#define PARAM9_1_DSTADDR_BIT_START                          16U
#define PARAM9_1_DSTADDR_BIT_END                            31U

/* Definition for field SRCACNT in Register PARAM9_2 */
#define PARAM9_2_SRCACNT_BIT_START                          0U
#define PARAM9_2_SRCACNT_BIT_END                            11U

/* Definition for field SRCREAL in Register PARAM9_2 */
#define PARAM9_2_SRCREAL_BIT_START                          12U
#define PARAM9_2_SRCREAL_BIT_END                            12U

/* Definition for field SRC16b32b in Register PARAM9_2 */
#define PARAM9_2_SRC16B32B_BIT_START                        13U
#define PARAM9_2_SRC16B32B_BIT_END                          13U

/* Definition for field SRCSIGNED in Register PARAM9_2 */
#define PARAM9_2_SRCSIGNED_BIT_START                        14U
#define PARAM9_2_SRCSIGNED_BIT_END                          14U

/* Definition for field SRCCONJ in Register PARAM9_2 */
#define PARAM9_2_SRCCONJ_BIT_START                          15U
#define PARAM9_2_SRCCONJ_BIT_END                            15U

/* Definition for field DSTACNT in Register PARAM9_2 */
#define PARAM9_2_DSTACNT_BIT_START                          16U
#define PARAM9_2_DSTACNT_BIT_END                            27U

/* Definition for field DSTREAL in Register PARAM9_2 */
#define PARAM9_2_DSTREAL_BIT_START                          28U
#define PARAM9_2_DSTREAL_BIT_END                            28U

/* Definition for field DST16b32b in Register PARAM9_2 */
#define PARAM9_2_DST16B32B_BIT_START                        29U
#define PARAM9_2_DST16B32B_BIT_END                          29U

/* Definition for field DSTSIGNED in Register PARAM9_2 */
#define PARAM9_2_DSTSIGNED_BIT_START                        30U
#define PARAM9_2_DSTSIGNED_BIT_END                          30U

/* Definition for field DSTCONJ in Register PARAM9_2 */
#define PARAM9_2_DSTCONJ_BIT_START                          31U
#define PARAM9_2_DSTCONJ_BIT_END                            31U

/* Definition for field SRCAINDX in Register PARAM9_3 */
#define PARAM9_3_SRCAINDX_BIT_START                         0U
#define PARAM9_3_SRCAINDX_BIT_END                           15U

/* Definition for field DSTAINDX in Register PARAM9_3 */
#define PARAM9_3_DSTAINDX_BIT_START                         16U
#define PARAM9_3_DSTAINDX_BIT_END                           31U

/* Definition for field SRCBINDX in Register PARAM9_4 */
#define PARAM9_4_SRCBINDX_BIT_START                         0U
#define PARAM9_4_SRCBINDX_BIT_END                           15U

/* Definition for field DSTBINDX in Register PARAM9_4 */
#define PARAM9_4_DSTBINDX_BIT_START                         16U
#define PARAM9_4_DSTBINDX_BIT_END                           31U

/* Definition for field REG_BCNT in Register PARAM9_5 */
#define PARAM9_5_REG_BCNT_BIT_START                         0U
#define PARAM9_5_REG_BCNT_BIT_END                           11U

/* Definition for field REG_SRCSCAL in Register PARAM9_5 */
#define PARAM9_5_REG_SRCSCAL_BIT_START                      12U
#define PARAM9_5_REG_SRCSCAL_BIT_END                        15U

/* Definition for field REG_DSTSCAL in Register PARAM9_5 */
#define PARAM9_5_REG_DSTSCAL_BIT_START                      16U
#define PARAM9_5_REG_DSTSCAL_BIT_END                        19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM9_5 */
#define PARAM9_5_REG_DST_SKIP_INIT_BIT_START                20U
#define PARAM9_5_REG_DST_SKIP_INIT_BIT_END                  29U

/* Definition for field NU in Register PARAM9_5 */
#define PARAM9_5_NU_BIT_START                               30U
#define PARAM9_5_NU_BIT_END                                 31U

/* Definition for field DSTWIDTH in Register PARAM9_6 */
#define PARAM9_6_DSTWIDTH_BIT_START                         0U
#define PARAM9_6_DSTWIDTH_BIT_END                           1U

/* Definition for field FFTSIZE in Register PARAM9_6 */
#define PARAM9_6_FFTSIZE_BIT_START                          2U
#define PARAM9_6_FFTSIZE_BIT_END                            5U

/* Definition for field WINSYMM in Register PARAM9_6 */
#define PARAM9_6_WINSYMM_BIT_START                          6U
#define PARAM9_6_WINSYMM_BIT_END                            6U

/* Definition for field INTERF_THRESH_EN in Register PARAM9_6 */
#define PARAM9_6_INTERF_THRESH_EN_BIT_START                 7U
#define PARAM9_6_INTERF_THRESH_EN_BIT_END                   7U

/* Definition for field BPMPHASE in Register PARAM9_6 */
#define PARAM9_6_BPMPHASE_BIT_START                         8U
#define PARAM9_6_BPMPHASE_BIT_END                           11U

/* Definition for field WINDOW_START in Register PARAM9_6 */
#define PARAM9_6_WINDOW_START_BIT_START                     12U
#define PARAM9_6_WINDOW_START_BIT_END                       21U

/* Definition for field BFLY_SCALING in Register PARAM9_6 */
#define PARAM9_6_BFLY_SCALING_BIT_START                     22U
#define PARAM9_6_BFLY_SCALING_BIT_END                       31U

/* Definition for field CIRCIRSHIFT in Register PARAM9_7 */
#define PARAM9_7_CIRCIRSHIFT_BIT_START                      0U
#define PARAM9_7_CIRCIRSHIFT_BIT_END                        11U

/* Definition for field TWIDINCR in Register PARAM9_7 */
#define PARAM9_7_TWIDINCR_BIT_START                         12U
#define PARAM9_7_TWIDINCR_BIT_END                           25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM9_7 */
#define PARAM9_7_WINDOW_INTERP_FRACTION_BIT_START           26U
#define PARAM9_7_WINDOW_INTERP_FRACTION_BIT_END             27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM9_7 */
#define PARAM9_7_CIRCSHIFTWRAP_BIT_START                    28U
#define PARAM9_7_CIRCSHIFTWRAP_BIT_END                      31U

/* Definition for field TRIGMODE in Register PARAM10_0 */
#define PARAM10_0_TRIGMODE_BIT_START                        0U
#define PARAM10_0_TRIGMODE_BIT_END                          2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM10_0 */
#define PARAM10_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START         3U
#define PARAM10_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END           6U

/* Definition for field CR4INTREN in Register PARAM10_0 */
#define PARAM10_0_CR4INTREN_BIT_START                       7U
#define PARAM10_0_CR4INTREN_BIT_END                         7U

/* Definition for field DMATRIGEN in Register PARAM10_0 */
#define PARAM10_0_DMATRIGEN_BIT_START                       8U
#define PARAM10_0_DMATRIGEN_BIT_END                         8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM10_0 */
#define PARAM10_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START         9U
#define PARAM10_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END           12U

/* Definition for field BPM_EN in Register PARAM10_0 */
#define PARAM10_0_BPM_EN_BIT_START                          13U
#define PARAM10_0_BPM_EN_BIT_END                            13U

/* Definition for field FFT_EN in Register PARAM10_0 */
#define PARAM10_0_FFT_EN_BIT_START                          14U
#define PARAM10_0_FFT_EN_BIT_END                            14U

/* Definition for field WINDOW_EN in Register PARAM10_0 */
#define PARAM10_0_WINDOW_EN_BIT_START                       15U
#define PARAM10_0_WINDOW_EN_BIT_END                         15U

/* Definition for field LOG2EN in Register PARAM10_0 */
#define PARAM10_0_LOG2EN_BIT_START                          16U
#define PARAM10_0_LOG2EN_BIT_END                            16U

/* Definition for field ABSEN in Register PARAM10_0 */
#define PARAM10_0_ABSEN_BIT_START                           17U
#define PARAM10_0_ABSEN_BIT_END                             17U

/* Definition for field CMULT_MODE in Register PARAM10_0 */
#define PARAM10_0_CMULT_MODE_BIT_START                      18U
#define PARAM10_0_CMULT_MODE_BIT_END                        20U

/* Definition for field ACCEL_MODE in Register PARAM10_0 */
#define PARAM10_0_ACCEL_MODE_BIT_START                      21U
#define PARAM10_0_ACCEL_MODE_BIT_END                        22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM10_0 */
#define PARAM10_0_FFT_OUTPUT_MODE_BIT_START                 23U
#define PARAM10_0_FFT_OUTPUT_MODE_BIT_END                   24U

/* Definition for field NU in Register PARAM10_0 */
#define PARAM10_0_NU_BIT_START                              25U
#define PARAM10_0_NU_BIT_END                                31U

/* Definition for field SRCADDR in Register PARAM10_1 */
#define PARAM10_1_SRCADDR_BIT_START                         0U
#define PARAM10_1_SRCADDR_BIT_END                           15U

/* Definition for field DSTADDR in Register PARAM10_1 */
#define PARAM10_1_DSTADDR_BIT_START                         16U
#define PARAM10_1_DSTADDR_BIT_END                           31U

/* Definition for field SRCACNT in Register PARAM10_2 */
#define PARAM10_2_SRCACNT_BIT_START                         0U
#define PARAM10_2_SRCACNT_BIT_END                           11U

/* Definition for field SRCREAL in Register PARAM10_2 */
#define PARAM10_2_SRCREAL_BIT_START                         12U
#define PARAM10_2_SRCREAL_BIT_END                           12U

/* Definition for field SRC16b32b in Register PARAM10_2 */
#define PARAM10_2_SRC16B32B_BIT_START                       13U
#define PARAM10_2_SRC16B32B_BIT_END                         13U

/* Definition for field SRCSIGNED in Register PARAM10_2 */
#define PARAM10_2_SRCSIGNED_BIT_START                       14U
#define PARAM10_2_SRCSIGNED_BIT_END                         14U

/* Definition for field SRCCONJ in Register PARAM10_2 */
#define PARAM10_2_SRCCONJ_BIT_START                         15U
#define PARAM10_2_SRCCONJ_BIT_END                           15U

/* Definition for field DSTACNT in Register PARAM10_2 */
#define PARAM10_2_DSTACNT_BIT_START                         16U
#define PARAM10_2_DSTACNT_BIT_END                           27U

/* Definition for field DSTREAL in Register PARAM10_2 */
#define PARAM10_2_DSTREAL_BIT_START                         28U
#define PARAM10_2_DSTREAL_BIT_END                           28U

/* Definition for field DST16b32b in Register PARAM10_2 */
#define PARAM10_2_DST16B32B_BIT_START                       29U
#define PARAM10_2_DST16B32B_BIT_END                         29U

/* Definition for field DSTSIGNED in Register PARAM10_2 */
#define PARAM10_2_DSTSIGNED_BIT_START                       30U
#define PARAM10_2_DSTSIGNED_BIT_END                         30U

/* Definition for field DSTCONJ in Register PARAM10_2 */
#define PARAM10_2_DSTCONJ_BIT_START                         31U
#define PARAM10_2_DSTCONJ_BIT_END                           31U

/* Definition for field SRCAINDX in Register PARAM10_3 */
#define PARAM10_3_SRCAINDX_BIT_START                        0U
#define PARAM10_3_SRCAINDX_BIT_END                          15U

/* Definition for field DSTAINDX in Register PARAM10_3 */
#define PARAM10_3_DSTAINDX_BIT_START                        16U
#define PARAM10_3_DSTAINDX_BIT_END                          31U

/* Definition for field SRCBINDX in Register PARAM10_4 */
#define PARAM10_4_SRCBINDX_BIT_START                        0U
#define PARAM10_4_SRCBINDX_BIT_END                          15U

/* Definition for field DSTBINDX in Register PARAM10_4 */
#define PARAM10_4_DSTBINDX_BIT_START                        16U
#define PARAM10_4_DSTBINDX_BIT_END                          31U

/* Definition for field REG_BCNT in Register PARAM10_5 */
#define PARAM10_5_REG_BCNT_BIT_START                        0U
#define PARAM10_5_REG_BCNT_BIT_END                          11U

/* Definition for field REG_SRCSCAL in Register PARAM10_5 */
#define PARAM10_5_REG_SRCSCAL_BIT_START                     12U
#define PARAM10_5_REG_SRCSCAL_BIT_END                       15U

/* Definition for field REG_DSTSCAL in Register PARAM10_5 */
#define PARAM10_5_REG_DSTSCAL_BIT_START                     16U
#define PARAM10_5_REG_DSTSCAL_BIT_END                       19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM10_5 */
#define PARAM10_5_REG_DST_SKIP_INIT_BIT_START               20U
#define PARAM10_5_REG_DST_SKIP_INIT_BIT_END                 29U

/* Definition for field NU in Register PARAM10_5 */
#define PARAM10_5_NU_BIT_START                              30U
#define PARAM10_5_NU_BIT_END                                31U

/* Definition for field DSTWIDTH in Register PARAM10_6 */
#define PARAM10_6_DSTWIDTH_BIT_START                        0U
#define PARAM10_6_DSTWIDTH_BIT_END                          1U

/* Definition for field FFTSIZE in Register PARAM10_6 */
#define PARAM10_6_FFTSIZE_BIT_START                         2U
#define PARAM10_6_FFTSIZE_BIT_END                           5U

/* Definition for field WINSYMM in Register PARAM10_6 */
#define PARAM10_6_WINSYMM_BIT_START                         6U
#define PARAM10_6_WINSYMM_BIT_END                           6U

/* Definition for field INTERF_THRESH_EN in Register PARAM10_6 */
#define PARAM10_6_INTERF_THRESH_EN_BIT_START                7U
#define PARAM10_6_INTERF_THRESH_EN_BIT_END                  7U

/* Definition for field BPMPHASE in Register PARAM10_6 */
#define PARAM10_6_BPMPHASE_BIT_START                        8U
#define PARAM10_6_BPMPHASE_BIT_END                          11U

/* Definition for field WINDOW_START in Register PARAM10_6 */
#define PARAM10_6_WINDOW_START_BIT_START                    12U
#define PARAM10_6_WINDOW_START_BIT_END                      21U

/* Definition for field BFLY_SCALING in Register PARAM10_6 */
#define PARAM10_6_BFLY_SCALING_BIT_START                    22U
#define PARAM10_6_BFLY_SCALING_BIT_END                      31U

/* Definition for field CIRCIRSHIFT in Register PARAM10_7 */
#define PARAM10_7_CIRCIRSHIFT_BIT_START                     0U
#define PARAM10_7_CIRCIRSHIFT_BIT_END                       11U

/* Definition for field TWIDINCR in Register PARAM10_7 */
#define PARAM10_7_TWIDINCR_BIT_START                        12U
#define PARAM10_7_TWIDINCR_BIT_END                          25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM10_7 */
#define PARAM10_7_WINDOW_INTERP_FRACTION_BIT_START          26U
#define PARAM10_7_WINDOW_INTERP_FRACTION_BIT_END            27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM10_7 */
#define PARAM10_7_CIRCSHIFTWRAP_BIT_START                   28U
#define PARAM10_7_CIRCSHIFTWRAP_BIT_END                     31U

/* Definition for field TRIGMODE in Register PARAM11_0 */
#define PARAM11_0_TRIGMODE_BIT_START                        0U
#define PARAM11_0_TRIGMODE_BIT_END                          2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM11_0 */
#define PARAM11_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START         3U
#define PARAM11_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END           6U

/* Definition for field CR4INTREN in Register PARAM11_0 */
#define PARAM11_0_CR4INTREN_BIT_START                       7U
#define PARAM11_0_CR4INTREN_BIT_END                         7U

/* Definition for field DMATRIGEN in Register PARAM11_0 */
#define PARAM11_0_DMATRIGEN_BIT_START                       8U
#define PARAM11_0_DMATRIGEN_BIT_END                         8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM11_0 */
#define PARAM11_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START         9U
#define PARAM11_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END           12U

/* Definition for field BPM_EN in Register PARAM11_0 */
#define PARAM11_0_BPM_EN_BIT_START                          13U
#define PARAM11_0_BPM_EN_BIT_END                            13U

/* Definition for field FFT_EN in Register PARAM11_0 */
#define PARAM11_0_FFT_EN_BIT_START                          14U
#define PARAM11_0_FFT_EN_BIT_END                            14U

/* Definition for field WINDOW_EN in Register PARAM11_0 */
#define PARAM11_0_WINDOW_EN_BIT_START                       15U
#define PARAM11_0_WINDOW_EN_BIT_END                         15U

/* Definition for field LOG2EN in Register PARAM11_0 */
#define PARAM11_0_LOG2EN_BIT_START                          16U
#define PARAM11_0_LOG2EN_BIT_END                            16U

/* Definition for field ABSEN in Register PARAM11_0 */
#define PARAM11_0_ABSEN_BIT_START                           17U
#define PARAM11_0_ABSEN_BIT_END                             17U

/* Definition for field CMULT_MODE in Register PARAM11_0 */
#define PARAM11_0_CMULT_MODE_BIT_START                      18U
#define PARAM11_0_CMULT_MODE_BIT_END                        20U

/* Definition for field ACCEL_MODE in Register PARAM11_0 */
#define PARAM11_0_ACCEL_MODE_BIT_START                      21U
#define PARAM11_0_ACCEL_MODE_BIT_END                        22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM11_0 */
#define PARAM11_0_FFT_OUTPUT_MODE_BIT_START                 23U
#define PARAM11_0_FFT_OUTPUT_MODE_BIT_END                   24U

/* Definition for field NU in Register PARAM11_0 */
#define PARAM11_0_NU_BIT_START                              25U
#define PARAM11_0_NU_BIT_END                                31U

/* Definition for field SRCADDR in Register PARAM11_1 */
#define PARAM11_1_SRCADDR_BIT_START                         0U
#define PARAM11_1_SRCADDR_BIT_END                           15U

/* Definition for field DSTADDR in Register PARAM11_1 */
#define PARAM11_1_DSTADDR_BIT_START                         16U
#define PARAM11_1_DSTADDR_BIT_END                           31U

/* Definition for field SRCACNT in Register PARAM11_2 */
#define PARAM11_2_SRCACNT_BIT_START                         0U
#define PARAM11_2_SRCACNT_BIT_END                           11U

/* Definition for field SRCREAL in Register PARAM11_2 */
#define PARAM11_2_SRCREAL_BIT_START                         12U
#define PARAM11_2_SRCREAL_BIT_END                           12U

/* Definition for field SRC16b32b in Register PARAM11_2 */
#define PARAM11_2_SRC16B32B_BIT_START                       13U
#define PARAM11_2_SRC16B32B_BIT_END                         13U

/* Definition for field SRCSIGNED in Register PARAM11_2 */
#define PARAM11_2_SRCSIGNED_BIT_START                       14U
#define PARAM11_2_SRCSIGNED_BIT_END                         14U

/* Definition for field SRCCONJ in Register PARAM11_2 */
#define PARAM11_2_SRCCONJ_BIT_START                         15U
#define PARAM11_2_SRCCONJ_BIT_END                           15U

/* Definition for field DSTACNT in Register PARAM11_2 */
#define PARAM11_2_DSTACNT_BIT_START                         16U
#define PARAM11_2_DSTACNT_BIT_END                           27U

/* Definition for field DSTREAL in Register PARAM11_2 */
#define PARAM11_2_DSTREAL_BIT_START                         28U
#define PARAM11_2_DSTREAL_BIT_END                           28U

/* Definition for field DST16b32b in Register PARAM11_2 */
#define PARAM11_2_DST16B32B_BIT_START                       29U
#define PARAM11_2_DST16B32B_BIT_END                         29U

/* Definition for field DSTSIGNED in Register PARAM11_2 */
#define PARAM11_2_DSTSIGNED_BIT_START                       30U
#define PARAM11_2_DSTSIGNED_BIT_END                         30U

/* Definition for field DSTCONJ in Register PARAM11_2 */
#define PARAM11_2_DSTCONJ_BIT_START                         31U
#define PARAM11_2_DSTCONJ_BIT_END                           31U

/* Definition for field SRCAINDX in Register PARAM11_3 */
#define PARAM11_3_SRCAINDX_BIT_START                        0U
#define PARAM11_3_SRCAINDX_BIT_END                          15U

/* Definition for field DSTAINDX in Register PARAM11_3 */
#define PARAM11_3_DSTAINDX_BIT_START                        16U
#define PARAM11_3_DSTAINDX_BIT_END                          31U

/* Definition for field SRCBINDX in Register PARAM11_4 */
#define PARAM11_4_SRCBINDX_BIT_START                        0U
#define PARAM11_4_SRCBINDX_BIT_END                          15U

/* Definition for field DSTBINDX in Register PARAM11_4 */
#define PARAM11_4_DSTBINDX_BIT_START                        16U
#define PARAM11_4_DSTBINDX_BIT_END                          31U

/* Definition for field REG_BCNT in Register PARAM11_5 */
#define PARAM11_5_REG_BCNT_BIT_START                        0U
#define PARAM11_5_REG_BCNT_BIT_END                          11U

/* Definition for field REG_SRCSCAL in Register PARAM11_5 */
#define PARAM11_5_REG_SRCSCAL_BIT_START                     12U
#define PARAM11_5_REG_SRCSCAL_BIT_END                       15U

/* Definition for field REG_DSTSCAL in Register PARAM11_5 */
#define PARAM11_5_REG_DSTSCAL_BIT_START                     16U
#define PARAM11_5_REG_DSTSCAL_BIT_END                       19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM11_5 */
#define PARAM11_5_REG_DST_SKIP_INIT_BIT_START               20U
#define PARAM11_5_REG_DST_SKIP_INIT_BIT_END                 29U

/* Definition for field NU in Register PARAM11_5 */
#define PARAM11_5_NU_BIT_START                              30U
#define PARAM11_5_NU_BIT_END                                31U

/* Definition for field DSTWIDTH in Register PARAM11_6 */
#define PARAM11_6_DSTWIDTH_BIT_START                        0U
#define PARAM11_6_DSTWIDTH_BIT_END                          1U

/* Definition for field FFTSIZE in Register PARAM11_6 */
#define PARAM11_6_FFTSIZE_BIT_START                         2U
#define PARAM11_6_FFTSIZE_BIT_END                           5U

/* Definition for field WINSYMM in Register PARAM11_6 */
#define PARAM11_6_WINSYMM_BIT_START                         6U
#define PARAM11_6_WINSYMM_BIT_END                           6U

/* Definition for field INTERF_THRESH_EN in Register PARAM11_6 */
#define PARAM11_6_INTERF_THRESH_EN_BIT_START                7U
#define PARAM11_6_INTERF_THRESH_EN_BIT_END                  7U

/* Definition for field BPMPHASE in Register PARAM11_6 */
#define PARAM11_6_BPMPHASE_BIT_START                        8U
#define PARAM11_6_BPMPHASE_BIT_END                          11U

/* Definition for field WINDOW_START in Register PARAM11_6 */
#define PARAM11_6_WINDOW_START_BIT_START                    12U
#define PARAM11_6_WINDOW_START_BIT_END                      21U

/* Definition for field BFLY_SCALING in Register PARAM11_6 */
#define PARAM11_6_BFLY_SCALING_BIT_START                    22U
#define PARAM11_6_BFLY_SCALING_BIT_END                      31U

/* Definition for field CIRCIRSHIFT in Register PARAM11_7 */
#define PARAM11_7_CIRCIRSHIFT_BIT_START                     0U
#define PARAM11_7_CIRCIRSHIFT_BIT_END                       11U

/* Definition for field TWIDINCR in Register PARAM11_7 */
#define PARAM11_7_TWIDINCR_BIT_START                        12U
#define PARAM11_7_TWIDINCR_BIT_END                          25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM11_7 */
#define PARAM11_7_WINDOW_INTERP_FRACTION_BIT_START          26U
#define PARAM11_7_WINDOW_INTERP_FRACTION_BIT_END            27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM11_7 */
#define PARAM11_7_CIRCSHIFTWRAP_BIT_START                   28U
#define PARAM11_7_CIRCSHIFTWRAP_BIT_END                     31U

/* Definition for field TRIGMODE in Register PARAM12_0 */
#define PARAM12_0_TRIGMODE_BIT_START                        0U
#define PARAM12_0_TRIGMODE_BIT_END                          2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM12_0 */
#define PARAM12_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START         3U
#define PARAM12_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END           6U

/* Definition for field CR4INTREN in Register PARAM12_0 */
#define PARAM12_0_CR4INTREN_BIT_START                       7U
#define PARAM12_0_CR4INTREN_BIT_END                         7U

/* Definition for field DMATRIGEN in Register PARAM12_0 */
#define PARAM12_0_DMATRIGEN_BIT_START                       8U
#define PARAM12_0_DMATRIGEN_BIT_END                         8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM12_0 */
#define PARAM12_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START         9U
#define PARAM12_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END           12U

/* Definition for field BPM_EN in Register PARAM12_0 */
#define PARAM12_0_BPM_EN_BIT_START                          13U
#define PARAM12_0_BPM_EN_BIT_END                            13U

/* Definition for field FFT_EN in Register PARAM12_0 */
#define PARAM12_0_FFT_EN_BIT_START                          14U
#define PARAM12_0_FFT_EN_BIT_END                            14U

/* Definition for field WINDOW_EN in Register PARAM12_0 */
#define PARAM12_0_WINDOW_EN_BIT_START                       15U
#define PARAM12_0_WINDOW_EN_BIT_END                         15U

/* Definition for field LOG2EN in Register PARAM12_0 */
#define PARAM12_0_LOG2EN_BIT_START                          16U
#define PARAM12_0_LOG2EN_BIT_END                            16U

/* Definition for field ABSEN in Register PARAM12_0 */
#define PARAM12_0_ABSEN_BIT_START                           17U
#define PARAM12_0_ABSEN_BIT_END                             17U

/* Definition for field CMULT_MODE in Register PARAM12_0 */
#define PARAM12_0_CMULT_MODE_BIT_START                      18U
#define PARAM12_0_CMULT_MODE_BIT_END                        20U

/* Definition for field ACCEL_MODE in Register PARAM12_0 */
#define PARAM12_0_ACCEL_MODE_BIT_START                      21U
#define PARAM12_0_ACCEL_MODE_BIT_END                        22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM12_0 */
#define PARAM12_0_FFT_OUTPUT_MODE_BIT_START                 23U
#define PARAM12_0_FFT_OUTPUT_MODE_BIT_END                   24U

/* Definition for field NU in Register PARAM12_0 */
#define PARAM12_0_NU_BIT_START                              25U
#define PARAM12_0_NU_BIT_END                                31U

/* Definition for field SRCADDR in Register PARAM12_1 */
#define PARAM12_1_SRCADDR_BIT_START                         0U
#define PARAM12_1_SRCADDR_BIT_END                           15U

/* Definition for field DSTADDR in Register PARAM12_1 */
#define PARAM12_1_DSTADDR_BIT_START                         16U
#define PARAM12_1_DSTADDR_BIT_END                           31U

/* Definition for field SRCACNT in Register PARAM12_2 */
#define PARAM12_2_SRCACNT_BIT_START                         0U
#define PARAM12_2_SRCACNT_BIT_END                           11U

/* Definition for field SRCREAL in Register PARAM12_2 */
#define PARAM12_2_SRCREAL_BIT_START                         12U
#define PARAM12_2_SRCREAL_BIT_END                           12U

/* Definition for field SRC16b32b in Register PARAM12_2 */
#define PARAM12_2_SRC16B32B_BIT_START                       13U
#define PARAM12_2_SRC16B32B_BIT_END                         13U

/* Definition for field SRCSIGNED in Register PARAM12_2 */
#define PARAM12_2_SRCSIGNED_BIT_START                       14U
#define PARAM12_2_SRCSIGNED_BIT_END                         14U

/* Definition for field SRCCONJ in Register PARAM12_2 */
#define PARAM12_2_SRCCONJ_BIT_START                         15U
#define PARAM12_2_SRCCONJ_BIT_END                           15U

/* Definition for field DSTACNT in Register PARAM12_2 */
#define PARAM12_2_DSTACNT_BIT_START                         16U
#define PARAM12_2_DSTACNT_BIT_END                           27U

/* Definition for field DSTREAL in Register PARAM12_2 */
#define PARAM12_2_DSTREAL_BIT_START                         28U
#define PARAM12_2_DSTREAL_BIT_END                           28U

/* Definition for field DST16b32b in Register PARAM12_2 */
#define PARAM12_2_DST16B32B_BIT_START                       29U
#define PARAM12_2_DST16B32B_BIT_END                         29U

/* Definition for field DSTSIGNED in Register PARAM12_2 */
#define PARAM12_2_DSTSIGNED_BIT_START                       30U
#define PARAM12_2_DSTSIGNED_BIT_END                         30U

/* Definition for field DSTCONJ in Register PARAM12_2 */
#define PARAM12_2_DSTCONJ_BIT_START                         31U
#define PARAM12_2_DSTCONJ_BIT_END                           31U

/* Definition for field SRCAINDX in Register PARAM12_3 */
#define PARAM12_3_SRCAINDX_BIT_START                        0U
#define PARAM12_3_SRCAINDX_BIT_END                          15U

/* Definition for field DSTAINDX in Register PARAM12_3 */
#define PARAM12_3_DSTAINDX_BIT_START                        16U
#define PARAM12_3_DSTAINDX_BIT_END                          31U

/* Definition for field SRCBINDX in Register PARAM12_4 */
#define PARAM12_4_SRCBINDX_BIT_START                        0U
#define PARAM12_4_SRCBINDX_BIT_END                          15U

/* Definition for field DSTBINDX in Register PARAM12_4 */
#define PARAM12_4_DSTBINDX_BIT_START                        16U
#define PARAM12_4_DSTBINDX_BIT_END                          31U

/* Definition for field REG_BCNT in Register PARAM12_5 */
#define PARAM12_5_REG_BCNT_BIT_START                        0U
#define PARAM12_5_REG_BCNT_BIT_END                          11U

/* Definition for field REG_SRCSCAL in Register PARAM12_5 */
#define PARAM12_5_REG_SRCSCAL_BIT_START                     12U
#define PARAM12_5_REG_SRCSCAL_BIT_END                       15U

/* Definition for field REG_DSTSCAL in Register PARAM12_5 */
#define PARAM12_5_REG_DSTSCAL_BIT_START                     16U
#define PARAM12_5_REG_DSTSCAL_BIT_END                       19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM12_5 */
#define PARAM12_5_REG_DST_SKIP_INIT_BIT_START               20U
#define PARAM12_5_REG_DST_SKIP_INIT_BIT_END                 29U

/* Definition for field NU in Register PARAM12_5 */
#define PARAM12_5_NU_BIT_START                              30U
#define PARAM12_5_NU_BIT_END                                31U

/* Definition for field DSTWIDTH in Register PARAM12_6 */
#define PARAM12_6_DSTWIDTH_BIT_START                        0U
#define PARAM12_6_DSTWIDTH_BIT_END                          1U

/* Definition for field FFTSIZE in Register PARAM12_6 */
#define PARAM12_6_FFTSIZE_BIT_START                         2U
#define PARAM12_6_FFTSIZE_BIT_END                           5U

/* Definition for field WINSYMM in Register PARAM12_6 */
#define PARAM12_6_WINSYMM_BIT_START                         6U
#define PARAM12_6_WINSYMM_BIT_END                           6U

/* Definition for field INTERF_THRESH_EN in Register PARAM12_6 */
#define PARAM12_6_INTERF_THRESH_EN_BIT_START                7U
#define PARAM12_6_INTERF_THRESH_EN_BIT_END                  7U

/* Definition for field BPMPHASE in Register PARAM12_6 */
#define PARAM12_6_BPMPHASE_BIT_START                        8U
#define PARAM12_6_BPMPHASE_BIT_END                          11U

/* Definition for field WINDOW_START in Register PARAM12_6 */
#define PARAM12_6_WINDOW_START_BIT_START                    12U
#define PARAM12_6_WINDOW_START_BIT_END                      21U

/* Definition for field BFLY_SCALING in Register PARAM12_6 */
#define PARAM12_6_BFLY_SCALING_BIT_START                    22U
#define PARAM12_6_BFLY_SCALING_BIT_END                      31U

/* Definition for field CIRCIRSHIFT in Register PARAM12_7 */
#define PARAM12_7_CIRCIRSHIFT_BIT_START                     0U
#define PARAM12_7_CIRCIRSHIFT_BIT_END                       11U

/* Definition for field TWIDINCR in Register PARAM12_7 */
#define PARAM12_7_TWIDINCR_BIT_START                        12U
#define PARAM12_7_TWIDINCR_BIT_END                          25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM12_7 */
#define PARAM12_7_WINDOW_INTERP_FRACTION_BIT_START          26U
#define PARAM12_7_WINDOW_INTERP_FRACTION_BIT_END            27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM12_7 */
#define PARAM12_7_CIRCSHIFTWRAP_BIT_START                   28U
#define PARAM12_7_CIRCSHIFTWRAP_BIT_END                     31U

/* Definition for field TRIGMODE in Register PARAM13_0 */
#define PARAM13_0_TRIGMODE_BIT_START                        0U
#define PARAM13_0_TRIGMODE_BIT_END                          2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM13_0 */
#define PARAM13_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START         3U
#define PARAM13_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END           6U

/* Definition for field CR4INTREN in Register PARAM13_0 */
#define PARAM13_0_CR4INTREN_BIT_START                       7U
#define PARAM13_0_CR4INTREN_BIT_END                         7U

/* Definition for field DMATRIGEN in Register PARAM13_0 */
#define PARAM13_0_DMATRIGEN_BIT_START                       8U
#define PARAM13_0_DMATRIGEN_BIT_END                         8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM13_0 */
#define PARAM13_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START         9U
#define PARAM13_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END           12U

/* Definition for field BPM_EN in Register PARAM13_0 */
#define PARAM13_0_BPM_EN_BIT_START                          13U
#define PARAM13_0_BPM_EN_BIT_END                            13U

/* Definition for field FFT_EN in Register PARAM13_0 */
#define PARAM13_0_FFT_EN_BIT_START                          14U
#define PARAM13_0_FFT_EN_BIT_END                            14U

/* Definition for field WINDOW_EN in Register PARAM13_0 */
#define PARAM13_0_WINDOW_EN_BIT_START                       15U
#define PARAM13_0_WINDOW_EN_BIT_END                         15U

/* Definition for field LOG2EN in Register PARAM13_0 */
#define PARAM13_0_LOG2EN_BIT_START                          16U
#define PARAM13_0_LOG2EN_BIT_END                            16U

/* Definition for field ABSEN in Register PARAM13_0 */
#define PARAM13_0_ABSEN_BIT_START                           17U
#define PARAM13_0_ABSEN_BIT_END                             17U

/* Definition for field CMULT_MODE in Register PARAM13_0 */
#define PARAM13_0_CMULT_MODE_BIT_START                      18U
#define PARAM13_0_CMULT_MODE_BIT_END                        20U

/* Definition for field ACCEL_MODE in Register PARAM13_0 */
#define PARAM13_0_ACCEL_MODE_BIT_START                      21U
#define PARAM13_0_ACCEL_MODE_BIT_END                        22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM13_0 */
#define PARAM13_0_FFT_OUTPUT_MODE_BIT_START                 23U
#define PARAM13_0_FFT_OUTPUT_MODE_BIT_END                   24U

/* Definition for field NU in Register PARAM13_0 */
#define PARAM13_0_NU_BIT_START                              25U
#define PARAM13_0_NU_BIT_END                                31U

/* Definition for field SRCADDR in Register PARAM13_1 */
#define PARAM13_1_SRCADDR_BIT_START                         0U
#define PARAM13_1_SRCADDR_BIT_END                           15U

/* Definition for field DSTADDR in Register PARAM13_1 */
#define PARAM13_1_DSTADDR_BIT_START                         16U
#define PARAM13_1_DSTADDR_BIT_END                           31U

/* Definition for field SRCACNT in Register PARAM13_2 */
#define PARAM13_2_SRCACNT_BIT_START                         0U
#define PARAM13_2_SRCACNT_BIT_END                           11U

/* Definition for field SRCREAL in Register PARAM13_2 */
#define PARAM13_2_SRCREAL_BIT_START                         12U
#define PARAM13_2_SRCREAL_BIT_END                           12U

/* Definition for field SRC16b32b in Register PARAM13_2 */
#define PARAM13_2_SRC16B32B_BIT_START                       13U
#define PARAM13_2_SRC16B32B_BIT_END                         13U

/* Definition for field SRCSIGNED in Register PARAM13_2 */
#define PARAM13_2_SRCSIGNED_BIT_START                       14U
#define PARAM13_2_SRCSIGNED_BIT_END                         14U

/* Definition for field SRCCONJ in Register PARAM13_2 */
#define PARAM13_2_SRCCONJ_BIT_START                         15U
#define PARAM13_2_SRCCONJ_BIT_END                           15U

/* Definition for field DSTACNT in Register PARAM13_2 */
#define PARAM13_2_DSTACNT_BIT_START                         16U
#define PARAM13_2_DSTACNT_BIT_END                           27U

/* Definition for field DSTREAL in Register PARAM13_2 */
#define PARAM13_2_DSTREAL_BIT_START                         28U
#define PARAM13_2_DSTREAL_BIT_END                           28U

/* Definition for field DST16b32b in Register PARAM13_2 */
#define PARAM13_2_DST16B32B_BIT_START                       29U
#define PARAM13_2_DST16B32B_BIT_END                         29U

/* Definition for field DSTSIGNED in Register PARAM13_2 */
#define PARAM13_2_DSTSIGNED_BIT_START                       30U
#define PARAM13_2_DSTSIGNED_BIT_END                         30U

/* Definition for field DSTCONJ in Register PARAM13_2 */
#define PARAM13_2_DSTCONJ_BIT_START                         31U
#define PARAM13_2_DSTCONJ_BIT_END                           31U

/* Definition for field SRCAINDX in Register PARAM13_3 */
#define PARAM13_3_SRCAINDX_BIT_START                        0U
#define PARAM13_3_SRCAINDX_BIT_END                          15U

/* Definition for field DSTAINDX in Register PARAM13_3 */
#define PARAM13_3_DSTAINDX_BIT_START                        16U
#define PARAM13_3_DSTAINDX_BIT_END                          31U

/* Definition for field SRCBINDX in Register PARAM13_4 */
#define PARAM13_4_SRCBINDX_BIT_START                        0U
#define PARAM13_4_SRCBINDX_BIT_END                          15U

/* Definition for field DSTBINDX in Register PARAM13_4 */
#define PARAM13_4_DSTBINDX_BIT_START                        16U
#define PARAM13_4_DSTBINDX_BIT_END                          31U

/* Definition for field REG_BCNT in Register PARAM13_5 */
#define PARAM13_5_REG_BCNT_BIT_START                        0U
#define PARAM13_5_REG_BCNT_BIT_END                          11U

/* Definition for field REG_SRCSCAL in Register PARAM13_5 */
#define PARAM13_5_REG_SRCSCAL_BIT_START                     12U
#define PARAM13_5_REG_SRCSCAL_BIT_END                       15U

/* Definition for field REG_DSTSCAL in Register PARAM13_5 */
#define PARAM13_5_REG_DSTSCAL_BIT_START                     16U
#define PARAM13_5_REG_DSTSCAL_BIT_END                       19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM13_5 */
#define PARAM13_5_REG_DST_SKIP_INIT_BIT_START               20U
#define PARAM13_5_REG_DST_SKIP_INIT_BIT_END                 29U

/* Definition for field NU in Register PARAM13_5 */
#define PARAM13_5_NU_BIT_START                              30U
#define PARAM13_5_NU_BIT_END                                31U

/* Definition for field DSTWIDTH in Register PARAM13_6 */
#define PARAM13_6_DSTWIDTH_BIT_START                        0U
#define PARAM13_6_DSTWIDTH_BIT_END                          1U

/* Definition for field FFTSIZE in Register PARAM13_6 */
#define PARAM13_6_FFTSIZE_BIT_START                         2U
#define PARAM13_6_FFTSIZE_BIT_END                           5U

/* Definition for field WINSYMM in Register PARAM13_6 */
#define PARAM13_6_WINSYMM_BIT_START                         6U
#define PARAM13_6_WINSYMM_BIT_END                           6U

/* Definition for field INTERF_THRESH_EN in Register PARAM13_6 */
#define PARAM13_6_INTERF_THRESH_EN_BIT_START                7U
#define PARAM13_6_INTERF_THRESH_EN_BIT_END                  7U

/* Definition for field BPMPHASE in Register PARAM13_6 */
#define PARAM13_6_BPMPHASE_BIT_START                        8U
#define PARAM13_6_BPMPHASE_BIT_END                          11U

/* Definition for field WINDOW_START in Register PARAM13_6 */
#define PARAM13_6_WINDOW_START_BIT_START                    12U
#define PARAM13_6_WINDOW_START_BIT_END                      21U

/* Definition for field BFLY_SCALING in Register PARAM13_6 */
#define PARAM13_6_BFLY_SCALING_BIT_START                    22U
#define PARAM13_6_BFLY_SCALING_BIT_END                      31U

/* Definition for field CIRCIRSHIFT in Register PARAM13_7 */
#define PARAM13_7_CIRCIRSHIFT_BIT_START                     0U
#define PARAM13_7_CIRCIRSHIFT_BIT_END                       11U

/* Definition for field TWIDINCR in Register PARAM13_7 */
#define PARAM13_7_TWIDINCR_BIT_START                        12U
#define PARAM13_7_TWIDINCR_BIT_END                          25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM13_7 */
#define PARAM13_7_WINDOW_INTERP_FRACTION_BIT_START          26U
#define PARAM13_7_WINDOW_INTERP_FRACTION_BIT_END            27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM13_7 */
#define PARAM13_7_CIRCSHIFTWRAP_BIT_START                   28U
#define PARAM13_7_CIRCSHIFTWRAP_BIT_END                     31U

/* Definition for field TRIGMODE in Register PARAM14_0 */
#define PARAM14_0_TRIGMODE_BIT_START                        0U
#define PARAM14_0_TRIGMODE_BIT_END                          2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM14_0 */
#define PARAM14_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START         3U
#define PARAM14_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END           6U

/* Definition for field CR4INTREN in Register PARAM14_0 */
#define PARAM14_0_CR4INTREN_BIT_START                       7U
#define PARAM14_0_CR4INTREN_BIT_END                         7U

/* Definition for field DMATRIGEN in Register PARAM14_0 */
#define PARAM14_0_DMATRIGEN_BIT_START                       8U
#define PARAM14_0_DMATRIGEN_BIT_END                         8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM14_0 */
#define PARAM14_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START         9U
#define PARAM14_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END           12U

/* Definition for field BPM_EN in Register PARAM14_0 */
#define PARAM14_0_BPM_EN_BIT_START                          13U
#define PARAM14_0_BPM_EN_BIT_END                            13U

/* Definition for field FFT_EN in Register PARAM14_0 */
#define PARAM14_0_FFT_EN_BIT_START                          14U
#define PARAM14_0_FFT_EN_BIT_END                            14U

/* Definition for field WINDOW_EN in Register PARAM14_0 */
#define PARAM14_0_WINDOW_EN_BIT_START                       15U
#define PARAM14_0_WINDOW_EN_BIT_END                         15U

/* Definition for field LOG2EN in Register PARAM14_0 */
#define PARAM14_0_LOG2EN_BIT_START                          16U
#define PARAM14_0_LOG2EN_BIT_END                            16U

/* Definition for field ABSEN in Register PARAM14_0 */
#define PARAM14_0_ABSEN_BIT_START                           17U
#define PARAM14_0_ABSEN_BIT_END                             17U

/* Definition for field CMULT_MODE in Register PARAM14_0 */
#define PARAM14_0_CMULT_MODE_BIT_START                      18U
#define PARAM14_0_CMULT_MODE_BIT_END                        20U

/* Definition for field ACCEL_MODE in Register PARAM14_0 */
#define PARAM14_0_ACCEL_MODE_BIT_START                      21U
#define PARAM14_0_ACCEL_MODE_BIT_END                        22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM14_0 */
#define PARAM14_0_FFT_OUTPUT_MODE_BIT_START                 23U
#define PARAM14_0_FFT_OUTPUT_MODE_BIT_END                   24U

/* Definition for field NU in Register PARAM14_0 */
#define PARAM14_0_NU_BIT_START                              25U
#define PARAM14_0_NU_BIT_END                                31U

/* Definition for field SRCADDR in Register PARAM14_1 */
#define PARAM14_1_SRCADDR_BIT_START                         0U
#define PARAM14_1_SRCADDR_BIT_END                           15U

/* Definition for field DSTADDR in Register PARAM14_1 */
#define PARAM14_1_DSTADDR_BIT_START                         16U
#define PARAM14_1_DSTADDR_BIT_END                           31U

/* Definition for field SRCACNT in Register PARAM14_2 */
#define PARAM14_2_SRCACNT_BIT_START                         0U
#define PARAM14_2_SRCACNT_BIT_END                           11U

/* Definition for field SRCREAL in Register PARAM14_2 */
#define PARAM14_2_SRCREAL_BIT_START                         12U
#define PARAM14_2_SRCREAL_BIT_END                           12U

/* Definition for field SRC16b32b in Register PARAM14_2 */
#define PARAM14_2_SRC16B32B_BIT_START                       13U
#define PARAM14_2_SRC16B32B_BIT_END                         13U

/* Definition for field SRCSIGNED in Register PARAM14_2 */
#define PARAM14_2_SRCSIGNED_BIT_START                       14U
#define PARAM14_2_SRCSIGNED_BIT_END                         14U

/* Definition for field SRCCONJ in Register PARAM14_2 */
#define PARAM14_2_SRCCONJ_BIT_START                         15U
#define PARAM14_2_SRCCONJ_BIT_END                           15U

/* Definition for field DSTACNT in Register PARAM14_2 */
#define PARAM14_2_DSTACNT_BIT_START                         16U
#define PARAM14_2_DSTACNT_BIT_END                           27U

/* Definition for field DSTREAL in Register PARAM14_2 */
#define PARAM14_2_DSTREAL_BIT_START                         28U
#define PARAM14_2_DSTREAL_BIT_END                           28U

/* Definition for field DST16b32b in Register PARAM14_2 */
#define PARAM14_2_DST16B32B_BIT_START                       29U
#define PARAM14_2_DST16B32B_BIT_END                         29U

/* Definition for field DSTSIGNED in Register PARAM14_2 */
#define PARAM14_2_DSTSIGNED_BIT_START                       30U
#define PARAM14_2_DSTSIGNED_BIT_END                         30U

/* Definition for field DSTCONJ in Register PARAM14_2 */
#define PARAM14_2_DSTCONJ_BIT_START                         31U
#define PARAM14_2_DSTCONJ_BIT_END                           31U

/* Definition for field SRCAINDX in Register PARAM14_3 */
#define PARAM14_3_SRCAINDX_BIT_START                        0U
#define PARAM14_3_SRCAINDX_BIT_END                          15U

/* Definition for field DSTAINDX in Register PARAM14_3 */
#define PARAM14_3_DSTAINDX_BIT_START                        16U
#define PARAM14_3_DSTAINDX_BIT_END                          31U

/* Definition for field SRCBINDX in Register PARAM14_4 */
#define PARAM14_4_SRCBINDX_BIT_START                        0U
#define PARAM14_4_SRCBINDX_BIT_END                          15U

/* Definition for field DSTBINDX in Register PARAM14_4 */
#define PARAM14_4_DSTBINDX_BIT_START                        16U
#define PARAM14_4_DSTBINDX_BIT_END                          31U

/* Definition for field REG_BCNT in Register PARAM14_5 */
#define PARAM14_5_REG_BCNT_BIT_START                        0U
#define PARAM14_5_REG_BCNT_BIT_END                          11U

/* Definition for field REG_SRCSCAL in Register PARAM14_5 */
#define PARAM14_5_REG_SRCSCAL_BIT_START                     12U
#define PARAM14_5_REG_SRCSCAL_BIT_END                       15U

/* Definition for field REG_DSTSCAL in Register PARAM14_5 */
#define PARAM14_5_REG_DSTSCAL_BIT_START                     16U
#define PARAM14_5_REG_DSTSCAL_BIT_END                       19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM14_5 */
#define PARAM14_5_REG_DST_SKIP_INIT_BIT_START               20U
#define PARAM14_5_REG_DST_SKIP_INIT_BIT_END                 29U

/* Definition for field NU in Register PARAM14_5 */
#define PARAM14_5_NU_BIT_START                              30U
#define PARAM14_5_NU_BIT_END                                31U

/* Definition for field DSTWIDTH in Register PARAM14_6 */
#define PARAM14_6_DSTWIDTH_BIT_START                        0U
#define PARAM14_6_DSTWIDTH_BIT_END                          1U

/* Definition for field FFTSIZE in Register PARAM14_6 */
#define PARAM14_6_FFTSIZE_BIT_START                         2U
#define PARAM14_6_FFTSIZE_BIT_END                           5U

/* Definition for field WINSYMM in Register PARAM14_6 */
#define PARAM14_6_WINSYMM_BIT_START                         6U
#define PARAM14_6_WINSYMM_BIT_END                           6U

/* Definition for field INTERF_THRESH_EN in Register PARAM14_6 */
#define PARAM14_6_INTERF_THRESH_EN_BIT_START                7U
#define PARAM14_6_INTERF_THRESH_EN_BIT_END                  7U

/* Definition for field BPMPHASE in Register PARAM14_6 */
#define PARAM14_6_BPMPHASE_BIT_START                        8U
#define PARAM14_6_BPMPHASE_BIT_END                          11U

/* Definition for field WINDOW_START in Register PARAM14_6 */
#define PARAM14_6_WINDOW_START_BIT_START                    12U
#define PARAM14_6_WINDOW_START_BIT_END                      21U

/* Definition for field BFLY_SCALING in Register PARAM14_6 */
#define PARAM14_6_BFLY_SCALING_BIT_START                    22U
#define PARAM14_6_BFLY_SCALING_BIT_END                      31U

/* Definition for field CIRCIRSHIFT in Register PARAM14_7 */
#define PARAM14_7_CIRCIRSHIFT_BIT_START                     0U
#define PARAM14_7_CIRCIRSHIFT_BIT_END                       11U

/* Definition for field TWIDINCR in Register PARAM14_7 */
#define PARAM14_7_TWIDINCR_BIT_START                        12U
#define PARAM14_7_TWIDINCR_BIT_END                          25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM14_7 */
#define PARAM14_7_WINDOW_INTERP_FRACTION_BIT_START          26U
#define PARAM14_7_WINDOW_INTERP_FRACTION_BIT_END            27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM14_7 */
#define PARAM14_7_CIRCSHIFTWRAP_BIT_START                   28U
#define PARAM14_7_CIRCSHIFTWRAP_BIT_END                     31U

/* Definition for field TRIGMODE in Register PARAM15_0 */
#define PARAM15_0_TRIGMODE_BIT_START                        0U
#define PARAM15_0_TRIGMODE_BIT_END                          2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM15_0 */
#define PARAM15_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START         3U
#define PARAM15_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END           6U

/* Definition for field CR4INTREN in Register PARAM15_0 */
#define PARAM15_0_CR4INTREN_BIT_START                       7U
#define PARAM15_0_CR4INTREN_BIT_END                         7U

/* Definition for field DMATRIGEN in Register PARAM15_0 */
#define PARAM15_0_DMATRIGEN_BIT_START                       8U
#define PARAM15_0_DMATRIGEN_BIT_END                         8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM15_0 */
#define PARAM15_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START         9U
#define PARAM15_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END           12U

/* Definition for field BPM_EN in Register PARAM15_0 */
#define PARAM15_0_BPM_EN_BIT_START                          13U
#define PARAM15_0_BPM_EN_BIT_END                            13U

/* Definition for field FFT_EN in Register PARAM15_0 */
#define PARAM15_0_FFT_EN_BIT_START                          14U
#define PARAM15_0_FFT_EN_BIT_END                            14U

/* Definition for field WINDOW_EN in Register PARAM15_0 */
#define PARAM15_0_WINDOW_EN_BIT_START                       15U
#define PARAM15_0_WINDOW_EN_BIT_END                         15U

/* Definition for field LOG2EN in Register PARAM15_0 */
#define PARAM15_0_LOG2EN_BIT_START                          16U
#define PARAM15_0_LOG2EN_BIT_END                            16U

/* Definition for field ABSEN in Register PARAM15_0 */
#define PARAM15_0_ABSEN_BIT_START                           17U
#define PARAM15_0_ABSEN_BIT_END                             17U

/* Definition for field CMULT_MODE in Register PARAM15_0 */
#define PARAM15_0_CMULT_MODE_BIT_START                      18U
#define PARAM15_0_CMULT_MODE_BIT_END                        20U

/* Definition for field ACCEL_MODE in Register PARAM15_0 */
#define PARAM15_0_ACCEL_MODE_BIT_START                      21U
#define PARAM15_0_ACCEL_MODE_BIT_END                        22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM15_0 */
#define PARAM15_0_FFT_OUTPUT_MODE_BIT_START                 23U
#define PARAM15_0_FFT_OUTPUT_MODE_BIT_END                   24U

/* Definition for field NU in Register PARAM15_0 */
#define PARAM15_0_NU_BIT_START                              25U
#define PARAM15_0_NU_BIT_END                                31U

/* Definition for field SRCADDR in Register PARAM15_1 */
#define PARAM15_1_SRCADDR_BIT_START                         0U
#define PARAM15_1_SRCADDR_BIT_END                           15U

/* Definition for field DSTADDR in Register PARAM15_1 */
#define PARAM15_1_DSTADDR_BIT_START                         16U
#define PARAM15_1_DSTADDR_BIT_END                           31U

/* Definition for field SRCACNT in Register PARAM15_2 */
#define PARAM15_2_SRCACNT_BIT_START                         0U
#define PARAM15_2_SRCACNT_BIT_END                           11U

/* Definition for field SRCREAL in Register PARAM15_2 */
#define PARAM15_2_SRCREAL_BIT_START                         12U
#define PARAM15_2_SRCREAL_BIT_END                           12U

/* Definition for field SRC16b32b in Register PARAM15_2 */
#define PARAM15_2_SRC16B32B_BIT_START                       13U
#define PARAM15_2_SRC16B32B_BIT_END                         13U

/* Definition for field SRCSIGNED in Register PARAM15_2 */
#define PARAM15_2_SRCSIGNED_BIT_START                       14U
#define PARAM15_2_SRCSIGNED_BIT_END                         14U

/* Definition for field SRCCONJ in Register PARAM15_2 */
#define PARAM15_2_SRCCONJ_BIT_START                         15U
#define PARAM15_2_SRCCONJ_BIT_END                           15U

/* Definition for field DSTACNT in Register PARAM15_2 */
#define PARAM15_2_DSTACNT_BIT_START                         16U
#define PARAM15_2_DSTACNT_BIT_END                           27U

/* Definition for field DSTREAL in Register PARAM15_2 */
#define PARAM15_2_DSTREAL_BIT_START                         28U
#define PARAM15_2_DSTREAL_BIT_END                           28U

/* Definition for field DST16b32b in Register PARAM15_2 */
#define PARAM15_2_DST16B32B_BIT_START                       29U
#define PARAM15_2_DST16B32B_BIT_END                         29U

/* Definition for field DSTSIGNED in Register PARAM15_2 */
#define PARAM15_2_DSTSIGNED_BIT_START                       30U
#define PARAM15_2_DSTSIGNED_BIT_END                         30U

/* Definition for field DSTCONJ in Register PARAM15_2 */
#define PARAM15_2_DSTCONJ_BIT_START                         31U
#define PARAM15_2_DSTCONJ_BIT_END                           31U

/* Definition for field SRCAINDX in Register PARAM15_3 */
#define PARAM15_3_SRCAINDX_BIT_START                        0U
#define PARAM15_3_SRCAINDX_BIT_END                          15U

/* Definition for field DSTAINDX in Register PARAM15_3 */
#define PARAM15_3_DSTAINDX_BIT_START                        16U
#define PARAM15_3_DSTAINDX_BIT_END                          31U

/* Definition for field SRCBINDX in Register PARAM15_4 */
#define PARAM15_4_SRCBINDX_BIT_START                        0U
#define PARAM15_4_SRCBINDX_BIT_END                          15U

/* Definition for field DSTBINDX in Register PARAM15_4 */
#define PARAM15_4_DSTBINDX_BIT_START                        16U
#define PARAM15_4_DSTBINDX_BIT_END                          31U

/* Definition for field REG_BCNT in Register PARAM15_5 */
#define PARAM15_5_REG_BCNT_BIT_START                        0U
#define PARAM15_5_REG_BCNT_BIT_END                          11U

/* Definition for field REG_SRCSCAL in Register PARAM15_5 */
#define PARAM15_5_REG_SRCSCAL_BIT_START                     12U
#define PARAM15_5_REG_SRCSCAL_BIT_END                       15U

/* Definition for field REG_DSTSCAL in Register PARAM15_5 */
#define PARAM15_5_REG_DSTSCAL_BIT_START                     16U
#define PARAM15_5_REG_DSTSCAL_BIT_END                       19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM15_5 */
#define PARAM15_5_REG_DST_SKIP_INIT_BIT_START               20U
#define PARAM15_5_REG_DST_SKIP_INIT_BIT_END                 29U

/* Definition for field NU in Register PARAM15_5 */
#define PARAM15_5_NU_BIT_START                              30U
#define PARAM15_5_NU_BIT_END                                31U

/* Definition for field DSTWIDTH in Register PARAM15_6 */
#define PARAM15_6_DSTWIDTH_BIT_START                        0U
#define PARAM15_6_DSTWIDTH_BIT_END                          1U

/* Definition for field FFTSIZE in Register PARAM15_6 */
#define PARAM15_6_FFTSIZE_BIT_START                         2U
#define PARAM15_6_FFTSIZE_BIT_END                           5U

/* Definition for field WINSYMM in Register PARAM15_6 */
#define PARAM15_6_WINSYMM_BIT_START                         6U
#define PARAM15_6_WINSYMM_BIT_END                           6U

/* Definition for field INTERF_THRESH_EN in Register PARAM15_6 */
#define PARAM15_6_INTERF_THRESH_EN_BIT_START                7U
#define PARAM15_6_INTERF_THRESH_EN_BIT_END                  7U

/* Definition for field BPMPHASE in Register PARAM15_6 */
#define PARAM15_6_BPMPHASE_BIT_START                        8U
#define PARAM15_6_BPMPHASE_BIT_END                          11U

/* Definition for field WINDOW_START in Register PARAM15_6 */
#define PARAM15_6_WINDOW_START_BIT_START                    12U
#define PARAM15_6_WINDOW_START_BIT_END                      21U

/* Definition for field BFLY_SCALING in Register PARAM15_6 */
#define PARAM15_6_BFLY_SCALING_BIT_START                    22U
#define PARAM15_6_BFLY_SCALING_BIT_END                      31U

/* Definition for field CIRCIRSHIFT in Register PARAM15_7 */
#define PARAM15_7_CIRCIRSHIFT_BIT_START                     0U
#define PARAM15_7_CIRCIRSHIFT_BIT_END                       11U

/* Definition for field TWIDINCR in Register PARAM15_7 */
#define PARAM15_7_TWIDINCR_BIT_START                        12U
#define PARAM15_7_TWIDINCR_BIT_END                          25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM15_7 */
#define PARAM15_7_WINDOW_INTERP_FRACTION_BIT_START          26U
#define PARAM15_7_WINDOW_INTERP_FRACTION_BIT_END            27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM15_7 */
#define PARAM15_7_CIRCSHIFTWRAP_BIT_START                   28U
#define PARAM15_7_CIRCSHIFTWRAP_BIT_END                     31U

/* Definition for field TRIGMODE in Register PARAM16_0 */
#define PARAM16_0_TRIGMODE_BIT_START                        0U
#define PARAM16_0_TRIGMODE_BIT_END                          2U

/* Definition for field DMA2ACC_CHANNEL_TRIGSRC in Register PARAM16_0 */
#define PARAM16_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_START         3U
#define PARAM16_0_DMA2ACC_CHANNEL_TRIGSRC_BIT_END           6U

/* Definition for field CR4INTREN in Register PARAM16_0 */
#define PARAM16_0_CR4INTREN_BIT_START                       7U
#define PARAM16_0_CR4INTREN_BIT_END                         7U

/* Definition for field DMATRIGEN in Register PARAM16_0 */
#define PARAM16_0_DMATRIGEN_BIT_START                       8U
#define PARAM16_0_DMATRIGEN_BIT_END                         8U

/* Definition for field ACC2DMA_CHANNEL_TRIGDST in Register PARAM16_0 */
#define PARAM16_0_ACC2DMA_CHANNEL_TRIGDST_BIT_START         9U
#define PARAM16_0_ACC2DMA_CHANNEL_TRIGDST_BIT_END           12U

/* Definition for field BPM_EN in Register PARAM16_0 */
#define PARAM16_0_BPM_EN_BIT_START                          13U
#define PARAM16_0_BPM_EN_BIT_END                            13U

/* Definition for field FFT_EN in Register PARAM16_0 */
#define PARAM16_0_FFT_EN_BIT_START                          14U
#define PARAM16_0_FFT_EN_BIT_END                            14U

/* Definition for field WINDOW_EN in Register PARAM16_0 */
#define PARAM16_0_WINDOW_EN_BIT_START                       15U
#define PARAM16_0_WINDOW_EN_BIT_END                         15U

/* Definition for field LOG2EN in Register PARAM16_0 */
#define PARAM16_0_LOG2EN_BIT_START                          16U
#define PARAM16_0_LOG2EN_BIT_END                            16U

/* Definition for field ABSEN in Register PARAM16_0 */
#define PARAM16_0_ABSEN_BIT_START                           17U
#define PARAM16_0_ABSEN_BIT_END                             17U

/* Definition for field CMULT_MODE in Register PARAM16_0 */
#define PARAM16_0_CMULT_MODE_BIT_START                      18U
#define PARAM16_0_CMULT_MODE_BIT_END                        20U

/* Definition for field ACCEL_MODE in Register PARAM16_0 */
#define PARAM16_0_ACCEL_MODE_BIT_START                      21U
#define PARAM16_0_ACCEL_MODE_BIT_END                        22U

/* Definition for field FFT_OUTPUT_MODE in Register PARAM16_0 */
#define PARAM16_0_FFT_OUTPUT_MODE_BIT_START                 23U
#define PARAM16_0_FFT_OUTPUT_MODE_BIT_END                   24U

/* Definition for field NU in Register PARAM16_0 */
#define PARAM16_0_NU_BIT_START                              25U
#define PARAM16_0_NU_BIT_END                                31U

/* Definition for field SRCADDR in Register PARAM16_1 */
#define PARAM16_1_SRCADDR_BIT_START                         0U
#define PARAM16_1_SRCADDR_BIT_END                           15U

/* Definition for field DSTADDR in Register PARAM16_1 */
#define PARAM16_1_DSTADDR_BIT_START                         16U
#define PARAM16_1_DSTADDR_BIT_END                           31U

/* Definition for field SRCACNT in Register PARAM16_2 */
#define PARAM16_2_SRCACNT_BIT_START                         0U
#define PARAM16_2_SRCACNT_BIT_END                           11U

/* Definition for field SRCREAL in Register PARAM16_2 */
#define PARAM16_2_SRCREAL_BIT_START                         12U
#define PARAM16_2_SRCREAL_BIT_END                           12U

/* Definition for field SRC16b32b in Register PARAM16_2 */
#define PARAM16_2_SRC16B32B_BIT_START                       13U
#define PARAM16_2_SRC16B32B_BIT_END                         13U

/* Definition for field SRCSIGNED in Register PARAM16_2 */
#define PARAM16_2_SRCSIGNED_BIT_START                       14U
#define PARAM16_2_SRCSIGNED_BIT_END                         14U

/* Definition for field SRCCONJ in Register PARAM16_2 */
#define PARAM16_2_SRCCONJ_BIT_START                         15U
#define PARAM16_2_SRCCONJ_BIT_END                           15U

/* Definition for field DSTACNT in Register PARAM16_2 */
#define PARAM16_2_DSTACNT_BIT_START                         16U
#define PARAM16_2_DSTACNT_BIT_END                           27U

/* Definition for field DSTREAL in Register PARAM16_2 */
#define PARAM16_2_DSTREAL_BIT_START                         28U
#define PARAM16_2_DSTREAL_BIT_END                           28U

/* Definition for field DST16b32b in Register PARAM16_2 */
#define PARAM16_2_DST16B32B_BIT_START                       29U
#define PARAM16_2_DST16B32B_BIT_END                         29U

/* Definition for field DSTSIGNED in Register PARAM16_2 */
#define PARAM16_2_DSTSIGNED_BIT_START                       30U
#define PARAM16_2_DSTSIGNED_BIT_END                         30U

/* Definition for field DSTCONJ in Register PARAM16_2 */
#define PARAM16_2_DSTCONJ_BIT_START                         31U
#define PARAM16_2_DSTCONJ_BIT_END                           31U

/* Definition for field SRCAINDX in Register PARAM16_3 */
#define PARAM16_3_SRCAINDX_BIT_START                        0U
#define PARAM16_3_SRCAINDX_BIT_END                          15U

/* Definition for field DSTAINDX in Register PARAM16_3 */
#define PARAM16_3_DSTAINDX_BIT_START                        16U
#define PARAM16_3_DSTAINDX_BIT_END                          31U

/* Definition for field SRCBINDX in Register PARAM16_4 */
#define PARAM16_4_SRCBINDX_BIT_START                        0U
#define PARAM16_4_SRCBINDX_BIT_END                          15U

/* Definition for field DSTBINDX in Register PARAM16_4 */
#define PARAM16_4_DSTBINDX_BIT_START                        16U
#define PARAM16_4_DSTBINDX_BIT_END                          31U

/* Definition for field REG_BCNT in Register PARAM16_5 */
#define PARAM16_5_REG_BCNT_BIT_START                        0U
#define PARAM16_5_REG_BCNT_BIT_END                          11U

/* Definition for field REG_SRCSCAL in Register PARAM16_5 */
#define PARAM16_5_REG_SRCSCAL_BIT_START                     12U
#define PARAM16_5_REG_SRCSCAL_BIT_END                       15U

/* Definition for field REG_DSTSCAL in Register PARAM16_5 */
#define PARAM16_5_REG_DSTSCAL_BIT_START                     16U
#define PARAM16_5_REG_DSTSCAL_BIT_END                       19U

/* Definition for field REG_DST_SKIP_INIT in Register PARAM16_5 */
#define PARAM16_5_REG_DST_SKIP_INIT_BIT_START               20U
#define PARAM16_5_REG_DST_SKIP_INIT_BIT_END                 29U

/* Definition for field NU in Register PARAM16_5 */
#define PARAM16_5_NU_BIT_START                              30U
#define PARAM16_5_NU_BIT_END                                31U

/* Definition for field DSTWIDTH in Register PARAM16_6 */
#define PARAM16_6_DSTWIDTH_BIT_START                        0U
#define PARAM16_6_DSTWIDTH_BIT_END                          1U

/* Definition for field FFTSIZE in Register PARAM16_6 */
#define PARAM16_6_FFTSIZE_BIT_START                         2U
#define PARAM16_6_FFTSIZE_BIT_END                           5U

/* Definition for field WINSYMM in Register PARAM16_6 */
#define PARAM16_6_WINSYMM_BIT_START                         6U
#define PARAM16_6_WINSYMM_BIT_END                           6U

/* Definition for field INTERF_THRESH_EN in Register PARAM16_6 */
#define PARAM16_6_INTERF_THRESH_EN_BIT_START                7U
#define PARAM16_6_INTERF_THRESH_EN_BIT_END                  7U

/* Definition for field BPMPHASE in Register PARAM16_6 */
#define PARAM16_6_BPMPHASE_BIT_START                        8U
#define PARAM16_6_BPMPHASE_BIT_END                          11U

/* Definition for field WINDOW_START in Register PARAM16_6 */
#define PARAM16_6_WINDOW_START_BIT_START                    12U
#define PARAM16_6_WINDOW_START_BIT_END                      21U

/* Definition for field BFLY_SCALING in Register PARAM16_6 */
#define PARAM16_6_BFLY_SCALING_BIT_START                    22U
#define PARAM16_6_BFLY_SCALING_BIT_END                      31U

/* Definition for field CIRCIRSHIFT in Register PARAM16_7 */
#define PARAM16_7_CIRCIRSHIFT_BIT_START                     0U
#define PARAM16_7_CIRCIRSHIFT_BIT_END                       11U

/* Definition for field TWIDINCR in Register PARAM16_7 */
#define PARAM16_7_TWIDINCR_BIT_START                        12U
#define PARAM16_7_TWIDINCR_BIT_END                          25U

/* Definition for field WINDOW_INTERP_FRACTION in Register PARAM16_7 */
#define PARAM16_7_WINDOW_INTERP_FRACTION_BIT_START          26U
#define PARAM16_7_WINDOW_INTERP_FRACTION_BIT_END            27U

/* Definition for field CIRCSHIFTWRAP in Register PARAM16_7 */
#define PARAM16_7_CIRCSHIFTWRAP_BIT_START                   28U
#define PARAM16_7_CIRCSHIFTWRAP_BIT_END                     31U


/**
 * @struct DSSHWACCPARAMRegs_t
 * @brief
 *   Module DSS_HW_ACC_PARAM Register Definition
 * @details
 *   This structure is used to access the DSS_HW_ACC_PARAM module registers.
 */
/**
 * @typedef DSSHWACCPARAMRegs
 * @brief
 *   Module DSS_HW_ACC_PARAM Register Frame type Definition
 * @details
 *   This type is used to access the DSS_HW_ACC_PARAM module registers.
 */
typedef volatile struct DSSHWACCPARAMRegs_t
{
    uint32_t    PARAM1_0                           ;        /* Offset = 0x000 */
    uint32_t    PARAM1_1                           ;        /* Offset = 0x004 */
    uint32_t    PARAM1_2                           ;        /* Offset = 0x008 */
    uint32_t    PARAM1_3                           ;        /* Offset = 0x00C */
    uint32_t    PARAM1_4                           ;        /* Offset = 0x010 */
    uint32_t    PARAM1_5                           ;        /* Offset = 0x014 */
    uint32_t    PARAM1_6                           ;        /* Offset = 0x018 */
    uint32_t    PARAM1_7                           ;        /* Offset = 0x01C */
    uint32_t    PARAM2_0                           ;        /* Offset = 0x020 */
    uint32_t    PARAM2_1                           ;        /* Offset = 0x024 */
    uint32_t    PARAM2_2                           ;        /* Offset = 0x028 */
    uint32_t    PARAM2_3                           ;        /* Offset = 0x02C */
    uint32_t    PARAM2_4                           ;        /* Offset = 0x030 */
    uint32_t    PARAM2_5                           ;        /* Offset = 0x034 */
    uint32_t    PARAM2_6                           ;        /* Offset = 0x038 */
    uint32_t    PARAM2_7                           ;        /* Offset = 0x03C */
    uint32_t    PARAM3_0                           ;        /* Offset = 0x040 */
    uint32_t    PARAM3_1                           ;        /* Offset = 0x044 */
    uint32_t    PARAM3_2                           ;        /* Offset = 0x048 */
    uint32_t    PARAM3_3                           ;        /* Offset = 0x04C */
    uint32_t    PARAM3_4                           ;        /* Offset = 0x050 */
    uint32_t    PARAM3_5                           ;        /* Offset = 0x054 */
    uint32_t    PARAM3_6                           ;        /* Offset = 0x058 */
    uint32_t    PARAM3_7                           ;        /* Offset = 0x05C */
    uint32_t    PARAM4_0                           ;        /* Offset = 0x060 */
    uint32_t    PARAM4_1                           ;        /* Offset = 0x064 */
    uint32_t    PARAM4_2                           ;        /* Offset = 0x068 */
    uint32_t    PARAM4_3                           ;        /* Offset = 0x06C */
    uint32_t    PARAM4_4                           ;        /* Offset = 0x070 */
    uint32_t    PARAM4_5                           ;        /* Offset = 0x074 */
    uint32_t    PARAM4_6                           ;        /* Offset = 0x078 */
    uint32_t    PARAM4_7                           ;        /* Offset = 0x07C */
    uint32_t    PARAM5_0                           ;        /* Offset = 0x080 */
    uint32_t    PARAM5_1                           ;        /* Offset = 0x084 */
    uint32_t    PARAM5_2                           ;        /* Offset = 0x088 */
    uint32_t    PARAM5_3                           ;        /* Offset = 0x08C */
    uint32_t    PARAM5_4                           ;        /* Offset = 0x090 */
    uint32_t    PARAM5_5                           ;        /* Offset = 0x094 */
    uint32_t    PARAM5_6                           ;        /* Offset = 0x098 */
    uint32_t    PARAM5_7                           ;        /* Offset = 0x09C */
    uint32_t    PARAM6_0                           ;        /* Offset = 0x0A0 */
    uint32_t    PARAM6_1                           ;        /* Offset = 0x0A4 */
    uint32_t    PARAM6_2                           ;        /* Offset = 0x0A8 */
    uint32_t    PARAM6_3                           ;        /* Offset = 0x0AC */
    uint32_t    PARAM6_4                           ;        /* Offset = 0x0B0 */
    uint32_t    PARAM6_5                           ;        /* Offset = 0x0B4 */
    uint32_t    PARAM6_6                           ;        /* Offset = 0x0B8 */
    uint32_t    PARAM6_7                           ;        /* Offset = 0x0BC */
    uint32_t    PARAM7_0                           ;        /* Offset = 0x0C0 */
    uint32_t    PARAM7_1                           ;        /* Offset = 0x0C4 */
    uint32_t    PARAM7_2                           ;        /* Offset = 0x0C8 */
    uint32_t    PARAM7_3                           ;        /* Offset = 0x0CC */
    uint32_t    PARAM7_4                           ;        /* Offset = 0x0D0 */
    uint32_t    PARAM7_5                           ;        /* Offset = 0x0D4 */
    uint32_t    PARAM7_6                           ;        /* Offset = 0x0D8 */
    uint32_t    PARAM7_7                           ;        /* Offset = 0x0DC */
    uint32_t    PARAM8_0                           ;        /* Offset = 0x0E0 */
    uint32_t    PARAM8_1                           ;        /* Offset = 0x0E4 */
    uint32_t    PARAM8_2                           ;        /* Offset = 0x0E8 */
    uint32_t    PARAM8_3                           ;        /* Offset = 0x0EC */
    uint32_t    PARAM8_4                           ;        /* Offset = 0x0F0 */
    uint32_t    PARAM8_5                           ;        /* Offset = 0x0F4 */
    uint32_t    PARAM8_6                           ;        /* Offset = 0x0F8 */
    uint32_t    PARAM8_7                           ;        /* Offset = 0x0FC */
    uint32_t    PARAM9_0                           ;        /* Offset = 0x100 */
    uint32_t    PARAM9_1                           ;        /* Offset = 0x104 */
    uint32_t    PARAM9_2                           ;        /* Offset = 0x108 */
    uint32_t    PARAM9_3                           ;        /* Offset = 0x10C */
    uint32_t    PARAM9_4                           ;        /* Offset = 0x110 */
    uint32_t    PARAM9_5                           ;        /* Offset = 0x114 */
    uint32_t    PARAM9_6                           ;        /* Offset = 0x118 */
    uint32_t    PARAM9_7                           ;        /* Offset = 0x11C */
    uint32_t    PARAM10_0                          ;        /* Offset = 0x120 */
    uint32_t    PARAM10_1                          ;        /* Offset = 0x124 */
    uint32_t    PARAM10_2                          ;        /* Offset = 0x128 */
    uint32_t    PARAM10_3                          ;        /* Offset = 0x12C */
    uint32_t    PARAM10_4                          ;        /* Offset = 0x130 */
    uint32_t    PARAM10_5                          ;        /* Offset = 0x134 */
    uint32_t    PARAM10_6                          ;        /* Offset = 0x138 */
    uint32_t    PARAM10_7                          ;        /* Offset = 0x13C */
    uint32_t    PARAM11_0                          ;        /* Offset = 0x140 */
    uint32_t    PARAM11_1                          ;        /* Offset = 0x144 */
    uint32_t    PARAM11_2                          ;        /* Offset = 0x148 */
    uint32_t    PARAM11_3                          ;        /* Offset = 0x14C */
    uint32_t    PARAM11_4                          ;        /* Offset = 0x150 */
    uint32_t    PARAM11_5                          ;        /* Offset = 0x154 */
    uint32_t    PARAM11_6                          ;        /* Offset = 0x158 */
    uint32_t    PARAM11_7                          ;        /* Offset = 0x15C */
    uint32_t    PARAM12_0                          ;        /* Offset = 0x160 */
    uint32_t    PARAM12_1                          ;        /* Offset = 0x164 */
    uint32_t    PARAM12_2                          ;        /* Offset = 0x168 */
    uint32_t    PARAM12_3                          ;        /* Offset = 0x16C */
    uint32_t    PARAM12_4                          ;        /* Offset = 0x170 */
    uint32_t    PARAM12_5                          ;        /* Offset = 0x174 */
    uint32_t    PARAM12_6                          ;        /* Offset = 0x178 */
    uint32_t    PARAM12_7                          ;        /* Offset = 0x17C */
    uint32_t    PARAM13_0                          ;        /* Offset = 0x180 */
    uint32_t    PARAM13_1                          ;        /* Offset = 0x184 */
    uint32_t    PARAM13_2                          ;        /* Offset = 0x188 */
    uint32_t    PARAM13_3                          ;        /* Offset = 0x18C */
    uint32_t    PARAM13_4                          ;        /* Offset = 0x190 */
    uint32_t    PARAM13_5                          ;        /* Offset = 0x194 */
    uint32_t    PARAM13_6                          ;        /* Offset = 0x198 */
    uint32_t    PARAM13_7                          ;        /* Offset = 0x19C */
    uint32_t    PARAM14_0                          ;        /* Offset = 0x1A0 */
    uint32_t    PARAM14_1                          ;        /* Offset = 0x1A4 */
    uint32_t    PARAM14_2                          ;        /* Offset = 0x1A8 */
    uint32_t    PARAM14_3                          ;        /* Offset = 0x1AC */
    uint32_t    PARAM14_4                          ;        /* Offset = 0x1B0 */
    uint32_t    PARAM14_5                          ;        /* Offset = 0x1B4 */
    uint32_t    PARAM14_6                          ;        /* Offset = 0x1B8 */
    uint32_t    PARAM14_7                          ;        /* Offset = 0x1BC */
    uint32_t    PARAM15_0                          ;        /* Offset = 0x1C0 */
    uint32_t    PARAM15_1                          ;        /* Offset = 0x1C4 */
    uint32_t    PARAM15_2                          ;        /* Offset = 0x1C8 */
    uint32_t    PARAM15_3                          ;        /* Offset = 0x1CC */
    uint32_t    PARAM15_4                          ;        /* Offset = 0x1D0 */
    uint32_t    PARAM15_5                          ;        /* Offset = 0x1D4 */
    uint32_t    PARAM15_6                          ;        /* Offset = 0x1D8 */
    uint32_t    PARAM15_7                          ;        /* Offset = 0x1DC */
    uint32_t    PARAM16_0                          ;        /* Offset = 0x1E0 */
    uint32_t    PARAM16_1                          ;        /* Offset = 0x1E4 */
    uint32_t    PARAM16_2                          ;        /* Offset = 0x1E8 */
    uint32_t    PARAM16_3                          ;        /* Offset = 0x1EC */
    uint32_t    PARAM16_4                          ;        /* Offset = 0x1F0 */
    uint32_t    PARAM16_5                          ;        /* Offset = 0x1F4 */
    uint32_t    PARAM16_6                          ;        /* Offset = 0x1F8 */
    uint32_t    PARAM16_7                          ;        /* Offset = 0x1FC */
} DSSHWACCPARAMRegs;

#endif

#ifdef __cplusplus
}
#endif

#endif /* REG_DSSHWACCPARAM_H */
/* END OF REG_DSSHWACCPARAM_H */

