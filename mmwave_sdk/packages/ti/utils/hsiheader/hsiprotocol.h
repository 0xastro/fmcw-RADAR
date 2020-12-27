/**
 *   @file  hsiprotocol.h
 *
 *   @brief
 *      The header file exposes the HSI header and associated definitions.
 *      HSI headers are created and attached to an LVDS stream which is
 *      being transmitted. The headers can be decoded on the peer where
 *      the stream is received to get access to the actual data.
 *
 *      This file can be used independently and does not have any
 *      dependencies on the mmWave SDK.
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
#ifndef HSI_PROTOCOL_H
#define HSI_PROTOCOL_H

/** @addtogroup HSI_HEADER_PROTOCOL
 @{ */

/**********************************************************************************
 * Standard Endianess Macros:
 * - The HSI Header Protocol works natively over Little Endian. Use the following
 *   macros to ensure portability.
 **********************************************************************************/
#if defined(_LITTLE_ENDIAN)

/**
 * @brief   Macro which converts 16 bit data from host to network order.
 */
#define  HSIHeader_htons(a)     (a)

/**
 * @brief   Macro which converts 32 bit data from host to network order.
 */
#define  HSIHeader_htonl(a)     (a)

/**
 * @brief   Macro which converts 64 bit data from host to network order.
 */
#define  HSIHeader_htonll(a)    (a)

/**
 * @brief   Macro which converts 32 bit data from network to host order.
 */
#define  HSIHeader_ntohl(a)     (a)

/**
 * @brief   Macro which converts 16 bit data from network to host order.
 */
#define  HSIHeader_ntohs(a)     (a)

/**
 * @brief   Macro which converts 64 bit data from network to host order.
 */
#define  HSIHeader_ntohll(a)    (a)

#elif defined(_BIG_ENDIAN)

/**
 * @brief   Macro which converts 16 bit data from host to network order.
 */
#define  HSIHeader_htons(a)    ((((a)>>8)&0x00FF) + (((a)<<8)&0xFF00))

/**
 * @brief   Macro which converts 32 bit data from host to network order.
 */
#define  HSIHeader_htonl(a)   ((((a)>>24)& 0x000000FF) +  (((a)>>8) & 0x0000FF00) +     \
                               (((a)<<8) & 0x00FF0000)  + (((a)<<24)& 0xFF000000))

/**
 * @brief   Macro which converts 64 bit data from host to network order.
 */
#define  HSIHeader_htonll(a)   ((((a)>>56)& 0x00000000000000FF) + (((a)>>40)& 0x000000000000FF00) +    \
                                (((a)>>24)& 0x0000000000FF0000) + (((a)>>8) & 0x00000000FF000000) +    \
                                (((a)<<8) & 0x000000FF00000000) + (((a)<<24)& 0x0000FF0000000000) +    \
                                (((a)<<40)& 0x00FF000000000000) + (((a)<<56)& 0xFF00000000000000))

/**
 * @brief   Macro which converts 64 bit data from network to host order.
 */
#define  HSIHeader_ntohll(a)   HSIHeader_htonll(a)

/**
 * @brief   Macro which converts 32 bit data from network to host order.
 */
#define  HSIHeader_ntohl(a)   HSIHeader_htonl(a)

/**
 * @brief   Macro which converts 16 bit data from network to host order.
 */
#define  HSIHeader_ntohs(a)   HSIHeader_htons(a)

#else
#error "Please define the Endianess (_BIG_ENDIAN) or (_LITTLE_ENDIAN)"
#endif

/**
 * @brief
 *  HSI Header Identifier1: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID1              0x0CDA0ADC0CDA0ADCU


/**
 * @brief
 *  HSI Header Identifier2: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID2              0x09CC0CC909CC0CC9U

/**
 * @brief
 *  HSI Header Identifier3: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID3              0x08CC0CC808CC0CC8U

/**
 * @brief
 *  HSI Header Identifier4: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID4              0x0f48084f0f48084fU

/**
 * @brief
 *  HSI Header Identifier5: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID5              0x076C0C67076C0C67U

/**
 * @brief
 *  HSI Header Identifier6: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID6              0x0DDA0ADD0DDA0ADDU

/**
 * @brief
 *  HSI Header Identifier7: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID7              0x0EDA0ADE0EDA0ADEU

/**
 * @brief
 *  HSI Header Identifier8: These are unique identifiers which
 *  are attached at the beginning of each header.
 *
 * @sa HSIHeader::id
 */
