/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
/** ============================================================================
 *  @file       qspiflash.c
 *
 *  @brief      QSPI Flash driver
 *
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

#include <ti/common/sys_common.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/SemaphoreP.h>

#include <ti/drivers/qspi/qspi.h>
#include <ti/drivers/qspiflash/qspiflash.h>
#include <ti/drivers/qspiflash/include/qspiflash_internal.h>


/* Driver debug flag */
/* #define    QSPIFLASH_DEBUG          1U */
#define     QSPIFLASH_PARAMS_CHECK

/**************************************************************************
 *************************** External  Variables *******************************
 **************************************************************************/
 /* QSPI flash Hardware configuration */
extern QSPIFlash_HWCfg gQSPIFlashHwCfg;

/* Flash device specific Function table */
extern QSPIFlash_deviceFxn QSPIFlash_deviceFxnTbl_spansion;
extern QSPIFlash_deviceFxn QSPIFlash_deviceFxnTbl_macronix;

/**************************************************************************
 *************************** Internal Functions Prototype ***********************
 **************************************************************************/
static uint8_t QSPIFlashReadByte(QSPI_Handle QSPIHandle);
static uint32_t QSPIFlashReadCmd(QSPI_Handle QSPIHandle, uint32_t cmd);
static void QSPIFlashWriteAddr(QSPI_Handle QSPIHandle, uint32_t addr, uint32_t frameLen);
static void QSPIFlashWriteByte(QSPI_Handle QSPIHandle, uint8_t data, uint32_t frameLen);
static void QSPIFlashGetDeviceID(QSPI_Handle QSPIHandle, uint32_t cmd, SPIFLASH_devID* devID);
static void QSPIFlashDefaultCmd_init(SPIFLASH_Params     *ptrFlashParams);
static void QSPIFlashReadSFDP(QSPI_Handle QSPIHandle, uint32_t addr, uint32_t dataLen, uint8_t *receiveData);
static int32_t QSPIFlashReadDeviceParams(QSPIFLASH_Driver       *ptrQSPIFlashDrv);
static uint8_t QSPIFlashSupportModes(QSPIFlash_Handle QSPIFlashHandle);
static void QSPIFlashSysDmaIntHandler(uint32_t dmaChannel, DMA_IntType dmaIntType, void *arg);

/**************************************************************************
 *************************** Internal Functions ********************************
 **************************************************************************/

 /**
 *  @b Description
 *  @n
 *     This function reads one byte from QSPI interface
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval                     A byte data returned from read
 */
static uint8_t QSPIFlashReadByte(QSPI_Handle QSPIHandle)
{
    uint32_t        data;

    QSPI_cfgRead (QSPIHandle, QSPI_CMD_4PIN_READ_SINGLE, QSPI_WORDLEN_8BIT, &data, 1U);
    return ((uint8_t)(data & 0xFFU));
}

 /**
 *  @b Description
 *  @n
 *     This function sends a command to QSPI flash, then read the result back.
 *
 *   @param[in] QSPIHandle          QSPI driver handle
 *   @param[in] cmd                 Read command
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval                         Result data returned from command
 */
static uint32_t QSPIFlashReadCmd(QSPI_Handle QSPIHandle, uint32_t cmd)
{
    uint32_t        data;

    QSPI_cfgWrite (QSPIHandle, QSPI_CMD_4PIN_WRITE_SINGLE, QSPI_WORDLEN_8BIT, cmd, 2U);

    QSPI_cfgRead (QSPIHandle, QSPI_CMD_4PIN_READ_SINGLE, QSPI_WORDLEN_8BIT, &data, 1U);

    return (data);
}

 /**
 *  @b Description
 *  @n
 *     This function reads one word from QSPI interface
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval                     A word data returned from read
 */
static uint32_t QSPIFlashReadWord(QSPI_Handle QSPIHandle)
{
    uint32_t        data;

    QSPI_cfgRead (QSPIHandle, QSPI_CMD_6PIN_READ_QUAD, QSPI_WORDLEN_32BIT, &data, 1U);

    return (data);
}

/**
 *  @b Description
 *  @n
 *      This function sends status command to flash, and read back the status
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *   @param[in] cmd             Status command byte
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval                     Status read back from flash.
 */
uint32_t QSPIFlashGetStatus(QSPI_Handle QSPIHandle, uint32_t cmd)
{
    return (QSPIFlashReadCmd(QSPIHandle, cmd));
}

/**
 *  @b Description
 *  @n
 *      This function reads half word through QSPI driver
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval                     32bits data with half word in lower 16bits.
 */
static uint32_t QSPIFlashReadHalfWord(QSPI_Handle QSPIHandle)
{
    uint32_t        data;

    QSPI_cfgRead (QSPIHandle, QSPI_CMD_4PIN_READ_DUAL, QSPI_WORDLEN_16BIT, &data, 1U);

    return (data);
}

