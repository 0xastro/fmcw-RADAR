/**
 *   @file  sys_common_xwr18xx.h
 *
 *   @brief
 *      This is the common header file used by the various mmWave SDK
 *      modules for XWR18XX device.
 *
 *  \par
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

#ifndef SYS_COMMON_XWR18XX_H
#define SYS_COMMON_XWR18XX_H

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(SOC_XWR18XX))

#ifdef SUBSYS_MSS
#include <ti/common/sys_common_xwr18xx_mss.h>
#endif

#ifdef SUBSYS_DSS
#include <ti/common/sys_common_xwr18xx_dss.h>
#endif

/***********************************************************************
 * Peripheral number of instance definition
 ***********************************************************************/
#define HWA_NUM_INSTANCES            (1U)

/*************************************************************
 * BASE Address for the various module as seen by EDMA3
 *************************************************************/
#define EDMA3_DSS_ADCBUFF_BASE        0x21000000U
#define EDMA3_DSS_FIFO_BASE           0x21020000U
#define EDMA3_DSS_CQ0BUFF_BASE        0x21028000U
#define EDMA3_DSS_CQ1BUFF_BASE        0x21028200U
#define EDMA3_DSS_CQ2BUFF_BASE        0x21028400U

/*************************************************************
 * BASE Address used in the Single Chirp Mode to get the
 * Chirp Profile Data
 *************************************************************/
#define EDMA3_DSS_CP0_BASE            0x21070020U
#define EDMA3_DSS_CP1_BASE            0x21070030U
#define EDMA3_DSS_CP2_BASE            0x21070040U
#define EDMA3_DSS_CP3_BASE            0x21070050U

/*************************************************************
 * BASE Address used in the Multiple Chirp Mode to get the
 * Chirp Profile Data
 *************************************************************/
#define EDMA3_DSS_CHIRP0_BASE         0x21070100U
#define EDMA3_DSS_CHIRP1_BASE         0x21070140U
#define EDMA3_DSS_CHIRP2_BASE         0x21070180U
#define EDMA3_DSS_CHIRP3_BASE         0x210701C0U
#define EDMA3_DSS_CHIRP4_BASE         0x21070200U
#define EDMA3_DSS_CHIRP5_BASE         0x21070240U
#define EDMA3_DSS_CHIRP6_BASE         0x21070280U
#define EDMA3_DSS_CHIRP7_BASE         0x210702C0U

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
/* NOTE: EDMA numInstance, base address, interrupt number are defined in
 *       sys_common_xwr18xx_mss/sys_common_xwr18xx_dss.h */
#define EDMA_NUM_CC                          (2U)
#define EDMA_NUM_TC_IN_CC0                   (2U)

/*! Note even though EDMA's CCCFG register indicates 6 TCs for CC1, only first 2
 *  are verified for radar processing flows, hence we limit to 2 TCs. */
#define EDMA_NUM_TC_IN_CC1                   (2U)

#define EDMA_NUM_PARAM_SETS_CC0              (128U)
#define EDMA_NUM_PARAM_SETS_CC1              (256U)

#define EDMA_MAX_NUM_TC                      MAX(EDMA_NUM_TC_IN_CC0, EDMA_NUM_TC_IN_CC1)

/*! @brief EDMA event interrupt mapping for CC 0 */
#define EDMA_TPCC0_REQ_CBUFF_0    (0U)
#define EDMA_TPCC0_REQ_CBUFF_1    (1U)
#define EDMA_TPCC0_REQ_CBUFF_2    (2U)
#define EDMA_TPCC0_REQ_CBUFF_3    (3U)
#define EDMA_TPCC0_REQ_CBUFF_4    (4U)
#define EDMA_TPCC0_REQ_CBUFF_5    (5U)
#define EDMA_TPCC0_REQ_CBUFF_6    (6U)

#define EDMA_TPCC0_REQ_FREE_0    (7U)

#define EDMA_TPCC0_REQ_DFE_FRAME_START  (8U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_39    (8U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_9     (8U)

#define EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL  (9U)
#define EDMA_TPCC0_REQ_DMM_SW_INTR_11   (9U)
#define EDMA_TPCC0_REQ_DMM_SW_INTR_43   (9U)

