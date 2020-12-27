/****************************************************************************************
 * FileName     : rl_messages.h
 *
 * Description  : This file includes all the Messages that is communicated by the Radar APIs
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
#ifndef RL_MESSAGES_H
#define RL_MESSAGES_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************
 */

/****************************************************************************************
 * MACRO DEFINITIONS
 ****************************************************************************************
 */
#define RL_MAX_SB_IN_MSG                               (32U)
#define RL_GET_UNIQUE_SBID(x,y)                        (((x)* RL_MAX_SB_IN_MSG) + (y))
#define RL_GET_SBID_FROM_MSG(x,y)                      ((x) - (RL_MAX_SB_IN_MSG *(y)))
#define RL_GET_MSGID_FROM_SBID(x)                      ((x) / RL_MAX_SB_IN_MSG)
#define RL_GET_SBID_FROM_UNIQ_SBID(x)                  ((x) % RL_MAX_SB_IN_MSG)

#define RL_SBC_ID_INDEX                     (0U)     /* RHCP SB Id Intex */
#define RL_SBC_ID_SIZE                      (2U)     /* RHCP SB Id Size in Bytes */
#define RL_SBC_LEN_INDEX                    (RL_SBC_ID_INDEX + RL_SBC_ID_SIZE)
#define RL_SBC_LEN_SIZE                     (2U)     /* RHCP SB Length Size */
#define RL_SBC_PL_INDEX                     (RL_SBC_LEN_INDEX + RL_SBC_LEN_SIZE)
#define RL_MIN_SBC_LEN                      (RL_SBC_LEN_SIZE + RL_SBC_ID_SIZE)

/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */

/*! \brief
* Command Message ID : Unique ID of each Command Messages - max 8 bits (0 - 191)
*/
/* radarSS Msg IDs  */
#define RL_RF_RESP_ERROR_MSG              (0x00U)
#define RL_RF_RESERVED0_MSG               (0x01U)
#define RL_RF_RESERVED1_MSG               (0x02U)
#define RL_RF_RESERVED2_MSG               (0x03U)
#define RL_RF_STATIC_CONF_SET_MSG         (0x04U)
#define RL_RF_STATIC_CONF_GET_MSG         (0x05U)
#define RL_RF_INIT_MSG                    (0x06U)
#define RL_RF_RESERVED3_MSG               (0x07U)
#define RL_RF_DYNAMIC_CONF_SET_MSG        (0x08U)
#define RL_RF_DYNAMIC_CONF_GET_MSG        (0x09U)
#define RL_RF_FRAME_TRIG_MSG              (0x0AU)
#define RL_RF_RESERVED4_MSG               (0x0BU)
#define RL_RF_ADVANCED_FEATURES_SET_MSG   (0x0CU)
#define RL_RF_ADVANCED_FEATURES_GET_MSG   (0x0DU)
#define RL_RF_MONITORING_CONF_SET_MSG     (0x0EU)
#define RL_RF_MONITORING_CONF_GET_MSG     (0x0FU)
#define RL_RF_RESERVED5_MSG               (0x10U)
#define RL_RF_STATUS_GET_MSG              (0x11U)
#define RL_RF_RESERVED6_MSG               (0x12U)
#define RL_RF_MONITORING_REPO_GET_MSG     (0x13U)
#define RL_RF_RESERVED7_MSG               (0x14U)
#define RL_RF_RESERVED8_MSG               (0x15U)
#define RL_RF_MISC_CONF_SET_MSG           (0x16U)
#define RL_RF_MISC_CONF_GET_MSG           (0x17U)
#define RL_RF_ASYNC_EVENT_MSG             (0x80U)
#define RL_RF_ASYNC_EVENT_1_MSG           (0x81U)


/* DSS Msg IDs */
#define RL_DSP_RESERVED0_MSG              (0x100U)
#define RL_DSP_RESERVED1_MSG              (0x101U)
#define RL_DSP_CONFIG_SET_MSG             (0x102U)
#define RL_DSP_CONFIG_GET_MSG             (0x103U)
#define RL_DSP_RESERVED2_MSG              (0x104U)
#define RL_DSP_RESERVED3_MSG              (0x105U)
#define RL_DSP_CONFIG_APPLY_MSG           (0x106U)
#define RL_DSP_STATUS_GET_MSG             (0x107U)

