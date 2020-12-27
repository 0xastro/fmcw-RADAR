/**
 *   @file  edma_xwr1xxx.c
 *
 *   @brief
 *      XWR1xxx edma configuration.
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
#include <ti/drivers/edma/edma.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/edma/include/edma_xwr1xxx.h>

/*! @brief EDMA hardware attributes global. */
const EDMA_hwAttrs_t gEdmaHwAttrs[EDMA_NUM_CC] = {
#if defined(SOC_XWR14XX)
    {
        .CCbaseAddress      = EDMA_CC0_BASE_ADDRESS,
        .TCbaseAddress[0]   = EDMA_CC0_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_CC0_TC1_BASE_ADDRESS,
        .numEventQueues = EDMA_NUM_TC,
        .numParamSets       = EDMA_NUM_PARAM_SETS,
        .transferCompletionInterruptNum = EDMA_CC0_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum = EDMA_CC0_ERRROR_INTR_ID,
        .isChannelMapExist = false,
        .transferControllerErrorInterruptNum[0] = EDMA_CC0_TC0_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_CC0_TC1_ERRROR_INTR_ID
    }
#endif

#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
    {
        .CCbaseAddress      = EDMA_CC0_BASE_ADDRESS,
        .TCbaseAddress[0]   = EDMA_CC0_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_CC0_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_NUM_TC_IN_CC0,
        .numParamSets       = EDMA_NUM_PARAM_SETS_CC0,
        .transferCompletionInterruptNum = EDMA_CC0_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum  = EDMA_CC0_ERRROR_INTR_ID,
        .isChannelMapExist = false,
        .transferControllerErrorInterruptNum[0] = EDMA_CC0_TC0_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_CC0_TC1_ERRROR_INTR_ID
    },
    {
        .CCbaseAddress      = EDMA_CC1_BASE_ADDRESS,
        .TCbaseAddress[0]   = EDMA_CC1_TC0_BASE_ADDRESS,
        .TCbaseAddress[1]   = EDMA_CC1_TC1_BASE_ADDRESS,
        .numEventQueues     = EDMA_NUM_TC_IN_CC1,
        .numParamSets       = EDMA_NUM_PARAM_SETS_CC1,
        .isChannelMapExist = false,
#if defined(SUBSYS_DSS)
        .transferCompletionInterruptNum = EDMA_CC1_TRANSFER_COMPLETE_INTR_ID,
        .errorInterruptNum  = EDMA_CC1_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_CC1_TC0_ERRROR_INTR_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_CC1_TC1_ERRROR_INTR_ID
#elif defined(SUBSYS_MSS)
        .transferCompletionInterruptNum = EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .errorInterruptNum  = EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[0] = EDMA_INTERRUPT_NOT_CONNECTED_ID,
        .transferControllerErrorInterruptNum[1] = EDMA_INTERRUPT_NOT_CONNECTED_ID
#endif
}
#endif
};


