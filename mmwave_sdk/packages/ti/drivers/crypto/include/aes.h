/**
 *   @file  aes.h
 *
 *   @brief
 *      This is the AES module header file for the register level AES driver.
 *      It defines various encryption modes supported by the AES module.
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

#ifndef ti_drivers_AES__include
#define ti_drivers_AES__include

#ifdef __cplusplus
extern "C" {
#endif

/**
@defgroup CRYPTO_AES_RL_FUNCTION             Crypto Driver AES Register Level Functions
@ingroup CRYPTO_INTERNAL_FUNCTION
@brief
*   The section has a list of all the AES functions used internally by the driver
*/

/**
@defgroup CRYPTO_AES_RL_DATA_STRUCTURE       Crypto Driver AES Register Level Data Structures
@ingroup CRYPTO_INTERNAL_DATA_STRUCTURE
@brief
*   The section has a list of all the data structures used internally by the driver
*/

/** @addtogroup CRYPTO_AES_RL_DATA_STRUCTURE
 @{ */

/** AES operation direction */
#define AES_CFG_DIR_ENCRYPT             (0x00000004U)
#define AES_CFG_DIR_DECRYPT             (0x00000000U)

/** AES key size */
#define AES_CFG_KEY_SIZE_128BIT         (0x00000008U)
#define AES_CFG_KEY_SIZE_192BIT         (0x00000010U)
#define AES_CFG_KEY_SIZE_256BIT         (0x00000018U)

/** AES mode of operation */
#define AES_CFG_MODE_ECB                (0x00000000U)
#define AES_CFG_MODE_CBC                (0x00000020U)

/** AES counter mode */
#define AES_CFG_MODE_CTR                (0x00000040U)

/** AES Integer counter mode(ICM) */
#define AES_CFG_MODE_ICM                (0x00000200U)

/** AES cipher feedback mode(CFB128) */
#define AES_CFG_MODE_CFB                (0x00000400U)

/** AES XTS mode */
#define AES_CFG_MODE_XTS_TWEAKJL        (0x00000800U)
#define AES_CFG_MODE_XTS_K2IJL          (0x00001000U)
#define AES_CFG_MODE_XTS_K2ILJ0         (0x00001800U)

/** AES F8 mode */
#define AES_CFG_MODE_F8                 (0x00002000U)

/** AES F9 mode */
#define AES_CFG_MODE_F9                 (0x20004000U)

/** AES CBC-MAC mode */
#define AES_CFG_MODE_CBCMAC             (0x20008004U)

/** AES GCM mode */
#define AES_CFG_MODE_GCM_HLY0ZERO       (0x20010040U)
#define AES_CFG_MODE_GCM_HLY0CALC       (0x20020040U)
#define AES_CFG_MODE_GCM_HY0CALC        (0x20030040U)

/** AES CCM mode */
#define AES_CFG_MODE_CCM                (0x20040040U)

/** AES mode invalid */
#define AES_CFG_MODE_INVALID             (0xFFFFFFFFU)

/** AES Counter width Used in with CTR, CCM, or GCM modes. */
#define AES_CFG_CTR_WIDTH_32            (0x00000000U)
#define AES_CFG_CTR_WIDTH_64            (0x00000080U)
#define AES_CFG_CTR_WIDTH_96            (0x0000100U)
#define AES_CFG_CTR_WIDTH_128           (0x00000180U)

/** AES counter mode with width specified */
#define AES_CFG_MODE_CTR_32             (AES_CFG_MODE_CTR | AES_CFG_CTR_WIDTH_32)
#define AES_CFG_MODE_CTR_64             (AES_CFG_MODE_CTR | AES_CFG_CTR_WIDTH_64)
#define AES_CFG_MODE_CTR_96             (AES_CFG_MODE_CTR | AES_CFG_CTR_WIDTH_96)
#define AES_CFG_MODE_CTR_128            (AES_CFG_MODE_CTR | AES_CFG_CTR_WIDTH_128)

/** AES CCM L and M defines */
#define AES_CFG_CCM_L_2                 (0x00080000U)
#define AES_CFG_CCM_L_4                 (0x00180000U)
#define AES_CFG_CCM_L_8                 (0x00380000U)
#define AES_CFG_CCM_M_4                 (0x00400000U)
#define AES_CFG_CCM_M_6                 (0x00800000U)
#define AES_CFG_CCM_M_8                 (0x00c00000U)
#define AES_CFG_CCM_M_10                (0x01000000U)
#define AES_CFG_CCM_M_12                (0x01400000U)
#define AES_CFG_CCM_M_14                (0x01800000U)
#define AES_CFG_CCM_M_16                (0x01c00000U)

