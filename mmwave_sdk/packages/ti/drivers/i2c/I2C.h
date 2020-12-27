/**
 *   @file  I2C.h
 *
 *   @brief
 *      This is the header file for the I2C master driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the I2C master driver.
 *
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
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

/** @mainpage I2C Driver
 *
 * For I2C Master Driver refer to
 *  \subpage I2CMasterDriver
 *
 *  For I2C Slave Driver refer to
 *  \subpage <ti/drivers/i2c/I2CSlave.h>
 */

/** \page I2CMasterDriver
 *
 *  The I2C header file should be included in an application as follows:
 *  @code
    #include <ti/drivers/i2c/I2C.h>
    @endcode
 *
 *  The i2c/include/reg_i2c.h has the register layer definitons for the
 *  I2C Module.
 *
 *  # Operation #
 *  The I2C driver operates as a master on a single-master I2C bus in
 *  I2C_MODE_BLOCKING mode.
 *  In blocking mode, the task's execution is blocked during the I2C
 *  transaction. When the transfer has completed, code execution will resume.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the SYS/BIOS specific primitives to allow for thread-safe
 *  operation.
 *
 *  ## Opening the driver #
 *
 *  @code
 *  I2C_Handle      handle;
 *  I2C_Params      params;
 *  I2C_Transaction i2cTransaction;
 *
 *  I2C_Params_init(&params);
 *  handle = I2C_open(someI2C_configIndexValue, &params);
 *  if (!handle) {
 *      System_printf("I2C did not open");
 *  }
 *  @endcode
 *
 *  ## Transferring data #
 *  A I2C transaction with a I2C peripheral is started by calling
 *  I2C_transfer(). The details of the I2C transaction is specified with a
 *  I2C_Transaction data structure. This structure allows for any of the three
 *  types of transactions: Write, Read, or Write/Read. Each transfer is
 *  performed atomically with the I2C slave peripheral.
 *
 *  @code
 *  I2C_Transaction i2cTransaction;
 *
 *  i2cTransaction.writeBuf = someWriteBuffer;
 *  i2cTransaction.writeCount = numOfBytesToWrite;
 *
 *  i2cTransaction.readBuf = someReadBuffer;
 *  i2cTransaction.readCount = numOfBytesToRead;
 *
 *  i2cTransaction.slaveAddress = someI2CSlaveAddress;
 *
 *  ret = I2C_transfer(handle, &i2cTransaction);
 *  if (!ret) {
 *      System_printf("Unsuccessful I2C transfer");
 *  }
 *  @endcode
 *
 *  # Implementation #
 *
 *  This module serves as the main interface for TI-RTOS
 *  applications. Its purpose is to redirect the module's APIs to specific
 *  peripheral implementations which are specified using a pointer to a
 *  I2C_FxnTable.
 *
 *  The I2C driver interface module is joined (at link time) to a
 *  NULL-terminated array of I2C_Config data structures named *I2C_config*.
 *  *I2C_config* is implemented in the application with each entry being an
 *  instance of a I2C peripheral. Each entry in *I2C_config* contains a:
 *  - (I2C_FxnTable *) to a set of functions that implement a I2C peripheral
 *  - (void *) data object that is associated with the I2C_FxnTable
 *  - (void *) hardware attributes that are associated to the I2C_FxnTable
 *
 *  A platform specific I2C file present in the *ti/drivers/i2c/platform*
 *  directory. This file is built as a part of the I2C Library for the specific
 *  platform.
 *
 */

/** @defgroup I2C_DRIVER      I2C Master Driver
 */
#ifndef ti_drivers_I2C__include
#define ti_drivers_I2C__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>

/**
@defgroup I2C_DRIVER_EXTERNAL_FUNCTION     I2C Driver External Functions
@ingroup I2C_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the I2C driver
*/

