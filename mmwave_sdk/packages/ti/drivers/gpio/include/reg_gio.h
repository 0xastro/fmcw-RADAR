/**
 *  @file    reg_gio.h
 *
 *  @brief
 *    This file gives register definitions of MSS_GIO module.
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

#ifndef REG_GIO_H
#define REG_GIO_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup GPIO_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/* Definition for field RESET in Register GIOGCR */
#define GIOGCR_RESET_BIT_START                              0U
#define GIOGCR_RESET_BIT_END                                0U

/* Definition for field NU0 in Register GIOGCR */
#define GIOGCR_NU0_BIT_START                                1U
#define GIOGCR_NU0_BIT_END                                  31U

/* Definition for field GIOINTDET_0 in Register GIOINTDET */
#define GIOINTDET_GIOINTDET_0_BIT_START                     0U
#define GIOINTDET_GIOINTDET_0_BIT_END                       7U

/* Definition for field GIOINTDET_1 in Register GIOINTDET */
#define GIOINTDET_GIOINTDET_1_BIT_START                     8U
#define GIOINTDET_GIOINTDET_1_BIT_END                       15U

/* Definition for field GIOINTDET_2 in Register GIOINTDET */
#define GIOINTDET_GIOINTDET_2_BIT_START                     16U
#define GIOINTDET_GIOINTDET_2_BIT_END                       23U

/* Definition for field GIOINTDET_3 in Register GIOINTDET */
#define GIOINTDET_GIOINTDET_3_BIT_START                     24U
#define GIOINTDET_GIOINTDET_3_BIT_END                       31U

/* Definition for field GIOPOL_0 in Register GIOPOL */
#define GIOPOL_GIOPOL_0_BIT_START                           0U
#define GIOPOL_GIOPOL_0_BIT_END                             7U

/* Definition for field GIOPOL_1 in Register GIOPOL */
#define GIOPOL_GIOPOL_1_BIT_START                           8U
#define GIOPOL_GIOPOL_1_BIT_END                             15U

/* Definition for field GIOPOL_2 in Register GIOPOL */
#define GIOPOL_GIOPOL_2_BIT_START                           16U
#define GIOPOL_GIOPOL_2_BIT_END                             23U

/* Definition for field GIOPOL_3 in Register GIOPOL */
#define GIOPOL_GIOPOL_3_BIT_START                           24U
#define GIOPOL_GIOPOL_3_BIT_END                             31U

/* Definition for field GIOENASET_0 in Register GIOENASET */
#define GIOENASET_GIOENASET_0_BIT_START                     0U
#define GIOENASET_GIOENASET_0_BIT_END                       7U

/* Definition for field GIOENASET_1 in Register GIOENASET */
#define GIOENASET_GIOENASET_1_BIT_START                     8U
#define GIOENASET_GIOENASET_1_BIT_END                       15U

/* Definition for field GIOENASET_2 in Register GIOENASET */
#define GIOENASET_GIOENASET_2_BIT_START                     16U
#define GIOENASET_GIOENASET_2_BIT_END                       23U

/* Definition for field GIOENASET_3 in Register GIOENASET */
#define GIOENASET_GIOENASET_3_BIT_START                     24U
#define GIOENASET_GIOENASET_3_BIT_END                       31U

/* Definition for field GIOENACLR_0 in Register GIOENACLR */
#define GIOENACLR_GIOENACLR_0_BIT_START                     0U
#define GIOENACLR_GIOENACLR_0_BIT_END                       7U

/* Definition for field GIOENACLR_1 in Register GIOENACLR */
#define GIOENACLR_GIOENACLR_1_BIT_START                     8U
#define GIOENACLR_GIOENACLR_1_BIT_END                       15U

/* Definition for field GIOENACLR_2 in Register GIOENACLR */
#define GIOENACLR_GIOENACLR_2_BIT_START                     16U
#define GIOENACLR_GIOENACLR_2_BIT_END                       23U

/* Definition for field GIOENACLR_3 in Register GIOENACLR */
#define GIOENACLR_GIOENACLR_3_BIT_START                     24U
#define GIOENACLR_GIOENACLR_3_BIT_END                       31U

