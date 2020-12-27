/*
 * detect_obj.h
 *
 */

#ifndef DETECT_OBJ_H_
#define DETECT_OBJ_H_

/*! @brief Maximum number of detected objects by HWA. */
#define MMW_MAX_OBJ_OUT 100

/*! @brief Converts Doppler index to signed variable. Value greater than or equal
 *         half the Doppler FFT size will become negative.
 *         Needed for extended maximum velocity.
 */
#define DOPPLER_IDX_TO_SIGNED(_idx, _fftSize) ((_idx) < (_fftSize)/2 ? \
        ((int16_t) (_idx)) : ((int16_t) (_idx) - (int16_t) (_fftSize)))

/*! @brief Converts signed Doppler index to unsigned variable (zero to FFT size -1).
 */
#define DOPPLER_IDX_TO_UNSIGNED(_idx, _fftSize) ((_idx) & (_fftSize - 1))

/*!
 *  @brief    Detected object estimated parameters
 *
 */
typedef volatile struct MmwDemo_detectedObj_t
{
    uint16_t   rangeIdx;     /*!< @brief Range index */
    int16_t   dopplerIdx;   /*!< @brief Doppler index. Note that it is changed
                                 to signed integer in order to handle extended maximum velocity.
                                 Neagative values correspond to the object moving toward
                                 sensor, and positive values correspond to the
                                 object moving away from the sensor */
    uint16_t  peakVal;      /*!< @brief Peak value */
    int16_t  x;             /*!< @brief x - coordinate in meters. Q format depends on the range resolution */
    int16_t  y;             /*!< @brief y - coordinate in meters. Q format depends on the range resolution */
    int16_t  z;             /*!< @brief z - coordinate in meters. Q format depends on the range resolution */
} MmwDemo_detectedObj;


#endif /* DETECT_OBJ_H_ */
