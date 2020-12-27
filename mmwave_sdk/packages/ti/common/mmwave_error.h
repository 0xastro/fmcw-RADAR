/**
 *   @file  mmwave_error.h
 *
 *   @brief
 *      Base error codes for the mmWave modules.
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

#ifndef MMWAVE_ERROR_H
#define MMWAVE_ERROR_H

/**************************************************************************
 * Base Error Code for the mmWave modules
 **************************************************************************/
#define MMWAVE_ERRNO_UART_BASE               (-2000)
#define MMWAVE_ERRNO_CRC_BASE                (-2100)
#define MMWAVE_ERRNO_MAILBOX_BASE            (-2200)
#define MMWAVE_ERRNO_PINMUX_BASE             (-2300)
#define MMWAVE_ERRNO_ADCBUF_BASE             (-2400)
#define MMWAVE_ERRNO_ESM_BASE                (-2500)
#define MMWAVE_ERRNO_SPI_BASE                (-2600)
#define MMWAVE_ERRNO_DMA_BASE                (-2700)
#define MMWAVE_ERRNO_HWA_BASE                (-2800)
#define MMWAVE_ERRNO_SOC_BASE                (-2900)
#define MMWAVE_ERRNO_EDMA_BASE               (-3000)
#define MMWAVE_ERRNO_BASE                    (-3100)
#define MMWAVE_ERRNO_CSI_BASE                (-3200)
#define MMWAVE_ERRNO_CBUFF_BASE              (-3300)
#define MMWAVE_ERRNO_CAN_BASE                (-3400)
#define MMWAVE_ERRNO_CANFD_BASE              (-3500)
#define MMWAVE_ERRNO_QSPI_BASE               (-3600)
#define MMWAVE_ERRNO_QSPIFLASH_BASE          (-3700)
#define MMWAVE_ERRNO_I2C_BASE                (-3800)
#define MMWAVE_ERRNO_WATCHDOG_BASE           (-3900)
#define MMWAVE_ERRNO_HSI_HDR_BASE            (-4000)
#define MMWAVE_ERRNO_CRYPTO_BASE             (-4100)
#define MMWAVE_ERRNO_DPM_BASE                (-4200)
#define MMWAVE_ERRNO_SBL_BASE                (-4400)

/**************************************************************************
 * Base Error Code for the mmWave data path (-30000 - -59999)
 **************************************************************************/
#define MMWAVE_ERRNO_DPU_BASE               (-30000)
#define MMWAVE_ERRNO_DPC_BASE               (-40000)
#define MMWAVE_ERRNO_DEMO_BASE              (-50000)

#endif  /* MMWAVE_ERROR_H */

