/*
 *   @file  main_mss_standalone.c
 *
 *   @brief
 *      Unit Test code for the Mailbox MSS only
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

int32_t testFailedFlag = 0;
/**************************************************************************
 *************************** Mailbox Test Functions **************************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      The function is used to test the Mailbox Driver *WRITE* functions with
 *      various invalid arguments.
 *
 *  @param[in]  handle
 *      Mailbox Handle
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t Test_mssDssMultiChannelOpen(void)
{
    Mailbox_Config  cfg;
    Mbox_Handle  handle;
    int32_t         errCode, size;
    int32_t         i;
    Mbox_Handle  handleArray[MAILBOX_CH_ID_MAX + 1];
    uint32_t        buffer;

    /*Try opening the same channel twice */
    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("Error: Unable to initialize configuration.\n");
        testFailedFlag = 1;
    }
    
    cfg.chType = MAILBOX_CHTYPE_MULTI;
    cfg.chId   = MAILBOX_CH_ID_0;

    /* Open the  Instance */
    handle = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return -1;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return -1;
    }
    handleArray[0] = handle;
    printf("Debug: Mailbox Instance %p has been opened successfully\n", handle);
    /* Open the second  Instance */
    handle = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Expected: Unable to open the Mailbox Instance for same channel ID\n");
        if(errCode != MAILBOX_EBADCHID)
        {
            printf("Error: bad error code\n");
            testFailedFlag = 1;
            return -1;
        }
    }
    else
    {
        printf("Error: Opened the same channel twice\n");
        testFailedFlag = 1;
        return -1;
    }
    
    /* Try opening channel with different channel type */
    cfg.chType = MAILBOX_CHTYPE_SINGLE;
    cfg.chId   = MAILBOX_CH_ID_1;
    handle = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Expected: Unable to open the Mailbox Instance due to different channel type\n");
        if(errCode != MAILBOX_EBADCHTYPE)
        {
            printf("Error: bad error code\n");
            testFailedFlag = 1;
            return -1;
        }
    }
    else
    {
        printf("Error: Opened channel with different type\n");
        testFailedFlag = 1;
        return -1;
    }

    cfg.chType = MAILBOX_CHTYPE_MULTI;
    for(i=1;i<=MAILBOX_CH_ID_MAX;i++)
    {
        cfg.chId   = (Mailbox_ChID)i;

        /* Open the  Instance */
        handle = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
        if (handle == NULL)
        {
            printf("Error: Unable to open the Mailbox Instance\n");
            testFailedFlag = 1;
            return -1;
        }
        if (errCode != 0)
        {
            printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
            testFailedFlag = 1;
            return -1;
        }
        handleArray[i] = handle;
        printf("Debug: Mailbox Instance %p has been opened successfully\n", handle);
    }    
    
    /*delete all instances in different order*/
    for(i=MAILBOX_CH_ID_MAX;i>=0;i--)
    {
        /* Close the  Instance */
        if (Mailbox_close(handleArray[i]) != 0)
        {    
            printf("Failed to close instance %d\n",i);
            testFailedFlag = 1;
            return -1;
        }
        printf("Debug: closed instance %d\n",i);
    }    
    printf("Debug: all instances closed\n");
    
    /* open all channels again*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        cfg.chId   = (Mailbox_ChID)i;

        /* Open the  Instance */
        handle = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
        if (handle == NULL)
        {
            printf("Error: Unable to open the Mailbox Instance\n");
            testFailedFlag = 1;
            return -1;
        }
        if (errCode != 0)
        {
            printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
            testFailedFlag = 1;
            return -1;
        }
        handleArray[i] = handle;
        printf("Debug: Mailbox Instance %p has been opened successfully\n", handle);
    }    

    /*delete all instances*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        /* Close the  Instance */
        if (Mailbox_close(handleArray[i]) != 0)
        {    
            printf("Failed to close instance %d\n",i);
            testFailedFlag = 1;
            return -1;
        }
        printf("Debug: closed instance %d\n",i);
    }    
    printf("Debug: all instances closed\n");

    /* open all channels again*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        cfg.chId   = (Mailbox_ChID)i;
        cfg.readMode = MAILBOX_MODE_POLLING;
        cfg.writeMode = MAILBOX_MODE_POLLING;

        /* Open the  Instance */
        handle = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
        if (handle == NULL)
        {
            printf("Error: Unable to open the Mailbox Instance\n");
            testFailedFlag = 1;
            return -1;
        }
        if (errCode != 0)
        {
            printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
            testFailedFlag = 1;
            return -1;
        }
        handleArray[i] = handle;
        printf("Debug: Mailbox Instance %p has been opened successfully\n", handle);
    }    

    /* read from all channels*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
          printf("Debug: Reading from mailbox %d\n", i);
       size = Mailbox_read(handleArray[i], (uint8_t*)&buffer, sizeof(buffer));
       printf("size=%d\n",size);
    }    

    /* write to all channels*/
    printf("Debug: Writing to mailbox %d\n", MAILBOX_CH_ID_MAX);
    buffer = 0x1234ABCD;
    size = Mailbox_write(handleArray[MAILBOX_CH_ID_MAX], (uint8_t*)&buffer, sizeof(buffer));
    printf("size=%d\n",size);
    for(i=0;i<=MAILBOX_CH_ID_MAX-1;i++)
    {
       printf("Debug: Writing to mailbox %d\n", i);
       size = Mailbox_write(handleArray[i], (uint8_t*)&buffer, sizeof(buffer));
       printf("size=%d\n",size);
    }
    #if 0
    /*check if mailbox buffer contains correct data*/
    if((uint32_t)(*(uint32_t*)0xF0604000) != MAILBOX_CH_ID_MAX)
    {
        printf("Error: CH ID mismatch. Mailbox TX memory value =0x%x and expected value is 0x%x\n",(uint32_t)((uint32_t*)0xF0604000),MAILBOX_CH_ID_MAX);
        testFailedFlag = 1;
        return -1;
    }
    if((uint32_t)(*(uint32_t*)0xF0604004) != buffer)
    {
        printf("Error: CH ID mismatch. Mailbox TX memory value =0x%x and expected value is 0x%x\n",(uint32_t)((uint32_t*)0xF0604004),buffer);
        testFailedFlag = 1;
        return -1;
    }
    printf("Mailbox buffer correct!\n");
    

    while(1)
    {
    }
    #endif

    /*delete all instances*/
    for(i=0;i<=MAILBOX_CH_ID_MAX;i++)
    {
        /* Open the  Instance */
        if (Mailbox_close(handleArray[i]) != 0)
        {    
            printf("Failed to close instance %d\n",i);
            testFailedFlag = 1;
            return -1;
        }
        printf("Debug: closed instance %d\n",i);
    }    
    printf("Debug: all instances closed\n");
    
    return 0;
}
 
