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
 *  @file       qspiflash_device_macronix.c
 *
 *  @brief      QSPI Flash devices configuration
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


#define FLASH_CONFIG_QUADREAD_ENABLE        0x40U

/**************************************************************************
 *************************** Internal Functions Prototype ***********************
 **************************************************************************/

static void QSPIFlash_macronix_init(SPIFLASH_Params     *ptrFlashParams);
static void QSPIFlash_macronix_progQuadRead(QSPIFlash_Handle QSPIFlashHandle);

/**************************************************************************
 *************************** Internal Functions ********************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *     This function initializes MACRONIX serial flash command set
 *
 *   @param[in] ptrFlashParams         Pointer to the serial Flash parameters
 
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIFlash_macronix_init(SPIFLASH_Params     *ptrFlashParams)
{
    if(ptrFlashParams->devID.Manufacture != MACRONIX_DEV)
    {
        DebugP_assert(0U);
    }

    /* Read Status commands */
    ptrFlashParams->readSR2Cmd = FLASH_CMD_RDCR_MACRONIX;
    ptrFlashParams->readSR3Cmd = 0;

    ptrFlashParams->writeStatusRegCmd = FLASH_CMD_WR_SR;
    ptrFlashParams->readDataCmd = FLASH_CMD_FAST_SINGLE_READ;

    /* Protection bit mask in status register */
    ptrFlashParams->protectBitsMask = (uint8_t)0x3CU;
}

/**
 *  @b Description
 *  @n
 *     This function initializes MACRONIX serial flash command set
 *
 *   @param[in] QSPIFlashHandle         Pointer to the QSPI flash driver object
 
 *  \ingroup QSPIFLASH_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval None
 */
static void QSPIFlash_macronix_progQuadRead(QSPIFlash_Handle QSPIFlashHandle)
{
    uint32_t            statusReg, newStatusReg;
    uint32_t            configReg;
    uint32_t            status;
    QSPI_Handle         QSPIHandle;
    SPIFLASH_Params     *ptrFlashParams;
    QSPIFLASH_Driver    *ptrQSPIFlashDrv;

    DebugP_assert(QSPIFlashHandle != (QSPIFlash_Handle)NULL);
    
    ptrQSPIFlashDrv = (QSPIFLASH_Driver *)QSPIFlashHandle;

    QSPIHandle =ptrQSPIFlashDrv->ptrQSPIHandle;
    
    /* Get flash parameter pointer */
    ptrFlashParams  = &ptrQSPIFlashDrv->flashParams;

    if(ptrFlashParams->devID.Manufacture != MACRONIX_DEV)
    {
        DebugP_assert(0U);
    }

    /* Read status register1 */
    statusReg = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);

    configReg = QSPIFlashGetStatus( QSPIHandle, ptrFlashParams->readSR2Cmd);

    newStatusReg = statusReg | FLASH_CONFIG_QUADREAD_ENABLE;
    
    if(statusReg != newStatusReg)
    {
        /* Make write enable flash before write or erase */
        QSPIFlash_writeEnable(ptrQSPIFlashDrv);

        /*send write register command*/
        QSPIFlashWriteCmd( QSPIHandle, ptrFlashParams->writeStatusRegCmd, 3U);
        
        /*send status register byte*/
        QSPIFlashWriteCmd(QSPIHandle, newStatusReg, 2U);
        
        /*send configuratin register byte*/
        QSPIFlashWriteCmd(QSPIHandle, configReg, 1U);

        do
        {
            status = QSPIFlashGetStatus(QSPIHandle, ptrFlashParams->readSR1Cmd);
        }while((status & (FLASH_STATUS_BUSY |FLASH_STATUS_WRENABLE )) != 0x0U);
    }
}


QSPIFlash_deviceFxn QSPIFlash_deviceFxnTbl_macronix =
{
    &QSPIFlash_macronix_init,
    &QSPIFlash_macronix_progQuadRead
};
