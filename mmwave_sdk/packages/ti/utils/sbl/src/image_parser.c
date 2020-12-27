/*
 *  @file image_parser.c
 *  @brief Parses all the RPRC images received from the host.
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
#include <string.h>

/* SBL internal include file. */
#include <ti/utils/sbl/include/sbl_internal.h>

/**************************************************************************
 **************************** Extern Definitions **************************
 **************************************************************************/
extern uint32_t SBL_calculateImageOffset(uint32_t sectionPtr, uint32_t sectionLen);
extern void SBL_mpuConfigBSS(bool enable);


/**************************************************************************
 *************************** Function Definitions *************************
 **************************************************************************/
static void SBL_applyConfigFile(uint32_t *bufferPtr, uint32_t bufferLen);
static void SBL_updateVarHeaderStateEnd(void);
static void SBL_readRPRCHeaderStateUpdate(uint32_t bytesRead);
static uint32_t SBL_extractRPRCHeader(uint32_t readPtrLcl[], uint32_t bufferLen);
static uint32_t SBL_readRPRCHeader(uint32_t* bufferPtr, uint32_t bufferLenIp);
static uint32_t SBL_handleSectionDwldComplete(void);
static uint32_t SBL_readRPRCSecContent(uint32_t* bufferPtr, uint32_t bufferLenIp);
static uint32_t SBL_readAlignmentBytes(uint32_t* bufferPtr, uint32_t bufferLenIp);
static uint32_t SBL_extractRPRCSecHeader(uint32_t readPtrLcl[]);
static uint32_t SBL_updateVarSectionHeaderStateEnd(void);
static uint32_t SBL_readRPRCSecHeader(uint32_t* bufferPtr, uint32_t bufferLenIp);


/*!
 *  @b Description
 *  @n
 *      Apply config file. This function implements the state m/c for the RPRC image parser.
 *
 *  @param[in]  bufferPtr
 *      Config file buffer pointer.
 *  @param[in] bufferLen
 *      Data length.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static void SBL_applyConfigFile(uint32_t *bufferPtr, uint32_t bufferLen)
{
    uint32_t        remainingBytes;
    uint32_t*       writeAddr;
    uint32_t        writeValue;
    uint32_t*       ptrLcl = bufferPtr;

    remainingBytes = bufferLen;
    do
    {

        writeAddr = (uint32_t *)*ptrLcl;
        ptrLcl++;
        writeValue = *ptrLcl;
        ptrLcl++;

        /* Write the configuration */
        if (((uint32_t)writeAddr >= 0xFFFFE2C4U) && ((uint32_t)writeAddr <= 0xFFFFE2D0U))
        {
            /* Do not apply the configuration if accessing the secure registers */
        }
        else
        {
            *writeAddr = writeValue;
        }
        remainingBytes -= 8U;

    }
    while (remainingBytes != 0U);

    return;
}

/*!
 *  @b Description
 *  @n
 *      Used to update globals at the end of RPRC header.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable.
 */
static void SBL_updateVarHeaderStateEnd(void)
{
    /* Populate the RPRC file size, if header read is complete. */
    if (gSblMCB.imageParams.hdrState == SBL_RPRC_HDR_STATE_END)
    {
		gSblMCB.imageParams.fileSize = gSblMCB.metaHeader.imageDetails[gSblMCB.numFilesWritten - 1U].fileSize;
		gSblMCB.imageParams.fileType = gSblMCB.metaHeader.imageDetails[gSblMCB.numFilesWritten - 1U].magicWord;
        if ((gSblMCB.imageParams.fileSize % 64U) != 0U)
        {
            gSblMCB.imageParams.numAlignBytes = (64U - (gSblMCB.imageParams.fileSize % 64U));
        }
        else
        {
			gSblMCB.imageParams.numAlignBytes = 0U;
        }
    }
}

/*!
 *  @b Description
 *  @n
 *      Used to read RPRC header header state update.
 *
 *  @param[in]  bytesRead
 *      Number of bytes used.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable.
 */
