/**
 *  @file    reg_trng.h
 *
 *  @brief
 *    This file gives register definitions of MSS_TRNG module.
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

#ifndef REG_TRNG_H
#define REG_TRNG_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field RNG_OUT_L in Register TRNG_OUTPUT_L_1 */
#define TRNG_OUTPUT_L_1_RNG_OUT_L_BIT_START                 0U
#define TRNG_OUTPUT_L_1_RNG_OUT_L_BIT_END                   31U

/* Definition for field RNG_OUT_H in Register TRNG_OUTPUT_H */
#define TRNG_OUTPUT_H_RNG_OUT_H_BIT_START                   0U
#define TRNG_OUTPUT_H_RNG_OUT_H_BIT_END                     31U

/* Definition for field READY in Register TRNG_STATUS */
#define TRNG_STATUS_READY_BIT_START                         0U
#define TRNG_STATUS_READY_BIT_END                           0U

/* Definition for field SHUTDOWN_OFLO in Register TRNG_STATUS */
#define TRNG_STATUS_SHUTDOWN_OFLO_BIT_START                 1U
#define TRNG_STATUS_SHUTDOWN_OFLO_BIT_END                   1U

/* Definition for field NEED_CLOCK in Register TRNG_STATUS */
#define TRNG_STATUS_NEED_CLOCK_BIT_START                    31U
#define TRNG_STATUS_NEED_CLOCK_BIT_END                      31U

/* Definition for field READY in Register TRNG_INTMASK */
#define TRNG_INTMASK_READY_BIT_START                        0U
#define TRNG_INTMASK_READY_BIT_END                          0U

/* Definition for field SHUTDOWN_OFLO in Register TRNG_INTMASK */
#define TRNG_INTMASK_SHUTDOWN_OFLO_BIT_START                1U
#define TRNG_INTMASK_SHUTDOWN_OFLO_BIT_END                  1U

/* Definition for field READY in Register TRNG_INTACK */
#define TRNG_INTACK_READY_BIT_START                         0U
#define TRNG_INTACK_READY_BIT_END                           0U

/* Definition for field SHUTDOWN_OFLO in Register TRNG_INTACK */
#define TRNG_INTACK_SHUTDOWN_OFLO_BIT_START                 1U
#define TRNG_INTACK_SHUTDOWN_OFLO_BIT_END                   1U

/* Definition for field TEST_MODE in Register TRNG_CONTROL */
#define TRNG_CONTROL_TEST_MODE_BIT_START                    1U
#define TRNG_CONTROL_TEST_MODE_BIT_END                      1U

/* Definition for field NO_LFSR_FB in Register TRNG_CONTROL */
#define TRNG_CONTROL_NO_LFSR_FB_BIT_START                   2U
#define TRNG_CONTROL_NO_LFSR_FB_BIT_END                     2U

/* Definition for field ENABLE_TRNG in Register TRNG_CONTROL */
#define TRNG_CONTROL_ENABLE_TRNG_BIT_START                  10U
#define TRNG_CONTROL_ENABLE_TRNG_BIT_END                    10U

/* Definition for field STARTUP_CYCLES in Register TRNG_CONTROL */
#define TRNG_CONTROL_STARTUP_CYCLES_BIT_START               16U
#define TRNG_CONTROL_STARTUP_CYCLES_BIT_END                 31U

/* Definition for field MIN_REFILL_CYCLES in Register TRNG_CONFIG */
#define TRNG_CONFIG_MIN_REFILL_CYCLES_BIT_START             0U
#define TRNG_CONFIG_MIN_REFILL_CYCLES_BIT_END               7U

/* Definition for field SAMPLE_DIV in Register TRNG_CONFIG */
#define TRNG_CONFIG_SAMPLE_DIV_BIT_START                    8U
#define TRNG_CONFIG_SAMPLE_DIV_BIT_END                      11U

