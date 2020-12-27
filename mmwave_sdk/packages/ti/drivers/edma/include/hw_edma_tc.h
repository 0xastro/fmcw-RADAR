/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2014
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
 *
 */

/**
*
*  \file    hw_edma_tc.h
*
*  \brief   register-level header file for EDMA_TC
*
**/

#ifndef HW_EDMA_TC_H_
#define HW_EDMA_TC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************************************************
* Register Definitions
****************************************************************************************************/
#define EDMA_TC_PID                                                                                        (0x0U)
#define EDMA_TC_TCCFG                                                                                      (0x4U)
#define EDMA_TC_SYSCONFIG                                                                                  (0x10U)
#define EDMA_TC_TCSTAT                                                                                     (0x100U)
#define EDMA_TC_INTSTAT                                                                                    (0x104U)
#define EDMA_TC_INTEN                                                                                      (0x108U)
#define EDMA_TC_INTCLR                                                                                     (0x10cU)
#define EDMA_TC_INTCMD                                                                                     (0x110U)
#define EDMA_TC_ERRSTAT                                                                                    (0x120U)
#define EDMA_TC_ERREN                                                                                      (0x124U)
#define EDMA_TC_ERRCLR                                                                                     (0x128U)
#define EDMA_TC_ERRDET                                                                                     (0x12cU)
#define EDMA_TC_ERRCMD                                                                                     (0x130U)
#define EDMA_TC_RDRATE                                                                                     (0x140U)
#define EDMA_TC_POPT                                                                                       (0x0U)
#define EDMA_TC_PSRC                                                                                       (0x4U)
#define EDMA_TC_PCNT                                                                                       (0x8U)
#define EDMA_TC_PDST                                                                                       (0xcU)
#define EDMA_TC_PBIDX                                                                                      (0x10U)
#define EDMA_TC_PMPPRXY                                                                                    (0x14U)
#define EDMA_TC_SAOPT                                                                                      (0x240U)
#define EDMA_TC_SASRC                                                                                      (0x244U)
#define EDMA_TC_SACNT                                                                                      (0x248U)
#define EDMA_TC_SADST                                                                                      (0x24cU)
#define EDMA_TC_SABIDX                                                                                     (0x250U)
#define EDMA_TC_SAMPPRXY                                                                                   (0x254U)
#define EDMA_TC_SACNTRLD                                                                                   (0x258U)
#define EDMA_TC_SASRCBREF                                                                                  (0x25cU)
#define EDMA_TC_SADSTBREF                                                                                  (0x260U)
#define EDMA_TC_DFCNTRLD                                                                                   (0x280U)
#define EDMA_TC_DFSRCBREF                                                                                  (0x284U)
#define EDMA_TC_DFDSTBREF                                                                                  (0x288U)
#define EDMA_TC_DFOPT(n)                                                                                   (0x300U + ((n) * 64))
#define EDMA_TC_DFSRC(n)                                                                                   (0x304U + ((n) * 64))
#define EDMA_TC_DFCNT(n)                                                                                   (0x308U + ((n) * 64))
#define EDMA_TC_DFDST(n)                                                                                   (0x30cU + ((n) * 64))
#define EDMA_TC_DFBIDX(n)                                                                                  (0x310U + ((n) * 64))
#define EDMA_TC_DFMPPRXY(n)                                                                                (0x314U + ((n) * 64))

/****************************************************************************************************
* Field Definition Macros
****************************************************************************************************/

#define EDMA_TC_PID_MINOR_SHIFT                                                                           (0U)
#define EDMA_TC_PID_MINOR_MASK                                                                            (0x0000003fU)

#define EDMA_TC_PID_MAJOR_SHIFT                                                                           (8U)
#define EDMA_TC_PID_MAJOR_MASK                                                                            (0x00000700U)

#define EDMA_TC_PID_CUSTOM_SHIFT                                                                          (6U)
#define EDMA_TC_PID_CUSTOM_MASK                                                                           (0x000000c0U)

#define EDMA_TC_PID_FUNC_SHIFT                                                                            (16U)
#define EDMA_TC_PID_FUNC_MASK                                                                             (0x0fff0000U)

#define EDMA_TC_PID_RTL_SHIFT                                                                             (11U)
#define EDMA_TC_PID_RTL_MASK                                                                              (0x0000f800U)

#define EDMA_TC_PID_SCHEME_SHIFT                                                                          (30U)
#define EDMA_TC_PID_SCHEME_MASK                                                                           (0xc0000000U)

#define EDMA_TC_PID_RESERVED_SHIFT                                                                        (28U)
#define EDMA_TC_PID_RESERVED_MASK                                                                         (0x30000000U)

