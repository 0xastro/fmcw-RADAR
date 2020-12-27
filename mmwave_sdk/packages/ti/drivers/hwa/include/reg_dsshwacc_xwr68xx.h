/**
 *  @file    reg_dsshwacc_xwr68xx.h
 *
 *  @brief
 *    This file gives register definitions of DSS_HW_ACC module.
 *
 *  This file is auto-generated on 7/30/2018.
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

#ifndef REG_DSSHWACC_XWR68XX_H
#define REG_DSSHWACC_XWR68XX_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field ACCENABLE in Register HWACCREG1 */
#define HWACCREG1_ACCENABLE_BIT_START                       0U
#define HWACCREG1_ACCENABLE_BIT_END                         2U

/* Definition for field ACCCLKEN in Register HWACCREG1 */
#define HWACCREG1_ACCCLKEN_BIT_START                        3U
#define HWACCREG1_ACCCLKEN_BIT_END                          3U

/* Definition for field ACCRESET in Register HWACCREG1 */
#define HWACCREG1_ACCRESET_BIT_START                        4U
#define HWACCREG1_ACCRESET_BIT_END                          6U

/* Definition for field NU2 in Register HWACCREG1 */
#define HWACCREG1_NU2_BIT_START                             7U
#define HWACCREG1_NU2_BIT_END                               7U

/* Definition for field NLOOPS in Register HWACCREG1 */
#define HWACCREG1_NLOOPS_BIT_START                          8U
#define HWACCREG1_NLOOPS_BIT_END                            19U

/* Definition for field PARAMSTART in Register HWACCREG1 */
#define HWACCREG1_PARAMSTART_BIT_START                      20U
#define HWACCREG1_PARAMSTART_BIT_END                        23U

/* Definition for field PARAMSTOP in Register HWACCREG1 */
#define HWACCREG1_PARAMSTOP_BIT_START                       24U
#define HWACCREG1_PARAMSTOP_BIT_END                         27U

/* Definition for field FFT1DEN in Register HWACCREG1 */
#define HWACCREG1_FFT1DEN_BIT_START                         28U
#define HWACCREG1_FFT1DEN_BIT_END                           28U

/* Definition for field NU in Register HWACCREG1 */
#define HWACCREG1_NU_BIT_START                              29U
#define HWACCREG1_NU_BIT_END                                31U

/* Definition for field DMA2ACCTRIG in Register HWACCREG2 */
#define HWACCREG2_DMA2ACCTRIG_BIT_START                     0U
#define HWACCREG2_DMA2ACCTRIG_BIT_END                       15U

/* Definition for field NU in Register HWACCREG2 */
#define HWACCREG2_NU_BIT_START                              16U
#define HWACCREG2_NU_BIT_END                                31U

/* Definition for field CR42ACCTRIG in Register HWACCREG3 */
#define HWACCREG3_CR42ACCTRIG_BIT_START                     0U
#define HWACCREG3_CR42ACCTRIG_BIT_END                       0U

/* Definition for field NU in Register HWACCREG3 */
#define HWACCREG3_NU_BIT_START                              1U
#define HWACCREG3_NU_BIT_END                                15U

/* Definition for field CR42DMATRIG in Register HWACCREG3 */
#define HWACCREG3_CR42DMATRIG_BIT_START                     16U
#define HWACCREG3_CR42DMATRIG_BIT_END                       31U

/* Definition for field PARAMDONESTAT in Register HWACCREG4 */
#define HWACCREG4_PARAMDONESTAT_BIT_START                   0U
#define HWACCREG4_PARAMDONESTAT_BIT_END                     15U

/* Definition for field PARAMDONECLR in Register HWACCREG4 */
#define HWACCREG4_PARAMDONECLR_BIT_START                    16U
#define HWACCREG4_PARAMDONECLR_BIT_END                      31U

/* Definition for field BPMPATTERNMSB in Register HWACCREG5 */
#define HWACCREG5_BPMPATTERNMSB_BIT_START                   0U
#define HWACCREG5_BPMPATTERNMSB_BIT_END                     31U

/* Definition for field BPMPATTERNLSB in Register HWACCREG6 */
#define HWACCREG6_BPMPATTERNLSB_BIT_START                   0U
#define HWACCREG6_BPMPATTERNLSB_BIT_END                     31U

/* Definition for field BPMRATE in Register HWACCREG7 */
#define HWACCREG7_BPMRATE_BIT_START                         0U
#define HWACCREG7_BPMRATE_BIT_END                           9U

/* Definition for field NU1 in Register HWACCREG7 */
#define HWACCREG7_NU1_BIT_START                             10U
#define HWACCREG7_NU1_BIT_END                               15U

/* Definition for field DITHERTWIDEN in Register HWACCREG7 */
#define HWACCREG7_DITHERTWIDEN_BIT_START                    16U
#define HWACCREG7_DITHERTWIDEN_BIT_END                      16U

/* Definition for field NU2 in Register HWACCREG7 */
#define HWACCREG7_NU2_BIT_START                             17U
#define HWACCREG7_NU2_BIT_END                               23U

/* Definition for field STG1LUTSELWR in Register HWACCREG7 */
#define HWACCREG7_STG1LUTSELWR_BIT_START                    24U
#define HWACCREG7_STG1LUTSELWR_BIT_END                      24U

/* Definition for field NU3 in Register HWACCREG7 */
#define HWACCREG7_NU3_BIT_START                             25U
#define HWACCREG7_NU3_BIT_END                               31U

/* Definition for field INTERFTHRESH in Register HWACCREG8 */
#define HWACCREG8_INTERFTHRESH_BIT_START                    0U
#define HWACCREG8_INTERFTHRESH_BIT_END                      23U

/* Definition for field FFTSUMDIV in Register HWACCREG8 */
#define HWACCREG8_FFTSUMDIV_BIT_START                       24U
#define HWACCREG8_FFTSUMDIV_BIT_END                         28U

/* Definition for field NU in Register HWACCREG8 */
#define HWACCREG8_NU_BIT_START                              29U
#define HWACCREG8_NU_BIT_END                                31U

