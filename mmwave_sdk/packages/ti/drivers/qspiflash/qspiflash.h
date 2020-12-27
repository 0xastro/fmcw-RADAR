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
 *  @file       qspiflash.h
 *
 *  @brief      QSPI Flash
 *
 */

/** @mainpage QSPI Flash Driver
 *
 *  The QSPI Flash driver provides functionaity to access external SPI flash
 *
 *  The QSPI Flash header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/qspiflash/qspiflash.h>
 *  @endcode
 *
 *  ## Initializing the driver #
 *  The QSPI Flash Driver needs to be initialized once across the System. This is  done using #QSPIFlash_init.
 *  None of the QSPI Flash API can be used without invoking this API.
 *
 *  ## Opening the driver #
 *  Once the QSPI flash Driver has been initialized, the QSPI Driver can be opened using the #QSPIFlash_open.
 *  Successful #QSPIFlash_open call will return a #QSPIFlash_Handle, which can be used to read/write from/to serial
 *  flash. QSPI driver handle needs to be provided to the QSPI flash driver in order to access serial flash through 
 *  QSPI hardware interface.
 *
 *  ## Get flash device id #
 *  Once the QSPI flash Driver has been opened,  #QSPIFlash_getDeviceID can be used to get the serial flash 
 *  manufacture id and flash capacity. 
 *
 *  ## Read/Write in QSPI config mode #
 *  Once the QSPI flash Driver has been opened,  #QSPIFlash_singleRead and #QSPIFlash_singleWrite can be 
 *  used to read/write  one byte from/to serial flash
 *
 *  ## Read/Write in memory mapped mode #
 *  QSPI flash Driver can also be used memory mapped mode, #QSPIFlash_configMmapRead  and 
 *  #QSPIFlash_configMmapWrite need to be called to set QSPI in memory mapped mode. #QSPIFlash_mmapRead 
 *  and #QSPIFlash_mmapWrite are the functions  to read/Write from/to serial flash. Please note that the data length
 *  is in number of double words(8 bytes) and source and destination address should be 8 bytes aligned.
 *
 *  In memory mapped mode, serial flash can also be read/written in DMA mode. #QSPIFlash_sysDmaRead and 
 *  #QSPIFlash_sysDmaWrite are the two functions to read/write in DMA mode. Please note that DMA handle and dma 
 *  channel number need to be  provided to QSPI Flash driver. 
 *
 *  @note  In memory mapped mode, because of MPU setting of the mapped flash memory in NON-strongly ordered mode,
 *  ARM flush of writing buffer may be delayed. If read comes before the memory is flushed, incorrect content maybe read
 *  back. To avoid the issue, MEM_BARRIER() can be used when needed to make sure memory content is updated before 
 *  read. For more details on the mode that flash memory is configured for, please refer to SOC_mpu_config.
 *
 *  ## Instrumentation #
 *  Uses DebugP_log functions for debug messages
 *
 *  ## Serial Flash device File #
 *  Every serial flash has to provide an init function and quad read programming function if quad read is supported.
 *
 * ============================================================================
 */

#ifndef QSPI_FLASH_H
#define QSPI_FLASH_H

#include <ti/drivers/dma/dma.h>
#include <ti/drivers/qspi/qspi.h>

