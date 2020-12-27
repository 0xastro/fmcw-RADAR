/**
 *  @file    reg_mailbox.h
 *
 *  @brief
 *    This file gives register definitions of MAILBOX module.
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

#ifndef REG_MAILBOX_H
#define REG_MAILBOX_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field MAILBOX_INT_MASK in Register INT_MASK */
#define INT_MASK_MAILBOX_INT_MASK_BIT_START                 0U
#define INT_MASK_MAILBOX_INT_MASK_BIT_END                   0U

/* Definition for field MAILBOX_ACK_INT_MASK in Register INT_MASK */
#define INT_MASK_MAILBOX_ACK_INT_MASK_BIT_START             1U
#define INT_MASK_MAILBOX_ACK_INT_MASK_BIT_END               1U

/* Definition for field MAILBOX_INT_MASK_SET in Register INT_MASK_SET */
#define INT_MASK_SET_MAILBOX_INT_MASK_SET_BIT_START         0U
#define INT_MASK_SET_MAILBOX_INT_MASK_SET_BIT_END           0U

/* Definition for field MAILBOX_ACK_INT_MASK_SET in Register INT_MASK_SET */
#define INT_MASK_SET_MAILBOX_ACK_INT_MASK_SET_BIT_START     1U
#define INT_MASK_SET_MAILBOX_ACK_INT_MASK_SET_BIT_END       1U

/* Definition for field MAILBOX_INT_MASK_CLR in Register INT_MASK_CLR */
#define INT_MASK_CLR_MAILBOX_INT_MASK_CLR_BIT_START         0U
#define INT_MASK_CLR_MAILBOX_INT_MASK_CLR_BIT_END           0U

/* Definition for field MAILBOX_ACK_INT_MASK_CLR in Register INT_MASK_CLR */
#define INT_MASK_CLR_MAILBOX_ACK_INT_MASK_CLR_BIT_START     1U
#define INT_MASK_CLR_MAILBOX_ACK_INT_MASK_CLR_BIT_END       1U

/* Definition for field MAILBOX_INT_STS_CLR in Register INT_STS_CLR */
#define INT_STS_CLR_MAILBOX_INT_STS_CLR_BIT_START           0U
#define INT_STS_CLR_MAILBOX_INT_STS_CLR_BIT_END             0U

/* Definition for field MAILBOX_ACK_INT_STS_CLR in Register INT_STS_CLR */
#define INT_STS_CLR_MAILBOX_ACK_INT_STS_CLR_BIT_START       1U
#define INT_STS_CLR_MAILBOX_ACK_INT_STS_CLR_BIT_END         1U

/* Definition for field MAILBOX_INT_ACK in Register INT_ACK */
#define INT_ACK_MAILBOX_INT_ACK_BIT_START                   0U
#define INT_ACK_MAILBOX_INT_ACK_BIT_END                     0U

/* Definition for field MAILBOX_ACK_INT_ACK in Register INT_ACK */
#define INT_ACK_MAILBOX_ACK_INT_ACK_BIT_START               1U
#define INT_ACK_MAILBOX_ACK_INT_ACK_BIT_END                 1U

/* Definition for field MAILBOX_INT_TRIG in Register INT_TRIG */
#define INT_TRIG_MAILBOX_INT_TRIG_BIT_START                 0U
#define INT_TRIG_MAILBOX_INT_TRIG_BIT_END                   0U

/* Definition for field MAILBOX_ACK_TRIG in Register INT_TRIG */
#define INT_TRIG_MAILBOX_ACK_TRIG_BIT_START                 1U
#define INT_TRIG_MAILBOX_ACK_TRIG_BIT_END                   1U

/* Definition for field MAILBOX_INT_STS_MASKED in Register INT_STS_MASKED */
#define INT_STS_MASKED_MAILBOX_INT_STS_MASKED_BIT_START     0U
#define INT_STS_MASKED_MAILBOX_INT_STS_MASKED_BIT_END       0U

/* Definition for field MAILBOX_ACK_STS_MASKED in Register INT_STS_MASKED */
#define INT_STS_MASKED_MAILBOX_ACK_STS_MASKED_BIT_START     1U
#define INT_STS_MASKED_MAILBOX_ACK_STS_MASKED_BIT_END       1U

/* Definition for field MAILBOX_INT_STS_RAW in Register INT_STS_RAW */
#define INT_STS_RAW_MAILBOX_INT_STS_RAW_BIT_START           0U
#define INT_STS_RAW_MAILBOX_INT_STS_RAW_BIT_END             0U

/* Definition for field MAILBOX_ACK_STS_RAW in Register INT_STS_RAW */
#define INT_STS_RAW_MAILBOX_ACK_STS_RAW_BIT_START           1U
#define INT_STS_RAW_MAILBOX_ACK_STS_RAW_BIT_END             1U



/**
 * @struct MAILBOXRegs_t
 * @brief
 *   Module MAILBOX Register Definition
 * @details
 *   This structure is used to access the MAILBOX module registers.
 */
/**
 * @typedef MAILBOXRegs
 * @brief
 *   Module MAILBOX Register Frame type Definition
 * @details
 *   This type is used to access the MAILBOX module registers.
 */
typedef volatile struct MAILBOXRegs_t
{
    uint32_t    INT_MASK                           ;        /* Offset = 0x000 */
    uint32_t    RESTRICTED1                        ;        /* Offset = 0x004 */
    uint32_t    INT_MASK_SET                       ;        /* Offset = 0x008 */
    uint32_t    RESTRICTED2                        ;        /* Offset = 0x00C */
    uint32_t    INT_MASK_CLR                       ;        /* Offset = 0x010 */
    uint32_t    RESTRICTED3                        ;        /* Offset = 0x014 */
    uint32_t    INT_STS_CLR                        ;        /* Offset = 0x018 */
    uint32_t    RESTRICTED4                        ;        /* Offset = 0x01C */
    uint32_t    INT_ACK                            ;        /* Offset = 0x020 */
    uint32_t    RESTRICTED5                        ;        /* Offset = 0x024 */
    uint32_t    INT_TRIG                           ;        /* Offset = 0x028 */
    uint32_t    RESTRICTED6                        ;        /* Offset = 0x02C */
    uint32_t    INT_STS_MASKED                     ;        /* Offset = 0x030 */
    uint32_t    RESTRICTED7                        ;        /* Offset = 0x034 */
    uint32_t    INT_STS_RAW                        ;        /* Offset = 0x038 */
} MAILBOXRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_MAILBOX_H */
/* END OF REG_MAILBOX_H */