/* Definition for field ICMULTSCALE in Register HWACCREG9 */
#define HWACCREG9_ICMULTSCALE_BIT_START                     0U
#define HWACCREG9_ICMULTSCALE_BIT_END                       20U

/* Definition for field NU in Register HWACCREG9 */
#define HWACCREG9_NU_BIT_START                              21U
#define HWACCREG9_NU_BIT_END                                31U

/* Definition for field QCMULTSCALE in Register HWACCREG10 */
#define HWACCREG10_QCMULTSCALE_BIT_START                    0U
#define HWACCREG10_QCMULTSCALE_BIT_END                      20U

/* Definition for field NU in Register HWACCREG10 */
#define HWACCREG10_NU_BIT_START                             21U
#define HWACCREG10_NU_BIT_END                               31U

/* Definition for field LFSRSEED in Register HWACCREG11 */
#define HWACCREG11_LFSRSEED_BIT_START                       0U
#define HWACCREG11_LFSRSEED_BIT_END                         28U

/* Definition for field NU in Register HWACCREG11 */
#define HWACCREG11_NU_BIT_START                             29U
#define HWACCREG11_NU_BIT_END                               30U

/* Definition for field LFSRLOAD in Register HWACCREG11 */
#define HWACCREG11_LFSRLOAD_BIT_START                       31U
#define HWACCREG11_LFSRLOAD_BIT_END                         31U

/* Definition for field ACC_TRIGGER_IN_STAT in Register HWACCREG12 */
#define HWACCREG12_ACC_TRIGGER_IN_STAT_BIT_START            0U
#define HWACCREG12_ACC_TRIGGER_IN_STAT_BIT_END              18U

/* Definition for field NU1 in Register HWACCREG12 */
#define HWACCREG12_NU1_BIT_START                            19U
#define HWACCREG12_NU1_BIT_END                              23U

/* Definition for field ACC_TRIGGER_IN_CLR in Register HWACCREG12 */
#define HWACCREG12_ACC_TRIGGER_IN_CLR_BIT_START             19U
#define HWACCREG12_ACC_TRIGGER_IN_CLR_BIT_END               19U

/* Definition for field NU2 in Register HWACCREG12 */
#define HWACCREG12_NU2_BIT_START                            25U
#define HWACCREG12_NU2_BIT_END                              31U

/* Definition for field CFAR_THRESH in Register HWACCREG13 */
#define HWACCREG13_CFAR_THRESH_BIT_START                    0U
#define HWACCREG13_CFAR_THRESH_BIT_END                      17U

/* Definition for field NU in Register HWACCREG13 */
#define HWACCREG13_NU_BIT_START                             18U
#define HWACCREG13_NU_BIT_END                               31U

/* Definition for field OUTRAMISO in Register HWACCREG14 */
#define HWACCREG14_OUTRAMISO_BIT_START                      0U
#define HWACCREG14_OUTRAMISO_BIT_END                        2U

/* Definition for field NU1 in Register HWACCREG14 */
#define HWACCREG14_NU1_BIT_START                            3U
#define HWACCREG14_NU1_BIT_END                              3U

/* Definition for field OUTRAMAGOODIN in Register HWACCREG14 */
#define HWACCREG14_OUTRAMAGOODIN_BIT_START                  4U
#define HWACCREG14_OUTRAMAGOODIN_BIT_END                    6U

/* Definition for field NU2 in Register HWACCREG14 */
#define HWACCREG14_NU2_BIT_START                            7U
#define HWACCREG14_NU2_BIT_END                              7U

/* Definition for field OUTRAMAONIN in Register HWACCREG14 */
#define HWACCREG14_OUTRAMAONIN_BIT_START                    8U
#define HWACCREG14_OUTRAMAONIN_BIT_END                      10U

/* Definition for field NU3 in Register HWACCREG14 */
#define HWACCREG14_NU3_BIT_START                            11U
#define HWACCREG14_NU3_BIT_END                              31U

/* Definition for field SPARE2 in Register HWACCREG15 */
#define HWACCREG15_SPARE2_BIT_START                         0U
#define HWACCREG15_SPARE2_BIT_END                           31U

/* Definition for field MAX1VALUE in Register MAX1VALUE */
#define MAX1VALUE_MAX1VALUE_BIT_START                       0U
#define MAX1VALUE_MAX1VALUE_BIT_END                         23U

/* Definition for field NU in Register MAX1VALUE */
#define MAX1VALUE_NU_BIT_START                              24U
#define MAX1VALUE_NU_BIT_END                                31U

/* Definition for field MAX1INDEX in Register MAX1INDEX */
#define MAX1INDEX_MAX1INDEX_BIT_START                       0U
#define MAX1INDEX_MAX1INDEX_BIT_END                         11U

/* Definition for field NU in Register MAX1INDEX */
#define MAX1INDEX_NU_BIT_START                              12U
#define MAX1INDEX_NU_BIT_END                                31U

/* Definition for field ISUM1LSB in Register ISUM1LSB */
#define ISUM1LSB_ISUM1LSB_BIT_START                         0U
#define ISUM1LSB_ISUM1LSB_BIT_END                           31U

/* Definition for field ISUM1MSB in Register ISUM1MSB */
#define ISUM1MSB_ISUM1MSB_BIT_START                         0U
#define ISUM1MSB_ISUM1MSB_BIT_END                           3U

/* Definition for field NU in Register ISUM1MSB */
#define ISUM1MSB_NU_BIT_START                               4U
#define ISUM1MSB_NU_BIT_END                                 31U

/* Definition for field QSUM1LSB in Register QSUM1LSB */
#define QSUM1LSB_QSUM1LSB_BIT_START                         0U
#define QSUM1LSB_QSUM1LSB_BIT_END                           31U

/* Definition for field QSUM1MSB in Register QSUM1MSB */
#define QSUM1MSB_QSUM1MSB_BIT_START                         0U
#define QSUM1MSB_QSUM1MSB_BIT_END                           3U

/* Definition for field NU in Register QSUM1MSB */
#define QSUM1MSB_NU_BIT_START                               4U
#define QSUM1MSB_NU_BIT_END                                 31U

