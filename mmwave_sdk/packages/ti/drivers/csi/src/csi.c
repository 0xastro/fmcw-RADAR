/**
 *   @file  csi.c
 *
 *   @brief
 *      The file implements the CSI Driver
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/csi/include/reg_csi2phy.h>
#include <ti/drivers/csi/include/reg_csi2protocolengine.h>
#include <ti/drivers/csi/include/csi_internal.h>
#include <ti/drivers/csi/csi.h>

/**************************************************************************
 *************************** Local Functions ******************************
 **************************************************************************/

static void    CSI_setupClocks      (CSI_DriverMCB* ptrCSIDriver);
static void    CSI_setupPhy         (CSI_DriverMCB* ptrCSIDriver, CSI_PhyParams* ptrParams);
static void    CSI_setupOCPConfig   (CSI_DriverMCB* ptrCSIDriver);
static void    CSI_setupClockTimings(CSI_DriverMCB* ptrCSIDriver);
static void    CSI_setupTimingTA    (CSI_DriverMCB* ptrCSIDriver, uint32_t forceTxStopModeIO);
static void    CSI_reset            (CSI_DriverMCB* ptrCSIDriver);
static void    CSI_setupComplexIO   (CSI_DriverMCB* ptrCSIDriver);
static void    CSI_setupControl     (CSI_DriverMCB* ptrCSIDriver, uint32_t ifEnable);
static int32_t CSI_setupVCFIFO      (CSI_DriverMCB* ptrCSIDriver);
static void    CSI_configVC         (CSI_DriverMCB* ptrCSIDriver, uint32_t enableChannel);
static int32_t CSI_getHSIClock      (CSI_DataRate dataRate, uint32_t* hsiClock);

