/*
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for Crypto driver
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/family/arm/v7a/Pmu.h>

/* mmWave SK Include Files: */
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/crypto/Crypto.h>
#include <ti/drivers/esm/esm.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/drivers/osal/HwiP.h>

#include "test_vector_aes.h"
#include "test_vector_hmac.h"

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
#define AES_PAD_UP(x, y)   (((x) + ((y) - 1U)) / (y) * (y))

/* Global Variables */
uint8_t                 gCipherText[TEST_AES_MAX_DATA_LEN + 4U];
uint8_t                 gAuthCipherText[TEST_AES_MAX_DATA_LEN + 4U];
uint8_t                 gDigest[CRYPTO_MAX_DIGEST_SIZE];

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**
 *  @b Description
 *  @n
 *      Platform specific intializations.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t PlatformInit(void)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Compares input buffer  adn output buffer for dataLen number of bytes.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t testVerifyData(uint8_t *inputBuffer, uint8_t *outputBuffer, uint32_t dataLen, uint32_t* location)
{
    int32_t         retVal = 0U;
    uint32_t        index;

    for (index = 0U; index < dataLen; index++)
    {
        if (*inputBuffer++ != *outputBuffer++)
        {
            *location = index;
            retVal = -1;
            break;
        }
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Test code to test the AES mode.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t aesModeTest()
{
    Crypto_Handle               cryptoHandle;
    Crypto_AESMode              mode;
    Crypto_EncryptParams        params;
    aesTestVector*              ptrTestVector;
    int32_t                     retVal;
    uint32_t                    location;
    uint32_t                    index = 0;
    uint8_t                     testIV[TEST_AES_IV_SIZE];
    uint8_t                     testTag[16U];
    uint32_t                    paddedDataLen;
    uint32_t                    padAadLength;
    int32_t                     testResult = 0U;

    /* Initialize the Crypto driver */
    Crypto_init();

    /* Open the Crypto driver */
    cryptoHandle = Crypto_open(Crypto_AES);

    if (cryptoHandle == NULL)
    {
        System_printf ("Error: Crypto Driver Open failed\n");
        return -1;
    }

    ptrTestVector = (aesTestVector*) cryptoAesTestCases[index].testData;

    while (ptrTestVector != NULL)
    {
        memset ((void *) &params, 0U, sizeof (Crypto_EncryptParams));
        memset ((void *) &gCipherText[0U], 0U, sizeof (gCipherText));
        memset ((void *) &testTag[0U], 0U, sizeof (testTag));

        /* Call the encrypt */
        mode = ptrTestVector->mode;
        params.ptrKey = (uint8_t*)ptrTestVector->key1;
        params.keySize = ptrTestVector->key1Size;
        memcpy ((void *)&testIV, ptrTestVector->iv, 16U);
        params.ptrIV = (uint8_t*)&testIV[0U];

        /* Pad data for block ciphers */
        if ((mode == Crypto_AES_CBC) || (mode == Crypto_AES_ECB))
        {
            uint32_t    byteCount;

            byteCount = ptrTestVector->dataLen % 16U;
            if (byteCount != 0U)
            {
                paddedDataLen = ptrTestVector->dataLen + (16U - byteCount);
            }
            else
            {
                paddedDataLen = ptrTestVector->dataLen;
            }
        }
        else
        {
            paddedDataLen = ptrTestVector->dataLen;
        }

        /* Handling AES_CTR mode */
        if (mode == Crypto_AES_CTR)
        {
            params.modeParams.u.counterWidth = (Crypto_AesCounterWidth)ptrTestVector->userInput1;
        }

        /* Handling GCM mode */
        if (mode == Crypto_AES_GCM)
        {
            params.aadParams.input.ptrKey2 = (uint8_t*)ptrTestVector->key2;
            params.aadParams.input.key2Size = ptrTestVector->key2Size;
            params.aadParams.input.authLen = ptrTestVector->authLen;
            params.aadParams.tag = (uint8_t*)&testTag[0U];
            params.modeParams.u.gcmMode = (Crypto_AesGcmMode)ptrTestVector->userInput1;
        }

        /* Handling CCM mode */
        if (mode == Crypto_AES_CCM)
        {
            params.aadParams.input.ptrKey2 = (uint8_t*)ptrTestVector->key2;
            params.aadParams.input.key2Size = ptrTestVector->key2Size;
            params.aadParams.input.authLen = ptrTestVector->authLen;
            params.aadParams.tag = (uint8_t*)&testTag[0U];
            params.modeParams.u.ccmParam.lWidth = (Crypto_AesCCM_LWidth)ptrTestVector->userInput1;
            params.modeParams.u.ccmParam.mWidth = (Crypto_AesCCM_MWidth)ptrTestVector->userInput2;
        }

        retVal = Crypto_encrypt (cryptoHandle, mode, (uint8_t*)ptrTestVector->inputText, paddedDataLen, (uint8_t*)&gCipherText[0], &params);
        if (retVal < 0)
        {
            System_printf ("Error: Encrypt for test %d: AES Mode - %s keysize - %s bit, Datalen - %d failed\n",
                            index, aesModeString[mode],
                            ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_128BIT ? "128":  ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_192BIT ? "192": "256",
                            ptrTestVector->dataLen);
            testResult = -1;
        }

        /* Verify the data */
        retVal = testVerifyData ((uint8_t*)&gCipherText[0], (uint8_t*)ptrTestVector->outputText, ptrTestVector->dataLen, &location);
        if (retVal < 0)
        {
            System_printf ("Error: Encrypt Data verification for test %d: AES Mode - %s keysize - %s bit, Datalen - %d at location %d failed\n",
                            index, aesModeString[mode],
                            ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_128BIT ? "128":  ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_192BIT ? "192": "256",
                            ptrTestVector->dataLen, location);
            testResult = -1;
        }

        /* Verify if tag value is correct */
        if ((mode == Crypto_AES_GCM) || (mode == Crypto_AES_CCM))
        {
            retVal = testVerifyData ((uint8_t*)&testTag[0U], (uint8_t*)ptrTestVector->tag, 16U, &location);
            if (retVal < 0)
            {
                System_printf ("Error: Tag Data verification for test %d: AES Mode - %s keysize - %s bit, Datalen - %d at location %d failed\n",
                            index, aesModeString[mode],
                            ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_128BIT ? "128":  ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_192BIT ? "192": "256",
                            ptrTestVector->dataLen, location);
                testResult = -1;
            }
            else
            {
                System_printf ("Debug: Tag Data verification for test %d: AES Mode - %s keysize - %s bit, Datalen - %d passed\n",
                            index, aesModeString[mode],
                            ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_128BIT ? "128":  ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_192BIT ? "192": "256",
                            ptrTestVector->dataLen);
            }
        }

        memcpy ((void *)&testIV, ptrTestVector->iv, 16U);
        params.ptrIV = (uint8_t*)&testIV[0];

        /* Prep the data for combined mode decrypt. AAD, followed by pad, followed by data */
        if ((mode == Crypto_AES_GCM) || (mode == Crypto_AES_CCM))
        {
            memset ((void *) &gAuthCipherText[0U], 0U, sizeof (gAuthCipherText));

            padAadLength = AES_PAD_UP(ptrTestVector->authLen, 16U);
            memcpy ((void *)gAuthCipherText, ptrTestVector->inputText, ptrTestVector->authLen);
            memcpy ((void *)(gAuthCipherText + padAadLength), ptrTestVector->outputText, paddedDataLen);
        }

        /* Call the decrypt function */
        if ((mode == Crypto_AES_GCM) || (mode == Crypto_AES_CCM))
        {
            retVal = Crypto_decrypt (cryptoHandle, mode, (uint8_t*)&gAuthCipherText[0], paddedDataLen, (uint8_t*)&gCipherText[0], &params);
        }
        else
        {
            retVal = Crypto_decrypt (cryptoHandle, mode, (uint8_t*)ptrTestVector->outputText, paddedDataLen, (uint8_t*)&gCipherText[0], &params);
        }

        if (retVal < 0)
        {
            System_printf ("Error: Decrypt for test %d: AES Mode - %s keysize - %s bit, Datalen - %d failed\n",
                            index, aesModeString[mode],
                            ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_128BIT ? "128":  ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_192BIT ? "192": "256",
                            ptrTestVector->dataLen);
            testResult = -1;
        }

        /* Verify the data */
        if ((mode == Crypto_AES_GCM) || (mode == Crypto_AES_CCM))
        {
            retVal = testVerifyData ((uint8_t*)&gCipherText[0U], (uint8_t*)(ptrTestVector->inputText + padAadLength), ptrTestVector->dataLen, &location);
        }
        else
        {
            retVal = testVerifyData ((uint8_t*)&gCipherText[0U], (uint8_t*)ptrTestVector->inputText, ptrTestVector->dataLen, &location);
        }

        if (retVal < 0)
        {
            System_printf ("Error: Decrypt Data verification for test %d: AES Mode - %s keysize - %s bit, Datalen - %d at location %d failed\n",
                            index, aesModeString[mode],
                            ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_128BIT ? "128":  ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_192BIT ? "192": "256",
                            ptrTestVector->dataLen, location);
            testResult = -1;
        }
        else
        {
            System_printf ("Debug: Crypto test %d: AES Mode - %s keysize - %s bit, Datalen - %d passed\n", index, aesModeString[mode],
                            ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_128BIT ? "128":  ptrTestVector->key1Size == Crypto_AES_KEY_SIZE_192BIT ? "192": "256",
                            ptrTestVector->dataLen);
        }

        index++;
        ptrTestVector = (aesTestVector*) cryptoAesTestCases[index].testData;
    }
    /* Close Crypto driver */
    Crypto_close(cryptoHandle);
    return testResult;
}

/**
 *  @b Description
 *  @n
 *      Test code to test the HMAC mode.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t hmacModeTest()
{
    Crypto_Handle               cryptoHandle;
    Crypto_HmacMode             mode;
    Crypto_HmacParams           params;
    hmacTestVector*             ptrTestVector;
    int32_t                     retVal;
    uint32_t                    location;
    uint32_t                    index = 0;

    /* Initialize the Crypto driver */
    Crypto_init();

    /* Open the Crypto driver */
    cryptoHandle = Crypto_open(Crypto_HMAC);

    if (cryptoHandle == NULL)
    {
        System_printf ("Error: Crypto Driver Open failed\n");
        return -1;
    }

    ptrTestVector = (hmacTestVector*) cryptoHmacTestCases[index].testData;

    while (ptrTestVector != NULL)
    {
        if (ptrTestVector == NULL)
            break;

        Crypto_hmacParams_init(&params);
        memset ((void *) &gDigest[0], 0, sizeof (gDigest));

        /* Generate the digest */
        mode = ptrTestVector->mode;
        params.ptrKey = (uint8_t*)ptrTestVector->key;

        params.digestCount = 64U;
        if (mode == Crypto_HMAC_MD5)
            params.ptrKey = NULL;

        retVal = Crypto_sign (cryptoHandle, mode, (uint8_t*)ptrTestVector->inputText, ptrTestVector->dataLen, (uint8_t*)&gDigest[0], &params);
        if (retVal < 0U)
        {
            System_printf ("Error: Crypto sign failed. Test %d: Mode - %s Datalen - %d\n", index, hmacModeString[mode], ptrTestVector->dataLen);
            return -1;
        }

        /* Verify the data */
        retVal = testVerifyData ((uint8_t*)&gDigest[0U], (uint8_t*)ptrTestVector->digest, CRYPTO_MAX_DIGEST_SIZE, &location);
        if (retVal < 0U)
        {
            System_printf ("Error: Digest Data verification failed for Test %d: Mode - %s Datalen - %d at location %d failed\n",
                            index, hmacModeString[mode], ptrTestVector->dataLen, location);
            return -1;
        }
        System_printf ("Debug: Crypto Test %d: Mode - %s Datalen - %d passed\n", index, hmacModeString[mode], ptrTestVector->dataLen);

        index++;
        ptrTestVector = (hmacTestVector*) cryptoHmacTestCases[index].testData;
    }

    /* Close Crypto driver */
    Crypto_close(cryptoHandle);

    return 0U;
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void Test_initTask(UArg arg0, UArg arg1)
{
    int32_t         retVal = 0U;

    /* Initialize the test logger framework: */
    MCPI_Initialize ();

    /* Initialize the platform */
    retVal = PlatformInit();
    if (retVal < 0U)
    {
        MCPI_setFeatureTestResult("Platform initialization failed", MCPI_TestResult_FAIL);
        MCPI_setTestResult ();
        BIOS_exit(0);
    }

    System_printf ("*******************************************************\n");
    System_printf ("                CRYPTO Unit Test Menu                  \n");
    System_printf ("*******************************************************\n");
    System_printf("\nDebug: AES testing\n");
    retVal = aesModeTest();

    if (retVal == -1)
    {
        MCPI_setFeatureTestResult("AES mode testing", MCPI_TestResult_FAIL);
        System_printf("Debug: AES testing failed\n");
    }
    else
    {
        MCPI_setFeatureTestResult("AES mode testing", MCPI_TestResult_PASS);
        System_printf("Debug: AES testing passed\n");
    }

    System_printf("\n\nDebug: HMAC testing\n");
    retVal = hmacModeTest();
    if (retVal == -1)
    {
        MCPI_setFeatureTestResult("HMAC mode testing", MCPI_TestResult_FAIL);
        System_printf("Debug: HMAC testing failed\n");
    }
    else
    {
        MCPI_setFeatureTestResult("HMAC mode testing", MCPI_TestResult_PASS);
        System_printf("Debug: HMAC testing passed\n");
    }

    MCPI_setTestResult ();

    /* Exit BIOS */
    BIOS_exit(0);

    return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t main (void)
{
	Task_Params     taskParams;
    SOC_Handle      socHandle;
    ESM_Handle      esmHandle;
    SOC_Cfg         socCfg;
    int32_t         errCode;

    /* Initialize the ESM: Dont clear errors as TI RTOS does it */
    esmHandle = ESM_init(0U);

    /* Initialize the SOC confiugration: */
    memset ((void *)&socCfg, 0, sizeof(SOC_Cfg));

    /* Populate the SOC configuration: */
    socCfg.clockCfg = SOC_SysClock_INIT;

    /* Initialize the SOC Module: This is done as soon as the application is started
     * to ensure that the MPU is correctly configured. */
    socHandle = SOC_init (&socCfg, &errCode);
    if (socHandle == NULL)
    {
        System_printf ("Error: SOC Module Initialization failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Check if the SOC is a secure device */
    if (SOC_isSecureDevice(socHandle, &errCode))
    {
        /* Disable firewall for crypto module */
        SOC_controlSecureFirewall(socHandle, 
                                  (uint32_t)(SOC_SECURE_FIREWALL_CRYPTO),
                                  SOC_SECURE_FIREWALL_DISABLE,
                                  &errCode);
    }
    else
    {
        /* non secure device */
        System_printf ("Error: Crypto functionality not supported on non secure device !!!");
 
        /* cleanup state */
        SOC_deinit(socHandle, &errCode);
        ESM_close(esmHandle);
        return -1;
    }

    /* Initialize the Task Parameters. */
    Task_Params_init(&taskParams);
    taskParams.stackSize = 6*1024;
    Task_create(Test_initTask, &taskParams, NULL);

    /* Start BIOS */
	BIOS_start();
    return 0;
}


