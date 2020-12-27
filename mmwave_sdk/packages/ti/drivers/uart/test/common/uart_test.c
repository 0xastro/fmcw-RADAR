/*
 *   @file  uart_test.c
 *
 *   @brief
 *      Common Unit Test Functions which test the UART functionality.
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

/* mmWave SK Include Files: */
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/* MAX Data Pattern Test Size for the Data Tests: */
#define MAX_TEST_BUFFER_SIZE        128

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/* Global Variable which is used to control if bypass non-loopback test */
uint8_t         gMCPIUARTNonLoopbackTestEnable = 1;

/* Global Variable which holds the UART Handle */
UART_Handle     gUARTHandle = NULL;

/**************************************************************************
 ************************** External Definitions **************************
 **************************************************************************/
extern void Test_benchmarkStart(uint32_t counter);
extern uint32_t Test_benchmarkStop(uint32_t counter);

/* SOC specific */
extern uint32_t gCPUClockFrequency;

#ifdef SUBSYS_MSS
/* Global DMA Handle */
extern DMA_Handle   gDMAHandle;
#else
/* Global Variable which holds the EDMA Handle */
extern EDMA_Handle  gEDMAHandle;
#endif

/**************************************************************************
 *************************** UART Test Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is a workaround which is applicable only if the UART is configured
 *      to operate in loopback mode. The workaround requires the transmission
 *      of a dummy character immediately after the UART has been initialized.
 *
 *  @param[in]  uartHandle
 *      UART Driver Handle for which the workaround is to be implemented.
 *
 *  @retval
 *      Not applicable
 */