#define EDMA_TC_TCCFG_FIFOSIZE_SHIFT                                                                      (0U)
#define EDMA_TC_TCCFG_FIFOSIZE_MASK                                                                       (0x00000007U)
#define EDMA_TC_TCCFG_FIFOSIZE_FIFO_128_BYTE                                                              (2U)
#define EDMA_TC_TCCFG_FIFOSIZE_FIFO_512_BYTE                                                              (4U)
#define EDMA_TC_TCCFG_FIFOSIZE_FIFO_64_BYTE                                                               (1U)
#define EDMA_TC_TCCFG_FIFOSIZE_FIFO_256_BYTE                                                              (3U)
#define EDMA_TC_TCCFG_FIFOSIZE_FIFO_32_BYTE                                                               (0U)

#define EDMA_TC_TCCFG_DREGDEPTH_SHIFT                                                                     (8U)
#define EDMA_TC_TCCFG_DREGDEPTH_MASK                                                                      (0x00000300U)
#define EDMA_TC_TCCFG_DREGDEPTH_FIFODEPTH_1                                                               (0U)
#define EDMA_TC_TCCFG_DREGDEPTH_FIFODEPTH_2                                                               (1U)
#define EDMA_TC_TCCFG_DREGDEPTH_FIFODEPTH_4                                                               (2U)

#define EDMA_TC_TCCFG_BUSWIDTH_SHIFT                                                                      (4U)
#define EDMA_TC_TCCFG_BUSWIDTH_MASK                                                                       (0x00000030U)
#define EDMA_TC_TCCFG_BUSWIDTH_BW_64BIT                                                                   (1U)
#define EDMA_TC_TCCFG_BUSWIDTH_BW_32BIT                                                                   (0U)
#define EDMA_TC_TCCFG_BUSWIDTH_BW_128BIT                                                                  (2U)

#define EDMA_TC_TCCFG_RESERVED_SHIFT                                                                      (3U)
#define EDMA_TC_TCCFG_RESERVED_MASK                                                                       (0x00000008U)

#define EDMA_TC_TCCFG_RESERVED1_SHIFT                                                                     (6U)
#define EDMA_TC_TCCFG_RESERVED1_MASK                                                                      (0x000000c0U)

#define EDMA_TC_TCCFG_RESERVED2_SHIFT                                                                     (10U)
#define EDMA_TC_TCCFG_RESERVED2_MASK                                                                      (0xfffffc00U)

#define EDMA_TC_SYSCONFIG_IDLEMODE_SHIFT                                                                  (2U)
#define EDMA_TC_SYSCONFIG_IDLEMODE_MASK                                                                   (0x0000000cU)
#define EDMA_TC_SYSCONFIG_IDLEMODE_FORCE_IDLE                                                              (0U)
#define EDMA_TC_SYSCONFIG_IDLEMODE_NO_IDLE                                                                 (1U)
#define EDMA_TC_SYSCONFIG_IDLEMODE_SMART_IDLE                                                              (2U)
#define EDMA_TC_SYSCONFIG_IDLEMODE_RESERVED                                                                (3U)

#define EDMA_TC_SYSCONFIG_STANDBYMODE_SHIFT                                                               (4U)
#define EDMA_TC_SYSCONFIG_STANDBYMODE_MASK                                                                (0x00000030U)
#define EDMA_TC_SYSCONFIG_STANDBYMODE_FORCE_STANDBY                                                        (0U)
#define EDMA_TC_SYSCONFIG_STANDBYMODE_NO_STANDBY                                                           (1U)
#define EDMA_TC_SYSCONFIG_STANDBYMODE_SMART_STANDBY                                                        (2U)
#define EDMA_TC_SYSCONFIG_STANDBYMODE_RESERVED                                                             (3U)

#define EDMA_TC_TCSTAT_DFSTRTPTR_SHIFT                                                                    (12U)
#define EDMA_TC_TCSTAT_DFSTRTPTR_MASK                                                                     (0x00003000U)

#define EDMA_TC_TCSTAT_SRCACTV_SHIFT                                                                      (1U)
#define EDMA_TC_TCSTAT_SRCACTV_MASK                                                                       (0x00000002U)

#define EDMA_TC_TCSTAT_PROGBUSY_SHIFT                                                                     (0U)
#define EDMA_TC_TCSTAT_PROGBUSY_MASK                                                                      (0x00000001U)

#define EDMA_TC_TCSTAT_DSTACTV_SHIFT                                                                      (4U)
#define EDMA_TC_TCSTAT_DSTACTV_MASK                                                                       (0x00000070U)
#define EDMA_TC_TCSTAT_DSTACTV_FIFOISEMPTY                                                                (0U)
#define EDMA_TC_TCSTAT_DSTACTV_FIFO_1_TR                                                                  (1U)
#define EDMA_TC_TCSTAT_DSTACTV_FIFO_3_TR                                                                  (3U)
#define EDMA_TC_TCSTAT_DSTACTV_FIFO_4_TR                                                                  (4U)
#define EDMA_TC_TCSTAT_DSTACTV_FIFO_2_TR                                                                  (2U)

#define EDMA_TC_TCSTAT_WSACTV_SHIFT                                                                       (2U)
#define EDMA_TC_TCSTAT_WSACTV_MASK                                                                        (0x00000004U)

