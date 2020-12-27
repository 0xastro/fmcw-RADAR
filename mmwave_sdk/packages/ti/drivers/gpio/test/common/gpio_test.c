/*
 *   @file  gpio_test.c
 *
 *   @brief
 *      This file contains common GPIO Test Functions.
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
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/gpio/gpio.h>
#include <ti/drivers/osal/MemoryP.h>

/**************************************************************************
 *************************** GPIO Test Functions **************************
 **************************************************************************/

/* Global semaphore handle which tracks if the GPIO Switch was pressed */
Semaphore_Handle    gGPIOInputSemaphoreHandle;

/**************************************************************************
 *************************** GPIO Test Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Callback function invoked when the GPIO switch is pressed
 *
 *  @param[in]  index
 *      GPIO index configured as input
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -   <0
 */
static void myGPIOSwitchPressCallbackFxn(unsigned int index)
{
    /* Post the semaphore & wakeup the test task to indicate that the
     * switch press has been detected */
    Semaphore_post (gGPIOInputSemaphoreHandle);
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the GPIO Input
 *
 *  @param[in]  gpioNum
 *      GPIO to be used for testing the feature
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -   <0
 */
int32_t Test_input (uint8_t gpioNum)
{
    Semaphore_Params    semParams;

    /* Initialize the semaphore parameters */
    Semaphore_Params_init(&semParams);
    gGPIOInputSemaphoreHandle = Semaphore_create(0U, &semParams, NULL);
    if (gGPIOInputSemaphoreHandle == NULL)
        return -1;

    /********************************************************************
     * The Test will wait for the switch to be pressed
     ********************************************************************/
    System_printf ("Debug: The test will wait for the switch attached to\n", gpioNum);
    System_printf ("       GPIO %d to be pressed.\n", gpioNum);
    System_printf ("------------------------------------------------------\n");

    /* Set the GPIO to be input: Interrupt capabilities */
    GPIO_setConfig (gpioNum, GPIO_CFG_INPUT | GPIO_CFG_IN_INT_RISING | GPIO_CFG_IN_INT_LOW);

    /* Register the callback function: */
    GPIO_setCallback (gpioNum, myGPIOSwitchPressCallbackFxn);

    /* Enable the interrupts: */
    GPIO_enableInt (gpioNum);

    /* Wait for the semaphore to be posted */
    Semaphore_pend (gGPIOInputSemaphoreHandle, BIOS_WAIT_FOREVER);

    /* Control comes here implies that the switch has been pressed. */
    System_printf ("Debug: Switch press has been detected\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the GPIO Output
 *
 *  @param[in]  gpioNum
 *      GPIO to be used for testing the feature
 *
 *  @retval
 *      Success  -   0
 *  @retval
 *      Error    -   <0
 */
int32_t Test_output (uint8_t gpioNum)
{
    int32_t     index;
    int32_t     numBlinks = 20;

    /********************************************************************
     * The Test will blink an LED on the console
     ********************************************************************/
    System_printf ("Debug: The GPIO Output test will blink an LED on the\n");
    System_printf ("       board. The GPIO Number %d is being used in this\n", gpioNum);
    System_printf ("       test. The test will blink the LED %d times\n", numBlinks);
    System_printf ("------------------------------------------------------\n");
    System_printf ("Press any key to start the test: ");
    scanf ("%d\n", &index);

    /* Set the GPIO to be output */
    GPIO_setConfig (gpioNum, GPIO_CFG_OUTPUT);

    /* Cycle through and toggle the LED */
    for (index = 0; index < numBlinks; index++)
    {
        GPIO_toggle (gpioNum);
        Task_sleep(1000);
    }
    return 0;
}