#define RL_DSP_ASYNC_EVENT_MSG            (0x180U)

/* MSS Msg IDs */
#define RL_DEV_POWERUP_MSG                (0x200U)
#define RL_DEV_RESERVED1_MSG              (0x201U)
#define RL_DEV_CONFIG_SET_MSG             (0x202U)
#define RL_DEV_CONFIG_GET_MSG             (0x203U)
#define RL_DEV_FILE_DOWNLOAD_MSG          (0x204U)
#define RL_DEV_RESERVED2_MSG              (0x205U)
#define RL_DEV_CONFIG_APPLY_MSG           (0x206U)
#define RL_DEV_STATUS_GET_MSG             (0x207U)
#define RL_DEV_MONITORING_CONF_SET_MSG    (0x208U)
#define RL_DEV_MONITORING_CONF_GET_MSG    (0x209U)
#define RL_DEV_RESERVED3_MSG              (0x20AU)
#define RL_DEV_MONITORING_REPORT_GET_MSG  (0x20BU)
#define RL_DEV_INTERNAL_CONF_SET_MSG      (0x20CU)
#define RL_DEV_INTERNAL_CONF_GET_MSG      (0x20DU)
#define RL_DEV_ASYNC_EVENT_MSG            (0x280U)


/*! \brief
* Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_RESP_ERROR_MSG
*/
#define RL_RF_RESP_ERROR_SB               (0x0U)
#define RL_RF_RESP_MAX                    (0x1U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_STATIC_CONF_SET_MSG/RL_RF_STATIC_CONF_GET_MSG
*/
#define RL_RF_CHAN_CONF_SB                  (0x0U)
#define RL_RF_RESERVED_SB                   (0x1U)
#define RL_RF_ADCOUT_CONF_SB                (0x2U)
#define RL_RF_LOWPOWERMODE_CONF_SB          (0x3U)
#define RL_RF_DYNAMICPOWERSAVE_CONF_SB      (0x4U)
#define RL_RF_HIGHSPEEDINTFCLK_CONF_SET_SB  (0x5U)
#define RL_RF_DEVICE_CFG_SB                 (0x6U)
#define RL_RF_RADAR_MISC_CTL_SB             (0x7U)
#define RL_RF_CAL_MON_FREQ_LIMIT_SB         (0x8U)
#define RL_RF_INIT_CALIB_CONF_SB            (0x9U)
#define RL_RF_TX_FREQ_PWR_LIMIT_SB          (0xAU)
#define RL_RF_CAL_DATA_RD_WR_SB             (0xBU)
#define RL_RF_PH_SHIFT_CAL_DATA_RD_WR_SB    (0xCU)
#define RL_RF_STAT_CONF_SB_MAX              (0xDU)


