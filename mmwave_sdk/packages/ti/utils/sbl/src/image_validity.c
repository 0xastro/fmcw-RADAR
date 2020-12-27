/*
 *  @file image_validity.c
 *  @brief This file contains routines that are used to validate all the
 *  RPRC images received from the host.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
/* Standard Include Files. */
#include <stdio.h>
#include <stdint.h>

/* MMWSDK include files */
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>

/* SBL internal include file. */
#include <ti/utils/sbl/include/sbl_internal.h>

/**************************************************************************
 *************************** Function Definitions *************************
 **************************************************************************/
static uint32_t SBL_computeDecryptLength(uint32_t remainingBytes);
static uint32_t SBL_computeAuthLength(uint32_t remainingBytes);
static void SBL_imageAuthErrorStatus(int32_t status);
static int32_t SBL_imageAuthentication(uint32_t *inputBuffer, \
                               uint32_t inpBufLen, uint8_t fileType, uint32_t blkType);
static int32_t SBL_decryptStateExec(void);

/*!
 *  @b Description
 *  @n
 *      This function computes the current length to decrypt.
 *
 *  @param[in]  remainingBytes
 *      Number of bytes in buffer.
 *
 *  @retval
 *      Computed decrypt length.
 */
static uint32_t SBL_computeDecryptLength(uint32_t remainingBytes)
{
    uint32_t        returnLength = 0U;

    /* If it is the first block of the Image */
    if (gSblMCB.decryptProcess.decryptSizeRead == 0U)
    {
        returnLength = ((remainingBytes >= gSblMCB.decryptProcess.decryptLength) ? \
                                          gSblMCB.decryptProcess.decryptLength : remainingBytes);
        if (returnLength == gSblMCB.decryptProcess.decryptLength)
        {
            gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_ALIGN;
        }
        else
        {
            gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_PROCESS;
        }
    }

    /* If it is the intermediate block of the Image */
    else if (gSblMCB.decryptProcess.decryptLength > (remainingBytes + gSblMCB.decryptProcess.decryptSizeRead))
    {
        returnLength = remainingBytes;
        gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_PROCESS;
    }

    /* If it is the last block of the Image */
    else if (gSblMCB.decryptProcess.decryptLength <= (remainingBytes + gSblMCB.decryptProcess.decryptSizeRead))
    {
        returnLength = (gSblMCB.decryptProcess.decryptLength - gSblMCB.decryptProcess.decryptSizeRead);
        gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_ALIGN;
    }
    else
    {
        /* Do nothing */
    }

    return(returnLength);

}

/*!
 *  @b Description
 *  @n
 *      This function calculates the authentication length.
 *
 *  @param[in] remainingBytes
 *      Number of bytes in buffer.
 *
 *  @retval
 *      Computed authentication length.
 */
static uint32_t SBL_computeAuthLength(uint32_t remainingBytes)
{
    uint32_t        returnLength = 0U;

    /*  Remaining length for the authentication process */
    if (gSblMCB.authProcess.authSizeRead == 0U)
    {
        returnLength = (remainingBytes >= gSblMCB.authProcess.authSize) ? \
                                          gSblMCB.authProcess.authSize : remainingBytes;
        if (returnLength == gSblMCB.authProcess.authSize)
        {
            gSblMCB.authProcess.authState = SBL_AUTH_STATE_ALIGN;
            /* What if the first blk is also final? */
            gSblMCB.authProcess.authBlkType = 0x3U;
        }
        else
        {
            gSblMCB.authProcess.authState = SBL_AUTH_STATE_PROCESS;
            gSblMCB.authProcess.authBlkType = 0x00;
        }
    }
    else if (gSblMCB.authProcess.authSize > (remainingBytes + gSblMCB.authProcess.authSizeRead))
    {
        returnLength = remainingBytes;
        gSblMCB.authProcess.authState = SBL_AUTH_STATE_PROCESS;
        gSblMCB.authProcess.authBlkType = 0x01U;
    }
    else if (gSblMCB.authProcess.authSize <= (remainingBytes + gSblMCB.authProcess.authSizeRead))
    {
        returnLength = (gSblMCB.authProcess.authSize - gSblMCB.authProcess.authSizeRead);
        gSblMCB.authProcess.authState = SBL_AUTH_STATE_ALIGN;
        gSblMCB.authProcess.authBlkType = 0x02U;
    }
    else
    {
        /* Do nothing */
    }

    return(returnLength);
}

