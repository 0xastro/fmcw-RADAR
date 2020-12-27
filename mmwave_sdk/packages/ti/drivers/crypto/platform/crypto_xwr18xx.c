/*
 *   @file  crypto_xwr18xx.c
 *
 *   @brief
 *      XWR18xx CRYPTO configuration
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
#include <ti/drivers/crypto/include/crypto_internal.h>

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/
/**
 * @brief   This is the XWR18xx CRYPTO configuration. There is only 1 CRYPTO instance
 * available. This should *NOT* be modified by the customer.
 */
Crypto_HwCfg gCryptoHwCfg[1] =
{
    /* CRYPTO Hardware configuration:
     * - Base address of AES memory mapped registers
     * - Base address of SHA memory mapped registers
     * - Base address of DTHE memory mapped registers
     * - AES Interrupt source
     * - HMAC Interrupt source
     */
    {
        ((volatile AESRegs*)SOC_XWR18XX_MSS_AES_BASE_ADDRESS),
        ((volatile SHARegs*)SOC_XWR18XX_MSS_SHA_BASE_ADDRESS),
        ((volatile DTHERegs*)SOC_XWR18XX_MSS_DTHE_BASE_ADDRESS),
        SOC_XWR18XX_MSS_CRYPTO_AESP_INT,
        SOC_XWR18XX_MSS_CRYPTO_SHAP_INT
    }
};

Crypto_Config Crypto_config[] =
{
    {
        (void *)NULL,                   /* Crypto AES Driver Object:        */
        (void *)&gCryptoHwCfg           /* Crypto AES Hw configuration:     */
    },
    {
        (void *)NULL,                   /* Crypto HMAC Driver Object:       */
        (void *)&gCryptoHwCfg           /* Crypto HMAC Hw configuration:    */
    },
    {
        NULL,
        NULL
    }
};
