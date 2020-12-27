/*
 *   @file  main_dss.c
 *
 *   @brief
 *      Unit Test code for the Mailbox DSS.
 *      Implements message exchange between MSS and DSS.
 *      Runs on DSS.
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
#include <ti/sysbios/family/c64p/Cache.h>
#include <ti/sysbios/family/c64p/Hwi.h>
#include <ti/sysbios/family/c64p/EventCombiner.h>

/* Mailbox Driver: */
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/mailbox/include/mailbox_internal.h>

#include <ti/drivers/soc/soc.h>
#include "ti/utils/testlogger/logger.h"

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* *************** Test definition: ***************************************************
1. MSS and DSS come out of reset and initialize mailbox driver.
2. DSS loops, waiting for message 1 from MSS
3. MSS sends message 1 to DSS and MSS waits for message 2 from DSS
4. DSS receives message 1 from MSS, DSS confirms that content matches expected values
5. DSS sends message 2 to MSS
6. MSS receives message 2 from DSS, MSS confirms that content matches expected values
7. Steps 2-6 are repeated with another pattern, this time only 1 word message each way
8. MSS and DSS declare test PASS or FAIL
*/

/**************************************************************************
 *************************** Mailbox Test Functions **************************
 **************************************************************************/
volatile uint32_t testAppCallbackFlag=0;

static void Test_appCallbackFunction(Mbox_Handle handle, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlag=1;
}

/**************************************************************************
 * Multichannel Test: globals start
 **************************************************************************/
/* DSS will open 4 channels to MSS:
    chId    readMode   writeMode
    1       callback   polling
    3       callback   blocking
    4       blocking   blocking
    7       blocking   blocking
 */
Mbox_Handle  handleArray[MAILBOX_CH_ID_MAX + 1];

void multiChannelTest (void);
void bigTransferTest (void);

volatile uint32_t testAppCallbackFlag1=0;
static void Test_appCallbackFunction1(Mbox_Handle handleDss, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlag1=1;
}
volatile uint32_t testAppCallbackFlag3=0;
static void Test_appCallbackFunction3(Mbox_Handle handleDss, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlag3=1;
}

/**************************************************************************
 * Multichannel Test globals end
 **************************************************************************/

uint32_t gTestPatternWordSend_0=0xAABBCCDD;
uint32_t gTestPatternWordSend_1=0x01020304;
uint32_t gTestPatternWordSend_2=0x05060708;
uint32_t gTestPatternWordSend_3=0xFA12FA12;

uint32_t gTestPatternWordReceive_0=0x11111111;
uint32_t gTestPatternWordReceive_1=0x22222222;
uint32_t gTestPatternWordReceive_2=0xFFFFFFFF;
uint32_t gTestPatternWordReceive_3=0x88887777;

uint32_t volatile gTestFailFlag = 0;

extern int32_t SOC_isMMWaveDSSOperational (SOC_Handle handle, int32_t* errCode);
extern int32_t SOC_setMMWaveDSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);
extern int32_t SOC_isMMWaveMSSOperational (SOC_Handle handle, int32_t* errCode);
extern int32_t SOC_setMMWaveMSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);

SOC_Handle  socHandle;

