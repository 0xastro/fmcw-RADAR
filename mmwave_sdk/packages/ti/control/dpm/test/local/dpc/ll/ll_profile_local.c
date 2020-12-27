/*
 *   @file  ll_profile.c
 *
 *   @brief
 *      LL Profile
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* mmWave SDK Include Files: */
#include <ti/common/mmwave_sdk_version.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/test/local/dpc/ll/ll_profile_local.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/**
 * @brief
 *  LL Profile MCB
 *
 * @details
 *  MCB which holds all the relevant information required to execute
 *  the profile
 */
typedef struct LLProfile_MCB_t
{
    /**
     * @brief   DPM Handle
     */
    DPM_Handle              dpmHandle;

    /**
     * @brief   HL DPC Handle:
     */
    DPM_DPCHandle           dpcHandle;

    /**
     * @brief   DPM Initialization configuration
     */
    DPM_InitCfg             dpmInitCfg;

    /**
     * @brief   FFTC Configuration:
     */
    LL_FFTCCfg              fftcCfg;

    /**
     * @brief   Peak Grouping Configuration:
     */
    LL_PeakGroupingCfg      peakGroupingCfg;

    /**
     * @brief   Flag which indicates that the data has been injected and can be processed
     * by the data path profile.
     */
    bool                    dataAvailable;

    /**
     * @brief   Data Input Buffer which is valid only if data is available
     */
    DPM_Buffer              dataInputBuffer;

    /**
     * @brief   Pointer to the data memory: This is the memory where the LL Profile Output
     * will be placed.
     */
    uint8_t*                ptrLLProfileOutput;
}LLProfile_MCB;

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/

static DPM_DPCHandle LLProfile_init
(
    DPM_Handle      dpmHandle,
    DPM_InitCfg*    ptrInitCfg,
    int32_t*        errCode
);
static int32_t LLProfile_execute
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrResult
);
static int32_t LLProfile_ioctl
(
    DPM_DPCHandle   handle,
    uint32_t        cmd,
    void*           arg,
    uint32_t        argLen
);
static int32_t LLProfile_start  (DPM_DPCHandle handle);
static int32_t LLProfile_stop   (DPM_DPCHandle handle);
static int32_t LLProfile_deinit (DPM_DPCHandle handle);
static void    LLProfile_chirpAvailable (DPM_DPCHandle handle);
static void    LLProfile_frameStart (DPM_DPCHandle handle);
static int32_t LLProfile_injectData (DPM_DPCHandle handle, DPM_Buffer* ptrBuffer);

/**************************************************************************
 ************************* Global Declarations ****************************
 **************************************************************************/

/** @addtogroup OOB_PROFILE_GLOBAL
 @{ */

/**
 * @brief   Global Data Memory which is needed by the LL Profile. This memory
 * needs to be placed into L3 memory. This will have shared memory contents.
 */
#pragma DATA_SECTION(gLLOutput, "llProfileData");
uint8_t gLLOutput[1024];

/**
 * @brief   LL Profile:
 */
DPM_ProcChainCfg gLLProfileCfg =
{
    LLProfile_init,            /* Initialization Function:         */
    LLProfile_start,           /* Start Function:                  */
    LLProfile_execute,         /* Execute Function:                */
    LLProfile_ioctl,           /* Configuration Function:          */
    LLProfile_stop,            /* Stop Function:                   */
    LLProfile_deinit,          /* Deinitialization Function:       */
    LLProfile_injectData,      /* Inject Data Function:            */
    LLProfile_chirpAvailable,  /* Chirp Available Function:        */
    LLProfile_frameStart       /* Frame Start Function:            */
};

/**
 * @brief   Global MCB for the LL Profile
 */
LLProfile_MCB   gLLProfileMCB;

/**
@}
*/