#define EDMA_TC_TCSTAT_ACTV_SHIFT                                                                         (8U)
#define EDMA_TC_TCSTAT_ACTV_MASK                                                                          (0x00000100U)

#define EDMA_TC_TCSTAT_RESERVED_0_SHIFT                                                                   (14U)
#define EDMA_TC_TCSTAT_RESERVED_0_MASK                                                                    (0xffffc000U)

#define EDMA_TC_TCSTAT_RESERVED_1_SHIFT                                                                   (9U)
#define EDMA_TC_TCSTAT_RESERVED_1_MASK                                                                    (0x00000e00U)

#define EDMA_TC_TCSTAT_RESERVED_2_SHIFT                                                                   (7U)
#define EDMA_TC_TCSTAT_RESERVED_2_MASK                                                                    (0x00000080U)

#define EDMA_TC_TCSTAT_RESERVED_3_SHIFT                                                                   (3U)
#define EDMA_TC_TCSTAT_RESERVED_3_MASK                                                                    (0x00000008U)

#define EDMA_TC_INTSTAT_PROGEMPTY_SHIFT                                                                   (0U)
#define EDMA_TC_INTSTAT_PROGEMPTY_MASK                                                                    (0x00000001U)

#define EDMA_TC_INTSTAT_TRDONE_SHIFT                                                                      (1U)
#define EDMA_TC_INTSTAT_TRDONE_MASK                                                                       (0x00000002U)

#define EDMA_TC_INTSTAT_RESERVED_0_SHIFT                                                                  (2U)
#define EDMA_TC_INTSTAT_RESERVED_0_MASK                                                                   (0xfffffffcU)

#define EDMA_TC_INTEN_TRDONE_SHIFT                                                                        (1U)
#define EDMA_TC_INTEN_TRDONE_MASK                                                                         (0x00000002U)

#define EDMA_TC_INTEN_PROGEMPTY_SHIFT                                                                     (0U)
#define EDMA_TC_INTEN_PROGEMPTY_MASK                                                                      (0x00000001U)

#define EDMA_TC_INTEN_RESERVED_0_SHIFT                                                                    (2U)
#define EDMA_TC_INTEN_RESERVED_0_MASK                                                                     (0xfffffffcU)

#define EDMA_TC_INTCLR_PROGEMPTY_SHIFT                                                                    (0U)
#define EDMA_TC_INTCLR_PROGEMPTY_MASK                                                                     (0x00000001U)

#define EDMA_TC_INTCLR_TRDONE_SHIFT                                                                       (1U)
#define EDMA_TC_INTCLR_TRDONE_MASK                                                                        (0x00000002U)

#define EDMA_TC_INTCLR_RESERVED_0_SHIFT                                                                   (2U)
#define EDMA_TC_INTCLR_RESERVED_0_MASK                                                                    (0xfffffffcU)

#define EDMA_TC_INTCMD_EVAL_SHIFT                                                                         (0U)
#define EDMA_TC_INTCMD_EVAL_MASK                                                                          (0x00000001U)

#define EDMA_TC_INTCMD_SET_SHIFT                                                                          (1U)
#define EDMA_TC_INTCMD_SET_MASK                                                                           (0x00000002U)

#define EDMA_TC_INTCMD_RESERVED_0_SHIFT                                                                   (2U)
#define EDMA_TC_INTCMD_RESERVED_0_MASK                                                                    (0xfffffffcU)

#define EDMA_TC_ERRSTAT_BUSERR_SHIFT                                                                      (0U)
#define EDMA_TC_ERRSTAT_BUSERR_MASK                                                                       (0x00000001U)

#define EDMA_TC_ERRSTAT_TRERR_SHIFT                                                                       (2U)
#define EDMA_TC_ERRSTAT_TRERR_MASK                                                                        (0x00000004U)

#define EDMA_TC_ERRSTAT_MMRAERR_SHIFT                                                                     (3U)
#define EDMA_TC_ERRSTAT_MMRAERR_MASK                                                                      (0x00000008U)

#define EDMA_TC_ERRSTAT_RESERVED_0_SHIFT                                                                  (4U)
#define EDMA_TC_ERRSTAT_RESERVED_0_MASK                                                                   (0xfffffff0U)

#define EDMA_TC_ERRSTAT_RESERVED_1_SHIFT                                                                  (1U)
#define EDMA_TC_ERRSTAT_RESERVED_1_MASK                                                                   (0x00000002U)

#define EDMA_TC_ERREN_TRERR_SHIFT                                                                         (2U)
#define EDMA_TC_ERREN_TRERR_MASK                                                                          (0x00000004U)

#define EDMA_TC_ERREN_BUSERR_SHIFT                                                                        (0U)
#define EDMA_TC_ERREN_BUSERR_MASK                                                                         (0x00000001U)

#define EDMA_TC_ERREN_MMRAERR_SHIFT                                                                       (3U)
#define EDMA_TC_ERREN_MMRAERR_MASK                                                                        (0x00000008U)

#define EDMA_TC_ERREN_RESERVED_0_SHIFT                                                                    (4U)
#define EDMA_TC_ERREN_RESERVED_0_MASK                                                                     (0xfffffff0U)

