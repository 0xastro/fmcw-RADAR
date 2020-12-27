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

/** ===========================================================
 *  @file       ADCBuf.h
 *
 *  @brief      ADCBuf driver interface
 *
 */

 /** @mainpage ADCBuf Driver
 *
 *  The ADCBuf header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/ADCBuf.h>
 *  @endcode
 *
 *  # Operation #
 *  The ADCBuf driver in TI-RTOS samples an analogue waveform at a specified
 *  frequency. The resulting samples are transferred to a buffer provided by
 *  the application. The driver can either take n samples once, or continuously
 *  sample by double-buffering and providing a callback to process each finished
 *  buffer.
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the SYS/BIOS specific primitives to allow for thread-safe
 *  operation.
 *
 *  The platform specific ADCBUF file present in the ti/drivers/adcbuf/platform
 *  directory. This file is built as a part of the ADCBUF Library for the specific
 *  platform.
 *
 *  ## Opening the driver #
 *
 *  @code
 *      ADCBuf_Handle adcBufHandle;
 *      ADCBuf_Params adcBufParams;
 *
 *      ADCBuf_Params_init(&adcBufParams);
 *      adcBufHandle = ADCBuf_open(Board_ADCBuf0, &adcBufParams);
 *  @endcode
 *
 *  ## Making a conversion #
 *  In this context, a conversion refers to taking multiple ADC samples and
 *  transferring them to an application-provided buffer.
 *  To start a conversion, the application must configure an ADCBuf_Conversion struct
 *  and call ADCBuf_convert(). In blocking mode, ADCBuf_convert() will return
 *  when the conversion is finished and the desired number of samples have been made.
 *  In callback mode, ADCBuf_convert() will return immediately and the application will
 *  get a callback when the conversion is done.
 *
 *  @code
 *      ADCBuf_Conversion blockingConversion;
 *
 *      blockingConversion.arg = NULL;
 *      blockingConversion.adcChannel = Board_ADCCHANNEL_A1;
 *      blockingConversion.sampleBuffer = sampleBufferOnePtr;
 *      blockingConversion.sampleBufferTwo = NULL;
 *      blockingConversion.samplesRequestedCount = ADCBUFFERSIZE;
 *
 *      if (!ADCBuf_convert(adcBuf, &continuousConversion, 1)) {
 *          // handle error
 *      }
 *  @endcode
 *
 *  ## Canceling a conversion #
 *  ADCBuf_convertCancel() is used to cancel an ADCBuf conversion when the driver is
 *  used in ADCBuf_RETURN_MODE_CALLBACK.
 *
 *  Calling this API while no conversion is in progress has no effect. If a
 *  conversion is in progress, it is canceled and the provided callback function
 *  is called.
 *
 *  In ADCBuf_RECURRENCE_MODE_CONTINUOUS, this function must be called to stop the
 *  conversion. The driver will continue providing callbacks with fresh samples
 *  until thie ADCBuf_convertCancel() function is called. The callback function is not
 *  called after ADCBuf_convertCancel() while in ADCBuf_RECURRENCE_MODE_CONTINUOUS.
 *
 *  # Implementation #
 *
 *  This module serves as the main interface for TI-RTOS applications. Its
 *  purpose is to redirect the module's APIs to specific peripheral
 *  implementations which are specified using a pointer to an ADCBuf_FxnTable.
 *
 *  The ADCBuf driver interface module is joined (at link time) to a
 *  NULL-terminated array of ADCBuf_Config data structures named *ADCBuf_config*.
 *  *ADCBuf_config* is implemented in the application with each entry being an
 *  instance of an ADCBuf peripheral. Each entry in *ADCBuf_config* contains a:
 *  - (ADCBuf_FxnTable *) to a set of functions that implement an ADCBuf peripheral
 *  - (void *) data object that is associated with the ADCBuf_FxnTable
 *  - (void *) hardware attributes that are associated to the ADCBuf_FxnTable
 *
 *  # Instrumentation #
 *
 *  The ADCBuf driver interface produces log statements if instrumentation is
 *  enabled.
 *
 *  Diagnostics Mask | Log details                      |
 *  ---------------- | ---------------------------------|
 *  Diags_USER1      | basic operations performed       |
 *  Diags_USER2      | detailed operations performed    |
 *
 *  ============================================================================
 */

