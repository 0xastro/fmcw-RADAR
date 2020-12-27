/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the CBUFF Driver which executes on the MSS
 *      The testing is only done for the CBUFF API. For functionality
 *      testing please refer to the streaming tests or the demo.
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
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>

/* mmWaveSDK Include Files */
#include <ti/common/sys_common.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 **************************** Local Definitions ***************************
 **************************************************************************/

/**
 * @brief   This is the number of EDMA Channels which have been allocated and can be used
 * by the CBUFF Driver.
 */
#define TEST_APP_MAX_EDMA_TABLE_ENTRIES         27

/**************************************************************************
 **************************** Global Variables ****************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(32768);

/**
 * @brief   Global CBUFF EDMA Channel Resource Table:
 */
static CBUFF_EDMAChannelCfg gCBUFFEDMAChannelResource [TEST_APP_MAX_EDMA_TABLE_ENTRIES] =
{
    /* EDMA Channel Identifier, Shadow Link Channel Identifier */
    {  EDMA_TPCC0_REQ_CBUFF_0,  65 },
    {  EDMA_TPCC0_REQ_FREE_0,   66 },
    {  EDMA_TPCC0_REQ_FREE_1,   67 },
    {  EDMA_TPCC0_REQ_FREE_2,   68 },
    {  EDMA_TPCC0_REQ_FREE_3,   69 },
    {  EDMA_TPCC0_REQ_FREE_4,   70 },
    {  EDMA_TPCC0_REQ_FREE_5,   71 },
    {  EDMA_TPCC0_REQ_FREE_6,   72 },
    {  EDMA_TPCC0_REQ_FREE_7,   73 },
    {  EDMA_TPCC0_REQ_FREE_8,   74 },
    {  EDMA_TPCC0_REQ_FREE_9,   75 },
    {  EDMA_TPCC0_REQ_FREE_10,  76 },
    {  EDMA_TPCC0_REQ_FREE_11,  77 },
    {  EDMA_TPCC0_REQ_FREE_12,  78 },
    {  EDMA_TPCC0_REQ_FREE_13,  79 },
    {  EDMA_TPCC0_REQ_FREE_14,  80 },
    {  EDMA_TPCC0_REQ_FREE_15,  81 },
    {  EDMA_TPCC0_REQ_FREE_16,  82 },
    {  EDMA_TPCC0_REQ_FREE_17,  82 },
    {  EDMA_TPCC0_REQ_FREE_18,  83 },
    {  EDMA_TPCC0_REQ_FREE_19,  84 },
    {  EDMA_TPCC0_REQ_FREE_20,  85 },
    {  EDMA_TPCC0_REQ_FREE_21,  86 },
    {  EDMA_TPCC0_REQ_FREE_22,  87 },
    {  EDMA_TPCC0_REQ_FREE_23,  88 },
    {  EDMA_TPCC0_REQ_FREE_24,  89 },
    {  EDMA_TPCC0_REQ_FREE_25,  90 }
};

/**
 * @brief   This is a global variable which tracks the EDMA Channels which have been allocated to the
 * CBUFF.
 */
uint8_t gCBUFFEDMAChannelResourceCounter = 0U;

/**
 * @brief   Global Variable which is used to test the failure of EDMA Channel allocation
 */
uint8_t gTestEDMAChannelAllocationFailure = 0U;

/**************************************************************************
 **************************** Extern Functions ****************************
 **************************************************************************/
extern int32_t Test_dataFormats
(
    SOC_Handle        socHandle,
    EDMA_Handle       edmaHandle,
    ADCBuf_Handle     adcBufHandle,
    CBUFF_DataMode    dataMode
);
extern int32_t Test_EDMAChannelAllocationFailures
(
    SOC_Handle        socHandle,
    EDMA_Handle       edmaHandle,
    ADCBuf_Handle     adcBufHandle,
    CBUFF_DataMode    dataMode
);
extern int32_t Test_MultipleSessions
(
    SOC_Handle        socHandle,
    EDMA_Handle       edmaHandle,
    ADCBuf_Handle     adcBufHandle
);
extern int32_t Test_InvalidDataSize
(
    SOC_Handle      socHandle,
    EDMA_Handle     edmaHandle
);