/**
@defgroup I2C_DRIVER_ERROR_CODE            I2C Driver Error code
@ingroup I2C_DRIVER
*  Base error code for the I2C module is defined in the
*  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define I2C_EINVAL                 (MMWAVE_ERRNO_I2C_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define I2C_EINUSE                 (MMWAVE_ERRNO_I2C_BASE-2)

/**
 * @brief   Error Code: Operation is not implemented.
 */
#define I2C_ENOTIMPL               (MMWAVE_ERRNO_I2C_BASE-3)

/** @}*/

/**
 *  @defgroup I2C_CONTROL I2C Control command and status codes
 *  These I2C macros are reservations for I2C.h
 *  @{
 */


/**
 *  @defgroup I2C_CMD Command Codes
 *  I2C_CMD_* macros are general command codes for I2C_control(). Not all I2C
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup I2C_CONTROL
 */

/*!
 * @brief   Command code used by I2C_control() to place I2C in loopback mode
 *
 * This command is available to place the I2C Driver in loopback mode. This is
 * a debug only feature. With this command code, @b arg is a pointer to an
 * integer. @b *arg contains the status of the loopback. 1 will enable the loopback
 * and 0 will disable it
 */
#define I2C_CMD_LOOPBACK           0

/*!
 * @brief   Command code used by I2C_control() to choose the addressing mode
 *
 * This command is available to choose the addressing mode in the I2C Driver.
 * With this command code, @b arg is a pointer to an integer.
 * @b *arg contains the status of the address mode. 1 will set the mode to 10-bit addressing mode.
 * 0 will set the mode to 7-bit addressing mode.
 *
 * Note: When set to 1, I2C master driver doesnot support combined formats.
 */
#define I2C_CMD_ADDR_MODE           1

/*!
 * @brief   Command code used by I2C_control() to configure I2C own address
 *
 * This command is available to configure I2C own address.
 * With this command code, @b arg is a pointer to an integer.
 *
 * Note: The addressing mode must be set before configuring I2C own address.
 */

#define I2C_CMD_I2COAR             2
/** @}*/
/** @}*/

/*!
 *  @brief      A handle that is returned from a I2C_open() call.
 */
typedef struct I2C_Config_t     *I2C_Handle;

/*!
 *  @brief  I2C transaction
 *
 *  This structure defines the nature of the I2C transaction. This structure
 *  specifies the buffer and buffer's size that is to be written to or read from
 *  the I2C slave peripheral.
 */
typedef struct I2C_Transaction_t {
    void        *writeBuf;    /*!< Buffer containing data to be written */
    size_t      writeCount;   /*!< Number of bytes to be written to the slave */

    void        *readBuf;     /*!< Buffer to which data is to be read into */
    size_t      readCount;    /*!< Number of bytes to be read from the slave */

    uint32_t    slaveAddress; /*!< Address of the I2C slave device */
} I2C_Transaction;

/*!
 *  @brief  I2C transfer mode
 *
 *  I2C_MODE_BLOCKING blocks task execution while an I2C transfer is in progress
 */
typedef enum I2C_TransferMode_t {
    I2C_MODE_BLOCKING   /*!< I2C_transfer blocks execution*/
} I2C_TransferMode;

/*!
 *  @brief  I2C bitRate
 *
 *  Specify one of the standardized I2C bus bit rates for I2C communications.
 *  The default is I2C_100kHz.
 */
typedef enum I2C_BitRate_t {
    I2C_100kHz = 0,
    I2C_400kHz = 1
} I2C_BitRate;

/*!
 *  @brief  I2C Parameters
 *
 *  I2C parameters are used with the I2C_open() call. Default values for
 *  these parameters are set using I2C_Params_init().
 *
 *  If I2C_TransferMode is set to I2C_MODE_BLOCKING then I2C_transfer function
 *  calls will block thread execution until the transaction has completed.
 *
 *  I2C_BitRate specifies the I2C bus rate used for I2C communications.
 *
 *  @sa     I2C_Params_init()
 */
typedef struct I2C_Params_t {
    I2C_TransferMode    transferMode; /*!< Blocking */
    I2C_BitRate         bitRate; /*!< I2C bus bit rate */
    uintptr_t           custom;  /*!< Custom argument used by driver
                                      implementation */
} I2C_Params;

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_cancel().
 */
