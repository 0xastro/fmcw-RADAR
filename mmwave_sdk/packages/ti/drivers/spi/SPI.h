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
 *  @file       SPI.h
 *
 *  @brief      SPI driver interface
 *
 */

 /** @mainpage SPI Driver
 *
 *  The SPI header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/SPI.h>
 *  @endcode
 *
 *  # Operation #
 *  The SPI driver in TI-RTOS is designed to serve a means to move data
 *  between SPI peripherals. This driver does not interpret any of the data sent
 *  to or received from this peripheral.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the SYS/BIOS specific primitives to allow for thread-safe
 *  operation.
 *
 *  The SPI driver operates on some key definitions and assumptions:
 *  - The driver operates transparent from the chip select. Some SPI controllers
 *    feature a hardware chip select to assert SPI slave peripherals. See the
 *    specific peripheral implementations on chip select requirements.
 *
 *  - The SPI protocol does not account for a built-in handshaking mechanism and
 *    neither does this SPI driver. Therefore, when operating in ::SPI_SLAVE
 *    mode, the application must provide such a mechanism to ensure that the
 *    SPI slave is ready for the SPI master. The SPI slave must call
 *    SPI_transfer() *before* the SPI master starts transmitting. Some example
 *    application mechanisms could include:
 *    - Timed delays on the SPI master to guarantee the SPI slave is be ready
 *      for a SPI transaction.
 *    - A form of GPIO flow control from the slave to the SPI master to notify
 *      the master when ready.
 *  - When SPI operates in ::SPI_MASTER, the partition of TX/RX RAM should be 
 *    provided by application by providing proper values in ::SPI_Params, such as 
 *    number of slaves and slave profiles. 
 *    The maximum number of supported slaves is platform specfic, please refer to 
 *    TRM for the information. The maximum number of slaves supported in the 
 *    driver is ::MIBSPI_SLAVE_MAX.
 *
 *  The platform specific SPI file present in the ti/drivers/spi/platform
 *  directory. This file is built as a part of the SPI Library for the specific
 *  platform.
 *
 *  ## Opening the driver #
 *  
 *  Code examples for opening SPI driver in master and slave mode
 *
 *  Open SPI driver as ::SPI_MASTER
 *
 *  @code
 *  SPI_Handle      handle;
 *  SPI_Params      params;
 *  SPI_Transaction spiTransaction;
 *
 *  SPI_Params_init(&params);
 *  params.mode  = SPI_MASTER;
 *  params.u.masterParams.bitRate = bitRate;
 *  params.u.masterParams.numSlaves = 1;
 *  params.u.masterParams.slaveProf[0].chipSelect = 0;
 *  params.u.masterParams.slaveProf[0].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
 *  params.u.masterParams.slaveProf[0].dmaCfg.txDmaChanNum =1U; 
 *  params.u.masterParams.slaveProf[0].dmaCfg.rxDmaChanNum =0U;
 *  params.u.masterParams.slaveProf[1].chipSelect = 1;
 *  params.u.masterParams.slaveProf[1].ramBufLen = MIBSPI_RAM_MAX_ELEM/2;
 *  params.u.masterParams.slaveProf[1].dmaCfg.txDmaChanNum =3U; 
 *  params.u.masterParams.slaveProf[1].dmaCfg.rxDmaChanNum =2U;
 *
 *  handle = SPI_open(someSPI_configIndexValue, &params);
 *  if (!handle) {
 *      System_printf("SPI did not open");
 *  }
 *  @endcode
 *
 *
 *  Open SPI driver as ::SPI_SLAVE
 *
 *  @code
 *  SPI_Handle      handle;
 *  SPI_Params      params;
 *  SPI_Transaction spiTransaction;
 *
 *  SPI_Params_init(&params);
 *  params.mode = SPI_SLAVE;
 *  params.frameFormat = SPI_POL0_PHA0;
 *  params.pinMode = SPI_PINMODE_4PIN_CS;
 *  params.shiftFormat = SPI_MSB_FIRST;
 *  params.dmaEnable = 1;
 *  params.dmaHandle = gDmaHandle;
 *  params.u.slaveParams.dmaCfg.txDmaChanNum =1U;
 *  params.u.slaveParams.dmaCfg.rxDmaChanNum =0U;
 *  handle = SPI_open(someSPI_configIndexValue, &params);
 *  if (!handle) {
 *      System_printf("SPI did not open");
 *  }
 *  @endcode
 *
 *  ## Transferring data #
 *  Data transmitted and received by the SPI peripheral is performed using
 *  SPI_transfer(). SPI_transfer() accepts a pointer to a SPI_Transaction
 *  structure that dictates what quantity of data is sent and received.
 *
 *  @code
 *  SPI_Transaction spiTransaction;
 *
 *  spiTransaction.count = someIntegerValue;
 *  spiTransaction.txBuf = transmitBufferPointer;
 *  spiTransaction.rxBuf = receiveBufferPointer;
 *  spiTransaction.slaveIndex = index1;  // For master mode only
 *
 *  ret = SPI_transfer(handle, &spiTransaction);
 *  if (!ret) {
 *      System_printf("Unsuccessful SPI transfer");
 *  }
 *  @endcode
 *
 *  When SPI driver is configured in ::SPI_MASTER model, slave index need to be
 *  provided to indicate which slave the transfer is for.
 *
 *  ## Canceling a transaction #
 *  SPI_transferCancel() is used to cancel a SPI transaction - not supported.
 *
 *  @code
 *  SPI_transferCancel(handle);
 *  @endcode
 *
 *  # Implementation #
 *
 *  This module serves as the main interface for TI-RTOS applications. Its
 *  purpose is to redirect the module's APIs to specific peripheral
 *  implementations which are specified using a pointer to a SPI_FxnTable.
 *
 *  The SPI driver interface module is joined (at link time) to a
 *  NULL-terminated array of SPI_Config data structures named *SPI_config*.
 *  *SPI_config* is implemented in the application with each entry being an
 *  instance of a SPI peripheral. Each entry in *SPI_config* contains a:
 *  - (SPI_FxnTable *) to a set of functions that implement a SPI peripheral
 *  - (void *) data object that is associated with the SPI_FxnTable
 *  - (void *) hardware attributes that are associated to the SPI_FxnTable
 *
 *  # SPI transfer with multiple RAM buffer (icount)
 *  SPI hardware has an internal RAM buffer that stores transmit/receive data 
 *  element in 8bits or 16bits. 
 *  The SPI driver has a compile time option to transfer data bigger than RAM 
 *  buffer size. This is intended for high throughput transfers. But it has some 
 *  limitations and not supported in all SPI modes.
 *  
 *  SPI mode |  Pin Mode  | Supported?  | Limitations
 *  -------- | -------- |----------- |----------
 *  Master   |  4-pin  | Yes       |  Note1
 *  Master   |  3-pin  | Yes       |  Note1
 *  Slave    |  4-pin  | Yes       |  Note2
 *  Slave    |  3-pin  | No        |  None
 * 
 * 
 *  Note1: There will be gaps between every transfer of the RAM buffer size for 
 *        internal DMA copy of the data. During this time, Clock and CS will be 
 *        inactive.
 *  
 *  Note2: Slave needs time to do DMA copy of received data for every RAM buffer.
 *        Hence require the SPI master to deactivate  CS/ClK signal during this time. 
 *        When using another XWR1xxx device as master, it can be achieved by
 *        setting C2Tdelay/T2Cdelay/wdelay. 
 *  
 *  The maximum elements is defined as MIBSPI_RAM_MAX_ELEM. 
 *  For multiple slaves scenarios, this number is divided 
 *  among all slaves. This information is provided from application in 
 *  SPI_MasterModeParams. The maximum icount value is 32. Please make sure 
 *  the transfer length is not exceed 32 * ramLen. For High clock rate, it is recommended
 *  to use small icount values.
 *
 *  # Limitation #
 *  
 *  # Instrumentation #
 *
 *  The SPI driver interface produces log statements if instrumentation is
 *  enabled.
 *
 *  Diagnostics Mask | Log details |
 *  ---------------- | ----------- |
 *  Diags_USER1      | basic operations performed |
 *  Diags_USER2      | detailed operations performed |
 *
 *  ============================================================================
 */

