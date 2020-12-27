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
/** ============================================================================
 *  @file       mailbox.h
 *
 *  @brief      Mailbox driver interface
 *
 */

 /** @mainpage Mailbox Driver
 *
 *  The Mailbox driver simplifies reading and writing to the Mailbox
 *  peripherals on the board with multiple modes of operation and performance.
 *  These include blocking, non-blocking, callback. 
 *  Throughout this document the term <B>local endpoint</B> refers to the subsystem that instantiates the mailbox driver. 
 *  The term <B>remote endpoint</B> refers to the subsystem that communicates to the local endpoint through the mailbox instance.\n
 *  The application initializes the Mailbox driver by calling Mailbox_init() and it is then ready to
 *  open a Mailbox instance to a remote endpoint by calling Mailbox_open(). Once the mailbox instance is open, 
 *  read and write operations can be performed.\n
 *
 *  The Mailbox header file should be included in an application as follows:
 *  @code
    #include <ti/drivers/mailbox/mailbox.h>
    @endcode
 *
 *  The mailbox/include/reg_mailbox.h has the register layer definitions for the
 *  Mailbox Module.
 *
 *  ## Initializing the driver #
 *  The Mailbox Driver needs to be initialized once per master (MSS or DSS). This is
 *  done using the #Mailbox_init. None of the Mailbox APIs can be used before invoking
 *  this API.
 *
 * ## Opening the driver #
 * Once the Mailbox Driver has been initialized, the Mailbox Driver instance can be opened
 * using the #Mailbox_open. \n
 * For <B>XWR14xx:</B> \n
 * Between MSS<->BSS only one instance of the driver can be opened.\n 
 * Note that the mailbox driver is instantiated in the MSS only. In the BSS, the BSS firmware handles the mailbox operations.\n
 * For <B>XWR16xx/XWR18xx/XWR68xx:</B> \n
 * Between MSS<->BSS only one instance of the driver can be opened.\n
 * Note that the mailbox driver is instantiated in the MSS only. In the BSS, the BSS firmware handles the mailbox operations.\n
 * Between DSS<->BSS only one instance of the driver can be opened.\n
 * Note that the mailbox driver is instantiated in the DSS only. In the BSS, the BSS firmware handles the mailbox operations.\n
 * Between MSS<->DSS multiple instances of the driver can be opened (each instance controls one virtual channel).\n
 * Note that the mailbox driver is instantiated both in the MSS and DSS.\n
 *
 *
 *  <B>Example 1:\n </B>  
 *  The example below applies for <B>XWR14xx</B> and <B>XWR16xx/XWR18xx/XWR68xx</B> SoCs.\n
 *  Following is pseudo code for opening the Mailbox driver for MSS to talk to BSS.\n
 *  In this example the mailbox write mode is set to be polling and mailbox read
 *  mode is set to be callback.
 *
 *  @code
 *   
     
    Mbox_Handle  handle;
    int32_t         errCode;
    Mailbox_Config  cfg;
    
    Mailbox_init(MAILBOX_TYPE_MSS);
    
    if(Mailbox_Config_init(&cfg) < 0)
    {
        Error: Unable to initialize configuration
    }
    
    cfg.writeMode    = MAILBOX_MODE_POLLING;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = myApp_mboxCallbackFxn;
    
    handle = Mailbox_open(MAILBOX_TYPE_BSS, &cfg, &errCode);
    
    if((handle == NULL) || (errCode != 0))
    {
        Error: Unable to open mailbox between MSS and BSS.
    }
     
 *  @endcode
 *
 *
 *  <B>Example 2:\n </B>  
 *  The example below applies for <B>XWR16xx/XWR18xx/XWR68xx</B> SoC only.\n
 *  Following is pseudo code for opening the Mailbox driver for MSS to talk to DSS.\n
 *  In this example the mailbox write mode is set to be polling and mailbox read
 *  mode is set to be callback. Also, in this example, MSS and DSS applications are using
 *  multi-channels to communicate and the specific channels used in this case have ID 
 *  MAILBOX_CH_ID_0 and MAILBOX_CH_ID_1.\n
 *  Note that both endpoints need to use the same ID so that the channels are connected correctly.\n
 *  Note that all mailbox channels are bi-directional. In this example, 2 bi-directional channels
 *  are open to show the configuration of multiple mailbox channels.
 * 
 *  The following code runs on MSS:
 *
 *  @code
 *   
     
    Mbox_Handle  handle[2];
    int32_t         errCode;
    Mailbox_Config  cfg;
    
    Mailbox_init(MAILBOX_TYPE_MSS);
    
    if(Mailbox_Config_init(&cfg) < 0)
    {
        Error: Unable to initialize configuration
    }
    
    cfg.writeMode    = MAILBOX_MODE_POLLING;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = myApp_mboxCallbackFxn_MSS_ch0;
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_0;
    
    handle[0] = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    
    if((handle == NULL) || (errCode != 0))
    {
        Error: Unable to open mailbox channel 0 between MSS and DSS.
    }
    cfg.writeMode    = MAILBOX_MODE_POLLING;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = myApp_mboxCallbackFxn_MSS_ch1;
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_1;
    
    handle[1] = Mailbox_open(MAILBOX_TYPE_DSS, &cfg, &errCode);
    
    if((handle == NULL) || (errCode != 0))
    {
        Error: Unable to open mailbox channel 1 between MSS and DSS.
    }
     
 *  @endcode
 *
 *  The following code runs on DSS:
 *
 *  @code
 *   
     
    Mbox_Handle  handle[2];
    int32_t         errCode;
    Mailbox_Config  cfg;
    
    Mailbox_init(MAILBOX_TYPE_DSS);
    
    if(Mailbox_Config_init(&cfg) < 0)
    {
        Error: Unable to initialize configuration
    }
    
    cfg.writeMode    = MAILBOX_MODE_POLLING;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = myApp_mboxCallbackFxn_DSS_ch0;
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_0;
    
    handle[0] = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    
    if((handle == NULL) || (errCode != 0))
    {
        Error: Unable to open mailbox channel 0 between DSS and MSS.
    }

    cfg.writeMode    = MAILBOX_MODE_POLLING;
    cfg.readMode     = MAILBOX_MODE_CALLBACK;
    cfg.readCallback = myApp_mboxCallbackFxn_DSS_ch1;
    cfg.chType       = MAILBOX_CHTYPE_MULTI;
    cfg.chId         = MAILBOX_CH_ID_1;
    
    handle[1] = Mailbox_open(MAILBOX_TYPE_MSS, &cfg, &errCode);
    
    if((handle == NULL) || (errCode != 0))
    {
        Error: Unable to open mailbox channel 1 between DSS and MSS.
    }
    
 *  @endcode

 *
 * ## Writing a message #
 * Once the Mailbox Driver has been opened, the application can write a message to the remote
 * endpoint using #Mailbox_write.
 *
 * ## Reading a message #
 * Once the Mailbox Driver has been opened, the application can read a message from the remote
 * endpoint using #Mailbox_read followed by #Mailbox_readFlush after the message is fully read.
 *
 ** @defgroup MAILBOX_DRIVER      MAILBOX Driver
 */
