/****************************************************************************************
 * FileName     : rl_monitoring.h
 *
 * Description  : This file defines the functions required for Monitoring.
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
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
#ifndef RL_MONITORING_H
#define RL_MONITORING_H

/******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief
* Supported maximum number of RF frequencies for monitoring
*/
#define RL_MON_RF_FREQ_CNT                      (3U)

/*! \brief
* Max Number of (primary + secondary) slices to monitor
*/
#define RL_NUM_MON_SLICES_MAX                   (127U)


/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/*! \brief
* Digital monitoring configuration
*/
typedef struct rlMonDigEnables
{
    /**
     * @brief  Bit: Dig Monitoring \n
                 0 Reserved \n
                 1 CR4 and VIM lockstep test of diagnostic \n
                 2 Reserved \n
                 3 VIM test of diagnostic \n
                 4 Reserved \n
                 5 Reserved \n
                 6 CRC test of diagnostic \n
                 7 RAMPGEN memory ECC test of diagnostic \n
                 8 DFE Parity test of diagnostic \n
                 9 DFE memory ECC test of diagnostic \n
                 10 RAMPGEN lockstep test of diagnostic \n
                 11 FRC lockstep test of diagnostic \n
                 12 Reserved \n
                 13 Reserved \n
                 14 Reserved \n
                 15 Reserved \n
                 16 ESM test of diagnostic \n
                 17 DFE STC \n
                 18 Reserved \n
                 19 ATCM, BTCM ECC test of diagnostic \n
                 20 ATCM, BTCM parity test of diagnostic \n
                 21 Reserved \n
                 22 Reserved \n
                 23 Reserved \n
                 24 FFT test of diagnostic \n
                 25 RTI test of diagnostic \n
                 26 PCR test of diagnostic \n
                 31:27 RESERVED \n
     */
    rlUInt32_t enMask;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value    Definition \n
                 0     Production mode. Latent faults are tested and  any failures are reported \n
                 1     Characterization mode. Faults are injected and failures are reported
                       which allows testing of the failure reporting path  \n
     */
    rlUInt8_t testMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Value    Definition \n
                 0     Production mode. Latent faults are tested and  any failures are reported \n
                 1     Characterization mode. Faults are injected and failures are reported
                       which allows testing of the failure reporting path  \n
     */
    rlUInt8_t testMode;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlMonDigEnables_t;

/*! \brief
* Digital monitoring latent fault reporting configuration
*/
typedef struct rlDigMonPeriodicConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value    Definition \n
                 0     Report is sent every monitoring period \n
                 1     Report is sent only on a failure \n
                 2     RESERVED \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Value    Definition \n
                 0     Report is sent every monitoring period \n
                 1     Report is sent only on a failure \n
                 2     RESERVED \n
     */
    rlUInt8_t  reportMode;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t  reserved1;
    /**
     * @brief  Bit     Monitoring \n
               0      PERIODIC_CONFG_REGISTER_READ_EN \n
               1      ESM_MONITORING_EN \n
               2      DFE_STC_EN \n
               3      FRAME_TIMING_MONITORING_EN \n
               31:4   RESERVED \n
     */
    rlUInt32_t periodicEnableMask;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlDigMonPeriodicConf_t;

/*! \brief
* Analog monitoring configuration
*/
typedef struct rlMonAnaEnables
{
    /**
     * @brief  Bit   Analog monitoring control \n
                0   TEMPERATURE_MONITOR_EN \n
                1   RX_GAIN_PHASE_MONITOR_EN \n
                2   RX_NOISE_MONITOR_EN \n
                3   RX_IFSTAGE_MONITOR_EN \n
                4   TX0_POWER_MONITOR_EN \n
                5   TX1_POWER_MONITOR_EN \n
                6   TX2_POWER_MONITOR_EN \n
                7   TX0_BALLBREAK_MONITOR_EN \n
                8   TX1_BALLBREAK_MONITOR_EN \n
                9   TX2_BALLBREAK_MONITOR_EN \n
                10  TX_GAIN_PHASE_MONITOR_EN \n
                11  TX0_BPM_MONITOR_EN \n
                12  TX1_BPM_MONITOR_EN \n
                13  TX2_BPM_MONITOR_EN \n
                14  SYNTH_FREQ_MONITOR_EN \n
                15  EXTERNAL_ANALOG_SIGNALS_MONITOR_EN \n
                16  INTERNAL_TX0_SIGNALS_MONITOR_EN \n
                17  INTERNAL_TX1_SIGNALS_MONITOR_EN \n
                18  INTERNAL_TX2_SIGNALS_MONITOR_EN \n
                19  INTERNAL_RX_SIGNALS_MONITOR_EN \n
                20  INTERNAL_PMCLKLO_SIGNALS_MONITOR_EN \n
                21  INTERNAL_GPADC_SIGNALS_MONITOR_EN \n
                22  PLL_CONTROL_VOLTAGE_MONITOR_EN \n
                23  DCC_CLOCK_FREQ_MONITOR_EN \n
                24  RX_IF_SATURATION_MONITOR_EN \n
                25  RX_SIG_IMG_BAND_MONITORING_EN \n
                26  RX_MIXER_INPUT_POWER_MONITOR \n
                31:27   RESERVED \n
     */
    rlUInt32_t enMask;
    /**
     * @brief  LDO short circuit monitoring enable. There are no reports for these monitors. \n
               If there is any fault, the asyncevent RL_RF_AE_ANALOG_FAULT_SB will be sent.
                 Bit    Description
                 b0     APLL LDO short circuit monitoring
                        0 - disable, 1 - enable
                 b1     SYNTH VCO LDO short circuit monitoring
                        0 - disable, 1 - enable
                 b2     PA LDO short circuit monitoring
                        0 - disable, 1 - enable
                 b31:3 RESERVED
     */
    rlUInt32_t ldoScEn;
} rlMonAnaEnables_t;

/*! \brief
* Temperature sensor monitoring configuration
*/
typedef struct rlTempMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value    Definition \n
                0      Report is sent every monitoring period without threshold check \n
                1      Report is send only upon a failure (after checking for thresholds) \n
                2      Report is sent every monitoring period with threshold check \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Value    Definition \n
                0      Report is sent every monitoring period without threshold check \n
                1      Report is send only upon a failure (after checking for thresholds) \n
                2      Report is sent every monitoring period with threshold check \n
     */
    rlUInt8_t  reportMode;
#endif
    /**
     * @brief  The temperatures read from near the sensors near the RF analog modules
                 are compared against a minimum threshold. The comparison result is part
                 of the monitoring report message (Error bit is set if any measurement
                 is outside this (minimum, maximum) range).\n
                 1 LSB = 1 degree Celsius, signed number \n
                 Valid range: -99 degree Celsius to 199 degree Celsius \n
     */
    rlInt16_t  anaTempThreshMin;
    /**
     * @brief  The temperatures read from near the sensors near the RF analog modules
                 are compared against a maximum threshold. The comparison result is part
                 of the monitoring report message (Error bit is set if any measurement
                 is outside this (minimum, maximum) range). \n
                 1 LSB = 1 degree Celsius, signed number \n
                 Valid range: -99 degree Celsius to 199 degree Celsius \n
     */
    rlInt16_t  anaTempThreshMax;
    /**
     * @brief  The temperatures read from near the sensor near the digital module are
                  compared against a minimum threshold. The comparison result is part of
                  the monitoring report message (Error bit is set if any measurement is
                  outside this (minimum, maximum) range). \n
                  1 LSB = 1 degree Celsius, signed number \n
                  Valid range: -99 degree Celsius to 199 degree Celsius \n
     */
    rlInt16_t  digTempThreshMin;
    /**
     * @brief  The temperatures read from near the sensor near the digital module are
                 compared against a maximum threshold. The comparison result is part of
                 the monitoring report message (Error bit is set if any measurement is
                 outside this (minimum, maximum) range). \n
                 1 LSB = 1 degree Celsius, signed number \n
                 Valid range: -99 degree Celsius to 199 degree Celsius \n
     */
    rlInt16_t  digTempThreshMax;
    /**
     * @brief  The maximum difference across temperatures read from all the enabled
                 sensors is compared against this threshold.The comparison result is part
                 of the monitoring report message(Error bit is set if the measured difference
                 exceeds this field). \n
                 1 LSB = 1o Celsius, unsigned number \n
                 Valid range: 0 degree Celsius to 100 degree Celsius \n
     */
    rlUInt16_t tempDiffThresh;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlTempMonConf_t;

/*! \brief
* RX gain and phase monitoring configuration
*/
typedef struct rlRxGainPhaseMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  This field indicates the RF frequencies inside the profile's RF band at which to
                 measure the required parameters. When each bit in this field is set, the
                 measurement at the corresponding RF frequency is enabled w.r.t. the profile's
                 RF band. \n
                 Bit number  RF frequency                        RF name \n
                     0       Lowest RF frequency                 RF1 \n
                             in profile's sweep bandwidth \n
                     1       Center RF frequency in profile's    RF2 \n
                             sweep bandwidth \n
                     2       Highest RF frequency in             RF3 \n
                             profile's sweep bandwidth \n
                 The RF name column is mentioned here to set the convention for the
                 purpose of reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Value     Definition \n
                0       TX0 is used for generating loopback signal for RX gain measurement \n
                1       TX1 is used for generating loopback signal for RX gain measurement. \n
     */
    rlUInt8_t txSel;
#else
    /**
     * @brief  This field indicates the RF frequencies inside the profile's RF band at which to  \n
                 measure the required parameters. When each bit in this field is set, the \n
                 measurement at the corresponding RF frequency is enabled w.r.t. the profile's  \n
                 RF band. \n
                 Bit number  RF frequency                        RF name \n
                     0       Lowest RF frequency                 RF1        \n
                             in profile's sweep bandwidth \n
                     1       Center RF frequency in profile's    RF2 \n
                             sweep bandwidth \n
                     2       Highest RF frequency in             RF3 \n
                             profile's sweep bandwidth \n
                 The RF name column is mentioned here to set the convention for the \n
                 purpose of reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  Value     Definition \n
                0       TX0 is used for generating loopback signal for RX gain measurement \n
                1       TX1 is used for generating loopback signal for RX gain measurement. \n
     */
    rlUInt8_t txSel;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
#endif
    /**
     * @brief  The magnitude of difference between the programmed and measured RX gain for each \n
                 enabled channel at each enabled RF frequency, is compared against this \n
                 threshold. The comparison result is part of the monitoring report message \n
                 (Error bit is set if any measurement is above this threshold). Before the \n
                 comparison, the measured gains for each RF and RX are adjusted by subtracting \n
                 the offset given in the RX_GAIN_MISMATCH_OFFSET_VALUE field \n
                 1 LSB = 0.1 dB \n
                 Valid range: 0 to 60 (0 to 6dB) \n
     */
    rlUInt16_t rxGainAbsThresh;
    /**
     * @brief  The magnitude of difference between measured RX gains across the enabled channels \n
                 at each enabled RF frequency is compared against this threshold. The comparison \n
                 result is part of the monitoring report message (Error bit is set if the \n
                 measurement is above this threshold). Before the comparison, the measured gains \n
                 for each RF and RX are adjusted by subtracting the offset given in the \n
                 RX_GAIN_MISMATCH_OFFSET_VALUE field. \n
                 1 LSB = 0.1 dB \n
                 Valid range: 0 to 60 (0 to 6dB) \n
     */
    rlUInt16_t rxGainMismatchErrThresh;
    /**
     * @brief  The magnitude of measured RX gain flatness error, for each enabled channel, is \n
                 compared against this threshold. The flatness error for a channel is defined as \n
                 the peak to peak variation across RF frequencies. The comparison result is part \n
                 of the monitoring report message (Error bit is set if any measurement is above \n
                 this threshold). Before the comparison, the measured gains for each RF and RX \n
                 are adjusted by subtracting the offset given in the \n
                 RX_GAIN_MISMATCH_OFFSET_VALUE field. \n
                 1 LSB = 0.1 dB \n
                 Valid range: 0 to 60 (0 to 6dB) \n
                 This flatness check is applicable only if multiple RF Frequencies are enabled,  \n
                 i.e., RF_FREQ_BITMASK has bit numbers 0,1,2 set \n
     */
    rlUInt16_t rxGainFlatnessErrThresh;
    /**
     * @brief  The magnitude of measured RX phase mismatch across the enabled channels at each \n
                 enabled RF frequency is compared against this threshold. The comparison result \n
                 is part of the monitoring report message (Error bit is set if any measurement \n
                 is above this threshold). Before the comparison, the measured phases for each \n
                 RF and RX are adjusted by subtracting the offset given in the \n
                 RX_PHASE_MISMATCH_OFFSET_VALUE field. \n
                 1 LSB = 360(degree) / 2^16 . \n
                 Valid range: corresponding to 0 degree to 20 degree. \n
     */
    rlUInt16_t rxGainPhaseMismatchErrThresh;
    /**
     * @brief  The offsets to be subtracted from the measured RX gain for each RX and RF before \n
                 the relevant threshold comparisons are given here. \n
                 Byte numbers corresponding to different RX and RF, in this field are \n
                 here: \n
                         RF1     RF2     RF3 \n
                 RX0    [1:0]   [9:8]    [17:16] \n
                 RX1    [3:2]   [11:10]  [19:18] \n
                 RX2    [5:4]   [13:12]  [21:20] \n
                 RX3    [7:6]   [15:14]  [23:22] \n
                 1 LSB = 0.1 dB, signed number \n
                 Only the entries of enabled RF Frequencies and enabled RX channels are \n
                 considered. \n
     */
    rlInt16_t rxGainMismatchOffsetVal[RL_RX_CNT][RL_MON_RF_FREQ_CNT];
    /**
     ** @brief  The offsets to be subtracted from the measured RX phase for each RX and RF \n
                 before the relevant threshold comparisons are given here. Byte numbers \n
                 corresponding to different RX and RF, in this field are here: \n
                         RF1       RF2       RF3 \n
                 RX0     [1:0]     [9:8]     [17:16] \n
                 RX1     [3:2]     [11:10]   [19:18] \n
                 RX2     [5:4]     [13:12]   [21:20] \n
                 RX3     [7:6]     [15:14]   [23:22] \n
                 1 LSB = 360(degree) / 2^16 , unsigned number \n
                 Only the entries of enabled RF Frequencies and enabled RX channels are \n
                 considered. \n
     */
    rlUInt16_t rxGainPhaseMismatchOffsetVal[RL_RX_CNT][RL_MON_RF_FREQ_CNT];
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlRxGainPhaseMonConf_t;

/*! \brief
* RX noise monitoring configuration
*/
typedef struct rlRxNoiseMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  This field indicates the exact RF frequencies inside the profile's RF band at \n
                 which to measure the required parameters. When each bit in this field is set, \n
                 the measurement at the corresponding RF frequency is enabled w.r.t. the \n
                 profile's RF band. \n
                 Bit number   RF frequency                    RF name \n
                     0        Lowest RF frequency in          RF1 \n
                             profile's sweep bandwidth \n
                     1        Center RF frequency in          RF2 \n
                             profile's sweep bandwidth \n
                     2        Highest RF frequency in         RF3 \n
                             profile's sweep bandwidth \n
                 The RF name column is mentioned here to set the convention for the purpose of \n
                 reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
#else
     /**
     * @brief  This field indicates the exact RF frequencies inside the profile's RF band at \n
                 which to measure the required parameters. When each bit in this field is set, \n
                 the measurement at the corresponding RF frequency is enabled w.r.t. the \n
                 profile's RF band. \n
                 Bit number   RF frequency                    RF name \n
                     0        Lowest RF frequency in          RF1 \n
                             profile's sweep bandwidth \n
                     1        Center RF frequency in          RF2 \n
                             profile's sweep bandwidth \n
                     2        Highest RF frequency in         RF3 \n
                             profile's sweep bandwidth \n
                 The RF name column is mentioned here to set the convention for the purpose of \n
                 reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved0;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value        Definition \n
                0          Report is sent every monitoring period without threshold check \n
                1          Report is send only upon a failure (after checking for thresholds) \n
                2          Report is sent every monitoring period with threshold check  \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
    /**
     * @brief  Value        Definition \n
                0          Report is sent every monitoring period without threshold check \n
                1          Report is send only upon a failure (after checking for thresholds) \n
                2          Report is sent every monitoring period with threshold check  \n
     */
    rlUInt8_t  reportMode;
#endif
    /**
     * @brief  The measured RX input referred noise figure at the enabled RF frequencies, for \n
                 all channels, is compared against this threshold. The comparison result is part \n
                 of the monitoring report message (Error bit is set if any measurement is above \n
                 this threshold). \n
                 1 LSB = 0.1 dB \n
                 Valid range: 0 to 300 (0 to 30dB) \n
     */
    rlUInt16_t noiseThresh;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlRxNoiseMonConf_t;

/*! \brief
* RX IF stage monitoring configuration
*/
typedef struct rlRxIfStageMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  Value       Definition \n
                 0        Report is sent every monitoring period without threshold check \n
                 1        Report is send only upon a failure (after checking for thresholds) \n
                 2        Report is sent every monitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
#else
    /**
     * @brief  Value       Definition \n
                 0        Report is sent every monitoring period without threshold check \n
                 1        Report is send only upon a failure (after checking for thresholds) \n
                 2        Report is sent every monitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t  reserved0;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t  reserved1;
    /**
     * @brief  The absolute values of RX IF HPF cutoff percentage frequency errors are \n
                 compared against the corresponding thresholds given in this field. The \n
                 comparison results are part of the monitoring report message (Error bit is set \n
                 if the absolute value of the errors exceeds respective thresholds). \n
                 1 LSB = 1%, unsigned number \n
                 Valid range: 1% to 99% \n
     */
    rlUInt16_t hpfCutoffErrThresh;
    /**
     * @brief  The absolute values of RX IF LPF cutoff percentage frequency errors are compared \n
                 against the corresponding thresholds given in this field. The comparison \n
                 results are part of the monitoring report message (Error bit is set if the \n
                 absolute value of the errors exceeds respective thresholds). \n
                 1 LSB = 1%, unsigned number \n
                 Valid range: 1% to 99% \n
     */
    rlUInt16_t lpfCutoffErrThresh;
    /**
     * @brief  The absolute deviation of RX IFA Gain from the expected gain for each enabled RX \n
                 channel is compared against the thresholds given in this field. The comparison \n
                 result is part of the monitoring report message (Error bit is set if the \n
                 absolute \n
                 value of the errors exceeds respective thresholds). \n
                 1 LSB = 0.1dB, unsigned number \n
                 Valid range: 0 to 60 (0 to 6dB) \n
     */
    rlUInt16_t ifaGainErrThresh;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlRxIfStageMonConf_t;

/*! \brief
* TX power monitoring configuration
*/
typedef struct rlTxPowMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  This field indicates the exact RF frequencies inside the profile's RF band at \n
                 which to measure the required parameters. When each bit in this field is set, \n
                 the measurement at the corresponding RF frequency is enabled w.r.t. the \n
                 profile's RF band. \n

                 Bit number      RF frequency                            RF \n
                     0           Lowest RF frequency in profile's        RF1 \n
                                 sweep bandwidth \n
                     1           Center RF frequency in profile's        RF2 \n
                                 sweep bandwidth \n
                     2           Highest RF frequency in profile's       RF3 \n
                                 sweep bandwidth \n
                 The RF Name column is mentioned here to set the convention for the purpose \n
                 of reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
#else
    /**
     * @brief  This field indicates the exact RF frequencies inside the profile's RF band at \n
                 which to measure the required parameters. When each bit in this field is set, \n
                 the measurement at the corresponding RF frequency is enabled w.r.t. the \n
                 profile's RF band. \n

                 Bit number      RF frequency                            RF \n
                     0           Lowest RF frequency in profile's        RF1 \n
                                 sweep bandwidth \n
                     1           Center RF frequency in profile's        RF2 \n
                                 sweep bandwidth \n
                     2           Highest RF frequency in profile's       RF3 \n
                                 sweep bandwidth \n
                 The RF Name column is mentioned here to set the convention for the purpose \n
                 of reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved0;
#ifndef MMWL_BIG_ENDIAN
    /*!< Value    Definition \n
            0     Report is sent every monitoring period without threshold check \n
            1     Report is send only upon a failure (after checking for thresholds) \n
            2     Report is sent everymonitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
    /*!< Value    Definition \n
            0     Report is sent every monitoring period without threshold check \n
            1     Report is send only upon a failure (after checking for thresholds) \n
            2     Report is sent everymonitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
#endif
    /**
     * @brief  The magnitude of difference between the programmed and measured TX power for \n
                 each enabled channel at each enabled RF frequency, is compared against this \n
                 threshold. The comparison result is part of the monitoring report message(Error \n
                 bit is set if any measurement is above this threshold). \n
                 1 LSB = 0.1 dB \n
                 Valid range: 0 to 60 (0 to 6dB) \n
     */
    rlUInt16_t txPowAbsErrThresh;
    /**
     * @brief  The magnitude of measured TX power flatness error, for each enabled channel, is \n
                 compared against this threshold. The flatness error for a channel is defined as \n
                 the peak to peak variation across RF frequencies. The comparison result is part \n
                 of the monitoring report message(Error bit is set if any measurement is above \n
                 this threshold). \n
                 1 LSB = 0.1 dB \n
                 Valid range: 0 to 60 (0 to 6dB) \n
                 This flatness check is applicable only if multiple RF Frequencies are enabled. \n
     */
    rlUInt16_t txPowFlatnessErrThresh;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved2;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved3;
} rlTxPowMonConf_t;


