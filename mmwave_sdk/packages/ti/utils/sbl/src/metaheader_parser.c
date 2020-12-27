/*
 *  @file metaheader_parser.c
 *  @brief Parses the meta header of all the RPRC images received from the host.
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
 *************************** Global Variables *****************************
 **************************************************************************/
#pragma DATA_ALIGN(metaHeaderLcl, 64);
uint32_t            metaHeaderLcl[8U + (SBL_NUM_FILES_SUPPORTED * 8U)] = {0U};

/**************************************************************************
 *************************** Function Definitions *************************
 **************************************************************************/
/*!
 *  @b Description
 *  @n
 *      This function extracts the image details from Meta Header.
 *
 *  @param[in] bufferPtrLcl
 *      Buffer pointer.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_imageDetailParse(uint32_t bufferPtrLcl[])
{
    uint32_t        numBytesRead = 0U;
    uint32_t        index;
    uint32_t        fileTypeLcl;

    for (index = 0U; index < gSblMCB.metaHeader.numFiles; index++)
    {
        fileTypeLcl = *bufferPtrLcl;
        gSblMCB.metaHeader.imageDetails[index].fileType = (uint8_t)fileTypeLcl;

        bufferPtrLcl++;
        numBytesRead += 4U;

        gSblMCB.metaHeader.imageDetails[index].magicWord = *bufferPtrLcl;
        bufferPtrLcl++;
        numBytesRead += 4U;


        gSblMCB.metaHeader.imageDetails[index].fileOffset = *bufferPtrLcl;
        bufferPtrLcl++;
        numBytesRead += 4U;

        gSblMCB.metaHeader.imageDetails[index].fileCRCHi = *bufferPtrLcl;
        bufferPtrLcl++;
        numBytesRead += 4U;

        gSblMCB.metaHeader.imageDetails[index].fileCRCLo = *bufferPtrLcl;
        bufferPtrLcl++;
        numBytesRead += 4U;

        gSblMCB.metaHeader.imageDetails[index].fileSize = *bufferPtrLcl;
        bufferPtrLcl++;
        numBytesRead += 4U;

        gSblMCB.metaHeader.imageDetails[index].filePresent = 1U;

        if ((((gSblMCB.metaHeader.imageDetails[index].magicWord & SBL_SUBSYSTEM_MASK) >> \
              SBL_SUBSYSTEM_SHIFT) == SBL_MSS_SUBSYSTEM) ||
            (((gSblMCB.metaHeader.imageDetails[index].magicWord & SBL_SUBSYSTEM_MASK) >> \
              SBL_SUBSYSTEM_SHIFT) == SBL_MSS_SUBSYSTEM_DEBUG))
        {
            gSblMCB.mssImagePresent = 1U;
        }

        /*read rsvd byte */
        bufferPtrLcl += 2U;
        numBytesRead += 8U;
    }
    return(numBytesRead);
}

