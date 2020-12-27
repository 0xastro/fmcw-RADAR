/**
 *   @file  sys_common_xwr18xx_mss.h
 *
 *   @brief
 *      This is the common header file used by the various mmWave SDK
 *      modules for XWR18xx Master subsystem.
 */

 /*  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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

#ifndef SYS_COMMON_XWR18XX_MSS_H
#define SYS_COMMON_XWR18XX_MSS_H

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(SOC_XWR18XX) && defined(SUBSYS_MSS) )

/*! @brief Ensures that data transfer has finished. */
static inline void MEM_BARRIER(void)
{
    asm(" dsb ");
}

/***********************************************************************
 * MSS - Memory Map
 ************************************************************************/
/**
 * @defgroup SOC_XWR18XX_MSS_BASE_ADDRESS     SOC_XWR18XX_MSS_BASE_ADDRESS
 * @brief    Macros that define Base Addresses of various modules
 * @{
 */

/* MSS System Memory */
#define SOC_XWR18XX_MSS_TCMA_BASE_ADDRESS            0x00000000U    /* TCM RAM-A */
#define SOC_XWR18XX_MSS_SHMEM_TCMA_NUM_BANK          MMWAVE_SHMEM_TCMA_NUM_BANK
#define SOC_XWR18XX_MSS_SHMEM_TCMA_SIZE              MMWAVE_SHMEM_TCMA_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE          /* Extended on Share Memory */
#define SOC_XWR18XX_MSS_TCMA_SIZE                    MMWAVE_SHMEM_TCMA_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE+0x80000U /* Extended on Share Memory + Default 512KB */
       
#define SOC_XWR18XX_MSS_TCMB_BASE_ADDRESS            0x08000000U    /* TCM RAM-B */
#define SOC_XWR18XX_MSS_SHMEM_TCMB_NUM_BANK          MMWAVE_SHMEM_TCMB_NUM_BANK
#define SOC_XWR18XX_MSS_SHMEM_TCMB_SIZE              MMWAVE_SHMEM_TCMB_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE          /* Extended on Share Memory */
#define SOC_XWR18XX_MSS_TCMB_SIZE                    MMWAVE_SHMEM_TCMB_NUM_BANK*MMWAVE_SHMEM_BANK_SIZE+0x30000U /* Extended on Share Memory + Default 192KB */ 
                   
#define SOC_XWR18XX_MSS_SW_BUFFER_BASE_ADDRESS       0x0C200000U    /* Software Buffer */
#define SOC_XWR18XX_MSS_SW_BUFFER_SIZE               0x2000U        /* Size: 8KB */

/* EDMA defines */
#define SOC_XWR18XX_MSS_EDMA_TPCC0_BASE_ADDRESS     0x50010000U    /* EDMA TPCC0 memory space: 0x5001:0000-0x5001:3FFF */
#define SOC_XWR18XX_MSS_EDMA_TPCC1_BASE_ADDRESS     0x500A0000U    /* EDMA TPCC1 memory space: 0x500A:0000-0x500A:3FFF */
#define SOC_XWR18XX_MSS_EDMA_TPTC0_BASE_ADDRESS     0x50000000U    /* EDMA TPTC0 memory space: 0x5000:0000-0x5000:03FF */
#define SOC_XWR18XX_MSS_EDMA_TPTC1_BASE_ADDRESS     0x50000800U    /* EDMA TPTC1 memory space: 0x5000:0800-0x5000:0BFF */
#define SOC_XWR18XX_MSS_EDMA_TPTC2_BASE_ADDRESS     0x50090000U    /* EDMA TPTC2 memory space: 0x5009:0000-0x5009:03FF */
#define SOC_XWR18XX_MSS_EDMA_TPTC3_BASE_ADDRESS     0x50090400U    /* EDMA TPTC3 memory space: 0x5009:0400-0x5009:07FF */

/* CC0 defines */
#define EDMA_CC0_BASE_ADDRESS                       SOC_XWR18XX_MSS_EDMA_TPCC0_BASE_ADDRESS
#define EDMA_CC0_TC0_BASE_ADDRESS                   SOC_XWR18XX_MSS_EDMA_TPTC0_BASE_ADDRESS
#define EDMA_CC0_TC1_BASE_ADDRESS                   SOC_XWR18XX_MSS_EDMA_TPTC1_BASE_ADDRESS

/* CC1 defines */
#define EDMA_CC1_BASE_ADDRESS                       SOC_XWR18XX_MSS_EDMA_TPCC1_BASE_ADDRESS
#define EDMA_CC1_TC0_BASE_ADDRESS                   SOC_XWR18XX_MSS_EDMA_TPTC2_BASE_ADDRESS
#define EDMA_CC1_TC1_BASE_ADDRESS                   SOC_XWR18XX_MSS_EDMA_TPTC3_BASE_ADDRESS

/* DSS Controller Register */
#define SOC_XWR18XX_MSS_DSSREG_BASE_ADDRESS          0x50000400U
#define SOC_XWR18XX_MSS_DSSREG2_BASE_ADDRESS         0x50000C00U

/* CBUFF */
#define SOC_XWR18XX_MSS_CBUFF_BASE_ADDRESS           0x50070000U

/* HWA */
#define SOC_XWR18XX_MSS_HWA_1_COMMON_BASE_ADDRESS     0x50080800U
#define SOC_XWR18XX_MSS_HWA_1_PARAM_BASE_ADDRESS      0x50080000U
#define SOC_XWR18XX_MSS_HWA_1_RAM_BASE_ADDRESS        0x50081000U

/* L3 RAM, size and num bank definition in sys_common_18xx.h */
#define SOC_XWR18XX_MSS_L3RAM_BASE_ADDRESS           0x51000000U