#define EDMA_TPCC0_REQ_FREE_1     (10U)
#define EDMA_TPCC0_REQ_FREE_2     (11U)
#define EDMA_TPCC0_REQ_FREE_3     (12U)
#define EDMA_TPCC0_REQ_FREE_4     (13U)
#define EDMA_TPCC0_REQ_FREE_5     (14U)
#define EDMA_TPCC0_REQ_FREE_6     (15U)
#define EDMA_TPCC0_REQ_FREE_7     (16U)

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

#define EDMA_TPCC0_REQ_FREE_8     (33U)
#define EDMA_TPCC0_REQ_FREE_9     (34U)
#define EDMA_TPCC0_REQ_FREE_10    (35U)
#define EDMA_TPCC0_REQ_FREE_11    (36U)
#define EDMA_TPCC0_REQ_FREE_12    (37U)
#define EDMA_TPCC0_REQ_FREE_13    (38U)
#define EDMA_TPCC0_REQ_FREE_14    (39U)

#define EDMA_TPCC0_REQ_FRC_LOGICAL_FRAME_START  (40U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_10            (40U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_40            (40U)

#define EDMA_TPCC0_REQ_ADC_VALID_FALL  (41U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_12   (41U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_44   (41U)

#define EDMA_TPCC0_REQ_UART_RX  (42U)
#define EDMA_TPCC0_REQ_UART_TX  (43U)

#define EDMA_TPCC0_REQ_DMMSW_INTR_13  (44U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_14  (45U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_15  (46U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_16  (47U)
#define EDMA_TPCC0_REQ_DMMSW_INTR_17  (48U)

#define EDMA_TPCC0_REQ_GPIO_0  (49U)
#define EDMA_TPCC0_REQ_GPIO_1  (50U)
#define EDMA_TPCC0_REQ_GPIO_2  (51U)

#define EDMA_TPCC0_REQ_RTI1_0  (52U)
#define EDMA_TPCC0_REQ_RTI1_1  (53U)
#define EDMA_TPCC0_REQ_RTI1_2  (54U)
#define EDMA_TPCC0_REQ_RTI1_3  (55U)

#define EDMA_TPCC0_REQ_RTI2_0  (56U)
#define EDMA_TPCC0_REQ_RTI2_1  (57U)
#define EDMA_TPCC0_REQ_RTI2_2  (58U)
#define EDMA_TPCC0_REQ_RTI2_3  (59U)

#define EDMA_TPCC0_REQ_FREE_15  (60U)
#define EDMA_TPCC0_REQ_FREE_16  (61U)
#define EDMA_TPCC0_REQ_FREE_17  (62U)

#define EDMA_TPCC0_REQ_DMMSW_INTR_18  (63U)

/*! @brief EDMA event interrupt mapping for CC 1 */
#define EDMA_TPCC1_REQ_CBUFF_0    (0U)
#define EDMA_TPCC1_REQ_CBUFF_1    (1U)
#define EDMA_TPCC1_REQ_CBUFF_2    (2U)
#define EDMA_TPCC1_REQ_CBUFF_3    (3U)
#define EDMA_TPCC1_REQ_CBUFF_4    (4U)
#define EDMA_TPCC1_REQ_CBUFF_5    (5U)
#define EDMA_TPCC1_REQ_CBUFF_6    (6U)

#define EDMA_TPCC1_REQ_FREE_0    (7U)

#define EDMA_TPCC1_REQ_DFE_FRAME_START  (8U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_39    (8U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_19    (8U)

#define EDMA_TPCC1_REQ_DFE_CHIRP_AVAIL  (9U)
#define EDMA_TPCC1_REQ_DMM_SW_INTR_21   (9U)
#define EDMA_TPCC1_REQ_DMM_SW_INTR_43   (9U)

#define EDMA_TPCC1_REQ_FREE_1     (10U)
#define EDMA_TPCC1_REQ_FREE_2     (11U)
#define EDMA_TPCC1_REQ_FREE_3     (12U)
#define EDMA_TPCC1_REQ_FREE_4     (13U)
#define EDMA_TPCC1_REQ_FREE_5     (14U)
#define EDMA_TPCC1_REQ_FREE_6     (15U)
#define EDMA_TPCC1_REQ_FREE_7     (16U)
#define EDMA_TPCC1_REQ_FREE_8     (17U)
#define EDMA_TPCC1_REQ_FREE_9     (18U)
#define EDMA_TPCC1_REQ_FREE_10    (19U)
#define EDMA_TPCC1_REQ_FREE_11    (20U)
#define EDMA_TPCC1_REQ_FREE_12    (21U)
#define EDMA_TPCC1_REQ_FREE_13    (22U)
#define EDMA_TPCC1_REQ_FREE_14    (23U)
#define EDMA_TPCC1_REQ_FREE_15    (24U)
#define EDMA_TPCC1_REQ_FREE_16    (25U)
#define EDMA_TPCC1_REQ_FREE_17    (26U)
#define EDMA_TPCC1_REQ_FREE_18    (27U)
#define EDMA_TPCC1_REQ_FREE_19    (28U)
#define EDMA_TPCC1_REQ_FREE_20    (29U)
#define EDMA_TPCC1_REQ_FREE_21    (30U)
#define EDMA_TPCC1_REQ_FREE_22    (31U)
#define EDMA_TPCC1_REQ_FREE_23    (32U)

