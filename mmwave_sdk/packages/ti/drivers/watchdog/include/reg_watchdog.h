/**
 *  @file    reg_watchdog.h
 *
 *  @brief
 *    This file gives register definitions of MSS_RTIB module.
 *
 *  This file is auto-generated on 3/27/2017.
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

#ifndef REG_RTIB_H
#define REG_RTIB_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Definition for field CNT0EN in Register RTIGCTRL */
#define RTIGCTRL_CNT0EN_BIT_START                           0U
#define RTIGCTRL_CNT0EN_BIT_END                             0U

/* Definition for field CNT1EN in Register RTIGCTRL */
#define RTIGCTRL_CNT1EN_BIT_START                           1U
#define RTIGCTRL_CNT1EN_BIT_END                             1U

/* Definition for field COS in Register RTIGCTRL */
#define RTIGCTRL_COS_BIT_START                              15U
#define RTIGCTRL_COS_BIT_END                                15U

/* Definition for field NTUSEL in Register RTIGCTRL */
#define RTIGCTRL_NTUSEL_BIT_START                           16U
#define RTIGCTRL_NTUSEL_BIT_END                             19U

/* Definition for field TBEXT in Register RTITBCTRL */
#define RTITBCTRL_TBEXT_BIT_START                           0U
#define RTITBCTRL_TBEXT_BIT_END                             0U

/* Definition for field INC in Register RTITBCTRL */
#define RTITBCTRL_INC_BIT_START                             1U
#define RTITBCTRL_INC_BIT_END                               1U

/* Definition for field CAPCNTR0 in Register RTICAPCTRL */
#define RTICAPCTRL_CAPCNTR0_BIT_START                       0U
#define RTICAPCTRL_CAPCNTR0_BIT_END                         0U

/* Definition for field CAPCNTR1 in Register RTICAPCTRL */
#define RTICAPCTRL_CAPCNTR1_BIT_START                       1U
#define RTICAPCTRL_CAPCNTR1_BIT_END                         1U

/* Definition for field COMP0SEL in Register RTICOMPCTRL */
#define RTICOMPCTRL_COMP0SEL_BIT_START                      0U
#define RTICOMPCTRL_COMP0SEL_BIT_END                        0U

/* Definition for field COMP1SEL in Register RTICOMPCTRL */
#define RTICOMPCTRL_COMP1SEL_BIT_START                      4U
#define RTICOMPCTRL_COMP1SEL_BIT_END                        4U

/* Definition for field COMP2SEL in Register RTICOMPCTRL */
#define RTICOMPCTRL_COMP2SEL_BIT_START                      8U
#define RTICOMPCTRL_COMP2SEL_BIT_END                        8U

/* Definition for field COMP3SEL in Register RTICOMPCTRL */
#define RTICOMPCTRL_COMP3SEL_BIT_START                      12U
#define RTICOMPCTRL_COMP3SEL_BIT_END                        12U

/* Definition for field FRC0 in Register RTIFRC0 */
#define RTIFRC0_FRC0_BIT_START                              0U
#define RTIFRC0_FRC0_BIT_END                                31U

/* Definition for field UC0 in Register RTIUC0 */
#define RTIUC0_UC0_BIT_START                                0U
#define RTIUC0_UC0_BIT_END                                  31U

/* Definition for field CPUC0 in Register RTICPUC0 */
#define RTICPUC0_CPUC0_BIT_START                            0U
#define RTICPUC0_CPUC0_BIT_END                              31U

/* Definition for field CAFRC0 in Register RTICAFRC0 */
#define RTICAFRC0_CAFRC0_BIT_START                          0U
#define RTICAFRC0_CAFRC0_BIT_END                            31U

/* Definition for field CAUC0 in Register RTICAUC0 */
#define RTICAUC0_CAUC0_BIT_START                            0U
#define RTICAUC0_CAUC0_BIT_END                              31U

/* Definition for field FRC1 in Register RTIFRC1 */
#define RTIFRC1_FRC1_BIT_START                              0U
#define RTIFRC1_FRC1_BIT_END                                31U

/* Definition for field UC1 in Register RTIUC1 */
#define RTIUC1_UC1_BIT_START                                0U
#define RTIUC1_UC1_BIT_END                                  31U

/* Definition for field CPUC1 in Register RTICPUC1 */
#define RTICPUC1_CPUC1_BIT_START                            0U
#define RTICPUC1_CPUC1_BIT_END                              31U

