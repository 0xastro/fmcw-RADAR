/**
 *  @file    reg_sci.h
 *
 *  @brief
 *    This file gives register definitions of MSS_SCIA module.
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

#ifndef REG_SCI_H
#define REG_SCI_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field RESET in Register SCIGCR0 */
#define SCIGCR0_RESET_BIT_START                             0U
#define SCIGCR0_RESET_BIT_END                               0U

/* Definition for field COMM_MODE in Register SCIGCR1 */
#define SCIGCR1_COMM_MODE_BIT_START                         0U
#define SCIGCR1_COMM_MODE_BIT_END                           0U

/* Definition for field TIMING_MODE in Register SCIGCR1 */
#define SCIGCR1_TIMING_MODE_BIT_START                       1U
#define SCIGCR1_TIMING_MODE_BIT_END                         1U

/* Definition for field PARITY_ENA in Register SCIGCR1 */
#define SCIGCR1_PARITY_ENA_BIT_START                        2U
#define SCIGCR1_PARITY_ENA_BIT_END                          2U

/* Definition for field PARITY in Register SCIGCR1 */
#define SCIGCR1_PARITY_BIT_START                            3U
#define SCIGCR1_PARITY_BIT_END                              3U

/* Definition for field STOP in Register SCIGCR1 */
#define SCIGCR1_STOP_BIT_START                              4U
#define SCIGCR1_STOP_BIT_END                                4U

/* Definition for field CLOCK in Register SCIGCR1 */
#define SCIGCR1_CLOCK_BIT_START                             5U
#define SCIGCR1_CLOCK_BIT_END                               5U

/* Definition for field SW_nRESET in Register SCIGCR1 */
#define SCIGCR1_SW_NRESET_BIT_START                         7U
#define SCIGCR1_SW_NRESET_BIT_END                           7U

/* Definition for field SLEEP in Register SCIGCR1 */
#define SCIGCR1_SLEEP_BIT_START                             8U
#define SCIGCR1_SLEEP_BIT_END                               8U

/* Definition for field POWERDOWN in Register SCIGCR1 */
#define SCIGCR1_POWERDOWN_BIT_START                         9U
#define SCIGCR1_POWERDOWN_BIT_END                           9U

/* Definition for field LOOP_BACK in Register SCIGCR1 */
#define SCIGCR1_LOOP_BACK_BIT_START                         16U
#define SCIGCR1_LOOP_BACK_BIT_END                           16U

/* Definition for field CONT in Register SCIGCR1 */
#define SCIGCR1_CONT_BIT_START                              17U
#define SCIGCR1_CONT_BIT_END                                17U

/* Definition for field RXENA in Register SCIGCR1 */
#define SCIGCR1_RXENA_BIT_START                             24U
#define SCIGCR1_RXENA_BIT_END                               24U

/* Definition for field TXENA in Register SCIGCR1 */
#define SCIGCR1_TXENA_BIT_START                             25U
#define SCIGCR1_TXENA_BIT_END                               25U

/* Definition for field SET_BRKDT_INT in Register SCISETINT */
#define SCISETINT_SET_BRKDT_INT_BIT_START                   0U
#define SCISETINT_SET_BRKDT_INT_BIT_END                     0U

/* Definition for field SET_WAKEUP_INT in Register SCISETINT */
#define SCISETINT_SET_WAKEUP_INT_BIT_START                  1U
#define SCISETINT_SET_WAKEUP_INT_BIT_END                    1U

/* Definition for field SET_TX_INT in Register SCISETINT */
#define SCISETINT_SET_TX_INT_BIT_START                      8U
#define SCISETINT_SET_TX_INT_BIT_END                        8U

/* Definition for field SET_RX_INT in Register SCISETINT */
#define SCISETINT_SET_RX_INT_BIT_START                      9U
#define SCISETINT_SET_RX_INT_BIT_END                        9U

/* Definition for field SET_TX_DMA in Register SCISETINT */
#define SCISETINT_SET_TX_DMA_BIT_START                      16U
#define SCISETINT_SET_TX_DMA_BIT_END                        16U

/* Definition for field SET_RX_DMA in Register SCISETINT */
#define SCISETINT_SET_RX_DMA_BIT_START                      17U
#define SCISETINT_SET_RX_DMA_BIT_END                        17U

