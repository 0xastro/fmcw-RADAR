/*
*   @file  data_card.cpp
*
*   @brief
*      The file showcases the usage of the HSI Protocol exported API
*      and how to parse the received stream.
*
*  \par
*  NOTE:
*      (C) Copyright 2018 Texas Instruments, Inc.
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Data Card Protocol Header: */
#include <ti/utils/hsiheader/hsiprotocol.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/**
* @brief   CBUFF Unit Display size on the console for the data
*/
#define DATA_CARD_MAX_DISPLAY_SIZE       8

/**************************************************************************
 ******************************* Globals **********************************
 **************************************************************************/

/**
 * @brief   Global variable which tracks the size of the file which has been
 * processed
*/
uint32_t gFileSizeRead = 0;

/**************************************************************************
******************************* DataCard *********************************
**************************************************************************/

/**
*  @b Description
*  @n
*      The function is used to print the usage
*
*  @retval
*      Not applicable
*/
static void DataCard_printUsage(void)
{
    printf("data_card.exe <Serialized Input File>");
    return;
}

/**
*  @b Description
*  @n
*      Utility function which converts the interleaved mode to
*      a string to be displayed for debug purposes.
*
*  @param[in]  interleavedMode
*      Interleaved Mode
*
*  @retval
*      String
*/
static const char* DataCard_toInterleavedModeString(uint8_t interleavedMode)
{
    switch (interleavedMode)
    {
    case HSI_HEADER_MODE_INTERLEAVED:
    {
        return "Interleaved Mode";
    }
    case HSI_HEADER_MODE_NON_INTERLEAVED:
    {
        return "Non-Interleaved Mode";
    }
    default:
    {
        assert(0);
        return "Bad INTERLEAVED MODE";
    }
    }
}

/**
*  @b Description
*  @n
*      Utility function which converts the interleaved mode to
*      a string to be displayed for debug purposes.
*
*  @param[in]  interleavedMode
*      Interleaved Mode
*
*  @retval
*      String
*/
static const char* DataCard_toChirpModeString(uint16_t chirpMode)
{
    switch (chirpMode)
    {
    case 0:
    {
        return "Continuous Mode";
    }
    case 1:
    {
        return "Single Chirp Mode";
    }
    case 2:
    {
        return "Multiple Chirp Mode (2)";
    }
    case 3:
    {
        return "Multiple Chirp Mode (3)";
    }
    case 4:
    {
        return "Multiple Chirp Mode (4)";
    }
    case 5:
    {
        return "Multiple Chirp Mode (5)";
    }
    case 6:
    {
        return "Multiple Chirp Mode (6)";
    }
    case 7:
    {
        return "Multiple Chirp Mode (7)";
    }
    case 8:
    {
        return "Multiple Chirp Mode (8)";
    }
    default:
    {
        /* Out of range: */
        assert(0);
        return "Error: Invalid Chirp Mode";
    }
    }
}

/**
*  @b Description
*  @n
*      Utility function which converts the platform to a string
*
*  @param[in]  interleavedMode
*      Interleaved Mode
*
*  @retval
*      String
*/
static const char* DataCard_toPlatformString(uint8_t platform)
{
    switch (platform)
    {
    case HSI_PLATFORM_XWR14XX:
    {
        return "XWR14xx";
    }
    case HSI_PLATFORM_XWR16XX:
    {
        return "XWR16xx";
    }
    case HSI_PLATFORM_XWR18XX:
    {
        return "XWR18xx";
    }
    case HSI_PLATFORM_XWR68XX:
    {
        return "XWR68xx";
    }
    default:
    {
        assert(0);
        return "Bad PLATFORM";
    }
    }
}

/**
*  @b Description
*  @n
*      Utility function which converts the version to a string
*
*  @param[in]   version
*      version
*  @param[out]  versionString
*      Version String
*  @param[in]   versionStringLen
*      Length of the Version String
*
*  @retval
*      Not applicable
*/
static void DataCard_toVersionString(HSIHeader* ptrHeader, char* versionString, size_t versionStringLen)
{
    uint8_t major;
    uint8_t minor;
    uint8_t bugFix;
    uint8_t build;

    /* Extract the version from the header: */
    HSIHeader_getVersion(ptrHeader, &major, &minor, &bugFix, &build);

    /* Create the version string: */
    snprintf(versionString, versionStringLen, "%d.%d.%d.%d", major, minor, bugFix, build);
    return;
}

/**
*  @b Description
*  @n
*      Utility function which converts the interleaved mode to
*      a string to be displayed for debug purposes.
*
*  @param[in]  dataSize
*      Data Size
*
*  @retval
*      String
*/
static const char* DataCard_toDataSizeString(uint8_t dataSize)
{
    switch (dataSize)
    {
    case HSI_HEADER_SIZE_16BIT:
    {
        return "16bit";
    }
    case HSI_HEADER_SIZE_14BIT:
    {
        return "14bit";
    }
    case HSI_HEADER_SIZE_12BIT:
    {
        return "12bit";
    }
    default:
    {
        assert(0);
        return "BAD Data Size";
    }
    }
}

/**
*  @b Description
*  @n
*      Utility function which converts the identifier to
*      a string to be displayed for debug purposes.
*
*  @param[in]  id
*      Identifier to be displayed
*
*  @retval
*      Identifier string
*/
static const char* DataCard_toIdString(uint64_t id)
{
    switch (id)
    {
    case HSI_HEADER_ID1:
    {
        return "Data ID1";
    }
    case HSI_HEADER_ID2:
    {
        return "Data ID2";
    }
    case HSI_HEADER_ID3:
    {
        return "Data ID3";
    }
    case HSI_HEADER_ID4:
    {
        return "Data ID4";
    }
    case HSI_HEADER_ID5:
    {
        return "Data ID5";
    }
    case HSI_HEADER_ID6:
    {
        return "Data ID6";
    }
    case HSI_HEADER_ID7:
    {
        return "Data ID7";
    }
    case HSI_HEADER_ID8:
    {
        return "Data ID8";
    }
    default:
    {
        /* Error: This should never occur */
        assert(0);
        return "Bad IDENTIFIER";
    }
    }
}