#ifndef ti_drivers_Mailbox__include
#define ti_drivers_Mailbox__include

/**
@defgroup MAILBOX_DRIVER_EXTERNAL_FUNCTION            Mailbox Driver External Functions
@ingroup MAILBOX_DRIVER
@brief
*   This section has a list of all the exported API which the application needs to
*   invoke in order to use the driver
*/
/**
@defgroup MAILBOX_DRIVER_EXTERNAL_DATA_STRUCTURE      Mailbox Driver External Data Structures
@ingroup MAILBOX_DRIVER
@brief
*   This section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup MAILBOX_DRIVER_ERROR_CODE                   Mailbox Driver Error Codes
@ingroup MAILBOX_DRIVER
@brief
*   This section has a list of all the error codes which are generated by the Mailbox Driver
*   module
*/
/**
@defgroup MAILBOX_DRIVER_INTERNAL_FUNCTION            Mailbox Driver Internal Functions
@ingroup MAILBOX_DRIVER
@brief
*   This section has a list of all internal API which are not exposed to the external
*   application.
*/
/**
@defgroup MAILBOX_DRIVER_INTERNAL_DATA_STRUCTURE      Mailbox Driver Internal Data Structures
@ingroup MAILBOX_DRIVER
@brief
*   This section has a list of all internal data structures which are used internally
*   by the Mailbox module.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>
#include <ti/drivers/osal/SemaphoreP.h>


/** @addtogroup MAILBOX_DRIVER_ERROR_CODE
 *
 * @brief
 *  Base error code for the Mailbox module is defined below
 *  \include ti/common/mmwave_error.h
 *
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define MAILBOX_EINVAL                 (MMWAVE_ERRNO_MAILBOX_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define MAILBOX_EINUSE                 (MMWAVE_ERRNO_MAILBOX_BASE-2)

/**
 * @brief   Error Code: Out of memory
 */
