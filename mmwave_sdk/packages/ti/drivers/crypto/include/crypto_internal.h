/**
 *   @file  crypto_internal.h
 *
 *   @brief
 *      This is the internal Header for the Crypto Driver. This header
 *      file should *NOT* be directly included by applications.
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

#ifndef CRYPTO_INTERNAL_H
#define CRYPTO_INTERNAL_H

#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/crypto/Crypto.h>
#include <ti/drivers/crypto/include/reg_aes.h>
#include <ti/drivers/crypto/include/reg_sha.h>
#include <ti/drivers/crypto/include/reg_dthe.h>
#include <ti/drivers/crypto/include/aes.h>
#include <ti/drivers/crypto/include/hmac.h>

/**
@defgroup CRYPTO_INTERNAL_FUNCTION             Crypto Driver Internal Functions
@ingroup CRYPTO_DRIVER
@brief
*   The section has a list of all the internal functions used by the driver
*/

/**
@defgroup CRYPTO_INTERNAL_DATA_STRUCTURE       Crypto Driver Internal Data Structures
@ingroup CRYPTO_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * Port of the Crypto Layer:
 * These macros translate the Crypto defines to the underlying register
 * level AES module defines.
 *********************************************************************/

/* Crypto key size to AES module key size */
#define Crypto_getAesKeySize(keySize) (                                 \
    (keySize == Crypto_AES_KEY_SIZE_128BIT)? AES_CFG_KEY_SIZE_128BIT:   \
    (keySize == Crypto_AES_KEY_SIZE_192BIT)? AES_CFG_KEY_SIZE_192BIT:   \
    (keySize == Crypto_AES_KEY_SIZE_256BIT)? AES_CFG_KEY_SIZE_256BIT:0)

/* Crypto mode to HMAC module mode config */
#define Crypto_getHmacConfig(mode) (                                   \
    (mode == Crypto_HMAC_MD5)?              HMAC_ALGO_MD5:           \
    (mode == Crypto_HMAC_SHA1)?             HMAC_ALGO_SHA1:          \
    (mode == Crypto_HMAC_SHA224)?           HMAC_ALGO_SHA224:        \
    (mode == Crypto_HMAC_SHA256)?           HMAC_ALGO_SHA256:0)

/** @addtogroup CRYPTO_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  Crypto Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
typedef struct Crypto_HwCfg_t
{
    /**
     * @brief   Base address of the Crypto address space to be used.
     */
    AESRegs*            ptrAesBase;

    /**
     * @brief   Base address of the Crypto address space to be used.
     */
    SHARegs*            ptrHmacBase;

    /**
     * @brief   Base address of the Crypto address space to be used.
     */
    DTHERegs*           ptrDtheBase;

    /**
     * @brief   AES interrupt number
     */
    uint32_t            aesInterruptNum;

    /**
     * @brief   HMAC interrupt number
     */
    uint32_t            hmacInterruptNum;
}Crypto_HwCfg;

/**
 * @brief
 *  Crypto Driver Master Control Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the Crypto Driver.
 */
typedef struct Crypto_MCB_t
{
    /**
     * @brief   Crypto type for which this MCB is used - AES or HMAC
     */
    Crypto_Type                 type;

    /**
     * @brief   Handle to the Semaphore used to notify application of I2C transfer complete
     */
    SemaphoreP_Handle           sem;
} Crypto_MCB;

/**
@}
*/

extern void AES_processData(AESRegs* baseAddress, const uint8_t* ptrInput, uint8_t* ptrOutput, uint32_t dataLen);
extern void AES_generateDataMAC(AESRegs* baseAddress, const uint8_t* ptrInput, uint8_t* ptrTag, uint32_t dataLen);
extern void AES_processDataAE(AESRegs* baseAddress, const uint8_t* ptrInput, uint8_t* ptrOutput,
                              uint32_t dataLen, const uint8_t* ptrAuthInput, uint8_t* ptrTag, uint32_t authDataLen);
extern void HMAC_writeDataMultiple(SHARegs *hmacBaseAddress, const uint8_t* ptrData, uint32_t dataLength);
extern void HMAC_computeHash(SHARegs *hmacBaseAddress, const uint8_t* ptrData, uint32_t dataLength, uint8_t* ptrHashResult);
extern void HMAC_generateHMACPPKey(SHARegs *hmacBaseAddress, const uint8_t* ptrKey,
                                  uint32_t keySize, uint8_t* ptrPPKey);
#ifdef __cplusplus
}
#endif

#endif /* CRYPTO_INTERNAL_H */