#ifndef ti_drivers_SPI__include
#define ti_drivers_SPI__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/drivers/dma/dma.h>

/**
@defgroup SPI_DRIVER_EXTERNAL_FUNCTION            SPI Driver External Functions
@ingroup SPI_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE     SPI Driver External Data Structures
@ingroup SPI_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup SPI_DRIVER_ERROR_CODE                   SPI Driver Error Codes
@ingroup SPI_DRIVER
@brief
*   The section has a list of all the error codes which are generated by the CRC Driver
*   module
*/
/**
@defgroup SPI_DRIVER_INTERNAL_FUNCTION            SPI Driver Internal Functions
@ingroup SPI_DRIVER
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup SPI_DRIVER_INTERNAL_DATA_STRUCTURE      SPI Driver Internal Data Structures
@ingroup SPI_DRIVER
@brief
*   The section has a list of all internal data structures which are used internally
*   by the SPI module.
*/

/**
 *  @defgroup SPI_CONTROL SPI_control command and status codes
 *  These SPI macros are reservations for SPI.h
 *  @{
 */

/*!
 * Common SPI_control command code reservation offset.
 * SPI driver implementations should offset command codes with SPI_CMD_RESERVED
 * growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define SPIXYZ_CMD_COMMAND0     SPI_CMD_RESERVED + 0
 * #define SPIXYZ_CMD_COMMAND1     SPI_CMD_RESERVED + 1
 * @endcode
 */
