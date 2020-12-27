/**
 *   @file  Crypto.h
 *
 *   @brief
 *      This is the header file for the Crypto driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the Crypto driver.
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

/** @mainpage Crypto Driver
 *
 *  The Crypto header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/crypto/Crypto.h>
 *  @endcode
 *
 *  The crypto/include/reg_xxx.h has the register layer definitons for the
 *  Crypto Module.
 *
 *  # Operation #
 *  The Crypto driver is used for AES and HMAC Hash functions.
 *  This driver provides API for
 *      - AES Encrypt and Decrypt.
 *      - HMAC hash Sign and Verify.
 *
 *  The application initializes the Crypto driver by calling Crypto_init()
 *  and is then ready to open a Crypto by calling Crypto_open().
 *
 *  The APIs in this driver serve as an interface to a typical TI-RTOS
 *  application. The specific peripheral implementations are responsible to
 *  create all the OSAL specific primitives to allow for thread-safe
 *  operation.
 *
 *  ## Opening the driver #
 *
 *  @code
 *  Crypto_Handle      handle;
 *
 *  handle = Crypto_open(index, Crypto_AES | Crypto_HMAC);
 *  if (!handle)
 *  {
 *      System_printf("Crypto driver open failed\n");
 *  }
 *  @endcode
 *
 *
 *  ## AES data encryption #
 *
 *  @code
 *  Crypto_AESMode              mode = aesMode;
 *  Crypto_Params               params;
 *  unsigned char               plainData[16] = "whatsoever123456";
 *  unsigned int                plainDataLen = sizeof(plainData);
 *  unsigned char               cipherData[16];
 *  unsigned int                cipherDataLen;
 *
 *  params.aes.keySize = desiredKeySize;
 *  params.aes.ptrKey = (Crypto_KeyPtr)desiredKey; // desiredKey length should be as the desiredKeySize
 *  params.aes.pIV = (void *)pointerToInitVector;
 *  ret = Crypto_encrypt(handle, mode , plainData, plainDataLen, cipherData , &cipherDataLen , &params);
 *
 *  @endcode
 *
 *  ## Generate HMAC Hash signature #
 *
 *  @code
 *  Crypto_HmacMode         mode = hmacMode;
 *  Crypto_Params           params;
 *  unsigned char           dataBuff[] = "whatsoever";
 *  unsigned int            dataLength = sizeof(dataBuff);
 *  unsigned char           signatureBuff[32];
 *
 *  params.ptrKey = pointerToHMACkey;
 *  params.moreData = 0;
 *  ret = Crypto_sign(handle, mode, &dataBuff, dataLength, &signatureBuff, &params);
 *
 *  @endcode
 *
 *  # Implementation #
 *
 *  The Crypto driver interface module is joined (at link time) to a
 *  NULL-terminated array of Crypto_Config data structures named *Crypto_config*.
 *  *Crypto_config* is implemented in the application with each entry being an
 *  instance of a Crypto peripheral. Each entry in *Crypto_config* contains a:
 *  - (void *) data object that is pointed to Crypto_Object
 *
 */

/** @defgroup CRYPTO_DRIVER      Crypto Driver
 */
#ifndef ti_drivers_CRYPTO__include
#define ti_drivers_CRYPTO__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/common/mmwave_error.h>

/**
@defgroup CRYPTO_DRIVER_EXTERNAL_FUNCTION     Crypto Driver External Functions
@ingroup CRYPTO_DRIVER
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the Crypto driver
*/

/**
@defgroup CRYPTO_DRIVER_EXTERNAL_DATA_STRUCTURE      Crypto Driver External Data Structures
@ingroup CRYPTO_DRIVER
@brief
*   The section has a list of all the data structures which are exposed to the application
*/

/**
@defgroup CRYPTO_DRIVER_ERROR_CODE            Crypto Driver Error code
@ingroup CRYPTO_DRIVER
*  Base error code for the Crypto module is defined in the
*  \include ti/common/mmwave_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define CRYPTO_EINVAL                   (MMWAVE_ERRNO_CRYPTO_BASE-1)

/**
 * @brief   Error Code: Operation cannot be implemented because a previous
 * operation is still not complete.
 */
#define CRYPTO_EINUSE                   (MMWAVE_ERRNO_CRYPTO_BASE-2)

/**
 * @brief   Error Code: Operation is not implemented.
 */
#define CRYPTO_ENOTIMPL                 (MMWAVE_ERRNO_CRYPTO_BASE-3)

/**
 * @brief   Error Code: Verification failed.
 */
