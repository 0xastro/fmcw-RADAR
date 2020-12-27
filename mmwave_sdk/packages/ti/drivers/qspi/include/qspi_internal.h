/*
 * Copyright (c) 2017, Texas Instruments Incorporated
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


/** @file qspi_internal.h
*   @brief QSPI Driver internal Definition File
*
*/

#ifndef QSPI_INTERNAL_H
#define QSPI_INTERNAL_H

#include "ti/drivers/qspi/include/reg_qspi.h"

/**
 * @brief   QSPI status in Busy state
 */
#define QSPI_SPI_STATUS_REG_BUSY              (1U)

/**
 * @brief   QSPI Clock control setting to disable clock
 */
#define QSPI_CLK_CTRL_CLKEN_DCLOCK_OFF        (0U)

/**
 * @brief   QSPI Clock control setting to enable clock
 */
#define QSPI_CLK_CTRL_CLKEN_DCLOCK_ON         (1U)

/**
 * @brief   QSPI clock setting for maximum data delay after CS is activated
 */
#define QSPI_MAX_DATA_DELAY                   (3U)

/**
 * @brief
 *  QSPI interface configuration
 *
 * @details
 *  The enum includes all the supported interface mode in QSPI
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef enum QSPI_INTF_e
{
    /* QSPI config interface mode */
    QSPI_INTF_CFG = 0x0U,

    /* QSPI_Memory map mode */
    QSPI_INTF_MM   = 0x1U
}QSPI_INTF;

/**
 * @brief
 *  QSPI idle mode configuration
 *
 * @details
 *  The enum includes all the supported idle mode in QSPI
 *
 *  \ingroup QSPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
 typedef enum QSPI_IDLE_MODE_e
{
    /* QSPI forced idle mode */
    QSPI_IDLE_MODE_FORCE_IDLE = 0U,

    /* QSPI no-idle mode */
    QSPI_IDLE_MODE_NO_IDLE = 1U,

    /* QSPI smart idle mode */
    QSPI_IDLE_MODE_SMART_IDLE = 2U,

    /* QSPI smart idle that can be waked up mode */
    QSPI_IDLE_MODE_SMART_IDLE_WAKEUP_CAPABLE = 3U
}QSPI_IDLE_MODE;

#endif /* end of QSPI_INTERNAL_H */

