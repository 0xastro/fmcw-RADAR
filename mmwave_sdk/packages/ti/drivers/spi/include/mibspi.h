/*
 * Copyright (c) 2016, Texas Instruments Incorporated
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
#ifndef MIBSPI_H
#define MIBSPI_H

#include "reg_mibspi.h"
#include <ti/drivers/dma/dma.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/osal/HwiP.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Transfer group used in slave mode
 */
#define MIBSPI_SLAVEMODE_TRANS_GROUP        0U

/**
 * @brief
 *  MIBSPI interrupt level
 */
#define MIBSPI_INT_LEVEL                    1U

/**
 * @brief
 *  Maximum CS supported for the device
 */
#define MIBSPI_MAX_CS                       MIBSPI_SLAVE_MAX

/**
 * @brief
 *  CS configuration value when none of the CS is activated
 */
#define CS_NONE                             0xFFU

/**
 * @brief
 *  Max number of transport group
 */
#define MIBSPI_NUM_TRANS_GROUP              8U

/**
 * @brief
 *  Max number of DMA group
 */
#define MIBSPI_NUM_DMA_GROUP                8U

/**
 * @brief
 *  MibSPI RAM buffer mode
 */
#define MIBSPI_RAM_BUFFER_MODE              6U

/**
 * @brief
 *  Invalide DMA reqline value
 */
#define MIBSPI_INVALID_DMA_REQLINE          (uint8_t)0xFFU

/**
 * @brief
 *  MibSPI Driver DMA call back funtion
 */
typedef void (*t_callBackFuncPTR)(void *transaction);

/**
 * @brief
 *  MibSPI Driver DMA interrupt handler function
 */
typedef void (*t_intHandlerFunc)(void);

/**
 * @brief
 *  SPI Driver Transfer type
 *
 * @details
 *  The structure is used to store the internal transfer type
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MibSpi_transferType_e
{
    /* MIBSPI transfer to send data */
    MIBSPI_TRANSFER_TX = 0,

    /* MIBSPI transfer to receive data */
    MIBSPI_TRANSFER_RX,

    /* MIBSPI transfer to send dummy data */
    MIBSPI_TRANSFER_DUMMY_TX,

    /* MIBSPI transfer to put recevied data in scratch buffer */
    MIBSPI_TRANSFER_DUMMY_RX
}MibSpi_transferType;

/**
 * @brief
 *  SPI Driver DMA request line configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration for DMA request lines.
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpi_DMAReqlineCfg_t
{
    /**
     * @brief   TX DMA Request Line number
     */
    uint8_t     txDmaReqLine;

    /**
     * @brief   RX DMA Request Line number
     */
    uint8_t     rxDmaReqLine;
}MibSpi_DMAReqlineCfg;

/**
 * @brief
 *  SPI Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to SPI driver instance
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpi_HwCfg_t
{
    /**
     * @brief   Base address of the MibSpi register address space to be used.
     */
    MIBSPIRegs             *ptrSpiRegBase;

    /**
     * @brief   Base address of the MibSpi ram address space to be used.
     */
    MIBSPIRam               *ptrMibSpiRam;

    /**
     * @brief   SPI clock source frequency in Hz
     *             It will be used to calculate prescaler for Master mode
     */
    uint32_t                clockSrcFreq;

    /**
     * @brief   Interrupt Number for INT0
     */
    uint32_t                interrupt0Num;

    /**
     * @brief   Interrupt Number for INT1
     */
    uint32_t                interrupt1Num;

    /**
     * @brief   SPI DMA reqline definition
     */
    MibSpi_DMAReqlineCfg    dmaReqlineCfg[MIBSPI_SLAVE_MAX];
}MibSpi_HwCfg;