/* Definition for field SET_RX_DMA_ALL in Register SCISETINT */
#define SCISETINT_SET_RX_DMA_ALL_BIT_START                  18U
#define SCISETINT_SET_RX_DMA_ALL_BIT_END                    18U

/* Definition for field SET_PE_INT in Register SCISETINT */
#define SCISETINT_SET_PE_INT_BIT_START                      24U
#define SCISETINT_SET_PE_INT_BIT_END                        24U

/* Definition for field SET_OE_INT in Register SCISETINT */
#define SCISETINT_SET_OE_INT_BIT_START                      25U
#define SCISETINT_SET_OE_INT_BIT_END                        25U

/* Definition for field SET_FE_INT in Register SCISETINT */
#define SCISETINT_SET_FE_INT_BIT_START                      26U
#define SCISETINT_SET_FE_INT_BIT_END                        26U

/* Definition for field CLR_BRKDT_INT in Register SCICLEARINT */
#define SCICLEARINT_CLR_BRKDT_INT_BIT_START                 0U
#define SCICLEARINT_CLR_BRKDT_INT_BIT_END                   0U

/* Definition for field CLR_WAKEUP_INT in Register SCICLEARINT */
#define SCICLEARINT_CLR_WAKEUP_INT_BIT_START                1U
#define SCICLEARINT_CLR_WAKEUP_INT_BIT_END                  1U

/* Definition for field CLR_TX_INT in Register SCICLEARINT */
#define SCICLEARINT_CLR_TX_INT_BIT_START                    8U
#define SCICLEARINT_CLR_TX_INT_BIT_END                      8U

/* Definition for field CLR_RX_INT in Register SCICLEARINT */
#define SCICLEARINT_CLR_RX_INT_BIT_START                    9U
#define SCICLEARINT_CLR_RX_INT_BIT_END                      9U

/* Definition for field CLR_TX_DMA in Register SCICLEARINT */
#define SCICLEARINT_CLR_TX_DMA_BIT_START                    16U
#define SCICLEARINT_CLR_TX_DMA_BIT_END                      16U

/* Definition for field CLR_RX_DMA in Register SCICLEARINT */
#define SCICLEARINT_CLR_RX_DMA_BIT_START                    17U
#define SCICLEARINT_CLR_RX_DMA_BIT_END                      17U

/* Definition for field CLR_RX_DMA_ALL in Register SCICLEARINT */
#define SCICLEARINT_CLR_RX_DMA_ALL_BIT_START                18U
#define SCICLEARINT_CLR_RX_DMA_ALL_BIT_END                  18U

/* Definition for field CLR_PE_INT in Register SCICLEARINT */
#define SCICLEARINT_CLR_PE_INT_BIT_START                    24U
#define SCICLEARINT_CLR_PE_INT_BIT_END                      24U

/* Definition for field CLR_OE_INT in Register SCICLEARINT */
#define SCICLEARINT_CLR_OE_INT_BIT_START                    25U
#define SCICLEARINT_CLR_OE_INT_BIT_END                      25U

/* Definition for field CLR_FE_INT in Register SCICLEARINT */
#define SCICLEARINT_CLR_FE_INT_BIT_START                    26U
#define SCICLEARINT_CLR_FE_INT_BIT_END                      26U

/* Definition for field SET_BRKDT_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_BRKDT_INT_LVL_BIT_START            0U
#define SCISETINTLVL_SET_BRKDT_INT_LVL_BIT_END              0U

/* Definition for field SET_WAKEUP_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_WAKEUP_INT_LVL_BIT_START           1U
#define SCISETINTLVL_SET_WAKEUP_INT_LVL_BIT_END             1U

/* Definition for field SET_TX_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_TX_INT_LVL_BIT_START               8U
#define SCISETINTLVL_SET_TX_INT_LVL_BIT_END                 8U

/* Definition for field SET_RX_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_RX_INT_LVL_BIT_START               9U
#define SCISETINTLVL_SET_RX_INT_LVL_BIT_END                 9U

/* Definition for field SET_INC_BR_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_INC_BR_INT_LVL_BIT_START           15U
#define SCISETINTLVL_SET_INC_BR_INT_LVL_BIT_END             15U

/* Definition for field SET_RX_DMA_ALL_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_RX_DMA_ALL_INT_LVL_BIT_START       18U
#define SCISETINTLVL_SET_RX_DMA_ALL_INT_LVL_BIT_END         18U

/* Definition for field SET_PE_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_PE_INT_LVL_BIT_START               24U
#define SCISETINTLVL_SET_PE_INT_LVL_BIT_END                 24U

/* Definition for field SET_OE_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_OE_INT_LVL_BIT_START               25U
#define SCISETINTLVL_SET_OE_INT_LVL_BIT_END                 25U

/* Definition for field SET_FE_INT_LVL in Register SCISETINTLVL */
#define SCISETINTLVL_SET_FE_INT_LVL_BIT_START               26U
#define SCISETINTLVL_SET_FE_INT_LVL_BIT_END                 26U