/* Definition for field GIOLVLSET_0 in Register GIOLVLSET */
#define GIOLVLSET_GIOLVLSET_0_BIT_START                     0U
#define GIOLVLSET_GIOLVLSET_0_BIT_END                       7U

/* Definition for field GIOLVLSET_1 in Register GIOLVLSET */
#define GIOLVLSET_GIOLVLSET_1_BIT_START                     8U
#define GIOLVLSET_GIOLVLSET_1_BIT_END                       15U

/* Definition for field GIOLVLSET_2 in Register GIOLVLSET */
#define GIOLVLSET_GIOLVLSET_2_BIT_START                     16U
#define GIOLVLSET_GIOLVLSET_2_BIT_END                       23U

/* Definition for field GIOLVLSET_3 in Register GIOLVLSET */
#define GIOLVLSET_GIOLVLSET_3_BIT_START                     24U
#define GIOLVLSET_GIOLVLSET_3_BIT_END                       31U

/* Definition for field GIOLVLCLR_0 in Register GIOLVLCLR */
#define GIOLVLCLR_GIOLVLCLR_0_BIT_START                     0U
#define GIOLVLCLR_GIOLVLCLR_0_BIT_END                       7U

/* Definition for field GIOLVLCLR_1 in Register GIOLVLCLR */
#define GIOLVLCLR_GIOLVLCLR_1_BIT_START                     8U
#define GIOLVLCLR_GIOLVLCLR_1_BIT_END                       15U

/* Definition for field GIOLVLCLR_2 in Register GIOLVLCLR */
#define GIOLVLCLR_GIOLVLCLR_2_BIT_START                     16U
#define GIOLVLCLR_GIOLVLCLR_2_BIT_END                       23U

/* Definition for field GIOLVLCLR_3 in Register GIOLVLCLR */
#define GIOLVLCLR_GIOLVLCLR_3_BIT_START                     24U
#define GIOLVLCLR_GIOLVLCLR_3_BIT_END                       31U

/* Definition for field GIOFLG_0 in Register GIOFLG */
#define GIOFLG_GIOFLG_0_BIT_START                           0U
#define GIOFLG_GIOFLG_0_BIT_END                             7U

/* Definition for field GIOFLG_1 in Register GIOFLG */
#define GIOFLG_GIOFLG_1_BIT_START                           8U
#define GIOFLG_GIOFLG_1_BIT_END                             15U

/* Definition for field GIOFLG_2 in Register GIOFLG */
#define GIOFLG_GIOFLG_2_BIT_START                           16U
#define GIOFLG_GIOFLG_2_BIT_END                             23U

/* Definition for field GIOFLG_3 in Register GIOFLG */
#define GIOFLG_GIOFLG_3_BIT_START                           24U
#define GIOFLG_GIOFLG_3_BIT_END                             31U

/* Definition for field GIOOFFA in Register GIOOFFA */
#define GIOOFFA_GIOOFFA_BIT_START                           0U
#define GIOOFFA_GIOOFFA_BIT_END                             5U

/* Definition for field NU1 in Register GIOOFFA */
#define GIOOFFA_NU1_BIT_START                               6U
#define GIOOFFA_NU1_BIT_END                                 31U

/* Definition for field GIOOFFB in Register GIOOFFB */
#define GIOOFFB_GIOOFFB_BIT_START                           0U
#define GIOOFFB_GIOOFFB_BIT_END                             5U

/* Definition for field NU2 in Register GIOOFFB */
#define GIOOFFB_NU2_BIT_START                               6U
#define GIOOFFB_NU2_BIT_END                                 31U

/* Definition for field GIOEMUA in Register GIOEMUA */
#define GIOEMUA_GIOEMUA_BIT_START                           0U
#define GIOEMUA_GIOEMUA_BIT_END                             5U

/* Definition for field NU3 in Register GIOEMUA */
#define GIOEMUA_NU3_BIT_START                               6U
#define GIOEMUA_NU3_BIT_END                                 31U

/* Definition for field GIOEMUB in Register GIOEMUB */
#define GIOEMUB_GIOEMUB_BIT_START                           0U
#define GIOEMUB_GIOEMUB_BIT_END                             5U

