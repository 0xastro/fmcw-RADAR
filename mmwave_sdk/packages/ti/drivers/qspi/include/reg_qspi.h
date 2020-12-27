/**
 *  @file    reg_qspi.h
 *
 *  @brief
 *    This file gives register definitions of MSS_QSPI module.
 *
 *  This file is auto-generated on 3/17/2017.
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

#ifndef REG_QSPI_H
#define REG_QSPI_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field MINOR in Register PID */
#define PID_MINOR_BIT_START                                 0U
#define PID_MINOR_BIT_END                                   5U

/* Definition for field CUSTOM in Register PID */
#define PID_CUSTOM_BIT_START                                6U
#define PID_CUSTOM_BIT_END                                  7U

/* Definition for field MAJOR in Register PID */
#define PID_MAJOR_BIT_START                                 8U
#define PID_MAJOR_BIT_END                                   10U

/* Definition for field RTL in Register PID */
#define PID_RTL_BIT_START                                   11U
#define PID_RTL_BIT_END                                     15U

/* Definition for field FUNC in Register PID */
#define PID_FUNC_BIT_START                                  16U
#define PID_FUNC_BIT_END                                    27U

/* Definition for field SCHEME in Register PID */
#define PID_SCHEME_BIT_START                                30U
#define PID_SCHEME_BIT_END                                  31U

/* Definition for field IDLEMODE in Register SYSCONFIG */
#define SYSCONFIG_IDLEMODE_BIT_START                        2U
#define SYSCONFIG_IDLEMODE_BIT_END                          3U

/* Definition for field FIRQ_RAW in Register INTR_STATUS_RAW_SET */
#define INTR_STATUS_RAW_SET_FIRQ_RAW_BIT_START              0U
#define INTR_STATUS_RAW_SET_FIRQ_RAW_BIT_END                0U

/* Definition for field WIRQ_RAW in Register INTR_STATUS_RAW_SET */
#define INTR_STATUS_RAW_SET_WIRQ_RAW_BIT_START              1U
#define INTR_STATUS_RAW_SET_WIRQ_RAW_BIT_END                1U

/* Definition for field FIRQ_ENA in Register INTR_STATUS_ENABLED_CLEAR */
#define INTR_STATUS_ENABLED_CLEAR_FIRQ_ENA_BIT_START        0U
#define INTR_STATUS_ENABLED_CLEAR_FIRQ_ENA_BIT_END          0U

/* Definition for field WIRQ_ENA in Register INTR_STATUS_ENABLED_CLEAR */
#define INTR_STATUS_ENABLED_CLEAR_WIRQ_ENA_BIT_START        1U
#define INTR_STATUS_ENABLED_CLEAR_WIRQ_ENA_BIT_END          1U

/* Definition for field FIRQ_ENA_SET in Register INTR_ENABLE_SET */
#define INTR_ENABLE_SET_FIRQ_ENA_SET_BIT_START              0U
#define INTR_ENABLE_SET_FIRQ_ENA_SET_BIT_END                0U

/* Definition for field WIRQ_ENA_SET in Register INTR_ENABLE_SET */
#define INTR_ENABLE_SET_WIRQ_ENA_SET_BIT_START              1U
#define INTR_ENABLE_SET_WIRQ_ENA_SET_BIT_END                1U

/* Definition for field FIRQ_ENA_CLR in Register INTR_ENABLE_CLEAR */
#define INTR_ENABLE_CLEAR_FIRQ_ENA_CLR_BIT_START            0U
#define INTR_ENABLE_CLEAR_FIRQ_ENA_CLR_BIT_END              0U

/* Definition for field WIRQ_ENA_CLR in Register INTR_ENABLE_CLEAR */
#define INTR_ENABLE_CLEAR_WIRQ_ENA_CLR_BIT_START            1U
#define INTR_ENABLE_CLEAR_WIRQ_ENA_CLR_BIT_END              1U

/* Definition for field EOI_VECTOR in Register INTC_EOI */
#define INTC_EOI_EOI_VECTOR_BIT_START                       0U
#define INTC_EOI_EOI_VECTOR_BIT_END                         31U

/* Definition for field DCLK_DIV in Register SPI_CLOCK_CNTRL */
#define SPI_CLOCK_CNTRL_DCLK_DIV_BIT_START                  0U
#define SPI_CLOCK_CNTRL_DCLK_DIV_BIT_END                    15U

/* Definition for field CLKEN in Register SPI_CLOCK_CNTRL */
#define SPI_CLOCK_CNTRL_CLKEN_BIT_START                     31U
#define SPI_CLOCK_CNTRL_CLKEN_BIT_END                       31U

/* Definition for field CKP0 in Register SPI_DC */
#define SPI_DC_CKP0_BIT_START                               0U
#define SPI_DC_CKP0_BIT_END                                 0U

