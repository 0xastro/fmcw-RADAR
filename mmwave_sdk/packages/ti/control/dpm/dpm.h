/**
 *   @file  dpm.h
 *
 *   @brief
 *      Data Path Manager
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @mainpage Data Path Manager (DPM)
 *
 *  The DPM Module provides a well defined IPC mechanism which allows
 *  application and data path processing chains (DPC) to communicate with
 *  each other. The applications and DPC could be located on the same subsystem
 *  or could be located on different subsystems. The DPM was designed
 *  for the following:-
 *
 *  1. Modularity
 *     The framework provides a clear demarcation between the application
 *     code and the data path processing chain.
 *
 *  2. Simple Intuitive API
 *     The framework exposes a set of standard well defined API which allows
 *     application developers to load, configure and execute the processing
 *     chain
 *
 * Data Path Processing Chain (DPC) can be defined as any execution block which
 * receives input data, processes it and generates an output. Example:
 * The Low Level processing chain can be defined as a a block which
 * receives ADC data and generate a point cloud.
 *
 * Except the XWR14xx all the other supported platforms have the DSS and MSS
 * subsystem. In most use cases the MSS is defined as a control domain while
 * the DSS actually executes the DPC. The DPM module provides an ability to
 * exchange configuration between the MSS and DSS via the IPC. Application and
 * DPC developers are thus abstracted from this complexity.
 *
 * ## Processing Chain Developers #
 *
 * The DPM can be used by developers to create their own processing chains
 * in which case please refer to the processing chain exported API
 * @sa DPM_PROCESSING_CHAIN_API
 *
 * ## Reporting #
 *
 * Application using the framework need to register a report function which receives
 * status reports from the framework module. This is an important feedback mechanism
 * through which the framework can keep the application informed about the status of
 * operations. For a list of all the reports @sa DPM_Report
 *
 * ## Domains #
 *
 * The DPM module introduces a concept of domains which identifies the extent to
 * which the Control and Data Path processing chains are distributed
 *
 * ### Local Domains #
 *
 * In the local domain the control and DPC are executing on the same subsystem.
 *
 * @image html local_domain.png
 *
 * ### Remote Domains #
 *
 * In the remote domain the control is executing on one subsystem while the DPC
 * is executing on another subsystem
 *
 * @image html remote_domain.png
 *
 * ### Distributed Domains #
 *
 * In the distributed domain the control is executing on one subsystem while the DPC
 * is executing on both the subsystems
 *
 * @image html distributed_domain.png
 *
 * ## Restrictions #
 * The DPM framework is currently limited to a single remote/distributed domain.
 * Multiple local domains can be instantiated without any issues.
 *
 * Due to memory consideration, DPM has IPC local queue set to @ref DPM_MAX_LOCAL_QUEUE.
 * 
 * In remote/distributed domain setup, messages delivered through report function to MSS/DSS is not serialized.
 * Hence the order (MSS gets it first or DSS gets it first) cannot be guaranteed.
 */

/** @defgroup DPM      DPM API
 */
#ifndef DPM_H
#define DPM_H

/* mmWave SDK Include Files: */
#include <ti/common/mmwave_error.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/SemaphoreP.h>

