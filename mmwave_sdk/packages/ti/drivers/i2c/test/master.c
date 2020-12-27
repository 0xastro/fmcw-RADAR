/*
 *   @file  master.c
 *
 *   @brief
 *      Unit Test code for I2C Master driver
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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
#include <ti/drivers/i2c/I2C.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/esm/esm.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/drivers/osal/HwiP.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
/** \brief Number of messages sent */
#define I2C_TEST_MESSAGE_COUNT          100
#define I2C_TEST_BLOCKING_MODE          1

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* Global Variables */
volatile uint32_t       testSelection = 0;
volatile uint32_t       gTransferDone = 0;
volatile uint32_t       gTransferErrors = 0, gTransferCount = 0;
uint8_t                 rxData[16];
uint8_t                 txData[16];
uint32_t                rxTicks[I2C_TEST_MESSAGE_COUNT];
uint32_t                txTicks[I2C_TEST_MESSAGE_COUNT];
uint32_t                minRxTicks;
uint32_t                maxRxTicks;
uint32_t                minTxTicks;
uint32_t                maxTxTicks;
uint32_t                totalTxTicks;
uint32_t                totalRxTicks;
I2C_Transaction         i2cTransaction;

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
 *      Platform specific intializations.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */

static int32_t PlatformInit(void)
{
#if (defined(SOC_XWR14XX))
    /* Setup the PINMUX to bring out the XWR14xx I2C pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINR3_PADAH, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINR3_PADAH, SOC_XWR14XX_PINR3_PADAH_I2C_SDA);
    Pinmux_Set_OverrideCtrl(SOC_XWR14XX_PINP4_PADAI, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR14XX_PINP4_PADAI, SOC_XWR14XX_PINP4_PADAI_I2C_SCL);
#elif (defined(SOC_XWR16XX))
    /* Setup the PINMUX to bring out the XWR16xx I2C pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR16XX_PINF13_PADAH, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR16XX_PINF13_PADAH, SOC_XWR16XX_PINF13_PADAH_I2C_SDA);

    Pinmux_Set_OverrideCtrl(SOC_XWR16XX_PING14_PADAI, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR16XX_PING14_PADAI, SOC_XWR16XX_PING14_PADAI_I2C_SCL);
#elif (defined(SOC_XWR18XX))
    /* Setup the PINMUX to bring out the XWR18xx I2C pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PINF13_PADAH, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR18XX_PINF13_PADAH, SOC_XWR18XX_PINF13_PADAH_I2C_SDA);

    Pinmux_Set_OverrideCtrl(SOC_XWR18XX_PING14_PADAI, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR18XX_PING14_PADAI, SOC_XWR18XX_PING14_PADAI_I2C_SCL); 
#elif (defined(SOC_XWR68XX))
    /* Setup the PINMUX to bring out the XWR68xx I2C pins */
    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PINF13_PADAH, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PINF13_PADAH, SOC_XWR68XX_PINF13_PADAH_I2C_SDA);

    Pinmux_Set_OverrideCtrl(SOC_XWR68XX_PING14_PADAI, PINMUX_OUTEN_RETAIN_HW_CTRL, PINMUX_INPEN_RETAIN_HW_CTRL);
    Pinmux_Set_FuncSel(SOC_XWR68XX_PING14_PADAI, SOC_XWR68XX_PING14_PADAI_I2C_SCL); 
#endif

    return 0;
}