#define CRYPTO_EVERIFY                  (MMWAVE_ERRNO_CRYPTO_BASE-4)

/** @}*/

/** @}*/

/** @addtogroup CRYPTO_DRIVER_EXTERNAL_DATA_STRUCTURE
@{ */

#define CRYPTO_MD5_BLOCK_SIZE       64
#define CRYPTO_SHA1_BLOCK_SIZE      64
#define CRYPTO_SHA256_BLOCK_SIZE    64

#define CRYPTO_MD5_DIGEST_SIZE      16
#define CRYPTO_SHA1_DIGEST_SIZE     20
#define CRYPTO_SHA256_DIGEST_SIZE   32

#define CRYPTO_MAX_DIGEST_SIZE CRYPTO_SHA256_DIGEST_SIZE
#define CRYPTO_MAX_BLOCK_SIZE  CRYPTO_SHA256_BLOCK_SIZE

/*!
 *  @brief  Cryptography types configuration.
 *
 *  This enum defines bitwise Cryptography types.
 */
typedef enum
{
    Crypto_AES  = 0x01,    /*!< Advanced Encryption Standard */
    Crypto_HMAC = 0x02     /*!< Cryptographic hash function */
}Crypto_Type;

/*!
 *  @brief  AES Cryptography mode configuration.
 *
 *  This enum defines the AES Cryptography modes.
 */
typedef enum Crypto_AESMode_t{
    Crypto_AES_ECB = 1,                     /*!< AES Electronic CodeBook mode */
    Crypto_AES_CBC,                         /*!< AES Cipher Block Chaining mode */
    Crypto_AES_CTR,                         /*!< AES Counter mode */
    Crypto_AES_ICM,                         /*!< AES Integer Counter Mode */
    Crypto_AES_CFB,                         /*!< AES Cipher FeedBack mode */
    Crypto_AES_GCM,                         /*!< AES Galois/Counter mode */
    Crypto_AES_CCM                          /*!< AES Counter with CBC-MAC Mode */
}Crypto_AESMode;

/*!
 *  @brief  HMAC Cryptography mode configuration
 *
 *  This enum defines the HMAC HASH algorithms modes.
 */
typedef enum Crypto_HmacMode_t{
    Crypto_HMAC_MD5 =  1,                       /*!< MD5 used keyed-hash message authentication code */
    Crypto_HMAC_SHA1,                           /*!< SHA1 used keyed-hash message authentication code */
    Crypto_HMAC_SHA224,                         /*!< SHA224 used keyed-hash message authentication code */
    Crypto_HMAC_SHA256                          /*!< SHA256 used keyed-hash message authentication code */
}Crypto_HmacMode;

/*!
 *  @brief  AES Cryptography key size type configuration.
 *
 *  This enum defines the AES key size types.
 */
typedef enum Crypto_AesKeySize_t{
    Crypto_AES_KEY_SIZE_128BIT,
    Crypto_AES_KEY_SIZE_192BIT,
    Crypto_AES_KEY_SIZE_256BIT
}Crypto_AesKeySize;

/*!
 *  @brief  AES Counter Mode counter width configuration.
 *
 *  This enum defines the AES Counter mode specific parameters.
 *  Valid for Crypto_AES_CTR mode only.
 */
typedef enum Crypto_AesCounterWidth_t{
    Crypto_AES_COUNTER_SIZE_32BIT,
    Crypto_AES_COUNTER_SIZE_64BIT,
    Crypto_AES_COUNTER_SIZE_96BIT,
    Crypto_AES_COUNTER_SIZE_128BIT
}Crypto_AesCounterWidth;

/*!
 *  @brief  AES GCM Mode configuration.
 *
 *  This enum defines the AES GCM mode specific parameters.
 *  Valid for Crypto_AES_GCM mode only.
 */
typedef enum Crypto_AesGcmMode_t{
    Crypto_AES_GCM_HLY0ZERO,                /*!< AES Galois/Counter mode with GHASH with H loaded and Y0-encrypted forced to zero. */
    Crypto_AES_GCM_HLY0CALC,                /*!< AES Galois/Counter mode with GHASH with H loaded and Y0-encrypted calculated internally. */
    Crypto_AES_GCM_HY0CALC,                 /*!< AES Galois/Counter mode with autonomous GHASH (both H and Y0-encrypted calculated internally). */
}Crypto_AesGcmMode;

/*!
 *  @brief  AES CCM length field configuration.
 *
 *  This enum defines the AES CCM_L specific parameters.
 *  Valid for Crypto_AES_CCM mode only.
 */