/* ADC Buffer, size definition in sys_common_18xx.h */
#define SOC_XWR18XX_MSS_ADCBUF_BASE_ADDRESS          0x52000000U

/* CQ Buffer, size definition in sys_common_18xx.h  */
#define SOC_XWR18XX_MSS_CHIRPINFO_BASE_ADDRESS       0x52028000U

/* HWA Memory, size, num banks, window ram size, num param sets
   definitions in sys_common_18xx.h */
#define SOC_XWR18XX_MSS_HWA_MEM0_BASE_ADDRESS        0x52030000U
#define SOC_XWR18XX_MSS_HWA_MEM2_BASE_ADDRESS        0x52038000U

/* HSRAM, size definition in sys_common_18xx.h  */
#define SOC_XWR18XX_MSS_HSRAM_BASE_ADDRESS           0x52080000U

/* EXT FLASH */
#define SOC_XWR18XX_MSS_EXT_FLASH_BASE_ADDRESS       0xC0000000U

/* System Peripheral Registers */
#define SOC_XWR18XX_MSS_QSPI_BASE_ADDRESS             0xC0800000U
#define SOC_XWR18XX_MSS_DMA_2_PKT_BASE_ADDRESS        0xFCF81000U
#define SOC_XWR18XX_MSS_DMA_2_CTRL_BASE_ADDRESS       0xFCFFF800U
#define SOC_XWR18XX_MSS_DTHE_BASE_ADDRESS             0xFD000000U
#define SOC_XWR18XX_MSS_SHA_BASE_ADDRESS              0xFD004000U
#define SOC_XWR18XX_MSS_AES_BASE_ADDRESS              0xFD006000U
#define SOC_XWR18XX_MSS_CRC_BASE_ADDRESS              0xFE000000U
#define SOC_XWR18XX_MSS_MIBSPIA_RAM_BASE_ADDRESS      0xFF0E0000U
#define SOC_XWR18XX_MSS_MIBSPIB_RAM_BASE_ADDRESS      0xFF0C0000U
#define SOC_XWR18XX_MSS_DCAN_MEM_BASE_ADDRESS         0xFF1E0000U
#define SOC_XWR18XX_MSS_MCAN_MEM_BASE_ADDRESS         0xFF500000U
#define SOC_XWR18XX_MSS_GIO_BASE_ADDRESS              0xFFF7BC00U
#define SOC_XWR18XX_MSS_MCAN_BASE_ADDRESS             0xFFF7C800U
#define SOC_XWR18XX_MSS_I2C_BASE_ADDRESS              0xFFF7D400U
#define SOC_XWR18XX_MSS_DCAN_BASE_ADDRESS             0xFFF7DC00U
#define SOC_XWR18XX_MSS_SCI_A_BASE_ADDRESS            0xFFF7E500U
#define SOC_XWR18XX_MSS_SCI_B_BASE_ADDRESS            0xFFF7E700U
#define SOC_XWR18XX_MSS_MIBSPIA_BASE_ADDRESS          0xFFF7F400U
#define SOC_XWR18XX_MSS_MIBSPIB_BASE_ADDRESS          0xFFF7F600U
#define SOC_XWR18XX_MSS_GPIO_BASE_ADDRESS             0xFFF7BC00U
#define SOC_XWR18XX_MSS_DMA_1_PKT_BASE_ADDRESS        0xFFF80000U
#define SOC_XWR18XX_MSS_TOP_RCM_BASE_ADDRESS          0xFFFFE100U
#define SOC_XWR18XX_MSS_PINMUX_BASE_ADDRESS           0xFFFFEA00U
#define SOC_XWR18XX_MSS_RTIB_BASE_ADDRESS             0xFFFFEE00U
#define SOC_XWR18XX_MSS_ESM_BASE_ADDRESS              0xFFFFF500U
#define SOC_XWR18XX_MSS_GPCFG_BASE_ADDRESS            0xFFFFF800U
#define SOC_XWR18XX_MSS_DMA_1_CTRL_BASE_ADDRESS       0xFFFFF000U
#define SOC_XWR18XX_MSS_RCM_BASE_ADDRESS              0xFFFFFF00U
/*Next 4 defines: MSS mailbox base addresses to communicate with BSS*/
#define SOC_XWR18XX_MSS_MBOX_MSS_BSS_REG_BASE_ADDRESS 0xF0608000U
#define SOC_XWR18XX_MSS_MBOX_MSS_BSS_MEM_BASE_ADDRESS 0xF0602000U
#define SOC_XWR18XX_MSS_MBOX_BSS_MSS_REG_BASE_ADDRESS 0xF0608600U
#define SOC_XWR18XX_MSS_MBOX_BSS_MSS_MEM_BASE_ADDRESS 0xF0601000U
/*Next 4 defines: MSS mailbox base addresses to communicate with DSS*/
#define SOC_XWR18XX_MSS_MBOX_MSS_DSS_REG_BASE_ADDRESS 0xF0608400U
#define SOC_XWR18XX_MSS_MBOX_MSS_DSS_MEM_BASE_ADDRESS 0xF0604000U
#define SOC_XWR18XX_MSS_MBOX_DSS_MSS_REG_BASE_ADDRESS 0xF0608300U
#define SOC_XWR18XX_MSS_MBOX_DSS_MSS_MEM_BASE_ADDRESS 0xF0605000U

/** @}*/ //SOC_XWR18XX_MSS_BASE_ADDRESS

/***********************************************************************
 * MSS DMA Request Line Mapping
 ************************************************************************/
/**
 * @defgroup SOC_XWR18XX_DMA_REQLINES     SOC_XWR18XX_DMA_REQLINES
 * @brief    Macros that define values for XWR18xx DMA request lines
 * @{
 */
