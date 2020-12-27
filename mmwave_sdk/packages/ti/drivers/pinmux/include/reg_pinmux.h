/**
 *  @file    reg_pinmux.h
 *
 *  @brief
 *    This file gives register definitions of MSS_IOMUX module.
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

#ifndef REG_PINMUX_H
#define REG_PINMUX_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 1 /* consolidate all bitfields into one single set */
/* Definition for field func_sel in Register PADAA_cfg_reg */
#define PADS_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADS_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADS_cfg_reg */
#define PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADS_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADS_cfg_reg */
#define PADS_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADS_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADS_cfg_reg */
#define PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADS_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADS_cfg_reg */
#define PADS_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADS_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADS_cfg_reg */
#define PADS_CFG_REG_PI_BIT_START                          8U
#define PADS_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADS_cfg_reg */
#define PADS_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADS_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADS_cfg_reg */
#define PADS_CFG_REG_SC1_BIT_START                         10U
#define PADS_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADS_cfg_reg */
#define PADS_CFG_REG_NU_BIT_START                          11U
#define PADS_CFG_REG_NU_BIT_END                            31U


/**
 * @struct PINMUXRegs_t
 * @brief
 *   Module MSS_IOMUX Register Definition
 * @details
 *   This structure is used to access the MSS_IOMUX module registers.
 */
/**
 * @typedef PINMUXRegs
 * @brief
 *   Module MSS_IOMUX Register Frame type Definition
 * @details
 *   This type is used to access the MSS_IOMUX module registers.
 */
typedef volatile struct PINMUXRegs_t
{
    uint32_t    PADxx_CFG_REG[49]                  ;        /* Offset = 0x000 - 0x0C4*/
    uint32_t    RESTRICTED1[75]                    ;        /* Offset = 0x0C4 */
    uint32_t    USERMODEEN                         ;        /* Offset = 0x1F0 */
    uint32_t    PADGLBLCFGREG                      ;        /* Offset = 0x1F4 */
    uint32_t    IOCFGKICK0                         ;        /* Offset = 0x1F8 */
    uint32_t    IOCFGKICK1                         ;        /* Offset = 0x1FC */
} PINMUXRegs;

