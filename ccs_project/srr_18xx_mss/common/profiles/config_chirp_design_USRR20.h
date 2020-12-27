/*
 *   @file  srr_config_consts.h
 *
 *   @brief
 *      This file holds constants related to the USRR20 chirp configuration. 
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

/*! @brief Ultra short range chirp profile - 20 m range, 4.3cm resolution. better angular resolution,  approximately 18kmph max - vel. */
#define PROFILE_USRR_PROFILE_ID              (1U)
#define PROFILE_USRR_HPFCORNER_FREQ1_VAL     RL_RX_HPF1_175_KHz
#define PROFILE_USRR_HPFCORNER_FREQ2_VAL     RL_RX_HPF2_350_KHz
#define PROFILE_USRR_RX_GAIN_VAL             (30U)
#define PROFILE_USRR_DIGOUT_SAMPLERATE_VAL   (6222U)
#define PROFILE_USRR_ADC_SAMPLE_VAL          (512U)
#define PROFILE_USRR_IDLE_TIME_VAL           (700U)
#define PROFILE_USRR_RAMP_END_TIME_VAL       (8728U)
#define PROFILE_USRR_START_FREQ_GHZ          (77.01f)
#define PROFILE_USRR_START_FREQ_VAL          (CONV_FREQ_GHZ_TO_CODEWORD(PROFILE_USRR_START_FREQ_GHZ))
#define PROFILE_USRR_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_USRR_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_USRR_FREQ_SLOPE_MHZ_PER_US   (42.0f)
#define PROFILE_USRR_FREQ_SLOPE_VAL          (CONV_SLOPE_MHZ_PER_US_TO_CODEWORD(PROFILE_USRR_FREQ_SLOPE_MHZ_PER_US))
#define PROFILE_USRR_TX_START_TIME_VAL       (100U)  // 1us
#define PROFILE_USRR_ADC_START_TIME_VAL      (480U)  // 4.8 us
#define PROFILE_USRR_LAMBDA_MILLIMETER       (SPEED_OF_LIGHT_IN_METERS_PER_USEC/PROFILE_USRR_START_FREQ_GHZ)

#define CHIRP_USRR_0_PROFILE_ID                (1U)
#define CHIRP_USRR_0_START_INDEX               (256U + 0U)
#define CHIRP_USRR_0_END_INDEX                 (256U + 0U)
#define CHIRP_USRR_0_START_FREQ_VAL            (0U)
#define CHIRP_USRR_0_FREQ_SLOPE_VAL            (0U)
#define CHIRP_USRR_0_IDLE_TIME_VAL             (0U)
#define CHIRP_USRR_0_ADC_START_TIME_VAL        (0U)
#define CHIRP_USRR_0_TX_CHANNEL                (TX_CHANNEL_1_ENABLE)

#define CHIRP_USRR_1_PROFILE_ID                (1U)
#define CHIRP_USRR_1_START_INDEX               (256U + 1U)
#define CHIRP_USRR_1_END_INDEX                 (256U + 1U)
#define CHIRP_USRR_1_START_FREQ_VAL            (0U)
#define CHIRP_USRR_1_FREQ_SLOPE_VAL            (0U)
#define CHIRP_USRR_1_IDLE_TIME_VAL             (0U)
#define CHIRP_USRR_1_ADC_START_TIME_VAL        (0U)
#define CHIRP_USRR_1_TX_CHANNEL                (TX_CHANNEL_2_ENABLE)


#define CHIRP_USRR_2_PROFILE_ID                (1U)
#define CHIRP_USRR_2_START_INDEX               (256U + 2U)
#define CHIRP_USRR_2_END_INDEX                 (256U + 2U)
#define CHIRP_USRR_2_START_FREQ_VAL            (0U)
#define CHIRP_USRR_2_FREQ_SLOPE_VAL            (0U)
#define CHIRP_USRR_2_IDLE_TIME_VAL             (0U)
#define CHIRP_USRR_2_ADC_START_TIME_VAL        (0U)
#define CHIRP_USRR_2_TX_CHANNEL                (TX_CHANNEL_3_ENABLE)

