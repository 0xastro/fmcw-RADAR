/****************************************************************************************
 * FileName     : rl_device.h
 *
 * Description  : This file defines the functions required to Control mmwave radar Device.
 *
 ****************************************************************************************
 * (C) Copyright 2014, Texas Instruments Incorporated. - TI web address www.ti.com
 *---------------------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT,  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */
/****************************************************************************************
* FILE INCLUSION PROTECTION
****************************************************************************************
*/
#ifndef RL_DEVICE_H
#define RL_DEVICE_H

/****************************************************************************************
* INCLUDE FILES
****************************************************************************************
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/control/mmwavelink/include/rl_protocol.h>

/****************************************************************************************
* MACRO DEFINITIONS
****************************************************************************************
*/

/******************************************************************************
 * TYPE-DEFINE STRUCT/ENUM/UNION DEFINITIONS
 ******************************************************************************
 */


/*! \brief
* IQ Swap Selection
*/
#define RL_DEV_I_FIRST                  (0x0U)
#define RL_DEV_Q_FIRST                  (0x1U)
#define RL_DEV_IQSWAP_MAX               (0x2U)

/*! \brief
* Channel Interleave Selection
*/
#define RL_DEV_CH_INTERLEAVED           (0x0U)
#define RL_DEV_CH_NON_INTERLEAVED       (0x1U)
#define RL_DEV_CH_INTERLEAVE_MAX        (0x2U)


/*! \brief
* File Dowload data structure
*/
typedef struct rlFileData
{
    /**
     * @brief  File data length
     */
    rlUInt32_t chunkLen;
    /**
     * @brief  File data buffer
     */
    rlUInt16_t fData[RL_CMD_PL_LEN_MAX/2U];
}rlFileData_t;


/*! \brief
* mmwave radar device MCU Clock output
*/
typedef struct rlMcuClkCfg
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field controls the enable - disable of the MCU clock. \n
                  Value    Description \n
                  0x0      Disable MCU clock \n
                  0x1      Enable MCU clock  \n
     */
    rlUInt8_t mcuClkCtrl;
    /**
     * @brief  This field specifies the source of the MCU clock. Applicable only in case of MCU \n
                 clock enable.Else ignored. \n
                  Value   Description \n
                  0x0     XTAL(as connected to the device) \n
                  0x1     RESERVED \n
                  0x2     600MHz PLL divided clock \n
     */
    rlUInt8_t mcuClkSrc;
    /**
     * @brief  This field specifies the division factor to be applied to source clock. \n
                 Applicable only in case of MCU \n
                  clock enable. Else ignored. \n
                  Value     Description \n
                  0x0       Divide by 1 \n
                  0x1       Divide by 2 \n
                  ...       ... \n
                  0xFF      Divide by 256 \n
     */
    rlUInt8_t srcClkDiv;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
#else
    /**
     * @brief  This field specifies the source of the MCU clock. Applicable only in case of MCU \n
                 clock enable.Else ignored. \n
                  Value   Description \n
                  0x0     XTAL(as connected to the device) \n
                  0x1     RESERVED \n
                  0x2     600MHz PLL divided clock \n
     */
    rlUInt8_t mcuClkSrc;
    /**
     * @brief  This field controls the enable - disable of the MCU clock. \n
                  Value    Description \n
                  0x0      Disable MCU clock \n
                  0x1      Enable MCU clock  \n
     */
    rlUInt8_t mcuClkCtrl;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
    /**
     * @brief  This field specifies the division factor to be applied to source clock. \n
                 Applicable only in case of MCU \n
                  clock enable. Else ignored. \n
                  Value     Description \n
                  0x0       Divide by 1 \n
                  0x1       Divide by 2 \n
                  ...       ... \n
                  0xFF      Divide by 256 \n
     */
    rlUInt8_t srcClkDiv;
#endif
}rlMcuClkCfg_t;

/*! \brief
* mmwave radar device PMIC Clock output
*/
typedef struct rlPmicClkCfg
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field controls the enable - disable of the PMIC clock. \n
                   Value  Description \n
                    0x0    Disable PMIC clock \n
                    0x1    Enable PMIC clock \n
     */
    rlUInt8_t pmicClkCtrl;
    /**
     * @brief  This field specifies the source of the PMIC clock. Applicable only in case of \n
                   PMIC clock enable. Else ignored. \n
                    Value   Description \n
                    0x0     XTAL (as connected to the device) \n
                    0x2     600MHz PLL divided clock \n
     */
    rlUInt8_t pmicClkSrc;
    /**
     * @brief  This field specifies the division factor to be applie to source clock. \n
                   Applicable only in case of PMIC clock \n
                    enable. Else ignored. \n
                    Value   Description \n
                    0x0     Divide by 1 \n
                    0x1     Divide by 2 \n
                    ...     ... \n
                    0xFF    Divide by 256 \n
     */
    rlUInt8_t srcClkDiv;
    /**
     * @brief  This field specifies the mode of operation for the PMIC clock generation. \n
                   Applicable only in case of PMIC clock \n
                    enable. Else ignored. \n
                    Value   Description \n
                    0x0     Continuous mode (free running mode where the frequency \n
                            change/jump is triggered based on configured number of \n
                            internal clock ticks) \n
                    0x1     Chirp-to-Chirp staircase mode (frequency change/jump is \n
                            triggered at every chirp boundary) \n
     */
    rlUInt8_t modeSel;
