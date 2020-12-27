/*
 *   @file  adcbuf_mmwave.c
 *
 *   @brief
 *      The file implements the mmWave ADC buffer Driver.
 *
 *  The ti/drivers/soc/include/reg_dssreg.h has the register layer definitons for the
 *  ADCBUF Module.
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
#include <stdlib.h>
#include <ti/drivers/osal/MemoryP.h>
#include <ti/drivers/adcbuf/ADCBuf.h>
#include <ti/drivers/adcbuf/include/adcbuf_mmwave.h>
#include <ti/drivers/osal/DebugP.h>
#include <ti/drivers/osal/HwiP.h>

/*
 * =============================================================================
 * Local Definitions
 * =============================================================================
 */
#define ADCBUF_PARAMS_CHECK       1

#if ADCBUF_PARAMS_CHECK

/* Number of register bits to configure threshold */
#define ADCBUF_NUMBITS_CHIRPTHRESHOLD         (5U)

/* Number of register bits to configure number of samples */
#define ADCBUF_NUMBITS_NUMBER_SAMPLES         (ADCBUFCFG4_ADCBUFSAMPCNT_BIT_END - \
                                               ADCBUFCFG4_ADCBUFSAMPCNT_BIT_START + 1U)

/* Number of register bits to configure channel address offset */
#define ADCBUF_NUMBITS_CHAN_ADDR_OFFSET       (ADCBUFCFG2_ADCBUFADDRX0_BIT_END - \
                                               ADCBUFCFG2_ADCBUFADDRX0_BIT_START + 1U)
#endif
/*
 * =============================================================================
 * Function prototype
 * =============================================================================
 */

/* Internal Functions */
static void ADCBUFSrcSelect(DSSRegs  *ptrDssRegBase, uint32_t source);
static void ADCBUFContinuousModeCtrl(DSSRegs  *ptrDssRegBase, uint32_t mode);
static void ADCBUFContinuousModeStart(DSSRegs  *ptrDssRegBase,  uint16_t numSamples);
static void ADCBUFContinuousModeStop(DSSRegs  *ptrDssRegBase);
static void ADCBUFConfigureDataFormat(DSSRegs  *ptrDssRegBase, uint8_t dataFormat, uint8_t interleave, uint8_t iqConfig);
static void ADCBUFChannelEnSetOffset(DSSRegs  *ptrDssRegBase, uint8_t channel, uint16_t offset);
static void ADCBUFChannelDisable(DSSRegs  *ptrDssRegBase, uint8_t channel);
static void ADCBUFTestPatternConfig(DSSRegs  *ptrDssRegBase, const ADCBuf_TestPatternConf *testPatternConf);
static void ADCBUFTestPatternStart(DSSRegs  *ptrDssRegBase);
static void ADCBUFTestPatternStop(DSSRegs  *ptrDssRegBase);
static void ADCBUFCQConfig(DSSRegs  *ptrDssRegBase, ADCBuf_CQConf *cqCfg);
#if ADCBUF_PARAMS_CHECK
static int32_t ADCBUFDriverParamsCheck(const ADCBuf_Params *params);
static int32_t ADCBUFCmdParamCheck(ADCBufMMWave_CMD cmd, void* arg);
#endif
#ifdef SOC_XWR14XX
static void ADCBUFSetNumChirpThreshhold(DSSRegs  *ptrDssRegBase, uint32_t threshhold);
#endif
#if (defined(SOC_XWR16XX ) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
static void ADCBUFSetPingNumChirpThreshhold(DSSRegs  *ptrDssRegBase, uint32_t threshhold);
static void ADCBUFSetPongNumChirpThreshhold(DSSRegs  *ptrDssRegBase, uint32_t threshhold);
#endif
/* External Functions */
void ADCBUF_MMWave_init(ADCBuf_Handle handle);
ADCBuf_Handle ADCBUF_MMWave_open(ADCBuf_Handle handle, const ADCBuf_Params *params);
void ADCBUF_MMWave_close(ADCBuf_Handle handle);
int_fast16_t ADCBUF_MMWave_control(ADCBuf_Handle handle, uint_fast8_t cmd, void * arg);
uint32_t ADCBUF_MMWave_getChanBufAddr(ADCBuf_Handle handle, uint8_t channel, int32_t *errCode);

/*
 * =============================================================================
 * Constants
 * =============================================================================
 */

const ADCBuf_FxnTable gADCBufFxnTable = {
    /*! Function to close the specified peripheral */
    &ADCBUF_MMWave_close,
    /*! Function to driver implementation specific control function */
    &ADCBUF_MMWave_control,
    /*! Function to initialize the given data object */
    &ADCBUF_MMWave_init,
    /*! Function to open the specified peripheral */
    &ADCBUF_MMWave_open,
    /*! Function to get ADCBuf channel address */
    &ADCBUF_MMWave_getChanBufAddr
};

/*
 * =============================================================================
 * Internal  Function Definitions
 * =============================================================================
 */

/**
 *  @b Description
 *  @n
 *      Selects the source of ADCBUF (DFE or HIL)
 *      HIL is only support in XWR16xx/XWR18xx/XWR68xx
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  source
 *      Source of the ADCBuf data. Don't care for XWR14xx.
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFSrcSelect(DSSRegs  *ptrDssRegBase, uint32_t source)
{
#if (defined(SOC_XWR16XX ) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
    /* Setup the ADC buffer source */
    ptrDssRegBase->DMMSWINT1 = CSL_FINSR(ptrDssRegBase->DMMSWINT1,
                                            DMMSWINT1_DMMADCBUFWREN_BIT_END,
                                            DMMSWINT1_DMMADCBUFWREN_BIT_START,
                                            source);
#endif
}

