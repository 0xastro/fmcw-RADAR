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
  *  ======== dma_xwr68xx.c ========
  */

#include <stdint.h>
#include <ti/drivers/dma/dma.h>
#include <ti/drivers/dma/include/dma_internal.h>
#include <ti/common/sys_common.h>


#define XWR68XX_DEFAULT_PORT_ASSIGNMENT             0x7U /*PortB*/


#ifdef SUBSYS_MSS

DMA_Driver *gDMADriverPtr[DMA_NUM_INSTANCES];

DMA_HWAttrs gDMAHwCfg[DMA_NUM_INSTANCES] =
{
    /* DMA1 Hardware configuration:
     */
    {
        0U,
        SOC_XWR68XX_MSS_DMA_1_CTRL_BASE_ADDRESS,
        SOC_XWR68XX_MSS_DMA_1_PKT_BASE_ADDRESS,
        SOC_XWR68XX_NUM_DMA_CHANNELS_PER_INSTANCE,
        SOC_XWR68XX_NUM_DMA_REQLINES_PER_INSTANCE,
        XWR68XX_DEFAULT_PORT_ASSIGNMENT,
        {
            SOC_XWR68XX_MSS_DMA1_FTC_INT,
            SOC_XWR68XX_MSS_DMA1_LFS_INT,
            SOC_XWR68XX_MSS_DMA1_HBC_INT,
            SOC_XWR68XX_MSS_DMA1_BTC_INT,
            SOC_XWR68XX_MSS_DMA1_BUS_ERR_INT
        }
    },
    /* DMA2 Hardware configuration:
     */
    {
        1U,
        SOC_XWR68XX_MSS_DMA_2_CTRL_BASE_ADDRESS,
        SOC_XWR68XX_MSS_DMA_2_PKT_BASE_ADDRESS,
        SOC_XWR68XX_NUM_DMA_CHANNELS_PER_INSTANCE,
        SOC_XWR68XX_NUM_DMA_REQLINES_PER_INSTANCE,
        XWR68XX_DEFAULT_PORT_ASSIGNMENT,
        {
            SOC_XWR68XX_MSS_DMA2_FTC_INT,
            SOC_XWR68XX_MSS_DMA2_LFS_INT,
            SOC_XWR68XX_MSS_DMA2_HBC_INT,
            SOC_XWR68XX_MSS_DMA2_BTC_INT,
            SOC_XWR68XX_MSS_DMA2_BUS_ERR_INT
        }
    }
};
#endif


