/*
 *   @file  sbl.c
 *
 *   @brief
 *      Top level functions for initializing the peripherals,
 *      downloading the application meta image to the FLASH using
 *      serial interface AND functions for downloading the meta image
 *      from SFLASH to the RAM.
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
#include <string.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/arm/v7a/Pmu.h>

/* MMWSDK include files */
#include <ti/common/sys_common.h>
#include <ti/drivers/dma/dma.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/SemaphoreP.h>

/* SBL internal include file. */
#include <ti/utils/sbl/include/sbl_internal.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/
#define SBL_ERASE_SIZE_KB               1024U
#define SBL_PING_BUFFER_SIZE            2048U
#define SBL_BUFFER_LENGTH               (SBL_PING_BUFFER_SIZE * 2U)

/**************************************************************************
 *************************** External definitions *************************
 **************************************************************************/
/* Location where the vector table will be copied. It is placed in the
 * VECTORS section at location 0x0 using the linker command file */
extern uint8_t      _appVecs[SBL_VECTOR_TABLE_SIZE];

extern void SBL_mpuConfigDefault(void);
extern void SBL_platformInit(void);

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
/* Data buffer used to load the image from flash to RAM */
#pragma DATA_ALIGN(dataBuff, 64);
uint8_t dataBuff[SBL_BUFFER_LENGTH];

/* Global SBL Master Control Block */
SBL_MCB             gSblMCB;

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/
static void inline SBL_switchBuffer();

/**************************************************************************
 *************************** Function Definitions *************************
 **************************************************************************/
/*!
 *  @b Description
 *  @n
 *      Switches between Ping and Pong Buffer
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
static void inline SBL_switchBuffer()
{
    if(gSblMCB.currBuff == gSblMCB.pingAddr)
    {
        gSblMCB.currBuff = gSblMCB.pongAddr;
    }
    else
    {
        gSblMCB.currBuff = gSblMCB.pingAddr;
    }
}

/*!
 *  @b Description
 *  @n
 *      Manages the parsing of buffers
 *
 *  @param[in]  arg0
 *      Task Arg0
 *  @param[in]  arg1
 *      Task Arg1
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
void SBL_bufferMgmtTask(UArg arg0, UArg arg1)
{
    while(1)
    {
        SemaphoreP_pend(gSblMCB.threadCommSem1, SemaphoreP_WAIT_FOREVER);
        if(gSblMCB.pingAddr == gSblMCB.currBuff)
        {
            SBL_bufferMgmt((uint32_t*)gSblMCB.pongAddr, SBL_PING_BUFFER_SIZE, 0);
        }
        else
        {
            SBL_bufferMgmt((uint32_t*)gSblMCB.pingAddr, SBL_PING_BUFFER_SIZE, 0);
        }

        SemaphoreP_post(gSblMCB.threadCommSem2);
    }
}

/*!
 *  @b Description
 *  @n
 *      Downloads the Application Metaimage from FLASH to respective RAMs
 *
 *  @param[in]  qspiFlashHandle
 *      Handle of QSPI Flash module.
 *  @param[in]  flashAddr
 *      Address of SFLASH location where the application meta image is written to.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   SBL Error code
 */