#define SPI_CMD_RESERVED            32U

/*!
 * Common SPI_control status code reservation offset.
 * SPI driver implementations should offset status codes with
 * SPI_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define SPIXYZ_STATUS_ERROR0    SPI_STATUS_RESERVED - 0
 * #define SPIXYZ_STATUS_ERROR1    SPI_STATUS_RESERVED - 1
 * #define SPIXYZ_STATUS_ERROR2    SPI_STATUS_RESERVED - 2
 * @endcode
 */
#define SPI_STATUS_RESERVED        (-32)

/**
 *  @defgroup SPI_STATUS Status Codes
 *  SPI_STATUS_* macros are general status codes returned by SPI_control()
 *  @{
 *  @ingroup SPI_CONTROL
 */

/*!
 * @brief   Successful status code returned by SPI_control().
 *
 * SPI_control() returns SPI_STATUS_SUCCESS if the control code was executed
 * successfully.
 *
 *  \ingroup SPI_DRIVER_ERROR_CODE
 *
 */
#define SPI_STATUS_SUCCESS         0

/*!
 * @brief   An error status code returned by SPI_control() for undefined
 * command codes.
 *
 * SPI_control() returns SPI_EINVALCMD if the control code is not
 * recognized by the driver implementation.
 *
 *  \ingroup SPI_DRIVER_ERROR_CODE
 *
 */
#define SPI_EINVALCMD                        (MMWAVE_ERRNO_SPI_BASE - 1)

/*!
 * @brief   An error  code returned by SPI APIs for invalid input values
 *
 *  \ingroup SPI_DRIVER_ERROR_CODE
 *
 */
#define SPI_EINVAL                           (MMWAVE_ERRNO_SPI_BASE - 2)

/**
 * @brief   An error  code returned by SPI APIs for features not implemented
 *
 *  \ingroup SPI_DRIVER_ERROR_CODE
 *
 */
#define SPI_ENOTIMPL                         (MMWAVE_ERRNO_SPI_BASE - 3)