/* Definition for field CLR_BRKDT_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_BRKDT_INT_LVL_BIT_START          0U
#define SCICLEARINTLVL_CLR_BRKDT_INT_LVL_BIT_END            0U

/* Definition for field CLR_WAKEUP_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_WAKEUP_INT_LVL_BIT_START         1U
#define SCICLEARINTLVL_CLR_WAKEUP_INT_LVL_BIT_END           1U

/* Definition for field CLR_TX_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_TX_INT_LVL_BIT_START             8U
#define SCICLEARINTLVL_CLR_TX_INT_LVL_BIT_END               8U

/* Definition for field CLR_RX_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_RX_INT_LVL_BIT_START             9U
#define SCICLEARINTLVL_CLR_RX_INT_LVL_BIT_END               9U

/* Definition for field CLR_INC_BR_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_INC_BR_INT_LVL_BIT_START         15U
#define SCICLEARINTLVL_CLR_INC_BR_INT_LVL_BIT_END           15U

/* Definition for field CLR_RX_DMA_ALL_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_RX_DMA_ALL_INT_LVL_BIT_START     18U
#define SCICLEARINTLVL_CLR_RX_DMA_ALL_INT_LVL_BIT_END       18U

/* Definition for field CLR_PE_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_PE_INT_LVL_BIT_START             24U
#define SCICLEARINTLVL_CLR_PE_INT_LVL_BIT_END               24U

/* Definition for field CLR_OE_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_OE_INT_LVL_BIT_START             25U
#define SCICLEARINTLVL_CLR_OE_INT_LVL_BIT_END               25U

/* Definition for field CLR_FE_INT_LVL in Register SCICLEARINTLVL */
#define SCICLEARINTLVL_CLR_FE_INT_LVL_BIT_START             26U
#define SCICLEARINTLVL_CLR_FE_INT_LVL_BIT_END               26U

/* Definition for field BRKDT in Register SCIFLR */
#define SCIFLR_BRKDT_BIT_START                              0U
#define SCIFLR_BRKDT_BIT_END                                0U

/* Definition for field WAKEUP in Register SCIFLR */
#define SCIFLR_WAKEUP_BIT_START                             1U
#define SCIFLR_WAKEUP_BIT_END                               1U

/* Definition for field IDLE in Register SCIFLR */
#define SCIFLR_IDLE_BIT_START                               2U
#define SCIFLR_IDLE_BIT_END                                 2U

/* Definition for field Bus_busy_flag in Register SCIFLR */
#define SCIFLR_BUS_BUSY_FLAG_BIT_START                      3U
#define SCIFLR_BUS_BUSY_FLAG_BIT_END                        3U

/* Definition for field TXRDY in Register SCIFLR */
#define SCIFLR_TXRDY_BIT_START                              8U
#define SCIFLR_TXRDY_BIT_END                                8U

/* Definition for field RXRDY in Register SCIFLR */
#define SCIFLR_RXRDY_BIT_START                              9U
#define SCIFLR_RXRDY_BIT_END                                9U

/* Definition for field TXWAKE in Register SCIFLR */
#define SCIFLR_TXWAKE_BIT_START                             10U
#define SCIFLR_TXWAKE_BIT_END                               10U

/* Definition for field TX_EMPTY in Register SCIFLR */
#define SCIFLR_TX_EMPTY_BIT_START                           11U
#define SCIFLR_TX_EMPTY_BIT_END                             11U

/* Definition for field RXWAKE in Register SCIFLR */
#define SCIFLR_RXWAKE_BIT_START                             12U
#define SCIFLR_RXWAKE_BIT_END                               12U

/* Definition for field PE in Register SCIFLR */
#define SCIFLR_PE_BIT_START                                 24U
#define SCIFLR_PE_BIT_END                                   24U