#define HSI_HEADER_ID8              0x0FDA0ADF0FDA0ADFU

/**
 * @brief
 *  Platform is XWR14xx
 *
 * @sa HSIHeader::platform
 */
#define HSI_PLATFORM_XWR14XX                    0x1

/**
 * @brief
 *  Platform is XWR16xx
 *
 * @sa HSIHeader::platform
 */
#define HSI_PLATFORM_XWR16XX                    0x2

/**
 * @brief
 *  Platform is XWR18xx
 *

 * @sa HSIHeader::platform
 */
#define HSI_PLATFORM_XWR18XX                    0x3

/**
 * @brief
 *  Platform is XWR68xx
 *

 * @sa HSIHeader::platform
 */
#define HSI_PLATFORM_XWR68XX                    0x4

/**
 * @brief
 *  ADC Data is interleaved
 *
 * @sa HSIHeader::interleavedMode
 */
#define HSI_HEADER_MODE_INTERLEAVED             0x1

/**
 * @brief
 *  Data is non-interleaved. This is applicable for ADC while
 *  operating in non-interleaved mode and for all user data.
 *
 * @sa HSIHeader::interleavedMode
 */
#define HSI_HEADER_MODE_NON_INTERLEAVED         0x2

/**
 * @brief
 *  Size of the data (16bit) being transferred streamed over the LVDS
 *
 * @sa HSIHeader::dataSize
 */
#define HSI_HEADER_SIZE_16BIT                   0x0

/**
 * @brief
 *  Size of the data (14bit) being transferred streamed over the LVDS
 *
 * @sa HSIHeader::dataSize
 */
#define HSI_HEADER_SIZE_14BIT                   0x1

/**
 * @brief
 *  Size of the data (12bit) being transferred streamed over the LVDS
 *
 * @sa HSIHeader::dataSize
 */
#define HSI_HEADER_SIZE_12BIT                   0x2

/**
 * @brief
 *  Real data being streamed out
 *
 * @sa HSIHeader::dataType
 */
#define HSI_HEADER_TYPE_REAL                    0x1

/**
 * @brief
 *  Complex data being streamed out
 *
 * @sa HSIHeader::dataType
 */
#define HSI_HEADER_TYPE_COMPLEX                 0x2

/**
 * @brief
 *  Data Format: Only ADC data is being streamed over the LVDS
 *
 * @sa HSIHeader::dataFmt
 */
#define HSI_HEADER_FMT_ADC                      0x1

/**
 * @brief
 *  Data Format: Chirp Profile + ADC data is being streamed over the LVDS
 *
 * @sa HSIHeader::dataFmt
 */
#define HSI_HEADER_FMT_CP_ADC                   0x2

/**
 * @brief
 *  Data Format: ADC data + Chirp Profile is being streamed over the LVDS
 *
 * @sa HSIHeader::dataFmt
 */
#define HSI_HEADER_FMT_ADC_CP                   0x3

/**
 * @brief
 *  Data Format: Chirp Profile + ADC data + Chirp Quality is being streamed over the LVDS
 *
 * @sa HSIHeader::dataFmt
 */
#define HSI_HEADER_FMT_CP_ADC_CQ                0x4

/**
 * @brief
 *  Data Format: User Data is being streamed over the LVDS
 *
 * @sa HSIHeader::dataFmt
 */
#define HSI_HEADER_FMT_USER                     0x5

/**
 * @brief
 *  Data Format: ADC + User Data is being streamed over the LVDS
 *
 * @sa HSIHeader::dataFmt
 */
#define HSI_HEADER_FMT_ADC_USER                 0x6

/**
 * @brief
 *  Data Format: Chirp Profile + ADC data + Chirp Quality + User Data is being streamed over the LVDS
 *
 * @sa HSIHeader::dataFmt
 */
#define HSI_HEADER_FMT_CP_ADC_CQ_USER           0x7

/**
 * @brief
 *  This is the maximum number of user buffers which can be supported in the HSI Header. This
 *  should be the same as the MAX number of user buffers supported in the CBUFF Driver. However
 *  we cannot use the same macro here because it will make the file dependent on the mmWave
 *  SDK. So instead we redefine the macro and verify the sanity during the
 *  HSI Header initialization
 */