/**
 *  @b Description
 *  @n
 *      This function writes a command to QSPI flash
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *   @param[in] cmd             Flash command to be sent to flash
 *   @param[in] frameLen        Number of bytes of the command
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlashWriteCmd(QSPI_Handle QSPIHandle, uint32_t cmd, uint32_t frameLen)
{
    QSPI_cfgWrite (QSPIHandle, QSPI_CMD_4PIN_WRITE_SINGLE, QSPI_WORDLEN_8BIT, cmd, frameLen);

}

/**
 *  @b Description
 *  @n
 *      This function writes a 24bit address  to QSPI flash
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *   @param[in] addr            Flash address to be sent to flash
 *   @param[in] frameLen        Number of elements for the rest of the operation
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIFlashWriteAddr(QSPI_Handle QSPIHandle, uint32_t addr, uint32_t frameLen)
{
    QSPI_cfgWrite (QSPIHandle, QSPI_CMD_4PIN_WRITE_SINGLE, QSPI_WORDLEN_24BIT, addr, frameLen);
}

/**
 *  @b Description
 *  @n
 *      This function writes 8bit data  to QSPI flash
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *   @param[in] data            Data to be sent to flash
 *   @param[in] frameLen        Number of elements for the rest of the operation
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIFlashWriteByte(QSPI_Handle QSPIHandle, uint8_t data, uint32_t frameLen)
{
    QSPI_cfgWrite (QSPIHandle, QSPI_CMD_4PIN_WRITE_SINGLE, QSPI_WORDLEN_8BIT, (uint32_t)data, frameLen);
}

/**
 *  @b Description
 *  @n
 *     This function reads flash device ID infomation.
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *   @param[in] cmd             Read id command
 *   @param[in] devID           Device Id info read from the flash
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIFlashGetDeviceID(QSPI_Handle QSPIHandle, uint32_t cmd, SPIFLASH_devID* devID)
{
    uint8_t index;
    uint8_t data[6];
    uint32_t status;
    uint8_t dataLen = (uint8_t)0U;

    switch (cmd)
    {
        case FLASH_CMD_DEV_ID:
            dataLen = 5U;
            break;

        case FLASH_CMD_JEDEC_ID:
            dataLen = 3U;
            break;

        default:
            DebugP_assert(0U);
            break;
    }

    /* Write SFDP Command */
    QSPIFlashWriteCmd(QSPIHandle, cmd, dataLen + 1U);

    /* Read Bytes */
    for(index = 0U; index < dataLen; index++)
    {
        data[index] = QSPIFlashReadByte(QSPIHandle);
    }

    /* Check BUSY flag */
    do
    {
       status = QSPIFlashGetStatus(QSPIHandle, FLASH_CMD_RDSR1);
    }while((status & 0x1U) != 0x0U);

    switch (cmd)
    {
        case FLASH_CMD_DEV_ID:
            devID->Manufacture = data[3];
            devID->device     = data[4];
            devID->capacity   = data[4];
            break;

        case FLASH_CMD_JEDEC_ID:
            devID->Manufacture = data[0];
            devID->device     = data[1];
            devID->capacity   = data[2];
            break;

        default:
            DebugP_assert(0U);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *     This function initialize default serial flash command set
 *
 *   @param[in] ptrFlashParams         Pointer to the serial Flash parameters

 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIFlashDefaultCmd_init(SPIFLASH_Params     *ptrFlashParams)
{
    /* Read Status commands */
    ptrFlashParams->readSR1Cmd = FLASH_CMD_RDSR1;
    ptrFlashParams->readSR2Cmd = FLASH_CMD_RDCR_SPANSION;
    ptrFlashParams->readSR3Cmd = FLASH_CMD_RDSR3_SPANSION;

    /* Write enable/disable commands */
    ptrFlashParams->writeEnableCmd = FLASH_CMD_WR_ENABLE;
    ptrFlashParams->writeDisableCmd = FLASH_CMD_WR_DISABLE;

    /* Write status Register commands */
    ptrFlashParams->writeStatusRegCmd = FLASH_CMD_WR_SR;

    /* Page programming commands */
    ptrFlashParams->pageProgramCmd = FLASH_CMD_PAGE_PROGRAM;

    /* Erase commands */
    ptrFlashParams->sectorEraseCmd = FLASH_CMD_SECTOR_ERASE;
    ptrFlashParams->blockEraseCmd = FLASH_CMD_BLOCK_ERASE;
    ptrFlashParams->chipEraseCmd = FLASH_CMD_CHIP_ERASE;

    /* Single read commands */
    ptrFlashParams->readDataCmd = FLASH_CMD_FAST_SINGLE_READ;

    /* Quad read command */
    ptrFlashParams->quadReadCmd = FLASH_CMD_QUAD_READ;

    /* Dual read command */
    ptrFlashParams->dualReadCmd = FLASH_CMD_DUAL_READ;

    /* Enable Reset command */
    ptrFlashParams->resetEnableCmd = FLASH_CMD_RESET_ENABLE;

    /* Reset command */
    ptrFlashParams->resetCmd = FLASH_CMD_RESET;

    /* Reset continuous mode command */
    ptrFlashParams->contModeResetCmd    =FLASH_CMD_CONT_MODE_RESET;

    /* Protection bit mask in status register */
    ptrFlashParams->protectBitsMask = 0;
}

/**
 *  @b Description
 *  @n
 *     This function reads flash SFDP.
 *
 *   @param[in] QSPIHandle      QSPI driver handle
 *   @param[in] addr            Address offset to read
 *   @param[in] dataLen         Data length of the read operation
 *   @param[in] receiveData     Pointer to store the received data
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIFlashReadSFDP(QSPI_Handle QSPIHandle, uint32_t addr, uint32_t dataLen, uint8_t *receiveData)
{
    uint8_t         index;
    uint32_t        status;

    /* Write SFDP Command */
    QSPIFlashWriteCmd(QSPIHandle, FLASH_CMD_SFDP, dataLen + 3U);

    /* Write Address offset */
    QSPIFlashWriteAddr(QSPIHandle, addr, dataLen + 2U);

    /* 8 Dummy cycles - 1 byte */
    QSPI_cfgWrite (QSPIHandle, QSPI_CMD_4PIN_WRITE_SINGLE, QSPI_WORDLEN_8BIT, 0x0, dataLen + 1U);

    /* Read Bytes */
    for(index = 0U; index < dataLen; index++)
    {
        receiveData[index] = QSPIFlashReadByte(QSPIHandle);
    }
    /* Check BUSY flag */
    do
    {
       status = QSPIFlashGetStatus(QSPIHandle, FLASH_CMD_RDSR1);
    }while((status & 0x1U) != 0x0U);

}

/**
 *  @b Description
 *  @n
 *     This function reads flash params
 *
 *   @param[in] ptrQSPIFlashDrv      Pointer to the QSPI flash driver object
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval     =0                   Successful device parameter read
 *  @retval     <0                   Failure error code
 */