#else
    /**
     * @brief  This field specifies the source of the PMIC clock. Applicable only in case of \n
                   PMIC clock enable. Else ignored. \n
                    Value   Description \n
                    0x0     XTAL (as connected to the device) \n
                    0x2     600MHz PLL divided clock \n
     */
    rlUInt8_t pmicClkSrc;
    /**
     * @brief  This field controls the enable - disable of the PMIC clock. \n
                   Value  Description \n
                    0x0    Disable PMIC clock \n
                    0x1    Enable PMIC clock \n
     */
    rlUInt8_t pmicClkCtrl;
    /**
     * @brief  This field specifies the mode of operation for the PMIC clock generation. \n
                   Applicable only in case of PMIC clock \n
                    enable. Else ignored. \n
                    Value   Description \n
                    0x0     Continuous mode (free running mode where the frequency \n
                            change/jump is triggered based on configured number of \n
                            internal clock ticks) \n
                    0x1     Chirp-to-Chirp staircase mode (frequency change/jump is \n
                    triggered at every chirp boundary) \n
     */
    rlUInt8_t modeSel;
    /**
     * @brief  This field specifies the division factor to be applie to source clock. \n
                   Applicable only in case of PMIC clock \n
                    enable. Else ignored. \n
                    Value   Description \n
                    0x0     Divide by 1 \n
                    0x1     Divide by 2 \n
                    ...     ... \n
                    0xFF    Divide by 256 \n
     */
    rlUInt8_t srcClkDiv;
#endif
    /**
     * @brief  Applicable only in case of PMIC clock enable. Else ignored. \n
                   Bit[25:0] - Frequency slope value to be applied in [8.18] format. \n
                   1 LSB = 1/218 \n
                   In continuous mode this value is accumulated every PMIC clock tick \n
                   with the seed as MIN_NDIV_VAL till MAX_NDIV_VAL is reached \n
                   In the stair case mode this value is accumulated every chirp with the \n
                   seed as MIN_NDIV_VAL till MAX_NDIV_VAL is reached \n
     */
    rlUInt32_t freqSlope;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Applicable only in case of PMIC clock enable. Else ignored. Min allowed divider \n
                   value (depends upon the highest desired clock frequency) \n
     */
    rlUInt8_t minNdivVal;
    /**
     * @brief  Applicable only in case of PMIC clock enable. Else ignored. Max allowed divider \n
                  value (depends upon the lowest desired clock frequency) \n
     */
    rlUInt8_t maxNdivVal;
    /**
     * @brief  Applicable only in case of PMIC clock enable. Else ignored. This field controls \n
                  the enable-disable of the clock dithering. Adds a pseudo random real number \n
                  (0 or 1) to the accumulated divide value. Hence it brings a random dithering \n
                  of 1 LSB. \n
                   Value   Description \n
                   0x0     Clock dithering disabled \n
                   0x1     Clock dithering enabled \n
     */
    rlUInt8_t clkDitherEn;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
#else
    /**
     * @brief  Applicable only in case of PMIC clock enable. Else ignored. Max allowed divider \n
                  value (depends upon the lowest desired clock frequency) \n
     */
    rlUInt8_t maxNdivVal;
    /**
     * @brief  Applicable only in case of PMIC clock enable. Else ignored. Min allowed divider \n
                   value (depends upon the highest desired clock frequency) \n
     */
    rlUInt8_t minNdivVal;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
    /**
     * @brief  Applicable only in case of PMIC clock enable. Else ignored. This field controls \n
                  the enable-disable of the clock dithering. Adds a pseudo random real number \n
                  (0 or 1) to the accumulated divide value. Hence it brings a random dithering \n
                  of 1 LSB. \n
                   Value   Description \n
                   0x0     Clock dithering disabled \n
                   0x1     Clock dithering enabled \n
     */
    rlUInt8_t clkDitherEn;
#endif
}rlPmicClkCfg_t;