#define SOC_XWR18XX_MSS_SPIA_CHAN1_DMA_REQ           (0U)   /* SPIA Channle-1 DMA Request Line                      */
#define SOC_XWR18XX_MSS_SPIA_CHAN0_DMA_REQ           (1U)   /* SPIA Channle-0 DMA Request Line                      */
#define SOC_XWR18XX_MSS_SPIB_CHAN1_DMA_REQ           (2U)   /* SPIB Channle-1 DMA Request Line                      */
#define SOC_XWR18XX_MSS_SPIB_CHAN0_DMA_REQ           (3U)   /* SPIB Channle-0 DMA Request Line                      */
#define SOC_XWR18XX_MSS_QSPI_DMA_REQ                 (4U)   /* QSPI DMA Request Line                                */
#define SOC_XWR18XX_MSS_SPIA_CHAN3_DMA_REQ           (5U)   /* SPIA Channle-3 DMA Request Line                      */
#define SOC_XWR18XX_MSS_DCAN_IF2_DMA_REQ             (6U)   /* DCAN DMA request assignment For Interface Register 2 */
#define SOC_XWR18XX_MSS_CBUFF_DMA_REQ                (7U)   /* Common buffer DMA Request Line                       */
#define SOC_XWR18XX_MSS_DCAN_IF1_DMA_REQ             (8U)   /* DCAN DMA request assignment For Interface Register 1 */
#define SOC_XWR18XX_MSS_SPIA_CHAN5_DMA_REQ           (9U)   /* SPIA Channle-5 DMA Request Line                      */
#define SOC_XWR18XX_MSS_I2C_RX_DMA_REQ               (10U)  /* Channel ID for I2C RX DMA Request                    */
#define SOC_XWR18XX_MSS_I2C_TX_DMA_REQ               (11U)  /* Channel ID for I2C TX DMA Request                    */
#define SOC_XWR18XX_MSS_RTI_COMP0_DMA_REQ            (12U)  /* RTI DMA request  for Compare 0                       */
#define SOC_XWR18XX_MSS_RTI_COMP1_DMA_REQ            (13U)  /* RTI DMA request  for Compare 1                       */
#define SOC_XWR18XX_MSS_DCAN_IF3_DMA_REQ             (16U)  /* DCAN DMA request assignment For Interface Register 3 */
#define SOC_XWR18XX_MSS_SPIA_CHAN2_DMA_REQ           (17U)  /* SPIA Channle-2 DMA Request Line                      */
#define SOC_XWR18XX_MSS_RTI_COMP2_DMA_REQ            (18U)  /* RTI DMA request for Compare 2                        */
#define SOC_XWR18XX_MSS_RTI_COMP3_DMA_REQ            (19U)  /* RTI DMA request for Compare 3                        */
#define SOC_XWR18XX_MSS_WDT_DMA_REQ0                 (20U)  /* WatchDog DMA Request 0                               */
#define SOC_XWR18XX_MSS_WDT_DMA_REQ1                 (21U)  /* WatchDog DMA Request 1                               */
#define SOC_XWR18XX_MSS_SPIA_CHAN4_DMA_REQ           (22U)  /* SPIA Channle-4 DMA Request Line                      */
#define SOC_XWR18XX_MSS_EPWM3_DMA_REQ0               (23U)  /* ePWM3 DMA Request Line-0                             */
#define SOC_XWR18XX_MSS_WDT_DMA_REQ2                 (24U)  /* WatchDog DMA Request 2                               */
#define SOC_XWR18XX_MSS_WDT_DMA_REQ3                 (25U)  /* WatchDog DMA Request 3                               */
#define SOC_XWR18XX_MSS_CRC_CH1_DMA_REQ              (26U)  /* MCRC DMA Request For Channel 1                       */
#define SOC_XWR18XX_MSS_CRC_CH2_DMA_REQ              (27U)  /* MCRC DMA Request For Channel 2                       */
#define SOC_XWR18XX_MSS_SCIB_RX_DMA_REQ              (28U)  /* Channel ID for SCI2-B RX DMA Request                 */
#define SOC_XWR18XX_MSS_SCIB_TX_DMA_REQ              (29U)  /* Channel ID for SCI-B TX DMA Request                  */
#define SOC_XWR18XX_MSS_SCIA_RX_DMA_REQ              (30U)  /* Channel ID for SCI-A RX DMA Request                  */
#define SOC_XWR18XX_MSS_SCIA_TX_DMA_REQ              (31U)  /* Channel ID for SCI-A TX DMA Request                  */

#define SOC_XWR18XX_MSS_GIO0_DMA_REQ                 (32U)  /* GIO-0 DMA Request                                    */
#define SOC_XWR18XX_MSS_GIO1_DMA_REQ                 (33U)  /* GIO-1 DMA Request                                    */
#define SOC_XWR18XX_MSS_GIO2_DMA_REQ                 (34U)  /* GIO-2 DMA Request                                    */
#define SOC_XWR18XX_MSS_EPWM1_DMA_REQ0               (35U)  /* ePWM1 DMA Request Line-0                             */
#define SOC_XWR18XX_MSS_SPIB_CHAN2_DMA_REQ           (37U)  /* SPIB Channle-2 DMA Request Line                      */
#define SOC_XWR18XX_MSS_SPIB_CHAN3_DMA_REQ           (38U)  /* SPIB Channle-3 DMA Request Line                      */
#define SOC_XWR18XX_MSS_EPWM1_DMA_REQ1               (39U)  /* ePWM1 DMA Request Line-1                             */
#define SOC_XWR18XX_MSS_EPWM2_DMA_REQ1               (40U)  /* ePWM2 DMA Request Line-1                             */
#define SOC_XWR18XX_MSS_EPWM2_DMA_REQ2               (41U)  /* ePWM2 DMA Request Line-2                             */
#define SOC_XWR18XX_MSS_SPIB_CHAN4_DMA_REQ           (42U)  /* SPIB Channle-4 DMA Request Line                      */
#define SOC_XWR18XX_MSS_SPIB_CHAN5_DMA_REQ           (43U)  /* SPIB Channle-5 DMA Request Line                      */
#define SOC_XWR18XX_MSS_EPWM1_DMA_REQ2               (45U)  /* ePWM1 DMA Request Line-2                             */
#define SOC_XWR18XX_MSS_GIO14_DMA_REQ                (46U)  /* GIO-14 DMA Request                                   */
#define SOC_XWR18XX_MSS_GIO15_DMA_REQ                (47U)  /* GIO-15 DMA Request                                   */

