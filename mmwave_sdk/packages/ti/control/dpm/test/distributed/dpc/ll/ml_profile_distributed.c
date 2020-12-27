/*
 *   @file  ml_profile.c
 *
 *   @brief
 *      Medium Level profile executing on the DSS
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
#include <ti/control/dpm/test/distributed/dpc/ll/ll_common_profile.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/**
 * @brief
 *  ML Profile MCB
 *
 * @details
 *  MCB which holds all the relevant information required to execute
 *  the profile
 */
typedef struct MLProfile_MCB_t
{
    /**
     * @brief   DPM Handle
     */
    DPM_Handle              dpmHandle;

    /**
     * @brief   ML DPC Handle:
     */
    DPM_DPCHandle           dpcHandle;

    /**
     * @brief   DPM Initialization configuration
     */
    DPM_InitCfg             dpmInitCfg;

    /**
     * @brief   SOC Handle
     */
    SOC_Handle              socHandle;

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
}MLProfile_MCB;

/**************************************************************************
 ************************** Local Functions *******************************
 **************************************************************************/

static DPM_DPCHandle MLProfile_init
(
    DPM_Handle      dpmHandle,
    DPM_InitCfg*    ptrInitCfg,
    int32_t*        errCode
);
static int32_t MLProfile_execute
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrResult
);
static int32_t MLProfile_ioctl
(
    DPM_DPCHandle   handle,
    uint32_t        cmd,
    void*           arg,
    uint32_t        argLen
);
static int32_t MLProfile_start  (DPM_DPCHandle handle);
static int32_t MLProfile_stop   (DPM_DPCHandle handle);
static int32_t MLProfile_deinit (DPM_DPCHandle handle);
static void    MLProfile_chirpAvailable (DPM_DPCHandle handle);
static void    MLProfile_frameStart (DPM_DPCHandle handle);
static int32_t MLProfile_injectData (DPM_DPCHandle  handle, DPM_Buffer* ptrBuffer);

/**************************************************************************
 ************************* Global Declarations ****************************
 **************************************************************************/

/** @addtogroup OOB_PROFILE_GLOBAL
 @{ */

/**
 * @brief   OOB Profile:
 */
DPM_ProcChainCfg gMLProfileCfg =
{
    MLProfile_init,            /* Initialization Function:         */
    MLProfile_start,           /* Start Function:                  */
    MLProfile_execute,         /* Execute Function:                */
    MLProfile_ioctl,           /* Configuration Function:          */
    MLProfile_stop,            /* Stop Function:                   */
    MLProfile_deinit,          /* Deinitialization Function:       */
    MLProfile_injectData,      /* Inject Data Function:            */
    MLProfile_chirpAvailable,  /* Chirp Available Function:        */
    MLProfile_frameStart       /* Frame Start Function:            */
};

/**
 * @brief   Global MCB for the ML Profile
 */
MLProfile_MCB   gMLProfileMCB;

/**
@}
*/