/*! \brief
* Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_INIT_MSG
*/
#define RL_RF_RF_INIT_SB                    (0x0U)
#define RL_RF_INIT_SB_MAX                   (0x1U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_DYNAMIC_CONF_SET_MSG/RL_RF_DYNAMIC_CONF_GET_MSG
*/
#define RL_RF_PROFILE_CONF_SB               (0x0U)
#define RL_RF_CHIRP_CONF_SB                 (0x1U)
#define RL_RF_FRAME_CONF_SB                 (0x2U)
#define RL_RF_CONT_STREAMING_MODE_CONF_SB   (0x3U)
#define RL_RF_CONT_STREAMING_MODE_EN_SB     (0x4U)
#define RL_RF_FRAME_CONF_ADVANCED_SB        (0x5U)
#define RL_RF_PERCHIRPPHASESHIFT_CONF_SB    (0x6U)
#define RL_RF_PROG_FILT_COEFF_RAM_SET_SB    (0x7U)
#define RL_RF_PROG_FILT_CONF_SET_SB         (0x8U)
#define RL_RF_CALIB_MON_TIME_UNIT_SB        (0x9U)
#define RL_RF_RUN_TIME_CALIB_CONF_TRIG_SB   (0xAU)
#define RL_RF_INTER_RX_GAIN_PH_FREQ_CTRL_SB (0xBU)
#define RL_RF_RX_GAIN_TEMPLUT_CONF_SB       (0xCU)
#define RL_RF_TX_GAIN_TEMPLUT_CONF_SB       (0xDU)
#define RL_RF_LB_BURST_CFG_SET_SB           (0xEU)
#define RL_RF_DYN_CHIRP_CFG_SET_SB          (0xFU)
#define RL_RF_DYN_PERCHIRP_PHSHFT_CFG_SET_SB (0x10U)
#define RL_RF_DYN_CHIRP_CFG_EN_SB           (0x11U)
#define RL_RF_INTERCHIRP_BLOCK_CTRL_SB      (0x12U)
#define RL_RF_SUBFRAME_START_SB             (0x13U)
#define RL_RF_RESERVED0                     (0x14U)
#define RL_RF_DYNAMIC_CONF_SB_MAX           (0x15U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_FRAME_TRIG_MSG
*/
#define RL_RF_FRAMESTARTSTOP_CONF_SB        (0x0U)
#define RL_RF_FRAME_TRIG_SB_MAX             (0x1U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_ADVANCED_FEATURES_SET_MSG/RL_RF_ADVANCED_FEATURES_GET_MSG
*/
#define RL_RF_BPM_COMMON_CONF_SB            (0x0U)
#define RL_RF_BPM_CHIRP_CONF_SB             (0x1U)
#define RL_RF_BPM_LFSR_CONF_SB              (0x2U)
#define RL_RF_BPM_PROGSEQ_CONF_SB           (0x3U)
#define RL_RF_INTERFERENCEDET_CONF_SB       (0x4U)
#define RL_RF_BUMPEREST_CONF_SB             (0x5U)
#define RL_RF_ADV_FEATURES_SB_MAX           (0x6U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_MONITORING_CONF_SET_MSG/RL_RF_MONITORING_CONF_GET_MSG
*/
#define RL_RF_MONITORING_CONF_SB            (0x0U)
#define RL_RF_MONIT_CONF_SB_MAX             (0x1U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_STATUS_GET_MSG
*/
#define RL_RF_RFVERSION_SB                  (0x0U)
#define RL_RF_RFCPUFAULT_STATUS_SB          (0x1U)
#define RL_RF_RFESMFAULT_STATUS_SB          (0x2U)
#define RL_RF_DIEID_STATUS_SB               (0x3U)
#define RL_RF_BOOTUP_BIST_STATUS_SB         (0x4U)
#define RL_RF_STATUS_SB_MAX                 (0x5U)

/*! \brief
* GET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_MONITORING_REPO_GET_MSG
*/
#define RL_RF_MONITORING_REPO_GET_SB         (0x0U)
#define RL_RF_MONITORING_REPO_GET_SB_MAX     (0x1U)

