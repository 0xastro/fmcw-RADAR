/**
 *   @file  UART.h
 *
 *   @brief
 *      This is the header file for the UART driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the UART driver.
 *
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
/** @mainpage UART Driver
 *
 *  The UART header file should be included in an application as follows:
 *  @code
    #include <ti/drivers/uart/UART.h>
    @endcode
 *
 *  The uart/include/reg_sci.h has the register layer definitons for the
 *  UART SCI Module.
 *
 *  ## Link Requirements #
 *  The UART driver interface module is joined (at link time) to a
 *  NULL-terminated array of UART_Config data structures named *UART_config*.
 *  *UART_config* is implemented in the application with each entry being an
 *  instance of a UART peripheral. Each entry in *UART_config* contains a:
 *  - (UART_FxnTable *) to a set of functions that implement a UART peripheral
 *  - (void *) data object that is associated with the UART_FxnTable
 *  - (void *) hardware attributes that are associated with the UART_FxnTable
 *
 *  There is a platform specific UART file present in the *ti/drivers/uart/platform*
 *  directory. This file is built as a part of the UART Library for the specific
 *  platform.
 *
 *  ## Opening the driver #
 *
 *  @code
    UART_Handle      handle;
    UART_Params      params;

    UART_Params_init(&params);
    params.baudRate  = someNewBaudRate;
    params.writeDataMode = UART_DATA_BINARY;
    params.readDataMode = UART_DATA_BINARY;
    params.readReturnMode = UART_RETURN_FULL;
    params.readEcho = UART_ECHO_OFF;
    handle = UART_open(someUART_configIndexValue, &params);
    if (!handle) {
        // Error: Unable to open the UART
    }
    @endcode
 *
 *  ## Writing data #
 *
 *  @code
    const unsigned char hello[] = "Hello World\n";

    ret = UART_write(handle, hello, sizeof(hello));

    @endcode
 *
 *  ## Reading data #
 *
 *  @code
    unsigned char rxBuffer[20];

    ret = UART_read(handle, rxBuffer, sizeof(rxBuffer));

    @endcode
 *
 *  ## DMA Interface #
 *  The UART driver can be hooked up with the DMA (MSS)/EDMA (DSS). In this case
 *  case the read & write operations are done using the DMA/EDMA blocks. This would
 *  make the UART driver require the DMA or EDMA drivers. On memory constrained
 *  system this might not be acceptable; in which case the driver also provides a
 *  default no-DMA layer. In order to use this please modify the hardware attributes
 *  'gUartSciHwCfg' defined in the platform specific file and replace the DMA functions
 *  with the no-DMA variants.
 *
 *  @sa
 *      UartSci_noDMAIsDMAEnabled
 *      UartSci_noDMAOpen
 *      UartSci_noDMAClose
 *      UartSci_noDMAInitiateRxDMA
 *      UartSci_noDMAInitiateTxDMA
 *
 *   Please rebuild the driver after this modification.
 *
 *   While using the DMA interface in the read mode please be aware that the function
 *   UART_read/UART_readPolling always operate in UART_RETURN_FULL mode.
 */

/** @defgroup UART_DRIVER      UART Driver
 */
#ifndef ti_drivers_UART__include
#define ti_drivers_UART__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>
#ifdef SUBSYS_MSS
#include <ti/drivers/dma/dma.h>
#else
#include <ti/drivers/edma/edma.h>
#endif

/**
@defgroup UART_DRIVER_EXTERNAL_FUNCTION     UART Driver External Functions
@ingroup UART_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the UART driver
*/

/**
@defgroup UART_DRIVER_ERROR_CODE            UART Driver Error code
@ingroup UART_DRIVER
*  Base error code for the UART module is defined in the
*  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define UART_EINVAL                 (MMWAVE_ERRNO_UART_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define UART_EINUSE                 (MMWAVE_ERRNO_UART_BASE-2)

/**
 * @brief   Error Code: Operation is not implemented.
 */
#define UART_ENOTIMPL               (MMWAVE_ERRNO_UART_BASE-3)

/** @}*/

/**
 *  @defgroup UART_CMD Command Codes
 *  UART_CMD_* macros are general command codes for UART_control(). Not all UART
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup UART_CONTROL
 */

/*!
 * @brief   Command code used by UART_control() to place the UART in loopback mode
 *
 * This command is available to place the UART Driver in loopback mode. This is
 * a debug only feature. With this command code, @b arg is a pointer to an
 * integer. @b *arg contains the status of the loopback. 1 will enable the loopback
 * and 0 will disable it
 */
#define UART_CMD_LOOPBACK           0U

