/**
 *   @file  Crypto.c
 *
 *   @brief
 *      The file implements the Crypto Driver for the XWR16xx/XWR18xx.
 *      The file implements the driver to conform to the MCPI standards.
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
#include <stdint.h>
#include <string.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>
#include <ti/common/hw_types.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/crypto/include/crypto_internal.h>

/**************************************************************************
 ****************************** Defines ***********************************
 **************************************************************************/
#define AES_PAD_UP(x, y)   (((x) + ((y) - 1U)) / (y) * (y))

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern Crypto_Config Crypto_config[];

/**************************************************************************
 ************************* Internal functions *****************************
 **************************************************************************/
static uint32_t Crypto_getAesConfig(Crypto_AESMode mode, Crypto_EncryptParams* ptrParams);
static int32_t Crypto_aesProcess(Crypto_Config* ptrCryptoConfig, Crypto_AESMode mode,
                uint32_t cryptoDirection, uint8_t* ptrInput, uint32_t dataLength,
                uint8_t* ptrOutput, Crypto_EncryptParams* ptrParams);
static int32_t Crypto_hmacProcess(Crypto_Config* ptrCryptoConfig, uint32_t cryptoConfig, uint8_t* ptrInput,
                uint32_t dataLength, uint8_t* ptrSignature, Crypto_HmacParams *ptrParams);

/**
 *  @b Description
 *  @n
 *      The function figures out the AES module configuration based on the crypto mode.
 *
 *  @param[in]  mode
 *      AES crypto mode.
 *  @param[in] ptrParams
 *      Pointer AES encrypt parameters.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   AES module configuration values
 *  @retval
 *      Error   -   AES_CFG_MODE_INVALID
 */
static uint32_t Crypto_getAesConfig(Crypto_AESMode mode, Crypto_EncryptParams* ptrParams)
{
    uint32_t                cryptoConfig;

    switch (mode)
    {
        case (Crypto_AES_ECB):
        {
            cryptoConfig = AES_CFG_MODE_ECB;
            break;
        }
        case (Crypto_AES_CBC):
        {
            cryptoConfig = AES_CFG_MODE_CBC;
            break;
        }
        case (Crypto_AES_CTR):
        {
            switch (ptrParams->modeParams.u.counterWidth)
            {
                case(Crypto_AES_COUNTER_SIZE_32BIT):
                {
                    cryptoConfig = AES_CFG_MODE_CTR_32;
                    break;
                }
                case(Crypto_AES_COUNTER_SIZE_64BIT):
                {
                    cryptoConfig = AES_CFG_MODE_CTR_64;
                    break;
                }
                case(Crypto_AES_COUNTER_SIZE_96BIT):
                {
                    cryptoConfig = AES_CFG_MODE_CTR_96;
                    break;
                }
                case(Crypto_AES_COUNTER_SIZE_128BIT):
                {
                    cryptoConfig = AES_CFG_MODE_CTR_128;
                    break;
                }
                default:
                {
                    /* Option is NOT supported */
                    cryptoConfig = AES_CFG_MODE_INVALID;
                    break;
                }
            }
            break;
        }
        case (Crypto_AES_ICM):
        {
            cryptoConfig = AES_CFG_MODE_ICM;
            break;
        }
        case (Crypto_AES_CFB):
        {
            cryptoConfig = AES_CFG_MODE_CFB;
            break;
        }
        case (Crypto_AES_GCM):
        {
            switch (ptrParams->modeParams.u.gcmMode)
            {
                case(Crypto_AES_GCM_HLY0ZERO):
                {
                    cryptoConfig = AES_CFG_MODE_GCM_HLY0ZERO;
                    break;
                }
                case(Crypto_AES_GCM_HLY0CALC):
                {
                    cryptoConfig = AES_CFG_MODE_GCM_HLY0CALC;
                    break;
                }
                case(Crypto_AES_GCM_HY0CALC):
                {
                    cryptoConfig = AES_CFG_MODE_GCM_HY0CALC;
                    break;
                }
                default:
                {
                    /* Option is NOT supported */
                    cryptoConfig = AES_CFG_MODE_INVALID;
                    break;
                }
            }
            break;
        }
        case (Crypto_AES_CCM):
        {
            cryptoConfig = AES_CFG_MODE_CCM;
            switch (ptrParams->modeParams.u.ccmParam.lWidth)
            {
                case(Crypto_AES_CCM_L_2):
                {
                    cryptoConfig |= AES_CFG_CCM_L_2;
                    break;
                }
                case(Crypto_AES_CCM_L_4):
                {
                    cryptoConfig |= AES_CFG_CCM_L_4;
                    break;
                }
                case(Crypto_AES_CCM_L_8):
                {
                    cryptoConfig |= AES_CFG_CCM_L_8;
                    break;
                }
                default:
                {
                    /* Option is NOT supported */
                    cryptoConfig = AES_CFG_MODE_INVALID;
                    break;
                }
            }
            switch (ptrParams->modeParams.u.ccmParam.mWidth)
            {
                case(Crypto_AES_CCM_M_4):
                {
                    cryptoConfig |= AES_CFG_CCM_M_4;
                    break;
                }
                case(Crypto_AES_CCM_M_6):
                {
                    cryptoConfig |= AES_CFG_CCM_M_6;
                    break;
                }
                case(Crypto_AES_CCM_M_8):
                {
                    cryptoConfig |= AES_CFG_CCM_M_8;
                    break;
                }
                case(Crypto_AES_CCM_M_10):
                {
                    cryptoConfig |= AES_CFG_CCM_M_10;
                    break;
                }
                case(Crypto_AES_CCM_M_12):
                {
                    cryptoConfig |= AES_CFG_CCM_M_12;
                    break;
                }
                case(Crypto_AES_CCM_M_14):
                {
                    cryptoConfig |= AES_CFG_CCM_M_14;
                    break;
                }
                case(Crypto_AES_CCM_M_16):
                {
                    cryptoConfig |= AES_CFG_CCM_M_16;
                    break;
                }
                default:
                {
                    /* Option is NOT supported */
                    cryptoConfig = AES_CFG_MODE_INVALID;
                    break;
                }
            }
            break;
        }
        default:
        {
            /* Option is NOT supported */
            cryptoConfig = AES_CFG_MODE_INVALID;
            break;
        }
    }
    return cryptoConfig;
}

