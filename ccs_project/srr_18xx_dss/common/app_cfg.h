/**
 *   @file  device_cfg.h
 *   @brief
 *      This file holds constants related to the system chirp configuration
 *      as well as the calling the profiles configs  such as:
 *      config_chirp_design_USRR20.h
 *      config_chirp_design_USRR30.h
 *      To Add profiles, create the profile in the following path
 *      common/profiles/
 *      under the following name convention:
 *      config_chirp_design_XRRxx.h X
 *      where
 *      S:Short, M: Medium, US: Ultrashort .. etc
 *      xx is the number of chips within the frame.
 *
 */

#include "device_cfg.h"

/* The chirp design for the MRR TI design is as follows.
 *
 * The Radar operates in two modes - essentially two subfram-
 * es per frame, with alternate subframes belonging to one of
 * the two modes.
 * 1. A low range-resolution (30cm), high max range (80m),
 *      high max velocity(55kmph), poor angle resolution medium
 *      range radar (MRR) mode - called the MRR80 mode.
 * 2. A high range-resolution (4.5cm), low max range (20m)
 *    low max velocity (18kmph), high angle resolution ultra
 *    short range (USRR) mode - called the USRR20 mode.
 *
 * However the max velocity of 80m is not enough for the MRR
 * use case, and we need to facilitate algorithmic techniques
 * to improve it. We will create two kinds of chirp in the
 * MRR80 mode with have a max unambiguous velocity of 55kmph
 * and the other having a max unambiguous velocity of
 * (55/1.2) kmph, and then use the 'chinese remainder
 *  theorem' to estimate the true velocity.
 *
 * The MRR80 consists of one profile, two kinds of chirps,
 * with each chirp being repeated 64 times. These two sets of
 * chirps are processed seperately. The estimate of velocity
 * in one set, is compared with the estimate in the other set
 * to generate the true velocity. Only one tx is active in
 * this mode. i.e. No MIMO is used in MRR80.
 *
 * USRR20 consists of one profile (much higher BW MRR80), two
 * chirps (in a TDM-MIMO configuration), processed as a conventi-
 * nal two-tx four-rx capture. Some improvements to the max-
 * velocity is done so that we can still maintain a reasonable 
 * 36kmph max velocity.
 *
 * Since the chirp design of MRR80 and USRR20 are  different 
 * two different 'processing paths' are used to process them. 
 * 
 * USRR20 is processed by POINT_CLOUD_PROCESSING path and 
 * MRR80 is processed by the MAX_VEL_ENH_PROCESSING path.
 * 
 */

#ifndef MRR_CONFIG_CONSTS_H
#define MRR_CONFIG_CONSTS_H
#include <ti/common/sys_common.h>



/*! @brief Reduced Thresholds. 
 * The following line (if uncommented) reduces thresholds for the USRR detection algorithm 
 * to enable the detection of weak targets.  */
// #define LOW_THRESHOLD_FOR_USRR 
/*! @brief 120m max range. 
 * The MRR was initially designed for 80m, but with a slight configuration change will work at 120m (boresight). 
 * Uncomment the following line for enabling that config. */                     
//#define MRR_RANGE_120m


#define NUM_RX_CHANNELS                 (4U)
#define NUM_CHIRP_PROG                  (3U) /* 3TX antennas so Three for USRR20 */
#define NUM_PROFILES                    (1U) /* one profile for USRR mode of operation*/
#define NUM_SUBFRAMES                   (1U) /* one subframe for USRR mode operation: No advanced frame configuration  */

/** @brief  ENABLE USRR configuraion */
#define SUBFRAME_CONF_USRR



/*! @brief Number of chirps to be collected in the ADC buffer, before the chirp available interrupt. */
#define ADCBUFF_CHIRP_THRESHOLD             (1U)

/*! @brief Which subframe is used to do max-vel-enhancement. */
//#define MAX_VEL_ENH_SUBFRAME_IDX (0U)


/*! @brief Add in the USRR20 profile for > Max Range 20m; Better Range Resolution */
//#include "profiles/mrr_config_chirp_design_USRR20.h"
/*! @brief Add in the USRR30 profile for > Max Range 30m; Lower Range Resolution */
#include <common/profiles/config_chirp_design_USRR30.h>

#ifdef SUBFRAME_CONF_USRR
        #define FRAME_PERIODICITY_VAL (SUBFRAME_USRR_PERIODICITY_VAL)
#endif


/*! @brief The total frame periodicity in seconds. */
#define FRAME_PERIODICITY_SEC  (FRAME_PERIODICITY_VAL*5e-9)

/*! @brief The number of SNR Thresholds - used to vary the SNR requirement as a function of range. */
#define MAX_NUM_RANGE_DEPENDANT_SNR_THRESHOLDS (3U)

/*! @brief There are two processing paths in the application. */
#define MAX_VEL_ENH_PROCESSING (0U)
#define POINT_CLOUD_PROCESSING (1U)

/*! @brief The maximum number of clusters out of the dbscan algorithm (for the USRR subframe). */
#define MAX_NUM_CLUSTER_USRR (24U)

/*! @brief The maximum number of clusters out of the dbscan algorithm (for the MRR subframe). */
#define MAX_NUM_CLUSTER_MRR (32U)

/*! @brief The maximum number of tracked objects from the Kalman filter. */
#define MAX_TRK_OBJs (32U)

/*! @brief Fractional bit width for most of the report data (range, velocity, x, y, etc). */
#define REPORT_N_BIT_FRAC (7U)