/* Definition for field CSP0 in Register SPI_DC */
#define SPI_DC_CSP0_BIT_START                               1U
#define SPI_DC_CSP0_BIT_END                                 1U

/* Definition for field CKPH0 in Register SPI_DC */
#define SPI_DC_CKPH0_BIT_START                              2U
#define SPI_DC_CKPH0_BIT_END                                2U

/* Definition for field DD0 in Register SPI_DC */
#define SPI_DC_DD0_BIT_START                                3U
#define SPI_DC_DD0_BIT_END                                  4U

/* Definition for field CKP1 in Register SPI_DC */
#define SPI_DC_CKP1_BIT_START                               8U
#define SPI_DC_CKP1_BIT_END                                 8U

/* Definition for field CSP1 in Register SPI_DC */
#define SPI_DC_CSP1_BIT_START                               9U
#define SPI_DC_CSP1_BIT_END                                 9U

/* Definition for field CKPH1 in Register SPI_DC */
#define SPI_DC_CKPH1_BIT_START                              10U
#define SPI_DC_CKPH1_BIT_END                                10U

/* Definition for field DD1 in Register SPI_DC */
#define SPI_DC_DD1_BIT_START                                11U
#define SPI_DC_DD1_BIT_END                                  12U

/* Definition for field CKP2 in Register SPI_DC */
#define SPI_DC_CKP2_BIT_START                               16U
#define SPI_DC_CKP2_BIT_END                                 16U

/* Definition for field CSP2 in Register SPI_DC */
#define SPI_DC_CSP2_BIT_START                               17U
#define SPI_DC_CSP2_BIT_END                                 17U

/* Definition for field CKPH2 in Register SPI_DC */
#define SPI_DC_CKPH2_BIT_START                              18U
#define SPI_DC_CKPH2_BIT_END                                18U

/* Definition for field DD2 in Register SPI_DC */
#define SPI_DC_DD2_BIT_START                                19U
#define SPI_DC_DD2_BIT_END                                  20U

/* Definition for field CKP3 in Register SPI_DC */
#define SPI_DC_CKP3_BIT_START                               24U
#define SPI_DC_CKP3_BIT_END                                 24U

/* Definition for field CSP3 in Register SPI_DC */
#define SPI_DC_CSP3_BIT_START                               25U
#define SPI_DC_CSP3_BIT_END                                 25U

/* Definition for field CKPH3 in Register SPI_DC */
#define SPI_DC_CKPH3_BIT_START                              26U
#define SPI_DC_CKPH3_BIT_END                                26U

/* Definition for field DD3 in Register SPI_DC */
#define SPI_DC_DD3_BIT_START                                27U
#define SPI_DC_DD3_BIT_END                                  28U

/* Definition for field FLEN in Register SPI_CMD */
#define SPI_CMD_FLEN_BIT_START                              0U
#define SPI_CMD_FLEN_BIT_END                                11U

/* Definition for field WIRQ in Register SPI_CMD */
#define SPI_CMD_WIRQ_BIT_START                              14U
#define SPI_CMD_WIRQ_BIT_END                                14U

/* Definition for field FIRQ in Register SPI_CMD */
#define SPI_CMD_FIRQ_BIT_START                              15U
#define SPI_CMD_FIRQ_BIT_END                                15U

/* Definition for field CMD in Register SPI_CMD */
#define SPI_CMD_CMD_BIT_START                               16U
#define SPI_CMD_CMD_BIT_END                                 18U

/* Definition for field WLEN in Register SPI_CMD */
#define SPI_CMD_WLEN_BIT_START                              19U
#define SPI_CMD_WLEN_BIT_END                                25U

/* Definition for field CSNUM in Register SPI_CMD */
#define SPI_CMD_CSNUM_BIT_START                             28U
#define SPI_CMD_CSNUM_BIT_END                               29U

/* Definition for field BUSY in Register SPI_STATUS */
#define SPI_STATUS_BUSY_BIT_START                           0U
#define SPI_STATUS_BUSY_BIT_END                             0U

/* Definition for field WC in Register SPI_STATUS */
#define SPI_STATUS_WC_BIT_START                             1U
#define SPI_STATUS_WC_BIT_END                               1U

/* Definition for field FC in Register SPI_STATUS */
#define SPI_STATUS_FC_BIT_START                             2U
#define SPI_STATUS_FC_BIT_END                               2U

/* Definition for field WDCNT in Register SPI_STATUS */
#define SPI_STATUS_WDCNT_BIT_START                          16U
#define SPI_STATUS_WDCNT_BIT_END                            27U

/* Definition for field DATA in Register SPI_DATA */
#define SPI_DATA_DATA_BIT_START                             0U
#define SPI_DATA_DATA_BIT_END                               31U

