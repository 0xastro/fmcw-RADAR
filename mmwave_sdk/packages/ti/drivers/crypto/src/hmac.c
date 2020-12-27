/**
 *   @file  hmac.c
 *
 *   @brief
 *      The file implements the register level functions for HMAC/HASH module using SHA/MD5 algorithms.
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
#include <ti/common/hw_types.h>

/**************************************************************************
 ****************************** Defines ***********************************
 **************************************************************************/
/** Interrupt masks */
#define P_EIP57T_INTERRUPT_MASK         (0x00000007U)

/**
 *  @b Description
 *  @n
 *      The function Initialize the HMAC module.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_init(DTHERegs* dtheBaseAddress, SHARegs* hmacBaseAddress)
{
    uint32_t        regVal;

    /* Disable DMA requests */
    hmacBaseAddress->P_SYSCONFIG = CSL_FINSR(hmacBaseAddress->P_SYSCONFIG, 3U, 3U, 0x0U);

    /* Enable public world interrupts Disable all others */
    hmacBaseAddress->P_SYSCONFIG = CSL_FINSR(hmacBaseAddress->P_SYSCONFIG, 2U, 2U, 0x1U);

    /* Enable all interrupts */
    regVal = hmacBaseAddress->P_SYSCONFIG;
    regVal |= (uint32_t) HMAC_SYSCONFIG_PIT_EN;

    hmacBaseAddress->P_SYSCONFIG = CSL_FINSR(hmacBaseAddress->P_SYSCONFIG, 31U, 0U, regVal);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function configures the HMAC module with input configuration parameters.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  cryptoMode
 *      Specifies the Hash algorithm. Valid values are:
 *          HMAC_ALGO_MD5
 *          HMAC_ALGO_SHA1
 *          HMAC_ALGO_SHA224
 *          HMAC_ALGO_SHA256
 *
 *  @param[in]  useAlgoConstant
 *      If set to 1, The initial digest register will be overwritten with the algorithm
 *      constants for the selected algorithm
 *  @param[in]  closeHash
 *      Specifies if the hash is closed or not.
 *      1 - hash/HMAC will be 'closed' at the end of the block and appropriate padding is added.
 *      0 - hash/HMAC will not be closed and can be continued later.
 *  @param[in]  useHMACKey
 *      The hash core will perform HMAC key processing on the 512 bit HMAC key loaded in the digest registers.
 *  @param[in]  HMACOuterHash
 *      1 - The HMAC Outer Hash is performed on the hash digest when the inner hash hash finished.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_setConfig(SHARegs *hmacBaseAddress, uint32_t cryptoMode,
                uint8_t useAlgoConstant, uint8_t closeHash,
                uint8_t useHMACKey, uint8_t HMACOuterHash)
{
    volatile uint32_t    regVal;

    regVal = (uint32_t) (cryptoMode | ((useAlgoConstant & 0x1U) << HMAC_USE_ALGO_CONSTANT_SHIFT) |
                    ((closeHash & 0x1U) << HMAC_CLOSE_HASH_SHIFT) |
                    ((useHMACKey & 0x1U) << HMAC_HMAC_KEY_PROC_SHIFT) |
                    ((HMACOuterHash & 0x1U) << HMAC_HMAC_OUTER_HASH_SHIFT));

    /* Write the value in the MODE register */
    hmacBaseAddress->P_MODE = CSL_FINSR(hmacBaseAddress->P_MODE, 31U, 0U, regVal);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function checks if the interrupt register is ready for operation indicated by flag.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  flag
 *      Indicates the bit positions to check if ready.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      0   - Not ready for access
 *      >0  - Ready for access
 */
uint32_t HMAC_isReady(const SHARegs* hmacBaseAddress, uint32_t flag)
{
    uint32_t        regVal;

    /* Read the control register to check if context, input or output is ready. */
    regVal = hmacBaseAddress->P_IRQSTATUS;

    return (regVal & flag);
}

/**
 *  @b Description
 *  @n
 *      The function configures the data length of the the block to be processed.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  dataLength
 *      Length of the block in bytes.
 *      Note: The value must be a multiple of 64 if the close hash is not set in the mode register.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_setDataLength(SHARegs *hmacBaseAddress, uint32_t dataLength)
{
    /* Set the LENGTH register to start processing */
    hmacBaseAddress->P_LENGTH = CSL_FINSR(hmacBaseAddress->P_LENGTH, 31U, 0U, dataLength);

    return;
}