#define MAILBOX_ENOMEM                 (MMWAVE_ERRNO_MAILBOX_BASE-3)

/**
 * @brief   Error Code: Invalid configuration
 */
#define MAILBOX_EINVALCFG              (MMWAVE_ERRNO_MAILBOX_BASE-4)

/**
 * @brief   Error Code: TX mailbox full. Application tried to send a message before ACK of
 *          the previously TX message was received. 
 *
 *          This ACK is part of the mailbox peripheral protocol layer and is 
 *          transparent to the application.
 */
#define MAILBOX_ETXFULL                (MMWAVE_ERRNO_MAILBOX_BASE-5)

/**
 * @brief   Error Code: Write Acknowledge timed out. Driver was pending on semaphore
 *          waiting for an acknowledge and the semaphore timed out.
 */
#define MAILBOX_ETXACKTIMEDOUT         (MMWAVE_ERRNO_MAILBOX_BASE-6)

/**
 * @brief   Error Code: Blocking read timed out.
 */
#define MAILBOX_EREADTIMEDOUT          (MMWAVE_ERRNO_MAILBOX_BASE-7)

/**
 * @brief   Error Code: Mailbox driver already initialized.
 */
#define MAILBOX_EINITIALIZED           (MMWAVE_ERRNO_MAILBOX_BASE-8)

/**
 * @brief   Error Code: Osal call failed.
 */
#define MAILBOX_EOSAL                  (MMWAVE_ERRNO_MAILBOX_BASE-9)

/**
 * @brief   Error Code: Invalid channel type. 
 *          Indicates that the channel type is invalid, OR
 *          Indicates that a mailbox instance is tried to be 
 *          opened with a channel type different than other instances (for the same endpoint).
 */
#define MAILBOX_EBADCHTYPE             (MMWAVE_ERRNO_MAILBOX_BASE-10)

/**
 * @brief   Error Code: Invalid channel type. 
 *          Indicates that the channel ID is invalid, OR
 *          Indicates that a channel ID is already in use.
 */
#define MAILBOX_EBADCHID               (MMWAVE_ERRNO_MAILBOX_BASE-11)

/**
 * @brief   Error Code: Mailbox channel in use by another instance.
 */
#define MAILBOX_ECHINUSE               (MMWAVE_ERRNO_MAILBOX_BASE-12)


/** @}*/

/**
 * @brief   Number of possible endpoint types
 */
#define MAILBOX_NUM_ENDPOINT_TYPES  (uint8_t)3

/**
 * @brief   Mailbox buffer size (in bytes) for data transfer (per direction).
 */
#define MAILBOX_DATA_BUFFER_SIZE   (uint32_t)2044U

/*!
 *  @brief    Wait forever define
 */
#define MAILBOX_WAIT_FOREVER   SemaphoreP_WAIT_FOREVER

/** @addtogroup MAILBOX_DRIVER_EXTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief      A handle that is returned from a Mailbox_open() call.
 */
typedef void*  Mbox_Handle;


/*!
 *  @brief      Mailbox Type settings
 *
 *  This enum defines the types of Mailbox. 
 *  Must use the enum values listed below as these values are used in expressions.
 *  A mailbox connects a "local endpoint" to a "remote endpoint". 
 *  Each endpoint is of a type defined by this enum.
 */
typedef enum
{
    /*!
      *  BSS Mailbox type
      */
    MAILBOX_TYPE_BSS = 0, 
    
    /*!
      *  MSS Mailbox type
      */
    MAILBOX_TYPE_MSS = 1
    #if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
    ,
    /*!
      *  DSS Mailbox type
      */
    MAILBOX_TYPE_DSS = 2,
    /*!
      *  Last Mailbox type
      */
    MAILBOX_TYPE_LAST = MAILBOX_TYPE_DSS
    #endif
} Mailbox_Type;


/*!
 *  @brief      Mailbox mode settings
 *
 *  This enum defines the read and write modes for the configured Mailbox.
 *  Note that not all types are supported by read/write APIs.
 */