#define HSI_HEADER_MAX_USER_BUFFER              3U

/**
 * @brief
 *  This is the maximum number of chirp quality which can be supported in the HSI Header. This
 *  should be the same as the definition supported in the ADCBUF Driver. However we cannot
 *  use the same macro here because it will make the file dependent on the mmWave SDK.
 *  So instead we redefine the macro and verify the sanity during the HSI Header
 *  initialization
 */
#define HSI_HEADER_MAX_CQ                       3U

/**
 * @brief
 *  The DCA1000EVM FPGA needs a minimum delay of 12ms between Bit clock starts and
 *  actual LVDS Data start to lock the LVDS PLL IP. This is documented in the DCA UG
 */
#define HSI_DCA_MIN_DELAY_MSEC                  12U

/**
 * @brief
 *  The HSI Header Size is the maximum size of the HSI Header. The DCA1000EVM FPGA
 *  requires the data to be aligned on 256 bytes. While creating a header applications
 *  can specify the alignment requirements.
 */
#define HSI_DATA_CARD_ALIGNMENT                 256U

/**
 * @brief
 *  The HSI header allows an application to add an application defined extension
 *  header. This is the maximum size of the header.
 */
#define HSI_HEADER_MAX_APP_HDR_SIZE             8U

/**
 * @brief
 *  HSI Data Card Header
 *
 * @details
 *  The HSI Data Card Header is used by the TI Data card while configured to
 *  operate in Multi-Mode
 */
typedef struct HSIDataCardHeader_t
{
    /**
     * @brief   Unique HSI Identifier associated with the header.
     */
    uint64_t                id;

    /**
     * @brief   Total length (in BYTES) of all the data which is being streamed.
     * This is the lower 12 bits.
     */
    uint16_t                totalLengthLSW;

    /**
     * @brief   Total length (in BYTES) of all the data which is being streamed.
     * This is the higher 12 bits. Application developers can use the helper
     * function to get the total length of the packet. The size excludes the
     * data card header.
     *
     * @sa HSIHeader_getTotalLength
     */
    uint16_t                totalLengthMSW;

    /**
     * @brief   Reserved:
     */
    uint32_t                reserved;
}HSIDataCardHeader;

/**
 * @brief
 *  HSI SDK Header
 *
 * @details
 *  The HSI SDK Header is a header which provides meta-information to the
 *  receiver of the HSI stream. This information can be used to parse the
 *  stream and access the data
 */