/* Definition for field OE in Register SCIFLR */
#define SCIFLR_OE_BIT_START                                 25U
#define SCIFLR_OE_BIT_END                                   25U

/* Definition for field FE in Register SCIFLR */
#define SCIFLR_FE_BIT_START                                 26U
#define SCIFLR_FE_BIT_END                                   26U

/* Definition for field INTVECT0 in Register SCIINTVECT0 */
#define SCIINTVECT0_INTVECT0_BIT_START                      0U
#define SCIINTVECT0_INTVECT0_BIT_END                        3U

/* Definition for field INTVECT1 in Register SCIINTVECT1 */
#define SCIINTVECT1_INTVECT1_BIT_START                      0U
#define SCIINTVECT1_INTVECT1_BIT_END                        3U

/* Definition for field CHAR in Register SCICHAR */
#define SCICHAR_CHAR_BIT_START                              0U
#define SCICHAR_CHAR_BIT_END                                2U

/* Definition for field BAUD in Register SCIBAUD */
#define SCIBAUD_BAUD_BIT_START                              0U
#define SCIBAUD_BAUD_BIT_END                                23U

/* Definition for field ED in Register SCIED */
#define SCIED_ED_BIT_START                                  0U
#define SCIED_ED_BIT_END                                    7U

/* Definition for field RD in Register SCIRD */
#define SCIRD_RD_BIT_START                                  0U
#define SCIRD_RD_BIT_END                                    7U

/* Definition for field TD in Register SCITD */
#define SCITD_TD_BIT_START                                  0U
#define SCITD_TD_BIT_END                                    7U

/* Definition for field CLK_FUNC in Register SCIPIO0 */
#define SCIPIO0_CLK_FUNC_BIT_START                          0U
#define SCIPIO0_CLK_FUNC_BIT_END                            0U

/* Definition for field RX_FUNC in Register SCIPIO0 */
#define SCIPIO0_RX_FUNC_BIT_START                           1U
#define SCIPIO0_RX_FUNC_BIT_END                             1U

/* Definition for field TX_FUNC in Register SCIPIO0 */
#define SCIPIO0_TX_FUNC_BIT_START                           2U
#define SCIPIO0_TX_FUNC_BIT_END                             2U

/* Definition for field CLK_DIR in Register SCIPIO1 */
#define SCIPIO1_CLK_DIR_BIT_START                           0U
#define SCIPIO1_CLK_DIR_BIT_END                             0U

/* Definition for field RX_DIR in Register SCIPIO1 */
#define SCIPIO1_RX_DIR_BIT_START                            1U
#define SCIPIO1_RX_DIR_BIT_END                              1U

/* Definition for field TX_DIR in Register SCIPIO1 */
#define SCIPIO1_TX_DIR_BIT_START                            2U
#define SCIPIO1_TX_DIR_BIT_END                              2U

/* Definition for field CLK_DATA_IN in Register SCIPIO2 */
#define SCIPIO2_CLK_DATA_IN_BIT_START                       0U
#define SCIPIO2_CLK_DATA_IN_BIT_END                         0U

/* Definition for field RX_DATA_IN in Register SCIPIO2 */
#define SCIPIO2_RX_DATA_IN_BIT_START                        1U
#define SCIPIO2_RX_DATA_IN_BIT_END                          1U

/* Definition for field TX_DATA_IN in Register SCIPIO2 */
#define SCIPIO2_TX_DATA_IN_BIT_START                        2U
#define SCIPIO2_TX_DATA_IN_BIT_END                          2U

/* Definition for field CLK_DATA_OUT in Register SCIPIO3 */
#define SCIPIO3_CLK_DATA_OUT_BIT_START                      0U
#define SCIPIO3_CLK_DATA_OUT_BIT_END                        0U

/* Definition for field RX_DATA_OUT in Register SCIPIO3 */
#define SCIPIO3_RX_DATA_OUT_BIT_START                       1U
#define SCIPIO3_RX_DATA_OUT_BIT_END                         1U

/* Definition for field TX_DATA_OUT in Register SCIPIO3 */
#define SCIPIO3_TX_DATA_OUT_BIT_START                       2U
#define SCIPIO3_TX_DATA_OUT_BIT_END                         2U

/* Definition for field CLK_DATA_SET in Register SCIPIO4 */
#define SCIPIO4_CLK_DATA_SET_BIT_START                      0U
#define SCIPIO4_CLK_DATA_SET_BIT_END                        0U