/*! \brief
* TX power monitoring configuration
*/
typedef struct rlAllTxPowMonConf
{
    /**
     * @brief  Power Monitoring Configuration for Tx0
     */
    rlTxPowMonConf_t *tx0PowrMonCfg;
    /**
     * @brief  Power Monitoring Configuration for Tx1
     */
    rlTxPowMonConf_t *tx1PowrMonCfg;
    /**
     * @brief  Power Monitoring Configuration for Tx2
     */
    rlTxPowMonConf_t *tx2PowrMonCfg;
}rlAllTxPowMonConf_t;

/*! \brief
* TX ballbreak monitoring configuration
*/
typedef struct rlTxBallbreakMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value    Definition \n
                0      Report is sent every monitoring period without threshold check \n
                1      Report is send only upon a failure (after checking for thresholds) \n
                2      Report is sent every monitoring. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Value    Definition \n
                0      Report is sent every monitoring period without threshold check \n
                1      Report is send only upon a failure (after checking for thresholds) \n
                2      Report is sent every monitoring. \n
     */
    rlUInt8_t  reportMode;
#endif
    /*!< The TX reflection coefficient's magnitude for each enabled channel is compared against \n
         the threshold given here. The comparison result is part of the monitoring report \n
         message (Error bit is set if the measurement is lower than this threshold, with the \n
         units of both quantities being the same). \n
         1 LSB = 0.1 dB, signed number \n
         Valid range: -90 to -250 (-9dB to -25dB) \n
     */
    rlInt16_t txReflCoeffMagThresh;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlTxBallbreakMonConf_t;

