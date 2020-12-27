/**
 *   @file  hw_dcan.h
 *
 *   @brief
 *      The file contains the DCAN register definitions.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HW_DCAN_H_
#define HW_DCAN_H_

#ifdef __cplusplus
extern "C"
{
#endif


/****************************************************************************************************
* Register Definitions
****************************************************************************************************/

#define DCAN_CTL                                                (0x0U)
#define DCAN_ES                                                 (0x4U)
#define DCAN_ERRC                                               (0x8U)
#define DCAN_BTR                                                (0xcU)
#define DCAN_INT                                                (0x10U)
#define DCAN_TEST                                               (0x14U)
#define DCAN_PERR                                               (0x1cU)
#define DCAN_ECCDIAG                                            (0x24U)
#define DCAN_ECCDIAG_STAT                                       (0x28U)
#define DCAN_ECCCS                                              (0x2cU)
#define DCAN_ECCSERR                                            (0x30U)
#define DCAN_ABOTR                                              (0x80U)
#define DCAN_TXRQ_X                                             (0x84U)
#define DCAN_TXRQ(n)                                            ((uint32_t)0x88 + ((n) * (uint32_t)4))
#define DCAN_NWDAT_X                                            (0x98U)
#define DCAN_NWDAT(n)                                           ((uint32_t)0x9c + ((n) * (uint32_t)4))
#define DCAN_INTPND_X                                           (0xacU)
#define DCAN_INTPND(n)                                          ((uint32_t)0xB0 + ((n) * (uint32_t)4))
#define DCAN_MSGVAL_X                                           (0xc0U)
#define DCAN_MSGVAL(n)                                          ((uint32_t)0xC4 + ((n) * (uint32_t)4))
#define DCAN_INTMUX(n)                                          ((uint32_t)0xD8 + ((n) * (uint32_t)4))
#define DCAN_IFCMD(n)                                           ((uint32_t)0x100 + (((n) - 1U) * (uint32_t)0x20))
#define DCAN_IFMSK(n)                                           ((uint32_t)0x104 + (((n) - 1U) * (uint32_t)0x20))
#define DCAN_IFARB(n)                                           ((uint32_t)0x108 + (((n) - 1U) * (uint32_t)0x20))
#define DCAN_IFMCTL(n)                                          ((uint32_t)0x10c + (((n) - 1U) * (uint32_t)0x20))
#define DCAN_IFDATA(n)                                          ((uint32_t)0x110 + (((n) - 1U) * (uint32_t)0x20))
#define DCAN_IFDATB(n)                                          ((uint32_t)0x114 + (((n) - 1U) * (uint32_t)0x20))
#define DCAN_IF3OBS                                             (0x140U)
#define DCAN_IF3UPD(n)                                          ((uint32_t)0x160 + ((n) * (uint32_t)4))
#define DCAN_TIOC                                               (0x1e0U)
#define DCAN_RIOC                                               (0x1e4U)

/****************************************************************************************************
* Field Definition Macros
****************************************************************************************************/

#define DCAN_CTL_INIT_SHIFT                                                             (0U)
#define DCAN_CTL_INIT_MASK                                                              (0x00000001U)
#define DCAN_CTL_INIT_EN_1_0X0                                                           (0U)
#define DCAN_CTL_INIT_EN_2_0X1                                                           (1U)

#define DCAN_CTL_IE0_SHIFT                                                              (1U)
#define DCAN_CTL_IE0_MASK                                                               (0x00000002U)
#define DCAN_CTL_IE0_EN_1_0X0                                                            (0U)
#define DCAN_CTL_IE0_EN_2_0X1                                                            (1U)

#define DCAN_CTL_SIE_SHIFT                                                              (2U)
#define DCAN_CTL_SIE_MASK                                                               (0x00000004U)
#define DCAN_CTL_SIE_EN_1_0X0                                                            (0U)
#define DCAN_CTL_SIE_EN_2_0X1                                                            (1U)

#define DCAN_CTL_EIE_SHIFT                                                              (3U)
#define DCAN_CTL_EIE_MASK                                                               (0x00000008U)
#define DCAN_CTL_EIE_EN_1_0X0                                                            (0U)
#define DCAN_CTL_EIE_EN_2_0X1                                                            (1U)

#define DCAN_CTL_DAR_SHIFT                                                              (5U)
#define DCAN_CTL_DAR_MASK                                                               (0x00000020U)
#define DCAN_CTL_DAR_EN_1_0X0                                                            (0U)
#define DCAN_CTL_DAR_EN_2_0X1                                                            (1U)

#define DCAN_CTL_CCE_SHIFT                                                              (6U)
#define DCAN_CTL_CCE_MASK                                                               (0x00000040U)
#define DCAN_CTL_CCE_EN_1_0X0                                                            (0U)
#define DCAN_CTL_CCE_EN_2_0X1                                                            (1U)

#define DCAN_CTL_TEST_SHIFT                                                             (7U)
#define DCAN_CTL_TEST_MASK                                                              (0x00000080U)
#define DCAN_CTL_TEST_EN_1_0X0                                                           (0U)
#define DCAN_CTL_TEST_EN_2_0X1                                                           (1U)

#define DCAN_CTL_IDS_SHIFT                                                              (8U)
#define DCAN_CTL_IDS_MASK                                                               (0x00000100U)
#define DCAN_CTL_IDS_EN_1_0X0                                                            (0U)
#define DCAN_CTL_IDS_EN_2_0X1                                                            (1U)

#define DCAN_CTL_ABO_SHIFT                                                              (9U)
#define DCAN_CTL_ABO_MASK                                                               (0x00000200U)
#define DCAN_CTL_ABO_EN_1_0X0                                                            (0U)
#define DCAN_CTL_ABO_EN_2_0X1                                                            (1U)

#define DCAN_CTL_PMD_SHIFT                                                              (10U)
#define DCAN_CTL_PMD_MASK                                                               (0x00003c00U)
#define DCAN_CTL_PMD_EN_1_0X0                                                            (0U)
#define DCAN_CTL_PMD_EN_2_0X1                                                            (1U)

#define DCAN_CTL_SWR_SHIFT                                                              (15U)
#define DCAN_CTL_SWR_MASK                                                               (uint32_t)(0x00008000U)
#define DCAN_CTL_SWR_EN_1_0X0                                                            (0U)
#define DCAN_CTL_SWR_EN_2_0X1                                                            (1U)

#define DCAN_CTL_INITDBG_SHIFT                                                          (16U)
#define DCAN_CTL_INITDBG_MASK                                                           (0x00010000U)
#define DCAN_CTL_INITDBG_EN_1_0X0                                                        (0U)
#define DCAN_CTL_INITDBG_EN_2_0X1                                                        (1U)

#define DCAN_CTL_IE1_SHIFT                                                              (17U)
#define DCAN_CTL_IE1_MASK                                                               (0x00020000U)
#define DCAN_CTL_IE1_EN_1_0X0                                                            (0U)
#define DCAN_CTL_IE1_EN_2_0X1                                                            (1U)

#define DCAN_CTL_DE1_SHIFT                                                              (18U)
#define DCAN_CTL_DE1_MASK                                                               (0x00040000U)
#define DCAN_CTL_DE1_EN_1_0X0                                                            (0U)
#define DCAN_CTL_DE1_EN_2_0X1                                                            (1U)

#define DCAN_CTL_DE2_SHIFT                                                              (19U)
#define DCAN_CTL_DE2_MASK                                                               (0x00080000U)
#define DCAN_CTL_DE2_EN_1_0X0                                                            (0U)
#define DCAN_CTL_DE2_EN_2_0X1                                                            (1U)