/*! \brief
* SET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_MONITORING_CONF_SET_MSG
*/
#define RL_RF_DIG_MON_EN_SB                 (0x0U)
#define RL_RF_DIG_MON_PERIODIC_CONF_SB      (0x1U)
#define RL_RF_ANA_MON_EN_SB                 (0x2U)
#define RL_RF_TEMP_MON_CONF_SB              (0x3U)
#define RL_RF_RX_GAIN_PHASE_MON_CONF_SB     (0x4U)
#define RL_RF_RX_NOISE_MON_CONF_SB          (0x5U)
#define RL_RF_RX_IFSTAGE_MON_CONF_SB        (0x6U)
#define RL_RF_TX0_POW_MON_CONF_SB           (0x7U)
#define RL_RF_TX1_POW_MON_CONF_SB           (0x8U)
#define RL_RF_TX2_POW_MON_CONF_SB           (0x9U)
#define RL_RF_TX0_BALLBREAK_MON_CONF_SB     (0xAU)
#define RL_RF_TX1_BALLBREAK_MON_CONF_SB     (0xBU)
#define RL_RF_TX2_BALLBREAK_MON_CONF_SB     (0xCU)
#define RL_RF_TX_GAIN_PHASE_MISMATCH_MON_CONF_SB  (0xDU)
#define RL_RF_TX0_BPM_MON_CONF_SB           (0xEU)
#define RL_RF_TX1_BPM_MON_CONF_SB           (0xFU)
#define RL_RF_TX2_BPM_MON_CONF_SB           (0x10U)
#define RL_RF_SYNTH_FREQ_MON_CONF_SB        (0x11U)
#define RL_RF_EXT_ANA_SIGNALS_MON_CONF_SB       (0x12U)
#define RL_RF_TX0_INT_ANA_SIGNALS_MON_CONF_SB   (0x13U)
#define RL_RF_TX1_INT_ANA_SIGNALS_MON_CONF_SB   (0x14U)
#define RL_RF_TX2_INT_ANA_SIGNALS_MON_CONF_SB   (0x15U)
#define RL_RF_RX_INT_ANA_SIGNALS_MON_CONF_SB    (0x16U)
#define RL_RF_PMCLKLO_INT_ANA_SIGNALS_MON_CONF_SB (0x17U)
#define RL_RF_GPADC_INT_ANA_SIGNALS_MON_CONF_SB (0x18U)
#define RL_RF_PLL_CONTROL_VOLT_MON_CONF_SB      (0x19U)
#define RL_RF_DUAL_CLOCK_COMP_MON_CONF_SB       (0x1AU)
#define RL_RF_RX_SATURATION_MON_CONF_SB         (0x1BU)
#define RL_RF_RX_SIGIMG_MON_CONF_SB             (0x1CU)
#define RL_RF_RX_MIXER_IN_POWR_CONF_SB          (0x1DU)
#define RL_RF_SYNTH_LIN_MON_CONF_SB             (0x1EU)
#define RL_RF_FAULT_INJECTION_CONF_SB           (0x1FU)

/*! \brief
* SET Sub-block ID :   - max 32 SBC
* MsgID : RL_RF_MISC_CONF_SET_MSG
*/
#define RL_RF_ORBIT_ENABLE_SB               (0x00U)
#define RL_RF_ORBIT_TEST_API_SB             (0x01U)
#define RL_RF_TEST_SOURCE_CONFIG_SET_SB     (0x02U)
#define RL_RF_TEST_SOURCE_ENABLE_SET_SB     (0x03U)
#define RL_RF_DEBUG_API_SB                  (0x04U)
#define RL_RF_REG_CONFIG_SET_SB             (0x05U)
#define RL_RF_TRIGGER_CALIB_MON_SB          (0x06U)
#define RL_RF_CALIB_DISABLE_SB              (0x07U)
#define RL_RF_MISC_CONF_SET_RESERVED_01     (0x08U)
#define RL_RF_MISC_CONF_SET_RESERVED_02     (0x09U)
#define RL_RF_CHAR_CONF_DYNAMIC_SET_SB      (0x0AU)
#define RL_RF_TEMP_SENS_TRIM_SET_SB         (0x0BU)
#define RL_RF_LDOBYPASS_SET_SB              (0x0CU)
#define RL_RF_PALOOPBACK_CFG_SET_SB         (0x0DU)
#define RL_RF_PSLOOPBACK_CFG_SET_SB         (0x0EU)
#define RL_RF_IFLOOPBACK_CFG_SET_SB         (0x0FU)
#define RL_RF_GPADC_CONF_SET_SB             (0x10U)
#define RL_RF_PD_TRIM_1GHZ_SET_SB           (0x11U)
#define RL_RF_MEAS_TX_POWER_SET_SB          (0x12U)
#define RL_RF_MEAS_PD_POWER_SET_SB          (0x13U)
#define RL_RF_MISC_CONF_SET_SB_MAX          (0x14U)

