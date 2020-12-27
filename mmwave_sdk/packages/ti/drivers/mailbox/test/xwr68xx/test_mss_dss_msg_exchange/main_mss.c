/*
 *   @file  main_mss.c
 *
 *   @brief
 *      Unit Test code for the Mailbox MSS
 *      Implements message exchange between MSS and DSS.
 *      Runs on MSS.
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

#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include "ti/utils/testlogger/logger.h"


/* Mailbox Driver: */
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/mailbox/include/mailbox_internal.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/* If the define below is enabled, the MSS will receive a message
   from BSS during the MSS-DSS message exchange.
   If the define is not enabled only the MSS-DSS messages will happen.
*/   
#define MAILBOX_TEST_BSS_DRIVER_INSTANCE 1

/* *************** Test definition: ***************************************************
1. MSS and DSS come out of reset and initialize mailbox driver.
2. DSS loops, waiting for message 1 from MSS
3. MSS sends message 1 to DSS and MSS waits for message 2 from DSS
4. DSS receives message 1 from MSS, DSS confirms that content matches expected values
5. DSS sends message 2 to MSS
6. MSS receives message 2 from DSS, MSS confirms that content matches expected values
   (if MAILBOX_TEST_BSS_DRIVER_INSTANCE is defined, MSS will receive a message from BSS
    when MSS is in the middle of processing the DSS message of step 6)
7. Steps 2-6 are repeated with another pattern, this time only 1 word message each way
   (no BSS interruption this time around)
8. MSS and DSS declare test PASS or FAIL
*/


/**************************************************************************
 *************************** Mailbox Test Functions **************************
 **************************************************************************/
#ifdef MAILBOX_TEST_BSS_DRIVER_INSTANCE
void simulateRxMsgFromBss (Mbox_Handle  handle);

volatile uint32_t testAppCallbackFlagBss=0;
static void Test_appCallbackFunctionBss(Mbox_Handle handle, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlagBss=1;
}
#endif

volatile uint32_t testAppCallbackFlag=0;
static void Test_appCallbackFunction(Mbox_Handle handle, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlag=1;
}

SOC_Handle      socHandle;

extern int32_t SOC_isMMWaveDSSOperational (SOC_Handle handle, int32_t* errCode);
extern int32_t SOC_setMMWaveDSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);
extern int32_t SOC_isMMWaveMSSOperational (SOC_Handle handle, int32_t* errCode);
extern int32_t SOC_setMMWaveMSSLinkState(SOC_Handle handle, uint8_t state, int32_t* errCode);

void Test_mssWaitSync(void)
{
    int32_t  errCode;
    int32_t  retVal = 0;
    
    printf("MSS: Sync starting...\n");
    if(socHandle == NULL)
    {
        printf("MSS: ERROR! soc handle NULL in Sync fnx...\n");
    }
    SOC_setMMWaveMSSLinkState (socHandle, 1U, &errCode);
    
    while (retVal == 0)
    {
        retVal = SOC_isMMWaveDSSOperational(socHandle,&errCode);
        Task_sleep(10);
    }
    
    SOC_setMMWaveDSSLinkState (socHandle, 0U, &errCode);
}

/**************************************************************************
 * Multichannel Test globals start
 **************************************************************************/
/* MSS will open 4 channels to DSS:
    chId    readMode   writeMode
    1       callback   polling
    3       callback   blocking
    4       blocking   blocking
 */

/*Global array to keep handles of channels between mss and dss*/ 
Mbox_Handle  handleArray[MAILBOX_CH_ID_MAX + 1];

void multiChannelTest (void);
void bigTransferTest (void);

volatile uint32_t testAppCallbackFlag1=0;
static void Test_appCallbackFunction1(Mbox_Handle handle, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlag1=1;
}
volatile uint32_t testAppCallbackFlag3=0;
static void Test_appCallbackFunction3(Mbox_Handle handle, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlag3=1;
}