/* SUBFRAME Configuration */
#define SUBFRAME_USRR_CHIRP_START_IDX           (CHIRP_USRR_0_START_INDEX)
#define SUBFRAME_USRR_CHIRP_END_IDX             (CHIRP_USRR_2_END_INDEX)
#define SUBFRAME_USRR_LOOP_COUNT                (32U)
#define SUBFRAME_USRR_PERIODICITY_VAL           (6000000U) // 30ms
#define SUBFRAME_USRR_TRIGGER_DELAY_VAL         (0U)
#define SUBFRAME_USRR_NUM_REAL_ADC_SAMPLES      (PROFILE_USRR_ADC_SAMPLE_VAL * 2)
#define SUBFRAME_USRR_NUM_CMPLX_ADC_SAMPLES     (PROFILE_USRR_ADC_SAMPLE_VAL)
#define SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS    ((CHIRP_USRR_0_END_INDEX - CHIRP_USRR_0_START_INDEX + 1)*SUBFRAME_USRR_LOOP_COUNT)
#define SUBFRAME_USRR_CHIRPTYPE_1_NUM_CHIRPS    ((CHIRP_USRR_1_END_INDEX - CHIRP_USRR_1_START_INDEX + 1)*SUBFRAME_USRR_LOOP_COUNT)
#define SUBFRAME_USRR_CHIRPTYPE_2_NUM_CHIRPS    ((CHIRP_USRR_2_END_INDEX - CHIRP_USRR_2_START_INDEX + 1)*SUBFRAME_USRR_LOOP_COUNT)

#define SUBFRAME_USRR_NUM_TX                    (3U)
#define SUBFRAME_USRR_NUM_VIRT_ANT                (SUBFRAME_USRR_NUM_TX*NUM_RX_CHANNELS)
#define SUBFRAME_USRR_NUM_ANGLE_BINS            (64U)
#define SUBFRAME_USRR_NUM_CHIRPS_TOTAL            ((SUBFRAME_USRR_CHIRP_END_IDX - SUBFRAME_USRR_CHIRP_START_IDX + 1) * SUBFRAME_USRR_LOOP_COUNT)

/*! @brief Derived parameters. */
#define PROFILE_USRR_RANGE_RESOLUTION_METERS        ((MMWDEMO_SPEED_OF_LIGHT_IN_METERS_PER_USEC * PROFILE_USRR_DIGOUT_SAMPLERATE_VAL)/ (2000.0f * PROFILE_USRR_FREQ_SLOPE_MHZ_PER_US * SUBFRAME_USRR_NUM_CMPLX_ADC_SAMPLES))

/* The USRR mode uses 3 Tx in TDM MIMO mode. As such every chirp repetition period will be the same. */
#define SUBFRAME_USRR_CHIRP_REPETITION_PERIOD_US ((( PROFILE_USRR_IDLE_TIME_VAL + PROFILE_USRR_RAMP_END_TIME_VAL)/100.0f))

#define SUBFRAME_USRR_VEL_RESOLUTION_M_P_S       (((1000.0f/(3.0f*SUBFRAME_USRR_CHIRP_REPETITION_PERIOD_US))/SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS)*(PROFILE_USRR_LAMBDA_MILLIMETER/2.0f))
#define SUBFRAME_USRR_MAX_VEL_M_P_S     (SUBFRAME_USRR_VEL_RESOLUTION_M_P_S*SUBFRAME_USRR_CHIRPTYPE_0_NUM_CHIRPS/2)

#define SUBFRAME_USRR_MIN_SNR_dB (22.0f)

#define SUBFRAME_USRR_NUM_CHIRPTYPES (3U)