/* Definition for field RCMD in Register SPI_SETUP0 */
#define SPI_SETUP0_RCMD_BIT_START                           0U
#define SPI_SETUP0_RCMD_BIT_END                             7U

/* Definition for field NUM_A_BYTES in Register SPI_SETUP0 */
#define SPI_SETUP0_NUM_A_BYTES_BIT_START                    8U
#define SPI_SETUP0_NUM_A_BYTES_BIT_END                      9U

/* Definition for field NUM_D_BYTES in Register SPI_SETUP0 */
#define SPI_SETUP0_NUM_D_BYTES_BIT_START                    10U
#define SPI_SETUP0_NUM_D_BYTES_BIT_END                      11U

/* Definition for field READ_TYPE in Register SPI_SETUP0 */
#define SPI_SETUP0_READ_TYPE_BIT_START                      12U
#define SPI_SETUP0_READ_TYPE_BIT_END                        13U

/* Definition for field WCMD in Register SPI_SETUP0 */
#define SPI_SETUP0_WCMD_BIT_START                           16U
#define SPI_SETUP0_WCMD_BIT_END                             23U

/* Definition for field NUM_D_BITS in Register SPI_SETUP0 */
#define SPI_SETUP0_NUM_D_BITS_BIT_START                     24U
#define SPI_SETUP0_NUM_D_BITS_BIT_END                       28U

/* Definition for field RCMD in Register SPI_SETUP1 */
#define SPI_SETUP1_RCMD_BIT_START                           0U
#define SPI_SETUP1_RCMD_BIT_END                             7U

/* Definition for field NUM_A_BYTES in Register SPI_SETUP1 */
#define SPI_SETUP1_NUM_A_BYTES_BIT_START                    8U
#define SPI_SETUP1_NUM_A_BYTES_BIT_END                      9U

/* Definition for field NUM_D_BYTES in Register SPI_SETUP1 */
#define SPI_SETUP1_NUM_D_BYTES_BIT_START                    10U
#define SPI_SETUP1_NUM_D_BYTES_BIT_END                      11U

/* Definition for field READ_TYPE in Register SPI_SETUP1 */
#define SPI_SETUP1_READ_TYPE_BIT_START                      12U
#define SPI_SETUP1_READ_TYPE_BIT_END                        13U

/* Definition for field WCMD in Register SPI_SETUP1 */
#define SPI_SETUP1_WCMD_BIT_START                           16U
#define SPI_SETUP1_WCMD_BIT_END                             23U

/* Definition for field NUM_D_BITS in Register SPI_SETUP1 */
#define SPI_SETUP1_NUM_D_BITS_BIT_START                     24U
#define SPI_SETUP1_NUM_D_BITS_BIT_END                       28U

/* Definition for field RCMD in Register SPI_SETUP2 */
#define SPI_SETUP2_RCMD_BIT_START                           0U
#define SPI_SETUP2_RCMD_BIT_END                             7U

/* Definition for field NUM_A_BYTES in Register SPI_SETUP2 */
#define SPI_SETUP2_NUM_A_BYTES_BIT_START                    8U
#define SPI_SETUP2_NUM_A_BYTES_BIT_END                      9U

/* Definition for field NUM_D_BYTES in Register SPI_SETUP2 */
#define SPI_SETUP2_NUM_D_BYTES_BIT_START                    10U
#define SPI_SETUP2_NUM_D_BYTES_BIT_END                      11U

/* Definition for field READ_TYPE in Register SPI_SETUP2 */
#define SPI_SETUP2_READ_TYPE_BIT_START                      12U
#define SPI_SETUP2_READ_TYPE_BIT_END                        13U

/* Definition for field WCMD in Register SPI_SETUP2 */
#define SPI_SETUP2_WCMD_BIT_START                           16U
#define SPI_SETUP2_WCMD_BIT_END                             23U

/* Definition for field NUM_D_BITS in Register SPI_SETUP2 */
#define SPI_SETUP2_NUM_D_BITS_BIT_START                     24U
#define SPI_SETUP2_NUM_D_BITS_BIT_END                       28U

/* Definition for field RCMD in Register SPI_SETUP3 */
#define SPI_SETUP3_RCMD_BIT_START                           0U
#define SPI_SETUP3_RCMD_BIT_END                             7U

/* Definition for field NUM_A_BYTES in Register SPI_SETUP3 */
#define SPI_SETUP3_NUM_A_BYTES_BIT_START                    8U
#define SPI_SETUP3_NUM_A_BYTES_BIT_END                      9U

/* Definition for field NUM_D_BYTES in Register SPI_SETUP3 */
#define SPI_SETUP3_NUM_D_BYTES_BIT_START                    10U
#define SPI_SETUP3_NUM_D_BYTES_BIT_END                      11U

/* Definition for field READ_TYPE in Register SPI_SETUP3 */
#define SPI_SETUP3_READ_TYPE_BIT_START                      12U
#define SPI_SETUP3_READ_TYPE_BIT_END                        13U