#ifdef SOC_XWR14XX
/**
 *  @b Description
 *  @n
 *      Set number of Chirps  threshhold to trigger ping/pong buffer switch
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  threshhold
 *      Number of chirps
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFSetNumChirpThreshhold(DSSRegs  *ptrDssRegBase, uint32_t threshhold)
{
    /* Number of chirps for PING/PONG buffer */
    ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                             ADCBUFCFG1_ADCBUFNUMCHRP_BIT_END,
                                             ADCBUFCFG1_ADCBUFNUMCHRP_BIT_START,
                                             threshhold);
}
#endif
#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
/**
 *  @b Description
 *  @n
 *      Set number of Chirps  threshhold for Ping buffer to trigger ping/pong buffer switch
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  threshhold
 *      Number of chirps
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFSetPingNumChirpThreshhold(DSSRegs  *ptrDssRegBase, uint32_t threshhold)
{
    /* Number of chirps for PING buffer */
    ptrDssRegBase->ADCBUFCFG4 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG4,
                                             ADCBUFCFG4_ADCBUFNUMCHRPPING_BIT_END,
                                             ADCBUFCFG4_ADCBUFNUMCHRPPING_BIT_START,
                                             threshhold);
}

/**
 *  @b Description
 *  @n
 *      Set number of Chirps  threshhold for Pong buffer to trigger ping/pong buffer switch
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  threshhold
 *      Number of chirps
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFSetPongNumChirpThreshhold(DSSRegs  *ptrDssRegBase, uint32_t threshhold)
{
    /* Number of chirps for Pong buffer */
    ptrDssRegBase->ADCBUFCFG4 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG4,
                                             ADCBUFCFG4_ADCBUFNUMCHRPPONG_BIT_END,
                                             ADCBUFCFG4_ADCBUFNUMCHRPPONG_BIT_START,
                                             threshhold);
}
#endif

/**
 *  @b Description
 *  @n
 *      ADC Buffer continuous mode control. Set to 1 to operate in continuous mode
 *
 *  @param[in]  ptrDssRegBase
 *      Base Address to the DSS Register Base
 *  @param[in]  mode
 *      Continuous mode, 1 to enable, 0 to disable.
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFContinuousModeCtrl(DSSRegs  *ptrDssRegBase, uint32_t mode)
{
    /* Setup the continuous mode control */
    ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                            ADCBUFCFG1_ADCBUFCONTMODEEN_BIT_END,
                                            ADCBUFCFG1_ADCBUFCONTMODEEN_BIT_START,
                                            mode);
}

/**
 *  @b Description
 *  @n
 *      Start ADC Buffer in continuous mode.
 *
 *  @param[in]  ptrDssRegBase
 *      Base address to the DSS Register Base
 *  @param[in]  numSamples
 *      Number of smples to be saved in ping/pong buffer before buffer switch.
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFContinuousModeStart(DSSRegs  *ptrDssRegBase,   uint16_t numSamples)
{
    /* Starts the continuous mode operation */
    ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                            ADCBUFCFG1_ADCBUFCONTSTRTPL_BIT_END,
                                            ADCBUFCFG1_ADCBUFCONTSTRTPL_BIT_START,
                                            1U);

    /* Setup the sample count */
    ptrDssRegBase->ADCBUFCFG4 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG4,
                                             ADCBUFCFG4_ADCBUFSAMPCNT_BIT_END,
                                             ADCBUFCFG4_ADCBUFSAMPCNT_BIT_START,
                                             (uint32_t)numSamples);
}

/**
 *  @b Description
 *  @n
 *      Stop ADC Buffer in continuous mode.
 *
 *  @param[in]  ptrDssRegBase
 *      Base address to the DSS Register Base
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFContinuousModeStop(DSSRegs  *ptrDssRegBase)
{
    ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                            ADCBUFCFG1_ADCBUFCONTSTOPPL_BIT_END,
                                            ADCBUFCFG1_ADCBUFCONTSTOPPL_BIT_START,
                                            1U);
}

/**
 *  @b Description
 *  @n
 *      Configure ADC Buffer data format.
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  dataFormat
 *      0 for complex, 1 for real data format.
 *  @param[in]  interleave
 *      Enable interleave mode.
 *  @param[in]  iqConfig
 *      In complex mode, 0 to store as Q(MSB) + I(LSB), 1 to store as I(MSB) +Q(LSB)
 *      In Real mode, this field is don't care
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFConfigureDataFormat(DSSRegs  *ptrDssRegBase, uint8_t dataFormat, uint8_t interleave, uint8_t iqConfig)
{
    if(dataFormat == 0)    /* Complex data format */
    {
        /* The requested data format is complex */
        ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                 ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_END,
                                                 ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_START,
                                                 (uint32_t)dataFormat);

        /* Setup the IQ swap configuration */
        ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                 ADCBUFCFG1_ADCBUFIQSWAP_BIT_END,
                                                 ADCBUFCFG1_ADCBUFIQSWAP_BIT_START,
                                                 (uint32_t)iqConfig);
    }
    else
    {
        /* The requested data format is real */
        ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                 ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_END,
                                                 ADCBUFCFG1_ADCBUFREALONLYMODE_BIT_START,
                                                 (uint32_t)dataFormat);
    }

    /* Update the interleave mode */
    ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                             ADCBUFCFG1_ADCBUFWRITEMODE_BIT_END,
                                             ADCBUFCFG1_ADCBUFWRITEMODE_BIT_START,
                                             (uint32_t)interleave);
}