/**************************************************************************
 * Multichannel Test globals end
 **************************************************************************/


uint32_t gTestPatternWordReceive_0=0xAABBCCDD;
uint32_t gTestPatternWordReceive_1=0x01020304;
uint32_t gTestPatternWordReceive_2=0x05060708;
uint32_t gTestPatternWordReceive_3=0xFA12FA12;

uint32_t gTestPatternWordSend_0=0x11111111;
uint32_t gTestPatternWordSend_1=0x22222222;
uint32_t gTestPatternWordSend_2=0xFFFFFFFF;
uint32_t gTestPatternWordSend_3=0x88887777;

uint32_t volatile gTestFailFlag = 0;
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
    Mbox_Handle  handleDss;
#ifdef MAILBOX_TEST_BSS_DRIVER_INSTANCE
    Mbox_Handle  handleBss;
#endif    
    int32_t         errCode;
    uint32_t        bufferRx;
    uint32_t        bufferTx[3];
    uint32_t        size;
    Mailbox_Driver *driver;
    
    Test_mssWaitSync();
    
    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_MSS);

    /* Setup the default Mailbox Parameters */
    Mailbox_Config_init(&cfg);
    cfg.readMode = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction;

    /* Open the  Instance to DSS */
    handleDss = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handleDss == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        return;
    }
    printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleDss);    

#ifdef MAILBOX_TEST_BSS_DRIVER_INSTANCE
    /* Setup the default Mailbox Parameters */
    Mailbox_Config_init(&cfg);
    cfg.readMode = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunctionBss;

    /* Open the  Instance to BSS */
    handleBss = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handleBss == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        return;
    }
    printf("MSS: Mailbox Instance to BSS %p has been opened successfully\n", handleBss);    
