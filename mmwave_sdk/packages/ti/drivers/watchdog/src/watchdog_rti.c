/**
 *   @file  watchdog_rti.c
 *
 *   @brief
 *      The file implements the Watchdog Driver for the XWR14xx/XWR16xx/XWR18xx/XWR68xx.
 *      The file implements the driver to conform to the MCPI standards.
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
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/watchdog/include/watchdog_internal.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/* Registered callback functions: */
static void         WatchdogRTI_clear(Watchdog_Handle handle);
static void         WatchdogRTI_close(Watchdog_Handle handle);
static int32_t      WatchdogRTI_control(Watchdog_Handle handle, uint32_t cmd, void *arg);
static void         WatchdogRTI_init(Watchdog_Handle handle);
static Watchdog_Handle   WatchdogRTI_open(Watchdog_Handle handle, Watchdog_Params *params);
/* Watchdog Driver API: */
static void         WatchdogRTI_callback (void* arg);

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   Watchdog Driver Function Table
 */
Watchdog_FxnTable gWatchdogFxnTable =
{
    &WatchdogRTI_clear,
    &WatchdogRTI_close,
    &WatchdogRTI_control,
    &WatchdogRTI_init,
    &WatchdogRTI_open
};

/**************************************************************************
 ************************* Watchdog  Driver Functions *********************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      The function is the registered ESM callback function for the Watchdog Driver.
 *      This function is used only when the reset mode is set to NMI interrupt mode.
 *
 *  @param[in]  arg
 *      Argument which is registered with the ESM module while registering the notify function.
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */

static void WatchdogRTI_callback (void* arg)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_MCB*           ptrWatchdogMCB;
    Watchdog_HwCfg*         ptrHwCfg;

    /* Get the Watchdog Configuration */
    ptrWatchdogConfig = (Watchdog_Config*)arg;

    /* Get the Watchdog Driver Object */
    ptrWatchdogMCB = (Watchdog_MCB*)ptrWatchdogConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (Watchdog_HwCfg*)ptrWatchdogConfig->hwAttrs;

    /* Increment the ISR counter */
    ptrWatchdogMCB->interruptsRxed++;

    /* Clear the status flags */
    ptrHwCfg->ptrWatchdogBase->RTIWDSTATUS = WATCHDOG_CLEAR_STATUS;

    /* Call the registered callback. */
    if (ptrWatchdogMCB->params.callbackFxn != NULL)
    {
        ptrWatchdogMCB->params.callbackFxn((Watchdog_Handle)arg);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      The function is clears the watchdog to prevent a reset signal or
 *      interrupt from being generated. Its the application's responsibilty
 *      to clear the watch dog timer.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void WatchdogRTI_clear(Watchdog_Handle handle)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_HwCfg*         ptrHwCfg;
    Watchdog_MCB*           ptrWatchdogMCB;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if (handle != NULL)
    {
        /* Get the Watchdog Configuration: */
        ptrWatchdogConfig = (Watchdog_Config*)handle;

        /* Get the Watchdog Driver Object */
        ptrWatchdogMCB = (Watchdog_MCB*)ptrWatchdogConfig->object;
        if (ptrWatchdogMCB != NULL)
        {
            ptrWatchdogMCB->watchdogCleared++;
        }
        /* Get the hardware configuration: */
        ptrHwCfg = (Watchdog_HwCfg*)ptrWatchdogConfig->hwAttrs;

        ptrHwCfg->ptrWatchdogBase->RTIWDKEY = WATCHDOG_FIRST_WDKEY;
        ptrHwCfg->ptrWatchdogBase->RTIWDKEY = WATCHDOG_SECOND_WDKEY;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog Driver instance is being closed
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void WatchdogRTI_close(Watchdog_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog Driver is being passed a control command.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *  @param[in]  cmd
 *      Watchdog command
 *  @param[in]  arg
 *      Opaque handle to the argument
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   Watchdog Error code
 */
static int32_t WatchdogRTI_control(Watchdog_Handle handle, uint32_t cmd, void *arg)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_HwCfg*         ptrHwCfg;
    Watchdog_MCB*           ptrWatchdogMCB;
    int32_t                 errCode;

    /* Initialize and setup the error code: */
    errCode = 0;

    /* Sanity Check: We need to ensure that a valid argument was passed */
    if ((handle == NULL) || (arg == NULL))
    {
        errCode = WATCHDOG_EINVAL;
    }
    else
    {
        /* Get the Watchdog Configuration: */
        ptrWatchdogConfig = (Watchdog_Config*)handle;

        /* Get the Watchdog Driver Object */
        ptrWatchdogMCB = (Watchdog_MCB*)ptrWatchdogConfig->object;

        if (ptrWatchdogMCB == NULL)
        {
            errCode = WATCHDOG_EINVAL;
        }
        else
        {
            /* Get the hardware configuration: */
            ptrHwCfg = (Watchdog_HwCfg*)ptrWatchdogConfig->hwAttrs;

            /* Processing is done on the basis of the command: */
            switch (cmd)
            {
                case WATCHDOG_CMD_RESETMODE:
                {
                    int32_t*   resetMode;

                    /* Get the reset mode from the argument */
                    resetMode = (int32_t*)arg;

                    /* Only NMI mode is supported in the DSS subsystem.
                     * Resetting the system is supported only in the MSS sysbsystem. */
#ifdef SUBSYS_DSS
                    if ((Watchdog_ResetMode)*resetMode == Watchdog_RESET_ON)
                    {
                        errCode = WATCHDOG_ENOTIMPL;
                    }
#else
                    /* Check if we need to change the reset mode */
                    if (*resetMode != ptrWatchdogMCB->params.resetMode)
                    {
                        /* Configure the reset mode */
                        ptrHwCfg->ptrWatchdogBase->RTIWWDRXNCTRL = CSL_FINSR(ptrHwCfg->ptrWatchdogBase->RTIWWDRXNCTRL, RTIWWDRXNCTRL_WWDRXN_BIT_END,
                                                                        RTIWWDRXNCTRL_WWDRXN_BIT_START, *resetMode);

                        /* Configure the reset mode */
                        ptrWatchdogMCB->params.resetMode = (Watchdog_ResetMode)*resetMode;
                    }
#endif
                    break;
                }
                case WATCHDOG_CMD_WINDOWSIZE:
                {
                    int32_t*    windowSize;

                    /* Get the window size from the argument */
                    windowSize = (int32_t*)arg;

                    /* Check if we need to change the window size */
                    if (*windowSize != ptrWatchdogMCB->params.windowSize)
                    {
                        /* Configure the window size */
                        ptrHwCfg->ptrWatchdogBase->RTIWWDSIZECTRL = *windowSize;

                        /* Configure the window size */
                        ptrWatchdogMCB->params.windowSize = (Watchdog_WindowSize)*windowSize;
                    }
                    break;
                }
                default:
                {
                    /* Error: Unsuported/Invalid command specified */
                    errCode = WATCHDOG_EINVAL;
                    break;
                }
            }
        }
    }
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog driver is initialized.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver which is to be initialized.
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void WatchdogRTI_init(Watchdog_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the Watchdog Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  handle
 *      Handle to the Watchdog Driver
 *  @param[in]  params
 *      Watchdog Parameters with which the driver is being opened
 *
 *  \ingroup WATCHDOG_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
static Watchdog_Handle WatchdogRTI_open(Watchdog_Handle handle, Watchdog_Params* params)
{
    Watchdog_Config*        ptrWatchdogConfig;
    Watchdog_MCB*           ptrWatchdogMCB;
    Watchdog_HwCfg*         ptrHwCfg;
    Watchdog_Handle         retHandle = NULL;
    ESM_NotifyParams        notifyParams;
    int32_t                 errCode, retVal;

    /* Parameter check */
    if (params->preloadValue > WATCHDOG_MAX_PRELOAD_VALUE)
    {
        goto endInit;
    }

    /* Only NMI mode is supported in the DSS subsystem.
     * Resetting the system is supported only in the MSS sysbsystem. */
#ifdef SUBSYS_DSS
    if (params->resetMode == Watchdog_RESET_ON)
    {
        goto endInit;
    }
#endif

    /* Get the Watchdog Configuration: */
    ptrWatchdogConfig = (Watchdog_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (Watchdog_HwCfg*)ptrWatchdogConfig->hwAttrs;

    /* Ensure that the driver is NOT being used: */
    if (ptrWatchdogConfig->object != NULL)
    {
        /* Error: Watchdog Driver is already being used. */
        DebugP_log1 ("Debug: Watchdog Driver(%p) has already been opened\n", ptrHwCfg->ptrWatchdogBase);
    }
    else
    {
        /* Allocate memory for the driver: */
        ptrWatchdogMCB = MemoryP_ctrlAlloc ((uint32_t)sizeof(Watchdog_MCB), 0);
        if (ptrWatchdogMCB == NULL)
        {
            /* Error: Out of memory */
            DebugP_log1 ("Debug: Watchdog Driver(%p) Out of memory\n", ptrHwCfg->ptrWatchdogBase);
        }
        else
        {
            /* Initialize the memory: */
            memset ((void *)ptrWatchdogMCB, 0, sizeof(Watchdog_MCB));

            /* Setup the return handle: */
            retHandle = handle;
        }
    }

    /* Configure the Watchdog driver. */
    if (retHandle != NULL)
    {
        /* Copy over the Watchdog Parameters */
        memcpy ((void*)&ptrWatchdogMCB->params, (void *)params, sizeof(Watchdog_Params));

        /* Write this back to the Watchdog Configuration */
        ptrWatchdogConfig->object = (void *)ptrWatchdogMCB;

        /* Bring watchdog out of reset */
        retVal = SOC_enableWatchdog(ptrWatchdogMCB->params.socHandle, &errCode);
        if (retVal < 0)
        {
            /* Cleanup the allocated memory */
            MemoryP_ctrlFree (ptrWatchdogMCB, (uint32_t)sizeof(Watchdog_MCB));
            retHandle = NULL;
            goto endInit;
        }

        /* Register to get a callback from the ESM module if NMI interrupt mode is configured */
        if (ptrWatchdogMCB->params.resetMode == Watchdog_RESET_OFF)
        {
            notifyParams.groupNumber = ptrHwCfg->groupNum;
            notifyParams.errorNumber = ptrHwCfg->errorNum;
            notifyParams.arg = (void *)ptrWatchdogConfig;
            notifyParams.notify = WatchdogRTI_callback;

#ifdef SUBSYS_DSS
            /* Unmask the Group 2 ESM errors to enable the generation of NMI. */
            retVal = SOC_configureDSSESMMask (ptrWatchdogMCB->params.socHandle, ptrHwCfg->errorNum, 0, &errCode);
            if (retVal < 0)
            {
                /* Cleanup the allocated memory */
                MemoryP_ctrlFree (ptrWatchdogMCB, (uint32_t)sizeof(Watchdog_MCB));
                retHandle = NULL;
                goto endInit;
            }
#endif
            retVal = ESM_registerNotifier (ptrWatchdogMCB->params.esmHandle, &notifyParams, &errCode);
            if (retVal < 0)
            {
                /* Cleanup the allocated memory */
                MemoryP_ctrlFree (ptrWatchdogMCB, (uint32_t)sizeof(Watchdog_MCB));
                retHandle = NULL;
                goto endInit;
            }
        }
        else
        {
            /* Configure the SOC moule to trigger a warm reset upon watchdog reset */
            retVal = SOC_triggerWarmReset(ptrWatchdogMCB->params.socHandle, &errCode);
            if (retVal < 0)
            {
                /* Cleanup the allocated memory */
                MemoryP_ctrlFree (ptrWatchdogMCB, (uint32_t)sizeof(Watchdog_MCB));
                retHandle = NULL;
                goto endInit;
            }
        }

        /* Clear the status flags */
        ptrHwCfg->ptrWatchdogBase->RTIWDSTATUS = WATCHDOG_CLEAR_STATUS;

        /* Configure the window size */
        ptrHwCfg->ptrWatchdogBase->RTIWWDSIZECTRL = ptrWatchdogMCB->params.windowSize;

        /* Configure the preload value */
        ptrHwCfg->ptrWatchdogBase->RTIDWDPRLD = CSL_FINSR(ptrHwCfg->ptrWatchdogBase->RTIDWDPRLD, RTIDWDPRLD_DWDPRLD_BIT_END,
                                                        RTIDWDPRLD_DWDPRLD_BIT_START, ptrWatchdogMCB->params.preloadValue);

        /* Configure the reset mode */
        ptrHwCfg->ptrWatchdogBase->RTIWWDRXNCTRL = CSL_FINSR(ptrHwCfg->ptrWatchdogBase->RTIWWDRXNCTRL, RTIWWDRXNCTRL_WWDRXN_BIT_END,
                                                            RTIWWDRXNCTRL_WWDRXN_BIT_START, ptrWatchdogMCB->params.resetMode);

        /* Configure the stall mode */
        ptrHwCfg->ptrWatchdogBase->RTIGCTRL = CSL_FINSR(ptrHwCfg->ptrWatchdogBase->RTIGCTRL, RTIGCTRL_COS_BIT_END,
                                                            RTIGCTRL_COS_BIT_START, ptrWatchdogMCB->params.debugStallMode);

        /* Enable the watchdog timer */
        ptrHwCfg->ptrWatchdogBase->RTIDWDCTRL = WATCHDOG_CONTROL_ENABLED;

        /* Mark the driver to be operational */
        ptrWatchdogMCB->state = Watchdog_DriverState_OPERATIONAL;
    }
endInit:
    return retHandle;
}