/*!
 *  @b Description
 *  @n
 *      This function implements the parser for the meta header.
 *
 *  @param[in]  bufferPtr
 *      Pointer to the buffer containing the meta header.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
uint32_t SBL_metaHeaderParser(uint32_t bufferPtr[])
{
    uint32_t        numBytesRead = 0U;
    uint32_t        metaHeaderStart,metaHeaderEnd;
    uint64_t        crcResultCalculated, crcResultExpected;
    int8_t          status;
    uint32_t        bytesUsed;
    CRC_SigGenCfg   signGenCfg;
    int32_t         errCode;
    uint32_t*       metaHdrStartVal = bufferPtr;

    gSblMCB.metaHdrAbsent = 0U;

    metaHeaderStart = *bufferPtr;
    bufferPtr++;
    numBytesRead += 4U;

    gSblMCB.metaHeader.numFiles = *bufferPtr;
    bufferPtr++;
    numBytesRead += 4U;

    gSblMCB.metaHeader.devId = *bufferPtr;
    bufferPtr++;
    numBytesRead += 4U;

    gSblMCB.metaHeader.hdrCRCHi = *bufferPtr;
    bufferPtr++;
    numBytesRead += 4U;

    gSblMCB.metaHeader.hdrCRCLo = *bufferPtr;
    bufferPtr++;
    numBytesRead += 4U;

    gSblMCB.metaHeader.imageSize = *bufferPtr;
    bufferPtr++;
    numBytesRead += 4U;

    if (metaHeaderStart != SBL_META_HDR_START)
    {
        gSblMCB.metaHdrAbsent = 1U;
        gSblMCB.errorStatus |= SBL_METAHEADER_NOT_FOUND;
        numBytesRead = 0U;
    }
    else
    {
        /* Number of files should not exceed 4U */
        if (gSblMCB.metaHeader.numFiles > SBL_NUM_FILES_SUPPORTED)
        {
            gSblMCB.metaHdrAbsent = 1U;
            gSblMCB.errorStatus |= SBL_METAHEADER_NUMFILES_ERROR;
            numBytesRead = 0U;
        }
        else
        {
            bytesUsed = SBL_imageDetailParse(bufferPtr);
            bufferPtr += (bytesUsed / 4U);
            numBytesRead += bytesUsed;

            if (gSblMCB.mssImagePresent == 0U)
            {
                gSblMCB.metaHdrAbsent = 1U;
                gSblMCB.errorStatus |= SBL_MSSIMAGE_NOT_FOUND;
                numBytesRead = 0U;
            }
            else
            {
                gSblMCB.metaHeader.shMemAlloc = *bufferPtr;
                bufferPtr++;
                numBytesRead += 4U;

                metaHeaderEnd = *bufferPtr;
                bufferPtr++;
                numBytesRead += 4U;

                if(metaHeaderEnd != SBL_META_HDR_END)
                {
                    SBL_printf("Error: Meta header end mismatch\r\n");
                    DebugP_assert(0);
                };

                (void)memcpy((void*)metaHeaderLcl, (void *)metaHdrStartVal, numBytesRead);
                metaHeaderLcl[3U] = 0U;
                metaHeaderLcl[4U] = 0U;

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
                        SBL_printf ("Error: CRC Driver (Channel 2) open failed [Error code %d]\r\n", errCode);
                        DebugP_assert(0);
                    }
                }

                /* Compute Header CRC */
                signGenCfg.ptrData       = (uint8_t*)metaHeaderLcl;
                signGenCfg.dataLen       = numBytesRead;

                /* Allocate a unique transaction id: */
                if (CRC_getTransactionId(gSblMCB.crcHandle, &signGenCfg.transactionId, &errCode) < 0)
                {
                    SBL_printf ("Error: CRC Driver (Channel 2) get transaction id failed [Error code %d]\r\n", errCode);
                    DebugP_assert(0);
                }
                /* Compute the signature for the specific data on Channel-1 */
                if (CRC_computeSignature (gSblMCB.crcHandle, &signGenCfg, &errCode) < 0)
                {
                    SBL_printf ("Error: CRC Driver (Channel 2) compute signature failed [Error code %d]\r\n", errCode);
                    DebugP_assert(0);
                }

                /* Get the signature: This will give us the valid CRC signature and should also release
                 * the transaction identifier. */
                if (CRC_getSignature (gSblMCB.crcHandle, signGenCfg.transactionId, &crcResultCalculated, &errCode) < 0)
                {
                    SBL_printf ("Error: CRC Driver (Channel 2) Get signature failed [Error %d]\r\n", errCode);
                    DebugP_assert(0);
                }

				if (CRC_close (gSblMCB.crcHandle, &errCode) < 0)
				{
					SBL_printf ("Error: CRC Driver close failed [Error code %d]\r\n", errCode);
                    DebugP_assert(0);
				}
				gSblMCB.crcHandle = NULL;

                crcResultExpected = gSblMCB.metaHeader.hdrCRCLo;
                crcResultExpected = crcResultExpected | \
                                    (((uint64_t)(gSblMCB.metaHeader.hdrCRCHi)) << (uint64_t)32U);
                status = (crcResultCalculated == crcResultExpected)? 0 : MINUS_ONE;

                if (status < 0)
                {
                    gSblMCB.metaHdrAbsent = 1U;
                    gSblMCB.errorStatus |= SBL_METAHEADER_CRC_FAILURE;
                    numBytesRead = 0U;
                }
            }
        }
    }

    return(numBytesRead);
}

