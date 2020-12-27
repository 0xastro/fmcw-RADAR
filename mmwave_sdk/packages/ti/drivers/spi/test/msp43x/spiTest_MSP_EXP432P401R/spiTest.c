/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== spiTestMSP43x.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <string.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>

/* Example/Board Header files */
#include "Board.h"


/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/
/* SPI test definitions */
#define COMMAND_LENGTH             12
#define MAGIC_NUMBER               0x3456
#define SPI_TEST_MSGLEN            128
#define SPI_TEST_SYNC_MSGLEN       16
#define SPI_TEST_LOOP                  1000
#define TASKSTACKSIZE                  768

/**************************************************************************
 *************************** Local Data Structure *************************
 **************************************************************************/

/* Test Message Data Structure */
typedef struct spiTestMsg
{
    /* Test message MAGIC number */
    unsigned short magicNumber;

    /* Test message sequence number */
    unsigned short seqNo;

    /* Test message valid data length */
    unsigned short dataLen;

    /* Test message checksum */
    unsigned short checksum;

    /* Test message data payload */
    unsigned char  data[SPI_TEST_MSGLEN - 8];
}spiTestMsg;

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
unsigned char SPITestRxBuffer[SPI_TEST_MSGLEN];
unsigned char SPITestTxBuffer[SPI_TEST_MSGLEN];

/* Allocate buffers in .dma section of memory for concerto devices */
#ifdef MWARE
#pragma DATA_SECTION(SPITestRxBuffer, ".dma");
#pragma DATA_SECTION(SPITestTxBuffer, ".dma");
#endif

Task_Struct task0Struct, task1Struct;
Char task0Stack[TASKSTACKSIZE], task1Stack[TASKSTACKSIZE];

/* SPI test global variables */
unsigned short   gTestMsgSeqNo = 0;
#ifdef TEST_SPI_MASTER
uint8_t          gMasterMode   = true;
#else
uint8_t          gMasterMode   = false;
#endif

/**************************************************************************
 *************************** SPI Test Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      SPI read only function.
 *
 *   @param[in] handle             SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] buffer              Pointer of the buffer
 *
 *  @retval
 *      Not Applicable.
 */
static bool Test_spiRead(const SPI_Handle handle, uint32_t dataLen, void* buffer)
{

    SPI_Transaction transaction;
    bool            ret = true;
#if 1
    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = NULL;
    transaction.rxBuf = buffer;
    transaction.arg    = NULL;

    /* Start Data Transfer */
    ret = SPI_transfer(handle, &transaction);
#else
    uint32_t        loop;
    char *          pBuff = buffer;
    for (loop=0; loop<dataLen/2; loop++)
    {
        /* Configure Data Transfer */
        transaction.count = 2;
        transaction.txBuf = NULL;
        transaction.rxBuf = pBuff;
        transaction.arg   = NULL;

        /* Start Data Transfer */
        ret = SPI_transfer(handle, &transaction);
        pBuff +=2;
        Task_sleep(1);
    }


#endif
    return ret;
}

/**
 *  @b Description
 *  @n
 *      SPI write only function.
 *
 *   @param[in] handle             SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] buffer              Pointer of the buffer
 *
 *  @retval
 *      Not Applicable.
 */
static bool Test_spiWrite(const SPI_Handle handle, uint32_t dataLen, void* buffer)
{
    SPI_Transaction transaction;
    bool            ret = true;
#if 1
    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = buffer;
    transaction.rxBuf = NULL;
    transaction.arg   = NULL;

    /* Start Data Transfer */
    ret = SPI_transfer(handle, &transaction);
#else
    uint32_t        loop;
    char *          pBuff = buffer;
    for (loop=0; loop<dataLen/2; loop++)
    {
        /* Configure Data Transfer */
        transaction.count = 2;
        transaction.txBuf = pBuff;
        transaction.rxBuf = NULL;
        transaction.arg   = NULL;

        /* Start Data Transfer */
        ret = SPI_transfer(handle, &transaction);
        pBuff += 2;
    }


#endif
    return ret;
}

#if 0
/**
 *  @b Description
 *  @n
 *      SPI read and write function.
 *
 *   @param[in] handle             SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] inBuffer           Pointer of the read buffer
 *   @param[in] outBuffer         Pointer of the write buffer
 *
 *  @retval
 *      Not Applicable.
 */