/*! \brief
* TX ballbreak monitoring configuration
*/
typedef struct rlAllTxBallBreakMonCfg
{
    /**
     * @brief  Tx ballbreak monitoring config for Tx0
     */
    rlTxBallbreakMonConf_t *tx0BallBrkMonCfg;
    /**
     * @brief  Tx ballbreak monitoring config for Tx1
     */
    rlTxBallbreakMonConf_t *tx1BallBrkMonCfg;
    /**
     * @brief  Tx ballbreak monitoring config for Tx2.
     */
    rlTxBallbreakMonConf_t *tx2BallBrkMonCfg;
}rlAllTxBallBreakMonCfg_t;

/*! \brief
* TX gain and phase mismatch monitoring configuration
*/
typedef struct rlTxGainPhaseMismatchMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the Profile Index for which this monitoring configuration \n
                 applies. The TX settings corresponding to this profile index are used during \n
                 the monitoring. The RX gain used in this measurement may differ from the given \n
                 profile's RX gain. \n
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  This field indicates the exact RF frequencies inside the profile's RF band at \n
                 which to measure the required parameters. When each bit in this field is set, \n
                 the measurement at the corresponding RF frequency is enabled wrt the profile's \n
                 RF band. \n
                 Bit         RF frequency                            RF \n
                 number                                              name \n
                 0           Lowest RF frequency in profile's        RF1 \n
                             sweep bandwidth \n
                 1           Center RF frequency in profile's        RF2 \n
                             sweep bandwidth \n
                 2           Highest RF frequency in profile's       RF3 \n
                             sweep bandwidth \n
                 The RF Name column is mentioned here to set the convention for the purpose of \n
                 reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
    /**
     * @brief  This field indicates the TX channels that should be compared for gain and phase \n
                 balance. Setting the corresponding bit to 1 enables that channel for imbalance \n
                 measurement \n
                 Bit number     TX Channel \n
                     0          TX0 \n
                     1          TX1 \n
                     2          TX2 \n
     */
    rlUInt8_t  txEn;
    /**
     * @brief  This field indicates the RX channels that should be enabled for TX to RX loopback
               measurement. Setting the corresponding bit to 1 enables that channel for imbalance
               measurement.
               Bit   RX Channel
                0    RX0
                1    RX1
                2    RX2
                3    RX3
     */
    rlUInt8_t  rxEn;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
#else
    /**
     * @brief  This field indicates the exact RF frequencies inside the profile's RF band at \n
                 which to measure the required parameters. When each bit in this field is set, \n
                 the measurement at the corresponding RF frequency is enabled wrt the profile's \n
                 RF band. \n
                 Bit         RF frequency                            RF \n
                 number                                              name \n
                 0           Lowest RF frequency in profile's        RF1 \n
                             sweep bandwidth \n
                 1           Center RF frequency in profile's        RF2 \n
                             sweep bandwidth \n
                 2           Highest RF frequency in profile's       RF3 \n
                             sweep bandwidth \n
                 The RF Name column is mentioned here to set the convention for the purpose of \n
                 reporting and describing many monitoring packets. \n
     */
    rlUInt8_t  rfFreqBitMask;
    /**
     * @brief  This field indicates the Profile Index for which this monitoring configuration \n
                 applies. The TX settings corresponding to this profile index are used during \n
                 the monitoring. The RX gain used in this measurement may differ from the given \n
                 profile's RX gain. \n
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  This field indicates the RX channels that should be enabled for TX to RX loopback
               measurement. Setting the corresponding bit to 1 enables that channel for imbalance
               measurement.
               Bit   RX Channel
                0    RX0
                1    RX1
                2    RX2
                3    RX3
     */
    rlUInt8_t  rxEn;
    /**
     * @brief  This field indicates the TX channels that should be compared for gain and phase \n
                 balance. Setting the corresponding bit to 1 enables that channel for imbalance \n
                 measurement \n
                 Bit number     TX Channel \n
                     0          TX0 \n
                     1          TX1 \n
                     2          TX2 \n
     */
    rlUInt8_t  txEn;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
#endif
    /**
     * @brief  The magnitude of difference between measured TX powers across the enabled \n
                 channels at each enabled RF frequency is compared against this threshold. The \n
                 comparison result is part of the monitoring report message(Error bit is set if \n
                 the measurement is above this threshold). Before the comparison, the measured \n
                 gains for each RF and RX are adjusted by subtracting the offset given in the \n
                 TX_GAIN_MISMATCH_OFFSET_VALUE field. \n
                 1 LSB = 0.1dB, signed number \n
                 Valid range: 0 to 60 (0 to 6dB) \n
     */
    rlInt16_t txGainMismatchThresh;
    /**
     * @brief  The magnitude of measured TX phase mismatch across the enabled channels at each \n
                 enabled RF frequency is compared against this threshold. The comparison result \n
                 is part of the monitoring report message (Error bit is set if any measurement \n
                 is above this threshold). Before the comparison, the measured gains for each RF \n
                 and RX are adjusted by subtracting the offset given in the \n
                 TX_PHASE_MISMATCH_OFFSET_VALUE field. \n
                 1 LSB = 360(degree)/ 2^16 , unsigned number \n
                 Valid range: corresponding to 0 degree to 20 degree. \n
     */
    rlUInt16_t txPhaseMismatchThresh;
    /**
     * @brief  The offsets to be subtracted from the measured TX gain for each TX and RF before \n
                 the relevant threshold comparisons are given here. Byte numbers corresponding \n
                 to different RX and RF, in this field are here: \n
                         RF1      RF2        RF3 \n
                TX0     [1:0]    [7:6]      [13:12] \n
                TX1     [3:2]    [9:8]      [15:14] \n
                TX2     [5:4]    [11:10]    [17:16] \n
                 1 LSB = 0.1 dB \n
                 Only the entries of enabled RF Frequencies and enabled TX \n
                 channels are considered. \n
     */
    rlUInt16_t txGainMismatchOffsetVal[RL_TX_CNT][RL_MON_RF_FREQ_CNT];
    /**
     * @brief  The offsets to be subtracted from the measured TX phase for each TX and RF before \n
                 the  \n
                 relevant threshold comparisons are given here. Byte numbers corresponding to  \n
                 different RX and RF, in this field are here: \n
                         RF1      RF2      RF3 \n
                 TX0     [1:0]    [7:6]    [13:12] \n
                 TX1     [3:2]    [9:8]    [15:14] \n
                 TX2     [5:4]    [11:10]  [17:16] \n
                 1 LSB = 360(degree)/216. \n
                 Only the entries of enabled RF Frequencies and enabled TX channels \n
                 are considered. \n
     */
    rlUInt16_t txPhaseMismatchOffsetVal[RL_TX_CNT][RL_MON_RF_FREQ_CNT];
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved2;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved3;
} rlTxGainPhaseMismatchMonConf_t;