typedef enum
{
    /*!
      *  Blocking mode.
      *  Driver uses a semaphore to block while data is being sent or received. Context of the call
      *  must be a Task. Please refer to Mailbox_write() and Mailbox_read() APIs for specific information.
      */
    MAILBOX_MODE_BLOCKING,

    /*!
      *  Polling mode. 
      *  Please refer to Mailbox_write() and Mailbox_read() APIs for specific information.
      */
    MAILBOX_MODE_POLLING,

    /*!
      *  Call back mode. 
      *  Valid only for Mailbox_read API.
      *  Application call back functions are called when interrupt is received
      *  for a new message to be read or when interrupted is received from an acknowledge of a 
      *  previously transmitted message. Application still needs to call the Mailbox_read() API
      *  to handle the received message.
      */
    MAILBOX_MODE_CALLBACK
} Mailbox_Mode;

/*!
 *  @brief      Mailbox operation mode settings
 *
 *  This enum defines the operation modes for the configured Mailbox.
 */
typedef enum
{
    /*!
      *  Current only one available operation mode. In this mode the driver allows for a partial
      *  read of a received message. #Mailbox_readFlush() needs to be used once application finishes
      *  reading the message.
      */
    MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED
} Mailbox_OpMode;

/*!
 *  @brief      Mailbox data transfer mode
 *
 *  This enum defines the data transfer mode for the mailbox instance.\n
 *  It configures how the data is transferred from the application buffer
 *  to the mailbox peripheral and from the mailbox peripheral to the 
 *  application buffer in the #Mailbox_write and #Mailbox_read APIs.
 *  The configuration applies to both directions.\n
 */
typedef enum
{
    /*!
      *  Data transfer between maibox memory and application memory done through memcpy()
      */
    MAILBOX_DATA_TRANSFER_MEMCPY,
    /*!
      *  Data transfer between maibox memory and application memory done through DMA
      *  @warning    This mode is currently not supported.
      */
    MAILBOX_DATA_TRANSFER_DMA
} Mailbox_DataTransferMode;

/*!
 *  @brief      Mailbox channel type
 *
 *  This enum defines the mailbox channel type.\n Used between MSS and DSS only.\n
 *  This field allows MSS<->DSS to use multiple (virtual) mailbox channels to communicate.\n
 *  As there is only one mailbox physical channel, even when multiple channels are open, only one
 *  channel can be used at a time by the application.\n
 *  When multiple channels are used, each channel is identified by a #Mailbox_ChID.\n
 *  The maximum number of channels (or mailbox instances) between MSS and DSS is given by #MAILBOX_CH_ID_MAX.\n
 *  Each channel is created by opening a new instance of the mailbox driver between MSS and DSS using
 *  a different Mailbox_ChID.\n
 *  If a channel is opened as #MAILBOX_CHTYPE_SINGLE then only one MSS<->DSS channel is allowed and Mailbox_ChID
 *  does not need to be populated.\n
 *  Note that the channel types can not be mixed. That is, a channel of type #MAILBOX_CHTYPE_SINGLE in 
 *  MSS (or DSS) can not communicate with a channel of type #MAILBOX_CHTYPE_MULTI on DSS (or MSS).\n
 *  Also note that if a channel is opened of type #MAILBOX_CHTYPE_MULTI, then all channels <B>must</B> be of #MAILBOX_CHTYPE_MULTI.
 *  @warning  
 *  On <B>XWR14xx</B> this field is NOT used. Application does not need to set this field in <B>XWR14xx</B>.\n 
 *  On <B>XWR16xx/XWR18xx/XWR68xx</B> this field is used for the communication between MSS<->DSS only and needs to be set 
 *  accordingly by the application when opening channels between MSS<->DSS.\n
 *  On <B>XWR16xx/XWR18xx/XWR68xx</B> this field is NOT used for the communication between MSS<->BSS or between DSS<->BSS. 
 *  Therefore, application does not need to set this field when MSS or DSS open a channel with BSS. 
 */
typedef enum
{
    /*!
      *  Only one mailbox channel (in each direction) allowed between DSS and MSS.\n
      *  In this case, the field Mailbox_ChID does NOT need to be specified.
      */
    MAILBOX_CHTYPE_SINGLE,
    /*!
      *  Multiple mailbox channels (in each direction) allowed between DSS and MSS.\n
      *  In this case, the field Mailbox_ChID needs to be specified.
      */
    MAILBOX_CHTYPE_MULTI
} Mailbox_ChType;

