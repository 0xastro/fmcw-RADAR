/**
 *   @file  sys_common_xwr16xx_dss.h
 *
 *   @brief
 *      This is the common header file used by the various mmWave SDK
 *      modules for XWR16xx DSP subsystem.
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

#ifndef SYS_COMMON_XWR16XX_DSS_H
#define SYS_COMMON_XWR16XX_DSS_H

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(SOC_XWR16XX) && defined(SUBSYS_DSS) )

/***********************************************************************
 * DSS - Memory Map - Defined in XWR16xx TRM
 ************************************************************************/
/**
 * @defgroup SOC_XWR16XX_DSS_BASE_ADDRESS     SOC_XWR16XX_DSS_BASE_ADDRESS
 * @brief    Macros that define Base Addresses of various modules
 * @{
 */

/* DSP Memory */
#define SOC_XWR16XX_DSS_L1D_BASE_ADDRESS            0x00F00000U    /* L1 Data memory space: 0x00F0:0000-0x00F0:7FFF */
#define SOC_XWR16XX_DSS_L1D_SIZE                    0x8000U        /* Size: 32KB */
#define SOC_XWR16XX_DSS_L1P_BASE_ADDRESS            0x00E00000U    /* L1 Program memory space: 0x00E0:0000-0x00E0:7FFF */
#define SOC_XWR16XX_DSS_L1P_SIZE                    0x8000U        /* Size: 32KB */
#define SOC_XWR16XX_DSS_L2_UMAP0_BASE_ADDRESS       0x00800000U    /* L2 UMAP0 RAM space: 0x0080:0000-0x0081:FFFF */
#define SOC_XWR16XX_DSS_L2_UMAP0_SIZE               0x20000U       /* Size: 128KB */
#define SOC_XWR16XX_DSS_L2_UMAP1_BASE_ADDRESS       0x007E0000U    /* L2 UMAP1 RAM space: 0x007E:0000-0x007F:FFFF */
#define SOC_XWR16XX_DSS_L2_UMAP1_SIZE               0x20000U       /* Size: 128KB */

/* EDMA Memory */
#define SOC_XWR16XX_DSS_EDMA_TPCC0_BASE_ADDRESS     0x02010000U    /* EDMA TPCC0 memory space: 0x0201:0000-0x0201:3FFF */
#define SOC_XWR16XX_DSS_EDMA_TPCC1_BASE_ADDRESS     0x020A0000U    /* EDMA TPCC1 memory space: 0x020A:0000-0x020A:3FFF */
#define SOC_XWR16XX_DSS_EDMA_TPTC0_BASE_ADDRESS     0x02000000U    /* EDMA TPTC0 memory space: 0x0200:0000-0x0200:03FF */
#define SOC_XWR16XX_DSS_EDMA_TPTC1_BASE_ADDRESS     0x02000800U    /* EDMA TPTC1 memory space: 0x0200:0800-0x0200:0BFF */
#define SOC_XWR16XX_DSS_EDMA_TPTC2_BASE_ADDRESS     0x02090000U    /* EDMA TPTC2 memory space: 0x0209:0000-0x0209:03FF */
#define SOC_XWR16XX_DSS_EDMA_TPTC3_BASE_ADDRESS     0x02090400U    /* EDMA TPTC3 memory space: 0x0209:0400-0x0209:07FF */

/* CC0 defines */
#define EDMA_CC0_BASE_ADDRESS           SOC_XWR16XX_DSS_EDMA_TPCC0_BASE_ADDRESS
#define EDMA_CC0_TC0_BASE_ADDRESS       SOC_XWR16XX_DSS_EDMA_TPTC0_BASE_ADDRESS
#define EDMA_CC0_TC1_BASE_ADDRESS       SOC_XWR16XX_DSS_EDMA_TPTC1_BASE_ADDRESS

