/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for CANFD
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
#include <ti/drivers/canfd/canfd.h>
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

/** \brief Test type defines */
#define MCAN_APP_TEST_INTERNAL_LOOPBACK     1U
#define MCAN_APP_TEST_EXTERNAL_LOOPBACK     2U
#define MCAN_APP_TEST_MULTIPLE_TRANSMISSION 3U
#define MCAN_APP_TEST_EXTERNAL_DATA         4U
#define MCAN_APP_TEST_EVM_EVM               5U
#define MCAN_APP_TEST_TX_CANCEL             6U
#define MCAN_APP_TEST_POWERDOWN             7U
#define MCAN_APP_TEST_MSGID_RANGE           8U

#define MCAN_APP_TEST_EVM_MODE_TX           1U
#define MCAN_APP_TEST_EVM_MODE_RX           2U
#define MCAN_APP_TEST_EVM_MODE_FIFO         3U

/** \brief Number of messages sent */
#define MCAN_APP_TEST_MESSAGE_COUNT         100U

/** \brief Number of messages sent */
#define MCAN_APP_TEST_DATA_SIZE             64U

/** \brief VBUSP Frequency in MHz */
#define VBUSP_FREQ                          (MSS_SYS_VCLK /1000000U)

/* Global Variables */
CANFD_Reason            gErrorReason;
volatile uint32_t       testSelection = 0;
volatile uint32_t       gTxDoneFlag = 0, gRxDoneFlag = 0;
volatile uint32_t       iterationCount = 0U;
volatile uint32_t       gTxPkts = 0, gRxPkts = 0, gTxCancelPkts = 0, gErrStatusInt = 0;
uint8_t                 rxData[MCAN_APP_TEST_DATA_SIZE];
uint32_t                txDataLength, rxDataLength, testDataLength = 0U;
uint32_t                gErrMsgLstCnt = 0U;
uint32_t                gErrDataMissMatchCnt = 0U;
uint32_t                gErrGetData = 0U;
uint32_t                gErrFrameType = 0U;
uint32_t                rxTicks[MCAN_APP_TEST_MESSAGE_COUNT];
uint32_t                txTicks[MCAN_APP_TEST_MESSAGE_COUNT];
uint32_t                minRxTicks;
uint32_t                maxRxTicks;
uint32_t                minTxTicks;
uint32_t                maxTxTicks;
uint32_t                totalTxTicks;
uint32_t                totalRxTicks;
uint8_t                 txData[128U] =
{0xA1, 0x1A, 0xFF, 0xFF, 0xC1, 0x1C, 0xB1, 0x1B,
0xA2, 0x2A, 0xFF, 0xFF, 0xC2, 0x2C, 0xB2, 0x2B,
0xA3, 0x3A, 0xFF, 0xFF, 0xC3, 0x3C, 0xB3, 0x3B,
0xA4, 0x4A, 0xFF, 0xFF, 0xC4, 0x4C, 0xB4, 0x4B,
0xA5, 0x5A, 0xFF, 0xFF, 0xC5, 0x5C, 0xB5, 0x5B,
0xA6, 0x6A, 0xFF, 0xFF, 0xC6, 0x6C, 0xB6, 0x6B,
0xA7, 0x7A, 0xFF, 0xFF, 0xC7, 0x7C, 0xB7, 0x7B,
0xA8, 0x8A, 0xFF, 0xFF, 0xC8, 0x8C, 0xB8, 0x8B,
0xA1, 0x1A, 0xFF, 0xFF, 0xC1, 0x1C, 0xB1, 0x1B,
0xA2, 0x2A, 0xFF, 0xFF, 0xC2, 0x2C, 0xB2, 0x2B,
0xA3, 0x3A, 0xFF, 0xFF, 0xC3, 0x3C, 0xB3, 0x3B,
0xA4, 0x4A, 0xFF, 0xFF, 0xC4, 0x4C, 0xB4, 0x4B,
0xA5, 0x5A, 0xFF, 0xFF, 0xC5, 0x5C, 0xB5, 0x5B,
0xA6, 0x6A, 0xFF, 0xFF, 0xC6, 0x6C, 0xB6, 0x6B,
0xA7, 0x7A, 0xFF, 0xFF, 0xC7, 0x7C, 0xB7, 0x7B,
0xA8, 0x8A, 0xFF, 0xFF, 0xC8, 0x8C, 0xB8, 0x8B
};

uint32_t                gDisplayStats = 0;
uint32_t                gMCANMode = 1U;
CANFD_MCANFrameType     testFrameType = CANFD_MCANFrameType_FD;
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
 *      Handle to the CANFD Driver
 *  @param[in]  reason
 *      Cause of the interrupt which prompted the callback.
 *  @param[in]  errStatusResp
 *      Response structure populated with the value of the fields that caused the error or status interrupt.
 *      Processing of this structure is dependent on the callback reason.
 *
 *  @retval
 *      Not Applicable.
 */