/*!
 * @brief   Command code used by UART_control() to get the UART Instance statistics
 *
 * This command is available to get the statistics associated with the UART instance.
 * This is a debug only feature. With this command code, @b arg is a pointer to the
 * UART_Stats structure.
 */
#define UART_CMD_GET_STATS          1U

/** @}*/

/*!
 *  @brief    Wait forever define
 */
#define UART_WAIT_FOREVER           0xFFFFFFFFU

/*!
 *  @brief      A handle that is returned from a UART_open() call.
 */
typedef struct UART_Config_t     *UART_Handle;

/*!
 *  @brief      UART return mode settings
 *
 *  This enumeration defines the return modes for UART_read() and
 *  UART_readPolling(). This mode only functions when in UART_DATA_TEXT mode.
 *
 *  UART_RETURN_FULL unblocks when the read buffer has been filled.
 *  UART_RETURN_NEWLINE unblocks or performs a callback whenever a newline
 *  character has been received.
 *
 *  UART operation | UART_RETURN_FULL | UART_RETURN_NEWLINE |
 *  -------------- | ---------------- | ------------------- |
 *  UART_read      | Returns when buffer is full | Returns when buffer is full or newline was read |
 *  UART_write     | Sends data as is | Sends data with an additional newline at the end |
 *
 *  @pre        UART driver must be used in UART_DATA_TEXT mode.
 */
typedef enum UART_ReturnMode_e
{
    /*! Unblock/callback when buffer is full. */
    UART_RETURN_FULL,

    /*! Unblock/callback when newline character is received. */
    UART_RETURN_NEWLINE
} UART_ReturnMode;

/*!
 *  @brief      UART data mode settings
 *
 *  This enumeration defines the data mode for read and write.
 *  In UART_DATA_TEXT mode the driver will examine the UART_ReturnMode value.
 *
 */
typedef enum UART_DataMode_e
{
    UART_DATA_BINARY = 0, /*!< Data is not processed */
    UART_DATA_TEXT = 1    /*!< Data is processed according to above */
} UART_DataMode;

/*!
 *  @brief      UART echo settings
 *
 *  This enumeration defines if the driver will echo data when uses in
 *  UART_DATA_TEXT mode. This only applies to data received by the UART.
 *
 *  UART_ECHO_ON will echo back characters it received while in UART_DATA_TEXT
 *  mode.
 *  UART_ECHO_OFF will not echo back characters it received in UART_DATA_TEXT
 *  mode.
 *
 *  @pre        UART driver must be used in UART_DATA_TEXT mode.
 */
typedef enum UART_Echo_e
{
    UART_ECHO_OFF = 0,  /*!< Data is not echoed */
    UART_ECHO_ON = 1    /*!< Data is echoed */
} UART_Echo;

/*!
 *  @brief    UART data length settings
 *
 *  This enumeration defines the UART data lengths.
 */
typedef enum UART_LEN_e
{
    UART_LEN_5 = 0,        /*!< Data length is 5 bits */
    UART_LEN_6 = 1,        /*!< Data length is 6 bits */
    UART_LEN_7 = 2,        /*!< Data length is 7 bits */
    UART_LEN_8 = 3         /*!< Data length is 8 bits */
} UART_LEN;

/*!
 *  @brief    UART stop bit settings
 *
 *  This enumeration defines the UART stop bits.
 */
typedef enum UART_STOP_e
{
    UART_STOP_ONE = 0,  /*!< One stop bit */
    UART_STOP_TWO = 1   /*!< Two stop bits */
} UART_STOP;

/*!
 *  @brief    UART parity type settings
 *
 *  This enumeration defines the UART parity types.
 */
typedef enum UART_PAR_e
{
    UART_PAR_NONE = 0,  /*!< No parity */
    UART_PAR_EVEN = 1,  /*!< Parity bit is even */
    UART_PAR_ODD  = 2,  /*!< Parity bit is odd */
    UART_PAR_ZERO = 3,  /*!< Parity bit is always zero */
    UART_PAR_ONE  = 4   /*!< Parity bit is always one */
} UART_PAR;

/*!
 *  @brief    UART Parameters
 *
 *  UART parameters are used to with the UART_open() call. Default values for
 *  these parameters are set using UART_Params_init().
 *
 *  @sa       UART_Params_init()
 */