/* Definition for field NU4 in Register GIOEMUB */
#define GIOEMUB_NU4_BIT_START                               6U
#define GIOEMUB_NU4_BIT_END                                 31U

/* Definition for field GIODIRA in Register GIODIRA */
#define GIODIRA_GIODIRA_BIT_START                           0U
#define GIODIRA_GIODIRA_BIT_END                             7U

/* Definition for field NU5 in Register GIODIRA */
#define GIODIRA_NU5_BIT_START                               8U
#define GIODIRA_NU5_BIT_END                                 31U

/* Definition for field GIODINA in Register GIODINA */
#define GIODINA_GIODINA_BIT_START                           0U
#define GIODINA_GIODINA_BIT_END                             7U

/* Definition for field NU11 in Register GIODINA */
#define GIODINA_NU11_BIT_START                              8U
#define GIODINA_NU11_BIT_END                                31U

/* Definition for field GIODOUTA in Register GIODOUTA */
#define GIODOUTA_GIODOUTA_BIT_START                         0U
#define GIODOUTA_GIODOUTA_BIT_END                           7U

/* Definition for field NU17 in Register GIODOUTA */
#define GIODOUTA_NU17_BIT_START                             8U
#define GIODOUTA_NU17_BIT_END                               31U

/* Definition for field GIODSETA in Register GIOSETA */
#define GIOSETA_GIODSETA_BIT_START                          0U
#define GIOSETA_GIODSETA_BIT_END                            7U

/* Definition for field NU23 in Register GIOSETA */
#define GIOSETA_NU23_BIT_START                              8U
#define GIOSETA_NU23_BIT_END                                31U

/* Definition for field GIODCLRA in Register GIOCLRA */
#define GIOCLRA_GIODCLRA_BIT_START                          0U
#define GIOCLRA_GIODCLRA_BIT_END                            7U

/* Definition for field NU29 in Register GIOCLRA */
#define GIOCLRA_NU29_BIT_START                              8U
#define GIOCLRA_NU29_BIT_END                                31U

/* Definition for field GIOPDRA in Register GIOPDRA */
#define GIOPDRA_GIOPDRA_BIT_START                           0U
#define GIOPDRA_GIOPDRA_BIT_END                             7U

/* Definition for field NU35 in Register GIOPDRA */
#define GIOPDRA_NU35_BIT_START                              8U
#define GIOPDRA_NU35_BIT_END                                31U

/* Definition for field GIODIRB in Register GIODIRB */
#define GIODIRB_GIODIRB_BIT_START                           0U
#define GIODIRB_GIODIRB_BIT_END                             7U

/* Definition for field NU6 in Register GIODIRB */
#define GIODIRB_NU6_BIT_START                               8U
#define GIODIRB_NU6_BIT_END                                 31U

/* Definition for field GIODINB in Register GIODINB */
#define GIODINB_GIODINB_BIT_START                           0U
#define GIODINB_GIODINB_BIT_END                             7U

/* Definition for field NU12 in Register GIODINB */
#define GIODINB_NU12_BIT_START                              8U
#define GIODINB_NU12_BIT_END                                31U

/* Definition for field GIODOUTB in Register GIODOUTB */
#define GIODOUTB_GIODOUTB_BIT_START                         0U
#define GIODOUTB_GIODOUTB_BIT_END                           7U

/* Definition for field NU18 in Register GIODOUTB */
#define GIODOUTB_NU18_BIT_START                             8U
#define GIODOUTB_NU18_BIT_END                               31U

/* Definition for field GIODSETB in Register GIOSETB */
#define GIOSETB_GIODSETB_BIT_START                          0U
#define GIOSETB_GIODSETB_BIT_END                            7U

/* Definition for field NU24 in Register GIOSETB */
#define GIOSETB_NU24_BIT_START                              8U
#define GIOSETB_NU24_BIT_END                                31U

/* Definition for field GIODCLRB in Register GIOCLRB */
#define GIOCLRB_GIODCLRB_BIT_START                          0U
#define GIOCLRB_GIODCLRB_BIT_END                            7U