/**
@defgroup DPM_EXTERNAL_FUNCTION            DPM External Functions
@ingroup DPM
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup DPM_EXTERNAL_DATA_STRUCTURE       DPM External Data Structures
@ingroup DPM
@brief
*   The section has a list of all the data structures which are exposed to the
*   application
*/
/**
@defgroup DPM_EXTERNAL_DEFINITIONS          DPM External Defintions
@ingroup DPM
@brief
*   The section has a list of all external definitions which are exposed by the
*   module.
*/
/**
@defgroup DPM_COMMAND                       DPM Commands
@ingroup DPM
@brief
*   The section has a list of all commands which are supported by the DPM framework
*   These commands are always available and can be used irrespective of the DPC.
*/
/**
@defgroup DPM_ERROR_CODE                    DPM Error Codes
@ingroup DPM
@brief
*   The section has a list of all the error codes which are generated by the module
*/
/**
@defgroup DPM_INTERNAL_FUNCTION             DPM Internal Functions
@ingroup DPM
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup DPM_INTERNAL_DATA_STRUCTURE       DPM Internal Data Structures
@ingroup DPM
@brief
*   The section has a list of all internal data structures which are used internally
*   by the module.
*/
/**
@defgroup DPM_INTERNAL_DEFINITIONS          DPM Internal Defintions
@ingroup DPM
@brief
*   The section has a list of all internal definitions which are used internally
*   by the module.
*/
/**
@defgroup DPM_PROCESSING_CHAIN_API          DPM Processing Chain API
@ingroup DPM
@brief
*   The section has a list of all function which need to be populated by a developers
*   writing a custom processing chain to fit in with the DPM framework
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DPM_ERROR_CODE
 *  Base error code for the mmWave module is defined in the
 *  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define DPM_EINVAL                  (MMWAVE_ERRNO_DPM_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define DPM_ENOMEM                  (MMWAVE_ERRNO_DPM_BASE-2)

/**
 * @brief   Error Code: Not supported
 */
#define DPM_ENOTSUP                 (MMWAVE_ERRNO_DPM_BASE-3)

/**
 * @brief   Error Code: Processing chain configuration error
 */
#define DPM_EPROFCFG                (MMWAVE_ERRNO_DPM_BASE-4)

/**
 * @brief   Error Code: Invalid IOCTL command passed to the processing
 * chain. Processing chain developers should set the error code to this
 * if the command during the configuration does not fall between the
 * supported range.
 */
#define DPM_EINVCMD                 (MMWAVE_ERRNO_DPM_BASE-5)

/**
@}
*/

/** @addtogroup DPM_EXTERNAL_DEFINITIONS
 @{ */

/**
 * @brief
 *  This is the maximum length of the file name on assertion
 */
#define DPM_MAX_FILE_NAME_LEN                       32

/**
 * @brief
 *  This is the maximum size of the informational message block which can
 *  be passed by the DPC to the application
 */
#define DPM_MAX_INFO_SIZE                           64

/**
 * @brief
 *  This is the common definition for the maximum number of buffers which
 *  can be exchanged by the application with the DPC
 */
#define DPM_MAX_BUFFER                              3

/**
 * @brief   This is the maximum size of DPC configuration data which can be
 * transported by the DPM module.
 */
#define DPM_MAX_DPC_DATA                            512U

/**
@}
*/

/** @addtogroup DPM_COMMAND
 @{ */

/**
 * @brief
 * This is the command which can be used by the application to notify
 * the processing chain that buffers are available to be processed.
 *      arg    = &DPM_Buffer
 *      argLen = sizeof (DPM_Buffer)
 *  This will only notify the local processing chain and will
 *  not relay the request to the remote or distributed domain.
 *
 * *NOTE*: Please refer to the DPC documentation on the exact format
 * of data which is to be passed.
 */
#define DPM_CMD_INJECT_DATA             1

/**
 * @brief
 * This is the command which can be used by the application to inform the
 * module that there was a CPU/ESM fault reported by the mmWave link layer.
 * This is passed to the processing chain to take appropriate action.
 *      arg    = NULL
 *      argLen = 0
 */
#define DPM_CMD_BSS_FAULT               2

/**
 * @brief
 * This is the command which can be used by the processing chain to inform
 * the application about a fault/exception which the chain encountered.
 * The application is notified about the assertion via the reporting API.
 *      arg    = &assert
 *      argLen = sizeof (DPM_DPCAssert)
 * @sa
 * DPM_Report_DPC_ASSERT
 */
#define DPM_CMD_DPC_ASSERT              3

/**
 * @brief
 * This is a generic command which can be used by the processing chain to send
 * any custom information to the application. The payload is not interpreted by
 * the framework and is passed to the application via the reporting mechanism
 *      arg    = &info
 *      argLen <= sizeof(DPM_DPCInfo)
 * @sa
 *  DPM_Report_DPC_INFO
 */
#define DPM_CMD_DPC_INFO                4