/*!
 *  @brief      Mailbox channel ID
 *
 *  This enum defines the possible mailbox channel IDs.\n Used between MSS and DSS only.\n
 *  IDs are used only when #Mailbox_ChType is of type #MAILBOX_CHTYPE_MULTI.
 *
 *  @warning  
 *  On <B>XWR14xx</B> this field is NOT used. Application does not need to set this field in <B>XWR14xx</B>.\n 
 *  On <B>XWR16xx/XWR18xx/XWR68xx</B> this field is used for the communication between MSS<->DSS only and needs to be set 
 *  accordingly by the application when opening channels between MSS<->DSS.\n
 *  On <B>XWR16xx/XWR18xx/XWR68xx</B> this field is NOT used for the communication between MSS<->BSS or between DSS<->BSS. 
 *  Therefore, application does not need to set this field when MSS or DSS open a channel with BSS. 
 */
typedef enum
{
    /*!
      *  ID = 0
      */
    MAILBOX_CH_ID_0 = 0,
    /*!
      *  ID = 1
      */
    MAILBOX_CH_ID_1 = 1,
    /*!
      *  ID = 2
      */
    MAILBOX_CH_ID_2 = 2,
    /*!
      *  ID = 3
      */
    MAILBOX_CH_ID_3 = 3,
    /*!
      *  ID = 4
      */
    MAILBOX_CH_ID_4 = 4,
    /*!
      *  ID = 5
      */
    MAILBOX_CH_ID_5 = 5,
    /*!
      *  ID = 6
      */
    MAILBOX_CH_ID_6 = 6,
    /*!
      *  ID = 7
      */
    MAILBOX_CH_ID_7 = 7,
    /*!
      *  Maximum ID value, which is #MAILBOX_CH_ID_7 
      */
    MAILBOX_CH_ID_MAX = MAILBOX_CH_ID_7    
} Mailbox_ChID;

/*!
 *  @brief      The definition of a callback function used by the MAILBOX driver
 *              when used in MAILBOX_MODE_CALLBACK.
 *              The callback can occur in task or HWI context.
 *
 *  @warning    Making Mailbox_read() calls within its own callback
 *              routines are STRONGLY discouraged as it will impact Task and
 *              System stack size requirements! 
 *
 *  @param      handle                  Mbox_Handle
 *  @param      remoteEndpoint          Remote endpoint
 *
 */
typedef void        (*Mailbox_Callback)    (Mbox_Handle handle, Mailbox_Type remoteEndpoint);

/*!
 *  @brief    Mailbox Configuration Parameters
 *
 *  Mailbox configuration parameters are used with the Mailbox_open() call. Default values for
 *  these parameters are set using Mailbox_Config_init().
 *
 *  @sa       Mailbox_Config_init()
 *
 */
typedef struct Mailbox_Config_t
{
    /**
     * @brief  Mailbox read mode
     */
    Mailbox_Mode             readMode;         
    /**
     * @brief  Mailbox write mode
     */
    Mailbox_Mode             writeMode;      
    /**
     * @brief   Timeout for read semaphore
     */
    uint32_t                 readTimeout;      
    /**
     * @brief   Timeout for write semaphore
     */
    uint32_t                 writeTimeout;     
    /**
     * @brief   Pointer to read callback
     */
    Mailbox_Callback         readCallback;
    /**
     * @brief  Driver mode of operation
     */
    Mailbox_OpMode           opMode;  
    /**
     * @brief  Data transfer mode
     */
    Mailbox_DataTransferMode dataTransferMode;
    /**
     * @brief  Mailbox channel type.
     *         Used only when remote endpoint is MSS or DSS.
     *
     *  @warning  
     *  On <B>XWR14xx</B> this field is NOT used. Application does not need to set this field in <B>XWR14xx</B>.\n 
     *  On <B>XWR16xx/XWR18xx/XWR68xx</B> this field is used for the communication between MSS<->DSS only and needs to be set accordingly. 
     *  For MSS<->BSS or DSS<->BSS communication in <B>XWR16xx/XWR18xx/XWR68xx</B> application does not need to set this field.
     */
    Mailbox_ChType           chType;    
    /**
     * @brief  Channel ID for this instance of the driver. 
     *         Used only when remote endpoint is MSS or DSS and when chType is set to MAILBOX_CHTYPE_MULTI.
     *
     *  @warning  
     *  On <B>XWR14xx</B> this field is NOT used. Application does not need to set this field in <B>XWR14xx</B>.\n 
     *  On <B>XWR16xx/XWR18xx/XWR68xx</B> this field is used for the communication between MSS<->DSS only and needs to be set accordingly. 
     *  For MSS<->BSS or DSS<->BSS communication in <B>XWR16xx/XWR18xx/XWR68xx</B> application does not need to set this field.
     */
    Mailbox_ChID             chId;    
} Mailbox_Config;