#else
/* Definition for field func_sel in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAA_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAA_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAA_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAA_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAA_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_PI_BIT_START                          8U
#define PADAA_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAA_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_SC1_BIT_START                         10U
#define PADAA_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAA_cfg_reg */
#define PADAA_CFG_REG_NU_BIT_START                          11U
#define PADAA_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAB_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAB_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAB_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAB_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAB_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_PI_BIT_START                          8U
#define PADAB_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAB_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_SC1_BIT_START                         10U
#define PADAB_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAB_cfg_reg */
#define PADAB_CFG_REG_NU_BIT_START                          11U
#define PADAB_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAC_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAC_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAC_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAC_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAC_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_PI_BIT_START                          8U
#define PADAC_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAC_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_SC1_BIT_START                         10U
#define PADAC_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAC_cfg_reg */
#define PADAC_CFG_REG_NU_BIT_START                          11U
#define PADAC_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAD_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAD_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAD_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAD_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAD_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_PI_BIT_START                          8U
#define PADAD_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAD_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_SC1_BIT_START                         10U
#define PADAD_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAD_cfg_reg */
#define PADAD_CFG_REG_NU_BIT_START                          11U
#define PADAD_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAE_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAE_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAE_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAE_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAE_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_PI_BIT_START                          8U
#define PADAE_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAE_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_SC1_BIT_START                         10U
#define PADAE_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAE_cfg_reg */
#define PADAE_CFG_REG_NU_BIT_START                          11U
#define PADAE_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAF_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAF_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAF_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAF_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAF_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_PI_BIT_START                          8U
#define PADAF_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAF_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_SC1_BIT_START                         10U
#define PADAF_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAF_cfg_reg */
#define PADAF_CFG_REG_NU_BIT_START                          11U
#define PADAF_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAG_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAG_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAG_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAG_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAG_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_PI_BIT_START                          8U
#define PADAG_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAG_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_SC1_BIT_START                         10U
#define PADAG_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAG_cfg_reg */
#define PADAG_CFG_REG_NU_BIT_START                          11U
#define PADAG_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAH_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAH_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAH_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAH_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAH_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_PI_BIT_START                          8U
#define PADAH_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAH_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_SC1_BIT_START                         10U
#define PADAH_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAH_cfg_reg */
#define PADAH_CFG_REG_NU_BIT_START                          11U
#define PADAH_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAI_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAI_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAI_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAI_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAI_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_PI_BIT_START                          8U
#define PADAI_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAI_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_SC1_BIT_START                         10U
#define PADAI_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAI_cfg_reg */
#define PADAI_CFG_REG_NU_BIT_START                          11U
#define PADAI_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAJ_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAJ_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAJ_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAJ_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAJ_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_PI_BIT_START                          8U
#define PADAJ_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAJ_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_SC1_BIT_START                         10U
#define PADAJ_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAJ_cfg_reg */
#define PADAJ_CFG_REG_NU_BIT_START                          11U
#define PADAJ_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAK_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAK_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAK_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAK_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAK_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_PI_BIT_START                          8U
#define PADAK_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAK_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_SC1_BIT_START                         10U
#define PADAK_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAK_cfg_reg */
#define PADAK_CFG_REG_NU_BIT_START                          11U
#define PADAK_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAL_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAL_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAL_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAL_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAL_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_PI_BIT_START                          8U
#define PADAL_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAL_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_SC1_BIT_START                         10U
#define PADAL_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAL_cfg_reg */
#define PADAL_CFG_REG_NU_BIT_START                          11U
#define PADAL_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAM_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAM_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAM_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAM_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAM_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_PI_BIT_START                          8U
#define PADAM_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAM_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_SC1_BIT_START                         10U
#define PADAM_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAM_cfg_reg */
#define PADAM_CFG_REG_NU_BIT_START                          11U
#define PADAM_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAN_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAN_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAN_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAN_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAN_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_PI_BIT_START                          8U
#define PADAN_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAN_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_SC1_BIT_START                         10U
#define PADAN_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAN_cfg_reg */
#define PADAN_CFG_REG_NU_BIT_START                          11U
#define PADAN_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAO_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAO_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAO_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAO_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAO_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_PI_BIT_START                          8U
#define PADAO_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAO_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_SC1_BIT_START                         10U
#define PADAO_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAO_cfg_reg */
#define PADAO_CFG_REG_NU_BIT_START                          11U
#define PADAO_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAP_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAP_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAP_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAP_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAP_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_PI_BIT_START                          8U
#define PADAP_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAP_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_SC1_BIT_START                         10U
#define PADAP_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAP_cfg_reg */
#define PADAP_CFG_REG_NU_BIT_START                          11U
#define PADAP_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAQ_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAQ_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAQ_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAQ_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAQ_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_PI_BIT_START                          8U
#define PADAQ_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAQ_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_SC1_BIT_START                         10U
#define PADAQ_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAQ_cfg_reg */
#define PADAQ_CFG_REG_NU_BIT_START                          11U
#define PADAQ_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAR_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAR_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAR_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAR_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAR_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_PI_BIT_START                          8U
#define PADAR_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAR_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_SC1_BIT_START                         10U
#define PADAR_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAR_cfg_reg */
#define PADAR_CFG_REG_NU_BIT_START                          11U
#define PADAR_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAS_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAS_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAS_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAS_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAS_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_PI_BIT_START                          8U
#define PADAS_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAS_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_SC1_BIT_START                         10U
#define PADAS_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAS_cfg_reg */
#define PADAS_CFG_REG_NU_BIT_START                          11U
#define PADAS_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAT_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAT_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAT_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAT_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAT_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_PI_BIT_START                          8U
#define PADAT_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAT_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_SC1_BIT_START                         10U
#define PADAT_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAT_cfg_reg */
#define PADAT_CFG_REG_NU_BIT_START                          11U
#define PADAT_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAU_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAU_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAU_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAU_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAU_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_PI_BIT_START                          8U
#define PADAU_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAU_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_SC1_BIT_START                         10U
#define PADAU_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAU_cfg_reg */
#define PADAU_CFG_REG_NU_BIT_START                          11U
#define PADAU_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAV_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAV_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAV_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAV_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAV_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_PI_BIT_START                          8U
#define PADAV_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAV_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_SC1_BIT_START                         10U
#define PADAV_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAV_cfg_reg */
#define PADAV_CFG_REG_NU_BIT_START                          11U
#define PADAV_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAW_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAW_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAW_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAW_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAW_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_PI_BIT_START                          8U
#define PADAW_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAW_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_SC1_BIT_START                         10U
#define PADAW_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAW_cfg_reg */
#define PADAW_CFG_REG_NU_BIT_START                          11U
#define PADAW_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAX_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAX_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAX_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAX_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAX_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_PI_BIT_START                          8U
#define PADAX_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAX_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_SC1_BIT_START                         10U
#define PADAX_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAX_cfg_reg */
#define PADAX_CFG_REG_NU_BIT_START                          11U
#define PADAX_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAY_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAY_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAY_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAY_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAY_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_PI_BIT_START                          8U
#define PADAY_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAY_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_SC1_BIT_START                         10U
#define PADAY_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAY_cfg_reg */
#define PADAY_CFG_REG_NU_BIT_START                          11U
#define PADAY_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADAZ_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADAZ_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADAZ_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADAZ_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADAZ_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_PI_BIT_START                          8U
#define PADAZ_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADAZ_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_SC1_BIT_START                         10U
#define PADAZ_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADAZ_cfg_reg */
#define PADAZ_CFG_REG_NU_BIT_START                          11U
#define PADAZ_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBA_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBA_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBA_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBA_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBA_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_PI_BIT_START                          8U
#define PADBA_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBA_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_SC1_BIT_START                         10U
#define PADBA_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBA_cfg_reg */
#define PADBA_CFG_REG_NU_BIT_START                          11U
#define PADBA_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBB_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBB_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBB_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBB_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBB_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_PI_BIT_START                          8U
#define PADBB_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBB_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_SC1_BIT_START                         10U
#define PADBB_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBB_cfg_reg */
#define PADBB_CFG_REG_NU_BIT_START                          11U
#define PADBB_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBC_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBC_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBC_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBC_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBC_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_PI_BIT_START                          8U
#define PADBC_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBC_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_SC1_BIT_START                         10U
#define PADBC_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBC_cfg_reg */
#define PADBC_CFG_REG_NU_BIT_START                          11U
#define PADBC_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBD_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBD_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBD_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBD_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBD_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_PI_BIT_START                          8U
#define PADBD_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBD_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_SC1_BIT_START                         10U
#define PADBD_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBD_cfg_reg */
#define PADBD_CFG_REG_NU_BIT_START                          11U
#define PADBD_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBE_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBE_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBE_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBE_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBE_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_PI_BIT_START                          8U
#define PADBE_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBE_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_SC1_BIT_START                         10U
#define PADBE_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBE_cfg_reg */
#define PADBE_CFG_REG_NU_BIT_START                          11U
#define PADBE_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBF_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBF_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBF_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBF_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBF_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_PI_BIT_START                          8U
#define PADBF_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBF_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_SC1_BIT_START                         10U
#define PADBF_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBF_cfg_reg */
#define PADBF_CFG_REG_NU_BIT_START                          11U
#define PADBF_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBG_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBG_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBG_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBG_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBG_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_PI_BIT_START                          8U
#define PADBG_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBG_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_SC1_BIT_START                         10U
#define PADBG_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBG_cfg_reg */
#define PADBG_CFG_REG_NU_BIT_START                          11U
#define PADBG_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBH_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBH_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBH_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBH_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBH_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_PI_BIT_START                          8U
#define PADBH_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBH_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_SC1_BIT_START                         10U
#define PADBH_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBH_cfg_reg */
#define PADBH_CFG_REG_NU_BIT_START                          11U
#define PADBH_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBI_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBI_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBI_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBI_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBI_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_PI_BIT_START                          8U
#define PADBI_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBI_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_SC1_BIT_START                         10U
#define PADBI_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBI_cfg_reg */
#define PADBI_CFG_REG_NU_BIT_START                          11U
#define PADBI_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBJ_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBJ_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBJ_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBJ_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBJ_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_PI_BIT_START                          8U
#define PADBJ_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBJ_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_SC1_BIT_START                         10U
#define PADBJ_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBJ_cfg_reg */
#define PADBJ_CFG_REG_NU_BIT_START                          11U
#define PADBJ_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBK_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBK_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBK_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBK_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBK_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_PI_BIT_START                          8U
#define PADBK_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBK_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_SC1_BIT_START                         10U
#define PADBK_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBK_cfg_reg */
#define PADBK_CFG_REG_NU_BIT_START                          11U
#define PADBK_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBL_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBL_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBL_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBL_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBL_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_PI_BIT_START                          8U
#define PADBL_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBL_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_SC1_BIT_START                         10U
#define PADBL_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBL_cfg_reg */
#define PADBL_CFG_REG_NU_BIT_START                          11U
#define PADBL_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBM_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBM_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBM_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBM_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBM_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_PI_BIT_START                          8U
#define PADBM_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBM_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_SC1_BIT_START                         10U
#define PADBM_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBM_cfg_reg */
#define PADBM_CFG_REG_NU_BIT_START                          11U
#define PADBM_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBN_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBN_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBN_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBN_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBN_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_PI_BIT_START                          8U
#define PADBN_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBN_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_SC1_BIT_START                         10U
#define PADBN_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBN_cfg_reg */
#define PADBN_CFG_REG_NU_BIT_START                          11U
#define PADBN_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBO_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBO_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBO_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBO_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBO_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_PI_BIT_START                          8U
#define PADBO_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBO_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_SC1_BIT_START                         10U
#define PADBO_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBO_cfg_reg */
#define PADBO_CFG_REG_NU_BIT_START                          11U
#define PADBO_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBP_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBP_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBP_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBP_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBP_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_PI_BIT_START                          8U
#define PADBP_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBP_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_SC1_BIT_START                         10U
#define PADBP_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBP_cfg_reg */
#define PADBP_CFG_REG_NU_BIT_START                          11U
#define PADBP_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBQ_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBQ_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBQ_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBQ_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBQ_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_PI_BIT_START                          8U
#define PADBQ_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBQ_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_SC1_BIT_START                         10U
#define PADBQ_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBQ_cfg_reg */
#define PADBQ_CFG_REG_NU_BIT_START                          11U
#define PADBQ_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBR_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBR_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBR_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBR_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBR_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_PI_BIT_START                          8U
#define PADBR_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBR_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_SC1_BIT_START                         10U
#define PADBR_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBR_cfg_reg */
#define PADBR_CFG_REG_NU_BIT_START                          11U
#define PADBR_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBS_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBS_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBS_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBS_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBS_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_PI_BIT_START                          8U
#define PADBS_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBS_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_SC1_BIT_START                         10U
#define PADBS_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBS_cfg_reg */
#define PADBS_CFG_REG_NU_BIT_START                          11U
#define PADBS_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBT_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBT_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBT_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBT_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBT_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_PI_BIT_START                          8U
#define PADBT_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBT_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_SC1_BIT_START                         10U
#define PADBT_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBT_cfg_reg */
#define PADBT_CFG_REG_NU_BIT_START                          11U
#define PADBT_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBU_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBU_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBU_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBU_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBU_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_PI_BIT_START                          8U
#define PADBU_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBU_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_SC1_BIT_START                         10U
#define PADBU_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBU_cfg_reg */
#define PADBU_CFG_REG_NU_BIT_START                          11U
#define PADBU_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBV_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBV_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBV_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBV_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBV_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_PI_BIT_START                          8U
#define PADBV_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBV_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_SC1_BIT_START                         10U
#define PADBV_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBV_cfg_reg */
#define PADBV_CFG_REG_NU_BIT_START                          11U
#define PADBV_CFG_REG_NU_BIT_END                            31U

