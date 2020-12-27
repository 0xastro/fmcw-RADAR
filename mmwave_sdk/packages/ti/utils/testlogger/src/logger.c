/*
 *   @file  logger.c
 *
 *   @brief
 *      MCPI Test Logger
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ti/utils/testlogger/logger.h>

/* MCPI => MCU Connectivety Product Initiative */
/**************************************************************************
 *************************** MCPI Global Variables ************************
 **************************************************************************/
/**
 *  @var    MCPI_Result
 *  @brief  Variable to store Test Result
 *          MCPI script uses it to determine Pass/Fail for the testcase.
 *
 *  @var    MCPI_FailSignature
 *  @brief  Variable to store Test Failure Signature
 *          User can decide how he/she wants to use it.
 *          MCPI script will print it in test log file.
 *
 *  @var    MCPI_logMessage[BUF_SIZE]
 *  @brief  Buffer to store Test Log Message.
 *          MCPI script will print the it in test log file.
 */
uint8_t  MCPI_Result;
int      MCPI_FailSignature;

extern char         MCPI_logMessage[];
extern uint32_t     gMCPILogMessageSize;


/**************************************************************************
 *************************** MCPI Function Prototypes *********************
 **************************************************************************/


/*************************************************************************
 * @brief  This function initializes Test result variables and log buffer.
 *
 * @return Void
*************************************************************************/
void MCPI_Initialize()
{
    MCPI_Result = MCPI_TestResult_FAIL; // initialize result to fail
    MCPI_FailSignature = 0;             // initialize Failure signature to zero
    strcpy((char*)MCPI_logMessage, ""); // initialize log message buffer to empty string
    return;
} /* end of function MCPI_Initialize() */


/*************************************************************************
 * @brief  This function must be called after every feature test done.
 *         This function 1) Set MCPI_Result = FAIL if the feature test is not pass
 *                       2) Print the feature name and test result in console.
 *                       3) Append the feature name and test result to the end 
 *                          of MCPI_logMessage buffer with buffer overflow protection.
 *
 * @return Void
*************************************************************************/
void MCPI_setFeatureTestResult(const char* featureName, MCPI_TestResult result)
{
    char tempFeaturename[170];//hold the overflow protected feature name       
    char tempMsg[200];        //hold the overflow protected message 
    static uint32_t logMessageLen = 0;
        
    /* Copy feature name with overflow protection */
    strncpy(tempFeaturename, featureName, 170);
        
    if (result == MCPI_TestResult_PASS)
    {
        /* Print in console */
        printf ("Feature: %s: Passed\n", featureName);
        /* Assemble the message to be print in logMessage with overflow protection */
        snprintf (tempMsg, 200, "%s%s%s", "\nFeature tested: ", tempFeaturename, ": Passed");
    }
    else
    {
        MCPI_FailSignature++;

        /* Print in console */
        printf ("Feature: %s: Failed\n", featureName);
        /* Assemble the message to be print in logMessage with overflow protection */
        snprintf (tempMsg, 200, "%s%s%s", "\nFeature tested: ", tempFeaturename, ": Failed");
    }

    logMessageLen += strlen(tempMsg);

    if (logMessageLen < gMCPILogMessageSize)
    {
        /* Append the message to end of MCPI_logMessage buffer */
	strcpy(MCPI_logMessage, strcat(MCPI_logMessage, tempMsg));
    }   
    else
    {
        /* Print overflow error message in console */
        printf ("Error: MCPI_logMessage buffer overflow logMessageLen = %d\n", logMessageLen);

        /* TODO: stop the test */
    } 
 
    return;

} /* end of function MCPI_setFeatureTestResult() */


/*************************************************************************
 * @brief  This function must be called right before BIOS_exit(0) i.e after all feature tests done.
 *         This function set MCPI_Result = PASS only if all feature tests pass.
 *                      
 * @return Void
*************************************************************************/
void MCPI_setTestResult()
{    
    if (MCPI_FailSignature == 0)
    {
        MCPI_Result = MCPI_TestResult_PASS;
    }
    else
    {
        MCPI_Result = MCPI_TestResult_FAIL;
    }
    
    return;

} /* end of function MCPI_TestResult() */