/* Definition for field MAX2VALUE in Register MAX2VALUE */
#define MAX2VALUE_MAX2VALUE_BIT_START                       0U
#define MAX2VALUE_MAX2VALUE_BIT_END                         23U

/* Definition for field NU in Register MAX2VALUE */
#define MAX2VALUE_NU_BIT_START                              24U
#define MAX2VALUE_NU_BIT_END                                31U

/* Definition for field MAX2INDEX in Register MAX2INDEX */
#define MAX2INDEX_MAX2INDEX_BIT_START                       0U
#define MAX2INDEX_MAX2INDEX_BIT_END                         11U

/* Definition for field NU in Register MAX2INDEX */
#define MAX2INDEX_NU_BIT_START                              12U
#define MAX2INDEX_NU_BIT_END                                31U

/* Definition for field ISUM2LSB in Register ISUM2LSB */
#define ISUM2LSB_ISUM2LSB_BIT_START                         0U
#define ISUM2LSB_ISUM2LSB_BIT_END                           31U

/* Definition for field ISUM2MSB in Register ISUM2MSB */
#define ISUM2MSB_ISUM2MSB_BIT_START                         0U
#define ISUM2MSB_ISUM2MSB_BIT_END                           3U

/* Definition for field NU in Register ISUM2MSB */
#define ISUM2MSB_NU_BIT_START                               4U
#define ISUM2MSB_NU_BIT_END                                 31U

/* Definition for field QSUM2LSB in Register QSUM2LSB */
#define QSUM2LSB_QSUM2LSB_BIT_START                         0U
#define QSUM2LSB_QSUM2LSB_BIT_END                           31U

/* Definition for field QSUM2MSB in Register QSUM2MSB */
#define QSUM2MSB_QSUM2MSB_BIT_START                         0U
#define QSUM2MSB_QSUM2MSB_BIT_END                           3U

/* Definition for field NU in Register QSUM2MSB */
#define QSUM2MSB_NU_BIT_START                               4U
#define QSUM2MSB_NU_BIT_END                                 31U

/* Definition for field MAX3VALUE in Register MAX3VALUE */
#define MAX3VALUE_MAX3VALUE_BIT_START                       0U
#define MAX3VALUE_MAX3VALUE_BIT_END                         23U

/* Definition for field NU in Register MAX3VALUE */
#define MAX3VALUE_NU_BIT_START                              24U
#define MAX3VALUE_NU_BIT_END                                31U

/* Definition for field MAX3INDEX in Register MAX3INDEX */
#define MAX3INDEX_MAX3INDEX_BIT_START                       0U
#define MAX3INDEX_MAX3INDEX_BIT_END                         11U

/* Definition for field NU in Register MAX3INDEX */
#define MAX3INDEX_NU_BIT_START                              12U
#define MAX3INDEX_NU_BIT_END                                31U

/* Definition for field ISUM3LSB in Register ISUM3LSB */
#define ISUM3LSB_ISUM3LSB_BIT_START                         0U
#define ISUM3LSB_ISUM3LSB_BIT_END                           31U

/* Definition for field ISUM3MSB in Register ISUM3MSB */
#define ISUM3MSB_ISUM3MSB_BIT_START                         0U
#define ISUM3MSB_ISUM3MSB_BIT_END                           3U

/* Definition for field NU in Register ISUM3MSB */
#define ISUM3MSB_NU_BIT_START                               4U
#define ISUM3MSB_NU_BIT_END                                 31U

/* Definition for field QSUM3LSB in Register QSUM3LSB */
#define QSUM3LSB_QSUM3LSB_BIT_START                         0U
#define QSUM3LSB_QSUM3LSB_BIT_END                           31U

/* Definition for field QSUM3MSB in Register QSUM3MSB */
#define QSUM3MSB_QSUM3MSB_BIT_START                         0U
#define QSUM3MSB_QSUM3MSB_BIT_END                           3U

/* Definition for field NU in Register QSUM3MSB */
#define QSUM3MSB_NU_BIT_START                               4U
#define QSUM3MSB_NU_BIT_END                                 31U

/* Definition for field MAX4VALUE in Register MAX4VALUE */
#define MAX4VALUE_MAX4VALUE_BIT_START                       0U
#define MAX4VALUE_MAX4VALUE_BIT_END                         23U

/* Definition for field NU in Register MAX4VALUE */
#define MAX4VALUE_NU_BIT_START                              24U
#define MAX4VALUE_NU_BIT_END                                31U

/* Definition for field MAX4INDEX in Register MAX4INDEX */
#define MAX4INDEX_MAX4INDEX_BIT_START                       0U
#define MAX4INDEX_MAX4INDEX_BIT_END                         11U

/* Definition for field NU in Register MAX4INDEX */
#define MAX4INDEX_NU_BIT_START                              12U
#define MAX4INDEX_NU_BIT_END                                31U

/* Definition for field ISUM4LSB in Register ISUM4LSB */
#define ISUM4LSB_ISUM4LSB_BIT_START                         0U
#define ISUM4LSB_ISUM4LSB_BIT_END                           31U

/* Definition for field ISUM4MSB in Register ISUM4MSB */
#define ISUM4MSB_ISUM4MSB_BIT_START                         0U
#define ISUM4MSB_ISUM4MSB_BIT_END                           3U

/* Definition for field NU in Register ISUM4MSB */
#define ISUM4MSB_NU_BIT_START                               4U
#define ISUM4MSB_NU_BIT_END                                 31U

/* Definition for field QSUM4LSB in Register QSUM4LSB */
#define QSUM4LSB_QSUM4LSB_BIT_START                         0U
#define QSUM4LSB_QSUM4LSB_BIT_END                           31U

/* Definition for field QSUM4MSB in Register QSUM4MSB */
#define QSUM4MSB_QSUM4MSB_BIT_START                         0U
#define QSUM4MSB_QSUM4MSB_BIT_END                           3U

/* Definition for field NU in Register QSUM4MSB */
#define QSUM4MSB_NU_BIT_START                               4U
#define QSUM4MSB_NU_BIT_END                                 31U

