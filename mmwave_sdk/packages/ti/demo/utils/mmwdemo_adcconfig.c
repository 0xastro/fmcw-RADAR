/**
 *   @file  mmwdemo_adcconfig.c
 *
 *   @brief
 *      Implements High level ADC open/config APIs
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 Texas Instruments, Inc.
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <ti/common/sys_common.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/demo/utils/mmwdemo_adcconfig.h>

#ifdef MMWDEMO_CONFIGADCBUF_DBG
#include <xdc/runtime/System.h>
#endif

/**
 *  @b Description
 *  @n
 *      Function initializes and opens ADCBuf driver
 *
 *  \ingroup MMWDEMO_ADCCONFIG_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   ADCBuf driver handle
 *      Fail        NULL
 */
ADCBuf_Handle MmwDemo_ADCBufOpen(void)
{
    ADCBuf_Params       ADCBufparams;
    ADCBuf_Handle       ADCBufHandle = NULL;

    /* Initialize the ADCBUF */
    ADCBuf_init();

    /*****************************************************************************
     * Start ADCBUF driver:
     *****************************************************************************/
    /* ADCBUF Params initialize */
    ADCBuf_Params_init(&ADCBufparams);
    ADCBufparams.chirpThresholdPing = 1;
    ADCBufparams.chirpThresholdPong = 1;
    ADCBufparams.continousMode  = 0;

    /* Open ADCBUF driver */
    ADCBufHandle = ADCBuf_open(0, &ADCBufparams);

    return ADCBufHandle;
}

/**
 *  @b Description
 *  @n
 *      Function configures ADCBuf driver with data path parameters parsed from configurations
 *
 *  @param[in] adcBufHandle   ADCBuf driver handle
 *  @param[in] rxChannelEn    rx channel enable bit mask as described in rlChanCfg_t in rl_sensor.h
 *  @param[in] chirpThreshold  Chirp threshold
 *  @param[in] chanDataSize   Data size of the ADC channel
 *  @param[in] adcBufCfg     pointer to ADCBuf configuration
 *  @param[out] rxChanOffset  pointer to rx channel offset in the ADC buffer,
 *                            for each of enabled rx antenna
 *
 *  \ingroup MMWDEMO_ADCCONFIG_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *      Fail        < 0, one of ADCBuf driver error codes
 */
int32_t MmwDemo_ADCBufConfig
(
    ADCBuf_Handle         adcBufHandle,
    uint16_t              rxChannelEn,
    uint8_t               chirpThreshold,
    uint32_t              chanDataSize,
    MmwDemo_ADCBufCfg     *adcBufCfg,
    uint16_t              *rxChanOffset
)
{
    ADCBuf_dataFormat   dataFormat;
    ADCBuf_RxChanConf   rxChanConf;
    int32_t             retVal = 0U;
    uint8_t             channel;
    uint32_t            rxChanMask = 0xF;
    int32_t             rxChanOffsetIndx = 0;

    /*****************************************************************************
     * Data path :: ADCBUF driver Configuration
     *****************************************************************************/
    /* Populate data format from configuration */
    dataFormat.adcOutFormat       = adcBufCfg->adcFmt;
    dataFormat.channelInterleave  = adcBufCfg->chInterleave;
    dataFormat.sampleInterleave   = adcBufCfg->iqSwapSel;

    /* Disable all ADCBuf channels */
    if ((retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CHANNEL_DISABLE, (void *)&rxChanMask)) < 0)
    {
#ifdef MMWDEMO_CONFIGADCBUF_DBG
       System_printf("Error: Disable ADCBuf channels failed with [Error=%d]\n", retVal);
#endif
        goto exit;
    }

    retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CONF_DATA_FORMAT, (void *)&dataFormat);
    if (retVal < 0)
    {
        goto exit;
    }

    memset((void*)&rxChanConf, 0, sizeof(ADCBuf_RxChanConf));

    /* Enable Rx Channels */
    for (channel = 0; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++)
    {
        if(rxChannelEn & (0x1U << channel))
        {
            /* Populate the receive channel configuration: */
            rxChanConf.channel = channel;
            retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CHANNEL_ENABLE, (void *)&rxChanConf);
            if (retVal < 0)
            {
#ifdef MMWDEMO_CONFIGADCBUF_DBG
                System_printf("Error: MMWDemoDSS ADCBuf Control for Channel %d Failed with error[%d]\n", channel, retVal);
#endif
                goto exit;
            }
            /* Offset starts from 0 for the first channel */
            rxChanOffset[rxChanOffsetIndx++] = rxChanConf.offset;

            /* Calculate offset for the next channel */
            rxChanConf.offset  += chanDataSize * chirpThreshold;
        }
    }

#if (defined SOC_XWR16XX) || (defined SOC_XWR18XX) || (defined SOC_XWR68XX)
    /* Set ping/pong chirp threshold: */
    retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD,
                            (void *)&chirpThreshold);
    if(retVal < 0)
    {
#ifdef MMWDEMO_CONFIGADCBUF_DBG
        System_printf("Error: ADCbuf Ping Chirp Threshold Failed with Error[%d]\n", retVal);
#endif
        goto exit;
    }
    retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD,
                            (void *)&chirpThreshold);
    if(retVal < 0)
    {
#ifdef MMWDEMO_CONFIGADCBUF_DBG
        System_printf("Error: ADCbuf Pong Chirp Threshold Failed with Error[%d]\n", retVal);
#endif
        goto exit;
    }
#endif

#ifdef SOC_XWR14XX
    /* Set ping/pong chirp threshold: */
    retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD,
                            (void *)&chirpThreshold);
    if(retVal < 0)
    {
#ifdef MMWDEMO_CONFIGADCBUF_DBG
        System_printf("Error: ADCbuf Chirp Threshold Failed with Error[%d]\n", retVal);
#endif
        goto exit;
    }
#endif

exit:
    return (retVal);
}
