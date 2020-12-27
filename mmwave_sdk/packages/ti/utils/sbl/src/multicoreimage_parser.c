/*
 *  @file multicoreimage_parser.c
 *  @brief Contains the multi core image parser state machine.
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
/* MMWSDK include files */
#include <ti/drivers/osal/DebugP.h>

/* SBL internal include file. */
#include <ti/utils/sbl/include/sbl_internal.h>

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
#pragma DATA_ALIGN(imageChunk, 64);
uint32_t            imageChunk[512U];

/**************************************************************************
 *************************** Function Definitions *************************
 **************************************************************************/
static uint32_t SBL_stateMetaHeaderParser(uint32_t readPtrLcl[]);
static uint32_t SBL_stateRPRCDownloadParse(uint32_t readPtrLcl[], uint32_t bufferLen);


/*!
 *  @b Description
 *  @n
 *      This function parses the Meta Header.
 *
 *  @param[in]  readPtrLcl
 *      Pointer to the input buffer.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 */
static uint32_t SBL_stateMetaHeaderParser(uint32_t readPtrLcl[])
{
     uint32_t       bytesRead;

     bytesRead = SBL_metaHeaderParser(readPtrLcl);
     if (bytesRead == 0U)
     {
         gSblMCB.bufferParserState = SBL_BUFFER_PARSER_COMPLETED;
         SBL_printf("Error: Meta header absent!!!\r\n");
     }
     else
     {
        /* Image is appended with zeros to make the
         * RPRC start 64 byte aligned */
		if ((bytesRead % 64U) != 0U)
		{
			bytesRead += (64U - (bytesRead % 64U));
		}

		/* Update the next parser state to RPRC_DWLD */
		gSblMCB.bufferParserState = SBL_BUFFER_PARSER_RPRC_DWLD;
    }

    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      This function parses the RPRC image.
 *
 *  @param[in]  readPtrLcl
 *      Pointer to the input buffer.
 *  @param[in]  bufferLen
 *      Input buffer length.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Number of bytes read.
 *
 */
static uint32_t SBL_stateRPRCDownloadParse(uint32_t readPtrLcl[], uint32_t bufferLen)
{
    uint32_t        bytesRead;

    /* Confidentiality Check - Decrypt the image chunk */
    (void)SBL_decryptImage((uint8_t *)readPtrLcl, (uint8_t *)imageChunk, bufferLen);

    /* Authentication check - Perform the integrity check of the image */
    (void)SBL_imageValidationCheck(&imageChunk[0], bufferLen);
    if (SBL_CHECK_ERROR_STATUS)
    {
        gSblMCB.bufferParserState = SBL_BUFFER_PARSER_COMPLETED;
        bytesRead = 0U;
        SBL_printf("Error: Authentication check failed!!!\r\n");
        DebugP_assert(0);
    }
    else
    {
        /* Load image - to the destination location */
        bytesRead = SBL_imageParser(&imageChunk[0], bufferLen);


        if (SBL_CHECK_ERROR_STATUS)
        {
            SBL_printf("Error: RPRC Parsing Failure!!!\r\n");
            gSblMCB.bufferParserState = SBL_BUFFER_PARSER_COMPLETED;
            bytesRead = 0U;
            DebugP_assert(0);
        }
        else
        {
            if ((gSblMCB.imageParams.state == SBL_RPRC_PARSER_STATE_IDLE) &&
                ((gSblMCB.numFilesWritten) == gSblMCB.metaHeader.numFiles))
            {
                /* Image load complete */
                gSblMCB.bufferParserState = SBL_BUFFER_PARSER_COMPLETED;
                SBL_printf("Debug: Parsing completed\r\n");
            }
            else
            {
               /* Do nothing */
            }
        }
    }
    return(bytesRead);
}

/*!
 *  @b Description
 *  @n
 *      This function implements the state machine for the Meta Image parsing.
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable.
 */
void SBL_multicoreImageParser(void)
{
    uint32_t*       readPtr;
    uint32_t        bytesRead;
    uint32_t        bufferLen;

    readPtr   = &(gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.readBufferIdx].inputBuffer[0U]);
    bufferLen = (gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.readBufferIdx].buferLen * 4U);
    do
    {
        /* Assumption is that at a time 2K of data is available.
          So, the metaheader parsing and the CERT parsing will happen in one go */
        switch (gSblMCB.bufferParserState)
        {
            case SBL_BUFFER_PARSER_STATE_IDLE:
            {
                gSblMCB.bufferParserState = SBL_BUFFER_PARSER_META_HEADER;
                break;
            }
            case SBL_BUFFER_PARSER_META_HEADER:
            {
                /* MetaHeaderParser */
                bytesRead = SBL_stateMetaHeaderParser(readPtr);

                if (bytesRead == 0U)
                {
                    bufferLen = 0U;
                    break;
                }
                else
                {
                    /* Check if the total size of the image is less that one block size(2K) */
                    if (gSblMCB.metaHeader.imageSize <= bufferLen)
                    {
                        bufferLen = gSblMCB.metaHeader.imageSize;
                        gSblMCB.lessThan2048 = 1U;
                    }
                    bufferLen -= bytesRead;
                    readPtr += (bytesRead / 4U);
                    }
                    break;
            }
            case SBL_BUFFER_PARSER_RPRC_DWLD:
            {
                /* RPRC Image parsing */
                bytesRead = SBL_stateRPRCDownloadParse(readPtr, bufferLen);
                if (bytesRead == 0U)
                {
                    bufferLen = 0U;
                }
                else
                {
                    bufferLen -= bytesRead;
                    readPtr += (bytesRead / 4U);
                }
                break;
            }
            default:
            {
                /* Invalid state log error */
                gSblMCB.bufferParserState = SBL_BUFFER_PARSER_COMPLETED;
                bufferLen = 0U;
                break;
            }
        }
    }while (bufferLen != 0U);

    /* Parsing is completed. Reset the buffer state and mark them empty. */
    SBL_resetBuffers();
}

