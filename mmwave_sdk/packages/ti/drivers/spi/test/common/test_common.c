/*
 *   @file  test_common.c
 *
 *   @brief
 *      Unit Test common code for the MibSpi Driver
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
#include <ti/drivers/spi/SPI.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/esm/esm.h>

/* TOP RCM register file */
#include <ti/drivers/soc/soc.h>

/* MCPI logger include files */
#include <ti/utils/testlogger/logger.h>

/**************************************************************************
 *************************** Local Definitions *********************************
 **************************************************************************/

/* Test flag for multiple icount */
/* #define SPI_MULT_ICOUNT_SUPPORT */

/* VBUSP Frequency in MHz */
#define VBUSP_FREQ              MSS_SYS_VCLK /1000000U

/* Number of Loops of Data echo test when XWR1xxx is configured as Master */
#define SPI_TEST_LOOP           1000

#ifdef SPI_MULT_ICOUNT_SUPPORT
 /* Block size used for the test*/
#define SPI_DATA_BLOCK_SIZE     1024
#else
 /* Block size used for the test*/
#define SPI_DATA_BLOCK_SIZE     128
#endif

/* Test message definitio for SPI test as a slave with external devices */
#define MAGIC_NUMBER            0x3456

#ifdef SPI_MULT_ICOUNT_SUPPORT
#define SPI_TEST_MSGLEN         1024
#else
#define SPI_TEST_MSGLEN         128
#endif
#define SPI_TEST_SYNC_MSGLEN    16
#define SWAP_BYTES(x)           (( (x&0xff) << 8) | ((x >>8) & 0xff))

/**************************************************************************
 *************************** Local Data Structure ******************************
 **************************************************************************/

/* SPI Test message definition */
typedef struct spiTestMsg
{
    /* Test message MAGIC number */
    uint16_t magicNumber;

    /* Test message sequence number */
    uint16_t seqNo;

    /* Test message valid data length */
    uint16_t dataLen;

    /* Test message checksum */
    uint16_t checksum;

    /* Test message data payload */
    uint8_t data[SPI_TEST_MSGLEN - 8];
}spiTestMsg;

/**************************************************************************
 *************************** Global Definitions ********************************
 **************************************************************************/
 /* Global data buffers used for SPI test */
volatile uint8_t    txBuf[SPI_DATA_BLOCK_SIZE];
volatile uint8_t    rxBuf[SPI_DATA_BLOCK_SIZE];

/* Test case flag */
extern bool         gXWR1xxxSlaveReady;

/* System DMA handle, created in init Task */
extern DMA_Handle   gDmaHandle;
extern bool         gXWR1xxxMasterWithXWR1xxx;
extern bool         gXWR1xxxSlaveWithXWR1xxx  ;

/* Debug flag */
uint32_t            gPrintHwStats = 1;

/**************************************************************************
 *************************** SPI Test Functions *******************************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      This function delay by running in tight loops.
 *
 *   @param[in] count             count for the delay loop
 *
 *  @retval
 *      Not Applicable.
 */
void Test_delay(uint32_t count)
{
    volatile uint32_t i;

    for(i=0; i<count; i++);
}

/**
 *  @b Description
 *  @n
 *      This function starts a PMU counter .
 *
 *   @param[in] counter             Counter id used for benchmarking
 *
 *  @retval
 *      Not Applicable.
 */
void Test_benchmarkStart(counter)
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
 *      This function stops a PMU counter and returns the current counter value.
 *
 *   @param[in] counter             Counter id used for benchmarking
 *
 *  @retval
 *      Current PMU counter value.
 */
uint32_t Test_benchmarkStop(counter)
{
    /* Stop PMU counter */
    Pmu_stopCounter(counter);

    /* Read PMU counter */
    return (Pmu_getCount(counter));
}

/**
 *  @b Description
 *  @n
 *      SPI get hardware statistics function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] printStats        Flag to print the stats
 *
 *   @retval    Successful        =0
 *                 Failed         <0
 */