/*! \brief
* mmwave radar device latent fault test
*/
typedef struct rllatentFault
{
    /**
     * @brief  Bits    Definition \n
               0       MibSPI self-test \n
               1       DMA self-test \n
               2       RESERVED \n
               3       RTI self-test \n
               4       ESM self-test \n
               5       EDMA self-test \n
               6       CRC self-test \n
               7       VIM self-test \n
               8       RESERVED \n
               9       Mailbox self-test \n
               10      LVDS pattern generation test \n
               11      CSI2 pattern generation test \n
               12      Generating NERROR \n
               13      MibSPI single bit error test \n
               14      MibSPI double bit error test \n
               15      DMA Parity error \n
               16      TCMA RAM single bit errors \n
               17      TCMB RAM single bit errors \n
               18      TCMA RAM double bit errors \n
               19      TCMB RAM double bit errors \n
               20      TCMA RAM parity errors. \n
               21      TCMB RAM parity errors. \n
               22      RESERVED \n
               23      RESERVED \n
               24      DMA MPU Region tests \n
               25      MSS Mailbox Single bit errors \n
               26      MSS Mailbox double bit errors \n
               27      radarSS Mailbox Single bit errors \n
               28      radarSS Mailbox double bit errors \n
               29      EDMA MPU test \n
               30      EDMA parity test \n
               31      CSI2 parity test \n
     */
    rlUInt32_t testEn1;
    /**
     * @brief  Bits    Definition \n
               0       DCC self test \n
               1       DCC fault insertion \n
               2       PCR fault generation test \n
               3       VIM RAM parity test \n 
               4       SCI boot time test  \n       
               31:5    RESERVED \n
     */
    rlUInt32_t testEn2;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value  Definition \n
               0      Report is sent after test completion \n
               1      Report is send only upon a failure \n
     */
    rlUInt8_t  repMode;
    /**
     * @brief  Value  Definition \n
               0      Production mode. Latent faults are tested and any failures are reported \n
               1      Characterization mode. Faults are injected and failures are reported which \n
                      allows testing of the failure reporting \n
     */
    rlUInt8_t  testMode;
#else
    /**
     * @brief  Value  Definition \n
               0      Production mode. Latent faults are tested and any failures are reported \n
               1      Characterization mode. Faults are injected and failures are reported which \n
                      allows testing of the failure reporting \n
     */
    rlUInt8_t  testMode;
    /**
    * @brief  Value  Definition \n
              0      Report is sent after test completion \n
              1      Report is send only upon a failure \n
    */
    rlUInt8_t  repMode;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved;

}rllatentFault_t;

/*! \brief
* mmwave radar periodicity test config
*/
typedef struct rlperiodicTest
{   /**
     * @brief  1 LSB = 1 ms Periodicity at which tests need to be run
     */
    rlUInt32_t periodicity;
    /**
    * @brief  Bit value definition: 1 - Enable, 0 - Disable \n
                Bit Monitoring type \n
                0   PERIODIC_CONFG_REGISTER_READ_EN \n
                1   ESM_MONITORING_EN \n
                31:2  RESERVED \n
    */
    rlUInt32_t testEn;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value   Definition \n
               0      Report is sent every monitoring period \n
               1      Report is sent only on a failure \n
     */
    rlUInt8_t  repMode;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t  reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t  reserved0;
    /**
    * @brief  Value   Definition \n
               0      Report is sent every monitoring period \n
               1      Report is sent only on a failure \n
    */
    rlUInt8_t  repMode;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
}rlperiodicTest_t;