/* Definition for field CAFRC1 in Register RTICAFRC1 */
#define RTICAFRC1_CAFRC1_BIT_START                          0U
#define RTICAFRC1_CAFRC1_BIT_END                            31U

/* Definition for field CAUC1 in Register RTICAUC1 */
#define RTICAUC1_CAUC1_BIT_START                            0U
#define RTICAUC1_CAUC1_BIT_END                              31U

/* Definition for field COMP0 in Register RTICOMP0 */
#define RTICOMP0_COMP0_BIT_START                            0U
#define RTICOMP0_COMP0_BIT_END                              31U

/* Definition for field UDCP0 in Register RTIUDCP0 */
#define RTIUDCP0_UDCP0_BIT_START                            0U
#define RTIUDCP0_UDCP0_BIT_END                              31U

/* Definition for field COMP1 in Register RTICOMP1 */
#define RTICOMP1_COMP1_BIT_START                            0U
#define RTICOMP1_COMP1_BIT_END                              31U

/* Definition for field UDCP1 in Register RTIUDCP1 */
#define RTIUDCP1_UDCP1_BIT_START                            0U
#define RTIUDCP1_UDCP1_BIT_END                              31U

/* Definition for field COMP2 in Register RTICOMP2 */
#define RTICOMP2_COMP2_BIT_START                            0U
#define RTICOMP2_COMP2_BIT_END                              31U

/* Definition for field UDCP2 in Register RTIUDCP2 */
#define RTIUDCP2_UDCP2_BIT_START                            0U
#define RTIUDCP2_UDCP2_BIT_END                              31U

/* Definition for field COMP3 in Register RTICOMP3 */
#define RTICOMP3_COMP3_BIT_START                            0U
#define RTICOMP3_COMP3_BIT_END                              31U

/* Definition for field UDCP3 in Register RTIUDCP3 */
#define RTIUDCP3_UDCP3_BIT_START                            0U
#define RTIUDCP3_UDCP3_BIT_END                              31U

/* Definition for field TBLCOMP in Register RTITBLCOMP */
#define RTITBLCOMP_TBLCOMP_BIT_START                        0U
#define RTITBLCOMP_TBLCOMP_BIT_END                          31U

/* Definition for field TBHCOMP in Register RTITBHCOMP */
#define RTITBHCOMP_TBHCOMP_BIT_START                        0U
#define RTITBHCOMP_TBHCOMP_BIT_END                          31U

/* Definition for field SETINT0 in Register RTISETINT */
#define RTISETINT_SETINT0_BIT_START                         0U
#define RTISETINT_SETINT0_BIT_END                           0U

/* Definition for field SETINT1 in Register RTISETINT */
#define RTISETINT_SETINT1_BIT_START                         1U
#define RTISETINT_SETINT1_BIT_END                           1U

/* Definition for field SETINT2 in Register RTISETINT */
#define RTISETINT_SETINT2_BIT_START                         2U
#define RTISETINT_SETINT2_BIT_END                           2U

/* Definition for field SETINT3 in Register RTISETINT */
#define RTISETINT_SETINT3_BIT_START                         3U
#define RTISETINT_SETINT3_BIT_END                           3U

/* Definition for field SETDMA0 in Register RTISETINT */
#define RTISETINT_SETDMA0_BIT_START                         8U
#define RTISETINT_SETDMA0_BIT_END                           8U

/* Definition for field SETDMA1 in Register RTISETINT */
#define RTISETINT_SETDMA1_BIT_START                         9U
#define RTISETINT_SETDMA1_BIT_END                           9U

/* Definition for field SETDMA2 in Register RTISETINT */
#define RTISETINT_SETDMA2_BIT_START                         10U
#define RTISETINT_SETDMA2_BIT_END                           10U

/* Definition for field SETDMA3 in Register RTISETINT */
#define RTISETINT_SETDMA3_BIT_START                         11U
#define RTISETINT_SETDMA3_BIT_END                           11U

/* Definition for field SETTBINT in Register RTISETINT */
#define RTISETINT_SETTBINT_BIT_START                        16U
#define RTISETINT_SETTBINT_BIT_END                          16U

/* Definition for field SETOVL0INT in Register RTISETINT */
#define RTISETINT_SETOVL0INT_BIT_START                      17U
#define RTISETINT_SETOVL0INT_BIT_END                        17U