/*! \brief
* GET Sub-block ID : SB - max 32 SBC
* MsgID : RL_RF_MISC_CONF_GET_MSG
*/
#define  RL_RF_MISC_CONF_GET_RESERVED1      (0x00U)
#define  RL_RF_MISC_CONF_GET_RESERVED2      (0x01U)
#define  RL_RF_TEST_SOURCE_CONFIG_GET_SB    (0x02U)
#define  RL_RF_TEST_SOURCE_ENABLE_GET_SB    (0x03U)
#define  RL_RF_MISC_CONF_GET_RESERVED3      (0x04U)
#define  RL_RF_REG_CONFIG_GET_SB            (0x05U)
#define  RL_RF_MISC_CONF_GET_RESERVED4      (0x06U)
#define  RL_RF_MISC_CONF_GET_RESERVED5      (0x07U)
#define  RL_RF_MISC_CONF_GET_RESERVED6      (0x08U)
#define  RL_RF_CHAR_CONF_STATIC_GET_SB      (0x09U)
#define  RL_RF_CHAR_CONF_DYNAMIC_GET_SB     (0x0AU)
#define  RL_RF_MISC_CONF_GET_RESERVED7      (0x0BU)
#define  RL_RF_MISC_CONF_GET_SB_MAX         (0x0CU)


/*! \brief
* Sub-block ID :  - max 32 SBC
* MsgID : RL_RF_ASYNC_EVENT_MSG
*/
#define RL_RF_AE_RESERVED1                          (0x00U)
#define RL_RF_AE_RESERVED2                          (0x01U)
#define RL_RF_AE_CPUFAULT_SB                        (0x02U)
#define RL_RF_AE_ESMFAULT_SB                        (0x03U)
#define RL_RF_AE_INITCALIBSTATUS_SB                 (0x04U)
#define RL_RF_AE_RESERVED3                          (0x05U)
#define RL_RF_AE_RESERVED4                          (0x06U)
#define RL_RF_AE_RESERVED5                          (0x07U)
#define RL_RF_AE_RESERVED6                          (0x08U)
#define RL_RF_AE_RESERVED7                          (0x09U)
#define RL_RF_AE_RESERVED8                          (0x0AU)
#define RL_RF_AE_FRAME_TRIGGER_RDY_SB               (0x0BU)
#define RL_RF_AE_GPADC_MEAS_DATA_SB                 (0x0CU)
#define RL_RF_AE_RESERVED9                          (0x0DU)
#define RL_RF_AE_RESERVED10                         (0x0EU)
#define RL_RF_AE_FRAME_END_SB                       (0x0FU)
#define RL_RF_AE_ANALOG_FAULT_SB                    (0x10U)
#define RL_RF_AE_MON_TIMING_FAIL_REPORT_SB          (0x11U)
#define RL_RF_AE_RUN_TIME_CALIB_REPORT_SB           (0x12U)
#define RL_RF_AE_DIG_LATENTFAULT_REPORT_SB          (0x13U)
#define RL_RF_AE_MON_DFE_STATISTICS_SB              (0x14U)
#define RL_RF_AE_MON_REPORT_HEADER_SB               (0x15U)
#define RL_RF_AE_MON_DIG_PERIODIC_REPORT_SB         (0x16U)
#define RL_RF_AE_MON_TEMPERATURE_REPORT_SB          (0x17U)
#define RL_RF_AE_MON_RX_GAIN_PHASE_REPORT           (0X18U)
#define RL_RF_AE_MON_RX_NOISE_FIG_REPORT            (0X19U)
#define RL_RF_AE_MON_RX_IF_STAGE_REPORT             (0X1AU)
#define RL_RF_AE_MON_TX0_POWER_REPORT               (0X1BU)
#define RL_RF_AE_MON_TX1_POWER_REPORT               (0X1CU)
#define RL_RF_AE_MON_TX2_POWER_REPORT               (0X1DU)
#define RL_RF_AE_MON_TX0_BALLBREAK_REPORT           (0X1EU)
#define RL_RF_AE_MON_TX1_BALLBREAK_REPORT           (0X1FU)

