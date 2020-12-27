/**
 *  @file    reg_i2c.h
 *
 *  @brief
 *    This file gives register definitions of MSS_I2C module.
 *
 *  This file is auto-generated on 3/15/2017.
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

#ifndef REG_I2C_H
#define REG_I2C_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field A9_A0 in Register ICOAR */
#define ICOAR_A9_A0_BIT_START                               0U
#define ICOAR_A9_A0_BIT_END                                 9U

/* Definition for field NU in Register ICOAR */
#define ICOAR_NU_BIT_START                                  10U
#define ICOAR_NU_BIT_END                                    31U

/* Definition for field AL in Register ICIMR */
#define ICIMR_AL_BIT_START                                  0U
#define ICIMR_AL_BIT_END                                    0U

/* Definition for field NACK in Register ICIMR */
#define ICIMR_NACK_BIT_START                                1U
#define ICIMR_NACK_BIT_END                                  1U

/* Definition for field ARDY in Register ICIMR */
#define ICIMR_ARDY_BIT_START                                2U
#define ICIMR_ARDY_BIT_END                                  2U

/* Definition for field ICRRDY in Register ICIMR */
#define ICIMR_ICRRDY_BIT_START                              3U
#define ICIMR_ICRRDY_BIT_END                                3U

/* Definition for field ICXRDY in Register ICIMR */
#define ICIMR_ICXRDY_BIT_START                              4U
#define ICIMR_ICXRDY_BIT_END                                4U

/* Definition for field SCD in Register ICIMR */
#define ICIMR_SCD_BIT_START                                 5U
#define ICIMR_SCD_BIT_END                                   5U

/* Definition for field AAS in Register ICIMR */
#define ICIMR_AAS_BIT_START                                 6U
#define ICIMR_AAS_BIT_END                                   6U

/* Definition for field NU in Register ICIMR */
#define ICIMR_NU_BIT_START                                  7U
#define ICIMR_NU_BIT_END                                    31U

/* Definition for field AL in Register ICSTR */
#define ICSTR_AL_BIT_START                                  0U
#define ICSTR_AL_BIT_END                                    0U

/* Definition for field NACK in Register ICSTR */
#define ICSTR_NACK_BIT_START                                1U
#define ICSTR_NACK_BIT_END                                  1U

/* Definition for field ARDY in Register ICSTR */
#define ICSTR_ARDY_BIT_START                                2U
#define ICSTR_ARDY_BIT_END                                  2U

/* Definition for field ICRRDY in Register ICSTR */
#define ICSTR_ICRRDY_BIT_START                              3U
#define ICSTR_ICRRDY_BIT_END                                3U

/* Definition for field ICXRDY in Register ICSTR */
#define ICSTR_ICXRDY_BIT_START                              4U
#define ICSTR_ICXRDY_BIT_END                                4U

/* Definition for field SCD in Register ICSTR */
#define ICSTR_SCD_BIT_START                                 5U
#define ICSTR_SCD_BIT_END                                   5U

/* Definition for field NU1 in Register ICSTR */
#define ICSTR_NU1_BIT_START                                 6U
#define ICSTR_NU1_BIT_END                                   7U

/* Definition for field AD0 in Register ICSTR */
#define ICSTR_AD0_BIT_START                                 8U
#define ICSTR_AD0_BIT_END                                   8U

/* Definition for field AAS in Register ICSTR */
#define ICSTR_AAS_BIT_START                                 9U
#define ICSTR_AAS_BIT_END                                   9U

/* Definition for field XSMT in Register ICSTR */
#define ICSTR_XSMT_BIT_START                                10U
#define ICSTR_XSMT_BIT_END                                  10U

/* Definition for field RSFULL in Register ICSTR */
#define ICSTR_RSFULL_BIT_START                              11U
#define ICSTR_RSFULL_BIT_END                                11U

/* Definition for field BB in Register ICSTR */
#define ICSTR_BB_BIT_START                                  12U
#define ICSTR_BB_BIT_END                                    12U

/* Definition for field NACKSNT in Register ICSTR */
#define ICSTR_NACKSNT_BIT_START                             13U
#define ICSTR_NACKSNT_BIT_END                               13U

/* Definition for field SDIR in Register ICSTR */
#define ICSTR_SDIR_BIT_START                                14U
#define ICSTR_SDIR_BIT_END                                  14U

/* Definition for field NU2 in Register ICSTR */
#define ICSTR_NU2_BIT_START                                 15U
#define ICSTR_NU2_BIT_END                                   31U