#define EDMA_TC_ERREN_RESERVED_1_SHIFT                                                                    (1U)
#define EDMA_TC_ERREN_RESERVED_1_MASK                                                                     (0x00000002U)

#define EDMA_TC_ERRCLR_MMRAERR_SHIFT                                                                      (3U)
#define EDMA_TC_ERRCLR_MMRAERR_MASK                                                                       (0x00000008U)

#define EDMA_TC_ERRCLR_TRERR_SHIFT                                                                        (2U)
#define EDMA_TC_ERRCLR_TRERR_MASK                                                                         (0x00000004U)

#define EDMA_TC_ERRCLR_BUSERR_SHIFT                                                                       (0U)
#define EDMA_TC_ERRCLR_BUSERR_MASK                                                                        (0x00000001U)

#define EDMA_TC_ERRCLR_RESERVED_0_SHIFT                                                                   (4U)
#define EDMA_TC_ERRCLR_RESERVED_0_MASK                                                                    (0xfffffff0U)

#define EDMA_TC_ERRCLR_RESERVED_1_SHIFT                                                                   (1U)
#define EDMA_TC_ERRCLR_RESERVED_1_MASK                                                                    (0x00000002U)

#define EDMA_TC_ERRDET_TCCHEN_SHIFT                                                                       (17U)
#define EDMA_TC_ERRDET_TCCHEN_MASK                                                                        (0x00020000U)

#define EDMA_TC_ERRDET_TCINTEN_SHIFT                                                                      (16U)
#define EDMA_TC_ERRDET_TCINTEN_MASK                                                                       (0x00010000U)

#define EDMA_TC_ERRDET_TCC_SHIFT                                                                          (8U)
#define EDMA_TC_ERRDET_TCC_MASK                                                                           (0x00003f00U)

#define EDMA_TC_ERRDET_STAT_SHIFT                                                                         (0U)
#define EDMA_TC_ERRDET_STAT_MASK                                                                          (0x0000000fU)
#define EDMA_TC_ERRDET_STAT_NOERROR1                                                                      (0U)
#define EDMA_TC_ERRDET_STAT_WRITEEXERR                                                                    (15U)
#define EDMA_TC_ERRDET_STAT_READADDRERR                                                                   (1U)
#define EDMA_TC_ERRDET_STAT_WRITEADDRERR                                                                  (9U)
#define EDMA_TC_ERRDET_STAT_WRITEDATAERR                                                                  (12U)
#define EDMA_TC_ERRDET_STAT_WRITEPRIVERR                                                                  (10U)
#define EDMA_TC_ERRDET_STAT_READPRIVERR                                                                   (2U)
#define EDMA_TC_ERRDET_STAT_READTIMEOUTERR                                                                (3U)
#define EDMA_TC_ERRDET_STAT_NOERROR2                                                                      (8U)
#define EDMA_TC_ERRDET_STAT_WRITETIMEOUTERR                                                               (11U)
#define EDMA_TC_ERRDET_STAT_READDATAERR                                                                   (4U)
#define EDMA_TC_ERRDET_STAT_READEXERR                                                                     (7U)

#define EDMA_TC_ERRDET_RESERVED_0_SHIFT                                                                   (18U)
#define EDMA_TC_ERRDET_RESERVED_0_MASK                                                                    (0xfffc0000U)

#define EDMA_TC_ERRDET_RESERVED_1_SHIFT                                                                   (14U)
#define EDMA_TC_ERRDET_RESERVED_1_MASK                                                                    (0x0000c000U)

#define EDMA_TC_ERRDET_RESERVED_2_SHIFT                                                                   (4U)
#define EDMA_TC_ERRDET_RESERVED_2_MASK                                                                    (0x000000f0U)

#define EDMA_TC_ERRCMD_SET_SHIFT                                                                          (1U)
#define EDMA_TC_ERRCMD_SET_MASK                                                                           (0x00000002U)

#define EDMA_TC_ERRCMD_EVAL_SHIFT                                                                         (0U)
#define EDMA_TC_ERRCMD_EVAL_MASK                                                                          (0x00000001U)

#define EDMA_TC_ERRCMD_RESERVED_0_SHIFT                                                                   (2U)
#define EDMA_TC_ERRCMD_RESERVED_0_MASK                                                                    (0xfffffffcU)

#define EDMA_TC_RDRATE_RDRATE_SHIFT                                                                       (0U)
#define EDMA_TC_RDRATE_RDRATE_MASK                                                                        (0x00000007U)
#define EDMA_TC_RDRATE_RDRATE_READEVERY32CYCLES                                                           (4U)
#define EDMA_TC_RDRATE_RDRATE_READEVERY16CYCLES                                                           (3U)
#define EDMA_TC_RDRATE_RDRATE_AFAP                                                                        (0U)
#define EDMA_TC_RDRATE_RDRATE_READEVERY8CYCLES                                                            (2U)
#define EDMA_TC_RDRATE_RDRATE_READEVERY4CYCLES                                                            (1U)