/**
 *  @b Description
 *  @n
 *      The function is used to test the Mailbox Driver *WRITE* functions with
 *      various invalid arguments.
 *
 *  @param[in]  handle
 *      Mailbox Handle
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t Test_writeInvalidArgs(Mbox_Handle handle)
{
    uint8_t     buffer[4];
    int32_t     status;

    /**************************************************************************
     * Test: Write with invalid buffer
     **************************************************************************/
    status = Mailbox_write(handle, NULL, 128);
    if (status != MAILBOX_EINVAL)
    {
        printf("Error: Mailbox Write with invalid buffer returned %d\n", status);
        testFailedFlag = 1;
        return -1;
    }
    printf("Debug: Mailbox Write with invalid buffer passed\n");

    /**************************************************************************
     * Test: Write with invalid size
     **************************************************************************/
    status = Mailbox_write(handle, &buffer[0], 0);
    if (status != MAILBOX_EINVAL)
    {
        printf("Error: Mailbox Write with invalid size returned %d\n", status);
        testFailedFlag = 1;
        return -1;
    }
    printf("Debug: Mailbox Write with invalid size passed\n");

    /**************************************************************************
     * Test: Write with invalid buffer and size
     **************************************************************************/
    status = Mailbox_write(handle, NULL, 0);
    if (status != MAILBOX_EINVAL)
    {
        printf("Error: Mailbox Write with invalid buffer & size returned %d\n", status);
        testFailedFlag = 1;
        return -1;
    }
    printf("Debug: Mailbox Write with invalid buffer & size passed\n");


    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test the Mailbox Driver *READ* functions with
 *      various invalid arguments.
 *
 *  @param[in]  handle
 *      Mailbox Handle
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t Test_readInvalidArgs(Mbox_Handle handle)
{
    uint8_t     buffer[4];
    int32_t     status;

    /**************************************************************************
     * Test: Read with invalid buffer
     **************************************************************************/
    status = Mailbox_read(handle, NULL, 128);
    if (status != MAILBOX_EINVAL)
    {
        printf("Error: Mailbox Read with invalid buffer returned %d\n", status);
        testFailedFlag = 1;
        return -1;
    }
    printf("Debug: Mailbox Read with invalid buffer passed\n");

    /**************************************************************************
     * Test: Read with invalid size
     **************************************************************************/
    status = Mailbox_read(handle, &buffer[0], 0);
    if (status != MAILBOX_EINVAL)
    {
        printf("Error: Mailbox Read with invalid size returned %d\n", status);
        testFailedFlag = 1;
        return -1;
    }
    printf("Debug: Mailbox Read with invalid size passed\n");

    /**************************************************************************
     * Test: Read with invalid buffer and size
     **************************************************************************/
    status = Mailbox_read(handle, NULL, 0);
    if (status != MAILBOX_EINVAL)
    {
        printf("Error: Mailbox Read with invalid buffer & size returned %d\n", status);
        testFailedFlag = 1;
        return -1;
    }
    printf("Debug: Mailbox Read with invalid buffer & size passed\n");

    return 0;
}