typedef struct HSISDKHeader_t
{
    /**
     * @brief   Version of the mmWave SDK associated with the HSI Header. Refer to
     * the following file mmwave_sdk_version.h for more information. The field is
     * encoded as follows:-
     *
     * @code
        11      9       6         3       0
        -----------------------------------
        | Major | Minor | Bug Fix | Build |
        -----------------------------------
       @endcode
     *
     * Application developers can use the helper function to extract this information
     * @sa HSIHeader_getVersion
     */
    uint16_t                 version;

    /**
     * @brief   In order to ensure that the entire data is streamed out there are
     * certain size alignments which need to be enforced. This is explained in more
     * detail in the TRM. The size of the header is thus not *fixed* and could be
     * padded to ensure alignment. The size is specified in CBUFF Units.
     *
     * In order to get the padding size refer to the following helper function:-
     *  @sa HSIHeader_getPaddingSize
     *
     * Application developers can use the helper function to skip the header to get to
     * the payload
     * @sa HSIHeader_getPayload
     */
    uint16_t                headerSize;

    /**
     * @brief   Platform on which the HSI Header was generated
     *
     *  @sa
     *      HSI_PLATFORM_XWR14XX
     *  @sa
     *      HSI_PLATFORM_XWR16XX
     *  @sa
     *      HSI_PLATFORM_XWR18XX
     *  @sa
     *      HSI_PLATFORM_XWR68XX
     */
    uint8_t                 platform;

    /**
     * @brief   Mode in which the data is being transferred.
     * - ADC Data can be Interleaved or Non-Interleaved
     * - User data is always Non-interleaved
     *
     * @sa
     *      HSI_HEADER_MODE_INTERLEAVED
     * @sa
     *      HSI_HEADER_MODE_NON_INTERLEAVED
     */
    uint8_t                 interleavedMode;

    /**
     * @brief   This is the number of bits being transferred
     *  @sa
     *      HSI_HEADER_SIZE_16BIT
     *  @sa
     *      HSI_HEADER_SIZE_14BIT
     *  @sa
     *      HSI_HEADER_SIZE_12BIT
     */
    uint8_t                 dataSize;

    /**
     * @brief   This is the type of data being streamed
     *  @sa
     *      HSI_HEADER_TYPE_REAL
     *  @sa
     *      HSI_HEADER_TYPE_COMPLEX
     */
    uint8_t                 dataType;

    /**
     * @brief   Bit Flag which will indicate the receive channel status on
     * whether it is enabled or disabled
     *  Bit Number | Channel Number
     *  -----------|---------------
     *   Bit 0     |  Channel 0
     *   Bit 1     |  Channel 1
     *   Bit 2     |  Channel 2
     *   Bit 3     |  Channel 3
     */
    uint8_t                 rxChannelStatus;

    /**
     * @brief   This is the format of the data which is being streamed out.
     */
    uint8_t                 dataFmt;

    /**
     * @brief   This is used to indicate the number of chirps. Set to 1 for
     * single chirp mode and between 2 to 8 for multiple chirp mode.
     *
     * *NOTE*: Multiple Chirp Mode is applicable only for XWR16xx/XWR18xx/XWR68xx
     */
    uint16_t                 chirpMode;

    /**
     * @brief   Size in CBUFF Units of the ADC Data for each channel
     */
    uint16_t                adcDataSize;

    /**
     * @brief   Size in CBUFF Units of the Chirp Profile for each channel
     */
    uint16_t                cpDataSize;

    /**
     * @brief   Size in CBUFF Units of the Chirp Quality (CQx). If the
     * size is set to 0 then there is no CQx data present.
     */
    uint16_t                cqDataSize[HSI_HEADER_MAX_CQ];

    /**
     * @brief   Size in CBUFF Units of the User Buffer
     */
    uint16_t                userBufSize[HSI_HEADER_MAX_USER_BUFFER];

    /**
     * @brief   Application defined extension header: This is available to the
     * application to add an application specific *static* header. Once the
     * headers are configured and the sessions are executing this data should not
     * be changed.
     */
    uint8_t                 appExtensionHeader[HSI_HEADER_MAX_APP_HDR_SIZE];
}HSISDKHeader;

/**
 * @brief
 *  HSI Header
 *
 * @details
 *  This is the header which is created by the HSI Module. The header
 *  should be configured as a part of the CBUFF session. The header allows
 *  the ability to specify meta-information which can be used on the
 *  remote end to help decode the received data.
 *
 *  *NOTE*: The data which gets streamed out could be 12bit/14bit/16bit
 *  But in order to make the header generic each field is defined to be
 *  at most 12bits
 */
typedef struct HSIHeader_t
{
    /**
     * @brief   Data Card Header: The data card header is required and is
     * interpreted the TI Data Card in Multi-Mode
     */
    HSIDataCardHeader       dataCardHeader;

    /**
     * @brief   SDK Header:
     */
    HSISDKHeader            sdkHeader;

    /**
     * @brief   Padding Buffer:
     */
    uint8_t                 paddingBuffer[HSI_DATA_CARD_ALIGNMENT -
                                          sizeof(HSIDataCardHeader) -
                                          sizeof(HSISDKHeader)];
}HSIHeader;

/***********************************************************************************
 * Exported Protocol Inline Functions:
 ***********************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is an exported function which is provided to convert
 *      CBUFF Units into bytes
 *
 *  @param[in]  cbuffUnits
 *      CBUFF Units to convert
 *
 *  \ingroup HSI_HEADER_PROTOCOL
 *
 *  @retval
 *      Number of bytes
 */
static inline uint16_t HSIHeader_toBytes (uint16_t cbuffUnits)
{
    return (cbuffUnits << 1U);
}

/**
 *  @b Description
 *  @n
 *      This is an exported function which is provided to convert
 *      bytes into CBUFF Units
 *
 *  @param[in]  bytes
 *      Bytes to convert
 *
 *  \ingroup HSI_HEADER_PROTOCOL
 *
 *  @retval
 *      Number of CBUFF Units
 */
