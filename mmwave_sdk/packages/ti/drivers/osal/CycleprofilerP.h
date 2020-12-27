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
/** ============================================================================
 *  @defgroup CYCLEPROFILERP_OSAL     CycleprofilerP OSAL Porting Layer
 *
 *  @brief      Cycle profiler module to measure CPU cycles
 *
 *  Cycle profiler module gives APIs that allows a segment of code to be profiled
 *  with the APIs to measure CPU cycles.
 *  ============================================================================
 */

#ifndef ti_drivers_ports_CycleprofilerP__include
#define ti_drivers_ports_CycleprofilerP__include

/**
@defgroup CYCLEPROFILERP_OSAL_EXTERNAL_FUNCTION            CycleprofilerP OSAL External Functions
@ingroup CYCLEPROFILERP_OSAL
@brief
* The section documents the external API exposed by the OSAL Porting layer.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*!
 *  @brief  Function to initialize cycle profiler
 *
 *  \ingroup CYCLEPROFILERP_OSAL_EXTERNAL_FUNCTION
 */
extern void CycleprofilerP_init(void);

/*!
 *  @brief  Function to delete a semaphore.
 *
 *  @return Get CPU cycle count time stamp
 *
 *  \ingroup CYCLEPROFILERP_OSAL_EXTERNAL_FUNCTION
 */
extern uint32_t CycleprofilerP_getTimeStamp(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_ports_CycleprofilerP__include */