/**
 *  @b Description
 *  @n
 *      Enable ADC Buffer RX channels
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  channel
 *      RX channel number
 *  @param[in]  offset
 *      Address offset in Ping/Pong buffer
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFChannelEnSetOffset(DSSRegs  *ptrDssRegBase, uint8_t channel, uint16_t offset)
{
    switch(channel)
    {
        case 0U:
            /* Enable the channel */
            ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                     ADCBUFCFG1_RX0EN_BIT_END,
                                                     ADCBUFCFG1_RX0EN_BIT_START,
                                                     1U);

            /* Setup the offset */
            ptrDssRegBase->ADCBUFCFG2 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG2,
                                                    ADCBUFCFG2_ADCBUFADDRX0_BIT_END,
                                                    ADCBUFCFG2_ADCBUFADDRX0_BIT_START,
                                                    ((uint32_t)offset >> 4U));
            break;
        case 1U:
            /* Enable the channel */
            ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                     ADCBUFCFG1_RX1EN_BIT_END,
                                                     ADCBUFCFG1_RX1EN_BIT_START,
                                                     1U);
            /* Setup the offset */
            ptrDssRegBase->ADCBUFCFG2 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG2,
                                                    ADCBUFCFG2_ADCBUFADDRX1_BIT_END,
                                                    ADCBUFCFG2_ADCBUFADDRX1_BIT_START,
                                                    ((uint32_t)offset >> 4U));
            break;
        case 2U:
            /* Enable the channel */
            ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                     ADCBUFCFG1_RX2EN_BIT_END,
                                                     ADCBUFCFG1_RX2EN_BIT_START,
                                                     1U);
            /* Setup the offset */
            ptrDssRegBase->ADCBUFCFG3 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG3,
                                                    ADCBUFCFG3_ADCBUFADDRX2_BIT_END,
                                                    ADCBUFCFG3_ADCBUFADDRX2_BIT_START,
                                                    ((uint32_t)offset >> 4U));
            break;
        case 3U:
            /* Enable the channel */
            ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                     ADCBUFCFG1_RX3EN_BIT_END,
                                                     ADCBUFCFG1_RX3EN_BIT_START,
                                                     1U);

            /* Setup the offset */
            ptrDssRegBase->ADCBUFCFG3 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG3,
                                                    ADCBUFCFG3_ADCBUFADDRX3_BIT_END,
                                                    ADCBUFCFG3_ADCBUFADDRX3_BIT_START,
                                                    ((uint32_t)offset >> 4U));
            break;

        default:
            /* Not  supported channels, code should not end up here */
            DebugP_assert(0);
            break;
    }
}

/**
 *  @b Description
 *  @n
 *      Disable ADC Buffer RX channels
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  channel
 *      RX channel number
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFChannelDisable(DSSRegs  *ptrDssRegBase, uint8_t channel)
{
    /* Disable the channel */
    ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                             ADCBUFCFG1_RX0EN_BIT_END + channel,
                                             ADCBUFCFG1_RX0EN_BIT_START + channel,
                                             0U);
}

/**
 *  @b Description
 *  @n
 *      Configure Test pattern for ADC Buffer. Based
*   on the passed values, it sets up the offset and value for each successive
*   sample for the test pattern IQ data, for each of the 4 channels. It also
*   configures period between successive samples of test pattern, and number of
*   samples to store(per channel) in each Ping and Pong register in continuous
*   mode of ADC Buffer.
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  testPatternConf
 *      Configuratio of test pattern for all channels
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFTestPatternConfig(DSSRegs  *ptrDssRegBase, const ADCBuf_TestPatternConf *testPatternConf)
{
    /* Setup the test pattern */
    /* RX1 Config */
    ptrDssRegBase->TESTPATTERNRX1ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX1ICFG,
                                                    TESTPATTERNRX1ICFG_TSTPATRX1IINCR_BIT_END,
                                                    TESTPATTERNRX1ICFG_TSTPATRX1IINCR_BIT_START,
                                                    testPatternConf->rxConfig[0].rxIInc);

    ptrDssRegBase->TESTPATTERNRX1ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX1ICFG,
                                                    TESTPATTERNRX1ICFG_TSTPATRX1IOFFSET_BIT_END,
                                                    TESTPATTERNRX1ICFG_TSTPATRX1IOFFSET_BIT_START,
                                                    testPatternConf->rxConfig[0].rxIOffset);

    ptrDssRegBase->TESTPATTERNRX1QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX1QCFG,
                                                    TESTPATTERNRX1QCFG_TSTPATRX1QINCR_BIT_END,
                                                    TESTPATTERNRX1QCFG_TSTPATRX1QINCR_BIT_START,
                                                    testPatternConf->rxConfig[0].rxQInc);

    ptrDssRegBase->TESTPATTERNRX1QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX1QCFG,
                                                    TESTPATTERNRX1QCFG_TSTPATRX1QOFFSET_BIT_END,
                                                    TESTPATTERNRX1QCFG_TSTPATRX1QOFFSET_BIT_START,
                                                    testPatternConf->rxConfig[0].rxQOffset);

    /* RX2 Config */
    ptrDssRegBase->TESTPATTERNRX2ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX2ICFG,
                                                    TESTPATTERNRX2ICFG_TSTPATRX2IINCR_BIT_END,
                                                    TESTPATTERNRX2ICFG_TSTPATRX2IINCR_BIT_START,
                                                    testPatternConf->rxConfig[1].rxIInc);

    ptrDssRegBase->TESTPATTERNRX2ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX2ICFG,
                                                    TESTPATTERNRX2ICFG_TSTPATRX2IOFFSET_BIT_END,
                                                    TESTPATTERNRX2ICFG_TSTPATRX2IOFFSET_BIT_START,
                                                    testPatternConf->rxConfig[1].rxIOffset);

    ptrDssRegBase->TESTPATTERNRX2QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX2QCFG,
                                                     TESTPATTERNRX2QCFG_TSTPATRX2QINCR_BIT_END,
                                                     TESTPATTERNRX2QCFG_TSTPATRX2QINCR_BIT_START,
                                                     testPatternConf->rxConfig[1].rxQInc);

    ptrDssRegBase->TESTPATTERNRX2QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX2QCFG,
                                                     TESTPATTERNRX2QCFG_TSTPATRX2QOFFSET_BIT_END,
                                                     TESTPATTERNRX2QCFG_TSTPATRX2QOFFSET_BIT_START,
                                                     testPatternConf->rxConfig[1].rxQOffset);

    /* RX3 Config */
    ptrDssRegBase->TESTPATTERNRX3ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX3ICFG,
                                                    TESTPATTERNRX3ICFG_TSTPATRX3IINCR_BIT_END,
                                                    TESTPATTERNRX3ICFG_TSTPATRX3IINCR_BIT_START,
                                                    testPatternConf->rxConfig[2].rxIInc);

    ptrDssRegBase->TESTPATTERNRX3ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX3ICFG,
                                                    TESTPATTERNRX3ICFG_TSTPATRX3IOFFSET_BIT_END,
                                                    TESTPATTERNRX3ICFG_TSTPATRX3IOFFSET_BIT_START,
                                                    testPatternConf->rxConfig[2].rxIOffset);

    ptrDssRegBase->TESTPATTERNRX3QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX3QCFG,
                                                     TESTPATTERNRX3QCFG_TSTPATRX3QINCR_BIT_END,
                                                     TESTPATTERNRX3QCFG_TSTPATRX3QINCR_BIT_START,
                                                     testPatternConf->rxConfig[2].rxQInc);

    ptrDssRegBase->TESTPATTERNRX3QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX3QCFG,
                                                     TESTPATTERNRX3QCFG_TSTPATRX3QOFFSET_BIT_END,
                                                     TESTPATTERNRX3QCFG_TSTPATRX3QOFFSET_BIT_START,
                                                     testPatternConf->rxConfig[2].rxQOffset);

    /* RX4 Config */
    ptrDssRegBase->TESTPATTERNRX4ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX4ICFG,
                                                    TESTPATTERNRX4ICFG_TSTPATRX4IINCR_BIT_END,
                                                    TESTPATTERNRX4ICFG_TSTPATRX4IINCR_BIT_START,
                                                    testPatternConf->rxConfig[3].rxIInc);
    ptrDssRegBase->TESTPATTERNRX4ICFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX4ICFG,
                                                    TESTPATTERNRX4ICFG_TSTPATRX4IOFFSET_BIT_END,
                                                    TESTPATTERNRX4ICFG_TSTPATRX4IOFFSET_BIT_START,
                                                    testPatternConf->rxConfig[3].rxIOffset);

    ptrDssRegBase->TESTPATTERNRX4QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX4QCFG,
                                                     TESTPATTERNRX4QCFG_TSTPATRX4QINCR_BIT_END,
                                                     TESTPATTERNRX4QCFG_TSTPATRX4QINCR_BIT_START,
                                                     testPatternConf->rxConfig[3].rxQInc);
    ptrDssRegBase->TESTPATTERNRX4QCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNRX4QCFG,
                                                     TESTPATTERNRX4QCFG_TSTPATRX4QOFFSET_BIT_END,
                                                     TESTPATTERNRX4QCFG_TSTPATRX4QOFFSET_BIT_START,
                                                     testPatternConf->rxConfig[3].rxQOffset);

    /* Setup the period */
    ptrDssRegBase->TESTPATTERNVLDCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNVLDCFG,
                                                    TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_END,
                                                    TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_START,
                                                    testPatternConf->period);

    /* Setup the sample count */
    ptrDssRegBase->ADCBUFCFG4 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG4,
                                             ADCBUFCFG4_ADCBUFSAMPCNT_BIT_END,
                                             ADCBUFCFG4_ADCBUFSAMPCNT_BIT_START,
                                             (testPatternConf->numSamples));
}