#if 0
static void Test_printStatus(Mbox_Handle handle)
{
    Mailbox_Stats       stats;
    
    if(Mailbox_getStats(handle, &stats) < 0)
    {
        printf("Error: Mailbox_getStats failed\n"); 
    }
    else
    {
        printf("     txBoxStatus         =%d \n", stats.txBoxStatus         );
        printf("     txCount             =%d \n", stats.txCount             );
        printf("     rxCount             =%d \n", stats.rxCount                );
        printf("     newMessageFlag      =%d \n", stats.newMessageFlag      );
        printf("     numBytesRead        =%d \n", stats.numBytesRead        );
        printf("     boxFullIsrCount     =%d \n", stats.boxFullIsrCount     );
        printf("     boxEmptyIsrCount    =%d \n", stats.boxEmptyIsrCount    );
        printf("     readFlushCount       =%d \n", stats.readFlushCount      );
        printf("     boxEmptyIsrErrCount =%d \n", stats.boxEmptyIsrErrCount );
        printf("     boxFullIsrErrCount  =%d \n", stats.boxFullIsrErrCount  );
        printf("     regLclToRemRaw      =%d \n", stats.regLclToRemRaw      ); 
        printf("     regRemToLocRaw      =%d \n", stats.regRemToLocRaw      ); 
        printf("     regLclToRemMask     =%d \n", stats.regLclToRemMask     ); 
        printf("     regRemToLocMask     =%d \n", stats.regRemToLocMask     ); 

    }
}
#endif