/**************************************************************************
 *********************** LL Profile Functions *****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the LL Profile registered function which is invoked to receive
 *      data from the application.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[in]  ptrBuffer
 *      Pointer to the buffers on which the profile will execute
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t LLProfile_injectData
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrBuffer
)
{
    uintptr_t   key;

    /* Sanity Checking: Ensure that the previous set of data is processed before
     * new data is injected. */
    DebugP_assert (gLLProfileMCB.dataAvailable == false);

    /* Critical Section: The Input buffers are protected from concurrent acccess */
    key = HwiP_disable();
    memcpy ((void *)&gLLProfileMCB.dataInputBuffer, (void*)ptrBuffer, sizeof(DPM_Buffer));
    gLLProfileMCB.dataAvailable = true;
    HwiP_restore(key);

    /* Notify the DPM Module that the LL DPC is ready for execution */
    DebugP_assert (DPM_notifyExecute (gLLProfileMCB.dpmHandle, handle, false) == 0);
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Profile Chirp Available Function which is invoked by the framework
 *      on the reception of the chirp available ISR
 *
 *  @param[in]  handle
 *      Profile handle
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void LLProfile_chirpAvailable (DPM_DPCHandle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      Profile Frame Start Function which is invoked by the framework
 *      on the reception of the frame start ISR
 *
 *  @param[in]  handle
 *      Profile handle
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void LLProfile_frameStart (DPM_DPCHandle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      Profile Execute Function which is invoked by the framework
 *      to execute the profile
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[out]  ptrResult
 *      Pointer to the result
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t LLProfile_execute
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrResult
)
{
    int32_t         retVal = 0;
    DPM_Buffer      dataInput;
    uintptr_t       key;

    /* Data should always be available when the execute method is invoked */
    DebugP_assert (gLLProfileMCB.dataAvailable == true);

    /* Critical Section: Copy over the input DPM Buffer and reset the flag */
    key = HwiP_disable();
    memcpy ((void *)&dataInput, (void*)&gLLProfileMCB.dataInputBuffer, sizeof(DPM_Buffer));
    gLLProfileMCB.dataAvailable = false;
    HwiP_restore(key);

    /********************************************************************************
     * Process the received data:
     * - In this dummy profile we simply write to the output buffer
     ********************************************************************************/
    sprintf ((char*)gLLProfileMCB.ptrLLProfileOutput, "[%s]LL Processing [FFTC %d:%d] [Peak Grouping: %d]",
             (char*)dataInput.ptrBuffer[0],
             gLLProfileMCB.fftcCfg.dummy1,
             gLLProfileMCB.fftcCfg.dummy2,
             gLLProfileMCB.peakGroupingCfg.dummy1);

    /********************************************************************************
     * The LL has completed its work. Populate the results
     ********************************************************************************/
    ptrResult->ptrBuffer[0] = gLLProfileMCB.ptrLLProfileOutput;
    ptrResult->size[0]      = strlen ((char*)gLLProfileMCB.ptrLLProfileOutput);

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Profile Start Function which is invoked by the framework
 *      during the start process.
 *
 *  @param[in]  handle
 *      Handle to the Profile
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t LLProfile_start (DPM_DPCHandle handle)
{
    int32_t     retVal = 0;

    /* Sanity Checking: Ensure that the LL Profile has been configured and it is valid */
    if ((gLLProfileMCB.fftcCfg.dummy1 == 0U) || (gLLProfileMCB.fftcCfg.dummy2 == 0U))
    {
        retVal = DPM_EPROFCFG;
        goto exit;
    }

    /* Sanity Checking: Ensure that the LL Profile has been configured and it is valid */
    if (gLLProfileMCB.peakGroupingCfg.dummy1 == 0U)
    {
        retVal = DPM_EPROFCFG;
        goto exit;
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Profile Stop Function which is invoked by the framework
 *      during the stop process.
 *
 *  @param[in]  handle
 *      Handle to the Profile
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t LLProfile_stop (DPM_DPCHandle handle)
{
    /* Reset the configuration: */
    memset ((void *)&gLLProfileMCB.fftcCfg, 0, sizeof(LL_FFTCCfg));
    memset ((void *)&gLLProfileMCB.peakGroupingCfg, 0, sizeof(LL_PeakGroupingCfg));

    /* Reset the data flags: There is no more data available */
    gLLProfileMCB.dataAvailable = false;
    memset ((void *)&gLLProfileMCB.dataInputBuffer, 0, sizeof(DPM_Buffer));
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Profile registered function which provides profile specific
 *      configurability.
 *
 *  @param[in]  handle
 *      Handle to the Module
 *  @param[in]  cmd
 *      Capture Profile specific commands
 *  @param[in]  arg
 *      Command specific arguments
 *  @param[in]  argLen
 *      Length of the arguments which is also command specific
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t LLProfile_ioctl
(
    DPM_DPCHandle   handle,
    uint32_t        cmd,
    void*           arg,
    uint32_t        argLen
)
{
    int32_t     retVal = 0;

    /* Process the command: */
    switch (cmd)
    {
        case LL_CMD_FFTC_CFG:
        {
            /* Sanity Checking: Validate the length */
            DebugP_assert (argLen == sizeof(LL_FFTCCfg));

            /* Copy over the FFTC configuration: */
            memcpy ((void*)&gLLProfileMCB.fftcCfg, (void*)arg, sizeof(LL_FFTCCfg));
            break;
        }
        case LL_CMD_PEAK_GROUPING_CFG:
        {
            /* Sanity Checking: Validate the length */
            DebugP_assert (argLen == sizeof(LL_PeakGroupingCfg));

            /* Copy over the Peak Grouping configuration: */
            memcpy ((void*)&gLLProfileMCB.peakGroupingCfg, (void*)arg, sizeof(LL_PeakGroupingCfg));
            break;
        }
        default:
        {
            /* The command is not supported. Set the error code as documented */
            retVal = DPM_EINVCMD;
            break;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Profile Initialization Function which is invoked by the framework
 *      during the Initialization process.
 *
 *  @param[in]  dpmHandle
 *      Handle to the Test Framework Module
 *  @param[in]  ptrInitCfg
 *      Pointer to the initialization configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static DPM_DPCHandle LLProfile_init
(
    DPM_Handle          dpmHandle,
    DPM_InitCfg*        ptrInitCfg,
    int32_t*            errCode
)
{
    DPM_DPCHandle   retHandle = NULL;

    /**************************************************************
     * Sanity Testing: Validate the processing chain arguments.
     **************************************************************/
    DebugP_assert (ptrInitCfg->argSize == 0U);
    DebugP_assert (ptrInitCfg->arg     == NULL);

    /* Setup the return handle: */
    retHandle = (DPM_DPCHandle)&gLLProfileMCB;

    /* Initialize the allocated memory: */
    memset ((void *)&gLLProfileMCB, 0, sizeof(LLProfile_MCB));

    /* Copy over the DPM configuration: */
    memcpy ((void*)&gLLProfileMCB.dpmInitCfg, (void*)ptrInitCfg, sizeof(DPM_InitCfg));

    /* Populate the LL Profile: */
    gLLProfileMCB.dpmHandle          = dpmHandle;
    gLLProfileMCB.dpcHandle          = retHandle;
    gLLProfileMCB.dataAvailable      = false;
    gLLProfileMCB.ptrLLProfileOutput = &gLLOutput[0];
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      Profile Deinitialization Function which is invoked by the framework
 *      during the Initialization process.
 *
 *  @param[in]  handle
 *      Handle to the profile
 *
 *  \ingroup LL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t LLProfile_deinit (DPM_DPCHandle handle)
{
    return 0;
}