/**
*  @b Description
*  @n
*      Utility function which converts the channel status to
*      a string to be displayed for debug purposes.
*
*  @param[in]  channelStatus
*      Channel Status
*  @param[out]  statusString
*      Populated with the status string
*  @param[in]  statusStringLen
*      Length of the status string
*
*  @retval
*      Identifier string
*/
static void DataCard_toChannelStatusString(uint8_t channelStatus, char* statusString, size_t statusStringLen)
{
    char    channelNumber[5];
    uint8_t numChannels = 0;

    if (channelStatus == 0)
    {
        strncpy(statusString, "No Channel Enabled", statusStringLen);
        return;
    }

    /* Initialize the status string: */
    memset ((void*)statusString, 0, statusStringLen);
    memset((void*)&channelNumber[0], 0, sizeof(channelNumber));

    if (channelStatus & 0x1)
    {
        channelNumber[numChannels] = '0';
        numChannels++;
    }
    if (channelStatus & 0x2)
    {
        channelNumber[numChannels] = '1';
        numChannels++;
    }
    if (channelStatus & 0x4)
    {
        channelNumber[numChannels] = '2';
        numChannels++;
    }
    if (channelStatus & 0x8)
    {
        channelNumber[numChannels] = '3';
        numChannels++;
    }

    /* Create the channel status string: */
    snprintf(statusString, statusStringLen, "Channel-%s", channelNumber);
    return;
}

/**
*  @b Description
*  @n
*      Utility function which converts the identifier to
*      a string to be displayed for debug purposes.
*
*  @param[in]  id
*      Identifier to be displayed
*
*  @retval
*      Identifier string
*/
static const char* DataCard_toDataFmtString(uint8_t dataFmt)
{
    switch (dataFmt)
    {
    case HSI_HEADER_FMT_ADC:
    {
        return "ADC";
    }
    case HSI_HEADER_FMT_CP_ADC:
    {
        return "CP + ADC";
    }
    case HSI_HEADER_FMT_ADC_CP:
    {
        return "ADC + CP";
    }
    case HSI_HEADER_FMT_CP_ADC_CQ:
    {
        return "CP + ADC + CQ";
    }
    case HSI_HEADER_FMT_USER:
    {
        return "User";
    }
    case HSI_HEADER_FMT_ADC_USER:
    {
        return "ADC + User";
    }
    case HSI_HEADER_FMT_CP_ADC_CQ_USER:
    {
        return "CP + ADC + CQ + User";
    }
    default:
    {
        /* Error: This should never occur */
        assert(0);
        return "Bad Data Format Identifier";
    }
    }
}

/**
*  @b Description
*  @n
*      The function is used to read a CBUFF Unit from the file
*      The file is a text based file and so the function will
*      convert the numbers from text to 16-bit signed integers
*      which is a CBUFF Unit
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  dataSize
*      Data Size i.e. the number of bits for which the data is valid
*  @param[out] cbuff
*      CBUFF Unit Read from the file
*
*  @retval
*      Success -   0
*  @retval
*      Error   -   <0
*/
static int32_t DataCard_read16bit(FILE* fp, uint8_t dataSize, uint16_t* cbuff)
{
    uint8_t     index = 0;
    int16_t     readData;
    size_t      retVal;

    /* Initialize the cbuff unit: */
    *cbuff = 0;

    /* Read out the 16 bits of data: */
    retVal = fread(&readData, sizeof(uint16_t), 1, fp);
    if (retVal != 1U)
    {
        /* Reached the end of the file: Close the file: */
        fclose(fp);
        exit (0);
    }

    /* Track the total size of the file read: */
    gFileSizeRead = gFileSizeRead + sizeof(uint16_t);

    /* Honor the data size: */
    switch (dataSize)
    {
    case HSI_HEADER_SIZE_12BIT:
    {
        readData = readData & 0xFFFU;
        break;
    }
    case HSI_HEADER_SIZE_14BIT:
    {
        readData = readData & 0x3FFFU;
        break;
    }
    case HSI_HEADER_SIZE_16BIT:
    {
        readData = readData & 0xFFFFU;
        break;
    }
    default:
    {
        /* Error: Invalid argument */
        assert(0);
        break;
    }
    }

    /* Convert to host order */
    *cbuff = HSIHeader_ntohs(readData);
    return 0;
}

/**
*  @b Description
*  @n
*      The function is used to read the identifier
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[out] id
*      Data Card Identifier
*
*  @retval
*      Success -   0
*  @retval
*      Error   -   <0
*/
static int32_t DataCard_readID(FILE* fp, uint64_t* id)
{
    uint16_t    word[4];

    /* There should be no error detected while reading the identifiers */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &word[0]);
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &word[1]);
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &word[2]);
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &word[3]);

    /* Combine into a single identifier:
    * Ensure that the identifier is converted to host format: */
    *id = ((uint64_t)word[3] << 48) |
          ((uint64_t)word[2] << 32) |
          ((uint64_t)word[1] << 16) |
          ((uint64_t)word[0]);
    *id = HSIHeader_ntohll(*id);

    /* Sanity Checking: We will ensure that this matches the protocol specification */
    switch (*id)
    {
    case HSI_HEADER_ID1:
    case HSI_HEADER_ID2:
    case HSI_HEADER_ID3:
    case HSI_HEADER_ID4:
    case HSI_HEADER_ID5:
    case HSI_HEADER_ID6:
    case HSI_HEADER_ID7:
    case HSI_HEADER_ID8:
    {
        /* This is a valid identifier: */
        break;
    }
    default:
    {
        /* Error: This should never occur */
        assert(0);
        return -1;
    }
    }
    return 0;
}

/**
*  @b Description
*  @n
*      The function is used to skip num CBUFF units
*
*  @param[in]  prefixString
*      Prefix string to be displayed
*  @param[in] fp
*      Pointer to the file from where the data is read
*  @param[in] numCBUFF
*      Number of CBUFF Units to be displayed
*
*  @retval
*      Not applicable
*/
static void DataCard_skip(FILE* fp, HSIHeader* ptrHeader, uint16_t numCBUFF)
{
    uint16_t    tmp;

    /* Read out the data: */
    while (numCBUFF > 0)
    {
        /* Read out the data: */
        DataCard_read16bit(fp, ptrHeader->sdkHeader.dataSize, &tmp);
        numCBUFF = numCBUFF - 1;
    }
}