typedef enum Crypto_AesCCM_LWidth_t{
    Crypto_AES_CCM_L_2,                /*!< The width of the length field in bytes = 2 bytes. */
    Crypto_AES_CCM_L_4,                /*!< The width of the length field in bytes = 4 bytes. */
    Crypto_AES_CCM_L_8,                /*!< The width of the length field in bytes = 8 bytes. */
}Crypto_AesCCM_LWidth;

/*!
 *  @brief  AES CCM length field configuration.
 *
 *  This enum defines the AES CCM_M specific parameters.
 *  Valid for Crypto_AES_CCM mode only.
 */
typedef enum Crypto_AesCCM_MWidth_t{
    Crypto_AES_CCM_M_4,                /*!< The width of the authentication length field in bytes = 4 bytes. */
    Crypto_AES_CCM_M_6,                /*!< The width of the authentication length field in bytes = 6 bytes. */
    Crypto_AES_CCM_M_8,                /*!< The width of the authentication length field in bytes = 8 bytes. */
    Crypto_AES_CCM_M_10,               /*!< The width of the authentication length field in bytes = 10 bytes. */
    Crypto_AES_CCM_M_12,               /*!< The width of the authentication length field in bytes = 12 bytes. */
    Crypto_AES_CCM_M_14,               /*!< The width of the authentication length field in bytes = 14 bytes. */
    Crypto_AES_CCM_M_16,               /*!< The width of the authentication length field in bytes = 16 bytes. */
}Crypto_AesCCM_MWidth;

/*!
 *  @brief  AES CCM mode configuration.
 *
 *  This structure defines the Additional CCM mode parameters used for Crypto_AES_CCM.
 */
typedef struct Crypto_AesCcmCfg_t{
    Crypto_AesCCM_LWidth    lWidth;     /*!< Width of the length field. The width of the length field in bytes is programmed value + 1. */
    Crypto_AesCCM_MWidth    mWidth;     /*!< Length of the authentication field. Authentication field length equals two times (the value of CCM-M + one).
                                          Note that the AES Engine always returns a 128-bit authentication field, of which the M least significant bytes are valid.
                                         */
}Crypto_AesCcmCfg;

/*!
 *  @brief  AES Additional Authentication Data input parameters.
 *
 *  This structure defines the AES Additional Authentication Data input parameters used for
 *  Crypto_AES_GCM and Crypto_AES_CCM.
 */
typedef struct Crypto_AesAadInputParams_t{
    const uint8_t*          ptrKey2;        /*!< pointer to AES second key */
    Crypto_AesKeySize       key2Size;       /*!< AES second Key size type */
    uint32_t                authLen;        /*!< length of the additional authentication data in bytes */
}Crypto_AesAadInputParams;

/*!
 *  @brief  AES Additional Authentication Data Parameters.
 *
 *  This union defines the AES additional authentication parameters used for
 *  Crypto_AES_GCM and Crypto_AES_CCM.
 */
typedef union Crypto_AesAadParams_t{
    Crypto_AesAadInputParams        input;  /*!< Input - additional authentication data */
    uint8_t*                        tag;    /*!< Output - pointer to a 4-word array where the hash tag is written */
}Crypto_AesAadParams;

/*!
 *  @brief  AES Mode specific parameters. Certain AES modes need additional configuration that be handled here.
 *  Valid for AES_CTR, GCM and CCM modes only.
 *
 *  This structure defines the AES mode specific parameters for Crypto_AES_CTR, Crypto_AES_GCM and Crypto_AES_CCM.
 *  Crypto_AES_GCM and Crypto_AES_CCM.
 */
typedef struct Crypto_AesModeParams_t{
    union
    {
        Crypto_AesCounterWidth      counterWidth;       /*!< Counter width */
        Crypto_AesGcmMode           gcmMode;            /*!< GCM mode */
        Crypto_AesCcmCfg            ccmParam;           /*!< CCM parameters */
    }u;
}Crypto_AesModeParams;

/*!
 *  @brief  AES Parameters.
 *
 *  This structure defines the AES parameters used in Crypto_encrypt and Crypto_decrypt functions.
 */
typedef struct Crypto_EncryptParams_t
{
    const uint8_t*              ptrKey;         /*!< pointer to AES key */
    Crypto_AesKeySize           keySize;        /*!< AES Key size type */
    void*                       ptrIV;          /*!< Pointer to AES Initialization Vector */
    Crypto_AesAadParams         aadParams;      /*!< AES additional authentication parameters */
    Crypto_AesModeParams        modeParams;     /*!< AES mode specific parameters */
}Crypto_EncryptParams;

