/**
 *   @file  framework_core.c
 *
 *   @brief
 *      The file implements the core TestFmk functionality. The core module
 *      provides the functionality to the users of the TestFmk module.
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
#include <ti/common/sys_common.h>
#include <ti/drivers/soc/soc.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/test/common/framework.h>
#include <ti/drivers/test/common/framework_internal.h>

/**************************************************************************
 *************************** Local Functions ******************************
 **************************************************************************/

static void TestFmk_chirpISR(uintptr_t arg);
static void TestFmk_frameStartISR(uintptr_t arg);
static void TestFmk_edmaTCErrorFxn (EDMA_Handle handle, EDMA_transferControllerErrorInfo_t* ptrErrorInfo);
static void TestFmk_edmaErrorFxn(EDMA_Handle handle, EDMA_errorInfo_t* ptrErrorInfo);

/**************************************************************************
 ************************* TestFmk Module Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      TestFmk Module instantiated callback function which is registered with
 *      the EDMA driver and is invoked on an error
 *
 *  @param[in]  handle
 *      EDMA Driver Handle
 *  @param[in]  ptrErrorInfo
 *      Pointer to the error information
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmk_edmaErrorFxn
(
    EDMA_Handle         handle,
    EDMA_errorInfo_t*   ptrErrorInfo
)
{
    DebugP_assert (0);
}

/**
 *  @b Description
 *  @n
 *      TestFmk Module instantiated callback function which is registered with
 *      the EDMA driver and is invoked on a transfer controller error
 *
 *  @param[in]  handle
 *      EDMA Driver Handle
 *  @param[in]  ptrErrorInfo
 *      Pointer to the error information
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmk_edmaTCErrorFxn
(
    EDMA_Handle                         handle,
    EDMA_transferControllerErrorInfo_t* ptrErrorInfo
)
{
    DebugP_assert (0);
}

/**
 *  @b Description
 *  @n
 *      Chirp Available System ISR registered by the TestFmk module
 *
 *  @param[in]  arg
 *      TestFmk Module Handle
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmk_chirpISR(uintptr_t arg)
{
    TestFmk_MCB*    ptrTestFmk;

    /* Get the pointer to the TestFmk Module: */
    ptrTestFmk = (TestFmk_MCB*)arg;

    /* Pass the control back to the profile registered chirp available function: */
    ptrTestFmk->profileCfg.chirpAvailableFxn (ptrTestFmk->profileHandle);

    /* Post the semaphore: */
    SemaphoreP_postFromISR (ptrTestFmk->semaphoreHandle);
    return;
}