/*! \brief
* TX BPM monitoring configuration
*/
typedef struct rlTxBpmMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the Profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  This field indicates the phase shifter monitoring configuration. \n
               Bit     Definition \n
               5:0     Phase shifter monitoring increment value \n
                       1 LSB = 5.625 degree \n
               6       Phase shifter monitoring auto increment enabled. On each FTTI phase shift \n
                       value increment by mentioned increment value at bit 0:5 \n
               7       Phase shifter monitoring enabled \n       
     */
    rlUInt8_t  phaseShifterMonCnfg;
    /**
     * @brief  Phase1 of the phase shifter of TX which needs to be monitored
               1 LSB = 5.625 degree
     */
    rlUInt8_t  phaseShifterMon1;
    /**
     * @brief  Phase2 of the phase shifter of TX which needs to be monitored
               1 LSB = 5.625 degree
     */
    rlUInt8_t  phaseShifterMon2;
    /**
     * @brief  Value      Definition \n
                 0       Report is sent every monitoring period without threshold check \n
                 1       Report is send only upon a failure after checking for thresholds) \n
                 2       Report is sent every monitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  This field indicates the RX channels that should be enabled for TX to RX loopback
               measurement. Setting the corresponding bit to 1 enables that channel for imbalance
               measurement.
               Bit   RX Channel
                0    RX0
                1    RX1
                2    RX2
                3    RX3
     */
    rlUInt8_t  rxEn;
#else
    /**
     * @brief  This field indicates the phase shifter monitoring configuration. \n
               Bit     Definition \n
               5:0     Phase shifter monitoring increment value \n
                       1 LSB = 5.625 degree \n
               6       Phase shifter monitoring auto increment enabled. On each FTTI phase shift \n
                       value increment by mentioned increment value at bit 0:5 \n
               7       Phase shifter monitoring enabled \n       
     */
    rlUInt8_t  phaseShifterMonCnfg;
    /**
     * @brief  This field indicates the Profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  Phase2 of the phase shifter of TX which needs to be monitored
               1 LSB = 5.625 degree
     */
    rlUInt8_t  phaseShifterMon2;
    /**
     * @brief  Phase1 of the phase shifter of TX which needs to be monitored
               1 LSB = 5.625 degree
     */
    rlUInt8_t  phaseShifterMon1;
    /**
     * @brief  This field indicates the RX channels that should be enabled for TX to RX loopback
               measurement. Setting the corresponding bit to 1 enables that channel for imbalance
               measurement.
               Bit   RX Channel
                0    RX0
                1    RX1
                2    RX2
                3    RX3
     */
    rlUInt8_t  rxEn;
    /**
     * @brief  Value      Definition \n
                 0       Report is sent every monitoring period without threshold check \n
                 1       Report is send only upon a failure after checking for thresholds) \n
                 2       Report is sent every monitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
#endif
    /**
     * @brief  The deviation of the TX output phase difference between the two BPM settings from \n
                 the ideal 180o is compared against the threshold given here. The comparison \n
                 result is part of the monitoring report message (Error bit is set if the \n
                 measurement is lower than this threshold, with the units of both quantities \n
                 being the same). \n
                 1 LSB = 360(degree) /2^16. \n
                 Valid range: 0 degree to 20 degree \n
     */
    rlUInt16_t txBpmPhaseErrThresh;
    /**
     * @brief  The deviation of the TX output amplitude difference between the two BPM settings \n
                 is compared against the threshold given here. The comparison result is part of \n
                 the monitoring report message (Error bit is set if the measurement is lower \n
                 than this threshold, with the units of both quantities being the same). \n
                 1 LSB = 0.1 dB \n
                 Valid range: 0 to 60 (0 to 6dB) \n
     */
    rlUInt16_t txBpmAmplErrThresh;
    /**
     * @brief  Maximum threshold for the difference in the 2 configured phase shift values
               1 LSB = 5.625 degree
     */
    rlUInt16_t phaseShifterThreshMax;
    /**
    * @brief   Minimum threshold for the difference in the 2 configured phase shift values
               1 LSB = 5.625 degree
    */
    rlUInt16_t phaseShifterThreshMin;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved;
} rlTxBpmMonConf_t;

/*! \brief
* TX BPM monitoring configuration
*/
typedef struct rlAllTxBpmMonConf
{
    /**
     * @brief  Tx-0 BPM monitoring config
     */
    rlTxBpmMonConf_t *tx0BpmMonCfg;
    /**
     * @brief  Tx-1 BPM monitoring config
     */
    rlTxBpmMonConf_t *tx1BpmMonCfg;
    /**
     * @brief  Tx-2 BPM monitoring config
     */
    rlTxBpmMonConf_t *tx2BpmMonCfg;
}rlAllTxBpmMonConf_t;

/*! \brief
* Synthesizer frequency monitoring configuration
*/
typedef struct rlSynthFreqMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the Profile Index for which this configuration applies.
     */
    rlUInt8_t   profileIndx;
    /**
     * @brief  Value    Definition \n
                 0     Report is sent every monitoring period without threshold check \n
                 1     Report is send only upon a failure (after checking for thresholds) \n
                 2     Report is sent every monitoring period with threshold check \n
     */
    rlUInt8_t   reportMode;
#else
    /**
     * @brief  Value    Definition \n
                 0     Report is sent every monitoring period without threshold check \n
                 1     Report is send only upon a failure (after checking for thresholds) \n
                 2     Report is sent every monitoring period with threshold check \n
     */
    rlUInt8_t   reportMode;
    /**
     * @brief  This field indicates the Profile Index for which this configuration applies.
     */
    rlUInt8_t   profileIndx;
#endif
    /**
     * @brief  During the chirp, the error of the measured instantaneous chirp frequency w.r.t. \n
                 the desired value is continuously compared against this threshold. \n
                 The comparison result is part of the monitoring report message (Error bit is \n
                 set if the measurement is above this threshold, ever during the previous \n
                 monitoring period). \n
                 1 LSB = 10 kHz \n
                 Valid range: 0 to 10000 (0 to 100 MHz) \n
     */
    rlUInt16_t  freqErrThresh;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field determines when the monitoring starts in each \n
                 chirp relative to the start of the ramp. \n
                 1 LSB = 0.2us, signed number \n
                 Valid range: -25us to 25us \n
     */
    rlInt8_t    monStartTime;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t   reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t   reserved0;
    /**
     * @brief  This field determines when the monitoring starts in each \n
                 chirp relative to the start of the ramp. \n
                 1 LSB = 0.2us, signed number \n
                 Valid range: -25us to 25us \n
     */
    rlInt8_t    monStartTime;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t  reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlSynthFreqMonConf_t;

/*! \brief
* External analog signals monitoring configuration
*/
typedef struct rlExtAnaSignalsMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value    Definition \n
               0       Report is sent every monitoring period without threshold check \n
               1       Report is send only upon a failure (after checking for thresholds) \n
               2       Report is sent every monitoring period with threshold check \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  This field indicates the sets of externally fed DC signals which are to be \n
                 monitored using GPADC. When each bit in this field is set, the corresponding \n
                 signal is monitored. The monitored signals are compared against programmed \n
                 limits. The comparison result is part of the monitoring report message. \n
                 Bit Location  SIGNAL \n
                     0         ANALOGTEST1 \n
                     1         ANALOGTEST2 \n
                     2         ANALOGTEST3 \n
                     3         ANALOGTEST4 \n
                     4         ANAMUX \n
                     5         VSENSE \n
                     Others    RESERVED \n
     */
    rlUInt8_t  signalInpEnables;
    /**
     * @brief  This field indicates the sets of externally fed DC signals which are to be \n
                 buffered before being fed to the GPADC. When each bit in this field is set, the \n
                 corresponding signal is buffered before the GPADC. The monitored signals are \n
                 compared against programmed limits. The comparison result is part of the \n
                 monitoring report message. \n
                 Bit      SIGNAL \n
                 0        ANALOGTEST1 \n
                 1        ANALOGTEST2 \n
                 2        ANALOGTEST3 \n
                 3        ANALOGTEST4 \n
                 4        ANAMUX \n
                 Others   RESERVED \n
     */
    rlUInt8_t  signalBuffEnables;
    /**
     * @brief  After connecting an external signal to the GPADC, the amount of time to wait for \n
                 it to settle before taking GPADC samples is programmed in this field. For each \n
                 signal, after that settling time, GPADC measurements take place for 6.4us \n
                 (averaging 4 samples of the GPADC output).The byte locations of the settling \n
                 times for each signal are tabulated here: \n
                 Byte Location   SIGNAL \n
                     0           ANALOGTEST1 \n
                     1           ANALOGTEST2 \n
                     2           ANALOGTEST3 \n
                     3           ANALOGTEST4 \n
                     4           ANAMUX \n
                     5           VSENSE \n
                     1 LSB = 0.8us \n
                 Valid range: 0 to 12us \n
                 Valid programming condition: all the signals that are enabled \n
                 should take a total of <100us, including the programmed settling \n
                 times and a fixed 6.4us of measurement time per enabled signal. \n
     */
    rlUInt8_t signalSettlingTime[6U];
    /**
     * @brief  The external DC signals measured on GPADC are compared against these minimum and \n
                 maximum thresholds. The comparison result is part of the monitoring report \n
                 message (Error bit is set if any measurement is outside this (minimum, maximum) \n
                 range). \n
                 Byte Location  Threshold     SIGNAL \n
                     0           Minimum      ANALOGTEST1 \n
                     1           Minimum      ANALOGTEST2 \n
                     2           Minimum      ANALOGTEST3 \n
                     3           Minimum      ANALOGTEST4 \n
                     4           Minimum      ANAMUX \n
                     5           Minimum      VSENSE \n
                     6           Maximum      ANALOGTEST1 \n
                     7           Maximum      ANALOGTEST2 \n
                     8           Maximum      ANALOGTEST3 \n
                     9           Maximum      ANALOGTEST4 \n
                     10          Maximum      ANAMUX \n
                     11          Maximum      VSENSE \n
                 1 LSB = 1.8V / 256 \n
                 Valid range: 0 to 255(0 to 1.79V) \n
     */
    rlUInt8_t signalThresh[12U];
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Value    Definition \n
               0       Report is sent every monitoring period without threshold check \n
               1       Report is send only upon a failure (after checking for thresholds) \n
               2       Report is sent every monitoring period with threshold check \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  This field indicates the sets of externally fed DC signals which are to be \n
                 buffered before being fed to the GPADC. When each bit in this field is set, \n
                 the corresponding signal is buffered before the GPADC. The monitored signals \n
                 are compared against programmed limits. The comparison result is part of the  \n
                 monitoring report message. \n
                 Bit      SIGNAL \n
                 0        ANALOGTEST1 \n
                 1        ANALOGTEST2 \n
                 2        ANALOGTEST3 \n
                 3        ANALOGTEST4 \n
                 4        ANAMUX \n
                 Others   RESERVED \n
     */
    rlUInt8_t  signalBuffEnables;
    /**
     * @brief  This field indicates the sets of externally fed DC signals which are to be \n
                 monitored using GPADC. When each bit in this field is set, the corresponding \n
                 signal is monitored. The monitored signals are compared against programmed \n
                 limits. The comparison result is part of the monitoring report message. \n
                 Bit Location  SIGNAL \n
                     0         ANALOGTEST1 \n
                     1         ANALOGTEST2 \n
                     2         ANALOGTEST3 \n
                     3         ANALOGTEST4 \n
                     4         ANAMUX \n
                     5         VSENSE \n
                     Others    RESERVED \n
     */
    rlUInt8_t  signalInpEnables;
    /**
     * @brief  After connecting an external signal to the GPADC, the amount of time to wait for \n
                 it to settle before taking GPADC samples is programmed in this field. For each \n
                 signal, after that settling time, GPADC measurements take place for 6.4us \n
                 (averaging 4 samples of the GPADC output).The byte locations of the settling \n
                 times for each signal are tabulated here: \n
                 Byte Location   SIGNAL \n
                     0           ANALOGTEST1 \n
                     1           ANALOGTEST2 \n
                     2           ANALOGTEST3 \n
                     3           ANALOGTEST4 \n
                     4           ANAMUX \n
                     5           VSENSE \n
                     1 LSB = 0.8us \n
                 Valid range: 0 to 12us \n
                 Valid programming condition: all the signals that are enabled \n
                 should take a total of <100us, including the programmed settling \n
                 times and a fixed 6.4us of measurement time per enabled signal. \n
     */
    rlUInt8_t signalSettlingTime[6U];
    /**
     * @brief  The external DC signals measured on GPADC are compared against these minimum and \n
                 maximum thresholds. The comparison result is part of the monitoring report \n
                 message (Error bit is set if any measurement is outside this (minimum, maximum) \n
                 range). \n
                 Byte Location  Threshold     SIGNAL \n
                     0           Minimum      ANALOGTEST1 \n
                     1           Minimum      ANALOGTEST2 \n
                     2           Minimum      ANALOGTEST3 \n
                     3           Minimum      ANALOGTEST4 \n
                     4           Minimum      ANAMUX \n
                     5           Minimum      VSENSE \n
                     6           Maximum      ANALOGTEST1 \n
                     7           Maximum      ANALOGTEST2 \n
                     8           Maximum      ANALOGTEST3 \n
                     9           Maximum      ANALOGTEST4 \n
                     10          Maximum      ANAMUX \n
                     11          Maximum      VSENSE \n
                 1 LSB = 1.8V / 256 \n
                 Valid range: 0 to 255 (0 to 1.79V) \n
     */
    rlUInt8_t signalThresh[12U];
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved3;
} rlExtAnaSignalsMonConf_t;