/*! \brief
* mmwave radar test pattern config
*/
typedef struct rltestPattern
{
#ifndef MMWL_BIG_ENDIAN
     /**
      * @brief  This field controls the enable-disable of the generation of the test pattern. \n
                    Value   Description \n
                    0x0     Disable test pattern generation \n
                    0x1     Enable test pattern generation \n
                    0x2     Frame clock calibration mode - Enable test pattern generation \n
     */
    rlUInt8_t  testPatGenCtrl;
    /**
     * @brief  Number of system clocks (200 MHz) between successive samples for the test pattern \n
                    gen. Applicable only in case of Test pattern enable. Else ignored. \n
     */
    rlUInt8_t  testPatGenTime;
#else
    /**
     * @brief  Number of system clocks (200 MHz) between successive samples for the test pattern \n
                    gen. Applicable only in case of Test pattern enable. Else ignored. \n
     */
    rlUInt8_t  testPatGenTime;
     /**
      * @brief  This field controls the enable-disable of the generation of the test pattern. \n
                    Value   Description \n
                    0x0     Disable test pattern generation \n
                    0x1     Enable test pattern generation \n
     */
    rlUInt8_t  testPatGenCtrl;
#endif
    /**
     * @brief  Number of ADC samples to capture for each RX
                Valid range: 64 to MAX_NUM_SAMPLES,
                Where MAX_NUM_SAMPLES is such that all the enabled RX channels’ data fits into \n
                16 kB memory, with each sample consuming 2 bytes for real ADC output case and 4 \n
                bytes for complex 1x and complex 2x ADC output cases. For example:
                4 RX, Complex ADC output -> MAX_NUM_SAMPLES = 1024
                4 RX, Real ADC output -> MAX_NUM_SAMPLES = 2048
                2 RX, Complex ADC output -> MAX_NUM_SAMPLES = 2048
                2 RX, Real ADC output -> MAX_NUM_SAMPLES = 4096
     */
    rlUInt16_t testPatrnPktSize;
    /**
     * @brief Number of test pattern packets to send, for infinite packets set it to 0
     */
    rlUInt32_t numTestPtrnPkts;
    /**
     * @brief  This field specifies the values for Rx0, I channel. Applicable only in case of \n
                    test pattern enable. Else ignored. Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data  \n
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data \n
     */
    rlUInt32_t testPatRx0Icfg;
    /**
     * @brief  This field specifies the values for Rx0, Q channel. Applicable only in case of \n
                   test pattern enable. Else ignored. \n
                    Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data \n
     */
    rlUInt32_t testPatRx0Qcfg;
    /**
     * @brief  This field specifies the values for Rx1, I channel. Applicable only in case of \n
                   test pattern enable. Else ignored. \n
                    Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data
     */
    rlUInt32_t testPatRx1Icfg;
    /**
     * @brief  This field specifies the values for Rx1, Q channel. \n
                    Applicable only in case of test pattern enable. Else ignored. \n
                    Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data
     */
    rlUInt32_t testPatRx1Qcfg;
    /**
     * @brief  This field specifies the values for Rx2, I channel. \n
                    Applicable only in case of test pattern enable. Else ignored. \n
                    Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data
     */
    rlUInt32_t testPatRx2Icfg;
    /**
     * @brief  This field specifies the values for Rx2, Q channel. \n
                    Applicable only in case of test pattern enable. Else ignored. \n
                    Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data
     */
    rlUInt32_t testPatRx2Qcfg;
    /**
     * @brief  This field specifies the values for Rx3, I channel. \n
                    Applicable only in case of test pattern enable. Else ignored. \n
                    Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data
     */
    rlUInt32_t testPatRx3Icfg;
    /**
     * @brief  This field specifies the values for Rx3, Q channel. \n
                    Applicable only in case of test pattern enable. Else ignored. \n
                    Bits    Description \n
                    [15:0]  Start offset value to be used for the first sample for the test \n
                            pattern data
                    [31:16] Value to be added for each successive sample for the test pattern \n
                            data
     */
    rlUInt32_t testPatRx3Qcfg;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
}rltestPattern_t;

/*! \brief
* mmwave radar data format config
*/
typedef struct rlDevDataFmtCfg
{
    /**
     * @brief  RX Channel Bitmap \n
                    b0 RX0 Channel Enable \n
                    0 Disable RX Channel 0 \n
                    1 Enable RX Channel 0 \n
                    b1 RX1 Channel Enable \n
                    0 Disable RX Channel 1 \n
                    1 Enable RX Channel 1 \n
                    b2 RX2 Channel Enable \n
                    0 Disable RX Channel 2 \n
                    1 Enable RX Channel 2 \n
                    b3 RX3 Channel Enable \n
                    0 Disable RX Channel 3 \n
                    1 Enable RX Channel 3 \n
     */
    rlUInt16_t rxChannelEn;
    /**
     * @brief  ADC out bits - 0(12 Bits), 1(14 Bits), 2(16 Bits)
     */
    rlUInt16_t adcBits;
    /**
     * @brief  ADC out format - 0(Real), 1(Complex), 2(Complex with Image band), 3(Pseudo Real)
     */
    rlUInt16_t adcFmt;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  I/Q Swap selection for complex outputs \n
                  0 Sample interleave mode - I first \n
                  1 Sample interleave mode - Q first \n
                  others reserved \n
     */
    rlUInt8_t iqSwapSel;
    /**
     * @brief  Channel interleaving of the samples stored in \n
                  the ADC buffer to be transferred out on the data path \n
                  0 - Interleaved \n
                  1 - Non Interleaved \n
                  others reserved \n
     */
    rlUInt8_t chInterleave;
#else
    /**
     * @brief  Channel interleaving of the samples stored in \n
                  the ADC buffer to be transferred out on the data path \n
                  0 - Interleaved \n
                  1 - Non Interleaved \n
                  others reserved \n
     */
    rlUInt8_t chInterleave;
    /**
     * @brief  I/Q Swap selection for complex outputs \n
                 0 Sample interleave mode - I first \n
                 1 Sample interleave mode - Q first \n
                 others reserved \n
     */
    rlUInt8_t iqSwapSel;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved;
}rlDevDataFmtCfg_t;