/**
 *  @b Description
 *  @n
 *      Starts the test pattern generation
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFTestPatternStart(DSSRegs  *ptrDssRegBase)
{
    /* Lower the clock */
    ptrDssRegBase->TESTPATTERNVLDCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNVLDCFG,
                                                     TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_END,
                                                     TESTPATTERNVLDCFG_TSTPATVLDCNT_BIT_START,
                                                     0x32U);

    /* Test pattern start */
    ptrDssRegBase->TESTPATTERNVLDCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNVLDCFG,
                                                     TESTPATTERNVLDCFG_TSTPATGENEN_BIT_END,
                                                     TESTPATTERNVLDCFG_TSTPATGENEN_BIT_START,
                                                     0x7U);
}

/**
 *  @b Description
 *  @n
 *      Stop the test pattern generation
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFTestPatternStop(DSSRegs  *ptrDssRegBase)
{
    ptrDssRegBase->TESTPATTERNVLDCFG = CSL_FINSR(ptrDssRegBase->TESTPATTERNVLDCFG,
                                                     TESTPATTERNVLDCFG_TSTPATGENEN_BIT_END,
                                                     TESTPATTERNVLDCFG_TSTPATGENEN_BIT_START,
                                                     0x0U);
}

/**
 *  @b Description
 *  @n
 *      Configure the Chirp Quality parameters
 *
 *  @param[in]  ptrDssRegBase
 *      Pointer to the DSS Register Base
 *  @param[in]  cqCfg
 *      Pointer to the CQ configuration
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static void ADCBUFCQConfig(DSSRegs  *ptrDssRegBase, ADCBuf_CQConf *cqCfg)
{
    /* Configure the CQ data width */
    ptrDssRegBase->CQCFG1 = CSL_FINSR(ptrDssRegBase->CQCFG1,
                                        CQCFG1_CQDATAWIDTH_BIT_END,
                                        CQCFG1_CQDATAWIDTH_BIT_START,
                                        cqCfg->cqDataWidth);

    /* Configure the if 96 bit pack mode */
    ptrDssRegBase->CQCFG1 = CSL_FINSR(ptrDssRegBase->CQCFG1,
                                        CQCFG1_CQ96BITPACKEN_BIT_END,
                                        CQCFG1_CQ96BITPACKEN_BIT_START,
                                        cqCfg->cq96BitPackEn);

    /* Configure the CQ1 base address */
    ptrDssRegBase->CQCFG1 = CSL_FINSR(ptrDssRegBase->CQCFG1,
                                        CQCFG1_CQ0BASEADDR_BIT_END,
                                        CQCFG1_CQ0BASEADDR_BIT_START,
                                        (uint32_t)cqCfg->cq0AddrOffset >> 4U);

    /* Configure the CQ2 base address */
    ptrDssRegBase->CQCFG1 = CSL_FINSR(ptrDssRegBase->CQCFG1,
                                        CQCFG1_CQ1BASEADDR_BIT_END,
                                        CQCFG1_CQ1BASEADDR_BIT_START,
                                        (uint32_t)cqCfg->cq1AddrOffset >> 4U);

    /* Configure the CQ3 base address */
    ptrDssRegBase->CQCFG1 = CSL_FINSR(ptrDssRegBase->CQCFG1,
                                        CQCFG1_CQ2BASEADDR_BIT_END,
                                        CQCFG1_CQ2BASEADDR_BIT_START,
                                        (uint32_t)cqCfg->cq2AddrOffset >> 4U);

}