/*! \brief
* Internal signals in the TX path monitoring configuration
*/
typedef struct rlTxIntAnaSignalsMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  The RF analog settings corresponding to this profile are used for monitoring the \n
                 enabled signals, using test chirps (static frequency, at the center of the \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure(after checking for thresholds) \n
                 2    Report is sent every monitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
#else
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure(after checking for thresholds) \n
                 2    Report is sent every monitoring period with threshold check. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  The RF analog settings corresponding to this profile are used for monitoring the \n
                 enabled signals, using test chirps (static frequency, at the center of the \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t  profileIndx;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t  reserved0;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t  reserved1;
} rlTxIntAnaSignalsMonConf_t;


/*! \brief
* Internal signals in the TX path monitoring configuration
*/
typedef struct rlAllTxIntAnaSignalsMonConf
{
    /**
     * @brief  Internal signals in the Tx-0 path monitoring config
     */
    rlTxIntAnaSignalsMonConf_t *tx0IntAnaSgnlMonCfg;
    /**
     * @brief  Internal signals in the Tx-1 path monitoring config
     */
    rlTxIntAnaSignalsMonConf_t *tx1IntAnaSgnlMonCfg;
    /**
     * @brief  Internal signals in the Tx-2 path monitoring config
     */
    rlTxIntAnaSignalsMonConf_t *tx2IntAnaSgnlMonCfg;
}rlAllTxIntAnaSignalsMonConf_t;

typedef struct rlRxIntAnaSignalsMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
      * @brief  The RF analog settings corresponding to this profile are used for monitoring the \n
                 enabled signals, using test chirps(static frequency,at the center of the \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t  reportMode;
#else
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t  reportMode;
    /**
      * @brief  The RF analog settings corresponding to this profile are used for monitoring \n
                 the enabled signals, using test chirps(static frequency,at the center of the \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t  profileIndx;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t  reserved0;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t  reserved1;
} rlRxIntAnaSignalsMonConf_t;

/*! \brief
* Internal signals for PM, CLK and LO monitoring configuration
*/
typedef struct rlPmClkLoIntAnaSignalsMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  The RF analog settings corresponding to this profile are used for monitoring the \n
                 enabled signals, using test chirps(static frequency, at the center of the  \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t  reportMode;

    /**
     * @brief  Value   Definition \n
                 0    20GHz sync monitoring disabled \n
                 1    SYNC_IN monitoring enabled \n
                 2    SYNC_OUT monitoring enabled \n
                 3    SYNC_CLKOUT monitoring enabled \n
     */
    rlUInt8_t  sync20GSigSel;

    /**
     * @brief  Minimum threshold for 20GHz monitoring\n
               1 LSB = 1 dBm
     */
    rlInt8_t   sync20GMinThresh;

    /**
     * @brief  Maximum threshold for 20GHz monitoring\n
               1 LSB = 1 dBm
     */
    rlInt8_t   sync20GMaxThresh;

    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
#else
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  The RF analog settings corresponding to this profile are used for monitoring the \n
                 enabled signals, using test chirps(static frequency, at the center of the \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t  profileIndx;

    /**
     * @brief  Minimum threshold for 20GHz monitoring\n
               1 LSB = 1 dBm
     */
    rlInt8_t   sync20GMinThresh;

    /**
     * @brief  Value   Definition \n
                 0    20GHz sync monitoring disabled \n
                 1    SYNC_IN monitoring enabled \n
                 2    SYNC_OUT monitoring enabled \n
                 3    SYNC_CLKOUT monitoring enabled \n
     */
    rlUInt8_t  sync20GSigSel;

    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;

    /**
     * @brief  Maximum threshold for 20GHz monitoring\n
               1 LSB = 1 dBm
     */
    rlInt8_t   sync20GMaxThresh;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t  reserved1;
} rlPmClkLoIntAnaSignalsMonConf_t;

/*! \brief
* Internal signals for GPADC monitoring configuration
*/
typedef struct rlGpadcIntAnaSignalsMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t reportMode;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
} rlGpadcIntAnaSignalsMonConf_t;

/*! \brief
* Internal signals for PLL control voltage monitoring configuration
*/
typedef struct rlPllContrlVoltMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking for thresholds) \n
                 2    Report is sent every monitoring period with threshold chec. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking for thresholds) \n
                 2    Report is sent every monitoring period with threshold chec. \n
     */
    rlUInt8_t  reportMode;
#endif
    /**
     * @brief  This field indicates the sets of signals which are to be monitored. When each bit \n
                in this field is set, the corresponding signal set is monitored using test \n
                chirps. Rest of the RF analog may not be ON during these test chirps. The APLL \n
                VCO control voltage can be monitored. The Synthesizer VCO control voltage for \n
                both VCO1 and VCO2 can be monitored, while operating at their respective \n
                minimum and maximum frequencies, and their respective VCO slope (Hz/V) can be \n
                monitored if both frequencies are enabled for that VCO. The monitored signals \n
                are compared against internally chosen valid limits. The comparison results are \n
                part of the monitoring \n
                report message. \n
                        Bit Location   SIGNAL \n
                            0          APLL_VCTRL \n
                            1          SYNTH_VCO1_VCTRL \n
                            2          SYNTH_VCO2_VCTRL \n
                          15:3         RESERVED \n
                The synthesizer VCO extreme frequencies are: \n
                Synthesizer VCO      Frequency Limits (Min, Max) \n
                    VCO1             (76GHz, 78GHz) \n
                    VCO2             (77GHz, 81GHz) \n
                Synthesizer measurements are done with TX switched off to avoid emissions. \n
                For the failure reporting, the internally chosen valid limits are (tentative): \n
                for the measured control voltage levels: 0.15V to 1.25V; \n
                for the synthesizer VCO slope: -20 to +20% of 1.1GHz/V for VCO2 and 0.55GHz/V \n
                for VCO1. \n
     */
    rlUInt16_t signalEnables;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved1;
} rlPllContrVoltMonConf_t;

/*! \brief
* Internal signals for DCC based clock monitoring configuration
*/
typedef struct rlDualClkCompMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief  Value   Definition \n
                 0    Report is sent every monitoring period without threshold check \n
                 1    Report is send only upon a failure (after checking internal thresholds) \n
     */
    rlUInt8_t  reportMode;
#endif
    /**
     * @brief  This field indicates which pairs of clocks to monitor. When a bit in the field is \n
                 set to 1, the firmware monitors the corresponding clock pair by deploying the \n
                 hardware's Dual Clock Comparator in the corresponding DCC mode. \n
                 Bit  CLOCK PAIR \n
                 0    0 \n
                 1    1 \n
                 2    2 \n
                 3    3 \n
                 4    4 \n
                 5    5 \n
                 15:6 RESERVED \n
                 The comparison results are part of the monitoring report message. The \n
                 definition of the clock pairs and their error thresholds for failure reporting \n
                 are given in the table below the message definition. \n
     */
    rlUInt16_t dccPairEnables;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved1;
} rlDualClkCompMonConf_t;

/*! \brief
* RX saturation monitoring configuration
*/
typedef struct rlRxSatMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
    /**
     * @brief  01 => Enable only the ADC saturation monitor \n
               11 => Enable both the ADC and IFA1 saturation monitors \n
     */
    rlUInt8_t  satMonSel;