/* Definition for field WCMD in Register SPI_SETUP3 */
#define SPI_SETUP3_WCMD_BIT_START                           16U
#define SPI_SETUP3_WCMD_BIT_END                             23U

/* Definition for field NUM_D_BITS in Register SPI_SETUP3 */
#define SPI_SETUP3_NUM_D_BITS_BIT_START                     24U
#define SPI_SETUP3_NUM_D_BITS_BIT_END                       28U

/* Definition for field MMPT_S in Register SPI_SWITCH */
#define SPI_SWITCH_MMPT_S_BIT_START                         0U
#define SPI_SWITCH_MMPT_S_BIT_END                           0U

/* Definition for field MM_INT_EN in Register SPI_SWITCH */
#define SPI_SWITCH_MM_INT_EN_BIT_START                      1U
#define SPI_SWITCH_MM_INT_EN_BIT_END                        1U

/* Definition for field DATA in Register SPI_DATA1 */
#define SPI_DATA1_DATA_BIT_START                            0U
#define SPI_DATA1_DATA_BIT_END                              31U

/* Definition for field DATA in Register SPI_DATA2 */
#define SPI_DATA2_DATA_BIT_START                            0U
#define SPI_DATA2_DATA_BIT_END                              31U

/* Definition for field DATA in Register SPI_DATA3 */
#define SPI_DATA3_DATA_BIT_START                            0U
#define SPI_DATA3_DATA_BIT_END                              31U



/**
 * @struct QSPIRegs_t
 * @brief
 *   Module MSS_QSPI Register Definition
 * @details
 *   This structure is used to access the MSS_QSPI module registers.
 */
/**
 * @typedef QSPIRegs
 * @brief
 *   Module MSS_QSPI Register Frame type Definition
 * @details
 *   This type is used to access the MSS_QSPI module registers.
 */
typedef volatile struct QSPIRegs_t
{
    uint32_t    PID                                ;        /* Offset = 0x000 */
    uint32_t    MSS_QSPI_RESERVED1                 ;        /* Offset = 0x004 */
    uint32_t    MSS_QSPI_RESERVED2                 ;        /* Offset = 0x008 */
    uint32_t    MSS_QSPI_RESERVED3                 ;        /* Offset = 0x00C */
    uint32_t    SYSCONFIG                          ;        /* Offset = 0x010 */
    uint32_t    MSS_QSPI_RESERVED4                 ;        /* Offset = 0x014 */
    uint32_t    MSS_QSPI_RESERVED5                 ;        /* Offset = 0x018 */
    uint32_t    MSS_QSPI_RESERVED6                 ;        /* Offset = 0x01C */
    uint32_t    INTR_STATUS_RAW_SET                ;        /* Offset = 0x020 */
    uint32_t    INTR_STATUS_ENABLED_CLEAR          ;        /* Offset = 0x024 */
    uint32_t    INTR_ENABLE_SET                    ;        /* Offset = 0x028 */
    uint32_t    INTR_ENABLE_CLEAR                  ;        /* Offset = 0x02C */
    uint32_t    INTC_EOI                           ;        /* Offset = 0x030 */
    uint32_t    MSS_QSPI_RESERVED7                 ;        /* Offset = 0x034 */
    uint32_t    MSS_QSPI_RESERVED8                 ;        /* Offset = 0x038 */
    uint32_t    MSS_QSPI_RESERVED9                 ;        /* Offset = 0x03C */
    uint32_t    SPI_CLOCK_CNTRL                    ;        /* Offset = 0x040 */
    uint32_t    SPI_DC                             ;        /* Offset = 0x044 */
    uint32_t    SPI_CMD                            ;        /* Offset = 0x048 */
    uint32_t    SPI_STATUS                         ;        /* Offset = 0x04C */
    uint32_t    SPI_DATA                           ;        /* Offset = 0x050 */
#if 0
    uint32_t    SPI_SETUP0                         ;        /* Offset = 0x054 */
    uint32_t    SPI_SETUP1                         ;        /* Offset = 0x058 */
    uint32_t    SPI_SETUP2                         ;        /* Offset = 0x05C */
    uint32_t    SPI_SETUP3                         ;        /* Offset = 0x060 */
#else
    uint32_t    SPI_SETUP[4]                       ;        /* Offset = 0x054 */
#endif
    uint32_t    SPI_SWITCH                         ;        /* Offset = 0x064 */
    uint32_t    SPI_DATA1                          ;        /* Offset = 0x068 */
    uint32_t    SPI_DATA2                          ;        /* Offset = 0x06C */
    uint32_t    SPI_DATA3                          ;        /* Offset = 0x070 */
} QSPIRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_QSPI_H */
/* END OF REG_QSPI_H */