#ifndef ti_drivers_adcbuf__include
#define ti_drivers_adcbuf__include

/**
@defgroup ADCBUF_DRIVER_EXTERNAL_FUNCTION            ADCBUF Driver External Functions
@ingroup ADCBUF_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE     ADCBUF Driver External Data Structures
@ingroup ADCBUF_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup ADCBUF_DRIVER_ERROR_CODE                   ADCBUF Driver Error Codes
@ingroup ADCBUF_DRIVER
@brief
*   The section has a list of all the error codes which are generated by the CRC Driver
*   module
*/
/**
@defgroup ADCBUF_DRIVER_INTERNAL_FUNCTION            ADCBUF Driver Internal Functions
@ingroup ADCBUF_DRIVER
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup ADCBUF_DRIVER_INTERNAL_DATA_STRUCTURE      ADCBUF Driver Internal Data Structures
@ingroup ADCBUF_DRIVER
@brief
*   The section has a list of all internal data structures which are used internally
*   by the ADCBUF module.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>
#include <ti/common/sys_common.h>

/*!
 * Common ADCBuf_control command code reservation offset.
 * ADC driver implementations should offset command codes with ADCBuf_CMD_RESERVED
 * growing positively
 *
 * Example implementation specific command codes:
 * @code
 * #define ADCXYZ_COMMAND0         ADCBuf_CMD_RESERVED + 0
 * #define ADCXYZ_COMMAND1         ADCBuf_CMD_RESERVED + 1
 * @endcode
 */
#define ADCBuf_CMD_RESERVED             32

/*!
 * Common ADCBuf_control status code reservation offset.
 * ADC driver implementations should offset status codes with
 * ADCBuf_STATUS_RESERVED growing negatively.
 *
 * Example implementation specific status codes:
 * @code
 * #define ADCXYZ_STATUS_ERROR0    ADCBuf_STATUS_RESERVED - 0
 * #define ADCXYZ_STATUS_ERROR1    ADCBuf_STATUS_RESERVED - 1
 * #define ADCXYZ_STATUS_ERROR2    ADCBuf_STATUS_RESERVED - 2
 * @endcode
 */
#define ADCBuf_STATUS_RESERVED          (-32)

/*!
 * \brief  Success status code returned by:
 * ADCBuf_control()
 *
 * Functions return ADCBuf_STATUS_SUCCESS if the call was executed
 * successfully.
 */
#define ADCBuf_STATUS_SUCCESS           0

/*!
 * \brief   Generic error status code returned by ADCBuf_control().
 *
 * ADCBuf_control() returns ADCBuf_STATUS_ERROR if the control code was not executed
 * successfully.
 *
 *  \ingroup ADCBUF_DRIVER_ERROR_CODE
 *
 */
#define ADCBuf_STATUS_ERROR             (MMWAVE_ERRNO_ADCBUF_BASE - 1)

/*!
 * \brief   An error status code returned by ADCBuf_control() for undefined
 * command codes.
 *
 * ADCBuf_control() returns ADCBuf_STATUS_UNDEFINEDCMD if the control code is not
 * recognized by the driver implementation.
 *
 *  \ingroup ADCBUF_DRIVER_ERROR_CODE
 *
 */
#define ADCBuf_STATUS_UNDEFINEDCMD      (MMWAVE_ERRNO_ADCBUF_BASE - 2)

/*!
 * \brief   An error status code returned by ADCBuf_control() for feature not implemented.
 *
 * ADCBuf_control() returns ADCBuf_STATUS_NOT_IMPLEMENTED if the control command
 * was not supported.
 *
 *  \ingroup ADCBUF_DRIVER_ERROR_CODE
 *
 */
#define ADCBuf_STATUS_NOT_IMPLEMENTED   (MMWAVE_ERRNO_ADCBUF_BASE - 3)

/*!
 * \brief   An error status code returned by ADCBuf_control() for invalid input parameters.
 *
 * ADCBuf_control() returns ADCBuf_STATUS_INVALID_PARAMS if the control code gets
 * invalid parameters.
 *
 *  \ingroup ADCBUF_DRIVER_ERROR_CODE
 *
 */
#define ADCBuf_STATUS_INVALID_PARAMS  (MMWAVE_ERRNO_ADCBUF_BASE - 4)