/**
 * @brief   An error  code returned by SPI APIs for features not implemented
 *
 *  \ingroup SPI_DRIVER_ERROR_CODE
 *
 */
#define SPI_EDMACONF                         (MMWAVE_ERRNO_SPI_BASE - 4)

/** @}*/

/**
 *  @defgroup SPI_CMD Command Codes
 *  SPI_CMD_* macros are general command codes for SPI_control(). Not all SPI
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup SPI_CONTROL
 */

/*!
 * @brief Command used by SPI_control to enable loopback
 *
 */
#define SPI_CMD_LOOPBACK_ENABLE                         (uint32_t)(SPI_CMD_RESERVED + 0)

/*!
 * @brief Command used by SPI_control to set Clock phase and polarity
 *
 */
#define SPI_CMD_SET_CLOCK_PHASEPOLARITY                 (uint32_t)(SPI_CMD_RESERVED + 1)

/** @}*/

/** @}*/

/*!
 *  @brief    Wait forever define
 */
#define SPI_WAIT_FOREVER             0xFFFFFFFFU

/*!
 *  @brief    Maximum element in MibSPI RAM. 
 *            For 8bit data size, the maximum transfer size is 64 bytes
 *            For 16bit data size, the maximum transfer size is 128 bytes
 */
#define MIBSPI_RAM_MAX_ELEM          64U

/**
 * @brief
 *  Max number of slaves supported when MibSPI is configured as master
 */
#define MIBSPI_SLAVE_MAX             3U

/*!
 *  @brief      A handle that is returned from a SPI_open() call.
 */
typedef struct SPI_Config_t      *SPI_Handle;

/*!
 *  @brief      Status codes that are set by the SPI driver.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum SPI_Status_e
{
    /* SPI transfer completed */
    SPI_TRANSFER_COMPLETED = 0,

    /* SPI transfer started */
    SPI_TRANSFER_STARTED,

    /* SPI transfer canceled */
    SPI_TRANSFER_CANCELED,

    /* SPI transfer failed */
    SPI_TRANSFER_FAILED,

    /* SPI transfer CSN deasserted  */
    SPI_TRANSFER_CSN_DEASSERT,

    /* SPI transfer timeout */
    SPI_TRANSFER_TIMEOUT,

    /* SPI transfer invalid input parameter */
    SPI_TRANSFER_INVAL,

    /* SPI transfer in progress */
    SPI_TRANSFER_INPROG
} SPI_Status;

/*!
 *  @brief
 *  A ::SPI_Transaction data structure is used with SPI_transfer(). It indicates
 *  how many ::SPI_FrameFormat frames are sent and received from the buffers
 *  pointed to txBuf and rxBuf.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct SPI_Transaction_t
{
    /* User input (write-only) fields */
    size_t     count;      /*!< Number of frames for this transaction */
    void      *txBuf;      /*!< void * to a buffer with data to be transmitted */
    void      *rxBuf;      /*!< void * to a buffer to receive data */
    uint8_t   slaveIndex; /*!< Index of the slave enabled for this transfer */

    /* User output (read-only) fields */
    SPI_Status status;     /*!< Status code set by SPI_transfer */

    /* Driver-use only fields */
} SPI_Transaction;

/*!
 *  @brief      The definition of a callback function used by the SPI driver
 *              Callback mode is not support.
 *
 *  @param      SPI_Handle          SPI_Handle
 *  @param      SPI_Transaction*    SPI_Transaction*
 */
typedef void        (*SPI_CallbackFxn) (SPI_Handle handle,
                                        SPI_Transaction * transaction);

/*!
 *  @brief
 *  A ::SPI_Stats data structure is used with SPI_getStats() to get driver statistics. 
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct SPI_Stats_t
{
    uint32_t   dlenErr;     /*!<Number of data length error */
    uint32_t   timeout;     /*!<Number of timeout */
    uint32_t   parErr;      /*!<Number of Parity error */
    uint32_t   desync;      /*!<Number of De-synchronization of slave device - master only */
    uint32_t   bitErr;      /*!<Number of data mismatch transmit data error - master only */
    uint32_t   rxOvrnErr;   /*!<Number of RX Overrun Error */
} SPI_Stats;