int32_t SBL_imageLoader(QSPIFlash_Handle qspiFlashHandle, uint32_t flashAddr)
{
    Task_Params         taskParams;
    SemaphoreP_Params   semParams;
    DMA_Params          dmaParams;
    DMA_Handle          dmaHandle = NULL;
    uint32_t            dmaChan = 10U;
    uint32_t            rdPtr = (uint32_t)flashAddr;
    uint32_t            totalLength;
    uint32_t            numBlocks;
    uint32_t            index = 0U;
    uint32_t            remainingBytes;
    uint8_t             isLastBuffer = 0U;
    int32_t             retVal = 0U;
    uint32_t            loopCnt;

    /* Initialize DMA parameters */
    DMA_Params_init(&dmaParams);

    gSblMCB.bufferParserState = SBL_BUFFER_PARSER_STATE_IDLE;
    SBL_parserInitialization();
    SBL_imageDecryptParamsInit();
    SBL_imageAuthParamsInit();
    gSblMCB.lessThan2048 = 0U;
    gSblMCB.metaHdrAbsent = 0U;
    gSblMCB.errorStatus = 0U;
    gSblMCB.pingAddr = &dataBuff[0];
    gSblMCB.pongAddr = &dataBuff[SBL_PING_BUFFER_SIZE];

    /* Open DMA driver instance 0 */
    dmaHandle = DMA_open(0, &dmaParams, &retVal);

    if(dmaHandle == NULL)
    {
        SBL_printf("Error: Open DMA driver failed [Error code %d]\r\n", retVal);
        goto exitImageLoader;
    }

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    gSblMCB.threadCommSem1 = SemaphoreP_create(0, &semParams);
    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    gSblMCB.threadCommSem2 = SemaphoreP_create(0, &semParams);

    SemaphoreP_pend(gSblMCB.threadCommSem1, SemaphoreP_NO_WAIT);
    SemaphoreP_pend(gSblMCB.threadCommSem2, SemaphoreP_NO_WAIT);


    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6U * 1024U;
    taskParams.priority = 1;
    Task_create(SBL_bufferMgmtTask, &taskParams, NULL);

    /* Memory map read back from FLASH in Quad mode */
    QSPIFlash_configMmapRead(qspiFlashHandle, FLASH_QUAD_MODE, &retVal);

    gSblMCB.currBuff = gSblMCB.pingAddr;

    /* Fill the ping buffer */
    QSPIFlash_sysDmaRead(qspiFlashHandle, dmaHandle, dmaChan, (rdPtr + index), (uint32_t)gSblMCB.currBuff, SBL_PING_BUFFER_SIZE / sizeof(uint64_t));

    /* Load the input buffer to the RAM */
    SBL_bufferMgmt((uint32_t*)gSblMCB.currBuff, SBL_PING_BUFFER_SIZE, 0);

    /* Switch the buffer */
    SBL_switchBuffer();

    /* Check if the Meta header is present */
    if ((gSblMCB.metaHdrAbsent == 1U))
    {
        retVal = MINUS_ONE;
    }
    else
    {
        if(gSblMCB.bufferParserState == SBL_BUFFER_PARSER_COMPLETED)
        {
            retVal = MINUS_ONE;
        }
        else
        {
            /* Get the meta image size */
            totalLength = gSblMCB.metaHeader.imageSize;
            numBlocks = (totalLength / SBL_PING_BUFFER_SIZE);

            index += SBL_PING_BUFFER_SIZE;

            /* Handle case where only 2 2048  aligned buffers possible */
            if (numBlocks == 2U)
            {
                QSPIFlash_sysDmaRead(qspiFlashHandle, dmaHandle, dmaChan, (rdPtr + index), (uint32_t)gSblMCB.currBuff, SBL_PING_BUFFER_SIZE / sizeof(uint64_t));

                SBL_bufferMgmt((uint32_t*)gSblMCB.currBuff, SBL_PING_BUFFER_SIZE, isLastBuffer);

                /* Switch the buffer */
                SBL_switchBuffer();
                index += SBL_PING_BUFFER_SIZE;
            }

            /* Download the Metaimage to RAM in chunks of 2K */
            for (loopCnt = 2U; ((loopCnt < (numBlocks)) && \
                        (gSblMCB.bufferParserState != SBL_BUFFER_PARSER_COMPLETED)); loopCnt++)
            {
                if (loopCnt == 2U)
                {
                    QSPIFlash_sysDmaRead(qspiFlashHandle, dmaHandle, dmaChan, (rdPtr + index), (uint32_t)gSblMCB.currBuff, SBL_PING_BUFFER_SIZE / sizeof(uint64_t));

                    /* Switch the buffer */
                    SBL_switchBuffer();
                    index += SBL_PING_BUFFER_SIZE;
                }

                SemaphoreP_post(gSblMCB.threadCommSem1);
                QSPIFlash_sysDmaRead(qspiFlashHandle, dmaHandle, dmaChan, (rdPtr + index), (uint32_t)gSblMCB.currBuff, SBL_PING_BUFFER_SIZE / sizeof(uint64_t));
                SemaphoreP_pend(gSblMCB.threadCommSem2, SemaphoreP_WAIT_FOREVER);

                /* Switch the buffer */
                SBL_switchBuffer();
                index += SBL_PING_BUFFER_SIZE;
            }

            SemaphoreP_post(gSblMCB.threadCommSem1);
            SemaphoreP_pend(gSblMCB.threadCommSem2, SemaphoreP_WAIT_FOREVER);

            /* Get the remaining bytes (non 2K) */
            remainingBytes = (totalLength - index);

            if ((gSblMCB.lessThan2048 == 1U) && (gSblMCB.bufferParserState != SBL_BUFFER_PARSER_COMPLETED))
            {
                gSblMCB.bufferParserState = SBL_BUFFER_PARSER_COMPLETED;
                retVal = MINUS_ONE;
            }

            /* Handle the remaining bytes. */
            if ((remainingBytes  != 0U) && (gSblMCB.bufferParserState != SBL_BUFFER_PARSER_COMPLETED))
            {
                QSPIFlash_sysDmaRead(qspiFlashHandle, dmaHandle, dmaChan, (rdPtr + index), (uint32_t)gSblMCB.currBuff, (remainingBytes / sizeof(uint64_t)));

                SBL_bufferMgmt((uint32_t*)gSblMCB.currBuff, ((remainingBytes / sizeof(uint64_t)) * sizeof(uint64_t)), isLastBuffer);

                index += ((remainingBytes / sizeof(uint64_t)) * sizeof(uint64_t));
                if ((remainingBytes % sizeof(uint64_t)) != 0U)
                {
                    QSPIFlash_mmapRead(qspiFlashHandle, (uint32_t *)(rdPtr + index), 1, (uint32_t *)gSblMCB.currBuff);
                    isLastBuffer = 1U;
                    SBL_bufferMgmt((uint32_t*)gSblMCB.currBuff, (remainingBytes % sizeof(uint64_t)), isLastBuffer);
                }
                else
                {
                    /* If the last buffer is exactly 8 byte aligned. Trigger parsing. */
                    isLastBuffer = 1U;
                    SBL_bufferMgmt((uint32_t*)gSblMCB.currBuff, 0U, isLastBuffer);
                }
            }
            else
            {
                /* If the last buffer is exactly 2048 byte aligned. Trigger parsing. */
                isLastBuffer = 1U;
                SBL_bufferMgmt((uint32_t*)gSblMCB.currBuff, 0U, isLastBuffer);
            }
        }

    }
exitImageLoader:
    return retVal;
}

