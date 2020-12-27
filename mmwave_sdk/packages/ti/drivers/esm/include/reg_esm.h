/**
 *  @file    reg_esm.h
 *
 *  @brief
 *    This file gives register definitions of MSS_ESM module.
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

#ifndef REG_ESM_H
#define REG_ESM_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field IEPSET in Register ESMIEPSR1 */
#define ESMIEPSR1_IEPSET_BIT_START                          0U
#define ESMIEPSR1_IEPSET_BIT_END                            31U

/* Definition for field IEPCLR in Register ESMIEPCR1 */
#define ESMIEPCR1_IEPCLR_BIT_START                          0U
#define ESMIEPCR1_IEPCLR_BIT_END                            31U

/* Definition for field INTENSET in Register ESMIESR1 */
#define ESMIESR1_INTENSET_BIT_START                         0U
#define ESMIESR1_INTENSET_BIT_END                           31U

/* Definition for field INTENCLR in Register ESMIECR1 */
#define ESMIECR1_INTENCLR_BIT_START                         0U
#define ESMIECR1_INTENCLR_BIT_END                           31U

/* Definition for field INTLVLSET in Register ESMILSR1 */
#define ESMILSR1_INTLVLSET_BIT_START                        0U
#define ESMILSR1_INTLVLSET_BIT_END                          31U

/* Definition for field INTLVLCLR in Register ESMILCR1 */
#define ESMILCR1_INTLVLCLR_BIT_START                        0U
#define ESMILCR1_INTLVLCLR_BIT_END                          31U

/* Definition for field ESF in Register ESMSR1 */
#define ESMSR1_ESF_BIT_START                                0U
#define ESMSR1_ESF_BIT_END                                  31U

/* Definition for field ESF in Register ESMSR2 */
#define ESMSR2_ESF_BIT_START                                0U
#define ESMSR2_ESF_BIT_END                                  31U

/* Definition for field ESF in Register ESMSR3 */
#define ESMSR3_ESF_BIT_START                                0U
#define ESMSR3_ESF_BIT_END                                  31U

/* Definition for field EPSF in Register ESMEPSR */
#define ESMEPSR_EPSF_BIT_START                              0U
#define ESMEPSR_EPSF_BIT_END                                0U

/* Definition for field INTOFFH in Register ESMIOFFHR */
#define ESMIOFFHR_INTOFFH_BIT_START                         0U
#define ESMIOFFHR_INTOFFH_BIT_END                           6U

/* Definition for field INTOFFL in Register ESMIOFFLR */
#define ESMIOFFLR_INTOFFL_BIT_START                         0U
#define ESMIOFFLR_INTOFFL_BIT_END                           6U

/* Definition for field LTCP in Register ESMLTCR */
#define ESMLTCR_LTCP_BIT_START                              0U
#define ESMLTCR_LTCP_BIT_END                                15U

/* Definition for field LTCP in Register ESMLTCPR */
#define ESMLTCPR_LTCP_BIT_START                             0U
#define ESMLTCPR_LTCP_BIT_END                               15U

/* Definition for field EKEY in Register ESMEKR */
#define ESMEKR_EKEY_BIT_START                               0U
#define ESMEKR_EKEY_BIT_END                                 3U

/* Definition for field ESF in Register ESMSSR2 */
#define ESMSSR2_ESF_BIT_START                               0U
#define ESMSSR2_ESF_BIT_END                                 31U

/* Definition for field IEPSET in Register ESMIEPSR4 */
#define ESMIEPSR4_IEPSET_BIT_START                          0U
#define ESMIEPSR4_IEPSET_BIT_END                            31U

/* Definition for field IEPCLR in Register ESMIEPCR4 */
#define ESMIEPCR4_IEPCLR_BIT_START                          0U
#define ESMIEPCR4_IEPCLR_BIT_END                            31U

/* Definition for field INTENSET in Register ESMIESR4 */
#define ESMIESR4_INTENSET_BIT_START                         0U
#define ESMIESR4_INTENSET_BIT_END                           31U

/* Definition for field INTENCLR in Register ESMIECR4 */
#define ESMIECR4_INTENCLR_BIT_START                         0U
#define ESMIECR4_INTENCLR_BIT_END                           31U

/* Definition for field INTLVLSET in Register ESMILSR4 */
#define ESMILSR4_INTLVLSET_BIT_START                        0U
#define ESMILSR4_INTLVLSET_BIT_END                          31U

/* Definition for field INTLVLCLR in Register ESMILCR4 */
#define ESMILCR4_INTLVLCLR_BIT_START                        0U
#define ESMILCR4_INTLVLCLR_BIT_END                          31U

