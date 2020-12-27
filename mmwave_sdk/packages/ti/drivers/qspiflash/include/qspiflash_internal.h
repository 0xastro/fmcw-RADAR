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
 *  @file       qspiflash_internal.h
 *
 *  @brief      QSPI Flash Internal include file
 *
 */

#ifndef QSPI_FLASH_INTERNAL_H
#define QSPI_FLASH_INTERNAL_H

#include <ti/drivers/dma/dma.h>
#include <ti/drivers/qspi/qspi.h>
#include <ti/drivers/qspiflash/qspiflash.h>

#ifdef __cplusplus
    extern "C" {
#endif


/****  SFLASH COMMANDS  ****/
#define FLASH_CMD_DEV_ID                0x90U
#define FLASH_CMD_JEDEC_ID              0x9FU

#define FLASH_CMD_WR_ENABLE             0x06U
#define FLASH_CMD_VOL_SR_WR_ENABLE      0x50U
#define FLASH_CMD_WR_DISABLE            0x04U
#define FLASH_CMD_RDSR1                 0x05U

#define FLASH_CMD_RDCR_SPANSION         0x35U
#define FLASH_CMD_RDCR_MACRONIX         0x15U
#define FLASH_CMD_RDCR_MICRON           0xB5U

#define FLASH_CMD_RDSR3_SPANSION        0x33U

#define FLASH_CMD_WR_SR                 0x01U
#define FLASH_CMD_WRCR_MICRON           0xB1U
#define FLASH_CMD_PAGE_PROGRAM          0x02U

#define FLASH_CMD_SECTOR_ERASE          0x20U
#define FLASH_CMD_BLOCK_ERASE           0xD8U
#define FLASH_CMD_CHIP_ERASE            0x60U

#define FLASH_CMD_ERS_PROG_SUS          0x75U
#define FLASH_CMD_ERS_PROG_RESUME       0x7AU

#define FLASH_CMD_POWERDOWN             0xB9U
#define FLASH_CMD_SINGLE_READ           0x03U
#define FLASH_CMD_FAST_SINGLE_READ      0x0BU
#define FLASH_CMD_REL_POWERDOWN         0xABU

#define FLASH_CMD_ERS_SEC_REG           0x44U
#define FLASH_CMD_PROG_SEC_REG          0x42U
#define FLASH_CMD_READ_SEC_REG          0x48U
#define FLASH_CMD_ENABLE_QPI            0x38U
/*#define FLASH_CMD_RESET                 0xF0U */
#define FLASH_CMD_DUAL_READ             0x3BU
#define FLASH_CMD_QUAD_READ             0x6BU
#define FLASH_CMD_SFDP                  0x5AU

#define FLASH_CMD_RESET_ENABLE          0x66U
#define FLASH_CMD_RESET                 0x99U
#define FLASH_CMD_CONT_MODE_RESET       0xFFU

#define FLASH_STATUS_BUSY               0x1U
#define FLASH_STATUS_WRENABLE           0x2U

/**** SFLASH Properties ****/

#define FLASH_PAGE_MASK                 (0x000000FFU)
#define FLASH_PAGE_SIZE                 (256U)  /* bytes */

#define FLASH_SECTOR_SIZE               (4096U)  /* 4k bytes */
#define FLASH_BLOCK_SIZE                (65536U)  /* 64k bytes */

/* Definition for JEDEC */
#define FLASH_SFDP_HEADER_LEN           (8U)    /* Bytes */
#define FLASH_SFDP_SIGNATURE            (0x50444653U)
#define FLASH_JEDEC_HEADER_ADDR         (0x08U)
#define FLASH_JEDEC_PARAM_LEN           (16U)   /* Bytes */

/* Definition for number of Addr bytes */
#define FLASH_ADDR_3BYTES               0U
#define FLASH_ADDR_3_AND_4_BYTES        1U
#define FLASH_ADDR_4BYTES               2U

#define FLASH_PARAMS_DUMMY_CYCLE_MASK    0x1FU

/** @addtogroup QSPIFLASH_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief QSPIFlash device parameter definition
 *
 *  @note QSPI flash device parameters
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct SPIFLASH_Params_t
{
    /* SPI flash device information */
    SPIFLASH_devID      devID;

    /* Number of Address bytes */
    uint8_t             numAddrBytes;

    /* Quad read command */
    uint8_t             quadReadCmd;

    /* Dual read command */
    uint8_t             dualReadCmd;

    /* Flash supported read command */
    uint8_t             readCmdSupport;

    /* Read status register 1 command */
    uint8_t             readSR1Cmd;

    /* Read status register 2 command */
    uint8_t             readSR2Cmd;

    /* Read status register 3 command */
    uint8_t             readSR3Cmd;

    /* Wrtie enable command */
    uint8_t             writeEnableCmd;

    /* Wrtie disable command */
    uint8_t             writeDisableCmd;

    /* Wrtie status register command */
    uint8_t             writeStatusRegCmd;

    /* Page programming command */
    uint8_t             pageProgramCmd;

    /* Sector erase command */
    uint8_t             sectorEraseCmd;

    /* Block erase command */
    uint8_t             blockEraseCmd;

    /* Chip erase command */
    uint8_t             chipEraseCmd;

    /* Enable Reset command */
    uint8_t             resetEnableCmd;

    /* Reset command */
    uint8_t             resetCmd;

    /* Reset continuous mode command */
    uint8_t             contModeResetCmd;
    
    /* Single read command */
    uint8_t             readDataCmd;

    /* Protection bits mask in Status register */
    uint8_t             protectBitsMask;

    /* Memory mapped configurations for single read command */
    QSPI_memConfig      mmapSingleReadCfg;

    /* Memory mapped configurations for dual read command */
    QSPI_memConfig      mmapDualReadCfg;

    /* Memory mapped configurations for quad read command */
    QSPI_memConfig      mmapQuadReadCfg;
} SPIFLASH_Params;

