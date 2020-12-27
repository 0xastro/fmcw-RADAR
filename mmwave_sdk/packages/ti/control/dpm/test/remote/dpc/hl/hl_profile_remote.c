/*
 *   @file  hl_profile.c
 *
 *   @brief
 *      HL profile
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the fohlowing conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the fohlowing disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the fohlowing disclaimer in the
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
#include <ti/control/dpm/test/remote/dpc/hl/hl_profile_remote.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/**
 * @brief
 *  HL Profile MCB
 *
 * @details
 *  MCB which holds ahl the relevant information required to execute
 *  the profile
 */
typedef struct HLProfile_MCB_t
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
     * @brief   GTRACK configuration
     */
    HL_GTrackCfg            gtrackCfg;

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
}HLProfile_MCB;

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/

static DPM_DPCHandle HLProfile_init
(
    DPM_Handle      dpmHandle,
    DPM_InitCfg*    ptrInitCfg,
    int32_t*        errCode
);
static int32_t HLProfile_execute
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrResult
);
static int32_t HLProfile_ioctl
(
    DPM_DPCHandle   handle,
    uint32_t        cmd,
    void*           arg,
    uint32_t        argLen
);
static int32_t HLProfile_start  (DPM_DPCHandle handle);
static int32_t HLProfile_stop   (DPM_DPCHandle handle);
static int32_t HLProfile_deinit (DPM_DPCHandle handle);
static int32_t HLProfile_injectData (DPM_DPCHandle  handle, DPM_Buffer* ptrBuffer);

/**************************************************************************
 ************************* Global Declarations ****************************
 **************************************************************************/

/** @addtogroup OOB_PROFILE_GLOBAL
 @{ */

/**
 * @brief   OOB Profile:
 */
DPM_ProcChainCfg gHLProfileCfg =
{
    HLProfile_init,            /* Initialization Function:         */
    HLProfile_start,           /* Start Function:                  */
    HLProfile_execute,         /* Execute Function:                */
    HLProfile_ioctl,           /* Configuration Function:          */
    HLProfile_stop,            /* Stop Function:                   */
    HLProfile_deinit,          /* Deinitialization Function:       */
    HLProfile_injectData,      /* Inject Data Function:            */
    NULL,                      /* Chirp Available Function:        */
    NULL                       /* Frame Start Function:            */
};

/**
 * @brief   Global MCB for the HL Profile
 */
HLProfile_MCB   gHLProfileMCB;

/**
@}
*/

/**************************************************************************
 *********************** HL Profile Functions *****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the HL Profile registered function which is invoked to receive
 *      data from the application.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[in]  ptrBuffer
 *      Pointer to the buffers on which the profile will execute
 *
 *  \ingroup HL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t HLProfile_injectData
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrBuffer
)
{
    uintptr_t   key;

    /* Sanity Checking: Ensure that the previous set of data is processed before
     * new data is injected. */
    DebugP_assert (gHLProfileMCB.dataAvailable == false);

    /* Critical Section: The Input buffers are protected from concurrent acccess */
    key = HwiP_disable();

    /* The LL Profile assumes and documents that the data buffer passed to the
     * function should be placed in persistent memory. We simply copy the DPM
     * Buffer but not the actual contents of the buffer. */
    memcpy ((void *)&gHLProfileMCB.dataInputBuffer, (void*)ptrBuffer, sizeof(DPM_Buffer));

    /* Set the flag to indicate that the data is available to be processed: */
    gHLProfileMCB.dataAvailable = true;

    /* End of Critical Section: */
    HwiP_restore(key);

    /* Notify the DPM Module that the HL DPC is ready for execution */
    DebugP_assert (DPM_notifyExecute (gHLProfileMCB.dpmHandle, handle, false) == 0);
    return 0;
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
 *  \ingroup HL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t HLProfile_execute
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrResult
)
{
    int32_t         retVal = 0;
    DPM_Buffer      dataInput;
    uintptr_t       key;
    char            output[256];

    /* Data should always be available when the execute method is invoked */
    DebugP_assert (gHLProfileMCB.dataAvailable == true);

    /* Critical Section: Copy over the input DPM Buffer and reset the flag */
    key = HwiP_disable();
    memcpy ((void *)&dataInput, (void*)&gHLProfileMCB.dataInputBuffer, sizeof(DPM_Buffer));
    gHLProfileMCB.dataAvailable = false;
    HwiP_restore(key);

    /* Create the output buffer: */
    sprintf ((char*)&output, "HL Processing [GTrack %d:%d:%d]",
             gHLProfileMCB.gtrackCfg.dummy1,
             gHLProfileMCB.gtrackCfg.dummy2,
             gHLProfileMCB.gtrackCfg.dummy3);

    /* Concat the output to the received buffer: */
    strcat ((char*)dataInput.ptrBuffer[0], (char*)&output[0]);

    /********************************************************************************
     * The HL has completed its work and the results can now be reported back to the
     * application
     ********************************************************************************/
    ptrResult->ptrBuffer[0] = dataInput.ptrBuffer[0];
    ptrResult->size[0]      = strlen ((char*)dataInput.ptrBuffer[0]);
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
 *  \ingroup HL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t HLProfile_start (DPM_DPCHandle handle)
{
    int32_t     retVal = 0;

    /* Sanity Checking: Ensure that the HL Profile has been configured and it is valid */
    if ((gHLProfileMCB.gtrackCfg.dummy1 == 0U) ||
        (gHLProfileMCB.gtrackCfg.dummy2 == 0U) ||
        (gHLProfileMCB.gtrackCfg.dummy3 == 0U))
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
 *  \ingroup HL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t HLProfile_stop (DPM_DPCHandle handle)
{
    /* Reset the configuration: */
    memset ((void *)&gHLProfileMCB.gtrackCfg, 0, sizeof(HL_GTrackCfg));
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
 *
 *  \ingroup HL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t HLProfile_ioctl
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
        case HL_CMD_GTRACK_CFG:
        {
            /* Sanity Checking: Validate the length */
            DebugP_assert (argLen == sizeof(HL_GTrackCfg));

            /* Copy over the GTRACK configuration: */
            memcpy ((void*)&gHLProfileMCB.gtrackCfg, (void*)arg, sizeof(HL_GTrackCfg));
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
 *  \ingroup HL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static DPM_DPCHandle HLProfile_init
(
    DPM_Handle          dpmHandle,
    DPM_InitCfg*        ptrInitCfg,
    int32_t*            errCode
)
{
    DPM_DPCHandle   retHandle = NULL;

    /* Setup the return handle: */
    retHandle = (DPM_DPCHandle)&gHLProfileMCB;

    /* Initialize the ahlocated memory: */
    memset ((void *)&gHLProfileMCB, 0, sizeof(HLProfile_MCB));

    /* Copy over the DPM configuration: */
    memcpy ((void*)&gHLProfileMCB.dpmInitCfg, (void*)ptrInitCfg, sizeof(DPM_InitCfg));

    /* Populate the OOB Profile: */
    gHLProfileMCB.dpmHandle  = dpmHandle;
    gHLProfileMCB.dpcHandle  = retHandle;
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
 *  \ingroup HL_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t HLProfile_deinit (DPM_DPCHandle handle)
{
    return 0;
}