#define EDMA_TPCC1_REQ_MCRC_0     (33U)
#define EDMA_TPCC1_REQ_MCRC_1     (34U)

#define EDMA_TPCC1_REQ_FRC_0      (35U)
#define EDMA_TPCC1_REQ_FRC_1      (36U)
#define EDMA_TPCC1_REQ_FRC_2      (37U)
#define EDMA_TPCC1_REQ_FRC_3      (38U)

#define EDMA_TPCC1_REQ_FREE_24    (39U)

#define EDMA_TPCC1_REQ_FRC_LOGICAL_FRAME_START  (40U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_20            (40U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_40            (40U)

#define EDMA_TPCC1_REQ_ADC_VALID_FALL  (41U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_22   (41U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_44   (41U)

#define EDMA_TPCC1_REQ_UART_RX  (42U)
#define EDMA_TPCC1_REQ_UART_TX  (43U)

#define EDMA_TPCC1_REQ_DMMSW_INTR_23  (44U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_24  (45U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_25  (46U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_26  (47U)
#define EDMA_TPCC1_REQ_DMMSW_INTR_27  (48U)

#define EDMA_TPCC1_REQ_GPIO_0  (49U)
#define EDMA_TPCC1_REQ_GPIO_1  (50U)
#define EDMA_TPCC1_REQ_GPIO_2  (51U)

#define EDMA_TPCC1_REQ_RTI1_0  (52U)
#define EDMA_TPCC1_REQ_RTI1_1  (53U)
#define EDMA_TPCC1_REQ_RTI1_2  (54U)
#define EDMA_TPCC1_REQ_RTI1_3  (55U)

#define EDMA_TPCC1_REQ_RTI2_0  (56U)
#define EDMA_TPCC1_REQ_RTI2_1  (57U)
#define EDMA_TPCC1_REQ_RTI2_2  (58U)
#define EDMA_TPCC1_REQ_RTI2_3  (59U)

#define EDMA_TPCC1_REQ_FREE_25  (60U)
#define EDMA_TPCC1_REQ_FREE_26  (61U)
#define EDMA_TPCC1_REQ_FREE_27  (62U)

#define EDMA_TPCC1_REQ_DMMSW_INTR_28  (63U)

/** @}*/ /* end defgroup EDMA_HW_DEFS */

#define SYS_COMMON_NUM_TX_ANTENNAS  3U
#define SYS_COMMON_TX_ELEV_ANTENNA_MASK      2U
#define SYS_COMMON_TX_AZIM_ANTENNA_MASK      5U

/* System Memory sizes, address are in corresponding _dss/_mss.h files */
#define SOC_L3RAM_SIZE                  (MMWAVE_L3RAM_NUM_BANK * MMWAVE_SHMEM_BANK_SIZE)
#define SOC_L3RAM_NUM_BANK              MMWAVE_L3RAM_NUM_BANK
#define SOC_ADCBUF_SIZE                 0x8000U        /* Size: 32KB */
#define SOC_CHIRPINFO_SIZE              0x2000U        /* Size: 8KB */
#define SOC_HWA_MEM_SIZE                (65536U)
#define SOC_HWA_NUM_MEM_BANKS           (4U)
#define SOC_HWA_WINDOW_RAM_SIZE_IN_SAMPLES (1024U)
#define SOC_HWA_NUM_PARAM_SETS          (16U)
#define SOC_HSRAM_SIZE                  0x8000U        /* Size: 32KB */

#endif /* #if (defined(SOC_XWR18XX)) */

#ifdef __cplusplus
}
#endif

#endif /* SYS_COMMON_XWR18XX_H */