/**
 *  @b Description
 *  @n
 *      The function is called to encrypt, decrypt or authenticate data based on a specified AES mode.
 *      The additional result is stored in the encrypt output parameters.
 *
 *  @param[in]  ptrCryptoConfig
 *      Pointer to the Crypto Driver configuration
 *  @param[in]  mode
 *      AES encrypt mode.
 *  @param[in]  cryptoDirection
 *      Direction of crypto - Encrypt or Decrypt.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[in]  dataLength
 *      Data length in bytes. Valid values are 1 to less than 2^32.
 *  @param[out] ptrOutput
 *      Pointer to the output data buffer.
 *  @param[in] ptrParams
 *      Pointer AES parameters. This structure contains both input and output parameters based on the mode.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
static int32_t Crypto_aesProcess(Crypto_Config* ptrCryptoConfig, Crypto_AESMode mode, uint32_t cryptoDirection, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrOutput, Crypto_EncryptParams* ptrParams)
{
    Crypto_HwCfg*           ptrHwCfg;
    int32_t                 retVal = 0U;
    uint32_t                cryptoConfig;
    uint32_t                padAadLength;

    /* Get the hardware configuration: */
    ptrHwCfg = (Crypto_HwCfg*)ptrCryptoConfig->hwAttrs;

    /* Figure out the AES module configuration based on the crypto mode */
    cryptoConfig = Crypto_getAesConfig(mode, ptrParams);
    if (cryptoConfig == AES_CFG_MODE_INVALID)
    {
        retVal = CRYPTO_EINVAL;
        goto endProcess;
    }

    /* Configure the AES module with direction (encryption or decryption), key size etc. */
    AES_config(ptrHwCfg->ptrAesBase, cryptoDirection | cryptoConfig | Crypto_getAesKeySize(ptrParams->keySize));

    switch (mode)
    {
        /* Crypto mode ECB */
        case (Crypto_AES_ECB):
        {
            /* Write key1. */
            AES_setKey1(ptrHwCfg->ptrAesBase, (uint8_t *)ptrParams->ptrKey, Crypto_getAesKeySize(ptrParams->keySize));

            /* Start the crypto process */
            AES_processData(ptrHwCfg->ptrAesBase, ptrInput, ptrOutput, dataLength);
            break;
        }

        /* Crypto mode CBC, CFB, AES_CTR */
        case (Crypto_AES_CBC):
        case (Crypto_AES_CFB):
        case (Crypto_AES_CTR):
        case (Crypto_AES_ICM):
        {
            /* Set the initialization vector */
            AES_setIV(ptrHwCfg->ptrAesBase, ptrParams->ptrIV);

            /* Write key1. */
            AES_setKey1(ptrHwCfg->ptrAesBase, (uint8_t *)ptrParams->ptrKey, Crypto_getAesKeySize(ptrParams->keySize));

            /* Start the crypto process */
            AES_processData(ptrHwCfg->ptrAesBase, ptrInput, ptrOutput, dataLength);

            /* Get the initialization vector */
            AES_getIV(ptrHwCfg->ptrAesBase, ptrParams->ptrIV);
            break;
        }

        /* Crypto mode GCM */
        case (Crypto_AES_GCM):
        {
            /* Set the initialization vector */
            AES_setIV(ptrHwCfg->ptrAesBase, ptrParams->ptrIV);

            /* Write key1. */
            AES_setKey1(ptrHwCfg->ptrAesBase, (uint8_t *)ptrParams->ptrKey, Crypto_getAesKeySize(ptrParams->keySize));

            /* Write key2 if H is loaded. */
            if ((ptrParams->modeParams.u.gcmMode == Crypto_AES_GCM_HLY0ZERO) || (ptrParams->modeParams.u.gcmMode == Crypto_AES_GCM_HLY0CALC))
            {
                AES_setKey2(ptrHwCfg->ptrAesBase, ptrParams->aadParams.input.ptrKey2, Crypto_getAesKeySize(ptrParams->aadParams.input.key2Size));
            }

            /* Start the crypto process */
            /* Check if AAD was padded, If so skip the padding */
            padAadLength = AES_PAD_UP(ptrParams->aadParams.input.authLen, 16U);
            AES_processDataAE(ptrHwCfg->ptrAesBase, (uint8_t *)(ptrInput + padAadLength), ptrOutput, dataLength,
                        ptrInput, ptrParams->aadParams.tag, ptrParams->aadParams.input.authLen);

            /* Get the initialization vector */
            AES_getIV(ptrHwCfg->ptrAesBase, ptrParams->ptrIV);
            break;
        }

        /* Crypto mode CCM */
        case (Crypto_AES_CCM):
        {
            /* Set the initialization vector */
            AES_setIV(ptrHwCfg->ptrAesBase, ptrParams->ptrIV);

            /* Write key1. */
            AES_setKey1(ptrHwCfg->ptrAesBase, (uint8_t *)ptrParams->ptrKey, Crypto_getAesKeySize(ptrParams->keySize));

            /* Write key1. */
            AES_setKey2(ptrHwCfg->ptrAesBase, ptrParams->aadParams.input.ptrKey2, Crypto_getAesKeySize(ptrParams->aadParams.input.key2Size));

            /* Start the crypto process */
            /* Check if AAD was padded, If so skip the padding */
            padAadLength = AES_PAD_UP(ptrParams->aadParams.input.authLen, 16U);
            AES_processDataAE(ptrHwCfg->ptrAesBase, (uint8_t *)(ptrInput + padAadLength), ptrOutput, dataLength,
                        ptrInput, ptrParams->aadParams.tag, ptrParams->aadParams.input.authLen);

            /* Get the initialization vector */
            AES_getIV(ptrHwCfg->ptrAesBase, ptrParams->ptrIV);
            break;
        }
        default:
        {
            /* Option is NOT supported */
            retVal = CRYPTO_EINVAL;
            break;
        }
    }