typedef void        (*I2C_CancelFxn)    (I2C_Handle handle);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_close().
 */
typedef void        (*I2C_CloseFxn)    (I2C_Handle handle);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_control().
 */
typedef int32_t     (*I2C_ControlFxn)  (I2C_Handle handle,
                                        uint32_t cmd,
                                        void *arg);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_init().
 */
typedef void        (*I2C_InitFxn)     (I2C_Handle handle);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_open().
 */
typedef I2C_Handle  (*I2C_OpenFxn)     (I2C_Handle handle,
                                        I2C_Params *params);

/*!
 *  @brief      A function pointer to a driver-specific implementation of
 *              I2C_transfer().
 */
typedef bool        (*I2C_TransferFxn) (I2C_Handle handle,
                                        I2C_Transaction *transaction);

/*!
 *  @brief      The definition of an I2C function table that contains the
 *              required set of functions to control a specific I2C driver
 *              implementation.
 */
typedef struct I2C_FxnTable_t {
    /*! Cancel all I2C data transfers */
    I2C_CancelFxn        cancelFxn;

    /*! Close the specified peripheral */
    I2C_CloseFxn        closeFxn;

    /*! Implementation-specific control function */
    I2C_ControlFxn      controlFxn;

    /*! Initialize the given data object */
    I2C_InitFxn         initFxn;

    /*! Open the specified peripheral */
    I2C_OpenFxn         openFxn;

    /*! Initiate an I2C data transfer */
    I2C_TransferFxn     transferFxn;
} I2C_FxnTable;

/*!
 *  @brief  I2C global configuration
 *
 *  The I2C_Config structure contains a set of pointers used to characterize
 *  the I2C driver implementation.
 *
 *  This structure needs to be defined before calling I2C_init() and it must
 *  not be changed thereafter.
 *
 *  @sa     I2C_init()
 */
typedef struct I2C_Config_t {
    /*! Pointer to a table of driver-specific implementations of I2C APIs */
    I2C_FxnTable const  *fxnTablePtr;

    /*! Pointer to a driver-specific data object */
    void                *object;

    /*! Pointer to a driver-specific hardware attributes structure */
    void                *hwAttrs;
} I2C_Config;


/*!
 *  @brief  Cancel all I2C transfers
 *
 *  @pre    I2C_Transfer() has been called.
 *
 *  @param  handle  An I2C_Handle returned from I2C_open
 *
 *  @sa     I2C_transfer()
 *
 *  \ingroup I2C_DRIVER_EXTERNAL_FUNCTION
 */
extern void I2C_cancel(I2C_Handle handle);

/*!
 *  @brief  Close an I2C peripheral specified by an I2C handle
 *
 *  @pre    I2C_open() has been called.
 *
 *  @param  handle  A I2C_Handle returned from I2C_open
 *
 *  @sa     I2C_open()
 *
 *  \ingroup I2C_DRIVER_EXTERNAL_FUNCTION
 */
extern void I2C_close(I2C_Handle handle);

/*!
 *  @brief  Perform implementation specific features on a given
 *          I2C_Handle.
 *
 *  Commands for I2C_control can originate from I2C.h or from implementation
 *  specific I2C*.h (_I2CCC26XX.h_, _I2CTiva.h_, etc.. ) files.
 *  While commands from I2C.h are API portable across driver implementations,
 *  not all implementations may support all these commands.
 *  Conversely, commands from driver implementation specific I2C*.h files add
 *  unique driver capabilities but are not API portable across all I2C driver
 *  implementations.
 *
 *  Commands supported by I2C.h follow a I2C_CMD_\<cmd\> naming
 *  convention.<br>
 *  Commands supported by I2C*.h follow a I2C*_CMD_\<cmd\> naming
 *  convention.<br>
 *  Each control command defines @b arg differently. The types of @b arg are
 *  documented with each command.
 *
 *  See @ref I2C_CMD "I2C_control command codes" for command codes.
 *
 *  See @ref I2C_DRIVER_ERROR_CODE "I2C_control return status codes" for status codes.
 *
 *  @pre    I2C_open() has to be called first.
 *
 *  @param  handle      A I2C handle returned from I2C_open()
 *
 *  @param  cmd         I2C.h or I2C*.h command.
 *
 *  @param  arg         An optional R/W (read/write) command argument
 *                      accompanied with cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  @sa     I2C_open()
 *
 *  \ingroup I2C_DRIVER_EXTERNAL_FUNCTION
 */