/* CC1 defines */
#define EDMA_CC1_BASE_ADDRESS           SOC_XWR16XX_DSS_EDMA_TPCC1_BASE_ADDRESS
#define EDMA_CC1_TC0_BASE_ADDRESS       SOC_XWR16XX_DSS_EDMA_TPTC2_BASE_ADDRESS
#define EDMA_CC1_TC1_BASE_ADDRESS       SOC_XWR16XX_DSS_EDMA_TPTC3_BASE_ADDRESS

/* DSS Controller Register */
#define SOC_XWR16XX_DSS_DSSREG_BASE_ADDRESS         0x02000400U    /* DSS Controller Registers memroy space: 0x0200:0400-0x0200:07FF */
#define SOC_XWR16XX_DSS_DSSREG2_BASE_ADDRESS        0x02000C00U    /* DSS Controller2 Registers memroy space: 0x0200:0C00-0x0200:0FFF */
#define SOC_XWR16XX_DSP_ICFG_BASE_ADDRESS           0x01800000U    /* DSP (C674) Megamodule base address */
#define SOC_XWR16XX_DSS_GPCFG_BASE_ADDRESS          0x05FFF800U    /* GPCFG Registers memroy space: 0x05FF:F800 - 0x05FF:FBFF */

/* System Memory */
#define SOC_XWR16XX_DSS_L3RAM_BASE_ADDRESS          0x20000000U    /* L3 shared memory */
#define SOC_XWR16XX_DSS_L3RAM_SIZE                  MMWAVE_L3RAM_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE       /* Size of L3 RAM */
#define SOC_XWR16XX_DSS_ADCBUF_BASE_ADDRESS         0x21000000U    /* ADC buffer memroy space: 0x2100:0000-0x2100:07FC */
#define SOC_XWR16XX_DSS_ADCBUF_SIZE                 0x8000U        /* Size: 32KB */
#define SOC_XWR16XX_DSS_CHIRPINFO_BASE_ADDRESS      0x21028000U    /* Chirp INFO memroy space: 0x2102:8000-0x2102:9FFF */
#define SOC_XWR16XX_DSS_CHIRPINFO_SIZE              0x2000U        /* Size: 8KB */
#define SOC_XWR16XX_DSS_HSRAM_BASE_ADDRESS          0x21080000U    /* HS-RAM shared memory : 0x2108:0000-2108:7FFC */
#define SOC_XWR16XX_DSS_HSRAM_SIZE                  0x8000U        /* Size: 32KB */

/* System Peripheral Registers */
/* RTI */
#define SOC_XWR16XX_DSS_RTIA_BASE_ADDRESS           0x02020000U
#define SOC_XWR16XX_DSS_RTIB_BASE_ADDRESS           0x020F0000U

/* SCI */
#define SOC_XWR16XX_DSS_SCI_BASE_ADDRESS            0x02030000U

/* STC */
#define SOC_XWR16XX_DSS_STC_BASE_ADDRESS            0x02040000U

/* VIN */
#define SOC_XWR16XX_DSS_VIN_BASE_ADDRESS            0x02050000U

/* CBUFF */
#define SOC_XWR16XX_DSS_CBUFF_BASE_ADDRESS          0x02070000U

/* ESM */
#define SOC_XWR16XX_DSS_ESM_BASE_ADDRESS            0x020D0000U

/* RCM */
#define SOC_XWR16XX_DSS_RCM_BASE_ADDRESS            0x05FFFF00U

/* TOP RCM */
#define SOC_XWR16XX_DSS_TOP_RCM_BASE_ADDRESS        0x05FFE100U

/* CRC */
#define SOC_XWR16XX_DSS_CRC_BASE_ADDRESS            0x22000000U