/* Definition for field MAX_REFILL_CYCLES in Register TRNG_CONFIG */
#define TRNG_CONFIG_MAX_REFILL_CYCLES_BIT_START             16U
#define TRNG_CONFIG_MAX_REFILL_CYCLES_BIT_END               31U

/* Definition for field ALARM_THRESHOLD in Register TRNG_ALARMCNT */
#define TRNG_ALARMCNT_ALARM_THRESHOLD_BIT_START             0U
#define TRNG_ALARMCNT_ALARM_THRESHOLD_BIT_END               7U

/* Definition for field SHUTDOWN_THRESHOLD in Register TRNG_ALARMCNT */
#define TRNG_ALARMCNT_SHUTDOWN_THRESHOLD_BIT_START          16U
#define TRNG_ALARMCNT_SHUTDOWN_THRESHOLD_BIT_END            20U

/* Definition for field SHUTDOWN_COUNT in Register TRNG_ALARMCNT */
#define TRNG_ALARMCNT_SHUTDOWN_COUNT_BIT_START              24U
#define TRNG_ALARMCNT_SHUTDOWN_COUNT_BIT_END                29U

/* Definition for field FRO_ENABLES in Register TRNG_FROENABLE */
#define TRNG_FROENABLE_FRO_ENABLES_BIT_START                0U
#define TRNG_FROENABLE_FRO_ENABLES_BIT_END                  23U

/* Definition for field FRO_DETUNES in Register TRNG_FRODETUNE */
#define TRNG_FRODETUNE_FRO_DETUNES_BIT_START                0U
#define TRNG_FRODETUNE_FRO_DETUNES_BIT_END                  23U

/* Definition for field FRO_ALARMMASKS in Register TRNG_ALARMMASK */
#define TRNG_ALARMMASK_FRO_ALARMMASKS_BIT_START             0U
#define TRNG_ALARMMASK_FRO_ALARMMASKS_BIT_END               23U

/* Definition for field FRO_ALARMSTOPS in Register TRNG_ALARMSTOP */
#define TRNG_ALARMSTOP_FRO_ALARMSTOPS_BIT_START             0U
#define TRNG_ALARMSTOP_FRO_ALARMSTOPS_BIT_END               23U

/* Definition for field LFSR_31_0 in Register TRNG_LFSR_L */
#define TRNG_LFSR_L_LFSR_31_0_BIT_START                     0U
#define TRNG_LFSR_L_LFSR_31_0_BIT_END                       31U

/* Definition for field LFSR_63_32 in Register TRNG_LFSR_M */
#define TRNG_LFSR_M_LFSR_63_32_BIT_START                    0U
#define TRNG_LFSR_M_LFSR_63_32_BIT_END                      31U

/* Definition for field LFSR_80_64 in Register TRNG_LFSR_H */
#define TRNG_LFSR_H_LFSR_80_64_BIT_START                    0U
#define TRNG_LFSR_H_LFSR_80_64_BIT_END                      31U

/* Definition for field SAMPLE_COUNTER in Register TRNG_COUNT */
#define TRNG_COUNT_SAMPLE_COUNTER_BIT_START                 0U
#define TRNG_COUNT_SAMPLE_COUNTER_BIT_END                   23U

/* Definition for field TEST_EN_OUT in Register TRNG_TEST */
#define TRNG_TEST_TEST_EN_OUT_BIT_START                     0U
#define TRNG_TEST_TEST_EN_OUT_BIT_END                       0U

/* Definition for field TEST_PATT_FRO in Register TRNG_TEST */
#define TRNG_TEST_TEST_PATT_FRO_BIT_START                   1U
#define TRNG_TEST_TEST_PATT_FRO_BIT_END                     1U

/* Definition for field TEST_PATT_DET in Register TRNG_TEST */
#define TRNG_TEST_TEST_PATT_DET_BIT_START                   2U
#define TRNG_TEST_TEST_PATT_DET_BIT_END                     2U