/**************************************************************************
 *************************** CBUFF Test Functions *************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the registered allocation function which is hooked up with the
 *      CBUFF driver to allocate EDMA Channels
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t Test_EDMAAllocateCBUFFChannel (CBUFF_EDMAInfo* ptrEDMAInfo, CBUFF_EDMAChannelCfg* ptrEDMAChannelCfg)
{
    /* Are we testing the EDMA allocation failure mode? */
    if (gTestEDMAChannelAllocationFailure == 0U)
    {
        /* NO: Have we allocated all the EDMA channels? */
        if (gCBUFFEDMAChannelResourceCounter >= TEST_APP_MAX_EDMA_TABLE_ENTRIES)
        {
            /* Error: Exceeded the allocated table. Failure */
            DebugP_assert (0);
            return -1;
        }

        /* Special case handling: First EDMA channel which is being allocated */
        if (ptrEDMAInfo->isFirstEDMAChannel == true)
        {
            switch (ptrEDMAInfo->dmaNum)
            {
                case 0:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_0;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 65;
                    break;
                }
                case 1:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_1;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 100;
                    break;
                }
                case 2:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_2;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 101;
                    break;
                }
                case 3:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_3;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 102;
                    break;
                }
                case 4:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_4;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 103;
                    break;
                }
                case 5:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_5;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 104;
                    break;
                }
                case 6:
                {
                    ptrEDMAChannelCfg->chainChannelsId      = EDMA_TPCC0_REQ_CBUFF_6;
                    ptrEDMAChannelCfg->shadowLinkChannelsId = 105;
                    break;
                }
                default:
                {
                    DebugP_assert (0);
                    break;
                }
            }
        }
        else
        {
            /* Copy over the allocated EDMA configuration. */
            memcpy ((void *)ptrEDMAChannelCfg, (void*)&gCBUFFEDMAChannelResource[gCBUFFEDMAChannelResourceCounter],
                    sizeof(CBUFF_EDMAChannelCfg));
        }

        /* Increment the number of EDMA Channels which have been allocated */
        gCBUFFEDMAChannelResourceCounter++;
        return 0;
    }
    else
    {
        /* YES: Return a failure to indicate that the channel allocation failed. We use this
         * to ensure that the CBUFF driver correctly reports errors. */
        return -1;
    }
}

/**
 *  @b Description
 *  @n
 *      This is the registered free function which is hooked up with the
 *      CBUFF driver to free the allocated EDMA Channels
 *
 *  @retval
 *      Not applicable
 */
void Test_EDMAFreeCBUFFChannel(CBUFF_EDMAChannelCfg* ptrEDMACfg)
{
    /* Debug Message: */
    gCBUFFEDMAChannelResourceCounter--;
    return;
}

/**
 *  @b Description
 *  @n
 *      Registered callback function with the EDMA to be invoked on an error
 *
 *  @retval
 *      Not Applicable.
 */
void Test_edmaErrorCallbackFxn(EDMA_Handle handle, EDMA_errorInfo_t *errorInfo)
{
    DebugP_assert(0);
}

/**
 *  @b Description
 *  @n
 *      Registered callback function with the EDMA to be invoked on an error
 *
 *  @retval
 *      Not Applicable.
 */
