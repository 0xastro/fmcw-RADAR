/**
 *   @file  watchdog_internal.h
 *
 *   @brief
 *      This is the internal Header for the Watchdog Driver. This header
 *      file should *NOT* be directly included by applications.
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

#ifndef WATCHDOG_INTERNAL_H
#define WATCHDOG_INTERNAL_H

#include <ti/drivers/osal/HwiP.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/watchdog/Watchdog.h>
#include <ti/drivers/watchdog/include/reg_watchdog.h>

#define WATCHDOG_CONTROL_ENABLED                (0xA98559DAU)
#define WATCHDOG_CONTROL_DISABLED               (0x5312ACEDU)
#define WATCHDOG_CLEAR_STATUS                   (0xFFU)
#define WATCHDOG_FIRST_WDKEY                    (0xE51AU)
#define WATCHDOG_SECOND_WDKEY                   (0xA35CU)
#define WATCHDOG_MAX_PRELOAD_VALUE              (0xFFFU)

/**
@defgroup WATCHDOG_INTERNAL_FUNCTION             Watchdog Driver Internal Functions
@ingroup WATCHDOG_DRIVER
@brief
*   The section has a list of all the internal functions used by the driver
*/

/**
@defgroup WATCHDOG_INTERNAL_DATA_STRUCTURE       Watchdog Driver Internal Data Structures
@ingroup WATCHDOG_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup WATCHDOG_INTERNAL_DATA_STRUCTURE
 @{ */

/**
 * @brief
 *  Watchdog Driver Status
 *
 * @details
 *  The enumeration describes the status of the Watchdog Driver Instance
 */
typedef enum Watchdog_DriverState_t
{
    /**
     * @brief   Driver is uninitialized.
     */
    Watchdog_DriverState_UNINIT,

    /**
     * @brief   Driver is operational.
     */
    Watchdog_DriverState_OPERATIONAL
}Watchdog_DriverState;

/**
 * @brief
 *  Watchdog Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 */
typedef struct Watchdog_HwCfg_t
{
    /**
     * @brief   Base address of the Watchdog address space to be used.
     */
    RTIBRegs*           ptrWatchdogBase;

    /**
     * @brief   Group Number to which the ESM error belongs
     */
    uint32_t            groupNum;

    /**
     * @brief   ESM error Number
     */
    uint32_t            errorNum;
}Watchdog_HwCfg;

/**
 * @brief
 *  Watchdog Driver Master Control Block
 *
 * @details
 *  The structure is used to hold all the pertinent information with respect
 *  to the Watchdog Driver.
 */
typedef struct Watchdog_MCB_t
{
    /**
     * @brief   Watchdog driver internal state
     */
    Watchdog_DriverState         state;

    /**
     * @brief   Watchdog Parameters which were used to initialize the driver instance
     */
    Watchdog_Params              params;

    /**
     * @brief   Registered Interrupt Handler.
     */
    HwiP_Handle                 hwiHandle;

    /**
     * @brief   Number of interrupts received. Valid only when resetMode = Watchdog_RESET_OFF
     */
    uint32_t                    interruptsRxed;

    /**
     * @brief   Number of times watchdog was serviced
     */
    uint32_t                    watchdogCleared;
} Watchdog_MCB;

/**
@}
*/

#ifdef __cplusplus
}
#endif

#endif /* WATCHDOG_INTERNAL_H */