#define DCAN_CTL_DE3_SHIFT                                                              (20U)
#define DCAN_CTL_DE3_MASK                                                               (0x00100000U)
#define DCAN_CTL_DE3_EN_1_0X0                                                            (0U)
#define DCAN_CTL_DE3_EN_2_0X1                                                            (1U)

#define DCAN_CTL_PDR_SHIFT                                                              (24U)
#define DCAN_CTL_PDR_MASK                                                               (0x01000000U)
#define DCAN_CTL_PDR_EN_1_0X0                                                            (0U)
#define DCAN_CTL_PDR_EN_2_0X1                                                            (1U)

#define DCAN_CTL_WUBA_SHIFT                                                             (25U)
#define DCAN_CTL_WUBA_MASK                                                              (0x02000000U)
#define DCAN_CTL_WUBA_EN_1_0X0                                                           (0U)
#define DCAN_CTL_WUBA_EN_2_0X1                                                           (1U)

#define DCAN_ES_LEC_SHIFT                                                               (0U)
#define DCAN_ES_LEC_MASK                                                                (0x00000007U)
#define DCAN_ES_LEC_EN_1_0X0                                                             (0U)
#define DCAN_ES_LEC_EN_2_0X1                                                             (1U)
#define DCAN_ES_LEC_EN_3_0X2                                                             (2U)
#define DCAN_ES_LEC_EN_4_0X3                                                             (3U)
#define DCAN_ES_LEC_EN_5_0X4                                                             (4U)
#define DCAN_ES_LEC_EN_6_0X5                                                             (5U)
#define DCAN_ES_LEC_EN_7_0X6                                                             (6U)
#define DCAN_ES_LEC_EN_8_0X7                                                             (7U)

#define DCAN_ES_TXOK_SHIFT                                                              (3U)
#define DCAN_ES_TXOK_MASK                                                               (0x00000008U)
#define DCAN_ES_TXOK_EN_1_0X0                                                            (0U)
#define DCAN_ES_TXOK_EN_2_0X1                                                            (1U)

#define DCAN_ES_RXOK_SHIFT                                                              (4U)
#define DCAN_ES_RXOK_MASK                                                               (0x00000010U)
#define DCAN_ES_RXOK_EN_1_0X0                                                            (0U)
#define DCAN_ES_RXOK_EN_2_0X1                                                            (1U)

#define DCAN_ES_EPASS_SHIFT                                                             (5U)
#define DCAN_ES_EPASS_MASK                                                              (0x00000020U)
#define DCAN_ES_EPASS_EN_1_0X0                                                           (0U)
#define DCAN_ES_EPASS_EN_2_0X1                                                           (1U)

#define DCAN_ES_EWARN_SHIFT                                                             (6U)
#define DCAN_ES_EWARN_MASK                                                              (0x00000040U)
#define DCAN_ES_EWARN_EN_1_0X0                                                           (0U)
#define DCAN_ES_EWARN_EN_2_0X1                                                           (1U)

#define DCAN_ES_BOFF_SHIFT                                                              (7U)
#define DCAN_ES_BOFF_MASK                                                               (0x00000080U)
#define DCAN_ES_BOFF_EN_1_0X0                                                            (0U)
#define DCAN_ES_BOFF_EN_2_0X1                                                            (1U)

#define DCAN_ES_PER_SHIFT                                                               (8U)
#define DCAN_ES_PER_MASK                                                                (0x00000100U)
#define DCAN_ES_PER_EN_1_0X0                                                             (0U)
#define DCAN_ES_PER_EN_2_0X1                                                             (1U)

#define DCAN_ES_WAKEUP_PND_SHIFT                                                        (9U)
#define DCAN_ES_WAKEUP_PND_MASK                                                         (0x00000200U)
#define DCAN_ES_WAKEUP_PND_EN_1_0X0                                                      (0U)
#define DCAN_ES_WAKEUP_PND_EN_2_0X1                                                      (1U)

#define DCAN_ES_PDA_SHIFT                                                               (10U)
#define DCAN_ES_PDA_MASK                                                                (0x00000400U)
#define DCAN_ES_PDA_EN_1_0X0                                                             (0U)
#define DCAN_ES_PDA_EN_2_0X1                                                             (1U)

#define DCAN_ERRC_TEC_SHIFT                                                             (0U)
#define DCAN_ERRC_TEC_MASK                                                              (0x000000ffU)

#define DCAN_ERRC_REC_SHIFT                                                             (8U)
#define DCAN_ERRC_REC_MASK                                                              (0x00007f00U)

#define DCAN_ERRC_RP_SHIFT                                                              (15U)
#define DCAN_ERRC_RP_MASK                                                               (0x00008000U)
#define DCAN_ERRC_RP_EN_1_0X0                                                            (0U)
#define DCAN_ERRC_RP_EN_2_0X1                                                            (1U)

#define DCAN_BTR_BRP_SHIFT                                                              (uint32_t)(0U)
#define DCAN_BTR_BRP_MASK                                                                (0x0000003fU)

#define DCAN_BTR_SJW_SHIFT                                                              (uint32_t)(6U)
#define DCAN_BTR_SJW_MASK                                                               (0x000000c0U)

#define DCAN_BTR_TSEG1_SHIFT                                                            (uint32_t)(8U)
#define DCAN_BTR_TSEG1_MASK                                                             (0x00000f00U)

#define DCAN_BTR_TSEG2_SHIFT                                                            (uint32_t)(12U)
#define DCAN_BTR_TSEG2_MASK                                                             (0x00007000U)

#define DCAN_BTR_BRPE_SHIFT                                                             (uint32_t)(16U)
#define DCAN_BTR_BRPE_MASK                                                              (0x000f0000U)

#define DCAN_INT_INT0ID_SHIFT                                                           (0U)
#define DCAN_INT_INT0ID_MASK                                                            (0x0000ffffU)
#define DCAN_INT_INT0ID_EN_1_0X0                                                         (0U)
#define DCAN_INT_INT0ID_EN_2_0X0                                                         (0U)
#define DCAN_INT_INT0ID_EN_3_0X0                                                         (0U)
#define DCAN_INT_INT0ID_EN_4_0X0080                                                      (128U)
#define DCAN_INT_INT0ID_EN_5_0X1F40                                                      (8000U)

#define DCAN_INT_INT1ID_SHIFT                                                           (16U)
#define DCAN_INT_INT1ID_MASK                                                            (0x00ff0000U)
#define DCAN_INT_INT1ID_EN_1_0X0                                                         (0U)
#define DCAN_INT_INT1ID_EN_2_0X0                                                         (0U)
#define DCAN_INT_INT1ID_EN_3_0X80                                                        (128U)

#define DCAN_TEST_SILENT_SHIFT                                                          (3U)
#define DCAN_TEST_SILENT_MASK                                                           (0x00000008U)
#define DCAN_TEST_SILENT_EN_1_0X0                                                        (0U)
#define DCAN_TEST_SILENT_EN_2_0X1                                                        (1U)

#define DCAN_TEST_LBACK_SHIFT                                                           (4U)
#define DCAN_TEST_LBACK_MASK                                                            (0x00000010U)
#define DCAN_TEST_LBACK_EN_1_0X0                                                         (0U)
#define DCAN_TEST_LBACK_EN_2_0X1                                                         (1U)

#define DCAN_TEST_TX_SHIFT                                                              (5U)
#define DCAN_TEST_TX_MASK                                                               (0x00000060U)
#define DCAN_TEST_TX_EN_1_0X0                                                            (0U)
#define DCAN_TEST_TX_EN_2_0X1                                                            (1U)
#define DCAN_TEST_TX_EN_3_0XA                                                            (10U)
#define DCAN_TEST_TX_EN_4_0XB                                                            (11U)