/* Definition for field TEST_SELECT in Register TRNG_TEST */
#define TRNG_TEST_TEST_SELECT_BIT_START                     8U
#define TRNG_TEST_TEST_SELECT_BIT_END                       12U

/* Definition for field TEST_PATTERN in Register TRNG_TEST */
#define TRNG_TEST_TEST_PATTERN_BIT_START                    16U
#define TRNG_TEST_TEST_PATTERN_BIT_END                      27U

/* Definition for field NR_OF_FROS in Register TRNG_OPTIONS */
#define TRNG_OPTIONS_NR_OF_FROS_BIT_START                   6U
#define TRNG_OPTIONS_NR_OF_FROS_BIT_END                     11U

/* Definition for field BASIC_EIP_NUMBER in Register TRNG_EIP_REV */
#define TRNG_EIP_REV_BASIC_EIP_NUMBER_BIT_START             0U
#define TRNG_EIP_REV_BASIC_EIP_NUMBER_BIT_END               7U

/* Definition for field COMPLEMENT_OF_BASIC_EIP_NUMBER in Register TRNG_EIP_REV */
#define TRNG_EIP_REV_COMPLEMENT_OF_BASIC_EIP_NUMBER_BIT_START 8U
#define TRNG_EIP_REV_COMPLEMENT_OF_BASIC_EIP_NUMBER_BIT_END 15U

/* Definition for field HW_PATCH_LEVEL in Register TRNG_EIP_REV */
#define TRNG_EIP_REV_HW_PATCH_LEVEL_BIT_START               16U
#define TRNG_EIP_REV_HW_PATCH_LEVEL_BIT_END                 19U

/* Definition for field MINOR_HW_REVISION in Register TRNG_EIP_REV */
#define TRNG_EIP_REV_MINOR_HW_REVISION_BIT_START            20U
#define TRNG_EIP_REV_MINOR_HW_REVISION_BIT_END              23U

/* Definition for field MAJOR_HW_REVISION in Register TRNG_EIP_REV */
#define TRNG_EIP_REV_MAJOR_HW_REVISION_BIT_START            24U
#define TRNG_EIP_REV_MAJOR_HW_REVISION_BIT_END              27U

/* Definition for field READY in Register MMR_STATUS_EN */
#define MMR_STATUS_EN_READY_BIT_START                       0U
#define MMR_STATUS_EN_READY_BIT_END                         0U

/* Definition for field SHUTDOWN_OFLO in Register MMR_STATUS_EN */
#define MMR_STATUS_EN_SHUTDOWN_OFLO_BIT_START               1U
#define MMR_STATUS_EN_SHUTDOWN_OFLO_BIT_END                 1U

/* Definition for field REVISION in Register TRNG_REV */
#define TRNG_REV_REVISION_BIT_START                         0U
#define TRNG_REV_REVISION_BIT_END                           7U

/* Definition for field AUTOIDLE in Register SYS_CONFIG_REG */
#define SYS_CONFIG_REG_AUTOIDLE_BIT_START                   0U
#define SYS_CONFIG_REG_AUTOIDLE_BIT_END                     0U

/* Definition for field IDLE_TYPE in Register SYS_CONFIG_REG */
#define SYS_CONFIG_REG_IDLE_TYPE_BIT_START                  3U
#define SYS_CONFIG_REG_IDLE_TYPE_BIT_END                    4U

/* Definition for field SW_INTR_STATUS_SET in Register MMR_STATUS_SET */
#define MMR_STATUS_SET_SW_INTR_STATUS_SET_BIT_START         0U
#define MMR_STATUS_SET_SW_INTR_STATUS_SET_BIT_END           1U

/* Definition for field SOFT_RESET in Register SOFT_RESET_REG */
#define SOFT_RESET_REG_SOFT_RESET_BIT_START                 0U
#define SOFT_RESET_REG_SOFT_RESET_BIT_END                   0U