/*!
 *  @brief
 *  Definitions for various SPI modes of operation.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum SPI_Mode_e
{
    SPI_MASTER    = 0,        /*!< SPI in master mode*/
    SPI_SLAVE       = 1         /*!< SPI in slave mode*/
} SPI_Mode;

/*!
 *  @brief
 *  Definitions for various SPI data frame formats.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum SPI_FrameFormat_e
{
    SPI_POL0_PHA0   = 0,    /*!< SPI mode Polarity 0 Phase 0 */
    SPI_POL0_PHA1   = 1,    /*!< SPI mode Polarity 0 Phase 1 */
    SPI_POL1_PHA0   = 2,    /*!< SPI mode Polarity 1 Phase 0 */
    SPI_POL1_PHA1   = 3,    /*!< SPI mode Polarity 1 Phase 1 */
} SPI_FrameFormat;

/*!
 *  @brief
 *  Definitions for SPI Pins Operation Mode.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum SPI_PinMode_e
{
    SPI_PINMODE_3PIN   = 0,        /*!< 3 Pin operation Mode */
    SPI_PINMODE_4PIN_CS  = 1       /*!< 4 Pin operation Mode with CS signal */
} SPI_PinMode;

/*!
 *  @brief
 *  Definitions for SPI Data shift format.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum SPI_DataShiftFmt_e
{
    SPI_MSB_FIRST = 0,    /*!< MSB shift out first */
    SPI_LSB_FIRST  = 1    /*!< LSB shift out first */
} SPI_DataShiftFmt;

/*!
 *  @brief
 *  Definitions for  SPI loopback modes.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum MibSpi_LoopBackType_e
{
    SPI_LOOPBK_DIGITAL= 0,    /*!< SPI digital loopback  mode */
    SPI_LOOPBK_ANALOG = 1,    /*!< SPI analog loopback  mode */
    SPI_LOOPBK_NONE = 2,    /*!< SPI loopback  disable */
} MibSpi_LoopBackType;

/*!
 *  @brief
 *
 *  SPI transfer mode determines the whether the SPI controller operates
 *  synchronously or asynchronously. In ::SPI_MODE_BLOCKING mode SPI_transfer()
 *  blocks code execution until the SPI transaction has completed.
 *  Only ::SPI_MODE_BLOCKING is supported.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum SPI_TransferMode_e {
    /*!
     * SPI_transfer() blocks execution. This mode can only be used when called
     * within a Task context
     */
    SPI_MODE_BLOCKING
} SPI_TransferMode;

/*!
 *  @brief SPI DMA channel config
 *
 *  When DMA is enabled, system DMA channel number need to be provided per transfer group.
 *  In SPI slave mode, only one transfer group is used, one pair of DMA channel number(TX and RX) need to configurred.
 *  In SPI master mode, DMA channel should be provided per slave profile.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct SPI_DMAChanCfg_t
{
    uint32_t            txDmaChanNum;              /*!< TX DMA Channel number, must be provided by application,
                                                        refer to DMA driver for possible DMA channels */

    uint32_t            rxDmaChanNum;              /*!< RX DMA channel number, must be provided by application,
                                                        refer to DMA driver for possible DMA channels */
} SPI_DMAChanCfg;

/*!
 *  @brief SPI slave profile Parameters
 *
 *  When SPI is configured as master mode, it may connect to multiple slaves. This data structure captures 
 *  the the configurations for remote slaves.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct SPI_SlaveProfile_t
{
    uint8_t             chipSelect;                /*!< CS0-CS7 signal number from 0 -7 */

    uint8_t             ramBufLen;                 /*!< RAM Length in bytes that used by the slave */

    SPI_DMAChanCfg      dmaCfg;                    /*!< System DMA channel configuration, used when DMA is enabled */
} SPI_SlaveProfile;