/**
*  @b Description
*  @n
*      The function is used to read the Data Card Header
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[out] ptrHeader
*      Pointer to the header which is populated
*
*  @retval
*      Success -   0
*  @retval
*      Error   -   <0
*/
static int32_t DataCard_readHeader(FILE* fp, HSIHeader* ptrHeader)
{
    uint16_t    tmp;
    uint16_t    tmp1;
    uint8_t     index;
    uint16_t    paddingSize;
    uint16_t    bytesRead = 0;

    /* Initialize the contents of the header: */
    memset((void*)ptrHeader, 0, sizeof(HSIHeader));

    /* Read the identifier: */
    DataCard_readID(fp, &ptrHeader->dataCardHeader.id);

    /* Read the total length: */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->dataCardHeader.totalLengthLSW);
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->dataCardHeader.totalLengthMSW);

    /* Read the reserved fields: */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &tmp);
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &tmp1);
    ptrHeader->dataCardHeader.reserved = (tmp1 << 16) | tmp;

    /* Read the version: */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->sdkHeader.version);

    /* Read the header size: */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->sdkHeader.headerSize);

    /* Read out the Header:
    * - Platform
    * - Interleaved/Non-Interleaved Mode */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &tmp);
    ptrHeader->sdkHeader.platform        = (tmp & 0x00FF);
    ptrHeader->sdkHeader.interleavedMode = (tmp & 0xFF00) >> 8;

    /* Read out the Header:
    * - Data Size
    * - Data Type */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &tmp);
    tmp = HSIHeader_ntohs(tmp);
    ptrHeader->sdkHeader.dataSize = (tmp & 0x00FF);
    ptrHeader->sdkHeader.dataType = (tmp & 0xFF00) >> 8;

    /* Read out the Header:
    * - Rx Channel Status
    * - Data Format */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &tmp);
    ptrHeader->sdkHeader.rxChannelStatus = (tmp & 0x00FF);
    ptrHeader->sdkHeader.dataFmt         = (tmp & 0xFF00) >> 8;

    /* Read out the Header:
    * - Number of Chirps */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->sdkHeader.chirpMode);

    /* Read out the ADC & CP Size Informations: */
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->sdkHeader.adcDataSize);
    DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->sdkHeader.cpDataSize);

    /* Read out all the CQ Size Information: */
    for (index = 0; index < HSI_HEADER_MAX_CQ; index++)
    {
        DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->sdkHeader.cqDataSize[index]);
    }

    /* Read out the User Buffer Size Information: */
    for (index = 0; index < HSI_HEADER_MAX_USER_BUFFER; index++)
    {
        DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &ptrHeader->sdkHeader.userBufSize[index]);
    }

    /* Read out the application defined header: The header is specified in byte format: */
    for (index = 0; index < HSI_HEADER_MAX_APP_HDR_SIZE; index = index + 2)
    {
        DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, (uint16_t*)&ptrHeader->sdkHeader.appExtensionHeader[index]);
    }

    /* Determine the padding size: */
    paddingSize = HSIHeader_getPaddingSize(ptrHeader);

    /* Read out the padding: */
    while (paddingSize > 0)
    {
        DataCard_read16bit(fp, HSI_HEADER_SIZE_12BIT, &tmp);
        paddingSize = paddingSize - 1U;
    }
    return 0;
}

/**
*  @b Description
*  @n
*      The function is used to display num CBUFF units
*
*  @param[in]  prefixString
*      Prefix string to be displayed
*  @param[in]  ptrHeader
*      Pointer to the HSI Header
*  @param[in] fp
*      Pointer to the file from where the data is read
*  @param[in] numCBUFF
*      Number of CBUFF Units to be displayed
*
*  @retval
*      Not applicable
*/
static void DataCard_display(const char* prefixString, HSIHeader* ptrHeader, FILE* fp, uint16_t numCBUFF)
{
    uint16_t    tmp;

    /* We dump the first N CBUFF Units of the data payload: */
    printf("%s: ", prefixString);

    /* Read through and display the data */
    while (numCBUFF > 0)
    {
        /* Read out the data: */
        DataCard_read16bit(fp, ptrHeader->sdkHeader.dataSize, &tmp);
        numCBUFF = numCBUFF - 1;
        printf("%d ", (int16_t)tmp);
    }
    printf("\n");
}

/**
*  @b Description
*  @n
*      This is a utility function which is used to display the chirp quality
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*
*  @retval
*      Not applicable
*/
static void DataCard_displayCQ(FILE* fp, HSIHeader* ptrHeader)
{
    uint8_t     index;
    char        displayString[256];

    /* Display the CQ: */
    for (index = 0; index < HSI_HEADER_MAX_CQ; index++)
    {
        /* Was there a CQ specified? */
        if (ptrHeader->sdkHeader.cqDataSize[index] != 0U)
        {
            /* YES: Create a display string */
            sprintf(displayString, "CQ%d               :", index);
            DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cqDataSize[index]);
        }
    }

    return;
}

/**
*  @b Description
*  @n
*      This is a utility function which is used to display the user buffers
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*
*  @retval
*      Not applicable
*/
static void DataCard_displayUser(FILE* fp, HSIHeader* ptrHeader)
{
    uint8_t     index;
    char        displayString[256];

    /* Display the User Buffers: */
    for (index = 0; index < HSI_HEADER_MAX_USER_BUFFER; index++)
    {
        /* Was there a User Buffer specified? */
        if (ptrHeader->sdkHeader.userBufSize[index] != 0)
        {
            /* YES: Create a display string */
            sprintf(displayString, "User%d               :", index);
            DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);
            DataCard_skip(fp, ptrHeader, ptrHeader->sdkHeader.userBufSize[index] - DATA_CARD_MAX_DISPLAY_SIZE);
        }
    }
    return;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Interleaved Data Mode
