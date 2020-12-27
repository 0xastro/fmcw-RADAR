/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for CAN
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/arm/v7a/Pmu.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/can/can.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/esm/esm.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/drivers/osal/HwiP.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/** \brief Number of messages sent */
#define DCAN_APP_TEST_INTERNAL_LOOPBACK 1
#define DCAN_APP_TEST_EXTERNAL_LOOPBACK 2
#define DCAN_APP_TEST_PARITY            3
#define DCAN_APP_TEST_EXTERNAL_DATA     4

/** \brief Number of messages sent */
#define DCAN_APP_TEST_MESSAGE_COUNT     100

/** \brief DCAN input clock - 20MHz */
#define DCAN_APP_INPUT_CLK              (20000000U)
/** \brief DCAN output bit rate - 1MHz */
#define DCAN_APP_BIT_RATE               (1000000U)
/** \brief DCAN Propagation Delay - 700ns */
#define DCAN_APP_PROP_DELAY             (700U)
/** \brief DCAN Sampling Point - 70% */
#define DCAN_APP_SAMP_PT                (70U)

/** \brief DCAN TX message object used */
#define DCAN_TX_MSG_OBJ                 (0x1U)
/** \brief DCAN RX message object used */
#define DCAN_RX_MSG_OBJ                 (0x2U)


/** \brief DCAN Message Object used for PARITY Test */
#define DCAN_NUM_MSG_OBJ                 (0x1)

#define DCAN_MSG_OBJ_1                   (0x4)
#define DCAN_MSG_OBJ_2                   (0x6)

/** \brief Message Object Size*/
#define DCAN_MSG_OBJ_SIZE                (0x20U)

/** \brief DCAN Message Object RAM Address */
#if (defined(SOC_XWR14XX))
#define DCAN_MSG_OBJ_RAM_ADDR_1         ((SOC_XWR14XX_MSS_DCAN_MEM_BASE_ADDRESS) + \
                                         (DCAN_MSG_OBJ_1 * DCAN_MSG_OBJ_SIZE))
#define DCAN_MSG_OBJ_RAM_ADDR_2         ((SOC_XWR14XX_MSS_DCAN_MEM_BASE_ADDRESS) + \
                                         (DCAN_MSG_OBJ_2 * DCAN_MSG_OBJ_SIZE))
#elif defined(SOC_XWR16XX)
#define DCAN_MSG_OBJ_RAM_ADDR_1         ((SOC_XWR16XX_MSS_DCAN_MEM_BASE_ADDRESS) + \
                                         (DCAN_MSG_OBJ_1 * DCAN_MSG_OBJ_SIZE))
#define DCAN_MSG_OBJ_RAM_ADDR_2         ((SOC_XWR16XX_MSS_DCAN_MEM_BASE_ADDRESS) + \
                                        (DCAN_MSG_OBJ_2 * DCAN_MSG_OBJ_SIZE))
#elif defined(SOC_XWR18XX)
#define DCAN_MSG_OBJ_RAM_ADDR_1         ((SOC_XWR18XX_MSS_DCAN_MEM_BASE_ADDRESS) + \
                                         (DCAN_MSG_OBJ_1 * DCAN_MSG_OBJ_SIZE))
#define DCAN_MSG_OBJ_RAM_ADDR_2         ((SOC_XWR18XX_MSS_DCAN_MEM_BASE_ADDRESS) + \
                                        (DCAN_MSG_OBJ_2 * DCAN_MSG_OBJ_SIZE))
#endif

/* Global Variables */
volatile uint32_t       testSelection = 0;
volatile uint32_t       gTxDoneFlag = 0, gRxDoneFlag = 0, gParityErrFlag = 0;
uint32_t                iterationCount = 0U;
volatile uint32_t       gTxPkts = 0, gRxPkts = 0, gErrStatusInt = 0;
CAN_DCANCfgParams       appDcanCfgParams;
CAN_DCANMsgObjCfgParams appDcanTxCfgParams;
CAN_DCANMsgObjCfgParams appDcanRxCfgParams;
CAN_DCANBitTimeParams   appDcanBitTimeParams;
CAN_DCANData            appDcanTxData;
CAN_DCANData            appDcanRxData;
uint32_t                dataLength = 0U;
uint32_t                msgLstErrCnt = 0U;
uint32_t                dataMissMatchErrCnt = 0U;
uint32_t                rxTicks[DCAN_APP_TEST_MESSAGE_COUNT];
uint32_t                txTicks[DCAN_APP_TEST_MESSAGE_COUNT];
uint32_t                minRxTicks;
uint32_t                maxRxTicks;
uint32_t                minTxTicks;
uint32_t                maxTxTicks;
uint32_t                totalTxTicks;
uint32_t                totalRxTicks;
uint32_t                gDisplayStats = 0;