static void MCANAppErrStatusCallback(CANFD_Handle handle, CANFD_Reason reason, CANFD_ErrStatusResp* errStatusResp)
{
    gErrStatusInt++;
    gErrorReason = reason;
    return;
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function to handle Tx complete and receive interrupts.
 *
 *   @param[in] handle
 *      Handle to the message object
 *   @param[in] reason
 *      Cause of the interrupt which prompted the callback.
 *
 *  @retval
 *      Not Applicable.
 */
static void MCANAppCallback(CANFD_MsgObjHandle handle, CANFD_Reason reason)
{
    int32_t                 errCode, retVal;
    uint32_t                id;
    CANFD_MCANFrameType     rxFrameType;
    CANFD_MCANXidType       rxIdType;

    if (reason == CANFD_Reason_TX_COMPLETION)
    {
        {
            gTxPkts++;
            gTxDoneFlag = 1;
            return;
        }
    }
    if (reason == CANFD_Reason_RX)
    {
        {
            /* Reset the receive buffer */
            memset(&rxData, 0, sizeof (rxData));
            testDataLength = 0;

            if ((testSelection != MCAN_APP_TEST_EXTERNAL_DATA) && (gRxPkts < MCAN_APP_TEST_MESSAGE_COUNT))
            {
                /* Reset the counter: */
                Test_benchmarkStart(0);
            }
            retVal = CANFD_getData (handle, &id, &rxFrameType, &rxIdType, &rxDataLength, &rxData[0], &errCode);
            if (retVal < 0)
            {
                gErrGetData++;
                return;
            }

            if (rxFrameType != testFrameType)
            {
                gErrFrameType++;
                return;
            }

            if ((testSelection != MCAN_APP_TEST_EXTERNAL_DATA) && (gRxPkts < MCAN_APP_TEST_MESSAGE_COUNT))
            {
                /* Stop the counter: */
                rxTicks[gRxPkts] = Test_benchmarkStop(0);

                /* Update the receive statistics: */
                minRxTicks   = (minRxTicks < rxTicks[gRxPkts]) ? minRxTicks : rxTicks[gRxPkts];
                maxRxTicks   = (maxRxTicks > rxTicks[gRxPkts]) ? maxRxTicks : rxTicks[gRxPkts];
                totalRxTicks = totalRxTicks + rxTicks[gRxPkts];
            }

            /* Validate the data */
            if ((testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK) || (testSelection == MCAN_APP_TEST_EXTERNAL_LOOPBACK) ||
                            (testSelection == MCAN_APP_TEST_EVM_EVM))
            {
                /* Check if sent data has been received */
                if (rxDataLength == txDataLength)
                {
                    while (testDataLength < rxDataLength)
                    {
                        if (rxData[testDataLength] != txData[testDataLength])
                        {
                            gErrDataMissMatchCnt++;
                        }
                        testDataLength++;
                    }
                }

            }
            gRxPkts++;
            gRxDoneFlag = 1U;
            return;
        }
    }
    if (reason == CANFD_Reason_TX_CANCELED)
    {
        {
            gTxCancelPkts++;
            gTxDoneFlag = 1U;
            return;
        }
    }
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
    /* Setup the PINMUX to bring out the XWR16xx CAN pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR16XX_PINE14_PADAE, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR16XX_PINE14_PADAE, SOC_XWR16XX_PINE14_PADAE_CANFD_TX);

    Pinmux_Set_OverrideCtrl(SOC_XWR16XX_PIND13_PADAD, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR16XX_PIND13_PADAD, SOC_XWR16XX_PIND13_PADAD_CANFD_RX);

    return 0;
}

static void MCANAppInitParams(CANFD_MCANInitParams* mcanCfgParams)
{
    /*Intialize MCAN Config Params*/
    memset (mcanCfgParams, sizeof (CANFD_MCANInitParams), 0);

    mcanCfgParams->fdMode          = 0x1U;
    mcanCfgParams->brsEnable       = 0x1U;
    mcanCfgParams->txpEnable       = 0x0U;
    mcanCfgParams->efbi            = 0x0U;
    mcanCfgParams->pxhddisable     = 0x0U;
    mcanCfgParams->darEnable       = 0x1U;
    mcanCfgParams->wkupReqEnable   = 0x1U;
    mcanCfgParams->autoWkupEnable  = 0x1U;
    mcanCfgParams->emulationEnable = 0x0U;
    mcanCfgParams->emulationFAck   = 0x0U;
    mcanCfgParams->clkStopFAck     = 0x0U;
    mcanCfgParams->wdcPreload      = 0x0U;
    mcanCfgParams->tdcEnable       = 0x1U;
    mcanCfgParams->tdcConfig.tdcf  = 0U;
    mcanCfgParams->tdcConfig.tdco  = 8U;
    mcanCfgParams->monEnable       = 0x0U;
    mcanCfgParams->asmEnable       = 0x0U;
    mcanCfgParams->tsPrescalar     = 0x0U;
    mcanCfgParams->tsSelect        = 0x0U;
    mcanCfgParams->timeoutSelect   = CANFD_MCANTimeOutSelect_CONT;
    mcanCfgParams->timeoutPreload  = 0x0U;
    mcanCfgParams->timeoutCntEnable= 0x0U;
    mcanCfgParams->filterConfig.rrfe        = 0x1U;
    mcanCfgParams->filterConfig.rrfs        = 0x1U;
    mcanCfgParams->filterConfig.anfe        = 0x1U;
    mcanCfgParams->filterConfig.anfs        = 0x1U;
    mcanCfgParams->msgRAMConfig.lss         = 127U;
    mcanCfgParams->msgRAMConfig.lse         = 64U;
    mcanCfgParams->msgRAMConfig.txBufNum    = 32U;
    mcanCfgParams->msgRAMConfig.txFIFOSize  = 0U;
    mcanCfgParams->msgRAMConfig.txBufMode   = 0U;
    mcanCfgParams->msgRAMConfig.txEventFIFOSize         = 0U;
    mcanCfgParams->msgRAMConfig.txEventFIFOWaterMark    = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO0size             = 64U;
    mcanCfgParams->msgRAMConfig.rxFIFO0OpMode           = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO0waterMark        = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO1size             = 64U;
    mcanCfgParams->msgRAMConfig.rxFIFO1waterMark        = 0U;
    mcanCfgParams->msgRAMConfig.rxFIFO1OpMode           = 0U;

    mcanCfgParams->eccConfig.enable         = 1U;
    mcanCfgParams->eccConfig.enableChk      = 1U;
    mcanCfgParams->eccConfig.enableRdModWr  = 1U;

    mcanCfgParams->errInterruptEnable   = 1U;
    mcanCfgParams->dataInterruptEnable  = 1U;
    mcanCfgParams->appErrCallBack       = MCANAppErrStatusCallback;
    mcanCfgParams->appDataCallBack      = MCANAppCallback;
}

static int32_t mcanLoopbackTest()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    CANFD_MsgObjHandle          rxMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    uint8_t                     value;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams   rxMsgObjectParams;
    CANFD_MCANLoopbackCfgParams mcanloopbackParams;
    CANFD_MCANMsgObjectStats    msgObjStats;

    gTxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(&mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (40)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x1U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    mcanloopbackParams.enable = 1U;
    if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
        mcanloopbackParams.mode = CANFD_MCANLoopBackMode_INTERNAL;
    else
        mcanloopbackParams.mode = CANFD_MCANLoopBackMode_EXTERNAL;

    optionTLV.type = CANFD_Option_MCAN_LOOPBACK;
    optionTLV.length = sizeof(CANFD_MCANLoopbackCfgParams);
    optionTLV.value = (void*) &mcanloopbackParams;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Loopback failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    txMsgObjectParams.msgIdentifier = 0x29E;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        System_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    rxMsgObjectParams.msgIdentifier = 0x29E;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        System_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the measurement counters */
    minRxTicks   = 0xFFFFFFFFU;
    maxRxTicks   = 0U;
    minTxTicks   = 0xFFFFFFFFU;
    maxTxTicks   = 0U;
    totalTxTicks = 0U;
    totalRxTicks = 0U;

    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        /* Reset the counter: */
        Test_benchmarkStart(0);

        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[0], &errCode);

            if (retVal < 0)
            {
                System_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
                return -1;
            }
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

    System_printf("Debug: Number of iterations              : %d\n", iterationCount);
    System_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
    System_printf("Debug: Number of messages received       : %d\n", gRxPkts);
    System_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    System_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    System_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    System_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    System_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    System_printf("\n\n");

    msgObjStats.handle = rxMsgObjHandle;
    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);

    System_printf("Debug: Receive & Transmit Measurements\n");
    System_printf("Debug: Rx Min:%d Max: %d Average:%d ticks\n", minRxTicks, maxRxTicks, totalRxTicks/gRxPkts);
    System_printf("Debug: Tx Min:%d Max: %d Average:%d ticks\n", minTxTicks, maxTxTicks, totalTxTicks/gTxPkts);

    System_printf("\n\n");

    /* Reset the driver */
    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 1U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Mode - SW INIT failed [Error code %d]\n", errCode);
        return -1;
    }

    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 0U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Mode - NORMAL failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(txMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(rxMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD delete Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    if ((gErrMsgLstCnt == 0) && (gErrDataMissMatchCnt == 0) && (gErrFrameType == 0) && (gErrGetData == 0))
    {
        if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
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
        if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
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


static int32_t mcanTestTxCancel()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    uint8_t                     value;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANLoopbackCfgParams mcanloopbackParams;
    CANFD_MCANMsgObjectStats    msgObjStats;

    gTxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(&mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (40)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x1U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    mcanloopbackParams.enable = 1U;
    mcanloopbackParams.mode = CANFD_MCANLoopBackMode_EXTERNAL;

    optionTLV.type = CANFD_Option_MCAN_LOOPBACK;
    optionTLV.length = sizeof(CANFD_MCANLoopbackCfgParams);
    optionTLV.value = (void*) &mcanloopbackParams;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Loopback failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    txMsgObjectParams.msgIdentifier = 0x29E;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        System_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        /* Send data over Tx message object */
        if (testFrameType == CANFD_MCANFrameType_FD)
            retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
        else
            retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[0], &errCode);

        if (retVal < 0)
        {
            System_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
            return -1;
        }

        /* Cancel data over Tx message object */
        retVal = CANFD_transmitDataCancel (txMsgObjHandle, &errCode);
        if (retVal < 0)
        {
            System_printf ("Error: CANFD transmit data cancel for iteration %d failed [Error code %d]\n", iterationCount, errCode);
            return -1;
        }

        while (gTxDoneFlag == 0);
        gTxDoneFlag = 0;

        iterationCount++;
    }

    System_printf("Debug: Number of iterations              : %d\n", iterationCount);
    System_printf("Debug: Number of messages Cancelled      : %d\n", gTxCancelPkts);
    System_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    System_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    System_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    System_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    System_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    System_printf("\n\n");

    /* Reset the driver */
    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 1U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Mode - SW INIT failed [Error code %d]\n", errCode);
        return -1;
    }

    optionTLV.type = CANFD_Option_MCAN_MODE;
    optionTLV.length = sizeof(uint8_t);
    value = 0U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Mode - NORMAL failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(txMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Tx transmit cancel testing for %d iterations Passed\n", iterationCount);
    return retVal;
}

static int32_t mcanPowerDownTest()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    CANFD_MsgObjHandle          rxMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    uint8_t                     value;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams   rxMsgObjectParams;
    CANFD_MCANLoopbackCfgParams mcanloopbackParams;
    CANFD_MCANMsgObjectStats    msgObjStats;

    gTxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(&mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (40)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x4U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x1U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    mcanloopbackParams.enable = 1;
    mcanloopbackParams.mode = CANFD_MCANLoopBackMode_EXTERNAL;

    optionTLV.type = CANFD_Option_MCAN_LOOPBACK;
    optionTLV.length = sizeof(CANFD_MCANLoopbackCfgParams);
    optionTLV.value = (void*) &mcanloopbackParams;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Loopback failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_11_BIT;
    txMsgObjectParams.msgIdentifier = 0x29E;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        System_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_11_BIT;
    rxMsgObjectParams.msgIdentifier = 0x29E;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        System_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the measurement counters */
    minRxTicks   = 0xFFFFFFFFU;
    maxRxTicks   = 0U;
    minTxTicks   = 0xFFFFFFFFU;
    maxTxTicks   = 0U;
    totalTxTicks = 0U;
    totalRxTicks = 0U;

    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        /* Reset the counter: */
        Test_benchmarkStart(0);

        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);

            if (retVal < 0)
            {
                System_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
                return -1;
            }
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

    System_printf("Debug: Number of iterations              : %d\n", iterationCount);
    System_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
    System_printf("Debug: Number of messages received       : %d\n", gRxPkts);
    System_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    System_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    System_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    System_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    System_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    System_printf("\n\n");

    msgObjStats.handle = rxMsgObjHandle;
    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);

    System_printf("\n\n");

    /* Power Down the module */
    optionTLV.type = CANFD_Option_MCAN_POWER_DOWN;
    optionTLV.length = sizeof(uint8_t);
    value = 1U;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Power Down failed [Error code %d]\n", errCode);
        return -1;
    }
    else
        System_printf ("Debug: CANFD Module Power Down sucessful\n");

    optionTLV.type = CANFD_Option_MCAN_POWER_DOWN;
    optionTLV.length = sizeof(uint8_t);
    value = 0;
    optionTLV.value = (void*) &value;

    retVal =  CANFD_setOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD set option Wake Up failed [Error code %d]\n", errCode);
        return -1;
    }
    else
        System_printf ("Debug: CANFD Module is Wake Up sucessful\n");

    System_printf("\n\n");

    iterationCount = 0;
    while (iterationCount != MCAN_APP_TEST_MESSAGE_COUNT)
    {
        /* Reset the counter: */
        Test_benchmarkStart(0);

        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);

            if (retVal < 0)
            {
                System_printf ("Error: CANFD transmit data for iteration %d failed [Error code %d]\n", iterationCount, errCode);
                return -1;
            }
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

    System_printf("\n\n");
    System_printf("Debug: Number of iterations              : %d\n", iterationCount);
    System_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
    System_printf("Debug: Number of messages received       : %d\n", gRxPkts);
    System_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
    System_printf("Debug: Number of data mismatch           : %d\n", gErrDataMissMatchCnt);
    System_printf("Debug: Number of Frame mismatch          : %d\n", gErrFrameType);
    System_printf("Debug: Number of Get_Data errors         : %d\n", gErrGetData);
    System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
    System_printf("\n\n");

    msgObjStats.handle = txMsgObjHandle;
    optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
    optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
    optionTLV.value = (void*) &msgObjStats;

    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
    System_printf("\n\n");

    msgObjStats.handle = rxMsgObjHandle;
    retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        return -1;
    }

    System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
    System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
    System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
    System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);

    System_printf("\n\n");

    retVal = CANFD_deleteMsgObject(txMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deleteMsgObject(rxMsgObjHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD delete Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    retVal = CANFD_deinit(canHandle, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD deinit failed [Error code %d]\n", errCode);
        return -1;
    }

    if ((gErrMsgLstCnt != 0) || (gErrDataMissMatchCnt != 0) && (gErrFrameType == 0) && (gErrGetData == 0))
    {
        System_printf("Debug: Power down testing with data failed\n");
        retVal = -1;
    }
    return retVal;
}

static int32_t mcanTransmitTest()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle;
    CANFD_MsgObjHandle          rxMsgObjHandle;
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_OptionTLV             optionTLV;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams   rxMsgObjectParams;
    CANFD_MCANMsgObjectStats    msgObjStats;
    CANFD_MCANErrCntStatus      errCounter;
    CANFD_MCANProtocolStatus    protoStatus;
	uint32_t                    length = MCAN_APP_TEST_DATA_SIZE;
	uint32_t                    index = 0U;
	uint32_t                    msgId ;
    uint32_t                    idIndex = 0;

    gTxDoneFlag = 0;
    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(&mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (40)/(((2+2+3)+1)*BRP) = 5Mhz
    */
#if 1
    mcanBitTimingParams.nomBrp      = 0x2U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

	/*5MHz*/
    mcanBitTimingParams.dataBrp     = 0x1U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;
#else
    mcanBitTimingParams.nomBrp      = 0x2U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

	/*8MHz*/
	mcanBitTimingParams.dataBrp     = 0x1U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x1U;
    mcanBitTimingParams.dataPseg2   = 0x1U;
    mcanBitTimingParams.dataSjw     = 0x1U;
#endif

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the transmit message object */
    txMsgObjectParams.direction = CANFD_Direction_TX;
    txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    txMsgObjectParams.msgIdentifier = 0xC1;

    txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
    if (txMsgObjHandle == NULL)
    {
        System_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
    rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    rxMsgObjectParams.msgIdentifier = 0xC1;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
    if (rxMsgObjHandle == NULL)
    {
        System_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
        return -1;
    }

    while (1)
    {
        /* Send data over Tx message object */
        if (testFrameType == CANFD_MCANFrameType_FD)
		{

			length = 128U;
            if(idIndex > 10)
            {
                idIndex = 0;
            }
			msgId = txMsgObjectParams.msgIdentifier + idIndex;
			Task_sleep(1U);
            while(length > MCAN_APP_TEST_DATA_SIZE)
            {

				retVal = CANFD_transmitData (txMsgObjHandle, msgId, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
				length = length - MCAN_APP_TEST_DATA_SIZE;
				Task_sleep(1);
			}
            idIndex++;
		}
        else
        {

			length = 128U;
			index = 0;
			msgId = txMsgObjectParams.msgIdentifier;
			Task_sleep(1);
            while(length > 8U)
            {

				retVal = CANFD_transmitData (txMsgObjHandle, msgId, CANFD_MCANFrameType_CLASSIC, 8U, &txData[index], &errCode);
				length = length - 8U;
				index = index + 8U;
				msgId = msgId + 1U;
				Task_sleep(1U);
			}
			retVal = CANFD_transmitData (txMsgObjHandle, msgId, CANFD_MCANFrameType_CLASSIC, length, &txData[index], &errCode);

		}
        Task_sleep(2U);
        if (retVal < 0)
        {
            gErrMsgLstCnt++;
            continue;
        }

        iterationCount++;

        if (gDisplayStats == 1U)
        {
            gDisplayStats = 0;

            System_printf("Debug: Number of iterations              : %d\n", iterationCount);
            System_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
            System_printf("Debug: Number of messages received       : %d\n", gRxPkts);
            System_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
            System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
            System_printf("Debug: Error Reason                      : %d\n", gErrorReason);
            System_printf("\n\n");

            msgObjStats.handle = txMsgObjHandle;
            optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
            optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
            optionTLV.value = (void*) &msgObjStats;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
            System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            System_printf("\n\n");

            msgObjStats.handle = rxMsgObjHandle;
            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
            System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            System_printf("\n\n");

            optionTLV.type = CANFD_Option_MCAN_ERROR_COUNTER;
            optionTLV.length = sizeof(CANFD_MCANErrCntStatus);
            optionTLV.value = (void*) &errCounter;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CANFD get error counter failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: Receive passive status            : %d\n", errCounter.rpStatus);
            System_printf("Debug: Transmit Error Counter            : %d\n", errCounter.transErrLogCnt);
            System_printf("Debug: Receive Error Counter             : %d\n", errCounter.recErrCnt);
            System_printf("Debug: Error Logging Counter             : %d\n", errCounter.canErrLogCnt);
            System_printf("\n\n");

            optionTLV.type = CANFD_Option_MCAN_PROTOCOL_STATUS;
            optionTLV.length = sizeof(CANFD_MCANProtocolStatus);
            optionTLV.value = (void*) &protoStatus;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CANFD get protocol status failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: LEC                               : %d\n", protoStatus.lastErrCode);
            System_printf("Debug: Activity                          : %d\n", protoStatus.act);
            System_printf("Debug: Error Passive                     : %d\n", protoStatus.errPassive);
            System_printf("Debug: Warning Status                    : %d\n", protoStatus.warningStatus);
            System_printf("Debug: Bus Off Status                    : %d\n", protoStatus.busOffStatus);
            System_printf("Debug: Data Phase LEC                    : %d\n", protoStatus.dlec);
            System_printf("Debug: Rx ESI flag                       : %d\n", protoStatus.resi);
            System_printf("Debug: Rx BRS flag                       : %d\n", protoStatus.rbrs);
            System_printf("Debug: Rx CAN FD                         : %d\n", protoStatus.rfdf);
            System_printf("Debug: Protocol Exception Event          : %d\n", protoStatus.pxe);
            System_printf("Debug: TDC value                         : %d\n", protoStatus.tdcv);
            System_printf("\n\n");
        }
    }
}

static int32_t mcanEVM_EVMTest()
{
    CANFD_Handle                        canHandle;
    CANFD_MsgObjHandle                  txMsgObjHandle;
    CANFD_MsgObjHandle                  rxMsgObjHandle;
    CANFD_MsgObjHandle                  rxRangeMsgObjHandle;
    int32_t                             retVal = 0;
    int32_t                             errCode = 0;
    int32_t                             i;
    volatile float                      throughput;
    CANFD_OptionTLV                     optionTLV;
    CANFD_MCANInitParams                mcanCfgParams;
    CANFD_MCANBitTimingParams           mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams           txMsgObjectParams;
    CANFD_MCANMsgObjCfgParams           rxMsgObjectParams;
    CANFD_MCANRxMsgObjRangeCfgParams    rxRangeMsgObjectParams;
    CANFD_MCANMsgObjectStats            msgObjStats;
    CANFD_MCANErrCntStatus              errCounter;
    CANFD_MCANProtocolStatus            protoStatus;

    gTxDoneFlag = 0;
    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(&mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (40)/(((2+2+3)+1)*BRP) = 5Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x2U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x1U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x2U;
    mcanBitTimingParams.dataPseg2   = 0x3U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the measurement counters */
    minRxTicks   = 0xFFFFFFFFU;
    maxRxTicks   = 0U;
    minTxTicks   = 0xFFFFFFFFU;
    maxTxTicks   = 0U;
    totalTxTicks = 0U;
    totalRxTicks = 0U;

    /* Set the EVM in transmit mode*/
    if (gMCANMode == MCAN_APP_TEST_EVM_MODE_TX)
    {
        /* Setup the transmit message object */
        txMsgObjectParams.direction = CANFD_Direction_TX;
        txMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
        txMsgObjectParams.msgIdentifier = 0xC1;

        txMsgObjHandle = CANFD_createMsgObject (canHandle, &txMsgObjectParams, &errCode);
        if (txMsgObjHandle == NULL)
        {
            System_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
            return -1;
        }
        else
        {
            System_printf ("Debug: EVM in transmit mode...\n");
            System_printf("\n\n");
        }

        for (i = 0; i < MCAN_APP_TEST_MESSAGE_COUNT; i++)
        {
            /* Send data over Tx message object */
            if (testFrameType == CANFD_MCANFrameType_FD)
                retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_FD, MCAN_APP_TEST_DATA_SIZE, &txData[0], &errCode);
            else
    		{
			    uint32_t len = MCAN_APP_TEST_DATA_SIZE;
		    	uint32_t index = 0U;
                while(len > 8U)
                {
			    	retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[index], &errCode);
		    		len = len - 8U;
	    			index = index + 8U;
    			}
			    retVal = CANFD_transmitData (txMsgObjHandle, txMsgObjectParams.msgIdentifier, CANFD_MCANFrameType_CLASSIC, len, &txData[index], &errCode);

		    }
            if (retVal < 0)
            {
                return -1;
            }
            /* Wait for the transmit complete interrupt */
            while (gTxDoneFlag == 0);
            gTxDoneFlag = 0;
        }

        System_printf("Debug: Number of iterations              : %d\n", iterationCount);
        System_printf("Debug: Number of messages transmitted    : %d\n", gTxPkts);
        System_printf("Debug: Number of messages lost           : %d\n", gErrMsgLstCnt);
        System_printf("Debug: Error Status Interrupt            : %d\n", gErrStatusInt);
        System_printf("\n\n");

        msgObjStats.handle = txMsgObjHandle;
        optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
        optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
        optionTLV.value = (void*) &msgObjStats;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        }

        System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
        System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
        System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
        System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
        System_printf("\n\n");
    }

    /* Set the EVM in recieve mode*/
    if ((gMCANMode == MCAN_APP_TEST_EVM_MODE_RX) || (gMCANMode == MCAN_APP_TEST_EVM_MODE_FIFO))
    {
        if (gMCANMode == MCAN_APP_TEST_EVM_MODE_RX)
        {
            /* Setup the receive message object */
            rxMsgObjectParams.direction = CANFD_Direction_RX;
            rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
            rxMsgObjectParams.msgIdentifier = 0xC1;

            rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);
            if (rxMsgObjHandle == NULL)
            {
                System_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
                return -1;
            }
            else
            {
                System_printf ("Debug: EVM in Rx buffer mode...\n");
            }

        }
        else if (gMCANMode == MCAN_APP_TEST_EVM_MODE_FIFO)
        {
            rxRangeMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
        	rxRangeMsgObjectParams.startMsgIdentifier = 0xC0;
        	rxRangeMsgObjectParams.endMsgIdentifier = 0xC4;

    	    rxRangeMsgObjHandle = CANFD_createRxRangeMsgObject (canHandle, &rxRangeMsgObjectParams, &errCode);
    	    if (rxRangeMsgObjHandle == NULL)
            {
	            System_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
    	        return -1;
    	    }
            else
            {
                System_printf ("Debug: EVM in Rx FIFO mode...\n");
            }
        }

        /* Wait for the packet to arrive */
        while (gRxPkts != MCAN_APP_TEST_MESSAGE_COUNT);

        gRxDoneFlag = 0;

        if (gMCANMode == MCAN_APP_TEST_EVM_MODE_RX)
            msgObjStats.handle = rxMsgObjHandle;
        else
            msgObjStats.handle = rxRangeMsgObjHandle;

        optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
        optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
        optionTLV.value = (void*) &msgObjStats;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
        }

        System_printf("Debug: Message Identifier                : 0x%x\n", msgObjStats.startMsgIdentifier);
        System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
        System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
        System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
        System_printf("Debug: Number of messages received       : %d\n", gRxPkts);
        System_printf("\n\n");


        System_printf("Debug: Receive & Transmit Measurements\n");
        System_printf("Debug: Rx Min:%d Max:%d Total:%d ticks, Total packets: %d\n", minRxTicks, maxRxTicks, totalRxTicks, gRxPkts);

        /* 8 bits per byte * (data size + header) * Number of packet * VBUS frequency / total ticks */
        throughput = 8.0 * (MCAN_APP_TEST_DATA_SIZE + 16U) * gRxPkts * VBUSP_FREQ / totalRxTicks;
        printf("Debug: Rx Throughput: %.2f Mbps\n", throughput);

        System_printf("\n\n");

        optionTLV.type = CANFD_Option_MCAN_ERROR_COUNTER;
        optionTLV.length = sizeof(CANFD_MCANErrCntStatus);
        optionTLV.value = (void*) &errCounter;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            System_printf ("Error: CANFD get error counter failed [Error code %d]\n", errCode);
        }

        System_printf("Debug: Receive passive status            : %d\n", errCounter.rpStatus);
        System_printf("Debug: Transmit Error Counter            : %d\n", errCounter.transErrLogCnt);
        System_printf("Debug: Receive Error Counter             : %d\n", errCounter.recErrCnt);
        System_printf("Debug: Error Logging Counter             : %d\n", errCounter.canErrLogCnt);
        System_printf("\n\n");

        optionTLV.type = CANFD_Option_MCAN_PROTOCOL_STATUS;
        optionTLV.length = sizeof(CANFD_MCANProtocolStatus);
        optionTLV.value = (void*) &protoStatus;

        retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
        if (retVal < 0)
        {
            System_printf ("Error: CANFD get protocol status failed [Error code %d]\n", errCode);
        }

        System_printf("Debug: LEC                               : %d\n", protoStatus.lastErrCode);
        System_printf("Debug: Activity                          : %d\n", protoStatus.act);
        System_printf("Debug: Error Passive                     : %d\n", protoStatus.errPassive);
        System_printf("Debug: Warning Status                    : %d\n", protoStatus.warningStatus);
        System_printf("Debug: Bus Off Status                    : %d\n", protoStatus.busOffStatus);
        System_printf("Debug: Data Phase LEC                    : %d\n", protoStatus.dlec);
        System_printf("Debug: Rx ESI flag                       : %d\n", protoStatus.resi);
        System_printf("Debug: Rx BRS flag                       : %d\n", protoStatus.rbrs);
        System_printf("Debug: Rx CAN FD                         : %d\n", protoStatus.rfdf);
        System_printf("Debug: Protocol Exception Event          : %d\n", protoStatus.pxe);
        System_printf("Debug: TDC value                         : %d\n", protoStatus.tdcv);
        System_printf("\n\n");
    }

    return 0;
}