void Test_edmaTransferControllerErrorCallbackFxn
(
    EDMA_Handle                         handle,
    EDMA_transferControllerErrorInfo_t* errorInfo
)
{
    DebugP_assert(0);
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
    SOC_Handle              socHandle;
    int32_t                 errCode;
    EDMA_Handle             edmaHandle;
    EDMA_errorConfig_t      errorConfig;
    EDMA_instanceInfo_t     instanceInfo;
    ADCBuf_Params           params;
    ADCBuf_Handle           adcBufHandle;

    /* Get the SOC Handle: */
    socHandle = (SOC_Handle)arg0;

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize the EDMA: */
    errCode = EDMA_init(0);
    if (errCode != EDMA_NO_ERROR)
    {
        System_printf("Error: Unable to initialize EDMA, errorCode = %d\n", errCode);
        return;
    }
    System_printf ("Debug: EDMA instance 0 has been initialized\n");

    /* Open the first edma Instance */
    edmaHandle = EDMA_open(0, &errCode, &instanceInfo);
    if (edmaHandle == NULL)
    {
        System_printf("Error: Unable to open the edma Instance, erorCode = %d\n", errCode);
        return;
    }

    /* Setup the EDMA Error Monitoring: */
    errorConfig.isConfigAllEventQueues              = true;
    errorConfig.isConfigAllTransferControllers      = true;
    errorConfig.isEventQueueThresholdingEnabled     = true;
    errorConfig.eventQueueThreshold                 = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
    errorConfig.isEnableAllTransferControllerErrors = true;
    errorConfig.callbackFxn                         = Test_edmaErrorCallbackFxn;
    errorConfig.transferControllerCallbackFxn       = Test_edmaTransferControllerErrorCallbackFxn;
    errCode = EDMA_configErrorMonitoring(edmaHandle, &errorConfig);
    if (errCode != EDMA_NO_ERROR)
    {
        System_printf("Debug: EDMA_configErrorMonitoring() failed with errorCode = %d\n", errCode);
        return;
    }

    /***************************************************************************************
     * ADCBUF Initializations:
     ***************************************************************************************/
    ADCBuf_init ();

    /* Open the ADC Driver: */
    ADCBuf_Params_init(&params);

    /* Setup the chirp thresholds: */
    params.chirpThreshold = 1U;
    params.continousMode  = 0U;

    /* Open the ADC Driver: */
    adcBufHandle = ADCBuf_open(0, &params);
    if (adcBufHandle == NULL)
    {
        System_printf ("Error: Unable to open the ADC Driver\n");
        return;
    }

    /* Test: Invalid Data Size */
    if (Test_InvalidDataSize (socHandle, edmaHandle) < 0)
        return;

    /* Test the Interleaved Mode: */
    if (Test_dataFormats (socHandle, edmaHandle, adcBufHandle, CBUFF_DataMode_INTERLEAVED) < 0)
        return;

    /* Test the Non-Interleaved Mode: */
    if (Test_dataFormats (socHandle, edmaHandle, adcBufHandle, CBUFF_DataMode_NON_INTERLEAVED) < 0)
        return;

    /* Test Multiple Sessions: */
    if (Test_MultipleSessions (socHandle, edmaHandle, adcBufHandle) < 0)
        return;

    /* Test: EDMA Memory allocation failures in Interleaved Mode:*/
    if (Test_EDMAChannelAllocationFailures (socHandle, edmaHandle, adcBufHandle, CBUFF_DataMode_INTERLEAVED) < 0)
        return;

    /* Test: EDMA Memory allocation failures in Non-Interleaved Mode */
    if (Test_EDMAChannelAllocationFailures (socHandle, edmaHandle, adcBufHandle, CBUFF_DataMode_NON_INTERLEAVED) < 0)
        return;

    /* Shutdown and deinitialize the SOC Module */
    if (SOC_deinit (socHandle, &errCode) < 0)
    {
        System_printf ("Error: CBUFF Driver SOC Deinitialization %s failed [Error code %d]\n", errCode);
    }

    /* Set the MCPI result for Test Automation */
    MCPI_setTestResult ();

    /* Unit Testing is complete */
    System_printf ("Debug: CBUFF Unit Tests completed\n");

    /* exit BIOS */
    BIOS_exit(0);
    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point to the unit tests
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -   <0
 */
int main (void)
{
	Task_Params     taskParams;
    SOC_Handle      socHandle;
    int32_t         errCode;
    SOC_Cfg         socCfg;

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

    /* Debug Message: */
    System_printf ("***********************************************\n");
    System_printf ("************** CBUFF Unit Tests ***************\n");
    System_printf ("***********************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    taskParams.arg0      = (UArg)socHandle;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
	return 0;
}

