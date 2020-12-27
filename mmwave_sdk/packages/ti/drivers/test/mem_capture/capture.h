/**
 *   @file  capture.h
 *
 *   @brief
 *      The exported header file for the capture profile.
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
#ifndef CAPTURE_H
#define CAPTURE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CAPTURE_PROFILE      Capture Profile
 *
 * The Profile can be used to capture ADC samples into memory.
 *
 * Loading Profile:
 * ----------------
 * - The Profile can be loaded into the Test Framework by using the
 *   following profile configuration: "gCaptureProfileCfg"
 *
 * Configurability:
 * ----------------
 *  - Supports XWR14xx/XWR16xx/XWR18xx/XWR68xx
 *  - Does *NOT* use the mmWave control module
 *  - Does *NOT* expose any additional commands.
 *  - Uses hardcoded EDMA channel identifiers.
 *  - Uses EDMA channels to DMA the data from the ADC buffer
 *    into the gDataCube memory. The gDataCube memory is global and the
 *    profile defines this global data buffer to be placed into the
 *    ".dataCubeMemory" memory section.
 *
 * Execution:
 * ----------
 * - The profile executes every chirp and if there is space will DMA the
 *   data into the global buffer as long as there is space in the buffer.
 */
/**
@defgroup CAPTURE_PROFILE_GLOBAL                        Capture Global
@ingroup CAPTURE_PROFILE
@brief
*   This section has a list of all the globals exposed by the profile.
*/
/**
@defgroup CAPTURE_PROFILE_INTERNAL_DATA_STRUCTURES      Capture Profile Internal Data Structures
@ingroup CAPTURE_PROFILE
@brief
*   This section has a list of all the internal data structures which are a part of the profile module.
*/
/**
@defgroup CAPTURE_PROFILE_INTERNAL_FUNCTION             Capture Profile Internal Functions
@ingroup CAPTURE_PROFILE
@brief
*   This section has a list of all the internal function which are a part of the profile module.
*   These are not exposed to the application and are completely abstracted by the framework.
*/

/* Application developers: Use this profile configuration to load the profile within the framework. */
extern TestFmk_ProfileCfg       gCaptureProfileCfg;

#ifdef __cplusplus
}
#endif

#endif /* CAPTURE_H */
