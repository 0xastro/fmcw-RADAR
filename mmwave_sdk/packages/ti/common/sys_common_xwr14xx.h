/**
 *   @file  sys_common_xwr14xx.h
 *
 *   @brief
 *      This is the common header file used by the various mmWave SDK
 *      modules for XWR14xx device.
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

#ifndef SYS_COMMON_XWR14XX_H
#define SYS_COMMON_XWR14XX_H

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Ensures that data transfer has finished. */
static inline void MEM_BARRIER(void)
{
    asm(" dsb ");
}

/*
 * DMA Request Line Mapping
 */
#define SOC_XWR14XX_MSS_SPIA_CH1_DMA_REQ     (0U)    /* MIBSPI/SPI1 (MIBSPI-A) Channel-1 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH0_DMA_REQ     (1U)    /* MIBSPI/SPI1 (MIBSPI-A) Channel-0 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIB_RX_DMA_REQ      (2U)    /* SPI2 (SPI-B) Receive DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIB_TX_DMA_REQ      (3U)    /* SPI2 (SPI-B) Transmit DMA Request Line*/
#define SOC_XWR14XX_MSS_QSPI_DMA_REQ         (4U)    /* QSPI DMA request DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH3_DMA_REQ     (5U)    /* MIBSPI/SPI1 (MIBSPI-A) Channel-3 DMA Request Line*/
#define SOC_XWR14XX_MSS_DCAN_IF2_DMA_REQ     (6U)    /* CAN (DCAN) IF2 DMA Request Line*/
#define SOC_XWR14XX_MSS_CBUFF_DMA_REQ        (7U)    /* Common Buffer DMA Request Line*/
#define SOC_XWR14XX_MSS_DCAN_IF1_DMA_REQ     (8U)    /* CAN (DCAN) IF1 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH5_DMA_REQ     (9U)    /* MIBSPI/SPI1 (MIBSPI-A) Channel-5 DMA Request Line*/
#define SOC_XWR14XX_MSS_I2C_RX_DMA_REQ       (10U)   /* I2C receive DMA Request Line*/
#define SOC_XWR14XX_MSS_I2C_TX_DMA_REQ       (11U)   /* I2C Transmit DMA Request Line*/
#define SOC_XWR14XX_MSS_RTI_COM0_DMA_REQ     (12U)   /* RTI1 DMAREQ0 DMA Request Line*/
#define SOC_XWR14XX_MSS_RTI_COM1_DMA_REQ     (13U)   /* RTI1 DMAREQ1 DMA Request Line*/
#define SOC_XWR14XX_MSS_DCAN_IF3_DMA_REQ     (16U)   /* CAN (DCAN) IF3 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH9_DMA_REQ     (17U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-9 DMA Request Line*/
#define SOC_XWR14XX_MSS_RTI_COM2_DMA_REQ     (18U)   /* RTI1 DMAREQ2 DMA Request Line*/
#define SOC_XWR14XX_MSS_RTI_COM3_DMA_REQ     (19U)   /* RTI1 DMAREQ3 DMA Request Line*/
#define SOC_XWR14XX_MSS_WDT_DMA_REQ0         (20U)   /* WDT/RTI2 DMAREQ0 DMA Request Line*/
#define SOC_XWR14XX_MSS_WDT_DMA_REQ1         (21U)   /* WDT/RTI2 DMAREQ1 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH10_DMA_REQ    (22U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-10 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH11_DMA_REQ    (23U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-11 DMA Request Line*/
#define SOC_XWR14XX_MSS_WDT_DMA_REQ2         (24U)   /* WDT/RTI2 DMAREQ2 DMA Request Line*/
#define SOC_XWR14XX_MSS_WDT_DMA_REQ3         (25U)   /* WDT/RTI2 DMAREQ3 DMA Request Line*/
#define SOC_XWR14XX_MSS_CRC_CH1_DMA_REQ      (26U)   /* CRC DMAREQ0 DMA Request Line*/
#define SOC_XWR14XX_MSS_CRC_CH2_DMA_REQ      (27U)   /* CRC DMAREQ1 DMA Request Line*/
#define SOC_XWR14XX_MSS_SCIB_RX_DMA_REQ      (28U)   /* UART2 receive DMA Request Line*/
#define SOC_XWR14XX_MSS_SCIB_TX_DMA_REQ      (29U)   /* UART2 Transmit DMA Request Line*/
#define SOC_XWR14XX_MSS_SCIA_RX_DMA_REQ      (30U)   /* UART1 receive DMA Request Line*/
#define SOC_XWR14XX_MSS_SCIA_TX_DMA_REQ      (31U)   /* UART1 Transmit DMA Request Line*/
#define SOC_XWR14XX_MSS_GIO0_DMA_REQ         (32U)   /* GIO-0 DMA Request Line*/
#define SOC_XWR14XX_MSS_GIO1_DMA_REQ         (33U)   /* GIO-1 DMA Request Line*/
#define SOC_XWR14XX_MSS_GIO2_DMA_REQ         (34U)   /* GIO-2 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH6_DMA_REQ     (36U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-6 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH7_DMA_REQ     (37U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-7 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH8_DMA_REQ     (38U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-8 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH12_DMA_REQ    (42U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-12 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH13_DMA_REQ    (43U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-13 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH14_DMA_REQ    (44U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-14 DMA Request Line*/
#define SOC_XWR14XX_MSS_SPIA_CH15_DMA_REQ    (45U)   /* MIBSPI/SPI1 (MIBSPI-A) Channel-15 DMA Request Line*/
#define SOC_XWR14XX_MSS_GIO14_DMA_REQ        (46U)   /* GIO-14 DMA Request Line*/
#define SOC_XWR14XX_MSS_GIO15_DMA_REQ        (47U)   /* GIO-15 DMA Request Line*/