#define DCAN_TEST_RX_SHIFT                                                              (7U)
#define DCAN_TEST_RX_MASK                                                               (0x00000080U)
#define DCAN_TEST_RX_EN_1_0X0                                                            (0U)
#define DCAN_TEST_RX_EN_2_0X1                                                            (1U)

#define DCAN_TEST_EXL_SHIFT                                                             (8U)
#define DCAN_TEST_EXL_MASK                                                              (0x00000100U)
#define DCAN_TEST_EXL_EN_1_0X0                                                           (0U)
#define DCAN_TEST_EXL_EN_2_0X1                                                           (1U)

#define DCAN_TEST_RDA_SHIFT                                                             (9U)
#define DCAN_TEST_RDA_MASK                                                              (0x00000200U)
#define DCAN_TEST_RDA_EN_1_0X0                                                           (0U)
#define DCAN_TEST_RDA_EN_2_0X1                                                           (1U)

#define DCAN_PERR_MESSAGE_NUMBER_SHIFT                                                  (0U)
#define DCAN_PERR_MESSAGE_NUMBER_MASK                                                   (0x000000ffU)
#define DCAN_PERR_MESSAGE_NUMBER_EN_1_0X80                                               (128U)

#define DCAN_PERR_WORD_NUMBER_SHIFT                                                     (8U)
#define DCAN_PERR_WORD_NUMBER_MASK                                                      (0x00000700U)
#define DCAN_PERR_WORD_NUMBER_EN_1_0X05                                                 (5U)

#define DCAN_ECCDIAG_ECCDIAG_SHIFT                                                      (0U)
#define DCAN_ECCDIAG_ECCDIAG_MASK                                                       (0x0000000fU)

#define DCAN_ECCDIAG_STAT_SE_FLAG_SHIFT                                                 (0U)
#define DCAN_ECCDIAG_STAT_SE_FLAG_MASK                                                  (0x00000001U)

#define DCAN_ECCDIAG_STAT_DE_FLAG_SHIFT                                                 (8U)
#define DCAN_ECCDIAG_STAT_DE_FLAG_MASK                                                  (0x00000100U)

#define DCAN_ECCCS_SBE_EVT_EN_SHIFT                                                     (24U)
#define DCAN_ECCCS_SBE_EVT_EN_MASK                                                      (0x0f000000U)
#define DCAN_ECCCS_SBE_EVT_EN_EN_1_0X0                                                  (0U)
#define DCAN_ECCCS_SBE_EVT_EN_EN_2_0X1                                                  (1U)

#define DCAN_ECCCS_ECCMODE_SHIFT                                                        (16U)
#define DCAN_ECCCS_ECCMODE_MASK                                                         (0x000f0000U)
#define DCAN_ECCCS_ECCMODE_EN_1_0X0                                                     (0U)
#define DCAN_ECCCS_ECCMODE_EN_2_0X1                                                     (1U)

#define DCAN_ECCCS_DE_FLAG_SHIFT                                                        (8U)
#define DCAN_ECCCS_DE_FLAG_MASK                                                         (0x00000100U)

#define DCAN_ECCCS_SE_FLAG_SHIFT                                                        (0U)
#define DCAN_ECCCS_SE_FLAG_MASK                                                         (0x00000001U)

#define DCAN_ECCSERR_MSG_NUM_SHIFT                                                      (0U)
#define DCAN_ECCSERR_MSG_NUM_MASK                                                       (0x000000ffU)

#define DCAN_ABOTR_ABO_TIME_SHIFT                                                       (0U)
#define DCAN_ABOTR_ABO_TIME_MASK                                                        (0xffffffffU)

#define DCAN_TXRQ_X_TXRQSTREG1_SHIFT                                                    (0U)
#define DCAN_TXRQ_X_TXRQSTREG1_MASK                                                     (0x00000003U)

#define DCAN_TXRQ_X_TXRQSTREG2_SHIFT                                                    (2U)
#define DCAN_TXRQ_X_TXRQSTREG2_MASK                                                     (0x0000000cU)

#define DCAN_TXRQ_X_TXRQSTREG3_SHIFT                                                    (4U)
#define DCAN_TXRQ_X_TXRQSTREG3_MASK                                                     (0x00000030U)

#define DCAN_TXRQ_X_TXRQSTREG4_SHIFT                                                    (6U)
#define DCAN_TXRQ_X_TXRQSTREG4_MASK                                                     (0x000000c0U)

#define DCAN_TXRQ_X_TXRQSTREG5_SHIFT                                                    (8U)
#define DCAN_TXRQ_X_TXRQSTREG5_MASK                                                     (0x00000300U)

#define DCAN_TXRQ_X_TXRQSTREG6_SHIFT                                                    (10U)
#define DCAN_TXRQ_X_TXRQSTREG6_MASK                                                     (0x00000c00U)

#define DCAN_TXRQ_X_TXRQSTREG7_SHIFT                                                    (12U)
#define DCAN_TXRQ_X_TXRQSTREG7_MASK                                                     (0x00003000U)

#define DCAN_TXRQ_X_TXRQSTREG8_SHIFT                                                    (14U)
#define DCAN_TXRQ_X_TXRQSTREG8_MASK                                                     (0x0000c000U)

#define DCAN_TXRQ12_TXRQS_0_SHIFT                                                       (0U)
#define DCAN_TXRQ12_TXRQS_0_MASK                                                        (0x0000ffffU)
#define DCAN_TXRQ12_TXRQS_0_EN_1_0X0                                                     (0U)
#define DCAN_TXRQ12_TXRQS_0_EN_2_0X1                                                     (1U)

#define DCAN_TXRQ12_TXRQS_1_SHIFT                                                       (16U)
#define DCAN_TXRQ12_TXRQS_1_MASK                                                        (0xffff0000U)
#define DCAN_TXRQ12_TXRQS_1_EN_0X0                                                       (0U)
#define DCAN_TXRQ12_TXRQS_1_EN_2_0X1                                                     (1U)

#define DCAN_TXRQ34_TXRQS_0_SHIFT                                                       (0U)
#define DCAN_TXRQ34_TXRQS_0_MASK                                                        (0x0000ffffU)
#define DCAN_TXRQ34_TXRQS_0_EN_1_0X0                                                     (0U)
#define DCAN_TXRQ34_TXRQS_0_EN_2_0X1                                                     (1U)

#define DCAN_TXRQ34_TXRQS_1_SHIFT                                                       (16U)
#define DCAN_TXRQ34_TXRQS_1_MASK                                                        (0xffff0000U)
#define DCAN_TXRQ34_TXRQS_1_EN_0X0                                                       (0U)
#define DCAN_TXRQ34_TXRQS_1_EN_2_0X1                                                     (1U)

#define DCAN_TXRQ56_TXRQS_0_SHIFT                                                       (0U)
#define DCAN_TXRQ56_TXRQS_0_MASK                                                        (0x0000ffffU)
#define DCAN_TXRQ56_TXRQS_0_EN_1_0X0                                                     (0U)
#define DCAN_TXRQ56_TXRQS_0_EN_2_0X1                                                     (1U)

#define DCAN_TXRQ56_TXRQS_1_SHIFT                                                       (16U)
#define DCAN_TXRQ56_TXRQS_1_MASK                                                        (0xffff0000U)
#define DCAN_TXRQ56_TXRQS_1_EN_0X0                                                       (0U)
#define DCAN_TXRQ56_TXRQS_1_EN_2_0X1                                                     (1U)

#define DCAN_TXRQ78_TXRQS_0_SHIFT                                                       (0U)
#define DCAN_TXRQ78_TXRQS_0_MASK                                                        (0x0000ffffU)
#define DCAN_TXRQ78_TXRQS_0_EN_1_0X0                                                     (0U)
#define DCAN_TXRQ78_TXRQS_0_EN_2_0X1                                                     (1U)