/* Definition for field SETOVL1INT in Register RTISETINT */
#define RTISETINT_SETOVL1INT_BIT_START                      18U
#define RTISETINT_SETOVL1INT_BIT_END                        18U

/* Definition for field CLEARINT0 in Register RTICLEARINT */
#define RTICLEARINT_CLEARINT0_BIT_START                     0U
#define RTICLEARINT_CLEARINT0_BIT_END                       0U

/* Definition for field CLEARINT1 in Register RTICLEARINT */
#define RTICLEARINT_CLEARINT1_BIT_START                     1U
#define RTICLEARINT_CLEARINT1_BIT_END                       1U

/* Definition for field CLEARINT2 in Register RTICLEARINT */
#define RTICLEARINT_CLEARINT2_BIT_START                     2U
#define RTICLEARINT_CLEARINT2_BIT_END                       2U

/* Definition for field CLEARINT3 in Register RTICLEARINT */
#define RTICLEARINT_CLEARINT3_BIT_START                     3U
#define RTICLEARINT_CLEARINT3_BIT_END                       3U

/* Definition for field CLEARDMA0 in Register RTICLEARINT */
#define RTICLEARINT_CLEARDMA0_BIT_START                     8U
#define RTICLEARINT_CLEARDMA0_BIT_END                       8U

/* Definition for field CLEARDMA1 in Register RTICLEARINT */
#define RTICLEARINT_CLEARDMA1_BIT_START                     9U
#define RTICLEARINT_CLEARDMA1_BIT_END                       9U

/* Definition for field CLEARDMA2 in Register RTICLEARINT */
#define RTICLEARINT_CLEARDMA2_BIT_START                     10U
#define RTICLEARINT_CLEARDMA2_BIT_END                       10U

/* Definition for field CLEARDMA3 in Register RTICLEARINT */
#define RTICLEARINT_CLEARDMA3_BIT_START                     11U
#define RTICLEARINT_CLEARDMA3_BIT_END                       11U

/* Definition for field CLEARTBINT in Register RTICLEARINT */
#define RTICLEARINT_CLEARTBINT_BIT_START                    16U
#define RTICLEARINT_CLEARTBINT_BIT_END                      16U

/* Definition for field CLEAROVL0INT in Register RTICLEARINT */
#define RTICLEARINT_CLEAROVL0INT_BIT_START                  17U
#define RTICLEARINT_CLEAROVL0INT_BIT_END                    17U

/* Definition for field CLEAROVL1INT in Register RTICLEARINT */
#define RTICLEARINT_CLEAROVL1INT_BIT_START                  18U
#define RTICLEARINT_CLEAROVL1INT_BIT_END                    18U

/* Definition for field INT0 in Register RTIINTFLAG */
#define RTIINTFLAG_INT0_BIT_START                           0U
#define RTIINTFLAG_INT0_BIT_END                             0U

/* Definition for field INT1 in Register RTIINTFLAG */
#define RTIINTFLAG_INT1_BIT_START                           1U
#define RTIINTFLAG_INT1_BIT_END                             1U

/* Definition for field INT2 in Register RTIINTFLAG */
#define RTIINTFLAG_INT2_BIT_START                           2U
#define RTIINTFLAG_INT2_BIT_END                             2U

/* Definition for field INT3 in Register RTIINTFLAG */
#define RTIINTFLAG_INT3_BIT_START                           3U
#define RTIINTFLAG_INT3_BIT_END                             3U

/* Definition for field TBINT in Register RTIINTFLAG */
#define RTIINTFLAG_TBINT_BIT_START                          16U
#define RTIINTFLAG_TBINT_BIT_END                            16U

/* Definition for field OVL0INT in Register RTIINTFLAG */
#define RTIINTFLAG_OVL0INT_BIT_START                        17U
#define RTIINTFLAG_OVL0INT_BIT_END                          17U

/* Definition for field OVL1INT in Register RTIINTFLAG */
#define RTIINTFLAG_OVL1INT_BIT_START                        18U
#define RTIINTFLAG_OVL1INT_BIT_END                          18U

/* Definition for field DWDCTRL in Register RTIDWDCTRL */
#define RTIDWDCTRL_DWDCTRL_BIT_START                        0U
#define RTIDWDCTRL_DWDCTRL_BIT_END                          31U

/* Definition for field DWDPRLD in Register RTIDWDPRLD */
#define RTIDWDPRLD_DWDPRLD_BIT_START                        0U
#define RTIDWDPRLD_DWDPRLD_BIT_END                          11U

