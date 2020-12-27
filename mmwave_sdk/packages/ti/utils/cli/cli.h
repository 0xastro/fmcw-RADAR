/**
 *   @file  cli.h
 *
 *   @brief
 *      This is the main header file for the CLI
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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
/** @mainpage CLI Utility
 *
 *  The CLI utility library provides a simple CLI console over the specified
 *  serial port.
 *
 *  The CLI library provides the following features:
 *  - Simple command parser
 *  - Applications can register their own custom commands
 *  - mmWave Extension support which handles the well defined mmWave control
 *    commands.
 *
 *  The CLI header file should be included in an application as follows:
 *  @code
    #include <ti/utils/cli/cli.h>
    @endcode
 *
 * The CLI library provides an mmWave extension library which provides predefined
 * CLI command handlers for the mmWave configuration commands. The mmWave configuration
 * is stored internally and an application can use #CLI_getMMWaveExtensionConfig
 * to get a copy of this configuration. This can then be used to configure the mmWave
 */
/** @defgroup CLI_UTIL      CLI Utility
 */
#ifndef CLI_H
#define CLI_H

#ifdef __cplusplus
extern "C" {
#endif

/* mmWave SDK Include Files: */
#include <ti/control/mmwave/mmwave.h>

/**
@defgroup CLI_UTIL_EXTERNAL_FUNCTION            CLI Utility External Functions
@ingroup CLI_UTIL
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/
/**
@defgroup CLI_UTIL_EXTERNAL_DATA_STRUCTURE      CLI Utility External Data Structures
@ingroup CLI_UTIL
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup CLI_UTIL_EXTERNAL_DEFINITION          CLI Utility External Definitions
@ingroup CLI_UTIL
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup CLI_UTIL_INTERNAL_FUNCTION            CLI Utility Internal Functions
@ingroup CLI_UTIL
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup CLI_UTIL_INTERNAL_DATA_STRUCTURE      CLI Utility Internal Data Structures
@ingroup CLI_UTIL
@brief
*   The section has a list of all internal data structures which are used internally
*   by the CLI module.
*/

/**************************************************************************
 ************************* CLI Module Definitions *************************
 **************************************************************************/

/** @addtogroup CLI_UTIL_EXTERNAL_DEFINITION
 @{ */

/**
 * @brief   This is the maximum number of CLI commands which are supported
 */
#define     CLI_MAX_CMD         32

/**
 * @brief
 *  This is the maximum number of CLI arguments which can be passed to a
 *  command.
 */
#define     CLI_MAX_ARGS        40

/**
@}
*/

/**************************************************************************
 ************************** CLI Data Structures ***************************
 **************************************************************************/

/** @addtogroup CLI_UTIL_EXTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief   Handle to the CLI module:
 */
typedef void*   CLI_Handle;

/**
 * @brief   CLI command handler:
 *
 *  @param[in]  argc
 *      Number of arguments
 *  @param[in]  argv
 *      Pointer to the arguments
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
typedef int32_t (*CLI_CmdHandler)(int32_t argc, char* argv[]);

/**
 * @brief
 *  CLI command table entry
 *
 * @details
 *  This is command entry which holds information which maps a
 *  command string to the corresponding command handler.
 */
typedef struct CLI_CmdTableEntry_t
{
    /**
     * @brief   Command string
     */
    char*               cmd;

    /**
     * @brief   CLI Command Help string
     */
    char*               helpString;

    /**
     * @brief   Command Handler to be executed
     */
    CLI_CmdHandler      cmdHandlerFxn;
}CLI_CmdTableEntry;

/**
 * @brief
 *  CLI configuration
 *
 * @details
 *  This is the configuration structure which is used to initialize and open
 *  the CLI module.
 */
typedef struct CLI_Cfg_t
{
    /**
     * @brief   CLI Prompt string (if any to be displayed)
     */
    char*               cliPrompt;

    /**
     * @brief   Optional banner string if any to be displayed on startup of the CLI
     */
    char*               cliBanner;

    /**
     * @brief   UART Command Handle used by the CLI
     */
    UART_Handle         cliUartHandle;

    /**
     * @brief   The CLI has an mmWave extension which can be enabled by this
     * field. The extension supports the well define mmWave link CLI command(s)
     * In order to use the extension the application should have initialized
     * and setup the mmWave.
     */
    uint8_t             enableMMWaveExtension;

    /**
     * @brief   The SOC driver handle is used to acquire device part number
     */
    SOC_Handle          socHandle;

    /**
     * @brief   The mmWave control handle which needs to be specified if
     * the mmWave extensions are being used. The CLI Utility works only
     * in the FULL configuration mode. If the handle is opened in
     * MINIMAL configuration mode the CLI mmWave extension will fail
     */
    MMWave_Handle       mmWaveHandle;

    /**
     * @brief   Task Priority: The CLI executes in the context of a task
     * which executes with this priority
     */
    uint8_t             taskPriority;

    /**
     * @brief   Flag which determines if the the CLI Write should use the UART
     * in polled or blocking mode.
     */
    bool                usePolledMode;

    /**
     * @brief   This is the table which specifies the supported CLI commands
     */
    CLI_CmdTableEntry   tableEntry[CLI_MAX_CMD];
}CLI_Cfg;

/**
@}
*/

/**************************************************************************
 *************************** Extern Definitions ***************************
 **************************************************************************/
extern int32_t CLI_open (CLI_Cfg* ptrCLICfg);
extern void    CLI_write (const char* format, ...);
extern int32_t CLI_close (void);
extern void    CLI_getMMWaveExtensionConfig(MMWave_CtrlCfg* ptrCtrlCfg);
extern void    CLI_getMMWaveExtensionOpenConfig(MMWave_OpenCfg* ptrOpenCfg);

#ifdef __cplusplus
}
#endif

#endif /* CLI_H */