#define EDMA_TC_RDRATE_RESERVED_0_SHIFT                                                                   (3U)
#define EDMA_TC_RDRATE_RESERVED_0_MASK                                                                    (0xfffffff8U)

#define EDMA_TC_POPT_FWID_SHIFT                                                                           (8U)
#define EDMA_TC_POPT_FWID_MASK                                                                            (0x00000700U)
#define EDMA_TC_POPT_FWID_FIFOWIDTH128BIT                                                                 (4U)
#define EDMA_TC_POPT_FWID_FIFOWIDTH16BIT                                                                  (1U)
#define EDMA_TC_POPT_FWID_FIFOWIDTH32BIT                                                                  (2U)
#define EDMA_TC_POPT_FWID_FIFOWIDTH64BIT                                                                  (3U)
#define EDMA_TC_POPT_FWID_FIFOWIDTH8BIT                                                                   (0U)
#define EDMA_TC_POPT_FWID_FIFOWIDTH256BIT                                                                 (5U)

#define EDMA_TC_POPT_TCC_SHIFT                                                                            (12U)
#define EDMA_TC_POPT_TCC_MASK                                                                             (0x0003f000U)

#define EDMA_TC_POPT_DAM_SHIFT                                                                            (1U)
#define EDMA_TC_POPT_DAM_MASK                                                                             (0x00000002U)

#define EDMA_TC_POPT_PRI_SHIFT                                                                            (4U)
#define EDMA_TC_POPT_PRI_MASK                                                                             (0x00000070U)
#define EDMA_TC_POPT_PRI_PRIORITY0                                                                        (0U)
#define EDMA_TC_POPT_PRI_PRIORITY3                                                                        (3U)
#define EDMA_TC_POPT_PRI_PRIORITY4                                                                        (4U)
#define EDMA_TC_POPT_PRI_PRIORITY1                                                                        (1U)
#define EDMA_TC_POPT_PRI_PRIORITY2                                                                        (2U)
#define EDMA_TC_POPT_PRI_PRIORITY7                                                                        (7U)
#define EDMA_TC_POPT_PRI_PRIORITY5                                                                        (5U)
#define EDMA_TC_POPT_PRI_PRIORITY6                                                                        (6U)

#define EDMA_TC_POPT_TCINTEN_SHIFT                                                                        (20U)
#define EDMA_TC_POPT_TCINTEN_MASK                                                                         (0x00100000U)

#define EDMA_TC_POPT_TCCHEN_SHIFT                                                                         (22U)
#define EDMA_TC_POPT_TCCHEN_MASK                                                                          (0x00400000U)

#define EDMA_TC_POPT_SAM_SHIFT                                                                            (0U)
#define EDMA_TC_POPT_SAM_MASK                                                                             (0x00000001U)

#define EDMA_TC_POPT_RESERVED_0_SHIFT                                                                     (23U)
#define EDMA_TC_POPT_RESERVED_0_MASK                                                                      (0xff800000U)

#define EDMA_TC_POPT_RESERVED_1_SHIFT                                                                     (21U)
#define EDMA_TC_POPT_RESERVED_1_MASK                                                                      (0x00200000U)

#define EDMA_TC_POPT_RESERVED_2_SHIFT                                                                     (18U)
#define EDMA_TC_POPT_RESERVED_2_MASK                                                                      (0x000c0000U)

#define EDMA_TC_POPT_RESERVED_3_SHIFT                                                                     (11U)
#define EDMA_TC_POPT_RESERVED_3_MASK                                                                      (0x00000800U)

#define EDMA_TC_POPT_RESERVED_4_SHIFT                                                                     (7U)
#define EDMA_TC_POPT_RESERVED_4_MASK                                                                      (0x00000080U)

#define EDMA_TC_POPT_RESERVED_5_SHIFT                                                                     (2U)
#define EDMA_TC_POPT_RESERVED_5_MASK                                                                      (0x0000000cU)

#define EDMA_TC_PSRC_SADDR_SHIFT                                                                          (0U)
#define EDMA_TC_PSRC_SADDR_MASK                                                                           (0xffffffffU)

#define EDMA_TC_PCNT_BCNT_SHIFT                                                                           (16U)
#define EDMA_TC_PCNT_BCNT_MASK                                                                            (0xffff0000U)

#define EDMA_TC_PCNT_ACNT_SHIFT                                                                           (0U)
#define EDMA_TC_PCNT_ACNT_MASK                                                                            (0x0000ffffU)

#define EDMA_TC_PDST_DADDR_SHIFT                                                                          (0U)
#define EDMA_TC_PDST_DADDR_MASK                                                                           (0xffffffffU)

#define EDMA_TC_PBIDX_SBIDX_SHIFT                                                                         (0U)
#define EDMA_TC_PBIDX_SBIDX_MASK                                                                          (0x0000ffffU)

#define EDMA_TC_PBIDX_DBIDX_SHIFT                                                                         (16U)
#define EDMA_TC_PBIDX_DBIDX_MASK                                                                          (0xffff0000U)