extern int32_t I2C_control(I2C_Handle handle, uint32_t cmd, void *arg);

/*!
 *  @brief  Initializes the I2C module
 *
 *  @pre    The I2C_config structure must exist and be persistent before this
 *          function can be called. This function must also be called before
 *          any other I2C driver APIs. This function call does not modify any
 *          peripheral registers.
 *
 *  \ingroup I2C_DRIVER_EXTERNAL_FUNCTION
 */
extern void I2C_init(void);

/*!
 *  @brief  Initialize a given I2C peripheral specified by the
 *          particular index value. The parameter specifies which mode the I2C
 *          will operate.
 *
 *  @pre    I2C controller has been initialized
 *
 *  @param  index         Logical peripheral number for the I2C indexed into
 *                        the I2C_config table
 *
 *  @param  params        Pointer to a parameter block. Default values will be
 *                        used if NULL is specified. All the fields in this
 *                        structure are are considered RO (read-only).
 *
 *  @return A I2C_Handle on success or a NULL on an error or if it has been
 *          opened already.
 *
 *  @sa     I2C_init()
 *  @sa     I2C_close()
 *
 *  \ingroup I2C_DRIVER_EXTERNAL_FUNCTION
 */
extern I2C_Handle I2C_open(uint32_t index, I2C_Params *params);

/*!
 *  @brief  Initialize an I2C_Params struct to its defaults
 *
 *  @param  params      A pointer to I2C_Params structure for
 *                      initialization
 *
 *  Defaults values are:
 *      transferMode = I2C_MODE_BLOCKING
 *      transferCallbackFxn = NULL
 *      bitRate = I2C_100kHz
 *
 *  \ingroup I2C_DRIVER_EXTERNAL_FUNCTION
 */
extern void I2C_Params_init(I2C_Params *params);

/*!
 *  @brief  Perform an I2C transaction with an I2C slave peripheral.
 *
 *  This function will start an I2C transfer and can only be called from a Task
 *  context when in I2C_MODE_BLOCKING.
 *  The I2C transfer procedure starts with evaluating how many bytes are to be
 *  written and how many are to be read from the I2C peripheral. Any data to be
 *  written will always be sent before any data is read.
 *
 *  The data written to the peripheral is preceded with the peripheral's 7-bit
 *  I2C slave address (with the Write bit set).
 *  After all the data has been transmitted, the driver will evaluate if any
 *  data needs to be read from the device.
 *  If so, a Re-START bit is sent, along with the same 7-bit I2C slave address
 *  (with the Read bit). Else, the transfer is concluded with a STOP bit.
 *  After the specified number of bytes have been read by the I2C, the transfer
 *  is ended with a NACK and STOP bit.
 *
 *  In I2C_MODE_BLOCKING, the I2C_transfer will block task execution until the
 *  transaction completes.
 *
 *  @param  handle      A I2C_Handle
 *
 *  @param  transaction A pointer to an I2C_Transaction. All of the fields
 *                      within transaction should be considered write only,
 *                      unless otherwise noted in the driver implementation.
 *
 *  @return In I2C_MODE_BLOCKING: true for a successful transfer; false for an
 *          error (for example, an I2C bus fault (NACK)).
 *
 *  @sa     I2C_open
 *
 *  \ingroup I2C_DRIVER_EXTERNAL_FUNCTION
 */
extern bool I2C_transfer(I2C_Handle handle, I2C_Transaction *transaction);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_I2C__include */