/*!
 * \brief   An error status code returned by ADCBuf_control() for invalid
 * command codes.
 *
 * ADCBuf_control() returns ADCBuf_STATUS_INVALID_CMD if the control code is not
 * recognized by the driver implementation.
 *
 *  \ingroup ADCBUF_DRIVER_ERROR_CODE
 *
 */
#define ADCBuf_STATUS_INVALID_CMD      (MMWAVE_ERRNO_ADCBUF_BASE - 5)


/*!
 *  @brief      A handle that is returned from an ADCBuf_open() call.
 */
typedef struct ADCBuf_Config_t              *ADCBuf_Handle;

/*!
 *  @brief
 *  An ::ADCBuf_Conversion data structure is used with ADCBuf_convert(). It indicates
 *  which channel to perform the ADC conversion on, how many conversions to make, and where to put them.
 *  The arg variable is an user-definable argument which gets passed to the
 *  ::ADCBuf_Callback when the ADC driver is in ADCBuf_RETURN_MODE_CALLBACK.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBuf_Conversion_t
{
    uint16_t            samplesRequestedCount;      /*!< Number of samples to convert and return */
    void                *sampleBuffer;              /*!< Buffer the results of the conversions are stored in */
    void                *sampleBufferTwo;           /*!< A second buffer that is filled in ADCBuf_RECURRENCE_MODE_CONTINUOUS mode while
                                                        the first buffer is processed by the application. The value is not used in
                                                        ADCBuf_RECURRENCE_MODE_ONE_SHOT mode. */
    void                *arg;                       /*!< Argument to be passed to the callback function in ADCBuf_RETURN_MODE_CALLBACK */
    uint32_t            adcChannel;                 /*!< Channel to perform the ADC conversion on. Mapping of channel to pin or internal signal is device specific. */
} ADCBuf_Conversion;

/*!
 *  @brief      The definition of a callback function used by the ADC driver
 *              when used in ADCBuf_RETURN_MODE_CALLBACK. It is called in a HWI or SWI context depending on the device specific implementation.
 */
typedef void (*ADCBuf_Callback)            (ADCBuf_Handle handle,
                                            ADCBuf_Conversion *conversion,
                                            void *activeADCBuffer);

/*!
 *  @brief      The definition of a callback function used by the ADCBUF interrupt handler
 */
typedef void (*ADCBuf_IntCallbackFxn)            (ADCBuf_Handle handle);

/**
 * @brief
 *  ADCBUF Source selection
 *
 * @details
 *  The structure is used to define ADCBUF driver commands.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum ADCBufSource_e
{
    /**
     * @brief  Select DFE as source.
     */
    ADCBUF_SOURCE_DFE        = 0,

    /**
     * @brief  Select HIL as source.
     */
    ADCBUF_SOURCE_HIL
}ADCBufSource;

/**
 * @brief
 *  ADC Buffer data format Parameters
 *
 * @details
 *  The structure is used to define ADC Buffer data format.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBuf_dataFormat_t
{
    /* ADC out Format
         0 - Complex Data mode
         1 - Real data mode
      */
    uint8_t     adcOutFormat;

    /* Sample interleave - IQswap
         0 - I is stored in LSB, Q is stored in MSB
         1 - Q is stored in LSB, I is stored in MSB
      */
    uint8_t     sampleInterleave;

    /* channel interleave
         0 - interleaved
         1 - non-interleaved
      */
    uint8_t     channelInterleave;
}ADCBuf_dataFormat;

/**
 * @brief
 *  ADC Buffer RX channel configuration Parameters
 *
 * @details
 *  The structure is used to define ADC Buffer RX Channel configuration.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBuf_RxChanConf_t
{
    /* RX channel id - 0~3 */
    uint8_t   channel;

    /* Address offset for the channel in Ping/Pong buffer
      Used only in non-interleaved mode, it must be 16 bytes aligned.
     */
    uint16_t  offset;
}ADCBuf_RxChanConf;

/**
 * @brief
 *  ADC Buffer test pattern configuration Parameters
 *
 * @details
 *  The structure is used to define ADC Buffer test pattern configuration.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBuf_rxTestPatternConf_t
{
    /* I sample offset */
    uint16_t rxIOffset;

    /* I sample incremental value */
    uint16_t rxIInc;

    /* Q sample offset */
    uint16_t rxQOffset;

    /* Q sample incremental value */
    uint16_t rxQInc;
}ADCBuf_rxTestPatternConf;

