/*
 *  @file metaheader_parser.h
 *  @brief Contains the metaheader definitions.
 *
 *   This file contains:
 *   - parse all the RPRC images received from the host
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


#ifndef METAHEADER_PARSER_H
#define METAHEADER_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SBL_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 *  @brief Maximum number files supported by the parser.
 */
#define SBL_NUM_FILES_SUPPORTED     (5U)

/**
 *  @brief Metaheader start word.
 */
#define SBL_META_HDR_START          (0x5254534DU)

/**
 *  @brief Metaheader end word.
 */
#define SBL_META_HDR_END            (0x444E454DU)

/*!
 *  @brief Device type.
 */
typedef enum SBL_DeviceType_t
{
    SBL_DEVICE_HS,               /**< Device type HS.          */
    SBL_DEVICE_GP                /**< Device type GP.          */
}SBL_DeviceType;

/*!
 *   @brief Structure containing image details.
 */
typedef struct SBL_ImageDetails_t
{
    uint32_t        magicWord;       /**< magic word of image.      */
    uint32_t        fileOffset;      /**< offset of the magic word. */
    uint32_t        fileSize;        /**< Image size.               */
    uint32_t        fileCRCHi;       /**< Image CRC [7-4] bytes.    */
    uint32_t        fileCRCLo;       /**< Image CRC [3-0] bytes.    */
    uint8_t         filePresent;     /**< Image valid flag.         */
    uint8_t         fileType;        /**< Image id.                 */
}SBL_ImageDetails;

/*!
 *   @brief Structure containing all the metaheader details.
 */
typedef struct SBL_MetaHeader_t
{
    uint32_t            numFiles;                               /**< Num of files present.       */
    uint32_t            devId;                                  /**< Device Id.                  */
    uint32_t            hdrCRCHi;                               /**< Metaheader CRC [7-4] bytes. */
    uint32_t            hdrCRCLo;                               /**< Metaheader CRC [3-0] bytes. */
    uint32_t            imageSize;                              /**< Multicore image size.       */
    uint32_t            shMemAlloc;                             /**< shared memory allocation.   */
    SBL_DeviceType      deviceType;                             /**< Device type HS/GP.          */
    SBL_ImageDetails    imageDetails[SBL_NUM_FILES_SUPPORTED];  /**< All image details.          */
}SBL_MetaHeader;

/**
@}
*/

extern uint32_t SBL_metaHeaderParser(uint32_t readPtr[]);

#ifdef __cplusplus
}
#endif

#endif

