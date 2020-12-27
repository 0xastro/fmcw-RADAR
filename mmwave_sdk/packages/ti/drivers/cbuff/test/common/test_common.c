/*
 *   @file  test_common.c
 *
 *   @brief
 *      Unit Test Functions which test the CBUFF functionality
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

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>

/* mmWaveSDK Include Files */
#include <ti/common/sys_common.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 ***************************** Global Variables ***************************
 **************************************************************************/

/**
 * @brief   Global String for each CBUFF data format for display
 */
char* gCBUFFDataFormatString[] =
{
    "ADC",                                              /* CBUFF_DataFmt_ADC_DATA       */
    "Chirp Parameters + ADC" ,                          /* CBUFF_DataFmt_CP_ADC         */
    "ADC + Chirp Parameters" ,                          /* CBUFF_DataFmt_ADC_CP         */
    "Chirp Parameters + ADC + Chirp Quality",           /* CBUFF_DataFmt_CP_ADC_CQ      */
    "ADC + User",                                       /* CBUFF_DataFmt_ADC_USER       */
    "Chirp Parameters + ADC + Chirp Quality + User",    /* CBUFF_DataFmt_CP_ADC_CQ_USER */
    "Invalid Data Format String"
};

/**
 * @brief   User Buffer(s): Placed into the specific L3 memory
 */
#pragma DATA_SECTION(gUserBuffer0,      ".cbuffL3Memory");
#pragma DATA_SECTION(gUserBuffer1,      ".cbuffL3Memory");
#pragma DATA_SECTION(gUserBuffer2,      ".cbuffL3Memory");
#pragma DATA_SECTION(gHeader,           ".cbuffL3Memory");
uint16_t gUserBuffer0[16];
uint16_t gUserBuffer1[16];
uint16_t gUserBuffer2[16];
uint16_t gHeader[16];

/**************************************************************************
 *************************** Extern Declarations **************************
 **************************************************************************/
extern int32_t Test_EDMAAllocateCBUFFChannel
(
    CBUFF_EDMAInfo*         ptrEDMAInfo,
    CBUFF_EDMAChannelCfg*   ptrEDMAChannelCfg
);
extern void     Test_EDMAFreeCBUFFChannel(CBUFF_EDMAChannelCfg* ptrEDMACfg);
extern uint8_t  gCBUFFEDMAChannelResourceCounter;
extern uint8_t  gTestEDMAChannelAllocationFailure;

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/
static int32_t Test_enableADCChannel (ADCBuf_Handle adcBufHandle, uint8_t numChannel);

/**************************************************************************
 ************************** CBUFF Test Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable "numChannel" ADC channels.
 *      This will disable all the channels and enable only the number of
 *      channels specified in the argument.
 *      For example:-
 *          numChannel = 1       ADC Channel 0 is enabled
 *          numChannel = 2       ADC Channel 0 and 1 are enabled
 *          numChannel = 3       ADC Channel 0, 1 and 2 are enabled
 *          numChannel = 4       ADC Channel 0, 1, 2 and 3 are enabled
 *
 *  @param[in]  adcBufHandle
 *      ADCBUF Driver Handle
 *  @param[in]  numChannel
 *      Number of channels to be enabled
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -  <0
 */
static int32_t Test_enableADCChannel (ADCBuf_Handle adcBufHandle, uint8_t numChannel)
{
    int32_t             errCode;
    ADCBuf_RxChanConf   rxChanCfg;
    ADCBuf_CQConf       cqCfg;
    uint8_t             index;
    uint32_t            rxChanMask  = 0xF;

    /* Sanity Check: Validate the number of channels to be enabled. */
    DebugP_assert (numChannel != 0U);
    DebugP_assert (numChannel <= SYS_COMMON_NUM_RX_CHANNEL);

    /************************************************************************
     * Disable all the ADC channels:
     ************************************************************************/
    errCode = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CHANNEL_DISABLE, (void *)&rxChanMask);
    if (errCode != ADCBuf_STATUS_SUCCESS)
    {
        /* Error: Unable to disable all channels. */
        System_printf ("Error: Unable to disable the ADC Channels [Error: %d]\n", errCode);
        return -1;
    }

    /************************************************************************
     * Enable the "N" channels:
     ************************************************************************/
    for (index = 0U; index < numChannel; index++)
    {
        /* Initialize the receive channel configuration: */
        memset((void*)&rxChanCfg, 0, sizeof(ADCBuf_RxChanConf));

        /* Setup the channel to enabled: */
        rxChanCfg.channel = index;

        /* Enable the channel: */
        errCode  = ADCBuf_control (adcBufHandle, ADCBufMMWave_CMD_CHANNEL_ENABLE, (void *)&rxChanCfg);
        if (errCode != ADCBuf_STATUS_SUCCESS)
        {
            /* Error: Unable to enable the receive channel */
            System_printf ("Error: Unable to enable the ADC Channel %d [Error: %d]\n", errCode);
            return -1;
        }
    }

    /************************************************************************
     * Chirp Quality (CQ):
     ************************************************************************/
    memset((void *)&cqCfg, 0, sizeof(ADCBuf_CQConf));

    /* Populate the Chirp Quality Configuration:
     * - 16bit
     * - CQ1 starts from the beginning
     * - CQ2 should be 16bytes aligned */
    cqCfg.cqDataWidth   = 0;
    cqCfg.cq1AddrOffset = 0x0U;
    cqCfg.cq2AddrOffset = 0x1000U;

    /* Configure the Chirp Quality: */
    errCode = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CONF_CQ, (void *)&cqCfg);
    if (errCode != ADCBuf_STATUS_SUCCESS)
    {
        System_printf ("Error: MMWDemoDSS Unable to configure the CQ\n");
        return -1;
    }

    /* ADC has been configured properly: */
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the session creation & deletion
 *      for different data formats.
 *
 *  @param[in]  socHandle
 *      SOC Driver Handle
 *  @param[in]  edmaHandle
 *      EDMA Driver Handle
 *  @param[in]  adcBufHandle
 *      ADCBUF Driver Handle
 *  @param[in]  dataMode
 *      CBUFF Driver Data Mode
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -  <0
 */
