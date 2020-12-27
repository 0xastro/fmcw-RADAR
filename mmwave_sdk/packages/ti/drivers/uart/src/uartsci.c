/**
 *   @file  uartsci.c
 *
 *   @brief
 *      The file implements the UART Serial Communication Interface Driver
 *      for the XWR14xx and XWR16xx. The file implements the driver to conform
 *      to the MCPI standards.
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
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/uart/include/uartsci.h>

/**************************************************************************
 ************************** Local Definitions *****************************
 **************************************************************************/

/* Registered callback functions: */
static void         UartSci_close(UART_Handle handle);
static void         UartSci_init(UART_Handle handle);
static UART_Handle  UartSci_open(UART_Handle handle, UART_Params *params);
static int32_t      UartSci_read(UART_Handle handle, uint8_t *buffer, uint32_t size);
static int32_t      UartSci_readPolling(UART_Handle handle, uint8_t *buffer, uint32_t size);
static void         UartSci_readCancel(UART_Handle handle);
static int32_t      UartSci_write(UART_Handle handle, uint8_t *buffer, uint32_t size);
static int32_t      UartSci_writePolling(UART_Handle handle, uint8_t* buffer, uint32_t size);
static void         UartSci_writeCancel(UART_Handle handle);
static int32_t      UartSci_control(UART_Handle handle, uint32_t cmd, void *arg);

/* Read/Write SCI Registers API: */
static void     UartSci_enableTxInterrupt (volatile SCIRegs* ptrSCIRegs);
static void     UartSci_disableTxInterrupt (volatile SCIRegs* ptrSCIRegs);
static uint32_t UartSci_isTxInterruptEnabled (const volatile SCIRegs* ptrSCIRegs);
static void     UartSci_enableRxInterrupt (volatile SCIRegs* ptrSCIRegs);
static void     UartSci_disableRxInterrupt (volatile SCIRegs* ptrSCIRegs);
static uint32_t UartSci_isRxInterruptEnabled (const volatile SCIRegs* ptrSCIRegs);
static uint32_t UartSci_isRxFree (const volatile SCIRegs* ptrSCIRegs);
static uint32_t UartSci_isTxFree (const volatile SCIRegs* ptrSCIRegs);
static uint32_t UartSci_isRxOverrun (const volatile SCIRegs* ptrSCIRegs);
static void     UartSci_clearRxOverrun (volatile SCIRegs* ptrSCIRegs);
static uint32_t UartSci_isParityError (const volatile SCIRegs* ptrSCIRegs);
static void     UartSci_clearParityError (volatile SCIRegs* ptrSCIRegs);
static uint32_t UartSci_isFramingError (const volatile SCIRegs* ptrSCIRegs);
static void     UartSci_clearFramingError (volatile SCIRegs* ptrSCIRegs);
static uint8_t  UartSci_getCh (const volatile SCIRegs* ptrSCIRegs);
static void     UartSci_putCh (volatile SCIRegs* ptrSCIRegs, uint8_t ch);

/* SCI Driver API: */
static void    UartSci_ISR (uintptr_t arg);
static int32_t UartSci_validateParams (UART_Params* params, UartSci_HwCfg* ptrHwCfg);
static void    UartSci_writeChar(UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg);
static void    UartSci_readChar(UartSci_Driver* ptrUartSciDriver, UartSci_HwCfg* ptrHwCfg);

/**************************************************************************
 ************************** Local Declarations ****************************
 **************************************************************************/

/**
 * @brief   Global Constant CR
 */
static const uint8_t   CONST_CR =  (uint8_t)'\r';

/**
 * @brief   Global Constant LF
 */
static const uint8_t   CONST_LF =  (uint8_t)'\n';

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

/**
 * @brief   UART-SCI Driver Function Table
 */
UART_FxnTable gUartSciFxnTable =
{
    &UartSci_close,
    &UartSci_control,
    &UartSci_init,
    &UartSci_open,
    &UartSci_read,
    &UartSci_readPolling,
    &UartSci_readCancel,
    &UartSci_write,
    &UartSci_writePolling,
    &UartSci_writeCancel,
};

/**************************************************************************
 ************************* UART SCI Driver Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the transmit interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_enableTxInterrupt (volatile SCIRegs* ptrSCIRegs)
{
    ptrSCIRegs->SCISETINT = CSL_FINSR (ptrSCIRegs->SCISETINT, 8U, 8U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to check if the transmit interrupt
 *      for the specific SCI Instance is enabled or not
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Interrupt is enabled
 *  @retval
 *      0   -   Interrupt is disabled
 */