*       - ADC Data Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_Interleaved_ADC
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;
    char        displayString[256];

    /* Compute the total length of the data: */
    totalLength = (ptrHeader->sdkHeader.adcDataSize * numRxChannels);

    /********************************************************************
    * ADC Data for all active channels (First & last N Units)
    * - Display the first N Units
    * - Skip to the last N Units
    * - Display the last N Units
    ********************************************************************/
    sprintf(displayString, "ADC [All Channels First %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /* Skip to the last N Units */
    skipSize = totalLength - (2 * DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_skip(fp, ptrHeader, skipSize);

    /* Display the last N Units */
    sprintf(displayString, "ADC [All Channels Last  %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Non-Interleaved Data Mode
*       - ADC Data Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_NonInterleaved_ADC
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint8_t     channelIndex;
    char        displayString[256];
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;

    /* Are we operating in single chirp or continuous mode? */
    if (ptrHeader->sdkHeader.chirpMode > 1U)
    {
        /********************************************************************
         * Multiple Chirp Mode:
         ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the ADC Data: */
                totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode);
            }
        }
    }
    else
    {
        /********************************************************************
         * Single Chirp/Continuous Mode: Cycle through all the channels
         ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /* YES: Account for the ADC Data: */
                totalLength = totalLength + ptrHeader->sdkHeader.adcDataSize;

                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Are we operating in Chirp or Continuous Mode?
                * - Skip to the last N Units */
                if (ptrHeader->sdkHeader.chirpMode == 0U)
                {
                    /* Continuous Mode: */
                    skipSize = (ptrHeader->sdkHeader.adcDataSize - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                }
                else
                {
                    /* Chirp Mode: */
                    skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                }
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);
            }
        }
    }
    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Interleaved Data Mode
*       - CP_ADC Data Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_Interleaved_CP_ADC
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;
    char        displayString[256];

    /******************************************************************
    * - Continuous Mode is not supported.
    * - Multiple Chirp mode is only supported on XWR16xx/XWR18xx/XWR68xx which operates
    *   only in Non-Interleaved mode
    ******************************************************************/
    assert(ptrHeader->sdkHeader.chirpMode == 1U);

    /* Compute the total length of the data: */
    totalLength = (ptrHeader->sdkHeader.adcDataSize * numRxChannels);
    totalLength = totalLength + (ptrHeader->sdkHeader.cpDataSize * numRxChannels);

    /********************************************************************
    * Single Chirp Mode:
    * - Chirp Profiles for all active channels (Display all the CP)
    * - Display the first N Units
    * - Skip to the last N Units
    * - Display the last N Units
    ********************************************************************/
    DataCard_display("CP  [All channels]:", ptrHeader, fp, (ptrHeader->sdkHeader.cpDataSize * numRxChannels));

    /********************************************************************
    * ADC Data for all active channels (First & last N Units)
    * - Display the first N Units
    * - Skip to the last N Units
    * - Display the last N Units
    ********************************************************************/
    DataCard_display("ADC [All Channels First %d Bytes]:", ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /* Skip to the last N Units of the ADC Data */
    skipSize = (ptrHeader->sdkHeader.adcDataSize * numRxChannels) - (2 * DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_skip(fp, ptrHeader, skipSize);

    /* Display the last N Units */
    DataCard_display("ADC [All Channels Last %d Bytes]:", ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Non-Interleaved Data Mode
*       - CP_ADC Data Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_NonInterleaved_CP_ADC
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint8_t     channelIndex;
    uint16_t    chirpIndex;
    char        displayString[256];
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;

    /* Debug: Continuous Chirp Mode is NOT supported */
    assert(ptrHeader->sdkHeader.chirpMode != 0U);

    /* Are we operating in single or multiple chirp mode? */
    if (ptrHeader->sdkHeader.chirpMode == 1U)
    {
        /********************************************************************
        * Single Chirp Mode:
        * - Chirp Profiles for each channels (Display the CP)
        * - ADC Data for each channels (Display only N Units)
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /* YES: Display the Chirp Profile */
                sprintf(displayString, "CP [Channel %d] :", (channelIndex + 1));
                DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);

                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the CP & ADC: */
                totalLength = totalLength + ptrHeader->sdkHeader.cpDataSize;
                totalLength = totalLength + ptrHeader->sdkHeader.adcDataSize;
            }
        }
    }
    else
    {
        /********************************************************************
        * Multiple Chirp Mode:
        * - Cycle through all the Chirps and display the CPx for each
        *   enabled ADC Channel
        * - The LVDS Stream has CP for all the channels in Multiple Chirp
        *   Mode even though the channels are not enabled.
        ********************************************************************/
        for (chirpIndex = 0; chirpIndex < ptrHeader->sdkHeader.chirpMode; chirpIndex++)
        {
            /* Cycle through all the channels: */
            for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
            {
                /* Is the channel enabled? */
                if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
                {
                    /* YES: Display the CP for the enabled channel */
                    sprintf(displayString, "CP%d [Channel %d] :", chirpIndex, (channelIndex + 1));
                    DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);
                }
                else
                {
                    /* NO: Skip the CP for this channel */
                    DataCard_skip(fp, ptrHeader, ptrHeader->sdkHeader.cpDataSize);
                }
                /* Account for each CP: */
                totalLength = totalLength + ptrHeader->sdkHeader.cpDataSize;
            }
        }

        /********************************************************************
        * ADC Data for each channels (Display only N Units)
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the ADC Data: */
                totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode);
            }
        }
    }
    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Interleaved Data Mode