/* Definition for field func_sel in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_FUNC_SEL_BIT_START                    0U
#define PADBW_CFG_REG_FUNC_SEL_BIT_END                      3U

/* Definition for field ie_override_ctrl in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_IE_OVERRIDE_CTRL_BIT_START            4U
#define PADBW_CFG_REG_IE_OVERRIDE_CTRL_BIT_END              4U

/* Definition for field ie_override in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_IE_OVERRIDE_BIT_START                 5U
#define PADBW_CFG_REG_IE_OVERRIDE_BIT_END                   5U

/* Definition for field oe_override_ctrl in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_OE_OVERRIDE_CTRL_BIT_START            6U
#define PADBW_CFG_REG_OE_OVERRIDE_CTRL_BIT_END              6U

/* Definition for field oe_override in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_OE_OVERRIDE_BIT_START                 7U
#define PADBW_CFG_REG_OE_OVERRIDE_BIT_END                   7U

/* Definition for field pi in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_PI_BIT_START                          8U
#define PADBW_CFG_REG_PI_BIT_END                            8U

/* Definition for field pupdsel in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_PUPDSEL_BIT_START                     9U
#define PADBW_CFG_REG_PUPDSEL_BIT_END                       9U

/* Definition for field sc1 in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_SC1_BIT_START                         10U
#define PADBW_CFG_REG_SC1_BIT_END                           10U

/* Definition for field NU in Register PADBW_cfg_reg */
#define PADBW_CFG_REG_NU_BIT_START                          11U
#define PADBW_CFG_REG_NU_BIT_END                            31U