int32_t Test_dataFormats
(
    SOC_Handle      socHandle,
    EDMA_Handle     edmaHandle,
    ADCBuf_Handle   adcBufHandle,
    CBUFF_DataMode  dataMode
)
{
    CBUFF_SessionCfg    sessionCfg;
    CBUFF_Handle        cbuffHandle;
    CBUFF_DataFmt       dataFormat;
    int32_t             errCode;
    char                testMessage[256];
    CBUFF_InitCfg       initCfg;
    CBUFF_SessionHandle sessionHandle;
    uint8_t             numChannel;

    /* Debug Message: */
    System_printf ("---------------------------------------------------\n");
    System_printf ("Debug: Testing CBUFF Data Formats in %s mode\n",
                   (dataMode == CBUFF_DataMode_INTERLEAVED) ? "Interleaved" : "Non Interleaved");
    System_printf ("---------------------------------------------------\n");

    /* Initialize the configuration: */
    memset ((void*)&initCfg, 0, sizeof(CBUFF_InitCfg));

    /* Populate the configuration: */
    initCfg.socHandle                = socHandle;
    initCfg.outputDataFmt            = CBUFF_OutputDataFmt_16bit;
    initCfg.enableECC                = 1U;
    initCfg.crcEnable                = 1U;
    initCfg.enableDebugMode          = false;
    initCfg.maxSessions              = 2U;
    initCfg.interface                = CBUFF_Interface_LVDS;
    initCfg.u.lvdsCfg.crcEnable      = 0U;
    initCfg.u.lvdsCfg.msbFirst       = 1U;
    initCfg.u.lvdsCfg.ddrClockMode   = 1U;
    initCfg.u.lvdsCfg.ddrClockModeMux= 1U;

    /* Setup the lanes: This configuration is platform specific */
#ifdef SOC_XWR14XX
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0xFU;
#else
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0x3U;
#endif

    /* Initialize the CBUFF Driver: */
    cbuffHandle = CBUFF_init (&initCfg, &errCode);
    if (cbuffHandle == NULL)
    {
        System_printf ("Error: CBUFF Driver initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Cycle through all the ADC Channels */
    for (numChannel = 1U; numChannel <= SYS_COMMON_NUM_RX_CHANNEL; numChannel++)
    {
        /* Enable the  */
        Test_enableADCChannel (adcBufHandle, numChannel);

        /* Cycle through all the data formats: */
        for (dataFormat = CBUFF_DataFmt_ADC_DATA; dataFormat < CBUFF_DataFmt_MAX; dataFormat++)
        {
            /* Create the MCPI Test Message: */
            sprintf (testMessage, "CBUFF HW Session: ADC Channels=%d Data Format [%s] %s",
                     numChannel,
                     (dataMode == CBUFF_DataMode_INTERLEAVED) ? "Interleaved" : "Non-Interleaved",
                     gCBUFFDataFormatString[dataFormat]);

            /* Initialize the configuration */
            memset ((void*)&sessionCfg, 0, sizeof(CBUFF_SessionCfg));

            /* Populate the configuration: */
            sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_HW;
            sessionCfg.edmaHandle                         = edmaHandle;
            sessionCfg.allocateEDMAChannelFxn             = Test_EDMAAllocateCBUFFChannel;
            sessionCfg.freeEDMAChannelFxn                 = Test_EDMAFreeCBUFFChannel;
            sessionCfg.dataType                           = CBUFF_DataType_COMPLEX;
            sessionCfg.header.size                        = 0U;
            sessionCfg.header.address                     = 0U;
            sessionCfg.u.hwCfg.dataMode                   = dataMode;
            sessionCfg.u.hwCfg.dataFormat                 = dataFormat;
            sessionCfg.u.hwCfg.opMode                     = CBUFF_OperationalMode_CHIRP;
            sessionCfg.u.hwCfg.numChirpsPerFrame          = 0x20U;
            sessionCfg.u.hwCfg.chirpMode                  = 0x1U;
            sessionCfg.u.hwCfg.numADCSamples              = 256U;
            sessionCfg.u.hwCfg.adcBufHandle               = adcBufHandle;
            sessionCfg.u.hwCfg.cqSize[0]                  = 0U;
            sessionCfg.u.hwCfg.cqSize[1]                  = 128U;
            sessionCfg.u.hwCfg.cqSize[2]                  = 128U;
            sessionCfg.u.hwCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];
            sessionCfg.u.hwCfg.userBufferInfo[0].size     = sizeof(gUserBuffer0);
            sessionCfg.u.hwCfg.userBufferInfo[1].address  = (uint32_t)&gUserBuffer1[0];
            sessionCfg.u.hwCfg.userBufferInfo[1].size     = sizeof(gUserBuffer1);
            sessionCfg.u.hwCfg.userBufferInfo[2].address  = (uint32_t)&gUserBuffer2[0];
            sessionCfg.u.hwCfg.userBufferInfo[2].size     = sizeof(gUserBuffer2);

            /* Create the HW Session: */
            sessionHandle = CBUFF_createSession (cbuffHandle, &sessionCfg, &errCode);
            if (sessionHandle == NULL)
            {
                System_printf ("Error: Unable to create the session [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                return -1;
            }

            /* Debug Message: Display the EDMA Channel Usage for the test. */
            System_printf ("Debug: EDMA Channel Usage = %d\n", gCBUFFEDMAChannelResourceCounter);

            /* Activate the session: */
            if (CBUFF_activateSession (sessionHandle, &errCode) < 0)
            {
                System_printf ("Error: Unable to activate the session [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                return -1;
            }

            /* Deactivate the session: */
            if (CBUFF_deactivateSession (sessionHandle, &errCode) < 0)
            {
                System_printf ("Error: Unable to deactivate the session [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                return -1;
            }

            /* Delete the session: */
            if (CBUFF_deleteSession (sessionHandle, &errCode) < 0)
            {
                System_printf ("Error: Unable to delete the session [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                return -1;
            }

            /* Sanity Check: Ensure that all the EDMA resources have been cleaned up */
            if (gCBUFFEDMAChannelResourceCounter != 0)
            {
                System_printf ("Error: EDMA Channel Memory leak detected\n");
                return -1;
            }
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_PASS);
        }
    }

    /* Deinitialize and shutdown the CBUFF: */
    if (CBUFF_deinit (cbuffHandle, &errCode) < 0)
    {
        System_printf ("Debug: CBUFF Driver deinit %s failed [Error code %d]\n", errCode);
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the failure of EDMA Channel allocation.
 *
 *  @param[in]  socHandle
 *      SOC Driver Handle
 *  @param[in]  edmaHandle
 *      EDMA Driver Handle
 *  @param[in]  adcBufHandle
 *      ADCBUF Driver Handle
 *  @param[in]  dataMode
 *      CBUFF Driver Data Mode
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -  <0
 */
int32_t Test_EDMAChannelAllocationFailures
(
    SOC_Handle      socHandle,
    EDMA_Handle     edmaHandle,
    ADCBuf_Handle   adcBufHandle,
    CBUFF_DataMode  dataMode
)
{
    CBUFF_SessionCfg    sessionCfg;
    CBUFF_Handle        cbuffHandle;
    CBUFF_DataFmt       dataFormat;
    CBUFF_InitCfg       initCfg;
    int32_t             errCode;
    CBUFF_SessionHandle sessionHandle;
    char                testMessage[256];

    /* Debug Message: */
    System_printf ("---------------------------------------------------\n");
    System_printf ("Debug: Testing EDMA Channel Allocation Failures in %s mode\n",
                   (dataMode == CBUFF_DataMode_INTERLEAVED) ? "Interleaved" : "Non Interleaved");
    System_printf ("---------------------------------------------------\n");

    /* Set the EDMA Allocation Failure Flag */
    gTestEDMAChannelAllocationFailure = 1U;

    /* Initialize the configuration: */
    memset ((void*)&initCfg, 0, sizeof(CBUFF_InitCfg));

    /* Populate the configuration: */
    initCfg.socHandle                = socHandle;
    initCfg.outputDataFmt            = CBUFF_OutputDataFmt_16bit;
    initCfg.enableECC                = 1U;
    initCfg.crcEnable                = 1U;
    initCfg.enableDebugMode          = false;
    initCfg.maxSessions              = 2U;
    initCfg.interface                = CBUFF_Interface_LVDS;
    initCfg.u.lvdsCfg.crcEnable      = 0U;
    initCfg.u.lvdsCfg.msbFirst       = 1U;
    initCfg.u.lvdsCfg.ddrClockMode   = 1U;
    initCfg.u.lvdsCfg.ddrClockModeMux= 1U;

    /* Setup the lanes: This configuration is platform specific */
#ifdef SOC_XWR14XX
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0xFU;
#else
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0x3U;
#endif

    /* Initialize the CBUFF Driver: */
    cbuffHandle = CBUFF_init (&initCfg, &errCode);
    if (cbuffHandle == NULL)
    {
        System_printf ("Error: CBUFF Driver initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Enable all the ADC Channels for this test */
    Test_enableADCChannel (adcBufHandle, SYS_COMMON_NUM_RX_CHANNEL);

    /* Cycle through all the data formats: */
    for (dataFormat = CBUFF_DataFmt_ADC_DATA; dataFormat < CBUFF_DataFmt_MAX; dataFormat++)
    {
        /* Initialize the configuration */
        memset ((void*)&sessionCfg, 0, sizeof(CBUFF_SessionCfg));

        /* Create the MCPI Test Message: */
        sprintf (testMessage, "CBUFF: EDMA Memory Allocation Test for Data Format %s",
                 gCBUFFDataFormatString[dataFormat]);

        /* Populate the configuration: */
        sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_HW;
        sessionCfg.edmaHandle                         = edmaHandle;
        sessionCfg.allocateEDMAChannelFxn             = Test_EDMAAllocateCBUFFChannel;
        sessionCfg.freeEDMAChannelFxn                 = Test_EDMAFreeCBUFFChannel;
        sessionCfg.dataType                           = CBUFF_DataType_COMPLEX;
		sessionCfg.u.hwCfg.dataMode                   = dataMode;
		sessionCfg.u.hwCfg.dataFormat                 = dataFormat;
		sessionCfg.u.hwCfg.opMode                     = CBUFF_OperationalMode_CHIRP;
		sessionCfg.u.hwCfg.numChirpsPerFrame          = 0x20U;
        sessionCfg.u.hwCfg.chirpMode                  = 0x1U;
		sessionCfg.u.hwCfg.adcBufHandle               = adcBufHandle;
        sessionCfg.u.hwCfg.cqSize[0]                  = 0U;
        sessionCfg.u.hwCfg.cqSize[1]                  = 128U;
        sessionCfg.u.hwCfg.cqSize[2]                  = 128U;
		sessionCfg.u.hwCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];
		sessionCfg.u.hwCfg.userBufferInfo[0].size     = sizeof(gUserBuffer0);
		sessionCfg.u.hwCfg.userBufferInfo[1].address  = (uint32_t)&gUserBuffer1[0];
		sessionCfg.u.hwCfg.userBufferInfo[1].size     = sizeof(gUserBuffer1);
		sessionCfg.u.hwCfg.userBufferInfo[2].address  = (uint32_t)&gUserBuffer2[0];
		sessionCfg.u.hwCfg.userBufferInfo[2].size     = sizeof(gUserBuffer2);
		sessionCfg.u.hwCfg.numADCSamples              = 256U;

        /* Open the CBUFF Driver: */
        sessionHandle = CBUFF_createSession (cbuffHandle, &sessionCfg, &errCode);
        if (sessionHandle != NULL)
        {
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Open Failed: Validate the error code */
        if (errCode != CBUFF_EDMA_FAIL)
        {
            System_printf ("Error: CBUFF open failed with an invalid error code %d\n", errCode);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
        }
        else
        {
            /* Test Passed with a valid error code */
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_PASS);
        }
    }

    /* Deinitialize and shutdown the CBUFF: */
    if (CBUFF_deinit (cbuffHandle, &errCode) < 0)
    {
        System_printf ("Debug: CBUFF Driver deinit %s failed [Error code %d]\n", errCode);
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the invalid data size
 *
 *  @param[in]  socHandle
 *      SOC Driver Handle
 *  @param[in]  edmaHandle
 *      EDMA Driver Handle
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -  <0
 */
int32_t Test_InvalidDataSize
(
    SOC_Handle      socHandle,
    EDMA_Handle     edmaHandle
)
{
    CBUFF_SessionCfg    sessionCfg;
    CBUFF_Handle        cbuffHandle;
    CBUFF_InitCfg       initCfg;
    int32_t             errCode;
    CBUFF_SessionHandle sessionHandle;
    char                testMessage[256];

    /* Debug Message: */
    System_printf ("---------------------------------------------------\n");
    System_printf ("Debug: Testing Invalid Data Size Failures\n");
    System_printf ("---------------------------------------------------\n");

    /* Initialize the configuration: */
    memset ((void*)&initCfg, 0, sizeof(CBUFF_InitCfg));

    /* Populate the configuration: */
    initCfg.socHandle                = socHandle;
    initCfg.outputDataFmt            = CBUFF_OutputDataFmt_16bit;
    initCfg.enableECC                = 1U;
    initCfg.crcEnable                = 1U;
    initCfg.enableDebugMode          = false;
    initCfg.maxSessions              = 2U;
    initCfg.interface                = CBUFF_Interface_LVDS;
    initCfg.u.lvdsCfg.crcEnable      = 0U;
    initCfg.u.lvdsCfg.msbFirst       = 1U;
    initCfg.u.lvdsCfg.ddrClockMode   = 1U;
    initCfg.u.lvdsCfg.ddrClockModeMux= 1U;

    /* Setup the lanes: This configuration is platform specific */
#ifdef SOC_XWR14XX
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0xFU;
#else
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0x3U;
#endif

    /* Initialize the CBUFF Driver: */
    cbuffHandle = CBUFF_init (&initCfg, &errCode);
    if (cbuffHandle == NULL)
    {
        System_printf ("Error: CBUFF Driver initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Initialize the configuration */
    memset ((void*)&sessionCfg, 0, sizeof(CBUFF_SessionCfg));

    /* Create the MCPI Test Message: */
    sprintf (testMessage, "CBUFF: Invalid Minimum Data Size Test");

    /* Populate the configuration: */
    sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_SW;
    sessionCfg.edmaHandle                         = edmaHandle;
    sessionCfg.allocateEDMAChannelFxn             = Test_EDMAAllocateCBUFFChannel;
    sessionCfg.freeEDMAChannelFxn                 = Test_EDMAFreeCBUFFChannel;
    sessionCfg.dataType                           = CBUFF_DataType_COMPLEX;
    sessionCfg.u.swCfg.userBufferInfo[0].size     = 2U;
    sessionCfg.u.swCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];
    sessionCfg.u.swCfg.userBufferInfo[1].size     = 2U;
    sessionCfg.u.swCfg.userBufferInfo[1].address  = (uint32_t)&gUserBuffer1[0];

    /* Open the CBUFF Driver: */
    sessionHandle = CBUFF_createSession (cbuffHandle, &sessionCfg, &errCode);
    if (sessionHandle != NULL)
    {
        MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
        return -1;
    }

    /* Open Failed: Validate the error code */
    if (errCode != CBUFF_ELIMIT)
    {
        System_printf ("Error: CBUFF open failed with an invalid error code %d\n", errCode);
        MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
    }
    else
    {
        /* Test Passed with a valid error code */
        MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_PASS);
    }

    /* Initialize the configuration */
    memset ((void*)&sessionCfg, 0, sizeof(CBUFF_SessionCfg));

    /* Create the MCPI Test Message: */
    sprintf (testMessage, "CBUFF: Invalid Maximum Data Size Test");

    /* Populate the configuration: */
    sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_SW;
    sessionCfg.edmaHandle                         = edmaHandle;
    sessionCfg.allocateEDMAChannelFxn             = Test_EDMAAllocateCBUFFChannel;
    sessionCfg.freeEDMAChannelFxn                 = Test_EDMAFreeCBUFFChannel;
    sessionCfg.dataType                           = CBUFF_DataType_COMPLEX;
    sessionCfg.u.swCfg.userBufferInfo[0].size     = 0x4000;
    sessionCfg.u.swCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];

    /* Open the CBUFF Driver: */
    sessionHandle = CBUFF_createSession (cbuffHandle, &sessionCfg, &errCode);
    if (sessionHandle != NULL)
    {
        MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
        return -1;
    }

    /* Open Failed: Validate the error code */
    if (errCode != CBUFF_ELIMIT)
    {
        System_printf ("Error: CBUFF open failed with an invalid error code %d\n", errCode);
        MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
    }
    else
    {
        /* Test Passed with a valid error code */
        MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_PASS);
    }

    /* Deinitialize and shutdown the CBUFF: */
    if (CBUFF_deinit (cbuffHandle, &errCode) < 0)
    {
        System_printf ("Debug: CBUFF Driver deinit %s failed [Error code %d]\n", errCode);
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test multiple CBUFF Sessions. The test
 *      will activate and deactivate all the sessions to ensure that the
 *      CBUFF API work as expected for the defined limits. This is only
 *      for the CBUFF API Testing.
 *
 *  @param[in]  socHandle
 *      SOC Driver Handle
 *  @param[in]  edmaHandle
 *      EDMA Driver Handle
 *  @param[in]  adcBufHandle
 *      ADCBUF Driver Handle
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -  <0
 */
int32_t Test_MultipleSessions
(
    SOC_Handle      socHandle,
    EDMA_Handle     edmaHandle,
    ADCBuf_Handle   adcBufHandle
)
{
    CBUFF_SessionCfg        sessionCfg;
    CBUFF_Handle            cbuffHandle;
    uint8_t                 index;
    CBUFF_InitCfg           initCfg;
    int32_t                 errCode;
    CBUFF_SessionHandle     tmpSessionHandle;
    CBUFF_SessionHandle     sessionHandle[CBUFF_MAX_NUM_SESSION];
    char                    testMessage[256];

    /* Debug Message: */
    System_printf ("---------------------------------------------------\n");
    System_printf ("Debug: Testing Multiple Sessions\n");
    System_printf ("---------------------------------------------------\n");

    /* Create the MCPI Test Message: */
    sprintf (testMessage, "CBUFF: Multiple Session");

    /* Initialize the configuration: */
    memset ((void*)&initCfg, 0, sizeof(CBUFF_InitCfg));

    /* Populate the configuration: */
    initCfg.socHandle                = socHandle;
    initCfg.outputDataFmt            = CBUFF_OutputDataFmt_16bit;
    initCfg.enableECC                = 1U;
    initCfg.crcEnable                = 1U;
    initCfg.enableDebugMode          = false;
    initCfg.maxSessions              = CBUFF_MAX_NUM_SESSION;
    initCfg.interface                = CBUFF_Interface_LVDS;
    initCfg.u.lvdsCfg.crcEnable      = 0U;
    initCfg.u.lvdsCfg.msbFirst       = 1U;
    initCfg.u.lvdsCfg.ddrClockMode   = 1U;
    initCfg.u.lvdsCfg.ddrClockModeMux= 1U;

    /* Setup the lanes: This configuration is platform specific */
#ifdef SOC_XWR14XX
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0xFU;
#else
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0x3U;
#endif

    /* Initialize the CBUFF Driver: */
    cbuffHandle = CBUFF_init (&initCfg, &errCode);
    if (cbuffHandle == NULL)
    {
        System_printf ("Error: CBUFF Driver initialization failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
        return -1;
    }

    /* Enable all the ADC Channels for this test */
    Test_enableADCChannel (adcBufHandle, SYS_COMMON_NUM_RX_CHANNEL);

    /* Cycle through and create all the sessions. */
    for (index = 0U; index < CBUFF_MAX_NUM_SESSION; index++)
    {
        /* Initialize the configuration */
        memset ((void*)&sessionCfg, 0, sizeof(CBUFF_SessionCfg));

        /* Populate the configuration: */
        sessionCfg.edmaHandle                 = edmaHandle;
        sessionCfg.allocateEDMAChannelFxn     = Test_EDMAAllocateCBUFFChannel;
        sessionCfg.freeEDMAChannelFxn         = Test_EDMAFreeCBUFFChannel;
		sessionCfg.dataType                   = CBUFF_DataType_COMPLEX;
        sessionCfg.header.size                = 0U;
        sessionCfg.header.address             = 0U;

        /* The first session is a hardware session; while the other sessions are
         * software triggered sessions. */
        if (index == 0U)
        {
            /* Hardware Triggered Session: */
            sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_HW;
            sessionCfg.u.hwCfg.numChirpsPerFrame          = 0x20U;
            sessionCfg.u.hwCfg.chirpMode                  = 0x1U;
    		sessionCfg.u.hwCfg.dataMode                   = CBUFF_DataMode_NON_INTERLEAVED;
	    	sessionCfg.u.hwCfg.dataFormat                 = CBUFF_DataFmt_ADC_DATA;
    		sessionCfg.u.hwCfg.opMode                     = CBUFF_OperationalMode_CHIRP;
	    	sessionCfg.u.hwCfg.adcBufHandle               = adcBufHandle;
    		sessionCfg.u.hwCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];
	    	sessionCfg.u.hwCfg.userBufferInfo[0].size     = sizeof(gUserBuffer0);
		    sessionCfg.u.hwCfg.userBufferInfo[1].address  = (uint32_t)&gUserBuffer1[0];
    		sessionCfg.u.hwCfg.userBufferInfo[1].size     = sizeof(gUserBuffer1);
	    	sessionCfg.u.hwCfg.userBufferInfo[2].address  = (uint32_t)&gUserBuffer2[0];
    		sessionCfg.u.hwCfg.userBufferInfo[2].size     = sizeof(gUserBuffer2);
	    	sessionCfg.u.hwCfg.numADCSamples              = 256U;
            sessionCfg.u.hwCfg.cqSize[0]                  = 0U;
            sessionCfg.u.hwCfg.cqSize[1]                  = 128U;
            sessionCfg.u.hwCfg.cqSize[2]                  = 128U;
        }
        else
        {
            /* Software Triggered Session: */
            sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_SW;
    		sessionCfg.u.swCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];
	    	sessionCfg.u.swCfg.userBufferInfo[0].size     = sizeof(gUserBuffer0);
		    sessionCfg.u.swCfg.userBufferInfo[1].address  = (uint32_t)&gUserBuffer1[0];
    		sessionCfg.u.swCfg.userBufferInfo[1].size     = sizeof(gUserBuffer1);
	    	sessionCfg.u.swCfg.userBufferInfo[2].address  = (uint32_t)&gUserBuffer2[0];
    		sessionCfg.u.swCfg.userBufferInfo[2].size     = sizeof(gUserBuffer2);
        }

        /* Open the CBUFF Driver: */
        sessionHandle[index] = CBUFF_createSession (cbuffHandle, &sessionCfg, &errCode);
        if (sessionHandle[index] == NULL)
        {
            System_printf ("Error: Unable to create the session %d [Error code %d]\n",
                           index, errCode);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }
        System_printf ("Debug: Session %d created successfully\n", index);
    }

    /* Cycle through and activate and deactivate sessions */
    for (index = 0U; index < CBUFF_MAX_NUM_SESSION; index++)
    {
        /* Activate the current session: */
        if (CBUFF_activateSession (sessionHandle[index], &errCode) < 0)
        {
            System_printf ("Error: Unable to activate the session [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Get the current active session from the CBUFF Driver: */
        if (CBUFF_control (cbuffHandle,
                           CBUFF_Command_GET_ACTIVE_SESSION,
                           (void*)&tmpSessionHandle,
                           sizeof(CBUFF_SessionHandle),
                           &errCode) < 0)
        {
            System_printf ("Error: Unable to get the active session [Error: %d]\n", errCode);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Sanity Check: Ensure that the CBUFF Driver gives us the correct session. */
        if (tmpSessionHandle != sessionHandle[index])
        {
            System_printf ("Error: Mismatch in the active session. Expected: %x Got: %x\n",
                            sessionHandle[index], tmpSessionHandle);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Deactivate the session: */
        if (CBUFF_deactivateSession (sessionHandle[index], &errCode) < 0)
        {
            System_printf ("Error: Unable to deactivate the session [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Get the current active session from the CBUFF Driver: */
        if (CBUFF_control (cbuffHandle,
                           CBUFF_Command_GET_ACTIVE_SESSION,
                           (void*)&tmpSessionHandle,
                           sizeof(CBUFF_SessionHandle),
                           &errCode) < 0)
        {
            System_printf ("Error: Unable to get the active session [Error: %d]\n", errCode);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Sanity Check: There is no active session so we should get NULL here. */
        if (tmpSessionHandle != NULL)
        {
            System_printf ("Error: Mismatch in the active session. Expected: NULL Got: %x\n",
                           tmpSessionHandle);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Debug Message: */
        System_printf ("Debug: Session %d has been activated and deactivated successfully\n", index);
    }

    /* Cycle through and delete all the sessions: */
    for (index = 0U; index < CBUFF_MAX_NUM_SESSION; index++)
    {
        if (CBUFF_deleteSession (sessionHandle[index], &errCode) < 0)
        {
            System_printf ("Error: Unable to delete the session [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
            return -1;
        }
        System_printf ("Debug: Session %d has been deleted successfully\n", index);
    }

    /* Deinitialize and shutdown the CBUFF: */
    if (CBUFF_deinit (cbuffHandle, &errCode) < 0)
    {
        System_printf ("Debug: CBUFF Driver deinit %s failed [Error code %d]\n", errCode);
        return -1;
    }
    MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_PASS);
    return 0;
}

#if defined(SOC_XWR16XX) || defined (SOC_XWR18XX) || defined (SOC_XWR68XX)

/**
 *  @b Description
 *  @n
 *      The function is used to test the session creation for Multiple Chirp
 *      Mode. This is only applicable for the XWR16xx/XWR18xx/XWR68xx
 *
 *  @param[in]  socHandle
 *      SOC Driver Handle
 *  @param[in]  edmaHandle
 *      EDMA Driver Handle
 *  @param[in]  adcBufHandle
 *      ADCBUF Driver Handle
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -  <0
 */
int32_t Test_MultipleChirpMode
(
    SOC_Handle      socHandle,
    EDMA_Handle     edmaHandle,
    ADCBuf_Handle   adcBufHandle
)
{
    CBUFF_SessionCfg    sessionCfg;
    CBUFF_Handle        cbuffHandle;
    CBUFF_DataFmt       dataFormat;
    int32_t             errCode;
    char                testMessage[256];
    CBUFF_InitCfg       initCfg;
    CBUFF_SessionHandle sessionHandle;
    uint8_t             numChannel;
    uint32_t            chirpMode;

    /* Debug Message: */
    System_printf ("---------------------------------------------------\n");
    System_printf ("Debug: Testing Multiple Chirp Mode\n");
    System_printf ("---------------------------------------------------\n");

    /* Reset the EDMA Allocation Failure Flag */
    gTestEDMAChannelAllocationFailure = 0U;

    /* Initialize the configuration: */
    memset ((void*)&initCfg, 0, sizeof(CBUFF_InitCfg));

    /* Populate the configuration: */
    initCfg.socHandle                = socHandle;
    initCfg.outputDataFmt            = CBUFF_OutputDataFmt_16bit;
    initCfg.enableECC                = 1U;
    initCfg.crcEnable                = 1U;
    initCfg.enableDebugMode          = false;
    initCfg.maxSessions              = 2U;
    initCfg.interface                = CBUFF_Interface_LVDS;
    initCfg.u.lvdsCfg.crcEnable      = 0U;
    initCfg.u.lvdsCfg.msbFirst       = 1U;
    initCfg.u.lvdsCfg.ddrClockMode   = 1U;
    initCfg.u.lvdsCfg.ddrClockModeMux= 1U;
    initCfg.u.lvdsCfg.lvdsLaneEnable = 0x3U;

    /* Initialize the CBUFF Driver: */
    cbuffHandle = CBUFF_init (&initCfg, &errCode);
    if (cbuffHandle == NULL)
    {
        System_printf ("Error: CBUFF Driver initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Cycle through all the ADC Channels */
    for (numChannel = 1U; numChannel <= SYS_COMMON_NUM_RX_CHANNEL; numChannel++)
    {
        /* Enable the ADC Channel: */
        Test_enableADCChannel (adcBufHandle, numChannel);

        /* Cycle through all the data formats: */
        for (dataFormat = CBUFF_DataFmt_ADC_DATA; dataFormat < CBUFF_DataFmt_MAX; dataFormat++)
        {
            /* Cycle through all the Multiple Chirp Modes */
            for (chirpMode = 1U; chirpMode < 9U; chirpMode++)
            {
                /* Create the MCPI Test Message: */
                sprintf (testMessage, "CBUFF ADC Channel: %d Multiple Chirp: %d Data Format: %s",
                         numChannel, chirpMode, gCBUFFDataFormatString[dataFormat]);

                /* Initialize the configuration */
                memset ((void*)&sessionCfg, 0, sizeof(CBUFF_SessionCfg));

                /* Populate the configuration: */
                sessionCfg.executionMode                      = CBUFF_SessionExecuteMode_HW;
                sessionCfg.edmaHandle                         = edmaHandle;
                sessionCfg.allocateEDMAChannelFxn             = Test_EDMAAllocateCBUFFChannel;
                sessionCfg.freeEDMAChannelFxn                 = Test_EDMAFreeCBUFFChannel;
                sessionCfg.dataType                           = CBUFF_DataType_COMPLEX;
                sessionCfg.header.size                        = 0U;
                sessionCfg.header.address                     = 0U;
                sessionCfg.u.hwCfg.dataMode                   = CBUFF_DataMode_NON_INTERLEAVED;
                sessionCfg.u.hwCfg.dataFormat                 = dataFormat;
                sessionCfg.u.hwCfg.opMode                     = CBUFF_OperationalMode_CHIRP;
                sessionCfg.u.hwCfg.numChirpsPerFrame          = 0x20U;
                sessionCfg.u.hwCfg.chirpMode                  = chirpMode;
                sessionCfg.u.hwCfg.numADCSamples              = 256U;
                sessionCfg.u.hwCfg.adcBufHandle               = adcBufHandle;
                sessionCfg.u.hwCfg.cqSize[0]                  = 0U;
                sessionCfg.u.hwCfg.cqSize[1]                  = 128U;
                sessionCfg.u.hwCfg.cqSize[2]                  = 128U;
                sessionCfg.u.hwCfg.userBufferInfo[0].address  = (uint32_t)&gUserBuffer0[0];
                sessionCfg.u.hwCfg.userBufferInfo[0].size     = sizeof(gUserBuffer0);
                sessionCfg.u.hwCfg.userBufferInfo[1].address  = (uint32_t)&gUserBuffer1[0];
                sessionCfg.u.hwCfg.userBufferInfo[1].size     = sizeof(gUserBuffer1);
                sessionCfg.u.hwCfg.userBufferInfo[2].address  = (uint32_t)&gUserBuffer2[0];
                sessionCfg.u.hwCfg.userBufferInfo[2].size     = sizeof(gUserBuffer2);

                /* Create the HW Session: */
                sessionHandle = CBUFF_createSession (cbuffHandle, &sessionCfg, &errCode);
                if (sessionHandle == NULL)
                {
                    System_printf ("Error: Unable to create the session [Error code %d]\n", errCode);
                    MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                    return -1;
                }

                /* Debug Message: Display the EDMA Channel Usage for the test. */
                System_printf ("Debug: EDMA Channel Usage = %d\n", gCBUFFEDMAChannelResourceCounter);

                /* Activate the session: */
                if (CBUFF_activateSession (sessionHandle, &errCode) < 0)
                {
                    System_printf ("Error: Unable to activate the session [Error code %d]\n", errCode);
                    MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                    return -1;
                }

                /* Deactivate the session: */
                if (CBUFF_deactivateSession (sessionHandle, &errCode) < 0)
                {
                    System_printf ("Error: Unable to deactivate the session [Error code %d]\n", errCode);
                    MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                    return -1;
                }

                /* Delete the session: */
                if (CBUFF_deleteSession (sessionHandle, &errCode) < 0)
                {
                    System_printf ("Error: Unable to delete the session [Error code %d]\n", errCode);
                    MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_FAIL);
                    return -1;
                }

                /* Sanity Check: Ensure that all the EDMA resources have been cleaned up */
                if (gCBUFFEDMAChannelResourceCounter != 0)
                {
                    System_printf ("Error: EDMA Channel Memory leak detected\n");
                    return -1;
                }
                MCPI_setFeatureTestResult (testMessage, MCPI_TestResult_PASS);
            }
        }
    }

    /* Deinitialize and shutdown the CBUFF: */
    if (CBUFF_deinit (cbuffHandle, &errCode) < 0)
    {
        System_printf ("Debug: CBUFF Driver deinit %s failed [Error code %d]\n", errCode);
        return -1;
    }
    return 0;
}


#endif