/**
 * @brief
 *  Mailbox Statistics 
 *
 * @details
 *  The structure describes the Mailbox statistics information
 *
 */
typedef struct Mailbox_Stats_t
{
    /**
     * @brief   Status of TX box (full or empty)
     */
    uint32_t              txBoxStatus;  
    /**
     * @brief   Number of TX messages successfully sent (acknowledgement has been received)
     */
    uint16_t              txCount;  
    /**
     * @brief   Number of RX messages received
     */
    uint16_t              rxCount;  
    /**
     * @brief   Flag used for read polling mode. Indicate if a new message has arrived. 
     */
    uint32_t              newMessageFlag;
    /**
     * @brief   Number of bytes already read by past calls to the read API in current message
     */
    uint32_t              numBytesRead; 
    /**
     * @brief   Number of full box ISR received
     */
    uint16_t              boxFullIsrCount;  
    /**
     * @brief   Number of empty box ISR received
     */
    uint16_t              boxEmptyIsrCount;  
    /**
     * @brief   Number of times the readFlush API was called.
     */
    uint16_t              readFlushCount; 
    /**
     * @brief   Number of full box ISR received with invalid arg
     */
    uint8_t               boxFullIsrErrCount;  
    /**
     * @brief   Number of empty box ISR received with invalid arg
     */
    uint8_t               boxEmptyIsrErrCount;  
    /**
     * @brief   Mailbox register
     */
    uint32_t              regLclToRemRaw;  
    /**
     * @brief   Mailbox register
     */
    uint32_t              regRemToLocRaw;  
    /**
     * @brief   Mailbox register
     */
    uint32_t              regLclToRemMask;  
    /**
     * @brief   Mailbox register
     */
    uint32_t              regRemToLocMask;  
}Mailbox_Stats;

/**
@}
*/

/*!
 *  @brief  Function to close a Mailbox peripheral specified by the Mailbox handle
 *
 *  @pre    Mailbox_open() has been called.
 *
 *  @param[in]  handle      A Mbox_Handle returned from Mailbox_open()
 *
 *  @sa     Mailbox_open()
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_close(Mbox_Handle handle);

/*!
 *  @brief  Function to initialize the Mailbox module.\n
 *          It must be called only once per local endpoint.
 *
 *  @pre    This function must be called before
 *          any other Mailbox driver API is used.
 *
 *  @param[in]   localEndpoint   Type of local mailbox endpoint. 
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_init(Mailbox_Type localEndpoint);

/*!
 *  @brief  Function to initialize an instance of the mailbox driver.
 *
 *  For <B>XWR14xx</B>: \n
 *  Between MSS<->BSS only one instance of the driver can be opened.\n
 *  For <B>XWR16xx/XWR18xx/XWR68xx</B>: \n
 *  Between MSS<->BSS only one instance of the driver can be opened.\n
 *  Between DSS<->BSS only one instance of the driver can be opened.\n
 *  Between MSS<->DSS multiple instances of the driver can be opened (each instance controls one virtual mailbox channel).\n
 *  @sa Mailbox_ChType Mailbox_ChID
 *
 *  @pre    Mailbox_init() has been called
 *
 *  @param[in]   cfg                Pointer to configuration block. 
 *  @param[in]   remoteEndpoint     Remote endpoint that this mailbox instance will communicate with. 
 *  @param[out]  errCode            Pointer to error code value/status to be returned by the function
 *                        
 *
 *  @return A Mbox_Handle upon success. NULL if an error occurs.
 *
 *  @sa     Mailbox_init()
 *  @sa     Mailbox_close()
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern Mbox_Handle Mailbox_open(Mailbox_Type remoteEndpoint, const Mailbox_Config *cfg,  int32_t* errCode);

/*!
 *  @brief  Function to initialize the Mailbox_Config struct to its defaults
 *
 *  @param[in]  cfg     A pointer to Mailbox_Config structure for
 *                      initialization
 *
 *  Defaults values are:
 * @code
       readMode         = Mailbox_MODE_BLOCKING
       writeMode        = Mailbox_MODE_BLOCKING
       readTimeout      = MAILBOX_WAIT_FOREVER     
       writeTimeout     = MAILBOX_WAIT_FOREVER    
       readCallback     = NULL
       opMode           = MAILBOX_OPERATION_MODE_PARTIAL_READ_ALLOWED
       dataTransferMode = MAILBOX_DATA_TRANSFER_MEMCPY
       chType           = MAILBOX_CHTYPE_SINGLE
       chId             = MAILBOX_CH_ID_0   
   @endcode   
 *
 *  @warning  
 *  On <B>XWR14xx</B> the fields chType and chId are NOT used. Application does not need to set these
 *  fields in <B>XWR14xx</B>.\n 
 *  On <B>XWR16xx/XWR18xx/XWR68xx</B> the fields chType and chId are used for the communication between MSS<->DSS only 
 *  and need to be set accordingly.\n 
 *  For MSS<->BSS or DSS<->BSS communication in <B>XWR16xx/XWR18xx/XWR68xx</B> application does not need to set these
 *  fields.
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */ 
extern int32_t Mailbox_Config_init(Mailbox_Config *cfg);