/* Definition for field USERMODEEN in Register USERMODEEN */
#define USERMODEEN_USERMODEEN_BIT_START                     0U
#define USERMODEEN_USERMODEEN_BIT_END                       31U

/* Definition for field PADGLBLCFGREG in Register PADGLBLCFGREG */
#define PADGLBLCFGREG_PADGLBLCFGREG_BIT_START               0U
#define PADGLBLCFGREG_PADGLBLCFGREG_BIT_END                 31U

/* Definition for field IOCFGKICK0 in Register IOCFGKICK0 */
#define IOCFGKICK0_IOCFGKICK0_BIT_START                     0U
#define IOCFGKICK0_IOCFGKICK0_BIT_END                       31U

/* Definition for field IOCFGKICK1 in Register IOCFGKICK1 */
#define IOCFGKICK1_IOCFGKICK1_BIT_START                     0U
#define IOCFGKICK1_IOCFGKICK1_BIT_END                       31U

/**
 * @struct PINMUXRegs_t
 * @brief
 *   Module MSS_IOMUX Register Definition
 * @details
 *   This structure is used to access the MSS_IOMUX module registers.
 */
/**
 * @typedef PINMUXRegs
 * @brief
 *   Module MSS_IOMUX Register Frame type Definition
 * @details
 *   This type is used to access the MSS_IOMUX module registers.
 */
