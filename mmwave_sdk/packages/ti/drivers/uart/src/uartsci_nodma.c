/**
 *   @file  uartsci_nodma.c
 *
 *   @brief
 *      The file implements a NULL UART DMA Interface layer.
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
#include <string.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/uart/include/uartsci.h>

/**************************************************************************
 ************************ UART No-DMA Functions ***************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function returns the status of the DMA configuration.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      false - DMA Services are always disabled
 */
bool UartSci_noDMAIsDMAEnabled (UartSci_Driver* ptrUartSciDriver)
{
    return false;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initiate the receive DMA. For the No
 *      DMA this should never be invoked and is a stub error catching
 *      function.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  dstAddress
 *      Destination address of the buffer where the data is to be placed
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to true to indicate that we need to poll for completion else
 *      set to false
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error Code  -   <0
 */
int32_t UartSci_noDMAInitiateRxDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        dstAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    /* Control should never come here */
    DebugP_assert (0);
    return MINUS_ONE;
}

/**
 *  @b Description
 *  @n
 *      The function is used to initiate the transmit DMA. For the No
 *      DMA this should never be invoked and is a stub error catching
 *      function.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  srcAddress
 *      Source address of the buffer which is to be transmitted
 *  @param[in]  numBytes
 *      Number of bytes to be transmitted
 *  @param[in]  pollingMode
 *      Set to true to indicate that we need to poll for completion else
 *      set to false
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     -   0
 *  @retval
 *      Error Code  -   <0
 */
int32_t UartSci_noDMAInitiateTxDMA
(
    UartSci_Driver* ptrUartSciDriver,
    uint32_t        srcAddress,
    uint32_t        numBytes,
    bool            pollingMode
)
{
    /* Control should never come here */
    DebugP_assert (0);
    return MINUS_ONE;
}

/**
 *  @b Description
 *  @n
 *      The function is used to open the No-DMA. This is a stub function
 *      which always succeeds.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART SCI Driver
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART SCI Hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Always return 0
 */
int32_t UartSci_noDMAOpen (UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to close the no DMA interface layer.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART driver for which the DMA is being closed
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success - Always return 0
 */
int32_t UartSci_noDMAClose (UartSci_Driver* ptrUartSciDriver)
{
    return 0;
}