#define SOC_XWR14XX_NUM_DMA_CHANNELS_PER_INSTANCE    (32U)  /* Number of DMA channels per DMA instance              */
#define SOC_XWR14XX_NUM_DMA_REQLINES_PER_INSTANCE    (48U)  /* Number of DMA REQ Lines per DMA instance             */


/*
 * MSS VIM Priority Macro List
 */
#define SOC_XWR14XX_MSS_VIM_PRIORITY_2           (2U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_3           (3U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_4           (4U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_5           (5U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_6           (6U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_7           (7U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_8           (8U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_9           (9U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_10          (10U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_11          (11U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_12          (12U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_13          (13U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_14          (14U)
#define SOC_XWR14XX_MSS_VIM_PRIORITY_15          (15U)


/*
 * MSS VIM Interrupt Mapping
 */
#define SOC_XWR14XX_MSS_ESM_HIGH_PRIORITY_INT      (0U)      /* VIM Line for ESM High Priority Interrupt         */
#define SOC_XWR14XX_MSS_RTI_COMPARE0_INT           (2U)      /* RTI Interrupt 0                                  */
#define SOC_XWR14XX_MSS_RTI_COMPARE1_INT           (3U)      /* RTI Interrupt 1                                  */
#define SOC_XWR14XX_MSS_RTI_COMPARE2_INT           (4U)      /* RTI Interrupt 2                                  */
#define SOC_XWR14XX_MSS_RTI_COMPARE3_INT           (5U)      /* RTI Interrupt 3                                  */
#define SOC_XWR14XX_MSS_RTI_OVRFLW0_INT            (6U)      /* RTI OverFlow Interrupt 0                         */
#define SOC_XWR14XX_MSS_RTI_OVRFLW1_INT            (7U)      /* RTI OverFlow Interrupt 1                         */
#define SOC_XWR14XX_MSS_RTI_TIMEBASE_INT           (8U)      /* Timebase Interrupt Channel                       */
#define SOC_XWR14XX_MSS_GIO_LVL0_INT               (9U)      /* GIO high level Interrupt                         */
#define SOC_XWR14XX_MSS_WDT_REQ0_INT               (10U)     /* WDT Interrupt 0                                  */
#define SOC_XWR14XX_MSS_WDT_REQ1_INT               (11U)     /* WDT Interrupt 1                                  */
#define SOC_XWR14XX_MSS_SPIA_LVL0_INT              (12U)     /* SPIA Interrupt Level 0                           */
#define SOC_XWR14XX_MSS_WDT_REQ2_INT               (13U)     /* WDT Interrupt 2                                  */
#define SOC_XWR14XX_MSS_WDT_REQ3_INT               (14U)     /* WDT Interrupt 3                                  */
#define SOC_XWR14XX_MSS_WDT_OVRFLW0_INT            (15U)     /* WDT OverFlow Interrupt 0                         */
#define SOC_XWR14XX_MSS_DCAN_LVL0_INT              (16U)     /* CAN Interrupt Level 0                            */
#define SOC_XWR14XX_MSS_SPIB_LVL0_INT              (17U)     /* SPIB Interrupt Level 0                           */
#define SOC_XWR14XX_MSS_MCRC_REQ_INT               (19U)     /* MCRC Interrupt                                   */
#define SOC_XWR14XX_MSS_ESM_LOW_PRIORITY_INT       (20U)     /* VIM Line for ESM Low Priority Interrupt          */
#define SOC_XWR14XX_MSS_SYS_SW4_INT                (21U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR14XX_MSS_PMU_IRQ_INT                (22U)     /* ~nPMUIRQ - PMU IRQ interrupt                     */
#define SOC_XWR14XX_MSS_GIO_LVL1_INT               (23U)     /* GIO Low Level Interrupt                          */
#define SOC_XWR14XX_MSS_WDT_OVRFLW1_INT            (24U)     /* WDT OverFlow Interrupt 1                         */
#define SOC_XWR14XX_MSS_WDT_TB_INT                 (25U)     /* WDT TB Interrupt Req                             */
#define SOC_XWR14XX_MSS_SPIA_LVL1_INT              (26U)     /* SPIA Interrupt Level 0                           */
#define SOC_XWR14XX_MSS_QSPI_INT_REQ               (27U)     /* QSPI Interrupt                                   */
#define SOC_XWR14XX_MSS_DCAN_LVL1_INT              (29U)     /* CAN Interrupt Level 1                            */
#define SOC_XWR14XX_MSS_SPIB_LVL1_INT              (30U)     /* SPIB Interrupt Level 1                           */
#define SOC_XWR14XX_MSS_DMA_FTC_INT                (33U)     /* DMA FTC (Frame Transfer Complete) Interrupt      */
#define SOC_XWR14XX_MSS_DMA_LFS_INT                (34U)     /* DMA LFS (Last Frame Transfer Started) Interrupt  */
#define SOC_XWR14XX_MSS_DMA_HBC_INT                (39U)     /* DMA HBC (First Half of Block Complete) Interrupt */
#define SOC_XWR14XX_MSS_DMA_BTC_INT                (40U)     /* DMA BTC (Block Transfer Complete) Interrupt      */
#define SOC_XWR14XX_MSS_FPU_INT                    (47U)     /* Floating point unit Interrupt                    */
#define SOC_XWR14XX_MSS_SCIA_LVL0_INT              (64U)     /* UARTA Interrupt Level 0                          */
#define SOC_XWR14XX_MSS_SCIB_LVL0_INT              (65U)     /* UARTB Interrupt Level 0                          */
#define SOC_XWR14XX_MSS_I2C_LVL0_INT               (66U)     /* I2C Level 0 Interrupt                            */
#define SOC_XWR14XX_MSS_DMA_BER_INT                (70U)     /* DMA Bus error Interrupt                          */
#define SOC_XWR14XX_MSS_SCIA_LVL1_INT              (74U)     /* UARTA Interrupt Level 1                          */
#define SOC_XWR14XX_MSS_SCIB_LVL1_INT              (75U)     /* UARTB Interrupt Level 1                          */
#define SOC_XWR14XX_MSS_SYS_SW0_INT                (76U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR14XX_MSS_SYS_SW1_INT                (77U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR14XX_MSS_SYS_SW2_INT                (78U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR14XX_MSS_SYS_SW3_INT                (79U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR14XX_MSS_CCCA_DONE_INT              (80U)     /* CCC-A Done Interrupt                             */
#define SOC_XWR14XX_MSS_CCCB_DONE_INT              (81U)     /* CCC-B Done Interrupt                             */
#define SOC_XWR14XX_MSS_DCCA_DONE_INT              (82U)     /* DCC-A Done Interrupt                             */
#define SOC_XWR14XX_MSS_DCCB_DONE_INT              (83U)     /* DCC-B Done Interrupt                             */
#define SOC_XWR14XX_MSS_SYS_SW5_INT                (84U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR14XX_MSS_PBIST_IRQ_INT              (85U)     /* PBIST IRQ Interrupt                              */
#define SOC_XWR14XX_MSS_SW_BSS_MSS_IRQ0_INT        (95U)     /* software Interrupt request-0 from BSS to MSS     */
#define SOC_XWR14XX_MSS_SW_BSS_MSS_IRQ1_INT        (96U)     /* software Interrupt request-1 from BSS to MSS     */
#define SOC_XWR14XX_BSS_ADC_VALID_INT              (97U)     /* Falling edge of ADC Valid Interrupt              */
#define SOC_XWR14XX_BSS_FRAME_START_INT            (98U)     /* Frame Start Interrupt to MSS                     */
#define SOC_XWR14XX_BSS_CHIRP_START_INT            (99U)     /* BSS Chirp start Interrupt                        */
#define SOC_XWR14XX_BSS_CHIRP_END_INT              (100U)    /* BSS Chirp end Interrupt                          */
#define SOC_XWR14XX_BSS_FRAME_END_INT              (101U)    /* BSS Frame end Interrupt                          */
#define SOC_XWR14XX_BSS_STC_DONE_INT               (105U)    /* BSS STC done Interrupt                           */
#define SOC_XWR14XX_DSS_TPTC0_IRQ_DONE             (112U)    /* EDMA TPTC0 Done Interrupt                        */
#define SOC_XWR14XX_DSS_TPTC0_IRQ_ERR              (113U)    /* EDMA TPTC0 Error Interrupt                       */
#define SOC_XWR14XX_DSS_TPTC1_IRQ_DONE             (114U)    /* EDMA TPTC1 Done Interrupt                        */
#define SOC_XWR14XX_DSS_TPTC1_IRQ_ERR              (115U)    /* EDMA TPTC1 Error Interrupt                       */
#define SOC_XWR14XX_DSS_TPCC_IRQ_DONE              (116U)    /* EDMA TPCC Done Interrupt                         */
#define SOC_XWR14XX_DSS_TPCC_IRQ_ERR               (117U)    /* EDMA TPCC Error Interrupt                        */
#define SOC_XWR14XX_DSS_CBUFF_IRQ                  (118U)    /* CBUFF Interrupt                                  */
#define SOC_XWR14XX_DSS_CSI_IRQ                    (119U)    /* CSI Interrupt                                    */
#define SOC_XWR14XX_DSS_CBUFF_ERR_INTR             (120U)    /* CBUFF error Interrup                             */
#define SOC_XWR14XX_DSS_FRAME_START_IRQ            (122U)    /* Frame start interrupt routed to DSS              */
#define SOC_XWR14XX_DSS_CHIRP_AVAIL_IRQ            (123U)    /* BSS chirp available interrupt                    */
#define SOC_XWR14XX_DSS_HW_ACC_PARAM_DONE_IRQ  (125U)        /* HWA param done interrupt                         */
#define SOC_XWR14XX_DSS_HW_ACC_DONE_IRQ            (126U)    /* HWA done Interrupt                               */
#define SOC_XWR14XX_DSS_HW_ACC_ERR_IRQ             (127U)    /* HWA access error Interrupt                       */