#else
    /**
     * @brief  01 => Enable only the ADC saturation monitor \n
               11 => Enable both the ADC and IFA1 saturation monitors \n
     */
    rlUInt8_t  satMonSel;
    /**
     * @brief  This field indicates the profile Index for which this configuration applies.
     */
    rlUInt8_t  profileIndx;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved0;
    /**
     * @brief  It specifies the duration of each (primary) time slice. \n
                 1 LSB = 0.16us. \n
                 Valid range: 4 to floor(ADC sampling time us/0.16 us) \n
                 NOTES: The minimum allowed duration of each (primary) time slice is  \n
                 4 LSBs = 0.64us. Also, the maximum number of (primary) time slices that will \n
                 be monitored in a chirp is 64 so the recommendation is to set this value to \n
                 correspond to (ADC sampling time / 64). If the slice is smaller, such that the \n
                 ADC sampling time is longer than 64 primary slices,some regions of the valid \n
                 duration of a chirp may go un-monitored. \n
     */
    rlUInt16_t primarySliceDuration;
    /**
     * @brief  Number of (primary + secondary) time slices to monitor. \n
                 Valid range: 1 to 127 \n
                 NOTE: Together with SAT_MON_PRIMARY_TIME_SLICE_DURATION, this determines the \n
                 full duration of the ADC valid time that gets covered by the monitor \n
     */
    rlUInt16_t numSlices;
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field is applicable only for SAT_MON_MODE = 0 Masks RX channels used for \n
                 monitoring. In every slice, saturation counts for all unmasked channels are \n
                 added together, and the total is capped to 127. The 8 bits are mapped \n
                 (MSB->LSB) to: \n
                 [RX3Q, RX2Q, RX1Q, RX0Q, RX3I, RX2I, RX1I, RX0I] \n
                 00000000 => All channels unmasked \n
                 11111111 => All channels masked. \n
     */
    rlUInt8_t rxChannelMask;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved1;
#else
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved1;
    /**
     * @brief  This field is applicable only for SAT_MON_MODE = 0 Masks RX channels used for \n
                 monitoring. In every slice, saturation counts for all unmasked channels are \n
                 added together, and the total is capped to 127. The 8 bits are mapped \n
                 (MSB->LSB) to: \n
                 [RX3Q, RX2Q, RX1Q, RX0Q, RX3I, RX2I, RX1I, RX0I] \n
                 00000000 => All channels unmasked \n
                 11111111 => All channels masked. \n
     */
    rlUInt8_t rxChannelMask;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved2;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved3;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved4;
} rlRxSatMonConf_t;

/*! \brief
* Signal and image band energy monitoring configuration
*/
typedef struct rlSigImgMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  This field indicates the profile index for which this configuration applies.
     */
    rlUInt8_t profileIndx;
    /**
     * @brief  Number of (primary + secondary) slices to monitor Valid range: 1 to 127.
     */
    rlUInt8_t numSlices;
#else
    /**
     * @brief  Number of (primary + secondary) slices to monitor Valid range: 1 to 127.
     */
    rlUInt8_t numSlices;
    /**
     * @brief  This field indicates the profile index for which this configuration applies.
     */
    rlUInt8_t profileIndx;
#endif
    /**
     * @brief  This field specifies the number of samples constituting each time slice. The \n
                 minimum allowed value for this parameter is 4. \n
                 Valid range: 4 to NUM_ADC_SAMPLES \n
                 NOTE: The maximum number of (primary) time slices that will be monitored in a \n
                 chirp is 64, so our recommendation is that this value should at least equal \n
                 (NUM_ADC_SAMPLES / 64). If the slice is smaller, such that the number of ADC \n
                 samples per chirp is larger than 64 primary slices, some regions of the valid \n
                 duration of a chirp may go un-monitored. \n
     */
    rlUInt16_t timeSliceNumSamples;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved0;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved1;
} rlSigImgMonConf_t;

/*! \brief
* RX mixer input power monitoring configuration
*/
typedef struct rlRxMixInPwrMonConf
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  The RF analog settings corresponding to this profile are used for monitoring RX \n
                 mixer input power using test chirps (static frequency, at the center of the \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t profileIndx;
    /**
     * @brief    Indicates the desired reporting verbosity and threshold usage. \n
                 Value = 0    Report is sent every monitoring period without threshold check \n
                 Value = 1    Report is send only upon a failure (after checking for thresholds) \n
                 Value = 2    Report is sent every monitoring period with threshold check. \n
                 Other values: RESERVED. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  This field indicates if and which TX channels should be enabled while measuring \n
                 RX mixer input power. Setting a bit to 1 enables the corresponding TX channel. \n
                 Enabling a TX channel may help find reflection power while disabling may help \n
                 find interference power. \n
                   Bit number   TX Channel \n
                   0            TX0 \n
                   1            TX1 \n
                   2            TX2 \n
     */
    rlUInt8_t txEnable;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved0;
#else
    /**
     * @brief    Indicates the desired reporting verbosity and threshold usage. \n
                 Value = 0    Report is sent every monitoring period without threshold check \n
                 Value = 1    Report is send only upon a failure (after checking for thresholds) \n
                 Value = 2    Report is sent every monitoring period with threshold check. \n
                 Other values: RESERVED. \n
     */
    rlUInt8_t  reportMode;
    /**
     * @brief  The RF analog settings corresponding to this profile are used for monitoring RX \n
                 mixer input power using test chirps (static frequency, at the center of the \n
                 profile's RF frequency band). \n
     */
    rlUInt8_t profileIndx;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t reserved0;
    /**
     * @brief  This field indicates if and which TX channels should be enabled while measuring \n
                 RX mixer input power. Setting a bit to 1 enables the corresponding TX channel. \n
                 Enabling a TX channel may help find reflection power while disabling may help \n
                 find interference power. \n
                   Bit number   TX Channel \n
                   0            TX0 \n
                   1            TX1 \n
                   2            TX2 \n
     */
    rlUInt8_t txEnable;
#endif
    /**
     * @brief  The measured RX mixer input voltage swings during this monitoring is compared \n
                 against the minimum and maximum thresholds configured in this field. The
                 comparison result is part of the monitoring report message (Error bit is set if \n
                 any measurement is outside this (minimum, maximum) range). \n
                   Byte number  Threshold \n
                   0            Minimum Threshold \n
                   1            Maximum Threshold \n
                 Only the RX channels enabled in the static configuration APIs are monitored. \n
                 1 LSB = 1800mV/256, unsigned number \n
                 Valid range: 0 to 255 (0 to 1792.96 mV), maximum threshold >= minimum threshold \n
     */
    rlUInt16_t thresholds;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved1;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt32_t reserved2;
}rlRxMixInPwrMonConf_t;

/*! \brief
* RX signal and image band energy statistics
*/
typedef struct rlRfSigImgPowerCqData
{
    /**
     * @brief  Number of (primary + secondary) slices to monitor Valid range: 1 to 127.
     */
    rlUInt16_t numSlices;
    /**
    * @brief The signal band and image band are separated using a two-channel filter
             bank and the ADC sampling time duration is monitored in terms of primary and
             secondary time slices as configured using rlRfRxSigImgMonConfig.
             If Number of Slices configured in rlRfRxSigImgMonConfig is N, then number
             of Primary slices = N+1/2 and number of secondary slices = N-1/2 \n
             For each of the two bands (signal and image), for each time slice,
             the input-referred average power in the slice in negative dBm is
             recorded as an 8-bit unsigned number, with 1 LSB = -0.5 dBm.
             CQ data is stored in 16bit format as follows:
             Pi[1] Ps[1], Si[1] Ss[1], Pi[2] Ps[2], Si[2] Ss[2]......
             Pi[63] Ps[63], Si[63] Ss[63], Pi[64] Ps[64]
             Where,
             Pi = Primary Slice Image Band Power, 1LSB = -0.5dBm
             Ps = Primary Slice Signal Band Power, 1LSB = -0.5dBm
             Si = Secondary Slice Image Band Power, 1LSB = -0.5dBm
             Ss = Secondary Slice Signal Band Power, 1LSB = -0.5dBm

             This data is stored in CQ1 section of CQ RAM. If multiple chirps
             are defined, then this data is concatenated and stored in CQ RAM
             in ping pong manner

             Note: CQ0 section in CQ RAM will contain invalid data and the user should
             to ignore this.
    */
    rlUInt16_t sigImgPowerCqVal[RL_NUM_MON_SLICES_MAX];
}rlRfSigImgPowerCqData_t;

/*! \brief
* RX ADC and IF saturation information
*/
typedef struct rlRfRxSaturationCqData
{
    /**
     * @brief  Number of (primary + secondary) slices to monitor Valid range: 1 to 127.
     */
    rlUInt8_t numSlices;
    /**
    * @brief The analog to digital interface includes a 100 MHz bit stream indicating
             saturation events in the ADC/IF sections, for each channel. This one-bit
             indicator for each channel is monitored during the ADC sampling time
             duration in a time-sliced manner as defined in rlRfRxIfSatMonConfig.\n
             If Number of Slices configured in rlRfRxSigImgMonConfig is N, then number
             of Primary slices = (N+1)/2 and number of secondary slices = (N-1)/2 \n
             For each time slice, a saturation event count is recorded. This count
             is the sum of saturation event counts across all RX channels selected
             for monitoring, capped to a maximum count of 255 (8 bits).\n
             CQ data is stored in 16bit format as follows:
             P[1], S[1], P[2] S[2]......P[63], S[63], P[64]
             Where,
             P[n] = indicates the accumulated saturation count for all enabled RX
                    channels in primary slice n
             S[n] = indicates the accumulated saturation count for all enabled RX
                    channels in secondary slice n

             This data is stored in CQ2 section of CQ RAM. If multiple chirps
             are defined, then this data is concatenated and stored in CQ RAM
             in ping pong manner

             Note 1: CQ0 section in CQ RAM will contain invalid data.
             Note 2: This satCqVal data transfer happen through DMA not through SPI. So there
                     is no problem of endianess.
                     
    */
    rlUInt8_t satCqVal[RL_NUM_MON_SLICES_MAX];
}rlRfRxSaturationCqData_t;