static void Test_sciLoopbackWorkaround(UART_Handle handle)
{
    uint8_t dummyCharacter = 'X';
    int32_t status;

    /* Write out a DUMMY character: */
    status = UART_writePolling(handle, &dummyCharacter, 1);
    if (status < 0)
    {
        printf("Error: Unable to send out the data over UART [Error %d]\n", status);
    }
    else
    {
        printf ("Debug: Dummy loopback workaround completed.\n");
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the *test* buffer with a predefined
 *      signature
 *
 *  @param[in]  ptrBuffer
 *      Buffer to be populated
 *  @param[in]  size
 *      Size of the buffer
 *
 *  @retval
 *      Not applicable
 */
static void Test_populateBuffer (uint8_t* ptrBuffer, uint32_t size)
{
    uint32_t index;
    char     ch = 'A';

    /* Populate the data buffer: */
    for (index = 0; index < size; index++)
    {
        *(ptrBuffer + index) = ch++;
        if (ch == 'Z')
            ch = 'A';
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the *test* buffer
 *
 *  @param[in]  ptrBuffer
 *      Buffer to be populated
 *  @param[in]  size
 *      Size of the buffer
 *
 *  @retval
 *      0   -   Validated
 *  @retval
 *      <0   -  Not valid
 */
static int32_t Test_validateBuffer (uint8_t* ptrBuffer, uint32_t size)
{
    uint32_t    index;
    char        ch = 'A';
    uint32_t    startIndex;

    /* HACK: There is a workaround required to get the loopback functionality
     * to work. This requires us to send a DUMMY character. This is NOT applicable
     * for a real world use case. */
    if (size == (MAX_TEST_BUFFER_SIZE + 1))
    {
        /* Dummy Character detected; skip it. */
        startIndex = 1;
        size       = size - 1;
        printf ("Debug: Dummy character detected in the validation skipping it.\n");
    }
    else
    {
        /* Dummy Character NOT detected */
        startIndex = 0;
    }

    /* Populate the data buffer: */
    for (index = startIndex; index < size; index++)
    {
        if (*(ptrBuffer + index) != ch)
        {
            printf ("Error: Invalid data buffer Expected %c got %c @ index %d\n",
                    ch, *(ptrBuffer + index), index);
            return -1;
        }

        /* Continue the validation: */
        ch++;
        if (ch == 'Z')
            ch = 'A';
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the UART Driver *WRITE* functions with
 *      various invalid arguments.
 *
 *  @param[in]  handle
 *      UART Handle
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t Test_writeInvalidArgs(UART_Handle handle)
{
    uint8_t     buffer[4];
    int32_t     status;

    /**************************************************************************
     * Test: Write with invalid buffer
     **************************************************************************/
    status = UART_write(handle, NULL, 128);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Write with invalid buffer returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Write with invalid buffer passed\n");

    /**************************************************************************
     * Test: Write with invalid size
     **************************************************************************/
    status = UART_write(handle, &buffer[0], 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Write with invalid size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Write with invalid size passed\n");

    /**************************************************************************
     * Test: Write with invalid buffer and size
     **************************************************************************/
    status = UART_write(handle, NULL, 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Write with invalid buffer & size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Write with invalid buffer & size passed\n");

    /**************************************************************************
     * Test: Write Polling with invalid buffer
     **************************************************************************/
    status = UART_writePolling(handle, NULL, 128);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Write with invalid buffer returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Write Polling with invalid buffer passed\n");

    /**************************************************************************
     * Test: Write Polling with invalid size
     **************************************************************************/
    status = UART_writePolling(handle, &buffer[0], 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Write with invalid size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Write Polling with invalid size passed\n");

    /**************************************************************************
     * Test: Write Polling with invalid buffer and size
     **************************************************************************/
    status = UART_writePolling(handle, NULL, 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Write with invalid buffer & size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Write Polling with invalid buffer & size passed\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the UART Driver *READ* functions with
 *      various invalid arguments.
 *
 *  @param[in]  handle
 *      UART Handle
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t Test_readInvalidArgs(UART_Handle handle)
{
    uint8_t     buffer[4];
    int32_t     status;

    /**************************************************************************
     * Test: Read with invalid buffer
     **************************************************************************/
    status = UART_read(handle, NULL, 128);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Read with invalid buffer returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Read with invalid buffer passed\n");

    /**************************************************************************
     * Test: Read with invalid size
     **************************************************************************/
    status = UART_read(handle, &buffer[0], 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Read with invalid size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Read with invalid size passed\n");

    /**************************************************************************
     * Test: Read with invalid buffer and size
     **************************************************************************/
    status = UART_read(handle, NULL, 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Read with invalid buffer & size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Read with invalid buffer & size passed\n");

    /**************************************************************************
     * Test: Read Polling with invalid buffer
     **************************************************************************/
    status = UART_readPolling(handle, NULL, 128);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Read with invalid buffer returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Read Polling with invalid buffer passed\n");

    /**************************************************************************
     * Test: Read Polling with invalid size
     **************************************************************************/
    status = UART_readPolling(handle, &buffer[0], 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Read with invalid size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Read Polling with invalid size passed\n");

    /**************************************************************************
     * Test: Read Polling with invalid buffer and size
     **************************************************************************/
    status = UART_readPolling(handle, NULL, 0);
    if (status != UART_EINVAL)
    {
        printf("Error: UART Read with invalid buffer & size returned %d\n", status);
        return -1;
    }
    printf("Debug: UART Read Polling with invalid buffer & size passed\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the UART Driver *READ/WRITE* API
 *      in polled mode
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t Test_pollingData(void)
{
    int32_t         status;
    UART_Params     params;
    UART_Handle     handle;
    uint32_t        baudRate[] = { 9600, 115200, 921600 };
    uint32_t        index;
    uint32_t        i;
    int32_t         loopbackStatus;
    uint32_t        rxTicks[MAX_TEST_BUFFER_SIZE];
    uint32_t        txTicks[MAX_TEST_BUFFER_SIZE];
    uint8_t         txData[MAX_TEST_BUFFER_SIZE];
    uint8_t         rxData[MAX_TEST_BUFFER_SIZE];
    uint32_t        minRxTicks;
    uint32_t        maxRxTicks;
    uint32_t        minTxTicks;
    uint32_t        maxTxTicks;
    uint32_t        totalTxTicks;
    uint32_t        totalRxTicks;

    /* Cycle through all the baud rates: */
    for (index = 0; index <= ((sizeof(baudRate)/sizeof(uint32_t)) - 1); index++)
    {
        /* Debug Message: */
        printf("*************************************************************\n");
        printf("Debug: Testing UART in Polling Mode @ Baud Rate: %d\n", baudRate[index]);
        printf("*************************************************************\n");

        /*******************************************************************************
         * Initialization:
         *******************************************************************************/
        UART_Params_init(&params);
        params.clockFrequency = gCPUClockFrequency;
        params.isPinMuxDone   = 1;
        params.baudRate       = baudRate[index];
        params.readEcho       = UART_ECHO_OFF;
        handle = UART_open(0, &params);
        if (!handle)
        {
            printf ("Error: Unable to open the UART Driver\n");
            return -1;
        }

        /* Populate the transmit buffer: */
        Test_populateBuffer (&txData[0], sizeof(txData));

        /* Enable the loopback: */
        loopbackStatus = 1;
        if (UART_control (handle, UART_CMD_LOOPBACK, &loopbackStatus) < 0)
        {
            printf ("Error: Unable to configure the UART Driver in Loopback mode\n");
            return -1;
        }

        /* Implement the Loopback workaround */
        Test_sciLoopbackWorkaround (handle);

        /* Initialize the measurement counters */
        minRxTicks   = 0xFFFFFFFFU;
        maxRxTicks   = 0U;
        minTxTicks   = 0xFFFFFFFFU;
        maxTxTicks   = 0U;
        totalTxTicks = 0U;
        totalRxTicks = 0U;

        /*******************************************************************************
         * Execute the *DATA* test
         *******************************************************************************/
        for (i = 0; i < sizeof(txData); i++)
        {
            /* Reset the counter: */
            Test_benchmarkStart(0);

            /* Write out a character: */
            status = UART_writePolling(handle, &txData[i], 1);
            if (status < 0)
            {
                printf("Error: Unable to send out the data over UART [Error %d]\n", status);
                return -1;
            }

            /* Stop the counter: */
            txTicks[i] = Test_benchmarkStop(0);

            /* Update the transmit statistics: */
            minTxTicks   = (minTxTicks<txTicks[i]) ? minTxTicks : txTicks[i];
            maxTxTicks   = (maxTxTicks>txTicks[i]) ? maxTxTicks : txTicks[i];
            totalTxTicks = totalTxTicks + txTicks[i];

            /* Reset the counter: */
            Test_benchmarkStart(0);

            /* Read the character: */
            status = UART_readPolling(handle, &rxData[i], 1);
            if (status < 0)
            {
                printf("Error: Unable to read the data over UART [Error %d]\n", status);
                return -1;
            }

            /* Stop the counter: */
            rxTicks[i] = Test_benchmarkStop(0);

            /* Update the receive statistics: */
            minRxTicks   = (minRxTicks<rxTicks[i]) ? minRxTicks : rxTicks[i];
            maxRxTicks   = (maxRxTicks>rxTicks[i]) ? maxRxTicks : rxTicks[i];
            totalRxTicks = totalRxTicks + rxTicks[i];
        }

        /***********************************************************************************
         * Data Validation:
         ***********************************************************************************/
        if (Test_validateBuffer (&rxData[0], sizeof(rxData)) < 0)
            return -1;

        /* Debug Message: */
        printf ("Debug: Successfully validated receive/transmit for payload of %d bytes\n", MAX_TEST_BUFFER_SIZE);

        /* Debug Message: */
        printf ("Debug: Receive & Transmit Measurements\n");
        printf ("Debug: Baud Rate  : %d\n", baudRate[index]);
        printf ("Debug: Rx Average : %d ticks\n", totalRxTicks/i);
        printf ("Debug: Tx Average : %d ticks\n", totalTxTicks/i);

        /*******************************************************************************
         * Cleanup:
         *******************************************************************************/
        loopbackStatus = 0;
        if (UART_control (handle, UART_CMD_LOOPBACK, &loopbackStatus) < 0)
        {
            printf ("Error: Unable to configure the UART Driver in Normal mode\n");
            return -1;
        }
        UART_close(handle);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Test Task: This sends out the data on the UART
 *
 *  @retval
 *      Not Applicable.
 */
void Test_nonLoopbackMode(UArg arg0, UArg arg1)
{
    UART_Params     params;
    UART_Handle     uartHandle;
    uint32_t        baudRate = 115200;
    int32_t         status;
    char            messageString[MAX_TEST_BUFFER_SIZE];
    char            rxMessageString[MAX_TEST_BUFFER_SIZE];
    int32_t         numBytes;

    /* Debug Message: */
    printf("*************************************************************************\n");
    printf("Debug: UART NON-Loopback Mode Tests starting [Baud Rate: %d]\n", baudRate);
    printf("Please Ensure that console applications are running on the UART Ports\n");
    printf("*************************************************************************\n");

    /* Setup the default UART Parameters */
    UART_Params_init(&params);
    params.clockFrequency = gCPUClockFrequency;
    params.baudRate       = baudRate;
    params.isPinMuxDone   = 1;

    /* Open the UART Instance */
    uartHandle = UART_open(0, &params);
    if (uartHandle == NULL)
    {
        printf("Error: Unable to open the UART Instance 0\n");
        MCPI_setFeatureTestResult ("UART Non Loopback", MCPI_TestResult_FAIL);
        return;
    }

    /****************************************************************************
     * Sanity Check: UART-1 is capable of receiving/transmitting the data
     *  TEST: Blocking Mode
     ****************************************************************************/
    {
        /* Initialize the arrays */
        memset ((void *)&messageString, 0, sizeof(messageString));
        memset ((void *)&rxMessageString, 0, sizeof(messageString));

        numBytes = snprintf (messageString, sizeof(messageString), "Testing Blocking Mode on UART-1\n");
        UART_write(uartHandle, (uint8_t*)&messageString[0], numBytes);
        numBytes = snprintf (messageString, sizeof(messageString), "*******************************\n");
        UART_write(uartHandle, (uint8_t*)&messageString[0], numBytes);
        numBytes = snprintf (messageString, sizeof(messageString), "Please ECHO the following message back\n");
        UART_write(uartHandle, (uint8_t*)&messageString[0], numBytes);
        numBytes = snprintf (messageString, sizeof(messageString), "UART is working\n");
        UART_write(uartHandle, (uint8_t*)&messageString[0], numBytes);

        /* Read the response back: */
        status = UART_read(uartHandle, (uint8_t*)&rxMessageString[0], strlen(messageString)+1);
        if (status < 0)
        {
            printf("Error: Unable to read out the data over UART0 [Error %d]\n", status);
            MCPI_setFeatureTestResult ("UART Non Loopback Blocking Read", MCPI_TestResult_FAIL);
            return;
        }

        /* Debug Message: */
        printf ("Debug: Got %d bytes '%s'\n", status, rxMessageString);

        /* Validate the strings: */
        if (strncmp (rxMessageString, messageString, (strlen(messageString) - 1)) != 0)
        {
            printf ("Error: Mismatch detected Received String ['%s']\n", rxMessageString);
            MCPI_setFeatureTestResult ("UART Non Loopback Blocking Read", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("UART Non Loopback Blocking Read", MCPI_TestResult_PASS);
    }

    /****************************************************************************
     * Sanity Check: UART-1 is capable of receiving/transmitting the data
     * TEST: Polling Mode
     ****************************************************************************/
    {
        /* Initialize the arrays */
        memset ((void *)&messageString, 0, sizeof(messageString));
        memset ((void *)&rxMessageString, 0, sizeof(messageString));

        numBytes = snprintf (messageString, sizeof(messageString), "Testing Polling Mode on UART-1\n");
        UART_writePolling(uartHandle, (uint8_t*)&messageString[0], numBytes);
        numBytes = snprintf (messageString, sizeof(messageString), "*******************************\n");
        UART_writePolling(uartHandle, (uint8_t*)&messageString[0], numBytes);
        numBytes = snprintf (messageString, sizeof(messageString), "Please ECHO the following message back:\n");
        UART_writePolling(uartHandle, (uint8_t*)&messageString[0], numBytes);
        numBytes = snprintf (messageString, sizeof(messageString), "UART is still working\n");
        UART_writePolling(uartHandle, (uint8_t*)&messageString[0], numBytes);

        /* Read the response back: */
        status = UART_readPolling(uartHandle, (uint8_t*)&rxMessageString[0], strlen(messageString)+1);
        if (status < 0)
        {
            printf("Error: Unable to read out the data over UART0 [Error %d]\n", status);
            MCPI_setFeatureTestResult ("UART Non Loopback Polling Read", MCPI_TestResult_FAIL);
            return;
        }

        /* Debug Message: */
        printf ("Debug: Got %d bytes '%s'\n", status, rxMessageString);

        /* Validate the strings: */
        if (strncmp (rxMessageString, messageString, (strlen(messageString) - 1)) != 0)
        {
            printf ("Error: Mismatch detected Received String ['%s']\n", rxMessageString);
            MCPI_setFeatureTestResult ("UART Non Loopback Polling Read", MCPI_TestResult_FAIL);
            return;
        }
        MCPI_setFeatureTestResult ("UART Non Loopback Polling Read", MCPI_TestResult_PASS);
    }

    /* Clean up:*/
    UART_close(uartHandle);

    /* Report the test status: */
    MCPI_setTestResult ();

    /* Exit BIOS: All the tests are completed */
    BIOS_exit(0);
    return;
}

/**
 *  @b Description
 *  @n
 *      Receive Task
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_rxTask(UArg arg0, UArg arg1)
{
    int32_t         status;
    uint8_t         rxData[MAX_TEST_BUFFER_SIZE];
    Task_Params     taskParams;

    /* Debug Message: */
    printf ("Debug: UART Receive Task [Waiting for the UART Handle]\n");

    /* Loop around and make sure that the UART Handle has been created: */
    while (1)
    {
        if (gUARTHandle != NULL)
            break;
        Task_sleep(1);
    }

    /* Debug Message: */
    printf ("Debug: UART Receive Task is active\n");

    /* Read the data: */
    status = UART_read(gUARTHandle, &rxData[0], MAX_TEST_BUFFER_SIZE);
    if (status < 0)
    {
        printf("Error: UART Read with invalid buffer returned %d\n", status);
        MCPI_setFeatureTestResult ("UART Blocking Read", MCPI_TestResult_FAIL);
        return;
    }
    printf("Debug: UART Read has been completed\n");

    /***********************************************************************************
     * Data Validation:
     ***********************************************************************************/
    if (Test_validateBuffer (&rxData[0], status) < 0)
        return;

    /* Close the driver: */
    UART_close(gUARTHandle);
    gUARTHandle = NULL;

    /* Debug Message: */
    printf("Debug: UART Blocking Read Test completed successfully\n");

    /* Log the test results */
    MCPI_setFeatureTestResult ("UART Blocking Read", MCPI_TestResult_PASS);

    if (gMCPIUARTNonLoopbackTestEnable == 1)
    {
        /* Launch the NON-Loopback Tests */
        Task_Params_init(&taskParams);
        taskParams.stackSize = 4*1024;
        taskParams.priority  = 1;
        Task_create(Test_nonLoopbackMode, &taskParams, NULL);
    }
    else
    {
        /* Report the test status: */
        MCPI_setTestResult ();

        /* Exit BIOS */
        BIOS_exit(0);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Transmit Task
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_txTask(UArg arg0, UArg arg1)
{
    UART_Params     params;
    UART_Handle     handle;
    int32_t         loopbackStatus;
    uint32_t        baudRate = 921600;
    uint8_t         txData[MAX_TEST_BUFFER_SIZE];
    int32_t         status;

    /* Setup the default UART Parameters */
    UART_Params_init(&params);
    params.clockFrequency = gCPUClockFrequency;
    params.baudRate       = baudRate;
    params.readEcho       = UART_ECHO_OFF;
    params.isPinMuxDone   = 1;

    /* Open the UART Instance */
    handle = UART_open(0, &params);
    if (handle == NULL)
    {
        printf("Error: Unable to open the UART Instance\n");
        MCPI_setFeatureTestResult ("UART Blocking Write", MCPI_TestResult_FAIL);
        return;
    }

    /* Debug Message: */
    printf("*************************************************************\n");
    printf("Debug: Testing UART in Blocking Mode @ Baud Rate: %d\n", params.baudRate);
    printf("*************************************************************\n");

    /* Enable the loopback: */
    loopbackStatus = 1;
    if (UART_control (handle, UART_CMD_LOOPBACK, &loopbackStatus) < 0)
    {
        printf ("Error: Unable to configure the UART Driver in Loopback mode\n");
        MCPI_setFeatureTestResult ("UART Blocking Write", MCPI_TestResult_FAIL);
        return;
    }

    /* Populate the transmit buffer: */
    Test_populateBuffer (&txData[0], sizeof(txData));

    /* Implement the Loopback workaround */
    Test_sciLoopbackWorkaround (handle);

    /* We can now execute the test: Wake up the receive task. */
    gUARTHandle = handle;
    Task_sleep(1);

    /* Send out the data */
    status = UART_write(gUARTHandle, &txData[0], MAX_TEST_BUFFER_SIZE);
    if (status < 0)
    {
        printf("Error: UART Write failed [Error code %d]\n", status);
        MCPI_setFeatureTestResult ("UART Blocking Write", MCPI_TestResult_FAIL);
        return;
    }

    /* Log the test results */
    MCPI_setFeatureTestResult ("UART Blocking Write", MCPI_TestResult_PASS);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the blocking data API
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
int32_t Test_blockingData(void)
{
    Task_Params     taskParams;

    /********************************************************************
     * Test: Blocking Data test launch the receive & transmit tasks
     ********************************************************************/
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    taskParams.priority  = 3;
    Task_create(Test_rxTask, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    taskParams.priority  = 2;
    Task_create(Test_txTask, &taskParams, NULL);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the exported UART API.
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error       -   <0
 */
int32_t Test_uartAPI (void)
{
    UART_Params     params;
    UART_Handle     handle;

    /* Setup the default UART Parameters */
    UART_Params_init(&params);
    params.clockFrequency = gCPUClockFrequency;
    params.isPinMuxDone   = 1;

    /* Open the UART Instance */
    handle = UART_open(0, &params);
    if (handle == NULL)
    {
        printf("Error: Unable to open the UART Instance\n");
        return -1;
    }
    printf("Debug: UART Instance %p has been opened successfully\n", handle);

    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    UART_close(handle);
    printf("Debug: UART Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver
     **************************************************************************/
    handle = UART_open(0, &params);
    if (handle == NULL)
    {
        printf("Error: Unable to open the UART Instance\n");
        return -1;
    }
    printf("Debug: UART Instance %p has been reopened successfully\n", handle);

    /* Test: This will test the UART Write with Invalid arguments */
    if (Test_writeInvalidArgs(handle) != 0)
        return -1;

    /* Test: This will test the UART Read with Invalid arguments */
    if (Test_readInvalidArgs(handle) != 0)
        return -1;

    /* Close the driver: */
    UART_close(handle);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to profile the UART Transmit API for
 *      comparison in DMA and Non-DMA mode
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t Test_profileTransmit(uint32_t uartInstanceNum)
{
    int32_t         status;
    UART_Params     params;
    UART_Handle     handle;
    int32_t         testIndex;
    int32_t         dataSizeIndex;
    uint32_t        dataSize[] = { 64, 256, 1024, 4096, 8000 };
    uint32_t        baudRate =  115200;
    uint8_t*        ptrTxData;
    uint32_t        txTimeTaken;
    UART_Stats      stats;
    char            testDescription[128];

    /********************************************************************************
     * Execute the test for all data sizes
     ********************************************************************************/
    for (dataSizeIndex = 0; dataSizeIndex < sizeof(dataSize)/sizeof(uint32_t); dataSizeIndex++)
    {
        /********************************************************************************
         * For each data size we will run through the following:-
         * - Test Index 0: Non-DMA + Blocking Mode
         * - Test Index 1: DMA + Blocking Mode
         * - Test Index 2: Non-DMA + Polling Mode
         * - Test Index 3: DMA + Polling Mode
         ********************************************************************************/
        for (testIndex = 0; testIndex < 4; testIndex++)
        {
            /*******************************************************************************
             * Initialization:
             *******************************************************************************/
            UART_Params_init(&params);
            params.readDataMode   = UART_DATA_BINARY;
            params.writeDataMode  = UART_DATA_BINARY;
            params.isPinMuxDone   = 1;
            params.baudRate       = baudRate;
            params.readEcho       = UART_ECHO_OFF;

            /* Are we operating in DMA or Normal Mode? */
            if ((testIndex == 1) || (testIndex == 3))
            {
                /* DMA Mode: If we are executing in DSS we need to use the
                 * EDMA Handle; While executing on the MSS we need to use
                 * the DMA Handle. */
#ifdef SUBSYS_MSS
                params.dmaHandle      = gDMAHandle;
                params.txDMAChannel   = 1;
                params.rxDMAChannel   = 2;
#else
                params.edmaHandle     = gEDMAHandle;
                params.paramSetId     = EDMA_NUM_DMA_CHANNELS;
#endif
            }

            /* Open the UART Instance: */
            handle = UART_open(uartInstanceNum, &params);
            if (handle == NULL)
            {
                printf ("Error: Unable to open the UART Driver while profiling Transmit Data\n");
                return -1;
            }

            /******************************************************************************
             * Debug Message:
             ******************************************************************************/
            switch (testIndex)
            {
                case 0:
                {
                    sprintf (testDescription, "UART %d Profile Test in Non-DMA Blocking Mode [Data Size %d bytes]",
                             uartInstanceNum, dataSize[dataSizeIndex]);
                    break;
                }
                case 1:
                {
                    sprintf (testDescription, "UART %d Profile Test in DMA Blocking Mode [Data Size %d bytes]",
                             uartInstanceNum, dataSize[dataSizeIndex]);
                    break;
                }
                case 2:
                {
                    sprintf (testDescription, "UART %d Profile Test in Non-DMA Polling Mode [Data Size %d bytes]",
                             uartInstanceNum, dataSize[dataSizeIndex]);
                    break;
                }
                case 3:
                {
                    sprintf (testDescription, "UART %d Profile Test in DMA Polling Mode [Data Size %d bytes]",
                             uartInstanceNum, dataSize[dataSizeIndex]);
                    break;
                }
                default:
                {
                    /* Control should never come here */
                    DebugP_assert (0);
                    return -1;
                }
            }

            /* Allocate the memory */
            ptrTxData = Memory_alloc (NULL, dataSize[dataSizeIndex], 0, 0);
            if (ptrTxData == NULL)
            {
                System_printf ("Error: Unable to allocate memory\n");
                MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
                return -1;
            }

            /* Initialize the allocated memory: */
            memset ((void *)ptrTxData, 0, dataSize[dataSizeIndex]);

            /* Populate the transmit buffer: */
            Test_populateBuffer (ptrTxData, dataSize[dataSizeIndex]);

            /****************************************************************
             * Profile the time taken to send out the data
             ****************************************************************/
            Test_benchmarkStart(0);

            if ((testIndex == 0) || (testIndex == 1))
                status = UART_write(handle, ptrTxData, dataSize[dataSizeIndex]);
            else
                status = UART_writePolling(handle, ptrTxData, dataSize[dataSizeIndex]);

            txTimeTaken = Test_benchmarkStop(0);

            /* Were we able to send out the data */
            if (status != dataSize[dataSizeIndex])
            {
                System_printf ("Error: Unable to send out the data [Status %d]\n", status);
                MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
                return -1;
            }

            /* Get the UART Statistics: */
            if (UART_control (handle, UART_CMD_GET_STATS, &stats) < 0)
            {
                MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
                return -1;
            }

            /* Debug Message: */
            System_printf ("Debug: Transmit Measurements\n");
            System_printf ("Debug: Baud Rate        : %d\n", baudRate);
            System_printf ("Debug: Data Size        : %d\n", dataSize[dataSizeIndex]);
            System_printf ("Debug: Transmit         : %d ticks\n", txTimeTaken);
            System_printf ("Debug: Tx Interrupt     : %d\n", stats.numTxInterrupts);
            System_printf ("Debug: Tx DMA Interrupt : %d\n", stats.numTxDMAInterrupts);

            /* Test Passed: */
            MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_PASS);

            /* Clean out the memory: */
            Memory_free (NULL, ptrTxData, dataSize[dataSizeIndex]);

            /* Close the driver: */
            UART_close(handle);
        }
    }

    /********************************************************************************************
     * This is just a beautification; we have mucked around with the UART Console by dumping
     * all the data. So here we open a console and just put a banner to make it clean
     ********************************************************************************************/
    {
        char* ptrTestDelimitter = "\n\r*********************************************\n\r";

        UART_Params_init(&params);
        params.clockFrequency = gCPUClockFrequency;
        params.readDataMode   = UART_DATA_BINARY;
        params.writeDataMode  = UART_DATA_BINARY;
        params.isPinMuxDone   = 1;
        params.baudRate       = baudRate;
        params.readEcho       = UART_ECHO_OFF;

        /* Open the UART Instance: */
        handle = UART_open(uartInstanceNum, &params);
        if (!handle)
        {
            printf ("Error: Unable to open the UART Driver while printing the test banner\n");
            return -1;
        }
        Task_sleep(1);

        /* Write the banner on the console*/
        UART_writePolling (handle, (uint8_t*)ptrTestDelimitter, strlen(ptrTestDelimitter));
        UART_writePolling (handle, (uint8_t*)ptrTestDelimitter, strlen(ptrTestDelimitter));

        /* Close the driver: */
        UART_close(handle);
    }

    return 0;
}


#ifdef SUBSYS_MSS

/**
 *  @b Description
 *  @n
 *      The function is used to test the UART Driver *READ/WRITE* API
 *      in polled DMA and Non-DMA mode
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t Test_DMAReadWrite(void)
{
    int32_t         status;
    UART_Params     params;
    UART_Handle     handle;
    int32_t         testIndex;
    uint32_t        baudRate =  115200;
    uint8_t         txData[MAX_TEST_BUFFER_SIZE];
    uint8_t         rxData[MAX_TEST_BUFFER_SIZE];
    char            testDescription[128];
    UART_Stats      stats;

    /* Inform the user that they need to have a serial console active. */
    printf ("NOTE: Please ensure that a serial console is ACTIVE and executing @ Baud Rate %d\n", baudRate);
    printf ("      Press any key to proceed\n");
    scanf ("%d", &testIndex);

    /* Populate the transmit buffer: */
    printf ("The following string should be visible on the serial console. Please copy-paste\n");
    printf ("this on the serial console. This will be used to validate the receive buffer\n");
    Test_populateBuffer (&txData[0], sizeof(txData));
    for (testIndex = 0; testIndex < MAX_TEST_BUFFER_SIZE; testIndex++)
        printf ("%c", txData[testIndex]);
    printf("\n");

    /********************************************************************************
     * There are 2 Iterations for the DMA Test to verify DMA operations working in
     * - Blocking Mode
     * - Polling Mode
     ********************************************************************************/
    for (testIndex = 0; testIndex < 2; testIndex++)
    {
        /* Debug Message: */
        if (testIndex == 0)
            sprintf (testDescription, "UART DMA Test with Polling API");
        else
            sprintf (testDescription, "UART DMA Test with Blocking API");

        /*******************************************************************************
         * Initialization:
         *******************************************************************************/
        UART_Params_init(&params);
        params.clockFrequency = gCPUClockFrequency;
        params.readDataMode   = UART_DATA_BINARY;
        params.writeDataMode  = UART_DATA_BINARY;
        params.isPinMuxDone   = 1;
        params.baudRate       = baudRate;
        params.readEcho       = UART_ECHO_OFF;
        params.dmaHandle      = gDMAHandle;
        params.txDMAChannel   = 1;
        params.rxDMAChannel   = 2;

        /* Open the UART Instance: */
        handle = UART_open(0, &params);
        if (!handle)
        {
            printf ("Error: Unable to open the UART Driver\n");
            MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Initialize the receive buffer: */
        memset ((void *)&rxData[0], 0, sizeof(rxData));

        /* Write out the entire data: */
        if (testIndex == 0)
        {
            /* Test Index 0 -> Use the Polling API */
            status = UART_writePolling(handle, &txData[0], sizeof(txData));
        }
        else
        {
            /* Test Index 1 -> Use the Blocking API */
            status = UART_write(handle, &txData[0], sizeof(txData));
        }

        /* Was the write successful? */
        if (status != MAX_TEST_BUFFER_SIZE)
        {
            printf("Error: Unable to send out the data over UART [Error %d]\n", status);
            MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Inform the user that they need to echo back the characters from the console back
         * so that the test can proceed. */
        printf ("NOTE: Please copy and paste the characters back from the console for the test to proceed\n");
        printf ("      The test does *NOT* echo back the characters on the console.\n");

        /* Write out the entire data: */
        if (testIndex == 0)
        {
            /* Test Index 0 -> Use the Polling API */
            status = UART_readPolling(handle, &rxData[0], sizeof(rxData));
        }
        else
        {
            /* Test Index 1 -> Use the Blocking API */
            status = UART_read(handle, &rxData[0], sizeof(rxData));
        }

        /* Was the read succesful? */
        if (status != MAX_TEST_BUFFER_SIZE)
        {
            printf("Error: Unable to read the data over UART [Error %d]\n", status);
            MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
            return -1;
        }

        /***********************************************************************************
         * Data Validation: This is to ensure that there is no garbage received or sent.
         ***********************************************************************************/
        if (Test_validateBuffer (&rxData[0], sizeof(rxData)) < 0)
        {
            MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
            return -1;
        }

        /* Get the UART Statistics: */
        if (UART_control (handle, UART_CMD_GET_STATS, &stats) < 0)
        {
            MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
            return -1;
        }

        /***********************************************************************************
         * Statistics Validation:
         * - Polling Mode : No Rx or Tx DMA Interrupts
         * - Blocking Mode: 1 Rx & 1 Tx DMA Interrupt
         ***********************************************************************************/
        if (testIndex == 0)
        {
            /* Polling Mode: */
            if ((stats.numTxDMAInterrupts == 0U) && (stats.numRxDMAInterrupts == 0U))
            {
                MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_PASS);
            }
            else
            {
                /* Error: Invalid number of interrupts detected. */
                System_printf ("Error: Rx DMA Interrupt %d Tx DMA Interrupt %d\n",
                                stats.numRxDMAInterrupts, stats.numTxDMAInterrupts);
                MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
            }
        }
        else
        {
            /* Blocking Mode: */
            if ((stats.numTxDMAInterrupts == 1U) && (stats.numRxDMAInterrupts == 1U))
            {
                MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_PASS);
            }
            else
            {
                /* Error: Invalid number of interrupts detected. */
                System_printf ("Error: Rx DMA Interrupt %d Tx DMA Interrupt %d\n",
                                stats.numRxDMAInterrupts, stats.numTxDMAInterrupts);
                MCPI_setFeatureTestResult (testDescription, MCPI_TestResult_FAIL);
            }
        }
        /* Close the UART Instance */
        UART_close(handle);
    }
    return 0;
}

#endif