/*
 * MSS ESM Interrupt mapping
 */
/* Group 1 Errors */
#define SOC_XWR14XX_ANA_LIMP_MODE_SYNC_ESM       (63U)
#define SOC_XWR14XX_DCCB_ERROR_ESM               (62U)
#define SOC_XWR14XX_MSS_MBOX4BSS_UERR_ESM        (61U)
#define SOC_XWR14XX_MSS_MBOX4BSS_SBERR_ESM       (60U)
#define SOC_XWR14XX_BSS_MBOX4MSS_UERR_ESM        (59U)
#define SOC_XWR14XX_BSS_MBOX4MSS_SBERR_ESM       (58U)
#define SOC_XWR14XX_BSS_TO_MSS_TRIG_ESM          (57U)
#define SOC_XWR14XX_BSS_TO_MSS_ESM               (56U)
#define SOC_XWR14XX_EFC_ERROR_ESM                (40U)
#define SOC_XWR14XX_VIM_SELFTEST_ERR_ESM         (39U)
#define SOC_XWR14XX_GEM_IRQ_23_ESM               (36U)
#define SOC_XWR14XX_GEM_IRQ_22_ESM               (35U)
#define SOC_XWR14XX_CQDET_MODEERR_ESM            (34U)
#define SOC_XWR14XX_CAN_SERR_ESM                 (33U)
#define SOC_XWR14XX_ATCM_SINGLE_ERR_ESM          (32U)
#define SOC_XWR14XX_CCMR4_ERROR_SELFTEST_ESM     (31U)
#define SOC_XWR14XX_DCCA_ERROR_ESM               (30U)
#define SOC_XWR14XX_GEM_IRQ_ESM                  (29U)
#define SOC_XWR14XX_B1TCM_SINGLE_ERR_ESM         (28U)
#define SOC_XWR14XX_STC_ERROR_ESM                (27U)
#define SOC_XWR14XX_B0TCM_SINGLE_ERR_ESM         (26U)
#define SOC_XWR14XX_SPIA_SERR_ESM                (25U)
#define SOC_XWR14XX_FRC_COMP_ERR_ESM             (22U)
#define SOC_XWR14XX_CAN_UERR_ESM                 (21U)
#define SOC_XWR14XX_VIM_SERR_ESM                 (20U)
#define SOC_XWR14XX_FRC_SELFTEST_ERR_ESM         (19U)
#define SOC_XWR14XX_GEM_IRQ_20_ESM               (18U)
#define SOC_XWR14XX_SPIA_UERR_ESM                (17U)
#define SOC_XWR14XX_VIM_UERR_ESM                 (15U)
#define SOC_XWR14XX_DMA_WRERR_INT_PLS_ESM        (13U)
#define SOC_XWR14XX_GEM_IRQ_19_ESM               (9U )
#define SOC_XWR14XX_GEM_IRQ_18_ESM               (8U )
#define SOC_XWR14XX_GEM_IRQ_17_ESM               (7U )
#define SOC_XWR14XX_GEM_IRQ_16_ESM               (6U )
#define SOC_XWR14XX_DMA_UERR_ESM                 (3U )
#define SOC_XWR14XX_DMA_MPV_ESM                  (2U )
#define SOC_XWR14XX_NERROR_IN_SYNC_ESM           (0U )