static int32_t Test_getSpiStats(const SPI_Handle handle, bool printStats)
{
    SPI_Stats   stats;
    int32_t     retVal;

    retVal = SPI_getStats(handle, &stats);
    if(retVal == 0)
    {
        if(printStats)
        {
            System_printf("SPI hardware stats:\n");
            System_printf("datalen Error    = %d\n", stats.dlenErr);
            System_printf("timeout          = %d\n", stats.timeout);
            System_printf("Parity Error     = %d\n", stats.parErr);
            System_printf("Desync error     = %d\n", stats.desync);
            System_printf("Tx bit error     = %d\n", stats.bitErr);
            System_printf("RX overrun       = %d\n", stats.rxOvrnErr);
        }
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      SPI read only function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] buffer            Pointer of the buffer
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t Test_spiRead(const SPI_Handle handle, uint32_t dataLen, void* buffer, uint8_t slaveIndex)
{
    SPI_Transaction transaction;

    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = NULL;
    transaction.rxBuf = buffer;
    transaction.slaveIndex = slaveIndex;

    /* Start Data Transfer */
    if (SPI_transfer(handle, &transaction) != true)
    {
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      SPI write only function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] buffer            Pointer of the buffer
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t Test_spiWrite(const SPI_Handle handle, uint32_t dataLen, void* buffer, uint8_t slaveIndex)
{
    SPI_Transaction transaction;

    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = buffer;
    transaction.rxBuf = NULL;
    transaction.slaveIndex = slaveIndex;

    /* Start Data Transfer */
    if (SPI_transfer(handle, &transaction) != true)
    {
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      SPI read and write function.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] dataLen           Number of bytes need to be transferred
 *   @param[in] inBuffer          Pointer of the read buffer
 *   @param[in] outBuffer         Pointer of the write buffer
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval    Successful         =0
 *                 Failed         <0
 */
static int32_t Test_spiReadWrite(const SPI_Handle handle, uint32_t dataLen, void* inBuffer, void* outBuffer, uint8_t slaveIndex)
{
    SPI_Transaction  transaction;

    /* Configure Data Transfer */
    transaction.count = dataLen;
    transaction.txBuf = outBuffer;
    transaction.rxBuf = inBuffer;
    transaction.slaveIndex = slaveIndex;

    /* Start Data Transfer */
    if (SPI_transfer(handle, &transaction) != true)
    {
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in Digital Loopback mode.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Flag for internal/external loopback
 *   @param[in] maxElem           Maxim data element
 *   @param[in] dataSize          Data size in number of bytes
 *
 *  @retval    Successful                   =0
 *             Number of transfer failures  >0
 *             API failures                 <0
 */
static int32_t Test_spiLoopback(const SPI_Handle handle, uint8_t slaveIndex, uint32_t maxElem, uint8_t dataSize)
{
    uint8_t            loopback;
    uint32_t           loop;
    uint32_t           idx;
    uint32_t           failed = 0;
    uint32_t           len=0;

    /* Only dataSize of 1 byte or 2 bytes are supported */
    if ((dataSize != (uint8_t)1U) && (dataSize != (uint8_t)2U))
        return -1;

    /* Enable digital loopback */
    loopback = SPI_LOOPBK_DIGITAL;
    if(SPI_control(handle, SPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;
    for(loop=0; loop < maxElem; loop++)
    {
        len = (maxElem - loop) * dataSize;

        /* Prepare Tx/Rx Buffer */
        for(idx=0; idx<maxElem * dataSize; idx++)
        {
            txBuf[idx] = (loop * 0x10 + 0x55 + idx) & 0xFF;
        }

        /* Clear receive buffer */
        memset((void *)&rxBuf[0], 0x0, SPI_DATA_BLOCK_SIZE);

        if(Test_spiReadWrite(handle, len, (void *)rxBuf, (void *)txBuf, slaveIndex) == 0)
        {
            /* Check data integrity */
            if (memcmp((void *)txBuf, (void *)rxBuf, len) != 0)
            {
                System_printf("Error: SPI_transfer is successful with incorrect data(0x%x), length = %d\n", rxBuf[0], len);
                failed++;
            }
        }
        else
        {
            System_printf("Debug: SPI_transfer failed for length = %d\n", len);
            failed++;
        }
    }
    System_printf("Debug: Finished Digital loopback with various length test,  failed %d out of %d times\n", failed, loop);

    /* Disable digital loopback */
    loopback = SPI_LOOPBK_NONE;
    if(SPI_control(handle, SPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
        return -1;

    /* Return number of failures */
    return failed;
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver throughput in Loopback mode.
 *      Test is performed with following configurations:
 *       a. 16bits data size
 *       b. one slave
 *       c. Maximum data buffer
 *
 *   @param[in] inst             SPI instance (0=MibSPIA, 1=MibSPIB)
 *   @param[in] bitRate          SPI clock bit rate
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_spiLoopBackDataThroughput(uint32_t inst, uint32_t bitRate)
{
    SPI_Params       params;
    SPI_Handle       handle;
    SPI_Transaction  transaction;
    uint8_t          loopback;
    uint32_t         failed = 0;
    uint32_t         idx=0;
    uint32_t         loop;
    volatile uint32_t  cycles;
    float            throughput = 0;
    char             testCase[64];

    snprintf(testCase, 64, "SPI throughput test at bitRate %d Kbps", bitRate/1000);
    /**************************************************************************
     * Test: SPI Open
     **************************************************************************/
    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);
    params.frameFormat = SPI_POL0_PHA0;

    /* Enable DMA and set DMA channels to be used */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;
    params.eccEnable = 1;

    params.mode = SPI_MASTER;
    params.u.masterParams.bitRate = bitRate;

    params.u.masterParams.numSlaves = 1U;
    params.u.masterParams.slaveProf[0].chipSelect = 0U;
    params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
    params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U;
    params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;

    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");

        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);

        return;
    }

    /* Set SPI Digital loopback mode */
    loopback = SPI_LOOPBK_DIGITAL;
    if(SPI_control(handle, SPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        return;
    }

    /* Prepare data in TX Buffer */
    for(idx=0; idx<SPI_DATA_BLOCK_SIZE; idx++)
    {
        txBuf[idx] =  (0x55 + idx) & 0xFF;
    }

    memset((void *)&rxBuf[0], 0x0, SPI_DATA_BLOCK_SIZE);

    // Start the counter
    Test_benchmarkStart(0);

    for(loop=0; loop < 1000; loop++)
    {
        /* Change only the first bytes in the TX buffer */
        txBuf[0] = loop;

        /* Configure Data Transfer */
        transaction.count = SPI_DATA_BLOCK_SIZE;
        transaction.txBuf = (void *)txBuf;
        transaction.rxBuf = (void *)rxBuf;
        transaction.slaveIndex = 0U;

        /* Start Data Transfer */
        if (SPI_transfer(handle, &transaction) != true)
        {
            System_printf("Debug: SPI_transfer failed for , loop=%d\n", loop);
            failed++;
        }
    }

    // Stop the counter
    cycles = Test_benchmarkStop(0);
    throughput = 8.0 * SPI_DATA_BLOCK_SIZE * loop  * VBUSP_FREQ / cycles;

    System_printf("Debug: Finished Digital loopback througput test,  failed %d out of %d times\n", failed, loop);

    /* Disable SPI Digital loopback mode */
    loopback = SPI_LOOPBK_NONE;
    if (SPI_control(handle, SPI_CMD_LOOPBACK_ENABLE, (void *)&loopback) < 0)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        return;
    }

    /* Check failures and return results */
    if(failed == 0)
    {
        snprintf(testCase, 64, "SPI throughput at bitRate %d Kbps : %f Mbps", bitRate/1000, throughput);

        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }

    /* Print hardware error stats */
    Test_getSpiStats(handle, gPrintHwStats);

    /* Close the driver: */
    SPI_close(handle);
}

/**
 *  @b Description
 *  @n
 *       This function generates the test pattern for read/write test.
 *
 *   @param[in] buffer           Buffer pointer for the generated data
 *   @param[in] dataLen          Buffer len (in bytes) for pattern generation
 *
 *  @retval    None
 */
static void Test_generateTestPattern(volatile uint8_t * buffer, uint32_t dataLen)
{
    uint32_t               index;
    volatile uint8_t       *ptrBuff;

    ptrBuff =  buffer;
    for(index=0; index<dataLen; index++)
    {
        *ptrBuff = (index+0x55) & 0xFF;
        ptrBuff++;
    }
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in SPI master mode.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_spiMasterWrite(const SPI_Handle handle, uint8_t slaveIndex)
{
    uint16_t    loop;
    uint32_t    cycles;
    uint32_t    delayCycles;
    float       throughput;
    uint32_t    delay = 8U * SPI_TEST_MSGLEN;

    /* Initialize txBuf */
    Test_generateTestPattern(txBuf, SPI_TEST_MSGLEN);

    Test_benchmarkStart(0);
    Test_benchmarkStart(1);

    for (loop = 0; loop < SPI_TEST_LOOP; loop++)
    {
        /* Write data to slave */
        txBuf[0] = loop;
        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, slaveIndex);

        Pmu_startCounter(1);
        Test_delay(delay);
        Pmu_stopCounter(1);
    }

    cycles = Test_benchmarkStop(0);
    delayCycles = Pmu_getCount(1);

    throughput = 8.0 * SPI_TEST_MSGLEN * loop  * VBUSP_FREQ / (cycles - delayCycles);

    // Read and print the count
    System_printf("Master write Throughput = %.2f Mbps\n", throughput);
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver master mode Read data from MSP  SPI slave.
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Slave index for the SPI transfer
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_spiMasterRead(const SPI_Handle handle, uint8_t slaveIndex)
{
    uint16_t    loop;
    uint32_t    cycles;
    uint32_t    delayCycles;
    float       throughput;
    uint32_t    delay = 8U * SPI_TEST_MSGLEN;

    Test_benchmarkStart(0);
    Test_benchmarkStart(1);

    for (loop=0; loop<SPI_TEST_LOOP; loop++)
    {
        /* Read data from slave */
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, slaveIndex);
        Pmu_startCounter(1);
        Test_delay(delay);
        Pmu_stopCounter(1);

        rxBuf[0] = loop;
    }
    cycles = Test_benchmarkStop(0);
    delayCycles = Pmu_getCount(1);

    throughput = 8.0 * SPI_TEST_MSGLEN * loop  * VBUSP_FREQ / (cycles -delayCycles) ;

    // Read and print the count
    System_printf("Debug: Master read Throughput = %.2f Mbps\n", throughput);

}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in slave  mode
 *
 *   @param[in] handle        SPI driver handle
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_spiSlaveRead(const SPI_Handle handle)
{
    uint16_t    loop;

    for (loop=0; loop<SPI_TEST_LOOP; loop++)
    {
        /* Read two byte at a time */
        {
            Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver in slave mode.
 *
 *   @param[in] handle      SPI driver handle
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_spiSlaveWrite(const SPI_Handle handle)
{
    uint16_t    loop;

    Test_generateTestPattern(txBuf, SPI_TEST_MSGLEN);

    for (loop=0; loop<SPI_TEST_LOOP; loop++)
    {
        txBuf[0]= loop;

        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
    }
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver by send back data recevied from MSP
 *
 *   @param[in] handle                          SPI driver handle
 *
 *  @retval
 *      Not Applicable.
 */
static uint16_t Test_spiMasterDataEchoWithMSP43x(const SPI_Handle handle, uint8_t slaveIndex)
{
    spiTestMsg*     pTestMsg;
    uint16_t        retry=0;
    uint16_t        msgSeqNo = 0;
    uint16_t        testLoop =0;
    uint16_t        totalTestLoop;

    Task_sleep(1000);

    /**************************************************************************
     * Test: SPI Sync with Host
     **************************************************************************/
    while(1)
    {
        Task_sleep(1);
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, slaveIndex);


        pTestMsg = (spiTestMsg*)&rxBuf[0];

        if(pTestMsg->magicNumber != SWAP_BYTES(MAGIC_NUMBER))
        {
            System_printf("Incorrect magic Number: 0x%x 0x%x\n", pTestMsg->magicNumber, MAGIC_NUMBER);
            continue;
        }

        if((pTestMsg->dataLen != SWAP_BYTES(SPI_TEST_SYNC_MSGLEN)) || (pTestMsg->seqNo != 0))
        {
            System_printf("Incorrect datalen(%d), seqNo= %d\n", pTestMsg->dataLen, pTestMsg->seqNo);
            continue;
        }

        /* Got the correct sync message, send back the message */
        msgSeqNo = SWAP_BYTES(pTestMsg->seqNo);
        pTestMsg->seqNo =SWAP_BYTES(msgSeqNo+1);

        /* Save the Sync message for re-sent */
        memcpy((void *)txBuf, (void *)pTestMsg, SPI_TEST_MSGLEN);

        Task_sleep(1);

        /* Send back the sync message */
        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)pTestMsg, slaveIndex);

        break;
    }

    System_printf("Synced with Host, start data echo test\n");

    /* Test loop count from first 16 bits of Sync message */
    testLoop = pTestMsg->data[0] * 256 + pTestMsg->data[1] ;
    totalTestLoop = testLoop;
    System_printf("Total Test loop=%d\n", testLoop);
    testLoop++;

    while(testLoop)
    {
        Test_delay( 6000);
        /* Read test message from Host */
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, slaveIndex);

        if(pTestMsg->magicNumber  != SWAP_BYTES(MAGIC_NUMBER))
        {
            continue;
        }

        if((pTestMsg->seqNo == 0) && (pTestMsg->dataLen == SWAP_BYTES(SPI_TEST_SYNC_MSGLEN)))
        {
            Test_delay( 6000);

            /* Re-sent the Sync message */
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, slaveIndex);
            continue;
        }

        /* Receive Previous message */
        if(pTestMsg->seqNo == SWAP_BYTES(msgSeqNo))
        {
            Test_delay( 6000);

            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, slaveIndex);
            retry++;

            continue;
        }

        /* Incorrect seqNo, drop the message */
        if(pTestMsg->seqNo != SWAP_BYTES(msgSeqNo + 2))
        {
            retry++;
            continue;
        }

        /* Correctly receive the test data message, prepare to echo back */
        pTestMsg->seqNo = SWAP_BYTES(SWAP_BYTES(pTestMsg->seqNo) + 1);

        /* Save the data message for re-sent */
        memcpy((void *)txBuf, (void *)pTestMsg, SPI_TEST_MSGLEN);

        Test_delay(6000);

        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, slaveIndex);
        retry = 0;
        testLoop--;
        msgSeqNo += 2;

    }

    return (totalTestLoop);
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver by send back data recevied from MSP
 *
 *   @param[in] handle                          SPI driver handle
 *
 *  @retval
 *      Not Applicable.
 */
static uint16_t Test_spiSlaveDataEchoWithMSP43x(const SPI_Handle handle)
{
    spiTestMsg*              pTestMsg;
    uint16_t                 retry=0;
    uint16_t                 msgSeqNo = 0;
    uint16_t                 testLoop =0;
    uint16_t                 totalTestLoop;

    /**************************************************************************
     * Test: SPI Sync with Host
     **************************************************************************/
    while(1)
    {
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);

        pTestMsg = (spiTestMsg*)&rxBuf[0];
        printf("recv msg: magic=%x, dataLen=%d, seq=%d\n", pTestMsg->magicNumber, pTestMsg->dataLen, pTestMsg->seqNo);

        if(pTestMsg->magicNumber  != SWAP_BYTES(MAGIC_NUMBER))
        {
            System_printf("Incorrect magic Number: 0x%x 0x%x\n", pTestMsg->magicNumber, MAGIC_NUMBER);
            continue;
        }

        if((pTestMsg->dataLen != SWAP_BYTES(SPI_TEST_SYNC_MSGLEN)) || (pTestMsg->seqNo != 0))
        {
            System_printf("Incorrect datalen(%d), seqNo= %d\n", pTestMsg->dataLen, pTestMsg->seqNo);
            continue;
        }

        pTestMsg->magicNumber = MAGIC_NUMBER;

        /* Got the correct sync message, send back the message */
        msgSeqNo = SWAP_BYTES(pTestMsg->seqNo);
        pTestMsg->seqNo =SWAP_BYTES(msgSeqNo+1);

        /* Send back the sync message */
        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)pTestMsg, 0U);

        break;
    }

    /* Save the Sync message for re-sent */
    memcpy((void *)txBuf, (void *)pTestMsg, SPI_TEST_MSGLEN);

    /* Test loop count from first 16 bits of Sync message */
    testLoop = pTestMsg->data[0] * 256 + pTestMsg->data[1] ;
    totalTestLoop = testLoop;
    System_printf("Total Test loop=%d\n", testLoop);
    testLoop++;

    while(testLoop)
    {
        /* Read test message from Host */
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);

        if(pTestMsg->magicNumber  != SWAP_BYTES(MAGIC_NUMBER))
        {
            continue;
        }

        if((pTestMsg->seqNo == 0) && (pTestMsg->dataLen == SWAP_BYTES(SPI_TEST_SYNC_MSGLEN)))
        {
            /* Re-sent the Sync message */
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
            continue;
        }

        /* Receive Previous message */
        if(pTestMsg->seqNo == SWAP_BYTES(msgSeqNo))
        {
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
            retry++;

            continue;
        }

        /* Incorrect seqNo, drop the message */
        if(pTestMsg->seqNo != SWAP_BYTES(msgSeqNo + 2))
        {
            retry++;
            continue;
        }

        /* Correctly receive the test data message, prepare to echo back */
        pTestMsg->seqNo = SWAP_BYTES(SWAP_BYTES(pTestMsg->seqNo) + 1);

        /* Save the data message for re-sent */
        memcpy((void *)txBuf, (void *)pTestMsg, SPI_TEST_MSGLEN);

        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
        retry = 0;
        testLoop--;
        msgSeqNo += 2;
    }

    return (totalTestLoop);
}


/**
 *  @b Description
 *  @n
 *       This function tests SPI driver throughput in Loopback mode.
 *
 *   @param[in] masterMode       Flag for master mode, 1 - master, 0 - slave
 *   @param[in] bitRate          SPI clock bit rate
 *
 *  @retval    None
 */
void Test_spiMSP432xThroughput(bool masterMode, uint32_t bitRate)
{
    SPI_Params         params;
    SPI_Handle         handle;
    volatile uint32_t  cycles;
    float              throughput;
    uint32_t           loop = SPI_TEST_LOOP;

    /**************************************************************************
     * Test: SPI Open
     **************************************************************************/
    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);
    params.frameFormat = SPI_POL0_PHA0;
    params.shiftFormat = SPI_MSB_FIRST;
    params.pinMode = SPI_PINMODE_3PIN;

    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1U;
    params.dmaHandle = gDmaHandle;

    params.eccEnable = 1U;

    if(masterMode == true)
    {
        params.mode = SPI_MASTER;

        params.u.masterParams.bitRate = bitRate;
        params.u.masterParams.numSlaves = 1;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
        params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U;
        params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;
    }
    else
    {
        params.mode = SPI_SLAVE;
        params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
        params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;
    }

    handle = SPI_open(0, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        return;
    }
    System_printf("Debug: SPI throughput test with bit rate = %d Kbps\n", bitRate/1000);

    memset((void *)&rxBuf[0], 0x0, SPI_DATA_BLOCK_SIZE);

    // Start the counter
    Test_benchmarkStart(0);

    if (masterMode == true)
        loop = Test_spiMasterDataEchoWithMSP43x(handle, 0U);
    else
         loop = Test_spiSlaveDataEchoWithMSP43x(handle);

    // Stop the counter
    cycles = Test_benchmarkStop(0);

    // Calculate and print the count
    throughput = 8.0 * SPI_TEST_MSGLEN * loop  * VBUSP_FREQ / cycles;
    System_printf("Debug: throughput overall = %.2f Mbps\n", throughput);

    /* Close the driver: */
    SPI_close(handle);
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver slave mode with SPI master running on a MSP device.
 *
 *   @param[in] masterMode       Flag for master mode, 1 - master, 0 - slave
 *   @param[in] bitRate          SPI clock bit rate
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiWithMSP43x(bool master, uint32_t bitRate)
{
    SPI_Params     params;
    SPI_Handle     handle;

    /**************************************************************************
     * Test: SPI Open
     **************************************************************************/
    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);
    if(master == true)
    {
        params.mode = SPI_MASTER;

        params.u.masterParams.bitRate = bitRate;
        params.u.masterParams.numSlaves = 1;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
        params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U;
        params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;
    }
    else
    {
        params.mode = SPI_SLAVE;
        params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
        params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;
    }
    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;

    params.frameFormat = SPI_POL0_PHA0;
    params.shiftFormat = SPI_MSB_FIRST;
    params.pinMode = SPI_PINMODE_3PIN;
    params.eccEnable = 1;

    handle = SPI_open(0, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        return;
    }
    System_printf("Debug: SPI Instance %p has been reopened in %s mode successfully\n", handle, master? "MASTER" : "SLAVE");

    if(master == true)
    {
        /**************************************************************************
         * Test: Master SPI Read/Write
         **************************************************************************/
        System_printf("Start SPI Read test with MSP43x\n");
        Test_spiMasterRead(handle, 0U);

        Task_sleep(100);

        System_printf("Start SPI Write test with MSP43x\n");
        Test_spiMasterWrite(handle, 0U);

        Task_sleep(100);

        /**************************************************************************
         * Test: SPI data echo with MSP
         **************************************************************************/
        System_printf("Start SPI Data echo test with MSP43x\n");
        Test_spiMasterDataEchoWithMSP43x(handle, 0U);
    }
    else
    {
        gXWR1xxxSlaveReady = true;

        /**************************************************************************
         * Test: Slave SPI Read/Write
         **************************************************************************/
        System_printf("Start SPI Read test with MSP43x\n");
        Test_spiSlaveRead(handle);

        System_printf("Start SPI Write test with MSP43x\n");
        Test_spiSlaveWrite(handle);

        /**************************************************************************
         * Test: SPI data echo with MSP
         **************************************************************************/
        System_printf("Start SPI Data echo test with MSP43x\n");
        Test_spiSlaveDataEchoWithMSP43x(handle);
    }

    /**************************************************************************
     * Test: SPI Close
     **************************************************************************/
    /* Close the driver: */
    SPI_close(handle);
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver by send back data recevied from MSP
 *
 *   @param[in] handle            SPI driver handle
 *   @param[in] slaveIndex        Slave index for the data echo test
 *
 *  @retval
 *      Not Applicable.
 */
static uint16_t Test_spiXWR1xxxMasterDataEcho(const SPI_Handle handle, uint8_t slaveIndex)
{
    spiTestMsg*              pTestMsgOut;
    spiTestMsg*              pTestMsgIn;
    uint16_t                 retry=0;
    uint32_t                 loop;
    uint32_t                 failed = 0;
    uint32_t                 gTestMsgSeqNo = 0;

    Task_sleep(5000);

    gTestMsgSeqNo = 0;

    /* Test data echo */
    pTestMsgOut = (spiTestMsg*)&txBuf[0];
    pTestMsgIn = (spiTestMsg*)&rxBuf[0];

    /* Init the buffer with 0xabab */
    memset ((void *)pTestMsgOut, 0xab, SPI_DATA_BLOCK_SIZE);

    /* Setup the message header */
    pTestMsgOut->magicNumber = MAGIC_NUMBER;
    pTestMsgOut->seqNo = gTestMsgSeqNo;
    pTestMsgOut->dataLen = SPI_TEST_SYNC_MSGLEN;
    pTestMsgOut->data[1] = SPI_TEST_LOOP & 0xff;
    pTestMsgOut->data[0] = (SPI_TEST_LOOP>>8) & 0xff;

    /**************************************************************************
     * Test: SPI Sync with Host
     **************************************************************************/
    while(1)
    {
        Test_delay(5000);

        Test_spiWrite(handle, SPI_TEST_MSGLEN, (Ptr)pTestMsgOut, slaveIndex);

        Test_delay(5000);

        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)pTestMsgIn, slaveIndex);
        System_printf("Master: recv msg: magic=%x, dataLen=%d, seq=%d\n", pTestMsgIn->magicNumber, pTestMsgIn->dataLen, pTestMsgIn->seqNo);

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
    }

    System_printf("Master: Synced with Slave, start data echo test\n");
    gTestMsgSeqNo+= 2;

    /* Initialize the data pattern */
    Test_generateTestPattern((Ptr)pTestMsgOut, SPI_TEST_MSGLEN);
    pTestMsgOut->magicNumber = MAGIC_NUMBER;
    pTestMsgOut->dataLen = SPI_TEST_MSGLEN;
    Test_delay(5000);

    for (loop = 0; loop < SPI_TEST_LOOP + 1; loop++)
    {
        retry = 0;

        /* Update sequence number */
        pTestMsgOut->seqNo = gTestMsgSeqNo;

        do
        {
            Test_delay(5000);
            /* Initiate SPI transfer */
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (Ptr)pTestMsgOut, slaveIndex);
            Test_delay(5000);
            Test_spiRead(handle, SPI_TEST_MSGLEN, (Ptr)pTestMsgIn, slaveIndex);

            if ((pTestMsgIn->magicNumber != MAGIC_NUMBER) ||
                (pTestMsgIn->dataLen != SPI_TEST_MSGLEN) ||
                (pTestMsgIn->seqNo != (gTestMsgSeqNo + 1)))
            {
                retry++;
                System_printf("Master: Incorrect in Msg: magic =0x%x, dataLen=%d, seqNo=%d, loop=%d\n",
                             pTestMsgIn->magicNumber, pTestMsgIn->dataLen, pTestMsgIn->seqNo, loop);
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

    return (loop);
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver by send back data recevied from MSP
 *
 *   @param[in] handle                          SPI driver handle
 *
 *  @retval
 *      Not Applicable.
 */
static uint16_t Test_spiXWR1xxxSlaveDataEcho(const SPI_Handle handle)
{
    spiTestMsg*              pTestMsg;
    uint16_t                 retry=0;
    uint16_t                 msgSeqNo = 0;
    uint16_t                 testLoop =0;
    uint16_t                 totalTestLoop;

    /**************************************************************************
     * Test: SPI Sync with Host
     **************************************************************************/
    while(1)
    {
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);

        pTestMsg = (spiTestMsg*)&rxBuf[0];

        if(pTestMsg->magicNumber != MAGIC_NUMBER)
        {
            System_printf("Slave:Incorrect magic Number: 0x%x 0x%x\n", pTestMsg->magicNumber, MAGIC_NUMBER);
            continue;
        }

        if((pTestMsg->dataLen != SPI_TEST_SYNC_MSGLEN) || (pTestMsg->seqNo != 0))
        {
            System_printf("Slave:Incorrect datalen(%d), seqNo= %d\n", pTestMsg->dataLen, pTestMsg->seqNo);
            continue;
        }

        pTestMsg->magicNumber = MAGIC_NUMBER;

        /* Got the correct sync message, send back the message */
        msgSeqNo = pTestMsg->seqNo;
        pTestMsg->seqNo = msgSeqNo + 1;

        /* Send back the sync message */
        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)pTestMsg, 0U);

        break;
    }

    /* Save the Sync message for re-sent */
    memcpy((void *)txBuf, (void *)pTestMsg, SPI_TEST_MSGLEN);

    /* Test loop count from first 16 bits of Sync message */
    testLoop = pTestMsg->data[0] * 256 + pTestMsg->data[1] ;
    totalTestLoop = testLoop;
    testLoop++;

    while(testLoop)
    {
        /* Read test message from Host */
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);

        if(pTestMsg->magicNumber != MAGIC_NUMBER)
        {
            continue;
        }

        if((pTestMsg->seqNo == 0) && (pTestMsg->dataLen == SPI_TEST_SYNC_MSGLEN))
        {
            /* Re-sent the Sync message */
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
            continue;
        }

        /* Receive Previous message */
        if(pTestMsg->seqNo == msgSeqNo)
        {
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
            retry++;

            continue;
        }

        /* Incorrect seqNo, drop the message */
        if(pTestMsg->seqNo != (msgSeqNo + 2))
        {
            retry++;
            continue;
        }

        /* Correctly receive the test data message, prepare to echo back */
        pTestMsg->seqNo = (pTestMsg->seqNo + 1);

        /* Save the data message for re-sent */
        memcpy((void *)txBuf, (void *)pTestMsg, SPI_TEST_MSGLEN);

        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
        retry = 0;
        testLoop--;
        msgSeqNo += 2;

    }

    return (totalTestLoop);
}