/*! \brief
* mmwave radar data path config.
*/
typedef struct rlDevDataPathCfg
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Data Path Interface, \n
               0x0  CSI2 interface selected \n
               0x1  LVDS interface selected \n
               0x2  SPI interface selected (applicable when a large inter-frame time is
                   provided to transfer the data over the SPI), data will be chunked into multiple
                   async event message where message SB-ID is RL_DEV_AE_MSS_ADC_DATA_SB \n
     */
    rlUInt8_t intfSel;
    /**
     * @brief  Data out Format, \n
                 b5:0 Packet 0 content selection \n
                 000001 - ADC_DATA_ONLY \n
                 000110 - CP_ADC_DATA \n
                 001001 - ADC_CP_DATA \n
                 110110 - CP_ADC_CQ_DATA \n
                 b7:6 Packet 0 virtual channel number (valid only for CSI2) \n
                 00   Virtual channel number 0 (Default) \n
                 01   Virtual channel number 1 \n
                 10   Virtual channel number 2 \n
                 11   Virtual channel number 3 \n
     */
    rlUInt8_t transferFmtPkt0;
    /**
     * @brief  Data out Format, \n
                 b5:0 Packet 1 content selection \n
                 000000 - Suppress Packet 1 \n
                 001110 - CP_CQ_DATA \n
                 001011 - CQ_CP_DATA \n
                 b7:6 Packet 1 virtual channel number (valid only for CSI2) \n
                 00   Virtual channel number 0 (Default) \n
                 01   Virtual channel number 1 \n
                 10   Virtual channel number 2 \n
                 11   Virtual channel number 3 \n
     */
    rlUInt8_t transferFmtPkt1;
    /**
     * @brief  This field specifies the data size of CQ samples on the lanes. \n
               b1:0   Data size \n
                00    12 bits   \n
                01    14 bits   \n
                10    16 bits   \n
              b7:2    Reserved
     */
    rlUInt8_t cqConfig;
    /**
     * @brief  Number of samples (in 16 bit halfwords) of CQ0 data to be
                transferred. Valid range [32 halfwords to 128 halfwords] \n
                Value 0 = Disabled \n
                Note: Ensure that the number of halfwords specified are a multiple
                of the number of lanes selected.
     */
    rlUInt8_t cq0TransSize;
    /**
     * @brief  Number of samples (in 16 bit halfwords) of CQ1 data to be
                transferred. Valid range [32 halfwords to 128 halfwords] \n
                Value 0 = Disabled \n
                Note: Ensure that the number of halfwords specified are a multiple
                of the number of lanes selected.
     */
    rlUInt8_t cq1TransSize;
    /**
     * @brief  Number of samples (in 16 bit halfwords) of CQ2 data to be
                transferred. Valid range [32 halfwords to 128 halfwords] \n
                Value 0 = Disabled \n
                Note: Ensure that the number of halfwords specified are a multiple
                of the number of lanes selected.
     */
    rlUInt8_t cq2TransSize;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
#else
    /**
     * @brief  Data out Format, \n
                 b5:0 Packet 0 content selection \n
                 000001 - ADC_DATA_ONLY \n
                 000110 - CP_ADC_DATA \n
                 001001 - ADC_CP_DATA \n
                 110110 - CP_ADC_CQ_DATA \n
                 b7:6 Packet 0 virtual channel number (valid only for CSI2) \n
                 00   Virtual channel number 0 (Default)\n
                 01   Virtual channel number 1 \n
                 10   Virtual channel number 2 \n
                 11   Virtual channel number 3 \n
     */
    rlUInt8_t transferFmtPkt0;
    /**
     * @brief  Data Path Interface, \n
               0x0  CSI2 interface selected \n
               0x1  LVDS interface selected \n
               0x2  SPI interface selected (applicable when a large inter-frame time is
                   provided to transfer the data over the SPI), data will be chunked into multiple
                   async event message where message SB-ID is RL_DEV_AE_MSS_ADC_DATA_SB \n
     */
    rlUInt8_t intfSel;
    /**
     * @brief  This field specifies the data size of CQ samples on the lanes. \n
               b1:0   Data size \n
                00    12 bits   \n
                01    14 bits   \n
                10    16 bits   \n
              b7:2    Reserved
     */
    rlUInt8_t cqConfig;
    /**
     * @brief  Data out Format, \n
                 b5:0 Packet 1 content selection \n
                 000000 - Suppress Packet 1 \n
                 001110 - CP_CQ_DATA \n
                 001011 - CQ_CP_DATA \n
                 b7:6 Packet 1 virtual channel number (valid only for CSI2) \n
                 00   Virtual channel number 0 (Default) \n
                 01   Virtual channel number 1 \n
                 10   Virtual channel number 2 \n
                 11   Virtual channel number 3 \n
     */
    rlUInt8_t transferFmtPkt1;
    /**
     * @brief  Number of samples (in 16 bit halfwords) of CQ2 data to be
                transferred. Valid range [32 halfwords to 128 halfwords] \n
                Value 0 = Disabled \n
                Note: Ensure that the number of halfwords specified are a multiple
                of the number of lanes selected.
     */
    rlUInt8_t cq2TransSize;
    /**
     * @brief  Number of samples (in 16 bit halfwords) of CQ1 data to be
                transferred. Valid range [32 halfwords to 128 halfwords] \n
                Value 0 = Disabled \n
                Note: Ensure that the number of halfwords specified are a multiple
                of the number of lanes selected.
     */
    rlUInt8_t cq1TransSize;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved;
    /**
     * @brief  Number of samples (in 16 bit halfwords) of CQ0 data to be
                transferred. Valid range [32 halfwords to 128 halfwords] \n
                Value 0 = Disabled \n
                Note: Ensure that the number of halfwords specified are a multiple
                of the number of lanes selected.
     */
    rlUInt8_t cq0TransSize;