/* Definition for field NU30 in Register GIOCLRB */
#define GIOCLRB_NU30_BIT_START                              8U
#define GIOCLRB_NU30_BIT_END                                31U

/* Definition for field GIOPDRB in Register GIOPDRB */
#define GIOPDRB_GIOPDRB_BIT_START                           0U
#define GIOPDRB_GIOPDRB_BIT_END                             7U

/* Definition for field NU36 in Register GIOPDRB */
#define GIOPDRB_NU36_BIT_START                              8U
#define GIOPDRB_NU36_BIT_END                                31U

/* Definition for field GIODIRC in Register GIODIRC */
#define GIODIRC_GIODIRC_BIT_START                           0U
#define GIODIRC_GIODIRC_BIT_END                             7U

/* Definition for field NU7 in Register GIODIRC */
#define GIODIRC_NU7_BIT_START                               8U
#define GIODIRC_NU7_BIT_END                                 31U

/* Definition for field GIODINC in Register GIODINC */
#define GIODINC_GIODINC_BIT_START                           0U
#define GIODINC_GIODINC_BIT_END                             7U

/* Definition for field NU13 in Register GIODINC */
#define GIODINC_NU13_BIT_START                              8U
#define GIODINC_NU13_BIT_END                                31U

/* Definition for field GIODOUTC in Register GIODOUTC */
#define GIODOUTC_GIODOUTC_BIT_START                         0U
#define GIODOUTC_GIODOUTC_BIT_END                           7U

/* Definition for field NU19 in Register GIODOUTC */
#define GIODOUTC_NU19_BIT_START                             8U
#define GIODOUTC_NU19_BIT_END                               31U

/* Definition for field GIODSETC in Register GIOSETC */
#define GIOSETC_GIODSETC_BIT_START                          0U
#define GIOSETC_GIODSETC_BIT_END                            7U

/* Definition for field NU25 in Register GIOSETC */
#define GIOSETC_NU25_BIT_START                              8U
#define GIOSETC_NU25_BIT_END                                31U

/* Definition for field GIODCLRC in Register GIOCLRC */
#define GIOCLRC_GIODCLRC_BIT_START                          0U
#define GIOCLRC_GIODCLRC_BIT_END                            7U

/* Definition for field NU31 in Register GIOCLRC */
#define GIOCLRC_NU31_BIT_START                              8U
#define GIOCLRC_NU31_BIT_END                                31U

/* Definition for field GIOPDRC in Register GIOPDRC */
#define GIOPDRC_GIOPDRC_BIT_START                           0U
#define GIOPDRC_GIOPDRC_BIT_END                             7U

/* Definition for field NU37 in Register GIOPDRC */
#define GIOPDRC_NU37_BIT_START                              8U
#define GIOPDRC_NU37_BIT_END                                31U

/* Definition for field GIODIRD in Register GIODIRD */
#define GIODIRD_GIODIRD_BIT_START                           0U
#define GIODIRD_GIODIRD_BIT_END                             7U

/* Definition for field NU8 in Register GIODIRD */
#define GIODIRD_NU8_BIT_START                               8U
#define GIODIRD_NU8_BIT_END                                 31U

/* Definition for field GIODIND in Register GIODIND */
#define GIODIND_GIODIND_BIT_START                           0U
#define GIODIND_GIODIND_BIT_END                             7U

/* Definition for field NU14 in Register GIODIND */
#define GIODIND_NU14_BIT_START                              8U
#define GIODIND_NU14_BIT_END                                31U

/* Definition for field GIODOUTD in Register GIODOUTD */
#define GIODOUTD_GIODOUTD_BIT_START                         0U
#define GIODOUTD_GIODOUTD_BIT_END                           7U

/* Definition for field NU20 in Register GIODOUTD */
#define GIODOUTD_NU20_BIT_START                             8U
#define GIODOUTD_NU20_BIT_END                               31U

/* Definition for field GIODSETD in Register GIOSETD */
#define GIOSETD_GIODSETD_BIT_START                          0U
#define GIOSETD_GIODSETD_BIT_END                            7U