/** Interrupt flags */
#define AES_INT_CONTEXT_IN              (0x00000001U)
#define AES_INT_CONTEXT_OUT             (0x00000008U)
#define AES_INT_DATA_IN                 (0x00000002U)
#define AES_INT_DATA_OUT                (0x00000004U)

/** DMA flags */
#define AES_INT_DMA_CONTEXT_IN          (0x00010000U)
#define AES_INT_DMA_CONTEXT_OUT         (0x00020000U)
#define AES_INT_DMA_DATA_IN             (0x00040000U)
#define AES_INT_DMA_DATA_OUT            (0x00080000U)
#define AES_DMA_DATA_IN                 (0x00000040U)
#define AES_DMA_DATA_OUT                (0x00000020U)
#define AES_DMA_CONTEXT_IN              (0x00000080U)
#define AES_DMA_CONTEXT_OUT             (0x00000100U)

/** AES Control flags */
#define AES_CTRL_CONTEXT_READY          (0x80000000U)
#define AES_CTRL_SVCTXTRDY              (0x40000000U)
#define AES_CTRL_SAVE_CONTEXT           (0x20000000U)
#define AES_CTRL_CCM_M                  (0x01C00000U)
#define AES_CTRL_CCM_L                  (0x00380000U)
#define AES_CTRL_CCM                    (0x00040000U)
#define AES_CTRL_GCM                    (0x00030000U)
#define AES_CTRL_CBCMAC                 (0x00008000U)
#define AES_CTRL_F9                     (0x00004000U)
#define AES_CTRL_F8                     (0x00002000U)
#define AES_CTRL_XTS                    (0x00001800U)
#define AES_CTRL_CFB                    (0x00000400U)
#define AES_CTRL_ICM                    (0x00000200U)
#define AES_CTRL_CTR_WIDTH              (0x00000180U)
#define AES_CTRL_CTR                    (0x00000040U)
#define AES_CTRL_MODE                   (0x00000020U)
#define AES_CTRL_KEY_SIZE               (0x00000018U)
#define AES_CTRL_DIRECTION              (0x00000004U)
#define AES_CTRL_INPUT_READY            (0x00000002U)
#define AES_CTRL_OUTPUT_READY           (0x00000001U)

/* AES Interrupt mask */
#define AES_INTERRUPT_SET_MASK          (0x0000000FU)
/**
@}
*/

/** @addtogroup CRYPTO_AES_RL_FUNCTION
 @{ */

extern void AES_init(AESRegs* aesBaseAddress);
extern void AES_config(AESRegs *baseAddress, uint32_t config);
extern uint32_t AES_isReady(const AESRegs* aesBaseAddress, uint32_t flag);
extern void AES_setKey1(AESRegs *baseAddress, const uint8_t* ptrKey, uint32_t keySize);
extern void AES_setKey2(AESRegs *baseAddress, const uint8_t* ptrKey, uint32_t keySize);
extern void AES_setKey3(AESRegs *baseAddress, const uint8_t* ptrKey);
extern void AES_setIV(AESRegs* baseAddress, const uint8_t* ptrIVData);
extern void AES_getIV(AESRegs* baseAddress, uint8_t* ptrIVData);
extern void AES_readTag(const AESRegs* baseAddress, uint8_t* ptrTag);
extern void AES_setDataLength(AESRegs* baseAddress, uint64_t dataLen);
extern void AES_setAuthDataLength(AESRegs* baseAddress, uint32_t dataLen);
extern void AES_readDataBlocking(const AESRegs* baseAddress, uint8_t* ptrData, uint32_t dataLen);
extern void AES_writeDataBlocking(AESRegs* baseAddress, const uint8_t* ptrData, uint32_t dataLen);
extern void AES_enableInterrupt(DTHERegs* dtheBaseAddress, AESRegs* aesBaseAddress, uint32_t intFlags);
extern void AES_disableInterrupt(DTHERegs* dtheBaseAddress, AESRegs* aesBaseAddress, uint32_t intFlags);
extern void AES_clearInterrupt(DTHERegs* dtheBaseAddress, uint32_t intFlags);
extern uint32_t AES_getInterruptStatus(DTHERegs* dtheBaseAddress, AESRegs* aesBaseAddress, uint8_t masked);

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_AES__include */