#endif
    /**************************************************************************
     * Test: Write message to DSS
     **************************************************************************/
    printf("MSS: ************ Writing message 1 to DSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_0, sizeof(gTestPatternWordSend_0)); 
    memcpy ((void *)(&bufferTx[1]), (void *)&gTestPatternWordSend_1, sizeof(gTestPatternWordSend_1)); 
    memcpy ((void *)(&bufferTx[2]), (void *)&gTestPatternWordSend_2, sizeof(gTestPatternWordSend_2)); 
    
    size = Mailbox_write(handleDss, (uint8_t*)bufferTx, sizeof(bufferTx));
    printf("MSS: Ack received from DSS for message 1. Size=%d\n",size);
    if(size != sizeof(bufferTx))
    {
        printf("MSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }


    /**************************************************************************
     * Test: Wait for message from DSS
     **************************************************************************/
    printf("MSS: ************ Waiting for message 2 from DSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0){}
    printf("MSS: Message 2 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_0)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }
    
#ifdef MAILBOX_TEST_BSS_DRIVER_INSTANCE    
    /************* test MSS<->BSS connection ******/
    printf("MSS: Receive now msg from BSS =================================\n");
    /* simulate a received message from BSS*/
    simulateRxMsgFromBss(handleBss);        
    printf("MSS: Finished processing msg from BSS =========================\n");
    /************* end of test MSS<->BSS connection ******/

    driver = (Mailbox_Driver*)handleDss;
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 1                               ||  
        driver->rxCount           != 1                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 4                               ||
        driver->boxFullIsrCount   != 1                               ||
        driver->boxEmptyIsrCount  != 1                               ||
        driver->readFlushCount    != 0)                               
    {
        printf("MSS: Error. Stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
    else
    {
        printf("MSS: stats OK for DSS mailbox instance.\n");
    }
#endif
    
    /*read second word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_1)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    /*read third word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_2)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }

    printf("MSS: Flushing read\n");
    Mailbox_readFlush(handleDss);
    testAppCallbackFlag = 0;
    
    /**************************************************************************
     * Test: Write message to DSS
     **************************************************************************/
    printf("MSS: ************ Writing message 3 to DSS ****************\n");
    memset ((void *)bufferTx, 0, sizeof(bufferTx));
    memcpy ((void *)(&bufferTx[0]), (void *)&gTestPatternWordSend_3, sizeof(gTestPatternWordSend_3)); 
    size = Mailbox_write(handleDss, (uint8_t*)bufferTx, sizeof(gTestPatternWordSend_3));
    printf("MSS: Ack received from DSS for message 3. Size=%d\n",size);
    if(size != sizeof(gTestPatternWordSend_3))
    {
        printf("MSS: Error. Write failed.\n");
        gTestFailFlag = 1;
    }

    /**************************************************************************
     * Test: Wait for message from DSS
     **************************************************************************/
    printf("MSS: ************ Waiting for message 4 from DSS ****************\n");

    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0){}
    printf("MSS: Message 4 received.\n");

    /*read first word*/
    bufferRx = 0;
    size = Mailbox_read(handleDss, (uint8_t *)&bufferRx, 4);
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_3)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
    }        

    printf("MSS: Flushing read\n");
    Mailbox_readFlush(handleDss);
    testAppCallbackFlag = 0;


    /**************************************************************************
     * Test: Check driver internal stats
     **************************************************************************/
    driver = (Mailbox_Driver*)handleDss;
    printf("MSS: Checking driver internal stats\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 2                               ||  
        driver->rxCount           != 2                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 2                               ||
        driver->boxEmptyIsrCount  != 2                               ||
        driver->readFlushCount    != 2)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
    else
    {
        printf("MSS: stats OK for DSS mailbox instance.\n");
    }

#ifdef MAILBOX_TEST_BSS_DRIVER_INSTANCE
    driver = (Mailbox_Driver*)handleBss;
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 0                               ||  
        driver->rxCount           != 1                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 1                               ||
        driver->boxEmptyIsrCount  != 0                               ||
        driver->readFlushCount    != 1)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
    else
    {
        printf("MSS: stats OK for BSS mailbox instance.\n");
    }

    if (Mailbox_close(handleBss) != 0)
    {    
        printf("MSS: Error: Failed to close Bss mailbox\n");
        gTestFailFlag = 1;
        return;
    }

#endif     

    if (Mailbox_close(handleDss) != 0)
    {    
        printf("MSS: Error: Failed to close Dss mailbox\n");
        gTestFailFlag = 1;
        return;
    }

    /**************************************************************************
     * Multi-channel Test
     **************************************************************************/
//    Task_sleep(10);
    multiChannelTest();
    
    /**************************************************************************
     * Test 2KB message
     **************************************************************************/
    Test_mssWaitSync();
    bigTransferTest();    
    
    /**************************************************************************
     * Test END
     **************************************************************************/
    printf("MSS: TEST ENDED\n");
    if(gTestFailFlag == 0)
    {
        MCPI_setFeatureTestResult("MSS: MSS_DSS_msg_exchange", MCPI_TestResult_PASS);
        printf("MSS: Test Passed\n");
    }    
    else
    {
        MCPI_setFeatureTestResult("MSS: MSS_DSS_msg_exchange", MCPI_TestResult_FAIL);
        printf("MSS: Test Failed\n");
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

    printf("\nMSS: Starting %dKB transfer test.\n",BIG_MSG_SIZE);
    
    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("MSS: Error: Unable to initialize configuration.\n");
        gTestFailFlag = 1;
        return;
    }

    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_2;/*use ID not used until now even though channels are closed at this point*/

    handle = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handle);    
    
    /*Initialize message to be transferred*/
    for(i=0;i<BIG_MSG_SIZE;i++)
    {
        txBigData[i] = i % 0xFF;
    }

    printf("MSS: ************ Writing big message to DSS ****************\n");
    size = Mailbox_write(handle, (uint8_t*)txBigData, BIG_MSG_SIZE);
    if(size != BIG_MSG_SIZE)
    {
        printf("MSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }
    else
    {
        printf("MSS: Transfer completed.\n");        
    }

    /*initialize rx buffer */
    memset(rxBigData,0,BIG_MSG_SIZE);
    
    System_printf("MSS: Waiting for big message to arrive...\n");    
    size = Mailbox_read(handle, (uint8_t *)rxBigData, BIG_MSG_SIZE);
    System_printf("MSS: Read %d bytes. \n",size);
    
    /*check for received pattern*/
    for(i=0;i<BIG_MSG_SIZE;i++)
    {
        if(rxBigData[i] != (i + 0xF)%0xFF)
        {
            System_printf("MSS: Error: Byte Position %d. Received %d Expected %d. \n",i,rxBigData[i], (i % 0xFF));
            gTestFailFlag = 1;
        }
    }
    Mailbox_readFlush(handle);

    System_printf("MSS: Message received. \n");


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
        printf("MSS: Channel 1 received message.\n");

        /*read first word*/
        bufferRx = 0;
        size = Mailbox_read(handleArray[1], (uint8_t *)&bufferRx, 4);
        printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_1)
        {
            printf("MSS: Error. Pattern mismatch.\n");
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
        printf("MSS: Channel 3 received message.\n");

        /*read first word*/
        bufferRx = 0;
        size = Mailbox_read(handleArray[3], (uint8_t *)&bufferRx, 4);
        printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_3)
        {
            printf("MSS: Error. Pattern mismatch.\n");
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
        printf("MSS: Channel 4 received message.\n");
        printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
        if (bufferRx != gTestPatternWordReceive_2)
        {
            printf("MSS: Error. Pattern mismatch.\n");
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
    int32_t         errCode,i,size;
    Task_Params        taskParams;
    uint32_t        bufferRx;
    Mailbox_Driver *driver;    

    printf("*************************************************\n");
    printf("\nMSS: Starting Multichannel Test.\n");
    
    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("Error: Unable to initialize configuration.\n");
        gTestFailFlag = 1;
        return;
    }
    /****** ch 1 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_1;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction1;
    cfg.writeMode    = MAILBOX_MODE_POLLING;

    handleArray[1] = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handleArray[1] == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[1]);    

    /****** ch 3 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_3;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction3;
    cfg.writeMode    = MAILBOX_MODE_BLOCKING;

    handleArray[3] = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handleArray[3] == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[3]);    

    /****** ch 4 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_4;
    cfg.readMode     = MAILBOX_MODE_BLOCKING;
    cfg.writeMode    = MAILBOX_MODE_BLOCKING;

    handleArray[4] = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handleArray[4] == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[4]);    

    /****** ch 7 *********************************************/
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_7;
    cfg.readMode     = MAILBOX_MODE_BLOCKING;
    cfg.writeMode    = MAILBOX_MODE_BLOCKING;

    handleArray[7] = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handleArray[7] == NULL)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance\n");
        gTestFailFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        printf("MSS: Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        gTestFailFlag = 1;
        return;
    }
    printf("MSS: Mailbox Instance to DSS %p has been opened successfully\n", handleArray[7]);    

    /***************************************************/

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
    //Task_sleep(10);
    Test_mssWaitSync();

    
    for(i=0;i<3;i++)
    {
        /* channels 3 and 4 are write blocking so can write back to back*/
        printf("MSS: ************ Writing a message in CHANNEL 3 to DSS ****************\n");
        size = Mailbox_write(handleArray[3], (uint8_t*)&gTestPatternWordSend_3, sizeof(gTestPatternWordSend_3));
        if(size != sizeof(gTestPatternWordSend_3))
        {
            printf("MSS: Error. Write failed. Error=%d\n",size);
            gTestFailFlag = 1;
        }

        printf("MSS: ************ Writing a message in CHANNEL 4 to DSS ****************\n");
        size = Mailbox_write(handleArray[4], (uint8_t*)&gTestPatternWordSend_2, sizeof(gTestPatternWordSend_2));
        if(size != sizeof(gTestPatternWordSend_2))
        {
            printf("MSS: Error. Write failed. Error=%d\n",size);
            gTestFailFlag = 1;
        }

    }    

    printf("MSS: ************ Writing a message in CHANNEL 1 to DSS ****************\n");
    size = Mailbox_write(handleArray[1], (uint8_t*)&gTestPatternWordSend_1, sizeof(gTestPatternWordSend_1));
    if(size != sizeof(gTestPatternWordSend_1))
    {
        printf("MSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }

    /*give time for remote endpoint to ack the last write which is a polling write*/
    Test_mssWaitSync();
    Task_sleep(100);
    printf("MSS: ************ Writing a message in CHANNEL 7 to DSS ****************\n");
    size = Mailbox_write(handleArray[7], (uint8_t*)&gTestPatternWordSend_0, sizeof(gTestPatternWordSend_0));
    if(size != sizeof(gTestPatternWordSend_0))
    {
        printf("MSS: Error. Write failed. Error=%d\n",size);
        gTestFailFlag = 1;
    }
    
    /* Multichannel test end once a message is received in channel 7 which is readBlocking*/
    size = Mailbox_read(handleArray[7], (uint8_t *)&bufferRx, 4);
    printf("MSS: Channel 7 received message.\n");
    printf("MSS: Read %d bytes. Val=0x%x \n",size, bufferRx);
    if (bufferRx != gTestPatternWordReceive_0)
    {
        printf("MSS: Error. Pattern mismatch.\n");
        gTestFailFlag = 1;
        return;
    }
    Mailbox_readFlush(handleArray[7]);
    
    /*check channel stats*/
    /*channel 1*/
    driver = (Mailbox_Driver*)handleArray[1];
    printf("MSS: Checking driver internal stats for channel 1\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 1                               ||  
        driver->rxCount           != 1                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 1                               ||
        driver->boxEmptyIsrCount  != 1                               ||
        driver->readFlushCount    != 1)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*channel 3*/
    driver = (Mailbox_Driver*)handleArray[3];
    printf("MSS: Checking driver internal stats for channel 3\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 3                               ||  
        driver->rxCount           != 3                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 3                               ||
        driver->boxEmptyIsrCount  != 3                               ||
        driver->readFlushCount    != 3)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*channel 4*/
    driver = (Mailbox_Driver*)handleArray[4];
    printf("MSS: Checking driver internal stats for channel 4\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 3                               ||  
        driver->rxCount           != 3                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 3                               ||
        driver->boxEmptyIsrCount  != 3                               ||
        driver->readFlushCount    != 3)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*channel 7*/
    driver = (Mailbox_Driver*)handleArray[7];
    printf("MSS: Checking driver internal stats for channel 7\n");
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 1                               ||  
        driver->rxCount           != 1                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 1                               ||
        driver->boxEmptyIsrCount  != 1                               ||
        driver->readFlushCount    != 1)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }

    /*check global variables*/
    if(gMailboxMCB.bssInstCount != 0)
    {
        printf("MSS: Error: gMailboxMCB.bssInstCount=%d\n",gMailboxMCB.bssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.mssDssInstCount != 4)
    {
        printf("MSS: Error: gMailboxMCB.mssDssInstCount=%d\n",gMailboxMCB.mssDssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.initFlag != 1)
    {
        printf("MSS: Error: gMailboxMCB.initFlag=%d\n",gMailboxMCB.initFlag);
        gTestFailFlag = 1;
    }
    if((gMailboxMCB.errCnt.bssFull != 0) || (gMailboxMCB.errCnt.bssEmpty != 0) || (gMailboxMCB.errCnt.mssDssFull != 0) || (gMailboxMCB.errCnt.mssDssEmpty != 0))
    {
        printf("MSS: Error: gMailboxMCB.errCnt.bssFull=%d\n",gMailboxMCB.errCnt.bssFull);
        printf("MSS: Error: gMailboxMCB.errCnt.bssEmpty=%d\n",gMailboxMCB.errCnt.bssEmpty);
        printf("MSS: Error: gMailboxMCB.errCnt.mssDssFull=%d\n",gMailboxMCB.errCnt.mssDssFull);
        printf("MSS: Error: gMailboxMCB.errCnt.mssDssEmpty=%d\n",gMailboxMCB.errCnt.mssDssEmpty);
        gTestFailFlag = 1;
    }
    
    /*close all channels*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        /* Open the  Instance */
        if((i==1) || (i==3) || (i==4) || (i==7))
        {
            if (Mailbox_close(handleArray[i]) != 0)
            {    
                printf("MSS: Error: Failed to close instance %d\n",i);
                gTestFailFlag = 1;
            }
            printf("Debug: closed instance %d\n",i);
        }
    }    
    
    /*check global variables*/
    if(gMailboxMCB.bssInstCount != 0)
    {
        printf("MSS: Error: gMailboxMCB.bssInstCount=%d\n",gMailboxMCB.bssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.mssDssInstCount != 0)
    {
        printf("MSS: Error: gMailboxMCB.mssDssInstCount=%d\n",gMailboxMCB.mssDssInstCount);
        gTestFailFlag = 1;
    }
    if(gMailboxMCB.initFlag != 1)
    {
        printf("MSS: Error: gMailboxMCB.initFlag=%d\n",gMailboxMCB.initFlag);
        gTestFailFlag = 1;
    }

    if((gMailboxMCB.hwiHandles.bssFull != 0) || (gMailboxMCB.hwiHandles.bssEmpty != 0) || (gMailboxMCB.hwiHandles.mssDssFull != 0) || (gMailboxMCB.hwiHandles.mssDssEmpty != 0))
    {
        printf("MSS: Error: gMailboxMCB.hwiHandles.bssFull=%d\n",gMailboxMCB.hwiHandles.bssFull);
        printf("MSS: Error: gMailboxMCB.hwiHandles.bssEmpty=%d\n",gMailboxMCB.hwiHandles.bssEmpty);
        printf("MSS: Error: gMailboxMCB.hwiHandles.mssDssFull=%d\n",gMailboxMCB.hwiHandles.mssDssFull);
        printf("MSS: Error: gMailboxMCB.hwiHandles.mssDssEmpty=%d\n",gMailboxMCB.hwiHandles.mssDssEmpty);
        gTestFailFlag = 1;
    }
    
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        if(gMailboxMCB.handleArray[i] != NULL)
        {
            printf("MSS: Error: gMailboxMCB.handleArray[%d]=0x%x is not NULL\n",i, gMailboxMCB.handleArray[i]);
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
    int32_t         errCode;
    int32_t         retVal;
    SOC_Cfg         socCfg;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    ESM_init(0U);
    /* Initialize test logger */
    MCPI_Initialize();
    

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: On the EVM we want to ensure that the BSS is
     * executing and the system clock is correctly configured. */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);

    if (socHandle == NULL)
    {
        printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /*
    SOC_init() unhalts the BSS. The mailbox unit test assumes that the BSS is halted. 
    The mailbox is tested with BSS unhalted as part of the mmwavelink test.
    For the mailbox unit test the BSS must be halted so we can read/write arbitrarily in the 
    mailbox without being disturbed by BSS or BSS returning errors.
    */

    retVal = SOC_haltBSS(socHandle, &errCode);
    if (retVal < 0)
    {
        /* FATAL Error: Halt the BSS failed. This has to be reported to the application */
        printf("Error: Halt the BSS failed [Error code %d]\n", errCode);
        DebugP_assert (0);
    }
    else
    {
        /* Success: BSS has been halted successfully. */
        printf("Debug: Halted the BSS\n");
    }


    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 4*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}

#ifdef MAILBOX_TEST_BSS_DRIVER_INSTANCE

uint32_t gTestBssPatternWord_0=0xAAAAAAAA;
uint32_t gTestBssPatternWord_1=0xBBBBBBBB;
uint32_t gTestBssPatternWord_2=0xCCCCCCCC;

void simulateRxMsgFromBss (Mbox_Handle  handle)
{
    Mailbox_Driver*   driver;
    uint32_t*         dataPtr;
    uint32_t          buffer;
    int32_t           size;

    driver = (Mailbox_Driver*)handle;
    
    /*before receiving the message check stats for this instance*/
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 0                               ||  
        driver->rxCount           != 0                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 0                               ||
        driver->boxEmptyIsrCount  != 0                               ||
        driver->readFlushCount    != 0)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
    else
    {
        printf("MSS: stats OK for BSS mailbox instance - before message.\n");
    }

    
    /*fill in pattern into mailbox data buffer to be read by the local mailbox 
      as if this was written by the remote mailbox*/
    dataPtr = (uint32_t *)((driver->hwCfg)->baseRemoteToLocal.data);
    memcpy ((void *)(&dataPtr[0]), (void *)&gTestBssPatternWord_0, sizeof(gTestBssPatternWord_0)); 
    memcpy ((void *)(&dataPtr[1]), (void *)&gTestBssPatternWord_1, sizeof(gTestBssPatternWord_1)); 
    memcpy ((void *)(&dataPtr[2]), (void *)&gTestBssPatternWord_2, sizeof(gTestBssPatternWord_2));     
      
    /*trigger a fake "mailbox full" interrupt from remote to local endpoint*/
    ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_TRIG = 0x1U;
    
    /* wait for call back to set flag */
    while (testAppCallbackFlagBss == 0){}
    
    printf("Received msg from BSS. Call back finished\n");

    /*read first word*/
    memset ((void *)&buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, (uint8_t*)&buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, buffer);
    if (buffer != gTestBssPatternWord_0)
    {
        printf("MSS: Error. Pattern mismatch from BSS read.\n");
        gTestFailFlag = 1;
    }        

    /*read word 2*/
    memset ((void *)&buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, (uint8_t*)&buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, buffer);
    if (buffer != gTestBssPatternWord_1)
    {
        printf("MSS: Error. Pattern mismatch from BSS read.\n");
        gTestFailFlag = 1;
    }    

    /*read word 3*/
    memset ((void *)&buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, (uint8_t*)&buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, buffer);
    if (buffer != gTestBssPatternWord_2)
    {
        printf("MSS: Error. Pattern mismatch from BSS read.\n");
        gTestFailFlag = 1;
    }

    printf("Flushing read\n");
    Mailbox_readFlush(handle);

    /*check stats for this instance*/
    if( driver->txBoxStatus       != MAILBOX_TX_BOX_EMPTY            ||
        driver->txCount           != 0                               ||  
        driver->rxCount           != 1                               ||  
        driver->newMessageFlag    != MAILBOX_NEW_MESSAGE_NOT_RECEIVED||
        driver->numBytesRead      != 0                               ||
        driver->boxFullIsrCount   != 1                               ||
        driver->boxEmptyIsrCount  != 0                               ||
        driver->readFlushCount    != 1)                               
    {
        printf("MSS: Error. Internal stats failed.\n");
        printf("MSS: txBoxStatus      =%d\n",driver->txBoxStatus     );
        printf("MSS: txCount          =%d\n",driver->txCount         );
        printf("MSS: rxCount          =%d\n",driver->rxCount         );
        printf("MSS: newMessageFlag   =%d\n",driver->newMessageFlag  );
        printf("MSS: numBytesRead     =%d\n",driver->numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d\n",driver->boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d\n",driver->boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d\n",driver->readFlushCount  );
        gTestFailFlag = 1;
    }
    else
    {
        printf("MSS: stats OK for BSS mailbox instance - after message.\n");
    }
        

}
#endif