static bool Test_spiReadWrite(const SPI_Handle handle, uint32_t dataLen, void* inBuffer, void* outBuffer)
{
    SPI_Transaction transaction;
    bool            ret;

    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = outBuffer;
    transaction.rxBuf = inBuffer;
    transaction.arg    = NULL;

    /* Start Data Transfer */
    ret = SPI_transfer(handle, &transaction);
    return (ret);
}
#endif

/**
 *  @b Description
 *  @n
 *       This function generates the test pattern for read/write test. 
 *
 *   @param[in] buffer             Buffer pointer for the generated data
 *   @param[in] dataLen          Buffer len (in bytes) for pattern generation
 *
 *  @retval    None
 */
static void Test_generateTestPattern(Ptr buffer, uint8_t seed)
{
    uint32_t index;
    uint8_t  *pBuffer = buffer;
    
    for(index=0; index<SPI_TEST_MSGLEN; index++)
    {
        *pBuffer = (index + seed) & 0xFF;
        pBuffer++;
    }
}

/**
 *  @b Description
 *  @n
 *       This function tests SPI write when SPI works as SPI slave.
 *
 *   @param[in] spiHandle       SPI driver handle
 *
 *  @retval    Success     - true
 *                 Fail           - false
 */
static bool Test_blockWriteSlave(SPI_Handle spiHandle)
{
    uint32_t loop;
    bool     ret;
    
    /* Create test pattern */
    Test_generateTestPattern((Ptr)SPITestTxBuffer, 0x55);

    /* Start write test */
    for (loop=0; loop < 1000; loop++)
    {
        SPITestTxBuffer[0] = loop;
        ret = Test_spiWrite(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestTxBuffer);
    }
    return (ret);
}

/**
 *  @b Description
 *  @n
 *       This function tests SPI read when SPI works as SPI slave.
 *
 *   @param[in] spiHandle       SPI driver handle
 *
 *  @retval    Success     - true
 *                 Fail           - false
 */
static bool Test_blockReadSlave(SPI_Handle spiHandle)
{
    uint32_t loop;
    bool     ret;

    /* Start write test */
    for (loop=0; loop < 1000; loop++)
    {
        ret = Test_spiRead(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestRxBuffer);
        SPITestRxBuffer[SPI_TEST_MSGLEN - 1] = loop;
    }
    return (ret);
}

/**
 *  @b Description
 *  @n
 *       This function tests SPI write when SPI works as SPI master.
 *
 *   @param[in] spiHandle       SPI driver handle
 *
 *  @retval    Success     - true
 *                 Fail           - false
 */
static bool Test_blockWriteMaster(SPI_Handle spiHandle)
{
    uint32_t loop;
    bool     ret;
    
    /* Create test pattern */
    Test_generateTestPattern((Ptr)SPITestTxBuffer, 0x55);

    /* Start write test */
    for (loop=0; loop < 1000; loop++)
    {
        SPITestTxBuffer[0] = loop;
        ret = Test_spiWrite(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestTxBuffer);
        Task_sleep(10);
    }
    return (ret);
}

/**
 *  @b Description
 *  @n
 *       This function tests SPI read when SPI works as SPI master.
 *
 *   @param[in] spiHandle       SPI driver handle
 *
 *  @retval    Success     - true
 *                 Fail           - false
 */
static bool Test_blockReadMaster(SPI_Handle spiHandle)
{
    uint32_t loop;
    bool     ret;

    /* Start write test */
    for (loop=0; loop < 1000; loop++)
    {
        ret = Test_spiRead(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestRxBuffer);
        SPITestRxBuffer[2] = loop;
        Task_sleep(1);
    }
    return (ret);
}

/**
 *  @b Description
 *  @n
 *       This function implements tight loop delay.
 *
 *   @param[in] count       Loop count for the delay
 *
 *  @retval    Success     - true
 *                 Fail           - false
 */
Test_delay(uint32_t count)
{
    volatile uint32_t i;
    
    for(i=0; i<count; i++);
}

/**
 *  @b Description
 *  @n
 *       This function tests SPI interface with data Echoing back
 *
 *   @param[in] spiHandle       SPI driver handle
 *   @param[in] masterMode   Flag for master/slave mode
 *
 *  @retval    None
 */