typedef struct rlAnaFaultInj
{
#ifndef MMWL_BIG_ENDIAN
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief Primary Fault: RX Gain. This field indicates which RX RF sections should have \n
               fault injected. If the fault is enabled, the RX RF gain drops significantly. \n
               The fault can be used to cause significant gain change, inter-RX gain imbalance \n
               and an uncontrolled amount of inter-RX phase imbalance. \n
                Bit RX Channel  \n
                 0  RX0         \n
                 1  RX1         \n
                 2  RX2         \n
                 3  RX3         \n
                Others RESERVED \n
                For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxGainDrop;
    /**
     * @brief Primary Fault: RX Phase. This field indicates which RX channels should have \n
               fault injected. If the fault is enabled, the RX phase gets inverted. The fault \n
               can be used to cause a controlled amount (180o) of inter-RX phase imbalance. \n
                Bit RX Channel   \n
                 0  RX0          \n
                 1  RX1          \n
                 2  RX2          \n
                 3  RX3          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxPhInv;
    /**
     * @brief Primary Fault: RX Noise. This field indicates which RX channels should have fault \n
               injected. If the fault is enabled, the RX IFA square wave loopback paths are \n
               engaged to inject high noise at RX IFA input. The fault can be used to cause \n
               significant RX noise floor elevation. \n
                Bit RX Channel   \n
                 0  RX0          \n
                 1  RX1          \n
                 2  RX2          \n
                 3  RX3          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxHighNoise;
    /**
     * @brief Primary Fault: Cutoff frequencies of RX IFA HPF & LPF, IFA Gain. This field \n
               indicates which RX channels should have fault injected. If the fault is enabled, \n
               the RX IFA HPF cutoff frequency becomes very high (about 15MHz). The fault can be \n
               used to cause the measured inband IFA gain, HPF and LPF attenuations to vary from \n
               ideal expectations. \n
                Bit RX Channel   \n
                 0  RX0          \n
                 1  RX1          \n
                 2  RX2          \n
                 3  RX3          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault \n
               Note: during the execution of RX_IFSTAGE_MONITOR, the RX_HIGH_NOISE faults are \n
                     temporarily removed.
     */
    rlUInt8_t  rxIfStagesFault;
    /**
     * @brief Primary Fault: RX Mixer LO input swing reduction. This field indicates which RX \n
               channels should have fault injected. If the fault is enabled, the RX mixer LO \n
               input swing is significantly reduced. The fault is primarily expected to be \n
               detected by RX_INTERNAL_ANALOG_SIGNALS_MONITOR (under PWRDET_RX category). \n
                Bit Channel       \n
                 0   RX0 and RX1  \n
                 1   RX2 and RX3  \n
                 Others RESERVED  \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxLoAmpFault;
    /**
     * @brief Primary Fault: TX PA input signal generator turning off. This field indicates \n
               which TX channels should have fault injected. If the fault is enabled, the \n
               amplifier generating TX power amplifier's LO input signal is turned off. The \n
               fault is primarily expected to be detected by \n
               TX<n>_INTERNAL_ANALOG_SIGNALS_MONITOR (under DCBIAS category). \n
                Bit Channel     \n
                 0  TX0 and TX1 \n
                 1  TX2 (applicable only if available in the xWR device) \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  txLoAmpFault;
    /**
     * @brief Primary Fault: TX Gain (power). This field indicates which TX RF sections should \n
               have fault injected. If the fault is enabled, the TX RF gain drops significantly. \n
               The fault can be used to cause significant TX output power change, inter-TX gain \n
               imbalance and an uncontrolled amount of inter-TX phase imbalance. \n
                Bit TX Channel   \n
                 0  TX0          \n
                 1  TX1          \n
                 2  TX2          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  txGainDrop;
    /**
     * @brief Primary Fault: TX Phase. This field indicates if TX channels should have fault \n
               injected, along with some further programmability. If the fault is enabled, the \n
               TX BPM polarity (phase) is forced to a constant value as programmed. The fault \n
               can be used to cause a controlled amount (180 degree) of inter-TX phase imbalance \n
               as well as BPM functionality failure. \n
                Bit TX Channel     \n
                 0  TX fault (Common for all TX channels) \n
                 1  RESERVED       \n
                 2  RESERVED       \n
                 3  TX0 BPM VALUE  \n
                 4  TX1 BPM VALUE  \n
                 5  TX2 BPM VALUE  \n
                Others RESERVED    \n
                For each TX<n> BPM VALUE: Applicable only if TX FAULT is enabled. \n
                Value = 0: force TX<n> BPM polarity to 0 \n
                Value = 1: force TX<n> BPM polarity to 1.
     */
    rlUInt8_t  txPhInv;
    /**
     * @brief Primary Fault: Synthesizer Frequency. This field indicates which Synthesizer \n
               faults should be injected. SYNTH_VCO_OPENLOOP: If the fault is enabled, the \n
               synthesizer is forced in open loop mode with the VCO control voltage forced to \n
               a constant. In order to avoid out of band emissions in this faulty state, this \n
               fault is injected just before the PLL_CONTROL_VOLTAGE_MONITOR is executed and \n
               released just after its completion.
               SYNTH_FREQ_MON_OFFSET: If the fault is enabled, the synthesizer frequency \n
               monitor's ideal frequency ramp waveform is forced to be offset from the actual \n
               ramp waveform by a constant, causing monitoring to detect failures.
               Bit Enable Fault
                0  SYNTH_VCO_OPENLOOP
                1  SYNTH_FREQ_MON_OFFSET
                Others RESERVED
               For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  synthFault;
    /**
     * @brief This field indicates whether some LDO output voltage faults should be injected or
              not.\n
               Bit Enable Fault \n
               0   SUPPLY_LDO_RX_LODIST_FAULT \n
               Others RESERVED\ n
               SUPPLY_LDO_RX_LODIST_FAULT: if enabled, the RX LO distribution sub system's LDO \n
               output voltage is slightly changed compared to normal levels to cause \n
               INTERNAL_PMCLKLO_SIGNALS_MONITOR to detect failure (under SUPPLY category). \n
               For each bit, 1 = inject fault, 0 = remove injected fault \n
               Note: This fault injection is ineffective under LDO bypass condition.
     */
    rlUInt8_t  supplyLdoFault;
    /**
     * @brief This field indicates whether a few miscellaneous faults should be injected or not. \n
               Bit Enable Fault         \n
               0  GPADC_CLK_FREQ_FAULT  \n
               Others RESERVED          \n
               GPADC_CLK_FREQ_FAULT: if enabled, the GPADC clock frequency is slightly increased \n
               compared to normal usage to cause BSS DCC_CLOCK_FREQ_MONITOR to detect failure. \n
               For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  miscFault;
    /**
     * @brief This field indicates whether faults should be forced in the threshold comparisons \n
               in the software layer of some monitors. If a fault is enabled, the logic in the \n
               min-max threshold comparisons used for failure detection is inverted, causing a \n
               fault to be reported. During these faults, no hardware fault condition is  \n
               injected in the device.
                Bit Enable Fault
                0  EXTERNAL_ANALOG_SIGNALS_MONITOR
                1  GPADC_INTERNAL_SIGNALS_MONITOR
                Others RESERVED
                For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  miscThreshFault;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
#else
    /**
     * @brief Primary Fault: RX Gain. This field indicates which RX RF sections should have \n
               fault injected. If the fault is enabled, the RX RF gain drops significantly. \n
               The fault can be used to cause significant gain change, inter-RX gain imbalance \n
               and an uncontrolled amount of inter-RX phase imbalance. \n
                Bit RX Channel  \n
                 0  RX0         \n
                 1  RX1         \n
                 2  RX2         \n
                 3  RX3         \n
                Others RESERVED \n
                For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxGainDrop;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved0;
    /**
     * @brief Primary Fault: RX Noise. This field indicates which RX channels should have fault \n
               injected. If the fault is enabled, the RX IFA square wave loopback paths are \n
               engaged to inject high noise at RX IFA input. The fault can be used to cause \n
               significant RX noise floor elevation. \n
                Bit RX Channel   \n
                 0  RX0          \n
                 1  RX1          \n
                 2  RX2          \n
                 3  RX3          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxHighNoise;
    /**
     * @brief Primary Fault: RX Phase. This field indicates which RX channels should have \n
               fault injected. If the fault is enabled, the RX phase gets inverted. The fault \n
               can be used to cause a controlled amount (180o) of inter-RX phase imbalance. \n
                Bit RX Channel   \n
                 0  RX0          \n
                 1  RX1          \n
                 2  RX2          \n
                 3  RX3          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxPhInv;
    /**
     * @brief Primary Fault: RX Mixer LO input swing reduction. This field indicates which RX \n
               channels should have fault injected. If the fault is enabled, the RX mixer LO \n
               input swing is significantly reduced. The fault is primarily expected to be \n
               detected by RX_INTERNAL_ANALOG_SIGNALS_MONITOR (under PWRDET_RX category). \n
                Bit Channel       \n
                 0   RX0 and RX1  \n
                 1   RX2 and RX3  \n
                 Others RESERVED  \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  rxLoAmpFault;
    /**
     * @brief Primary Fault: Cutoff frequencies of RX IFA HPF & LPF, IFA Gain. This field \n
               indicates which RX channels should have fault injected. If the fault is enabled, \n
               the RX IFA HPF cutoff frequency becomes very high (about 15MHz). The fault can be \n
               used to cause the measured inband IFA gain, HPF and LPF attenuations to vary from \n
               ideal expectations. \n
                Bit RX Channel   \n
                 0  RX0          \n
                 1  RX1          \n
                 2  RX2          \n
                 3  RX3          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault \n
               Note: during the execution of RX_IFSTAGE_MONITOR, the RX_HIGH_NOISE faults are \n
                     temporarily removed.
     */
    rlUInt8_t  rxIfStagesFault;
    /**
     * @brief Primary Fault: TX Gain (power). This field indicates which TX RF sections should \n
               have fault injected. If the fault is enabled, the TX RF gain drops significantly. \n
               The fault can be used to cause significant TX output power change, inter-TX gain \n
               imbalance and an uncontrolled amount of inter-TX phase imbalance. \n
                Bit TX Channel   \n
                 0  TX0          \n
                 1  TX1          \n
                 2  TX2          \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  txGainDrop;
    /**
     * @brief Primary Fault: TX PA input signal generator turning off. This field indicates \n
               which TX channels should have fault injected. If the fault is enabled, the \n
               amplifier generating TX power amplifier's LO input signal is turned off. The \n
               fault is primarily expected to be detected by \n
               TX<n>_INTERNAL_ANALOG_SIGNALS_MONITOR (under DCBIAS category). \n
                Bit Channel     \n
                 0  TX0 and TX1 \n
                 1  TX2 (applicable only if available in the xWR device) \n
                 Others RESERVED \n
                 For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  txLoAmpFault;
    /**
     * @brief Primary Fault: Synthesizer Frequency. This field indicates which Synthesizer \n
               faults should be injected. SYNTH_VCO_OPENLOOP: If the fault is enabled, the \n
               synthesizer is forced in open loop mode with the VCO control voltage forced to \n
               a constant. In order to avoid out of band emissions in this faulty state, this \n
               fault is injected just before the PLL_CONTROL_VOLTAGE_MONITOR is executed and \n
               released just after its completion.
               SYNTH_FREQ_MON_OFFSET: If the fault is enabled, the synthesizer frequency \n
               monitor's ideal frequency ramp waveform is forced to be offset from the actual \n
               ramp waveform by a constant, causing monitoring to detect failures.
               Bit Enable Fault
                0  SYNTH_VCO_OPENLOOP
                1  SYNTH_FREQ_MON_OFFSET
                Others RESERVED
               For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  synthFault;
    /**
     * @brief Primary Fault: TX Phase. This field indicates if TX channels should have fault \n
               injected, along with some further programmability. If the fault is enabled, the \n
               TX BPM polarity (phase) is forced to a constant value as programmed. The fault \n
               can be used to cause a controlled amount (180 degree) of inter-TX phase imbalance \n
               as well as BPM functionality failure. \n
                Bit TX Channel     \n
                 0  TX fault (Common for all TX channels) \n
                 1  RESERVED       \n
                 2  RESERVED       \n
                 3  TX0 BPM VALUE  \n
                 4  TX1 BPM VALUE  \n
                 5  TX2 BPM VALUE  \n
                Others RESERVED    \n
                For each TX<n> BPM VALUE: Applicable only if TX FAULT is enabled. \n
                Value = 0: force TX<n> BPM polarity to 0 \n
                Value = 1: force TX<n> BPM polarity to 1.
     */
    rlUInt8_t  txPhInv;
    /**
     * @brief This field indicates whether a few miscellaneous faults should be injected or not. \n
               Bit Enable Fault         \n
               0  GPADC_CLK_FREQ_FAULT  \n
               Others RESERVED          \n
               GPADC_CLK_FREQ_FAULT: if enabled, the GPADC clock frequency is slightly increased \n
               compared to normal usage to cause BSS DCC_CLOCK_FREQ_MONITOR to detect failure. \n
               For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  miscFault;
    /**
     * @brief This field indicates whether some LDO o/p voltage faults should be injected or not.\n
               Bit Enable Fault \n
               0   SUPPLY_LDO_RX_LODIST_FAULT \n
               Others RESERVED\ n
               SUPPLY_LDO_RX_LODIST_FAULT: if enabled, the RX LO distribution sub system's LDO \n
               output voltage is slightly changed compared to normal levels to cause \n
               INTERNAL_PMCLKLO_SIGNALS_MONITOR to detect failure (under SUPPLY category). \n
               For each bit, 1 = inject fault, 0 = remove injected fault \n
     */
    rlUInt8_t  supplyLdoFault;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt8_t  reserved1;
    /**
     * @brief This field indicates whether faults should be forced in the threshold comparisons \n
               in the software layer of some monitors. If a fault is enabled, the logic in the \n
               min-max threshold comparisons used for failure detection is inverted, causing a \n
               fault to be reported. During these faults, no hardware fault condition is  \n
               injected in the device.
                Bit Enable Fault
                0  EXTERNAL_ANALOG_SIGNALS_MONITOR
                1  GPADC_INTERNAL_SIGNALS_MONITOR
                Others RESERVED
                For each bit, 1 = inject fault, 0 = remove injected fault
     */
    rlUInt8_t  miscThreshFault;
#endif
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved2;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved3;
    /**
     * @brief  Reserved for Future use
     */
    rlUInt16_t reserved4;
} rlAnaFaultInj_t;

