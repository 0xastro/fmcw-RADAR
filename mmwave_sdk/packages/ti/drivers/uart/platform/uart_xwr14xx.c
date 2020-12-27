/*
 *   @file  uart_xwr14xx.c
 *
 *   @brief
 *      XWR14xx UART configuration: This file is provided as a sample
 *      file but it can be customized by application developers to
 *      meet their application requirements.
 *
 *      By default the file exposes UART-1 and UART-3; but developers
 *      could only decide to use UART-3. In that case they could modify
 *      the defintion of 'UART_config'.
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
#include <ti/common/sys_common.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/uart/include/uartsci.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/

/**
 * @brief   This is the UART Driver registered function table
 */
extern UART_FxnTable gUartSciFxnTable;

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   UART1 is tied to SCI-A and UART3 is tied to SCI-B. This is XWR14xx
 * specific configuration and should *NOT* be modified by the customer.
 */
UartSci_HwCfg gUartSciHwCfg[2] =
{
    /* UART1 Hardware configuration:
     * - Capable of sending and receiving data
     * - PIN MUX is required */
    {
        ((volatile SCIRegs*)SOC_XWR14XX_MSS_SCI_A_BASE_ADDRESS),
        UartSci_Duplexity_FULL,
        UartSci_PinMux_REQUIRED,
        SOC_XWR14XX_MSS_SCIA_LVL0_INT,
        SOC_XWR14XX_MSS_SCIA_TX_DMA_REQ,
        SOC_XWR14XX_MSS_SCIA_RX_DMA_REQ,
        &UartSci_noDMAOpen,
        &UartSci_noDMAClose,
        &UartSci_noDMAIsDMAEnabled,
        &UartSci_noDMAInitiateRxDMA,
        &UartSci_noDMAInitiateTxDMA
    },

    /* UART3 Hardware configuration:
     * - Capable of only sending data
     * - PIN MUX is not required */
    {
        ((volatile SCIRegs*)SOC_XWR14XX_MSS_SCI_B_BASE_ADDRESS),
        UartSci_Duplexity_TX_ONLY,
        UartSci_PinMux_NOT_REQUIRED,
        SOC_XWR14XX_MSS_SCIB_LVL0_INT,
        SOC_XWR14XX_MSS_SCIB_TX_DMA_REQ,
        SOC_XWR14XX_MSS_SCIB_RX_DMA_REQ,
        &UartSci_noDMAOpen,
        &UartSci_noDMAClose,
        &UartSci_noDMAIsDMAEnabled,
        &UartSci_noDMAInitiateRxDMA,
        &UartSci_noDMAInitiateTxDMA
    }
};

/**
 * @brief   The XWR14xx Platform has 2 UART Modules which can be used.
 * UART1 which uses the SCI-A and UART3 which uses SCI-B. Application
 * developers can modify the configuration definition below as per their
 * requirements.
 */
UART_Config UART_config[] =
{
    {
        &gUartSciFxnTable,            /* UART SCI Driver Function Table:        */
        (void *)NULL,                 /* UART Driver Object:                    */
        (void *)&gUartSciHwCfg[0]     /* UART Hw configuration:                 */
    },
    {
        &gUartSciFxnTable,            /* UART SCI Driver Function Table:        */
        (void *)NULL,                 /* UART Driver Object:                    */
        (void *)&gUartSciHwCfg[1]     /* UART Hw configuration:                 */
    },
    {
        NULL,                         /* UART SCI Driver Function Table:        */
        NULL,                         /* UART Driver Object:                    */
        NULL                          /* UART Hw configuration:                 */
    }
};

