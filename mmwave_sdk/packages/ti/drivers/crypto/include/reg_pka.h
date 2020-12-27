/**
 *  @file    reg_pka.h
 *
 *  @brief
 *    This file gives register definitions of MSS_PKA module.
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

#ifndef REG_PKA_H
#define REG_PKA_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field OVERFLOW in Register PKA_STATUS */
#define PKA_STATUS_OVERFLOW_BIT_START                       0U
#define PKA_STATUS_OVERFLOW_BIT_END                         0U

/* Definition for field MMMBUSY in Register PKA_STATUS */
#define PKA_STATUS_MMMBUSY_BIT_START                        1U
#define PKA_STATUS_MMMBUSY_BIT_END                          1U

/* Definition for field CMDERROR in Register PKA_STATUS */
#define PKA_STATUS_CMDERROR_BIT_START                       2U
#define PKA_STATUS_CMDERROR_BIT_END                         2U

/* Definition for field MMMOP in Register PKA_CONTROL */
#define PKA_CONTROL_MMMOP_BIT_START                         0U
#define PKA_CONTROL_MMMOP_BIT_END                           1U

/* Definition for field MMEXP in Register PKA_CONTROL */
#define PKA_CONTROL_MMEXP_BIT_START                         2U
#define PKA_CONTROL_MMEXP_BIT_END                           2U

/* Definition for field ABORT in Register PKA_CONTROL */
#define PKA_CONTROL_ABORT_BIT_START                         5U
#define PKA_CONTROL_ABORT_BIT_END                           5U

/* Definition for field NBASE in Register PKA_NBASE */
#define PKA_NBASE_NBASE_BIT_START                           0U
#define PKA_NBASE_NBASE_BIT_END                             9U

/* Definition for field NYDIGITS in Register PKA_NBASE */
#define PKA_NBASE_NYDIGITS_BIT_START                        16U
#define PKA_NBASE_NYDIGITS_BIT_END                          23U

/* Definition for field XBASE in Register PKA_XBASE */
#define PKA_XBASE_XBASE_BIT_START                           0U
#define PKA_XBASE_XBASE_BIT_END                             9U

/* Definition for field XDIGITS in Register PKA_XBASE */
#define PKA_XBASE_XDIGITS_BIT_START                         16U
#define PKA_XBASE_XDIGITS_BIT_END                           23U

/* Definition for field YBASE in Register PKA_YBASE */
#define PKA_YBASE_YBASE_BIT_START                           0U
#define PKA_YBASE_YBASE_BIT_END                             9U

/* Definition for field NPASSES in Register PKA_YBASE */
#define PKA_YBASE_NPASSES_BIT_START                         16U
#define PKA_YBASE_NPASSES_BIT_END                           23U

/* Definition for field BBASE in Register PKA_BBASE */
#define PKA_BBASE_BBASE_BIT_START                           0U
#define PKA_BBASE_BBASE_BIT_END                             10U

/* Definition for field BCNTR in Register PKA_BBASE */
#define PKA_BBASE_BCNTR_BIT_START                           16U
#define PKA_BBASE_BCNTR_BIT_END                             28U

/* Definition for field NACC in Register PKA_NACC */
#define PKA_NACC_NACC_BIT_START                             0U
#define PKA_NACC_NACC_BIT_END                               7U

/* Definition for field NACCBUSY in Register PKA_NACC */
#define PKA_NACC_NACCBUSY_BIT_START                         8U
#define PKA_NACC_NACCBUSY_BIT_END                           8U

/* Definition for field EXPARRAY in Register PKA_NACC */
#define PKA_NACC_EXPARRAY_BIT_START                         16U
#define PKA_NACC_EXPARRAY_BIT_END                           20U

/* Definition for field NZERO in Register PKA_NZERO */
#define PKA_NZERO_NZERO_BIT_START                           0U
#define PKA_NZERO_NZERO_BIT_END                             7U

/* Definition for field CONF in Register PKA_OPTIONS */
#define PKA_OPTIONS_CONF_BIT_START                          2U
#define PKA_OPTIONS_CONF_BIT_END                            4U

/* Definition for field MMM3A in Register PKA_OPTIONS */
#define PKA_OPTIONS_MMM3A_BIT_START                         12U
#define PKA_OPTIONS_MMM3A_BIT_END                           12U

/* Definition for field PENUM in Register PKA_OPTIONS */
#define PKA_OPTIONS_PENUM_BIT_START                         16U
#define PKA_OPTIONS_PENUM_BIT_END                           21U

/* Definition for field FIFO in Register PKA_OPTIONS */
#define PKA_OPTIONS_FIFO_BIT_START                          24U
#define PKA_OPTIONS_FIFO_BIT_END                            31U

/* Definition for field EIPNUM in Register PKA_REVSFT */
#define PKA_REVSFT_EIPNUM_BIT_START                         0U
#define PKA_REVSFT_EIPNUM_BIT_END                           7U

/* Definition for field EIPNUM1 in Register PKA_REVSFT */
#define PKA_REVSFT_EIPNUM1_BIT_START                        8U
#define PKA_REVSFT_EIPNUM1_BIT_END                          15U

/* Definition for field PATCH in Register PKA_REVSFT */
#define PKA_REVSFT_PATCH_BIT_START                          16U
#define PKA_REVSFT_PATCH_BIT_END                            19U

/* Definition for field REVMIN in Register PKA_REVSFT */
#define PKA_REVSFT_REVMIN_BIT_START                         20U
#define PKA_REVSFT_REVMIN_BIT_END                           23U

/* Definition for field REVMAX in Register PKA_REVSFT */
#define PKA_REVSFT_REVMAX_BIT_START                         24U
#define PKA_REVSFT_REVMAX_BIT_END                           27U