static int32_t QSPIFlashReadDeviceParams(QSPIFLASH_Driver       *ptrQSPIFlashDrv)
{
    FLASH_SFDPHeader*   ptrSFDPHdr;
    FLASH_ParamHeader*  ptrParamHdr;
    FLASH_SPIBasicParam* ptrBasicParam;
    uint32_t            paramData[4U] = {0U};
    uint32_t            jedec_addr;
    uint32_t            dummyCycles;
    uint32_t            numAddrBytes;
    int32_t             retVal = 0;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;

    /* Get QSPI driver handle */
    QSPIHandle      = ptrQSPIFlashDrv->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &ptrQSPIFlashDrv->flashParams;

    /* Get SPI flash device id */
    QSPIFlashGetDeviceID(QSPIHandle, FLASH_CMD_JEDEC_ID, &ptrFlashParams->devID);

    /* Read SFDP header */
    QSPIFlashReadSFDP( QSPIHandle, 0U, FLASH_SFDP_HEADER_LEN, (uint8_t *)paramData);
    ptrSFDPHdr = (FLASH_SFDPHeader*)paramData;

    /* Validate SFDP signature */
    if(ptrSFDPHdr->signature != FLASH_SFDP_SIGNATURE)
    {
        retVal = QSPIFLASH_ESFDP;
        goto Exit;
    }

    /* Read JEDEC Params header */
    QSPIFlashReadSFDP(  QSPIHandle, FLASH_JEDEC_HEADER_ADDR,  FLASH_SFDP_HEADER_LEN, (uint8_t *)paramData);
    ptrParamHdr = (FLASH_ParamHeader *)paramData;

    jedec_addr = (uint32_t)(ptrParamHdr->paramTblPtr[2] << 16) + (uint32_t)(ptrParamHdr->paramTblPtr[1] << 8) + ptrParamHdr->paramTblPtr[0];

    /* Read JEDEC parameters 1-2 */
    QSPIFlashReadSFDP(QSPIHandle, jedec_addr, FLASH_JEDEC_PARAM_LEN, (uint8_t *)paramData);
    ptrBasicParam = (FLASH_SPIBasicParam*)paramData;

    ptrFlashParams->readCmdSupport = ptrBasicParam->readSupport;
    ptrFlashParams->sectorEraseCmd = ptrBasicParam->eraseCmd;

    /* Check number of Addr Bytes */
    numAddrBytes = ((ptrFlashParams->readCmdSupport & 0x06U) >> 1U);
    switch (numAddrBytes)
    {
        case FLASH_ADDR_3BYTES:
        case FLASH_ADDR_3_AND_4_BYTES:
          ptrFlashParams->numAddrBytes = 3U;
          break;

        case FLASH_ADDR_4BYTES:
          ptrFlashParams->numAddrBytes = 4U;
          break;

        default:
          ptrFlashParams->numAddrBytes = 3U;
          break;
    }

    /* Single Fast Read */
    ptrFlashParams->mmapSingleReadCfg.numDBits= 0U;
    ptrFlashParams->mmapSingleReadCfg.writeCmd= 0U;
    ptrFlashParams->mmapSingleReadCfg.readType= 0U;
    ptrFlashParams->mmapSingleReadCfg.numDBytes= 1U;
    ptrFlashParams->mmapSingleReadCfg.numABytes= ptrFlashParams->numAddrBytes - 1U;
    ptrFlashParams->mmapSingleReadCfg.readCmd= FLASH_CMD_FAST_SINGLE_READ;

    /* Dual Output Read */
    dummyCycles  = ptrBasicParam->dualOutCfg & FLASH_PARAMS_DUMMY_CYCLE_MASK;
    ptrFlashParams->mmapDualReadCfg.numDBits = (dummyCycles % 8U);
    ptrFlashParams->mmapDualReadCfg.writeCmd = 0U;
    ptrFlashParams->mmapDualReadCfg.readType = 1U;
    ptrFlashParams->mmapDualReadCfg.numDBytes = (dummyCycles / 8U);
    ptrFlashParams->mmapDualReadCfg.numABytes = ptrFlashParams->numAddrBytes - 1U;
    ptrFlashParams->mmapDualReadCfg.readCmd = ptrBasicParam->dualOutCmd;
    ptrFlashParams->dualReadCmd = ptrBasicParam->dualOutCmd;

    /* Quad Output Read */
    dummyCycles  = ptrBasicParam->quadOutCfg & FLASH_PARAMS_DUMMY_CYCLE_MASK;
    ptrFlashParams->mmapQuadReadCfg.numDBits = (dummyCycles % 8U);
    ptrFlashParams->mmapQuadReadCfg.writeCmd = 0U;
    ptrFlashParams->mmapQuadReadCfg.readType = 3U;
    ptrFlashParams->mmapQuadReadCfg.numDBytes = (dummyCycles / 8U);
    ptrFlashParams->mmapQuadReadCfg.numABytes = ptrFlashParams->numAddrBytes - 1U;
    ptrFlashParams->mmapQuadReadCfg.readCmd = ptrBasicParam->quadOutCmd;
    ptrFlashParams->quadReadCmd = ptrBasicParam->quadOutCmd;

Exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *     This function gets flash supported 
 *
 *   @param[in] QSPIFlashHandle      Hanlde of the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval                          Support mode for the flash device
 */
static uint8_t QSPIFlashSupportModes(QSPIFlash_Handle QSPIFlashHandle)
{
    SPIFLASH_Params     *ptrFlashParams;

    /* Get flash parameter pointer */
    ptrFlashParams  = &((QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    return ptrFlashParams->readCmdSupport;
}

/**************************************************************************
 *************************** Exported Functions ********************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *     This function gets flash device information
 *
 *   @param[in] QSPIFlashHandle      Hanlde of the QSPI flash driver
 *   @param[in] devId                Pointer to Deviece id structure
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlash_getDeviceID(QSPIFlash_Handle QSPIFlashHandle, SPIFLASH_devID *devId)
{
    QSPI_Handle QSPIHandle;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get SPI flash device id */
    QSPIFlashGetDeviceID(QSPIHandle, FLASH_CMD_JEDEC_ID, devId);
}

/**
 *  @b Description
 *  @n
 *     This function enables write to flash. It is needed before  erase and page program operation.
 *
 *   @param[in] QSPIFlashHandle      Hanlde of the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlash_writeEnable(QSPIFlash_Handle QSPIFlashHandle)
{
    uint32_t            status;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    QSPIFLASH_Driver    *ptrQSPIFlashDrv;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    /* Get QSPI driver handle */
    QSPIHandle      = ptrQSPIFlashDrv->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &ptrQSPIFlashDrv->flashParams;

    do
    {
        status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
    }while((status & FLASH_STATUS_BUSY) != 0x0U);

    /*send write enable command*/
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->writeEnableCmd, 1U);

    do
    {
       status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
    }while((status & FLASH_STATUS_WRENABLE) != FLASH_STATUS_WRENABLE);
}

/**
 *  @b Description
 *  @n
 *     This function disables write to flash.
 *
 *   @param[in] QSPIFlashHandle      Hanlde of the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlash_writeDisable(QSPIFlash_Handle QSPIFlashHandle)
{
    uint32_t            status;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    QSPIFLASH_Driver    *ptrQSPIFlashDrv;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    /* Get QSPI driver handle */
    QSPIHandle      = ptrQSPIFlashDrv->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &ptrQSPIFlashDrv->flashParams;

    do
    {
        status = QSPIFlashGetStatus(QSPIHandle,ptrFlashParams->readSR1Cmd);
    }while((status & FLASH_STATUS_BUSY) != 0x0U);

    /*send write enable command*/
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->writeDisableCmd, 1U);

    do
    {
       status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
    }while((status & FLASH_STATUS_WRENABLE) == FLASH_STATUS_WRENABLE);
}