/* Definition for field ESF in Register ESMSR4 */
#define ESMSR4_ESF_BIT_START                                0U
#define ESMSR4_ESF_BIT_END                                  31U

/* Definition for field IEPSET in Register ESMIEPSR7 */
#define ESMIEPSR7_IEPSET_BIT_START                          0U
#define ESMIEPSR7_IEPSET_BIT_END                            31U

/* Definition for field IEPCLR in Register ESMIEPCR7 */
#define ESMIEPCR7_IEPCLR_BIT_START                          0U
#define ESMIEPCR7_IEPCLR_BIT_END                            31U

/* Definition for field INTENSET in Register ESMIESR7 */
#define ESMIESR7_INTENSET_BIT_START                         0U
#define ESMIESR7_INTENSET_BIT_END                           31U

/* Definition for field INTENCLR in Register ESMIECR7 */
#define ESMIECR7_INTENCLR_BIT_START                         0U
#define ESMIECR7_INTENCLR_BIT_END                           31U

/* Definition for field INTLVLSET in Register ESMILSR7 */
#define ESMILSR7_INTLVLSET_BIT_START                        0U
#define ESMILSR7_INTLVLSET_BIT_END                          31U

/* Definition for field INTLVLCLR in Register ESMILCR7 */
#define ESMILCR7_INTLVLCLR_BIT_START                        0U
#define ESMILCR7_INTLVLCLR_BIT_END                          31U

/* Definition for field ESF in Register ESMSR7 */
#define ESMSR7_ESF_BIT_START                                0U
#define ESMSR7_ESF_BIT_END                                  31U



/**
 * @struct ESMRegs_t
 * @brief
 *   Module MSS_ESM Register Definition
 * @details
 *   This structure is used to access the MSS_ESM module registers.
 */
/**
 * @typedef ESMRegs
 * @brief
 *   Module MSS_ESM Register Frame type Definition
 * @details
 *   This type is used to access the MSS_ESM module registers.
 */
typedef volatile struct ESMRegs_t
{
    uint32_t    ESMIEPSR1                          ;        /* Offset = 0x000 */
    uint32_t    ESMIEPCR1                          ;        /* Offset = 0x004 */
    uint32_t    ESMIESR1                           ;        /* Offset = 0x008 */
    uint32_t    ESMIECR1                           ;        /* Offset = 0x00C */
    uint32_t    ESMILSR1                           ;        /* Offset = 0x010 */
    uint32_t    ESMILCR1                           ;        /* Offset = 0x014 */
    uint32_t    ESMSR1                             ;        /* Offset = 0x018 */
    uint32_t    ESMSR2                             ;        /* Offset = 0x01C */
    uint32_t    ESMSR3                             ;        /* Offset = 0x020 */
    uint32_t    ESMEPSR                            ;        /* Offset = 0x024 */
    uint32_t    ESMIOFFHR                          ;        /* Offset = 0x028 */
    uint32_t    ESMIOFFLR                          ;        /* Offset = 0x02C */
    uint32_t    ESMLTCR                            ;        /* Offset = 0x030 */
    uint32_t    ESMLTCPR                           ;        /* Offset = 0x034 */
    uint32_t    ESMEKR                             ;        /* Offset = 0x038 */
    uint32_t    ESMSSR2                            ;        /* Offset = 0x03C */
    uint32_t    ESMIEPSR4                          ;        /* Offset = 0x040 */
    uint32_t    ESMIEPCR4                          ;        /* Offset = 0x044 */
    uint32_t    ESMIESR4                           ;        /* Offset = 0x048 */
    uint32_t    ESMIECR4                           ;        /* Offset = 0x04C */
    uint32_t    ESMILSR4                           ;        /* Offset = 0x050 */
    uint32_t    ESMILCR4                           ;        /* Offset = 0x054 */
    uint32_t    ESMSR4                             ;        /* Offset = 0x058 */
    uint32_t    RESTRICTED1[9]                     ;        /* Offset = 0x05C */
    uint32_t    ESMIEPSR7                          ;        /* Offset = 0x080 */
    uint32_t    ESMIEPCR7                          ;        /* Offset = 0x084 */
    uint32_t    ESMIESR7                           ;        /* Offset = 0x088 */
    uint32_t    ESMIECR7                           ;        /* Offset = 0x08C */
    uint32_t    ESMILSR7                           ;        /* Offset = 0x090 */
    uint32_t    ESMILCR7                           ;        /* Offset = 0x094 */
    uint32_t    ESMSR7                             ;        /* Offset = 0x098 */
} ESMRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_ESM_H */
/* END OF REG_ESM_H */