/**
 * @brief
 * Processing chain specific command start index: Command Index before this
 * are reserved for use by the core Module. All processing chains should use
 * this as a starting index for their own custom commmands.
 */
#define DPM_CMD_DPC_START_INDEX         100

/**
@}
*/

/** @addtogroup DPM_EXTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  DPM Handle
 */
typedef void*   DPM_Handle;

/**
 * @brief
 *  DPM Processing Chain Handle
 */
typedef void*   DPM_DPCHandle;

/**
 * @brief
 *  DPM Data Path Domain
 *
 * @details
 *  The enumeration is used to describe the domains to which the data path
 *  processing chains extend.
 */
typedef enum DPM_Domain_e
{
    /**
     * @brief   Localized Domain: The Control & Data Path chain
     * is executed in the context of single subsystem i.e. MSS
     * or DSS only.
     *
     */
    DPM_Domain_LOCALIZED = 0x1,

    /**
     * @brief   Remote Domain: The DPM Data Path Processing chain
     * is executed in the context of single subsystem i.e. MSS or DSS only.
     * However the DPM control can be executed from another subsystem.
     *
     * For example: On the XWR16xx the MSS could be controlling the DPM
     * while the data processing chain is entirely executing on the DSS
     */
    DPM_Domain_REMOTE,

    /**
     * @brief   Distributed Domain: The DPM Data Path Processing chain
     * is executed in the context of multiple subsystems. The DPM control
     * can be executed from either subsystem
     */
    DPM_Domain_DISTRIBUTED
}DPM_Domain;

/**
 * @brief
 *  Report Types
 *
 * @details
 *  The enumeration is used to describe the various reports which are generated
 *  by the Module. Applications are notified about the operational status
 *  of the Module through these reports via the registered Reporting Function.
 *  Applications are expected to interpret these reports and take appropriate
 *  action.
 */
typedef enum DPM_Report_e
{
    /**
     * @brief   This is the report type to indicate that the processing chain
     * has been started.
     *
     *      Argument | Value        | Description
     *      ---------|--------------|------------
     *      arg0     | 0            | Not Used
     *      arg1     | 0            | Not Used
     *
     * This report is issued to all DPM entities
     *
     * Once the processing chain has been successfully started the application can start
     * the BSS via the MMWave API
     *
     * @ref DPM_start
     */
    DPM_Report_DPC_STARTED = 0x1,

    /**
     * @brief   This is the report type to indicate that the processing chain has
     * been stopped.
     *
     *      Argument | Value        | Description
     *      ---------|--------------|------------
     *      arg0     | 0            | Not Used
     *      arg1     | 0            | Not Used
     *
     * This report is issued to all DPM entities
     *
     * @ref DPM_stop
     */
    DPM_Report_DPC_STOPPED,

    /**
     * @brief   This is the report type to indicate that the processing chain
     * executing has detected a fault/exception
     *
     *      Argument | Value                | Description
     *      ---------|----------------------|------------
     *      arg0     | &DPM_DPCAssert       | Address of the assert information
     *      arg1     | 0                    | Not Used
     *
     * This report is issued to all DPM entities
     *
     * @ref DPM_DPCAssert
     */
    DPM_Report_DPC_ASSERT,

    /**
     * @brief   This is the report type to indicate that the processing chain
     * is being configured via the DPM_ioctl interface.  The following is the
     * interpretation of the optional arguments:-
     *
     *      Argument | Value                 | Description
     *      ---------|-----------------------|------------
     *      arg0     | Command               | Standard/DPC specific commands
     *      arg1     | Command Specific Data | Refer to the command documentation
     *
     * This report is issued to all DPM entities
     *
     * @ref DPM_ioctl
     */
    DPM_Report_IOCTL,

    /**
     * @brief   This is the report type to indicate that the processing
     * chain has results which are available.
     *
     *      Argument | Value                 | Description
     *      ---------|-----------------------|------------
     *      arg0     | &DPM_Buffer           | Address of the DPM Result
     *      arg1     | 0                     | Not Used
     *
     * This report is issued only to the remote DPM entity on which the DPC
     * is *not* executing.
     *
     * @ref DPM_Buffer
     */
    DPM_Report_NOTIFY_DPC_RESULT,

    /**
     * @brief   This is the report type to indicate that the remote DPM domain
     * has acknowledged the DPC results.
     *
     *      Argument | Value                 | Description
     *      ---------|-----------------------|------------
     *      arg0     | &DPM_Buffer           | Address of the DPM Result
     *      arg1     | 0                     | Not Used
     *
     * This report is issued only to the DPM entity on which the DPC is executing.
     *
     * This is an acknowledgment that the results have been passed to the remote DPM
     * domain. This report is only generated if the "isAckNeeded" flag is set.
     *
     * @ref DPM_Buffer
     */
    DPM_Report_NOTIFY_DPC_RESULT_ACKED,

    /**
     * @brief   This is the report type to indicate that the DPC has passed
     * information to the application. Application developers would need to read
     * the processing chain specific documentation to interpret this information.
     *
     *      Argument | Value            | Description
     *      ---------|------------------|------------
     *      arg0     | &DPM_DPCInfo     | DPC Specific information
     *      arg1     | 0                | Not Used
     *
     * This report is issued to all DPM entities.
     *
     * @ref DPM_DPCInfo
     */
    DPM_Report_DPC_INFO
}DPM_Report;

