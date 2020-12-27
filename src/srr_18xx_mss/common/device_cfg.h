
#ifndef DEVICE_CFG_H
#define DEVICE_CFG_H

/* Tx Channel Configuration */
#define TX_CHANNEL_1_ENABLE                 (1U << 0U)
#define TX_CHANNEL_2_ENABLE                 (1U << 1U)
#define TX_CHANNEL_3_ENABLE                 (1U << 2U)
#define TX_CHANNEL_1_2_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE)
#define TX_CHANNEL_2_3_ENABLE               (TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_3_ENABLE               (TX_CHANNEL_1_ENABLE | TX_CHANNEL_3_ENABLE)
#define TX_CHANNEL_1_2_3_ENABLE             (TX_CHANNEL_1_ENABLE | TX_CHANNEL_2_ENABLE | TX_CHANNEL_3_ENABLE)

/* Rx Channel Configuration */
#define RX_CHANNEL_1_ENABLE                 (1U << 0U)
#define RX_CHANNEL_2_ENABLE                 (1U << 1U)
#define RX_CHANNEL_3_ENABLE                 (1U << 2U)
#define RX_CHANNEL_4_ENABLE                 (1U << 3U)
#define RX_CHANNEL_1_2_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE)
#define RX_CHANNEL_1_3_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_1_4_ENABLE               (RX_CHANNEL_1_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_2_3_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_4_ENABLE               (RX_CHANNEL_2_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_3_4_ENABLE               (RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE)
#define RX_CHANNEL_2_3_4_ENABLE             (RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_3_4_ENABLE             (RX_CHANNEL_1_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)
#define RX_CHANNEL_1_2_3_4_ENABLE           (RX_CHANNEL_1_ENABLE | RX_CHANNEL_2_ENABLE | RX_CHANNEL_3_ENABLE | RX_CHANNEL_4_ENABLE)

/* ADC Config Settings */
#define ADC_BITS_12                         (0U)
#define ADC_BITS_14                         (1U)
#define ADC_BITS_16                         (2U)

#define ADC_FORMAT_REAL                     (0U)
#define ADC_FORMAT_COMPLEX                  (1U)
#define ADC_FORMAT_CPMLEX_WITH_IMG_BAND     (2U)

#define ADC_I_FIRST                         (0U)
#define ADC_Q_FIRST                         (1U)

#define ADC_INTERLEAVED_MODE                (0U)
#define ADC_NON_INTERLEAVED_MODE            (1U)

/* Data Path Configuration */
#define DATA_PATH_CSI2                      (0U)
#define DATA_PATH_LVDS                      (1U)


#define DATA_PATH_FMT1_SUPRESS              (0U)
#define DATA_PATH_FMT1_CP_CQ                (1U)
#define DATA_PATH_FMT1_CQ_CP                (2U)

#define DATA_PATH_FMT0_ADC_DATA_ONLY        (0U)
#define DATA_PATH_FMT0_CP_ADC_DATA          (1U)
#define DATA_PATH_FMT0_ADC_CP_DATA          (2U)
#define DATA_PATH_FMT0_CP_ADC_CQ_DATA       (3U)

#define DATA_PATH_CQ_FMT_BITS_12            (0U)
#define DATA_PATH_CQ_FMT_BITS_14            (1U)
#define DATA_PATH_CQ_FMT_BITS_16            (2U)

/* LVDS Clock Configuration */
#define LVDS_LANE_CLOCK_SDR                 (0U)
#define LVDS_LANE_CLOCK_DDR                 (1U)

#define LVDS_ALL_LANE_EN                    (0xFU)

#define LVDS_DATA_RATE_900                  (0U)
#define LVDS_DATA_RATE_600                  (1U)
#define LVDS_DATA_RATE_450                  (2U)
#define LVDS_DATA_RATE_400                  (3U)
#define LVDS_DATA_RATE_300                  (4U)
#define LVDS_DATA_RATE_225                  (5U)
#define LVDS_DATA_RATE_150                  (6U)