static int32_t i2cBlockingModeTest()
{
    I2C_Handle      i2cHandle;
    I2C_Params      i2cParams;
    bool            retVal = false;
    int32_t         errCode = 0;
    uint32_t        i = 0, numTestBytes;
    uint32_t        arg;

    /* Reset the transmit and receive buffer */
    memset(&rxData, 0, sizeof (rxData));

    /* Initializa the I2C driver */
    I2C_init();

    /* Initialize the I2C driver default parameters */
    I2C_Params_init(&i2cParams);

    i2cParams.transferMode = I2C_MODE_BLOCKING;
    i2cParams.bitRate = I2C_100kHz;

    /* Open the I2C driver */
    i2cHandle = I2C_open(0, &i2cParams);

    if (i2cHandle == NULL)
    {
        System_printf ("Error: I2C Driver Open failed\n");
        return -1;
    }

    /* Configure the I2C device in loopback mode  */
    arg = 1;
    errCode = I2C_control (i2cHandle, I2C_CMD_LOOPBACK, (void* )&arg);
    if (errCode < 0)
    {
        System_printf ("Error: I2C control Set loopback failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Test combined write and read */
    txData[0] = 0x4;
    i2cTransaction.slaveAddress = 0x48;
    i2cTransaction.writeBuf = txData;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxData;
    i2cTransaction.readCount = 1;

    /* Writing to slave and read what was sent by slave as well */
    retVal = I2C_transfer(i2cHandle, &i2cTransaction);

    if (retVal == false)
    {
        System_printf ("Error: I2C combined Transfer to Slave failed\n");
        return -1;
    }

    for (i = 0; i < i2cTransaction.writeCount; i++)
    {
        if (rxData[i] != txData[i])
        {
            System_printf ("Error: I2C read error Tx data (%d) Rx data (%d)\n", txData[i], rxData[i]);
            return -1;
        }
    }

    System_printf ("Debug: I2C combined Transfer to Slave passed\n");

    /* Test write only followed by read only */
    memset(&rxData, 0, sizeof (rxData));
    txData[0] = 0xA;
    numTestBytes = 1;
    i2cTransaction.slaveAddress = 0x48;
    i2cTransaction.writeBuf = txData;
    i2cTransaction.writeCount = numTestBytes;
    i2cTransaction.readBuf = rxData;
    i2cTransaction.readCount = 0;

    /* Writing to slave */
    retVal = I2C_transfer(i2cHandle, &i2cTransaction);
    if (retVal == false)
    {
        System_printf ("Error: I2C Write to Slave failed\n");
        return -1;
    }
    System_printf ("Debug: I2C Write to Slave passed\n");

    i2cTransaction.writeCount = 0;
    i2cTransaction.readBuf = rxData;
    i2cTransaction.readCount = numTestBytes;

    /* Re-try writing to slave till I2C_transfer returns true */
    retVal = I2C_transfer(i2cHandle, &i2cTransaction);
    if (retVal == false)
    {
        System_printf ("Error: I2C Read from Slave failed\n");
        return -1;
    }

    for (i = 0; i < numTestBytes; i++)
    {
        if (rxData[i] != txData[i])
        {
            System_printf ("Error: I2C read error Tx data (%d) Rx data (%d)\n", txData[i], rxData[i]);
            return -1;
        }
    }
    System_printf ("Debug: I2C Read from Slave passed\n");

    /* Close I2C driver */
    I2C_close(i2cHandle);
    return 0;
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
        System_printf ("I2C Master Unit Test Menu                   \n");
        System_printf ("Please select the type of test to execute:  \n");
        System_printf ("1. I2C Blocking mode test                   \n");
        System_printf ("*******************************************************\n");
        System_printf ("> Enter your selection: ");

        while (testSelection == 0);

        /* Validate the selection: */
        if (testSelection == I2C_TEST_BLOCKING_MODE)
            break;
    }

    if (testSelection == I2C_TEST_BLOCKING_MODE)
    {
        System_printf("Debug: Blocking mode testing\n");
        retVal = i2cBlockingModeTest();
        if (retVal == -1)
            MCPI_setFeatureTestResult("Blocking mode testing", MCPI_TestResult_FAIL);
        else
            MCPI_setFeatureTestResult("Blocking mode testing", MCPI_TestResult_PASS);
    }

    if (retVal < 0)
    {
        System_printf("Debug: I2C testing failed\n");
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

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
    return 0;
}

