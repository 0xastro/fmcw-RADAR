/**
 *   @file  I2CSlave.h
 *
 *   @brief
 *      This is the header file for the I2C slave driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the I2C slave driver.
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

/** \page I2CSlaveDriver
 *
 *  @file       I2CSlave.h
 *
 *  @brief      I2CSlave driver interface
 *
 *  The I2CSlave header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/i2c/I2CSlave.h>
 *  @endcode
 *
 *  The i2c/include/reg_i2c.h has the register layer definitons for the
 *  I2C Module.
 *
 *  # Operation #
 *  The I2CSlave driver operates as a slave on an I2C bus in
 *  I2CSLAVE_MODE_BLOCKING mode.
 *  In blocking mode, the task's execution is blocked during the I2CSlave
 *  read/write transfer. When the transfer has completed, code execution will
 *  resume.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the SYS/BIOS specific primitives to allow for thread-safe
 *  operation.
 *
 *  ## Opening the driver #
 *
 *  @code
 *  I2CSlave_Handle      handle;
 *  I2CSlave_Params      params;
 *
 *  I2CSlave_Params_init(&params);
 *  handle = I2CSlave_open(someI2CSlave_configIndexValue, &params);
 *  if (!handle) {
 *      System_printf("I2CSlave did not open");
 *  }
 *  @endcode
 *
 *  ## Transferring data #
 *  A I2CSlave transaction with a I2CSlave peripheral is started by calling
 *  I2CSlave_read() or I2CSlave_write().
 *  Each transfer is performed atomically with the I2CSlave peripheral.
 *
 *  @code
 *  ret = I2CSlave_read(i2cSlave, buffer, 5)
 *  if (!ret) {
 *      System_printf("Unsuccessful I2CSlave read");
 *  }
 *
 *  I2CSlave_write(i2cSlave, buffer, 3);
 *  if (!ret) {
 *      System_printf("Unsuccessful I2CSlave write");
 *  }

 *  @endcode
 *
 *  # Implementation #
 *
 *  This module serves as the main interface for TI-RTOS
 *  applications. Its purpose is to redirect the module's APIs to specific
 *  peripheral implementations which are specified using a pointer to a
 *  I2CSlave_FxnTable.
 *
 *  The I2CSlave driver interface module is joined (at link time) to a
 *  NULL-terminated array of I2CSlave_Config data structures named
 *  *I2CSlave_config*. *I2CSlave_config* is implemented in the application
 *  with each entry being an instance of a I2CSlave peripheral. Each entry in
 *  *I2CSlave_config* contains a:
 *  - (I2CSlave_FxnTable *) to a set of functions that implement an I2CSlave
 *  - (void *) data object that is associated with the I2CSlave_FxnTable
 *  - (void *) hardware attributes that are associated to the I2CSlave_FxnTable
 *
 *  ============================================================================
 */

/** @defgroup I2CSLAVE_DRIVER      I2C Slave Driver
 */

#ifndef ti_drivers_I2CSLAVE__include
#define ti_drivers_I2CSLAVE__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>

/**
@defgroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION     I2C Slave Driver External Functions
@ingroup I2CSLAVE_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the I2C Slave driver
*/

/**
@defgroup I2CSLAVE_DRIVER_ERROR_CODE            I2C Slave Driver Error code
@ingroup I2CSLAVE_DRIVER
*  Base error code for the I2C module is defined in the
*  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define I2CSLAVE_EINVAL                 (MMWAVE_ERRNO_I2C_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define I2CSLAVE_EINUSE                 (MMWAVE_ERRNO_I2C_BASE-2)

/**
 * @brief   Error Code: Operation is not implemented.
 */
#define I2CSLAVE_ENOTIMPL               (MMWAVE_ERRNO_I2C_BASE-3)

/** @}*/

/**
 *  @defgroup I2CSLAVE_CONTROL I2C Slave Control command and status codes
 *  These I2C macros are reservations for I2CSlave.h
 *  @{
 */

/**
 *  @defgroup I2CSLAVE_CMD Command Codes
 *  I2CSLAVE_CMD_* macros are general command codes for I2CSlave_control(). Not all I2C Slave
 *  driver implementations support these command codes.
 *  @{
 *  @ingroup I2CSLAVE_CONTROL
 */

/*!
 * @brief   Command code used by I2CSlave_control() to choose the addressing mode
 *
 * This command is available to choose the addressing mode in the I2C Slave Driver.
 * With this command code, @b arg is a pointer to an integer.
 * @b *arg contains the status of the address mode. 1 will set the mode to 10-bit addressing mode.
 * 0 will set the mode to 7-bit addressing mode.
 *
 * Note: When set to 1, I2C master driver doesnot support combined formats.
 */
#define I2C_CMD_ADDR_MODE           1

/** @}*/
/** @}*/