endProcess:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the Crypto driver close function called close a
 *      Crypto Driver instance is being closed
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
int32_t Crypto_close(Crypto_Handle handle)
{
    Crypto_Config*        ptrCryptoConfig;
    Crypto_MCB*           ptrCryptoMCB;
    int32_t               retVal = 0U;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if (handle == NULL)
    {
        retVal = CRYPTO_EINVAL;
    }
    else
    {
        /* Get the Crypto Configuration: */
        ptrCryptoConfig = (Crypto_Config*)handle;

        /* Get the Crypto Driver Object */
        ptrCryptoMCB = (Crypto_MCB*)ptrCryptoConfig->object;

        if (ptrCryptoMCB == NULL)
        {
            retVal = CRYPTO_EINVAL;
        }
        else
        {
            /* Clean up the semaphore */
            if (ptrCryptoMCB->sem)
            {
                /* Delete the semaphore. */
                SemaphoreP_delete(ptrCryptoMCB->sem);
            }

            MemoryP_ctrlFree (ptrCryptoMCB, sizeof(Crypto_MCB));

            /* Mark the object as closed */
            ptrCryptoConfig->object = NULL;
        }
    }
    return retVal;
}


/**
 *  @b Description
 *  @n
 *      The function is the Crypto driver initialization function.
 *
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void Crypto_init(void)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Crypto Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  cryptoType
 *      Type of Crypto instance that is opened. Valid values are
 *          Crypto_AES  - AES Crypto engine
 *          Crypto_HMAC - HMAC Crypto engine
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
Crypto_Handle Crypto_open(Crypto_Type cryptoType)
{
    Crypto_Handle           retHandle = NULL;
    Crypto_Config*          ptrCryptoConfig;
    Crypto_MCB*             ptrCryptoMCB;
    Crypto_HwCfg*           ptrHwCfg;
    SemaphoreP_Params       semParams;
    uint32_t                index;

    /* Get the Crypto Configuration for instance "type" */
    if (cryptoType == Crypto_AES)
    {
        index = 0U;
    }
    else
    {
        index = 1U;
    }

    ptrCryptoConfig = (Crypto_Config*)&Crypto_config[index];

    if (ptrCryptoConfig->object != NULL)
    {
        /* Instance was previously opened. Return error */
        retHandle = NULL;
    }
    else
    {
        /* Get the hardware configuration: */
        ptrHwCfg = (Crypto_HwCfg*)ptrCryptoConfig->hwAttrs;

        /* Allocate memory for the driver */
        ptrCryptoMCB = MemoryP_ctrlAlloc ((uint32_t)sizeof(Crypto_MCB), 0);
        if (ptrCryptoMCB == NULL)
        {
            /* Error: Out of memory */
            retHandle = NULL;
        }
        else
        {
            /* Initialize the memory: */
            memset ((void *)ptrCryptoMCB, 0, sizeof(Crypto_MCB));

            /* Store the object handle */
            ptrCryptoMCB->type = cryptoType;
            ptrCryptoConfig->object = ptrCryptoMCB;

            /* Setup the return handle: */
            retHandle = (Crypto_Handle)ptrCryptoConfig;
        }
    }

    if (retHandle != NULL)
    {
        /* Create the semaphore */
        SemaphoreP_Params_init(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;

        ptrCryptoMCB->sem = SemaphoreP_create(1, &semParams);
        if (ptrCryptoMCB->sem == NULL)
        {
            MemoryP_ctrlFree (ptrCryptoMCB, sizeof(Crypto_MCB));
            retHandle = NULL;
        }
        else
        {
            /* Initialize the module */
            if (cryptoType == Crypto_AES)
            {
                AES_init(ptrHwCfg->ptrAesBase);
            }
            else
            {
                HMAC_init(ptrHwCfg->ptrDtheBase, ptrHwCfg->ptrHmacBase);
            }
        }
    }

    /* Return the address of the handle */
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is initializes the HMAC default parameters.
 *
 *  @param[in]  params
 *      Pointer to HMAC  params.
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
int32_t Crypto_hmacParams_init(Crypto_HmacParams *params)
{
    int32_t         retVal = 0U;

    if (params == NULL)
    {
        retVal = CRYPTO_EINVAL;
    }
    else
    {
        memset(params, 0U, sizeof(Crypto_HmacParams));
        params->first = 1U;

        /* All supported hashing algorithms block size are equal, set one as default */
        params->blockSize = CRYPTO_SHA256_BLOCK_SIZE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is called to encrypt plain text data using a specific AES mode. Depending on the
 *      mode the data is encrypted and or authenticated. The additional result is stored in the
 *      encrypt output parameters.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver
 *  @param[in]  mode
 *      AES encrypt mode.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[in]  dataLength
 *      Data length in bytes. Valid values are 1 to less than 2^32.
 *  @param[out] ptrOutput
 *      Pointer to the output data buffer.
 *  @param[in] ptrParams
 *      Pointer AES parameters. This structure contains both input and output parameters based on the mode.
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
int32_t Crypto_encrypt(Crypto_Handle handle, Crypto_AESMode mode, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrOutput, Crypto_EncryptParams* ptrParams)
{
    Crypto_Config*          ptrCryptoConfig;
    Crypto_MCB*             ptrCryptoMCB;
    int32_t                 retVal = 0U;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (ptrInput == NULL) || (ptrOutput == NULL) || (ptrParams == NULL))
    {
        retVal = CRYPTO_EINVAL;
    }
    else
    {
        /* Get the Crypto Configuration: */
        ptrCryptoConfig = (Crypto_Config*)handle;

        /* Get the Crypto Driver Object */
        ptrCryptoMCB = (Crypto_MCB*)ptrCryptoConfig->object;

        if (ptrCryptoMCB == NULL)
        {
            retVal = CRYPTO_EINVAL;
        }
        else if (ptrCryptoMCB->type != Crypto_AES)
        {
            retVal = CRYPTO_EINVAL;
        }
        else
        {
            /* Get the semaphore */
            SemaphoreP_pend(ptrCryptoMCB->sem, SemaphoreP_WAIT_FOREVER);
            {
                retVal = Crypto_aesProcess(ptrCryptoConfig, mode, AES_CFG_DIR_ENCRYPT, ptrInput, dataLength, ptrOutput, ptrParams);

                /* release the semaphore */
                SemaphoreP_post(ptrCryptoMCB->sem);
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is called to decrypt ciphered text data using a specific AES mode. Depending on the
 *      mode the data is encrypted and or authenticated. The additional result is stored in the
 *      encrypt output parameters.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver
 *  @param[in]  mode
 *      AES encrypt mode.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[in]  dataLength
 *      Data length in bytes. Valid values are 1 to less than 2^32.
 *  @param[out] ptrOutput
 *      Pointer to the output data buffer.
 *  @param[in] ptrParams
 *      Pointer AES parameters. This structure contains both input and output parameters based on the mode.
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
int32_t Crypto_decrypt(Crypto_Handle handle, Crypto_AESMode mode, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrOutput, Crypto_EncryptParams* ptrParams)
{
    Crypto_Config*          ptrCryptoConfig;
    Crypto_MCB*             ptrCryptoMCB;
    int32_t                 retVal = 0U;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (ptrInput == NULL) || (ptrOutput == NULL) || (ptrParams == NULL))
    {
        retVal = CRYPTO_EINVAL;
    }
    else
    {
        /* Get the Crypto Configuration: */
        ptrCryptoConfig = (Crypto_Config*)handle;

        /* Get the Crypto Driver Object */
        ptrCryptoMCB = (Crypto_MCB*)ptrCryptoConfig->object;

        if (ptrCryptoMCB == NULL)
        {
            retVal = CRYPTO_EINVAL;
        }
        else if (ptrCryptoMCB->type != Crypto_AES)
        {
            retVal = CRYPTO_EINVAL;
        }
        else
        {
            /* Get the semaphore */
            SemaphoreP_pend(ptrCryptoMCB->sem, SemaphoreP_WAIT_FOREVER);
            {
                retVal = Crypto_aesProcess(ptrCryptoConfig, mode, AES_CFG_DIR_DECRYPT, ptrInput, dataLength, ptrOutput, ptrParams);

                /* release the semaphore */
                SemaphoreP_post(ptrCryptoMCB->sem);
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to process the HMAC params and configure the HMAC engine. It sets the HMAC key,
 *      processes the input data and produces the hash.
 *
 *  @param[in]  ptrCryptoConfig
 *      Pointer to the crypto config block
 *  @param[in]  cryptoConfig
 *      HMAC hash algorithm.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[in]  dataLength
 *      Data length in bytes.
 *  @param[in] ptrSignature
 *      Pointer to the HMAC Hash.
 *          input pointer - If HMAC flag was set, the ptrSignature pointer is used as the input HMAC Hash value.
 *          output pointer - For the generated Hash value.
 *  @param[in] ptrParams
 *      Pointer HMAC parameters.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
static int32_t Crypto_hmacProcess(Crypto_Config* ptrCryptoConfig, uint32_t cryptoConfig, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrSignature, Crypto_HmacParams *ptrParams)
{
    Crypto_HwCfg*           ptrHwCfg;
    uint32_t                newLength = 0U;
    uint32_t                copyLength = 0U;
    uint32_t                totalLength = 0U;
    uint32_t                blockComplementLength = 0U;
    uint32_t                blockRemainder = 0U;
    int32_t                 retVal = 0U;
    uint8_t                 useAlgoConstant;
    uint8_t                 closeHash;
    uint8_t                 useHMACKey;
    uint8_t                 HMACOuterHash;

    /* Get the hardware configuration: */
    ptrHwCfg = (Crypto_HwCfg*)ptrCryptoConfig->hwAttrs;

    if (ptrParams->moreData == 1U)
    {
        /* Less than block size available. Copy the received data to the internal buffer and return. */
        if((ptrParams->bufferLength + dataLength) < ptrParams->blockSize)
        {
            memcpy(&ptrParams->buffer[ptrParams->bufferLength], ptrInput, dataLength);
            ptrParams->bufferLength += dataLength;
            goto endProcess;
        }

        if(ptrParams->first)
        {
            /* If Keyed Hashing is used, set Key */
            if (ptrParams->ptrKey != NULL)
            {
                HMAC_setHMACKey(ptrHwCfg->ptrHmacBase, ptrParams->ptrKey);

                /* Set the HMAC digest count */
                HMAC_writeDigestCount(ptrHwCfg->ptrHmacBase, 64U);

                /* Compute the mode values */
                useAlgoConstant = 0;
                closeHash = 0;
                useHMACKey = 1;
                HMACOuterHash = 0;

                /* Configure the HMAC algorithm and mode */
                HMAC_setConfig(ptrHwCfg->ptrHmacBase, cryptoConfig, useAlgoConstant, closeHash, useHMACKey, HMACOuterHash);
            }
            else
            {
                /* Compute the mode values */
                useAlgoConstant = 1;
                closeHash = 0;
                useHMACKey = 0;
                HMACOuterHash = 0;

                /* Configure the HMAC algorithm and mode */
                HMAC_setConfig(ptrHwCfg->ptrHmacBase, cryptoConfig, useAlgoConstant, closeHash, useHMACKey, HMACOuterHash);
            }

            /* Will write data in this iteration. Set first to zero */
            ptrParams->first = 0;
        }
        else
        {
            /* Compute the mode values */
            useAlgoConstant = 0;
            closeHash = 0;
            useHMACKey = 0;
            HMACOuterHash = 0;

            /* Configure the HMAC algorithm and mode */
            HMAC_setConfig(ptrHwCfg->ptrHmacBase, cryptoConfig, useAlgoConstant, closeHash, useHMACKey, HMACOuterHash);

            /* Write the intermediate digest and count in case it isn't the first round */
            HMAC_writeResult(ptrHwCfg->ptrHmacBase, ptrParams->innerDigest);
            HMAC_writeDigestCount(ptrHwCfg->ptrHmacBase, ptrParams->digestCount);
        }
    }
    else
    {
        if(ptrParams->first)
        {
            /* If Keyed Hashing is used, set Key */
            if (ptrParams->ptrKey != NULL)
            {
                /* Set the HMAC key */
                HMAC_setHMACKey(ptrHwCfg->ptrHmacBase, ptrParams->ptrKey);

                /* Set the HMAC digest count */
                HMAC_writeDigestCount(ptrHwCfg->ptrHmacBase, 64U);

                /* Compute the mode values */
                useAlgoConstant = 0;
                closeHash = 1;
                useHMACKey = 1;
                HMACOuterHash = 1;

                /* Configure the HMAC algorithm and mode */
                HMAC_setConfig(ptrHwCfg->ptrHmacBase, cryptoConfig, useAlgoConstant, closeHash, useHMACKey, HMACOuterHash);
            }
            else
            {
                /* Compute the mode values */
                useAlgoConstant = 1;
                closeHash = 1;
                useHMACKey = 0;
                HMACOuterHash = 0;

                /* Configure the HMAC algorithm and mode */
                HMAC_setConfig(ptrHwCfg->ptrHmacBase, cryptoConfig, useAlgoConstant, closeHash, useHMACKey, HMACOuterHash);
            }
        }
        else
        {
            /* Write the intermediate digest and count in case it isn't the first round */
            HMAC_writeResult(ptrHwCfg->ptrHmacBase, ptrParams->innerDigest);
            HMAC_writeDigestCount(ptrHwCfg->ptrHmacBase, ptrParams->digestCount);

            if (ptrParams->ptrKey != NULL)
            {
                /* Compute the mode values */
                useAlgoConstant = 0;
                closeHash = 1;
                useHMACKey = 0;
                HMACOuterHash = 1;

                /* Configure the HMAC algorithm and mode */
                HMAC_setConfig(ptrHwCfg->ptrHmacBase, cryptoConfig, useAlgoConstant, closeHash, useHMACKey, HMACOuterHash);
            }
            else
            {
                /* Compute the mode values */
                useAlgoConstant = 0;
                closeHash = 1;
                useHMACKey = 0;
                HMACOuterHash = 0;

                /* Configure the HMAC algorithm and mode */
                HMAC_setConfig(ptrHwCfg->ptrHmacBase, cryptoConfig, useAlgoConstant, closeHash, useHMACKey, HMACOuterHash);
            }

            /* This is the last iteration for the requested calculation */
            /* Set the first flag to 1 (initial value) for next calculations */
            ptrParams->first = 1;
        }
    }

    /* Set the length of the data that will be written to the SHA module in this iteration */
    /* In case it isn't the last iteration, it has to be an integer multiple of block size */
    if (ptrParams->moreData)
    {
        totalLength = ((ptrParams->bufferLength + dataLength) / ptrParams->blockSize) * ptrParams->blockSize;
    }
    else
    {
        totalLength = ptrParams->bufferLength + dataLength;
    }
    HMAC_setDataLength(ptrHwCfg->ptrHmacBase, totalLength);

    /* In case there is data in the internal buffer, complement to a block size (if the length is sufficient) and write */
    if (ptrParams->bufferLength)
    {
        blockComplementLength = ptrParams->blockSize - ptrParams->bufferLength;

        /* Copy to the internal buffer */
        /* The length to copy is the minimum between the block complement and the data length */
        copyLength = dataLength < blockComplementLength ? dataLength : blockComplementLength;
        memcpy(&ptrParams->buffer[ptrParams->bufferLength], ptrInput, copyLength);
        ptrParams->bufferLength += copyLength;

        /* If bufferLength is smaller than block size this must be the last iteration */
        /* Write and set the buffer and buffer length to zero */
        HMAC_writeDataMultiple(ptrHwCfg->ptrHmacBase, ptrParams->buffer, ptrParams->bufferLength);

        ptrParams->bufferLength = 0U;
        memset(ptrParams->buffer, 0U, sizeof(ptrParams->buffer));
    }

    /* If data length is greater than block complement, write the rest of the data */
    if (dataLength > blockComplementLength)
    {
        newLength = dataLength - blockComplementLength;
        ptrInput += blockComplementLength;
    }

    if (ptrParams->moreData)
    {
        /* Remaining length is smaller than block size - Save data to the internal buffer */
        if (newLength < ptrParams->blockSize)
        {
            memcpy(&ptrParams->buffer[ptrParams->bufferLength], ptrInput, newLength);
            ptrParams->bufferLength = newLength;
        }
        /* Remaining length is greater than block size - write blocks and save remainder to the internal buffer */
        else
        {
            blockRemainder = newLength % ptrParams->blockSize;
            newLength -=  blockRemainder;
            HMAC_writeDataMultiple(ptrHwCfg->ptrHmacBase, ptrInput, newLength);

            if (blockRemainder)
            {
                ptrInput += newLength;
                memcpy(&ptrParams->buffer[ptrParams->bufferLength], ptrInput, blockRemainder);
                ptrParams->bufferLength += blockRemainder;
            }
        }
    }
    else
    {
        /* Last iteration, write all the data */
        if (newLength)
        {
            HMAC_writeDataMultiple(ptrHwCfg->ptrHmacBase, ptrInput, newLength);
        }
    }

    /* Wait for the output to be ready */
    while ((HMAC_isReady (ptrHwCfg->ptrHmacBase, HMAC_INT_OUTPUT_READY)) == 0U);

    /* Read the result */
    if (ptrParams->moreData == 1U)
    {
        /* Read the digest and count to an internal parameter (will be used in the next iteration) */
        HMAC_readResult(ptrHwCfg->ptrHmacBase, ptrParams->innerDigest);
        ptrParams->digestCount = HMAC_readDigestCount(ptrHwCfg->ptrHmacBase);
    }
    else
    {
        /* Read the final digest result to an outer pointer */
        HMAC_readResult(ptrHwCfg->ptrHmacBase, ptrSignature);
    }
endProcess:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is called to generate the HMAC Hash value of given plain Text.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver
 *  @param[in]  mode
 *      HMAC hash algorithm.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[in]  dataLength
 *      Data length in bytes.
 *  @param[in] ptrSignature
 *      Pointer to the HMAC Hash.
 *          input pointer - If HMAC flag was set, the ptrSignature pointer is used as the input HMAC Hash value.
 *          output pointer - For the generated Hash value.
 *  @param[in] ptrParams
 *      Pointer HMAC parameters.
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
int32_t Crypto_sign(Crypto_Handle handle, Crypto_HmacMode mode , uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrSignature, Crypto_HmacParams* ptrParams)
{
    Crypto_Config*          ptrCryptoConfig;
    Crypto_MCB*             ptrCryptoMCB;
    int32_t                 retVal = 0U;
    uint32_t                cryptoConfig;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (ptrInput == NULL) || (ptrSignature == NULL) || (ptrParams == NULL))
    {
        retVal = CRYPTO_EINVAL;
    }
    else
    {
        /* Get the Crypto Configuration: */
        ptrCryptoConfig = (Crypto_Config*)handle;

        /* Get the Crypto Driver Object */
        ptrCryptoMCB = (Crypto_MCB*)ptrCryptoConfig->object;

        if (ptrCryptoMCB == NULL)
        {
            retVal = CRYPTO_EINVAL;
        }
        else if (ptrCryptoMCB->type != Crypto_HMAC)
        {
            retVal = CRYPTO_EINVAL;
        }
        else
        {
            /* Get the semaphore */
            SemaphoreP_pend(ptrCryptoMCB->sem, SemaphoreP_WAIT_FOREVER);
            {
                /* Translate the crypto mode to the underlying HMAC register level driver mode */
                cryptoConfig = Crypto_getHmacConfig((uint32_t)mode);

                retVal = Crypto_hmacProcess(ptrCryptoConfig, cryptoConfig, ptrInput, dataLength, ptrSignature, ptrParams);

                /* release the semaphore */
                SemaphoreP_post(ptrCryptoMCB->sem);
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is called to verify the HMAC Hash value of given input Text.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver
 *  @param[in]  mode
 *      HMAC hash algorithm.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[in]  dataLength
 *      Data length in bytes.
 *  @param[in] ptrSignature
 *      Pointer to the HMAC Hash.
 *          input pointer - If HMAC flag was set, the ptrSignature pointer is used as the input HMAC Hash value.
 *          output pointer - For the generated Hash value.
 *  @param[in] ptrParams
 *      Pointer HMAC parameters.
 *
 *  \ingroup CRYPTO_DRIVER_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
int32_t Crypto_verify(Crypto_Handle handle, Crypto_HmacMode mode, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrSignature, Crypto_HmacParams* ptrParams)
{
    Crypto_Config*          ptrCryptoConfig;
    Crypto_MCB*             ptrCryptoMCB;
    int32_t                 retVal = 0U;
    uint32_t                cryptoConfig;
    int8_t                  signature[CRYPTO_MAX_DIGEST_SIZE];
    uint32_t                signatureSize;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (ptrInput == NULL) || (ptrSignature == NULL) || (ptrParams == NULL))
    {
        retVal = CRYPTO_EINVAL;
    }
    else
    {
        /* Get the Crypto Configuration: */
        ptrCryptoConfig = (Crypto_Config*)handle;

        /* Get the Crypto Driver Object */
        ptrCryptoMCB = (Crypto_MCB*)ptrCryptoConfig->object;

        if (ptrCryptoMCB == NULL)
        {
            retVal = CRYPTO_EINVAL;
        }
        else if (ptrCryptoMCB->type != Crypto_HMAC)
        {
            retVal = CRYPTO_EINVAL;
        }
        else
        {
            /* Get the semaphore */
            SemaphoreP_pend(ptrCryptoMCB->sem, SemaphoreP_WAIT_FOREVER);
            {
                /* Translate the crypto mode to the underlying HMAC register level driver mode */
                cryptoConfig = Crypto_getHmacConfig((uint32_t)mode);

                /* Get the digest size */
                signatureSize = HMAC_getDigestSize(cryptoConfig);

                if (signatureSize > 0)
                {
                    if (ptrParams->moreData == 0)
                    {
                        /* This is the last block. Save the received signature. */
                        memcpy ((void*)&signature[0], (void*)ptrSignature, signatureSize);
                    }

                    /* Calculate the signature */
                    retVal = Crypto_hmacProcess(ptrCryptoConfig, cryptoConfig, ptrInput, dataLength, ptrSignature, ptrParams);

                    /* compare the calculated signature to the received signature */
                    if (retVal == 0)
                    {
                        if (ptrParams->moreData == 0)
                        {
                            if (memcmp((void*)signature, (void*)ptrSignature, signatureSize) != 0)
                            {
                                retVal = CRYPTO_EVERIFY;
                            }
                        }
                    }
                }

                /* release the semaphore */
                SemaphoreP_post(ptrCryptoMCB->sem);
            }
        }
    }
    return retVal;
}