#define DCAN_TXRQ78_TXRQS_1_SHIFT                                                       (16U)
#define DCAN_TXRQ78_TXRQS_1_MASK                                                        (0xffff0000U)
#define DCAN_TXRQ78_TXRQS_1_EN_0X0                                                       (0U)
#define DCAN_TXRQ78_TXRQS_1_EN_2_0X1                                                     (1U)

#define DCAN_NWDAT_X_NEWDATREG1_SHIFT                                                   (0U)
#define DCAN_NWDAT_X_NEWDATREG1_MASK                                                    (0x00000003U)

#define DCAN_NWDAT_X_NEWDATREG2_SHIFT                                                   (2U)
#define DCAN_NWDAT_X_NEWDATREG2_MASK                                                    (0x0000000cU)

#define DCAN_NWDAT_X_NEWDATREG3_SHIFT                                                   (4U)
#define DCAN_NWDAT_X_NEWDATREG3_MASK                                                    (0x00000030U)

#define DCAN_NWDAT_X_NEWDATREG4_SHIFT                                                   (6U)
#define DCAN_NWDAT_X_NEWDATREG4_MASK                                                    (0x000000c0U)

#define DCAN_NWDAT_X_NEWDATREG5_SHIFT                                                   (8U)
#define DCAN_NWDAT_X_NEWDATREG5_MASK                                                    (0x00000300U)

#define DCAN_NWDAT_X_NEWDATREG6_SHIFT                                                   (10U)
#define DCAN_NWDAT_X_NEWDATREG6_MASK                                                    (0x00000c00U)

#define DCAN_NWDAT_X_NEWDATREG7_SHIFT                                                   (12U)
#define DCAN_NWDAT_X_NEWDATREG7_MASK                                                    (0x00003000U)

#define DCAN_NWDAT_X_NEWDATREG8_SHIFT                                                   (14U)
#define DCAN_NWDAT_X_NEWDATREG8_MASK                                                    (0x0000c000U)

#define DCAN_NWDAT12_NEWDAT_0_SHIFT                                                     (0U)
#define DCAN_NWDAT12_NEWDAT_0_MASK                                                      (0x0000ffffU)
#define DCAN_NWDAT12_NEWDAT_0_EN_1_0X0                                                   (0U)
#define DCAN_NWDAT12_NEWDAT_0_EN_2_0X1                                                   (1U)

#define DCAN_NWDAT12_NEWDAT_1_SHIFT                                                     (16U)
#define DCAN_NWDAT12_NEWDAT_1_MASK                                                      (0xffff0000U)
#define DCAN_NWDAT12_NEWDAT_1_EN_0X0                                                     (0U)
#define DCAN_NWDAT12_NEWDAT_1_EN_2_0X1                                                   (1U)

#define DCAN_NWDAT34_NEWDAT_0_SHIFT                                                     (0U)
#define DCAN_NWDAT34_NEWDAT_0_MASK                                                      (0x0000ffffU)
#define DCAN_NWDAT34_NEWDAT_0_EN_1_0X0                                                   (0U)
#define DCAN_NWDAT34_NEWDAT_0_EN_2_0X1                                                   (1U)

#define DCAN_NWDAT34_NEWDAT_1_SHIFT                                                     (16U)
#define DCAN_NWDAT34_NEWDAT_1_MASK                                                      (0xffff0000U)
#define DCAN_NWDAT34_NEWDAT_1_EN_0X0                                                     (0U)
#define DCAN_NWDAT34_NEWDAT_1_EN_2_0X1                                                   (1U)

#define DCAN_NWDAT56_NEWDAT_0_SHIFT                                                     (0U)
#define DCAN_NWDAT56_NEWDAT_0_MASK                                                      (0x0000ffffU)
#define DCAN_NWDAT56_NEWDAT_0_EN_1_0X0                                                   (0U)
#define DCAN_NWDAT56_NEWDAT_0_EN_2_0X1                                                   (1U)

#define DCAN_NWDAT56_NEWDAT_1_SHIFT                                                     (16U)
#define DCAN_NWDAT56_NEWDAT_1_MASK                                                      (0xffff0000U)
#define DCAN_NWDAT56_NEWDAT_1_EN_0X0                                                     (0U)
#define DCAN_NWDAT56_NEWDAT_1_EN_2_0X1                                                   (1U)

#define DCAN_NWDAT78_NEWDAT_0_SHIFT                                                     (0U)
#define DCAN_NWDAT78_NEWDAT_0_MASK                                                      (0x0000ffffU)
#define DCAN_NWDAT78_NEWDAT_0_EN_1_0X0                                                   (0U)
#define DCAN_NWDAT78_NEWDAT_0_EN_2_0X1                                                   (1U)

#define DCAN_NWDAT78_NEWDAT_1_SHIFT                                                     (16U)
#define DCAN_NWDAT78_NEWDAT_1_MASK                                                      (0xffff0000U)
#define DCAN_NWDAT78_NEWDAT_1_EN_0X0                                                     (0U)
#define DCAN_NWDAT78_NEWDAT_1_EN_2_0X1                                                   (1U)

#define DCAN_INTPND_X_INTPNDREG1_SHIFT                                                  (0U)
#define DCAN_INTPND_X_INTPNDREG1_MASK                                                   (0x00000003U)

#define DCAN_INTPND_X_INTPNDREG2_SHIFT                                                  (2U)
#define DCAN_INTPND_X_INTPNDREG2_MASK                                                   (0x0000000cU)

#define DCAN_INTPND_X_INTPNDREG3_SHIFT                                                  (4U)
#define DCAN_INTPND_X_INTPNDREG3_MASK                                                   (0x00000030U)

#define DCAN_INTPND_X_INTPNDREG4_SHIFT                                                  (6U)
#define DCAN_INTPND_X_INTPNDREG4_MASK                                                   (0x000000c0U)

#define DCAN_INTPND_X_INTPNDREG5_SHIFT                                                  (8U)
#define DCAN_INTPND_X_INTPNDREG5_MASK                                                   (0x00000300U)

#define DCAN_INTPND_X_INTPNDREG6_SHIFT                                                  (10U)
#define DCAN_INTPND_X_INTPNDREG6_MASK                                                   (0x00000c00U)

#define DCAN_INTPND_X_INTPNDREG7_SHIFT                                                  (12U)
#define DCAN_INTPND_X_INTPNDREG7_MASK                                                   (0x00003000U)

#define DCAN_INTPND_X_INTPNDREG8_SHIFT                                                  (14U)
#define DCAN_INTPND_X_INTPNDREG8_MASK                                                   (0x0000c000U)

#define DCAN_INTPND12_INTPND_0_SHIFT                                                    (0U)
#define DCAN_INTPND12_INTPND_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTPND12_INTPND_0_EN_1_0X0                                                  (0U)
#define DCAN_INTPND12_INTPND_0_EN_2_0X1                                                  (1U)

#define DCAN_INTPND12_INTPND_1_SHIFT                                                    (16U)
#define DCAN_INTPND12_INTPND_1_MASK                                                     (0xffff0000U)
#define DCAN_INTPND12_INTPND_1_EN_0X0                                                    (0U)
#define DCAN_INTPND12_INTPND_1_EN_2_0X1                                                  (1U)

#define DCAN_INTPND34_INTPND_0_SHIFT                                                    (0U)
#define DCAN_INTPND34_INTPND_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTPND34_INTPND_0_EN_1_0X0                                                  (0U)
#define DCAN_INTPND34_INTPND_0_EN_2_0X1                                                  (1U)