/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */

 /**
 *  @defgroup Monitoring Monitoring
 *  @brief mmwave radar RF/Sensor Monitoring module
 *
 * mmWave Device monitoring can be configured through a set of APIs defined in
  this section.  Note that these APIs cover the RF/Analog related monitoring mechanisms.
  There are separate monitoring mechanisms for the digital logic (including the
  processor, memory, etc.) which are internal to the device and not explicitly
  enabled through these APIs.\n

  The monitoring APIs are structured as follows:\n
   -# There are common configuration APIs that control the overall periodicity of
      monitoring, as well as, enable/disable control for each monitoring mechanism.\n
   -# Then, for each monitoring mechanism there is an individual API to allow the
      application to set an appropriate threshold for declaring failure from that monitoring.\n
   -# Also, for each monitoring mechanism, there is an individual API to report
      soft (raw) values from that monitoring.\n
   -# The Raw, Failure or Periodic Monitoring report   are sent to application using
      asynchronous events.

 * Below is the list of Monitors and corresponding duration in microseconds \n

 <table>
<caption id="multi_row_0">Monitoring Duration</caption>
<tr><th>Monitors                      <th>Duration(us)
<tr><td>Temperature<td>200
<tr><td>RX gain phase (assumes 1 RF frequency)<td>1250
<tr><td>RX noise figure (assumes 1 RF frequency)<td>250
<tr><td>RX IF stage (assumes 1 RF frequency)<td>1000
<tr><td>TX power (assumes 1 TX, 1 RF frequency)<td>200
<tr><td>TX ballbreak (assumes 1 TX)<td>250
<tr><td>TX gain phase mismatch (assumes 1 TX, 1 RF frequency)<td>400
<tr><td>TX BPM<td>575
<tr><td>Synthesizer frequency <td>0
<tr><td>External analog signals (all 6 GPADC channels enabled)<td>150
<tr><td>TX Internal analog signals (assumes 1 TX)<td>200
<tr><td>RX internal analog signals<td>1700
<tr><td>PMCLKLO internal analog signals<td>400
<tr><td>GPADC internal signals<td>50
<tr><td>PLL control voltage <td>210
<tr><td>Dual clock comparator (assumes 6 clock comparators)<td>110
<tr><td>RX saturation detector <td>0
<tr><td>RX signal and image band monitor<td>0
<tr><td>RX mixer input power<td>350
<tr><td>DFE statistics<td>100
<tr><td>Report formatting<td>700
<tr><td>Watchdog idle period<td>10% of FTTI duration
</table>

 *
 *    Related Files
 *   - rl_monitoring.c
 *  @addtogroup Monitoring
 *  @{
 */

/* Digital Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfDigMonEnableConfig(rlUInt8_t deviceMap,
                                                 rlMonDigEnables_t* data);

/* Digital Monitoring Periodic Configuration */
MMWL_EXPORT rlReturnVal_t rlRfDigMonPeriodicConfig(rlUInt8_t deviceMap,
                                                   rlDigMonPeriodicConf_t* data);
/* Analog Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfAnaMonConfig(rlUInt8_t deviceMap,
                                           rlMonAnaEnables_t* data);
/* TemperatureSsensor  Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfTempMonConfig(rlUInt8_t deviceMap, rlTempMonConf_t* data);
/* RX Gain and Phase Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfRxGainPhMonConfig(rlUInt8_t deviceMap,
                                                rlRxGainPhaseMonConf_t* data);
/* RX Noise Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfRxNoiseMonConfig(rlUInt8_t deviceMap,
                                               rlRxNoiseMonConf_t* data);
/* RX IF Stage Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfRxIfStageMonConfig(rlUInt8_t deviceMap,
                                                 rlRxIfStageMonConf_t* data);
/* TX Power Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfTxPowrMonConfig(rlUInt8_t deviceMap,
                                              rlAllTxPowMonConf_t *data);
/* TX Ballbreak Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfTxBallbreakMonConfig(rlUInt8_t deviceMap,
                                                   rlAllTxBallBreakMonCfg_t* data);
/* TX Gain Phase Mismatch Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfTxGainPhaseMismatchMonConfig(rlUInt8_t deviceMap,
                                                           rlTxGainPhaseMismatchMonConf_t* data);
/* TX BPM Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfTxBpmMonConfig(rlUInt8_t deviceMap,
                                             rlAllTxBpmMonConf_t* data);
/* Synth Freq Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfSynthFreqMonConfig(rlUInt8_t deviceMap,
                                                 rlSynthFreqMonConf_t* data);
/* External Analog Signals Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfExtAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                     rlExtAnaSignalsMonConf_t* data);
/* TX Internal Analog Signals Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfTxIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                       rlAllTxIntAnaSignalsMonConf_t* data);
/* RX Internal Analog Signals Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfRxIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                       rlRxIntAnaSignalsMonConf_t* data);
/* PM, CLK, LO Internal Analog Signals Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfPmClkLoIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                           rlPmClkLoIntAnaSignalsMonConf_t* data);
/* GPADC Internal Analog Signals Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfGpadcIntAnaSignalsMonConfig(rlUInt8_t deviceMap,
                                                          rlGpadcIntAnaSignalsMonConf_t* data);
/* PLL Control Voltage Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfPllContrlVoltMonConfig(rlUInt8_t deviceMap,
                                                     rlPllContrVoltMonConf_t* data);
/* Dual Clock Comparator Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfDualClkCompMonConfig(rlUInt8_t deviceMap,
                                                   rlDualClkCompMonConf_t* data);
/* RX Saturation Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfRxIfSatMonConfig(rlUInt8_t deviceMap,
                                               rlRxSatMonConf_t* data);
/* RX Signal Image band Monitoring Configuration */
MMWL_EXPORT rlReturnVal_t rlRfRxSigImgMonConfig(rlUInt8_t deviceMap,
                                                rlSigImgMonConf_t* data);
/* RX mixer input power monitoring.Configuration */
MMWL_EXPORT rlReturnVal_t rlRfRxMixerInPwrConfig(rlUInt8_t deviceMap,
                                                 rlRxMixInPwrMonConf_t* data);
/* Analog fault injection Configuration */
MMWL_EXPORT rlReturnVal_t rlRfAnaFaultInjConfig(rlUInt8_t deviceMap,
                                                rlAnaFaultInj_t* data);
/*!
 Close the Doxygen group.
 @}
 */

#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF RL_MONITORING_H FILE
 */