/* MailBox */
/*Next 4 defines: DSS mailbox base addresses to communicate with MSS*/
#define SOC_XWR16XX_DSS_MBOX_DSS_MSS_REG_BASE_ADDRESS            0x04608300U
#define SOC_XWR16XX_DSS_MBOX_DSS_MSS_MEM_BASE_ADDRESS            0x50605000U
#define SOC_XWR16XX_DSS_MBOX_MSS_DSS_REG_BASE_ADDRESS            0x04608400U
#define SOC_XWR16XX_DSS_MBOX_MSS_DSS_MEM_BASE_ADDRESS            0x50604000U
/*Next 4 defines: DSS mailbox base addresses to communicate with BSS*/
#define SOC_XWR16XX_DSS_MBOX_DSS_BSS_REG_BASE_ADDRESS            0x04608100U
#define SOC_XWR16XX_DSS_MBOX_DSS_BSS_MEM_BASE_ADDRESS            0x50607000U
#define SOC_XWR16XX_DSS_MBOX_BSS_DSS_REG_BASE_ADDRESS            0x04608200U
#define SOC_XWR16XX_DSS_MBOX_BSS_DSS_MEM_BASE_ADDRESS            0x50606000U
/** @}*/ //SOC_XWR16XX_MSS_BASE_ADDRESS

/***********************************************************************
 * DSS - C674x INTC Interrupt Mapping
 ************************************************************************/
/**
 * @defgroup SOC_XWR16XX_DSS_INTERRUPTS_MAP     SOC_XWR16XX_DSS_INTERRUPTS_MAP
 * @brief    Macros that define VIM Interrupt Mapping
 * @{
 */

/* EVT0: INT controller, output of event combiner 0, for events 1-31 */
#define SOC_XWR16XX_DSS_INTC_EVENT0                      (0U)
/* EVT1: INT controller, output of event combiner 0, for events 32-63 */
#define SOC_XWR16XX_DSS_INTC_EVENT1                      (1U)
/* EVT2: INT controller, output of event combiner 0, for events 64-95 */
#define SOC_XWR16XX_DSS_INTC_EVENT2                      (2U)
/* EVT3: INT controller, output of event combiner 0, for events 96-127 */
#define SOC_XWR16XX_DSS_INTC_EVENT3                      (3U)

/* IDMAINTx From EMC */
#define SOC_XWR16XX_DSS_INTC_IDMAINT0                    (13U)    /* IDMA channel 0 interrupt */
#define SOC_XWR16XX_DSS_INTC_IDMAINT1                    (14U)    /* IDMA channel 1 interrupt */