/*!
 *  @brief QSPIFlash JEDEC standard SFDP header
 *
 *  @note QSPI flash SFDP header definiton
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct FLASH_SFDPHeader_t
{
    /* SFDP Signature = 0x50444653 -> "PDFS" */
    uint32_t    signature;

    /* SFDP Minor Revision */
    uint8_t     minorRev;

    /* SFDP Major Revision */
    uint8_t     majorRev;

    /* Number of parameter headers (zero based) */
    uint8_t     numParamsHdr;

    uint8_t     reserved0;
}FLASH_SFDPHeader;

/*!
 *  @brief QSPIFlash JEDEC standard SFDP parmaeter header
 *
 *  @note QSPI flash SFDP parameter header definiton
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct FLASH_ParamHeader_t
{
    /* Param id LSB */
    uint8_t     paramIdLSB;

    /* Parameter Minor Revision */
    uint8_t     minorRev;

    /* Parameter Major Revision */
    uint8_t     majorRev;

    /* Number of DWORDS(4-byte) of parameter table */
    uint8_t     paramTblLen;

    /* Parameter Table address byte offset - 3bytes */
    uint8_t     paramTblPtr[3];

    /* Param id MSB */
    uint8_t     paramIMLSB;
}FLASH_ParamHeader;

/*!
 *  @brief QSPIFlash JEDEC standard basic SPI parameters
 *
 *  @note QSPI flash basic SPI parameters defined by JEDEC standard
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct FLASH_SPIBasicParam_t
{
    /* Special feature supported */
    uint8_t     feature;

    /* Uniform 4KB erase command */
    uint8_t     eraseCmd;

    /* Supported read options */
    uint8_t     readSupport;

    /* Byte not used */
    uint8_t     unUsed;

    /* Flash density 00-16MB, 01 - 32MB, 03-64MB */
    uint32_t    density;

    /* Quad I/O(1-4-4) support configuration */
    uint8_t     quadInOutCfg;

    /* Quad I/O(1-4-4) command */
    uint8_t     quadInOutCmd;

    /* Quad Out(1-1-4) support configuration */
    uint8_t     quadOutCfg;

    /* Quad Out(1-1-4) command */
    uint8_t     quadOutCmd;

    /* Dual out(1-1-2) support configuration */
    uint8_t     dualOutCfg;

    /* Dual out(1-1-2) command */
    uint8_t     dualOutCmd;

    /* Dual I/O(1-2-2) support configuration */
    uint8_t     dualInOutCfg;

    /* Dual I/O(1-2-2) command */
    uint8_t     dualInOutCmd;

    /* Other params will be extended in future */
}FLASH_SPIBasicParam;


/**
 *  @b Description
 *  @n
 *      Flash command set initialization function
 *
 *  @param[in]  params      Pointer to the SPI flash parameter set
 *
 *  @retval
 *      Not applicable
 */
typedef void (*QSPIFlash_initFxn)(SPIFLASH_Params* params);

/**
 *  @b Description
 *  @n
 *      Function to program SPI flash in Quad read mode
 *
 *  @param[in]  ptrQSPIFlashDrv      Pointer to QSPI flash driver object
 *
 *  @retval
 *      Not applicable
 */
typedef void (*QSPIFlash_progQuadReadFxn)(QSPIFlash_Handle QSPIFlashHandle);

/**
 * @brief
 *  mmWave Cooperative mode configuration
 *
 * @details
 *  The structure is used to describe configuration supported if the mmWave
 *  is executing in cooperative mode. In this mode each mmWave module domain
 *  is capable of executing the mmWave API and is also responsible for notifying
 *  its peer execution domain on the reception of configuration/events.
 */
typedef struct QSPIFlash_deviceFxn_t
{
    /**
     * @brief   This Function initializes SPI flash command set.
     */
    QSPIFlash_initFxn               initFxn;

    /**
     * @brief   This function programs SPI flash in quad read mdoe.
     */
    QSPIFlash_progQuadReadFxn       progQuadReadFxn;
}QSPIFlash_deviceFxn;

/*!
 *  @brief QSPIFlash driver object
 *
 *  @note QSPI flash driver object used internally to save runtime configurations
 *  of QSPI flash driver.
 *
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct QSPIFLASH_Driver_t
{
    /* QSPI Driver Handle */
    QSPI_Handle         ptrQSPIHandle;

    /* EXT Flash base Address */
    uint32_t            extFlashBaseAddr;

    /* Handle to the Transfer complete semaphore */
    SemaphoreP_Handle   transferSema;

    /* SPI flash params */
    SPIFLASH_Params     flashParams;

    /* SPI Flash device function table */
    QSPIFlash_deviceFxn  devFxnTbl;

    /* DMA copy stats */
    uint32_t            dmaIntCnt;
} QSPIFLASH_Driver;

/** @}*/

extern uint32_t QSPIFlashGetStatus(QSPI_Handle QSPIHandle, uint32_t cmd);
extern void QSPIFlashWriteCmd(QSPI_Handle QSPIHandle, uint32_t cmd, uint32_t frameLen);

#endif /* QSPI_FLASH_INTERNAL_H */
