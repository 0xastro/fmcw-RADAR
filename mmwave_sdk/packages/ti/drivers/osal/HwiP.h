/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** @defgroup HWIP_OSAL      HwiP OSAL Porting Layer
 *
 *  @brief      Hardware Interrupt module for the RTOS Porting Interface
 *
 *  The ::HwiP_disable/::HwiP_restore APIs can be called recursively. The order
 *  of the HwiP_restore calls, must be in reversed order. For example:
 *  @code
 *  uintptr_t key1, key2;
 *  key1 = HwiP_disable();
 *  key2 = HwiP_disable();
 *  HwiP_restore(key2);
 *  HwiP_restore(key1);
 *  @endcode
 */

#ifndef ti_drivers_ports_HwiP__include
#define ti_drivers_ports_HwiP__include

/**
@defgroup HWIP_OSAL_EXTERNAL_FUNCTION               HwiP OSAL External Functions
@ingroup HWIP_OSAL
@brief
* The section documents the external API exposed by the OSAL Porting layer.
*/
/**
@defgroup HWIP_OSAL_EXTERNAL_DATA_STRUCTURES        HwiP OSAL External Data Structures
@ingroup HWIP_OSAL
@brief
*   The section has a list of all the definitions which are exposed to the developers
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/** @addtogroup HWIP_OSAL_EXTERNAL_DATA_STRUCTURES
 @{ */

/*!
 *  @brief    Opaque client reference to an instance of a HwiP
 *
 *  A HwiP_Handle returned from the ::HwiP_create represents that instance.
 */
typedef  void *HwiP_Handle;

/*!
 *  @brief    Status codes for HwiP APIs
 */
typedef enum HwiP_Status_e
{
    HwiP_OK         = 0,
    HwiP_FAILURE    = -1
} HwiP_Status;

/*!
 *  @brief    Interrupt Type for HwiP APIs
 */
typedef enum HwiP_Type_e
{
    HwiP_Type_IRQ         = 0,
    HwiP_Type_FIQ         = 1
} HwiP_Type;


/*!
 *  @brief  Prototype for the entry function for a hardware interrupt
 */
typedef void (*HwiP_Fxn)(uintptr_t arg);

/*!
 *  @brief    Basic HwiP Parameters
 *
 *  Structure that contains the parameters passed into ::HwiP_create
 *  when creating a HwiP instance. The ::HwiP_Params_init function should
 *  be used to initialize the fields to default values before the application sets
 *  the fields manually. The HwiP default parameters are noted in
 *  HwiP_Params_init.
 */
typedef struct HwiP_Params_t
{
    char      *name;      /*!< Name of the HWI instance. Memory must
                               persist for the life of the HWI instance.
                               This can be used for debugging purposes, or
                               set to NULL if not needed. */
    uintptr_t  arg;       /*!< Argument passed into the Hwi function. */
    uint32_t   priority;  /*!< Device specific priority. */
    HwiP_Type  type;      /*!< Device specific interrupt type - FIQ, IRQ (default) */
    uint8_t    enableInt; /*!< Flag to enable this interrupt when object is created. Default is true */ 
} HwiP_Params;

/**
@}
*/

/*!
 *  @brief  Function to clear a single interrupt
 *
 *  @param  interruptNum interrupt number to clear
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern void HwiP_clearInterrupt(int32_t interruptNum);

/*!
 *  @brief  Function to create an interrupt on CortexM devices
 *
 *  @param  interruptNum Interrupt Vector Id
 *
 *  @param  hwiFxn entry function of the hardware interrupt
 *
 *  @param  params    Pointer to the instance configuration parameters. NULL
 *                    denotes to use the default parameters. The HwiP default
 *                    parameters are noted in ::HwiP_Params_init.
 *
 *  @return A HwiP_Handle on success or a NULL on an error
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern HwiP_Handle HwiP_create(int32_t interruptNum, HwiP_Fxn hwiFxn,
                               HwiP_Params *params);

/*!
 *  @brief  Function to delete an interrupt on CortexM devices
 *
 *  @param  handle returned from the HwiP_create call
 *
 *  @return
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern HwiP_Status HwiP_delete(HwiP_Handle handle);

/*!
 *  @brief  Function to disable interrupts to enter a critical region
 *
 *  This function can be called multiple times, but must unwound in the reverse
 *  order. For example
 *  @code
 *  uintptr_t key1, key2;
 *  key1 = HwiP_disable();
 *  key2 = HwiP_disable();
 *  HwiP_restore(key2);
 *  HwiP_restore(key1);
 *  @endcode
 *
 *  @return A key that must be passed to HwiP_restore to re-enable interrupts.
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern uintptr_t HwiP_disable(void);

/*!
 *  @brief  Function to disable a single interrupt
 *
 *  @param  interruptNum interrupt number to disable
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern void HwiP_disableInterrupt(int32_t interruptNum);

/*!
 *  @brief  Function to enable a single interrupt
 *
 *  @param  interruptNum interrupt number to enable
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern void HwiP_enableInterrupt(int32_t interruptNum);

/*!
 *  @brief  Initialize params structure to default values.
 *
 *  The default parameters are:
 *   - name: NULL
 *   - arg: 0
 *   - priority: ~0
 *
 *  @param params  Pointer to the instance configuration parameters.
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern void HwiP_Params_init(HwiP_Params *params);

/*!
 *  @brief  Function to restore interrupts to exit a critical region
 *
 *  @param  key return from HwiP_disable
 *
 * \ingroup HWIP_OSAL_EXTERNAL_FUNCTION
 */
extern void HwiP_restore(uintptr_t key);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ports_HwiP__include */