/* Definition for field PULSE_INT_CLEAR in Register IRQ_EOI_REG */
#define IRQ_EOI_REG_PULSE_INT_CLEAR_BIT_START               0U
#define IRQ_EOI_REG_PULSE_INT_CLEAR_BIT_END                 0U

/* Definition for field TRNGIRQEN in Register TRNG_IRQSTATUS */
#define TRNG_IRQSTATUS_TRNGIRQEN_BIT_START                  0U
#define TRNG_IRQSTATUS_TRNGIRQEN_BIT_END                    0U



/**
 * @struct TRNGRegs_t
 * @brief
 *   Module MSS_TRNG Register Definition
 * @details
 *   This structure is used to access the MSS_TRNG module registers.
 */
/**
 * @typedef TRNGRegs
 * @brief
 *   Module MSS_TRNG Register Frame type Definition
 * @details
 *   This type is used to access the MSS_TRNG module registers.
 */
typedef volatile struct TRNGRegs_t
{
    uint32_t    TRNG_OUTPUT_L_1                    ;        /* Offset = 0x000 */
    uint32_t    TRNG_OUTPUT_H                      ;        /* Offset = 0x004 */
    uint32_t    TRNG_STATUS                        ;        /* Offset = 0x008 */
    uint32_t    TRNG_INTMASK                       ;        /* Offset = 0x00C */
    uint32_t    TRNG_INTACK                        ;        /* Offset = 0x010 */
    uint32_t    TRNG_CONTROL                       ;        /* Offset = 0x014 */
    uint32_t    TRNG_CONFIG                        ;        /* Offset = 0x018 */
    uint32_t    TRNG_ALARMCNT                      ;        /* Offset = 0x01C */
    uint32_t    TRNG_FROENABLE                     ;        /* Offset = 0x020 */
    uint32_t    TRNG_FRODETUNE                     ;        /* Offset = 0x024 */
    uint32_t    TRNG_ALARMMASK                     ;        /* Offset = 0x028 */
    uint32_t    TRNG_ALARMSTOP                     ;        /* Offset = 0x02C */
    uint32_t    TRNG_LFSR_L                        ;        /* Offset = 0x030 */
    uint32_t    TRNG_LFSR_M                        ;        /* Offset = 0x034 */
    uint32_t    TRNG_LFSR_H                        ;        /* Offset = 0x038 */
    uint32_t    TRNG_COUNT                         ;        /* Offset = 0x03C */
    uint32_t    TRNG_TEST                          ;        /* Offset = 0x040 */
    uint32_t    RESTRICTED1[13]                    ;        /* Offset = 0x044 */
    uint32_t    TRNG_OPTIONS                       ;        /* Offset = 0x078 */
    uint32_t    TRNG_EIP_REV                       ;        /* Offset = 0x07C */
    uint32_t    RESTRICTED2[2006]                  ;        /* Offset = 0x080 */
    uint32_t    MMR_STATUS_EN                      ;        /* Offset = 0x1FD8 */
    uint32_t    RESTRICTED3                        ;        /* Offset = 0x1FDC */
    uint32_t    TRNG_REV                           ;        /* Offset = 0x1FE0 */
    uint32_t    SYS_CONFIG_REG                     ;        /* Offset = 0x1FE4 */
    uint32_t    RESTRICTED4                        ;        /* Offset = 0x1FE8 */
    uint32_t    MMR_STATUS_SET                     ;        /* Offset = 0x1FEC */
    uint32_t    SOFT_RESET_REG                     ;        /* Offset = 0x1FF0 */
    uint32_t    IRQ_EOI_REG                        ;        /* Offset = 0x1FF4 */
    uint32_t    TRNG_IRQSTATUS                     ;        /* Offset = 0x1FF8 */
} TRNGRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_TRNG_H */
/* END OF REG_TRNG_H */