static void Test_dataEcho(SPI_Handle spiHandle, bool masterMode)
{
    unsigned short loop = 0, failed=0;
    bool transferOK;
    spiTestMsg*   pTestMsgOut;
    spiTestMsg*   pTestMsgIn;
    unsigned char retry = 0;
    
    gTestMsgSeqNo = 0;
    
    /* Test data echo */
    pTestMsgOut = (spiTestMsg*)&SPITestTxBuffer[0];
    pTestMsgIn = (spiTestMsg*)&SPITestRxBuffer[0];
    
    pTestMsgOut->magicNumber = MAGIC_NUMBER;
    pTestMsgOut->seqNo = gTestMsgSeqNo;
    pTestMsgOut->dataLen = SPI_TEST_SYNC_MSGLEN;
    pTestMsgOut->data[0] = SPI_TEST_LOOP & 0xff;
    pTestMsgOut->data[1] = (SPI_TEST_LOOP>>8) & 0xff;
    
    do
    {
        if(masterMode) Test_delay(5000);
        transferOK = Test_spiWrite(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestTxBuffer);
        if(transferOK) 
        {
            /* Print contents of master receive buffer */
            System_printf("Master: Send Sync message\n");
        }
        else
        {
            System_printf("Unsuccessful master SPI transfer");
        }
        
        if(masterMode) Test_delay(5000);
        transferOK = Test_spiRead(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestRxBuffer);
        
        if(transferOK) 
        {
            /* Print contents of master receive buffer */
            System_printf("Master: Receive Sync message\n");
        }
        else {
            System_printf("Unsuccessful master SPI transfer");
        }
        
        if (pTestMsgIn->magicNumber != MAGIC_NUMBER)
        {
            continue;
        }
        if (pTestMsgIn->dataLen != SPI_TEST_SYNC_MSGLEN)
        {
            continue;
        }
        if (pTestMsgIn->seqNo != (gTestMsgSeqNo + 1))
        {
            continue;
        }
        break;
    }while(1);
    
    gTestMsgSeqNo += 2;

    /* Initialize the data pattern */
    Test_generateTestPattern((Ptr)SPITestTxBuffer, 0x0);
    pTestMsgOut->magicNumber = MAGIC_NUMBER;
    pTestMsgOut->dataLen = SPI_TEST_MSGLEN;
    if(masterMode) Test_delay(50000);

    for (loop = 0; loop < SPI_TEST_LOOP + 1; loop++)
    {
        retry = 0;
        
        /* Update sequence number */
        pTestMsgOut->seqNo = gTestMsgSeqNo;
        
        do
        {
            if(masterMode)
            Test_delay(5000);
            /* Initiate SPI transfer */
            transferOK = Test_spiWrite(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestTxBuffer);
            if(transferOK != true) 
            {
                System_printf("Unsuccessful master SPI transfer");
            }
            
            if(masterMode)
                Test_delay(5000);
            Test_spiRead(spiHandle, SPI_TEST_MSGLEN, (Ptr)SPITestRxBuffer);
            
            if ((pTestMsgIn->magicNumber != MAGIC_NUMBER) ||
                (pTestMsgIn->dataLen != SPI_TEST_MSGLEN) ||
                (pTestMsgIn->seqNo != (gTestMsgSeqNo + 1)))
            {
                retry++;
                System_printf("Incorrect in Msg: magic =0x%x, dataLen=%d, seqNo=%d\n", pTestMsgIn->magicNumber, pTestMsgIn->dataLen, pTestMsgIn->seqNo);
            }
            else
                break;
        } while (1);
        
        if (memcmp((void *)pTestMsgIn->data, (void *)pTestMsgOut->data, pTestMsgOut->dataLen - 8) != 0)
        {
            failed++;
            System_printf("Data test failed for loop=%d\n",loop);
        }
        
        gTestMsgSeqNo += 2;
    }
    
    System_printf("Total test loop=%d, failed %d loops\n", SPI_TEST_LOOP, failed);
    System_flush();
}

/**
 *  @b Description
 *  @n
 *       This function is main thread for SPI test when works as SPI slave
 *
 *   @param[in] arg0       Arg0 for the task
 *   @param[in] arg1       Arg1 for the task
 *
 *  @retval    None
 */
