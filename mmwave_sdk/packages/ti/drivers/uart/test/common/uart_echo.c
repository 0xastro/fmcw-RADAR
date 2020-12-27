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

#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* mmWave SK Include Files: */
#include <ti/drivers/uart/UART.h>
#include <ti/drivers/pinmux/pinmux.h>

/* MCPI global Variable which is used to set 
 * test on UART Polling mode or Blocking mode  
 */
uint8_t gMCPIUARTPollingModeEnable = 0;

/* SOC specific */
extern uint32_t gCPUClockFrequency;

uint32_t BAUDRATE = 9600;
uint8_t STOPBITS = 1;
uint8_t DATALEN = 8;
uint8_t PARITY_VALUE = 4;


uint8_t uartDataLenths [] = {
    0, // Padding
    0,
    0,
    0,
    0,
    (uint8_t) UART_LEN_5,
    (uint8_t) UART_LEN_6,
    (uint8_t) UART_LEN_7,
    (uint8_t) UART_LEN_8
};

uint8_t uartParityTypes [] = {
    (uint8_t) UART_PAR_ZERO,
    (uint8_t) UART_PAR_ONE,
    (uint8_t) UART_PAR_EVEN,
    (uint8_t) UART_PAR_ODD,
    (uint8_t) UART_PAR_NONE
};

uint8_t uartStopBits [] = {
    0,  // Padding
    (uint8_t) UART_STOP_ONE,
    (uint8_t) UART_STOP_TWO
};

/*
 *  ======== echoFxn ========
 */
void Test_echo(UArg a0, UArg a1)
{
    uint8_t input;
    UART_Params     params;
    UART_Handle     handle;

    /* Setup the default UART Parameters */
    UART_Params_init(&params);
    params.writeDataMode = UART_DATA_BINARY;
    params.readDataMode = UART_DATA_BINARY;
    params.readReturnMode = UART_RETURN_FULL;
    params.readEcho = UART_ECHO_OFF;

    params.clockFrequency = gCPUClockFrequency;
    params.isPinMuxDone   = 1;

    params.baudRate = BAUDRATE;
    params.dataLength = (UART_LEN) uartDataLenths[DATALEN];
    params.parityType = (UART_PAR) uartParityTypes[PARITY_VALUE];
    params.stopBits = (UART_STOP) uartStopBits[STOPBITS]; 

    /* Open the UART1 Instance */
    handle = UART_open(0, &params);
    if (handle == NULL)
    {
        printf("Error: Unable to open the UART Instance\n");
        return;
    }

    System_printf("B:%ld_L:%d_P:%d_S:%d\n"
                  "Starting the UART Echo example\n",
                   params.baudRate,
                   params.dataLength,
                   params.parityType,
                   params.stopBits);
    System_flush();
    
    /* Loop forever echoing */
    if (gMCPIUARTPollingModeEnable == 1)
    {
        while (TRUE) 
        {
            UART_readPolling(handle, &input, 1);
            UART_writePolling(handle, &input, 1); 
        }
    }
    else
    {
        while (TRUE) 
        {
            UART_read(handle, &input, 1);
            UART_write(handle, &input, 1); 
        }
    }
}