#if ADCBUF_PARAMS_CHECK
/**
 *  @b Description
 *  @n
 *      Parameter check for input parameters. This function should be called when a parameter is expected.
 *  Hence arg should be a valid pointer.
 *
 *  @param[in]  params
 *      Parameter block for the ADCBUF Instance. If NULL, it will use default values.
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static int32_t ADCBUFDriverParamsCheck(const ADCBuf_Params *params)
{
    int32_t retCode = ADCBuf_STATUS_SUCCESS;
    uint32_t    paramVal;

    /* Check continuous mode of the ADCBUF */
    paramVal = params->continousMode;
    retCode = ADCBUFCmdParamCheck( ADCBufMMWave_CMD_SET_CONTINUOUS_MODE, (void *)&paramVal);
    if(retCode < 0)
    {
        goto Exit;
    }

    /* Check chirp threshhold */
#ifdef SOC_XWR14XX
    paramVal = params->chirpThreshold;
    retCode = ADCBUFCmdParamCheck( ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD, (void *)&paramVal);
    if(retCode < 0)
    {
        goto Exit;
    }
#endif

#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
    paramVal = params->chirpThresholdPing;
    retCode = ADCBUFCmdParamCheck( ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD, (void *)&paramVal);
    if(retCode < 0)
    {
        goto Exit;
    }

    paramVal = params->chirpThresholdPong;
    retCode = ADCBUFCmdParamCheck( ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD, (void *)&paramVal);
    if(retCode < 0)
    {
        goto Exit;
    }
#endif

Exit:
    return retCode;
}

/**
 *  @b Description
 *  @n
 *      Parameter check for input parameters. This function should be called when a parameter is expected.
 *  Hence arg should be a valid pointer.
 *
 *  @param[in]  cmd
 *      ADCBUF control command
 *  @param[in]  arg
 *      ACCBUF control arguments.
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
static int32_t ADCBUFCmdParamCheck(ADCBufMMWave_CMD cmd, void* arg)
{
    ADCBuf_dataFormat    *dataFormat;
    ADCBuf_RxChanConf    *rxChanConf;
    ADCBuf_CQConf        *cqConf;
    uint32_t             paramVal;

    int32_t retCode = ADCBuf_STATUS_SUCCESS;

    /* Validate the pointer to the command arguments
     * validate argument is 4 bytes  aligned.
     */
    if ((arg == (void *)NULL) || (((uint32_t)arg % 4U) != 0))
    {
        retCode = ADCBuf_STATUS_INVALID_PARAMS;
    }
    else
    {

        switch (cmd) {
            /* 1 bit setting Command */
            case ADCBufMMWave_CMD_SET_SRC:
            case ADCBufMMWave_CMD_SET_CONTINUOUS_MODE:
                paramVal = *(uint32_t *)arg;
                if (paramVal >= ((uint32_t)0x1U << 1U))
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

            case ADCBufMMWave_CMD_CONF_DATA_FORMAT:
                dataFormat = (ADCBuf_dataFormat *)arg;

                if ( (dataFormat->adcOutFormat >= (uint8_t)(0x1U << 1U))  ||
                      (dataFormat->channelInterleave >= (uint8_t)(0x1U << 1U)) ||
                      (dataFormat->sampleInterleave >= (uint8_t)(0x1U << 1U)) )
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

            case ADCBufMMWave_CMD_CONF_CQ:
                cqConf = (ADCBuf_CQConf *)arg;
                if( (cqConf->cqDataWidth > (uint8_t)(0x1U<<2U)) ||
                     (cqConf->cq96BitPackEn > (uint8_t)(0x1U << 1U)) )
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

              case ADCBufMMWave_CMD_START_CONTINUOUS_MODE:
                /* 16 bits for NUMBER OF SAMPLES */
                paramVal = *(uint32_t *)arg;
                if( paramVal >= ((uint32_t)0x1U << ADCBUF_NUMBITS_NUMBER_SAMPLES))
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

            case ADCBufMMWave_CMD_CHANNEL_ENABLE:
                rxChanConf = (ADCBuf_RxChanConf *)arg;

                /* Hardware supports channels 0-3 */
                if( (rxChanConf->channel >= SYS_COMMON_NUM_RX_CHANNEL) ||
                    (((uint32_t)rxChanConf->offset >> 4) >= ((uint32_t)0x1U << ADCBUF_NUMBITS_CHAN_ADDR_OFFSET)) )
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

            case ADCBufMMWave_CMD_CHANNEL_DISABLE:
                paramVal = *(uint32_t *)arg;

                /* Hardware supports channels 0-3 */
                if(((paramVal & 0xFFFFFFF0U) != 0U) || (paramVal == 0U))
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
            /* XWR16XX specific commands */
            case ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD:
            case ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD:
                if(((*(uint8_t *)arg) == 0) || ((*(uint8_t *)arg) > (uint8_t)(0x1U <<ADCBUF_NUMBITS_CHIRPTHRESHOLD)))
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

            case ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD:
                retCode = ADCBuf_STATUS_INVALID_CMD;
                break;
#else
            /* XWR14XX specific commands */
            case ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD:
                if(((*(uint8_t *)arg) == 0) || ((*(uint8_t *)arg) > (uint8_t)(0x1U <<ADCBUF_NUMBITS_CHIRPTHRESHOLD)))
                {
                    retCode = ADCBuf_STATUS_INVALID_PARAMS;
                }
                break;

            case ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD:
            case ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD:
                retCode = ADCBuf_STATUS_INVALID_CMD;
                break;
#endif
            default:
                break;
        }
    }

    return retCode;
}
#endif