/**
 *  @b Description
 *  @n
 *     This function prepares the serial flash for quad interface operation
 *
 *   @param[in] QSPIFlashHandle      Hanlde of the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlash_progQuadRead(QSPIFlash_Handle QSPIFlashHandle)
{
    QSPIFLASH_Driver    *ptrQSPIFlashDrv;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    if(ptrQSPIFlashDrv->devFxnTbl.progQuadReadFxn != NULL)
    {
        ptrQSPIFlashDrv->devFxnTbl.progQuadReadFxn(QSPIFlashHandle);
    }
    else
    {
        DebugP_assert(0U);
    }
}

/**
 *  @b Description
 *  @n
 *     This function clears the block protection bits
 *
 *   @param[in] QSPIFlashHandle      Hanlde of the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlash_clearBlockProtectBits(QSPIFlash_Handle QSPIFlashHandle)
{
    uint32_t            statusReg;
    uint32_t            configReg;
    uint32_t            status;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &((QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    /* Read status register1 */
    statusReg=QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
    if((statusReg & ptrFlashParams->protectBitsMask) != 0x0U)
    {
        statusReg = statusReg & (~ptrFlashParams->protectBitsMask);
    }

    /* Read configuration register */
    configReg= QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR2Cmd);

    QSPIFlash_writeEnable(QSPIFlashHandle);

    /* Send write register command */
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->writeStatusRegCmd, 3U);

    /* Send status register byte */
    QSPIFlashWriteCmd(QSPIHandle, statusReg, 2U);

    /* Send configuratin register byte */
    QSPIFlashWriteCmd(QSPIHandle, configReg, 1U);

    /* Wait for WEN bit to be reset */
    do
    {
        status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
    }while((status & (FLASH_STATUS_BUSY |FLASH_STATUS_WRENABLE )) != 0x0U);
}

/**
 *  @b Description
 *  @n
 *     This function does single read in config mode
 *
 *   @param[in] QSPIFlashHandle  Handle of the QSPI flash driver
 *   @param[in] memAddr          Flash address.
 *   @param[in] dataLen          data length in bytes
 *   @param[in] userDataBuf      Pointer to the data buffer to be written.
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful read
 */