#define DCAN_INTPND34_INTPND_1_SHIFT                                                    (16U)
#define DCAN_INTPND34_INTPND_1_MASK                                                     (0xffff0000U)
#define DCAN_INTPND34_INTPND_1_EN_0X0                                                    (0U)
#define DCAN_INTPND34_INTPND_1_EN_2_0X1                                                  (1U)

#define DCAN_INTPND56_INTPND_0_SHIFT                                                    (0U)
#define DCAN_INTPND56_INTPND_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTPND56_INTPND_0_EN_1_0X0                                                  (0U)
#define DCAN_INTPND56_INTPND_0_EN_2_0X1                                                  (1U)

#define DCAN_INTPND56_INTPND_1_SHIFT                                                    (16U)
#define DCAN_INTPND56_INTPND_1_MASK                                                     (0xffff0000U)
#define DCAN_INTPND56_INTPND_1_EN_0X0                                                    (0U)
#define DCAN_INTPND56_INTPND_1_EN_2_0X1                                                  (1U)

#define DCAN_INTPND78_INTPND_0_SHIFT                                                    (0U)
#define DCAN_INTPND78_INTPND_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTPND78_INTPND_0_EN_1_0X0                                                  (0U)
#define DCAN_INTPND78_INTPND_0_EN_2_0X1                                                  (1U)

#define DCAN_INTPND78_INTPND_1_SHIFT                                                    (16U)
#define DCAN_INTPND78_INTPND_1_MASK                                                     (0xffff0000U)
#define DCAN_INTPND78_INTPND_1_EN_0X0                                                    (0U)
#define DCAN_INTPND78_INTPND_1_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL_X_MSGVALREG1_SHIFT                                                  (0U)
#define DCAN_MSGVAL_X_MSGVALREG1_MASK                                                   (0x00000003U)

#define DCAN_MSGVAL_X_MSGVALREG2_SHIFT                                                  (2U)
#define DCAN_MSGVAL_X_MSGVALREG2_MASK                                                   (0x0000000cU)

#define DCAN_MSGVAL_X_MSGVALREG3_SHIFT                                                  (4U)
#define DCAN_MSGVAL_X_MSGVALREG3_MASK                                                   (0x00000030U)

#define DCAN_MSGVAL_X_MSGVALREG4_SHIFT                                                  (6U)
#define DCAN_MSGVAL_X_MSGVALREG4_MASK                                                   (0x000000c0U)

#define DCAN_MSGVAL_X_MSGVALREG5_SHIFT                                                  (8U)
#define DCAN_MSGVAL_X_MSGVALREG5_MASK                                                   (0x00000300U)

#define DCAN_MSGVAL_X_MSGVALREG6_SHIFT                                                  (10U)
#define DCAN_MSGVAL_X_MSGVALREG6_MASK                                                   (0x00000c00U)

#define DCAN_MSGVAL_X_MSGVALREG7_SHIFT                                                  (12U)
#define DCAN_MSGVAL_X_MSGVALREG7_MASK                                                   (0x00003000U)

#define DCAN_MSGVAL_X_MSGVALREG8_SHIFT                                                  (14U)
#define DCAN_MSGVAL_X_MSGVALREG8_MASK                                                   (0x0000c000U)

#define DCAN_MSGVAL12_MSGVAL_0_SHIFT                                                    (0U)
#define DCAN_MSGVAL12_MSGVAL_0_MASK                                                     (0x0000ffffU)
#define DCAN_MSGVAL12_MSGVAL_0_EN_1_0X0                                                  (0U)
#define DCAN_MSGVAL12_MSGVAL_0_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL12_MSGVAL_1_SHIFT                                                    (16U)
#define DCAN_MSGVAL12_MSGVAL_1_MASK                                                     (0xffff0000U)
#define DCAN_MSGVAL12_MSGVAL_1_EN_0X0                                                    (0U)
#define DCAN_MSGVAL12_MSGVAL_1_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL34_MSGVAL_0_SHIFT                                                    (0U)
#define DCAN_MSGVAL34_MSGVAL_0_MASK                                                     (0x0000ffffU)
#define DCAN_MSGVAL34_MSGVAL_0_EN_1_0X0                                                  (0U)
#define DCAN_MSGVAL34_MSGVAL_0_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL34_MSGVAL_1_SHIFT                                                    (16U)
#define DCAN_MSGVAL34_MSGVAL_1_MASK                                                     (0xffff0000U)
#define DCAN_MSGVAL34_MSGVAL_1_EN_0X0                                                    (0U)
#define DCAN_MSGVAL34_MSGVAL_1_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL56_MSGVAL_0_SHIFT                                                    (0U)
#define DCAN_MSGVAL56_MSGVAL_0_MASK                                                     (0x0000ffffU)
#define DCAN_MSGVAL56_MSGVAL_0_EN_1_0X0                                                  (0U)
#define DCAN_MSGVAL56_MSGVAL_0_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL56_MSGVAL_1_SHIFT                                                    (16U)
#define DCAN_MSGVAL56_MSGVAL_1_MASK                                                     (0xffff0000U)
#define DCAN_MSGVAL56_MSGVAL_1_EN_0X0                                                    (0U)
#define DCAN_MSGVAL56_MSGVAL_1_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL78_MSGVAL_0_SHIFT                                                    (0U)
#define DCAN_MSGVAL78_MSGVAL_0_MASK                                                     (0x0000ffffU)
#define DCAN_MSGVAL78_MSGVAL_0_EN_1_0X0                                                  (0U)
#define DCAN_MSGVAL78_MSGVAL_0_EN_2_0X1                                                  (1U)

#define DCAN_MSGVAL78_MSGVAL_1_SHIFT                                                    (16U)
#define DCAN_MSGVAL78_MSGVAL_1_MASK                                                     (0xffff0000U)
#define DCAN_MSGVAL78_MSGVAL_1_EN_0X0                                                    (0U)
#define DCAN_MSGVAL78_MSGVAL_1_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX12_INTMUX_0_SHIFT                                                    (0U)
#define DCAN_INTMUX12_INTMUX_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTMUX12_INTMUX_0_EN_1_0X0                                                  (0U)
#define DCAN_INTMUX12_INTMUX_0_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX12_INTMUX_1_SHIFT                                                    (16U)
#define DCAN_INTMUX12_INTMUX_1_MASK                                                     (0xffff0000U)
#define DCAN_INTMUX12_INTMUX_1_EN_0X0                                                    (0U)
#define DCAN_INTMUX12_INTMUX_1_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX34_INTMUX_0_SHIFT                                                    (0U)
#define DCAN_INTMUX34_INTMUX_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTMUX34_INTMUX_0_EN_1_0X0                                                  (0U)
#define DCAN_INTMUX34_INTMUX_0_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX34_INTMUX_1_SHIFT                                                    (16U)
#define DCAN_INTMUX34_INTMUX_1_MASK                                                     (0xffff0000U)
#define DCAN_INTMUX34_INTMUX_1_EN_0X0                                                    (0U)
#define DCAN_INTMUX34_INTMUX_1_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX56_INTMUX_0_SHIFT                                                    (0U)
#define DCAN_INTMUX56_INTMUX_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTMUX56_INTMUX_0_EN_1_0X0                                                  (0U)
#define DCAN_INTMUX56_INTMUX_0_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX56_INTMUX_1_SHIFT                                                    (16U)
#define DCAN_INTMUX56_INTMUX_1_MASK                                                     (0xffff0000U)
#define DCAN_INTMUX56_INTMUX_1_EN_0X0                                                    (0U)
#define DCAN_INTMUX56_INTMUX_1_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX78_INTMUX_0_SHIFT                                                    (0U)
#define DCAN_INTMUX78_INTMUX_0_MASK                                                     (0x0000ffffU)
#define DCAN_INTMUX78_INTMUX_0_EN_1_0X0                                                  (0U)
#define DCAN_INTMUX78_INTMUX_0_EN_2_0X1                                                  (1U)