/* Definition for field PARAMADDR in Register RDSTATUS */
#define RDSTATUS_PARAMADDR_BIT_START                        0U
#define RDSTATUS_PARAMADDR_BIT_END                          3U

/* Definition for field LOOPCNT in Register RDSTATUS */
#define RDSTATUS_LOOPCNT_BIT_START                          4U
#define RDSTATUS_LOOPCNT_BIT_END                            15U

/* Definition for field NU in Register RDSTATUS */
#define RDSTATUS_NU_BIT_START                               16U
#define RDSTATUS_NU_BIT_END                                 31U

/* Definition for field SIGDMACH1DONE in Register SIGDMACH1DONE */
#define SIGDMACH1DONE_SIGDMACH1DONE_BIT_START               0U
#define SIGDMACH1DONE_SIGDMACH1DONE_BIT_END                 31U

/* Definition for field SIGDMACH2DONE in Register SIGDMACH2DONE */
#define SIGDMACH2DONE_SIGDMACH2DONE_BIT_START               0U
#define SIGDMACH2DONE_SIGDMACH2DONE_BIT_END                 31U

/* Definition for field SIGDMACH3DONE in Register SIGDMACH3DONE */
#define SIGDMACH3DONE_SIGDMACH3DONE_BIT_START               0U
#define SIGDMACH3DONE_SIGDMACH3DONE_BIT_END                 31U

/* Definition for field SIGDMACH4DONE in Register SIGDMACH4DONE */
#define SIGDMACH4DONE_SIGDMACH4DONE_BIT_START               0U
#define SIGDMACH4DONE_SIGDMACH4DONE_BIT_END                 31U

/* Definition for field SIGDMACH5DONE in Register SIGDMACH5DONE */
#define SIGDMACH5DONE_SIGDMACH5DONE_BIT_START               0U
#define SIGDMACH5DONE_SIGDMACH5DONE_BIT_END                 31U

/* Definition for field SIGDMACH6DONE in Register SIGDMACH6DONE */
#define SIGDMACH6DONE_SIGDMACH6DONE_BIT_START               0U
#define SIGDMACH6DONE_SIGDMACH6DONE_BIT_END                 31U

/* Definition for field SIGDMACH7DONE in Register SIGDMACH7DONE */
#define SIGDMACH7DONE_SIGDMACH7DONE_BIT_START               0U
#define SIGDMACH7DONE_SIGDMACH7DONE_BIT_END                 31U

/* Definition for field SIGDMACH8DONE in Register SIGDMACH8DONE */
#define SIGDMACH8DONE_SIGDMACH8DONE_BIT_START               0U
#define SIGDMACH8DONE_SIGDMACH8DONE_BIT_END                 31U

/* Definition for field SIGDMACH9DONE in Register SIGDMACH9DONE */
#define SIGDMACH9DONE_SIGDMACH9DONE_BIT_START               0U
#define SIGDMACH9DONE_SIGDMACH9DONE_BIT_END                 31U

/* Definition for field SIGDMACH10DONE in Register SIGDMACH10DONE */
#define SIGDMACH10DONE_SIGDMACH10DONE_BIT_START             0U
#define SIGDMACH10DONE_SIGDMACH10DONE_BIT_END               31U

/* Definition for field SIGDMACH11DONE in Register SIGDMACH11DONE */
#define SIGDMACH11DONE_SIGDMACH11DONE_BIT_START             0U
#define SIGDMACH11DONE_SIGDMACH11DONE_BIT_END               31U

/* Definition for field SIGDMACH12DONE in Register SIGDMACH12DONE */
#define SIGDMACH12DONE_SIGDMACH12DONE_BIT_START             0U
#define SIGDMACH12DONE_SIGDMACH12DONE_BIT_END               31U

/* Definition for field SIGDMACH13DONE in Register SIGDMACH13DONE */
#define SIGDMACH13DONE_SIGDMACH13DONE_BIT_START             0U
#define SIGDMACH13DONE_SIGDMACH13DONE_BIT_END               31U

/* Definition for field SIGDMACH14DONE in Register SIGDMACH14DONE */
#define SIGDMACH14DONE_SIGDMACH14DONE_BIT_START             0U
#define SIGDMACH14DONE_SIGDMACH14DONE_BIT_END               31U

/* Definition for field SIGDMACH15DONE in Register SIGDMACH15DONE */
#define SIGDMACH15DONE_SIGDMACH15DONE_BIT_START             0U
#define SIGDMACH15DONE_SIGDMACH15DONE_BIT_END               31U

/* Definition for field SIGDMACH16DONE in Register SIGDMACH16DONE */
#define SIGDMACH16DONE_SIGDMACH16DONE_BIT_START             0U
#define SIGDMACH16DONE_SIGDMACH16DONE_BIT_END               31U

/* Definition for field ERRCODECLR in Register MEMACCESSERR */
#define MEMACCESSERR_ERRCODECLR_BIT_START                   0U
#define MEMACCESSERR_ERRCODECLR_BIT_END                     3U

/* Definition for field NU1 in Register MEMACCESSERR */
#define MEMACCESSERR_NU1_BIT_START                          4U
#define MEMACCESSERR_NU1_BIT_END                            7U

/* Definition for field ERRCODEMASK in Register MEMACCESSERR */
#define MEMACCESSERR_ERRCODEMASK_BIT_START                  8U
#define MEMACCESSERR_ERRCODEMASK_BIT_END                    11U

/* Definition for field NU2 in Register MEMACCESSERR */
#define MEMACCESSERR_NU2_BIT_START                          12U
#define MEMACCESSERR_NU2_BIT_END                            15U

/* Definition for field STATERRCODE in Register MEMACCESSERR */
#define MEMACCESSERR_STATERRCODE_BIT_START                  16U
#define MEMACCESSERR_STATERRCODE_BIT_END                    19U

/* Definition for field NU3 in Register MEMACCESSERR */
#define MEMACCESSERR_NU3_BIT_START                          20U
#define MEMACCESSERR_NU3_BIT_END                            31U

/* Definition for field FFTCLCIPSTAT in Register FFTCLIP */
#define FFTCLIP_FFTCLCIPSTAT_BIT_START                      0U
#define FFTCLIP_FFTCLCIPSTAT_BIT_END                        9U