/* Definition for field ICCL15_ICCL0 in Register ICCLKL */
#define ICCLKL_ICCL15_ICCL0_BIT_START                       0U
#define ICCLKL_ICCL15_ICCL0_BIT_END                         15U

/* Definition for field NU in Register ICCLKL */
#define ICCLKL_NU_BIT_START                                 16U
#define ICCLKL_NU_BIT_END                                   31U

/* Definition for field ICCH15_ICCLH0 in Register ICCLKH */
#define ICCLKH_ICCH15_ICCLH0_BIT_START                      0U
#define ICCLKH_ICCH15_ICCLH0_BIT_END                        15U

/* Definition for field NU in Register ICCLKH */
#define ICCLKH_NU_BIT_START                                 16U
#define ICCLKH_NU_BIT_END                                   31U

/* Definition for field ICDC15_ICDC0 in Register ICCNT */
#define ICCNT_ICDC15_ICDC0_BIT_START                        0U
#define ICCNT_ICDC15_ICDC0_BIT_END                          15U

/* Definition for field NU in Register ICCNT */
#define ICCNT_NU_BIT_START                                  16U
#define ICCNT_NU_BIT_END                                    31U

/* Definition for field D7_D0 in Register ICDRR */
#define ICDRR_D7_D0_BIT_START                               0U
#define ICDRR_D7_D0_BIT_END                                 7U

/* Definition for field NU in Register ICDRR */
#define ICDRR_NU_BIT_START                                  8U
#define ICDRR_NU_BIT_END                                    31U

/* Definition for field A9_A0 in Register ICSAR */
#define ICSAR_A9_A0_BIT_START                               0U
#define ICSAR_A9_A0_BIT_END                                 9U

/* Definition for field NU in Register ICSAR */
#define ICSAR_NU_BIT_START                                  10U
#define ICSAR_NU_BIT_END                                    31U

/* Definition for field D7_D0 in Register ICDXR */
#define ICDXR_D7_D0_BIT_START                               0U
#define ICDXR_D7_D0_BIT_END                                 7U

/* Definition for field NU in Register ICDXR */
#define ICDXR_NU_BIT_START                                  8U
#define ICDXR_NU_BIT_END                                    31U

/* Definition for field BC2_BC1_BC0 in Register ICMDR */
#define ICMDR_BC2_BC1_BC0_BIT_START                         0U
#define ICMDR_BC2_BC1_BC0_BIT_END                           2U

/* Definition for field FDF in Register ICMDR */
#define ICMDR_FDF_BIT_START                                 3U
#define ICMDR_FDF_BIT_END                                   3U

/* Definition for field STB in Register ICMDR */
#define ICMDR_STB_BIT_START                                 4U
#define ICMDR_STB_BIT_END                                   4U

/* Definition for field IRS in Register ICMDR */
#define ICMDR_IRS_BIT_START                                 5U
#define ICMDR_IRS_BIT_END                                   5U

/* Definition for field DLB in Register ICMDR */
#define ICMDR_DLB_BIT_START                                 6U
#define ICMDR_DLB_BIT_END                                   6U

/* Definition for field RM in Register ICMDR */
#define ICMDR_RM_BIT_START                                  7U
#define ICMDR_RM_BIT_END                                    7U

/* Definition for field XA in Register ICMDR */
#define ICMDR_XA_BIT_START                                  8U
#define ICMDR_XA_BIT_END                                    8U

/* Definition for field TRX in Register ICMDR */
#define ICMDR_TRX_BIT_START                                 9U
#define ICMDR_TRX_BIT_END                                   9U

/* Definition for field MST in Register ICMDR */
#define ICMDR_MST_BIT_START                                 10U
#define ICMDR_MST_BIT_END                                   10U

/* Definition for field STP in Register ICMDR */
#define ICMDR_STP_BIT_START                                 11U
#define ICMDR_STP_BIT_END                                   11U

/* Definition for field NU1 in Register ICMDR */
#define ICMDR_NU1_BIT_START                                 12U
#define ICMDR_NU1_BIT_END                                   12U

/* Definition for field STT in Register ICMDR */
#define ICMDR_STT_BIT_START                                 13U
#define ICMDR_STT_BIT_END                                   13U

/* Definition for field FREE in Register ICMDR */
#define ICMDR_FREE_BIT_START                                14U
#define ICMDR_FREE_BIT_END                                  14U

/* Definition for field NACKMOD in Register ICMDR */
#define ICMDR_NACKMOD_BIT_START                             15U
#define ICMDR_NACKMOD_BIT_END                               15U

/* Definition for field NU2 in Register ICMDR */
#define ICMDR_NU2_BIT_START                                 16U
#define ICMDR_NU2_BIT_END                                   31U