typedef struct UART_Params_t
{
    uint32_t          readTimeout;      /*!< Timeout for read semaphore */
    uint32_t          writeTimeout;     /*!< Timeout for write semaphore */
    UART_ReturnMode   readReturnMode;   /*!< Receive return mode */
    UART_DataMode     readDataMode;     /*!< Type of data being read */
    UART_DataMode     writeDataMode;    /*!< Type of data being written */
    UART_Echo         readEcho;         /*!< Echo received data back */
    uint32_t          baudRate;         /*!< Baud rate for UART */
    UART_LEN          dataLength;       /*!< Data length for UART */
    UART_STOP         stopBits;         /*!< Stop bits for UART */
    UART_PAR          parityType;       /*!< Parity bit type for UART */
    uint32_t          clockFrequency;   /*!< CPU Clock Frequency */
    uint32_t          isPinMuxDone;     /*!< Set the flag to indicate if PIN MUX has been completed
                                             The driver does *not* verify if the operation was done
                                             correctly. The application is responsible for ensuring
                                             that the PIN MUX was done correctly.  */
#ifdef SUBSYS_MSS
    DMA_Handle        dmaHandle;        /*!< Handle to the DMA Instance. If this is set to NULL the
                                             UART driver instance will not use the DMA lines to send
                                             and receive data. This is valid only on MSS */
    uint32_t          txDMAChannel;     /*!< This is the transmit DMA channel number which is used
                                             if the UART Instance is configured to use the DMA services */
    uint32_t          rxDMAChannel;     /*!< This is the receive DMA channel number which is used
                                             if the UART Instance is configured to use the DMA services.
                                             *NOTE* Certain UART instances are Tx Only. For such UART
                                             instances this field is not used. Please refer to the
                                             platform specific file for the duplexity field. */
#else
    EDMA_Handle       edmaHandle;       /*!< Handle to the EDMA Instance. If this is set to NULL the
                                             UART driver instance will not use the EDMA events to
                                             send and receive data. This is valid only on DSS.
                                             Applications need to ensure that the cache contents of the
                                             data being transmitted are written back before using the
                                             write API. */
    uint8_t           paramSetId;       /*!< This is the identifier for a PaRAM set which will be used
                                             to setup the transfers. */
#endif
} UART_Params;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_CloseFxn().
 */
