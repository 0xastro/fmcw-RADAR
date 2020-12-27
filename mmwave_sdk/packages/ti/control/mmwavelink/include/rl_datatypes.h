/****************************************************************************************
 * FileName     : rl_datatypes.h
 *
 * Description  : This file defines the datatypes.
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
 *
 ****************************************************************************************
 * Revision History   :
 *---------------------------------------------------------------------------------------
 * Version  Date        Author    Defect No       Description
 *---------------------------------------------------------------------------------------
 * 0.1      12May2015   Kaushal    -               Initial Version
 *
 ****************************************************************************************
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ******************************************************************************
 */
#ifndef RL_DATATYPES_H
#define RL_DATATYPES_H

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stddef.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPE-DEFINE STRUCT/ENUM/UNION DEFINITIONS
 *******************************************************************************
 */
/*! \brief
 * Standard C data types typedef
 */
typedef unsigned char                   rlUInt8_t;
typedef unsigned short                  rlUInt16_t;
typedef unsigned int                    rlUInt32_t;
typedef unsigned long long              rlUInt64_t;
typedef signed long long                rlInt64_t;
typedef char                            rlInt8_t;
typedef signed char                     rlSInt8_t;
typedef signed short                    rlInt16_t;
typedef signed int                      rlInt32_t;
typedef void                            rlVoid_t;


/*! \brief
* Communication Interface Handle
*/
typedef void* rlComIfHdl_t;

/*! \brief
* OS Message Queue Object Handle
*/
typedef void* rlOsiMsgQHdl_t;

/*! \brief
* OS Semaphore Object Handle
*/
typedef void* rlOsiSemHdl_t;

/*! \brief
* OS Mutex Object Handle
*/
typedef void* rlOsiMutexHdl_t;

/*! \brief
* OS Time data type
*/
typedef rlUInt32_t rlOsiTime_t;


/******************************************************************************
 * MACRO DEFINITIONS
 *******************************************************************************
 */

/*! \brief
 * Standard MACROs
 */
#define RL_NULL                     (0U)
#define RL_NULL_PTR                 (NULL)
#define RL_INVALID                  (-1)
#define RL_TRUE                     (rlUInt8_t)(1U)
#define RL_FALSE                    (rlUInt8_t)(0U)
#define RL_LOGICAL_TRUE             (RL_TRUE != RL_FALSE)
#define RL_LOGICAL_FALSE            (RL_TRUE == RL_FALSE)

/*! \brief
 * mmWaveLink Constants for numberic values
 */
#define RL_ZERO                     (0U)
#define RL_ONE                      (1U)
#define RL_TWO                      (2U)
#define RL_THREE                    (3U)
#define RL_FOUR                     (4U)
#define RL_FIVE                     (5U)
#define RL_SIX                      (6U)
#define RL_SEVEN                    (7U)
#define RL_EIGHT                    (8U)
#define RL_NINE                     (9U)
#define RL_TEN                      (10U)
#define RL_ELEVEN                   (11U)
#define RL_TWELVE                   (12U)
#define RL_THIRTEEN                 (13U)
#define RL_FOURTEEN                 (14U)
#define RL_FIFTEEN                  (15U)
#define RL_SIXTEEN                  (16U)
#define RL_SEVENTEEN                (17U)
#define RL_EIGHTEEN                 (18U)
#define RL_NINETEEN                 (19U)
#define RL_TWENTY                   (20U)
#define RL_TWENTY_ONE               (21U)
#define RL_TWENTY_TWO               (22U)
#define RL_TWENTY_THREE             (23U)
#define RL_TWENTY_FOUR              (24U)
#define RL_TWENTY_FIVE              (25U)
#define RL_TWENTY_SIX               (26U)
#define RL_TWENTY_SEVEN             (27U)
#define RL_TWENTY_EIGHT             (28U)
#define RL_TWENTY_NINE              (29U)
#define RL_THIRTY                   (30U)
#define RL_THIRTY_ONE               (31U)
#define RL_THIRTY_TWO               (32U)

/*! \brief
 * mmWaveLink Constants for OS delay in milliseconds
 */
#define RL_OSI_WAIT_FOREVER         (0xFFFFU)
#define RL_OSI_WAIT_SECOND          (0x3E8U)
#define RL_OSI_NO_WAIT              (0U)

/*! \brief
 * MACRO for API error checks
 */
#define RL_RET_OK_CHECK(Func)   {\
                                    if(RL_RET_CODE_OK != (Func))\
                                    {\
                                        return  returnVal;\
                                    }\
                                    else\
                                    {\
                                        /* No Error */\
                                    }\
                                }

/*! \brief
 * MACRO for OS Interface error checks
 */
#define RL_RET_OSI_CHECK(Func)  {\
                                    if((rlInt32_t)RL_RET_CODE_OK != (rlInt32_t)(Func))\
                                    {\
                                        return (rlInt32_t)RL_RET_CODE_RADAR_OSIF_ERROR;\
                                    }\
                                    else\
                                    {\
                                        /* No Error */\
                                    }\
                                }

/*! \brief
 * MACRO for Communication Interface error checks
 */
#define RL_RET_IF_CHECK(Func)     {\
                                    if(RL_RET_CODE_OK != (Func))\
                                    {\
                                        return  RL_RET_CODE_RADAR_IF_ERROR;\
                                    }\
                                    else\
                                    {\
                                        /* No Error */\
                                    }\
                                }

/*! \brief
 * MACRO for Driver Assert with Line Number
 */
#define RL_DRIVER_ASSERT(line )  {\
                                    return RL_RET_CODE_FATAL_ERROR;\
                                 }

/*! \brief
 * Assert Macro
 */
#define RL_ASSERT(expr)            {\
                                        if(!(expr))\
                                        {\
                                            RL_DRIVER_ASSERT(__LINE__);\
                                        }\
                                        else\
                                        {\
                                            ;\
                                        }\
                                    }
/*! \brief
 * MACRO for error checks
 */
#define RL_ERROR(expr, error)      {\
                                        if(!(expr))\
                                        {\
                                            return (error);\
                                        }\
                                   }
/*! \brief
 * MACRO for protocol error checks
 */
#define RL_VERIFY_PROTOCOL(expr)   RL_ERROR((expr), (RL_RET_CODE_PROTOCOL_ERROR))


#ifdef __cplusplus
}
#endif


#endif

/*
* END OF RL_DATATYPES_H
*/

