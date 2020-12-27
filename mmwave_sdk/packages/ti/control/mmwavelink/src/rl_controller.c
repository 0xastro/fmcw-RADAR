/****************************************************************************************
 * FileName     : rl_controller.c
 *
 * Description  : This file defines the functions to construct Radar Messages.
 *
 ****************************************************************************************
 * (C) Copyright 2014, Texas Instruments Incorporated. - TI web address www.ti.com
 *---------------------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

 /*
 ****************************************************************************************
 * Revision History   :
 *---------------------------------------------------------------------------------------
 * Version  Date        Author             Defect No               Description
 *---------------------------------------------------------------------------------------
 * 0.1.0    12May2015   Kaushal Kukkar                    Initial Version
 *
 * 0.6.0    15Nov2016   Kaushal Kukkar    AUTORADAR-666   Logging Feature
 *
 * 0.9.1                Jitendra Gupta    MMWL-5          Code size optimization
 ****************************************************************************************
 */

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stdlib.h>
#include <string.h>
#include <ti/control/mmwavelink/include/rl_datatypes.h>
#include <ti/control/mmwavelink/include/rl_controller.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/control/mmwavelink/include/rl_trace.h>

/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */

/** @fn rlReturnVal_t rlAppendSubBlock(rlUInt8_t rhcpPayload[],
*                       rlUInt16_t sbId, rlUInt16_t sbLen, rlUInt8_t *sbData)
*
*   @brief Appends sub block data to payload buffer
*   @param[out] rhcpPayload - payload buffer
*   @param[in] sbId - sub block Id
*   @param[in] sbLen - sub block Length
*   @param[in] sbData - sub block data
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Appends sub block data to payload buffer
*/
/* DesignId :  */
/* Requirements : AUTORADAR_REQ-772 */
rlReturnVal_t rlAppendSubBlock(rlUInt8_t rhcpPayload[],
                    rlUInt16_t sblkId, rlUInt16_t sbLen, rlUInt8_t* sbData)
{
    rlReturnVal_t retVal;
    /* check for null pointer */
    if ((RL_NULL_PTR == rhcpPayload) || (sbLen > RL_CMD_PL_LEN_MAX))
    {
        /* set error code */
        retVal = RL_RET_CODE_FATAL_ERROR;
    }
    else
    {
        /* Append SB Id  */
        /*AR_CODE_REVIEW MR:R.11.2 <APPROVED> "The Payload SbId is of type UINT16,
            * so pointer conversion type to UINT16*
            * is required" */
        /*LDRA_INSPECTED 94 S */
        /*LDRA_INSPECTED 95 S */
        /*LDRA_INSPECTED 87 S */
        *((rlUInt16_t*)(rhcpPayload + RL_SBC_ID_INDEX)) = sblkId;

        /* Append SB Len */
        /*AR_CODE_REVIEW MR:R.11.2 <APPROVED> "The Payload SbLen is of type UINT16,
            * so pointer conversion type to UINT16*
            * is required" */
        /*LDRA_INSPECTED 94 S */
        /*LDRA_INSPECTED 95 S */
        /*LDRA_INSPECTED 87 S */
        *((rlUInt16_t*)(rhcpPayload + RL_SBC_LEN_INDEX)) = sbLen + \
            RL_SBC_ID_SIZE + RL_SBC_LEN_SIZE;

        /* Append SB Payload */
        if ((sbLen > 0U) && (RL_NULL_PTR != sbData))
        {
            (void)memcpy(&rhcpPayload[RL_SBC_PL_INDEX], sbData, sbLen);
            retVal = RL_RET_CODE_OK;
        }
        else
        {
            /* For most of get command sub-block length is zero and sbData is NULL */
            if ((sbLen == 0U) && (RL_NULL_PTR == sbData))
            {
                retVal = RL_RET_CODE_OK;
            }
            else
            {
                retVal = RL_RET_CODE_INVALID_INPUT;
            }
        }
    }
    return retVal;
}

/** @fn rlReturnVal_t rlAppendDummy(rlUInt8_t rhcpPayload[], rlUInt8_t dummyLen)
*
*   @brief Appends dummy bytes to Payload buffer
*   @param[out] rhcpPayload - payload buffer
*   @param[in] dummyLen - numnber of dummy bytes
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Appends dummy bytes to Payload buffer
*/
/* DesignId :  */
/* Requirements :  */
rlReturnVal_t rlAppendDummy(rlUInt8_t rhcpPayload[], rlUInt8_t dummyLen)
{
    rlUInt8_t indx;

    RL_LOGV_ARG0("rlAppendDummy starts...\n");

    /* In the given array fill dummy byte for the requested length */
    for (indx = 0U; indx < dummyLen; indx++)
    {
        /* fill dummy byte */
        rhcpPayload[indx] = RL_PROTOCOL_DUMMY_BYTE;
    }
    RL_LOGV_ARG0("rlAppendDummy ends...\n");

    return RL_RET_CODE_OK;
}