typedef void        (*UART_CloseFxn)          (UART_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_ControlFxn().
 */
typedef int32_t     (*UART_ControlFxn)        (UART_Handle handle,
                                               uint32_t cmd,
                                               void *arg);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_InitFxn().
 */
typedef void        (*UART_InitFxn)           (UART_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_OpenFxn().
 */
typedef UART_Handle (*UART_OpenFxn)           (UART_Handle handle,
                                               UART_Params *params);
/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_ReadFxn().
 */
typedef int32_t     (*UART_ReadFxn)           (UART_Handle handle, uint8_t *buffer,
                                               uint32_t size);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_ReadPollingFxn().
 */
typedef int32_t     (*UART_ReadPollingFxn)    (UART_Handle handle, uint8_t *buffer,
                                               uint32_t size);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_ReadCancelFxn().
 */
typedef void    (*UART_ReadCancelFxn)     (UART_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_WriteFxn().
 */
typedef int32_t (*UART_WriteFxn)          (UART_Handle handle,
                                           uint8_t *buffer,
                                           uint32_t size);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_WritePollingFxn().
 */
typedef int32_t (*UART_WritePollingFxn)   (UART_Handle handle,
                                           uint8_t *buffer,
                                           uint32_t size);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              UART_WriteCancelFxn().
 */
typedef void    (*UART_WriteCancelFxn)    (UART_Handle handle);

/*!
 *  @brief      The definition of a UART function table that contains the
 *              required set of functions to control a specific UART driver
 *              implementation.
 */
typedef struct UART_FxnTable_t
{
    /*! Function to close the specified peripheral */
    UART_CloseFxn           closeFxn;

    /*! Function to implementation specific control function */
    UART_ControlFxn         controlFxn;

    /*! Function to initialize the given data object */
    UART_InitFxn            initFxn;

    /*! Function to open the specified peripheral */
    UART_OpenFxn            openFxn;

    /*! Function to read from the specified peripheral */
    UART_ReadFxn            readFxn;

    /*! Function to read via polling from the specified peripheral */
    UART_ReadPollingFxn     readPollingFxn;

    /*! Function to cancel a read from the specified peripheral */
    UART_ReadCancelFxn      readCancelFxn;

    /*! Function to write from the specified peripheral */
    UART_WriteFxn           writeFxn;

    /*! Function to write via polling from the specified peripheral */
    UART_WritePollingFxn    writePollingFxn;

    /*! Function to cancel a write from the specified peripheral */
    UART_WriteCancelFxn     writeCancelFxn;
} UART_FxnTable;

/*!
 *  @brief  UART Global configuration
 *
 *  The UART_Config structure contains a set of pointers used to characterize
 *  the UART driver implementation.
 *
 *  This structure needs to be defined before calling UART_init() and it must
 *  not be changed thereafter.
  *
 *  @sa     UART_init()
 */
typedef struct UART_Config_t
{
    /*! Pointer to a table of driver-specific implementations of UART APIs */
    UART_FxnTable const    *fxnTablePtr;

    /*! Pointer to a driver specific data object */
    void                   *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void                   *hwAttrs;
} UART_Config;

/*!
 *  @brief  UART Statistics
 *
 *  This structure holds the statistics which are maintained by the UART
 *  driver instance and can be used for debugging purpose
  *
 *  @sa     UART_control()
 */
typedef struct UART_Stats_t
{
    /**
     * @brief   Number of receive interrupts
     */
    uint32_t    numRxInterrupts;

    /**
     * @brief   Number of transmit interrupts
     */
    uint32_t    numTxInterrupts;

    /**
     * @brief   Number of receive overrun interrupts
     */
    uint32_t    numRxOverrunInterrupts;

    /**
     * @brief   Number of dummy UART reads: Data was received on the console
     * but the application did not provide a buffer.
     */
    uint32_t    numDummyRead;

    /**
     * @brief   Total Number of interrupts
     */
    uint32_t    totalInterrupts;

    /**
     * @brief   Number of Receive DMA Interrupts: This is only if the UART instance
     * is configured to operate in DMA mode.
     */
    uint32_t    numRxDMAInterrupts;

    /**
     * @brief   Number of Transmit DMA Interrupts: This is only if the UART instance
     * is configured to operate in DMA mode.
     */
    uint32_t    numTxDMAInterrupts;

    /**
     * @brief   Number of Framing Errors:
     */
    uint32_t    numFramingErrors;

    /**
     * @brief   Number of Parity Errors:
     */
    uint32_t    numParityError;
}UART_Stats;

/*!
 *  @brief  Function to close a UART peripheral specified by the UART handle
 *
 *  @pre    UART_open() has been called.
 *
 *  @param  handle      A UART_Handle returned from UART_open()
 *
 *  @sa     UART_open()
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern void UART_close(UART_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a given
 *          UART_Handle.
 *
 *  Commands for UART_control can originate from UART.h or from implementation
 *  specific UART*.h (_UARTCC26XX.h_, _UARTTiva.h_, etc.. ) files.
 *  While commands from UART.h are API portable across driver implementations,
 *  not all implementations may support all these commands.
 *  Conversely, commands from driver implementation specific UART*.h files add
 *  unique driver capabilities but are not API portable across all UART driver
 *  implementations.
 *
 *  Commands supported by UART.h follow a UART_CMD_\<cmd\> naming
 *  convention.<br>
 *  Commands supported by UART*.h follow a UART*_CMD_\<cmd\> naming
 *  convention.<br>
 *  Each control command defines @b arg differently. The types of @b arg are
 *  documented with each command.
 *
 *  See @ref UART_CMD "UART_control command codes" for command codes.
 *
 *  See @ref UART_DRIVER_ERROR_CODE "UART_control return status codes" for status codes.
 *
 *  @pre    UART_open() has to be called.
 *
 *  @param  handle      A UART handle returned from UART_open()
 *
 *  @param  cmd         UART.h or UART*.h commands.
 *
 *  @param  arg         An optional R/W (read/write) command argument
 *                      accompanied with cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  @sa     UART_open()
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern int32_t UART_control(UART_Handle handle, uint32_t cmd, void *arg);

/*!
 *  @brief  Function to initialize the UART module
 *
 *  @pre    The UART_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other UART driver APIs.
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern void UART_init(void);

/*!
 *  @brief  Function to initialize a given UART peripheral
 *
 *  Function to initialize a given UART peripheral specified by the
 *  particular index value.
 *
 *  @pre    UART_init() has been called
 *
 *  @param  index         Logical peripheral number for the UART indexed into
 *                        the UART_config table
 *
 *  @param  params        Pointer to a parameter block. If NULL, default
 *                        parameter values will be used. All the fields in
 *                        this structure are RO (read-only).
 *
 *  @return A UART_Handle upon success. NULL if an error occurs, or if the
 *          indexed UART peripheral is already opened.
 *
 *  @sa     UART_init()
 *  @sa     UART_close()
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern UART_Handle UART_open(uint32_t index, UART_Params *params);

/*!
 *  @brief  Function to initialize the UART_Params struct to its defaults
 *
 *  @param  params      An pointer to UART_Params structure for
 *                      initialization
 *
 *  Defaults values are:
 *      readTimeout = UART_WAIT_FOREVER;
 *      writeTimeout = UART_WAIT_FOREVER;
 *      readReturnMode = UART_RETURN_NEWLINE;
 *      readDataMode = UART_DATA_TEXT;
 *      writeDataMode = UART_DATA_TEXT;
 *      readEcho = UART_ECHO_ON;
 *      baudRate = 115200;
 *      dataLength = UART_LEN_8;
 *      stopBits = UART_STOP_ONE;
 *      parityType = UART_PAR_NONE;
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern void UART_Params_init(UART_Params *params);

/*!
 *  @brief  Function that writes data to a UART with interrupts enabled. Usage
 *          of this API is mutually exclusive with usage of
 *          UART_writePolling(). In other words, for an opened UART peripheral,
 *          either UART_write() or UART_writePolling() may be used, but not
 *          both.
 *
 *  In UART_MODE_BLOCKING, UART_write() will block task execution until all
 *  the data in buffer has been written.
 *
 *  @sa UART_writePolling()
 *
 *  @param  handle      A UART_Handle
 *
 *  @param  buffer      A WO (write-only) pointer to buffer containing data to
 *                      be written to the UART.
 *
 *  @param  size        The number of bytes in the buffer that should be written
 *                      to the UART.
 *
 *  @return Returns the number of bytes that have been written to the UART.
 *          If an error occurs, one of the UART Error codes is returned.
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern int32_t UART_write(UART_Handle handle, uint8_t *buffer, uint32_t size);

/*!
 *  @brief  Function that writes data to a UART, polling the peripheral to
 *          wait until new data can be written. Usage of this API is mutually
 *          exclusive with usage of UART_write().
 *
 *  This function initiates an operation to write data to a UART controller.
 *
 *  UART_writePolling() will not return until all the data was written to the
 *  UART (or to its FIFO if applicable).
 *
 *  @sa UART_write()
 *
 *  @param  handle      A UART_Handle
 *
 *  @param  buffer      A pointer to the buffer containing the data to
 *                      be written to the UART.
 *
 *  @param  size        The number of bytes in the buffer that should be written
 *                      to the UART.
 *
 *  @return Returns the number of bytes that have been written to the UART.
 *          If an error occurs, one of the UART Error codes is returned.
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern int32_t UART_writePolling(UART_Handle handle, uint8_t *buffer,
                                 uint32_t size);

/*!
 *  @brief  Function that cancels a UART_write() function call.
 *
 *  This function cancels a UART_write() operation to a UART peripheral.
 *
 *  @param  handle      A UART_Handle
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern void UART_writeCancel(UART_Handle handle);

/*!
 *  @brief  Function that reads data from a UART with interrupt enabled. This
 *          API must be used mutually exclusive with UART_readPolling().
 *
 *  This function initiates an operation to read data from a UART controller.
 *
 *  In UART_MODE_BLOCKING, UART_read() will block task execution until all
 *  the data in buffer has been read.
 *
 *  @sa UART_readPolling()
 *
 *  @param  handle      A UART_Handle
 *
 *  @param  buffer      A RO (read-only) pointer to an empty buffer in which
 *                      received data should be written to.
 *
 *  @param  size        The number of bytes to be written into buffer
 *
 *  @return Returns the number of bytes that have been read from the UART,
 *          one of the UART Error codes on an error.
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern int32_t UART_read(UART_Handle handle, uint8_t *buffer, uint32_t size);

/*!
 *  @brief  Function that reads data from a UART without interrupts. This API
 *          must be used mutually exclusive with UART_read().
 *
 *  This function initiates an operation to read data from a UART peripheral.
 *
 *  UART_readPolling will not return until size data was read to the UART.
 *
 *  @sa UART_read()
 *
 *  @param  handle      A UART_Handle
 *
 *  @param  buffer      A RO (read-only) pointer to an empty buffer in which
 *                      received data should be written to.
 *
 *  @param  size        The number of bytes to be written into buffer
 *
 *  @return Returns the number of bytes that have been read from the UART,
 *          one of the UART Error codes on an error.
 *
 *  \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern int32_t UART_readPolling(UART_Handle handle, uint8_t *buffer, uint32_t size);

/*!
 *  @brief  Function that cancels a UART_read() function call.
 *  This function cancels a UART_read() operation for a UART peripheral.
 *
 *  @param  handle      A UART_Handle
 *
 * \ingroup UART_DRIVER_EXTERNAL_FUNCTION
 */
extern void UART_readCancel(UART_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_UART__include */