/*! @brief Fractional bit width for Thresholds for CFAR data (rangeSNRdB, dopplerSNRdB, AzimSNR, etc). */
#define CFARTHRESHOLD_N_BIT_FRAC (8U) 

/*! @brief The radar's range estimate has a constant error due to the finite distance from the antenna to the LO. */ 
#define MIN_RANGE_OFFSET_METERS (0.075f)

/*! @brief Wait for MIN_TICK_FOR_TX before letting the tracker results out. */ 
#define MIN_TICK_FOR_TX (10U)

/*! @brief We discard objects at extreme angles (greater than 55 degrees) 
 * from the tracking procedure. */ 
#define SIN_55_DEGREES (0.8192f)

/*! @brief We discard objects with poor azimuth SNR from the tracking 
 * procedure. */ 
#define TRK_SIN_AZIM_THRESH (1.0f/256.0f)

/*! @brief In processing the max-velocity enhancement subframe we need to check for det matrix transfer only after
 *   the second set of chirps are processed. */ 
#define CHECK_FOR_DET_MATRIX_TX 1
#define DO_NOT_CHECK_FOR_DET_MATRIX_TX 0

/*! @brief  The maximum number of objects to be send out per frame. 
 *  This number is upper bounded by the transfer rate to the external device. */
#define MRR_MAX_OBJ_OUT 200


/*! @brief  The maximum number of objects detected in the 'Max velocity enhanced' processing path. 
 * Because of all the pruning, and higher thresholds, and lower resolution, 
 * fewer objects are detected in MAX_VEL_ENH_PROCESSING. */
#define MAX_DET_OBJECTS_RAW_MAX_VEL_ENH_PROCESSING 200      
/*! @brief  The maximum number of objects detected in the 'point cloud ' processing path. */
#define MAX_DET_OBJECTS_RAW_POINT_CLOUD_PROCESSING 900  
/*! @brief  The two peaks (from the 'fast chirp' and the 'slow chirp' should be within 2 dB. */
#define MAX_VEL_IMPROVEMENT_ASSOCIATION_THRESH_DB (3U)
/*! @brief  Convert the threshold to a CFAR magnitude difference. */
#define MAX_VEL_IMPROVEMENT_ASSOCIATION_THRESH ( (( 1U << CFARTHRESHOLD_N_BIT_FRAC) * NUM_RX_CHANNELS * MAX_VEL_IMPROVEMENT_ASSOCIATION_THRESH_DB) / 6U)
/*! @brief  Search across 2 bins in the slow chirp. */
#define MAX_VEL_IMPROVEMENT_NUM_SPREAD  (2U)
/*! @brief  Max velocity improvement of 3x. */
#define MAX_VEL_ENH_NUM_NYQUIST (2U)
/*! @brief  Restrict the number of detected objects per range-gate to 3. */
#define MAX_NUM_DET_PER_RANGE_GATE (3U)


/*! @brief Unused #defines?? THEY ARE USED */
#define FRAME_CHIRP_START_IDX           (0U)
#define FRAME_CHIRP_END_IDX             (1U)
#define FRAME_COUNT_VAL                 (0U)
#define FRAME_LOOP_COUNT                (64U)
#define FRAME_TRIGGER_DELAY_VAL         (0U)
#define FRAME_NUM_REAL_ADC_SAMPLES      (512U)
#define FRAME_NUM_CMPLX_ADC_SAMPLES     (256U)

/*! @brief There are two TPCCs available on the 18xx.  @{ */
#define EDMA_INSTANCE_A (0U)
#define EDMA_INSTANCE_B (1U)


/*! @brief EDMA allocation and configuration table for FFT processing of subframe 0 AKA: (The whole frame) */

#define MRR_SF0_EDMA_CH_1D_IN_PING      EDMA_TPCC0_REQ_FREE_0
#define MRR_SF0_EDMA_CH_1D_IN_PONG      EDMA_TPCC0_REQ_FREE_1
#define MRR_SF0_EDMA_CH_1D_OUT_PING     EDMA_TPCC0_REQ_FREE_2
#define MRR_SF0_EDMA_CH_1D_OUT_PONG     EDMA_TPCC0_REQ_FREE_3
#define MRR_SF0_EDMA_CH_2D_IN_PING      EDMA_TPCC0_REQ_FREE_4
#define MRR_SF0_EDMA_CH_2D_IN_PONG      EDMA_TPCC0_REQ_FREE_5
#define MRR_SF0_EDMA_CH_DET_MATRIX      EDMA_TPCC0_REQ_FREE_6
#define MRR_SF0_EDMA_CH_DET_MATRIX2     EDMA_TPCC0_REQ_FREE_7
#define MRR_SF0_EDMA_CH_3D_IN_PING      EDMA_TPCC0_REQ_FREE_8
#define MRR_SF0_EDMA_CH_3D_IN_PONG      EDMA_TPCC0_REQ_FREE_9


/* Use the faster EDMA_INSTANCE_A (TPCC0) in the DSS */
#define EDMA_INSTANCE_DSS EDMA_INSTANCE_A
#define EDMA_INSTANCE_MSS EDMA_INSTANCE_B


#define MRR_EDMA_TRIGGER_ENABLE  1
#define MRR_EDMA_TRIGGER_DISABLE 0


/*! @brief Flag to enable the max velocity enhancement in point cloud processing.*/


#define MAX_VEL_POINT_CLOUD_PROCESSING_IS_ENABLED    0
#endif
