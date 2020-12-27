 /*
 * (C) Copyright 2016, Texas Instruments Incorporated -  http://www.ti.com/
 *-------------------------------------------------------------------------
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 *   @file     mmwavelib.h
 *
 *   @brief    Header file for mmwavelib library routines
 */

/** @mainpage MMWAVELIB library
*
*  The MMWAVELIB provides an optimized function library for basic radar-cube procesing.
*
*  The header file should be included in an application together with dsplib headers as follows:
*  @code
*  #include <ti/alg/mmwavelib/mmwavelib.h>
*  #include "gen_twiddle_fft32x32.h"
*  #include "gen_twiddle_fft16x16.h"
*  #include "DSP_fft16x16.h"
*  #include "DSP_fft32x32.h"
*  #include "DSPF_sp_fftSPxSP.h"
@endcode
*
*  ## Initialization: #
*  Function gen_twiddle_fft16x16 and gen_twiddle_fft32x32 shall be called.
*  
*  ============================================================================
*/
/**
 @defgroup MMWAVELIB_DETECT                  Detection
 @ingroup MMWAVELIB
 @brief
 *   The functions related to detecting objects
 */
 /**
 @defgroup MMWAVELIB_FFT                     FFT Utilities
 @ingroup MMWAVELIB
 @brief
 *   The functions facilitate stage wise FFT
 */

 /**
 @defgroup MMWAVELIB_VEC_UTILS               Vector Utilities 
 @ingroup MMWAVELIB
 @brief
 *   Vector arithmetic functions
 */

 /**
 @defgroup MMWAVELIB_FFT_FLOAT               Float-point FFT Utilities 
 @ingroup MMWAVELIB
 @brief
 *   The functions to facilitate floating point FFT
 */
 
 /**
 @defgroup MMWAVELIB_DETECT_FLOAT           Float-point Detection 
 @ingroup MMWAVELIB
 @brief
 *   The CFAR detection algorithm based on floating point input signal
 */
 
 /**
 @defgroup MMWAVELIB_AOAESTIMATION          Float-point AOA Estimation
 @ingroup MMWAVELIB
 @brief
 *   The AOA Beamformaing Estimation based on floating point input data
 */
 
 
 /**
 @defgroup MMWAVELIB_CLUSTERING            Clustering DBSCAN Algorithm
 @ingroup MMWAVELIB
 @brief
 *   The Clustering DBSCAN algorithm
 */
 

/**
 @defgroup MMWAVELIB_PEAKPRUNING           Cfar peak outputs group/pruning Algorithm
 @ingroup MMWAVELIB
 @brief
 *   Cfar peak group/pruning Algorithm
 */


 /**
 @defgroup MMWAVELIB_MAT_UTILS               Matrix Utilities 
 @ingroup MMWAVELIB
 @brief
 *   Matrix utility functions
 */
 
 
#ifndef MMWAVELIB_H
#define MMWAVELIB_H 1
#include <stdint.h>
#include <vect.h>

/* fixed point headers */
#include <ti/alg/mmwavelib/src/vecutils/mmwavelib_vecutils.h>
#include <ti/alg/mmwavelib/src/fft/mmwavelib_fft.h>
#include <ti/alg/mmwavelib/src/detection/mmwavelib_detection.h>
/* add fixed point clustering dbscan algorithm */
#include <ti/alg/mmwavelib/src/detection/mmwavelib_clustering.h>
#include <ti/alg/mmwavelib/src/detection/mmwavelib_peakpruning.h>

/* float point headers */
#include <ti/alg/mmwavelib/src/fft/mmwavelib_fft_float.h>
#include <ti/alg/mmwavelib/src/vecutils/mmwavelib_poweracc_float.h>
#include <ti/alg/mmwavelib/src/detection/mmwavelib_detection_float.h>
#include <ti/alg/mmwavelib/src/detection/mmwavelib_aoaest_float.h>

#include <ti/alg/mmwavelib/src/matutils/mmwavelib_matutils.h>

#endif /* MMWAVELIB_H */