/*!
 *  @brief SPI master Parameters
 *
 *  SPI master Parameters are used  with the SPI_open() call when mode is set to SPI_MASTER. 
 *  Default values for these parameters are set using SPI_Params_init().
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         SPI_Params_init()
 */
typedef struct SPI_MasterModeParams_t
{
    uint32_t            bitRate;                    /*!< SPI bit rate in Hz */

    uint8_t             numSlaves;                  /*!< Number of slaves connected to SPI master */

    uint8_t             t2cDelay;                   /*!< Transmit end to chip select inactive delay */
    
    uint8_t             c2tDelay;                   /*!< Chip select active to transmit start delay */
    
    uint8_t             wDelay;                     /*!< Delay in between transmissions */
    
    SPI_SlaveProfile    slaveProf[MIBSPI_SLAVE_MAX]; /*!< Slave profile for each slave */
} SPI_MasterModeParams;

/*!
 *  @brief SPI slave mode Parameters
 *
 *  SPI slave Parameters are used with the SPI_open() call when mode is set to SPI_SLAVE. 
 *  Default values for these parameters are set using SPI_Params_init().
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         SPI_Params_init()
 */
typedef struct SPI_SlaveModeParams_t
{
    SPI_DMAChanCfg      dmaCfg;                    /*!< System DMA channel configuration, used when DMA is enabled */

    uint8_t             chipSelect;                /*!< CS0-CS7 signal number from 0 -7 */
    
} SPI_SlaveModeParams;

/*!
 *  @brief SPI Parameters
 *
 *  SPI Parameters are used to with the SPI_open() call. Default values for
 *  these parameters are set using SPI_Params_init().
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa         SPI_Params_init()
 */
typedef struct SPI_Params_t
{
    SPI_Mode            mode;                      /*!< Master or Slave mode, refer to ::SPI_Mode_e */

    union 
    {
        SPI_MasterModeParams  masterParams;        /*!< Configuration dedicated to master mode, refer to ::SPI_MasterModeParams_t */
    
        SPI_SlaveModeParams   slaveParams;         /*!< Configuration dedicated to slave mode, refer to ::SPI_SlaveModeParams_t */
    }u;
    
    SPI_PinMode         pinMode;                   /*!< Pin operation Mode, refer to ::SPI_PinMode_e */

    uint32_t            dataSize;                  /*!< SPI data frame size in bits, only 8 and 16 are supported */

    SPI_FrameFormat     frameFormat;               /*!< SPI frame format, refer to ::SPI_FrameFormat_e */

    SPI_DataShiftFmt    shiftFormat;                  /*!< SPI Data shift format, refer to ::SPI_DataShiftFmt_e */ 
    
    uint8_t             dmaEnable;                 /*!< DMA mode enable. When Dma mode is disable, SPI driver
                                                        is operated in polling mode, there is while(1) loop waiting for
                                                        hardware to finish */

    DMA_Handle          dmaHandle;                 /*!< System DMA Handle */
                                                        
    uint8_t             eccEnable;                 /*!< ECC mode enable */
    
    uint8_t             csHold;                    /*!< CS Hold enable */

    uint16_t            txDummyValue;              /*!< 16 bit value transmitted when no TX data is provided for SPI_tranfer() */

    SPI_TransferMode    transferMode;              /*!< Blocking or Callback mode */

    uint32_t            transferTimeout;           /*!< Transfer timeout in system ticks (Not supported with all implementations */

    SPI_CallbackFxn     transferCallbackFxn;       /*!< Callback function pointer, refer to ::SPI_CallbackFxn */

    uintptr_t           custom;                    /*!< Custom argument used by driver implementation */
} SPI_Params;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_close().
 */
