/*******************************************************************************
 * @file    mmw_messages.h
 * @brief   This is the main common header file for both the MSS and DSS
 * @author  A. Astro, a.astro7x@gmail.com
 * @date    Jan 13 2020
 * @version 0.1
 *******************************************************************************/


#ifndef MMW_MESSAGES_H
#define MMW_MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Output packet length is a multiple of this value, must be power of 2*/
#define MMW_OUTPUT_MSG_SEGMENT_LEN 32

/*!
 * @brief
 *  Message types used in Millimeter Wave Demo for the communication between
 *  target and host, and also for Mailbox communication
 *  between MSS and DSS on the XWR16xx platform. Message types are used to indicate
 *  different type detection information sent out from the target.
 *
 */
typedef enum mmWaveMSG_OUT_TYPE_e {

    OUTPUT_MSG_DETECTED_POINTS = 1,     /*!< @brief   List of detected points */
    OUTPUT_MSG_RANGE_PROFILE,           /*!< @brief   Range profile */
    OUTPUT_MSG_NOISE_PROFILE,           /*!< @brief   Noise floor profile */
    OUTPUT_MSG_AZIMUT_STATIC_HEAT_MAP,  /*!< @brief   Samples to calculate static azimuth  heatmap */
    OUTPUT_MSG_RANGE_DOPPLER_HEAT_MAP,  /*!< @brief   Range/Doppler detection matrix */
    OUTPUT_MSG_STATS,                   /*!< @brief   Stats information */
    OUTPUT_MSG_MAX                      /*!< @brief   Max TLV outptut message */
} mmWaveMSG_OUT_TYPE;



/*!
 * @brief
 *  The structure defines the message header for reporting detection information from data path.
 *  Processed by both MSS and DSS
 *
 */
typedef struct mmWave_OUT_MSG_header_t {

    uint16_t    magicWord[4];            /*!< @brief   Output buffer magic word (sync word). It is initialized to {0x0102,0x0304,0x0506,0x0708} */
    uint32_t    version;                 /*!< @brief   Version: : MajorNum * 2^24 + MinorNum * 2^16 + BugfixNum * 2^8 + BuildNum   */
    uint32_t    totalPacketLen;          /*!< @brief   Total packet length including header in Bytes */
    uint32_t    platform;                /*!< @brief   platform type */
    uint32_t    frameNumber;             /*!< @brief   Frame number */
    uint32_t    timeCpuCycles;           /*!< @brief   Time in CPU cycles when the message was created. For XWR16xx/XWR18xx: DSP CPU cycles, for XWR14xx: R4F CPU cycles */
    uint32_t    numDetectedObj;          /*!< @brief   Number of detected objects */
    uint32_t    numTLVs;                 /*!< @brief   Number of TLVs */

#if (defined(SOC_XWR18XX) || defined(ENABLE_ADVANCED_FRAME))
    uint32_t    subFrameNumber;          /*!< @brief For frame config (not advanced), this is always set to 0. */
#endif
} mmWave_OUT_MSG_header;


/*!
 * @brief
 * Structure holds information about detected objects.
 * This information is sent in front of the array of detected objects
 * Sent by DSS
 */
typedef struct mmWave_OUT_MSG_stats_dataObjDescr_t {

    uint16_t     numDetetedObj; /*!< @brief   Number of detected objects */
    uint16_t     xyzQFormat;    /*!< @brief   Q format of detected objects x/y/z coordinates */

} mmWave_OUT_MSG_stats_dataObjDescr;


/**
 * @brief
 *  The structure defines the message body for  reporting
 *  detected objects from data path.
 *  Processed by both MSS and DSS
 */
typedef struct mmWave_OUT_MSG_tl_t {

    uint32_t    type;        /*!< @brief   TLV type */
    uint32_t    length;      /*!< @brief   Length in bytes */

} mmWave_OUT_MSG_tl;



#if 0

/*!
 * @brief
 * The structure holds stats information.
 * This is a payload of the TLV message item
 * that holds stats information from data path
 */

typedef struct mmWave_OUT_MSG_stats_t {

    uint32_t     interFrameProcessingTime;   /*!< @brief   Interframe processing time in usec */
    uint32_t     transmitOutputTime;         /*!< @brief   Transmission time of output detection informaion in usec */
    uint32_t     interFrameProcessingMargin; /*!< @brief   Interframe processing margin in usec */
    uint32_t     interChirpProcessingMargin; /*!< @brief   Interchirp processing margin in usec */
    uint32_t     activeFrameCPULoad;         /*!< @brief   CPU Load (%) during active frame duration */
    uint32_t     interFrameCPULoad;          /*!< @brief   CPU Load (%) during inter frame duration */

} mmWave_OUT_MSG_stats;
#endif



/**
 * @brief
 * The enum is used to hold all the messages types used for Mailbox communication
 * between MSS and DSS in mmw Demo.
 *
 */