/*!
 *  @brief      A handle that is returned from a I2CSlave_open() call.
 */
typedef struct I2CSlave_Config_t    *I2CSlave_Handle;

/*!
 *  @brief  I2CSlave mode
 *
 *  This enum defines the state of the I2CSlave driver's state-machine. Do not
 *  modify.
 */
typedef enum I2CSlave_Mode_t {
    I2CSLAVE_IDLE_MODE = 0,     /* I2CSlave is not performing a transaction */
    I2CSLAVE_WRITE_MODE = 1,    /* I2CSlave is currently performing write */
    I2CSLAVE_READ_MODE = 2,     /* I2CSlave is currently performing read */
    I2CSLAVE_START_MODE = 3,    /* I2CSlave received a START from a master */
    I2CSLAVE_ERROR = 0xFF       /* I2CSlave error has occurred, exit gracefully */
} I2CSlave_Mode;

/*!
 *  @brief  I2CSlave transfer mode
 *
 *  I2CSLAVE_MODE_BLOCKING block task execution a I2CSlave transfer is in
 *  progress.
 */
typedef enum I2CSlave_TransferMode_t {
    I2CSLAVE_MODE_BLOCKING   /*!< I2CSlave read/write blocks execution*/
} I2CSlave_TransferMode;

/*!
 *  @brief  I2CSlave Parameters
 *
 *  I2CSlave parameters are used to with the I2CSlave_open() call. Default
 *  values for
 *  these parameters are set using I2CSlave_Params_init().
 *
 *  If I2CSlave_TransferMode is set to I2CSLAVE_MODE_BLOCKING then I2CSlave_read
 *  or I2CSlave_write function calls will block thread execution until the
 *  transaction has completed.
 *
 *  @sa     I2CSlave_Params_init()
 */
typedef struct I2CSlave_Params_t {
    /*!< Blocking or Callback mode */
    I2CSlave_TransferMode   transferMode;

    /*!< Address associated with current I2CSlave device */
    uint_fast16_t           slaveAddress;

    /*!< Custom argument used by driver implementation */
    uintptr_t               custom;
} I2CSlave_Params;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_close().
 */
