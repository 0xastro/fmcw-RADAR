/**
 *   @file  aes_crypto.c
 *
 *   @brief
 *      The file implements the AES crypto functions that process the
 *      given data and generate the output based on the crypto module configuration.
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

/**
 *  @b Description
 *  @n
 *      The function processes(encrypt or decrypt) a block of input data and stores the output data into a user
 *      specified output buffer.
 *      The function chunks the block of data into 16 byte chunks for processing.
 *      Note: The AES module must be configured for mode, keys etc. before calling this function.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[out] ptrOutput
 *      Pointer to the output data buffer.
 *  @param[in]  dataLen
 *      Data length in bytes. Valid values are 1 to less than 2^32.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_processData(AESRegs* baseAddress, const uint8_t* ptrInput, uint8_t* ptrOutput, uint32_t dataLen)
{
    uint32_t        blockCount;
    uint32_t        byteCount;
    uint32_t        index;

    /* Set the data length to trigger the engine to use the configured context */
    AES_setDataLength(baseAddress, (uint64_t) dataLen);

    /* Chunk the data into 16 byte blocks */
    blockCount = dataLen/16U;
    for (index = 0U; index < blockCount; index++)
    {
        /* Write the input data */
        AES_writeDataBlocking(baseAddress, (ptrInput + (index * 16U)), 16U);

        /* Read the output data */
        AES_readDataBlocking(baseAddress, (ptrOutput + (index * 16U)), 16U);
    }

    /* Check for any remaining bytes */
    byteCount = dataLen % 16U;
    if (byteCount != 0U)
    {
        /* Write the input data */
        AES_writeDataBlocking(baseAddress, (ptrInput + (blockCount * 16U)), byteCount);

        /* Read the output data */
        AES_readDataBlocking(baseAddress, (ptrOutput + (blockCount * 16U)), byteCount);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function generates the MAC(message authentication code) using CBC-MAC and F9 mode.
 *      Note: The AES module must be configured for mode(CBC-MAC or F9), keys etc. before calling this function.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[out] ptrTag
 *      Pointer to the 16 byte tag data buffer.
 *  @param[in]  dataLen
 *      Data length of the input buffer in bytes.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_generateDataMAC(AESRegs* baseAddress, const uint8_t* ptrInput, uint8_t* ptrTag, uint32_t dataLen)
{
    uint32_t        blockCount;
    uint32_t        byteCount;
    uint32_t        index;

    /* Set the data length to trigger the engine to use the configured context */
    AES_setDataLength(baseAddress, (uint64_t)dataLen);

    /* Chunk the data into 16 byte blocks */
    blockCount = dataLen/16U;
    for (index = 0U; index < blockCount; index++)
    {
        /* Write the input data */
        AES_writeDataBlocking(baseAddress, (ptrInput + (index * 16U)), 16U);
    }

    /* Check for any remaining bytes */
    byteCount = dataLen % 16U;
    if (byteCount != 0U)
    {
        /* Write the input data */
        AES_writeDataBlocking(baseAddress, (ptrInput + (blockCount * 16U)), byteCount);
    }

    /* Wait for the context data regsiters to be ready */
    while ((AES_isReady (baseAddress, AES_CTRL_SVCTXTRDY)) == 0U);

    /* Read the hash tag value. */
    AES_readTag(baseAddress, ptrTag);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function processes(encrypt or decrypt) and authenticates a block of input data and stores the output data into a user
 *      specified output buffer. It also generates the MAC(message authentication code).
 *      Note: The AES module MUST be configured for mode(Only CCM and GCM modes should be used), keys etc. before calling this function.
 *      Note: The input data, output data and authentication data MUST be padded to the 16-byte boundary.
 *
 *  @param[in]  baseAddress
 *      Base address of AES module memory mapped registers.
 *  @param[in]  ptrInput
 *      Pointer to the input data buffer.
 *  @param[out]  ptrOutput
 *      Pointer to the output data buffer.
 *  @param[in]  dataLen
 *      Data length of the input buffer in bytes.
 *  @param[in]  ptrAuthInput
 *      Pointer to the authentication input data buffer.
 *  @param[out]  ptrTag
 *      Pointer to the 16 byte hash tag data buffer.
 *  @param[in]  authDataLen
 *      Data length of the authentication input data buffer in bytes.
 *
 *  \ingroup CRYPTO_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void AES_processDataAE(AESRegs* baseAddress, const uint8_t* ptrInput, uint8_t* ptrOutput,
                              uint32_t dataLen, const uint8_t* ptrAuthInput, uint8_t* ptrTag, uint32_t authDataLen)
{
    uint32_t        count = 0U;

    /* Set the data length to trigger the engine to use the configured context */
    AES_setDataLength(baseAddress, (uint64_t)dataLen);

    /* Set the additional authentication data length */
    AES_setAuthDataLength(baseAddress, authDataLen);

    /* Write authentication data blocks. The data is padded to the 16 byte boundary */
    while (count < authDataLen)
    {
        /* Write the input authentication data */
        AES_writeDataBlocking(baseAddress, (ptrAuthInput + count), 16U);
        count += 16U;
    }

    count = 0U;
    /* Process data blocks */
    while (count < dataLen)
    {
        /* Write the input data */
        AES_writeDataBlocking(baseAddress, (ptrInput + count), 16U);

        /* Read the output data */
        AES_readDataBlocking(baseAddress, (ptrOutput + count), 16U);
        count += 16U;
    }

    /* Wait for the context data regsiters to be ready */
    while ((AES_isReady (baseAddress, AES_CTRL_SVCTXTRDY)) == 0U);

    /* Read the hash tag value. */
    AES_readTag(baseAddress, ptrTag);

    return;
}