typedef enum mbox_message_type_e 
{
    /*! @brief   message types for MSS to DSS communication */
    MBOX_MSS2DSS_GUIMON_CFG = 0xFEED0001,
    MBOX_MSS2DSS_CFAR_RANGE_CFG,
    MBOX_MSS2DSS_CFAR_DOPPLER_CFG,
    MBOX_MSS2DSS_PEAK_GROUPING_CFG,
    MBOX_MSS2DSS_MULTI_OBJ_BEAM_FORM,
    MBOX_MSS2DSS_CALIB_DC_RANGE_SIG,
    MBOX_MSS2DSS_DETOBJ_SHIPPED,
    MBOX_MSS2DSS_SET_DATALOGGER,
    MBOX_MSS2DSS_ADCBUFCFG,
    MBOX_MSS2DSS_EXTENDED_MAX_VELOCITY,
    MBOX_MSS2DSS_CLUTTER_REMOVAL,
    MBOX_MSS2DSS_COMP_RANGE_BIAS_AND_RX_CHAN_PHASE,
    MBOX_MSS2DSS_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE,

    /*! @brief   message types for DSS to MSS communication */
    MBOX_DSS2MSS_CONFIGDONE = 0xFEED0100,
    MBOX_DSS2MSS_DETOBJ_READY,
    MBOX_DSS2MSS_STOPDONE,
    MBOX_DSS2MSS_ASSERT_INFO,
    MBOX_DSS2MSS_ISR_INFO_ADDRESS,
    MBOX_DSS2MSS_MEASUREMENT_INFO
}mbox_message_type;


/*! @brief Software interrupt number used by DSS to signal exception from DSS to MSS */
#define MMW_DSS2MSS_EXCEPTION_SIGNALLING_SW_INT_DSS    1
/*! @brief Software interrupt ID on MSS corresponding to 
           @ref MMW_DSS2MSS_EXCEPTION_SIGNALLING_SW_INT_DSS */
#define MMW_DSS2MSS_EXCEPTION_SIGNALLING_SW_INT_MSS    SOC_XWR16XX_MSS_DSS2MSS_SW1_INT


 /*! @brief
  * Exception ID definitions for DSS to MSS urgent exception signalling through
  * software interrupt.
  * DSS to MSS chirp processing deadline miss exception ID
  */
#define MMW_DSS2MSS_CHIRP_PROC_DEADLINE_MISS_EXCEPTION 0

/*! @brief
 * Exception ID definitions for DSS to MSS urgent exception signalling through
 * software interrupt.
 * DSS to MSS frame processing deadline miss exception ID
 */
#define MMW_DSS2MSS_FRAME_PROC_DEADLINE_MISS_EXCEPTION 1



/**
 * @brief
 *  The structure describes the TLV part of the message from DSS to MSS
 *  on data path detection information.
 */
typedef struct mmWaveMSG_TLV_t {
    uint32_t    type;       /*!< @brief	Payload type */
    uint32_t    length;     /*!< @brief	Length in bytes */
    uint32_t    address;    /*!< @brief	Address of the payload */
} mmWaveMSG_TLV;

/**
 * @brief
 *   The structure defines the message body for for reporting detection information
 *   from data path to MSS
 */
typedef struct mmWave_detObjMsg_t {
    mmWave_OUT_MSG_header   header;                 /*!< @brief Header of the detection information message */
    mmWaveMSG_TLV           tlv[OUTPUT_MSG_MAX];    /*!< @brief TLVs of the detection information */
} mmWave_detInfoMsg;

#define MMWAVE_MAX_FILE_NAME_SIZE 128

/**
 * @brief
 *  The structure defines the message body for the information
 *  on a DSS exception that should be forwarded to the MSS reporting
 *  the DSS assertion information.
 */
typedef struct mmWave_dssAssertInfoMsg_t {
    char     file[MMWAVE_MAX_FILE_NAME_SIZE];       /*!< @brief file name */
    uint32_t line;                                  /*!< @brief line number */
}mmWave_dssAssertInfoMsg;

/**
 * @brief
 * The union defines the message body for various configuration messages.
 * For passing configuration from MSS to DSS.
 */
typedef union mmWaveMSG_body_u {

    mmWave_detInfoMsg       detObj;                 /*!< @brief   Detectection Information message */
    uint8_t                 dataLogger;             /*!< @brief   Datapath output logger setting */
    mmWave_dssAssertInfoMsg assertInfo;             /*!< @brief   DSS assertion information */
    uint32_t                dss2mssISRinfoAddress;  /*!< @brief   Address of DSS to MSS ISR information storage, typically in HSRAM */
}mmWaveMSG_body;

/*! @brief For advanced frame config, below define means the configuration given is
 * global at frame level and therefore it is broadcast to all sub-frames.
 */
#define MMW_SUBFRAME_NUM_FRAME_LEVEL_CONFIG (-1)

/**
 * @brief
 * The structure defines the message structure used for communication between MSS and DSS.
 */
typedef struct mmWaveMSG_t {

    int8_t                      subFrameNum;    /*!< @brief  Subframe number for which this message is applicable. */
    mbox_message_type           type;           /*!< @brief  message type */
    mmWaveMSG_body              body;           /*!< @brief  message body */

}mmWaveMSG;



#ifdef __cplusplus
}
#endif

#endif /* MMW_MESSAGES_H */