/**
 * @brief
 *  SPI Driver Statistics
 *
 * @details
 *  The structure is used to store driver statistics
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpiDriverHWStats_t
{
    /**
     * @brief   Number of data length error interrupts
     */
    uint32_t    dlenErr;

    /**
     * @brief   Number of timeout interrupts
     */
    uint32_t    timeout;

    /**
     * @brief   Number of Parity Error interrupts
     */
    uint32_t    parErr;

    /**
     * @brief   Number of De-synchronization of slave device interrupts
     */
    uint32_t    desync;

    /**
     * @brief   Number of mismatch of internal transmit data and transmittted data error interrupts
     */
    uint32_t    bitErr;

    /**
     * @brief   Number of RX Overrun Error Interrupts
     */
    uint32_t    rxOvrnErr;

    /**
     * @brief   Number of RX Full Interrupts
     */
    uint32_t    rxFull;

    /**
     * @brief   Number of TX Empty Interrupts
     */
    uint32_t    txEmpty;

    /**
     * @brief   Number of TG Complete interrupts
     */
    uint32_t    TGComplete[MIBSPI_NUM_TRANS_GROUP];
}MibSpiDriverHWStats;

/**
 * @brief
 *  SPI Driver Info for Master
 *
 * @details
 *  The structure is used to store the driver info for Master.
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpi_driverDmaInfo_t
{
    /**
     * @brief   DMA channel for TX.
     */
    uint32_t    dmaChannelTx;

    /**
     * @brief   DMA channel for RX.
     */
    uint32_t    dmaChannelRx;

    /**
     * @brief   DMA reqline for TX.
     */
    uint32_t    dmaReqlineTx;

    /**
     * @brief   DMA reqline for RX.
     */
    uint32_t    dmaReqlineRx;

    /**
     * @brief   Number of DMA Recieve Interrupts
     */
    uint32_t    rxDmaIntCnt;

    /**
     * @brief   Number of DMA Transmit Interrupts
     */
    uint32_t    txDmaIntCnt;
}MibSpi_driverDmaInfo;

/*!
 *  @brief      SPI Driver Object
 *
 * @details
 *  The structure is used to store the SPI driver internal variables.
 *  The application must not access any member variables of this structure!
 *
 *  \ingroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE
 *
 */
typedef struct MibSpiDriver_Object_t
{
    /**
     * @brief   SPI driver handle.
     */
    SPI_Handle              spiHandle;

    /**
     * @brief   Pointer to MibSpi driver Hardware Configuration
     */
    MibSpi_HwCfg            *ptrHwCfg;

    /**
     * @brief   MibSpi driver parameters
     */
    SPI_Params              params;

    /**
     * @brief   Registered Interrupt Handler.
     */
    HwiP_Handle             hwiHandle;

    /**
     * @brief   SPI transaction configuration for the current transaction. If there is
     *             no on-going transaction, it is set to NULL.
     */
    SPI_Transaction         *transaction;

    /**
     * @brief   Handle to the Transfer complete semaphore.
     */
    SemaphoreP_Handle       transferCompleteSem;

    /**
     * @brief   DMA information used in transfer for remote peers
     */
    MibSpi_driverDmaInfo    dmaInfo[MIBSPI_SLAVE_MAX];

    /**
     * @brief   MibSPI mode RAM offset start settings for remote peers
     */
    uint8_t                 rambufStart[MIBSPI_SLAVE_MAX];

    /**
     * @brief   MibSPI mode RAM offset end settings for remote peers
     */
    uint8_t                 rambufEnd[MIBSPI_SLAVE_MAX];

    /**
     * @brief   Rx Scratch buffer, used as scratch buffer to dump received
     *          data from SPI transfer when application does not provide
     *          receive buffer.
     */
    uint16_t                rxScratchBuffer;

    /**
     * @brief   Tx Scratch buffer, used when TX data is not provided for SPI_transfer()
     *          The driver transmits txDummyValue from ::SPI_Params_t
     */
    uint16_t                txScratchBuffer;

    /**
     * @brief   MibSpi driver stats.
     */
    MibSpiDriverHWStats     hwStats;
} MibSpiDriver_Object, *MibSpiDriver_Handle;

/**@}*/
#ifdef __cplusplus
}
#endif

#endif