/* Events for different IP Modules */
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC0_IRQ_DONE        (16U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC0_IRQ_ERR         (17U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC1_IRQ_DONE        (18U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC1_IRQ_ERR         (19U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPCC0_IRQ_DONE        (20U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPCC0_IRQ_ERR         (21U)
#define SOC_XWR16XX_DSS_INTC_EVENT_CBUFF_IRQ             (22U)
#define SOC_XWR16XX_DSS_INTC_EVENT_CBUFF_ERR_INTR        (24U)

/* The following 3 interrupts share the same interrupt event */
#define SOC_XWR16XX_DSS_INTC_EVENT_FRAME_START           (26U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT0            (26U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT39           (26U)

/* The following 3 interrupts share the same interrupt event */
#define SOC_XWR16XX_DSS_INTC_EVENT_CHIRP_AVAIL           (27U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT2            (27U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT43           (27U)

#define SOC_XWR16XX_DSS_INTC_EVENT_HW_ACC_PARAM_DONE (29U)
#define SOC_XWR16XX_DSS_INTC_EVENT_HW_ACC_DONE           (30U)
#define SOC_XWR16XX_DSS_INTC_EVENT_HW_ACC_ERR            (31U)
#define SOC_XWR16XX_DSS_INTC_EVENT_ESM_LOW_PRIORITY  (32U)
#define SOC_XWR16XX_DSS_INTC_EVENT_MCRC                  (33U)

#define SOC_XWR16XX_DSS_INTC_EVENT_PROG_FILT_ERR         (34U)
#define SOC_XWR16XX_DSS_INTC_EVENT_GEM_WAKEUP_FROM_DFT (35U)
#define SOC_XWR16XX_DSS_INTC_EVENT_GEM_STC_DONE          (36U)
#define SOC_XWR16XX_DSS_INTC_EVENT_PBIST_DONE            (37U)

#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT8            (46U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT4            (47U)

#define SOC_XWR16XX_DSS_INTC_EVENT_MSS_SW0_INT           (58U)
#define SOC_XWR16XX_DSS_INTC_EVENT_MSS_SW1_INT           (59U)

#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT5            (60U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT6            (61U)
#define SOC_XWR16XX_DSS_INTC_EVENT_BSS_SW1_INT           (62U)
#define SOC_XWR16XX_DSS_INTC_EVENT_BSS_SW2_INT           (63U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC2_IRQ_DONE        (64U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC2_IRQ_ERR         (65U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC3_IRQ_DONE        (66U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPTC3_IRQ_ERR         (67U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPCC1_IRQ_DONE        (68U)
#define SOC_XWR16XX_DSS_INTC_EVENT_TPCC1_IRQ_ERR         (69U)

/* The following 3 interrupts share the same interrupt event */
#define SOC_XWR16XX_DSS_INTC_EVENT_ADC_DATA_VALID        (70U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT3            (70U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT44           (70U)

#define SOC_XWR16XX_DSS_INTC_EVENT_UART_REQ0             (71U)
#define SOC_XWR16XX_DSS_INTC_EVENT_UART_REQ1             (72U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI0_OVERFLOW_0       (73U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI0_OVERFLOW_1       (74U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI0_0                (75U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI0_1                (76U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI0_2                (77U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI0_3                (78U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI1_OVERFLOW_0       (79U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI1_OVERFLOW_1       (80U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI1_0                (81U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI1_1                (82U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI1_2                (83U)
#define SOC_XWR16XX_DSS_INTC_EVENT_RTI1_3                (84U)
#define SOC_XWR16XX_DSS_INTC_MBOX_DSS_BSS_BOX_FULL       (85U)
#define SOC_XWR16XX_DSS_INTC_MBOX_DSS_BSS_BOX_EMPTY  (86U)
#define SOC_XWR16XX_DSS_INTC_EVENT_GPIO_0                (87U)
#define SOC_XWR16XX_DSS_INTC_EVENT_GPIO_1                (88U)
#define SOC_XWR16XX_DSS_INTC_EVENT_GPIO_2                (89U)
#define SOC_XWR16XX_DSS_INTC_EVENT_GPIO_3                (90U)
#define SOC_XWR16XX_DSS_INTC_MBOX_DSS_MSS_BOX_FULL       (91U)
#define SOC_XWR16XX_DSS_INTC_MBOX_DSS_MSS_BOX_EMPTY  (92U)

/* The following 3 interrupts share the same interrupt event */
#define SOC_XWR16XX_DSS_INTC_EVENT_LOGICAL_FRAME_START (93U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT1            (93U)
#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT40           (93U)

#define SOC_XWR16XX_DSS_INTC_EVENT_DMM_SWINT7            (94U)

/* C674x INT System Megamodule Events */
#define SOC_XWR16XX_DSS_INTC_EVENT_INTERR                (96U)       /* Dropped CPU interrupt event */
#define SOC_XWR16XX_DSS_INTC_EVENT_EMC_IDMAERR           (97U)       /* Invalid IDMA parameters */

/* C674x INT System Megamodule Events */
#define SOC_XWR16XX_DSS_INTC_EVENT_PMC_ED                (113U)      /* L1P Parity Error interrupt */
#define SOC_XWR16XX_DSS_INTC_EVENT_UMC_ED1               (116U)      /* L2 ECC Single Error Correction */
#define SOC_XWR16XX_DSS_INTC_EVENT_UMC_ED2               (117U)      /* L2 ECC Double Error Detection */
#define SOC_XWR16XX_DSS_INTC_EVENT_PDC_INT               (118U)      /* PDC sleep interrupt */
#define SOC_XWR16XX_DSS_INTC_EVENT_SYS_CMPA              (119U)      /* SYS CPU memory protection fault */
#define SOC_XWR16XX_DSS_INTC_EVENT_L1P_CMPA              (120U)      /* L1P CPU memory protection fault */
#define SOC_XWR16XX_DSS_INTC_EVENT_L1P_DMPA              (121U)      /* L1P DMA memory protection fault */
#define SOC_XWR16XX_DSS_INTC_EVENT_L1D_CMPA              (122U)      /* L1D CPU memory protection fault */
#define SOC_XWR16XX_DSS_INTC_EVENT_L1D_DMPA              (123U)      /* L1D DMA memory protection fault */
#define SOC_XWR16XX_DSS_INTC_EVENT_L2_CMPA               (124U)      /* L2 CPU memory protection fault*/
#define SOC_XWR16XX_DSS_INTC_EVENT_L2_DMPA               (125U)      /* L2 DMA memory protection fault */
#define SOC_XWR16XX_DSS_INTC_EVENT_EMC_CMPA              (126U)      /* EMC CPU memory protection fault */
#define SOC_XWR16XX_DSS_INTC_EVENT_EMC_BUSERR            (127U)      /* EMC_BUSERR EMC Bus error interrupt */
 /** @}*/ //SOC_XWR16XX_DSS_INTERRUPTS_MAP

/*
 * DSS ESM Interrupt mapping
 */

/***********************************************************************
 * DSS - ESM Interrupt mapping
 ***********************************************************************/
/* Group 1 Errors */
#define SOC_XWR16XX_DSS_DSP_L1P_PARITY_ERR_ESM                (56U)
#define SOC_XWR16XX_DSS_MSS2DSS_MB_REPAIR_ERR_ESM             (55U)
#define SOC_XWR16XX_DSS_MSS2DSS_MB_FATAL_ERR_ESM              (54U)
#define SOC_XWR16XX_DSS_DSS2MSS_MB_REPAIR_ERR_ESM             (53U)
#define SOC_XWR16XX_DSS_DSS2MSS_MB_FATAL_ERR_ESM              (52U)
#define SOC_XWR16XX_DSS_BSS2DSS_MB_REPAIR_ERR_ESM             (51U)
#define SOC_XWR16XX_DSS_DSS2BSS_MB_REPAIR_ERR_ESM             (50U)
#define SOC_XWR16XX_DSS_BSS2DSS_MB_FATAL_ERR_ESM              (49U)
#define SOC_XWR16XX_DSS_DSS2BSS_MB_FATAL_ERR_ESM              (48U)
#define SOC_XWR16XX_DSS_CFG_MSTID_MPU_ERR_ESM                 (31U)

#define SOC_XWR16XX_DSS_DATA_TXFR_RAM_ECC_FATAL_ERR_ESM        (28U)
#define SOC_XWR16XX_DSS_DATA_TXFR_RAM_ECC_REPAIR_ERR_ESM        (27U)

#define SOC_XWR16XX_DSS_STC_ERR_ESM                           (26U)
#define SOC_XWR16XX_DSP_L2RAM_ECC_REPAIR_ERR_ESM              (25U)
#define SOC_XWR16XX_DSS_HSRAM1_ECC_FATAL_ERR_ESM              (24U)
#define SOC_XWR16XX_DSS_HSRAM1_ECC_REPAIR_ERR_ESM             (23U)
#define SOC_XWR16XX_DSS_ADCBUF_PONG_FATAL_ERR_ESM             (18U)
#define SOC_XWR16XX_DSS_ADCBUF_PONG_ECC_REPAIR_ERR_ESM        (17U)
#define SOC_XWR16XX_DSS_ADCBUF_PING_FATAL_ERR_ESM             (16U)
#define SOC_XWR16XX_DSS_ADCBUF_PING_ECC_REPAIR_ESM            (15U)

#define SOC_XWR16XX_DSS_TPTC3_WR_MPU_ERR_ESM                  (14U)
#define SOC_XWR16XX_DSS_TPTC3_RD_MPU_ERR_ESM                  (13U)
#define SOC_XWR16XX_DSS_TPTC2_WR_MPU_ERR_ESM                  (12U)
#define SOC_XWR16XX_DSS_TPTC2_RD_MPU_ERR_ESM                  (11U)
#define SOC_XWR16XX_DSS_TPCC1_PARITY_ERR_ESM                  (10U)
#define SOC_XWR16XX_DSS_CBUFF_SAFETY_ERR_ESM                  (9U)
#define SOC_XWR16XX_DSS_TPTC1_WR_MPU_ERR_ESM                  (8U)
#define SOC_XWR16XX_DSS_TPTC1_RD_MPU_ERR_ESM                  (7U)
#define SOC_XWR16XX_DSS_TPTC0_WR_MPU_ERR_ESM                  (6U)
#define SOC_XWR16XX_DSS_TPTC0_RD_MPU_ERR_ESM                  (5U)
#define SOC_XWR16XX_DSS_CBUFF_ECC_FATAL_ERR_ESM               (4U)
#define SOC_XWR16XX_DSS_CBUFF_ECC_REPAIR_ERR_ESM              (3U)
#define SOC_XWR16XX_DSS_TPCC_PARITY_ERR_ESM                   (2U)
#define SOC_XWR16XX_DSS_L3RAM_ECC_FATAL_ERR_ESM               (1U)
#define SOC_XWR16XX_DSS_L3RAM_ECC_REPAIR_ERR_ESM              (0U)

/* Group 2 Errors */
#define SOC_XWR16XX_DSP_L2RAM_ECC_FATAL_ERR_ESM               (5U)
#define SOC_XWR16XX_DSP_PBIST_ERR_ESM                         (4U)
#define SOC_XWR16XX_DSS_STC_ERR_GRP2_ESM                      (3U)
#define SOC_XWR16XX_DSS_WATCHDOG_NMI_ESM                      (2U)
#define SOC_XWR16XX_DSP_L2_UMAP1_PARITY_ERR_ESM               (1U)
#define SOC_XWR16XX_DSP_L2_UMAP0_PARITY_ERR_ESM               (0U)

/***********************************************************************
 * DSS - CLOCK settings
 ***********************************************************************/
/* Sys_vclk : 200MHz */
#define DSS_SYS_VCLK                                    (200000000U)
#define DSP_CLOCK_MHZ                                   (600U)
/***********************************************************************
 * DSS - Peripheral number of instance definition
 ***********************************************************************/

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
/* Note: Number of instances of CC, TC, param sets are defined in sys_common_xwr16xx.h */
#define EDMA_CC0_TRANSFER_COMPLETE_INTR_ID   SOC_XWR16XX_DSS_INTC_EVENT_TPCC0_IRQ_DONE
#define EDMA_CC0_ERRROR_INTR_ID              SOC_XWR16XX_DSS_INTC_EVENT_TPCC0_IRQ_ERR
#define EDMA_CC0_TC0_ERRROR_INTR_ID          SOC_XWR16XX_DSS_INTC_EVENT_TPTC0_IRQ_ERR
#define EDMA_CC0_TC1_ERRROR_INTR_ID          SOC_XWR16XX_DSS_INTC_EVENT_TPTC1_IRQ_ERR

#define EDMA_CC1_TRANSFER_COMPLETE_INTR_ID   SOC_XWR16XX_DSS_INTC_EVENT_TPCC1_IRQ_DONE
#define EDMA_CC1_ERRROR_INTR_ID              SOC_XWR16XX_DSS_INTC_EVENT_TPCC1_IRQ_ERR
#define EDMA_CC1_TC0_ERRROR_INTR_ID          SOC_XWR16XX_DSS_INTC_EVENT_TPTC2_IRQ_ERR
#define EDMA_CC1_TC1_ERRROR_INTR_ID          SOC_XWR16XX_DSS_INTC_EVENT_TPTC3_IRQ_ERR

/** @}*/ /* end defgroup EDMA_HW_DEFS */

#endif /* #if (defined(SOC_XWR16XX) && defined(SUBSYS_DSS) ) */

#ifdef __cplusplus
}
#endif

#endif /* SYS_COMMON_XWR16XX_DSS_H */
