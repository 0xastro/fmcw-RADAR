/**
 *   @file  adcbuf_xwr1xxx.c
 *
 *   @brief
 *      XWR14xx ADCBUF configuration: This file is provided as a sample
 *      file but it can be customized by application developers to
 *      meet their application requirements.
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
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/adcbuf/include/adcbuf_mmwave.h>

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/

/**
 * @brief   This is the ADCBUF Driver registered function table
 */
extern ADCBuf_FxnTable gADCBufFxnTable;

/**************************************************************************
 ************************** Global Variables ******************************
 **************************************************************************/

#ifdef SOC_XWR14XX
/**
 * @brief   This is XWR14xx ADCBUF specific configuration and should *NOT* be modified
 * by the customer.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for MSS: */
    {
        ((DSSRegs *)SOC_XWR14XX_DSS_REG_BASE_ADDRESS),
        SOC_XWR14XX_DSS_CHIRP_AVAIL_IRQ,
        SOC_XWR14XX_MSS_ADCBUF_BASE_ADDRESS,
        SOC_XWR14XX_MSS_CHIRPINFO_BASE_ADDRESS
    }
};
#endif

#if defined(SOC_XWR16XX )
#ifdef SUBSYS_MSS
/**
 * @brief   This is XWR16xx ADCBUF specific configuration for MSS Subsystem.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for MSS: */
    {
        ((DSSRegs *)SOC_XWR16XX_MSS_DSSREG_BASE_ADDRESS),
        SOC_XWR16XX_MSS_CHIRP_AVAIL_IRQ,
        SOC_XWR16XX_MSS_ADCBUF_BASE_ADDRESS,
        SOC_XWR16XX_MSS_CHIRPINFO_BASE_ADDRESS
    }
};
#endif

#ifdef SUBSYS_DSS
/**
 * @brief   This is XWR16xx ADCBUF specific configuration for DSS Subsystem.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for DSS: */
    {
        ((DSSRegs *)SOC_XWR16XX_DSS_DSSREG_BASE_ADDRESS),
        SOC_XWR16XX_DSS_INTC_EVENT_CHIRP_AVAIL,
        SOC_XWR16XX_DSS_ADCBUF_BASE_ADDRESS,
        SOC_XWR16XX_DSS_CHIRPINFO_BASE_ADDRESS
    }
};
#endif

#endif /* SOC_XWR16XX */

#if defined(SOC_XWR18XX)
#ifdef SUBSYS_MSS
/**
 * @brief   This is XWR18xx ADCBUF specific configuration for MSS Subsystem.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for MSS: */
    {
        ((DSSRegs *)SOC_XWR18XX_MSS_DSSREG_BASE_ADDRESS),
        SOC_XWR18XX_MSS_CHIRP_AVAIL_IRQ,
        SOC_XWR18XX_MSS_ADCBUF_BASE_ADDRESS,
        SOC_XWR18XX_MSS_CHIRPINFO_BASE_ADDRESS
    }
};
#endif

#ifdef SUBSYS_DSS
/**
 * @brief   This is XWR18xx ADCBUF specific configuration for DSS Subsystem.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for DSS: */
    {
        ((DSSRegs *)SOC_XWR18XX_DSS_DSSREG_BASE_ADDRESS),
        SOC_XWR18XX_DSS_INTC_EVENT_CHIRP_AVAIL,
        SOC_XWR18XX_DSS_ADCBUF_BASE_ADDRESS,
        SOC_XWR18XX_DSS_CHIRPINFO_BASE_ADDRESS
    }
};
#endif

#endif /* SOC_XWR18XX */

#if defined(SOC_XWR68XX)
#ifdef SUBSYS_MSS
/**
 * @brief   This is XWR68xx ADCBUF specific configuration for MSS Subsystem.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for MSS: */
    {
        ((DSSRegs *)SOC_XWR68XX_MSS_DSSREG_BASE_ADDRESS),
        SOC_XWR68XX_MSS_CHIRP_AVAIL_IRQ,
        SOC_XWR68XX_MSS_ADCBUF_BASE_ADDRESS,
        SOC_XWR68XX_MSS_CHIRPINFO_BASE_ADDRESS
    }
};
#endif

#ifdef SUBSYS_DSS
/**
 * @brief   This is XWR68xx ADCBUF specific configuration for DSS Subsystem.
 */
ADCBuf_HwCfg gADCBufHwCfg[1] =
{
    /* ADCBUF Hardware configuration for DSS: */
    {
        ((DSSRegs *)SOC_XWR68XX_DSS_DSSREG_BASE_ADDRESS),
        SOC_XWR68XX_DSS_INTC_EVENT_CHIRP_AVAIL,
        SOC_XWR68XX_DSS_ADCBUF_BASE_ADDRESS,
        SOC_XWR68XX_DSS_CHIRPINFO_BASE_ADDRESS
    }
};
#endif

#endif /* SOC_XWR68XX */

/**
 * @brief   There is only one ADCBUF .
 */
ADCBuf_Config ADCBuf_config[2] =
{
    /* ADCBuf driver config */
    {
        &gADCBufFxnTable,                      /* ADCBUF Driver Function Table            */
        (void *)NULL,                              /* ADCBUF Driver Object:                      */
        (void const *)&gADCBufHwCfg[0]         /* ADCBUF Hw configuration:                  */
    },

    /* Dummy ADCBuf driver config */
    {
        NULL,                      /* ADCBUF Driver Function Table            */
        NULL,                              /* ADCBUF Driver Object:                      */
        NULL       /* ADCBUF Hw configuration:                  */
    }
};

