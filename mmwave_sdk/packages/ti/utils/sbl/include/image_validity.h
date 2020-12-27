/*
 *  @file image_validity.h
 *  @brief This file contains all the data structures that are used to validate
 *  all the RPRC images received from the host.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

#ifndef IMAGE_VALIDITY_H
#define IMAGE_VALIDITY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SBL_INTERNAL_DATA_STRUCTURE
 @{ */

/*!
 *  @brief Authentication states.
 */
typedef enum SBL_AUTH_STATE_t
{
    SBL_AUTH_STATE_START = 0,       /**< Auth State start.      */
    SBL_AUTH_STATE_PROCESS,         /**< Auth state process.    */
    SBL_AUTH_STATE_END,             /**< Auth state end.        */
    SBL_AUTH_STATE_ALIGN            /**< Auth state align.      */
}SBL_AUTH_STATE;

/*!
 *  @brief Authentication states.
 */
typedef enum SBL_DECRYPT_STATE_t
{
    SBL_DECRYPT_STATE_START = 0,    /**< Decrypt State start.      */
    SBL_DECRYPT_STATE_PROCESS,      /**< Decrypt state process.    */
    SBL_DECRYPT_STATE_END,          /**< Decrypt state end.        */
    SBL_DECRYPT_STATE_ALIGN         /**< Decrypt state align.      */
}SBL_DECRYPT_STATE;

/*!
 *  @brief Authentication state m/c parameters.
 */
typedef struct SBL_AuthProcess_t
{
    uint32_t authSize;              /**< Image size to authenticated.         */
    uint32_t authSizeRead;          /**< Image size processed.                */
    uint32_t numAlignBytes;         /**< Number of alignment bytes.           */
    uint8_t  authState;             /**< Auth state m/c state.                */
    uint8_t  authBlkType;           /**< Auth current blk type.               */
    uint8_t  noFileIncr;            /**< Flag to skip numFiles incrementing.  */
    uint8_t  numFilesWritten;       /**< Number of images processed.          */
}SBL_AuthProcess;

/*!
 *  @brief Decryption state m/c parameters.
 */
typedef struct SBL_DecryptProcess_t
{
    uint32_t decryptLength;         /**< Image size to decrypted.     */
    uint32_t decryptSizeRead;       /**< Image size processed.        */
    uint32_t numAlignBytes;         /**< Number of alignment bytes.   */
    uint32_t imageId;               /**< Image id.                    */
    uint8_t  decryptState;          /**< Decrypt state m/c state.     */
    uint8_t  numFilesWritten;       /**< Number of images processed.  */
}SBL_DecryptProcess;

/**
@}
*/

extern void SBL_imageDecryptParamsInit(void);
extern int32_t SBL_decryptImage(uint8_t *cipherText, uint8_t *decryptedData, uint32_t dataLength);
extern void SBL_imageAuthParamsInit(void);
extern int32_t SBL_imageValidationCheck(uint32_t* readPtr, uint32_t bufferLen);

#ifdef __cplusplus
}
#endif
#endif

