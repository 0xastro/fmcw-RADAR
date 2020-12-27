/**
 *   @file  mmwave.c
 *
 *   @brief
 *      The file implements the mmWave control module
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <stdint.h>
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/mmwave/include/mmwave_internal.h>

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/

/**
 * @brief   This the global variable which track all the information which
 * is required by the mmWave control module.
 */
MMWave_MCB   gMMWave_MCB;

/**************************************************************************
 ************************* Control Functions ******************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is used to encode the error code. This encoded error
 *      will be returned back to the application.
 *
 *  @param[in]  errorLevel
 *      Error level to be encoded
 *  @param[in]  mmWaveError
 *      Error code for the mmWave
 *  @param[in]  subSysError
 *      Subsystem error code
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Encoded error code
 */
int32_t MMWave_encodeError
(
    MMWave_ErrorLevel   errorLevel,
    int32_t             mmWaveError,
    int32_t             subSysError
)
{
    int32_t     encodedErrorCode;
    uint32_t    tmpEncodedErrorCode;

    /* Determine the error level */
    if (errorLevel == MMWave_ErrorLevel_SUCCESS)
    {
        /* No error: */
        encodedErrorCode = 0;
    }
    else
    {
        /* Warning or Error: */
        tmpEncodedErrorCode = CSL_FMKR (31U, 16U, (uint32_t)mmWaveError);
        tmpEncodedErrorCode = tmpEncodedErrorCode | CSL_FMKR (15U, 2U,  (uint32_t)subSysError);
        tmpEncodedErrorCode = tmpEncodedErrorCode | CSL_FMKR (1U,  0U,  (uint32_t)errorLevel);

        /* Convert the error code into an integer. */
        encodedErrorCode = (int32_t)tmpEncodedErrorCode;
    }
    return encodedErrorCode;
}