/**
 *  @b Description
 *  @n
 *      This function starts the PMU counter.
 *
 *   @param[in] counter
 *      Counter id used for benchmarking
 *
 *  @retval
 *      Not Applicable.
 */
void Test_benchmarkStart(uint32_t counter)
{
    /* Initialize counter to count cycles */
    Pmu_configureCounter(counter, 0x11, FALSE);

    /* Reset PMU counter */
    Pmu_resetCount(counter);

    /* Start PMU counter */
    Pmu_startCounter(counter);
}

/**
 *  @b Description
 *  @n
 *      This function stops a PMU counter and returns the current
 *      counter value.
 *
 *   @param[in] counter
 *      Counter id used for benchmarking
 *
 *  @retval
 *      Current PMU counter value.
 */
uint32_t Test_benchmarkStop(uint32_t counter)
{
    /* Stop PMU counter */
    Pmu_stopCounter(counter);

    /* Read PMU counter */
    return (Pmu_getCount(counter));
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle error and status interrupts.
 *
 *   @param[in] handle
 *      Handle to the CAN Driver
 *   @param[in] errStatusResp
 *      Response structure containing the Error and status information
 *
 *  @retval
 *      Not Applicable.
 */
static void DCANAppErrStatusCallback(CAN_Handle handle, CAN_ErrStatusResp* errStatusResp)
{
    gErrStatusInt++;
    if (errStatusResp->parityError == 1)
    {
        gParityErrFlag = 1;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle Tx complete and receive interrupts.
 *
 *   @param[in] handle
 *      Handle to the message object
 *   @param[in] msgObjectNum
 *      Message object number
 *   @param[in] direction
 *      Direction of the object number
 *
 *  @retval
 *      Not Applicable.
 */
static void DCANAppCallback(CAN_MsgObjHandle handle, uint32_t msgObjectNum, CAN_Direction direction)
{
    int32_t     errCode, retVal;

    if (direction == CAN_Direction_TX)
    {
        if (msgObjectNum != DCAN_TX_MSG_OBJ)
        {
            System_printf ("Error: Tx callback received for incorrect Message Object %d\n", msgObjectNum);
            return;
        }
        else
        {
            gTxPkts++;
            gTxDoneFlag = 1;
            return;
        }
    }
    if (direction == CAN_Direction_RX)
    {
        if (msgObjectNum != DCAN_RX_MSG_OBJ)
        {
            System_printf ("Error: Rx callback received for incorrect Message Object %d\n", msgObjectNum);
            return;
        }
        else
        {
            /* Reset the receive buffer */
            memset(&appDcanRxData, 0, sizeof (appDcanRxData));
            dataLength = 0;

            if ((testSelection != DCAN_APP_TEST_EXTERNAL_DATA) && (gRxPkts < DCAN_APP_TEST_MESSAGE_COUNT))
            {
                /* Reset the counter: */
                Test_benchmarkStart(0);
            }
            retVal = CAN_getData (handle, &appDcanRxData, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CAN receive data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
                return;
            }

            if ((testSelection != DCAN_APP_TEST_EXTERNAL_DATA) && (gRxPkts < DCAN_APP_TEST_MESSAGE_COUNT))
            {
                /* Stop the counter: */
                rxTicks[gRxPkts] = Test_benchmarkStop(0);

                /* Update the receive statistics: */
                minRxTicks   = (minRxTicks < rxTicks[gRxPkts]) ? minRxTicks : rxTicks[gRxPkts];
                maxRxTicks   = (maxRxTicks > rxTicks[gRxPkts]) ? maxRxTicks : rxTicks[gRxPkts];
                totalRxTicks = totalRxTicks + rxTicks[gRxPkts];
            }

            /* Check if sent data is lost or not */
            if (appDcanRxData.msgLostFlag == 1)
            {
                msgLstErrCnt++;
            }

            /* Validate the data */
            if ((testSelection == DCAN_APP_TEST_INTERNAL_LOOPBACK) || (testSelection == DCAN_APP_TEST_EXTERNAL_LOOPBACK))
            {
                /* Check if sent data has been received */
                if (appDcanRxData.dataLength == appDcanTxData.dataLength)
                {
                    while (dataLength < appDcanRxData.dataLength)
                    {
                        if (appDcanRxData.msgData[dataLength] != appDcanTxData.msgData[dataLength])
                        {
                            dataMissMatchErrCnt++;
                            System_printf ("Error: CAN receive data mismatch for iteration %d at byte %d\n", iterationCount, dataLength);
                        }
                        dataLength++;
                    }
                }
            }
            gRxPkts++;
            gRxDoneFlag = 1;
            return;
        }
    }
}

/**
 * \brief   This function takes I/P Clk frequency, required bit-rate, reference
 *          sampling point, propagation delayon the CAN bus and calculates the
 *          value to be programmed for DCAN BTR register.
 *          This API doesn't do the actual programming. This is
 *          intended to be used as a utility function. And the application
 *          should call the #DCANSetBitTime function to do the actual
 *          programming.
 *
 * \param   clkFreq       I/P clock frequency to DCAN controller in terms of MHz
 * \param   bitRate       Required bit-rate on the CAN bus in KHz
 * \param   refSamplePnt  Reference Sampling point in terms of %
 * \param   propDelay     Required Propagation delay in terms of ns
 * \param   bitTimeParams  Pointer to params where the calculated register
 *                        fields are populated
 *
 * \return  Returns the error status information as STW_SOK for success and
 *          STW_EFAIL when no valid baudrate calculation possible for the
 *          configured baudrate and propagation delay
 */
int32_t DCANAppCalcBitTimeParams(uint32_t               clkFreq,
                                uint32_t                bitRate,
                                uint32_t                refSamplePnt,
                                uint32_t                propDelay,
                                CAN_DCANBitTimeParams*  bitTimeParams)
{
    Double  tBitRef = 1000 * 1000 / bitRate;
    Double  newBaud = 0, newNProp = 0, newNSeg = 0, newSjw = 0, newP = 0;
    Double  nQRef, nProp, fCan, nQ, nSeg, baud, sp, p, newSp = 0;
    float   tQ;

    for (p = 1; p <= 1024; p++)
    {
        tQ    = ((p / clkFreq) * 1000.0);
        nQRef = tBitRef / tQ;

        if ((nQRef >= 8) && (nQRef <= 25))
        {
            nProp = ceil(propDelay / tQ);
            fCan  = clkFreq / p;
            nQ    = fCan / bitRate * 1000;
            nSeg  = ceil((nQ - nProp - 1) / 2);

            if ((nProp <= 8) && (nProp > 0) && (nSeg <= 8) && (nSeg > 0))
            {
                baud = fCan / (1 + nProp + 2 * nSeg) * 1000;

                sp = (1 + nProp + nSeg) / (1 + nProp + nSeg + nSeg) * 100;

                if ((abs(baud - bitRate)) < (abs(newBaud - bitRate)))
                {
                    newBaud  = baud;
                    newNProp = nProp;
                    newNSeg  = nSeg;
                    newSjw   = (nSeg < 4) ? nSeg : 4;
                    newP     = p - 1;
                    newSp    = sp;
                }
                else if ((abs(baud - bitRate)) == (abs(newBaud - bitRate)))
                {
                    if ((abs(sp - refSamplePnt)) < (abs(newSp - refSamplePnt)))
                    {
                        newBaud  = baud;
                        newNProp = nProp;
                        newNSeg  = nSeg;
                        newSjw   = (nSeg < 4) ? nSeg : 4;
                        newP     = p - 1;
                        newSp    = sp;
                    }
                }
            }
        }
    }
    if ((newBaud == 0) || (newBaud > 1000))
    {
        return -1;
    }

    bitTimeParams->baudRatePrescaler    = (((uint32_t) newP) & 0x3F);
    bitTimeParams->baudRatePrescalerExt =
        ((((uint32_t) newP) & 0x3C0) ? (((uint32_t) newP) & 0x3C0) >> 6 : 0);
    bitTimeParams->syncJumpWidth = ((uint32_t) newSjw) - 1;

    /* propSeg = newNProp, phaseSeg = newNSeg, samplePoint = newSp
     * nominalBitTime = (1 + newNProp + 2 * newNSeg), nominalBitRate = newBaud
     * brpFreq  = clkFreq / (brp + 1), brpeFreq = clkFreq / (newP + 1)
     * brp      = bitTimeParams->baudRatePrescaler;
     */

    bitTimeParams->timeSegment1 = newNProp + newNSeg - 1;
    bitTimeParams->timeSegment2 = newNSeg - 1;

    return 0;
}

/**
 *  @b Description
 *  @n
 *      Platform specific intializations.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */

static int32_t PlatformInit(void)
{
    int32_t         errCode;
    SOC_Handle      socHandle;
    SOC_Cfg         socCfg;

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

#if (defined(SOC_XWR14XX))
    /* Setup the PINMUX to bring out the XWR14xx CAN pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINP5_PADAE, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINP5_PADAE, SOC_XWR14XX_PINP5_PADAE_CAN_TX);
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINR8_PADAD, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINR8_PADAD, SOC_XWR14XX_PINR8_PADAD_CAN_RX);
#elif (defined (SOC_XWR16XX))
    /* Setup the PINMUX to bring out the XWR16xx CAN pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR16XX_PINC13_PADAG, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR16XX_PINC13_PADAG, SOC_XWR16XX_PINC13_PADAG_CAN_TX);

    Pinmux_Set_OverrideCtrl(SOC_XWR16XX_PINE13_PADAF, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR16XX_PINE13_PADAF, SOC_XWR16XX_PINE13_PADAF_CAN_RX);
#elif (defined (SOC_XWR18XX))
    /* Setup the PINMUX to bring out the XWR18xx CAN pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PINE15_PADAG, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR18XX_PINE15_PADAG, SOC_XWR18XX_PINE15_PADAG_CAN_TX);

    Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PINE13_PADAF, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR18XX_PINE13_PADAF, SOC_XWR18XX_PINE13_PADAF_CAN_RX);
#endif

    /* Configure the divide value for DCAN source clock */
    SOC_setPeripheralClock(socHandle, SOC_MODULE_DCAN, SOC_CLKSOURCE_VCLK, 9U, &errCode);

    /* Initialize peripheral memory */
    SOC_initPeripheralRam(socHandle, SOC_MODULE_DCAN, &errCode);

    return 0;
}

static void DCANAppInitParams(CAN_DCANCfgParams*        dcanCfgParams,
                              CAN_DCANMsgObjCfgParams*  dcanTxCfgParams,
                              CAN_DCANMsgObjCfgParams*  dcanRxCfgParams,
                              CAN_DCANData*             dcanTxData)
{
    /*Intialize DCAN Config Params*/

    if (testSelection == DCAN_APP_TEST_PARITY)
    {
        dcanCfgParams->parityEnable         = 1;
        dcanCfgParams->intrLine0Enable      = 1;
        dcanCfgParams->intrLine1Enable      = 0;
        dcanCfgParams->testModeEnable       = 0;
        dcanCfgParams->eccModeEnable        = 1;
        dcanCfgParams->eccDiagModeEnable    = 0;
        dcanCfgParams->sbeEventEnable       = 1;
        dcanCfgParams->stsChangeIntrEnable  = 0;
    }
    else if (testSelection == DCAN_APP_TEST_EXTERNAL_DATA)
    {
        dcanCfgParams->parityEnable         = 0;
        dcanCfgParams->intrLine0Enable      = 1;
        dcanCfgParams->intrLine1Enable      = 1;
        dcanCfgParams->testModeEnable       = 0;
        dcanCfgParams->eccModeEnable        = 0;
        dcanCfgParams->stsChangeIntrEnable  = 0;
    }
    else
    {
        dcanCfgParams->parityEnable         = 0;
        dcanCfgParams->intrLine0Enable      = 1;
        dcanCfgParams->intrLine1Enable      = 1;
        dcanCfgParams->eccModeEnable        = 0;
        dcanCfgParams->testModeEnable       = 1;
        if (testSelection == DCAN_APP_TEST_INTERNAL_LOOPBACK)
        {
            dcanCfgParams->testMode = CAN_DCANTestMode_LPBACK;
            dcanCfgParams->stsChangeIntrEnable  = 0;
        }
        else
        {
            dcanCfgParams->testMode = CAN_DCANTestMode_EXT_LPBACK;
            dcanCfgParams->stsChangeIntrEnable  = 1;
        }
    }

    dcanCfgParams->autoRetransmitDisable = 1;
    dcanCfgParams->autoBusOnEnable       = 0;
    dcanCfgParams->errIntrEnable         = 1;
    dcanCfgParams->autoBusOnTimerVal     = 0;
    dcanCfgParams->if1DmaEnable          = 0;
    dcanCfgParams->if2DmaEnable          = 0;
    dcanCfgParams->if3DmaEnable          = 0;
    dcanCfgParams->ramAccessEnable       = 0;
    dcanCfgParams->appCallBack           = DCANAppErrStatusCallback;

    /*Intialize DCAN tx Config Params*/
    dcanTxCfgParams->xIdFlagMask       = 0x1;
    dcanTxCfgParams->dirMask           = 0x1;
    dcanTxCfgParams->msgIdentifierMask = 0x1FFFFFFF;

    dcanTxCfgParams->msgValid      = 1;
    dcanTxCfgParams->xIdFlag       = CAN_DCANXidType_11_BIT;
    dcanTxCfgParams->direction     = CAN_Direction_TX;
    dcanTxCfgParams->msgIdentifier = 0xC1;

    dcanTxCfgParams->uMaskUsed    = 1;
    if (testSelection == DCAN_APP_TEST_PARITY)
        dcanTxCfgParams->intEnable    = 0;
    else
        dcanTxCfgParams->intEnable    = 1;

    dcanTxCfgParams->remoteEnable = 0;
    dcanTxCfgParams->fifoEOBFlag  = 1;
    dcanTxCfgParams->appCallBack  = DCANAppCallback;

    /*Intialize DCAN Rx Config Params*/
    dcanRxCfgParams->xIdFlagMask       = 0x1;
    dcanRxCfgParams->msgIdentifierMask = 0x1FFFFFFF;
    dcanRxCfgParams->dirMask           = 0x1;

    dcanRxCfgParams->msgValid      = 1;
    dcanRxCfgParams->xIdFlag       = CAN_DCANXidType_11_BIT;
    dcanRxCfgParams->direction     = CAN_Direction_RX;
    dcanRxCfgParams->msgIdentifier = 0xC1;

    dcanRxCfgParams->uMaskUsed    = 1;
    if (testSelection == DCAN_APP_TEST_PARITY)
        dcanRxCfgParams->intEnable    = 0;
    else
        dcanRxCfgParams->intEnable    = 1;

    dcanRxCfgParams->remoteEnable = 0;
    dcanRxCfgParams->fifoEOBFlag  = 1;
    dcanRxCfgParams->appCallBack  = DCANAppCallback;

    /*Intialize DCAN Tx transfer Params*/
    dcanTxData->dataLength = DCAN_MAX_MSG_LENGTH;
    dcanTxData->msgData[0] = 0xA5;
    dcanTxData->msgData[1] = 0x5A;
    dcanTxData->msgData[2] = 0xFF;
    dcanTxData->msgData[3] = 0xFF;
    dcanTxData->msgData[4] = 0xC3;
    dcanTxData->msgData[5] = 0x3C;
    dcanTxData->msgData[6] = 0xB4;
    dcanTxData->msgData[7] = 0x4B;
}

static int32_t dcanParityTest(void)
{
    CAN_Handle              canHandle;
    CAN_MsgObjHandle        txMsgObjHandle;
    CAN_MsgObjHandle        rxMsgObjHandle;
    int32_t                 retVal = 0;
    int32_t                 errCode = 0;
    CAN_OptionTLV           optionTLV;
    uint8_t                 value;
    CAN_DCANEccErrSts       eccErrSts;
    int32_t*                ptrMsgObjMem;

    gTxDoneFlag = 0;

    /* Initialize the DCAN parameters that need to be specified by the application */
    DCANAppInitParams(&appDcanCfgParams,
                      &appDcanTxCfgParams,
                      &appDcanRxCfgParams,
                      &appDcanTxData);

    /* Initialize the CAN driver */
    canHandle = CAN_init(&appDcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CAN Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Set the desired bit rate based on input clock */
    retVal = DCANAppCalcBitTimeParams(DCAN_APP_INPUT_CLK / 1000000,
                                            DCAN_APP_BIT_RATE / 1000,
                                            DCAN_APP_SAMP_PT,
                                            DCAN_APP_PROP_DELAY,
                                            &appDcanBitTimeParams);
    if (retVal < 0)
    {
        System_printf ("Error: CAN Module bit time parameters are incorrect \n");
        return -1;
    }

    /* Configure the CAN driver */
    retVal = CAN_configBitTime (canHandle, &appDcanBitTimeParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjHandle = CAN_createMsgObject (canHandle, DCAN_MSG_OBJ_1, &appDcanTxCfgParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        System_printf ("Error: CAN create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjHandle = CAN_createMsgObject (canHandle, DCAN_MSG_OBJ_2, &appDcanRxCfgParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        System_printf ("Error: CAN create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Send data over Tx message object */
    retVal = CAN_transmitData (txMsgObjHandle, &appDcanTxData, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
        return -1;
    }

    /* Disable parity to corrupt the RAM */
    optionTLV.type = CAN_Option_DCAN_PARITY;
    optionTLV.length = sizeof(uint8_t);
    value = 0;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option disable PARITY failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Enable ECC Diagnostics */
    optionTLV.type = CAN_Option_DCAN_ECC_DIAG;
    optionTLV.length = sizeof(uint8_t);
    value = 1;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option enable ECC DIAG failed [Error code %d]\n", errCode);
        return -1;
    }

    ptrMsgObjMem = (int32_t *) (DCAN_MSG_OBJ_RAM_ADDR_1);
    /* Corrupting the data in Message RAM in RDA Mode :
     * as 0xC343B44A instead of 0xC343B44B which introduces
     * single bit error for ECC test / Parity Error for Parity test.
     */
    *ptrMsgObjMem = (*ptrMsgObjMem & 0xFFFFFFFE);

    ptrMsgObjMem = (int32_t *) (DCAN_MSG_OBJ_RAM_ADDR_2);

    /* Corrupting the data in Message RAM in RDA Mode :
     * as 0xC343B448 instead of 0xC343B44B which introduces single bit error.
     */
    *ptrMsgObjMem = (*ptrMsgObjMem & 0xFFFFFFFC);

    /* Enable parity to corrupt the RAM */
    optionTLV.type = CAN_Option_DCAN_PARITY;
    optionTLV.length = sizeof(uint8_t);
    value = 1;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option enable PARITY failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Reset the receive buffer */
    memset(&appDcanRxData, 0, sizeof (appDcanRxData));
    dataLength = 0;
    retVal = CAN_getData (txMsgObjHandle, &appDcanRxData, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN receive data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
        return -1;
    }

    /* Wait for the parity error interrupt */
    while (gParityErrFlag == 0);

    /* Get the ECC error status */
    optionTLV.type = CAN_Option_DCAN_ECC_ERROR_STATUS;
    optionTLV.length = sizeof(CAN_DCANEccErrSts);
    optionTLV.value = (void*) &eccErrSts;

    retVal =  CAN_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN get option ECC status failed [Error code %d]\n", errCode);
        return -1;
    }

    if ((eccErrSts.singleBitErr != 1) || (eccErrSts.doubleBitErr != 1) ||
            (eccErrSts.messageNum != DCAN_MSG_OBJ_1))
    {
        System_printf ("Error: ECC error status check failed. Single bit error %d Double bit error %d Message Number %d\n",
                        eccErrSts.singleBitErr, eccErrSts.doubleBitErr, eccErrSts.messageNum);
        return -1;
    }
    else
    {
        System_printf ("Debug: ECC error status check passed. Single bit error %d Double bit error %d Message Number %d\n",
                        eccErrSts.singleBitErr, eccErrSts.doubleBitErr, eccErrSts.messageNum);
    }

    /* Clear the ECC error status */
    optionTLV.type = CAN_Option_DCAN_CLEAR_ECC_ERROR_STATUS;
    optionTLV.length = sizeof(uint8_t);
    value = 1;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option Clear the ECC error status failed [Error code %d]\n", errCode);
        return -1;
    }


    /* Get the ECC Diagnostics error status */
    optionTLV.type = CAN_Option_DCAN_ECC_DIAG_ERROR_STATUS;
    optionTLV.length = sizeof(CAN_DCANEccErrSts);
    optionTLV.value = (void*) &eccErrSts;

    retVal =  CAN_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN get option ECC Diagnostics status failed [Error code %d]\n", errCode);
        return -1;
    }

    if ((eccErrSts.singleBitErr != 1) || (eccErrSts.doubleBitErr != 1) ||
            (eccErrSts.messageNum != DCAN_MSG_OBJ_1))
    {
        System_printf ("Error: ECC Diag error status check failed Single bit error %d Double bit error %d\n",
                        eccErrSts.singleBitErr, eccErrSts.doubleBitErr);
        return -1;
    }
    else
    {
        System_printf ("Debug: ECC Diag error status check passed Single bit error %d Double bit error %d\n",
                        eccErrSts.singleBitErr, eccErrSts.doubleBitErr);
    }

    /* Clear the ECC Diagnostics error status */
    optionTLV.type = CAN_Option_DCAN_CLEAR_ECC_DIAG_ERROR_STATUS;
    optionTLV.length = sizeof(uint8_t);
    value = 1;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option Clear the ECC Diagnostics error status failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Disable ECC Diagnostics */
    optionTLV.type = CAN_Option_DCAN_ECC_DIAG;
    optionTLV.length = sizeof(uint8_t);
    value = 0;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option disable ECC DIAG failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CAN_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    return 0;
}

static int32_t dcanLoopbackTest()
{
    CAN_Handle              canHandle;
    CAN_MsgObjHandle        txMsgObjHandle;
    CAN_MsgObjHandle        rxMsgObjHandle;
    int32_t                 retVal = 0;
    int32_t                 errCode = 0;
    CAN_OptionTLV           optionTLV;
    uint8_t                 value;
    CAN_DCANMsgObjectStats  msgObjStats;

    gTxDoneFlag = 0;

    /* Initialize the DCAN parameters that need to be specified by the application */
    DCANAppInitParams(&appDcanCfgParams,
                      &appDcanTxCfgParams,
                      &appDcanRxCfgParams,
                      &appDcanTxData);

    /* Initialize the CAN driver */
    canHandle = CAN_init(&appDcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CAN Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Set the desired bit rate based on input clock */
    retVal = DCANAppCalcBitTimeParams(DCAN_APP_INPUT_CLK / 1000000,
                                            DCAN_APP_BIT_RATE / 1000,
                                            DCAN_APP_SAMP_PT,
                                            DCAN_APP_PROP_DELAY,
                                            &appDcanBitTimeParams);
    if (retVal < 0)
    {
        System_printf ("Error: CAN Module bit time parameters are incorrect \n");
        return -1;
    }

    /* Configure the CAN driver */
    retVal = CAN_configBitTime (canHandle, &appDcanBitTimeParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjHandle = CAN_createMsgObject (canHandle, DCAN_TX_MSG_OBJ, &appDcanTxCfgParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        System_printf ("Error: CAN create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjHandle = CAN_createMsgObject (canHandle, DCAN_RX_MSG_OBJ, &appDcanRxCfgParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        System_printf ("Error: CAN create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the measurement counters */
    minRxTicks   = 0xFFFFFFFFU;
    maxRxTicks   = 0U;
    minTxTicks   = 0xFFFFFFFFU;
    maxTxTicks   = 0U;
    totalTxTicks = 0U;
    totalRxTicks = 0U;

    while (iterationCount != DCAN_APP_TEST_MESSAGE_COUNT)
    {
        /* Reset the counter: */
        Test_benchmarkStart(0);

        /* Send data over Tx message object */
        retVal = CAN_transmitData (txMsgObjHandle, &appDcanTxData, &errCode);
        if (retVal < 0)
        {
            System_printf ("Error: CAN transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
            return -1;
        }

        /* Stop the counter: */
        txTicks[iterationCount] = Test_benchmarkStop(0);

        /* Update the transmit statistics: */
        minTxTicks   = (minTxTicks < txTicks[iterationCount]) ? minTxTicks : txTicks[iterationCount];
        maxTxTicks   = (maxTxTicks > txTicks[iterationCount]) ? maxTxTicks : txTicks[iterationCount];
        totalTxTicks = totalTxTicks + txTicks[iterationCount];

        while (gTxDoneFlag == 0);
        gTxDoneFlag = 0;

        while (gRxDoneFlag == 0);
        gRxDoneFlag = 0;

        iterationCount++;
    }

    optionTLV.type = CAN_Option_DCAN_POWER_DOWN;
    optionTLV.length = sizeof(uint8_t);
    value = 1;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option SLEEP failed [Error code %d]\n", errCode);
        return -1;
    }

    optionTLV.type = CAN_Option_DCAN_POWER_DOWN;
    optionTLV.length = sizeof(uint8_t);
    value = 0;
    optionTLV.value = (void*) &value;

    retVal =  CAN_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN set option WAKEUP failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Number of iterations              : %d\n", iterationCount);
    System_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
    System_printf("Debug: Number of messages received       : %d\n", gRxPkts);
    System_printf("Debug: Number of messages lost           : %d\n", msgLstErrCnt);
    System_printf("Debug: Number of data mismatch           : %d\n", dataMissMatchErrCnt);
    System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    System_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CAN_Option_DCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CAN_DCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CAN_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message object number             : %d\n", msgObjStats.msgObjectNum);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    System_printf("\n\n");

    msgObjStats.handle = rxMsgObjHandle;
    retVal =  CAN_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message object number             : %d\n", msgObjStats.msgObjectNum);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);

    System_printf("Debug: Receive & Transmit Measurements\n");
    System_printf("Debug: Rx Min:%d Max: %d Average:%d ticks\n", minRxTicks, maxRxTicks, totalRxTicks/gTxPkts);
    System_printf("Debug: Tx Min:%d Max: %d Average:%d ticks\n", minTxTicks, maxTxTicks, totalTxTicks/gRxPkts);

    System_printf("\n\n");

    retVal = CAN_deleteMsgObject(txMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN delete Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CAN_deleteMsgObject(rxMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN delete Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CAN_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    if ((msgLstErrCnt == 0) && (dataMissMatchErrCnt == 0))
    {
        if (testSelection == DCAN_APP_TEST_INTERNAL_LOOPBACK)
        {
            System_printf("Debug: Internal loopback testing for %d iterations Passed\n", iterationCount);
        }
        else
        {
            System_printf("Debug: External loopback testing for %d iterations Passed\n", iterationCount);
        }
    }
    else
    {
        if (testSelection == DCAN_APP_TEST_INTERNAL_LOOPBACK)
        {
            System_printf("Debug: Internal loopback testing for %d iterations Failed\n", iterationCount);
            retVal = -1;
        }
        else
        {
            System_printf("Debug: External loopback testing for %d iterations Failed\n", iterationCount);
            retVal = -1;
        }
    }
    return retVal;
}

static int32_t dcanTransmitTest()
{
    CAN_Handle              canHandle;
    CAN_MsgObjHandle        txMsgObjHandle;
    CAN_MsgObjHandle        rxMsgObjHandle;
    int32_t                 retVal = 0;
    int32_t                 errCode = 0;
    CAN_DCANMsgObjectStats  msgObjStats;
    CAN_OptionTLV           optionTLV;
    CAN_DCANErrorCounter    errCounter;

    /* Initialize the DCAN parameters that need to be specified by the application */
    DCANAppInitParams(&appDcanCfgParams,
                      &appDcanTxCfgParams,
                      &appDcanRxCfgParams,
                      &appDcanTxData);

    /* Initialize the CAN driver */
    canHandle = CAN_init(&appDcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CAN Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Set the desired bit rate based on input clock */
    retVal = DCANAppCalcBitTimeParams(DCAN_APP_INPUT_CLK / 1000000,
                                            DCAN_APP_BIT_RATE / 1000,
                                            DCAN_APP_SAMP_PT,
                                            DCAN_APP_PROP_DELAY,
                                            &appDcanBitTimeParams);
    if (retVal < 0)
    {
        System_printf ("Error: CAN Module bit time parameters are incorrect \n");
        return -1;
    }

    /* Configure the CAN driver */
    retVal = CAN_configBitTime (canHandle, &appDcanBitTimeParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CAN Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjHandle = CAN_createMsgObject (canHandle, DCAN_TX_MSG_OBJ, &appDcanTxCfgParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        System_printf ("Error: CAN create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjHandle = CAN_createMsgObject (canHandle, DCAN_RX_MSG_OBJ, &appDcanRxCfgParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        System_printf ("Error: CAN create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    while (1)
    {
        /* Send data over Tx message object */
        retVal = CAN_transmitData (txMsgObjHandle, &appDcanTxData, &errCode);
        if (retVal < 0)
        {
            System_printf ("Error: CAN transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
            return -1;
        }

        iterationCount++;

        if (gDisplayStats == 1)
        {
            gDisplayStats = 0;
            if (((msgLstErrCnt == 0) &&
                 (dataMissMatchErrCnt == 0)) && (retVal == 0))
            {
                System_printf("Debug: External transmit testing Passed\n");
            }
            else
            {
                System_printf("Debug: External transmit testing failed\n");
                retVal = -1;
            }

            System_printf("Debug: Number of iterations              : %d\n", iterationCount);
            System_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
            System_printf("Debug: Number of messages received       : %d\n", gRxPkts);
            System_printf("Debug: Number of messages lost           : %d\n", msgLstErrCnt);
            System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
            System_printf("\n\n");

            msgObjStats.handle = txMsgObjHandle;
            optionTLV.type = CAN_Option_DCAN_MSG_OBJECT_STATS;
            optionTLV.length = sizeof(CAN_DCANMsgObjectStats);
            optionTLV.value = (void*) &msgObjStats;

            retVal =  CAN_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CAN get stats failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: Message object number             : %d\n", msgObjStats.msgObjectNum);
            System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            System_printf("\n\n");

            msgObjStats.handle = rxMsgObjHandle;
            retVal =  CAN_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CAN get stats failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: Message object number             : %d\n", msgObjStats.msgObjectNum);
            System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            System_printf("\n\n");

            optionTLV.type = CAN_Option_DCAN_ERROR_COUNTER;
            optionTLV.length = sizeof(CAN_DCANErrorCounter);
            optionTLV.value = (void*) &errCounter;

            retVal =  CAN_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CAN get error counter failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: Receive passive error             : %d\n", errCounter.rxErrPassive);
            System_printf("Debug: Transmit Error Counter            : %d\n", errCounter.txErrCounter);
            System_printf("Debug: Receive Error Counter             : %d\n", errCounter.rxErrCounter);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    int32_t         retVal = 0;

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize the platform */
    retVal = PlatformInit();
    if (retVal < 0)
    {
        MCPI_setFeatureTestResult("Platform initialization failed", MCPI_TestResult_FAIL);
        MCPI_setTestResult ();
        BIOS_exit(0);
    }

    while (1)
    {
        System_printf ("*******************************************************\n");
        System_printf ("CAN Unit Test Menu                          \n");
        System_printf ("Please select the type of test to execute:  \n");
        System_printf ("1. DCAN Internal loopback test              \n");
        System_printf ("2. DCAN External loopback test              \n");
        System_printf ("3. DCAN Parity test                         \n");
        System_printf ("4. DCAN Tx/Rx test                          \n");
        System_printf ("*******************************************************\n");
        System_printf ("> Enter your selection: ");

        while (testSelection == 0);

        /* Validate the selection: */
        if ((testSelection >= DCAN_APP_TEST_INTERNAL_LOOPBACK) && (testSelection <= DCAN_APP_TEST_EXTERNAL_DATA))
            break;
    }

    if (testSelection == DCAN_APP_TEST_INTERNAL_LOOPBACK)
    {
        System_printf("Debug: Internal loopback testing\n");
        retVal = dcanLoopbackTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Internal loopback testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Internal loopback testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == DCAN_APP_TEST_EXTERNAL_LOOPBACK)
    {
        System_printf("Debug: External loopback testing\n");
        retVal = dcanLoopbackTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("External loopback testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("External loopback testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == DCAN_APP_TEST_PARITY)
    {
        System_printf("Debug: parity testing\n");
        retVal = dcanParityTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Parity testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Parity testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == DCAN_APP_TEST_EXTERNAL_DATA)
    {
        System_printf("Debug: External transmit testing\n");
        retVal = dcanTransmitTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("External transmit testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("External transmit testing", MCPI_TestResult_PASS);
    }
    if (retVal < 0)
    {
        System_printf("Debug:CAN testing failed\n");
    }

    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
 *
 *  @retval
 *      Not Applicable.
 */
int32_t main (void)
{
	Task_Params     taskParams;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);


    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
    return 0;
}