typedef volatile struct PINMUXRegs_t
{
    uint32_t    PADAA_CFG_REG                      ;        /* Offset = 0x000 */
    uint32_t    PADAB_CFG_REG                      ;        /* Offset = 0x004 */
    uint32_t    PADAC_CFG_REG                      ;        /* Offset = 0x008 */
    uint32_t    PADAD_CFG_REG                      ;        /* Offset = 0x00C */
    uint32_t    PADAE_CFG_REG                      ;        /* Offset = 0x010 */
    uint32_t    PADAF_CFG_REG                      ;        /* Offset = 0x014 */
    uint32_t    PADAG_CFG_REG                      ;        /* Offset = 0x018 */
    uint32_t    PADAH_CFG_REG                      ;        /* Offset = 0x01C */
    uint32_t    PADAI_CFG_REG                      ;        /* Offset = 0x020 */
    uint32_t    PADAJ_CFG_REG                      ;        /* Offset = 0x024 */
    uint32_t    PADAK_CFG_REG                      ;        /* Offset = 0x028 */
    uint32_t    PADAL_CFG_REG                      ;        /* Offset = 0x02C */
    uint32_t    PADAM_CFG_REG                      ;        /* Offset = 0x030 */
    uint32_t    PADAN_CFG_REG                      ;        /* Offset = 0x034 */
    uint32_t    PADAO_CFG_REG                      ;        /* Offset = 0x038 */
    uint32_t    PADAP_CFG_REG                      ;        /* Offset = 0x03C */
    uint32_t    PADAQ_CFG_REG                      ;        /* Offset = 0x040 */
    uint32_t    PADAR_CFG_REG                      ;        /* Offset = 0x044 */
    uint32_t    PADAS_CFG_REG                      ;        /* Offset = 0x048 */
    uint32_t    PADAT_CFG_REG                      ;        /* Offset = 0x04C */
    uint32_t    PADAU_CFG_REG                      ;        /* Offset = 0x050 */
    uint32_t    PADAV_CFG_REG                      ;        /* Offset = 0x054 */
    uint32_t    PADAW_CFG_REG                      ;        /* Offset = 0x058 */
    uint32_t    PADAX_CFG_REG                      ;        /* Offset = 0x05C */
    uint32_t    PADAY_CFG_REG                      ;        /* Offset = 0x060 */
    uint32_t    PADAZ_CFG_REG                      ;        /* Offset = 0x064 */
    uint32_t    PADBA_CFG_REG                      ;        /* Offset = 0x068 */
    uint32_t    PADBB_CFG_REG                      ;        /* Offset = 0x06C */
    uint32_t    PADBC_CFG_REG                      ;        /* Offset = 0x070 */
    uint32_t    PADBD_CFG_REG                      ;        /* Offset = 0x074 */
    uint32_t    PADBE_CFG_REG                      ;        /* Offset = 0x078 */
    uint32_t    PADBF_CFG_REG                      ;        /* Offset = 0x07C */
    uint32_t    PADBG_CFG_REG                      ;        /* Offset = 0x080 */
    uint32_t    PADBH_CFG_REG                      ;        /* Offset = 0x084 */
    uint32_t    PADBI_CFG_REG                      ;        /* Offset = 0x088 */
    uint32_t    PADBJ_CFG_REG                      ;        /* Offset = 0x08C */
    uint32_t    PADBK_CFG_REG                      ;        /* Offset = 0x090 */
    uint32_t    PADBL_CFG_REG                      ;        /* Offset = 0x094 */
    uint32_t    PADBM_CFG_REG                      ;        /* Offset = 0x098 */
    uint32_t    PADBN_CFG_REG                      ;        /* Offset = 0x09C */
    uint32_t    PADBO_CFG_REG                      ;        /* Offset = 0x0A0 */
    uint32_t    PADBP_CFG_REG                      ;        /* Offset = 0x0A4 */
    uint32_t    PADBQ_CFG_REG                      ;        /* Offset = 0x0A8 */
    uint32_t    PADBR_CFG_REG                      ;        /* Offset = 0x0AC */
    uint32_t    PADBS_CFG_REG                      ;        /* Offset = 0x0B0 */
    uint32_t    PADBT_CFG_REG                      ;        /* Offset = 0x0B4 */
    uint32_t    PADBU_CFG_REG                      ;        /* Offset = 0x0B8 */
    uint32_t    PADBV_CFG_REG                      ;        /* Offset = 0x0BC */
    uint32_t    PADBW_CFG_REG                      ;        /* Offset = 0x0C0 */
    uint32_t    RESTRICTED1[75]                    ;        /* Offset = 0x0C4 */
    uint32_t    USERMODEEN                         ;        /* Offset = 0x1F0 */
    uint32_t    PADGLBLCFGREG                      ;        /* Offset = 0x1F4 */
    uint32_t    IOCFGKICK0                         ;        /* Offset = 0x1F8 */
    uint32_t    IOCFGKICK1                         ;        /* Offset = 0x1FC */
} PINMUXRegs;
#endif

#ifdef __cplusplus
}
#endif

#endif /* REG_PINMUX_H */
/* END OF REG_PINMUX_H */