/* Group 2 Errors */
#define SOC_XWR14XX_VIM_COMPARE_ERROR_ESM        (25U)
#define SOC_XWR14XX_WDT_NMI_REQ_ESM              (24U)
#define SOC_XWR14XX_MSS_CR4_LIVELOCK_ESM         (16U)
#define SOC_XWR14XX_B1TCM_PARITY_ERR_ESM         (8U )
#define SOC_XWR14XX_B0TCM_PARITY_ERR_ESM         (6U )
#define SOC_XWR14XX_ATCM_PARITY_ERR_ESM          (4U )
#define SOC_XWR14XX_CCMR4_ERROR_COMPARE_ESM      (2U )

/* Group 3 Errors */
#define SOC_XWR14XX_ATCM_UNCORR_ERR_ESM          (7U )
#define SOC_XWR14XX_B1TCM_UNCORR_ERR_ESM         (5U )
#define SOC_XWR14XX_B0TCM_UNCORR_ERR_ESM         (3U )
#define SOC_XWR14XX_EFC_AUTOLOAD_ERROR_ESM       (1U )

/***********************************************************************
 * MSS - Peripheral number of instance definition
 ***********************************************************************/
#define HWA_NUM_INSTANCES            (1U)
#define DMA_NUM_INSTANCES            (1U)

