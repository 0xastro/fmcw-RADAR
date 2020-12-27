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
*  @file       dma_internal.h
*
*  @brief      DMA internal header file
*
*  @details    The DMA internal header file should be NOT be included by an application 
*  ============================================================================
*/
/**
@defgroup DMA_DRIVER_INTERNAL_FUNCTION            DMA Driver Internal Functions
@ingroup DMA_DRIVER
@brief
*   The section has a list of all the internal functions used by the driver
*/
/**
@defgroup DMA_DRIVER_INTERNAL_DATA_STRUCTURE      DMA Driver Internal Data Structures
@ingroup DMA_DRIVER
@brief
*   The section has a list of all the data structures which are used internally by the driver
*/

#ifndef DMA_INTERNAL_H
#define DMA_INTERNAL_H
    
#ifdef __cplusplus
    extern "C" {
#endif
    
#include <stdint.h>
#include <stddef.h>
#include <ti/drivers/osal/HwiP.h>

/** @addtogroup DMA_DRIVER_INTERNAL_DATA_STRUCTURE
 @{ */

/*!
 * @brief
 *  DMA Channel Map table structure
 *
 * @details
 *  DMA channel map structure for channel to reqLine mapping. One structure per channel.
 *
 */
typedef struct DMA_ChanMapTable_t {
    uint32_t            reqLine;         /*!< DMA request Line for this channel */
    uint32_t            inUse;           /*!< DMA channel mapping state */
} DMA_ChanMapTable;

/*!
 * @brief
 *  DMA Interrupt context structure
 *
 * @details
 *  DMA interrupt context structure per interrup type per channel.
 *
 */
typedef struct DMA_InterruptCtx_t {
    uint32_t                bIsEnabled;      /*!< boolean to store enable/disable status of DMA interrupt */
    DMA_IntHandlerFuncPTR   callbackFn;      /*!< DMA interrupt callback function */
    void                    *callbackArg;    /*!< DMA interrupt callback function argument */
} DMA_InterruptCtx;


/*!
 *  @brief    DMA driver internal Config
 */
typedef struct DMA_Driver_t {
    /**
     * @brief   DMA instance number
     */
    uint32_t                        instanceNum;
    /**
     * @brief   DMA instance reference (open) count
     */
    uint32_t                        refCnt;
    /**
     * @brief   DMA instance channel inUse map
     */
    uint32_t                        inUseChannelMap;
    /**
     * @brief   DMA Hardware related params 
     */
    DMA_HWAttrs         const       *hwAttrs;
    
    /**
     * @brief   Registered Interrupt Handlers.
     */
    HwiP_Handle                     hwiHandle[DMA_IntType_NUM]; 
    
    /**
     * @brief   Mapping table for Channel to request Line
     */
    DMA_ChanMapTable                *chanMapTable; /*[NUM_DMA_CHANNELS_PER_INSTANCE];*/
    
    /**
     * @brief   interrupt context for each channel each interrupt type
     */
    DMA_InterruptCtx                *interruptCtx[DMA_IntType_NUM]; /*[NUM_DMA_CHANNELS_PER_INSTANCE];*/
     
} DMA_Driver;

/** @}*/

#ifdef __cplusplus
}
#endif


#endif /* DMA_INTERNAL_H */