static void Test_checkStatus(Mbox_Handle handle, uint32_t txBoxStatus, uint32_t txCount, uint32_t rxCount, uint32_t newMessageFlag,
                             uint32_t numBytesRead, uint32_t boxFullIsrCount, uint32_t boxEmptyIsrCount, uint32_t readFlushCount)
{
    Mailbox_Driver*   driver;
    driver = (Mailbox_Driver*)handle;
    if(    driver->txBoxStatus       != txBoxStatus     ||
        driver->txCount           != txCount         ||  
        driver->rxCount           != rxCount         ||  
        driver->newMessageFlag    != newMessageFlag  ||
        driver->numBytesRead      != numBytesRead    ||
        driver->boxFullIsrCount   != boxFullIsrCount ||
        driver->boxEmptyIsrCount  != boxEmptyIsrCount||
        driver->readFlushCount    != readFlushCount )
    {
        testFailedFlag = 1;    
        printf("MSS: Error. Stats failed.\n");
        printf("MSS: txBoxStatus      =%d expected value is %d\n",driver->txBoxStatus     ,txBoxStatus     );
        printf("MSS: txCount          =%d expected value is %d\n",driver->txCount         ,txCount         );
        printf("MSS: rxCount          =%d expected value is %d\n",driver->rxCount         ,rxCount         );
        printf("MSS: newMessageFlag   =%d expected value is %d\n",driver->newMessageFlag  ,newMessageFlag  );
        printf("MSS: numBytesRead     =%d expected value is %d\n",driver->numBytesRead    ,numBytesRead    );
        printf("MSS: boxFullIsrCount  =%d expected value is %d\n",driver->boxFullIsrCount ,boxFullIsrCount );
        printf("MSS: boxEmptyIsrCount =%d expected value is %d\n",driver->boxEmptyIsrCount,boxEmptyIsrCount);
        printf("MSS: readFlushCount   =%d expected value is %d\n",driver->readFlushCount  ,readFlushCount  );        
    }    
}        


/**************************************************************************
 * Test read in callback mode for a message sent by a specified remote endpoint 
 **************************************************************************/
volatile uint32_t testAppCallbackFlag=0;

static void Test_appCallbackFunction(Mbox_Handle handle, Mailbox_Type remoteEndpoint)
{
    testAppCallbackFlag=1;
}

uint32_t gTestPatternWord_0=0xAABBCCDD;
uint32_t gTestPatternWord_1=0x01020304;
uint32_t gTestPatternWord_2=0x05060708;

static int32_t Test_readCallback(Mbox_Handle handle, Mailbox_Type remoteEndpoint, uint32_t checkStatus)
{
    Mailbox_Driver*   driver;
    uint32_t*         dataPtr;
    uint8_t           buffer[12];
    int32_t           size;
    
    printf("\nStarting Test_readCallback\n");
    testAppCallbackFlag=0;
    
    driver = (Mailbox_Driver*)handle;
    
    /*fill in pattern into mailbox data buffer to be read by the local mailbox 
      as if this was written by the remote mailbox*/
    dataPtr = (uint32_t *)((driver->hwCfg)->baseRemoteToLocal.data);
    memcpy ((void *)(&dataPtr[0]), (void *)&gTestPatternWord_0, sizeof(gTestPatternWord_0)); 
    memcpy ((void *)(&dataPtr[1]), (void *)&gTestPatternWord_1, sizeof(gTestPatternWord_1)); 
    memcpy ((void *)(&dataPtr[2]), (void *)&gTestPatternWord_2, sizeof(gTestPatternWord_2));     
       
    /*trigger a fake "mailbox full" interrupt from remote to local endpoint*/
    ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_TRIG = 0x1U;
    
    /* wait for call back to set flag */
    while (testAppCallbackFlag == 0){}
    
    printf("Call back finished\n");

    if(checkStatus)
        Test_checkStatus(handle,0,0,0,1,0,1,0,0);

    /*read first word*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    if(size!=4)
        testFailedFlag = 1;
        
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));

    if(checkStatus)
        Test_checkStatus(handle,0,0,1,0,4,1,0,0);

    /*read word 2*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=4)
        testFailedFlag = 1;

    if(checkStatus)
        Test_checkStatus(handle,0,0,1,0,8,1,0,0);

    /*read word 3*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=4)
        testFailedFlag = 1;
    
    if(checkStatus)
        Test_checkStatus(handle,0,0,1,0,12,1,0,0);

    printf("Flushing read\n");
    Mailbox_readFlush(handle);
    
    if(checkStatus)
        Test_checkStatus(handle,0,0,1,0,0,1,0,1);

    /*new read should fail as mailbox was flushed*/
    size = Mailbox_read(handle, buffer, 3);
    printf("After flush size should be zero. Size=%d \n",size);
    
    /*update pattern so function can be called again with different pattern */
    gTestPatternWord_0++;
    gTestPatternWord_1++;
    gTestPatternWord_2++;
    testAppCallbackFlag = 0;
    
    return 0;
}