#define EDMA_TC_PMPPRXY_SECURE_SHIFT                                                                      (9U)
#define EDMA_TC_PMPPRXY_SECURE_MASK                                                                       (0x00000200U)

#define EDMA_TC_PMPPRXY_PRIV_SHIFT                                                                        (8U)
#define EDMA_TC_PMPPRXY_PRIV_MASK                                                                         (0x00000100U)

#define EDMA_TC_PMPPRXY_PRIVID_SHIFT                                                                      (0U)
#define EDMA_TC_PMPPRXY_PRIVID_MASK                                                                       (0x0000000fU)

#define EDMA_TC_PMPPRXY_RESERVED_0_SHIFT                                                                  (10U)
#define EDMA_TC_PMPPRXY_RESERVED_0_MASK                                                                   (0xfffffc00U)

#define EDMA_TC_PMPPRXY_RESERVED_1_SHIFT                                                                  (4U)
#define EDMA_TC_PMPPRXY_RESERVED_1_MASK                                                                   (0x000000f0U)

#define EDMA_TC_SAOPT_TCCHEN_SHIFT                                                                        (22U)
#define EDMA_TC_SAOPT_TCCHEN_MASK                                                                         (0x00400000U)

#define EDMA_TC_SAOPT_TCINTEN_SHIFT                                                                       (20U)
#define EDMA_TC_SAOPT_TCINTEN_MASK                                                                        (0x00100000U)

#define EDMA_TC_SAOPT_DAM_SHIFT                                                                           (1U)
#define EDMA_TC_SAOPT_DAM_MASK                                                                            (0x00000002U)

#define EDMA_TC_SAOPT_SAM_SHIFT                                                                           (0U)
#define EDMA_TC_SAOPT_SAM_MASK                                                                            (0x00000001U)

#define EDMA_TC_SAOPT_TCC_SHIFT                                                                           (12U)
#define EDMA_TC_SAOPT_TCC_MASK                                                                            (0x0003f000U)

#define EDMA_TC_SAOPT_FWID_SHIFT                                                                          (8U)
#define EDMA_TC_SAOPT_FWID_MASK                                                                           (0x00000700U)
#define EDMA_TC_SAOPT_FWID_FIFOWIDTH8BIT                                                                  (0U)
#define EDMA_TC_SAOPT_FWID_FIFOWIDTH256BIT                                                                (5U)
#define EDMA_TC_SAOPT_FWID_FIFOWIDTH64BIT                                                                 (3U)
#define EDMA_TC_SAOPT_FWID_FIFOWIDTH32BIT                                                                 (2U)
#define EDMA_TC_SAOPT_FWID_FIFOWIDTH128BIT                                                                (4U)
#define EDMA_TC_SAOPT_FWID_FIFOWIDTH16BIT                                                                 (1U)

#define EDMA_TC_SAOPT_PRI_SHIFT                                                                           (4U)
#define EDMA_TC_SAOPT_PRI_MASK                                                                            (0x00000070U)
#define EDMA_TC_SAOPT_PRI_PRIORITY2                                                                       (2U)
#define EDMA_TC_SAOPT_PRI_PRIORITY1                                                                       (1U)
#define EDMA_TC_SAOPT_PRI_PRIORITY4                                                                       (4U)
#define EDMA_TC_SAOPT_PRI_PRIORITY7                                                                       (7U)
#define EDMA_TC_SAOPT_PRI_PRIORITY3                                                                       (3U)
#define EDMA_TC_SAOPT_PRI_PRIORITY5                                                                       (5U)
#define EDMA_TC_SAOPT_PRI_PRIORITY0                                                                       (0U)
#define EDMA_TC_SAOPT_PRI_PRIORITY6                                                                       (6U)

#define EDMA_TC_SAOPT_RESERVED_5_SHIFT                                                                    (2U)
#define EDMA_TC_SAOPT_RESERVED_5_MASK                                                                     (0x0000000cU)

#define EDMA_TC_SAOPT_RESERVED_4_SHIFT                                                                    (7U)
#define EDMA_TC_SAOPT_RESERVED_4_MASK                                                                     (0x00000080U)

#define EDMA_TC_SAOPT_RESERVED_3_SHIFT                                                                    (11U)
#define EDMA_TC_SAOPT_RESERVED_3_MASK                                                                     (0x00000800U)

#define EDMA_TC_SAOPT_RESERVED_2_SHIFT                                                                    (18U)
#define EDMA_TC_SAOPT_RESERVED_2_MASK                                                                     (0x000c0000U)

#define EDMA_TC_SAOPT_RESERVED_1_SHIFT                                                                    (21U)
#define EDMA_TC_SAOPT_RESERVED_1_MASK                                                                     (0x00200000U)

#define EDMA_TC_SAOPT_RESERVED_0_SHIFT                                                                    (23U)
#define EDMA_TC_SAOPT_RESERVED_0_MASK                                                                     (0xff800000U)