/**
 * @brief
 *  ADC Buffer test pattern configuration Parameters
 *
 * @details
 *  The structure is used to define ADC Buffer test pattern configuration.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBuf_TestPatternConf_t
{
    /* Test pattern configuration for 4 channels */
    ADCBuf_rxTestPatternConf rxConfig[SYS_COMMON_NUM_RX_CHANNEL];

    /* Periodicity of the pattern */
    uint16_t               period;

    /* Sample count to store in ADC buffer */
    uint16_t               numSamples;
}ADCBuf_TestPatternConf;

/**
 * @brief
 *  Chirp Quality(CQ) configuration Parameters
 *
 * @details
 *  The structure is used to define Chirp Quality configuration.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct ADCBuf_CQConf_t
{
    /*! 0x0 or 0x1:Raw16, 0x2:Raw12, 0x3:Raw14 */
    uint8_t   cqDataWidth;

    /*! Set in case of 3 channel mode  */
    uint8_t   cq96BitPackEn;

    /*! CQ0 Address offset : 16bytes aligned address for Storing CQ0 */
    uint16_t  cq0AddrOffset;

    /*! CQ1 Address offset : 16 bytes aligned address for Storing CQ1 */
    uint16_t  cq1AddrOffset;

    /*! CQ2 Address offset : 16 bytes aligned address for Storing CQ2 */
    uint16_t  cq2AddrOffset;
}ADCBuf_CQConf;

/*!
 *  @brief ADC Parameters
 *
 *  ADC Parameters are used to with the ADCBuf_open() call. Default values for
 *  these parameters are set using ADCBuf_Params_init().
 *
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa     ADCBuf_Params_init()
 */
typedef struct ADCBuf_Params_t
{
    /* ADC buffer source, DFE or HIL */
    ADCBufSource        source;

    /* Continuous mode selection */
    uint8_t             continousMode;

#ifdef SOC_XWR14XX
    /* Chirp Threshold for non-continous operation */
    uint8_t             chirpThreshold;
#endif

#if defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX)
    /* Ping buffer Chirp Threshold for non-continous operation */
    uint8_t             chirpThresholdPing;

    /* Pong buffer Chirp Threshold for non-continous operation */
    uint8_t             chirpThresholdPong;
#endif

    /* Custom configuration, Pointer to a device specific extension of the ADCBuf_Params */
    void                *custom;
} ADCBuf_Params;

/**
 * @brief
 *  ADCBUF Command
 *
 * @details
 *  The structure is used to define ADCBUF driver commands.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum ADCBufMMWave_CMD_e
{
    /**
     * @brief   set ADCBUF source, it can be DFE or VIN.
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_SRC    = 0,

    /**
     * @brief   Set number of Chirps to be stored in each Ping and Pong buffer.
     *          It should be programmed one less the actual number needed .
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD,

    /**
     * @brief   Set number of Chirps to be stored in each Ping buffer.
     *          It should be programmed one less the actual number needed .
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD,

    /**
     * @brief   Set number of Chirps to be stored in each  Pong buffer.
     *          It should be programmed one less the actual number needed .
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD,

    /**
     * @brief   Enables/Disables Continuous mode for ADCBUF.
     *             1 to enable continuous mode.
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_SET_CONTINUOUS_MODE,

    /**
     * @brief   Starts Continuous mode for ADCBUF.
     *          Number of Samples to store in Ping/Pong buffer needs to be provided.
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_START_CONTINUOUS_MODE,

    /**
     * @brief   Stops Continuous mode for ADCBUF.
     *          The size of the argument size for this command is 0 bytes
     */
    ADCBufMMWave_CMD_STOP_CONTINUOUS_MODE,

    /**
     * @brief   Configures ADCBUF data format.
     *          The size of the argument size for this command is size of \ref ADCBuf_dataFormat
     */
    ADCBufMMWave_CMD_CONF_DATA_FORMAT,

    /**
     * @brief   Enable RX channels and configures the address offset in ADCBUF for the channel.
     *          The size of the argument size for this command is size of \ref ADCBuf_RxChanConf
     */
    ADCBufMMWave_CMD_CHANNEL_ENABLE,

    /**
     * @brief   Disable RX channels specified with RX channel bitmask
     *          The size of the argument size for this command is 4 bytes
     */
    ADCBufMMWave_CMD_CHANNEL_DISABLE,

    /**
     * @brief   Test pattern configuration.
     *          The size of the argument size for this command is size of \ref ADCBuf_TestPatternConf
     */
    ADCBufMMWave_CMD_CONF_TEST_PATTERN,

    /**
     * @brief   Starts Test pattern generation. Reboot is required when switching from Test pattern mode
     *          to normal operation mode.
     *          The size of the argument size for this command is 0 bytes
     */
    ADCBufMMWave_CMD_START_TEST_PATTERN,

    /**
     * @brief   Stops Test pattern generation.
     *          The size of the argument size for this command is 0 bytes
     */
    ADCBufMMWave_CMD_STOP_TEST_PATTERN,

    /**
     * @brief   Chirp Quality configuration.
     *          The size of the argument size for this command is size of \ref ADCBuf_CQConf
     */
    ADCBufMMWave_CMD_CONF_CQ,

    /**
     * @brief  Last command.
     */
    ADCBufMMWave_CMD_LAST
}ADCBufMMWave_CMD;

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              ADCBuf_close().
 */