/**
 *  @b Description
 *  @n
 *      Applications can register a report function which is invoked by the module
 *      to report the status of the processing chain to the application. The mechanism
 *      can be used to inform the applications about the status of the processing chain.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  instanceId
 *      DPM Instance Identifier which generated the response.
 *  @param[in]  errCode
 *      Error code associated with the status event. This will be set to 0 to indicate
 *      that the report was successful.
 *  @param[in]  arg0
 *      Optional argument.
 *  @param[in]  arg1
 *      Optional argument.
 *
 *  *NOTE*: Please refer to the @ref DPM_Report for more information
 *  about the optional arguments and how to interpret them.
 *
 *  @retval
 *      Not applicable
 */
typedef void (*DPM_ReportFxn) (DPM_Report   reportType,
                               uint32_t     instanceId,
                               int32_t      errCode,
                               uint32_t     arg0,
                               uint32_t     arg1);

/**
 * @brief
 *  Processing Chain Assert
 *
 * @details
 *  Processing chains while executing on a core can encounter a fatal error.
 *  In such a scenario the DPM entities would need to be notified
 *  with information relevant to the error. The assertion information
 *  block here can be exchanged
 */
typedef struct DPM_DPCAssert_t
{
    /**
     * @brief   Line Number:
     */
    uint32_t        lineNum;

    /**
     * @brief   File Name:
     */
    char            fileName[DPM_MAX_FILE_NAME_LEN];

    /**
     * @brief   Processing Chain specific argument1
     */
    uint32_t        arg0;

    /**
     * @brief   Processing Chain specific argument2
     */
    uint32_t        arg1;
}DPM_DPCAssert;

/**
 * @brief
 *  Data Path Processing Chain Information
 *
 * @details
 *  Processing Chain specific information which can be passed by DPC
 *  developers to the application. The DPM Module does not define this
 *  information but is DPC specific. Please refer to the DPC documentation
 *  on how this information is interpreted.
 */
typedef struct DPM_DPCInfo_t
{
    /**
     * @brief   DPC Developers can populate the informational block
     * with custom information and can use the DPM to send this to
     * the application. Application are notified via the reporting mechanism
     */
    uint8_t     info[DPM_MAX_INFO_SIZE];
}DPM_DPCInfo;

/**
 * @brief
 *  Initialization configuration
 *
 * @details
 *  The structure is used to initialize the DPM module
 */
