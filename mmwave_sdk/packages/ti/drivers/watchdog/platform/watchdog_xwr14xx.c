/*
 *   @file  watchdog_xwr14xx.c
 *
 *   @brief
 *      XWR14xx Watchdog configuration
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
#include <ti/drivers/watchdog/Watchdog.h>
#include <ti/drivers/watchdog/include/watchdog_internal.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/

/**
 * @brief   This is the Watchdog Driver registered function table
 */
extern Watchdog_FxnTable gWatchdogFxnTable;

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/
/**
 * @brief   This is the XWR14xx Watchdog configuration. There is only 1 Watchdog instance
 * available. This should *NOT* be modified by the customer.
 */
Watchdog_HwCfg gWatchdogHwCfg[1] =
{
    /* Watchdog Hardware configuration:
     * - Base address of memory mapped registers
     * - ESM group number
     * - ESM error number */
    {
        ((volatile RTIBRegs*)SOC_XWR14XX_MSS_RTIB_BASE_ADDRESS),
        2,
        SOC_XWR14XX_WDT_NMI_REQ_ESM
    }
};

Watchdog_Config Watchdog_config[] =
{
    {
        &gWatchdogFxnTable,             /* Watchdog Driver Function Table:     */
        (void *)NULL,                   /* Watchdog Driver Object:             */
        (void *)&gWatchdogHwCfg         /* Watchdog Hw configuration:          */
    },
    {
        NULL,
        NULL,
        NULL
    }
};
