/**
 *   @file  cli_internal.h
 *
 *   @brief
 *      This is the internal Header for the CLI utility. This header file
 *      should *NOT* be directly included by applications.
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
#ifndef CLI_INTERNAL_H
#define CLI_INTERNAL_H

/* Include Files: */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/utils/cli/cli.h>

#define CLI_MAX_PARTNO_STRING_LEN          31U

/**
 * @brief
 *  CLI Master control block
 *
 * @details
 *  This is the MCB which tracks the CLI module
 */
typedef struct CLI_MCB_t
{
    /**
     * @brief   Configuration which was used to configure the CLI module
     */
    CLI_Cfg         cfg;

    /**
     * @brief   This is the number of CLI commands which have been added to the module
     */
    uint32_t        numCLICommands;

    /**
     * @brief   CLI Task Handle:
     */
    Task_Handle     cliTaskHandle;
}CLI_MCB;

/**
 * @brief
 *  CLI device part number information
 *
 * @details
 *  This is the struct to define part number and its corresponding string to be used in CLI
 */
typedef struct CLI_partInfoString_t
{
    uint8_t     partNumber;
    uint8_t     partNumString[CLI_MAX_PARTNO_STRING_LEN];
}CLI_partInfoString;

/*******************************************************************************************
 * CLI Global Variables:
 *******************************************************************************************/
extern CLI_MCB gCLI;

/*******************************************************************************************
 * mmWave Extension Exported API:
 *******************************************************************************************/
extern int32_t CLI_MMWaveExtensionInit(CLI_Cfg* ptrCLICfg);
extern int32_t CLI_MMWaveExtensionHandler(int32_t argc, char* argv[]);
extern void    CLI_MMWaveExtensionHelp(void);

#endif /* CLI_INTERNAL_H */