volatile uint32_t testReadStartFlag=0;

static int32_t Test_readBlocking(Mbox_Handle handle, Mailbox_Type remoteEndpoint, uint32_t triggerInt)
{
    Mailbox_Driver*   driver;
    uint32_t*         dataPtr;
    uint8_t           buffer[12];
    int32_t           size;
    
    printf("\nStarting Test_readBlocking\n");
    
    driver = (Mailbox_Driver*)handle;
    
    /*fill in pattern into mailbox data buffer to be read by the local mailbox 
      as if this was written by the remote mailbox*/
    dataPtr = (uint32_t *)((driver->hwCfg)->baseRemoteToLocal.data);
    memcpy ((void *)(&dataPtr[0]), (void *)&gTestPatternWord_0, sizeof(gTestPatternWord_0)); 
    memcpy ((void *)(&dataPtr[1]), (void *)&gTestPatternWord_1, sizeof(gTestPatternWord_1)); 
    memcpy ((void *)(&dataPtr[2]), (void *)&gTestPatternWord_2, sizeof(gTestPatternWord_2)); 
       
    /*read first word*/
    memset ((void *)buffer, 0, sizeof(buffer));

    if(triggerInt==1)
    {
        testReadStartFlag=1;
    }
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));

    /*read word 2*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=4)
        testFailedFlag = 1;

    /*read word 3*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=4)
        testFailedFlag = 1;

    printf("Flushing read\n");
    Mailbox_readFlush(handle);
    /*update pattern so function can be called again with different pattern */
    gTestPatternWord_0++;
    gTestPatternWord_1++;
    gTestPatternWord_2++;
    
    return 0;
}

static int32_t Test_readPolling(Mbox_Handle handle, Mailbox_Type remoteEndpoint, uint32_t checkStatus)
{
    Mailbox_Driver*   driver;
    uint32_t*         dataPtr;
    uint8_t           buffer[12];
    int32_t           size;
    
    printf("\nStarting Test_readPolling\n");
    
    driver = (Mailbox_Driver*)handle;
    

    /*fill in pattern into mailbox data buffer to be read by the local mailbox 
      as if this was written by the remote mailbox*/
    dataPtr = (uint32_t *)((driver->hwCfg)->baseRemoteToLocal.data);
    memcpy ((void *)(&dataPtr[0]), (void *)&gTestPatternWord_0, sizeof(gTestPatternWord_0)); 
    memcpy ((void *)(&dataPtr[1]), (void *)&gTestPatternWord_1, sizeof(gTestPatternWord_1)); 
    memcpy ((void *)(&dataPtr[2]), (void *)&gTestPatternWord_2, sizeof(gTestPatternWord_2)); 
    
    /* ideally should flush the caches here to make sure we will really read back the data from mailbox*/

    if(checkStatus==0)
        Test_checkStatus(handle,0,0,0,0,0,0,0,0);

    memset ((void *)buffer, 0, sizeof(buffer));

    printf("First two reads should be zero bytes as no interupt has been sent by transmitter \n");
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=0)
        testFailedFlag = 1;
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=0)
        testFailedFlag = 1;

    /*trigger a fake "mailbox full" interrupt from remote to local endpoint*/
    ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_TRIG = 0x1U;

    /*read word 1*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=4)
        testFailedFlag = 1;

    /*read word 2*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=4)
        testFailedFlag = 1;
    
    if(checkStatus==0)
        Test_checkStatus(handle,0,0,1,0,8,1,0,0);

    /*read word 3*/
    memset ((void *)buffer, 0, sizeof(buffer));
    size = Mailbox_read(handle, buffer, 4);
    printf("Size=%d bytes. Val=0x%x \n",size, (uint32_t)(*((uint32_t *)buffer)));
    if(size!=4)
        testFailedFlag = 1;
    
    if(checkStatus==0)
        Test_checkStatus(handle,0,0,1,0,12,1,0,0);

    printf("Flushing read\n");
    Mailbox_readFlush(handle);

    if(checkStatus==0)
        Test_checkStatus(handle,0,0,1,0,0,1,0,1);

    /*update pattern so function can be called again with different pattern */
    gTestPatternWord_0++;
    gTestPatternWord_1++;
    gTestPatternWord_2++;

    return 0;
}