/* Definition for field NU26 in Register GIOSETD */
#define GIOSETD_NU26_BIT_START                              8U
#define GIOSETD_NU26_BIT_END                                31U

/* Definition for field GIODCLRD in Register GIOCLRD */
#define GIOCLRD_GIODCLRD_BIT_START                          0U
#define GIOCLRD_GIODCLRD_BIT_END                            7U

/* Definition for field NU32 in Register GIOCLRD */
#define GIOCLRD_NU32_BIT_START                              8U
#define GIOCLRD_NU32_BIT_END                                31U

/* Definition for field GIOPDRD in Register GIOPDRD */
#define GIOPDRD_GIOPDRD_BIT_START                           0U
#define GIOPDRD_GIOPDRD_BIT_END                             7U

/* Definition for field NU38 in Register GIOPDRD */
#define GIOPDRD_NU38_BIT_START                              8U
#define GIOPDRD_NU38_BIT_END                                31U

/* Definition for field GIODIRE in Register GIODIRE */
#define GIODIRE_GIODIRE_BIT_START                           0U
#define GIODIRE_GIODIRE_BIT_END                             7U

/* Definition for field NU9 in Register GIODIRE */
#define GIODIRE_NU9_BIT_START                               8U
#define GIODIRE_NU9_BIT_END                                 31U

/* Definition for field GIODINE in Register GIODINE */
#define GIODINE_GIODINE_BIT_START                           0U
#define GIODINE_GIODINE_BIT_END                             7U

/* Definition for field NU15 in Register GIODINE */
#define GIODINE_NU15_BIT_START                              8U
#define GIODINE_NU15_BIT_END                                31U

/* Definition for field GIODOUTE in Register GIODOUTE */
#define GIODOUTE_GIODOUTE_BIT_START                         0U
#define GIODOUTE_GIODOUTE_BIT_END                           7U

/* Definition for field NU21 in Register GIODOUTE */
#define GIODOUTE_NU21_BIT_START                             8U
#define GIODOUTE_NU21_BIT_END                               31U

/* Definition for field GIODSETE in Register GIOSETE */
#define GIOSETE_GIODSETE_BIT_START                          0U
#define GIOSETE_GIODSETE_BIT_END                            7U

/* Definition for field NU27 in Register GIOSETE */
#define GIOSETE_NU27_BIT_START                              8U
#define GIOSETE_NU27_BIT_END                                31U

/* Definition for field GIODCLRE in Register GIOCLRE */
#define GIOCLRE_GIODCLRE_BIT_START                          0U
#define GIOCLRE_GIODCLRE_BIT_END                            7U

/* Definition for field NU33 in Register GIOCLRE */
#define GIOCLRE_NU33_BIT_START                              8U
#define GIOCLRE_NU33_BIT_END                                31U

/* Definition for field GIOPDRE in Register GIOPDRE */
#define GIOPDRE_GIOPDRE_BIT_START                           0U
#define GIOPDRE_GIOPDRE_BIT_END                             7U

/* Definition for field NU39 in Register GIOPDRE */
#define GIOPDRE_NU39_BIT_START                              8U
#define GIOPDRE_NU39_BIT_END                                31U

/* Definition for field GIODIRF in Register GIODIRF */
#define GIODIRF_GIODIRF_BIT_START                           0U
#define GIODIRF_GIODIRF_BIT_END                             7U

/* Definition for field NU10 in Register GIODIRF */
#define GIODIRF_NU10_BIT_START                              8U
#define GIODIRF_NU10_BIT_END                                31U

/* Definition for field GIODINF in Register GIODINF */
#define GIODINF_GIODINF_BIT_START                           0U
#define GIODINF_GIODINF_BIT_END                             7U

/* Definition for field NU16 in Register GIODINF */
#define GIODINF_NU16_BIT_START                              8U
#define GIODINF_NU16_BIT_END                                31U

/* Definition for field GIODOUTF in Register GIODOUTF */
#define GIODOUTF_GIODOUTF_BIT_START                         0U
#define GIODOUTF_GIODOUTF_BIT_END                           7U