/* Definition for field Y_MINOR in Register PKA_REVISION */
#define PKA_REVISION_Y_MINOR_BIT_START                      0U
#define PKA_REVISION_Y_MINOR_BIT_END                        5U

/* Definition for field CUSTOM in Register PKA_REVISION */
#define PKA_REVISION_CUSTOM_BIT_START                       6U
#define PKA_REVISION_CUSTOM_BIT_END                         7U

/* Definition for field X_MAJOR in Register PKA_REVISION */
#define PKA_REVISION_X_MAJOR_BIT_START                      8U
#define PKA_REVISION_X_MAJOR_BIT_END                        10U

/* Definition for field R_RTL in Register PKA_REVISION */
#define PKA_REVISION_R_RTL_BIT_START                        11U
#define PKA_REVISION_R_RTL_BIT_END                          14U

/* Definition for field FUNC in Register PKA_REVISION */
#define PKA_REVISION_FUNC_BIT_START                         16U
#define PKA_REVISION_FUNC_BIT_END                           27U

/* Definition for field SCHEME in Register PKA_REVISION */
#define PKA_REVISION_SCHEME_BIT_START                       30U
#define PKA_REVISION_SCHEME_BIT_END                         31U

/* Definition for field AUTOIDLE in Register PKA_SYSCONFIG */
#define PKA_SYSCONFIG_AUTOIDLE_BIT_START                    0U
#define PKA_SYSCONFIG_AUTOIDLE_BIT_END                      0U

/* Definition for field SOFTRESET in Register PKA_SYSCONFIG */
#define PKA_SYSCONFIG_SOFTRESET_BIT_START                   1U
#define PKA_SYSCONFIG_SOFTRESET_BIT_END                     1U

/* Definition for field SIDLE in Register PKA_SYSCONFIG */
#define PKA_SYSCONFIG_SIDLE_BIT_START                       4U
#define PKA_SYSCONFIG_SIDLE_BIT_END                         5U

/* Definition for field RESETDONE in Register PKA_SYSSTATUS */
#define PKA_SYSSTATUS_RESETDONE_BIT_START                   0U
#define PKA_SYSSTATUS_RESETDONE_BIT_END                     0U

/* Definition for field PKAIRQSTAT in Register PKA_IRQSTATUS */
#define PKA_IRQSTATUS_PKAIRQSTAT_BIT_START                  0U
#define PKA_IRQSTATUS_PKAIRQSTAT_BIT_END                    0U

/* Definition for field PKAIRQEN in Register PKA_IRQENABLE */
#define PKA_IRQENABLE_PKAIRQEN_BIT_START                    0U
#define PKA_IRQENABLE_PKAIRQEN_BIT_END                      0U

/* Definition for field DATARAM in Register PKA_RAM_START */
#define PKA_RAM_START_DATARAM_BIT_START                     0U
#define PKA_RAM_START_DATARAM_BIT_END                       31U

/* Definition for field DATARAM in Register PKA_RAM_END */
#define PKA_RAM_END_DATARAM_BIT_START                       0U
#define PKA_RAM_END_DATARAM_BIT_END                         31U



/**
 * @struct PKARegs_t
 * @brief
 *   Module MSS_PKA Register Definition
 * @details
 *   This structure is used to access the MSS_PKA module registers.
 */
/**
 * @typedef PKARegs
 * @brief
 *   Module MSS_PKA Register Frame type Definition
 * @details
 *   This type is used to access the MSS_PKA module registers.
 */
typedef volatile struct PKARegs_t
{
    uint32_t    RESTRICTED0[32]                    ;        /* Offset = 0x000 */
    uint32_t    PKA_STATUS                         ;        /* Offset = 0x080 */
    uint32_t    PKA_CONTROL                        ;        /* Offset = 0x084 */
    uint32_t    RESTRICTED1[6]                     ;        /* Offset = 0x088 */
    uint32_t    PKA_NBASE                          ;        /* Offset = 0x0A0 */
    uint32_t    PKA_XBASE                          ;        /* Offset = 0x0A4 */
    uint32_t    PKA_YBASE                          ;        /* Offset = 0x0A8 */
    uint32_t    PKA_BBASE                          ;        /* Offset = 0x0AC */
    uint32_t    PKA_NACC                           ;        /* Offset = 0x0B0 */
    uint32_t    PKA_NZERO                          ;        /* Offset = 0x0B4 */
    uint32_t    RESTRICTED2[16]                    ;        /* Offset = 0x0B8 */
    uint32_t    PKA_OPTIONS                        ;        /* Offset = 0x0F8 */
    uint32_t    PKA_REVSFT                         ;        /* Offset = 0x0FC */
    uint32_t    RESTRICTED3[1976]                  ;        /* Offset = 0x100 */
    uint32_t    PKA_REVISION                       ;        /* Offset = 0x1FE0 */
    uint32_t    RESTRICTED4[3]                     ;        /* Offset = 0x1FE4 */
    uint32_t    PKA_SYSCONFIG                      ;        /* Offset = 0x1FF0 */
    uint32_t    PKA_SYSSTATUS                      ;        /* Offset = 0x1FF4 */
    uint32_t    PKA_IRQSTATUS                      ;        /* Offset = 0x1FF8 */
    uint32_t    PKA_IRQENABLE                      ;        /* Offset = 0x1FFC */
    uint32_t    PKA_RAM_START                      ;        /* Offset = 0x2000 */
    uint32_t    RESTRICTED5[1022]                  ;        /* Offset = 0x2004 */
    uint32_t    PKA_RAM_END                        ;        /* Offset = 0x2FFC */
} PKARegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_PKA_H */
/* END OF REG_PKA_H */