/**************************************************************************
 *************************** CSI Driver Functions *************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to get the HSI clock from the specified data rate
 *
 *  @param[in]  dataRate
 *      CSI Data Rate
 *  @param[out] hsiClock
 *      Populated HSI Clock to be used.
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t CSI_getHSIClock (CSI_DataRate dataRate, uint32_t* hsiClock)
{
    int32_t retVal = 0;

    /* Initialize and setup the HSI Clock: */
    switch (dataRate)
    {
        case CSI_DataRate_600Mbps:
        {
            *hsiClock = 1200U;
            break;
        }
        case CSI_DataRate_450Mbps:
        {
            *hsiClock = 900U;
            break;
        }
        case CSI_DataRate_400Mbps:
        {
            *hsiClock = 800U;
            break;
        }
        case CSI_DataRate_300Mbps:
        {
            *hsiClock = 600U;
            break;
        }
        case CSI_DataRate_225Mbps:
        {
            *hsiClock = 450U;
            break;
        }
        case CSI_DataRate_150Mbps:
        {
            *hsiClock = 300U;
            break;
        }
        default:
        {
            /* Error: This is an invalid data rate specified: */
            retVal = -1;
            break;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the clocks
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_setupClocks (CSI_DriverMCB* ptrCSIDriver)
{
#if 0
    {        /* dsiClk_t */
        CHNGE_2_ON_PLL_HSDIVISER,
        1U, /* lpRxSynchroEnable */
        1U, /* lpClkEnable */
        0U, /* hsManualStopCtrl */
        0U, /* hsAutoStopEnable */
        0U, /* lpClkNullPktSize */
        0U, /* lpClkNullPktEn */
        1U, /* cioClkIcg */
        1U, /* ddrClkAlwaysOn */
        10U /* lpClkDivisor */
    },

    dsiPllPwrCmdStatus_t  pllPwrCmd;          /*Command for power control of the CSI2 PLL Control module*/
    uint8                 lpRxSynchroEnable;  /*Defines if the functional is higher or lower than 30 MHz.
                                               The information is used to define synchronization to be
                                               used for RxValidEsc*/
    uint8                 lpClkEnable;        /*Controls the gating of the TXCLKESC clock*/
    uint8                 hsManualStopCtrl;   /*In case HS_AUTO_STOP_ENABLE=0, the bitfield allows manual
                                                 control of the assertion/deassertion of the signal CSI2StopClk
                                                 by the user*/
    uint8                 hsAutoStopEnable;   /*Enables the automatic assertion/de-assertion of CSI2StopClk signal*/
    uint8                 lpClkNullPktSize;   /*Indicates the size of LP NULL Packets to be sent automatically
                                                when after the last LP packet transfer. It is used by the receiver
                                                to drain its internal pipeline*/
    uint8                 lpClkNullPktEn;     /*Enables the generation of NULL packet in low speed*/
    uint8                 cioClkIcg;          /*Gates SCPClk clock provided to CSI2-PHY and PLL-CTRL module*/
    uint8                 ddrClkAlwaysOn;     /*Defines if the DDR clock is also sent when there is no HS packets
                                                sent to the peripheral (low power mode). So TXRequest for the
                                                clock lane is not deasserted*/
    uint8                 lpClkDivisor;       /*Defines the ratio to be used for the generation of the Low Power
                                                mode clock from CSI2 functional clock*/
#endif
    /* Setup the CSI2 Clock Control:
     * - PLL Power command is set o STATE_ON_ALL
     * -
     * - LP Clock is enabled
     * - [Reset Value] HS Manual Stop control is deasserted
     * - [Reset Value] HS Auto Stop control is deasserted
     * - [Reset Value] Size of the LP NULL packets to be sent is 0
     * - [Reset Value] Disable the generation of NULL packets in low speed
     * - Enable the clock generation to the SCP as OCP_CLK/4
     * - DDR clock is always sent to the peripheral
     * - LP Clock divisor is set to 10  */
    ptrCSIDriver->ptrProtReg->CSI2_CLK_CTRL = CSL_FMKR (31U, 30U, 2U) |
                                             CSL_FMKR (21U, 21U, 1U) |
                                             CSL_FMKR (20U, 20U, 1U) |
                                             CSL_FMKR (19U, 19U, 0U) |
                                             CSL_FMKR (18U, 18U, 0U) |
                                             CSL_FMKR (17U, 16U, 0U) |
                                             CSL_FMKR (15U, 15U, 0U) |
                                             CSL_FMKR (14U, 14U, 1U) |
                                             CSL_FMKR (13U, 13U, 1U) |
                                             CSL_FMKR (12U, 0U,  10U);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default PHY parameters
 *
 *  @param[in]  dataRate
 *      Data Rate for which the PHY parameters will be configured
 *  @param[out]  ptrPhyParams
 *      Default PHY Parameters populated by the API
 *
 *  \ingroup CSI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
int32_t CSI_phyParamsInit (CSI_DataRate dataRate, CSI_PhyParams* ptrPhyParams)
{
#if 0
    /* csi2CompIoCfg */
    {
        /* dsiComplexIoReg0CIOCfg_t -> dsiComplexIoReg0Strct */
        {THS_PREPARE, THS_PREPARE_ZERO, THS_TRAIL, THS_EXIT},

        /* dsiComplexIoReg1CIOCfg_t -> dsiComplexIoReg1Strct */
        {0U, 0U, 0U, TLPX_HALF, TCLK_TRAIL, TCLK_ZERO, 0U, 0U, 1U},

        /* dsiComplexIoReg2CIOCfg_t -> dsiComplexIoReg2Strct */
        {0xB8U, 1U, 0U, 0U, 0U, 0U, TCLK_PREPARE},
        ...
    }
#endif
    uint8_t     thsPrepare;
    uint8_t     thsPrepareZero;
    uint8_t     thsTrail;
    uint8_t     thsExit;
    uint8_t     tlpxHalf;
    uint8_t     tclkTrail;
    uint8_t     tclkZero;
    uint8_t     tclkPrepare;
    uint32_t    hsiClock;
    int32_t     retVal = 0;

    /* Get the HSI Clock: */
    retVal = CSI_getHSIClock (dataRate, &hsiClock);
    if (retVal == 0)
    {
        /*************************************************************************
         * THS PREPARE calculation:
         *************************************************************************/
        thsPrepare = (uint8_t)((70U * (hsiClock/(uint32_t)4U) / (uint32_t)1000U) + (uint32_t)2U);

        /*************************************************************************
         * THS_PREPARE_ZERO calculation:
         *************************************************************************/
        if(hsiClock == 300U)
        {
            thsPrepareZero = 10U;
        }
        else if(hsiClock == 450U)
        {
            thsPrepareZero = 14U;
        }
        else if(hsiClock == 900U)
        {
            thsPrepareZero = 36U;
        }
        else
        {
            thsPrepareZero = (uint8_t)((175U*(hsiClock/(uint32_t)4U) / (uint32_t)1000U) + (uint32_t)2U);
        }

        /*************************************************************************
         * THS_TRAIL calculation:
         *************************************************************************/
        thsTrail = (uint8_t)((60U*(hsiClock/(uint32_t)4U) / (uint32_t)1000U) + (uint32_t)5U);

        /*************************************************************************
         * THS_EXIT calculation:
         *************************************************************************/
        thsExit = (uint8_t)(145U*(hsiClock/(uint32_t)4U) / (uint32_t)1000U);

        /*************************************************************************
         * TLPX_HALF calculation:
         *************************************************************************/
        tlpxHalf = (uint8_t)(30U * (hsiClock /(uint32_t)4U) / (uint32_t)1000U);

        /*************************************************************************
         * TCLK TRAIL calculation:
         *************************************************************************/
        tclkTrail = (uint8_t)(80U * (hsiClock /(uint32_t)4U) / (uint32_t)1000U);

        /*************************************************************************
         * TCLK ZERO calculation:
         *************************************************************************/
        tclkZero = (uint8_t)(265U * (hsiClock / 4U) / 1000U);

        /*************************************************************************
         * TCLK PREPARE calculation:
         *************************************************************************/
        tclkPrepare = (uint8_t)(65U*(hsiClock/(uint32_t)4U) / (uint32_t)1000U);

        /* Configure PHY REGISTER0: */
        ptrPhyParams->phyRegister[0] = CSL_FMKR (31U, 24U, thsPrepare)      |
                                       CSL_FMKR (23U, 16U, thsPrepareZero)  |
                                       CSL_FMKR (15U, 8U,  thsTrail)        |
                                       CSL_FMKR (7U,  0U,  thsExit);

        /* Configure PHY REGISTER1: */
        ptrPhyParams->phyRegister[1] = CSL_FMKR (31U, 29U, 0U)              |
                                       CSL_FMKR (28U, 27U, 0U)              |
                                       CSL_FMKR (26U, 24U, 0U)              |
                                       CSL_FMKR (20U, 16U, tlpxHalf)        |
                                       CSL_FMKR (15U,  8U, tclkTrail)       |
                                       CSL_FMKR (7U,   0U, tclkZero);

        /* Configure PHY REGISTER2: */
        ptrPhyParams->phyRegister[2] = CSL_FMKR (31U, 24U, 0xB8U)           |
                                       CSL_FMKR (23U, 23U, 1U)              |
                                       CSL_FMKR (22U, 22U, 0U)              |
                                       CSL_FMKR (21U, 17U, 0U)              |
                                       CSL_FMKR (16U, 16U, 0U)              |
                                       CSL_FMKR (15U, 11U, 0U)              |
                                       CSL_FMKR (7U,  0U,  tclkPrepare);

        /* These are the values of the registers after reset. */
        ptrPhyParams->phyRegister[3]  = 0x625D21A0U;
        ptrPhyParams->phyRegister[4]  = 0x625D21A0U;
        ptrPhyParams->phyRegister[5]  = 0x00000000U;
        ptrPhyParams->phyRegister[6]  = 0x00010000U;
        ptrPhyParams->phyRegister[7]  = 0x00000000U;
        ptrPhyParams->phyRegister[8]  = 0x00000410U;
        ptrPhyParams->phyRegister[9]  = 0x00020001U;
        ptrPhyParams->phyRegister[10] = 0x00000000U;
        ptrPhyParams->phyRegister[11] = 0x7D20A59AU;
        ptrPhyParams->phyRegister[12] = 0x04320000U;
        ptrPhyParams->phyRegister[13] = 0x00000000U;
        ptrPhyParams->phyRegister[14] = 0x3E080000U;
        ptrPhyParams->phyRegister[15] = 0x01400003U;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the CSI2 PHY Registers
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *  @param[in]  ptrParams
 *      Pointer to the PHY Parameters to be setup
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_setupPhy (CSI_DriverMCB* ptrCSIDriver, CSI_PhyParams* ptrParams)
{
    uint32_t dummy;
    uint32_t index;

    /* Configure PHY Register(s): */
    for (index = 0U; index < 16U; index++)
    {
        ptrCSIDriver->ptrPhyReg->REGISTER[index] = ptrParams->phyRegister[index];
    }

    /* Perform a dummy read of the SCP Interface */
    for (index = 0U; index < 16U; index++)
    {
        /* Perform a dummy read: */
        dummy = ptrCSIDriver->ptrPhyReg->REGISTER[index];
        dummy = dummy;

        /* Debug: Display the PHY Register */
        DebugP_log3 ("Debug: CSI Phy Register[%d] [0x%x] = 0x%08x\n", index, &ptrCSIDriver->ptrPhyReg->REGISTER[index], dummy);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the complex IO configuration which
 *      controls the lane configuration, order and position of the lanes
 *      and the polarity order.
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_setupComplexIO (CSI_DriverMCB* ptrCSIDriver)
{
#if 0
    /* csi2CompIoCfg */
    {
        ...

        /* dsiComplexIoCIOCfg1_t    -> dsiComplexIoCfg1Strct */
        {
            1U, 1U, PWER_CMD_IN_ON_STATE, 0U,
            DATA4_POL, DATA_LANE_AT_POSITION_5,
            DATA3_POL, DATA_LANE_AT_POSITION_4,
            DATA2_POL, DATA_LANE_AT_POSITION_2,
            DATA1_POL, DATA_LANE_AT_POSITION_1,
            CLOCK_POL, CLOCK_LANE_AT_POS_3
        },

        /* dsiComplexIoCIOCfg2_t    -> dsiComplexIoCfg2Strct */
        {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U}
    }
#endif
    /* Configure the CSI2 Complex IO configuration:
     * - Shadowing is enabled
     * - Go bit is enabled which allows synchronized update of the shadow registers
     * - Power command is set to STATE_ON
     * - [Reset Value] Lane4 Polarity is set to PLUSMINUS
     * - Data Lane4 is at position 5
     * - [Reset Value] Lane3 Polarity is set to PLUSMINUS
     * - Data Lane3 is at position 4
     * - [Reset Value] Lane2 Polarity is set to PLUSMINUS
     * - Data Lane2 is at position 2
     * - [Reset Value] Lane1 Polarity is set to PLUSMINUS
     * - Data Lane1 is at position 1
     * - [Reset Value] Clock Polarity is set to PLUSMINUS
     * - Clock is at position 3 */
    ptrCSIDriver->ptrProtReg->CSI2_COMPLEXIO_CFG1 = CSL_FMKR (31U, 31U, 1U)  |
                                                   CSL_FMKR (30U, 30U, 1U)  |
                                                   CSL_FMKR (28U, 27U, 1U)  |
                                                   CSL_FMKR (20U, 20U, 0U)  |
                                                   CSL_FMKR (19U, 19U, 0U)  |
                                                   CSL_FMKR (18U, 16U, 5U)  |
                                                   CSL_FMKR (15U, 15U, 0U)  |
                                                   CSL_FMKR (14U, 12U, 4U)  |
                                                   CSL_FMKR (11U, 11U, 0U)  |
                                                   CSL_FMKR (10U, 8U,  2U)  |
                                                   CSL_FMKR (7U,  7U,  0U)  |
                                                   CSL_FMKR (6U,  4U,  1U)  |
                                                   CSL_FMKR (3U,  3U,  0U)  |
                                                   CSL_FMKR (2U,  0U,  3U);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the CSI2 Control which is used to
 *      control the CSI2 protocol engine module
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *  @param[in]  ifEnable
 *      Status flag which indicates if the interface is to enabled or
 *      disabled
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_setupControl (CSI_DriverMCB* ptrCSIDriver, uint32_t ifEnable)
{
    uint32_t fifoMode = (uint32_t)ptrCSIDriver->cfg.fifoArbiteration;

    /* Configure the CSI2 Control:
     * - [Reset Value] Disable the display update sync usage
     * - [Reset Value] HSA Blanking Mode: Packets in the Tx FIFO are sent during the blanking mode
     * - [Reset Value] HBP Blanking Mode: Packets in the Tx FIFO are sent during the blanking mode
     * - [Reset Value] HFP Blanking Mode: Packets in the Tx FIFO are sent during the blanking mode
     * - [Reset Value] LPS is used during the blanking periods
     * - [Reset Value] Enable EOT
     * - [Reset Value] HSYNC end pulse is disabled
     * - [Reset Value] HSYNC start pulse is disabled
     * - [Reset Value] VSYNC end pulse is disabled
     * - [Reset Value] VSYNC start pulse is disabled
     * - [Reset Value] Immediate Trigger reset mode
     * - [Reset Value] No Line Buffer
     * - [Reset Value] Active Low VSYNC signal on the video port
     * - [Reset Value] Active Low HSYNC signal on the video port
     * - [Reset Value] Active Low Data Enable signal on the video port
     * - Video Port Clock Polarity is active high [XXX: Documentation]
     * - [Reset Value] 16bits data width of the video port data bus
     * - [Reset Value] Cancel the request trigger to the peripheral
     * - Setup the FIFO Arbiteration mode as per the configuration
     * - [Reset Value] Disable the ECC check for received header
     * - [Reset Value] Disable the checksum check for received payload
     * - Enable/Disable the channel as per the configuration */
    ptrCSIDriver->ptrProtReg->CSI2_CTRL = CSL_FMKR (24U, 24U, 0U)        |
                                         CSL_FMKR (23U, 23U, 0U)        |
                                         CSL_FMKR (22U, 22U, 0U)        |
                                         CSL_FMKR (21U, 21U, 0U)        |
                                         CSL_FMKR (20U, 20U, 0U)        |
                                         CSL_FMKR (19U, 19U, 0U)        |
                                         CSL_FMKR (18U, 18U, 0U)        |
                                         CSL_FMKR (17U, 17U, 0U)        |
                                         CSL_FMKR (16U, 16U, 0U)        |
                                         CSL_FMKR (15U, 15U, 0U)        |
                                         CSL_FMKR (14U, 14U, 1U)        |
                                         CSL_FMKR (13U, 12U, 0U)        |
                                         CSL_FMKR (11U, 11U, 0U)        |
                                         CSL_FMKR (10U, 10U, 0U)        |
                                         CSL_FMKR (9U,  9U,  0U)        |
                                         CSL_FMKR (8U,  8U,  1U)        |
                                         CSL_FMKR (7U,  6U,  0U)        |
                                         CSL_FMKR (5U,  5U,  0U)        |
                                         CSL_FMKR (3U,  3U,  fifoMode)  |
                                         CSL_FMKR (2U,  2U,  0U)        |
                                         CSL_FMKR (1U,  1U,  0U)        |
                                         CSL_FMKR (0U,  0U,  ifEnable);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the CSI2 OCP system configuration
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_setupOCPConfig (CSI_DriverMCB* ptrCSIDriver)
{
    /* Configure the OCP System configuration:
     * - [Reset Value] OCP & Functional clocks are off
     * - Slave Interface power management is force idle
     * - Wakeup mode is enabled
     * - OCP clock is free running */
    ptrCSIDriver->ptrProtReg->CSI2_SYSCONFIG = CSL_FMKR (9U, 8U, 0U)     |
                                              CSL_FMKR (4U, 3U, 0U)     |
                                              CSL_FMKR (2U, 2U, 1U)     |
                                              CSL_FMKR (0U, 0U, 0U);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to perform the reset of the CSI Interface
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_reset (CSI_DriverMCB* ptrCSIDriver)
{
    /* Software reset the CSI Protocol Engine: */
    ptrCSIDriver->ptrProtReg->CSI2_SYSCONFIG = CSL_FINSR (ptrCSIDriver->ptrProtReg->CSI2_SYSCONFIG, 1U, 1U, 1U);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the virtual channel FIFO configuration
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Number of virtual channels configured
 *  @retval
 *      Error   -   <0
 */
static int32_t CSI_setupVCFIFO (CSI_DriverMCB* ptrCSIDriver)
{
    uint32_t    index;
    uint32_t    lastActiveCh;
    uint32_t    rxFIFOSize = 0U;
    uint32_t    txFIFOSize = 0U;
    int32_t     retVal = 0;
    uint32_t    rxFIFOAddress[CSI_VirtualCh_MAX_CHANNEL];
    uint32_t    txFIFOAddress[CSI_VirtualCh_MAX_CHANNEL];

    /* Cycle through the virtual channel configuration block: */
    for (index = 0U; index < CSI_VirtualCh_MAX_CHANNEL; index++)
    {
        /* Is there a valid channel configuration specified? */
        if (ptrCSIDriver->cfg.virtualChannelCfg[index].channelId != CSI_VirtualCh_INVALID)
        {
            /* YES: Track the receive & transmit FIFO sizes for all the configured channels */
            rxFIFOSize = rxFIFOSize + (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[index].rxFIFOSize;
            txFIFOSize = txFIFOSize + (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[index].txFIFOSize;

            /* Mark the virtual channel as this is now active: */
            ptrCSIDriver->virtualChannel[index].isActive   = 1U;
            ptrCSIDriver->virtualChannel[index].isTxActive = 0U;

            /* Track the number of virtual channels: */
            retVal++;
        }
    }

    /* Sanity Check: As per the specifications the FIFO size(s) cannot exceed 128*33 bits */
    if ((rxFIFOSize > (uint32_t)CSI_VC_FIFO_Size_128) || (txFIFOSize > (uint32_t)CSI_VC_FIFO_Size_128))
    {
        /* Error: Invalid configuration */
        retVal = -1;
    }
    else
    {
        /* Reset the FIFO Address allocations: */
        memset ((void *)&rxFIFOAddress[0], 0, sizeof(rxFIFOAddress));
        memset ((void *)&txFIFOAddress[0], 0, sizeof(txFIFOAddress));

        /* Initialize the last active channel: */
        lastActiveCh = (uint32_t)CSI_VirtualCh_INVALID;

        /* Cycle through all the virtual channels: */
        for (index = 0U; index < (uint32_t)CSI_VirtualCh_MAX_CHANNEL; index++)
        {
            /* Is the virtual channel active? */
            if (ptrCSIDriver->virtualChannel[index].isActive == 1U)
            {
                /* YES: Is this the first active channel? */
                if (lastActiveCh == (uint32_t)CSI_VirtualCh_INVALID)
                {
                    /* YES: The RX & TX FIFO Address start from 0 */
                    rxFIFOAddress[index] = 0;
                    txFIFOAddress[index] = 0;
                }
                else
                {
                    /* YES: Track the FIFO Address allocations from the last active channel */
                    rxFIFOAddress[index] = rxFIFOAddress[lastActiveCh] +
                                           (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[lastActiveCh].rxFIFOSize;
                    txFIFOAddress[index] = txFIFOAddress[lastActiveCh] +
                                           (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[lastActiveCh].txFIFOSize;
                }
                /* Remember the last active channel: */
                lastActiveCh = index;
            }
        }

        /* Debug: Display the FIFO Size and Address */
        for (index = 0U; index < (uint32_t)CSI_VirtualCh_MAX_CHANNEL; index++)
        {
            /* Is the virtual channel active? */
            if (ptrCSIDriver->virtualChannel[index].isActive == 1U)
            {
                DebugP_log3 ("Debug: Receive  VC %d FIFO Size: %d FIFO Address: %d\n",
                             index, ptrCSIDriver->cfg.virtualChannelCfg[index].rxFIFOSize, rxFIFOAddress[index]);
                DebugP_log3 ("Debug: Transmit VC %d FIFO Size: %d FIFO Address: %d\n",
                             index, ptrCSIDriver->cfg.virtualChannelCfg[index].txFIFOSize, txFIFOAddress[index]);
            }
        }

        /* Configure the CSI2 TX FIFO registers */
        ptrCSIDriver->ptrProtReg->CSI2_TX_FIFO_VC_SIZE =
            CSL_FMKR (31U, 28U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[3].txFIFOSize)  |
            CSL_FMKR (26U, 24U, txFIFOAddress[3])                                             |
            CSL_FMKR (23U, 20U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[2].txFIFOSize)  |
            CSL_FMKR (18U, 16U, txFIFOAddress[2])                                             |
            CSL_FMKR (15U, 12U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[1].txFIFOSize)  |
            CSL_FMKR (10U,  8U, txFIFOAddress[1])                                             |
            CSL_FMKR (7U,   4U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[0].txFIFOSize)  |
            CSL_FMKR (2U,   0U, txFIFOAddress[0]);

        /* Configure the CSI2 RX FIFO registers */
        ptrCSIDriver->ptrProtReg->CSI2_RX_FIFO_VC_SIZE =
            CSL_FMKR (31U, 28U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[3].rxFIFOSize)  |
            CSL_FMKR (26U, 24U, rxFIFOAddress[3])                                             |
            CSL_FMKR (23U, 20U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[2].rxFIFOSize)  |
            CSL_FMKR (18U, 16U, rxFIFOAddress[2])                                             |
            CSL_FMKR (15U, 12U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[1].rxFIFOSize)  |
            CSL_FMKR (10U,  8U, rxFIFOAddress[1])                                             |
            CSL_FMKR (7U,   4U, (uint32_t)ptrCSIDriver->cfg.virtualChannelCfg[0].rxFIFOSize)  |
            CSL_FMKR (2U,   0U, rxFIFOAddress[0]);
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the virtual channels
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *  @param[in]  enableChannel
 *      Flag which indicates if the channel is to be enabled or not
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_configVC (CSI_DriverMCB* ptrCSIDriver, uint32_t enableChannel)
{
    uint32_t    index;

#if 0
    {
        0U, 0U, DMA_RQST_4, DMA_THRSHOLD_1_32_BITS, DMA_RQST_0, DMA_THRSHOLD_16_32_BITS
        (vpSource)0U, (rgb565Order)1U, (modeSpeed)1U, (eccTxEn)1U, (csTxEn)1U, (btaEn)0U, (mode)0U, (btaLongEn)0U,
        (btaShortEn)0U, (source)0U, (vcEn)0U
    }
#endif

    /* Cycle through the virtual channel configuration block: */
    for (index = 0; index < (uint32_t)CSI_VirtualCh_MAX_CHANNEL; index++)
    {
        /* Is the virtual channel active? */
        if (ptrCSIDriver->virtualChannel[index].isActive == 1U)
        {
            /* YES: Configure the virtual channels:-
             * - [Reset Value] DCS Continue command code
             * - [Reset Value] Disable the insertion of the DCS command codes
             * - Dont use the Rx DMA
             * - [Reset Value] Rx DMA Threshold is set to 32 bits
             * - Tx DMA use DMA_REQ0 [XXX: This needs to be modified for different virtual channels]
             * - Tx Threshold is 16 words.
             * - [Reset Value] VP Source is set to Video Port 1
             * - RGB565 Order is set to RGB565_VIDEO
             * - OCP Bus Width is set to 32 bit data width
             * - High Speed mode
             * - Enable Error Correction code (ECC) for transmit [Long & Short Packets]
             * - Enable Checksum generation (CS) for long packet
             * - [Reset Value] Disable Bus Turn around
             * - [Reset Value] Command Mode
             * - [Reset Value] Disable Bus Turn around after long packet transmission
             * - [Reset Value] Disable Bus Turn around after short packet transmission
             * - [Reset Value] Source is the OCP Slave Port
             * - Enable/Disable the Virtual Channel as per the configuration */
            ptrCSIDriver->ptrProtReg->CSI2VCPARAMS[index].CSI2_VC_CTRL = CSL_FMKR (31U, 31U, 0U) |
                                                                         CSL_FMKR (30U, 30U, 0U) |
                                                                         CSL_FMKR (29U, 27U, 4U) |
                                                                         CSL_FMKR (26U, 24U, 0U) |
                                                                         CSL_FMKR (23U, 21U, 0U) |
                                                                         CSL_FMKR (19U, 17U, 4U) |
                                                                         CSL_FMKR (13U, 13U, 0U) |
                                                                         CSL_FMKR (12U, 12U, 1U) |
                                                                         CSL_FMKR (11U, 10U, 3U) |
                                                                         CSL_FMKR (9U,  9U,  1U) |
                                                                         CSL_FMKR (8U,  8U,  1U) |
                                                                         CSL_FMKR (7U,  7U,  1U) |
                                                                         CSL_FMKR (6U,  6U,  0U) |
                                                                         CSL_FMKR (4U,  4U,  0U) |
                                                                         CSL_FMKR (3U,  3U,  0U) |
                                                                         CSL_FMKR (2U,  2U,  0U) |
                                                                         CSL_FMKR (1U,  1U,  0U) |
                                                                         CSL_FMKR (0U,  0U,  enableChannel);
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the clock timings
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_setupClockTimings (CSI_DriverMCB* ptrCSIDriver)
{
    uint8_t ddrClkPre;

    /*************************************************************************
     * DDR_CLK_PRE calculation:
     *************************************************************************/
    ddrClkPre = (uint8_t)((400U * (ptrCSIDriver->hsiClock / (uint32_t)8U ) / (uint32_t)1000U ) + (uint32_t)20U);

    /* Setup the clock timings: This setups the CSI2 Protocol Engine module timers:-
     * - The DDR CLK Pre is dependent on the HSI Clock Rate
     * - The DDR CLK Post is always set to 0x32. */
    ptrCSIDriver->ptrProtReg->CSI2_CLK_TIMING  = CSL_FMKR (15U, 8U, ddrClkPre)    |
                                                CSL_FMKR (7U,  0U, 32U);

    /* Setup the stop clock latency. */
    ptrCSIDriver->ptrProtReg->CSI2_STOPCLK_TIMING = CSL_FMKR (7U, 0U, 0U);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to setup the timing turn around configuration
 *
 *  @param[in]  ptrCSIDriver
 *      Pointer to the CSI Driver
 *  @param[in]  forceTxStopModeIO
 *      Flag which sets the Force Tx Stop mode mode IO bit
 *
 *  \ingroup CSI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void CSI_setupTimingTA (CSI_DriverMCB* ptrCSIDriver, uint32_t forceTxStopModeIO)
{
    /* Configure the CSI2 Module Timers:
     * - [Reset Value] Disable the Turn around timer
     * - [Reset Value] Disable the 16x multiplication factor
     * - [Reset Value] Disable the 8x multiplication factor
     * - Disable the turn around counter
     * - Configue the Tx Stop Mode IO as per the API configuration
     * - Disable the 16x multiplication factor for the Stop State Counter
     * - Disable the 4x multiplication factor for the Stop State Counter
     * - Configure the stop state counter */
    ptrCSIDriver->ptrProtReg->CSI2_TIMING1 = CSL_FMKR (31U, 31U, 0U)                 |
                                            CSL_FMKR (30U, 30U, 0U)                 |
                                            CSL_FMKR (29U, 29U, 0U)                 |
                                            CSL_FMKR (28U, 16U, 0U)                 |
                                            CSL_FMKR (15U, 15U, forceTxStopModeIO)  |
                                            CSL_FMKR (14U, 14U, 0U)                 |
                                            CSL_FMKR (13U, 13U, 0U)                 |
                                            CSL_FMKR (12U,  0U, 200U);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open and setup the CSI driver
 *
 *  @param[in]  ptrCfg
 *      Pointer to the configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CSI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Handle to the CSI Driver
 *  @retval
 *      Error       - NULL
 */
CSI_Handle CSI_open (CSI_Cfg* ptrCfg, int32_t* errCode)
{
    CSI_Handle          retHandle;
    CSI_DriverMCB*      ptrCSIDriver;
    volatile int32_t    statusFlag;
    uint32_t            hsiClock;

    /* Sanity Check: Validate the arguments */
    if ((ptrCfg == NULL) || (CSI_getHSIClock (ptrCfg->dataRate, &hsiClock) < 0))
    {
        *errCode  = CSI_EINVAL;
        retHandle = NULL;
    }
    else
    {
        /* Allocate memory for the CSI Driver */
        ptrCSIDriver = MemoryP_ctrlAlloc (sizeof(CSI_DriverMCB), 0);
        if (ptrCSIDriver == NULL)
        {
            *errCode  = CSI_ENOMEM;
            retHandle = NULL;
        }
        else
        {
            /* Initialize the allocated memory */
            memset ((void*)ptrCSIDriver, 0, sizeof(CSI_DriverMCB));

            /* Copy over the initialization configuration: */
            memcpy ((void *)&ptrCSIDriver->cfg, (void*)ptrCfg, sizeof(CSI_Cfg));

            /* Populate the Driver MCB: */
            ptrCSIDriver->ptrPhyReg  = gCSIPhyRegister;
            ptrCSIDriver->ptrProtReg = gCSIProtocolEngineRegister;
            ptrCSIDriver->hsiClock   = hsiClock;

            /* Enable the CSI: */
            if (SOC_enableCSI (ptrCSIDriver->cfg.socHandle, errCode) == 0)
            {
                /* Reset the CSI: */
                CSI_reset (ptrCSIDriver);

                /* Setup the CSI Clocks: */
                CSI_setupClocks (ptrCSIDriver);

                /* Setup the CSI2 PHY Registers: */
                CSI_setupPhy (ptrCSIDriver, &ptrCSIDriver->cfg.phyParams);

                /* Setup the Complex IO Lane configurations: */
                CSI_setupComplexIO (ptrCSIDriver);

                /*****************************************************************
                 * SYNC: Has the complex IO power status being completed?
                 *****************************************************************/
                statusFlag = 0;
                while (statusFlag == 0)
                {
                    if (CSL_FEXTR (ptrCSIDriver->ptrProtReg->CSI2_COMPLEXIO_CFG1, 26U, 25U) != 0U)
                    {
                        /* Documentation states that the power control is not in the OFF state */
                        statusFlag = 1;
                    }
                }

                /* Setup the CSI2 Control: */
                CSI_setupControl (ptrCSIDriver, 1U);

                /* Setup the CSI2 Control: */
                CSI_setupControl (ptrCSIDriver, 0U);

                /* Configure the virtual channels: */
                if (CSI_setupVCFIFO (ptrCSIDriver) <= 0)
                {
                    /* Error: Invalid or no virtual channels are specified. We cannot proceed */
                    *errCode  = CSI_EINVAL;
                    retHandle = NULL;
                    MemoryP_ctrlFree (ptrCSIDriver, (uint32_t)sizeof(CSI_DriverMCB));
                }
                else
                {
                    /* Setup the OCP configuration: */
                    CSI_setupOCPConfig (ptrCSIDriver);

                    /* Configure the virtual channel control: */
                    CSI_configVC (ptrCSIDriver, 0U);

                    /* Setup the Clock Timings */
                    CSI_setupClockTimings (ptrCSIDriver);

                    /* Setup the Timing Turnaround configuration: */
                    CSI_setupTimingTA (ptrCSIDriver, 0U);

                    /* Setup the Timing Turnaround configuration: Force the Tx Stop Mode IO Bit */
                    CSI_setupTimingTA (ptrCSIDriver, 1U);

                    /* Enable all the virtual channels */
                    CSI_configVC (ptrCSIDriver, 1U);

                    /* Setup the CSI2 Control and enable the interface */
                    CSI_setupControl (ptrCSIDriver, 1U);

                    /*****************************************************************
                     * SYNC: Has the TX STOP Mode IO been completed?
                     *****************************************************************/
                    statusFlag = 0;
                    while (statusFlag == 0)
                    {
                        if (CSL_FEXTR (ptrCSIDriver->ptrProtReg->CSI2_TIMING1, 15U, 15U) == 0U)
                        {
                            /* Documentation states that the Tx Mode IO has been completed. */
                            statusFlag = 1;
                        }
                    }

                    /* Setup the return handle */
                    retHandle = (CSI_Handle)ptrCSIDriver;
                }
            }
            else
            {
                /* Error: Unable to enable the CSI. Report the failure the error code has already been
                 * populated */
                retHandle = NULL;
                MemoryP_ctrlFree (ptrCSIDriver, sizeof(CSI_DriverMCB));
            }
        }
    }
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the CSI Driver
 *
 *  @param[in]  csiHandle
 *      Handle to the CSI driver
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup CSI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t CSI_close (CSI_Handle csiHandle, int32_t* errCode)
{
    CSI_DriverMCB*  ptrCSIDriver;
    int32_t         retVal = 0;

    /* Get the driver MCB: */
    ptrCSIDriver = (CSI_DriverMCB*)csiHandle;
    if (ptrCSIDriver == NULL)
    {
        *errCode = CSI_EINVAL;
        retVal   = -1;
    }
    else
    {
        /* Reset the CSI: */
        CSI_reset (ptrCSIDriver);

        /* Cleanup the memory: */
        MemoryP_ctrlFree (ptrCSIDriver, sizeof(CSI_DriverMCB));
    }
    return retVal;
}

