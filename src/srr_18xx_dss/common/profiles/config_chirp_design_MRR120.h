/*
 *   @file  mrr_config_chirp_design_MRR120.h
 *
 *   @brief
 *      This file holds constants related to the MRR (120m) chirp configuration. 
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

 
 
/*! @brief  MRR profile ID. */ 
#define PROFILE_MRR_PROFILE_ID              (0U)
/*! @brief  HPF 1 corner frequency. */ 
#define PROFILE_MRR_HPFCORNER_FREQ1_VAL     RL_RX_HPF1_175_KHz
/*! @brief  HPF 2 corner frequency. */ 
#define PROFILE_MRR_HPFCORNER_FREQ2_VAL     RL_RX_HPF2_350_KHz
/*! @brief  Rx gain is kept at the maximum . */ 
#define PROFILE_MRR_RX_GAIN_VAL             (44U)
/*! @brief  ADC Output rate is 5Mhz. */ 
#define PROFILE_MRR_DIGOUT_SAMPLERATE_VAL   (4652U)
#define PROFILE_MRR_ADC_SAMPLE_VAL          (256U)
#define PROFILE_MRR_IDLE_TIME_VAL           (500U)
#define PROFILE_MRR_RAMP_END_TIME_VAL       (6000U)
#define PROFILE_MRR_START_FREQ_GHZ          (76.01f)
#define PROFILE_MRR_START_FREQ_VAL          (CONV_FREQ_GHZ_TO_CODEWORD(PROFILE_MRR_START_FREQ_GHZ))
#define PROFILE_MRR_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_MRR_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_MRR_FREQ_SLOPE_MHZ_PER_US   (4.0f)
#define PROFILE_MRR_FREQ_SLOPE_VAL          (CONV_SLOPE_MHZ_PER_US_TO_CODEWORD(PROFILE_MRR_FREQ_SLOPE_MHZ_PER_US))
#define PROFILE_MRR_TX_START_TIME_VAL       (100U)  // 1us
#define PROFILE_MRR_ADC_START_TIME_VAL      (480U)  // 5us

#define PROFILE_MRR_LAMBDA_MILLIMETER       (MMWDEMO_SPEED_OF_LIGHT_IN_METERS_PER_USEC/PROFILE_MRR_START_FREQ_GHZ)                 

/*! @brief Define 128 chirps,  the first 64 will have an idle time of
 * 3us, and the remaining 64 will have an idle time of 14.8us
 * (11.8us extra 'idle time') */
#define CHIRP_MRR_0_PROFILE_ID                (0U)
#define CHIRP_MRR_0_START_INDEX               (0U)
#define CHIRP_MRR_0_END_INDEX                 (127U)
#define CHIRP_MRR_0_START_FREQ_VAL            (0U)
#define CHIRP_MRR_0_FREQ_SLOPE_VAL            (0U)
#define CHIRP_MRR_0_IDLE_TIME_VAL             (0U)
#define CHIRP_MRR_0_ADC_START_TIME_VAL        (0U)
#define CHIRP_MRR_0_TX_CHANNEL                (TX_CHANNEL_1_ENABLE|TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)


#define CHIRP_MRR_1_PROFILE_ID                (0U)
#define CHIRP_MRR_1_START_INDEX               (128U)
#define CHIRP_MRR_1_END_INDEX                 (255U)
#define CHIRP_MRR_1_START_FREQ_VAL            (0U)
#define CHIRP_MRR_1_FREQ_SLOPE_VAL            (0U)
#define CHIRP_MRR_1_IDLE_TIME_VAL             (1300U)
#define CHIRP_MRR_1_ADC_START_TIME_VAL        (0U)
#define CHIRP_MRR_1_TX_CHANNEL                (TX_CHANNEL_1_ENABLE|TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)