/* Definition for field NU22 in Register GIODOUTF */
#define GIODOUTF_NU22_BIT_START                             8U
#define GIODOUTF_NU22_BIT_END                               31U

/* Definition for field GIODSETF in Register GIOSETF */
#define GIOSETF_GIODSETF_BIT_START                          0U
#define GIOSETF_GIODSETF_BIT_END                            7U

/* Definition for field NU28 in Register GIOSETF */
#define GIOSETF_NU28_BIT_START                              8U
#define GIOSETF_NU28_BIT_END                                31U

/* Definition for field GIODCLRF in Register GIOCLRF */
#define GIOCLRF_GIODCLRF_BIT_START                          0U
#define GIOCLRF_GIODCLRF_BIT_END                            7U

/* Definition for field NU34 in Register GIOCLRF */
#define GIOCLRF_NU34_BIT_START                              8U
#define GIOCLRF_NU34_BIT_END                                31U

/* Definition for field GIOPDRF in Register GIOPDRF */
#define GIOPDRF_GIOPDRF_BIT_START                           0U
#define GIOPDRF_GIOPDRF_BIT_END                             7U

/* Definition for field NU40 in Register GIOPDRF */
#define GIOPDRF_NU40_BIT_START                              8U
#define GIOPDRF_NU40_BIT_END                                31U



/**
 * @brief
 *  Only Ports A, B, C and D are capable of interrupts.
 *  This is a system limit on the GPIO IP Block.
 */
#define GPIO_MAX_INTERRUPT_PORT         4U

/**
 * @brief
 *  This is the maximum number of pins per GPIO port
 *  This is a system limit on the GPIO IP Block.
 */
#define GPIO_MAX_PINS_PER_PORT          8U

/**
 * @brief
 *  This is the maximum number of interrupts which can be supported.
 *  Only Ports A, B, C and D are capable of interrupts. Also since
 *  each port can have a maximum of 8 pins this is the maximum number
 *  of interrupts which can be supported
 *
 *  This is a system limit on the GPIO IP Block.
 */
#define GPIO_MAX_INTERRUPT              (GPIO_MAX_INTERRUPT_PORT * GPIO_MAX_PINS_PER_PORT)

/*
 * @brief
 *  This is used to define the number of GPIO ports which are available
 *  and this is device specific.
 */
#ifdef SOC_XWR14XX
#define GPIO_MAX_PORT                  4U
#elif (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
#define GPIO_MAX_PORT                  6U
#else
#error "Error: Check the compiler flags: SOC_XWR14XX , SOC_XWR16XX , SOC_XWR18XX or SOC_XWR68XX is not defined"
#endif

/**
 * @brief
 *  GPIO Port Registers
 *
 * @details
 *  This structure defines the register overlay used for each
 *  GPIO Port.
 */
typedef volatile struct GIOPortRegs_t
{
    uint32_t    GIODIR;
    uint32_t    GIODIN;
    uint32_t    GIODOUT;
    uint32_t    GIODSET;
    uint32_t    GIODCLR;
    uint32_t    GIOPDR;
    uint32_t    RESTRICTED[2];
}GIOPortRegs;

/**
 * @struct GIORegs_t
 * @brief
 *   Module MSS_GIO Register Definition
 * @details
 *   This structure is used to access the MSS_GIO module registers.
 */
/**
 * @typedef GIORegs
 * @brief
 *   Module MSS_GIO Register Frame type Definition
 * @details
 *   This type is used to access the MSS_GIO module registers.
 */