Void slaveTaskFxn (UArg arg0, UArg arg1)
{
    SPI_Handle     spiHandle;
    SPI_Params     params;
    uint32_t       loop;

    /* Initialize SPI paramters */
    SPI_Params_init(&params);
    params.frameFormat = SPI_POL0_PHA1;
    params.mode = SPI_SLAVE;

    /* Initialize SPI handle as default master */
    spiHandle = SPI_open(Board_SPI0, &params);
    if (spiHandle == NULL) {
        System_abort("Error initializing SPI\n");
    }
    else {
        System_printf("SPI initialized as SPI Slave\n");
    }
    System_flush();

    /* Test host write */
    if(Test_blockWriteSlave(spiHandle) == true)
    {
        System_printf("Test host write succeeded!\n");
    }
    else
    {
        System_printf("Test host write failed!\n");
    }

    /* Test Host read */
    if(Test_blockReadSlave(spiHandle) == true)
    {
        System_printf("Test host read succeeded!\n");
    }
    else
    {
        System_printf("Test host read failed!\n");
    }
    System_flush();

    System_printf("Start Data Echo test\n");
    System_flush();

    for(loop = 0; loop < 10; loop++)
    {
        /* Data Echo Test */
        Test_dataEcho(spiHandle, false);
    }

    /* Deinitialize SPI */
    SPI_close(spiHandle);

    System_printf("Done\n");

    System_flush();
}

/**
 *  @b Description
 *  @n
 *       This function is main thread for SPI test when works as SPI master
 *
 *   @param[in] arg0       Arg0 for the task
 *   @param[in] arg1       Arg1 for the task
 *
 *  @retval    None
 */
Void masterTaskFxn (UArg arg0, UArg arg1)
{
    SPI_Handle     spiHandle;
    SPI_Params     params;
    uint8_t        loop;

    /* Initialize SPI paramters */
    SPI_Params_init(&params);
    
    /* Set clock bit rate and poloarity */
    params.frameFormat = SPI_POL0_PHA1;
    params.mode = SPI_MASTER;
    params.bitRate = 1000000;

    /* Initialize SPI handle as default master */
    spiHandle = SPI_open(Board_SPI0, &params);
    if (spiHandle == NULL) {
        System_abort("Error initializing SPI\n");
    }
    else {
        System_printf("SPI initialized as SPI master\n");
    }
    System_flush();

    /* Test host write */
    if(Test_blockWriteMaster(spiHandle) == true)
    {
        System_printf("Test host write succeeded!\n");
    }
    else
    {
        System_printf("Test host write failed!\n");
    }

    Task_sleep(10000);

    /* Test Host read */
    if(Test_blockReadMaster(spiHandle) == true)
    {
        System_printf("Test host read succeeded!\n");
    }
    else
    {
        System_printf("Test host read failed!\n");
    }
    System_flush();

    Task_sleep(10000);
    System_printf("Start Data Echo test\n");
    System_flush();

    for(loop = 0; loop < 10; loop++)
    {
        /* Data Echo Test */
        Test_dataEcho(spiHandle, true);
    }

    /* Deinitialize SPI */
    SPI_close(spiHandle);

    System_printf("Done\n");

    System_flush();
}

/**
 *  @b Description
 *  @n
 *       Main Function of the SPI test project. Depending on the build, it spawn a 
 *  master/slave task to perform SPI test
 *
 *  @retval    0
 */
int main(void)
{
    /* Construct BIOS objects */
    Task_Params taskParams;

    /* Call board init functions. */
    Board_initGeneral();
    Board_initGPIO();
    Board_initSPI();

    if(gMasterMode == true)
    {
        /* Construct master Task threads */
        Task_Params_init(&taskParams);
        taskParams.priority = 1;
        taskParams.stackSize = TASKSTACKSIZE;
        taskParams.stack = &task0Stack;
        Task_construct(&task0Struct, (Task_FuncPtr)masterTaskFxn, &taskParams, NULL);
    }
    else
    {
        /* Construct slave Task threads */
        Task_Params_init(&taskParams);
        taskParams.priority = 1;
        taskParams.stackSize = TASKSTACKSIZE;
        taskParams.stack = &task0Stack;
        Task_construct(&task0Struct, (Task_FuncPtr)slaveTaskFxn, &taskParams, NULL);
    }
    
    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the SPI Test on MSP43x.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