/* Definition for field NU1 in Register FFTCLIP */
#define FFTCLIP_NU1_BIT_START                               10U
#define FFTCLIP_NU1_BIT_END                                 15U

/* Definition for field CLRFFTCLIPSTAT in Register FFTCLIP */
#define FFTCLIP_CLRFFTCLIPSTAT_BIT_START                    16U
#define FFTCLIP_CLRFFTCLIPSTAT_BIT_END                      16U

/* Definition for field NU2 in Register FFTCLIP */
#define FFTCLIP_NU2_BIT_START                               17U
#define FFTCLIP_NU2_BIT_END                                 31U

/* Definition for field FFTPEAKCNT in Register FFTPEAKCNT */
#define FFTPEAKCNT_FFTPEAKCNT_BIT_START                     0U
#define FFTPEAKCNT_FFTPEAKCNT_BIT_END                       11U

/* Definition for field NU in Register FFTPEAKCNT */
#define FFTPEAKCNT_NU_BIT_START                             12U
#define FFTPEAKCNT_NU_BIT_END                               31U

/* Definition for field WIN_RAM_ECC_EN in Register ECCENABLE */
#define ECCENABLE_WIN_RAM_ECC_EN_BIT_START                  0U
#define ECCENABLE_WIN_RAM_ECC_EN_BIT_END                    0U

/* Definition for field PARAM_ECC_EN in Register ECCENABLE */
#define ECCENABLE_PARAM_ECC_EN_BIT_START                    1U
#define ECCENABLE_PARAM_ECC_EN_BIT_END                      1U

/* Definition for field IPING_ECC_EN in Register ECCENABLE */
#define ECCENABLE_IPING_ECC_EN_BIT_START                    12U
#define ECCENABLE_IPING_ECC_EN_BIT_END                      12U

/* Definition for field IPONG_ECC_EN in Register ECCENABLE */
#define ECCENABLE_IPONG_ECC_EN_BIT_START                    13U
#define ECCENABLE_IPONG_ECC_EN_BIT_END                      13U

/* Definition for field OPING_ECC_EN in Register ECCENABLE */
#define ECCENABLE_OPING_ECC_EN_BIT_START                    14U
#define ECCENABLE_OPING_ECC_EN_BIT_END                      14U

/* Definition for field OPONG_ECC_EN in Register ECCENABLE */
#define ECCENABLE_OPONG_ECC_EN_BIT_START                    15U
#define ECCENABLE_OPONG_ECC_EN_BIT_END                      15U

/* Definition for field FSM_LOCKSTEP_EN in Register ECCENABLE */
#define ECCENABLE_FSM_LOCKSTEP_EN_BIT_START                 16U
#define ECCENABLE_FSM_LOCKSTEP_EN_BIT_END                   16U

/* Definition for field FSM_LOCKSTEP_SELFTEST_EN in Register ECCENABLE */
#define ECCENABLE_FSM_LOCKSTEP_SELFTEST_EN_BIT_START        17U
#define ECCENABLE_FSM_LOCKSTEP_SELFTEST_EN_BIT_END          17U

/* Definition for field NU2 in Register ECCENABLE */
#define ECCENABLE_NU2_BIT_START                             18U
#define ECCENABLE_NU2_BIT_END                               31U

/* Definition for field WIN_RAM_ECC_ERRCLR in Register ECCERRCLR */
#define ECCERRCLR_WIN_RAM_ECC_ERRCLR_BIT_START              0U
#define ECCERRCLR_WIN_RAM_ECC_ERRCLR_BIT_END                0U

/* Definition for field PARAM_ECC_ERRCLR in Register ECCERRCLR */
#define ECCERRCLR_PARAM_ECC_ERRCLR_BIT_START                1U
#define ECCERRCLR_PARAM_ECC_ERRCLR_BIT_END                  1U

/* Definition for field IPING_ECC_ERRCLR in Register ECCERRCLR */
#define ECCERRCLR_IPING_ECC_ERRCLR_BIT_START                12U
#define ECCERRCLR_IPING_ECC_ERRCLR_BIT_END                  12U

/* Definition for field IPONG_ECC_ERRCLR in Register ECCERRCLR */
#define ECCERRCLR_IPONG_ECC_ERRCLR_BIT_START                13U
#define ECCERRCLR_IPONG_ECC_ERRCLR_BIT_END                  13U

/* Definition for field OPING_ECC_ERRCLR in Register ECCERRCLR */
#define ECCERRCLR_OPING_ECC_ERRCLR_BIT_START                14U
#define ECCERRCLR_OPING_ECC_ERRCLR_BIT_END                  14U

/* Definition for field OPONG_ECC_ERRCLR in Register ECCERRCLR */
#define ECCERRCLR_OPONG_ECC_ERRCLR_BIT_START                15U
#define ECCERRCLR_OPONG_ECC_ERRCLR_BIT_END                  15U

/* Definition for field NU in Register ECCERRCLR */
#define ECCERRCLR_NU_BIT_START                              16U
#define ECCERRCLR_NU_BIT_END                                31U

/* Definition for field WIN_RAM_INIT in Register MEMINIT */
#define MEMINIT_WIN_RAM_INIT_BIT_START                      0U
#define MEMINIT_WIN_RAM_INIT_BIT_END                        0U

/* Definition for field PARAM_INIT in Register MEMINIT */
#define MEMINIT_PARAM_INIT_BIT_START                        1U
#define MEMINIT_PARAM_INIT_BIT_END                          1U

/* Definition for field IPING_INIT in Register MEMINIT */
#define MEMINIT_IPING_INIT_BIT_START                        12U
#define MEMINIT_IPING_INIT_BIT_END                          12U

/* Definition for field IPONG_INIT in Register MEMINIT */
#define MEMINIT_IPONG_INIT_BIT_START                        13U
#define MEMINIT_IPONG_INIT_BIT_END                          13U

/* Definition for field OPING_INIT in Register MEMINIT */
#define MEMINIT_OPING_INIT_BIT_START                        14U
#define MEMINIT_OPING_INIT_BIT_END                          14U