*       - ADC_CP Data Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_Interleaved_ADC_CP
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;
    char        displayString[256];

    /******************************************************************
    * - Continuous Mode is not supported.
    * - Multiple Chirp mode is only supported on XWR16xx/XWR18xx/XWR68xx which operates
    *   only in Non-Interleaved mode
    ******************************************************************/
    assert(ptrHeader->sdkHeader.chirpMode == 1U);

    /* Compute the total length of the data: */
    totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * numRxChannels);
    totalLength = totalLength + (ptrHeader->sdkHeader.cpDataSize  * numRxChannels);

    /********************************************************************
    * ADC Data for all active channels (First & last N Units)
    * - Display the first N Units
    * - Skip to the last N Units
    * - Display the last N Units
    ********************************************************************/
    sprintf(displayString, "ADC [All Channels First %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /* Skip to the last N Units */
    skipSize = (ptrHeader->sdkHeader.adcDataSize * numRxChannels) - (2 * DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_skip(fp, ptrHeader, skipSize);

    /* Display the last N Units */
    sprintf(displayString, "ADC [All Channels Last  %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /********************************************************************
    * Single Chirp Mode:
    * - Chirp Profiles for all active channels (Display all the CP)
    ********************************************************************/
    DataCard_display("CP  [All channels]:", ptrHeader, fp, (ptrHeader->sdkHeader.cpDataSize * numRxChannels));

    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Non-Interleaved Data Mode
*       - ADC_CP Data Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_NonInterleaved_ADC_CP
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint16_t    chirpIndex;
    uint8_t     channelIndex;
    char        displayString[256];
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;

    /* Debug: Continuous Chirp Mode is NOT supported */
    assert(ptrHeader->sdkHeader.chirpMode != 0U);

    /* Are we operating in single or multiple chirp mode? */
    if (ptrHeader->sdkHeader.chirpMode == 1U)
    {
        /********************************************************************
        * Single Chirp Mode:
        * - ADC Data for each channels (Display only N Units)
        * - Chirp Profiles for each channels (Display the CP)
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the ADC Data: */
                totalLength = totalLength + ptrHeader->sdkHeader.adcDataSize;

                /* Display the Chirp Profile */
                sprintf(displayString, "CP [Channel %d] :", (channelIndex + 1));
                DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);

                /* Account for the CP Data: */
                totalLength = totalLength + ptrHeader->sdkHeader.cpDataSize;
            }
        }
    }
    else
    {
        /********************************************************************
        * Multiple Chirp Mode:
        * - ADC Data for each channels (Display only N Units)
        * - Chirp Profiles for each channels (Display the CP)
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the ADC Data: */
                totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode);
            }
        }

        /********************************************************************
        * - Cycle through all the Chirps and display the CPx for each
        *   enabled ADC Channel
        * - The LVDS Stream has CP for all the channels in Multiple Chirp
        *   Mode even though the channels are not enabled.
        ********************************************************************/
        for (chirpIndex = 0; chirpIndex < ptrHeader->sdkHeader.chirpMode; chirpIndex++)
        {
            /* Cycle through all the channels: */
            for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
            {
                /* Is the channel enabled? */
                if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
                {
                    /* YES: Display the CP for the enabled channel */
                    sprintf(displayString, "CP%d [Channel %d] :", chirpIndex, (channelIndex + 1));
                    DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);
                }
                else
                {
                    /* NO: Skip the CP for this channel */
                    DataCard_skip(fp, ptrHeader, ptrHeader->sdkHeader.cpDataSize);
                }
                /* Account for each CP: */
                totalLength = totalLength + ptrHeader->sdkHeader.cpDataSize;
            }
        }
    }
    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Interleaved Data Mode
*       - CP_ADC_CQ Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_Interleaved_CP_ADC_CQ
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;
    char        displayString[256];

    /******************************************************************
    * - Continuous Mode is not supported.
    * - Multiple Chirp mode is only supported on XWR16xx/XWR18xx/XWR68xx which operates
    *   only in Non-Interleaved mode
    ******************************************************************/
    assert(ptrHeader->sdkHeader.chirpMode == 1U);

    /* Compute the total length of the data: */
    totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * numRxChannels);
    totalLength = totalLength + (ptrHeader->sdkHeader.cpDataSize  * numRxChannels);
    totalLength = totalLength + (ptrHeader->sdkHeader.cqDataSize[0] +
                                 ptrHeader->sdkHeader.cqDataSize[1] +
                                 ptrHeader->sdkHeader.cqDataSize[2]);

    /********************************************************************
    * Single Chirp Mode:
    * - Chirp Profiles for all active channels (Display all the CP)
    * - ADC Data for all active channels (Display only N Units)
    * - Chirp Quality 1
    * - Chirp Quality 2
    * - Chirp Quality 3
    ********************************************************************/
    DataCard_display("CP  [All channels]:", ptrHeader, fp, (ptrHeader->sdkHeader.cpDataSize * numRxChannels));

    /********************************************************************
    * ADC Data for all active channels (First & last N Units)
    * - Display the first N Units
    * - Skip to the last N Units
    * - Display the last N Units
    ********************************************************************/
    sprintf(displayString, "ADC [All Channels First %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /* Skip to the last N Units */
    skipSize = (ptrHeader->sdkHeader.adcDataSize * numRxChannels) - (2 * DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_skip(fp, ptrHeader, skipSize);

    /* Display the last N Units */
    sprintf(displayString, "ADC [All Channels Last  %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /********************************************************************
    * Chirp Quality:
    ********************************************************************/
    DataCard_displayCQ(fp, ptrHeader);

    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Non Interleaved Data Mode
*       - CP_ADC_CQ Data Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_NonInterleaved_CP_ADC_CQ
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint16_t    chirpIndex;
    uint8_t     channelIndex;
    char        displayString[256];
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;

    /* Debug: Continuous Chirp Mode is NOT supported */
    assert(ptrHeader->sdkHeader.chirpMode != 0U);

    /* Are we operating in single or multiple chirp mode? */
    if (ptrHeader->sdkHeader.chirpMode == 1U)
    {
        /********************************************************************
        * Single Chirp Mode:
        * - Chirp Profiles for each channels (Display the CP)
        * - ADC Data for each channels (Display only N Units)
        * - Chirp Quality 1
        * - Chirp Quality 2
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /* YES: Display the Chirp Profile */
                sprintf(displayString, "CP [Channel %d] :", (channelIndex + 1));
                DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);

                /* Account for the CP Data: */
                totalLength = totalLength + ptrHeader->sdkHeader.cpDataSize;

                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the ADC Data: */
                totalLength = totalLength + ptrHeader->sdkHeader.adcDataSize;
            }
        }

        /* Display the chirp quality: */
        DataCard_displayCQ(fp, ptrHeader);

        /* Account for the CQ Data */
        totalLength = totalLength + (ptrHeader->sdkHeader.cqDataSize[0] +
                                     ptrHeader->sdkHeader.cqDataSize[1] +
                                     ptrHeader->sdkHeader.cqDataSize[2]);
    }
    else
    {
        /********************************************************************
        * Multiple Chirp Mode:
        * - Chirp Profiles for each channels (Display the CP)
        * - ADC Data for each channels (Display only N Units)
        * - Chirp Quality 1
        * - Chirp Quality 2
        * - Chirp Quality 3
        ********************************************************************/
        for (chirpIndex = 0; chirpIndex < ptrHeader->sdkHeader.chirpMode; chirpIndex++)
        {
            /* Cycle through all the channels: */
            for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
            {
                /* Is the channel enabled? */
                if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
                {
                    /* YES: Display the CP for the enabled channel */
                    sprintf(displayString, "CP%d [Channel %d] :", chirpIndex, (channelIndex + 1));
                    DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);
                }
                else
                {
                    /* NO: Skip the CP for this channel */
                    DataCard_skip(fp, ptrHeader, ptrHeader->sdkHeader.cpDataSize);
                }
                /* Account for each CP: */
                totalLength = totalLength + ptrHeader->sdkHeader.cpDataSize;
            }
        }

        /********************************************************************
        * ADC Data for each channels (Display only N Units)
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the ADC Data: */
                totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode);
            }
        }

        /********************************************************************
        * Chirp Quality:
        ********************************************************************/
        DataCard_displayCQ(fp, ptrHeader);

        /* Account for the CQ Data */
        totalLength = totalLength + (ptrHeader->sdkHeader.cqDataSize[0] +
                                     ptrHeader->sdkHeader.cqDataSize[1] +
                                     ptrHeader->sdkHeader.cqDataSize[2]);
    }
    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Interleaved Data Mode