/***********************************************************************
 * MSS - Memory Map
 ************************************************************************/
/*! @brief Base Addresses of various modules. */
#define SOC_XWR14XX_MSS_TCMA_BASE_ADDRESS             0x00000000U
#define SOC_XWR14XX_MSS_TCMB_BASE_ADDRESS             0x08000000U
#define SOC_XWR14XX_MSS_SW_BUFFER_BASE_ADDRESS        0x0C200000U
#define SOC_XWR14XX_DSS_TPTC0_BASE_ADDRESS            0x50000000U
#define SOC_XWR14XX_DSS_REG_BASE_ADDRESS              0x50000400U
#define SOC_XWR14XX_DSS_TPTC1_BASE_ADDRESS            0x50000800U
#define SOC_XWR14XX_DSS_TPCC_BASE_ADDRESS             0x50010000U
#define SOC_XWR14XX_MSS_CSI_PROT_ENG_BASE_ADDRESS     0x50060000U
#define SOC_XWR14XX_MSS_CSI_PHY_BASE_ADDRESS          0x50060200U
#define SOC_XWR14XX_MSS_CBUFF_BASE_ADDRESS            0x50070000U
#define SOC_XWR14XX_MSS_HWA_1_COMMON_BASE_ADDRESS     0x50080800U
#define SOC_XWR14XX_MSS_HWA_1_PARAM_BASE_ADDRESS      0x50080000U
#define SOC_XWR14XX_MSS_HWA_1_RAM_BASE_ADDRESS        0x50081000U
#define SOC_XWR14XX_MSS_L3RAM_BASE_ADDRESS            0x51000000U
#define SOC_XWR14XX_MSS_ADCBUF_BASE_ADDRESS           0x52000000U
#define SOC_XWR14XX_MSS_CHIRPINFO_BASE_ADDRESS        0x52028000U
#define SOC_XWR14XX_MSS_HWA_MEM0_BASE_ADDRESS         0x52030000U
#define SOC_XWR14XX_MSS_HWA_MEM2_BASE_ADDRESS         0x52038000U
#define SOC_XWR14XX_MSS_EXT_FLASH_BASE_ADDRESS        0xC0000000U
#define SOC_XWR14XX_MSS_QSPI_BASE_ADDRESS             0xC0800000U
#define SOC_XWR14XX_MSS_MAILBOX_MEM_BASE_ADDRESS      0xF0601000U
#define SOC_XWR14XX_MSS_CRC_BASE_ADDRESS              0xFE000000U
#define SOC_XWR14XX_MSS_MIBSPIA_RAM_BASE_ADDRESS      0xFF0E0000U
#define SOC_XWR14XX_MSS_MIBSPIB_RAM_BASE_ADDRESS      0xFF0C0000U
#define SOC_XWR14XX_MSS_DCAN_MEM_BASE_ADDRESS         0xFF1E0000U
#define SOC_XWR14XX_MSS_GPIO_BASE_ADDRESS             0xFFF7BC00U
#define SOC_XWR14XX_MSS_I2C_BASE_ADDRESS              0xFFF7D400U
#define SOC_XWR14XX_MSS_DCAN_BASE_ADDRESS             0xFFF7DC00U
#define SOC_XWR14XX_MSS_SCI_A_BASE_ADDRESS            0xFFF7E500U
#define SOC_XWR14XX_MSS_SCI_B_BASE_ADDRESS            0xFFF7E700U
#define SOC_XWR14XX_MSS_MIBSPIA_BASE_ADDRESS          0xFFF7F400U
#define SOC_XWR14XX_MSS_MIBSPIB_BASE_ADDRESS          0xFFF7F600U
#define SOC_XWR14XX_MSS_DMA_1_PKT_BASE_ADDRESS        0xFFF80000U
#define SOC_XWR14XX_MSS_TOP_RCM_BASE_ADDRESS          0xFFFFE100U
#define SOC_XWR14XX_MSS_PINMUX_BASE_ADDRESS           0xFFFFEA00U
#define SOC_XWR14XX_MSS_RTIB_BASE_ADDRESS             0xFFFFEE00U
#define SOC_XWR14XX_MSS_ESM_BASE_ADDRESS              0xFFFFF500U
#define SOC_XWR14XX_MSS_GPCFG_BASE_ADDRESS            0xFFFFF800U
#define SOC_XWR14XX_MSS_DMA_1_CTRL_BASE_ADDRESS       0xFFFFF000U
#define SOC_XWR14XX_MSS_RCM_BASE_ADDRESS              0xFFFFFF00U
#define SOC_XWR14XX_MSS_MBOX_MSS_BSS_REG_BASE_ADDRESS 0xF0608000U
#define SOC_XWR14XX_MSS_MBOX_MSS_BSS_MEM_BASE_ADDRESS 0xF0602000U
#define SOC_XWR14XX_MSS_MBOX_BSS_MSS_REG_BASE_ADDRESS 0xF0608600U
#define SOC_XWR14XX_MSS_MBOX_BSS_MSS_MEM_BASE_ADDRESS 0xF0601000U