/* Definition for field OPONG_INIT in Register MEMINIT */
#define MEMINIT_OPONG_INIT_BIT_START                        15U
#define MEMINIT_OPONG_INIT_BIT_END                          15U

/* Definition for field NU in Register MEMINIT */
#define MEMINIT_NU_BIT_START                                16U
#define MEMINIT_NU_BIT_END                                  31U

/* Definition for field WIN_RAM_INITDONE in Register MEMINITDONE */
#define MEMINITDONE_WIN_RAM_INITDONE_BIT_START              0U
#define MEMINITDONE_WIN_RAM_INITDONE_BIT_END                0U

/* Definition for field PARAM_INITDONE in Register MEMINITDONE */
#define MEMINITDONE_PARAM_INITDONE_BIT_START                1U
#define MEMINITDONE_PARAM_INITDONE_BIT_END                  1U

/* Definition for field IPING_INITDONE in Register MEMINITDONE */
#define MEMINITDONE_IPING_INITDONE_BIT_START                12U
#define MEMINITDONE_IPING_INITDONE_BIT_END                  12U

/* Definition for field IPONG_INITDONE in Register MEMINITDONE */
#define MEMINITDONE_IPONG_INITDONE_BIT_START                13U
#define MEMINITDONE_IPONG_INITDONE_BIT_END                  13U

/* Definition for field OPING_INITDONE in Register MEMINITDONE */
#define MEMINITDONE_OPING_INITDONE_BIT_START                14U
#define MEMINITDONE_OPING_INITDONE_BIT_END                  14U

/* Definition for field OPONG_INITDONE in Register MEMINITDONE */
#define MEMINITDONE_OPONG_INITDONE_BIT_START                15U
#define MEMINITDONE_OPONG_INITDONE_BIT_END                  15U

/* Definition for field NU in Register MEMINITDONE */
#define MEMINITDONE_NU_BIT_START                            16U
#define MEMINITDONE_NU_BIT_END                              31U

/* Definition for field NU1 in Register ECC_SBE_STATUS */
#define ECC_SBE_STATUS_NU1_BIT_START                        0U
#define ECC_SBE_STATUS_NU1_BIT_END                          0U

/* Definition for field NU2 in Register ECC_SBE_STATUS */
#define ECC_SBE_STATUS_NU2_BIT_START                        1U
#define ECC_SBE_STATUS_NU2_BIT_END                          1U

/* Definition for field IPING_ECC_SBE in Register ECC_SBE_STATUS */
#define ECC_SBE_STATUS_IPING_ECC_SBE_BIT_START              12U
#define ECC_SBE_STATUS_IPING_ECC_SBE_BIT_END                12U

/* Definition for field IPONG_ECC_SBE in Register ECC_SBE_STATUS */
#define ECC_SBE_STATUS_IPONG_ECC_SBE_BIT_START              13U
#define ECC_SBE_STATUS_IPONG_ECC_SBE_BIT_END                13U

/* Definition for field OPING_ECC_SBE in Register ECC_SBE_STATUS */
#define ECC_SBE_STATUS_OPING_ECC_SBE_BIT_START              14U
#define ECC_SBE_STATUS_OPING_ECC_SBE_BIT_END                14U

/* Definition for field OPONG_ECC_SBE in Register ECC_SBE_STATUS */
#define ECC_SBE_STATUS_OPONG_ECC_SBE_BIT_START              15U
#define ECC_SBE_STATUS_OPONG_ECC_SBE_BIT_END                15U

/* Definition for field NU3 in Register ECC_SBE_STATUS */
#define ECC_SBE_STATUS_NU3_BIT_START                        16U
#define ECC_SBE_STATUS_NU3_BIT_END                          31U

/* Definition for field NU1 in Register ECC_DBE_STATUS */
#define ECC_DBE_STATUS_NU1_BIT_START                        0U
#define ECC_DBE_STATUS_NU1_BIT_END                          0U

/* Definition for field NU2 in Register ECC_DBE_STATUS */
#define ECC_DBE_STATUS_NU2_BIT_START                        1U
#define ECC_DBE_STATUS_NU2_BIT_END                          1U

/* Definition for field IPING_ECC_DBE in Register ECC_DBE_STATUS */
#define ECC_DBE_STATUS_IPING_ECC_DBE_BIT_START              12U
#define ECC_DBE_STATUS_IPING_ECC_DBE_BIT_END                12U

/* Definition for field IPONG_ECC_DBE in Register ECC_DBE_STATUS */
#define ECC_DBE_STATUS_IPONG_ECC_DBE_BIT_START              13U
#define ECC_DBE_STATUS_IPONG_ECC_DBE_BIT_END                13U

/* Definition for field OPING_ECC_DBE in Register ECC_DBE_STATUS */
#define ECC_DBE_STATUS_OPING_ECC_DBE_BIT_START              14U
#define ECC_DBE_STATUS_OPING_ECC_DBE_BIT_END                14U

/* Definition for field OPONG_ECC_DBE in Register ECC_DBE_STATUS */
#define ECC_DBE_STATUS_OPONG_ECC_DBE_BIT_START              15U
#define ECC_DBE_STATUS_OPONG_ECC_DBE_BIT_END                15U

/* Definition for field NU3 in Register ECC_DBE_STATUS */
#define ECC_DBE_STATUS_NU3_BIT_START                        16U
#define ECC_DBE_STATUS_NU3_BIT_END                          31U

/* Definition for field WIN_RAM_ERR_ADDR in Register WINRAMERRLOC */
#define WINRAMERRLOC_WIN_RAM_ERR_ADDR_BIT_START             0U
#define WINRAMERRLOC_WIN_RAM_ERR_ADDR_BIT_END               15U

/* Definition for field WIN_RAM_ERR_BIT in Register WINRAMERRLOC */
#define WINRAMERRLOC_WIN_RAM_ERR_BIT_BIT_START              16U
#define WINRAMERRLOC_WIN_RAM_ERR_BIT_BIT_END                31U

/* Definition for field PARAM_ERR_ADDR in Register PARAMRAMERRLOC */
#define PARAMRAMERRLOC_PARAM_ERR_ADDR_BIT_START             0U
#define PARAMRAMERRLOC_PARAM_ERR_ADDR_BIT_END               15U