/**
 *  @b Description
 *  @n
 *      This function tests SPI driver slave mode with SPI master running on a MSP device.
 *
 *   @param[in] masterMode       Flag for master mode, 1 - master, 0 - slave
 *   @param[in] bitRate          SPI clock bit rate
 *   @param[in] pinMode          pin Mode setting (3Pin or 4Pin)
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiWithXWR1xxx(bool master, uint32_t bitRate, SPI_PinMode pinMode)
{
    SPI_Params     params;
    SPI_Handle     handle;
    char           testCase[64];

    snprintf(testCase, 64, "SPI %s mode %s test at bitRate %d Kbps ",
           (pinMode == SPI_PINMODE_3PIN)? "3Pin":"4Pin",
           (master == true)? "Master":"Slave",
           bitRate/1000);

    /**************************************************************************
     * Test: SPI Open
     **************************************************************************/
    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);
    if(master == true)
    {
        params.mode = SPI_MASTER;

        params.u.masterParams.bitRate = bitRate;
        params.u.masterParams.numSlaves = 1;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
        params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U;
        params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;

        /* Delay parameters */
        if(pinMode == SPI_PINMODE_4PIN_CS)
        {
#ifdef SPI_MULT_ICOUNT_SUPPORT
            /* Settings for icount != 0
              These timing settings are used to
              add delays in Master to make sure slave has enough time for
              DMA copy of the received data */
            params.u.masterParams.c2tDelay = 50;
            params.u.masterParams.t2cDelay = 20;
            params.u.masterParams.wDelay = 20;

#else
            params.u.masterParams.c2tDelay = 0x20;
            params.u.masterParams.t2cDelay = 0x20;
            params.u.masterParams.wDelay = 0;
#endif
        }
    }
    else
    {
        params.mode = SPI_SLAVE;
        params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
        params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;
    }

    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;

    params.frameFormat = SPI_POL0_PHA0;
    params.pinMode = pinMode;

    handle = SPI_open(0, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        return;
    }
    System_printf("Debug: SPI Instance %p has been reopened in %s mode successfully\n", handle, master? "MASTER" : "SLAVE");

    if(master == true)
    {
        /**************************************************************************
         * Test: Master SPI Read/Write
         **************************************************************************/
        System_printf("Master: Start SPI Write test with XWR1xxx\n");
        Test_spiMasterWrite(handle, 0U);

        Task_sleep(10000);

        System_printf("Master: Start SPI Read test with XWR1xxx\n");
        Test_spiMasterRead(handle, 0U);

        Task_sleep(10000);
        /**************************************************************************
         * Test: SPI data echo with MSP
         **************************************************************************/
        System_printf("Master: Start SPI Data echo test with XWR1xxx\n");
        Test_spiXWR1xxxMasterDataEcho(handle, 0U);
    }
    else
    {
        gXWR1xxxSlaveReady = true;

        /**************************************************************************
         * Test: Slave SPI Read/Write
         **************************************************************************/
        //System_printf("Start SPI Read test with XWR1xxx\n");
        Test_spiSlaveRead(handle);

        //System_printf("Start SPI Write test with XWR1xxx\n");
        Test_spiSlaveWrite(handle);

        /**************************************************************************
         * Test: SPI data echo with MSP
         **************************************************************************/
        //System_printf("Start SPI Data echo test with XWR1xxx\n");
        Test_spiXWR1xxxSlaveDataEcho(handle);
    }

    MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);

    /**************************************************************************
     * Test: SPI Close
     **************************************************************************/
    /* Close the driver: */
    SPI_close(handle);
}

