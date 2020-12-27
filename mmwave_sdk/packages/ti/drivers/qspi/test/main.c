/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the QSPI Driver
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
#include <ti/sysbios/family/arm/v7a/Pmu.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/qspi/qspi.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/esm/esm.h>

/* TOP RCM register file */
#include <ti/drivers/soc/include/reg_toprcm.h>
#include <ti/drivers/soc/soc.h>

/* MCPI logger include files */
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
 *************************** QSPI Test Functions *******************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *   @param[in] arg0               Task Arg0
 *   @param[in] arg1               Task Arg1
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    QSPI_Params     params;
    QSPI_Handle     handle;
    int32_t         errCode = 0;
    int32_t         retVal = 0;
    uint8_t         tempData[QSPI_MAX_NUM_WORDS + 10U];
    SOC_Handle      socHandle;

    /**************************************************************************
     * Test: MCPI_Initialize
     **************************************************************************/
    /* Initialize MCPI logger framework */
    MCPI_Initialize ();

    /* Initialize the QSPI */
    QSPI_init();

    /**************************************************************************
     * Test: QSPI peripheral clock setup through SOC driver
     **************************************************************************/

    socHandle = (SOC_Handle)arg0;

    /* Set QSPI peripheral clock to 200MHz */
    retVal = SOC_setPeripheralClock(socHandle, SOC_MODULE_QSPI, SOC_CLKSOURCE_VCLK, 0U, &errCode);
    if(retVal < 0)
    {
        MCPI_setFeatureTestResult ("QSPI set peripheral clock through SOC ", MCPI_TestResult_FAIL);
        System_printf("SOC driver clock configuration for QSPI with error : %d\n", errCode);
        goto Exit;
    }

    /**************************************************************************
     * Test: Verify QSPI Open API
     **************************************************************************/
    /* Setup the default QSPI Parameters */
    QSPI_Params_init(&params);

    /* QSPI clock is set to 200MHz, pass it to QSPI driver */
    params.qspiClk = 200 * 1000000U;

    /* Clock rate to external SPI device */
    params.bitRate = 40 * 1000000U;

    /* Open the QSPI Instance for MibSpiA */
    handle = QSPI_open(&params, &errCode);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ("QSPI Open API ", MCPI_TestResult_FAIL);
        System_printf("QSPI open with error : %d\n", errCode);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPI Open API ", MCPI_TestResult_PASS);

        /* Graceful shutdown */
        QSPI_close(handle);
        System_printf("Debug: QSPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify QSPI Open API param check:: bitRate
     **************************************************************************/
    /* Setup the default QSPI Parameters */
    QSPI_Params_init(&params);

    params.bitRate = 0U;

    /* Open the QSPI Instance for MibSpiA */
    handle = QSPI_open(&params, &errCode);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ("QSPI Open API with zero QSPI clock rate", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPI Open API with zero QSPI clock rate ", MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        QSPI_close(handle);
    }

    /**************************************************************************
     * Test: Verify QSPI Open API param check:: qspiClk
     **************************************************************************/
    /* Setup the default QSPI Parameters */
    QSPI_Params_init(&params);

    params.bitRate = 61 * 1000000U;

    /* Open the QSPI Instance for MibSpiA */
    handle = QSPI_open(&params, &errCode);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ("QSPI Open API with QSPI bit clock rate higher than 60MHz", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPI Open API with QSPI bit clock rate higher than 60MHz ", MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        QSPI_close(handle);
    }

    /**************************************************************************
     * Test: Verify QSPI Open API param check:: qspiClk
     **************************************************************************/
    /* Setup the default QSPI Parameters */
    QSPI_Params_init(&params);
    /* Invalid QSPI Clock */
    params.qspiClk = 0U;

    /* Open the QSPI Instance for MibSpiA */
    handle = QSPI_open(&params, &errCode);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ("QSPI Open API with zero QSPI source clock rate", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPI Open API with zero QSPI source clock rate ", MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        QSPI_close(handle);
    }

    /**************************************************************************
     * Test: Verify QSPI Open API param check:: csPoarity
     **************************************************************************/
    /* Setup the default QSPI Parameters */
    QSPI_Params_init(&params);

    params.csPolarity = 2U;

    /* Open the QSPI Instance for MibSpiA */
    handle = QSPI_open(&params, &errCode);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ("QSPI Open API with incorrect CS polarity", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPI Open API with incorrect CS polarity ", MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        QSPI_close(handle);
    }


    /**************************************************************************
     * Test: Verify QSPI Open API param check:: dataDelay
     **************************************************************************/
    /* Setup the default QSPI Parameters */
    QSPI_Params_init(&params);

    params.dataDelay = (uint8_t)10U;

    /* Open the QSPI Instance for MibSpiA */
    handle = QSPI_open(&params, &errCode);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ("QSPI Open API with incorrect data delay ", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPI Open API with incorrect data delay ", MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        QSPI_close(handle);
    }

    /**************************************************************************
     * Test: Verify QSPI read/write param check
     **************************************************************************/
    /* Setup the default QSPI Parameters */
    QSPI_Params_init(&params);

    /* Open the QSPI Instance for MibSpiA */
    handle = QSPI_open(&params, &errCode);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ("QSPI Read API with incorrect number of Words ", MCPI_TestResult_FAIL);
    }
    else
    {

        retVal = QSPI_cfgRead(handle, QSPI_CMD_4PIN_READ_SINGLE, QSPI_WORDLEN_8BIT, (uint32_t *)&tempData[0], QSPI_MAX_NUM_WORDS + 1U);

        if (retVal == 0)
        {
            MCPI_setFeatureTestResult ("QSPI Read API with incorrect number of Words ", MCPI_TestResult_FAIL);
        }
        else
        {
            MCPI_setFeatureTestResult ("QSPI Read API with incorrect number of Words ", MCPI_TestResult_PASS);
        }

        /* Graceful shutdown */
        QSPI_close(handle);
    }
Exit:
    System_printf("Debug: QSPI Test is done!\n", handle);

    MCPI_setTestResult ();

    /* After test all done, terminate DSP by calling BIOS_exit().
        This is required by MCPI test framework script.
        MCPI test framework script waits for DSP halt,
        if DSP doesn't halt, script will wait until timeout then claim test fail.
     */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
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
        goto Exit;
    }

    /* Debug Message: */
    System_printf ("******************************************\n");
    System_printf ("Debug: QSPI Driver Test Application Start \n");
    System_printf ("******************************************\n");

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    taskParams.arg0 = (UArg)socHandle;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();

Exit:
    return 0;
}