/* Definition for field AWDST in Register RTIWDSTATUS */
#define RTIWDSTATUS_AWDST_BIT_START                         0U
#define RTIWDSTATUS_AWDST_BIT_END                           0U

/* Definition for field DWDST in Register RTIWDSTATUS */
#define RTIWDSTATUS_DWDST_BIT_START                         1U
#define RTIWDSTATUS_DWDST_BIT_END                           1U

/* Definition for field KEYST in Register RTIWDSTATUS */
#define RTIWDSTATUS_KEYST_BIT_START                         2U
#define RTIWDSTATUS_KEYST_BIT_END                           2U

/* Definition for field STARTTIMEVIOL in Register RTIWDSTATUS */
#define RTIWDSTATUS_STARTTIMEVIOL_BIT_START                 3U
#define RTIWDSTATUS_STARTTIMEVIOL_BIT_END                   3U

/* Definition for field ENDTIMEVIOL in Register RTIWDSTATUS */
#define RTIWDSTATUS_ENDTIMEVIOL_BIT_START                   4U
#define RTIWDSTATUS_ENDTIMEVIOL_BIT_END                     4U

/* Definition for field DWWD_ST in Register RTIWDSTATUS */
#define RTIWDSTATUS_DWWD_ST_BIT_START                       5U
#define RTIWDSTATUS_DWWD_ST_BIT_END                         5U

/* Definition for field WDKEY in Register RTIWDKEY */
#define RTIWDKEY_WDKEY_BIT_START                            0U
#define RTIWDKEY_WDKEY_BIT_END                              15U

/* Definition for field DWDCNTR in Register RTIDWDCNTR */
#define RTIDWDCNTR_DWDCNTR_BIT_START                        0U
#define RTIDWDCNTR_DWDCNTR_BIT_END                          24U

/* Definition for field WWDRXN in Register RTIWWDRXNCTRL */
#define RTIWWDRXNCTRL_WWDRXN_BIT_START                      0U
#define RTIWWDRXNCTRL_WWDRXN_BIT_END                        3U

/* Definition for field WWDSIZE in Register RTIWWDSIZECTRL */
#define RTIWWDSIZECTRL_WWDSIZE_BIT_START                    0U
#define RTIWWDSIZECTRL_WWDSIZE_BIT_END                      31U

/* Definition for field INTCLRENABLE0 in Register RTIINTCLRENABLE */
#define RTIINTCLRENABLE_INTCLRENABLE0_BIT_START             0U
#define RTIINTCLRENABLE_INTCLRENABLE0_BIT_END               3U

/* Definition for field INTCLRENABLE1 in Register RTIINTCLRENABLE */
#define RTIINTCLRENABLE_INTCLRENABLE1_BIT_START             8U
#define RTIINTCLRENABLE_INTCLRENABLE1_BIT_END               11U

/* Definition for field INTCLRENABLE2 in Register RTIINTCLRENABLE */
#define RTIINTCLRENABLE_INTCLRENABLE2_BIT_START             16U
#define RTIINTCLRENABLE_INTCLRENABLE2_BIT_END               19U

/* Definition for field INTCLRENABLE3 in Register RTIINTCLRENABLE */
#define RTIINTCLRENABLE_INTCLRENABLE3_BIT_START             24U
#define RTIINTCLRENABLE_INTCLRENABLE3_BIT_END               27U

/* Definition for field COMP0CLR in Register RTICOMP0CLR */
#define RTICOMP0CLR_COMP0CLR_BIT_START                      0U
#define RTICOMP0CLR_COMP0CLR_BIT_END                        31U

/* Definition for field COMP1CLR in Register RTICOMP1CLR */
#define RTICOMP1CLR_COMP1CLR_BIT_START                      0U
#define RTICOMP1CLR_COMP1CLR_BIT_END                        31U

/* Definition for field COMP2CLR in Register RTICOMP2CLR */
#define RTICOMP2CLR_COMP2CLR_BIT_START                      0U
#define RTICOMP2CLR_COMP2CLR_BIT_END                        31U

/* Definition for field COMP3CLR in Register RTICOMP3CLR */
#define RTICOMP3CLR_COMP3CLR_BIT_START                      0U
#define RTICOMP3CLR_COMP3CLR_BIT_END                        31U