/*! @brief  SUBFRAME configuration. */
#define SUBFRAME_MRR_CHIRP_START_IDX           (0U)
#define SUBFRAME_MRR_CHIRP_END_IDX             (255U)
#define SUBFRAME_MRR_LOOP_COUNT                (1U)
#define SUBFRAME_MRR_PERIODICITY_VAL           (6000000U) // 30ms
#define SUBFRAME_MRR_TRIGGER_DELAY_VAL         (0U)
#define SUBFRAME_MRR_NUM_REAL_ADC_SAMPLES      (PROFILE_MRR_ADC_SAMPLE_VAL * 2)
#define SUBFRAME_MRR_NUM_CMPLX_ADC_SAMPLES     (PROFILE_MRR_ADC_SAMPLE_VAL)
#define SUBFRAME_MRR_CHIRPTYPE_0_NUM_CHIRPS    ((CHIRP_MRR_0_END_INDEX - CHIRP_MRR_0_START_INDEX + 1)*SUBFRAME_MRR_LOOP_COUNT)
#define SUBFRAME_MRR_CHIRPTYPE_1_NUM_CHIRPS    ((CHIRP_MRR_1_END_INDEX - CHIRP_MRR_1_START_INDEX + 1)*SUBFRAME_MRR_LOOP_COUNT)
#define SUBFRAME_MRR_NUM_TX                    (1U)  //Three Tx simultaneous

#define SUBFRAME_MRR_NUM_VIRT_ANT                (SUBFRAME_MRR_NUM_TX*NUM_RX_CHANNELS)
#define SUBFRAME_MRR_NUM_ANGLE_BINS            (32U)
#define SUBFRAME_MRR_NUM_CHIRPS_TOTAL            ((SUBFRAME_MRR_CHIRP_END_IDX - SUBFRAME_MRR_CHIRP_START_IDX + 1) * SUBFRAME_MRR_LOOP_COUNT)

#define PROFILE_MRR_RANGE_RESOLUTION_METERS ((MMWDEMO_SPEED_OF_LIGHT_IN_METERS_PER_USEC * PROFILE_MRR_DIGOUT_SAMPLERATE_VAL)/ (2000.0f * PROFILE_MRR_FREQ_SLOPE_MHZ_PER_US * SUBFRAME_MRR_NUM_CMPLX_ADC_SAMPLES) )
    
#define SUBFRAME_MRR_CHIRPTYPE_0_CHIRP_REPETITION_PERIOD_US ((CHIRP_MRR_0_IDLE_TIME_VAL + PROFILE_MRR_IDLE_TIME_VAL + PROFILE_MRR_RAMP_END_TIME_VAL)/100.0f)
#define SUBFRAME_MRR_CHIRPTYPE_0_VEL_RESOLUTION_M_P_S       (((1000.0f/SUBFRAME_MRR_CHIRPTYPE_0_CHIRP_REPETITION_PERIOD_US)/SUBFRAME_MRR_CHIRPTYPE_0_NUM_CHIRPS)*(PROFILE_MRR_LAMBDA_MILLIMETER/2))
#define SUBFRAME_MRR_CHIRPTYPE_0_MAX_VEL_M_P_S     (SUBFRAME_MRR_CHIRPTYPE_0_VEL_RESOLUTION_M_P_S*SUBFRAME_MRR_CHIRPTYPE_0_NUM_CHIRPS/2)
#define INV_SUBFRAME_MRR_CHIRPTYPE_0_VEL_RESOLUTION_M_P_S       (1.0f/SUBFRAME_MRR_CHIRPTYPE_0_VEL_RESOLUTION_M_P_S)
    
#define SUBFRAME_MRR_CHIRPTYPE_1_CHIRP_REPETITION_PERIOD_US ((CHIRP_MRR_1_IDLE_TIME_VAL + PROFILE_MRR_IDLE_TIME_VAL + PROFILE_MRR_RAMP_END_TIME_VAL)/100.0f)
#define SUBFRAME_MRR_CHIRPTYPE_1_VEL_RESOLUTION_M_P_S       (((1000.0f/SUBFRAME_MRR_CHIRPTYPE_1_CHIRP_REPETITION_PERIOD_US)/SUBFRAME_MRR_CHIRPTYPE_1_NUM_CHIRPS)*(PROFILE_MRR_LAMBDA_MILLIMETER/2))
#define SUBFRAME_MRR_CHIRPTYPE_1_MAX_VEL_M_P_S     ((SUBFRAME_MRR_CHIRPTYPE_1_VEL_RESOLUTION_M_P_S*SUBFRAME_MRR_CHIRPTYPE_1_NUM_CHIRPS/2)
#define INV_SUBFRAME_MRR_CHIRPTYPE_1_VEL_RESOLUTION_M_P_S       (1.0f/SUBFRAME_MRR_CHIRPTYPE_1_VEL_RESOLUTION_M_P_S)

#define SUBFRAME_MRR_MIN_SNR_dB (14.0f)

#define SUBFRAME_MRR_NUM_CHIRPTYPES (2U)