/*!
 *  @b Description
 *  @n
 *      Sets the authentication error status.
 *
 *  @retval
 *      Not applicable.
 */
static void SBL_imageAuthErrorStatus(int32_t status)
{
    if (status < 0)
    {
        switch (gSblMCB.authProcess.numFilesWritten)
        {
            case 1U:
                    gSblMCB.errorStatus |= SBL_RPRC_IMG1_AUTH_FAILURE;
                    break;
            case 2U:
                    gSblMCB.errorStatus |= SBL_RPRC_IMG2_AUTH_FAILURE;
                    break;
            case 3U:
                    gSblMCB.errorStatus |= SBL_RPRC_IMG3_AUTH_FAILURE;
                    break;
            case 4U:
                    gSblMCB.errorStatus |= SBL_RPRC_IMG4_AUTH_FAILURE;
                    break;
            default:
                    gSblMCB.errorStatus |= SBL_RPRC_IMG1_AUTH_FAILURE;
                    break;
        }
    }
    else
    {
        /*Do nothng*/
    }
}

/*!
 *  @b Description
 *  @n
 *      This function authenticates the image being downloaded.
 *      @note This should be used only for HS devices.
 *
 *  @param[in]  inputBuffer
 *      Pointer to input data buffer.
 *  @param[in]  inpBufLen
 *      Length of the input buffer.
 *  @param[in]  fileType
 *      The ID of the file being authenticated.
 *  @param[in]  blkType
 *      Block type.
 *      Valid values are: FIRST, INTERMEDIATE, FINAL
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   < 0
 */