/*!
 *  @brief  Function that writes data to a Mailbox. 
 *
 * Mailbox can only send one message at a time to a remote endpoint. 
 * After data is copied to mailbox buffer, driver triggers interrupt to remote endpoint. 
 * This means that  a call to the Mailbox_write() is always a complete mailbox transaction.
 * A new message can only be sent after the previous message has been acknowledged by the remote endpoint. 
 * The acknowledgement process is handled internally by the driver.
 * All interrupts related to the write operations are managed by the driver and are not exposed to the application.
 * If called with data that will surpass the maximum size of the mailbox buffer, the write operation will fail with an error 
 * code and nothing will be written to the mailbox buffer. Returns number of bytes written or error.\n
 * The maximum size of the mailbox buffer is given by #MAILBOX_DATA_BUFFER_SIZE. Application code is responsible for
 * fragmentation of the message if the size is bigger than the mailbox buffer.
 *
 *  Write modes:
 *
 * In Mailbox_MODE_BLOCKING: \n
 *  Mailbox_write() will block task execution until the message has been copied from application to the mailbox buffer
 *  and an acknowledgement is received from the remote endpoint.
 *  If a new Mailbox_write() is issued before the acknowledgement is received, the write will fail with an error 
 *  code returned to application. 
 *
 * In Mailbox_MODE_POLLING: \n
 *  Mailbox_write() will block task execution until the message has been copied from application to the mailbox buffer
 *  but it does not wait for an acknowledgement from the remote endpoint.
 *  If a new Mailbox_write() is issued before the acknowledgement is received, the write will fail with an error code 
 *  returned to application. 
 *  In this mode, application does not know when acknowledgement is received and it may try to write the next message 
 *  multiple times until it succeeds as if it is polling for the status of the acknowledgement.
 *
 *  @pre    Mailbox_open() has been called
 *
 *
 *  @param[in]  handle      A Mbox_Handle
 *
 *  @param[in]  buffer      A pointer to buffer containing data to
 *                          be written to the Mailbox.
 *
 *  @param[in]  size        The number of bytes in the buffer that should be written
 *                          to the Mailbox.
 *
 *  @return Returns the number of bytes that have been written to the Mailbox.
 *          If an error occurs, one of the Mailbox Error codes is returned. 
 *
 *  @sa Mailbox_writeReset()
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
 /* Code review: Input variable "size" can be 16bit but 32bit matches the RL interface*/
extern int32_t Mailbox_write(Mbox_Handle handle, const uint8_t *buffer, uint32_t size);


