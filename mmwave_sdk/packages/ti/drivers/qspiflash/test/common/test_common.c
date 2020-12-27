/*
 *   @file  qspiflash_testCommon.c
 *
 *   @brief
 *      Unit Test code for the MibSpi Driver
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
#include <ti/drivers/qspiflash/qspiflash.h>
#include <ti/drivers/pinmux/pinmux.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/esm/esm.h>

/* TOP RCM register file */
#include <ti/drivers/soc/include/reg_toprcm.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/dma/dma.h>

/* MCPI logger include files */
#include <ti/utils/testlogger/logger.h>

#define TEST_DATA_LENGTH    4096U

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/*! L3 RAM for Tx data buffer */
#pragma DATA_SECTION(gTestDataArray, ".l3Data");
#pragma DATA_ALIGN(gTestDataArray, 64);
uint8_t  gTestDataArray[TEST_DATA_LENGTH];

/* System DMA handle, created in init Task */
DMA_Handle          gDmaHandle = NULL;
QSPI_Handle   gQSPIDrv = (QSPI_Handle)NULL;

/* Macro used in test */
#define SWAP_BYTES_IN_HALFWORD(x)      (( (x&0xff) << 8) | ((x >>8) & 0xff))
#define SWAP_BYTES_IN_WORD(x)      ( ((x&0xff) << 24) | ((x&0xff00) << 8) | ((x&0xff0000) >> 8) | ((x >>24) & 0xff) )

/**************************************************************************
 *************************** QSPI Test Functions *******************************
 **************************************************************************/

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
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *   @param[in] arg0               Task Arg0
 *   @param[in] arg1               Task Arg1
 *
 *  @retval
 *      Not Applicable.
 */
