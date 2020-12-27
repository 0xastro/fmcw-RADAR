/**
 *   @file  buffer_mgmt.c
 *
 *   @brief
 *      Manages the buffer rotation and buffer status based on the available input buffers.
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
/* SBL internal include file. */
#include <ti/utils/sbl/include/sbl_internal.h>

/**************************************************************************
 *************************** Function Definitions *************************
 **************************************************************************/
/*!
 * @b Description
 *  @n
 *      This is the buffer management function that manages the input buffers.
 *      It invokes the parsing routine when the buffer is full.
 *
 *   @param[in]  inputBuffer
 *       Input data to sent from the interface
 *   @param[in]  bufferLen
 *       Length of the input buffer
 *   @param[in]  isLastBufferLcl
 *       Flag to indicate the the last buffer
 *
 *  \ingroup SBL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 *
 */
void SBL_bufferMgmt(uint32_t inputBuffer[], uint32_t bufferLen,
                uint8_t isLastBufferLcl)
{
    uint32_t        bufferWrIdx;
    uint32_t        loopCnt;
    uint32_t        counter = 65535U;

    bufferWrIdx = gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.writeBufferIdx].buferLen;

    /* Start filling the buffer */
    for (loopCnt = 0U; loopCnt <  (bufferLen / 4U); loopCnt++)
    {
        gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.writeBufferIdx].bufferstate = SBL_BUFFER_FILLING;

        gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.writeBufferIdx].inputBuffer[bufferWrIdx] = *inputBuffer;
        bufferWrIdx++;
        inputBuffer++;
        gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.writeBufferIdx].buferLen = (bufferWrIdx);

        /* Check if buffer is full */
        if (bufferWrIdx >= (SBL_INPUT_BUFFER_SIZE))
        {
            /* Change the buffer state to indicate buffer is full */
            gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.writeBufferIdx].bufferstate = SBL_BUFFER_FULL;
            gSblMCB.inputBuffers.writeBufferIdx++;

            /* Compute the next input buffer write index */
            gSblMCB.inputBuffers.writeBufferIdx = (gSblMCB.inputBuffers.writeBufferIdx % SBL_NUM_INPUT_BUFFERS);

            /* Reset the local buffer write index */
            bufferWrIdx = 0U;
            do
            {
                /* Parsing not done.. Wait here!! */
                counter--;
            }while ((counter > 0U) && \
                   (gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.writeBufferIdx].bufferstate \
                                                                    != SBL_BUFFER_EMPTY));
            /* Call the buffer parser */
            SBL_multicoreImageParser();
        }
    }
    if (isLastBufferLcl == 1U)
    {
        /* Call the buffer parser for the last buffer */
        SBL_multicoreImageParser();
    }
}


void SBL_resetBuffers(void)
{
    /* Mark buffer as empty as the parsing is complete */
    gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.readBufferIdx].bufferstate = SBL_BUFFER_EMPTY;
    gSblMCB.inputBuffers.bufferPrms[gSblMCB.inputBuffers.readBufferIdx].buferLen = 0U;

    gSblMCB.inputBuffers.readBufferIdx++;
    gSblMCB.inputBuffers.readBufferIdx = (gSblMCB.inputBuffers.readBufferIdx % SBL_NUM_INPUT_BUFFERS);
}


