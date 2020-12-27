/**
 *   @file  dpm_core.c
 *
 *   @brief
 *      The file implements the core DPM functionality. The file
 *      implements the exported API which is available to the
 *      application/processing chain developers.
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
#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/include/dpm_internal.h>

/**************************************************************************
 *************************** Local Functions ******************************
 **************************************************************************/

static void    DPM_chirpISR(uintptr_t arg);
static void    DPM_frameStartISR(uintptr_t arg);
static int32_t DPM_initDPC (DPM_MCB* ptrDPM, DPM_ProcChainCfg* ptrProcChainCfg);
static int32_t DPM_deinitDPC (DPM_MCB* ptrDPM);

/**************************************************************************
 ************************* DPM Module Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Chirp Available System ISR registered by the DPM module
 *
 *  @param[in]  arg
 *      DPM Module Handle
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void DPM_chirpISR (uintptr_t arg)
{
    DPM_MCB*    ptrDPM;

    /* Get the pointer to the DPM Module: */
    ptrDPM = (DPM_MCB*)arg;

    /* Pass the control back to the processing chain registered chirp available function: */
    ptrDPM->procChainCfg.chirpAvailableFxn (ptrDPM->dpcHandle);
    return;
}

/**
 *  @b Description
 *  @n
 *      Frame Start System ISR registered by the DPM module
 *
 *  @param[in]  arg
 *      DPM Module Handle
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void DPM_frameStartISR (uintptr_t arg)
{
    DPM_MCB*    ptrDPM;

    /* Get the pointer to the DPM Module: */
    ptrDPM = (DPM_MCB*)arg;

    /* Pass the control back to the processing chain registered frame start function: */
    ptrDPM->procChainCfg.frameStartFxn (ptrDPM->dpcHandle);
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to load the processing chain
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM Module
 *  @param[in]  ptrProcChainCfg
 *      Processing chain to be loaded
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t DPM_initDPC
(
    DPM_MCB*            ptrDPM,
    DPM_ProcChainCfg*   ptrProcChainCfg
)
{
    int32_t                 errCode;
    int32_t                 retVal;
    SOC_SysIntListenerCfg   socIntCfg;

    /* Do we need to load the processing chain? All DPM Instances might
     * not be responsible for the actual processing chain execution. */
    if (ptrProcChainCfg == NULL)
    {
        /* There is no need to load the processing chain. */
        retVal = 0;
        goto exit;
    }

    /* Sanity Check: Validate the arguments */
    if ((ptrProcChainCfg->initFxn    == NULL)     ||
        (ptrProcChainCfg->ioctlFxn   == NULL)     ||
        (ptrProcChainCfg->startFxn   == NULL)     ||
        (ptrProcChainCfg->stopFxn    == NULL)     ||
        (ptrProcChainCfg->executeFxn == NULL)     ||
        (ptrProcChainCfg->deinitFxn  == NULL))
    {
        /* Error: User is trying to load an invalid processing chain. */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* Do we need to register the chirp available function? */
    if (ptrProcChainCfg->chirpAvailableFxn != NULL)
    {
        /* Initialize the SOC Listener configuration: */
        memset ((void*)&socIntCfg, 0, sizeof(SOC_SysIntListenerCfg));

        /* Populate the configuration: */
        socIntCfg.systemInterrupt  = gDPMHwAttrib.chirpAvailableSysInterrupt;
        socIntCfg.listenerFxn      = DPM_chirpISR;
        socIntCfg.arg              = (uintptr_t)ptrDPM;
        ptrDPM->chirpAvailableListenerHandle = SOC_registerSysIntListener(ptrDPM->initCfg.socHandle, &socIntCfg, &errCode);
        if (ptrDPM->chirpAvailableListenerHandle == NULL)
        {
            /* Error: Unable to register the system interrupt */
            retVal = errCode;
            goto exit;
        }
    }

    /* Do we need to register the frame start function? */
    if (ptrProcChainCfg->frameStartFxn != NULL)
    {
        /* Initialize the SOC Listener configuration: */
        memset ((void*)&socIntCfg, 0, sizeof(SOC_SysIntListenerCfg));

        /* Populate the configuration: */
        socIntCfg.systemInterrupt  = gDPMHwAttrib.frameStartSysInterrupt;
        socIntCfg.listenerFxn      = DPM_frameStartISR;
        socIntCfg.arg              = (uintptr_t)ptrDPM;
        ptrDPM->frameStartListenerHandle = SOC_registerSysIntListener(ptrDPM->initCfg.socHandle, &socIntCfg, &errCode);
        if (ptrDPM->frameStartListenerHandle == NULL)
        {
            /* Error: Unable to register the system interrupt */
            retVal = errCode;
            goto exit;
        }
    }

    /* Copy over the processing chain configuration: */
    memcpy ((void*)&ptrDPM->procChainCfg, (void*)ptrProcChainCfg, sizeof(DPM_ProcChainCfg));

    /* Initialize the processing chain: */
    ptrDPM->dpcHandle = ptrDPM->procChainCfg.initFxn ((DPM_Handle)ptrDPM, &ptrDPM->initCfg, &errCode);
    if (ptrDPM->dpcHandle == NULL)
    {
        /* Error: Unable to initialize the the processing chain. */
        retVal = errCode;
        goto exit;
    }

    /* DPM Module has been initialized succesfully */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to unload and deinitialize the processing
 *      chain which has been previously loaded
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM Module
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
static int32_t DPM_deinitDPC (DPM_MCB* ptrDPM)
{
    int32_t     retVal;
    int32_t     errCode;

    /* Initialize the return value: */
    retVal = 0;

    /* Deregister the frame start listener */
    if (ptrDPM->frameStartListenerHandle != NULL)
    {
        retVal = SOC_deregisterSysIntListener (ptrDPM->initCfg.socHandle, gDPMHwAttrib.frameStartSysInterrupt,
                                               ptrDPM->frameStartListenerHandle, &errCode);
        if (retVal < 0)
        {
            /* Error: Unable to deregister the system interrupt */
            retVal = errCode;
            goto exit;
        }

        /* Reset the frame start listener: */
        ptrDPM->frameStartListenerHandle = NULL;
    }

    /* Deregister the chirp available listener */
    if (ptrDPM->chirpAvailableListenerHandle != NULL)
    {
        retVal = SOC_deregisterSysIntListener (ptrDPM->initCfg.socHandle, gDPMHwAttrib.chirpAvailableSysInterrupt,
                                               ptrDPM->chirpAvailableListenerHandle, &errCode);
        if (retVal < 0)
        {
            /* Error: Unable to deregister the system interrupt */
            retVal = errCode;
            goto exit;
        }

        /* Reset the chirp available listener: */
        ptrDPM->chirpAvailableListenerHandle = NULL;
    }

    /* Is there a processing chain loaded? */
    if (ptrDPM->dpcHandle != NULL)
    {
        /* Sanity Check: We have already verified this. */
        DebugP_assert (ptrDPM->procChainCfg.deinitFxn != NULL);

        /* Deinitialize the Processing Chain: */
        retVal = ptrDPM->procChainCfg.deinitFxn (ptrDPM->dpcHandle);
        if (retVal < 0)
        {
            /* Error: Processing chain deinitialization failed */
            goto exit;
        }
    }

    /* Profile has been successfully unloaded: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function executes the DPM Module. Execution of the DPM module involves
 *      the following:-
 *       - Handling of the reception of the IPC Messages exchanged between the
 *         DPM Peers.
 *       - Execution & processing of the input data which has either been injected
 *         *or* received via the chirp available.
 *
 *  @param[in]  handle
 *      Handle to the DPM Module
 *  @param[out]  ptrResult
 *      Pointer to the result populated by the loaded processing chain.
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_execute (DPM_Handle handle, DPM_Buffer* ptrResult)
{
    DPM_MCB*    ptrDPM;
    uintptr_t   key;
    bool        executeDPC;
    int32_t     retVal;
    int32_t     errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if (ptrDPM == NULL)
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* Pending on the semaphore: Waiting for events to be received */
    SemaphoreP_pend (ptrDPM->semaphoreHandle, SemaphoreP_WAIT_FOREVER);

    /* Initialize the result buffer: */
    memset ((void *)ptrResult, 0, sizeof(DPM_Buffer));

    /* Process any IPC messages: We ignore the return value because if there
     * are any issues while processing the IPC Message the application is
     * notified via the reporting mechanism */
    DPM_msgRecv (ptrDPM, &errCode);

    /* Are we responsible for the processing chain? */
    if (ptrDPM->dpcHandle != NULL)
    {
        /**********************************************************************
         * Critical Section: Take a snapshot of the status flag and reset it
         **********************************************************************/
        key = HwiP_disable();
        executeDPC         = ptrDPM->executeDPC;
        ptrDPM->executeDPC = false;
        HwiP_restore(key);

        /* Do we need to invoke the method? */
        if (executeDPC == true)
        {
            /* YES: Invoke the processing chain execute method */
            retVal = ptrDPM->procChainCfg.executeFxn (ptrDPM->dpcHandle, ptrResult);
        }
        else
        {
            /* NO: We are done */
            retVal = 0;
        }
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
 *      The function is used to configure the processing chain.
 *
 *  @param[in]  handle
 *      Handle to the DPM Module
 *  @param[in]  cmd
 *      Profile specific command
 *  @param[in]  arg
 *      Argument which is command specific
 *  @param[in]  argLen
 *      Length of the arguments which is also command specific
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   one of @ref DPM_ERROR_CODE
 */
int32_t DPM_ioctl (DPM_Handle handle, uint32_t cmd, void* arg, uint32_t argLen)
{
    DPM_MCB*    ptrDPM;
    DPM_Msg     message;
    int32_t     retVal;
    int32_t     errCode;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if (ptrDPM == NULL)
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* Is this command meant for the core DPM module or the processing chain? */
    switch (cmd)
    {
        case DPM_CMD_INJECT_DATA:
        {
            /* Sanity Check: Validate the arguments */
            if (argLen != sizeof(DPM_Buffer))
            {
                /* Error: Invalid argument. */
                retVal = DPM_EINVAL;
                goto exit;
            }

            /* Sanity Checking: We can only inject data into a local DPM Data Path.
             * Is one executing on the DPM Instance? */
            if (ptrDPM->dpcHandle != NULL)
            {
                /* YES: Does the processing chain support injection of data? */
                if (ptrDPM->procChainCfg.injectDataFxn == NULL)
                {
                    /* Error: This feature is not supported by the processing chain. */
                    retVal = DPM_ENOTSUP;
                }
                else
                {
                    /* Profile should be in the correct state before we can inject the data */
                    if (ptrDPM->procChainStatus & DPM_PROC_CHAIN_STATUS_STARTED)
                    {
                        /* Immediately pass the data to the processing chain: */
                        retVal = ptrDPM->procChainCfg.injectDataFxn (ptrDPM->dpcHandle,
                                                                     (DPM_Buffer*)arg);
                    }
                    else
                    {
                        /* Error: The processing chain is not ready to get the data */
                        retVal = DPM_EPROFCFG;
                    }
                }
            }
            else
            {
                /* Error: Trying to inject data into a DPM Profile but none is registered
                 * This is an invalid use case. Data can only be injected if a processing
                 * chain has been registered in the local DPM instance. */
                retVal = DPM_EINVAL;
            }
            goto exit;
        }
        case DPM_CMD_BSS_FAULT:
        {
            /* Initialize the message: */
            DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_BSS_FAULT, 0U);
            retVal = DPM_pipeSend (ptrDPM, &message, &errCode);
            goto exit;
        }
        case DPM_CMD_DPC_ASSERT:
        {
            /* Sanity Check: Validate the arguments */
            if (argLen != sizeof(DPM_DPCAssert))
            {
                /* Error: Invalid argument. */
                retVal = DPM_EINVAL;
                goto exit;
            }

            /* We need to inform the DPM entities that the processing chain has asserted. */
            DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_DPC_ASSERT, sizeof(DPM_DPCAssert));
            memcpy ((void*)&message.u.assertMsg, (void*)arg, argLen);
            retVal = DPM_pipeSend (ptrDPM, &message, &errCode);
            goto exit;
        }
        case DPM_CMD_DPC_INFO:
        {
            /* Sanity Check: Validate the arguments */
            if (argLen > sizeof(DPM_DPCInfo))
            {
                /* Error: Invalid argument. */
                retVal = DPM_EINVAL;
                goto exit;
            }

            /* The processing chain has an informational message which needs to be sent out */
            DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_DPC_INFO, argLen);
            memcpy ((void*)&message.u.dpcInfoMsg, (void*)arg, argLen);
            retVal = DPM_pipeSend (ptrDPM, &message, &errCode);
            goto exit;
        }
        default:
        {
            /*************************************************************************
             * The command was not meant for the core DPM Module. We will pass this to
             * the processing chain for handling. The status of the command will be
             * reported back to the application via the reporting mechanism:-
             *
             * Sanity Check: Ensure that the message can fit within the internal buffer
             *************************************************************************/
            if (argLen > DPM_MAX_DPC_DATA)
            {
                /* Error: Invalid argument (We dont have enough space to fit the message) */
                retVal = DPM_EINVAL;
                goto exit;
            }

            /* Initialize the message:
             * - The size of the DPC Configurational messages accounts for the application
             *   provided argument length. The payload length needs to account for the
             *   the command identifier and length. */
            DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_DPC_CFG, (sizeof(DPM_DPCCfg) - DPM_MAX_DPC_DATA) + argLen);
            message.u.dpcCfgMsg.cmd    = cmd;
            message.u.dpcCfgMsg.cmdLen = argLen;
            memcpy ((void*)&message.u.dpcCfgMsg.data[0], (void*)arg, argLen);
            retVal = DPM_pipeSend (ptrDPM, &message, &errCode);
            goto exit;
        }
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to start the processing chain
 *
 *  @param[in]  handle
 *      Handle to the DPM Module
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_start (DPM_Handle handle)
{
    DPM_MCB*    ptrDPM;
    DPM_Msg     message;
    int32_t     retVal;
    int32_t     errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if (ptrDPM == NULL)
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the processing chain status
     *  - DPC should NOT be started. */
    if ((ptrDPM->procChainStatus & DPM_PROC_CHAIN_STATUS_STARTED) != 0U)
    {
        /* Error: Profile is an invalid state. Please refer to the calling API sequence. */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* We need to now inform the DPM entities that the processing chain is being started */
    DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_START, 0U);
    retVal = DPM_pipeSend (ptrDPM, &message, &errCode);
    if (retVal < 0)
    {
        /* Error: Unable to send the pipe message */
        retVal = errCode;
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to send the processing chain results to the
 *      remote DPM entities. This function is only applicable for
 *      remote & distributed domains.
 *
 *      The flag "isAckNeeded" can be set and this would cause the
 *      DPM framework to send to a report once the peer domain has
 *      been notified about the result availablity. This is useful
 *      and can be used to ensure that the result buffer is not
 *      being reused
 *
 *      \ref DPM_Report_NOTIFY_DPC_RESULT_ACKED
 *
 *  @param[in]  handle
 *      Handle to the DPM Module
 *  @param[in]  isAckNeeded
 *      Flag which indicates if an acknowledgment is needed after
 *      the results have been passed to the remote DPM entities.
 *  @param[in]  ptrResult
 *      Profile results which are to be sent
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_sendResult
(
    DPM_Handle  handle,
    bool        isAckNeeded,
    DPM_Buffer* ptrResult
)
{
    DPM_MCB*    ptrDPM;
    int32_t     retVal;
    int32_t     errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if (ptrDPM == NULL)
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* For Local Domains; the results are immediately available after the invocation
     * of the DPM Execute API. We dont need to use this to send the results */
    if (ptrDPM->initCfg.domain == DPM_Domain_LOCALIZED)
    {
        retVal = DPM_ENOTSUP;
        goto exit;
    }

    /* Sanity Checking: Domain Tables should be setup by now */
    DebugP_assert (ptrDPM->ptrDomainFxnTable != NULL);

    /* Domain Specific:
     * - This is not a relay request */
    retVal = ptrDPM->ptrDomainFxnTable->sendResult (ptrDPM, isAckNeeded, false, ptrResult, &errCode);
    if (retVal < 0)
    {
        retVal = errCode;
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to relay the partial processing chain results from one
 *      domain to another.
 *
 *      This is applicable only for processing chain developers while operating
 *      in the Distributed domain mode.
 *
 *  @param[in]  handle
 *      Handle to the DPM Module
 *  @param[in]  dpcHandle
 *      Processing Chain handle
 *  @param[in]  ptrResult
 *      Profile results which are to be sent
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_relayResult
(
    DPM_Handle          handle,
    DPM_DPCHandle       dpcHandle,
    DPM_Buffer*         ptrResult
)
{
    DPM_MCB*    ptrDPM;
    int32_t     retVal;
    int32_t     errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if ((ptrDPM == NULL) || (dpcHandle == NULL))
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* For Local/Remote Domains; the results dont need to be relayed. This
     * functionality is only available to distributed domains */
    if ((ptrDPM->initCfg.domain == DPM_Domain_LOCALIZED) ||
        (ptrDPM->initCfg.domain == DPM_Domain_REMOTE))
    {
        /* Error: Relaying is NOT supported */
        retVal = DPM_ENOTSUP;
        goto exit;
    }

    /* Sanity Checking: Domain Tables should be setup by now */
    DebugP_assert (ptrDPM->ptrDomainFxnTable != NULL);

    /* Domain Specific: We dont want an acknowlegment back for relayed results.
     * - This is a relay request */
    retVal = ptrDPM->ptrDomainFxnTable->sendResult (ptrDPM, false, true, ptrResult, &errCode);
    if (retVal < 0)
    {
        retVal = errCode;
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to stop the processing chain.
 *
 *  @param[in]  handle
 *      Handle to the DPM Module
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_stop (DPM_Handle handle)
{
    DPM_MCB*    ptrDPM;
    DPM_Msg     message;
    int32_t     retVal;
    int32_t     errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if (ptrDPM == NULL)
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* Sanity Check: Validate the processing chain status
     *  - DPC should be started. */
    if ((ptrDPM->procChainStatus & DPM_PROC_CHAIN_STATUS_STARTED) == 0U)
    {
        /* Error: DPC is an invalid state. Please refer to the calling API sequence. */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* We need to now inform the DPM entities that the processing chain is being stopped */
    DPM_msgInitHeader (ptrDPM, &message, DPM_MessageId_STOP, 0U);
    retVal = DPM_pipeSend (ptrDPM, &message, &errCode);
    if (retVal < 0)
    {
        /* Error: Unable to send over the pipe */
        retVal = errCode;
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is available to processing chain developers to
 *      notify the DPM module that the processing chain is ready to
 *      be executed. This function needs to be invoked by the DPC which
 *      in turn will allow the DPM framework to invoke the profile
 *      registered execute method.
 *
*       *Note*: DPC developers should be aware that DPM will issue DPC
*       provided execute only once even if multiple notifications have
*       been issued by the DPC prior to or while DPC's execute is going
*       on. In other words, DPM does not track multiple outstanding
*       notifications, they are all treated as a single notification.
*       So DPC will need to maintain its own execution status to handle
*       multiple notifications in a single execute method.
*
 *  @param[in]  handle
 *      Handle to the DPM Module
 *  @param[in]  dpcHandle
 *      Processing Chain handle
 *  @param[in]  isrContext
 *      Set the flag to be true if the function is being invoked from
 *      ISR context else set the flag to be false
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_notifyExecute
(
    DPM_Handle      handle,
    DPM_DPCHandle   dpcHandle,
    bool            isrContext
)
{
    DPM_MCB*    ptrDPM;
    uintptr_t   key;
    int32_t     retVal;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if ((ptrDPM == NULL) || (dpcHandle == NULL))
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /**********************************************************************
     * Critical Section: Set the flag to indicate that the DPC Execute
     * method can be invoked.
     **********************************************************************/
    key = HwiP_disable();
    ptrDPM->executeDPC = true;
    HwiP_restore(key);

    /* Post the semaphore: Accounting for the execution context */
    if (isrContext == true)
    {
        /* ISR Context: */
        SemaphoreP_postFromISR (ptrDPM->semaphoreHandle);
    }
    else
    {
        /* Task Context: */
        SemaphoreP_post (ptrDPM->semaphoreHandle);
    }

    /* Notification completed successfully: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to synchronize the execution of the framework
 *      between the DPM domains. This is an [optional] step and provides a
 *      method of synchronization if the DPM is executing on different domains.
 *
 *  @param[in]  handle
 *      Handle to the DPM
 *  @param[out] errCode
 *      Error code populated by the API on an error
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Synchronized    -   1
 *  @retval
 *      Unsynchronized  -   0
 *  @retval
 *      Error           -   <0
 */
int32_t DPM_synch (DPM_Handle handle, int32_t* errCode)
{
    DPM_MCB*    ptrDPM;
    int32_t     retVal = MINUS_ONE;

    /* Get the framework MCB: */
    ptrDPM = (DPM_MCB*)handle;
    if (ptrDPM == NULL)
    {
        *errCode = DPM_EINVAL;
        goto exit;
    }

    /* Is the DPM Instance executing in a Localized Domain? */
    if (ptrDPM->initCfg.domain == DPM_Domain_LOCALIZED)
    {
        /* YES: Localized domains are always synchronized. */
        retVal = 1;
        goto exit;
    }

    /* Sanity Checking: Domain Tables should be setup by now */
    DebugP_assert (ptrDPM->ptrDomainFxnTable != NULL);

    /* Device specific synchronization: */
    retVal = ptrDPM->ptrDomainFxnTable->synch (ptrDPM, errCode);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initialize the data path manager and
 *      initialize and load the processing chain.
 *
 *  @param[in]  ptrInitCfg
 *      Pointer to the initialization configuration
 *  @param[out] errCode
 *      Error code populated on error
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Handle to the DPM Module
 *  @retval
 *      Error   -   NULL
 */
DPM_Handle DPM_init (DPM_InitCfg* ptrInitCfg, int32_t* errCode)
{
    DPM_MCB*            ptrDPM = NULL;
    SemaphoreP_Params   semParams;
    int32_t             retVal;

    /* Initialize the error code: */
    *errCode = 0;

    /* Sanity Check: Validate the arguments */
    if ((ptrInitCfg             == NULL) ||
        (ptrInitCfg->reportFxn  == NULL) ||
        (ptrInitCfg->socHandle  == NULL))
    {
        /* Error: Invalid argument */
        *errCode = DPM_EINVAL;
        goto exit;
    }

    /* Allocate memory for the DPM module: */
    ptrDPM = MemoryP_ctrlAlloc (sizeof(DPM_MCB), 0U);
    if (ptrDPM == NULL)
    {
        *errCode = DPM_ENOMEM;
        goto exit;
    }

    /* Initialize the DPM Master control block: */
    memset ((void*)ptrDPM, 0, sizeof(DPM_MCB));

    /* Copy over the initialization configuration: */
    memcpy ((void*)&ptrDPM->initCfg, (void*)ptrInitCfg, sizeof(DPM_InitCfg));

    /* Initialize the DPM Semaphore: */
    SemaphoreP_Params_init (&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;
    ptrDPM->semaphoreHandle = SemaphoreP_create(0, &semParams);

    /* Initialize the Pipe Module: */
    if (DPM_pipeInit (ptrDPM, errCode) < 0)
    {
        /* Error: Unable to initialize the Pipe Module. */
        goto exit;
    }

    /* Setup the DPM Domain Table: */
    switch (ptrDPM->initCfg.domain)
    {
        case DPM_Domain_LOCALIZED:
        {
            /* Local Domain: These are handled via the pipes */
            break;
        }
        case DPM_Domain_REMOTE:
        {
            ptrDPM->ptrDomainFxnTable = &gDPMRemoteTable;
            break;
        }
        case DPM_Domain_DISTRIBUTED:
        {
            ptrDPM->ptrDomainFxnTable = &gDPMRemoteTable;
            break;
        }
        default:
        {
            *errCode = DPM_EINVAL;
            goto exit;
        }
    }

    /* Initialize the Domain: */
    if (ptrDPM->ptrDomainFxnTable != NULL)
    {
        if (ptrDPM->ptrDomainFxnTable->init (ptrDPM, errCode) < 0)
        {
            /* Error: Unable to initialize the Domain. */
            goto exit;
        }
    }

    /* Load and initialize the processing chain: */
    retVal = DPM_initDPC (ptrDPM, ptrDPM->initCfg.ptrProcChainCfg);
    if (retVal < 0)
    {
        /* Error: Unable to initialize the processing chain */
        *errCode = retVal;
        goto exit;
    }

exit:
    if (*errCode != 0)
    {
        /* Clean up the DPM Module */
        DPM_deinit (ptrDPM);
        ptrDPM = NULL;
    }
    return (DPM_Handle)ptrDPM;
}

/**
 *  @b Description
 *  @n
 *      The function is used to deinitialize and shutdown the processing chain
 *
 *  @param[in]  handle
 *      Handle to the DPM Module
 *
 *  \ingroup DPM_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_deinit (DPM_Handle handle)
{
    DPM_MCB*    ptrDPM;
    int32_t     retVal;
    int32_t     errCode = 0;

    /* Sanity Check: Validate the arguments */
    ptrDPM = (DPM_MCB*)handle;
    if (ptrDPM == NULL)
    {
        /* Error: Invalid argument */
        retVal = DPM_EINVAL;
        goto exit;
    }

    /* Unload processing chain which have been loaded: */
    retVal = DPM_deinitDPC (ptrDPM);
    if (retVal < 0)
    {
        /* Error: Unable to unload the processing chain */
        goto exit;
    }

    /* Deinitialize the Pipe Module: */
    if (DPM_pipeDeinit (ptrDPM, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the pipe module  */
        retVal = errCode;
        goto exit;
    }

    /* Was the domain registered? */
    if (ptrDPM->ptrDomainFxnTable != NULL)
    {
        /* YES: Domain Deinitialization */
        retVal = ptrDPM->ptrDomainFxnTable->deinit (ptrDPM, &errCode);
        if (retVal < 0)
        {
            /* Error: Unable to deinitialize the domain */
            retVal = errCode;
            goto exit;
        }
    }

    /* Cleanup the memory allocated for the DPM Module: */
    MemoryP_ctrlFree (ptrDPM, sizeof(DPM_MCB));

    /* Setup the return value */
    retVal = 0;

exit:
    return retVal;
}