#define DCAN_INTMUX78_INTMUX_1_SHIFT                                                    (16U)
#define DCAN_INTMUX78_INTMUX_1_MASK                                                     (0xffff0000U)
#define DCAN_INTMUX78_INTMUX_1_EN_0X0                                                    (0U)
#define DCAN_INTMUX78_INTMUX_1_EN_2_0X1                                                  (1U)

/* IFxCMD mu */
#define DCAN_IFCMD_ARB_MASK                                                              (0x00200000U)
#define DCAN_IFCMD_ARB_SHIFT                                                             (0x00000015U)
#define DCAN_IFCMD_ARB_NOCHANGE                                                          (0x0U)
#define DCAN_IFCMD_ARB_OBJTOREG                                                          (0x1U)
#define DCAN_IFCMD_ARB_REGTOOBJ                                                          (0x1U)
#define DCAN_IFCMD_ARB_EN_1_0X0                                                          (0U)
#define DCAN_IFCMD_ARB_EN_2_0X1                                                          (1U)

#define DCAN_IFCMD_BUSY_MASK                                                             (0x00008000U)
#define DCAN_IFCMD_BUSY_SHIFT                                                            (0x0000000FU)
#define DCAN_IFCMD_BUSY_NOTRANSFER                                                       (0x0U)
#define DCAN_IFCMD_BUSY_TRANSFER                                                         (0x1U)

#define DCAN_IFCMD_CLRINTPND_MASK                                                        (0x00080000U)
#define DCAN_IFCMD_CLRINTPND_SHIFT                                                       (0x00000013U)
#define DCAN_IFCMD_CLRINTPND_CHANGE                                                      (0x1U)
#define DCAN_IFCMD_CLRINTPND_NOCHANGE                                                    (0x0U)
#define DCAN_IFCMD_CLRINTPND_EN_1_0X0                                                    (0U)
#define DCAN_IFCMD_CLRINTPND_EN_2_0X1                                                    (1U)

#define DCAN_IFCMD_CONTROL_MASK                                                          (0x00100000U)
#define DCAN_IFCMD_CONTROL_SHIFT                                                         (0x00000014U)
#define DCAN_IFCMD_CONTROL_EN_1_0X0                                                      (0U)
#define DCAN_IFCMD_CONTROL_EN_2_0X1                                                      (1U)

#define DCAN_IFCMD_DATAA_MASK                                                            (0x00020000U)
#define DCAN_IFCMD_DATAA_SHIFT                                                           (0x00000011U)
#define DCAN_IFCMD_DATAA_EN_1_0X0                                                        (0U)
#define DCAN_IFCMD_DATAA_EN_2_0X1                                                        (1U)

#define DCAN_IFCMD_DATAB_MASK                                                            (0x00010000U)
#define DCAN_IFCMD_DATAB_SHIFT                                                           (0x00000010U)
#define DCAN_IFCMD_DATAB_EN_1_0X0                                                        (0U)
#define DCAN_IFCMD_DATAB_EN_2_0X1                                                        (1U)

#define DCAN_IFCMD_DMAACTIVE_MASK                                                        (0x00004000U)
#define DCAN_IFCMD_DMAACTIVE_SHIFT                                                       (0x0000000EU)
#define DCAN_IFCMD_DMAACTIVE_ACTIVE                                                      (0x1U)
#define DCAN_IFCMD_DMAACTIVE_INACTIVE                                                    (0x0U)

#define DCAN_IFCMD_MASK_MASK                                                             (0x00400000U)
#define DCAN_IFCMD_MASK_SHIFT                                                            (0x00000016U)
#define DCAN_IFCMD_MASK_EN_1_0X0                                                         (0U)
#define DCAN_IFCMD_MASK_EN_2_0X1                                                         (1U)

#define DCAN_IFCMD_MESSAGENUMBER_MASK                                                    (0x000000FFU)
#define DCAN_IFCMD_MESSAGENUMBER_SHIFT                                                   (0x00000000U)

#define DCAN_IFCMD_TXRQST_NEWDAT_MASK                                                    (0x00040000U)
#define DCAN_IFCMD_TXRQST_NEWDAT_SHIFT                                                   (0x00000012U)
#define DCAN_IFCMD_TXRQST_NEWDAT_EN_1_0X0                                                (0U)
#define DCAN_IFCMD_TXRQST_NEWDAT_EN_2_0X1                                                (1U)


#define DCAN_IFCMD_WR_RD_MASK                                                            (0x00800000U)
#define DCAN_IFCMD_WR_RD_SHIFT                                                           (0x00000017U)
#define DCAN_IFCMD_WR_RD_EN_1_0X0                                                        (0U)
#define DCAN_IFCMD_WR_RD_EN_2_0X1                                                        (1U)


/* IFxMSK mu */
#define DCAN_IFMSK_MDIR_MASK                                                             (0x40000000U)
#define DCAN_IFMSK_MDIR_SHIFT                                                            (0x00000001EU)
#define DCAN_IFMSK_MDIR_NOTUSED                                                          (0x0U)
#define DCAN_IFMSK_MDIR_USED                                                             (0x1U)

#define DCAN_IFMSK_MSK_MASK                                                              (0x1FFFFFFFU)
#define DCAN_IFMSK_MSK_SHIFT                                                             (0x00000000U)
#define DCAN_IFMSK_MSK_NOTUSED                                                           (0x0U)
#define DCAN_IFMSK_MSK_USED                                                              (0x1U)

#define DCAN_IFMSK_MXTD_MASK                                                             (0x80000000U)
#define DCAN_IFMSK_MXTD_SHIFT                                                            (0x00000001FU)
#define DCAN_IFMSK_MXTD_NOTUSED                                                          (0x0U)
#define DCAN_IFMSK_MXTD_USED                                                             (0x1U)


/* IFxARB mu */
#define DCAN_IFARB_DIR_MASK                                                              (0x20000000U)
#define DCAN_IFARB_DIR_SHIFT                                                             (0x0000001DU)
#define DCAN_IFARB_DIR_RECEIVE                                                           (0x0U)
#define DCAN_IFARB_DIR_TRANSMIT                                                          (0x1U)

#define DCAN_IFARB_MSGVAL_MASK                                                           (0x80000000U)
#define DCAN_IFARB_MSGVAL_SHIFT                                                          (0x0000001FU)
#define DCAN_IFARB_MSGVAL_IGNORED                                                        (0x0U)
#define DCAN_IFARB_MSGVAL_USED                                                           (0x1U)

#define DCAN_IFARB_MSGID_MASK                                                            (0x1FFFFFFFU)
#define DCAN_IFARB_MSGID_SHIFT                                                           (0x000000000U)

#define DCAN_IFARB_XTD_MASK                                                              (0x40000000U)
#define DCAN_IFARB_XTD_SHIFT                                                             (0x0000001EU)
#define DCAN_IFARB_XTD_11_BIT                                                            (0x0U)
#define DCAN_IFARB_XTD_29_BIT                                                            (0x1U)

/* IFxMCTL mu */
#define DCAN_IFMCTL_DATALENGTHCODE_MASK                                                  (0x0000000FU)
#define DCAN_IFMCTL_DATALENGTHCODE_SHIFT                                                 (0x00000000U)

#define DCAN_IFMCTL_EOB_MASK                                                             (0x00000080U)
#define DCAN_IFMCTL_EOB_SHIFT                                                            (0x00000007U)
#define DCAN_IFMCTL_EOB_BYTE8                                                            (0U)
#define DCAN_IFMCTL_EOB_SAME                                                             (1U)

