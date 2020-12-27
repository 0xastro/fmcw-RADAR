/**
 *   @file  hl_profile.h
 *
 *   @brief
 *      DPM HL Profile Header File
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
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHAHL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef HL_PROFILE_H
#define HL_PROFILE_H

/* MMWAVE Driver Include Files */
#include <ti/common/mmwave_error.h>
#include <ti/drivers/mailbox/mailbox.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/cbuff/cbuff.h>
#include <ti/control/dpm/dpm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
@defgroup HL_PROFILE_GLOBAL                        HL Profile Global
@ingroup HL_PROFILE
@brief
*   This section has a list of all the globals exposed by the profile.
*/
/**
@defgroup HL_PROFILE_DATA_STRUCTURES               HL Profile Data Structures
@ingroup HL_PROFILE
@brief
*   This section has a list of all the data structures which are a part of the profile module
*   and which are exposed to the application
*/
/**
@defgroup HL_PROFILE_DEFINITIONS                   HL Profile Definitions
@ingroup HL_PROFILE
@brief
*   This section has a list of all the definitions which are used by the HL Profile.
*/
/**
@defgroup HL_PROFILE_INTERNAL_DATA_STRUCTURES      HL Profile Internal Data Structures
@ingroup HL_PROFILE
@brief
*   This section has a list of all the internal data structures which are a part of the profile module.
*/
/**
@defgroup HL_PROFILE_INTERNAL_FUNCTION             HL Profile Internal Functions
@ingroup HL_PROFILE
@brief
*   This section has a list of all the internal function which are a part of the profile module.
*   These are not exposed to the application and are completely abstracted by the framework.
*/
/**
@defgroup HL_PROFILE_COMMAND                       HL Profile Commands
@ingroup HL_PROFILE
@brief
*   This section has a list of all the commands which are supported by the HL Profile
*/

/** @addtogroup HL_PROFILE_DEFINITIONS
 @{ */

/**
 * @brief   Unique DPM Instance Identifier for the HL Data Path Profile
 */
#define HL_DPM_INSTANCE_ID                3333

/**
 * @brief   Initial argument passed during HL processing chain initialization
 */
#define HL_INIT_ARGUMENT                  0xCCCCCCCC

/**
@}
*/

/** @addtogroup HL_PROFILE_DATA_STRUCTURES
 @{ */

/**
 * @brief
 *  ML Data Path Profile Peak Grouping Configuration
 *
 * @details
 *  This is a template structure holding the Peak Grouping configuration
 */
typedef struct HL_GTrackCfg_t
{
    /**
     * @brief   Dummy Parameter1:
     */
    uint32_t    dummy1;

    /**
     * @brief   Dummy Parameter2:
     */
    uint32_t    dummy2;

    /**
     * @brief   Dummy Parameter3:
     */
    uint32_t    dummy3;
}HL_GTrackCfg;

/**
@}
*/

/** @addtogroup HL_PROFILE_COMMAND
 @{ */

/**
 * @brief
 *  The LL Profile uses this to configure the GTrack configuration
 *      arg    = &fftcCfg
 *      argLen = sizeof(HL_GTrackCfg)
 */
#define HL_CMD_GTRACK_CFG               (DPM_CMD_DPC_START_INDEX + 1U)

/**
@}
*/

/* Application developers: Use this profile configuration to load the profile within the DPM Module. */
extern DPM_ProcChainCfg   gHLProfileCfg;

#endif /* HL_PROFILE_H */