/*!
 *  @brief  HMAC Parameters.
 *
 *  This structure defines the HMAC parameters used in Crypto_sign and Crypto_verify functions.
 */
typedef struct Crypto_HmacParams_t{
    /*! pointer to hash key */
    uint8_t*            ptrKey;

    /*! True value will NOT close the HMAC HW machine */
    uint8_t             moreData;

    /*! Reserved for future use */
    void*               ptrContext;

    /*! True if no data was written to the HMAC engine */
    uint8_t             first;

    /*! Number of bytes that was written to the HMAC engine */
    uint32_t            digestCount;

    /*! Intermediate digest */
    uint8_t             innerDigest[CRYPTO_MAX_DIGEST_SIZE];

    /*! Internal buffer - used when moreData sets to true and the data length is not an integer multiple of blockSize */
    uint8_t             buffer[CRYPTO_MAX_BLOCK_SIZE];

    /*! Number of bytes in buffer */
    uint32_t            bufferLength;

    /*! Block size of the hashing algorithm in use */
    uint32_t            blockSize;
}Crypto_HmacParams;

/*!
 *  @brief      A handle that is returned from a Crypto_open() call.
 */
typedef struct Crypto_Config_t*     Crypto_Handle;

/*!
 *  @brief  Crypto Global configuration.
 *
 *  The Crypto_Config structure contains a set of pointers used to characterize
 *  the Crypto driver implementation.
 *
 *  This structure needs to be defined before calling Crypto_init() and it must
 *  not be changed thereafter.
 *
 *  @sa     Crypto_init()
 */
typedef struct Crypto_Config_t {

    /*! Pointer to a driver specific data object */
    void*               object;

    /*! Pointer to a driver specific hardware attributes structure */
    void                const *hwAttrs;
} Crypto_Config;

/** @}*/

/**
 *  @b Description
 *  @n
 *      The function is the Crypto driver close function called close a
 *      Crypto Driver instance is being closed.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver.
 *
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
extern int32_t Crypto_close(Crypto_Handle handle);

/**
 *  @b Description
 *  @n
 *      The function is the Crypto driver initialization function.
 *
 *
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
extern void Crypto_init(void);

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Crypto Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  type
 *      Type of Crypto instance that is opened. Valid values are
 *          Crypto_AES  - AES Crypto engine
 *          Crypto_HMAC - HMAC Crypto engine
 *
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
extern Crypto_Handle Crypto_open(Crypto_Type type);

/**
 *  @b Description
 *  @n
 *      The function is called to encrypt plain text data using a specific AES mode. Depending on the
 *      mode the data is encrypted and or authenticated. The additional result is stored in the
 *      encrypt output parameters.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver.
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
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
extern int32_t Crypto_encrypt(Crypto_Handle handle, Crypto_AESMode mode, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrOutput, Crypto_EncryptParams* ptrParams);

/**
 *  @b Description
 *  @n
 *      The function is called to decrypt ciphered text data using a specific AES mode. Depending on the
 *      mode the data is encrypted and or authenticated. The additional result is stored in the
 *      encrypt output parameters.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver.
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
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
extern int32_t Crypto_decrypt(Crypto_Handle handle, Crypto_AESMode mode, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrOutput, Crypto_EncryptParams* ptrParams);

/**
 *  @b Description
 *  @n
 *      The function is initializes the HMAC default parameters.
 *
 *  @param[in]  params
 *      Pointer to HMAC  params.
 *
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
extern int32_t Crypto_hmacParams_init(Crypto_HmacParams *params);

/**
 *  @b Description
 *  @n
 *      The function is called to generate the HMAC Hash value of given plain Text.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver.
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
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code.
 */
extern int32_t Crypto_sign(Crypto_Handle handle, Crypto_HmacMode mode, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrSignature, Crypto_HmacParams* ptrParams);

/**
 *  @b Description
 *  @n
 *      The function is called to verify the HMAC Hash value of given input Text.
 *
 *  @param[in]  handle
 *      Handle to the Crypto Driver.
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
 *  \ingroup CRYPTO_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Crypto Error code
 */
extern int32_t Crypto_verify(Crypto_Handle handle, Crypto_HmacMode mode, uint8_t* ptrInput, uint32_t dataLength, uint8_t* ptrSignature, Crypto_HmacParams* ptrParams);
#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_CRYPTO__include */