typedef volatile struct GIORegs_t
{
    uint32_t    GIOGCR                             ;        /* Offset = 0x000 */
    uint32_t    RESTRICTED1                        ;        /* Offset = 0x004 */
    uint32_t    GIOINTDET                          ;        /* Offset = 0x008 */
    uint32_t    GIOPOL                             ;        /* Offset = 0x00C */
    uint32_t    GIOENASET                          ;        /* Offset = 0x010 */
    uint32_t    GIOENACLR                          ;        /* Offset = 0x014 */
    uint32_t    GIOLVLSET                          ;        /* Offset = 0x018 */
    uint32_t    GIOLVLCLR                          ;        /* Offset = 0x01C */
    uint32_t    GIOFLG                             ;        /* Offset = 0x020 */
    uint32_t    GIOOFFA                            ;        /* Offset = 0x024 */
    uint32_t    GIOOFFB                            ;        /* Offset = 0x028 */
    uint32_t    GIOEMUA                            ;        /* Offset = 0x02C */
    uint32_t    GIOEMUB                            ;        /* Offset = 0x030 */

    /* NOTE: Modification done manually. The GPIO Port can be written as a structure
     * with N elements. The number of elements can be platform specific. This allows
     * the GPIO driver to work across multiple platforms */
#if 0
    uint32_t    GIODIRA                            ;        /* Offset = 0x034 */
    uint32_t    GIODINA                            ;        /* Offset = 0x038 */
    uint32_t    GIODOUTA                           ;        /* Offset = 0x03C */
    uint32_t    GIOSETA                            ;        /* Offset = 0x040 */
    uint32_t    GIOCLRA                            ;        /* Offset = 0x044 */
    uint32_t    GIOPDRA                            ;        /* Offset = 0x048 */
    uint32_t    RESTRICTED2[2]                     ;        /* Offset = 0x04C */
    uint32_t    GIODIRB                            ;        /* Offset = 0x054 */
    uint32_t    GIODINB                            ;        /* Offset = 0x058 */
    uint32_t    GIODOUTB                           ;        /* Offset = 0x05C */
    uint32_t    GIOSETB                            ;        /* Offset = 0x060 */
    uint32_t    GIOCLRB                            ;        /* Offset = 0x064 */
    uint32_t    GIOPDRB                            ;        /* Offset = 0x068 */
    uint32_t    RESTRICTED3[2]                     ;        /* Offset = 0x06C */
    uint32_t    GIODIRC                            ;        /* Offset = 0x074 */
    uint32_t    GIODINC                            ;        /* Offset = 0x078 */
    uint32_t    GIODOUTC                           ;        /* Offset = 0x07C */
    uint32_t    GIOSETC                            ;        /* Offset = 0x080 */
    uint32_t    GIOCLRC                            ;        /* Offset = 0x084 */
    uint32_t    GIOPDRC                            ;        /* Offset = 0x088 */
    uint32_t    RESTRICTED4[2]                     ;        /* Offset = 0x08C */
    uint32_t    GIODIRD                            ;        /* Offset = 0x094 */
    uint32_t    GIODIND                            ;        /* Offset = 0x098 */
    uint32_t    GIODOUTD                           ;        /* Offset = 0x09C */
    uint32_t    GIOSETD                            ;        /* Offset = 0x0A0 */
    uint32_t    GIOCLRD                            ;        /* Offset = 0x0A4 */
    uint32_t    GIOPDRD                            ;        /* Offset = 0x0A8 */
    uint32_t    RESTRICTED5[2]                     ;        /* Offset = 0x0AC */
    uint32_t    GIODIRE                            ;        /* Offset = 0x0B4 */
    uint32_t    GIODINE                            ;        /* Offset = 0x0B8 */
    uint32_t    GIODOUTE                           ;        /* Offset = 0x0BC */
    uint32_t    GIOSETE                            ;        /* Offset = 0x0C0 */
    uint32_t    GIOCLRE                            ;        /* Offset = 0x0C4 */
    uint32_t    GIOPDRE                            ;        /* Offset = 0x0C8 */
    uint32_t    RESTRICTED6[2]                     ;        /* Offset = 0x0CC */
    uint32_t    GIODIRF                            ;        /* Offset = 0x0D4 */
    uint32_t    GIODINF                            ;        /* Offset = 0x0D8 */
    uint32_t    GIODOUTF                           ;        /* Offset = 0x0DC */
    uint32_t    GIOSETF                            ;        /* Offset = 0x0E0 */
    uint32_t    GIOCLRF                            ;        /* Offset = 0x0E4 */
    uint32_t    GIOPDRF                            ;        /* Offset = 0x0E8 */
#else
    GIOPortRegs portRegs[GPIO_MAX_PORT];
#endif
} GIORegs;

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* REG_GIO_H */
/* END OF REG_GIO_H */