/* Definition for field INTCODE in Register ICIVR */
#define ICIVR_INTCODE_BIT_START                             0U
#define ICIVR_INTCODE_BIT_END                               2U

/* Definition for field NU1 in Register ICIVR */
#define ICIVR_NU1_BIT_START                                 3U
#define ICIVR_NU1_BIT_END                                   7U

/* Definition for field TESTMD in Register ICIVR */
#define ICIVR_TESTMD_BIT_START                              8U
#define ICIVR_TESTMD_BIT_END                                11U

/* Definition for field NU2 in Register ICIVR */
#define ICIVR_NU2_BIT_START                                 12U
#define ICIVR_NU2_BIT_END                                   31U

/* Definition for field BCM in Register ICEMDR */
#define ICEMDR_BCM_BIT_START                                0U
#define ICEMDR_BCM_BIT_END                                  0U

/* Definition for field IGNACK in Register ICEMDR */
#define ICEMDR_IGNACK_BIT_START                             1U
#define ICEMDR_IGNACK_BIT_END                               1U

/* Definition for field NU in Register ICEMDR */
#define ICEMDR_NU_BIT_START                                 2U
#define ICEMDR_NU_BIT_END                                   31U

/* Definition for field IPSC7_IPSC0 in Register ICPSC */
#define ICPSC_IPSC7_IPSC0_BIT_START                         0U
#define ICPSC_IPSC7_IPSC0_BIT_END                           7U

/* Definition for field NU in Register ICPSC */
#define ICPSC_NU_BIT_START                                  8U
#define ICPSC_NU_BIT_END                                    31U

/* Definition for field REVISION in Register ICPID1 */
#define ICPID1_REVISION_BIT_START                           0U
#define ICPID1_REVISION_BIT_END                             7U

/* Definition for field CLASS in Register ICPID1 */
#define ICPID1_CLASS_BIT_START                              8U
#define ICPID1_CLASS_BIT_END                                15U

/* Definition for field NU in Register ICPID1 */
#define ICPID1_NU_BIT_START                                 16U
#define ICPID1_NU_BIT_END                                   31U

/* Definition for field TYPE in Register ICPID2 */
#define ICPID2_TYPE_BIT_START                               0U
#define ICPID2_TYPE_BIT_END                                 7U

/* Definition for field NU in Register ICPID2 */
#define ICPID2_NU_BIT_START                                 8U
#define ICPID2_NU_BIT_END                                   31U

/* Definition for field RXDMAEN in Register ICDMAC */
#define ICDMAC_RXDMAEN_BIT_START                            0U
#define ICDMAC_RXDMAEN_BIT_END                              0U

/* Definition for field TXDMAEN in Register ICDMAC */
#define ICDMAC_TXDMAEN_BIT_START                            1U
#define ICDMAC_TXDMAEN_BIT_END                              1U

/* Definition for field NU in Register ICDMAC */
#define ICDMAC_NU_BIT_START                                 2U
#define ICDMAC_NU_BIT_END                                   31U

/* Definition for field PFUNC0 in Register ICPFUNC */
#define ICPFUNC_PFUNC0_BIT_START                            0U
#define ICPFUNC_PFUNC0_BIT_END                              0U

/* Definition for field NU in Register ICPFUNC */
#define ICPFUNC_NU_BIT_START                                1U
#define ICPFUNC_NU_BIT_END                                  31U

/* Definition for field PDIR0 in Register ICPDIR */
#define ICPDIR_PDIR0_BIT_START                              0U
#define ICPDIR_PDIR0_BIT_END                                0U

/* Definition for field PDIR1 in Register ICPDIR */
#define ICPDIR_PDIR1_BIT_START                              1U
#define ICPDIR_PDIR1_BIT_END                                1U

/* Definition for field NU in Register ICPDIR */
#define ICPDIR_NU_BIT_START                                 2U
#define ICPDIR_NU_BIT_END                                   31U

/* Definition for field PDIN0 in Register ICPDIN */
#define ICPDIN_PDIN0_BIT_START                              0U
#define ICPDIN_PDIN0_BIT_END                                0U

/* Definition for field PDIN1 in Register ICPDIN */
#define ICPDIN_PDIN1_BIT_START                              1U
#define ICPDIN_PDIN1_BIT_END                                1U

/* Definition for field NU in Register ICPDIN */
#define ICPDIN_NU_BIT_START                                 2U
#define ICPDIN_NU_BIT_END                                   31U