/*!
 *  @brief  Function that reads data from a Mailbox.
 *
 * 
 * Mailbox can only read one message at a time from a remote endpoint.
 * Multiple Mailbox_read() calls can be done for the same message in the mailbox. 
 *
 * E.g. Application can read part of the message to figure out full message length and issue a subsequent Mailbox_read(). 
 *
 * Mailbox driver keeps track internally of number of bytes read for a message. 
 * The first Mailbox_read() for a message always starts at byte zero. 
 * In case of multiple  Mailbox_read() calls for the same message, the subsequent reads start in the 
 * next byte from where the previous read stopped. 
 * Once application finishes reading the message it <B>must</B> issue a  Mailbox_readFlush() 
 * to release the mailbox buffer and notify the remote endpoint.\n 
 * The maximum size of the mailbox buffer is given by #MAILBOX_DATA_BUFFER_SIZE. Application code is responsible for
 * fragmentation of the message if the size is bigger than the mailbox buffer.
 *
 * All interrupts related to the read operation are managed by the driver and are not exposed to the application.
 * Returns number of bytes read or error.
 *
 *  Read modes:
 * 
 * In Mailbox_MODE_BLOCKING: \n
 * If this is the first read on a new message, Mailbox_read() will block task execution until a new message has arrived 
 * in the local Mailbox buffer
 * and it is copied to the application buffer.
 * If this is not a new message, Mailbox_read() will block task execution until "size" bytes are copied to the application buffer.
 *
 * In Mailbox_MODE_POLLING: \n
 * If this is the first read on a new message and the new message has not arrived, Mailbox_read() will return size 
 * zero to indicate no bytes were read. 
 * If the new message has arrived or if this is not a new message, Mailbox_read() will copy the data to application buffer. 
 * Mailbox_read() will return after "size" bytes are copied to the application buffer.
 *
 * In Mailbox_MODE_CALLBACK: \n
 * The driver will invoke the application callback function once a new message is received in the mailbox.
 * Application is responsible for calling Mailbox_read() to read the new message. 
 * Mailbox_read() copies the data into the application buffer and then exit. 
 *
 * In any of the modes described above, Mailbox_readFlush() needs to be issued after the message is fully read by the application.
 *
 *  @pre    Mailbox_open() has been called
 *
 *  @param[in]  handle        A Mbox_Handle
 *
 *  @param[in]  buffer        A pointer to an empty buffer in which
 *                            received data should be written to.
 *
 *  @param[in]  size          The number of bytes to be written into buffer
 *
 *  @return Returns the number of bytes that have been read from the Mailbox.
 *          If an error occurs, one of the Mailbox Error codes is returned.
 *
 *  @sa Mailbox_readFlush()
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
 /* Code review: Input variable "size" can be 16bit but 32bit matches the RL interface*/
extern int32_t Mailbox_read(Mbox_Handle handle, uint8_t *buffer, uint32_t size);


/*!
 *  @brief  Function that should be called after application is done reading the message.
 *
 * Notifies the remote endpoint that the local mailbox is ready to receive a new message after the previous message was read.\n
 * Remote endpoint can not send a new message to the local endpoint until Mailbox_readFlush() is issued by the local endpoint.
 * Once Mailbox_readFlush() is issued, the local endpoint must assume that the previously received message
 * is no longer in the mailbox buffer and subsequent Mailbox_read() will return no data until a new message arrives in the mailbox.
 *
 *  @pre    Mailbox_open() has been called
 *
 *  @param[in]  handle        A Mbox_Handle
 *
 *  @return Returns error code in case of failure.
 *
 *  @sa Mailbox_read()
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_readFlush(Mbox_Handle handle);

/*!
 *  @brief  Function that should be called if application is unable to write to mailbox.
 *
 * This function is used to handle the abnormal situation when an acknowledgement was never received for the previous
 * message sent in the mailbox. This acknowledgement is at the internal Mailbox Peripheral protocol layer and 
 * the application perceives this situation when a call to Mailbox_write() returns error code #MAILBOX_ETXFULL. 
 *
 * This condition can arise in two circumstances:
 * 1. Not enough processing time: The local endpoint is sending a message before the remote endpoint had 
 * chance to acknowledge the previous message (not enough time).
 * 2. Enough processing time: The remote endpoint did not acknowledge the previous message even though enough time has elapsed. 
 *    This indicates an error condition in the remote endpoint.
 * 
 * Mailbox_writeReset() resets the mailbox driver state machine so that application can send a new message to the remote endpoint.
 *
 * After invoking this function, the application can proceed to write a new message to the remote endpoint using Mailbox_write().
 *
 *  @pre    Mailbox_open() has been called
 *
 *  @param[in]  handle        A Mbox_Handle
 *
 *  @return Returns error code in case of failure.
 *
 *  @sa Mailbox_write()
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_writeReset(Mbox_Handle handle);

/*!
 *  @brief  Function that collects mailbox driver statistics
 *
 * This function is used to collect mailbox driver statistics.
 * It's main purpose is debugging of mailbox driver.
 *
 *  @pre    Mailbox_open() has been called
 *
 *  @param[in]   handle        A Mbox_Handle
 *  @param[out]  stats         Pointer to mailbox stats structure to be filled
 *
 *
 *  @return Returns error code in case of failure.
 *
 *  \ingroup MAILBOX_DRIVER_EXTERNAL_FUNCTION
 *
 */
extern int32_t Mailbox_getStats(Mbox_Handle handle, Mailbox_Stats * stats);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_Mailbox__include */