#define EDMA_TC_SASRC_SADDR_SHIFT                                                                         (0U)
#define EDMA_TC_SASRC_SADDR_MASK                                                                          (0xffffffffU)

#define EDMA_TC_SACNT_ACNT_SHIFT                                                                          (0U)
#define EDMA_TC_SACNT_ACNT_MASK                                                                           (0x0000ffffU)

#define EDMA_TC_SACNT_BCNT_SHIFT                                                                          (16U)
#define EDMA_TC_SACNT_BCNT_MASK                                                                           (0xffff0000U)

#define EDMA_TC_SADST_DADDR_SHIFT                                                                         (0U)
#define EDMA_TC_SADST_DADDR_MASK                                                                          (0xffffffffU)

#define EDMA_TC_SABIDX_DBIDX_SHIFT                                                                        (16U)
#define EDMA_TC_SABIDX_DBIDX_MASK                                                                         (0xffff0000U)

#define EDMA_TC_SABIDX_SBIDX_SHIFT                                                                        (0U)
#define EDMA_TC_SABIDX_SBIDX_MASK                                                                         (0x0000ffffU)

#define EDMA_TC_SAMPPRXY_PRIV_SHIFT                                                                       (8U)
#define EDMA_TC_SAMPPRXY_PRIV_MASK                                                                        (0x00000100U)

#define EDMA_TC_SAMPPRXY_PRIVID_SHIFT                                                                     (0U)
#define EDMA_TC_SAMPPRXY_PRIVID_MASK                                                                      (0x0000000fU)

#define EDMA_TC_SAMPPRXY_SECURE_SHIFT                                                                     (9U)
#define EDMA_TC_SAMPPRXY_SECURE_MASK                                                                      (0x00000200U)

#define EDMA_TC_SAMPPRXY_RESERVED_0_SHIFT                                                                 (10U)
#define EDMA_TC_SAMPPRXY_RESERVED_0_MASK                                                                  (0xfffffc00U)

#define EDMA_TC_SAMPPRXY_RESERVED_1_SHIFT                                                                 (4U)
#define EDMA_TC_SAMPPRXY_RESERVED_1_MASK                                                                  (0x000000f0U)

#define EDMA_TC_SACNTRLD_ACNTRLD_SHIFT                                                                    (0U)
#define EDMA_TC_SACNTRLD_ACNTRLD_MASK                                                                     (0x0000ffffU)

#define EDMA_TC_SACNTRLD_RESERVED_0_SHIFT                                                                 (16U)
#define EDMA_TC_SACNTRLD_RESERVED_0_MASK                                                                  (0xffff0000U)

#define EDMA_TC_SASRCBREF_SADDRBREF_SHIFT                                                                 (0U)
#define EDMA_TC_SASRCBREF_SADDRBREF_MASK                                                                  (0xffffffffU)

#define EDMA_TC_SADSTBREF_DADDRBREF_SHIFT                                                                 (0U)
#define EDMA_TC_SADSTBREF_DADDRBREF_MASK                                                                  (0xffffffffU)

#define EDMA_TC_DFCNTRLD_ACNTRLD_SHIFT                                                                    (0U)
#define EDMA_TC_DFCNTRLD_ACNTRLD_MASK                                                                     (0x0000ffffU)

#define EDMA_TC_DFCNTRLD_RESERVED_0_SHIFT                                                                 (16U)
#define EDMA_TC_DFCNTRLD_RESERVED_0_MASK                                                                  (0xffff0000U)

#define EDMA_TC_DFSRCBREF_SADDRBREF_SHIFT                                                                 (0U)
#define EDMA_TC_DFSRCBREF_SADDRBREF_MASK                                                                  (0xffffffffU)

#define EDMA_TC_DFDSTBREF_DADDRBREF_SHIFT                                                                 (0U)
#define EDMA_TC_DFDSTBREF_DADDRBREF_MASK                                                                  (0xffffffffU)

#define EDMA_TC_DFOPT_TCCHEN_SHIFT                                                                        (22U)
#define EDMA_TC_DFOPT_TCCHEN_MASK                                                                         (0x00400000U)

#define EDMA_TC_DFOPT_FWID_SHIFT                                                                          (8U)
#define EDMA_TC_DFOPT_FWID_MASK                                                                           (0x00000700U)
#define EDMA_TC_DFOPT_FWID_FIFOWIDTH64BIT                                                                 (3U)
#define EDMA_TC_DFOPT_FWID_FIFOWIDTH256BIT                                                                (5U)
#define EDMA_TC_DFOPT_FWID_FIFOWIDTH16BIT                                                                 (1U)
#define EDMA_TC_DFOPT_FWID_FIFOWIDTH128BIT                                                                (4U)
#define EDMA_TC_DFOPT_FWID_FIFOWIDTH32BIT                                                                 (2U)
#define EDMA_TC_DFOPT_FWID_FIFOWIDTH8BIT                                                                  (0U)

#define EDMA_TC_DFOPT_TCC_SHIFT                                                                           (12U)
#define EDMA_TC_DFOPT_TCC_MASK                                                                            (0x0003f000U)