/**
 * @struct RTIBRegs_t
 * @brief
 *   Module MSS_RTIB Register Definition
 * @details
 *   This structure is used to access the MSS_RTIB module registers.
 */
/**
 * @typedef RTIBRegs
 * @brief
 *   Module MSS_RTIB Register Frame type Definition
 * @details
 *   This type is used to access the MSS_RTIB module registers.
 */
typedef volatile struct RTIBRegs_t
{
    uint32_t    RTIGCTRL                           ;        /* Offset = 0x000 */
    uint32_t    RTITBCTRL                          ;        /* Offset = 0x004 */
    uint32_t    RTICAPCTRL                         ;        /* Offset = 0x008 */
    uint32_t    RTICOMPCTRL                        ;        /* Offset = 0x00C */
    uint32_t    RTIFRC0                            ;        /* Offset = 0x010 */
    uint32_t    RTIUC0                             ;        /* Offset = 0x014 */
    uint32_t    RTICPUC0                           ;        /* Offset = 0x018 */
    uint32_t    RESTRICTED1                        ;        /* Offset = 0x01C */
    uint32_t    RTICAFRC0                          ;        /* Offset = 0x020 */
    uint32_t    RTICAUC0                           ;        /* Offset = 0x024 */
    uint32_t    RESTRICTED2[2]                     ;        /* Offset = 0x028 */
    uint32_t    RTIFRC1                            ;        /* Offset = 0x030 */
    uint32_t    RTIUC1                             ;        /* Offset = 0x034 */
    uint32_t    RTICPUC1                           ;        /* Offset = 0x038 */
    uint32_t    RESTRICTED3                        ;        /* Offset = 0x03C */
    uint32_t    RTICAFRC1                          ;        /* Offset = 0x040 */
    uint32_t    RTICAUC1                           ;        /* Offset = 0x044 */
    uint32_t    RESTRICTED4[2]                     ;        /* Offset = 0x048 */
    uint32_t    RTICOMP0                           ;        /* Offset = 0x050 */
    uint32_t    RTIUDCP0                           ;        /* Offset = 0x054 */
    uint32_t    RTICOMP1                           ;        /* Offset = 0x058 */
    uint32_t    RTIUDCP1                           ;        /* Offset = 0x05C */
    uint32_t    RTICOMP2                           ;        /* Offset = 0x060 */
    uint32_t    RTIUDCP2                           ;        /* Offset = 0x064 */
    uint32_t    RTICOMP3                           ;        /* Offset = 0x068 */
    uint32_t    RTIUDCP3                           ;        /* Offset = 0x06C */
    uint32_t    RTITBLCOMP                         ;        /* Offset = 0x070 */
    uint32_t    RTITBHCOMP                         ;        /* Offset = 0x074 */
    uint32_t    RESTRICTED5[2]                     ;        /* Offset = 0x078 */
    uint32_t    RTISETINT                          ;        /* Offset = 0x080 */
    uint32_t    RTICLEARINT                        ;        /* Offset = 0x084 */
    uint32_t    RTIINTFLAG                         ;        /* Offset = 0x088 */
    uint32_t    RESTRICTED6                        ;        /* Offset = 0x08C */
    uint32_t    RTIDWDCTRL                         ;        /* Offset = 0x090 */
    uint32_t    RTIDWDPRLD                         ;        /* Offset = 0x094 */
    uint32_t    RTIWDSTATUS                        ;        /* Offset = 0x098 */
    uint32_t    RTIWDKEY                           ;        /* Offset = 0x09C */
    uint32_t    RTIDWDCNTR                         ;        /* Offset = 0x0A0 */
    uint32_t    RTIWWDRXNCTRL                      ;        /* Offset = 0x0A4 */
    uint32_t    RTIWWDSIZECTRL                     ;        /* Offset = 0x0A8 */
    uint32_t    RTIINTCLRENABLE                    ;        /* Offset = 0x0AC */
    uint32_t    RTICOMP0CLR                        ;        /* Offset = 0x0B0 */
    uint32_t    RTICOMP1CLR                        ;        /* Offset = 0x0B4 */
    uint32_t    RTICOMP2CLR                        ;        /* Offset = 0x0B8 */
    uint32_t    RTICOMP3CLR                        ;        /* Offset = 0x0BC */
} RTIBRegs;

#ifdef __cplusplus
}
#endif

#endif /* REG_RTIB_H */
/* END OF REG_RTIB_H */