/* Definition for field RX_DATA_SET in Register SCIPIO4 */
#define SCIPIO4_RX_DATA_SET_BIT_START                       1U
#define SCIPIO4_RX_DATA_SET_BIT_END                         1U

/* Definition for field TX_DATA_SET in Register SCIPIO4 */
#define SCIPIO4_TX_DATA_SET_BIT_START                       2U
#define SCIPIO4_TX_DATA_SET_BIT_END                         2U

/* Definition for field CLK_DATA_CLR in Register SCIPIO5 */
#define SCIPIO5_CLK_DATA_CLR_BIT_START                      0U
#define SCIPIO5_CLK_DATA_CLR_BIT_END                        0U

/* Definition for field RX_DATA_CLR in Register SCIPIO5 */
#define SCIPIO5_RX_DATA_CLR_BIT_START                       1U
#define SCIPIO5_RX_DATA_CLR_BIT_END                         1U

/* Definition for field TX_DATA_CLR in Register SCIPIO5 */
#define SCIPIO5_TX_DATA_CLR_BIT_START                       2U
#define SCIPIO5_TX_DATA_CLR_BIT_END                         2U

/* Definition for field CLK_PDR in Register SCIPIO6 */
#define SCIPIO6_CLK_PDR_BIT_START                           0U
#define SCIPIO6_CLK_PDR_BIT_END                             0U

/* Definition for field RX_PDR in Register SCIPIO6 */
#define SCIPIO6_RX_PDR_BIT_START                            1U
#define SCIPIO6_RX_PDR_BIT_END                              1U

/* Definition for field TX_PDR in Register SCIPIO6 */
#define SCIPIO6_TX_PDR_BIT_START                            2U
#define SCIPIO6_TX_PDR_BIT_END                              2U

/* Definition for field CLK_PD in Register SCIPIO7 */
#define SCIPIO7_CLK_PD_BIT_START                            0U
#define SCIPIO7_CLK_PD_BIT_END                              0U

/* Definition for field RX_PD in Register SCIPIO7 */
#define SCIPIO7_RX_PD_BIT_START                             1U
#define SCIPIO7_RX_PD_BIT_END                               1U

/* Definition for field TX_PD in Register SCIPIO7 */
#define SCIPIO7_TX_PD_BIT_START                             2U
#define SCIPIO7_TX_PD_BIT_END                               2U

/* Definition for field CLK_PSL in Register SCIPIO8 */
#define SCIPIO8_CLK_PSL_BIT_START                           0U
#define SCIPIO8_CLK_PSL_BIT_END                             0U

/* Definition for field RX_PSL in Register SCIPIO8 */
#define SCIPIO8_RX_PSL_BIT_START                            1U
#define SCIPIO8_RX_PSL_BIT_END                              1U

/* Definition for field TX_PSL in Register SCIPIO8 */
#define SCIPIO8_TX_PSL_BIT_START                            2U
#define SCIPIO8_TX_PSL_BIT_END                              2U

/* Definition for field CLK_SL in Register SCIPIO9 */
#define SCIPIO9_CLK_SL_BIT_START                            0U
#define SCIPIO9_CLK_SL_BIT_END                              0U

/* Definition for field RX_SL in Register SCIPIO9 */
#define SCIPIO9_RX_SL_BIT_START                             1U
#define SCIPIO9_RX_SL_BIT_END                               1U

/* Definition for field TX_SL in Register SCIPIO9 */
#define SCIPIO9_TX_SL_BIT_START                             2U
#define SCIPIO9_TX_SL_BIT_END                               2U

/* Definition for field RXP_ENA in Register SCIIODCTRL */
#define SCIIODCTRL_RXP_ENA_BIT_START                        0U
#define SCIIODCTRL_RXP_ENA_BIT_END                          0U

/* Definition for field LBP_ENA in Register SCIIODCTRL */
#define SCIIODCTRL_LBP_ENA_BIT_START                        1U
#define SCIIODCTRL_LBP_ENA_BIT_END                          1U

/* Definition for field IODFTENA in Register SCIIODCTRL */
#define SCIIODCTRL_IODFTENA_BIT_START                       8U
#define SCIIODCTRL_IODFTENA_BIT_END                         11U

