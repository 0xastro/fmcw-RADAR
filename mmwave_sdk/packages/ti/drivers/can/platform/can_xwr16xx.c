/*
 *   @file  can_xwr16xx.c
 *
 *   @brief
 *      XWR16xx CAN controller configuration. This is XWR16xx specific
 *      configuration and should *NOT* be modified by the customer.
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
#include <ti/drivers/can/include/can_internal.h>
#include <ti/common/sys_common.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/


/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   Platform specific CAN initializations. This is XWR16xx
 * specific configuration and should *NOT* be modified by the customer.
 */
CAN_HwCfg gCanHwCfg[1] =
{
    /* DCAN Hardware configuration:
     * - CAN controller register Base Address
     * - CAN message RAM memory Base Address
     * - CAN Interrupt 0
     * - CAN Interrupt 1
     */
    {
        SOC_XWR16XX_MSS_DCAN_BASE_ADDRESS,
        SOC_XWR16XX_MSS_DCAN_MEM_BASE_ADDRESS,
        SOC_XWR16XX_MSS_DCAN_LVL0_INT,
        SOC_XWR16XX_MSS_DCAN_LVL1_INT
    }
};