typedef struct DPM_InitCfg_t
{
    /**
     * @brief   Handle to the SOC Driver
     */
    SOC_Handle                  socHandle;

    /**
     * @brief   Execution domain.
     */
    DPM_Domain                  domain;

    /**
     * @brief   Each DPM Instance should be allocated a unique identifier
     * This will be reported to the application through the reporting mechanism
     * Unique DPM Instance identifiers are especially useful if operating in a
     * Distributed domain mode.
     */
    uint32_t                    instanceId;

    /**
     * @brief   This is an argument which is passed to the processing chain
     * during initialization. The DPM framework does not interpret this and
     * passes this as is to the processing chain. For more information please
     * refer to the processing chain documentation.
     */
    void*                       arg;

    /**
     * @brief   This is the size of the argument specified above. This is not
     * used by the DPM framework but is passed as is to the processing chain.
     * For more information please refer to the processing chain documentation.
     */
    uint32_t                    argSize;

    /**
     * @brief   Pointer to the processing configuration which is to be loaded
     * and executed in the DPM Instance.
     */
    struct DPM_ProcChainCfg_t*  ptrProcChainCfg;

    /**
     * @brief   Report Function: The DPM module will invoke the application
     * registered function to report events and status about the processing
     * chain
     */
    DPM_ReportFxn               reportFxn;
}DPM_InitCfg;

/**
 * @brief
 *  DPM Buffer Interface
 *
 * @details
 *  The DPM processing chain receive data buffers which are to be
 *  processed. The DPC processes these buffers and generates
 *  the results which are then passed to the application. The structure
 *  encompasses this information.
 */
typedef struct DPM_Buffer_t
{
    /**
     * @brief   Pointer to the buffer
     */
    uint8_t*            ptrBuffer[DPM_MAX_BUFFER];

    /**
     * @brief   Size of the result buffer
     */
    uint32_t            size[DPM_MAX_BUFFER];
}DPM_Buffer;

/**
 *  @b Description
 *  @n
 *      This is the DPC registered callback function which is
 *      can be used to pass input data to the processing chain to be processed.
 *      The format of the actual data buffers is DPC specific.
 *
 *      This function could be invoked from any execution context. It could
 *      be triggered by the application from an ISR or from another thread.
 *      Internally the DPM will notify the processing chain immediately about
 *      the buffer availability. DPC developers could either consume the buffer
 *      immediately or could process it in the DPM Framework execution context
 *
 *  @param[in]  handle
 *      Handle to the DPC
 *  @param[in]  ptrBuffer
 *      Pointer to the data buffer
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
typedef int32_t (*DPM_ProcChainInjectDataFxn) (DPM_DPCHandle    handle,
                                               DPM_Buffer*      ptrBuffer);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is invoked once the
 *      chirp available interrupt has been detected.
 *
 *      This is a special case inject data function which indicates that
 *      chirp data is available. This is always invoked in ISR context.
 *
 *  @param[in]  handle
 *      Handle to the DPC
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Not applicable
 */
typedef void (*DPM_ProcChainChirpAvailableCallbackFxn) (DPM_DPCHandle handle);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is invoked once the
 *      frame start interrupt has been detected
 *
 *  @param[in]  handle
 *      Handle to the DPC
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Not applicable
 */
typedef void (*DPM_ProcChainFrameStartCallbackFxn) (DPM_DPCHandle handle);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is used to initialize
 *      and setup the processing chain. This is invoked during the DPM
 *      Initialization process.
 *
 *  @param[in]  dpmHandle
 *      Handle to the DPM Module
 *  @param[in]  ptrInitCfg
 *      Pointer to the initialization configuration
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Success - DPC Handle
 *  @retval
 *      Error   - NULL
 */