#ifdef __cplusplus
    extern "C" {
#endif

/**
@defgroup QSPIFLASH_DRIVER_EXTERNAL_FUNCTION            QSPIFlash Driver External Functions
@ingroup QSPIFLASH_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/

/**
@defgroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION            QSPIFlash Driver Internal Functions
@ingroup QSPIFLASH_DRIVER
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/

/** @addtogroup QSPIFLASH_DRIVER_ERROR_CODE
 *  Base error code for the QSPI module is defined in the
 *  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   QSPI DRIVER Error Code: Invalid command
 */
#define QSPIFLASH_EINVAL                            (MMWAVE_ERRNO_QSPIFLASH_BASE - 1)

/**
 * @brief   QSPI DRIVER Error Code: Out of memory
 */
#define QSPIFLASH_ENOMEM                            (MMWAVE_ERRNO_QSPIFLASH_BASE - 2)

/**
 * @brief   QSPI DRIVER Error Code: Already in use
 */
#define QSPIFLASH_ESFDP                             (MMWAVE_ERRNO_QSPIFLASH_BASE - 3)

/**
 * @brief   QSPI DRIVER Error Code: Flash device is not supported
 */
#define QSPIFLASH_ENOTSUPPORTED                     (MMWAVE_ERRNO_QSPIFLASH_BASE - 4)

/** @}*/

/* QSPI flash supported mode definition */
#define IS_QUAD_SUPPORTED(byte)         ((byte) & 0x40U)
#define IS_DUAL_SUPPORTED(byte)         ((byte) & 0x01U)

/* SPI flash manufacture id */
#define SPANSION_DEV                    (0x01U)
#define MACRONIX_DEV                    (0xC2U)
#define MICRON_DEV                      (0x20U)

/*!
 *  @brief      A handle that is returned from a QSPI_open() call.
 */
typedef void*      QSPIFlash_Handle;

/*!
 *  @brief    QSPI flash Read mode
 *
 *  QSPI flash Memory mapped read mode
 *
 */
typedef enum FLASH_READ_MODE_e
{
    /* Single byte read mode */
    FLASH_SINGLE_MODE = 0U,

    /* Dual byte read mode */
    FLASH_DUAL_MODE = 1U,

    /* Quad byte read mode */
    FLASH_QUAD_MODE = 2U,

    /* Use the supported mode in following sequence
       quadRead-> dualRead->singleRead */
    FLASH_AUTO_MODE = 3U
}FLASH_READ_MODE;

/** @addtogroup QSPIFLASH_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

typedef struct SPIFLASH_devID_t
{
    uint8_t     Manufacture;
    uint8_t     device;
    uint8_t     capacity;
}SPIFLASH_devID;

/*!
 *  @brief    QSPI flash H/W Parameters
 *
 *  QSPI flash parameters are used with the QSPIFlash_open() call.
 *
 */
typedef struct QSPIFlash_HWCfg_t {
    uint32_t       extFlashBaseAddr;           /*!< ExtFlash base address */
} QSPIFlash_HWCfg;

/*!
 *  @brief QSPIFlash Parameters
 *
 *  QSPI flash Parameters are used to with the QSPIFlash_open() call. Default values for
 *  these parameters are set using QSPIFlash_Params_init().
 *
 *  \ingroup QSPIFLASH_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         QSPIFlash_Params_init()
 */
typedef struct QSPIFlash_Params_t
{
    uint32_t                bitRate;                    /*!< QSPI bit rate in Hz */

    QSPI_clockMode          clkMode;                    /*!< QSPI clock polarity and phase settings */

    uint8_t                 dataDelay ;                 /*!< QSPI clock delay, number of clock cycles data is delayed
                                                             after CS becomes active */

    uint8_t                 csPolarity;                 /*!< QSPI Chip select polarity 0- Active low, 1- Active high */
} QSPIFlash_Params;
/** @}*/


/*******************************************************************************************************
 * QSPI Exported API
 *******************************************************************************************************/
extern void QSPIFlash_init(void);
extern QSPIFlash_Handle QSPIFlash_open(QSPI_Handle QSPIHandle, int32_t *errCode);
extern int32_t QSPIFlash_close(QSPIFlash_Handle QSPIFlashHandle);

extern int32_t QSPIFlash_sectorErase(QSPIFlash_Handle QSPIFlashHandle, uint32_t addr);
extern int32_t QSPIFlash_blockErase(QSPIFlash_Handle QSPIFlashHandle, uint32_t addr);
extern int32_t QSPIFlash_chipErase(QSPIFlash_Handle QSPIFlashHandle);
extern void QSPIFlash_writeEnable(QSPIFlash_Handle QSPIFlashHandle);
extern void QSPIFlash_writeDisable(QSPIFlash_Handle QSPIFlashHandle);

extern void QSPIFlash_singleRead(QSPIFlash_Handle QSPIFlashHandle, uint32_t memAddr, uint32_t dataLen, uint8_t* userDataBuf);
extern void QSPIFlash_singleWrite(QSPIFlash_Handle QSPIFlashHandle, uint32_t memAddr, uint32_t dataLen, uint8_t* srcDataAddr);
extern void QSPIFlash_dualRead(QSPIFlash_Handle QSPIFlashHandle, uint32_t srcAddr, uint32_t dataLen, uint16_t* userDataBuf);
extern void QSPIFlash_quadRead(QSPIFlash_Handle QSPIFlashHandle, uint32_t srcAddr, uint32_t dataLen, uint32_t* userDataBuf);

extern int32_t QSPIFlash_configMmapRead(QSPIFlash_Handle QSPIFlashHandle, FLASH_READ_MODE mode, int32_t *errCode);
extern void QSPIFlash_configMmapWrite(QSPIFlash_Handle QSPIFlashHandle);
extern int32_t QSPIFlash_mmapRead(QSPIFlash_Handle QSPIFlashHandle, const uint32_t *memAddr, uint32_t dataLen, uint32_t* userDataBuf);
extern int32_t QSPIFlash_mmapWrite(QSPIFlash_Handle QSPIFlashHandle, const uint32_t* memAddr, uint32_t dataLen, uint32_t* srcDataAddr);

extern void QSPIFlash_progQuadRead(QSPIFlash_Handle QSPIFlashHandle);
extern void QSPIFlash_clearBlockProtectBits(QSPIFlash_Handle QSPIFlashHandle);

extern void QSPIFlash_getDeviceID(QSPIFlash_Handle QSPIFlashHandle, SPIFLASH_devID *devId);
extern uint32_t QSPIFlash_getExtFlashAddr(QSPIFlash_Handle QSPIFlashHandle);

extern int32_t QSPIFlash_sysDmaRead(QSPIFlash_Handle QSPIFlashHandle, DMA_Handle dmaHandle, uint32_t dmaChan, uint32_t srcAddr, uint32_t dstAddr, uint32_t dataLen);
extern int32_t QSPIFlash_sysDmaWrite(QSPIFlash_Handle QSPIFlashHandle, DMA_Handle dmaHandle, uint32_t dmaChan, uint32_t srcAddr, uint32_t dstAddr, uint32_t dataLen);

#ifdef __cplusplus
}
#endif

#endif /* QSPI_DRIVER_H */