#define DCAN_IFMCTL_INTPND_MASK                                                          (0x00002000U)
#define DCAN_IFMCTL_INTPND_SHIFT                                                         (0x0000000DU)
#define DCAN_IFMCTL_INTPND_INTERRUPT                                                     (0x1U)
#define DCAN_IFMCTL_INTPND_NOINTERRUPT                                                   (0x0U)

#define DCAN_IFMCTL_MSGLST_MASK                                                          (0x00004000U)
#define DCAN_IFMCTL_MSGLST_SHIFT                                                         (0x0000000EU)
#define DCAN_IFMCTL_MSGLST_MSGLOST                                                       (0x1U)
#define DCAN_IFMCTL_MSGLST_NOMSGLOST                                                     (0x0U)

#define DCAN_IFMCTL_NEWDAT_MASK                                                          (0x00008000U)
#define DCAN_IFMCTL_NEWDAT_SHIFT                                                         (0x0000000FU)
#define DCAN_IFMCTL_NEWDAT_NEWDATA                                                       (0x1U)
#define DCAN_IFMCTL_NEWDAT_NONEWDATA                                                     (0x0U)

#define DCAN_IFMCTL_RMTEN_MASK                                                           (0x00000200U)
#define DCAN_IFMCTL_RMTEN_SHIFT                                                          (0x00000009U)
#define DCAN_IFMCTL_RMTEN_DISABLE                                                        (0x0U)
#define DCAN_IFMCTL_RMTEN_ENABLE                                                         (0x1U)

#define DCAN_IFMCTL_RXIE_MASK                                                            (0x00000400U)
#define DCAN_IFMCTL_RXIE_SHIFT                                                           (0x0000000AU)
#define DCAN_IFMCTL_RXIE_NOTRIGGER                                                       (0x0U)
#define DCAN_IFMCTL_RXIE_TRIGGER                                                         (0x1U)

#define DCAN_IFMCTL_TXIE_MASK                                                            (0x00000800U)
#define DCAN_IFMCTL_TXIE_SHIFT                                                           (0x0000000BU)
#define DCAN_IFMCTL_TXIE_NOTRIGGER                                                       (0x0U)
#define DCAN_IFMCTL_TXIE_TRIGGER                                                         (0x1U)

#define DCAN_IFMCTL_TXRQST_MASK                                                          (0x00000100U)
#define DCAN_IFMCTL_TXRQST_SHIFT                                                         (0x00000008U)
#define DCAN_IFMCTL_TXRQST_NOREQUESTED                                                   (0x0U)
#define DCAN_IFMCTL_TXRQST_REQUESTED                                                     (0x1U)

#define DCAN_IFMCTL_UMASK_MASK                                                           (0x00001000U)
#define DCAN_IFMCTL_UMASK_SHIFT                                                          (0x0000000CU)
#define DCAN_IFMCTL_UMASK_IGNORED                                                        (0x0U)
#define DCAN_IFMCTL_UMASK_MASKED                                                         (0x1U)

/* IFxDATA mu */
#define DCAN_IFDATA_DCAN_IFDATA_MASK                                                     (0xFFFFFFFFU)
#define DCAN_IFDATA_DCAN_IFDATA_SHIFT                                                    (0x00000000U)


/* IFxDATB mu */
#define DCAN_IFDATB_DCAN_IFDATB_MASK                                                     (0xFFFFFFFFU)
#define DCAN_IFDATB_DCAN_IFDATB_SHIFT                                                    (0x00000000U)

#define DCAN_IF3OBS_MASK_SHIFT                                                          (0U)
#define DCAN_IF3OBS_MASK_MASK                                                           (0x00000001U)
#define DCAN_IF3OBS_MASK_EN_1_0X0                                                        (0U)
#define DCAN_IF3OBS_MASK_EN_2_0X1                                                        (1U)

#define DCAN_IF3OBS_ARB_SHIFT                                                           (1U)
#define DCAN_IF3OBS_ARB_MASK                                                            (0x00000002U)
#define DCAN_IF3OBS_ARB_EN_1_0X0                                                         (0U)
#define DCAN_IF3OBS_ARB_EN_2_0X1                                                         (1U)

#define DCAN_IF3OBS_CTRL_SHIFT                                                          (2U)
#define DCAN_IF3OBS_CTRL_MASK                                                           (0x00000004U)
#define DCAN_IF3OBS_CTRL_EN_1_0X0                                                        (0U)
#define DCAN_IF3OBS_CTRL_EN_2_0X1                                                        (1U)

#define DCAN_IF3OBS_DATAA_SHIFT                                                         (3U)
#define DCAN_IF3OBS_DATAA_MASK                                                          (0x00000008U)
#define DCAN_IF3OBS_DATAA_EN_1_0X0                                                       (0U)
#define DCAN_IF3OBS_DATAA_EN_2_0X1                                                       (1U)

#define DCAN_IF3OBS_DATAB_SHIFT                                                         (4U)
#define DCAN_IF3OBS_DATAB_MASK                                                          (0x00000010U)
#define DCAN_IF3OBS_DATAB_EN_1_0X0                                                       (0U)
#define DCAN_IF3OBS_DATAB_EN_2_0X1                                                       (1U)

#define DCAN_IF3OBS_IF3_SM_SHIFT                                                        (8U)
#define DCAN_IF3OBS_IF3_SM_MASK                                                         (0x00000100U)
#define DCAN_IF3OBS_IF3_SM_EN_1_0X0                                                      (0U)
#define DCAN_IF3OBS_IF3_SM_EN_2_0X1                                                      (1U)

#define DCAN_IF3OBS_IF3_SA_SHIFT                                                        (9U)
#define DCAN_IF3OBS_IF3_SA_MASK                                                         (0x00000200U)
#define DCAN_IF3OBS_IF3_SA_EN_1_0X0                                                      (0U)
#define DCAN_IF3OBS_IF3_SA_EN_2_0X1                                                      (1U)

#define DCAN_IF3OBS_IF3_SC_SHIFT                                                        (10U)
#define DCAN_IF3OBS_IF3_SC_MASK                                                         (0x00000400U)
#define DCAN_IF3OBS_IF3_SC_EN_1_0X0                                                      (0U)
#define DCAN_IF3OBS_IF3_SC_EN_2_0X1                                                      (1U)

#define DCAN_IF3OBS_IF3_SDA_SHIFT                                                       (11U)
#define DCAN_IF3OBS_IF3_SDA_MASK                                                        (0x00000800U)
#define DCAN_IF3OBS_IF3_SDA_EN_1_0X0                                                     (0U)
#define DCAN_IF3OBS_IF3_SDA_EN_2_0X1                                                     (1U)

#define DCAN_IF3OBS_IF3_SDB_SHIFT                                                       (12U)
#define DCAN_IF3OBS_IF3_SDB_MASK                                                        (0x00001000U)
#define DCAN_IF3OBS_IF3_SDB_EN_1_0X0                                                     (0U)
#define DCAN_IF3OBS_IF3_SDB_EN_2_0X1                                                     (1U)

#define DCAN_IF3OBS_IF3_UPD_SHIFT                                                       (15U)
#define DCAN_IF3OBS_IF3_UPD_MASK                                                        (0x00008000U)
#define DCAN_IF3OBS_IF3_UPD_EN_1_0X0                                                     (0U)
#define DCAN_IF3OBS_IF3_UPD_EN_2_0X1                                                     (1U)