#define SOC_XWR18XX_MSS_SHA0_DMA_REQ                 (48U)  /* CRYPTO SHA-0 DMA Request                             */
#define SOC_XWR18XX_MSS_SHA1_DMA_REQ                 (49U)  /* CRYPTO SHA-1 DMA Request                             */
#define SOC_XWR18XX_MSS_SHA2_DMA_REQ                 (50U)  /* CRYPTO SHA-2 DMA Request                             */
#define SOC_XWR18XX_MSS_SHA3_DMA_REQ                 (51U)  /* CRYPTO SHA-3 DMA Request                             */
#define SOC_XWR18XX_MSS_SHA4_DMA_REQ                 (52U)  /* CRYPTO SHA-4 DMA Request                             */
#define SOC_XWR18XX_MSS_SHA5_DMA_REQ                 (53U)  /* CRYPTO SHA-5 DMA Request                             */
#define SOC_XWR18XX_MSS_AES0_DMA_REQ                 (54U)  /* CRYPTO AES-0 DMA Request                             */
#define SOC_XWR18XX_MSS_AES1_DMA_REQ                 (55U)  /* CRYPTO AES-1 DMA Request                             */
#define SOC_XWR18XX_MSS_AES2_DMA_REQ                 (56U)  /* CRYPTO AES-2 DMA Request                             */
#define SOC_XWR18XX_MSS_AES3_DMA_REQ                 (57U)  /* CRYPTO AES-3 DMA Request                             */
#define SOC_XWR18XX_MSS_AES4_DMA_REQ                 (58U)  /* CRYPTO AES-4 DMA Request                             */
#define SOC_XWR18XX_MSS_AES5_DMA_REQ                 (59U)  /* CRYPTO AES-5 DMA Request                             */
#define SOC_XWR18XX_MSS_AES6_DMA_REQ                 (60U)  /* CRYPTO AES-6 DMA Request                             */
#define SOC_XWR18XX_MSS_AES7_DMA_REQ                 (61U)  /* CRYPTO AES-7 DMA Request                             */

#define SOC_XWR18XX_MSS_MACN_CHAN0_DMA_REQ           (62U)  /* MCAN/CAN-FD Channel-0 DMA Request                    */
#define SOC_XWR18XX_MSS_MACN_CHAN1_DMA_REQ           (63U)  /* MCAN/CAN-FD Channel-1 DMA Request                    */

#define SOC_XWR18XX_NUM_DMA_CHANNELS_PER_INSTANCE    (32U)  /* Number of DMA channels per DMA instance              */
#define SOC_XWR18XX_NUM_DMA_REQLINES_PER_INSTANCE    (64U)  /* Number of DMA REQ Lines per DMA instance             */


/** @}*/ //SOC_XWR18XX_DMA_REQLINES


/***********************************************************************
 * MSS VIM Interrupt Mapping
 ************************************************************************/
/**
 * @defgroup SOC_XWR18XX_MSS_INTERRUPTS_MAP     SOC_XWR18XX_MSS_INTERRUPTS_MAP
 * @brief    Macros that define VIM Interrupt Mapping
 * @{
 */