*       - ADC_USER Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_Interleaved_ADC_USER
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;
    char        displayString[256];

    /* Compute the total length of the data: */
    totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * numRxChannels);
    totalLength = totalLength + (ptrHeader->sdkHeader.userBufSize[0] +
                                 ptrHeader->sdkHeader.userBufSize[1] +
                                 ptrHeader->sdkHeader.userBufSize[2]);

    /********************************************************************
    * ADC Data for all active channels (First & last N Units)
    * - Display the first N Units
    * - Skip to the last N Units
    * - Display the last N Units
    ********************************************************************/
    sprintf(displayString, "ADC [All Channels First %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /* Skip to the last N Units */
    skipSize = (ptrHeader->sdkHeader.adcDataSize * numRxChannels) - (2 * DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_skip(fp, ptrHeader, skipSize);

    /* Display the last N Units */
    sprintf(displayString, "ADC [All Channels Last  %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /********************************************************************
    * User Data:
    ********************************************************************/
    DataCard_displayUser(fp, ptrHeader);

    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Non Interleaved Data Mode
*       - ADC_USER Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_NonInterleaved_ADC_USER
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint8_t     channelIndex;
    char        displayString[256];
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;

    /********************************************************************
    * - ADC Data for each channels (Display only N Units)
    ********************************************************************/
    for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
    {
        /* Is the channel enabled? */
        if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
        {
            /********************************************************************
            * ADC Data for active channels (First & last N Units)
            * - Display the first N Units
            * - Skip to the last N Units
            * - Display the last N Units
            ********************************************************************/
            sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
            DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

            /* Skip to the last N Units */
            skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
            DataCard_skip(fp, ptrHeader, skipSize);

            /* Display the last N Units */
            sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
            DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

            /* Skip the rest of the data till the next channel data */
            DataCard_skip(fp, ptrHeader, ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - DATA_CARD_MAX_DISPLAY_SIZE));

            /* Account for the ADC Data: */
            totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode);
        }
    }

    /* Display the user data: */
    DataCard_displayUser(fp, ptrHeader);

    /* Compute the total length of the data: */
    totalLength = totalLength + (ptrHeader->sdkHeader.userBufSize[0] +
                                 ptrHeader->sdkHeader.userBufSize[1] +
                                 ptrHeader->sdkHeader.userBufSize[2]);
    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Interleaved Data Mode
*       - CP_ADC_CQ_USER Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_Interleaved_CP_ADC_CQ_USER
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;
    char        displayString[256];

    /******************************************************************
    * - Continuous Mode is not supported.
    * - Multiple Chirp mode is only supported on XWR16xx/XWR18xx/XWR68xx which operates
    *   only in Non-Interleaved mode
    ******************************************************************/
    assert(ptrHeader->sdkHeader.chirpMode == 1U);

    /* Compute the total length of the data: */
    totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * numRxChannels);
    totalLength = totalLength + (ptrHeader->sdkHeader.cpDataSize  * numRxChannels);
    totalLength = totalLength + (ptrHeader->sdkHeader.cqDataSize[0] +
                                 ptrHeader->sdkHeader.cqDataSize[1] +
                                 ptrHeader->sdkHeader.cqDataSize[2]);
    totalLength = totalLength + (ptrHeader->sdkHeader.userBufSize[0] +
                                 ptrHeader->sdkHeader.userBufSize[1] +
                                 ptrHeader->sdkHeader.userBufSize[2]);

    /********************************************************************
    * Single Chirp Mode:
    * - Chirp Profiles for all active channels (Display all the CP)
    ********************************************************************/
    DataCard_display("CP  [All channels]:", ptrHeader, fp, (ptrHeader->sdkHeader.cpDataSize * numRxChannels));

    /********************************************************************
    * ADC Data for all active channels (First & last N Units)
    * - Display the first N Units
    * - Skip to the last N Units
    * - Display the last N Units
    ********************************************************************/
    sprintf(displayString, "ADC [All Channels First %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /* Skip to the last N Units */
    skipSize = (ptrHeader->sdkHeader.adcDataSize * numRxChannels) - (2 * DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_skip(fp, ptrHeader, skipSize);

    /* Display the last N Units */
    sprintf(displayString, "ADC [All Channels Last  %d Bytes]:", DATA_CARD_MAX_DISPLAY_SIZE);
    DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

    /********************************************************************
    * Chirp Quality:
    ********************************************************************/
    DataCard_displayCQ(fp, ptrHeader);

    /********************************************************************
    * User:
    ********************************************************************/
    DataCard_displayUser(fp, ptrHeader);
    return totalLength;
}

/**
*  @b Description
*  @n
*      The function is used to handle the following:-
*       - Non-Interleaved Data Mode
*       - CP_ADC_CQ_USER Format
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in]  ptrHeader
*      Pointer to the data card header
*  @param[in]  numRxChannels
*      Number of ADC receive channels.
*
*  @retval
*      Total Transfer size
*/
static uint32_t DataCard_NonInterleaved_CP_ADC_CQ_USER
(
    FILE*       fp,
    HSIHeader*  ptrHeader,
    uint8_t     numRxChannels
)
{
    uint8_t     channelIndex;
    char        displayString[256];
    uint32_t    totalLength = 0U;
    uint32_t    skipSize;
    uint16_t    chirpIndex;

    /* Debug: Continuous Chirp Mode is NOT supported */
    assert(ptrHeader->sdkHeader.chirpMode != 0U);

    /* Are we operating in single or multiple chirp mode? */
    if (ptrHeader->sdkHeader.chirpMode == 1U)
    {
        /********************************************************************
        * Single Chirp Mode:
        * - Chirp Profiles for each channels (Display the CP)
        * - ADC Data for each channels (Display only N Units)
        * - Chirp Quality 1
        * - Chirp Quality 2
        * - Chirp Quality 3
        * - User Data
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /********************************************************************
                * Chirp Profile:
                ********************************************************************/
                sprintf(displayString, "CP [Channel %d] :", (channelIndex + 1));
                DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);

                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);
            }
        }

        /********************************************************************
        * Chirp Quality:
        ********************************************************************/
        DataCard_displayCQ(fp, ptrHeader);

        /********************************************************************
        * User:
        ********************************************************************/
        DataCard_displayUser(fp, ptrHeader);

        /* Compute the total length of the data: */
        totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * numRxChannels);
        totalLength = totalLength + (ptrHeader->sdkHeader.cpDataSize  * numRxChannels);
        totalLength = totalLength + (ptrHeader->sdkHeader.cqDataSize[0] +
                                     ptrHeader->sdkHeader.cqDataSize[1] +
                                     ptrHeader->sdkHeader.cqDataSize[2]);
        totalLength = totalLength + (ptrHeader->sdkHeader.userBufSize[0] +
                                     ptrHeader->sdkHeader.userBufSize[1] +
                                     ptrHeader->sdkHeader.userBufSize[2]);
    }
    else
    {
        /********************************************************************
        * Multiple Chirp Mode:
        * - Chirp Profiles for each channels (Display the CP)
        * - ADC Data for each channels (Display only N Units)
        * - Chirp Quality 1
        * - Chirp Quality 2
        * - Chirp Quality 3
        * - User Data
        ********************************************************************/
        for (chirpIndex = 0; chirpIndex < ptrHeader->sdkHeader.chirpMode; chirpIndex++)
        {
            /* Cycle through all the channels: */
            for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
            {
                /* Is the channel enabled? */
                if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
                {
                    /* YES: Display the CP for the enabled channel */
                    sprintf(displayString, "CP%d [Channel %d] :", chirpIndex, (channelIndex + 1));
                    DataCard_display(displayString, ptrHeader, fp, ptrHeader->sdkHeader.cpDataSize);
                }
                else
                {
                    /* NO: Skip the CP for this channel */
                    DataCard_skip(fp, ptrHeader, ptrHeader->sdkHeader.cpDataSize);
                }
                /* Account for each CP: */
                totalLength = totalLength + ptrHeader->sdkHeader.cpDataSize;
            }
        }

        /********************************************************************
        * ADC Data for each channels (Display only N Units)
        ********************************************************************/
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (ptrHeader->sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /********************************************************************
                * ADC Data for active channels (First & last N Units)
                * - Display the first N Units
                * - Skip to the last N Units
                * - Display the last N Units
                ********************************************************************/
                sprintf(displayString, "ADC [Channel %d First %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Skip to the last N Units */
                skipSize = ((ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode) - (2 * DATA_CARD_MAX_DISPLAY_SIZE));
                DataCard_skip(fp, ptrHeader, skipSize);

                /* Display the last N Units */
                sprintf(displayString, "ADC [Channel %d Last %d bytes]:", (channelIndex + 1), DATA_CARD_MAX_DISPLAY_SIZE);
                DataCard_display(displayString, ptrHeader, fp, DATA_CARD_MAX_DISPLAY_SIZE);

                /* Account for the ADC Data: */
                totalLength = totalLength + (ptrHeader->sdkHeader.adcDataSize * ptrHeader->sdkHeader.chirpMode);
            }
        }

        /********************************************************************
        * Chirp Quality:
        ********************************************************************/
        DataCard_displayCQ(fp, ptrHeader);

        /* Account for the CQ Data */
        totalLength = totalLength + (ptrHeader->sdkHeader.cqDataSize[0] +
                                     ptrHeader->sdkHeader.cqDataSize[1] +
                                     ptrHeader->sdkHeader.cqDataSize[2]);

        /********************************************************************
        * User:
        ********************************************************************/
        DataCard_displayUser(fp, ptrHeader);

        /* Account for the user buffer sizes: */
        totalLength = totalLength + (ptrHeader->sdkHeader.userBufSize[0] +
                                     ptrHeader->sdkHeader.userBufSize[1] +
                                     ptrHeader->sdkHeader.userBufSize[2]);
    }
    return totalLength;
}