/* Definition for field PARAM_ERR_BIT in Register PARAMRAMERRLOC */
#define PARAMRAMERRLOC_PARAM_ERR_BIT_BIT_START              16U
#define PARAMRAMERRLOC_PARAM_ERR_BIT_BIT_END                31U

/* Definition for field IPING_ERR_ADDR in Register IPINGERRLOC */
#define IPINGERRLOC_IPING_ERR_ADDR_BIT_START                0U
#define IPINGERRLOC_IPING_ERR_ADDR_BIT_END                  15U

/* Definition for field IPING_ERR_BIT_REGION in Register IPINGERRLOC */
#define IPINGERRLOC_IPING_ERR_BIT_REGION_BIT_START          16U
#define IPINGERRLOC_IPING_ERR_BIT_REGION_BIT_END            31U

/* Definition for field IPING_ERR_BIT_LOC in Register IPINGSBELOC */
#define IPINGSBELOC_IPING_ERR_BIT_LOC_BIT_START             0U
#define IPINGSBELOC_IPING_ERR_BIT_LOC_BIT_END               31U

/* Definition for field IPONG_ERR_ADDR in Register IPONGERRLOC */
#define IPONGERRLOC_IPONG_ERR_ADDR_BIT_START                0U
#define IPONGERRLOC_IPONG_ERR_ADDR_BIT_END                  15U

/* Definition for field IPONG_ERR_BIT_REGION in Register IPONGERRLOC */
#define IPONGERRLOC_IPONG_ERR_BIT_REGION_BIT_START          16U
#define IPONGERRLOC_IPONG_ERR_BIT_REGION_BIT_END            31U

/* Definition for field IPONG_ERR_BIT_LOC in Register IPONGSBELOC */
#define IPONGSBELOC_IPONG_ERR_BIT_LOC_BIT_START             0U
#define IPONGSBELOC_IPONG_ERR_BIT_LOC_BIT_END               31U

/* Definition for field OPING_ERR_ADDR in Register OPINGERRLOC */
#define OPINGERRLOC_OPING_ERR_ADDR_BIT_START                0U
#define OPINGERRLOC_OPING_ERR_ADDR_BIT_END                  15U

/* Definition for field OPING_ERR_BIT_REGION in Register OPINGERRLOC */
#define OPINGERRLOC_OPING_ERR_BIT_REGION_BIT_START          16U
#define OPINGERRLOC_OPING_ERR_BIT_REGION_BIT_END            31U

/* Definition for field OPING_ERR_BIT_LOC in Register OPINGSBELOC */
#define OPINGSBELOC_OPING_ERR_BIT_LOC_BIT_START             0U
#define OPINGSBELOC_OPING_ERR_BIT_LOC_BIT_END               31U

/* Definition for field OPONG_ERR_ADDR in Register OPONGERRLOC */
#define OPONGERRLOC_OPONG_ERR_ADDR_BIT_START                0U
#define OPONGERRLOC_OPONG_ERR_ADDR_BIT_END                  15U

/* Definition for field OPONG_ERR_BIT_REGION in Register OPONGERRLOC */
#define OPONGERRLOC_OPONG_ERR_BIT_REGION_BIT_START          16U
#define OPONGERRLOC_OPONG_ERR_BIT_REGION_BIT_END            31U

/* Definition for field OPONG_ERR_BIT_LOC in Register OPONGSBELOC */
#define OPONGSBELOC_OPONG_ERR_BIT_LOC_BIT_START             0U
#define OPONGSBELOC_OPONG_ERR_BIT_LOC_BIT_END               31U

/** @struct hwaMaxStats
*   @brief  Hardware accelerator stats Register Definition
*
*   This structure is used to access the Hardware accelerator stats registers within the common space.
*/
/** @typedef hwaMaxStats_t
*   @brief  Hardware accelerator stats Register Definition
*
*   This structure is used to access the Hardware accelerator stats registers within the common space.
*/
typedef volatile struct hwaMaxStats
{
    uint32_t    MAXnVALUE       ;        /* AddrOffset = 0x000 */
    uint32_t    MAXnINDEX       ;        /* AddrOffset = 0x004 */
    uint32_t    ISUMnLSB        ;        /* AddrOffset = 0x008 */
    uint32_t    ISUMnMSB        ;        /* AddrOffset = 0x00C */
    uint32_t    QSUMnLSB        ;        /* AddrOffset = 0x010 */
    uint32_t    QSUMnMSB        ;        /* AddrOffset = 0x014 */
} hwaMaxStats_t;


/**
 * @struct DSSHWACCRegs_t
 * @brief
 *   Module DSS_HW_ACC Register Definition
 * @details
 *   This structure is used to access the DSS_HW_ACC module registers.
 */
/**
 * @typedef DSSHWACCRegs
 * @brief
 *   Module DSS_HW_ACC Register Frame type Definition
 * @details
 *   This type is used to access the DSS_HW_ACC module registers.
 */