/*!
 *  @b Description
 *  @n
 *      This function download the application meta imageover over a device peripheral.
 *      It does the following:
 *      1. Erases the portion of SFLASH where the Metaimage has to be stored.
 *      2. Receives the Metaimage over transport interface.
 *      3. Stores the image into the SFLASH.
 *
 *  @param[in]  qspiFlashHandle
 *      Handle of QSPI Flash module.
 *  @param[in]  flashAddr
 *      Address of SFLASH location where the application meta image is written to.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   SBL Error code
 */
int32_t SBL_imageFlasher(QSPIFlash_Handle qspiFlashHandle, uint32_t flashAddr)
{
    int32_t     totDataLen = 0;
    int32_t     retVal = 0;
    uint32_t    numSectErase = 0;
    uint32_t    index;

    SBL_printf("Debug: Erasing SFlash...\r\n");

    /* Calculate the number of 4K sectors to erase. */
    numSectErase = (SBL_ERASE_SIZE_KB + 3) / 4;

    /* Erase FLASH memory*/
    for (index = 0; index < numSectErase; index++)
    {
        retVal = QSPIFlash_sectorErase(qspiFlashHandle, flashAddr + (index * 4096));
        if(retVal != 0)
        {
            goto exitImageFlasher;
        }
    }
    SBL_printf("Debug: Flash Erase complete\r\n");

    totDataLen = SBL_transportDownloadFile (qspiFlashHandle, flashAddr, SBL_MAX_METAIMAGE_SIZE);

    if (totDataLen < 0)
    {
        SBL_printf("Error: File download failed [Error code %d]\r\n", totDataLen);
        retVal = totDataLen;
    }
    else
    {
        SBL_printf("Debug: Total data written = 0x%x\r\n", totDataLen);
    }
exitImageFlasher:
    return retVal;
}

