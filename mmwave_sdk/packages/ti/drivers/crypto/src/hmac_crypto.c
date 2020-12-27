/**
 *   @file  hmac_crypto.c
 *
 *   @brief
 *      The file implements the HMAC/HASH module functions that process the
 *      given data and generate the output based on the module configuration.
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

/**
 *  @b Description
 *  @n
 *      This function writes multiple words of data into the data register.
 *      The function waits for each block of data to be processed
 *      before another block is written.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  ptrData
 *      Pointer to the data buffer.
 *  @param[in]  dataLength
 *      Length of the data in bytes.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_writeDataMultiple(SHARegs *hmacBaseAddress, const uint8_t* ptrData, uint32_t dataLength)
{
    uint32_t                count;
    uint32_t                index;
    uint32_t                lastWord;
    volatile uint32_t*      ptrAddr;

    /* Calculate the number of blocks of data */
    count = dataLength / 64U;

    /** Loop through all the blocks and write them into the data registers. */
    for(index = 0; index < count; index++)
    {
        /* Write the data block of 16 words. */
        HMAC_writeData(hmacBaseAddress, ptrData);

        /* Increment the pointer to point to the next block of data. */
        ptrData += 64U;
    }

    /* Calculate the remaining bytes of data that don't make up a full block. */
    count = dataLength % 64U;

    /** If there are bytes that do not make up a whole block, then
     *  write them separately.
     */
    if (count != 0U)
    {
        /* Wait until the engine has finished processing the previous block. */
        while ((HMAC_isReady (hmacBaseAddress, HMAC_INT_INPUT_READY)) == 0U);

        /* Loop through the remaining words. */
        count = count / 4U;

        ptrAddr = &hmacBaseAddress->P_DATA0_IN;
        for (index = 0U; index < count; index++)
        {
            /* Write the word into the data register. */
            (*ptrAddr) = *((uint32_t *)ptrData);
            ptrData += 4U;
        }

        /* Loop through the remaining bytes.. */
        count = dataLength % 4U;
        if (count != 0U)
        {
            /* Write a word into the DATA_IN. Pad the remaining bytes with zero. */
            lastWord = 0U;
            (void)memcpy((void *)&lastWord, (const void *)ptrData, count);

            /* Write data to DATA_IN register. */
            (*ptrAddr) = lastWord;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      This function computes the hash using the algorithm and modes that are configured in the Mode register.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  ptrData
 *      Pointer to the data buffer.
 *  @param[in]  dataLength
 *      Data length in bytes.
 *  @param[out]  ptrHashResult
 *      Pointer to buffer where hash result is stored.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_computeHash(SHARegs *hmacBaseAddress, const uint8_t* ptrData, uint32_t dataLength, uint8_t* ptrHashResult)
{
    /* Wait for the context to be ready before writing the new context */
    while ((HMAC_isReady (hmacBaseAddress, HMAC_INT_CONTEXT_READY)) == 0U);

    /* Write the length */
    HMAC_setDataLength(hmacBaseAddress, dataLength);

    /*  Write the data */
    HMAC_writeDataMultiple(hmacBaseAddress, ptrData, dataLength);

    /* Wait for the output to be ready */
    while ((HMAC_isReady (hmacBaseAddress, HMAC_INT_OUTPUT_READY)) == 0U);

    /* Read the result */
    HMAC_readResult(hmacBaseAddress, ptrHashResult);

    return;
}

/**
 *  @b Description
 *  @n
 *      This function is used to pre process a HMAC key. The pre-processed key can then be
 *      used with further HMAC operations to speed up processing time.
 *
 *  @param[in]  hmacBaseAddress
 *      Base address of HMAC module memory mapped registers.
 *  @param[in]  ptrKey
 *      Pointer to the key.
 *  @param[in]  keySize
 *      Key Size in bytes.
 *      If the key size is less than 512 bits(64 bytes), the function will be padded with zeros.
 *  @param[out]  ptrPPKey
 *      Pointer to the pre-processed key. Must be 64 bytes longs.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void HMAC_generateHMACPPKey(SHARegs *hmacBaseAddress, const uint8_t* ptrKey,
                                  uint32_t keySize, uint8_t* ptrPPKey)
{
    uint32_t                key_512Bit[16U] = {0U};
    uint32_t                index;
    uint32_t                keyIndex;
    volatile uint32_t*      ptrAddr;

    /* Taking care of padding to get 512 bit key */
    (void)memcpy((void *)key_512Bit, (const void *)ptrKey, keySize);

    /* Wait for the context to be ready */
    while ((HMAC_isReady (hmacBaseAddress, HMAC_INT_CONTEXT_READY)) == 0U);

    /* Set the HMAC keyVal */
    HMAC_setHMACKey(hmacBaseAddress, (uint8_t*)&key_512Bit[0]);

    /* Set the flag to cause the HMAC key to be pre-processed */
    hmacBaseAddress->P_MODE = CSL_FINSR(hmacBaseAddress->P_MODE, S_MODE_HMAC_KEY_PROC_BIT_END, S_MODE_HMAC_KEY_PROC_BIT_START, 1);

    /* Set the length to zero to start the HMAC key pre-processing */
    hmacBaseAddress->P_LENGTH = CSL_FINSR(hmacBaseAddress->P_LENGTH, 31U, 0U, 0U);

    /* Wait for key to be processed */
    while ((HMAC_isReady (hmacBaseAddress, HMAC_INT_OUTPUT_READY)) == 0U);

    /* Read the pre-processed key */
    /* Read the first 256 bits from outer digest registers */
    keyIndex = 0U;
    ptrAddr = &hmacBaseAddress->P_ODIGEST_A;

    for (index = 0U; index < 8U; index++)
    {
        *((uint32_t *)(ptrPPKey + keyIndex)) = (*ptrAddr++);
        keyIndex += 4U;
    }

    /* Read the next 256 bits from inner digest registers */
    ptrAddr = &hmacBaseAddress->P_IDIGEST_A;
    for (index = 0U; index < 8U; index++)
    {
        *((uint32_t *)(ptrPPKey + keyIndex)) = (*ptrAddr++);
        keyIndex += 4U;
    }
    return;
}

