/**
 *   @file  logger.h
 *
 *   @brief
 *      This is the header file for the MCPI Test Logger
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
#ifndef TEST_LOGGER_H
#define TEST_LOGGER_H

/* Serves as the flag for the benchmarking overview script. To be used as the first thing
printed with benchmarking strings. */
#define BENCHMARK "Benchmark : "

/**************************************************************************
 *************************** MCPI Definitions *****************************
 **************************************************************************/

/**
 *  @def    PASS
 *  @brief  Test Pass
 *
 *  @def    FAIL
 *  @brief  Test Fail
 */
typedef enum MCPI_TestResult_e
{
   MCPI_TestResult_PASS = 0,
   MCPI_TestResult_FAIL
} MCPI_TestResult;

/**
 * @brief
 *  MCPI Log Buffer Initialization
 *
 * @details
 *  The macro should be invoked by the application and the size of the MCPI
 *  log buffer should be specified. The log buffer is placed in a memory
 *  section .MCPILogBuffer.
 *
 *  Failure to invoke this macro will result in a linking error
 */
#define MCPI_LOGBUF_INIT(SIZE_BUFFER)                                       \
    _Pragma ("DATA_SECTION(MCPI_logMessage,      \".MCPILogBuffer\")")      \
    char     MCPI_logMessage[SIZE_BUFFER];                                  \
    uint32_t gMCPILogMessageSize = SIZE_BUFFER;

/* MCPI => MCU Connectivety Product Initiative */
/**************************************************************************
 *************************** MCPI Function Prototypes *********************
 **************************************************************************/
extern void MCPI_Initialize();
extern void MCPI_setFeatureTestResult(const char* featureName, MCPI_TestResult result);
extern void MCPI_setTestResult();

#endif /* TEST_LOGGER_H */