typedef void        (*I2CSlave_CloseFxn)    (I2CSlave_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_control().
 */
typedef int32_t   (*I2CSlave_ControlFxn)  (I2CSlave_Handle handle,
                                        uint32_t cmd,
                                        void *arg);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_init().
 */
typedef void        (*I2CSlave_InitFxn)     (I2CSlave_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_open().
 */
typedef I2CSlave_Handle  (*I2CSlave_OpenFxn)     (I2CSlave_Handle handle,
                                        const I2CSlave_Params *params);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_WriteTransaction().
 */
typedef bool        (*I2CSlave_WriteFxn) (I2CSlave_Handle handle,
        const void *buffer, size_t size);


/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              I2CSlave_ReadFxn().
 */
typedef bool        (*I2CSlave_ReadFxn) (I2CSlave_Handle handle, void *buffer,
        size_t size);


/*!
 *  @brief      The definition of a I2CSlave function table that contains the
 *              required set of functions to control a specific I2CSlave
 *              driver implementation.
 */
typedef struct I2CSlave_FxnTable_t {
    /*! Function to close the specified peripheral */
    I2CSlave_CloseFxn           closeFxn;

    /*! Function to implementation specific control function */
    I2CSlave_ControlFxn         controlFxn;

    /*! Function to initialize the given data object */
    I2CSlave_InitFxn            initFxn;

    /*! Function to open the specified peripheral */
    I2CSlave_OpenFxn            openFxn;

    /*! Function to initiate a I2CSlave data read */
    I2CSlave_ReadFxn            readFxn;

    /*! Function to initiate a I2CSlave data write */
    I2CSlave_WriteFxn           writeFxn;
} I2CSlave_FxnTable;

/*!
 *  @brief  I2CSlave Global configuration
 *
 *  The I2CSlave_Config structure contains a set of pointers used to
 *  characterize the I2CSlave driver implementation.
 *
 *  This structure needs to be defined before calling I2CSlave_init() and it
 *  must not be changed thereafter.
 *
 *  @sa     I2CSlave_init()
 */
typedef struct I2CSlave_Config_t {
    /*! Pointer to a table of driver-specific implementations of I2CSlave APIs*/
    I2CSlave_FxnTable const *fxnTablePtr;

    /*! Pointer to a driver specific data object */
    void                *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void                *hwAttrs;
} I2CSlave_Config;


/*!
 *  @brief  Function to close a I2CSlave peripheral specified by the I2CSlave
 *  handle
 *  @pre    I2CSlave_open() had to be called first.
 *
 *  @param  handle  A I2CSlave_Handle returned from I2CSlave_open
 *
 *  @sa     I2CSlave_open()
 *
 *  \ingroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION
 */
extern void I2CSlave_close(I2CSlave_Handle handle);

/*!
 *  @brief  Function performs implementation specific features on a given
 *          I2CSlave_Handle.
 *
 *  @pre    I2CSlave_open() has to be called first.
 *
 *  @param  handle      A I2CSlave handle returned from I2CSlave_open()
 *
 *  @param  cmd         A command value defined by the driver specific
 *                      implementation
 *
 *  @param  arg         An optional R/W (read/write) argument that is
 *                      accompanied with cmd
 *
 *  @return Implementation specific return codes. Negative values indicate
 *          unsuccessful operations.
 *
 *  @sa     I2CSlave_open()
 *
 *  \ingroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION
 */
extern int32_t I2CSlave_control(I2CSlave_Handle handle, uint32_t cmd,
    void *arg);

/*!
 *  @brief  Function to initializes the I2CSlave module
 *
 *  @pre    The I2CSlave_config structure must exist and be persistent before
 *          this function can be called. This function must also be called
 *          before any other I2CSlave driver APIs. This function call does not
 *          modify any peripheral registers.
 *
 *  \ingroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION
 */
extern void I2CSlave_init(void);

/*!
 *  @brief  Function to initialize a given I2CSlave peripheral specified by the
 *          particular index value. The parameter specifies which mode the
 *          I2CSlave will operate.
 *
 *  @pre    I2CSlave controller has been initialized
 *
 *  @param  index         Logical peripheral number for the I2CSlave indexed
 *                        into the I2CSlave_config table
 *
 *  @param  params        Pointer to an parameter block, if NULL it will use
 *                        default values. All the fields in this structure are
 *                        RO (read-only).
 *
 *  @return A I2CSlave_Handle on success or a NULL on an error or if it has been
 *          opened already.
 *
 *  @sa     I2CSlave_init()
 *  @sa     I2CSlave_close()
 *
 *  \ingroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION
 */
extern I2CSlave_Handle I2CSlave_open(uint32_t index,
    I2CSlave_Params *params);

/*!
 *  @brief  Function to initialize the I2CSlave_Params struct to its defaults
 *
 *  @param  params      An pointer to I2CSlave_Params structure for
 *                      initialization
 *
 *  Defaults values are:
 *      transferMode = I2CSLAVE_MODE_BLOCKING
 *      transferCallbackFxn = NULL
 *
 *  \ingroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION
 */
extern void I2CSlave_Params_init(I2CSlave_Params *params);

/*!
 *  @brief  Function that handles the I2CSlave read for SYS/BIOS
 *
 *  This function will start a I2CSlave read and can only be called from a
 *  Task context when in I2CSLAVE_MODE_BLOCKING.
 *  The I2CSlave read procedure starts with evaluating how many bytes are to be
 *  readby the I2CSlave peripheral.
 *
 *  The data written by the I2CSlave is synchronized with the START and STOP
 *  from the master.
 *
 *  In I2CSLAVE_MODE_BLOCKING, I2CSlave read/write will block task execution until
 *  the transaction has completed.
 *
 *  @param  handle      A I2CSlave_Handle
 *
 *  @param  buffer      A RO (read-only) pointer to an empty buffer in which
 *                      received data should be written to.
 *
 *  @param  size        The number of bytes to be written into buffer
 *
 *  @return true on successful transfer
 *          false on an error
 *
 *  @sa     I2CSlave_open
 *
 *  \ingroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION
 */
extern bool I2CSlave_read(I2CSlave_Handle handle, void *buffer,
        size_t size);
/*!
 *  @brief  Function that handles the I2CSlave write for SYS/BIOS
 *
 *  This function will start a I2CSlave write and can only be called from a
 *  Task context when in I2CSLAVE_MODE_BLOCKING.
 *  The I2CSlave transfer procedure starts with evaluating how many bytes are
 *  to be written.
 *
 *  The data written by the I2CSlave is synchronized with the START and STOP
 *  from the master. If slave does not have as many bytes requested by master
 *  it writes 0xFF. I2CSlave keeps sending 0xFF till master sends a STOP.
 *
 *  In I2CSLAVE_MODE_BLOCKING, I2CSlave read/write will block task execution
 *  until the transaction has completed.
 *
 *  @param  handle      A I2CSlave_Handle
 *
 *  @param  buffer      A WO (write-only) pointer to buffer containing data to
 *                      be written to the master.
 *
 *  @param  size        The number of bytes in buffer that should be written
 *                      onto the master.
 *
 *  @return true on successful write
 *          false on an error
 *
 *  @sa     I2CSlave_open
 *
 *  \ingroup I2CSLAVE_DRIVER_EXTERNAL_FUNCTION
 */
extern bool I2CSlave_write(I2CSlave_Handle handle, const void *buffer,
        size_t size);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_I2CSLAVE__include */