/**************************************************************************
 *********************** ML Profile Functions *****************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the ML Profile registered function which is invoked to receive
 *      data from the application.
 *
 *  @param[in]  handle
 *      Profile handle
 *  @param[in]  ptrBuffer
 *      Pointer to the buffers on which the profile will execute
 *
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MLProfile_injectData
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrBuffer
)
{
    uintptr_t   key;

    /* Sanity Checking: Ensure that the previous set of data is processed before
     * new data is injected. */
    DebugP_assert (gMLProfileMCB.dataAvailable == false);

    /* Critical Section: The Input buffers are protected from concurrent acccess */
    key = HwiP_disable();

    /* The HL Profile assumes and documents that the data buffer passed to the
     * function should be placed in persistent memory. We simply copy the DPM
     * Buffer but not the actual contents of the buffer. */
    memcpy ((void *)&gMLProfileMCB.dataInputBuffer, (void*)ptrBuffer, sizeof(DPM_Buffer));

    /* Set the flag to indicate that the data is available to be processed: */
    gMLProfileMCB.dataAvailable = true;

    /* End of Critical Section: */
    HwiP_restore(key);

    /* Notify the DPM Module that the ML DPC is ready for execution */
    DebugP_assert (DPM_notifyExecute (gMLProfileMCB.dpmHandle, handle, false) == 0);
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
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MLProfile_chirpAvailable (DPM_DPCHandle handle)
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
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void MLProfile_frameStart (DPM_DPCHandle handle)
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
 *  @param[out] errCode
 *      Error code populated by the API
 *
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t MLProfile_execute
(
    DPM_DPCHandle   handle,
    DPM_Buffer*     ptrResult
)
{
    uint32_t        translatedAddress;
    DPM_Buffer      dataInput;
    uintptr_t       key;
    int32_t         errCode = 0;
    int32_t         retVal  = 0;
    char            output[256];

    /* Data should always be available when the execute method is invoked */
    DebugP_assert (gMLProfileMCB.dataAvailable == true);

    /* Critical Section: Copy over the input DPM Buffer and reset the flag */
    key = HwiP_disable();
    memcpy ((void *)&dataInput, (void*)&gMLProfileMCB.dataInputBuffer, sizeof(DPM_Buffer));
    gMLProfileMCB.dataAvailable = false;
    HwiP_restore(key);

    /********************************************************************************
     * Process the received data:
     * - In this dummy profile we simply write to the output buffer
     * - First Translate it to a local address
     ********************************************************************************/
    translatedAddress = SOC_translateAddress((uint32_t)dataInput.ptrBuffer[0],
                                             SOC_TranslateAddr_Dir_FROM_OTHER_CPU,
                                             &errCode);
    if (translatedAddress == SOC_TRANSLATEADDR_INVALID)
    {
        /* Error: Invalid address passed from the processing chain */
        retVal = errCode;
        goto exit;
    }
    dataInput.ptrBuffer[0] = (uint8_t*)translatedAddress;

    /* Create the output buffer: */
    sprintf ((char*)&output, "ML Processing [Peak Grouping %d]",
             gMLProfileMCB.peakGroupingCfg.dummy1);

    /* Concat the output to the received buffer: */
    strcat ((char*)dataInput.ptrBuffer[0], (char*)&output[0]);

    /********************************************************************************
     * The ML has completed its work and the results can now be reported back to the
     * application
     ********************************************************************************/
    ptrResult->ptrBuffer[0] = dataInput.ptrBuffer[0];
    ptrResult->size[0]      = strlen ((char*)dataInput.ptrBuffer[0]);

exit:
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
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t MLProfile_start (DPM_DPCHandle handle)
{
    int32_t     retVal = 0;

    /* Sanity Checking: Ensure that the ML Profile has been configured and it is valid */
    if (gMLProfileMCB.peakGroupingCfg.dummy1 == 0)
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
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t MLProfile_stop (DPM_DPCHandle handle)
{
    /* Reset the configuration: */
    memset ((void*)&gMLProfileMCB.peakGroupingCfg, 0, sizeof(LL_PeakGroupingCfg));
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
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MLProfile_ioctl
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
        case LL_CMD_PEAK_GROUPING_CFG:
        {
            /* Sanity Checking: Validate the length */
            DebugP_assert (argLen == sizeof(LL_PeakGroupingCfg));

            /* Copy over the Peak Grouping configuration: */
            memcpy ((void*)&gMLProfileMCB.peakGroupingCfg, (void*)arg, sizeof(LL_PeakGroupingCfg));
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
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static DPM_DPCHandle MLProfile_init
(
    DPM_Handle      dpmHandle,
    DPM_InitCfg*    ptrInitCfg,
    int32_t*        errCode
)
{
    DPM_DPCHandle   retHandle;
    uint32_t        dpcArg;

    /**************************************************************
     * Sanity Testing: Validate the processing chain arguments.
     **************************************************************/
    DebugP_assert (ptrInitCfg->argSize == 4U);
    dpcArg = *(uint32_t*)ptrInitCfg->arg;
    DebugP_assert (dpcArg == ML_INIT_ARGUMENT);

    /* Setup the return handle: */
    retHandle = (DPM_DPCHandle)&gMLProfileMCB;

    /* Initialize the allocated memory: */
    memset ((void *)&gMLProfileMCB, 0, sizeof(MLProfile_MCB));

    /* Copy over the DPM configuration: */
    memcpy ((void*)&gMLProfileMCB.dpmInitCfg, (void*)ptrInitCfg, sizeof(DPM_InitCfg));

    /* Populate the ML Profile: */
    gMLProfileMCB.dpmHandle = dpmHandle;
    gMLProfileMCB.socHandle = ptrInitCfg->socHandle;
    gMLProfileMCB.dpcHandle = retHandle;
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
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup ML_PROFILE_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t MLProfile_deinit (DPM_DPCHandle handle)
{
    return -1;
}

