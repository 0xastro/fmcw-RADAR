/****************************************************************************************
 * FileName     : rl_controller.h
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

/****************************************************************************************
* FILE INCLUSION PROTECTION
****************************************************************************************
*/
#ifndef RL_CONTROLLER_H
#define RL_CONTROLLER_H


/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <ti/control/mmwavelink/mmwavelink.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */

/* Radar Configuration Functions */
rlReturnVal_t rlAppendSubBlock(rlUInt8_t rhcpPayload[], rlUInt16_t sblkId, rlUInt16_t sbLen,
                               rlUInt8_t* sbData);
rlReturnVal_t rlAppendDummy(rlUInt8_t rhcpPayload[], rlUInt8_t dummyLen);
rlReturnVal_t rlGetSubBlock(rlUInt8_t rhcpPayload[], rlUInt16_t* sbcId, rlUInt16_t* sbLen,
                            rlUInt8_t* sbData);
void rlGetSubBlockId(const rlUInt8_t rhcpPayload[], rlUInt16_t* sbcId);
void rlGetSubBlockLen(const rlUInt8_t rhcpPayload[], rlUInt16_t* sbcLen);


#ifdef __cplusplus
}
#endif

#endif

/*
 * END OF RL_CONTROLLER_H FILE
 */
