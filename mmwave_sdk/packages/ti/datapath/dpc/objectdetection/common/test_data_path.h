#ifndef TEST_DATA_PATH_H_
#define TEST_DATA_PATH_H_

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* BIOS/XDC Include Files. */
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <ti/common/sys_common.h>
#include <ti/drivers/edma/edma.h>
#include <ti/drivers/esm/esm.h>
#include <ti/drivers/soc/soc.h>
#include <ti/control/dpm/dpm.h>
#include <ti/datapath/dpc/dpu/aoaproc/aoaprochwa.h>
#include <ti/datapath/dpc/objectdetection/objdethwa/objectdetection.h>
#include <ti/drivers/osal/SemaphoreP.h>
#include <ti/control/mmwavelink/mmwavelink.h>

/*!
 *  @brief    Detected object estimated parameters
 *
 */
typedef volatile struct ObjectDetection_Test_detectedObj_t
{
    uint16_t   rangeIdx;     /*!< @brief Range index */
    int16_t   dopplerIdx;   /*!< @brief Doppler index. Note that it is changed
                                 to signed integer in order to handle extended maximum velocity.
                                 Neagative values correspond to the object moving toward
                                 sensor, and positive values correspond to the
                                 object moving away from the sensor */
    uint8_t detObj2dAzimIdx; /*!< @brief Detected objects azimuth index for debugging */
    float detObjElevationAngle; /*!< @brief Detected object elevation angle for debugging */

    uint16_t  peakVal;      /*!< @brief Peak value */
    int16_t  x;             /*!< @brief x - coordinate in meters. Q format depends on the range resolution */
    int16_t  y;             /*!< @brief y - coordinate in meters. Q format depends on the range resolution */
    int16_t  z;             /*!< @brief z - coordinate in meters. Q format depends on the range resolution */
} ObjectDetection_Test_detectedObj;


/**
 * @brief
 *  Data Path Information per subframe
 *
 * @details
 *  The structure is used to hold all the relevant information for
 *  the data path per subframe
 */
typedef struct ObjectDetection_Test_SubFr_DataPathObj_t
{

    /*! @brief   MultiObjectBeamForming en/dis */
    bool    multiObjBeamFormingCfgEnabled;

    /*! @brief   Number of receive channels */
    uint32_t numRxAntennas;

    /*! @brief number of transmit antennas */
    uint32_t numTxAntennas;

    /*! @brief number of virtual antennas */
    uint32_t numVirtualAntennas;

    /*! @brief number of virtual azimuth antennas */
    uint32_t numVirtualAntAzim;

    /*! @brief number of virtual elevation antennas */
    uint32_t numVirtualAntElev;

    /*! @brief number of ADC samples */
    uint32_t numAdcSamples;

    /*! @brief  rxChan offset in ADCBuf in number of samples */
    uint32_t rxChanOffsetInSamples;

    /*! @brief Rx antenna samples in ADC buffer: 0-noninterleaved 1-interleaved */
    uint8_t isInterleavedSampInAdcBuf;

    /*! @brief (tx1+tx2)/(tx1-tx2) BPM format: 0-Disabled 1-Enabled */
    uint8_t isBpmEnabled;

    /*! @brief number of range bins */
    uint32_t numRangeBins;

    /*! @brief number of chirps per frame */
    uint32_t numChirpsPerFrame;

    /*! @brief number of Doppler chirps */
    uint32_t numDopplerChirps;

    /*! @brief number of Doppler bins */
    uint32_t numDopplerBins;

    /*! @brief number of angle bins */
    uint32_t numAngleBins;

    /*! Static configuration */
    DPC_ObjectDetection_StaticCfg staticCfg;

    /*! Dynamic configuration */
    DPC_ObjectDetection_DynCfg dynCfg;

    /*!< @brief  Sub-frame unit test results */
    DPC_ObjectDetection_ExecuteResult *result;

} ObjectDetection_Test_SubFr_DataPathObj;


/**
 * @brief
 *  Millimeter Wave Demo Data Path Information.
 *
 * @details
 *  The structure is used to hold all the relevant information for
 *  the data path.
 */
typedef struct ObjectDetection_Test_DataPathObj_t
{
    /*! @brief   Number of sub-frames */
    uint8_t numSubFrames;

    /*! @brief   Handle to the DPM Module */
    DPM_Handle                  dpmHandle;

    /*! @brief   Handle to the hardware accelerator */
    HWA_Handle                  hwaHandle;

    /*! @brief   Handle of the EDMA driver. */
    EDMA_Handle                 edmaHandle[EDMA_NUM_CC];

    /*! @brief pointer to ADC buffer */
    cmplx16ImRe_t *ADCdataBuf;

    /*! @brief Number of detected objects. */
    uint32_t numObjOut;

    /*!< @brief  Common configuration storage */
    DPC_ObjectDetection_PreStartCommonCfg commonCfg;

    /*!< @brief For notification to test task that the frame processing is completed  */
    SemaphoreP_Handle semaphoreHandle;


    ObjectDetection_Test_SubFr_DataPathObj testSubFrDataPathObj[RL_MAX_SUBFRAMES];

} ObjectDetection_Test_DataPathObj;


#endif /* TEST_DATA_PATH_H_ */