#define SOC_XWR18XX_MSS_ESM_HIGH_PRIORITY_INT      (0U)      /* VIM Line for ESM High Priority Interrupt         */
#define SOC_XWR18XX_MSS_RTI_COMPARE0_INT           (2U)      /* RTI Interrupt 0                                  */
#define SOC_XWR18XX_MSS_RTI_COMPARE1_INT           (3U)      /* RTI Interrupt 1                                  */
#define SOC_XWR18XX_MSS_RTI_COMPARE2_INT           (4U)      /* RTI Interrupt 2                                  */
#define SOC_XWR18XX_MSS_RTI_COMPARE3_INT           (5U)      /* RTI Interrupt 3                                  */
#define SOC_XWR18XX_MSS_RTI_OVRFLW0_INT            (6U)      /* RTI OverFlow Interrupt 0                         */
#define SOC_XWR18XX_MSS_RTI_OVRFLW1_INT            (7U)      /* RTI OverFlow Interrupt 1                         */
#define SOC_XWR18XX_MSS_RTI_TIMEBASE_INT           (8U)      /* Timebase Interrupt Channel                       */
#define SOC_XWR18XX_MSS_GIO_LVL0_INT               (9U)      /* GIO high level Interrupt                         */
#define SOC_XWR18XX_MSS_WDT_REQ0_INT               (10U)     /* WDT Interrupt 0                                  */
#define SOC_XWR18XX_MSS_WDT_REQ1_INT               (11U)     /* WDT Interrupt 1                                  */
#define SOC_XWR18XX_MSS_SPIA_LVL0_INT              (12U)     /* SPIA Interrupt Level 0                           */
#define SOC_XWR18XX_MSS_WDT_REQ2_INT               (13U)     /* WDT Interrupt 2                                  */
#define SOC_XWR18XX_MSS_WDT_REQ3_INT               (14U)     /* WDT Interrupt 3                                  */
#define SOC_XWR18XX_MSS_WDT_OVRFLW0_INT            (15U)     /* WDT OverFlow Interrupt 0                         */
#define SOC_XWR18XX_MSS_DCAN_LVL0_INT              (16U)     /* DCAN Interrupt Level 0                           */
#define SOC_XWR18XX_MSS_SPIB_LVL0_INT              (17U)     /* SPIB Interrupt Level 0                           */
#define SOC_XWR18XX_MSS_GPIO0_INT                  (18U)     /* GPIO Interrupt Req0                              */
#define SOC_XWR18XX_MSS_MCRC_REQ_INT               (19U)     /* MCRC Interrupt Req                               */
#define SOC_XWR18XX_MSS_ESM_LOW_PRIORITY_INT       (20U)     /* VIM Line for ESM Low Priority Interrupt          */
#define SOC_XWR18XX_MSS_SYS_SW4_INT                (21U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR18XX_MSS_PMU_IRQ_INT                (22U)     /* PMUIRQ - PMU IRQ interrupt                       */
#define SOC_XWR18XX_MSS_GIO_LVL1_INT               (23U)     /* GIO high level Interrupt Req                     */
#define SOC_XWR18XX_MSS_WDT_OVRFLW1_INT            (24U)     /* WDT OverFlow Interrupt Req1                      */
#define SOC_XWR18XX_MSS_WDT_TB_INT                 (25U)     /* WDT TB Interrupt Req                             */
#define SOC_XWR18XX_MSS_SPIA_LVL1_INT              (26U)     /* SPIA Interrupt Level 1                           */
#define SOC_XWR18XX_MSS_QSPI_INT_REQ               (27U)     /* QSPI Interrupt                                   */
#define SOC_XWR18XX_MSS_DMM_38_INT                 (28U)     /* DMM-38 Software Interrupt                        */
#define SOC_XWR18XX_MSS_DCAN_LVL1_INT              (29U)     /* CAN Interrupt Req1                               */
#define SOC_XWR18XX_MSS_SPIB_LVL1_INT              (30U)     /* SPIB Interrupt Level 1                           */
#define SOC_XWR18XX_MSS_CRYPTO_SHAS_INT            (31U)     /* CRYPTO SHA-S Interrupt                           */
#define SOC_XWR18XX_MSS_GPIO1_INT                  (32U)     /* GPIO Interrupt Req1                              */
#define SOC_XWR18XX_MSS_DMA1_FTC_INT               (33U)     /* DMA FTC (Frame Transfer Complete) Interrupt      */
#define SOC_XWR18XX_MSS_DMA1_LFS_INT               (34U)     /* DMA LFS (Last Frame Transfer Started) Interrupt  */
#define SOC_XWR18XX_MSS_MCAN_LVL0_INT              (35U)     /* MCAN/CAN-FD interrupt 0                          */
#define SOC_XWR18XX_MSS_DMM_LVL0_INT               (36U)     /* DMM level0 Interrupt                             */
#define SOC_XWR18XX_MSS_CRYPTO_SHAP_INT            (37U)     /* CRYPTO SHA-P Interrupt                           */
#define SOC_XWR18XX_MSS_CRYPTO_TRNG_INT            (38U)     /* CRYPTO TRNG Interrupt                            */
#define SOC_XWR18XX_MSS_DMA1_HBC_INT               (39U)     /* DMA1 HBC (First Half of Block Complete) Interrupt  */
#define SOC_XWR18XX_MSS_DMA1_BTC_INT               (40U)     /* DMA1 BTC (Block Transfer Complete) Interrupt     */
#define SOC_XWR18XX_MSS_DMA2_FTC_INT               (41U)     /* DMA2 FTC (Frame Transfer Complete) Interrupt     */
#define SOC_XWR18XX_MSS_MCAN_LVL1_INT              (42U)     /* MCAN CAN-FD interrupt                            */
#define SOC_XWR18XX_MSS_DMM1_LVL1_INT              (43U)     /* DMM1 level1 Interrupt                            */
#define SOC_XWR18XX_MSS_MCAN_MSG0_INT              (44U)     /* MCAN/CAN-FD Message filter interrupt-0           */
#define SOC_XWR18XX_MSS_DMA2_LFS_INT               (45U)     /* DMA2 Last Frame Complete Interrupt               */
#define SOC_XWR18XX_MSS_MCAN_MSG1_INT              (46U)     /* MCAN/CAN-FD Message filter interrupt-1           */
#define SOC_XWR18XX_MSS_FPU_INT                    (47U)     /* Floating point unit Interrupt                    */
#define SOC_XWR18XX_MSS_GPIO2_INT                  (48U)     /* GPIO Interrupt Req2                              */
#define SOC_XWR18XX_MSS_DMA2_HBC_INT               (49U)     /* DMA2 HBC (First Half of Block Complete) Interrupt  */
#define SOC_XWR18XX_MSS_DMA2_BTC_INT               (50U)     /* DMA2 BTC (Block Transfer Complete) Interrupt     */
#define SOC_XWR18XX_MSS_DMA2_BUS_ERR_INT           (51U)     /* DMA2 Bus Error Interrupt                         */
#define SOC_XWR18XX_MSS_DSS2MSS_SW1_INT            (52U)     /* DSS to MSS Software triggered Interrupt-2        */
#define SOC_XWR18XX_MSS_CRYPTO_PKA_INT             (53U)     /* PKA module interrupt                             */
#define SOC_XWR18XX_MSS_CRYPTO_AESS_INT            (54U)     /* AES-S module interrupt                           */
#define SOC_XWR18XX_MSS_MCAN_MSG2_INT              (55U)     /* MCAN/CAN-FD Message filter interrupt-2           */
#define SOC_XWR18XX_MSS_CRYPTO_AESP_INT            (56U)     /* AES-P module interrupt                           */
#define SOC_XWR18XX_MSS_DMM2_LVL0_INT              (57U)     /* DMM2 level0 Interrupt                            */
#define SOC_XWR18XX_MSS_DMM2_LVL1_INT              (58U)     /* DMM2 level1 Interrupt                            */
#define SOC_XWR18XX_MSS_MBOX_MSS_DSS_BOX_FULL_INT  (59U)     /* DSS to MSS Mailbox full Interrupt                */
#define SOC_XWR18XX_MSS_MBOX_MSS_DSS_BOX_EMPTY_INT (60U)     /* DSS to MSS Mailbox empty Interrupt               */
#define SOC_XWR18XX_MSS_DSS2MSS_SW2_INT            (61U)     /* DSS to MSS Software triggered Interrupt-2        */
#define SOC_XWR18XX_MSS_DEBUGSS_INT                (62U)     /* DebugSS Interrupt                                */
#define SOC_XWR18XX_MSS_GEM_STC_INT                (63U)     /* GEM STC done Interrupt                           */
#define SOC_XWR18XX_MSS_SCIA_LVL0_INT              (64U)     /* UARTA Interrupt Req0                             */
#define SOC_XWR18XX_MSS_SCIB_LVL0_INT              (65U)     /* UARTB Interrupt Req0                             */
#define SOC_XWR18XX_MSS_I2C_LVL0_INT               (66U)     /* I2C Level 0 Interrupt                            */
#define SOC_XWR18XX_MSS_DMM_34_INT                 (67U)     /* DMM interrupt 34                                 */
#define SOC_XWR18XX_MSS_DMM_35_INT                 (68U)     /* DMM interrupt 35                                 */
#define SOC_XWR18XX_MSS_DMM_36_INT                 (69U)     /* DMM interrupt 36                                 */
#define SOC_XWR18XX_MSS_DMA1_BUS_ERR_INT           (70U)     /* DMA1 BUS error Interrupt                         */
#define SOC_XWR18XX_MSS_DMM_30_INT                 (71U)     /* DMM interrupt 30                                 */
#define SOC_XWR18XX_MSS_DMM_33_INT                 (73U)     /* DMM interrupt 33                                 */
#define SOC_XWR18XX_MSS_SCIA_LVL1_INT              (74U)     /* UARTA Interrupt Req1                             */
#define SOC_XWR18XX_MSS_SCIB_LVL1_INT              (75U)     /* UARTB Interrupt Req1                             */
#define SOC_XWR18XX_MSS_SYS_SW0_INT                (76U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR18XX_MSS_SYS_SW1_INT                (77U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR18XX_MSS_SYS_SW2_INT                (78U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR18XX_MSS_SYS_SW3_INT                (79U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR18XX_MSS_CCCA_DONE_INT              (80U)     /* CCC-A Done Interrupt                             */
#define SOC_XWR18XX_MSS_CCCB_DONE_INT              (81U)     /* CCC-B Done Interrupt                             */
#define SOC_XWR18XX_MSS_DCCA_DONE_INT              (82U)     /* DCC-A Done Interrupt                             */
#define SOC_XWR18XX_MSS_DCCB_DONE_INT              (83U)     /* DCC-B Done Interrupt                             */
#define SOC_XWR18XX_MSS_SYS_SW5_INT                (84U)     /* VIM Line for System SW Interrupt                 */
#define SOC_XWR18XX_MSS_PBIST_IRQ_INT              (85U)     /* PBIST IRQ Interrupt                              */
#define SOC_XWR18XX_MSS_DMM_32_INT                 (86U)     /* DMM interrupt 32                                 */