#endif
}rlDevDataPathCfg_t;

/*! \brief
* mmwave radar data path lane enable
*/
typedef struct rlDevLaneEnable
{
    /**
     * @brief  Lane Enable Bitmap  \n
                 b0 Lane 0 Enable  \n
                  0 Disable lane 0 \n
                  1 Enable lane 0  \n
                 b1 Lane 1 Enable  \n
                  0 Disable lane 1 \n
                  1 Enable lane 1  \n
                 b2 Lane 2 Enable  \n
                  0 Disable lane 2 \n
                  1 Enable lane 2  \n
                 b3 Lane 3 Enable  \n
                  0 Disable lane 3 \n
                  1 Enable lane 3  \n
                 b15:4 Reserved    \n
     */
    rlUInt16_t laneEn;
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved;
}rlDevLaneEnable_t;

/*! \brief
* DataPath clock configuration
*/
typedef struct rlDevDataPathClkCfg
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Clock COnfiguration \n
                 0 -SDR Clock \n
                 1 - DDR Clock (Only valid value for CSI2) \n
     */
    rlUInt8_t laneClkCfg;
    /**
     * @brief  Data rate selection \n
                 0000b - 900 Mbps (DDR only) \n
                 0001b - 600 Mbps (DDR only) \n
                 0010b - 450 Mbps (SDR, DDR) \n
                 0011b - 400 Mbps (DDR only) \n
                 0100b - 300 Mbps (SDR, DDR) \n
                 0101b - 225 Mbps (DDR only) \n
                 0110b - 150 Mbps (DDR only) \n
                 Others - Reserved \n
     */
    rlUInt8_t dataRate;
#else
    /**
     * @brief  Data rate selection \n
                 0000b - 900 Mbps (DDR only) \n
                 0001b - 600 Mbps (DDR only) \n
                 0010b - 450 Mbps (SDR, DDR) \n
                 0011b - 400 Mbps (DDR only) \n
                 0100b - 300 Mbps (SDR, DDR) \n
                 0101b - 225 Mbps (DDR only) \n
                 0110b - 150 Mbps (DDR only) \n
                 Others - Reserved \n
     */
    rlUInt8_t dataRate;
    /**
     * @brief  Clock COnfiguration \n
                 0 -SDR Clock \n
                 1 - DDR Clock (Only valid value for CSI2) \n
     */
    rlUInt8_t laneClkCfg;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved;
}rlDevDataPathClkCfg_t;

/*! \brief
* LVDS Lane configuration
*/
typedef struct rlDevLvdsLaneCfg
{
    /**
     * @brief  Lane format \n
                 0x0000 Format map 0 (Rx0,Rx1,...) \n
                 0x0001 Format map 1 (Rx3,Rx2,...) \n
     */
    rlUInt16_t laneFmtMap;
    /**
     * @brief  Lane Parameter configurations \n
                 b0 - 0(LSB first), 1(MSB first) \n
                 b1 - 0(Packet End Pulse Disable), 1(enable) \n
                 b2 - 0(CRC disabled), 1(CRC enabled)        \n
                 b7:3 - Reserved \n
                 b8 - Configures LSB/MSB first for CRC \n
                      0(CRC value swapped wrt to MSB_FIRST setting) \n
                      1(CRC value follows MSB_FIRST setting) \n
                 b9 - Frame clock state during idle \n
                      0(Frame clock is held low) \n
                      1(Frame clock is held high) \n
                 b10 - Frame clock period for CRC(when CRC enabled - b2) \n
                       0(32-bit CRC is trasmitted as single sample with frame clock set to \n
                         16high, 16low configuration) \n
                       1(32-bit CRC is trasmitted as single sample with frame clock set to \n
                         8high, 8low configuration) \n
                 b11 - Bit clock state during idle \n
                       0(Bit clock toggles during idle when there are no transmission) \n
                       1(Bit clock doesn't toggle during idle when there are no transmission,  \n
                         the value of bit clock is held low) \n
                 b12 - CRC inversion control(when CRC enabled - b2) \n
                       0(The calcualted value of 32-bit ethernet polynomial CRC is inverted and \n
                         sent out) \n
                       1(The calcualted value of 32-bit ethernet polynomial CRC is sent without \n
                         inversion) \n
                 b15:13 - Reserved \n
     */
    rlUInt16_t laneParamCfg;
}rlDevLvdsLaneCfg_t;

/*! \brief
* Continous streaming mode configuration
*/
typedef struct rlDevContStreamingModeCfg
{
    /**
     * @brief  Enable - 1, Disable - 0
     */
    rlUInt16_t contStreamModeEn;
    /**
     * @brief  Reserved for future use \n
     */
    rlUInt16_t reserved;
}rlDevContStreamingModeCfg_t;

