/*
 * gen_frame_data.h
 *
 */

#ifndef GEN_FRAME_DATA_H_
#define GEN_FRAME_DATA_H_

/* BIOS/XDC Include Files. */
#include <xdc/std.h>

#include "test_data_path.h"

#define MAX_NUM_GEN_TARGETS 350

#define MAX_NUM_OBJECTS 350

/**
 * @brief
 *  Unit test scene configuration: single object parameters
 *
 */
typedef struct sceneObject_t_ {
    uint32_t amplitude;      /*!< @brief Beat signal amplitude */
    uint32_t azimPhyBin;     /*!< @brief Expected peak position in azimuth FFT output */
    uint32_t rngPhyBin;      /*!< @brief Expected peak position range FFT output */
    int32_t  doppPhyBin;     /*!< @brief Expected peak position Doppler FFT output */
    int32_t elevationDeg;    /*!< @brief Elevation in Degrees */
} sceneObject_t;

/**
 * @brief
 *  Unit test scene configuration: group of objects
 *
 */
typedef struct sceneConfig_t_ {
    int32_t  snrdB;                             /*!< @brief SNR in dB */
    uint32_t numTargets;                        /*!< @brief Number of objects in the scene */
    sceneObject_t sceneObj[MAX_NUM_OBJECTS];    /*!< @brief Array of scene objects */
} sceneConfig_t;

/*!
 *  @brief    Reference target parameters
 */
typedef struct targetObjStruc_t_
{
    /*! @brief Signal amplitude */
    float amplitude;
    /*! @brief  range phase */
    float rngPhy;
    /*! @brief  Doppler phase */
    float doppPhy;
    /*! @brief  azimuth phase */
    float azimPhy;
    /*! @brief  range phase increment */
    float rngPhyIncr;
    /*! @brief  Doppler phase increment */
    float doppPhyIncr;
    /*! @brief  Azimuth phase increment */
    float azimPhyIncr;
    /*! @brief  expected maximum position in range FFT */
    uint32_t rngPhyBin;
    /*! @brief  expected maximum position in Doppler FFT*/
    int32_t doppPhyBin;
    /*! @brief  expected maximum position in azimmuth FFT*/
    uint32_t azimPhyBin;
    /*! @brief Target elevation */
    float elevationPhy;
    /*! @brief 
      Variable used to store the azimuth phase of the non-elevation
      antennas that will be used in the elevation antenna due to the
      physical position of the elevation antenna with respect to the 
      non-elevation antennas.*/
    float azimPhyElevAntenna[4/*max num RX antennas*/];  
} targetObjStruc_t;

/*!
 *  @brief    Radar reference data generation parameters
 */
typedef struct genFrameStruc_t_
{
    /*! @brief Number of targets */
    uint32_t numTargets;
    /*! @brief chirp index */
    uint32_t chirpIndex;
    /*! @brief  chirp counter modulo number of Tx antennas */
    uint32_t chirpModNumTxAntCntr;
    /*! @brief  number of Tx antennas */
    uint32_t numTxAntennas;
    /*! @brief  Noise scale */
    float noiseScale;
    /*! @brief  snr in dB*/
    float snrdB;
    /*! @brief  array of reference target objects */
    targetObjStruc_t targetObj[MAX_NUM_GEN_TARGETS];
} genFrameStruc_t;


/**
 *  @b Description
 *  @n
 *      Initializes radar ADC reference data.
 *
 *  @param[in]  dataPathObj
 *      Pointer to data path object
 *
 *  @param[in]  genFrmDataObj
 *      Pointer to reference data object
 *
 *  @retval
 *      Not Applicable.
 */
void initFrameData (ObjectDetection_Test_SubFr_DataPathObj *dataPathObj,
                    genFrameStruc_t *genFrmDataObj,
                    sceneConfig_t *sceneConfig);

/**
 *  @b Description
 *  @n
 *      Generates one chirp of reference data into adc buffer. Function is called per chirp.
 *
 *  @param[in]  dataPathObj
 *      Pointer to data path object
 *
 *  @param[in]  genFrmDataObj
 *      Pointer to reference data object
 *
 *  @param[in]  outputBuffer
 *      Used in XWR14xx only. Pointer to buffer were chirp data will be written to.
 *
 *  @retval
 *      Not Applicable.
 */
void genChirpData (ObjectDetection_Test_SubFr_DataPathObj *obj,
                   genFrameStruc_t *frm,
                   uint8_t *outputBuffer);


#endif /* GEN_FRAME_DATA_H_ */