void QSPIFlash_singleRead(QSPIFlash_Handle QSPIFlashHandle, uint32_t memAddr, uint32_t dataLen, uint8_t* userDataBuf)
{
    uint32_t            index;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    uint8_t             totalBytes = 0;

    DebugP_assert((memAddr != 0U));
    DebugP_assert((userDataBuf != (uint8_t*)NULL));
    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &((QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    /* Check read command variation */
    if(ptrFlashParams->readDataCmd == FLASH_CMD_FAST_SINGLE_READ)
    {
        totalBytes = 3U;
    }
    else
    {
        /* Device have to support FLASH_CMD_SINGLE_READ */
        totalBytes = 2U;
    }

    /* Send quad read command */
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->readDataCmd, (dataLen + totalBytes--));

    /* Send 24 bit address */
    QSPIFlashWriteAddr(QSPIHandle, memAddr, (dataLen + totalBytes--));

    if(ptrFlashParams->readDataCmd == FLASH_CMD_FAST_SINGLE_READ)
    {
        /* Send dummy byte - only reqired in fast single read mode */
        QSPIFlashWriteByte(QSPIHandle, 0U, dataLen + totalBytes);
    }
    else
    {
        /* Dummy byte is not required for normal single read command */
    }

    /* Read data from Flash*/
    for(index = 0x0U; index < dataLen; index++)
    {
        userDataBuf[index] = QSPIFlashReadByte(QSPIHandle);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *     This function does dual read in config mode
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] srcAddr          Flash address, half word aligned
 *   @param[in] dataLen          Data length in half words
 *   @param[in] userDataBuf      Pointer to the data to be written, half word aligned
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlash_dualRead(QSPIFlash_Handle QSPIFlashHandle, uint32_t srcAddr, uint32_t dataLen, uint16_t* userDataBuf)
{
    uint32_t            index;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &((QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    /*send quad read command*/
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->dualReadCmd, (dataLen + 3U));

    /*send 24 bit address*/
    QSPIFlashWriteAddr(QSPIHandle, srcAddr, (dataLen + 2U));

    /*----- Send dummy byte -------*/
    QSPIFlashWriteByte(QSPIHandle, 0U, dataLen + 1U);

    /* Read data from Flash*/
    for(index = 0x0U; index < dataLen; index++)
    {
        userDataBuf[ index] = QSPIFlashReadHalfWord(QSPIHandle);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *     This function does quad read in config mode
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] srcAddr          Flash address, word aligned
 *   @param[in] dataLen          Data length in words
 *   @param[in] userDataBuf      Word aligned data buffer pointer to save data from flash
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful read
 */
void QSPIFlash_quadRead(QSPIFlash_Handle QSPIFlashHandle, uint32_t srcAddr, uint32_t dataLen, uint32_t* userDataBuf)
{
    uint32_t            index;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &((QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    /*send quad read command*/
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->quadReadCmd, (dataLen + 3U));

    /*send 24 bit address*/
    QSPIFlashWriteAddr(QSPIHandle, srcAddr, (dataLen + 2U));

    /*----- Send dummy byte -------*/
    QSPIFlashWriteCmd(QSPIHandle, 0U, dataLen + 1U);

    /*---Read data from Flash----*/
    for (index = 0x0U; index < dataLen; index++)
    {
        userDataBuf[index]  = QSPIFlashReadWord(QSPIHandle);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *     This function reads from QSPI flash in memory mapped mode
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] memAddr          Flash memory mapped address
 *   @param[in] dataLen          Length of data in double words
 *   @param[in] userDataBuf      Pointer to the data buffer to save flash data
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful read\n
 *  @retval     <0               Error code when read failed
 *
 */
int32_t QSPIFlash_mmapRead(QSPIFlash_Handle QSPIFlashHandle, const uint32_t *memAddr, uint32_t dataLen, uint32_t* userDataBuf)
{
    int32_t             retVal = 0;

    DebugP_assert((memAddr!= (uint32_t*)NULL));
    DebugP_assert((userDataBuf != (uint32_t *)NULL));
    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

#ifdef QSPIFLASH_PARAMS_CHECK
    if ((((uint32_t)memAddr % 8U) != 0) ||
        (((uint32_t)userDataBuf % 8U) != 0))
    {
        retVal = QSPIFLASH_EINVAL;
        goto Exit;
    }
#endif

    /* Read and store the data to receive data buffer */
    memcpy((void *)userDataBuf, (void *)memAddr, dataLen*8U);

Exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *     This function does a single byte Flash write in config mode
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] memAddr          Flash memory mapped address
 *   @param[in] dataLen          Length of data in bytes
 *   @param[in] srcDataAddr      Data buffer pointer to copy data from
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     None
 */
void QSPIFlash_singleWrite(QSPIFlash_Handle QSPIFlashHandle, uint32_t memAddr, uint32_t dataLen, uint8_t* srcDataAddr)
{
    uint8_t             *arrayBase;
    uint32_t            index;
    uint32_t            status;
    uint32_t            writeLen;
    uint32_t            available;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;

    DebugP_assert((memAddr!= 0U));
    DebugP_assert(srcDataAddr != NULL);
    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);
    
    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &((QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    arrayBase= (uint8_t *)srcDataAddr;

    do
    {
        writeLen = dataLen;

        /* calculate space available in the page */
        available = (FLASH_PAGE_SIZE - (memAddr & FLASH_PAGE_MASK));
        if(writeLen > available)
        {
            writeLen = available;
        }

        /* make write enable flash before write or erase */
        QSPIFlash_writeEnable(QSPIFlashHandle);

        QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->pageProgramCmd, (writeLen + 2U));

        QSPIFlashWriteAddr(QSPIHandle, memAddr, writeLen + 1U);

        for(index = 0x0U; index < writeLen; index++)
        {
            QSPIFlashWriteByte(QSPIHandle, arrayBase[index], 1U);
        }

        /* check BUSY flag before issuing next command*/
        do
        {
            status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
        }while((status & FLASH_STATUS_BUSY) != 0x0U);

        memAddr += writeLen;
        arrayBase += writeLen;
        dataLen -= writeLen;
    }while(dataLen > 0U);
}

/**
 *  @b Description
 *  @n
 *     This function writes to Flash in memory mapped mode
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] memAddr          memory mapped flash address
 *   @param[in] dataLen          Data length in double words
 *   @param[in] srcDataAddr      Data buffer Pointer to copy data from
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful write\n
 *  @retval     <0               Error code when write failed
 */
int32_t QSPIFlash_mmapWrite(QSPIFlash_Handle QSPIFlashHandle, const uint32_t* memAddr, uint32_t dataLen, uint32_t* srcDataAddr)
{
    uint32_t            status;
    uint32_t            index;
    uint64_t *          ptrSrc = (uint64_t*) srcDataAddr;
    uint64_t*           ptrDst =  (uint64_t*)memAddr;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    int32_t             retVal = 0;

    DebugP_assert((memAddr!= (uint32_t*)NULL));
    DebugP_assert((srcDataAddr!= (uint32_t*)NULL));
    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

#ifdef QSPIFLASH_PARAMS_CHECK
    if ((((uint32_t)memAddr % 8U) != 0) ||
        (((uint32_t)srcDataAddr % 8U) != 0))
    {
        retVal = QSPIFLASH_EINVAL;
        goto Exit;
    }
#endif

    /* Get flash parameter pointer */
    ptrFlashParams  = &((QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    /* Copy data */
    for(index = 0; index < dataLen; index++)
    {
        ptrDst[index] = ptrSrc[index];

        do
        {
            /* Get QSPI Flash Status */
             status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
        }while((status & FLASH_STATUS_BUSY) != 0x0U);

        /* It is a must to have step for Macronix flash device
           Checking FLASH_STATUS_WRENABLE bit is not reliable enough
         */
        {
            QSPIFlash_writeEnable(QSPIFlashHandle);
        }
    }
Exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *     This function sets up configuration for memory-map read
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] mode             Mode of the memory mapped read
 *   @param[in] errCode          Error code populated in the API
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful read\n
 *  @retval     <0               Error code when read failed
 */
int32_t QSPIFlash_configMmapRead(QSPIFlash_Handle QSPIFlashHandle, FLASH_READ_MODE mode, int32_t *errCode)
{
    QSPI_memConfig      *mmapCfg;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    uint8_t             QSPImodes;
    int32_t             retVal = 0;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &( (QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    /* Get supported mode */
    QSPImodes = QSPIFlashSupportModes(QSPIFlashHandle);

    if(mode == FLASH_AUTO_MODE)
    {
        if(IS_QUAD_SUPPORTED(QSPImodes) != 0U)
        {
            /* Use QuadRead mode */
            mmapCfg = &ptrFlashParams->mmapQuadReadCfg;

            /* enable quad read mode */
            QSPIFlash_progQuadRead(QSPIFlashHandle);
        }
        else if (IS_DUAL_SUPPORTED(QSPImodes) != 0U)
        {
            mmapCfg = &ptrFlashParams->mmapDualReadCfg;
        }
        else
        {
            mmapCfg = &ptrFlashParams->mmapSingleReadCfg;
        }
    }
    else 
    {
    switch(mode)
    {
        case FLASH_QUAD_MODE:
                if(IS_QUAD_SUPPORTED(QSPImodes) != 0U)
                {
                    mmapCfg = &ptrFlashParams->mmapQuadReadCfg;
                }
                else
                {
                    *errCode = QSPIFLASH_EINVAL;
                    retVal = MINUS_ONE;
                    goto exit;
                }
            break;
        case FLASH_DUAL_MODE:
                if(IS_DUAL_SUPPORTED(QSPImodes) != 0U)
                {
                    mmapCfg = &ptrFlashParams->mmapDualReadCfg;
                }
                else
                {
                    *errCode = QSPIFLASH_EINVAL;
                    retVal = MINUS_ONE;
                    goto exit;
                }
            break;
            case FLASH_SINGLE_MODE:
                mmapCfg = &ptrFlashParams->mmapSingleReadCfg;
                break;
            default:
                /* Mode not defined */
                DebugP_assert(0);
                break;
        }
    }

    /* Setup Memory access for Read */
    QSPI_memAccessCfg(QSPIHandle,  mmapCfg);
exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *     This function sets up configuration for memory-map write
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */
void QSPIFlash_configMmapWrite(QSPIFlash_Handle QSPIFlashHandle)
{
    QSPI_memConfig      mmapCfg;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &( (QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    /* make write enable flash before write or erase */
    QSPIFlash_writeEnable(QSPIFlashHandle);

    mmapCfg.numABytes    = ptrFlashParams->numAddrBytes - 1U;
    mmapCfg.numDBits     = 0x0U;
    mmapCfg.numDBytes    = 0x0U;
    mmapCfg.readCmd      = 0x0U;
    mmapCfg.readType     = 0x0U;
    mmapCfg.writeCmd     = ptrFlashParams->pageProgramCmd;

    /* Setup Memory access for Write */
    QSPI_memAccessCfg(QSPIHandle,  &mmapCfg);
}

/**
 *  @b Description
 *  @n
 *     This function erases a flash sector.
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] addr             Address of the sector to be erased
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful sector erase\n
 *  @retval     <0               Error code when sector erase failed
 */
int32_t QSPIFlash_sectorErase(QSPIFlash_Handle QSPIFlashHandle, uint32_t addr)
{
    uint8_t             status;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    int32_t             retVal = 0;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &( (QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    if( ptrFlashParams->sectorEraseCmd == (uint8_t)0U)
    {
        retVal = QSPIFLASH_ENOTSUPPORTED;
    }
    else
    {
        /* Make write enable flash before erase */
        QSPIFlash_writeEnable(QSPIFlashHandle);

        /* Send Sector Erase command */
        QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->sectorEraseCmd, 2U);

        /* Send address */
        QSPIFlashWriteAddr(QSPIHandle, addr, 1U);

        do
        {
            status = (uint8_t)QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
        }while((status & ((uint8_t)(FLASH_STATUS_BUSY |FLASH_STATUS_WRENABLE ))) != ((uint8_t)0x0U)) ;
    }
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *     This function erases a 64K block.
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *   @param[in] addr             Address of the 64KB block to be erased
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful block erase\n
 *  @retval     <0               Error code when block erase failed
 */
int32_t QSPIFlash_blockErase(QSPIFlash_Handle QSPIFlashHandle, uint32_t addr)
{
    uint8_t             status;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    int32_t             retVal = 0;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &( (QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    if( ptrFlashParams->blockEraseCmd == (uint8_t)0U)
    {
        retVal = QSPIFLASH_ENOTSUPPORTED;
    }
    else
    {
        /* Make write enable flash before erase */
        QSPIFlash_writeEnable(QSPIFlashHandle);

        /* Send Sector Erase command */
        QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->blockEraseCmd, 2U);

        /* Send address */
        QSPIFlashWriteAddr(QSPIHandle, addr, 1U);

        do
        {
            status = (uint8_t)QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
        }while((status & ((uint8_t)(FLASH_STATUS_BUSY |FLASH_STATUS_WRENABLE ))) != ((uint8_t)0x0U)) ;
    }
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *     This function erases the complete flash.
 *
 *   @param[in] QSPIFlashHandle  Hanlde of the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0               Successful chip erase\n
 *  @retval     <0               Error code when chip erase failed
 */
int32_t QSPIFlash_chipErase(QSPIFlash_Handle QSPIFlashHandle)
{
    uint32_t            status;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    int32_t             retVal = 0;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    QSPIHandle =( (QSPIFLASH_Driver *)QSPIFlashHandle)->ptrQSPIHandle;

    /* Get flash parameter pointer */
    ptrFlashParams  = &( (QSPIFLASH_Driver *)QSPIFlashHandle)->flashParams;

    if( ptrFlashParams->chipEraseCmd == (uint8_t)0U)
    {
        retVal = QSPIFLASH_ENOTSUPPORTED;
    }
    else
    {
        /* Make write enable flash before erase */
        QSPIFlash_writeEnable(QSPIFlashHandle);

        /* Send Sector Erase command */
        QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->chipEraseCmd, 0U);

        do
        {
            status = (uint8_t)QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
        }while((status & ((uint8_t)(FLASH_STATUS_BUSY |FLASH_STATUS_WRENABLE ))) != ((uint8_t)0x0U)) ;
    }
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      This function handles the DMA interrupt which is generated from QSPI
 *
 *   @param[in] dmaChannel   DMA channel
 *   @param[in] dmaIntType   DMA interrupt type FTC/BTC
 *   @param[in] arg          DMA interrupt argument
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval    None.
 */
static void QSPIFlashSysDmaIntHandler(uint32_t dmaChannel, DMA_IntType dmaIntType, void *arg)
{
    QSPIFLASH_Driver *ptrQSPIFlashDrv;

    DebugP_assert(arg != NULL);

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)arg;

    switch(dmaIntType)
    {
        case DMA_IntType_FTC:
        case DMA_IntType_BTC:
            ptrQSPIFlashDrv->dmaIntCnt++;

            /* QSPI-RX DMA complete interrupt */
            SemaphoreP_post(ptrQSPIFlashDrv->transferSema);

            break;
        case DMA_IntType_HBC:
        case DMA_IntType_LFS:
        default:
            DebugP_assert(0U);
            break;
    }
    return ;
}

/**
 *  @b Description
 *  @n
 *      This function inits QSPI Flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval None
 */

void QSPIFlash_init(void)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      This function opens QSPI Flash driver, can get flash device infomation
 *
 *   @param[in] QSPIHandle   Handle to the QSPI driver
 *   @param[in] errCode      Error code to be populated by the driver in case of error
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval    QSPIFlash handle       Success
 *  @retval    NULL pointer.          Failure
 */
QSPIFlash_Handle QSPIFlash_open(QSPI_Handle QSPIHandle, int32_t *errCode)
{
    int32_t             retVal =0;
    QSPIFLASH_Driver*   ptrQSPIFlashDrv;
    SemaphoreP_Params   semParams;
    QSPIFlash_Handle    retHandle = (QSPIFlash_Handle)NULL;
    SPIFLASH_Params     *ptrFlashParams;
    SPIFLASH_devID      devId;

#ifdef QSPIFLASH_PARAMS_CHECK
    if (QSPIHandle == (QSPI_Handle)NULL)
    {
        retVal = QSPIFLASH_EINVAL;
        goto Exit;
    }
#endif

    /* Allocate memory for Driver object */
    ptrQSPIFlashDrv = MemoryP_ctrlAlloc ((uint32_t)sizeof(QSPIFLASH_Driver), 0U);
    if (ptrQSPIFlashDrv == NULL)
    {
        /* Error: Out of memory */
        DebugP_log1 ("Debug: QSPIFlash Driver Out of memory (requested size: %d)\n", (uint32_t)sizeof(QSPIFLASH_Driver));
        retVal = QSPIFLASH_ENOMEM;
        goto Exit;
    }

    /* Initialize driver object */
    memset((void *)ptrQSPIFlashDrv, 0, sizeof(QSPIFLASH_Driver));

    /* Save QSPI driver Handle */
    ptrQSPIFlashDrv->ptrQSPIHandle = QSPIHandle;

    /* Save EXT Flash Base Address */
    ptrQSPIFlashDrv->extFlashBaseAddr = gQSPIFlashHwCfg.extFlashBaseAddr;

    /* Get flash parameter pointer */
    ptrFlashParams  = &ptrQSPIFlashDrv->flashParams;

    /* Configure default command set */
    QSPIFlashDefaultCmd_init(ptrFlashParams);

    retVal = QSPIFlashReadDeviceParams(ptrQSPIFlashDrv);

    /* Get SPI flash device id */
    QSPIFlashGetDeviceID(QSPIHandle, FLASH_CMD_JEDEC_ID, &devId);

    if(devId.Manufacture == SPANSION_DEV)
    {
        ptrQSPIFlashDrv->devFxnTbl = QSPIFlash_deviceFxnTbl_spansion;
    }
    else if (devId.Manufacture == MACRONIX_DEV)
    {
        ptrQSPIFlashDrv->devFxnTbl = QSPIFlash_deviceFxnTbl_macronix;
    }
    else
    {
        retVal = QSPIFLASH_ENOTSUPPORTED;
        goto Exit;
    }

    /* Call SPI flash device init function to set special command set and masks */
    ptrQSPIFlashDrv->devFxnTbl.initFxn(ptrFlashParams);

    /* Update device specific single read command */
    ptrFlashParams->mmapSingleReadCfg.readCmd= ptrFlashParams->readDataCmd;
    if(ptrFlashParams->mmapSingleReadCfg.readCmd == FLASH_CMD_FAST_SINGLE_READ)
    {
            ptrFlashParams->mmapSingleReadCfg.numDBytes= 1U;
    }
    else
    {
            ptrFlashParams->mmapSingleReadCfg.numDBytes= 0U;
    }

    /* Flash reset */
    if (ptrFlashParams->contModeResetCmd != 0)
    {
        QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->contModeResetCmd, 1U);
        QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->contModeResetCmd, 1U);
    }
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->resetEnableCmd, 1U);
    QSPIFlashWriteCmd(QSPIHandle, ptrFlashParams->resetCmd, 1U);

#ifdef QSPIFLASH_DEBUG
    {
        uint8_t             status = 0;
        
        /* Read status */
        status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
        printf("status1 = %x\n", status);
        status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR2Cmd);
        printf("status2 = %x\n", status);
        status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR3Cmd);
        printf("status3 = %x\n", status);
    }

    /* Debug messages */
    printf("--------- SPI flash detected-----------\n");
    printf("Device manufacture:         %x\n",ptrFlashParams->devID.Manufacture);
    printf("Device type:                %x\n", ptrFlashParams->devID.device);
    printf("Device capacity:            %x\n", ptrFlashParams->devID.capacity);
    printf("Number of Address bytes:    %d\n", ptrFlashParams->numAddrBytes);
    printf("Erase command:              %x\n", ptrFlashParams->sectorEraseCmd);
    printf("Dual read command:          %x\n", ptrFlashParams->dualReadCmd);
    printf("Quad read command:          %x\n", ptrFlashParams->quadReadCmd);
    printf("Read command support:       %x\n", ptrFlashParams->readCmdSupport);
    printf("------------------------------------\n");
#endif

    /* Create semaphore used for DMA transfer */
    SemaphoreP_Params_init(&semParams);
    semParams.mode            = SemaphoreP_Mode_BINARY;
    ptrQSPIFlashDrv->transferSema = SemaphoreP_create(0, &semParams);
    DebugP_assert(ptrQSPIFlashDrv->transferSema != (SemaphoreP_Handle)NULL);

    retHandle = (QSPI_Handle)ptrQSPIFlashDrv;

Exit:
    if(errCode != NULL)
    {
        *errCode = retVal;
    }

    return (retHandle);
}

/**
 *  @b Description
 *  @n
 *      This function closes QSPI Flash driver and resources used by the driver
 *
 *   @param[in] QSPIFlashHandle  Handle to the QSPI flash driver
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval    =0            Successful close
 *  @retval    <0            Error code when close with failures
 */
int32_t QSPIFlash_close(QSPIFlash_Handle QSPIFlashHandle)
{
    int32_t                 retVal = 0;
    QSPIFLASH_Driver*       ptrQSPIFlashDrv;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    /* Delete Semaphore */
    if(ptrQSPIFlashDrv->transferSema != (SemaphoreP_Handle)NULL)
    {
        retVal = SemaphoreP_delete(ptrQSPIFlashDrv->transferSema);
    }

    /* Free Driver memory */
    MemoryP_ctrlFree(QSPIFlashHandle, (uint32_t)sizeof(QSPIFLASH_Driver));

    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      This function returns Base address of the External Flash
 *
 *   @param[in] QSPIFlashHandle  Handle to the QSPI flash driver object
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      External flash Address
 */
uint32_t QSPIFlash_getExtFlashAddr(QSPIFlash_Handle QSPIFlashHandle)
{
    QSPIFLASH_Driver*       ptrQSPIFlashDrv;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    return (ptrQSPIFlashDrv->extFlashBaseAddr);
}

/**
 *  @b Description
 *  @n
 *     This function sets up DMA write, and transfer data from user buffer to flash
 *
 *   @param[in] QSPIFlashHandle     Handle to the QSPI flash driver object
 *   @param[in] dmaHandle           Handle to DMA driver
 *   @param[in] dmaChan             DMA channel number used for the DMA read
 *   @param[in] srcAddr             User buffer address of the write operation, address should be double word aligned
 *   @param[in] dstAddr             Flash address of the write operation , address should be double word aligned
 *   @param[in] dataLen             Data length of the DMA read in double words
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval     =0                   Data is written successfully\n
 *  @retval     <0                   Error code for DMA write operation
 */
int32_t QSPIFlash_sysDmaWrite(QSPIFlash_Handle QSPIFlashHandle, DMA_Handle dmaHandle, uint32_t dmaChan, uint32_t srcAddr, uint32_t dstAddr, uint32_t dataLen)
{
    uint32_t            status;
    int32_t             retVal = 0;
    uint32_t            index;
    DMA_CtrlPktParams   ctrlPacket ={0U};
    QSPI_Handle             QSPIHandle;
    QSPIFLASH_Driver*       ptrQSPIFlashDrv;

    DebugP_assert(dmaHandle != (DMA_Handle)NULL);
    DebugP_assert(srcAddr != 0U);
    DebugP_assert(dstAddr != 0U);
    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);
    
#ifdef QSPIFLASH_PARAMS_CHECK
    if (((dstAddr % 8U) != 0) ||
        ((srcAddr % 8U) != 0))
    {
        retVal = QSPIFLASH_EINVAL;
        goto Exit;
    }
#endif

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    QSPIHandle =ptrQSPIFlashDrv->ptrQSPIHandle;

    ctrlPacket.srcAddr         = srcAddr;
    ctrlPacket.destAddr        = dstAddr;
    ctrlPacket.nextChannel     = DMA_CHANNEL_NONE;
    ctrlPacket.autoInitiation  = 0U;

    /* Setup DMA frame params */
    ctrlPacket.destFrameIndexOffset = 0U;
    ctrlPacket.srcFrameIndexOffset  = 0U;
    ctrlPacket.destElemIndexOffset  = 0U;
    ctrlPacket.srcElemIndexOffset   = 0U;
    ctrlPacket.frameXferCnt         = 1U;
    ctrlPacket.srcAddrMode          = DMA_AddrMode_PostIncrement;
    ctrlPacket.destAddrMode         = DMA_AddrMode_PostIncrement;
    ctrlPacket.srcElemSize          = DMA_ElemSize_64bit;
    ctrlPacket.destElemSize         = DMA_ElemSize_64bit;

    ctrlPacket.elemXferCnt      = 1U;
    ctrlPacket.xferType         = DMA_XferType_Block;
    ctrlPacket.frameXferCnt      = 1U;

    for(index=0; index < dataLen; index ++)
    {
        /* make write enable flash before write or erase */
        QSPIFlash_writeEnable(QSPIFlashHandle);

        ctrlPacket.srcAddr         = srcAddr;
        ctrlPacket.destAddr        = dstAddr;

        /* Set DMA control packet */
        retVal = DMA_setChannelParams(dmaHandle, dmaChan, &ctrlPacket);
        if(retVal < 0)
        {
            goto Exit;
        }

        retVal = DMA_enableInterrupt(dmaHandle, dmaChan, DMA_IntType_BTC, QSPIFlashSysDmaIntHandler, QSPIFlashHandle);
        if(retVal < 0)
        {
            goto Exit;
        }

        /* Enable DMA transfer channel */
        retVal = DMA_enableChannel(dmaHandle, dmaChan, DMA_ChTriggerType_SW);
        if (retVal < 0)
        {
            goto Exit;
        }
        SemaphoreP_pend(ptrQSPIFlashDrv->transferSema, SemaphoreP_WAIT_FOREVER);

        do
        {
           status = QSPIFlashGetStatus(QSPIHandle, FLASH_CMD_RDSR1);
        }while((status & 0x1U) != 0x0U);

        /* Increment Source and destination address by 8 byte */
        srcAddr = srcAddr + 8U;
        dstAddr = dstAddr + 8U;
        retVal = DMA_disableInterrupt(dmaHandle, dmaChan, DMA_IntType_BTC);
        if (retVal < 0)
        {
            goto Exit;
        }
        
        retVal = DMA_disableChannel (dmaHandle, dmaChan, DMA_ChTriggerType_SW);

        if (retVal < 0)
        {
            goto Exit;
        }
    }
Exit:

    return (retVal);
}

/**
 *  @b Description
 *  @n
 *     This function sets up DMA read
 *
 *
 *   @param[in] QSPIFlashHandle     Handle to the QSPI flash driver object
 *   @param[in] dmaHandle           Handle to DMA driver
 *   @param[in] dmaChan             DMA channel number used for the DMA read
 *   @param[in] srcAddr             Flash source address of the read operation, address should be double word aligned
 *   @param[in] dstAddr             Destination address of the read operation , address should be double word aligned
 *   @param[in] dataLen             Data length of the DMA read in double words
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval  =0                   Data is read successfully\n
 *  @retval  <0                   Error code for DMA read operation
 */
int32_t QSPIFlash_sysDmaRead(QSPIFlash_Handle QSPIFlashHandle, DMA_Handle dmaHandle, uint32_t dmaChan, uint32_t srcAddr, uint32_t dstAddr, uint32_t dataLen)
{
    uint32_t                status;
    int32_t                 retVal;
    DMA_CtrlPktParams       ctrlPacket ={0U};
    QSPI_Handle             QSPIHandle;
    QSPIFLASH_Driver*       ptrQSPIFlashDrv;

    DebugP_assert(dmaHandle != (DMA_Handle)NULL);
    DebugP_assert(srcAddr != 0U);
    DebugP_assert(dstAddr != 0U);
    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);

#ifdef QSPIFLASH_PARAMS_CHECK
    if (((dstAddr % 8U) != 0) ||
        ((srcAddr % 8U) != 0))
    {
        retVal = QSPIFLASH_EINVAL;
        goto Exit;
    }
#endif

    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    QSPIHandle =ptrQSPIFlashDrv->ptrQSPIHandle;

    ctrlPacket.srcAddr         = srcAddr;
    ctrlPacket.destAddr        = dstAddr;
    ctrlPacket.nextChannel     = DMA_CHANNEL_NONE;
    ctrlPacket.autoInitiation  = 0U;

    /* Setup DMA frame params */
    ctrlPacket.destFrameIndexOffset = 0U;
    ctrlPacket.srcFrameIndexOffset  = 0U;
    ctrlPacket.destElemIndexOffset  = 0U;
    ctrlPacket.srcElemIndexOffset   = 0U;
    ctrlPacket.frameXferCnt         = 1U;
    ctrlPacket.srcAddrMode          = DMA_AddrMode_PostIncrement;
    ctrlPacket.destAddrMode         = DMA_AddrMode_PostIncrement;
    ctrlPacket.srcElemSize          = DMA_ElemSize_64bit;
    ctrlPacket.destElemSize         = DMA_ElemSize_64bit;

    ctrlPacket.elemXferCnt      = dataLen;
    ctrlPacket.xferType         = DMA_XferType_Block;
    ctrlPacket.frameXferCnt      = 1U;

    /* Set DMA control packet */
    retVal = DMA_setChannelParams(dmaHandle, dmaChan, &ctrlPacket);
    if(retVal < 0)
    {
        goto Exit;
    }

    retVal = DMA_enableInterrupt(dmaHandle, dmaChan, DMA_IntType_BTC, QSPIFlashSysDmaIntHandler, QSPIFlashHandle);
    if(retVal < 0)
    {
        goto Exit;
    }

    /* Enable DMA transfer channel */
    retVal = DMA_enableChannel(dmaHandle, dmaChan, DMA_ChTriggerType_SW);
    if (retVal < 0)
    {
        goto Exit;
    }

    SemaphoreP_pend(ptrQSPIFlashDrv->transferSema, SemaphoreP_WAIT_FOREVER);

    do
    {
       status = QSPIFlashGetStatus(QSPIHandle, FLASH_CMD_RDSR1);
    }while((status & 0x1U) != 0x0U);

    DMA_disableInterrupt(dmaHandle, dmaChan, DMA_IntType_BTC);
    DMA_disableChannel (dmaHandle, dmaChan, DMA_ChTriggerType_SW);

Exit:
    return (retVal);
}