typedef void (*ADCBuf_CloseFxn)                     (ADCBuf_Handle handle);


/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              ADCBuf_open().
 */
typedef ADCBuf_Handle (*ADCBuf_OpenFxn)             (ADCBuf_Handle handle,
                                                     const ADCBuf_Params *params);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              ADCBuf_control().
 */
typedef int_fast16_t (*ADCBuf_ControlFxn)           (ADCBuf_Handle handle,
                                                     uint_fast8_t cmd,
                                                     void *arg);
/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              ADCBuf_init().
 */
typedef void (*ADCBuf_InitFxn)                      (ADCBuf_Handle handle);

/*!
 *  @brief      A function pointer to a driver specific implementation of
 *              ADCBuf_getChanBufAddr().
 */
typedef uint32_t (*ADCBuf_GetChanBufAddrFxn)         (ADCBuf_Handle handle,
                                                     uint8_t channel,
                                                     int32_t *errCode);

/*!
 *  @brief      The definition of an ADCBuf function table that contains the
 *              required set of functions to control a specific ADC driver
 *              implementation.
 */
typedef struct ADCBuf_FxnTable_t
{
    /*! Function to close the specified peripheral */
    ADCBuf_CloseFxn            closeFxn;
    /*! Function to driver implementation specific control function */
    ADCBuf_ControlFxn          controlFxn;
    /*! Function to initialize the given data object */
    ADCBuf_InitFxn             initFxn;
    /*! Function to open the specified peripheral */
    ADCBuf_OpenFxn             openFxn;
    /*! Function to get ADC buffer address for a given channel */
    ADCBuf_GetChanBufAddrFxn   getChanBufAddr;
} ADCBuf_FxnTable;

/*!
 *  @brief ADCBuf Global configuration
 *
 *  The ADCBuf_Config structure contains a set of pointers used to characterise
 *  the ADC driver implementation.
 *
 *  This structure needs to be defined before calling ADCBuf_init() and it must
 *  not be changed thereafter.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 *  @sa     ADCBuf_init()
 */
typedef struct ADCBuf_Config_t
{
    /*! Pointer to a table of driver-specific implementations of ADC APIs */
    const ADCBuf_FxnTable   *fxnTablePtr;

    /*! Pointer to a driver specific data object */
    void                    *object;

    /*! Pointer to a driver specific hardware attributes structure */
    void const              *hwAttrs;
} ADCBuf_Config;


/**
 * @brief
 *  ADCBUF CQ monitoring type
 *
 * @details
 *  The structure is used to define CQ monitoring types.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef enum ADCBufMMWave_CQType_e
{
    /**
     * @brief   CQ type for CQ0.
     */
    ADCBufMMWave_CQType_CQ0,

    /**
     * @brief   CQ type for CQ1.
     */
    ADCBufMMWave_CQType_CQ1,

    /**
     * @brief   CQ type for CQ2.
     */
    ADCBufMMWave_CQType_CQ2,

    /**
     * @brief   Maximum number of CQ which can be supported.
     */
    ADCBufMMWave_CQType_MAX_CQ
}ADCBufMMWave_CQType;