typedef DPM_DPCHandle (*DPM_ProcChainInitFxn) (DPM_Handle         dpmHandle,
                                               DPM_InitCfg*       ptrInitCfg,
                                               int32_t*           errCode);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is used to execute the processing
 *      chain.
 *
 *  @param[in]  handle
 *      DPC Handle
 *  @param[out]  ptrResult
 *      Pointer to the result populated by the processing chain.
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
typedef int32_t (*DPM_ProcChainExecuteFxn) (DPM_DPCHandle  handle,
                                            DPM_Buffer*    ptrResult);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is used to configure the processing
 *      chain. DPC developers can use this to provide the ability to provide additional
 *      control capability.
 *
 *      NOTE: Processing Chain developers should ensure that the custom commands be offset
 *      by DPM_CMD_DPC_START_INDEX. This will ensure that there is no overlap
 *      with the standard DPM framework provided commands. If the command provided
 *      to the DPC is not within range; the DPC chain should return an error
 *      with the error code set to DPM_EINVCMD.
 *
 *      In addition to the DPC specific commands the following commands should also
 *      be handled by the processing chain developers:
 *      @sa DPM_CMD_BSS_FAULT
 *
 *  @param[in]  handle
 *      DPC handle
 *  @param[in]  cmd
 *      Command to be processed
 *  @param[in]  arg
 *      Command specific argument.
 *  @param[in]  argLen
 *      Command specific argument length
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
typedef int32_t (*DPM_ProcChainIoctlFxn) (DPM_DPCHandle   handle,
                                          uint32_t        cmd,
                                          void*           arg,
                                          uint32_t        argLen);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is used to start the processing
 *      chain.
 *
 *  @param[in]  handle
 *      DPC handle
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
typedef int32_t (*DPM_ProcChainStartFxn) (DPM_DPCHandle handle);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is used to stop the processing chain.
 *
 *  @param[in]  handle
 *      DPC handle
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
typedef int32_t (*DPM_ProcChainStopFxn) (DPM_DPCHandle handle);

/**
 *  @b Description
 *  @n
 *      This is the DPC registered function which is used to deinitialize the processing
 *      chain and is invoked during the DPM Deinitialization process.
 *
 *  @param[in]  handle
 *      DPC handle
 *
 *  \ingroup DPM_PROCESSING_CHAIN_API
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
typedef int32_t (*DPM_ProcChainDeinitFxn) (DPM_DPCHandle handle);

/**
 * @brief
 *  Processing Chain Configuration
 *
 * @details
 *  The DPM module is responsible for the registeration and execution of
 *  processing chains. Processing Chains can be created and registered by
 *  populating the following configuration
 */
typedef struct DPM_ProcChainCfg_t
{
    /**
     * @brief   Initialization function:
     */
    DPM_ProcChainInitFxn                      initFxn;

    /**
     * @brief   Start function:
     */
    DPM_ProcChainStartFxn                     startFxn;

    /**
     * @brief   Execute function:
     */
    DPM_ProcChainExecuteFxn                   executeFxn;

    /**
     * @brief   IOCTL function:
     */
    DPM_ProcChainIoctlFxn                     ioctlFxn;

    /**
     * @brief   Stop function:
     */
    DPM_ProcChainStopFxn                      stopFxn;

    /**
     * @brief   Deinitialization function:
     */
    DPM_ProcChainDeinitFxn                    deinitFxn;

    /**
     * @brief   [Optional] Inject Data Function:
     */
    DPM_ProcChainInjectDataFxn                injectDataFxn;

    /**
     * @brief   [Optional] Chirp Available Callback function:
     */
    DPM_ProcChainChirpAvailableCallbackFxn    chirpAvailableFxn;

    /**
     * @brief   [Optional] Frame Start Callback function:
     */
    DPM_ProcChainFrameStartCallbackFxn        frameStartFxn;
}DPM_ProcChainCfg;

/**
@}
*/

/***********************************************************************************************
 * DPM Exported API:
 ***********************************************************************************************/
extern DPM_Handle DPM_init (DPM_InitCfg* ptrInitCfg, int32_t* errCode);
extern int32_t DPM_synch (DPM_Handle handle, int32_t* errCode);
extern int32_t DPM_ioctl (DPM_Handle handle, uint32_t cmd, void* arg, uint32_t argLen);
extern int32_t DPM_start (DPM_Handle handle);
extern int32_t DPM_stop (DPM_Handle handle);
extern int32_t DPM_execute (DPM_Handle handle, DPM_Buffer* ptrResult);
extern int32_t DPM_sendResult (DPM_Handle handle, bool isAckNeeded, DPM_Buffer* ptrResult);
extern int32_t DPM_relayResult (DPM_Handle handle, DPM_DPCHandle dpcHandle, DPM_Buffer* ptrResult);
extern int32_t DPM_notifyExecute (DPM_Handle handle, DPM_DPCHandle dpcHandle, bool isrContext);
extern int32_t DPM_deinit (DPM_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* DPM_H */