/*!
 *  @b Description
 *  @n
 *      Decides whether the application acts as meta Image Flasher or
 *      Image Loader based upon user input.
 *
 *  @param[in]  arg0
 *      Task Arg0
 *  @param[in]  arg1
 *      Task Arg1
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
void SBL_initTask(UArg arg0, UArg arg1)
{
    int32_t             retVal = 0;
    uint32_t            flashAddr = 0U;
    SPIFLASH_devID      devId;
    uint8_t             userInput = 0;
    uint8_t             autoboot = SBL_AUTOBOOT_COUNT;
    uint32_t            metaimageUpdate = 0;
    QSPI_Params         QSPIParams;
    QSPIFlash_Handle    qspiFlashHandle = NULL;

    /* Initialize transport peripheral */
    SBL_transportInit();

    /* Configure transport peripheral */
    retVal = SBL_transportConfig();

    /* Check if the transport peripheral was configured successfully? */
    if (retVal != 0)
    {
        DebugP_assert(0);
    }

    SBL_printf ("\r\n");
    SBL_printf ("**********************************************\r\n");
    SBL_printf ("Debug: Secondary Bootloader Application Start \r\n");
    SBL_printf ("**********************************************\r\n");

    SBL_printf ("Press any key to stop auto boot and Update Meta Image...\r\n");
    SBL_printf ("Loading existing Meta Image from Flash in ");

    do
    {
        retVal = SBL_transportRead((uint8_t*)&userInput, 1U);

        /* Check if user interrupted the autoboot */
        if (retVal == 1U)
        {
            metaimageUpdate = 1;
            SBL_printf ("\r\nDebug: Update Meta Image selected\r\n");
        }
        else
        {
            SBL_printf ("  %d", autoboot--);
            continue;
        }
    }while((retVal == 0) && (autoboot != 0));

    SBL_printf ("\r\n");

    /* Initialize the QSPI Driver */
    QSPI_init();

    /* Initialize the QSPI Flash */
    QSPIFlash_init();

    /* Open QSPI driver */
    QSPI_Params_init(&QSPIParams);

    /* Set the QSPI peripheral clock  */
    QSPIParams.qspiClk = SOC_getMSSVCLKFrequency(gSblMCB.socHandle, &retVal);

    QSPIParams.clkMode = QSPI_CLOCK_MODE_0;

    /* Running at 40MHz QSPI bit rate
     * QSPI bit clock rate derives from QSPI peripheral clock(qspiClk)
     and divide clock internally down to bit clock rate
     BitClockRate = qspiClk/divisor(setup by QSPI driver internally)
     */
    QSPIParams.bitRate = 40 * 1000000U;

    gSblMCB.qspiHandle = QSPI_open(&QSPIParams, &retVal);

    if (gSblMCB.qspiHandle == NULL)
    {
        SBL_printf("Error: QSPI_open failed [Error code %d]\r\n", retVal);
        DebugP_assert(0);
    }

    /* Open the QSPI Instance */
    qspiFlashHandle = QSPIFlash_open(gSblMCB.qspiHandle, &retVal);
    if (qspiFlashHandle == NULL)
    {
        SBL_printf("Error: QSPIFlash Open API failed.\r\n");
        DebugP_assert(0);
    }

    /* Get SPI Flash id */
    QSPIFlash_getDeviceID(qspiFlashHandle, &devId);

    /* get the flash address */
    flashAddr = QSPIFlash_getExtFlashAddr(qspiFlashHandle);

    SBL_printf("Debug: Device info: Manufacturer: %x, Device type = %x, Capacity = %x\r\n",
                    devId.Manufacture, devId.device, devId.capacity);

    SBL_printf("\r\nDebug: Loading application metaImage from Flash address: %x\r\n",
                    (flashAddr + (uint32_t)SBL_METAIMAGE_OFFSET));

    /*  Update the metaimage in the Sflash using choosen device peripheral. */
    if(metaimageUpdate == 1)
    {
        retVal = SBL_imageFlasher(qspiFlashHandle, (flashAddr + (uint32_t)SBL_METAIMAGE_OFFSET));
        if(retVal != 0)
        {
            SBL_printf("\r\nError: Could not download the image to Flash. Resetting the board to retry\r\n");

            /* Reset the MSS core */
            SOC_softReset(gSblMCB.socHandle, &retVal);
        }
    }

    /* Download the metaimage present in the SFLASH into the RAM */
    retVal = SBL_imageLoader(qspiFlashHandle, (flashAddr + (uint32_t)SBL_METAIMAGE_OFFSET));

    if(retVal != 0)
    {
        SBL_printf("\r\nError: Could not download the metaimage to RAM. Trying to boot the backup factory default image.\r\n");
        SBL_printf("\r\nDebug: Loading backup factory default image from Flash address: %x\r\n",
                       (flashAddr + (uint32_t)SBL_BACKUP_IMAGE_OFFSET));

        /* Error loading image. Fall back to factory default image */
        retVal = SBL_imageLoader(qspiFlashHandle, (flashAddr + (uint32_t)SBL_BACKUP_IMAGE_OFFSET));
        if(retVal != 0)
        {
            SBL_printf("\r\nError: Could not download the backup factory default image metaimage to RAM. Resetting the board to retry\r\n");

            /* Reset the MSS core */
            SOC_softReset(gSblMCB.socHandle, &retVal);
        }
    }

    /* Loading was successful, cleanup and restore the interrupt vector table */
    if (retVal == 0)
	{
	    QSPIFlash_close(qspiFlashHandle);
		QSPI_close(gSblMCB.qspiHandle);
        SBL_transportDeinit();
		memcpy((void*)&_appVecs[0], (void*)&gSblMCB.sblIntVecTable, SBL_VECTOR_TABLE_SIZE);
    }

    /* Reset the MSS core */
    SOC_softReset(gSblMCB.socHandle, &retVal);

    BIOS_exit(0);

    return;
}