void Test_dssWaitSync(void)
{
    int32_t  errCode;
    int32_t  retVal = 0;
    
    System_printf("DSS: Sync starting...\n");
    if(socHandle == NULL)
    {
        System_printf("DSS: ERROR! soc handle NULL in Sync fnx...\n");
    }
    SOC_setMMWaveDSSLinkState (socHandle, 1U, &errCode);
    
    while (retVal == 0)
    {
        retVal = SOC_isMMWaveMSSOperational(socHandle,&errCode);
        Task_sleep(10);
    }
    
    SOC_setMMWaveMSSLinkState (socHandle, 0U, &errCode);
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
void Test_initTask(UArg arg0, UArg arg1)
{
    Mailbox_Config  cfg;
    Mbox_Handle  handle;
    int32_t         errCode;
    uint32_t        bufferRx;
    uint32_t        bufferTx[3];
    uint32_t        size;
    Hwi_Params      params;
    uint32_t        i;

    /*Configuring event combiner*/
    Hwi_Params_init(&params);
    params.enableInt = TRUE;
    for (i = 0; i < 4; i++)
    {   
        params.arg = i;
        params.eventId = i;
        if (Hwi_create(4 + i, &EventCombiner_dispatch, &params, NULL) == NULL)
        {
            System_printf("failed to create Hwi interrupt %d\n",4 + i);
        }
    }
    
    Test_dssWaitSync();
    
    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_DSS);
    
    /* Setup the default Mailbox Parameters */
    Mailbox_Config_init(&cfg);
    cfg.readMode = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction;

    /* Open the  Instance */
    handle = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    
    if (handle == NULL)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance\n");
        return;
    }
    if (errCode != 0)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        return;
    }
    System_printf("DSS: Mailbox Instance %p has been opened successfully\n", handle);    

    /**************************************************************************
     * Test: Wait for message from MSS
     **************************************************************************/
    System_printf("DSS: ************ Waiting for message 1 from MSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0)
    {
        Task_sleep(100);
    }
    System_printf("DSS: Message 1 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handle, (uint8_t *)&bufferRx, 4);
    System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_0)
    {
        System_printf("DSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }
        
    /*read second word*/
    bufferRx = 0;
    size = Mailbox_read(handle, (uint8_t *)&bufferRx, 4);
    System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_1)
    {
        System_printf("DSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    /*read third word*/
    bufferRx = 0;
    size = Mailbox_read(handle, (uint8_t *)&bufferRx, 4);
    System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_2)
    {
        System_printf("DSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    System_printf("DSS: Flushing read\n");
    Mailbox_readFlush(handle);
    testAppCallbackFlag = 0;

    /**************************************************************************
     * Test: Write message to MSS
     **************************************************************************/
    System_printf("DSS: ************ Writing message 2 to MSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_0, sizeof(gTestPatternWordSend_0)); 
    memcpy ((void *)(&bufferTx[1]), (void *)&gTestPatternWordSend_1, sizeof(gTestPatternWordSend_1)); 
    memcpy ((void *)(&bufferTx[2]), (void *)&gTestPatternWordSend_2, sizeof(gTestPatternWordSend_2)); 
    
    size = Mailbox_write(handle, (uint8_t*)bufferTx, sizeof(bufferTx));
    System_printf("DSS: Ack received from MSS for message 2. Size=%d\n",size);
    if(size != sizeof(bufferTx))
    {
        System_printf("DSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }

    /**************************************************************************
     * Test: Wait for message from MSS
     **************************************************************************/
    System_printf("DSS: ************ Waiting for message 3 from MSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0)
    {
        Task_sleep(100);
    }
    System_printf("DSS: Message 3 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handle, (uint8_t *)&bufferRx, 4);
    System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_3)
    {
        System_printf("DSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }        

    System_printf("DSS: Flushing read\n");
    Mailbox_readFlush(handle);
    testAppCallbackFlag = 0;

    /**************************************************************************
     * Test: Write message to MSS
     **************************************************************************/
    System_printf("DSS: ************ Writing message 4 to MSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_3, sizeof(gTestPatternWordSend_3)); 
    size = Mailbox_write(handle, (uint8_t*)bufferTx, sizeof(gTestPatternWordSend_3));
    System_printf("DSS: Ack received from MSS for message 4. Size=%d\n",size);
    if(size != sizeof(gTestPatternWordSend_3))
    {
        System_printf("DSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }

    /**************************************************************************
     * Test: Check driver internal stats
     **************************************************************************/
    Mailbox_Driver *driver = (Mailbox_Driver*)handle;
    System_printf("DSS: Checking driver internal stats\n");
    if(    driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 2                               ||  
        driver->rxCount           != 2                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 2                               ||
        driver->boxEmptyIsrCount  != 2                               ||
        driver->readFlushCount    != 2)                               
    {
        System_printf("DSS: Error. Internal stats failed.\n");
        System_printf("DSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        System_printf("DSS: txCount          =%d\n",driver->txCount         );
        System_printf("DSS: rxCount          =%d\n",driver->rxCount         );
        System_printf("DSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        System_printf("DSS: numBytesRead     =%d\n",driver->numBytesRead    );
        System_printf("DSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        System_printf("DSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        System_printf("DSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
     

    if (Mailbox_close(handle) != 0)
    {    
        System_printf("DSS: Error: Failed to close mailbox\n");
        gTestFailFlag = 1;
        return;
    }

    /**************************************************************************
     * Multi-channel Test
     **************************************************************************/
    //Task_sleep(10);
    multiChannelTest();

    /**************************************************************************
     * Test 2KB message
     **************************************************************************/
    Test_dssWaitSync();
    bigTransferTest();    
    
    /**************************************************************************
     * Test END
     **************************************************************************/
    System_printf("DSS: TEST ENDED\n");
    if(gTestFailFlag == 0)
    {
        MCPI_setFeatureTestResult("DSS: MSS_DSS_msg_exchange", MCPI_TestResult_PASS);
        System_printf("DSS: Test Passed\n");
    }    
    else
    {
        MCPI_setFeatureTestResult("DSS: MSS_DSS_msg_exchange", MCPI_TestResult_FAIL);
        System_printf("DSS: Test Failed\n");
    }

    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

#define BIG_MSG_SIZE MAILBOX_DATA_BUFFER_SIZE
uint8_t txBigData[BIG_MSG_SIZE];
uint8_t rxBigData[BIG_MSG_SIZE];

void bigTransferTest (void)
{
    Mailbox_Config  cfg;
    int32_t         errCode,i,size;
    Mbox_Handle  handle;

    System_printf("\nDSS: Starting %dKB transfer test.\n",BIG_MSG_SIZE);
    
    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        System_printf("DSS: Error: Unable to initialize configuration.\n");
        gTestFailFlag = 1;
        return;
    }

    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_2;/*use ID not used until now even though channels are closed at this point*/

    handle = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    if (handle == NULL)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("DSS: Mailbox Instance to MSS %p has been opened successfully\n", handle);    
    
    /*initialize rx buffer */
    memset(rxBigData,0,BIG_MSG_SIZE);
    
    System_printf("DSS: Waiting for big message to arrive...\n");    
    size = Mailbox_read(handle, (uint8_t *)rxBigData, BIG_MSG_SIZE);
    System_printf("DSS: Read %d bytes. \n",size);
    
    /*check for received pattern*/
    for(i=0;i<BIG_MSG_SIZE;i++)
    {
        if(rxBigData[i] != i % 0xFF)
        {
            System_printf("DSS: Error: Byte Position %d. Received %d Expected %d. \n",i,rxBigData[i], (i % 0xFF));
            gTestFailFlag = 1;
        }
    }
    Mailbox_readFlush(handle);

    System_printf("DSS: Sending big data msg to MSS \n");
        
    /*Initialize message to be transferred*/
    for(i=0;i<BIG_MSG_SIZE;i++)
    {
        txBigData[i] = (i + 0xF)%0xFF; /*pattern different than rx pattern*/
    }

    System_printf("DSS: ************ Writing big message to MSS ****************\n");
    size = Mailbox_write(handle, (uint8_t*)txBigData, BIG_MSG_SIZE);
    if(size != BIG_MSG_SIZE)
    {
        System_printf("DSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }
    else
    {
        System_printf("DSS: Transfer completed.\n");        
    }


}

void Test_channel1Task(UArg arg0, UArg arg1)
{
    uint32_t        bufferRx;
    uint32_t        size;
    
    while(1)
    {
        while (testAppCallbackFlag1 == 0)
        {
            Task_sleep(100);
            Task_yield();
        }
        testAppCallbackFlag1 = 0;
        System_printf("DSS: Channel 1 received message.\n");

        /*read first word*/
        bufferRx = 0;
        size = Mailbox_read(handleArray[1], (uint8_t *)&bufferRx, 4);
        System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_1)
        {
            System_printf("DSS: Error. Pattern mismatch.\n");
            gTestFailFlag = 1;
            return;
        }
        Mailbox_readFlush(handleArray[1]);
    }    

}

void Test_channel3Task(UArg arg0, UArg arg1)
{
    uint32_t        bufferRx;
    uint32_t        size;
    
    while(1)
    {
        while (testAppCallbackFlag3 == 0)
        {
            Task_sleep(100);
            Task_yield();
        }
        testAppCallbackFlag3 = 0;
        System_printf("DSS: Channel 3 received message.\n");

        /*read first word*/
        bufferRx = 0;
        size = Mailbox_read(handleArray[3], (uint8_t *)&bufferRx, 4);
        System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_3)
        {
            System_printf("DSS: Error. Pattern mismatch.\n");
            gTestFailFlag = 1;
            return;
        }
        Mailbox_readFlush(handleArray[3]);
    }    

}

void Test_channel4Task(UArg arg0, UArg arg1)
{
    uint32_t        bufferRx;
    uint32_t        size;
    
    while(1)
    {
        size = Mailbox_read(handleArray[4], (uint8_t *)&bufferRx, 4);
        System_printf("DSS: Channel 4 received message.\n");
        System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_2)
        {
            System_printf("DSS: Error. Pattern mismatch.\n");
            gTestFailFlag = 1;
            return;
        }
        Mailbox_readFlush(handleArray[4]);
    }    

}

extern Mailbox_MCB gMailboxMCB;

Task_Handle multiChTaskHandle[MAILBOX_CH_ID_MAX];

void multiChannelTest (void)
{
    Mailbox_Config  cfg;
    int32_t         errCode;
    int32_t         size,i;
    Task_Params        taskParams;
    uint32_t        bufferRx;
    Mailbox_Driver *driver;    
    

    System_printf("*************************************************\n");
    System_printf("\nDSS: Starting Multichannel Test.\n");
    
    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        System_printf("DSS: Error: Unable to initialize configuration.\n");
        gTestFailFlag = 1;
        return;
    }
    /****** ch 1 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_1;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction1;
    cfg.writeMode    = MAILBOX_MODE_POLLING;

    handleArray[1] = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    if (handleArray[1] == NULL)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("DSS: Mailbox Instance to MSS %p has been opened successfully\n", handleArray[1]);    

    /****** ch 3 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_3;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction3;
    cfg.writeMode    = MAILBOX_MODE_BLOCKING;

    handleArray[3] = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    if (handleArray[3] == NULL)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("DSS: Mailbox Instance to MSS %p has been opened successfully\n", handleArray[3]);    

    /****** ch 4 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_4;
    cfg.readMode     = MAILBOX_MODE_BLOCKING;
    cfg.writeMode    = MAILBOX_MODE_BLOCKING;

    handleArray[4] = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    if (handleArray[4] == NULL)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("DSS: Mailbox Instance to MSS %p has been opened successfully\n", handleArray[4]);    

    /****** ch 7 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_7;
    cfg.readMode     = MAILBOX_MODE_BLOCKING;
    cfg.writeMode    = MAILBOX_MODE_BLOCKING;

    handleArray[7] = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    if (handleArray[7] == NULL)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        System_printf("DSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    System_printf("DSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[7]);    

    /***************************************************/
    /*start tasks used for the multichannel test*/
    Task_Params_init(&taskParams);
    taskParams.stackSize = 2*1024;
    multiChTaskHandle[1] = Task_create(Test_channel1Task, &taskParams, NULL);
    
    Task_Params_init(&taskParams);
    taskParams.stackSize = 2*1024;
    multiChTaskHandle[3] = Task_create(Test_channel3Task, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.stackSize = 2*1024;
    multiChTaskHandle[4] = Task_create(Test_channel4Task, &taskParams, NULL);

    /*give time for remote endpoint to open channels*/
    //Task_sleep(1000);
    Test_dssWaitSync();

    for(i=0;i<3;i++)
    {
        /* channels 3 and 4 are write blocking so can write back to back*/
        System_printf("DSS: ************ Writing a message in CHANNEL 3 to MSS ****************\n");
        size = Mailbox_write(handleArray[3], (uint8_t*)&gTestPatternWordSend_3, sizeof(gTestPatternWordSend_3));
        if(size != sizeof(gTestPatternWordSend_3))
        {
            System_printf("DSS: Error. Write failed. Error=%d\n",size);
            gTestFailFlag = 1;
        }

        System_printf("DSS: ************ Writing a message in CHANNEL 4 to MSS ****************\n");
        size = Mailbox_write(handleArray[4], (uint8_t*)&gTestPatternWordSend_2, sizeof(gTestPatternWordSend_2));
        if(size != sizeof(gTestPatternWordSend_2))
        {
            System_printf("DSS: Error. Write failed. Error=%d\n",size);
            gTestFailFlag = 1;
        }

    }    

    System_printf("DSS: ************ Writing a message in CHANNEL 1 to MSS ****************\n");
    size = Mailbox_write(handleArray[1], (uint8_t*)&gTestPatternWordSend_1, sizeof(gTestPatternWordSend_1));
    if(size != sizeof(gTestPatternWordSend_1))
    {
        System_printf("DSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }
    Test_dssWaitSync();
    
    /* Multichannel test end once a message is received in channel 7 which is readBlocking*/
    size = Mailbox_read(handleArray[7], (uint8_t *)&bufferRx, 4);
    System_printf("DSS: Channel 7 received message.\n");
    System_printf("DSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_0)
    {
        System_printf("DSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
        return;
    }
    Mailbox_readFlush(handleArray[7]);
    
    System_printf("DSS: ************ Writing a message in CHANNEL 7 to MSS ****************\n");
    size = Mailbox_write(handleArray[7], (uint8_t*)&gTestPatternWordSend_0, sizeof(gTestPatternWordSend_0));
    if(size != sizeof(gTestPatternWordSend_0))
    {
        System_printf("DSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }

    /*check channel stats*/
    /*channel 1*/
    driver = (Mailbox_Driver*)handleArray[1];
    System_printf("DSS: Checking driver internal stats for channel 1\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 1                               ||  
        driver->rxCount           != 1                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 1                               ||
        driver->boxEmptyIsrCount  != 1                               ||
        driver->readFlushCount    != 1)                               
    {
        System_printf("DSS: Error. Internal stats failed.\n");
        System_printf("DSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        System_printf("DSS: txCount          =%d\n",driver->txCount         );
        System_printf("DSS: rxCount          =%d\n",driver->rxCount         );
        System_printf("DSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        System_printf("DSS: numBytesRead     =%d\n",driver->numBytesRead    );
        System_printf("DSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        System_printf("DSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        System_printf("DSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*channel 3*/
    driver = (Mailbox_Driver*)handleArray[3];
    System_printf("DSS: Checking driver internal stats for channel 3\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 3                               ||  
        driver->rxCount           != 3                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 3                               ||
        driver->boxEmptyIsrCount  != 3                               ||
        driver->readFlushCount    != 3)                               
    {
        System_printf("DSS: Error. Internal stats failed.\n");
        System_printf("DSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        System_printf("DSS: txCount          =%d\n",driver->txCount         );
        System_printf("DSS: rxCount          =%d\n",driver->rxCount         );
        System_printf("DSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        System_printf("DSS: numBytesRead     =%d\n",driver->numBytesRead    );
        System_printf("DSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        System_printf("DSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        System_printf("DSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*channel 4*/
    driver = (Mailbox_Driver*)handleArray[4];
    System_printf("DSS: Checking driver internal stats for channel 4\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 3                               ||  
        driver->rxCount           != 3                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 3                               ||
        driver->boxEmptyIsrCount  != 3                               ||
        driver->readFlushCount    != 3)                               
    {
        System_printf("DSS: Error. Internal stats failed.\n");
        System_printf("DSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        System_printf("DSS: txCount          =%d\n",driver->txCount         );
        System_printf("DSS: rxCount          =%d\n",driver->rxCount         );
        System_printf("DSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        System_printf("DSS: numBytesRead     =%d\n",driver->numBytesRead    );
        System_printf("DSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        System_printf("DSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        System_printf("DSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*channel 7*/
    driver = (Mailbox_Driver*)handleArray[7];
    System_printf("DSS: Checking driver internal stats for channel 7\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 1                               ||  
        driver->rxCount           != 1                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 1                               ||
        driver->boxEmptyIsrCount  != 1                               ||
        driver->readFlushCount    != 1)                               
    {
        System_printf("DSS: Error. Internal stats failed.\n");
        System_printf("DSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        System_printf("DSS: txCount          =%d\n",driver->txCount         );
        System_printf("DSS: rxCount          =%d\n",driver->rxCount         );
        System_printf("DSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        System_printf("DSS: numBytesRead     =%d\n",driver->numBytesRead    );
        System_printf("DSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        System_printf("DSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        System_printf("DSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*check global variables*/
    if(gMailboxMCB.bssInstCount != 0)
    {
        System_printf("DSS: Error: gMailboxMCB.bssInstCount=%d\n",gMailboxMCB.bssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.mssDssInstCount != 4)
    {
        System_printf("DSS: Error: gMailboxMCB.mssDssInstCount=%d\n",gMailboxMCB.mssDssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.initFlag != 1)
    {
        System_printf("DSS: Error: gMailboxMCB.initFlag=%d\n",gMailboxMCB.initFlag);
        gTestFailFlag = 1;
    }
    if((gMailboxMCB.errCnt.bssFull != 0) || (gMailboxMCB.errCnt.bssEmpty != 0) || (gMailboxMCB.errCnt.mssDssFull != 0) || (gMailboxMCB.errCnt.mssDssEmpty != 0))
    {
        System_printf("DSS: Error: gMailboxMCB.errCnt.bssFull=%d\n",gMailboxMCB.errCnt.bssFull);
        System_printf("DSS: Error: gMailboxMCB.errCnt.bssEmpty=%d\n",gMailboxMCB.errCnt.bssEmpty);
        System_printf("DSS: Error: gMailboxMCB.errCnt.mssDssFull=%d\n",gMailboxMCB.errCnt.mssDssFull);
        System_printf("DSS: Error: gMailboxMCB.errCnt.mssDssEmpty=%d\n",gMailboxMCB.errCnt.mssDssEmpty);
        gTestFailFlag = 1;
    }
    
    /*close all channels*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        if((i==1) || (i==3) || (i==4) || (i==7))
        {
            if (Mailbox_close(handleArray[i]) != 0)
            {    
                System_printf("DSS: Error: Failed to close instance %d\n",i);
                gTestFailFlag = 1;
            }
            System_printf("Debug: closed instance %d\n",i);
        }
    }    
    
    /*check global variables*/
    if(gMailboxMCB.bssInstCount != 0)
    {
        System_printf("DSS: Error: gMailboxMCB.bssInstCount=%d\n",gMailboxMCB.bssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.mssDssInstCount != 0)
    {
        System_printf("DSS: Error: gMailboxMCB.mssDssInstCount=%d\n",gMailboxMCB.mssDssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.initFlag != 1)
    {
        System_printf("DSS: Error: gMailboxMCB.initFlag=%d\n",gMailboxMCB.initFlag);
        gTestFailFlag = 1;
    }

    if((gMailboxMCB.hwiHandles.bssFull != 0) || (gMailboxMCB.hwiHandles.bssEmpty != 0) || (gMailboxMCB.hwiHandles.mssDssFull != 0) || (gMailboxMCB.hwiHandles.mssDssEmpty != 0))
    {
        System_printf("DSS: Error: gMailboxMCB.hwiHandles.bssFull=%d\n",gMailboxMCB.hwiHandles.bssFull);
        System_printf("DSS: Error: gMailboxMCB.hwiHandles.bssEmpty=%d\n",gMailboxMCB.hwiHandles.bssEmpty);
        System_printf("DSS: Error: gMailboxMCB.hwiHandles.mssDssFull=%d\n",gMailboxMCB.hwiHandles.mssDssFull);
        System_printf("DSS: Error: gMailboxMCB.hwiHandles.mssDssEmpty=%d\n",gMailboxMCB.hwiHandles.mssDssEmpty);
        gTestFailFlag = 1;
    }
    
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        if(gMailboxMCB.handleArray[i] != NULL)
        {
            System_printf("DSS: Error: gMailboxMCB.handleArray[%d]=0x%x is not NULL\n",i, gMailboxMCB.handleArray[i]);
            gTestFailFlag = 1;
        }    
    }    
    
    /*close auxiliary tasks*/
    Task_delete(&multiChTaskHandle[1]);
    Task_delete(&multiChTaskHandle[3]);
    Task_delete(&multiChTaskHandle[4]);

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
    Task_Params    taskParams;

    SOC_Cfg             socCfg;
    int32_t             errCode;

    /* Initialize test logger */
    MCPI_Initialize();

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: We are bypassing the clock initialization
     * in the unit test here since the MSS unit test is doing this. */
    socCfg.clockCfg = SOC_SysClock_BYPASS_INIT;

    /* Initialize the SOC Module: */
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
    return 0;
}