static inline uint16_t HSIHeader_toCBUFFUnits (uint16_t bytes)
{
    return (bytes >> 1U);
}

/**
 *  @b Description
 *  @n
 *      This is an exported function which is used to skip the HSI header
 *      and return a pointer to the actual payload.
 *
 *  @param[in]  ptrHeader
 *      Pointer to the Header
 *
 *  \ingroup HSI_HEADER_PROTOCOL
 *
 *  @retval
 *      Pointer to the data stream
 */
static inline uint8_t* HSIHeader_getPayload (HSIHeader* ptrHeader)
{
    /* Convert the header size into bytes. Skip the header to get the payload */
    return (uint8_t*)ptrHeader + HSIHeader_toBytes(ptrHeader->sdkHeader.headerSize);
}

/**
 *  @b Description
 *  @n
 *      This is an exported function which is used to get the size of the HSI Header
 *      The size of the HSI Header is typically passed to the CBUFF Sessions and
 *      is the amount of data actually streamed out. The header size will include
 *      any padding
 *
 *  @param[in]  ptrHeader
 *      Pointer to the Header
 *
 *  \ingroup HSI_HEADER_PROTOCOL
 *
 *  @retval
 *      Size of the HSI Header to be streamed out in CBUFF Units
 */
static inline uint16_t HSIHeader_getHeaderSize (HSIHeader* ptrHeader)
{
    return ptrHeader->sdkHeader.headerSize;
}

/**
 *  @b Description
 *  @n
 *      This is an exported function which is used to get the padding size
 *
 *  @param[in]  ptrHeader
 *      Pointer to the Header
 *
 *  \ingroup HSI_HEADER_PROTOCOL
 *
 *  @retval
 *      Padding Size in CBUFF Units
 */
static inline uint16_t HSIHeader_getPaddingSize (HSIHeader* ptrHeader)
{
    uint16_t    minHeaderSize;

    /* Mandatory Size of the Header: Size of the header without any padding. */
    minHeaderSize = sizeof (HSIDataCardHeader) + sizeof(HSISDKHeader);
    minHeaderSize = HSIHeader_toCBUFFUnits (minHeaderSize);

    /* What ever is left after the mandatory header is the padding */
    return (ptrHeader->sdkHeader.headerSize - minHeaderSize);
}

/**
 *  @b Description
 *  @n
 *      This is an exported function which is used to extract the version
 *      information from the HSI header
 *
 *  @param[in]  ptrHeader
 *      Pointer to the Header
 *  @param[out] major
 *      mmWave SDK Major Version Number
 *  @param[out] minor
 *      mmWave SDK Minor Version Number
 *  @param[out] bugFix
 *      mmWave SDK Bug Fix Number
 *  @param[out] build
 *      mmWave SDK Build Number
 *
 *  \ingroup HSI_HEADER_PROTOCOL
 *
 *  @retval
 *      Not applicable
 */
static inline void HSIHeader_getVersion
(
    HSIHeader*  ptrHeader,
    uint8_t*    major,
    uint8_t*    minor,
    uint8_t*    bugFix,
    uint8_t*    build
)
{
    uint16_t    version = HSIHeader_htons (ptrHeader->sdkHeader.version);

    *major  = (version & 0xE00) >> 9;
    *minor  = (version & 0x1C0) >> 6;
    *bugFix = (version & 0x031) >> 3;
    *build  = (version & 0x7);
}

/**
 *  @b Description
 *  @n
 *      This is an exported function which is provided to get the
 *      total length of the entire stream. This is specified in
 *      bytes and includes the HSI header & actual data
 *
 *  @param[in]  ptrHeader
 *      Pointer to the Header
 *
 *  \ingroup HSI_HEADER_PROTOCOL
 *
 *  @retval
 *      Total Length of the packet in bytes
 */
static inline uint32_t HSIHeader_getTotalLength (HSIHeader* ptrHeader)
{
    uint32_t    totalLength;

    totalLength = sizeof(HSIDataCardHeader);
    totalLength += (((HSIHeader_htons(ptrHeader->dataCardHeader.totalLengthMSW) & 0xFFF) << 12U) |
                     (HSIHeader_htons(ptrHeader->dataCardHeader.totalLengthLSW) & 0xFFF));
    return totalLength;
}

/**
@}
*/

#endif /* HSI_PROTOCOL_H */