/**************************************************************************
 * Test write in blocking mode
 **************************************************************************/
volatile uint32_t testWriteSentFlag=0;
Mbox_Handle gHandle=NULL;

int32_t Test_writeBlocking(Mbox_Handle handle)
{
    uint32_t          buffer;
    int32_t           size;

    printf("\nStarting Test_writeBlocking\n");
    testWriteSentFlag=0;
    buffer = 0x88FF77EE;/*some pattern to check in the memory window*/
    
    /*store handle to be used by other task to send the ack */
    gHandle = handle;
    testWriteSentFlag=1;
    size = Mailbox_write(handle, (uint8_t*)(&buffer), sizeof(buffer));
    printf("Ack received. Size=%d\n",size);
    
    if(size < 0)
    {
        printf("Error, write failed.\n");
        return -1;
    }
    
    return 0;
}

static int32_t Test_writePolling(Mbox_Handle handle)
{
    uint32_t          buffer;
    int32_t           size;

    printf("\nStarting Test_writePolling\n");
    testWriteSentFlag=0;
    buffer = 0xaaaaaaaa;/*some pattern to check in the memory window*/
    
    /*store handle to be used by other task to send the ack */
    gHandle = handle;
    testWriteSentFlag=1;
    size = Mailbox_write(handle, (uint8_t*)(&buffer), sizeof(buffer));
    printf("Write done. Size=%d\n",size);
    
    if(size < 0)
    {
        printf("Error, write failed.\n");
        return -1;
    }
    
    return 0;
}

#define TASK_SLEEP_TIME 500
static void Test_writeBlockingSendAck(void)
{
    Mailbox_Driver* driver;

    /*once testWriteSentFlag is set to 1, trigger ack interrupt for testing of Mailbox_write()*/
    while(testWriteSentFlag==0)
    {
        Task_sleep(10);
    }
    testWriteSentFlag = 0;
    Task_sleep(TASK_SLEEP_TIME);
    if(gHandle == NULL)
    {
        printf("Error: Test_writeBlockingSendAck: NULL handle\n");
        return;
    }
    //Test_printStatus(gHandle);
    driver = (Mailbox_Driver*)gHandle;
    ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_TRIG = (0X1U << 0X1U);

    printf("Test_writeBlockingSendAck: Ack sent\n");

}

