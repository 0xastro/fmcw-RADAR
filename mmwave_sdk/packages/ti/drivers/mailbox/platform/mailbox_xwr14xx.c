/*
 *   @file  mailbox_xwr14xx.c
 *
 *   @brief
 *      XWR14xx Mailbox configuration: This file is provided as a sample
 *      file but it can be customized by application developers to
 *      meet their application requirements.
 *
 *      By default the file exposes configurations for the MSS mailbox.
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
#include <ti/drivers/mailbox/include/mailbox_internal.h>
#include <ti/common/sys_common.h>


/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/
/**
 * @brief   This is XWR14xx specific configuration and should *NOT* be modified by the customer.
 */
Mailbox_HwCfg   gMailboxMssBssHwCfg = 
{
	(MAILBOXRegs *) SOC_XWR14XX_MSS_MBOX_MSS_BSS_REG_BASE_ADDRESS,
	(uint8_t *)     SOC_XWR14XX_MSS_MBOX_MSS_BSS_MEM_BASE_ADDRESS,
	(MAILBOXRegs *) SOC_XWR14XX_MSS_MBOX_BSS_MSS_REG_BASE_ADDRESS,
	(uint8_t *)     SOC_XWR14XX_MSS_MBOX_BSS_MSS_MEM_BASE_ADDRESS,
	                SOC_XWR14XX_MSS_SW_BSS_MSS_IRQ0_INT,
	                SOC_XWR14XX_MSS_SW_BSS_MSS_IRQ1_INT
};