/*
 * =============================================================================
 * External  Function Definitions
 * =============================================================================
 */

/**
 *  @b Description
 *  @n
 *      Function to initializes the ADCBUF_MMWave driver. This function
 *   must also be called before any other ADC driver APIs.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      N/A
 */
void ADCBUF_MMWave_init(ADCBuf_Handle handle)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      Open ADCBUF Driver
 *
 *  @param[in]  handle
 *      ADCBuf Instance Handle
 *  @param[in]  params
 *      Parameter block for the ADCBUF Instance. If NULL, it will use default values.
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   ADCBUF driver handle
 *  @retval
 *      Error     -   NULL
 */
ADCBuf_Handle ADCBUF_MMWave_open(ADCBuf_Handle handle, const ADCBuf_Params *params)
{
    ADCBufMMWave_Object      *ptrADCBufObject;
    ADCBuf_HwCfg             *ptrADCBufHwCfg;
    DSSRegs                  *ptrDssRegBase;
    ADCBuf_Handle             retHandle = NULL;

    /* Sanity check handle */
    DebugP_assert(handle != (ADCBuf_Handle)NULL);

    /* Sanity check handle */
    DebugP_assert(params != (const ADCBuf_Params *)NULL);

    /* Get the pointer to the object and hwAttrs */
    ptrADCBufHwCfg = (ADCBuf_HwCfg *)handle->hwAttrs;

    /* Initialize object handle */
    ptrADCBufObject = NULL;

    /* Sanity check the driver is not opened */
    if (handle->object != NULL)
    {
        /* Error: Driver is already being used. */
        DebugP_log1 ("ADCBUF: Driver (%p) has already been opened\n", handle->object);
    }
    else
    {
#if ADCBUF_PARAMS_CHECK
        /* Validate params */
        if ( ADCBUFDriverParamsCheck(params)< 0)
        {
             /* Error: Invalid chirpThreshold, put in debug log */
            DebugP_log1 ("ADCBUF: Invalid chirp Threshold setting(%d)\n", params->chirpThreshold);
        }
        else
#endif
        {
            /* Allocate memory for the driver: */
            ptrADCBufObject = MemoryP_ctrlAlloc ((uint32_t)sizeof(ADCBufMMWave_Object), 0);
            if (ptrADCBufObject == NULL)
            {
                /* Error: memory allocation failed */
                DebugP_log1 ("ADCBUF: Failed allocating memory for Driver(%p) Object\n", handle);
            }
            else
            {
                /* Initialize the memory: */
                memset ((void *)ptrADCBufObject, 0, sizeof(ADCBufMMWave_Object));

                /* Save driver object */
                handle->object               = (void *)ptrADCBufObject;
            }
        }
    }

    /* ADCBuf hardware initialization */
    if (ptrADCBufObject != NULL)
    {
        /* Get the DSS register base address  */
        ptrDssRegBase = ptrADCBufHwCfg->ptrDSSRegBase;

        ptrADCBufObject->ptrDssRegBase = ptrDssRegBase;

        /* Configuration of ADCBUF params */
        ADCBUFSrcSelect(ptrDssRegBase, (uint32_t)params->source);
        ADCBUFContinuousModeCtrl(ptrDssRegBase, params->continousMode);

        /* Configurate chirp threshold */
#ifdef SOC_XWR14XX
        ADCBUFSetNumChirpThreshhold(ptrDssRegBase, ((uint32_t)params->chirpThreshold - 1U));
#else
        ADCBUFSetPingNumChirpThreshhold(ptrDssRegBase, ((uint32_t)params->chirpThresholdPing - 1U));
        ADCBUFSetPongNumChirpThreshhold(ptrDssRegBase, ((uint32_t)params->chirpThresholdPong - 1U));
#endif
        /* Mark the handle as being used */
        ptrADCBufObject->isOpen = true;

        retHandle = handle;
    }

    return retHandle;

}

/**
 *  @b Description
 *  @n
 *      Close ADCBUF Driver. ADCBUF_MMWave_open() has to be called first.
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      None
 */
void ADCBUF_MMWave_close(ADCBuf_Handle handle)
{
    ADCBufMMWave_Object         *ptrADCBufObject;

    DebugP_assert( handle != (ADCBuf_Handle)NULL );

    /* Get the pointer to the object */
    ptrADCBufObject = handle->object;

    /* Mark the module as available */
    ptrADCBufObject->isOpen = false;

    /* Free the memory */
    MemoryP_ctrlFree (ptrADCBufObject, (uint32_t)sizeof(ADCBufMMWave_Object));

    /* Reset the object handle : */
    handle->object = NULL;
}

/**
 *  @b Description
 *  @n
 *      Get/Set ADCBUF_MMWave specific ADCBUF functions
 *
 *  @param[in]  handle
 *      ADCBUF Instance Handle
 *  @param[in]  cmd
 *      Command to the ADCBUF driver
 *  @param[in]  arg
 *      Argument for the driver command
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Status of the command execution
 */