void Test_initTask(UArg arg0, UArg arg1)
{
    DMA_Params      dmaParams;
    int32_t         retVal = 0;
    uint32_t        flashAddr = 0U;
    SPIFLASH_devID  devId;
    uint32_t        index;
    uint32_t        testDataLen = 0U;
    uint8_t*        readDataArray;
    QSPI_Params     QSPIParams;
    QSPIFlash_Handle QSPIFlashHandle = NULL;
    uint32_t        dmaChan = 10;
    char            name[16] = {0};
    char            testCase[128];

    /**************************************************************************
     * Test: MCPI_Initialize
     **************************************************************************/
    /* Initialize MCPI logger framework */
    MCPI_Initialize ();

    /* Initialize the QSPI Driver */
    QSPI_init();

    /* Initialize the QSPI Flash */
    QSPIFlash_init();

    /* Open QSPI driver */
    QSPI_Params_init(&QSPIParams);

    /* Set the QSPI peripheral clock to 200MHz  */
    QSPIParams.qspiClk = 200 * 1000000U;

    QSPIParams.clkMode = QSPI_CLOCK_MODE_0;

    /* Running at 40MHz QSPI bit rate
     * QSPI bit clock rate derives from QSPI peripheral clock(qspiClk)
       and divide clock internally down to bit clock rate
       BitClockRate = qspiClk/divisor(=5, setup by QSPI driver internally)
     */
    QSPIParams.bitRate = 40 * 1000000U;

    gQSPIDrv = QSPI_open(&QSPIParams, &retVal);

    if(gQSPIDrv == NULL)
    {
        printf("QSPI_open failed with error=%d\n", retVal);
        goto Exit;
    }

    /* Init SYSDMA params */
    DMA_Params_init(&dmaParams);

    /* Open DMA driver instance 0 for SPI test */
    gDmaHandle = DMA_open(0, &dmaParams, &retVal);

    if(gDmaHandle == NULL)
    {
        printf("Open DMA driver failed with error=%d\n", retVal);
        return;
    }

    readDataArray = Memory_alloc (NULL, TEST_DATA_LENGTH, 64, NULL);
    if(readDataArray == NULL)
    {
        printf("Test array allocation failed\n");
    }
    else
    {
        printf("Test array allocation -> 0x%x\n\n", readDataArray);
    }

    /**************************************************************************
     * Test: Open API test
     **************************************************************************/

    /* Open the QSPI Instance */
    QSPIFlashHandle = QSPIFlash_open(gQSPIDrv, &retVal);
    if (QSPIFlashHandle == NULL )
    {
        MCPI_setFeatureTestResult ("QSPIFlash Open API ", MCPI_TestResult_FAIL);
        goto Exit;
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash Open API ", MCPI_TestResult_PASS);

        /* Graceful shutdown */
        QSPIFlash_close(QSPIFlashHandle);
        System_printf("Debug: QSPIFlash Instance has been closed successfully\n");
    }

    /**************************************************************************
     * Test: Re-open for additional tests
     **************************************************************************/
    QSPIFlashHandle = QSPIFlash_open(gQSPIDrv, &retVal);
    if (QSPIFlashHandle == NULL )
    {
        MCPI_setFeatureTestResult ("QSPIFlash re-Open API ", MCPI_TestResult_FAIL);
        goto Exit;
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash re-Open API ", MCPI_TestResult_PASS);
    }

    /**************************************************************************
     * Test: Read ID API test
     **************************************************************************/

    /* Get SPI Flash id */
    QSPIFlash_getDeviceID(QSPIFlashHandle, &devId);

    if(devId.Manufacture == SPANSION_DEV)
    {
        snprintf(name, 16, "%s", "SPANSION");
    }
    else if (devId.Manufacture == MACRONIX_DEV)
    {
        snprintf(name, 16, "%s", "MACRONIX");
    }
    else
    {
        snprintf(name, 16, "%s", "UNKNOWN");
    }

    /* Log device information */
    snprintf(testCase, 128, "QSPIFlash Read Id API: Manufacture %s(0x%x), device type = 0x%x, capacity = 0x%x",
        name,
        devId.Manufacture,
        devId.device,
        devId.capacity);

    if((devId.Manufacture != 0U) && (devId.capacity != 0U))
    {
        MCPI_setFeatureTestResult (testCase, MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult (testCase, MCPI_TestResult_FAIL);
        goto Exit;
    }

    /**************************************************************************
     * Test: single write, single read
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xab + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    QSPIFlash_singleRead(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)&readDataArray[0]);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /single read API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /single read API", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: single write , dual read
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0x55 + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr += 0x181000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    QSPIFlash_dualRead(QSPIFlashHandle, flashAddr, testDataLen/2U, (uint16_t *)&readDataArray[0]);

    /* Swap two bytes in one half word */
    {
        uint16_t        *data = (uint16_t *)readDataArray;

        for (index = 0; index <testDataLen/2U; index++)
        {
            data[index] = SWAP_BYTES_IN_HALFWORD(data[index]);
        }
    }
    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /dual read API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /dual read API", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: single write , quad read
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0x99 + index;
    }
    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr += 0x181000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    QSPIFlash_quadRead(QSPIFlashHandle, flashAddr, testDataLen/4U, (uint32_t *)&readDataArray[0]);

    /* Swap four bytes in one word */
    {
        uint32_t        *data = (uint32_t *)readDataArray;

        for (index = 0; index <testDataLen/2U; index++)
        {
            data[index] = SWAP_BYTES_IN_WORD(data[index]);
        }
    }

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /quad read API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /quad read API", MCPI_TestResult_FAIL);
    }
    /**************************************************************************
     * Test: single write and memory Read in Memory mapped mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xab + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_QUAD_MODE, &retVal)< 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_mmapRead(QSPIFlashHandle, (uint32_t *)flashAddr, testDataLen/8U, (uint32_t *)&readDataArray[0]);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap read test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap read test", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: single write and dma Read in Memory mapped quadRead mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xab + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_QUAD_MODE, &retVal) < 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(quad read) dma read test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(quad read) dma read test", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: single write and dma Read in Memory mapped dualRead mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xb0 + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x100000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_DUAL_MODE, &retVal) < 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }


    QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(dual read) dma read test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(dual read) dma read test", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: single write and dma Read in Memory mapped singleRead mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xb0 + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x100000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_SINGLE_MODE, &retVal) < 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(single read) dma read test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(single read) dma read test", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: single write and dma Read in Memory mapped auto Read mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xb7 + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x100000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_AUTO_MODE, &retVal) < 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(auto select read) dma read test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /mmap(auto select read) dma read test", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: Read/Write data Test in Memory mapped mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xbb + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    /* Memory Write to FLASH in config mode */
    QSPIFlash_configMmapWrite(QSPIFlashHandle);

    QSPIFlash_mmapWrite(QSPIFlashHandle, (uint32_t *)flashAddr, testDataLen/8U, (uint32_t *)&gTestDataArray[0]);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_QUAD_MODE, &retVal)< 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_mmapRead(QSPIFlashHandle, (uint32_t *)flashAddr, testDataLen/8U, (uint32_t *)&readDataArray[0]);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash mmap read /write API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash mmap read /write API", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: DMA mode Read/Write data Test in Memory mapped mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xcd + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 256U * 4U;

    memset((void *)&readDataArray[0], 0, testDataLen);

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    /* Memory Write to FLASH in config mode */
    QSPIFlash_configMmapWrite(QSPIFlashHandle);

    /* Setup DMA for TX */
    QSPIFlash_mmapWrite(QSPIFlashHandle, (uint32_t *)flashAddr, testDataLen/8U, (uint32_t *)&gTestDataArray[0]);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    memset((void *)&readDataArray[0], 0, testDataLen);

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_AUTO_MODE, &retVal) < 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash mmap write /dma read test", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash mmap write /dma read test", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: DMA mode Read/Write data Test in Memory mapped mode
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xdd + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 256U * 4U;

    memset((void *)&readDataArray[0], 0, testDataLen);

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    /* Memory Write to FLASH in config mode */
    QSPIFlash_configMmapWrite(QSPIFlashHandle);

    /* Setup DMA for TX */
    QSPIFlash_sysDmaWrite(QSPIFlashHandle, gDmaHandle, dmaChan, (uint32_t )&gTestDataArray[0], flashAddr, testDataLen/8U);

    /* Wait for data landed in memory before reading */
    MEM_BARRIER();

    memset((void *)&readDataArray[0], 0, testDataLen);

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_QUAD_MODE, &retVal)<0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash dma read /write API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash dma read /write API", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: single write, single read
     **************************************************************************/

    /* Generate data */
    for (index = 0; index < TEST_DATA_LENGTH; index++)
    {
        gTestDataArray[index] = 0xab + index;
    }

    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 256U * 4U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    QSPIFlash_singleWrite(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)gTestDataArray);

    QSPIFlash_singleRead(QSPIFlashHandle, flashAddr, testDataLen, (uint8_t *)&readDataArray[0]);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /single read API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash single write /single read API", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: sector Erase
     **************************************************************************/
    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 4096U;

    /* Erase a sector for test */
    QSPIFlash_sectorErase(QSPIFlashHandle, flashAddr);

    memset((void *)&gTestDataArray[0], 0xFF, testDataLen);
    memset((void *)&readDataArray[0], 0x0, testDataLen);

    /* Memory map read back from FLASH in Quad mode */
    if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_AUTO_MODE, &retVal) < 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
    }

    QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

    /* Check data */
    retVal = memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;
    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash sector erase API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash sector erase API", MCPI_TestResult_FAIL);
    }

    /**************************************************************************
     * Test: block Erase
     **************************************************************************/
    flashAddr = QSPIFlash_getExtFlashAddr(QSPIFlashHandle);

    /* Set flash address for  the test */
    flashAddr = flashAddr + 0x180000U;
    testDataLen = 4096U;

    /* Erase a sector for test */
    QSPIFlash_blockErase(QSPIFlashHandle, flashAddr);

    memset((void *)&gTestDataArray[0], 0xFF, testDataLen);

    retVal = 0;

    /* Test loop for 16 * 4KB */
    for (index = 0; index < 16U; index++)
    {
        memset((void *)&readDataArray[0], 0x0, testDataLen);

        /* Memory map read back from FLASH in Quad mode */
        if(QSPIFlash_configMmapRead(QSPIFlashHandle, FLASH_AUTO_MODE, &retVal)<0)
        {
            MCPI_setFeatureTestResult ("QSPIFlash setting memory map read ", MCPI_TestResult_FAIL);
        }

        QSPIFlash_sysDmaRead(QSPIFlashHandle, gDmaHandle, dmaChan, flashAddr, (uint32_t)&readDataArray[0], testDataLen/8U);

        /* Check data */
        retVal += memcmp((void *)&gTestDataArray[0], (void *)&readDataArray[0], testDataLen) ;

        /* Change flash address */
        flashAddr += testDataLen;
    }

    if(retVal == 0)
    {
        MCPI_setFeatureTestResult ("QSPIFlash block erase API", MCPI_TestResult_PASS);
    }
    else
    {
        MCPI_setFeatureTestResult ("QSPIFlash block erase API", MCPI_TestResult_FAIL);
    }

Exit:
    /* Graceful shutdown */
    QSPIFlash_close(QSPIFlashHandle);
    System_printf("Debug: QSPIFlash Instance has been closed successfully\n");

    QSPI_close(gQSPIDrv);
    System_printf("Debug: QSPI has been closed successfully\n");

    DMA_close(gDmaHandle);
    System_printf("Debug: DMA has been closed successfully\n");

    System_printf("Debug: QSPIFlash Test is done!\n");

    MCPI_setTestResult ();

    /* After test all done, terminate DSP by calling BIOS_exit().
        This is required by MCPI test framework script.
        MCPI test framework script waits for DSP halt,
        if DSP doesn't halt, script will wait until timeout then claim test fail.
     */
    BIOS_exit(0);

    return;
}