static int32_t SBL_imageAuthentication(uint32_t *inputBuffer, \
                               uint32_t inpBufLen, uint8_t fileType, uint32_t blkType)
{
    uint64_t        crcResultCalculated, crcResultExpected;
    int32_t         status = MINUS_ONE;
    CRC_SigGenCfg   signGenCfg;
    int32_t         errCode;
    uint32_t        imagValTransId = 0;

    if(gSblMCB.crcHandle == NULL)
    {
        CRC_Config crcCfg;
        /* Initialize the default CRC Configuration: */
        CRC_initConfigParams(&crcCfg);
        crcCfg.channel = CRC_Channel_CH2;
        crcCfg.mode = CRC_Operational_Mode_FULL_CPU;
        crcCfg.type = CRC_Type_64BIT;
        crcCfg.bitSwap = CRC_BitSwap_MSB;
        crcCfg.byteSwap = CRC_ByteSwap_ENABLED;
        crcCfg.dataLen = CRC_DataLen_64_BIT;

        /* Open the CRC Driver: */
        gSblMCB.crcHandle = CRC_open (&crcCfg, &errCode);
        if (gSblMCB.crcHandle == NULL)
        {
            SBL_printf ("Error: CRC Driver (Channel 1) open failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
        }
    }

	/*Check if image fits into the full buffer*/
	if (blkType == 0x03U)
	{
        signGenCfg.ptrData       = (uint8_t*)inputBuffer;
        signGenCfg.dataLen       = gSblMCB.authProcess.authSize;

        /* Allocate a unique transaction id: */
        if (CRC_getTransactionId(gSblMCB.crcHandle, &imagValTransId, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (blk3) get transaction id failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
        }
		signGenCfg.transactionId = imagValTransId;

        /* Compute the signature for the specific data on Channel-1 */
        if (CRC_computeSignature (gSblMCB.crcHandle, &signGenCfg, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (blk3) compute signature failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
        }

		/* Get the signature: This will give us the valid CRC signature and should also release
         * the transaction identifier. */
        if (CRC_getSignature (gSblMCB.crcHandle, signGenCfg.transactionId, &crcResultCalculated, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (blk3) Get signature failed [Error %d]\r\n", errCode);
            DebugP_assert(0);
        }

		if (CRC_close (gSblMCB.crcHandle, &errCode) < 0)
		{
			SBL_printf ("Error: CRC Driver close failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
		}
		gSblMCB.crcHandle = NULL;
		crcResultExpected = \
		gSblMCB.metaHeader.imageDetails[(gSblMCB.authProcess.numFilesWritten - 1U)].fileCRCLo;
		crcResultExpected = crcResultExpected |
            (((uint64_t)((uint64_t)gSblMCB.metaHeader.imageDetails[(gSblMCB.authProcess.numFilesWritten - 1U)] \
														   .fileCRCHi)) << 32U);
		status = (crcResultCalculated == crcResultExpected)? 0 : MINUS_ONE;
	}
	/* Check if it is the first buffer for CRC computation */
	else if (blkType == 0x00U)
	{
        signGenCfg.ptrData       = (uint8_t*)inputBuffer;
        signGenCfg.dataLen       = inpBufLen;

        /* Allocate a unique transaction id: */
        if (CRC_getTransactionId(gSblMCB.crcHandle, &imagValTransId, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (blk0) get transaction id failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
        }
		signGenCfg.transactionId = imagValTransId;
        /* Compute the signature for the specific data on Channel-1 */
        if (CRC_computeSignature (gSblMCB.crcHandle, &signGenCfg, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (blk0) compute signature failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
        }
		status = 0;
	}
	else if (blkType == 0x01U)
	{
        signGenCfg.ptrData       = (uint8_t*)inputBuffer;
        signGenCfg.dataLen       = inpBufLen;

		signGenCfg.transactionId = imagValTransId;
        /* Compute the signature for the specific data on Channel-1 */
        if (CRC_computeSignature (gSblMCB.crcHandle, &signGenCfg, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (blk1) compute signature failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
        }
        status = 0;
	}
	else if (blkType == 0x02U)
	{
        signGenCfg.ptrData       = (uint8_t*)inputBuffer;
        signGenCfg.dataLen       = inpBufLen;

		signGenCfg.transactionId = imagValTransId;
        /* Compute the signature for the specific data on Channel-1 */
        if (CRC_computeSignature (gSblMCB.crcHandle, &signGenCfg, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (blk2) compute signature failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
        }

		/* Get the signature: This will give us the valid CRC signature and should also release
         * the transaction identifier. */
        if (CRC_getSignature (gSblMCB.crcHandle, signGenCfg.transactionId, &crcResultCalculated, &errCode) < 0)
        {
            SBL_printf ("Error: CRC Driver (Channel 1) Get signature failed [Error %d]\r\n", errCode);
            DebugP_assert(0);
        }

		if (CRC_close (gSblMCB.crcHandle, &errCode) < 0)
		{
			SBL_printf ("Error: CRC Driver close failed [Error code %d]\r\n", errCode);
            DebugP_assert(0);
		}
		gSblMCB.crcHandle = NULL;

		crcResultExpected = \
		   gSblMCB.metaHeader.imageDetails[(gSblMCB.authProcess.numFilesWritten - 1U)].fileCRCLo;
		crcResultExpected = crcResultExpected | (((uint64_t) \
	    ((uint64_t)gSblMCB.metaHeader.imageDetails[(gSblMCB.authProcess.numFilesWritten - 1U)].fileCRCHi)) << 32U);
		status = (crcResultCalculated == crcResultExpected)? 0 : MINUS_ONE;
	}
	else
	{
		/* Error */
	}
	/* Perform Authentication only if test port is disabled. */
	SBL_imageAuthErrorStatus(status);

    return status;
}

/*!
 *  @b Description
 *  @n
 *      This functions Starts the decrypt process and sets the parameters
 *      for the next block of decryption.
 *
 *  @retval
 *      Returns 0
 */
static int32_t SBL_decryptStateExec(void)
{
       gSblMCB.decryptProcess.numFilesWritten++;
	   gSblMCB.decryptProcess.decryptLength = \
		   gSblMCB.metaHeader.imageDetails[gSblMCB.decryptProcess.numFilesWritten - 1U].fileSize;
	   gSblMCB.decryptProcess.imageId = \
	   ((gSblMCB.metaHeader.imageDetails[gSblMCB.decryptProcess.numFilesWritten - 1U].magicWord \
														   & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT);

       if ((gSblMCB.decryptProcess.decryptLength % 16U) != 0U)
       {
            gSblMCB.decryptProcess.decryptLength += (16U - (gSblMCB.decryptProcess.decryptLength % 16U));
       }

       gSblMCB.decryptProcess.decryptSizeRead = 0U;

       if ((gSblMCB.decryptProcess.decryptLength % 64U) != 0U)
       {
            gSblMCB.decryptProcess.numAlignBytes = (64U - (gSblMCB.decryptProcess.decryptLength % 64U));
       }
       else
       {
            gSblMCB.decryptProcess.numAlignBytes = 0U;
       }

       gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_PROCESS;
       return 0;
}

/*!
 *  @b Description
 *  @n
 *      This function resets the decryption parameters.
 *
 *  @retval
 *      Not applicable.
 */
void SBL_imageDecryptParamsInit(void)
{
    gSblMCB.decryptProcess.decryptLength = 0U;
    gSblMCB.decryptProcess.decryptSizeRead = 0U;
    gSblMCB.decryptProcess.numAlignBytes = 0U;
    gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_START;
    gSblMCB.decryptProcess.imageId = 0U;
    gSblMCB.decryptProcess.numFilesWritten = 0U;
}

/*!
 *  @b Description
 *  @n
 *      This function decrypts the data if encrypted.
 *  @param[in]  cipherText
 *      Pointer to the input cipher text.
 *  @param[in]  decryptedData
 *      Pointer to the decrypted data.
 *  @param[in]  dataLength
 *      Length of the cipher text.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   SBL Error code
 */
int32_t SBL_decryptImage(uint8_t *cipherText, uint8_t *decryptedData, uint32_t dataLength)
{
   uint32_t         remainingBytes = dataLength;
   uint32_t         bytesRead = 0U;
   int32_t          status = MINUS_ONE;

   do
   {
       switch (gSblMCB.decryptProcess.decryptState)
       {
           /* If it is the intermediate block of the Image */
           case SBL_DECRYPT_STATE_START:
           {
               status = SBL_decryptStateExec();
               break;
           }

           /* Start decrypt process for the block. */
           case SBL_DECRYPT_STATE_PROCESS:
           {
               bytesRead = SBL_computeDecryptLength(remainingBytes);
				/* unencrypted */
			   (void)memcpy(decryptedData, cipherText, bytesRead);
			   status = 0;
               remainingBytes -= bytesRead;
               cipherText += (bytesRead);
               decryptedData += (bytesRead);
               gSblMCB.decryptProcess.decryptSizeRead += bytesRead;
               break;
           }

           /* Look for alignment bytes prent, if yes skip. */
           case SBL_DECRYPT_STATE_ALIGN:
           {
               if (gSblMCB.decryptProcess.numAlignBytes != 0U)
               {
                   if (remainingBytes >= gSblMCB.decryptProcess.numAlignBytes)
                   {
                       bytesRead = gSblMCB.decryptProcess.numAlignBytes;
                       gSblMCB.decryptProcess.numAlignBytes = 0U;
                       gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_START;
                   }
                   else
                   {
                       bytesRead = remainingBytes;
                       gSblMCB.decryptProcess.numAlignBytes -= remainingBytes;
                       gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_ALIGN;
                   }
                   remainingBytes -= bytesRead;
                   cipherText += (bytesRead);
                   decryptedData += (bytesRead);
               }
               else
               {
                   gSblMCB.decryptProcess.decryptState = SBL_DECRYPT_STATE_START;
               }
               status = 0;

               break;
           }
           default:
                   /* Do nothing */
                   status = MINUS_ONE;
                   break;
       }
   }
   while (remainingBytes != 0U);

   return(status);
}

/*!
 *  @b Description
 *  @n
 *      Initializes the decryption parameters to reset state.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void SBL_imageAuthParamsInit(void)
{
    gSblMCB.authProcess.authSize = 0U;
    gSblMCB.authProcess.authSizeRead = 0U;
    gSblMCB.authProcess.numAlignBytes = 0U;
    gSblMCB.authProcess.authState = SBL_AUTH_STATE_START;
    gSblMCB.authProcess.authBlkType = 0x00U;
    gSblMCB.authProcess.noFileIncr = 0U;
    gSblMCB.authProcess.numFilesWritten = 0U;
}

/*!
 *  @b Description
 *  @n
 *      Authenticates and validates the image.
 *
 *  @param[in] readPtr
 *       Input data that has to be validated.
 *  @param[in] bufferLen
 *      Data length in bytes.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   SBL Error code
 */
int32_t SBL_imageValidationCheck(uint32_t* readPtr, uint32_t bufferLen)
{
    uint32_t        remainingBytes = bufferLen;
    uint32_t        bytesRead = 0U;
    int32_t         status;
    uint32_t*       readPtrLcl = readPtr;
    do
    {
        switch (gSblMCB.authProcess.authState)
        {
            case SBL_AUTH_STATE_START:
            {
                gSblMCB.authProcess.numFilesWritten++;
                gSblMCB.authProcess.authBlkType = 0x00U;

				gSblMCB.authProcess.authSize = gSblMCB.metaHeader.imageDetails[gSblMCB.authProcess.numFilesWritten - 1U].fileSize;
                gSblMCB.authProcess.authSizeRead = 0U;
                if ((gSblMCB.authProcess.authSize % 64U) != 0U)
                {
                    gSblMCB.authProcess.numAlignBytes = (64U - (gSblMCB.authProcess.authSize % 64U));
                }
                else
                {
                    gSblMCB.authProcess.numAlignBytes = 0U;
                }
                gSblMCB.authProcess.authState = SBL_AUTH_STATE_PROCESS;
                gSblMCB.authProcess.noFileIncr = 0U;
                if (remainingBytes < SBL_RPRC_HDR_LENGTH)
                {
                    gSblMCB.authProcess.noFileIncr = 1U;
                }
                status = 0;
                break;
            }
            case SBL_AUTH_STATE_PROCESS:
            {
                bytesRead = SBL_computeAuthLength(remainingBytes);
                status = SBL_imageAuthentication(readPtrLcl, bytesRead,
                            gSblMCB.authProcess.numFilesWritten, gSblMCB.authProcess.authBlkType);

                remainingBytes -= bytesRead;
                readPtrLcl += (bytesRead / 4U);
                gSblMCB.authProcess.authSizeRead += bytesRead;
                break;
            }
            case SBL_AUTH_STATE_ALIGN:
            {
                if (gSblMCB.authProcess.numAlignBytes != 0U)
                {
                    if (remainingBytes >= gSblMCB.authProcess.numAlignBytes)
                    {
                        bytesRead = gSblMCB.authProcess.numAlignBytes;
                        gSblMCB.authProcess.numAlignBytes = 0U;
                        gSblMCB.authProcess.authState = SBL_AUTH_STATE_START;
                    }
                    else
                    {
                        bytesRead = remainingBytes;
                        gSblMCB.authProcess.numAlignBytes -= bytesRead;
                        gSblMCB.authProcess.authState = SBL_AUTH_STATE_ALIGN;
                    }
                    remainingBytes -= bytesRead;
                    readPtrLcl += (bytesRead / 4U);
                }
                else
                {
                    gSblMCB.authProcess.authState = SBL_AUTH_STATE_START;
                }
                status = 0;
                break;
            }
            default:
                /* Do nothing */
                status = SBL_EINVAL;
                break;
        }
    }
    while (remainingBytes != 0U);

    return status;

}