#define SOC_XWR18XX_MSS_MBOX_MSS_BSS_BOX_FULL_INT  (95U)     /* software Interrupt request-0 from BSS to MSS       */
#define SOC_XWR18XX_MSS_MBOX_MSS_BSS_BOX_EMPTY_INT (96U)     /* software Interrupt request-1 from BSS to MSS     */
#define SOC_XWR18XX_MSS_ADC_VALID_INT              (97U)     /* ADC valid Fall Interrupt                         */

/* The following share same interrupt line */
#define SOC_XWR18XX_MSS_DMM_29_INT                 (98U)     /* DMM Interrupt 29 Interrupt                       */
#define SOC_XWR18XX_MSS_FRAME_START_INT            (98U)     /* Frame Start Interrupt                            */

#define SOC_XWR18XX_MSS_CHIRP_START_INT            (99U)     /* Chirp Start Interrupt                            */
#define SOC_XWR18XX_MSS_CHIRP_END_INT              (100U)    /* Chirp End Interrupt                              */
#define SOC_XWR18XX_MSS_FRAME_END_INT              (101U)    /* Frame End Interrupt                              */
#define SOC_XWR18XX_MSS_EPWM1_1_INT                (104U)    /* ePWM1 interrupt-1                                */
#define SOC_XWR18XX_MSS_STC_DONE_INT               (105U)    /* STC Done Interrupt                               */
#define SOC_XWR18XX_MSS_EPWM1_2_INT                (107U)    /* ePWM1 interrupt-2                                */
#define SOC_XWR18XX_MSS_EPWM2_1_INT                (108U)    /* ePWM2 interrupt-1                                */
#define SOC_XWR18XX_MSS_EPWM2_2_INT                (109U)    /* ePWM2 interrupt-2                                */
#define SOC_XWR18XX_MSS_EPWM3_1_INT                (110U)    /* ePWM3 interrupt-1                                */
#define SOC_XWR18XX_MSS_EPWM3_2_INT                (111U)    /* ePWM3 interrupt-2                                */
#define SOC_XWR18XX_MSS_EDMA_TPTC0_DONE_INT        (112U)    /* TPTC-0 Interrupt                                 */
#define SOC_XWR18XX_MSS_EDMA_TPTC0_ERROR_INT       (113U)    /* TPTC-0 Error  Interrupt                          */
#define SOC_XWR18XX_MSS_EDMA_TPTC1_DONE_INT        (114U)    /* TPTC-1 Interrupt                                 */
#define SOC_XWR18XX_MSS_EDMA_TPTC1_ERROR_INT       (115U)    /* TPTC-1 Error Interrupt                           */
#define SOC_XWR18XX_MSS_EDMA_TPCC0_DONE_INT        (116U)    /* TPCC-0 Interrupt                                 */
#define SOC_XWR18XX_MSS_EDMA_TPCC0_ERROR_INT       (117U)    /* TPCC-0 Error Interrupt                           */
#define SOC_XWR18XX_MSS_CBUFF_INT                  (118U)    /* Common Buffer Interrupt                          */
#define SOC_XWR18XX_MSS_CBUFF_ERR_INT              (120U)    /* Common Buffer Error Interrupt                    */
#define SOC_XWR18XX_MSS_DMM_37_INT                 (121U)    /* DMM interrupt 37                                 */
#define SOC_XWR18XX_DSPSS_FRAME_START_INT          (122U)    /* Frame start Interrupt routed to DSPSS            */
#define SOC_XWR18XX_MSS_CHIRP_AVAIL_IRQ            (123U)    /* Chirp Available Interrupt                        */
#define SOC_XWR18XX_DSPSS_PBIST_DONE_INT           (124U)    /* DSPSS PBIST done Interrupt                       */

