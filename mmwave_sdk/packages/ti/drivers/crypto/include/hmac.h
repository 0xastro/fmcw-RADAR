/**
 *   @file  hmac.h
 *
 *   @brief
 *      This is the HMAC module header file for the register level HMAC driver.
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

#ifndef ti_drivers_HMAC__include
#define ti_drivers_HMAC__include

#ifdef __cplusplus
extern "C" {
#endif

/**
@defgroup CRYPTO_HMAC_RL_FUNCTION             Crypto Driver HMAC Register Level Functions
@ingroup CRYPTO_INTERNAL_FUNCTION
@brief
*   The section has a list of all the HMAC functions used internally by the driver
*/

/**
@defgroup CRYPTO_HMAC_RL_DATA_STRUCTURE       Crypto Driver HMAC Register Level Data Structures
@ingroup CRYPTO_INTERNAL_DATA_STRUCTURE
@brief
*   The section has a list of all the HMAC data structures used internally by the driver
*/

/** @addtogroup CRYPTO_HMAC_RL_DATA_STRUCTURE
 @{ */

/** MD5 Algorithm */
#define HMAC_ALGO_MD5               (0x00000000U)

/** SHA-1 Algorithm */
#define HMAC_ALGO_SHA1              (0x00000002U)

/** SHA-224 Algorithm */
#define HMAC_ALGO_SHA224            (0x00000004U)

/** SHA-256 Algorithm */
#define HMAC_ALGO_SHA256            (0x00000006U)


/** MD5 Digest size */
#define HMAC_MD5_DIGEST_SIZE        (16U)

/** SHA-1 Digest size */
#define HMAC_SHA1_DIGEST_SIZE       (20U)

/** SHA-224 Digest size */
#define HMAC_SHA224_DIGEST_SIZE     (28U)

/** SHA-256 Digest size */
#define HMAC_SHA256_DIGEST_SIZE     (32U)

/** Maximum Digest size */
#define HMAC_DIGEST_MAX_SIZE        HMAC_SHA256_DIGEST_SIZE

/** Minimum Digest size */
#define HMAC_DIGEST_MIN_SIZE        HMAC_MD5_DIGEST_SIZE

/** Interrupt and DMA flags */
#define HMAC_INT_CONTEXT_READY          (0x00000008U)
#define HMAC_INT_PARTHASH_READY         (0x00000004U)
#define HMAC_INT_INPUT_READY            (0x00000002U)
#define HMAC_INT_OUTPUT_READY           (0x00000001U)
#define HMAC_INT_DMA_CONTEXT_IN         (0x00010000U)
#define HMAC_INT_DMA_DATA_IN            (0x00040000U)
#define HMAC_INT_DMA_CONTEXT_OUT        (0x00020000U)
#define HMAC_MODE_HMAC_OUTER_HASH       (0x00000080U)
#define HMAC_INTERRUPT_SET_MASK         (0x0000000FU)

/** HMAC Algoritm mask */
#define HMAC_MODE_ALGO_MASK             (0x00000007U)

/** Shift values defined for Hash Mode register access */
#define HMAC_USE_ALGO_CONSTANT_SHIFT    (0x00000003U)
#define HMAC_CLOSE_HASH_SHIFT           (0x00000004U)
#define HMAC_HMAC_KEY_PROC_SHIFT        (0x00000005U)
#define HMAC_REUSE_HMAC_KEY_SHIFT       (0x00000006U)
#define HMAC_HMAC_OUTER_HASH_SHIFT      (0x00000007U)

/** Defines for SysConfig register access */
#define HMAC_SYSCONFIG_PADVANCED        (0x00000080U)
#define HMAC_SYSCONFIG_PDMA_EN          (0x00000008U)
#define HMAC_SYSCONFIG_PIT_EN           (0x00000004U)

/**
@}
*/

/** @addtogroup CRYPTO_HMAC_RL_FUNCTION
 @{ */

extern void HMAC_init(DTHERegs* dtheBaseAddress, SHARegs* hmacBaseAddress);
extern void HMAC_setConfig(SHARegs *hmacBaseAddress, uint32_t cryptoMode,
                uint8_t useAlgoConstant, uint8_t closeHash,
                uint8_t useHMACKey, uint8_t HMACOuterHash);
extern uint32_t HMAC_isReady(const SHARegs* hmacBaseAddress, uint32_t flag);
extern void HMAC_setDataLength(SHARegs *hmacBaseAddress, uint32_t dataLength);
extern void HMAC_writeData(SHARegs *hmacBaseAddress, const uint8_t* ptrData);
extern uint32_t HMAC_getDigestSize(uint32_t algo);
extern void HMAC_readResult(SHARegs *hmacBaseAddress, uint8_t* ptrData);
extern void HMAC_writeResult(SHARegs *hmacBaseAddress, uint8_t* ptrData);
extern uint32_t HMAC_readDigestCount(SHARegs *hmacBaseAddress);
extern void HMAC_writeDigestCount(SHARegs *hmacBaseAddress, uint32_t digestCount);
extern void HMAC_setHMACKey(SHARegs *hmacBaseAddress, const uint8_t* ptrKey);
extern void HMAC_setHMACPPKey(SHARegs *hmacBaseAddress, const uint8_t* ptrKey);
extern void HMAC_enableInterrupt(DTHERegs* dtheBaseAddress, SHARegs* hmacBaseAddress, uint32_t intFlags);
extern void HMAC_disableInterrupt(DTHERegs* dtheBaseAddress, SHARegs* hmacBaseAddress, uint32_t intFlags);
extern void HMAC_clearInterrupt(DTHERegs* dtheBaseAddress, uint32_t intFlags);
extern uint32_t HMAC_getInterruptStatus(DTHERegs* dtheBaseAddress, SHARegs *hmacBaseAddress, uint8_t masked);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_HMAC__include */