/*! \brief
* Sub-block ID : - max 32 SBC
* MsgID : RL_RF_ASYNC_EVENT_1_MSG
*/
#define RL_RF_AE_MON_TX2_BALLBREAK_REPORT           (0X00U)
#define RL_RF_AE_MON_TX_GAIN_MISMATCH_REPORT        (0X01U)
#define RL_RF_AE_MON_TX0_BPM_REPORT                 (0X02U)
#define RL_RF_AE_MON_TX1_BPM_REPORT                 (0X03U)
#define RL_RF_AE_MON_TX2_BPM_REPORT                 (0X04U)
#define RL_RF_AE_MON_SYNTHESIZER_FREQ_REPORT        (0X05U)
#define RL_RF_AE_MON_EXT_ANALOG_SIG_REPORT          (0X06U)
#define RL_RF_AE_MON_TX0_INT_ANA_SIG_REPORT         (0X07U)
#define RL_RF_AE_MON_TX1_INT_ANA_SIG_REPORT         (0X08U)
#define RL_RF_AE_MON_TX2_INT_ANA_SIG_REPORT         (0X09U)
#define RL_RF_AE_MON_RX_INT_ANALOG_SIG_REPORT       (0X0AU)
#define RL_RF_AE_MON_PMCLKLO_INT_ANA_SIG_REPORT     (0X0BU)
#define RL_RF_AE_MON_GPADC_INT_ANA_SIG_REPORT       (0X0CU)
#define RL_RF_AE_MON_PLL_CONTROL_VOLT_REPORT        (0X0DU)
#define RL_RF_AE_MON_DCC_CLK_FREQ_REPORT            (0X0EU)
#define RL_RF_AE_MON_RESERVED0                      (0x0FU)
#define RL_RF_AE_MON_RESERVED1                      (0x10U)
#define RL_RF_AE_MON_RX_MIXER_IN_PWR_REPORT         (0x11U)
#define RL_RF_AE_SB_MAX                             (0x12U)


/*! \brief
* Unique ID of each SB - max 32 SBC
* MsgId - RL_DEV_POWERUP_MSG
*/
#define RL_SYS_RF_POWERUP_SB                (0x0U)
#define RL_SYS_NUM_OF_RF_POWERUP_SBC        (0x1U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID - RL_DEV_FILE_DOWNLOAD_MSG
*/
#define RL_SYS_FILE_DWLD_SB                 (0x0U)
#define RL_SYS_NUM_OF_FILE_DWLD_SBC         (0x1U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID - RL_DEV_CONFIG_SET_MSG
*/
#define RL_DEV_MCUCLOCK_CONF_SET_SB                (0x00U)
#define RL_DEV_RX_DATA_FORMAT_CONF_SET_SB          (0x01U)
#define RL_DEV_RX_DATA_PATH_CONF_SET_SB            (0x02U)
#define RL_DEV_DATA_PATH_LANEEN_SET_SB             (0x03U)
#define RL_DEV_DATA_PATH_CLOCK_SET_SB              (0x04U)
#define RL_DEV_DATA_PATH_CFG_SET_SB                (0x05U)
#define RL_DEV_RX_CONTSTREAMING_MODE_CONF_SET_SB   (0x06U)
#define RL_DEV_CSI2_CFG_SET_SB                     (0x07U)
#define RL_DEV_PMICCLOCK_CONF_SET_SB               (0x08U)
#define RL_DEV_PERIODICTESTS_CONF_SB               (0x09U)
#define RL_DEV_LATENTFAULT_TEST_CONF_SB            (0x0AU)
#define RL_DEV_TESTPATTERN_GEN_SET_SB              (0x0BU)
#define RL_DEV_MISC_CFG_SET_SB                     (0x0CU)
#define RL_DEV_CONFIG_SB_MAX                       (0x0DU)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID - RL_DEV_CONFIG_APPLY_MSG
*/
#define RL_SYS_CONFIG_APPLY_SB                      (0x0U)
#define RL_ADV_FRAME_DATA_CONFIG_SB                 (0x1U)
#define RL_SYS_CONFIG_APPLY_SB_MAX                  (0x2U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID - RL_DEV_STATUS_GET_MSG
*/
#define RL_SYS_VERSION_SB                           (0x0U)
#define RL_SYS_CPUFAULT_STATUS_SB                   (0x1U)
#define RL_SYS_ESMFAULT_STATUS_SB                   (0x2U)
#define RL_SYS_FRAME_CONFIG_GET_SB                  (0x3U)
#define RL_SYS_ADV_FRAME_CONFIG_GET_SB              (0x4U)
#define RL_SYS_NUM_OF_STATUS_SBC                    (0x5U)