static void SBL_readRPRCHeaderStateUpdate(uint32_t bytesRead)
{
    if ((gSblMCB.errorStatus & SBL_RPRC_HDR_NOT_FOUND) == 0U)
    {
        SBL_updateVarHeaderStateEnd();
        gSblMCB.imageParams.imageSizeRead +=  bytesRead;
    }
}

/*!
 *  @b Description
 *  @n
 *      Used to extract the RPRC header.
 *
 *  @param[in]  readPtrLcl
 *      Buffer pointer.
 *  @param[in]  bufferLen
 *      Buffer length.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_extractRPRCHeader(uint32_t readPtrLcl[], uint32_t bufferLen)
{
    uint32_t        bytesRead;
    uint32_t*       ptrLcl = readPtrLcl;
    uint32_t        header;

    header = *ptrLcl;
    if (header != SBL_RPRC_MAGIC_WORD)
    {
        gSblMCB.errorStatus |= SBL_RPRC_HDR_NOT_FOUND;
        bytesRead = bufferLen;
    }
    else
    {
        ptrLcl++;
        bytesRead = 4U;

        /* Reading entryPoint 0 */
        ptrLcl++;
        bytesRead += 4U;

        /* Reading entryPoint 1 */
        ptrLcl++;
        bytesRead += 4U;

        gSblMCB.imageParams.numSections = (uint32_t)*ptrLcl;
        ptrLcl++;
        bytesRead += 4U;

        gSblMCB.imageParams.numSectionsWritten = 0U;

        /* Reading the version number */
        ptrLcl++;
        bytesRead += 4U;

        /* Reading the rsvd Header */
        ptrLcl++;
        bytesRead += 4U;
        gSblMCB.imageParams.hdrState = SBL_RPRC_HDR_STATE_END;
        gSblMCB.imageParams.state  = SBL_RPRC_PARSER_SECTION_HEADER;
        gSblMCB.numFilesWritten++;
    }
    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      RPRC Image header parser. This function implements the state m/c for the RPRC image header parser.
 *
 *  @param[in]  bufferPtr
 *      Pointer to the buffer used for parsing.
 *  @param[in]  bufferLenIp
 *      Buffer length in bytes.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_readRPRCHeader(uint32_t* bufferPtr, uint32_t bufferLenIp)
{
    uint32_t        bufferLen =  bufferLenIp;
    uint32_t*       ptrLcl = bufferPtr;
    uint32_t        bytesRead = 0U;
    uint32_t        header;

    /* If the length of the available buffer is >= to the size of the RPRCheader, read the
      header in one go. Else, we mark the state of the RPRC header expected in the next
      input buffer */
    if ((bufferLenIp >= SBL_RPRC_HDR_LENGTH) && (gSblMCB.imageParams.hdrState == SBL_RPRC_HDR_STATE_MAGIC))
    {
        bytesRead = SBL_extractRPRCHeader(bufferPtr, bufferLen);
    }
    else
    {
        while ((bufferLen - bytesRead) >= 8U)
        {
            switch (gSblMCB.imageParams.hdrState)
            {
                case SBL_RPRC_HDR_STATE_MAGIC:
                {
                    /* Read the header to check for "RPRC" */
                    header = *ptrLcl;
                    if (header != SBL_RPRC_MAGIC_WORD)
                    {
                        gSblMCB.errorStatus |= SBL_RPRC_HDR_NOT_FOUND;
                        bytesRead = bufferLenIp;
                    }
                    else
                    {
                        ptrLcl++;
                        bytesRead += 4U;
                        ptrLcl++;
                        bytesRead += 4U;
                        gSblMCB.imageParams.hdrState = SBL_RPRC_HDR_STATE_LEN;
                    }
                    break;
                }
                case SBL_RPRC_HDR_STATE_LEN:
                {
                    /* Reading entryPoint 0 */
                    ptrLcl++;
                    bytesRead += 4U;

                    gSblMCB.imageParams.numSections = (uint32_t)*ptrLcl;
                    ptrLcl++;
                    bytesRead += 4U;
                    gSblMCB.imageParams.hdrState = SBL_RPRC_HDR_STATE_RSVD;
                    break;
                }
                case SBL_RPRC_HDR_STATE_RSVD:
                {
                    /* Reading the version number */
                    ptrLcl++;
                    bytesRead += 4U;

                    /* Reading the rsvd Header */
                    ptrLcl++;
                    bytesRead += 4U;
                    gSblMCB.imageParams.hdrState = SBL_RPRC_HDR_STATE_END;
                    gSblMCB.imageParams.state  = SBL_RPRC_PARSER_SECTION_HEADER;
                    gSblMCB.numFilesWritten++;
                    break;
                }
                case SBL_RPRC_HDR_STATE_END:
                {
                    /* RPRC Header done. */
                    break;
                }
                default:
                {
                   /* Default */
                   break;
                }
            }
            if (gSblMCB.imageParams.hdrState == SBL_RPRC_HDR_STATE_END)
            {
                /* Break out of the while loop */
               break;
            }
        }
    }

    SBL_readRPRCHeaderStateUpdate(bytesRead);
    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      Used to update globals on sectionDwld complete.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_handleSectionDwldComplete(void)
{
    uint32_t        bytesRead;

    gSblMCB.imageParams.numSectionsWritten++;

    if (gSblMCB.imageParams.numSections > gSblMCB.imageParams.numSectionsWritten)
    {
        gSblMCB.imageParams.state = SBL_RPRC_PARSER_SECTION_HEADER;
        memset((void*)&gSblMCB.imageParams.sectionParams, 0U, sizeof(SBL_sectionParams));
        bytesRead = 1U;
    }
    else
    {
        if (gSblMCB.imageParams.imageSizeRead != gSblMCB.imageParams.fileSize)
        {
            gSblMCB.errorStatus |= SBL_RPRC_PARSER_FILE_LENGTH_MISMATCH;
            bytesRead = 0U;
        }
        else
        {
            if ((((gSblMCB.imageParams.fileType & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT) == SBL_CONFIG_SUBSYSTEM))
            {
                SBL_applyConfigFile((uint32_t *)&gSblMCB.configFile[0U],
                            (uint32_t)gSblMCB.imageParams.sectionParams.secLen);
            }

            /* Disable the MPU setting for BSS regions */
            if (((gSblMCB.imageParams.fileType & SBL_SUBSYSTEM_MASK) >> SBL_SUBSYSTEM_SHIFT) == SBL_BSS_SUBSYSTEM)
            {
                SBL_mpuConfigBSS(false);
            }

            if (gSblMCB.imageParams.numAlignBytes != 0U)
            {
                gSblMCB.imageParams.state = SBL_RPRC_PARSER_ALIGNMENT_BYTES;
            }
            else
            {
                gSblMCB.imageParams.state = SBL_RPRC_PARSER_STATE_IDLE;
                memset((void*)&gSblMCB.imageParams, 0U, sizeof(SBL_ImageParams));
            }
            bytesRead = 1U;
        }
    }

    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      This function process the RPRC image section content.
 *
 *  @param[in]  bufferPtr
 *      Pointer to the buffer that contains data for parsing.
 *  @param[in] bufferLenIp
 *      Buffer length in bytes.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_readRPRCSecContent(uint32_t* bufferPtr, uint32_t bufferLenIp)
{
    uint32_t        bufferLen =  bufferLenIp;
    uint32_t        bytesRead = 0U;
    uint32_t*       ptrLcl = bufferPtr;
    uint32_t        size;
    uint32_t        retVal;

    /* For downloading the section content */
    if (gSblMCB.imageParams.sectionParams.secLen > gSblMCB.imageParams.sectionParams.writtenLen)
    {
        if ((bufferLen - bytesRead) >= (gSblMCB.imageParams.sectionParams.secLen - gSblMCB.imageParams.sectionParams.writtenLen))
        {
            size = (gSblMCB.imageParams.sectionParams.secLen - gSblMCB.imageParams.sectionParams.writtenLen);
        }
        else
        {
            size = (bufferLen - bytesRead);
        }

        if(gSblMCB.imageParams.sectionParams.sectionPtr != 0)
        {
            memcpy((void*)gSblMCB.imageParams.sectionParams.sectionPtr,
                            (const void*)ptrLcl, size);
        }
        else
        {
            memcpy((void*)&gSblMCB.sblIntVecTable[0], (const void*)ptrLcl, 64U);
        }

        gSblMCB.imageParams.sectionParams.writtenLen += size;
        gSblMCB.imageParams.sectionParams.sectionPtr += size / 4U;
        ptrLcl += (size / 4U);
        bytesRead += size;

        gSblMCB.imageParams.imageSizeRead +=  bytesRead;
    }
    if (gSblMCB.imageParams.sectionParams.secLen == gSblMCB.imageParams.sectionParams.writtenLen)
    {
        retVal = SBL_handleSectionDwldComplete();
        if (retVal == 0U)
        {
            bytesRead = bufferLen;
        }
    }
    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      This function returns number of alignment bytes used.
 *
 *  @param[in]  bufferPtr
 *      Pointer to the buffer that contains data for parsing.
 *  @param[in] bufferLenIp
 *      Buffer length in bytes.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_readAlignmentBytes(uint32_t* bufferPtr, uint32_t bufferLenIp)
{
    uint32_t        bytesRead;
    uint32_t*       ptrLcl = bufferPtr;

    if (bufferLenIp >= gSblMCB.imageParams.numAlignBytes)
    {
        ptrLcl += (gSblMCB.imageParams.numAlignBytes / 4U);
        bytesRead = gSblMCB.imageParams.numAlignBytes;
        gSblMCB.imageParams.state = SBL_RPRC_PARSER_STATE_IDLE;
        memset((void*)&gSblMCB.imageParams, 0U, sizeof(SBL_ImageParams));
    }
    else
    {
        ptrLcl += (bufferLenIp / 4U);
        bytesRead = bufferLenIp;
        gSblMCB.imageParams.numAlignBytes -= bufferLenIp;
        gSblMCB.imageParams.state = SBL_RPRC_PARSER_ALIGNMENT_BYTES;
    }
    return bytesRead;
}

/*!
 *  @b Description
 *  @n
 *      This function is used to extract the section header.
 *
 *  @param[in]  readPtrLcl
 *      Buffer pointer
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_extractRPRCSecHeader(uint32_t readPtrLcl[])
{
    uint32_t        bytesRead;
    uint32_t*       ptrLcl = readPtrLcl;

    gSblMCB.imageParams.sectionParams.secStrtAddr = *(uint64_t*)ptrLcl;

    ptrLcl += 2U;
    bytesRead = 8U;

    gSblMCB.imageParams.sectionParams.secLen = (uint32_t)*ptrLcl;
    ptrLcl++;
    bytesRead += 4U;

    /* Reading rsvd Sec Header 0 */
    ptrLcl++;
    bytesRead += 4U;

    /* Reading rsvd Sec Header 1 */
    ptrLcl++;
    bytesRead += 4U;

    /* Reading rsvd Sec Header 2 */
    ptrLcl++;
    bytesRead += 4U;
    gSblMCB.imageParams.sectionParams.secState = SBL_SEC_STATE_HDR_END;

    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      Used to update the globals in end sectionHeader.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Image offset.
 */
static uint32_t SBL_updateVarSectionHeaderStateEnd(void)
{
    uint32_t        offset;

    if (gSblMCB.imageParams.sectionParams.secState == SBL_SEC_STATE_HDR_END)
    {
        gSblMCB.imageParams.state = SBL_RPRC_PARSER_SECTION_DOWNLOAD;

        /* If section length is not 8 byte multiple, make it 8 byte multiple
          as the image has zeros appended to make it 8 byte multiple */
        if ((gSblMCB.imageParams.sectionParams.secLen % 8U) != 0U)
        {
            gSblMCB.imageParams.sectionParams.secLen += (8U - (gSblMCB.imageParams.sectionParams.secLen % 8U));
        }

        offset = SBL_calculateImageOffset((uint32_t)gSblMCB.imageParams.sectionParams.secStrtAddr,
                    gSblMCB.imageParams.sectionParams.secLen);

        gSblMCB.imageParams.sectionParams.sectionPtr = \
              (uint32_t*)((uint32_t)gSblMCB.imageParams.sectionParams.secStrtAddr + (uint32_t)offset);
        gSblMCB.imageParams.sectionParams.writtenLen = 0U;
     }
     else
     {
         offset = 1U;
     }
     return(offset);
}

/*!
 *  @b Description
 *  @n
 *      This function implements the state m/c for the RPRC image section header parser.
 *
 *  @param[in]  bufferPtr
 *      Pointer to the buffer that contains data for parsing.
 *  @param[in] bufferLenIp
 *      Buffer length in bytes.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_readRPRCSecHeader(uint32_t* bufferPtr, uint32_t bufferLenIp)
{
    uint32_t        bufferLen;
    uint32_t        bytesRead = 0U;
    uint32_t*       ptrLcl = bufferPtr;

    /* If the length of the avavilable buffer is >= to the size of the section header, read the
      header in one go. Else, we mark the state of the section header expected in the next
      input buffer */
    if ((bufferLenIp >= SBL_RPRC_SEC_HDR_LENGTH) &&
        (gSblMCB.imageParams.sectionParams.secState == SBL_SEC_STATE_START_ADDR))
    {
        bytesRead = SBL_extractRPRCSecHeader(bufferPtr);
    }
    else
    {
        bufferLen =  bufferLenIp;
        while ((bufferLen - bytesRead) >= 8U)
        {
            switch (gSblMCB.imageParams.sectionParams.secState)
            {
                case SBL_SEC_STATE_START_ADDR:
                {
                    gSblMCB.imageParams.sectionParams.secStrtAddr = *(uint64_t*)ptrLcl;
                    ptrLcl += 2U;
                    bytesRead += 8U;
                    gSblMCB.imageParams.sectionParams.secState = SBL_SEC_STATE_SEC_LEN;
                    break;
                }
                case SBL_SEC_STATE_SEC_LEN:
                {
                    gSblMCB.imageParams.sectionParams.secLen = (uint32_t)*ptrLcl;
                    ptrLcl++;
                    bytesRead += 4U;

                    /* Reading rsvd Sec Header 0 */
                    ptrLcl++;
                    bytesRead += 4U;
                    gSblMCB.imageParams.sectionParams.secState = SBL_SEC_STATE_RSVD;
                    break;
                }
                case SBL_SEC_STATE_RSVD:
                {
                    /* Reading rsvd Sec Header 1 */
                    ptrLcl++;
                    bytesRead += 4U;

                    /* Reading rsvd Sec Header 2 */
                    ptrLcl++;
                    bytesRead += 4U;
                    gSblMCB.imageParams.sectionParams.secState = SBL_SEC_STATE_HDR_END;
                    break;
                }
                case SBL_SEC_STATE_HDR_END:
                {
                    /* Empty */
                    break;
                }
                default:
                {
                    /* Empty */
                    break;
                }
            }
            if (gSblMCB.imageParams.sectionParams.secState == SBL_SEC_STATE_HDR_END)
            {
                /*Break from the while loop as well*/
                break;
            }
        }
    }

    (void)SBL_updateVarSectionHeaderStateEnd();
    if (SBL_CHECK_ERROR_STATUS)
    {
        bytesRead = bufferLenIp;
    }

    gSblMCB.imageParams.imageSizeRead +=  bytesRead;

    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      This function is the initialization function for all the parser variables.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable.
 */
void SBL_parserInitialization(void)
{
    gSblMCB.imageParams.state = SBL_RPRC_PARSER_STATE_IDLE;
    gSblMCB.imageParams.numSections = 0U;
    gSblMCB.imageParams.numSectionsWritten = 0U;
    gSblMCB.imageParams.imageSizeRead = 0U;
    gSblMCB.imageParams.fileSize = 0U;
    gSblMCB.imageParams.fileType = 0U;
    gSblMCB.imageParams.hdrState = SBL_RPRC_HDR_STATE_MAGIC;
    gSblMCB.imageParams.numAlignBytes = 0U;

    gSblMCB.imageParams.sectionParams.secStrtAddr = 0U;
    gSblMCB.imageParams.sectionParams.sectionPtr = NULL;
    gSblMCB.imageParams.sectionParams.secLen = 0U;
    gSblMCB.imageParams.sectionParams.writtenLen = 0U;
    gSblMCB.imageParams.sectionParams.secState = SBL_SEC_STATE_START_ADDR;

    gSblMCB.inputBuffers.bufferPrms[0].buferLen = 0U;
    gSblMCB.inputBuffers.bufferPrms[0].bufferstate = SBL_BUFFER_EMPTY;
    gSblMCB.inputBuffers.bufferPrms[1].buferLen = 0U;
    gSblMCB.inputBuffers.bufferPrms[1].bufferstate = SBL_BUFFER_EMPTY;
    gSblMCB.inputBuffers.writeBufferIdx = 0U;
    gSblMCB.inputBuffers.readBufferIdx  = 0U;

    gSblMCB.numFilesWritten = 0U;

}

/*!
 *  @b Description
 *  @n
 *      This function implements the state m/c for the RPRC image parser.
 *
 *   @param[in]  bufferPtr
 *      Input buffer for parsing
 *   @param[in]  bufferLen
 *      Input buffer data length
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes used.
 *
 */
uint32_t SBL_imageParser(uint32_t* bufferPtr, uint32_t bufferLen)
{
    uint32_t        bytesRead;
    uint32_t        remainingBytes;
    uint32_t        totalBytesUsed = 0U;
    uint32_t*       ptrLcl = bufferPtr;

    remainingBytes = bufferLen;

    do
    {
        /* STATE 1 for RPRC image parsing */
        if (gSblMCB.imageParams.state == SBL_RPRC_PARSER_STATE_IDLE)
        {

            bytesRead = SBL_readRPRCHeader(ptrLcl, remainingBytes);
            remainingBytes -= bytesRead;
            ptrLcl += (bytesRead / 4U);
            totalBytesUsed += bytesRead;
        }

        /* STATE 2 for reading the section headers in the RPRC Image. */
        if (gSblMCB.imageParams.state == SBL_RPRC_PARSER_SECTION_HEADER)
        {

            bytesRead = SBL_readRPRCSecHeader(ptrLcl, remainingBytes);
            remainingBytes -= bytesRead;
            ptrLcl += (bytesRead / 4U);
            totalBytesUsed += bytesRead;
        }

        /* STATE 3 for reading the section content in the RPRC Image
          and loading into the respective memories. */
        if (gSblMCB.imageParams.state == SBL_RPRC_PARSER_SECTION_DOWNLOAD)
        {
            bytesRead = SBL_readRPRCSecContent(ptrLcl, remainingBytes);
            remainingBytes -= bytesRead;
            ptrLcl += (bytesRead / 4U);
            totalBytesUsed += bytesRead;
        }

        /* STATE 4 for reading the alignment bytes at the
          end of the RPRC image before the next RPRC image start. */
        if (gSblMCB.imageParams.state == SBL_RPRC_PARSER_ALIGNMENT_BYTES)
        {
            bytesRead = SBL_readAlignmentBytes(ptrLcl, remainingBytes);
            remainingBytes -= bytesRead;
            ptrLcl += (bytesRead / 4U);
            totalBytesUsed += bytesRead;
        }
    }
    while (remainingBytes != 0U);


    return(totalBytesUsed);
}

