/**
 *   @file  ll_profile.h
 *
 *   @brief
 *      DPM LL Profile Header File
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
#ifndef LL_PROFILE_H
#define LL_PROFILE_H

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
@defgroup LL_PROFILE_GLOBAL                        LL Profile Global
@ingroup LL_PROFILE
@brief
*   This section has a list of all the globals exposed by the profile.
*/
/**
@defgroup LL_PROFILE_DATA_STRUCTURES               LL Profile Data Structures
@ingroup LL_PROFILE
@brief
*   This section has a list of all the data structures which are a part of the profile module
*   and which are exposed to the application
*/
/**
@defgroup LL_PROFILE_DEFINITIONS                   LL Profile Definitions
@ingroup LL_PROFILE
@brief
*   This section has a list of all the definitions which are used by the LL Profile.
*/
/**
@defgroup LL_PROFILE_INTERNAL_DATA_STRUCTURES      LL Profile Internal Data Structures
@ingroup LL_PROFILE
@brief
*   This section has a list of all the internal data structures which are a part of the profile module.
*/
/**
@defgroup LL_PROFILE_INTERNAL_FUNCTION             LL Profile Internal Functions
@ingroup LL_PROFILE
@brief
*   This section has a list of all the internal function which are a part of the profile module.
*   These are not exposed to the application and are completely abstracted by the framework.
*/
/**
@defgroup LL_PROFILE_COMMAND                       LL Profile Commands
@ingroup LL_PROFILE
@brief
*   This section has a list of all the commands which are supported by the LL Profile
*/

/** @addtogroup LL_PROFILE_DEFINITIONS
 @{ */

/**
 * @brief   Unique DPM Instance identifier for the LL Data Path Profile executing on the MSS
 */
#define LL_MSS_DPM_INSTANCE_ID                1111

/**
 * @brief   Unique DPM Instance identifier for the LL Data Path Profile executing on the DSS
 */
#define LL_DSS_DPM_INSTANCE_ID                2222

/**
@}
*/

/** @addtogroup LL_PROFILE_DATA_STRUCTURES
 @{ */

/**
 * @brief
 *  LL Data Path Profile FFTC configuration
 *
 * @details
 *  This is a template structure holding the FFTC configuration
 */
typedef struct LL_FFTCCfg_t
{
    /**
     * @brief   Dummy Parameter1:
     */
    uint32_t    dummy1;

    /**
     * @brief   Dummy Parameter2:
     */
    uint32_t    dummy2;
}LL_FFTCCfg;

/**
 * @brief
 *  ML Data Path Profile Peak Grouping Configuration
 *
 * @details
 *  This is a template structure holding the Peak Grouping configuration
 */
typedef struct LL_PeakGroupingCfg_t
{
    /**
     * @brief   Dummy Parameter1:
     */
    uint32_t    dummy1;
}LL_PeakGroupingCfg;

/**
@}
*/

/** @addtogroup LL_PROFILE_COMMAND
 @{ */

/**
 * @brief
 *  The LL Profile uses this to configure the FFTC
 *      arg    = &fftcCfg
 *      argLen = sizeof(LL_FFTCCfg)
 */
#define LL_CMD_FFTC_CFG                         (DPM_CMD_DPC_START_INDEX + 1U)

/**
 * @brief
 *  The LL Profile uses this to configure the Peak Grouping configuration
 *      arg    = &peakGroupingCfg
 *      argLen = sizeof(LL_PeakGroupingCfg)
 */
#define LL_CMD_PEAK_GROUPING_CFG                (DPM_CMD_DPC_START_INDEX + 2U)

/**
@}
*/

/* Application developers: Use this profile configuration to load the profile within the DPM Module. */
extern DPM_ProcChainCfg   gLLProfileCfg;

#endif /* LL_PROFILE_H */

