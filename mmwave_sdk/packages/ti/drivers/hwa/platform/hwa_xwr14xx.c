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

 /*
  *  ======== hwa_xwr14xx.c ========
  */

#include <stdint.h>
#include <ti/drivers/hwa/hwa.h>
#include <ti/drivers/hwa/include/hwa_internal.h>
#include <ti/common/sys_common.h>


#define XWR14XX_NUM_HWA_PARAMSETS_PER_INSTANCE       16
#define XWR14XX_NUM_HWA_DMACHANNELS_PER_INSTANCE     16


/* storage for HWA driver object handles */
HWA_Driver *gHWADriverPtr[HWA_NUM_INSTANCES] = { NULL };

/* XWR14xx specific HWA hardware attributes */
HWA_HWAttrs gHWAHwCfg[HWA_NUM_INSTANCES] =
{
    /* HWA1 Hardware configuration:
     */
    {
        0U,
        SOC_XWR14XX_MSS_HWA_1_COMMON_BASE_ADDRESS,
        SOC_XWR14XX_MSS_HWA_1_PARAM_BASE_ADDRESS,
        SOC_XWR14XX_MSS_HWA_1_RAM_BASE_ADDRESS,
        SOC_XWR14XX_DSS_REG_BASE_ADDRESS,
        XWR14XX_NUM_HWA_PARAMSETS_PER_INSTANCE,
        SOC_XWR14XX_DSS_HW_ACC_PARAM_DONE_IRQ,
        SOC_XWR14XX_DSS_HW_ACC_DONE_IRQ,
        XWR14XX_NUM_HWA_DMACHANNELS_PER_INSTANCE,
        SOC_XWR14XX_MSS_HWA_MEM0_BASE_ADDRESS,
        SOC_XWR14XX_MSS_HWA_MEM_SIZE
    }
};