static int32_t mcanMultiTransmission()
{
    CANFD_Handle                canHandle;
    CANFD_MsgObjHandle          txMsgObjHandle[5U];
    int32_t                     retVal = 0;
    int32_t                     errCode = 0;
    CANFD_MCANInitParams        mcanCfgParams;
    CANFD_MCANBitTimingParams   mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams   txMsgObjectParams[5U];
	int32_t                     index;

    gTxDoneFlag = 0;
    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(&mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Nominal Bit rate = (40)/(((2+8+6)+5)*2) = 1Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x2U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x2U;
    mcanBitTimingParams.dataPropSeg = 0x8U;
    mcanBitTimingParams.dataPseg1   = 0x6U;
    mcanBitTimingParams.dataPseg2   = 0x5U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    while (1)
    {
        /* Setup the transmit message object */
	    for(index = 0; index < 5U ; index++)
    	{
	        txMsgObjectParams[index].direction = CANFD_Direction_TX;
	        txMsgObjectParams[index].msgIdType = CANFD_MCANXidType_29_BIT;
    	    txMsgObjectParams[index].msgIdentifier = 0xC1 + index;

    	    txMsgObjHandle[index] = CANFD_createMsgObject (canHandle, &txMsgObjectParams[index], &errCode);
	        if (txMsgObjHandle[index] == NULL)
	        {
	            System_printf ("Error: CANFD create Tx message object failed [Error code %d]\n", errCode);
    	        return -1;
	        }
    	}
        gTxPkts = 0;
    	for(index = 0; index < 5U ; index++)
	    {
		    retVal = CANFD_transmitData (txMsgObjHandle[index], txMsgObjectParams[index].msgIdentifier, CANFD_MCANFrameType_CLASSIC, 8U, &txData[0], &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CANFD transmit data retry failed [Error code %d]\n", errCode);
            }
            Task_sleep(1);
    	}

        /* Delete the transmit message object */
    	for(index = 0; index < 5U ; index++)
	    {
            retVal = CANFD_deleteMsgObject(txMsgObjHandle[index], &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CANFD delete Tx message object failed [Error code %d]\n", errCode);
                return -1;
            }
        }
    }
}

static int32_t mcanMsgIdRangeTest()
{
    CANFD_Handle                        canHandle;
    CANFD_MsgObjHandle                  rxMsgObjHandle;
    CANFD_MsgObjHandle                  rxRangeMsgObjHandle[3U];
    int32_t                             retVal = 0;
    int32_t                             errCode = 0;
    CANFD_MCANInitParams                mcanCfgParams;
    CANFD_MCANBitTimingParams           mcanBitTimingParams;
    CANFD_MCANMsgObjCfgParams           rxMsgObjectParams;
    CANFD_MCANRxMsgObjRangeCfgParams    rxRangeMsgObjectParams;
    CANFD_MCANMsgObjectStats            msgObjStats;
    CANFD_OptionTLV                     optionTLV;
	int32_t                             index;

    gRxDoneFlag = 0;

    MCANAppInitParams (&mcanCfgParams);

    /* Initialize the CANFD driver */
    canHandle = CANFD_init(&mcanCfgParams, &errCode);
    if (canHandle == NULL)
    {
        System_printf ("Error: CANFD Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Configuring 1Mbps and 5Mbps as nominal and data bit-rate respectively
        Prop seg: 8
        Ph seg 1: 6
        Ph Seg2 : 5
        Sync jump: 1
        BRP(Baud rate Prescaler): 2

        Nominal Bit rate = (40)/(((8+6+5)+1)*BRP) = 1Mhz

        Timing Params for Data Bit rate:
        Prop seg: 2
        Ph seg 1: 2
        Ph Seg2 : 3
        Sync jump: 1
        BRP(Baud rate Prescaler): 1

        Data Bit rate = (40)/(((1+2+1)+1)*1) = 8Mhz
    */

    mcanBitTimingParams.nomBrp      = 0x2U;
    mcanBitTimingParams.nomPropSeg  = 0x8U;
    mcanBitTimingParams.nomPseg1    = 0x6U;
    mcanBitTimingParams.nomPseg2    = 0x5U;
    mcanBitTimingParams.nomSjw      = 0x1U;

    mcanBitTimingParams.dataBrp     = 0x1U;
    mcanBitTimingParams.dataPropSeg = 0x2U;
    mcanBitTimingParams.dataPseg1   = 0x1U;
    mcanBitTimingParams.dataPseg2   = 0x1U;
    mcanBitTimingParams.dataSjw     = 0x1U;

    /* Configure the CAN driver */
    retVal = CANFD_configBitTime (canHandle, &mcanBitTimingParams, &errCode);
    if (retVal < 0)
    {
        System_printf ("Error: CANFD Module configure bit time failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Setup the single receive message object */
    rxMsgObjectParams.direction = CANFD_Direction_RX;
	rxMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    rxMsgObjectParams.msgIdentifier = 0xC1;

    rxMsgObjHandle = CANFD_createMsgObject (canHandle, &rxMsgObjectParams, &errCode);

    if (rxMsgObjHandle == NULL)
    {
	    System_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
    	return -1;
    }


    /* Setup the range receive message object */
	for(index = 0; index < 3U ; index++)
    {
	    rxRangeMsgObjectParams.msgIdType = CANFD_MCANXidType_29_BIT;
    	rxRangeMsgObjectParams.startMsgIdentifier = 0xC2 + (10 * index);
    	rxRangeMsgObjectParams.endMsgIdentifier = 0xC4 + (10 * index);

    	rxRangeMsgObjHandle[index] = CANFD_createRxRangeMsgObject (canHandle, &rxRangeMsgObjectParams, &errCode);
	    if (rxRangeMsgObjHandle[index] == NULL)
        {
	        System_printf ("Error: CANFD create Rx message object failed [Error code %d]\n", errCode);
    	    return -1;
	    }
    }


    while (1)
    {

        /* Wait for the packet to arrive */
        while (gRxDoneFlag == 0);
        gRxDoneFlag = 0;

        Task_sleep(1);

        if (gDisplayStats == 1U)
        {
            gDisplayStats = 0;

            msgObjStats.handle = rxMsgObjHandle;
            optionTLV.type = CANFD_Option_MCAN_MSG_OBJECT_STATS;
            optionTLV.length = sizeof(CANFD_MCANMsgObjectStats);
            optionTLV.value = (void*) &msgObjStats;

            retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
            if (retVal < 0)
            {
                System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
            }

            System_printf("Debug: Single Message Identifier\n");
            System_printf("Debug: Start Message Identifier          : 0x%x\n", msgObjStats.startMsgIdentifier);
            System_printf("Debug: End Message Identifier            : 0x%x\n", msgObjStats.endMsgIdentifier);
            System_printf("Debug: Direction                         : %s\n", (msgObjStats.direction == 0) ? "Receive" : "Transmit");
            System_printf("Debug: Number of interrupts received     : %d\n", msgObjStats.interruptsRxed);
            System_printf("Debug: Number of messages processed      : %d\n", msgObjStats.messageProcessed);
            System_printf("\n\n");

            for(index = 0; index < 3U ; index++)
            {
                msgObjStats.handle = rxRangeMsgObjHandle[index];

                retVal =  CANFD_getOptions(canHandle, &optionTLV, &errCode);
                if (retVal < 0)
                {
                    System_printf ("Error: CANFD get stats failed [Error code %d]\n", errCode);
                }

                System_printf("Debug: Range Message Identifier\n");
                System_printf("Debug: %d Start Message Identifier          : 0x%x\n", index, msgObjStats.startMsgIdentifier);
                System_printf("Debug: %d End Message Identifier            : 0x%x\n", index, msgObjStats.endMsgIdentifier);
                System_printf("Debug: %d Direction                         : %s\n", index, (msgObjStats.direction == 0) ? "Receive" : "Transmit");
                System_printf("Debug: %d Number of interrupts received     : %d\n", index, msgObjStats.interruptsRxed);
                System_printf("Debug: %d Number of messages processed      : %d\n", index, msgObjStats.messageProcessed);
                System_printf("\n\n");
            }
            System_printf("Debug: Number of Frame mismatch            : %d\n", gErrFrameType);
            System_printf("Debug: Number of Get_Data errors           : %d\n", gErrGetData);
            System_printf("Debug: Error Status Interrupt              : %d\n", gErrStatusInt);
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
        System_printf ("CANFD Unit Test Menu                        \n");
        System_printf ("Please select the type of test to execute:  \n");
        System_printf ("1. MCAN Internal loopback test              \n");
        System_printf ("2. MCAN External loopback test              \n");
        System_printf ("3. MCAN Multiple Tx test                    \n");
        System_printf ("4. MCAN External Tx/Rx test                 \n");
        System_printf ("5. MCAN EVM-EVM test                        \n");
        System_printf ("6. MCAN Tx Cancel test                      \n");
        System_printf ("7. MCAN Power down test                     \n");
        System_printf ("8. MCAN Message Id Range test               \n");
        System_printf ("*******************************************************\n");
        System_printf ("> Enter your selection: ");

        while (testSelection == 0);

        /* Validate the selection: */
        if ((testSelection >= MCAN_APP_TEST_INTERNAL_LOOPBACK) && (testSelection <= MCAN_APP_TEST_MSGID_RANGE))
            break;
    }

    if (testSelection == MCAN_APP_TEST_INTERNAL_LOOPBACK)
    {
        System_printf("Debug: Internal loopback testing\n");
        retVal = mcanLoopbackTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Internal loopback testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Internal loopback testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == MCAN_APP_TEST_EXTERNAL_LOOPBACK)
    {
        System_printf("Debug: External loopback testing\n");
        retVal = mcanLoopbackTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("External loopback testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("External loopback testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == MCAN_APP_TEST_TX_CANCEL)
    {
        System_printf("Debug: Tx Cancel testing\n");
        retVal = mcanTestTxCancel();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Tx Cancel testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Tx Cancel testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == MCAN_APP_TEST_EXTERNAL_DATA)
    {
        System_printf("Debug: External transmit testing\n");
        retVal = mcanTransmitTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("External transmit testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("External transmit testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == MCAN_APP_TEST_EVM_EVM)
    {
        System_printf("Debug: EVM-EVM testing\n");
        retVal = mcanEVM_EVMTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("EVM to EVM testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("EVM to EVM testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == MCAN_APP_TEST_POWERDOWN)
    {
        System_printf("Debug: Power Down testing\n");
        retVal = mcanPowerDownTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Power Down testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Power Down testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == MCAN_APP_TEST_MULTIPLE_TRANSMISSION)
	{
		System_printf("Debug: Multiple transmission testing\n");
        retVal = mcanMultiTransmission();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Multiple transmission testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Multiple transmission testing", MCPI_TestResult_PASS);
    }
    else if (testSelection == MCAN_APP_TEST_MSGID_RANGE)
    {
        System_printf("Debug: Message Id Range testing\n");
        retVal = mcanMsgIdRangeTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Message Id Range testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Message Id Range testing", MCPI_TestResult_PASS);
    }

    if (retVal < 0)
    {
        System_printf("Debug:CANFD testing failed\n");
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
    SOC_Handle      socHandle;
    SOC_Cfg         socCfg;
    int32_t         errCode;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);

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

    /* Configure the divide value for MCAN source clock */
    SOC_setPeripheralClock(socHandle, SOC_MODULE_MCAN, SOC_CLKSOURCE_VCLK, 4U, &errCode);

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6U*1024U;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
    return 0;
}