/* Definition for field PDOUT0 in Register ICPDOUT */
#define ICPDOUT_PDOUT0_BIT_START                            0U
#define ICPDOUT_PDOUT0_BIT_END                              0U

/* Definition for field PDOUT1 in Register ICPDOUT */
#define ICPDOUT_PDOUT1_BIT_START                            1U
#define ICPDOUT_PDOUT1_BIT_END                              1U

/* Definition for field NU in Register ICPDOUT */
#define ICPDOUT_NU_BIT_START                                2U
#define ICPDOUT_NU_BIT_END                                  31U

/* Definition for field PDSET0 in Register ICPDSET */
#define ICPDSET_PDSET0_BIT_START                            0U
#define ICPDSET_PDSET0_BIT_END                              0U

/* Definition for field PDSET1 in Register ICPDSET */
#define ICPDSET_PDSET1_BIT_START                            1U
#define ICPDSET_PDSET1_BIT_END                              1U

/* Definition for field NU in Register ICPDSET */
#define ICPDSET_NU_BIT_START                                2U
#define ICPDSET_NU_BIT_END                                  31U

/* Definition for field PDCLR0 in Register ICPDCLR */
#define ICPDCLR_PDCLR0_BIT_START                            0U
#define ICPDCLR_PDCLR0_BIT_END                              0U

/* Definition for field PDCLR1 in Register ICPDCLR */
#define ICPDCLR_PDCLR1_BIT_START                            1U
#define ICPDCLR_PDCLR1_BIT_END                              1U

/* Definition for field NU in Register ICPDCLR */
#define ICPDCLR_NU_BIT_START                                2U
#define ICPDCLR_NU_BIT_END                                  31U

/* Definition for field PDRV0 in Register ICPDRV */
#define ICPDRV_PDRV0_BIT_START                              0U
#define ICPDRV_PDRV0_BIT_END                                0U

/* Definition for field PDRV1 in Register ICPDRV */
#define ICPDRV_PDRV1_BIT_START                              1U
#define ICPDRV_PDRV1_BIT_END                                1U

/* Definition for field NU in Register ICPDRV */
#define ICPDRV_NU_BIT_START                                 2U
#define ICPDRV_NU_BIT_END                                   31U



/**
 * @struct I2CRegs_t
 * @brief
 *   Module MSS_I2C Register Definition
 * @details
 *   This structure is used to access the MSS_I2C module registers.
 */
/**
 * @typedef I2CRegs
 * @brief
 *   Module MSS_I2C Register Frame type Definition
 * @details
 *   This type is used to access the MSS_I2C module registers.
 */
typedef volatile struct I2CRegs_t
{
    uint32_t    ICOAR                              ;        /* Offset = 0x000 */
    uint32_t    ICIMR                              ;        /* Offset = 0x004 */
    uint32_t    ICSTR                              ;        /* Offset = 0x008 */
    uint32_t    ICCLKL                             ;        /* Offset = 0x00C */
    uint32_t    ICCLKH                             ;        /* Offset = 0x010 */
    uint32_t    ICCNT                              ;        /* Offset = 0x014 */
    uint32_t    ICDRR                              ;        /* Offset = 0x018 */
    uint32_t    ICSAR                              ;        /* Offset = 0x01C */
    uint32_t    ICDXR                              ;        /* Offset = 0x020 */
    uint32_t    ICMDR                              ;        /* Offset = 0x024 */
    uint32_t    ICIVR                              ;        /* Offset = 0x028 */
    uint32_t    ICEMDR                             ;        /* Offset = 0x02C */
    uint32_t    ICPSC                              ;        /* Offset = 0x030 */
    uint32_t    ICPID1                             ;        /* Offset = 0x034 */
    uint32_t    ICPID2                             ;        /* Offset = 0x038 */
    uint32_t    ICDMAC                             ;        /* Offset = 0x03C */
    uint32_t    I2C_RESERVED1                      ;        /* Offset = 0x040 */
    uint32_t    I2C_RESERVED2                      ;        /* Offset = 0x044 */
    uint32_t    ICPFUNC                            ;        /* Offset = 0x048 */
    uint32_t    ICPDIR                             ;        /* Offset = 0x04C */
    uint32_t    ICPDIN                             ;        /* Offset = 0x050 */
    uint32_t    ICPDOUT                            ;        /* Offset = 0x054 */
    uint32_t    ICPDSET                            ;        /* Offset = 0x058 */
    uint32_t    ICPDCLR                            ;        /* Offset = 0x05C */
    uint32_t    ICPDRV                             ;        /* Offset = 0x060 */
} I2CRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_I2C_H */
/* END OF REG_I2C_H */

