/**
 *   @file  csi_stream.h
 *
 *   @brief
 *      The exported header file for the CSI Stream Profile.
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
#ifndef CSI_STREAM_H
#define CSI_STREAM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CSI_STREAM_PROFILE      CSI Stream Profile
 *
 * ## Introduction #
 *
 * The Profile can be used to stream the ADC Data over the CSI and
 * will also keep a copy of first "N" samples into the memory so that
 * it is possible to compare the received & streamed data. This is
 * only for debugging purposes.
 *
 * ## Loading Profile #
 * The Profile can be loaded into the Test Framework by using the
 * following profile configuration: "gCSIStreamProfileCfg".
 *
 * ## Configurability #
 *  - Supports only XWR14xx
 *  - CSI Data Rate: @450Mhz
 *  - Does *NOT* use the mmWave control module
 *  - The following command is supported by the profile:-
 *      CSI_STREAM_PROFILE_CMD_NUM_CHIRPS_PER_FRAME
 *  - Uses hardcoded EDMA channel identifiers.
 *  - Uses EDMA channels to DMA the data from the ADC buffer
 *    into the gDataCube memory. The gDataCube memory is global and the
 *    profile defines this global data buffer to be placed into the
 *    ".dataCubeMemory" memory section.
 *
 * ## Execution #
 * The profile executes every chirp and if there is space will DMA the
 * data into the global buffer as long as there is space in the buffer.
 *
 * ## Output #
 *  The CSI Stream profile does not generate any results.
 */
/**
@defgroup CSI_STREAM_PROFILE_GLOBAL                        CSI Stream Profile Global
@ingroup CSI_STREAM_PROFILE
@brief
*   This section has a list of all the globals exposed by the profile.
*/
/**
@defgroup CSI_STREAM_PROFILE_INTERNAL_DATA_STRUCTURES      CSI Stream Profile Internal Data Structures
@ingroup CSI_STREAM_PROFILE
@brief
*   This section has a list of all the internal data structures which are a part of the profile module.
*/
/**
@defgroup CSI_STREAM_PROFILE_INTERNAL_FUNCTION             CSI Stream Profile Internal Functions
@ingroup CSI_STREAM_PROFILE
@brief
*   This section has a list of all the internal function which are a part of the profile module.
*   These are not exposed to the application and are completely abstracted by the framework.
*/
/**
@defgroup CSI_STREAM_PROFILE_COMMAND                        CSI Stream Profile Commands
@ingroup CSI_STREAM_PROFILE
@brief
*   This section has a list of all the commands which are supported by the CSI Stream Profile
*/

/** @addtogroup CSI_STREAM_PROFILE_COMMAND
 @{ */

/**
 * @brief
 *  This is the CSI Stream Profile command to configure the number of chirps per frame.
 *      arg    = &numChirps
 *      argLen = sizeof (uint32_t)
 */
#define CSI_STREAM_PROFILE_SET_NUM_CHIRPS_PER_FRAME         (TEST_FMK_CMD_PROFILE_START_INDEX + 0U)

/**
@}
*/

/* Application developers: Use this profile configuration to load the profile within the framework. */
extern TestFmk_ProfileCfg       gCSIStreamProfileCfg;

#ifdef __cplusplus
}
#endif

#endif /* CSI_STREAM_H */