/**
 *  @b Description
 *  @n
 *      This function tests SPI driver slave mode with external PC.
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiSlaveWithPC(void)
{
    SPI_Params     params;
    SPI_Handle     handle;
    spiTestMsg*    pTestMsg;
    uint16_t       retry=0;
    uint16_t       msgSeqNo = 0;
    uint32_t       loop;
    volatile uint32_t  cycles;
    float          throughput;
    uint32_t       totalLoop;

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    params.mode = SPI_SLAVE;
    params.frameFormat = SPI_POL0_PHA1;
    params.pinMode = SPI_PINMODE_4PIN_CS;
    params.shiftFormat = SPI_MSB_FIRST;

    /* When communicating with PC through FTDI, it is difficult to toggle CS for every two bytes, hence set csHold to 1.
       In this mode, the highest working SPI clock is 2MHz */
    params.csHold = 1;

    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;
    params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;

    handle = SPI_open(0, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        MCPI_setFeatureTestResult ("SPI Slave with PC ", MCPI_TestResult_FAIL);
        return;
    }

    memset((void *)txBuf, 0x55, SPI_TEST_MSGLEN);

    System_printf("Debug: SPI Instance %p has been reopened in SLAVE mode successfully for 4pin FTDI testing\n", handle);
    gXWR1xxxSlaveReady = true;

    while(1)
    {
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);

        pTestMsg = (spiTestMsg*)&rxBuf[0];
        if(pTestMsg->magicNumber  != SWAP_BYTES(MAGIC_NUMBER))
        {
            System_printf("incorrect magic Number: 0x%x 0x%x\n", pTestMsg->magicNumber, SWAP_BYTES(MAGIC_NUMBER));
            continue;
        }

        if((pTestMsg->dataLen != SWAP_BYTES(SPI_TEST_SYNC_MSGLEN)) || (pTestMsg->seqNo != 0))
        {
            System_printf("incorrect datalen(%d), seqNo= %d\n", pTestMsg->dataLen, pTestMsg->seqNo);
            continue;
        }

        /* Got the correct sync message, send back the message */
        msgSeqNo = SWAP_BYTES(pTestMsg->seqNo);
        pTestMsg->seqNo = SWAP_BYTES(msgSeqNo+1);

        /* Read two byte , send it back */
        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)pTestMsg, 0U);

        break;
    }

    /* Save the Sync message for re-sent */
    memcpy((void *)txBuf, (void *)pTestMsg, SPI_TEST_MSGLEN);

    /* Got the Sync message, waiting for next test message */
    msgSeqNo += 2;

    /* Test loop count from first 16 bits of Sync message */
    loop = pTestMsg->data[0] * 256 + pTestMsg->data[1] ;
    System_printf("Test loop=%d, bufferLen=%d\n", loop, SPI_TEST_MSGLEN);
    totalLoop = loop;

    while(loop)
    {
        /* Read two byte , send it back */
        Test_spiRead(handle, SPI_TEST_MSGLEN, (void *)rxBuf, 0U);

        if(pTestMsg->magicNumber != SWAP_BYTES(MAGIC_NUMBER))
        {
            continue;
        }

        if((pTestMsg->seqNo == 0) && (pTestMsg->dataLen == SWAP_BYTES(SPI_TEST_SYNC_MSGLEN)))
        {
            /* Re-sent the Sync message */
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
            continue;
        }

        /* Receive Last message */
        if(pTestMsg->seqNo == SWAP_BYTES(msgSeqNo - 2))
        {
            Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)txBuf, 0U);
            retry++;

            continue;
        }

        /* Incorrect seqNo, drop the message */
        if(pTestMsg->seqNo != SWAP_BYTES(msgSeqNo))
        {
            retry++;
            continue;
        }

        if(loop == totalLoop)
        {
            // Start the counter after receive the first message
            Test_benchmarkStart(0);
        }

        retry = 0;
        loop--;

        /* Receive the message with correct header, increment the seq Number */
        pTestMsg->seqNo = SWAP_BYTES(SWAP_BYTES(pTestMsg->seqNo) + 1);

        Test_spiWrite(handle, SPI_TEST_MSGLEN, (void *)pTestMsg, 0U);

        msgSeqNo += 2;
    }
    // Stop the counter
    cycles = Test_benchmarkStop(0);

    // Calculate and print the count
    throughput = 8.0 * SPI_TEST_MSGLEN * totalLoop  * VBUSP_FREQ / cycles;
    System_printf("Debug: throughput overall = %.2f Mbps\n", throughput);

    MCPI_setFeatureTestResult ("SPI Slave with PC ", MCPI_TestResult_PASS);
}