/**
*  @b Description
*  @n
*      Entry Point
*
*  @param[in]  argc
*      Number of arguments
*  @param[in] argv
*      Argument List
*
*  @retval
*      Success - 0
*  @retval
*      Error   - <0
*/
int main(int32_t argc, char* argv[])
{
    FILE*           fp;
    uint16_t        channelIndex;
    HSIHeader       hsiHeader;
    char            displayString[256];
    uint8_t         numRxChannels = 0;
    uint32_t        numHeader = 0;
    uint32_t        totalLength = 0;
    uint8_t         index;

    /* Sanity Check: Validate the arguments */
    if (argc != 2)
    {
        DataCard_printUsage();
        return -1;
    }

    /* Open the file: */
    fp = fopen(argv[1], "rb");
    if (fp == NULL)
    {
        printf("Error: Unable to open the file %s\n", argv[1]);
        return -1;
    }
    printf("Debug: File: %s\n", argv[1]);

    /* Cycle through the file: */
    while (1)
    {
        /* Once we are done with the N headers we can stop: */
        if (numHeader == 512)
            break;

        /* Read the data card header: */
        if (DataCard_readHeader(fp, &hsiHeader) < 0)
            return -1;

        /* New Header has been detected: */
        totalLength = 0;
        numRxChannels = 0;
        numHeader = numHeader + 1;

        /* Get the number of active ADC Channels */
        for (channelIndex = 0U; channelIndex < 4U; channelIndex++)
        {
            /* Is the channel enabled? */
            if (hsiHeader.sdkHeader.rxChannelStatus & (1 << channelIndex))
            {
                /* YES: Increment the number of receive channels: */
                numRxChannels = numRxChannels + 1;
            }
        }

        /* Get the version string: */
        DataCard_toVersionString(&hsiHeader, displayString, sizeof(displayString));

        /* Debug: */
        printf("******************************************************************\n");
        printf("Debug: [Detected] %s with mmWave SDK Version: %s Platform: %s\n",
            DataCard_toIdString(hsiHeader.dataCardHeader.id), displayString,
            DataCard_toPlatformString(hsiHeader.sdkHeader.platform));

        /* Display the header & padding size: */
        printf("Debug: Header size: %d Padding size: %d\n",
            hsiHeader.sdkHeader.headerSize, HSIHeader_getPaddingSize(&hsiHeader));

        /* Convert the receive channel status into a string: */
        DataCard_toChannelStatusString(hsiHeader.sdkHeader.rxChannelStatus, &displayString[0], sizeof(displayString));

        /* Display the Header Information and sizes: */
        printf("Debug: %s %s %s %s\n",
            displayString,
            DataCard_toInterleavedModeString(hsiHeader.sdkHeader.interleavedMode),
            DataCard_toDataSizeString(hsiHeader.sdkHeader.dataSize),
            DataCard_toDataFmtString(hsiHeader.sdkHeader.dataFmt));
        printf("Debug: %s\n", DataCard_toChirpModeString(hsiHeader.sdkHeader.chirpMode));
        printf("Debug: ADC:   %d CP:    %d \n",
            hsiHeader.sdkHeader.adcDataSize,
            hsiHeader.sdkHeader.cpDataSize);
        printf("Debug: CQ0:   %d CQ1:   %d CQ2:   %d\n",
            hsiHeader.sdkHeader.cqDataSize[0],
            hsiHeader.sdkHeader.cqDataSize[1],
            hsiHeader.sdkHeader.cqDataSize[2]);
        printf("Debug: User1: %d User2: %d User3 : %d\n",
            hsiHeader.sdkHeader.userBufSize[0],
            hsiHeader.sdkHeader.userBufSize[1],
            hsiHeader.sdkHeader.userBufSize[2]);
        printf("Debug: Application Header: ");
        for (index = 0; index < HSI_HEADER_MAX_APP_HDR_SIZE; index++)
        {
            printf("0x%x ", hsiHeader.sdkHeader.appExtensionHeader[index]);
        }
        printf("\n");

        /* Compute the total payload size: This is dependent on the data format. */
        switch (hsiHeader.sdkHeader.dataFmt)
        {
        case HSI_HEADER_FMT_ADC:
        {
            /* Are we operating in Interleaved/Non-Interleaved Mode? */
            if (hsiHeader.sdkHeader.interleavedMode == HSI_HEADER_MODE_INTERLEAVED)
            {
                /* Interleaved Mode: */
                totalLength = DataCard_Interleaved_ADC(fp, &hsiHeader, numRxChannels);
            }
            else
            {
                /* Non-Interleaved Mode: */
                totalLength = DataCard_NonInterleaved_ADC(fp, &hsiHeader, numRxChannels);
            }
            break;
        }
        case HSI_HEADER_FMT_CP_ADC:
        {
            /* Are we operating in Interleaved/Non-Interleaved Mode? */
            if (hsiHeader.sdkHeader.interleavedMode == HSI_HEADER_MODE_INTERLEAVED)
            {
                /* Interleaved Mode: */
                totalLength = DataCard_Interleaved_CP_ADC(fp, &hsiHeader, numRxChannels);
            }
            else
            {
                /* Non-Interleaved Mode: */
                totalLength = DataCard_NonInterleaved_CP_ADC(fp, &hsiHeader, numRxChannels);
            }
            break;
        }
        case HSI_HEADER_FMT_ADC_CP:
        {
            /* Are we operating in Interleaved/Non-Interleaved Mode? */
            if (hsiHeader.sdkHeader.interleavedMode == HSI_HEADER_MODE_INTERLEAVED)
            {
                totalLength = DataCard_Interleaved_ADC_CP(fp, &hsiHeader, numRxChannels);
            }
            else
            {
                totalLength = DataCard_NonInterleaved_ADC_CP(fp, &hsiHeader, numRxChannels);
            }
            break;
        }
        case HSI_HEADER_FMT_CP_ADC_CQ:
        {
            /* Are we operating in Interleaved/Non-Interleaved Mode? */
            if (hsiHeader.sdkHeader.interleavedMode == HSI_HEADER_MODE_INTERLEAVED)
            {
                totalLength = DataCard_Interleaved_CP_ADC_CQ(fp, &hsiHeader, numRxChannels);
            }
            else
            {
                totalLength = DataCard_NonInterleaved_CP_ADC_CQ(fp, &hsiHeader, numRxChannels);
            }
            break;
        }
        case HSI_HEADER_FMT_USER:
        {
            /* Display the user buffers: */
            DataCard_displayUser(fp, &hsiHeader);

            /* Compute the total length of the data: */
            totalLength = totalLength + (hsiHeader.sdkHeader.userBufSize[0] +
                                         hsiHeader.sdkHeader.userBufSize[1] +
                                         hsiHeader.sdkHeader.userBufSize[2]);
            break;
        }
        case HSI_HEADER_FMT_ADC_USER:
        {
            /* Are we operating in Interleaved/Non-Interleaved Mode? */
            if (hsiHeader.sdkHeader.interleavedMode == HSI_HEADER_MODE_INTERLEAVED)
            {
                totalLength = DataCard_Interleaved_ADC_USER(fp, &hsiHeader, numRxChannels);
            }
            else
            {
                totalLength = DataCard_NonInterleaved_ADC_USER(fp, &hsiHeader, numRxChannels);
            }
            break;
        }
        case HSI_HEADER_FMT_CP_ADC_CQ_USER:
        {
            /* Are we operating in Interleaved/Non-Interleaved Mode? */
            if (hsiHeader.sdkHeader.interleavedMode == HSI_HEADER_MODE_INTERLEAVED)
            {
                totalLength = DataCard_Interleaved_CP_ADC_CQ_USER(fp, &hsiHeader, numRxChannels);
            }
            else
            {
                totalLength = DataCard_NonInterleaved_CP_ADC_CQ_USER(fp, &hsiHeader, numRxChannels);
            }
            break;
        }
        default:
        {
            /* Error: This should never occur */
            assert(0);
        }
        }

        /* The total length will include the header size */
        totalLength = totalLength + hsiHeader.sdkHeader.headerSize;

        /* Convert the total length to bytes */
        totalLength = HSIHeader_toBytes(totalLength);
        if (totalLength != HSIHeader_getTotalLength(&hsiHeader))
        {
            printf("Error: Mismatch in total length detected Computed = %d Expected = %d\n",
                totalLength, HSIHeader_getTotalLength(&hsiHeader));
            assert(0);
            return -1;
        }
    }

    /* Close the file: */
    fclose(fp);
    return 0;
}