/* Definition for field TX_SHIFT in Register SCIIODCTRL */
#define SCIIODCTRL_TX_SHIFT_BIT_START                       16U
#define SCIIODCTRL_TX_SHIFT_BIT_END                         18U

/* Definition for field PIN_SAMPLE_MASK in Register SCIIODCTRL */
#define SCIIODCTRL_PIN_SAMPLE_MASK_BIT_START                19U
#define SCIIODCTRL_PIN_SAMPLE_MASK_BIT_END                  20U

/* Definition for field BRKDT_ENA in Register SCIIODCTRL */
#define SCIIODCTRL_BRKDT_ENA_BIT_START                      24U
#define SCIIODCTRL_BRKDT_ENA_BIT_END                        24U

/* Definition for field PEN in Register SCIIODCTRL */
#define SCIIODCTRL_PEN_BIT_START                            25U
#define SCIIODCTRL_PEN_BIT_END                              25U

/* Definition for field FEN in Register SCIIODCTRL */
#define SCIIODCTRL_FEN_BIT_START                            26U
#define SCIIODCTRL_FEN_BIT_END                              26U



/**
 * @struct SCIRegs_t
 * @brief
 *   Module MSS_SCIA Register Definition
 * @details
 *   This structure is used to access the MSS_SCIA module registers.
 */
/**
 * @typedef SCIRegs
 * @brief
 *   Module MSS_SCIA Register Frame type Definition
 * @details
 *   This type is used to access the MSS_SCIA module registers.
 */
typedef volatile struct SCIRegs_t
{
    uint32_t    SCIGCR0                            ;        /* Offset = 0x000 */
    uint32_t    SCIGCR1                            ;        /* Offset = 0x004 */
    uint32_t    RESERVED1                          ;        /* Offset = 0x008 */
    uint32_t    SCISETINT                          ;        /* Offset = 0x00C */
    uint32_t    SCICLEARINT                        ;        /* Offset = 0x010 */
    uint32_t    SCISETINTLVL                       ;        /* Offset = 0x014 */
    uint32_t    SCICLEARINTLVL                     ;        /* Offset = 0x018 */
    uint32_t    SCIFLR                             ;        /* Offset = 0x01C */
    uint32_t    SCIINTVECT0                        ;        /* Offset = 0x020 */
    uint32_t    SCIINTVECT1                        ;        /* Offset = 0x024 */
    uint32_t    SCICHAR                            ;        /* Offset = 0x028 */
    uint32_t    SCIBAUD                            ;        /* Offset = 0x02C */
    uint32_t    SCIED                              ;        /* Offset = 0x030 */
    uint32_t    SCIRD                              ;        /* Offset = 0x034 */
    uint32_t    SCITD                              ;        /* Offset = 0x038 */
    uint32_t    SCIPIO0                            ;        /* Offset = 0x03C */
    uint32_t    SCIPIO1                            ;        /* Offset = 0x040 */
    uint32_t    SCIPIO2                            ;        /* Offset = 0x044 */
    uint32_t    SCIPIO3                            ;        /* Offset = 0x048 */
    uint32_t    SCIPIO4                            ;        /* Offset = 0x04C */
    uint32_t    SCIPIO5                            ;        /* Offset = 0x050 */
    uint32_t    SCIPIO6                            ;        /* Offset = 0x054 */
    uint32_t    SCIPIO7                            ;        /* Offset = 0x058 */
    uint32_t    SCIPIO8                            ;        /* Offset = 0x05C */
    uint32_t    RESERVED2                          ;        /* Offset = 0x060 */
    uint32_t    RESERVED3                          ;        /* Offset = 0x064 */
    uint32_t    RESERVED4                          ;        /* Offset = 0x068 */
    uint32_t    RESERVED5                          ;        /* Offset = 0x06C */
    uint32_t    RESERVED6                          ;        /* Offset = 0x070 */
    uint32_t    RESERVED7                          ;        /* Offset = 0x074 */
    uint32_t    RESERVED8                          ;        /* Offset = 0x078 */
    uint32_t    RESERVED9                          ;        /* Offset = 0x07C */
    uint32_t    SCIPIO9                            ;        /* Offset = 0x080 */
    uint32_t    RESTRICTED1[3]                     ;        /* Offset = 0x084 */
    uint32_t    SCIIODCTRL                         ;        /* Offset = 0x090 */
} SCIRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_SCI_H */
/* END OF REG_SCI_H */