/** @fn rlReturnVal_t rlGetSubBlock(rlUInt8_t rhcpPayload[],
*                 rlUInt16_t* sbId, rlUInt16_t* sbLen, rlUInt8_t* sbData)
*
*   @brief Reads sub block data from payload buffer
*   @param[in] rhcpPayload - payload buffer
*   @param[out] sbId - sub block Id
*   @param[out] sbLen - sub block Length
*   @param[out] sbData - sub block data
*
*   @return rlReturnVal_t Success - 0, Failure - Error Code
*
*   Reads sub block data from payload buffer
*/
/* DesignId :  */
/* Requirements :  */
rlReturnVal_t rlGetSubBlock(rlUInt8_t rhcpPayload[],
                   rlUInt16_t* sbcId, rlUInt16_t* sbLen, rlUInt8_t* sbData)
{
    rlReturnVal_t retVal;

    RL_LOGV_ARG0("rlGetSubBlock starts...\n");

    /* check for NULL prointer for all input parameters */
    if ((RL_NULL_PTR == rhcpPayload) || (RL_NULL_PTR == sbcId) ||\
        (RL_NULL_PTR == sbLen) || (RL_NULL_PTR == sbData))
    {
        /* set error code */
        retVal = RL_RET_CODE_FATAL_ERROR;
    }
    else
    {
        /* Get SB Id */
        /*AR_CODE_REVIEW MR:R.11.2 <APPROVED> "The Payload SbId is of type UINT16,
            * so pointer conversion type to UINT16*
            * is required" */
        /*LDRA_INSPECTED 94 S */
        /*LDRA_INSPECTED 95 S */
        /*LDRA_INSPECTED 87 S */
        *sbcId = *((rlUInt16_t*)(rhcpPayload + RL_SBC_ID_INDEX));

        /* Get SB Len */
        /*AR_CODE_REVIEW MR:R.11.2 <APPROVED> "The Payload SbLen is of type UINT16,
            * so pointer conversion type to UINT16*
            * is required" */
        /*LDRA_INSPECTED 94 S */
        /*LDRA_INSPECTED 95 S */
        /*LDRA_INSPECTED 87 S */
        *sbLen = *((rlUInt16_t*)(rhcpPayload + RL_SBC_LEN_INDEX));
        /* check if sub-block length is beyond defined limit */
        if ((*sbLen > RL_CMD_PL_LEN_MAX))
        {
            /* set error code */
            retVal = RL_RET_CODE_FATAL_ERROR;
        }
        else
        {
            /* Get SB Payload */
            if ((*sbLen > 0U) && (RL_NULL_PTR != sbData))
            {
                /* copy input payload to sub-block data buffer */
                (void)memcpy(sbData, &rhcpPayload[RL_SBC_PL_INDEX], (*sbLen -
                                    (RL_SBC_ID_SIZE + RL_SBC_LEN_SIZE)));
                RL_LOGD_ARG0("rhcpPayload is copied\n");
            }
            retVal = RL_RET_CODE_OK;
        }
    }
    RL_LOGV_ARG0("rlGetSubBlock ends...\n");
    return retVal;
}

/** @fn void rlGetSubBlockId(rlUInt8_t rhcpPayload[], rlUInt16_t* sbId)
*
*   @brief Reads sub block Id from payload buffer
*   @param[in] rhcpPayload - payload buffer
*   @param[out] sbId - sub block Id
*
*   @return NONE
*
*   Reads sub block Id from payload buffer
*/
/* DesignId :  */
/* Requirements :  */
void rlGetSubBlockId(const rlUInt8_t rhcpPayload[], rlUInt16_t* sbcId)
{
    /* Get SB Id */
    /*AR_CODE_REVIEW MR:R.11.2 <APPROVED> "The Payload SbId is of type UINT16,
        * so pointer conversion type to UINT16*
        * is required" */
    /*LDRA_INSPECTED 94 S */
    /*LDRA_INSPECTED 95 S */
    /*LDRA_INSPECTED 87 S */
    *sbcId = *((rlUInt16_t*)(rhcpPayload + RL_SBC_ID_INDEX));
    RL_LOGD_ARG0("DEBUG: Parsed sub block ID\n");
}

/** @fn void rlGetSubBlockLen(rlUInt8_t rhcpPayload[], rlUInt16_t* sbLen)
*
*   @brief Reads sub block Length from payload buffer
*   @param[in] rhcpPayload - payload buffer
*   @param[out] sbLen - sub block Len
*
*   @return NONE
*
*   Reads sub block Length from payload buffer
*/
/* DesignId :  */
/* Requirements :  */
void rlGetSubBlockLen(const rlUInt8_t rhcpPayload[], rlUInt16_t* sbcLen)
{
    /* Get SB Len */
    /*AR_CODE_REVIEW MR:R.11.2 <APPROVED> "The Payload SbLen is of type UINT16,
        * so pointer conversion type to UINT16*
        * is required" */
    /*LDRA_INSPECTED 94 S */
    /*LDRA_INSPECTED 95 S */
    /*LDRA_INSPECTED 87 S */
    *sbcLen = *((rlUInt16_t*)(rhcpPayload + RL_SBC_LEN_INDEX));
    RL_LOGD_ARG0("DEBUG: Parsed sub block len\n");
}
/*
 * END OF rl_controller.c FILE
 */