typedef volatile struct DSSHWACCRegs_t
{
    uint32_t    HWACCREG1                          ;        /* Offset = 0x000 */
    uint32_t    HWACCREG2                          ;        /* Offset = 0x004 */
    uint32_t    HWACCREG3                          ;        /* Offset = 0x008 */
    uint32_t    HWACCREG4                          ;        /* Offset = 0x00C */
    uint32_t    HWACCREG5                          ;        /* Offset = 0x010 */
    uint32_t    HWACCREG6                          ;        /* Offset = 0x014 */
    uint32_t    HWACCREG7                          ;        /* Offset = 0x018 */
    uint32_t    HWACCREG8                          ;        /* Offset = 0x01C */
    uint32_t    HWACCREG9                          ;        /* Offset = 0x020 */
    uint32_t    HWACCREG10                         ;        /* Offset = 0x024 */
    uint32_t    HWACCREG11                         ;        /* Offset = 0x028 */
    uint32_t    HWACCREG12                         ;        /* Offset = 0x02C */
    uint32_t    HWACCREG13                         ;        /* Offset = 0x030 */
    uint32_t    HWACCREG14                         ;        /* Offset = 0x034 */
    uint32_t    HWACCREG15                         ;        /* Offset = 0x038 */
    uint32_t    RESTRICTED1                        ;        /* Offset = 0x03C */
#if 0    
    uint32_t    MAX1VALUE                          ;        /* Offset = 0x040 */
    uint32_t    MAX1INDEX                          ;        /* Offset = 0x044 */
    uint32_t    ISUM1LSB                           ;        /* Offset = 0x048 */
    uint32_t    ISUM1MSB                           ;        /* Offset = 0x04C */
    uint32_t    QSUM1LSB                           ;        /* Offset = 0x050 */
    uint32_t    QSUM1MSB                           ;        /* Offset = 0x054 */
    uint32_t    MAX2VALUE                          ;        /* Offset = 0x058 */
    uint32_t    MAX2INDEX                          ;        /* Offset = 0x05C */
    uint32_t    ISUM2LSB                           ;        /* Offset = 0x060 */
    uint32_t    ISUM2MSB                           ;        /* Offset = 0x064 */
    uint32_t    QSUM2LSB                           ;        /* Offset = 0x068 */
    uint32_t    QSUM2MSB                           ;        /* Offset = 0x06C */
    uint32_t    MAX3VALUE                          ;        /* Offset = 0x070 */
    uint32_t    MAX3INDEX                          ;        /* Offset = 0x074 */
    uint32_t    ISUM3LSB                           ;        /* Offset = 0x078 */
    uint32_t    ISUM3MSB                           ;        /* Offset = 0x07C */
    uint32_t    QSUM3LSB                           ;        /* Offset = 0x080 */
    uint32_t    QSUM3MSB                           ;        /* Offset = 0x084 */
    uint32_t    MAX4VALUE                          ;        /* Offset = 0x088 */
    uint32_t    MAX4INDEX                          ;        /* Offset = 0x08C */
    uint32_t    ISUM4LSB                           ;        /* Offset = 0x090 */
    uint32_t    ISUM4MSB                           ;        /* Offset = 0x094 */
    uint32_t    QSUM4LSB                           ;        /* Offset = 0x098 */
    uint32_t    QSUM4MSB                           ;        /* Offset = 0x09C */
#else
    hwaMaxStats_t HWACCSTATSREG[4];      /* AddrOffset = 0x040 - 0x09C */
#endif
    uint32_t    RESTRICTED2[3]                     ;        /* Offset = 0x0A0 */
    uint32_t    RDSTATUS                           ;        /* Offset = 0x0AC */
#if 0    
    uint32_t    SIGDMACH1DONE                      ;        /* Offset = 0x0B0 */
    uint32_t    SIGDMACH2DONE                      ;        /* Offset = 0x0B4 */
    uint32_t    SIGDMACH3DONE                      ;        /* Offset = 0x0B8 */
    uint32_t    SIGDMACH4DONE                      ;        /* Offset = 0x0BC */
    uint32_t    SIGDMACH5DONE                      ;        /* Offset = 0x0C0 */
    uint32_t    SIGDMACH6DONE                      ;        /* Offset = 0x0C4 */
    uint32_t    SIGDMACH7DONE                      ;        /* Offset = 0x0C8 */
    uint32_t    SIGDMACH8DONE                      ;        /* Offset = 0x0CC */
    uint32_t    SIGDMACH9DONE                      ;        /* Offset = 0x0D0 */
    uint32_t    SIGDMACH10DONE                     ;        /* Offset = 0x0D4 */
    uint32_t    SIGDMACH11DONE                     ;        /* Offset = 0x0D8 */
    uint32_t    SIGDMACH12DONE                     ;        /* Offset = 0x0DC */
    uint32_t    SIGDMACH13DONE                     ;        /* Offset = 0x0E0 */
    uint32_t    SIGDMACH14DONE                     ;        /* Offset = 0x0E4 */
    uint32_t    SIGDMACH15DONE                     ;        /* Offset = 0x0E8 */
    uint32_t    SIGDMACH16DONE                     ;        /* Offset = 0x0EC */
#else
    uint32_t    SIGDMACHxDONE[16];       /* AddrOffset = 0x0B0 */
#endif
    uint32_t    MEMACCESSERR                       ;        /* Offset = 0x0F0 */
    uint32_t    FFTCLIP                            ;        /* Offset = 0x0F4 */
    uint32_t    FFTPEAKCNT                         ;        /* Offset = 0x0F8 */
    uint32_t    RESTRICTED3[65]                    ;        /* Offset = 0x0FC */
    uint32_t    ECCENABLE                          ;        /* Offset = 0x200 */
    uint32_t    ECCERRCLR                          ;        /* Offset = 0x204 */
    uint32_t    MEMINIT                            ;        /* Offset = 0x208 */
    uint32_t    MEMINITDONE                        ;        /* Offset = 0x20C */
    uint32_t    ECC_SBE_STATUS                     ;        /* Offset = 0x210 */
    uint32_t    ECC_DBE_STATUS                     ;        /* Offset = 0x214 */
    uint32_t    WINRAMERRLOC                       ;        /* Offset = 0x218 */
    uint32_t    PARAMRAMERRLOC                     ;        /* Offset = 0x21C */
    uint32_t    RESTRICTED4[10]                    ;        /* Offset = 0x220 */
    uint32_t    IPINGERRLOC                        ;        /* Offset = 0x248 */
    uint32_t    IPINGSBELOC                        ;        /* Offset = 0x24C */
    uint32_t    IPONGERRLOC                        ;        /* Offset = 0x250 */
    uint32_t    IPONGSBELOC                        ;        /* Offset = 0x254 */
    uint32_t    OPINGERRLOC                        ;        /* Offset = 0x258 */
    uint32_t    OPINGSBELOC                        ;        /* Offset = 0x25C */
    uint32_t    OPONGERRLOC                        ;        /* Offset = 0x260 */
    uint32_t    OPONGSBELOC                        ;        /* Offset = 0x264 */
} DSSHWACCRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_DSSHWACC_XWR68XX_H */
/* END OF REG_DSSHWACC_XWR68XX_H */

