/*
 *  @file image_parser.h
 *  @brief Defines the data structures used during the parsing of all the
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

#ifndef IMAGE_PARSER_H
#define IMAGE_PARSER_H

#include <ti/utils/sbl/include/metaheader_parser.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SBL_INTERNAL_DATA_STRUCTURE
 @{ */

#define SBL_RPRC_HDR_LENGTH             (24U)           /**< RPRC header length in bytes.           */
#define SBL_RPRC_MAGIC_WORD             (0x43525052U)   /**< RPRC header magic word.                */
#define SBL_RPRC_SEC_HDR_LENGTH         (24U)           /**< RPRC section header length in bytes.   */
#define SBL_CONFIG_FILE_SIZE            (2048U)         /**< Config file size max length in bytes.  */

#define SBL_MSS_SUBSYSTEM               (0x3551U)       /**< MSS subsystem identifier.              */
#define SBL_MSS_SUBSYSTEM_DEBUG         (0x355DU)       /**< MSS subsystem identifier for debug.    */
#define SBL_BSS_SUBSYSTEM               (0xB551U)       /**< BSS subsystem identifier.              */
#define SBL_DSS_SUBSYSTEM               (0xD551U)       /**< DSS subsystem identifier.              */
#define SBL_CONFIG_SUBSYSTEM            (0xCF91U)       /**< Config subsystem identifier.           */
#define SBL_SUBSYSTEM_MASK              (0xFFFF0000U)   /**< subsystem identifier mask.             */
#define SBL_SUBSYSTEM_SHIFT             (16U)           /**< subsystem identifier shift.            */

/*!
 *   @brief Section header parser states.
 */
typedef enum SBL_SECTION_STATE_t
{
    SBL_SEC_STATE_START_ADDR = 0,               /**< Section header parser state start address.    */
    SBL_SEC_STATE_SEC_LEN,                      /**< Section header parser state section length.   */
    SBL_SEC_STATE_RSVD,                         /**< Section header parser state reserved.         */
    SBL_SEC_STATE_HDR_END                       /**< Section header parser state header end.       */
}SBL_SECTION_STATE;

/*!
 *   @brief RPRC header parser states.
 */
typedef enum SBL_RPRC_HDR_STATE_t
{
    SBL_RPRC_HDR_STATE_MAGIC = 0,               /**< RPRC header parser state magic number.    */
    SBL_RPRC_HDR_STATE_LEN,                     /**< RPRC header parser state length.          */
    SBL_RPRC_HDR_STATE_RSVD,                    /**< RPRC header parser state reserved.        */
    SBL_RPRC_HDR_STATE_END                      /**< RPRC header parser state header end.      */
}SBL_RPRC_HDR_STATE;

/*!
 *   @brief RPRC Image parser states.
 */
typedef enum SBL_RPRC_PARSER_STATE_t
{
    SBL_RPRC_PARSER_STATE_IDLE = 0,             /**< RPRC parser state idle.                */
    SBL_RPRC_PARSER_SECTION_HEADER,             /**< RPRC parser state section header.      */
    SBL_RPRC_PARSER_SECTION_DOWNLOAD,           /**< RPRC parser state section download.    */
    SBL_RPRC_PARSER_ALIGNMENT_BYTES             /**< RPRC parser state alignment bytes.     */
}SBL_RPRC_PARSER_STATE;

/*!
 *   @brief Multicore image buffer parser states.
 */
typedef enum SBL_BUFFER_PARSER_STATE_t
{
    SBL_BUFFER_PARSER_STATE_IDLE = 0U,          /**< Multicore image buffer parser state idle.          */
    SBL_BUFFER_PARSER_META_HEADER = 1U,         /**< Multicore image buffer parser state meta header.   */
    SBL_BUFFER_PARSER_RPRC_DWLD = 3U,           /**< Multicore image buffer parser state RPRC download. */
    SBL_BUFFER_PARSER_COMPLETED = 4U            /**< Multicore image buffer parser state completed.     */
}SBL_BUFFER_PARSER_STATE;

/*!
 *   @brief Structure describes section parser parameters.
 */
typedef struct SBL_sectionParams_t
{
    uint64_t            secStrtAddr;        /**< section start address.         */
    uint32_t*           sectionPtr;         /**< section current address.       */
    uint32_t            secLen;             /**< section  length.               */
    uint32_t            writtenLen;         /**< Length of the section written. */
    SBL_SECTION_STATE   secState;           /**< section parser state.          */
}SBL_sectionParams;

/*!
 *   @brief Structure describes image parser parameters.
 */
typedef struct SBL_ImagePrms_t
{
    uint32_t                numSections;        /**< Number of Sections .               */
    uint32_t                numSectionsWritten; /**< Number of sections written.        */
    uint32_t                imageSizeRead;      /**< Length of Image read in Bytes.     */
    uint32_t                fileSize;           /**< Size of the image.                 */
    uint32_t                fileType;           /**< image type.                        */
    uint32_t                numAlignBytes;      /**< Number of alignment bytes.         */
    SBL_sectionParams       sectionParams;      /**< section parser parameters.         */
    SBL_RPRC_PARSER_STATE   state;              /**< State of the RPRC parser.          */
    SBL_RPRC_HDR_STATE      hdrState;           /**< State of the RPRC Header parser.   */
}SBL_ImageParams;

/**
@}
*/

extern void SBL_parserInitialization(void);
extern uint32_t SBL_imageParser(uint32_t* bufferPtr, uint32_t bufferLen);
extern void SBL_multicoreImageParser(void);

#ifdef __cplusplus
}
#endif

#endif /* IMAGE_PARSER_H */