#define EDMA_TC_DFOPT_PRI_SHIFT                                                                           (4U)
#define EDMA_TC_DFOPT_PRI_MASK                                                                            (0x00000070U)
#define EDMA_TC_DFOPT_PRI_PRIORITY0                                                                       (0U)
#define EDMA_TC_DFOPT_PRI_PRIORITY1                                                                       (1U)
#define EDMA_TC_DFOPT_PRI_PRIORITY5                                                                       (5U)
#define EDMA_TC_DFOPT_PRI_PRIORITY4                                                                       (4U)
#define EDMA_TC_DFOPT_PRI_PRIORITY2                                                                       (2U)
#define EDMA_TC_DFOPT_PRI_PRIORITY3                                                                       (3U)
#define EDMA_TC_DFOPT_PRI_PRIORITY6                                                                       (6U)
#define EDMA_TC_DFOPT_PRI_PRIORITY7                                                                       (7U)

#define EDMA_TC_DFOPT_DAM_SHIFT                                                                           (1U)
#define EDMA_TC_DFOPT_DAM_MASK                                                                            (0x00000002U)

#define EDMA_TC_DFOPT_SAM_SHIFT                                                                           (0U)
#define EDMA_TC_DFOPT_SAM_MASK                                                                            (0x00000001U)

#define EDMA_TC_DFOPT_TCINTEN_SHIFT                                                                       (20U)
#define EDMA_TC_DFOPT_TCINTEN_MASK                                                                        (0x00100000U)

#define EDMA_TC_DFOPT_RESERVED_0_SHIFT                                                                    (23U)
#define EDMA_TC_DFOPT_RESERVED_0_MASK                                                                     (0xff800000U)

#define EDMA_TC_DFOPT_RESERVED_1_SHIFT                                                                    (21U)
#define EDMA_TC_DFOPT_RESERVED_1_MASK                                                                     (0x00200000U)

#define EDMA_TC_DFOPT_RESERVED_2_SHIFT                                                                    (18U)
#define EDMA_TC_DFOPT_RESERVED_2_MASK                                                                     (0x000c0000U)

#define EDMA_TC_DFOPT_RESERVED_3_SHIFT                                                                    (11U)
#define EDMA_TC_DFOPT_RESERVED_3_MASK                                                                     (0x00000800U)

#define EDMA_TC_DFOPT_RESERVED_4_SHIFT                                                                    (7U)
#define EDMA_TC_DFOPT_RESERVED_4_MASK                                                                     (0x00000080U)

#define EDMA_TC_DFOPT_RESERVED_5_SHIFT                                                                    (2U)
#define EDMA_TC_DFOPT_RESERVED_5_MASK                                                                     (0x0000000cU)

#define EDMA_TC_DFSRC_SADDR_SHIFT                                                                         (0U)
#define EDMA_TC_DFSRC_SADDR_MASK                                                                          (0xffffffffU)

#define EDMA_TC_DFCNT_ACNT_SHIFT                                                                          (0U)
#define EDMA_TC_DFCNT_ACNT_MASK                                                                           (0x0000ffffU)

#define EDMA_TC_DFCNT_BCNT_SHIFT                                                                          (16U)
#define EDMA_TC_DFCNT_BCNT_MASK                                                                           (0xffff0000U)

#define EDMA_TC_DFDST_DADDR_SHIFT                                                                         (0U)
#define EDMA_TC_DFDST_DADDR_MASK                                                                          (0xffffffffU)

#define EDMA_TC_DFBIDX_DBIDX_SHIFT                                                                        (16U)
#define EDMA_TC_DFBIDX_DBIDX_MASK                                                                         (0xffff0000U)

#define EDMA_TC_DFBIDX_SBIDX_SHIFT                                                                        (0U)
#define EDMA_TC_DFBIDX_SBIDX_MASK                                                                         (0x0000ffffU)

#define EDMA_TC_DFMPPRXY_PRIV_SHIFT                                                                       (8U)
#define EDMA_TC_DFMPPRXY_PRIV_MASK                                                                        (0x00000100U)

#define EDMA_TC_DFMPPRXY_PRIVID_SHIFT                                                                     (0U)
#define EDMA_TC_DFMPPRXY_PRIVID_MASK                                                                      (0x0000000fU)

#define EDMA_TC_DFMPPRXY_SECURE_SHIFT                                                                     (9U)
#define EDMA_TC_DFMPPRXY_SECURE_MASK                                                                      (0x00000200U)

#define EDMA_TC_DFMPPRXY_RESERVED_0_SHIFT                                                                 (10U)
#define EDMA_TC_DFMPPRXY_RESERVED_0_MASK                                                                  (0xfffffc00U)

#define EDMA_TC_DFMPPRXY_RESERVED_1_SHIFT                                                                 (4U)
#define EDMA_TC_DFMPPRXY_RESERVED_1_MASK                                                                  (0x000000f0U)

#ifdef __cplusplus
}
#endif
#endif  /* _HW_EDMA_TC_H_ */