/*! @brief Sizes of various modules. */
#define SOC_XWR14XX_MSS_ADCBUF_SIZE               0x4000U
#define SOC_XWR14XX_MSS_CHIRPINFO_SIZE            0x800U

#define SOC_XWR14XX_MSS_L3RAM_NUM_BANK            MMWAVE_L3RAM_NUM_BANK
#define SOC_XWR14XX_MSS_L3RAM_SIZE                MMWAVE_L3RAM_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE

#define SOC_XWR14XX_MSS_SHMEM_TCMA_NUM_BANK       MMWAVE_SHMEM_TCMA_NUM_BANK
#define SOC_XWR14XX_MSS_SHMEM_TCMA_SIZE           MMWAVE_SHMEM_TCMA_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE           /* Extended on Share Memory */
#define SOC_XWR14XX_MSS_TCMA_SIZE                 MMWAVE_SHMEM_TCMA_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE+0x20000U  /* Extended on Share Memory + Default 128KB */

#define SOC_XWR14XX_MSS_SHMEM_TCMB_NUM_BANK       MMWAVE_SHMEM_TCMB_NUM_BANK
#define SOC_XWR14XX_MSS_SHMEM_TCMB_SIZE           MMWAVE_SHMEM_TCMB_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE           /* Extended on Share Memory */
#define SOC_XWR14XX_MSS_TCMB_SIZE                 MMWAVE_SHMEM_TCMB_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE+0x10000U  /* Extended on Share Memory + Default 64KB */

