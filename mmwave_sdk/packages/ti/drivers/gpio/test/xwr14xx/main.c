/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the GPIO Driver
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
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/osal/MemoryP.h>
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

extern int32_t Test_input (uint8_t gpioNum);
extern int32_t Test_output (uint8_t gpioNum);

/**************************************************************************
 *************************** GPIO Test Functions ***************************
 **************************************************************************/

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
    System_printf ("************** GPIO Unit Tests ****************\n");
    System_printf ("***********************************************\n");

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize the GPIO */
    GPIO_init ();

    /* Test the GPIO Input: Configure pin N13 as GPIO_1 input */
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINN7_PADAC, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINN7_PADAC, SOC_XWR14XX_PINN7_PADAC_GPIO_1);

    /* Test the GPIO Input */
    if (Test_input (SOC_XWR14XX_GPIO_1) < 0)
    {
        MCPI_setFeatureTestResult ("GPIO Input", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("GPIO Input", MCPI_TestResult_PASS);

    /* Test the GPIO Output: Configure pin N13 as GPIO_2 output */
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINN13_PADAZ, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINN13_PADAZ, SOC_XWR14XX_PINN13_PADAZ_GPIO_2);

    /* Test the GPIO Output */
    if (Test_output (SOC_XWR14XX_GPIO_2) < 0)
    {
        MCPI_setFeatureTestResult ("GPIO Output", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("GPIO Output", MCPI_TestResult_PASS);
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
int32_t main (void)
{
    int32_t         errCode;
    SOC_Handle      socHandle;
    Task_Params     taskParams;
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

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
}