#define SOC_XWR18XX_DSS_HW_ACC_PARAM_DONE_IRQ  (125U)        /* HWA param done interrupt                         */
#define SOC_XWR18XX_DSS_HW_ACC_DONE_IRQ            (126U)    /* HWA done Interrupt                               */
#define SOC_XWR18XX_DSS_HW_ACC_ERR_IRQ             (127U)    /* HWA access error Interrupt                       */

 /** @}*/ //SOC_XWR18XX_MSS_INTERRUPTS_MAP

/*
 * MSS ESM Interrupt mapping
 */
/* Group 1 Errors */
#define SOC_XWR18XX_MSS_ANA_LIMP_MODE_ESM                   (63U)
#define SOC_XWR18XX_MSS_DCCB_ERR_ESM                        (62U)
#define SOC_XWR18XX_MSS_MAILBOX_BSS2MSS_FATAL_ERR_ESM       (61U)
#define SOC_XWR18XX_MSS_MAILBOX_BSS2MSS_REPAIR_ERR_ESM      (60U)
#define SOC_XWR18XX_MSS_MAILBOX_MSS2BSS_FATAL_ERR_ESM       (59U)
#define SOC_XWR18XX_MSS_MAILBOX_MSS2BSS_REPAIR_ERR_ESM      (58U)
#define SOC_XWR18XX_MSS_BSS_CRITICAL_ERR_ESM                (57U)
#define SOC_XWR18XX_MSS_BSS_TO_MSS_HI_ESM                   (56U)
#define SOC_XWR18XX_MSS_CLOCK_SUPPLY_ERR_ESM                (55U)
#define SOC_XWR18XX_MSS_BSS_TO_MSS_LO_ESM                   (54U)
#define SOC_XWR18XX_MSS_MAILBOX_DSS2MSS_FATAL_ESM           (53U)
#define SOC_XWR18XX_MSS_MAILBOX_DSS2MSS_REPAIR_ESM          (52U)
#define SOC_XWR18XX_MSS_MCAN_MEM_FATAL_ERR_ESM              (51U)
#define SOC_XWR18XX_MSS_MCAN_MEM_REPAIR_ERR_ESM             (50U)
#define SOC_XWR18XX_MSS_MIBSPIB_MEM_FATAL_ERR_ESM           (49U)
#define SOC_XWR18XX_MSS_MCRC_ERR_ESM                        (48U)
#define SOC_XWR18XX_MSS_MCAN_EXT_TIMESTAMP_ERR_ESM          (47U)
#define SOC_XWR18XX_MSS_MIBSPIB_MEM_REPAIR_ERR              (45U)
#define SOC_XWR18XX_MSS_MAILBOX_MSS2DSS_FATAL_ERR           (43U)
#define SOC_XWR18XX_MSS_MAILBOX_MSS2DSS_REPAIR_ERR          (42U)
#define SOC_XWR18XX_MSS_DSS_ESM_GP1_ERR_ESM                 (41U)
#define SOC_XWR18XX_MSS_DSS_CBUFF_SAFETY_ERR_ESM            (38U)
#define SOC_XWR18XX_MSS_DSS_GP2_ERR_ESM                     (37U)
#define SOC_XWR18XX_MSS_DSS_TPTC1_WR_MPU_ERR_ESM            (36U)
#define SOC_XWR18XX_MSS_DSS_TPTC1_RD_MPU_ERR_ESM            (35U)
#define SOC_XWR18XX_MSS_HVMODE_ERR_ESM                      (34U)
#define SOC_XWR18XX_MSS_DCAN_RAM_REPAIR_ERR_ESM             (33U)
#define SOC_XWR18XX_MSS_TCMA_REPAIR_ERR_ESM                 (32U)
#define SOC_XWR18XX_MSS_DCCA_ERR_ESM                        (30U)
#define SOC_XWR18XX_MSS_DSS_TPTC0_WR_MPU_ERR_ESM            (29U)
#define SOC_XWR18XX_MSS_TCMB1_REPAIR_ESM                    (28U)
#define SOC_XWR18XX_MSS_STC_ERR_ESM                         (27U)
#define SOC_XWR18XX_MSS_TCMB0_REPAIR_ESM                    (26U)
#define SOC_XWR18XX_MSS_MIBSPIA_MEM_REPAIR_ERR_ESM          (25U)
#define SOC_XWR18XX_MSS_DMA2_MEM_PARITY_ERR_ESM             (24U)
#define SOC_XWR18XX_MSS_DMA2_MPU_ERR_ESM                    (23U)
#define SOC_XWR18XX_MSS_FRC_COMP_ERR_ESM                    (22U)
#define SOC_XWR18XX_MSS_DCAN_RAM_FATAL_ERR_ESM              (21U)
#define SOC_XWR18XX_MSS_VIM_RAM_REPAIR_ERR_ESM              (20U)
#define SOC_XWR18XX_DSS_TPTC0_RD_MPU_ERR_ESM                (18U)
#define SOC_XWR18XX_MSS_MIBSPIA_MEM_FATAL_ERR_ESM           (17U)
#define SOC_XWR18XX_MSS_SECURE_RAM_FATAL_ERR_ESM            (16U)
#define SOC_XWR18XX_MSS_VIM_RAM_FATAL_ERR_ESM               (15U)
#define SOC_XWR18XX_MSS_SECURE_RAM_REPAIR_ERR_ESM           (14U)
#define SOC_XWR18XX_MSS_MAILBOX_BSS2DSS_FATAL_ERR_ESM       (12U)
#define SOC_XWR18XX_MSS_MAILBOX_BSS2DSS_REPAIR_ERR_ESM      (11U)
#define SOC_XWR18XX_MSS_MAILBOX_DSS2BSS_FATAL_ESM           (10U)
#define SOC_XWR18XX_MSS_CBUFF_ECC_FATAL_ERR_ESM             (9U)
#define SOC_XWR18XX_MSS_CBUFF_ECC_REPAIR_ERR_ESM            (8U)
#define SOC_XWR18XX_MSS_TPCC_PARITY_ERR_ESM                 (7U)
#define SOC_XWR18XX_MSS_MAILBOX_DSS2BSS_REPAIR_ERR_ESM      (5U)
#define SOC_XWR18XX_MSS_CCCB_ERR_ESM                        (4U)
#define SOC_XWR18XX_MSS_DMA_MEM_PARITY_ERR_ESM              (3U)
#define SOC_XWR18XX_MSS_DMA_MPU_ERR_ESM                     (2U)
#define SOC_XWR18XX_MSS_CCCA_ERR_ESM                        (1U)
#define SOC_XWR18XX_MSS_NERROR_PAD_IN_ESM                   (0U)