static void Test_readBlockingSendFullInt(void)
{
    Mailbox_Driver* driver;

    while(testReadStartFlag==0)
    {
        Task_sleep(10);
    }
    testReadStartFlag = 0;
    Task_sleep(TASK_SLEEP_TIME);
    if(gHandle == NULL)
    {
        printf("Error: Test_readBlockingSendFullInt: NULL handle\n");
        return;
    }
    //Test_printStatus(gHandle);

    printf("Test_readBlockingSendFullInt: Full interrupt will be sent\n");

    driver = (Mailbox_Driver*)gHandle;
    /*trigger a fake "mailbox full" interrupt from remote to local endpoint*/
    ((driver->hwCfg)->baseRemoteToLocal.reg)->INT_TRIG = 0x1U;


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
    Mbox_Handle  handle, handleTemp;
    int32_t         errCode;
    int32_t         i;
    
    /* Initialize the Mailbox */
    Mailbox_init(MAILBOX_TYPE_MSS);
    
    /* Test multi-channel*/
    if (Test_mssDssMultiChannelOpen() != 0)
        return;

    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("Error: Unable to initialize configuration.\n");
        testFailedFlag = 1;
    }

    /* Open the  Instance */
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return;
    }
    printf("Debug: Mailbox Instance %p has been opened successfully\n", handle);

    /**************************************************************************
     * Test: Graceful shutdown
     **************************************************************************/
    Mailbox_close(handle);
    printf("Debug: Mailbox Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver
     **************************************************************************/
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return;
    }
    printf("Debug: Mailbox Instance %p has been reopened successfully\n", handle);

    /* Test: This will test the Mailbox Write with Invalid arguments */
    if (Test_writeInvalidArgs(handle) != 0)
        return;

    /* Test: This will test the Mailbox Read with Invalid arguments */
    if (Test_readInvalidArgs(handle) != 0)
        return;

    /**************************************************************************
     * Test: read in callback mode
     **************************************************************************/
    printf("\n************ READ IN CALLBACK MODE ****************\n");
    /* close mailbox as it needs to be reopened to be reconfigured */
    Mailbox_close(handle);

    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        testFailedFlag = 1;
        printf("Error: Unable to initialize configuration.\n");
    }
    cfg.readMode = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction;
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return ;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return ;
    }

    if (Test_readCallback(handle, MAILBOX_TYPE_BSS, 1) != 0)
        return;
    
    /**************************************************************************
     * Test: write in blocking mode to the same endpoint as above
     **************************************************************************/
    printf("\n************ WRITE IN BLOCKING MODE ****************\n");
    for(i=0;i<3;i++)
    {
        if (Test_writeBlocking(handle) != 0)
            return;
        if(i==0)    
            Test_checkStatus(handle,0,1,1,0,0,1,1,1);
        if(i==1)    
            Test_checkStatus(handle,0,2,1,0,0,1,2,1);
        if(i==2)    
            Test_checkStatus(handle,0,3,1,0,0,1,3,1);
            
    }        
    
    /********************************************************************
     * Test: Close and open mailbox
     ********************************************************************/
    printf("\n************ CLOSE/OPEN AND RE-OPEN ****************\n");
    printf("Close and Open driver\n");

    Mailbox_close(handle);
    handle = NULL;
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return;
    }
    printf("Debug: Mailbox Instance %p has been opened successfully\n", handle);
    Test_checkStatus(handle,0,0,0,0,0,0,0,0);

    /********************************************************************
     * Test: Try to open again to same remote endpoint. 
     ********************************************************************/
    printf("Trying to reopen the driver for the same endpoint \n");
    handleTemp = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if ((handleTemp != NULL) || (errCode == 0))
    {
        printf("Error: Mailbox opened twice\n");
        testFailedFlag = 1;
    }
    else
    {
        printf("OK, could not reopen\n");
    }

    /**************************************************************************
     * Test: multiple read in callback mode
     **************************************************************************/
    printf("\n************ MULTIPLE READ IN CALLBACK MODE ****************\n");
    /* close mailbox as it needs to be reopened to be reconfigured */
    if (Mailbox_close(handle) != 0)
    {    
        printf("Failed to close\n");
        testFailedFlag = 1;
        return;
    }

    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("Error: Unable to initialize configuration.\n");
        testFailedFlag = 1;
    }
    cfg.readMode = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = Test_appCallbackFunction;
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return ;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return ;
    }

    for(i=0;i<3;i++)
    {
        if (Test_readCallback(handle, MAILBOX_TYPE_BSS, 0) != 0)
            return;
    }        
    Test_checkStatus(handle,0,0,3,0,0,3,0,3);

    /**************************************************************************
     * Test: read in blocking mode
     **************************************************************************/
    printf("\n************ MULTIPLE READ IN BLOCKING MODE ****************\n");
    /* close mailbox as it needs to be reopened to be reconfigured */
    Mailbox_close(handle);

    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("Error: Unable to initialize configuration.\n");
    }
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return ;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return ;
    }

    for(i=0;i<3;i++)
    {
        if (Test_readBlocking(handle, MAILBOX_TYPE_BSS, 1) != 0)
            return;
    }
    Test_checkStatus(handle,0,0,3,0,0,3,0,3);

    printf("\n************ SEND FULL INTERRUPT BEFORE READ IN BLOCKING MODE IS CALLED ****************\n");
    testReadStartFlag=1;
    Task_sleep(TASK_SLEEP_TIME + 500);//make sure interrupt is triggered before test starts
    if (Test_readBlocking(handle, MAILBOX_TYPE_BSS, 0) != 0)
        return;

    Test_checkStatus(handle,0,0,4,0,0,4,0,4);

    /**************************************************************************
     * Test: read in polling mode
     **************************************************************************/
    printf("\n************ MULTIPLE READs IN POLLING MODE ****************\n");
    /* close mailbox as it needs to be reopened to be reconfigured */
    Mailbox_close(handle);

    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("Error: Unable to initialize configuration.\n");
        testFailedFlag = 1;
    }
    cfg.readMode = MAILBOX_MODE_POLLING;
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return ;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return ;
    }

    for(i=0;i<3;i++)
    {
        if (Test_readPolling(handle, MAILBOX_TYPE_BSS, i) != 0)
            return;
    }        

    Test_checkStatus(handle,0,0,3,0,0,3,0,3);

    /**************************************************************************
     * Test: read in polling mode
     **************************************************************************/
    printf("\n************ MULTIPLE Writes IN POLLING MODE ****************\n");
    /* close mailbox as it needs to be reopened to be reconfigured */
    Mailbox_close(handle);

    /* Setup the default Mailbox Parameters */
    if(Mailbox_Config_init(&cfg) < 0)
    {
        printf("Error: Unable to initialize configuration.\n");
        testFailedFlag = 1;
    }
    cfg.writeMode = MAILBOX_MODE_POLLING;
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    if (handle == NULL)
    {
        printf("Error: Unable to open the Mailbox Instance\n");
        testFailedFlag = 1;
        return ;
    }
    if (errCode != 0)
    {
        printf("Error: Unable to open the Mailbox Instance. Error=%d\n",errCode);
        testFailedFlag = 1;
        return ;
    }

    printf("First and third write should work. Second write should fail as interrupt has not arrived\n");

    Test_checkStatus(handle,0,0,0,0,0,0,0,0);
    Test_writePolling(handle);
    Test_checkStatus(handle,1,1,0,0,0,0,0,0);
    Test_writePolling(handle);
    Task_sleep(TASK_SLEEP_TIME + 700);
    Test_checkStatus(handle,0,1,0,0,0,0,1,0);
    Test_writePolling(handle);
    
        
    /**************************************************************************
     * Test END
     **************************************************************************/
    printf("\n************ TEST ENDED ****************\n");
    
    if(testFailedFlag==1)
    {
        MCPI_setFeatureTestResult("MSS_standalone", MCPI_TestResult_FAIL);
        printf("TEST FAILED\n");
    }    
    else
    {
        MCPI_setFeatureTestResult("MSS_standalone", MCPI_TestResult_PASS);
        printf("TEST PASSED\n");
    }    
    
    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

/* Task that is used to simulate acks/read/write/interupts from another master*/
void Test_auxiliaryTask(UArg arg0, UArg arg1)
{

    while(1)
    {
        /* send ack to write when flag is set*/
        Test_writeBlockingSendAck();
    }    
    
}

void Test_auxiliaryReadTask(UArg arg0, UArg arg1)
{

    while(1)
    {
        /* send ack to write when flag is set*/
        Test_readBlockingSendFullInt();
    }    
    
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
    Task_Params        taskParams;
    int32_t         errCode;
    int32_t         retVal;
    SOC_Cfg         socCfg;
    SOC_Handle      socHandle;

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
        System_printf("Error: Halt the BSS failed [Error code %d]\n", errCode);
        DebugP_assert (0);
    }
    else
    {
        /* Success: BSS has been halted successfully. */
        System_printf("Debug: Halted the BSS\n");
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 3*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.stackSize = 1024;
    Task_create(Test_auxiliaryTask, &taskParams, NULL);

    Task_Params_init(&taskParams);
    taskParams.stackSize = 1024;
    Task_create(Test_auxiliaryReadTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
    return 0;
}