/* LCDS Lane Configuration */
#define LVDS_LANE1_DISABLE                  (0U)
#define LVDS_LANE1_FORMAT_0                 (1U)
#define LVDS_LANE1_FORMAT_1                 (2U)

#define LVDS_LANE2_DISABLE                  (0U)
#define LVDS_LANE2_FORMAT_0                 (1U)
#define LVDS_LANE2_FORMAT_1                 (2U)

#define LVDS_LANE3_DISABLE                  (0U)
#define LVDS_LANE3_FORMAT_0                 (1U)
#define LVDS_LANE3_FORMAT_1                 (2U)

#define LVDS_LANE4_DISABLE                  (0U)
#define LVDS_LANE4_FORMAT_0                 (1U)
#define LVDS_LANE4_FORMAT_1                 (2U)

#define LVDS_LANE_MSB_FIRST_ENABLE          (1U)
#define LVDS_LANE_MSB_FIRST_DISABLE         (0U)
#define LVDS_LANE_PACKET_END_PULSE_ENABLE   (1U)
#define LVDS_LANE_PACKET_END_PULSE_DISABLE  (0U)
#define LVDS_LANE_CRC_ENABLE                (1U)
#define LVDS_LANE_CRC_DISABLE               (0U)
#define LVDS_LANE_TI_MODE_ENABLE            (1U)
#define LVDS_LANE_TI_MODE_DISABLE           (0U)

/* Advanced configuration */
#define ANA_CHANNEL_COMPLEX_CHAIN           (0U)
#define ANA_CHANNEL_REAL_CHAIN              (1U)

#define LP_ADC_MODE_REGULAR                 (0U)
#define LP_ADC_MODE_LOW_POWER               (1U)

#define NOISE_FIGURE_LOW                    (0U)
#define NOISE_FIGURE_HIGH                   (1U)

/* CHIRP Profile Settings */
#define CHIRP_HPF1_CORNER_FREQ_175K         (0U)
#define CHIRP_HPF1_CORNER_FREQ_235K         (1U)
#define CHIRP_HPF1_CORNER_FREQ_350K         (2U)
#define CHIRP_HPF1_CORNER_FREQ_700K         (3U)

#define CHIRP_HPF2_CORNER_FREQ_350K         (0U)
#define CHIRP_HPF2_CORNER_FREQ_700K         (1U)
#define CHIRP_HPF2_CORNER_FREQ_1_4M         (2U)
#define CHIRP_HPF2_CORNER_FREQ_2_8M         (3U)
#define CHIRP_HPF2_CORNER_FREQ_5M           (4U)
#define CHIRP_HPF2_CORNER_FREQ_7_5M         (5U)
#define CHIRP_HPF2_CORNER_FREQ_10M          (6U)
#define CHIRP_HPF2_CORNER_FREQ_15M          (7U)

/* Some MACROS to simplify programming the device */
#define ROUND_TO_INT32(X)    ((int32_t) (X))
#define CONV_FREQ_GHZ_TO_CODEWORD(X) ROUND_TO_INT32(X * (1.0e9/53.644))
#define CONV_SLOPE_MHZ_PER_US_TO_CODEWORD(X) (ROUND_TO_INT32(X * (1000.0/48.279)))

#define LOG2_APPROX(X) ((X <= 1)? 0:((X <= 2)? 1:((X <= 4)? 2:((X <= 8)? 3:((X <= 16)? 4:((X <= 32)? 5:((X <= 64)? 6:((X <= 128)? 7:((X <= 256)? 8:((X <= 512)? 9:((X <= 1024)? 10:11)))))))))))

#define SPEED_OF_LIGHT_IN_METERS_PER_SEC (3.0e8)

#define SPEED_OF_LIGHT_IN_METERS_PER_USEC (3.0e2) 


#endif