#define SOC_XWR14XX_MSS_HWA_MEM_SIZE              (65536U)
#define SOC_XWR14XX_MSS_HWA_NUM_MEM_BANKS         (4U)
#define SOC_XWR14XX_MSS_HWA_WINDOW_RAM_SIZE_IN_SAMPLES (1024U)
#define SOC_XWR14XX_MSS_HWA_NUM_PARAM_SETS        (16U)

/***********************************************************************
 * MSS - CLOCK settings
 ***********************************************************************/
 /* Sys_vclk : 200MHz */
#define MSS_SYS_VCLK                   200000000U
#define R4F_CLOCK_MHZ                  200U

/*************************************************************
 * BASE Address for the various module as seen by EDMA3
 *************************************************************/
#define EDMA3_DSS_ADCBUFF_BASE        0x21000000U
#define EDMA3_DSS_FIFO_BASE           0x21020000U
#define EDMA3_DSS_CP0_BASE            0x21070020U
#define EDMA3_DSS_CP1_BASE            0x21070030U
#define EDMA3_DSS_CP2_BASE            0x21070040U
#define EDMA3_DSS_CP3_BASE            0x21070050U

/*! @brief R4F to Hardware Accelerator address translation macro. */
#define ADDR_TRANSLATE_CPU_TO_HWA(x)  (uint16_t)(((uint32_t)(x) - SOC_XWR14XX_MSS_HWA_MEM0_BASE_ADDRESS) & 0x0000FFFFU)

/** @defgroup EDMA_HW_DEFS EDMA Hardware Definitions.
 *
 * @brief
 *  EDMA hardware definitions consisting of :
 *  - CC and TC base addresses.
 *  - Event mappings into the 64 EDMA lines.
 *  - Interrupt numbers out of EDMA.
 *  - Number of instances of CC, TC, param sets.
 *
 @{ */

#define EDMA_CC0_BASE_ADDRESS         SOC_XWR14XX_DSS_TPCC_BASE_ADDRESS
#define EDMA_CC0_TC0_BASE_ADDRESS     SOC_XWR14XX_DSS_TPTC0_BASE_ADDRESS
#define EDMA_CC0_TC1_BASE_ADDRESS     SOC_XWR14XX_DSS_TPTC1_BASE_ADDRESS

#define EDMA_NUM_CC            (1U)
#define EDMA_NUM_TC            (2U)
#define EDMA_NUM_PARAM_SETS    (128U)
#define EDMA_CC0_TRANSFER_COMPLETE_INTR_ID  SOC_XWR14XX_DSS_TPCC_IRQ_DONE
#define EDMA_CC0_ERRROR_INTR_ID             SOC_XWR14XX_DSS_TPCC_IRQ_ERR
#define EDMA_CC0_TC0_ERRROR_INTR_ID         SOC_XWR14XX_DSS_TPTC0_IRQ_ERR
#define EDMA_CC0_TC1_ERRROR_INTR_ID         SOC_XWR14XX_DSS_TPTC1_IRQ_ERR

#define EDMA_MAX_NUM_TC        EDMA_NUM_TC

/*! @brief EDMA event interrupt mapping */
#define EDMA_TPCC0_REQ_CBUFF_0    (0U)
#define EDMA_TPCC0_REQ_CBUFF_1    (1U)
#define EDMA_TPCC0_REQ_CBUFF_2    (2U)
#define EDMA_TPCC0_REQ_CBUFF_3    (3U)
#define EDMA_TPCC0_REQ_CBUFF_4    (4U)
#define EDMA_TPCC0_REQ_CBUFF_5    (5U)
#define EDMA_TPCC0_REQ_CBUFF_6    (6U)