/*!
 *  @brief  Function to close an ADC peripheral specified by the ADC handle
 *
 *  @pre    ADCBuf_open() has to be called first.
 *
 *  @pre    In ADCBuf_RECURRENCE_MODE_CONTINUOUS, the application must call ADCBuf_convertCancel() first.
 *
 *  @param  handle      An ADCBuf handle returned from ADCBuf_open()
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     ADCBuf_open()
 */
extern void ADCBuf_close(ADCBuf_Handle handle);


/*!
 *  @brief  Function performs implementation specific features on a given
 *          ADCBuf_Handle.
 *
 *  @pre    ADCBuf_open() has to be called first.
 *
 *  @param  handle      An ADCBuf handle returned from ADCBuf_open()
 *
 *  @param  cmd         A command value defined by the driver specific
 *                      implementation
 *
 *  @param  arg         A pointer to an optional R/W (read/write) argument that
 *                      is accompanied with cmd. arg should be 4 bytes aligned.
 *
 *  @return An ADCBuf_Status describing an error or success state. Negative values
 *          indicates an error.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     ADCBuf_open()
 */
extern int_fast16_t ADCBuf_control(ADCBuf_Handle handle, uint_fast8_t cmd, void * arg);

/*!
 *  @brief  This function gets the physical address of ADCBuf for a given receive channel
 *
 *  @pre    ADCBuf_open() has to be called first.
 *
 *  @param  handle      An ADCBuf handle returned from ADCBuf_open()
 *
 *  @param  channel     Receive channel number
 *
 *  @param  errCode     Pointer to an error code populated by the driver
 *
 *  @return An ADCBuf physical address for the given channel on success or 0 on an error
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern uint32_t ADCBuf_getChanBufAddr(ADCBuf_Handle handle, uint8_t channel, int32_t *errCode);

/*!
 *  @brief  This function gets the physical address of chirp info(CQ) buffer for a given CQ type
 *
 *  @pre    ADCBuf_open() has to be called first.
 *
 *  @param  handle      An ADCBuf handle returned from ADCBuf_open()
 *
 *  @param  cqType      Type of CQ that request the address
 *
 *  @param  errCode     Pointer to an error code populated by the driver
 *
 *  @return An CQ buffer physical address for the given cqType on success or 0 on an error
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern uint32_t ADCBUF_MMWave_getCQBufAddr
(
    ADCBuf_Handle handle,
    ADCBufMMWave_CQType cqType,
    int32_t *errCode
);

/*!
 *  @brief  This function initializes the ADC module. This function must be called
 *          before any other functions are called.
 *
 *  @pre    The ADCBuf_Config structure must exist and be persistent before this
 *          function can be called.
 *          This function call does not modify any peripheral registers.
 *          Function should only be called once.
 */
extern void ADCBuf_init(void);

/*!
 *  @brief  This function sets all fields of a specified ADCBuf_Params structure to their
 *          default values.
 *
 *  @param  params      A pointer to ADCBuf_Params structure for initialization
 *
 *  Default values are:
 *                      returnMode         = ADCBuf_RETURN_MODE_BLOCKING,
 *                      blockingTimeout    = 25000,
 *                      callbackFxn        = NULL,
 *                      recurrenceMode     = ADCBuf_RECURRENCE_MODE_ONE_SHOT,
 *                      samplingFrequency  = 10000,
 *                      custom             = NULL
 *
 *  ADCBuf_Params::blockingTimeout should be set large enough to allow for the desired number of samples to be
 *  collected with the specified frequency.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern void ADCBuf_Params_init(ADCBuf_Params *params);

/*!
 *  @brief  This function opens a given ADCBuf peripheral.
 *
 *  @param  index       Logical peripheral number for the ADCBuf indexed into
 *                      the ADCBuf_config table
 *
 *  @param  params      Pointer to an parameter block, if NULL it will use
 *                      default values.
 *
 *  @return An ADCBuf_Handle on success or a NULL on an error or if it has been
 *          opened already. If NULL is returned further ADC API calls will
 *          result in undefined behaviour.
 *
 *  \ingroup ADCBUF_DRIVER_EXTERNAL_FUNCTION
 *
 *  @sa     ADCBuf_close()
 */
extern ADCBuf_Handle ADCBuf_open(uint_fast8_t index, ADCBuf_Params *params);

#ifdef __cplusplus
}
#endif
#endif /* ti_drivers_adcbuf__include */
