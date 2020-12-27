/*
 *   @file  uart_xwr18xx.c
 *
 *   @brief
 *      XWR18xx UART configuration: This file is provided as a sample
 *      file but it can be customized by application developers to
 *      meet their application requirements.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2017 Texas Instruments, Inc.
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

/**************************************************************************
 * On the XWR18xx: The UART is available on both the DSS and MSS. The UART
 * platform configuration is MSS and DSS specific
 **************************************************************************/

#ifdef SUBSYS_MSS

/**
 * @brief   This is the XWR18xx MSS specific UART configuration. There are
 * 2 UART instances available on the MSS. UART1 is tied to SCI-A and UART3 is
 * tied to SCI-B. This should *NOT* be modified by the customer.
 */
UartSci_HwCfg gUartSciHwCfg[2] =
{
    /* UART1 Hardware configuration:
     * - Capable of sending and receiving data
     * - PIN MUX is required */
    {
        ((volatile SCIRegs*)SOC_XWR18XX_MSS_SCI_A_BASE_ADDRESS),
        UartSci_Duplexity_FULL,
        UartSci_PinMux_REQUIRED,
        SOC_XWR18XX_MSS_SCIA_LVL0_INT,
        SOC_XWR18XX_MSS_SCIA_TX_DMA_REQ,
        SOC_XWR18XX_MSS_SCIA_RX_DMA_REQ,
        &UartSci_openDMA,
        &UartSci_closeDMA,
        &UartSci_isDMAEnabled,
        &UartSci_initiateRxDMA,
        &UartSci_initiateTxDMA
    },

    /* UART3 Hardware configuration:
     * - Capable of only sending data
     * - PIN MUX is required */
    {
        ((volatile SCIRegs*)SOC_XWR18XX_MSS_SCI_B_BASE_ADDRESS),
        UartSci_Duplexity_TX_ONLY,
        UartSci_PinMux_REQUIRED,
        SOC_XWR18XX_MSS_SCIB_LVL0_INT,
        SOC_XWR18XX_MSS_SCIB_TX_DMA_REQ,
        SOC_XWR18XX_MSS_SCIB_RX_DMA_REQ,
        &UartSci_openDMA,
        &UartSci_closeDMA,
        &UartSci_isDMAEnabled,
        &UartSci_initiateRxDMA,
        &UartSci_initiateTxDMA
    }
};

/**
 * @brief   The MSS on the XWR18xx Platform has 2 UART Modules which can be used.
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

#elif defined (SUBSYS_DSS)

/**
 * @brief   This is the XWR18xx DSS specific UART configuration. There is only 1 available
 * UART instance. This should *NOT* be modified by the customer.
 */
UartSci_HwCfg gUartSciHwCfg[1] =
{
    /* UART2 Hardware configuration:
     * - Capable of sending data only
     * - PIN MUX is required */
    {
        ((volatile SCIRegs*)SOC_XWR18XX_DSS_SCI_BASE_ADDRESS),
        UartSci_Duplexity_TX_ONLY,
        UartSci_PinMux_REQUIRED,
        SOC_XWR18XX_DSS_INTC_EVENT_UART_REQ0,
        EDMA_TPCC1_REQ_UART_TX,
        EDMA_TPCC1_REQ_UART_RX,
        &UartSci_openEDMA,
        &UartSci_closeEDMA,
        &UartSci_isEDMAEnabled,
        &UartSci_initiateRxEDMA,
        &UartSci_initiateTxEDMA
    }
};

/**
 * @brief   The DSS on the XWR18xx Platform has 1 UART Modules which can be used.
 */
UART_Config UART_config[] =
{
    {
        &gUartSciFxnTable,            /* UART SCI Driver Function Table:        */
        (void *)NULL,                 /* UART Driver Object:                    */
        (void *)&gUartSciHwCfg[0]     /* UART Hw configuration:                 */
    },
    {
        NULL,                         /* UART SCI Driver Function Table:        */
        NULL,                         /* UART Driver Object:                    */
        NULL                          /* UART Hw configuration:                 */
    }
};

#else
#error "Error: Please check the compiler flags since SUBSYS_XXX is not defined for the XWR18xx device"
#endif