int_fast16_t ADCBUF_MMWave_control(ADCBuf_Handle handle, uint_fast8_t cmd, void *arg)
{
    ADCBufMMWave_Object     *object;
    DSSRegs                 *ptrDssRegBase;
    int_fast16_t            status = ADCBuf_STATUS_SUCCESS;
    ADCBuf_dataFormat       *dataFormat;
    ADCBuf_RxChanConf       *rxChanConf;
    ADCBuf_TestPatternConf  *testPatternConf;
    ADCBuf_CQConf           *cqConf;
    uint32_t                 chirpThreshold;

#if ADCBUF_PARAMS_CHECK
    if (handle == (ADCBuf_Handle)NULL)
    {
        status = ADCBuf_STATUS_INVALID_PARAMS;
        goto Exit;
    }
#endif

    /* Get the Object from ADCBuf Handle */
    object = handle->object;

    /* Get the DSSREG base address */
    ptrDssRegBase = object->ptrDssRegBase;

#if ADCBUF_PARAMS_CHECK
    status = ADCBUFCmdParamCheck((ADCBufMMWave_CMD)cmd, arg);
#endif

    switch (cmd) {
        case ADCBufMMWave_CMD_SET_SRC:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                ADCBUFSrcSelect(ptrDssRegBase, *(uint32_t *)arg);
            }
            break;

#if (defined(SOC_XWR16XX) || defined(SOC_XWR18XX) || defined(SOC_XWR68XX))
        case ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                chirpThreshold = *(uint32_t *)arg;
                ADCBUFSetPingNumChirpThreshhold(ptrDssRegBase, chirpThreshold - 1U);
            }
            break;

        case ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                chirpThreshold = *(uint32_t *)arg;
                ADCBUFSetPongNumChirpThreshhold(ptrDssRegBase, chirpThreshold - 1U);
            }
            break;
#endif
#ifdef SOC_XWR14XX
        case ADCBufMMWave_CMD_SET_CHIRP_THRESHHOLD:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                chirpThreshold = *(uint32_t *)arg;
                ADCBUFSetNumChirpThreshhold(ptrDssRegBase, chirpThreshold - 1U);
            }
            break;
#endif
        case ADCBufMMWave_CMD_SET_CONTINUOUS_MODE:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                ADCBUFContinuousModeCtrl(ptrDssRegBase, *(uint32_t *)arg);
            }
            break;

        case ADCBufMMWave_CMD_START_CONTINUOUS_MODE:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                ADCBUFContinuousModeStart(ptrDssRegBase, *(uint16_t *)arg);
            }
            break;

        case ADCBufMMWave_CMD_STOP_CONTINUOUS_MODE:
            ADCBUFContinuousModeStop(ptrDssRegBase);
            status = ADCBuf_STATUS_SUCCESS;
            break;

        case ADCBufMMWave_CMD_CONF_DATA_FORMAT:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                dataFormat = (ADCBuf_dataFormat *)arg;
                ADCBUFConfigureDataFormat(ptrDssRegBase, dataFormat->adcOutFormat,
                                                          dataFormat->channelInterleave,
                                                          dataFormat->sampleInterleave);
            }
            break;

        case ADCBufMMWave_CMD_CHANNEL_ENABLE:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                rxChanConf = (ADCBuf_RxChanConf *)arg;
                ADCBUFChannelEnSetOffset(ptrDssRegBase, rxChanConf->channel, rxChanConf->offset);
            }
            break;

        case ADCBufMMWave_CMD_CHANNEL_DISABLE:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                uint8_t                  channel;
                uint32_t                 channelMask;

                channelMask = *(uint32_t *)arg;
                for(channel = 0; channel <SYS_COMMON_NUM_RX_CHANNEL; channel++)
                {
                    if(channelMask & ((uint32_t)0x1U << channel))
                    {
                        ADCBUFChannelDisable(ptrDssRegBase, channel);
                    }
                }
            }
            break;

        case ADCBufMMWave_CMD_CONF_TEST_PATTERN:
            testPatternConf = (ADCBuf_TestPatternConf *)arg;
            ADCBUFTestPatternConfig(ptrDssRegBase, testPatternConf);
            status = ADCBuf_STATUS_SUCCESS;
            break;

        case ADCBufMMWave_CMD_START_TEST_PATTERN:
            ADCBUFTestPatternStart(ptrDssRegBase);

            /* The following is for test pattern test only, in other cases, this value should not be touched. */
#ifdef SOC_XWR14XX
            ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                   31U,
                                                   24U,
                                                   0U);
#else
            ptrDssRegBase->ADCBUFCFG1 = CSL_FINSR(ptrDssRegBase->ADCBUFCFG1,
                                                   31U,
                                                   17U,
                                                   0U);
#endif
            status = ADCBuf_STATUS_SUCCESS;
            break;

        case ADCBufMMWave_CMD_STOP_TEST_PATTERN:
            ADCBUFTestPatternStop(ptrDssRegBase);
            status = ADCBuf_STATUS_SUCCESS;
            break;

        case ADCBufMMWave_CMD_CONF_CQ:
            if (status == ADCBuf_STATUS_SUCCESS)
            {
                cqConf = (ADCBuf_CQConf *)arg;
                ADCBUFCQConfig(ptrDssRegBase, cqConf);
            }
            break;

        default:
            status = ADCBuf_STATUS_UNDEFINEDCMD;
            break;
    }

#if ADCBUF_PARAMS_CHECK
Exit:
#endif

    return status;
}

/**
 *  @b Description
 *  @n
 *      Get address of ADC buffer for a given receive channel
 *
 *  @param[in]  handle
 *      ADCBuf Instance Handle
 *  @param[in]  channel
 *      ADCBuf receive channel number
 *  @param[in]  errCode
 *      Error code populated by driver if error condition is detected
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   ADCBuf Physical adderss of the receive channel
 *  @retval
 *      Error  -    0U
 */
