/*
 *   @file  serialize.cpp
 *
 *   @brief
 *      The file is used to serialize the data received from the data card
 *      over the LVDS lanes into a sequential buffer. The serialization
 *      process is required only if the data samples are complex.
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
static void Serialize_printUsage(void)
{
    printf("serialize.exe <LVDS Lanes> <RAW Binary File> <Serialized Output File>");
    return;
}

/**
 *  @b Description
 *  @n
 *   The function is used to read out 16bit data from the file.
 *
 *  @param[in]  fp
 *      Pointer to the file which is to be read
 *  @param[out] ptrData
 *      Data Read from the file
 *
 *  @retval
 *       Success -   0
 *  @retval
 *       Error   -   <0
 */
static int32_t Serialize_read16bit(FILE* fp, uint16_t* ptrData)
{
    if (fread(ptrData, sizeof(uint16_t), 1, fp) != 1U)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
*  @b Description
*  @n
*   The function is used to write out 16bit data to the file.
*
*  @param[in]  fp
*      Pointer to the file which is to be read
*  @param[in] ptrData
*      Data to be written to the file
*
*  @retval
*       Not applicable
*/
static void Serialize_write16bit(FILE* fp, uint16_t data)
{
    assert(fwrite(&data, sizeof(uint16_t), 1, fp) != 0U);
}

/**
 *  @b Description
 *  @n
 *   The function takes the input LVDS Stream data and serializes it back
 *   discounting the LVDS Lanes. The function does not validate the size
 *   of the input and output arrays. Array size should be 2*numLVDSLanes
 *
 *  @param[in]  numLVDSLanes
 *      Number of LVDS Lanes
 *  @param[in] ptrInputData
 *      Input Data to be serialized
 *  @param[out] ptrOutputData
 *      Serialized output data
 *
 *  @retval
 *       Not applicable
*/
static void Serialize_serializer
(
    uint32_t    numLVDSLanes,
    uint16_t*   ptrInputData,
    uint16_t*   ptrOutputData
)
{
    /* Are we operating on 2 or 4 lanes? */
    if (numLVDSLanes == 4U)
    {
        /* 4 Lanes: */
        ptrOutputData[0] = ptrInputData[0];
        ptrOutputData[1] = ptrInputData[4];
        ptrOutputData[2] = ptrInputData[1];
        ptrOutputData[3] = ptrInputData[5];
        ptrOutputData[4] = ptrInputData[2];
        ptrOutputData[5] = ptrInputData[6];
        ptrOutputData[6] = ptrInputData[3];
        ptrOutputData[7] = ptrInputData[7];
    }
    else
    {
        /* 2 Lanes: */
        ptrOutputData[0] = ptrInputData[0];
        ptrOutputData[1] = ptrInputData[2];
        ptrOutputData[2] = ptrInputData[1];
        ptrOutputData[3] = ptrInputData[3];
    }
    return;
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
    FILE*           fpInput;
    FILE*           fpOutput;
    uint32_t        numLVDSLanes;
    uint16_t        inputData[8];
    uint16_t        outputData[8];
    uint16_t        numUnitsToSerialize;
    uint16_t        index;
    bool            completed = false;
    uint32_t        counter = 0;

    /* Sanity Check: Validate the arguments */
    if (argc != 4)
    {
        Serialize_printUsage();
        return -1;
    }

    /* Get the number of LVDS Lanes: */
    numLVDSLanes = atoi (argv[1]);
    if ((numLVDSLanes != 2U) && (numLVDSLanes != 4U))
    {
        printf ("Error: Invalid number of LVDS Lanes\n");
        printf ("       XWR14xx = 4 LVDS Lanes\n");
        printf ("       XWR16xx/XWR18xx/XWR68xx = 2 LVDS Lanes\n");
        return -1;
    }

    /* Open the input file: */
    fpInput = fopen(argv[2], "rb");
    if (fpInput == NULL)
    {
        printf("Error: Unable to open the file %s\n", argv[1]);
        return -1;
    }

    /* Open the output file: */
    fpOutput = fopen(argv[3], "wb");
    if (fpOutput == NULL)
    {
        printf("Error: Unable to open the file %s\n", argv[2]);
        /* Close the input file */
        fclose (fpInput);
        return -1;
    }

    /* The received data stream from the XWR1xx is operating in Complex mode
     * - Accounting for the number of LVDS Lanes we will serialize this into
     *   a contiguous buffer */
    numUnitsToSerialize = numLVDSLanes * 2U;

    /* Debug Message: */
    printf("Starting the Serialization Process \n");

    /* Cycle through till the end of the file: */
    while (1)
    {
        /* Reset the memory: */
        memset ((void *)&inputData[0], 0,  sizeof(inputData));
        memset ((void *)&outputData[0], 0, sizeof(outputData));

        /* Read out the number of units to serialize */
        for (index = 0; index < numUnitsToSerialize; index++)
        {
            if (Serialize_read16bit(fpInput, &inputData[index]) != 0)
            {
                completed = true;
                break;
            }
        }

        /* Did we reach the end of the file? */
        if (completed == true)
            break;

        /* Increment the counter: */
        counter = counter + 1;
        if ((counter % 10000U) == 0U)
            printf(".");

        /* Serialize the input: */
        Serialize_serializer (numLVDSLanes, &inputData[0], &outputData[0]);

        /* Write out the output data to the file: */
        for (index = 0; index < numUnitsToSerialize; index++)
        {
            Serialize_write16bit (fpOutput, outputData[index]);
        }
    }
    printf("\nCompleted\n");

    /* Close the files: */
    fclose (fpInput);
    fclose (fpOutput);
    return 0;
}
