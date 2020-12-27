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
*  @file       hwa_internal.h
*
*  @brief      HWA internal header file
*  The HWA internal header file should be _NOT_ be included by an application 
*
* ============================================================================
*/
/**
@defgroup HWA_DRIVER_INTERNAL_FUNCTION            HWA Driver Internal Functions
@ingroup HWA_DRIVER
@brief
*   The section has a list of all the internal functions used by the driver
*/
/**
@defgroup HWA_DRIVER_INTERNAL_DATA_STRUCTURE      HWA Driver Internal Data Structures
@ingroup HWA_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifndef HWA_INTERNAL_H
#define HWA_INTERNAL_H
    
#ifdef __cplusplus
    extern "C" {
#endif
    
#include <stdint.h>
#include <stddef.h>
#include <ti/drivers/osal/HwiP.h>

/** @addtogroup HWA_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

#define HWA_RAM_WINDOW_SIZE_IN_BYTES                     1024*4     /*!< Internal or Window RAM size in bytes. See \ref HWA_configRam */


/*!
 * @brief
 *  HWA Interrupt context structure for paramset done interrupt
 *
 * @details
 *  HWA interrupt context structure per paramSet.
 *
 */
typedef struct HWA_InterruptCtx_t {
    bool                                bIsEnabled;      /*!< boolean to store enable/disable status of HWA interrupt */
    HWA_ParamDone_IntHandlerFuncPTR     callbackFn;      /*!< HWA interrupt callback function */
    void                                *callbackArg;    /*!< HWA interrupt callback function argument */
} HWA_InterruptCtx;

/*!
 * @brief
 *  HWA Interrupt context structure for done interrupt
 *
 * @details
 *  HWA interrupt context structure for done interrupt which is triggered when all paramsets are executed
 *
 */
typedef struct HWA_DoneInterruptCtx_t {
    bool                                bIsEnabled;      /*!< boolean to store enable/disable status of HWA interrupt */
    HWA_Done_IntHandlerFuncPTR          callbackFn;      /*!< HWA interrupt callback function */
    void                                *callbackArg;    /*!< HWA interrupt callback function argument */
} HWA_DoneInterruptCtx;

/*!
 *  @brief    HWA driver internal Config
 */
typedef struct HWA_Driver_t {
    /**
     * @brief   HWA instance number
     */
    uint32_t                        instanceNum;
    /**
     * @brief   HWA instance reference (open) count
     */
    uint32_t                        refCnt;
    /**
     * @brief   HWA instance config is in progress. 
     *          Protects Common register acccess in HWA_configCommon() and HWA_configRam().
     */
    uint8_t                        configInProgress;
    /**
     * @brief   HWA paramset config is in progress [used as bitmap].
     *          Protects Paramset register access in HWA_configParamSet(),
     *          HWA_enableParamSetInterrupt() and HWA_disableParamSetInterrupt()
     */
    uint16_t                       paramSetMapInProgress;
    /**
     * @brief   HWA Hardware related params 
     */
    HWA_HWAttrs         const       *hwAttrs;
    
    /**
     * @brief   Registered Interrupt Handler for each paramset completion
     */
    HwiP_Handle                     hwiHandleParamSet; 
    
    /**
     * @brief   Registered Interrupt Handler for interrupt at the end of group of paramsets 
     */
    HwiP_Handle                     hwiHandleDone; 
    
    /**
     * @brief   SOC handle passed by the application 
     */
    SOC_Handle                      socHandle;
    
    /**
     * @brief   interrupt context for each paramset
     */
    HWA_InterruptCtx                *interruptCtxParamSet; /*[NUM_HWA_PARAMSETS_PER_INSTANCE];*/
    
    /**
     * @brief   interrupt context for all paramset done interrupt
     */
    HWA_DoneInterruptCtx             interruptCtxDone; 
} HWA_Driver;

/** @}*/


#ifdef __cplusplus
}
#endif


#endif /* HWA_INTERNAL_H */