#define DCAN_IF3UPD12_IF3UPDEN_0_SHIFT                                                  (0U)
#define DCAN_IF3UPD12_IF3UPDEN_0_MASK                                                   (0x0000ffffU)
#define DCAN_IF3UPD12_IF3UPDEN_0_EN_1_0X0                                                (0U)
#define DCAN_IF3UPD12_IF3UPDEN_0_EN_2_0X1                                                (1U)

#define DCAN_IF3UPD12_IF3UPDEN_1_SHIFT                                                  (16U)
#define DCAN_IF3UPD12_IF3UPDEN_1_MASK                                                   (0xffff0000U)
#define DCAN_IF3UPD12_IF3UPDEN_1_EN_0X0                                                  (0U)
#define DCAN_IF3UPD12_IF3UPDEN_1_EN_2_0X1                                                (1U)

#define DCAN_IF3UPD34_IF3UPDEN_0_SHIFT                                                  (0U)
#define DCAN_IF3UPD34_IF3UPDEN_0_MASK                                                   (0x0000ffffU)
#define DCAN_IF3UPD34_IF3UPDEN_0_EN_1_0X0                                                (0U)
#define DCAN_IF3UPD34_IF3UPDEN_0_EN_2_0X1                                                (1U)

#define DCAN_IF3UPD34_IF3UPDEN_1_SHIFT                                                  (16U)
#define DCAN_IF3UPD34_IF3UPDEN_1_MASK                                                   (0xffff0000U)
#define DCAN_IF3UPD34_IF3UPDEN_1_EN_0X0                                                  (0U)
#define DCAN_IF3UPD34_IF3UPDEN_1_EN_2_0X1                                                (1U)

#define DCAN_IF3UPD56_IF3UPDEN_0_SHIFT                                                  (0U)
#define DCAN_IF3UPD56_IF3UPDEN_0_MASK                                                   (0x0000ffffU)
#define DCAN_IF3UPD56_IF3UPDEN_0_EN_1_0X0                                                (0U)
#define DCAN_IF3UPD56_IF3UPDEN_0_EN_2_0X1                                                (1U)

#define DCAN_IF3UPD56_IF3UPDEN_1_SHIFT                                                  (16U)
#define DCAN_IF3UPD56_IF3UPDEN_1_MASK                                                   (0xffff0000U)
#define DCAN_IF3UPD56_IF3UPDEN_1_EN_0X0                                                  (0U)
#define DCAN_IF3UPD56_IF3UPDEN_1_EN_2_0X1                                                (1U)

#define DCAN_IF3UPD78_IF3UPDEN_0_SHIFT                                                  (0U)
#define DCAN_IF3UPD78_IF3UPDEN_0_MASK                                                   (0x0000ffffU)
#define DCAN_IF3UPD78_IF3UPDEN_0_EN_1_0X0                                                (0U)
#define DCAN_IF3UPD78_IF3UPDEN_0_EN_2_0X1                                                (1U)

#define DCAN_IF3UPD78_IF3UPDEN_1_SHIFT                                                  (16U)
#define DCAN_IF3UPD78_IF3UPDEN_1_MASK                                                   (0xffff0000U)
#define DCAN_IF3UPD78_IF3UPDEN_1_EN_0X0                                                  (0U)
#define DCAN_IF3UPD78_IF3UPDEN_1_EN_2_0X1                                                (1U)

#define DCAN_TIOC_IN_SHIFT                                                              (0U)
#define DCAN_TIOC_IN_MASK                                                               (0x00000001U)
#define DCAN_TIOC_IN_EN_1_0X0                                                            (0U)
#define DCAN_TIOC_IN_EN_2_0X1                                                            (1U)

#define DCAN_TIOC_OUT_SHIFT                                                             (1U)
#define DCAN_TIOC_OUT_MASK                                                              (0x00000002U)
#define DCAN_TIOC_OUT_EN_1_0X0                                                           (0U)
#define DCAN_TIOC_OUT_EN_2_0X1                                                           (1U)

#define DCAN_TIOC_DIR_SHIFT                                                             (2U)
#define DCAN_TIOC_DIR_MASK                                                              (0x00000004U)
#define DCAN_TIOC_DIR_EN_1_0X0                                                           (0U)
#define DCAN_TIOC_DIR_EN_2_0X1                                                           (1U)

#define DCAN_TIOC_FUNC_SHIFT                                                            (3U)
#define DCAN_TIOC_FUNC_MASK                                                             (0x00000008U)
#define DCAN_TIOC_FUNC_EN_1_0X0                                                          (0U)
#define DCAN_TIOC_FUNC_EN_2_0X1                                                          (1U)

#define DCAN_TIOC_OD_SHIFT                                                              (16U)
#define DCAN_TIOC_OD_MASK                                                               (0x00010000U)
#define DCAN_TIOC_OD_EN_1_0X0                                                            (0U)
#define DCAN_TIOC_OD_EN_2_0X1                                                            (1U)

#define DCAN_TIOC_PD_SHIFT                                                              (17U)
#define DCAN_TIOC_PD_MASK                                                               (0x00020000U)
#define DCAN_TIOC_PD_EN_1_0X0                                                            (0U)
#define DCAN_TIOC_PD_EN_2_0X1                                                            (1U)

#define DCAN_TIOC_PU_SHIFT                                                              (18U)
#define DCAN_TIOC_PU_MASK                                                               (0x00040000U)
#define DCAN_TIOC_PU_EN_1_0X0                                                            (0U)
#define DCAN_TIOC_PU_EN_2_0X1                                                            (1U)

#define DCAN_RIOC_IN_SHIFT                                                              (0U)
#define DCAN_RIOC_IN_MASK                                                               (0x00000001U)
#define DCAN_RIOC_IN_EN_1_0X0                                                            (0U)
#define DCAN_RIOC_IN_EN_2_0X1                                                            (1U)

#define DCAN_RIOC_OUT_SHIFT                                                             (1U)
#define DCAN_RIOC_OUT_MASK                                                              (0x00000002U)
#define DCAN_RIOC_OUT_EN_1_0X0                                                           (0U)
#define DCAN_RIOC_OUT_EN_2_0X1                                                           (1U)

#define DCAN_RIOC_DIR_SHIFT                                                             (2U)
#define DCAN_RIOC_DIR_MASK                                                              (0x00000004U)
#define DCAN_RIOC_DIR_EN_1_0X0                                                           (0U)
#define DCAN_RIOC_DIR_EN_2_0X1                                                           (1U)

#define DCAN_RIOC_FUNC_SHIFT                                                            (3U)
#define DCAN_RIOC_FUNC_MASK                                                             (0x00000008U)
#define DCAN_RIOC_FUNC_EN_1_0X0                                                          (0U)
#define DCAN_RIOC_FUNC_EN_2_0X1                                                          (1U)

#define DCAN_RIOC_OD_SHIFT                                                              (16U)
#define DCAN_RIOC_OD_MASK                                                               (0x00010000U)
#define DCAN_RIOC_OD_EN_1_0X0                                                            (0U)
#define DCAN_RIOC_OD_EN_2_0X1                                                            (1U)

#define DCAN_RIOC_PD_SHIFT                                                              (17U)
#define DCAN_RIOC_PD_MASK                                                               (0x00020000U)
#define DCAN_RIOC_PD_EN_1_0X0                                                            (0U)
#define DCAN_RIOC_PD_EN_2_0X1                                                            (1U)

#define DCAN_RIOC_PU_SHIFT                                                              (18U)
#define DCAN_RIOC_PU_MASK                                                               (0x00040000U)
#define DCAN_RIOC_PU_EN_1_0X0                                                            (0U)
#define DCAN_RIOC_PU_EN_2_0X1                                                            (1U)

#ifdef __cplusplus
}
#endif
#endif  /* HW_DCAN_H_ */