static uint32_t UartSci_isTxInterruptEnabled (const volatile SCIRegs* ptrSCIRegs)
{
    return CSL_FEXTR (ptrSCIRegs->SCISETINT, 8U, 8U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the transmit DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_enableTxDMA (volatile SCIRegs* ptrSCIRegs)
{
    /* Enable Transmit DMA */
    ptrSCIRegs->SCISETINT = CSL_FINSR (ptrSCIRegs->SCISETINT, 16U, 16U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the transmit DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disableTxDMA (volatile SCIRegs* ptrSCIRegs)
{
    /* Disable the Transmit DMA */
    ptrSCIRegs->SCICLEARINT = CSL_FINSR (ptrSCIRegs->SCICLEARINT, 16U, 16U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the transmit interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_disableTxInterrupt (volatile SCIRegs* ptrSCIRegs)
{
    ptrSCIRegs->SCICLEARINT = CSL_FMKR (8U, 8U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the receive interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_enableRxInterrupt (volatile SCIRegs* ptrSCIRegs)
{
    ptrSCIRegs->SCISETINT = CSL_FINSR (ptrSCIRegs->SCISETINT, 9U, 9U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to check if the transmit interrupt
 *      for the specific SCI Instance is enabled or not
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Interrupt is enabled
 *  @retval
 *      0   -   Interrupt is disabled
 */
static uint32_t UartSci_isRxInterruptEnabled (const volatile SCIRegs* ptrSCIRegs)
{
    return CSL_FEXTR (ptrSCIRegs->SCISETINT, 9U, 9U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the receive interrupt
 *      for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_disableRxInterrupt (volatile SCIRegs* ptrSCIRegs)
{
    ptrSCIRegs->SCICLEARINT = CSL_FMKR (9U, 9U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to enable the receive DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_enableRxDMA (volatile SCIRegs* ptrSCIRegs)
{
    /* Enable the Rx DMA All and Rx DMA */
    ptrSCIRegs->SCISETINT = CSL_FINSR (ptrSCIRegs->SCISETINT, 18U, 18U, 1U);
    ptrSCIRegs->SCISETINT = CSL_FINSR (ptrSCIRegs->SCISETINT, 17U, 17U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to disable the receive DMA
 *      functionality for the specific SCI Instance
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
void UartSci_disableRxDMA (volatile SCIRegs* ptrSCIRegs)
{
    /* Disable the Rx DMA All and Rx DMA */
    ptrSCIRegs->SCICLEARINT = CSL_FINSR (ptrSCIRegs->SCICLEARINT, 18U, 18U, 1U);
    ptrSCIRegs->SCICLEARINT = CSL_FINSR (ptrSCIRegs->SCICLEARINT, 17U, 17U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if the SCI receiver
 *      has been overrun or not?
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Receiver overrun has been detected
 *  @retval
 *      0   -   Receiver overrun not detected
 */
static uint32_t UartSci_isRxOverrun (const volatile SCIRegs* ptrSCIRegs)
{
    return CSL_FEXTR(ptrSCIRegs->SCIFLR, 25U, 25U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear the SCI receiver.
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_clearRxOverrun (volatile SCIRegs* ptrSCIRegs)
{
    /* Write a 1 to the Overrun bit will clear the status of the overrun */
    ptrSCIRegs->SCIFLR = CSL_FMKR (25U, 25U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if there is a
 *      framing error or not?
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Framing error has been detected
 *  @retval
 *      0   -   Framing error has not been detected
 */
static uint32_t UartSci_isFramingError (const volatile SCIRegs* ptrSCIRegs)
{
    return CSL_FEXTR(ptrSCIRegs->SCIFLR, 26U, 26U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear the framing error
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_clearFramingError (volatile SCIRegs* ptrSCIRegs)
{
    /* Write a 1 to the Overrun bit will clear the status of the overrun */
    ptrSCIRegs->SCIFLR = CSL_FMKR (26U, 26U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if there is a
 *      parity error or not?
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Parity error has been detected
 *  @retval
 *      0   -   Parity error has not been detected
 */
static uint32_t UartSci_isParityError (const volatile SCIRegs* ptrSCIRegs)
{
    return CSL_FEXTR(ptrSCIRegs->SCIFLR, 24U, 24U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to clear the parity error
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_clearParityError (volatile SCIRegs* ptrSCIRegs)
{
    /* Write a 1 to the Parity Error bit will clear the status */
    ptrSCIRegs->SCIFLR = CSL_FMKR (24U, 24U, 1U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if the SCI receiver
 *      is free or not.
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Receiver is free
 *  @retval
 *      0   -   Receiver is not free
 */
static uint32_t UartSci_isRxFree (const volatile SCIRegs* ptrSCIRegs)
{
    return CSL_FEXTR(ptrSCIRegs->SCIFLR, 9U, 9U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to determine if the SCI transmitter
 *      is free or not.
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      1   -   Transmitter is free
 *  @retval
 *      0   -   Transmitter is not free
 */
static uint32_t UartSci_isTxFree (const volatile SCIRegs* ptrSCIRegs)
{
    return CSL_FEXTR(ptrSCIRegs->SCIFLR, 8U, 8U);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to put a character
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *  @param[in]  ch
 *      Character to be placed
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_putCh (volatile SCIRegs* ptrSCIRegs, uint8_t ch)
{
    ptrSCIRegs->SCITD = CSL_FINSR (ptrSCIRegs->SCITD, 7U, 0U, (uint32_t)ch);
}

/**
 *  @b Description
 *  @n
 *      Utility function which is used to get a character
 *
 *  @param[in]  ptrSCIRegs
 *      Pointer to the SCI Base
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Character which is read from the SCI Register
 */
static uint8_t UartSci_getCh (const volatile SCIRegs* ptrSCIRegs)
{
    return (uint8_t)(CSL_FEXTR (ptrSCIRegs->SCIRD, 7U, 0U));
}

/**
 *  @b Description
 *  @n
 *      The function is used to write the character from the write buffer
 *      on the UART.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART Driver Hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_writeChar
(
    UartSci_Driver*   ptrUartSciDriver,
    UartSci_HwCfg*    ptrHwCfg
)
{
    /* Is the UART Driver operating in TEXT/BINARY Mode? */
    if (ptrUartSciDriver->params.writeDataMode == UART_DATA_TEXT)
    {
        /* TEXT Mode: */
        if (ptrUartSciDriver->writeCR)
        {
            /* Write the return character */
            UartSci_putCh (ptrHwCfg->ptrSCIRegs, CONST_CR);

            /* Write size is incremented an additional time on the detection of '\n'
             * So we decrement it back again here. */
            ptrUartSciDriver->writeSize--;
            ptrUartSciDriver->writeCount++;
            ptrUartSciDriver->writeCR = 0;
        }
        else
        {
            /* Add a return if next character is a newline. */
            if (*(char *)ptrUartSciDriver->ptrWriteBuffer == CONST_LF)
            {
               ptrUartSciDriver->writeSize++;
               ptrUartSciDriver->writeCR = 1;
            }

            /* Write the character to the transmit buffer: */
            UartSci_putCh (ptrHwCfg->ptrSCIRegs, *(uint8_t*)ptrUartSciDriver->ptrWriteBuffer);

            /* Move to the next character */
            ptrUartSciDriver->ptrWriteBuffer = (uint8_t *)ptrUartSciDriver->ptrWriteBuffer + 1;
            ptrUartSciDriver->writeSize--;
            ptrUartSciDriver->writeCount++;
        }
    }
    else
    {
        /* Binary Mode: */
        UartSci_putCh (ptrHwCfg->ptrSCIRegs, *(uint8_t *)ptrUartSciDriver->ptrWriteBuffer);

        /* Move to the next character */
        ptrUartSciDriver->ptrWriteBuffer = (uint8_t *)ptrUartSciDriver->ptrWriteBuffer + 1;
        ptrUartSciDriver->writeSize--;
        ptrUartSciDriver->writeCount++;
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to read the character and place this into the
 *      read buffer.
 *
 *  @param[in]  ptrUartSciDriver
 *      Pointer to the UART Driver Instance
 *  @param[in]  ptrHwCfg
 *      Pointer to the UART Driver Hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_readChar
(
    UartSci_Driver* ptrUartSciDriver,
    UartSci_HwCfg*  ptrHwCfg
)
{
    uint8_t readIn;

    /* Read the char: */
    readIn = UartSci_getCh (ptrHwCfg->ptrSCIRegs) >> ptrUartSciDriver->shiftJustification;

    /* Is the UART Driver operating in TEXT/BINARY Mode? */
    if (ptrUartSciDriver->params.readDataMode == UART_DATA_TEXT)
    {
        /* TEXT Mode: */
        if (readIn == CONST_CR)
        {
            /* Do we need to echo character? */
            if (ptrUartSciDriver->params.readEcho)
            {
                /* YES: Loop around and wait until TX is ready */
                while (1)
                {
                    if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
                    {
                        UartSci_putCh (ptrHwCfg->ptrSCIRegs, CONST_CR);
                        break;
                    }
                }
            }
            readIn = CONST_LF;
        }
    }

    /* Do we need to echo character? */
    if (ptrUartSciDriver->params.readEcho)
    {
        /* YES: Loop around and wait until TX is ready */
        while (1)
        {
            if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
            {
                UartSci_putCh (ptrHwCfg->ptrSCIRegs, readIn);
                break;
            }
        }
    }

    /* Place the character into the receive buffer and increment the various counters */
    *(uint8_t *)ptrUartSciDriver->ptrReadBuffer = readIn;
    ptrUartSciDriver->ptrReadBuffer = (uint8_t *)ptrUartSciDriver->ptrReadBuffer + 1;
    ptrUartSciDriver->readCount++;
    ptrUartSciDriver->readSize--;

    /* Is the read complete? */
    if ((ptrUartSciDriver->params.readReturnMode == UART_RETURN_NEWLINE) && (readIn == CONST_LF))
    {
        /* YES: In return mode; NEWLINE we can stop the reception operation when we receive
         * a new line character. */
        ptrUartSciDriver->readSize = 0;
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the UART SCI Driver.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_ISR (uintptr_t arg)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)arg;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Increment the total number of interrupts received. */
    ptrUartSciDriver->stats.totalInterrupts++;

    /* Are receive interrupts enabled? Only then do we handle them in the ISR context. */
    if (UartSci_isRxInterruptEnabled (ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the total number of receive interrupts: */
        ptrUartSciDriver->stats.numRxInterrupts++;

        /* Is there a Rx Interrupt? */
        if (UartSci_isRxFree(ptrHwCfg->ptrSCIRegs) == 1U)
        {
            /* YES: Do we have a valid data buffer where we need to place the data? */
            if (ptrUartSciDriver->readSize > 0)
            {
                /* Read the character: */
                UartSci_readChar (ptrUartSciDriver, ptrHwCfg);

                /* Sanity Check: The read size can never be negative */
                DebugP_assert (ptrUartSciDriver->readSize >= 0);

                /* Are we done with the read buffer ? */
                if (ptrUartSciDriver->readSize == 0)
                {
                    /* YES: Do we need to post a semaphore? */
                    if (ptrUartSciDriver->readSem)
                    {
                        /* YES: Read was operating in blocking mode. Post the semaphore to
                         * wakeup the calling thread since all the data has been received */
                        SemaphoreP_postFromISR (ptrUartSciDriver->readSem);
                    }

                    /* Disable RX interrupt until we do a new read */
                    UartSci_disableRxInterrupt(ptrHwCfg->ptrSCIRegs);
                }
            }
            else
            {
                /* We received a character but there was no application provided buffer. We
                 * cannot keep this. */
                ptrUartSciDriver->stats.numDummyRead++;

                /* Dummy read and drop the received character */
                UartSci_getCh (ptrHwCfg->ptrSCIRegs);
            }
        }
    }

    /* Are transmit interrupts enabled? Only then do we handle them in the ISR context. */
    if (UartSci_isTxInterruptEnabled (ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the total number of transmit interrupts: */
        ptrUartSciDriver->stats.numTxInterrupts++;

        /* Is there a Tx Interrupt? */
        if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
        {
            /* YES: Is there any data which needs to be written? */
            if (ptrUartSciDriver->writeSize > 0)
            {
                /* Write the character: */
                UartSci_writeChar (ptrUartSciDriver, ptrHwCfg);

                /* Sanity Check: The write size can never be negative */
                DebugP_assert (ptrUartSciDriver->writeSize >= 0);

                /* Are we done with the write buffer ? */
                if (ptrUartSciDriver->writeSize <= 0)
                {
                    /* YES: Do we need to post a semaphore? */
                    if (ptrUartSciDriver->writeSem)
                    {
                        /* YES: Write was operating in blocking mode. Post the semaphore to
                         * wakeup the calling thread since all the data has been written. */
                        SemaphoreP_postFromISR (ptrUartSciDriver->writeSem);
                    }

                    /* Disable TX interrupt until we do a new write */
                    UartSci_disableTxInterrupt(ptrHwCfg->ptrSCIRegs);
                }
            }
        }
    }

    /* Is there an overrun error? */
    if (UartSci_isRxOverrun(ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the number of overrun counter: */
        ptrUartSciDriver->stats.numRxOverrunInterrupts++;

        /* Clear the overrun status: */
        UartSci_clearRxOverrun (ptrHwCfg->ptrSCIRegs);
    }

    /* Is there a framing error? */
    if (UartSci_isFramingError(ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the stats: */
        ptrUartSciDriver->stats.numFramingErrors++;

        /* Clear the framing error: */
        UartSci_clearFramingError (ptrHwCfg->ptrSCIRegs);
    }

    /* Is there a parity error? */
    if (UartSci_isParityError(ptrHwCfg->ptrSCIRegs) == 1U)
    {
        /* Increment the stats: */
        ptrUartSciDriver->stats.numParityError++;

        /* Clear the parity error: */
        UartSci_clearParityError (ptrHwCfg->ptrSCIRegs);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be read from the UART Driver. This function implements
 *      a semaphore blocking operation if the UART driver instance is configured
 *      to operate in BLOCKING mode.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data which needs to be read
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been read
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_read(UART_Handle handle, uint8_t *buffer, uint32_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;
    SemaphoreP_Status   status;
    int32_t             retVal = 0;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL) || (ptrHwCfg->duplexity == UartSci_Duplexity_TX_ONLY))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    /* Disable preemption while checking if the uart is in use. */
    key = HwiP_disable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->readSize != 0U)
    {
        /* YES: Restore the interrupts and report the error to the application. */
        HwiP_restore(key);
        DebugP_log1 ("UART:(%p) Could not read data, UART in use\n", ptrHwCfg->ptrSCIRegs);
        retVal = UART_EINUSE;
        goto exit;
    }

    /* Save the data to be read */
    ptrUartSciDriver->ptrReadBuffer = buffer;
    ptrUartSciDriver->readSize      = size;
    ptrUartSciDriver->readCount     = 0;

    /* Restore the interrupts: */
    HwiP_restore(key);

    /* Determine the DMA Mode for the Driver: */
    if (ptrHwCfg->isDMAEnabledFxn(ptrUartSciDriver) == true)
    {
        /****************************************************************
         * DMA Mode: Initiate the Receive DMA
         ****************************************************************/
        retVal = ptrHwCfg->initiateRxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, size, false);
        if (retVal < 0)
        {
            /* Error: Unable to initiate the receive DMA */
            goto exit;
        }
    }
    else
    {
        /****************************************************************
         * Normal Mode: Enable the receive interrupt
         ****************************************************************/
        UartSci_enableRxInterrupt (ptrHwCfg->ptrSCIRegs);
    }

    /* Block the callee; till the UART reads are complete */
    status = SemaphoreP_pend (ptrUartSciDriver->readSem, ptrUartSciDriver->params.readTimeout);
    if (status == SemaphoreP_TIMEOUT)
    {
        /* Reset the read size */
        ptrUartSciDriver->readSize = 0;

        /* Report the error condition: */
        DebugP_log2 ("UART:(%p) Read timed out %d bytes read\n",
                     ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->readCount);
    }

    /* Setup the number of bytes which have been read */
    retVal = ptrUartSciDriver->readCount;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be read in polling mode from the UART Driver. The function
 *      will loop around till all the buffer is completely filled up or if the new
 *      line character is received.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data which needs to be read
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been read
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_readPolling(UART_Handle handle, uint8_t *buffer, uint32_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;
    int32_t             retVal;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL) || (ptrHwCfg->duplexity == UartSci_Duplexity_TX_ONLY))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    /* Disable preemption while checking if the uart is in use. */
    key = HwiP_disable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->readSize != 0)
    {
        /* YES: Restore the interrupts */
        HwiP_restore(key);

        /* Report the error to the application. */
        DebugP_log1 ("UART:(%p) Could not read data, UART in use\n", ptrHwCfg->ptrSCIRegs);

        /* Setup the error code: */
        retVal = UART_EINUSE;
        goto exit;
    }

    /* Save the data to be read */
    ptrUartSciDriver->ptrReadBuffer = buffer;
    ptrUartSciDriver->readSize      = size;
    ptrUartSciDriver->readCount     = 0;

    /* Restore the interrupts: */
    HwiP_restore(key);

    /* Determine the DMA Mode for the Driver: */
    if (ptrHwCfg->isDMAEnabledFxn(ptrUartSciDriver) == true)
    {
        /****************************************************************
         * DMA Mode: Initiate the Receive DMA
         ****************************************************************/
        retVal = ptrHwCfg->initiateRxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, size, true);
        if (retVal < 0)
        {
            /* Error: Unable to initiate the receive DMA */
            goto exit;
        }
    }
    else
    {
        /****************************************************************
         * Normal Mode: Read out all the data
         ****************************************************************/
        while (ptrUartSciDriver->readSize > 0)
        {
            /* Is the receiver free? */
            if (UartSci_isRxFree(ptrHwCfg->ptrSCIRegs) == 1U)
            {
                /* YES: Read out a character from the buffer. */
                UartSci_readChar (ptrUartSciDriver, ptrHwCfg);
            }
        }
    }

    /* Setup the number of bytes which have been read */
    retVal = ptrUartSciDriver->readCount;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the a previous data read operation has to be cancelled.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_readCancel(UART_Handle handle)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = HwiP_disable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->readSize == 0)
    {
        /* No: There is nothing to cancel */
        HwiP_restore(key);
    }
    else
    {
        /* YES: Reset the read size to 0; this will stop the read operations */
        ptrUartSciDriver->readSize = 0;

        /* Determine the DMA Mode for the Driver: */
        if (ptrHwCfg->isDMAEnabledFxn(ptrUartSciDriver) == true)
        {
            /****************************************************************
             * DMA Mode: Disable the Rx DMA
             ****************************************************************/
            UartSci_disableRxDMA (ptrHwCfg->ptrSCIRegs);
        }
        else
        {
            /****************************************************************
             * Normal Mode: Disable the Rx interrupt
             ****************************************************************/
            UartSci_disableRxInterrupt (ptrHwCfg->ptrSCIRegs);
        }

        /* Restore the interrupts: */
        HwiP_restore(key);

        /* We have cancelled the read operation; Are we operating the driver in BLOCKING
         * read mode? */
        if (ptrUartSciDriver->readSem)
        {
            /* YES: There could be a task which is blocked waiting for the read operation
             * to complete. We post the semaphore to wake up the task. */
            SemaphoreP_post(ptrUartSciDriver->readSem);
        }

        /* Debug Message: */
        DebugP_log2 ("Debug: UART(%p) read canceled %d bytes have been read\n",
                      ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->readCount);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be written to the UART Driver. This function implements
 *      a semaphore blocking operation if the UART driver instance is configured
 *      to operate in BLOCKING mode.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data to be written
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been transferred
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_write(UART_Handle handle, uint8_t *buffer, uint32_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    SemaphoreP_Status   status;
    uintptr_t           key;
    int32_t             retVal = 0;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = HwiP_disable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->writeSize != 0)
    {
        /* YES: Restore the interrupts */
        HwiP_restore(key);

        /* Log the error to the application: */
        DebugP_log1 ("UART:(%p) Could not write data, UART in use\n", ptrHwCfg->ptrSCIRegs);

        /* Setup the error code: */
        retVal = UART_EINUSE;
        goto exit;
    }

    /* NO: Save the data to be written */
    ptrUartSciDriver->ptrWriteBuffer = buffer;
    ptrUartSciDriver->writeSize      = size;
    ptrUartSciDriver->writeCount     = 0;

    /* Restore the interrupts: */
    HwiP_restore(key);

    /* Determine the DMA Mode for the Driver: */
    if (ptrHwCfg->isDMAEnabledFxn(ptrUartSciDriver) == true)
    {
        /****************************************************************
         * DMA Mode: Initiate the transmit DMA
         ****************************************************************/
        retVal = ptrHwCfg->initiateTxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, size, false);
        if (retVal < 0)
        {
            /* Error: Unable to initiate the transmit DMA */
            goto exit;
        }
    }
    else
    {
        /****************************************************************
         * Normal Mode: We always need to send out the first character
         * because the Transmit interrupt is only generated after the
         * first transfer from the TD to the TXSHF
         ****************************************************************/
        UartSci_writeChar(ptrUartSciDriver, ptrHwCfg);

        /* Do we have more data to send? */
        if (ptrUartSciDriver->writeSize == 0U)
        {
            /* NO: This is the case where there is only 1 byte of data to be
             * sent out. Setup the return value and we are done. */
            retVal = ptrUartSciDriver->writeCount;
            goto exit;
        }

        /* Enable the Transmit Interrupt: */
        UartSci_enableTxInterrupt (ptrHwCfg->ptrSCIRegs);
    }

    /* Block the callee; till the UART writes are complete */
    status = SemaphoreP_pend (ptrUartSciDriver->writeSem, ptrUartSciDriver->params.writeTimeout);
    if (status == SemaphoreP_TIMEOUT)
    {
        /* Time out: Write has not been completed in the specified duration
         * Disable the transmit interrupt */
        UartSci_disableTxInterrupt (ptrHwCfg->ptrSCIRegs);

        /* Reset the write size */
        ptrUartSciDriver->writeSize = 0;

        /* Report the error condition: */
        DebugP_log2 ("UART:(%p) Write timed out %d bytes written\n",
                     ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->writeCount);
    }

    /* Return the number of bytes which have been sent out. */
    retVal = ptrUartSciDriver->writeCount;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the data is to be written to the UART Driver but this will poll the UART
 *      driver in a while loop and will only return once all the data has been
 *      sent out.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  buffer
 *      Pointer to the data buffer
 *  @param[in]  size
 *      Size of the data to be written
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - Number of bytes which have been transferred
 *  @retval
 *      Error       - UART Error code
 */
static int32_t UartSci_writePolling(UART_Handle handle, uint8_t* buffer, uint32_t size)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;
    int32_t             retVal = 0;

    /* Sanity Check: Validate the arguments */
    if ((size == 0U) || (buffer == NULL))
    {
        /* Error: Invalid Arguments */
        retVal = UART_EINVAL;
        goto exit;
    }

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = HwiP_disable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->writeSize != 0)
    {
        /* YES: Restore the interrupts and report the error to the application. */
        HwiP_restore(key);
        DebugP_log1 ("UART:(%p) Could not write data, UART in use\n", ptrHwCfg->ptrSCIRegs);

        /* Setup the error code: */
        retVal = UART_EINUSE;
        goto exit;
    }

    /* NO: Save the data to be written */
    ptrUartSciDriver->ptrWriteBuffer = (uint8_t*)buffer;
    ptrUartSciDriver->writeSize      = (uint32_t)size;
    ptrUartSciDriver->writeCount     = 0U;

    /* Restore the interrupts: */
    HwiP_restore(key);

    /* Determine the DMA Mode for the Driver: */
    if (ptrHwCfg->isDMAEnabledFxn(ptrUartSciDriver) == true)
    {
        /****************************************************************
         * DMA Mode: Initiate the transmit DMA
         ****************************************************************/
        retVal = ptrHwCfg->initiateTxDMAFxn (ptrUartSciDriver, (uint32_t)buffer, size, true);
        if (retVal < 0)
        {
            /* Error: Unable to initiate the transmit DMA. The return value
             * is already setup with the error code. */
            goto exit;
        }
    }
    else
    {
        /****************************************************************
         * Normal Mode: Disable the Tx interrupt
         ****************************************************************/
        UartSci_disableTxInterrupt (ptrHwCfg->ptrSCIRegs);

        /* Send out all the data: */
        while (ptrUartSciDriver->writeSize > 0)
        {
            /* Is the transmitter free? */
            if (UartSci_isTxFree(ptrHwCfg->ptrSCIRegs) == 1U)
            {
                /* YES: Write out a character from the buffer. */
                UartSci_writeChar (ptrUartSciDriver, ptrHwCfg);
            }
        }
    }

    /* Setup the number of bytes which have been written */
    retVal = ptrUartSciDriver->writeCount;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked
 *      when the UART Driver write operations are ben canceled.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_writeCancel(UART_Handle handle)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    uintptr_t           key;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable preemption while checking if the uart is in use. */
    key = HwiP_disable();

    /* Is the UART in use? */
    if (ptrUartSciDriver->writeSize == 0)
    {
        /* NO: Restore the interrupts and report the error to the application. */
        HwiP_restore(key);
    }
    else
    {
        /* Set the write size to 0; this will prevent any more characters from being
         * transmitted. */
        ptrUartSciDriver->writeSize = 0;

        /* Determine the DMA Mode for the Driver: */
        if (ptrHwCfg->isDMAEnabledFxn(ptrUartSciDriver) == true)
        {
            /* DMA Mode: Disable the Tx DMA */
            UartSci_disableTxDMA (ptrHwCfg->ptrSCIRegs);
        }
        else
        {
            /* Normal Mode: Disable the Tx interrupt: */
            UartSci_disableTxInterrupt (ptrHwCfg->ptrSCIRegs);
        }

        /* Restore the interrupts: */
        HwiP_restore(key);

        /* We have cancelled the write operation. Are we operating the UART Driver in BLOCKING
         * write mode? */
        if (ptrUartSciDriver->writeSem)
        {
            /* YES: There could be a task which is blocked waiting for the write operation to
             * complete. We post the semaphore to wake up the task. */
            SemaphoreP_post(ptrUartSciDriver->writeSem);
        }

        /* Debug Message: */
        DebugP_log2 ("Debug: UART(%p) write canceled %d bytes have been written\n",
                      ptrHwCfg->ptrSCIRegs, ptrUartSciDriver->writeCount);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART Driver instance is being passed a control command.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  cmd
 *      UART command
 *  @param[in]  arg
 *      Opaque handle to the argument
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   UART Error code
 */
static int32_t UartSci_control(UART_Handle handle, uint32_t cmd, void *arg)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;
    int32_t             errCode;

    /* Initialize and setup the error code: */
    errCode = 0;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Processing is done on the basis of the command: */
    switch (cmd)
    {
        case UART_CMD_LOOPBACK:
        {
            int32_t*   loopback;

            /* Sanity Check: We need to ensure that a valid argument was passed */
            if (arg == NULL)
            {
                /* Error: No valid argument was passed. */
                errCode = UART_EINVAL;
            }
            else
            {
                /* Get the loopback status from the argument */
                loopback = (int32_t*)arg;

                /* The SCI Module can only be configured while it is RESET: */
                ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR(ptrHwCfg->ptrSCIRegs->SCIGCR1, 7U, 7U, 0U);

                /* Do we need to enable/disable loopback */
                if (*loopback == 1)
                {
                    /* Enable Loopback: */
                    ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR(ptrHwCfg->ptrSCIRegs->SCIGCR1, 16U, 16U, 1U);
                }
                else
                {
                    /* Disable Loopback: */
                    ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR(ptrHwCfg->ptrSCIRegs->SCIGCR1, 16U, 16U, 0U);
                }

                /* Bring the SCI module out of RESET: */
                ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR(ptrHwCfg->ptrSCIRegs->SCIGCR1, 7U, 7U, 1U);
            }
            break;
        }
        case UART_CMD_GET_STATS:
        {
            UART_Stats* ptrUARTStats;

            /* Sanity Check: We need to ensure that a valid argument was passed */
            if (arg == NULL)
            {
                /* Error: No valid argument was passed. */
                errCode = UART_EINVAL;
            }
            else
            {
                /* Get the pointer to the UART statistics */
                ptrUARTStats = (UART_Stats*)arg;

                /* Copy over the UART statistics */
                memcpy ((void*)ptrUARTStats, (void*)&ptrUartSciDriver->stats, sizeof(UART_Stats));
            }
            break;
        }
        default:
        {
            /* Error: Unsuported/Invalid command specified */
            errCode = UART_EINVAL;
            break;
        }
    }
    return errCode;
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the arguments
 *
 *  @param[in]  params
 *      UART Parameters to be validated
 *  @param[in]  ptrHwCfg
 *      Pointer to the hardware configuration
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t UartSci_validateParams (UART_Params* params, UartSci_HwCfg* ptrHwCfg)
{
    int32_t retVal = 0;

    /* Sanity Check: The current implementation of the UART Driver currently only supports
     * NONE, ODD and EVEN Parity.  */
    if (params->parityType >= UART_PAR_ZERO)
    {
        retVal = MINUS_ONE;
    }

    /* Sanity Check: Ensure that the clock frequency is NON-Zero. */
    if (params->clockFrequency == 0U)
    {
        retVal = MINUS_ONE;
    }

    /* Sanity Check: Ensure that if the driver instance is capable of PIN MUX then the
     * application has explicitly acknowledged it. */
    if (ptrHwCfg->pinMux == UartSci_PinMux_REQUIRED)
    {
        if (params->isPinMuxDone != 1)
        {
            retVal = MINUS_ONE;
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART Driver instance is being opened with a specific application
 *      supplied arguments.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *  @param[in]  params
 *      UART Parameters with which the driver is being opened
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   Driver Handle
 *  @retval
 *      Error   -   NULL
 */
static UART_Handle UartSci_open(UART_Handle handle, UART_Params* params)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    SemaphoreP_Params   semParams;
    HwiP_Params         hwiParams;
    UartSci_HwCfg*      ptrHwCfg;
    UART_Handle         retHandle = NULL;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Ensure that the driver is NOT being used: */
    if (ptrUARTConfig->object != NULL)
    {
        /* Error: UART Driver is already being used. */
        DebugP_log1 ("Debug: UART Driver (%p) has already been opened\n", ptrHwCfg->ptrSCIRegs);
        goto exit;
    }

    /* Sanity Check: Validate the arguments */
    if (UartSci_validateParams(params, ptrHwCfg) < 0)
    {
        /* Error: Invalid arguments have been passed to the driver */
        DebugP_log1 ("Debug: UART Driver (%p) Invalid arguments\n", ptrHwCfg->ptrSCIRegs);
        goto exit;
    }

    /* Allocate memory for the driver: */
    ptrUartSciDriver = MemoryP_ctrlAlloc ((uint32_t)sizeof(UartSci_Driver), 0);
    if (ptrUartSciDriver == NULL)
    {
        /* Error: Out of memory */
        DebugP_log1 ("Debug: UART Driver (%p) Out of memory\n", ptrHwCfg->ptrSCIRegs);
        goto exit;
    }

    /* Initialize the memory: */
    memset ((void *)ptrUartSciDriver, 0, sizeof(UartSci_Driver));

    /* Copy over the UART Parameters */
    memcpy ((void*)&ptrUartSciDriver->params, (void *)params, sizeof(UART_Params));

    /* Open the DMA Block associated with the UART Instance */
    if (ptrHwCfg->openDMAFxn (ptrUartSciDriver, ptrHwCfg) < 0)
    {
        /* Error: Unable to open the DMA Block. */
        goto exit;
    }

    /* Create a binary semaphore which is used to handle the Blocking operation. */
    SemaphoreP_Params_init(&semParams);
    semParams.mode             = SemaphoreP_Mode_BINARY;
    ptrUartSciDriver->writeSem = SemaphoreP_create(0, &semParams);

    /* Create a binary semaphore which is used to handle the Blocking operation. */
    SemaphoreP_Params_init(&semParams);
    semParams.mode            = SemaphoreP_Mode_BINARY;
    ptrUartSciDriver->readSem = SemaphoreP_create(0, &semParams);

    /* Reset the SCI Module: */
    ptrHwCfg->ptrSCIRegs->SCIGCR0 = 0U;
    ptrHwCfg->ptrSCIRegs->SCIGCR0 = 1U;

    /* Put the SCI Module in Sleep State: */
    ptrHwCfg->ptrSCIRegs->SCIGCR1 = 0;

    /* Disable the interrupts: */
    ptrHwCfg->ptrSCIRegs->SCICLEARINT   = 0xFFFFFFFFU;
    ptrHwCfg->ptrSCIRegs->SCICLEARINTLVL= 0xFFFFFFFFU;

    /* Setup the SCI Global Control Register:
     *  - Receiver Enabled
     *  - Transmit Enabled
     *  - Internal Clock
     *  - Asynchronous Timing Mode */
    ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FMKR (25U, 25U, 1U) |
                                    CSL_FMKR (24U, 24U, 1U) |
                                    CSL_FMKR (5U, 5U, 1U)   |
                                    CSL_FMKR (1U, 1U, 1U);

    /* Setup the Stop Bits: */
    if (params->stopBits == UART_STOP_ONE)
    {
        /* 1 Stop Bit: */
        ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR (ptrHwCfg->ptrSCIRegs->SCIGCR1, 4U, 4U, 0U);
    }
    else
    {
        /* 2 Stop Bits: */
        ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR (ptrHwCfg->ptrSCIRegs->SCIGCR1, 4U, 4U, 1U);
    }

    /* Setup the Parity: */
    switch (params->parityType)
    {
        case UART_PAR_NONE:
        {
            /* No Parity: */
            ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR (ptrHwCfg->ptrSCIRegs->SCIGCR1, 2U, 2U, 0U);
            break;
        }
        case UART_PAR_EVEN:
        {
            /* Even Parity: */
            ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR (ptrHwCfg->ptrSCIRegs->SCIGCR1, 2U, 2U, 1U);
            ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR (ptrHwCfg->ptrSCIRegs->SCIGCR1, 3U, 3U, 1U);
            break;
        }
        case UART_PAR_ODD:
        {
            /* Odd Parity: */
            ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR (ptrHwCfg->ptrSCIRegs->SCIGCR1, 2U, 2U, 1U);
            ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR (ptrHwCfg->ptrSCIRegs->SCIGCR1, 3U, 3U, 0U);
            break;
        }
        default:
        {
            /* Error: We have already verified the parameters. Control should never come here. */
            DebugP_assert (0);
            break;
        }
    }

    /* Setup the Baud Rate: */
    ptrHwCfg->ptrSCIRegs->SCIBAUD = params->clockFrequency / (16U * (params->baudRate + 1U));

    /* Setup the data length control: */
    switch (params->dataLength)
    {
        case UART_LEN_5:
        {
            /* Data Length is 5 */
            ptrHwCfg->ptrSCIRegs->SCICHAR = CSL_FMKR (2U, 0U, 4U);
            ptrUartSciDriver->shiftJustification = 3U;
            break;
        }
        case UART_LEN_6:
        {
            /* Data Length is 6 */
            ptrHwCfg->ptrSCIRegs->SCICHAR = CSL_FMKR (2U, 0U, 5U);
            ptrUartSciDriver->shiftJustification = 2U;
            break;
        }
        case UART_LEN_7:
        {
            /* Data Length is 7 */
            ptrHwCfg->ptrSCIRegs->SCICHAR = CSL_FMKR (2U, 0U, 6U);
            ptrUartSciDriver->shiftJustification = 1U;
            break;
        }
        case UART_LEN_8:
        {
            /* Data Length is 8 */
            ptrHwCfg->ptrSCIRegs->SCICHAR = CSL_FMKR (2U, 0U, 7U);
            ptrUartSciDriver->shiftJustification = 0U;
            break;
        }
        default:
        {
            /* Error: We have already verified the parameters. Control should never come here. */
            DebugP_assert (0);
            break;
        }
    }

    /* Setup the pin function control:
     *  - Enable the SCI Tx Pin [TX FUNC]
     *  - Enable the SCI Rx Pin [RX FUNC] */
    ptrHwCfg->ptrSCIRegs->SCIPIO0 = CSL_FMKR (2U, 2U, 1U) |
                                    CSL_FMKR (1U, 1U, 1U);

    /* Setup the output pin direction:
     *  - Tx Data Direction [TX_DATA OUT]
     *  - Rx Data Direction [RX_DATA OUT] */
    ptrHwCfg->ptrSCIRegs->SCIPIO3 = CSL_FMKR (2U, 2U, 0U) |
                                    CSL_FMKR (1U, 1U, 0U);

    /* Setup the output pin direction:
     *  - General Purpose Input Pin [TX DATA DIR]
     *  - General Purpose Input Pin [RX DATA DIR] */
    ptrHwCfg->ptrSCIRegs->SCIPIO1 = CSL_FMKR (2U, 2U, 0U) |
                                    CSL_FMKR (1U, 1U, 0U);

    /* Setup the drain control:
     *  - Tx Open Drain
     *  - Rx Open Drain */
    ptrHwCfg->ptrSCIRegs->SCIPIO6 = CSL_FMKR (2U, 2U, 0U) |
                                    CSL_FMKR (1U, 1U, 0U);

    /* Setup the pullup/pulldown control:
     *  - Enable Tx pull control
     *  - Enable Rx pull control */
    ptrHwCfg->ptrSCIRegs->SCIPIO7 = CSL_FMKR (2U, 2U, 0U) |
                                    CSL_FMKR (1U, 1U, 0U);

    /* Setup the pull select:
     *  - Tx pullup
     *  - Rx pullup */
    ptrHwCfg->ptrSCIRegs->SCIPIO8 = CSL_FMKR (2U, 2U, 1U) |
                                    CSL_FMKR (1U, 1U, 1U);

    /* Setup the interrupt level [Mapped to INT0]
     *  - Framing Error
     *  - Overrun Error
     *  - Parity Error
     *  - Receive
     *  - Transmit
     *  - Wakeup
     *  - Break Detect */
    ptrHwCfg->ptrSCIRegs->SCISETINTLVL = CSL_FMKR (26U, 26U, 0U) |
                                         CSL_FMKR (25U, 25U, 0U) |
                                         CSL_FMKR (24U, 24U, 0U) |
                                         CSL_FMKR (9U,  9U,  0U) |
                                         CSL_FMKR (8U,  8U,  0U) |
                                         CSL_FMKR (1U,  1U,  0U) |
                                         CSL_FMKR (0U,  0U,  0U);

    /* Enable the Interrupts:
     * - Framing Error
     * - Overrun Error
     * - Parity Error */
    ptrHwCfg->ptrSCIRegs->SCISETINT = CSL_FMKR (26U, 26U, 1U) |
                                      CSL_FMKR (25U, 25U, 1U) |
                                      CSL_FMKR (24U, 24U, 1U);

    /* Register the Interrupt Handler: */
    HwiP_Params_init(&hwiParams);
    hwiParams.name = "UART";
    hwiParams.arg  = (uintptr_t)handle;
    ptrUartSciDriver->hwiHandle = HwiP_create(ptrHwCfg->interruptNum, UartSci_ISR, &hwiParams);

    /* Debug Message: */
    DebugP_log2 ("Debug: UART Driver Registering HWI ISR [%p] for Interrupt %d\n",
                 ptrUartSciDriver->hwiHandle, ptrHwCfg->interruptNum);

    /* Start the SCI: */
    ptrHwCfg->ptrSCIRegs->SCIGCR1 = CSL_FINSR(ptrHwCfg->ptrSCIRegs->SCIGCR1, 7U, 7U, 1U);

    /* Write this back to the UART Configuration */
    ptrUARTConfig->object = (void *)ptrUartSciDriver;

    /* Mark the driver to be operational */
    ptrUartSciDriver->status = UartSci_DriverStatus_OPERATIONAL;

    /* Debug Message: */
    DebugP_log1 ("Debug: UART Driver %p opened\n", ptrHwCfg->ptrSCIRegs);

    /* Setup the return handle */
    retHandle = (UART_Handle)handle;

exit:
    return retHandle;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART Driver instance is being closed
 *
 *  @param[in]  handle
 *      Handle to the UART Driver
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_close(UART_Handle handle)
{
    UART_Config*        ptrUARTConfig;
    UartSci_Driver*     ptrUartSciDriver;
    UartSci_HwCfg*      ptrHwCfg;

    /* Get the UART Configuration: */
    ptrUARTConfig = (UART_Config*)handle;

    /* Get the UART Driver Instance: */
    ptrUartSciDriver = (UartSci_Driver*)ptrUARTConfig->object;

    /* Get the hardware configuration: */
    ptrHwCfg = (UartSci_HwCfg*)ptrUARTConfig->hwAttrs;

    /* Disable the interrupts: */
    ptrHwCfg->ptrSCIRegs->SCICLEARINT = 0xFFFFFFFFU;

    /* Was the HWI registered?  */
    if (ptrUartSciDriver->hwiHandle)
    {
        /* YES: Delete and unregister the interrupt handler. */
        HwiP_delete(ptrUartSciDriver->hwiHandle);
    }

    /* Was the UART Driver operating in Write Blocking mode? */
    if (ptrUartSciDriver->writeSem)
    {
        /* YES: Delete the write semaphore */
        SemaphoreP_delete (ptrUartSciDriver->writeSem);
    }

    /* Was the UART Driver operating in Read Blocking mode? */
    if (ptrUartSciDriver->readSem)
    {
        /* YES: Delete the read semaphore */
        SemaphoreP_delete (ptrUartSciDriver->readSem);
    }

    /* Close the DMA block associated with the UART Instance */
    ptrHwCfg->closeDMAFxn (ptrUartSciDriver);

    /* The driver is not in use: */
    ptrUartSciDriver->status = UartSci_DriverStatus_UNINITIALIZED;

    /* Cleanup the allocated memory: */
    MemoryP_ctrlFree (ptrUartSciDriver, (uint32_t)sizeof(UartSci_Driver));

    /* Unregister from the configuration: */
    ptrUARTConfig->object = NULL;
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered callback function which is invoked when
 *      the UART drivers are initialized.
 *
 *  @param[in]  handle
 *      Handle to the UART Driver which is to be initialized.
 *
 *  \ingroup UART_SCI_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void UartSci_init(UART_Handle handle)
{
    return;
}

