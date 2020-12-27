/**
 *   @file  aes.c
 *
 *   @brief
 *      The file implements the register level AES functions.
 *      No validations are performed on input parameters. The caller of the APIs need
 *      to ensure the input parameters are valid.
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
#include <ti/drivers/crypto/include/crypto_internal.h>

/**************************************************************************
 ****************************** Defines ***********************************
 **************************************************************************/
/** Interrupt masks */
#define P_EIP36T_INTERRUPT_SET_MASK         (0x0000000FU)

/**
 *  @b Description
 *  @n
 *      The function Initialize the AES module.
 *
 *  @param[in]  aesBaseAddress
 *      Base address of AES module memory mapped registers.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_init(AESRegs* aesBaseAddress)
{
    /* Configure module to use regular cryptographic mode (use the key registers for key input).
     * Disable Interrupt and DMA requests
     */
    aesBaseAddress->P_SYSCONFIG = CSL_FINSR(aesBaseAddress->P_SYSCONFIG, 9U, 4U, 0U);

    /* Disable all interrupts */
    aesBaseAddress->P_IRQENABLE = CSL_FINSR(aesBaseAddress->P_IRQENABLE, 3U, 0U, 0U);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function configures the AES module with input configuration parameters.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *
 *  @param[in]  config
 *      Configuration parameters for the AES module. The following bit flags are supported.
 *      Direction of the operation: Valid values are:
 *          AES_CFG_DIR_ENCRYPT - Encryption mode
 *          AES_CFG_DIR_DECRYPT - Decryption mode
 *
 *      Key size: Valid values are:
 *          AES_CFG_KEY_SIZE_128BIT - 128 bit key
 *          AES_CFG_KEY_SIZE_192BIT - 192 bit key
 *          AES_CFG_KEY_SIZE_256BIT - 256 bit key
 *
 *      Mode of operation: Valid values are:
 *          AES_CFG_MODE_ECB - Electronic codebook mode
 *          AES_CFG_MODE_CBC - Cipher-block chaining mode
 *          AES_CFG_MODE_CFB - Cipher feedback mode
 *          AES_CFG_MODE_CTR - Counter mode
 *          AES_CFG_MODE_ICM - Integer counter mode
 *          AES_CFG_MODE_XTS - Ciphertext stealing mode
 *          AES_CFG_MODE_XTS_TWEAKJL - XEX-baseAddressd tweaked-codebook mode with
 *                  ciphertext stealing with previous/intermediate tweak value and j loaded
 *          AES_CFG_MODE_XTS_K2IJL - XEX-baseAddressd tweaked-codebook mode with
 *                  ciphertext stealing with key2, i and j loaded
 *          AES_CFG_MODE_XTS_K2ILJ0 - XEX-baseAddressd tweaked-codebook mode with
 *                  ciphertext stealing with key2 and i loaded, j = 0
 *          AES_CFG_MODE_F8 - F8 mode
 *          AES_CFG_MODE_F9 - F9 mode
 *          AES_CFG_MODE_CBCMAC - Cipher block chaining message authentication code mode
 *          AES_CFG_MODE_GCM - Galois/counter mode
 *          AES_CFG_MODE_GCM_HLY0ZERO - Galois/counter mode with GHASH with H loaded and Y0-encrypted forced to zero
 *          AES_CFG_MODE_GCM_HLY0CALC - Galois/counter mode with GHASH with H loaded and Y0-encrypted calculated internally
 *          AES_CFG_MODE_GCM_HY0CALC - Galois/Counter mode with autonomous GHASH (both H and Y0-encrypted calculated internally)
 *          AES_CFG_MODE_CCM - Counter with CBC-MAC mode
 *
 *      Counter width: MUST be defined for CTR, CCM or GCM modes. Valid values are:
 *          AES_CFG_CTR_WIDTH_32 - 32 bit counter
 *          AES_CFG_CTR_WIDTH_64 - 64 bit counter
 *          AES_CFG_CTR_WIDTH_96 - 96 bit counter
 *          AES_CFG_CTR_WIDTH_128 - 128 bit counter
 *
 *      Length for CCM operation. Valid values for CCM_L are:
 *          AES_CFG_CCM_L_2 - 2 bytes
 *          AES_CFG_CCM_L_4 - 4 bytes
 *          AES_CFG_CCM_L_8 - 8 bytes
 *
 *      Length for CCM operation. Valid values for CCM_M are:
 *          AES_CFG_CCM_M_4 - 4 bytes
 *          AES_CFG_CCM_M_6 - 6 bytes
 *          AES_CFG_CCM_M_8 - 8 bytes
 *          AES_CFG_CCM_M_10 - 10 bytes
 *          AES_CFG_CCM_M_12 - 12 bytes
 *          AES_CFG_CCM_M_14 - 14 bytes
 *          AES_CFG_CCM_M_16 - 16 bytes
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_config(AESRegs *baseAddress, uint32_t config)
{
    uint32_t        regVal;

    /* Save context field before configuration the control register */
    regVal = baseAddress->P_CTRL;

    /* Check if we need to save the context */
    if ((regVal & AES_CTRL_SAVE_CONTEXT) != 0U)
    {
        config = config | AES_CTRL_SAVE_CONTEXT;
    }

    /* Write to the control register */
    baseAddress->P_CTRL = config;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function checks if the control register is ready for operation indicated by flag.
 *
 *  @param[in]  aesBaseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  flag
 *      Indicates the bit positions to check if ready.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      0 - Not ready for access
 *      1 - Ready for access
 */
uint32_t AES_isReady(const AESRegs* aesBaseAddress, uint32_t flag)
{
    uint32_t        regVal;

    /* Read the control register to check if context, input or output is ready. */
    regVal = aesBaseAddress->P_CTRL;

    return (regVal & flag);
}

/**
 *  @b Description
 *  @n
 *      The function writes the key1 registers based on the key length. This function will be used
 *      to configure the keys in all AES modes.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrKey
 *      Pointer to the keys.
 *  @param[in]  keySize
 *      Size of the keys. Valid values are:
 *          AES_CFG_KEY_SIZE_128BIT
 *          AES_CFG_KEY_SIZE_192BIT
 *          AES_CFG_KEY_SIZE_256BIT
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_setKey1(AESRegs *baseAddress, const uint8_t* ptrKey, uint32_t keySize)
{
    /* Write first four words */
    baseAddress->P_KEY1_0 = *((uint32_t *)(ptrKey + 0U));
    baseAddress->P_KEY1_1 = *((uint32_t *)(ptrKey + 4U));
    baseAddress->P_KEY1_2 = *((uint32_t *)(ptrKey + 8U));
    baseAddress->P_KEY1_3 = *((uint32_t *)(ptrKey + 12U));

    /* Write the next 2 words if the key size is 192 or 256 bits */
    if (keySize != AES_CFG_KEY_SIZE_128BIT)
    {
        baseAddress->P_KEY1_4 = *((uint32_t *)(ptrKey + 16U));
        baseAddress->P_KEY1_5 = *((uint32_t *)(ptrKey + 20U));
    }

    /* Write the next 2 words if the key size is 256 bits */
    if (keySize == AES_CFG_KEY_SIZE_256BIT)
    {
        baseAddress->P_KEY1_6 = *((uint32_t *)(ptrKey + 24U));
        baseAddress->P_KEY1_7 = *((uint32_t *)(ptrKey + 28U));
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function writes the key2 registers based on the key length. This function will be used
 *      to configure the keys in F8, F9, XTS, CCM, and CBC-MAC AES modes.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrKey
 *      Pointer to the keys.
 *  @param[in]  keySize
 *      Size of the keys. Valid values are:
 *          AES_CFG_KEY_SIZE_128BIT
 *          AES_CFG_KEY_SIZE_192BIT
 *          AES_CFG_KEY_SIZE_256BIT
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_setKey2(AESRegs* baseAddress, const uint8_t* ptrKey, uint32_t keySize)
{
    /* Write first four words */
    baseAddress->P_KEY2_0 = *((uint32_t *)(ptrKey + 0U));
    baseAddress->P_KEY2_1 = *((uint32_t *)(ptrKey + 4U));
    baseAddress->P_KEY2_2 = *((uint32_t *)(ptrKey + 8U));
    baseAddress->P_KEY2_3 = *((uint32_t *)(ptrKey + 12U));

    /* Write the next 2 words if the key size is 192 or 256 bits */
    if (keySize != AES_CFG_KEY_SIZE_128BIT)
    {
        baseAddress->P_KEY2_4 = *((uint32_t *)(ptrKey + 16U));
        baseAddress->P_KEY2_5 = *((uint32_t *)(ptrKey + 20U));
    }

    /* Write the next 2 words if the key size is 256 bits */
    if (keySize == AES_CFG_KEY_SIZE_256BIT)
    {
        baseAddress->P_KEY2_6 = *((uint32_t *)(ptrKey + 24U));
        baseAddress->P_KEY2_7 = *((uint32_t *)(ptrKey + 28U));
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function writes to the upper key2 registers. This function will be used
 *      to configure the keys in F8 and CBC-MAC AES modes.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrKey
 *      Pointer to the keys. Key size is always 128 bits.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_setKey3(AESRegs* baseAddress, const uint8_t* ptrKey)
{
    /* Write the key into the upper 4 key registers */
    baseAddress->P_KEY2_4 = *((uint32_t *)(ptrKey + 0U));
    baseAddress->P_KEY2_5 = *((uint32_t *)(ptrKey + 4U));
    baseAddress->P_KEY2_6 = *((uint32_t *)(ptrKey + 8U));
    baseAddress->P_KEY2_7 = *((uint32_t *)(ptrKey + 12U));

    return;
}

/**
 *  @b Description
 *  @n
 *      The function writes the Initialization Vector input registers.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrIVData
 *      Pointer to the IV data. IV size is always 128 bits.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_setIV(AESRegs* baseAddress, const uint8_t* ptrIVData)
{
    /* Write data into IV registers */
    baseAddress->P_IV_IN_0 = *((uint32_t *)(ptrIVData + 0U));
    baseAddress->P_IV_IN_1 = *((uint32_t *)(ptrIVData + 4U));
    baseAddress->P_IV_IN_2 = *((uint32_t *)(ptrIVData + 8U));
    baseAddress->P_IV_IN_3 = *((uint32_t *)(ptrIVData + 12U));

    return;
}

/**
 *  @b Description
 *  @n
 *      The function reads the value in Initialization Vector input registers to a user specified buffer.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[out]  ptrIVData
 *      Pointer to the read IV data. IVdata MUST be atleast 16 bytes.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_getIV(AESRegs* baseAddress, uint8_t* ptrIVData)
{
    /* Read data from IV registers */
    *((uint32_t *)(ptrIVData + 0U))  = baseAddress->P_IV_IN_0;
    *((uint32_t *)(ptrIVData + 4U))  = baseAddress->P_IV_IN_1;
    *((uint32_t *)(ptrIVData + 8U))  = baseAddress->P_IV_IN_2;
    *((uint32_t *)(ptrIVData + 12U)) = baseAddress->P_IV_IN_3;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function stores the value in tag registers to a user specified buffer.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[out] ptrTag
 *      Pointer to the read tag data. Tag MUST be atleast 16 bytes.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_readTag(const AESRegs* baseAddress, uint8_t* ptrTag)
{
    /* Store the tag data */
    *((uint32_t *)(ptrTag + 0U))   = baseAddress->P_TAG_OUT_0;
    *((uint32_t *)(ptrTag + 4U))   = baseAddress->P_TAG_OUT_1;
    *((uint32_t *)(ptrTag + 8U))   = baseAddress->P_TAG_OUT_2;
    *((uint32_t *)(ptrTag + 12U))  = baseAddress->P_TAG_OUT_3;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function configures the data length in the AES module.
 *      This function stores the cryptographic data length in blocks for all modes.
 *      Data lengths up to (2^61 - 1) bytes are allowed.  For GCM, any value up
 *      to (2^36 - 2) bytes are allowed because a 32-bit block counter is used. For
 *      basic modes (ECB/CBC/CTR/ICM/CFB128), zero can be programmed into the
 *      length field, indicating that the length is infinite.
 *
 *      When this function is called, the engine is triggered to start using this context.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  dataLen
 *      Data length in bytes.
 *
 *  @note This length does not include the authentication-only data used in
 *      some modes.  Use the aesSetAuthDataLength() function to specify the
 *      authentication data length.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_setDataLength(AESRegs* baseAddress, uint64_t dataLen)
{
    /* Write the length register by shifting the 64-bit length */
    baseAddress->P_C_LENGTH_0 = (uint32_t) (dataLen);
    baseAddress->P_C_LENGTH_1 = (uint32_t)(dataLen >> (uint64_t)32U);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function configures the authentication data length in the XTS mode and
 *      combined modes for GCM/CCM.
 *      Supported AAD lengths are:
 *          CCM are from 0 to (2^16 - 28) bytes.
 *          GCM, any value up to (2^32 - 1) can be used.
 *          XTS mode, this register is used to load j.  Loading of j is only required if j != 0.
 *          j represents the sequential number of the 128-bit blocks inside the data unit.
 *          Consequently, j must be multiplied by 16 when passed to this function, thereby
 *          placing the block number in bits [31:4] of the register.
 *
 *      When this function is called, the engine is triggered to start using this context for GCM and CCM modes.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  dataLen
 *      Data length in bytes.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_setAuthDataLength(AESRegs* baseAddress, uint32_t dataLen)
{
    /* Write the length into the register */
    baseAddress->P_AUTH_LENGTH = dataLen;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function reads the ciphered or plain text data into the user specified buffer.
 *      If the data is not ready, the function will block the caller.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[out]  ptrData
 *      Pointer to the data buffer.
 *  @param[in]  dataLen
 *      Data length in bytes. Valid values are 1 to 16.
 *
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_readDataBlocking(const AESRegs* baseAddress, uint8_t* ptrData, uint32_t dataLen)
{
    uint32_t        temp[4];

    /* Wait until data is available */
    while ((AES_isReady (baseAddress, AES_CTRL_OUTPUT_READY)) == 0U);

    /* Read a block of data from the data registers */
    temp[0] = baseAddress->P_DATA_IN_3;
    temp[1] = baseAddress->P_DATA_IN_2;
    temp[2] = baseAddress->P_DATA_IN_1;
    temp[3] = baseAddress->P_DATA_IN_0;

    /* Copy data bytes */
    memcpy((void *)ptrData, (void *)temp, dataLen);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function writes the ciphered or plain text data into the data registers.
 *      If the data registers are not ready, the function will block the caller.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrData
 *      Pointer to the data buffer.
 *  @param[in]  dataLen
 *      Data length in bytes. Valid values are 1 to 16.
 *
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_writeDataBlocking(AESRegs* baseAddress, const uint8_t* ptrData, uint32_t dataLen)
{
    uint32_t        temp[4U] = {0U, 0U, 0U, 0U};

    /* Wait until data registers are free */
    while ((AES_isReady (baseAddress, AES_CTRL_INPUT_READY)) == 0U);

    memcpy((void *)temp, (const void *)ptrData, dataLen);

    /* Write a block of data into the data registers */
    baseAddress->P_DATA_IN_3 = temp[0U];
    baseAddress->P_DATA_IN_2 = temp[1U];
    baseAddress->P_DATA_IN_1 = temp[2U];
    baseAddress->P_DATA_IN_0 = temp[3U];

    return;
}

/**
 *  @b Description
 *  @n
 *      The function enables the AES interrupts.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  aesBaseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  intFlags
 *      Bit mask for the following interrupt sources.
 *          AES_INT_CONTEXT_IN      - Context interrupt
 *          AES_INT_CONTEXT_OUT     - Authentication tag and IV interrupt.
 *          AES_INT_DATA_IN         - Data input interrupt
 *          AES_INT_DATA_OUT        - Data output interrupt
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_enableInterrupt(DTHERegs* dtheBaseAddress, AESRegs* aesBaseAddress, uint32_t intFlags)
{
    uint32_t        regVal;

    /* Set the interrupt flags in the DTHE module */
    regVal = dtheBaseAddress->P_EIP36T_IMST;
    regVal &= ~(intFlags & P_EIP36T_INTERRUPT_SET_MASK);
    dtheBaseAddress->P_EIP36T_IMST = regVal;

    /* Set the interrupt flags in the AES module */
    regVal = aesBaseAddress->P_IRQENABLE;
    regVal |= intFlags & AES_INTERRUPT_SET_MASK;
    aesBaseAddress->P_IRQENABLE = regVal;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function disables the AES interrupts.
 *
 *   @note The DMA done interrupts are the only interrupts that can be cleared.
 *      The remaining interrupts can be disabled instead using this function.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  aesBaseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  intFlags
 *      Bit mask for the following interrupt sources.
 *          AES_INT_CONTEXT_IN      - Context interrupt
 *          AES_INT_CONTEXT_OUT     - Authentication tag and IV interrupt.
 *          AES_INT_DATA_IN         - Data input interrupt
 *          AES_INT_DATA_OUT        - Data output interrupt
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_disableInterrupt(DTHERegs* dtheBaseAddress, AESRegs* aesBaseAddress, uint32_t intFlags)
{
    uint32_t        regVal;

    /* Clear the interrupt flags in the DTHE module */
    regVal = dtheBaseAddress->P_EIP36T_IMST;
    regVal |= (intFlags & P_EIP36T_INTERRUPT_SET_MASK);
    dtheBaseAddress->P_EIP36T_IMST = regVal;

    /* Clear the interrupt flags in the AES module */
    regVal = aesBaseAddress->P_IRQENABLE;
    regVal &= ~(intFlags & AES_INTERRUPT_SET_MASK);
    aesBaseAddress->P_IRQENABLE = regVal;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function clears the AES interrupts.
 *
 *   @note The DMA done interrupts are the only interrupts that can be cleared using this function.
 *      The remaining interrupts can be disabled using the AES_disableInterrupt() function.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  intFlags
 *      Bit mask for the following interrupt sources.
 *          AES_INT_DMA_CONTEXT_IN      - Context DMA done interrupt
 *          AES_INT_DMA_CONTEXT_OUT     - Authentication tag (and IV) DMA done interrupt
 *          AES_INT_DMA_DATA_IN         - Data input DMA done interrupt
 *          AES_INT_DMA_DATA_OUT        - Data output DMA done interrupt
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_clearInterrupt(DTHERegs* dtheBaseAddress, uint32_t intFlags)
{
    uint32_t        regVal;

    /* Clear the interrupt flags in the DTHE module */
    regVal = ((intFlags >> 16U) & AES_INTERRUPT_SET_MASK);
    dtheBaseAddress->P_EIP36T_ICIS = regVal;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function returns the interrupt status.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  aesBaseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  masked
 *      0 - Raw interrupt status is read.
 *      1 - Masked interrupt status is read.
 *
 *  \ingroup CRYPTO_AES_RL_FUNCTION
 *
 *  @retval
 *      Bit mask for the following interrupt sources.
 *          AES_INT_CONTEXT_IN      - Context interrupt
 *          AES_INT_CONTEXT_OUT     - Authentication tag and IV interrupt.
 *          AES_INT_DATA_IN         - Data input interrupt
 *          AES_INT_DATA_OUT        - Data output interrupt
 */
uint32_t AES_getInterruptStatus(DTHERegs* dtheBaseAddress, AESRegs* aesBaseAddress, uint8_t masked)
{
    uint32_t        regVal;
    uint32_t        irqEnable;
    uint32_t        irqStatus;
    uint32_t        retVal;

    /* Read the IRQ status register */
    if (masked == 1U)
    {
        regVal = dtheBaseAddress->P_EIP36T_IMIS;
        irqEnable = aesBaseAddress->P_IRQENABLE;
        irqStatus = aesBaseAddress->P_IRQSTATUS;

        retVal = ((irqStatus & irqEnable) | ((regVal & AES_INTERRUPT_SET_MASK) << 16U));
    }
    else
    {
        regVal = dtheBaseAddress->P_EIP36T_IRIS;
        irqStatus = aesBaseAddress->P_IRQSTATUS;

        retVal = (irqStatus | ((regVal & AES_INTERRUPT_SET_MASK) << 16U));
    }
    return retVal;
}