/* Group 2 Errors */
#define SOC_XWR18XX_MSS_WDT_NMI_REQ_ESM              (24U)
#define SOC_XWR18XX_MSS_MSS_CR4_LIVELOCK_ESM         (16U)
#define SOC_XWR18XX_MSS_TCMB1_PARITY_ERR_ESM         (8U )
#define SOC_XWR18XX_MSS_TCMB0_PARITY_ERR_ESM         (6U )
#define SOC_XWR18XX_MSS_TCMA_PARITY_ERR_ESM          (4U )

/* Group 3 Errors */
#define SOC_XWR18XX_MSS_TCMA_UNCORR_ERR_ESM          (7U )
#define SOC_XWR18XX_MSS_TCMB1_UNCORR_ERR_ESM         (5U )
#define SOC_XWR18XX_MSS_TCMB0_UNCORR_ERR_ESM         (3U )
#define SOC_XWR18XX_MSS_EFC_AUTOLOAD_ERROR_ESM       (1U )


/***********************************************************************
 * MSS - CLOCK settings
 ***********************************************************************/
 /* Sys_vclk : 200MHz */
#define MSS_SYS_VCLK                  200000000U
#define R4F_CLOCK_MHZ                 200U

/***********************************************************************
 * MSS - Peripheral number of instance definition
 ***********************************************************************/
#define DMA_NUM_INSTANCES            (2U)


/*! @brief R4F to Hardware Accelerator address translation macro. */
#define ADDR_TRANSLATE_CPU_TO_HWA(x)  (uint16_t)(((uint32_t)(x) - SOC_XWR18XX_MSS_HWA_MEM0_BASE_ADDRESS) & 0x0000FFFFU)

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
/* Note: Number of instances of CC, TC, param sets are defined in sys_common_xwr18xx.h */
#define EDMA_CC0_TRANSFER_COMPLETE_INTR_ID   SOC_XWR18XX_MSS_EDMA_TPCC0_DONE_INT
#define EDMA_CC0_ERRROR_INTR_ID              SOC_XWR18XX_MSS_EDMA_TPCC0_ERROR_INT
#define EDMA_CC0_TC0_ERRROR_INTR_ID          SOC_XWR18XX_MSS_EDMA_TPTC0_ERROR_INT
#define EDMA_CC0_TC1_ERRROR_INTR_ID          SOC_XWR18XX_MSS_EDMA_TPTC1_ERROR_INT

/** @}*/ /* end defgroup EDMA_HW_DEFS */

#endif /* #if (defined(SOC_XWR18XX) && defined(SUBSYS_MSS) ) */

#ifdef __cplusplus
}
#endif

#endif /* SYS_COMMON_XWR18XX_MSS_H */