typedef void        (*SPI_CloseFxn)          (SPI_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_control().
 */
typedef int32_t     (*SPI_ControlFxn)        (SPI_Handle handle,
                                              uint32_t cmd,
                                              void *arg);

typedef int32_t     (*SPI_GetStatsFxn)       (SPI_Handle handle, 
                                             SPI_Stats *ptrStats);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_init().
 */
typedef void        (*SPI_InitFxn)           (SPI_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_open().
 */
typedef SPI_Handle  (*SPI_OpenFxn)           (SPI_Handle handle,
                                              SPI_Params *params);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_serviceISR().
 */
typedef void        (*SPI_ServiceISRFxn)     (SPI_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_transfer().
 */
typedef bool        (*SPI_TransferFxn)       (SPI_Handle handle,
                                              SPI_Transaction *transaction);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              SPI_transferCancel().
 */
typedef void        (*SPI_TransferCancelFxn) (SPI_Handle handle);

/*!
 *  @brief      The definition of a SPI function table that contains the
 *              required set of functions to control a specific SPI driver
 *              implementation.
 */
typedef struct SPI_FxnTable_t
{
    /*! Function to close the specified peripheral */
    SPI_CloseFxn            closeFxn;

    /*! Function to implementation specific control function */
    SPI_ControlFxn          controlFxn;

    /*! Function to initialize the given data object */
    SPI_InitFxn             initFxn;

    /*! Function to open the specified peripheral */
    SPI_OpenFxn             openFxn;

    /*! Function to initiate a SPI data transfer */
    SPI_TransferFxn         transferFxn;

    /*! Function to cancel SPI data transfer */
    SPI_TransferCancelFxn   transferCancelFxn;

    /*! Function to service the SPI instance */
    SPI_ServiceISRFxn       serviceISRFxn;

    /*! Function to get SPI driver stats */
    SPI_GetStatsFxn         getStatsFxn;
} SPI_FxnTable;

/*!
 *  @brief SPI Global configuration
 *
 *  The SPI_Config structure contains a set of pointers used to characterize
 *  the SPI driver implementation.
 *
 *  This structure needs to be defined before calling SPI_init() and it must
 *  not be changed thereafter.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa     SPI_init()
 */
typedef struct SPI_Config_t
{
    /*! Pointer to a table of driver-specific implementations of SPI APIs */
    SPI_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver specific data object */
    void             *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void const       *hwAttrs;
} SPI_Config;

/*!
 *  @brief  Function to close a SPI peripheral specified by the SPI handle
 *
 *  @pre    SPI_open() has to be called first.
 *
 *  @param  handle A SPI handle returned from SPI_open()
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     SPI_open()
 */
extern void SPI_close(SPI_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a given
 *          SPI_Handle.
 *
 *  Commands for SPI_control can originate from SPI.h or from implementation
 *  specific SPI*.h (_SPICC26XX.h_, _SPITiva.h_, etc.. ) files.
 *  While commands from SPI.h are API portable across driver implementations,
 *  not all implementations may support all these commands.
 *  Conversely, commands from driver implementation specific SPI*.h files add
 *  unique driver capabilities but are not API portable across all SPI driver
 *  implementations.
 *
 *  Commands supported by SPI.h follow a SPI_CMD_\<cmd\> naming
 *  convention.<br>
 *  Commands supported by SPI*.h follow a SPI*_CMD_\<cmd\> naming
 *  convention.<br>
 *  Each control command defines @b arg differently. The types of @b arg are
 *  documented with each command.
 *
 *  See @ref SPI_CMD "SPI_control command codes" for command codes.
 *
 *  See @ref SPI_STATUS "SPI_control return status codes" for status codes.
 *
 *  @pre    SPI_open() has to be called first.
 *
 *  @param  handle      A SPI handle returned from SPI_open()
 *
 *  @param  cmd         SPI.h or SPI*.h commands.
 *
 *  @param  arg         An optional R/W (read/write) command argument
 *                      accompanied with cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     SPI_open()
 */
extern int32_t SPI_control(SPI_Handle handle, uint32_t cmd, void *arg);

/*!
 *  @brief  This function gets hardware statistics from SPI module.
 *
 *  @param  handle      A SPI_Handle
 *
 *  @param  ptrStats    A pointer to a SPI_Stats.
 *
 *  @return 0 if got statistics successfully; else SPI error code
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     SPI_open
 */
extern int32_t SPI_getStats(SPI_Handle handle, SPI_Stats *ptrStats);

/*!
 *  @brief  This function initializes the SPI module.
 *
 *  @pre    The SPI_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other SPI driver APIs. This function call does not modify any
 *          peripheral registers.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern void SPI_init(void);

/*!
 *  @brief  This function opens a given SPI peripheral.
 *
 *  @pre    SPI controller has been initialized using SPI_init()
 *
 *  @param  index         Logical peripheral number for the SPI indexed into
 *                        the SPI_config table
 *
 *  @param  params        Pointer to an parameter block, if NULL it will use
 *                        default values. All the fields in this structure are
 *                        RO (read-only).
 *
 *  @return A SPI_Handle on success or a NULL on an error or if it has been
 *          opened already.
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     SPI_init()
 *  @sa     SPI_close()
 */
extern SPI_Handle SPI_open(uint32_t index, SPI_Params *params);

/*!
 *  @brief  Function to initialize the SPI_Params struct to its defaults
 *
 *  @param  params      An pointer to SPI_Params structure for
 *                      initialization
 *
 *  Defaults values are:
 *      transferMode        = SPI_MODE_BLOCKING
 *      transferTimeout     = SPI_WAIT_FOREVER
 *      transferCallbackFxn = NULL
 *      mode                = SPI_SLAVE
 *      elemSize            = SPI_ELEMSIZE_16BIT
 *      frameFormat         = SPI_POL0_PHA0
 *      dmaEnable           = 1
 *      eccEnable           = 1
 *      pinMode             = SPI_PINMODE_4PIN_CS
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern void SPI_Params_init(SPI_Params *params);

/*!
 *  @brief  Function to service the SPI module's interrupt service routine
 *
 *  This function is not supported on all driver implementations.  Refer to
 *  implementation specific documentation for details.
 *
 *  @param  handle      A SPI_Handle
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern void SPI_serviceISR(SPI_Handle handle);

/*!
 *  @brief  Function to perform SPI transactions
 *
 *  If the SPI is in ::SPI_MASTER mode, it will immediately start the
 *  transaction. If the SPI is in ::SPI_SLAVE mode, it prepares itself for a
 *  transaction with a SPI master.
 *
 *  In ::SPI_MODE_BLOCKING, SPI_transfer will block task execution until the
 *  transaction has completed.
 *
 *  When Dma mode is disable, SPI driver is operated in polling mode, there is
 *  while(1) loop waiting for hardware to finish.
 *
 *  @param  handle      A SPI_Handle
 *
 *  @param  transaction A pointer to a SPI_Transaction. All of the fields within
 *                      transaction except SPI_Transaction.count and
 *                      SPI_Transaction.status are WO (write-only) unless
 *                      otherwise noted in the driver implementations. If a
 *                      transaction timeout has occured, SPI_Transaction.count
 *                      will contain the number of frames that were transferred.
 *
 *  @return true if started successfully; else false
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     SPI_open
 *  @sa     SPI_transferCancel
 */
extern bool SPI_transfer(SPI_Handle handle, SPI_Transaction *transaction);

/*!
 *  @brief  Function to cancel SPI transactions
 *
 *  In ::SPI_MODE_BLOCKING, SPI_transferCancel has no effect.
 *
 *  @param  handle      A SPI_Handle
 *
 *  \ingroup SPI_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     SPI_open
 *  @sa     SPI_transfer
 */
extern void SPI_transferCancel(SPI_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_SPI__include */
