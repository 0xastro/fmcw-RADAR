/*
 *   @file buffer_mgmt.h
 *   @brief
 *      Defines the input buffers and buffer states used for input buffer parsing.
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


#ifndef BUFFER_MGMT_H
#define BUFFER_MGMT_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SBL_INTERNAL_DATA_STRUCTURE
 @{ */

#define SBL_INPUT_BUFFER_SIZE       (512U)      /**< Size of the buffers in words. */
#define SBL_NUM_INPUT_BUFFERS       (2U)        /**< Number of input buffers.      */

/*!
 *  @brief  Current buffer states
 */
typedef enum SBL_BufferState_t
{
    SBL_BUFFER_EMPTY = 0,                       /**< Empty available buffer.   */
    SBL_BUFFER_FILLING,                         /**< Buffer busy with data being read into the buffer.   */
    SBL_BUFFER_FULL                             /**< Buffer has data ready to be processed.   */
}SBL_BufferState;

/*!
 *  @brief Structure containing buffer structure parameters.
 */
typedef struct SBL_BufferParams_t
{
    uint32_t            inputBuffer[SBL_INPUT_BUFFER_SIZE];     /**< Input buffer.         */
    uint32_t            buferLen;                               /**< Input buffer length.  */
    SBL_BufferState     bufferstate;                            /**< Input buffer state.   */
}SBL_BufferParams;

/*!
 *  @brief Input buffer structure used for image parsing.
 */
typedef struct SBL_InputBuffer_t
{
    SBL_BufferParams    bufferPrms[SBL_NUM_INPUT_BUFFERS];      /**< Input buffer parameters.   */
    uint32_t            writeBufferIdx;                         /**< Write buffer index.        */
    uint32_t            readBufferIdx;                          /**< Read buffer index.         */
}SBL_InputBuffer;

/**
@}
*/

extern void SBL_bufferMgmt(uint32_t inputBuffer[], uint32_t bufferLen, uint8_t isLastBufferLcl);
extern void SBL_resetBuffers(void);

#ifdef __cplusplus
}
#endif
#endif