/**
 *  @b Description
 *  @n
 *      Application can use the API to decode the error. The mmWave API
 *      can return errors because of either the mmWave or the underlying
 *      mmWave Link API.
 *
 *  @param[in]  errCode
 *      Encoded error code retreived from the mmWave API. This is the error
 *      code which will be decoded.
 *  @param[out] errorLevel
 *      Error level populated by the API.
 *  @param[out] mmWaveError
 *      Error code from the mmWave module. This could be set to 0
 *      to indicate that the mmWave module did not return an error.
 *  @param[out] subSysError
 *      Subsystem error code. This could be set to 0 to indicate that there
 *      was no error at the subsystem level.
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void MMWave_decodeError
(
    int32_t             errCode,
    MMWave_ErrorLevel*  errorLevel,
    int16_t*            mmWaveError,
    int16_t*            subSysError
)
{
    uint16_t    tmpSubSysError;

    /* Get the error level: */
    *errorLevel = (MMWave_ErrorLevel)CSL_FEXTR ((uint32_t)errCode, 1U, 0U);

    /* Determine the error level */
    if (*errorLevel == MMWave_ErrorLevel_SUCCESS)
    {
        /* No error: */
        *mmWaveError = 0;
        *subSysError = 0;
    }
    else
    {
        /* Warning or Error: */
        *mmWaveError = (int16_t)CSL_FEXTR ((uint32_t)errCode, 31U, 16U);

        /* Get the subsystem error: Handle the sign extension correctly. */
        tmpSubSysError = CSL_FEXTR ((uint32_t)errCode, 15U, 2U) << 2;
        tmpSubSysError = tmpSubSysError >> 2;
        *subSysError   = (int16_t)tmpSubSysError;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      This is a utility function which given the error code will return
 *      the corresponding error level.
 *
 *  @param[in]  errCode
 *      Encoded error code retreived from the mmWave API. This is the error
 *      code which will be decoded.
 *
 *  \ingroup MMWAVE_CTRL_INTERNAL_FUNCTION
 *
 *  @retval
 *      Decoded Error Level
 */
MMWave_ErrorLevel MMWave_decodeErrorLevel (int32_t errCode)
{
    return (MMWave_ErrorLevel)CSL_FEXTR ((uint32_t)errCode, 1U, 0U);
}

/**
 *  @b Description
 *  @n
 *      The function is used to synchronize the execution of the control
 *      module between the execution domains. Applications are recommended
 *      to invoke the function until the "sync" is acheived. Failure to
 *      do so will result in the mmWave module misbehaving.
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave control module
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
int32_t MMWave_sync (MMWave_Handle mmWaveHandle, int32_t* errCode)
{
    MMWave_MCB* ptrMMWaveMCB;
    int32_t     retVal = MINUS_ONE;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the control module */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;
    if (ptrMMWaveMCB == NULL)
    {
        /* Error: Invalid argument. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* SOC Specific Synchronizaton: */
    retVal = MMWave_deviceSyncFxn (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: SOC Synchronization failed; error code is already setup. */
        goto exit;
    }

    /* Did we synchronize? */
    if (retVal == 1)
    {
        /* YES: Set the synchronization flag appropriately */
        ptrMMWaveMCB->status = ptrMMWaveMCB->status | (MMWAVE_STATUS_SYNCHRONIZED);
    }
    else
    {
        /* NO: Still not synchronized */
        ptrMMWaveMCB->status = ptrMMWaveMCB->status & (~(uint32_t)MMWAVE_STATUS_SYNCHRONIZED);
    }

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Return value has already been setup. Fall through */
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to execute the mmWave control module and
 *      this should only be called in the context of an executing task
 *      The function blocks and process mmWave link messages.
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave control module
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_execute (MMWave_Handle mmWaveHandle, int32_t* errCode)
{
    MMWave_MCB*     ptrMMWaveMCB;
    int32_t         retVal;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the control module */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;
    if (ptrMMWaveMCB == NULL)
    {
        /* Error: Invalid argument. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Enforce the proper calling order of the API. We need the mmWave to be
     * synchronized before we proceed. This is very important else we cannot
     * guarantee the passing of messages between the mmWave modules */
    if ((ptrMMWaveMCB->status & MMWAVE_STATUS_SYNCHRONIZED) == 0U)
    {
        /* Error: Invalid use case: Synchronization is required */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Pending on the semaphore: Waiting for events to be received */
    SemaphoreP_pend (ptrMMWaveMCB->linkSemHandle, SemaphoreP_WAIT_FOREVER);

    /* Execute and process all the link messages: */
    retVal = MMWave_executeLink (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to execute the link; error code is already setup */
        goto exit;
    }

    /* Successfully execute the link: Perform any SOC processing */
    retVal = MMWave_deviceExecuteFxn (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to perform SOC processing; error code is already setup */
        goto exit;
    }

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        retVal = 0;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the exported API which is used to configure the mmWave.
 *      This is only applicable in the full configuration mode.
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave control module
 *  @param[in]  ptrControlCfg
 *      Pointer to the control configuration
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @pre
 *      MMWave_init
 *  @pre
 *      MMWave_sync
 *  @pre
 *      MMWave_open
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_config (MMWave_Handle mmWaveHandle, MMWave_CtrlCfg* ptrControlCfg, int32_t* errCode)
{
    MMWave_MCB* ptrMMWaveMCB;
    int32_t     retVal;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the control module */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;
    if ((ptrMMWaveMCB == NULL) || (ptrControlCfg == NULL))
    {
        /* Error: Invalid argument. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /****************************************************************************************
     * Sanity Check:
     *  - Validate the prerequisites
     ****************************************************************************************/
    if (ptrMMWaveMCB->initCfg.cfgMode == MMWave_ConfigurationMode_FULL)
    {
        /* Full Configuration Mode: Ensure that the application has opened the mmWave module. */
        if (((ptrMMWaveMCB->status & MMWAVE_STATUS_SYNCHRONIZED) == 0U) ||
            ((ptrMMWaveMCB->status & MMWAVE_STATUS_OPENED)       == 0U))
        {
            /* Error: Invalid usage the module should be synchronized before it can be started. */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
            goto exit;
        }

        /* Initialize the DFE Output mode: */
        ptrMMWaveMCB->dfeDataOutputMode = ptrControlCfg->dfeDataOutputMode;
    }
    else
    {
        /* Minimal Configuration Mode: Application is responsible for the configuration of the BSS using
         * the mmWave Link API. This is av invalid use case */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Configure the link: */
    if (MMWave_configLink (ptrMMWaveMCB, ptrControlCfg, errCode) < 0)
    {
        /* Error: Unable to configure the link; error code is already setup. */
        goto exit;
    }

    /* SOC specific configuration: */
    retVal = MMWave_deviceCfgFxn (ptrMMWaveMCB, ptrControlCfg, errCode);
    if (retVal < 0)
    {
        /* Error: SOC configuration failed; error code is already setup */
        goto exit;
    }

    /* The module has been configured successfully: */
    ptrMMWaveMCB->status = ptrMMWaveMCB->status | MMWAVE_STATUS_CONFIGURED;

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        retVal = 0;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to start the mmWave control module after the
 *      configuration has been applied.
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave control module
 *  @param[in]  ptrCalibrationCfg
 *      Pointer to the calibration configuration
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @pre
 *      MMWave_init
 *  @pre
 *      MMWave_sync
 *  @pre
 *      MMWave_open
 *  @pre
 *      MMWave_config (Only in full configuration mode)
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_start (MMWave_Handle mmWaveHandle, const MMWave_CalibrationCfg* ptrCalibrationCfg, int32_t* errCode)
{
    MMWave_MCB*     ptrMMWaveMCB;
    int32_t         retVal =1;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the control module */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;
    if ((ptrMMWaveMCB == NULL) || (ptrCalibrationCfg == NULL))
    {
        /* Error: Invalid argument. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /****************************************************************************************
     * Sanity Check:
     *  - Validate the prerequisites
     ****************************************************************************************/
    if (ptrMMWaveMCB->initCfg.cfgMode == MMWave_ConfigurationMode_FULL)
    {
        /* Full Configuration Mode: Ensure that the application has configured the mmWave module
         * Only then can we start the module. */
        if (((ptrMMWaveMCB->status & MMWAVE_STATUS_SYNCHRONIZED) == 0U)    ||
            ((ptrMMWaveMCB->status & MMWAVE_STATUS_OPENED)       == 0U)    ||
            ((ptrMMWaveMCB->status & MMWAVE_STATUS_CONFIGURED)   == 0U))
        {
            /* Error: Invalid usage the module should be synchronized before it can be started. */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
            goto exit;
        }

        /* Sanity Check: Validate the DFE output mode. This should always match in the FULL configuration mode. */
        if (ptrMMWaveMCB->dfeDataOutputMode != ptrCalibrationCfg->dfeDataOutputMode)
        {
            /* Error: Invalid argument. */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
            goto exit;
        }
    }
    else
    {
        /* Minimal Configuration Mode: Application should have opened and synchronized the mmWave module
         * Configuration of the mmWave link is the responsibility of the application using the link API */
        if (((ptrMMWaveMCB->status & MMWAVE_STATUS_SYNCHRONIZED) == 0U) ||
            ((ptrMMWaveMCB->status & MMWAVE_STATUS_OPENED)       == 0U))
        {
            /* Error: Invalid usage the module should be synchronized before it can be started. */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
            goto exit;
        }

        /* Initialize the DFE Output mode: */
        ptrMMWaveMCB->dfeDataOutputMode = ptrCalibrationCfg->dfeDataOutputMode;
    }

    /* Sanity Check: Ensure that the module has not already been started */
    if ((ptrMMWaveMCB->status & MMWAVE_STATUS_STARTED) == MMWAVE_STATUS_STARTED)
    {
        /* Error: Invalid usage the module should be stopped before it can be started again. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Copy over the calibration configuration: */
    memcpy ((void*)&ptrMMWaveMCB->calibrationCfg, (const void*)ptrCalibrationCfg, sizeof(MMWave_CalibrationCfg));

    /* SOC specific start: We need to notify the peer domain before the real time starts. */
    retVal = MMWave_deviceStartFxn (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: SOC Start failed; error code is already setup */
        goto exit;
    }

    /* Start the mmWave link: */
    retVal = MMWave_startLink (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to start the link; error code is already setup */
        goto exit;
    }

    /* The module has been started successfully: */
    ptrMMWaveMCB->status = ptrMMWaveMCB->status | MMWAVE_STATUS_STARTED;

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        retVal = 0;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to stop the mmWave control module after the
 *      configuration has been applied.
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave control module
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @pre
 *      MMWave_start
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_stop (MMWave_Handle mmWaveHandle, int32_t* errCode)
{
    MMWave_MCB*         ptrMMWaveMCB;
    int32_t             retVal;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the control module */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;
    if (ptrMMWaveMCB == NULL)
    {
        /* Error: Invalid argument. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /****************************************************************************************
     * Sanity Check:
     *  - Validate the prerequisites
     ****************************************************************************************/
    if ((ptrMMWaveMCB->status & MMWAVE_STATUS_STARTED) == 0U)
    {
        /* Error: Invalid usage the module should be started before it can be stopped. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Stop the mmWave link */
    retVal = MMWave_stopLink (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Unable to stop the link; error code is setup already */
        goto exit;
    }

    /* Invoke the SOC specific function to handle the stop */
    retVal = MMWave_deviceStopFxn (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: SOC specific function could not be stopped. Error code is already
         * setup */
        goto exit;
    }

    /* The module has been stopped successfully: */
    ptrMMWaveMCB->status = ptrMMWaveMCB->status & (~(uint32_t)MMWAVE_STATUS_STARTED);

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        retVal = 0;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the mmWave module.
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave module
 *  @param[in]  ptrOpenCfg
 *      Pointer to the open configuration
 *  @param[in] ptrCalibData
 *      This is the pointer to the calibration data which is passed
 *      to the BSS to bypass calibration and restore it to a previously saved
 *      value. Set this to NULL and the mmWave module will request the BSS to
 *      perform the calibration.
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @pre
 *      MMWave_init
 *  @pre
 *      MMWave_sync
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_open
(
    MMWave_Handle           mmWaveHandle,
    const MMWave_OpenCfg*   ptrOpenCfg,
    rlCalibrationData_t*    ptrCalibData,
    int32_t*                errCode
)
{
    int32_t         retVal;
    MMWave_MCB*     ptrMMWaveMCB;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the mmWave module: */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;

    /* Sanity Check: Validate the arguments */
    if ((ptrMMWaveMCB == NULL) || (ptrOpenCfg == NULL))
    {
        /* Error: Invalid arguments. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /****************************************************************************************
     * Sanity Check:
     *  - Validate the prerequisites
     ****************************************************************************************/
    if ((ptrMMWaveMCB->status & MMWAVE_STATUS_SYNCHRONIZED) == 0U)
    {
        /* Error: Invalid usage the module should be synchronized before it can be opened. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Sanity Check: Ensure that the module is not already open */
    if ((ptrMMWaveMCB->status & MMWAVE_STATUS_OPENED) == MMWAVE_STATUS_OPENED)
    {
        /* Error: Invalid usage the module should be closed before it can be opened */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Copy over the configuration: */
    memcpy ((void *)&ptrMMWaveMCB->openCfg, (const void*)ptrOpenCfg, sizeof(MMWave_OpenCfg));

    /* Open the mmWave Link: */
    retVal = MMWave_openLink (ptrMMWaveMCB, ptrCalibData, errCode);
    if (retVal < 0)
    {
        /* Error: mmWave Open failed; error code is already setup  */
        goto exit;
    }

    /* SOC specific open: */
    retVal = MMWave_deviceOpenFxn (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: SOC Open failed; error code is already setup  */
        goto exit;
    }

    /* The module has been opened successfully: */
    ptrMMWaveMCB->status = ptrMMWaveMCB->status | MMWAVE_STATUS_OPENED;

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        retVal = 0;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the mmWave module.
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave module
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @pre
 *      MMWave_init
 *  @pre
 *      MMWave_sync
 *  @pre
 *      MMWave_open
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_close (MMWave_Handle mmWaveHandle, int32_t* errCode)
{
    int32_t         retVal;
    MMWave_MCB*     ptrMMWaveMCB;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the mmWave module: */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;

    /* Sanity Check: Validate the arguments */
    if (ptrMMWaveMCB == NULL)
    {
        /* Error: Invalid arguments. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Sanity Check: Ensure that the module is not already closed */
    if ((ptrMMWaveMCB->status & MMWAVE_STATUS_OPENED) == 0U)
    {
        /* Error: Invalid usage the module should be opened before it can be closed */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* SOC specific close: */
    retVal = MMWave_deviceCloseFxn (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: SOC Close failed; error code is already setup  */
        goto exit;
    }

    /* Close the mmWave Link: */
    retVal = MMWave_closeLink (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: mmWave Close failed; error code is already setup  */
        goto exit;
    }

    /* The module has been closed successfully: */
    ptrMMWaveMCB->status = ptrMMWaveMCB->status & (~(uint32_t)MMWAVE_STATUS_OPENED);

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        retVal = 0;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the mmWave control module. This
 *      needs to be invoked before any other control API is invoked.
 *
 *  @param[in]  ptrCtrlInitCfg
 *      Pointer to the control init configuration
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Handle to the control module
 *  @retval
 *      Error   -   NULL
 */
MMWave_Handle MMWave_init (MMWave_InitCfg* ptrCtrlInitCfg, int32_t* errCode)
{
    MMWave_MCB*         ptrMMWaveMCB = NULL;
    SemaphoreP_Params   semParams;
    int32_t             retVal;

    /* Initialize the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    if ((ptrCtrlInitCfg == NULL) || (ptrCtrlInitCfg->eventFxn == NULL))
    {
        /* Error: Invalid argument detected */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Initialize the memory block */
    memset ((void *)&gMMWave_MCB, 0, sizeof(MMWave_MCB));

    /* Populate the control MCB */
    memcpy ((void *)&gMMWave_MCB.initCfg, (void *)ptrCtrlInitCfg, sizeof(MMWave_InitCfg));

    /* Are we operating in full configuration mode? */
    if (gMMWave_MCB.initCfg.cfgMode == MMWave_ConfigurationMode_FULL)
    {
        /* YES: Setup the default semaphore parameters */
        SemaphoreP_Params_init (&semParams);

        /* Configure the semaphore parameters */
        semParams.name = "MMWave_CfgSemaphore";
        semParams.mode = SemaphoreP_Mode_BINARY;

        /* Create the semaphore used for protection of the *full configuration* */
        gMMWave_MCB.cfgSemHandle = SemaphoreP_create(1U, &semParams);
        if (gMMWave_MCB.cfgSemHandle == NULL)
        {
            /* Error: Semaphore was not created. */
            *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EOS, 0);
            goto exit;
        }
    }

    /* Setup the default semaphore parameters */
    SemaphoreP_Params_init (&semParams);

    /* Configure the semaphore parameters */
    semParams.name = "MMWave_Semaphore";
    semParams.mode = SemaphoreP_Mode_BINARY;

    /* Create the mmWave link semaphore: */
    gMMWave_MCB.linkSemHandle = SemaphoreP_create (0U, &semParams);
    if (gMMWave_MCB.linkSemHandle == NULL)
    {
        /* Error: Semaphore was not created. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EOS, 0);
        goto exit;
    }

    /* Initialize the mmWave Link module: */
    retVal = MMWave_initLink (&gMMWave_MCB, errCode);
    if (retVal < 0)
    {
        /* Error: Failure to open the mmWave Link; error code is already setup. */
        goto exit;
    }

    /* SOC Initializations: */
    retVal = MMWave_deviceInitFxn (&gMMWave_MCB, errCode);
    if (retVal < 0)
    {
        /* Error: SOC Initialization failed; error code is already setup  */
        goto exit;
    }

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        ptrMMWaveMCB = &gMMWave_MCB;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        ptrMMWaveMCB = NULL;
    }
    return (MMWave_Handle)ptrMMWaveMCB;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize and shutdown the mmWave module
 *
 *  @param[in]  mmWaveHandle
 *      Handle to the mmWave control module
 *  @param[out] errCode
 *      Encoded Error code populated by the API on an error
 *
 *  \ingroup MMWAVE_CTRL_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MMWave_deinit (MMWave_Handle mmWaveHandle, int32_t* errCode)
{
    MMWave_MCB*     ptrMMWaveMCB;
    int32_t         retVal;

    /* Initialize the error code: */
    *errCode = 0;

    /* Get the pointer to the control module */
    ptrMMWaveMCB = (MMWave_MCB*)mmWaveHandle;
    if (ptrMMWaveMCB == NULL)
    {
        /* Error: Invalid argument. */
        *errCode = MMWave_encodeError (MMWave_ErrorLevel_ERROR, MMWAVE_EINVAL, 0);
        goto exit;
    }

    /* Perform the SOC specific deinitialization: */
    retVal = MMWave_deviceDeinitFxn (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: SOC Device initialization failed. Error code is already setup */
        goto exit;
    }

    /* Deinitialize the link: */
    retVal = MMWave_deinitLink (ptrMMWaveMCB, errCode);
    if (retVal < 0)
    {
        /* Error: Link deinitialization failed. Error code is already setup */
        goto exit;
    }

    /* SOC was deinitialized: Delete the semaphore handle */
    SemaphoreP_delete (ptrMMWaveMCB->linkSemHandle);

    /* Delete the configuration semaphore handle (if available). The semaphore is
     * only created in full configuration mode. */
    if (ptrMMWaveMCB->cfgSemHandle != NULL)
    {
        SemaphoreP_delete (ptrMMWaveMCB->cfgSemHandle);
    }

    /* Reset the memory: */
    memset ((void *)ptrMMWaveMCB, 0, sizeof(MMWave_MCB));

exit:
    /* Determing the error level from the error code? */
    if (MMWave_decodeErrorLevel (*errCode) == MMWave_ErrorLevel_SUCCESS)
    {
        /* Success: Setup the return value */
        retVal = 0;
    }
    else
    {
        /* Informational/Error: Setup the return value */
        retVal = MINUS_ONE;
    }
    return retVal;
}

