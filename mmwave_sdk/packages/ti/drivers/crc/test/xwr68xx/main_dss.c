/*
 *   @file  main_dss.c
 *
 *   @brief
 *      Unit Test code for the CRC on the XWR68xx DSS
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
#include <c6x.h>

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
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>
#include <ti/sysbios/family/c64p/Cache.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/crc/crc.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**************************************************************************
 *************************** Extern Functions *****************************
 **************************************************************************/

extern int32_t Test_crcInitAPI(void);
extern int32_t Test_crcComputeSignatureAPI(void);
extern int32_t Test_crcGetSignatureAPI(void);
extern int32_t Test_crcCancelSignatureAPI(void);
extern int32_t Test_crcAPI(CRC_Type type, CRC_DataLen dataLen);

/**************************************************************************
 *************************** CRC Test Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to setup the event combiner.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_setupEventCombiner (void)
{
    Hwi_Params  params;
    uint32_t    i;

    Hwi_Params_init(&params);
    params.enableInt = TRUE;
    for (i = 0; i < 4; i++)
    {
        params.arg      = i;
        params.eventId  = i;
        if (Hwi_create(4 + i, &EventCombiner_dispatch, &params, NULL) == NULL)
        {
            System_printf("failed to create Hwi interrupt %d\n",4 + i);
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system and executes the test.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    /* Debug Message: */
    System_printf ("***********************************************\n");
    System_printf ("************** CRC Unit Tests *****************\n");
    System_printf ("***********************************************\n");

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize & setup the Event combiner: */
    Test_setupEventCombiner();

    /* Test the Basic CRC API: */
    if (Test_crcInitAPI() < 0)
    {
        MCPI_setFeatureTestResult ("Basic CRC API", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("Basic CRC API", MCPI_TestResult_PASS);

    /* Test the Compute CRC API: */
    if (Test_crcComputeSignatureAPI() < 0)
    {
        MCPI_setFeatureTestResult ("Compute CRC Signature", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("Compute CRC Signature", MCPI_TestResult_PASS);

    /* Test the Get CRC Signature API: */
    if (Test_crcGetSignatureAPI() < 0)
    {
        MCPI_setFeatureTestResult ("Get CRC Signature", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("Get CRC Signature", MCPI_TestResult_PASS);

    /* Test the Cancel CRC API: */
    if (Test_crcCancelSignatureAPI() < 0)
    {
        MCPI_setFeatureTestResult ("Cancel CRC Signature", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("Cancel CRC Signature", MCPI_TestResult_PASS);

    /*************************************************************************
     * CRC Signature for CRC Type: 16bit
     *************************************************************************/
    if (Test_crcAPI(CRC_Type_16BIT, CRC_DataLen_16_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 16bit Data Length: 16bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 16bit Data Length: 16bit", MCPI_TestResult_PASS);

    if (Test_crcAPI(CRC_Type_16BIT, CRC_DataLen_32_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 16bit Data Length: 32bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 16bit Data Length: 32bit", MCPI_TestResult_PASS);

    if (Test_crcAPI(CRC_Type_16BIT, CRC_DataLen_64_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 16bit Data Length: 64bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 16bit Data Length: 64bit", MCPI_TestResult_PASS);

    /*************************************************************************
     * CRC Signature for CRC Type: 32bit
     *************************************************************************/
    if (Test_crcAPI(CRC_Type_32BIT, CRC_DataLen_16_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 32bit Data Length: 16bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 32bit Data Length: 16bit", MCPI_TestResult_PASS);

    if (Test_crcAPI(CRC_Type_32BIT, CRC_DataLen_32_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 32bit Data Length: 32bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 32bit Data Length: 32bit", MCPI_TestResult_PASS);

    if (Test_crcAPI(CRC_Type_32BIT, CRC_DataLen_64_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 32bit Data Length: 64bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 32bit Data Length: 64bit", MCPI_TestResult_PASS);

    /*************************************************************************
     * CRC Signature for CRC Type: 64bit
     *************************************************************************/
    if (Test_crcAPI(CRC_Type_64BIT, CRC_DataLen_16_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 64bit Data Length: 16bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 64bit Data Length: 16bit", MCPI_TestResult_PASS);

    if (Test_crcAPI(CRC_Type_64BIT, CRC_DataLen_32_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 64bit Data Length: 32bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 64bit Data Length: 32bit", MCPI_TestResult_PASS);

    if (Test_crcAPI(CRC_Type_64BIT, CRC_DataLen_64_BIT) < 0)
    {
        MCPI_setFeatureTestResult ("CRC Type: 64bit Data Length: 64bit", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("CRC Type: 64bit Data Length: 64bit", MCPI_TestResult_PASS);

    /* All the testing was successful: */
    System_printf ("Debug: CRC Unit Tests completed successfully\n");

    MCPI_setTestResult ();

    BIOS_exit(0);
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

    /* Start the Timestamp counter */
    TSCL = 0;

    /* Enable the caches. */
    Cache_enable(Cache_Type_L1P);
    Cache_enable(Cache_Type_L1D);

    /* Debug Message: */
    System_printf ("******************************************\n");
    System_printf ("Debug: Launching the CRC Test Application\n");
    System_printf ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Debug Message: */
    System_printf ("Debug: Launching BIOS\n");

    /* Start BIOS */
	BIOS_start();
    return 0;
}