/*!
 *  @b Description
 *  @n
 *      Called from main.c that runs in TCMA memory.
 *      It initializes the SOC, Configures MPU, halts DSS, performs platform
 *      related intializations and launches the init task.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
void SBL_init(void)
{
    Task_Params     taskParams;
    int32_t         errCode;
    SOC_Cfg         socCfg;

    /* Initialize the SBL master control block */
    memset ((void *)&gSblMCB, 0, sizeof(SBL_MCB));

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_BYPASS_INIT;

    /* MPU is configured by SBL. Bypass the default configuration. */
    socCfg.mpuCfg = SOC_MPUCfg_BYPASS_CONFIG;

    /* DSS MUST be in halted state suring SBL execution.
     * Failure to do so will cause a crash.
     */
    socCfg.dssCfg = SOC_DSSCfg_HALT;

    /* Initialize the SOC Module: The default MPU configuration is bypassed. */
    gSblMCB.socHandle = SOC_init (&socCfg, &errCode);

    DebugP_assert (gSblMCB.socHandle != NULL);

    /* Configure the MPU for MSS regions, DSS regions and peripherals after calling SOC init.
     * BSS regions will be configured when BSS image is downloaded.
     */
    SBL_mpuConfigDefault();

    /* Initialize the platform */
    SBL_platformInit();

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6U * 1024U;
    taskParams.priority = 2U;
    Task_create(SBL_initTask, &taskParams, NULL);

    /* Start BIOS */
    BIOS_start();
}