/*! \brief
* Sub-block ID :   - max 32 SBC
* Async Event Sub-Blocks from MSS. MsgID - RL_DEV_ASYNC_EVENT_MSG
*/
#define RL_DEV_AE_MSSPOWERUPDONE_SB                 (0x0U)
#define RL_DEV_AE_RFPOWERUPDONE_SB                  (0x1U)
#define RL_DEV_AE_MSS_CPUFAULT_SB                   (0x2U)
#define RL_DEV_AE_MSS_ESMFAULT_SB                   (0x3U)
#define RL_DEV_AE_RESERVED0                         (0x4U)
#define RL_DEV_AE_MSS_BOOTERRSTATUS_SB              (0x5U)
#define RL_DEV_AE_MSS_LATENTFLT_TEST_REPORT_SB      (0x6U)
#define RL_DEV_AE_MSS_PERIODIC_TEST_STATUS_SB       (0x7U)
#define RL_DEV_AE_MSS_RF_ERROR_STATUS_SB            (0x8U)
#define RL_DEV_AE_MSS_VMON_ERROR_STATUS_SB          (0x9U)
#define RL_DEV_AE_MSS_ADC_DATA_SB                   (0xAU)
#define RL_DEV_AE_SB_MAX                            (0xBU)

/*! \brief
* rlSysAeFaultRepoMsgSbcId_t Sub-block ID :   - max 32 SBC
*/
#define RL_SYS_AE_DEV_CPUFAULT_SB              (0x0U)
#define RL_SYS_AE_DEV_ESMFAULT_SB              (0x1U)
#define RL_SYS_NUM_OF_AE_DEVFAULT_REPO_SBC     (0x2U)


/*! \brief
* Sub-block ID :   - max 32 SBC
* MsgID - RL_RF_MONITORING_REPO_GET_MSG
*/
#define RL_RF_DFE_STATISTICS_REPORT_GET_SB      (0x0U)

/*! \brief
* SET/GET Sub-block ID :   - max 32 SBC
* MsgID - RL_DEV_INTERNAL_CONF_SET_MSG, RL_DEV_INTERNAL_CONF_GET_MSG
*/
#define RL_DEV_MEM_REGISTER_SB                      (0x0U)
#define RL_DEV_NUM_OF_INTERN_CONF_SBC               (0x1U)


/*! \brief
* Response Error types : 16 bits
*/
#define RL_SYS_NRESP_INVLD_OPCODE               (0x0001U)    /* Invalid Opcode */
#define RL_SYS_NRESP_INVLD_MSGID                (0x0002U)    /* Invalid MsgID */
#define RL_SYS_NRESP_INVLD_NUM_SB               (0x0003U)    /* Invalid Num of SBC */
#define RL_SYS_NRESP_INVLD_SB_ID                (0x0004U)    /* Invalid SB ID */
#define RL_SYS_NRESP_INVLD_SB_LEN               (0x0005U)    /* Invalid SB Length */
#define RL_SYS_NRESP_SB_GET_ERROR               (0x0006U)    /* Error in Get SBC */
#define RL_SYS_NRESP_SB_SET_ERROR               (0x0007U)    /* Error in Set SBC */
#define RL_SYS_NRESP_SB_SET_INVL_DATA           (0x0008U)    /* Error in SBC DATA */
#define RL_SYS_NRESP_CRC_FAILED                 (0x0009U)    /* CRC of HDR+PAYLD mismatched */
#define RL_SYS_NRESP_FILETYPE_MISMATCHED        (0x000AU)    /* File type is mis-matched with
                                                                  magic num */
#define RL_SYS_NRESP_REG_WRITE_FAILED           (0x000BU)
#define RL_SYS_NRESP_ERROR_RESRVD               (0xFFFFU)    /* Reserved */

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */

#endif
/*
 * END OF RL_MESSAGES_H
 */