/*! \brief
* CSI2 configuration
*/
typedef struct rlDevCsi2Cfg
{
    /**
     * @brief  b2:0 - DATA_LANE0_POS \n
                Valid values (Should be a unique position if lane 0 is enabled, ignored if \n
                lane 0 is not enabled): \n
                000b - Unused, 001b - Position 1 (default), \n
                010b - Position 2, 011b - Position 3, \n
                100b - Position 4, 101b - Position 5 \n
                b3 DATA_LANE0_POL \n
                0b - PLUSMINUS pin order, 1b - MINUSPLUS pin order \n
                b6:4 DATA_LANE1_POS \n
                Valid values (Should be a unique position if lane 1 is \n
                enabled, ignored if lane 1 is not enabled): \n
                000b - Unused, 001b - Position 1, \n
                010b - Position 2 (default), 011b - Position 3, \n
                100b - Position 4, 101b - Position 5 \n
                b7 DATA_LANE1_POL \n
                0b - PLUSMINUS pin order, 1b - MINUSPLUS pin order \n
                b10:8 DATA_LANE2_POS \n
                Valid values (Should be a unique position if lane 2 is \n
                enabled, ignored if lane 2 is not enabled): \n
                000b - Unused, 001b - Position 1, \n
                010b - Position 2, 011b - Position 3, \n
                100b - Position 4 (default), 101b - Position 5 \n
                b11 DATA_LANE2_POL \n
                0b - PLUSMINUS pin order, 1b - MINUSPLUS pin order \n
                b14:12 DATA_LANE3_POS \n
                Valid values (Should be a unique position if lane 3 is \n
                enabled, ignored if lane 3 is not enabled): \n
                000b - Unused, 001b - Position 1, \n
                010b - Position 2, 011b - Position 3, \n
                100b - Position 4, 101b - Position 5 (default) \n
                b15 DATA_LANE3_POL \n
                0b - PLUSMINUS pin order, 1b - MINUSPLUS pin order \n
                b18:16 CLOCK_POS \n
                Valid values (Should be a unique position): \n
                001b - Position 1, \n
                010b - Position 2, 011b - Position 3 (default), \n
                100b - Position 4 \n
                b19 CLOCK_POL \n
                0b - PLUSMINUS pin order, 1b - MINUSPLUS pin order \n
                b31:20 RESERVED \n
     */
    rlUInt32_t lanePosPolSel;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved1;
}rlDevCsi2Cfg_t;

/*! \brief
* mmwave radar high speed clock configuration
*/
typedef struct rlDevHsiClk
{
    /**
     * @brief  High Speed Interface Clock configurations. Below table indicates possible values
               for different data rate supported \n
               HSICLKRATECODE (corresponding datarate in Mbps): \n
               SDR - 0x5(900 mbps), 0xA(600 mbps), 0x6(450 mbps), 0x2(400 mbps),
                     0xB(300 mbps), 0x7(225 mbps)  \n
               DDR - 0xD(900 mbps), 0x9(600 mbps), 0x5(450 mbps), 0x1(400 mbps),
                     0xA(300 mbps), 0x6(225 mbps ), 0xB(150 mbps) \n
     */
    rlUInt16_t hsiClk;
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved;
}rlDevHsiClk_t;

/*! \brief
* mmwave radar high speed Data path configuraiton
*/
typedef struct rlDevHsiCfg
{
     /**
      * @brief  Data format config
      */
    rlDevDataFmtCfg_t *datafmt;
     /**
      * @brief  Data path config
      */
    rlDevDataPathCfg_t *dataPath;
     /**
      * @brief  Data path clock configuration
      */
    rlDevDataPathClkCfg_t *dataPathClk;
}rlDevHsiCfg_t;

typedef struct rlDevConfig
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Set CRC type of Async Event message from MSS to Host \n
                 0 - 16 Bit CRC \n
                 1 - 32 Bit CRC \n
                 2 - 64 Bit CRC \n
     */
    rlUInt8_t aeCrcConfig;
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Set CRC type of Async Event message from MSS to Host \n
                 0 - 16 Bit CRC \n
                 1 - 32 Bit CRC \n
                 2 - 64 Bit CRC \n
     */
    rlUInt8_t aeCrcConfig;
#endif
    /**
     * @brief  Reserved for future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved2;
    /**
     * @brief  Reserved for future use
     */
    rlUInt32_t reserved3;
}rlDevMiscCfg_t;

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */

 /**
 *  @defgroup Device Device
 *  @brief Radar Device Management Module.
 *
 *  The Device module has interface for Enabling and controlling Radar device.
 *  Configures the callbacks(SPI, Interrupt, OS etc) for communication with device
 *  It also allows Firmware download over SPI.
 *
 *    Related Files
 *   - rl_device.c
 *  @addtogroup Device
 *  @{
 */