/**
 *  @b Description
 *  @n
 *      Frame Start System ISR registered by the TestFmk module
 *
 *  @param[in]  arg
 *      TestFmk Module Handle
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void TestFmk_frameStartISR(uintptr_t arg)
{
    TestFmk_MCB*    ptrTestFmk;

    /* Get the pointer to the TestFmk Module: */
    ptrTestFmk = (TestFmk_MCB*)arg;

    /* Pass the control back to the profile registered frame start function: */
    ptrTestFmk->profileCfg.frameStartFxn (ptrTestFmk->profileHandle);

    /* Post the semaphore: */
    SemaphoreP_postFromISR (ptrTestFmk->semaphoreHandle);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function executes the TestFmk Module. Execution of the TestFmk module involves
 *      the following:-
 *       - Handling of the reception of the IPC Messages exchanged between the
 *         TestFmk Peers.
 *       - Processing of the ADC data as per the loaded profile.
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[out]  ptrResult
 *      Pointer to the result populated by the loaded profile.
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_execute (TestFmk_Handle handle, TestFmk_Result* ptrResult, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    int32_t     retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Pending on the semaphore: Waiting for events to be received */
    SemaphoreP_pend (ptrTestFmk->semaphoreHandle, SemaphoreP_WAIT_FOREVER);

    /* Initialize the result buffer: */
    ptrResult->ptrBuffer  = NULL;
    ptrResult->size       = 0U;

    /* Process any IPC messages: We ignore the return value because if there
     * are any issues while processing the IPC Message the application is
     * notified via the reporting mechanism */
    TestFmk_IPCRecv (ptrTestFmk, errCode);

    /* Are we responsible for the profile? */
    if (ptrTestFmk->profileHandle != NULL)
    {
        /* YES: Invoke the profile execution */
        retVal = ptrTestFmk->profileCfg.executeFxn (ptrTestFmk->profileHandle, ptrResult, errCode);
    }
    else
    {
        /* We are done: */
        retVal = 0;
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the ADC channels
 *      as per the supplied configuration
 *
 *  @param[in]  ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[in]  ptrTestFmkCfg
 *      Pointer to the TestFmk configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_enableADC
(
    TestFmk_MCB*        ptrTestFmk,
    TestFmk_Cfg*        ptrTestFmkCfg,
    int32_t*            errCode
)
{
    int32_t             retVal      = MINUS_ONE;
    uint32_t            rxChanMask  = 0xF;
    uint8_t             numBytePerSample;
    ADCBuf_dataFormat   dataFormat;
    ADCBuf_RxChanConf   rxChanCfg;
    ADCBuf_CQConf       cqCfg;
    uint8_t             channel;
    uint32_t            chirpThreshold;

    /* Sanity Checking: Number of ADC Samples needs to be provided in Frame Mode. */
    if ((ptrTestFmkCfg->dfeDataOutputMode != MMWave_DFEDataOutputMode_CONTINUOUS))
    {
        /* We are operating in Frame or Advanced Frame Mode: */
        if (ptrTestFmkCfg->u.frameCfg.numADCSamples == 0U)
        {
            /* Error: Invalid configuration */
            *errCode = TESTFMK_EINVAL;
            goto exit;
        }
    }
    else
    {
        /* We are operating in continuous mode. */
        if (ptrTestFmkCfg->u.contCfg.dataTransSize == 0)
        {
            /* Error: Invalid configuration */
            *errCode = TESTFMK_EINVAL;
            goto exit;
        }
    }

    /*******************************************************************************
     * Disable all ADCBuf channels:
     *******************************************************************************/
    *errCode = ADCBuf_control(ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_CHANNEL_DISABLE, (void *)&rxChanMask);
    if (*errCode != ADCBuf_STATUS_SUCCESS)
    {
        /* Error: Unable to disable all channels. */
        goto exit;
    }

    /*******************************************************************************
     * Configure the ADC Buffer Data Format:
     *******************************************************************************/
    memset ((void *)&dataFormat, 0, sizeof(ADCBuf_dataFormat));

    /* Populate the configuration: */
    dataFormat.adcOutFormat       = ptrTestFmkCfg->adcFmt;
    dataFormat.sampleInterleave   = ptrTestFmkCfg->iqSwapSel;
    dataFormat.channelInterleave  = ptrTestFmkCfg->chInterleave;
    *errCode = ADCBuf_control(ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_CONF_DATA_FORMAT, (void *)&dataFormat);
    if (*errCode != ADCBuf_STATUS_SUCCESS)
    {
        /* Error: Unable to configure the data format */
        goto exit;
    }

    /*******************************************************************************
     * Determine the number of bytes per ADC sample:
     *******************************************************************************/
    if (ptrTestFmkCfg->adcFmt == 0)
    {
        /* Complex = 4 bytes */
        numBytePerSample = 4;
    }
    else
    {
        /* Real = 2 bytes */
        numBytePerSample = 2;
    }

    /*******************************************************************************
     * Enable the receive channels:
     *******************************************************************************/
    memset((void*)&rxChanCfg, 0, sizeof(ADCBuf_RxChanConf));

    /* Cycle through all the supported receive channels: */
    for (channel = 0; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++)
    {
        /* Is the channel enabled? */
        if (ptrTestFmkCfg->rxChannelEn & (0x1<<channel))
        {
            /* YES: Populate the receive channel configuration: */
            rxChanCfg.channel = channel;

            /* Enable the channel: */
            *errCode  = ADCBuf_control (ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_CHANNEL_ENABLE, (void *)&rxChanCfg);
            if (*errCode != ADCBuf_STATUS_SUCCESS)
            {
                /* Error: Unable to enable the receive channel */
                goto exit;
            }

            /*********************************************************************************
             * Get the next channel data buffer offset
             *********************************************************************************/
            if (ptrTestFmkCfg->dfeDataOutputMode == MMWave_DFEDataOutputMode_CONTINUOUS)
            {
                /*********************************************************************************
                 * Continuous Mode:
                 *********************************************************************************/
                rxChanCfg.offset += ptrTestFmkCfg->u.contCfg.dataTransSize *
                                    numBytePerSample;
            }
            else
            {
                /*********************************************************************************
                 * Frame Mode: Calculate ADCBuf size per channel
                 *********************************************************************************/
                rxChanCfg.offset += ptrTestFmkCfg->u.frameCfg.numADCSamples * numBytePerSample * ptrTestFmkCfg->chirpThreshold;
            }
        }
    }

    /***************************************************************************************
     * Chirp Threshold Configuration:
     ***************************************************************************************/
    chirpThreshold = (uint32_t)ptrTestFmkCfg->chirpThreshold;

#ifdef SOC_XWR14XX
    {
        /* Configure the chirp threshold */
        *errCode = ADCBuf_control(ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD,
                                  (void*)&chirpThreshold);
        if (*errCode != ADCBuf_STATUS_SUCCESS)
        {
            /* Error: Unable to configure the chirp threshold */
            goto exit;
        }
    }
#elif defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX)
    {
        /* Configure the chirp threshold */
        *errCode = ADCBuf_control(ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD,
                                  (void*)&chirpThreshold);
        if (*errCode != ADCBuf_STATUS_SUCCESS)
        {
            /* Error: Unable to configure the chirp threshold */
            goto exit;
        }
        /* Configure the chirp threshold */
        *errCode = ADCBuf_control(ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD,
                                  (void*)&chirpThreshold);
        if (*errCode != ADCBuf_STATUS_SUCCESS)
        {
            /* Error: Unable to configure the chirp threshold */
            goto exit;
        }
    }
#endif

    /***************************************************************************************
     * Chirp Quality Configuration:
     ***************************************************************************************/
    if (ptrTestFmkCfg->enableCQ == 1U)
    {
        /* Configure the chirp quality: Copy over to overcome any alignment issues */
        memcpy ((void*)&cqCfg, (void *)&ptrTestFmkCfg->cqCfg, sizeof(ADCBuf_CQConf));
        *errCode = ADCBuf_control(ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_CONF_CQ,
                                  (void *)&cqCfg);
        if (*errCode != ADCBuf_STATUS_SUCCESS)
        {
            /* Error: Unable to configure the chirp quality */
            goto exit;
        }
    }

    /***************************************************************************************
     * Continuous Mode configuration:
     ***************************************************************************************/
    if (ptrTestFmkCfg->dfeDataOutputMode == MMWave_DFEDataOutputMode_CONTINUOUS)
    {
        uint32_t    arg;

        /* Configure ADCBUF Driver in continuous mode */
        arg = 1U;
        *errCode = ADCBuf_control (ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_SET_CONTINUOUS_MODE, (void *)&arg);
        if (*errCode != ADCBuf_STATUS_SUCCESS)
        {
            /* Error: Unable to enable the continuous mode in the ADC Driver */
            goto exit;
        }

        /* Start the continuous streaming mode in ADCBUFF */
        *errCode = ADCBuf_control (ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_START_CONTINUOUS_MODE,
                                   (void *)&ptrTestFmkCfg->u.contCfg.dataTransSize);
        if (*errCode != ADCBuf_STATUS_SUCCESS)
        {
            /* Error: Unable to start the continuous mode */
            goto exit;
        }
    }

    /* ADCBUF Driver has been configured successfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to disable all the ADC channels.
 *
 *  @param[in]  ptrTestFmk
 *      Pointer to the TestFmk module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_disableADC
(
    TestFmk_MCB*    ptrTestFmk,
    int32_t*        errCode
)
{
    int32_t     retVal      = MINUS_ONE;
    uint32_t    rxChanMask  = 0xF;

    /*******************************************************************************
     * Disable all ADCBuf channels:
     *******************************************************************************/
    *errCode = ADCBuf_control(ptrTestFmk->adcBufHandle, ADCBufMMWave_CMD_CHANNEL_DISABLE,
                             (void *)&rxChanMask);
    if (*errCode != ADCBuf_STATUS_SUCCESS)
    {
        /* Error: Unable to disable all channels. */
        goto exit;
    }

    /* ADC channels have been disabled */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to control the profile.
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[in]  cmd
 *      Profile specific command
 *  @param[in]  arg
 *      Argument which is command specific
 *  @param[in]  argLen
 *      Length of the arguments which is also command specific
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_ioctl (TestFmk_Handle handle, uint32_t cmd, void* arg, uint32_t argLen, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    TestFmk_Msg     message;
    int32_t     retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the profile status
     *  - Profile should be loaded */
    if ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_LOADED) == 0U)
    {
        /* Error: Profile is an invalid state. Please refer to the calling API sequence. */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Is this command meant for the core TestFmk module or the profile? */
    switch (cmd)
    {
        case TEST_FMK_CMD_GET_ADCBUF_HANDLE:
        {
            /* Sanity Check: Validate the arguments */
            if (argLen != sizeof(ADCBuf_Handle))
            {
                /* Error: Invalid argument. */
                *errCode = TESTFMK_EINVAL;
                goto exit;
            }

            /* Return the ADCBUF Driver Handle: We are done. */
            memcpy (arg, (void*)&ptrTestFmk->adcBufHandle, argLen);
            retVal = 0;
            goto exit;
        }
        case TEST_FMK_CMD_GET_EDMA_HANDLE:
        {
            /* Sanity Check: Validate the arguments */
            if (argLen != (sizeof(EDMA_Handle) * EDMA_NUM_CC))
            {
                /* Error: Invalid argument. */
                *errCode = TESTFMK_EINVAL;
                goto exit;
            }

            /* Return the EDMA Driver Handle(s): We are done. */
            memcpy (arg, (void*)&ptrTestFmk->edmaHandle, argLen);
            retVal = 0;
            goto exit;
        }
        case TEST_FMK_CMD_GET_CONFIG:
        {
            /* Sanity Check: Validate the arguments */
            if (argLen != sizeof(TestFmk_Cfg) || ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_CONFIGURED) == 0))
            {
                /* Error: Invalid argument. */
                *errCode = TESTFMK_EINVAL;
                goto exit;
            }

            /* Copy over the TestFmk configuration: */
            memcpy (arg, (void*)&ptrTestFmk->testfmkCfg, argLen);
            retVal = 0;
            goto exit;
        }
        case TEST_FMK_CMD_SET_CONFIG:
        {
            /* Sanity Check: Validate the arguments */
            if (argLen != sizeof(TestFmk_Cfg))
            {
                /* Error: Invalid argument. */
                *errCode = TESTFMK_EINVAL;
                goto exit;
            }

            /* We need to now inform the TestFmk entities that the configuration has been received. */
            TestFmk_IPCMsgInit (&message, TestFmk_MessageType_SET_TESTFMK_CFG);
            memcpy ((void*)&message.u.testfmkCfg, (void*)arg, sizeof(TestFmk_Cfg));
            retVal = TestFmk_IPCSend (ptrTestFmk, &message, errCode);
            goto exit;
        }
        case TEST_FMK_CMD_NOTIFY_FAULT:
        {
            /* Initialize the message: */
            TestFmk_IPCMsgInit (&message, TestFmk_MessageType_FAULT);
            retVal = TestFmk_IPCSend (ptrTestFmk, &message, errCode);
            goto exit;
        }
        default:
        {
            /*************************************************************************
             * The command was not meant for the core TestFmk Module. We will pass this to
             * the profile for handling. The status of the command will be reported
             * back to the application via the reporting mechansim
             *
             * Sanity Check: Ensure that the message can fit within the IPC Buffer
             *************************************************************************/
            if (argLen > TEST_FMK_MAX_PROFILE_DATA)
            {
                /* Error: Invalid argument (We dont have enough space to fit the message */
                *errCode = TESTFMK_EINVAL;
                goto exit;
            }

            /* Initialize the message: */
            TestFmk_IPCMsgInit (&message, TestFmk_MessageType_PROFILE);
            message.u.profileMsg.cmd    = cmd;
            message.u.profileMsg.cmdLen = argLen;
            memcpy ((void*)&message.u.profileMsg.profileData[0], (void*)arg, argLen);
            retVal = TestFmk_IPCSend (ptrTestFmk, &message, errCode);
            goto exit;
        }
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to start the profile.
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_start (TestFmk_Handle handle, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    TestFmk_Msg     message;
    int32_t     retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the profile status
     *  - Profile should be loaded
     *  - Profile should be open
     *  - Profile should be configured
     *  - Profile should NOT be started. */
    if (((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_LOADED)     == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_OPENED)     == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_CONFIGURED) == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_STARTED)    != 0U))
    {
        /* Error: Profile is an invalid state. Please refer to the calling API sequence. */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* We need to now inform the TestFmk entities that the profile is being started */
    TestFmk_IPCMsgInit (&message, TestFmk_MessageType_START);
    retVal = TestFmk_IPCSend (ptrTestFmk, &message, errCode);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to stop the profile.
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_stop (TestFmk_Handle handle, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    TestFmk_Msg     message;
    int32_t     retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the profile status
     *  - Profile should be loaded
     *  - Profile should be open
     *  - Profile should be started. */
    if (((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_LOADED)  == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_OPENED)  == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_STARTED) == 0U))
    {
        /* Error: Profile is an invalid state. Please refer to the calling API sequence. */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* We need to now inform the TestFmk entities that the profile is being stopped */
    TestFmk_IPCMsgInit (&message, TestFmk_MessageType_STOP);
    retVal = TestFmk_IPCSend (ptrTestFmk, &message, errCode);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the TestFmk Module and associated profile with
 *      the specific mmWave link open configuration.
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[in]  ptrOpenCfg
 *      Pointer to the mmWave module open configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_open (TestFmk_Handle handle, MMWave_OpenCfg* ptrOpenCfg, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    TestFmk_Msg     message;
    int32_t         retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the profile status
     *  - Profile should be loaded
     *  - Profile should not already be open */
    if (((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_LOADED) == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_OPENED) != 0U) )
    {
        /* Error: Profile is an invalid state. Please refer to the calling API sequence. */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* We need to now inform the TestFmk entities that the profile is being opened */
    TestFmk_IPCMsgInit (&message, TestFmk_MessageType_OPEN);
    memcpy ((void*)&message.u.mmWaveOpenCfg, (void*)ptrOpenCfg, sizeof(MMWave_OpenCfg));
    retVal = TestFmk_IPCSend (ptrTestFmk, &message, errCode);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the TestFmk Module and associated profile.
 *      This is only applicable from the TestFmk Instance which has loaded the
 *      profile.
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_close (TestFmk_Handle handle, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    TestFmk_Msg     message;
    int32_t     retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the profile status
     *  - Profile should be loaded
     *  - Profile should be open
     *  - Profile should NOT be started */
    if (((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_LOADED)  == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_OPENED)  == 0U) ||
        ((ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_STARTED) != 0U))
    {
        /* Error: Profile is an invalid state. Please refer to the calling API sequence. */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* We need to now inform the TestFmk entities that the profile is being closed */
    TestFmk_IPCMsgInit (&message, TestFmk_MessageType_CLOSE);
    retVal = TestFmk_IPCSend (ptrTestFmk, &message, errCode);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to load the profile.
 *
 *  @param[in]  testfmkHandle
 *      Handle to the TestFmk Module
 *  @param[in]  ptrProfileCfg
 *      Profile to be loaded
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_loadProfile
(
    TestFmk_Handle      testfmkHandle,
    TestFmk_ProfileCfg* ptrProfileCfg,
    int32_t*            errCode
)
{
    TestFmk_MCB*            ptrTestFmk;
    int32_t                 retVal = MINUS_ONE;
    ADCBuf_Params           params;
    uint8_t                 numInstances;
    SOC_SysIntListenerCfg   socIntCfg;
    uint8_t                 index;
    EDMA_instanceInfo_t     edmaInstanceInfo;
    EDMA_errorConfig_t      edmaErrorConfig;
    TestFmk_Msg             message;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)testfmkHandle;
    if ((ptrTestFmk == NULL) || (ptrProfileCfg == NULL))
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the profile status
     *  - No other profile should be loaded */
    if (ptrTestFmk->profileStatus & TEST_FMK_PROFILE_STATUS_LOADED)
    {
        /* Error: Invalid use case; previous profile should be unloaded */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the arguments */
    if ((ptrProfileCfg->initFxn    == NULL)     ||
        (ptrProfileCfg->controlFxn == NULL)     ||
        (ptrProfileCfg->openFxn    == NULL)     ||
        (ptrProfileCfg->closeFxn   == NULL)     ||
        (ptrProfileCfg->startFxn   == NULL)     ||
        (ptrProfileCfg->stopFxn    == NULL)     ||
        (ptrProfileCfg->executeFxn == NULL)     ||
        (ptrProfileCfg->deinitFxn  == NULL))
    {
        /* Error: User is trying to load an invalid profile. Profiles need to implement
         * all the framework required functions. */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Initialize and load the profile */
    ptrTestFmk->profileHandle = ptrProfileCfg->initFxn (testfmkHandle, &ptrTestFmk->initCfg,
                                                        ptrTestFmk->semaphoreHandle, errCode);
    if (ptrTestFmk->profileHandle == NULL)
    {
        /* Error: Unable to initialize and load the profile. */
        goto exit;
    }

    /* Do we need to register the chirp available function? */
    if (ptrProfileCfg->chirpAvailableFxn != NULL)
    {
        /* Initialize the SOC Listener configuration: */
        memset ((void*)&socIntCfg, 0, sizeof(SOC_SysIntListenerCfg));

        /* Populate the configuration: */
        socIntCfg.systemInterrupt  = gTestFmkHwAttrib.chirpAvailableSysInterrupt;
        socIntCfg.listenerFxn      = TestFmk_chirpISR;
        socIntCfg.arg              = (uintptr_t)ptrTestFmk;
        ptrTestFmk->chirpAvailableListenerHandle = SOC_registerSysIntListener(ptrTestFmk->initCfg.socHandle, &socIntCfg, errCode);
        if (ptrTestFmk->chirpAvailableListenerHandle == NULL)
        {
            /* Error: Unable to register the system interrupt; error code is already setup */
            goto exit;
        }
    }

    /* Do we need to register the frame start function? */
    if (ptrProfileCfg->frameStartFxn != NULL)
    {
        /* Initialize the SOC Listener configuration: */
        memset ((void*)&socIntCfg, 0, sizeof(SOC_SysIntListenerCfg));

        /* Populate the configuration: */
        socIntCfg.systemInterrupt  = gTestFmkHwAttrib.frameStartSysInterrupt;
        socIntCfg.listenerFxn      = TestFmk_frameStartISR;
        socIntCfg.arg              = (uintptr_t)ptrTestFmk;
        ptrTestFmk->frameStartListenerHandle = SOC_registerSysIntListener(ptrTestFmk->initCfg.socHandle, &socIntCfg, errCode);
        if (ptrTestFmk->frameStartListenerHandle == NULL)
        {
            /* Error: Unable to register the system interrupt; error code is already setup */
            goto exit;
        }
    }

    /* Copy over the profile configuration: */
    memcpy ((void*)&ptrTestFmk->profileCfg, (void*)ptrProfileCfg, sizeof(TestFmk_ProfileCfg));

    /***************************************************************************************
     * ADCBUF Initializations:
     ***************************************************************************************/
    ADCBuf_init ();

    /* Open the ADC Driver: */
    ADCBuf_Params_init(&params);

#ifdef SOC_XWR14XX
    params.chirpThreshold = 1U;
#elif defined(SOC_XWR16XX) || defined (SOC_XWR18XX) || defined (SOC_XWR68XX)
    params.chirpThresholdPing = 1U;
    params.chirpThresholdPong = 1U;
#endif
    params.continousMode  = 0U;
    ptrTestFmk->adcBufHandle = ADCBuf_open(0, &params);
    DebugP_assert (ptrTestFmk->adcBufHandle != NULL);

    /***************************************************************************************
     * EDMA Initializations:
     ***************************************************************************************/
    numInstances = EDMA_getNumInstances();
    for (index = 0U; index < numInstances; index++)
    {
        /* Initialize the EDMA */
        EDMA_init (index);

        /* Open the EDMA: */
        ptrTestFmk->edmaHandle[index] = EDMA_open(index, errCode, &edmaInstanceInfo);
        DebugP_assert (ptrTestFmk->edmaHandle[index] != NULL);

        /* Initialize the EDMA Error configuration: */
        memset ((void *)&edmaErrorConfig, 0, sizeof(EDMA_errorConfig_t));

        /* Populate the EDMA Error configuration: */
        edmaErrorConfig.isConfigAllEventQueues              = true;
        edmaErrorConfig.isConfigAllTransferControllers      = true;
        edmaErrorConfig.isEventQueueThresholdingEnabled     = true;
        edmaErrorConfig.eventQueueThreshold                 = EDMA_EVENT_QUEUE_THRESHOLD_MAX;
        edmaErrorConfig.isEnableAllTransferControllerErrors = true;
        edmaErrorConfig.callbackFxn                         = TestFmk_edmaErrorFxn;
        edmaErrorConfig.transferControllerCallbackFxn       = TestFmk_edmaTCErrorFxn;

        /* Configure the error monitoring for the EDMA Instance: */
        *errCode = EDMA_configErrorMonitoring(ptrTestFmk->edmaHandle[index], &edmaErrorConfig);
        if (*errCode != EDMA_NO_ERROR)
        {
            /* Error: Unable to configure the error monitoring */
            goto exit;
        }
    }

    /* Profile has been loaded successfully: Setup the return value and error code */
    retVal   = 0;
    *errCode = 0;

exit:
    if (ptrTestFmk != NULL)
    {
        /* We need to now inform the TestFmk entities about the status of the profile */
        TestFmk_IPCMsgInit (&message, TestFmk_MessageType_RESPONSE);

        /* Setup the response: Profile has been loaded */
        message.u.responseMsg.requestMsgType = TestFmk_MessageType_LOAD;
        message.u.responseMsg.errCode        = *errCode;
        message.u.responseMsg.arg0           = 0U;
        message.u.responseMsg.arg1           = 0U;

        /* Send out the message: */
        TestFmk_IPCSend (ptrTestFmk, &message, errCode);
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to unload and deinitialize a profile which has
 *      been previously loaded.
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_unloadProfile (TestFmk_Handle handle, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    uint8_t         numInstances;
    uint8_t         index;
    TestFmk_Msg     message;
    int32_t         retVal = MINUS_ONE;

    /* Initialize the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Deregister the frame start listener */
    if (ptrTestFmk->frameStartListenerHandle != NULL)
    {
        retVal = SOC_deregisterSysIntListener (ptrTestFmk->initCfg.socHandle, gTestFmkHwAttrib.frameStartSysInterrupt,
                                               ptrTestFmk->frameStartListenerHandle, errCode);
        if (retVal < 0)
        {
            /* Error: Unable to deregister the system interrupt */
            goto exit;
        }

        /* Reset the frame start listener: */
        ptrTestFmk->frameStartListenerHandle = NULL;
    }

    /* Deregister the chirp available listener */
    if (ptrTestFmk->chirpAvailableListenerHandle != NULL)
    {
        retVal = SOC_deregisterSysIntListener (ptrTestFmk->initCfg.socHandle, gTestFmkHwAttrib.chirpAvailableSysInterrupt,
                                               ptrTestFmk->chirpAvailableListenerHandle, errCode);
        if (retVal < 0)
        {
            /* Error: Unable to deregister the system interrupt */
            goto exit;
        }

        /* Reset the chirp available listener: */
        ptrTestFmk->chirpAvailableListenerHandle = NULL;
    }

    /* Is there a profile loaded? */
    if (ptrTestFmk->profileHandle != NULL)
    {
        /* Sanity Check: We have already verified this. */
        DebugP_assert (ptrTestFmk->profileCfg.deinitFxn != NULL);

        /* Deinitialize the profile: */
        retVal = ptrTestFmk->profileCfg.deinitFxn (ptrTestFmk->profileHandle, errCode);
        if (retVal < 0)
        {
            /* Error: Profile deinitialization failed; error code is already setup */
            goto exit;
        }
    }

    /* Close the ADCBUF Driver: */
    if (ptrTestFmk->adcBufHandle != NULL)
    {
        ADCBuf_close(ptrTestFmk->adcBufHandle);
    }

    /* Close the EDMA Instances: */
    numInstances = EDMA_getNumInstances();
    for (index = 0U; index < numInstances; index++)
    {
        /* Close the EDMA Instance: */
        if (ptrTestFmk->edmaHandle[index] != NULL)
        {
            EDMA_close (ptrTestFmk->edmaHandle[index]);
        }
    }

    /* Profile has been unloaded: */
    retVal = 0;

exit:
    if (ptrTestFmk != NULL)
    {
        /* We need to now inform the TestFmk entities about the status of the profile */
        TestFmk_IPCMsgInit (&message, TestFmk_MessageType_RESPONSE);

        /* Setup the response: Profile has been loaded */
        message.u.responseMsg.requestMsgType = TestFmk_MessageType_UNLOAD;
        message.u.responseMsg.errCode        = *errCode;
        message.u.responseMsg.arg0           = 0U;
        message.u.responseMsg.arg1           = 0U;

        /* Send out the message: */
        TestFmk_IPCSend (ptrTestFmk, &message, errCode);
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to synchronize the execution of the framework
 *      between the domains.
 *
 *  @param[in]  fmkHandle
 *      Handle to the framework
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
int32_t TestFmk_synch (TestFmk_Handle fmkHandle, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    int32_t         retVal = MINUS_ONE;

    /* Get the framework MCB: */
    ptrTestFmk = (TestFmk_MCB*)fmkHandle;
    if (ptrTestFmk == NULL)
    {
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

#if defined(SOC_XWR16XX) || defined (SOC_XWR18XX) || defined (SOC_XWR68XX)
#ifdef SUBSYS_DSS
    retVal = SOC_isMSSOperational (ptrTestFmk->initCfg.socHandle, errCode);
#else
    retVal = SOC_isDSSOperational (ptrTestFmk->initCfg.socHandle, errCode);
#endif
#else
    /* XWR14xx: We are always synchronized */
    retVal = 1;
#endif

exit:
    return retVal;

}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the data path manager and
 *      initialize and load the profile.
 *
 *  @param[in]  ptrInitCfg
 *      Pointer to the initialization configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Handle to the TestFmk Module
 *  @retval
 *      Error   -   NULL
 */
TestFmk_Handle TestFmk_init (TestFmk_InitCfg* ptrInitCfg, int32_t* errCode)
{
    TestFmk_MCB*        ptrTestFmk = NULL;
    SemaphoreP_Params   semParams;

    /* Sanity Check: Validate the arguments */
    if ((ptrInitCfg             == NULL) ||
        (ptrInitCfg->reportFxn  == NULL) ||
        (ptrInitCfg->socHandle  == NULL))
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Allocate memory for the TestFmk module: */
    ptrTestFmk = MemoryP_ctrlAlloc (sizeof(TestFmk_MCB), 0U);
    if (ptrTestFmk == NULL)
    {
        *errCode = TESTFMK_ENOMEM;
        goto exit;
    }

    /* Initialize the TestFmk Master control block: */
    memset ((void*)ptrTestFmk, 0, sizeof(TestFmk_MCB));

    /* Copy over the initialization configuration: */
    memcpy ((void*)&ptrTestFmk->initCfg, (void*)ptrInitCfg, sizeof(TestFmk_InitCfg));

    /* Initialize the TestFmk Semaphore: */
    SemaphoreP_Params_init (&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    ptrTestFmk->semaphoreHandle = SemaphoreP_create(0, &semParams);

    /* Initialize the IPC Module: */
    if (TestFmk_IPCInit (ptrTestFmk, errCode) < 0)
    {
        /* Error: Unable to initialize the IPC Module. */
        goto exit;
    }

#if defined(SOC_XWR16XX) || defined (SOC_XWR18XX) || defined (SOC_XWR68XX)
#ifdef SUBSYS_DSS
    (void)SOC_setDSSLinkState (ptrTestFmk->initCfg.socHandle, 1U, errCode);
#else
    (void)SOC_setMSSLinkState (ptrTestFmk->initCfg.socHandle, 1U, errCode);
#endif
#else
    /* There is no need to set any state: */
#endif

exit:
    return (TestFmk_Handle)ptrTestFmk;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize and shutdown the profile
 *
 *  @param[in]  handle
 *      Handle to the TestFmk Module
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup TEST_FRAMEWORK_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t TestFmk_deinit (TestFmk_Handle handle, int32_t* errCode)
{
    TestFmk_MCB*    ptrTestFmk;
    int32_t     retVal = MINUS_ONE;

    /* Sanity Check: Validate the arguments */
    ptrTestFmk = (TestFmk_MCB*)handle;
    if (ptrTestFmk == NULL)
    {
        /* Error: Invalid argument */
        *errCode = TESTFMK_EINVAL;
        goto exit;
    }

    /* Unload profile which have been loaded: */
    if (TestFmk_unloadProfile (handle, errCode) < 0)
    {
        /* Error: Unable to unload  the profile. Error code is already setup */
        goto exit;
    }

    /* Deinitialize the IPC Module: */
    if (TestFmk_IPCDeinit (ptrTestFmk, errCode) < 0)
    {
        /* Error: Unable to initialize the IPC Module. Error code is already setup */
        goto exit;
    }

#if defined(SOC_XWR16XX) || defined (SOC_XWR18XX) || defined (SOC_XWR68XX)
#ifdef SUBSYS_DSS
    retVal = SOC_setDSSLinkState (ptrTestFmk->initCfg.socHandle, 0U, errCode);
#else
    retVal = SOC_setMSSLinkState (ptrTestFmk->initCfg.socHandle, 0U, errCode);
#endif
#else
    /* There is no need to reset any state: */
#endif

    /* Cleanup the memory allocated for the TestFmk Module: */
    MemoryP_ctrlFree (ptrTestFmk, sizeof(TestFmk_MCB));

    /* Setup the return value */
    retVal = 0;

exit:
    return retVal;
}