#define EDMA_TPCC0_REQ_FREE_0    (7U)

#define EDMA_TPCC0_REQ_DFE_FRAME_START     (8U)
#define EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL     (9U)

#define EDMA_TPCC0_REQ_CSI2_0     (10U)
#define EDMA_TPCC0_REQ_CSI2_1     (11U)
#define EDMA_TPCC0_REQ_CSI2_2     (12U)
#define EDMA_TPCC0_REQ_CSI2_3     (13U)

#define EDMA_TPCC0_REQ_VIN_FRAME_START        (14U)
#define EDMA_TPCC0_REQ_VIN_CHIRP_AVAILABLE    (15U)
#define EDMA_TPCC0_REQ_VIN_CHANNEL_AVAILABLE  (16U)

#define EDMA_TPCC0_REQ_HWACC_0    (17U)
#define EDMA_TPCC0_REQ_HWACC_1    (18U)
#define EDMA_TPCC0_REQ_HWACC_2    (19U)
#define EDMA_TPCC0_REQ_HWACC_3    (20U)
#define EDMA_TPCC0_REQ_HWACC_4    (21U)
#define EDMA_TPCC0_REQ_HWACC_5    (22U)
#define EDMA_TPCC0_REQ_HWACC_6    (23U)
#define EDMA_TPCC0_REQ_HWACC_7    (24U)
#define EDMA_TPCC0_REQ_HWACC_8    (25U)
#define EDMA_TPCC0_REQ_HWACC_9    (26U)
#define EDMA_TPCC0_REQ_HWACC_10   (27U)
#define EDMA_TPCC0_REQ_HWACC_11   (28U)
#define EDMA_TPCC0_REQ_HWACC_12   (29U)
#define EDMA_TPCC0_REQ_HWACC_13   (30U)
#define EDMA_TPCC0_REQ_HWACC_14   (31U)
#define EDMA_TPCC0_REQ_HWACC_15   (32U)

#define EDMA_TPCC0_REQ_FREE_1     (33U)
#define EDMA_TPCC0_REQ_FREE_2     (34U)

#define EDMA_TPCC0_REQ_FRC_0      (35U)
#define EDMA_TPCC0_REQ_FRC_1      (36U)
#define EDMA_TPCC0_REQ_FRC_2      (37U)
#define EDMA_TPCC0_REQ_FRC_3      (38U)

#define EDMA_TPCC0_REQ_FREE_3     (39U)

#define EDMA_TPCC0_REQ_FRC_LOGICAL_FRAME_START (40U)
#define EDMA_TPCC0_REQ_ADC_VALID_FALL          (41U)

#define EDMA_TPCC0_REQ_FREE_4    (42U)
#define EDMA_TPCC0_REQ_FREE_5    (43U)
#define EDMA_TPCC0_REQ_FREE_6    (44U)
#define EDMA_TPCC0_REQ_FREE_7    (45U)
#define EDMA_TPCC0_REQ_FREE_8    (46U)
#define EDMA_TPCC0_REQ_FREE_9    (47U)
#define EDMA_TPCC0_REQ_FREE_10   (48U)
#define EDMA_TPCC0_REQ_FREE_11   (49U)
#define EDMA_TPCC0_REQ_FREE_12   (50U)
#define EDMA_TPCC0_REQ_FREE_13   (51U)
#define EDMA_TPCC0_REQ_FREE_14   (52U)
#define EDMA_TPCC0_REQ_FREE_15   (53U)
#define EDMA_TPCC0_REQ_FREE_16   (54U)
#define EDMA_TPCC0_REQ_FREE_17   (55U)
#define EDMA_TPCC0_REQ_FREE_18   (56U)
#define EDMA_TPCC0_REQ_FREE_19   (57U)
#define EDMA_TPCC0_REQ_FREE_20   (58U)
#define EDMA_TPCC0_REQ_FREE_21   (59U)
#define EDMA_TPCC0_REQ_FREE_22   (60U)
#define EDMA_TPCC0_REQ_FREE_23   (61U)
#define EDMA_TPCC0_REQ_FREE_24   (62U)
#define EDMA_TPCC0_REQ_FREE_25   (63U)

/** @}*/ /* end defgroup EDMA_HW_DEFS */

#define SYS_COMMON_NUM_TX_ANTENNAS  3U

#ifdef __cplusplus
}
#endif

#endif /* SYS_COMMON_XWR14XX_H */