uint32_t ADCBUF_MMWave_getChanBufAddr(ADCBuf_Handle handle, uint8_t channel, int32_t *errCode)
{
    ADCBufMMWave_Object      *ptrADCBufObject;
    DSSRegs                  *ptrDssRegBase;
    ADCBuf_HwCfg             *ptrADCBufHwCfg;
    uint32_t                 chanAddress = (uint32_t)0U;

    /* Parameter check */
    if ((channel >= SYS_COMMON_NUM_RX_CHANNEL) ||
       (handle == (ADCBuf_Handle)NULL))
    {
        /* Out of range channel number or invalid handle */
        *errCode = ADCBuf_STATUS_INVALID_PARAMS;
        goto Exit;
    }

    /* Sanity check Object handle */
    DebugP_assert(handle->object != (ADCBufMMWave_Object *)NULL);

    /* Get the Object from ADCBuf Handle */
    ptrADCBufObject = handle->object;

    /* Get the pointer to the object and hwAttrs */
    ptrADCBufHwCfg = (ADCBuf_HwCfg *)handle->hwAttrs;

    /* Get the DSSREG base address */
    ptrDssRegBase = ptrADCBufObject->ptrDssRegBase;

    /* Set default value for errCode */
    *errCode = ADCBuf_STATUS_SUCCESS;

    /* Check if the channel is enabled? */
    if(CSL_FEXTR(ptrDssRegBase->ADCBUFCFG1, ADCBUFCFG1_RX0EN_BIT_END + channel,
                 ADCBUFCFG1_RX0EN_BIT_START + channel) != (uint32_t)0U)
    {
        uint32_t addrOffset;

        switch(channel)
        {
            case 0U:
                addrOffset = CSL_FEXTR(ptrDssRegBase->ADCBUFCFG2,
                                      ADCBUFCFG2_ADCBUFADDRX0_BIT_END,
                                      ADCBUFCFG2_ADCBUFADDRX0_BIT_START);
                break;
            case 1U:
                addrOffset = CSL_FEXTR(ptrDssRegBase->ADCBUFCFG2,
                                      ADCBUFCFG2_ADCBUFADDRX1_BIT_END,
                                      ADCBUFCFG2_ADCBUFADDRX1_BIT_START);
                break;

            case 2U:
                addrOffset = CSL_FEXTR(ptrDssRegBase->ADCBUFCFG3,
                                      ADCBUFCFG3_ADCBUFADDRX2_BIT_END,
                                      ADCBUFCFG3_ADCBUFADDRX2_BIT_START);
                break;
            case 3U:
                addrOffset = CSL_FEXTR(ptrDssRegBase->ADCBUFCFG3,
                                      ADCBUFCFG3_ADCBUFADDRX3_BIT_END,
                                      ADCBUFCFG3_ADCBUFADDRX3_BIT_START);
                break;
            default:
                *errCode = ADCBuf_STATUS_INVALID_PARAMS;
                break;
        }

        if(*errCode == ADCBuf_STATUS_SUCCESS)
        {
            /* Calculate the physical address for the channel */
            chanAddress = ptrADCBufHwCfg->ptrADCBUFBaseAddr + ((uint32_t)addrOffset << 4U);
        }
        else
        {
            /* Code should not reach here */
        }
    }
    else
    {
        /* Channel is not enabled */
        *errCode = ADCBuf_STATUS_INVALID_PARAMS;
    }
Exit:
    return chanAddress;
}


/**
 *  @b Description
 *  @n
 *      Get address of CQ buffer for a given CQ
 *
 *  @param[in]  handle
 *      ADCBuf Instance Handle
 *  @param[in]  cqType
 *      CQ type that request the buffer address
 *  @param[in]  errCode
 *      Error code populated by driver if error condition is detected
 *
 *  \ingroup ADCBUF_DRIVER_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   CQ buffer Physical adderss of the requested CQ type
 *  @retval
 *      Error  -    0U
 */
uint32_t ADCBUF_MMWave_getCQBufAddr
(
    ADCBuf_Handle handle,
    ADCBufMMWave_CQType cqType,
    int32_t *errCode
)
{
    ADCBufMMWave_Object      *ptrADCBufObject;
    DSSRegs                  *ptrDssRegBase;
    ADCBuf_HwCfg             *ptrADCBufHwCfg;
    uint32_t                 chanAddress;
    uint32_t                 addrOffset;

    /* Set default value for errCode */
    *errCode = ADCBuf_STATUS_SUCCESS;

    /* Parameter check */
    if (handle == (ADCBuf_Handle)NULL)
    {
        /* Out of range channel number or invalid handle */
        *errCode = ADCBuf_STATUS_INVALID_PARAMS;
        goto Exit;
    }

    /* Sanity check Object handle */
    DebugP_assert(handle->object != (ADCBufMMWave_Object *)NULL);

    /* Get the Object from ADCBuf Handle */
    ptrADCBufObject = handle->object;

    /* Get the pointer to the object and hwAttrs */
    ptrADCBufHwCfg = (ADCBuf_HwCfg *)handle->hwAttrs;

    /* Get the DSSREG base address */
    ptrDssRegBase = ptrADCBufObject->ptrDssRegBase;

    switch(cqType)
    {
        case ADCBufMMWave_CQType_CQ0:
            addrOffset = CSL_FEXTR(ptrDssRegBase->CQCFG1,
                                  CQCFG1_CQ0BASEADDR_BIT_END,
                                  CQCFG1_CQ0BASEADDR_BIT_START);

            break;

        case ADCBufMMWave_CQType_CQ1:
            addrOffset = CSL_FEXTR(ptrDssRegBase->CQCFG1,
                                  CQCFG1_CQ1BASEADDR_BIT_END,
                                  CQCFG1_CQ1BASEADDR_BIT_START);

            break;

        case ADCBufMMWave_CQType_CQ2:
            addrOffset = CSL_FEXTR(ptrDssRegBase->CQCFG1,
                                  CQCFG1_CQ2BASEADDR_BIT_END,
                                  CQCFG1_CQ2BASEADDR_BIT_START);

            break;

        default:
            *errCode = ADCBuf_STATUS_INVALID_PARAMS;
            break;
    }
Exit:

    if(*errCode == ADCBuf_STATUS_SUCCESS)
    {
        /* Calculate the physical address for the channel */
        chanAddress = ptrADCBufHwCfg->ptrCQBUFBaseAddr + ((uint32_t)addrOffset << 4U);
    }
    else
    {
        /* Return 0 indicating invalid address */
        chanAddress = (uint32_t)0U;
    }

    return chanAddress;
}