/* Device Interface Functions */
MMWL_EXPORT rlReturnVal_t rlDevicePowerOn(rlUInt8_t deviceMap,
                                          rlClientCbs_t clientCb);
MMWL_EXPORT rlReturnVal_t rlDeviceAddDevices(rlUInt8_t deviceMap);
MMWL_EXPORT rlReturnVal_t rlDeviceRemoveDevices(rlUInt8_t deviceMap);
MMWL_EXPORT rlReturnVal_t rlDevicePowerOff(void);

/* RF/DSP Start Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceRfStart(rlUInt8_t deviceMap);

/* File Download Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceFileDownload(rlUInt8_t deviceMap,
                                               rlFileData_t *data,
                                               rlUInt16_t remChunks);

/* Get Version Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceGetMssVersion(rlUInt8_t deviceMap,
                                                rlFwVersionParam_t *data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetRfVersion(rlUInt8_t deviceMap,
                                               rlFwVersionParam_t *data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetVersion(rlUInt8_t deviceMap,
                                             rlVersion_t *data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetMmWaveLinkVersion(rlSwVersionParam_t *data);

/* MCU Clock configuration Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceMcuClkConfig(rlUInt8_t deviceMap,
                                               rlMcuClkCfg_t *data);

/* PMIC Clock configuration Functions */
MMWL_EXPORT rlReturnVal_t rlDevicePmicClkConfig(rlUInt8_t deviceMap,
                                                rlPmicClkCfg_t *data);

/* Latetnt fault test configuration Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceLatentFaultTests(rlUInt8_t deviceMap,
                                                   rllatentFault_t *data);

/* Periodic test configuration Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceEnablePeriodicTests(rlUInt8_t deviceMap,
                                                      rlperiodicTest_t *data);

/* Test pattern configuration Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceSetTestPatternConfig(rlUInt8_t deviceMap,
                                                       rltestPattern_t *data);

/* mmWaveLink Protocol configuration Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceConfigureCrc(rlCrcType_t crcType);
MMWL_EXPORT rlReturnVal_t rlDeviceConfigureAckTimeout(rlUInt32_t ackTimeout);

/* Continuous streaming mode Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceSetContStreamingModeConfig(rlUInt8_t deviceMap,
                                                             rlDevContStreamingModeCfg_t *data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetContStreamingModeConfig(rlUInt8_t deviceMap,
                                                             rlDevContStreamingModeCfg_t *data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetMiscConfig(rlUInt8_t deviceMap,
                                                rlDevMiscCfg_t *data);

/*!
 Close the Doxygen group.
 @}
 */

 /**
 *  @defgroup Data_Path Data Path
 *  @brief mmWave Radar Data Path(LVDS/CSI2) Module.
 *
 *  The Data path module has interface for Enabling and controlling high speed
 *  data interface such as CSI2 and LVDS. Configures the data format, data rate,
 *  lane parameters. \n
 *  Below diagram shows the data transfer for different data formats and lanes
 *  on high speed interface
 *
 *  @image html data_path_lanes.png
 *
 *    Related Files
 *   - rl_device.c
 *  @addtogroup Data_Path
 *  @{
 */
/*data Path(LVDS/CSI2) configuration Functions */
MMWL_EXPORT rlReturnVal_t rlDeviceSetDataFmtConfig(rlUInt8_t deviceMap,
                                                   rlDevDataFmtCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetDataFmtConfig(rlUInt8_t deviceMap,
                                                   rlDevDataFmtCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetDataPathConfig(rlUInt8_t deviceMap,
                                                    rlDevDataPathCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetDataPathConfig(rlUInt8_t deviceMap,
                                                    rlDevDataPathCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetLaneConfig(rlUInt8_t deviceMap,
                                                rlDevLaneEnable_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetLaneConfig(rlUInt8_t deviceMap,
                                                rlDevLaneEnable_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetDataPathClkConfig(rlUInt8_t deviceMap,
                                                       rlDevDataPathClkCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetDataPathClkConfig(rlUInt8_t deviceMap,
                                                       rlDevDataPathClkCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetLvdsLaneConfig(rlUInt8_t deviceMap,
                                                    rlDevLvdsLaneCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetLvdsLaneConfig(rlUInt8_t deviceMap,
                                                    rlDevLvdsLaneCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetHsiConfig(rlUInt8_t deviceMap,
                                               rlDevHsiCfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetHsiClk(rlUInt8_t deviceMap,
                                            rlDevHsiClk_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceSetCsi2Config(rlUInt8_t deviceMap,
                                                rlDevCsi2Cfg_t*data);
MMWL_EXPORT rlReturnVal_t rlDeviceGetCsi2Config(rlUInt8_t deviceMap,
                                                rlDevCsi2Cfg_t*data);



/*!
 Close the Doxygen group.
 @}
 */

#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF RL_DEVICE_H
 */