/**
 *  @b Description
 *  @n
 *      This function writes 16 words of data into the data register.
 *      If the data registers are not ready, the function will block the caller.

 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  ptrData
 *      Pointer to the data buffer.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_writeData(SHARegs *hmacBaseAddress, const uint8_t* ptrData)
{
    uint32_t                index;
    uint32_t                dataIndex = 0U;
    volatile uint32_t*      ptrAddr;

    /* Wait for the module to be ready to accept data */
    while ((HMAC_isReady (hmacBaseAddress, HMAC_INT_INPUT_READY)) == 0U);

    ptrAddr = &hmacBaseAddress->P_DATA0_IN;

    /* Write the 16 words of data */
    for (index = 0U; index < 16U; index++)
    {
        (*ptrAddr) = *((uint32_t *)(ptrData + dataIndex));
        dataIndex += 4U;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      This function returns the number of bytes of digest for the specified hash algorithm.
 *
 *  @param[in]  config
 *      Bit values for the algorithm as specified in the mode register.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Number of bytes in the digest.
 */
uint32_t HMAC_getDigestSize(uint32_t config)
{
    uint32_t        digestSize;

    /* Determine the number of bytes in the result, based on the hash type */
    switch (config)
    {
        /* The MD5 hash is being used */
        case HMAC_ALGO_MD5:
        {
            /* There are 16 bytes in the MD5 hash */
            digestSize = HMAC_MD5_DIGEST_SIZE;
            break;
        }

        /* The SHA-1 hash is being used */
        case HMAC_ALGO_SHA1:
        {
            /* There are 20 bytes in the SHA-1 hash */
            digestSize = HMAC_SHA1_DIGEST_SIZE;
            break;
        }

        /* The SHA-224 hash is being used */
        case HMAC_ALGO_SHA224:
        {
            /* There are 28 bytes in the SHA-224 hash */
            digestSize = HMAC_SHA224_DIGEST_SIZE;
            break;
        }

        /* The SHA-256 hash is being used */
        case HMAC_ALGO_SHA256:
        {
            /* There are 32 bytes in the SHA-256 hash */
            digestSize = HMAC_SHA256_DIGEST_SIZE;
            break;
        }

        default:
        {
            /** Return without reading a result since the hardware appears to be
             *  misconfigured.
             */
            digestSize = 0U; /* Hash algorithm not supported or hardware is misconfigured */
            break;
        }
    }

    return digestSize;
}

/**
 *  @b Description
 *  @n
 *      This function reads the result of a hash operation.The length of hash
 *      or digest depends on the algorithm used for hashing.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  ptrData
 *      Pointer to a data buffer where the data is stored.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_readResult(SHARegs *hmacBaseAddress, uint8_t* ptrData)
{
    uint32_t                regVal;
    uint32_t                config;
    uint32_t                index;
    uint32_t                hashLen;
    volatile uint32_t*      ptrAddr;

    regVal = hmacBaseAddress->P_MODE;
    config = regVal & HMAC_MODE_ALGO_MASK;

    /* Determine the number of bytes in the result */
    hashLen = HMAC_getDigestSize(config);
    hashLen = hashLen / 4U;

    if (hashLen != 0U)
    {
        ptrAddr = &hmacBaseAddress->P_IDIGEST_A;
        for (index = 0U; index < hashLen; index++)
        {
            *((uint32_t *)(ptrData + (index * 4U))) = (*ptrAddr++);
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      This function writes the result of a hash operation.The length of hash
 *      or digest depends on the algorithm used for hashing.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[out]  ptrData
 *      Pointer to a data buffer where the data is stored.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_writeResult(SHARegs *hmacBaseAddress, uint8_t* ptrData)
{
    uint32_t                regVal;
    uint32_t                config;
    uint32_t                index;
    uint32_t                hashLen;
    volatile uint32_t*      ptrAddr;

    regVal = hmacBaseAddress->P_MODE;
    config = regVal & HMAC_MODE_ALGO_MASK;

    /* Determine the number of bytes in the result */
    hashLen = HMAC_getDigestSize(config);
    hashLen = hashLen / 4U;

    if (hashLen != 0U)
    {
        ptrAddr = &hmacBaseAddress->P_IDIGEST_A;
        for (index = 0U; index < hashLen; index++)
        {
            (*ptrAddr++) = *((uint32_t *)(ptrData + (index * 4U)));
        }
    }
    return;
}


/**
 *  @b Description
 *  @n
 *      This function reads the byte count of the current hashing operation.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Digest Count in bytes.
 */
uint32_t HMAC_readDigestCount(SHARegs *hmacBaseAddress)
{
    return(hmacBaseAddress->P_DIGEST_COUNT);
}

/**
 *  @b Description
 *  @n
 *      This function writes the byte count of the current hashing operation.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  digestCount
 *      Number of bytes to write.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_writeDigestCount(SHARegs *hmacBaseAddress, uint32_t digestCount)
{
	hmacBaseAddress->P_DIGEST_COUNT = digestCount;
}

/**
 *  @b Description
 *  @n
 *      This function writes HMAC key to the inner or outer digest registers.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  ptrKey
 *      Pointer to the key.
 *      Note: Key MUST be 64 bytes long.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_setHMACKey(SHARegs *hmacBaseAddress, const uint8_t* ptrKey)
{
    uint32_t                index;
    uint32_t                keyIndex;
    volatile uint32_t*      ptrAddr;

    /* Write first 256 bits of the key in outer digest registers */
    keyIndex = 0U;
    ptrAddr = &hmacBaseAddress->P_ODIGEST_A;
    for (index = 0U; index < 8U; index++)
    {
        (*ptrAddr++) = * ((uint32_t *)(ptrKey + keyIndex));
        keyIndex += 4U;
    }

    /* Write the next 256 bits in inner digest registers */
    ptrAddr = &hmacBaseAddress->P_IDIGEST_A;
    for (index = 0U; index < 8U; index++)
    {
        (*ptrAddr++) = * ((uint32_t *)(ptrKey + keyIndex));
        keyIndex += 4U;
    }

    return;
}


/**
 *  @b Description
 *  @n
 *      This function writes a pre-processed HMAC key to the inner or outer digest registers.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  ptrKey
 *      Pointer to the key.
 *      Note: Key MUST be 64 bytes long.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_setHMACPPKey(SHARegs *hmacBaseAddress, const uint8_t* ptrKey)
{
    HMAC_setHMACKey(hmacBaseAddress, ptrKey);

    /* Configure the engine to set the Outer Hash and Close Hash */
    hmacBaseAddress->P_MODE = CSL_FINSR (hmacBaseAddress->P_MODE, S_MODE_HMAC_OUTER_HASH_BIT_END, S_MODE_HMAC_OUTER_HASH_BIT_START, 1U);
    hmacBaseAddress->P_MODE = CSL_FINSR (hmacBaseAddress->P_MODE, S_MODE_CLOSE_HASH_BIT_END, S_MODE_CLOSE_HASH_BIT_START, 1U);

    /* Set the digest count to 64 for the preprocessed key. */
    hmacBaseAddress->P_DIGEST_COUNT = 64U;

    return;
}

/**
 *  @b Description
 *  @n
 *      The function enables the HMAC interrupts.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  intFlags
 *      Bit mask for the following interrupt sources.
 *          HMAC_INT_CONTEXT_READY      - Context input registers are ready.
 *          HMAC_INT_PARTHASH_READY     - Context output registers are ready after a context switch.
 *          HMAC_INT_INPUT_READY        - Data FIFO is ready to receive data.
 *          HMAC_INT_OUTPUT_READY       - Context output registers are ready.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_enableInterrupt(DTHERegs* dtheBaseAddress, SHARegs* hmacBaseAddress, uint32_t intFlags)
{
    uint32_t        regVal;

    /* Enable the interrupt sources */
    regVal = dtheBaseAddress->P_EIP57T_IMST;
    regVal &= ~(intFlags & P_EIP57T_INTERRUPT_MASK);
    dtheBaseAddress->P_EIP57T_IMST = CSL_FINSR(dtheBaseAddress->P_EIP57T_IMST, 31U, 0U, regVal);

    regVal = hmacBaseAddress->P_IRQENABLE;
    regVal |= (intFlags & HMAC_INTERRUPT_SET_MASK);
    hmacBaseAddress->P_IRQENABLE = CSL_FINSR(hmacBaseAddress->P_IRQENABLE, 31U, 0U, regVal);

    /* Enable all interrupts */
    regVal = hmacBaseAddress->P_SYSCONFIG;
    regVal |= (uint32_t) HMAC_SYSCONFIG_PIT_EN;
    hmacBaseAddress->P_SYSCONFIG = CSL_FINSR(hmacBaseAddress->P_SYSCONFIG, 31U, 0U, regVal);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function disables the HMAC interrupts.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  intFlags
 *      Bit mask for the following interrupt sources.
 *          HMAC_INT_CONTEXT_READY      - Context input registers are ready.
 *          HMAC_INT_PARTHASH_READY     - Context output registers are ready after a context switch.
 *          HMAC_INT_INPUT_READY        - Data FIFO is ready to receive data.
 *          HMAC_INT_OUTPUT_READY       - Context output registers are ready.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_disableInterrupt(DTHERegs* dtheBaseAddress, SHARegs* hmacBaseAddress, uint32_t intFlags)
{
    uint32_t        regVal;

    /* Clear the interrupt flags in the DTHE module */
    regVal = dtheBaseAddress->P_EIP57T_IMST;
    regVal |= (intFlags & P_EIP57T_INTERRUPT_MASK);
    dtheBaseAddress->P_EIP57T_IMST = CSL_FINSR(dtheBaseAddress->P_EIP57T_IMST, 31U, 0U, regVal);

    /* Clear the interrupt flags */
    regVal = hmacBaseAddress->P_IRQENABLE;
    regVal &= ~(intFlags & HMAC_INTERRUPT_SET_MASK);
    hmacBaseAddress->P_IRQENABLE = CSL_FINSR(hmacBaseAddress->P_IRQENABLE, 31U, 0U, regVal);

    regVal = hmacBaseAddress->P_IRQENABLE;

    /* If there are no interrupts enabled, then disable all interrupts */
    if (regVal == 0x00U)
    {
        regVal = hmacBaseAddress->P_SYSCONFIG;
        regVal &= ~HMAC_SYSCONFIG_PIT_EN;
        hmacBaseAddress->P_SYSCONFIG = CSL_FINSR(hmacBaseAddress->P_SYSCONFIG, 31U, 0U, regVal);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function clears the HMAC interrupts.
 *
 *   @note The DMA done interrupts are the only interrupts that can be cleared using this function.
 *      The remaining interrupts can be disabled using the HMAC_disableInterrupt() function.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  intFlags
 *      Bit mask for the following interrupt sources.
 *          HMAC_INT_DMA_CONTEXT_IN     - Context DMA done interrupt
 *          HMAC_INT_DMA_CONTEXT_OUT    - Authentication tag (and IV) DMA done interrupt
 *          HMAC_INT_DMA_DATA_IN        - Data input DMA done interrupt
 *          HMAC_INT_DMA_DATA_OUT       - Data output DMA done interrupt
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_clearInterrupt(DTHERegs* dtheBaseAddress, uint32_t intFlags)
{
    uint32_t        regVal;

    /* Clear the interrupt flags */
    regVal = dtheBaseAddress->P_EIP57T_IMST;
    regVal |= (intFlags & P_EIP57T_INTERRUPT_MASK);
    dtheBaseAddress->P_EIP57T_IMST = CSL_FINSR(dtheBaseAddress->P_EIP57T_IMST, 31U, 0U, regVal);

    return;
}

/**
 *  @b Description
 *  @n
 *      The function returns the interrupt status.
 *
 *  @param[in]  dtheBaseAddress
 *      Base address of DTHE module memory mapped registers.
 *  @param[in]  hmacBaseAddress
 *      Base address of SHA module memory mapped registers.
 *  @param[in]  masked
 *      0 - Raw interrupt status is read.
 *      1 - Masked interrupt status is read.
 *
 *  \ingroup CRYPTO_HMAC_RL_FUNCTION
 *
 *  @retval
 *      Bit mask for the following interrupt sources.
 *          HMAC_INT_CONTEXT_READY    - Context input registers are ready.
 *          HMAC_INT_PARTHASH_READY   - Context output registers are ready after a context switch.
 *          HMAC_INT_INPUT_READY      - Data FIFO is ready to receive data.
 *          HMAC_INT_OUTPUT_READY     - Context output registers are ready.
 */
uint32_t HMAC_getInterruptStatus(DTHERegs* dtheBaseAddress, SHARegs* hmacBaseAddress, uint8_t masked)
{
    uint32_t        regVal;
    uint32_t        irqEnable;
    uint32_t        irqStatus;
    uint32_t        retVal;

    /* Read the IRQ status register */
    if (masked == 1U)
    {
        regVal = dtheBaseAddress->P_EIP57T_IMIS;
        irqEnable = hmacBaseAddress->P_IRQENABLE;
        irqStatus = hmacBaseAddress->P_IRQSTATUS;

        retVal = ((irqStatus & irqEnable) | (regVal & P_EIP57T_INTERRUPT_MASK));
    }
    else
    {
        regVal = dtheBaseAddress->P_EIP57T_IRIS;
        irqStatus = hmacBaseAddress->P_IRQSTATUS;

        retVal = (irqStatus | (regVal & P_EIP57T_INTERRUPT_MASK));
    }
    return retVal;
}