/**
 *  @b Description
 *  @n
 *      SPI API test for both instance - MibSPIA & MibSPIB.
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiAPI_twoInstance(void)
{
    SPI_Params     params;
    SPI_Handle     handleA = NULL;
    SPI_Handle     handleB = NULL;
    uint32_t       dmaChanNum = 0;
    uint32_t       instA = 0;
    uint32_t       instB = 1;
    char           testCase[64];

    /**************************************************************************
     * Test: Verify SPI open/close API
     **************************************************************************/
    snprintf(testCase, 64, "SPI_open API test for two instances");

    /* Setup the default SPIA Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;

    params.mode = SPI_SLAVE;
    params.u.slaveParams.dmaCfg.txDmaChanNum =dmaChanNum++;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =dmaChanNum++;

    /* Open the SPI Instance for MibSpiA */
    handleA = SPI_open(instA, &params);
    if (handleA == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        goto exit;
    }
    System_printf("Debug: SPI Instance(%d) %p has been opened successfully\n", instA, handleA);

    /* Setup the default SPIB Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;

    params.mode = SPI_SLAVE;
    params.u.slaveParams.dmaCfg.txDmaChanNum =dmaChanNum++;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =dmaChanNum++;

    /* Open the SPI Instance for MibSpiB */
    handleB = SPI_open(instB, &params);
    if (handleB == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        goto exit;
    }
    System_printf("Debug: SPI Instance(1) %p has been opened successfully\n", handleB);
    MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);

    /* Close SPIA */
    SPI_close(handleA);
    System_printf("Debug: SPI Instance(0) %p has been closed successfully\n", handleA);
    handleA= NULL;

    /* Close SPIB */
    SPI_close(handleB);
    System_printf("Debug: SPI Instance(1) %p has been closed successfully\n", handleB);
    handleB = NULL;

    /**************************************************************************
     * Test: Verify SPI API parameters for DMA channel configuration
     **************************************************************************/
    snprintf(testCase, 64, "SPI_open API test for two instances: DMA channel validation");

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;

    params.mode = SPI_SLAVE;
    params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;


    /* Open the SPI Instance for MibSpiA */
    handleA = SPI_open(instA, &params);
    if (handleA != NULL)
    {
        System_printf("Debug: SPI Instance(0) %p has been opened successfully\n", handleA);
    }
    else
    {
        System_printf("Error: Failed to open SPI Instance(0)\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        goto exit;
    }

    /* Setup the default SPIB Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels that same as SPIA, test should fail */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;

    params.mode = SPI_SLAVE;
    params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;

    /* Open the SPI Instance for MibSpiA */
    handleB = SPI_open(instB, &params);

    if (handleB == NULL)
    {
        System_printf("Debug: passed DMA channel number check for two SPI instances.\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        System_printf("Error: Failed DMA channel number check for two SPI instances\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }
exit:
    if(handleA != NULL)
    {
        SPI_close(handleA);
        System_printf("Debug: SPI Instance %p has been closed successfully\n", handleA);
    }

    if(handleB != NULL)
    {
        SPI_close(handleB);
        System_printf("Debug: SPI Instance %p has been closed successfully\n", handleB);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      SPI API test for one instance.
 *
 *   @param[in] inst               SPI instance: 0-SPIA, 1-SPIB
 *
 *  @retval
 *      Not Applicable.
 */
void Test_spiAPI_oneInstance(uint8_t inst)
{
    SPI_Params     params;
    SPI_Handle     handle;
    char           testCase[64];

    /**************************************************************************
     * Test: Verify SPI open/close API
     **************************************************************************/
    snprintf(testCase, 64, "SPI_open API test (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    params.mode = SPI_SLAVE;

    /* Enable DMA and set DMA channels */
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle;
    params.u.slaveParams.dmaCfg.txDmaChanNum = 1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum = 0U;

    /* Open the SPI Instance for MibSpi */
    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    System_printf("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Close SPI */
    SPI_close(handle);
    System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Verify SPI API for DMA channel configuration
     **************************************************************************/
    snprintf(testCase, 64, "SPI_open API test - DMA channel validation (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle;

    params.mode = SPI_SLAVE;

    /* Open the SPI Instance for MibSpiA */
    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Debug: Passed DMA channel number check\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        System_printf("Error: Failed SPI DMA channel number check\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        SPI_close(handle);
        System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify SPI parameters check::chipSelect
     **************************************************************************/
    snprintf(testCase, 64, "SPI_open API test - chip select validation (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels */
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle;
    params.u.slaveParams.dmaCfg.txDmaChanNum = 1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum = 0U;

    params.mode = SPI_SLAVE;

    /* Incorrect chip select */
    params.u.slaveParams.chipSelect = (uint8_t)10U;

    /* Open the SPI Instance for MibSpiA */
    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        SPI_close(handle);
        System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify SPI open API for parameters check
     **************************************************************************/
    snprintf(testCase, 64, "SPI_open API test - data size validation (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels to be used */
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle;
    params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;

    params.mode = SPI_SLAVE;

    /* Incorrect data Size */
    params.dataSize = 7U;

    /* Open the SPI Instance */
    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        SPI_close(handle);
        System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify SPI parameters check::master clock
     **************************************************************************/
    snprintf(testCase, 64, "SPI_open API test - bit rate validation (instance=%d)", inst);

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels */
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle;
    params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;

    params.mode = SPI_MASTER;
    params.u.masterParams.bitRate = 0;

    /* Open the SPI Instance for MibSpiA */
    handle = SPI_open(0, &params);
    if (handle == NULL)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);

        /* Graceful shutdown */
        SPI_close(handle);
        System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);
    }

    /**************************************************************************
     * Test: Verify SPI transfer API for parameters check
     **************************************************************************/

    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);

    /* Enable DMA and set DMA channels to be used */
    params.dmaEnable = (uint8_t)1U;
    params.dmaHandle = gDmaHandle;
    params.dataSize = 16U;

    params.mode = SPI_SLAVE;
    params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
    params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;

    System_printf("Debug: Open the SPI Instance for SPI transfer parameter check test\n");

    /* Open the SPI Instance for MibSpi */
    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }
    else
    {
        SPI_Transaction transaction;
        bool          ret;

        /* Configure Data Transfer */
        transaction.count = (uint16_t)1U;
        transaction.txBuf = (void *)txBuf;
        transaction.rxBuf = (void *)rxBuf;
        transaction.slaveIndex = (uint16_t)0U;

        snprintf(testCase, 64, "SPI_transfer API test - data size=%d validation (instance=%d)", transaction.count, inst);

        /* Start Data Transfer */
        ret = SPI_transfer(handle, &transaction);
        if (ret != true)
        {
            System_printf("Debug: SPI_transfer with data size = %d failed with error=%d.\n", transaction.count, transaction.status );
            MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
        }
        else
        {
            System_printf("Error: SPI_transfer with data size = %d should fail.\n", transaction.count);
            MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        }

        /* Configure Data Transfer */
        transaction.count = (uint16_t)11U;
        transaction.txBuf = (void *)txBuf;
        transaction.rxBuf = (void *)rxBuf;
        transaction.slaveIndex = (uint16_t)0U;

        snprintf(testCase, 64, "SPI_transfer API test - data size=%d validation (instance=%d)",transaction.count, inst);

        /* Start Data Transfer */
        ret = SPI_transfer(handle, &transaction);
        if (ret != true)
        {
            System_printf("Debug: SPI_transfer with data size = %d failed with error=%d. \n", transaction.count, transaction.status);
            MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
        }
        else
        {
            System_printf("Error: SPI_transfer with data size = %d should fail.\n", transaction.count);
            MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        }

        /* Configure Data Transfer */
        transaction.count = (uint16_t)10U;
        transaction.txBuf = (void *)NULL;
        transaction.rxBuf = (void *)NULL;
        transaction.slaveIndex = (uint16_t)0U;

        snprintf(testCase, 64, "SPI_transfer API test - buffer address validation (instance=%d)", inst);

        /* Start Data Transfer */
        ret = SPI_transfer(handle, &transaction);
        if (ret != true)
        {
            System_printf("Debug: SPI_transfer failed with NULL pointers for both TX and RX. \n", transaction.status);
            MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
        }
        else
        {
            System_printf("Error: SPI_transfer with NULL pointers for both TX and RX should fail.\n");
            MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        }

        /* Graceful shutdown */
        SPI_close(handle);
        System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);
    }
}

/**
 *  @b Description
 *  @n
 *      SPI loopback test.
 *
 *   @param[in] inst               SPI instance: 0-SPIA, 1-SPIB
 *
 *  @retval
 *      Not Applicable.
 */
void Test_loopback_oneInstance(uint32_t inst, uint8_t slaveIndex)
{
    SPI_Params     params;
    SPI_Handle     handle;
    char           testCase[64];

    snprintf(testCase, 64, "SPI loopback test - instance(%d), 16bits DMA mode", inst);

    /**************************************************************************
     * Test: Open the driver in master mode for loopback test
     **************************************************************************/
    /* Setup the default SPI Parameters */
    SPI_Params_init(&params);
    params.frameFormat = SPI_POL0_PHA0;

    /* Enable DMA and set DMA channels to be used */
    params.dmaEnable = 1;
    params.dmaHandle = gDmaHandle;
    params.eccEnable = 1;
    params.mode = SPI_MASTER;
    params.u.masterParams.bitRate = 1000000U;

    /* mibSPIA support only one slave */
    if(inst == 0)
    {
        params.u.masterParams.numSlaves = 1;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM;
        params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U;
        params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;
    }
    else if(inst == 1)
    {
        /* The total element size of 3 slaves is MIBSPI_RAM_MAX_ELEM
         * In this example, it is distributed among 3 slaves by MIBSPI_RAM_MAX_ELEM - 6U, 4U, and 2U
         */
        memset((void *)&params.u.masterParams.slaveProf[0], 0, sizeof(params.u.masterParams.slaveProf));

        params.u.masterParams.numSlaves = 3;
        params.u.masterParams.slaveProf[0].chipSelect = 0;
        params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
        params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U;
        params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;
        params.u.masterParams.slaveProf[1].chipSelect = 1;
        params.u.masterParams.slaveProf[1].ramBufLen = MIBSPI_RAM_MAX_ELEM/4;
        params.u.masterParams.slaveProf[1].dmaCfg.txDmaChanNum =3U;
        params.u.masterParams.slaveProf[1].dmaCfg.rxDmaChanNum =2U;
        params.u.masterParams.slaveProf[2].chipSelect = 2;
        params.u.masterParams.slaveProf[2].ramBufLen = MIBSPI_RAM_MAX_ELEM/4;
        params.u.masterParams.slaveProf[2].dmaCfg.txDmaChanNum =5U;
        params.u.masterParams.slaveProf[2].dmaCfg.rxDmaChanNum =4U;
    }
    else
    {
        printf("Error: Invalid instance(=%d) of MibSPI\n", inst);
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }

    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
        return;
    }
    System_printf("Debug: SPI Instance %p has been reopened in master mode successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 2) == 0)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }

    /* Close the driver: */
    SPI_close(handle);
    System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback test without DMA
     **************************************************************************/
    snprintf(testCase, 64, "SPI loopback test - instance(%d), 16bits non-DMA mode", inst);

    /* Change dma configuration */
    params.dmaEnable = 0;

    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        return;
    }
    System_printf("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 2) == 0U)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }

    /* Close the driver: */
    SPI_close(handle);
    System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback test DMA enabled 8bits
     **************************************************************************/
    snprintf(testCase, 64, "SPI loopback test - instance(%d), 8bits DMA mode", inst);

    /* Change dma configuration */
    params.dmaEnable = 1;
    params.dataSize =8;

    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        return;
    }
    System_printf("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 1) == 0U)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }

    /* Close the driver: */
    SPI_close(handle);
    System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback test DMA enabled 8bits
     **************************************************************************/
    snprintf(testCase, 64, "SPI loopback test - instance(%d), 8bits non-DMA mode", inst);

    /* Change dma configuration */
    params.dmaEnable = 0;
    params.dataSize =8;

    handle = SPI_open(inst, &params);
    if (handle == NULL)
    {
        System_printf("Error: Unable to open the SPI Instance\n");
        return;
    }
    System_printf("Debug: SPI Instance %p has been opened successfully\n", handle);

    /* Start Internal Loopback Test in master mode */
    if(Test_spiLoopback(handle, slaveIndex, params.u.masterParams.slaveProf[slaveIndex].ramBufLen, 1) == 0U)
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ((const char *)&testCase[0], MCPI_TestResult_FAIL);
    }

    /* Close the driver: */
    SPI_close(handle);
    System_printf("Debug: SPI Instance %p has been closed successfully\n", handle);

    /**************************************************************************
     * Test: Reopen the driver in master mode for loopback throughput test
     **************************************************************************/
    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 1000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 2000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 6000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 10000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 20000000U);

    /* Start Loopback throughput Test in master mode */
    Test_spiLoopBackDataThroughput(inst, 40000000U);
}

/**
 *  @b Description
 *  @n
 *      SPI loopback test.
 *
 *   @param[in] inst               SPI instance: 0-SPIA, 1-SPIB
 *
 *  @retval
 *      Not Applicable.
 */
void Test_XWR1xxxWithXWR1xxx(SOC_Handle socHandle)
{
    int32_t         errCode;


    /***********************************************************
     *****************  4 Pin mode testing ************************
     ***********************************************************/
    if(gXWR1xxxMasterWithXWR1xxx)
    {
        /* Enable output control for SPIA in 4Pin mode */
        if(SOC_SPIOutputCtrl(socHandle, 0U, 0U, &errCode) < 0)
        {
            /* Debug Message: */
            System_printf ("Debug: SOC_SPIOutputCtrl failed with Error [%d]\n", errCode);
            return;
        }

        Test_spiWithXWR1xxx(true, 1000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 1MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 2000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 2MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 6000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 6MHz passed!\n");

        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 10000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 10MHz passed!\n");

        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 20000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 20MHz passed!\n");

        Task_sleep(3000);
        Test_spiWithXWR1xxx(true, 40000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 40MHz passed!\n");
    }
    else if(gXWR1xxxSlaveWithXWR1xxx)
    {
        /* Enable output control for SPIA in 4Pin mode */
        if(SOC_SPIOutputCtrl(socHandle, 0U, 0U, &errCode) < 0)
        {
            /* Debug Message: */
            System_printf ("Debug: SOC_SPIOutputCtrl failed with Error [%d]\n", errCode);
            return;
        }

        Test_spiWithXWR1xxx(false, 1000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 2000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 6000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 10000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 20000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 40000000U, SPI_PINMODE_4PIN_CS);
    }

    /* Swap master and slave re-do the test */
    if(gXWR1xxxMasterWithXWR1xxx)
    {
        Test_spiWithXWR1xxx(false, 1000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 2000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 6000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 10000000U, SPI_PINMODE_4PIN_CS);
        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 20000000U, SPI_PINMODE_4PIN_CS);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 40000000U, SPI_PINMODE_4PIN_CS);
    }
    else if(gXWR1xxxSlaveWithXWR1xxx)
    {
        Task_sleep(5000);

        Test_spiWithXWR1xxx(true, 1000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 1MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 2000000U, SPI_PINMODE_4PIN_CS);

        System_printf("Debug: 4pin mode 2MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 6000000U, SPI_PINMODE_4PIN_CS);

        System_printf("Debug: 4pin mode 6MHz passed!\n");

        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 10000000U, SPI_PINMODE_4PIN_CS);

        System_printf("Debug: 4pin mode 10MHz passed!\n");

        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 20000000U, SPI_PINMODE_4PIN_CS);

        System_printf("Debug: 4pin mode 20MHz passed!\n");

        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 40000000U, SPI_PINMODE_4PIN_CS);
        System_printf("Debug: 4pin mode 40MHz passed!\n");

        System_printf("Debug: 4pin mode passed!\n");
    }

    /***********************************************************
     *****************  3 Pin mode testing ************************
     ***********************************************************/
    if(gXWR1xxxMasterWithXWR1xxx)
    {

        /* Enable output control for SPIA 3-pin Mode */
        if(SOC_SPIOutputCtrl(socHandle, 0U, 1U, &errCode) < 0)
        {
            /* Debug Message: */
            System_printf ("Debug: SOC_SPIOutputCtrl failed with Error [%d]\n", errCode);
            return;
        }
        Task_sleep(5000);

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 1000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 1MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 2000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 2MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 6000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 6MHz passed!\n");

        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 10000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 10MHz passed!\n");

#ifndef SPI_MULT_ICOUNT_SUPPORT
        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 20000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 20MHz passed!\n");
#endif
    }
    else if(gXWR1xxxSlaveWithXWR1xxx)
    {
        /* Enable output control for SPIA 3-pin Mode */
        if(SOC_SPIOutputCtrl(socHandle, 0U, 1U, &errCode) < 0)
        {
            /* Debug Message: */
            System_printf ("Debug: SOC_SPIOutputCtrl failed with Error [%d]\n", errCode);
            return;
        }

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 1000000U, SPI_PINMODE_3PIN);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 2000000U, SPI_PINMODE_3PIN);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 6000000U, SPI_PINMODE_3PIN);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 10000000U, SPI_PINMODE_3PIN);

#ifndef SPI_MULT_ICOUNT_SUPPORT
        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 20000000U, SPI_PINMODE_3PIN);
#endif
    }

    if(gXWR1xxxMasterWithXWR1xxx)
    {
        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 1000000U, SPI_PINMODE_3PIN);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 2000000U, SPI_PINMODE_3PIN);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 6000000U, SPI_PINMODE_3PIN);

        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 10000000U, SPI_PINMODE_3PIN);

#ifndef SPI_MULT_ICOUNT_SUPPORT
        Task_sleep(100);
        Test_spiWithXWR1xxx(false, 20000000U, SPI_PINMODE_3PIN);
#endif
    }
    else if(gXWR1xxxSlaveWithXWR1xxx)
    {
        Task_sleep(5000);
        Test_spiWithXWR1xxx(true, 1000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 1MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 2000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 2MHz passed!\n");

        Task_sleep(1000);
        Test_spiWithXWR1xxx(true, 6000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 6MHz passed!\n");

        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 10000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 10MHz passed!\n");

#ifndef SPI_MULT_ICOUNT_SUPPORT
        Task_sleep(2000);
        Test_spiWithXWR1xxx(true, 20000000U, SPI_PINMODE_3PIN);
        System_printf("Debug: 3pin mode 20MHz passed!\n");
#endif

        System_printf("Debug: 3pin mode passed!\n");
    }
}
